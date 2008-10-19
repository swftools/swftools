/* swfabc.c

   Routines for handling Flash2 AVM2 ABC Actionscript

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2007,2008 Matthias Kramm <kramm@quiss.org>
 
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
#include "swfabc.h"

char stringbuffer[2048];

dict_t* dict_new() {
    dict_t*d = malloc(sizeof(dict_t));
    memset(d, 0, sizeof(dict_t));
    return d;
}

void dict_free(dict_t*dict) {
    if(dict->d)
    free(dict->d);dict->d = 0;
    free(dict);
}

const char*dict_getstr(dict_t*dict, int nr) {
    if(nr > dict->num || nr<0) {
	printf("error: reference to string %d in dict\n");
	return 0;
    }
    return dict->d[nr].name;
}
char*dict_getdata(dict_t*dict, int nr) {
    if(nr > dict->num || nr<0) {
	printf("error: reference to string %d in dict\n");
	return 0;
    }
    return dict->d[nr].data;
}
int dict_append(dict_t*dict, const char*name, void*data) {
    while(dict->size <= dict->num) {
	dict->size += 64;
	if(!dict->d) {
	    dict->d = malloc(sizeof(dict_entry_t)*dict->size);
	} else {
	    dict->d = realloc(dict->d, sizeof(dict_entry_t)*dict->size);
	}
    }
    if(name) {
	dict->d[dict->num].name = strdup(name);
    } else {
	dict->d[dict->num].name = 0;
    }
    dict->d[dict->num].data = data;
    return dict->num++;
}
int dict_find(dict_t*dict, const char*name)
{
    if(!name)
	name = "";
    int t;
    for(t=0;t<dict->num;t++) {
	if(dict->d[t].name && !strcmp(dict->d[t].name,name))
	    return t;
    }
    return -1;
}
int dict_find2(dict_t*dict, const char*name, void*data)
{
    if(!name)
	name = "";
    int t;
    for(t=0;t<dict->num;t++) {
	if(dict->d[t].name && !strcmp(dict->d[t].name,name) && dict->d[t].data == data)
	    return t;
    }
    return -1;
}
int dict_update(dict_t*dict, const char*name, void*data) {
    int pos = dict_find(dict, name);
    if(pos>=0) {
	dict->d[pos].data = data;
	return pos;
    }
    return dict_append(dict, name, data);
}
int dict_append_if_new(dict_t*dict, const char*name, void*data) {
    int pos = dict_find(dict, name);
    if(pos>=0)
	return pos;
    return dict_append(dict, name, data);
}
int dict_append_if_new2(dict_t*dict, const char*name, void*data) {
    int pos = dict_find2(dict, name, data);
    if(pos>=0)
	return pos;
    return dict_append(dict, name, data);
}

typedef struct _commonlist {
    void*entry;
    struct _commonlist*next;
    struct _commonlist*last[0];
} commonlist_t;

int list_length(void*_list)
{
    commonlist_t*l = (commonlist_t*)_list;
    int n=0;
    while(l) {
        l = l->next;
        n++;
    }
    return n;
}
void list_append(void*_list, void*entry)
{
    commonlist_t**list = (commonlist_t**)_list;
    commonlist_t* n = 0;
    if(!*list) {
        n = malloc(sizeof(commonlist_t)+sizeof(commonlist_t*));
        *list = n;
    } else {
        n = malloc(sizeof(commonlist_t));
        (*list)->last[0]->next = n;
    }
    n->next = 0;
    n->entry = entry;
    (*list)->last[0] = n;
}

int swf_GetU30(TAG*tag)
{
    U32 shift = 0;
    U32 s = 0;
    while(1) {
	U8 b = swf_GetU8(tag);
	s|=(b&127)<<shift;
	shift+=7;
	if(!(b&128))
	    break;
    }
    return s;
}

int swf_GetS30(TAG*tag)
{
    U32 shift = 0;
    U32 s = 0;
    while(1) {
	U8 b = swf_GetU8(tag);
	s|=(b&127)<<shift;
	shift+=7;
	if(!(b&128)) {
            if(b&64) {
                s|=0xffffffff<<shift;
            }
	    break;
        }
    }
    return s;
}

double swf_GetD64(TAG*tag)
{
    double value = *(double*)&tag->data[tag->pos];
    swf_GetU32(tag);
    swf_GetU32(tag);
    return value;
}


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
{0x80, "coerce", "m"},
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

int swf_GetU24(TAG*tag)
{
    int b1 = swf_GetU8(tag);
    int b2 = swf_GetU8(tag);
    int b3 = swf_GetU8(tag);
    return b3<<16|b2<<8|b1;
}
int swf_GetS24(TAG*tag)
{
    int b1 = swf_GetU8(tag);
    int b2 = swf_GetU8(tag);
    int b3 = swf_GetU8(tag);
    if(b3&0x80) {
        return -1-((b3<<16|b2<<8|b1)^0xffffff);
    } else {
        return b3<<16|b2<<8|b1;
    }
}
static int parse_code(TAG*tag, int len, abc_file_t*pool, char*prefix)
{
    int end=tag->pos+len;
    while(tag->pos<end) {
	U8 opcode = swf_GetU8(tag);
	int t;
	char found = 0;
	for(t=0;t<sizeof(opcodes)/sizeof(opcodes[0]);t++) {
	    if(opcodes[t].opcode == opcode) {
		printf("%s%s ", prefix, opcodes[t].name);
		char*p = opcodes[t].params;
		char first = 1;
		while(*p) {
		    if(!first)
			printf(", ");
		    if(*p == 'n') {
			int n = swf_GetU30(tag);
			printf("%d params", n);
		    } else if(*p == '2') {
			const char* m = dict_getstr(pool->multinames, swf_GetU30(tag));
			printf("%s", m);
		    } else if(*p == 'm') {
			int n = swf_GetU30(tag);
			printf("[method%d]", n);
		    } else if(*p == 'c') {
			int n = swf_GetU30(tag);
                        abc_class_t*cls = (abc_class_t*)dict_getdata(pool->classes, n);
			printf("[classinfo%d %s]", n, cls->classname);
		    } else if(*p == 'i') {
			int n = swf_GetU30(tag);
			printf("[methodbody%d]", n);
		    } else if(*p == 'u') {
			int n = swf_GetU30(tag);
			printf("%d", n);
		    } else if(*p == 'b') {
			int b = swf_GetU8(tag);
			printf("%02x", b);
		    } else if(*p == 'j') {
			printf("%d", swf_GetS24(tag));
		    } else if(*p == 's') {
			const char*s = dict_getstr(pool->strings, swf_GetU30(tag));
			printf("\"%s\"", s);
		    } else if(*p == 'S') {
                        swf_GetU24(tag); //default
			int num = swf_GetU30(tag)+1;
                        int t;
                        for(t=0;t<num;t++) 
                            swf_GetU24(tag);
		    } else {
                        printf("Can't parse opcode param type \"%c\"\n", *p);
                        return 0;
                    }
		    p++;
		    first = 0;
		}
		found = 1;
		break;
	    }
	}
	if(!found) {
	    printf("Can't parse opcode %02x\n", opcode);
	    return 0;
	}
	printf("\n");
    }
    if(tag->pos!=end) {
	printf("Read beyond end of ABC Bytecode\n");
	return 0;
    }
    return 1;
}

static char* access2str(int type)
{
    if(type==0x08) return "";
    else if(type==0x16) return "package";
    else if(type==0x17) return "packageinternal";
    else if(type==0x18) return "protected";
    else if(type==0x19) return "explicit";
    else if(type==0x1A) return "staticprotected";
    else if(type==0x05) return "private";
    else return "undefined";
}


char* multiname_to_string(abc_multiname_t*m)
{
    char*mname = 0;
    if(m->type==0x07 || m->type==0x0d) {
        mname = malloc(strlen(m->ns->name)+strlen(m->name)+32);
        sprintf(mname, "<%s>\0", access2str(m->ns->access));
        strcat(mname, m->ns->name);
        strcat(mname, "::");
        strcat(mname, m->name);
    } else if(m->type==0x0f || m->type==0x10) {
        mname = strdup(m->name);
    } else if(m->type==0x11 || m->type==0x12) {
        mname = strdup("");
    } else if(m->type==0x09 || m->type==0x0e) {
        mname = malloc(strlen(m->namespace_set_name)+strlen(m->name)+16);
        strcpy(mname, m->namespace_set_name);
        strcat(mname, "::");
        strcat(mname, m->name);
    } else if(m->type==0x1b || m->type==0x1c) {
        mname = strdup(m->namespace_set_name);
    }
    return mname;
}


static void dump_traits(const char*prefix, dict_t*traits, abc_file_t*pool);

static char* params_to_string(abc_multiname_list_t*list)
{
    abc_multiname_list_t*l;
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
        names[n] = multiname_to_string(list->abc_multiname);
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

static void dump_method(const char*prefix, const char*type, const char*name, int nr, abc_file_t*pool)
{
    if(nr >= pool->methods->num) {
	printf("Invalid method number: %d\n", nr);
	return;
    }
    abc_method_t*m = (abc_method_t*)dict_getdata(pool->methods, nr);

    const char*return_type = "";
    if(m->return_type)
        return_type = multiname_to_string(m->return_type);

    char*paramstr = params_to_string(m->parameters);

    printf("%s%s %s %s=%s %s\n", prefix, type, return_type, name, m->name, paramstr);

    abc_method_body_t*c = (abc_method_body_t*)dict_getdata(pool->method_bodies, m->method_body_index);
    
    printf("%s[%d %d %d %d %d]\n", prefix, c->max_stack, c->local_count, c->init_scope_depth, c->max_scope_depth, c->exception_count);

    swf_SetTagPos(c->tag, 0);
    char prefix2[80];
    sprintf(prefix2, "%s    ", prefix);
    if(c->traits)
        dump_traits(prefix, c->traits, pool);
    printf("%s{\n", prefix);
    parse_code(c->tag, c->tag->len, pool,prefix2);
    printf("%s}\n\n", prefix);
}

//#define DEBUG
#define DEBUG if(0)

static void parse_metadata(TAG*tag, abc_file_t*pool)
{
    int t;
    int num_metadata = swf_GetU30(tag);
    DEBUG printf("%d metadata\n");
    for(t=0;t<num_metadata;t++) {
        const char*name = dict_getstr(pool->strings, swf_GetU30(tag));
        int num = swf_GetU30(tag);
        int s;
        DEBUG printf("  %s\n", name);
        for(s=0;s<num;s++) {
            const char*key = dict_getstr(pool->strings, swf_GetU30(tag));
            const char*value = dict_getstr(pool->strings, swf_GetU30(tag));
            DEBUG printf("    %s=%s\n", key, value);
        }
    }
}

#define TRAIT_SLOT 0
#define TRAIT_METHOD 1
#define TRAIT_GETTER 2
#define TRAIT_SETTER 3
#define TRAIT_CLASS 4
#define TRAIT_FUNCTION 5
#define TRAIT_CONST 6

static dict_t* traits_parse(TAG*tag, abc_file_t*pool)
{
    int num_traits = swf_GetU30(tag);
    dict_t*traits = dict_new();
    int t;
    if(num_traits) {
        DEBUG printf("%d traits\n", num_traits);
    }
    
    for(t=0;t<num_traits;t++) {
	abc_trait_t*trait = malloc(sizeof(abc_trait_t));
	memset(trait, 0, sizeof(abc_trait_t));
	dict_append(traits, 0, trait);
	trait->name_index = swf_GetU30(tag);
	const char*name = dict_getstr(pool->multinames, trait->name_index);
	U8 kind = trait->type = swf_GetU8(tag);
        U8 attributes = kind&0xf0;
        kind&=0x0f;
	DEBUG printf("  trait %d) %s type=%02x\n", t, name, kind);
	if(kind == 1 || kind == 2 || kind == 3) { // method / getter / setter
	    trait->disp_id = swf_GetU30(tag);
	    trait->nr = swf_GetU30(tag);
	    DEBUG printf("  method/getter/setter\n");
	} else if(kind == 5) { // function
	    trait->slot_id =  swf_GetU30(tag);
	    trait->nr = swf_GetU30(tag);
	} else if(kind == 4) { // class
	    trait->slot_id = swf_GetU30(tag);
	    trait->cls = swf_GetU30(tag);
	    DEBUG printf("  class %s %d %d\n", name, trait->slot_id, trait->cls);
	} else if(kind == 0 || kind == 6) { // slot, const
            /* a slot is a variable in a class that is shared amonst all instances
               of the same type, but which has a unique location in each object 
               (in other words, slots are non-static, traits are static)
             */
	    trait->slot_id = swf_GetU30(tag);
	    const char*type_name = dict_getstr(pool->multinames, swf_GetU30(tag));
	    trait->vindex = swf_GetU30(tag);
	    if(trait->vindex) {
		trait->vkind = swf_GetU8(tag);
	    }
	    DEBUG printf("  slot %s %d %s (vindex=%d)\n", name, trait->slot_id, type_name, trait->vindex);
	} else {
	    printf("    can't parse trait type %d\n", kind);
	    return 0;
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

static void dump_traits(const char*prefix, dict_t*traits, abc_file_t*pool)
{
    int num_traits = traits->num;
    int t;
    for(t=0;t<num_traits;t++) {
	abc_trait_t*trait = (abc_trait_t*)dict_getdata(traits, t);
	const char*name = dict_getstr(pool->multinames, trait->name_index);
	U8 kind = trait->type;
        U8 attributes = kind&0xf0;
        kind&=0x0f;
	if(kind == TRAIT_METHOD) {
	    dump_method(prefix, "method", name, trait->nr, pool);
	} else if(kind == TRAIT_GETTER) {
	    dump_method(prefix, "getter", name, trait->nr, pool);
        } else if(kind == TRAIT_SETTER) {
	    dump_method(prefix, "setter", name, trait->nr, pool);
	} else if(kind == TRAIT_FUNCTION) { // function
	    dump_method(prefix, "function", name, trait->nr, pool);
	} else if(kind == TRAIT_CLASS) { // class
            abc_class_t*cls = (abc_class_t*)dict_getdata(pool->classes, trait->cls);
            if(!cls) {
	        printf("%sslot %d: class %s=class%d %d\n", prefix, trait->slot_id, name, trait->cls);
            } else {
	        printf("%sslot %d: class %s=%s\n", prefix, trait->slot_id, name, cls->classname);
            }
	} else if(kind == TRAIT_SLOT || kind == TRAIT_CONST) { // slot, const
	    int slot_id = trait->slot_id;
	    const char*type_name = dict_getstr(pool->multinames, trait->type_index);
	    printf("%sslot %s %d %s (vindex=%d)\n", prefix, name, trait->slot_id, type_name, trait->vindex);
	} else {
	    printf("    can't dump trait type %d\n", kind);
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

    f->ints = dict_new();
    dict_append(f->ints, 0, (void*)(ptroff_t)0);
    f->uints = dict_new();
    dict_append(f->uints, 0, (void*)(ptroff_t)0);
    f->floats = dict_new();
    dict_append(f->floats, 0, 0);
    f->strings = dict_new();
    dict_append(f->strings, "--<UNDEFINED_STRING>--", 0);
    f->namespaces = dict_new();
    dict_append(f->namespaces, "--<UNDEFINED_NAMESPACE>--", 0);
    f->namespace_sets = dict_new();
    dict_append(f->namespace_sets, "--<UNDEFINED_NSSET>--", 0);
    f->sets = dict_new();
    dict_append(f->sets, "--<UNDEFINED_SET>--", 0);
    f->multinames = dict_new();
    dict_append(f->multinames, "--<UNDEFINED_MULTINAME>--", 0);

    // abc_file

    f->methods = dict_new();
    f->classes = dict_new();
    f->scripts = dict_new();
    f->method_bodies = dict_new();

    return f;
}

static abc_namespace_t* namespace_new(U8 access, const char*name)
{
    abc_namespace_t*ns = malloc(sizeof(abc_namespace_t));
    memset(ns, 0, sizeof(abc_namespace_t));

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
abc_namespace_t* abc_namespace(abc_file_t*file, const char*name) {
    return namespace_new(0x08, name);
}
abc_namespace_t* abc_packagenamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x16 , name);
}
abc_namespace_t* abc_packageinternalnamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x17, name);
}
abc_namespace_t* abc_protectednamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x18, name);
}
abc_namespace_t* abc_explicitnamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x19, name);
}
abc_namespace_t* abc_staticprotectednamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x1a, name);
}
abc_namespace_t* abc_privatenamespace(abc_file_t*file, const char*name) {
    return namespace_new(0x05, name);
}

static int multiname_index(abc_file_t*pool, const char*name2) 
{
    if(!name2)
	name2 = "::";
    int pos = dict_find(pool->multinames, name2);
    if(pos>=0)
	return pos;

    char*n = strdup(name2);
    char*p = strstr(n, "::");
    char*namespace=0,*name=0;
    if(!p) {
        if(strchr(n, ':')) {
            fprintf(stderr, "Error: single ':' in name\n");
        }
	namespace = "";
	name = n;
    } else {
	*p = 0;
	namespace = n;
	name = p+2;
        if(strchr(namespace, ':')) {
            fprintf(stderr, "Error: single ':' in namespace\n");
        }
        if(strchr(name, ':')) {
            fprintf(stderr, "Error: single ':' in qualified name\n");
        }
    }

    abc_multiname_t*m = malloc(sizeof(abc_multiname_t));
    memset(m, 0, sizeof(abc_multiname_t));

    m->type = QNAME;
    m->namespace_set_name = 0;
    m->ns = namespace_new(0, namespace);
    m->name = name;
    return dict_append(pool->multinames, name2, m);
}
static abc_multiname_t* multiname_fromstring(abc_file_t*pool, const char*name)
{
    int i = multiname_index(pool, name);
    return (abc_multiname_t*)dict_getdata(pool->multinames, i);
}


#define CLASS_SEALED 1
#define CLASS_FINAL 2
#define CLASS_INTERFACE 4
#define CLASS_PROTECTED_NS 8

abc_class_t* abc_class_new(abc_file_t*pool, char*classname, char*superclass) {
    abc_class_t* c = malloc(sizeof(abc_class_t));
    memset(c, 0, sizeof(abc_class_t));
    c->index = dict_append(pool->classes, 0, c);
    c->pool = pool;
    c->classname = strdup(classname);
    c->superclass = superclass?strdup(superclass):0;
    c->flags = 0;
    c->iinit = -1;
    c->static_constructor_index = -1;
    c->traits = dict_new();
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
    c->index = dict_append(pool->method_bodies, 0, c);
    c->tag = swf_InsertTag(0,0);
    c->pool = pool;
    c->traits = dict_new();

    /* construct method object */
    abc_method_t* m = malloc(sizeof(abc_method_t));
    memset(m, 0, sizeof(abc_method_t));
    m->index = dict_append(pool->methods, 0, m);
    if(returntype && strcmp(returntype, "void")) {
	m->return_type = multiname_fromstring(pool, returntype);
    } else {
	m->return_type = 0;
    }
    int t;
    for(t=0;t<num_params;t++) {
	const char*param = va_arg(va, const char*);
	list_append(&m->parameters, multiname_fromstring(pool, param));
    }

    /* crosslink the two objects */
    m->method_body_index = c->index;
    c->method = m;

    return c;
}

abc_method_body_t* abc_class_constructor(abc_class_t*cls, char*returntype, int num_params, ...) 
{
    va_list va;
    va_start(va, num_params);
    abc_method_body_t* c = add_method(cls->pool, cls, returntype, num_params, va);
    va_end(va);
    cls->iinit = c->index;
    return c;
}

abc_method_body_t* abc_class_staticconstructor(abc_class_t*cls, char*returntype, int num_params, ...) 
{
    va_list va;
    va_start(va, num_params);
    abc_method_body_t* c = add_method(cls->pool, cls, returntype, num_params, va);
    va_end(va);
    cls->static_constructor_index = c->index;
    return c;
}

abc_trait_t*trait_new(int type, int name_index, int data1, int data2, int vindex, int vkind)
{
    abc_trait_t*trait = malloc(sizeof(abc_trait_t));
    memset(trait, 0, sizeof(abc_trait_t));
    trait->type = type;
    trait->name_index = name_index;
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
    dict_append(cls->traits, 0, trait_new(TRAIT_METHOD, multiname_index(pool, name), 0, c->method->index, 0, 0));
    return c;
}

void abc_AddSlot(abc_class_t*cls, char*name, int slot, char*multiname)
{
    abc_file_t*pool = cls->pool;
    int i = multiname_index(pool, name);
    dict_append(cls->traits, 0, trait_new(TRAIT_SLOT, i, slot, multiname_index(pool, multiname), 0, 0));
}

void abc_method_body_addClassTrait(abc_method_body_t*code, char*multiname, int slotid, abc_class_t*cls)
{
    abc_file_t*pool = code->pool;
    int i = multiname_index(pool, multiname);
    abc_trait_t*trait = trait_new(TRAIT_CLASS, i, slotid, cls->index, 0, 0);
    dict_append(code->traits, 0, trait);
}

/* notice: traits of a method (body) belonging to an init script
   and traits of the init script are *not* the same thing */
void abc_initscript_addClassTrait(abc_script_t*script, char*multiname, int slotid, abc_class_t*cls)
{
    abc_file_t*pool = script->pool;
    int i = multiname_index(pool, multiname);
    abc_trait_t*trait = trait_new(TRAIT_CLASS, i, slotid, cls->index, 0, 0);
    dict_append(script->traits, 0, trait);
}

abc_script_t* abc_initscript(abc_file_t*pool, char*returntype, int num_params, ...) 
{
    va_list va;
    va_start(va, num_params);
    abc_method_body_t* c = add_method(pool, 0, returntype, num_params, va);
    abc_script_t* s = malloc(sizeof(abc_script_t));
    s->method = c->method;
    s->traits = dict_new();
    s->pool = pool;
    dict_append(pool->scripts, 0, s);
    va_end(va);
    return s;
}

void swf_SetU30(TAG*tag, U32 u)
{
    do {
	swf_SetU8(tag, (u&~0x7f?0x80:0) | (u&0x7F));
	u>>=7;
    } while(u);
}
void swf_SetU30String(TAG*tag, const char*str)
{
    int l = strlen(str);
    swf_SetU30(tag, l);
    swf_SetBlock(tag, (void*)str, l);
}

static void write_traits(abc_file_t*pool, TAG*tag, dict_t*traits)
{
    if(!traits) {
        swf_SetU30(tag, 0);
        return;
    }
    swf_SetU30(tag, traits->num);
    int s;

    for(s=0;s<traits->num;s++) {
	abc_trait_t*trait = (abc_trait_t*)dict_getdata(traits, s);
	swf_SetU30(tag, trait->name_index);
	swf_SetU8(tag, trait->type);
	swf_SetU30(tag, trait->data1);
	swf_SetU30(tag, trait->data2);
	if(trait->type == 0) { //slot
	    swf_SetU30(tag, trait->vindex);
	    if(trait->vindex) {
		swf_SetU8(tag, trait->vkind);
	    }
	}
    }
}

int register_multiname(abc_file_t*pool, abc_multiname_t*n)
{
    if(!n)
        return 0;
    /* FIXME: might create duplicates */
    return dict_append_if_new2(pool->multinames, n->name, n);
}

int register_namespace(abc_file_t*pool, abc_namespace_t*ns)
{
    /* FIXME: might create duplicates */
    return dict_append_if_new2(pool->namespaces, ns->name, ns);
}

static inline abc_multiname_t*multiname_lookup(abc_file_t*pool, int i)
{
    return (abc_multiname_t*)dict_getdata(pool->multinames, i);
}

void* swf_ReadABC(TAG*tag)
{
    abc_file_t* pool = abc_file_new();

    swf_SetTagPos(tag, 0);
    int t;
    if(tag->id == ST_DOABC) {
        /* the strange 72 tag from flex doesn't have abc flags */
        U32 abcflags = swf_GetU32(tag);
        DEBUG printf("flags=%08x\n", abcflags);
        char*classname = swf_GetString(tag);
    }
    U32 version = swf_GetU32(tag);
    if(version!=0x002e0010) {
        fprintf(stderr, "Warning: unknown AVM2 version %08x\n", version);
    }

    int num_ints = swf_GetU30(tag);
    DEBUG printf("%d ints\n", num_ints);
    for(t=1;t<num_ints;t++) {
        S32 v = swf_GetU30(tag);
        DEBUG printf("int %d) %d\n", t, v);
        dict_append(pool->ints, 0, (void*)(ptroff_t)v);
    }

    int num_uints = swf_GetU30(tag);
    DEBUG printf("%d uints\n", num_uints);
    for(t=1;t<num_uints;t++) {
        U32 v = swf_GetS30(tag);
        DEBUG printf("uint %d) %d\n", t, v);
        dict_append(pool->uints, 0, (void*)(ptroff_t)v);
    }
    
    int num_floats = swf_GetU30(tag);
    DEBUG printf("%d floats\n", num_floats);
    for(t=1;t<num_floats;t++) {
        double d = swf_GetD64(tag);
        DEBUG printf("float %d) %f\n", t, d);
        dict_append(pool->floats, 0, 0);
    }
    
    int num_strings = swf_GetU30(tag);
    DEBUG printf("%d strings\n", num_strings);
    for(t=1;t<num_strings;t++) {
	int len = swf_GetU30(tag);
	char*s = malloc(len+1);
	swf_GetBlock(tag, s, len);
	s[len] = 0;
	dict_append(pool->strings, s, 0);
	DEBUG printf("%d) \"%s\"\n", t, pool->strings->d[t].name);
    }
    int num_namespaces = swf_GetU30(tag);
    DEBUG printf("%d namespaces\n", num_namespaces);
    for(t=1;t<num_namespaces;t++) {
	U8 type = swf_GetU8(tag);
	int namenr = swf_GetU30(tag);
	const char*name = dict_getstr(pool->strings, namenr);
        abc_namespace_t*ns = malloc(sizeof(abc_namespace_t));
        memset(ns, 0, sizeof(abc_namespace_t));
        ns->access = type;
        ns->name = strdup(name);
	dict_append(pool->namespaces, name, ns);
	int w = 0;
	DEBUG w=1;
	if(w) {
	    if(type==0x08) printf("Namespace %s\n", name);
	    else if(type==0x16) printf("PackageNamespace %s\n", name);
	    else if(type==0x17) printf("PackageInternalNs %s\n", name);
	    else if(type==0x18) printf("ProtectedNamespace %s\n", name);
	    else if(type==0x19) printf("ExplicitNamespace %s\n", name);
	    else if(type==0x1A) printf("StaticProtectedNs %s\n", name);
	    else if(type==0x05) printf("PrivateNs %s\n", name);
	    else {
		printf("Undefined namespace type\n");
		return 0;
	    }
	}
    }
    int num_sets = swf_GetU30(tag);
    DEBUG printf("%d namespace sets\n", num_sets);
    for(t=1;t<num_sets;t++) {
        int count = swf_GetU30(tag);
        int s;
        const char**name = malloc(sizeof(const char*)*count);
        int l = 0;
        for(s=0;s<count;s++) {
            int nsnr = swf_GetU30(tag);
            name[s] = dict_getstr(pool->namespaces, nsnr);
            l += strlen(name[s])+1;
        }
        char*desc = malloc(l+16);
        strcpy(desc, "{");
        for(s=0;s<count;s++) {
            strcat(desc, name[s]);
            strcat(desc, ",");
        }
        strcat(desc, "}");
        dict_append(pool->namespace_sets, desc, 0);
        DEBUG printf("set %d) %s\n", t, desc);
    }

    int num_multinames = swf_GetU30(tag);
    DEBUG printf("%d multinames\n", num_multinames);
    for(t=1;t<num_multinames;t++) {
        abc_multiname_t*m = malloc(sizeof(abc_multiname_t));
        memset(m, 0, sizeof(abc_multiname_t));

	m->type = swf_GetU8(tag);

	if(m->type==0x07 || m->type==0x0d) {
	    int nr1 = swf_GetU30(tag);
            m->ns = (abc_namespace_t*)dict_getdata(pool->namespaces, nr1);
            int name_index = swf_GetU30(tag);
	    m->name = dict_getstr(pool->strings, name_index);
	} else if(m->type==0x0f || m->type==0x10) {
            int name_index = swf_GetU30(tag);
	    m->name = dict_getstr(pool->strings, name_index);
	} else if(m->type==0x11 || m->type==0x12) {
	} else if(m->type==0x09 || m->type==0x0e) {
            int name_index = swf_GetU30(tag);
            int namespace_set_index = swf_GetU30(tag);
	    m->name = dict_getstr(pool->strings, name_index);
	    m->namespace_set_name = dict_getstr(pool->namespace_sets, namespace_set_index);
        } else if(m->type==0x1b || m->type==0x1c) {
            int namespace_set_index = swf_GetU30(tag);
	    m->namespace_set_name = dict_getstr(pool->namespace_sets, namespace_set_index);
	} else {
	    printf("can't parse type %d multinames yet\n", m->type);
	    return 0;
	}
	char*mname = multiname_to_string(m);
        DEBUG printf("multiname %d) %s\n", t, mname);
	dict_append(pool->multinames, mname, m);
	free(mname);
    }
    
    int num_methods = swf_GetU30(tag);
    DEBUG printf("%d methods\n", num_methods);
    for(t=0;t<num_methods;t++) {
	abc_method_t*m = malloc(sizeof(abc_method_t));
	memset(m, 0, sizeof(*m));
	int param_count = swf_GetU30(tag);
	int return_type_index = swf_GetU30(tag);
        m->return_type = multiname_lookup(pool, return_type_index);
	m->index = t;

	int s;
	for(s=0;s<param_count;s++) {
	    int type_index = swf_GetU30(tag);
            list_append(&m->parameters, multiname_lookup(pool, type_index));
        }

	int namenr = swf_GetU30(tag);
	m->name = "";
	if(namenr)
	    m->name = dict_getstr(pool->strings, namenr);

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
            abc_multiname_list_t*l = m->parameters;
            while(l) {
	        m->name = dict_getstr(pool->strings, swf_GetU30(tag));
                l = l->next;
            }
	}
	dict_append(pool->methods, m->name, m);
    }
            
    parse_metadata(tag, pool);
	
    /* skip classes, and scripts for now, and do the real parsing later */
    int num_classes = swf_GetU30(tag);
    int classes_pos = tag->pos;
    DEBUG printf("%d classes\n", num_classes);
    for(t=0;t<num_classes;t++) {
	abc_class_t*cls = malloc(sizeof(abc_class_t));
	memset(cls, 0, sizeof(abc_class_t));
	dict_append(pool->classes, 0, cls);
	
        DEBUG printf("class %d\n", t);
	swf_GetU30(tag); //classname
	swf_GetU30(tag); //supername
	cls->flags = swf_GetU8(tag);
	if(cls->flags&8) 
	    swf_GetU30(tag); //protectedNS
        int s;
	int inum = swf_GetU30(tag); //interface count
        abc_multiname_list_t*list = 0;
        for(s=0;s<inum;s++) {
            int interface_index = swf_GetU30(tag);
            const char*interface = dict_getstr(pool->multinames, interface_index);
            abc_multiname_t* m = (abc_multiname_t*)dict_getdata(pool->multinames, interface_index);
            list_append(&list, m);
            DEBUG printf("  class %d interface: %s\n", t, interface);
        }

	cls->iinit = swf_GetU30(tag);
	cls->traits = traits_parse(tag, pool);
    }
    for(t=0;t<num_classes;t++) {
        abc_class_t*cls = (abc_class_t*)dict_getdata(pool->classes, t);
	cls->static_constructor_index = swf_GetU30(tag); // cinit
	cls->static_constructor_traits = traits_parse(tag, pool);
    }
    int num_scripts = swf_GetU30(tag);
    DEBUG printf("%d scripts\n", num_scripts);
    for(t=0;t<num_scripts;t++) {
	int init = swf_GetU30(tag);
	dict_t*traits = traits_parse(tag, pool); //TODO: store
    }

    int num_method_bodies = swf_GetU30(tag);
    DEBUG printf("%d method bodies\n", num_method_bodies);
    for(t=0;t<num_method_bodies;t++) {
	int methodnr = swf_GetU30(tag);
	if(methodnr >= pool->methods->num) {
	    printf("Invalid method number: %d\n", methodnr);
	    return 0;
	}
	abc_method_t*m = (abc_method_t*)dict_getdata(pool->methods, methodnr);
	abc_method_body_t*c = malloc(sizeof(abc_method_body_t));
	memset(c, 0, sizeof(abc_method_body_t));
	c->max_stack = swf_GetU30(tag);
	c->local_count = swf_GetU30(tag);
	c->init_scope_depth = swf_GetU30(tag);
	c->max_scope_depth = swf_GetU30(tag);
	int code_length = swf_GetU30(tag);
	c->method = m;
	m->method_body_index = t;

	c->tag = swf_InsertTag(0,0);

	swf_CopyData(c->tag, tag, code_length);

	int exception_count = swf_GetU30(tag);
        int s;
        for(s=0;s<exception_count;s++) {
            swf_GetU30(tag); //from
            swf_GetU30(tag); //to
            swf_GetU30(tag); //target
            swf_GetU30(tag); //exc_type
            swf_GetU30(tag); //var_name
        }
	c->traits = traits_parse(tag, pool);
	if(!c->traits) {
	    fprintf(stderr, "Can't parse code traits\n");
	    return 0;
	}
	DEBUG printf("method_body %d) (method %d), %d bytes of code", t, methodnr, code_length);
	int r,l = code_length>32?32:code_length;
	for(r=0;r<l;r++) {
	    DEBUG printf("%02x ", c->tag->data[r]);
	}
	DEBUG printf("\n");

	dict_append(pool->method_bodies, 0, c);
    }
    if(tag->len - tag->pos) {
	fprintf(stderr, "%d unparsed bytes remaining in ABC block\n", tag->len - tag->pos);
	return 0;
    }

    swf_SetTagPos(tag, classes_pos);
    for(t=0;t<num_classes;t++) {
        abc_class_t*cls = (abc_class_t*)dict_getdata(pool->classes, t);

        int classname_index = swf_GetU30(tag);
        int superclass_index = swf_GetU30(tag);
	cls->classname = dict_getstr(pool->multinames, classname_index);
	cls->superclass = dict_getstr(pool->multinames, superclass_index);
	cls->flags = swf_GetU8(tag);
	const char*ns = "";
	if(cls->flags&8) {
            int ns_index = swf_GetU30(tag);
	    cls->protectedNS = dict_getstr(pool->namespaces, ns_index);
	}
        
        if(cls->flags&1) printf("sealed ");
        if(cls->flags&2) printf("final ");
        if(cls->flags&4) printf("interface ");
        if(cls->flags&8) {
            printf("protectedNS<%s> ", cls->protectedNS);
        }

	printf("class %s", cls->classname);
        if(cls->superclass && cls->superclass[0]) {
            printf(" extends %s", cls->superclass);
            abc_multiname_list_t*ilist = cls->interfaces;
            if(ilist)
                printf(" implements");
            while(ilist) {
                char*s = multiname_to_string(ilist->abc_multiname);
                printf(" %d", s);
                free(s);
                ilist = ilist->next;
            }
            ilist->next;
            
        }
        if(cls->flags&0xf0) 
            printf("extra flags=%02x\n", cls->flags&0xf0);
	printf("{\n");
	
        dump_method("    ","staticconstructor", "", cls->static_constructor_index, pool);
        dump_traits("    ", cls->static_constructor_traits, pool);
	
        int num_interfaces = swf_GetU30(tag); //interface count
        int s;
        for(s=0;s<num_interfaces;s++) {
            swf_GetU30(tag); // multiname index TODO
        }
	cls->iinit = swf_GetU30(tag);
	dump_method("    ","constructor", cls->classname, cls->iinit, pool);
	cls->traits = traits_parse(tag, pool);
	if(!cls->traits) {
	    fprintf(stderr, "Can't parse class traits\n");
	    return 0;
	}
	dump_traits("    ",cls->traits, pool);
	
        printf("}\n");
    }
    for(t=0;t<num_classes;t++) {
        /* SKIP */
	swf_GetU30(tag); // cindex
	traits_parse(tag, pool); // TODO: free
    }
    int num_scripts2 = swf_GetU30(tag);
    printf("\n");
    for(t=0;t<num_scripts2;t++) {
	int init = swf_GetU30(tag);
        abc_method_t*m = (abc_method_t*)dict_getdata(pool->methods, init);
        
        abc_script_t*s = malloc(sizeof(abc_script_t));
        memset(s, 0, sizeof(abc_script_t));
        s->method = m;
        s->traits = traits_parse(tag, pool);
        dict_append(pool->scripts, 0, s);
        if(!s->traits) {
	    fprintf(stderr, "Can't parse script traits\n");
            return 0;
        }
        dump_method("","initmethod", "init", init, pool);
        dump_traits("", s->traits, pool);
    }
    return pool;
}

void swf_WriteABC(TAG*abctag, void*code)
{
    abc_file_t*pool = (abc_file_t*)code;

    TAG*tmp = swf_InsertTag(0,0);
    TAG*tag = tmp;
    int t;

    swf_SetU30(tag, pool->methods->num);
    for(t=0;t<pool->methods->num;t++) {
	abc_method_t*m = (abc_method_t*)dict_getdata(pool->methods, t);
        int n = 0;
        abc_multiname_list_t*l = m->parameters;
	swf_SetU30(tag, list_length(m->parameters));
	swf_SetU30(tag, register_multiname(pool, m->return_type));
	int s;
        while(l) {
	    swf_SetU30(tag, register_multiname(pool, l->abc_multiname));
            l = l->next;
	}
	swf_SetU30(tag, 0); // name
	swf_SetU8(tag, 0); //flags
    }
    
    swf_SetU30(tag, 0);//metadata

    swf_SetU30(tag, pool->classes->num);

    for(t=0;t<pool->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)dict_getdata(pool->classes, t);
   
        int classname_index = multiname_index(pool, c->classname);
        int superclass_index = multiname_index(pool, c->superclass);

	swf_SetU30(tag, classname_index);
	swf_SetU30(tag, superclass_index);

	swf_SetU8(tag, c->flags); // flags
        if(c->flags&0x08) {
            abc_namespace_t*ns = abc_protectednamespace(pool, c->protectedNS);
            int ns_index = register_namespace(pool, ns);
	    swf_SetU30(tag, ns_index);
        }

	swf_SetU30(tag, 0); // no interfaces
	if(c->iinit<0) {
	    fprintf(stderr, "Error: Class %s has no constructor\n", c->classname);
	    return;
	}
	swf_SetU30(tag, c->iinit);
	write_traits(pool, tag, c->traits);
    }
    for(t=0;t<pool->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)dict_getdata(pool->classes, t);
	if(c->static_constructor_index<0) {
	    fprintf(stderr, "Error: Class %s has no static constructor\n", c->classname);
	    return;
	}
	swf_SetU30(tag, c->static_constructor_index);
        write_traits(pool, tag, c->static_constructor_traits);
    }

    swf_SetU30(tag, pool->scripts->num);
    for(t=0;t<pool->scripts->num;t++) {
	abc_script_t*s = (abc_script_t*)dict_getdata(pool->scripts, t);
	swf_SetU30(tag, s->method->index); //!=t!
	write_traits(pool, tag, s->traits);
    }

    swf_SetU30(tag, pool->method_bodies->num);
    for(t=0;t<pool->method_bodies->num;t++) {
	abc_method_body_t*c = (abc_method_body_t*)dict_getdata(pool->method_bodies, t);
	abc_method_t*m = c->method;
	swf_SetU30(tag, m->index);
	swf_SetU30(tag, c->max_stack);
	swf_SetU30(tag, c->local_count);
	swf_SetU30(tag, c->init_scope_depth);
	swf_SetU30(tag, c->max_scope_depth);
	swf_SetU30(tag, c->tag->len);
	swf_SetBlock(tag, c->tag->data, c->tag->len);
	swf_SetU30(tag, c->exception_count);
        write_traits(pool, tag, c->traits);
    }

    // --- start to write real tag --
    
    tag = abctag;

    for(t=1;t<pool->multinames->num;t++) {
        abc_multiname_t*m = (abc_multiname_t*)dict_getdata(pool->multinames, t);
        if(m->ns) {
            register_namespace(pool, m->ns);
        }
        if(m->namespace_set_name) {
            // FIXME
        }
        if(m->name)
            dict_append_if_new(pool->strings, m->name, 0);
    }
    for(t=1;t<pool->namespaces->num;t++) {
        char*namespace_name = (char*)dict_getstr(pool->namespaces, t);
        dict_append_if_new(pool->strings, namespace_name, 0);
    }

    swf_SetU32(tag, 1); // flags
    swf_SetU8(tag, 0); //classname

    swf_SetU16(tag, 0x10); //version
    swf_SetU16(tag, 0x2e);

    swf_SetU30(tag, pool->ints->num>1?pool->ints->num:0);
    // ...
    swf_SetU30(tag, pool->uints->num>1?pool->uints->num:0);
    // ...
    swf_SetU30(tag, pool->floats->num>1?pool->floats->num:0);
    // ...
    swf_SetU30(tag, pool->strings->num>1?pool->strings->num:0);
    for(t=1;t<pool->strings->num;t++) {
	swf_SetU30String(tag, dict_getstr(pool->strings, t));
    }
    swf_SetU30(tag, pool->namespaces->num>1?pool->namespaces->num:0);
    for(t=1;t<pool->namespaces->num;t++) {
	abc_namespace_t*ns= (abc_namespace_t*)dict_getdata(pool->namespaces, t);
	const char*name = ns->name;
	int i = dict_find(pool->strings, name);
	if(i<0) {
	    fprintf(stderr, "Couldn't find namespace \"%s\" in constant pool\n", name);
	    return;
	}
	swf_SetU8(tag, ns->access);
	swf_SetU30(tag, i);
    }
    swf_SetU30(tag, pool->sets->num>1?pool->sets->num:0);
    // ...

    swf_SetU30(tag, pool->multinames->num>1?pool->multinames->num:0);
    // ...
    for(t=1;t<pool->multinames->num;t++) {
	abc_multiname_t*m = (abc_multiname_t*)dict_getdata(pool->multinames, t);
	swf_SetU8(tag, m->type);

        if(m->ns) {
            assert(m->type==0x07 || m->type==0x0d);
            /* fixme: might find wrong version */
	    int i = dict_find2(pool->namespaces, m->ns->name, (void*)(ptroff_t)m->ns);
            if(i<0) fprintf(stderr, "internal error: unregistered namespace %02x %s %s\n", m->ns->access, access2str(m->ns->access), m->ns->name);
	    swf_SetU30(tag, i);
        }
        if(m->namespace_set_name) {
            assert(m->type==0x09 || m->type==0x0e || m->type==0x1c || m->type==0x1b);
	    int i = dict_find(pool->namespace_sets, m->namespace_set_name);
            if(i<0) fprintf(stderr, "internal error: unregistered namespace set\n");
	    swf_SetU30(tag, i);
        }
        if(m->name) {
            assert(m->type==0x09 || m->type==0x0e || m->type==0x07 || m->type==0x0d || m->type==0x0f || m->type==0x10);
	    int i = dict_find(pool->strings, m->name);
            if(i<0) fprintf(stderr, "internal error: unregistered name\n");
	    swf_SetU30(tag, i);
        }
    }

    swf_SetBlock(tag, tmp->data, tmp->len);

    swf_DeleteTag(0, tmp);
}

#include "swfabc_ops.c"

void swf_AddButtonLinks(SWF*swf, char stop_each_frame)
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

    abc_getlocal_0(c);
    abc_pushscope(c);
    abc_returnvoid(c);

    c = abc_class_constructor(cls, 0, 0);
    c->max_stack = 3;
    c->local_count = 1;
    c->init_scope_depth = 10;
    c->max_scope_depth = 11;
    
    abc_debugfile(c, "constructor.as");

    abc_getlocal_0(c);
    abc_pushscope(c);

    abc_getlocal_0(c);
    abc_constructsuper(c,0);

    abc_getlex(c, "[package]flash.system::Security");
    abc_pushstring(c, "*");
    abc_callpropvoid(c, "[package]::allowDomain", 1);
    
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
                abc_findpropstrict(c,"[package]::addFrameScript");
                abc_pushbyte(c,frame);
                abc_getlex(c,framename);
                abc_callpropvoid(c,"[package]::addFrameScript",2);

                f = abc_class_method(cls, 0, framename, 0);
                f->max_stack = 3;
                f->local_count = 1;
                f->init_scope_depth = 10;
                f->max_scope_depth = 11;
                abc_debugfile(f, "framescript.as");
                abc_debugline(f, 1);
                abc_getlocal_0(f);
                abc_pushscope(f);
            }

            if(tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2) {
                U16 id = swf_GetDefineID(tag);
                sprintf(buttonname, "::button%d", swf_GetDefineID(tag));
                abc_getlex(f,buttonname);
                abc_getlex(f,"flash.events::MouseEvent");
                abc_getproperty(f, "::CLICK");
                sprintf(functionname, "::clickLink1");
                abc_getlex(f,functionname);
                abc_callpropvoid(f, "::addEventListener" ,2);

                if(stop_each_frame) {
                    abc_findpropstrict(f, "[package]::stop");
                    abc_callpropvoid(f, "[package]::stop", 0);
                }
                needs_framescript = 1;
            }
            if(tag->id == ST_SHOWFRAME) {
                if(f) {
                    abc_returnvoid(f);
                    f = 0;
                }
                frame++;
            }
            tag = tag->next;
        }
        if(f) {
            abc_returnvoid(f);
        }
    }
    abc_returnvoid(c);

    tag = swf->firstTag;
    while(tag) {
        if(tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2) {
            char buttonname[80];
            sprintf(buttonname, "::button%d", swf_GetDefineID(tag));
            abc_AddSlot(cls, buttonname, 0, "flash.display::SimpleButton");
        }
        tag = tag->next;
    }

    c = abc_class_method(cls, "::void", "::clickLink1", 1, "flash.events::MouseEvent");
    c->max_stack = 3;
    c->local_count = 2;
    c->init_scope_depth = 10;
    c->max_scope_depth = 11;
    abc_getlocal_0(c);
    abc_pushscope(c);
    abc_findpropstrict(c,"flash.net::navigateToURL");
    abc_findpropstrict(c,"flash.net::URLRequest");
    abc_pushstring(c,"http://www.quiss.org/");
    //abc_pushstring(c,"file:///home/kramm/c/swftools/lib/modules/test2.html");
    abc_constructprop(c,"flash.net::URLRequest", 1);
    abc_callpropvoid(c,"flash.net::navigateToURL", 1);
    abc_returnvoid(c);
  

    abc_script_t*s = abc_initscript(file, 0, 0);
    c = (abc_method_body_t*)dict_getdata(file->method_bodies, s->method->method_body_index);
    c->max_stack = 2;
    c->local_count = 1;
    c->init_scope_depth = 1;
    c->max_scope_depth = 9;
    abc_getlocal_0(c);
    abc_pushscope(c);
    abc_getscopeobject(c, 0);
    abc_getlex(c,"::Object");
    abc_pushscope(c);
    abc_getlex(c,"flash.events::EventDispatcher");
    abc_pushscope(c);
    abc_getlex(c,"flash.display::DisplayObject");
    abc_pushscope(c);
    abc_getlex(c,"flash.display::InteractiveObject");
    abc_pushscope(c);
    abc_getlex(c,"flash.display::DisplayObjectContainer");
    abc_pushscope(c);
    abc_getlex(c,"flash.display::Sprite");
    abc_pushscope(c);
    abc_getlex(c,"flash.display::MovieClip");
    abc_pushscope(c);
    abc_getlex(c,"flash.display::MovieClip");
    abc_newclass(c,cls);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_initproperty(c,"rfx::MainTimeline");
    abc_returnvoid(c);

    //abc_method_body_addClassTrait(c, "rfx:MainTimeline", 1, cls);
    abc_initscript_addClassTrait(s, "rfx::MainTimeline", 1, cls);

    swf_WriteABC(abctag, file);
}

