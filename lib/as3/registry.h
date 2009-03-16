/* registry.h

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

#ifndef __abc_registry_h__
#define __abc_registry_h__

#include "pool.h"

DECLARE(slotinfo);
DECLARE(classinfo);
DECLARE(memberinfo);
DECLARE(methodinfo);
DECLARE(unresolvedinfo);
DECLARE(varinfo);
DECLARE_LIST(classinfo);
DECLARE_LIST(slotinfo);

/* member/class flags */
#define FLAG_FINAL 1
#define FLAG_BUILTIN 128

/* member flags */
#define FLAG_STATIC 2
#define FLAG_OVERRIDE 8
#define FLAG_NATIVE 16

/* class flags */
#define FLAG_DYNAMIC 8
#define FLAG_INTERFACE 16

#define INFOTYPE_SLOT 1
#define INFOTYPE_METHOD 2
#define INFOTYPE_CLASS 3
#define INFOTYPE_UNRESOLVED 4
#define SUBTYPE_GET 1
#define SUBTYPE_SET 2
#define SUBTYPE_GETSET 3

struct _slotinfo {
    U8 kind,subtype,flags,access;
    const char*package;
    const char*name;
    int slot;
};
struct _unresolvedinfo {
    U8 kind,subtype,flags,access;
    const char*package;
    const char*name;
    int slot;
    namespace_list_t*nsset;
};
struct _classinfo {
    U8 kind,subtype,flags,access;
    const char*package;
    const char*name;
    int slot;
    classinfo_t*superclass;
    dict_t members;
    void*data; //TODO: get rid of this- parser.y should pass type/value/code triples around
    classinfo_t*interfaces[0];
};
struct _memberinfo {
    U8 kind,subtype,flags,access;
    const char*package;
    const char*name;
    int slot;
    union {
        classinfo_t*return_type;
        classinfo_t*type;
    };
    classinfo_t*parent;
};
struct _methodinfo {
    U8 kind,subtype,flags,access;
    const char*package;
    const char*name;
    int slot;
    classinfo_t*return_type;
    classinfo_t*parent;
    classinfo_list_t*params;
};
struct _varinfo {
    U8 kind,subtype,flags,access;
    const char*package;
    const char*name;
    int slot;
    classinfo_t*type;
    classinfo_t*parent;
    constant_t*value;
};

extern type_t slotinfo_type;
char slotinfo_equals(slotinfo_t*c1, slotinfo_t*c2);

void registry_init();
        
classinfo_t* classinfo_register(int access, const char*package, const char*name, int num_interfaces);
methodinfo_t* methodinfo_register_onclass(classinfo_t*cls, U8 access, const char*ns, const char*name);
methodinfo_t* methodinfo_register_global(U8 access, const char*package, const char*name);
varinfo_t* varinfo_register_onclass(classinfo_t*cls, U8 access,  const char*ns, const char*name);
varinfo_t* varinfo_register_global(U8 access, const char*package, const char*name);

slotinfo_t* registry_resolve(slotinfo_t*s);
void registry_resolve_all();

slotinfo_t* registry_find(const char*package, const char*name);
void registry_dump();
memberinfo_t* registry_findmember(classinfo_t*cls, const char*ns, const char*name, char superclasses);
memberinfo_t* registry_findmember_nsset(classinfo_t*cls, namespace_list_t*ns, const char*name, char superclasses);

void registry_fill_multiname(multiname_t*m, namespace_t*n, slotinfo_t*c);
multiname_t* classinfo_to_multiname(slotinfo_t*cls);

char registry_isfunctionclass();
char registry_isclassclass();

classinfo_t* slotinfo_asclass(slotinfo_t*f);
classinfo_t* slotinfo_gettype(slotinfo_t*);

namespace_t access2namespace(U8 access, char*package);

// static multinames
classinfo_t* registry_getanytype();
classinfo_t* registry_getarrayclass();
classinfo_t* registry_getobjectclass();
classinfo_t* registry_getnumberclass();
classinfo_t* registry_getstringclass();
classinfo_t* registry_getintclass();
classinfo_t* registry_getuintclass();
classinfo_t* registry_getnullclass();
classinfo_t* registry_getregexpclass();
classinfo_t* registry_getbooleanclass();
classinfo_t* registry_getMovieClip();
classinfo_t* registry_getclassclass(classinfo_t*a);

char* infotypename(slotinfo_t*s);

/* convenience functions */
#define sig2mname(x) (x->superclass,classinfo_to_multiname((slotinfo_t*)(x)))
#define TYPE_ANY                  registry_getanytype()
#define TYPE_IS_ANY(t)    ((t) == registry_getanytype())
#define TYPE_INT                  registry_getintclass()
#define TYPE_IS_INT(t)    ((t) == registry_getintclass())
#define TYPE_UINT                 registry_getuintclass()
#define TYPE_IS_UINT(t)   ((t) == registry_getuintclass())
#define TYPE_NUMBER               registry_getnumberclass()
#define TYPE_IS_NUMBER(t) ((t) == registry_getnumberclass())
#define TYPE_FLOAT                registry_getnumberclass()
#define TYPE_IS_FLOAT(t)  ((t) == registry_getnumberclass())
#define TYPE_BOOLEAN              registry_getbooleanclass()
#define TYPE_IS_BOOLEAN(t)((t) == registry_getbooleanclass())
#define TYPE_STRING               registry_getstringclass()
#define TYPE_IS_STRING(t) ((t) == registry_getstringclass())
#define TYPE_REGEXP               registry_getregexpclass()
#define TYPE_IS_REGEXP(t) ((t) == registry_getregexpclass())

#define TYPE_OBJECT               registry_getobjectclass()

#define TYPE_FUNCTION(f)          ((f)->return_type,slotinfo_asclass((slotinfo_t*)(f)))
#define TYPE_IS_FUNCTION(t)       registry_isfunctionclass(t)

#define TYPE_CLASS(f)             ((f)->superclass,slotinfo_asclass((slotinfo_t*)(f)))
#define TYPE_IS_CLASS(t)          registry_isclassclass(t)

#define TYPE_NULL                 registry_getnullclass()
#define TYPE_IS_NULL(t)   ((t) == registry_getnullclass())
        
#define TYPE_IS_BUILTIN_SIMPLE(type) (TYPE_IS_INT(type) || \
                                      TYPE_IS_UINT(type) || \
                                      TYPE_IS_FLOAT(type) || \
                                      TYPE_IS_BOOLEAN(type) || \
                                      TYPE_IS_STRING(type))


#endif
