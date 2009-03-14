/* compiler.h

   Compiler for parsing Flash2 AVM2 ABC Actionscript

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2008/2009 Matthias Kramm <kramm@quiss.org>
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "tokenizer.h"
#include "files.h"
#include "parser.h"
#include "parser.tab.h"
#include "compiler.h"
#include "../os.h"
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif

/* flex/bison definitions */
extern int a3_parse();
extern int as3_lex();
extern int as3_lex_destroy();

void as3_setverbosity(int level)
{
    as3_verbosity=level;
}
void as3_add_include_dir(char*dir)
{
    add_include_dir(dir);
}

static char registry_initialized = 0;
static char parser_initialized = 0;

//#define STORE_TOKENS

//#define DEBUG
#define DEBUG if(0)

#ifdef STORE_TOKENS
static mem_t tokens;
#endif

int a3_lex()
{
    as3_tokencount++;
#ifdef STORE_TOKENS
    if(as3_pass==1) {
        int token = as3_lex();
        /* FIXME: current_file needs to be stored, too */
        mem_put(&tokens, &token, sizeof(token));
        mem_put(&tokens, &a3_lval, sizeof(a3_lval));
        mem_put(&tokens, &current_line, sizeof(current_line));
        mem_put(&tokens, &current_column, sizeof(current_column));
        return token;
    } else {
        int token;
        mem_get(&tokens, &token, sizeof(token));
        mem_get(&tokens, &a3_lval, sizeof(a3_lval));
        mem_get(&tokens, &current_line, sizeof(current_line));
        mem_get(&tokens, &current_column, sizeof(current_column));
        return token;
    }
#else
    return as3_lex();
#endif
}

typedef struct _compile_list {
    const char*name;
    const char*filename;
    struct _compile_list*next;
} compile_list_t;
static compile_list_t*compile_list=0;

static void as3_parse_file_or_array(int pass, const char*name, const char*filename, const void*mem, int length)
{
    if(!registry_initialized) {
        registry_initialized = 1;
        registry_init();
    }
    if(!parser_initialized) {
        parser_initialized = 1;
        initialize_parser();
#if defined(STORE_TOKENS)
        mem_init(&tokens);
#endif
    }
    as3_pass=pass;

    FILE*fi = 0;
    if(filename) {
        if(as3_pass==1 && !mem) {
            compile_list_t*c = rfx_calloc(sizeof(compile_list_t));
            c->next = compile_list;
            c->name = strdup(name);
            c->filename = strdup(filename);
            compile_list = c;
        }
        DEBUG printf("[pass %d] parse file %s %s\n", pass, name, filename);
        fi = enter_file2(name, filename, 0);
        as3_file_input(fi);
    } else {
        DEBUG printf("[pass %d] parse bytearray %s (%d bytes)\n", pass, name, length);
        enter_file(name, name, 0);
        as3_buffer_input((void*)mem, length);
    }

    as3_tokencount=0;
    initialize_file(name, filename);
    a3_parse();
    as3_lex_destroy();
    finish_file();
}

typedef struct _scheduled_file {
    char*name;
    char*filename;
    struct _scheduled_file*next;
} scheduled_file_t;

static scheduled_file_t*scheduled=0;
dict_t*scheduled_dict=0;

void as3_parse_scheduled(int pass)
{
    DEBUG printf("[pass %d] parse scheduled\n", pass);

    while(scheduled) {
        scheduled_file_t*s = scheduled;
        scheduled = 0;
        while(s) {
            scheduled_file_t*old = s;
            as3_parse_file_or_array(pass, s->name, s->filename, 0,0);
            s = s->next;

            free(old->filename);
            free(old->name);
            old->filename = old->name = 0;
            free(old);
        }
    }
    if(scheduled_dict) {
        dict_destroy(scheduled_dict);
        scheduled_dict=0;
    }
}

void as3_schedule_file(const char*name, const char*filename) 
{
    if(!scheduled_dict) {
        scheduled_dict = dict_new();
    }

    filename = normalize_path(filename);
    
    if(dict_contains(scheduled_dict, filename)) {
        return; //already processed
    } else {
        dict_put(scheduled_dict, filename, 0);
    }
    DEBUG printf("[pass %d] schedule %s %s\n", as3_pass, name, filename);

    NEW(scheduled_file_t, f);
    f->name = strdup(name);
    f->filename = strdup(filename);
    f->next = scheduled; // dfs
    scheduled = f;
}

void as3_parse_list()
{
    while(compile_list) {
        as3_parse_file_or_array(2, compile_list->name, compile_list->filename, 0,0);
        compile_list = compile_list->next;
    }
}

void as3_parse_bytearray(const char*name, const void*mem, int length)
{
    as3_pass = 1;
    as3_parse_file_or_array(1, name, 0, mem, length);
    as3_parse_scheduled(1);
    
    as3_pass = 2;
    as3_parse_file_or_array(2, name, 0, mem, length);
    as3_parse_list();
}

void as3_parse_file(const char*filename) 
{
    char*fullfilename = find_file(filename, 1);
    if(!fullfilename)
        return; // not found

    compile_list = 0;
    as3_pass = 1;
    as3_parse_file_or_array(1, filename, fullfilename, 0,0);
    as3_parse_scheduled(1);

    as3_pass = 2;
    as3_parse_list();

    free(fullfilename);
}

void as3_parse_directory(const char*dir)
{
    compile_list = 0;
    as3_pass=1;
    as3_schedule_directory(dir);
    if(!scheduled)
        as3_warning("Directory %s doesn't contain any ActionScript files", dir);
    as3_parse_scheduled(1);

    as3_pass=2;
    as3_parse_list();
}

char as3_schedule_directory(const char*dirname)
{
    DEBUG printf("[pass %d] schedule directory %s\n", as3_pass, dirname);
    char ok=0;
#ifdef HAVE_DIRENT_H
    include_dir_t*i = current_include_dirs;
    while(i) {
        char*fulldirname = concat_paths(i->path, dirname);
        DEBUG printf("[pass %d] ... %s\n", as3_pass, fulldirname);
        DIR*dir = opendir(fulldirname);
        if(dir) {
            ok = 1;
            struct dirent*ent;
            while(1) {
                ent = readdir(dir);
                if (!ent) 
                    break;
                char*name = ent->d_name;
                char type = 0;
                if(!name) continue;
                int l=strlen(name);
                if(l<4)
                    continue;
                if(strncasecmp(&name[l-3], ".as", 3)) 
                    continue;
                char*fullfilename = concatPaths(fulldirname, name);
                as3_schedule_file(name, fullfilename);
                free(fullfilename);
            }
        }
        free(fulldirname);
        i = i->next;
    }
#endif
    return ok;
}

void as3_schedule_package(const char*package)
{
    DEBUG printf("[pass %d] schedule package %s\n", as3_pass, package);
    char*dirname = strdup(package);
    int s=0;
    while(dirname[s]) {
        if(dirname[s]=='.') dirname[s]='/';
        s++;
    };
    if(!as3_schedule_directory(dirname))
        as3_softwarning("Could not find package %s in file system", package);
}

static void schedule_class(const char*package, const char*cls, char error)
{
    if(error) {
        DEBUG printf("[pass %d] schedule class %s.%s\n",  as3_pass, package, cls);
    }
    if(!cls) {
        as3_schedule_package(package);
        return;
    }
    int l1 = package?strlen(package):0;
    int l2 = cls?strlen(cls):0;
    char*filename = malloc(l1+l2+5);
    int s=0,t=0;
    while(package[s]) {
        if(package[s]=='.')
            filename[t++]='/';
        else
            filename[t++] = package[s];
        s++;
    }
    if(t)
        filename[t++] = '/';

    strcpy(filename+t, cls);
    strcpy(filename+t+l2, ".as");
    char*f=0;
    if(!(f=find_file(filename, error))) {
        int i;
        /* try lower case filename (not packagename!), too */
        for(i=t;i<t+l2;i++) {
            if(filename[i]>='A' && filename[i]<='Z')
                filename[i] += 'a'-'A';
        }
        if(!(f=find_file(filename, error))) {
            if(error) {
                strcpy(filename+t, cls);
                strcpy(filename+t+l2, ".as");
                as3_warning("Could not open file %s", filename);
            }
            return;
        }
    }
    as3_schedule_file(filename, f);
}

void as3_schedule_class(const char*package, const char*cls)
{
    schedule_class(package, cls, 1);
}

void as3_schedule_class_noerror(const char*package, const char*cls)
{
    schedule_class(package, cls, 0);
}


static void*as3code = 0;
void* as3_getcode()
{
    if(parser_initialized) {
        parser_initialized = 0;
        as3code = finish_parser();
    }
    return as3code;
}
char* as3_getglobalclass()
{
    return as3_globalclass;
}

void as3_destroy() 
{
    if(parser_initialized) {
        parser_initialized = 0;
        swf_FreeABC(finish_parser());
#ifdef STORE_TOKENS
        mem_clear(&tokens);
#endif
    }
    if(as3_globalclass) {
        free(as3_globalclass);as3_globalclass=0;
    }
}

