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
asset_bundle_list_t*assets=0;

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
static unsigned int memberinfo_hash(slotinfo_t*c)
{
    unsigned int hash = 0;
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
type_t memberinfo_type = {
    hash: (hash_func)memberinfo_hash,
    equals: (equals_func)slotinfo_equals,
    dup: (dup_func)dummy_clone, // all signatures are static
    free: (free_func)dummy_destroy,
};

// ----------------------- assets -------------------------------------
static void use_asset(asset_bundle_t*a)
{
    a->used = 1;
    asset_bundle_list_t*l = a->dependencies;
    while(l) {
	if(!l->asset_bundle->used) {
	    use_asset(l->asset_bundle);
	}
	l = l->next;
    }
}
void registry_use(slotinfo_t*s)
{
    if(!s) return;
    if(!(s->flags&FLAG_USED)) {
	s->flags |= FLAG_USED;
	if(s->kind == INFOTYPE_CLASS) {
	    classinfo_t*c=(classinfo_t*)s;
	    if(c->assets) {
		use_asset(c->assets);
	    }
	    int t=0;
	    while(c->interfaces[t]) {
		registry_use((slotinfo_t*)c->interfaces[t]);
		t++;
	    }
	    while(c->superclass) {
		c = c->superclass;
		registry_use((slotinfo_t*)c);
	    }
	} else if(s->kind == INFOTYPE_METHOD) {
	    methodinfo_t*m=(methodinfo_t*)s;
	    if(m->parent) {
		registry_use((slotinfo_t*)m->parent);
	    }
	} else if(s->kind == INFOTYPE_VAR) {
	    varinfo_t*v=(varinfo_t*)s;
	    if(v->parent) {
		registry_use((slotinfo_t*)v->parent);
	    }
	}
    }
}
void registry_add_asset(asset_bundle_t*bundle)
{
    list_append(assets, bundle);
}
asset_bundle_list_t*registry_getassets()
{
    return assets;
}
// ----------------------- resolving ----------------------------------
slotinfo_t* registry_resolve(slotinfo_t*_s)
{
    if(!_s || _s->kind != INFOTYPE_UNRESOLVED)
        return _s;
    unresolvedinfo_t*s = (unresolvedinfo_t*)_s;

    if(s->package)
        return registry_find(s->package, s->name);

    namespace_list_t*l = s->nsset;
    while(l) {
        slotinfo_t* n = registry_find(l->namespace->name, s->name);
        if(n) return n;
        l = l->next;
    }
    return 0;
}

static slotinfo_list_t*unresolved = 0;
static void schedule_for_resolve(slotinfo_t*s)
{
    list_append(unresolved, s);
}
static void resolve_on_slot(slotinfo_t*_member)
{
    if(_member->kind == INFOTYPE_VAR) {
        varinfo_t*member = (varinfo_t*)_member;
        member->type = (classinfo_t*)registry_resolve((slotinfo_t*)member->type);
    } else if(_member->kind == INFOTYPE_METHOD) {
        methodinfo_t*member = (methodinfo_t*)_member;
        member->return_type = (classinfo_t*)registry_resolve((slotinfo_t*)member->return_type);
        classinfo_list_t*l = member->params;
        while(l) {
            l->classinfo = (classinfo_t*)registry_resolve((slotinfo_t*)l->classinfo);
            l = l->next;
        }
    } else fprintf(stderr, "Internal Error: bad slot %s", _member->name);
}
static void resolve_on_class(slotinfo_t*_cls)
{
    classinfo_t*cls = (classinfo_t*)_cls;
    cls->superclass = (classinfo_t*)registry_resolve((slotinfo_t*)cls->superclass);
	
    DICT_ITERATE_DATA(&cls->members,slotinfo_t*,m) {
	resolve_on_slot(m);
    }
    DICT_ITERATE_DATA(&cls->static_members,slotinfo_t*,m2) {
	resolve_on_slot(m2);
    }

    int t=0;
    while(cls->interfaces[t]) {
        cls->interfaces[t] = (classinfo_t*)registry_resolve((slotinfo_t*)cls->interfaces[t]);
        t++;
    }
}
void registry_resolve_all()
{
    while(unresolved) {
        slotinfo_t*_s = unresolved->slotinfo;
        if(_s->kind == INFOTYPE_CLASS) {
            resolve_on_class(_s);
        } else if(_s->kind == INFOTYPE_METHOD || _s->kind == INFOTYPE_VAR) {
            resolve_on_slot(_s);
        } else {
            fprintf(stderr, "Internal Error: object %s.%s has bad type\n", _s->package, _s->name);
        }
        slotinfo_list_t*tofree = unresolved;
        unresolved = unresolved->next;
        free(tofree);
    }
}
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
    dict_init2(&c->members, &memberinfo_type, AVERAGE_NUMBER_OF_MEMBERS);
    dict_init2(&c->static_members, &memberinfo_type, AVERAGE_NUMBER_OF_MEMBERS);

    schedule_for_resolve((slotinfo_t*)c);
    return c;
}
methodinfo_t* methodinfo_register_onclass(classinfo_t*cls, U8 access, const char*ns, const char*name, char is_static)
{
    NEW(methodinfo_t,m);
    m->kind = INFOTYPE_METHOD;
    m->access = access;
    m->name = name;
    m->package = ns;
    m->parent = cls;
    if(!is_static) 
	dict_put(&cls->members, m, m);
    else
	dict_put(&cls->static_members, m, m);
    return m;
}
varinfo_t* varinfo_register_onclass(classinfo_t*cls, U8 access, const char*ns, const char*name, char is_static)
{
    NEW(varinfo_t,m);
    m->kind = INFOTYPE_VAR;
    m->access = access;
    m->name = name;
    m->package = ns;
    m->parent = cls;
    if(!is_static) 
	dict_put(&cls->members, m, m);
    else
	dict_put(&cls->static_members, m, m);
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
    
    schedule_for_resolve((slotinfo_t*)m);
    return m;
}
varinfo_t* varinfo_register_global(U8 access, const char*package, const char*name)
{
    NEW(varinfo_t, m);
    m->kind = INFOTYPE_VAR;
    m->flags = FLAG_STATIC;
    m->access = access;
    m->package = package;
    m->name = name;
    m->parent = 0;
    dict_put(registry_classes, m, m);
    
    schedule_for_resolve((slotinfo_t*)m);
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

memberinfo_t* registry_findmember(classinfo_t*cls, const char*ns, const char*name, char recursive, char is_static)
{
    memberinfo_t tmp;
    tmp.name = name;
    tmp.package = ns?ns:"";

    if(!recursive) {
	if(!is_static) 
	    return (memberinfo_t*)dict_lookup(&cls->members, &tmp);
	else
	    return (memberinfo_t*)dict_lookup(&cls->static_members, &tmp);
    }
    /* look at classes directly extended by this class */
    slotinfo_t*m = 0;
    classinfo_t*s = cls;

    if(recursive>1) // check *only* superclasses
        s = s->superclass;

    while(s) {
        if(s->kind == INFOTYPE_UNRESOLVED)
            break;

	if(!is_static) {
	    m = (slotinfo_t*)dict_lookup(&s->members, &tmp);
	    if(m) return (memberinfo_t*)m;
	}
	m = (slotinfo_t*)dict_lookup(&s->static_members, &tmp);
	if(m) return (memberinfo_t*)m;

        s = s->superclass;
    }
    /* look at interfaces, and parent interfaces */
    int t=0;
    while(cls->interfaces[t]) {
        classinfo_t*s = cls->interfaces[t];
        if(s->kind != INFOTYPE_UNRESOLVED) {
            while(s) {
		if(!is_static) {
		    m = (slotinfo_t*)dict_lookup(&s->members, &tmp);
		    if(m) return (memberinfo_t*)m;
		}
		m = (slotinfo_t*)dict_lookup(&s->static_members, &tmp);
		if(m) return (memberinfo_t*)m;

                s = s->superclass;
            }
        }
        t++;
    }
    return 0;
}

memberinfo_t* registry_findmember_nsset(classinfo_t*cls, namespace_list_t*ns, const char*name, char superclasses, char is_static)
{
    memberinfo_t*m = 0;
    while(ns) {
        m = registry_findmember(cls, ns->namespace->name, name, superclasses, is_static);
        if(m) return m;
        ns = ns->next;
    }
    m = registry_findmember(cls, "", name, superclasses, is_static);
    if(m) return m;
    /* TODO: it maybe would be faster to just store the builtin namespace as "" in
             builtins.c (update: some members (e.g. XML.length) are present both for
            "" and "http:...builtin") */
    m = registry_findmember(cls, "http://adobe.com/AS3/2006/builtin", name, superclasses, is_static);
    if(m) return m;
    return 0;
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
    if(f->kind == INFOTYPE_METHOD) {
        c->name = "Function";
        c->superclass = registry_getobjectclass();
    } else if(f->kind == INFOTYPE_CLASS) {
        c->name = "Class";
        c->superclass = registry_getobjectclass();
    } else if(f->kind == INFOTYPE_VAR) {
        c->name = "Object";
    } else {
        c->name = "undefined";
    }
    
    dict_init2(&c->members, &memberinfo_type, 1);
    dict_init2(&c->static_members, &memberinfo_type, 1);
    c->data = f;
    dict_put(functionobjects, f, c);
    return c;
}

classinfo_t* slotinfo_gettype(slotinfo_t*f)
{
    if(f) {
       if(f->kind == INFOTYPE_METHOD) {
           return slotinfo_asclass(f);
       } else if(f->kind == INFOTYPE_VAR) {
           varinfo_t*v = (varinfo_t*)f;
           return v->type;
       } else 
           return 0;
    } else {
       return TYPE_ANY;
    }
}

// ----------------------- package handling ---------------------------
char registry_ispackage(const char*package)
{
    /* crude approximation of "the real thing", but sufficient for now */
    return !strncmp(package, "flash", 5);
}
// ----------------------- builtin types ------------------------------

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
classinfo_t* registry_getdateclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "Date");
    return c;
}
classinfo_t* registry_getxmlclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "XML");
    return c;
}
classinfo_t* registry_getxmllistclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "XMLList");
    return c;
}
classinfo_t* registry_getnamespaceclass() {
    static classinfo_t*c = 0;
    if(!c) c = (classinfo_t*)registry_safefind("", "Namespace");
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
classinfo_t voidclass = {
    INFOTYPE_CLASS,0,0,ACCESS_PACKAGE, "", "void", 0, 0, 0
};
classinfo_t* registry_getvoidclass() {
    return &voidclass;
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
    if(!s) 
	return "(unknown)";
    if(s->kind == INFOTYPE_CLASS) return "class";
    else if(s->kind == INFOTYPE_VAR) return "var";
    else if(s->kind == INFOTYPE_METHOD) return "function";
    else return "object";
}

void slotinfo_dump(slotinfo_t*s)
{
    if(s->package[0]) {
        printf("%s %s.%s", infotypename(s), s->package, s->name);
    } else {
        printf("%s %s", infotypename(s), s->name);
    }
    if(s->kind == INFOTYPE_CLASS) {
        classinfo_t*c = (classinfo_t*)s;
    }
    else if(s->kind == INFOTYPE_VAR) {
        varinfo_t*v = (varinfo_t*)s;
        printf(":%s", v->type?v->type->name:"*");
        if(v->value)
            printf("=%s", constant_tostring(v->value));
        if(v->slot)
            printf(" (slot:%d)", v->slot);
    }
    else if(s->kind == INFOTYPE_METHOD) {
        methodinfo_t*m = (methodinfo_t*)s;
    }
    else {
    }
    printf("\n");
}

