/* code.h

   Routines for handling Flash2 AVM2 ABC Actionscript

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

#ifndef __abc_code_h__
#define __abc_code_h__

#include "../q.h"

DECLARE(code);
DECLARE_LIST(code);
DECLARE(codestats);
DECLARE(codelookup);
DECLARE(lookupswitch);


#include "abc.h"

typedef struct _opcode
{
    unsigned char opcode;
    char*name;
    char*params;

    int stack_minus;
    int stack_plus;
    int scope_stack_plus;
    int flags;
} opcode_t;

struct _code {
    void*data[2];
    code_t*next;
    code_t*prev;

    code_t*branch;
    int pos; //used during code path evaluation
    
    U8 opcode;
};

struct _lookupswitch {
    code_t*def;
    code_list_t*targets;
};

#define FLAGS_ACTIVATION 0x02
#define FLAGS_SET_DXNS 0x40
struct _codestats {
    int max_stack;
    int local_count;
    int max_scope_depth;
    int flags;
};

struct _codelookup {
    code_t**bytepos;
    int len;
};

code_t*code_dup(code_t*c);

code_t*add_opcode(code_t*atag, U8 op);

code_t*code_parse(TAG*tag, int len, abc_file_t*file, pool_t*pool, codelookup_t**codelookup);
int        code_dump(code_t*c);
int        code_dump2(code_t*c, abc_exception_list_t*exceptions, abc_file_t*file, char*prefix, FILE*fo);
void       code_write(TAG*tag, code_t*code, pool_t*pool, abc_file_t*file);
void       code_free(code_t*c);
code_t* code_atposition(codelookup_t*l, int pos);
void codelookup_free(codelookup_t*codelookup);

code_t*code_cutlast(code_t*c);

codestats_t* code_get_statistics(code_t*code, abc_exception_list_t*exceptions);

void codestats_print(codestats_t*s);
void codestats_free(codestats_t*s);

code_t* code_start(code_t*c);

code_t* code_append(code_t*code, code_t*toappend);

code_t* cut_last_push(code_t*_c);

char is_getlocal(code_t*c);

#define code_new() (0)

#endif
