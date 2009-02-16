/* registry.c

   Routines for compiling Flash2 AVM2 ABC Actionscript

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

#include <assert.h>
#include "pool.h"
#include "registry.h"
#include "builtin.h"

dict_t*registry_classes=0;

// ----------------------- class signature ------------------------------

char slotinfo_equals(slotinfo_t*c1, slotinfo_t*c2)
{
    if(!!c1 != !!c2)
        return 0;
    /* notice: access right is *not* respected */
    if(!strcmp(c1->name, c2->name) &&
       !strcmp(c1->package, c2->package)) {
        return 1;
    }
    return 0;
}
static unsigned int slotinfo_hash(slotinfo_t*c)
{
    unsigned int hash = 0;
    hash = crc32_add_string(hash, c->package);
    hash = crc32_add_string(hash, c->name);
    return hash;
}

static void* dummy_clone(void*other) {return other;}
static void dummy_destroy(slotinfo_t*c) {}

type_t slotinfo_type = {
    hash: (hash_func)slotinfo_hash,
    equals: (equals_func)slotinfo_equals,
    dup: (dup_func)dummy_clone, // all signatures are static
    free: (free_func)dummy_destroy,
};

// ------------------------- constructors --------------------------------

#define AVERAGE_NUMBER_OF_MEMBERS 8
classinfo_t* classinfo_register(int access, const char*package, const char*name, int num_interfaces)
{
    classinfo_t*c = rfx_calloc(sizeof(classinfo_t)+(sizeof(classinfo_t*)*(num_interfaces+1)));
    c->interfaces[0] = 0;
    c->kind = INFOTYPE_CLASS;
    c->access = access;
    c->package = package;
    c->name = name;
    dict_put(registry_classes, c, c);
    dict_init2(&c->members, &slotinfo_type, AVERAGE_NUMBER_OF_MEMBERS);
    return c;
}
methodinfo_t* methodinfo_register_onclass(classinfo_t*cls, U8 access, const char*ns, const char*name)
{
    NEW(methodinfo_t,m);
    m->kind = INFOTYPE_METHOD;
    m->access = access;
    m->name = name;
    m->package = ns;
    m->parent = cls;
    dict_put(&cls->members, m, m);
    return m;
}
varinfo_t* varinfo_register_onclass(classinfo_t*cls, U8 access, const char*ns, const char*name)
{
    NEW(varinfo_t,m);
    m->kind = INFOTYPE_SLOT;
    m->access = access;
    m->name = name;
    m->package = ns;
    m->parent = cls;
    dict_put(&cls->members, m, m);
    return m;
}
methodinfo_t* methodinfo_register_global(U8 access, const char*package, const char*name)
{
    NEW(methodinfo_t, m);
    m->kind = INFOTYPE_METHOD;
    m->flags = FLAG_STATIC;
    m->access = access;
    m->package = package;
    m->name = name;
    m->parent = 0;
    dict_put(registry_classes, m, m);
    return m;
}
varinfo_t* varinfo_register_global(U8 access, const char*package, const char*name)
{
    NEW(varinfo_t, m);
    m->kind = INFOTYPE_SLOT;
    m->flags = FLAG_STATIC;
    m->access = access;
    m->package = package;
    m->name = name;
    m->parent = 0;
    dict_put(registry_classes, m, m);
    return m;
}

// --------------- builtin classes (from builtin.c) ----------------------

void registry_init()
{
    if(!registry_classes)
        registry_classes = builtin_getclasses();
}
slotinfo_t* registry_find(const char*package, const char*name)
{
    assert(registry_classes);
    slotinfo_t tmp;
    tmp.package = package;
    tmp.name = name;
    slotinfo_t* c = (slotinfo_t*)dict_lookup(registry_classes, &tmp);
    /*if(c)
        printf("%s.%s->%08x (%s.%s)\n", package, name, c, c->package, c->name);*/
    return c;
}
slotinfo_t* registry_safefind(const char*package, const char*name)
{
    slotinfo_t*c = registry_find(package, name);
    if(!c) {
        printf("%s.%s\n", package, name);
    }
    assert(c);
    return c;
}
void registry_dump()
{
    int t;
    for(t=0;t<registry_classes->hashsize;t++) {
        dictentry_t*e = registry_classes->slots[t];
        while(e) {
            slotinfo_t*i = (slotinfo_t*)e->key;
            printf("[%s] %s.%s\n", access2str(i->access), i->package, i->name);
            e = e->next;
        }
    }
}

memberinfo_t* registry_findmember(classinfo_t*cls, const char*ns, const char*name, char recursive)
{
    memberinfo_t tmp;
    tmp.name = name;
    tmp.package = ns?ns:"";

    if(!recursive) {
        return (memberinfo_t*)dict_lookup(&cls->members, &tmp);
    }
    /* look at classes directly extended by this class */
    slotinfo_t*m = 0;
    classinfo_t*s = cls;

    if(recursive>1) // check *only* superclasses
        s = s->superclass;

    while(s) {
        m = (slotinfo_t*)dict_lookup(&s->members, &tmp);
        if(m) {
            return (memberinfo_t*)m;
        }
        s = s->superclass;
    }
    /* look at interfaces, and parent interfaces */
    int t=0;
    while(cls->interfaces[t]) {
        classinfo_t*s = cls->interfaces[t];
        while(s) {
            m = (slotinfo_t*)dict_lookup(&s->members, &tmp);
            if(m) {
                return (memberinfo_t*)m;
            }
            s = s->superclass;
        }
        t++;
    }
    return 0;
}

memberinfo_t* registry_findmember_nsset(classinfo_t*cls, namespace_list_t*ns, const char*name, char superclasses)
{
    while(ns) {
        memberinfo_t*m = registry_findmember(cls, ns->namespace->name, name, superclasses);
        if(m) return m;
        ns = ns->next;
    }
    return registry_findmember(cls, "", name, superclasses);
}


void registry_fill_multiname(multiname_t*m, namespace_t*n, slotinfo_t*c)
{
    m->type = QNAME;
    m->ns = n;
    m->ns->access = c->access;
    m->ns->name = (char*)c->package;
    m->name = c->name;
    m->namespace_set = 0;
}
multiname_t* classinfo_to_multiname(slotinfo_t*cls)
{
    if(!cls)
        return 0;
    multiname_t*m=0;
    namespace_t ns = {cls->access, (char*)cls->package};
    return multiname_new(&ns,cls->name);
}

// ----------------------- memberinfo methods ------------------------------

/* hacky code to wrap a variable or function into a "type"
   object, but keep a pointer to the "value" */
static dict_t* functionobjects = 0;
classinfo_t* slotinfo_asclass(slotinfo_t*f) {
    if(!functionobjects) {
        functionobjects = dict_new2(&ptr_type);
    } else {
        classinfo_t*c = dict_lookup(functionobjects, f);
        if(c)
            return c;
    }

    classinfo_t*c = rfx_calloc(sizeof(classinfo_t)+sizeof(classinfo_t*));
    c->access = ACCESS_PUBLIC;
    c->package = "";
    if(f->kind == INFOTYPE_METHOD)
        c->name = "Function";
    else if(f->kind == INFOTYPE_CLASS)
        c->name = "Class";
    else if(f->kind == INFOTYPE_SLOT)
        c->name = "Object";
    else {
        c->name = "undefined";
    }
    
    dict_init2(&c->members, &slotinfo_type, 1);
    c->data = f;
    dict_put(functionobjects, f, c);
    return c;
}

classinfo_t* slotinfo_gettype(slotinfo_t*f)
{
    if(f) {
       if(f->kind == INFOTYPE_METHOD) {
           return slotinfo_asclass(f);
       } else if(f->kind == INFOTYPE_SLOT) {
           varinfo_t*v = (varinfo_t*)f;
           return v->type;
       } else 
           return 0;
    } else {
       return registry_getanytype();
    }
}
// ----------------------- builtin types ------------------------------
classinfo_t* registry_getanytype() {return 0;}

char registry_isfunctionclass(classinfo_t*c) {
    return (c && c->package && c->name && 
            !strcmp(c->package, "") && !strcmp(c->name, "Function"));
}
char registry_isclassclass(classinfo_t*c) {
    return (c && c->package && c->name && 
            !strcmp(c->package, "") && !strcmp(c->name, "Class"));
}

classinfo_t* registry_getobjectclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "Object");
    return c;
}
classinfo_t* registry_getstringclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "String");
    return c;
}
classinfo_t* registry_getarrayclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "Array");
    return c;
}
classinfo_t* registry_getintclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "int");
    return c;
}
classinfo_t* registry_getuintclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "uint");
    return c;
}
classinfo_t* registry_getbooleanclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "Boolean");
    return c;
}
classinfo_t* registry_getnumberclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "Number");
    return c;
}
classinfo_t* registry_getregexpclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "RegExp");
    return c;
}
classinfo_t* registry_getMovieClip() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("flash.display", "MovieClip");
    return c;
}

// ----------------------- builtin dummy types -------------------------
classinfo_t nullclass = {
    INFOTYPE_CLASS,0,0,ACCESS_PACKAGE, "", "null", 0, 0, 0
};
classinfo_t* registry_getnullclass() {
    return &nullclass;
}

namespace_t access2namespace(U8 access, char*package)
{
    namespace_t ns;
    ns.access = access;
    ns.name = package;
    return ns;
}

char* infotypename(slotinfo_t*s)
{
    if(s->kind == INFOTYPE_CLASS) return "class";
    else if(s->kind == INFOTYPE_SLOT) return "member";
    else if(s->kind == INFOTYPE_METHOD) return "method";
    else return "object";
}

