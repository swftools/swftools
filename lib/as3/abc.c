/* abc.c

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

#include <stdarg.h>
#include <assert.h>
#include "../rfxswf.h"
#include "../q.h"
#include "abc.h"

char stringbuffer[2048];

typedef struct _opcode
{
    unsigned char opcode;
    char*name;
    char*params;
} opcode_t;

/* 2 = multiname
   m = method
   n = number of params
   i = method info
   b = byte
   s = short
   c = class
   s = string
   S = switch
*/

int abc_RegisterNameSpace(abc_file_t*file, const char*name);
int abc_RegisterPackageNameSpace(abc_file_t*file, const char*name);
int abc_RegisterPackageInternalNameSpace(abc_file_t*file, const char*name);
int abc_RegisterProtectedNameSpace(abc_file_t*file, const char*name);
int abc_RegisterExplicitNameSpace(abc_file_t*file, const char*name);
int abc_RegisterStaticProtectedNameSpace(abc_file_t*file, const char*name);
int abc_RegisterPrivateNameSpace(abc_file_t*file, const char*name);


opcode_t opcodes[]={
{0xa0, "add", ""},
{0xc5, "add_i", ""},
{0x86, "atype", "2"},
{0x87, "astypelate", ""},
{0xA8, "bitand", ""},
{0x97, "bitnot", ""},
{0xa9, "bitor", ""},
{0xaa, "bitxor", ""},
{0x41, "call", "n"},
{0x43, "callmethod", "mn"},
{0x4c, "callproplex", "2n"},
{0x46, "callproperty", "2n"},
{0x4f, "callpropvoid", "2n"},
{0x44, "callstatic", "in"},
{0x45, "callsuper", "2n"},
{0x4e, "callsupervoid", "2n"},
{0x78, "checkfilter", ""},
{0x80, "coerce", "2"},
{0x82, "coerce_a", ""},
{0x85, "coerce_s", ""},
{0x42, "construct", "n"},
{0x4a, "constructprop", "2n"},
{0x49, "constructsuper", "n"},
{0x76, "convert_b", ""},
{0x73, "convert_i", ""},
{0x75, "convert_d", ""},
{0x77, "convert_o", ""},
{0x74, "convert_u", ""},
{0x70, "convert_s", ""},
{0xef, "debug", "bsbu"},
{0xf1, "debugfile", "s"},
{0xf0, "debugline", "u"},
{0x94, "declocal", "u"},
{0xc3, "declocal_i", "u"},
{0x93, "decrement", ""},
{0xc1, "decrement_i", ""},
{0x6a, "deleteproperty", "2"},
{0xa3, "divide", ""},
{0x2a, "dup", ""},
{0x06, "dxns", "s"},
{0x07, "dxnslate", ""},
{0xab, "equals", ""},
{0x72, "esc_xattr", ""},
{0x71, "esc_xelem", ""},
{0x5e, "findproperty", "2"},
{0x5d, "findpropstrict", "2"},
{0x59, "getdescendants", "2"},
{0x64, "getglobalscope", ""},
{0x6e, "getglobalslot", "u"},
{0x60, "getlex", "2"},
{0x62, "getlocal", "u"},
{0xd0, "getlocal_0", ""},
{0xd1, "getlocal_1", ""},
{0xd2, "getlocal_2", ""},
{0xd3, "getlocal_3", ""},
{0x66, "getproperty", "2"},
{0x65, "getscopeobject", "u"},
{0x6c, "getslot", "u"},
{0x04, "getsuper", "2"},
{0xaf, "greaterequals", ""},
{0x1f, "hasnext", ""},
{0x32, "hasnext2", "uu"},
{0x13, "ifeq", "j"},
{0x12, "iffalse", "j"},
{0x18, "ifge", "j"},
{0x17, "ifgt", "j"},
{0x16, "ifle", "j"},
{0x15, "iflt", "j"},
{0x0f, "ifnge", "j"},
{0x0e, "ifngt", "j"},
{0x0d, "ifnle", "j"},
{0x0c, "ifnlt", "j"},
{0x14, "ifne", "j"},
{0x19, "ifstricteq", "j"},
{0x1a, "ifstrictne", "j"},
{0x11, "iftrue", "j"},
{0xb4, "in", ""},
{0x92, "inclocal", "u"},
{0xc2, "inclocal_i", "u"},
{0x91, "increment", ""},
{0xc0, "increment_i", ""},
{0x68, "initproperty", "2"},
{0xb1, "instanceof", ""},
{0xb2, "istype", "2"},
{0xb3, "istypelate", ""},
{0x10, "jump", "j"},
{0x08, "kill", "u"},
{0x09, "label", ""},
{0xae, "lessequals", ""},
{0xad, "lessthan", ""},
{0x1b, "lookupswitch", "S"},
{0xa5, "lshift", ""},
{0xa4, "modulo", ""},
{0xa2, "multiply", ""},
{0xc7, "multiply_i", ""},
{0x90, "negate", ""},
{0xc4, "negate_i", ""},
{0x57, "newactivation", ""},
{0x56, "newarray", "u"},
{0x5a, "newcatch", "u"}, //index into exception_info
{0x58, "newclass", "c"}, //index into class_info
{0x40, "newfunction", "u"}, //index into method_info
{0x55, "newobject", "u"},
{0x1e, "nextname", ""},
{0x23, "nextvalue", ""},
{0x02, "nop", ""},
{0x96, "not", ""},
{0x29, "pop", ""},
{0x1d, "popscope", ""},
{0x24, "pushbyte", "b"},
{0x2f, "pushdouble", "u"}, //index into floats
{0x27, "pushfalse", ""},
{0x2d, "pushint", "u"}, //index into ints
{0x31, "pushnamespace", "u"}, //index into namespace
{0x28, "pushnan", ""},
{0x20, "pushnull", ""},
{0x30, "pushscope", ""},
{0x25, "pushshort", "u"},
{0x2c, "pushstring", "s"},
{0x26, "pushtrue", ""},
{0x2e, "pushuint", "u"}, //index into uints
{0x21, "pushundefined", ""},
{0x1c, "pushwith", ""},
{0x48, "returnvalue", ""},
{0x47, "returnvoid", ""},
{0xa6, "rshift", ""},
{0x63, "setlocal", "u"},
{0xd4, "setlocal_0", ""},
{0xd5, "setlocal_1", ""},
{0xd6, "setlocal_2", ""},
{0xd7, "setlocal_3", ""},
{0x6f, "setglobalshot", "u"},
{0x61, "setproperty", "2"},
{0x6d, "setslot", "u"},
{0x05, "setsuper", "2"},
{0xac, "strictequals", ""},
{0xa1, "subtract", ""},
{0xc6, "subtract_i", ""},
{0x2b, "swap", ""},
{0x03, "throw", ""},
{0x95, "typeof", ""},
{0xa7, "urshift", ""},
{0xb0, "xxx", ""},
};

static U8 op2index[256] = {255};

opcode_t* opcode_get(U8 op)
{
    int t;
    if(op2index[0]==255) 
        memset(op2index, 0, sizeof(op2index));

    if(op2index[op])
        return &opcodes[op2index[op]];

    for(t=0;t<sizeof(opcodes)/sizeof(opcodes[0]);t++) {
        op2index[opcodes[t].opcode] = t;
        if(opcodes[t].opcode==op)
            return &opcodes[t];
    }
    return 0;
}

static void code_free(abc_code_t*code)
{
    while(code) {
        abc_code_t*next = code->next;
        memset(code, 0, sizeof(code));
        free(code);
        code = next;
    }
}

static abc_code_t*parse_code(TAG*tag, int len, abc_file_t*file, pool_t*pool)
{
    abc_code_t*head=0;
    abc_code_t*code=0;
    int end=tag->pos+len;
    while(tag->pos<end) {
        U8 opcode = swf_GetU8(tag);
        opcode_t*op = opcode_get(opcode);
	if(!op) {
	    fprintf(stderr, "Can't parse opcode %02x\n", opcode);
	    return head;
        }
        NEW(abc_code_t,c);
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
                data = array_getvalue(pool->multinames, swf_GetU30(tag));
            } else if(*p == 'm') { //method
                data = array_getvalue(file->methods, swf_GetU30(tag));
            } else if(*p == 'c') { //classinfo
                data = array_getvalue(file->classes, swf_GetU30(tag));
            } else if(*p == 'i') { //methodbody TODO
                data = array_getvalue(file->method_bodies, swf_GetU30(tag));
            } else if(*p == 'u') { // lookoup TODO
                data = (void*)(ptroff_t)swf_GetU30(tag);
            } else if(*p == 'b') { // byte
                data = (void*)(ptroff_t)swf_GetU8(tag);
            } else if(*p == 'j') { // jump TODO
                data = (void*)(ptroff_t)swf_GetS24(tag);
            } else if(*p == 's') { // string
                data = (void*)array_getkey(pool->strings, swf_GetU30(tag));
            } else if(*p == 'S') { // switch statement TODO
                /* I hate these things */
                swf_GetU24(tag); //default
                int num = swf_GetU30(tag)+1;
                int t;
                for(t=0;t<num;t++) 
                    swf_GetU24(tag);
                data = 0;
            } else {
                printf("Can't parse opcode param type \"%c\"\n", *p);
                return 0;
            }
            code->params[pos++] = data;
            p++;
        }
    }
    return head;
}

static int dump_code(abc_code_t*c, abc_file_t*file, char*prefix, FILE*fo)
{
    pool_t*pool = pool_new();

    while(c) {
	U8 opcode = c->opcode;
	int t;
	char found = 0;
        opcode_t*op = opcode_get(opcode);
	if(!op) {
	    fprintf(stderr, "Can't parse opcode %02x\n", opcode);
	    return 0;
	} else {
            fprintf(fo, "%s%s ", prefix, op->name);
            char*p = op->params;
            char first = 1;
            int pos=0;
            while(*p) {
                void*data = c->params[pos];
                if(pos>0)
                    printf(", ");

                if(*p == 'n') {
                    int n = (ptroff_t)data;
                    printf("%d params", n);
                } else if(*p == '2') {
                    multiname_t*n = (multiname_t*)data;
                    char* m = multiname_to_string(n);
                    printf("%s", m);
                    free(m);
                } else if(*p == 'm') {
                    int n = (ptroff_t)data;
                    abc_method_t*cls = (abc_method_t*)array_getvalue(file->methods, n);
                    printf("[method%d]", n);
                } else if(*p == 'c') {
                    int n = (ptroff_t)data;
                    abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, n);
                    printf("[classinfo%d %s]", n, cls->classname);
                } else if(*p == 'i') {
                    int n = (ptroff_t)data;
                    abc_method_body_t*cls = (abc_method_body_t*)array_getvalue(file->method_bodies, n);
                    printf("[methodbody%d]", n);
                } else if(*p == 'u') {
                    int n = (ptroff_t)data;
                    printf("%d", n);
                } else if(*p == 'b') {
                    int b = (ptroff_t)data;
                    printf("%02x", b);
                } else if(*p == 'j') {
                    int n = (ptroff_t)data;
                    printf("%d", n);
                } else if(*p == 's') {
                    printf("\"%s\"", data);
                } else if(*p == 'S') {
                    printf("[switch data]");
                } else {
                    fprintf(stderr, "Can't parse opcode param type \"%c\"\n", *p);
                    return 0;
                }
                p++;
                pos++;
                first = 0;
            }
            found = 1;
            break;
	}
        c = c->next;
    }
    return 1;
}

static int opcode_write(TAG*tag, abc_code_t*code, pool_t*pool, abc_file_t*file)
{
    opcode_t*c = opcode_get(code->opcode);
    char*p = c->params;
    int pos = 0;
    int len = 0;
    while(*p) {
        void*data = code->params[pos++];
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
        } else if(*p == 'b') { // byte
            if(tag)
                swf_SetU8(tag, (ptroff_t)data);
            len++;
        } else if(*p == 'j') { // jump
            len += swf_SetS24(tag, (ptroff_t)data);
        } else if(*p == 's') { // string
            int index = pool_register_string(pool, data);
            len += swf_SetU30(tag, index);
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


static char* params_to_string(multiname_list_t*list)
{
    multiname_list_t*l;
    int n;

    l = list;
    n = 0;
    while(list) {
        n++;list=list->next;
    }

    char**names = (char**)malloc(sizeof(char*)*n);
    
    l = list;
    n = 0;
    int size = 0;
    while(list) {
        names[n] = multiname_to_string(list->multiname);
        size += strlen(names[n]) + 2;
        n++;list=list->next;
    }

    char* params = malloc(size+5);
    params[0]='(';
    params[1]=0;
    l = list;
    int s=0;
    n = 0;
    while(list) {
        if(s)
            strcat(params, ", ");
        strcat(params, names[n]);
        free(names[n]);
        n++;
        s=1;
    }
    free(names);
    strcat(params, ")");
    int t;
    return params;
}

//#define DEBUG
#define DEBUG if(0)

static void parse_metadata(TAG*tag, abc_file_t*file, pool_t*pool)
{
    int t;
    int num_metadata = swf_GetU30(tag);

    DEBUG printf("%d metadata\n");
    for(t=0;t<num_metadata;t++) {
        const char*entry_name = array_getkey(pool->strings, swf_GetU30(tag));
        int num = swf_GetU30(tag);
        int s;
        DEBUG printf("  %s\n", entry_name);
        array_t*items = array_new();
        for(s=0;s<num;s++) {
            const char*key = array_getkey(pool->strings, swf_GetU30(tag));
            const char*value = array_getkey(pool->strings, swf_GetU30(tag));
            DEBUG printf("    %s=%s\n", key, value);
            array_append(items, key, value);
        }
        array_append(file->metadata, entry_name, items);
    }
}

#define TRAIT_SLOT 0
#define TRAIT_METHOD 1
#define TRAIT_GETTER 2
#define TRAIT_SETTER 3
#define TRAIT_CLASS 4
#define TRAIT_FUNCTION 5
#define TRAIT_CONST 6

static void traits_free(trait_list_t*traits) 
{
    trait_list_t*t = traits;
    while(t) {
        free(t->trait);t->trait = 0;
        t = t->next;
    }
    list_free(traits);
}

static trait_list_t* traits_parse(TAG*tag, pool_t*pool, abc_file_t*file)
{
    int num_traits = swf_GetU30(tag);
    trait_list_t*traits = list_new();
    int t;
    if(num_traits) {
        DEBUG printf("%d traits\n", num_traits);
    }
    
    for(t=0;t<num_traits;t++) {
	trait_t*trait = malloc(sizeof(trait_t));
	memset(trait, 0, sizeof(trait_t));
	list_append(traits, trait);
	trait->name = pool_lookup_multiname(pool, swf_GetU30(tag));
	const char*name = 0;
	DEBUG name = multiname_to_string(trait->name);
	U8 kind = trait->type = swf_GetU8(tag);
        U8 attributes = kind&0xf0;
        kind&=0x0f;
	DEBUG printf("  trait %d) %s type=%02x\n", t, name, kind);
	if(kind == TRAIT_METHOD || kind == TRAIT_GETTER || kind == TRAIT_SETTER) { // method / getter / setter
	    trait->disp_id = swf_GetU30(tag);
	    trait->method = (abc_method_t*)array_getvalue(file->methods, swf_GetU30(tag));
	    DEBUG printf("  method/getter/setter\n");
	} else if(kind == TRAIT_FUNCTION) { // function
	    trait->slot_id =  swf_GetU30(tag);
	    trait->method = (abc_method_t*)array_getvalue(file->methods, swf_GetU30(tag));
	} else if(kind == TRAIT_CLASS) { // class
	    trait->slot_id = swf_GetU30(tag);
	    trait->cls = (abc_class_t*)array_getvalue(file->classes, swf_GetU30(tag));
	    DEBUG printf("  class %s %d %d\n", name, trait->slot_id, trait->cls);
	} else if(kind == TRAIT_SLOT || kind == TRAIT_CONST) { // slot, const
            /* a slot is a variable in a class that is shared amonst all instances
               of the same type, but which has a unique location in each object 
               (in other words, slots are non-static, traits are static)
             */
	    trait->slot_id = swf_GetU30(tag);
            trait->type_name = (multiname_t*)array_getvalue(pool->multinames, swf_GetU30(tag));
	    trait->vindex = swf_GetU30(tag);
	    if(trait->vindex) {
		trait->vkind = swf_GetU8(tag);
	    }
	    DEBUG printf("  slot %s %d %s (vindex=%d)\n", name, trait->slot_id, trait->type_name->name, trait->vindex);
	} else {
	    fprintf(stderr, "Can't parse trait type %d\n", kind);
	}
        if(attributes&0x40) {
            int num = swf_GetU30(tag);
            int s;
            for(s=0;s<num;s++) {
                swf_GetU30(tag); //index into metadata array
            }
        }
    }
    return traits;
}

void traits_skip(TAG*tag)
{
    int num_traits = swf_GetU30(tag);
    int t;
    for(t=0;t<num_traits;t++) {
        swf_GetU30(tag);
	U8 kind = swf_GetU8(tag);
        U8 attributes = kind&0xf0;
        kind&=0x0f;
        swf_GetU30(tag);
        swf_GetU30(tag);
	if(kind == TRAIT_SLOT || kind == TRAIT_CONST) {
	    if(swf_GetU30(tag)) swf_GetU8(tag);
        } else if(kind>TRAIT_CONST) {
	    fprintf(stderr, "Can't parse trait type %d\n", kind);
	}
        if(attributes&0x40) {
            int s, num = swf_GetU30(tag);
            for(s=0;s<num;s++) swf_GetU30(tag);
        }
    }
}

void swf_CopyData(TAG*to, TAG*from, int len)
{
    unsigned char*data = malloc(len);
    swf_GetBlock(from, data, len);
    swf_SetBlock(to, data, len);
    free(data);
}

abc_file_t*abc_file_new()
{
    abc_file_t*f = malloc(sizeof(abc_file_t));
    memset(f, 0, sizeof(abc_file_t));
    f->metadata = array_new();
    f->methods = array_new();
    f->classes = array_new();
    f->scripts = array_new();
    f->method_bodies = array_new();

    return f;
}

static namespace_t* namespace_new(U8 access, const char*name)
{
    namespace_t*ns = malloc(sizeof(namespace_t));
    memset(ns, 0, sizeof(namespace_t));

    if(access==0) { // autodetect access
	char*n = strdup(name);
	if(n[0] == '[') {
	    char*bracket = strchr(n, ']');
	    if(bracket) {
		*bracket = 0;
		char*a = n+1;
		name += (bracket-n)+1;
		if(!strcmp(a, "")) access=0x16;
		else if(!strcmp(a, "package")) access=0x16;
		else if(!strcmp(a, "packageinternal")) access=0x17;
		else if(!strcmp(a, "protected")) access=0x18;
		else if(!strcmp(a, "explicit")) access=0x19;
		else if(!strcmp(a, "staticprotected")) access=0x1a;
		else if(!strcmp(a, "private")) access=0x05;
		else {
		    fprintf(stderr, "Undefined access level: [%s]\n", a);
                    return 0;
		}
	    }
	} else {
	    access = 0x16;
	}
	free(n);
    }
    ns->access = access;
    ns->name = strdup(name);
    return ns;
}
namespace_t* namespace(abc_file_t*file, const char*name) {
    return namespace_new(0x08, name);
}
namespace_t* abc_packagenamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x16 , name);
}
namespace_t* abc_packageinternalnamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x17, name);
}
namespace_t* abc_protectednamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x18, name);
}
namespace_t* abc_explicitnamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x19, name);
}
namespace_t* abc_staticprotectednamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x1a, name);
}
namespace_t* abc_privatenamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x05, name);
}

#define CLASS_SEALED 1
#define CLASS_FINAL 2
#define CLASS_INTERFACE 4
#define CLASS_PROTECTED_NS 8

abc_class_t* abc_class_new(abc_file_t*pool, char*classname, char*superclass) {
    abc_class_t* c = malloc(sizeof(abc_class_t));
    memset(c, 0, sizeof(abc_class_t));
    c->index = array_append(pool->classes, classname, c);
    c->pool = pool;
    c->classname = multiname_fromstring(classname);
    c->superclass = superclass?multiname_fromstring(superclass):0;
    c->flags = 0;
    c->constructor = 0;
    c->static_constructor = 0;
    c->traits = list_new();
    return c;
}

void abc_class_sealed(abc_class_t*c)
{
    c->flags |= CLASS_SEALED;
}
void abc_class_final(abc_class_t*c)
{
    c->flags |= CLASS_FINAL;
}
void abc_class_interface(abc_class_t*c)
{
    c->flags |= CLASS_INTERFACE;
}
void abc_class_protectedNS(abc_class_t*c, char*namespace)
{
    c->protectedNS = namespace;
    c->flags |= CLASS_PROTECTED_NS;
}

abc_method_body_t* add_method(abc_file_t*pool, abc_class_t*cls, char*returntype, int num_params, va_list va)
{
    /* construct code (method body) object */
    abc_method_body_t* c = malloc(sizeof(abc_method_body_t));
    memset(c, 0, sizeof(abc_method_body_t));
    c->index = array_append(pool->method_bodies, 0, c);
    c->pool = pool;
    c->traits = list_new();
    c->code = 0;

    /* construct method object */
    abc_method_t* m = malloc(sizeof(abc_method_t));
    memset(m, 0, sizeof(abc_method_t));
    m->index = array_append(pool->methods, 0, m);
    if(returntype && strcmp(returntype, "void")) {
	m->return_type = multiname_fromstring(returntype);
    } else {
	m->return_type = 0;
    }
    int t;
    for(t=0;t<num_params;t++) {
	const char*param = va_arg(va, const char*);
	list_append(m->parameters, multiname_fromstring(param));
    }

    /* crosslink the two objects */
    m->body = c;
    c->method = m;

    return c;
}

abc_method_body_t* abc_class_constructor(abc_class_t*cls, char*returntype, int num_params, ...) 
{
    va_list va;
    va_start(va, num_params);
    abc_method_body_t* c = add_method(cls->pool, cls, returntype, num_params, va);
    va_end(va);
    cls->constructor = c->method;
    return c;
}

abc_method_body_t* abc_class_staticconstructor(abc_class_t*cls, char*returntype, int num_params, ...) 
{
    va_list va;
    va_start(va, num_params);
    abc_method_body_t* c = add_method(cls->pool, cls, returntype, num_params, va);
    va_end(va);
    cls->static_constructor = c->method;
    return c;
}

trait_t*trait_new(int type, multiname_t*name, int data1, int data2, int vindex, int vkind)
{
    trait_t*trait = malloc(sizeof(trait_t));
    memset(trait, 0, sizeof(trait_t));
    trait->type = type;
    trait->name = name;
    trait->data1 = data1;
    trait->data2 = data2;
    trait->vindex = vindex;
    trait->vkind = vkind;
    return trait;
}

abc_method_body_t* abc_class_method(abc_class_t*cls, char*returntype, char*name, int num_params, ...)
{
    abc_file_t*pool = cls->pool;
    va_list va;
    va_start(va, num_params);
    abc_method_body_t* c = add_method(cls->pool, cls, returntype, num_params, va);
    va_end(va);
    list_append(cls->traits, trait_new(TRAIT_METHOD, multiname_fromstring(name), 0, c->method->index, 0, 0));
    return c;
}

void abc_AddSlot(abc_class_t*cls, char*name, int slot, char*multiname)
{
    abc_file_t*pool = cls->pool;
    multiname_t*m = multiname_fromstring(multiname);
    list_append(cls->traits, trait_new(TRAIT_SLOT, m, slot, 0, 0, 0));
}

void abc_method_body_addClassTrait(abc_method_body_t*code, char*multiname, int slotid, abc_class_t*cls)
{
    abc_file_t*pool = code->pool;
    multiname_t*m = multiname_fromstring(multiname);
    trait_t*trait = trait_new(TRAIT_CLASS, m, slotid, 0, 0, 0);
    trait->cls = cls;
    list_append(code->traits, trait);
}

/* notice: traits of a method (body) belonging to an init script
   and traits of the init script are *not* the same thing */
void abc_initscript_addClassTrait(abc_script_t*script, char*multiname, int slotid, abc_class_t*cls)
{
    abc_file_t*pool = script->pool;
    multiname_t*m = multiname_fromstring(multiname);
    trait_t*trait = trait_new(TRAIT_CLASS, m, slotid, 0, 0, 0);
    trait->cls = cls;
    list_append(script->traits, trait);
}

abc_script_t* abc_initscript(abc_file_t*pool, char*returntype, int num_params, ...) 
{
    va_list va;
    va_start(va, num_params);
    abc_method_body_t* c = add_method(pool, 0, returntype, num_params, va);
    abc_script_t* s = malloc(sizeof(abc_script_t));
    s->method = c->method;
    s->traits = list_new();
    s->pool = pool;
    array_append(pool->scripts, 0, s);
    va_end(va);
    return s;
}

static void write_traits(pool_t*pool, TAG*tag, trait_list_t*traits)
{
    if(!traits) {
        swf_SetU30(tag, 0);
        return;
    }
    swf_SetU30(tag, list_length(traits));
    int s;

    while(traits) {
	trait_t*trait = traits->trait;

	swf_SetU30(tag, pool_register_multiname(pool, trait->name));
	swf_SetU8(tag, trait->type);

	swf_SetU30(tag, trait->data1);

        if(trait->type == TRAIT_CLASS) {
            swf_SetU30(tag, trait->cls->index);
        } else if(trait->type == TRAIT_GETTER ||
                  trait->type == TRAIT_SETTER ||
                  trait->type == TRAIT_METHOD) {
            swf_SetU30(tag, trait->method->index);
        } else if(trait->type == TRAIT_SLOT ||
                  trait->type == TRAIT_CONST) {
            int index = pool_register_multiname(pool, trait->type_name);
            swf_SetU30(tag, index);
        } else  {
	    swf_SetU30(tag, trait->data2);
        }

	if(trait->type == TRAIT_SLOT || trait->type == TRAIT_CONST) {
	    swf_SetU30(tag, trait->vindex);
	    if(trait->vindex) {
		swf_SetU8(tag, trait->vkind);
	    }
	}
        traits = traits->next;
    }
}

static void dump_traits(FILE*fo, const char*prefix, trait_list_t*traits, abc_file_t*file);

static void dump_method(FILE*fo, const char*prefix, const char*type, const char*name, abc_method_t*m, abc_file_t*file)
{
    const char*return_type = "";
    if(m->return_type)
        return_type = multiname_to_string(m->return_type);

    char*paramstr = params_to_string(m->parameters);

    fprintf(fo, "%s%s %s %s=%s %s\n", prefix, type, return_type, name, m->name, paramstr);

    abc_method_body_t*c = m->body;
    
    fprintf(fo, "%s[%d %d %d %d %d]\n", prefix, c->max_stack, c->local_count, c->init_scope_depth, c->max_scope_depth, c->exception_count);

    char prefix2[80];
    sprintf(prefix2, "%s    ", prefix);
    if(c->traits)
        dump_traits(fo, prefix, c->traits, file);
    fprintf(fo, "%s{\n", prefix);
    dump_code(c->code, file, prefix2, fo);
    fprintf(fo, "%s}\n\n", prefix);
}

static void dump_traits(FILE*fo, const char*prefix, trait_list_t*traits, abc_file_t*file)
{
    int t;
    while(traits) {
	trait_t*trait = traits->trait;
	char*name = multiname_to_string(trait->name);
	U8 kind = trait->type;
        U8 attributes = kind&0xf0;
        kind&=0x0f;
	if(kind == TRAIT_METHOD) {
            abc_method_t*m = trait->method;
	    dump_method(fo, prefix, "method", name, m, file);
	} else if(kind == TRAIT_GETTER) {
            abc_method_t*m = trait->method;
	    dump_method(fo, prefix, "getter", name, m, file);
        } else if(kind == TRAIT_SETTER) {
            abc_method_t*m = trait->method;
	    dump_method(fo, prefix, "setter", name, m, file);
	} else if(kind == TRAIT_FUNCTION) { // function
            abc_method_t*m = trait->method;
	    dump_method(fo, prefix, "function", name, m, file);
	} else if(kind == TRAIT_CLASS) { // class
            abc_class_t*cls = trait->cls;
            if(!cls) {
	        fprintf(fo, "%sslot %d: class %s=class%d\n", prefix, trait->slot_id, name);
            } else {
	        fprintf(fo, "%sslot %d: class %s=%s\n", prefix, trait->slot_id, name, cls->classname);
            }
	} else if(kind == TRAIT_SLOT || kind == TRAIT_CONST) { // slot, const
	    int slot_id = trait->slot_id;
	    char*type_name = multiname_to_string(trait->type_name);
	    fprintf(fo, "%sslot %s %d %s (vindex=%d)\n", prefix, name, trait->slot_id, type_name, trait->vindex);
            free(type_name);
	} else {
	    fprintf(fo, "%s    can't dump trait type %d\n", prefix, kind);
	}
        free(name);
        traits=traits->next;
    }
}

void* swf_DumpABC(FILE*fo, void*code, char*prefix)
{
    abc_file_t* file = (abc_file_t*)code;
        
    if(file->name)
        fprintf(fo, "%s#\n", prefix);
        fprintf(fo, "%s#name: %s\n", prefix, file->name);
        fprintf(fo, "%s#\n", prefix);

    int t;
    for(t=0;t<file->metadata->num;t++) {
        const char*entry_name = array_getkey(file->metadata, t);
        fprintf(fo, "%s#Metadata \"%s\":\n", prefix, entry_name);
        int s;
        array_t*items = (array_t*)array_getvalue(file->metadata, t);
        for(s=0;s<items->num;s++) {
            fprintf(fo, "%s#  %s=%s\n", prefix, array_getkey(items, s), array_getvalue(items,s));
        }
    }

    for(t=0;t<file->classes->num;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
        char prefix2[80];
        sprintf(prefix2, "%s    ", prefix);

        fprintf(fo, "%s", prefix);
        if(cls->flags&1) fprintf(fo, "sealed ");
        if(cls->flags&2) fprintf(fo, "final ");
        if(cls->flags&4) fprintf(fo, "interface ");
        if(cls->flags&8) {
            fprintf(fo, "protectedNS<%s> ", cls->protectedNS);
        }

	fprintf(fo, "class %s", cls->classname);
        if(cls->superclass) {
            fprintf(fo, " extends %s", cls->superclass);
            multiname_list_t*ilist = cls->interfaces;
            if(ilist)
                fprintf(fo, " implements");
            while(ilist) {
                char*s = multiname_to_string(ilist->multiname);
                fprintf(fo, " %d", s);
                free(s);
                ilist = ilist->next;
            }
            ilist->next;
            
        }
        if(cls->flags&0xf0) 
            fprintf(fo, "extra flags=%02x\n", cls->flags&0xf0);
	fprintf(fo, "%s{\n", prefix);
	
        dump_method(fo, prefix2,"staticconstructor", "", cls->static_constructor, file);
        dump_traits(fo, prefix2, cls->static_constructor_traits, file);
	
        char*n = multiname_to_string(cls->classname);
	dump_method(fo, prefix2, "constructor", n, cls->constructor, file);
        free(n);
	dump_traits(fo, prefix2,cls->traits, file);
        fprintf(fo, "%s}\n", prefix);
    }
    fprintf(fo, "%s\n", prefix);

    for(t=0;t<file->scripts->num;t++) {
        abc_script_t*s = (abc_script_t*)array_getvalue(file->scripts, t);
        dump_method(fo, prefix,"initmethod", "init", s->method, file);
        dump_traits(fo, prefix, s->traits, file);
    }
    return file;
}

void* swf_ReadABC(TAG*tag)
{
    abc_file_t* file = abc_file_new();
    pool_t*pool = pool_new();

    swf_SetTagPos(tag, 0);
    int t;
    if(tag->id == ST_DOABC) {
        U32 abcflags = swf_GetU32(tag);
        DEBUG printf("flags=%08x\n", abcflags);
        char*name= swf_GetString(tag);
        file->name = name?strdup(name):0;
    }
    U32 version = swf_GetU32(tag);
    if(version!=0x002e0010) {
        fprintf(stderr, "Warning: unknown AVM2 version %08x\n", version);
    }

    pool_read(pool, tag);

    int num_methods = swf_GetU30(tag);
    DEBUG printf("%d methods\n", num_methods);
    for(t=0;t<num_methods;t++) {
	abc_method_t*m = malloc(sizeof(abc_method_t));
	memset(m, 0, sizeof(*m));
	int param_count = swf_GetU30(tag);
	int return_type_index = swf_GetU30(tag);
        m->return_type = pool_lookup_multiname(pool, return_type_index);

	int s;
	for(s=0;s<param_count;s++) {
	    int type_index = swf_GetU30(tag);
            multiname_t*param = (multiname_t*)pool_lookup_multiname(pool, type_index);
            /* type_index might be 0, which probably means ... (varargs) */
            list_append(m->parameters, param);
        }

	int namenr = swf_GetU30(tag);
	m->name = "";
	if(namenr)
	    m->name = array_getkey(pool->strings, namenr);

	m->flags = swf_GetU8(tag);
	
        DEBUG printf("method %d) %s flags=%02x\n", t, params_to_string(m->parameters), m->flags);

        if(m->flags&0x08) {
            /* optional parameters */
            int num = swf_GetU30(tag);
            int s;
            for(s=0;s<num;s++) {
                int val = swf_GetU30(tag);
                U8 kind = swf_GetU8(tag); // specifies index type for "val"
            }
        }
	if(m->flags&0x80) {
            /* debug information- not used by avm2 */
            multiname_list_t*l = m->parameters;
            while(l) {
	        m->name = array_getkey(pool->strings, swf_GetU30(tag));
                l = l->next;
            }
	}
	array_append(file->methods, m->name, m);
    }
            
    parse_metadata(tag, file, pool);
	
    /* skip classes, and scripts for now, and do the real parsing later */
    int num_classes = swf_GetU30(tag);
    int classes_pos = tag->pos;
    DEBUG printf("%d classes\n", num_classes);
    for(t=0;t<num_classes;t++) {
	abc_class_t*cls = malloc(sizeof(abc_class_t));
	memset(cls, 0, sizeof(abc_class_t));
	array_append(file->classes, 0, cls);
	
        DEBUG printf("class %d\n", t);
	swf_GetU30(tag); //classname
	swf_GetU30(tag); //supername
	cls->flags = swf_GetU8(tag);
	if(cls->flags&8) 
	    swf_GetU30(tag); //protectedNS
        int s;
	int inum = swf_GetU30(tag); //interface count
        cls->interfaces = 0;
        for(s=0;s<inum;s++) {
            int interface_index = swf_GetU30(tag);
            multiname_t* m = (multiname_t*)array_getvalue(pool->multinames, interface_index);
            list_append(cls->interfaces, m);
            DEBUG printf("  class %d interface: %s\n", t, m->name);
        }

	swf_GetU30(tag); //iinit
	traits_skip(tag);
    }
    for(t=0;t<num_classes;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
	int cinit = swf_GetU30(tag);
        cls->static_constructor = (abc_method_t*)array_getvalue(file->methods, cinit);
        traits_skip(tag);
    }
    int num_scripts = swf_GetU30(tag);
    DEBUG printf("%d scripts\n", num_scripts);
    for(t=0;t<num_scripts;t++) {
	int init = swf_GetU30(tag);
        traits_skip(tag);
    }

    int num_method_bodies = swf_GetU30(tag);
    DEBUG printf("%d method bodies\n", num_method_bodies);
    for(t=0;t<num_method_bodies;t++) {
	int methodnr = swf_GetU30(tag);
	if(methodnr >= file->methods->num) {
	    printf("Invalid method number: %d\n", methodnr);
	    return 0;
	}
	abc_method_t*m = (abc_method_t*)array_getvalue(file->methods, methodnr);
	abc_method_body_t*c = malloc(sizeof(abc_method_body_t));
	memset(c, 0, sizeof(abc_method_body_t));
	c->max_stack = swf_GetU30(tag);
	c->local_count = swf_GetU30(tag);
	c->init_scope_depth = swf_GetU30(tag);
	c->max_scope_depth = swf_GetU30(tag);
	int code_length = swf_GetU30(tag);
	c->method = m;
	m->body = c;

        int pos = tag->pos + code_length;
        c->code = parse_code(tag, code_length, file, pool);
        tag->pos = pos;

	int exception_count = swf_GetU30(tag);
        int s;
        for(s=0;s<exception_count;s++) {
            swf_GetU30(tag); //from
            swf_GetU30(tag); //to
            swf_GetU30(tag); //target
            swf_GetU30(tag); //exc_type
            swf_GetU30(tag); //var_name
        }
	c->traits = traits_parse(tag, pool, file);

	DEBUG printf("method_body %d) (method %d), %d bytes of code", t, methodnr, code_length);

	array_append(file->method_bodies, 0, c);
    }
    if(tag->len - tag->pos) {
	fprintf(stderr, "%d unparsed bytes remaining in ABC block\n", tag->len - tag->pos);
	return 0;
    }

    swf_SetTagPos(tag, classes_pos);
    for(t=0;t<num_classes;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);

        int classname_index = swf_GetU30(tag);
        int superclass_index = swf_GetU30(tag);
	cls->classname = (multiname_t*)array_getvalue(pool->multinames, classname_index);
	cls->superclass = (multiname_t*)array_getvalue(pool->multinames, superclass_index);
	cls->flags = swf_GetU8(tag);
	const char*ns = "";
	if(cls->flags&8) {
            int ns_index = swf_GetU30(tag);
	    cls->protectedNS = array_getkey(pool->namespaces, ns_index);
	}
        
        int num_interfaces = swf_GetU30(tag); //interface count
        int s;
        for(s=0;s<num_interfaces;s++) {
            swf_GetU30(tag); // multiname index TODO
        }
	int iinit = swf_GetU30(tag);
        cls->constructor = (abc_method_t*)array_getvalue(file->methods, iinit);
	cls->traits = traits_parse(tag, pool, file);
    }
    for(t=0;t<num_classes;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
        /* SKIP */
	swf_GetU30(tag); // cindex
	cls->static_constructor_traits = traits_parse(tag, pool, file);
    }
    int num_scripts2 = swf_GetU30(tag);
    for(t=0;t<num_scripts2;t++) {
	int init = swf_GetU30(tag);
        abc_method_t*m = (abc_method_t*)array_getvalue(file->methods, init);
        
        abc_script_t*s = malloc(sizeof(abc_script_t));
        memset(s, 0, sizeof(abc_script_t));
        s->method = m;
        s->traits = traits_parse(tag, pool, file);
        array_append(file->scripts, 0, s);
        if(!s->traits) {
	    fprintf(stderr, "Can't parse script traits\n");
            return 0;
        }
    }

    //pool_destroy(pool);
    return file;
}

static void code_write(TAG*tag, abc_code_t*code, pool_t*pool, abc_file_t*file)
{
    int len = 0;
    abc_code_t*c = code;
    while(c) {
        len += opcode_write(0, c, pool, file);
        c = c->next;
    }
    swf_SetU30(tag, len);
    int pos = tag->len;
    c = code;
    while(c) {
        opcode_write(tag, c, pool, file);
        c = c->next;
    }
    assert(tag->len - pos == len);
}

void swf_WriteABC(TAG*abctag, void*code)
{
    abc_file_t*file = (abc_file_t*)code;
    pool_t*pool = pool_new();

    TAG*tmp = swf_InsertTag(0,0);
    TAG*tag = tmp;
    int t;

    swf_SetU30(tag, file->methods->num);
   
    /* enumerate classes, methods and method bodies */
    for(t=0;t<file->methods->num;t++) {
	abc_method_t*m = (abc_method_t*)array_getvalue(file->methods, t);
        m->index = t;
    }
    for(t=0;t<file->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)array_getvalue(file->classes, t);
        c->index = t;
    }
    for(t=0;t<file->method_bodies->num;t++) {
        abc_method_body_t*m = (abc_method_body_t*)array_getvalue(file->method_bodies, t);
        m->index = t;
    }

    for(t=0;t<file->methods->num;t++) {
	abc_method_t*m = (abc_method_t*)array_getvalue(file->methods, t);
        int n = 0;
        multiname_list_t*l = m->parameters;
	swf_SetU30(tag, list_length(m->parameters));
        if(m->return_type) 
	    swf_SetU30(tag, pool_register_multiname(pool, m->return_type));
        else
	    swf_SetU30(tag, 0);
	int s;
        while(l) {
	    swf_SetU30(tag, pool_register_multiname(pool, l->multiname));
            l = l->next;
	}
	swf_SetU30(tag, 0); // name
	swf_SetU8(tag, 0); //flags
    }
   
    /* write metadata */
    swf_SetU30(tag, file->metadata->num);
    for(t=0;t<file->metadata->num;t++) {
        const char*entry_name = array_getkey(file->metadata, t);
        swf_SetU30(tag, pool_register_string(pool, entry_name));
        array_t*items = (array_t*)array_getvalue(file->metadata, t);
        int s;
        for(s=0;s<items->num;s++) {
            int i1 = pool_register_string(pool, array_getkey(items, s));
            int i2 = pool_register_string(pool, array_getvalue(items, s));
            swf_SetU30(tag, i1);
            swf_SetU30(tag, i2);
        }
    }

    swf_SetU30(tag, file->classes->num);
    for(t=0;t<file->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)array_getvalue(file->classes, t);
   
        int classname_index = pool_register_multiname(pool, c->classname);
        int superclass_index = pool_register_multiname(pool, c->superclass);

	swf_SetU30(tag, classname_index);
	swf_SetU30(tag, superclass_index);

	swf_SetU8(tag, c->flags); // flags
        if(c->flags&0x08) {
            namespace_t*ns = abc_protectednamespace(file, c->protectedNS);
            int ns_index = pool_register_namespace(pool, ns);
	    swf_SetU30(tag, ns_index);
        }

	swf_SetU30(tag, 0); // no interfaces
	if(!c->constructor) {
	    fprintf(stderr, "Error: Class %s has no constructor\n", c->classname);
	    return;
	}
	swf_SetU30(tag, c->constructor->index);
	write_traits(pool, tag, c->traits);
    }
    for(t=0;t<file->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)array_getvalue(file->classes, t);
	if(!c->static_constructor) {
	    fprintf(stderr, "Error: Class %s has no static constructor\n", c->classname);
	    return;
	}
	swf_SetU30(tag, c->static_constructor->index);
        write_traits(pool, tag, c->static_constructor_traits);
    }

    swf_SetU30(tag, file->scripts->num);
    for(t=0;t<file->scripts->num;t++) {
	abc_script_t*s = (abc_script_t*)array_getvalue(file->scripts, t);
	swf_SetU30(tag, s->method->index); //!=t!
	write_traits(pool, tag, s->traits);
    }

    swf_SetU30(tag, file->method_bodies->num);
    for(t=0;t<file->method_bodies->num;t++) {
	abc_method_body_t*c = (abc_method_body_t*)array_getvalue(file->method_bodies, t);
	abc_method_t*m = c->method;
	swf_SetU30(tag, m->index);
	swf_SetU30(tag, c->max_stack);
	swf_SetU30(tag, c->local_count);
	swf_SetU30(tag, c->init_scope_depth);
	swf_SetU30(tag, c->max_scope_depth);

        code_write(tag, c->code, pool, file);

	swf_SetU30(tag, c->exception_count);
        write_traits(pool, tag, c->traits);
    }


    for(t=1;t<pool->multinames->num;t++) {
        multiname_t*m = (multiname_t*)array_getvalue(pool->multinames, t);
        if(m->ns) {
            pool_register_namespace(pool, m->ns);
        }
        if(m->namespace_set) {
            pool_register_namespace_set(pool, m->namespace_set);
        }
        if(m->name) {
            pool_register_string(pool, m->name);
        }
    }
    for(t=1;t<pool->namespace_sets->num;t++) {
        namespace_set_t*set = (namespace_set_t*)array_getvalue(pool->namespace_sets, t);
        namespace_list_t*i = set->namespaces;
        while(i) {
            pool_register_namespace(pool, i->namespace);
            i = i->next;
        }
    }
    for(t=1;t<pool->namespaces->num;t++) {
	namespace_t*ns= (namespace_t*)array_getvalue(pool->namespaces, t);
        array_append_if_new(pool->strings, ns->name, 0);
    }

    // --- start to write real tag --
    
    tag = abctag;

    if(tag->id == ST_DOABC) {
        swf_SetU32(tag, 1); // flags
        swf_SetString(tag, file->name);
    }

    swf_SetU16(tag, 0x10); //version
    swf_SetU16(tag, 0x2e);

    swf_SetU30(tag, pool->ints->num>1?pool->ints->num:0);
    for(t=1;t<pool->ints->num;t++) {
        S32 val = (ptroff_t)array_getvalue(pool->ints, t);
        swf_SetS30(tag, val);
    }
    swf_SetU30(tag, pool->uints->num>1?pool->uints->num:0);
    for(t=1;t<pool->uints->num;t++) {
        swf_SetU30(tag, (ptroff_t)array_getvalue(pool->uints, t));
    }
    swf_SetU30(tag, pool->floats->num>1?pool->floats->num:0);
    for(t=1;t<pool->floats->num;t++) {
        array_getvalue(pool->floats, t);
        swf_SetD64(tag, 0.0); // fixme
    }
    swf_SetU30(tag, pool->strings->num>1?pool->strings->num:0);
    for(t=1;t<pool->strings->num;t++) {
	swf_SetU30String(tag, array_getkey(pool->strings, t));
    }
    swf_SetU30(tag, pool->namespaces->num>1?pool->namespaces->num:0);
    for(t=1;t<pool->namespaces->num;t++) {
	namespace_t*ns= (namespace_t*)array_getvalue(pool->namespaces, t);
	const char*name = ns->name;
	int i = pool_find_string(pool, name);
	swf_SetU8(tag, ns->access);
	swf_SetU30(tag, i);
    }
    swf_SetU30(tag, pool->namespace_sets->num>1?pool->namespace_sets->num:0);
    for(t=1;t<pool->namespace_sets->num;t++) {
        namespace_set_t*set = (namespace_set_t*)array_getvalue(pool->namespace_sets, t);
        namespace_list_t*i = set->namespaces; 
        int len = list_length(i);
        swf_SetU30(tag, len);
        while(i) {
            int index = pool_find_namespace(pool, i->namespace);
            swf_SetU30(tag, index);
            i = i->next;
        }
    }

    swf_SetU30(tag, pool->multinames->num>1?pool->multinames->num:0);
    for(t=1;t<pool->multinames->num;t++) {
	multiname_t*m = (multiname_t*)array_getvalue(pool->multinames, t);
	swf_SetU8(tag, m->type);

        if(m->ns) {
            assert(m->type==0x07 || m->type==0x0d);
	    int i = pool_find_namespace(pool, m->ns);
            if(i<0) fprintf(stderr, "internal error: unregistered namespace %02x %s %s\n", m->ns->access, access2str(m->ns->access), m->ns->name);
	    swf_SetU30(tag, i);
        } else {
            assert(m->type!=0x07 && m->type!=0x0d);
        }
        if(m->name) {
            assert(m->type==0x09 || m->type==0x0e || m->type==0x07 || m->type==0x0d || m->type==0x0f || m->type==0x10);
	    int i = pool_find_string(pool, m->name);
            if(i<0) fprintf(stderr, "internal error: unregistered name\n");
	    swf_SetU30(tag, i);
        } else {
            assert(m->type!=0x09 && m->type!=0x0e && m->type!=0x07 && m->type!=0x0d && m->type!=0x0f && m->type!=0x10);
        }
        if(m->namespace_set) {
            assert(m->type==0x09 || m->type==0x0e || m->type==0x1c || m->type==0x1b);
	    int i = pool_find_namespace_set(pool, m->namespace_set);
            if(i<0) fprintf(stderr, "internal error: unregistered namespace set\n");
	    swf_SetU30(tag, i);
        } else {
            assert(m->type!=0x09 && m->type!=0x0e && m->type!=0x1c && m->type!=0x1b);
        }

    }

    swf_SetBlock(tag, tmp->data, tmp->len);

    swf_DeleteTag(0, tmp);
    pool_destroy(pool);
}

void swf_FreeABC(void*code)
{
    abc_file_t*file= (abc_file_t*)code;

    int t;
    for(t=0;t<file->metadata->num;t++) {
        array_t*items = (array_t*)array_getvalue(file->metadata, t);
        array_free(items);
    }
    array_free(file->metadata);

    for(t=0;t<file->methods->num;t++) {
        abc_method_t*m = (abc_method_t*)array_getvalue(file->methods, t);
        list_free(m->parameters);
        free(m);
    }
    array_free(file->methods);

    for(t=0;t<file->classes->num;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
        list_free(cls->interfaces);
        traits_free(cls->traits);
	traits_free(cls->static_constructor_traits);
        free(cls);
    }
    array_free(file->classes);

    for(t=0;t<file->scripts->num;t++) {
        abc_script_t*s = (abc_script_t*)array_getvalue(file->scripts, t);
        traits_free(s->traits);
        free(s);
    }
    array_free(file->scripts);

    for(t=0;t<file->method_bodies->num;t++) {
        abc_method_body_t*body = (abc_method_body_t*)array_getvalue(file->method_bodies, t);
        code_free(body->code);
	traits_free(body->traits);
        free(body);
    }
    array_free(file->method_bodies);

    free(file);
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

#include "abc_ops.c"

void swf_AddButtonLinks(SWF*swf, char stop_each_frame, char events)
{
    int num_frames = 0;
    int has_buttons = 0;
    TAG*tag=swf->firstTag;
    while(tag) {
        if(tag->id == ST_SHOWFRAME)
            num_frames++;
        if(tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2)
            has_buttons = 1;
        tag = tag->next;
    }

    abc_file_t*file = abc_file_new();
    abc_method_body_t*c = 0;
   
    abc_class_t*cls = abc_class_new(file, "rfx::MainTimeline", "flash.display::MovieClip");
    abc_class_protectedNS(cls, "rfx:MainTimeline");
  
    TAG*abctag = swf_InsertTagBefore(swf, swf->firstTag, ST_DOABC);
    
    tag = swf_InsertTag(abctag, ST_SYMBOLCLASS);
    swf_SetU16(tag, 1);
    swf_SetU16(tag, 0);
    swf_SetString(tag, "rfx.MainTimeline");

    c = abc_class_staticconstructor(cls, 0, 0);
    c->max_stack = 1;
    c->local_count = 1;
    c->init_scope_depth = 9;
    c->max_scope_depth = 10;

    __ getlocal_0(c);
    __ pushscope(c);
    __ returnvoid(c);

    c = abc_class_constructor(cls, 0, 0);
    c->max_stack = 3;
    c->local_count = 1;
    c->init_scope_depth = 10;
    c->max_scope_depth = 11;
    
    debugfile(c, "constructor.as");

    __ getlocal_0(c);
    __ pushscope(c);

    __ getlocal_0(c);
    __ constructsuper(c,0);

    __ getlex(c, "[package]flash.system::Security");
    __ pushstring(c, "*");
    __ callpropvoid(c, "[package]::allowDomain", 1);
    
    if(stop_each_frame || has_buttons) {
        int frame = 0;
        tag = swf->firstTag;
        abc_method_body_t*f = 0; //frame script
        while(tag && tag->id!=ST_END) {
            char framename[80];
            char needs_framescript=0;
            char buttonname[80];
            char functionname[80];
            sprintf(framename, "[packageinternal]rfx::frame%d", frame);
            
            if(!f && (tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2 || stop_each_frame)) {
                /* make the contructor add a frame script */
                __ findpropstrict(c,"[package]::addFrameScript");
                __ pushbyte(c,frame);
                __ getlex(c,framename);
                __ callpropvoid(c,"[package]::addFrameScript",2);

                f = abc_class_method(cls, 0, framename, 0);
                f->max_stack = 3;
                f->local_count = 1;
                f->init_scope_depth = 10;
                f->max_scope_depth = 11;
                __ debugfile(f, "framescript.as");
                __ debugline(f, 1);
                __ getlocal_0(f);
                __ pushscope(f);
            }

            if(tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2) {
                U16 id = swf_GetDefineID(tag);
                sprintf(buttonname, "::button%d", swf_GetDefineID(tag));
                __ getlex(f,buttonname);
                __ getlex(f,"flash.events::MouseEvent");
                __ getproperty(f, "::CLICK");
                sprintf(functionname, "::clickbutton%d", swf_GetDefineID(tag));
                __ getlex(f,functionname);
                __ callpropvoid(f, "::addEventListener" ,2);

                if(stop_each_frame) {
                    __ findpropstrict(f, "[package]::stop");
                    __ callpropvoid(f, "[package]::stop", 0);
                }
                needs_framescript = 1;

                abc_method_body_t*h =
                    abc_class_method(cls, "::void", functionname, 1, "flash.events::MouseEvent");
                h->max_stack = 6;
                h->local_count = 2;
                h->init_scope_depth = 10;
                h->max_scope_depth = 11;
                __ getlocal_0(h);
                __ pushscope(h);

                ActionTAG*oldaction = swf_ButtonGetAction(tag);
                if(oldaction && oldaction->op == ACTION__GOTOFRAME) {
                    int framenr = GET16(oldaction->data);
                    if(framenr>254) {
                        fprintf(stderr, "Warning: Couldn't translate jump to frame %d to flash 9 actionscript\n", framenr);
                    }
                    if(!events) {
                        __ findpropstrict(h,"[package]::gotoAndStop");
                        __ pushbyte(h,framenr+1);
                        __ callpropvoid(h,"[package]::gotoAndStop", 1);
                    } else {
                        char framename[80];
                        sprintf(framename, "frame%d", framenr);
                        __ getlocal_0(h); //this
                        __ findpropstrict(h, "[package]flash.events::TextEvent");
                        __ pushstring(h, "link");
                        __ pushtrue(h);
                        __ pushtrue(h);
                        __ pushstring(h, framename);
                        __ constructprop(h,"[package]flash.events::TextEvent", 4);
                        __ callpropvoid(h,"[package]::dispatchEvent", 1);
                    }
                } else if(oldaction && oldaction->op == ACTION__GETURL) {
                    if(!events) {
                        __ findpropstrict(h,"flash.net::navigateToURL");
                        __ findpropstrict(h,"flash.net::URLRequest");
                        // TODO: target _blank
                        __ pushstring(h,oldaction->data); //url
                        __ constructprop(h,"flash.net::URLRequest", 1);
                        __ callpropvoid(h,"flash.net::navigateToURL", 1);
                    } else {
                        __ getlocal_0(h); //this
                        __ findpropstrict(h, "[package]flash.events::TextEvent");
                        __ pushstring(h, "link");
                        __ pushtrue(h);
                        __ pushtrue(h);
                        __ pushstring(h,oldaction->data); //url
                        __ constructprop(h,"[package]flash.events::TextEvent", 4);
                        __ callpropvoid(h,"[package]::dispatchEvent", 1);
                    }
                } else if(oldaction) {
                    fprintf(stderr, "Warning: Couldn't translate button code of button %d to flash 9 abc action\n", id);
                }
                __ returnvoid(h);
                swf_ActionFree(oldaction);
            }
            if(tag->id == ST_SHOWFRAME) {
                if(f) {
                    __ returnvoid(f);
                    f = 0;
                }
                frame++;
            }
            tag = tag->next;
        }
        if(f) {
            __ returnvoid(f);
        }
    }
    __ returnvoid(c);

    tag = swf->firstTag;
    while(tag) {
        if(tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2) {
            char buttonname[80];
            sprintf(buttonname, "::button%d", swf_GetDefineID(tag));
            abc_AddSlot(cls, buttonname, 0, "flash.display::SimpleButton");
        }
        tag = tag->next;
    }


    abc_script_t*s = abc_initscript(file, 0, 0);
    c = s->method->body;
    c->max_stack = 2;
    c->local_count = 1;
    c->init_scope_depth = 1;
    c->max_scope_depth = 9;

    __ getlocal_0(c);
    __ pushscope(c);
    __ getscopeobject(c, 0);
    __ getlex(c,"::Object");
    __ pushscope(c);
    __ getlex(c,"flash.events::EventDispatcher");
    __ pushscope(c);
    __ getlex(c,"flash.display::DisplayObject");
    __ pushscope(c);
    __ getlex(c,"flash.display::InteractiveObject");
    __ pushscope(c);
    __ getlex(c,"flash.display::DisplayObjectContainer");
    __ pushscope(c);
    __ getlex(c,"flash.display::Sprite");
    __ pushscope(c);
    __ getlex(c,"flash.display::MovieClip");
    __ pushscope(c);
    __ getlex(c,"flash.display::MovieClip");
    __ newclass(c,cls);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ initproperty(c,"rfx::MainTimeline");
    __ returnvoid(c);

    //abc_method_body_addClassTrait(c, "rfx:MainTimeline", 1, cls);
    abc_initscript_addClassTrait(s, "rfx::MainTimeline", 1, cls);

    swf_WriteABC(abctag, file);
}

