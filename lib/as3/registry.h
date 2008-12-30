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

DECLARE(classinfo);
DECLARE_LIST(classinfo);
DECLARE(memberinfo);

struct _classinfo {
    U8 access;
    U8 flags;
    const char*package;
    const char*name;
    union {
        int slot;        // slot nr in initscript traits
        classinfo_t*cls; // specific class of a Class type
        memberinfo_t*function; //specific function of a Function type
    };

    classinfo_t*superclass;
    dict_t members;
    classinfo_t*interfaces[];
};
char classinfo_equals(classinfo_t*c1, classinfo_t*c2);

#define MEMBER_SLOT 1
#define MEMBER_GET 2
#define MEMBER_SET 4
#define MEMBER_GETSET 6
#define MEMBER_METHOD 8

struct _memberinfo {
    U8 kind;
    U8 flags;
    const char*name;
    union {
        classinfo_t*return_type;
        classinfo_t*type;
    };
    classinfo_list_t*params;
    int slot;
};

extern type_t classinfo_type;
extern type_t function_signature_type;

void registry_init();
        
classinfo_t* classinfo_register(int access, char*package, char*name, int num_interfaces);
memberinfo_t* memberinfo_register(classinfo_t*cls, const char*name, U8 type);

// static multinames
classinfo_t* registry_getanytype();
classinfo_t* registry_getobjectclass();
classinfo_t* registry_getnumberclass();
classinfo_t* registry_getstringclass();
classinfo_t* registry_getintclass();
classinfo_t* registry_getuintclass();
classinfo_t* registry_getnullclass();
classinfo_t* registry_getbooleanclass();
classinfo_t* registry_getfunctionclass();
classinfo_t* registry_getMovieClip();
classinfo_t* registry_getfunctionclass(memberinfo_t*f);
classinfo_t* registry_getclassclass(classinfo_t*a);

classinfo_t* registry_findclass(const char*package, const char*name);
memberinfo_t* registry_findmember(classinfo_t*cls, const char*name);

void registry_fill_multiname(multiname_t*m, namespace_t*n, classinfo_t*c);
multiname_t* classinfo_to_multiname(classinfo_t*cls);

char registry_isfunctionclass();
char registry_isclassclass();

/* convenience functions */
#define sig2mname(x) classinfo_to_multiname(x)
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

#define TYPE_FUNCTION(f)          registry_getfunctionclass(f)
#define TYPE_IS_FUNCTION(t)       registry_isfunctionclass(t)

#define TYPE_CLASS(f)             registry_getclassclass(f)
#define TYPE_IS_CLASS(t)          registry_isclassclass(t)

#define TYPE_NULL                 registry_getnullclass()
#define TYPE_IS_NULL(t)   ((t) == registry_getnullclass())
        
#define TYPE_IS_BUILTIN_SIMPLE(type) (TYPE_IS_INT(type) || \
                                      TYPE_IS_UINT(type) || \
                                      TYPE_IS_FLOAT(type) || \
                                      TYPE_IS_BOOLEAN(type) || \
                                      TYPE_IS_STRING(type))


#endif
