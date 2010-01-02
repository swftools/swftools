/* abc.h

   Routines for handling Flash2 AVM2 ABC Actionscript (header file)

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

#ifndef __swfabc_h__
#define __swfabc_h__

#include "../q.h"
#include "pool.h"

DECLARE(abc_file);
DECLARE(abc_method);
DECLARE(abc_method_body);
DECLARE(abc_interface);
DECLARE(abc_class);
DECLARE(abc_exception);
DECLARE(abc_asset);
DECLARE(asset_dependency);
DECLARE(asset_tag);
DECLARE_LIST(abc_asset);
DECLARE_LIST(asset_dependency);
DECLARE_LIST(asset_tag);
DECLARE_LIST(abc_exception);
DECLARE_LIST(TAG);

#include "code.h"
#include "opcodes.h"

DECLARE(trait);


#define METHOD_NEED_ARGUMENTS 1
#define METHOD_NEED_ACTIVATION 2
#define METHOD_NEED_REST 4
#define METHOD_HAS_OPTIONAL 8
#define METHOD_SET_DXNS 0x40
#define METHOD_HAS_PARAM_NAMES 0x80

struct _abc_method {
    multiname_t*return_type;
    multiname_list_t*parameters;
    constant_list_t*optional_parameters;
    const char*name;
    U8 flags;
    abc_method_body_t*body;

    trait_t*trait;
    
    int index; //filled in during writing
};

#define ABCFILE_LAZY 1
struct _abc_file {
    // abc_file

    const char*name;
    U32 flags;
    array_t*metadata;
    array_t*methods;
    array_t*classes;
    array_t*scripts;
    array_t*method_bodies;
};

abc_file_t*abc_file_new();

#define TRAIT_SLOT 0
#define TRAIT_METHOD 1
#define TRAIT_GETTER 2
#define TRAIT_SETTER 3
#define TRAIT_CLASS 4
#define TRAIT_FUNCTION 5
#define TRAIT_CONST 6
            
#define TRAIT_ATTR_FINAL 0x10
#define TRAIT_ATTR_OVERRIDE 0x20
#define TRAIT_ATTR_METADATA 0x40


struct _trait {
    unsigned char kind;
    unsigned char attributes;
    multiname_t*name;

    union {
        int disp_id;
        int slot_id;
        int data1;
    };
    union {
        abc_method_t*method;
        abc_class_t*cls;
        multiname_t*type_name;
        int data2;
    };
    constant_t*value;
};

trait_t*trait_new_method(trait_list_t**traits, multiname_t*name, abc_method_t*m);
trait_t*trait_new_member(trait_list_t**traits, multiname_t*type, multiname_t*name, constant_t*v);

#define CLASS_SEALED 1
#define CLASS_FINAL 2
#define CLASS_INTERFACE 4
#define CLASS_PROTECTED_NS 8

struct _abc_class {
    abc_file_t*file;
    
    multiname_t*classname;
    multiname_t*superclass;
    namespace_t*protectedNS;

    multiname_list_t*interfaces;

    abc_method_t*constructor;
    trait_list_t*traits;
    
    abc_method_t*static_constructor;
    trait_list_t*static_traits;
    
    U8 flags;

    abc_asset_t*asset; // swf tags needed for this class

    int init_scope_depth; // volatile, might be increased during code verification
    int index; //filled in during writing
};

void abc_method_init(abc_method_t*m, abc_file_t*file, multiname_t*returntype, char body);
abc_method_t* abc_method_new(abc_file_t*file, multiname_t*returntype, char body);

abc_class_t* abc_class_new(abc_file_t*file, multiname_t*classname, multiname_t*superclass);
abc_class_t* abc_class_new2(abc_file_t*file, char*classname, char*superclass);
void abc_class_sealed(abc_class_t*c);
void abc_class_final(abc_class_t*c);
void abc_class_interface(abc_class_t*c);
void abc_class_protectedNS(abc_class_t*c, char*namespace);
void abc_class_add_interface(abc_class_t*c, multiname_t*interface);
char*abc_class_fullname(abc_class_t*cls);

trait_t* traits_find_slotid(trait_list_t*traits, int slotid);

abc_method_t* abc_class_getconstructor(abc_class_t*cls, multiname_t*returntype);
abc_method_t* abc_class_getstaticconstructor(abc_class_t*cls, multiname_t*returntype);

abc_method_t* abc_class_method(abc_class_t*cls, multiname_t*returntype, multiname_t*name);
abc_method_t* abc_class_staticmethod(abc_class_t*cls, multiname_t*returntype, multiname_t*name);
trait_t*      abc_class_slot(abc_class_t*cls, multiname_t*name, multiname_t*type);
trait_t*      abc_class_staticslot(abc_class_t*cls, multiname_t*name, multiname_t*type);

struct _abc_exception {
    code_t*from;
    code_t*to;
    code_t*target;
    multiname_t*exc_type;
    multiname_t*var_name;
};

struct _abc_method_body {
    abc_file_t*file;
    //abc_class_t*cls;
    abc_method_t*method;
    code_t*code;

    struct {
        //for dumping: filled in during parsing
        int max_stack;
        int local_count;
        int max_scope_depth;
        int init_scope_depth;
    } old;

    int init_scope_depth; // volatile, might be increased during code verification

    abc_exception_list_t* exceptions;

    trait_list_t*traits;
    
    int index; // filled in during writing
    codestats_t*stats; //filled in during writing
};

typedef struct _abc_script {
    abc_method_t*method;
    abc_file_t*file;
    trait_list_t*traits;
} abc_script_t;

struct _asset_dependency {
    abc_asset_t*asset;
    int patch_pos;
};
struct _asset_tag {
    TAG*tag;
    asset_dependency_t*deps;
    int num_deps;
};
struct _abc_asset {
    asset_tag_list_t*tags;
    U16 id;
};

abc_method_t* abc_nullmethod(abc_file_t*file);
abc_script_t* abc_initscript(abc_file_t*file);
trait_t*abc_initscript_addClassTrait(abc_script_t*script, multiname_t*multiname, abc_class_t*cls);

#define __ 

#endif
