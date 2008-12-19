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

char class_signature_equals(class_signature_t*c1, class_signature_t*c2)
{
    /* notice: access right is *not* respected */
    if(!strcmp(c1->name, c2->name) &&
       !strcmp(c1->package, c2->package)) {
        return 1;
    }
    return 0;
}
static unsigned int class_signature_hash(class_signature_t*c)
{
    unsigned int hash = 0;
    hash = crc32_add_string(hash, c->package);
    hash = crc32_add_string(hash, c->name);
    return hash;
}

static void* dummy_clone(void*other) {return other;}
static void dummy_destroy(class_signature_t*c) {}

type_t class_signature_type = {
    hash: (hash_func)class_signature_hash,
    equals: (equals_func)class_signature_equals,
    /* all signatures are static */
    dup: (dup_func)dummy_clone,
    free: (free_func)dummy_destroy,
};

// ----------------------- function signature ------------------------------

static char function_signature_equals(function_signature_t*f1, function_signature_t*f2)
{
    return !strcmp(f1->name, f2->name);
}
static unsigned int function_signature_hash(function_signature_t*f)
{
    return crc32_add_string(0, f->name);
}
type_t function_signature_type = {
    hash: (hash_func)function_signature_hash,
    equals: (equals_func)function_signature_equals,
    /* all signatures are static */
    dup: (dup_func)dummy_clone,
    free: (free_func)dummy_destroy,
};

// ------------------------- constructors --------------------------------

class_signature_t* class_signature_register(int access, char*package, char*name)
{
    class_signature_t*c = malloc(sizeof(class_signature_t));
    c->access = access;
    c->package = package;
    c->name = name;
    dict_put(classes, c, c);
    return c;
}

// --------------- builtin classes (from builtin.c) ----------------------

void registry_init()
{
    classes = builtin_getclasses();
}
class_signature_t* registry_safefindclass(const char*package, const char*name)
{
    class_signature_t*c = registry_findclass(package, name);
    if(!c)
        printf("%s.%s\n", package, name);
    assert(c);
    return c;
}
class_signature_t* registry_findclass(const char*package, const char*name)
{
    assert(classes);
    class_signature_t tmp;
    tmp.package = package;
    tmp.name = name;
    class_signature_t* c = (class_signature_t*)dict_lookup(classes, &tmp);
    return c;
}
void registry_fill_multiname(multiname_t*m, namespace_t*n, class_signature_t*c)
{
    m->type = QNAME;
    m->ns = n;
    m->ns->access = c->access;
    m->ns->name = (char*)c->package;
    m->name = c->name;
    m->namespace_set = 0;
}
multiname_t* class_signature_to_multiname(class_signature_t*cls)
{
    if(!cls)
        return 0;
    multiname_t*m=0;
    namespace_t*ns = namespace_new(cls->access, cls->package);
    return multiname_new(ns,cls->name);
}

// ----------------------- builtin types ------------------------------
class_signature_t* registry_getanytype() {return 0;/*FIXME*/}

class_signature_t* registry_getobjectclass() {return registry_safefindclass("", "Object");}
class_signature_t* registry_getstringclass() {return registry_safefindclass("", "String");}
class_signature_t* registry_getintclass() {return registry_safefindclass("", "int");}
class_signature_t* registry_getuintclass() {return registry_safefindclass("", "uint");}
class_signature_t* registry_getbooleanclass() {return registry_safefindclass("", "Boolean");}
class_signature_t* registry_getnumberclass() {return registry_safefindclass("", "Number");}
class_signature_t* registry_getMovieClip() {return registry_safefindclass("flash.display", "MovieClip");}

// ----------------------- builtin dummy types -------------------------
class_signature_t nullclass = {
    ACCESS_PACKAGE, "", "null", 0, 0, 0,
};
class_signature_t* registry_getnullclass() {
    return &nullclass;
}

