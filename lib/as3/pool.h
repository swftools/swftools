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

typedef enum multiname_type
{QNAME=0x07,
 QNAMEA=0x0D,
 RTQNAME=0x0F,
 RTQNAMEA=0x10,
 RTQNAMEL=0x11,
 RTQNAMELA=0x12,
 MULTINAME=0x09,
 MULTINAMEA=0x0E,
 MULTINAMEL=0x1B,
 MULTINAMELA=0x1C
} multiname_type_t;

char* access2str(int type);

struct _namespace {
    U8 access;
    char*name;
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
char* namespace_set_to_string(namespace_set_t*set);
char* multiname_to_string(multiname_t*m);
char* namespace_to_string(namespace_t*ns);

/* integer -> object */
int pool_lookup_int(pool_t*pool, int i);
unsigned int pool_lookup_uint(pool_t*pool, int i);
double pool_lookup_float(pool_t*pool, int i);
char*pool_lookup_string(pool_t*pool, int i);
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
int pool_register_string(pool_t*pool, const char*s);
int pool_register_namespace(pool_t*pool, namespace_t*ns);
int pool_register_namespace_set(pool_t*pool, namespace_set_t*set);
int pool_register_multiname(pool_t*pool, multiname_t*n);
int pool_register_multiname2(pool_t*pool, char*name);

/* creation */
namespace_t* namespace_new(U8 access, const char*name);
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
void pool_read(pool_t*pool, TAG*tag);
void pool_write(pool_t*pool, TAG*tag);
void pool_destroy(pool_t*pool);

#endif
