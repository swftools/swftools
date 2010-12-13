#ifndef __parse_help__
#define  __parse_help__

#include "state.h"
#include "expr.h"
#include "tokenizer.h"
#include "common.h"

typedef struct _global {
    abc_file_t*file;

    parsedclass_list_t*classes;
    abc_script_t*classinit;

    abc_script_t*init; //package-level code

    dict_t*token2info;
    dict_t*file2token2info;
} global_t;

extern global_t*global;

typedef struct _variable {
    int index;
    classinfo_t*type;
    char init;
    char kill;
    char is_parameter;
    methodstate_t*is_inner_method;
} variable_t;

void initialize_file(char*filename);
void finish_file();

void initialize_parser();
void* finish_parser();

#define FLAG_PUBLIC 256
#define FLAG_PROTECTED 512
#define FLAG_PRIVATE 1024
#define FLAG_PACKAGEINTERNAL 2048
#define FLAG_NAMESPACE 4096

slotinfo_t* find_class(const char*name);
char is_break_or_jump(code_t*c);
code_t* insert_finally(code_t*c, code_t*finally, int tempvar);
typedcode_t push_class(slotinfo_t*a);
slotinfo_t* find_class(const char*name);
namespace_list_t*get_current_imports();
const char* get_package_from_name(const char*name);
char is_pushundefined(code_t*c);
code_t* coerce_to_type(code_t*c, classinfo_t*t);
code_t*converttype(code_t*c, classinfo_t*from, classinfo_t*to);
void continuejumpsto(code_t*c, char*name, code_t*jump);
void breakjumpsto(code_t*c, char*name, code_t*jump);
void insert_unresolved(methodstate_t*m, dict_t*xvars, dict_t*allvars);
void startfunction(modifiers_t*mod, enum yytokentype getset, char*name, params_t*params, classinfo_t*return_type);
void function_initvars(methodstate_t*m, char has_params, params_t*params, int flags, char var0);
abc_method_t* endfunction(modifiers_t*mod, enum yytokentype getset, char*name, params_t*params, classinfo_t*return_type, code_t*body);
void innerfunction(char*name, params_t*params, classinfo_t*return_type);
methodinfo_t*registerfunction(enum yytokentype getset, modifiers_t*mod, char*name, params_t*params, classinfo_t*return_type, int slot);
void check_override(memberinfo_t*m, int flags);
void check_constant_against_type(classinfo_t*t, constant_t*c);
void check_code_for_break(code_t*c);
void startclass(modifiers_t* mod, char*classname, classinfo_t*extends, classinfo_list_t*implements);
void endclass();
void innerfunctions2vars(methodstate_t*m);
memberinfo_t* findmember_nsset(classinfo_t*cls, const char*name, char recurse, char is_static);
namespace_t modifiers2access(modifiers_t*mod);
const char* lookup_namespace(const char*name);
void startpackage(char*name);
void endpackage();
code_t* wrap_function(code_t*c,code_t*header, code_t*body);
code_t* method_header(methodstate_t*m);
code_t* add_scope_code(code_t*c, methodstate_t*m, char init);
void unknown_variable(char*name);
code_t* var_block(code_t*body, dict_t*vars);
int gettempvar();
int new_variable(methodstate_t*method, const char*name, classinfo_t*type, char init, char maybeslot);
variable_t* new_variable2(methodstate_t*method, const char*name, classinfo_t*type, char init, char maybeslot);
int alloc_local();
code_t*defaultvalue(code_t*c, classinfo_t*type);
char variable_exists(char*name);
variable_t* find_variable_safe(state_t*s, char*name);
variable_t* find_variable(state_t*s, const char*name);
variable_t* find_slot(methodstate_t*m, const char*name);
code_t* method_header(methodstate_t*m);
code_t* wrap_function(code_t*c,code_t*header, code_t*body);
void function_initvars(methodstate_t*m, char has_params, params_t*params, int flags, char var0);
void state_destroy(state_t*state);
void as3_set_define(const char*c);

/* protected handling here is a big hack: we just assume the protectedns
   is package:class. the correct approach would be to add the proper
   namespace to all protected members in the registry, even though that
   would slow down searching */
#define MEMBER_MULTINAME(m,f,n) \
    multiname_t m;\
    namespace_t m##_ns;\
    if(f) { \
        m##_ns.access = ((slotinfo_t*)(f))->access; \
        if(m##_ns.access == ACCESS_NAMESPACE) \
            m##_ns.name = ((slotinfo_t*)(f))->package; \
        else if(m##_ns.access == ACCESS_PROTECTED && (f)->parent) \
            m##_ns.name = concat3((f)->parent->package,":",(f)->parent->name); \
        else \
            m##_ns.name = ""; \
        m.type = QNAME; \
        m.ns = &m##_ns; \
        m.namespace_set = 0; \
        m.name = ((slotinfo_t*)(f))->name; \
    } else { \
        m.type = MULTINAME; \
        m.ns =0; \
        m.namespace_set = &nopackage_namespace_set; \
        m.name = n; \
    }

/* warning: list length of namespace set is undefined */
#define MULTINAME_LATE(m, access, package) \
    namespace_t m##_ns = {access, package}; \
    namespace_set_t m##_nsset; \
    namespace_list_t m##_l;m##_l.next = 0; \
    m##_nsset.namespaces = &m##_l; \
    m##_nsset = m##_nsset; \
    m##_l.namespace = &m##_ns; \
    multiname_t m = {MULTINAMEL, 0, &m##_nsset, 0};

extern namespace_t stdns;
extern namespace_set_t nopackage_namespace_set;

#endif
