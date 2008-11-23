#include <assert.h>
#include "code.h"

#define OP_REGISTER 1
#define OP_STACK_ARGS 2
#define OP_STACK_NS 4
#define OP_SET_DXNS 8
#define OP_RETURN 16
#define OP_THROW 32
#define OP_BRANCH 32
#define OP_JUMP 64
#define OP_LABEL 128
#define OP_SWITCH 256
#define OP_NEED_ACTIVATION 512
#define OP_STACK_ARGS2 1024

/* 2 = u30 index into multiname
   m = u30 index into method
   n = u30 number of params
   i = u30 index into method info
   b = byte
   s = short
   c = u30 index into class
   s = string (u30 index into strings)
   S = switch
   u = u30
   r = register
*/
opcode_t opcodes[]={
{0xa0, "add", "",              -2, 1, 0, 0},
{0xc5, "add_i", "",            -2, 1, 0, 0},
{0x86, "astype", "2",          -1, 1, 0, 0},
{0x87, "astypelate", "",       -2, 1, 0, 0},
{0xA8, "bitand", "",           -2, 1, 0, 0},
{0x97, "bitnot", ""            -1, 1, 0, 0},
{0xa9, "bitor", "",            -2, 1, 0, 0},
{0xaa, "bitxor", ""            -2, 1, 0, 0},
{0x41, "call", "n",            -2, 1, 0, OP_STACK_ARGS},
{0x43, "callmethod", "mn",     -1, 1, 0, OP_STACK_ARGS},
{0x4c, "callproplex", "2n",    -1, 1, 0, OP_STACK_ARGS|OP_STACK_NS},
{0x46, "callproperty", "2n",   -1, 1, 0, OP_STACK_ARGS|OP_STACK_NS},
{0x4f, "callpropvoid", "2n",   -1, 0, 0, OP_STACK_ARGS|OP_STACK_NS},
{0x44, "callstatic", "in",     -1, 1, 0, OP_STACK_ARGS},
{0x45, "callsuper", "2n",      -1, 1, 0, OP_STACK_ARGS|OP_STACK_NS},
{0x4e, "callsupervoid", "2n",  -1, 0, 0, OP_STACK_ARGS|OP_STACK_NS},
{0x78, "checkfilter", "",      -1, 1, 0, 0},
{0x80, "coerce", "2",          -1, 1, 0, 0},
{0x82, "coerce_a", "",         -1, 1, 0, 0},
{0x85, "coerce_s", "",         -1, 1, 0, 0},
{0x42, "construct", "n",       -1, 1, 0, OP_STACK_ARGS},
{0x4a, "constructprop", "2n",  -1, 1, 0, OP_STACK_ARGS|OP_STACK_NS},
{0x49, "constructsuper", "n",  -1, 1, 0, OP_STACK_ARGS},
{0x76, "convert_b", "",        -1, 1, 0, 0},
{0x73, "convert_i", "",        -1, 1, 0, 0},
{0x75, "convert_d", "",        -1, 1, 0, 0},
{0x77, "convert_o", "",        -1, 1, 0, 0},
{0x74, "convert_u", "",        -1, 1, 0, 0},
{0x70, "convert_s", "",        -1, 1, 0, 0},
{0xef, "debug", "D",            0, 0, 0, 0},
{0xf1, "debugfile", "s",        0, 0, 0, 0},
{0xf0, "debugline", "u",        0, 0, 0, 0},
{0x94, "declocal", "r",         0, 0, 0, OP_REGISTER},
{0xc3, "declocal_i", "r",       0, 0, 0, OP_REGISTER},
{0x93, "decrement", "",        -1, 1, 0, 0}, 
{0xc1, "decrement_i", "",      -1, 1, 0, 0},
{0x6a, "deleteproperty", "2",  -1, 1, 0, OP_STACK_NS},
{0xa3, "divide", "",           -2, 1, 0, 0},
{0x2a, "dup", "",              -1, 2, 0, 0},
{0x06, "dxns", "s",             0, 0, 0, OP_SET_DXNS},
{0x07, "dxnslate", "",         -1, 0, 0, OP_SET_DXNS},
{0xab, "equals", "",           -2, 1, 0, 0},
{0x72, "esc_xattr", "",        -1, 1, 0, 0},
{0x71, "esc_xelem", "",        -1, 1, 0, 0},
{0x5e, "findproperty", "2",     0, 1, 0, OP_STACK_NS},
{0x5d, "findpropstrict", "2",   0, 1, 0, OP_STACK_NS},
{0x59, "getdescendants", "2",  -1, 1, 0, OP_STACK_NS},
{0x64, "getglobalscope", "",    0, 1, 0, 0},
{0x6e, "getglobalslot", "u",    0, 1, 0, 0},
{0x60, "getlex", "2",           0, 1, 0, 0}, //multiname may not be runtime
{0x62, "getlocal", "r",         0, 1, 0, OP_REGISTER},
{0xd0, "getlocal_0", "",        0, 1, 0, OP_REGISTER},
{0xd1, "getlocal_1", "",        0, 1, 0, OP_REGISTER},
{0xd2, "getlocal_2", "",        0, 1, 0, OP_REGISTER},
{0xd3, "getlocal_3", "",        0, 1, 0, OP_REGISTER},
{0x66, "getproperty", "2",     -1, 1, 0, OP_STACK_NS},
{0x65, "getscopeobject", "u",   0, 1, 0, 0}, // u = index into scope stack
{0x6c, "getslot", "u",         -1, 1, 0, 0},
{0x04, "getsuper", "2",        -1, 1, 0, OP_STACK_NS},
{0xaf, "greaterequals", "",    -2, 1, 0, 0},
{0x1f, "hasnext", "",          -2, 1, 0, 0},
{0x32, "hasnext2", "rr",        0, 1, 0, OP_REGISTER},
{0x13, "ifeq", "j",            -2, 0, 0, OP_BRANCH},
{0x12, "iffalse", "j",         -1, 0, 0, OP_BRANCH},
{0x18, "ifge", "j",            -2, 0, 0, OP_BRANCH},
{0x17, "ifgt", "j",            -2, 0, 0, OP_BRANCH},
{0x16, "ifle", "j",            -2, 0, 0, OP_BRANCH},
{0x15, "iflt", "j",            -2, 0, 0, OP_BRANCH},
{0x0f, "ifnge", "j",           -2, 0, 0, OP_BRANCH},
{0x0e, "ifngt", "j",           -2, 0, 0, OP_BRANCH},
{0x0d, "ifnle", "j",           -2, 0, 0, OP_BRANCH},
{0x0c, "ifnlt", "j",           -2, 0, 0, OP_BRANCH},
{0x14, "ifne", "j",            -2, 0, 0, OP_BRANCH},
{0x19, "ifstricteq", "j",      -2, 0, 0, OP_BRANCH},
{0x1a, "ifstrictne", "j",      -2, 0, 0, OP_BRANCH},
{0x11, "iftrue", "j",          -1, 0, 0, OP_BRANCH},
{0xb4, "in", "",               -2, 1, 0, 0},
{0x92, "inclocal", "r",         0, 0, 0, OP_REGISTER},
{0xc2, "inclocal_i", "r",       0, 0, 0, OP_REGISTER},
{0x91, "increment", "",        -1, 1, 0, 0},
{0xc0, "increment_i", "",      -1, 1, 0, 0},
{0x68, "initproperty", "2",    -1, 0, 0, OP_STACK_NS},
{0xb1, "instanceof", "",       -2, 1, 0, 0},
{0xb2, "istype", "2",          -1, 1, 0, 0}, // may not be a runtime multiname
{0xb3, "istypelate", "",       -2, 1, 0, 0},
{0x10, "jump", "j",             0, 0, 0, OP_JUMP},
{0x08, "kill", "r",             0, 0, 0, OP_REGISTER},
{0x09, "label", "",             0, 0, 0, OP_LABEL},
{0xae, "lessequals", "",       -2, 1, 0, 0},
{0xad, "lessthan", "",         -2, 1, 0, 0},
{0x1b, "lookupswitch", "S",    -1, 0, 0, OP_SWITCH},
{0xa5, "lshift", "",           -2, 1, 0, 0},
{0xa4, "modulo", "",           -2, 1, 0, 0},
{0xa2, "multiply", "",         -2, 1, 0, 0},
{0xc7, "multiply_i", "",       -2, 1, 0, 0},
{0x90, "negate", "",           -1, 1, 0, 0},
{0xc4, "negate_i", "",         -1, 1, 0, 0},
{0x57, "newactivation", "",     0, 1, 0, OP_NEED_ACTIVATION},
{0x56, "newarray", "n",         0, 1, 0, OP_STACK_ARGS},
{0x5a, "newcatch", "u",         0, 1, 0, 0}, //u = index into exception_info
{0x58, "newclass", "c",        -1, 1, 0, 0}, //c = index into class_info
{0x40, "newfunction", "m",      0, 1, 0, 0}, //i = index into method_info
{0x55, "newobject", "n",        0, 1, 0, OP_STACK_ARGS2},
{0x1e, "nextname", "",         -2, 1, 0, 0},
{0x23, "nextvalue", "",        -2, 1, 0, 0},
{0x02, "nop", "",               0, 0, 0, 0},
{0x96, "not", "",              -1, 1, 0 ,0},
{0x29, "pop", "",              -1, 0, 0, 0},
{0x1d, "popscope", "",          0, 0,-1, 0},
{0x24, "pushbyte", "b",         0, 1, 0, 0},
{0x2f, "pushdouble", "u",       0, 1, 0, 0}, //index into floats
{0x27, "pushfalse", "",         0, 1, 0, 0},
{0x2d, "pushint", "u",          0, 1, 0, 0}, //index into ints
{0x31, "pushnamespace", "u",    0, 1, 0, 0}, //index into namespace
{0x28, "pushnan", "",           0, 1, 0, 0},
{0x20, "pushnull", "",          0, 1, 0, 0},
{0x30, "pushscope", "",         0, 0, 1, 0},
{0x25, "pushshort", "u",        0, 1, 0, 0},
{0x2c, "pushstring", "s",       0, 1, 0, 0},
{0x26, "pushtrue", "",          0, 1, 0, 0},
{0x2e, "pushuint", "u",         0, 1, 0, 0}, //index into uints
{0x21, "pushundefined", "",     0, 1, 0, 0},
{0x1c, "pushwith", "",         -1, 0, 1, 0},
{0x48, "returnvalue", "",      -1, 0, 0, OP_RETURN},
{0x47, "returnvoid", "",        0, 0, 0, OP_RETURN},
{0xa6, "rshift", "",           -2, 1, 0, 0},
{0x63, "setlocal", "r",        -1, 0, 0, OP_REGISTER},
{0xd4, "setlocal_0", "",       -1, 0, 0, OP_REGISTER},
{0xd5, "setlocal_1", "",       -1, 0, 0, OP_REGISTER},
{0xd6, "setlocal_2", "",       -1, 0, 0, OP_REGISTER},
{0xd7, "setlocal_3", "",       -1, 0, 0, OP_REGISTER},
{0x6f, "setglobalslot", "u",   -1, 0, 0, 0},
{0x61, "setproperty", "2",     -2, 0, 0, OP_STACK_NS},
{0x6d, "setslot", "2",         -2, 0, 0, 0},
{0x05, "setsuper", "2",        -2, 0, 0, OP_STACK_NS},
{0xac, "strictequals", "",     -2, 1, 0, 0},
{0xa1, "subtract", "",         -2, 1, 0, 0},
{0xc6, "subtract_i", "",       -2, 1, 0, 0},
{0x2b, "swap", "",             -2, 2, 0, 0},
{0x03, "throw", "",            -1, 0, 0, OP_THROW},
{0x95, "typeof", "",           -1, 1, 0, 0},
{0xa7, "urshift", "",          -2, 1, 0, 0},
{0xb0, "xxx", "",               0, 0, 0, 0},
};

static U8 op2index[256] = {254};

opcode_t* opcode_get(U8 op)
{
    int t;
    if(op2index[0]==254) {
        memset(op2index, 255, sizeof(op2index));
        for(t=0;t<sizeof(opcodes)/sizeof(opcodes[0]);t++) {
            op2index[opcodes[t].opcode] = t;
        }
    }
    if(op2index[op]!=255)
        return &opcodes[op2index[op]];
    return 0;
}

abc_code_t*code_parse(TAG*tag, int len, abc_file_t*file, pool_t*pool)
{
    abc_code_t*head=0;
    abc_code_t*code=0;
    int start=tag->pos;
    int end=tag->pos+len;
    //printf("-->\n");fflush(stdout);

    abc_code_t**bytepos = rfx_calloc(sizeof(abc_code_t*)*len);
        
    while(tag->pos<end) {
        int codepos = tag->pos-start;
        U8 opcode = swf_GetU8(tag);
        opcode_t*op = opcode_get(opcode);
	if(!op) {
	    fprintf(stderr, "Can't parse opcode %02x\n", opcode);
	    return head;
        }
        //printf("%s\n", op->name);fflush(stdout);
        NEW(abc_code_t,c);
        c->pos = codepos;

        bytepos[codepos] = c;

        if(!head) {
            head = code = c;
        } else {
            code->next = c;
            code = c;
        }

        c->opcode = opcode;

        char*p = op->params;
        int pos = 0;
        while(*p) {
            void*data = 0;
            if(*p == 'n') { // number
                data = (void*)(ptroff_t)swf_GetU30(tag);
            } else if(*p == '2') { //multiname
                data = multiname_clone(pool_lookup_multiname(pool, swf_GetU30(tag)));
            } else if(*p == 'm') { //method
                data = array_getvalue(file->methods, swf_GetU30(tag));
            } else if(*p == 'c') { //classinfo
                data = array_getvalue(file->classes, swf_GetU30(tag));
            } else if(*p == 'i') {
                data = array_getvalue(file->method_bodies, swf_GetU30(tag));
            } else if(*p == 'u') { // generic integer
                data = (void*)(ptroff_t)swf_GetU30(tag);
            } else if(*p == 'r') { // local register
                data = (void*)(ptroff_t)swf_GetU30(tag);
            } else if(*p == 'b') { // byte
                data = (void*)(ptroff_t)swf_GetU8(tag);
            } else if(*p == 'j') { // jump
                int j = swf_GetS24(tag);
                data = (void*)(ptroff_t)j;
            } else if(*p == 's') { // string
                data = strdup((char*)pool_lookup_string(pool, swf_GetU30(tag)));
            } else if(*p == 'D') { // debug
                /*type, usually 1*/
                U8 type = swf_GetU8(tag);
                if(type!=1) 
                    fprintf(stderr, "Unknown debug type: %02x\n", type);
                /*register name*/
                code->data[0] = strdup((char*)pool_lookup_string(pool, swf_GetU30(tag)));
                /*register index*/
                code->data[1] = (void*)(ptroff_t)swf_GetU8(tag);
                /*unused*/
                swf_GetU30(tag);
            } else if(*p == 'S') { // switch statement TODO
                /* I hate these things */
                swf_GetU24(tag); //default
                int num = swf_GetU30(tag)+1;
                int t;
                for(t=0;t<num;t++) 
                    swf_GetU24(tag);
                data = 0;
            } else {
                printf("Can't parse opcode param type \"%c\".\n", *p);
                return 0;
            }
            if(data)
                code->data[pos++] = data;
            p++;
        }
    }

//#define DEBUG_BYTES
#ifdef DEBUG_BYTES
    int t;
    for(t=0;t<len;t++) {
        abc_code_t*c = bytepos[t];
        if(c) {
            opcode_t*op = opcode_get(c->opcode);
            if(op->flags & (OP_JUMP|OP_BRANCH)) {
                printf("%5d) %02x %s %d\n", t, tag->data[start+t], op->name, c->data[0]);
            } else {
                printf("%5d) %02x %s\n", t, tag->data[start+t], op->name);
            }
        } else {
            printf("%5d) %02x\n", t, tag->data[start+t]);
        }
    }
    printf("%5d) %02x\n", t, tag->data[start+t]);
#endif

    abc_code_t*c = head;
    while(c) {
        opcode_t*op = opcode_get(c->opcode);
        if(op->flags & (OP_JUMP|OP_BRANCH)) {
            int j = ((int)(ptroff_t)c->data[0]) + 4;
#ifdef DEBUG_BYTES
            printf("%s %d %d\n", op->name, c->pos, j);
#endif
            if(c->pos+j < 0 ||
               c->pos+j > len ||
              (c->pos+j!=len && !bytepos[c->pos+j])) {
                /* flex likes to generate these. yuck. */
                fprintf(stderr, "Invalid jump instruction \"%s\" from %d to %d (%d)\n", op->name, c->pos, c->pos+j, len);
                code->branch = 0;
            } else {
                if(c->pos==len)
                    code->branch = 0;
                else
                    code->branch = bytepos[c->pos+j];
            }
        }
        c = c->next;
    } 
    free(bytepos);
    return head;
}

void code_free(abc_code_t*c)
{
    while(c) {
        abc_code_t*next = c->next;
        opcode_t*op = opcode_get(c->opcode);
        char*p = op?op->params:"";
        int pos=0;
        while(*p) {
            void*data = c->data[pos];
            if(*p == '2') { //multiname
                multiname_destroy(data);
            } else if(strchr("sD", *p)) {
                free(data);
            }
            c->data[pos]=0;
            p++;pos++;
        }
        memset(c, 0, sizeof(c));
        free(c);
        c = next;
    }
}

int code_dump(abc_code_t*c, abc_file_t*file, char*prefix, FILE*fo)
{
    pool_t*pool = pool_new();

    while(c) {
	U8 opcode = c->opcode;
	int t;
	char found = 0;
        opcode_t*op = opcode_get(opcode);
	if(!op) {
	    fprintf(stderr, "Can't parse opcode %02x.\n", opcode);
	    return 0;
	} else {
            fprintf(fo, "%s%s ", prefix, op->name);
            char*p = op->params;
            char first = 1;
            int pos=0;
            while(*p) {
                void*data = c->data[pos];
                if(pos>0)
                    printf(", ");

                if(*p == 'n') {
                    int n = (ptroff_t)data;
                    fprintf(fo, "%d params", n);
                } else if(*p == '2') {
                    multiname_t*n = (multiname_t*)data;
                    char* m = multiname_to_string(n);
                    fprintf(fo, "%s", m);
                    free(m);
                } else if(*p == 'm') {
                    abc_method_t*m = (abc_method_t*)data;
                    fprintf(fo, "[method %s]", m->name);
                } else if(*p == 'c') {
                    abc_class_t*cls = (abc_class_t*)data;
                    char*classname = multiname_to_string(cls->classname);
                    fprintf(fo, "[classinfo %s]", classname);
                    free(classname);
                } else if(*p == 'i') {
                    abc_method_body_t*b = (abc_method_body_t*)data;
                    fprintf(fo, "[methodbody]");
                } else if(*p == 'u') {
                    int n = (ptroff_t)data;
                    fprintf(fo, "%d", n);
                } else if(*p == 'r') {
                    int n = (ptroff_t)data;
                    fprintf(fo, "r%d", n);
                } else if(*p == 'b') {
                    int b = (ptroff_t)data;
                    fprintf(fo, "%02x", b);
                } else if(*p == 'j') {
                    int n = (ptroff_t)data;
                    fprintf(fo, "%d", n);
                } else if(*p == 's') {
                    fprintf(fo, "\"%s\"", data);
                } else if(*p == 'D') {
                    fprintf(fo, "[register %02x=%s]", (ptroff_t)c->data[1], (char*)c->data[0]);
                } else if(*p == 'S') {
                    fprintf(fo, "[switch data]");
                } else {
                    fprintf(stderr, "Can't parse opcode param type \"%c\"\n", *p);
                    return 0;
                }
                p++;
                pos++;
                first = 0;
            }
            fprintf(fo, "\n");
	}
        c = c->next;
    }
    return 1;
}

static int opcode_write(TAG*tag, abc_code_t*code, pool_t*pool, abc_file_t*file)
{
    opcode_t*op = opcode_get(code->opcode);
    char*p = op->params;
    int pos = 0;
    int len = 0;
    
    if(tag)
        swf_SetU8(tag, code->opcode);
    len++;

    while(*p) {
        void*data = code->data[pos++];
        assert(pos<=2);
        if(*p == 'n') { // number
            len += swf_SetU30(tag, (ptroff_t)data);
        } else if(*p == '2') { //multiname
            multiname_t*m = (multiname_t*)data;
            len += swf_SetU30(tag, pool_register_multiname(pool, m));
        } else if(*p == 'm') { //method
            abc_method_t*m = (abc_method_t*)data;
            len += swf_SetU30(tag, m->index);
        } else if(*p == 'c') { //classinfo 
            abc_class_t*cls = (abc_class_t*)data;
            len += swf_SetU30(tag, cls->index);
        } else if(*p == 'i') { //methodbody
            abc_method_body_t*m = (abc_method_body_t*)data;
            len += swf_SetU30(tag, m->index);
        } else if(*p == 'u') { // integer
            len += swf_SetU30(tag, (ptroff_t)data);
        } else if(*p == 'r') { // integer
            len += swf_SetU30(tag, (ptroff_t)data);
        } else if(*p == 'b') { // byte
            if(tag)
                swf_SetU8(tag, (ptroff_t)data);
            len++;
        } else if(*p == 'j') { // jump
            len += swf_SetS24(tag, (ptroff_t)data);
        } else if(*p == 's') { // string
            int index = pool_register_string(pool, data);
            len += swf_SetU30(tag, index);
        } else if(*p == 'D') { // debug statement
            if(tag)
                swf_SetU8(tag, 1);
            len++;
            len+=swf_SetU30(tag, pool_register_string(pool,code->data[0]));
            if(tag)
                swf_SetU8(tag, (ptroff_t)code->data[1]);
            len++;
            len+=swf_SetU30(tag, 0);
        } else if(*p == 'S') { // switch statement
            len+=swf_SetU24(tag, 0); //default
            len+=swf_SetU30(tag, 0); //nr-1
            len+=swf_SetU24(tag, 0); //first
        } else {
            printf("Can't parse opcode param type \"%c\"\n", *p);
        }
        p++;
    }
    return len;
}

void code_write(TAG*tag, abc_code_t*code, pool_t*pool, abc_file_t*file)
{
    int pos = 0;
    int length = 0;
    abc_code_t*c = code;
    while(c) {
        c->pos = pos;
        pos += opcode_write(0, c, pool, file);
        c = c->next;
    }
    length = pos;
    swf_SetU30(tag, pos);
    int start = tag->len;
    c = code;
    pos = 0;
    while(c) {
        opcode_t*op = opcode_get(code->opcode);
        if(op->flags&(OP_BRANCH|OP_JUMP)) {
            int skip = 0;
            if(c->branch)
                skip = (c->branch->pos) - pos - 4;
            else
                skip = length - pos - 4;
            c->data[0] = (void*)(ptroff_t)skip;
        }
        pos += opcode_write(tag, c, pool, file);
        c = c->next;
    }
    assert(tag->len - start == pos);
}


typedef struct {
    int stackpos;
    int scopepos;
    abc_code_t*code;
    char seen;
} stackpos_t;

typedef struct {
    stackpos_t*stack;
    int num;
    int maxlocal;
    int maxstack;
    int maxscope;
} currentstats_t;

static int stack_minus(abc_code_t*c)
{
    opcode_t*op = opcode_get(c->opcode);
    assert(op->stack_minus<=0);
    int stack = op->stack_minus;
    if(op->flags&OP_STACK_NS) {
        multiname_t*m = (multiname_t*)c->data[0];
        stack--; //read namespace
        if(m->type == RTQNAMEL || m->type == RTQNAMELA) {
            stack--; //read name
        }
    } 
    if(op->flags&OP_STACK_ARGS || op->flags&OP_STACK_ARGS2) {
        assert(strchr(op->params, 'n'));
        int nr = (ptroff_t)(op->params[0]=='n'?c->data[0]:c->data[1]);
        stack-=nr;
        if(op->flags&OP_STACK_ARGS2)
            stack-=nr;
    }
    return stack;
}
static void handleregister(currentstats_t*stats, int reg)
{
    if(reg > stats->maxlocal)
        stats->maxlocal = reg;
}

static void callcode(currentstats_t*stats, int pos, int stack, int scope)
{
    while(pos<stats->num) {
        if(stats->stack[pos].seen) {
            if(stats->stack[pos].stackpos != stack ||
               stats->stack[pos].scopepos != scope) {
                fprintf(stderr, "Stack mismatch at pos %d\n", pos);
            }
            return;
        }
        stats->stack[pos].seen = 1;
        stats->stack[pos].stackpos = stack;
        stats->stack[pos].scopepos = scope;

        abc_code_t*c = stats->stack[pos].code;
        opcode_t*op = opcode_get(c->opcode);

        stack += stack_minus(c);

        if(stack<0) {
            fprintf(stderr, "error: stack underflow at %d (%s)\n", pos, op->name);
            return;
        }

        stack += op->stack_plus;
        scope += op->scope_stack_plus;

        if(stack > stats->maxstack)
            stats->maxstack = stack;
        if(scope > stats->maxscope)
            stats->maxscope = scope;
        
        if(op->flags & OP_REGISTER) {
            char*p = op->params;
            int pos = 0;
            char ok=0;
            while(*p) {
                if(*p=='r') {
                    handleregister(stats, (ptroff_t)c->data[pos]);
                    ok = 1;
                }
                p++;
            }
            if(!ok) {
                handleregister(stats, c->opcode&3);
            }
        }
        if(op->flags & (OP_THROW|OP_RETURN))
            return;
        if(op->flags & OP_JUMP) {
            c = c->branch;
            pos = c->pos;
            continue;
        }
        if(op->flags & OP_BRANCH) {
            int newpos = c->branch->pos;
            callcode(stats, newpos, stack, scope);
        }
    
        pos++;
        if(pos<stats->num) {
            assert(c->next == stats->stack[pos].code);
        }
    }
}

codestats_t code_get_statistics(abc_code_t*code) 
{
    int num = 0;
    abc_code_t*c = code;
    while(c) {
        num++;
        c = c->next;
    }
    currentstats_t current;
    current.stack = rfx_calloc(sizeof(stackpos_t)*num);
    current.maxlocal = 0;
    current.maxstack = 0;
    current.maxscope = 0;
    current.num = num;

    num = 0;
    c = code;
    while(c) {
        //crosslink
        current.stack[num].code = c;
        c->pos = num;
        num++;
        c = c->next;
    }
    callcode(&current, 0, 0, 0);

    free(current.stack);

    codestats_t stats;
    stats.local_count = current.maxlocal;
    stats.max_stack = current.maxstack;
    stats.max_scope_depth = current.maxscope;
    return stats;
}
abc_code_t* add_opcode(abc_code_t*atag, U8 op)
{
    abc_code_t*tmp = (abc_code_t*)malloc(sizeof(abc_code_t));
    tmp->opcode = op;
    tmp->next = 0;
    if(atag) {
	tmp->prev = atag;
	atag->next = tmp;
	tmp->parent = atag->parent;
    } else {
	tmp->prev = 0;
	tmp->parent = tmp;
    }
    return tmp;
}

