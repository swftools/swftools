
/* A Bison parser, made by GNU Bison 2.4.1.  */

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
#define YYBISON_VERSION "2.4.1"

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
     T_STRING = 259,
     T_REGEXP = 260,
     T_EMPTY = 261,
     T_INT = 262,
     T_UINT = 263,
     T_FLOAT = 264,
     T_FOR = 265,
     T_WHILE = 266,
     T_DO = 267,
     T_SWITCH = 268,
     KW_IMPLEMENTS = 269,
     KW_NAMESPACE = 270,
     KW_PACKAGE = 271,
     KW_PROTECTED = 272,
     KW_ARGUMENTS = 273,
     KW_PUBLIC = 274,
     KW_PRIVATE = 275,
     KW_USE = 276,
     KW_INTERNAL = 277,
     KW_NEW = 278,
     KW_NATIVE = 279,
     KW_FUNCTION = 280,
     KW_FINALLY = 281,
     KW_UNDEFINED = 282,
     KW_NAN = 283,
     KW_CONTINUE = 284,
     KW_CLASS = 285,
     KW_CONST = 286,
     KW_CATCH = 287,
     KW_CASE = 288,
     KW_SET = 289,
     KW_VOID = 290,
     KW_THROW = 291,
     KW_STATIC = 292,
     KW_WITH = 293,
     KW_INSTANCEOF = 294,
     KW_IMPORT = 295,
     KW_RETURN = 296,
     KW_TYPEOF = 297,
     KW_INTERFACE = 298,
     KW_NULL = 299,
     KW_VAR = 300,
     KW_DYNAMIC = 301,
     KW_OVERRIDE = 302,
     KW_FINAL = 303,
     KW_EACH = 304,
     KW_GET = 305,
     KW_TRY = 306,
     KW_SUPER = 307,
     KW_EXTENDS = 308,
     KW_FALSE = 309,
     KW_TRUE = 310,
     KW_BOOLEAN = 311,
     KW_UINT = 312,
     KW_INT = 313,
     KW_NUMBER = 314,
     KW_STRING = 315,
     KW_DEFAULT = 316,
     KW_DEFAULT_XML = 317,
     KW_DELETE = 318,
     KW_IF = 319,
     KW_ELSE = 320,
     KW_BREAK = 321,
     KW_IS = 322,
     KW_IN = 323,
     KW_AS = 324,
     T_DICTSTART = 325,
     T_EQEQ = 326,
     T_EQEQEQ = 327,
     T_NE = 328,
     T_NEE = 329,
     T_LE = 330,
     T_GE = 331,
     T_ORBY = 332,
     T_DIVBY = 333,
     T_MODBY = 334,
     T_MULBY = 335,
     T_ANDBY = 336,
     T_PLUSBY = 337,
     T_MINUSBY = 338,
     T_XORBY = 339,
     T_SHRBY = 340,
     T_SHLBY = 341,
     T_USHRBY = 342,
     T_OROR = 343,
     T_ANDAND = 344,
     T_COLONCOLON = 345,
     T_MINUSMINUS = 346,
     T_PLUSPLUS = 347,
     T_DOTDOT = 348,
     T_DOTDOTDOT = 349,
     T_SHL = 350,
     T_USHR = 351,
     T_SHR = 352,
     prec_none = 353,
     below_semicolon = 354,
     below_assignment = 355,
     below_lt = 356,
     below_minus = 357,
     minusminus_prefix = 358,
     plusplus_prefix = 359,
     below_curly = 360,
     new2 = 361,
     above_identifier = 362,
     below_else = 363,
     above_function = 364
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
#line 284 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 274 "parser.y"


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

methodstate_t*methodstate_new()
{
    NEW(methodstate_t,m);
    m->allvars = dict_new();
    return m;
}
void methodstate_destroy(methodstate_t*m) 
{
    dict_destroy(m->unresolved_variables); m->unresolved_variables = 0;
    list_free(m->innerfunctions);m->innerfunctions=0;

    if(m->allvars) {
        DICT_ITERATE_DATA(m->allvars, void*, data) {free(data);}
        m->allvars = 0;
    }
}

typedef struct _state {
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

    state->namespaces = dict_new();
   
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
    
    list_free(state->active_namespace_urls)
    state->active_namespace_urls = 0;
    
    free(state);
}

static void old_state()
{
    if(!state || !state->old)
        syntaxerror("invalid nesting");
    state_t*leaving = state;
    
    state = state->old;

    if(as3_pass>1 && leaving->method && leaving->method != state->method && !leaving->method->inner) {
        methodstate_destroy(leaving->method);leaving->method=0;
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
        state->method->late_binding = 1; // init scripts use getglobalscope, so we need a getlocal0/pushscope
	state->method->allvars = dict_new();
    } else {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        state->method->variable_count = 0;
        if(!state->method)
            syntaxerror("internal error: skewed tokencount");
        function_initvars(state->method, 0, 0, 0, 1);
        global->init = 0;
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
        //if(global->init->method->body->code || global->init->traits) {
        if(global->init) {
            code_t*c = wrap_function(header, 0, global->init->method->body->code);
            global->init->method->body->code = abc_returnvoid(c);
            free(state->method);state->method=0;
        }
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
    char kill;
    char is_parameter;
    methodstate_t*is_inner_method;
} variable_t;

static variable_t* find_variable(state_t*s, const char*name)
{
    if(s->method->no_variable_scoping) {
        return dict_lookup(s->method->allvars, name);
    } else {
        state_t*top = s;
        while(s) {
            variable_t*v = 0;
            v = dict_lookup(s->vars, name);
            if(v) return v;
            if(s->new_vars) break;
            s = s->old;
        }
        return 0;
    }
}
static variable_t* find_slot(methodstate_t*m, const char*name)
{
    if(m && m->slots)
        return dict_lookup(m->slots, name);
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
    parserassert(!type || type->kind!=INFOTYPE_UNRESOLVED);
    if(TYPE_IS_INT(type)) {
       c = abc_pushbyte(c, 0);
    } else if(TYPE_IS_UINT(type)) {
       c = abc_pushuint(c, 0);
    } else if(TYPE_IS_FLOAT(type)) {
       c = abc_pushnan(c);
    } else if(TYPE_IS_BOOLEAN(type)) {
       c = abc_pushfalse(c);
    } else if(TYPE_IS_STRING(type)) {
       c = abc_pushnull(c);
       c = abc_coerce_s(c);
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

static variable_t* new_variable2(methodstate_t*method, const char*name, classinfo_t*type, char init, char maybeslot)
{
    if(maybeslot) {
        variable_t*v = find_slot(method, name);
        if(v) {
            alloc_local(); 
            return v;
        }
    }

    NEW(variable_t, v);
    v->index = alloc_local();
    v->type = type;
    v->init = v->kill = init;
 
    if(name) {
        if(!method->no_variable_scoping) 
        {
            if(dict_contains(state->vars, name)) {
                syntaxerror("variable %s already defined", name);
            }
            dict_put(state->vars, name, v);
        }
        if(method->no_variable_scoping && 
           as3_pass==2 && 
           dict_contains(state->method->allvars, name)) 
        {
            variable_t*v = dict_lookup(state->method->allvars, name);
            if(v->type != type && (!v->type || v->type->kind!=INFOTYPE_UNRESOLVED)) {
                syntaxerror("variable %s already defined.", name);
	    }
            return v;
        }
        dict_put(state->method->allvars, name, v);
    }

    return v;
}
static int new_variable(methodstate_t*method, const char*name, classinfo_t*type, char init, char maybeslot)
{
    return new_variable2(method, name, type, init, maybeslot)->index;
}

#define TEMPVARNAME "__as3_temp__"
int gettempvar()
{
    variable_t*v = find_variable(state, TEMPVARNAME);
    int i;
    if(v) 
        i = v->index;
    else
        i = new_variable(state->method, TEMPVARNAME, 0, 0, 0);
    parserassert(i);
    return i;
}

static code_t* var_block(code_t*body, dict_t*vars) 
{
    code_t*c = 0;
    code_t*k = 0;
    int t;
    DICT_ITERATE_DATA(vars, variable_t*, v) {
        if(v->type && v->init) {
            c = defaultvalue(c, v->type);
            c = abc_setlocal(c, v->index);
        }
        if(v->type && v->kill) {
            k = abc_kill(k, v->index); 
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
    if(m->uses_slots || m->innerfunctions || (m->late_binding && !m->inner)) {
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
    c = code_append(c, var_block(body, state->method->no_variable_scoping?state->method->allvars:state->vars));
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

static slotinfo_t* find_class(const char*name);

const char* lookup_namespace(const char*name)
{
    state_t*s = state;
    while(s) {
	const char*url = dict_lookup(s->namespaces, name);
	if(url) 
	    return url;
	s = s->old;
    }
    varinfo_t*a;
    registry_find(state->package, name);
    if(( a = (varinfo_t*)find_class(name) )) {
	if(a->kind == INFOTYPE_VAR) {
	    if(!a->value || !NS_TYPE(a->value->type)) 
		syntaxerror("%s.%s is not a namespace", a->package, a->name);
	    return a->value->ns->name;
	}
    }
    return 0;
}

static namespace_t modifiers2access(modifiers_t*mod)
{
    namespace_t ns;
    ns.access = 0;
    ns.name = "";
    if(mod->flags&FLAG_NAMESPACE)  {
        if(mod->flags&(FLAG_PRIVATE|FLAG_PROTECTED|FLAG_PACKAGEINTERNAL)) 
            syntaxerror("invalid combination of access levels and namespaces");
        ns.access = ACCESS_NAMESPACE;
	const char*url = lookup_namespace(mod->ns);
	if(!url) {
	    if(as3_pass>1) {
		syntaxerror("unknown namespace: %s (pass %d)", mod->ns, as3_pass);
	    } else {
		url = mod->ns;
	    }
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

static memberinfo_t* findmember_nsset(classinfo_t*cls, const char*name, char recurse, char is_static)
{
    return registry_findmember_nsset(cls, state->active_namespace_urls, name, recurse, is_static);
}

static void innerfunctions2vars(methodstate_t*m)
{
    methodstate_list_t*l = m->innerfunctions;
    while(l) {
        methodstate_t*m = l->methodstate;
        
        variable_t* v = new_variable2(state->method, m->info->name, TYPE_FUNCTION(m->info), 0, 0);
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
            index = new_variable(m, "this", 0, 0, 0);
        else if(!m->is_global)
            index = new_variable(m, (flags&FLAG_STATIC)?"class":"this", state->cls?state->cls->info:0, 0, 0);
        else
            index = new_variable(m, "globalscope", 0, 0, 0);
        if(index) {
            DICT_ITERATE_ITEMS(state->vars, char*, name, variable_t*, v) {
                printf("%s %d\n", name, v->index);
            }
        }
        parserassert(!index);
    }

    if(has_params) {
        param_list_t*p=0;
        for(p=params->list;p;p=p->next) {
            variable_t*v = new_variable2(m, p->param->name, p->param->type, 0, 1);
            v->is_parameter = 1;
        }
        if(as3_pass==2 && m->need_arguments) {
            /* arguments can never be used by an innerfunction (the inner functions
               have their own arguments var), so it's ok to  not initialize this until
               pass 2. (We don't know whether we need it before, anyway) */
            variable_t*v = new_variable2(m, "arguments", TYPE_ARRAY, 0, 0);
            m->need_arguments = v->index;
        }
    }
    
    innerfunctions2vars(m);
    
    if(as3_pass==2) {
        m->scope_code = add_scope_code(m->scope_code, m, 0);
        if(m->slots) {
            /* exchange unresolved identifiers with the actual objects */
            DICT_ITERATE_ITEMS(m->slots, char*, name, variable_t*, v1) {
                if(v1->type && v1->type->kind == INFOTYPE_UNRESOLVED) {
                    classinfo_t*type = (classinfo_t*)registry_resolve((slotinfo_t*)v1->type);
                    if(!type || type->kind != INFOTYPE_CLASS) {
                        syntaxerror("Couldn't find class %s::%s (%s)", v1->type->package, v1->type->name, name);
                    }
                    v1->type = type;
                }
            }
	}
	if(m->allvars) {
            DICT_ITERATE_ITEMS(m->allvars, char*, name2, variable_t*, v2) {
                if(v2->type && v2->type->kind == INFOTYPE_UNRESOLVED) {
                    classinfo_t*type = (classinfo_t*)registry_resolve((slotinfo_t*)v2->type);
                    if(!type || type->kind != INFOTYPE_CLASS) {
                        syntaxerror("Couldn't find class %s::%s (%s)", v2->type->package, v2->type->name, name2);
                    }
                    v2->type = type;
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
        state->cls->init = methodstate_new();
        state->cls->static_init = methodstate_new();
        state->cls->static_init->is_static=FLAG_STATIC;
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
    
        parserassert(state->cls && state->cls->info);
       
        state->method = state->cls->static_init;

        function_initvars(state->cls->init, 0, 0, 0, 1);
        state->cls->static_init->variable_count=1;
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
    if(m->parent == state->cls->info && !((flags^m->flags)&FLAG_STATIC))
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
        memberinfo_t* m = registry_findmember(state->cls->info, ns.name, name, 0, mod->flags&FLAG_STATIC);
        if(m) {
            syntaxerror("class already contains a %s '%s'", infotypename((slotinfo_t*)m), m->name);
        }
        minfo = methodinfo_register_onclass(state->cls->info, ns.access, ns.name, name, mod->flags&FLAG_STATIC);
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
        minfo = (methodinfo_t*)registry_findmember(state->cls->info, ns.name, name, 1, mod->flags&FLAG_STATIC);
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
            minfo = methodinfo_register_onclass(state->cls->info, ns.access, ns.name, name, mod->flags&FLAG_STATIC);
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
    variable_t*v = 0;

    if(as3_pass==1) {
        return_type = 0; // not valid in pass 1
        if(name) {
            v = new_variable2(parent_method, name, 0, 0, 0);
        }
    }

    new_state();
    state->new_vars = 1;
   
    if(as3_pass == 1) {
        state->method = methodstate_new();
        state->method->inner = 1;
        state->method->is_static = parent_method->is_static;
        state->method->variable_count = 0;
        state->method->abc = rfx_calloc(sizeof(abc_method_t));
        if(v) {
            v->is_inner_method = state->method;
        }

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

    if(as3_pass == 1) {
        state->method = methodstate_new();
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
            memberinfo_t*m = registry_findmember(state->cls->info, mod->ns, name, 2, mod->flags&FLAG_STATIC);
            check_override(m, mod->flags);
        }
            
        if(state->cls) { 
            state->cls->has_constructor |= state->method->is_constructor;
        }
        
        function_initvars(state->method, 1, params, mod->flags, 1);
    } 
}

static void insert_unresolved(methodstate_t*m, dict_t*xvars, dict_t*allvars)
{
    parserassert(m->inner);
    if(m->unresolved_variables) {
        dict_t*d = m->unresolved_variables;
        int t;
        DICT_ITERATE_KEY(d, char*, id) {
            /* check parent method's variables */
            variable_t*v;
            if(dict_contains(allvars, id)) {
                m->uses_parent_function = 1;
                state->method->uses_slots = 1;
                dict_put(xvars, id, 0);
            }
        }
    }
    methodstate_list_t*ml = m->innerfunctions;
    while(ml) {
        insert_unresolved(ml->methodstate, xvars, allvars);
        ml = ml->next;
    }
}

static abc_method_t* endfunction(modifiers_t*mod, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type, code_t*body)
{
    if(as3_pass==1) {
        dict_t*xvars = dict_new();
        
        if(state->method->unresolved_variables) {
            DICT_ITERATE_KEY(state->method->unresolved_variables, char*, vname) {
                if(!state->method->no_variable_scoping && dict_contains(state->method->allvars, vname)) {
                    variable_t*v = dict_lookup(state->method->allvars, vname);
                    if(!v->is_inner_method) {
                        state->method->no_variable_scoping = 1;
                        as3_warning("function %s uses forward or outer block variable references (%s): switching into compatibility mode", name, vname);
                    }
                }
            }
        }

        methodstate_list_t*ml = state->method->innerfunctions;
        while(ml) {
            insert_unresolved(ml->methodstate, xvars, state->method->allvars);
            ml = ml->next;
        }
        
        if(state->method->uses_slots) {
            state->method->slots = dict_new();
            int i = 1;
            DICT_ITERATE_ITEMS(state->method->allvars, char*, name, variable_t*, v) {
                if(!name) syntaxerror("internal error");
                if(v->index && dict_contains(xvars, name)) {
                    v->init = v->kill = 0;
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
            if(!global->init) global->init = abc_initscript(global->file);
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
        return abc_coerce2(c, &m);
    if(TYPE_IS_OBJECT(from) && TYPE_IS_XMLLIST(to))
        return abc_coerce2(c, &m);
    if(TYPE_IS_OBJECT(from) && TYPE_IS_ARRAY(to))
        return abc_coerce2(c, &m);

    classinfo_t*supertype = from;
    while(supertype) {
        if(supertype == to) {
             /* target type is one of from's superclasses.
                (not sure we need this coerce - as far as the verifier
                 is concerned, object==object (i think) */
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
code_t* coerce_to_type(code_t*c, classinfo_t*t)
{
    if(!t) {
        return abc_coerce_a(c);
    } else if(TYPE_IS_STRING(t)) {
        return abc_coerce_s(c);
    } else {
        MULTINAME(m, t);
        return abc_coerce2(c, &m);
    }
}

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
#line 2017 "parser.tab.c"
/* Unqualified %code blocks.  */

/* Line 274 of skeleton.m4  */
#line 2107 "parser.y"

    char is_subtype_of(classinfo_t*type, classinfo_t*supertype)
    {
        return 1; // FIXME
    }
    char do_init_variable(char*name)
    {
        if(!state->method->no_variable_scoping)
            return 0;
        if(!state->new_vars)
            return 1;
        return 1;
    }


/* Line 274 of skeleton.m4  */
#line 2557 "parser.y"

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
#line 2700 "parser.y"

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
            if(!global->init) global->init = abc_initscript(global->file);
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
            
            if(ns.access == ACCESS_PROTECTED) {
                ns.name = concat3(state->cls->info->package,":",state->cls->info->name);
            }
        }
        if(c)
            *c = code;
        if(m) 
            *m = *multiname_clone(&mname);
            
        return trait_new_member(traits, 0, multiname_clone(&mname), 0);
    }


/* Line 274 of skeleton.m4  */
#line 2874 "parser.y"

    static int xml_level = 0;


/* Line 274 of skeleton.m4  */
#line 3541 "parser.y"

    node_t* resolve_identifier(const char*name);
    node_t* get_descendants(node_t*e,const char*ns,const char*subnode,char multi, char attr)
    {
	typedcode_t v = node_read(e);
	typedcode_t w;

	multiname_t m = {0,0,0,subnode};
	namespace_t zero = {ZERONAMESPACE,"*"};
	if(!strcmp(ns,"*")) {
	    m.ns = &zero;
	    m.type = attr?QNAMEA:QNAME;
	} else {
	    typedcode_t w = node_read(resolve_identifier(ns));
	    if(!TYPE_IS_NAMESPACE(w.t)) {
		as3_softwarning("%s might not be a namespace", ns);
	    }
	    v.c = code_append(v.c, w.c);
	    v.c = converttype(v.c, w.t, TYPE_NAMESPACE);
	    m.type = attr?RTQNAMEA:RTQNAME;
	}

	if(!multi) {
	    v.c = abc_getproperty2(v.c, &m);
	} else {
	    v.c = abc_getdescendants2(v.c, &m);
	}

	if(TYPE_IS_XML(v.t)) {
	    v.t = TYPE_XMLLIST;
	} else {
	    v.c = abc_coerce_a(v.c);
	    v.t = TYPE_ANY;
	}
	return mkcodenode(v);
    }


/* Line 274 of skeleton.m4  */
#line 3712 "parser.y"

    node_t* var_read(variable_t*v)
    {
        typedcode_t o;
	o.c = abc_getlocal(0, v->index);
	o.t = v->type;
	return mkcodenode(o);
    }

    node_t* resolve_identifier(const char*name)
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
	    return var_read(v);
        }
        if((v = find_slot(state->method, name))) {
            o.c = abc_getscopeobject(o.c, 1);
            o.c = abc_getslot(o.c, v->index);
            o.t = v->type;
            return mkcodenode(o);
        }

        int i_am_static = state->method->is_static;

        if(!state->method->inner && !state->xmlfilter && state->cls)
	{
	    /* look at current class' members */
	    if((f = findmember_nsset(state->cls->info, name, 1, i_am_static)))
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
	    /* special case: it's allowed to access non-static constants
	       from a static context */
	    if(i_am_static && (f=findmember_nsset(state->cls->info, name, 1, 0))) {
		if(f->kind == INFOTYPE_VAR && (f->flags&FLAG_CONST)) {
		    varinfo_t*v = (varinfo_t*)f;
		    if(v->value) {
			return mkconstnode(v->value);
		    }
		}
	    }
	}
        
        /* look at actual classes, in the current package and imported */
        if(!state->xmlfilter && (a = find_class(name))) {
	    registry_use(a);
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
            o.c = abc___pushpackage__(o.c, (char*)name);
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
#line 3869 "parser.y"

    void add_active_url(const char*url)
    {
        NEW(namespace_t,n);
        n->name = url;
        list_append(state->active_namespace_urls, n);
    }



/* Line 274 of skeleton.m4  */
#line 2315 "parser.tab.c"

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
#define YYFINAL  162
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3382

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  135
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  135
/* YYNRULES -- Number of rules.  */
#define YYNRULES  356
/* YYNRULES -- Number of states.  */
#define YYNSTATES  612

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   364

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   119,     2,     2,     2,   117,   108,     2,
     123,   134,   116,   114,   101,   113,   128,   115,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   105,   100,
     110,   103,   111,   104,   129,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   125,     2,   126,   107,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   127,   106,   133,   118,     2,     2,     2,
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
      95,    96,    97,    98,    99,   102,   109,   112,   120,   121,
     122,   124,   130,   131,   132
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    10,    13,    15,    17,
      19,    21,    23,    25,    30,    32,    33,    35,    37,    40,
      42,    44,    46,    48,    50,    55,    60,    62,    64,    65,
      68,    70,    72,    74,    76,    78,    80,    82,    84,    86,
      88,    90,    92,    94,    96,   100,   103,   105,   107,   109,
     111,   113,   115,   117,   122,   125,   127,   129,   130,   134,
     137,   138,   141,   144,   146,   150,   154,   155,   158,   159,
     162,   169,   170,   172,   174,   178,   180,   183,   187,   196,
     203,   209,   216,   218,   221,   223,   226,   227,   229,   231,
     234,   236,   239,   244,   248,   249,   258,   259,   269,   270,
     276,   278,   281,   283,   286,   288,   289,   296,   299,   301,
     306,   309,   311,   313,   315,   317,   321,   323,   324,   331,
     332,   338,   341,   344,   349,   350,   352,   354,   357,   359,
     361,   363,   365,   367,   369,   371,   373,   375,   377,   378,
     381,   382,   385,   386,   389,   390,   400,   401,   410,   411,
     413,   415,   418,   420,   425,   427,   429,   434,   436,   437,
     439,   441,   444,   446,   449,   458,   460,   462,   463,   468,
     470,   474,   478,   479,   482,   484,   486,   488,   490,   492,
     494,   496,   498,   500,   502,   504,   506,   507,   508,   513,
     514,   519,   520,   523,   526,   529,   532,   536,   538,   540,
     541,   543,   550,   561,   573,   575,   578,   580,   584,   588,
     592,   596,   597,   599,   602,   607,   611,   613,   618,   621,
     623,   625,   626,   627,   640,   642,   643,   644,   655,   657,
     661,   663,   665,   667,   671,   673,   675,   677,   680,   681,
     682,   686,   687,   689,   691,   693,   696,   699,   700,   705,
     710,   715,   718,   720,   723,   725,   727,   729,   733,   735,
     739,   740,   742,   744,   746,   748,   750,   752,   756,   762,
     764,   766,   768,   770,   772,   774,   776,   778,   780,   782,
     786,   790,   794,   798,   802,   806,   810,   814,   818,   822,
     826,   830,   833,   836,   840,   844,   848,   852,   856,   860,
     864,   868,   872,   876,   880,   884,   888,   892,   896,   899,
     902,   904,   908,   911,   916,   920,   924,   928,   932,   936,
     940,   944,   948,   952,   956,   960,   964,   970,   973,   976,
     979,   982,   986,   989,   990,   997,   999,  1001,  1003,  1005,
    1011,  1015,  1021,  1027,  1032,  1039,  1044,  1051,  1058,  1065,
    1069,  1071,  1074,  1079,  1084,  1087,  1092
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     136,     0,    -1,   137,    -1,    -1,   138,    -1,   139,    -1,
     138,   139,    -1,   187,    -1,   199,    -1,   197,    -1,   233,
      -1,   208,    -1,   148,    -1,   150,   127,   137,   133,    -1,
     100,    -1,    -1,   141,    -1,   142,    -1,   141,   142,    -1,
     199,    -1,   197,    -1,   233,    -1,   208,    -1,   148,    -1,
     150,   127,   140,   133,    -1,   125,   149,   260,   126,    -1,
     100,    -1,   144,    -1,    -1,   144,   146,    -1,   146,    -1,
     268,    -1,   190,    -1,   162,    -1,   163,    -1,   164,    -1,
     165,    -1,   172,    -1,   158,    -1,   184,    -1,   180,    -1,
     256,    -1,   269,    -1,   267,    -1,   127,   144,   133,    -1,
     127,   133,    -1,   100,    -1,   145,    -1,   152,    -1,   166,
      -1,   167,    -1,   252,    -1,   182,    -1,   150,   127,   144,
     133,    -1,   146,   100,    -1,   146,    -1,   145,    -1,    -1,
       3,    90,     3,    -1,   103,   260,    -1,    -1,    45,   153,
      -1,    31,   153,    -1,   154,    -1,   153,   101,   154,    -1,
       3,   243,   151,    -1,    -1,   156,   147,    -1,    -1,    65,
     155,    -1,    64,   123,   254,   134,   155,   157,    -1,    -1,
     152,    -1,   256,    -1,    45,     3,   243,    -1,     3,    -1,
      10,   123,    -1,    10,    49,   123,    -1,   161,   159,   100,
     254,   100,   256,   134,   155,    -1,   161,   160,    68,   254,
     134,   155,    -1,    11,   123,   254,   134,   155,    -1,    12,
     155,    11,   123,   254,   134,    -1,    66,    -1,    66,     3,
      -1,    29,    -1,    29,     3,    -1,    -1,   169,    -1,   171,
      -1,   169,   171,    -1,   170,    -1,   169,   170,    -1,    33,
     260,   105,   143,    -1,    61,   105,   143,    -1,    -1,    13,
     123,   173,   260,   134,   127,   168,   133,    -1,    -1,    32,
     123,     3,   243,   134,   175,   127,   143,   133,    -1,    -1,
      26,   127,   177,   143,   133,    -1,   174,    -1,   178,   174,
      -1,   178,    -1,   178,   176,    -1,   176,    -1,    -1,    51,
     127,   181,   143,   133,   179,    -1,    36,   254,    -1,    36,
      -1,    38,   123,   254,   134,    -1,   183,   147,    -1,     3,
      -1,    16,    -1,    15,    -1,    28,    -1,   186,   128,   185,
      -1,   185,    -1,    -1,    16,   186,   127,   188,   140,   133,
      -1,    -1,    16,   127,   189,   140,   133,    -1,    40,     3,
      -1,    40,   239,    -1,    40,   186,   128,   116,    -1,    -1,
     192,    -1,   193,    -1,   192,   193,    -1,    19,    -1,    20,
      -1,    17,    -1,    37,    -1,    46,    -1,    48,    -1,    47,
      -1,    24,    -1,    22,    -1,     3,    -1,    -1,    53,   240,
      -1,    -1,    53,   241,    -1,    -1,    14,   241,    -1,    -1,
     191,    30,     3,   194,   196,   127,   198,   201,   133,    -1,
      -1,   191,    43,     3,   195,   127,   200,   204,   133,    -1,
      -1,   202,    -1,   203,    -1,   202,   203,    -1,   100,    -1,
     150,   127,   201,   133,    -1,   208,    -1,   233,    -1,   125,
     149,   260,   126,    -1,   145,    -1,    -1,   205,    -1,   206,
      -1,   205,   206,    -1,   100,    -1,    45,     3,    -1,   191,
      25,   232,     3,   123,   229,   134,   243,    -1,    45,    -1,
      31,    -1,    -1,   191,   207,   209,   210,    -1,   211,    -1,
     210,   101,   211,    -1,     3,   243,   151,    -1,    -1,   103,
     260,    -1,     7,    -1,     8,    -1,     9,    -1,     4,    -1,
      55,    -1,    54,    -1,    44,    -1,    27,    -1,    28,    -1,
     226,    -1,   110,    -1,   111,    -1,    -1,    -1,   127,   260,
     219,   133,    -1,    -1,   127,   260,   221,   133,    -1,    -1,
     222,   218,    -1,   222,     4,    -1,   222,   111,    -1,   226,
     222,    -1,   223,   226,   222,    -1,     3,    -1,   220,    -1,
      -1,   227,    -1,   215,   224,   225,   115,   217,   111,    -1,
     215,   224,   225,   216,   222,   110,   115,   224,   217,   111,
      -1,   215,   224,   225,   216,   222,   223,   110,   115,   224,
     217,   111,    -1,   228,    -1,   227,   228,    -1,   220,    -1,
     220,   103,     4,    -1,   220,   103,   220,    -1,     3,   103,
     220,    -1,     3,   103,     4,    -1,    -1,   230,    -1,    94,
     231,    -1,   230,   101,    94,   231,    -1,   230,   101,   231,
      -1,   231,    -1,     3,   105,   242,   212,    -1,     3,   212,
      -1,    50,    -1,    34,    -1,    -1,    -1,   191,    25,   232,
       3,   123,   229,   134,   243,   127,   234,   143,   133,    -1,
       3,    -1,    -1,    -1,    25,   235,   123,   229,   134,   243,
     127,   237,   143,   133,    -1,   185,    -1,   186,   128,   185,
      -1,   239,    -1,   238,    -1,   240,    -1,   241,   101,   240,
      -1,   240,    -1,   116,    -1,    35,    -1,   105,   242,    -1,
      -1,    -1,   123,   245,   134,    -1,    -1,   246,    -1,   247,
      -1,   253,    -1,   246,   101,    -1,   247,   253,    -1,    -1,
      23,   260,   248,   244,    -1,   260,   123,   245,   134,    -1,
      52,   123,   245,   134,    -1,    63,   260,    -1,    41,    -1,
      41,   254,    -1,   260,    -1,   255,    -1,   260,    -1,   255,
     101,   260,    -1,   260,    -1,   256,   101,   260,    -1,    -1,
     259,    -1,     3,    -1,     4,    -1,     7,    -1,     8,    -1,
       9,    -1,   258,   105,   253,    -1,   259,   101,   258,   105,
     253,    -1,   236,    -1,   264,    -1,   249,    -1,   251,    -1,
     250,    -1,   265,    -1,   213,    -1,   214,    -1,     5,    -1,
      18,    -1,   125,   245,   126,    -1,    70,   257,   133,    -1,
     260,   110,   260,    -1,   260,   111,   260,    -1,   260,    75,
     260,    -1,   260,    76,   260,    -1,   260,    71,   260,    -1,
     260,    72,   260,    -1,   260,    74,   260,    -1,   260,    73,
     260,    -1,   260,    88,   260,    -1,   260,    89,   260,    -1,
     119,   260,    -1,   118,   260,    -1,   260,   108,   260,    -1,
     260,   107,   260,    -1,   260,   106,   260,    -1,   260,    97,
     260,    -1,   260,    96,   260,    -1,   260,    95,   260,    -1,
     260,   115,   260,    -1,   260,   117,   260,    -1,   260,   114,
     260,    -1,   260,   113,   260,    -1,   260,   116,   260,    -1,
     260,    68,   260,    -1,   260,    69,   260,    -1,   260,    39,
     260,    -1,   260,    67,   260,    -1,    42,   260,    -1,    35,
     260,    -1,    35,    -1,   123,   255,   134,    -1,   113,   260,
      -1,   260,   125,   260,   126,    -1,   260,    80,   260,    -1,
     260,    79,   260,    -1,   260,    86,   260,    -1,   260,    85,
     260,    -1,   260,    87,   260,    -1,   260,    78,   260,    -1,
     260,    77,   260,    -1,   260,    84,   260,    -1,   260,    81,
     260,    -1,   260,    82,   260,    -1,   260,    83,   260,    -1,
     260,   103,   260,    -1,   260,   104,   260,   105,   260,    -1,
     260,    92,    -1,   260,    91,    -1,    92,   260,    -1,    91,
     260,    -1,    52,   128,     3,    -1,   129,     3,    -1,    -1,
     260,   128,   123,   261,   260,   134,    -1,     3,    -1,   116,
      -1,   185,    -1,   116,    -1,   260,   128,   262,    90,   263,
      -1,   260,    93,   263,    -1,   260,    93,   262,    90,   263,
      -1,   260,   128,   125,   260,   126,    -1,   260,   128,   129,
     263,    -1,   260,   128,   129,   262,    90,   263,    -1,   260,
      93,   129,   263,    -1,   260,    93,   129,   262,    90,   263,
      -1,   260,   128,   129,   125,   260,   126,    -1,   260,    93,
     129,   125,   260,   126,    -1,   260,   128,   263,    -1,     3,
      -1,    15,     3,    -1,    15,     3,   103,     3,    -1,    15,
       3,   103,     4,    -1,   191,   266,    -1,    62,    15,   103,
     260,    -1,    21,    15,   240,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1998,  1998,  2000,  2000,  2001,  2002,  2004,  2005,  2006,
    2007,  2008,  2009,  2010,  2011,  2013,  2013,  2014,  2015,  2017,
    2018,  2019,  2020,  2021,  2022,  2023,  2024,  2026,  2027,  2029,
    2032,  2035,  2036,  2037,  2038,  2039,  2040,  2041,  2042,  2043,
    2044,  2045,  2046,  2047,  2048,  2049,  2052,  2053,  2054,  2055,
    2056,  2057,  2058,  2059,  2071,  2072,  2076,  2087,  2095,  2122,
    2123,  2125,  2126,  2128,  2129,  2131,  2192,  2192,  2196,  2197,
    2200,  2216,  2217,  2218,  2223,  2227,  2232,  2233,  2235,  2255,
    2299,  2315,  2327,  2330,  2333,  2336,  2340,  2341,  2342,  2343,
    2344,  2345,  2347,  2358,  2361,  2361,  2392,  2392,  2417,  2417,
    2433,  2434,  2435,  2436,  2444,  2453,  2453,  2502,  2506,  2517,
    2527,  2544,  2545,  2546,  2547,  2549,  2550,  2552,  2552,  2554,
    2554,  2578,  2586,  2600,  2616,  2617,  2618,  2619,  2626,  2627,
    2628,  2629,  2630,  2631,  2632,  2633,  2634,  2635,  2639,  2640,
    2642,  2643,  2645,  2646,  2650,  2648,  2656,  2654,  2663,  2664,
    2665,  2666,  2667,  2668,  2669,  2670,  2671,  2673,  2679,  2680,
    2681,  2682,  2683,  2684,  2687,  2760,  2760,  2762,  2762,  2764,
    2765,  2767,  2851,  2852,  2860,  2861,  2864,  2865,  2866,  2867,
    2868,  2869,  2870,  2878,  2889,  2890,  2891,  2893,  2893,  2896,
    2896,  2899,  2900,  2903,  2908,  2911,  2914,  2917,  2920,  2924,
    2927,  2931,  2935,  2940,  2946,  2949,  2952,  2955,  2960,  2963,
    2966,  2976,  2980,  2986,  2992,  3000,  3005,  3011,  3019,  3027,
    3028,  3029,  3032,  3031,  3048,  3049,  3051,  3050,  3074,  3094,
    3109,  3110,  3112,  3113,  3115,  3116,  3117,  3126,  3127,  3131,
    3132,  3134,  3135,  3136,  3138,  3142,  3143,  3148,  3149,  3196,
    3246,  3267,  3289,  3292,  3299,  3302,  3305,  3308,  3311,  3314,
    3319,  3320,  3322,  3323,  3324,  3325,  3326,  3328,  3334,  3343,
    3344,  3345,  3346,  3347,  3348,  3350,  3354,  3359,  3377,  3388,
    3398,  3407,  3408,  3409,  3410,  3411,  3412,  3413,  3414,  3415,
    3416,  3417,  3418,  3419,  3420,  3421,  3422,  3423,  3424,  3425,
    3426,  3427,  3428,  3429,  3430,  3431,  3432,  3433,  3434,  3435,
    3436,  3437,  3438,  3439,  3440,  3441,  3442,  3443,  3444,  3445,
    3446,  3447,  3448,  3449,  3450,  3451,  3452,  3454,  3455,  3456,
    3457,  3459,  3474,  3482,  3482,  3536,  3537,  3538,  3539,  3579,
    3582,  3589,  3592,  3603,  3611,  3615,  3622,  3626,  3636,  3647,
    3844,  3878,  3885,  3892,  3899,  3921,  3928
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IDENTIFIER", "T_STRING", "T_REGEXP",
  "T_EMPTY", "T_INT", "T_UINT", "T_FLOAT", "\"for\"", "\"while\"",
  "\"do\"", "\"switch\"", "\"implements\"", "\"namespace\"", "\"package\"",
  "\"protected\"", "\"arguments\"", "\"public\"", "\"private\"", "\"use\"",
  "\"internal\"", "\"new\"", "\"native\"", "\"function\"", "\"finally\"",
  "\"undefined\"", "\"NaN\"", "\"continue\"", "\"class\"", "\"const\"",
  "\"catch\"", "\"case\"", "\"set\"", "\"void\"", "\"throw\"",
  "\"static\"", "\"with\"", "\"instanceof\"", "\"import\"", "\"return\"",
  "\"typeof\"", "\"interface\"", "\"null\"", "\"var\"", "\"dynamic\"",
  "\"override\"", "\"final\"", "\"each\"", "\"get\"", "\"try\"",
  "\"super\"", "\"extends\"", "\"false\"", "\"true\"", "\"Boolean\"",
  "\"uint\"", "\"int\"", "\"Number\"", "\"String\"", "\"default\"",
  "\"default xml\"", "\"delete\"", "\"if\"", "\"else\"", "\"break\"",
  "\"is\"", "\"in\"", "\"as\"", "\"{ (dictionary)\"", "\"==\"", "\"===\"",
  "\"!=\"", "\"!==\"", "\"<=\"", "\">=\"", "\"|=\"", "\"/=\"", "\"%=\"",
  "\"*=\"", "\"&=\"", "\"+=\"", "\"-=\"", "\"^=\"", "\">>=\"", "\"<<=\"",
  "\">>>=\"", "\"||\"", "\"&&\"", "\"::\"", "\"--\"", "\"++\"", "\"..\"",
  "\"...\"", "\"<<\"", "\">>>\"", "\">>\"", "prec_none", "below_semicolon",
  "';'", "','", "below_assignment", "'='", "'?'", "':'", "'|'", "'^'",
  "'&'", "below_lt", "'<'", "'>'", "below_minus", "'-'", "'+'", "'/'",
  "'*'", "'%'", "'~'", "'!'", "minusminus_prefix", "plusplus_prefix",
  "below_curly", "'('", "new2", "'['", "']'", "'{'", "'.'", "'@'",
  "above_identifier", "below_else", "above_function", "'}'", "')'",
  "$accept", "PROGRAM", "MAYBE_PROGRAM_CODE_LIST", "PROGRAM_CODE_LIST",
  "PROGRAM_CODE", "MAYBE_INPACKAGE_CODE_LIST", "INPACKAGE_CODE_LIST",
  "INPACKAGE_CODE", "MAYBECODE", "CODE", "CODE_STATEMENT", "CODEPIECE",
  "CODEBLOCK", "PACKAGE_INITCODE", "EMBED_START",
  "CONDITIONAL_COMPILATION", "MAYBEEXPRESSION", "VARIABLE_DECLARATION",
  "VARIABLE_LIST", "ONE_VARIABLE", "IF_CODEBLOCK", "$@1", "MAYBEELSE",
  "IF", "FOR_INIT", "FOR_IN_INIT", "FOR_START", "FOR", "FOR_IN", "WHILE",
  "DO_WHILE", "BREAK", "CONTINUE", "MAYBE_CASE_LIST", "CASE_LIST", "CASE",
  "DEFAULT", "SWITCH", "$@2", "CATCH", "$@3", "FINALLY", "$@4",
  "CATCH_LIST", "CATCH_FINALLY_LIST", "TRY", "$@5", "THROW", "WITH_HEAD",
  "WITH", "X_IDENTIFIER", "PACKAGE", "PACKAGE_DECLARATION", "$@6", "$@7",
  "IMPORT", "MAYBE_MODIFIERS", "MODIFIER_LIST", "MODIFIER", "EXTENDS",
  "EXTENDS_LIST", "IMPLEMENTS_LIST", "CLASS_DECLARATION", "$@8",
  "INTERFACE_DECLARATION", "$@9", "MAYBE_CLASS_BODY", "CLASS_BODY",
  "CLASS_BODY_ITEM", "MAYBE_INTERFACE_BODY", "INTERFACE_BODY",
  "IDECLARATION", "VARCONST", "SLOT_DECLARATION", "$@10", "SLOT_LIST",
  "ONE_SLOT", "MAYBECONSTANT", "CONSTANT", "XML", "OPEN", "CLOSE",
  "CLOSE2", "XMLEXPR1", "$@11", "XMLEXPR2", "$@12", "XMLTEXT", "XML2",
  "XML_ID_OR_EXPR", "MAYBE_XMLATTRIBUTES", "XMLNODE", "XMLATTRIBUTES",
  "XMLATTRIBUTE", "MAYBE_PARAM_LIST", "PARAM_LIST", "PARAM", "GETSET",
  "FUNCTION_DECLARATION", "$@13", "MAYBE_IDENTIFIER", "INNERFUNCTION",
  "$@14", "CLASS", "PACKAGEANDCLASS", "CLASS_SPEC", "CLASS_SPEC_LIST",
  "TYPE", "MAYBETYPE", "MAYBE_PARAM_VALUES", "MAYBE_EXPRESSION_LIST",
  "EXPRESSION_LIST", "EXPRESSION_LIST_AND_COMMA", "XX", "NEW",
  "FUNCTIONCALL", "DELETE", "RETURN", "NONCOMMAEXPRESSION", "EXPRESSION",
  "COMMA_EXPRESSION", "VOIDEXPRESSION", "MAYBE_DICT_EXPRPAIR_LIST",
  "DICTLH", "DICT_EXPRPAIR_LIST", "E", "$@15", "ID_OR_NS", "SUBNODE",
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
      59,    44,   355,    61,    63,    58,   124,    94,    38,   356,
      60,    62,   357,    45,    43,    47,    42,    37,   126,    33,
     358,   359,   360,    40,   361,    91,    93,   123,    46,    64,
     362,   363,   364,   125,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   135,   136,   137,   137,   138,   138,   139,   139,   139,
     139,   139,   139,   139,   139,   140,   140,   141,   141,   142,
     142,   142,   142,   142,   142,   142,   142,   143,   143,   144,
     144,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   146,   146,   146,   146,
     146,   146,   146,   146,   147,   147,   148,   149,   150,   151,
     151,   152,   152,   153,   153,   154,   156,   155,   157,   157,
     158,   159,   159,   159,   160,   160,   161,   161,   162,   163,
     164,   165,   166,   166,   167,   167,   168,   168,   168,   168,
     169,   169,   170,   171,   173,   172,   175,   174,   177,   176,
     178,   178,   179,   179,   179,   181,   180,   182,   182,   183,
     184,   185,   185,   185,   185,   186,   186,   188,   187,   189,
     187,   190,   190,   190,   191,   191,   192,   192,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   194,   194,
     195,   195,   196,   196,   198,   197,   200,   199,   201,   201,
     202,   202,   203,   203,   203,   203,   203,   203,   204,   204,
     205,   205,   206,   206,   206,   207,   207,   209,   208,   210,
     210,   211,   212,   212,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   214,   215,   216,   217,   219,   218,   221,
     220,   222,   222,   222,   222,   223,   223,   224,   224,   225,
     225,   226,   226,   226,   227,   227,   228,   228,   228,   228,
     228,   229,   229,   229,   229,   230,   230,   231,   231,   232,
     232,   232,   234,   233,   235,   235,   237,   236,   238,   239,
     240,   240,   241,   241,   242,   242,   242,   243,   243,   244,
     244,   245,   245,   245,   246,   247,   246,   248,   249,   250,
     250,   251,   252,   252,   253,   254,   255,   255,   256,   256,
     257,   257,   258,   258,   258,   258,   258,   259,   259,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   261,   260,   262,   262,   263,   263,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   264,
     265,   266,   266,   266,   267,   268,   269
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     4,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     1,     4,     4,     1,     1,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     4,     2,     1,     1,     0,     3,     2,
       0,     2,     2,     1,     3,     3,     0,     2,     0,     2,
       6,     0,     1,     1,     3,     1,     2,     3,     8,     6,
       5,     6,     1,     2,     1,     2,     0,     1,     1,     2,
       1,     2,     4,     3,     0,     8,     0,     9,     0,     5,
       1,     2,     1,     2,     1,     0,     6,     2,     1,     4,
       2,     1,     1,     1,     1,     3,     1,     0,     6,     0,
       5,     2,     2,     4,     0,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     2,
       0,     2,     0,     2,     0,     9,     0,     8,     0,     1,
       1,     2,     1,     4,     1,     1,     4,     1,     0,     1,
       1,     2,     1,     2,     8,     1,     1,     0,     4,     1,
       3,     3,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     0,     4,     0,
       4,     0,     2,     2,     2,     2,     3,     1,     1,     0,
       1,     6,    10,    11,     1,     2,     1,     3,     3,     3,
       3,     0,     1,     2,     4,     3,     1,     4,     2,     1,
       1,     0,     0,    12,     1,     0,     0,    10,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     2,     0,     0,
       3,     0,     1,     1,     1,     2,     2,     0,     4,     4,
       4,     2,     1,     2,     1,     1,     1,     3,     1,     3,
       0,     1,     1,     1,     1,     1,     1,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       1,     3,     2,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     2,     2,     2,
       2,     3,     2,     0,     6,     1,     1,     1,     1,     5,
       3,     5,     5,     4,     6,     4,     6,     6,     6,     3,
       1,     2,     4,     4,     2,     4,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     124,   350,   177,   277,   174,   175,   176,     0,     0,    66,
       0,     0,   130,   278,   128,   129,     0,   136,     0,   135,
     225,   181,   182,   310,   131,     0,     0,     0,   180,   132,
     134,   133,     0,     0,   179,   178,     0,     0,     0,   260,
       0,     0,    14,   184,     0,     0,     0,     0,   241,   124,
       0,     0,     2,   124,     5,    56,    12,     0,    38,    71,
      33,    34,    35,    36,    37,    40,   124,    39,     7,    32,
       0,   125,   126,     9,     8,    11,   275,   276,     0,   183,
      10,   269,   271,   273,   272,    41,   258,   270,   274,    43,
      31,    42,     0,     0,    76,     0,     0,   124,    94,   111,
     113,   112,   114,   119,   116,     0,     0,   350,   247,   224,
       0,   309,     0,   121,     0,   122,   308,   105,   241,     0,
       0,   251,     0,   262,   263,   264,   265,   266,     0,     0,
     261,   330,   329,   312,   292,   291,     0,   256,     0,   242,
     243,   244,   254,    84,     0,   108,   252,     0,    82,    46,
      45,   124,    47,    30,     0,    48,    49,    50,    52,     0,
      51,   332,     1,     6,   124,   350,     0,    72,     0,     0,
      73,    55,   110,     0,   221,     0,   166,     0,   165,   167,
     354,   137,   127,   197,     0,   198,   199,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   328,   327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   241,
       0,     0,    58,    77,     0,   255,     0,    67,     0,   124,
     117,     0,   228,     0,   231,   230,   356,   239,   211,     0,
       0,    28,     0,   331,     0,     0,   280,     0,     0,     0,
     311,   279,   245,   246,    85,   238,    62,    63,   107,   253,
      61,    83,    44,    29,   124,     0,   238,     0,     0,    54,
     351,   220,   219,     0,   138,   140,     0,   189,     0,   206,
       0,   200,   204,   259,   306,   307,   304,   305,   285,   286,
     288,   287,   283,   284,   320,   319,   315,   314,   322,   323,
     324,   321,   317,   316,   318,   289,   290,   111,   338,     0,
     337,     0,   340,   298,   297,   296,   325,     0,   295,   294,
     293,   281,   282,   302,   301,   299,   303,   300,     0,     0,
     333,     0,     0,     0,   349,    66,     0,     0,    26,    57,
       0,   124,    17,    23,     0,    20,    19,    22,    21,   124,
     115,     0,   241,   248,   172,     0,     0,   212,   216,   109,
     123,   229,     0,    27,   250,   355,    66,   267,     0,   257,
       0,    60,     0,   124,    13,    60,     0,     0,     0,     0,
       0,   142,     0,     0,   238,   168,   169,     0,     0,     0,
     185,   186,   191,   205,     0,     0,   345,     0,     0,   249,
     313,     0,     0,     0,     0,   343,     0,    80,     0,     0,
       0,   120,    18,   124,     0,     0,     0,     0,   218,   213,
     238,     0,     0,    68,     0,   236,   235,   234,   237,     0,
      65,    64,    53,     0,    66,   352,   353,   211,   139,     0,
       0,   232,   141,   146,    60,     0,   190,   210,   209,   207,
     208,     0,     0,     0,     0,   338,   341,   326,     0,   342,
       0,     0,   339,    81,    86,     0,     0,   118,   240,   173,
     172,     0,     0,   215,     0,     0,   100,   104,   102,   106,
      66,    70,   268,    59,     0,    79,     0,   143,   144,     0,
     124,   171,   170,   201,   193,   184,   194,     0,   192,     0,
     191,   348,   346,   334,   347,   344,     0,     0,     0,    87,
      90,    88,    25,    24,   217,   226,   214,    98,     0,   101,
     103,    69,    66,   238,   124,   233,     0,   162,     0,     0,
     124,   160,     0,   187,   184,   191,   195,     0,    28,    95,
      91,    89,    28,    28,   238,    78,     0,   152,    57,   157,
       0,     0,     0,   124,   150,   154,   155,   163,   221,   147,
     161,   186,     0,     0,   196,    28,    93,     0,     0,     0,
     222,     0,   124,   145,   151,     0,     0,   188,   186,    92,
     227,    99,    96,    28,     0,     0,     0,   202,     0,     0,
       0,   156,   153,   211,   203,    28,   223,     0,     0,   238,
      97,   164
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    51,    52,    53,    54,   350,   351,   352,   372,   373,
     152,   153,   172,   353,   420,   154,   440,   155,   270,   267,
      96,    97,   491,    58,   168,   169,    59,    60,    61,    62,
      63,   156,   157,   518,   519,   520,   521,    64,   238,   486,
     599,   487,   553,   488,   489,    65,   251,   158,    66,    67,
     320,   243,    68,   359,   239,    69,   159,    71,    72,   391,
     393,   450,   355,   534,   356,   500,   562,   563,   564,   539,
     540,   541,   179,   357,   286,   395,   396,   428,    76,    77,
      78,   402,   461,   508,   572,   185,   397,   462,   509,   186,
     290,    79,   291,   292,   366,   367,   368,   283,   358,   593,
     110,    81,   552,   244,   245,   437,   452,   438,   381,   363,
     138,   139,   140,   247,    82,    83,    84,   160,   141,   234,
     235,    85,   128,   129,   130,    86,   411,   321,   322,    87,
      88,   180,    89,    90,    91
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -508
static const yytype_int16 yypact[] =
{
    1734,  1659,  -508,  -508,  -508,  -508,  -508,    -3,   -45,  -508,
     -10,    93,  -508,  -508,  -508,  -508,    51,  -508,  2068,  -508,
      84,  -508,  -508,  2125,  -508,    -5,    73,  2068,  -508,  -508,
    -508,  -508,     8,   -49,  -508,  -508,   121,  2068,    22,   381,
    2068,  2068,  -508,  -508,  2068,  2068,  2068,  2068,  2068,   714,
     144,   155,  -508,   587,  -508,  -508,  -508,    32,  -508,   429,
    -508,  -508,  -508,  -508,  -508,  -508,  1988,  -508,  -508,  -508,
     126,   469,  -508,  -508,  -508,  -508,  -508,  -508,    15,  -508,
    -508,  -508,  -508,  -508,  -508,   131,  2882,  -508,  -508,  -508,
    -508,  -508,   219,   111,  -508,  2068,   224,  1988,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,   -23,   279,  -508,   -55,  -508,
     115,   -43,  2068,   116,   118,  -508,   -43,  -508,  2068,   240,
     145,   -43,  2068,  -508,  -508,  -508,  -508,  -508,   125,   154,
     148,   -43,   -43,   386,   -43,   -43,   -39,  2820,   135,   163,
    2068,  -508,  2820,   264,   265,  2068,  2068,   265,   274,  -508,
    -508,   841,  -508,  -508,   161,  -508,  -508,  -508,  -508,   275,
    -508,  -508,  -508,  -508,  1095,   225,   297,  -508,   201,   235,
     131,   205,  -508,   305,    90,   307,  -508,   308,  -508,  -508,
    -508,  -508,  -508,  -508,  2068,  -508,    16,  2068,  2068,  2068,
    2068,  2068,  2068,  2068,  2068,  2068,  2068,  2068,  2068,  2068,
    2068,  2068,  2068,  2068,  2068,  2068,  2068,  2068,  2068,  2068,
    2068,  -508,  -508,    44,  2068,  2068,  2068,  2068,  2068,  2068,
    2068,  2068,  2068,  2068,  2068,  2068,  2068,  2068,  2068,  2068,
    2068,   146,  -508,  -508,   179,   217,   198,  -508,  2068,  1222,
    -508,   279,   194,   199,  -508,  -508,  -508,   207,    23,   197,
     342,  1861,   202,  -508,  2068,   204,  -508,  2068,   381,  2068,
    -508,  -508,  -508,  -508,  -508,   223,   234,  -508,  -508,  -508,
     234,  -508,  -508,  -508,  1988,   211,   223,  2068,  2068,  -508,
     236,  -508,  -508,   343,   296,   301,   353,  2820,   259,   260,
      -4,    16,  -508,  2820,  1087,   579,   579,   579,  3254,  3254,
    3254,  3254,  1087,  1087,  2820,  2820,  2820,  2820,  2820,  2820,
    2820,  2820,  2820,  2820,  2820,  2944,  3006,   282,   285,   257,
    -508,   286,  -508,   679,   679,   679,  2820,  2324,  3068,  3130,
    3192,  1087,  1087,   386,   386,   -43,   -43,   -43,   230,  2386,
    -508,  2068,   281,   289,  -508,  -508,  2068,  2184,  -508,   243,
     238,  1349,  -508,  -508,   254,  -508,  -508,  -508,  -508,  1222,
    -508,   279,  2068,  -508,    47,   380,   252,   293,  -508,  -508,
    -508,   272,   269,  1861,  -508,  2820,  -508,  -508,   299,  2820,
     271,   304,   265,   968,  -508,   -26,   309,   276,   122,   288,
     279,   398,   279,   290,   223,   312,  -508,   283,    12,    17,
    -508,  -508,  -508,  -508,  2068,   325,  -508,   352,  2068,  -508,
    -508,  2068,  2448,  2068,   328,  -508,   352,  -508,   287,   292,
    2068,  -508,  -508,  1222,   298,   310,  2068,   271,  -508,  -508,
     223,    38,    67,   355,  2068,  -508,  -508,  -508,  -508,  2068,
    -508,  -508,  -508,  2068,  -508,  -508,  -508,    23,  -508,   279,
     302,  -508,   329,  -508,   304,   353,  -508,  -508,  -508,  -508,
    -508,   315,    19,  2510,   352,  -508,  -508,  2820,  2254,  -508,
    2572,   352,  -508,  -508,    42,  2634,   306,  -508,  -508,  2820,
     332,   314,   380,  -508,   316,   319,  -508,  -508,    67,  -508,
    -508,  -508,  -508,  2820,   -36,  -508,   311,   329,  -508,   279,
     233,  -508,  -508,  -508,  -508,   331,  -508,  2068,  -508,   338,
    -508,  -508,  -508,  -508,  -508,  -508,  2068,   344,   317,    42,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,   448,  -508,
    -508,  -508,  -508,   223,  1476,  -508,   450,  -508,   430,   326,
     295,  -508,    15,  2820,   346,  -508,    26,  2696,  1861,  -508,
    -508,  -508,  1861,  1861,   223,  -508,   336,  -508,   243,  -508,
     339,   206,   334,  1603,  -508,  -508,  -508,  -508,    90,  -508,
    -508,  -508,   337,    15,    26,  1861,  -508,   347,   349,   335,
    -508,  2068,  1476,  -508,  -508,   462,   364,  -508,  -508,  -508,
    -508,  -508,  -508,  1861,  2758,   354,   362,  -508,   365,   363,
     361,  -508,  -508,    23,  -508,  1861,  -508,   371,   374,   223,
    -508,  -508
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -508,  -508,   333,  -508,   443,  -339,  -508,   147,  -228,   -41,
       1,   -57,   411,    37,   -48,     2,    59,   459,   375,   140,
    -332,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,     5,     6,  -508,  -508,    35,
    -508,    39,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
      13,    41,  -508,  -508,  -508,  -508,     0,  -508,   455,  -508,
    -508,  -508,    61,  -508,    63,  -508,   -53,  -508,   -35,  -508,
    -508,    -9,  -508,     3,  -508,  -508,    75,    52,  -508,  -508,
    -508,  -508,  -507,  -508,  -508,  -169,  -508,  -459,  -508,  -471,
    -508,  -440,  -508,   242,  -433,  -508,  -334,   -27,     4,  -508,
    -508,  -508,  -508,  -508,   517,  -101,    95,   123,  -231,  -508,
    -106,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -130,   -54,
     498,   -52,  -508,   291,  -508,   -12,  -508,  -204,  -188,  -508,
    -508,  -508,  -508,  -508,  -508
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -337
static const yytype_int16 yytable[] =
{
      70,    55,    57,    75,    80,   246,   108,   170,   151,   171,
     263,   111,   252,   417,   496,   116,   457,   289,   183,   288,
     424,   459,   510,   504,   104,   121,   364,   343,   131,   132,
     504,   429,   133,   134,   135,   137,   142,    56,   213,   104,
     171,   364,   -74,   344,   433,   385,    93,   317,   211,   212,
     213,   546,   105,    70,    55,    57,    75,    80,   249,   100,
     101,    73,   259,    74,   586,   187,   106,   114,   255,   545,
     230,   571,   102,   231,   118,   516,   113,   439,    95,   119,
     229,   598,   230,   137,   476,   231,   574,   109,   100,   101,
      56,   268,   269,   484,   273,   260,    99,   483,   532,   485,
     137,   102,   588,   517,   240,   241,   142,   400,   100,   101,
     137,   401,   495,    98,    73,   405,    74,   365,   112,   242,
      94,   102,   289,   338,   281,   445,   446,   377,   142,   505,
     506,   406,   482,   137,   137,   117,   120,   506,   414,   184,
     282,   173,   184,   184,   184,   122,   507,   161,   526,   317,
     426,   174,   427,   507,   415,   162,   175,   176,   531,   164,
     318,   100,   101,   454,    70,    55,    57,    75,    80,   177,
     607,   178,   287,   319,   102,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   481,
     555,    56,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   142,   339,   466,
     103,   173,   232,   386,   387,    73,   347,    74,   472,   458,
     460,   174,   187,   383,   233,   236,   181,   176,   248,    70,
      55,   354,   375,   253,  -111,   142,   250,   379,   254,   258,
      12,   178,    14,    15,   360,    17,   425,    19,   256,   257,
     317,   261,   318,   371,   262,   137,   137,   264,   265,   340,
      24,   341,   100,   101,    99,   342,   512,   271,   536,    29,
      30,    31,    99,   515,   317,   102,   100,   101,   274,   448,
     173,   451,   418,   -75,   100,   101,   100,   101,   181,   102,
     276,   277,   556,   278,   492,   279,   435,   102,   280,   102,
     284,   285,    12,   345,    14,    15,   273,    17,   259,    19,
     576,   346,  -116,   579,   577,   578,   273,   361,   380,   412,
     362,   369,    24,   537,   137,   382,   374,   142,   376,   388,
     536,    29,    30,    31,   384,    99,   389,   589,   451,   390,
     142,    70,    55,   354,   392,    99,   394,   100,   101,    70,
      55,   354,   398,   399,   409,   600,  -158,   100,   101,  -241,
     102,   421,  -335,   318,   371,  -336,   407,   608,   611,   416,
     102,   423,   404,   364,   123,   124,   430,   436,   125,   126,
     127,   494,   463,   242,   431,   537,   467,   318,   535,   468,
    -115,   470,   432,   242,   434,   242,   413,   439,   475,   443,
     444,   447,   449,   455,   479,   464,   456,   453,   471,   474,
     490,   473,   142,    70,    55,   354,   503,   493,  -159,   498,
     499,   477,   165,     2,     3,   426,     4,     5,     6,   523,
     242,   525,   528,   527,   478,   533,   542,    13,   544,   548,
     549,   554,    18,   567,    20,   568,    21,    22,   370,   569,
     144,   573,   242,   580,    23,   596,   582,   583,   465,   592,
     587,    27,   181,    28,   166,   597,   604,   211,   212,   213,
     590,    33,   591,    34,    35,   603,    12,   602,    14,    15,
     605,    17,    37,    19,   606,   543,   163,   275,   422,    39,
     538,   226,   227,   228,   547,   609,    24,   610,   237,   229,
     581,   230,   242,   501,   231,    29,    30,    31,   167,   266,
      40,    41,   441,   529,   550,   551,   182,   530,   584,   595,
     502,   570,   524,   403,   561,   559,   560,   565,   566,    43,
     538,   585,    44,   115,   497,   136,   142,    45,    46,   378,
     480,     0,    47,     0,    48,     0,     0,     0,    50,     0,
       0,     0,     0,   561,   559,   560,   565,   566,     0,   594,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   559,   560,   565,   566,    -4,     0,     0,
       1,     2,     3,     0,     4,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,     0,     0,    21,    22,     0,     0,   188,     0,
       0,     0,    23,     0,    24,    25,     0,    26,     0,    27,
       0,    28,     0,    29,    30,    31,     0,     0,    32,    33,
       0,    34,    35,     0,     0,     0,  -337,  -337,  -337,    36,
      37,    38,     0,     0,   196,   197,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     211,   212,   213,     0,   214,   215,   216,     0,    40,    41,
       0,     0,     0,     0,     0,     0,     0,    42,     0,   222,
     223,     0,   224,   225,   226,   227,   228,    43,     0,     0,
      44,     0,   229,     0,   230,    45,    46,   231,     0,     0,
      47,     0,    48,     0,    49,     0,    50,     1,     2,     3,
      -4,     4,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
       0,    21,    22,   143,     0,   144,     0,     0,     0,    23,
     145,    24,    25,     0,    26,   146,    27,     0,    28,   147,
      29,    30,    31,     0,     0,    32,    33,     0,    34,    35,
     211,   212,   213,     0,     0,     0,    36,    37,    38,     0,
     148,     0,     0,     0,    39,     0,     0,     0,     0,     0,
       0,     0,   224,   225,   226,   227,   228,     0,     0,     0,
       0,     0,   229,     0,   230,    40,    41,   231,     0,     0,
       0,     0,     0,     0,   149,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,    44,     0,     0,
       0,     0,    45,    46,     0,     0,     0,    47,     0,    48,
       0,    49,     0,    50,     1,     2,     3,   150,     4,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     0,    21,    22,
     143,     0,   144,     0,     0,     0,    23,   145,    24,    25,
       0,    26,   146,    27,     0,    28,   147,    29,    30,    31,
       0,     0,    32,    33,     0,    34,    35,     0,     0,     0,
       0,     0,     0,    36,    37,    38,     0,   148,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,    41,     0,     0,     0,     0,     0,     0,
       0,   149,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,     0,    44,     0,     0,     0,     0,    45,
      46,     0,     0,     0,    47,     0,    48,     0,    49,     0,
      50,     1,     2,     3,   272,     4,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   143,     0,   144,
       0,     0,     0,    23,   145,    24,    25,     0,    26,   146,
      27,     0,    28,   147,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,   148,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,     0,     0,     0,   149,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,    44,     0,     0,     0,     0,    45,    46,     0,     0,
       0,    47,     0,    48,     0,    49,     0,    50,     1,     2,
       3,   442,     4,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    21,    22,     0,     0,  -337,     0,     0,     0,
      23,     0,    24,    25,     0,    26,     0,    27,     0,    28,
       0,    29,    30,    31,     0,     0,    32,    33,     0,    34,
      35,     0,     0,     0,     0,     0,     0,    36,    37,    38,
       0,     0,  -337,  -337,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   211,   212,
     213,     0,   214,   215,   216,     0,    40,    41,     0,     0,
       0,     0,     0,     0,     0,    42,     0,  -337,  -337,     0,
     224,   225,   226,   227,   228,    43,     0,     0,    44,     0,
     229,     0,   230,    45,    46,   231,     0,     0,    47,     0,
      48,     0,    49,     0,    50,     1,     2,     3,    -3,     4,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    12,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,    23,     0,    24,
      25,     0,    26,     0,    27,     0,    28,     0,    29,    30,
      31,     0,     0,    32,    33,     0,    34,    35,     0,     0,
       0,     0,     0,     0,    36,    37,    38,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,    41,     0,     0,     0,     0,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,    44,     0,     0,     0,     0,
      45,    46,     0,     0,     0,    47,     0,   349,     0,    49,
       0,    50,     1,     2,     3,   -15,     4,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    12,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    21,    22,     0,     0,
       0,     0,     0,     0,    23,     0,    24,    25,     0,    26,
       0,    27,     0,    28,     0,    29,    30,    31,     0,     0,
      32,    33,     0,    34,    35,     0,     0,     0,     0,     0,
       0,    36,    37,    38,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,    41,     0,     0,     0,     0,     0,     0,     0,   348,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,    44,     0,     0,     0,     0,    45,    46,     0,
       0,     0,    47,     0,   349,     0,    49,     0,    50,     1,
       2,     3,   -16,     4,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    12,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    21,    22,     0,     0,     0,     0,     0,
       0,    23,     0,    24,    25,     0,    26,     0,    27,     0,
      28,     0,    29,    30,    31,     0,     0,    32,    33,     0,
      34,    35,     0,     0,     0,     0,     0,     0,    36,    37,
      38,     0,     0,     0,     0,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   557,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,    44,
       0,     0,     0,     0,    45,    46,     0,     0,     0,    47,
       0,   558,     0,    49,     0,    50,     1,     2,     3,  -148,
       4,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      12,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      21,    22,     0,     0,     0,     0,     0,     0,    23,     0,
      24,    25,     0,    26,     0,    27,     0,    28,     0,    29,
      30,    31,     0,     0,    32,    33,     0,    34,    35,     0,
       0,     0,  -137,     0,     0,    36,    37,    38,     0,     0,
       0,     0,     0,    39,  -137,     0,  -137,     0,  -137,  -137,
       0,  -137,     0,  -137,  -137,     0,     0,     0,     0,  -137,
    -137,     0,     0,     0,    40,    41,  -137,     0,     0,     0,
       0,     0,  -137,   557,  -137,  -137,  -137,  -137,     0,     0,
       0,     0,     0,    43,     0,     0,    44,     0,     0,     0,
       0,    45,    46,     0,     0,     0,    47,     0,   558,     0,
      49,     0,    50,     0,    -3,     0,  -149,     1,     2,     3,
       0,     4,     5,     6,     7,     8,     9,    10,     0,    92,
      11,    12,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    21,    22,     0,     0,     0,     0,     0,     0,    23,
       0,    24,    25,     0,    26,     0,    27,     0,    28,     0,
      29,    30,    31,     0,     0,    32,    33,     0,    34,    35,
       0,     0,     0,     0,     0,     0,    36,    37,    38,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    40,    41,     0,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,    44,     0,     0,
       0,     0,    45,    46,     0,     0,     0,    47,     0,    48,
       0,    49,     0,    50,     1,     2,     3,     0,     4,     5,
       6,     7,     8,     9,    10,     0,  -124,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     0,    21,    22,
     143,     0,   144,     0,     0,     0,    23,   145,    24,    25,
       0,    26,   146,    27,     0,    28,   147,    29,    30,    31,
       0,     0,    32,    33,     0,    34,    35,     0,     0,     0,
       0,     0,     0,    36,    37,    38,     0,   148,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,    41,     0,     0,     0,     0,     0,     0,
       0,   149,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,     0,    44,     0,     0,     0,     0,    45,
      46,     0,     0,     0,    47,     0,    48,     0,    49,     0,
      50,     1,     2,     3,     0,     4,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   143,     0,   144,
       0,     0,     0,    23,   145,    24,    25,     0,    26,   146,
      27,     0,    28,   147,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,   148,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   107,     2,     3,     0,     4,     5,     6,     0,    40,
      41,     0,     0,     0,     0,     0,    13,     0,   149,     0,
       0,    18,     0,    20,     0,    21,    22,     0,    43,     0,
       0,    44,     0,    23,     0,     0,    45,    46,     0,     0,
      27,    47,    28,    48,     0,    49,     0,    50,     0,     0,
      33,     0,    34,    35,     0,     0,     0,     0,   107,     2,
       3,    37,     4,     5,     6,     0,     0,     0,    39,     0,
       0,     0,     0,    13,     0,     0,     0,     0,    18,     0,
      20,     0,    21,    22,     0,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    28,
       0,     0,     0,     0,     0,     0,     0,    33,    43,    34,
      35,    44,     0,     0,     0,     0,    45,    46,     0,     0,
       0,    47,     0,    48,     0,    39,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    40,    41,     0,     0,
       0,     0,     0,   188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,     0,
      48,   189,   190,   191,    50,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,   211,   212,   213,     0,   214,
     215,   216,     0,     0,     0,     0,     0,   217,   218,     0,
     219,   220,   221,   188,   222,   223,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,   229,     0,   230,
       0,     0,   231,     0,     0,     0,     0,     0,   419,     0,
       0,   189,   190,   191,     0,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,   211,   212,   213,     0,   214,
     215,   216,     0,     0,     0,     0,     0,   217,   218,     0,
     219,   220,   221,   188,   222,   223,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,   229,     0,   230,
       0,     0,   231,     0,     0,     0,     0,     0,   513,     0,
       0,   189,   190,   191,     0,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,   211,   212,   213,     0,   214,
     215,   216,     0,     0,     0,   188,     0,   217,   218,   408,
     219,   220,   221,     0,   222,   223,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,   229,     0,   230,
       0,     0,   231,   189,   190,   191,     0,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,     0,   211,   212,   213,
       0,   214,   215,   216,     0,     0,     0,   188,     0,   217,
     218,     0,   219,   220,   221,     0,   222,   223,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,   229,
       0,   230,   410,     0,   231,   189,   190,   191,     0,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,     0,   211,
     212,   213,     0,   214,   215,   216,     0,     0,     0,   188,
       0,   217,   218,     0,   219,   220,   221,     0,   222,   223,
       0,   224,   225,   226,   227,   228,     0,     0,     0,     0,
       0,   229,     0,   230,   469,     0,   231,   189,   190,   191,
       0,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
       0,   211,   212,   213,     0,   214,   215,   216,     0,     0,
       0,   188,     0,   217,   218,     0,   219,   220,   221,     0,
     222,   223,     0,   224,   225,   226,   227,   228,     0,     0,
       0,     0,     0,   229,     0,   230,   511,     0,   231,   189,
     190,   191,     0,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,     0,   211,   212,   213,     0,   214,   215,   216,
       0,     0,     0,   188,     0,   217,   218,     0,   219,   220,
     221,     0,   222,   223,     0,   224,   225,   226,   227,   228,
       0,     0,     0,     0,     0,   229,     0,   230,   514,     0,
     231,   189,   190,   191,     0,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,   211,   212,   213,     0,   214,
     215,   216,     0,     0,     0,   188,     0,   217,   218,     0,
     219,   220,   221,     0,   222,   223,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,   229,     0,   230,
     522,     0,   231,   189,   190,   191,     0,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,     0,   211,   212,   213,
       0,   214,   215,   216,     0,     0,     0,   188,     0,   217,
     218,   575,   219,   220,   221,     0,   222,   223,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,   229,
       0,   230,     0,     0,   231,   189,   190,   191,     0,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,     0,   211,
     212,   213,     0,   214,   215,   216,     0,     0,     0,   188,
       0,   217,   218,     0,   219,   220,   221,     0,   222,   223,
       0,   224,   225,   226,   227,   228,     0,     0,     0,     0,
       0,   229,     0,   230,   601,     0,   231,   189,   190,   191,
       0,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
       0,   211,   212,   213,     0,   214,   215,   216,     0,     0,
       0,   188,     0,   217,   218,     0,   219,   220,   221,     0,
     222,   223,     0,   224,   225,   226,   227,   228,     0,     0,
       0,     0,     0,   229,     0,   230,     0,     0,   231,   189,
     190,   191,     0,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,     0,   211,   212,   213,     0,   214,   215,   216,
       0,     0,     0,   188,     0,   217,   218,     0,   219,   220,
     221,     0,     0,   223,     0,   224,   225,   226,   227,   228,
       0,     0,     0,     0,     0,   229,     0,   230,     0,     0,
     231,   189,   190,   191,     0,   192,   193,   194,   195,   196,
     197,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   210,     0,   211,   212,   213,     0,   214,
     215,   216,     0,     0,     0,   188,     0,     0,     0,     0,
     219,   220,   221,     0,   222,   223,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,   229,     0,   230,
       0,     0,   231,   189,   190,   191,     0,   192,   193,   194,
     195,   196,   197,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   211,   212,   213,
       0,   214,   215,   216,     0,     0,     0,   188,     0,     0,
       0,     0,   219,   220,   221,     0,   222,   223,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,   229,
       0,   230,     0,     0,   231,   189,   190,   191,     0,   192,
     193,   194,   195,   196,   197,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   211,
     212,   213,     0,   214,   215,   216,     0,     0,     0,   188,
       0,     0,     0,     0,     0,   220,   221,     0,   222,   223,
       0,   224,   225,   226,   227,   228,     0,     0,     0,     0,
       0,   229,     0,   230,     0,     0,   231,   189,   190,   191,
       0,   192,   193,   194,   195,   196,   197,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   211,   212,   213,     0,   214,   215,   216,     0,     0,
       0,   188,     0,     0,     0,     0,     0,     0,   221,     0,
     222,   223,     0,   224,   225,   226,   227,   228,     0,     0,
       0,     0,     0,   229,     0,   230,     0,     0,   231,   189,
     190,   191,     0,   192,   193,   194,   195,   196,   197,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   211,   212,   213,     0,   214,   215,   216,
       0,     0,     0,   188,     0,     0,     0,     0,     0,     0,
    -337,     0,   222,   223,     0,   224,   225,   226,   227,   228,
       0,     0,     0,     0,     0,   229,     0,   230,     0,     0,
     231,   189,   190,   191,     0,  -337,  -337,  -337,  -337,   196,
     197,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   211,   212,   213,     0,   214,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,   223,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,   229,     0,   230,
       0,     0,   231
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,     0,   106,    18,    59,    49,    66,
     140,    23,   118,   345,   447,    27,     4,   186,     3,     3,
     359,     4,   462,     4,    11,    37,     3,   231,    40,    41,
       4,   365,    44,    45,    46,    47,    48,     0,    93,    26,
      97,     3,    68,   231,   376,   276,    49,     3,    91,    92,
      93,   510,    11,    53,    53,    53,    53,    53,   112,    15,
      16,     0,   101,     0,   571,   101,    15,    26,   122,   509,
     125,   542,    28,   128,   123,    33,     3,   103,   123,   128,
     123,   588,   125,    95,   423,   128,   545,     3,    15,    16,
      53,   145,   146,    26,   151,   134,     3,   431,   134,    32,
     112,    28,   573,    61,   127,   128,   118,   111,    15,    16,
     122,   115,   444,   123,    53,   319,    53,    94,   123,   106,
     123,    28,   291,   229,    34,     3,     4,   257,   140,   110,
     111,   319,    94,   145,   146,   127,    15,   111,   342,   127,
      50,    15,   127,   127,   127,   123,   127,     3,   482,     3,
     103,    25,   105,   127,   342,     0,    30,    31,   490,   127,
     116,    15,    16,   394,   164,   164,   164,   164,   164,    43,
     603,    45,   184,   129,    28,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   430,
     532,   164,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   407,
     127,    15,     3,   277,   278,   164,   238,   164,   416,   398,
     399,    25,   101,   274,   123,    11,     3,    31,   123,   239,
     239,   239,   254,     3,   128,   257,   128,   259,   103,   101,
      17,    45,    19,    20,   241,    22,   362,    24,   133,   105,
       3,   126,   116,   250,   101,   277,   278,     3,     3,   123,
      37,   125,    15,    16,     3,   129,   464,     3,    45,    46,
      47,    48,     3,   471,     3,    28,    15,    16,   127,   390,
      15,   392,   346,    68,    15,    16,    15,    16,     3,    28,
       3,   100,   533,    68,   434,   100,    35,    28,     3,    28,
       3,     3,    17,   134,    19,    20,   373,    22,   101,    24,
     548,   123,   128,   554,   552,   553,   383,   128,   105,   341,
     123,   134,    37,   100,   346,   101,   134,   349,   134,   103,
      45,    46,    47,    48,   133,     3,     3,   575,   449,    53,
     362,   351,   351,   351,    53,     3,     3,    15,    16,   359,
     359,   359,   103,   103,   134,   593,   133,    15,    16,   126,
      28,   133,    90,   116,   361,    90,    90,   605,   609,    90,
      28,   127,   125,     3,     3,     4,   134,   116,     7,     8,
       9,   443,   404,   380,   101,   100,   408,   116,   499,   411,
     128,   413,   133,   390,   105,   392,   125,   103,   420,   100,
     134,   123,    14,   101,   426,    90,   133,   127,    90,   127,
      65,   134,   434,   423,   423,   423,   111,   439,   133,   127,
     101,   133,     3,     4,     5,   103,     7,     8,     9,   133,
     427,   127,   123,   127,   134,   134,   115,    18,   110,   105,
     133,     3,    23,     3,    25,    25,    27,    28,   116,   133,
      31,   115,   449,   127,    35,     3,   127,   133,   116,   134,
     133,    42,     3,    44,    45,   111,   111,    91,    92,    93,
     133,    52,   133,    54,    55,   123,    17,   133,    19,    20,
     127,    22,    63,    24,   133,   507,    53,   164,   351,    70,
     500,   115,   116,   117,   516,   134,    37,   133,    97,   123,
     558,   125,   499,   454,   128,    46,    47,    48,    59,   144,
      91,    92,   382,   488,   519,   519,    71,   488,   563,   582,
     455,   540,   480,   291,   534,   534,   534,   534,   534,   110,
     540,   568,   113,    26,   449,    47,   558,   118,   119,   258,
     427,    -1,   123,    -1,   125,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,   563,   563,   563,   563,   563,    -1,   581,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   582,   582,   582,   582,   582,     0,    -1,    -1,
       3,     4,     5,    -1,     7,     8,     9,    10,    11,    12,
      13,    -1,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    27,    28,    -1,    -1,    39,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,    42,
      -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    67,    68,    69,    62,
      63,    64,    -1,    -1,    75,    76,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    92,    93,    -1,    95,    96,    97,    -1,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   110,
     111,    -1,   113,   114,   115,   116,   117,   110,    -1,    -1,
     113,    -1,   123,    -1,   125,   118,   119,   128,    -1,    -1,
     123,    -1,   125,    -1,   127,    -1,   129,     3,     4,     5,
     133,     7,     8,     9,    10,    11,    12,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    -1,    40,    41,    42,    -1,    44,    45,
      46,    47,    48,    -1,    -1,    51,    52,    -1,    54,    55,
      91,    92,    93,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,    91,    92,   128,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,   123,    -1,   125,
      -1,   127,    -1,   129,     3,     4,     5,   133,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,
      -1,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,    -1,
     129,     3,     4,     5,   133,     7,     8,     9,    10,    11,
      12,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,    -1,   129,     3,     4,
       5,   133,     7,     8,     9,    10,    11,    12,    13,    -1,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    27,    28,    -1,    -1,    39,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    40,    -1,    42,    -1,    44,
      -1,    46,    47,    48,    -1,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    -1,    75,    76,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,
      93,    -1,    95,    96,    97,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   110,   111,    -1,
     113,   114,   115,   116,   117,   110,    -1,    -1,   113,    -1,
     123,    -1,   125,   118,   119,   128,    -1,    -1,   123,    -1,
     125,    -1,   127,    -1,   129,     3,     4,     5,   133,     7,
       8,     9,    10,    11,    12,    13,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    40,    -1,    42,    -1,    44,    -1,    46,    47,
      48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,
      -1,   129,     3,     4,     5,   133,     7,     8,     9,    10,
      11,    12,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,
      -1,    42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,   123,    -1,   125,    -1,   127,    -1,   129,     3,
       4,     5,   133,     7,     8,     9,    10,    11,    12,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    40,    -1,    42,    -1,
      44,    -1,    46,    47,    48,    -1,    -1,    51,    52,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,    -1,   127,    -1,   129,     3,     4,     5,   133,
       7,     8,     9,    10,    11,    12,    13,    -1,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    40,    -1,    42,    -1,    44,    -1,    46,
      47,    48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,     3,    -1,    -1,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    15,    -1,    17,    -1,    19,    20,
      -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,    30,
      31,    -1,    -1,    -1,    91,    92,    37,    -1,    -1,    -1,
      -1,    -1,    43,   100,    45,    46,    47,    48,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,   123,    -1,   125,    -1,
     127,    -1,   129,    -1,     0,    -1,   133,     3,     4,     5,
      -1,     7,     8,     9,    10,    11,    12,    13,    -1,    90,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    40,    -1,    42,    -1,    44,    -1,
      46,    47,    48,    -1,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,   123,    -1,   125,
      -1,   127,    -1,   129,     3,     4,     5,    -1,     7,     8,
       9,    10,    11,    12,    13,    -1,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,
      -1,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,    -1,
     129,     3,     4,     5,    -1,     7,     8,     9,    10,    11,
      12,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,     8,     9,    -1,    91,
      92,    -1,    -1,    -1,    -1,    -1,    18,    -1,   100,    -1,
      -1,    23,    -1,    25,    -1,    27,    28,    -1,   110,    -1,
      -1,   113,    -1,    35,    -1,    -1,   118,   119,    -1,    -1,
      42,   123,    44,   125,    -1,   127,    -1,   129,    -1,    -1,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,     3,     4,
       5,    63,     7,     8,     9,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      25,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,   110,    54,
      55,   113,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,    -1,    70,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    67,    68,    69,   129,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,
     106,   107,   108,    39,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,
     106,   107,   108,    39,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    39,    -1,   103,   104,   105,
     106,   107,   108,    -1,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,    -1,   128,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    39,    -1,   103,
     104,    -1,   106,   107,   108,    -1,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,   126,    -1,   128,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    39,
      -1,   103,   104,    -1,   106,   107,   108,    -1,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,   128,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    96,    97,    -1,    -1,
      -1,    39,    -1,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,    -1,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,   126,    -1,   128,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    -1,    -1,    39,    -1,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,   114,   115,   116,   117,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,
     128,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    39,    -1,   103,   104,    -1,
     106,   107,   108,    -1,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
     126,    -1,   128,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    39,    -1,   103,
     104,   105,   106,   107,   108,    -1,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,    -1,   128,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    39,
      -1,   103,   104,    -1,   106,   107,   108,    -1,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,   128,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    96,    97,    -1,    -1,
      -1,    39,    -1,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,    -1,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,    -1,    -1,   128,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    -1,    -1,    39,    -1,   103,   104,    -1,   106,   107,
     108,    -1,    -1,   111,    -1,   113,   114,   115,   116,   117,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,
     128,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
     106,   107,   108,    -1,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,    -1,   128,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,    -1,   128,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,    -1,   128,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    92,    93,    -1,    95,    96,    97,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
     110,   111,    -1,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,    -1,    -1,   128,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,   110,   111,    -1,   113,   114,   115,   116,   117,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,
     128,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,    -1,   128
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     5,     7,     8,     9,    10,    11,    12,
      13,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    27,    28,    35,    37,    38,    40,    42,    44,    46,
      47,    48,    51,    52,    54,    55,    62,    63,    64,    70,
      91,    92,   100,   110,   113,   118,   119,   123,   125,   127,
     129,   136,   137,   138,   139,   145,   148,   150,   158,   161,
     162,   163,   164,   165,   172,   180,   183,   184,   187,   190,
     191,   192,   193,   197,   199,   208,   213,   214,   215,   226,
     233,   236,   249,   250,   251,   256,   260,   264,   265,   267,
     268,   269,    90,    49,   123,   123,   155,   156,   123,     3,
      15,    16,    28,   127,   185,   186,    15,     3,   260,     3,
     235,   260,   123,     3,   186,   239,   260,   127,   123,   128,
      15,   260,   123,     3,     4,     7,     8,     9,   257,   258,
     259,   260,   260,   260,   260,   260,   255,   260,   245,   246,
     247,   253,   260,    29,    31,    36,    41,    45,    66,   100,
     133,   144,   145,   146,   150,   152,   166,   167,   182,   191,
     252,     3,     0,   139,   127,     3,    45,   152,   159,   160,
     256,   146,   147,    15,    25,    30,    31,    43,    45,   207,
     266,     3,   193,     3,   127,   220,   224,   101,    39,    67,
      68,    69,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    91,    92,    93,    95,    96,    97,   103,   104,   106,
     107,   108,   110,   111,   113,   114,   115,   116,   117,   123,
     125,   128,     3,   123,   254,   255,    11,   147,   173,   189,
     127,   128,   185,   186,   238,   239,   240,   248,   123,   254,
     128,   181,   245,     3,   103,   254,   133,   105,   101,   101,
     134,   126,   101,   253,     3,     3,   153,   154,   254,   254,
     153,     3,   133,   146,   127,   137,     3,   100,    68,   100,
       3,    34,    50,   232,     3,     3,   209,   260,     3,   220,
     225,   227,   228,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,     3,   116,   129,
     185,   262,   263,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   245,   260,
     123,   125,   129,   262,   263,   134,   123,   260,   100,   125,
     140,   141,   142,   148,   150,   197,   199,   208,   233,   188,
     185,   128,   123,   244,     3,    94,   229,   230,   231,   134,
     116,   185,   143,   144,   134,   260,   134,   253,   258,   260,
     105,   243,   101,   144,   133,   243,   254,   254,   103,     3,
      53,   194,    53,   195,     3,   210,   211,   221,   103,   103,
     111,   115,   216,   228,   125,   262,   263,    90,   105,   134,
     126,   261,   260,   125,   262,   263,    90,   155,   254,   134,
     149,   133,   142,   127,   140,   245,   103,   105,   212,   231,
     134,   101,   133,   155,   105,    35,   116,   240,   242,   103,
     151,   154,   133,   100,   134,     3,     4,   123,   240,    14,
     196,   240,   241,   127,   243,   101,   133,     4,   220,     4,
     220,   217,   222,   260,    90,   116,   263,   260,   260,   126,
     260,    90,   263,   134,   127,   260,   140,   133,   134,   260,
     242,   243,    94,   231,    26,    32,   174,   176,   178,   179,
      65,   157,   253,   260,   256,   155,   229,   241,   127,   101,
     200,   151,   211,   111,     4,   110,   111,   127,   218,   223,
     226,   126,   263,   134,   126,   263,    33,    61,   168,   169,
     170,   171,   126,   133,   212,   127,   231,   127,   123,   174,
     176,   155,   134,   134,   198,   240,    45,   100,   191,   204,
     205,   206,   115,   260,   110,   226,   222,   260,   105,   133,
     170,   171,   237,   177,     3,   155,   243,   100,   125,   145,
     150,   191,   201,   202,   203,   208,   233,     3,    25,   133,
     206,   224,   219,   115,   222,   105,   143,   143,   143,   243,
     127,   149,   127,   133,   203,   232,   217,   133,   224,   143,
     133,   133,   134,   234,   260,   201,     3,   111,   217,   175,
     143,   126,   133,   123,   111,   127,   133,   229,   143,   134,
     133,   243
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
#line 2010 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 24:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2022 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 25:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2023 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(2) - (4)].number_int);PASS1 as3_warning("embed command ignored");}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2026 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2027 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 29:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2029 "parser.y"
    {
    (yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));
}
    }
    break;


  
    case 30:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2032 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 44:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2048 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 45:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2049 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 46:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2052 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2059 "parser.y"
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


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2071 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 55:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2072 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2076 "parser.y"
    {
    if((yyvsp[(1) - (1)].code)) {
        if(!global->init) 
            global->init = abc_initscript(global->file);
        code_t**cc = &global->init->method->body->code;
        *cc = code_append(*cc, (yyvsp[(1) - (1)].code));
    }
}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2087 "parser.y"
    {
    PASS_ALWAYS
    (yyval.number_int) = as3_pass;
    as3_pass=0;
}
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2095 "parser.y"
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


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2122 "parser.y"
    {(yyval.node)=(yyvsp[(2) - (2)].node);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2123 "parser.y"
    {(yyval.node)=mkdummynode();}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2125 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2126 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 63:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2128 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2129 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2132 "parser.y"
    {
PASS12
    if(variable_exists((yyvsp[(1) - (3)].id))) 
        syntaxerror("Variable %s already defined", (yyvsp[(1) - (3)].id));
PASS1
    new_variable(state->method, (yyvsp[(1) - (3)].id), (yyvsp[(2) - (3)].classinfo), 1, 0);
PASS2
   
    char slot = 0;
    int index = 0;
    variable_t*v = 0;
    if(state->method->uses_slots) {
        v = find_slot(state->method, (yyvsp[(1) - (3)].id));
        if(v && !v->init) {
            // this variable is stored in a slot
            v->init = 1;
            v->type = (yyvsp[(2) - (3)].classinfo);
            slot = 1;
        }
    }
    if(!v) {
        v = new_variable2(state->method, (yyvsp[(1) - (3)].id), (yyvsp[(2) - (3)].classinfo), 1, 0);
    }

    (yyval.code) = slot?abc_getscopeobject(0, 1):0;
    
    typedcode_t val = node_read((yyvsp[(3) - (3)].node));
    if(!is_subtype_of(val.t, (yyvsp[(2) - (3)].classinfo))) {
        syntaxerror("Can't convert %s to %s", val.t->name, (yyvsp[(2) - (3)].classinfo)->name);
    }
    if((yyvsp[(2) - (3)].classinfo)) {
        if(val.c->prev || val.c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = code_append((yyval.code), val.c);
            (yyval.code) = converttype((yyval.code), val.t, (yyvsp[(2) - (3)].classinfo));
        } else {
            code_free(val.c);
            (yyval.code) = defaultvalue((yyval.code), (yyvsp[(2) - (3)].classinfo));
        }
    } else {
        if(val.c->prev || val.c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = code_append((yyval.code), val.c);
            (yyval.code) = abc_coerce_a((yyval.code));
        } else {
            // don't do anything
            code_free(val.c);
            code_free((yyval.code));
            (yyval.code) = 0;
            break;
        }
    }
    if(slot) {
        (yyval.code) = abc_setslot((yyval.code), v->index);
    } else {
        (yyval.code) = abc_setlocal((yyval.code), v->index);
        v->init = do_init_variable((yyvsp[(1) - (3)].id));
    }
}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2192 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 67:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2192 "parser.y"
    {
    (yyval.code) = var_block((yyvsp[(2) - (2)].code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 68:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2196 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 69:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2197 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2200 "parser.y"
    {
    (yyval.code) = code_new();
    (yyval.code) = code_append((yyval.code), (yyvsp[(3) - (6)].value).c);
    code_t*myjmp,*myif = (yyval.code) = abc_iffalse((yyval.code), 0);
   
    (yyval.code) = code_append((yyval.code), (yyvsp[(5) - (6)].code));
    if((yyvsp[(6) - (6)].code)) {
        myjmp = (yyval.code) = abc_jump((yyval.code), 0);
    }
    myif->branch = (yyval.code) = abc_nop((yyval.code));
    if((yyvsp[(6) - (6)].code)) {
        (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (6)].code));
        myjmp->branch = (yyval.code) = abc_nop((yyval.code));
    }
}
    }
    break;


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2216 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2223 "parser.y"
    {
    PASS1 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable(state->method, (yyvsp[(2) - (3)].id),0,1,0);
    PASS2 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable(state->method, (yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
}
    }
    break;


  
    case 75:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2227 "parser.y"
    {
    PASS12
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 76:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2232 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2233 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 78:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2235 "parser.y"
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

    (yyval.code) = var_block((yyval.code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2255 "parser.y"
    {
    node_t*n = resolve_identifier((yyvsp[(2) - (6)].id));
    typedcode_t w = node_write(n);
    
    int it = alloc_local();
    int array = alloc_local();

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

    (yyval.code) = converttype((yyval.code), 0, w.t);
    (yyval.code) = code_append((yyval.code), w.c);

    (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (6)].code));
    (yyval.code) = abc_jump((yyval.code), loopstart);
    
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (6)].for_start).name, out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (6)].for_start).name, loopstart);
    
    myif->branch = out;

    (yyval.code) = abc_kill((yyval.code), it);
    (yyval.code) = abc_kill((yyval.code), array);

    (yyval.code) = var_block((yyval.code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 80:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2299 "parser.y"
    {

    (yyval.code) = code_new();

    code_t*myjmp = (yyval.code) = abc_jump((yyval.code), 0);
    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(5) - (5)].code));
    code_t*cont = (yyval.code) = abc_nop((yyval.code));
    myjmp->branch = cont;
    (yyval.code) = code_append((yyval.code), (yyvsp[(3) - (5)].value).c);
    (yyval.code) = abc_iftrue((yyval.code), loopstart);
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (5)].id), out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (5)].id), cont);
}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2315 "parser.y"
    {
    (yyval.code) = code_new();
    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(2) - (6)].code));
    code_t*cont = (yyval.code) = abc_nop((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(5) - (6)].value).c);
    (yyval.code) = abc_iftrue((yyval.code), loopstart);
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (6)].id), out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (6)].id), cont);
}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2327 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2330 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2333 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2336 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2340 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2341 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2342 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2343 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2344 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 91:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2345 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2347 "parser.y"
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


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2358 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 94:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2361 "parser.y"
    {PASS12 new_state();state->switch_var=alloc_local();}
    }
    break;


  
    case 95:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2361 "parser.y"
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
   
    (yyval.code) = var_block((yyval.code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 96:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2392 "parser.y"
    {PASS12 new_state();
                                                      state->exception_name=(yyvsp[(3) - (5)].id);
                                               PASS1 new_variable(state->method, (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0, 0);
                                               PASS2 new_variable(state->method, (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0, 0);
                                              }
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2397 "parser.y"
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

    c = var_block(c, state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 98:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2417 "parser.y"
    {PASS12 new_state();state->exception_name=0;}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2417 "parser.y"
    {
    (yyvsp[(4) - (5)].code) = var_block((yyvsp[(4) - (5)].code), state->vars);
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


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2433 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2434 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 102:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2435 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 103:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2436 "parser.y"
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


  
    case 104:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2444 "parser.y"
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


  
    case 105:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2453 "parser.y"
    {PASS12 new_state();
                 state->method->has_exceptions=1;
                 state->method->late_binding=1;//for invariant scope_code
                }
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2456 "parser.y"
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
        tmp = alloc_local();
    
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
   
    (yyval.code) = var_block((yyval.code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 107:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2502 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 108:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2506 "parser.y"
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


  
    case 109:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2517 "parser.y"
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


  
    case 110:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2527 "parser.y"
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


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2545 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2546 "parser.y"
    {PASS12 (yyval.id)="namespace";}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2547 "parser.y"
    {PASS12 (yyval.id)="NaN";}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2549 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2550 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 117:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2552 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 118:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2553 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2554 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2555 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2578 "parser.y"
    {
       PASS12
       slotinfo_t*s = registry_find(state->package, (yyvsp[(2) - (2)].id));
       if(!s && as3_pass==1) {as3_schedule_class(state->package, (yyvsp[(2) - (2)].id));}
       state_has_imports();
       dict_put(state->imports, state->package, (yyvsp[(2) - (2)].id));
       (yyval.code)=0;
}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2586 "parser.y"
    {
       PASS12
       slotinfo_t*s = registry_find((yyvsp[(2) - (2)].classinfo)->package, (yyvsp[(2) - (2)].classinfo)->name);
       if(!s && as3_pass==1) {
           as3_schedule_class((yyvsp[(2) - (2)].classinfo)->package, (yyvsp[(2) - (2)].classinfo)->name);
       }
       /*if(s && s->kind == INFOTYPE_VAR && TYPE_IS_NAMESPACE(s->type)) {
	    trie_put(active_namespaces, (unsigned char*)$2->name, 0);
       }*/
       state_has_imports();
       dict_put(state->imports, (yyvsp[(2) - (2)].classinfo)->name, (yyvsp[(2) - (2)].classinfo));
       import_toplevel((yyvsp[(2) - (2)].classinfo)->package);
       (yyval.code)=0;
}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2600 "parser.y"
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


  
    case 124:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2616 "parser.y"
    {PASS12 (yyval.flags).flags=0;(yyval.flags).ns=0;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2617 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2618 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2619 "parser.y"
    {
    PASS12 
    (yyval.flags).flags=(yyvsp[(1) - (2)].flags).flags|(yyvsp[(2) - (2)].flags).flags;
    if((yyvsp[(1) - (2)].flags).ns && (yyvsp[(2) - (2)].flags).ns) syntaxerror("only one namespace allowed in one declaration");
    (yyval.flags).ns=(yyvsp[(1) - (2)].flags).ns?(yyvsp[(1) - (2)].flags).ns:(yyvsp[(2) - (2)].flags).ns;

}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2626 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PUBLIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2627 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PRIVATE;(yyval.flags).ns=0;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2628 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PROTECTED;(yyval.flags).ns=0;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2629 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_STATIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2630 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_DYNAMIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2631 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_FINAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2632 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_OVERRIDE;(yyval.flags).ns=0;}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2633 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NATIVE;(yyval.flags).ns=0;}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2634 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PACKAGEINTERNAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2635 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NAMESPACE;
                               (yyval.flags).ns=(yyvsp[(1) - (1)].id);
                       }
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2639 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2640 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2642 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2643 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 142:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2645 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 143:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2646 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 144:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2650 "parser.y"
    {PASS12 startclass(&(yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 145:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2652 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 146:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2656 "parser.y"
    {PASS12 (yyvsp[(1) - (5)].flags).flags|=FLAG_INTERFACE;
                                          startclass(&(yyvsp[(1) - (5)].flags),(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 147:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2659 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 153:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2668 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 156:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2671 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(2) - (4)].number_int);PASS1 as3_warning("embed command ignored");}
    }
    break;


  
    case 157:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2673 "parser.y"
    {
    code_t*c = state->cls->static_init->header;
    c = code_append(c, (yyvsp[(1) - (1)].code));  
    state->cls->static_init->header = c;
}
    }
    break;


  
    case 163:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2684 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2687 "parser.y"
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


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2762 "parser.y"
    {PASS12 setslotstate(&(yyvsp[(1) - (2)].flags),(yyvsp[(2) - (2)].token));}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2762 "parser.y"
    {PASS12 (yyval.code)=(yyvsp[(4) - (4)].code);setslotstate(0, 0);}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2764 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2765 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2768 "parser.y"
    {
PASS12
    int flags = slotstate_flags->flags;
    namespace_t ns = modifiers2access(slotstate_flags);

    if(as3_pass == 1) {

        varinfo_t* info = 0;
        if(state->cls) {
            memberinfo_t*i = registry_findmember(state->cls->info, ns.name, (yyvsp[(1) - (3)].id), 1, slotstate_flags->flags&FLAG_STATIC);
            if(i) {
                check_override(i, flags);
            }
            info = varinfo_register_onclass(state->cls->info, ns.access, ns.name, (yyvsp[(1) - (3)].id), slotstate_flags->flags&FLAG_STATIC);
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


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2851 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2852 "parser.y"
    {
  (yyval.constant) = malloc(sizeof(constant_t));
  *(yyval.constant) = node_eval((yyvsp[(2) - (2)].node));
  if((yyval.constant)->type == CONSTANT_UNKNOWN) {
    syntaxerror("can't evaluate default parameter value (needs to be a compile-time constant)");
  }
}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2860 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2861 "parser.y"
    {
    (yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));
}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2864 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2865 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 178:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2866 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2867 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2868 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2869 "parser.y"
    {(yyval.constant) = constant_new_undefined((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2870 "parser.y"
    {(yyval.constant) = constant_new_float(__builtin_nan(""));}
    }
    break;


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2878 "parser.y"
    {
   multiname_t m = {QNAME, &stdns, 0, "XML"};
   typedcode_t v;
   v.c = 0;
   v.c = abc_getlex2(v.c, &m);
   v.c = code_append(v.c, node_read((yyvsp[(1) - (1)].node)).c);
   v.c = abc_construct(v.c, 1);
   v.t = TYPE_XML;
   (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 184:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2889 "parser.y"
    {PASS_ALWAYS if(!xml_level++) tokenizer_begin_xml();}
    }
    break;


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2890 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 186:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2891 "parser.y"
    {PASS_ALWAYS if(!--xml_level) tokenizer_end_xml(); else tokenizer_begin_xmltext();}
    }
    break;


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2893 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2893 "parser.y"
    {
    (yyval.node) = (yyvsp[(2) - (4)].node);
}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2896 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xml();}
    }
    break;


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2896 "parser.y"
    {
    (yyval.node) = (yyvsp[(2) - (4)].node);
}
    }
    break;


  
    case 191:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2899 "parser.y"
    {(yyval.node)=mkstringnode("");}
    }
    break;


  
    case 192:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2900 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
}
    }
    break;


  
    case 193:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2903 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(2) - (2)].str));
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),mkstringnode(str));
    free(str);
}
    }
    break;


  
    case 194:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2908 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node), mkstringnode(">"));
}
    }
    break;


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2911 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2914 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkaddnode((yyvsp[(2) - (3)].node),(yyvsp[(3) - (3)].node)));
}
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2917 "parser.y"
    {
    (yyval.node) = mkstringnode((yyvsp[(1) - (1)].id));
}
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2920 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2924 "parser.y"
    {
    (yyval.node) = mkstringnode("");
}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2927 "parser.y"
    {
    (yyval.node) = mkaddnode(mkstringnode(" "),(yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2931 "parser.y"
    {
    //$$ = allocprintf("<%s%s/>", $2, $3, $5, $8);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkstringnode("<"),(yyvsp[(2) - (6)].node)),(yyvsp[(3) - (6)].node)),mkstringnode("/>"));
}
    }
    break;


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2935 "parser.y"
    {
    //$$ = allocprintf("<%s%s>%s</%s>", $2, $3, $5, $8);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(
         mkstringnode("<"),(yyvsp[(2) - (10)].node)),(yyvsp[(3) - (10)].node)),mkstringnode(">")),(yyvsp[(5) - (10)].node)),mkstringnode("</")),(yyvsp[(8) - (10)].node)),mkstringnode(">"));
}
    }
    break;


  
    case 203:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2940 "parser.y"
    {
    //$$ = allocprintf("<%s%s>%s%s</%s>", $2, $3, $5, $6, $9);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(
         mkstringnode("<"),(yyvsp[(2) - (11)].node)),(yyvsp[(3) - (11)].node)),mkstringnode(">")),(yyvsp[(5) - (11)].node)),(yyvsp[(6) - (11)].node)),mkstringnode("</")),(yyvsp[(9) - (11)].node)),mkstringnode(">"));
}
    }
    break;


  
    case 204:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2946 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 205:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2949 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node), mkaddnode(mkstringnode(" "),(yyvsp[(2) - (2)].node)));
}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2952 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2955 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(3) - (3)].str));
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkstringnode(concat2("=",str)));
    free(str);
}
    }
    break;


  
    case 208:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2960 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkaddnode(mkstringnode("=\""), mkaddnode((yyvsp[(3) - (3)].node), mkstringnode("\""))));
}
    }
    break;


  
    case 209:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2963 "parser.y"
    {
    (yyval.node) = mkaddnode(mkaddnode(mkstringnode(concat2((yyvsp[(1) - (3)].id),"=\"")), (yyvsp[(3) - (3)].node)), mkstringnode("\""));
}
    }
    break;


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2966 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(3) - (3)].str));
    (yyval.node)=mkstringnode(allocprintf("%s=%s", (yyvsp[(1) - (3)].id),str));
    free(str);
    free((yyvsp[(1) - (3)].id));free((char*)(yyvsp[(3) - (3)].str).str);
}
    }
    break;


  
    case 211:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2976 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2980 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 213:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2986 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(2) - (2)].param));
}
    }
    break;


  
    case 214:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2992 "parser.y"
    {
    PASS12
    (yyval.params) =(yyvsp[(1) - (4)].params);
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(4) - (4)].param));
}
    }
    break;


  
    case 215:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3000 "parser.y"
    {
    PASS12
    (yyval.params) = (yyvsp[(1) - (3)].params);
    list_append((yyval.params).list, (yyvsp[(3) - (3)].param));
}
    }
    break;


  
    case 216:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3005 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    list_append((yyval.params).list, (yyvsp[(1) - (1)].param));
}
    }
    break;


  
    case 217:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3011 "parser.y"
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


  
    case 218:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3019 "parser.y"
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


  
    case 221:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3029 "parser.y"
    {PASS12 (yyval.token)=0;}
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3032 "parser.y"
    {PASS12 startfunction(&(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3033 "parser.y"
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


  
    case 225:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3049 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3051 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 227:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3052 "parser.y"
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


  
    case 228:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3074 "parser.y"
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
    registry_use(s);
}
    }
    break;


  
    case 229:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3094 "parser.y"
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
    registry_use(s);
}
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3112 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3113 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3115 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3116 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_ANY;}
    }
    break;


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3117 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_VOID;}
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3126 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3127 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3131 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3132 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3134 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3138 "parser.y"
    {(yyval.value_list).number=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3142 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3143 "parser.y"
    {
                                                  (yyval.value_list).number= (yyvsp[(1) - (2)].value_list).number+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3149 "parser.y"
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
    } else if(is_getlocal((yyval.value).c)) {
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_construct((yyval.value).c, (yyvsp[(4) - (4)].value_list).number);
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


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3196 "parser.y"
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
        (yyval.value).t = TYPE_ANY;
        (yyval.value).c = abc_coerce_a((yyval.value).c);
    }
}
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3246 "parser.y"
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


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3267 "parser.y"
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


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3289 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3292 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3299 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3302 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3305 "parser.y"
    {
    (yyval.node) = mkmultinode(&node_comma, (yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3308 "parser.y"
    {
    (yyval.node) = multinode_extend((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
}
    }
    break;


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3311 "parser.y"
    { 
    (yyval.code) = node_exec((yyvsp[(1) - (1)].node)); 
}
    }
    break;


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3314 "parser.y"
    { 
    (yyval.code) = (yyvsp[(1) - (3)].code);
    (yyval.code) = code_append((yyval.code), node_exec((yyvsp[(3) - (3)].node))); 
}
    }
    break;


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3319 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3320 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3322 "parser.y"
    {(yyval.code)=abc_pushstring(0,(yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3323 "parser.y"
    {(yyval.code)=abc_pushstring2(0,&(yyvsp[(1) - (1)].str));}
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3324 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3325 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3326 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3328 "parser.y"
    {
    (yyval.value_list).cc = 0;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(1) - (3)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (3)].value).c);
    (yyval.value_list).number = 2;
}
    }
    break;


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3334 "parser.y"
    {
    (yyval.value_list).cc = (yyvsp[(1) - (5)].value_list).cc;
    (yyval.value_list).number = (yyvsp[(1) - (5)].value_list).number+2;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (5)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(5) - (5)].value).c);
}
    }
    break;


  
    case 269:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3343 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3344 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3345 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3346 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3347 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3348 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    }
    break;


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3350 "parser.y"
    { 
    (yyval.node) = mkconstnode((yyvsp[(1) - (1)].constant));
}
    }
    break;


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3354 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3359 "parser.y"
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


  
    case 278:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3377 "parser.y"
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


  
    case 279:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3388 "parser.y"
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


  
    case 280:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3398 "parser.y"
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


  
    case 281:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3407 "parser.y"
    {(yyval.node) = mknode2(&node_lt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3408 "parser.y"
    {(yyval.node) = mknode2(&node_gt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3409 "parser.y"
    {(yyval.node) = mknode2(&node_le,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3410 "parser.y"
    {(yyval.node) = mknode2(&node_ge,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3411 "parser.y"
    {(yyval.node) = mknode2(&node_eqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 286:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3412 "parser.y"
    {(yyval.node) = mknode2(&node_eqeqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 287:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3413 "parser.y"
    {(yyval.node) = mknode2(&node_noteqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3414 "parser.y"
    {(yyval.node) = mknode2(&node_noteq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 289:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3415 "parser.y"
    {(yyval.node) = mknode2(&node_oror,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 290:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3416 "parser.y"
    {(yyval.node) = mknode2(&node_andand,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 291:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3417 "parser.y"
    {(yyval.node) = mknode1(&node_not, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 292:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3418 "parser.y"
    {(yyval.node) = mknode1(&node_bitnot, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 293:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3419 "parser.y"
    {(yyval.node) = mknode2(&node_bitand, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 294:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3420 "parser.y"
    {(yyval.node) = mknode2(&node_bitxor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 295:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3421 "parser.y"
    {(yyval.node) = mknode2(&node_bitor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 296:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3422 "parser.y"
    {(yyval.node) = mknode2(&node_shr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 297:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3423 "parser.y"
    {(yyval.node) = mknode2(&node_ushr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 298:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3424 "parser.y"
    {(yyval.node) = mknode2(&node_shl, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 299:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3425 "parser.y"
    {(yyval.node) = mknode2(&node_div, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 300:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3426 "parser.y"
    {(yyval.node) = mknode2(&node_mod, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 301:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3427 "parser.y"
    {(yyval.node) = mknode2(&node_plus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 302:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3428 "parser.y"
    {(yyval.node) = mknode2(&node_minus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 303:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3429 "parser.y"
    {(yyval.node) = mknode2(&node_multiply, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 304:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3430 "parser.y"
    {(yyval.node) = mknode2(&node_in, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 305:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3431 "parser.y"
    {(yyval.node) = mknode2(&node_as, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 306:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3432 "parser.y"
    {(yyval.node) = mknode2(&node_instanceof, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 307:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3433 "parser.y"
    {(yyval.node) = mknode2(&node_is, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 308:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3434 "parser.y"
    {(yyval.node) = mknode1(&node_typeof, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 309:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3435 "parser.y"
    {(yyval.node) = mknode1(&node_void, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 310:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3436 "parser.y"
    { (yyval.node) = mkconstnode(constant_new_undefined());}
    }
    break;


  
    case 311:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3437 "parser.y"
    { (yyval.node)=(yyvsp[(2) - (3)].node);}
    }
    break;


  
    case 312:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3438 "parser.y"
    {(yyval.node) = mknode1(&node_neg, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 313:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3439 "parser.y"
    {(yyval.node) = mknode2(&node_arraylookup, (yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node));}
    }
    break;


  
    case 314:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3440 "parser.y"
    {(yyval.node) = mknode2(&node_muleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 315:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3441 "parser.y"
    {(yyval.node) = mknode2(&node_modeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 316:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3442 "parser.y"
    {(yyval.node) = mknode2(&node_shleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 317:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3443 "parser.y"
    {(yyval.node) = mknode2(&node_shreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 318:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3444 "parser.y"
    {(yyval.node) = mknode2(&node_ushreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 319:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3445 "parser.y"
    { (yyval.node) = mknode2(&node_diveq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 320:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3446 "parser.y"
    { (yyval.node) = mknode2(&node_bitoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 321:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3447 "parser.y"
    { (yyval.node) = mknode2(&node_bitxoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 322:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3448 "parser.y"
    { (yyval.node) = mknode2(&node_bitandeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 323:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3449 "parser.y"
    { (yyval.node) = mknode2(&node_pluseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 324:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3450 "parser.y"
    { (yyval.node) = mknode2(&node_minuseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 325:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3451 "parser.y"
    { (yyval.node) = mknode2(&node_assign, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 326:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3452 "parser.y"
    { (yyval.node) = mknode3(&node_tenary, (yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node));}
    }
    break;


  
    case 327:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3454 "parser.y"
    { (yyval.node) = mknode1(&node_rplusplus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 328:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3455 "parser.y"
    { (yyval.node) = mknode1(&node_rminusminus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 329:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3456 "parser.y"
    {(yyval.node) = mknode1(&node_lplusplus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 330:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3457 "parser.y"
    {(yyval.node) = mknode1(&node_lminusminus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 331:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3460 "parser.y"
    { if(!state->cls->info)
                  syntaxerror("super keyword not allowed outside a class");
              classinfo_t*t = state->cls->info->superclass;
              if(!t) t = TYPE_OBJECT;
              memberinfo_t*f = findmember_nsset(t, (yyvsp[(3) - (3)].id), 1, 0);
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


  
    case 332:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3474 "parser.y"
    {
    typedcode_t v;
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(2) - (2)].id)};
    v.c = abc_getlex2(0, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 333:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3482 "parser.y"
    {PASS12 new_state();state->xmlfilter=1;}
    }
    break;


  
    case 334:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3482 "parser.y"
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
    
    c = var_block(c, state->vars);
    old_state();
    typedcode_t r;
    r.c = c;
    r.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(r);
}
    }
    break;


  
    case 335:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3536 "parser.y"
    {(yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 336:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3537 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 338:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3539 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 339:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3579 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].id), 0, 0);
}
    }
    break;


  
    case 340:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3582 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (3)].node));
    multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 341:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3589 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].id), 1, 0);
}
    }
    break;


  
    case 342:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3592 "parser.y"
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


  
    case 343:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3603 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getproperty2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 344:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3611 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (6)].node), (yyvsp[(4) - (6)].id), (yyvsp[(6) - (6)].id), 0, 1);
}
    }
    break;


  
    case 345:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3615 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 346:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3622 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (6)].node), (yyvsp[(4) - (6)].id), (yyvsp[(6) - (6)].id), 1, 1);
}
    }
    break;


  
    case 347:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3626 "parser.y"
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


  
    case 348:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3636 "parser.y"
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


  
    case 349:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3647 "parser.y"
    {
    typedcode_t v1 = node_read((yyvsp[(1) - (3)].node));
    (yyval.value).c = v1.c;
    classinfo_t*t = v1.t;
    char is_static = 0;
    if(TYPE_IS_CLASS(t) && t->data) {
        t = t->data;
        is_static = 1;
    }
    if(TYPE_IS_XML(t) && !findmember_nsset(t, (yyvsp[(3) - (3)].id), 1, is_static)) {
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
        (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_XMLLIST;
    } else if(t) {
        if(t->subtype==INFOTYPE_UNRESOLVED) {
            syntaxerror("syntaxerror: trying to resolve property '%s' on incomplete object '%s'", (yyvsp[(3) - (3)].id), t->name);
        }
        memberinfo_t*f = findmember_nsset(t, (yyvsp[(3) - (3)].id), 1, is_static);
        char noslot = 0;
        if(f && !is_static != !(f->flags&FLAG_STATIC))
           noslot=1;
        if(f && f->slot && !noslot) {
            (yyval.value).c = abc_getslot((yyval.value).c, f->slot);
        } else {
            if(!f) {
                if(!TYPE_IS_XMLLIST(t)) {
                    as3_softwarning("Access of undefined property '%s' in %s", (yyvsp[(3) - (3)].id), t->name);
                }
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


  
    case 350:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3844 "parser.y"
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
        unknown_variable((yyvsp[(1) - (1)].id));
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


  
    case 351:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3878 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (2)].id);
    n->url = (yyvsp[(2) - (2)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 352:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3885 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 353:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3892 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].str).str;
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 354:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3899 "parser.y"
    {
    PASS12
    dict_put(state->namespaces, (unsigned char*)(yyvsp[(2) - (2)].namespace_decl)->name, (void*)(yyvsp[(2) - (2)].namespace_decl)->url);

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


  
    case 355:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3922 "parser.y"
    {
    (yyval.code) = 0;
    (yyval.code) = code_append((yyval.code), node_read((yyvsp[(4) - (4)].node)).c);
    (yyval.code) = abc_dxnslate((yyval.code));
}
    }
    break;


  
    case 356:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3928 "parser.y"
    {
    PASS12

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

    const char*url = s->value->ns->name;
    dict_put(state->namespaces, (unsigned char*)(yyvsp[(3) - (3)].classinfo)->name, (void*)url);
    add_active_url(url);
    (yyval.code)=0;
}
    }
    break;




/* Line 1464 of skeleton.m4  */
#line 9011 "parser.tab.c"
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



