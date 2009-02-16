/* parser.lex

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
%{
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "abc.h"
#include "pool.h"
#include "files.h"
#include "tokenizer.h"
#include "registry.h"
#include "code.h"
#include "opcodes.h"
#include "compiler.h"

extern int a3_lex();

%}

//%glr-parser
//%expect-rr 1
%error-verbose

%union tokenunion {
    enum yytokentype token;

    classinfo_t*classinfo;
    classinfo_list_t*classinfo_list;
    slotinfo_t*slotinfo;
    slotinfo_list_t*slotinfo_list;

    int number_int;
    unsigned int number_uint;
    double number_float;
    code_t*code;
    typedcode_t value;
    //typedcode_list_t*value_list;
    codeandnumber_t value_list;
    param_t* param;
    params_t params;
    string_t str;
    char*id;
    constant_t*constant;
    for_start_t for_start;
    abc_exception_t *exception;
    regexp_t regexp;
    modifiers_t flags;
    struct {
        abc_exception_list_t *l;
        code_t*finally;
    } catch_list;
}


%token<id> T_IDENTIFIER T_NAMESPACE
%token<str> T_STRING
%token<regexp> T_REGEXP
%token<token> T_EMPTY
%token<number_int> T_INT
%token<number_uint> T_UINT
%token<number_uint> T_BYTE
%token<number_uint> T_SHORT
%token<number_float> T_FLOAT

%token<id> T_FOR "for"
%token<id> T_WHILE "while"
%token<id> T_DO "do"
%token<id> T_SWITCH "switch"

%token<token> KW_IMPLEMENTS "implements"
%token<token> KW_NAMESPACE "namespace"
%token<token> KW_PACKAGE "package"
%token<token> KW_PROTECTED "protected"
%token<token> KW_PUBLIC "public"
%token<token> KW_PRIVATE "private"
%token<token> KW_USE "use"
%token<token> KW_INTERNAL "internal"
%token<token> KW_NEW "new"
%token<token> KW_NATIVE "native"
%token<token> KW_FUNCTION "function"
%token<token> KW_FINALLY "finally"
%token<token> KW_UNDEFINED "undefined"
%token<token> KW_CONTINUE "continue"
%token<token> KW_CLASS "class"
%token<token> KW_CONST "const"
%token<token> KW_CATCH "catch"
%token<token> KW_CASE "case"
%token<token> KW_SET "set"
%token<token> KW_VOID "void"
%token<token> KW_THROW "throw"
%token<token> KW_STATIC "static"
%token<token> KW_WITH "with"
%token<token> KW_INSTANCEOF "instanceof"
%token<token> KW_IMPORT "import"
%token<token> KW_RETURN "return"
%token<token> KW_TYPEOF "typeof"
%token<token> KW_INTERFACE "interface"
%token<token> KW_NULL "null"
%token<token> KW_VAR "var"
%token<token> KW_DYNAMIC "dynamic"
%token<token> KW_OVERRIDE "override"
%token<token> KW_FINAL "final"
%token<token> KW_EACH "each"
%token<token> KW_GET "get"
%token<token> KW_TRY "try"
%token<token> KW_SUPER "super"
%token<token> KW_EXTENDS "extends"
%token<token> KW_FALSE "false"
%token<token> KW_TRUE "true"
%token<token> KW_BOOLEAN "Boolean"
%token<token> KW_UINT "uint"
%token<token> KW_INT "int"
%token<token> KW_NUMBER "Number"
%token<token> KW_STRING "String"
%token<token> KW_DEFAULT "default"
%token<token> KW_DELETE "delete"
%token<token> KW_IF "if"
%token<token> KW_ELSE  "else"
%token<token> KW_BREAK   "break"
%token<token> KW_IS "is"
%token<token> KW_IN "in"
%token<token> KW_AS "as"

%token<token> T_DICTSTART "{ (dictionary)"
%token<token> T_EQEQ "=="
%token<token> T_EQEQEQ "==="
%token<token> T_NE "!="
%token<token> T_NEE "!=="
%token<token> T_LE "<="
%token<token> T_GE ">="
%token<token> T_ORBY "|=" 
%token<token> T_DIVBY "/=" 
%token<token> T_MODBY "%="
%token<token> T_MULBY "*="
%token<token> T_PLUSBY "+=" 
%token<token> T_MINUSBY "-="
%token<token> T_XORBY "^="
%token<token> T_SHRBY ">>="
%token<token> T_SHLBY "<<="
%token<token> T_USHRBY ">>>="
%token<token> T_OROR "||"
%token<token> T_ANDAND "&&"
%token<token> T_COLONCOLON "::"
%token<token> T_MINUSMINUS "--"
%token<token> T_PLUSPLUS "++"
%token<token> T_DOTDOT ".."
%token<token> T_DOTDOTDOT "..."
%token<token> T_SHL "<<"
%token<token> T_USHR ">>>"
%token<token> T_SHR ">>"

%type <for_start> FOR_START
%type <id> X_IDENTIFIER PACKAGE FOR_IN_INIT MAYBE_IDENTIFIER NAMESPACE_ID
%type <token> VARCONST
%type <code> CODE
%type <code> CODEPIECE CODE_STATEMENT
%type <code> CODEBLOCK MAYBECODE MAYBE_CASE_LIST CASE_LIST DEFAULT CASE SWITCH WITH
%type <code> PACKAGE_DECLARATION SLOT_DECLARATION
%type <code> FUNCTION_DECLARATION PACKAGE_INITCODE
%type <code> VARIABLE_DECLARATION ONE_VARIABLE VARIABLE_LIST THROW
%type <exception> CATCH FINALLY
%type <catch_list> CATCH_LIST CATCH_FINALLY_LIST
%type <code> CLASS_DECLARATION
%type <code> NAMESPACE_DECLARATION
%type <code> INTERFACE_DECLARATION
%type <code> VOIDEXPRESSION
%type <value> EXPRESSION NONCOMMAEXPRESSION
%type <value> MAYBEEXPRESSION
%type <value> E DELETE
%type <value> CONSTANT
%type <code> FOR FOR_IN IF WHILE DO_WHILE MAYBEELSE BREAK RETURN CONTINUE TRY 
%type <value> INNERFUNCTION
%type <code> USE_NAMESPACE
%type <code> FOR_INIT
%type <code> IMPORT
%type <classinfo> MAYBETYPE
%type <token> GETSET
%type <param> PARAM
%type <params> PARAM_LIST
%type <params> MAYBE_PARAM_LIST
%type <flags> MAYBE_MODIFIERS
%type <flags> MODIFIER_LIST
%type <flags> MODIFIER
%type <constant> STATICCONSTANT MAYBESTATICCONSTANT
%type <classinfo_list> IMPLEMENTS_LIST
%type <classinfo> EXTENDS CLASS_SPEC
%type <classinfo_list> EXTENDS_LIST

%type <classinfo> CLASS PACKAGEANDCLASS
%type <classinfo_list> CLASS_SPEC_LIST

%type <classinfo> TYPE
//%type <token> VARIABLE
%type <value> VAR_READ
%type <value> NEW
//%type <token> T_IDENTIFIER
%type <value> FUNCTIONCALL
%type <value_list> MAYBE_EXPRESSION_LIST EXPRESSION_LIST EXPRESSION_LIST_AND_COMMA MAYBE_PARAM_VALUES MAYBE_EXPRPAIR_LIST EXPRPAIR_LIST

// precedence: from low to high

%left prec_none

%left below_semicolon
%left ';'
%left ','
%nonassoc below_assignment // for ?:, contrary to spec
%right '=' "*=" "/=" "%=" "+=" "-=" "<<=" ">>=" ">>>=" "&=" "^=" "|="
%right '?' ':'
%left "||"
%left "&&"
%left '|'
%left '^'
%nonassoc '&'
%nonassoc "==" "!=" "===" "!=="
%nonassoc "is" "as" "in"
%nonassoc "<=" '<' ">=" '>' "instanceof" // TODO: support "a < b < c" syntax?
%left "<<" ">>" ">>>" 
%left below_minus
%left '-' '+'
%left '/' '*' '%'
%left plusplus_prefix minusminus_prefix '~' '!' "void" "delete" "typeof" //FIXME: *unary* + - should be here, too
%left "--" "++" 
%nonassoc below_curly

%left '('
%left new2
%left '[' ']' "new" '{' "{ (dictionary)" '.' ".." "::" '@'

%left T_IDENTIFIER
%left above_identifier
%left below_else
%nonassoc "else"

// needed for "return" precedence:
%nonassoc T_STRING T_REGEXP
%nonassoc T_INT T_UINT T_BYTE T_SHORT T_FLOAT
%nonassoc "false" "true" "null" "undefined" "super" "function"
%left above_function


     
%{

static int a3_error(char*s)
{
   syntaxerror("%s", s); 
   return 0; //make gcc happy
}


static char* concat2(const char* t1, const char* t2)
{
    int l1 = strlen(t1);
    int l2 = strlen(t2);
    char*text = malloc(l1+l2+1);
    memcpy(text   , t1, l1);
    memcpy(text+l1, t2, l2);
    text[l1+l2] = 0;
    return text;
}
static char* concat3(const char* t1, const char* t2, const char* t3)
{
    int l1 = strlen(t1);
    int l2 = strlen(t2);
    int l3 = strlen(t3);
    char*text = malloc(l1+l2+l3+1);
    memcpy(text   , t1, l1);
    memcpy(text+l1, t2, l2);
    memcpy(text+l1+l2, t3, l3);
    text[l1+l2+l3] = 0;
    return text;
}

typedef struct _import {
    char*package;
} import_t;

DECLARE_LIST(import);

DECLARE(methodstate);
DECLARE_LIST(methodstate);

typedef struct _classstate {
    /* class data */
    classinfo_t*info;
    abc_class_t*abc;
   
    methodstate_t*init;
    methodstate_t*static_init;
    //code_t*init;
    //code_t*static_init;

    char has_constructor;
} classstate_t;

struct _methodstate {
    /* method data */
    methodinfo_t*info;
    char late_binding;
    char is_constructor;
    char has_super;
    char is_global;
    int variable_count;

    dict_t*unresolved_variables;

    char inner;
    char uses_parent_function;
    int uses_slots;
    dict_t*slots;

    abc_method_t*abc;
    int var_index; // for inner methods
    int slot_index; // for inner methods
    char is_a_slot; // for inner methods

    code_t*header;
    abc_exception_list_t*exceptions;
    
    methodstate_list_t*innerfunctions;
};

typedef struct _state {
    struct _state*old;
    int level;
    
    char*package;     
    import_list_t*wildcard_imports;
    namespace_list_t*active_namespaces;
    dict_t*imports;
    char has_own_imports;
    char new_vars; // e.g. transition between two functions
  
    classstate_t*cls;   
    methodstate_t*method;

    char*exception_name;
    
    dict_t*vars;
} state_t;

typedef struct _global {
    abc_file_t*file;
    abc_script_t*init;
    dict_t*token2info;
    dict_t*file2token2info;
} global_t;

static global_t*global = 0;
static state_t* state = 0;

DECLARE_LIST(state);

#define MULTINAME(m,x) \
    multiname_t m;\
    namespace_t m##_ns;\
    (x)->package; \
    registry_fill_multiname(&m, &m##_ns, (slotinfo_t*)(x));
                    
#define MEMBER_MULTINAME(m,f,n) \
    multiname_t m;\
    namespace_t m##_ns;\
    if(f) { \
        if((m##_ns.access = ((slotinfo_t*)(f))->access)==ACCESS_NAMESPACE) \
            m##_ns.name = ((slotinfo_t*)(f))->package; \
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

static namespace_t ns1 = {ACCESS_PRIVATE, ""};
static namespace_t ns2 = {ACCESS_PROTECTED, ""};
static namespace_t ns3 = {ACCESS_PACKAGEINTERNAL, ""};
static namespace_t ns4 = {ACCESS_PACKAGE, ""};
static namespace_list_t nl4 = {&ns4,0};
static namespace_list_t nl3 = {&ns3,&nl4};
static namespace_list_t nl2 = {&ns2,&nl3};
static namespace_list_t nl1 = {&ns1,&nl2};
static namespace_set_t nopackage_namespace_set = {&nl1};

static void new_state()
{
    NEW(state_t, s);
    state_t*oldstate = state;
    if(state)
        memcpy(s, state, sizeof(state_t)); //shallow copy
    if(!s->imports) {
        s->imports = dict_new();
    }
    state = s;
    state->level++;
    state->has_own_imports = 0;    
    state->vars = dict_new(); 
    state->old = oldstate;
    state->new_vars = 0;
}
static void state_has_imports()
{
    state->wildcard_imports = list_clone(state->wildcard_imports);
    state->imports = dict_clone(state->imports);
    state->has_own_imports = 1;
}

static void state_destroy(state_t*state)
{
    if(state->has_own_imports) {
        list_free(state->wildcard_imports);
        dict_destroy(state->imports);state->imports=0;
    }
    if(state->imports && (!state->old || state->old->imports!=state->imports)) {
        dict_destroy(state->imports);state->imports=0;
    }
    if(state->vars) {
        int t;
        for(t=0;t<state->vars->hashsize;t++) {
            dictentry_t*e =state->vars->slots[t];
            while(e) {
                free(e->data);e->data=0;
                e = e->next;
            }
        }
        dict_destroy(state->vars);state->vars=0;
    }
    
    free(state);
}

static void old_state()
{
    if(!state || !state->old)
        syntaxerror("invalid nesting");
    state_t*leaving = state;
    
    state = state->old;
    
    if(as3_pass>1 && leaving->method && leaving->method != state->method && !leaving->method->inner) {
        free(leaving->method);
        leaving->method=0;
    }
    if(as3_pass>1 && leaving->cls && leaving->cls != state->cls) {
        free(leaving->cls);
        leaving->cls=0;
    }
    
    state_destroy(leaving);
}

static code_t* method_header(methodstate_t*m);
static code_t* wrap_function(code_t*c,code_t*header, code_t*body);
static void function_initvars(methodstate_t*m, params_t*params, int flags, char var0);


static char* internal_filename_package = 0;
void initialize_file(char*filename)
{
    if(state) {
        syntaxerror("invalid call to initialize_file during parsing of another file");
    }
    new_state();
    state->package = internal_filename_package = strdup(filename);
    
    global->token2info = dict_lookup(global->file2token2info, 
                                     current_filename // use long version
                                    );
    if(!global->token2info) {
        global->token2info = dict_new2(&ptr_type);
        dict_put(global->file2token2info, current_filename, global->token2info);
    }
  
    if(as3_pass==1) {
        state->method = rfx_calloc(sizeof(methodstate_t));
        dict_put(global->token2info, (void*)(ptroff_t)as3_tokencount, state->method);
    } else {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        function_initvars(state->method, 0, 0, 1);
        global->init = abc_initscript(global->file);
        state->method->late_binding = 1; // init scripts use getglobalscope, so we need a getlocal0/pushscope
    }
}

void finish_file()
{
    if(!state || state->level!=1) {
        syntaxerror("unexpected end of file in pass %d", as3_pass);
    }

    if(as3_pass==2) {
        code_t*header = method_header(state->method);
        code_t*c = wrap_function(header, 0, global->init->method->body->code);
        global->init->method->body->code = c;
        free(state->method);state->method=0;
    }

    //free(state->package);state->package=0; // used in registry
    state_destroy(state);state=0;
}

void initialize_parser()
{
    global = rfx_calloc(sizeof(global_t));
    global->file = abc_file_new();
    global->file->flags &= ~ABCFILE_LAZY;
    global->file2token2info = dict_new();
    global->token2info = 0;
}

void* finish_parser()
{
    dict_free_all(global->file2token2info, 1, (void*)dict_destroy);

    global->token2info=0;

    return global->file;
}


static void xx_scopetest() 
{
    /* findpropstrict doesn't just return a scope object- it
       also makes it "active" somehow. Push local_0 on the
       scope stack and read it back with findpropstrict, it'll
       contain properties like "trace". Trying to find the same
       property on a "vanilla" local_0 yields only a "undefined" */
    //c = abc_findpropstrict(c, "[package]::trace");
    
    /*c = abc_getlocal_0(c);
    c = abc_findpropstrict(c, "[package]::trace");
    c = abc_coerce_a(c);
    c = abc_setlocal_1(c);

    c = abc_pushbyte(c, 0);
    c = abc_setlocal_2(c);
   
    code_t*xx = c = abc_label(c);
    c = abc_findpropstrict(c, "[package]::trace");
    c = abc_pushstring(c, "prop:");
    c = abc_hasnext2(c, 1, 2);
    c = abc_dup(c);
    c = abc_setlocal_3(c);
    c = abc_callpropvoid(c, "[package]::trace", 2);
    c = abc_getlocal_3(c);
    c = abc_kill(c, 3);
    c = abc_iftrue(c,xx);*/
}

typedef struct _variable {
    int index;
    classinfo_t*type;
    char init;
    methodstate_t*is_inner_method;
} variable_t;

static variable_t* find_variable(state_t*s, char*name)
{
    while(s) {
        variable_t*v = 0;
        v = dict_lookup(s->vars, name);
        if(v) return v;
        if(s->new_vars) break;
        s = s->old;
    }
    return 0;
}
static variable_t* find_slot(state_t*s, const char*name)
{
    if(s->method && s->method->slots)
        return dict_lookup(s->method->slots, name);
    return 0;
}

static variable_t* find_variable_safe(state_t*s, char*name)
{
    variable_t* v = find_variable(s, name);
    if(!v)
        syntaxerror("undefined variable: %s", name);
    return v;
}
static char variable_exists(char*name) 
{
    return dict_contains(state->vars, name);
}
code_t*defaultvalue(code_t*c, classinfo_t*type);

static variable_t* new_variable2(const char*name, classinfo_t*type, char init, char maybeslot)
{
    if(maybeslot) {
        variable_t*v = find_slot(state, name);
        if(v)
            return v;
    }

    NEW(variable_t, v);
    v->index = state->method->variable_count++;
    v->type = type;
    v->init = init;
    
    dict_put(state->vars, name, v);

    return v;
}
static int new_variable(const char*name, classinfo_t*type, char init, char maybeslot)
{
    return new_variable2(name, type, init, maybeslot)->index;
}

#define TEMPVARNAME "__as3_temp__"
static int gettempvar()
{
    variable_t*v = find_variable(state, TEMPVARNAME);
    if(v) 
        return v->index;
    return new_variable(TEMPVARNAME, 0, 0, 0);
}

code_t* var_block(code_t*body) 
{
    code_t*c = 0;
    code_t*k = 0;
    int t;
    int num=0;
    for(t=0;t<state->vars->hashsize;t++) {
        dictentry_t*e = state->vars->slots[t];
        while(e) {
            variable_t*v = (variable_t*)e->data;
            if(v->type && v->init) {
                c = defaultvalue(c, v->type);
                c = abc_setlocal(c, v->index);
                k = abc_kill(k, v->index); 
                num++;
            }
            e = e->next;
        }
    }

    if(k) {
        code_t*x = body;
        while(x) {
            if(x->opcode== OPCODE___BREAK__ ||
               x->opcode== OPCODE___CONTINUE__) {
               /* link kill code before break/continue */
                code_t*e = code_dup(k);
                code_t*s = code_start(e);
                s->prev = x->prev;
                if(x->prev) {
                    x->prev->next = s;
                }
                e->next = x;
                x->prev = e;
            }
            x = x->prev;
        }
    }
    
    c = code_append(c, body);
    c = code_append(c, k);
    return c;
}

void unknown_variable(char*name) 
{
    if(!state->method->unresolved_variables)
        state->method->unresolved_variables = dict_new();
    if(!dict_contains(state->method->unresolved_variables, name))
        dict_put(state->method->unresolved_variables, name, 0);
}

#define parserassert(b) {if(!(b)) parsererror(__FILE__, __LINE__,__func__);}

static void parsererror(const char*file, int line, const char*f)
{
    syntaxerror("internal error in %s, %s:%d", f, file, line);
}

   
static code_t* method_header(methodstate_t*m)
{
    code_t*c = 0;
    if(m->uses_slots || (m->late_binding && !m->inner)) {
        c = abc_getlocal_0(c);
        c = abc_pushscope(c);
    }
    if(m->uses_slots) {
        c = abc_newactivation(c);
        c = abc_pushscope(c);
    }
    methodstate_list_t*l = m->innerfunctions;
    while(l) {
        parserassert(l->methodstate->abc);
        if(m->uses_slots && l->methodstate->is_a_slot) {
            c = abc_getscopeobject(c, 1);
            c = abc_newfunction(c, l->methodstate->abc);
            c = abc_dup(c);
            c = abc_setlocal(c, l->methodstate->var_index);
            c = abc_setslot(c, l->methodstate->slot_index);
        } else {
            c = abc_newfunction(c, l->methodstate->abc);
            c = abc_setlocal(c, l->methodstate->var_index);
        }
        free(l->methodstate);l->methodstate=0;
        l = l->next;
    }
    if(m->header) {
        c = code_append(c, m->header);
        m->header = 0;
    }
    if(m->is_constructor && !m->has_super) {
        // call default constructor
        c = abc_getlocal_0(c);
        c = abc_constructsuper(c, 0);
    }
    list_free(m->innerfunctions);
    m->innerfunctions = 0;
    return c;
}
    

static code_t* wrap_function(code_t*c,code_t*header, code_t*body)
{
    c = code_append(c, header);
    c = code_append(c, var_block(body));
    /* append return if necessary */
    if(!c || (c->opcode != OPCODE_RETURNVOID && 
              c->opcode != OPCODE_RETURNVALUE)) {
        c = abc_returnvoid(c);
    }
    return c;
}


static void startpackage(char*name)
{
    new_state();
    /*printf("entering package \"%s\"\n", name);*/
    state->package = strdup(name);
}
static void endpackage()
{
    /*printf("leaving package \"%s\"\n", state->package);*/

    //used e.g. in classinfo_register:
    //free(state->package);state->package=0;

    old_state();
}

#define FLAG_PUBLIC 256
#define FLAG_PROTECTED 512
#define FLAG_PRIVATE 1024
#define FLAG_PACKAGEINTERNAL 2048
#define FLAG_NAMESPACE 4096

static namespace_t modifiers2access(modifiers_t*mod)
{
    namespace_t ns;
    ns.access = 0;
    ns.name = "";
    if(mod->flags&FLAG_NAMESPACE)  {
        if(mod->flags&(FLAG_PRIVATE|FLAG_PROTECTED|FLAG_PACKAGEINTERNAL)) 
            syntaxerror("invalid combination of access levels and namespaces");
        ns.access = ACCESS_NAMESPACE;
        ns.name = mod->ns;
    } else if(mod->flags&FLAG_PUBLIC)  {
        if(mod->flags&(FLAG_PRIVATE|FLAG_PROTECTED|FLAG_PACKAGEINTERNAL)) 
            syntaxerror("invalid combination of access levels");
        ns.access = ACCESS_PACKAGE;
    } else if(mod->flags&FLAG_PRIVATE) {
        if(mod->flags&(FLAG_PUBLIC|FLAG_PROTECTED|FLAG_PACKAGEINTERNAL)) 
            syntaxerror("invalid combination of access levels");
        ns.access = ACCESS_PRIVATE;
    } else if(mod->flags&FLAG_PROTECTED) {
        if(mod->flags&(FLAG_PUBLIC|FLAG_PRIVATE|FLAG_PACKAGEINTERNAL)) 
            syntaxerror("invalid combination of access levels");
        ns.access = ACCESS_PROTECTED;
    } else {
        ns.access = ACCESS_PACKAGEINTERNAL;
    }
    return ns;
}

static void function_initvars(methodstate_t*m, params_t*params, int flags, char var0)
{
    if(var0) {
        int index = -1;
        if(m->inner)
            index = new_variable("this", 0, 0, 0);
        else if(!m->is_global)
            index = new_variable((flags&FLAG_STATIC)?"class":"this", state->cls?state->cls->info:0, 0, 0);
        else
            index = new_variable("globalscope", 0, 0, 0);
        if(index)
            *(int*)0=0;
        parserassert(!index);
    }
    if(m->uses_slots) {
        /* as variables and slots share the same number, make sure
           that those variable indices are reserved. It's up to the
           optimizer to later shuffle the variables down to lower
           indices */
        m->variable_count = m->uses_slots; 
    }

    if(params) {
        param_list_t*p=0;
        for(p=params->list;p;p=p->next) {
            new_variable(p->param->name, p->param->type, 0, 1);
        }
    }
    
    methodstate_list_t*l = m->innerfunctions;
    while(l) {
        methodstate_t*m = l->methodstate;
        variable_t* v = new_variable2(m->info->name, TYPE_FUNCTION(m->info), 0, 1);
        m->var_index = v->index;
        m->slot_index = v->index;
        v->is_inner_method = m;
        l = l->next;
    }
}


char*as3_globalclass=0;
static void startclass(modifiers_t* mod, char*classname, classinfo_t*extends, classinfo_list_t*implements)
{
    if(state->cls) {
        syntaxerror("inner classes now allowed"); 
    }
    new_state();
    token_list_t*t=0;
    classinfo_list_t*mlist=0;

    if(mod->flags&~(FLAG_PACKAGEINTERNAL|FLAG_PUBLIC|FLAG_FINAL|FLAG_DYNAMIC|FLAG_INTERFACE))
        syntaxerror("invalid modifier(s)");

    if((mod->flags&(FLAG_PUBLIC|FLAG_PACKAGEINTERNAL)) == (FLAG_PUBLIC|FLAG_PACKAGEINTERNAL))
        syntaxerror("public and internal not supported at the same time.");

    /* create the class name, together with the proper attributes */
    int access=0;
    char*package=0;

    if(!(mod->flags&FLAG_PUBLIC) && state->package==internal_filename_package) {
        access = ACCESS_PRIVATE; package = internal_filename_package;
    } else if(!(mod->flags&FLAG_PUBLIC) && state->package!=internal_filename_package) {
        access = ACCESS_PACKAGEINTERNAL; package = state->package;
    } else if(state->package!=internal_filename_package) {
        access = ACCESS_PACKAGE; package = state->package;
    } else {
        syntaxerror("public classes only allowed inside a package");
    }

    if(as3_pass==1) {
        state->cls = rfx_calloc(sizeof(classstate_t));
        state->cls->init = rfx_calloc(sizeof(methodstate_t));
        state->cls->static_init = rfx_calloc(sizeof(methodstate_t));
        /* notice: we make no effort to initialize the top variable (local0) here,
           even though it has special meaning. We just rely on the facat
           that pass 1 won't do anything with variables */
        
        dict_put(global->token2info, (void*)(ptroff_t)as3_tokencount, state->cls);

        /* set current method to constructor- all code within the class-level (except
           static variable initializations) will be executed during construction time */
        state->method = state->cls->init;

        if(registry_find(package, classname)) {
            syntaxerror("Package \"%s\" already contains a class called \"%s\"", package, classname);
        }
        /* build info struct */
        int num_interfaces = (list_length(implements));
        state->cls->info = classinfo_register(access, package, classname, num_interfaces);
        state->cls->info->flags |= mod->flags & (FLAG_DYNAMIC|FLAG_INTERFACE|FLAG_FINAL);
    }
    
    if(as3_pass == 2) {
        state->cls = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        
        state->method = state->cls->init;
        parserassert(state->cls && state->cls->info);
       
        function_initvars(state->cls->init, 0, 0, 1);
        function_initvars(state->cls->static_init, 0, 0, 0);

        if(extends && (extends->flags & FLAG_FINAL))
            syntaxerror("Can't extend final class '%s'", extends->name);

        /* fill out interfaces and extends (we couldn't resolve those during the first pass) */
        state->cls->info->superclass = extends?extends:TYPE_OBJECT;
        int pos = 0;
        classinfo_list_t*l = implements;
        for(l=implements;l;l=l->next) {
            if(!(l->classinfo->flags & FLAG_INTERFACE))
                syntaxerror("'%s' is not an interface", l->classinfo->name);
            state->cls->info->interfaces[pos++] = l->classinfo;
        }

        /* generate the abc code for this class */
        MULTINAME(classname2,state->cls->info);
        multiname_t*extends2 = sig2mname(extends);

        state->cls->abc = abc_class_new(global->file, &classname2, extends2);
        if(state->cls->info->flags&FLAG_FINAL) abc_class_final(state->cls->abc);
        if(!(state->cls->info->flags&FLAG_DYNAMIC)) abc_class_sealed(state->cls->abc);
        if(state->cls->info->flags&FLAG_INTERFACE) {
            abc_class_interface(state->cls->abc);
        }

        abc_class_protectedNS(state->cls->abc, classname);

        for(mlist=implements;mlist;mlist=mlist->next) {
            MULTINAME(m, mlist->classinfo);
            abc_class_add_interface(state->cls->abc, &m);
        }

        /* write the construction code for this class to the global init
           function */
        int slotindex = abc_initscript_addClassTrait(global->init, &classname2, state->cls->abc);

        abc_method_body_t*m = global->init->method->body;
        __ getglobalscope(m);
        classinfo_t*s = extends;

        int count=0;
        
        while(s) {
            //TODO: take a look at the current scope stack, maybe 
            //      we can re-use something
            s = s->superclass;
            if(!s) 
            break;
           
            multiname_t*s2 = sig2mname(s);
            __ getlex2(m, s2);
            multiname_destroy(s2);

            __ pushscope(m); count++;
            m->code = m->code->prev->prev; // invert
        }
        /* continue appending after last op end */
        while(m->code && m->code->next) m->code = m->code->next; 

        /* TODO: if this is one of *our* classes, we can also 
                 do a getglobalscope/getslot <nr> (which references
                 the init function's slots) */
        if(extends2) {
            __ getlex2(m, extends2);
            __ dup(m);
            /* notice: we get a Verify Error #1107 if the top elemnt on the scope
               stack is not the superclass */
            __ pushscope(m);count++;
        } else {
            __ pushnull(m);
            /* notice: we get a verify error #1107 if the top element on the scope 
               stack is not the global object */
            __ getlocal_0(m);
            __ pushscope(m);count++;
        }
        __ newclass(m,state->cls->abc);
        while(count--) {
            __ popscope(m);
        }
        __ setslot(m, slotindex);
        multiname_destroy(extends2);

        /* flash.display.MovieClip handling */

        if(!as3_globalclass && (mod->flags&FLAG_PUBLIC) && slotinfo_equals((slotinfo_t*)registry_getMovieClip(),(slotinfo_t*)extends)) {
            if(state->package && state->package[0]) {
                as3_globalclass = concat3(state->package, ".", classname);
            } else {
                as3_globalclass = strdup(classname);
            }
        }
    }
}

static void setstaticfunction(int x)
{
    if(state->cls) {
        if(x&FLAG_STATIC) {
            state->method = state->cls->static_init;
        } else {
            state->method = state->cls->init;
        }
    } else {
        parserassert(state->method);
    }
}

static void endclass()
{
    if(as3_pass == 2) {
        if(!state->cls->has_constructor && !(state->cls->info->flags&FLAG_INTERFACE)) {
            code_t*c = 0;
            c = abc_getlocal_0(c);
            c = abc_constructsuper(c, 0);
            state->cls->init->header = code_append(state->cls->init->header, c);
            state->cls->has_constructor=1;
        }
        if(state->cls->init) {
            if(state->cls->info->flags&FLAG_INTERFACE) {
                if(state->cls->init->header) 
                    syntaxerror("interface can not have class-level code");
            } else {
                abc_method_t*m = abc_class_getconstructor(state->cls->abc, 0);
                code_t*c = method_header(state->cls->init);
                m->body->code = wrap_function(c, 0, m->body->code);
            }
        }
        if(state->cls->static_init) {
            abc_method_t*m = abc_class_getstaticconstructor(state->cls->abc, 0);
            code_t*c = method_header(state->cls->static_init);
            m->body->code = wrap_function(c, 0, m->body->code);
        }
    }

    old_state();
}

void check_code_for_break(code_t*c)
{
    while(c) {
        if(c->opcode == OPCODE___BREAK__) {
            char*name = string_cstr(c->data[0]);
            syntaxerror("Unresolved \"break %s\"", name);
        }
        if(c->opcode == OPCODE___CONTINUE__) {
            char*name = string_cstr(c->data[0]);
            syntaxerror("Unresolved \"continue %s\"", name);
        }
        c=c->prev;
    }
}


static void check_constant_against_type(classinfo_t*t, constant_t*c)
{
#define xassert(b) if(!(b)) syntaxerror("Invalid default value %s for type '%s'", constant_tostring(c), t->name)
   if(TYPE_IS_NUMBER(t)) {
        xassert(c->type == CONSTANT_FLOAT
             || c->type == CONSTANT_INT
             || c->type == CONSTANT_UINT);
   } else if(TYPE_IS_UINT(t)) {
        xassert(c->type == CONSTANT_UINT ||
               (c->type == CONSTANT_INT && c->i>=0));
   } else if(TYPE_IS_INT(t)) {
        xassert(c->type == CONSTANT_INT);
   } else if(TYPE_IS_BOOLEAN(t)) {
        xassert(c->type == CONSTANT_TRUE
             || c->type == CONSTANT_FALSE);
   }
}

static void check_override(memberinfo_t*m, int flags)
{
    if(!m)
        return;
    if(m->parent == state->cls->info)
        syntaxerror("class '%s' already contains a method/slot '%s'", m->parent->name, m->name);
    if(!m->parent)
        syntaxerror("internal error: overriding method %s, which doesn't have parent", m->name);
    if(m->access==ACCESS_PRIVATE)
        return;
    if(m->flags & FLAG_FINAL)
        syntaxerror("can't override final member %s", m->name);
    if((m->flags & FLAG_STATIC) && !(flags&FLAG_STATIC))
        syntaxerror("can't override static member %s", m->name);
    if(!(m->flags & FLAG_STATIC) && (flags&FLAG_STATIC))
        syntaxerror("can't override non-static member %s with static declaration", m->name);

    if(!(flags&FLAG_OVERRIDE)) {
        if(m->parent && !(m->parent->flags&FLAG_INTERFACE)) {
            if(m->kind == INFOTYPE_METHOD)
                syntaxerror("can't override without explicit 'override' declaration");
            else
                syntaxerror("can't override '%s'", m->name);
        }
    }
}

static methodinfo_t*registerfunction(enum yytokentype getset, modifiers_t*mod, char*name, params_t*params, classinfo_t*return_type, int slot)
{
    methodinfo_t*minfo = 0;
    namespace_t ns = modifiers2access(mod);
    if(!state->cls) {
        //package method
        minfo = methodinfo_register_global(ns.access, state->package, name);
        minfo->return_type = return_type;
    } else if(getset != KW_GET && getset != KW_SET) {
        //class method
        memberinfo_t* m = registry_findmember(state->cls->info, ns.name, name, 0);
        if(m) {
            printf("%s.%s | %s.%s\n", 
                m->package, m->name,
                ns.name, name);
            syntaxerror("class already contains a %s '%s'", infotypename((slotinfo_t*)m), m->name);
        }
        minfo = methodinfo_register_onclass(state->cls->info, ns.access, ns.name, name);
        minfo->return_type = return_type;
        // getslot on a member slot only returns "undefined", so no need
        // to actually store these
        //state->minfo->slot = state->method->abc->method->trait->slot_id;
    } else {
        //class getter/setter
        int gs = getset==KW_GET?SUBTYPE_GET:SUBTYPE_SET;
        classinfo_t*type=0;
        if(getset == KW_GET)
            type = return_type;
        else if(params->list && params->list->param)
            type = params->list->param->type;
        // not sure wether to look into superclasses here, too
        minfo = (methodinfo_t*)registry_findmember(state->cls->info, ns.name, name, 1);
        if(minfo) {
            if(minfo->kind!=INFOTYPE_SLOT)
                syntaxerror("class already contains a method called '%s'", name);
            if(!(minfo->subtype & (SUBTYPE_GETSET)))
                syntaxerror("class already contains a field called '%s'", name);
            if(minfo->subtype & gs)
                syntaxerror("getter/setter for '%s' already defined", name);
            /* make a setter or getter into a getset */
            minfo->subtype |= gs;
            if(!minfo->return_type) {
                minfo->return_type = type;
            } else {
                if(minfo && minfo->return_type != type)
                    syntaxerror("different type in getter and setter");
            }
        } else {
            minfo = methodinfo_register_onclass(state->cls->info, ns.access, ns.name, name);
            minfo->kind = INFOTYPE_SLOT; //hack
            minfo->subtype = gs;
            minfo->return_type = type;
        }
        /* can't assign a slot as getter and setter might have different slots */
        //minfo->slot = slot;
    }
    if(mod->flags&FLAG_FINAL) minfo->flags |= FLAG_FINAL;
    if(mod->flags&FLAG_STATIC) minfo->flags |= FLAG_STATIC;
    if(mod->flags&FLAG_OVERRIDE) minfo->flags |= FLAG_OVERRIDE;

    return minfo;
}

static void innerfunction(char*name, params_t*params, classinfo_t*return_type)
{
    //parserassert(state->method && state->method->info);

    methodstate_t*parent_method = state->method;

    if(as3_pass==1) {
        return_type = 0; // not valid in pass 1
    }

    new_state();
    state->new_vars = 1;
   
    if(as3_pass == 1) {
        state->method = rfx_calloc(sizeof(methodstate_t));
        state->method->inner = 1;
        state->method->variable_count = 0;
        state->method->abc = rfx_calloc(sizeof(abc_method_t));

        NEW(methodinfo_t,minfo);
        minfo->kind = INFOTYPE_METHOD;
        minfo->access = ACCESS_PACKAGEINTERNAL;
        minfo->name = name;
        state->method->info = minfo;

        if(parent_method)
            list_append(parent_method->innerfunctions, state->method);

        dict_put(global->token2info, (void*)(ptroff_t)as3_tokencount, state->method);
       
        function_initvars(state->method, params, 0, 1);
    }

    if(as3_pass == 2) {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        state->method->variable_count = 0;
        parserassert(state->method);

        state->method->info->return_type = return_type;
        function_initvars(state->method, params, 0, 1);
    }
}

static void startfunction(modifiers_t*mod, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type)
{
    if(state->method && state->method->info) {
        syntaxerror("not able to start another method scope");
    }
    new_state();
    state->new_vars = 1;
    
    if(as3_pass == 1) {
        state->method = rfx_calloc(sizeof(methodstate_t));
        state->method->has_super = 0;

        if(state->cls) {
            state->method->is_constructor = !strcmp(state->cls->info->name,name);
        } else {
            state->method->is_global = 1;
            state->method->late_binding = 1; // for global methods, always push local_0 on the scope stack
        }
        if(state->method->is_constructor)
            name = "__as3_constructor__";

        return_type = 0;
        state->method->info = registerfunction(getset, mod, name, params, return_type, 0);
       
        function_initvars(state->method, params, mod->flags, 1);
        
        dict_put(global->token2info, (void*)(ptroff_t)as3_tokencount, state->method);
    }

    if(as3_pass == 2) {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        state->method->variable_count = 0;
        parserassert(state->method);
                
        if(state->cls) {
            memberinfo_t*m = registry_findmember(state->cls->info, mod->ns, name, 2);
            check_override(m, mod->flags);
        }
            
        if(state->cls) { 
            state->cls->has_constructor |= state->method->is_constructor;
        }
        
        state->method->info->return_type = return_type;
        function_initvars(state->method, params, mod->flags, 1);
    } 
}

static abc_method_t* endfunction(modifiers_t*mod, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type, code_t*body)
{
    int flags = mod?mod->flags:0;

    if(as3_pass==1) {
        // store inner methods in variables
        function_initvars(state->method, 0, 0, 0);

        methodstate_list_t*ml = state->method->innerfunctions;
        dict_t*xvars = dict_new();
        while(ml) {
            methodstate_t*m = ml->methodstate;
            parserassert(m->inner);
            if(m->unresolved_variables) {
                dict_t*d = m->unresolved_variables;
                int t;
                for(t=0;t<d->hashsize;t++) {
                    dictentry_t*l = d->slots[t]; 
                    while(l) {
                        /* check parent method's variables */
                        if(find_variable(state, l->key)) {
                            m->uses_parent_function = 1;
                            state->method->uses_slots = 1;
                            dict_put(xvars, l->key, 0);
                        }
                        l = l->next;
                    }
                    if(l) break;
                }

                dict_destroy(m->unresolved_variables);
                m->unresolved_variables = 0;
            }
            ml = ml->next;
        }
        if(state->method->uses_slots) {
            state->method->slots = dict_new();
            int i = 1;
            DICT_ITERATE_ITEMS(state->vars, char*, name, variable_t*, v) {
                if(v->index && dict_contains(xvars, name)) {
                    v->init = 0;
                    v->index = i++;
                    if(v->is_inner_method) {
                        v->is_inner_method->is_a_slot = 1;
                    }
                    //v->type = 0;
                    dict_put(state->method->slots, name, v);
                }
            }
            state->method->uses_slots = i;
            dict_destroy(state->vars);state->vars = 0;
        }
        dict_destroy(xvars);

        old_state();
        return 0;
    }

    if(as3_pass==2) {
        /*if(state->method->uses_parent_function){
            syntaxerror("accessing variables of parent function from inner functions not supported yet");
        }*/

        abc_method_t*f = 0;

        multiname_t*type2 = sig2mname(return_type);
        int slot = 0;
        if(state->method->inner) {
            f = state->method->abc;
            abc_method_init(f, global->file, type2, 1);
        } else if(state->method->is_constructor) {
            f = abc_class_getconstructor(state->cls->abc, type2);
        } else if(!state->method->is_global) {
            namespace_t mname_ns = {state->method->info->access, ""};
            multiname_t mname = {QNAME, &mname_ns, 0, name};

            if(flags&FLAG_STATIC)
                f = abc_class_staticmethod(state->cls->abc, type2, &mname);
            else
                f = abc_class_method(state->cls->abc, type2, &mname);
            slot = f->trait->slot_id;
        } else {
            namespace_t mname_ns = {state->method->info->access, state->package};
            multiname_t mname = {QNAME, &mname_ns, 0, name};

            f = abc_method_new(global->file, type2, 1);
            trait_t*t = trait_new_method(&global->init->traits, multiname_clone(&mname), f);
            //abc_code_t*c = global->init->method->body->code;
        }
        //flash doesn't seem to allow us to access function slots
        //state->method->info->slot = slot;

        if(flags&FLAG_OVERRIDE) f->trait->attributes |= TRAIT_ATTR_OVERRIDE;
        if(getset == KW_GET) f->trait->kind = TRAIT_GETTER;
        if(getset == KW_SET) f->trait->kind = TRAIT_SETTER;
        if(params->varargs) f->flags |= METHOD_NEED_REST;

        char opt=0;
        param_list_t*p=0;
        for(p=params->list;p;p=p->next) {
            if(params->varargs && !p->next) {
                break; //varargs: omit last parameter in function signature
            }
            multiname_t*m = sig2mname(p->param->type);
            list_append(f->parameters, m);
            if(p->param->value) {
                check_constant_against_type(p->param->type, p->param->value);
                opt=1;list_append(f->optional_parameters, p->param->value);
            } else if(opt) {
                syntaxerror("non-optional parameter not allowed after optional parameters");
            }
        }
        if(state->method->slots) {
            DICT_ITERATE_ITEMS(state->method->slots, char*, name, variable_t*, v) {
                if(v->index) {
                    multiname_t*mname = multiname_new(namespace_new(ACCESS_PACKAGE, ""), name);
                    multiname_t*type = sig2mname(v->type);
                    trait_t*t = trait_new_member(&f->body->traits, type, mname, 0);
                    t->slot_id = v->index;
                }
            }
        }

        check_code_for_break(body);

        if(f->body) {
            f->body->code = body;
            f->body->exceptions = state->method->exceptions;
        } else { //interface
            if(body)
                syntaxerror("interface methods can't have a method body");
        }

        old_state();
        return f;
    }
        
    return 0;
}

char is_subtype_of(classinfo_t*type, classinfo_t*supertype)
{
    return 1; // FIXME
}

void breakjumpsto(code_t*c, char*name, code_t*jump) 
{
    while(c) {
        if(c->opcode == OPCODE___BREAK__) {
            string_t*name2 = c->data[0];
            if(!name2->len || !strncmp(name2->str, name, name2->len)) {
                c->opcode = OPCODE_JUMP;
                c->branch = jump;
            }
        }
        c=c->prev;
    }
}
void continuejumpsto(code_t*c, char*name, code_t*jump) 
{
    while(c) {
        if(c->opcode == OPCODE___CONTINUE__) {
            string_t*name2 = c->data[0];
            if(!name2->len || !strncmp(name2->str, name, name2->len)) {
                c->opcode = OPCODE_JUMP;
                c->branch = jump;
            }
        }
        c = c->prev;
    }
}

#define IS_INT(a) (TYPE_IS_INT((a)) || TYPE_IS_UINT((a)))
#define IS_NUMBER_OR_INT(a) (TYPE_IS_INT((a)) || TYPE_IS_UINT((a)) || TYPE_IS_NUMBER((a)))
#define BOTH_INT(a,b) (IS_INT(a) && IS_INT(b))

classinfo_t*join_types(classinfo_t*type1, classinfo_t*type2, char op)
{
    if(!type1 || !type2) 
        return registry_getanytype();
    if(TYPE_IS_ANY(type1) || TYPE_IS_ANY(type2))
        return registry_getanytype();

    if(op=='+') {
        if(IS_NUMBER_OR_INT(type1) && IS_NUMBER_OR_INT(type2)) {
            return TYPE_NUMBER;
        } else {
            return TYPE_ANY;
        }
    }

    if(type1 == type2)
        return type1;
    return registry_getanytype();
}
code_t*converttype(code_t*c, classinfo_t*from, classinfo_t*to)
{
    if(from==to)
        return c;
    if(!to) {
        return abc_coerce_a(c);
    }
    MULTINAME(m, to);
    if(!from) {
        // cast an "any" type to a specific type. subject to
        // runtime exceptions
        return abc_coerce2(c, &m);
    }
    
    if((TYPE_IS_NUMBER(from) || TYPE_IS_UINT(from) || TYPE_IS_INT(from)) &&
       (TYPE_IS_NUMBER(to) || TYPE_IS_UINT(to) || TYPE_IS_INT(to))) {
        // allow conversion between number types
        return abc_coerce2(c, &m);
    }
    //printf("%s.%s\n", from.package, from.name);
    //printf("%s.%s\n", to.package, to.name);

    classinfo_t*supertype = from;
    while(supertype) {
        if(supertype == to) {
             // target type is one of from's superclasses
             return abc_coerce2(c, &m);
        }
        int t=0;
        while(supertype->interfaces[t]) {
            if(supertype->interfaces[t]==to) {
                // target type is one of from's interfaces
                return abc_coerce2(c, &m);
            }
            t++;
        }
        supertype = supertype->superclass;
    }
    if(TYPE_IS_FUNCTION(from) && TYPE_IS_FUNCTION(to))
        return c;
    if(TYPE_IS_CLASS(from) && TYPE_IS_CLASS(to))
        return c;
    if(TYPE_IS_NULL(from) && !IS_NUMBER_OR_INT(to))
        return c;

    as3_error("can't convert type %s%s%s to %s%s%s", 
        from->package, from->package?".":"", from->name, 
        to->package, to->package?".":"", to->name);
    return c;
}

code_t*defaultvalue(code_t*c, classinfo_t*type)
{
    if(TYPE_IS_INT(type)) {
       c = abc_pushbyte(c, 0);
    } else if(TYPE_IS_UINT(type)) {
       c = abc_pushuint(c, 0);
    } else if(TYPE_IS_FLOAT(type)) {
       c = abc_pushnan(c);
    } else if(TYPE_IS_BOOLEAN(type)) {
       c = abc_pushfalse(c);
    } else if(!type) {
       //c = abc_pushundefined(c);
    } else {
       c = abc_pushnull(c);
       MULTINAME(m, type);
       c = abc_coerce2(c, &m);
    }
    return c;
}

char is_pushundefined(code_t*c)
{
    return (c && !c->prev && !c->next && c->opcode == OPCODE_PUSHUNDEFINED);
}

static slotinfo_t* find_class(char*name)
{
    slotinfo_t*c=0;

    c = registry_find(state->package, name);
    if(c) return c;

    /* try explicit imports */
    dictentry_t* e = dict_get_slot(state->imports, name);
    if(c) return c;
    while(e) {
        if(!strcmp(e->key, name)) {
            c = (slotinfo_t*)e->data;
            if(c) return c;
        }
        e = e->next;
    }

    /* try package.* imports */
    import_list_t*l = state->wildcard_imports;
    while(l) {
        //printf("does package %s contain a class %s?\n", l->import->package, name);
        c = registry_find(l->import->package, name);
        if(c) return c;
        l = l->next;
    }

    /* try global package */
    c = registry_find("", name);
    if(c) return c;
  
    /* try local "filename" package */
    c = registry_find(internal_filename_package, name);
    if(c) return c;

    return 0;
}

static char is_getlocal(code_t*c)
{
    if(!c || c->prev || c->next)
        return 0;
    return(c->opcode == OPCODE_GETLOCAL
        || c->opcode == OPCODE_GETLOCAL_0
        || c->opcode == OPCODE_GETLOCAL_1
        || c->opcode == OPCODE_GETLOCAL_2
        || c->opcode == OPCODE_GETLOCAL_3);
}
static int getlocalnr(code_t*c)
{
    if(c->opcode == OPCODE_GETLOCAL) {return (ptroff_t)c->data[0];}
    else if(c->opcode == OPCODE_GETLOCAL_0) {return 0;}
    else if(c->opcode == OPCODE_GETLOCAL_1) {return 1;}
    else if(c->opcode == OPCODE_GETLOCAL_2) {return 2;}
    else if(c->opcode == OPCODE_GETLOCAL_3) {return 3;}
    else syntaxerror("Internal error: opcode %02x is not a getlocal call", c->opcode);
    return 0;
}

static code_t* toreadwrite(code_t*in, code_t*middlepart, char justassign, char readbefore)
{
    /* converts this:

       [prefix code] [read instruction]

       to this:

       [prefix code] ([dup]) [read instruction] [middlepart] [setvar] [write instruction] [getvar]
    */
    if(in && in->opcode == OPCODE_COERCE_A) {
        in = code_cutlast(in);
    }
    if(in->next)
        syntaxerror("internal error");

    /* chop off read instruction */
    code_t*prefix = in;
    code_t*r = in;
    if(r->prev) {
        prefix = r->prev;r->prev = 0;
        prefix->next=0;
    } else {
        prefix = 0;
    }

    char use_temp_var = readbefore;

    /* generate the write instruction, and maybe append a dup to the prefix code */
    code_t* write = abc_nop(0);
    if(r->opcode == OPCODE_GETPROPERTY) {
        write->opcode = OPCODE_SETPROPERTY;
        multiname_t*m = (multiname_t*)r->data[0];
        write->data[0] = multiname_clone(m);
        if(m->type == QNAME || m->type == MULTINAME) {
            if(!justassign) {
                prefix = abc_dup(prefix); // we need the object, too
            }
            use_temp_var = 1;
        } else if(m->type == MULTINAMEL) {
            if(!justassign) {
                /* dupping two values on the stack requires 5 operations and one register- 
                   couldn't adobe just have given us a dup2? */
                int temp = gettempvar();
                prefix = abc_setlocal(prefix, temp);
                prefix = abc_dup(prefix);
                prefix = abc_getlocal(prefix, temp);
                prefix = abc_swap(prefix);
                prefix = abc_getlocal(prefix, temp);
                if(!use_temp_var);
                    prefix = abc_kill(prefix, temp);
            }
            use_temp_var = 1;
        } else {
            syntaxerror("illegal lvalue: can't assign a value to this expression (not a qname/multiname)");
        }
    } else if(r->opcode == OPCODE_GETSLOT) {
        write->opcode = OPCODE_SETSLOT;
        write->data[0] = r->data[0];
        if(!justassign) {
            prefix = abc_dup(prefix); // we need the object, too
        }
        use_temp_var = 1;
    } else if(r->opcode == OPCODE_GETLOCAL) { 
        write->opcode = OPCODE_SETLOCAL;
        write->data[0] = r->data[0];
    } else if(r->opcode == OPCODE_GETLOCAL_0) { 
        write->opcode = OPCODE_SETLOCAL_0;
    } else if(r->opcode == OPCODE_GETLOCAL_1) { 
        write->opcode = OPCODE_SETLOCAL_1;
    } else if(r->opcode == OPCODE_GETLOCAL_2) { 
        write->opcode = OPCODE_SETLOCAL_2;
    } else if(r->opcode == OPCODE_GETLOCAL_3) { 
        write->opcode = OPCODE_SETLOCAL_3;
    } else {
        code_dump(r);
        syntaxerror("illegal lvalue: can't assign a value to this expression");
    }
    code_t* c = 0;
    
    int temp = -1;
    if(!justassign) {
        if(use_temp_var) {
            /* with getproperty/getslot, we have to be extra careful not
               to execute the read code twice, as it might have side-effects
               (e.g. if the property is in fact a setter/getter combination)

               So read the value, modify it, and write it again,
               using prefix only once and making sure (by using a temporary
               register) that the return value is what we just wrote */
            temp = gettempvar();
            c = code_append(c, prefix);
            c = code_append(c, r);
            if(readbefore) {
                c = abc_dup(c);
                c = abc_setlocal(c, temp);
            }
            c = code_append(c, middlepart);
            if(!readbefore) {
                c = abc_dup(c);
                c = abc_setlocal(c, temp);
            }
            c = code_append(c, write);
            c = abc_getlocal(c, temp);
            c = abc_kill(c, temp);
        } else {
            /* if we're allowed to execute the read code twice *and*
               the middlepart doesn't modify the code, things are easier.
            */
            code_t* r2 = code_dup(r);
            //c = code_append(c, prefix);
            parserassert(!prefix);
            c = code_append(c, r);
            c = code_append(c, middlepart);
            c = code_append(c, write);
            c = code_append(c, r2);
        }
    } else {
        /* even smaller version: overwrite the value without reading
           it out first */
        if(!use_temp_var) {
            if(prefix) {
                c = code_append(c, prefix);
                c = abc_dup(c);
            }
            c = code_append(c, middlepart);
            c = code_append(c, write);
            c = code_append(c, r);
        } else {
            code_free(r);r=0;
            temp = gettempvar();
            if(prefix) {
                c = code_append(c, prefix);
            }
            c = code_append(c, middlepart);
            c = abc_dup(c);
            c = abc_setlocal(c, temp);
            c = code_append(c, write);
            c = abc_getlocal(c, temp);
            c = abc_kill(c, temp);
        }
    }
    return c;
}

char is_break_or_jump(code_t*c)
{
    if(!c)
        return 0;
    if(c->opcode == OPCODE_JUMP ||
       c->opcode == OPCODE___BREAK__ ||
       c->opcode == OPCODE___CONTINUE__ ||
       c->opcode == OPCODE_THROW ||
       c->opcode == OPCODE_RETURNVOID ||
       c->opcode == OPCODE_RETURNVALUE) {
       return 1;
    }
    return 0;
}


#define IS_FINALLY_TARGET(op) \
        ((op) == OPCODE___CONTINUE__ || \
         (op) == OPCODE___BREAK__ || \
         (op) == OPCODE_RETURNVOID || \
         (op) == OPCODE_RETURNVALUE || \
         (op) == OPCODE___RETHROW__)

static code_t* insert_finally_lookup(code_t*c, code_t*finally, int tempvar)
{
#define NEED_EXTRA_STACK_ARG
    code_t*finally_label = abc_nop(0);
    NEW(lookupswitch_t, l);
    //_lookupswitch

    code_t*i = c;
    int count=0;
    while(i) {
        code_t*prev = i->prev;
        if(IS_FINALLY_TARGET(i->opcode)) {
           code_t*p = prev;
           char needvalue=0;
           if(i->opcode == OPCODE___RETHROW__ ||
              i->opcode == OPCODE_RETURNVALUE) {
               if(i->opcode == OPCODE___RETHROW__)
                 i->opcode = OPCODE_THROW;
               needvalue=1;
               p = abc_coerce_a(p);
               p = abc_setlocal(p, tempvar);
           }
           p = abc_pushbyte(p, count++);
           p = abc_jump(p, finally_label);
           code_t*target = p = abc_label(p);
#ifdef NEED_EXTRA_STACK_ARG
           p = abc_pop(p);
#endif
           if(needvalue) {
               p = abc_getlocal(p, tempvar);
           }

           p->next = i;i->prev = p;
           list_append(l->targets, target);
        }
        i = prev;
    }

    code_t*j,*f;
    c = abc_pushbyte(c, -1);
    c = code_append(c, finally_label);
    c = code_append(c, finally);

#ifdef NEED_EXTRA_STACK_ARG
    c = abc_dup(c);
#endif
    c = abc_lookupswitch(c, l);
    c = l->def = abc_label(c);
#ifdef NEED_EXTRA_STACK_ARG
    c = abc_pop(c);
#endif

    return c;
}

static code_t* insert_finally_simple(code_t*c, code_t*finally, int tempvar)
{
    code_t*i = c;
    while(i) {
        code_t*prev = i->prev;
        if(IS_FINALLY_TARGET(i->opcode)) {
           if(i->opcode == OPCODE___RETHROW__)
                i->opcode = OPCODE_THROW;
           code_t*end = code_dup(finally);
           code_t*start = code_start(end);
           if(prev) prev->next = start;
           start->prev = prev;
           i->prev = end;
           end->next = i;
        }
        i = prev;
    }
    return code_append(c, finally);
}

code_t* insert_finally(code_t*c, code_t*finally, int tempvar)
{
    if(!finally)
        return c;
    code_t*i = c;
    char cantdup=0;
    int num_insertion_points=0;
    while(i) {
        if(IS_FINALLY_TARGET(i->opcode))
            num_insertion_points++;
        i = i->prev;
    }
    i = finally;
    int code_size=0;
    while(i) {
        code_size++;
        if(i->branch || i->opcode == OPCODE_LOOKUPSWITCH) {
            cantdup=1;
        }
        i = i->prev;
    }
    int simple_version_cost = (1+num_insertion_points)*code_size;
    int lookup_version_cost = 4*num_insertion_points + 5;

    if(cantdup || simple_version_cost > lookup_version_cost) {
        printf("lookup %d > *%d*\n", simple_version_cost, lookup_version_cost);
        return insert_finally_lookup(c, finally, tempvar);
    } else {
        printf("simple *%d* < %d\n", simple_version_cost, lookup_version_cost);
        return insert_finally_simple(c, finally, tempvar);
    }
}

#define PASS1 }} if(as3_pass == 1) {{
#define PASS1END }} if(as3_pass == 2) {{
#define PASS2 }} if(as3_pass == 2) {{
#define PASS12 }} {{
#define PASS12END }} if(as3_pass == 2) {{

%}

%%

/* ------------ code blocks / statements ---------------- */

PROGRAM: MAYBE_PROGRAM_CODE_LIST

MAYBE_PROGRAM_CODE_LIST: | PROGRAM_CODE_LIST 
PROGRAM_CODE_LIST: PROGRAM_CODE 
                 | PROGRAM_CODE_LIST PROGRAM_CODE

PROGRAM_CODE: PACKAGE_DECLARATION 
            | INTERFACE_DECLARATION 
            | CLASS_DECLARATION
            | FUNCTION_DECLARATION
            | SLOT_DECLARATION
            | PACKAGE_INITCODE
            | CONDITIONAL_COMPILATION '{' MAYBE_PROGRAM_CODE_LIST '}' // conditional compilation
            | ';'

MAYBE_INPACKAGE_CODE_LIST: | INPACKAGE_CODE_LIST
INPACKAGE_CODE_LIST: INPACKAGE_CODE 
                   | INPACKAGE_CODE_LIST INPACKAGE_CODE

INPACKAGE_CODE: INTERFACE_DECLARATION 
              | CLASS_DECLARATION
              | FUNCTION_DECLARATION
              | SLOT_DECLARATION
              | PACKAGE_INITCODE
              | CONDITIONAL_COMPILATION '{' MAYBE_INPACKAGE_CODE_LIST '}' // conditional compilation
              | ';'

MAYBECODE: CODE {$$=$1;}
MAYBECODE: {$$=code_new();}

CODE: CODE CODEPIECE {$$=code_append($1,$2);}
CODE: CODEPIECE {$$=$1;}

// code which may appear outside of methods
CODE_STATEMENT: IMPORT 
CODE_STATEMENT: FOR 
CODE_STATEMENT: FOR_IN 
CODE_STATEMENT: WHILE 
CODE_STATEMENT: DO_WHILE 
CODE_STATEMENT: SWITCH 
CODE_STATEMENT: IF
CODE_STATEMENT: WITH
CODE_STATEMENT: TRY
CODE_STATEMENT: VOIDEXPRESSION 
CODE_STATEMENT: USE_NAMESPACE
CODE_STATEMENT: NAMESPACE_DECLARATION
CODE_STATEMENT: '{' CODE '}' {$$=$2;}
CODE_STATEMENT: '{' '}' {$$=0;}

// code which may appear in methods
CODEPIECE: ';' {$$=0;}
CODEPIECE: CODE_STATEMENT
CODEPIECE: VARIABLE_DECLARATION
CODEPIECE: BREAK
CODEPIECE: CONTINUE
CODEPIECE: RETURN
CODEPIECE: THROW
CODEPIECE: CONDITIONAL_COMPILATION '{' CODE '}' {$$=$3;}

//CODEBLOCK :  '{' CODE '}' {$$=$2;}
//CODEBLOCK :  '{' '}'      {$$=0;}
CODEBLOCK :  CODEPIECE ';'             {$$=$1;}
CODEBLOCK :  CODEPIECE %prec below_semicolon {$$=$1;}

/* ------------ package init code ------------------- */

PACKAGE_INITCODE: CODE_STATEMENT {
    code_t**cc = &global->init->method->body->code;
    *cc = code_append(*cc, $1);
}

/* ------------ conditional compilation ------------- */

CONDITIONAL_COMPILATION: T_IDENTIFIER "::" T_IDENTIFIER 

/* ------------ variables --------------------------- */

MAYBEEXPRESSION : '=' NONCOMMAEXPRESSION {$$=$2;}
                |                {$$.c=abc_pushundefined(0);
                                  $$.t=TYPE_ANY;
                                 }

VARIABLE_DECLARATION : "var" VARIABLE_LIST {$$=$2;}
VARIABLE_DECLARATION : "const" VARIABLE_LIST {$$=$2;}

VARIABLE_LIST: ONE_VARIABLE                   {$$ = $1;}
VARIABLE_LIST: VARIABLE_LIST ',' ONE_VARIABLE {$$ = code_append($1, $3);}

ONE_VARIABLE: T_IDENTIFIER MAYBETYPE MAYBEEXPRESSION
{
PASS12
    if(variable_exists($1))
        syntaxerror("Variable %s already defined", $1);
PASS1
    new_variable($1, 0, 1, 0);
PASS2
   
    if(!is_subtype_of($3.t, $2)) {
        syntaxerror("Can't convert %s to %s", $3.t->name, 
                                              $2->name);
    }

    char slot = 0;
    int index = 0;
    if(state->method->uses_slots) {
        variable_t* v = find_slot(state, $1);
        if(v && !v->init) {
            // this variable is stored in a slot
            v->init = 1;
            v->type = $2;
            slot = 1;
            index = v->index;
        }
    }
    if(!index) {
        index = new_variable($1, $2, 1, 0);
    }

    $$ = slot?abc_getscopeobject(0, 1):0;
    
    if($2) {
        if($3.c->prev || $3.c->opcode != OPCODE_PUSHUNDEFINED) {
            $$ = code_append($$, $3.c);
            $$ = converttype($$, $3.t, $2);
        } else {
            code_free($3.c);
            $$ = defaultvalue($$, $2);
        }
    } else {
        if($3.c->prev || $3.c->opcode != OPCODE_PUSHUNDEFINED) {
            $$ = code_append($$, $3.c);
            $$ = abc_coerce_a($$);
        } else {
            // don't do anything
            code_free($3.c);
            code_free($$);
            $$ = 0;
            break;
        }
    }
    if(slot) {
        $$ = abc_setslot($$, index);
    } else {
        $$ = abc_setlocal($$, index);
    }
}

/* ------------ control flow ------------------------- */

MAYBEELSE:  %prec below_else {$$ = code_new();}
MAYBEELSE: "else" CODEBLOCK {$$=$2;}
//MAYBEELSE: ';' "else" CODEBLOCK {$$=$3;}

IF : "if" '(' {PASS12 new_state();} EXPRESSION ')' CODEBLOCK MAYBEELSE {
     
    $$ = code_new();
    $$ = code_append($$, $4.c);
    code_t*myjmp,*myif = $$ = abc_iffalse($$, 0);
   
    $$ = code_append($$, $6);
    if($7) {
        myjmp = $$ = abc_jump($$, 0);
    }
    myif->branch = $$ = abc_nop($$);
    if($7) {
        $$ = code_append($$, $7);
        myjmp->branch = $$ = abc_nop($$);
    }
    $$ = var_block($$);
    PASS12 old_state();
}

FOR_INIT : {$$=code_new();}
FOR_INIT : VARIABLE_DECLARATION
FOR_INIT : VOIDEXPRESSION

// TODO: why doesn't an %prec above_identifier resolve the r-r conflict here?
//       (I don't see any easy way to revolve this conflict otherwise, as we
//        can't touch VAR_READ without upsetting the precedence about "return")
FOR_IN_INIT : "var" T_IDENTIFIER MAYBETYPE {
    PASS1 $$=$2;new_variable($2,0,1,0);
    PASS2 $$=$2;new_variable($2,$3,1,0);
}
FOR_IN_INIT : T_IDENTIFIER {
    PASS12
    $$=$1;
}

FOR_START : T_FOR '(' {PASS12 new_state();$$.name=$1;$$.each=0;}
FOR_START : T_FOR "each" '(' {PASS12 new_state();$$.name=$1;$$.each=1;}

FOR : FOR_START FOR_INIT ';' EXPRESSION ';' VOIDEXPRESSION ')' CODEBLOCK {
    if($1.each) syntaxerror("invalid syntax: ; not allowed in for each statement");
    $$ = code_new();
    $$ = code_append($$, $2);
    code_t*loopstart = $$ = abc_label($$);
    $$ = code_append($$, $4.c);
    code_t*myif = $$ = abc_iffalse($$, 0);
    $$ = code_append($$, $8);
    code_t*cont = $$ = abc_nop($$);
    $$ = code_append($$, $6);
    $$ = abc_jump($$, loopstart);
    code_t*out = $$ = abc_nop($$);
    breakjumpsto($$, $1.name, out);
    continuejumpsto($$, $1.name, cont);
    myif->branch = out;

    $$ = var_block($$);
    PASS12 old_state();
}

FOR_IN : FOR_START FOR_IN_INIT "in" EXPRESSION ')' CODEBLOCK {
    variable_t*var = find_variable(state, $2);
    char*tmp1name = concat2($2, "__tmp1__");
    int it = new_variable(tmp1name, TYPE_INT, 0, 0);
    char*tmp2name = concat2($2, "__array__");
    int array = new_variable(tmp1name, 0, 0, 0);

    $$ = code_new();
    $$ = code_append($$, $4.c);
    $$ = abc_coerce_a($$);
    $$ = abc_setlocal($$, array);
    $$ = abc_pushbyte($$, 0);
    $$ = abc_setlocal($$, it);

    code_t*loopstart = $$ = abc_label($$);
    
    $$ = abc_hasnext2($$, array, it);
    code_t*myif = $$ = abc_iffalse($$, 0);
    $$ = abc_getlocal($$, array);
    $$ = abc_getlocal($$, it);
    if(!$1.each)
        $$ = abc_nextname($$);
    else
        $$ = abc_nextvalue($$);
    $$ = converttype($$, 0, var->type);
    $$ = abc_setlocal($$, var->index);

    $$ = code_append($$, $6);
    $$ = abc_jump($$, loopstart);
    
    code_t*out = $$ = abc_nop($$);
    breakjumpsto($$, $1.name, out);
    continuejumpsto($$, $1.name, loopstart);
    
    myif->branch = out;

    $$ = var_block($$);

    free(tmp1name);
    free(tmp2name);

    PASS12 old_state();
}

WHILE : T_WHILE '(' {PASS12 new_state();} EXPRESSION ')' CODEBLOCK {

    $$ = code_new();

    code_t*myjmp = $$ = abc_jump($$, 0);
    code_t*loopstart = $$ = abc_label($$);
    $$ = code_append($$, $6);
    code_t*cont = $$ = abc_nop($$);
    myjmp->branch = cont;
    $$ = code_append($$, $4.c);
    $$ = abc_iftrue($$, loopstart);
    code_t*out = $$ = abc_nop($$);
    breakjumpsto($$, $1, out);
    continuejumpsto($$, $1, cont);

    $$ = var_block($$);
    PASS12 old_state();
}

DO_WHILE : T_DO {PASS12 new_state();} CODEBLOCK "while" '(' EXPRESSION ')' {
    $$ = code_new();
    code_t*loopstart = $$ = abc_label($$);
    $$ = code_append($$, $3);
    code_t*cont = $$ = abc_nop($$);
    $$ = code_append($$, $6.c);
    $$ = abc_iftrue($$, loopstart);
    code_t*out = $$ = abc_nop($$);
    breakjumpsto($$, $1, out);
    continuejumpsto($$, $1, cont);
    
    $$ = var_block($$);
    PASS12 old_state();
}

BREAK : "break" %prec prec_none {
    $$ = abc___break__(0, "");
}
BREAK : "break" T_IDENTIFIER {
    $$ = abc___break__(0, $2);
}
CONTINUE : "continue" %prec prec_none {
    $$ = abc___continue__(0, "");
}
CONTINUE : "continue" T_IDENTIFIER {
    $$ = abc___continue__(0, $2);
}

MAYBE_CASE_LIST :           {$$=0;}
MAYBE_CASE_LIST : CASE_LIST {$$=$1;}
MAYBE_CASE_LIST : DEFAULT   {$$=$1;}
MAYBE_CASE_LIST : CASE_LIST DEFAULT {$$=code_append($1,$2);}
CASE_LIST: CASE             {$$=$1;}
CASE_LIST: CASE_LIST CASE   {$$=code_append($$,$2);}

CASE: "case" E ':' MAYBECODE {
    $$ = abc_dup(0);
    $$ = code_append($$, $2.c);
    code_t*j = $$ = abc_ifne($$, 0);
    $$ = code_append($$, $4);
    if($$->opcode != OPCODE___BREAK__) {
        $$ = abc___fallthrough__($$, "");
    }
    code_t*e = $$ = abc_nop($$);
    j->branch = e;
}
DEFAULT: "default" ':' MAYBECODE {
    $$ = $3;
}
SWITCH : T_SWITCH '(' {PASS12 new_state();} E ')' '{' MAYBE_CASE_LIST '}' {
    $$=$4.c;
    $$ = code_append($$, $7);
    code_t*out = $$ = abc_pop($$);
    breakjumpsto($$, $1, out);
    
    code_t*c = $$,*lastblock=0;
    while(c) {
        if(c->opcode == OPCODE_IFNE) {
            if(!c->next) syntaxerror("internal error in fallthrough handling");
            lastblock=c->next;
        } else if(c->opcode == OPCODE___FALLTHROUGH__) {
            if(lastblock) {
                c->opcode = OPCODE_JUMP;
                c->branch = lastblock;
            } else {
                /* fall through end of switch */
                c->opcode = OPCODE_NOP;
            }
        }
        c=c->prev;
    }
   
    $$ = var_block($$);
    PASS12 old_state();
}

/* ------------ try / catch /finally ---------------- */

CATCH: "catch" '(' T_IDENTIFIER MAYBETYPE ')' {PASS12 new_state();
                                                      state->exception_name=$3;
                                               PASS1 new_variable($3, 0, 0, 0);
                                               PASS2 new_variable($3, $4, 0, 0);
                                              } 
        '{' MAYBECODE '}' {
    namespace_t name_ns = {ACCESS_PACKAGE, ""};
    multiname_t name = {QNAME, &name_ns, 0, $3};
    
    NEW(abc_exception_t, e)
    e->exc_type = sig2mname($4);
    e->var_name = multiname_clone(&name);
    $$ = e;

    code_t*c = 0;
    int i = find_variable_safe(state, $3)->index;
    e->target = c = abc_nop(0);
    c = abc_setlocal(c, i);
    c = code_append(c, $8);
    c = abc_kill(c, i);

    c = var_block(c);
    PASS12 old_state();
}
FINALLY: "finally" '{' {PASS12 new_state();state->exception_name=0;} MAYBECODE '}' {
    $4 = var_block($4);
    if(!$4) {
        $$=0;
    } else {
        NEW(abc_exception_t, e)
        e->exc_type = 0; //all exceptions
        e->var_name = 0; //no name
        e->target = 0;
        e->to = abc_nop(0);
        e->to = code_append(e->to, $4);
        $$ = e;
    }
    PASS12 old_state();
}

CATCH_LIST: CATCH {$$.l=list_new();$$.finally=0;list_append($$.l,$1);}
CATCH_LIST: CATCH_LIST CATCH {$$=$1;list_append($$.l,$2);}
CATCH_FINALLY_LIST: CATCH_LIST {$$=$1;}
CATCH_FINALLY_LIST: CATCH_LIST FINALLY {
    $$ = $1;
    $$.finally = 0;
    if($2) {
        list_append($$.l,$2);
        $$.finally = $2->to;$2->to=0;
    }
}
CATCH_FINALLY_LIST: FINALLY {
    $$.l=list_new();
    $$.finally = 0;
    if($1) {
        list_append($$.l,$1);
        $$.finally = $1->to;$1->to=0;
    }
}

TRY : "try" '{' {PASS12 new_state();} MAYBECODE '}' CATCH_FINALLY_LIST {
    code_t*out = abc_nop(0);

    code_t*start = abc_nop(0);
    $$ = code_append(start, $4);
    if(!is_break_or_jump($4)) {
        $$ = abc_jump($$, out);
    }
    code_t*end = $$ = abc_nop($$);
  
    int tmp;
    if($6.finally)
        tmp = new_variable("__finally__", 0, 0, 0);
    
    abc_exception_list_t*l = $6.l;
    int count=0;
    while(l) {
        abc_exception_t*e = l->abc_exception;
        if(e->var_name) {
            $$ = code_append($$, e->target);
            $$ = abc_jump($$, out);
        } else {
            parserassert((ptroff_t)$6.finally);
            // finally block
            e->target = $$ = abc_nop($$);
            $$ = abc___rethrow__($$);
        }
        
        e->from = start;
        e->to = end;

        l = l->next;
    }
    $$ = code_append($$, out);

    $$ = insert_finally($$, $6.finally, tmp);
        
    list_concat(state->method->exceptions, $6.l);
   
    $$ = var_block($$);
    PASS12 old_state();
}

/* ------------ throw ------------------------------- */

THROW : "throw" EXPRESSION {
    $$=$2.c;
    $$=abc_throw($$);
}
THROW : "throw" %prec prec_none {
    if(!state->exception_name)
        syntaxerror("re-throw only possible within a catch block");
    variable_t*v = find_variable(state, state->exception_name);
    $$=code_new();
    $$=abc_getlocal($$, v->index);
    $$=abc_throw($$);
}

/* ------------ with -------------------------------- */

WITH : "with" '(' EXPRESSION ')' CODEBLOCK {
     $$ = $3.c;
     $$ = abc_pushscope($$);
     $$ = code_append($$, $5);
     $$ = abc_popscope($$);
}

/* ------------ packages and imports ---------------- */

X_IDENTIFIER: T_IDENTIFIER
            | "package" {PASS12 $$="package";}

PACKAGE: PACKAGE '.' X_IDENTIFIER {PASS12 $$ = concat3($1,".",$3);free($1);$1=0;}
PACKAGE: X_IDENTIFIER             {PASS12 $$=strdup($1);}

PACKAGE_DECLARATION : "package" PACKAGE '{' {PASS12 startpackage($2);free($2);$2=0;}
                                MAYBE_INPACKAGE_CODE_LIST '}' {PASS12 endpackage();$$=0;}
PACKAGE_DECLARATION : "package" '{' {PASS12 startpackage("");} 
                                MAYBE_INPACKAGE_CODE_LIST '}' {PASS12 endpackage();$$=0;}

IMPORT : "import" PACKAGEANDCLASS {
       PASS12
       slotinfo_t*s = registry_find($2->package, $2->name);
       if(!s) {// || !(s->flags&FLAG_BUILTIN)) {
           as3_schedule_class($2->package, $2->name);
       }

       PASS2
       classinfo_t*c = $2;
       if(!c) 
            syntaxerror("Couldn't import class\n");
       state_has_imports();
       dict_put(state->imports, c->name, c);
       $$=0;
}
IMPORT : "import" PACKAGE '.' '*' {
       PASS12
       if(strncmp("flash.", $2, 6)) {
           as3_schedule_package($2);
       }

       PASS2
       NEW(import_t,i);
       i->package = $2;
       state_has_imports();
       list_append(state->wildcard_imports, i);
       $$=0;
}

/* ------------ classes and interfaces (header) -------------- */

MAYBE_MODIFIERS : %prec above_function {PASS12 $$.flags=0;$$.ns=0;}
MAYBE_MODIFIERS : MODIFIER_LIST        {PASS12 $$=$1;}
MODIFIER_LIST : MODIFIER               {PASS12 $$=$1;}
MODIFIER_LIST : MODIFIER_LIST MODIFIER {
    PASS12 
    $$.flags=$1.flags|$2.flags;
    if($1.ns && $2.ns) syntaxerror("only one namespace allowed in one declaration");
    $$.ns=$1.ns?$1.ns:$2.ns;

}

MODIFIER : KW_PUBLIC {PASS12 $$.flags=FLAG_PUBLIC;$$.ns=0;}
         | KW_PRIVATE {PASS12 $$.flags=FLAG_PRIVATE;$$.ns=0;}
         | KW_PROTECTED {PASS12 $$.flags=FLAG_PROTECTED;$$.ns=0;}
         | KW_STATIC {PASS12 $$.flags=FLAG_STATIC;$$.ns=0;}
         | KW_DYNAMIC {PASS12 $$.flags=FLAG_DYNAMIC;$$.ns=0;}
         | KW_FINAL {PASS12 $$.flags=FLAG_FINAL;$$.ns=0;}
         | KW_OVERRIDE {PASS12 $$.flags=FLAG_OVERRIDE;$$.ns=0;}
         | KW_NATIVE {PASS12 $$.flags=FLAG_NATIVE;$$.ns=0;}
         | KW_INTERNAL {PASS12 $$.flags=FLAG_PACKAGEINTERNAL;$$.ns=0;}
         | T_NAMESPACE {PASS12 $$.flags=FLAG_NAMESPACE;
                               $$.ns=$1;
                       }

EXTENDS : {$$=registry_getobjectclass();}
EXTENDS : KW_EXTENDS CLASS_SPEC {$$=$2;}

EXTENDS_LIST : {PASS12 $$=list_new();}
EXTENDS_LIST : KW_EXTENDS CLASS_SPEC_LIST {PASS12 $$=$2;}

IMPLEMENTS_LIST : {PASS12 $$=list_new();}
IMPLEMENTS_LIST : KW_IMPLEMENTS CLASS_SPEC_LIST {PASS12 $$=$2;}

CLASS_DECLARATION : MAYBE_MODIFIERS "class" T_IDENTIFIER 
                              EXTENDS IMPLEMENTS_LIST 
                              '{' {PASS12 startclass(&$1,$3,$4,$5);} 
                              MAYBE_CLASS_BODY 
                              '}' {PASS12 endclass();$$=0;}

INTERFACE_DECLARATION : MAYBE_MODIFIERS "interface" T_IDENTIFIER 
                              EXTENDS_LIST 
                              '{' {PASS12 $1.flags|=FLAG_INTERFACE;
                                          startclass(&$1,$3,0,$4);}
                              MAYBE_INTERFACE_BODY 
                              '}' {PASS12 endclass();$$=0;}

/* ------------ classes and interfaces (body) -------------- */

MAYBE_CLASS_BODY : 
MAYBE_CLASS_BODY : CLASS_BODY
CLASS_BODY : CLASS_BODY_ITEM
CLASS_BODY : CLASS_BODY CLASS_BODY_ITEM
CLASS_BODY_ITEM : ';'
CLASS_BODY_ITEM : CONDITIONAL_COMPILATION '{' MAYBE_CLASS_BODY '}'
CLASS_BODY_ITEM : SLOT_DECLARATION
CLASS_BODY_ITEM : FUNCTION_DECLARATION

CLASS_BODY_ITEM : CODE_STATEMENT {
    code_t*c = state->cls->static_init->header;
    c = code_append(c, $1);  
    state->cls->static_init->header = c;
}

MAYBE_INTERFACE_BODY : 
MAYBE_INTERFACE_BODY : INTERFACE_BODY
INTERFACE_BODY : IDECLARATION
INTERFACE_BODY : INTERFACE_BODY IDECLARATION
IDECLARATION : ';'
IDECLARATION : "var" T_IDENTIFIER {
    syntaxerror("variable declarations not allowed in interfaces");
}
IDECLARATION : MAYBE_MODIFIERS "function" GETSET T_IDENTIFIER '(' MAYBE_PARAM_LIST ')' MAYBETYPE {
    PASS12
    $1.flags |= FLAG_PUBLIC;
    if($1.flags&(FLAG_PRIVATE|FLAG_PACKAGEINTERNAL|FLAG_PROTECTED)) {
        syntaxerror("invalid method modifiers: interface methods always need to be public");
    }
    startfunction(&$1,$3,$4,&$6,$8);
    endfunction(&$1,$3,$4,&$6,$8, 0);
    list_deep_free($6.list);
}

/* ------------ classes and interfaces (body, slots ) ------- */

VARCONST: "var" | "const"

SLOT_DECLARATION: MAYBE_MODIFIERS VARCONST T_IDENTIFIER {setstaticfunction($1.flags);} MAYBETYPE MAYBEEXPRESSION {
    int flags = $1.flags;
    namespace_t ns = modifiers2access(&$1);

    varinfo_t* info = 0;
    if(state->cls) {
        memberinfo_t*i = registry_findmember(state->cls->info, ns.name, $3, 1);
        if(i) {
            check_override(i, flags);
        }
        info = varinfo_register_onclass(state->cls->info, ns.access, ns.name, $3);
    } else {
        slotinfo_t*i = registry_find(state->package, $3);
        if(i) {
            syntaxerror("package %s already contains '%s'", state->package, $3);
        }
        if(ns.name && ns.name[0]) {
            syntaxerror("namespaces not allowed on package-level variables");
        }
        info = varinfo_register_global(ns.access, state->package, $3);
    }

    info->type = $5;
    info->flags = flags;

    /* slot name */
    multiname_t mname = {QNAME, &ns, 0, $3};
  
    trait_list_t**traits;
    code_t**code;
    if(!state->cls) {
        // global variable
        ns.name = state->package;
        traits = &global->init->traits;
        code = &global->init->method->body->code;
    } else if(flags&FLAG_STATIC) {
        // static variable
        traits = &state->cls->abc->static_traits;
        code = &state->cls->static_init->header;
    } else {
        // instance variable
        traits = &state->cls->abc->traits;
        code = &state->cls->init->header;
    }
    
    trait_t*t=0;
    if($5) {
        MULTINAME(m, $5);
        t = trait_new_member(traits, multiname_clone(&m), multiname_clone(&mname), 0);
    } else {
        t = trait_new_member(traits, 0, multiname_clone(&mname), 0);
    }
    info->slot = t->slot_id;
    
    /* initalization code (if needed) */
    code_t*c = 0;
    if($6.c && !is_pushundefined($6.c)) {
        c = abc_getlocal_0(c);
        c = code_append(c, $6.c);
        c = converttype(c, $6.t, $5);
        c = abc_setslot(c, t->slot_id);
    }

    *code = code_append(*code, c);

    if($2==KW_CONST) {
        t->kind= TRAIT_CONST;
    }

    $$=0;
    setstaticfunction(0);
}

/* ------------ constants -------------------------------------- */

MAYBESTATICCONSTANT: {$$=0;}
MAYBESTATICCONSTANT: '=' STATICCONSTANT {$$=$2;}

STATICCONSTANT : T_BYTE {$$ = constant_new_int($1);}
STATICCONSTANT : T_INT {$$ = constant_new_int($1);}
STATICCONSTANT : T_UINT {$$ = constant_new_uint($1);}
STATICCONSTANT : T_FLOAT {$$ = constant_new_float($1);}
STATICCONSTANT : T_STRING {$$ = constant_new_string2($1.str,$1.len);free((char*)$1.str);}
//STATICCONSTANT : T_NAMESPACE {$$ = constant_new_namespace($1);}
STATICCONSTANT : "true" {$$ = constant_new_true($1);}
STATICCONSTANT : "false" {$$ = constant_new_false($1);}
STATICCONSTANT : "null" {$$ = constant_new_null($1);}
STATICCONSTANT : T_IDENTIFIER {
    // TODO
    as3_warning("Couldn't resolve %s", $1);
    $$ = constant_new_null($1);
}

/* ------------ classes and interfaces (body, functions) ------- */

// non-vararg version
MAYBE_PARAM_LIST: {
    PASS12
    memset(&$$,0,sizeof($$));
}
MAYBE_PARAM_LIST: PARAM_LIST {
    PASS12
    $$=$1;
}

// vararg version
MAYBE_PARAM_LIST: "..." PARAM {
    PASS12
    memset(&$$,0,sizeof($$));
    $$.varargs=1;
    list_append($$.list, $2);
}
MAYBE_PARAM_LIST: PARAM_LIST ',' "..." PARAM {
    PASS12
    $$ =$1;
    $$.varargs=1;
    list_append($$.list, $4);
}

// non empty
PARAM_LIST: PARAM_LIST ',' PARAM {
    PASS12
    $$ = $1;
    list_append($$.list, $3);
}
PARAM_LIST: PARAM {
    PASS12
    memset(&$$,0,sizeof($$));
    list_append($$.list, $1);
}

PARAM:  T_IDENTIFIER ':' TYPE MAYBESTATICCONSTANT {
     PASS12
     $$ = rfx_calloc(sizeof(param_t));
     $$->name=$1;
     $$->type = $3;
     PASS2
     $$->value = $4;
}
PARAM:  T_IDENTIFIER MAYBESTATICCONSTANT {
     PASS12
     $$ = rfx_calloc(sizeof(param_t));
     $$->name=$1;
     $$->type = TYPE_ANY;
     PASS2
     $$->value = $2;
}
GETSET : "get"
       | "set"
       | {PASS12 $$=0;}

FUNCTION_DECLARATION: MAYBE_MODIFIERS "function" GETSET T_IDENTIFIER '(' MAYBE_PARAM_LIST ')' 
                      MAYBETYPE '{' {PASS12 startfunction(&$1,$3,$4,&$6,$8);} MAYBECODE '}' 
{
    PASS1 
    endfunction(&$1,$3,$4,&$6,0,0);
    PASS2
    if(!state->method->info) syntaxerror("internal error");
    
    code_t*c = method_header(state->method);
    c = wrap_function(c, 0, $11);

    endfunction(&$1,$3,$4,&$6,$8,c);
    PASS12
    list_deep_free($6.list);
    $$=0;
}

MAYBE_IDENTIFIER: T_IDENTIFIER
MAYBE_IDENTIFIER: {PASS12 $$=0;}
INNERFUNCTION: "function" MAYBE_IDENTIFIER '(' MAYBE_PARAM_LIST ')' MAYBETYPE 
               '{' {PASS12 innerfunction($2,&$4,$6);} MAYBECODE '}'
{
    PASS1
    endfunction(0,0,$2,&$4,0,0);
    PASS2
    methodinfo_t*f = state->method->info;
    if(!f || !f->kind) syntaxerror("internal error");
    
    code_t*c = method_header(state->method);
    c = wrap_function(c, 0, $9);

    int index = state->method->var_index;
    endfunction(0,0,$2,&$4,$6,c);
    
    $$.c = abc_getlocal(0, index);
    $$.t = TYPE_FUNCTION(f);

    PASS12 list_deep_free($4.list);
}


/* ------------- package + class ids --------------- */

CLASS: T_IDENTIFIER {
    PASS1 static slotinfo_t c;
          memset(&c, 0, sizeof(c));
          c.name = $1;
          $$ = (classinfo_t*)&c;
    PASS2
    slotinfo_t*s = find_class($1);
    if(!s) syntaxerror("Could not find class/method %s (current package: %s)\n", $1, state->package);
    $$ = (classinfo_t*)s;
}

PACKAGEANDCLASS : PACKAGE '.' T_IDENTIFIER {
    PASS1 static slotinfo_t c;
          memset(&c, 0, sizeof(c));
          c.package = $1;
          c.name = $3;
          $$=(classinfo_t*)&c;
    PASS2
    slotinfo_t*s = registry_find($1, $3);
    if(!s) syntaxerror("Couldn't find class/method %s.%s\n", $1, $3);
    free($1);$1=0;
    $$ = (classinfo_t*)s;
}

CLASS_SPEC: PACKAGEANDCLASS
          | CLASS

CLASS_SPEC_LIST : CLASS_SPEC {PASS12 $$=list_new();list_append($$, $1);}
CLASS_SPEC_LIST : CLASS_SPEC_LIST ',' CLASS_SPEC {PASS12 $$=$1;list_append($$,$3);}

TYPE : CLASS_SPEC {$$=$1;}
     | '*'        {$$=registry_getanytype();}
     | "void"     {$$=registry_getanytype();}
    /*
     |  "String"  {$$=registry_getstringclass();}
     |  "int"     {$$=registry_getintclass();}
     |  "uint"    {$$=registry_getuintclass();}
     |  "Boolean" {$$=registry_getbooleanclass();}
     |  "Number"  {$$=registry_getnumberclass();}
    */

MAYBETYPE: ':' TYPE {$$=$2;}
MAYBETYPE:          {$$=0;}

/* ----------function calls, delete, constructor calls ------ */

MAYBE_PARAM_VALUES :  %prec prec_none {$$.cc=0;$$.len=0;}
MAYBE_PARAM_VALUES : '(' MAYBE_EXPRESSION_LIST ')' {$$=$2;}

MAYBE_EXPRESSION_LIST : {$$.cc=0;$$.len=0;}
MAYBE_EXPRESSION_LIST : EXPRESSION_LIST
MAYBE_EXPRESSION_LIST : EXPRESSION_LIST_AND_COMMA

EXPRESSION_LIST : NONCOMMAEXPRESSION             {$$.len=1;
                                                  $$.cc = $1.c;
                                                 }

EXPRESSION_LIST_AND_COMMA: EXPRESSION_LIST ',' {$$ = $1;}
EXPRESSION_LIST : EXPRESSION_LIST_AND_COMMA NONCOMMAEXPRESSION {
                                                  $$.len= $1.len+1;
                                                  $$.cc = code_append($1.cc, $2.c);
                                                  }
               
XX : %prec new2
NEW : "new" E XX MAYBE_PARAM_VALUES {
    $$.c = $2.c;
    if($$.c->opcode == OPCODE_COERCE_A) $$.c = code_cutlast($$.c);
    
    code_t*paramcode = $4.cc;
    if($$.c->opcode == OPCODE_GETPROPERTY) {
        multiname_t*name = $$.c->data[0];$$.c->data[0]=0;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_constructprop2($$.c, name, $4.len);
        multiname_destroy(name);
    } else if($$.c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)$$.c->data[0];
        trait_t*t = traits_find_slotid(state->cls->abc->traits,slot);//FIXME
        multiname_t*name = t->name;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_constructprop2($$.c, name, $4.len);
    } else {
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_construct($$.c, $4.len);
    }
   
    $$.t = TYPE_ANY;
    if(TYPE_IS_CLASS($2.t) && $2.t->data) {
        $$.t = $2.t->data;
    } else {
        $$.c = abc_coerce_a($$.c);
        $$.t = TYPE_ANY;
    }
}

/* TODO: use abc_call (for calling local variables),
         abc_callstatic (for calling own methods) 
         call (for closures)
*/
FUNCTIONCALL : E '(' MAYBE_EXPRESSION_LIST ')' {
    
    $$.c = $1.c;
    if($$.c->opcode == OPCODE_COERCE_A) {
        $$.c = code_cutlast($$.c);
    }
    code_t*paramcode = $3.cc;

    $$.t = TYPE_ANY;
    if($$.c->opcode == OPCODE_GETPROPERTY) {
        multiname_t*name = $$.c->data[0];$$.c->data[0]=0;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_callproperty2($$.c, name, $3.len);
        multiname_destroy(name);
    } else if($$.c->opcode == OPCODE_GETSLOT && $$.c->prev->opcode != OPCODE_GETSCOPEOBJECT) {
        int slot = (int)(ptroff_t)$$.c->data[0];
        trait_t*t = traits_find_slotid(state->cls->abc->traits,slot);
        if(t->kind!=TRAIT_METHOD) {
            //ok: flash allows to assign closures to members.
        }
        multiname_t*name = t->name;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        //$$.c = abc_callmethod($$.c, t->method, len); //#1051 illegal early access binding
        $$.c = abc_callproperty2($$.c, name, $3.len);
    } else if($$.c->opcode == OPCODE_GETSUPER) {
        multiname_t*name = $$.c->data[0];$$.c->data[0]=0;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_callsuper2($$.c, name, $3.len);
        multiname_destroy(name);
    } else {
        $$.c = abc_getglobalscope($$.c);
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_call($$.c, $3.len);
    }
   
    if(TYPE_IS_FUNCTION($1.t) && $1.t->data) {
        $$.t = ((methodinfo_t*)($1.t->data))->return_type;
    } else {
        $$.c = abc_coerce_a($$.c);
        $$.t = TYPE_ANY;
    }
}

FUNCTIONCALL : "super" '(' MAYBE_EXPRESSION_LIST ')' {
    if(!state->cls) syntaxerror("super() not allowed outside of a class");
    if(!state->method) syntaxerror("super() not allowed outside of a function");
    if(!state->method->is_constructor) syntaxerror("super() not allowed outside of a constructor");

    $$.c = code_new();
    $$.c = abc_getlocal_0($$.c);

    $$.c = code_append($$.c, $3.cc);
    /*
    this is dependent on the control path, check this somewhere else
    if(state->method->has_super)
        syntaxerror("constructor may call super() only once");
    */
    state->method->has_super = 1;

    $$.c = abc_constructsuper($$.c, $3.len);
    $$.c = abc_pushundefined($$.c);
    $$.t = TYPE_ANY;
}

DELETE: "delete" E {
    $$.c = $2.c;
    if($$.c->opcode == OPCODE_COERCE_A) {
        $$.c = code_cutlast($$.c);
    }
    multiname_t*name = 0;
    if($$.c->opcode == OPCODE_GETPROPERTY) {
        $$.c->opcode = OPCODE_DELETEPROPERTY;
    } else if($$.c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)$$.c->data[0];
        multiname_t*name = traits_find_slotid(state->cls->abc->traits,slot)->name;
        $$.c = code_cutlast($$.c);
        $$.c = abc_deleteproperty2($$.c, name);
    } else {
        $$.c = abc_getlocal_0($$.c);
        MULTINAME_LATE(m, $2.t?$2.t->access:ACCESS_PACKAGE, "");
        $$.c = abc_deleteproperty2($$.c, &m);
    }
    $$.t = TYPE_BOOLEAN;
}

RETURN: "return" %prec prec_none {
    $$ = abc_returnvoid(0);
}
RETURN: "return" EXPRESSION {
    $$ = $2.c;
    $$ = abc_returnvalue($$);
}

// ----------------------- expression types -------------------------------------

NONCOMMAEXPRESSION : E        %prec below_minus {$$=$1;}
EXPRESSION : E                %prec below_minus {$$ = $1;}
EXPRESSION : EXPRESSION ',' E %prec below_minus {
    $$.c = $1.c;
    $$.c = cut_last_push($$.c);
    $$.c = code_append($$.c,$3.c);
    $$.t = $3.t;
}
VOIDEXPRESSION : EXPRESSION %prec below_minus {
    $$=cut_last_push($1.c);
}

// ----------------------- expression evaluation -------------------------------------

E : INNERFUNCTION %prec prec_none {$$ = $1;}
//V : CONSTANT                    {$$ = 0;}
E : CONSTANT
//V : VAR_READ %prec T_IDENTIFIER {$$ = 0;}
E : VAR_READ %prec T_IDENTIFIER {$$ = $1;}
//V : NEW                         {$$ = $1.c;}
E : NEW                         {$$ = $1;}
//V : DELETE                      {$$ = $1.c;}
E : DELETE                      {$$ = $1;}

E : FUNCTIONCALL

E : T_REGEXP {
    $$.c = 0;
    namespace_t ns = {ACCESS_PACKAGE, ""};
    multiname_t m = {QNAME, &ns, 0, "RegExp"};
    if(!$1.options) {
        $$.c = abc_getlex2($$.c, &m);
        $$.c = abc_pushstring($$.c, $1.pattern);
        $$.c = abc_construct($$.c, 1);
    } else {
        $$.c = abc_getlex2($$.c, &m);
        $$.c = abc_pushstring($$.c, $1.pattern);
        $$.c = abc_pushstring($$.c, $1.options);
        $$.c = abc_construct($$.c, 2);
    }
    $$.t = TYPE_REGEXP;
}

CONSTANT : T_BYTE {$$.c = abc_pushbyte(0, $1);
                   //MULTINAME(m, registry_getintclass());
                   //$$.c = abc_coerce2($$.c, &m); // FIXME
                   $$.t = TYPE_INT;
                  }
CONSTANT : T_SHORT {$$.c = abc_pushshort(0, $1);
                    $$.t = TYPE_INT;
                   }
CONSTANT : T_INT {$$.c = abc_pushint(0, $1);
                  $$.t = TYPE_INT;
                 }
CONSTANT : T_UINT {$$.c = abc_pushuint(0, $1);
                   $$.t = TYPE_UINT;
                  }
CONSTANT : T_FLOAT {$$.c = abc_pushdouble(0, $1);
                    $$.t = TYPE_FLOAT;
                   }
CONSTANT : T_STRING {$$.c = abc_pushstring2(0, &$1);free((char*)$1.str);
                     $$.t = TYPE_STRING;
                    }
CONSTANT : "undefined" {$$.c = abc_pushundefined(0);
                    $$.t = TYPE_ANY;
                   }
CONSTANT : "true" {$$.c = abc_pushtrue(0);
                    $$.t = TYPE_BOOLEAN;
                   }
CONSTANT : "false" {$$.c = abc_pushfalse(0);
                     $$.t = TYPE_BOOLEAN;
                    }
CONSTANT : "null" {$$.c = abc_pushnull(0);
                    $$.t = TYPE_NULL;
                   }

E : E '<' E {$$.c = code_append($1.c,$3.c);$$.c = abc_greaterequals($$.c);$$.c=abc_not($$.c);
             $$.t = TYPE_BOOLEAN;
            }
E : E '>' E {$$.c = code_append($1.c,$3.c);$$.c = abc_greaterthan($$.c);
             $$.t = TYPE_BOOLEAN;
            }
E : E "<=" E {$$.c = code_append($1.c,$3.c);$$.c = abc_greaterthan($$.c);$$.c=abc_not($$.c);
              $$.t = TYPE_BOOLEAN;
             }
E : E ">=" E {$$.c = code_append($1.c,$3.c);$$.c = abc_greaterequals($$.c);
              $$.t = TYPE_BOOLEAN;
             }
E : E "==" E {$$.c = code_append($1.c,$3.c);$$.c = abc_equals($$.c);
              $$.t = TYPE_BOOLEAN;
             }
E : E "===" E {$$.c = code_append($1.c,$3.c);$$.c = abc_strictequals($$.c);
              $$.t = TYPE_BOOLEAN;
              }
E : E "!==" E {$$.c = code_append($1.c,$3.c);$$.c = abc_strictequals($$.c);$$.c = abc_not($$.c);
              $$.t = TYPE_BOOLEAN;
             }
E : E "!=" E {$$.c = code_append($1.c,$3.c);$$.c = abc_equals($$.c);$$.c = abc_not($$.c);
              $$.t = TYPE_BOOLEAN;
             }

E : E "||" E {$$.t = join_types($1.t, $3.t, 'O');
              $$.c = $1.c;
              $$.c = converttype($$.c, $1.t, $$.t);
              $$.c = abc_dup($$.c);
              code_t*jmp = $$.c = abc_iftrue($$.c, 0);
              $$.c = cut_last_push($$.c);
              $$.c = code_append($$.c,$3.c);
              $$.c = converttype($$.c, $3.t, $$.t);
              code_t*label = $$.c = abc_label($$.c);
              jmp->branch = label;
             }
E : E "&&" E {
              $$.t = join_types($1.t, $3.t, 'A');
              /*printf("%08x:\n",$1.t);
              code_dump($1.c, 0, 0, "", stdout);
              printf("%08x:\n",$3.t);
              code_dump($3.c, 0, 0, "", stdout);
              printf("joining %08x and %08x to %08x\n", $1.t, $3.t, $$.t);*/
              $$.c = $1.c;
              $$.c = converttype($$.c, $1.t, $$.t);
              $$.c = abc_dup($$.c);
              code_t*jmp = $$.c = abc_iffalse($$.c, 0);
              $$.c = cut_last_push($$.c);
              $$.c = code_append($$.c,$3.c);
              $$.c = converttype($$.c, $3.t, $$.t);
              code_t*label = $$.c = abc_label($$.c);
              jmp->branch = label;              
             }

E : '!' E    {$$.c=$2.c;
              $$.c = abc_not($$.c);
              $$.t = TYPE_BOOLEAN;
             }

E : '~' E    {$$.c=$2.c;
              $$.c = abc_bitnot($$.c);
              $$.t = TYPE_INT;
             }

E : E '&' E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_bitand($$.c);
             $$.t = TYPE_INT;
            }

E : E '^' E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_bitxor($$.c);
             $$.t = TYPE_INT;
            }

E : E '|' E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_bitor($$.c);
             $$.t = TYPE_INT;
            }

E : E ">>" E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_rshift($$.c);
             $$.t = TYPE_INT;
            }
E : E ">>>" E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_urshift($$.c);
             $$.t = TYPE_INT;
            }
E : E "<<" E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_lshift($$.c);
             $$.t = TYPE_INT;
            }

E : E '/' E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_divide($$.c);
             $$.t = TYPE_NUMBER;
            }
E : E '%' E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_modulo($$.c);
             $$.t = TYPE_NUMBER;
            }
E : E '+' E {$$.c = code_append($1.c,$3.c);
             if(BOTH_INT($1.t, $3.t)) {
                $$.c = abc_add_i($$.c);
                $$.t = TYPE_INT;
             } else {
                $$.c = abc_add($$.c);
                $$.t = join_types($1.t,$3.t,'+');
             }
            }
E : E '-' E {$$.c = code_append($1.c,$3.c);
             if(BOTH_INT($1.t,$3.t)) {
                $$.c = abc_subtract_i($$.c);
                $$.t = TYPE_INT;
             } else {
                $$.c = abc_subtract($$.c);
                $$.t = TYPE_NUMBER;
             }
            }
E : E '*' E {$$.c = code_append($1.c,$3.c);
             if(BOTH_INT($1.t,$3.t)) {
                $$.c = abc_multiply_i($$.c);
                $$.t = TYPE_INT;
             } else {
                $$.c = abc_multiply($$.c);
                $$.t = TYPE_NUMBER;
             }
            }

E : E "in" E {$$.c = code_append($1.c,$3.c);
              $$.c = abc_in($$.c);
              $$.t = TYPE_BOOLEAN;
             }

E : E "as" E {char use_astype=0; // flash player's astype works differently than astypelate
              if(use_astype && TYPE_IS_CLASS($3.t) && $3.t->data) {
                MULTINAME(m, (classinfo_t*)($3.t->data));
                $$.c = abc_astype2($1.c, &m);
                $$.t = $3.t->data;
              } else {
                $$.c = code_append($1.c, $3.c);
                $$.c = abc_astypelate($$.c);
                $$.t = TYPE_ANY;
              }
             }

E : E "instanceof" E 
             {$$.c = code_append($1.c, $3.c);
              $$.c = abc_instanceof($$.c);
              $$.t = TYPE_BOOLEAN;
             }

E : E "is" E {$$.c = code_append($1.c, $3.c);
              $$.c = abc_istypelate($$.c);
              $$.t = TYPE_BOOLEAN;
             }

E : "typeof" '(' E ')' {
              $$.c = $3.c;
              $$.c = abc_typeof($$.c);
              $$.t = TYPE_STRING;
             }

E : "void" E {
              $$.c = cut_last_push($2.c);
              $$.c = abc_pushundefined($$.c);
              $$.t = TYPE_ANY;
             }

E : "void" { $$.c = abc_pushundefined(0);
             $$.t = TYPE_ANY;
           }

E : '(' EXPRESSION ')' {$$=$2;} //allow commas in here, too

E : '-' E {
  $$=$2;
  if(IS_INT($2.t)) {
   $$.c=abc_negate_i($$.c);
   $$.t = TYPE_INT;
  } else {
   $$.c=abc_negate($$.c);
   $$.t = TYPE_NUMBER;
  }
}

E : E '[' E ']' {
  $$.c = $1.c;
  $$.c = code_append($$.c, $3.c);

  MULTINAME_LATE(m, $1.t?$1.t->access:ACCESS_PACKAGE, "");
  $$.c = abc_getproperty2($$.c, &m);
  $$.t = 0; // array elements have unknown type
}

E : '[' MAYBE_EXPRESSION_LIST ']' {
    $$.c = code_new();
    $$.c = code_append($$.c, $2.cc);
    $$.c = abc_newarray($$.c, $2.len);
    $$.t = registry_getarrayclass();
}

MAYBE_EXPRPAIR_LIST : {$$.cc=0;$$.len=0;}
MAYBE_EXPRPAIR_LIST : EXPRPAIR_LIST {$$=$1;}

EXPRPAIR_LIST : NONCOMMAEXPRESSION ':' NONCOMMAEXPRESSION {
    $$.cc = 0;
    $$.cc = code_append($$.cc, $1.c);
    $$.cc = code_append($$.cc, $3.c);
    $$.len = 2;
}
EXPRPAIR_LIST : EXPRPAIR_LIST ',' NONCOMMAEXPRESSION ':' NONCOMMAEXPRESSION {
    $$.cc = $1.cc;
    $$.len = $1.len+2;
    $$.cc = code_append($$.cc, $3.c);
    $$.cc = code_append($$.cc, $5.c);
}
//MAYBECOMMA: ','
//MAYBECOMMA:

E : "{ (dictionary)" MAYBE_EXPRPAIR_LIST '}' {
    $$.c = code_new();
    $$.c = code_append($$.c, $2.cc);
    $$.c = abc_newobject($$.c, $2.len/2);
    $$.t = registry_getobjectclass();
}

E : E "*=" E { 
               code_t*c = $3.c;
               if(BOTH_INT($1.t,$3.t)) {
                c=abc_multiply_i(c);
               } else {
                c=abc_multiply(c);
               }
               c=converttype(c, join_types($1.t, $3.t, '*'), $1.t);
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }

E : E "%=" E { 
               code_t*c = abc_modulo($3.c);
               c=converttype(c, join_types($1.t, $3.t, '%'), $1.t);
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }
E : E "<<=" E { 
               code_t*c = abc_lshift($3.c);
               c=converttype(c, join_types($1.t, $3.t, '<'), $1.t);
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }
E : E ">>=" E { 
               code_t*c = abc_rshift($3.c);
               c=converttype(c, join_types($1.t, $3.t, '>'), $1.t);
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }
E : E ">>>=" E { 
               code_t*c = abc_urshift($3.c);
               c=converttype(c, join_types($1.t, $3.t, 'U'), $1.t);
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }
E : E "/=" E { 
               code_t*c = abc_divide($3.c);
               c=converttype(c, join_types($1.t, $3.t, '/'), $1.t);
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }
E : E "|=" E { 
               code_t*c = abc_bitor($3.c);
               c=converttype(c, TYPE_INT, $1.t);
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }
E : E "^=" E { 
               code_t*c = abc_bitxor($3.c);
               c=converttype(c, TYPE_INT, $1.t);
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }
E : E "+=" E { 
               code_t*c = $3.c;

               if(TYPE_IS_INT($1.t)) {
                c=abc_add_i(c);
               } else {
                c=abc_add(c);
                c=converttype(c, join_types($1.t, $3.t, '+'), $1.t);
               }
               
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }
E : E "-=" E { code_t*c = $3.c; 
               if(TYPE_IS_INT($1.t)) {
                c=abc_subtract_i(c);
               } else {
                c=abc_subtract(c);
                c=converttype(c, join_types($1.t, $3.t, '-'), $1.t);
               }
               
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
             }
E : E '=' E { code_t*c = 0;
              c = code_append(c, $3.c);
              c = converttype(c, $3.t, $1.t);
              $$.c = toreadwrite($1.c, c, 1, 0);
              $$.t = $1.t;
            }

E : E '?' E ':' E %prec below_assignment { 
              $$.t = join_types($3.t,$5.t,'?');
              $$.c = $1.c;
              code_t*j1 = $$.c = abc_iffalse($$.c, 0);
              $$.c = code_append($$.c, $3.c);
              $$.c = converttype($$.c, $3.t, $$.t);
              code_t*j2 = $$.c = abc_jump($$.c, 0);
              $$.c = j1->branch = abc_label($$.c);
              $$.c = code_append($$.c, $5.c);
              $$.c = converttype($$.c, $5.t, $$.t);
              $$.c = j2->branch = abc_label($$.c);
            }

E : E "++" { code_t*c = 0;
             classinfo_t*type = $1.t;
             if((is_getlocal($1.c) && TYPE_IS_INT($1.t)) || TYPE_IS_NUMBER($1.t)) {
                 int nr = getlocalnr($1.c);
                 code_free($1.c);$1.c=0;
                 if(TYPE_IS_INT($1.t)) {
                    $$.c = abc_getlocal(0, nr);
                    $$.c = abc_inclocal_i($$.c, nr);
                 } else if(TYPE_IS_NUMBER($1.t)) {
                    $$.c = abc_getlocal(0, nr);
                    $$.c = abc_inclocal($$.c, nr);
                 } else syntaxerror("internal error");
             } else {
                 if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                     c=abc_increment_i(c);
                     type = TYPE_INT;
                 } else {
                     c=abc_increment(c);
                     type = TYPE_NUMBER;
                 }
                 c=converttype(c, type, $1.t);
                 $$.c = toreadwrite($1.c, c, 0, 1);
                 $$.t = $1.t;
             }
           }

// TODO: use inclocal, like with ++
E : E "--" { code_t*c = 0;
             classinfo_t*type = $1.t;
             if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                 c=abc_decrement_i(c);
                 type = TYPE_INT;
             } else {
                 c=abc_decrement(c);
                 type = TYPE_NUMBER;
             }
             c=converttype(c, type, $1.t);
             $$.c = toreadwrite($1.c, c, 0, 1);
             $$.t = $1.t;
            }

E : "++" %prec plusplus_prefix E { code_t*c = 0;
             classinfo_t*type = $2.t;
             if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                 c=abc_increment_i(c);
                 type = TYPE_INT;
             } else {
                 c=abc_increment(c);
                 type = TYPE_NUMBER;
             }
             c=converttype(c, type, $2.t);
             $$.c = toreadwrite($2.c, c, 0, 0);
             $$.t = $2.t;
           }

E : "--" %prec minusminus_prefix E { code_t*c = 0;
             classinfo_t*type = $2.t;
             if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                 c=abc_decrement_i(c);
                 type = TYPE_INT;
             } else {
                 c=abc_decrement(c);
                 type = TYPE_NUMBER;
             }
             c=converttype(c, type, $2.t);
             $$.c = toreadwrite($2.c, c, 0, 0);
             $$.t = $2.t;
           }

E : "super" '.' T_IDENTIFIER 
           { if(!state->cls->info)
                  syntaxerror("super keyword not allowed outside a class");
              classinfo_t*t = state->cls->info->superclass;
              if(!t) t = TYPE_OBJECT;

              memberinfo_t*f = registry_findmember_nsset(t, state->active_namespaces, $3, 1);

              MEMBER_MULTINAME(m, f, $3);
              $$.c = 0;
              $$.c = abc_getlocal_0($$.c);
              $$.c = abc_getsuper2($$.c, &m);
              $$.t = slotinfo_gettype((slotinfo_t*)f);
           }

E : '@' T_IDENTIFIER {
              // attribute TODO
              $$.c = abc_pushundefined(0);
              $$.t = 0;
              as3_warning("ignored @ operator");
           }

E : E '.' '@' T_IDENTIFIER {
              // child attribute  TODO
              $$.c = abc_pushundefined(0);
              $$.t = 0;
              as3_warning("ignored .@ operator");
           }

E : E '.' T_IDENTIFIER "::" T_IDENTIFIER {
              // namespace declaration TODO
              $$.c = abc_pushundefined(0);
              $$.t = 0;
              as3_warning("ignored :: operator");
           }

E : E ".." T_IDENTIFIER {
              // descendants TODO
              $$.c = abc_pushundefined(0);
              $$.t = 0;
              as3_warning("ignored .. operator");
           }

E : E '.' '(' E ')' {
              // filter TODO
              $$.c = abc_pushundefined(0);
              $$.t = 0;
              as3_warning("ignored .() operator");
           }

//VARIABLE : VARIABLE "::" '[' EXPRESSION ']' // qualified expression



E : E '.' T_IDENTIFIER
            {$$.c = $1.c;
             classinfo_t*t = $1.t;
             char is_static = 0;
             if(TYPE_IS_CLASS(t) && t->data) {
                 t = t->data;
                 is_static = 1;
             }
             if(t) {
                 memberinfo_t*f = registry_findmember_nsset(t, state->active_namespaces, $3, 1);
                 char noslot = 0;
                 if(f && !is_static != !(f->flags&FLAG_STATIC))
                    noslot=1;
                 if(f && f->slot && !noslot) {
                     $$.c = abc_getslot($$.c, f->slot);
                 } else {
                     MEMBER_MULTINAME(m, f, $3);
                     $$.c = abc_getproperty2($$.c, &m);
                 }
                 /* determine type */
                 $$.t = slotinfo_gettype((slotinfo_t*)f);
                 if(!$$.t)
                    $$.c = abc_coerce_a($$.c);
             } else {
                 /* when resolving a property on an unknown type, we do know the
                    name of the property (and don't seem to need the package), but
                    we need to make avm2 try out all access modes */
                 multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, $3};
                 $$.c = abc_getproperty2($$.c, &m);
                 $$.c = abc_coerce_a($$.c);
                 $$.t = registry_getanytype();
             }
            }

VAR_READ : T_IDENTIFIER {
    PASS1
    /* Queue unresolved identifiers for checking against the parent
       function's variables.
       We consider everything which is not a local variable "unresolved".
       This encompasses class names, members of the surrounding class
       etc. which *correct* because local variables of the parent function
       would shadow those.
       */
    if(state->method->inner && !find_variable(state, $1)) {
        unknown_variable($1);
    }
    PASS2

    $$.t = 0;
    $$.c = 0;
    slotinfo_t*a = 0;
    memberinfo_t*f = 0;

    variable_t*v;
    /* look at variables */
    if((v = find_variable(state, $1))) {
        // $1 is a local variable
        $$.c = abc_getlocal($$.c, v->index);
        $$.t = v->type;
        break;
    }
    if((v = find_slot(state, $1))) {
        $$.c = abc_getscopeobject($$.c, 1);
        $$.c = abc_getslot($$.c, v->index);
        $$.t = v->type;
        break;
    }

    int i_am_static = (state->method && state->method->info)?(state->method->info->flags&FLAG_STATIC):FLAG_STATIC;

    /* look at current class' members */
    if(state->cls && (f = registry_findmember_nsset(state->cls->info, state->active_namespaces, $1, 1)) &&
        (f->flags&FLAG_STATIC) >= i_am_static) {
        // $1 is a function in this class
        int var_is_static = (f->flags&FLAG_STATIC);

        if(f->kind == INFOTYPE_METHOD) {
            $$.t = TYPE_FUNCTION(f);
        } else {
            $$.t = f->type;
        }
        if(var_is_static && !i_am_static) {
        /* access to a static member from a non-static location.
           do this via findpropstrict:
           there doesn't seem to be any non-lookup way to access
           static properties of a class */
            state->method->late_binding = 1;
            $$.t = f->type;
            namespace_t ns = {f->access, ""};
            multiname_t m = {QNAME, &ns, 0, $1};
            $$.c = abc_findpropstrict2($$.c, &m);
            $$.c = abc_getproperty2($$.c, &m);
            break;
        } else if(f->slot>0) {
            $$.c = abc_getlocal_0($$.c);
            $$.c = abc_getslot($$.c, f->slot);
            break;
        } else {
            namespace_t ns = {f->access, ""};
            multiname_t m = {QNAME, &ns, 0, $1};
            $$.c = abc_getlocal_0($$.c);
            $$.c = abc_getproperty2($$.c, &m);
            break;
        }
    } 
    
    /* look at actual classes, in the current package and imported */
    if((a = find_class($1))) {
        if(a->access == ACCESS_PACKAGEINTERNAL &&
           strcmp(a->package, state->package) &&
           strcmp(a->package, internal_filename_package)
           )
           syntaxerror("Can't access internal %s %s in package '%s' from package '%s'",
                infotypename(a),$1, a->package, state->package);

        if(a->kind != INFOTYPE_CLASS) {
            MULTINAME(m, a);
            $$.c = abc_findpropstrict2($$.c, &m);
            $$.c = abc_getproperty2($$.c, &m);
            if(a->kind == INFOTYPE_METHOD) {
                methodinfo_t*f = (methodinfo_t*)a;
                $$.t = TYPE_FUNCTION(f);
            } else {
                varinfo_t*v = (varinfo_t*)a;
                $$.t = v->type;
            }
        } else {
            classinfo_t*c = (classinfo_t*)a;
            if(c->slot) {
                $$.c = abc_getglobalscope($$.c);
                $$.c = abc_getslot($$.c, c->slot);
            } else {
                MULTINAME(m, c);
                $$.c = abc_getlex2($$.c, &m);
            }
            $$.t = TYPE_CLASS(c);
        }
        break;
    }

    /* unknown object, let the avm2 resolve it */
    if(1) {
        as3_softwarning("Couldn't resolve '%s', doing late binding", $1);
        state->method->late_binding = 1;
                
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, $1};

        $$.t = 0;
        $$.c = abc_findpropstrict2($$.c, &m);
        $$.c = abc_getproperty2($$.c, &m);
    }
}

// ----------------- namespaces -------------------------------------------------

NAMESPACE_ID : "namespace" T_IDENTIFIER {
    PASS12
    tokenizer_register_namespace($2);
    $$=$2;
}

NAMESPACE_DECLARATION : MAYBE_MODIFIERS NAMESPACE_ID {
    $$=0;
}
NAMESPACE_DECLARATION : MAYBE_MODIFIERS NAMESPACE_ID '=' T_IDENTIFIER {
    $$=0;
}
NAMESPACE_DECLARATION : MAYBE_MODIFIERS NAMESPACE_ID '=' T_STRING {
    $$=0;
}
USE_NAMESPACE : "use" PACKAGEANDCLASS {
    PASS12
    tokenizer_register_namespace($2->name);
    $$=0;
}

