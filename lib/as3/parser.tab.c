
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
#include "tokenizer.h"
#include "registry.h"
#include "code.h"
#include "opcodes.h"
#include "compiler.h"

extern int a3_lex();



/* Line 198 of skeleton.m4  */
#line 99 "parser.tab.c"

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
     T_BYTE = 265,
     T_SHORT = 266,
     T_FLOAT = 267,
     T_FOR = 268,
     T_WHILE = 269,
     T_DO = 270,
     T_SWITCH = 271,
     KW_IMPLEMENTS = 272,
     KW_NAMESPACE = 273,
     KW_PACKAGE = 274,
     KW_PROTECTED = 275,
     KW_PUBLIC = 276,
     KW_PRIVATE = 277,
     KW_USE = 278,
     KW_INTERNAL = 279,
     KW_NEW = 280,
     KW_NATIVE = 281,
     KW_FUNCTION = 282,
     KW_FINALLY = 283,
     KW_UNDEFINED = 284,
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
#line 44 "parser.y"
tokenunion
{

/* Line 223 of skeleton.m4  */
#line 44 "parser.y"

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



/* Line 223 of skeleton.m4  */
#line 277 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 263 "parser.y"


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



/* Line 273 of skeleton.m4  */
#line 1916 "parser.tab.c"

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
#define YYLAST   2992

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  304
/* YYNRULES -- Number of states.  */
#define YYNSTATES  514

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
       2,     2,     2,   118,     2,     2,     2,   116,   108,     2,
     122,   133,   115,   113,   100,   112,   127,   114,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   105,    99,
     109,   102,   110,   104,   128,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   124,     2,   125,   107,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   126,   106,   132,   117,     2,     2,     2,
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
      95,    96,    97,    98,   101,   103,   111,   119,   120,   121,
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
     275,   277,   280,   282,   283,   290,   293,   295,   301,   303,
     305,   309,   311,   312,   319,   320,   326,   329,   334,   335,
     337,   339,   342,   344,   346,   348,   350,   352,   354,   356,
     358,   360,   362,   363,   366,   367,   370,   371,   374,   375,
     385,   386,   395,   396,   398,   400,   403,   405,   410,   412,
     414,   416,   417,   419,   421,   424,   426,   429,   438,   440,
     442,   443,   450,   451,   454,   456,   458,   460,   462,   464,
     466,   468,   470,   472,   473,   475,   478,   483,   487,   489,
     494,   497,   499,   501,   502,   503,   516,   518,   519,   520,
     531,   533,   537,   539,   541,   543,   547,   549,   551,   553,
     556,   557,   558,   562,   563,   565,   567,   569,   572,   575,
     576,   581,   586,   591,   594,   596,   599,   601,   603,   607,
     609,   611,   613,   615,   617,   619,   621,   623,   625,   627,
     629,   631,   633,   635,   637,   639,   641,   643,   647,   651,
     655,   659,   663,   667,   671,   675,   679,   683,   686,   689,
     693,   697,   701,   705,   709,   713,   717,   721,   725,   729,
     733,   737,   741,   745,   749,   754,   757,   759,   763,   766,
     771,   775,   776,   778,   782,   788,   792,   796,   800,   804,
     808,   812,   816,   820,   824,   828,   832,   836,   842,   845,
     848,   851,   854,   858,   861,   866,   872,   876,   882,   886,
     888,   891,   894,   899,   904
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     135,     0,    -1,   136,    -1,    -1,   137,    -1,   138,    -1,
     137,   138,    -1,   185,    -1,   197,    -1,   195,    -1,   214,
      -1,   206,    -1,   147,    -1,   148,   126,   136,   132,    -1,
      99,    -1,    -1,   140,    -1,   141,    -1,   140,   141,    -1,
     197,    -1,   195,    -1,   214,    -1,   206,    -1,   147,    -1,
     148,   126,   139,   132,    -1,    99,    -1,   143,    -1,    -1,
     143,   145,    -1,   145,    -1,   188,    -1,   159,    -1,   160,
      -1,   161,    -1,   163,    -1,   171,    -1,   154,    -1,   182,
      -1,   179,    -1,   236,    -1,   244,    -1,   243,    -1,   126,
     143,   132,    -1,   126,   132,    -1,    99,    -1,   144,    -1,
     150,    -1,   165,    -1,   166,    -1,   233,    -1,   181,    -1,
     148,   126,   143,   132,    -1,   145,    99,    -1,   145,    -1,
     144,    -1,     3,    89,     3,    -1,   102,   234,    -1,    -1,
      46,   151,    -1,    32,   151,    -1,   152,    -1,   151,   100,
     152,    -1,     3,   224,   149,    -1,    -1,    65,   146,    -1,
      -1,    64,   122,   155,   235,   133,   146,   153,    -1,    -1,
     150,    -1,   236,    -1,    46,     3,   224,    -1,     3,    -1,
      13,   122,    -1,    13,    50,   122,    -1,   158,   156,    99,
     235,    99,   236,   133,   146,    -1,   158,   157,    68,   235,
     133,   146,    -1,    -1,    14,   122,   162,   235,   133,   146,
      -1,    -1,    15,   164,   146,    14,   122,   235,   133,    -1,
      66,    -1,    66,     3,    -1,    30,    -1,    30,     3,    -1,
      -1,   168,    -1,   170,    -1,   168,   170,    -1,   169,    -1,
     168,   169,    -1,    34,   237,   105,   142,    -1,    62,   105,
     142,    -1,    -1,    16,   122,   172,   237,   133,   126,   167,
     132,    -1,    -1,    33,   122,     3,   224,   133,   174,   126,
     142,   132,    -1,    -1,    28,   126,   176,   142,   132,    -1,
     173,    -1,   177,   173,    -1,   177,    -1,   177,   175,    -1,
     175,    -1,    -1,    52,   126,   180,   142,   132,   178,    -1,
      37,   235,    -1,    37,    -1,    39,   122,   235,   133,   146,
      -1,     3,    -1,    19,    -1,   184,   127,   183,    -1,   183,
      -1,    -1,    19,   184,   126,   186,   139,   132,    -1,    -1,
      19,   126,   187,   139,   132,    -1,    41,   220,    -1,    41,
     184,   127,   115,    -1,    -1,   190,    -1,   191,    -1,   190,
     191,    -1,    21,    -1,    22,    -1,    20,    -1,    38,    -1,
      47,    -1,    49,    -1,    48,    -1,    26,    -1,    24,    -1,
       4,    -1,    -1,    54,   221,    -1,    -1,    54,   222,    -1,
      -1,    17,   222,    -1,    -1,   189,    31,     3,   192,   194,
     126,   196,   199,   132,    -1,    -1,   189,    44,     3,   193,
     126,   198,   202,   132,    -1,    -1,   200,    -1,   201,    -1,
     200,   201,    -1,    99,    -1,   148,   126,   199,   132,    -1,
     206,    -1,   214,    -1,   144,    -1,    -1,   203,    -1,   204,
      -1,   203,   204,    -1,    99,    -1,    46,     3,    -1,   189,
      27,   213,     3,   122,   210,   133,   224,    -1,    46,    -1,
      32,    -1,    -1,   189,   205,     3,   207,   224,   149,    -1,
      -1,   102,   209,    -1,    10,    -1,     8,    -1,     9,    -1,
      12,    -1,     5,    -1,    56,    -1,    55,    -1,    45,    -1,
       3,    -1,    -1,   211,    -1,    93,   212,    -1,   211,   100,
      93,   212,    -1,   211,   100,   212,    -1,   212,    -1,     3,
     105,   223,   208,    -1,     3,   208,    -1,    51,    -1,    35,
      -1,    -1,    -1,   189,    27,   213,     3,   122,   210,   133,
     224,   126,   215,   142,   132,    -1,     3,    -1,    -1,    -1,
      27,   216,   122,   210,   133,   224,   126,   218,   142,   132,
      -1,     3,    -1,   184,   127,     3,    -1,   220,    -1,   219,
      -1,   221,    -1,   222,   100,   221,    -1,   221,    -1,   115,
      -1,    36,    -1,   105,   223,    -1,    -1,    -1,   122,   226,
     133,    -1,    -1,   227,    -1,   228,    -1,   234,    -1,   227,
     100,    -1,   228,   234,    -1,    -1,    25,   237,   229,   225,
      -1,   237,   122,   226,   133,    -1,    53,   122,   226,   133,
      -1,    63,   237,    -1,    42,    -1,    42,   235,    -1,   237,
      -1,   237,    -1,   235,   100,   237,    -1,   235,    -1,   217,
      -1,   238,    -1,   241,    -1,   230,    -1,   232,    -1,   231,
      -1,     6,    -1,    10,    -1,    11,    -1,     8,    -1,     9,
      -1,    12,    -1,     5,    -1,    29,    -1,    56,    -1,    55,
      -1,    45,    -1,   237,   109,   237,    -1,   237,   110,   237,
      -1,   237,    75,   237,    -1,   237,    76,   237,    -1,   237,
      71,   237,    -1,   237,    72,   237,    -1,   237,    74,   237,
      -1,   237,    73,   237,    -1,   237,    87,   237,    -1,   237,
      88,   237,    -1,   118,   237,    -1,   117,   237,    -1,   237,
     108,   237,    -1,   237,   107,   237,    -1,   237,   106,   237,
      -1,   237,    96,   237,    -1,   237,    95,   237,    -1,   237,
      94,   237,    -1,   237,   114,   237,    -1,   237,   116,   237,
      -1,   237,   113,   237,    -1,   237,   112,   237,    -1,   237,
     115,   237,    -1,   237,    68,   237,    -1,   237,    69,   237,
      -1,   237,    40,   237,    -1,   237,    67,   237,    -1,    43,
     122,   237,   133,    -1,    36,   237,    -1,    36,    -1,   122,
     235,   133,    -1,   112,   237,    -1,   237,   124,   237,   125,
      -1,   124,   226,   125,    -1,    -1,   240,    -1,   234,   105,
     234,    -1,   240,   100,   234,   105,   234,    -1,    70,   239,
     132,    -1,   237,    80,   237,    -1,   237,    79,   237,    -1,
     237,    85,   237,    -1,   237,    84,   237,    -1,   237,    86,
     237,    -1,   237,    78,   237,    -1,   237,    77,   237,    -1,
     237,    83,   237,    -1,   237,    81,   237,    -1,   237,    82,
     237,    -1,   237,   102,   237,    -1,   237,   104,   237,   105,
     237,    -1,   237,    91,    -1,   237,    90,    -1,    91,   237,
      -1,    90,   237,    -1,    53,   127,     3,    -1,   128,     3,
      -1,   237,   127,   128,     3,    -1,   237,   127,     3,    89,
       3,    -1,   237,    92,     3,    -1,   237,   127,   122,   237,
     133,    -1,   237,   127,     3,    -1,     3,    -1,    18,     3,
      -1,   189,   242,    -1,   189,   242,   102,     3,    -1,   189,
     242,   102,     5,    -1,    23,   220,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1893,  1893,  1895,  1895,  1896,  1897,  1899,  1900,  1901,
    1902,  1903,  1904,  1905,  1906,  1908,  1908,  1909,  1910,  1912,
    1913,  1914,  1915,  1916,  1917,  1918,  1920,  1921,  1923,  1924,
    1927,  1928,  1929,  1930,  1931,  1932,  1933,  1934,  1935,  1936,
    1937,  1938,  1939,  1940,  1943,  1944,  1945,  1946,  1947,  1948,
    1949,  1950,  1954,  1955,  1959,  1966,  1970,  1971,  1975,  1976,
    1978,  1979,  1981,  2042,  2043,  2046,  2046,  2065,  2066,  2067,
    2072,  2076,  2081,  2082,  2084,  2104,  2148,  2148,  2167,  2167,
    2182,  2185,  2188,  2191,  2195,  2196,  2197,  2198,  2199,  2200,
    2202,  2213,  2216,  2216,  2245,  2245,  2269,  2269,  2285,  2286,
    2287,  2288,  2296,  2305,  2305,  2350,  2354,  2365,  2374,  2375,
    2377,  2378,  2380,  2380,  2382,  2382,  2385,  2400,  2416,  2417,
    2418,  2419,  2427,  2428,  2429,  2430,  2431,  2432,  2433,  2434,
    2435,  2436,  2440,  2441,  2443,  2444,  2446,  2447,  2451,  2449,
    2457,  2455,  2464,  2465,  2466,  2467,  2468,  2469,  2470,  2471,
    2473,  2479,  2480,  2481,  2482,  2483,  2484,  2487,  2500,  2500,
    2502,  2502,  2577,  2578,  2580,  2581,  2582,  2583,  2584,  2586,
    2587,  2588,  2589,  2598,  2602,  2608,  2614,  2622,  2627,  2633,
    2641,  2649,  2650,  2651,  2654,  2653,  2670,  2671,  2673,  2672,
    2696,  2707,  2720,  2721,  2723,  2724,  2726,  2727,  2728,  2737,
    2738,  2742,  2743,  2745,  2746,  2747,  2749,  2753,  2754,  2759,
    2760,  2796,  2842,  2863,  2884,  2887,  2894,  2895,  2896,  2902,
    2908,  2910,  2912,  2914,  2916,  2918,  2920,  2937,  2942,  2945,
    2948,  2951,  2954,  2957,  2960,  2963,  2966,  2970,  2973,  2976,
    2979,  2982,  2985,  2988,  2991,  2995,  3006,  3024,  3029,  3034,
    3039,  3044,  3049,  3053,  3057,  3062,  3066,  3070,  3079,  3088,
    3098,  3103,  3115,  3121,  3126,  3132,  3138,  3142,  3144,  3155,
    3164,  3171,  3172,  3174,  3180,  3189,  3196,  3208,  3214,  3220,
    3226,  3232,  3238,  3244,  3250,  3263,  3274,  3281,  3294,  3321,
    3335,  3349,  3363,  3378,  3385,  3392,  3399,  3406,  3417,  3451,
    3572,  3578,  3581,  3584,  3587
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IDENTIFIER", "T_NAMESPACE",
  "T_STRING", "T_REGEXP", "T_EMPTY", "T_INT", "T_UINT", "T_BYTE",
  "T_SHORT", "T_FLOAT", "\"for\"", "\"while\"", "\"do\"", "\"switch\"",
  "\"implements\"", "\"namespace\"", "\"package\"", "\"protected\"",
  "\"public\"", "\"private\"", "\"use\"", "\"internal\"", "\"new\"",
  "\"native\"", "\"function\"", "\"finally\"", "\"undefined\"",
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
  "CATCH_LIST", "CATCH_FINALLY_LIST", "TRY", "$@7", "THROW", "WITH",
  "X_IDENTIFIER", "PACKAGE", "PACKAGE_DECLARATION", "$@8", "$@9", "IMPORT",
  "MAYBE_MODIFIERS", "MODIFIER_LIST", "MODIFIER", "EXTENDS",
  "EXTENDS_LIST", "IMPLEMENTS_LIST", "CLASS_DECLARATION", "$@10",
  "INTERFACE_DECLARATION", "$@11", "MAYBE_CLASS_BODY", "CLASS_BODY",
  "CLASS_BODY_ITEM", "MAYBE_INTERFACE_BODY", "INTERFACE_BODY",
  "IDECLARATION", "VARCONST", "SLOT_DECLARATION", "$@12",
  "MAYBESTATICCONSTANT", "STATICCONSTANT", "MAYBE_PARAM_LIST",
  "PARAM_LIST", "PARAM", "GETSET", "FUNCTION_DECLARATION", "$@13",
  "MAYBE_IDENTIFIER", "INNERFUNCTION", "$@14", "CLASS", "PACKAGEANDCLASS",
  "CLASS_SPEC", "CLASS_SPEC_LIST", "TYPE", "MAYBETYPE",
  "MAYBE_PARAM_VALUES", "MAYBE_EXPRESSION_LIST", "EXPRESSION_LIST",
  "EXPRESSION_LIST_AND_COMMA", "XX", "NEW", "FUNCTIONCALL", "DELETE",
  "RETURN", "NONCOMMAEXPRESSION", "EXPRESSION", "VOIDEXPRESSION", "E",
  "CONSTANT", "MAYBE_EXPRPAIR_LIST", "EXPRPAIR_LIST", "VAR_READ",
  "NAMESPACE_ID", "NAMESPACE_DECLARATION", "USE_NAMESPACE", 0
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
      44,   354,    61,   355,    63,    58,   124,    94,    38,    60,
      62,   356,    45,    43,    47,    42,    37,   126,    33,   357,
     358,   359,    40,   360,    91,    93,   123,    46,    64,   361,
     362,   363,   125,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
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
     178,   178,   178,   180,   179,   181,   181,   182,   183,   183,
     184,   184,   186,   185,   187,   185,   188,   188,   189,   189,
     190,   190,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   192,   192,   193,   193,   194,   194,   196,   195,
     198,   197,   199,   199,   200,   200,   201,   201,   201,   201,
     201,   202,   202,   203,   203,   204,   204,   204,   205,   205,
     207,   206,   208,   208,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   210,   210,   210,   210,   211,   211,   212,
     212,   213,   213,   213,   215,   214,   216,   216,   218,   217,
     219,   220,   221,   221,   222,   222,   223,   223,   223,   224,
     224,   225,   225,   226,   226,   226,   227,   228,   227,   229,
     230,   231,   231,   232,   233,   233,   234,   235,   235,   236,
     237,   237,   237,   237,   237,   237,   237,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   239,   239,   240,   240,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   241,
     242,   243,   243,   243,   244
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
       1,     2,     1,     0,     6,     2,     1,     5,     1,     1,
       3,     1,     0,     6,     0,     5,     2,     4,     0,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     2,     0,     2,     0,     2,     0,     9,
       0,     8,     0,     1,     1,     2,     1,     4,     1,     1,
       1,     0,     1,     1,     2,     1,     2,     8,     1,     1,
       0,     6,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     2,     4,     3,     1,     4,
       2,     1,     1,     0,     0,    12,     1,     0,     0,    10,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     2,
       0,     0,     3,     0,     1,     1,     1,     2,     2,     0,
       4,     4,     4,     2,     1,     2,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     2,     1,     3,     2,     4,
       3,     0,     1,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     5,     2,     2,
       2,     2,     3,     2,     4,     5,     3,     5,     3,     1,
       2,     2,     4,     4,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     118,   299,   131,   232,   226,   229,   230,   227,   228,   231,
       0,     0,    78,     0,     0,   124,   122,   123,     0,   130,
       0,   129,   187,   233,   266,   125,     0,     0,     0,   236,
     126,   128,   127,     0,     0,   235,   234,     0,     0,   271,
       0,     0,    14,     0,     0,     0,     0,   203,   118,     0,
       0,     2,   118,     5,    54,    12,     0,    36,    67,    31,
      32,    33,    34,    35,    38,    37,     7,    30,     0,   119,
     120,     9,     8,    11,    10,   220,   223,   225,   224,   219,
      39,   217,   221,   222,    41,    40,     0,     0,    72,    76,
     118,    92,   108,   109,   114,   111,     0,     0,   304,   299,
     209,   186,     0,   265,     0,     0,   116,     0,   103,   203,
       0,   213,    65,     0,   216,     0,   272,   291,   290,   268,
     248,   247,     0,     0,   204,   205,   206,    82,     0,   106,
     214,     0,    80,    44,    43,   118,    45,    29,     0,    46,
      47,    48,    50,     0,    49,   293,     1,     6,   118,   299,
       0,    68,     0,     0,    69,     0,   183,     0,   159,     0,
     158,     0,   301,   121,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   289,   288,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   203,     0,     0,    55,    73,
       0,    53,     0,     0,   118,   112,     0,     0,   201,   173,
       0,     0,     0,    27,     0,   292,     0,     0,   275,     0,
     267,   270,   207,   208,    83,   200,    59,    60,   105,   215,
      58,    81,    42,    28,   118,     0,   200,     0,     0,   300,
     182,   181,     0,   132,   134,   160,     0,   218,   262,   263,
     260,   261,   241,   242,   244,   243,   239,   240,   282,   281,
     277,   276,   284,   285,   283,   279,   278,   280,   245,   246,
     296,   254,   253,   252,   286,     0,   251,   250,   249,   237,
     238,   258,   257,   255,   259,   256,     0,     0,   298,     0,
       0,     0,    52,     0,     0,    25,     0,   118,    17,    23,
       0,    20,    19,    22,    21,   118,   110,   191,   203,   210,
     162,     0,     0,   174,   178,   118,   117,   264,     0,    26,
     212,     0,   273,     0,     0,    57,     0,   118,    13,    57,
       0,     0,     0,     0,   136,     0,     0,   200,   302,   303,
       0,   211,   269,     0,     0,   294,   118,     0,     0,   115,
      18,   118,     0,     0,     0,     0,   180,   175,   200,     0,
     107,     0,   118,     0,   190,   198,   197,   193,   192,   196,
     199,     0,    62,    61,    51,     0,   118,   173,   133,     0,
       0,   194,   135,   140,    57,   287,   295,   297,    77,     0,
      84,     0,   113,   202,   172,   168,   165,   166,   164,   167,
     171,   170,   169,   163,   162,     0,     0,   177,     0,     0,
      98,   102,   100,   104,    63,   274,    56,     0,    75,     0,
     137,   138,     0,   118,   161,    79,     0,     0,     0,    85,
      88,    86,    24,   179,   188,   176,    96,     0,    99,   101,
     118,    66,   118,   200,   118,   195,     0,   155,     0,     0,
     118,   153,     0,    27,    93,    89,    87,    27,    27,   200,
      64,    74,     0,   146,   150,     0,     0,     0,   118,   144,
     148,   149,   156,   183,   141,   154,    27,    91,     0,     0,
       0,   184,   118,   139,   145,     0,    90,   189,    97,    94,
      27,     0,     0,     0,     0,   147,   173,    27,   185,     0,
       0,   200,    95,   157
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,    53,   306,   307,   308,   328,   329,
     136,   137,   212,   309,   138,   382,   139,   240,   237,   451,
      57,   226,   152,   153,    58,    59,    60,    61,   210,    62,
      90,   140,   141,   438,   439,   440,   441,    63,   213,   420,
     503,   421,   468,   422,   423,    64,   223,   142,    65,    95,
      97,    66,   315,   214,    67,   143,    69,    70,   344,   346,
     390,   311,   454,   312,   433,   477,   478,   479,   459,   460,
     461,   161,   313,   347,   366,   413,   322,   323,   324,   252,
     314,   500,   102,    75,   467,   377,   378,   379,   392,   380,
     335,   319,   123,   124,   125,   218,    76,    77,    78,   144,
     126,    79,    80,    81,    82,   115,   116,    83,   162,    84,
      85
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -426
static const yytype_int16 yypact[] =
{
    1667,   -42,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,
     -37,   -73,  -426,   -27,    11,  -426,  -426,  -426,    71,  -426,
    2087,  -426,    78,  -426,  2157,  -426,     9,    71,    17,  -426,
    -426,  -426,  -426,    -2,    -4,  -426,  -426,  2087,    25,  2087,
    2087,  2087,  -426,  2087,  2087,  2087,  2087,  2087,   627,   138,
     143,  -426,   497,  -426,  -426,  -426,    27,  -426,  2017,  -426,
    -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,   417,   216,
    -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,    55,
    -426,  2499,  -426,  -426,  -426,  -426,   197,    82,  -426,  -426,
    1919,  -426,  -426,  -426,  -426,  -426,    19,    75,  -426,  -426,
     -51,  -426,    85,   -20,  2087,    84,  -426,  2087,  -426,  2087,
     209,   -20,  -426,   117,  2499,    91,   113,   -20,   -20,   361,
     -20,   -20,   -56,    99,   126,  2087,  -426,   224,   225,  2087,
    2087,   225,   226,  -426,  -426,   757,  -426,  -426,   104,  -426,
    -426,  -426,  -426,   214,  -426,  -426,  -426,  -426,  1017,   163,
     231,  -426,   136,   175,  -426,   241,    45,   245,  -426,   246,
    -426,   247,   149,  -426,  2087,  2087,  2087,  2087,  2087,  2087,
    2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,
    2087,  2087,  2087,  2087,  2087,  2087,  2087,  -426,  -426,   249,
    2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,
    2087,  2087,  2087,  2087,  2087,  2087,  2087,     5,  -426,  -426,
    2087,   159,   248,  2087,  1147,  -426,    71,    81,   144,    16,
     -54,    15,   307,  1793,   127,  -426,  2087,  2087,  -426,  2087,
    -426,  -426,  -426,  -426,  -426,   162,   168,  -426,    55,    55,
     168,  -426,  -426,  -426,  1919,   137,   162,  2087,  2087,  -426,
    -426,  -426,   267,   217,   220,  -426,    23,  2499,  1009,   489,
     489,   489,  2865,  2865,  2865,  2865,  1009,  1009,  2499,  2499,
    2499,  2499,  2499,  2499,  2499,  2499,  2499,  2499,  2560,  2621,
    -426,   213,   213,   213,  2499,  2316,  2682,  2743,  2804,  1009,
    1009,   361,   361,   -20,   -20,   -20,   145,  2377,   187,  2087,
     276,   -50,  -426,   158,  2182,  -426,   153,  1277,  -426,  -426,
     160,  -426,  -426,  -426,  -426,  1147,  -426,   165,  2087,  -426,
     -36,   284,   156,   190,  -426,  1919,  -426,  -426,   164,  1793,
    -426,   -35,  -426,   193,    21,   198,   225,   887,  -426,   -43,
      80,   -14,   179,   103,   289,   103,   186,   162,  -426,  -426,
    2087,  -426,  -426,   310,  2253,  -426,  1919,  2087,   188,  -426,
    -426,  1147,   191,   185,   459,    21,  -426,  -426,   162,    20,
    -426,    -6,  1919,  2087,   165,  -426,  -426,  -426,  -426,  -426,
    -426,  2087,  -426,  -426,  -426,  2087,  1919,    16,  -426,   103,
     204,  -426,   233,  -426,   198,  2499,  -426,  -426,  -426,   -13,
      29,   192,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,
    -426,  -426,  -426,  -426,   229,   210,   284,  -426,   212,   219,
    -426,  -426,    -6,  -426,   274,  -426,  -426,   211,  -426,   215,
     233,  -426,   103,   235,  -426,  -426,  2087,   238,   218,    29,
    -426,  -426,  -426,  -426,  -426,  -426,  -426,   343,  -426,  -426,
    1919,  -426,  1919,   162,  1407,  -426,   346,  -426,   324,   222,
     273,  -426,  2438,  1793,  -426,  -426,  -426,  1793,  1793,   162,
    -426,  -426,   232,  -426,  -426,   234,   108,   223,  1537,  -426,
    -426,  -426,  -426,    45,  -426,  -426,  1793,  -426,   227,   237,
     244,  -426,  1407,  -426,  -426,   349,  -426,  -426,  -426,  -426,
    1793,   239,   242,   270,   268,  -426,    16,  1793,  -426,   271,
     275,   162,  -426,  -426
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -426,  -426,   258,  -426,   305,  -294,  -426,    63,  -425,   -41,
       1,   -84,  -308,    58,     2,    14,   352,   290,    76,  -426,
    -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,
    -426,  -426,  -426,  -426,  -426,   -15,   -12,  -426,  -426,     8,
    -426,    10,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -128,
      94,  -426,  -426,  -426,  -426,     0,  -426,   356,  -426,  -426,
    -426,    60,  -426,    62,  -426,   -66,  -426,   -40,  -426,  -426,
     -24,  -426,     3,  -426,    28,  -426,  -372,  -426,  -301,   -46,
       4,  -426,  -426,  -426,  -426,  -426,    98,  -251,    50,   100,
    -236,  -426,  -100,  -426,  -426,  -426,  -426,  -426,  -426,  -426,
     -28,    -1,   -53,    -8,  -426,  -426,  -426,  -426,  -426,  -426,
    -426
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -153
static const yytype_int16 yytable[] =
{
      68,    54,    56,    73,    74,   154,   211,   135,   298,   224,
     339,   113,   100,    87,    92,   429,   103,   370,   317,   320,
     367,   362,   418,   320,   374,   -70,   348,   419,   349,   111,
      93,   114,   117,   118,    93,   119,   120,   121,   487,   114,
      93,   189,   488,   489,   164,   122,   164,    86,   398,    89,
     164,   243,    68,    54,    56,    73,    74,   375,    55,   381,
      71,   496,    72,   436,   424,   164,   364,   401,   417,   365,
     187,   188,   189,   206,    92,   504,   207,   230,   428,   325,
     250,   101,   510,   356,   317,    88,   164,   164,   316,   316,
      93,   437,   388,   316,   391,    91,   251,   233,   372,   222,
      93,   114,   205,   220,   206,   296,   374,   207,    96,   321,
      55,   394,    71,   416,    72,   445,    98,   114,   109,   386,
     435,   105,    93,   110,   108,   106,   155,   299,   238,   239,
     326,   104,   415,   300,   509,   156,   376,    94,   391,   107,
     158,   145,   470,   146,   471,   215,   216,   112,    68,    54,
      56,    73,    74,   148,   160,   164,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   385,
     164,   455,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   114,   297,   332,
     208,   333,   217,   337,   209,   304,    55,   219,    71,   301,
      72,   221,   225,   229,    68,    54,   310,   472,   363,   114,
       2,   114,   227,   228,   231,   331,   232,   234,   235,   241,
     244,   -71,   155,   490,   246,   247,    15,    16,    17,     2,
      19,   211,    21,   248,   249,   243,   340,   341,   253,   254,
     255,   256,   280,   243,    25,    15,    16,    17,   302,    19,
     330,    21,   303,    30,    31,    32,   318,   334,   336,   338,
     342,   343,   211,    25,   345,   513,   353,     2,   351,   355,
     357,   456,    30,    31,    32,   359,   361,   320,   211,   368,
     369,   354,  -108,    15,    16,    17,   371,    19,   373,    21,
     381,   387,   211,   187,   188,   189,   389,    68,    54,   310,
     114,    25,   393,   396,   400,    68,    54,   310,   403,   456,
      30,    31,    32,   402,   442,   200,   201,   202,   203,   204,
     431,   364,   427,   432,   457,   205,   444,   206,   446,   450,
     207,   447,   395,   463,   452,   425,   469,   165,   453,   482,
     464,   483,   502,   426,   484,   493,   399,   147,   491,   497,
     492,    68,    54,   310,   506,   114,   211,  -151,   211,   498,
     360,   505,   457,   114,   166,   167,   168,   499,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   507,   187,   188,   189,
     508,   190,   191,   192,   511,  -152,   245,   512,   434,   193,
     151,   194,   383,   195,   196,   197,   198,   199,   236,   200,
     201,   202,   203,   204,   465,   163,   501,   466,   462,   205,
     448,   206,   449,   458,   207,   155,   485,   495,   494,   430,
     327,     0,   443,     0,   156,     0,     0,     0,   157,   158,
       0,   187,   188,   189,   476,   474,   475,   480,   481,     0,
     458,   159,   404,   160,   405,   414,     0,   406,   407,   408,
       0,   409,     0,     0,     0,   202,   203,   204,   476,   474,
     475,   480,   481,   205,     0,   206,     0,     0,   207,     0,
       0,     0,   476,   474,   475,   480,   481,    -4,     0,     0,
       1,     2,     3,     4,   410,     5,     6,     7,     8,     9,
      10,    11,    12,    13,   411,   412,    14,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,   165,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,  -153,  -153,  -153,     0,
      37,    38,     0,     0,   173,   174,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   187,
     188,   189,     0,   190,   191,   192,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,    42,     0,   198,   199,
       0,   200,   201,   202,   203,   204,     0,     0,     0,    43,
       0,   205,     0,   206,    44,    45,   207,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,    -4,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,    22,     0,    23,   127,     0,   128,
       0,     0,     0,    24,   129,    25,    26,     0,    27,   130,
      28,     0,    29,   131,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,   132,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,   134,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,    22,     0,    23,   127,     0,   128,
       0,     0,     0,    24,   129,    25,    26,     0,    27,   130,
      28,     0,    29,   131,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,   132,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,   242,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,    22,     0,    23,   127,     0,   128,
       0,     0,     0,    24,   129,    25,    26,     0,    27,   130,
      28,     0,    29,   131,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,   132,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,   384,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,  -153,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,  -153,  -153,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   187,
     188,   189,     0,   190,   191,   192,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,    42,     0,  -153,  -153,
       0,   200,   201,   202,   203,   204,     0,     0,     0,    43,
       0,   205,     0,   206,    44,    45,   207,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,    -3,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,     0,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   305,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,   -15,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,     0,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   305,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,   -16,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,     0,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,  -142,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,     0,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,    -3,     0,  -143,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,     0,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     1,     2,     3,     4,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,  -118,     0,    15,    16,    17,    18,    19,    20,    21,
      22,     0,    23,   127,     0,   128,     0,     0,     0,    24,
     129,    25,    26,     0,    27,   130,    28,     0,    29,   131,
      30,    31,    32,     0,     0,    33,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    37,    38,     0,   132,
       0,     0,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,    41,     0,     0,     0,     0,     0,
       0,     0,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
      44,    45,     0,     0,     0,    46,     0,    47,     0,    48,
       0,    49,     1,     2,     3,     4,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    15,
      16,    17,    18,    19,    20,    21,    22,     0,    23,   127,
       0,   128,     0,     0,     0,    24,   129,    25,    26,     0,
      27,   130,    28,     0,    29,   131,    30,    31,    32,     0,
       0,    33,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    37,    38,     0,   132,     0,     0,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,     0,     0,     0,   133,     0,
     149,     0,     3,     4,     0,     5,     6,     7,     8,     9,
       0,    43,     0,     0,     0,     0,    44,    45,     0,     0,
       0,    46,    20,    47,    22,    48,    23,    49,     0,   128,
       0,     0,     0,    24,     0,     0,     0,     0,     0,     0,
      28,     0,    29,   150,     0,     0,     0,     0,     0,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    39,     0,     0,
      99,     0,     3,     4,     0,     5,     6,     7,     8,     9,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,    20,     0,    22,     0,    23,     0,     0,     0,
       0,     0,     0,    24,     0,     0,     0,     0,     0,    43,
      28,     0,    29,     0,    44,    45,     0,     0,     0,    46,
      34,    47,    35,    36,     0,    49,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    39,     0,     0,
      99,     0,     3,     4,     0,     5,     6,     7,     8,     9,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,    20,     0,    22,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,    29,     0,    44,    45,     0,     0,     0,    46,
      34,    47,    35,    36,     0,    49,     0,     0,     0,     0,
       0,     0,   165,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,   166,
     167,   168,     0,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,   187,   188,   189,     0,   190,   191,   192,    46,
       0,    47,     0,     0,   193,    49,   194,     0,   195,   196,
     197,   198,   199,   165,   200,   201,   202,   203,   204,     0,
       0,     0,     0,     0,   205,     0,   206,     0,     0,   207,
       0,     0,     0,     0,     0,   358,     0,     0,     0,     0,
     166,   167,   168,     0,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,     0,   187,   188,   189,     0,   190,   191,   192,
       0,     0,     0,     0,     0,   193,   165,   194,     0,   195,
     196,   197,   198,   199,     0,   200,   201,   202,   203,   204,
       0,     0,     0,     0,     0,   205,     0,   206,     0,     0,
     207,     0,     0,   166,   167,   168,   397,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,   187,   188,   189,     0,
     190,   191,   192,     0,     0,     0,     0,   165,   193,     0,
     194,   350,   195,   196,   197,   198,   199,     0,   200,   201,
     202,   203,   204,     0,     0,     0,     0,     0,   205,     0,
     206,     0,     0,   207,   166,   167,   168,     0,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   187,   188,   189,
       0,   190,   191,   192,     0,     0,     0,     0,   165,   193,
       0,   194,     0,   195,   196,   197,   198,   199,     0,   200,
     201,   202,   203,   204,     0,     0,     0,     0,     0,   205,
       0,   206,   352,     0,   207,   166,   167,   168,     0,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   187,   188,
     189,     0,   190,   191,   192,     0,     0,     0,     0,   165,
     193,     0,   194,   486,   195,   196,   197,   198,   199,     0,
     200,   201,   202,   203,   204,     0,     0,     0,     0,     0,
     205,     0,   206,     0,     0,   207,   166,   167,   168,     0,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   187,
     188,   189,     0,   190,   191,   192,     0,     0,     0,     0,
     165,   193,     0,   194,     0,   195,   196,   197,   198,   199,
       0,   200,   201,   202,   203,   204,     0,     0,     0,     0,
       0,   205,     0,   206,     0,     0,   207,   166,   167,   168,
       0,   169,   170,   171,   172,   173,   174,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   186,     0,
     187,   188,   189,     0,   190,   191,   192,     0,     0,     0,
       0,   165,     0,     0,     0,     0,   195,   196,   197,   198,
     199,     0,   200,   201,   202,   203,   204,     0,     0,     0,
       0,     0,   205,     0,   206,     0,     0,   207,   166,   167,
     168,     0,   169,   170,   171,   172,   173,   174,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   187,   188,   189,     0,   190,   191,   192,     0,     0,
       0,     0,   165,     0,     0,     0,     0,   195,   196,   197,
     198,   199,     0,   200,   201,   202,   203,   204,     0,     0,
       0,     0,     0,   205,     0,   206,     0,     0,   207,   166,
     167,   168,     0,   169,   170,   171,   172,   173,   174,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187,   188,   189,     0,   190,   191,   192,     0,
       0,     0,     0,   165,     0,     0,     0,     0,     0,   196,
     197,   198,   199,     0,   200,   201,   202,   203,   204,     0,
       0,     0,     0,     0,   205,     0,   206,     0,     0,   207,
     166,   167,   168,     0,   169,   170,   171,   172,   173,   174,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   187,   188,   189,     0,   190,   191,   192,
       0,     0,     0,     0,   165,     0,     0,     0,     0,     0,
       0,   197,   198,   199,     0,   200,   201,   202,   203,   204,
       0,     0,     0,     0,     0,   205,     0,   206,     0,     0,
     207,   166,   167,   168,     0,   169,   170,   171,   172,   173,
     174,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   187,   188,   189,     0,   190,   191,
     192,     0,     0,     0,     0,   165,     0,     0,     0,     0,
       0,     0,  -153,   198,   199,     0,   200,   201,   202,   203,
     204,     0,     0,     0,     0,     0,   205,     0,   206,     0,
       0,   207,   166,   167,   168,     0,  -153,  -153,  -153,  -153,
     173,   174,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   187,   188,   189,     0,   190,
     191,   192,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   198,   199,     0,   200,   201,   202,
     203,   204,     0,     0,     0,     0,     0,   205,     0,   206,
       0,     0,   207
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,     0,    58,    90,    48,     3,   109,
     246,    39,    20,    50,     3,   387,    24,   325,     3,     3,
     321,   315,    28,     3,     3,    68,     3,    33,     5,    37,
      19,    39,    40,    41,    19,    43,    44,    45,   463,    47,
      19,    92,   467,   468,   100,    46,   100,    89,   356,   122,
     100,   135,    52,    52,    52,    52,    52,    36,     0,   102,
       0,   486,     0,    34,   372,   100,   102,   361,   369,   105,
      90,    91,    92,   124,     3,   500,   127,   133,   386,   133,
      35,     3,   507,   133,     3,   122,   100,   100,   216,   217,
      19,    62,   343,   221,   345,   122,    51,   125,   133,   107,
      19,   109,   122,   104,   124,   205,     3,   127,    14,    93,
      52,   347,    52,    93,    52,   416,    18,   125,   122,   133,
     133,    27,    19,   127,   126,    27,    18,   122,   129,   130,
     115,   122,   368,   128,   506,    27,   115,   126,   389,   122,
      32,     3,   450,     0,   452,   126,   127,   122,   148,   148,
     148,   148,   148,   126,    46,   100,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    99,
     100,   432,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   227,
       3,   229,   127,   244,   122,   213,   148,   122,   148,   210,
     148,   127,     3,   100,   214,   214,   214,   453,   318,   227,
       4,   229,   105,   132,   125,   226,   100,     3,     3,     3,
     126,    68,    18,   469,     3,    99,    20,    21,    22,     4,
      24,   325,    26,    68,     3,   329,   247,   248,     3,     3,
       3,   102,     3,   337,    38,    20,    21,    22,    99,    24,
     133,    26,    14,    47,    48,    49,   122,   105,   100,   132,
       3,    54,   356,    38,    54,   511,    89,     4,   133,     3,
     122,    46,    47,    48,    49,   132,   126,     3,   372,   133,
     100,   299,   127,    20,    21,    22,   132,    24,   105,    26,
     102,   122,   386,    90,    91,    92,    17,   307,   307,   307,
     318,    38,   126,     3,   126,   315,   315,   315,   133,    46,
      47,    48,    49,   132,   132,   112,   113,   114,   115,   116,
     126,   102,   385,   100,    99,   122,   126,   124,   126,    65,
     127,   122,   350,   105,   133,   373,     3,    40,   133,     3,
     132,    27,     3,   381,   132,   132,   357,    52,   126,   132,
     126,   361,   361,   361,   122,   373,   450,   132,   452,   132,
     307,   132,    99,   381,    67,    68,    69,   133,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,   126,    90,    91,    92,
     132,    94,    95,    96,   133,   132,   148,   132,   394,   102,
      58,   104,   336,   106,   107,   108,   109,   110,   128,   112,
     113,   114,   115,   116,   439,    69,   492,   439,   436,   122,
     422,   124,   422,   433,   127,    18,   460,   483,   478,   389,
     133,    -1,   414,    -1,    27,    -1,    -1,    -1,    31,    32,
      -1,    90,    91,    92,   454,   454,   454,   454,   454,    -1,
     460,    44,     3,    46,     5,   365,    -1,     8,     9,    10,
      -1,    12,    -1,    -1,    -1,   114,   115,   116,   478,   478,
     478,   478,   478,   122,    -1,   124,    -1,    -1,   127,    -1,
      -1,    -1,   492,   492,   492,   492,   492,     0,    -1,    -1,
       3,     4,     5,     6,    45,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    55,    56,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    40,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,
      43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    67,    68,    69,    -1,
      63,    64,    -1,    -1,    75,    76,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    -1,    94,    95,    96,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,    -1,   112,
      -1,   122,    -1,   124,   117,   118,   127,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,   132,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    -1,    -1,    36,    37,    38,    39,    -1,    41,    42,
      43,    -1,    45,    46,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,   132,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    -1,    -1,    36,    37,    38,    39,    -1,    41,    42,
      43,    -1,    45,    46,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,   132,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    -1,    -1,    36,    37,    38,    39,    -1,    41,    42,
      43,    -1,    45,    46,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,   132,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    40,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,
      43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    75,    76,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    -1,    94,    95,    96,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,    -1,   112,
      -1,   122,    -1,   124,   117,   118,   127,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,   132,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,
      43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,   132,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,
      43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,   132,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,
      43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,   132,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,
      43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,    -1,     0,    -1,   132,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,
      43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,   128,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    -1,    -1,    36,
      37,    38,    39,    -1,    41,    42,    43,    -1,    45,    46,
      47,    48,    49,    -1,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,    -1,   126,
      -1,   128,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    -1,    -1,    36,    37,    38,    39,    -1,
      41,    42,    43,    -1,    45,    46,    47,    48,    49,    -1,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
       3,    -1,     5,     6,    -1,     8,     9,    10,    11,    12,
      -1,   112,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
      -1,   122,    25,   124,    27,   126,    29,   128,    -1,    32,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
       3,    -1,     5,     6,    -1,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    25,    -1,    27,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,   112,
      43,    -1,    45,    -1,   117,   118,    -1,    -1,    -1,   122,
      53,   124,    55,    56,    -1,   128,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
       3,    -1,     5,     6,    -1,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    25,    -1,    27,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    45,    -1,   117,   118,    -1,    -1,    -1,   122,
      53,   124,    55,    56,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,   122,
      -1,   124,    -1,    -1,   102,   128,   104,    -1,   106,   107,
     108,   109,   110,    40,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      -1,    -1,    -1,    -1,    -1,   102,    40,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,
     127,    -1,    -1,    67,    68,    69,   133,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    -1,    -1,    -1,    -1,    40,   102,    -1,
     104,   105,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,    -1,   127,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    -1,    -1,    -1,    -1,    40,   102,
      -1,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,   127,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    -1,    -1,    -1,    -1,    40,
     102,    -1,   104,   105,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,    -1,    -1,   127,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    -1,    -1,    -1,    -1,
      40,   102,    -1,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,    -1,   127,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,    -1,   127,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    -1,    94,    95,    96,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,    -1,    -1,   127,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    -1,    94,    95,    96,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,   127,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    -1,    94,    95,    96,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,
     127,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    -1,    94,    95,
      96,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,   127,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    -1,    94,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,    -1,   127
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    29,    36,    38,    39,    41,    43,    45,
      47,    48,    49,    52,    53,    55,    56,    63,    64,    70,
      90,    91,    99,   112,   117,   118,   122,   124,   126,   128,
     135,   136,   137,   138,   144,   147,   148,   154,   158,   159,
     160,   161,   163,   171,   179,   182,   185,   188,   189,   190,
     191,   195,   197,   206,   214,   217,   230,   231,   232,   235,
     236,   237,   238,   241,   243,   244,    89,    50,   122,   122,
     164,   122,     3,    19,   126,   183,   184,   184,   220,     3,
     237,     3,   216,   237,   122,   184,   220,   122,   126,   122,
     127,   237,   122,   234,   237,   239,   240,   237,   237,   237,
     237,   237,   235,   226,   227,   228,   234,    30,    32,    37,
      42,    46,    66,    99,   132,   143,   144,   145,   148,   150,
     165,   166,   181,   189,   233,     3,     0,   138,   126,     3,
      46,   150,   156,   157,   236,    18,    27,    31,    32,    44,
      46,   205,   242,   191,   100,    40,    67,    68,    69,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    90,    91,    92,
      94,    95,    96,   102,   104,   106,   107,   108,   109,   110,
     112,   113,   114,   115,   116,   122,   124,   127,     3,   122,
     162,   145,   146,   172,   187,   126,   127,   127,   229,   122,
     235,   127,   237,   180,   226,     3,   155,   105,   132,   100,
     133,   125,   100,   234,     3,     3,   151,   152,   235,   235,
     151,     3,   132,   145,   126,   136,     3,    99,    68,     3,
      35,    51,   213,     3,     3,     3,   102,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
       3,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   226,   237,     3,   122,
     128,   235,    99,    14,   237,    99,   139,   140,   141,   147,
     148,   195,   197,   206,   214,   186,   183,     3,   122,   225,
       3,    93,   210,   211,   212,   133,   115,   133,   142,   143,
     133,   235,   234,   234,   105,   224,   100,   143,   132,   224,
     235,   235,     3,    54,   192,    54,   193,   207,     3,     5,
     105,   133,   125,    89,   237,     3,   133,   122,   133,   132,
     141,   126,   139,   226,   102,   105,   208,   212,   133,   100,
     146,   132,   133,   105,     3,    36,   115,   219,   220,   221,
     223,   102,   149,   152,   132,    99,   133,   122,   221,    17,
     194,   221,   222,   126,   224,   237,     3,   133,   146,   235,
     126,   139,   132,   133,     3,     5,     8,     9,    10,    12,
      45,    55,    56,   209,   223,   224,    93,   212,    28,    33,
     173,   175,   177,   178,   146,   234,   234,   236,   146,   210,
     222,   126,   100,   198,   149,   133,    34,    62,   167,   168,
     169,   170,   132,   208,   126,   212,   126,   122,   173,   175,
      65,   153,   133,   133,   196,   221,    46,    99,   189,   202,
     203,   204,   237,   105,   132,   169,   170,   218,   176,     3,
     146,   146,   224,    99,   144,   148,   189,   199,   200,   201,
     206,   214,     3,    27,   132,   204,   105,   142,   142,   142,
     224,   126,   126,   132,   201,   213,   142,   132,   132,   133,
     215,   199,     3,   174,   142,   132,   122,   126,   132,   210,
     142,   133,   132,   224
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
        
    case 26:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1920 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1921 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1923 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 29:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1924 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 42:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1939 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 43:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1940 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 44:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1943 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 51:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1950 "parser.y"
    {(yyval.code)=(yyvsp[(3) - (4)].code);}
    }
    break;


  
    case 52:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1954 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1955 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1959 "parser.y"
    {
    code_t**cc = &global->init->method->body->code;
    *cc = code_append(*cc, (yyvsp[(1) - (1)].code));
}
    }
    break;


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1970 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (2)].value);}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1971 "parser.y"
    {(yyval.value).c=abc_pushundefined(0);
                                  (yyval.value).t=TYPE_ANY;
                                 }
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1975 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1976 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1978 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1979 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1982 "parser.y"
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
#line 2042 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2043 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2046 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2046 "parser.y"
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
#line 2065 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2072 "parser.y"
    {
    PASS1 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),0,1,0);
    PASS2 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
}
    }
    break;


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2076 "parser.y"
    {
    PASS12
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 72:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2081 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 73:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2082 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2084 "parser.y"
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
#line 2104 "parser.y"
    {
    variable_t*var = find_variable(state, (yyvsp[(2) - (6)].id));
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
#line 2148 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2148 "parser.y"
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
#line 2167 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2167 "parser.y"
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
#line 2182 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2185 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2188 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2191 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2195 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2196 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2197 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2198 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2199 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2200 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2202 "parser.y"
    {
    (yyval.code) = abc_dup(0);
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
#line 2213 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2216 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2216 "parser.y"
    {
    (yyval.code)=(yyvsp[(4) - (8)].value).c;
    (yyval.code) = code_append((yyval.code), (yyvsp[(7) - (8)].code));
    code_t*out = (yyval.code) = abc_pop((yyval.code));
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
#line 2245 "parser.y"
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
#line 2250 "parser.y"
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
#line 2269 "parser.y"
    {PASS12 new_state();state->exception_name=0;}
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2269 "parser.y"
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
#line 2285 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2286 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2287 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2288 "parser.y"
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
#line 2296 "parser.y"
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
#line 2305 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 104:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2305 "parser.y"
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
#line 2350 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2354 "parser.y"
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
#line 2365 "parser.y"
    {
     (yyval.code) = (yyvsp[(3) - (5)].value).c;
     (yyval.code) = abc_pushscope((yyval.code));
     (yyval.code) = code_append((yyval.code), (yyvsp[(5) - (5)].code));
     (yyval.code) = abc_popscope((yyval.code));
}
    }
    break;


  
    case 109:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2375 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 110:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2377 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 111:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2378 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2380 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2381 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2382 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2383 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2385 "parser.y"
    {
       PASS12
       slotinfo_t*s = registry_find((yyvsp[(2) - (2)].classinfo)->package, (yyvsp[(2) - (2)].classinfo)->name);
       if(!s) {// || !(s->flags&FLAG_BUILTIN)) {
           as3_schedule_class((yyvsp[(2) - (2)].classinfo)->package, (yyvsp[(2) - (2)].classinfo)->name);
       }

       PASS2
       classinfo_t*c = (yyvsp[(2) - (2)].classinfo);
       if(!c) 
            syntaxerror("Couldn't import class\n");
       state_has_imports();
       dict_put(state->imports, c->name, c);
       (yyval.code)=0;
}
    }
    break;


  
    case 117:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2400 "parser.y"
    {
       PASS12
       if(strncmp("flash.", (yyvsp[(2) - (4)].id), 6)) {
           as3_schedule_package((yyvsp[(2) - (4)].id));
       }

       PASS2
       NEW(import_t,i);
       i->package = (yyvsp[(2) - (4)].id);
       state_has_imports();
       list_append(state->wildcard_imports, i);
       (yyval.code)=0;
}
    }
    break;


  
    case 118:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2416 "parser.y"
    {PASS12 (yyval.flags).flags=0;(yyval.flags).ns=0;}
    }
    break;


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2417 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2418 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2419 "parser.y"
    {
    PASS12 
    (yyval.flags).flags=(yyvsp[(1) - (2)].flags).flags|(yyvsp[(2) - (2)].flags).flags;
    if((yyvsp[(1) - (2)].flags).ns && (yyvsp[(2) - (2)].flags).ns) syntaxerror("only one namespace allowed in one declaration");
    (yyval.flags).ns=(yyvsp[(1) - (2)].flags).ns?(yyvsp[(1) - (2)].flags).ns:(yyvsp[(2) - (2)].flags).ns;

}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2427 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PUBLIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2428 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PRIVATE;(yyval.flags).ns=0;}
    }
    break;


  
    case 124:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2429 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PROTECTED;(yyval.flags).ns=0;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2430 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_STATIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2431 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_DYNAMIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2432 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_FINAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2433 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_OVERRIDE;(yyval.flags).ns=0;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2434 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NATIVE;(yyval.flags).ns=0;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2435 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PACKAGEINTERNAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2436 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NAMESPACE;
                               (yyval.flags).ns=(yyvsp[(1) - (1)].id);
                       }
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2440 "parser.y"
    {(yyval.classinfo)=registry_getobjectclass();}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2441 "parser.y"
    {(yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2443 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2444 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2446 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2447 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2451 "parser.y"
    {PASS12 startclass(&(yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2453 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2457 "parser.y"
    {PASS12 (yyvsp[(1) - (5)].flags).flags|=FLAG_INTERFACE;
                                          startclass(&(yyvsp[(1) - (5)].flags),(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2460 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 150:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2473 "parser.y"
    {
    code_t*c = state->cls->static_init->header;
    c = code_append(c, (yyvsp[(1) - (1)].code));  
    state->cls->static_init->header = c;
}
    }
    break;


  
    case 156:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2484 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 157:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2487 "parser.y"
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


  
    case 160:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2502 "parser.y"
    {setstaticfunction((yyvsp[(1) - (3)].flags).flags);}
    }
    break;


  
    case 161:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2502 "parser.y"
    {
    int flags = (yyvsp[(1) - (6)].flags).flags;
    namespace_t ns = modifiers2access(&(yyvsp[(1) - (6)].flags));

    varinfo_t* info = 0;
    if(state->cls) {
        memberinfo_t*i = registry_findmember(state->cls->info, ns.name, (yyvsp[(3) - (6)].id), 1);
        if(i) {
            check_override(i, flags);
        }
        info = varinfo_register_onclass(state->cls->info, ns.access, ns.name, (yyvsp[(3) - (6)].id));
    } else {
        slotinfo_t*i = registry_find(state->package, (yyvsp[(3) - (6)].id));
        if(i) {
            syntaxerror("package %s already contains '%s'", state->package, (yyvsp[(3) - (6)].id));
        }
        if(ns.name && ns.name[0]) {
            syntaxerror("namespaces not allowed on package-level variables");
        }
        info = varinfo_register_global(ns.access, state->package, (yyvsp[(3) - (6)].id));
    }

    info->type = (yyvsp[(5) - (6)].classinfo);
    info->flags = flags;

    /* slot name */
    multiname_t mname = {QNAME, &ns, 0, (yyvsp[(3) - (6)].id)};
  
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
    if((yyvsp[(5) - (6)].classinfo)) {
        MULTINAME(m, (yyvsp[(5) - (6)].classinfo));
        t = trait_new_member(traits, multiname_clone(&m), multiname_clone(&mname), 0);
    } else {
        t = trait_new_member(traits, 0, multiname_clone(&mname), 0);
    }
    info->slot = t->slot_id;
    
    /* initalization code (if needed) */
    code_t*c = 0;
    if((yyvsp[(6) - (6)].value).c && !is_pushundefined((yyvsp[(6) - (6)].value).c)) {
        c = abc_getlocal_0(c);
        c = code_append(c, (yyvsp[(6) - (6)].value).c);
        c = converttype(c, (yyvsp[(6) - (6)].value).t, (yyvsp[(5) - (6)].classinfo));
        c = abc_setslot(c, t->slot_id);
    }

    *code = code_append(*code, c);

    if((yyvsp[(2) - (6)].token)==KW_CONST) {
        t->kind= TRAIT_CONST;
    }

    (yyval.code)=0;
    setstaticfunction(0);
}
    }
    break;


  
    case 162:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2577 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 163:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2578 "parser.y"
    {(yyval.constant)=(yyvsp[(2) - (2)].constant);}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2580 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_uint));}
    }
    break;


  
    case 165:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2581 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 166:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2582 "parser.y"
    {(yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));}
    }
    break;


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2583 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2584 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2586 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2587 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2588 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2589 "parser.y"
    {
    // TODO
    as3_warning("Couldn't resolve %s", (yyvsp[(1) - (1)].id));
    (yyval.constant) = constant_new_null((yyvsp[(1) - (1)].id));
}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2598 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2602 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2608 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(2) - (2)].param));
}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2614 "parser.y"
    {
    PASS12
    (yyval.params) =(yyvsp[(1) - (4)].params);
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(4) - (4)].param));
}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2622 "parser.y"
    {
    PASS12
    (yyval.params) = (yyvsp[(1) - (3)].params);
    list_append((yyval.params).list, (yyvsp[(3) - (3)].param));
}
    }
    break;


  
    case 178:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2627 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    list_append((yyval.params).list, (yyvsp[(1) - (1)].param));
}
    }
    break;


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2633 "parser.y"
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


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2641 "parser.y"
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


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2651 "parser.y"
    {PASS12 (yyval.token)=0;}
    }
    break;


  
    case 184:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2654 "parser.y"
    {PASS12 startfunction(&(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2655 "parser.y"
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


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2671 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2673 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2674 "parser.y"
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


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2696 "parser.y"
    {
    PASS1 static slotinfo_t c;
          memset(&c, 0, sizeof(c));
          c.name = (yyvsp[(1) - (1)].id);
          (yyval.classinfo) = (classinfo_t*)&c;
    PASS2
    slotinfo_t*s = find_class((yyvsp[(1) - (1)].id));
    if(!s) syntaxerror("Could not find class/method %s (current package: %s)\n", (yyvsp[(1) - (1)].id), state->package);
    (yyval.classinfo) = (classinfo_t*)s;
}
    }
    break;


  
    case 191:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2707 "parser.y"
    {
    PASS1 static slotinfo_t c;
          memset(&c, 0, sizeof(c));
          c.package = (yyvsp[(1) - (3)].id);
          c.name = (yyvsp[(3) - (3)].id);
          (yyval.classinfo)=(classinfo_t*)&c;
    PASS2
    slotinfo_t*s = registry_find((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id));
    if(!s) syntaxerror("Couldn't find class/method %s.%s\n", (yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id));
    free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;
    (yyval.classinfo) = (classinfo_t*)s;
}
    }
    break;


  
    case 194:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2723 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2724 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2726 "parser.y"
    {(yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2727 "parser.y"
    {(yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2728 "parser.y"
    {(yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2737 "parser.y"
    {(yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2738 "parser.y"
    {(yyval.classinfo)=0;}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2742 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2743 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 203:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2745 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2749 "parser.y"
    {(yyval.value_list).len=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2753 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 208:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2754 "parser.y"
    {
                                                  (yyval.value_list).len= (yyvsp[(1) - (2)].value_list).len+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2760 "parser.y"
    {
    (yyval.value).c = (yyvsp[(2) - (4)].value).c;
    if((yyval.value).c->opcode == OPCODE_COERCE_A) (yyval.value).c = code_cutlast((yyval.value).c);
    
    code_t*paramcode = (yyvsp[(4) - (4)].value_list).cc;
    if((yyval.value).c->opcode == OPCODE_GETPROPERTY) {
        multiname_t*name = (yyval.value).c->data[0];(yyval.value).c->data[0]=0;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_constructprop2((yyval.value).c, name, (yyvsp[(4) - (4)].value_list).len);
        multiname_destroy(name);
    } else if((yyval.value).c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)(yyval.value).c->data[0];
        trait_t*t = traits_find_slotid(state->cls->abc->traits,slot);//FIXME
        multiname_t*name = t->name;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_constructprop2((yyval.value).c, name, (yyvsp[(4) - (4)].value_list).len);
    } else {
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_construct((yyval.value).c, (yyvsp[(4) - (4)].value_list).len);
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


  
    case 211:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2796 "parser.y"
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
        (yyval.value).c = abc_callproperty2((yyval.value).c, name, (yyvsp[(3) - (4)].value_list).len);
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
        (yyval.value).c = abc_callproperty2((yyval.value).c, name, (yyvsp[(3) - (4)].value_list).len);
    } else if((yyval.value).c->opcode == OPCODE_GETSUPER) {
        multiname_t*name = (yyval.value).c->data[0];(yyval.value).c->data[0]=0;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_callsuper2((yyval.value).c, name, (yyvsp[(3) - (4)].value_list).len);
        multiname_destroy(name);
    } else {
        (yyval.value).c = abc_getglobalscope((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_call((yyval.value).c, (yyvsp[(3) - (4)].value_list).len);
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


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2842 "parser.y"
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

    (yyval.value).c = abc_constructsuper((yyval.value).c, (yyvsp[(3) - (4)].value_list).len);
    (yyval.value).c = abc_pushundefined((yyval.value).c);
    (yyval.value).t = TYPE_ANY;
}
    }
    break;


  
    case 213:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2863 "parser.y"
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


  
    case 214:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2884 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 215:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2887 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 216:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2894 "parser.y"
    {(yyval.value)=(yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 217:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2895 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 218:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2896 "parser.y"
    {
    (yyval.value).c = (yyvsp[(1) - (3)].value).c;
    (yyval.value).c = cut_last_push((yyval.value).c);
    (yyval.value).c = code_append((yyval.value).c,(yyvsp[(3) - (3)].value).c);
    (yyval.value).t = (yyvsp[(3) - (3)].value).t;
}
    }
    break;


  
    case 219:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2902 "parser.y"
    {
    (yyval.code)=cut_last_push((yyvsp[(1) - (1)].value).c);
}
    }
    break;


  
    case 220:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2908 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2912 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2914 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 224:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2916 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2920 "parser.y"
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


  
    case 227:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2937 "parser.y"
    {(yyval.value).c = abc_pushbyte(0, (yyvsp[(1) - (1)].number_uint));
                   //MULTINAME(m, registry_getintclass());
                   //$$.c = abc_coerce2($$.c, &m); // FIXME
                   (yyval.value).t = TYPE_INT;
                  }
    }
    break;


  
    case 228:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2942 "parser.y"
    {(yyval.value).c = abc_pushshort(0, (yyvsp[(1) - (1)].number_uint));
                    (yyval.value).t = TYPE_INT;
                   }
    }
    break;


  
    case 229:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2945 "parser.y"
    {(yyval.value).c = abc_pushint(0, (yyvsp[(1) - (1)].number_int));
                  (yyval.value).t = TYPE_INT;
                 }
    }
    break;


  
    case 230:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2948 "parser.y"
    {(yyval.value).c = abc_pushuint(0, (yyvsp[(1) - (1)].number_uint));
                   (yyval.value).t = TYPE_UINT;
                  }
    }
    break;


  
    case 231:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2951 "parser.y"
    {(yyval.value).c = abc_pushdouble(0, (yyvsp[(1) - (1)].number_float));
                    (yyval.value).t = TYPE_FLOAT;
                   }
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2954 "parser.y"
    {(yyval.value).c = abc_pushstring2(0, &(yyvsp[(1) - (1)].str));free((char*)(yyvsp[(1) - (1)].str).str);
                     (yyval.value).t = TYPE_STRING;
                    }
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2957 "parser.y"
    {(yyval.value).c = abc_pushundefined(0);
                    (yyval.value).t = TYPE_ANY;
                   }
    }
    break;


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2960 "parser.y"
    {(yyval.value).c = abc_pushtrue(0);
                    (yyval.value).t = TYPE_BOOLEAN;
                   }
    }
    break;


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2963 "parser.y"
    {(yyval.value).c = abc_pushfalse(0);
                     (yyval.value).t = TYPE_BOOLEAN;
                    }
    }
    break;


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2966 "parser.y"
    {(yyval.value).c = abc_pushnull(0);
                    (yyval.value).t = TYPE_NULL;
                   }
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2970 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2973 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2976 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2979 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2982 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 242:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2985 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
              }
    }
    break;


  
    case 243:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2988 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2991 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2995 "parser.y"
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


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3006 "parser.y"
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


  
    case 247:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3024 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3029 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_bitnot((yyval.value).c);
              (yyval.value).t = TYPE_INT;
             }
    }
    break;


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3034 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitand((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3039 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitxor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3044 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3049 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_rshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3053 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_urshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3057 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_lshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3062 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_divide((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3066 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_modulo((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3070 "parser.y"
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


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3079 "parser.y"
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


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3088 "parser.y"
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


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3098 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_in((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3103 "parser.y"
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


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3116 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_instanceof((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3121 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_istypelate((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3126 "parser.y"
    {
              (yyval.value).c = (yyvsp[(3) - (4)].value).c;
              (yyval.value).c = abc_typeof((yyval.value).c);
              (yyval.value).t = TYPE_STRING;
             }
    }
    break;


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3132 "parser.y"
    {
              (yyval.value).c = cut_last_push((yyvsp[(2) - (2)].value).c);
              (yyval.value).c = abc_pushundefined((yyval.value).c);
              (yyval.value).t = TYPE_ANY;
             }
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3138 "parser.y"
    { (yyval.value).c = abc_pushundefined(0);
             (yyval.value).t = TYPE_ANY;
           }
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3142 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (3)].value);}
    }
    break;


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3144 "parser.y"
    {
  (yyval.value)=(yyvsp[(2) - (2)].value);
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


  
    case 269:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3155 "parser.y"
    {
  (yyval.value).c = (yyvsp[(1) - (4)].value).c;
  (yyval.value).c = code_append((yyval.value).c, (yyvsp[(3) - (4)].value).c);

  MULTINAME_LATE(m, (yyvsp[(1) - (4)].value).t?(yyvsp[(1) - (4)].value).t->access:ACCESS_PACKAGE, "");
  (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
  (yyval.value).t = 0; // array elements have unknown type
}
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3164 "parser.y"
    {
    (yyval.value).c = code_new();
    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(2) - (3)].value_list).cc);
    (yyval.value).c = abc_newarray((yyval.value).c, (yyvsp[(2) - (3)].value_list).len);
    (yyval.value).t = registry_getarrayclass();
}
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3171 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3172 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3174 "parser.y"
    {
    (yyval.value_list).cc = 0;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(1) - (3)].value).c);
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (3)].value).c);
    (yyval.value_list).len = 2;
}
    }
    break;


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3180 "parser.y"
    {
    (yyval.value_list).cc = (yyvsp[(1) - (5)].value_list).cc;
    (yyval.value_list).len = (yyvsp[(1) - (5)].value_list).len+2;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (5)].value).c);
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(5) - (5)].value).c);
}
    }
    break;


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3189 "parser.y"
    {
    (yyval.value).c = code_new();
    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(2) - (3)].value_list).cc);
    (yyval.value).c = abc_newobject((yyval.value).c, (yyvsp[(2) - (3)].value_list).len/2);
    (yyval.value).t = registry_getobjectclass();
}
    }
    break;


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3196 "parser.y"
    { 
               code_t*c = (yyvsp[(3) - (3)].value).c;
               if(BOTH_INT((yyvsp[(1) - (3)].value).t,(yyvsp[(3) - (3)].value).t)) {
                c=abc_multiply_i(c);
               } else {
                c=abc_multiply(c);
               }
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '*'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3208 "parser.y"
    { 
               code_t*c = abc_modulo((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '%'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 278:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3214 "parser.y"
    { 
               code_t*c = abc_lshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '<'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 279:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3220 "parser.y"
    { 
               code_t*c = abc_rshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '>'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 280:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3226 "parser.y"
    { 
               code_t*c = abc_urshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, 'U'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 281:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3232 "parser.y"
    { 
               code_t*c = abc_divide((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '/'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3238 "parser.y"
    { 
               code_t*c = abc_bitor((yyvsp[(3) - (3)].value).c);
               c=converttype(c, TYPE_INT, (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3244 "parser.y"
    { 
               code_t*c = abc_bitxor((yyvsp[(3) - (3)].value).c);
               c=converttype(c, TYPE_INT, (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3250 "parser.y"
    { 
               code_t*c = (yyvsp[(3) - (3)].value).c;

               if(TYPE_IS_INT((yyvsp[(1) - (3)].value).t)) {
                c=abc_add_i(c);
               } else {
                c=abc_add(c);
                c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '+'), (yyvsp[(1) - (3)].value).t);
               }
               
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3263 "parser.y"
    { code_t*c = (yyvsp[(3) - (3)].value).c; 
               if(TYPE_IS_INT((yyvsp[(1) - (3)].value).t)) {
                c=abc_subtract_i(c);
               } else {
                c=abc_subtract(c);
                c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '-'), (yyvsp[(1) - (3)].value).t);
               }
               
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
             }
    }
    break;


  
    case 286:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3274 "parser.y"
    { code_t*c = 0;
              c = code_append(c, (yyvsp[(3) - (3)].value).c);
              c = converttype(c, (yyvsp[(3) - (3)].value).t, (yyvsp[(1) - (3)].value).t);
              (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 1, 0);
              (yyval.value).t = (yyvsp[(1) - (3)].value).t;
            }
    }
    break;


  
    case 287:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3281 "parser.y"
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


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3294 "parser.y"
    { code_t*c = 0;
             classinfo_t*type = (yyvsp[(1) - (2)].value).t;
             if((is_getlocal((yyvsp[(1) - (2)].value).c) && TYPE_IS_INT((yyvsp[(1) - (2)].value).t)) || TYPE_IS_NUMBER((yyvsp[(1) - (2)].value).t)) {
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
                 (yyval.value).c = toreadwrite((yyvsp[(1) - (2)].value).c, c, 0, 1);
                 (yyval.value).t = (yyvsp[(1) - (2)].value).t;
             }
           }
    }
    break;


  
    case 289:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3321 "parser.y"
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
             (yyval.value).c = toreadwrite((yyvsp[(1) - (2)].value).c, c, 0, 1);
             (yyval.value).t = (yyvsp[(1) - (2)].value).t;
            }
    }
    break;


  
    case 290:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3335 "parser.y"
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
             (yyval.value).c = toreadwrite((yyvsp[(2) - (2)].value).c, c, 0, 0);
             (yyval.value).t = (yyvsp[(2) - (2)].value).t;
           }
    }
    break;


  
    case 291:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3349 "parser.y"
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
             (yyval.value).c = toreadwrite((yyvsp[(2) - (2)].value).c, c, 0, 0);
             (yyval.value).t = (yyvsp[(2) - (2)].value).t;
           }
    }
    break;


  
    case 292:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3364 "parser.y"
    { if(!state->cls->info)
                  syntaxerror("super keyword not allowed outside a class");
              classinfo_t*t = state->cls->info->superclass;
              if(!t) t = TYPE_OBJECT;

              memberinfo_t*f = registry_findmember_nsset(t, state->active_namespaces, (yyvsp[(3) - (3)].id), 1);

              MEMBER_MULTINAME(m, f, (yyvsp[(3) - (3)].id));
              (yyval.value).c = 0;
              (yyval.value).c = abc_getlocal_0((yyval.value).c);
              (yyval.value).c = abc_getsuper2((yyval.value).c, &m);
              (yyval.value).t = slotinfo_gettype((slotinfo_t*)f);
           }
    }
    break;


  
    case 293:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3378 "parser.y"
    {
              // attribute TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored @ operator");
           }
    }
    break;


  
    case 294:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3385 "parser.y"
    {
              // child attribute  TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored .@ operator");
           }
    }
    break;


  
    case 295:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3392 "parser.y"
    {
              // namespace declaration TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored :: operator");
           }
    }
    break;


  
    case 296:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3399 "parser.y"
    {
              // descendants TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored .. operator");
           }
    }
    break;


  
    case 297:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3406 "parser.y"
    {
              // filter TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored .() operator");
           }
    }
    break;


  
    case 298:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3418 "parser.y"
    {(yyval.value).c = (yyvsp[(1) - (3)].value).c;
             classinfo_t*t = (yyvsp[(1) - (3)].value).t;
             char is_static = 0;
             if(TYPE_IS_CLASS(t) && t->data) {
                 t = t->data;
                 is_static = 1;
             }
             if(t) {
                 memberinfo_t*f = registry_findmember_nsset(t, state->active_namespaces, (yyvsp[(3) - (3)].id), 1);
                 char noslot = 0;
                 if(f && !is_static != !(f->flags&FLAG_STATIC))
                    noslot=1;
                 if(f && f->slot && !noslot) {
                     (yyval.value).c = abc_getslot((yyval.value).c, f->slot);
                 } else {
                     MEMBER_MULTINAME(m, f, (yyvsp[(3) - (3)].id));
                     (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
                 }
                 /* determine type */
                 (yyval.value).t = slotinfo_gettype((slotinfo_t*)f);
                 if(!(yyval.value).t)
                    (yyval.value).c = abc_coerce_a((yyval.value).c);
             } else {
                 /* when resolving a property on an unknown type, we do know the
                    name of the property (and don't seem to need the package), but
                    we need to make avm2 try out all access modes */
                 multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
                 (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
                 (yyval.value).c = abc_coerce_a((yyval.value).c);
                 (yyval.value).t = registry_getanytype();
             }
            }
    }
    break;


  
    case 299:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3451 "parser.y"
    {
    PASS1
    /* Queue unresolved identifiers for checking against the parent
       function's variables.
       We consider everything which is not a local variable "unresolved".
       This encompasses class names, members of the surrounding class
       etc. which *correct* because local variables of the parent function
       would shadow those.
       */
    if(state->method->inner && !find_variable(state, (yyvsp[(1) - (1)].id))) {
        unknown_variable((yyvsp[(1) - (1)].id));
    }
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
    if(state->cls && (f = registry_findmember_nsset(state->cls->info, state->active_namespaces, (yyvsp[(1) - (1)].id), 1)) &&
        (f->flags&FLAG_STATIC) >= i_am_static) {
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
            namespace_t ns = {f->access, ""};
            multiname_t m = {QNAME, &ns, 0, (yyvsp[(1) - (1)].id)};
            (yyval.value).c = abc_findpropstrict2((yyval.value).c, &m);
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
            break;
        } else if(f->slot>0) {
            (yyval.value).c = abc_getlocal_0((yyval.value).c);
            (yyval.value).c = abc_getslot((yyval.value).c, f->slot);
            break;
        } else {
            namespace_t ns = {f->access, ""};
            multiname_t m = {QNAME, &ns, 0, (yyvsp[(1) - (1)].id)};
            (yyval.value).c = abc_getlocal_0((yyval.value).c);
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
            break;
        }
    } 
    
    /* look at actual classes, in the current package and imported */
    if((a = find_class((yyvsp[(1) - (1)].id)))) {
        if(a->access == ACCESS_PACKAGEINTERNAL &&
           strcmp(a->package, state->package) &&
           strcmp(a->package, internal_filename_package)
           )
           syntaxerror("Can't access internal %s %s in package '%s' from package '%s'",
                infotypename(a),(yyvsp[(1) - (1)].id), a->package, state->package);

        if(a->kind != INFOTYPE_CLASS) {
            MULTINAME(m, a);
            (yyval.value).c = abc_findpropstrict2((yyval.value).c, &m);
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
            if(a->kind == INFOTYPE_METHOD) {
                methodinfo_t*f = (methodinfo_t*)a;
                (yyval.value).t = TYPE_FUNCTION(f);
            } else {
                varinfo_t*v = (varinfo_t*)a;
                (yyval.value).t = v->type;
            }
        } else {
            classinfo_t*c = (classinfo_t*)a;
            if(c->slot) {
                (yyval.value).c = abc_getglobalscope((yyval.value).c);
                (yyval.value).c = abc_getslot((yyval.value).c, c->slot);
            } else {
                MULTINAME(m, c);
                (yyval.value).c = abc_getlex2((yyval.value).c, &m);
            }
            (yyval.value).t = TYPE_CLASS(c);
        }
        break;
    }

    /* unknown object, let the avm2 resolve it */
    if(1) {
        as3_softwarning("Couldn't resolve '%s', doing late binding", (yyvsp[(1) - (1)].id));
        state->method->late_binding = 1;
                
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(1) - (1)].id)};

        (yyval.value).t = 0;
        (yyval.value).c = abc_findpropstrict2((yyval.value).c, &m);
        (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
    }
}
    }
    break;


  
    case 300:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3572 "parser.y"
    {
    PASS12
    tokenizer_register_namespace((yyvsp[(2) - (2)].id));
    (yyval.id)=(yyvsp[(2) - (2)].id);
}
    }
    break;


  
    case 301:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3578 "parser.y"
    {
    (yyval.code)=0;
}
    }
    break;


  
    case 302:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3581 "parser.y"
    {
    (yyval.code)=0;
}
    }
    break;


  
    case 303:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3584 "parser.y"
    {
    (yyval.code)=0;
}
    }
    break;


  
    case 304:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3587 "parser.y"
    {
    PASS12
    tokenizer_register_namespace((yyvsp[(2) - (2)].classinfo)->name);
    (yyval.code)=0;
}
    }
    break;




/* Line 1464 of skeleton.m4  */
#line 7905 "parser.tab.c"
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



