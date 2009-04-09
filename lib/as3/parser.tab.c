
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
     KW_DELETE = 317,
     KW_IF = 318,
     KW_ELSE = 319,
     KW_BREAK = 320,
     KW_IS = 321,
     KW_IN = 322,
     KW_AS = 323,
     T_DICTSTART = 324,
     T_EQEQ = 325,
     T_EQEQEQ = 326,
     T_NE = 327,
     T_NEE = 328,
     T_LE = 329,
     T_GE = 330,
     T_ORBY = 331,
     T_DIVBY = 332,
     T_MODBY = 333,
     T_MULBY = 334,
     T_ANDBY = 335,
     T_PLUSBY = 336,
     T_MINUSBY = 337,
     T_XORBY = 338,
     T_SHRBY = 339,
     T_SHLBY = 340,
     T_USHRBY = 341,
     T_OROR = 342,
     T_ANDAND = 343,
     T_COLONCOLON = 344,
     T_MINUSMINUS = 345,
     T_PLUSPLUS = 346,
     T_DOTDOT = 347,
     T_DOTDOTDOT = 348,
     T_SHL = 349,
     T_USHR = 350,
     T_SHR = 351,
     prec_none = 352,
     below_semicolon = 353,
     below_assignment = 354,
     below_lt = 355,
     below_minus = 356,
     minusminus_prefix = 357,
     plusplus_prefix = 358,
     below_curly = 359,
     new2 = 360,
     above_identifier = 361,
     below_else = 362,
     above_function = 363
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
#line 283 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 273 "parser.y"


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
    int variable_count;

    dict_t*unresolved_variables;

    char inner;
    char uses_parent_function;
    int uses_slots;
    dict_t*slots;
    int activation_var;

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
static void function_initvars(methodstate_t*m, params_t*params, int flags, char var0);


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
        function_initvars(state->method, 0, 0, 1);
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
        if(v)
            return v;
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
        if(!m->activation_var)
            m->activation_var = alloc_local();
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
            variable_t*v = new_variable2(p->param->name, p->param->type, 0, 1);
            v->is_parameter = 1;
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
    
    if(as3_pass==2) {
        m->scope_code = add_scope_code(m->scope_code, m, 0);
    }
    
    if(as3_pass==2 && m->slots) {
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
        state->cls->static_init->variable_count=1;
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
       
        function_initvars(state->cls->init, 0, 0, 1);
        function_initvars(state->cls->static_init, 0, 0, 0);

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

        abc_class_protectedNS(state->cls->abc, classname);

        for(mlist=implements;mlist;mlist=mlist->next) {
            MULTINAME(m, mlist->classinfo);
            abc_class_add_interface(state->cls->abc, &m);
        }

        NEW(parsedclass_t,p);
        p->cls = state->cls->info;
        p->abc = state->cls->abc;
        list_append(global->classes, p);

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
    state->allvars = dict_new();

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
        
        function_initvars(state->method, params, mod->flags, 1);
    } 
}

static abc_method_t* endfunction(modifiers_t*mod, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type, code_t*body)
{
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
                        variable_t*v;
                        if((v=find_variable(state, l->key))) {
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
                if(!name) syntaxerror("internal error");
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
          
            /* deal with protected */
            if(ns.access == ACCESS_PROTECTED && state->cls)
                ns.name = state->cls->info->name;

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

#define IS_NUMBER_OR_INT(a) (TYPE_IS_INT((a)) || TYPE_IS_UINT((a)) || TYPE_IS_NUMBER((a)))

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
    } else {
        classinfo_t*c = (classinfo_t*)a;
        if(c->slot) {
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
#line 1876 "parser.tab.c"
/* Unqualified %code blocks.  */

/* Line 274 of skeleton.m4  */
#line 1952 "parser.y"

    char is_subtype_of(classinfo_t*type, classinfo_t*supertype)
    {
        return 1; // FIXME
    }


/* Line 274 of skeleton.m4  */
#line 2401 "parser.y"

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
#line 2534 "parser.y"

    static int slotstate_varconst = 0;
    static modifiers_t*slotstate_flags = 0;
    static void setslotstate(modifiers_t* flags, int varconst)
    {
        slotstate_varconst = varconst;
        slotstate_flags = flags;
        if(state->cls) {
            if(flags && flags->flags&FLAG_STATIC) {
                state->method = state->cls->static_init;
            } else {
                state->method = state->cls->init;
            }
        } else {
            parserassert(state->method);
        }
    }
    static trait_t* add_abc_slot(modifiers_t* modifiers, const char*name, multiname_t*m, code_t***c)
    {
        int flags = modifiers->flags;
        namespace_t ns = modifiers2access(modifiers);
        /* deal with protected */
        if(ns.access == ACCESS_PROTECTED && state->cls)
            ns.name = state->cls->info->name;

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
            memcpy(m, &mname, sizeof(multiname_t));
            
        return trait_new_member(traits, 0, multiname_clone(&mname), 0);
    }


/* Line 274 of skeleton.m4  */
#line 2712 "parser.y"

    static int xml_level = 0;


/* Line 274 of skeleton.m4  */
#line 3390 "parser.y"

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

        int i_am_static = (state->method && state->method->info)?(state->method->info->flags&FLAG_STATIC):FLAG_STATIC;

        /* look at current class' members */
        if(!state->method->inner && 
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
                    namespace_t ns = {f->access, f->package};
                    multiname_t m = {QNAME, &ns, 0, name};
                    o.c = abc_getlocal_0(o.c);
                    o.c = abc_getproperty2(o.c, &m);
                    return mkcodenode(o);
                }
            }
        } 
        
        /* look at actual classes, in the current package and imported */
        if((a = find_class(name))) {
            o = push_class(a);
            return mkcodenode(o);
        }

        /* look through package prefixes */
        if(dict_contains(state->import_toplevel_packages, name) || 
           registry_ispackage(name)) {
            o.c = abc___pushpackage__(o.c, name);
            o.t = 0;
            return mkcodenode(o); //?
        }

        /* unknown object, let the avm2 resolve it */
        if(1) {
            //as3_softwarning("Couldn't resolve '%s', doing late binding", name);
            as3_warning("Couldn't resolve '%s', doing late binding", name);
            state->method->late_binding = 1;
                    
            multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, name};

            o.t = 0;
            o.c = abc_findpropstrict2(o.c, &m);
            o.c = abc_getproperty2(o.c, &m);
            return mkcodenode(o);
        }
    }


/* Line 274 of skeleton.m4  */
#line 3523 "parser.y"

    void add_active_url(const char*url)
    {
        NEW(namespace_t,n);
        n->name = url;
        list_append(state->active_namespace_urls, n);
    }



/* Line 274 of skeleton.m4  */
#line 2094 "parser.tab.c"

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
#define YYFINAL  153
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3311

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  128
/* YYNRULES -- Number of rules.  */
#define YYNRULES  331
/* YYNRULES -- Number of states.  */
#define YYNSTATES  570

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   363

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   118,     2,     2,     2,   116,   107,     2,
     122,   133,   115,   113,   100,   112,   127,   114,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   104,    99,
     109,   102,   110,   103,   128,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   124,     2,   125,   106,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   126,   105,   132,   117,     2,     2,     2,
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
      95,    96,    97,    98,   101,   108,   111,   119,   120,   121,
     123,   129,   130,   131
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
      85,    87,    89,    93,    96,    98,   100,   102,   104,   106,
     108,   110,   115,   118,   120,   122,   126,   129,   130,   133,
     136,   138,   142,   146,   147,   150,   151,   159,   160,   162,
     164,   168,   170,   173,   177,   186,   193,   194,   201,   202,
     210,   212,   215,   217,   220,   221,   223,   225,   228,   230,
     233,   238,   242,   243,   252,   253,   263,   264,   270,   272,
     275,   277,   280,   282,   283,   290,   293,   295,   300,   303,
     305,   307,   309,   313,   315,   316,   323,   324,   330,   333,
     338,   339,   341,   343,   346,   348,   350,   352,   354,   356,
     358,   360,   362,   364,   366,   367,   370,   371,   374,   375,
     378,   379,   389,   390,   399,   400,   402,   404,   407,   409,
     414,   416,   418,   420,   421,   423,   425,   428,   430,   433,
     442,   444,   446,   447,   452,   454,   458,   462,   463,   466,
     468,   470,   472,   474,   476,   478,   480,   482,   484,   486,
     488,   490,   491,   492,   495,   498,   501,   505,   516,   523,
     535,   536,   538,   540,   543,   547,   548,   550,   553,   558,
     562,   564,   569,   572,   574,   576,   577,   578,   591,   593,
     594,   595,   606,   608,   612,   614,   616,   618,   622,   624,
     626,   628,   631,   632,   633,   637,   638,   640,   642,   644,
     647,   650,   651,   656,   661,   666,   669,   671,   674,   676,
     678,   680,   684,   686,   690,   691,   693,   695,   697,   701,
     707,   709,   711,   713,   715,   717,   719,   721,   723,   725,
     729,   733,   737,   741,   745,   749,   753,   757,   761,   765,
     769,   773,   776,   779,   783,   787,   791,   795,   799,   803,
     807,   811,   815,   819,   823,   827,   831,   835,   839,   844,
     847,   849,   853,   856,   861,   865,   869,   873,   877,   881,
     885,   889,   893,   897,   901,   905,   909,   915,   918,   921,
     924,   927,   931,   934,   940,   942,   944,   946,   948,   954,
     958,   964,   969,   974,   981,   988,   992,   994,   997,  1002,
    1007,  1010
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     135,     0,    -1,   136,    -1,    -1,   137,    -1,   138,    -1,
     137,   138,    -1,   186,    -1,   198,    -1,   196,    -1,   227,
      -1,   207,    -1,   147,    -1,   148,   126,   136,   132,    -1,
      99,    -1,    -1,   140,    -1,   141,    -1,   140,   141,    -1,
     198,    -1,   196,    -1,   227,    -1,   207,    -1,   147,    -1,
     148,   126,   139,   132,    -1,    99,    -1,   143,    -1,    -1,
     143,   145,    -1,   145,    -1,   189,    -1,   159,    -1,   160,
      -1,   161,    -1,   163,    -1,   171,    -1,   154,    -1,   183,
      -1,   179,    -1,   250,    -1,   261,    -1,   260,    -1,   126,
     143,   132,    -1,   126,   132,    -1,    99,    -1,   144,    -1,
     150,    -1,   165,    -1,   166,    -1,   246,    -1,   181,    -1,
     148,   126,   143,   132,    -1,   145,    99,    -1,   145,    -1,
     144,    -1,     3,    89,     3,    -1,   102,   254,    -1,    -1,
      45,   151,    -1,    31,   151,    -1,   152,    -1,   151,   100,
     152,    -1,     3,   237,   149,    -1,    -1,    64,   146,    -1,
      -1,    63,   122,   155,   248,   133,   146,   153,    -1,    -1,
     150,    -1,   250,    -1,    45,     3,   237,    -1,     3,    -1,
      11,   122,    -1,    11,    49,   122,    -1,   158,   156,    99,
     248,    99,   250,   133,   146,    -1,   158,   157,    67,   248,
     133,   146,    -1,    -1,    12,   122,   162,   248,   133,   146,
      -1,    -1,    13,   164,   146,    12,   122,   248,   133,    -1,
      65,    -1,    65,     3,    -1,    29,    -1,    29,     3,    -1,
      -1,   168,    -1,   170,    -1,   168,   170,    -1,   169,    -1,
     168,   169,    -1,    33,   254,   104,   142,    -1,    61,   104,
     142,    -1,    -1,    14,   122,   172,   254,   133,   126,   167,
     132,    -1,    -1,    32,   122,     3,   237,   133,   174,   126,
     142,   132,    -1,    -1,    26,   126,   176,   142,   132,    -1,
     173,    -1,   177,   173,    -1,   177,    -1,   177,   175,    -1,
     175,    -1,    -1,    51,   126,   180,   142,   132,   178,    -1,
      36,   248,    -1,    36,    -1,    38,   122,   248,   133,    -1,
     182,   146,    -1,     3,    -1,    17,    -1,     4,    -1,   185,
     127,   184,    -1,   184,    -1,    -1,    17,   185,   126,   187,
     139,   132,    -1,    -1,    17,   126,   188,   139,   132,    -1,
      40,   233,    -1,    40,   185,   127,   115,    -1,    -1,   191,
      -1,   192,    -1,   191,   192,    -1,    19,    -1,    20,    -1,
      18,    -1,    37,    -1,    46,    -1,    48,    -1,    47,    -1,
      24,    -1,    22,    -1,     4,    -1,    -1,    53,   234,    -1,
      -1,    53,   235,    -1,    -1,    15,   235,    -1,    -1,   190,
      30,     3,   193,   195,   126,   197,   200,   132,    -1,    -1,
     190,    43,     3,   194,   126,   199,   203,   132,    -1,    -1,
     201,    -1,   202,    -1,   201,   202,    -1,    99,    -1,   148,
     126,   200,   132,    -1,   207,    -1,   227,    -1,   144,    -1,
      -1,   204,    -1,   205,    -1,   204,   205,    -1,    99,    -1,
      45,     3,    -1,   190,    25,   226,     3,   122,   223,   133,
     237,    -1,    45,    -1,    31,    -1,    -1,   190,   206,   208,
     209,    -1,   210,    -1,   209,   100,   210,    -1,     3,   237,
     149,    -1,    -1,   102,   254,    -1,     8,    -1,     9,    -1,
      10,    -1,     5,    -1,    55,    -1,    54,    -1,    44,    -1,
      27,    -1,    28,    -1,   219,    -1,   109,    -1,   110,    -1,
      -1,    -1,   217,     5,    -1,   217,   110,    -1,   219,   217,
      -1,   218,   219,   217,    -1,   214,     3,   220,   215,   217,
     109,   114,     3,   216,   110,    -1,   214,     3,   220,   114,
     216,   110,    -1,   214,     3,   220,   215,   217,   218,   109,
     114,     3,   216,   110,    -1,    -1,   221,    -1,   222,    -1,
     221,   222,    -1,     3,   102,     5,    -1,    -1,   224,    -1,
      93,   225,    -1,   224,   100,    93,   225,    -1,   224,   100,
     225,    -1,   225,    -1,     3,   104,   236,   211,    -1,     3,
     211,    -1,    50,    -1,    34,    -1,    -1,    -1,   190,    25,
     226,     3,   122,   223,   133,   237,   126,   228,   142,   132,
      -1,     3,    -1,    -1,    -1,    25,   229,   122,   223,   133,
     237,   126,   231,   142,   132,    -1,   184,    -1,   185,   127,
     184,    -1,   233,    -1,   232,    -1,   234,    -1,   235,   100,
     234,    -1,   234,    -1,   115,    -1,    35,    -1,   104,   236,
      -1,    -1,    -1,   122,   239,   133,    -1,    -1,   240,    -1,
     241,    -1,   247,    -1,   240,   100,    -1,   241,   247,    -1,
      -1,    23,   254,   242,   238,    -1,   254,   122,   239,   133,
      -1,    52,   122,   239,   133,    -1,    62,   254,    -1,    41,
      -1,    41,   248,    -1,   254,    -1,   249,    -1,   254,    -1,
     249,   100,   254,    -1,   254,    -1,   250,   100,   254,    -1,
      -1,   253,    -1,     3,    -1,     5,    -1,   252,   104,   247,
      -1,   253,   100,   252,   104,   247,    -1,   230,    -1,   257,
      -1,   243,    -1,   245,    -1,   244,    -1,   258,    -1,   212,
      -1,   213,    -1,     6,    -1,   124,   239,   125,    -1,    69,
     251,   132,    -1,   254,   109,   254,    -1,   254,   110,   254,
      -1,   254,    74,   254,    -1,   254,    75,   254,    -1,   254,
      70,   254,    -1,   254,    71,   254,    -1,   254,    73,   254,
      -1,   254,    72,   254,    -1,   254,    87,   254,    -1,   254,
      88,   254,    -1,   118,   254,    -1,   117,   254,    -1,   254,
     107,   254,    -1,   254,   106,   254,    -1,   254,   105,   254,
      -1,   254,    96,   254,    -1,   254,    95,   254,    -1,   254,
      94,   254,    -1,   254,   114,   254,    -1,   254,   116,   254,
      -1,   254,   113,   254,    -1,   254,   112,   254,    -1,   254,
     115,   254,    -1,   254,    67,   254,    -1,   254,    68,   254,
      -1,   254,    39,   254,    -1,   254,    66,   254,    -1,    42,
     122,   254,   133,    -1,    35,   254,    -1,    35,    -1,   122,
     249,   133,    -1,   112,   254,    -1,   254,   124,   254,   125,
      -1,   254,    79,   254,    -1,   254,    78,   254,    -1,   254,
      85,   254,    -1,   254,    84,   254,    -1,   254,    86,   254,
      -1,   254,    77,   254,    -1,   254,    76,   254,    -1,   254,
      83,   254,    -1,   254,    80,   254,    -1,   254,    81,   254,
      -1,   254,    82,   254,    -1,   254,   102,   254,    -1,   254,
     103,   254,   104,   254,    -1,   254,    91,    -1,   254,    90,
      -1,    91,   254,    -1,    90,   254,    -1,    52,   127,     3,
      -1,   128,     3,    -1,   254,   127,   122,   254,   133,    -1,
       3,    -1,     4,    -1,     3,    -1,   115,    -1,   254,   127,
     255,    89,   256,    -1,   254,    92,   256,    -1,   254,   127,
     124,   254,   125,    -1,   254,   127,   128,   256,    -1,   254,
      92,   128,   256,    -1,   254,   127,   128,   124,   254,   125,
      -1,   254,    92,   128,   124,   254,   125,    -1,   254,   127,
     256,    -1,     3,    -1,    16,     3,    -1,    16,     3,   102,
       3,    -1,    16,     3,   102,     5,    -1,   190,   259,    -1,
      21,    16,   234,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1857,  1857,  1859,  1859,  1860,  1861,  1863,  1864,  1865,
    1866,  1867,  1868,  1869,  1870,  1872,  1872,  1873,  1874,  1876,
    1877,  1878,  1879,  1880,  1881,  1882,  1884,  1885,  1887,  1890,
    1893,  1894,  1895,  1896,  1897,  1898,  1899,  1900,  1901,  1902,
    1903,  1904,  1905,  1906,  1909,  1910,  1911,  1912,  1913,  1914,
    1915,  1916,  1928,  1929,  1933,  1940,  1959,  1960,  1962,  1963,
    1965,  1966,  1968,  2028,  2029,  2032,  2032,  2051,  2052,  2053,
    2058,  2062,  2067,  2068,  2070,  2090,  2138,  2138,  2157,  2157,
    2172,  2175,  2178,  2181,  2185,  2186,  2187,  2188,  2189,  2190,
    2192,  2203,  2206,  2206,  2237,  2237,  2262,  2262,  2278,  2279,
    2280,  2281,  2289,  2298,  2298,  2347,  2351,  2362,  2372,  2389,
    2390,  2391,  2393,  2394,  2396,  2396,  2398,  2398,  2421,  2435,
    2451,  2452,  2453,  2454,  2461,  2462,  2463,  2464,  2465,  2466,
    2467,  2468,  2469,  2470,  2474,  2475,  2477,  2478,  2480,  2481,
    2485,  2483,  2491,  2489,  2498,  2499,  2500,  2501,  2502,  2503,
    2504,  2505,  2507,  2513,  2514,  2515,  2516,  2517,  2518,  2521,
    2587,  2587,  2589,  2589,  2591,  2592,  2594,  2678,  2679,  2688,
    2689,  2692,  2693,  2694,  2695,  2696,  2697,  2698,  2716,  2718,
    2719,  2720,  2722,  2723,  2724,  2726,  2727,  2729,  2733,  2736,
    2741,  2742,  2743,  2744,  2745,  2755,  2759,  2765,  2771,  2779,
    2784,  2790,  2798,  2806,  2807,  2808,  2811,  2810,  2827,  2828,
    2830,  2829,  2853,  2872,  2886,  2887,  2889,  2890,  2892,  2893,
    2894,  2903,  2904,  2908,  2909,  2911,  2912,  2913,  2915,  2919,
    2920,  2925,  2926,  2963,  3013,  3034,  3056,  3059,  3066,  3069,
    3072,  3075,  3078,  3081,  3086,  3087,  3089,  3090,  3092,  3098,
    3107,  3108,  3109,  3110,  3111,  3112,  3114,  3118,  3131,  3151,
    3161,  3170,  3171,  3172,  3173,  3174,  3175,  3176,  3177,  3178,
    3179,  3180,  3181,  3182,  3183,  3184,  3185,  3186,  3187,  3188,
    3189,  3190,  3191,  3192,  3193,  3194,  3195,  3196,  3197,  3198,
    3199,  3200,  3201,  3202,  3203,  3204,  3205,  3206,  3207,  3208,
    3209,  3210,  3211,  3212,  3213,  3214,  3215,  3217,  3218,  3219,
    3220,  3222,  3237,  3244,  3251,  3252,  3253,  3254,  3256,  3274,
    3281,  3292,  3299,  3306,  3316,  3327,  3497,  3532,  3539,  3546,
    3553,  3575
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
  "\"namespace\"", "\"package\"", "\"protected\"", "\"public\"",
  "\"private\"", "\"use\"", "\"internal\"", "\"new\"", "\"native\"",
  "\"function\"", "\"finally\"", "\"undefined\"", "\"NaN\"",
  "\"continue\"", "\"class\"", "\"const\"", "\"catch\"", "\"case\"",
  "\"set\"", "\"void\"", "\"throw\"", "\"static\"", "\"with\"",
  "\"instanceof\"", "\"import\"", "\"return\"", "\"typeof\"",
  "\"interface\"", "\"null\"", "\"var\"", "\"dynamic\"", "\"override\"",
  "\"final\"", "\"each\"", "\"get\"", "\"try\"", "\"super\"",
  "\"extends\"", "\"false\"", "\"true\"", "\"Boolean\"", "\"uint\"",
  "\"int\"", "\"Number\"", "\"String\"", "\"default\"", "\"delete\"",
  "\"if\"", "\"else\"", "\"break\"", "\"is\"", "\"in\"", "\"as\"",
  "\"{ (dictionary)\"", "\"==\"", "\"===\"", "\"!=\"", "\"!==\"", "\"<=\"",
  "\">=\"", "\"|=\"", "\"/=\"", "\"%=\"", "\"*=\"", "\"&=\"", "\"+=\"",
  "\"-=\"", "\"^=\"", "\">>=\"", "\"<<=\"", "\">>>=\"", "\"||\"", "\"&&\"",
  "\"::\"", "\"--\"", "\"++\"", "\"..\"", "\"...\"", "\"<<\"", "\">>>\"",
  "\">>\"", "prec_none", "below_semicolon", "';'", "','",
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
  "CONSTANT", "XML", "OPEN", "CLOSE", "CLOSE2", "XMLTEXT", "XML2",
  "XMLNODE", "MAYBE_XMLATTRIBUTES", "XMLATTRIBUTES", "XMLATTRIBUTE",
  "MAYBE_PARAM_LIST", "PARAM_LIST", "PARAM", "GETSET",
  "FUNCTION_DECLARATION", "$@13", "MAYBE_IDENTIFIER", "INNERFUNCTION",
  "$@14", "CLASS", "PACKAGEANDCLASS", "CLASS_SPEC", "CLASS_SPEC_LIST",
  "TYPE", "MAYBETYPE", "MAYBE_PARAM_VALUES", "MAYBE_EXPRESSION_LIST",
  "EXPRESSION_LIST", "EXPRESSION_LIST_AND_COMMA", "XX", "NEW",
  "FUNCTIONCALL", "DELETE", "RETURN", "NONCOMMAEXPRESSION", "EXPRESSION",
  "COMMA_EXPRESSION", "VOIDEXPRESSION", "MAYBE_DICT_EXPRPAIR_LIST",
  "DICTLH", "DICT_EXPRPAIR_LIST", "E", "ID_OR_NS", "SUBNODE", "MEMBER",
  "VAR_READ", "NAMESPACE_ID", "NAMESPACE_DECLARATION", "USE_NAMESPACE", 0
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
     345,   346,   347,   348,   349,   350,   351,   352,   353,    59,
      44,   354,    61,    63,    58,   124,    94,    38,   355,    60,
      62,   356,    45,    43,    47,    42,    37,   126,    33,   357,
     358,   359,    40,   360,    91,    93,   123,    46,    64,   361,
     362,   363,   125,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   134,   135,   136,   136,   137,   137,   138,   138,   138,
     138,   138,   138,   138,   138,   139,   139,   140,   140,   141,
     141,   141,   141,   141,   141,   141,   142,   142,   143,   143,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   145,   145,   145,   145,   145,   145,
     145,   145,   146,   146,   147,   148,   149,   149,   150,   150,
     151,   151,   152,   153,   153,   155,   154,   156,   156,   156,
     157,   157,   158,   158,   159,   160,   162,   161,   164,   163,
     165,   165,   166,   166,   167,   167,   167,   167,   168,   168,
     169,   170,   172,   171,   174,   173,   176,   175,   177,   177,
     178,   178,   178,   180,   179,   181,   181,   182,   183,   184,
     184,   184,   185,   185,   187,   186,   188,   186,   189,   189,
     190,   190,   191,   191,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   193,   193,   194,   194,   195,   195,
     197,   196,   199,   198,   200,   200,   201,   201,   202,   202,
     202,   202,   202,   203,   203,   204,   204,   205,   205,   205,
     206,   206,   208,   207,   209,   209,   210,   211,   211,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   213,   214,
     215,   216,   217,   217,   217,   218,   218,   219,   219,   219,
     220,   220,   221,   221,   222,   223,   223,   223,   223,   224,
     224,   225,   225,   226,   226,   226,   228,   227,   229,   229,
     231,   230,   232,   233,   234,   234,   235,   235,   236,   236,
     236,   237,   237,   238,   238,   239,   239,   239,   240,   241,
     240,   242,   243,   244,   244,   245,   246,   246,   247,   248,
     249,   249,   250,   250,   251,   251,   252,   252,   253,   253,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   255,   255,   256,   256,   254,   254,
     254,   254,   254,   254,   254,   257,   258,   259,   259,   259,
     260,   261
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     4,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     1,     4,     1,     1,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     4,     2,     1,     1,     3,     2,     0,     2,     2,
       1,     3,     3,     0,     2,     0,     7,     0,     1,     1,
       3,     1,     2,     3,     8,     6,     0,     6,     0,     7,
       1,     2,     1,     2,     0,     1,     1,     2,     1,     2,
       4,     3,     0,     8,     0,     9,     0,     5,     1,     2,
       1,     2,     1,     0,     6,     2,     1,     4,     2,     1,
       1,     1,     3,     1,     0,     6,     0,     5,     2,     4,
       0,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     2,     0,     2,     0,     2,
       0,     9,     0,     8,     0,     1,     1,     2,     1,     4,
       1,     1,     1,     0,     1,     1,     2,     1,     2,     8,
       1,     1,     0,     4,     1,     3,     3,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     0,     2,     2,     2,     3,    10,     6,    11,
       0,     1,     1,     2,     3,     0,     1,     2,     4,     3,
       1,     4,     2,     1,     1,     0,     0,    12,     1,     0,
       0,    10,     1,     3,     1,     1,     1,     3,     1,     1,
       1,     2,     0,     0,     3,     0,     1,     1,     1,     2,
       2,     0,     4,     4,     4,     2,     1,     2,     1,     1,
       1,     3,     1,     3,     0,     1,     1,     1,     3,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     2,
       1,     3,     2,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     2,     2,     2,
       2,     3,     2,     5,     1,     1,     1,     1,     5,     3,
       5,     4,     4,     6,     6,     3,     1,     2,     4,     4,
       2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     120,   326,   133,   172,   258,   169,   170,   171,     0,     0,
      78,     0,     0,   126,   124,   125,     0,   132,     0,   131,
     209,   176,   177,   290,   127,     0,     0,     0,   175,   128,
     130,   129,     0,     0,   174,   173,     0,     0,   244,     0,
       0,    14,   179,     0,     0,     0,     0,   225,   120,     0,
       0,     2,   120,     5,    54,    12,     0,    36,    67,    31,
      32,    33,    34,    35,    38,   120,    37,     7,    30,     0,
     121,   122,     9,     8,    11,   256,   257,     0,   178,    10,
     250,   252,   254,   253,    39,   242,   251,   255,    41,    40,
       0,     0,    72,    76,   120,    92,   109,   111,   110,   116,
     113,     0,     0,   326,   231,   208,     0,   289,     0,     0,
     118,     0,   103,   225,     0,   235,    65,   246,   247,     0,
       0,   245,   310,   309,   292,   272,   271,     0,   240,     0,
     226,   227,   228,   238,    82,     0,   106,   236,     0,    80,
      44,    43,   120,    45,    29,     0,    46,    47,    48,    50,
       0,    49,   312,     1,     6,   120,   326,     0,    68,     0,
       0,    69,    53,   108,     0,   205,     0,   161,     0,   160,
     162,   330,   123,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   308,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   225,     0,     0,    55,
      73,     0,     0,     0,   120,   114,     0,   212,     0,   215,
     214,   331,   223,   195,     0,   239,     0,     0,    27,     0,
     311,     0,   260,     0,     0,     0,   291,   259,   229,   230,
      83,   222,    59,    60,   105,   237,    58,    81,    42,    28,
     120,     0,   222,     0,     0,    52,   327,   204,   203,     0,
     134,   136,     0,     0,     0,   191,   192,   243,   286,   287,
     284,   285,   265,   266,   268,   267,   263,   264,   300,   299,
     295,   294,   302,   303,   304,   301,   297,   296,   298,   269,
     270,   316,   317,     0,   319,   278,   277,   276,   305,     0,
     275,   274,   273,   261,   262,   282,   281,   279,   283,   280,
       0,     0,   316,   315,     0,     0,     0,     0,   325,     0,
       0,     0,    25,     0,   120,    17,    23,     0,    20,    19,
      22,    21,   120,   112,     0,   225,   232,   167,     0,     0,
     196,   200,   107,   119,   213,   288,     0,    26,   234,     0,
     248,     0,   241,     0,    57,     0,   120,    13,    57,     0,
       0,     0,     0,     0,   138,     0,     0,   222,   163,   164,
       0,   180,   181,   182,   193,     0,   322,     0,   233,   293,
       0,     0,     0,   321,     0,   120,     0,     0,   117,    18,
     120,     0,     0,     0,     0,   202,   197,   222,     0,     0,
     120,     0,   220,   219,   218,   221,     0,    62,    61,    51,
       0,   120,   328,   329,   195,   135,     0,     0,   216,   137,
     142,    57,     0,   194,     0,     0,     0,   306,   313,   320,
       0,   318,    77,     0,    84,     0,   115,   224,   168,   167,
       0,     0,   199,     0,     0,    98,   102,   100,   104,    63,
     249,    56,     0,    75,     0,   139,   140,     0,   120,   166,
     165,   188,   183,   179,   184,     0,   182,   324,   323,    79,
       0,     0,     0,    85,    88,    86,    24,   201,   210,   198,
      96,     0,    99,   101,   120,    66,   120,   222,   120,   217,
       0,   157,     0,     0,   120,   155,     0,   179,   182,   185,
       0,    27,    93,    89,    87,    27,    27,   222,    64,    74,
       0,   148,   152,     0,     0,     0,   120,   146,   150,   151,
     158,   205,   143,   156,   181,     0,   186,    27,    91,     0,
       0,     0,   206,   120,   141,   147,     0,     0,   181,    90,
     211,    97,    94,    27,     0,     0,   187,     0,     0,     0,
     149,   195,   189,    27,   207,     0,     0,   222,    95,   159
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,    53,   333,   334,   335,   356,   357,
     143,   144,   163,   336,   145,   417,   146,   256,   253,   495,
      57,   241,   159,   160,    58,    59,    60,    61,   221,    62,
      94,   147,   148,   482,   483,   484,   485,    63,   223,   455,
     558,   456,   516,   457,   458,    64,   238,   149,    65,    66,
     227,   228,    67,   342,   224,    68,   150,    70,    71,   374,
     376,   427,   338,   498,   339,   468,   525,   526,   527,   503,
     504,   505,   170,   340,   272,   378,   379,   405,    75,    76,
      77,   383,   434,   435,   475,    78,   274,   275,   276,   349,
     350,   351,   269,   341,   553,   106,    80,   515,   229,   230,
     414,   429,   415,   364,   346,   129,   130,   131,   232,    81,
      82,    83,   151,   132,   234,   235,    84,   119,   120,   121,
      85,   327,   304,    86,    87,   171,    88,    89
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -467
static const yytype_int16 yypact[] =
{
    1718,   -46,  -467,  -467,  -467,  -467,  -467,  -467,   -29,   -54,
    -467,   -32,    60,  -467,  -467,  -467,    46,  -467,  2165,  -467,
      99,  -467,  -467,  2236,  -467,   -10,    82,    -2,  -467,  -467,
    -467,  -467,    22,   -61,  -467,  -467,  2165,    25,   110,  2165,
    2165,  -467,  -467,  2165,  2165,  2165,  2165,  2165,   678,   150,
     211,  -467,   548,  -467,  -467,  -467,    88,  -467,  2094,  -467,
    -467,  -467,  -467,  -467,  -467,  1970,  -467,  -467,  -467,    49,
     347,  -467,  -467,  -467,  -467,  -467,  -467,   213,  -467,  -467,
    -467,  -467,  -467,  -467,   120,  2936,  -467,  -467,  -467,  -467,
     220,   105,  -467,  -467,  1970,  -467,  -467,  -467,  -467,  -467,
    -467,    92,    82,  -467,   -55,  -467,   109,   157,  2165,    94,
    -467,  2165,  -467,  2165,   230,   157,  -467,  -467,  -467,   107,
     130,   141,   157,   157,   397,   157,   157,   -60,  2874,   118,
     152,  2165,  -467,  2874,   253,   254,  2165,  2165,   254,   255,
    -467,  -467,   808,  -467,  -467,   134,  -467,  -467,  -467,  -467,
     245,  -467,  -467,  -467,  -467,  1068,   195,   264,  -467,   176,
     209,   120,   178,  -467,   275,    33,   277,  -467,   279,  -467,
    -467,  -467,  -467,   280,  2165,  2165,  2165,  2165,  2165,  2165,
    2165,  2165,  2165,  2165,  2165,  2165,  2165,  2165,  2165,  2165,
    2165,  2165,  2165,  2165,  2165,  2165,  2165,  2165,  -467,  -467,
      13,  2165,  2165,  2165,  2165,  2165,  2165,  2165,  2165,  2165,
    2165,  2165,  2165,  2165,  2165,  2165,  2165,  2165,    21,  -467,
    -467,  2165,   273,  2165,  1198,  -467,    82,   163,   164,  -467,
    -467,  -467,   170,    39,   160,   194,   102,  2295,  1844,   167,
    -467,  2165,  -467,  2165,   110,  2165,  -467,  -467,  -467,  -467,
    -467,   197,   202,  -467,  -467,  -467,   202,  -467,  -467,  -467,
    1970,   171,   197,  2165,  2165,  -467,   207,  -467,  -467,   301,
     257,   260,   303,   212,    32,   280,  -467,  2874,  1060,   540,
     540,   540,  3184,  3184,  3184,  3184,  1060,  1060,  2874,  2874,
    2874,  2874,  2874,  2874,  2874,  2874,  2874,  2874,  2874,   349,
    2998,  -467,  -467,    16,  -467,   393,   393,   393,  2874,  2502,
    2229,  3060,  3122,  1060,  1060,   397,   397,   157,   157,   157,
     182,  2564,   227,  -467,  2165,  2165,    36,   229,  -467,   186,
     198,  2364,  -467,   189,  1328,  -467,  -467,   200,  -467,  -467,
    -467,  -467,  1198,  -467,    82,  2165,  -467,    50,   320,   191,
     231,  -467,  -467,  -467,   201,  -467,   206,  1844,  -467,   196,
    -467,   223,  2874,    72,   238,   254,   938,  -467,   -20,   242,
     214,   204,   208,    82,   330,    82,   222,   197,   246,  -467,
     350,  -467,  -467,  -467,  -467,  2165,  -467,  2165,  -467,  -467,
    2433,  2626,  2165,  -467,    15,  1970,  2165,   232,  -467,  -467,
    1198,   224,   221,  2165,    72,  -467,  -467,   197,    41,   103,
    1970,  2165,  -467,  -467,  -467,  -467,  2165,  -467,  -467,  -467,
    2165,  1970,  -467,  -467,    39,  -467,    82,   233,  -467,   261,
    -467,   238,   303,  -467,   247,    17,  2688,  2874,  -467,  -467,
    2750,  -467,  -467,   235,    27,   241,  -467,  -467,  2874,   258,
     236,   320,  -467,   251,   256,  -467,  -467,   103,  -467,   300,
    -467,  2874,   -52,  -467,   243,   261,  -467,    82,   218,  -467,
    -467,  -467,  -467,   267,  -467,   276,  -467,  -467,  -467,  -467,
    2165,   282,   259,    27,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,   384,  -467,  -467,  1970,  -467,  1970,   197,  1458,  -467,
     386,  -467,   367,   265,   250,  -467,   396,   290,  -467,    23,
    2812,  1844,  -467,  -467,  -467,  1844,  1844,   197,  -467,  -467,
     270,  -467,  -467,   281,   228,   278,  1588,  -467,  -467,  -467,
    -467,    33,  -467,  -467,  -467,   402,    23,  1844,  -467,   286,
     293,   294,  -467,  1458,  -467,  -467,   406,   302,  -467,  -467,
    -467,  -467,  -467,  1844,   296,   289,  -467,   304,   287,   297,
    -467,    39,  -467,  1844,  -467,   298,   310,   197,  -467,  -467
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -467,  -467,   271,  -467,   378,  -296,  -467,    98,  -466,   -31,
       1,   -58,   -88,    51,     2,    18,   375,   299,    70,  -467,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -467,  -467,   -37,   -36,  -467,  -467,    -9,
    -467,    -7,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
       9,    96,  -467,  -467,  -467,  -467,     0,  -467,   381,  -467,
    -467,  -467,    57,  -467,    58,  -467,   -91,  -467,   -73,  -467,
    -467,   -47,  -467,     3,  -467,  -467,    28,    20,  -467,  -467,
    -467,  -467,  -423,  -417,  -467,  -397,  -467,  -467,   199,  -411,
    -467,  -307,   -65,     4,  -467,  -467,  -467,  -467,  -467,   444,
     -87,    52,    68,  -192,  -467,   -99,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -122,  -125,   429,   -50,  -467,   237,  -467,
     -13,  -467,  -189,  -467,  -467,  -467,  -467,  -467
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -315
static const yytype_int16 yytable[] =
{
      69,    54,    56,    74,    79,   104,   222,   162,   161,   249,
     107,   254,   255,   464,   239,   231,   301,   142,   301,   301,
      91,   100,   472,   115,   322,   323,   122,   123,   472,   328,
     124,   125,   126,   128,   133,   100,   162,   200,   476,   301,
     245,   406,   347,    90,   347,   538,   401,   -70,   174,   539,
     540,    55,    69,    54,    56,    74,    79,    72,    73,   509,
     480,   113,   102,    96,    97,   164,   114,   267,    93,   217,
     368,   549,   218,   246,   165,    96,    97,    98,   508,   166,
     167,   496,   416,   268,   259,    96,    97,   559,   481,    98,
      95,   536,   168,    92,   169,   128,   329,   566,   237,    98,
     133,   452,   105,    55,   445,    96,    97,   412,   101,    72,
      73,   547,   108,   117,   386,   118,   359,   320,   133,    98,
     111,   360,   109,   128,   128,   557,   473,   474,   302,   453,
     302,   302,   348,   474,   451,   454,   302,   393,   369,   370,
     385,   303,   381,   324,   489,   325,   382,   116,   112,   326,
     565,   302,   403,   152,   404,    69,    54,    56,    74,    79,
     392,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   431,    99,   413,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   133,   321,   441,    55,   422,   128,   423,
     331,   153,    72,    73,   155,   450,   173,   353,   225,   226,
     174,   236,     2,   219,    69,    54,   337,   220,   128,   366,
     133,   233,   362,   240,   243,   343,    13,    14,    15,   242,
      17,   244,    19,   247,   164,   354,   402,   198,   199,   200,
     128,   128,   248,   165,     2,    24,   250,   251,   257,   167,
     260,   164,   -71,   500,    29,    30,    31,   262,    13,    14,
      15,   443,    17,   169,    19,   263,   264,   265,   266,   216,
     270,   217,   271,   273,   218,   330,   425,    24,   428,   460,
    -113,   344,   345,   352,   245,   500,    29,    30,    31,   259,
     358,   363,   365,   367,   372,   520,   377,   442,   259,   371,
     373,   390,   391,   375,   380,   388,  -314,   501,   394,   395,
     396,   398,   459,   347,   407,   541,   400,   411,  -112,   410,
     424,   408,   133,   463,    69,    54,   337,   162,   409,   428,
     416,   420,    69,    54,   337,   426,   432,   421,   430,   501,
    -153,     2,   162,   354,   447,   433,   446,   471,   444,   466,
     403,   467,   488,   162,   494,    13,    14,    15,   479,    17,
     462,    19,   436,   486,   437,   569,   497,   490,   491,   440,
     499,   506,  -154,   128,    24,   507,   511,   517,   175,   530,
     448,   512,   531,    29,    30,    31,   542,   532,   133,   534,
      69,    54,   337,   461,   535,   548,   518,   543,   519,   555,
     544,   561,   556,   563,   562,   176,   177,   178,   550,   179,
     180,   181,   182,   183,   184,   551,   261,   552,   560,   564,
     154,   567,   399,   158,   252,   418,   162,   197,   162,   198,
     199,   200,   568,   201,   202,   203,   513,   514,   492,   469,
     493,   172,   554,   545,   206,   207,   208,   533,   209,   210,
     470,   211,   212,   213,   214,   215,   546,   510,   502,   487,
     110,   216,   449,   217,   384,   127,   218,     0,   465,     0,
       0,   361,     0,   198,   199,   200,     0,   198,   199,   200,
       0,     0,     0,     0,     0,     0,     0,     0,   524,   522,
     523,   528,   529,     0,   502,   211,   212,   213,   214,   215,
       0,   213,   214,   215,     0,   216,     0,   217,     0,   216,
     218,   217,     0,     0,   218,     0,   524,   522,   523,   528,
     529,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   524,   522,   523,   528,   529,    -4,     0,
       0,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    21,    22,     0,     0,   175,
       0,     0,     0,    23,     0,    24,    25,     0,    26,     0,
      27,     0,    28,     0,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,  -315,  -315,  -315,     0,
      36,    37,     0,     0,   183,   184,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     198,   199,   200,     0,   201,   202,   203,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,    41,     0,   209,
     210,     0,   211,   212,   213,   214,   215,    42,     0,     0,
      43,     0,   216,     0,   217,    44,    45,   218,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     0,     0,     0,
      -4,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   134,     0,   135,
       0,     0,     0,    23,   136,    24,    25,     0,    26,   137,
      27,     0,    28,   138,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,   139,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
      43,     0,     0,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     0,     0,     0,
     141,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   134,     0,   135,
       0,     0,     0,    23,   136,    24,    25,     0,    26,   137,
      27,     0,    28,   138,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,   139,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
      43,     0,     0,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     0,     0,     0,
     258,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   134,     0,   135,
       0,     0,     0,    23,   136,    24,    25,     0,    26,   137,
      27,     0,    28,   138,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,   139,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
      43,     0,     0,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     0,     0,     0,
     419,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    21,    22,     0,     0,  -315,
       0,     0,     0,    23,     0,    24,    25,     0,    26,     0,
      27,     0,    28,     0,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,     0,  -315,  -315,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     198,   199,   200,     0,   201,   202,   203,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,    41,     0,  -315,
    -315,     0,   211,   212,   213,   214,   215,    42,     0,     0,
      43,     0,   216,     0,   217,    44,    45,   218,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     0,     0,     0,
      -3,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    21,    22,     0,     0,     0,
       0,     0,     0,    23,     0,    24,    25,     0,    26,     0,
      27,     0,    28,     0,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
      43,     0,     0,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     0,     0,     0,
     -15,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    21,    22,     0,     0,     0,
       0,     0,     0,    23,     0,    24,    25,     0,    26,     0,
      27,     0,    28,     0,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
      43,     0,     0,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     0,     0,     0,
     -16,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    21,    22,     0,     0,     0,
       0,     0,     0,    23,     0,    24,    25,     0,    26,     0,
      27,     0,    28,     0,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,   521,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
      43,     0,     0,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     0,     0,     0,
    -144,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    21,    22,     0,     0,     0,
       0,     0,     0,    23,     0,    24,    25,     0,    26,     0,
      27,     0,    28,     0,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,   521,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
      43,     0,     0,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     0,    -3,     0,
    -145,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    21,    22,     0,     0,     0,
       0,     0,     0,    23,     0,    24,    25,     0,    26,     0,
      27,     0,    28,     0,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
      43,     0,     0,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,    48,     0,    49,     1,     2,     3,
       4,     0,     5,     6,     7,     8,     9,    10,    11,     0,
    -120,     0,    13,    14,    15,    16,    17,    18,    19,    20,
       0,    21,    22,   134,     0,   135,     0,     0,     0,    23,
     136,    24,    25,     0,    26,   137,    27,     0,    28,   138,
      29,    30,    31,     0,     0,    32,    33,     0,    34,    35,
       0,     0,     0,     0,     0,     0,    36,    37,     0,   139,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,     0,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    42,     0,     0,    43,     0,     0,     0,
       0,    44,    45,     0,     0,     0,    46,     0,    47,     0,
      48,     0,    49,     1,     2,     3,     4,     0,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    13,    14,
      15,    16,    17,    18,    19,    20,     0,    21,    22,   134,
       0,   135,     0,     0,     0,    23,   136,    24,    25,     0,
      26,   137,    27,     0,    28,   138,    29,    30,    31,     0,
       0,    32,    33,     0,    34,    35,     0,     0,     0,     0,
       0,     0,    36,    37,     0,   139,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,     0,     0,     0,     0,     0,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    42,
       0,     0,    43,     0,     0,     0,     0,    44,    45,     0,
       0,     0,    46,     0,    47,     0,    48,   156,    49,     3,
       4,     0,     5,     6,     7,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,    20,
       0,    21,    22,     0,     0,   135,     0,     0,     0,    23,
       0,     0,     0,     0,     0,     0,    27,     0,    28,   157,
       0,     0,     0,     0,     0,     0,    33,     0,    34,    35,
       0,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,   103,     0,
       3,     4,     0,     5,     6,     7,     0,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,    18,     0,
      20,     0,    21,    22,     0,     0,     0,     0,     0,     0,
      23,     0,     0,    42,     0,     0,    43,    27,     0,    28,
       0,    44,    45,     0,     0,     0,    46,    33,    47,    34,
      35,     0,    49,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,   103,
       0,     3,     4,     0,     5,     6,     7,     0,     0,     0,
       0,     0,     0,     0,     0,    39,    40,     0,     0,    18,
       0,    20,     0,    21,    22,     0,     0,     0,   175,     0,
       0,     0,     0,     0,    42,     0,     0,    43,     0,     0,
      28,     0,    44,    45,     0,     0,     0,    46,    33,    47,
      34,    35,     0,    49,     0,   176,   177,   178,     0,   179,
     180,   181,   182,   183,   184,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   198,
     199,   200,     0,   201,   202,   203,    39,    40,     0,     0,
       0,     0,     0,     0,   175,   207,   208,     0,   209,   210,
       0,   211,   212,   213,   214,   215,     0,     0,     0,     0,
       0,   216,     0,   217,     0,     0,   218,     0,    46,     0,
      47,   176,   177,   178,    49,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,     0,   198,   199,   200,     0,   201,
     202,   203,     0,     0,     0,     0,     0,   204,   205,     0,
     206,   207,   208,   175,   209,   210,     0,   211,   212,   213,
     214,   215,     0,     0,     0,     0,     0,   216,     0,   217,
       0,     0,   218,     0,     0,     0,     0,     0,   355,     0,
     176,   177,   178,     0,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,     0,   198,   199,   200,     0,   201,   202,
     203,     0,     0,     0,     0,     0,   204,   205,     0,   206,
     207,   208,   175,   209,   210,     0,   211,   212,   213,   214,
     215,     0,     0,     0,     0,     0,   216,     0,   217,     0,
       0,   218,     0,     0,     0,     0,     0,   397,     0,   176,
     177,   178,     0,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,     0,   198,   199,   200,     0,   201,   202,   203,
       0,     0,     0,     0,     0,   204,   205,     0,   206,   207,
     208,   175,   209,   210,     0,   211,   212,   213,   214,   215,
       0,     0,     0,     0,     0,   216,     0,   217,     0,     0,
     218,     0,     0,     0,     0,     0,   438,     0,   176,   177,
     178,     0,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,     0,   198,   199,   200,     0,   201,   202,   203,     0,
       0,     0,     0,   175,   204,   205,   387,   206,   207,   208,
       0,   209,   210,     0,   211,   212,   213,   214,   215,     0,
       0,     0,     0,     0,   216,     0,   217,     0,     0,   218,
     176,   177,   178,     0,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,     0,   198,   199,   200,     0,   201,   202,
     203,     0,     0,     0,     0,   175,   204,   205,     0,   206,
     207,   208,     0,   209,   210,     0,   211,   212,   213,   214,
     215,     0,     0,     0,     0,     0,   216,     0,   217,   389,
       0,   218,   176,   177,   178,     0,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,     0,   198,   199,   200,     0,
     201,   202,   203,     0,     0,     0,     0,   175,   204,   205,
       0,   206,   207,   208,     0,   209,   210,     0,   211,   212,
     213,   214,   215,     0,     0,     0,     0,     0,   216,     0,
     217,   439,     0,   218,   176,   177,   178,     0,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,     0,   198,   199,
     200,     0,   201,   202,   203,     0,     0,     0,     0,   175,
     204,   205,     0,   206,   207,   208,     0,   209,   210,     0,
     211,   212,   213,   214,   215,     0,     0,     0,     0,     0,
     216,     0,   217,   477,     0,   218,   176,   177,   178,     0,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,     0,
     198,   199,   200,     0,   201,   202,   203,     0,     0,     0,
       0,   175,   204,   205,     0,   206,   207,   208,     0,   209,
     210,     0,   211,   212,   213,   214,   215,     0,     0,     0,
       0,     0,   216,     0,   217,   478,     0,   218,   176,   177,
     178,     0,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,     0,   198,   199,   200,     0,   201,   202,   203,     0,
       0,     0,     0,   175,   204,   205,   537,   206,   207,   208,
       0,   209,   210,     0,   211,   212,   213,   214,   215,     0,
       0,     0,     0,     0,   216,     0,   217,     0,     0,   218,
     176,   177,   178,     0,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,     0,   198,   199,   200,     0,   201,   202,
     203,     0,     0,     0,     0,   175,   204,   205,     0,   206,
     207,   208,     0,   209,   210,     0,   211,   212,   213,   214,
     215,     0,     0,     0,     0,     0,   216,     0,   217,     0,
       0,   218,   176,   177,   178,     0,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,     0,   198,   199,   200,     0,
     201,   202,   203,     0,     0,     0,     0,   175,   204,   205,
       0,   206,   207,   208,     0,     0,   210,     0,   211,   212,
     213,   214,   215,     0,     0,     0,     0,     0,   216,     0,
     217,     0,     0,   218,   176,   177,   178,     0,   179,   180,
     181,   182,   183,   184,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   198,   199,
     200,     0,   201,   202,   203,     0,     0,     0,     0,   175,
       0,     0,     0,   206,   207,   208,     0,   209,   210,     0,
     211,   212,   213,   214,   215,     0,     0,     0,     0,     0,
     216,     0,   217,     0,     0,   218,   176,   177,   178,     0,
     179,   180,   181,   182,   183,   184,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     198,   199,   200,     0,   201,   202,   203,     0,     0,     0,
       0,   175,     0,     0,     0,     0,     0,   208,     0,   209,
     210,     0,   211,   212,   213,   214,   215,     0,     0,     0,
       0,     0,   216,     0,   217,     0,     0,   218,   176,   177,
     178,     0,   179,   180,   181,   182,   183,   184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   198,   199,   200,     0,   201,   202,   203,     0,
       0,     0,     0,   175,     0,     0,     0,     0,     0,  -315,
       0,   209,   210,     0,   211,   212,   213,   214,   215,     0,
       0,     0,     0,     0,   216,     0,   217,     0,     0,   218,
     176,   177,   178,     0,  -315,  -315,  -315,  -315,   183,   184,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   198,   199,   200,     0,   201,   202,
     203,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   209,   210,     0,   211,   212,   213,   214,
     215,     0,     0,     0,     0,     0,   216,     0,   217,     0,
       0,   218
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,     0,    18,    94,    65,    58,   131,
      23,   136,   137,   424,   113,   102,     3,    48,     3,     3,
      49,    12,     5,    36,     3,     4,    39,    40,     5,   218,
      43,    44,    45,    46,    47,    26,    94,    92,   435,     3,
     100,   348,     3,    89,     3,   511,   342,    67,   100,   515,
     516,     0,    52,    52,    52,    52,    52,     0,     0,   476,
      33,   122,    16,     3,     4,    16,   127,    34,   122,   124,
     262,   537,   127,   133,    25,     3,     4,    17,   475,    30,
      31,   133,   102,    50,   142,     3,     4,   553,    61,    17,
     122,   508,    43,   122,    45,   108,   221,   563,   111,    17,
     113,   408,     3,    52,   400,     3,     4,    35,    12,    52,
      52,   534,   122,     3,   303,     5,   241,   216,   131,    17,
     122,   243,    26,   136,   137,   548,   109,   110,   115,    26,
     115,   115,    93,   110,    93,    32,   115,   326,   263,   264,
     124,   128,   110,   122,   451,   124,   114,   122,   126,   128,
     561,   115,   102,     3,   104,   155,   155,   155,   155,   155,
     124,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   377,   126,   115,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   394,   155,     3,   221,     5,
     223,     0,   155,   155,   126,   407,     3,   115,   126,   127,
     100,   127,     4,     3,   224,   224,   224,   122,   241,   260,
     243,   122,   245,     3,   104,   226,    18,    19,    20,   132,
      22,   100,    24,   125,    16,   236,   345,    90,    91,    92,
     263,   264,   100,    25,     4,    37,     3,     3,     3,    31,
     126,    16,    67,    45,    46,    47,    48,     3,    18,    19,
      20,   396,    22,    45,    24,    99,    67,    99,     3,   122,
       3,   124,     3,     3,   127,    12,   373,    37,   375,   411,
     127,   127,   122,   133,   100,    45,    46,    47,    48,   357,
     133,   104,   100,   132,     3,   497,     3,   395,   366,   102,
      53,   324,   325,    53,   102,   133,    89,    99,    89,   133,
     122,   132,   410,     3,   133,   517,   126,   104,   127,   133,
     122,   100,   345,   421,   334,   334,   334,   395,   132,   426,
     102,    99,   342,   342,   342,    15,   100,   133,   126,    99,
     132,     4,   410,   344,   133,     5,   132,   110,   126,   126,
     102,   100,   126,   421,    64,    18,    19,    20,   133,    22,
     420,    24,   385,   132,   387,   567,   133,   126,   122,   392,
     467,   114,   132,   396,    37,   109,   104,     3,    39,     3,
     403,   132,    25,    46,    47,    48,   126,   132,   411,     3,
     400,   400,   400,   416,   114,     3,   494,   126,   496,     3,
     132,   122,   110,   126,   110,    66,    67,    68,   132,    70,
      71,    72,    73,    74,    75,   132,   155,   133,   132,   132,
      52,   133,   334,    58,   135,   365,   494,    88,   496,    90,
      91,    92,   132,    94,    95,    96,   483,   483,   457,   431,
     457,    70,   543,   526,   105,   106,   107,   504,   109,   110,
     432,   112,   113,   114,   115,   116,   531,   480,   468,   449,
      26,   122,   404,   124,   275,    46,   127,    -1,   426,    -1,
      -1,   244,    -1,    90,    91,    92,    -1,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   498,   498,
     498,   498,   498,    -1,   504,   112,   113,   114,   115,   116,
      -1,   114,   115,   116,    -1,   122,    -1,   124,    -1,   122,
     127,   124,    -1,    -1,   127,    -1,   526,   526,   526,   526,
     526,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   543,   543,   543,   543,   543,     0,    -1,
      -1,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,    39,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,
      42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    66,    67,    68,    -1,
      62,    63,    -1,    -1,    74,    75,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    -1,    94,    95,    96,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   109,
     110,    -1,   112,   113,   114,   115,   116,   109,    -1,    -1,
     112,    -1,   122,    -1,   124,   117,   118,   127,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
     132,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
     132,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
     132,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
     132,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,    39,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,
      42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    74,    75,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    -1,    94,    95,    96,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   109,
     110,    -1,   112,   113,   114,   115,   116,   109,    -1,    -1,
     112,    -1,   122,    -1,   124,   117,   118,   127,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
     132,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,
      42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
     132,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,
      42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
     132,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,
      42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
     132,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,
      42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,    -1,     0,    -1,
     132,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,
      42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,    -1,   128,     3,     4,     5,
       6,    -1,     8,     9,    10,    11,    12,    13,    14,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    -1,    40,    41,    42,    -1,    44,    45,
      46,    47,    48,    -1,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    -1,    65,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,   122,    -1,   124,    -1,
     126,    -1,   128,     3,     4,     5,     6,    -1,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    27,    28,    29,
      -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,    -1,
      40,    41,    42,    -1,    44,    45,    46,    47,    48,    -1,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    -1,    65,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,    -1,   126,     3,   128,     5,
       6,    -1,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    25,
      -1,    27,    28,    -1,    -1,    31,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,     3,    -1,
       5,     6,    -1,     8,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    23,    -1,
      25,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,   109,    -1,    -1,   112,    42,    -1,    44,
      -1,   117,   118,    -1,    -1,    -1,   122,    52,   124,    54,
      55,    -1,   128,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,     3,
      -1,     5,     6,    -1,     8,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    23,
      -1,    25,    -1,    27,    28,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      44,    -1,   117,   118,    -1,    -1,    -1,   122,    52,   124,
      54,    55,    -1,   128,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    -1,    94,    95,    96,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    39,   106,   107,    -1,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,    -1,   127,    -1,   122,    -1,
     124,    66,    67,    68,   128,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,
     105,   106,   107,    39,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,   105,
     106,   107,    39,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      -1,    -1,    -1,    -1,    -1,   102,   103,    -1,   105,   106,
     107,    39,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,   133,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    -1,
      -1,    -1,    -1,    39,   102,   103,   104,   105,   106,   107,
      -1,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,   127,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    -1,    -1,    -1,    -1,    39,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,   127,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    -1,    -1,    -1,    -1,    39,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,   127,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    -1,    -1,    -1,    -1,    39,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,   127,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    -1,    -1,    -1,
      -1,    39,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,   127,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    -1,
      -1,    -1,    -1,    39,   102,   103,   104,   105,   106,   107,
      -1,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,   127,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    -1,    -1,    -1,    -1,    39,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,   127,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    -1,    -1,    -1,    -1,    39,   102,   103,
      -1,   105,   106,   107,    -1,    -1,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,    -1,   127,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    -1,    94,    95,    96,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,   105,   106,   107,    -1,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,    -1,    -1,   127,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    -1,    94,    95,    96,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,    -1,   127,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    -1,    94,    95,    96,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,   127,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    -1,    94,    95,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,   127
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     5,     6,     8,     9,    10,    11,    12,
      13,    14,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    27,    28,    35,    37,    38,    40,    42,    44,    46,
      47,    48,    51,    52,    54,    55,    62,    63,    69,    90,
      91,    99,   109,   112,   117,   118,   122,   124,   126,   128,
     135,   136,   137,   138,   144,   147,   148,   154,   158,   159,
     160,   161,   163,   171,   179,   182,   183,   186,   189,   190,
     191,   192,   196,   198,   207,   212,   213,   214,   219,   227,
     230,   243,   244,   245,   250,   254,   257,   258,   260,   261,
      89,    49,   122,   122,   164,   122,     3,     4,    17,   126,
     184,   185,    16,     3,   254,     3,   229,   254,   122,   185,
     233,   122,   126,   122,   127,   254,   122,     3,     5,   251,
     252,   253,   254,   254,   254,   254,   254,   249,   254,   239,
     240,   241,   247,   254,    29,    31,    36,    41,    45,    65,
      99,   132,   143,   144,   145,   148,   150,   165,   166,   181,
     190,   246,     3,     0,   138,   126,     3,    45,   150,   156,
     157,   250,   145,   146,    16,    25,    30,    31,    43,    45,
     206,   259,   192,     3,   100,    39,    66,    67,    68,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    90,    91,
      92,    94,    95,    96,   102,   103,   105,   106,   107,   109,
     110,   112,   113,   114,   115,   116,   122,   124,   127,     3,
     122,   162,   146,   172,   188,   126,   127,   184,   185,   232,
     233,   234,   242,   122,   248,   249,   127,   254,   180,   239,
       3,   155,   132,   104,   100,   100,   133,   125,   100,   247,
       3,     3,   151,   152,   248,   248,   151,     3,   132,   145,
     126,   136,     3,    99,    67,    99,     3,    34,    50,   226,
       3,     3,   208,     3,   220,   221,   222,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,     3,   115,   128,   256,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     239,   254,     3,     4,   122,   124,   128,   255,   256,   248,
      12,   254,    99,   139,   140,   141,   147,   148,   196,   198,
     207,   227,   187,   184,   127,   122,   238,     3,    93,   223,
     224,   225,   133,   115,   184,   133,   142,   143,   133,   248,
     247,   252,   254,   104,   237,   100,   143,   132,   237,   248,
     248,   102,     3,    53,   193,    53,   194,     3,   209,   210,
     102,   110,   114,   215,   222,   124,   256,   104,   133,   125,
     254,   254,   124,   256,    89,   133,   122,   133,   132,   141,
     126,   139,   239,   102,   104,   211,   225,   133,   100,   132,
     133,   104,    35,   115,   234,   236,   102,   149,   152,   132,
      99,   133,     3,     5,   122,   234,    15,   195,   234,   235,
     126,   237,   100,     5,   216,   217,   254,   254,   133,   125,
     254,   256,   146,   248,   126,   139,   132,   133,   254,   236,
     237,    93,   225,    26,    32,   173,   175,   177,   178,   146,
     247,   254,   250,   146,   223,   235,   126,   100,   199,   149,
     210,   110,     5,   109,   110,   218,   219,   125,   125,   133,
      33,    61,   167,   168,   169,   170,   132,   211,   126,   225,
     126,   122,   173,   175,    64,   153,   133,   133,   197,   234,
      45,    99,   190,   203,   204,   205,   114,   109,   219,   217,
     254,   104,   132,   169,   170,   231,   176,     3,   146,   146,
     237,    99,   144,   148,   190,   200,   201,   202,   207,   227,
       3,    25,   132,   205,     3,   114,   217,   104,   142,   142,
     142,   237,   126,   126,   132,   202,   226,   216,     3,   142,
     132,   132,   133,   228,   200,     3,   110,   216,   174,   142,
     132,   122,   110,   126,   132,   223,   142,   133,   132,   237
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
#line 1869 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 24:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1881 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 26:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1884 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1885 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1887 "parser.y"
    {
    (yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));
}
    }
    break;


  
    case 29:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1890 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 42:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1905 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 43:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1906 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 44:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1909 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 51:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1916 "parser.y"
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


  
    case 52:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1928 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1929 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1933 "parser.y"
    {
    code_t**cc = &global->init->method->body->code;
    *cc = code_append(*cc, (yyvsp[(1) - (1)].code));
}
    }
    break;


  
    case 55:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1940 "parser.y"
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


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1959 "parser.y"
    {(yyval.node)=(yyvsp[(2) - (2)].node);}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1960 "parser.y"
    {(yyval.node)=mkdummynode();}
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1962 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1963 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1965 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1966 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1969 "parser.y"
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


  
    case 63:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2028 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2029 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2032 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2032 "parser.y"
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


  
    case 67:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2051 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2058 "parser.y"
    {
    PASS1 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),0,1,0);
    PASS2 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
}
    }
    break;


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2062 "parser.y"
    {
    PASS12
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 72:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2067 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 73:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2068 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2070 "parser.y"
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


  
    case 75:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2090 "parser.y"
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


  
    case 76:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2138 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2138 "parser.y"
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


  
    case 78:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2157 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2157 "parser.y"
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


  
    case 80:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2172 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2175 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2178 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2181 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2185 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2186 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2187 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2188 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2189 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2190 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2192 "parser.y"
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


  
    case 91:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2203 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2206 "parser.y"
    {PASS12 new_state();state->switch_var=alloc_local();}
    }
    break;


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2206 "parser.y"
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


  
    case 94:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2237 "parser.y"
    {PASS12 new_state();
                                                      state->exception_name=(yyvsp[(3) - (5)].id);
                                               PASS1 new_variable((yyvsp[(3) - (5)].id), 0, 0, 0);
                                               PASS2 new_variable((yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0, 0);
                                              }
    }
    break;


  
    case 95:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2242 "parser.y"
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


  
    case 96:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2262 "parser.y"
    {PASS12 new_state();state->exception_name=0;}
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2262 "parser.y"
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


  
    case 98:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2278 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2279 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2280 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2281 "parser.y"
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


  
    case 102:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2289 "parser.y"
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


  
    case 103:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2298 "parser.y"
    {PASS12 new_state();
                 state->method->has_exceptions=1;
                 state->method->late_binding=1;//for invariant scope_code
                }
    }
    break;


  
    case 104:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2301 "parser.y"
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


  
    case 105:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2347 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2351 "parser.y"
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


  
    case 107:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2362 "parser.y"
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


  
    case 108:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2372 "parser.y"
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


  
    case 110:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2390 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 111:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2391 "parser.y"
    {PASS12 (yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2393 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2394 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2396 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2397 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2398 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 117:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2399 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 118:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2421 "parser.y"
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


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2435 "parser.y"
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


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2451 "parser.y"
    {PASS12 (yyval.flags).flags=0;(yyval.flags).ns=0;}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2452 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2453 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2454 "parser.y"
    {
    PASS12 
    (yyval.flags).flags=(yyvsp[(1) - (2)].flags).flags|(yyvsp[(2) - (2)].flags).flags;
    if((yyvsp[(1) - (2)].flags).ns && (yyvsp[(2) - (2)].flags).ns) syntaxerror("only one namespace allowed in one declaration");
    (yyval.flags).ns=(yyvsp[(1) - (2)].flags).ns?(yyvsp[(1) - (2)].flags).ns:(yyvsp[(2) - (2)].flags).ns;

}
    }
    break;


  
    case 124:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2461 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PUBLIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2462 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PRIVATE;(yyval.flags).ns=0;}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2463 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PROTECTED;(yyval.flags).ns=0;}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2464 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_STATIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2465 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_DYNAMIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2466 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_FINAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2467 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_OVERRIDE;(yyval.flags).ns=0;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2468 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NATIVE;(yyval.flags).ns=0;}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2469 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PACKAGEINTERNAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2470 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NAMESPACE;
                               (yyval.flags).ns=(yyvsp[(1) - (1)].id);
                       }
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2474 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2475 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2477 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2478 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2480 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2481 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2485 "parser.y"
    {PASS12 startclass(&(yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2487 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 142:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2491 "parser.y"
    {PASS12 (yyvsp[(1) - (5)].flags).flags|=FLAG_INTERFACE;
                                          startclass(&(yyvsp[(1) - (5)].flags),(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 143:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2494 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 149:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2503 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 152:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2507 "parser.y"
    {
    code_t*c = state->cls->static_init->header;
    c = code_append(c, (yyvsp[(1) - (1)].code));  
    state->cls->static_init->header = c;
}
    }
    break;


  
    case 158:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2518 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 159:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2521 "parser.y"
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


  
    case 162:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2589 "parser.y"
    {PASS12 setslotstate(&(yyvsp[(1) - (2)].flags),(yyvsp[(2) - (2)].token));}
    }
    break;


  
    case 163:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2589 "parser.y"
    {PASS12 (yyval.code)=(yyvsp[(4) - (4)].code);setslotstate(0, 0);}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2591 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 165:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2592 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 166:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2595 "parser.y"
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
                    c = abc_setproperty2(c, &mname);
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


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2678 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2679 "parser.y"
    {
  (yyval.constant) = malloc(sizeof(constant_t));
  *(yyval.constant) = node_eval((yyvsp[(2) - (2)].node));
  if((yyval.constant)->type == CONSTANT_UNKNOWN) {
    syntaxerror("can't evaluate default parameter value (needs to be a compile-time constant)");
  }
}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2688 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2689 "parser.y"
    {
    (yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));
}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2692 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2693 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2694 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2695 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2696 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2697 "parser.y"
    {(yyval.constant) = constant_new_undefined((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2698 "parser.y"
    {(yyval.constant) = constant_new_float(__builtin_nan(""));}
    }
    break;


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2718 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xml();xml_level++;}
    }
    break;


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2719 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2720 "parser.y"
    {PASS_ALWAYS if(!--xml_level) tokenizer_end_xml(); else tokenizer_begin_xmltext();}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2722 "parser.y"
    {(yyval.id)="";}
    }
    break;


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2723 "parser.y"
    {(yyval.id)=concat2((yyvsp[(1) - (2)].id), string_cstr(&(yyvsp[(2) - (2)].str)));}
    }
    break;


  
    case 184:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2724 "parser.y"
    {(yyval.id)=concat2((yyvsp[(1) - (2)].id), ">");}
    }
    break;


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2726 "parser.y"
    {(yyval.id)=concat2((yyvsp[(1) - (2)].id),(yyvsp[(2) - (2)].id));}
    }
    break;


  
    case 186:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2727 "parser.y"
    {(yyval.id)=concat3((yyvsp[(1) - (3)].id),(yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));free((yyvsp[(2) - (3)].id));free((yyvsp[(3) - (3)].id));}
    }
    break;


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2729 "parser.y"
    {
    (yyval.id) = allocprintf("<%s%s>%s</%s>", (yyvsp[(2) - (10)].id), (yyvsp[(3) - (10)].id), (yyvsp[(5) - (10)].id), (yyvsp[(8) - (10)].id));
    free((yyvsp[(2) - (10)].id));free((yyvsp[(3) - (10)].id));free((yyvsp[(5) - (10)].id));free((yyvsp[(8) - (10)].id));
}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2733 "parser.y"
    {
    (yyval.id) = allocprintf("<%s%s/>", (yyvsp[(2) - (6)].id), (yyvsp[(3) - (6)].id));
}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2736 "parser.y"
    {
    (yyval.id) = allocprintf("<%s%s>%s%s</%s>", (yyvsp[(2) - (11)].id), (yyvsp[(3) - (11)].id), (yyvsp[(5) - (11)].id), (yyvsp[(6) - (11)].id), (yyvsp[(9) - (11)].id));
    free((yyvsp[(2) - (11)].id));free((yyvsp[(3) - (11)].id));free((yyvsp[(5) - (11)].id));free((yyvsp[(6) - (11)].id));free((yyvsp[(6) - (11)].id));free((yyvsp[(9) - (11)].id));
}
    }
    break;


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2741 "parser.y"
    {(yyval.id)=strdup("");}
    }
    break;


  
    case 191:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2742 "parser.y"
    {(yyval.id)=concat2(" ",(yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 192:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2743 "parser.y"
    {(yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 193:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2744 "parser.y"
    {(yyval.id)=concat3((yyvsp[(1) - (2)].id)," ",(yyvsp[(2) - (2)].id));free((yyvsp[(1) - (2)].id));free((yyvsp[(2) - (2)].id));}
    }
    break;


  
    case 194:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2745 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(3) - (3)].str));
    (yyval.id)=allocprintf("%s=\"%s\"", (yyvsp[(1) - (3)].id),str);
    free(str);
    free((yyvsp[(1) - (3)].id));free((char*)(yyvsp[(3) - (3)].str).str);
}
    }
    break;


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2755 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2759 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2765 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(2) - (2)].param));
}
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2771 "parser.y"
    {
    PASS12
    (yyval.params) =(yyvsp[(1) - (4)].params);
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(4) - (4)].param));
}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2779 "parser.y"
    {
    PASS12
    (yyval.params) = (yyvsp[(1) - (3)].params);
    list_append((yyval.params).list, (yyvsp[(3) - (3)].param));
}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2784 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    list_append((yyval.params).list, (yyvsp[(1) - (1)].param));
}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2790 "parser.y"
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


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2798 "parser.y"
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


  
    case 205:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2808 "parser.y"
    {PASS12 (yyval.token)=0;}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2811 "parser.y"
    {PASS12 startfunction(&(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2812 "parser.y"
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


  
    case 209:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2828 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2830 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 211:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2831 "parser.y"
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


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2853 "parser.y"
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


  
    case 213:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2872 "parser.y"
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


  
    case 216:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2889 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 217:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2890 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 218:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2892 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 219:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2893 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_ANY;}
    }
    break;


  
    case 220:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2894 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_VOID;}
    }
    break;


  
    case 221:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2903 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2904 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2908 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 224:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2909 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 225:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2911 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 228:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2915 "parser.y"
    {(yyval.value_list).number=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 229:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2919 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 230:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2920 "parser.y"
    {
                                                  (yyval.value_list).number= (yyvsp[(1) - (2)].value_list).number+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2926 "parser.y"
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
    } else if((yyval.value).c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)(yyval.value).c->data[0];
        trait_t*t = traits_find_slotid(state->cls->abc->traits,slot);//FIXME
        multiname_t*name = t->name;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_constructprop2((yyval.value).c, name, (yyvsp[(4) - (4)].value_list).number);
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


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2963 "parser.y"
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
    } else if((yyval.value).c->opcode == OPCODE_GETSLOT && (yyval.value).c->prev->opcode != OPCODE_GETSCOPEOBJECT) {
        int slot = (int)(ptroff_t)(yyval.value).c->data[0];
        trait_t*t = traits_find_slotid(state->cls->abc->traits,slot);
        if(t->kind!=TRAIT_METHOD) {
            //ok: flash allows to assign closures to members.
        }
        multiname_t*name = t->name;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        //$$.c = abc_callmethod($$.c, t->method, len); //#1051 illegal early access binding
        (yyval.value).c = abc_callproperty2((yyval.value).c, name, (yyvsp[(3) - (4)].value_list).number);
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


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3013 "parser.y"
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


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3034 "parser.y"
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


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3056 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3059 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3066 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3069 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3072 "parser.y"
    {
    (yyval.node) = mkmultinode(&node_comma, (yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3075 "parser.y"
    {
    (yyval.node) = multinode_extend((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
}
    }
    break;


  
    case 242:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3078 "parser.y"
    { 
    (yyval.code) = node_exec((yyvsp[(1) - (1)].node)); 
}
    }
    break;


  
    case 243:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3081 "parser.y"
    { 
    (yyval.code) = (yyvsp[(1) - (3)].code);
    (yyval.code) = code_append((yyval.code), node_exec((yyvsp[(3) - (3)].node))); 
}
    }
    break;


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3086 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3087 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3089 "parser.y"
    {(yyval.code)=abc_pushstring(0,(yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 247:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3090 "parser.y"
    {(yyval.code)=abc_pushstring2(0,&(yyvsp[(1) - (1)].str));}
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3092 "parser.y"
    {
    (yyval.value_list).cc = 0;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(1) - (3)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (3)].value).c);
    (yyval.value_list).number = 2;
}
    }
    break;


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3098 "parser.y"
    {
    (yyval.value_list).cc = (yyvsp[(1) - (5)].value_list).cc;
    (yyval.value_list).number = (yyvsp[(1) - (5)].value_list).number+2;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (5)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(5) - (5)].value).c);
}
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3107 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3108 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3109 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3110 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3111 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3112 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3114 "parser.y"
    { 
    (yyval.node) = mkconstnode((yyvsp[(1) - (1)].constant));
}
    }
    break;


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3118 "parser.y"
    {
    typedcode_t v;
    v.c = 0;
    namespace_t ns = {ACCESS_PACKAGE, ""};
    multiname_t m = {QNAME, &ns, 0, "XML"};
    v.c = abc_getlex2(v.c, &m);
    v.c = abc_pushstring(v.c, (yyvsp[(1) - (1)].id));
    v.c = abc_construct(v.c, 1);
    v.t = TYPE_XML;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3131 "parser.y"
    {
    typedcode_t v;
    v.c = 0;
    namespace_t ns = {ACCESS_PACKAGE, ""};
    multiname_t m = {QNAME, &ns, 0, "RegExp"};
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


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3151 "parser.y"
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


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3161 "parser.y"
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


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3170 "parser.y"
    {(yyval.node) = mknode2(&node_lt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3171 "parser.y"
    {(yyval.node) = mknode2(&node_gt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3172 "parser.y"
    {(yyval.node) = mknode2(&node_le,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3173 "parser.y"
    {(yyval.node) = mknode2(&node_ge,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3174 "parser.y"
    {(yyval.node) = mknode2(&node_eqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3175 "parser.y"
    {(yyval.node) = mknode2(&node_eqeqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3176 "parser.y"
    {(yyval.node) = mknode2(&node_noteqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3177 "parser.y"
    {(yyval.node) = mknode2(&node_noteq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 269:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3178 "parser.y"
    {(yyval.node) = mknode2(&node_oror,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3179 "parser.y"
    {(yyval.node) = mknode2(&node_andand,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3180 "parser.y"
    {(yyval.node) = mknode1(&node_not, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3181 "parser.y"
    {(yyval.node) = mknode1(&node_bitnot, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3182 "parser.y"
    {(yyval.node) = mknode2(&node_bitand, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3183 "parser.y"
    {(yyval.node) = mknode2(&node_bitxor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3184 "parser.y"
    {(yyval.node) = mknode2(&node_bitor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3185 "parser.y"
    {(yyval.node) = mknode2(&node_shr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3186 "parser.y"
    {(yyval.node) = mknode2(&node_ushr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 278:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3187 "parser.y"
    {(yyval.node) = mknode2(&node_shl, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 279:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3188 "parser.y"
    {(yyval.node) = mknode2(&node_div, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 280:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3189 "parser.y"
    {(yyval.node) = mknode2(&node_mod, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 281:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3190 "parser.y"
    {(yyval.node) = mknode2(&node_plus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3191 "parser.y"
    {(yyval.node) = mknode2(&node_minus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3192 "parser.y"
    {(yyval.node) = mknode2(&node_multiply, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3193 "parser.y"
    {(yyval.node) = mknode2(&node_in, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3194 "parser.y"
    {(yyval.node) = mknode2(&node_as, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 286:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3195 "parser.y"
    {(yyval.node) = mknode2(&node_instanceof, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 287:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3196 "parser.y"
    {(yyval.node) = mknode2(&node_is, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3197 "parser.y"
    {(yyval.node) = mknode1(&node_typeof, (yyvsp[(3) - (4)].node));}
    }
    break;


  
    case 289:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3198 "parser.y"
    {(yyval.node) = mknode1(&node_void, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 290:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3199 "parser.y"
    { (yyval.node) = mkconstnode(constant_new_undefined());}
    }
    break;


  
    case 291:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3200 "parser.y"
    { (yyval.node)=(yyvsp[(2) - (3)].node);}
    }
    break;


  
    case 292:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3201 "parser.y"
    {(yyval.node) = mknode1(&node_neg, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 293:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3202 "parser.y"
    {(yyval.node) = mknode2(&node_arraylookup, (yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node));}
    }
    break;


  
    case 294:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3203 "parser.y"
    {(yyval.node) = mknode2(&node_muleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 295:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3204 "parser.y"
    {(yyval.node) = mknode2(&node_modeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 296:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3205 "parser.y"
    {(yyval.node) = mknode2(&node_shleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 297:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3206 "parser.y"
    {(yyval.node) = mknode2(&node_shreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 298:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3207 "parser.y"
    {(yyval.node) = mknode2(&node_ushreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 299:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3208 "parser.y"
    { (yyval.node) = mknode2(&node_diveq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 300:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3209 "parser.y"
    { (yyval.node) = mknode2(&node_bitoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 301:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3210 "parser.y"
    { (yyval.node) = mknode2(&node_bitxoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 302:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3211 "parser.y"
    { (yyval.node) = mknode2(&node_bitandeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 303:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3212 "parser.y"
    { (yyval.node) = mknode2(&node_pluseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 304:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3213 "parser.y"
    { (yyval.node) = mknode2(&node_minuseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 305:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3214 "parser.y"
    { (yyval.node) = mknode2(&node_assign, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 306:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3215 "parser.y"
    { (yyval.node) = mknode3(&node_tenary, (yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node));}
    }
    break;


  
    case 307:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3217 "parser.y"
    { (yyval.node) = mknode1(&node_rplusplus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 308:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3218 "parser.y"
    { (yyval.node) = mknode1(&node_rminusminus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 309:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3219 "parser.y"
    {(yyval.node) = mknode1(&node_lplusplus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 310:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3220 "parser.y"
    {(yyval.node) = mknode1(&node_lminusminus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 311:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3223 "parser.y"
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


  
    case 312:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3237 "parser.y"
    {
    // attribute occuring in .() loops 
    // TODO
    (yyval.node) = mkdummynode();
    as3_warning("ignored @ operator");
}
    }
    break;


  
    case 313:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3244 "parser.y"
    {
    // filter 
    // TODO: this needs to be implemented using a loop
    (yyval.node) = mkdummynode();
    as3_warning("ignored .() operator");
}
    }
    break;


  
    case 314:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3251 "parser.y"
    {(yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 315:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3252 "parser.y"
    {(yyval.id)=(char*)(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 317:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3254 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 318:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3256 "parser.y"
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


  
    case 319:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3274 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (3)].node));
    multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 320:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3281 "parser.y"
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


  
    case 321:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3292 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getproperty2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 322:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3299 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 323:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3306 "parser.y"
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


  
    case 324:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3316 "parser.y"
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


  
    case 325:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3327 "parser.y"
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
        as3_warning("Resolving %s on unknown type", (yyvsp[(3) - (3)].id));
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
        (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_ANY;
    }
}
    }
    break;


  
    case 326:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3497 "parser.y"
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


  
    case 327:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3532 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (2)].id);
    n->url = (yyvsp[(2) - (2)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 328:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3539 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 329:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3546 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].str).str;
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 330:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3553 "parser.y"
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


  
    case 331:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3575 "parser.y"
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
#line 8357 "parser.tab.c"
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



