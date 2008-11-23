#ifndef __abc_code_h__
#define __abc_code_h__

#include "../q.h"

DECLARE(abc_code);
DECLARE(codestats);

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

struct _abc_code {
    U8 opcode;
    U8 len;
    void*data[2];
    abc_code_t*next;
    abc_code_t*prev;
    abc_code_t*parent;

    abc_code_t*branch;
    int pos; //used during code path evaluation
};

struct _codestats {
    int max_stack;
    int local_count;
    int init_scope_depth;
    int max_scope_depth;
};

abc_code_t*add_opcode(abc_code_t*atag, U8 op);

abc_code_t*code_parse(TAG*tag, int len, abc_file_t*file, pool_t*pool);
int        code_dump(abc_code_t*c, abc_file_t*file, char*prefix, FILE*fo);
void       code_write(TAG*tag, abc_code_t*code, pool_t*pool, abc_file_t*file);
void       code_free(abc_code_t*c);


#endif
