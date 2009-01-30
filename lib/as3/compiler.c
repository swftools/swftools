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
#include <errno.h>

/* flex/bison definitions */
extern void avm2_set_in (FILE *  in_str );
extern int avm2_parse();

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

void as3_parse_file(char*filename) 
{
    if(!registry_initialized) {
        registry_initialized = 1;
        registry_init();
    }
    if(!parser_initialized) {
        parser_initialized = 1;
        initialize_parser();
    }

    as3_pass = 0;
    
    char*fullfilename = enter_file(filename, 0);
    FILE*fi = fopen(fullfilename, "rb");
    if(!fi) {
	syntaxerror("Couldn't find file %s: %s", fullfilename, strerror(errno));
    }
    /* pass 1 */
    as3_pass = 1;
    avm2_set_in(fi);
    initialize_file(filename);
    avm2_parse();
    avm2_lex_destroy();
    finish_file();

    /* pass 2 */
    // TODO: this should re-use tokens!
    enter_file(filename, 0);
    as3_pass = 2;
    fseek(fi, 0, SEEK_SET);
    avm2_set_in(fi);
    initialize_file(filename);
    avm2_parse();
    avm2_lex_destroy();
    finish_file();

    fclose(fi);

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
    }
    if(as3_globalclass) {
        free(as3_globalclass);as3_globalclass=0;
    }
}

