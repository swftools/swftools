/* files.c

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2008 Matthias Kramm <kramm@quiss.org>
 
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <errno.h>
#include "files.h"
#include "common.h"
#include "tokenizer.h"
#include "../os.h"

static int verbose = 0;
static void dbg(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(!verbose)
	return;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    printf("(includefilehandler) ");
    printf("%s\n", buf);
    fflush(stdout);
}


int current_line=1;
int current_column=0;
char* current_filename=0;
char* current_filename_short=0;
char* current_filename_long=0;
include_dir_t* current_include_dirs=0;

#define MAX_INCLUDE_DEPTH 16

void*include_stack[MAX_INCLUDE_DEPTH];
int line_stack[MAX_INCLUDE_DEPTH];
int column_stack[MAX_INCLUDE_DEPTH];
char* filename_stack[MAX_INCLUDE_DEPTH];
char* shortfilename_stack[MAX_INCLUDE_DEPTH];
char* longfilename_stack[MAX_INCLUDE_DEPTH];
include_dir_t* includedir_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;

void add_include_dir(char*dir)
{
    include_dir_t*d = malloc(sizeof(include_dir_t));
    memset(d , 0, sizeof(include_dir_t));
    d->path = strdup(dir);
    d->next = current_include_dirs;
    current_include_dirs = d;
}

void del_include_dirs(include_dir_t*d, include_dir_t*d2)
{
    while(d && d!=d2) {
        include_dir_t*next = d->next;
        free(d->path);d->path=0;
        d->next = 0;
        free(d);
        d = next;
    }
}

char*get_path(const char*file)
{
    char*path = strdup(file);
    char*r1 = strrchr(path, '/');
    char*r2 = strrchr(path, '\\');
    if(r1<r2) {
        *r2=0;
        return path;
    } else if(r1>r2) {
        *r1=0;
        return path;
    } else {
        return strdup(".");
    }
}

char is_absolute(const char*filename) 
{
    if(!filename || !filename[0])
        return 0;
    if(filename[0]=='/' || filename[0]=='\\')
        return 1;
    if(filename[1]==':' && filename[2]=='/')
        return 1;
    if(filename[1]==':' && filename[2]=='\\')
        return 1;
    return 0;
}

char* filename_to_lowercase(const char*name)
{
    char*n = strdup(name);
    //char*x1 = strrchr(name, '/');
    //char*x2 = strrchr(name, '\\');
    char*s = n;
    //if(x1+1>s) s=x1+1;
    //if(x2+1>s) s=x2+1;
    while(*s) {
	/* FIXME: what we probably should do here is use libc's tolower().
	   I don't really know yet, though, how Windows (or MacOS X) handles
	   lowercasing of Unicode filenames */
	if(*s>='A' && *s<='Z')
	    *s += 'a'-'A';
	s++;
    }
    return n;
}

char* normalize_path(const char*path)
{
    char*n = 0, *d = 0;
    if(!is_absolute(path)) {
        char buf[512];
        char*c = getcwd(buf,512);
        int l = strlen(buf);
        d = n = malloc(l+strlen(path)+10);
        strcpy(n, buf);d += l;
        if(!l || n[l-1]!=path_seperator) {
            *d=path_seperator;d++;
        }
    } else {
        d = n = strdup(path);
    }
    const char*s=path;
    char init = 1;

    while(*s) {
        if(init && s[0] == '.' && (s[1]==path_seperator || s[1]=='\0')) {
            if(!s[1]) break;
            s+=2;
            init=1;
            continue;
        }
        if(init && s[0] == '.' && s[1] == '.' && (s[2] == path_seperator || s[2]=='\0')) {
            // step one down
            char*last = 0;
            if(d<=n) 
                return 0;
            *--d = 0;
            if(!(last=strrchr(n, path_seperator))) {
                return 0;
            }
            d = last+1;
            if(!s[2]) break;
            s+=3;
            init=1;
            continue;
        }

        *d = *s;
        if(*s==path_seperator) init=1;
        else init=0;
        d++;s++;
    }
    if(d!=n && d[-1]==path_seperator) 
        d--;
    *d = 0;
 
#ifdef LOWERCASE_UPPERCASE
    n = filename_to_lowercase(n);
#endif
    
    return n;
}
static void testnormalize()
{
#define TEST(x) {printf("%s -> %s\n", (x), normalize_path(x));}
    TEST(".");
    TEST("../as3");
    TEST("../as3/");
    TEST("../as3/parser.y");
    TEST("../as3/ok/../ok/scope.as");
    TEST("ok/scope.as");
    TEST("ok/./scope.as");
    TEST("./ok/scope.as");
    TEST("./");
    TEST("/tmp/");
    TEST("/./tmp/");
    TEST("../");
    TEST("/");
    TEST("/tmp");
    TEST("/tmp/../usr/");
}

char* concat_paths(const char*base, const char*add)
{
    int l1 = strlen(base);
    int l2 = strlen(add);
    int pos = 0;
    char*n = 0;
    while(l1 && base[l1-1] == path_seperator)
	l1--;
    while(pos < l2 && add[pos] == path_seperator)
	pos++;
    n = (char*)malloc(l1 + (l2-pos) + 2);
    memcpy(n,base,l1);
    n[l1]=path_seperator;
    memcpy(&n[l1+1],&add[pos],l2-pos+1);
    return n;
}

char*find_file(const char*filename, char error)
{
    include_dir_t*i = current_include_dirs;
    FILE*fi = 0;
    if(is_absolute(filename)) {
        FILE*fi = fopen(filename, "rb");
        if(fi) {
            fclose(fi);
            return strdup(filename);
        }
    } else {
        if(!i && error) {
            as3_warning("Include directory stack is empty, while looking for file %s", filename);
        }
        while(i) {
            char*p = concat_paths(i->path, filename);
            fi = fopen(p, "rb");
            if(fi) {
                fclose(fi);
                return p;
            } else {
                free(p);
            }
            i = i->next;
        }
    }
    if(!error) {
        return 0;
    }

    as3_error("Couldn't find file %s", filename);
    i = current_include_dirs;
    while(i) {
        fprintf(stderr, "include dir: %s\n", i->path);
        i = i->next;
    }
    return 0;
}

void enter_file(const char*name, const char*filename, void*state)
{
    if(include_stack_ptr >= MAX_INCLUDE_DEPTH) {
    	as3_error("Includes nested too deeply");
	exit(1);
    }
    include_stack[include_stack_ptr] = state;
    line_stack[include_stack_ptr] = current_line;
    column_stack[include_stack_ptr] = current_column;
    shortfilename_stack[include_stack_ptr] = current_filename_short;
    longfilename_stack[include_stack_ptr] = current_filename_long;
    filename_stack[include_stack_ptr] = current_filename;
    includedir_stack[include_stack_ptr] = current_include_dirs;
    
    /*char*dir = get_path(filename);
    add_include_dir(dir);
    free(dir);*/

    include_stack_ptr++;
    
    dbg("entering file %s", filename);

    current_line=1;
    current_column=0;
    current_filename = strdup(name);
    current_filename_short = strdup(name);
    current_filename_long = strdup(filename);
}

FILE*enter_file2(const char*name, const char*filename, void*state)
{
    enter_file(name, filename, state);
    FILE*fi = fopen(filename, "rb");
    if(!fi) {
	as3_error("Couldn't find file %s: %s", filename, strerror(errno));
    }
    return fi;
}

void* leave_file()
{
    dbg("leaving file %s", current_filename);
    if(--include_stack_ptr<=0) {
        return 0;
    } else {
        free(current_filename);current_filename = filename_stack[include_stack_ptr];
        free(current_filename_short);current_filename_short = shortfilename_stack[include_stack_ptr];
        free(current_filename_long);current_filename_long = longfilename_stack[include_stack_ptr];
        current_column = column_stack[include_stack_ptr];
        current_line = line_stack[include_stack_ptr];
        del_include_dirs(includedir_stack[include_stack_ptr], current_include_dirs);
        current_include_dirs = includedir_stack[include_stack_ptr];
        return include_stack[include_stack_ptr];
    }
}

