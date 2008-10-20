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

#include "utils.h"

DECLARE(abc_file);
DECLARE(abc_method_body);
DECLARE(abc_interface);
DECLARE(abc_multiname);
DECLARE(abc_namespace);

DECLARE_LIST(abc_multiname);

typedef struct _abc_method {
    abc_multiname_t*return_type;
    abc_multiname_list_t*parameters;
    const char*name;
    U8 flags;

    int index;
    int method_body_index;
} abc_method_t;

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

struct _abc_namespace {
    U8 access;
    char*name;
};

struct _abc_multiname {
    multiname_type_t type;
    abc_namespace_t*ns;
    const char*namespace_set_name; // FIXME should be a struct
    const char*name;
};

struct _abc_file {

    // contant_pool

    dict_t*ints;
    dict_t*uints;
    dict_t*floats;
    dict_t*strings;
    dict_t*namespaces;
    dict_t*namespace_sets;
    dict_t*sets;
    dict_t*multinames;

    // abc_file

    dict_t*methods;
    dict_t*classes;
    dict_t*scripts;
    dict_t*method_bodies;
};

abc_file_t*abc_file_new();

typedef struct _abc_trait {
    unsigned char type;
    int name_index;

    union {
        int disp_id;
        int slot_id;
        int data1;
    };
    union {
        int nr;
        int cls;
        int type_index;
        int data2;
    };
    int vindex;
    int vkind;
} abc_trait_t;

typedef struct _abc_class {
    int index;
    abc_file_t*pool;
    
    const char*classname;
    const char*superclass;
    const char*protectedNS;

    abc_multiname_list_t*interfaces;

    int iinit;
    U8 flags;
    
    int static_constructor_index;
    dict_t*static_constructor_traits;

    dict_t*traits;
} abc_class_t;

abc_class_t* abc_class_new(abc_file_t*pool, char*classname, char*superclass);
void abc_class_sealed(abc_class_t*c);
void abc_class_final(abc_class_t*c);
void abc_class_interface(abc_class_t*c);
void abc_class_protectedNS(abc_class_t*c, char*namespace);

abc_method_body_t* abc_class_staticconstructor(abc_class_t*cls, char*returntype, int num_params, ...);
abc_method_body_t* abc_class_constructor(abc_class_t*cls, char*returntype, int num_params, ...);
abc_method_body_t* abc_class_method(abc_class_t*cls, char*returntype, char*name, int num_params, ...);

struct _abc_method_body {
    int index;
    abc_file_t*pool;
    //abc_class_t*cls;
    abc_method_t*method;
    TAG*tag;
    int max_stack;
    int local_count;
    int init_scope_depth;
    int max_scope_depth;
    int exception_count;
    dict_t*traits;
};

typedef struct _abc_label {
} abc_label_t;

typedef struct _abc_script {
    abc_method_t*method;
    abc_file_t*pool;
    dict_t*traits;
} abc_script_t;

abc_script_t* abc_initscript(abc_file_t*pool, char*returntype, int num_params, ...);

#endif
