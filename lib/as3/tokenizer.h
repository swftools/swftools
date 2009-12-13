/* tokenizer.h

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

#ifndef __parser_h__
#define __parser_h__

#include "../q.h"
#include "abc.h"
#include "pool.h"
#include "files.h"
#include "tokenizer.h"
#include "registry.h"
#include "code.h"
#include "opcodes.h"
#include "expr.h"

DECLARE(token);
DECLARE_LIST(token);
DECLARE(param);
DECLARE_LIST(param);
DECLARE(params);
DECLARE(modifiers);
DECLARE(namespace_decl);
DECLARE_LIST(namespace_decl);
DECLARE_LIST(typedcode);

struct _param {
    char*name;
    classinfo_t*type;
    constant_t*value;
};

struct _params {
    param_list_t*list;
    char varargs;
};

struct _modifiers {
    int flags;
    char*ns;
};
struct _namespace_decl {
    const char*name;
    const char*url;
};

/* small helper structs: */
typedef struct _codeandnumber {
    code_t*cc;
    int number;
} codeandnumber_t;
typedef struct _for_start {
    char*name;
    char each;
} for_start_t;
typedef struct _regexp {
    char*pattern;
    char*options;
} regexp_t;

extern char start_of_expression;

typedef token_t*tokenptr_t;

#include "parser.tab.h"

extern char*token2string(enum yytokentype nr, YYSTYPE v);
extern unsigned int as3_tokencount;

void as3_buffer_input(void*buffer, int len);
void as3_file_input(FILE*fi);

void tokenizer_begin_xml();
void tokenizer_begin_xmltext();
void tokenizer_end_xmltext();
void tokenizer_end_xml();

#define T_EOF 0

extern int avm2_lex();
extern int avm2_lex_destroy();

#endif
