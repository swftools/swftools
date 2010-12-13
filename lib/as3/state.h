#ifndef __state_h__
#define __state_h__

#include "../q.h"
#include "abc.h"
#include "registry.h"
#include "initcode.h"

DECLARE(state);
DECLARE_LIST(state);
DECLARE(import);
DECLARE_LIST(import);
DECLARE(methodstate);
DECLARE_LIST(methodstate);
DECLARE(classstate);

struct _state {
    struct _state*old;
    int level;
    
    char*package;     
    import_list_t*wildcard_imports;
    dict_t*import_toplevel_packages;
    dict_t*imports;

    dict_t*namespaces;
    namespace_list_t*active_namespace_urls;
    
    char has_own_imports;
    char new_vars; // e.g. transition between two functions
    char xmlfilter; // are we inside a xmlobj..() filter?
  
    classstate_t*cls;   
    methodstate_t*method;

    char*exception_name;

    int switch_var;
    
    dict_t*vars;
};

extern state_t* state;

struct _import {
    char*package;
};

struct _classstate {
    /* class data */
    classinfo_t*info;
    abc_class_t*abc;

    methodstate_t*init;
    methodstate_t*static_init;
    //code_t*init;
    //code_t*static_init;
    parsedclass_t*dependencies;

    char has_constructor;
};

struct _methodstate {
    /* method data */
    methodinfo_t*info;
    char has_exceptions;
    char late_binding;
    char is_constructor;
    char has_super;
    char is_global;
    char is_static;
    int variable_count;

    dict_t*unresolved_variables;
    dict_t*allvars; // all variables (in all sublevels, but not for inner functions)

    char inner;
    char uses_parent_function;
    char no_variable_scoping;
    int uses_slots;
    dict_t*slots;
    int activation_var;

    int need_arguments;

    abc_method_t*abc;
    int var_index; // for inner methods
    int slot_index; // for inner methods
    char is_a_slot; // for inner methods

    code_t*header;

    code_t*scope_code;
    abc_exception_list_t*exceptions;
    
    methodstate_list_t*innerfunctions;
};

void new_state();
void old_state();

methodstate_t*methodstate_new();
void methodstate_destroy(methodstate_t*m);

extern dict_t*definitions;
void as3_set_define(const char*c);

#endif //__state_h__
