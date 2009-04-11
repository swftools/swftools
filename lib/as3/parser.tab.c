
/* A Bison parser, made by GNU Bison 2.4.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "./skeleton.m4"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         a3_parse
#define yylex           a3_lex
#define yyerror         a3_error
#define yylval          a3_lval
#define yychar          a3_char
#define yydebug         a3_debug
#define yynerrs         a3_nerrs


/* Copy the first part of user declarations.  */

/* Line 198 of skeleton.m4  */
#line 23 "parser.y"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "abc.h"
#include "pool.h"
#include "files.h"
#include "common.h"
#include "tokenizer.h"
#include "registry.h"
#include "code.h"
#include "opcodes.h"
#include "compiler.h"
#include "expr.h"
#include "initcode.h"

extern int a3_lex();



/* Line 198 of skeleton.m4  */
#line 102 "parser.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_IDENTIFIER = 258,
     T_NAMESPACE = 259,
     T_STRING = 260,
     T_REGEXP = 261,
     T_EMPTY = 262,
     T_INT = 263,
     T_UINT = 264,
     T_FLOAT = 265,
     T_FOR = 266,
     T_WHILE = 267,
     T_DO = 268,
     T_SWITCH = 269,
     KW_IMPLEMENTS = 270,
     KW_NAMESPACE = 271,
     KW_PACKAGE = 272,
     KW_PROTECTED = 273,
     KW_ARGUMENTS = 274,
     KW_PUBLIC = 275,
     KW_PRIVATE = 276,
     KW_USE = 277,
     KW_INTERNAL = 278,
     KW_NEW = 279,
     KW_NATIVE = 280,
     KW_FUNCTION = 281,
     KW_FINALLY = 282,
     KW_UNDEFINED = 283,
     KW_NAN = 284,
     KW_CONTINUE = 285,
     KW_CLASS = 286,
     KW_CONST = 287,
     KW_CATCH = 288,
     KW_CASE = 289,
     KW_SET = 290,
     KW_VOID = 291,
     KW_THROW = 292,
     KW_STATIC = 293,
     KW_WITH = 294,
     KW_INSTANCEOF = 295,
     KW_IMPORT = 296,
     KW_RETURN = 297,
     KW_TYPEOF = 298,
     KW_INTERFACE = 299,
     KW_NULL = 300,
     KW_VAR = 301,
     KW_DYNAMIC = 302,
     KW_OVERRIDE = 303,
     KW_FINAL = 304,
     KW_EACH = 305,
     KW_GET = 306,
     KW_TRY = 307,
     KW_SUPER = 308,
     KW_EXTENDS = 309,
     KW_FALSE = 310,
     KW_TRUE = 311,
     KW_BOOLEAN = 312,
     KW_UINT = 313,
     KW_INT = 314,
     KW_NUMBER = 315,
     KW_STRING = 316,
     KW_DEFAULT = 317,
     KW_DEFAULT_XML = 318,
     KW_DELETE = 319,
     KW_IF = 320,
     KW_ELSE = 321,
     KW_BREAK = 322,
     KW_IS = 323,
     KW_IN = 324,
     KW_AS = 325,
     T_DICTSTART = 326,
     T_EQEQ = 327,
     T_EQEQEQ = 328,
     T_NE = 329,
     T_NEE = 330,
     T_LE = 331,
     T_GE = 332,
     T_ORBY = 333,
     T_DIVBY = 334,
     T_MODBY = 335,
     T_MULBY = 336,
     T_ANDBY = 337,
     T_PLUSBY = 338,
     T_MINUSBY = 339,
     T_XORBY = 340,
     T_SHRBY = 341,
     T_SHLBY = 342,
     T_USHRBY = 343,
     T_OROR = 344,
     T_ANDAND = 345,
     T_COLONCOLON = 346,
     T_MINUSMINUS = 347,
     T_PLUSPLUS = 348,
     T_DOTDOT = 349,
     T_DOTDOTDOT = 350,
     T_SHL = 351,
     T_USHR = 352,
     T_SHR = 353,
     prec_none = 354,
     below_semicolon = 355,
     below_assignment = 356,
     below_lt = 357,
     below_minus = 358,
     minusminus_prefix = 359,
     plusplus_prefix = 360,
     below_curly = 361,
     new2 = 362,
     above_identifier = 363,
     below_else = 364,
     above_function = 365
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union 
/* Line 223 of skeleton.m4  */
#line 47 "parser.y"
tokenunion
{

/* Line 223 of skeleton.m4  */
#line 47 "parser.y"

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
    namespace_decl_t* namespace_decl;
    node_t*node;
    struct {
        abc_exception_list_t *l;
        code_t*finally;
    } catch_list;



/* Line 223 of skeleton.m4  */
#line 285 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 275 "parser.y"


static int a3_error(char*s)
{
   syntaxerror("%s", s); 
   return 0; //make gcc happy
}

static void parsererror(const char*file, int line, const char*f)
{
    syntaxerror("internal error in %s, %s:%d", f, file, line);
}

#define parserassert(b) {if(!(b)) parsererror(__FILE__, __LINE__,__func__);}


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
    parsedclass_t*dependencies;

    char has_constructor;
} classstate_t;

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

    char inner;
    char uses_parent_function;
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

typedef struct _state {
    struct _state*old;
    int level;
    
    char*package;     
    import_list_t*wildcard_imports;
    dict_t*import_toplevel_packages;
    dict_t*imports;

    namespace_list_t*active_namespace_urls;
    
    char has_own_imports;
    char new_vars; // e.g. transition between two functions
    char xmlfilter; // are we inside a xmlobj..() filter?
  
    classstate_t*cls;   
    methodstate_t*method;

    char*exception_name;

    int switch_var;
    
    dict_t*vars;
    dict_t*allvars; // also contains variables from sublevels
} state_t;

typedef struct _global {
    abc_file_t*file;

    parsedclass_list_t*classes;
    abc_script_t*classinit;

    abc_script_t*init; //package-level code

    dict_t*token2info;
    dict_t*file2token2info;
} global_t;

static global_t*global = 0;
static state_t* state = 0;

DECLARE_LIST(state);

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

static namespace_t ns1 = {ACCESS_PRIVATE, ""};
static namespace_t ns2 = {ACCESS_PROTECTED, ""};
static namespace_t ns3 = {ACCESS_PACKAGEINTERNAL, ""};
static namespace_t stdns = {ACCESS_PACKAGE, ""};
static namespace_list_t nl4 = {&stdns,0};
static namespace_list_t nl3 = {&ns3,&nl4};
static namespace_list_t nl2 = {&ns2,&nl3};
static namespace_list_t nl1 = {&ns1,&nl2};
static namespace_set_t nopackage_namespace_set = {&nl1};

static dict_t*definitions=0;
void as3_set_define(const char*c)
{
    if(!definitions) 
        definitions = dict_new();
    if(!dict_contains(definitions,c))
        dict_put(definitions,c,0);
}

static void new_state()
{
    NEW(state_t, s);
    state_t*oldstate = state;
    if(state)
        memcpy(s, state, sizeof(state_t)); //shallow copy
    if(!s->imports) {
        s->imports = dict_new();
    }
    if(!s->import_toplevel_packages) {
        s->import_toplevel_packages = dict_new(); 
    }
    state = s;
    state->level++;
    state->has_own_imports = 0;    
    state->vars = dict_new(); 
    state->old = oldstate;
    state->new_vars = 0;

    trie_remember(active_namespaces);
   
    if(oldstate)
        state->active_namespace_urls = list_clone(oldstate->active_namespace_urls);
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
        dict_destroy(state->vars);state->vars=0;
    }
    if(state->new_vars && state->allvars) {
        parserassert(!state->old || state->old->allvars != state->allvars);
        DICT_ITERATE_DATA(state->allvars, void*, data) {
            free(data);
        }
        dict_destroy(state->allvars);
    }
    
    list_free(state->active_namespace_urls)
    state->active_namespace_urls = 0;
    
    free(state);
}

static void old_state()
{
    trie_rollback(active_namespaces);

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
static void function_initvars(methodstate_t*m, char has_params, params_t*params, int flags, char var0);


static char* internal_filename_package = 0;
void initialize_file(char*filename)
{
    if(state) {
        syntaxerror("invalid call to initialize_file during parsing of another file");
    }
    
    active_namespaces = trie_new();

    new_state();
    state->package = internal_filename_package = strdup(filename);
    state->allvars = dict_new();
    
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
        state->method->late_binding = 1; // init scripts use getglobalscope, so we need a getlocal0/pushscope
    } else {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        if(!state->method)
            syntaxerror("internal error: skewed tokencount");
        function_initvars(state->method, 0, 0, 0, 1);
        global->init = abc_initscript(global->file);
    }
}

void finish_file()
{
    if(!state || state->level!=1) {
        syntaxerror("unexpected end of file in pass %d", as3_pass);
    }
    
    if(as3_pass==2) {
        dict_del(global->file2token2info, current_filename);
        code_t*header = method_header(state->method);
        code_t*c = wrap_function(header, 0, global->init->method->body->code);
        global->init->method->body->code = abc_returnvoid(c);
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
    global->classinit = abc_initscript(global->file);
}

void* finish_parser()
{
    dict_free_all(global->file2token2info, 1, (void*)dict_destroy);
    global->token2info=0;
    
    initcode_add_classlist(global->classinit, global->classes);

    return global->file;
}

typedef struct _variable {
    int index;
    classinfo_t*type;
    char init;
    char is_parameter;
    methodstate_t*is_inner_method;
} variable_t;

static variable_t* find_variable(state_t*s, char*name)
{
    state_t*top = s;
    while(s) {
        variable_t*v = 0;
        v = dict_lookup(s->vars, name);
        if(v) return v;
        if(s->new_vars) break;
        s = s->old;
    }
    return dict_lookup(top->allvars, name);
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

static code_t*defaultvalue(code_t*c, classinfo_t*type)
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
        syntaxerror("internal error: can't generate default value for * type");
    } else {
       c = abc_pushnull(c);
       MULTINAME(m, type);
       c = abc_coerce2(c, &m);
    }
    return c;
}

static int alloc_local()
{
    return state->method->variable_count++;
}

static variable_t* new_variable2(const char*name, classinfo_t*type, char init, char maybeslot)
{
    if(maybeslot) {
        variable_t*v = find_slot(state, name);
        if(v) {
            alloc_local(); 
            return v;
        }
    }

    NEW(variable_t, v);
    v->index = alloc_local();
    v->type = type;
    v->init = init;
 
    if(name) {
        dict_put(state->vars, name, v);
        dict_put(state->allvars, name, v);
    }

    return v;
}
static int new_variable(const char*name, classinfo_t*type, char init, char maybeslot)
{
    return new_variable2(name, type, init, maybeslot)->index;
}

#define TEMPVARNAME "__as3_temp__"
int gettempvar()
{
    variable_t*v = find_variable(state, TEMPVARNAME);
    int i;
    if(v) 
        i = v->index;
    else
        i = new_variable(TEMPVARNAME, 0, 0, 0);
    parserassert(i);
    return i;
}

static code_t* var_block(code_t*body) 
{
    code_t*c = 0;
    code_t*k = 0;
    int t;
    int num=0;
    for(t=0;t<state->vars->hashsize;t++) {
        DICT_ITERATE_DATA(state->vars, variable_t*, v) {
            if(v->type && v->init) {
                c = defaultvalue(c, v->type);
                c = abc_setlocal(c, v->index);
                k = abc_kill(k, v->index); 
                num++;
            }
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

static void unknown_variable(char*name) 
{
    if(!state->method->unresolved_variables)
        state->method->unresolved_variables = dict_new();
    if(!dict_contains(state->method->unresolved_variables, name))
        dict_put(state->method->unresolved_variables, name, 0);
}

static code_t* add_scope_code(code_t*c, methodstate_t*m, char init)
{
    if(m->uses_slots || (m->late_binding && !m->inner)) { //???? especially inner functions need the pushscope
        c = abc_getlocal_0(c);
        c = abc_pushscope(c);
    }
    if(m->uses_slots) {
        /* FIXME: this alloc_local() causes variable indexes to be
           different in pass2 than in pass1 */
        if(!m->activation_var) {
            m->activation_var = alloc_local();
        }
        if(init) {
            c = abc_newactivation(c);
            c = abc_dup(c);
            c = abc_pushscope(c);
            c = abc_setlocal(c, m->activation_var);
        } else {
            c = abc_getlocal(c, m->activation_var);
            c = abc_pushscope(c);
        }
    }
    return c;
}

static code_t* method_header(methodstate_t*m)
{
    code_t*c = 0;

    c = add_scope_code(c, m, 1);

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

    if(m->slots) {
        /* all parameters that are used by inner functions
           need to be copied from local to slot */
        parserassert(m->activation_var);
        DICT_ITERATE_ITEMS(m->slots,char*,name,variable_t*,v) {
            if(v->is_parameter) {
                c = abc_getlocal(c, m->activation_var); 
                c = abc_getlocal(c, v->index); 
                c = abc_setslot(c, v->index); 
            }
        }
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
    state->package = strdup(name);
}
static void endpackage()
{
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
        state_t*s = state;
        const char*url = (const char*)trie_lookup(active_namespaces, mod->ns);
        if(!url) {
            /* shouldn't happen- the tokenizer only reports something as a namespace
               if it was already registered */
            trie_dump(active_namespaces);
            syntaxerror("unknown namespace: %s", mod->ns);
        }
        ns.name = url;
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
static slotinfo_t* find_class(const char*name);

static memberinfo_t* findmember_nsset(classinfo_t*cls, const char*name, char recurse)
{
    return registry_findmember_nsset(cls, state->active_namespace_urls, name, recurse);
}

static void innerfunctions2vars(methodstate_t*m)
{
    methodstate_list_t*l = m->innerfunctions;
    while(l) {
        methodstate_t*m = l->methodstate;
        
        variable_t* v = new_variable2(m->info->name, TYPE_FUNCTION(m->info), 0, 0);
        m->var_index = v->index;
        if(m->is_a_slot)
            m->slot_index = m->is_a_slot;
        v->is_inner_method = m;
        l = l->next;
    }
}

static void function_initvars(methodstate_t*m, char has_params, params_t*params, int flags, char var0)
{
    if(var0) {
        int index = -1;
        if(m->inner)
            index = new_variable("this", 0, 0, 0);
        else if(!m->is_global)
            index = new_variable((flags&FLAG_STATIC)?"class":"this", state->cls?state->cls->info:0, 0, 0);
        else
            index = new_variable("globalscope", 0, 0, 0);
        parserassert(!index);
    }

    if(has_params) {
        param_list_t*p=0;
        for(p=params->list;p;p=p->next) {
            variable_t*v = new_variable2(p->param->name, p->param->type, 0, 1);
            v->is_parameter = 1;
        }
        if(as3_pass==2 && m->need_arguments) {
            /* arguments can never be used by an innerfunction (the inner functions
               have their own arguments var), so it's ok to  not initialize this until
               pass 2. (We don't know whether we need it before, anyway) */
            variable_t*v = new_variable2("arguments", TYPE_ARRAY, 0, 0);
            m->need_arguments = v->index;
        }
    }
    
    innerfunctions2vars(m);
    
    if(as3_pass==2) {
        m->scope_code = add_scope_code(m->scope_code, m, 0);
        if(m->slots) {
            /* exchange unresolved identifiers with the actual objects */
            DICT_ITERATE_ITEMS(m->slots, char*, name, variable_t*, v) {
                if(v->type && v->type->kind == INFOTYPE_UNRESOLVED) {
                    classinfo_t*type = (classinfo_t*)registry_resolve((slotinfo_t*)v->type);
                    if(!type || type->kind != INFOTYPE_CLASS) {
                        syntaxerror("Couldn't find class %s::%s (%s)", v->type->package, v->type->name, name);
                    }
                    v->type = type;
                }
            }
        }
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
    
    if((mod->flags&(FLAG_PROTECTED|FLAG_STATIC)) == (FLAG_PROTECTED|FLAG_STATIC))
        syntaxerror("protected and static not supported at the same time.");
    
    //if(!(mod->flags&FLAG_INTERFACE) && !extends) {
    if(!(mod->flags&FLAG_INTERFACE) && !extends) {
        // all classes extend object
        extends = registry_getobjectclass();
    }

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
        state->cls->static_init->is_static=FLAG_STATIC;
        state->cls->static_init->variable_count=1;
        /* notice: we make no effort to initialize the top variable (local0) here,
           even though it has special meaning. We just rely on the fact
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
        state->cls->info->superclass = extends;
        
        int pos = 0;
        classinfo_list_t*l = implements;
        for(l=implements;l;l=l->next) {
            state->cls->info->interfaces[pos++] = l->classinfo;
        }
    }
    
    if(as3_pass == 2) {
        state->cls = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
    
        state->method = state->cls->init;
        parserassert(state->cls && state->cls->info);
       
        function_initvars(state->cls->init, 0, 0, 0, 1);
        function_initvars(state->cls->static_init, 0, 0, 0, 0);

        if(extends && (extends->flags & FLAG_FINAL))
            syntaxerror("Can't extend final class '%s'", extends->name);
        
        int pos = 0;
        while(state->cls->info->interfaces[pos]) {
            if(!(state->cls->info->interfaces[pos]->flags & FLAG_INTERFACE))
                syntaxerror("'%s' is not an interface", 
                    state->cls->info->interfaces[pos]->name);
            pos++;
        }

        /* generate the abc code for this class */
        MULTINAME(classname2,state->cls->info);
        multiname_t*extends2 = sig2mname(extends);

        /* don't add the class to the class index just yet- that will be done later
           by initscript */
        state->cls->abc = abc_class_new(0, &classname2, extends2);
        state->cls->abc->file = global->file;

        multiname_destroy(extends2);
        if(state->cls->info->flags&FLAG_FINAL) abc_class_final(state->cls->abc);
        if(!(state->cls->info->flags&FLAG_DYNAMIC)) abc_class_sealed(state->cls->abc);
        if(state->cls->info->flags&FLAG_INTERFACE) {
            abc_class_interface(state->cls->abc);
        }

        for(mlist=implements;mlist;mlist=mlist->next) {
            MULTINAME(m, mlist->classinfo);
            abc_class_add_interface(state->cls->abc, &m);
        }

        state->cls->dependencies = parsedclass_new(state->cls->info, state->cls->abc);
        list_append(global->classes, state->cls->dependencies);

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
      
        trait_list_t*trait = state->cls->abc->traits;
        /* switch all protected members to the protected ns of this class */
        while(trait) {
            trait_t*t = trait->trait;
            if(t->name->ns->access == ACCESS_PROTECTED) {
                if(!state->cls->abc->protectedNS) {
                    char*n = concat3(state->cls->info->package, ":", state->cls->info->name);
                    state->cls->abc->protectedNS = namespace_new_protected(n);
                    state->cls->abc->flags |= CLASS_PROTECTED_NS;
                }
                t->name->ns->name = strdup(state->cls->abc->protectedNS->name);
            }
            trait = trait->next;
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
        if(c->opcode == OPCODE___RETHROW__) {
            syntaxerror("Unresolved \"rethrow\"");
        }
        if(c->opcode == OPCODE___FALLTHROUGH__) {
            syntaxerror("Unresolved \"fallthrough\"");
        }
        if(c->opcode == OPCODE___PUSHPACKAGE__) {
            char*name = string_cstr(c->data[0]);
            syntaxerror("Can't reference a package (%s) as such", name);
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
    
    /* allow this. it's no issue.
    if((m->flags & FLAG_STATIC) && !(flags&FLAG_STATIC))
        syntaxerror("can't override static member %s", m->name);*/

    if(!(m->flags & FLAG_STATIC) && (flags&FLAG_STATIC))
        syntaxerror("can't override non-static member %s with static declaration", m->name);

    if(!(flags&FLAG_OVERRIDE) && !(flags&FLAG_STATIC) && !(m->flags&FLAG_STATIC)) {
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
        if(getset == KW_GET) {
            type = return_type;
        } else if(params->list && params->list->param && !params->list->next) {
            type = params->list->param->type;
        } else
            syntaxerror("setter function needs to take exactly one argument");
        // not sure wether to look into superclasses here, too
        minfo = (methodinfo_t*)registry_findmember(state->cls->info, ns.name, name, 1);
        if(minfo) {
            if(minfo->kind!=INFOTYPE_VAR)
                syntaxerror("class already contains a method called '%s'", name);
            if(!(minfo->subtype & (SUBTYPE_GETSET)))
                syntaxerror("class already contains a field called '%s'", name);
            if(minfo->subtype & gs)
                syntaxerror("getter/setter for '%s' already defined", name);
            /* make a setter or getter into a getset */
            minfo->subtype |= gs;
            
            /*
            FIXME: this check needs to be done in pass 2
            
            if((!minfo->return_type != !type) ||
                (minfo->return_type && type && 
                 !strcmp(minfo->return_type->name, type->name))) {
                syntaxerror("different type in getter and setter: %s and %s", 
                    minfo->return_type?minfo->return_type->name:"*", 
                    type?type->name:"*");
            }*/
        } else {
            minfo = methodinfo_register_onclass(state->cls->info, ns.access, ns.name, name);
            minfo->kind = INFOTYPE_VAR; //hack
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
    state->allvars = dict_new();
   
    if(as3_pass == 1) {
        state->method = rfx_calloc(sizeof(methodstate_t));
        state->method->inner = 1;
        state->method->is_static = parent_method->is_static;
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
    
        function_initvars(state->method, 1, params, 0, 1);
    }

    if(as3_pass == 2) {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        state->method->variable_count = 0;
        parserassert(state->method);

        state->method->info->return_type = return_type;
        function_initvars(state->method, 1, params, 0, 1);
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
    state->allvars = dict_new();

    if(as3_pass == 1) {
        state->method = rfx_calloc(sizeof(methodstate_t));
        state->method->has_super = 0;
        state->method->is_static = mod->flags&FLAG_STATIC;

        if(state->cls) {
            state->method->is_constructor = !strcmp(state->cls->info->name,name);
        } else {
            state->method->is_global = 1;
            state->method->late_binding = 1; // for global methods, always push local_0 on the scope stack
        }
        if(state->method->is_constructor)
            name = "__as3_constructor__";

        state->method->info = registerfunction(getset, mod, name, params, return_type, 0);
       
        function_initvars(state->method, 1, params, mod->flags, 1);
        
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
        
        function_initvars(state->method, 1, params, mod->flags, 1);
    } 
}

static abc_method_t* endfunction(modifiers_t*mod, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type, code_t*body)
{
    if(as3_pass==1) {
        innerfunctions2vars(state->method);

        methodstate_list_t*ml = state->method->innerfunctions;
        
        dict_t*xvars = dict_new();

        while(ml) {
            methodstate_t*m = ml->methodstate;
            parserassert(m->inner);
            if(m->unresolved_variables) {
                dict_t*d = m->unresolved_variables;
                int t;
                DICT_ITERATE_KEY(d, char*, id) {
                    /* check parent method's variables */
                    variable_t*v;
                    if((v=find_variable(state, id))) {
                        m->uses_parent_function = 1;
                        state->method->uses_slots = 1;
                        dict_put(xvars, id, 0);
                    }
                }
                dict_destroy(m->unresolved_variables);m->unresolved_variables = 0;
            }
            ml = ml->next;
        }
        
        if(state->method->uses_slots) {
            state->method->slots = dict_new();
            int i = 1;
            DICT_ITERATE_ITEMS(state->vars, char*, name, variable_t*, v) {
                if(!name) syntaxerror("internal error");
                if(v->index && dict_contains(xvars, name)) {
                    v->init = 0;
                    v->index = i;
                    if(v->is_inner_method) {
                        v->is_inner_method->is_a_slot = i;
                    }
                    i++;
                    dict_put(state->method->slots, name, v);
                }
            }
            state->method->uses_slots = i;
            dict_destroy(state->vars);state->vars = 0;
            parserassert(state->new_vars);
            dict_destroy(state->allvars);state->allvars = 0;
        }
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
            namespace_t ns = modifiers2access(mod);
            multiname_t mname = {QNAME, &ns, 0, name};
            if(mod->flags&FLAG_STATIC)
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

        if(mod && mod->flags&FLAG_OVERRIDE) f->trait->attributes |= TRAIT_ATTR_OVERRIDE;
        if(getset == KW_GET) f->trait->kind = TRAIT_GETTER;
        if(getset == KW_SET) f->trait->kind = TRAIT_SETTER;
        if(params->varargs) f->flags |= METHOD_NEED_REST;
        if(state->method->need_arguments) f->flags |= METHOD_NEED_ARGUMENTS;

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
                syntaxerror("function %s: non-optional parameter not allowed after optional parameters", name);
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

        /* Seems this works now.
        if(state->method->exceptions && state->method->uses_slots) {
           as3_warning("try/catch and activation not supported yet within the same method");
        }*/

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
        if(TYPE_IS_UINT(to))
            return abc_convert_u(c);
        else if(TYPE_IS_INT(to))
            return abc_convert_i(c);
        else if(TYPE_IS_NUMBER(to))
            return abc_convert_d(c);
        return abc_coerce2(c, &m);
    }

    if(TYPE_IS_XMLLIST(to) && TYPE_IS_XML(from))
        return c;

    if(TYPE_IS_BOOLEAN(to))
        return abc_convert_b(c);
    if(TYPE_IS_STRING(to))
        return abc_convert_s(c);
    if(TYPE_IS_OBJECT(to))
        return abc_convert_o(c);

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
        from->package, from->package[0]?".":"", from->name, 
        to->package, to->package[0]?".":"", to->name);

    return c;
}
/* move to ast.c todo end */

char is_pushundefined(code_t*c)
{
    return (c && !c->prev && !c->next && c->opcode == OPCODE_PUSHUNDEFINED);
}

static const char* get_package_from_name(const char*name)
{
    /* try explicit imports */
    dictentry_t* e = dict_get_slot(state->imports, name);
    while(e) {
        if(!strcmp(e->key, name)) {
            slotinfo_t*c = (slotinfo_t*)e->data;
            if(c) return c->package;
        }
        e = e->next;
    }
    return 0;
}
static namespace_list_t*get_current_imports()
{
    namespace_list_t*searchlist = 0;
    
    list_append(searchlist, namespace_new_package(state->package));

    import_list_t*l = state->wildcard_imports;
    while(l) {
        namespace_t*ns = namespace_new_package(l->import->package);
        list_append(searchlist, ns);
        l = l->next;
    }
    list_append(searchlist, namespace_new_package(""));
    list_append(searchlist, namespace_new_package(internal_filename_package));
    return searchlist;
}

static slotinfo_t* find_class(const char*name)
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
typedcode_t push_class(slotinfo_t*a)
{
    typedcode_t x;
    x.c = 0;
    x.t = 0;
    if(a->access == ACCESS_PACKAGEINTERNAL &&
       strcmp(a->package, state->package) &&
       strcmp(a->package, internal_filename_package)
       ) {
       syntaxerror("Can't access internal %s %s in package '%s' from package '%s'",
            infotypename(a), a->name, a->package, state->package);
    }


    if(a->kind != INFOTYPE_CLASS) {
        MULTINAME(m, a);
        x.c = abc_findpropstrict2(x.c, &m);
        x.c = abc_getproperty2(x.c, &m);
        if(a->kind == INFOTYPE_METHOD) {
            methodinfo_t*f = (methodinfo_t*)a;
            x.t = TYPE_FUNCTION(f);
        } else {
            varinfo_t*v = (varinfo_t*)a;
            x.t = v->type;
        }
        return x;
    } else {
        if(state->cls && state->method == state->cls->static_init) {
            /* we're in the static initializer. 
               record the fact that we're using this class here */
            parsedclass_add_dependency(state->cls->dependencies, (classinfo_t*)a);
        }
        classinfo_t*c = (classinfo_t*)a;
        //if(c->slot) {
        if(0) { //Error #1026: Slot 1 exceeds slotCount=0 of global
            x.c = abc_getglobalscope(x.c);
            x.c = abc_getslot(x.c, c->slot);
        } else {
            MULTINAME(m, c);
            x.c = abc_getlex2(x.c, &m);
        }
        x.t = TYPE_CLASS(c);
    }
    return x;
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
        //printf("(use lookup) simple=%d > lookup=%d\n", simple_version_cost, lookup_version_cost);
        return insert_finally_lookup(c, finally, tempvar);
    } else {
        //printf("(use simple) simple=%d < lookup=%d\n", simple_version_cost, lookup_version_cost);
        return insert_finally_simple(c, finally, tempvar);
    }
}

#define PASS1 }} if(as3_pass == 1) {{
#define PASS1END }} if(as3_pass == 2) {{
#define PASS2 }} if(as3_pass == 2) {{
#define PASS12 }} if(as3_pass == 1 || as3_pass == 2) {{
#define PASS12END }} if(as3_pass == 2) {{
#define PASS_ALWAYS }} {{



/* Line 273 of skeleton.m4  */
#line 1906 "parser.tab.c"
/* Unqualified %code blocks.  */

/* Line 274 of skeleton.m4  */
#line 1983 "parser.y"

    char is_subtype_of(classinfo_t*type, classinfo_t*supertype)
    {
        return 1; // FIXME
    }


/* Line 274 of skeleton.m4  */
#line 2433 "parser.y"

    static void state_has_imports()
    {
        state->wildcard_imports = list_clone(state->wildcard_imports);
        state->imports = dict_clone(state->imports);
        state->has_own_imports = 1;
    }
    static void import_toplevel(const char*package)
    {
        char* s = strdup(package);
        while(1) {
            dict_put(state->import_toplevel_packages, s, 0);
            char*x = strrchr(s, '.');
            if(!x)
                break;
            *x = 0;
        }
        free(s);
    }


/* Line 274 of skeleton.m4  */
#line 2566 "parser.y"

    static int slotstate_varconst = 0;
    static modifiers_t*slotstate_flags = 0;
    static void setslotstate(modifiers_t* flags, int varconst)
    {
        slotstate_varconst = varconst;
        slotstate_flags = flags;
        if(state->cls) {
            if(flags) {
                if(flags->flags&FLAG_STATIC) {
                    state->method = state->cls->static_init;
                } else {
                    state->method = state->cls->init;
                }
            } else {
                // reset to "default" state (all in class code is static by default) */
                state->method = state->cls->static_init;
            }
        } else {
            parserassert(state->method);
        }
    }
    static trait_t* add_abc_slot(modifiers_t* modifiers, const char*name, multiname_t*m, code_t***c)
    {
        int flags = modifiers->flags;
        namespace_t ns = modifiers2access(modifiers);

        /* slot name */
        multiname_t mname = {QNAME, &ns, 0, name};
      
        trait_list_t**traits;
        code_t**code=0;
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
        if(c)
            *c = code;
        if(m) 
            *m = *multiname_clone(&mname);
            
        return trait_new_member(traits, 0, multiname_clone(&mname), 0);
    }


/* Line 274 of skeleton.m4  */
#line 2746 "parser.y"

    static int xml_level = 0;


/* Line 274 of skeleton.m4  */
#line 3525 "parser.y"

    node_t* resolve_identifier(char*name)
    {
        typedcode_t o;
        o.t = 0;
        o.c = 0;

        slotinfo_t*a = 0;
        memberinfo_t*f = 0;

        variable_t*v;
        /* look at variables */
        if((v = find_variable(state, name))) {
            // name is a local variable
            o.c = abc_getlocal(o.c, v->index);
            o.t = v->type;
            return mkcodenode(o);
        }
        if((v = find_slot(state, name))) {
            o.c = abc_getscopeobject(o.c, 1);
            o.c = abc_getslot(o.c, v->index);
            o.t = v->type;
            return mkcodenode(o);
        }

        int i_am_static = state->method->is_static;

        /* look at current class' members */
        if(!state->method->inner && 
           !state->xmlfilter &&
            state->cls && 
            (f = findmember_nsset(state->cls->info, name, 1)))
        {
            // name is a member or attribute in this class
            int var_is_static = (f->flags&FLAG_STATIC);

            if(f->kind == INFOTYPE_VAR && (f->flags&FLAG_CONST)) {
                /* if the variable is a constant (and we know what is evaluates to), we
                   can just use the value itself */
                varinfo_t*v = (varinfo_t*)f;
                if(v->value) {
                    return mkconstnode(v->value);
                }
            }
           
            if(var_is_static >= i_am_static) {
                if(f->kind == INFOTYPE_METHOD) {
                    o.t = TYPE_FUNCTION(f);
                } else {
                    o.t = f->type;
                }

                if(var_is_static && !i_am_static) {
                /* access to a static member from a non-static location.
                   do this via findpropstrict:
                   there doesn't seem to be any non-lookup way to access
                   static properties of a class */
                    state->method->late_binding = 1;
                    o.t = f->type;
                    namespace_t ns = {f->access, f->package};
                    multiname_t m = {QNAME, &ns, 0, name};
                    o.c = abc_findpropstrict2(o.c, &m);
                    o.c = abc_getproperty2(o.c, &m);
                    return mkcodenode(o);
                } else if(f->slot>0) {
                    o.c = abc_getlocal_0(o.c);
                    o.c = abc_getslot(o.c, f->slot);
                    return mkcodenode(o);
                } else {
                    MEMBER_MULTINAME(m, f, name);
                    o.c = abc_getlocal_0(o.c);
                    o.c = abc_getproperty2(o.c, &m);
                    return mkcodenode(o);
                }
            }
        } 
        
        /* look at actual classes, in the current package and imported */
        if(!state->xmlfilter && (a = find_class(name))) {
            if(state->cls && state->cls->info == (classinfo_t*)a && i_am_static) {
                o.c = abc_getlocal_0(0);
                o.t = TYPE_CLASS((classinfo_t*)a);
            } else {
                o = push_class(a);
            }
            return mkcodenode(o);
        }

        /* look through package prefixes */
        if(!state->xmlfilter && 
           (dict_contains(state->import_toplevel_packages, name) || 
            registry_ispackage(name))) {
            o.c = abc___pushpackage__(o.c, name);
            o.t = 0;
            return mkcodenode(o); //?
        }

        /* unknown object, let the avm2 resolve it */
        if(1) {
            if(!state->method->inner && !state->xmlfilter) {
                /* we really should make inner functions aware of the class context */
                as3_warning("Couldn't resolve '%s', doing late binding", name);
            }
            state->method->late_binding = 1;
                    
            multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, name};

            o.t = 0;
            o.c = abc_findpropstrict2(o.c, &m);
            o.c = abc_getproperty2(o.c, &m);
            return mkcodenode(o);
        }
    }


/* Line 274 of skeleton.m4  */
#line 3667 "parser.y"

    void add_active_url(const char*url)
    {
        NEW(namespace_t,n);
        n->name = url;
        list_append(state->active_namespace_urls, n);
    }



/* Line 274 of skeleton.m4  */
#line 2134 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  161
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3421

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  136
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  135
/* YYNRULES -- Number of rules.  */
#define YYNRULES  351
/* YYNRULES -- Number of states.  */
#define YYNSTATES  598

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   365

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   120,     2,     2,     2,   118,   109,     2,
     124,   135,   117,   115,   102,   114,   129,   116,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   106,   101,
     111,   104,   112,   105,   130,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   126,     2,   127,   108,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   128,   107,   134,   119,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   103,   110,   113,   121,
     122,   123,   125,   131,   132,   133
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    10,    13,    15,    17,
      19,    21,    23,    25,    30,    32,    33,    35,    37,    40,
      42,    44,    46,    48,    50,    55,    57,    59,    60,    63,
      65,    67,    69,    71,    73,    75,    77,    79,    81,    83,
      85,    87,    89,    91,    95,    98,   100,   102,   104,   106,
     108,   110,   112,   117,   120,   122,   124,   128,   131,   132,
     135,   138,   140,   144,   148,   149,   152,   153,   161,   162,
     164,   166,   170,   172,   175,   179,   188,   195,   196,   203,
     204,   212,   214,   217,   219,   222,   223,   225,   227,   230,
     232,   235,   240,   244,   245,   254,   255,   265,   266,   272,
     274,   277,   279,   282,   284,   285,   292,   295,   297,   302,
     305,   307,   309,   311,   313,   317,   319,   320,   327,   328,
     334,   337,   342,   343,   345,   347,   350,   352,   354,   356,
     358,   360,   362,   364,   366,   368,   370,   371,   374,   375,
     378,   379,   382,   383,   393,   394,   403,   404,   406,   408,
     411,   413,   418,   420,   422,   424,   425,   427,   429,   432,
     434,   437,   446,   448,   450,   451,   456,   458,   462,   466,
     467,   470,   472,   474,   476,   478,   480,   482,   484,   486,
     488,   490,   492,   494,   495,   496,   501,   502,   507,   508,
     511,   514,   517,   520,   524,   526,   528,   539,   546,   558,
     559,   561,   563,   566,   568,   572,   576,   580,   584,   585,
     587,   590,   595,   599,   601,   606,   609,   611,   613,   614,
     615,   628,   630,   631,   632,   643,   645,   649,   651,   653,
     655,   659,   661,   663,   665,   668,   669,   670,   674,   675,
     677,   679,   681,   684,   687,   688,   693,   698,   703,   706,
     708,   711,   713,   715,   717,   721,   723,   727,   728,   730,
     732,   734,   736,   738,   740,   744,   750,   752,   754,   756,
     758,   760,   762,   764,   766,   768,   770,   774,   778,   782,
     786,   790,   794,   798,   802,   806,   810,   814,   818,   821,
     824,   828,   832,   836,   840,   844,   848,   852,   856,   860,
     864,   868,   872,   876,   880,   884,   889,   892,   894,   898,
     901,   906,   910,   914,   918,   922,   926,   930,   934,   938,
     942,   946,   950,   954,   960,   963,   966,   969,   972,   976,
     979,   980,   987,   989,   991,   993,   995,   997,  1003,  1007,
    1013,  1018,  1023,  1030,  1037,  1041,  1043,  1046,  1051,  1056,
    1059,  1064
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     137,     0,    -1,   138,    -1,    -1,   139,    -1,   140,    -1,
     139,   140,    -1,   188,    -1,   200,    -1,   198,    -1,   234,
      -1,   209,    -1,   149,    -1,   150,   128,   138,   134,    -1,
     101,    -1,    -1,   142,    -1,   143,    -1,   142,   143,    -1,
     200,    -1,   198,    -1,   234,    -1,   209,    -1,   149,    -1,
     150,   128,   141,   134,    -1,   101,    -1,   145,    -1,    -1,
     145,   147,    -1,   147,    -1,   269,    -1,   191,    -1,   161,
      -1,   162,    -1,   163,    -1,   165,    -1,   173,    -1,   156,
      -1,   185,    -1,   181,    -1,   257,    -1,   270,    -1,   268,
      -1,   128,   145,   134,    -1,   128,   134,    -1,   101,    -1,
     146,    -1,   152,    -1,   167,    -1,   168,    -1,   253,    -1,
     183,    -1,   150,   128,   145,   134,    -1,   147,   101,    -1,
     147,    -1,   146,    -1,     3,    91,     3,    -1,   104,   261,
      -1,    -1,    46,   153,    -1,    32,   153,    -1,   154,    -1,
     153,   102,   154,    -1,     3,   244,   151,    -1,    -1,    66,
     148,    -1,    -1,    65,   124,   157,   255,   135,   148,   155,
      -1,    -1,   152,    -1,   257,    -1,    46,     3,   244,    -1,
       3,    -1,    11,   124,    -1,    11,    50,   124,    -1,   160,
     158,   101,   255,   101,   257,   135,   148,    -1,   160,   159,
      69,   255,   135,   148,    -1,    -1,    12,   124,   164,   255,
     135,   148,    -1,    -1,    13,   166,   148,    12,   124,   255,
     135,    -1,    67,    -1,    67,     3,    -1,    30,    -1,    30,
       3,    -1,    -1,   170,    -1,   172,    -1,   170,   172,    -1,
     171,    -1,   170,   171,    -1,    34,   261,   106,   144,    -1,
      62,   106,   144,    -1,    -1,    14,   124,   174,   261,   135,
     128,   169,   134,    -1,    -1,    33,   124,     3,   244,   135,
     176,   128,   144,   134,    -1,    -1,    27,   128,   178,   144,
     134,    -1,   175,    -1,   179,   175,    -1,   179,    -1,   179,
     177,    -1,   177,    -1,    -1,    52,   128,   182,   144,   134,
     180,    -1,    37,   255,    -1,    37,    -1,    39,   124,   255,
     135,    -1,   184,   148,    -1,     3,    -1,    17,    -1,    16,
      -1,     4,    -1,   187,   129,   186,    -1,   186,    -1,    -1,
      17,   187,   128,   189,   141,   134,    -1,    -1,    17,   128,
     190,   141,   134,    -1,    41,   240,    -1,    41,   187,   129,
     117,    -1,    -1,   193,    -1,   194,    -1,   193,   194,    -1,
      20,    -1,    21,    -1,    18,    -1,    38,    -1,    47,    -1,
      49,    -1,    48,    -1,    25,    -1,    23,    -1,     4,    -1,
      -1,    54,   241,    -1,    -1,    54,   242,    -1,    -1,    15,
     242,    -1,    -1,   192,    31,     3,   195,   197,   128,   199,
     202,   134,    -1,    -1,   192,    44,     3,   196,   128,   201,
     205,   134,    -1,    -1,   203,    -1,   204,    -1,   203,   204,
      -1,   101,    -1,   150,   128,   202,   134,    -1,   209,    -1,
     234,    -1,   146,    -1,    -1,   206,    -1,   207,    -1,   206,
     207,    -1,   101,    -1,    46,     3,    -1,   192,    26,   233,
       3,   124,   230,   135,   244,    -1,    46,    -1,    32,    -1,
      -1,   192,   208,   210,   211,    -1,   212,    -1,   211,   102,
     212,    -1,     3,   244,   151,    -1,    -1,   104,   261,    -1,
       8,    -1,     9,    -1,    10,    -1,     5,    -1,    56,    -1,
      55,    -1,    45,    -1,    28,    -1,    29,    -1,   226,    -1,
     111,    -1,   112,    -1,    -1,    -1,   128,   261,   220,   134,
      -1,    -1,   128,   261,   222,   134,    -1,    -1,   223,   219,
      -1,   223,     5,    -1,   223,   112,    -1,   226,   223,    -1,
     224,   226,   223,    -1,     3,    -1,   221,    -1,   216,   225,
     227,   217,   223,   111,   116,   225,   218,   112,    -1,   216,
     225,   227,   116,   218,   112,    -1,   216,   225,   227,   217,
     223,   224,   111,   116,   225,   218,   112,    -1,    -1,   228,
      -1,   229,    -1,   228,   229,    -1,   221,    -1,   221,   104,
       5,    -1,   221,   104,   221,    -1,     3,   104,   221,    -1,
       3,   104,     5,    -1,    -1,   231,    -1,    95,   232,    -1,
     231,   102,    95,   232,    -1,   231,   102,   232,    -1,   232,
      -1,     3,   106,   243,   213,    -1,     3,   213,    -1,    51,
      -1,    35,    -1,    -1,    -1,   192,    26,   233,     3,   124,
     230,   135,   244,   128,   235,   144,   134,    -1,     3,    -1,
      -1,    -1,    26,   236,   124,   230,   135,   244,   128,   238,
     144,   134,    -1,   186,    -1,   187,   129,   186,    -1,   240,
      -1,   239,    -1,   241,    -1,   242,   102,   241,    -1,   241,
      -1,   117,    -1,    36,    -1,   106,   243,    -1,    -1,    -1,
     124,   246,   135,    -1,    -1,   247,    -1,   248,    -1,   254,
      -1,   247,   102,    -1,   248,   254,    -1,    -1,    24,   261,
     249,   245,    -1,   261,   124,   246,   135,    -1,    53,   124,
     246,   135,    -1,    64,   261,    -1,    42,    -1,    42,   255,
      -1,   261,    -1,   256,    -1,   261,    -1,   256,   102,   261,
      -1,   261,    -1,   257,   102,   261,    -1,    -1,   260,    -1,
       3,    -1,     5,    -1,     8,    -1,     9,    -1,    10,    -1,
     259,   106,   254,    -1,   260,   102,   259,   106,   254,    -1,
     237,    -1,   265,    -1,   250,    -1,   252,    -1,   251,    -1,
     266,    -1,   214,    -1,   215,    -1,     6,    -1,    19,    -1,
     126,   246,   127,    -1,    71,   258,   134,    -1,   261,   111,
     261,    -1,   261,   112,   261,    -1,   261,    76,   261,    -1,
     261,    77,   261,    -1,   261,    72,   261,    -1,   261,    73,
     261,    -1,   261,    75,   261,    -1,   261,    74,   261,    -1,
     261,    89,   261,    -1,   261,    90,   261,    -1,   120,   261,
      -1,   119,   261,    -1,   261,   109,   261,    -1,   261,   108,
     261,    -1,   261,   107,   261,    -1,   261,    98,   261,    -1,
     261,    97,   261,    -1,   261,    96,   261,    -1,   261,   116,
     261,    -1,   261,   118,   261,    -1,   261,   115,   261,    -1,
     261,   114,   261,    -1,   261,   117,   261,    -1,   261,    69,
     261,    -1,   261,    70,   261,    -1,   261,    40,   261,    -1,
     261,    68,   261,    -1,    43,   124,   261,   135,    -1,    36,
     261,    -1,    36,    -1,   124,   256,   135,    -1,   114,   261,
      -1,   261,   126,   261,   127,    -1,   261,    81,   261,    -1,
     261,    80,   261,    -1,   261,    87,   261,    -1,   261,    86,
     261,    -1,   261,    88,   261,    -1,   261,    79,   261,    -1,
     261,    78,   261,    -1,   261,    85,   261,    -1,   261,    82,
     261,    -1,   261,    83,   261,    -1,   261,    84,   261,    -1,
     261,   104,   261,    -1,   261,   105,   261,   106,   261,    -1,
     261,    93,    -1,   261,    92,    -1,    93,   261,    -1,    92,
     261,    -1,    53,   129,     3,    -1,   130,     3,    -1,    -1,
     261,   129,   124,   262,   261,   135,    -1,     3,    -1,   117,
      -1,     4,    -1,   186,    -1,   117,    -1,   261,   129,   263,
      91,   264,    -1,   261,    94,   264,    -1,   261,   129,   126,
     261,   127,    -1,   261,   129,   130,   264,    -1,   261,    94,
     130,   264,    -1,   261,   129,   130,   126,   261,   127,    -1,
     261,    94,   130,   126,   261,   127,    -1,   261,   129,   264,
      -1,     3,    -1,    16,     3,    -1,    16,     3,   104,     3,
      -1,    16,     3,   104,     5,    -1,   192,   267,    -1,    63,
      16,   104,   261,    -1,    22,    16,   241,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1887,  1887,  1889,  1889,  1890,  1891,  1893,  1894,  1895,
    1896,  1897,  1898,  1899,  1900,  1902,  1902,  1903,  1904,  1906,
    1907,  1908,  1909,  1910,  1911,  1912,  1914,  1915,  1917,  1920,
    1923,  1924,  1925,  1926,  1927,  1928,  1929,  1930,  1931,  1932,
    1933,  1934,  1935,  1936,  1937,  1940,  1941,  1942,  1943,  1944,
    1945,  1946,  1947,  1959,  1960,  1964,  1971,  1990,  1991,  1993,
    1994,  1996,  1997,  1999,  2059,  2060,  2063,  2063,  2082,  2083,
    2084,  2089,  2093,  2098,  2099,  2101,  2121,  2169,  2169,  2188,
    2188,  2203,  2206,  2209,  2212,  2216,  2217,  2218,  2219,  2220,
    2221,  2223,  2234,  2237,  2237,  2268,  2268,  2293,  2293,  2309,
    2310,  2311,  2312,  2320,  2329,  2329,  2378,  2382,  2393,  2403,
    2420,  2421,  2422,  2423,  2425,  2426,  2428,  2428,  2430,  2430,
    2453,  2467,  2483,  2484,  2485,  2486,  2493,  2494,  2495,  2496,
    2497,  2498,  2499,  2500,  2501,  2502,  2506,  2507,  2509,  2510,
    2512,  2513,  2517,  2515,  2523,  2521,  2530,  2531,  2532,  2533,
    2534,  2535,  2536,  2537,  2539,  2545,  2546,  2547,  2548,  2549,
    2550,  2553,  2621,  2621,  2623,  2623,  2625,  2626,  2628,  2712,
    2713,  2722,  2723,  2726,  2727,  2728,  2729,  2730,  2731,  2732,
    2750,  2752,  2753,  2754,  2756,  2756,  2760,  2760,  2764,  2765,
    2768,  2769,  2771,  2772,  2774,  2775,  2777,  2781,  2784,  2789,
    2790,  2791,  2792,  2794,  2797,  2801,  2804,  2807,  2817,  2821,
    2827,  2833,  2841,  2846,  2852,  2860,  2868,  2869,  2870,  2873,
    2872,  2889,  2890,  2892,  2891,  2915,  2934,  2948,  2949,  2951,
    2952,  2954,  2955,  2956,  2965,  2966,  2970,  2971,  2973,  2974,
    2975,  2977,  2981,  2982,  2987,  2988,  3032,  3082,  3103,  3125,
    3128,  3135,  3138,  3141,  3144,  3147,  3150,  3155,  3156,  3158,
    3159,  3160,  3161,  3162,  3164,  3170,  3179,  3180,  3181,  3182,
    3183,  3184,  3186,  3190,  3202,  3220,  3231,  3241,  3250,  3251,
    3252,  3253,  3254,  3255,  3256,  3257,  3258,  3259,  3260,  3261,
    3262,  3263,  3264,  3265,  3266,  3267,  3268,  3269,  3270,  3271,
    3272,  3273,  3274,  3275,  3276,  3277,  3278,  3279,  3280,  3281,
    3282,  3283,  3284,  3285,  3286,  3287,  3288,  3289,  3290,  3291,
    3292,  3293,  3294,  3295,  3297,  3298,  3299,  3300,  3302,  3317,
    3325,  3325,  3379,  3380,  3381,  3382,  3383,  3391,  3409,  3416,
    3427,  3434,  3441,  3451,  3462,  3640,  3676,  3683,  3690,  3697,
    3719,  3724
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IDENTIFIER", "T_NAMESPACE",
  "T_STRING", "T_REGEXP", "T_EMPTY", "T_INT", "T_UINT", "T_FLOAT",
  "\"for\"", "\"while\"", "\"do\"", "\"switch\"", "\"implements\"",
  "\"namespace\"", "\"package\"", "\"protected\"", "\"arguments\"",
  "\"public\"", "\"private\"", "\"use\"", "\"internal\"", "\"new\"",
  "\"native\"", "\"function\"", "\"finally\"", "\"undefined\"", "\"NaN\"",
  "\"continue\"", "\"class\"", "\"const\"", "\"catch\"", "\"case\"",
  "\"set\"", "\"void\"", "\"throw\"", "\"static\"", "\"with\"",
  "\"instanceof\"", "\"import\"", "\"return\"", "\"typeof\"",
  "\"interface\"", "\"null\"", "\"var\"", "\"dynamic\"", "\"override\"",
  "\"final\"", "\"each\"", "\"get\"", "\"try\"", "\"super\"",
  "\"extends\"", "\"false\"", "\"true\"", "\"Boolean\"", "\"uint\"",
  "\"int\"", "\"Number\"", "\"String\"", "\"default\"", "\"default xml\"",
  "\"delete\"", "\"if\"", "\"else\"", "\"break\"", "\"is\"", "\"in\"",
  "\"as\"", "\"{ (dictionary)\"", "\"==\"", "\"===\"", "\"!=\"", "\"!==\"",
  "\"<=\"", "\">=\"", "\"|=\"", "\"/=\"", "\"%=\"", "\"*=\"", "\"&=\"",
  "\"+=\"", "\"-=\"", "\"^=\"", "\">>=\"", "\"<<=\"", "\">>>=\"", "\"||\"",
  "\"&&\"", "\"::\"", "\"--\"", "\"++\"", "\"..\"", "\"...\"", "\"<<\"",
  "\">>>\"", "\">>\"", "prec_none", "below_semicolon", "';'", "','",
  "below_assignment", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "below_lt",
  "'<'", "'>'", "below_minus", "'-'", "'+'", "'/'", "'*'", "'%'", "'~'",
  "'!'", "minusminus_prefix", "plusplus_prefix", "below_curly", "'('",
  "new2", "'['", "']'", "'{'", "'.'", "'@'", "above_identifier",
  "below_else", "above_function", "'}'", "')'", "$accept", "PROGRAM",
  "MAYBE_PROGRAM_CODE_LIST", "PROGRAM_CODE_LIST", "PROGRAM_CODE",
  "MAYBE_INPACKAGE_CODE_LIST", "INPACKAGE_CODE_LIST", "INPACKAGE_CODE",
  "MAYBECODE", "CODE", "CODE_STATEMENT", "CODEPIECE", "CODEBLOCK",
  "PACKAGE_INITCODE", "CONDITIONAL_COMPILATION", "MAYBEEXPRESSION",
  "VARIABLE_DECLARATION", "VARIABLE_LIST", "ONE_VARIABLE", "MAYBEELSE",
  "IF", "$@1", "FOR_INIT", "FOR_IN_INIT", "FOR_START", "FOR", "FOR_IN",
  "WHILE", "$@2", "DO_WHILE", "$@3", "BREAK", "CONTINUE",
  "MAYBE_CASE_LIST", "CASE_LIST", "CASE", "DEFAULT", "SWITCH", "$@4",
  "CATCH", "$@5", "FINALLY", "$@6", "CATCH_LIST", "CATCH_FINALLY_LIST",
  "TRY", "$@7", "THROW", "WITH_HEAD", "WITH", "X_IDENTIFIER", "PACKAGE",
  "PACKAGE_DECLARATION", "$@8", "$@9", "IMPORT", "MAYBE_MODIFIERS",
  "MODIFIER_LIST", "MODIFIER", "EXTENDS", "EXTENDS_LIST",
  "IMPLEMENTS_LIST", "CLASS_DECLARATION", "$@10", "INTERFACE_DECLARATION",
  "$@11", "MAYBE_CLASS_BODY", "CLASS_BODY", "CLASS_BODY_ITEM",
  "MAYBE_INTERFACE_BODY", "INTERFACE_BODY", "IDECLARATION", "VARCONST",
  "SLOT_DECLARATION", "$@12", "SLOT_LIST", "ONE_SLOT", "MAYBECONSTANT",
  "CONSTANT", "XML", "OPEN", "CLOSE", "CLOSE2", "XMLEXPR1", "$@13",
  "XMLEXPR2", "$@14", "XMLTEXT", "XML2", "XML_ID_OR_EXPR", "XMLNODE",
  "MAYBE_XMLATTRIBUTES", "XMLATTRIBUTES", "XMLATTRIBUTE",
  "MAYBE_PARAM_LIST", "PARAM_LIST", "PARAM", "GETSET",
  "FUNCTION_DECLARATION", "$@15", "MAYBE_IDENTIFIER", "INNERFUNCTION",
  "$@16", "CLASS", "PACKAGEANDCLASS", "CLASS_SPEC", "CLASS_SPEC_LIST",
  "TYPE", "MAYBETYPE", "MAYBE_PARAM_VALUES", "MAYBE_EXPRESSION_LIST",
  "EXPRESSION_LIST", "EXPRESSION_LIST_AND_COMMA", "XX", "NEW",
  "FUNCTIONCALL", "DELETE", "RETURN", "NONCOMMAEXPRESSION", "EXPRESSION",
  "COMMA_EXPRESSION", "VOIDEXPRESSION", "MAYBE_DICT_EXPRPAIR_LIST",
  "DICTLH", "DICT_EXPRPAIR_LIST", "E", "$@17", "ID_OR_NS", "SUBNODE",
  "MEMBER", "VAR_READ", "NAMESPACE_ID", "NAMESPACE_DECLARATION",
  "DEFAULT_NAMESPACE", "USE_NAMESPACE", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,    59,    44,   356,    61,    63,    58,   124,    94,    38,
     357,    60,    62,   358,    45,    43,    47,    42,    37,   126,
      33,   359,   360,   361,    40,   362,    91,    93,   123,    46,
      64,   363,   364,   365,   125,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   136,   137,   138,   138,   139,   139,   140,   140,   140,
     140,   140,   140,   140,   140,   141,   141,   142,   142,   143,
     143,   143,   143,   143,   143,   143,   144,   144,   145,   145,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   147,   147,   147,   147,   147,
     147,   147,   147,   148,   148,   149,   150,   151,   151,   152,
     152,   153,   153,   154,   155,   155,   157,   156,   158,   158,
     158,   159,   159,   160,   160,   161,   162,   164,   163,   166,
     165,   167,   167,   168,   168,   169,   169,   169,   169,   170,
     170,   171,   172,   174,   173,   176,   175,   178,   177,   179,
     179,   180,   180,   180,   182,   181,   183,   183,   184,   185,
     186,   186,   186,   186,   187,   187,   189,   188,   190,   188,
     191,   191,   192,   192,   193,   193,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   195,   195,   196,   196,
     197,   197,   199,   198,   201,   200,   202,   202,   203,   203,
     204,   204,   204,   204,   204,   205,   205,   206,   206,   207,
     207,   207,   208,   208,   210,   209,   211,   211,   212,   213,
     213,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     215,   216,   217,   218,   220,   219,   222,   221,   223,   223,
     223,   223,   224,   224,   225,   225,   226,   226,   226,   227,
     227,   228,   228,   229,   229,   229,   229,   229,   230,   230,
     230,   230,   231,   231,   232,   232,   233,   233,   233,   235,
     234,   236,   236,   238,   237,   239,   240,   241,   241,   242,
     242,   243,   243,   243,   244,   244,   245,   245,   246,   246,
     246,   247,   248,   247,   249,   250,   251,   251,   252,   253,
     253,   254,   255,   256,   256,   257,   257,   258,   258,   259,
     259,   259,   259,   259,   260,   260,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     262,   261,   263,   263,   263,   264,   264,   261,   261,   261,
     261,   261,   261,   261,   265,   266,   267,   267,   267,   268,
     269,   270
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     4,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     1,     4,     1,     1,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     4,     2,     1,     1,     3,     2,     0,     2,
       2,     1,     3,     3,     0,     2,     0,     7,     0,     1,
       1,     3,     1,     2,     3,     8,     6,     0,     6,     0,
       7,     1,     2,     1,     2,     0,     1,     1,     2,     1,
       2,     4,     3,     0,     8,     0,     9,     0,     5,     1,
       2,     1,     2,     1,     0,     6,     2,     1,     4,     2,
       1,     1,     1,     1,     3,     1,     0,     6,     0,     5,
       2,     4,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     2,     0,     2,
       0,     2,     0,     9,     0,     8,     0,     1,     1,     2,
       1,     4,     1,     1,     1,     0,     1,     1,     2,     1,
       2,     8,     1,     1,     0,     4,     1,     3,     3,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     0,     4,     0,     4,     0,     2,
       2,     2,     2,     3,     1,     1,    10,     6,    11,     0,
       1,     1,     2,     1,     3,     3,     3,     3,     0,     1,
       2,     4,     3,     1,     4,     2,     1,     1,     0,     0,
      12,     1,     0,     0,    10,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     2,     0,     0,     3,     0,     1,
       1,     1,     2,     2,     0,     4,     4,     4,     2,     1,
       2,     1,     1,     1,     3,     1,     3,     0,     1,     1,
       1,     1,     1,     1,     3,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     2,     1,     3,     2,
       4,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     5,     2,     2,     2,     2,     3,     2,
       0,     6,     1,     1,     1,     1,     1,     5,     3,     5,
       4,     4,     6,     6,     3,     1,     2,     4,     4,     2,
       4,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     122,   345,   135,   174,   274,   171,   172,   173,     0,     0,
      79,     0,     0,   128,   275,   126,   127,     0,   134,     0,
     133,   222,   178,   179,   307,   129,     0,     0,     0,   177,
     130,   132,   131,     0,     0,   176,   175,     0,     0,     0,
     257,     0,     0,    14,   181,     0,     0,     0,     0,   238,
     122,     0,     0,     2,   122,     5,    55,    12,     0,    37,
      68,    32,    33,    34,    35,    36,    39,   122,    38,     7,
      31,     0,   123,   124,     9,     8,    11,   272,   273,     0,
     180,    10,   266,   268,   270,   269,    40,   255,   267,   271,
      42,    30,    41,     0,     0,    73,    77,   122,    93,   110,
     113,   112,   111,   118,   115,     0,     0,   345,   244,   221,
       0,   306,     0,     0,   120,     0,   104,   238,     0,     0,
     248,    66,   259,   260,   261,   262,   263,     0,     0,   258,
     327,   326,   309,   289,   288,     0,   253,     0,   239,   240,
     241,   251,    83,     0,   107,   249,     0,    81,    45,    44,
     122,    46,    29,     0,    47,    48,    49,    51,     0,    50,
     329,     1,     6,   122,   345,     0,    69,     0,     0,    70,
      54,   109,     0,   218,     0,   163,     0,   162,   164,   349,
     125,   194,     0,   195,   199,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   325,
     324,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,     0,
      56,    74,     0,     0,     0,   122,   116,     0,   225,     0,
     228,   227,   351,   236,   208,     0,   252,     0,     0,    27,
       0,   328,     0,     0,   277,     0,     0,     0,   308,   276,
     242,   243,    84,   235,    60,    61,   106,   250,    59,    82,
      43,    28,   122,     0,   235,     0,     0,    53,   346,   217,
     216,     0,   136,   138,     0,   186,     0,   203,     0,   200,
     201,   256,   303,   304,   301,   302,   282,   283,   285,   284,
     280,   281,   317,   316,   312,   311,   319,   320,   321,   318,
     314,   313,   315,   286,   287,   336,     0,   335,   338,   295,
     294,   293,   322,     0,   292,   291,   290,   278,   279,   299,
     298,   296,   300,   297,     0,     0,   110,   113,   336,   330,
       0,     0,     0,   344,     0,     0,     0,    25,     0,   122,
      17,    23,     0,    20,    19,    22,    21,   122,   114,     0,
     238,   245,   169,     0,     0,   209,   213,   108,   121,   226,
     305,     0,    26,   247,   350,     0,   264,     0,   254,     0,
      58,     0,   122,    13,    58,     0,     0,     0,     0,     0,
     140,     0,     0,   235,   165,   166,     0,     0,     0,   182,
     183,   188,   202,     0,   341,     0,   246,   310,     0,     0,
       0,   340,     0,   122,     0,     0,   119,    18,   122,     0,
       0,     0,     0,   215,   210,   235,     0,     0,   122,     0,
     233,   232,   231,   234,     0,    63,    62,    52,     0,   122,
     347,   348,   208,   137,     0,     0,   229,   139,   144,    58,
       0,   187,   207,   206,   204,   205,     0,     0,     0,   323,
       0,   339,     0,   337,    78,     0,    85,     0,   117,   237,
     170,   169,     0,     0,   212,     0,     0,    99,   103,   101,
     105,    64,   265,    57,     0,    76,     0,   141,   142,     0,
     122,   168,   167,   197,   190,   181,   191,     0,   189,     0,
     188,   343,   331,   342,    80,     0,     0,     0,    86,    89,
      87,    24,   214,   223,   211,    97,     0,   100,   102,   122,
      67,   122,   235,   122,   230,     0,   159,     0,     0,   122,
     157,     0,   184,   181,   188,   192,     0,    27,    94,    90,
      88,    27,    27,   235,    65,    75,     0,   150,   154,     0,
       0,     0,   122,   148,   152,   153,   160,   218,   145,   158,
     183,     0,     0,   193,    27,    92,     0,     0,     0,   219,
     122,   143,   149,     0,     0,   185,   183,    91,   224,    98,
      95,    27,     0,     0,   196,     0,     0,     0,   151,   208,
     198,    27,   220,     0,     0,   235,    96,   161
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    52,    53,    54,    55,   348,   349,   350,   371,   372,
     151,   152,   171,   351,   153,   435,   154,   268,   265,   520,
      59,   253,   167,   168,    60,    61,    62,    63,   232,    64,
      97,   155,   156,   507,   508,   509,   510,    65,   234,   477,
     586,   478,   542,   479,   480,    66,   249,   157,    67,    68,
     238,   239,    69,   357,   235,    70,   158,    72,    73,   390,
     392,   445,   353,   523,   354,   490,   551,   552,   553,   528,
     529,   530,   178,   355,   284,   394,   395,   423,    77,    78,
      79,   401,   456,   498,   561,   183,   396,   457,   499,   184,
      80,   288,   289,   290,   364,   365,   366,   281,   356,   581,
     110,    82,   541,   240,   241,   432,   447,   433,   380,   361,
     137,   138,   139,   243,    83,    84,    85,   159,   140,   245,
     246,    86,   127,   128,   129,    87,   408,   342,   318,    88,
      89,   179,    90,    91,    92
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -494
static const yytype_int16 yypact[] =
{
    1763,   -21,  -494,  -494,  -494,  -494,  -494,  -494,   -29,   -45,
    -494,   -36,    68,  -494,  -494,  -494,  -494,    75,  -494,  2217,
    -494,    91,  -494,  -494,   438,  -494,   -15,   316,    -5,  -494,
    -494,  -494,  -494,     7,   -37,  -494,  -494,   107,  2217,    24,
     367,  2217,  2217,  -494,  -494,  2217,  2217,  2217,  2217,  2217,
     707,   127,   139,  -494,   575,  -494,  -494,  -494,    23,  -494,
    2145,  -494,  -494,  -494,  -494,  -494,  -494,  2019,  -494,  -494,
    -494,   488,   431,  -494,  -494,  -494,  -494,  -494,  -494,    17,
    -494,  -494,  -494,  -494,  -494,  -494,    57,  2920,  -494,  -494,
    -494,  -494,  -494,   155,    36,  -494,  -494,  2019,  -494,  -494,
    -494,  -494,  -494,  -494,  -494,    28,   316,  -494,   -48,  -494,
      47,   213,  2217,    69,  -494,  2217,  -494,  2217,   194,   116,
     213,  -494,  -494,  -494,  -494,  -494,  -494,    92,   121,   132,
     213,   213,   379,   213,   213,   -42,  2858,   114,   146,  2217,
    -494,  2858,   246,   247,  2217,  2217,   247,   249,  -494,  -494,
     839,  -494,  -494,   125,  -494,  -494,  -494,  -494,   248,  -494,
    -494,  -494,  -494,  1103,   197,   264,  -494,   168,   203,    57,
     173,  -494,   274,    66,   276,  -494,   278,  -494,  -494,  -494,
    -494,  -494,  2217,  -494,    21,  2217,  2217,  2217,  2217,  2217,
    2217,  2217,  2217,  2217,  2217,  2217,  2217,  2217,  2217,  2217,
    2217,  2217,  2217,  2217,  2217,  2217,  2217,  2217,  2217,  -494,
    -494,   138,  2217,  2217,  2217,  2217,  2217,  2217,  2217,  2217,
    2217,  2217,  2217,  2217,  2217,  2217,  2217,  2217,  2217,   130,
    -494,  -494,  2217,   273,  2217,  1235,  -494,   316,   158,   160,
    -494,  -494,  -494,   167,    15,   157,   192,   229,  2276,  1891,
     165,  -494,  2217,  2217,  -494,  2217,   367,  2217,  -494,  -494,
    -494,  -494,  -494,   207,   212,  -494,  -494,  -494,   212,  -494,
    -494,  -494,  2019,   181,   207,  2217,  2217,  -494,   217,  -494,
    -494,   314,   262,   269,   315,  2858,   220,   221,     6,    21,
    -494,  2858,  1095,   567,   567,   567,  3292,  3292,  3292,  3292,
    1095,  1095,  2858,  2858,  2858,  2858,  2858,  2858,  2858,  2858,
    2858,  2858,  2858,  2982,  3044,  -494,   214,  -494,  -494,   313,
     313,   313,  2858,  2486,  3106,  3168,  3230,  1095,  1095,   379,
     379,   213,   213,   213,   199,  2548,   238,   244,   245,  -494,
    2217,   267,   254,  -494,   226,   224,  2346,  -494,   228,  1367,
    -494,  -494,   235,  -494,  -494,  -494,  -494,  1235,  -494,   316,
    2217,  -494,   -55,   363,   232,   280,  -494,  -494,  -494,   240,
    -494,   237,  1891,  -494,  2858,   243,  -494,   277,  2858,   292,
     281,   247,   971,  -494,    -6,   279,   252,   135,   270,   316,
     376,   316,   268,   207,   296,  -494,   265,    22,    34,  -494,
    -494,  -494,  -494,  2217,  -494,  2217,  -494,  -494,  2217,  2610,
    2217,  -494,   294,  2019,  2217,   272,  -494,  -494,  1235,   283,
     275,  2217,   292,  -494,  -494,   207,    19,    78,  2019,  2217,
    -494,  -494,  -494,  -494,  2217,  -494,  -494,  -494,  2217,  2019,
    -494,  -494,    15,  -494,   316,   284,  -494,   300,  -494,   281,
     315,  -494,  -494,  -494,  -494,  -494,   291,   253,  2672,  2858,
    2416,  -494,  2734,  -494,  -494,   287,    46,   289,  -494,  -494,
    2858,   309,   286,   363,  -494,   297,   312,  -494,  -494,    78,
    -494,   338,  -494,  2858,   -38,  -494,   298,   300,  -494,   316,
      27,  -494,  -494,  -494,  -494,   299,  -494,  2217,  -494,   327,
    -494,  -494,  -494,  -494,  -494,  2217,   318,   306,    46,  -494,
    -494,  -494,  -494,  -494,  -494,  -494,   442,  -494,  -494,  2019,
    -494,  2019,   207,  1499,  -494,   447,  -494,   427,   321,   255,
    -494,    17,  2858,   342,  -494,    25,  2796,  1891,  -494,  -494,
    -494,  1891,  1891,   207,  -494,  -494,   332,  -494,  -494,   335,
     328,   331,  1631,  -494,  -494,  -494,  -494,    66,  -494,  -494,
    -494,   334,    17,    25,  1891,  -494,   336,   340,   341,  -494,
    1499,  -494,  -494,   472,   365,  -494,  -494,  -494,  -494,  -494,
    -494,  1891,   347,   358,  -494,   373,   359,   352,  -494,    15,
    -494,  1891,  -494,   353,   355,   207,  -494,  -494
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -494,  -494,   337,  -494,   444,  -315,  -494,   150,  -475,   -34,
       1,   -60,   -87,    53,     2,    52,   446,   364,   129,  -494,
    -494,  -494,  -494,  -494,  -494,  -494,  -494,  -494,  -494,  -494,
    -494,  -494,  -494,  -494,  -494,     5,     8,  -494,  -494,    32,
    -494,    33,  -494,  -494,  -494,  -494,  -494,  -494,  -494,  -494,
      14,    56,  -494,  -494,  -494,  -494,     0,  -494,   430,  -494,
    -494,  -494,    59,  -494,    61,  -494,   -53,  -494,   -31,  -494,
    -494,   -14,  -494,     3,  -494,  -494,    72,    62,  -494,  -494,
    -494,  -494,  -464,  -494,  -494,  -169,  -494,  -457,  -494,  -493,
    -413,  -494,  -494,   239,  -419,  -494,  -344,   -39,     4,  -494,
    -494,  -494,  -494,  -494,   508,  -101,    93,   117,  -257,  -494,
    -103,  -494,  -494,  -494,  -494,  -494,  -494,  -494,  -130,  -132,
     490,   -52,  -494,   285,  -494,   -13,  -494,  -494,  -189,  -494,
    -494,  -494,  -494,  -494,  -494
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -335
static const yytype_int16 yytable[] =
{
      71,    56,    58,    76,    81,   242,   108,   170,   169,   261,
     233,   111,   266,   267,   250,   287,   150,   384,   362,   424,
     181,    94,   362,   486,   286,   120,   104,   452,   130,   131,
     494,     2,   132,   133,   134,   136,   141,   170,   560,   454,
     343,   104,   419,   535,   500,    13,   211,    15,    16,   421,
      18,   422,    20,    57,    71,    56,    58,    76,    81,    74,
     257,    75,   565,   -71,   185,    25,   566,   567,   105,   576,
      93,    99,   100,   525,    30,    31,    32,   563,   228,    96,
     505,   229,   474,   113,   101,   102,   534,   117,    98,   577,
     271,   106,   118,   258,   109,    95,   574,   521,   434,   136,
     344,   279,   248,   467,   141,   475,   587,    57,   506,   112,
     363,   476,   585,    74,   473,    75,   594,   280,   399,   115,
     287,   375,   400,   119,   334,   376,   141,   404,   526,   514,
     160,   136,   136,   336,   337,   116,   449,   496,   440,   161,
     441,    99,   100,   385,   386,   182,   101,   102,   121,   182,
     182,   163,   411,   497,   101,   102,   236,   237,   230,   185,
     231,  -155,   182,    71,    56,    58,    76,    81,   472,   285,
     593,   244,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   103,   251,   247,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   141,   335,    57,    99,   100,   136,
     252,   346,    74,   463,    75,   317,   254,   255,   453,   455,
     101,   102,    99,   100,   256,    71,    56,   352,   382,   374,
     136,   259,   141,   317,   378,   101,   102,   338,   260,   262,
     263,   358,   269,   272,   339,   315,   340,   420,   494,     2,
     341,   369,   136,   136,   172,   546,   -72,   274,   316,   275,
      99,   100,   276,    13,   277,    15,    16,   278,    18,   282,
      20,   283,   465,   101,   102,   345,   568,  -115,   443,   359,
     446,   360,   367,    25,   257,    99,   100,    99,   100,   482,
     373,   525,    30,    31,    32,   209,   210,   211,   101,   102,
     101,   102,   271,   379,   381,   383,   389,   388,   393,    99,
     100,   387,   271,   391,   397,   398,   464,   409,   430,  -332,
     317,   315,   101,   102,   406,  -334,  -333,   227,   597,   228,
     403,   481,   229,   446,   172,   412,   368,   141,   414,    71,
      56,   352,   485,   170,   173,   317,   526,    71,    56,   352,
     175,   413,   416,   418,   495,   496,   362,   425,   170,  -114,
     122,   427,   123,   369,   177,   124,   125,   126,   428,   170,
     438,   497,   426,   429,   315,   434,   484,   439,   524,  -156,
     458,   444,   459,   410,   442,   460,   448,   462,   450,   451,
     466,   136,   489,   493,   519,   209,   210,   211,   470,   431,
     469,   315,   488,   421,   513,   531,   141,   468,    71,    56,
     352,   483,   504,   511,   537,   515,   317,   222,   223,   224,
     225,   226,   544,   522,   545,     2,   516,   227,   533,   228,
     538,   107,   229,     3,     4,   543,     5,     6,     7,    13,
     556,    15,    16,   557,    18,   558,    20,    14,   562,   170,
     569,   170,    19,   570,    21,   571,    22,    23,   575,    25,
     578,   209,   210,   211,   579,   583,   580,   584,    30,    31,
      32,   588,   589,    29,   532,   590,   592,   591,   595,   596,
     527,    34,   536,    35,    36,   224,   225,   226,   162,   417,
     273,   491,   180,   227,   172,   228,   166,   264,   229,    40,
     436,   517,   518,   539,   173,   559,   540,   582,   573,   174,
     175,   572,   492,   550,   548,   549,   554,   555,   402,   527,
      41,    42,   176,   512,   177,   114,     0,   487,   135,   471,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   550,   548,   549,   554,   555,     0,     0,     0,
       0,     0,    48,     0,    49,     0,     0,     0,    51,     0,
     550,   548,   549,   554,   555,    -4,     0,     0,     1,     2,
       3,     4,     0,     5,     6,     7,     8,     9,    10,    11,
       0,     0,    12,    13,    14,    15,    16,    17,    18,    19,
      20,     0,     0,    22,    23,     0,     0,   186,     0,     0,
       0,    24,     0,    25,    26,     0,    27,     0,    28,     0,
      29,     0,    30,    31,    32,     0,     0,    33,    34,     0,
      35,    36,     0,     0,     0,  -335,  -335,  -335,    37,    38,
      39,     0,     0,   194,   195,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   209,
     210,   211,     0,   212,   213,   214,     0,    41,    42,     0,
       0,     0,     0,     0,     0,     0,    43,     0,   220,   221,
       0,   222,   223,   224,   225,   226,    44,     0,     0,    45,
       0,   227,     0,   228,    46,    47,   229,     0,     0,    48,
       0,    49,     0,    50,     0,    51,     0,     0,     0,    -4,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     0,    22,    23,   142,     0,   143,
       0,     0,     0,    24,   144,    25,    26,     0,    27,   145,
      28,     0,    29,   146,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,    39,     0,   147,     0,     0,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    41,
      42,     0,     0,     0,     0,     0,     0,     0,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,     0,
       0,    45,     0,     0,     0,     0,    46,    47,     0,     0,
       0,    48,     0,    49,     0,    50,     0,    51,     0,     0,
       0,   149,     1,     2,     3,     4,     0,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    13,    14,    15,
      16,    17,    18,    19,    20,    21,     0,    22,    23,   142,
       0,   143,     0,     0,     0,    24,   144,    25,    26,     0,
      27,   145,    28,     0,    29,   146,    30,    31,    32,     0,
       0,    33,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    37,    38,    39,     0,   147,     0,     0,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,    42,     0,     0,     0,     0,     0,     0,     0,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,    45,     0,     0,     0,     0,    46,    47,
       0,     0,     0,    48,     0,    49,     0,    50,     0,    51,
       0,     0,     0,   270,     1,     2,     3,     4,     0,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,    22,
      23,   142,     0,   143,     0,     0,     0,    24,   144,    25,
      26,     0,    27,   145,    28,     0,    29,   146,    30,    31,
      32,     0,     0,    33,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    37,    38,    39,     0,   147,     0,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,    42,     0,     0,     0,     0,     0,
       0,     0,   148,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    44,     0,     0,    45,     0,     0,     0,     0,
      46,    47,     0,     0,     0,    48,     0,    49,     0,    50,
       0,    51,     0,     0,     0,   437,     1,     2,     3,     4,
       0,     5,     6,     7,     8,     9,    10,    11,     0,     0,
      12,    13,    14,    15,    16,    17,    18,    19,    20,     0,
       0,    22,    23,     0,     0,  -335,     0,     0,     0,    24,
       0,    25,    26,     0,    27,     0,    28,     0,    29,     0,
      30,    31,    32,     0,     0,    33,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    37,    38,    39,     0,
       0,  -335,  -335,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   209,   210,   211,
       0,   212,   213,   214,     0,    41,    42,     0,     0,     0,
       0,     0,     0,     0,    43,     0,  -335,  -335,     0,   222,
     223,   224,   225,   226,    44,     0,     0,    45,     0,   227,
       0,   228,    46,    47,   229,     0,     0,    48,     0,    49,
       0,    50,     0,    51,     0,     0,     0,    -3,     1,     2,
       3,     4,     0,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    13,    14,    15,    16,    17,    18,    19,
      20,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,    24,     0,    25,    26,     0,    27,     0,    28,     0,
      29,     0,    30,    31,    32,     0,     0,    33,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    37,    38,
      39,     0,     0,     0,     0,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    41,    42,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,     0,     0,    45,
       0,     0,     0,     0,    46,    47,     0,     0,     0,    48,
       0,    49,     0,    50,     0,    51,     0,     0,     0,   -15,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    13,    14,    15,    16,    17,
      18,    19,    20,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,    39,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    41,
      42,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,     0,
       0,    45,     0,     0,     0,     0,    46,    47,     0,     0,
       0,    48,     0,    49,     0,    50,     0,    51,     0,     0,
       0,   -16,     1,     2,     3,     4,     0,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    13,    14,    15,
      16,    17,    18,    19,    20,     0,     0,    22,    23,     0,
       0,     0,     0,     0,     0,    24,     0,    25,    26,     0,
      27,     0,    28,     0,    29,     0,    30,    31,    32,     0,
       0,    33,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    37,    38,    39,     0,     0,     0,     0,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,    42,     0,     0,     0,     0,     0,     0,     0,
     547,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,    45,     0,     0,     0,     0,    46,    47,
       0,     0,     0,    48,     0,    49,     0,    50,     0,    51,
       0,     0,     0,  -146,     1,     2,     3,     4,     0,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    13,
      14,    15,    16,    17,    18,    19,    20,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,    24,     0,    25,
      26,     0,    27,     0,    28,     0,    29,     0,    30,    31,
      32,     0,     0,    33,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    37,    38,    39,     0,     0,     0,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,    42,     0,     0,     0,     0,     0,
       0,     0,   547,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    44,     0,     0,    45,     0,     0,     0,     0,
      46,    47,     0,     0,     0,    48,     0,    49,     0,    50,
       0,    51,     0,    -3,     0,  -147,     1,     2,     3,     4,
       0,     5,     6,     7,     8,     9,    10,    11,     0,     0,
      12,    13,    14,    15,    16,    17,    18,    19,    20,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,    24,
       0,    25,    26,     0,    27,     0,    28,     0,    29,     0,
      30,    31,    32,     0,     0,    33,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    37,    38,    39,     0,
       0,     0,     0,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    41,    42,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,     0,    45,     0,     0,
       0,     0,    46,    47,     0,     0,     0,    48,     0,    49,
       0,    50,     0,    51,     1,     2,     3,     4,     0,     5,
       6,     7,     8,     9,    10,    11,     0,  -122,     0,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,    22,
      23,   142,     0,   143,     0,     0,     0,    24,   144,    25,
      26,     0,    27,   145,    28,     0,    29,   146,    30,    31,
      32,     0,     0,    33,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    37,    38,    39,     0,   147,     0,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,    42,     0,     0,     0,     0,     0,
       0,     0,   148,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    44,     0,     0,    45,     0,     0,     0,     0,
      46,    47,     0,     0,     0,    48,     0,    49,     0,    50,
       0,    51,     1,     2,     3,     4,     0,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    13,    14,    15,
      16,    17,    18,    19,    20,    21,     0,    22,    23,   142,
       0,   143,     0,     0,     0,    24,   144,    25,    26,     0,
      27,   145,    28,     0,    29,   146,    30,    31,    32,     0,
       0,    33,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    37,    38,    39,     0,   147,     0,     0,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,    42,     0,     0,     0,     0,     0,     0,     0,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,    45,     0,     0,     0,     0,    46,    47,
       0,     0,     0,    48,     0,    49,     0,    50,   164,    51,
       3,     4,     0,     5,     6,     7,     0,     0,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,    19,
       0,    21,     0,    22,    23,     0,     0,   143,     0,     0,
       0,    24,     0,     0,     0,     0,     0,     0,    28,     0,
      29,   165,     0,     0,     0,     0,     0,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,    40,     0,     0,     0,
     107,     0,     3,     4,     0,     5,     6,     7,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    41,    42,     0,
       0,    19,     0,    21,     0,    22,    23,     0,     0,     0,
       0,     0,     0,    24,     0,     0,    44,     0,     0,    45,
      28,     0,    29,     0,    46,    47,     0,     0,     0,    48,
      34,    49,    35,    36,     0,    51,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    41,
      42,     0,     0,     0,     0,     0,   186,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,     0,
       0,    45,     0,     0,     0,     0,    46,    47,     0,     0,
       0,    48,     0,    49,   187,   188,   189,    51,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,     0,     0,     0,
     215,   216,     0,   217,   218,   219,   186,   220,   221,     0,
     222,   223,   224,   225,   226,     0,     0,     0,     0,     0,
     227,     0,   228,     0,     0,   229,     0,     0,     0,     0,
       0,   370,     0,     0,   187,   188,   189,     0,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,     0,     0,     0,
     215,   216,     0,   217,   218,   219,   186,   220,   221,     0,
     222,   223,   224,   225,   226,     0,     0,     0,     0,     0,
     227,     0,   228,     0,     0,   229,     0,     0,     0,     0,
       0,   415,     0,     0,   187,   188,   189,     0,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,     0,     0,     0,
     215,   216,     0,   217,   218,   219,   186,   220,   221,     0,
     222,   223,   224,   225,   226,     0,     0,     0,     0,     0,
     227,     0,   228,     0,     0,   229,     0,     0,     0,     0,
       0,   502,     0,     0,   187,   188,   189,     0,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,     0,   186,     0,
     215,   216,   405,   217,   218,   219,     0,   220,   221,     0,
     222,   223,   224,   225,   226,     0,     0,     0,     0,     0,
     227,     0,   228,     0,     0,   229,   187,   188,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,     0,
     209,   210,   211,     0,   212,   213,   214,     0,     0,     0,
     186,     0,   215,   216,     0,   217,   218,   219,     0,   220,
     221,     0,   222,   223,   224,   225,   226,     0,     0,     0,
       0,     0,   227,     0,   228,   407,     0,   229,   187,   188,
     189,     0,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,     0,   209,   210,   211,     0,   212,   213,   214,     0,
       0,     0,   186,     0,   215,   216,     0,   217,   218,   219,
       0,   220,   221,     0,   222,   223,   224,   225,   226,     0,
       0,     0,     0,     0,   227,     0,   228,   461,     0,   229,
     187,   188,   189,     0,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,     0,   209,   210,   211,     0,   212,   213,
     214,     0,     0,     0,   186,     0,   215,   216,     0,   217,
     218,   219,     0,   220,   221,     0,   222,   223,   224,   225,
     226,     0,     0,     0,     0,     0,   227,     0,   228,   501,
       0,   229,   187,   188,   189,     0,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,     0,   209,   210,   211,     0,
     212,   213,   214,     0,     0,     0,   186,     0,   215,   216,
       0,   217,   218,   219,     0,   220,   221,     0,   222,   223,
     224,   225,   226,     0,     0,     0,     0,     0,   227,     0,
     228,   503,     0,   229,   187,   188,   189,     0,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,     0,   186,     0,
     215,   216,   564,   217,   218,   219,     0,   220,   221,     0,
     222,   223,   224,   225,   226,     0,     0,     0,     0,     0,
     227,     0,   228,     0,     0,   229,   187,   188,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,     0,
     209,   210,   211,     0,   212,   213,   214,     0,     0,     0,
     186,     0,   215,   216,     0,   217,   218,   219,     0,   220,
     221,     0,   222,   223,   224,   225,   226,     0,     0,     0,
       0,     0,   227,     0,   228,     0,     0,   229,   187,   188,
     189,     0,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,     0,   209,   210,   211,     0,   212,   213,   214,     0,
       0,     0,   186,     0,   215,   216,     0,   217,   218,   219,
       0,     0,   221,     0,   222,   223,   224,   225,   226,     0,
       0,     0,     0,     0,   227,     0,   228,     0,     0,   229,
     187,   188,   189,     0,   190,   191,   192,   193,   194,   195,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   208,     0,   209,   210,   211,     0,   212,   213,
     214,     0,     0,     0,   186,     0,     0,     0,     0,   217,
     218,   219,     0,   220,   221,     0,   222,   223,   224,   225,
     226,     0,     0,     0,     0,     0,   227,     0,   228,     0,
       0,   229,   187,   188,   189,     0,   190,   191,   192,   193,
     194,   195,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   209,   210,   211,     0,
     212,   213,   214,     0,     0,     0,   186,     0,     0,     0,
       0,   217,   218,   219,     0,   220,   221,     0,   222,   223,
     224,   225,   226,     0,     0,     0,     0,     0,   227,     0,
     228,     0,     0,   229,   187,   188,   189,     0,   190,   191,
     192,   193,   194,   195,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,     0,   186,     0,
       0,     0,     0,     0,   218,   219,     0,   220,   221,     0,
     222,   223,   224,   225,   226,     0,     0,     0,     0,     0,
     227,     0,   228,     0,     0,   229,   187,   188,   189,     0,
     190,   191,   192,   193,   194,   195,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     209,   210,   211,     0,   212,   213,   214,     0,     0,     0,
     186,     0,     0,     0,     0,     0,     0,   219,     0,   220,
     221,     0,   222,   223,   224,   225,   226,     0,     0,     0,
       0,     0,   227,     0,   228,     0,     0,   229,   187,   188,
     189,     0,   190,   191,   192,   193,   194,   195,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   209,   210,   211,     0,   212,   213,   214,     0,
       0,     0,   186,     0,     0,     0,     0,     0,     0,  -335,
       0,   220,   221,     0,   222,   223,   224,   225,   226,     0,
       0,     0,     0,     0,   227,     0,   228,     0,     0,   229,
     187,   188,   189,     0,  -335,  -335,  -335,  -335,   194,   195,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   209,   210,   211,     0,   212,   213,
     214,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   220,   221,     0,   222,   223,   224,   225,
     226,     0,     0,     0,     0,     0,   227,     0,   228,     0,
       0,   229
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,     0,   106,    19,    67,    60,   139,
      97,    24,   144,   145,   117,   184,    50,   274,     3,   363,
       3,    50,     3,   442,     3,    38,    12,     5,    41,    42,
       5,     4,    45,    46,    47,    48,    49,    97,   531,     5,
     229,    27,   357,   500,   457,    18,    94,    20,    21,   104,
      23,   106,    25,     0,    54,    54,    54,    54,    54,     0,
     102,     0,   537,    69,   102,    38,   541,   542,    12,   562,
      91,     3,     4,    46,    47,    48,    49,   534,   126,   124,
      34,   129,   426,    27,    16,    17,   499,   124,   124,   564,
     150,    16,   129,   135,     3,   124,   560,   135,   104,   112,
     232,    35,   115,   418,   117,    27,   581,    54,    62,   124,
      95,    33,   576,    54,    95,    54,   591,    51,   112,   124,
     289,   253,   116,    16,   227,   255,   139,   316,   101,   473,
       3,   144,   145,     3,     4,   128,   393,   112,     3,     0,
       5,     3,     4,   275,   276,   128,    16,    17,   124,   128,
     128,   128,   341,   128,    16,    17,   128,   129,     3,   102,
     124,   134,   128,   163,   163,   163,   163,   163,   425,   182,
     589,   124,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   128,     3,   129,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   163,     3,     4,   232,
     104,   234,   163,   412,   163,   211,   134,   106,   397,   398,
      16,    17,     3,     4,   102,   235,   235,   235,   272,   252,
     253,   127,   255,   229,   257,    16,    17,   117,   102,     3,
       3,   237,     3,   128,   124,   117,   126,   360,     5,     4,
     130,   247,   275,   276,    16,   522,    69,     3,   130,   101,
       3,     4,    69,    18,   101,    20,    21,     3,    23,     3,
      25,     3,   414,    16,    17,    12,   543,   129,   389,   129,
     391,   124,   135,    38,   102,     3,     4,     3,     4,   429,
     135,    46,    47,    48,    49,    92,    93,    94,    16,    17,
      16,    17,   372,   106,   102,   134,    54,     3,     3,     3,
       4,   104,   382,    54,   104,   104,   413,   340,    36,    91,
     316,   117,    16,    17,   135,    91,    91,   124,   595,   126,
     126,   428,   129,   444,    16,    91,   117,   360,   124,   349,
     349,   349,   439,   413,    26,   341,   101,   357,   357,   357,
      32,   135,   134,   128,   111,   112,     3,   135,   428,   129,
       3,   134,     5,   359,    46,     8,     9,    10,   135,   439,
     101,   128,   102,   106,   117,   104,   438,   135,   489,   134,
     403,    15,   405,   126,   124,   408,   128,   410,   102,   134,
     128,   414,   102,   112,    66,    92,    93,    94,   421,   117,
     135,   117,   128,   104,   128,   116,   429,   134,   418,   418,
     418,   434,   135,   134,   106,   128,   412,   114,   115,   116,
     117,   118,   519,   135,   521,     4,   124,   124,   111,   126,
     134,     3,   129,     5,     6,     3,     8,     9,    10,    18,
       3,    20,    21,    26,    23,   134,    25,    19,   116,   519,
     128,   521,    24,   128,    26,   134,    28,    29,   134,    38,
     134,    92,    93,    94,   134,     3,   135,   112,    47,    48,
      49,   134,   124,    45,   497,   112,   134,   128,   135,   134,
     490,    53,   505,    55,    56,   116,   117,   118,    54,   349,
     163,   449,    72,   124,    16,   126,    60,   143,   129,    71,
     381,   479,   479,   508,    26,   529,   508,   570,   557,    31,
      32,   552,   450,   523,   523,   523,   523,   523,   289,   529,
      92,    93,    44,   471,    46,    27,    -1,   444,    48,   422,
      -1,   256,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   552,   552,   552,   552,   552,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,    -1,    -1,   130,    -1,
     570,   570,   570,   570,   570,     0,    -1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    28,    29,    -1,    -1,    40,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    41,    -1,    43,    -1,
      45,    -1,    47,    48,    49,    -1,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    -1,    68,    69,    70,    63,    64,
      65,    -1,    -1,    76,    77,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    -1,    96,    97,    98,    -1,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,   111,   112,
      -1,   114,   115,   116,   117,   118,   111,    -1,    -1,   114,
      -1,   124,    -1,   126,   119,   120,   129,    -1,    -1,   124,
      -1,   126,    -1,   128,    -1,   130,    -1,    -1,    -1,   134,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    -1,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    36,    37,    38,    39,    -1,    41,    42,
      43,    -1,    45,    46,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    -1,    67,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,
      -1,   124,    -1,   126,    -1,   128,    -1,   130,    -1,    -1,
      -1,   134,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    13,    14,    -1,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    36,    37,    38,    39,    -1,
      41,    42,    43,    -1,    45,    46,    47,    48,    49,    -1,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    -1,    67,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,   119,   120,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,    -1,   130,
      -1,    -1,    -1,   134,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    36,    37,    38,
      39,    -1,    41,    42,    43,    -1,    45,    46,    47,    48,
      49,    -1,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    -1,    67,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
     119,   120,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
      -1,   130,    -1,    -1,    -1,   134,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    28,    29,    -1,    -1,    40,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    41,    -1,    43,    -1,    45,    -1,
      47,    48,    49,    -1,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,
      -1,    76,    77,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      -1,    96,    97,    98,    -1,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   111,   112,    -1,   114,
     115,   116,   117,   118,   111,    -1,    -1,   114,    -1,   124,
      -1,   126,   119,   120,   129,    -1,    -1,   124,    -1,   126,
      -1,   128,    -1,   130,    -1,    -1,    -1,   134,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    13,    14,
      -1,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    41,    -1,    43,    -1,
      45,    -1,    47,    48,    49,    -1,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,   119,   120,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,    -1,   130,    -1,    -1,    -1,   134,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    -1,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,
      43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,
      -1,   124,    -1,   126,    -1,   128,    -1,   130,    -1,    -1,
      -1,   134,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    13,    14,    -1,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      41,    -1,    43,    -1,    45,    -1,    47,    48,    49,    -1,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,   119,   120,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,    -1,   130,
      -1,    -1,    -1,   134,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    -1,    41,    -1,    43,    -1,    45,    -1,    47,    48,
      49,    -1,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
     119,   120,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
      -1,   130,    -1,     0,    -1,   134,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    41,    -1,    43,    -1,    45,    -1,
      47,    48,    49,    -1,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,   119,   120,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,    -1,   130,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    13,    14,    -1,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    36,    37,    38,
      39,    -1,    41,    42,    43,    -1,    45,    46,    47,    48,
      49,    -1,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    -1,    67,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
     119,   120,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
      -1,   130,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    13,    14,    -1,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    36,    37,    38,    39,    -1,
      41,    42,    43,    -1,    45,    46,    47,    48,    49,    -1,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    -1,    67,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,   119,   120,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,     3,   130,
       5,     6,    -1,     8,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    28,    29,    -1,    -1,    32,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
       3,    -1,     5,     6,    -1,     8,     9,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    92,    93,    -1,
      -1,    24,    -1,    26,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,   111,    -1,    -1,   114,
      43,    -1,    45,    -1,   119,   120,    -1,    -1,    -1,   124,
      53,   126,    55,    56,    -1,   130,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,
      -1,   124,    -1,   126,    68,    69,    70,   130,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,   107,   108,   109,    40,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,   107,   108,   109,    40,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,   107,   108,   109,    40,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    -1,    -1,    -1,    40,    -1,
     104,   105,   106,   107,   108,   109,    -1,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,   129,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    96,    97,    98,    -1,    -1,    -1,
      40,    -1,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,    -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,    -1,   129,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    -1,
      -1,    -1,    40,    -1,   104,   105,    -1,   107,   108,   109,
      -1,   111,   112,    -1,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,   129,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,    -1,    -1,    40,    -1,   104,   105,    -1,   107,
     108,   109,    -1,   111,   112,    -1,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
      -1,   129,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      96,    97,    98,    -1,    -1,    -1,    40,    -1,   104,   105,
      -1,   107,   108,   109,    -1,   111,   112,    -1,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,    -1,   129,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    -1,    -1,    -1,    40,    -1,
     104,   105,   106,   107,   108,   109,    -1,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,   129,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    96,    97,    98,    -1,    -1,    -1,
      40,    -1,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,    -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,    -1,   129,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    -1,
      -1,    -1,    40,    -1,   104,   105,    -1,   107,   108,   109,
      -1,    -1,   112,    -1,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,   129,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   107,
     108,   109,    -1,   111,   112,    -1,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,
      -1,   129,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,
      96,    97,    98,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   107,   108,   109,    -1,   111,   112,    -1,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,    -1,   129,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    -1,    96,    97,    98,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,   129,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    -1,    96,    97,    98,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,
     112,    -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,    -1,   129,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    -1,    96,    97,    98,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,   111,   112,    -1,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,   129,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,
      -1,   129
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     5,     6,     8,     9,    10,    11,    12,
      13,    14,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    28,    29,    36,    38,    39,    41,    43,    45,
      47,    48,    49,    52,    53,    55,    56,    63,    64,    65,
      71,    92,    93,   101,   111,   114,   119,   120,   124,   126,
     128,   130,   137,   138,   139,   140,   146,   149,   150,   156,
     160,   161,   162,   163,   165,   173,   181,   184,   185,   188,
     191,   192,   193,   194,   198,   200,   209,   214,   215,   216,
     226,   234,   237,   250,   251,   252,   257,   261,   265,   266,
     268,   269,   270,    91,    50,   124,   124,   166,   124,     3,
       4,    16,    17,   128,   186,   187,    16,     3,   261,     3,
     236,   261,   124,   187,   240,   124,   128,   124,   129,    16,
     261,   124,     3,     5,     8,     9,    10,   258,   259,   260,
     261,   261,   261,   261,   261,   256,   261,   246,   247,   248,
     254,   261,    30,    32,    37,    42,    46,    67,   101,   134,
     145,   146,   147,   150,   152,   167,   168,   183,   192,   253,
       3,     0,   140,   128,     3,    46,   152,   158,   159,   257,
     147,   148,    16,    26,    31,    32,    44,    46,   208,   267,
     194,     3,   128,   221,   225,   102,    40,    68,    69,    70,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    92,
      93,    94,    96,    97,    98,   104,   105,   107,   108,   109,
     111,   112,   114,   115,   116,   117,   118,   124,   126,   129,
       3,   124,   164,   148,   174,   190,   128,   129,   186,   187,
     239,   240,   241,   249,   124,   255,   256,   129,   261,   182,
     246,     3,   104,   157,   134,   106,   102,   102,   135,   127,
     102,   254,     3,     3,   153,   154,   255,   255,   153,     3,
     134,   147,   128,   138,     3,   101,    69,   101,     3,    35,
      51,   233,     3,     3,   210,   261,     3,   221,   227,   228,
     229,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   117,   130,   186,   264,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   246,   261,     3,     4,   117,   124,
     126,   130,   263,   264,   255,    12,   261,   101,   141,   142,
     143,   149,   150,   198,   200,   209,   234,   189,   186,   129,
     124,   245,     3,    95,   230,   231,   232,   135,   117,   186,
     135,   144,   145,   135,   261,   255,   254,   259,   261,   106,
     244,   102,   145,   134,   244,   255,   255,   104,     3,    54,
     195,    54,   196,     3,   211,   212,   222,   104,   104,   112,
     116,   217,   229,   126,   264,   106,   135,   127,   262,   261,
     126,   264,    91,   135,   124,   135,   134,   143,   128,   141,
     246,   104,   106,   213,   232,   135,   102,   134,   135,   106,
      36,   117,   241,   243,   104,   151,   154,   134,   101,   135,
       3,     5,   124,   241,    15,   197,   241,   242,   128,   244,
     102,   134,     5,   221,     5,   221,   218,   223,   261,   261,
     261,   127,   261,   264,   148,   255,   128,   141,   134,   135,
     261,   243,   244,    95,   232,    27,    33,   175,   177,   179,
     180,   148,   254,   261,   257,   148,   230,   242,   128,   102,
     201,   151,   212,   112,     5,   111,   112,   128,   219,   224,
     226,   127,   135,   127,   135,    34,    62,   169,   170,   171,
     172,   134,   213,   128,   232,   128,   124,   175,   177,    66,
     155,   135,   135,   199,   241,    46,   101,   192,   205,   206,
     207,   116,   261,   111,   226,   223,   261,   106,   134,   171,
     172,   238,   178,     3,   148,   148,   244,   101,   146,   150,
     192,   202,   203,   204,   209,   234,     3,    26,   134,   207,
     225,   220,   116,   223,   106,   144,   144,   144,   244,   128,
     128,   134,   204,   233,   218,   134,   225,   144,   134,   134,
     135,   235,   202,     3,   112,   218,   176,   144,   134,   124,
     112,   128,   134,   230,   144,   135,   134,   244
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        
    case 13:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1899 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 24:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1911 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 26:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1914 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1915 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1917 "parser.y"
    {
    (yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));
}
    }
    break;


  
    case 29:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1920 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 43:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1936 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 44:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1937 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 45:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1940 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 52:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1947 "parser.y"
    {
    PASS_ALWAYS 
    if(as3_pass) {
        (yyval.code) = (yyvsp[(3) - (4)].code);
    } else {
        (yyval.code) = 0;
    }
    as3_pass=(yyvsp[(1) - (4)].number_int);
}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1959 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1960 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 55:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1964 "parser.y"
    {
    code_t**cc = &global->init->method->body->code;
    *cc = code_append(*cc, (yyvsp[(1) - (1)].code));
}
    }
    break;


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1971 "parser.y"
    {
    PASS12
    (yyval.number_int)=as3_pass;
    char*key = concat3((yyvsp[(1) - (3)].id),"::",(yyvsp[(3) - (3)].id));
    if(!definitions || !dict_contains(definitions, key)) {
        as3_pass=0;
    }
    free(key);
}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1990 "parser.y"
    {(yyval.node)=(yyvsp[(2) - (2)].node);}
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1991 "parser.y"
    {(yyval.node)=mkdummynode();}
    }
    break;


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1993 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1994 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1996 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1997 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 63:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2000 "parser.y"
    {
PASS12
    if(variable_exists((yyvsp[(1) - (3)].id)))
        syntaxerror("Variable %s already defined", (yyvsp[(1) - (3)].id));
PASS1
    new_variable((yyvsp[(1) - (3)].id), 0, 1, 0);
PASS2
   
    char slot = 0;
    int index = 0;
    if(state->method->uses_slots) {
        variable_t* v = find_slot(state, (yyvsp[(1) - (3)].id));
        if(v && !v->init) {
            // this variable is stored in a slot
            v->init = 1;
            v->type = (yyvsp[(2) - (3)].classinfo);
            slot = 1;
            index = v->index;
        }
    }
    if(!index) {
        index = new_variable((yyvsp[(1) - (3)].id), (yyvsp[(2) - (3)].classinfo), 1, 0);
    }

    (yyval.code) = slot?abc_getscopeobject(0, 1):0;
    
    typedcode_t v = node_read((yyvsp[(3) - (3)].node));
    if(!is_subtype_of(v.t, (yyvsp[(2) - (3)].classinfo))) {
        syntaxerror("Can't convert %s to %s", v.t->name, (yyvsp[(2) - (3)].classinfo)->name);
    }
    if((yyvsp[(2) - (3)].classinfo)) {
        if(v.c->prev || v.c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = code_append((yyval.code), v.c);
            (yyval.code) = converttype((yyval.code), v.t, (yyvsp[(2) - (3)].classinfo));
        } else {
            code_free(v.c);
            (yyval.code) = defaultvalue((yyval.code), (yyvsp[(2) - (3)].classinfo));
        }
    } else {
        if(v.c->prev || v.c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = code_append((yyval.code), v.c);
            (yyval.code) = abc_coerce_a((yyval.code));
        } else {
            // don't do anything
            code_free(v.c);
            code_free((yyval.code));
            (yyval.code) = 0;
            break;
        }
    }
    if(slot) {
        (yyval.code) = abc_setslot((yyval.code), index);
    } else {
        (yyval.code) = abc_setlocal((yyval.code), index);
    }
}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2059 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2060 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2063 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 67:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2063 "parser.y"
    {
     
    (yyval.code) = code_new();
    (yyval.code) = code_append((yyval.code), (yyvsp[(4) - (7)].value).c);
    code_t*myjmp,*myif = (yyval.code) = abc_iffalse((yyval.code), 0);
   
    (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (7)].code));
    if((yyvsp[(7) - (7)].code)) {
        myjmp = (yyval.code) = abc_jump((yyval.code), 0);
    }
    myif->branch = (yyval.code) = abc_nop((yyval.code));
    if((yyvsp[(7) - (7)].code)) {
        (yyval.code) = code_append((yyval.code), (yyvsp[(7) - (7)].code));
        myjmp->branch = (yyval.code) = abc_nop((yyval.code));
    }
    (yyval.code) = var_block((yyval.code));
    PASS12 old_state();
}
    }
    break;


  
    case 68:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2082 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2089 "parser.y"
    {
    PASS1 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),0,1,0);
    PASS2 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
}
    }
    break;


  
    case 72:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2093 "parser.y"
    {
    PASS12
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 73:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2098 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2099 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 75:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2101 "parser.y"
    {
    if((yyvsp[(1) - (8)].for_start).each) syntaxerror("invalid syntax: ; not allowed in for each statement");
    (yyval.code) = code_new();
    (yyval.code) = code_append((yyval.code), (yyvsp[(2) - (8)].code));
    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(4) - (8)].value).c);
    code_t*myif = (yyval.code) = abc_iffalse((yyval.code), 0);
    (yyval.code) = code_append((yyval.code), (yyvsp[(8) - (8)].code));
    code_t*cont = (yyval.code) = abc_nop((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (8)].code));
    (yyval.code) = abc_jump((yyval.code), loopstart);
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (8)].for_start).name, out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (8)].for_start).name, cont);
    myif->branch = out;

    (yyval.code) = var_block((yyval.code));
    PASS12 old_state();
}
    }
    break;


  
    case 76:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2121 "parser.y"
    {
    variable_t*var = find_variable(state, (yyvsp[(2) - (6)].id));
    if(!var) {
        syntaxerror("variable %s not known in this scope", (yyvsp[(2) - (6)].id));
    }

    char*tmp1name = concat2((yyvsp[(2) - (6)].id), "__tmp1__");
    int it = new_variable(tmp1name, TYPE_INT, 0, 0);
    char*tmp2name = concat2((yyvsp[(2) - (6)].id), "__array__");
    int array = new_variable(tmp1name, 0, 0, 0);

    (yyval.code) = code_new();
    (yyval.code) = code_append((yyval.code), (yyvsp[(4) - (6)].value).c);
    (yyval.code) = abc_coerce_a((yyval.code));
    (yyval.code) = abc_setlocal((yyval.code), array);
    (yyval.code) = abc_pushbyte((yyval.code), 0);
    (yyval.code) = abc_setlocal((yyval.code), it);

    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    
    (yyval.code) = abc_hasnext2((yyval.code), array, it);
    code_t*myif = (yyval.code) = abc_iffalse((yyval.code), 0);
    (yyval.code) = abc_getlocal((yyval.code), array);
    (yyval.code) = abc_getlocal((yyval.code), it);
    if(!(yyvsp[(1) - (6)].for_start).each)
        (yyval.code) = abc_nextname((yyval.code));
    else
        (yyval.code) = abc_nextvalue((yyval.code));
    (yyval.code) = converttype((yyval.code), 0, var->type);
    (yyval.code) = abc_setlocal((yyval.code), var->index);

    (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (6)].code));
    (yyval.code) = abc_jump((yyval.code), loopstart);
    
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (6)].for_start).name, out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (6)].for_start).name, loopstart);
    
    myif->branch = out;

    (yyval.code) = var_block((yyval.code));

    free(tmp1name);
    free(tmp2name);

    PASS12 old_state();
}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2169 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 78:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2169 "parser.y"
    {

    (yyval.code) = code_new();

    code_t*myjmp = (yyval.code) = abc_jump((yyval.code), 0);
    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (6)].code));
    code_t*cont = (yyval.code) = abc_nop((yyval.code));
    myjmp->branch = cont;
    (yyval.code) = code_append((yyval.code), (yyvsp[(4) - (6)].value).c);
    (yyval.code) = abc_iftrue((yyval.code), loopstart);
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (6)].id), out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (6)].id), cont);

    (yyval.code) = var_block((yyval.code));
    PASS12 old_state();
}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2188 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 80:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2188 "parser.y"
    {
    (yyval.code) = code_new();
    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(3) - (7)].code));
    code_t*cont = (yyval.code) = abc_nop((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (7)].value).c);
    (yyval.code) = abc_iftrue((yyval.code), loopstart);
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (7)].id), out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (7)].id), cont);
    
    (yyval.code) = var_block((yyval.code));
    PASS12 old_state();
}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2203 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2206 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2209 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2212 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2216 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2217 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2218 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2219 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2220 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2221 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 91:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2223 "parser.y"
    {
    (yyval.code) = abc_getlocal(0, state->switch_var);
    (yyval.code) = code_append((yyval.code), node_read((yyvsp[(2) - (4)].node)).c);
    code_t*j = (yyval.code) = abc_ifne((yyval.code), 0);
    (yyval.code) = code_append((yyval.code), (yyvsp[(4) - (4)].code));
    if((yyval.code)->opcode != OPCODE___BREAK__) {
        (yyval.code) = abc___fallthrough__((yyval.code), "");
    }
    code_t*e = (yyval.code) = abc_nop((yyval.code));
    j->branch = e;
}
    }
    break;


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2234 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2237 "parser.y"
    {PASS12 new_state();state->switch_var=alloc_local();}
    }
    break;


  
    case 94:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2237 "parser.y"
    {
    (yyval.code) = node_read((yyvsp[(4) - (8)].node)).c;
    (yyval.code) = abc_setlocal((yyval.code), state->switch_var);
    (yyval.code) = code_append((yyval.code), (yyvsp[(7) - (8)].code));

    code_t*out = (yyval.code) = abc_kill((yyval.code), state->switch_var);
    breakjumpsto((yyval.code), (yyvsp[(1) - (8)].id), out);
    
    code_t*c = (yyval.code),*lastblock=0;
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
   
    (yyval.code) = var_block((yyval.code));
    PASS12 old_state();
}
    }
    break;


  
    case 95:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2268 "parser.y"
    {PASS12 new_state();
                                                      state->exception_name=(yyvsp[(3) - (5)].id);
                                               PASS1 new_variable((yyvsp[(3) - (5)].id), 0, 0, 0);
                                               PASS2 new_variable((yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0, 0);
                                              }
    }
    break;


  
    case 96:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2273 "parser.y"
    {
    namespace_t name_ns = {ACCESS_PACKAGE, ""};
    multiname_t name = {QNAME, &name_ns, 0, (yyvsp[(3) - (9)].id)};
    
    NEW(abc_exception_t, e)
    e->exc_type = sig2mname((yyvsp[(4) - (9)].classinfo));
    e->var_name = multiname_clone(&name);
    (yyval.exception) = e;

    code_t*c = 0;
    int i = find_variable_safe(state, (yyvsp[(3) - (9)].id))->index;
    e->target = c = abc_nop(0);
    c = abc_setlocal(c, i);
    c = code_append(c, code_dup(state->method->scope_code));
    c = code_append(c, (yyvsp[(8) - (9)].code));
    c = abc_kill(c, i);

    c = var_block(c);
    PASS12 old_state();
}
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2293 "parser.y"
    {PASS12 new_state();state->exception_name=0;}
    }
    break;


  
    case 98:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2293 "parser.y"
    {
    (yyvsp[(4) - (5)].code) = var_block((yyvsp[(4) - (5)].code));
    if(!(yyvsp[(4) - (5)].code)) {
        (yyval.exception)=0;
    } else {
        NEW(abc_exception_t, e)
        e->exc_type = 0; //all exceptions
        e->var_name = 0; //no name
        e->target = 0;
        e->to = abc_nop(0);
        e->to = code_append(e->to, (yyvsp[(4) - (5)].code));
        (yyval.exception) = e;
    }
    PASS12 old_state();
}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2309 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2310 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2311 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 102:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2312 "parser.y"
    {
    (yyval.catch_list) = (yyvsp[(1) - (2)].catch_list);
    (yyval.catch_list).finally = 0;
    if((yyvsp[(2) - (2)].exception)) {
        list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));
        (yyval.catch_list).finally = (yyvsp[(2) - (2)].exception)->to;(yyvsp[(2) - (2)].exception)->to=0;
    }
}
    }
    break;


  
    case 103:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2320 "parser.y"
    {
    (yyval.catch_list).l=list_new();
    (yyval.catch_list).finally = 0;
    if((yyvsp[(1) - (1)].exception)) {
        list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));
        (yyval.catch_list).finally = (yyvsp[(1) - (1)].exception)->to;(yyvsp[(1) - (1)].exception)->to=0;
    }
}
    }
    break;


  
    case 104:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2329 "parser.y"
    {PASS12 new_state();
                 state->method->has_exceptions=1;
                 state->method->late_binding=1;//for invariant scope_code
                }
    }
    break;


  
    case 105:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2332 "parser.y"
    {
    code_t*out = abc_nop(0);

    code_t*start = abc_nop(0);
    (yyval.code) = code_append(start, (yyvsp[(4) - (6)].code));
    if(!is_break_or_jump((yyvsp[(4) - (6)].code))) {
        (yyval.code) = abc_jump((yyval.code), out);
    }
    code_t*end = (yyval.code) = abc_nop((yyval.code));
  
    int tmp;
    if((yyvsp[(6) - (6)].catch_list).finally)
        tmp = new_variable("__finally__", 0, 0, 0);
    
    abc_exception_list_t*l = (yyvsp[(6) - (6)].catch_list).l;
    int count=0;
    while(l) {
        abc_exception_t*e = l->abc_exception;
        if(e->var_name) {
            (yyval.code) = code_append((yyval.code), e->target);
            (yyval.code) = abc_jump((yyval.code), out);
        } else {
            parserassert((ptroff_t)(yyvsp[(6) - (6)].catch_list).finally);
            // finally block
            e->target = (yyval.code) = abc_nop((yyval.code));
            (yyval.code) = code_append((yyval.code), code_dup(state->method->scope_code));
            (yyval.code) = abc___rethrow__((yyval.code));
        }
        
        e->from = start;
        e->to = end;

        l = l->next;
    }
    (yyval.code) = code_append((yyval.code), out);

    (yyval.code) = insert_finally((yyval.code), (yyvsp[(6) - (6)].catch_list).finally, tmp);
        
    list_concat(state->method->exceptions, (yyvsp[(6) - (6)].catch_list).l);
   
    (yyval.code) = var_block((yyval.code));
    PASS12 old_state();
}
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2378 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 107:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2382 "parser.y"
    {
    if(!state->exception_name)
        syntaxerror("re-throw only possible within a catch block");
    variable_t*v = find_variable(state, state->exception_name);
    (yyval.code)=code_new();
    (yyval.code)=abc_getlocal((yyval.code), v->index);
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 108:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2393 "parser.y"
    {
     new_state();
     if(state->method->has_exceptions) {
         int v = alloc_local();
         state->method->scope_code = abc_getlocal(state->method->scope_code, v);
         state->method->scope_code = abc_pushwith(state->method->scope_code);
         (yyval.value_list).number = v;
     }
     (yyval.value_list).cc = (yyvsp[(3) - (4)].value).c;
}
    }
    break;


  
    case 109:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2403 "parser.y"
    {
     /* remove getlocal;pushwith from scope code again */
     state->method->scope_code = code_cutlast(code_cutlast(state->method->scope_code));

     (yyval.code) = (yyvsp[(1) - (2)].value_list).cc;
     if(state->method->has_exceptions) {
         (yyval.code) = abc_dup((yyval.code));
         (yyval.code) = abc_setlocal((yyval.code), (yyvsp[(1) - (2)].value_list).number);
     }
     (yyval.code) = abc_pushwith((yyval.code));
     (yyval.code) = code_append((yyval.code), (yyvsp[(2) - (2)].code));
     (yyval.code) = abc_popscope((yyval.code));
     old_state();
}
    }
    break;


  
    case 111:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2421 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2422 "parser.y"
    {PASS12 (yyval.id)="namespace";}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2423 "parser.y"
    {PASS12 (yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2425 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2426 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2428 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 117:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2429 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 118:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2430 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2431 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2453 "parser.y"
    {
       PASS12
       slotinfo_t*s = registry_find((yyvsp[(2) - (2)].classinfo)->package, (yyvsp[(2) - (2)].classinfo)->name);
       if(!s && as3_pass==1) {// || !(s->flags&FLAG_BUILTIN)) {
           as3_schedule_class((yyvsp[(2) - (2)].classinfo)->package, (yyvsp[(2) - (2)].classinfo)->name);
       }
       classinfo_t*c = (yyvsp[(2) - (2)].classinfo);
       if(!c) 
            syntaxerror("Couldn't import class\n");
       state_has_imports();
       dict_put(state->imports, c->name, c);
       import_toplevel(c->package);
       (yyval.code)=0;
}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2467 "parser.y"
    {
       PASS12
       if(strncmp("flash.", (yyvsp[(2) - (4)].id), 6) && as3_pass==1) {
           as3_schedule_package((yyvsp[(2) - (4)].id));
       }

       NEW(import_t,i);
       i->package = (yyvsp[(2) - (4)].id);
       state_has_imports();
       list_append(state->wildcard_imports, i);
       import_toplevel(i->package);
       (yyval.code)=0;
}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2483 "parser.y"
    {PASS12 (yyval.flags).flags=0;(yyval.flags).ns=0;}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2484 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 124:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2485 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2486 "parser.y"
    {
    PASS12 
    (yyval.flags).flags=(yyvsp[(1) - (2)].flags).flags|(yyvsp[(2) - (2)].flags).flags;
    if((yyvsp[(1) - (2)].flags).ns && (yyvsp[(2) - (2)].flags).ns) syntaxerror("only one namespace allowed in one declaration");
    (yyval.flags).ns=(yyvsp[(1) - (2)].flags).ns?(yyvsp[(1) - (2)].flags).ns:(yyvsp[(2) - (2)].flags).ns;

}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2493 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PUBLIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2494 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PRIVATE;(yyval.flags).ns=0;}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2495 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PROTECTED;(yyval.flags).ns=0;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2496 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_STATIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2497 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_DYNAMIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2498 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_FINAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2499 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_OVERRIDE;(yyval.flags).ns=0;}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2500 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NATIVE;(yyval.flags).ns=0;}
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2501 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PACKAGEINTERNAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2502 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NAMESPACE;
                               (yyval.flags).ns=(yyvsp[(1) - (1)].id);
                       }
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2506 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2507 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2509 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2510 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2512 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2513 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 142:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2517 "parser.y"
    {PASS12 startclass(&(yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 143:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2519 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 144:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2523 "parser.y"
    {PASS12 (yyvsp[(1) - (5)].flags).flags|=FLAG_INTERFACE;
                                          startclass(&(yyvsp[(1) - (5)].flags),(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 145:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2526 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 151:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2535 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 154:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2539 "parser.y"
    {
    code_t*c = state->cls->static_init->header;
    c = code_append(c, (yyvsp[(1) - (1)].code));  
    state->cls->static_init->header = c;
}
    }
    break;


  
    case 160:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2550 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 161:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2553 "parser.y"
    {
    PASS12
    (yyvsp[(1) - (8)].flags).flags |= FLAG_PUBLIC;
    if((yyvsp[(1) - (8)].flags).flags&(FLAG_PRIVATE|FLAG_PACKAGEINTERNAL|FLAG_PROTECTED)) {
        syntaxerror("invalid method modifiers: interface methods always need to be public");
    }
    startfunction(&(yyvsp[(1) - (8)].flags),(yyvsp[(3) - (8)].token),(yyvsp[(4) - (8)].id),&(yyvsp[(6) - (8)].params),(yyvsp[(8) - (8)].classinfo));
    endfunction(&(yyvsp[(1) - (8)].flags),(yyvsp[(3) - (8)].token),(yyvsp[(4) - (8)].id),&(yyvsp[(6) - (8)].params),(yyvsp[(8) - (8)].classinfo), 0);
    list_deep_free((yyvsp[(6) - (8)].params).list);
}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2623 "parser.y"
    {PASS12 setslotstate(&(yyvsp[(1) - (2)].flags),(yyvsp[(2) - (2)].token));}
    }
    break;


  
    case 165:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2623 "parser.y"
    {PASS12 (yyval.code)=(yyvsp[(4) - (4)].code);setslotstate(0, 0);}
    }
    break;


  
    case 166:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2625 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2626 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2629 "parser.y"
    {
PASS12
    int flags = slotstate_flags->flags;
    namespace_t ns = modifiers2access(slotstate_flags);

    if(as3_pass == 1) {

        varinfo_t* info = 0;
        if(state->cls) {
            memberinfo_t*i = registry_findmember(state->cls->info, ns.name, (yyvsp[(1) - (3)].id), 1);
            if(i) {
                check_override(i, flags);
            }
            info = varinfo_register_onclass(state->cls->info, ns.access, ns.name, (yyvsp[(1) - (3)].id));
        } else {
            slotinfo_t*i = registry_find(state->package, (yyvsp[(1) - (3)].id));
            if(i) {
                syntaxerror("package %s already contains '%s'", state->package, (yyvsp[(1) - (3)].id));
            }
            if(ns.name && ns.name[0]) {
                syntaxerror("namespaces not allowed on package-level variables");
            }
            info = varinfo_register_global(ns.access, state->package, (yyvsp[(1) - (3)].id));
        }

        info->type = (yyvsp[(2) - (3)].classinfo);
        info->flags = flags;
        
        dict_put(global->token2info, (void*)(ptroff_t)as3_tokencount, info);
    }

    if(as3_pass == 2) {
        varinfo_t*info = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);

        multiname_t mname;
        code_t**code;
        trait_t*t = add_abc_slot(slotstate_flags, (yyvsp[(1) - (3)].id), &mname, &code);

        if((yyvsp[(2) - (3)].classinfo)) {
            MULTINAME(m, (yyvsp[(2) - (3)].classinfo));
            t->type_name = multiname_clone(&m);
        }
        info->slot = t->slot_id;
        
        /* workaround for "VerifyError: Error #1053: Illegal override of ::test2 in C1" 
           FIXME: is there a way to use slots and still don't have conflicting overrides?
        */
        info->slot = t->slot_id = 0;
       
        constant_t cval = (yyvsp[(3) - (3)].node)->type->eval((yyvsp[(3) - (3)].node));
        if(cval.type!=CONSTANT_UNKNOWN) {
            /* compile time constant */
            t->value = malloc(sizeof(constant_t));
            memcpy(t->value, &cval, sizeof(constant_t));
            info->value = constant_clone(t->value);
        } else {
            typedcode_t v = node_read((yyvsp[(3) - (3)].node));
            /* initalization code (if needed) */
            code_t*c = 0;
            if(v.c && !is_pushundefined(v.c)) {
                c = abc_getlocal_0(c);
                c = code_append(c, v.c);
                c = converttype(c, v.t, (yyvsp[(2) - (3)].classinfo));
                if(!t->slot_id) {
                    c = abc_initproperty2(c, &mname);
                } else {
                    c = abc_setslot(c, t->slot_id);
                }
            }
            *code = code_append(*code, c);
        }

        if(slotstate_varconst==KW_CONST) {
            t->kind= TRAIT_CONST;
            info->flags |= FLAG_CONST;
        }
    }

    (yyval.code)=0;
}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2712 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2713 "parser.y"
    {
  (yyval.constant) = malloc(sizeof(constant_t));
  *(yyval.constant) = node_eval((yyvsp[(2) - (2)].node));
  if((yyval.constant)->type == CONSTANT_UNKNOWN) {
    syntaxerror("can't evaluate default parameter value (needs to be a compile-time constant)");
  }
}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2722 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2723 "parser.y"
    {
    (yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));
}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2726 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2727 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2728 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2729 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2730 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 178:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2731 "parser.y"
    {(yyval.constant) = constant_new_undefined((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2732 "parser.y"
    {(yyval.constant) = constant_new_float(__builtin_nan(""));}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2752 "parser.y"
    {PASS_ALWAYS if(!xml_level++) tokenizer_begin_xml();}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2753 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2754 "parser.y"
    {PASS_ALWAYS if(!--xml_level) tokenizer_end_xml(); else tokenizer_begin_xmltext();}
    }
    break;


  
    case 184:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2756 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2756 "parser.y"
    {
    (yyval.id)=strdup("{...}");
    as3_warning("xml string substitution not yet supported");
}
    }
    break;


  
    case 186:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2760 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xml();}
    }
    break;


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2760 "parser.y"
    {
    (yyval.id)=strdup("{...}");
    as3_warning("xml string substitution not yet supported");
}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2764 "parser.y"
    {(yyval.id)="";}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2765 "parser.y"
    {
    (yyval.id) = concat2((yyvsp[(1) - (2)].id), "{...}");
}
    }
    break;


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2768 "parser.y"
    {(yyval.id)=concat2((yyvsp[(1) - (2)].id), string_cstr(&(yyvsp[(2) - (2)].str)));}
    }
    break;


  
    case 191:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2769 "parser.y"
    {(yyval.id)=concat2((yyvsp[(1) - (2)].id), ">");}
    }
    break;


  
    case 192:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2771 "parser.y"
    {(yyval.id)=concat2((yyvsp[(1) - (2)].id),(yyvsp[(2) - (2)].id));}
    }
    break;


  
    case 193:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2772 "parser.y"
    {(yyval.id)=concat3((yyvsp[(1) - (3)].id),(yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));free((yyvsp[(2) - (3)].id));free((yyvsp[(3) - (3)].id));}
    }
    break;


  
    case 194:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2774 "parser.y"
    {(yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2775 "parser.y"
    {(yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2777 "parser.y"
    {
    (yyval.id) = allocprintf("<%s%s>%s</%s>", (yyvsp[(2) - (10)].id), (yyvsp[(3) - (10)].id), (yyvsp[(5) - (10)].id), (yyvsp[(8) - (10)].id));
    free((yyvsp[(2) - (10)].id));free((yyvsp[(3) - (10)].id));free((yyvsp[(5) - (10)].id));free((yyvsp[(8) - (10)].id));
}
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2781 "parser.y"
    {
    (yyval.id) = allocprintf("<%s%s/>", (yyvsp[(2) - (6)].id), (yyvsp[(3) - (6)].id));
}
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2784 "parser.y"
    {
    (yyval.id) = allocprintf("<%s%s>%s%s</%s>", (yyvsp[(2) - (11)].id), (yyvsp[(3) - (11)].id), (yyvsp[(5) - (11)].id), (yyvsp[(6) - (11)].id), (yyvsp[(9) - (11)].id));
    free((yyvsp[(2) - (11)].id));free((yyvsp[(3) - (11)].id));free((yyvsp[(5) - (11)].id));free((yyvsp[(6) - (11)].id));free((yyvsp[(9) - (11)].id));
}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2789 "parser.y"
    {(yyval.id)=strdup("");}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2790 "parser.y"
    {(yyval.id)=concat2(" ",(yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2791 "parser.y"
    {(yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2792 "parser.y"
    {(yyval.id)=concat3((yyvsp[(1) - (2)].id)," ",(yyvsp[(2) - (2)].id));free((yyvsp[(1) - (2)].id));free((yyvsp[(2) - (2)].id));}
    }
    break;


  
    case 203:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2794 "parser.y"
    {
    (yyval.id) = strdup("{...}");
}
    }
    break;


  
    case 204:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2797 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(3) - (3)].str));
    (yyval.id) = concat2("{...}=",str);
}
    }
    break;


  
    case 205:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2801 "parser.y"
    {
    (yyval.id) = strdup("{...}={...}");
}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2804 "parser.y"
    {
    (yyval.id) = concat2((yyvsp[(1) - (3)].id),"={...}");
}
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2807 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(3) - (3)].str));
    (yyval.id)=allocprintf("%s=%s", (yyvsp[(1) - (3)].id),str);
    free(str);
    free((yyvsp[(1) - (3)].id));free((char*)(yyvsp[(3) - (3)].str).str);
}
    }
    break;


  
    case 208:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2817 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 209:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2821 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2827 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(2) - (2)].param));
}
    }
    break;


  
    case 211:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2833 "parser.y"
    {
    PASS12
    (yyval.params) =(yyvsp[(1) - (4)].params);
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(4) - (4)].param));
}
    }
    break;


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2841 "parser.y"
    {
    PASS12
    (yyval.params) = (yyvsp[(1) - (3)].params);
    list_append((yyval.params).list, (yyvsp[(3) - (3)].param));
}
    }
    break;


  
    case 213:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2846 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    list_append((yyval.params).list, (yyvsp[(1) - (1)].param));
}
    }
    break;


  
    case 214:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2852 "parser.y"
    {
     PASS12
     (yyval.param) = rfx_calloc(sizeof(param_t));
     (yyval.param)->name=(yyvsp[(1) - (4)].id);
     (yyval.param)->type = (yyvsp[(3) - (4)].classinfo);
     PASS2
     (yyval.param)->value = (yyvsp[(4) - (4)].constant);
}
    }
    break;


  
    case 215:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2860 "parser.y"
    {
     PASS12
     (yyval.param) = rfx_calloc(sizeof(param_t));
     (yyval.param)->name=(yyvsp[(1) - (2)].id);
     (yyval.param)->type = TYPE_ANY;
     PASS2
     (yyval.param)->value = (yyvsp[(2) - (2)].constant);
}
    }
    break;


  
    case 218:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2870 "parser.y"
    {PASS12 (yyval.token)=0;}
    }
    break;


  
    case 219:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2873 "parser.y"
    {PASS12 startfunction(&(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 220:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2874 "parser.y"
    {
    PASS1 
    endfunction(&(yyvsp[(1) - (12)].flags),(yyvsp[(3) - (12)].token),(yyvsp[(4) - (12)].id),&(yyvsp[(6) - (12)].params),0,0);
    PASS2
    if(!state->method->info) syntaxerror("internal error");
    
    code_t*c = method_header(state->method);
    c = wrap_function(c, 0, (yyvsp[(11) - (12)].code));

    endfunction(&(yyvsp[(1) - (12)].flags),(yyvsp[(3) - (12)].token),(yyvsp[(4) - (12)].id),&(yyvsp[(6) - (12)].params),(yyvsp[(8) - (12)].classinfo),c);
    PASS12
    list_deep_free((yyvsp[(6) - (12)].params).list);
    (yyval.code)=0;
}
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2890 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2892 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 224:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2893 "parser.y"
    {
    PASS1
    endfunction(0,0,(yyvsp[(2) - (10)].id),&(yyvsp[(4) - (10)].params),0,0);
    PASS2
    methodinfo_t*f = state->method->info;
    if(!f || !f->kind) syntaxerror("internal error");
    
    code_t*c = method_header(state->method);
    c = wrap_function(c, 0, (yyvsp[(9) - (10)].code));

    int index = state->method->var_index;
    endfunction(0,0,(yyvsp[(2) - (10)].id),&(yyvsp[(4) - (10)].params),(yyvsp[(6) - (10)].classinfo),c);
    
    (yyval.value).c = abc_getlocal(0, index);
    (yyval.value).t = TYPE_FUNCTION(f);

    PASS12 list_deep_free((yyvsp[(4) - (10)].params).list);
}
    }
    break;


  
    case 225:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2915 "parser.y"
    {
    PASS1 NEW(unresolvedinfo_t,c);
          memset(c, 0, sizeof(*c));
          c->kind = INFOTYPE_UNRESOLVED;
          c->name = (yyvsp[(1) - (1)].id);
          c->package = get_package_from_name((yyvsp[(1) - (1)].id));
          if(!c->package) {
              c->nsset = get_current_imports();
              /* make the compiler look for this class in the current directory,
                 just in case: */
              as3_schedule_class_noerror(state->package, (yyvsp[(1) - (1)].id));
          }
          (yyval.classinfo) = (classinfo_t*)c;
    PASS2
    slotinfo_t*s = find_class((yyvsp[(1) - (1)].id));
    if(!s) syntaxerror("Could not find class/method %s (current package: %s)\n", (yyvsp[(1) - (1)].id), state->package);
    (yyval.classinfo) = (classinfo_t*)s;
}
    }
    break;


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2934 "parser.y"
    {
    PASS1 NEW(unresolvedinfo_t,c);
          memset(c, 0, sizeof(*c));
          c->kind = INFOTYPE_UNRESOLVED;
          c->package = (yyvsp[(1) - (3)].id);
          c->name = (yyvsp[(3) - (3)].id);
          (yyval.classinfo) = (classinfo_t*)c;
    PASS2
    slotinfo_t*s = registry_find((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id));
    if(!s) syntaxerror("Couldn't find class/method %s.%s\n", (yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id));
    free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;
    (yyval.classinfo) = (classinfo_t*)s;
}
    }
    break;


  
    case 229:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2951 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 230:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2952 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 231:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2954 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2955 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_ANY;}
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2956 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_VOID;}
    }
    break;


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2965 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2966 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2970 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2971 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2973 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2977 "parser.y"
    {(yyval.value_list).number=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 242:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2981 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 243:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2982 "parser.y"
    {
                                                  (yyval.value_list).number= (yyvsp[(1) - (2)].value_list).number+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2988 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(2) - (4)].node));
    (yyval.value).c = v.c;
    if((yyval.value).c->opcode == OPCODE_COERCE_A) (yyval.value).c = code_cutlast((yyval.value).c);
    
    code_t*paramcode = (yyvsp[(4) - (4)].value_list).cc;
    if((yyval.value).c->opcode == OPCODE_GETPROPERTY) {
        multiname_t*name = (yyval.value).c->data[0];(yyval.value).c->data[0]=0;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_constructprop2((yyval.value).c, name, (yyvsp[(4) - (4)].value_list).number);
        multiname_destroy(name);
    } else if(TYPE_IS_CLASS(v.t) && v.t->data) {
        code_free((yyval.value).c);
        classinfo_t*c = v.t->data;
        MULTINAME(m, c);
        (yyval.value).c = abc_findpropstrict2(0, &m);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_constructprop2((yyval.value).c, &m, (yyvsp[(4) - (4)].value_list).number);
    /*} else if($$.c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)$$.c->data[0];
        trait_t*t = traits_find_slotid(state->cls->abc->traits,slot);//FIXME
        multiname_t*name = t->name;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_constructprop2($$.c, name, $4.number);*/
    } else {
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_construct((yyval.value).c, (yyvsp[(4) - (4)].value_list).number);
    }
   
    (yyval.value).t = TYPE_ANY;
    if(TYPE_IS_CLASS(v.t) && v.t->data) {
        (yyval.value).t = v.t->data;
    } else {
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_ANY;
    }
}
    }
    break;


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3032 "parser.y"
    {
   
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    (yyval.value).c = v.c;
    if((yyval.value).c->opcode == OPCODE_COERCE_A) {
        (yyval.value).c = code_cutlast((yyval.value).c);
    }
    code_t*paramcode = (yyvsp[(3) - (4)].value_list).cc;

    (yyval.value).t = TYPE_ANY;
    if((yyval.value).c->opcode == OPCODE_GETPROPERTY) {
        multiname_t*name = (yyval.value).c->data[0];(yyval.value).c->data[0]=0;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_callproperty2((yyval.value).c, name, (yyvsp[(3) - (4)].value_list).number);
        multiname_destroy(name);
/*    } else if($$.c->opcode == OPCODE_GETSLOT && $$.c->prev->opcode != OPCODE_GETSCOPEOBJECT) {
        int slot = (int)(ptroff_t)$$.c->data[0];
        trait_t*t = traits_find_slotid(state->cls->abc->traits,slot);
        if(t->kind!=TRAIT_METHOD) {
            //ok: flash allows to assign closures to members.
        }
        multiname_t*name = t->name;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        //$$.c = abc_callmethod($$.c, t->method, len); //#1051 illegal early access binding
        $$.c = abc_callproperty2($$.c, name, $3.number);*/
    } else if((yyval.value).c->opcode == OPCODE_GETSUPER) {
        multiname_t*name = (yyval.value).c->data[0];(yyval.value).c->data[0]=0;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_callsuper2((yyval.value).c, name, (yyvsp[(3) - (4)].value_list).number);
        multiname_destroy(name);
    } else {
        (yyval.value).c = abc_getglobalscope((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_call((yyval.value).c, (yyvsp[(3) - (4)].value_list).number);
    }
   
    if(TYPE_IS_FUNCTION(v.t) && v.t->data) {
        (yyval.value).t = ((methodinfo_t*)(v.t->data))->return_type;
    } else if(TYPE_IS_CLASS(v.t) && v.t->data) {
        // calling a class is like a typecast
        (yyval.value).t = (classinfo_t*)v.t->data;
    } else {
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_ANY;
    }
}
    }
    break;


  
    case 247:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3082 "parser.y"
    {
    if(!state->cls) syntaxerror("super() not allowed outside of a class");
    if(!state->method) syntaxerror("super() not allowed outside of a function");
    if(!state->method->is_constructor) syntaxerror("super() not allowed outside of a constructor");

    (yyval.value).c = code_new();
    (yyval.value).c = abc_getlocal_0((yyval.value).c);

    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(3) - (4)].value_list).cc);
    /*
    this is dependent on the control path, check this somewhere else
    if(state->method->has_super)
        syntaxerror("constructor may call super() only once");
    */
    state->method->has_super = 1;

    (yyval.value).c = abc_constructsuper((yyval.value).c, (yyvsp[(3) - (4)].value_list).number);
    (yyval.value).c = abc_pushundefined((yyval.value).c);
    (yyval.value).t = TYPE_ANY;
}
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3103 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(2) - (2)].node));
    (yyval.value).c = v.c;
    if((yyval.value).c->opcode == OPCODE_COERCE_A) {
        (yyval.value).c = code_cutlast((yyval.value).c);
    }
    multiname_t*name = 0;
    if((yyval.value).c->opcode == OPCODE_GETPROPERTY) {
        (yyval.value).c->opcode = OPCODE_DELETEPROPERTY;
    } else if((yyval.value).c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)(yyval.value).c->data[0];
        multiname_t*name = traits_find_slotid(state->cls->abc->traits,slot)->name;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = abc_deleteproperty2((yyval.value).c, name);
    } else {
        (yyval.value).c = abc_getlocal_0((yyval.value).c);
        MULTINAME_LATE(m, v.t?v.t->access:ACCESS_PACKAGE, "");
        (yyval.value).c = abc_deleteproperty2((yyval.value).c, &m);
    }
    (yyval.value).t = TYPE_BOOLEAN;
}
    }
    break;


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3125 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3128 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3135 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3138 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3141 "parser.y"
    {
    (yyval.node) = mkmultinode(&node_comma, (yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3144 "parser.y"
    {
    (yyval.node) = multinode_extend((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
}
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3147 "parser.y"
    { 
    (yyval.code) = node_exec((yyvsp[(1) - (1)].node)); 
}
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3150 "parser.y"
    { 
    (yyval.code) = (yyvsp[(1) - (3)].code);
    (yyval.code) = code_append((yyval.code), node_exec((yyvsp[(3) - (3)].node))); 
}
    }
    break;


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3155 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3156 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3158 "parser.y"
    {(yyval.code)=abc_pushstring(0,(yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3159 "parser.y"
    {(yyval.code)=abc_pushstring2(0,&(yyvsp[(1) - (1)].str));}
    }
    break;


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3160 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3161 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3162 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3164 "parser.y"
    {
    (yyval.value_list).cc = 0;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(1) - (3)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (3)].value).c);
    (yyval.value_list).number = 2;
}
    }
    break;


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3170 "parser.y"
    {
    (yyval.value_list).cc = (yyvsp[(1) - (5)].value_list).cc;
    (yyval.value_list).number = (yyvsp[(1) - (5)].value_list).number+2;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (5)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(5) - (5)].value).c);
}
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3179 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3180 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3181 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 269:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3182 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3183 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3184 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    }
    break;


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3186 "parser.y"
    { 
    (yyval.node) = mkconstnode((yyvsp[(1) - (1)].constant));
}
    }
    break;


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3190 "parser.y"
    {
    typedcode_t v;
    v.c = 0;
    multiname_t m = {QNAME, &stdns, 0, "XML"};
    v.c = abc_getlex2(v.c, &m);
    v.c = abc_pushstring(v.c, (yyvsp[(1) - (1)].id));
    v.c = abc_construct(v.c, 1);
    v.t = TYPE_XML;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3202 "parser.y"
    {
    typedcode_t v;
    v.c = 0;
    multiname_t m = {QNAME, &stdns, 0, "RegExp"};
    if(!(yyvsp[(1) - (1)].regexp).options) {
        v.c = abc_getlex2(v.c, &m);
        v.c = abc_pushstring(v.c, (yyvsp[(1) - (1)].regexp).pattern);
        v.c = abc_construct(v.c, 1);
    } else {
        v.c = abc_getlex2(v.c, &m);
        v.c = abc_pushstring(v.c, (yyvsp[(1) - (1)].regexp).pattern);
        v.c = abc_pushstring(v.c, (yyvsp[(1) - (1)].regexp).options);
        v.c = abc_construct(v.c, 2);
    }
    v.t = TYPE_REGEXP;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3220 "parser.y"
    {
    PASS1
    state->method->need_arguments = 1;
    PASS2
    typedcode_t v;
    v.c = abc_getlocal(0, state->method->need_arguments);
    v.t = TYPE_ARRAY;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3231 "parser.y"
    {
    typedcode_t v;
    v.c = code_new();
    v.c = code_append(v.c, (yyvsp[(2) - (3)].value_list).cc);
    v.c = abc_newarray(v.c, (yyvsp[(2) - (3)].value_list).number);
    v.t = registry_getarrayclass();
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3241 "parser.y"
    {
    typedcode_t v;
    v.c = code_new();
    v.c = code_append(v.c, (yyvsp[(2) - (3)].value_list).cc);
    v.c = abc_newobject(v.c, (yyvsp[(2) - (3)].value_list).number/2);
    v.t = registry_getobjectclass();
    (yyval.node) =  mkcodenode(v);
}
    }
    break;


  
    case 278:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3250 "parser.y"
    {(yyval.node) = mknode2(&node_lt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 279:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3251 "parser.y"
    {(yyval.node) = mknode2(&node_gt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 280:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3252 "parser.y"
    {(yyval.node) = mknode2(&node_le,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 281:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3253 "parser.y"
    {(yyval.node) = mknode2(&node_ge,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3254 "parser.y"
    {(yyval.node) = mknode2(&node_eqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3255 "parser.y"
    {(yyval.node) = mknode2(&node_eqeqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3256 "parser.y"
    {(yyval.node) = mknode2(&node_noteqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3257 "parser.y"
    {(yyval.node) = mknode2(&node_noteq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 286:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3258 "parser.y"
    {(yyval.node) = mknode2(&node_oror,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 287:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3259 "parser.y"
    {(yyval.node) = mknode2(&node_andand,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3260 "parser.y"
    {(yyval.node) = mknode1(&node_not, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 289:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3261 "parser.y"
    {(yyval.node) = mknode1(&node_bitnot, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 290:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3262 "parser.y"
    {(yyval.node) = mknode2(&node_bitand, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 291:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3263 "parser.y"
    {(yyval.node) = mknode2(&node_bitxor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 292:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3264 "parser.y"
    {(yyval.node) = mknode2(&node_bitor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 293:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3265 "parser.y"
    {(yyval.node) = mknode2(&node_shr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 294:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3266 "parser.y"
    {(yyval.node) = mknode2(&node_ushr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 295:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3267 "parser.y"
    {(yyval.node) = mknode2(&node_shl, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 296:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3268 "parser.y"
    {(yyval.node) = mknode2(&node_div, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 297:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3269 "parser.y"
    {(yyval.node) = mknode2(&node_mod, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 298:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3270 "parser.y"
    {(yyval.node) = mknode2(&node_plus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 299:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3271 "parser.y"
    {(yyval.node) = mknode2(&node_minus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 300:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3272 "parser.y"
    {(yyval.node) = mknode2(&node_multiply, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 301:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3273 "parser.y"
    {(yyval.node) = mknode2(&node_in, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 302:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3274 "parser.y"
    {(yyval.node) = mknode2(&node_as, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 303:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3275 "parser.y"
    {(yyval.node) = mknode2(&node_instanceof, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 304:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3276 "parser.y"
    {(yyval.node) = mknode2(&node_is, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 305:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3277 "parser.y"
    {(yyval.node) = mknode1(&node_typeof, (yyvsp[(3) - (4)].node));}
    }
    break;


  
    case 306:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3278 "parser.y"
    {(yyval.node) = mknode1(&node_void, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 307:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3279 "parser.y"
    { (yyval.node) = mkconstnode(constant_new_undefined());}
    }
    break;


  
    case 308:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3280 "parser.y"
    { (yyval.node)=(yyvsp[(2) - (3)].node);}
    }
    break;


  
    case 309:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3281 "parser.y"
    {(yyval.node) = mknode1(&node_neg, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 310:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3282 "parser.y"
    {(yyval.node) = mknode2(&node_arraylookup, (yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node));}
    }
    break;


  
    case 311:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3283 "parser.y"
    {(yyval.node) = mknode2(&node_muleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 312:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3284 "parser.y"
    {(yyval.node) = mknode2(&node_modeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 313:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3285 "parser.y"
    {(yyval.node) = mknode2(&node_shleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 314:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3286 "parser.y"
    {(yyval.node) = mknode2(&node_shreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 315:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3287 "parser.y"
    {(yyval.node) = mknode2(&node_ushreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 316:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3288 "parser.y"
    { (yyval.node) = mknode2(&node_diveq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 317:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3289 "parser.y"
    { (yyval.node) = mknode2(&node_bitoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 318:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3290 "parser.y"
    { (yyval.node) = mknode2(&node_bitxoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 319:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3291 "parser.y"
    { (yyval.node) = mknode2(&node_bitandeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 320:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3292 "parser.y"
    { (yyval.node) = mknode2(&node_pluseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 321:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3293 "parser.y"
    { (yyval.node) = mknode2(&node_minuseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 322:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3294 "parser.y"
    { (yyval.node) = mknode2(&node_assign, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 323:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3295 "parser.y"
    { (yyval.node) = mknode3(&node_tenary, (yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node));}
    }
    break;


  
    case 324:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3297 "parser.y"
    { (yyval.node) = mknode1(&node_rplusplus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 325:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3298 "parser.y"
    { (yyval.node) = mknode1(&node_rminusminus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 326:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3299 "parser.y"
    {(yyval.node) = mknode1(&node_lplusplus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 327:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3300 "parser.y"
    {(yyval.node) = mknode1(&node_lminusminus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 328:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3303 "parser.y"
    { if(!state->cls->info)
                  syntaxerror("super keyword not allowed outside a class");
              classinfo_t*t = state->cls->info->superclass;
              if(!t) t = TYPE_OBJECT;
              memberinfo_t*f = findmember_nsset(t, (yyvsp[(3) - (3)].id), 1);
              MEMBER_MULTINAME(m, f, (yyvsp[(3) - (3)].id));
              typedcode_t v;
              v.c = 0;
              v.c = abc_getlocal_0(v.c);
              v.c = abc_getsuper2(v.c, &m);
              v.t = slotinfo_gettype((slotinfo_t*)f);
              (yyval.node) = mkcodenode(v);
           }
    }
    break;


  
    case 329:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3317 "parser.y"
    {
    typedcode_t v;
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(2) - (2)].id)};
    v.c = abc_getlex2(0, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 330:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3325 "parser.y"
    {PASS12 new_state();state->xmlfilter=1;}
    }
    break;


  
    case 331:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3325 "parser.y"
    {
    PASS1 old_state();
    PASS2
    typedcode_t v = node_read((yyvsp[(1) - (6)].node));
    typedcode_t w = node_read((yyvsp[(5) - (6)].node));
    code_t*c = 0;
    int index = alloc_local();
    int result = alloc_local();
    int tmp = alloc_local();
    int xml = alloc_local();
    
    c = code_append(c, v.c);
    c = abc_checkfilter(c);
    c = abc_coerce_a(c); //hasnext2 converts to *
    c = abc_setlocal(c, xml);
    multiname_t m = {QNAME, &stdns, 0, "XMLList"};
    c = abc_getlex2(c, &m);
    c = abc_construct(c, 0);
    c = abc_setlocal(c, result);
    c = abc_pushbyte(c, 0);
    c = abc_setlocal(c, index);
    code_t*jmp = c = abc_jump(c, 0);
    code_t*loop = c = abc_label(c);
    c = abc_getlocal(c, xml);
    c = abc_getlocal(c, index);
    c = abc_nextvalue(c);
    c = abc_dup(c);
    c = abc_setlocal(c, tmp);
    c = abc_pushwith(c);
    c = code_append(c, w.c);
    c = abc_popscope(c);
    code_t*b = c = abc_iffalse(c, 0);
    c = abc_getlocal(c, result);
    c = abc_getlocal(c, index);
    c = abc_getlocal(c, tmp);
    multiname_t m2 = {MULTINAMEL, 0, &nopackage_namespace_set, 0};
    c = abc_setproperty2(c, &m2);
    c = b->branch = jmp->branch = abc_nop(c);
    c = abc_kill(c, tmp);
    c = abc_hasnext2(c, xml, index);
    c = abc_iftrue(c, loop);
    c = abc_getlocal(c, result);
    c = abc_kill(c, xml);
    c = abc_kill(c, result);
    c = abc_kill(c, index);
    
    c = var_block(c);
    old_state();
    typedcode_t r;
    r.c = c;
    r.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(r);
}
    }
    break;


  
    case 332:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3379 "parser.y"
    {(yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 333:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3380 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 334:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3381 "parser.y"
    {(yyval.id)=(char*)(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 336:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3383 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 337:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3391 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (5)].node));
    typedcode_t w = node_read(resolve_identifier((yyvsp[(3) - (5)].id)));
    v.c = code_append(v.c, w.c);
    if(!TYPE_IS_NAMESPACE(w.t)) {
        as3_softwarning("%s might not be a namespace", (yyvsp[(3) - (5)].id));
    }
    v.c = converttype(v.c, w.t, TYPE_NAMESPACE);
    multiname_t m = {RTQNAME, 0, 0, (yyvsp[(5) - (5)].id)};
    v.c = abc_getproperty2(v.c, &m);
    if(TYPE_IS_XML(v.t)) {
        v.t = TYPE_XMLLIST;
    } else {
        v.c = abc_coerce_a(v.c);
        v.t = TYPE_ANY;
    }
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 338:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3409 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (3)].node));
    multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 339:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3416 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (5)].node));
    typedcode_t w = node_read((yyvsp[(4) - (5)].node));
    multiname_t m = {MULTINAMEL, 0, &nopackage_namespace_set, 0};
    v.c = code_append(v.c, w.c);
    v.c = converttype(w.c, w.t, TYPE_STRING);
    v.c = abc_getproperty2(v.c, &m);
    v.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 340:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3427 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getproperty2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 341:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3434 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 342:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3441 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (6)].node));
    typedcode_t w = node_read((yyvsp[(5) - (6)].node));
    multiname_t m = {MULTINAMELA, 0, &nopackage_namespace_set, 0};
    v.c = code_append(v.c, w.c);
    v.c = converttype(w.c, w.t, TYPE_STRING);
    v.c = abc_getproperty2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 343:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3451 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (6)].node));
    typedcode_t w = node_read((yyvsp[(5) - (6)].node));
    multiname_t m = {MULTINAMELA, 0, &nopackage_namespace_set, 0};
    v.c = code_append(v.c, w.c);
    v.c = converttype(w.c, w.t, TYPE_STRING);
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 344:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3462 "parser.y"
    {
    typedcode_t v1 = node_read((yyvsp[(1) - (3)].node));
    (yyval.value).c = v1.c;
    classinfo_t*t = v1.t;
    char is_static = 0;
    if(TYPE_IS_CLASS(t) && t->data) {
        t = t->data;
        is_static = 1;
    }
    if(TYPE_IS_XML(t)) {
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
        (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_XMLLIST;
    } else if(t) {
        if(t->subtype==INFOTYPE_UNRESOLVED) {
            syntaxerror("syntaxerror: trying to resolve property '%s' on incomplete object '%s'", (yyvsp[(3) - (3)].id), t->name);
        }
        memberinfo_t*f = findmember_nsset(t, (yyvsp[(3) - (3)].id), 1);
        char noslot = 0;
        if(f && !is_static != !(f->flags&FLAG_STATIC))
           noslot=1;
        if(f && f->slot && !noslot) {
            (yyval.value).c = abc_getslot((yyval.value).c, f->slot);
        } else {
            if(!f) {
                as3_softwarning("Access of undefined property '%s' in %s", (yyvsp[(3) - (3)].id), t->name);
            }
            MEMBER_MULTINAME(m, f, (yyvsp[(3) - (3)].id));
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
        }
        /* determine type */
        (yyval.value).t = slotinfo_gettype((slotinfo_t*)f);
        if(!(yyval.value).t)
           (yyval.value).c = abc_coerce_a((yyval.value).c);
        
    } else if(v1.c && v1.c->opcode == OPCODE___PUSHPACKAGE__) {
        string_t*package = v1.c->data[0];
        char*package2 = concat3(package->str, ".", (yyvsp[(3) - (3)].id));

        slotinfo_t*a = registry_find(package->str, (yyvsp[(3) - (3)].id));
        if(a) {
            (yyval.value) = push_class(a);
        } else if(dict_contains(state->import_toplevel_packages, package2) ||
                  registry_ispackage(package2)) {
            (yyval.value).c = v1.c;
            (yyval.value).c->data[0] = string_new4(package2);
            (yyval.value).t = 0;
        } else {
            syntaxerror("couldn't resolve %s", package2);
        }
    } else {
        /* when resolving a property on an unknown type, we do know the
           name of the property (and don't seem to need the package), but
           we need to make avm2 try out all access modes */
        as3_softwarning("Resolving %s on unknown type", (yyvsp[(3) - (3)].id));
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
        (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_ANY;
    }
}
    }
    break;


  
    case 345:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3640 "parser.y"
    {
    PASS1
    /* Queue unresolved identifiers for checking against the parent
       function's variables.
       We consider everything which is not a local variable "unresolved".
       This encompasses class names, members of the surrounding class
       etc. which is *correct* because local variables of the parent function
       would shadow those.
       */

    if(!find_variable(state, (yyvsp[(1) - (1)].id))) {
        if(state->method->inner) {
            unknown_variable((yyvsp[(1) - (1)].id));
        }
        /* let the compiler know that it might want to check the current directory/package
           for this identifier- maybe there's a file $1.as defining $1. */
        as3_schedule_class_noerror(state->package, (yyvsp[(1) - (1)].id));
    }
   
    (yyval.node) = 0;
    PASS2

    (yyval.node) = resolve_identifier((yyvsp[(1) - (1)].id));
}
    }
    break;


  
    case 346:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3676 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (2)].id);
    n->url = (yyvsp[(2) - (2)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 347:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3683 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 348:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3690 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].str).str;
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 349:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3697 "parser.y"
    {
    PASS12
    trie_put(active_namespaces, (yyvsp[(2) - (2)].namespace_decl)->name, (void*)(yyvsp[(2) - (2)].namespace_decl)->url);

    namespace_t access = modifiers2access(&(yyvsp[(1) - (2)].flags));
    varinfo_t* var = varinfo_register_global(access.access, state->package, (yyvsp[(2) - (2)].namespace_decl)->name);
    var->type = TYPE_NAMESPACE;
    namespace_t ns;
    ns.access = ACCESS_NAMESPACE;
    ns.name = (yyvsp[(2) - (2)].namespace_decl)->url;
    var->value = constant_new_namespace(&ns);
      
    if(as3_pass==2) {
        MULTINAME(m, TYPE_NAMESPACE);
        trait_t*t = add_abc_slot(&(yyvsp[(1) - (2)].flags), (yyvsp[(2) - (2)].namespace_decl)->name, 0, 0);
        t->value = var->value;
        t->type_name = multiname_clone(&m);
    }

    (yyval.code)=0;
}
    }
    break;


  
    case 350:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3720 "parser.y"
    {
    as3_warning("default xml namespaces not supported yet");
}
    }
    break;


  
    case 351:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3724 "parser.y"
    {
    PASS12
    const char*url = (yyvsp[(3) - (3)].classinfo)->name;

    varinfo_t*s = (varinfo_t*)(yyvsp[(3) - (3)].classinfo);
    if(s->kind == INFOTYPE_UNRESOLVED) {
        s = (varinfo_t*)registry_resolve((slotinfo_t*)s);
        if(!s)
            syntaxerror("Couldn't resolve namespace %s", (yyvsp[(3) - (3)].classinfo)->name);
    }

    if(!s || s->kind != INFOTYPE_VAR)
        syntaxerror("%s.%s is not a public namespace (%d)", (yyvsp[(3) - (3)].classinfo)->package, (yyvsp[(3) - (3)].classinfo)->name, s?s->kind:-1);
    if(!s->value || !NS_TYPE(s->value->type))
        syntaxerror("%s.%s is not a namespace", (yyvsp[(3) - (3)].classinfo)->package, (yyvsp[(3) - (3)].classinfo)->name);
    url = s->value->ns->name;

    trie_put(active_namespaces, (yyvsp[(3) - (3)].classinfo)->name, (void*)url);
    add_active_url(url);
    (yyval.code)=0;
}
    }
    break;




/* Line 1464 of skeleton.m4  */
#line 8735 "parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



