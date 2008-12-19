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

DECLARE(class_signature);
DECLARE_LIST(class_signature);
DECLARE(function_signature);

struct _class_signature {
    /* this is very similar to a QNAME */
    U8 access;
    const char*package;
    const char*name;

    class_signature_t*superclass;
    dict_t members;
    class_signature_t*interfaces[];
};
char class_signature_equals(class_signature_t*c1, class_signature_t*c2);
extern type_t class_signature_type;

struct _function_signature {
    const char*name;
    class_signature_list_t*params;
};
extern type_t function_signature_type;

void registry_init();
        
class_signature_t* class_signature_register(int access, char*package, char*name);

// static multinames
class_signature_t* registry_getanytype();
class_signature_t* registry_getobjectclass();
class_signature_t* registry_getnumberclass();
class_signature_t* registry_getstringclass();
class_signature_t* registry_getintclass();
class_signature_t* registry_getuintclass();
class_signature_t* registry_getnullclass();
class_signature_t* registry_getbooleanclass();
class_signature_t* registry_getMovieClip();

class_signature_t* registry_findclass(const char*package, const char*name);

void registry_fill_multiname(multiname_t*m, namespace_t*n, class_signature_t*c);
multiname_t* class_signature_to_multiname(class_signature_t*cls);
#define sig2mname(x) class_signature_to_multiname(x)

#endif
