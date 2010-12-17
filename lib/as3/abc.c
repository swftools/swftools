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
#include "assets.h"

char stringbuffer[2048];

int abc_RegisterNameSpace(abc_file_t*file, const char*name);
int abc_RegisterPackageNameSpace(abc_file_t*file, const char*name);
int abc_RegisterPackageInternalNameSpace(abc_file_t*file, const char*name);
int abc_RegisterProtectedNameSpace(abc_file_t*file, const char*name);
int abc_RegisterExplicitNameSpace(abc_file_t*file, const char*name);
int abc_RegisterStaticProtectedNameSpace(abc_file_t*file, const char*name);
int abc_RegisterPrivateNameSpace(abc_file_t*file, const char*name);

/* TODO: switch to a datastructure with just values */
#define NO_KEY ""

static void params_dump(FILE*fo, multiname_list_t*l, constant_list_t*o)
{
    int n = list_length(l);
    int no = list_length(o);
    int i = 0;

    fprintf(fo, "(");
    while(l) {
        char*s = multiname_tostring(l->multiname);
        fprintf(fo, "%s", s);
        free(s);
        if(i>=n-no) {
            s = constant_tostring(o->constant);
            fprintf(fo, " = ");
            fprintf(fo, "%s", s);
            free(s);
            o = o->next;
        }

        if(l->next)
            fprintf(fo, ", ");
        l = l->next;i++;
    }
    fprintf(fo, ")");
}

//#define DEBUG
#define DEBUG if(0)

static void parse_metadata(TAG*tag, abc_file_t*file, pool_t*pool)
{
    int t;
    int num_metadata = swf_GetU30(tag);

    DEBUG printf("%d metadata\n", num_metadata);
    for(t=0;t<num_metadata;t++) {
        const char*entry_name = pool_lookup_string(pool, swf_GetU30(tag));
        int num = swf_GetU30(tag);
        int s;
        DEBUG printf("  %s\n", entry_name);
        array_t*items = array_new();
        for(s=0;s<num;s++) {
            int i1 = swf_GetU30(tag);
            int i2 = swf_GetU30(tag);
            const char*key = i1?pool_lookup_string(pool, i1):"";
            const char*value = i2?pool_lookup_string(pool, i2):"";
            DEBUG printf("    %s=%s\n", key, value);
            array_append(items, key, strdup(value));
        }
        array_append(file->metadata, entry_name, items);
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
    f->flags = ABCFILE_LAZY;

    return f;
}

abc_class_t* abc_class_new(abc_file_t*file, multiname_t*classname, multiname_t*superclass) {
    
    NEW(abc_class_t,c);
    if(file)
        array_append(file->classes, NO_KEY, c);

    c->file = file;
    c->classname = multiname_clone(classname);
    c->superclass = multiname_clone(superclass);
    c->flags = 0;
    c->constructor = 0;
    c->static_constructor = 0;
    c->traits = list_new();
    return c;
}
abc_class_t* abc_class_new2(abc_file_t*pool, char*classname, char*superclass) 
{
    return abc_class_new(pool, multiname_fromstring(classname), multiname_fromstring(superclass));
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
    c->protectedNS = namespace_new_protected(namespace);
    c->flags |= CLASS_PROTECTED_NS;
}
void abc_class_add_interface(abc_class_t*c, multiname_t*interface)
{
    list_append(c->interfaces, multiname_clone(interface));
}
char*abc_class_fullname(abc_class_t*cls)
{
    const char*package = cls->classname->ns->name;
    const char*name = cls->classname->name;
    int l1 = strlen(package);
    int l2 = strlen(name);
    char*fullname = malloc(l1+l2+2);
    if(l1) {
	memcpy(fullname, package, l1);
	fullname[l1++]='.';
    }
    memcpy(fullname+l1, name, l2+1);
    return fullname;
}

void abc_method_init(abc_method_t*m, abc_file_t*file, multiname_t*returntype, char body)
{
    /* construct method object */
    m->index = array_length(file->methods);
    array_append(file->methods, NO_KEY, m);
    m->return_type = returntype;

    if(body) {
        /* construct code (method body) object */
        NEW(abc_method_body_t,c);
        array_append(file->method_bodies, NO_KEY, c);
        c->index = array_length(file->method_bodies);
        c->file = file;
        c->traits = list_new();
        c->code = 0;

        /* crosslink the two objects */
        m->body = c;
        c->method = m;
    }
}
abc_method_t* abc_method_new(abc_file_t*file, multiname_t*returntype, char body)
{
    NEW(abc_method_t,m);
    abc_method_init(m, file, returntype, body);
    return m;
}

abc_method_t* abc_class_getconstructor(abc_class_t*cls, multiname_t*returntype)
{
    if(cls->constructor) {
        return cls->constructor;
    }
    abc_method_t* m = abc_method_new(cls->file, returntype, 1);
    cls->constructor = m;
    return m;
}

abc_method_t* abc_class_getstaticconstructor(abc_class_t*cls, multiname_t*returntype)
{
    if(cls->static_constructor) {
        return cls->static_constructor;
    }
    abc_method_t* m = abc_method_new(cls->file, returntype, 1);
    cls->static_constructor = m;
    return m;
}

trait_t*trait_new(int type, multiname_t*name, int data1, int data2, constant_t*v)
{
    trait_t*trait = malloc(sizeof(trait_t));
    memset(trait, 0, sizeof(trait_t));
    trait->kind = type&0x0f;
    trait->attributes = type&0xf0;
    trait->name = name;
    trait->data1 = data1;
    trait->data2 = data2;
    trait->value = v;
    return trait;
}

trait_t*trait_new_member(trait_list_t**traits, multiname_t*type, multiname_t*name,constant_t*v)
{
    int kind = TRAIT_SLOT;
    trait_t*trait = malloc(sizeof(trait_t));
    memset(trait, 0, sizeof(trait_t));
    trait->kind = kind&0x0f;
    trait->attributes = kind&0xf0;
    trait->name = name;
    trait->type_name = type;
   
    trait->slot_id = list_length(*traits)+1;
    trait_list_t*l = *traits;
    list_append_(traits, trait);
    return trait;
}
trait_t*trait_new_method(trait_list_t**traits, multiname_t*name, abc_method_t*m)
{
    int type = TRAIT_METHOD;
    trait_t*trait = malloc(sizeof(trait_t));
    memset(trait, 0, sizeof(trait_t));
    trait->kind = type&0x0f;
    trait->attributes = type&0xf0;
    trait->name = name;
    trait->method = m;
    
    /* start assigning traits at position #1.
       Weird things happen when assigning slot 0- slot 0 and 1 seem
       to be identical */
    trait->slot_id = list_length(*traits)+1;
    list_append_(traits, trait);
    return trait;
}

abc_method_t* abc_class_method(abc_class_t*cls, multiname_t*returntype, multiname_t*name)
{
    abc_file_t*file = cls->file;
    abc_method_t* m = abc_method_new(cls->file, returntype, !(cls->flags&CLASS_INTERFACE));
    m->trait = trait_new_method(&cls->traits, multiname_clone(name), m);
    return m;
}
abc_method_t* abc_class_staticmethod(abc_class_t*cls, multiname_t*returntype, multiname_t*name)
{
    abc_file_t*file = cls->file;
    abc_method_t* m = abc_method_new(cls->file, returntype, !(cls->flags&CLASS_INTERFACE));
    m->trait = trait_new_method(&cls->static_traits, multiname_clone(name), m);
    return m;
}

trait_t* abc_class_slot(abc_class_t*cls, multiname_t*name, multiname_t*type)
{
    abc_file_t*file = cls->file;
    multiname_t*m_name = multiname_clone(name);
    multiname_t*m_type = multiname_clone(type);
    trait_t*t = trait_new_member(&cls->traits, m_type, m_name, 0);
    return t;
}
trait_t* abc_class_staticslot(abc_class_t*cls, multiname_t*name, multiname_t*type)
{
    abc_file_t*file = cls->file;
    multiname_t*m_name = multiname_clone(name);
    multiname_t*m_type = multiname_clone(type);
    trait_t*t = trait_new_member(&cls->static_traits, m_type, m_name, 0);
    return t;
}


trait_t* traits_find_slotid(trait_list_t*traits, int slotid)
{
    trait_list_t*l;
    trait_t*t=0;
    for(l=traits;l;l=l->next) {
        if(l->trait->slot_id==slotid) {
            t=l->trait; 
            break;
        }
    }
    return t;
}

void abc_method_body_addClassTrait(abc_method_body_t*code, char*multiname, int slotid, abc_class_t*cls)
{
    abc_file_t*file = code->file;
    multiname_t*m = multiname_fromstring(multiname);
    trait_t*trait = trait_new(TRAIT_CLASS, m, slotid, 0, 0);
    trait->cls = cls;
    list_append(code->traits, trait);
}

/* notice: traits of a method (body) belonging to an init script
   and traits of the init script are *not* the same thing */
trait_t* abc_initscript_addClassTrait(abc_script_t*script, multiname_t*multiname, abc_class_t*cls)
{
    abc_file_t*file = script->file;
    multiname_t*m = multiname_clone(multiname);
    int slotid = list_length(script->traits)+1;
    trait_t*trait = trait_new(TRAIT_CLASS, m, slotid, 0, 0);
    trait->cls = cls;
    list_append(script->traits, trait);
    return trait;
}

abc_script_t* abc_initscript(abc_file_t*file)
{
    abc_method_t*m = abc_method_new(file, 0, 1);
    abc_script_t* s = malloc(sizeof(abc_script_t));
    s->method = m;
    s->traits = list_new();
    s->file = file;
    array_append(file->scripts, NO_KEY, s);
    return s;
}

static void traits_dump(FILE*fo, const char*prefix, trait_list_t*traits, abc_file_t*file, dict_t*methods_seen);

static void dump_method(FILE*fo, const char*prefix, 
                                 const char*attr, 
                                 const char*type, 
                                 const char*name, 
                                 abc_method_t*m, abc_file_t*file, dict_t*methods_seen)
{
    if(methods_seen)
        dict_put(methods_seen, m, 0);

    char*return_type = 0;
    if(m->return_type)
        return_type = multiname_tostring(m->return_type);
    else
        return_type = strdup("*");

    fprintf(fo, "%s", prefix);
    fprintf(fo, "%s %s ", attr, type);
    fprintf(fo, "%s %s=%s", return_type, name, m->name);
    params_dump(fo, m->parameters, m->optional_parameters);
    fprintf(fo, "(%d params, %d optional)\n", list_length(m->parameters), list_length(m->optional_parameters));

    free(return_type);return_type=0;

    abc_method_body_t*c = m->body;
    if(!c) {
        return;
    }
    
    fprintf(fo, "%s[stack:%d locals:%d scope:%d-%d flags:", 
            prefix, c->old.max_stack, c->old.local_count, c->old.init_scope_depth, 
            c->old.max_scope_depth);


    int flags = c->method->flags;
    if(flags&METHOD_NEED_ARGUMENTS) {fprintf(fo, " need_arguments");flags&=~METHOD_NEED_ARGUMENTS;}
    if(flags&METHOD_NEED_ACTIVATION) {fprintf(fo, " need_activation");flags&=~METHOD_NEED_ACTIVATION;}
    if(flags&METHOD_NEED_REST) {fprintf(fo, " need_rest");flags&=~METHOD_NEED_REST;}
    if(flags&METHOD_HAS_OPTIONAL) {fprintf(fo, " has_optional");flags&=~METHOD_HAS_OPTIONAL;}
    if(flags&METHOD_SET_DXNS) {fprintf(fo, " set_dxns");flags&=~METHOD_SET_DXNS;}
    if(flags&METHOD_HAS_PARAM_NAMES) {fprintf(fo, " has_param_names");flags&=~METHOD_HAS_PARAM_NAMES;}
    if(flags) fprintf(fo, " %02x", flags);
    fprintf(fo, "]");

    if(m->trait) {
        fprintf(fo, " slot:%d", m->trait->slot_id);
    }
    fprintf(fo, "\n");


    char prefix2[80];
    sprintf(prefix2, "%s    ", prefix);
    if(c->traits)
        traits_dump(fo, prefix, c->traits, file, methods_seen);
    fprintf(fo, "%s{\n", prefix);
    code_dump2(c->code, c->exceptions, file, prefix2, fo);
    fprintf(fo, "%s}\n\n", prefix);
}

static void traits_free(trait_list_t*traits) 
{
    trait_list_t*t = traits;
    while(t) {
        if(t->trait->name) {
            multiname_destroy(t->trait->name);t->trait->name = 0;
        }
	if(t->trait->kind == TRAIT_SLOT || t->trait->kind == TRAIT_CONST) {
            multiname_destroy(t->trait->type_name);
        }
        if(t->trait->value) {
            constant_free(t->trait->value);t->trait->value = 0;
        }
        free(t->trait);t->trait = 0;
        t = t->next;
    }
    list_free(traits);
}
            
static char trait_is_method(trait_t*trait)
{
    return (trait->kind == TRAIT_METHOD || trait->kind == TRAIT_GETTER || 
            trait->kind == TRAIT_SETTER || trait->kind == TRAIT_FUNCTION);
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
        NEW(trait_t,trait);
	list_append(traits, trait);

	trait->name = multiname_clone(pool_lookup_multiname(pool, swf_GetU30(tag))); // always a QName (ns,name)

	const char*name = 0;
	DEBUG name = multiname_tostring(trait->name);
	U8 kind = swf_GetU8(tag);
        U8 attributes = kind&0xf0;
        kind&=0x0f;
        trait->kind = kind;
        trait->attributes = attributes;
	DEBUG printf("  trait %d) %s type=%02x\n", t, name, kind);
	if(kind == TRAIT_METHOD || kind == TRAIT_GETTER || kind == TRAIT_SETTER) { // method / getter / setter
	    trait->disp_id = swf_GetU30(tag);
	    trait->method = (abc_method_t*)array_getvalue(file->methods, swf_GetU30(tag));
            trait->method->trait = trait;
	    DEBUG printf("  method/getter/setter\n");
	} else if(kind == TRAIT_FUNCTION) { // function
	    trait->slot_id =  swf_GetU30(tag);
	    trait->method = (abc_method_t*)array_getvalue(file->methods, swf_GetU30(tag));
            trait->method->trait = trait;
	} else if(kind == TRAIT_CLASS) { // class
	    trait->slot_id = swf_GetU30(tag);
	    trait->cls = (abc_class_t*)array_getvalue(file->classes, swf_GetU30(tag));
	    DEBUG printf("  class %s %d %p\n", name, trait->slot_id, (int)trait->cls);
	} else if(kind == TRAIT_SLOT || kind == TRAIT_CONST) { // slot, const
	    trait->slot_id = swf_GetU30(tag);
            trait->type_name = multiname_clone(pool_lookup_multiname(pool, swf_GetU30(tag)));
	    int vindex = swf_GetU30(tag);
	    if(vindex) {
		int vkind = swf_GetU8(tag);
                trait->value = constant_fromindex(pool, vindex, vkind);
	    }
	    DEBUG printf("  slot %s %d %s (%s)\n", name, trait->slot_id, trait->type_name->name, constant_tostring(trait->value));
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


static void traits_write(pool_t*pool, TAG*tag, trait_list_t*traits)
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
	swf_SetU8(tag, trait->kind|trait->attributes);

	swf_SetU30(tag, trait->data1);

        if(trait->kind == TRAIT_CLASS) {
            swf_SetU30(tag, trait->cls->index);
        } else if(trait->kind == TRAIT_GETTER ||
                  trait->kind == TRAIT_SETTER ||
                  trait->kind == TRAIT_METHOD) {
            swf_SetU30(tag, trait->method->index);
        } else if(trait->kind == TRAIT_SLOT ||
                  trait->kind == TRAIT_CONST) {
            int index = pool_register_multiname(pool, trait->type_name);
            swf_SetU30(tag, index);
        } else  {
	    swf_SetU30(tag, trait->data2);
        }

	if(trait->kind == TRAIT_SLOT || trait->kind == TRAIT_CONST) {
            int vindex = constant_get_index(pool, trait->value);
	    swf_SetU30(tag, vindex);
	    if(vindex) {
		swf_SetU8(tag, trait->value->type);
	    }
	}
        if(trait->attributes&0x40) {
            // metadata
            swf_SetU30(tag, 0);
        }
        traits = traits->next;
    }
}


static void traits_dump(FILE*fo, const char*prefix, trait_list_t*traits, abc_file_t*file, dict_t*methods_seen)
{
    int t;
    while(traits) {
	trait_t*trait = traits->trait;
	char*name = multiname_tostring(trait->name);
	U8 kind = trait->kind;
        U8 attributes = trait->attributes;

        char a = attributes & (TRAIT_ATTR_OVERRIDE|TRAIT_ATTR_FINAL);
        char* type = "";
        if(a==TRAIT_ATTR_FINAL)
            type = "final ";
        else if(a==TRAIT_ATTR_OVERRIDE)
            type = "override ";
        else if(a==(TRAIT_ATTR_OVERRIDE|TRAIT_ATTR_FINAL))
            type = "final override ";
        
        if(attributes&TRAIT_ATTR_METADATA)
            fprintf(fo, "<metadata>");

	if(kind == TRAIT_METHOD) {
            abc_method_t*m = trait->method;
	    dump_method(fo, prefix, type, "method", name, m, file, methods_seen);
	} else if(kind == TRAIT_GETTER) {
            abc_method_t*m = trait->method;
	    dump_method(fo, prefix, type, "getter", name, m, file, methods_seen);
        } else if(kind == TRAIT_SETTER) {
            abc_method_t*m = trait->method;
	    dump_method(fo, prefix, type, "setter", name, m, file, methods_seen);
	} else if(kind == TRAIT_FUNCTION) { // function
            abc_method_t*m = trait->method;
	    dump_method(fo, prefix, type, "function", name, m, file, methods_seen);
	} else if(kind == TRAIT_CLASS) { // class
            abc_class_t*cls = trait->cls;
            if(!cls) {
	        fprintf(fo, "%sslot %d: class %s=00000000\n", prefix, trait->slot_id, name);
            } else {
	        fprintf(fo, "%sslot %d: class %s=%s\n", prefix, trait->slot_id, name, cls->classname->name);
            }
	} else if(kind == TRAIT_SLOT || kind == TRAIT_CONST) { // slot, const
	    int slot_id = trait->slot_id;
	    char*type_name = multiname_tostring(trait->type_name);
            char*value = constant_tostring(trait->value);
	    fprintf(fo, "%sslot %d: %s %s:%s %s %s\n", prefix, trait->slot_id, 
                    kind==TRAIT_CONST?"const":"var", name, type_name, 
                    trait->value?"=":"", trait->value?value:"");
            if(value) free(value);
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

    if(file->name) {
        fprintf(fo, "%s#\n", prefix);
        fprintf(fo, "%s#name: %s\n", prefix, file->name);
        fprintf(fo, "%s#\n", prefix);
    }

    int t;
    for(t=0;t<file->metadata->num;t++) {
        const char*entry_name = array_getkey(file->metadata, t);
        fprintf(fo, "%s#Metadata \"%s\":\n", prefix, entry_name);
        int s;
        array_t*items = (array_t*)array_getvalue(file->metadata, t);
        for(s=0;s<items->num;s++) {
            fprintf(fo, "%s#  %s=%s\n", prefix, (char*)array_getkey(items, s), (char*)array_getvalue(items,s));
        }
        fprintf(fo, "%s#\n", prefix);
    }

    dict_t*methods_seen = dict_new2(&ptr_type);
    for(t=0;t<file->classes->num;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
        char prefix2[80];
        sprintf(prefix2, "%s    ", prefix);

        fprintf(fo, "%s", prefix);
        if(cls->flags&1) fprintf(fo, "sealed ");
        if(cls->flags&2) fprintf(fo, "final ");
        if(cls->flags&4) fprintf(fo, "interface ");
        if(cls->flags&8) {
            char*s = namespace_tostring(cls->protectedNS);
            fprintf(fo, "protectedNS(%s) ", s);
            free(s);
        }

        char*classname = multiname_tostring(cls->classname);
	fprintf(fo, "class %s", classname);
        free(classname);
        if(cls->superclass) {
            char*supername = multiname_tostring(cls->superclass);
            fprintf(fo, " extends %s", supername);
            free(supername);
        }
        if(cls->interfaces) {
            multiname_list_t*ilist = cls->interfaces;
            if(ilist)
                fprintf(fo, " implements");
            while(ilist) {
                char*s = multiname_tostring(ilist->multiname);
                fprintf(fo, " %s", s);
                free(s);
                ilist = ilist->next;
            }
            ilist->next;
        }
        if(cls->flags&0xf0) 
            fprintf(fo, "extra flags=%02x\n", cls->flags&0xf0);
	fprintf(fo, "%s{\n", prefix);

        dict_put(methods_seen, cls->static_constructor, 0);
        dict_put(methods_seen, cls->constructor, 0);

        if(cls->static_constructor) {
            dump_method(fo, prefix2, "", "staticconstructor", "", cls->static_constructor, file, methods_seen);
        }
        traits_dump(fo, prefix2, cls->static_traits, file, methods_seen);
	
        char*n = multiname_tostring(cls->classname);
        if(cls->constructor)
	    dump_method(fo, prefix2, "", "constructor", n, cls->constructor, file, methods_seen);
        free(n);
	traits_dump(fo, prefix2,cls->traits, file, methods_seen);

	if(cls->asset) {
	    swf_DumpAsset(fo, cls->asset, prefix2);
	}

        fprintf(fo, "%s}\n", prefix);
    }
    fprintf(fo, "%s\n", prefix);

    for(t=0;t<file->scripts->num;t++) {
        abc_script_t*s = (abc_script_t*)array_getvalue(file->scripts, t);
        dump_method(fo, prefix, "", "initmethod", "init", s->method, file, methods_seen);
        traits_dump(fo, prefix, s->traits, file, methods_seen);
    }
    
    char extra=0;
    for(t=0;t<file->methods->num;t++) {
        abc_method_t*m = (abc_method_t*)array_getvalue(file->methods, t);
        if(!dict_contains(methods_seen, m)) {
            if(!extra) {
                extra=1;
                fprintf(fo, "\n");
                fprintf(fo, "%s//internal (non-class non-script) methods:\n", prefix);
            }
            char name[18];
            sprintf(name, "%08x ", m->index);
            dump_method(fo, prefix, "", "internalmethod", name, m, file, methods_seen);
        }
    }
    dict_destroy(methods_seen);

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
        file->name = (name&&name[0])?strdup(name):0;
    }
    U32 version = swf_GetU32(tag);
    if(version!=0x002e0010) {
        fprintf(stderr, "Warning: unknown AVM2 version %08x\n", version);
    }

    pool_read(pool, tag);

    int num_methods = swf_GetU30(tag);
    DEBUG printf("%d methods\n", num_methods);
    for(t=0;t<num_methods;t++) {
	NEW(abc_method_t,m);
	int param_count = swf_GetU30(tag);
	int return_type_index = swf_GetU30(tag);
        if(return_type_index)
            m->return_type = multiname_clone(pool_lookup_multiname(pool, return_type_index));
        else
            m->return_type = 0;

	int s;
	for(s=0;s<param_count;s++) {
	    int type_index = swf_GetU30(tag);
            
            /* type_index might be 0 ("*") */
            multiname_t*param = type_index?multiname_clone(pool_lookup_multiname(pool, type_index)):0;
            list_append(m->parameters, param);
        }

	int namenr = swf_GetU30(tag);
	if(namenr)
	    m->name = strdup(pool_lookup_string(pool, namenr));
        else
	    m->name = strdup("");

	m->flags = swf_GetU8(tag);
	
        DEBUG printf("method %d) %s ", t, m->name);
        DEBUG params_dump(stdout, m->parameters, m->optional_parameters);
        DEBUG printf("flags=%02x\n", m->flags);

        if(m->flags&0x08) {
            m->optional_parameters = list_new();
            int num = swf_GetU30(tag);
            int s;
            for(s=0;s<num;s++) {
                int vindex = swf_GetU30(tag);
                U8 vkind = swf_GetU8(tag); // specifies index type for "val"
                constant_t*c = constant_fromindex(pool, vindex, vkind);
                list_append(m->optional_parameters, c);

            }
        }
	if(m->flags&0x80) {
            /* debug information- not used by avm2 */
            multiname_list_t*l = m->parameters;
            while(l) {
	        const char*name = pool_lookup_string(pool, swf_GetU30(tag));
                l = l->next;
            }
	}
        m->index = array_length(file->methods);
	array_append(file->methods, NO_KEY, m);
    }
            
    parse_metadata(tag, file, pool);
	
    /* skip classes, and scripts for now, and do the real parsing later */
    int num_classes = swf_GetU30(tag);
    int classes_pos = tag->pos;
    DEBUG printf("%d classes\n", num_classes);
    for(t=0;t<num_classes;t++) {
	abc_class_t*cls = malloc(sizeof(abc_class_t));
	memset(cls, 0, sizeof(abc_class_t));
	
	swf_GetU30(tag); //classname
	swf_GetU30(tag); //supername

        array_append(file->classes, NO_KEY, cls);

	cls->flags = swf_GetU8(tag);
        DEBUG printf("class %d %02x\n", t, cls->flags);
	if(cls->flags&8) 
	    swf_GetU30(tag); //protectedNS
        int s;
	int inum = swf_GetU30(tag); //interface count
        cls->interfaces = 0;
        for(s=0;s<inum;s++) {
            int interface_index = swf_GetU30(tag);
            multiname_t* m = multiname_clone(pool_lookup_multiname(pool, interface_index));
            list_append(cls->interfaces, m);
            DEBUG printf("  class %d interface: %s\n", t, m->name);
        }

	int iinit = swf_GetU30(tag); //iinit
        DEBUG printf("--iinit-->%d\n", iinit);
	traits_skip(tag);
    }
    for(t=0;t<num_classes;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
	int cinit = swf_GetU30(tag);
        DEBUG printf("--cinit(%d)-->%d\n", t, cinit);
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
	c->old.max_stack = swf_GetU30(tag);
	c->old.local_count = swf_GetU30(tag);
	c->old.init_scope_depth = swf_GetU30(tag);
	c->old.max_scope_depth = swf_GetU30(tag);

        c->init_scope_depth = c->old.init_scope_depth;
	int code_length = swf_GetU30(tag);

	c->method = m;
	m->body = c;

        int pos = tag->pos + code_length;
        codelookup_t*codelookup = 0;
        c->code = code_parse(tag, code_length, file, pool, &codelookup);
        tag->pos = pos;

	int exception_count = swf_GetU30(tag);
        int s;
        c->exceptions = list_new();
        for(s=0;s<exception_count;s++) {
            abc_exception_t*e = malloc(sizeof(abc_exception_t));

            e->from = code_atposition(codelookup, swf_GetU30(tag));
            e->to = code_atposition(codelookup, swf_GetU30(tag));
            e->target = code_atposition(codelookup, swf_GetU30(tag));

            e->exc_type = multiname_clone(pool_lookup_multiname(pool, swf_GetU30(tag)));
            e->var_name = multiname_clone(pool_lookup_multiname(pool, swf_GetU30(tag)));
            //e->var_name = pool_lookup_string(pool, swf_GetU30(tag));
            //if(e->var_name) e->var_name = strdup(e->var_name);
            list_append(c->exceptions, e);
        }
        codelookup_free(codelookup);
	c->traits = traits_parse(tag, pool, file);

	DEBUG printf("method_body %d) (method %d), %d bytes of code\n", t, methodnr, code_length);

	array_append(file->method_bodies, NO_KEY, c);
    }
    if(tag->len - tag->pos) {
	fprintf(stderr, "ERROR: %d unparsed bytes remaining in ABC block\n", tag->len - tag->pos);
	return 0;
    }

    swf_SetTagPos(tag, classes_pos);
    for(t=0;t<num_classes;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);

        int classname_index = swf_GetU30(tag);
        int superclass_index = swf_GetU30(tag);
	cls->classname = multiname_clone(pool_lookup_multiname(pool, classname_index));
	cls->superclass = multiname_clone(pool_lookup_multiname(pool, superclass_index));
	cls->flags = swf_GetU8(tag);
	const char*ns = "";
	if(cls->flags&8) {
            int ns_index = swf_GetU30(tag);
	    cls->protectedNS = namespace_clone(pool_lookup_namespace(pool, ns_index));
	}
        
        int num_interfaces = swf_GetU30(tag); //interface count
        int s;
        for(s=0;s<num_interfaces;s++) {
            swf_GetU30(tag);
        }
	int iinit = swf_GetU30(tag);
        cls->constructor = (abc_method_t*)array_getvalue(file->methods, iinit);
	cls->traits = traits_parse(tag, pool, file);
    }
    for(t=0;t<num_classes;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
        /* SKIP */
	swf_GetU30(tag); // cindex
	cls->static_traits = traits_parse(tag, pool, file);
    }
    int num_scripts2 = swf_GetU30(tag);
    for(t=0;t<num_scripts2;t++) {
	int init = swf_GetU30(tag);
        abc_method_t*m = (abc_method_t*)array_getvalue(file->methods, init);
        
        abc_script_t*s = malloc(sizeof(abc_script_t));
        memset(s, 0, sizeof(abc_script_t));
        s->method = m;
        s->traits = traits_parse(tag, pool, file);
        array_append(file->scripts, NO_KEY, s);
    }

    pool_destroy(pool);
    return file;
}

static pool_t*writeABC(TAG*abctag, void*code, pool_t*pool)
{
    abc_file_t*file = (abc_file_t*)code;
    if(!pool) 
        pool = pool_new();
    if(!file)
        file = abc_file_new();

    TAG*tmp = swf_InsertTag(0,0);
    TAG*tag = tmp;
    int t;
  
    /* add method bodies where needed */
    for(t=0;t<file->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)array_getvalue(file->classes, t);
        if(!c->constructor) {
            if(!(c->flags&CLASS_INTERFACE)) {
                NEW(abc_method_t,m);array_append(file->methods, NO_KEY, m);
                NEW(abc_method_body_t,body);array_append(file->method_bodies, NO_KEY, body);
                // don't bother to set m->index
                body->method = m; m->body = body;
		if(c->superclass && c->superclass->name && strcmp(c->superclass->name,"Object")) {
		    body->code = abc_getlocal_0(body->code);
		    body->code = abc_constructsuper(body->code, 0);
		}
                body->code = abc_returnvoid(body->code);
                c->constructor = m;
            } else {
                NEW(abc_method_t,m);array_append(file->methods, NO_KEY, m);
                c->constructor = m;
            }
        }
        if(!c->static_constructor) {
            NEW(abc_method_t,m);array_append(file->methods, NO_KEY, m);
            NEW(abc_method_body_t,body);array_append(file->method_bodies, NO_KEY, body);
            body->method = m; m->body = body;
            body->code = abc_returnvoid(0);
            c->static_constructor = m;
        }
    }


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
    
    /* generate code statistics */
    for(t=0;t<file->method_bodies->num;t++) {
        abc_method_body_t*m = (abc_method_body_t*)array_getvalue(file->method_bodies, t);
        m->stats = code_get_statistics(m->code, m->exceptions);
    }
    
    /* level init scope depths: The init scope depth of a method is
       always as least as high as the init scope depth of it's surrounding
       class.
       A method has it's own init_scope_depth if it's an init method 
       (then its init scope depth is zero), or if it's used as a closure.

       Not sure yet what to do with methods which are used at different
       locations- e.g. the nullmethod is used all over the place.
       EDIT: flashplayer doesn't allow this anyway- a method can only
             be used once

       Also, I have the strong suspicion that flash player uses only
       the difference between max_scope_stack and init_scope_stack, anyway.
     */
    for(t=0;t<file->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)array_getvalue(file->classes, t);
        trait_list_t*traits = c->traits;
        if(c->constructor && c->constructor->body &&
           c->constructor->body->init_scope_depth < c->init_scope_depth) {
           c->constructor->body->init_scope_depth = c->init_scope_depth;
        }
        if(c->static_constructor && c->static_constructor->body &&
           c->static_constructor->body->init_scope_depth < c->init_scope_depth) {
           c->static_constructor->body->init_scope_depth = c->init_scope_depth;
        }
        while(traits) {
            trait_t*trait = traits->trait;
            if(trait_is_method(trait) && trait->method->body) {
                abc_method_body_t*body = trait->method->body;
	        if(body->init_scope_depth < c->init_scope_depth) {
                   body->init_scope_depth = c->init_scope_depth;
                }
            }
            traits = traits->next;
        }
    }
    
    for(t=0;t<file->methods->num;t++) {
	abc_method_t*m = (abc_method_t*)array_getvalue(file->methods, t);
        int n = 0;
        multiname_list_t*l = m->parameters;
        int num_params = list_length(m->parameters);
	swf_SetU30(tag, num_params);
        if(m->return_type) 
	    swf_SetU30(tag, pool_register_multiname(pool, m->return_type));
        else
	    swf_SetU30(tag, 0);
	int s;
        while(l) {
	    swf_SetU30(tag, pool_register_multiname(pool, l->multiname));
            l = l->next;
	}
        if(m->name) {
	    swf_SetU30(tag, pool_register_string(pool, m->name));
        } else {
	    swf_SetU30(tag, 0);
        }

        U8 flags = m->flags&(METHOD_NEED_REST|METHOD_NEED_ARGUMENTS);
        if(m->optional_parameters)
            flags |= METHOD_HAS_OPTIONAL;
        if(m->body) {
            flags |= m->body->stats->flags;
        }

	swf_SetU8(tag, flags);
        if(flags&METHOD_HAS_OPTIONAL) {
            swf_SetU30(tag, list_length(m->optional_parameters));
            constant_list_t*l = m->optional_parameters;
            while(l) {
                int i = constant_get_index(pool, l->constant);
                swf_SetU30(tag, i);
                if(!i) {
                    swf_SetU8(tag, CONSTANT_NULL);
                } else {
                    swf_SetU8(tag, l->constant->type);
                }
                l = l->next;
            }
        }
    }
   
    /* write metadata */
    swf_SetU30(tag, file->metadata->num);
    for(t=0;t<file->metadata->num;t++) {
        const char*entry_name = array_getkey(file->metadata, t);
        swf_SetU30(tag, pool_register_string(pool, entry_name));
        array_t*items = (array_t*)array_getvalue(file->metadata, t);
        swf_SetU30(tag, items->num);
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
            int ns_index = pool_register_namespace(pool, c->protectedNS);
	    swf_SetU30(tag, ns_index);
        }

        swf_SetU30(tag, list_length(c->interfaces));
        multiname_list_t*interface= c->interfaces;
        while(interface) {
            swf_SetU30(tag, pool_register_multiname(pool, interface->multiname));
            interface = interface->next;
        }

	assert(c->constructor);
	swf_SetU30(tag, c->constructor->index);

	traits_write(pool, tag, c->traits);
    }
    for(t=0;t<file->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)array_getvalue(file->classes, t);
	assert(c->static_constructor);
	swf_SetU30(tag, c->static_constructor->index);
        
        traits_write(pool, tag, c->static_traits);
    }

    swf_SetU30(tag, file->scripts->num);
    for(t=0;t<file->scripts->num;t++) {
	abc_script_t*s = (abc_script_t*)array_getvalue(file->scripts, t);
        if(!s->method->body || !s->method->body->code) {
            fprintf(stderr, "Internal Error: initscript has no body\n");
        }
	swf_SetU30(tag, s->method->index); //!=t!
	traits_write(pool, tag, s->traits);
    }

    swf_SetU30(tag, file->method_bodies->num);
    for(t=0;t<file->method_bodies->num;t++) {
	abc_method_body_t*c = (abc_method_body_t*)array_getvalue(file->method_bodies, t);
	abc_method_t*m = c->method;
	swf_SetU30(tag, m->index);

	//swf_SetU30(tag, c->old.max_stack);
	//swf_SetU30(tag, c->old.local_count);
	//swf_SetU30(tag, c->old.init_scope_depth);
	//swf_SetU30(tag, c->old.max_scope_depth);

	swf_SetU30(tag, c->stats->max_stack);
        int param_num = list_length(c->method->parameters)+1;
        if(c->method->flags&METHOD_NEED_REST)
            param_num++;
        if(param_num <= c->stats->local_count)
	    swf_SetU30(tag, c->stats->local_count);
        else
	    swf_SetU30(tag, param_num);

	swf_SetU30(tag, c->init_scope_depth);
	swf_SetU30(tag, c->stats->max_scope_depth+
                        c->init_scope_depth);

        code_write(tag, c->code, pool, file);

	swf_SetU30(tag, list_length(c->exceptions));
        abc_exception_list_t*l = c->exceptions;
        while(l) {
            // warning: assumes "pos" in each code_t is up-to-date
            swf_SetU30(tag, l->abc_exception->from->pos);
            swf_SetU30(tag, l->abc_exception->to->pos);
            swf_SetU30(tag, l->abc_exception->target->pos);
            swf_SetU30(tag, pool_register_multiname(pool, l->abc_exception->exc_type));
            swf_SetU30(tag, pool_register_multiname(pool, l->abc_exception->var_name));
            l = l->next;
        }

        traits_write(pool, tag, c->traits);
    }
   
    /* free temporary codestat data again. Notice: If we were to write this
       file multiple times, this can also be shifted to abc_file_free() */
    for(t=0;t<file->method_bodies->num;t++) {
        abc_method_body_t*m = (abc_method_body_t*)array_getvalue(file->method_bodies, t);
        codestats_free(m->stats);m->stats=0;
    }

    // --- start to write real tag --
    
    tag = abctag;

    if(tag->id == ST_DOABC) {
        swf_SetU32(tag, file->flags); // flags
        swf_SetString(tag, file->name);
    }

    swf_SetU16(tag, 0x10); //version
    swf_SetU16(tag, 0x2e);
    
    pool_write(pool, tag);
    
    swf_SetBlock(tag, tmp->data, tmp->len);

    swf_DeleteTag(0, tmp);
    return pool;
}

void swf_WriteABC(TAG*abctag, void*code)
{
    pool_t*pool = writeABC(abctag, code, 0);
    pool_optimize(pool);
    swf_ResetTag(abctag, abctag->id);
    writeABC(abctag, code, pool);
    pool_destroy(pool);
}

void abc_file_free(abc_file_t*file)
{
    if(!file)
        return;
    int t;
    if(file->metadata) {
        for(t=0;t<file->metadata->num;t++) {
            array_t*items = (array_t*)array_getvalue(file->metadata, t);
            int s;
            for(s=0;s<items->num;s++) {
                free(array_getvalue(items, s));
            }
            array_free(items);
        }
        array_free(file->metadata);file->metadata=0;
    }

    for(t=0;t<file->methods->num;t++) {
        abc_method_t*m = (abc_method_t*)array_getvalue(file->methods, t);

        multiname_list_t*param = m->parameters;
        while(param) {
            multiname_destroy(param->multiname);param->multiname=0;
            param = param->next;
        }
        list_free(m->parameters);m->parameters=0;
       
        constant_list_t*opt = m->optional_parameters;
        while(opt) {
            constant_free(opt->constant);opt->constant=0;
            opt = opt->next;
        }
        list_free(m->optional_parameters);m->optional_parameters=0;

        if(m->name) {
            free((void*)m->name);m->name=0;
        }
        if(m->return_type) {
            multiname_destroy(m->return_type);
        }
        free(m);
    }
    array_free(file->methods);file->methods=0;

    for(t=0;t<file->classes->num;t++) {
        abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
        traits_free(cls->traits);cls->traits=0;
	traits_free(cls->static_traits);cls->static_traits=0;

        if(cls->classname) {
            multiname_destroy(cls->classname);
        }
        if(cls->superclass) {
            multiname_destroy(cls->superclass);
        }

        multiname_list_t*i = cls->interfaces;
        while(i) {
            multiname_destroy(i->multiname);i->multiname=0;
            i = i->next;
        }
        list_free(cls->interfaces);cls->interfaces=0;

        if(cls->protectedNS) {
	    namespace_destroy(cls->protectedNS);
        }
        free(cls);
    }
    array_free(file->classes);file->classes=0;

    for(t=0;t<file->scripts->num;t++) {
        abc_script_t*s = (abc_script_t*)array_getvalue(file->scripts, t);
        traits_free(s->traits);s->traits=0;
        free(s);
    }
    array_free(file->scripts);file->scripts=0;

    for(t=0;t<file->method_bodies->num;t++) {
        abc_method_body_t*body = (abc_method_body_t*)array_getvalue(file->method_bodies, t);
        code_free(body->code);body->code=0;
	traits_free(body->traits);body->traits=0;

        abc_exception_list_t*ee = body->exceptions;
        while(ee) {
            abc_exception_t*e=ee->abc_exception;ee->abc_exception=0;
            e->from = e->to = e->target = 0;
            multiname_destroy(e->exc_type);e->exc_type=0;
            multiname_destroy(e->var_name);e->var_name=0;
            free(e);
            ee=ee->next;
        }
        list_free(body->exceptions);body->exceptions=0;
        
        free(body);
    }
    array_free(file->method_bodies);file->method_bodies=0;

    if(file->name) {
        free((void*)file->name);file->name=0;
    }

    free(file);
}

void swf_FreeABC(void*code)
{
    abc_file_t*file= (abc_file_t*)code;
    abc_file_free(file);
}

