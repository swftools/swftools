
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
#include "ast.h"

extern int a3_lex();



/* Line 198 of skeleton.m4  */
#line 101 "parser.tab.c"

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
     T_PLUSBY = 335,
     T_MINUSBY = 336,
     T_XORBY = 337,
     T_SHRBY = 338,
     T_SHLBY = 339,
     T_USHRBY = 340,
     T_OROR = 341,
     T_ANDAND = 342,
     T_COLONCOLON = 343,
     T_MINUSMINUS = 344,
     T_PLUSPLUS = 345,
     T_DOTDOT = 346,
     T_DOTDOTDOT = 347,
     T_SHL = 348,
     T_USHR = 349,
     T_SHR = 350,
     prec_none = 351,
     below_semicolon = 352,
     below_assignment = 353,
     below_minus = 355,
     minusminus_prefix = 356,
     plusplus_prefix = 357,
     below_curly = 358,
     new2 = 359,
     above_identifier = 360,
     below_else = 361,
     above_function = 362
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union 
/* Line 223 of skeleton.m4  */
#line 46 "parser.y"
tokenunion
{

/* Line 223 of skeleton.m4  */
#line 46 "parser.y"

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
    struct {
        abc_exception_list_t *l;
        code_t*finally;
    } catch_list;



/* Line 223 of skeleton.m4  */
#line 279 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 266 "parser.y"


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

static dict_t*definitions=0;
void as3_set_definition(const char*c)
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

typedef struct _variable {
    int index;
    classinfo_t*type;
    char init;
    char is_parameter;
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
 
    if(name) 
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

        /* fill out interfaces and extends (we couldn't resolve those during the first pass) */
        state->cls->info->superclass = extends;

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
        minfo->return_type = 0; // save this for pass 2
    } else if(getset != KW_GET && getset != KW_SET) {
        //class method
        memberinfo_t* m = registry_findmember(state->cls->info, ns.name, name, 0);
        if(m) {
            syntaxerror("class already contains a %s '%s'", infotypename((slotinfo_t*)m), m->name);
        }
        minfo = methodinfo_register_onclass(state->cls->info, ns.access, ns.name, name);
        minfo->return_type = 0; // save this for pass 2 
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
            if(minfo->kind!=INFOTYPE_SLOT)
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
            minfo->kind = INFOTYPE_SLOT; //hack
            minfo->subtype = gs;
            minfo->return_type = 0;
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
            namespace_t mname_ns = modifiers2access(mod);
            multiname_t mname = {QNAME, &mname_ns, 0, name};

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

/* TODO: move this to ast.c */
#define IS_INT(a) (TYPE_IS_INT((a)) || TYPE_IS_UINT((a)))
#define IS_NUMBER_OR_INT(a) (TYPE_IS_INT((a)) || TYPE_IS_UINT((a)) || TYPE_IS_NUMBER((a)))
#define BOTH_INT(a,b) (IS_INT(a) && IS_INT(b))
static classinfo_t*join_types(classinfo_t*type1, classinfo_t*type2, char op)
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


code_t* toreadwrite(code_t*in, code_t*middlepart, char justassign, char readbefore, char pushvalue)
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
    } else if(r->opcode == OPCODE_GETSUPER) { 
        write->opcode = OPCODE_SETSUPER;
        multiname_t*m = (multiname_t*)r->data[0];
        write->data[0] = multiname_clone(m);
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
            if(pushvalue && readbefore) {
                c = abc_dup(c);
                c = abc_setlocal(c, temp);
            }
            c = code_append(c, middlepart);
            if(pushvalue && !readbefore) {
                c = abc_dup(c);
                c = abc_setlocal(c, temp);
            }
            c = code_append(c, write);
            if(pushvalue) {
                c = abc_getlocal(c, temp);
                c = abc_kill(c, temp);
            }
        } else {
            /* if we're allowed to execute the read code twice *and*
               the middlepart doesn't modify the code, things are easier.
            */
            //c = code_append(c, prefix);
            parserassert(!prefix);
            code_t* r2 = 0;
            if(pushvalue) {
                r2 = code_dup(r);
            }
            c = code_append(c, r);
            c = code_append(c, middlepart);
            c = code_append(c, write);
            if(pushvalue) {
                c = code_append(c, r2);
            }
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
            if(pushvalue) {
                c = code_append(c, r);
            }
        } else {
            code_free(r);r=0;
            temp = gettempvar();
            if(prefix) {
                c = code_append(c, prefix);
            }
            c = code_append(c, middlepart);
            if(pushvalue) {
                c = abc_dup(c);
                c = abc_setlocal(c, temp);
            }
            c = code_append(c, write);
            if(pushvalue) {
                c = abc_getlocal(c, temp);
                c = abc_kill(c, temp);
            }
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
#line 2096 "parser.tab.c"
/* Unqualified %code blocks.  */

/* Line 274 of skeleton.m4  */
#line 2159 "parser.y"

    char is_subtype_of(classinfo_t*type, classinfo_t*supertype)
    {
        return 1; // FIXME
    }


/* Line 274 of skeleton.m4  */
#line 2611 "parser.y"

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
#line 2744 "parser.y"

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


/* Line 274 of skeleton.m4  */
#line 3885 "parser.y"

    void add_active_url(const char*url)
    {
        NEW(namespace_t,n);
        n->name = url;
        list_append(state->active_namespace_urls, n);
    }



/* Line 274 of skeleton.m4  */
#line 2165 "parser.tab.c"

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
#define YYFINAL  146
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3050

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  113
/* YYNRULES -- Number of rules.  */
#define YYNRULES  299
/* YYNRULES -- Number of states.  */
#define YYNSTATES  511

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   362

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   117,     2,     2,     2,   115,   107,     2,
     121,   132,   114,   112,    99,   111,   126,   113,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   104,    98,
     108,   101,   109,   103,   127,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   123,     2,   124,   106,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   125,   105,   131,   116,     2,     2,     2,
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
      95,    96,    97,   100,   102,   110,   118,   119,   120,   122,
     128,   129,   130
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
     468,   470,   472,   474,   476,   478,   480,   482,   484,   485,
     487,   490,   495,   499,   501,   506,   509,   511,   513,   514,
     515,   528,   530,   531,   532,   543,   545,   549,   551,   553,
     555,   559,   561,   563,   565,   568,   569,   570,   574,   575,
     577,   579,   581,   584,   587,   588,   593,   598,   603,   606,
     608,   611,   613,   615,   619,   621,   623,   625,   627,   629,
     631,   633,   635,   639,   643,   647,   651,   655,   659,   663,
     667,   671,   675,   678,   681,   685,   689,   693,   697,   701,
     705,   709,   713,   717,   721,   725,   729,   733,   737,   741,
     746,   749,   751,   755,   758,   763,   767,   768,   770,   774,
     780,   784,   788,   792,   796,   800,   804,   808,   812,   816,
     820,   824,   828,   834,   837,   840,   843,   846,   850,   853,
     858,   864,   868,   874,   878,   880,   883,   888,   893,   896
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     134,     0,    -1,   135,    -1,    -1,   136,    -1,   137,    -1,
     136,   137,    -1,   185,    -1,   197,    -1,   195,    -1,   216,
      -1,   206,    -1,   146,    -1,   147,   125,   135,   131,    -1,
      98,    -1,    -1,   139,    -1,   140,    -1,   139,   140,    -1,
     197,    -1,   195,    -1,   216,    -1,   206,    -1,   146,    -1,
     147,   125,   138,   131,    -1,    98,    -1,   142,    -1,    -1,
     142,   144,    -1,   144,    -1,   188,    -1,   158,    -1,   159,
      -1,   160,    -1,   162,    -1,   170,    -1,   153,    -1,   182,
      -1,   178,    -1,   238,    -1,   245,    -1,   244,    -1,   125,
     142,   131,    -1,   125,   131,    -1,    98,    -1,   143,    -1,
     149,    -1,   164,    -1,   165,    -1,   235,    -1,   180,    -1,
     147,   125,   142,   131,    -1,   144,    98,    -1,   144,    -1,
     143,    -1,     3,    88,     3,    -1,   101,   236,    -1,    -1,
      45,   150,    -1,    31,   150,    -1,   151,    -1,   150,    99,
     151,    -1,     3,   226,   148,    -1,    -1,    64,   145,    -1,
      -1,    63,   121,   154,   237,   132,   145,   152,    -1,    -1,
     149,    -1,   238,    -1,    45,     3,   226,    -1,     3,    -1,
      11,   121,    -1,    11,    49,   121,    -1,   157,   155,    98,
     237,    98,   238,   132,   145,    -1,   157,   156,    67,   237,
     132,   145,    -1,    -1,    12,   121,   161,   237,   132,   145,
      -1,    -1,    13,   163,   145,    12,   121,   237,   132,    -1,
      65,    -1,    65,     3,    -1,    29,    -1,    29,     3,    -1,
      -1,   167,    -1,   169,    -1,   167,   169,    -1,   168,    -1,
     167,   168,    -1,    33,   239,   104,   141,    -1,    61,   104,
     141,    -1,    -1,    14,   121,   171,   239,   132,   125,   166,
     131,    -1,    -1,    32,   121,     3,   226,   132,   173,   125,
     141,   131,    -1,    -1,    26,   125,   175,   141,   131,    -1,
     172,    -1,   176,   172,    -1,   176,    -1,   176,   174,    -1,
     174,    -1,    -1,    51,   125,   179,   141,   131,   177,    -1,
      36,   237,    -1,    36,    -1,    38,   121,   237,   132,    -1,
     181,   145,    -1,     3,    -1,    17,    -1,     4,    -1,   184,
     126,   183,    -1,   183,    -1,    -1,    17,   184,   125,   186,
     138,   131,    -1,    -1,    17,   125,   187,   138,   131,    -1,
      40,   222,    -1,    40,   184,   126,   114,    -1,    -1,   190,
      -1,   191,    -1,   190,   191,    -1,    19,    -1,    20,    -1,
      18,    -1,    37,    -1,    46,    -1,    48,    -1,    47,    -1,
      24,    -1,    22,    -1,     4,    -1,    -1,    53,   223,    -1,
      -1,    53,   224,    -1,    -1,    15,   224,    -1,    -1,   189,
      30,     3,   192,   194,   125,   196,   199,   131,    -1,    -1,
     189,    43,     3,   193,   125,   198,   202,   131,    -1,    -1,
     200,    -1,   201,    -1,   200,   201,    -1,    98,    -1,   147,
     125,   199,   131,    -1,   206,    -1,   216,    -1,   143,    -1,
      -1,   203,    -1,   204,    -1,   203,   204,    -1,    98,    -1,
      45,     3,    -1,   189,    25,   215,     3,   121,   212,   132,
     226,    -1,    45,    -1,    31,    -1,    -1,   189,   205,   207,
     208,    -1,   209,    -1,   208,    99,   209,    -1,     3,   226,
     148,    -1,    -1,   101,   211,    -1,     8,    -1,     9,    -1,
      10,    -1,     5,    -1,    55,    -1,    54,    -1,    44,    -1,
      27,    -1,    28,    -1,    -1,   213,    -1,    92,   214,    -1,
     213,    99,    92,   214,    -1,   213,    99,   214,    -1,   214,
      -1,     3,   104,   225,   210,    -1,     3,   210,    -1,    50,
      -1,    34,    -1,    -1,    -1,   189,    25,   215,     3,   121,
     212,   132,   226,   125,   217,   141,   131,    -1,     3,    -1,
      -1,    -1,    25,   218,   121,   212,   132,   226,   125,   220,
     141,   131,    -1,   183,    -1,   184,   126,   183,    -1,   222,
      -1,   221,    -1,   223,    -1,   224,    99,   223,    -1,   223,
      -1,   114,    -1,    35,    -1,   104,   225,    -1,    -1,    -1,
     121,   228,   132,    -1,    -1,   229,    -1,   230,    -1,   236,
      -1,   229,    99,    -1,   230,   236,    -1,    -1,    23,   239,
     231,   227,    -1,   239,   121,   228,   132,    -1,    52,   121,
     228,   132,    -1,    62,   239,    -1,    41,    -1,    41,   237,
      -1,   239,    -1,   239,    -1,   237,    99,   239,    -1,   237,
      -1,   219,    -1,   242,    -1,   232,    -1,   234,    -1,   233,
      -1,   211,    -1,     6,    -1,   239,   108,   239,    -1,   239,
     109,   239,    -1,   239,    74,   239,    -1,   239,    75,   239,
      -1,   239,    70,   239,    -1,   239,    71,   239,    -1,   239,
      73,   239,    -1,   239,    72,   239,    -1,   239,    86,   239,
      -1,   239,    87,   239,    -1,   117,   239,    -1,   116,   239,
      -1,   239,   107,   239,    -1,   239,   106,   239,    -1,   239,
     105,   239,    -1,   239,    95,   239,    -1,   239,    94,   239,
      -1,   239,    93,   239,    -1,   239,   113,   239,    -1,   239,
     115,   239,    -1,   239,   112,   239,    -1,   239,   111,   239,
      -1,   239,   114,   239,    -1,   239,    67,   239,    -1,   239,
      68,   239,    -1,   239,    39,   239,    -1,   239,    66,   239,
      -1,    42,   121,   239,   132,    -1,    35,   239,    -1,    35,
      -1,   121,   237,   132,    -1,   111,   239,    -1,   239,   123,
     239,   124,    -1,   123,   228,   124,    -1,    -1,   241,    -1,
     236,   104,   236,    -1,   241,    99,   236,   104,   236,    -1,
      69,   240,   131,    -1,   239,    79,   239,    -1,   239,    78,
     239,    -1,   239,    84,   239,    -1,   239,    83,   239,    -1,
     239,    85,   239,    -1,   239,    77,   239,    -1,   239,    76,
     239,    -1,   239,    82,   239,    -1,   239,    80,   239,    -1,
     239,    81,   239,    -1,   239,   101,   239,    -1,   239,   103,
     239,   104,   239,    -1,   239,    90,    -1,   239,    89,    -1,
      90,   239,    -1,    89,   239,    -1,    52,   126,     3,    -1,
     127,     3,    -1,   239,   126,   127,     3,    -1,   239,   126,
       3,    88,     3,    -1,   239,    91,     3,    -1,   239,   126,
     121,   239,   132,    -1,   239,   126,     3,    -1,     3,    -1,
      16,     3,    -1,    16,     3,   101,     3,    -1,    16,     3,
     101,     5,    -1,   189,   243,    -1,    21,    16,   223,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  2074,  2074,  2076,  2076,  2077,  2078,  2080,  2081,  2082,
    2083,  2084,  2085,  2086,  2087,  2089,  2089,  2090,  2091,  2093,
    2094,  2095,  2096,  2097,  2098,  2099,  2101,  2102,  2104,  2105,
    2108,  2109,  2110,  2111,  2112,  2113,  2114,  2115,  2116,  2117,
    2118,  2119,  2120,  2121,  2124,  2125,  2126,  2127,  2128,  2129,
    2130,  2131,  2135,  2136,  2140,  2147,  2166,  2167,  2171,  2172,
    2174,  2175,  2177,  2238,  2239,  2242,  2242,  2261,  2262,  2263,
    2268,  2272,  2277,  2278,  2280,  2300,  2348,  2348,  2367,  2367,
    2382,  2385,  2388,  2391,  2395,  2396,  2397,  2398,  2399,  2400,
    2402,  2413,  2416,  2416,  2447,  2447,  2472,  2472,  2488,  2489,
    2490,  2491,  2499,  2508,  2508,  2557,  2561,  2572,  2582,  2599,
    2600,  2601,  2603,  2604,  2606,  2606,  2608,  2608,  2631,  2645,
    2661,  2662,  2663,  2664,  2671,  2672,  2673,  2674,  2675,  2676,
    2677,  2678,  2679,  2680,  2684,  2685,  2687,  2688,  2690,  2691,
    2695,  2693,  2701,  2699,  2708,  2709,  2710,  2711,  2712,  2713,
    2714,  2715,  2717,  2723,  2724,  2725,  2726,  2727,  2728,  2731,
    2763,  2763,  2765,  2765,  2767,  2768,  2770,  2855,  2856,  2859,
    2860,  2863,  2864,  2865,  2866,  2867,  2868,  2869,  2884,  2888,
    2894,  2900,  2908,  2913,  2919,  2927,  2935,  2936,  2937,  2940,
    2939,  2956,  2957,  2959,  2958,  2982,  3001,  3015,  3016,  3018,
    3019,  3021,  3022,  3023,  3032,  3033,  3037,  3038,  3040,  3041,
    3042,  3044,  3048,  3049,  3054,  3055,  3091,  3137,  3158,  3179,
    3182,  3189,  3190,  3191,  3197,  3203,  3204,  3205,  3206,  3208,
    3210,  3215,  3232,  3235,  3238,  3241,  3244,  3247,  3250,  3253,
    3257,  3268,  3286,  3291,  3296,  3301,  3306,  3311,  3315,  3319,
    3324,  3328,  3332,  3341,  3350,  3360,  3365,  3377,  3383,  3388,
    3394,  3400,  3404,  3406,  3417,  3426,  3433,  3434,  3436,  3442,
    3451,  3458,  3470,  3476,  3482,  3488,  3494,  3500,  3506,  3512,
    3525,  3536,  3543,  3556,  3583,  3597,  3611,  3625,  3640,  3647,
    3654,  3661,  3668,  3683,  3740,  3849,  3856,  3863,  3870,  3894
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
  "\">=\"", "\"|=\"", "\"/=\"", "\"%=\"", "\"*=\"", "\"+=\"", "\"-=\"",
  "\"^=\"", "\">>=\"", "\"<<=\"", "\">>>=\"", "\"||\"", "\"&&\"", "\"::\"",
  "\"--\"", "\"++\"", "\"..\"", "\"...\"", "\"<<\"", "\">>>\"", "\">>\"",
  "prec_none", "below_semicolon", "';'", "','", "below_assignment", "'='",
  "\"&=\"", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "below_minus",
  "'-'", "'+'", "'/'", "'*'", "'%'", "'~'", "'!'", "minusminus_prefix",
  "plusplus_prefix", "below_curly", "'('", "new2", "'['", "']'", "'{'",
  "'.'", "'@'", "above_identifier", "below_else", "above_function", "'}'",
  "')'", "$accept", "PROGRAM", "MAYBE_PROGRAM_CODE_LIST",
  "PROGRAM_CODE_LIST", "PROGRAM_CODE", "MAYBE_INPACKAGE_CODE_LIST",
  "INPACKAGE_CODE_LIST", "INPACKAGE_CODE", "MAYBECODE", "CODE",
  "CODE_STATEMENT", "CODEPIECE", "CODEBLOCK", "PACKAGE_INITCODE",
  "CONDITIONAL_COMPILATION", "MAYBEEXPRESSION", "VARIABLE_DECLARATION",
  "VARIABLE_LIST", "ONE_VARIABLE", "MAYBEELSE", "IF", "$@1", "FOR_INIT",
  "FOR_IN_INIT", "FOR_START", "FOR", "FOR_IN", "WHILE", "$@2", "DO_WHILE",
  "$@3", "BREAK", "CONTINUE", "MAYBE_CASE_LIST", "CASE_LIST", "CASE",
  "DEFAULT", "SWITCH", "$@4", "CATCH", "$@5", "FINALLY", "$@6",
  "CATCH_LIST", "CATCH_FINALLY_LIST", "TRY", "$@7", "THROW", "WITH_HEAD",
  "WITH", "X_IDENTIFIER", "PACKAGE", "PACKAGE_DECLARATION", "$@8", "$@9",
  "IMPORT", "MAYBE_MODIFIERS", "MODIFIER_LIST", "MODIFIER", "EXTENDS",
  "EXTENDS_LIST", "IMPLEMENTS_LIST", "CLASS_DECLARATION", "$@10",
  "INTERFACE_DECLARATION", "$@11", "MAYBE_CLASS_BODY", "CLASS_BODY",
  "CLASS_BODY_ITEM", "MAYBE_INTERFACE_BODY", "INTERFACE_BODY",
  "IDECLARATION", "VARCONST", "SLOT_DECLARATION", "$@12", "SLOT_LIST",
  "ONE_SLOT", "MAYBECONSTANT", "CONSTANT", "MAYBE_PARAM_LIST",
  "PARAM_LIST", "PARAM", "GETSET", "FUNCTION_DECLARATION", "$@13",
  "MAYBE_IDENTIFIER", "INNERFUNCTION", "$@14", "CLASS", "PACKAGEANDCLASS",
  "CLASS_SPEC", "CLASS_SPEC_LIST", "TYPE", "MAYBETYPE",
  "MAYBE_PARAM_VALUES", "MAYBE_EXPRESSION_LIST", "EXPRESSION_LIST",
  "EXPRESSION_LIST_AND_COMMA", "XX", "NEW", "FUNCTIONCALL", "DELETE",
  "RETURN", "NONCOMMAEXPRESSION", "EXPRESSION", "VOIDEXPRESSION", "E",
  "MAYBE_EXPRPAIR_LIST", "EXPRPAIR_LIST", "VAR_READ", "NAMESPACE_ID",
  "NAMESPACE_DECLARATION", "USE_NAMESPACE", 0
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
     345,   346,   347,   348,   349,   350,   351,   352,    59,    44,
     353,    61,   354,    63,    58,   124,    94,    38,    60,    62,
     355,    45,    43,    47,    42,    37,   126,    33,   356,   357,
     358,    40,   359,    91,    93,   123,    46,    64,   360,   361,
     362,   125,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   133,   134,   135,   135,   136,   136,   137,   137,   137,
     137,   137,   137,   137,   137,   138,   138,   139,   139,   140,
     140,   140,   140,   140,   140,   140,   141,   141,   142,   142,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   144,   144,   144,   144,   144,   144,
     144,   144,   145,   145,   146,   147,   148,   148,   149,   149,
     150,   150,   151,   152,   152,   154,   153,   155,   155,   155,
     156,   156,   157,   157,   158,   159,   161,   160,   163,   162,
     164,   164,   165,   165,   166,   166,   166,   166,   167,   167,
     168,   169,   171,   170,   173,   172,   175,   174,   176,   176,
     177,   177,   177,   179,   178,   180,   180,   181,   182,   183,
     183,   183,   184,   184,   186,   185,   187,   185,   188,   188,
     189,   189,   190,   190,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   192,   192,   193,   193,   194,   194,
     196,   195,   198,   197,   199,   199,   200,   200,   201,   201,
     201,   201,   201,   202,   202,   203,   203,   204,   204,   204,
     205,   205,   207,   206,   208,   208,   209,   210,   210,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   212,   212,
     212,   212,   213,   213,   214,   214,   215,   215,   215,   217,
     216,   218,   218,   220,   219,   221,   222,   223,   223,   224,
     224,   225,   225,   225,   226,   226,   227,   227,   228,   228,
     228,   229,   230,   229,   231,   232,   233,   233,   234,   235,
     235,   236,   237,   237,   238,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   240,   240,   241,   241,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   242,   243,   243,   243,   244,   245
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
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       2,     4,     3,     1,     4,     2,     1,     1,     0,     0,
      12,     1,     0,     0,    10,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     2,     0,     0,     3,     0,     1,
       1,     1,     2,     2,     0,     4,     4,     4,     2,     1,
       2,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       2,     1,     3,     2,     4,     3,     0,     1,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     5,     2,     2,     2,     2,     3,     2,     4,
       5,     3,     5,     3,     1,     2,     4,     4,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     120,   294,   133,   172,   231,   169,   170,   171,     0,     0,
      78,     0,     0,   126,   124,   125,     0,   132,     0,   131,
     192,   176,   177,   261,   127,     0,     0,     0,   175,   128,
     130,   129,     0,     0,   174,   173,     0,     0,   266,     0,
       0,    14,     0,     0,     0,     0,   208,   120,     0,     0,
       2,   120,     5,    54,    12,     0,    36,    67,    31,    32,
      33,    34,    35,    38,   120,    37,     7,    30,     0,   121,
     122,     9,     8,    11,   230,    10,   225,   227,   229,   228,
     224,    39,   222,   226,    41,    40,     0,     0,    72,    76,
     120,    92,   109,   111,   110,   116,   113,     0,     0,   294,
     214,   191,     0,   260,     0,     0,   118,     0,   103,   208,
       0,   218,    65,     0,   221,     0,   267,   286,   285,   263,
     243,   242,     0,     0,   209,   210,   211,    82,     0,   106,
     219,     0,    80,    44,    43,   120,    45,    29,     0,    46,
      47,    48,    50,     0,    49,   288,     1,     6,   120,   294,
       0,    68,     0,     0,    69,    53,   108,     0,   188,     0,
     161,     0,   160,   162,   298,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   284,
     283,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,     0,     0,
      55,    73,     0,     0,     0,   120,   114,     0,   195,     0,
     198,   197,   299,   206,   178,     0,     0,     0,    27,     0,
     287,     0,     0,   270,     0,   262,   265,   212,   213,    83,
     205,    59,    60,   105,   220,    58,    81,    42,    28,   120,
       0,   205,     0,     0,    52,   295,   187,   186,     0,   134,
     136,     0,   223,   257,   258,   255,   256,   236,   237,   239,
     238,   234,   235,   277,   276,   272,   271,   279,   280,   278,
     274,   273,   275,   240,   241,   291,   249,   248,   247,   281,
       0,   246,   245,   244,   232,   233,   253,   252,   250,   254,
     251,     0,     0,   293,     0,     0,     0,     0,     0,    25,
       0,   120,    17,    23,     0,    20,    19,    22,    21,   120,
     112,     0,   208,   215,   167,     0,     0,   179,   183,   107,
     119,   196,   259,     0,    26,   217,     0,   268,     0,     0,
      57,     0,   120,    13,    57,     0,     0,     0,     0,     0,
     138,     0,     0,   205,   163,   164,     0,   216,   264,     0,
       0,   289,   120,     0,     0,   117,    18,   120,     0,     0,
       0,     0,   185,   180,   205,     0,     0,   120,     0,   203,
     202,   201,   204,     0,    62,    61,    51,     0,   120,   296,
     297,   178,   135,     0,     0,   199,   137,   142,    57,     0,
     282,   290,   292,    77,     0,    84,     0,   115,   207,   168,
     167,     0,     0,   182,     0,     0,    98,   102,   100,   104,
      63,   269,    56,     0,    75,     0,   139,   140,     0,   120,
     166,   165,    79,     0,     0,     0,    85,    88,    86,    24,
     184,   193,   181,    96,     0,    99,   101,   120,    66,   120,
     205,   120,   200,     0,   157,     0,     0,   120,   155,     0,
      27,    93,    89,    87,    27,    27,   205,    64,    74,     0,
     148,   152,     0,     0,     0,   120,   146,   150,   151,   158,
     188,   143,   156,    27,    91,     0,     0,     0,   189,   120,
     141,   147,     0,    90,   194,    97,    94,    27,     0,     0,
       0,     0,   149,   178,    27,   190,     0,     0,   205,    95,
     159
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    49,    50,    51,    52,   310,   311,   312,   333,   334,
     136,   137,   156,   313,   138,   384,   139,   245,   242,   448,
      56,   231,   152,   153,    57,    58,    59,    60,   212,    61,
      90,   140,   141,   435,   436,   437,   438,    62,   214,   416,
     500,   417,   465,   418,   419,    63,   228,   142,    64,    65,
     218,   219,    66,   319,   215,    67,   143,    69,    70,   350,
     352,   394,   315,   451,   316,   429,   474,   475,   476,   456,
     457,   458,   163,   317,   261,   354,   355,   372,    74,   326,
     327,   328,   258,   318,   497,   102,    76,   464,   220,   221,
     381,   396,   382,   340,   323,   123,   124,   125,   223,    77,
      78,    79,   144,   126,    80,    81,    82,   115,   116,    83,
     164,    84,    85
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -423
static const yytype_int16 yypact[] =
{
    1655,   -41,  -423,  -423,  -423,  -423,  -423,  -423,   -30,   -61,
    -423,   -52,    17,  -423,  -423,  -423,    61,  -423,  2098,  -423,
      87,  -423,  -423,  2153,  -423,   -19,    42,    11,  -423,  -423,
    -423,  -423,    -1,    -3,  -423,  -423,  2098,    19,  2098,  2098,
    2098,  -423,  2098,  2098,  2098,  2098,  2098,   623,    95,   136,
    -423,   494,  -423,  -423,  -423,    13,  -423,  2028,  -423,  -423,
    -423,  -423,  -423,  -423,  1905,  -423,  -423,  -423,   211,   394,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
      44,  -423,  2558,  -423,  -423,  -423,   174,    83,  -423,  -423,
    1905,  -423,  -423,  -423,  -423,  -423,  -423,    20,    42,  -423,
     -34,  -423,    85,    18,  2098,    79,  -423,  2098,  -423,  2098,
     204,    18,  -423,   105,  2558,    80,   113,    18,    18,   -50,
      18,    18,   -29,    89,   119,  2098,  -423,   217,   226,  2098,
    2098,   226,   230,  -423,  -423,   752,  -423,  -423,   109,  -423,
    -423,  -423,  -423,   214,  -423,  -423,  -423,  -423,  1010,   168,
     234,  -423,   140,   172,  -423,   142,  -423,   245,    16,   247,
    -423,   249,  -423,  -423,  -423,  -423,  2098,  2098,  2098,  2098,
    2098,  2098,  2098,  2098,  2098,  2098,  2098,  2098,  2098,  2098,
    2098,  2098,  2098,  2098,  2098,  2098,  2098,  2098,  2098,  -423,
    -423,   250,  2098,  2098,  2098,  2098,  2098,  2098,  2098,  2098,
    2098,  2098,  2098,  2098,  2098,  2098,  2098,  2098,  2098,    10,
    -423,  -423,  2098,   243,  2098,  1139,  -423,    42,   131,   133,
    -423,  -423,  -423,   141,    12,   -27,    64,  2178,  1780,   129,
    -423,  2098,  2098,  -423,  2098,  -423,  -423,  -423,  -423,  -423,
     159,   166,  -423,    44,    44,   166,  -423,  -423,  -423,  1905,
     135,   159,  2098,  2098,  -423,   169,  -423,  -423,   264,   216,
     218,   273,  2558,   486,   313,   313,   313,  2924,  2924,  2924,
    2924,   486,   486,  2558,  2558,  2558,  2558,  2558,  2558,  2558,
    2558,  2558,  2558,  2619,  2680,  -423,   359,   359,   359,  2558,
    2375,  2741,  2802,  2863,   486,   486,   -50,   -50,    18,    18,
      18,   145,  2436,   190,  2098,   277,   -16,   160,  2249,  -423,
     151,  1268,  -423,  -423,   158,  -423,  -423,  -423,  -423,  1139,
    -423,    42,  2098,  -423,    21,   281,   153,   187,  -423,  -423,
    -423,   163,  -423,   161,  1780,  -423,   -15,  -423,   186,    84,
     192,   226,   881,  -423,   -23,    29,    -2,   130,   173,    42,
     282,    42,   171,   159,   202,  -423,  2098,  -423,  -423,   301,
    2312,  -423,  1905,  2098,   180,  -423,  -423,  1139,   175,   176,
     290,    84,  -423,  -423,   159,    14,    30,  1905,  2098,  -423,
    -423,  -423,  -423,  2098,  -423,  -423,  -423,  2098,  1905,  -423,
    -423,    12,  -423,    42,   185,  -423,   224,  -423,   192,   273,
    2558,  -423,  -423,  -423,    15,    25,   184,  -423,  -423,  -423,
     215,   199,   281,  -423,   201,   207,  -423,  -423,    30,  -423,
     265,  -423,  -423,   198,  -423,   200,   224,  -423,    42,   227,
    -423,  -423,  -423,  2098,   231,   205,    25,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,   328,  -423,  -423,  1905,  -423,  1905,
     159,  1397,  -423,   336,  -423,   315,   219,   337,  -423,  2497,
    1780,  -423,  -423,  -423,  1780,  1780,   159,  -423,  -423,   222,
    -423,  -423,   223,   183,   229,  1526,  -423,  -423,  -423,  -423,
      16,  -423,  -423,  1780,  -423,   232,   233,   210,  -423,  1397,
    -423,  -423,   346,  -423,  -423,  -423,  -423,  1780,   235,   241,
     228,   240,  -423,    12,  1780,  -423,   244,   246,   159,  -423,
    -423
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -423,  -423,   238,  -423,   321,  -282,  -423,    67,  -422,   -25,
       1,   -55,   -74,    28,     2,    -8,   332,   263,    52,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,   -40,   -39,  -423,  -423,   -18,
    -423,   -17,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
       6,   103,  -423,  -423,  -423,  -423,     0,  -423,   326,  -423,
    -423,  -423,    48,  -423,    49,  -423,   -90,  -423,   -70,  -423,
    -423,   -48,  -423,     3,  -423,  -423,    24,     5,    40,  -377,
    -423,  -301,   -69,     4,  -423,  -423,  -423,  -423,  -423,   391,
     -91,    26,    59,  -240,  -423,   -97,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,   -32,    -9,   -49,   -13,  -423,  -423,  -423,
    -423,  -423,  -423
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -155
static const yytype_int16 yytable[] =
{
      68,    53,    55,    73,    75,   100,   113,   222,   154,   155,
     103,   344,   229,   303,   425,   324,   213,   324,    96,    87,
      92,    93,   135,   111,   373,   114,   117,   118,    54,   119,
     120,   121,    96,   114,    94,   155,   122,   368,   484,   189,
     190,   191,   485,   486,   -70,    92,    93,    86,    71,    72,
     256,    68,    53,    55,    73,    75,   414,   191,   433,    94,
      89,   493,   415,   204,   205,   206,   257,    92,    93,    91,
     166,   207,   166,   208,   413,   501,   209,    98,   383,    54,
     248,    94,   507,   166,   166,   406,   434,    92,    93,   208,
     101,    88,   209,   238,   227,   225,   114,   166,   145,    71,
      72,    94,   104,   235,   325,   329,   412,   189,   190,   191,
     301,   442,   114,   398,   166,    97,   362,   377,   109,   379,
     243,   244,   370,   110,   108,   371,   506,   387,   166,   105,
     388,   304,   107,   389,   411,   390,   146,   305,   148,   207,
     112,   208,    95,   166,   209,   216,   217,   432,    68,    53,
      55,    73,    75,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,    54,   210,   330,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   114,   302,    71,    72,   380,   157,
     337,   308,   338,   306,   211,   226,   224,   230,   158,   232,
     469,   233,   234,   236,   160,    68,    53,   314,   237,   114,
     239,   114,   336,   320,   342,   369,   487,   157,   162,   240,
     157,     2,   331,   246,   249,   -71,   158,   251,   252,   253,
     254,   159,   160,   345,   346,    13,    14,    15,   255,    17,
     259,    19,   260,   285,   161,   307,   162,  -113,   392,   321,
     395,   335,   322,   339,    24,   341,   343,   348,   510,   349,
     347,   351,   453,    29,    30,    31,   353,   357,   359,   248,
     361,   363,   365,   367,   324,   374,   375,   248,   403,  -112,
     378,   360,   376,   383,   391,     3,   397,   393,     5,     6,
       7,   399,   395,   420,   401,   405,   407,   155,   408,   114,
     427,    68,    53,   314,   424,   439,   370,    21,    22,    68,
      53,   314,   155,   428,   441,   454,   443,   331,   444,   447,
     449,   466,   450,   155,    28,   460,   461,   452,   423,   479,
     480,     2,   496,   400,    34,    35,   421,   488,   489,   499,
     481,   422,   167,   504,   404,    13,    14,    15,  -153,    17,
     490,    19,   503,   494,   495,   114,   502,    68,    53,   314,
     114,   505,   147,   467,    24,   468,   508,   509,   366,  -155,
    -155,  -155,   453,    29,    30,    31,   250,   175,   176,   151,
     430,   241,   155,   385,   155,   165,   462,   463,     2,   498,
     445,   446,   189,   190,   191,   491,   192,   193,   194,   482,
     409,   492,    13,    14,    15,   440,    17,   106,    19,   426,
     459,   200,   201,   431,   202,   203,   204,   205,   206,   455,
     410,    24,     0,     0,   207,   454,   208,     0,     0,   209,
      29,    30,    31,     0,     0,     0,     0,     0,   189,   190,
     191,   473,   471,   472,   477,   478,     0,   455,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -154,     0,
     202,   203,   204,   205,   206,   473,   471,   472,   477,   478,
     207,     0,   208,     0,     0,   209,     0,     0,     0,   473,
     471,   472,   477,   478,    -4,     0,     0,     1,     2,     3,
       4,     0,     5,     6,     7,     8,     9,    10,    11,     0,
       0,    12,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    21,    22,     0,     0,  -155,     0,     0,     0,    23,
       0,    24,    25,     0,    26,     0,    27,     0,    28,     0,
      29,    30,    31,     0,     0,    32,    33,     0,    34,    35,
       0,     0,     0,     0,     0,     0,    36,    37,     0,     0,
    -155,  -155,     0,    38,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   190,   191,     0,   192,
     193,   194,     0,    39,    40,     0,     0,     0,     0,     0,
       0,     0,    41,     0,  -155,  -155,     0,   202,   203,   204,
     205,   206,     0,     0,     0,    42,     0,   207,     0,   208,
      43,    44,   209,     0,     0,    45,     0,    46,     0,    47,
       0,    48,     0,     0,     0,    -4,     1,     2,     3,     4,
       0,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    13,    14,    15,    16,    17,    18,    19,    20,     0,
      21,    22,   127,     0,   128,     0,     0,     0,    23,   129,
      24,    25,     0,    26,   130,    27,     0,    28,   131,    29,
      30,    31,     0,     0,    32,    33,     0,    34,    35,     0,
       0,     0,     0,     0,     0,    36,    37,     0,   132,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,    40,     0,     0,     0,     0,     0,     0,
       0,   133,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,    43,
      44,     0,     0,     0,    45,     0,    46,     0,    47,     0,
      48,     0,     0,     0,   134,     1,     2,     3,     4,     0,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      13,    14,    15,    16,    17,    18,    19,    20,     0,    21,
      22,   127,     0,   128,     0,     0,     0,    23,   129,    24,
      25,     0,    26,   130,    27,     0,    28,   131,    29,    30,
      31,     0,     0,    32,    33,     0,    34,    35,     0,     0,
       0,     0,     0,     0,    36,    37,     0,   132,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    39,    40,     0,     0,     0,     0,     0,     0,     0,
     133,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,     0,    43,    44,
       0,     0,     0,    45,     0,    46,     0,    47,     0,    48,
       0,     0,     0,   247,     1,     2,     3,     4,     0,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    13,
      14,    15,    16,    17,    18,    19,    20,     0,    21,    22,
     127,     0,   128,     0,     0,     0,    23,   129,    24,    25,
       0,    26,   130,    27,     0,    28,   131,    29,    30,    31,
       0,     0,    32,    33,     0,    34,    35,     0,     0,     0,
       0,     0,     0,    36,    37,     0,   132,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,     0,     0,     0,     0,     0,   133,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    42,     0,     0,     0,     0,    43,    44,     0,
       0,     0,    45,     0,    46,     0,    47,     0,    48,     0,
       0,     0,   386,     1,     2,     3,     4,     0,     5,     6,
       7,     8,     9,    10,    11,     0,     0,    12,    13,    14,
      15,    16,    17,    18,    19,     0,     0,    21,    22,     0,
       0,     0,     0,     0,     0,    23,     0,    24,    25,     0,
      26,     0,    27,     0,    28,     0,    29,    30,    31,     0,
       0,    32,    33,     0,    34,    35,     0,     0,     0,     0,
       0,     0,    36,    37,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    39,
      40,     0,     0,     0,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,     0,    43,    44,     0,     0,
       0,    45,     0,    46,     0,    47,     0,    48,     0,     0,
       0,    -3,     1,     2,     3,     4,     0,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    21,    22,     0,     0,
       0,     0,     0,     0,    23,     0,    24,    25,     0,    26,
       0,    27,     0,    28,     0,    29,    30,    31,     0,     0,
      32,    33,     0,    34,    35,     0,     0,     0,     0,     0,
       0,    36,    37,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      42,     0,     0,     0,     0,    43,    44,     0,     0,     0,
      45,     0,    46,     0,    47,     0,    48,     0,     0,     0,
     -15,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    21,    22,     0,     0,     0,
       0,     0,     0,    23,     0,    24,    25,     0,    26,     0,
      27,     0,    28,     0,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    39,    40,     0,
       0,     0,     0,     0,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,    43,    44,     0,     0,     0,    45,
       0,    46,     0,    47,     0,    48,     0,     0,     0,   -16,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,    21,    22,     0,     0,     0,     0,
       0,     0,    23,     0,    24,    25,     0,    26,     0,    27,
       0,    28,     0,    29,    30,    31,     0,     0,    32,    33,
       0,    34,    35,     0,     0,     0,     0,     0,     0,    36,
      37,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    39,    40,     0,     0,
       0,     0,     0,     0,     0,   470,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,    43,    44,     0,     0,     0,    45,     0,
      46,     0,    47,     0,    48,     0,     0,     0,  -144,     1,
       2,     3,     4,     0,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    21,    22,     0,     0,     0,     0,     0,
       0,    23,     0,    24,    25,     0,    26,     0,    27,     0,
      28,     0,    29,    30,    31,     0,     0,    32,    33,     0,
      34,    35,     0,     0,     0,     0,     0,     0,    36,    37,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    39,    40,     0,     0,     0,
       0,     0,     0,     0,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
       0,     0,    43,    44,     0,     0,     0,    45,     0,    46,
       0,    47,     0,    48,     0,    -3,     0,  -145,     1,     2,
       3,     4,     0,     5,     6,     7,     8,     9,    10,    11,
       0,     0,    12,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    21,    22,     0,     0,     0,     0,     0,     0,
      23,     0,    24,    25,     0,    26,     0,    27,     0,    28,
       0,    29,    30,    31,     0,     0,    32,    33,     0,    34,
      35,     0,     0,     0,     0,     0,     0,    36,    37,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
       0,    43,    44,     0,     0,     0,    45,     0,    46,     0,
      47,     0,    48,     1,     2,     3,     4,     0,     5,     6,
       7,     8,     9,    10,    11,     0,  -120,     0,    13,    14,
      15,    16,    17,    18,    19,    20,     0,    21,    22,   127,
       0,   128,     0,     0,     0,    23,   129,    24,    25,     0,
      26,   130,    27,     0,    28,   131,    29,    30,    31,     0,
       0,    32,    33,     0,    34,    35,     0,     0,     0,     0,
       0,     0,    36,    37,     0,   132,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    39,
      40,     0,     0,     0,     0,     0,     0,     0,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,     0,    43,    44,     0,     0,
       0,    45,     0,    46,     0,    47,     0,    48,     1,     2,
       3,     4,     0,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    13,    14,    15,    16,    17,    18,    19,
      20,     0,    21,    22,   127,     0,   128,     0,     0,     0,
      23,   129,    24,    25,     0,    26,   130,    27,     0,    28,
     131,    29,    30,    31,     0,     0,    32,    33,     0,    34,
      35,     0,     0,     0,     0,     0,     0,    36,    37,     0,
     132,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,     0,     0,
       0,     0,     0,   133,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
       0,    43,    44,     0,     0,     0,    45,     0,    46,     0,
      47,   149,    48,     3,     4,     0,     5,     6,     7,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,     0,    20,     0,    21,    22,     0,     0,   128,
       0,     0,     0,    23,     0,     0,     0,     0,     0,     0,
      27,     0,    28,   150,     0,     0,     0,     0,     0,     0,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,     0,    38,     0,     0,
       0,    99,     0,     3,     4,     0,     5,     6,     7,     0,
       0,     0,     0,     0,     0,     0,     0,    39,    40,     0,
       0,    18,     0,    20,     0,    21,    22,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,    42,
      27,     0,    28,     0,    43,    44,     0,     0,     0,    45,
      33,    46,    34,    35,     0,    48,    99,     0,     3,     4,
      36,     5,     6,     7,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,    20,     0,
      21,    22,     0,     0,     0,     0,     0,    39,    40,     0,
       0,     0,     0,     0,     0,     0,     0,    28,     0,     0,
       0,     0,     0,     0,     0,    33,     0,    34,    35,    42,
       0,     0,     0,     0,    43,    44,     0,   167,     0,    45,
       0,    46,    38,     0,     0,    48,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,    40,   168,   169,   170,     0,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,     0,   189,   190,   191,
       0,   192,   193,   194,    45,     0,    46,     0,     0,   195,
      48,   196,     0,   197,   198,   199,   200,   201,   167,   202,
     203,   204,   205,   206,     0,     0,     0,     0,     0,   207,
       0,   208,     0,     0,   209,     0,     0,     0,     0,     0,
     332,     0,     0,     0,     0,   168,   169,   170,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,     0,   189,   190,
     191,     0,   192,   193,   194,     0,     0,     0,     0,     0,
     195,   167,   196,     0,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,     0,     0,     0,     0,     0,
     207,     0,   208,     0,     0,   209,     0,     0,   168,   169,
     170,   364,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
       0,   189,   190,   191,     0,   192,   193,   194,     0,     0,
       0,     0,     0,   195,   167,   196,     0,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,     0,     0,
       0,     0,     0,   207,     0,   208,     0,     0,   209,     0,
       0,   168,   169,   170,   402,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,   189,   190,   191,     0,   192,   193,
     194,     0,     0,     0,     0,   167,   195,     0,   196,   356,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,     0,     0,     0,     0,     0,   207,     0,   208,     0,
       0,   209,   168,   169,   170,     0,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,     0,   189,   190,   191,     0,   192,
     193,   194,     0,     0,     0,     0,   167,   195,     0,   196,
       0,   197,   198,   199,   200,   201,     0,   202,   203,   204,
     205,   206,     0,     0,     0,     0,     0,   207,     0,   208,
     358,     0,   209,   168,   169,   170,     0,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,   189,   190,   191,     0,
     192,   193,   194,     0,     0,     0,     0,   167,   195,     0,
     196,   483,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,     0,     0,     0,     0,     0,   207,     0,
     208,     0,     0,   209,   168,   169,   170,     0,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,     0,   189,   190,   191,
       0,   192,   193,   194,     0,     0,     0,     0,   167,   195,
       0,   196,     0,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,     0,     0,     0,     0,     0,   207,
       0,   208,     0,     0,   209,   168,   169,   170,     0,   171,
     172,   173,   174,   175,   176,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,     0,   189,   190,
     191,     0,   192,   193,   194,     0,     0,     0,     0,   167,
       0,     0,     0,     0,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,     0,     0,     0,     0,     0,
     207,     0,   208,     0,     0,   209,   168,   169,   170,     0,
     171,   172,   173,   174,   175,   176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   189,
     190,   191,     0,   192,   193,   194,     0,     0,     0,     0,
     167,     0,     0,     0,     0,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,     0,     0,     0,     0,
       0,   207,     0,   208,     0,     0,   209,   168,   169,   170,
       0,   171,   172,   173,   174,   175,   176,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,   190,   191,     0,   192,   193,   194,     0,     0,     0,
       0,   167,     0,     0,     0,     0,     0,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,     0,     0,     0,
       0,     0,   207,     0,   208,     0,     0,   209,   168,   169,
     170,     0,   171,   172,   173,   174,   175,   176,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,   190,   191,     0,   192,   193,   194,     0,     0,
       0,     0,   167,     0,     0,     0,     0,     0,     0,   199,
     200,   201,     0,   202,   203,   204,   205,   206,     0,     0,
       0,     0,     0,   207,     0,   208,     0,     0,   209,   168,
     169,   170,     0,   171,   172,   173,   174,   175,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,   190,   191,     0,   192,   193,   194,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
    -155,   200,   201,     0,   202,   203,   204,   205,   206,     0,
       0,     0,     0,     0,   207,     0,   208,     0,     0,   209,
     168,   169,   170,     0,  -155,  -155,  -155,  -155,   175,   176,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,     0,   192,   193,   194,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   200,   201,     0,   202,   203,   204,   205,   206,
       0,     0,     0,     0,     0,   207,     0,   208,     0,     0,
     209
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,     0,    18,    38,    98,    57,    64,
      23,   251,   109,     3,   391,     3,    90,     3,    12,    49,
       3,     4,    47,    36,   325,    38,    39,    40,     0,    42,
      43,    44,    26,    46,    17,    90,    45,   319,   460,    89,
      90,    91,   464,   465,    67,     3,     4,    88,     0,     0,
      34,    51,    51,    51,    51,    51,    26,    91,    33,    17,
     121,   483,    32,   113,   114,   115,    50,     3,     4,   121,
      99,   121,    99,   123,   375,   497,   126,    16,   101,    51,
     135,    17,   504,    99,    99,   367,    61,     3,     4,   123,
       3,   121,   126,   125,   107,   104,   109,    99,     3,    51,
      51,    17,   121,   132,    92,   132,    92,    89,    90,    91,
     207,   412,   125,   353,    99,    12,   132,   132,   121,    35,
     129,   130,   101,   126,   125,   104,   503,    98,    99,    26,
     132,   121,   121,     3,   374,     5,     0,   127,   125,   121,
     121,   123,   125,    99,   126,   125,   126,   132,   148,   148,
     148,   148,   148,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   148,     3,   114,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   148,   148,   114,    16,
     232,   214,   234,   212,   121,   126,   121,     3,    25,   104,
     450,   131,    99,   124,    31,   215,   215,   215,    99,   232,
       3,   234,   231,   217,   249,   322,   466,    16,    45,     3,
      16,     4,   226,     3,   125,    67,    25,     3,    98,    67,
      98,    30,    31,   252,   253,    18,    19,    20,     3,    22,
       3,    24,     3,     3,    43,    12,    45,   126,   349,   126,
     351,   132,   121,   104,    37,    99,   131,     3,   508,    53,
     101,    53,    45,    46,    47,    48,     3,   132,    88,   334,
       3,   121,   131,   125,     3,   132,    99,   342,   362,   126,
     104,   304,   131,   101,   121,     5,   125,    15,     8,     9,
      10,    99,   393,   377,     3,   125,   131,   362,   132,   322,
     125,   311,   311,   311,   388,   131,   101,    27,    28,   319,
     319,   319,   377,    99,   125,    98,   125,   321,   121,    64,
     132,     3,   132,   388,    44,   104,   131,   428,   387,     3,
      25,     4,   132,   356,    54,    55,   378,   125,   125,     3,
     131,   383,    39,   125,   363,    18,    19,    20,   131,    22,
     131,    24,   121,   131,   131,   378,   131,   367,   367,   367,
     383,   131,    51,   447,    37,   449,   132,   131,   311,    66,
      67,    68,    45,    46,    47,    48,   148,    74,    75,    57,
     398,   128,   447,   341,   449,    69,   436,   436,     4,   489,
     418,   418,    89,    90,    91,   475,    93,    94,    95,   457,
     370,   480,    18,    19,    20,   410,    22,    26,    24,   393,
     433,   108,   109,   399,   111,   112,   113,   114,   115,   429,
     371,    37,    -1,    -1,   121,    98,   123,    -1,    -1,   126,
      46,    47,    48,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,   451,   451,   451,   451,   451,    -1,   457,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,
     111,   112,   113,   114,   115,   475,   475,   475,   475,   475,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   489,
     489,   489,   489,   489,     0,    -1,    -1,     3,     4,     5,
       6,    -1,     8,     9,    10,    11,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    27,    28,    -1,    -1,    39,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    40,    -1,    42,    -1,    44,    -1,
      46,    47,    48,    -1,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    -1,    -1,
      74,    75,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    -1,    93,
      94,    95,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,    -1,   111,    -1,   121,    -1,   123,
     116,   117,   126,    -1,    -1,   121,    -1,   123,    -1,   125,
      -1,   127,    -1,    -1,    -1,   131,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    29,    -1,    31,    -1,    -1,    -1,    35,    36,
      37,    38,    -1,    40,    41,    42,    -1,    44,    45,    46,
      47,    48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    -1,    65,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,    -1,
     127,    -1,    -1,    -1,   131,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    13,    14,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,
      38,    -1,    40,    41,    42,    -1,    44,    45,    46,    47,
      48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    -1,    65,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,    -1,   125,    -1,   127,
      -1,    -1,    -1,   131,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,
      -1,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    -1,    65,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,    -1,   125,    -1,   127,    -1,
      -1,    -1,   131,     3,     4,     5,     6,    -1,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      40,    -1,    42,    -1,    44,    -1,    46,    47,    48,    -1,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,    -1,   127,    -1,    -1,
      -1,   131,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    13,    14,    -1,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,
      -1,    42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,    -1,   125,    -1,   127,    -1,    -1,    -1,
     131,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,
      42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,    -1,   127,    -1,    -1,    -1,   131,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    -1,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,    42,
      -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,    -1,   125,    -1,   127,    -1,    -1,    -1,   131,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    12,    13,
      14,    -1,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    40,    -1,    42,    -1,
      44,    -1,    46,    47,    48,    -1,    -1,    51,    52,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,    -1,   127,    -1,     0,    -1,   131,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    40,    -1,    42,    -1,    44,
      -1,    46,    47,    48,    -1,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,   121,    -1,   123,    -1,
     125,    -1,   127,     3,     4,     5,     6,    -1,     8,     9,
      10,    11,    12,    13,    14,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    27,    28,    29,
      -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,    -1,
      40,    41,    42,    -1,    44,    45,    46,    47,    48,    -1,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    -1,    65,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,    -1,   127,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    13,    14,
      -1,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    29,    -1,    31,    -1,    -1,    -1,
      35,    36,    37,    38,    -1,    40,    41,    42,    -1,    44,
      45,    46,    47,    48,    -1,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    -1,
      65,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,   121,    -1,   123,    -1,
     125,     3,   127,     5,     6,    -1,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    25,    -1,    27,    28,    -1,    -1,    31,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,     3,    -1,     5,     6,    -1,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      -1,    23,    -1,    25,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,   111,
      42,    -1,    44,    -1,   116,   117,    -1,    -1,    -1,   121,
      52,   123,    54,    55,    -1,   127,     3,    -1,     5,     6,
      62,     8,     9,    10,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    25,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    55,   111,
      -1,    -1,    -1,    -1,   116,   117,    -1,    39,    -1,   121,
      -1,   123,    69,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,   121,    -1,   123,    -1,    -1,   101,
     127,   103,    -1,   105,   106,   107,   108,   109,    39,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,
     101,    39,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    66,    67,
      68,   132,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      -1,    -1,    -1,   101,    39,   103,    -1,   105,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    66,    67,    68,   132,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    -1,    -1,    39,   101,    -1,   103,   104,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,
      -1,   126,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    -1,    -1,    39,   101,    -1,   103,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
     124,    -1,   126,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    -1,    -1,    39,   101,    -1,
     103,   104,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    -1,    -1,    39,   101,
      -1,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,    -1,   126,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,    -1,    -1,   126,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,   105,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,    -1,    -1,   126,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    -1,   121,    -1,   123,    -1,    -1,   126,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,    -1,
     126
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     8,     9,    10,    11,    12,
      13,    14,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    27,    28,    35,    37,    38,    40,    42,    44,    46,
      47,    48,    51,    52,    54,    55,    62,    63,    69,    89,
      90,    98,   111,   116,   117,   121,   123,   125,   127,   134,
     135,   136,   137,   143,   146,   147,   153,   157,   158,   159,
     160,   162,   170,   178,   181,   182,   185,   188,   189,   190,
     191,   195,   197,   206,   211,   216,   219,   232,   233,   234,
     237,   238,   239,   242,   244,   245,    88,    49,   121,   121,
     163,   121,     3,     4,    17,   125,   183,   184,    16,     3,
     239,     3,   218,   239,   121,   184,   222,   121,   125,   121,
     126,   239,   121,   236,   239,   240,   241,   239,   239,   239,
     239,   239,   237,   228,   229,   230,   236,    29,    31,    36,
      41,    45,    65,    98,   131,   142,   143,   144,   147,   149,
     164,   165,   180,   189,   235,     3,     0,   137,   125,     3,
      45,   149,   155,   156,   238,   144,   145,    16,    25,    30,
      31,    43,    45,   205,   243,   191,    99,    39,    66,    67,
      68,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    89,
      90,    91,    93,    94,    95,   101,   103,   105,   106,   107,
     108,   109,   111,   112,   113,   114,   115,   121,   123,   126,
       3,   121,   161,   145,   171,   187,   125,   126,   183,   184,
     221,   222,   223,   231,   121,   237,   126,   239,   179,   228,
       3,   154,   104,   131,    99,   132,   124,    99,   236,     3,
       3,   150,   151,   237,   237,   150,     3,   131,   144,   125,
     135,     3,    98,    67,    98,     3,    34,    50,   215,     3,
       3,   207,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,     3,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   228,   239,     3,   121,   127,   237,    12,   239,    98,
     138,   139,   140,   146,   147,   195,   197,   206,   216,   186,
     183,   126,   121,   227,     3,    92,   212,   213,   214,   132,
     114,   183,   132,   141,   142,   132,   237,   236,   236,   104,
     226,    99,   142,   131,   226,   237,   237,   101,     3,    53,
     192,    53,   193,     3,   208,   209,   104,   132,   124,    88,
     239,     3,   132,   121,   132,   131,   140,   125,   138,   228,
     101,   104,   210,   214,   132,    99,   131,   132,   104,    35,
     114,   223,   225,   101,   148,   151,   131,    98,   132,     3,
       5,   121,   223,    15,   194,   223,   224,   125,   226,    99,
     239,     3,   132,   145,   237,   125,   138,   131,   132,   211,
     225,   226,    92,   214,    26,    32,   172,   174,   176,   177,
     145,   236,   236,   238,   145,   212,   224,   125,    99,   198,
     148,   209,   132,    33,    61,   166,   167,   168,   169,   131,
     210,   125,   214,   125,   121,   172,   174,    64,   152,   132,
     132,   196,   223,    45,    98,   189,   202,   203,   204,   239,
     104,   131,   168,   169,   220,   175,     3,   145,   145,   226,
      98,   143,   147,   189,   199,   200,   201,   206,   216,     3,
      25,   131,   204,   104,   141,   141,   141,   226,   125,   125,
     131,   201,   215,   141,   131,   131,   132,   217,   199,     3,
     173,   141,   131,   121,   125,   131,   212,   141,   132,   131,
     226
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
#line 2086 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 24:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2098 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 26:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2101 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2102 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2104 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 29:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2105 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 42:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2120 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 43:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2121 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 44:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2124 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 51:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2131 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 52:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2135 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2136 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2140 "parser.y"
    {
    code_t**cc = &global->init->method->body->code;
    *cc = code_append(*cc, (yyvsp[(1) - (1)].code));
}
    }
    break;


  
    case 55:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2147 "parser.y"
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
#line 2166 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (2)].value);}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2167 "parser.y"
    {(yyval.value).c=abc_pushundefined(0);
                                  (yyval.value).t=TYPE_ANY;
                                 }
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2171 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2172 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2174 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2175 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2178 "parser.y"
    {
PASS12
    if(variable_exists((yyvsp[(1) - (3)].id)))
        syntaxerror("Variable %s already defined", (yyvsp[(1) - (3)].id));
PASS1
    new_variable((yyvsp[(1) - (3)].id), 0, 1, 0);
PASS2
   
    if(!is_subtype_of((yyvsp[(3) - (3)].value).t, (yyvsp[(2) - (3)].classinfo))) {
        syntaxerror("Can't convert %s to %s", (yyvsp[(3) - (3)].value).t->name, 
                                              (yyvsp[(2) - (3)].classinfo)->name);
    }

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
    
    if((yyvsp[(2) - (3)].classinfo)) {
        if((yyvsp[(3) - (3)].value).c->prev || (yyvsp[(3) - (3)].value).c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = code_append((yyval.code), (yyvsp[(3) - (3)].value).c);
            (yyval.code) = converttype((yyval.code), (yyvsp[(3) - (3)].value).t, (yyvsp[(2) - (3)].classinfo));
        } else {
            code_free((yyvsp[(3) - (3)].value).c);
            (yyval.code) = defaultvalue((yyval.code), (yyvsp[(2) - (3)].classinfo));
        }
    } else {
        if((yyvsp[(3) - (3)].value).c->prev || (yyvsp[(3) - (3)].value).c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = code_append((yyval.code), (yyvsp[(3) - (3)].value).c);
            (yyval.code) = abc_coerce_a((yyval.code));
        } else {
            // don't do anything
            code_free((yyvsp[(3) - (3)].value).c);
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
#line 2238 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2239 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2242 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2242 "parser.y"
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
#line 2261 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2268 "parser.y"
    {
    PASS1 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),0,1,0);
    PASS2 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
}
    }
    break;


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2272 "parser.y"
    {
    PASS12
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 72:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2277 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 73:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2278 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2280 "parser.y"
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
#line 2300 "parser.y"
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
#line 2348 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2348 "parser.y"
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
#line 2367 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2367 "parser.y"
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
#line 2382 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2385 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2388 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2391 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2395 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2396 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2397 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2398 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2399 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2400 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2402 "parser.y"
    {
    (yyval.code) = abc_getlocal(0, state->switch_var);
    (yyval.code) = code_append((yyval.code), (yyvsp[(2) - (4)].value).c);
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
#line 2413 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2416 "parser.y"
    {PASS12 new_state();state->switch_var=alloc_local();}
    }
    break;


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2416 "parser.y"
    {
    (yyval.code)=(yyvsp[(4) - (8)].value).c;
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
#line 2447 "parser.y"
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
#line 2452 "parser.y"
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
#line 2472 "parser.y"
    {PASS12 new_state();state->exception_name=0;}
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2472 "parser.y"
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
#line 2488 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2489 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2490 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2491 "parser.y"
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
#line 2499 "parser.y"
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
#line 2508 "parser.y"
    {PASS12 new_state();
                 state->method->has_exceptions=1;
                 state->method->late_binding=1;//for invariant scope_code
                }
    }
    break;


  
    case 104:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2511 "parser.y"
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
#line 2557 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2561 "parser.y"
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
#line 2572 "parser.y"
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
#line 2582 "parser.y"
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
#line 2600 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 111:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2601 "parser.y"
    {PASS12 (yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2603 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2604 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2606 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2607 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2608 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 117:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2609 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 118:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2631 "parser.y"
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
#line 2645 "parser.y"
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
#line 2661 "parser.y"
    {PASS12 (yyval.flags).flags=0;(yyval.flags).ns=0;}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2662 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2663 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2664 "parser.y"
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
#line 2671 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PUBLIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2672 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PRIVATE;(yyval.flags).ns=0;}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2673 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PROTECTED;(yyval.flags).ns=0;}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2674 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_STATIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2675 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_DYNAMIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2676 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_FINAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2677 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_OVERRIDE;(yyval.flags).ns=0;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2678 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NATIVE;(yyval.flags).ns=0;}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2679 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PACKAGEINTERNAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2680 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NAMESPACE;
                               (yyval.flags).ns=(yyvsp[(1) - (1)].id);
                       }
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2684 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2685 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2687 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2688 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2690 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2691 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2695 "parser.y"
    {PASS12 startclass(&(yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2697 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 142:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2701 "parser.y"
    {PASS12 (yyvsp[(1) - (5)].flags).flags|=FLAG_INTERFACE;
                                          startclass(&(yyvsp[(1) - (5)].flags),(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 143:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2704 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 149:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2713 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 152:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2717 "parser.y"
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
#line 2728 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 159:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2731 "parser.y"
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
#line 2765 "parser.y"
    {PASS12 setslotstate(&(yyvsp[(1) - (2)].flags),(yyvsp[(2) - (2)].token));}
    }
    break;


  
    case 163:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2765 "parser.y"
    {PASS12 (yyval.code)=(yyvsp[(4) - (4)].code);setslotstate(0, 0);}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2767 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 165:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2768 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 166:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2771 "parser.y"
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

        /* slot name */
        multiname_t mname = {QNAME, &ns, 0, (yyvsp[(1) - (3)].id)};
      
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
        if((yyvsp[(2) - (3)].classinfo)) {
            MULTINAME(m, (yyvsp[(2) - (3)].classinfo));
            t = trait_new_member(traits, multiname_clone(&m), multiname_clone(&mname), 0);
        } else {
            t = trait_new_member(traits, 0, multiname_clone(&mname), 0);
        }
        info->slot = t->slot_id;
        
        /* initalization code (if needed) */
        code_t*c = 0;
        if((yyvsp[(3) - (3)].value).c && !is_pushundefined((yyvsp[(3) - (3)].value).c)) {
            c = abc_getlocal_0(c);
            c = code_append(c, (yyvsp[(3) - (3)].value).c);
            c = converttype(c, (yyvsp[(3) - (3)].value).t, (yyvsp[(2) - (3)].classinfo));
            c = abc_setslot(c, t->slot_id);
        }

        *code = code_append(*code, c);

        if(slotstate_varconst==KW_CONST) {
            t->kind= TRAIT_CONST;
        }
    }

    (yyval.code)=0;
}
    }
    break;


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2855 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2856 "parser.y"
    {(yyval.constant)=(yyvsp[(2) - (2)].constant);}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2859 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2860 "parser.y"
    {
    (yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));
}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2863 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2864 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2865 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2866 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2867 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2868 "parser.y"
    {(yyval.constant) = constant_new_undefined((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2869 "parser.y"
    {(yyval.constant) = constant_new_float(__builtin_nan(""));}
    }
    break;


  
    case 178:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2884 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2888 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2894 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(2) - (2)].param));
}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2900 "parser.y"
    {
    PASS12
    (yyval.params) =(yyvsp[(1) - (4)].params);
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(4) - (4)].param));
}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2908 "parser.y"
    {
    PASS12
    (yyval.params) = (yyvsp[(1) - (3)].params);
    list_append((yyval.params).list, (yyvsp[(3) - (3)].param));
}
    }
    break;


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2913 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    list_append((yyval.params).list, (yyvsp[(1) - (1)].param));
}
    }
    break;


  
    case 184:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2919 "parser.y"
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


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2927 "parser.y"
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


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2937 "parser.y"
    {PASS12 (yyval.token)=0;}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2940 "parser.y"
    {PASS12 startfunction(&(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2941 "parser.y"
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


  
    case 192:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2957 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 193:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2959 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 194:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2960 "parser.y"
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


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2982 "parser.y"
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


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3001 "parser.y"
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


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3018 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3019 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3021 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3022 "parser.y"
    {PASS12 (yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 203:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3023 "parser.y"
    {PASS12 (yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 204:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3032 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 205:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3033 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3037 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3038 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 208:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3040 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 211:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3044 "parser.y"
    {(yyval.value_list).number=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3048 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 213:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3049 "parser.y"
    {
                                                  (yyval.value_list).number= (yyvsp[(1) - (2)].value_list).number+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 215:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3055 "parser.y"
    {
    (yyval.value).c = (yyvsp[(2) - (4)].value).c;
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
    if(TYPE_IS_CLASS((yyvsp[(2) - (4)].value).t) && (yyvsp[(2) - (4)].value).t->data) {
        (yyval.value).t = (yyvsp[(2) - (4)].value).t->data;
    } else {
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_ANY;
    }
}
    }
    break;


  
    case 216:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3091 "parser.y"
    {
    
    (yyval.value).c = (yyvsp[(1) - (4)].value).c;
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
   
    if(TYPE_IS_FUNCTION((yyvsp[(1) - (4)].value).t) && (yyvsp[(1) - (4)].value).t->data) {
        (yyval.value).t = ((methodinfo_t*)((yyvsp[(1) - (4)].value).t->data))->return_type;
    } else {
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_ANY;
    }
}
    }
    break;


  
    case 217:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3137 "parser.y"
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


  
    case 218:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3158 "parser.y"
    {
    (yyval.value).c = (yyvsp[(2) - (2)].value).c;
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
        MULTINAME_LATE(m, (yyvsp[(2) - (2)].value).t?(yyvsp[(2) - (2)].value).t->access:ACCESS_PACKAGE, "");
        (yyval.value).c = abc_deleteproperty2((yyval.value).c, &m);
    }
    (yyval.value).t = TYPE_BOOLEAN;
}
    }
    break;


  
    case 219:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3179 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 220:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3182 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 221:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3189 "parser.y"
    {(yyval.value)=(yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3190 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3191 "parser.y"
    {
    (yyval.value).c = (yyvsp[(1) - (3)].value).c;
    (yyval.value).c = cut_last_push((yyval.value).c);
    (yyval.value).c = code_append((yyval.value).c,(yyvsp[(3) - (3)].value).c);
    (yyval.value).t = (yyvsp[(3) - (3)].value).t;
}
    }
    break;


  
    case 224:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3197 "parser.y"
    {
    (yyval.code)=cut_last_push((yyvsp[(1) - (1)].value).c);
}
    }
    break;


  
    case 225:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3203 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3204 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 227:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3205 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 228:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3206 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 230:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3210 "parser.y"
    {
    node_t*n = mkconstnode((yyvsp[(1) - (1)].constant));
    (yyval.value) = node_read(n);
}
    }
    break;


  
    case 231:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3215 "parser.y"
    {
    (yyval.value).c = 0;
    namespace_t ns = {ACCESS_PACKAGE, ""};
    multiname_t m = {QNAME, &ns, 0, "RegExp"};
    if(!(yyvsp[(1) - (1)].regexp).options) {
        (yyval.value).c = abc_getlex2((yyval.value).c, &m);
        (yyval.value).c = abc_pushstring((yyval.value).c, (yyvsp[(1) - (1)].regexp).pattern);
        (yyval.value).c = abc_construct((yyval.value).c, 1);
    } else {
        (yyval.value).c = abc_getlex2((yyval.value).c, &m);
        (yyval.value).c = abc_pushstring((yyval.value).c, (yyvsp[(1) - (1)].regexp).pattern);
        (yyval.value).c = abc_pushstring((yyval.value).c, (yyvsp[(1) - (1)].regexp).options);
        (yyval.value).c = abc_construct((yyval.value).c, 2);
    }
    (yyval.value).t = TYPE_REGEXP;
}
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3232 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3235 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3238 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3241 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3244 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3247 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
              }
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3250 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3253 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3257 "parser.y"
    {(yyval.value).t = join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, 'O');
              (yyval.value).c = (yyvsp[(1) - (3)].value).c;
              (yyval.value).c = converttype((yyval.value).c, (yyvsp[(1) - (3)].value).t, (yyval.value).t);
              (yyval.value).c = abc_dup((yyval.value).c);
              code_t*jmp = (yyval.value).c = abc_iftrue((yyval.value).c, 0);
              (yyval.value).c = cut_last_push((yyval.value).c);
              (yyval.value).c = code_append((yyval.value).c,(yyvsp[(3) - (3)].value).c);
              (yyval.value).c = converttype((yyval.value).c, (yyvsp[(3) - (3)].value).t, (yyval.value).t);
              code_t*label = (yyval.value).c = abc_label((yyval.value).c);
              jmp->branch = label;
             }
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3268 "parser.y"
    {
              (yyval.value).t = join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, 'A');
              /*printf("%08x:\n",$1.t);
              code_dump($1.c, 0, 0, "", stdout);
              printf("%08x:\n",$3.t);
              code_dump($3.c, 0, 0, "", stdout);
              printf("joining %08x and %08x to %08x\n", $1.t, $3.t, $$.t);*/
              (yyval.value).c = (yyvsp[(1) - (3)].value).c;
              (yyval.value).c = converttype((yyval.value).c, (yyvsp[(1) - (3)].value).t, (yyval.value).t);
              (yyval.value).c = abc_dup((yyval.value).c);
              code_t*jmp = (yyval.value).c = abc_iffalse((yyval.value).c, 0);
              (yyval.value).c = cut_last_push((yyval.value).c);
              (yyval.value).c = code_append((yyval.value).c,(yyvsp[(3) - (3)].value).c);
              (yyval.value).c = converttype((yyval.value).c, (yyvsp[(3) - (3)].value).t, (yyval.value).t);
              code_t*label = (yyval.value).c = abc_label((yyval.value).c);
              jmp->branch = label;              
             }
    }
    break;


  
    case 242:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3286 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 243:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3291 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_bitnot((yyval.value).c);
              (yyval.value).t = TYPE_INT;
             }
    }
    break;


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3296 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitand((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3301 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitxor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3306 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 247:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3311 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_rshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3315 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_urshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3319 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_lshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3324 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_divide((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3328 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_modulo((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3332 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             if(BOTH_INT((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t)) {
                (yyval.value).c = abc_add_i((yyval.value).c);
                (yyval.value).t = TYPE_INT;
             } else {
                (yyval.value).c = abc_add((yyval.value).c);
                (yyval.value).t = join_types((yyvsp[(1) - (3)].value).t,(yyvsp[(3) - (3)].value).t,'+');
             }
            }
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3341 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             if(BOTH_INT((yyvsp[(1) - (3)].value).t,(yyvsp[(3) - (3)].value).t)) {
                (yyval.value).c = abc_subtract_i((yyval.value).c);
                (yyval.value).t = TYPE_INT;
             } else {
                (yyval.value).c = abc_subtract((yyval.value).c);
                (yyval.value).t = TYPE_NUMBER;
             }
            }
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3350 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             if(BOTH_INT((yyvsp[(1) - (3)].value).t,(yyvsp[(3) - (3)].value).t)) {
                (yyval.value).c = abc_multiply_i((yyval.value).c);
                (yyval.value).t = TYPE_INT;
             } else {
                (yyval.value).c = abc_multiply((yyval.value).c);
                (yyval.value).t = TYPE_NUMBER;
             }
            }
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3360 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_in((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3365 "parser.y"
    {char use_astype=0; // flash player's astype works differently than astypelate
              if(use_astype && TYPE_IS_CLASS((yyvsp[(3) - (3)].value).t) && (yyvsp[(3) - (3)].value).t->data) {
                MULTINAME(m, (classinfo_t*)((yyvsp[(3) - (3)].value).t->data));
                (yyval.value).c = abc_astype2((yyvsp[(1) - (3)].value).c, &m);
                (yyval.value).t = (yyvsp[(3) - (3)].value).t->data;
              } else {
                (yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
                (yyval.value).c = abc_astypelate((yyval.value).c);
                (yyval.value).t = TYPE_ANY;
              }
             }
    }
    break;


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3378 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_instanceof((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3383 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_istypelate((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3388 "parser.y"
    {
              (yyval.value).c = (yyvsp[(3) - (4)].value).c;
              (yyval.value).c = abc_typeof((yyval.value).c);
              (yyval.value).t = TYPE_STRING;
             }
    }
    break;


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3394 "parser.y"
    {
              (yyval.value).c = cut_last_push((yyvsp[(2) - (2)].value).c);
              (yyval.value).c = abc_pushundefined((yyval.value).c);
              (yyval.value).t = TYPE_ANY;
             }
    }
    break;


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3400 "parser.y"
    { (yyval.value).c = abc_pushundefined(0);
             (yyval.value).t = TYPE_ANY;
           }
    }
    break;


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3404 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (3)].value);}
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3406 "parser.y"
    {
  (yyval.value).c = (yyvsp[(2) - (2)].value).c;
  if(IS_INT((yyvsp[(2) - (2)].value).t)) {
   (yyval.value).c=abc_negate_i((yyval.value).c);
   (yyval.value).t = TYPE_INT;
  } else {
   (yyval.value).c=abc_negate((yyval.value).c);
   (yyval.value).t = TYPE_NUMBER;
  }
}
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3417 "parser.y"
    {
  (yyval.value).c = (yyvsp[(1) - (4)].value).c;
  (yyval.value).c = code_append((yyval.value).c, (yyvsp[(3) - (4)].value).c);

  MULTINAME_LATE(m, (yyvsp[(1) - (4)].value).t?(yyvsp[(1) - (4)].value).t->access:ACCESS_PACKAGE, "");
  (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
  (yyval.value).t = 0; // array elements have unknown type
}
    }
    break;


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3426 "parser.y"
    {
    (yyval.value).c = code_new();
    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(2) - (3)].value_list).cc);
    (yyval.value).c = abc_newarray((yyval.value).c, (yyvsp[(2) - (3)].value_list).number);
    (yyval.value).t = registry_getarrayclass();
}
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3433 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3434 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3436 "parser.y"
    {
    (yyval.value_list).cc = 0;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(1) - (3)].value).c);
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (3)].value).c);
    (yyval.value_list).number = 2;
}
    }
    break;


  
    case 269:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3442 "parser.y"
    {
    (yyval.value_list).cc = (yyvsp[(1) - (5)].value_list).cc;
    (yyval.value_list).number = (yyvsp[(1) - (5)].value_list).number+2;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (5)].value).c);
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(5) - (5)].value).c);
}
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3451 "parser.y"
    {
    (yyval.value).c = code_new();
    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(2) - (3)].value_list).cc);
    (yyval.value).c = abc_newobject((yyval.value).c, (yyvsp[(2) - (3)].value_list).number/2);
    (yyval.value).t = registry_getobjectclass();
}
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3458 "parser.y"
    { 
               code_t*c = (yyvsp[(3) - (3)].value).c;
               if(BOTH_INT((yyvsp[(1) - (3)].value).t,(yyvsp[(3) - (3)].value).t)) {
                c=abc_multiply_i(c);
               } else {
                c=abc_multiply(c);
               }
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '*'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3470 "parser.y"
    { 
               code_t*c = abc_modulo((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '%'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3476 "parser.y"
    { 
               code_t*c = abc_lshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '<'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3482 "parser.y"
    { 
               code_t*c = abc_rshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '>'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3488 "parser.y"
    { 
               code_t*c = abc_urshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, 'U'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3494 "parser.y"
    { 
               code_t*c = abc_divide((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '/'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3500 "parser.y"
    { 
               code_t*c = abc_bitor((yyvsp[(3) - (3)].value).c);
               c=converttype(c, TYPE_INT, (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 278:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3506 "parser.y"
    { 
               code_t*c = abc_bitxor((yyvsp[(3) - (3)].value).c);
               c=converttype(c, TYPE_INT, (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 279:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3512 "parser.y"
    { 
               code_t*c = (yyvsp[(3) - (3)].value).c;

               if(TYPE_IS_INT((yyvsp[(1) - (3)].value).t)) {
                c=abc_add_i(c);
               } else {
                c=abc_add(c);
                c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '+'), (yyvsp[(1) - (3)].value).t);
               }
               
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 280:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3525 "parser.y"
    { code_t*c = (yyvsp[(3) - (3)].value).c; 
               if(TYPE_IS_INT((yyvsp[(1) - (3)].value).t)) {
                c=abc_subtract_i(c);
               } else {
                c=abc_subtract(c);
                c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '-'), (yyvsp[(1) - (3)].value).t);
               }
               
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0, 1);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
             }
    }
    break;


  
    case 281:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3536 "parser.y"
    { code_t*c = 0;
              c = code_append(c, (yyvsp[(3) - (3)].value).c);
              c = converttype(c, (yyvsp[(3) - (3)].value).t, (yyvsp[(1) - (3)].value).t);
              (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 1, 0, 1);
              (yyval.value).t = (yyvsp[(1) - (3)].value).t;
            }
    }
    break;


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3543 "parser.y"
    { 
              (yyval.value).t = join_types((yyvsp[(3) - (5)].value).t,(yyvsp[(5) - (5)].value).t,'?');
              (yyval.value).c = (yyvsp[(1) - (5)].value).c;
              code_t*j1 = (yyval.value).c = abc_iffalse((yyval.value).c, 0);
              (yyval.value).c = code_append((yyval.value).c, (yyvsp[(3) - (5)].value).c);
              (yyval.value).c = converttype((yyval.value).c, (yyvsp[(3) - (5)].value).t, (yyval.value).t);
              code_t*j2 = (yyval.value).c = abc_jump((yyval.value).c, 0);
              (yyval.value).c = j1->branch = abc_label((yyval.value).c);
              (yyval.value).c = code_append((yyval.value).c, (yyvsp[(5) - (5)].value).c);
              (yyval.value).c = converttype((yyval.value).c, (yyvsp[(5) - (5)].value).t, (yyval.value).t);
              (yyval.value).c = j2->branch = abc_label((yyval.value).c);
            }
    }
    break;


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3556 "parser.y"
    { code_t*c = 0;
             classinfo_t*type = (yyvsp[(1) - (2)].value).t;
             if(is_getlocal((yyvsp[(1) - (2)].value).c) && (TYPE_IS_INT((yyvsp[(1) - (2)].value).t) || TYPE_IS_NUMBER((yyvsp[(1) - (2)].value).t))) {
                 int nr = getlocalnr((yyvsp[(1) - (2)].value).c);
                 code_free((yyvsp[(1) - (2)].value).c);(yyvsp[(1) - (2)].value).c=0;
                 if(TYPE_IS_INT((yyvsp[(1) - (2)].value).t)) {
                    (yyval.value).c = abc_getlocal(0, nr);
                    (yyval.value).c = abc_inclocal_i((yyval.value).c, nr);
                 } else if(TYPE_IS_NUMBER((yyvsp[(1) - (2)].value).t)) {
                    (yyval.value).c = abc_getlocal(0, nr);
                    (yyval.value).c = abc_inclocal((yyval.value).c, nr);
                 } else syntaxerror("internal error");
             } else {
                 if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                     c=abc_increment_i(c);
                     type = TYPE_INT;
                 } else {
                     c=abc_increment(c);
                     type = TYPE_NUMBER;
                 }
                 c=converttype(c, type, (yyvsp[(1) - (2)].value).t);
                 (yyval.value).c = toreadwrite((yyvsp[(1) - (2)].value).c, c, 0, 1, 1);
                 (yyval.value).t = (yyvsp[(1) - (2)].value).t;
             }
           }
    }
    break;


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3583 "parser.y"
    { code_t*c = 0;
             classinfo_t*type = (yyvsp[(1) - (2)].value).t;
             if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                 c=abc_decrement_i(c);
                 type = TYPE_INT;
             } else {
                 c=abc_decrement(c);
                 type = TYPE_NUMBER;
             }
             c=converttype(c, type, (yyvsp[(1) - (2)].value).t);
             (yyval.value).c = toreadwrite((yyvsp[(1) - (2)].value).c, c, 0, 1, 1);
             (yyval.value).t = (yyvsp[(1) - (2)].value).t;
            }
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3597 "parser.y"
    { code_t*c = 0;
             classinfo_t*type = (yyvsp[(2) - (2)].value).t;
             if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                 c=abc_increment_i(c);
                 type = TYPE_INT;
             } else {
                 c=abc_increment(c);
                 type = TYPE_NUMBER;
             }
             c=converttype(c, type, (yyvsp[(2) - (2)].value).t);
             (yyval.value).c = toreadwrite((yyvsp[(2) - (2)].value).c, c, 0, 0, 1);
             (yyval.value).t = (yyvsp[(2) - (2)].value).t;
           }
    }
    break;


  
    case 286:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3611 "parser.y"
    { code_t*c = 0;
             classinfo_t*type = (yyvsp[(2) - (2)].value).t;
             if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                 c=abc_decrement_i(c);
                 type = TYPE_INT;
             } else {
                 c=abc_decrement(c);
                 type = TYPE_NUMBER;
             }
             c=converttype(c, type, (yyvsp[(2) - (2)].value).t);
             (yyval.value).c = toreadwrite((yyvsp[(2) - (2)].value).c, c, 0, 0, 1);
             (yyval.value).t = (yyvsp[(2) - (2)].value).t;
           }
    }
    break;


  
    case 287:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3626 "parser.y"
    { if(!state->cls->info)
                  syntaxerror("super keyword not allowed outside a class");
              classinfo_t*t = state->cls->info->superclass;
              if(!t) t = TYPE_OBJECT;

              memberinfo_t*f = findmember_nsset(t, (yyvsp[(3) - (3)].id), 1);

              MEMBER_MULTINAME(m, f, (yyvsp[(3) - (3)].id));
              (yyval.value).c = 0;
              (yyval.value).c = abc_getlocal_0((yyval.value).c);
              (yyval.value).c = abc_getsuper2((yyval.value).c, &m);
              (yyval.value).t = slotinfo_gettype((slotinfo_t*)f);
           }
    }
    break;


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3640 "parser.y"
    {
              // attribute TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored @ operator");
           }
    }
    break;


  
    case 289:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3647 "parser.y"
    {
              // child attribute  TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored .@ operator");
           }
    }
    break;


  
    case 290:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3654 "parser.y"
    {
              // namespace declaration TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored :: operator");
           }
    }
    break;


  
    case 291:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3661 "parser.y"
    {
              // descendants TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored .. operator");
           }
    }
    break;


  
    case 292:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3668 "parser.y"
    {
              // filter TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored .() operator");
           }
    }
    break;


  
    case 293:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3683 "parser.y"
    {
    (yyval.value).c = (yyvsp[(1) - (3)].value).c;
    classinfo_t*t = (yyvsp[(1) - (3)].value).t;
    char is_static = 0;
    if(TYPE_IS_CLASS(t) && t->data) {
        t = t->data;
        is_static = 1;
    }
    if(t) {
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
                as3_warning("Access of undefined property '%s' in %s", (yyvsp[(3) - (3)].id), t->name);
            }
            
            MEMBER_MULTINAME(m, f, (yyvsp[(3) - (3)].id));
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
        }
        /* determine type */
        (yyval.value).t = slotinfo_gettype((slotinfo_t*)f);
        if(!(yyval.value).t)
           (yyval.value).c = abc_coerce_a((yyval.value).c);
    } else if((yyvsp[(1) - (3)].value).c && (yyvsp[(1) - (3)].value).c->opcode == OPCODE___PUSHPACKAGE__) {
        string_t*package = (yyvsp[(1) - (3)].value).c->data[0];
        char*package2 = concat3(package->str, ".", (yyvsp[(3) - (3)].id));

        slotinfo_t*a = registry_find(package->str, (yyvsp[(3) - (3)].id));
        if(a) {
            (yyval.value) = push_class(a);
        } else if(dict_contains(state->import_toplevel_packages, package2) ||
                  registry_ispackage(package2)) {
            (yyval.value).c = (yyvsp[(1) - (3)].value).c;
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
        (yyval.value).t = registry_getanytype();
    }
}
    }
    break;


  
    case 294:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3740 "parser.y"
    {
    PASS1
    /* Queue unresolved identifiers for checking against the parent
       function's variables.
       We consider everything which is not a local variable "unresolved".
       This encompasses class names, members of the surrounding class
       etc. which is *correct* because local variables of the parent function
       would shadow those.
       */
    if(state->method->inner && !find_variable(state, (yyvsp[(1) - (1)].id))) {
        unknown_variable((yyvsp[(1) - (1)].id));
    }
   
    /* let the compiler know that it might want to check the current directory/package
       for this identifier- maybe there's a file $1.as defining $1. */
    as3_schedule_class_noerror(state->package, (yyvsp[(1) - (1)].id));
    PASS2

    (yyval.value).t = 0;
    (yyval.value).c = 0;
    slotinfo_t*a = 0;
    memberinfo_t*f = 0;

    variable_t*v;
    /* look at variables */
    if((v = find_variable(state, (yyvsp[(1) - (1)].id)))) {
        // $1 is a local variable
        (yyval.value).c = abc_getlocal((yyval.value).c, v->index);
        (yyval.value).t = v->type;
        break;
    }
    if((v = find_slot(state, (yyvsp[(1) - (1)].id)))) {
        (yyval.value).c = abc_getscopeobject((yyval.value).c, 1);
        (yyval.value).c = abc_getslot((yyval.value).c, v->index);
        (yyval.value).t = v->type;
        break;
    }

    int i_am_static = (state->method && state->method->info)?(state->method->info->flags&FLAG_STATIC):FLAG_STATIC;

    /* look at current class' members */
    if(!state->method->inner && 
        state->cls && 
        (f = findmember_nsset(state->cls->info, (yyvsp[(1) - (1)].id), 1)) &&
        (f->flags&FLAG_STATIC) >= i_am_static) 
    {
        // $1 is a function in this class
        int var_is_static = (f->flags&FLAG_STATIC);

        if(f->kind == INFOTYPE_METHOD) {
            (yyval.value).t = TYPE_FUNCTION(f);
        } else {
            (yyval.value).t = f->type;
        }
        if(var_is_static && !i_am_static) {
        /* access to a static member from a non-static location.
           do this via findpropstrict:
           there doesn't seem to be any non-lookup way to access
           static properties of a class */
            state->method->late_binding = 1;
            (yyval.value).t = f->type;
            namespace_t ns = {f->access, f->package};
            multiname_t m = {QNAME, &ns, 0, (yyvsp[(1) - (1)].id)};
            (yyval.value).c = abc_findpropstrict2((yyval.value).c, &m);
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
            break;
        } else if(f->slot>0) {
            (yyval.value).c = abc_getlocal_0((yyval.value).c);
            (yyval.value).c = abc_getslot((yyval.value).c, f->slot);
            break;
        } else {
            namespace_t ns = {f->access, f->package};
            multiname_t m = {QNAME, &ns, 0, (yyvsp[(1) - (1)].id)};
            (yyval.value).c = abc_getlocal_0((yyval.value).c);
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
            break;
        }
    } 
    
    /* look at actual classes, in the current package and imported */
    if((a = find_class((yyvsp[(1) - (1)].id)))) {
        (yyval.value) = push_class(a);
        break;
    }

    /* look through package prefixes */
    if(dict_contains(state->import_toplevel_packages, (yyvsp[(1) - (1)].id)) || 
       registry_ispackage((yyvsp[(1) - (1)].id))) {
        (yyval.value).c = abc___pushpackage__((yyval.value).c, (yyvsp[(1) - (1)].id));
        (yyval.value).t = 0;
        break;
    }

    /* unknown object, let the avm2 resolve it */
    if(1) {
        //as3_softwarning("Couldn't resolve '%s', doing late binding", $1);
        as3_warning("Couldn't resolve '%s', doing late binding", (yyvsp[(1) - (1)].id));
        state->method->late_binding = 1;
                
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(1) - (1)].id)};

        (yyval.value).t = 0;
        (yyval.value).c = abc_findpropstrict2((yyval.value).c, &m);
        (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
    }
}
    }
    break;


  
    case 295:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3849 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (2)].id);
    n->url = (yyvsp[(2) - (2)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 296:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3856 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 297:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3863 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].str).str;
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 298:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3870 "parser.y"
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

    (yyval.code)=0;
}
    }
    break;


  
    case 299:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3894 "parser.y"
    {
    PASS12
    const char*url = (yyvsp[(3) - (3)].classinfo)->name;

    varinfo_t*s = (varinfo_t*)(yyvsp[(3) - (3)].classinfo);
    if(s->kind == INFOTYPE_UNRESOLVED) {
        s = (varinfo_t*)registry_resolve((slotinfo_t*)s);
        if(!s)
            syntaxerror("Couldn't resolve namespace %s", (yyvsp[(3) - (3)].classinfo)->name);
    }

    if(!s || s->kind != INFOTYPE_SLOT)
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
#line 8242 "parser.tab.c"
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



