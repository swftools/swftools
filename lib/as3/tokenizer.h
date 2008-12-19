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
#include "pool.h"
#include "code.h"
#include "registry.h"

DECLARE(token);
DECLARE_LIST(token);
DECLARE(typedcode);
DECLARE(typedcode_list);

struct _token {
    int type;
    char*text;
    token_list_t*tokens;
};

typedef struct _writeable {
    code_t*write;
    code_t*read;
    class_signature_t*type;
} writeable_t;

struct _typedcode {
    code_t*c;
    class_signature_t*t;
};
struct _typedcode_list {
    struct _typedcode typedcode;
    struct _typedcode_list*nxt;
};

extern char start_of_expression;

typedef token_t*tokenptr_t;

#include "parser.tab.h"

void syntaxerror(const char*format, ...);

#define T_EOF 0

#endif
