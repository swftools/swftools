#ifndef __code_h__
#define __code_h__

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
    char badbranch;
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

code_t*add_opcode(code_t*atag, U8 op);

code_t*code_parse(TAG*tag, int len, abc_file_t*file, pool_t*pool, codelookup_t**codelookup);
int        code_dump(code_t*c, exception_list_t*exceptions, abc_file_t*file, char*prefix, FILE*fo);
void       code_write(TAG*tag, code_t*code, pool_t*pool, abc_file_t*file);
void       code_free(code_t*c);
code_t* code_atposition(codelookup_t*l, int pos);
void codelookup_free(codelookup_t*codelookup);

codestats_t* code_get_statistics(code_t*code, exception_list_t*exceptions);

void codestats_print(codestats_t*s);
void codestats_free(codestats_t*s);


#endif
