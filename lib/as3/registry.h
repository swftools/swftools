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

#include "abc.h"

DECLARE(asset_bundle);
DECLARE_LIST(asset_bundle);
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
#define FLAG_USED 64

/* member flags */
#define FLAG_STATIC 2
#define FLAG_OVERRIDE 8
#define FLAG_NATIVE 16
#define FLAG_CONST 32

/* class flags */
#define FLAG_DYNAMIC 8
#define FLAG_INTERFACE 16

#define INFOTYPE_VAR 1
#define INFOTYPE_METHOD 2
#define INFOTYPE_CLASS 3
#define INFOTYPE_UNRESOLVED 4
#define SUBTYPE_GET 1
#define SUBTYPE_SET 2
#define SUBTYPE_GETSET 3

#define SLOTINFO_HEAD \
    U8 kind,subtype,flags,access; \
    const char*package; \
    const char*name; \
    int slot;

struct _slotinfo {
    SLOTINFO_HEAD;
};
struct _unresolvedinfo {
    SLOTINFO_HEAD;
    namespace_list_t*nsset;
};
struct _classinfo {
    SLOTINFO_HEAD;
    classinfo_t*superclass;
    dict_t members;
    dict_t static_members;
    void*data; //TODO: get rid of this- parser.y should pass type/value/code triples around
    asset_bundle_t*assets;
    classinfo_t*interfaces[];
};
struct _memberinfo {
    SLOTINFO_HEAD;
    union {
        classinfo_t*return_type;
        classinfo_t*type;
    };
    classinfo_t*parent;
};
struct _methodinfo /*extends memberinfo*/ {
    SLOTINFO_HEAD;
    classinfo_t*return_type;
    classinfo_t*parent;
    classinfo_list_t*params;
};
struct _varinfo /*extends memberinfo*/ {
    SLOTINFO_HEAD;
    classinfo_t*type;
    classinfo_t*parent;
    constant_t*value;
};

struct _asset_bundle {
    abc_file_t*file;
    asset_bundle_list_t*dependencies;
    char used;
};

extern type_t memberinfo_type;
extern type_t slotinfo_type;
char slotinfo_equals(slotinfo_t*c1, slotinfo_t*c2);

void registry_init();
        
classinfo_t* classinfo_register(int access, const char*package, const char*name, int num_interfaces);
methodinfo_t* methodinfo_register_onclass(classinfo_t*cls, U8 access, const char*ns, const char*name, char is_static);
methodinfo_t* methodinfo_register_global(U8 access, const char*package, const char*name);
varinfo_t* varinfo_register_onclass(classinfo_t*cls, U8 access,  const char*ns, const char*name, char is_static);
varinfo_t* varinfo_register_global(U8 access, const char*package, const char*name);

slotinfo_t* registry_resolve(slotinfo_t*s);
void registry_resolve_all();

slotinfo_t* registry_find(const char*package, const char*name);
void registry_dump();
memberinfo_t* registry_findmember(classinfo_t*cls, const char*ns, const char*name, char superclasses, char is_static);
memberinfo_t* registry_findmember_nsset(classinfo_t*cls, namespace_list_t*ns, const char*name, char superclasses, char is_static);

void registry_fill_multiname(multiname_t*m, namespace_t*n, slotinfo_t*c);
#define MULTINAME(m,x) \
    multiname_t m;\
    namespace_t m##_ns;\
    (x)->package; \
    registry_fill_multiname(&m, &m##_ns, (slotinfo_t*)(x));
                    
multiname_t* classinfo_to_multiname(slotinfo_t*cls);

char registry_isfunctionclass();
char registry_isclassclass();

classinfo_t* slotinfo_asclass(slotinfo_t*f);
classinfo_t* slotinfo_gettype(slotinfo_t*);

namespace_t access2namespace(U8 access, char*package);

char registry_ispackage(const char*package);

void registry_add_asset(asset_bundle_t*bundle);
void registry_use(slotinfo_t*s);
asset_bundle_list_t*registry_getassets();

// static multinames
classinfo_t voidclass;
classinfo_t* registry_getanytype();
classinfo_t* registry_getarrayclass();
classinfo_t* registry_getobjectclass();
classinfo_t* registry_getnumberclass();
classinfo_t* registry_getstringclass();
classinfo_t* registry_getdateclass();
classinfo_t* registry_getintclass();
classinfo_t* registry_getuintclass();
classinfo_t* registry_getnullclass();
classinfo_t* registry_getvoidclass();
classinfo_t* registry_getarrayclass();
classinfo_t* registry_getregexpclass();
classinfo_t* registry_getxmlclass();
classinfo_t* registry_getxmllistclass();
classinfo_t* registry_getbooleanclass();
classinfo_t* registry_getMovieClip();
classinfo_t* registry_getclassclass(classinfo_t*a);
classinfo_t* registry_getnamespaceclass();
extern classinfo_t voidclass;

char* infotypename(slotinfo_t*s);
void slotinfo_dump(slotinfo_t*s);

/* convenience functions */
#define sig2mname(x) (x->superclass,classinfo_to_multiname((slotinfo_t*)(x)))

#define TYPE_ANY             (0)
#define TYPE_IS_ANY(t)       ((t) == 0)
#define TYPE_INT                     registry_getintclass()
#define TYPE_IS_INT(t)       ((t) == registry_getintclass())
#define TYPE_UINT                    registry_getuintclass()
#define TYPE_IS_UINT(t)      ((t) == registry_getuintclass())
#define TYPE_NUMBER                  registry_getnumberclass()
#define TYPE_IS_NUMBER(t)    ((t) == registry_getnumberclass())
#define TYPE_FLOAT                   registry_getnumberclass()
#define TYPE_IS_FLOAT(t)     ((t) == registry_getnumberclass())
#define TYPE_BOOLEAN                 registry_getbooleanclass()
#define TYPE_IS_BOOLEAN(t)   ((t) == registry_getbooleanclass())
#define TYPE_STRING                  registry_getstringclass()
#define TYPE_IS_STRING(t)    ((t) == registry_getstringclass())
#define TYPE_DATE                    registry_getdateclass()
#define TYPE_IS_DATE(t)      ((t) == registry_getdateclass())
#define TYPE_OBJECT                  registry_getobjectclass()
#define TYPE_IS_OBJECT(t)    ((t) == registry_getobjectclass())
#define TYPE_REGEXP                  registry_getregexpclass()
#define TYPE_IS_REGEXP(t)    ((t) == registry_getregexpclass())
#define TYPE_VOID                    registry_getvoidclass()
#define TYPE_IS_VOID(t)      ((t) == registry_getvoidclass())
#define TYPE_ARRAY                   registry_getarrayclass()
#define TYPE_IS_ARRAY(t)     ((t) == registry_getarrayclass())
#define TYPE_NAMESPACE            registry_getnamespaceclass()
#define TYPE_IS_NAMESPACE(t) ((t) == registry_getnamespaceclass())
#define TYPE_FUNCTION(f)     ((f)->return_type,slotinfo_asclass((slotinfo_t*)(f)))
#define TYPE_IS_FUNCTION(t)  registry_isfunctionclass(t)
#define TYPE_CLASS(f)        ((f)->superclass,slotinfo_asclass((slotinfo_t*)(f)))
#define TYPE_IS_CLASS(t)     registry_isclassclass(t)
#define TYPE_NULL            registry_getnullclass()
#define TYPE_IS_NULL(t)      ((t) == registry_getnullclass())
#define TYPE_VOID            registry_getvoidclass()
#define TYPE_IS_VOID(t)      ((t) == registry_getvoidclass())
#define TYPE_XML            registry_getxmlclass()
#define TYPE_IS_XML(t)      ((t) == registry_getxmlclass())
#define TYPE_XMLLIST        registry_getxmllistclass()
#define TYPE_IS_XMLLIST(t)  ((t) == registry_getxmllistclass())
        
#define TYPE_IS_BUILTIN_SIMPLE(type) (TYPE_IS_INT(type) || \
                                      TYPE_IS_UINT(type) || \
                                      TYPE_IS_FLOAT(type) || \
                                      TYPE_IS_BOOLEAN(type) || \
                                      TYPE_IS_STRING(type))

#define IS_NUMBER_OR_INT(a) (TYPE_IS_INT((a)) || TYPE_IS_UINT((a)) || TYPE_IS_NUMBER((a)))


#endif
