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
DECLARE(abc_method_body);
DECLARE(abc_interface);
DECLARE(abc_code);
DECLARE(abc_class);

DECLARE(trait);

typedef struct _abc_method {
    multiname_t*return_type;
    multiname_list_t*parameters;
    const char*name;
    U8 flags;
    abc_method_body_t*body;
    
    int index; //filled in during writing
} abc_method_t;

struct _abc_file {
    // abc_file

    const char*name;
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
    int vindex;
    int vkind;
};

struct _abc_class {
    abc_file_t*pool;
    
    multiname_t*classname;
    multiname_t*superclass;
    namespace_t*protectedNS;

    multiname_list_t*interfaces;

    abc_method_t*constructor;
    U8 flags;
    
    abc_method_t*static_constructor;

    trait_list_t*static_constructor_traits;
    trait_list_t*traits;
    
    int index; //filled in during writing
};

abc_class_t* abc_class_new(abc_file_t*pool, multiname_t*classname, multiname_t*superclass);
abc_class_t* abc_class_new2(abc_file_t*pool, char*classname, char*superclass);
void abc_class_sealed(abc_class_t*c);
void abc_class_final(abc_class_t*c);
void abc_class_interface(abc_class_t*c);
void abc_class_protectedNS(abc_class_t*c, char*namespace);
void abc_class_add_interface(abc_class_t*c, multiname_t*interface);

abc_method_body_t* abc_class_staticconstructor(abc_class_t*cls, char*returntype, int num_params, ...);
abc_method_body_t* abc_class_constructor(abc_class_t*cls, char*returntype, int num_params, ...);
abc_method_body_t* abc_class_method(abc_class_t*cls, char*returntype, char*name, int num_params, ...);

struct _abc_code {
    U8 opcode;
    U8 len;
    void*params[2];
    abc_code_t*next;
    abc_code_t*prev;
    abc_code_t*parent;
};

struct _abc_method_body {
    abc_file_t*pool;
    //abc_class_t*cls;
    abc_method_t*method;
    abc_code_t*code;
    int max_stack;
    int local_count;
    int init_scope_depth;
    int max_scope_depth;
    int exception_count;
    trait_list_t*traits;
    
    int index; // filled in during writing
};

typedef struct _abc_label {
} abc_label_t;

typedef struct _abc_script {
    abc_method_t*method;
    abc_file_t*pool;
    trait_list_t*traits;
} abc_script_t;

abc_script_t* abc_initscript(abc_file_t*pool, char*returntype, int num_params, ...);

#define __ 

#endif
