/* pool.h

   Routines for handling Flash2 AVM2 ABC contantpool entries.

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

#ifndef __pool_h__
#define __pool_h__

#include "../q.h"
#include "../rfxswf.h"

DECLARE(pool);
DECLARE(constant);
DECLARE_LIST(constant);
DECLARE(multiname);
DECLARE(namespace);
DECLARE(namespace_set);
DECLARE_LIST(multiname);
DECLARE_LIST(namespace);
DECLARE_LIST(trait);

/* abc file constant pool */
struct _pool {
    array_t*x_ints;
    array_t*x_uints;
    array_t*x_floats;
    array_t*x_strings;
    array_t*x_namespaces;
    array_t*x_namespace_sets;
    array_t*x_multinames;
};

typedef enum access_type
{ACCESS_STANDARD=0x16,
 ACCESS_PACKAGE=0x16,
 ACCESS_PUBLIC=0x16,
 ACCESS_PACKAGEINTERNAL=0x17,
 ACCESS_PROTECTED=0x18,
 ACCESS_EXPLICIT=0x19,
 ACCESS_STATICPROTECTED=0x1a,
 ACCESS_PRIVATE=0x05,
 ACCESS_NAMESPACE=0x08
} access_type_t;


typedef enum constant_type
{
CONSTANT_STRING=0x01,
CONSTANT_INT=0x03,
CONSTANT_UINT=0x04,
CONSTANT_FLOAT=0x06,
 
CONSTANT_TRUE=0x0B,
CONSTANT_FALSE=0x0A,
CONSTANT_NULL=0x0C,
CONSTANT_UNDEFINED=0x00,
 
CONSTANT_NAMESPACE=0x08,
CONSTANT_NAMESPACE_PACKAGE=0x16,
CONSTANT_NAMESPACE_PACKAGEINTERNAL=0x17,
CONSTANT_NAMESPACE_PROTECTED=0x18,
CONSTANT_NAMESPACE_EXPLICIT=0x19,
CONSTANT_NAMESPACE_STATICPROTECTED=0x1A,
CONSTANT_NAMESPACE_PRIVATE=0x05,
CONSTANT_UNKNOWN=0x55, /*used internally */
} contant_type_t;

#define NS_TYPE(x) ((x) == 0x08 || (x) == 0x16 || (x) == 0x17 || (x) == 0x18 || (x) == 0x19 || (x) == 0x1a || (x) == 0x05)

struct _constant {
    int type;
    union {
        string_t* s;
        namespace_t*ns;
        double f;
        int i;
        unsigned int u;
    };
};

constant_t* constant_clone();
constant_t* constant_new_int(int x);
constant_t* constant_new_uint(unsigned int x);
constant_t* constant_new_float(double x);
constant_t* constant_new_string(const char*s);
constant_t* constant_new_string2(const char*s, int len);
constant_t* constant_new_namespace(namespace_t*ns);
constant_t* constant_new_true();
constant_t* constant_new_false();
constant_t* constant_new_null();
constant_t* constant_new_undefined();
char constant_has_index(constant_t*c);
constant_t* constant_fromindex(pool_t*pool, int index, int type);
char* constant_tostring(constant_t*c);
int constant_get_index(pool_t*pool, constant_t*c);
void constant_free(constant_t*c);

typedef enum multiname_type
{QNAME=0x07,
 RTQNAME=0x0F,
 RTQNAMEL=0x11,
 MULTINAME=0x09,
 MULTINAMEL=0x1B,
 QNAMEA=0x0D,
 RTQNAMEA=0x10,
 RTQNAMELA=0x12,
 MULTINAMEA=0x0E,
 MULTINAMELA=0x1C,
 POSTFIXTYPE=0x1D,
 ZERONAMESPACE=0
} multiname_type_t;

char* access2str(int type);

struct _namespace {
    U8 access;
    const char*name;
};
struct _namespace_set {
    namespace_list_t*namespaces;
};

extern type_t multiname_type;
struct _multiname {
    multiname_type_t type;
    namespace_t*ns;
    namespace_set_t*namespace_set;
    const char*name;
};

/* object -> string */
char* namespace_set_tostring(namespace_set_t*set);
char* multiname_tostring(multiname_t*m);
char* namespace_tostring(namespace_t*ns);

/* integer -> object */
int pool_lookup_int(pool_t*pool, int i);
unsigned int pool_lookup_uint(pool_t*pool, int i);
double pool_lookup_float(pool_t*pool, int i);
const char*pool_lookup_string(pool_t*pool, int i);
string_t pool_lookup_string2(pool_t*pool, int i);
namespace_t*pool_lookup_namespace(pool_t*pool, int i);
namespace_set_t*pool_lookup_namespace_set(pool_t*pool, int i);
multiname_t*pool_lookup_multiname(pool_t*pool, int i);

/* object -> integer (lookup) */
int pool_find_int(pool_t*pool, int x);
int pool_find_uint(pool_t*pool, unsigned int x);
int pool_find_double(pool_t*pool, double x);
int pool_find_namespace(pool_t*pool, namespace_t*ns);
int pool_find_namespace_set(pool_t*pool, namespace_set_t*set);
int pool_find_string(pool_t*pool, const char*s);
int pool_find_multiname(pool_t*pool, multiname_t*name);

/* object -> integer (lookup/creation) */
int pool_register_int(pool_t*p, int i);
int pool_register_uint(pool_t*p, unsigned int i);
int pool_register_float(pool_t*p, double d);
int pool_register_string(pool_t*pool, const char*s);
int pool_register_string2(pool_t*pool, string_t*s);
int pool_register_namespace(pool_t*pool, namespace_t*ns);
int pool_register_namespace_set(pool_t*pool, namespace_set_t*set);
int pool_register_multiname(pool_t*pool, multiname_t*n);
int pool_register_multiname2(pool_t*pool, char*name);

/* creation */
namespace_t* namespace_new(U8 access, const char*name);
namespace_t* namespace_new_namespace(const char*name);
namespace_t* namespace_new_package(const char*name);
namespace_t* namespace_new_packageinternal(const char*name);
namespace_t* namespace_new_protected(const char*name);
namespace_t* namespace_new_explicit(const char*name);
namespace_t* namespace_new_staticprotected(const char*name);
namespace_t* namespace_new_private(const char*name);
namespace_t* namespace_clone(namespace_t*ns);

namespace_t* namespace_fromstring(const char*name);

multiname_t* multiname_fromstring(const char*name);
multiname_t* multiname_new(namespace_t*ns, const char*name);
multiname_t* multiname_clone(multiname_t*other);

/* accessors etc. */
char multiname_late_namespace(multiname_t*m);
char multiname_late_name(multiname_t*m);

/* freeing */
void multiname_destroy(multiname_t*m);
void namespace_destroy(namespace_t*n);

/* pool constructors/destructors */
pool_t*pool_new();
void pool_optimize();
void pool_read(pool_t*pool, TAG*tag);
void pool_write(pool_t*pool, TAG*tag);
void pool_destroy(pool_t*pool);
void pool_dump(pool_t*pool, FILE*fo, char flags);

#endif
