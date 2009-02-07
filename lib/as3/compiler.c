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

static void as3_parse(const char*name, const char*filename, void*mem, int length)
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

#ifdef STORE_TOKENS
    tokens.pos = 0;
    tokens.read_pos = 0;
#endif

    FILE*fi = 0;
    if(filename) {
        fi = enter_file2(name, filename, 0);
        as3_file_input(fi);
    } else {
        enter_file(name, name, 0);
        as3_buffer_input(mem, length);
    }

    /* pass 1 */
    as3_pass = 1;
    as3_tokencount=0;
    initialize_file(name);
    a3_parse();
    as3_lex_destroy();
    finish_file();
    
#ifdef STORE_TOKENS
    tokens.read_pos = 0;
#endif

    if(filename) {
        fclose(fi);
        fi = enter_file2(name, filename, 0);
        as3_file_input(fi);
    } else {
        enter_file(name, name, 0);
        as3_buffer_input(mem, length);
    }

    /* pass 2 */
    as3_pass = 2;
    as3_tokencount=0;
    initialize_file(name);
    a3_parse();
    as3_lex_destroy();
    finish_file();

    if(filename) {
        fclose(fi);
    }
}

void as3_parse_bytearray(const char*name, void*mem, int length)
{
    as3_parse(name, 0, mem, length);
}

void as3_parse_file(const char*filename) 
{
    char*fullfilename = find_file(filename);
    if(!fullfilename)
        return; // not found
    as3_parse(filename, fullfilename, 0,0);
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

