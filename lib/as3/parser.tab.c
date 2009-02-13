
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
    int flags;

    classinfo_t*classinfo;
    classinfo_list_t*classinfo_list;

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
    struct {
        abc_exception_list_t *l;
        code_t*finally;
    } catch_list;



/* Line 223 of skeleton.m4  */
#line 275 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 259 "parser.y"


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

    char inner;
    abc_method_t*abc;
    int var_index; // for inner methods

    code_t*header;
    abc_exception_list_t*exceptions;
    
    methodstate_list_t*innerfunctions;
};

typedef struct _state {
    struct _state*old;
    int level;
    
    char*package;     
    import_list_t*wildcard_imports;
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
        m##_ns.access = (f)->access; \
        m##_ns.name = ""; \
        m.type = QNAME; \
        m.ns = &m##_ns; \
        m.namespace_set = 0; \
        m.name = f->name; \
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
static void function_initvars(methodstate_t*m, params_t*params, int flags);


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
        function_initvars(state->method, 0, 0);
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
} variable_t;

static variable_t* find_variable(char*name)
{
    state_t* s = state;
    while(s) {
        variable_t*v = 0;
        if(s->method)
            v = dict_lookup(s->vars, name);
        if(v) {
            return v;
        }
        if(s->new_vars)
            break;
        s = s->old;
    }
    return 0;
} 
static variable_t* find_variable_safe(char*name)
{
    variable_t* v = find_variable(name);
    if(!v)
        syntaxerror("undefined variable: %s", name);
    return v;
}
static char variable_exists(char*name) 
{
    return dict_lookup(state->vars, name)!=0;
}
code_t*defaultvalue(code_t*c, classinfo_t*type);
static int new_variable(const char*name, classinfo_t*type, char init)
{
    NEW(variable_t, v);
    v->index = state->method->variable_count;
    v->type = type;
    v->init = init;
    
    dict_put(state->vars, name, v);

    return state->method->variable_count++;
}
#define TEMPVARNAME "__as3_temp__"
static int gettempvar()
{
    variable_t*v = find_variable(TEMPVARNAME);
    if(v) 
        return v->index;
    return new_variable(TEMPVARNAME, 0, 0);
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

#define parserassert(b) {if(!(b)) parsererror(__FILE__, __LINE__,__func__);}

static void parsererror(const char*file, int line, const char*f)
{
    syntaxerror("internal error in %s, %s:%d", f, file, line);
}

   
static code_t* method_header(methodstate_t*m)
{
    code_t*c = 0;
    if(m->late_binding && !m->inner) {
        c = abc_getlocal_0(c);
        c = abc_pushscope(c);
    }
    /*if(m->innerfunctions) {
        c = abc_newactivation(c);
        c = abc_pushscope(c);
    }*/
    methodstate_list_t*l = m->innerfunctions;
    while(l) {
        parserassert(l->methodstate->abc);
        c = abc_newfunction(c, l->methodstate->abc);
        c = abc_setlocal(c, l->methodstate->var_index);
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

static int flags2access(int flags)
{
    int access = 0;
    if(flags&FLAG_PUBLIC)  {
        if(access&(FLAG_PRIVATE|FLAG_PROTECTED|FLAG_PACKAGEINTERNAL)) 
            syntaxerror("invalid combination of access levels");
        access = ACCESS_PACKAGE;
    } else if(flags&FLAG_PRIVATE) {
        if(access&(FLAG_PUBLIC|FLAG_PROTECTED|FLAG_PACKAGEINTERNAL)) 
            syntaxerror("invalid combination of access levels");
        access = ACCESS_PRIVATE;
    } else if(flags&FLAG_PROTECTED) {
        if(access&(FLAG_PUBLIC|FLAG_PRIVATE|FLAG_PACKAGEINTERNAL)) 
            syntaxerror("invalid combination of access levels");
        access = ACCESS_PROTECTED;
    } else {
        access = ACCESS_PACKAGEINTERNAL;
    }
    return access;
}

static void function_initvars(methodstate_t*m, params_t*params, int flags)
{
    if(m->inner)
        new_variable("this", 0, 0);
    else if(!m->is_global)
        new_variable((flags&FLAG_STATIC)?"class":"this", state->cls?state->cls->info:0, 0);
    else
        new_variable("globalscope", 0, 0);

    if(params) {
        param_list_t*p=0;
        for(p=params->list;p;p=p->next) {
            new_variable(p->param->name, p->param->type, 0);
        }
    }
    
    methodstate_list_t*l = m->innerfunctions;
    while(l) {
        methodstate_t*m = l->methodstate;
        m->var_index = new_variable(m->info->name, TYPE_FUNCTION(m->info), 0);
        l = l->next;
    }
}


char*as3_globalclass=0;
static void startclass(int flags, char*classname, classinfo_t*extends, classinfo_list_t*implements)
{
    if(state->cls) {
        syntaxerror("inner classes now allowed"); 
    }
    new_state();
    token_list_t*t=0;
    classinfo_list_t*mlist=0;

    if(flags&~(FLAG_PACKAGEINTERNAL|FLAG_PUBLIC|FLAG_FINAL|FLAG_DYNAMIC|FLAG_INTERFACE))
        syntaxerror("invalid modifier(s)");

    if((flags&(FLAG_PUBLIC|FLAG_PACKAGEINTERNAL)) == (FLAG_PUBLIC|FLAG_PACKAGEINTERNAL))
        syntaxerror("public and internal not supported at the same time.");

    /* create the class name, together with the proper attributes */
    int access=0;
    char*package=0;

    if(!(flags&FLAG_PUBLIC) && state->package==internal_filename_package) {
        access = ACCESS_PRIVATE; package = internal_filename_package;
    } else if(!(flags&FLAG_PUBLIC) && state->package!=internal_filename_package) {
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
        state->cls->info->flags |= flags & (FLAG_DYNAMIC|FLAG_INTERFACE|FLAG_FINAL);
    }
    
    if(as3_pass == 2) {
        state->cls = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        
        state->method = state->cls->init;
        parserassert(state->cls && state->cls->info);
        
        function_initvars(state->cls->init, 0, 0);
        function_initvars(state->cls->static_init, 0, 0);

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

        if(!as3_globalclass && (flags&FLAG_PUBLIC) && slotinfo_equals((slotinfo_t*)registry_getMovieClip(),(slotinfo_t*)extends)) {
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
               (c->type == CONSTANT_INT && c->i>0));
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

static methodinfo_t*registerfunction(enum yytokentype getset, int flags, char*name, params_t*params, classinfo_t*return_type, int slot)
{
    methodinfo_t*minfo = 0;
    U8 access = flags2access(flags);
    if(!state->cls) {
        //package method
        minfo = methodinfo_register_global(access, state->package, name);
        minfo->return_type = return_type;
    } else if(getset != KW_GET && getset != KW_SET) {
        //class method
        memberinfo_t* m = registry_findmember(state->cls->info, name, 0);
        if(m) {
            syntaxerror("class already contains a %s '%s'", infotypename((slotinfo_t*)m), m->name);
        }
        minfo = methodinfo_register_onclass(state->cls->info, access, name);
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
        minfo = (methodinfo_t*)registry_findmember(state->cls->info, name, 1);
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
            minfo = methodinfo_register_onclass(state->cls->info, access, name);
            minfo->kind = INFOTYPE_SLOT; //hack
            minfo->subtype = gs;
            minfo->return_type = type;
        }
        /* can't assign a slot as getter and setter might have different slots */
        //minfo->slot = slot;
    }
    if(flags&FLAG_FINAL) minfo->flags |= FLAG_FINAL;
    if(flags&FLAG_STATIC) minfo->flags |= FLAG_STATIC;
    if(flags&FLAG_OVERRIDE) minfo->flags |= FLAG_OVERRIDE;
    return minfo;
}

static void innerfunction(char*name, params_t*params, classinfo_t*return_type)
{
    //parserassert(state->method && state->method->info);

    methodstate_t*parent_method = state->method;

    if(as3_pass==1) {
        // not valid yet
        params = 0;
        return_type = 0;
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
    }

    if(as3_pass == 2) {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        parserassert(state->method);

        state->method->info->return_type = return_type;
        function_initvars(state->method, params, 0);
    }
}

static void startfunction(token_t*ns, int flags, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type)
{
    if(state->method && state->method->info) {
        syntaxerror("not able to start another method scope");
    }
    new_state();
    
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
        state->method->info = registerfunction(getset, flags, name, params, return_type, 0);
        
        dict_put(global->token2info, (void*)(ptroff_t)as3_tokencount, state->method);
    }

    if(as3_pass == 2) {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        parserassert(state->method);

        if(state->cls) {
            memberinfo_t*m = registry_findmember(state->cls->info, name, 2);
            check_override(m, flags);
        }
            
        if(state->cls) { 
            state->cls->has_constructor |= state->method->is_constructor;
        }
        
        state->method->info->return_type = return_type;
        function_initvars(state->method, params, flags);
    } 
}

static abc_method_t* endfunction(token_t*ns, int flags, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type, code_t*body)
{
    if(as3_pass==1) {
        old_state();
        return 0;
    }

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
    syntaxerror("can't convert type %s to %s", from->name, to->name);
    return 0; // make gcc happy
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
#line 1767 "parser.tab.c"

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
#define YYFINAL  145
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2994

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  110
/* YYNRULES -- Number of rules.  */
#define YYNRULES  303
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
      85,    87,    91,    94,    96,    98,   100,   102,   104,   106,
     108,   113,   115,   118,   120,   122,   126,   129,   130,   133,
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
     888,   892,   898,   904
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
      -1,   179,    -1,   236,    -1,   243,    -1,   126,   143,   132,
      -1,   126,   132,    -1,    99,    -1,   144,    -1,   150,    -1,
     165,    -1,   166,    -1,   233,    -1,   181,    -1,   148,   126,
     143,   132,    -1,   242,    -1,   145,    99,    -1,   145,    -1,
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
     133,    -1,   237,   127,     3,    -1,     3,    -1,   189,    18,
       3,    -1,   189,    18,     3,   102,     3,    -1,   189,    18,
       3,   102,     5,    -1,    23,    18,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1742,  1742,  1744,  1744,  1745,  1746,  1748,  1749,  1750,
    1751,  1752,  1753,  1754,  1755,  1757,  1757,  1758,  1759,  1761,
    1762,  1763,  1764,  1765,  1766,  1767,  1769,  1770,  1772,  1773,
    1776,  1777,  1778,  1779,  1780,  1781,  1782,  1783,  1784,  1785,
    1786,  1787,  1788,  1791,  1792,  1793,  1794,  1795,  1796,  1797,
    1798,  1800,  1804,  1805,  1809,  1816,  1820,  1821,  1825,  1826,
    1828,  1829,  1831,  1874,  1875,  1878,  1878,  1897,  1898,  1899,
    1902,  1905,  1909,  1910,  1912,  1932,  1975,  1975,  1994,  1994,
    2009,  2012,  2015,  2018,  2022,  2023,  2024,  2025,  2026,  2027,
    2029,  2040,  2043,  2043,  2072,  2072,  2092,  2092,  2109,  2110,
    2111,  2112,  2120,  2129,  2129,  2174,  2178,  2189,  2198,  2199,
    2201,  2202,  2204,  2204,  2206,  2206,  2209,  2223,  2239,  2240,
    2241,  2242,  2244,  2245,  2246,  2247,  2248,  2249,  2250,  2251,
    2252,  2253,  2255,  2256,  2258,  2259,  2261,  2262,  2266,  2264,
    2272,  2270,  2278,  2279,  2280,  2281,  2282,  2283,  2284,  2285,
    2287,  2293,  2294,  2295,  2296,  2297,  2298,  2301,  2314,  2314,
    2316,  2316,  2389,  2390,  2392,  2393,  2394,  2395,  2396,  2398,
    2399,  2400,  2401,  2410,  2414,  2420,  2426,  2434,  2439,  2445,
    2453,  2461,  2462,  2463,  2466,  2465,  2480,  2481,  2483,  2482,
    2504,  2513,  2526,  2527,  2529,  2530,  2532,  2533,  2534,  2543,
    2544,  2548,  2549,  2551,  2552,  2553,  2555,  2559,  2560,  2565,
    2566,  2602,  2648,  2669,  2690,  2693,  2700,  2701,  2702,  2708,
    2714,  2716,  2718,  2720,  2722,  2724,  2726,  2743,  2748,  2751,
    2754,  2757,  2760,  2763,  2766,  2769,  2772,  2776,  2779,  2782,
    2785,  2788,  2791,  2794,  2797,  2801,  2812,  2830,  2835,  2840,
    2845,  2850,  2855,  2859,  2863,  2868,  2872,  2876,  2885,  2894,
    2904,  2909,  2921,  2927,  2932,  2938,  2944,  2948,  2950,  2961,
    2970,  2977,  2978,  2980,  2986,  2995,  3002,  3014,  3020,  3026,
    3032,  3038,  3044,  3050,  3056,  3069,  3080,  3087,  3100,  3127,
    3141,  3155,  3169,  3184,  3191,  3198,  3205,  3212,  3223,  3257,
    3359,  3360,  3361,  3363
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
     144,   144,   144,   145,   145,   145,   145,   145,   145,   145,
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
     242,   242,   242,   243
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     4,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     1,     4,     1,     1,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     2,     1,     1,     3,     2,     0,     2,     2,
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
       3,     5,     5,     3
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
      39,   217,   221,   222,    40,     0,     0,    72,    76,   118,
      92,   108,   109,   114,   111,     0,     0,   299,   209,   186,
       0,   265,     0,     0,   116,     0,   103,   203,     0,   213,
      65,     0,   216,     0,   272,   291,   290,   268,   248,   247,
       0,     0,   204,   205,   206,    82,     0,   106,   214,     0,
      80,    43,    42,   118,    44,    29,     0,    45,    46,    47,
      49,     0,    48,    51,   293,     1,     6,   118,   299,     0,
      68,     0,     0,    69,   183,     0,   159,     0,   158,     0,
     121,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   289,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   203,     0,     0,    55,    73,     0,    53,     0,
       0,   118,   112,     0,   303,   201,   173,     0,     0,     0,
      27,     0,   292,     0,     0,   275,     0,   267,   270,   207,
     208,    83,   200,    59,    60,   105,   215,    58,    81,    41,
      28,   118,     0,     0,   200,     0,     0,   182,   181,     0,
     132,   134,   160,   218,   262,   263,   260,   261,   241,   242,
     244,   243,   239,   240,   282,   281,   277,   276,   284,   285,
     283,   279,   278,   280,   245,   246,   296,   254,   253,   252,
     286,     0,   251,   250,   249,   237,   238,   258,   257,   255,
     259,   256,     0,     0,   298,     0,     0,     0,    52,     0,
       0,    25,     0,   118,    17,    23,     0,    20,    19,    22,
      21,   118,   110,   203,   210,   162,     0,     0,   174,   178,
     118,   191,   117,   264,     0,    26,   212,     0,   273,     0,
       0,    57,     0,   118,   300,    13,    57,     0,     0,     0,
       0,   136,     0,     0,   200,     0,   211,   269,     0,     0,
     294,   118,     0,     0,   115,    18,   118,     0,     0,     0,
       0,   180,   175,   200,     0,   107,     0,   118,     0,   190,
     198,   197,     0,   193,   192,   196,   199,     0,    62,    61,
      50,     0,     0,   118,   173,   133,     0,     0,   194,   135,
     140,    57,   287,   295,   297,    77,     0,    84,     0,   113,
     202,   172,   168,   165,   166,   164,   167,   171,   170,   169,
     163,   162,     0,     0,   177,     0,     0,    98,   102,   100,
     104,    63,   274,     0,    56,   301,   302,     0,    75,     0,
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
      -1,    50,    51,    52,    53,   302,   303,   304,   324,   325,
     134,   135,   209,   305,   136,   378,   137,   237,   234,   451,
      57,   223,   151,   152,    58,    59,    60,    61,   207,    62,
      89,   138,   139,   438,   439,   440,   441,    63,   210,   417,
     503,   418,   468,   419,   420,    64,   220,   140,    65,    94,
     372,    66,   311,   211,    67,   141,    69,    70,   341,   343,
     387,   307,   454,   308,   433,   477,   478,   479,   459,   460,
     461,   159,   309,   344,   361,   410,   317,   318,   319,   249,
     310,   500,   100,    75,   467,   373,   374,   375,   389,   376,
     331,   314,   121,   122,   123,   215,    76,    77,    78,   142,
     124,    79,    80,    81,    82,   113,   114,    83,   143,    84
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -374
static const yytype_int16 yypact[] =
{
    1667,   -68,  -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,
     -30,   -97,  -374,   -57,    20,  -374,  -374,  -374,    49,  -374,
    2087,  -374,    71,  -374,  2157,  -374,   -37,    85,   -35,  -374,
    -374,  -374,  -374,     6,     8,  -374,  -374,  2087,    14,  2087,
    2087,  2087,  -374,  2087,  2087,  2087,  2087,  2087,   627,    88,
     177,  -374,   497,  -374,  -374,  -374,    81,  -374,  2017,  -374,
    -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,   308,   298,
    -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,    79,
    -374,  2562,  -374,  -374,  -374,   197,    86,  -374,  -374,  1919,
    -374,  -374,  -374,  -374,  -374,   -18,   206,  -374,   -46,  -374,
      93,   133,  2087,    83,  -374,  2087,  -374,  2087,   215,   133,
    -374,   117,  2562,    94,   127,   133,   133,   201,   133,   133,
     -53,   103,   129,  2087,  -374,   227,   228,  2087,  2087,   228,
     232,  -374,  -374,   757,  -374,  -374,   107,  -374,  -374,  -374,
    -374,   219,  -374,  -374,  -374,  -374,  -374,  1017,   170,   236,
    -374,   141,   176,  -374,    70,   242,  -374,   243,  -374,   244,
    -374,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,
    2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,
    2087,  2087,  2087,  2087,  -374,  -374,   245,  2087,  2087,  2087,
    2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,  2087,
    2087,  2087,  2087,  2087,    11,  -374,  -374,  2087,   159,   247,
    2087,  1147,  -374,    85,  -374,   137,    24,   -49,    16,  2182,
    1793,   130,  -374,  2087,  2087,  -374,  2087,  -374,  -374,  -374,
    -374,  -374,   157,   164,  -374,    79,    79,   164,  -374,  -374,
    -374,  1919,   262,   134,   157,  2087,  2087,  -374,  -374,   269,
     226,   231,  -374,  2562,  1009,   489,   489,   489,  2867,  2867,
    2867,  2867,  1009,  1009,  2562,  2562,  2562,  2562,  2562,  2562,
    2562,  2562,  2562,  2562,   304,  2623,  -374,   349,   349,   349,
    2562,  2379,  2684,  2745,  2806,  1009,  1009,   201,   201,   133,
     133,   133,   148,  2440,   193,  2087,   281,   -40,  -374,   167,
    2253,  -374,   154,  1277,  -374,  -374,   168,  -374,  -374,  -374,
    -374,  1147,  -374,  2087,  -374,    96,   287,   174,   200,  -374,
    1919,   181,  -374,  -374,   169,  1793,  -374,   -36,  -374,   204,
      26,   208,   228,   887,   212,  -374,   -20,    53,   -31,   199,
     125,   309,   125,   205,   157,  2087,  -374,  -374,   324,  2316,
    -374,  1919,  2087,   207,  -374,  -374,  1147,   202,   209,    67,
      26,  -374,  -374,   157,    25,  -374,   110,  1919,  2087,   181,
    -374,  -374,   210,  -374,  -374,  -374,  -374,  2087,  -374,  -374,
    -374,    39,  2087,  1919,    24,  -374,   125,   217,  -374,   251,
    -374,   208,  2562,  -374,  -374,  -374,   -14,    27,   221,  -374,
    -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,
    -374,   253,   233,   287,  -374,   234,   241,  -374,  -374,   110,
    -374,   267,  -374,   126,  -374,  -374,  -374,   250,  -374,   252,
     251,  -374,   125,   230,  -374,  -374,  2087,   259,   235,    27,
    -374,  -374,  -374,  -374,  -374,  -374,  -374,   362,  -374,  -374,
    1919,  -374,  1919,   157,  1407,  -374,   366,  -374,   347,   254,
     249,  -374,  2501,  1793,  -374,  -374,  -374,  1793,  1793,   157,
    -374,  -374,   256,  -374,  -374,   258,    74,   255,  1537,  -374,
    -374,  -374,  -374,    70,  -374,  -374,  1793,  -374,   257,   261,
     264,  -374,  1407,  -374,  -374,   385,  -374,  -374,  -374,  -374,
    1793,   270,   268,   265,   271,  -374,    24,  1793,  -374,   272,
     274,   157,  -374,  -374
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -374,  -374,   260,  -374,   352,  -285,  -374,    98,  -373,   -39,
       1,   -84,  -310,    31,     2,    17,   351,   289,    89,  -374,
    -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,  -374,
    -374,  -374,  -374,  -374,  -374,   -17,   -16,  -374,  -374,     5,
    -374,    13,  -374,  -374,  -374,  -374,  -374,  -374,  -374,  -202,
      36,  -374,  -374,  -374,  -374,     0,  -374,   356,  -374,  -374,
    -374,    58,  -374,    59,  -374,   -65,  -374,   -48,  -374,  -374,
     -26,  -374,     3,  -374,    32,  -374,  -369,  -374,  -298,   -47,
       4,  -374,  -374,  -374,  -374,  -374,   408,  -318,    51,    78,
    -237,  -374,   -99,  -374,  -374,  -374,  -374,  -374,  -374,  -374,
     -27,    -3,   -52,    -7,  -374,  -374,  -374,  -374,  -374,  -374
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -153
static const yytype_int16 yytable[] =
{
      68,    54,    56,    73,    74,   208,   153,   336,   221,   133,
     365,   312,   111,    98,   294,   429,   312,   101,   362,   321,
      86,    85,   385,    91,   388,    88,   357,   315,   315,   369,
     109,    55,   112,   115,   116,    92,   117,   118,   119,    92,
     112,   395,   425,   120,   426,    92,   186,   161,   -70,   240,
      95,   161,    68,    54,    56,    73,    74,   421,    71,    72,
     161,   436,   370,   103,   161,    90,   414,    96,   388,   161,
     401,   398,   402,   428,    99,   403,   404,   405,   203,   406,
     227,   204,   377,    55,   320,   102,   161,   105,    91,   437,
     487,   144,    87,   351,   488,   489,   230,   367,   219,   217,
     112,   154,   383,   292,    92,   247,   156,   391,   212,   213,
      71,    72,   407,   496,   455,   445,   112,   316,   413,   435,
     158,   248,   408,   409,   235,   236,   412,   504,   369,   321,
     107,   322,   106,   295,   510,   108,   110,   509,   415,   296,
     470,   371,   471,   416,    92,    92,    93,    68,    54,    56,
      73,    74,   382,   161,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   145,    55,   161,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   112,   293,   328,   359,   329,
     205,   360,   333,   300,   297,    71,    72,   147,   206,   214,
     218,    68,    54,   306,   358,   216,   472,   112,   222,   112,
     327,   312,   224,   184,   185,   186,   225,   226,   228,   229,
     231,   232,   490,   241,     2,   238,   208,   242,   -71,   244,
     245,   240,   337,   338,   246,   250,   251,   252,   276,   240,
      15,    16,    17,     2,    19,   202,    21,   203,   298,   313,
     204,   299,   330,   326,   332,   334,   335,   208,    25,    15,
      16,    17,   339,    19,   513,    21,   456,    30,    31,    32,
     340,   346,   348,   208,   350,   342,   354,    25,   349,   352,
     315,   184,   185,   186,   356,   456,    30,    31,    32,   208,
     364,   366,     2,    68,    54,   306,   112,   363,  -108,   368,
     377,    68,    54,   306,   381,   199,   200,   201,    15,    16,
      17,   384,    19,   202,    21,   203,   386,   393,   204,   457,
     427,   390,   450,   397,   399,   154,    25,   423,   392,   155,
     156,   422,   400,   431,   162,    30,    31,    32,   457,   396,
     424,   432,   157,   442,   158,   359,    68,    54,   306,   444,
     446,   112,  -151,   447,   463,   469,   208,   464,   208,   482,
     112,   163,   164,   165,   483,   166,   167,   168,   169,   170,
     171,  -152,   491,   452,   492,   453,   484,   493,   502,   497,
     506,   507,   183,   498,   184,   185,   186,   499,   187,   188,
     189,   355,   505,   508,   146,   511,   512,   243,   434,   150,
     192,   193,   194,   195,   196,   233,   197,   198,   199,   200,
     201,   379,   465,   466,   448,   160,   202,   501,   203,   462,
     494,   204,   449,   458,   485,   104,   495,   430,   411,   184,
     185,   186,     0,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   476,   474,   475,   480,   481,     0,
     458,   197,   198,   199,   200,   201,     0,     0,     0,     0,
       0,   202,     0,   203,     0,     0,   204,     0,   476,   474,
     475,   480,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   476,   474,   475,   480,   481,    -4,     0,     0,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,   162,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,  -153,  -153,  -153,     0,
      37,    38,     0,     0,   170,   171,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   184,
     185,   186,     0,   187,   188,   189,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,    42,     0,   195,   196,
       0,   197,   198,   199,   200,   201,     0,     0,     0,    43,
       0,   202,     0,   203,    44,    45,   204,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,    -4,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,    22,     0,    23,   125,     0,   126,
       0,     0,     0,    24,   127,    25,    26,     0,    27,   128,
      28,     0,    29,   129,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,   130,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,   132,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,    22,     0,    23,   125,     0,   126,
       0,     0,     0,    24,   127,    25,    26,     0,    27,   128,
      28,     0,    29,   129,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,   130,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,   239,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,    22,     0,    23,   125,     0,   126,
       0,     0,     0,    24,   127,    25,    26,     0,    27,   128,
      28,     0,    29,   129,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,   130,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,    47,     0,    48,     0,    49,     0,     0,     0,   380,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,  -153,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,  -153,  -153,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   184,
     185,   186,     0,   187,   188,   189,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,    42,     0,  -153,  -153,
       0,   197,   198,   199,   200,   201,     0,     0,     0,    43,
       0,   202,     0,   203,    44,    45,   204,     0,     0,    46,
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
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
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
      22,     0,    23,   125,     0,   126,     0,     0,     0,    24,
     127,    25,    26,     0,    27,   128,    28,     0,    29,   129,
      30,    31,    32,     0,     0,    33,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    37,    38,     0,   130,
       0,     0,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,    41,     0,     0,     0,     0,     0,
       0,     0,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
      44,    45,     0,     0,     0,    46,     0,    47,     0,    48,
       0,    49,     1,     2,     3,     4,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    15,
      16,    17,    18,    19,    20,    21,    22,     0,    23,   125,
       0,   126,     0,     0,     0,    24,   127,    25,    26,     0,
      27,   128,    28,     0,    29,   129,    30,    31,    32,     0,
       0,    33,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    37,    38,     0,   130,     0,     0,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,     0,     0,     0,   131,     0,
     148,     0,     3,     4,     0,     5,     6,     7,     8,     9,
       0,    43,     0,     0,     0,     0,    44,    45,     0,     0,
       0,    46,    20,    47,    22,    48,    23,    49,     0,   126,
       0,     0,     0,    24,     0,     0,     0,     0,     0,     0,
      28,     0,    29,   149,     0,     0,     0,     0,     0,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    39,     0,     0,
      97,     0,     3,     4,     0,     5,     6,     7,     8,     9,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,    20,     0,    22,     0,    23,     0,     0,     0,
       0,     0,     0,    24,     0,     0,     0,     0,     0,    43,
      28,     0,    29,     0,    44,    45,     0,     0,     0,    46,
      34,    47,    35,    36,     0,    49,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    39,     0,     0,
      97,     0,     3,     4,     0,     5,     6,     7,     8,     9,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,    20,     0,    22,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,    29,     0,    44,    45,     0,     0,     0,    46,
      34,    47,    35,    36,     0,    49,     0,     0,     0,     0,
       0,     0,   162,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,   163,
     164,   165,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,     0,   184,   185,   186,     0,   187,   188,   189,    46,
       0,    47,     0,     0,   190,    49,   191,     0,   192,   193,
     194,   195,   196,   162,   197,   198,   199,   200,   201,     0,
       0,     0,     0,     0,   202,     0,   203,     0,     0,   204,
       0,     0,     0,     0,     0,   323,     0,     0,     0,     0,
     163,   164,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,     0,   184,   185,   186,     0,   187,   188,   189,
       0,     0,     0,     0,     0,   190,   162,   191,     0,   192,
     193,   194,   195,   196,     0,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,     0,   203,     0,     0,
     204,     0,     0,   163,   164,   165,   353,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,     0,   184,   185,   186,     0,
     187,   188,   189,     0,     0,     0,     0,     0,   190,   162,
     191,     0,   192,   193,   194,   195,   196,     0,   197,   198,
     199,   200,   201,     0,     0,     0,     0,     0,   202,     0,
     203,     0,     0,   204,     0,     0,   163,   164,   165,   394,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,     0,   184,
     185,   186,     0,   187,   188,   189,     0,     0,     0,     0,
     162,   190,     0,   191,   345,   192,   193,   194,   195,   196,
       0,   197,   198,   199,   200,   201,     0,     0,     0,     0,
       0,   202,     0,   203,     0,     0,   204,   163,   164,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,     0,
     184,   185,   186,     0,   187,   188,   189,     0,     0,     0,
       0,   162,   190,     0,   191,     0,   192,   193,   194,   195,
     196,     0,   197,   198,   199,   200,   201,     0,     0,     0,
       0,     0,   202,     0,   203,   347,     0,   204,   163,   164,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
       0,   184,   185,   186,     0,   187,   188,   189,     0,     0,
       0,     0,   162,   190,     0,   191,   486,   192,   193,   194,
     195,   196,     0,   197,   198,   199,   200,   201,     0,     0,
       0,     0,     0,   202,     0,   203,     0,     0,   204,   163,
     164,   165,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,     0,   184,   185,   186,     0,   187,   188,   189,     0,
       0,     0,     0,   162,   190,     0,   191,     0,   192,   193,
     194,   195,   196,     0,   197,   198,   199,   200,   201,     0,
       0,     0,     0,     0,   202,     0,   203,     0,     0,   204,
     163,   164,   165,     0,   166,   167,   168,   169,   170,   171,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   184,   185,   186,     0,   187,   188,   189,
       0,     0,     0,     0,   162,     0,     0,     0,     0,   192,
     193,   194,   195,   196,     0,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,     0,   203,     0,     0,
     204,   163,   164,   165,     0,   166,   167,   168,   169,   170,
     171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,   185,   186,     0,   187,   188,
     189,     0,     0,     0,     0,   162,     0,     0,     0,     0,
       0,   193,   194,   195,   196,     0,   197,   198,   199,   200,
     201,     0,     0,     0,     0,     0,   202,     0,   203,     0,
       0,   204,   163,   164,   165,     0,   166,   167,   168,   169,
     170,   171,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,   185,   186,     0,   187,
     188,   189,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,     0,   194,   195,   196,     0,   197,   198,   199,
     200,   201,     0,     0,     0,     0,     0,   202,     0,   203,
       0,     0,   204,   163,   164,   165,     0,   166,   167,   168,
     169,   170,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   184,   185,   186,     0,
     187,   188,   189,     0,     0,     0,     0,   162,     0,     0,
       0,     0,     0,     0,  -153,   195,   196,     0,   197,   198,
     199,   200,   201,     0,     0,     0,     0,     0,   202,     0,
     203,     0,     0,   204,   163,   164,   165,     0,  -153,  -153,
    -153,  -153,   170,   171,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   184,   185,   186,
       0,   187,   188,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   195,   196,     0,   197,
     198,   199,   200,   201,     0,     0,     0,     0,     0,   202,
       0,   203,     0,     0,   204
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,     0,    89,    58,   244,   107,    48,
     320,   213,    39,    20,     3,   384,   218,    24,   316,     3,
      50,    89,   340,     3,   342,   122,   311,     3,     3,     3,
      37,     0,    39,    40,    41,    19,    43,    44,    45,    19,
      47,   351,     3,    46,     5,    19,    92,   100,    68,   133,
      14,   100,    52,    52,    52,    52,    52,   367,     0,     0,
     100,    34,    36,    27,   100,   122,   364,    18,   386,   100,
       3,   356,     5,   383,     3,     8,     9,    10,   124,    12,
     133,   127,   102,    52,   133,   122,   100,   122,     3,    62,
     463,     3,   122,   133,   467,   468,   123,   133,   105,   102,
     107,    27,   133,   202,    19,    35,    32,   344,   126,   127,
      52,    52,    45,   486,   432,   413,   123,    93,    93,   133,
      46,    51,    55,    56,   127,   128,   363,   500,     3,     3,
     122,   115,   126,   122,   507,   127,   122,   506,    28,   128,
     450,   115,   452,    33,    19,    19,   126,   147,   147,   147,
     147,   147,    99,   100,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,     0,   147,   100,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   224,   102,   226,
       3,   105,   241,   210,   207,   147,   147,   126,   122,     3,
     127,   211,   211,   211,   313,   122,   453,   224,     3,   226,
     223,   423,   105,    90,    91,    92,   132,   100,   125,   100,
       3,     3,   469,   126,     4,     3,   320,    18,    68,     3,
      99,   325,   245,   246,    68,     3,     3,     3,     3,   333,
      20,    21,    22,     4,    24,   122,    26,   124,    99,   122,
     127,    14,   105,   133,   100,     3,   132,   351,    38,    20,
      21,    22,     3,    24,   511,    26,    46,    47,    48,    49,
      54,   133,    89,   367,     3,    54,   132,    38,   295,   122,
       3,    90,    91,    92,   126,    46,    47,    48,    49,   383,
     100,   132,     4,   303,   303,   303,   313,   133,   127,   105,
     102,   311,   311,   311,   102,   114,   115,   116,    20,    21,
      22,   122,    24,   122,    26,   124,    17,     3,   127,    99,
     382,   126,    65,   126,   132,    27,    38,   127,   345,    31,
      32,   368,   133,   126,    40,    47,    48,    49,    99,   352,
     377,   100,    44,   132,    46,   102,   356,   356,   356,   126,
     126,   368,   132,   122,   105,     3,   450,   132,   452,     3,
     377,    67,    68,    69,    27,    71,    72,    73,    74,    75,
      76,   132,   126,   133,   126,   133,   132,   132,     3,   132,
     122,   126,    88,   132,    90,    91,    92,   133,    94,    95,
      96,   303,   132,   132,    52,   133,   132,   147,   391,    58,
     106,   107,   108,   109,   110,   126,   112,   113,   114,   115,
     116,   332,   439,   439,   419,    69,   122,   492,   124,   436,
     478,   127,   419,   433,   460,    27,   483,   386,   360,    90,
      91,    92,    -1,   411,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   454,   454,   454,   454,   454,    -1,
     460,   112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,    -1,   127,    -1,   478,   478,
     478,   478,   478,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   492,   492,   492,   492,   492,     0,    -1,    -1,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
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
      94,    95,    96,    -1,    -1,    -1,    -1,    -1,   102,    40,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,    -1,   127,    -1,    -1,    67,    68,    69,   133,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    -1,    -1,    -1,    -1,
      40,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,    -1,   127,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    -1,    -1,    -1,
      -1,    40,   102,    -1,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,   127,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    -1,    -1,
      -1,    -1,    40,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,    -1,    -1,   127,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    -1,
      -1,    -1,    -1,    40,   102,    -1,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,   127,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    -1,    94,    95,    96,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,
     127,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    -1,    94,    95,
      96,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,   127,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    -1,    94,
      95,    96,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,    -1,   127,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    -1,
      94,    95,    96,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,    -1,   127,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      -1,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,    -1,    -1,   127
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
     236,   237,   238,   241,   243,    89,    50,   122,   122,   164,
     122,     3,    19,   126,   183,   184,    18,     3,   237,     3,
     216,   237,   122,   184,   220,   122,   126,   122,   127,   237,
     122,   234,   237,   239,   240,   237,   237,   237,   237,   237,
     235,   226,   227,   228,   234,    30,    32,    37,    42,    46,
      66,    99,   132,   143,   144,   145,   148,   150,   165,   166,
     181,   189,   233,   242,     3,     0,   138,   126,     3,    46,
     150,   156,   157,   236,    27,    31,    32,    44,    46,   205,
     191,   100,    40,    67,    68,    69,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    90,    91,    92,    94,    95,    96,
     102,   104,   106,   107,   108,   109,   110,   112,   113,   114,
     115,   116,   122,   124,   127,     3,   122,   162,   145,   146,
     172,   187,   126,   127,     3,   229,   122,   235,   127,   237,
     180,   226,     3,   155,   105,   132,   100,   133,   125,   100,
     234,     3,     3,   151,   152,   235,   235,   151,     3,   132,
     145,   126,    18,   136,     3,    99,    68,    35,    51,   213,
       3,     3,     3,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,     3,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   226,   237,     3,   122,   128,   235,    99,    14,
     237,    99,   139,   140,   141,   147,   148,   195,   197,   206,
     214,   186,   183,   122,   225,     3,    93,   210,   211,   212,
     133,     3,   115,   133,   142,   143,   133,   235,   234,   234,
     105,   224,   100,   143,     3,   132,   224,   235,   235,     3,
      54,   192,    54,   193,   207,   105,   133,   125,    89,   237,
       3,   133,   122,   133,   132,   141,   126,   139,   226,   102,
     105,   208,   212,   133,   100,   146,   132,   133,   105,     3,
      36,   115,   184,   219,   220,   221,   223,   102,   149,   152,
     132,   102,    99,   133,   122,   221,    17,   194,   221,   222,
     126,   224,   237,     3,   133,   146,   235,   126,   139,   132,
     133,     3,     5,     8,     9,    10,    12,    45,    55,    56,
     209,   223,   224,    93,   212,    28,    33,   173,   175,   177,
     178,   146,   234,   127,   234,     3,     5,   236,   146,   210,
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
#line 1769 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1770 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1772 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 29:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1773 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 41:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1787 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 42:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1788 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 43:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1791 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 50:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1798 "parser.y"
    {(yyval.code)=(yyvsp[(3) - (4)].code);}
    }
    break;


  
    case 51:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1800 "parser.y"
    {/*TODO*/(yyval.code)=0;}
    }
    break;


  
    case 52:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1804 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1805 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1809 "parser.y"
    {
    code_t**cc = &global->init->method->body->code;
    *cc = code_append(*cc, (yyvsp[(1) - (1)].code));
}
    }
    break;


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1820 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (2)].value);}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1821 "parser.y"
    {(yyval.value).c=abc_pushundefined(0);
                                  (yyval.value).t=TYPE_ANY;
                                 }
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1825 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1826 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1828 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1829 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1832 "parser.y"
    {
    if(variable_exists((yyvsp[(1) - (3)].id)))
        syntaxerror("Variable %s already defined", (yyvsp[(1) - (3)].id));
   
    if(!is_subtype_of((yyvsp[(3) - (3)].value).t, (yyvsp[(2) - (3)].classinfo))) {
        syntaxerror("Can't convert %s to %s", (yyvsp[(3) - (3)].value).t->name, 
                                              (yyvsp[(2) - (3)].classinfo)->name);
    }

    int index = new_variable((yyvsp[(1) - (3)].id), (yyvsp[(2) - (3)].classinfo), 1);
    
    if((yyvsp[(2) - (3)].classinfo)) {
        if((yyvsp[(3) - (3)].value).c->prev || (yyvsp[(3) - (3)].value).c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = (yyvsp[(3) - (3)].value).c;
            (yyval.code) = converttype((yyval.code), (yyvsp[(3) - (3)].value).t, (yyvsp[(2) - (3)].classinfo));
            (yyval.code) = abc_setlocal((yyval.code), index);
        } else {
            code_free((yyvsp[(3) - (3)].value).c);
            (yyval.code) = defaultvalue(0, (yyvsp[(2) - (3)].classinfo));
            (yyval.code) = abc_setlocal((yyval.code), index);
        }
    } else {
        if((yyvsp[(3) - (3)].value).c->prev || (yyvsp[(3) - (3)].value).c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = (yyvsp[(3) - (3)].value).c;
            (yyval.code) = abc_coerce_a((yyval.code));
            (yyval.code) = abc_setlocal((yyval.code), index);
        } else {
            code_free((yyvsp[(3) - (3)].value).c);
            (yyval.code) = code_new();
        }
    }
    
    /* that's the default for a local register, anyway
        else {
        state->method->initcode = abc_pushundefined(state->method->initcode);
        state->method->initcode = abc_setlocal(state->method->initcode, index);
    }*/
    //printf("variable %s -> %d (%s)\n", $2->text, index, $4.t?$4.t->name:"");
}
    }
    break;


  
    case 63:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1874 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1875 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1878 "parser.y"
    {new_state();}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1878 "parser.y"
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
    old_state();
}
    }
    break;


  
    case 67:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1897 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1902 "parser.y"
    {
    (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1);
}
    }
    break;


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1905 "parser.y"
    {
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 72:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1909 "parser.y"
    {new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 73:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1910 "parser.y"
    {new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1912 "parser.y"
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
    old_state();
}
    }
    break;


  
    case 75:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1932 "parser.y"
    {
    variable_t*var = find_variable((yyvsp[(2) - (6)].id));
    char*tmp1name = concat2((yyvsp[(2) - (6)].id), "__tmp1__");
    int it = new_variable(tmp1name, TYPE_INT, 0);
    char*tmp2name = concat2((yyvsp[(2) - (6)].id), "__array__");
    int array = new_variable(tmp1name, 0, 0);

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
    old_state();

    free(tmp1name);
    free(tmp2name);
}
    }
    break;


  
    case 76:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1975 "parser.y"
    {new_state();}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1975 "parser.y"
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
    old_state();
}
    }
    break;


  
    case 78:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1994 "parser.y"
    {new_state();}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1994 "parser.y"
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
    old_state();
}
    }
    break;


  
    case 80:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2009 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2012 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2015 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2018 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2022 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2023 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2024 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2025 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2026 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2027 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2029 "parser.y"
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
#line 2040 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2043 "parser.y"
    {new_state();}
    }
    break;


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2043 "parser.y"
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
    old_state();
}
    }
    break;


  
    case 94:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2072 "parser.y"
    {new_state();state->exception_name=(yyvsp[(3) - (5)].id);new_variable((yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0);}
    }
    break;


  
    case 95:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2073 "parser.y"
    {
    namespace_t name_ns = {ACCESS_PACKAGE, ""};
    multiname_t name = {QNAME, &name_ns, 0, (yyvsp[(3) - (9)].id)};
    
    NEW(abc_exception_t, e)
    e->exc_type = sig2mname((yyvsp[(4) - (9)].classinfo));
    e->var_name = multiname_clone(&name);
    (yyval.exception) = e;

    code_t*c = 0;
    int i = find_variable_safe((yyvsp[(3) - (9)].id))->index;
    e->target = c = abc_nop(0);
    c = abc_setlocal(c, i);
    c = code_append(c, (yyvsp[(8) - (9)].code));
    c = abc_kill(c, i);

    c = var_block(c);
    old_state();
}
    }
    break;


  
    case 96:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2092 "parser.y"
    {new_state();state->exception_name=0;}
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2092 "parser.y"
    {
    (yyvsp[(4) - (5)].code) = var_block((yyvsp[(4) - (5)].code));
    if(!(yyvsp[(4) - (5)].code)) {
        (yyval.exception)=0;
        old_state();
    } else {
        NEW(abc_exception_t, e)
        e->exc_type = 0; //all exceptions
        e->var_name = 0; //no name
        e->target = 0;
        e->to = abc_nop(0);
        e->to = code_append(e->to, (yyvsp[(4) - (5)].code));
        old_state();
        (yyval.exception) = e;
    }
}
    }
    break;


  
    case 98:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2109 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2110 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2111 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2112 "parser.y"
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
#line 2120 "parser.y"
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
#line 2129 "parser.y"
    {new_state();}
    }
    break;


  
    case 104:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2129 "parser.y"
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
        tmp = new_variable("__finally__", 0, 0);
    
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
    old_state();
}
    }
    break;


  
    case 105:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2174 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2178 "parser.y"
    {
    if(!state->exception_name)
        syntaxerror("re-throw only possible within a catch block");
    variable_t*v = find_variable(state->exception_name);
    (yyval.code)=code_new();
    (yyval.code)=abc_getlocal((yyval.code), v->index);
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 107:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2189 "parser.y"
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
#line 2199 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 110:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2201 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 111:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2202 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2204 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2205 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2206 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2207 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2209 "parser.y"
    {
       PASS1 
       if(!registry_find((yyvsp[(2) - (2)].classinfo)->package, (yyvsp[(2) - (2)].classinfo)->name)) {
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
#line 2223 "parser.y"
    {
       PASS1 
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
#line 2239 "parser.y"
    {PASS12 (yyval.flags)=0;}
    }
    break;


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2240 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2241 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].token);}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2242 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (2)].flags)|(yyvsp[(2) - (2)].token);}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2244 "parser.y"
    {PASS12 (yyval.token)=FLAG_PUBLIC;}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2245 "parser.y"
    {PASS12 (yyval.token)=FLAG_PRIVATE;}
    }
    break;


  
    case 124:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2246 "parser.y"
    {PASS12 (yyval.token)=FLAG_PROTECTED;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2247 "parser.y"
    {PASS12 (yyval.token)=FLAG_STATIC;}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2248 "parser.y"
    {PASS12 (yyval.token)=FLAG_DYNAMIC;}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2249 "parser.y"
    {PASS12 (yyval.token)=FLAG_FINAL;}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2250 "parser.y"
    {PASS12 (yyval.token)=FLAG_OVERRIDE;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2251 "parser.y"
    {PASS12 (yyval.token)=FLAG_NATIVE;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2252 "parser.y"
    {PASS12 (yyval.token)=FLAG_PACKAGEINTERNAL;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2253 "parser.y"
    {PASS12 (yyval.token)=FLAG_NAMESPACE;}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2255 "parser.y"
    {(yyval.classinfo)=registry_getobjectclass();}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2256 "parser.y"
    {(yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2258 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2259 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2261 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2262 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2266 "parser.y"
    {PASS12 startclass((yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2268 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2272 "parser.y"
    {PASS12 startclass((yyvsp[(1) - (5)].flags)|FLAG_INTERFACE,(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2274 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 150:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2287 "parser.y"
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
#line 2298 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 157:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2301 "parser.y"
    {
    PASS12
    (yyvsp[(1) - (8)].flags) |= FLAG_PUBLIC;
    if((yyvsp[(1) - (8)].flags)&(FLAG_PRIVATE|FLAG_PACKAGEINTERNAL|FLAG_PROTECTED)) {
        syntaxerror("invalid method modifiers: interface methods always need to be public");
    }
    startfunction(0,(yyvsp[(1) - (8)].flags),(yyvsp[(3) - (8)].token),(yyvsp[(4) - (8)].id),&(yyvsp[(6) - (8)].params),(yyvsp[(8) - (8)].classinfo));
    endfunction(0,(yyvsp[(1) - (8)].flags),(yyvsp[(3) - (8)].token),(yyvsp[(4) - (8)].id),&(yyvsp[(6) - (8)].params),(yyvsp[(8) - (8)].classinfo), 0);
    list_deep_free((yyvsp[(6) - (8)].params).list);
}
    }
    break;


  
    case 160:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2316 "parser.y"
    {setstaticfunction((yyvsp[(1) - (3)].flags));}
    }
    break;


  
    case 161:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2316 "parser.y"
    {
    int flags = (yyvsp[(1) - (6)].flags);
    U8 access = flags2access((yyvsp[(1) - (6)].flags));

    varinfo_t* info = 0;
    if(state->cls) {
        memberinfo_t*i = registry_findmember(state->cls->info, (yyvsp[(3) - (6)].id), 1);
        if(i) {
            check_override(i, flags);
        }
        info = varinfo_register_onclass(state->cls->info, access, (yyvsp[(3) - (6)].id));
    } else {
        slotinfo_t*i = registry_find(state->package, (yyvsp[(3) - (6)].id));
        if(i) {
            syntaxerror("package %s already contains '%s'", state->package, (yyvsp[(3) - (6)].id));
        }
        info = varinfo_register_global(access, state->package, (yyvsp[(3) - (6)].id));
    }

    info->type = (yyvsp[(5) - (6)].classinfo);
    info->flags = flags;

    /* slot name */
    namespace_t mname_ns = {access, ""};
    multiname_t mname = {QNAME, &mname_ns, 0, (yyvsp[(3) - (6)].id)};
  
    trait_list_t**traits;
    code_t**code;
    if(!state->cls) {
        // global variable
        mname_ns.name = state->package;
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
#line 2389 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 163:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2390 "parser.y"
    {(yyval.constant)=(yyvsp[(2) - (2)].constant);}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2392 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_uint));}
    }
    break;


  
    case 165:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2393 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 166:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2394 "parser.y"
    {(yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));}
    }
    break;


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2395 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2396 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2398 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2399 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2400 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2401 "parser.y"
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
#line 2410 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2414 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2420 "parser.y"
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
#line 2426 "parser.y"
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
#line 2434 "parser.y"
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
#line 2439 "parser.y"
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
#line 2445 "parser.y"
    {
     PASS1 (yyval.param)=0;
     PASS2
     (yyval.param) = malloc(sizeof(param_t));
     (yyval.param)->name=(yyvsp[(1) - (4)].id);
     (yyval.param)->type = (yyvsp[(3) - (4)].classinfo);
     (yyval.param)->value = (yyvsp[(4) - (4)].constant);
}
    }
    break;


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2453 "parser.y"
    {
     PASS1 (yyval.param)=0;
     PASS2
     (yyval.param) = malloc(sizeof(param_t));
     (yyval.param)->name=(yyvsp[(1) - (2)].id);
     (yyval.param)->type = TYPE_ANY;
     (yyval.param)->value = (yyvsp[(2) - (2)].constant);
}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2461 "parser.y"
    {(yyval.token)=(yyvsp[(1) - (1)].token);}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2462 "parser.y"
    {(yyval.token)=(yyvsp[(1) - (1)].token);}
    }
    break;


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2463 "parser.y"
    {(yyval.token)=0;}
    }
    break;


  
    case 184:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2466 "parser.y"
    {PASS12 startfunction(0,(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2467 "parser.y"
    {
    PASS1 old_state();list_deep_free((yyvsp[(6) - (12)].params).list);
    PASS2
    if(!state->method->info) syntaxerror("internal error");
    
    code_t*c = method_header(state->method);
    c = wrap_function(c, 0, (yyvsp[(11) - (12)].code));

    endfunction(0,(yyvsp[(1) - (12)].flags),(yyvsp[(3) - (12)].token),(yyvsp[(4) - (12)].id),&(yyvsp[(6) - (12)].params),(yyvsp[(8) - (12)].classinfo),c);
    list_deep_free((yyvsp[(6) - (12)].params).list);
    (yyval.code)=0;
}
    }
    break;


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2481 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2483 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2484 "parser.y"
    {
    PASS1 old_state();list_deep_free((yyvsp[(4) - (10)].params).list);
    PASS2
    methodinfo_t*f = state->method->info;
    if(!f || !f->kind) syntaxerror("internal error");
    
    code_t*c = method_header(state->method);
    c = wrap_function(c, 0, (yyvsp[(9) - (10)].code));

    int index = state->method->var_index;
    endfunction(0,0,0,(yyvsp[(2) - (10)].id),&(yyvsp[(4) - (10)].params),(yyvsp[(6) - (10)].classinfo),c);
    list_deep_free((yyvsp[(4) - (10)].params).list);
    
    (yyval.value).c = abc_getlocal(0, index);
    (yyval.value).t = TYPE_FUNCTION(f);
}
    }
    break;


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2504 "parser.y"
    {
    PASS1 (yyval.classinfo)=0;
    PASS2
    /* try current package */
    slotinfo_t*s = find_class((yyvsp[(1) - (1)].id));
    if(!s) syntaxerror("Could not find class/method %s\n", (yyvsp[(1) - (1)].id));
    (yyval.classinfo) = (classinfo_t*)s;
}
    }
    break;


  
    case 191:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2513 "parser.y"
    {
    PASS1 static classinfo_t c;
          memset(&c, 0, sizeof(c));
          c.package = (yyvsp[(1) - (3)].id);
          c.name = (yyvsp[(3) - (3)].id);
          (yyval.classinfo)=&c;
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
#line 2529 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2530 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2532 "parser.y"
    {(yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2533 "parser.y"
    {(yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2534 "parser.y"
    {(yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2543 "parser.y"
    {(yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2544 "parser.y"
    {(yyval.classinfo)=0;}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2548 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2549 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 203:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2551 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2555 "parser.y"
    {(yyval.value_list).len=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2559 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 208:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2560 "parser.y"
    {
                                                  (yyval.value_list).len= (yyvsp[(1) - (2)].value_list).len+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2566 "parser.y"
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
        trait_t*t = abc_class_find_slotid(state->cls->abc,slot);//FIXME
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
#line 2602 "parser.y"
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
    } else if((yyval.value).c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)(yyval.value).c->data[0];
        trait_t*t = abc_class_find_slotid(state->cls->abc,slot);//FIXME
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
#line 2648 "parser.y"
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
#line 2669 "parser.y"
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
        multiname_t*name = abc_class_find_slotid(state->cls->abc,slot)->name;
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
#line 2690 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 215:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2693 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 216:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2700 "parser.y"
    {(yyval.value)=(yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 217:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2701 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 218:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2702 "parser.y"
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
#line 2708 "parser.y"
    {
    (yyval.code)=cut_last_push((yyvsp[(1) - (1)].value).c);
}
    }
    break;


  
    case 220:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2714 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2718 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2720 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 224:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2722 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2726 "parser.y"
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
#line 2743 "parser.y"
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
#line 2748 "parser.y"
    {(yyval.value).c = abc_pushshort(0, (yyvsp[(1) - (1)].number_uint));
                    (yyval.value).t = TYPE_INT;
                   }
    }
    break;


  
    case 229:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2751 "parser.y"
    {(yyval.value).c = abc_pushint(0, (yyvsp[(1) - (1)].number_int));
                  (yyval.value).t = TYPE_INT;
                 }
    }
    break;


  
    case 230:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2754 "parser.y"
    {(yyval.value).c = abc_pushuint(0, (yyvsp[(1) - (1)].number_uint));
                   (yyval.value).t = TYPE_UINT;
                  }
    }
    break;


  
    case 231:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2757 "parser.y"
    {(yyval.value).c = abc_pushdouble(0, (yyvsp[(1) - (1)].number_float));
                    (yyval.value).t = TYPE_FLOAT;
                   }
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2760 "parser.y"
    {(yyval.value).c = abc_pushstring2(0, &(yyvsp[(1) - (1)].str));free((char*)(yyvsp[(1) - (1)].str).str);
                     (yyval.value).t = TYPE_STRING;
                    }
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2763 "parser.y"
    {(yyval.value).c = abc_pushundefined(0);
                    (yyval.value).t = TYPE_ANY;
                   }
    }
    break;


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2766 "parser.y"
    {(yyval.value).c = abc_pushtrue(0);
                    (yyval.value).t = TYPE_BOOLEAN;
                   }
    }
    break;


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2769 "parser.y"
    {(yyval.value).c = abc_pushfalse(0);
                     (yyval.value).t = TYPE_BOOLEAN;
                    }
    }
    break;


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2772 "parser.y"
    {(yyval.value).c = abc_pushnull(0);
                    (yyval.value).t = TYPE_NULL;
                   }
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2776 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2779 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2782 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2785 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2788 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 242:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2791 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
              }
    }
    break;


  
    case 243:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2794 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2797 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2801 "parser.y"
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
#line 2812 "parser.y"
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
#line 2830 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2835 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_bitnot((yyval.value).c);
              (yyval.value).t = TYPE_INT;
             }
    }
    break;


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2840 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitand((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2845 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitxor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2850 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2855 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_rshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2859 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_urshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2863 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_lshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2868 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_divide((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2872 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_modulo((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2876 "parser.y"
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
#line 2885 "parser.y"
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
#line 2894 "parser.y"
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
#line 2904 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_in((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2909 "parser.y"
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
#line 2922 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_instanceof((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2927 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_istypelate((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2932 "parser.y"
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
#line 2938 "parser.y"
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
#line 2944 "parser.y"
    { (yyval.value).c = abc_pushundefined(0);
             (yyval.value).t = TYPE_ANY;
           }
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2948 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (3)].value);}
    }
    break;


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2950 "parser.y"
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
#line 2961 "parser.y"
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
#line 2970 "parser.y"
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
#line 2977 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2978 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2980 "parser.y"
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
#line 2986 "parser.y"
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
#line 2995 "parser.y"
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
#line 3002 "parser.y"
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
#line 3014 "parser.y"
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
#line 3020 "parser.y"
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
#line 3026 "parser.y"
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
#line 3032 "parser.y"
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
#line 3038 "parser.y"
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
#line 3044 "parser.y"
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
#line 3050 "parser.y"
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
#line 3056 "parser.y"
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
#line 3069 "parser.y"
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
#line 3080 "parser.y"
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
#line 3087 "parser.y"
    { 
              (yyval.value).t = join_types((yyvsp[(3) - (5)].value).t,(yyvsp[(5) - (5)].value).t,'?');
              (yyval.value).c = (yyvsp[(1) - (5)].value).c;
              code_t*j1 = (yyval.value).c = abc_iffalse((yyval.value).c, 0);
              (yyval.value).c = code_append((yyval.value).c, (yyvsp[(3) - (5)].value).c);
              (yyval.value).c = converttype((yyval.value).c, (yyvsp[(3) - (5)].value).t, (yyval.value).t);
              code_t*j2 = (yyval.value).c = abc_jump((yyval.value).c, 0);
              (yyval.value).c = j1->branch = abc_label((yyval.value).c);
              (yyval.value).c = code_append((yyval.value).c, (yyvsp[(5) - (5)].value).c);
              (yyval.value).c = converttype((yyval.value).c, (yyvsp[(3) - (5)].value).t, (yyval.value).t);
              (yyval.value).c = j2->branch = abc_label((yyval.value).c);
            }
    }
    break;


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3100 "parser.y"
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
#line 3127 "parser.y"
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
#line 3141 "parser.y"
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
#line 3155 "parser.y"
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
#line 3170 "parser.y"
    { if(!state->cls->info)
                  syntaxerror("super keyword not allowed outside a class");
              classinfo_t*t = state->cls->info->superclass;
              if(!t) t = TYPE_OBJECT;

              memberinfo_t*f = registry_findmember(t, (yyvsp[(3) - (3)].id), 1);
              namespace_t ns = {f->access, ""};
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
#line 3184 "parser.y"
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
#line 3191 "parser.y"
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
#line 3198 "parser.y"
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
#line 3205 "parser.y"
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
#line 3212 "parser.y"
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
#line 3224 "parser.y"
    {(yyval.value).c = (yyvsp[(1) - (3)].value).c;
             classinfo_t*t = (yyvsp[(1) - (3)].value).t;
             char is_static = 0;
             if(TYPE_IS_CLASS(t) && t->data) {
                 t = t->data;
                 is_static = 1;
             }
             if(t) {
                 memberinfo_t*f = registry_findmember(t, (yyvsp[(3) - (3)].id), 1);
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
#line 3257 "parser.y"
    {
    (yyval.value).t = 0;
    (yyval.value).c = 0;
    slotinfo_t*a = 0;
    memberinfo_t*f = 0;

    variable_t*v;
    /* look at variables */
    if((v = find_variable((yyvsp[(1) - (1)].id)))) {
        // $1 is a local variable
        (yyval.value).c = abc_getlocal((yyval.value).c, v->index);
        (yyval.value).t = v->type;
        break;
    }

    int i_am_static = (state->method && state->method->info)?(state->method->info->flags&FLAG_STATIC):FLAG_STATIC;

    /* look at current class' members */
    if(state->cls && (f = registry_findmember(state->cls->info, (yyvsp[(1) - (1)].id), 1)) &&
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
#line 3359 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 301:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3360 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 302:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3361 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 303:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3363 "parser.y"
    {
    PASS12
    tokenizer_register_namespace((yyvsp[(3) - (3)].id));
    (yyval.code)=0;
}
    }
    break;




/* Line 1464 of skeleton.m4  */
#line 7705 "parser.tab.c"
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



