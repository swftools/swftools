#ifndef __abc_code_h__
#define __abc_code_h__

#include "../q.h"

DECLARE(abc_code);

#include "abc.h"

typedef struct _opcode
{
    unsigned char opcode;
    char*name;
    char*params;

    int stack_minus;
    int stack_plus;
    int scope_stack_change;
    int flags;
} opcode_t;

struct _abc_code {
    U8 opcode;
    U8 len;
    void*params[2];
    abc_code_t*next;
    abc_code_t*prev;
    abc_code_t*parent;
};

abc_code_t* add_opcode(abc_code_t*atag, U8 op);

#endif
