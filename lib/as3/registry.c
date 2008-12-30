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

static dict_t*classes=0;

// ----------------------- class signature ------------------------------

char classinfo_equals(classinfo_t*c1, classinfo_t*c2)
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
static unsigned int classinfo_hash(classinfo_t*c)
{
    unsigned int hash = 0;
    hash = crc32_add_string(hash, c->package);
    hash = crc32_add_string(hash, c->name);
    return hash;
}

static void* dummy_clone(void*other) {return other;}
static void dummy_destroy(classinfo_t*c) {}

type_t classinfo_type = {
    hash: (hash_func)classinfo_hash,
    equals: (equals_func)classinfo_equals,
    /* all signatures are static */
    dup: (dup_func)dummy_clone,
    free: (free_func)dummy_destroy,
};

// ----------------------- function signature ------------------------------

static char memberinfo_equals(memberinfo_t*f1, memberinfo_t*f2)
{
    return !strcmp(f1->name, f2->name);
}
static unsigned int memberinfo_hash(memberinfo_t*f)
{
    return crc32_add_string(0, f->name);
}
type_t memberinfo_type = {
    hash: (hash_func)memberinfo_hash,
    equals: (equals_func)memberinfo_equals,
    /* all signatures are static */
    dup: (dup_func)dummy_clone,
    free: (free_func)dummy_destroy,
};

// ------------------------- constructors --------------------------------

#define AVERAGE_NUMBER_OF_MEMBERS 8
classinfo_t* classinfo_register(int access, char*package, char*name, int num_interfaces)
{
    classinfo_t*c = rfx_calloc(sizeof(classinfo_t)+(sizeof(classinfo_t*)*(num_interfaces+1)));
    c->interfaces[0] = 0;
    c->access = access;
    c->package = package;
    c->name = name;
    dict_put(classes, c, c);
    dict_init(&c->members,AVERAGE_NUMBER_OF_MEMBERS);
    return c;
}

/* function and class pointers get their own type class */
static dict_t* functionobjects = 0;
classinfo_t* registry_getfunctionclass(memberinfo_t*f) {
    if(!functionobjects) {
        functionobjects = dict_new2(&ptr_type);
    } else {
        classinfo_t*c = dict_lookup(functionobjects, f);
        if(c)
            return c;
    }

    NEW(classinfo_t,c);
    c->access = ACCESS_PUBLIC;
    c->package = "";
    c->name = "Function";
    dict_init(&c->members,1);
    dict_put(&c->members, "call", f);

    dict_put(functionobjects, f, c);
    return c;
}
static dict_t* classobjects = 0;
classinfo_t* registry_getclassclass(classinfo_t*a) {
    if(!classobjects) {
        classobjects = dict_new2(&ptr_type);
    } else {
        classinfo_t*c = dict_lookup(classobjects, a);
        if(c)
            return c;
    }

    NEW(classinfo_t,c);
    c->access = ACCESS_PUBLIC;
    c->package = "";
    c->name = "Class";
    
    NEW(memberinfo_t,m);
    m->kind = MEMBER_SLOT;
    m->name = "prototype";
    m->type = a;

    dict_init(&c->members,1);
    dict_put(&c->members, "prototype", m);

    dict_put(classobjects, a, c);
    return c;
}

memberinfo_t* memberinfo_register(classinfo_t*cls, const char*name, U8 kind)
{
    NEW(memberinfo_t,m);
    m->kind = kind;
    m->name = strdup(name);
    dict_put(&cls->members, name, m);
    return m;
}

// --------------- builtin classes (from builtin.c) ----------------------

void registry_init()
{
    classes = builtin_getclasses();
}
classinfo_t* registry_safefindclass(const char*package, const char*name)
{
    classinfo_t*c = registry_findclass(package, name);
    assert(c);
    return c;
}
classinfo_t* registry_findclass(const char*package, const char*name)
{
    assert(classes);
    classinfo_t tmp;
    tmp.package = package;
    tmp.name = name;
    classinfo_t* c = (classinfo_t*)dict_lookup(classes, &tmp);
    /*if(c)
        printf("%s.%s->%08x (%s.%s)\n", package, name, c, c->package, c->name);*/
    return c;
}
memberinfo_t* registry_findmember(classinfo_t*cls, const char*name)
{
    return (memberinfo_t*)dict_lookup(&cls->members, name);
}
void registry_fill_multiname(multiname_t*m, namespace_t*n, classinfo_t*c)
{
    m->type = QNAME;
    m->ns = n;
    m->ns->access = c->access;
    m->ns->name = (char*)c->package;
    m->name = c->name;
    m->namespace_set = 0;
}
multiname_t* classinfo_to_multiname(classinfo_t*cls)
{
    if(!cls)
        return 0;
    multiname_t*m=0;
    namespace_t*ns = namespace_new(cls->access, cls->package);
    return multiname_new(ns,cls->name);
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
    if(!c) c = registry_safefindclass("", "Object");
    return c;
}
classinfo_t* registry_getstringclass() {
    static classinfo_t*c = 0;
    if(!c) c = registry_safefindclass("", "String");
    return c;
}
classinfo_t* registry_getintclass() {
    static classinfo_t*c = 0;
    if(!c) c = registry_safefindclass("", "int");
    return c;
}
classinfo_t* registry_getuintclass() {
    static classinfo_t*c = 0;
    if(!c) c = registry_safefindclass("", "uint");
    return c;
}
classinfo_t* registry_getbooleanclass() {
    static classinfo_t*c = 0;
    if(!c) c = registry_safefindclass("", "Boolean");
    return c;
}
classinfo_t* registry_getnumberclass() {
    static classinfo_t*c = 0;
    if(!c) c = registry_safefindclass("", "Number");
    return c;
}
classinfo_t* registry_getMovieClip() {
    static classinfo_t*c = 0;
    if(!c) c = registry_safefindclass("flash.display", "MovieClip");
    return c;
}

// ----------------------- builtin dummy types -------------------------
classinfo_t nullclass = {
    ACCESS_PACKAGE, 0, "", "null", 0, 0, 0,
};
classinfo_t* registry_getnullclass() {
    return &nullclass;
}

