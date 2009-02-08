
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
     T_SHRBY = 337,
     T_SHLBY = 338,
     T_USHRBY = 339,
     T_OROR = 340,
     T_ANDAND = 341,
     T_COLONCOLON = 342,
     T_MINUSMINUS = 343,
     T_PLUSPLUS = 344,
     T_DOTDOT = 345,
     T_DOTDOTDOT = 346,
     T_SHL = 347,
     T_USHR = 348,
     T_SHR = 349,
     prec_none = 350,
     below_semicolon = 351,
     below_assignment = 352,
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
#line 273 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 257 "parser.y"


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

typedef struct _classstate {
    /* class data */
    classinfo_t*info;
    abc_class_t*abc;
    code_t*init;
    code_t*static_init;
    char has_constructor;
} classstate_t;

DECLARE_LIST(methodstate);

typedef struct _methodstate {
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

    abc_exception_list_t*exceptions;
    
    methodstate_list_t*innerfunctions;
} methodstate_t;

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
    if(leaving->cls && leaving->cls != state->cls) {
        free(leaving->cls);
        leaving->cls=0;
    }
    
    state_destroy(leaving);
}

static char* internal_filename_package = 0;
void initialize_file(char*filename)
{
    if(state) {
        syntaxerror("invalid call to initialize_file during parsing of another file");
    }
    new_state();
    state->package = internal_filename_package = strdup(filename);
    
    state->method = rfx_calloc(sizeof(methodstate_t));
    state->method->variable_count = 1;

    global->token2info = dict_lookup(global->file2token2info, 
                                     current_filename // use long version
                                    );
    if(!global->token2info) {
        global->token2info = dict_new2(&ptr_type);
        dict_put(global->file2token2info, current_filename, global->token2info);
    }
}

void finish_file()
{
    if(!state || state->level!=1) {
        syntaxerror("unexpected end of file in pass %d", as3_pass);
    }
    free(state->method);state->method=0;

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
    
    global->init = abc_initscript(global->file);
    code_t*c = global->init->method->body->code;
    c = abc_getlocal_0(c);
    c = abc_pushscope(c);
    global->init->method->body->code = c;
}

void* finish_parser()
{
    code_t*c = global->init->method->body->code;
    /*c = abc_findpropstrict(c, "[package]::trace");
      c = abc_pushstring(c, "[leaving global init function]");
      c = abc_callpropvoid(c, "[package]::trace", 1);*/
    c = abc_returnvoid(c);
    global->init->method->body->code = c;

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

   
code_t* method_header()
{
    code_t*c = 0;
    if(state->method->late_binding && !state->method->inner) {
        c = abc_getlocal_0(c);
        c = abc_pushscope(c);
    }
    /*if(state->method->innerfunctions) {
        c = abc_newactivation(c);
        c = abc_pushscope(c);
    }*/
    if(state->method->is_constructor && !state->method->has_super) {
        // call default constructor
        c = abc_getlocal_0(c);
        c = abc_constructsuper(c, 0);
    }
    methodstate_list_t*l = state->method->innerfunctions;
    while(l) {
        parserassert(l->methodstate->abc);
        c = abc_newfunction(c, l->methodstate->abc);
        c = abc_setlocal(c, l->methodstate->var_index);
        free(l->methodstate);l->methodstate=0;
        l = l->next;
    }
    list_free(state->method->innerfunctions);
    state->method->innerfunctions = 0;
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

char*as3_globalclass=0;
static void startclass(int flags, char*classname, classinfo_t*extends, classinfo_list_t*implements)
{
    if(state->cls) {
        syntaxerror("inner classes now allowed"); 
    }
    new_state();
    state->cls = rfx_calloc(sizeof(classstate_t));

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
        state->method = rfx_calloc(sizeof(methodstate_t)); // method state, for static constructor
        state->method->variable_count = 1;
        dict_put(global->token2info, (void*)(ptroff_t)as3_tokencount, state->method);

        if(registry_find(package, classname)) {
            syntaxerror("Package \"%s\" already contains a class called \"%s\"", package, classname);
        }
        /* build info struct */
        int num_interfaces = (list_length(implements));
        state->cls->info = classinfo_register(access, package, classname, num_interfaces);
        state->cls->info->flags |= flags & (FLAG_DYNAMIC|FLAG_INTERFACE|FLAG_FINAL);
    }
    
    if(as3_pass == 2) {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);

        state->cls->info = (classinfo_t*)registry_find(package, classname);
        parserassert((int)state->cls->info);

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

static void endclass()
{
    if(as3_pass == 2) {
        if(!state->cls->has_constructor && !(state->cls->info->flags&FLAG_INTERFACE)) {
            code_t*c = 0;
            c = abc_getlocal_0(c);
            c = abc_constructsuper(c, 0);
            state->cls->init = code_append(state->cls->init, c);
        }
        if(!state->method->late_binding) {
            // class initialization code uses late binding
            code_t*c = 0;
            c = abc_getlocal_0(c);
            c = abc_pushscope(c);
            state->cls->static_init = code_append(c, state->cls->static_init);
        }

        if(state->cls->init) {
            abc_method_t*m = abc_class_getconstructor(state->cls->abc, 0);
            m->body->code = wrap_function(0, state->cls->init, m->body->code);
        }
        if(state->cls->static_init) {
            abc_method_t*m = abc_class_getstaticconstructor(state->cls->abc, 0);
            m->body->code = wrap_function(0, state->cls->static_init, m->body->code);
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

static void function_initvars(params_t*params, int flags)
{
    if(state->method->inner)
        new_variable("this", 0, 0);
    else if(!state->method->is_global)
        new_variable((flags&FLAG_STATIC)?"class":"this", state->cls->info, 0);
    else
        new_variable("globalscope", 0, 0);

    param_list_t*p=0;
    for(p=params->list;p;p=p->next) {
        new_variable(p->param->name, p->param->type, 0);
    }
    
    methodstate_list_t*l = state->method->innerfunctions;
    while(l) {
        methodstate_t*m = l->methodstate;
        m->var_index = new_variable(m->info->name, TYPE_FUNCTION(m->info), 0);
        l = l->next;
    }
}

static void innerfunction(char*name, params_t*params, classinfo_t*return_type)
{
    parserassert(state->method && state->method->info);

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
        minfo->name = name;
        state->method->info = minfo;

        list_append(parent_method->innerfunctions, state->method);

        dict_put(global->token2info, (void*)(ptroff_t)as3_tokencount, state->method);
    }

    if(as3_pass == 2) {
        state->method = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);
        parserassert(state->method);

        state->method->info->return_type = return_type;
        function_initvars(params, 0);
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
        state->method->variable_count = 0;

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
        function_initvars(params, flags);
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
#line 1723 "parser.tab.c"

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
#define YYFINAL  125
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3223

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  109
/* YYNRULES -- Number of rules.  */
#define YYNRULES  301
/* YYNRULES -- Number of states.  */
#define YYNSTATES  512

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
     121,   132,   114,   112,    98,   111,   126,   113,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   104,    97,
     108,   100,   109,   103,   127,     2,     2,     2,     2,     2,
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
      95,    96,    99,   101,   102,   110,   118,   119,   120,   122,
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
      85,    87,    89,    91,    93,    95,    97,    99,   101,   106,
     108,   112,   115,   118,   120,   122,   126,   129,   130,   133,
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
     442,   448,   449,   452,   454,   456,   458,   460,   462,   464,
     466,   468,   470,   471,   473,   476,   481,   485,   487,   492,
     495,   497,   499,   500,   501,   514,   516,   517,   518,   529,
     531,   535,   537,   539,   541,   545,   547,   549,   551,   554,
     555,   556,   560,   561,   563,   565,   567,   570,   573,   574,
     579,   584,   589,   592,   594,   597,   599,   601,   605,   607,
     609,   611,   613,   615,   617,   619,   621,   623,   625,   627,
     629,   631,   633,   635,   637,   639,   641,   645,   649,   653,
     657,   661,   665,   669,   673,   677,   681,   684,   687,   691,
     695,   699,   703,   707,   711,   715,   719,   723,   727,   731,
     735,   739,   743,   747,   752,   755,   757,   761,   764,   769,
     773,   774,   776,   780,   786,   790,   794,   798,   802,   806,
     810,   814,   818,   822,   826,   830,   836,   839,   842,   845,
     848,   852,   855,   860,   866,   870,   876,   880,   882,   886,
     892,   898
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     134,     0,    -1,   135,    -1,    -1,   136,    -1,   137,    -1,
     136,   137,    -1,   184,    -1,   196,    -1,   194,    -1,   212,
      -1,   205,    -1,   146,    -1,   147,   125,   135,   131,    -1,
      97,    -1,    -1,   139,    -1,   140,    -1,   139,   140,    -1,
     196,    -1,   194,    -1,   212,    -1,   205,    -1,   146,    -1,
     147,   125,   138,   131,    -1,    97,    -1,   142,    -1,    -1,
     142,   144,    -1,   144,    -1,   187,    -1,   158,    -1,   159,
      -1,   160,    -1,   162,    -1,   170,    -1,   153,    -1,   181,
      -1,   178,    -1,   234,    -1,   241,    -1,    97,    -1,   143,
      -1,   149,    -1,   164,    -1,   165,    -1,   231,    -1,   180,
      -1,   147,   125,   142,   131,    -1,   240,    -1,   125,   142,
     131,    -1,   125,   131,    -1,   144,    97,    -1,   144,    -1,
     143,    -1,     3,    87,     3,    -1,   100,   232,    -1,    -1,
      46,   150,    -1,    32,   150,    -1,   151,    -1,   150,    98,
     151,    -1,     3,   222,   148,    -1,    -1,    65,   145,    -1,
      -1,    64,   121,   154,   233,   132,   145,   152,    -1,    -1,
     149,    -1,   234,    -1,    46,     3,   222,    -1,     3,    -1,
      13,   121,    -1,    13,    50,   121,    -1,   157,   155,    97,
     233,    97,   234,   132,   145,    -1,   157,   156,    68,   233,
     132,   145,    -1,    -1,    14,   121,   161,   233,   132,   145,
      -1,    -1,    15,   163,   145,    14,   121,   233,   132,    -1,
      66,    -1,    66,     3,    -1,    30,    -1,    30,     3,    -1,
      -1,   167,    -1,   169,    -1,   167,   169,    -1,   168,    -1,
     167,   168,    -1,    34,   235,   104,   141,    -1,    62,   104,
     141,    -1,    -1,    16,   121,   171,   235,   132,   125,   166,
     131,    -1,    -1,    33,   121,     3,   222,   132,   173,   125,
     141,   131,    -1,    -1,    28,   125,   175,   141,   131,    -1,
     172,    -1,   176,   172,    -1,   176,    -1,   176,   174,    -1,
     174,    -1,    -1,    52,   125,   179,   141,   131,   177,    -1,
      37,   233,    -1,    37,    -1,    39,   121,   233,   132,   145,
      -1,     3,    -1,    19,    -1,   183,   126,   182,    -1,   182,
      -1,    -1,    19,   183,   125,   185,   138,   131,    -1,    -1,
      19,   125,   186,   138,   131,    -1,    41,   218,    -1,    41,
     183,   126,   114,    -1,    -1,   189,    -1,   190,    -1,   189,
     190,    -1,    21,    -1,    22,    -1,    20,    -1,    38,    -1,
      47,    -1,    49,    -1,    48,    -1,    26,    -1,    24,    -1,
       4,    -1,    -1,    54,   219,    -1,    -1,    54,   220,    -1,
      -1,    17,   220,    -1,    -1,   188,    31,     3,   191,   193,
     125,   195,   198,   131,    -1,    -1,   188,    44,     3,   192,
     125,   197,   201,   131,    -1,    -1,   199,    -1,   200,    -1,
     199,   200,    -1,    97,    -1,   147,   125,   198,   131,    -1,
     205,    -1,   212,    -1,   143,    -1,    -1,   202,    -1,   203,
      -1,   202,   203,    -1,    97,    -1,    46,     3,    -1,   188,
      27,   211,     3,   121,   208,   132,   222,    -1,    46,    -1,
      32,    -1,   188,   204,     3,   222,   148,    -1,    -1,   100,
     207,    -1,    10,    -1,     8,    -1,     9,    -1,    12,    -1,
       5,    -1,    56,    -1,    55,    -1,    45,    -1,     3,    -1,
      -1,   209,    -1,    91,   210,    -1,   209,    98,    91,   210,
      -1,   209,    98,   210,    -1,   210,    -1,     3,   104,   221,
     206,    -1,     3,   206,    -1,    51,    -1,    35,    -1,    -1,
      -1,   188,    27,   211,     3,   121,   208,   132,   222,   125,
     213,   141,   131,    -1,     3,    -1,    -1,    -1,    27,   214,
     121,   208,   132,   222,   125,   216,   141,   131,    -1,     3,
      -1,   183,   126,     3,    -1,   218,    -1,   217,    -1,   219,
      -1,   220,    98,   219,    -1,   219,    -1,   114,    -1,    36,
      -1,   104,   221,    -1,    -1,    -1,   121,   224,   132,    -1,
      -1,   225,    -1,   226,    -1,   232,    -1,   225,    98,    -1,
     226,   232,    -1,    -1,    25,   235,   227,   223,    -1,   235,
     121,   224,   132,    -1,    53,   121,   224,   132,    -1,    63,
     235,    -1,    42,    -1,    42,   233,    -1,   235,    -1,   235,
      -1,   233,    98,   235,    -1,   233,    -1,   215,    -1,   236,
      -1,   239,    -1,   228,    -1,   230,    -1,   229,    -1,     6,
      -1,    10,    -1,    11,    -1,     8,    -1,     9,    -1,    12,
      -1,     5,    -1,    29,    -1,    56,    -1,    55,    -1,    45,
      -1,   235,   108,   235,    -1,   235,   109,   235,    -1,   235,
      74,   235,    -1,   235,    75,   235,    -1,   235,    70,   235,
      -1,   235,    71,   235,    -1,   235,    73,   235,    -1,   235,
      72,   235,    -1,   235,    85,   235,    -1,   235,    86,   235,
      -1,   117,   235,    -1,   116,   235,    -1,   235,   107,   235,
      -1,   235,   106,   235,    -1,   235,   105,   235,    -1,   235,
      94,   235,    -1,   235,    93,   235,    -1,   235,    92,   235,
      -1,   235,   113,   235,    -1,   235,   115,   235,    -1,   235,
     112,   235,    -1,   235,   111,   235,    -1,   235,   114,   235,
      -1,   235,    68,   235,    -1,   235,    69,   235,    -1,   235,
      40,   235,    -1,   235,    67,   235,    -1,    43,   121,   235,
     132,    -1,    36,   235,    -1,    36,    -1,   121,   233,   132,
      -1,   111,   235,    -1,   235,   123,   235,   124,    -1,   123,
     224,   124,    -1,    -1,   238,    -1,   232,   104,   232,    -1,
     238,    98,   232,   104,   232,    -1,   125,   237,   131,    -1,
     235,    79,   235,    -1,   235,    78,   235,    -1,   235,    83,
     235,    -1,   235,    82,   235,    -1,   235,    84,   235,    -1,
     235,    77,   235,    -1,   235,    76,   235,    -1,   235,    80,
     235,    -1,   235,    81,   235,    -1,   235,   100,   235,    -1,
     235,   103,   235,   104,   235,    -1,   235,    89,    -1,   235,
      88,    -1,    89,   235,    -1,    88,   235,    -1,    53,   126,
       3,    -1,   127,     3,    -1,   235,   126,   127,     3,    -1,
     235,   126,     3,    87,     3,    -1,   235,    90,     3,    -1,
     235,   126,   121,   235,   132,    -1,   235,   126,     3,    -1,
       3,    -1,   188,    18,     3,    -1,   188,    18,     3,   100,
       3,    -1,   188,    18,     3,   100,     5,    -1,    23,    18,
       3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1698,  1698,  1700,  1700,  1701,  1702,  1704,  1705,  1706,
    1707,  1708,  1709,  1710,  1711,  1713,  1713,  1714,  1715,  1717,
    1718,  1719,  1720,  1721,  1722,  1723,  1725,  1726,  1728,  1729,
    1732,  1733,  1734,  1735,  1736,  1737,  1738,  1739,  1740,  1741,
    1742,  1745,  1746,  1747,  1748,  1749,  1750,  1751,  1752,  1754,
    1756,  1757,  1758,  1759,  1763,  1770,  1774,  1775,  1779,  1780,
    1782,  1783,  1785,  1828,  1829,  1832,  1832,  1851,  1852,  1853,
    1856,  1859,  1863,  1864,  1866,  1886,  1929,  1929,  1948,  1948,
    1963,  1966,  1969,  1972,  1976,  1977,  1978,  1979,  1980,  1981,
    1983,  1994,  1997,  1997,  2026,  2026,  2046,  2046,  2063,  2064,
    2065,  2066,  2074,  2083,  2083,  2128,  2132,  2143,  2152,  2153,
    2155,  2156,  2158,  2158,  2160,  2160,  2163,  2177,  2193,  2194,
    2195,  2196,  2198,  2199,  2200,  2201,  2202,  2203,  2204,  2205,
    2206,  2207,  2209,  2210,  2212,  2213,  2215,  2216,  2220,  2218,
    2226,  2224,  2232,  2233,  2234,  2235,  2236,  2237,  2238,  2239,
    2241,  2247,  2248,  2249,  2250,  2251,  2252,  2255,  2268,  2268,
    2270,  2343,  2344,  2346,  2347,  2348,  2349,  2350,  2352,  2353,
    2354,  2355,  2364,  2368,  2374,  2380,  2388,  2393,  2399,  2407,
    2415,  2416,  2417,  2420,  2419,  2434,  2435,  2437,  2436,  2458,
    2467,  2480,  2481,  2483,  2484,  2486,  2487,  2488,  2497,  2498,
    2502,  2503,  2505,  2506,  2507,  2509,  2513,  2514,  2519,  2520,
    2556,  2602,  2623,  2644,  2647,  2654,  2655,  2656,  2662,  2668,
    2670,  2672,  2674,  2676,  2678,  2680,  2697,  2702,  2705,  2708,
    2711,  2714,  2717,  2720,  2723,  2726,  2730,  2733,  2736,  2739,
    2742,  2745,  2748,  2751,  2755,  2766,  2784,  2789,  2794,  2799,
    2804,  2809,  2813,  2817,  2822,  2826,  2830,  2839,  2848,  2858,
    2863,  2875,  2881,  2886,  2892,  2898,  2902,  2904,  2915,  2924,
    2931,  2932,  2934,  2940,  2949,  2956,  2968,  2974,  2980,  2986,
    2992,  2998,  3004,  3017,  3028,  3035,  3048,  3075,  3089,  3103,
    3117,  3132,  3139,  3146,  3153,  3160,  3171,  3205,  3307,  3308,
    3309,  3311
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
  "\"==\"", "\"===\"", "\"!=\"", "\"!==\"", "\"<=\"", "\">=\"", "\"|=\"",
  "\"/=\"", "\"%=\"", "\"*=\"", "\"+=\"", "\"-=\"", "\">>=\"", "\"<<=\"",
  "\">>>=\"", "\"||\"", "\"&&\"", "\"::\"", "\"--\"", "\"++\"", "\"..\"",
  "\"...\"", "\"<<\"", "\">>>\"", "\">>\"", "prec_none", "below_semicolon",
  "';'", "','", "below_assignment", "'='", "\"&=\"", "\"^=\"", "'?'",
  "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "below_minus", "'-'", "'+'",
  "'/'", "'*'", "'%'", "'~'", "'!'", "minusminus_prefix",
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
  "IDECLARATION", "VARCONST", "SLOT_DECLARATION", "MAYBESTATICCONSTANT",
  "STATICCONSTANT", "MAYBE_PARAM_LIST", "PARAM_LIST", "PARAM", "GETSET",
  "FUNCTION_DECLARATION", "$@12", "MAYBE_IDENTIFIER", "INNERFUNCTION",
  "$@13", "CLASS", "PACKAGEANDCLASS", "CLASS_SPEC", "CLASS_SPEC_LIST",
  "TYPE", "MAYBETYPE", "MAYBE_PARAM_VALUES", "MAYBE_EXPRESSION_LIST",
  "EXPRESSION_LIST", "EXPRESSION_LIST_AND_COMMA", "XX", "NEW",
  "FUNCTIONCALL", "DELETE", "RETURN", "NONCOMMAEXPRESSION", "EXPRESSION",
  "VOIDEXPRESSION", "E", "CONSTANT", "MAYBE_EXPRPAIR_LIST",
  "EXPRPAIR_LIST", "VAR_READ", "NAMESPACE_DECLARATION", "USE_NAMESPACE", 0
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
     345,   346,   347,   348,   349,   350,   351,    59,    44,   352,
      61,   353,   354,    63,    58,   124,    94,    38,    60,    62,
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
     143,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     145,   145,   145,   145,   146,   147,   148,   148,   149,   149,
     150,   150,   151,   152,   152,   154,   153,   155,   155,   155,
     156,   156,   157,   157,   158,   159,   161,   160,   163,   162,
     164,   164,   165,   165,   166,   166,   166,   166,   167,   167,
     168,   169,   171,   170,   173,   172,   175,   174,   176,   176,
     177,   177,   177,   179,   178,   180,   180,   181,   182,   182,
     183,   183,   185,   184,   186,   184,   187,   187,   188,   188,
     189,   189,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   191,   191,   192,   192,   193,   193,   195,   194,
     197,   196,   198,   198,   199,   199,   200,   200,   200,   200,
     200,   201,   201,   202,   202,   203,   203,   203,   204,   204,
     205,   206,   206,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   208,   208,   208,   208,   209,   209,   210,   210,
     211,   211,   211,   213,   212,   214,   214,   216,   215,   217,
     218,   219,   219,   220,   220,   221,   221,   221,   222,   222,
     223,   223,   224,   224,   224,   225,   226,   225,   227,   228,
     229,   229,   230,   231,   231,   232,   233,   233,   234,   235,
     235,   235,   235,   235,   235,   235,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     237,   237,   238,   238,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   239,   240,   240,
     240,   241
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     4,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     1,     4,     1,     1,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     1,
       3,     2,     2,     1,     1,     3,     2,     0,     2,     2,
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
       5,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     2,     4,     3,     1,     4,     2,
       1,     1,     0,     0,    12,     1,     0,     0,    10,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     2,     0,
       0,     3,     0,     1,     1,     1,     2,     2,     0,     4,
       4,     4,     2,     1,     2,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     2,     1,     3,     2,     4,     3,
       0,     1,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     5,     2,     2,     2,     2,
       3,     2,     4,     5,     3,     5,     3,     1,     3,     5,
       5,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     118,   297,   131,   231,   225,   228,   229,   226,   227,   230,
       0,     0,    78,     0,     0,   124,   122,   123,     0,   130,
       0,   129,   186,   232,   265,   125,     0,     0,     0,   235,
     126,   128,   127,     0,     0,   234,   233,     0,     0,     0,
       0,    14,     0,     0,     0,     0,   202,   270,     0,     0,
       2,   118,     5,    54,    12,     0,    36,    67,    31,    32,
      33,    34,    35,    38,    37,     7,    30,     0,   119,   120,
       9,     8,    11,    10,   219,   222,   224,   223,   218,    39,
     216,   220,   221,    40,     0,     0,    72,    76,   118,    92,
     108,   109,   114,   111,     0,     0,   297,   208,   185,     0,
     264,     0,     0,   116,     0,   103,   202,     0,   212,    65,
     289,   288,   267,   247,   246,     0,     0,   203,   204,   205,
     215,     0,     0,   271,   291,     1,     6,   118,   297,     0,
       0,    68,     0,     0,    69,   182,     0,   159,     0,   158,
       0,   121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   287,   286,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   202,     0,     0,    55,    73,     0,    82,   106,
     213,     0,    80,    41,   118,    42,    53,     0,     0,    43,
      44,    45,    47,     0,    46,    49,     0,   118,   112,     0,
     301,   200,   172,     0,     0,     0,    27,     0,   290,     0,
     266,   269,   206,   207,     0,   274,     0,     0,   199,    59,
      60,   199,    58,     0,     0,   181,   180,     0,   132,   134,
     199,   217,   261,   262,   259,   260,   240,   241,   243,   242,
     238,   239,   281,   280,   276,   275,   282,   283,   278,   277,
     279,   244,   245,   294,   253,   252,   251,   284,     0,   250,
     249,   248,   236,   237,   257,   256,   254,   258,   255,     0,
       0,   296,     0,     0,     0,    83,   105,   214,    81,    51,
     118,    29,   216,    52,     0,   118,     0,     0,    25,     0,
     118,    17,    23,     0,    20,    19,    22,    21,   118,   110,
     202,   209,   161,     0,     0,   173,   177,   118,   190,   117,
     263,     0,    26,   211,     0,   272,     0,    13,     0,    57,
       0,    57,     0,     0,     0,     0,   136,     0,     0,    57,
       0,   210,   268,     0,     0,   292,   118,    50,    28,     0,
     118,   298,     0,   115,    18,   118,     0,     0,     0,     0,
     179,   174,   199,     0,   107,     0,   118,     0,   189,   197,
     196,     0,   192,   191,   195,   198,     0,    62,    61,     0,
     118,   172,   133,     0,     0,   193,   135,   140,   160,   285,
     293,   295,    77,     0,    48,     0,    84,     0,   113,   201,
     171,   167,   164,   165,   163,   166,   170,   169,   168,   162,
     161,     0,     0,   176,     0,     0,    98,   102,   100,   104,
      63,   273,     0,    56,     0,    75,     0,   137,   138,     0,
     118,    79,   299,   300,     0,     0,     0,    85,    88,    86,
      24,   178,   187,   175,    96,     0,    99,   101,   118,    66,
     118,   199,   118,   194,     0,   155,     0,     0,   118,   153,
       0,    27,    93,    89,    87,    27,    27,   199,    64,    74,
       0,   146,   150,     0,     0,     0,   118,   144,   148,   149,
     156,   182,   141,   154,    27,    91,     0,     0,     0,   183,
     118,   139,   145,     0,    90,   188,    97,    94,    27,     0,
       0,     0,     0,   147,   172,    27,   184,     0,     0,   199,
      95,   157
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    49,    50,    51,    52,   299,   300,   301,   321,   322,
     195,   291,   197,   302,   198,   377,   199,   232,   230,   449,
      56,   219,   132,   133,    57,    58,    59,    60,   187,    61,
      88,   200,   201,   436,   437,   438,   439,    62,   206,   416,
     501,   417,   466,   418,   419,    63,   216,   202,    64,    93,
     371,    65,   308,   207,    66,   203,    68,    69,   336,   338,
     384,   304,   452,   305,   430,   475,   476,   477,   457,   458,
     459,   140,   306,   360,   409,   314,   315,   316,   237,   307,
     498,    99,    74,   465,   372,   373,   374,   386,   375,   329,
     311,   116,   117,   118,   211,    75,    76,    77,   204,   119,
      78,    79,    80,    81,   122,   123,    82,   205,    83
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -348
static const yytype_int16 yypact[] =
{
    1656,   -44,  -348,  -348,  -348,  -348,  -348,  -348,  -348,  -348,
     -31,   -41,  -348,   -30,     7,  -348,  -348,  -348,    82,  -348,
    2221,  -348,   100,  -348,   336,  -348,    -1,    19,    13,  -348,
    -348,  -348,  -348,    -2,   -77,  -348,  -348,  2221,    38,  2221,
    2221,  -348,  2221,  2221,  2221,  2221,  2221,  2221,   179,   196,
    -348,   495,  -348,  -348,  -348,    78,  -348,  2128,  -348,  -348,
    -348,  -348,  -348,  -348,  -348,  -348,  -348,   166,   240,  -348,
    -348,  -348,  -348,  -348,  -348,  -348,  -348,  -348,   115,  -348,
    2736,  -348,  -348,  -348,   213,   102,  -348,  -348,  1906,  -348,
    -348,  -348,  -348,  -348,  -102,   222,  -348,    -4,  -348,   109,
     -25,  2221,   110,  -348,  2221,  -348,  2221,   237,   -25,  -348,
     -25,   -25,   305,   -25,   -25,   -56,   118,   145,  2221,  -348,
    2736,   141,   116,   150,  -348,  -348,  -348,  1011,   181,   247,
     248,  -348,   155,   190,  -348,   -10,   253,  -348,   260,  -348,
     262,  -348,  2221,  2221,  2221,  2221,  2221,  2221,  2221,  2221,
    2221,  2221,  2221,  2221,  2221,  2221,  2221,  2221,  2221,  2221,
    2221,  2221,  2221,  2221,  -348,  -348,   266,  2221,  2221,  2221,
    2221,  2221,  2221,  2221,  2221,  2221,  2221,  2221,  2221,  2221,
    2221,  2221,  2221,  2221,    64,  -348,  -348,  2221,   273,  2221,
    2221,   247,   281,  -348,   624,  -348,   189,   271,   167,  -348,
    -348,  -348,  -348,   275,  -348,  -348,  2221,  1140,  -348,    19,
    -348,   173,    24,   -38,    66,  2282,  1781,   163,  -348,  2221,
    -348,  -348,  -348,  -348,  2221,  -348,  2221,   165,   193,   200,
    -348,   193,   200,  2221,  2221,  -348,  -348,   296,   252,   259,
     193,  2736,   998,   487,   487,   487,  3097,  3097,  3097,  3097,
     998,   998,  2736,  2736,  2736,  2736,  2736,  2736,  2736,  2736,
    2736,  2797,  2857,  -348,   287,   287,   287,  2736,  2492,  2917,
    2977,  3037,   998,   998,   305,   305,   -25,   -25,   -25,   175,
    2553,   227,  2221,   312,   -28,  -348,   115,   115,  -348,  -348,
     753,  -348,  2614,  -348,   195,  2031,   314,  2352,  -348,   187,
    1269,  -348,  -348,   194,  -348,  -348,  -348,  -348,  1140,  -348,
    2221,  -348,   131,   317,   191,   223,  -348,  1906,   198,  -348,
    -348,   197,  1781,  -348,   -27,  -348,   218,  -348,    12,   226,
     247,   -39,   135,   -14,   208,    89,   315,    89,   209,   226,
    2221,  -348,  -348,   332,  2422,  -348,  1906,  -348,  -348,  2221,
     882,   236,   212,  -348,  -348,  1140,   207,   211,    69,    12,
    -348,  -348,   193,    25,  -348,    60,  1906,  2221,   198,  -348,
    -348,   224,  -348,  -348,  -348,  -348,  2221,  -348,  -348,  2221,
    1906,    24,  -348,    89,   228,  -348,   251,  -348,  -348,  2736,
    -348,  -348,  -348,   -11,  -348,   178,    11,   220,  -348,  -348,
    -348,  -348,  -348,  -348,  -348,  -348,  -348,  -348,  -348,  -348,
     258,   234,   317,  -348,   241,   231,  -348,  -348,    60,  -348,
     297,  -348,   202,  -348,   235,  -348,   238,   251,  -348,    89,
     180,  -348,  -348,  -348,  2221,   264,   242,    11,  -348,  -348,
    -348,  -348,  -348,  -348,  -348,   368,  -348,  -348,  1906,  -348,
    1906,   193,  1398,  -348,   375,  -348,   352,   249,   233,  -348,
    2675,  1781,  -348,  -348,  -348,  1781,  1781,   193,  -348,  -348,
     257,  -348,  -348,   261,   188,   254,  1527,  -348,  -348,  -348,
    -348,   -10,  -348,  -348,  1781,  -348,   256,   265,   272,  -348,
    1398,  -348,  -348,   380,  -348,  -348,  -348,  -348,  1781,   274,
     263,   278,   276,  -348,    24,  1781,  -348,   277,   280,   193,
    -348,  -348
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -348,  -348,   270,  -348,   337,  -287,  -348,    90,  -193,  -178,
       1,   -76,  -271,     6,     2,    67,   355,   285,    85,  -348,
    -348,  -348,  -348,  -348,  -348,  -348,  -348,  -348,  -348,  -348,
    -348,  -348,  -348,  -348,  -348,   -21,   -20,  -348,  -348,    14,
    -348,    15,  -348,  -348,  -348,  -348,  -348,  -348,  -348,  -200,
      75,  -348,  -348,  -348,  -348,     0,  -348,   353,  -348,  -348,
    -348,    59,  -348,    62,  -348,   -68,  -348,   -53,  -348,  -348,
     -29,  -348,     3,    26,  -348,  -347,  -348,  -305,   -47,     4,
    -348,  -348,  -348,  -348,  -348,   408,  -317,    54,    79,  -184,
    -348,   -99,  -348,  -348,  -348,  -348,  -348,  -348,  -348,   -36,
       5,   -52,    -7,  -348,  -348,  -348,  -348,  -348,  -348
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -216
static const yytype_int16 yytable[] =
{
      67,    53,    55,    72,    73,   134,    54,   217,   361,   309,
      90,   121,   196,    97,   309,   368,   290,   100,   382,    85,
     385,   356,    90,   208,   209,   235,    91,   312,   312,   -70,
     108,    91,   110,   111,   426,   112,   113,   114,    91,   120,
     120,   236,   142,    84,   106,   434,   364,   331,   369,   107,
     115,    67,    53,    55,    72,    73,   339,    54,   413,    70,
     142,   376,    71,   164,   165,   166,   385,   281,   397,   318,
     142,   142,   400,   435,   401,   392,   220,   402,   403,   404,
      87,   405,   223,   279,   142,    91,   166,   142,   414,    94,
      86,    89,   368,   415,   317,   420,   182,   215,   183,   120,
      95,   184,   102,    98,   346,   366,   213,   443,    91,   425,
      70,   120,   453,    71,   406,   313,   412,   350,   380,   183,
     101,   431,   184,   105,   407,   408,   370,    67,    53,    55,
      72,    73,    92,    54,   104,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   507,   121,   109,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   120,   280,   468,   411,   469,
     319,   432,   124,   433,     2,   282,    70,   292,   325,    71,
     326,   283,   284,   135,   286,   287,   125,   136,   137,   297,
      15,    16,    17,   127,    19,   318,    21,    67,    53,   303,
     138,   357,   139,   142,   348,   135,   185,   120,    25,   120,
     137,    91,   309,   186,   324,   210,   454,    30,    31,    32,
     212,   358,   379,   142,   139,   359,   214,     2,   332,   333,
     218,   196,   221,   222,     2,   224,   348,   225,   226,   -71,
     228,   231,   233,    15,    16,    17,   238,    19,   234,    21,
      15,    16,    17,   239,    19,   240,    21,   470,   485,   263,
     196,    25,   486,   487,   348,   344,   285,   455,    25,   454,
      30,    31,    32,   488,   288,   294,   293,    30,    31,    32,
     196,   494,   295,   296,   310,   323,   327,   328,   330,   334,
      67,    53,   303,   120,   196,   502,   335,   341,    67,    53,
     303,  -151,   508,   337,   343,   345,   349,   351,   353,   355,
     312,   363,   367,   362,  -108,   511,   376,   424,   365,   381,
     455,   421,   383,   389,   387,   390,   395,   396,   398,    96,
     423,     3,     4,   399,     5,     6,     7,     8,     9,   429,
     422,   440,   445,   428,   393,    67,    53,   303,   358,   442,
     120,    20,   448,    22,  -152,    23,   444,   450,   461,   120,
     451,   467,   196,   462,   196,   164,   165,   166,   480,   481,
     482,    29,   489,   500,   504,   491,   490,   495,   126,    34,
     354,    35,    36,   164,   165,   166,   496,   227,   177,   178,
     179,   180,   181,   505,   497,   503,   388,   506,   182,   509,
     183,   510,   131,   184,   229,   378,   463,   464,   179,   180,
     181,   141,   499,   492,    39,    40,   182,   460,   183,   483,
     456,   184,   446,   447,   493,   103,   441,   427,   410,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,   472,   473,   478,   479,    45,   456,    46,
       0,    47,     0,    48,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   472,   473,   478,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   472,   473,   478,   479,    -4,     0,     0,     1,     2,
       3,     4,     0,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,    14,    15,    16,    17,    18,    19,
      20,    21,     0,     0,    23,     0,     0,   143,     0,     0,
       0,    24,     0,    25,    26,     0,    27,     0,    28,     0,
      29,     0,    30,    31,    32,     0,     0,    33,    34,     0,
      35,    36,     0,     0,  -216,  -216,  -216,     0,    37,    38,
       0,   151,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,   165,   166,     0,   167,
     168,   169,     0,    39,    40,     0,     0,     0,     0,     0,
       0,     0,    41,     0,     0,   175,   176,     0,   177,   178,
     179,   180,   181,     0,     0,     0,    42,     0,   182,     0,
     183,    43,    44,   184,     0,     0,    45,     0,    46,     0,
      47,     0,    48,     0,     0,     0,    -4,     1,     2,     3,
       4,     0,     5,     6,     7,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    15,    16,    17,    18,    19,    20,
      21,    22,     0,    23,   188,     0,   129,     0,     0,     0,
      24,   189,    25,    26,     0,    27,   190,    28,     0,    29,
     191,    30,    31,    32,     0,     0,    33,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    37,    38,     0,
     192,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,    40,     0,     0,     0,     0,     0,     0,
       0,   193,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
      43,    44,     0,     0,     0,    45,     0,    46,     0,    47,
       0,    48,     0,     0,     0,   289,     1,     2,     3,     4,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    15,    16,    17,    18,    19,    20,    21,
      22,     0,    23,   188,     0,   129,     0,     0,     0,    24,
     189,    25,    26,     0,    27,   190,    28,     0,    29,   191,
      30,    31,    32,     0,     0,    33,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    37,    38,     0,   192,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    39,    40,     0,     0,     0,     0,     0,     0,     0,
     193,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,    43,
      44,     0,     0,     0,    45,     0,    46,     0,    47,     0,
      48,     0,     0,     0,   347,     1,     2,     3,     4,     0,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    15,    16,    17,    18,    19,    20,    21,    22,
       0,    23,   188,     0,   129,     0,     0,     0,    24,   189,
      25,    26,     0,    27,   190,    28,     0,    29,   191,    30,
      31,    32,     0,     0,    33,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    37,    38,     0,   192,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,     0,     0,     0,     0,     0,   193,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,     0,    43,    44,
       0,     0,     0,    45,     0,    46,     0,    47,     0,    48,
       0,     0,     0,   394,     1,     2,     3,     4,     0,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
      14,    15,    16,    17,    18,    19,    20,    21,  -216,     0,
      23,     0,     0,     0,     0,     0,     0,    24,     0,    25,
      26,     0,    27,     0,    28,     0,    29,     0,    30,    31,
      32,     0,     0,    33,    34,     0,    35,    36,     0,     0,
       0,     0,  -216,  -216,    37,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   164,   165,   166,     0,
     167,   168,   169,     0,     0,     0,     0,     0,     0,    39,
      40,     0,     0,     0,     0,     0,  -216,  -216,    41,   177,
     178,   179,   180,   181,     0,     0,     0,     0,     0,   182,
       0,   183,    42,     0,   184,     0,     0,    43,    44,     0,
       0,     0,    45,     0,    46,     0,    47,     0,    48,     0,
       0,     0,    -3,     1,     2,     3,     4,     0,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      15,    16,    17,    18,    19,    20,    21,     0,     0,    23,
       0,     0,     0,     0,     0,     0,    24,     0,    25,    26,
       0,    27,     0,    28,     0,    29,     0,    30,    31,    32,
       0,     0,    33,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    37,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,     0,    43,    44,     0,     0,
       0,    45,     0,    46,     0,    47,     0,    48,     0,     0,
       0,   -15,     1,     2,     3,     4,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    15,
      16,    17,    18,    19,    20,    21,     0,     0,    23,     0,
       0,     0,     0,     0,     0,    24,     0,    25,    26,     0,
      27,     0,    28,     0,    29,     0,    30,    31,    32,     0,
       0,    33,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    37,    38,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    39,    40,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      42,     0,     0,     0,     0,    43,    44,     0,     0,     0,
      45,     0,    46,     0,    47,     0,    48,     0,     0,     0,
     -16,     1,     2,     3,     4,     0,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    15,    16,
      17,    18,    19,    20,    21,     0,     0,    23,     0,     0,
       0,     0,     0,     0,    24,     0,    25,    26,     0,    27,
       0,    28,     0,    29,     0,    30,    31,    32,     0,     0,
      33,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    39,    40,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,    43,    44,     0,     0,     0,    45,
       0,    46,     0,    47,     0,    48,     0,     0,     0,  -142,
       1,     2,     3,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    23,     0,     0,     0,
       0,     0,     0,    24,     0,    25,    26,     0,    27,     0,
      28,     0,    29,     0,    30,    31,    32,     0,     0,    33,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    39,    40,     0,     0,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,    43,    44,     0,     0,     0,    45,     0,
      46,     0,    47,     0,    48,     0,    -3,     0,  -143,     1,
       2,     3,     4,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,    14,    15,    16,    17,    18,
      19,    20,    21,     0,     0,    23,     0,     0,     0,     0,
       0,     0,    24,     0,    25,    26,     0,    27,     0,    28,
       0,    29,     0,    30,    31,    32,     0,     0,    33,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    37,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
       0,     0,    43,    44,     0,     0,     0,    45,     0,    46,
       0,    47,     0,    48,     1,     2,     3,     4,     0,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,  -118,
       0,    15,    16,    17,    18,    19,    20,    21,    22,     0,
      23,   188,     0,   129,     0,     0,     0,    24,   189,    25,
      26,     0,    27,   190,    28,     0,    29,   191,    30,    31,
      32,     0,     0,    33,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    37,    38,     0,   192,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    39,
      40,     0,     0,     0,     0,     0,     0,     0,   193,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    42,     0,     0,     0,     0,    43,    44,     0,
       0,     0,    45,     0,    46,     0,    47,     0,    48,     1,
       2,     3,     4,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    15,    16,    17,    18,
      19,    20,    21,    22,     0,    23,   188,     0,   129,     0,
       0,     0,    24,   189,    25,    26,     0,    27,   190,    28,
       0,    29,   191,    30,    31,    32,     0,     0,    33,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    37,
      38,     0,   192,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,     0,     0,
       0,     0,     0,   193,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
       0,     0,    43,    44,     0,     0,     0,    45,     0,    46,
       0,   194,     0,    48,     1,     2,     3,     4,     0,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    15,    16,    17,    18,    19,    20,    21,    22,     0,
      23,   188,     0,   129,     0,     0,     0,    24,   189,    25,
      26,     0,    27,   190,    28,     0,    29,   191,    30,    31,
      32,     0,     0,    33,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    37,    38,     0,   192,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    39,
      40,     0,     0,     0,     0,     0,     0,     0,   193,     0,
       0,   128,     0,     3,     4,     0,     5,     6,     7,     8,
       9,     0,    42,     0,     0,     0,     0,    43,    44,     0,
       0,     0,    45,    20,    46,    22,    47,    23,    48,     0,
     129,     0,     0,     0,    24,     0,     0,     0,     0,     0,
       0,    28,     0,    29,   130,     0,     0,     0,     0,     0,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    96,     0,     3,     4,     0,     5,
       6,     7,     8,     9,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,    43,    44,    20,     0,    22,    45,
      23,    46,     0,    47,     0,    48,     0,    24,     0,     0,
       0,     0,     0,     0,    28,     0,    29,     0,     0,     0,
       0,     0,     0,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    39,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    42,     0,     0,     0,     0,    43,    44,     0,
       0,     0,    45,     0,    46,     0,    47,     0,    48,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     164,   165,   166,     0,   167,   168,   169,     0,     0,     0,
       0,     0,   170,     0,     0,   171,     0,   172,   173,   174,
     175,   176,   143,   177,   178,   179,   180,   181,     0,     0,
       0,     0,     0,   182,     0,   183,     0,     0,   184,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     164,   165,   166,     0,   167,   168,   169,     0,     0,     0,
       0,     0,   170,     0,     0,   171,     0,   172,   173,   174,
     175,   176,   143,   177,   178,   179,   180,   181,     0,     0,
       0,     0,     0,   182,     0,   183,     0,     0,   184,     0,
       0,     0,     0,     0,   352,     0,     0,     0,     0,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     164,   165,   166,     0,   167,   168,   169,     0,     0,     0,
       0,     0,   170,     0,     0,   171,     0,   172,   173,   174,
     175,   176,   143,   177,   178,   179,   180,   181,     0,     0,
       0,     0,     0,   182,     0,   183,     0,     0,   184,     0,
       0,     0,     0,     0,   391,     0,     0,     0,     0,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     164,   165,   166,     0,   167,   168,   169,     0,     0,     0,
       0,     0,   170,   143,     0,   171,   340,   172,   173,   174,
     175,   176,     0,   177,   178,   179,   180,   181,     0,     0,
       0,     0,     0,   182,     0,   183,     0,     0,   184,     0,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,   166,     0,   167,   168,   169,     0,     0,
       0,     0,     0,   170,   143,     0,   171,     0,   172,   173,
     174,   175,   176,     0,   177,   178,   179,   180,   181,     0,
       0,     0,     0,     0,   182,     0,   183,   342,     0,   184,
       0,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   164,   165,   166,     0,   167,   168,   169,     0,
       0,     0,     0,     0,   170,   143,     0,   171,  -215,   172,
     173,   174,   175,   176,     0,   177,   178,   179,   180,   181,
       0,     0,     0,     0,     0,   182,     0,   183,     0,     0,
     184,     0,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   164,   165,   166,     0,   167,   168,   169,
       0,     0,     0,     0,     0,   170,   143,     0,   171,   484,
     172,   173,   174,   175,   176,     0,   177,   178,   179,   180,
     181,     0,     0,     0,     0,     0,   182,     0,   183,     0,
       0,   184,     0,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   164,   165,   166,     0,   167,   168,
     169,     0,     0,     0,     0,     0,   170,   143,     0,   171,
       0,   172,   173,   174,   175,   176,     0,   177,   178,   179,
     180,   181,     0,     0,     0,     0,     0,   182,     0,   183,
       0,     0,   184,     0,   144,   145,   146,   147,   148,   149,
     150,   151,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   163,     0,   164,   165,   166,     0,   167,
     168,   169,     0,     0,     0,     0,     0,   143,     0,     0,
       0,     0,   172,   173,   174,   175,   176,     0,   177,   178,
     179,   180,   181,     0,     0,     0,     0,     0,   182,     0,
     183,     0,     0,   184,   144,   145,   146,   147,   148,   149,
     150,   151,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,   165,   166,     0,   167,
     168,   169,     0,     0,     0,     0,     0,   143,     0,     0,
       0,     0,   172,   173,   174,   175,   176,     0,   177,   178,
     179,   180,   181,     0,     0,     0,     0,     0,   182,     0,
     183,     0,     0,   184,   144,   145,   146,   147,   148,   149,
     150,   151,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,   165,   166,     0,   167,
     168,   169,     0,     0,     0,     0,     0,   143,     0,     0,
       0,     0,     0,   173,   174,   175,   176,     0,   177,   178,
     179,   180,   181,     0,     0,     0,     0,     0,   182,     0,
     183,     0,     0,   184,   144,   145,   146,   147,   148,   149,
     150,   151,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,   165,   166,     0,   167,
     168,   169,     0,     0,     0,     0,     0,   143,     0,     0,
       0,     0,     0,     0,   174,   175,   176,     0,   177,   178,
     179,   180,   181,     0,     0,     0,     0,     0,   182,     0,
     183,     0,     0,   184,   144,   145,   146,   147,   148,   149,
     150,   151,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,   165,   166,     0,   167,
     168,   169,     0,     0,     0,     0,     0,   143,     0,     0,
       0,     0,     0,     0,  -216,   175,   176,     0,   177,   178,
     179,   180,   181,     0,     0,     0,     0,     0,   182,     0,
     183,     0,     0,   184,   144,   145,   146,  -216,  -216,  -216,
    -216,   151,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,   165,   166,     0,   167,
     168,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,   176,     0,   177,   178,
     179,   180,   181,     0,     0,     0,     0,     0,   182,     0,
     183,     0,     0,   184
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,     0,    57,     0,   106,   313,   209,
       3,    47,    88,    20,   214,     3,   194,    24,   335,    50,
     337,   308,     3,   125,   126,    35,    19,     3,     3,    68,
      37,    19,    39,    40,   381,    42,    43,    44,    19,    46,
      47,    51,    98,    87,   121,    34,   317,   231,    36,   126,
      45,    51,    51,    51,    51,    51,   240,    51,   363,     0,
      98,   100,     0,    88,    89,    90,   383,     3,   355,     3,
      98,    98,     3,    62,     5,   346,   132,     8,     9,    10,
     121,    12,   118,   182,    98,    19,    90,    98,    28,    14,
     121,   121,     3,    33,   132,   366,   121,   104,   123,   106,
      18,   126,    27,     3,   132,   132,   101,   412,    19,   380,
      51,   118,   429,    51,    45,    91,    91,   295,   132,   123,
     121,   132,   126,   125,    55,    56,   114,   127,   127,   127,
     127,   127,   125,   127,   121,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   504,   194,   121,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   448,   362,   450,
     114,     3,     3,     5,     4,   121,   127,   194,   224,   127,
     226,   127,   187,    27,   189,   190,     0,    31,    32,   206,
      20,    21,    22,   125,    24,     3,    26,   207,   207,   207,
      44,   310,    46,    98,   290,    27,     3,   224,    38,   226,
      32,    19,   422,   121,   219,     3,    46,    47,    48,    49,
     121,   100,    97,    98,    46,   104,   126,     4,   233,   234,
       3,   317,   124,    98,     4,   104,   322,   131,    98,    68,
       3,     3,    97,    20,    21,    22,     3,    24,    68,    26,
      20,    21,    22,     3,    24,     3,    26,   451,   461,     3,
     346,    38,   465,   466,   350,   282,     3,    97,    38,    46,
      47,    48,    49,   467,     3,    14,    97,    47,    48,    49,
     366,   484,   125,    18,   121,   132,   131,   104,    98,     3,
     300,   300,   300,   310,   380,   498,    54,   132,   308,   308,
     308,   131,   505,    54,    87,     3,   121,     3,   131,   125,
       3,    98,   104,   132,   126,   509,   100,   379,   131,   121,
      97,   367,    17,   340,   125,     3,   100,   125,   131,     3,
     376,     5,     6,   132,     8,     9,    10,    11,    12,    98,
     126,   131,   121,   125,   349,   355,   355,   355,   100,   125,
     367,    25,    65,    27,   131,    29,   125,   132,   104,   376,
     132,     3,   448,   131,   450,    88,    89,    90,     3,    27,
     131,    45,   125,     3,   121,   131,   125,   131,    51,    53,
     300,    55,    56,    88,    89,    90,   131,   127,   111,   112,
     113,   114,   115,   125,   132,   131,   339,   131,   121,   132,
     123,   131,    57,   126,   129,   330,   437,   437,   113,   114,
     115,    68,   490,   476,    88,    89,   121,   434,   123,   458,
     430,   126,   418,   418,   481,    27,   410,   383,   359,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   452,   452,   452,   452,   452,   121,   458,   123,
      -1,   125,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   476,   476,   476,
     476,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     490,   490,   490,   490,   490,     0,    -1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    29,    -1,    -1,    40,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    41,    -1,    43,    -1,
      45,    -1,    47,    48,    49,    -1,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    67,    68,    69,    -1,    63,    64,
      -1,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    92,
      93,    94,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,    -1,   111,    -1,   121,    -1,
     123,   116,   117,   126,    -1,    -1,   121,    -1,   123,    -1,
     125,    -1,   127,    -1,    -1,    -1,   131,     3,     4,     5,
       6,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    -1,    -1,
      36,    37,    38,    39,    -1,    41,    42,    43,    -1,    45,
      46,    47,    48,    49,    -1,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,
      -1,   127,    -1,    -1,    -1,   131,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    -1,    -1,    36,
      37,    38,    39,    -1,    41,    42,    43,    -1,    45,    46,
      47,    48,    49,    -1,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,    -1,
     127,    -1,    -1,    -1,   131,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    -1,    -1,    36,    37,
      38,    39,    -1,    41,    42,    43,    -1,    45,    46,    47,
      48,    49,    -1,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,    -1,   125,    -1,   127,
      -1,    -1,    -1,   131,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    40,    -1,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    -1,    41,    -1,    43,    -1,    45,    -1,    47,    48,
      49,    -1,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    74,    75,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    -1,    -1,    -1,    -1,    -1,   108,   109,    97,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,   111,    -1,   126,    -1,    -1,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,    -1,   125,    -1,   127,    -1,
      -1,    -1,   131,     3,     4,     5,     6,    -1,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    41,    -1,    43,    -1,    45,    -1,    47,    48,    49,
      -1,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,    -1,   127,    -1,    -1,
      -1,   131,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      41,    -1,    43,    -1,    45,    -1,    47,    48,    49,    -1,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,    -1,   125,    -1,   127,    -1,    -1,    -1,
     131,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,
      -1,    43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,
      52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,    -1,   127,    -1,    -1,    -1,   131,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,
      43,    -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,    -1,   125,    -1,   127,    -1,     0,    -1,   131,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    41,    -1,    43,
      -1,    45,    -1,    47,    48,    49,    -1,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,    -1,   127,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    -1,    -1,    36,    37,    38,
      39,    -1,    41,    42,    43,    -1,    45,    46,    47,    48,
      49,    -1,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,    -1,   125,    -1,   127,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    -1,    -1,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      -1,    -1,    36,    37,    38,    39,    -1,    41,    42,    43,
      -1,    45,    46,    47,    48,    49,    -1,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,    -1,   127,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    -1,    -1,    36,    37,    38,
      39,    -1,    41,    42,    43,    -1,    45,    46,    47,    48,
      49,    -1,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,     3,    -1,     5,     6,    -1,     8,     9,    10,    11,
      12,    -1,   111,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,   121,    25,   123,    27,   125,    29,   127,    -1,
      32,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,     5,     6,    -1,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,   116,   117,    25,    -1,    27,   121,
      29,   123,    -1,   125,    -1,   127,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,    -1,   125,    -1,   127,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,   105,   106,   107,
     108,   109,    40,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,   105,   106,   107,
     108,   109,    40,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,   105,   106,   107,
     108,   109,    40,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,    40,    -1,   103,   104,   105,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,   100,    40,    -1,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,   126,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,   100,    40,    -1,   103,   104,   105,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,    -1,
     126,    -1,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,   100,    40,    -1,   103,   104,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,
      -1,   126,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,   100,    40,    -1,   103,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,    -1,   126,    -1,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    29,    36,    38,    39,    41,    43,    45,
      47,    48,    49,    52,    53,    55,    56,    63,    64,    88,
      89,    97,   111,   116,   117,   121,   123,   125,   127,   134,
     135,   136,   137,   143,   146,   147,   153,   157,   158,   159,
     160,   162,   170,   178,   181,   184,   187,   188,   189,   190,
     194,   196,   205,   212,   215,   228,   229,   230,   233,   234,
     235,   236,   239,   241,    87,    50,   121,   121,   163,   121,
       3,    19,   125,   182,   183,    18,     3,   235,     3,   214,
     235,   121,   183,   218,   121,   125,   121,   126,   235,   121,
     235,   235,   235,   235,   235,   233,   224,   225,   226,   232,
     235,   232,   237,   238,     3,     0,   137,   125,     3,    32,
      46,   149,   155,   156,   234,    27,    31,    32,    44,    46,
     204,   190,    98,    40,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    88,    89,    90,    92,    93,    94,
     100,   103,   105,   106,   107,   108,   109,   111,   112,   113,
     114,   115,   121,   123,   126,     3,   121,   161,    30,    37,
      42,    46,    66,    97,   125,   143,   144,   145,   147,   149,
     164,   165,   180,   188,   231,   240,   171,   186,   125,   126,
       3,   227,   121,   233,   126,   235,   179,   224,     3,   154,
     132,   124,    98,   232,   104,   131,    98,   135,     3,   150,
     151,     3,   150,    97,    68,    35,    51,   211,     3,     3,
       3,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,     3,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   224,
     235,     3,   121,   127,   233,     3,   233,   233,     3,   131,
     142,   144,   235,    97,    14,   125,    18,   235,    97,   138,
     139,   140,   146,   147,   194,   196,   205,   212,   185,   182,
     121,   223,     3,    91,   208,   209,   210,   132,     3,   114,
     132,   141,   142,   132,   233,   232,   232,   131,   104,   222,
      98,   222,   233,   233,     3,    54,   191,    54,   192,   222,
     104,   132,   124,    87,   235,     3,   132,   131,   144,   121,
     142,     3,   132,   131,   140,   125,   138,   224,   100,   104,
     206,   210,   132,    98,   145,   131,   132,   104,     3,    36,
     114,   183,   217,   218,   219,   221,   100,   148,   151,    97,
     132,   121,   219,    17,   193,   219,   220,   125,   148,   235,
       3,   132,   145,   233,   131,   100,   125,   138,   131,   132,
       3,     5,     8,     9,    10,    12,    45,    55,    56,   207,
     221,   222,    91,   210,    28,    33,   172,   174,   176,   177,
     145,   232,   126,   232,   234,   145,   208,   220,   125,    98,
     197,   132,     3,     5,    34,    62,   166,   167,   168,   169,
     131,   206,   125,   210,   125,   121,   172,   174,    65,   152,
     132,   132,   195,   219,    46,    97,   188,   201,   202,   203,
     235,   104,   131,   168,   169,   216,   175,     3,   145,   145,
     222,    97,   143,   147,   188,   198,   199,   200,   205,   212,
       3,    27,   131,   203,   104,   141,   141,   141,   222,   125,
     125,   131,   200,   211,   141,   131,   131,   132,   213,   198,
       3,   173,   141,   131,   121,   125,   131,   208,   141,   132,
     131,   222
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
#line 1725 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1726 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1728 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 29:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1729 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 41:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1745 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 48:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1752 "parser.y"
    {(yyval.code)=(yyvsp[(3) - (4)].code);}
    }
    break;


  
    case 49:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1754 "parser.y"
    {/*TODO*/(yyval.code)=0;}
    }
    break;


  
    case 50:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1756 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 51:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1757 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 52:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1758 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1759 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1763 "parser.y"
    {
    code_t**cc = &global->init->method->body->code;
    *cc = code_append(*cc, (yyvsp[(1) - (1)].code));
}
    }
    break;


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1774 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (2)].value);}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1775 "parser.y"
    {(yyval.value).c=abc_pushundefined(0);
                                  (yyval.value).t=TYPE_ANY;
                                 }
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1779 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1780 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1782 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1783 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1786 "parser.y"
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
#line 1828 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1829 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1832 "parser.y"
    {new_state();}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1832 "parser.y"
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
#line 1851 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1856 "parser.y"
    {
    (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1);
}
    }
    break;


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1859 "parser.y"
    {
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 72:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1863 "parser.y"
    {new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 73:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1864 "parser.y"
    {new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1866 "parser.y"
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
#line 1886 "parser.y"
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
#line 1929 "parser.y"
    {new_state();}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1929 "parser.y"
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
#line 1948 "parser.y"
    {new_state();}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1948 "parser.y"
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
#line 1963 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1966 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1969 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1972 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1976 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1977 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1978 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1979 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1980 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1981 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1983 "parser.y"
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
#line 1994 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1997 "parser.y"
    {new_state();}
    }
    break;


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1997 "parser.y"
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
#line 2026 "parser.y"
    {new_state();state->exception_name=(yyvsp[(3) - (5)].id);new_variable((yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0);}
    }
    break;


  
    case 95:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2027 "parser.y"
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
#line 2046 "parser.y"
    {new_state();state->exception_name=0;}
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2046 "parser.y"
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
#line 2063 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2064 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2065 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2066 "parser.y"
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
#line 2074 "parser.y"
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
#line 2083 "parser.y"
    {new_state();}
    }
    break;


  
    case 104:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2083 "parser.y"
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
#line 2128 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2132 "parser.y"
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
#line 2143 "parser.y"
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
#line 2153 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 110:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2155 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 111:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2156 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2158 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2159 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2160 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2161 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2163 "parser.y"
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
#line 2177 "parser.y"
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
#line 2193 "parser.y"
    {PASS12 (yyval.flags)=0;}
    }
    break;


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2194 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2195 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].token);}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2196 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (2)].flags)|(yyvsp[(2) - (2)].token);}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2198 "parser.y"
    {PASS12 (yyval.token)=FLAG_PUBLIC;}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2199 "parser.y"
    {PASS12 (yyval.token)=FLAG_PRIVATE;}
    }
    break;


  
    case 124:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2200 "parser.y"
    {PASS12 (yyval.token)=FLAG_PROTECTED;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2201 "parser.y"
    {PASS12 (yyval.token)=FLAG_STATIC;}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2202 "parser.y"
    {PASS12 (yyval.token)=FLAG_DYNAMIC;}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2203 "parser.y"
    {PASS12 (yyval.token)=FLAG_FINAL;}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2204 "parser.y"
    {PASS12 (yyval.token)=FLAG_OVERRIDE;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2205 "parser.y"
    {PASS12 (yyval.token)=FLAG_NATIVE;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2206 "parser.y"
    {PASS12 (yyval.token)=FLAG_PACKAGEINTERNAL;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2207 "parser.y"
    {PASS12 (yyval.token)=FLAG_NAMESPACE;}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2209 "parser.y"
    {(yyval.classinfo)=registry_getobjectclass();}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2210 "parser.y"
    {(yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2212 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2213 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2215 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2216 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2220 "parser.y"
    {PASS12 startclass((yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2222 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2226 "parser.y"
    {PASS12 startclass((yyvsp[(1) - (5)].flags)|FLAG_INTERFACE,(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2228 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 150:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2241 "parser.y"
    {
    code_t*c = state->cls->static_init;
    c = code_append(c, (yyvsp[(1) - (1)].code));  
    state->cls->static_init = c;
}
    }
    break;


  
    case 156:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2252 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 157:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2255 "parser.y"
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
#line 2270 "parser.y"
    {
    int flags = (yyvsp[(1) - (5)].flags);
    U8 access = flags2access((yyvsp[(1) - (5)].flags));

    varinfo_t* info = 0;
    if(state->cls) {
        memberinfo_t*i = registry_findmember(state->cls->info, (yyvsp[(3) - (5)].id), 1);
        if(i) {
            check_override(i, flags);
        }

        info = varinfo_register_onclass(state->cls->info, access, (yyvsp[(3) - (5)].id));
    } else {
        slotinfo_t*i = registry_find(state->package, (yyvsp[(3) - (5)].id));
        if(i) {
            syntaxerror("package %s already contains '%s'", state->package, (yyvsp[(3) - (5)].id));
        }
        info = varinfo_register_global(access, state->package, (yyvsp[(3) - (5)].id));
    }

    info->type = (yyvsp[(4) - (5)].classinfo);
    info->flags = flags;

    /* slot name */
    namespace_t mname_ns = {access, ""};
    multiname_t mname = {QNAME, &mname_ns, 0, (yyvsp[(3) - (5)].id)};
  
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
        code = &state->cls->static_init;
    } else {
        // instance variable
        traits = &state->cls->abc->traits;
        code = &state->cls->init;
    }
    
    trait_t*t=0;
    if((yyvsp[(4) - (5)].classinfo)) {
        MULTINAME(m, (yyvsp[(4) - (5)].classinfo));
        t = trait_new_member(traits, multiname_clone(&m), multiname_clone(&mname), 0);
    } else {
        t = trait_new_member(traits, 0, multiname_clone(&mname), 0);
    }
    info->slot = t->slot_id;
    
    /* initalization code (if needed) */
    code_t*c = 0;
    if((yyvsp[(5) - (5)].value).c && !is_pushundefined((yyvsp[(5) - (5)].value).c)) {
        c = abc_getlocal_0(c);
        c = code_append(c, (yyvsp[(5) - (5)].value).c);
        c = converttype(c, (yyvsp[(5) - (5)].value).t, (yyvsp[(4) - (5)].classinfo));
        c = abc_setslot(c, t->slot_id);
    }

    *code = code_append(*code, c);

    if((yyvsp[(2) - (5)].token)==KW_CONST) {
        t->kind= TRAIT_CONST;
    }

    (yyval.code)=0;
}
    }
    break;


  
    case 161:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2343 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 162:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2344 "parser.y"
    {(yyval.constant)=(yyvsp[(2) - (2)].constant);}
    }
    break;


  
    case 163:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2346 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_uint));}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2347 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 165:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2348 "parser.y"
    {(yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));}
    }
    break;


  
    case 166:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2349 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2350 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2352 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2353 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2354 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2355 "parser.y"
    {
    // TODO
    as3_warning("Couldn't resolve %s", (yyvsp[(1) - (1)].id));
    (yyval.constant) = constant_new_null((yyvsp[(1) - (1)].id));
}
    }
    break;


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2364 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2368 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2374 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(2) - (2)].param));
}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2380 "parser.y"
    {
    PASS12
    (yyval.params) =(yyvsp[(1) - (4)].params);
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(4) - (4)].param));
}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2388 "parser.y"
    {
    PASS12
    (yyval.params) = (yyvsp[(1) - (3)].params);
    list_append((yyval.params).list, (yyvsp[(3) - (3)].param));
}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2393 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    list_append((yyval.params).list, (yyvsp[(1) - (1)].param));
}
    }
    break;


  
    case 178:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2399 "parser.y"
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


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2407 "parser.y"
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


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2415 "parser.y"
    {(yyval.token)=(yyvsp[(1) - (1)].token);}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2416 "parser.y"
    {(yyval.token)=(yyvsp[(1) - (1)].token);}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2417 "parser.y"
    {(yyval.token)=0;}
    }
    break;


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2420 "parser.y"
    {PASS12 startfunction(0,(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 184:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2421 "parser.y"
    {
    PASS1 old_state();list_deep_free((yyvsp[(6) - (12)].params).list);
    PASS2
    if(!state->method->info) syntaxerror("internal error");
    
    code_t*c = method_header();
    c = wrap_function(c, 0, (yyvsp[(11) - (12)].code));

    endfunction(0,(yyvsp[(1) - (12)].flags),(yyvsp[(3) - (12)].token),(yyvsp[(4) - (12)].id),&(yyvsp[(6) - (12)].params),(yyvsp[(8) - (12)].classinfo),c);
    list_deep_free((yyvsp[(6) - (12)].params).list);
    (yyval.code)=0;
}
    }
    break;


  
    case 186:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2435 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2437 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2438 "parser.y"
    {
    PASS1 old_state();list_deep_free((yyvsp[(4) - (10)].params).list);
    PASS2
    methodinfo_t*f = state->method->info;
    if(!f) syntaxerror("internal error");
    
    code_t*c = method_header();
    c = wrap_function(c, 0, (yyvsp[(9) - (10)].code));

    int index = state->method->var_index;
    endfunction(0,0,0,(yyvsp[(2) - (10)].id),&(yyvsp[(4) - (10)].params),(yyvsp[(6) - (10)].classinfo),c);
    list_deep_free((yyvsp[(4) - (10)].params).list);
    
    (yyval.value).c = abc_getlocal(0, index);
    (yyval.value).t = TYPE_FUNCTION(f);
}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2458 "parser.y"
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


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2467 "parser.y"
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


  
    case 193:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2483 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 194:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2484 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2486 "parser.y"
    {(yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2487 "parser.y"
    {(yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2488 "parser.y"
    {(yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2497 "parser.y"
    {(yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2498 "parser.y"
    {(yyval.classinfo)=0;}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2502 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2503 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2505 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 205:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2509 "parser.y"
    {(yyval.value_list).len=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2513 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2514 "parser.y"
    {
                                                  (yyval.value_list).len= (yyvsp[(1) - (2)].value_list).len+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 209:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2520 "parser.y"
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


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2556 "parser.y"
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


  
    case 211:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2602 "parser.y"
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


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2623 "parser.y"
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


  
    case 213:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2644 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 214:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2647 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 215:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2654 "parser.y"
    {(yyval.value)=(yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 216:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2655 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 217:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2656 "parser.y"
    {
    (yyval.value).c = (yyvsp[(1) - (3)].value).c;
    (yyval.value).c = cut_last_push((yyval.value).c);
    (yyval.value).c = code_append((yyval.value).c,(yyvsp[(3) - (3)].value).c);
    (yyval.value).t = (yyvsp[(3) - (3)].value).t;
}
    }
    break;


  
    case 218:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2662 "parser.y"
    {
    (yyval.code)=cut_last_push((yyvsp[(1) - (1)].value).c);
}
    }
    break;


  
    case 219:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2668 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 221:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2672 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2674 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2676 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 225:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2680 "parser.y"
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


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2697 "parser.y"
    {(yyval.value).c = abc_pushbyte(0, (yyvsp[(1) - (1)].number_uint));
                   //MULTINAME(m, registry_getintclass());
                   //$$.c = abc_coerce2($$.c, &m); // FIXME
                   (yyval.value).t = TYPE_INT;
                  }
    }
    break;


  
    case 227:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2702 "parser.y"
    {(yyval.value).c = abc_pushshort(0, (yyvsp[(1) - (1)].number_uint));
                    (yyval.value).t = TYPE_INT;
                   }
    }
    break;


  
    case 228:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2705 "parser.y"
    {(yyval.value).c = abc_pushint(0, (yyvsp[(1) - (1)].number_int));
                  (yyval.value).t = TYPE_INT;
                 }
    }
    break;


  
    case 229:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2708 "parser.y"
    {(yyval.value).c = abc_pushuint(0, (yyvsp[(1) - (1)].number_uint));
                   (yyval.value).t = TYPE_UINT;
                  }
    }
    break;


  
    case 230:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2711 "parser.y"
    {(yyval.value).c = abc_pushdouble(0, (yyvsp[(1) - (1)].number_float));
                    (yyval.value).t = TYPE_FLOAT;
                   }
    }
    break;


  
    case 231:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2714 "parser.y"
    {(yyval.value).c = abc_pushstring2(0, &(yyvsp[(1) - (1)].str));free((char*)(yyvsp[(1) - (1)].str).str);
                     (yyval.value).t = TYPE_STRING;
                    }
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2717 "parser.y"
    {(yyval.value).c = abc_pushundefined(0);
                    (yyval.value).t = TYPE_ANY;
                   }
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2720 "parser.y"
    {(yyval.value).c = abc_pushtrue(0);
                    (yyval.value).t = TYPE_BOOLEAN;
                   }
    }
    break;


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2723 "parser.y"
    {(yyval.value).c = abc_pushfalse(0);
                     (yyval.value).t = TYPE_BOOLEAN;
                    }
    }
    break;


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2726 "parser.y"
    {(yyval.value).c = abc_pushnull(0);
                    (yyval.value).t = TYPE_NULL;
                   }
    }
    break;


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2730 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2733 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2736 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2739 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2742 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2745 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
              }
    }
    break;


  
    case 242:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2748 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 243:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2751 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2755 "parser.y"
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


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2766 "parser.y"
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


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2784 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 247:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2789 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_bitnot((yyval.value).c);
              (yyval.value).t = TYPE_INT;
             }
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2794 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitand((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2799 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitxor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2804 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2809 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_rshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2813 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_urshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2817 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_lshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2822 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_divide((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2826 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_modulo((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2830 "parser.y"
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


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2839 "parser.y"
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


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2848 "parser.y"
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


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2858 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_in((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2863 "parser.y"
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


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2876 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_instanceof((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2881 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_istypelate((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2886 "parser.y"
    {
              (yyval.value).c = (yyvsp[(3) - (4)].value).c;
              (yyval.value).c = abc_typeof((yyval.value).c);
              (yyval.value).t = TYPE_STRING;
             }
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2892 "parser.y"
    {
              (yyval.value).c = cut_last_push((yyvsp[(2) - (2)].value).c);
              (yyval.value).c = abc_pushundefined((yyval.value).c);
              (yyval.value).t = TYPE_ANY;
             }
    }
    break;


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2898 "parser.y"
    { (yyval.value).c = abc_pushundefined(0);
             (yyval.value).t = TYPE_ANY;
           }
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2902 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (3)].value);}
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2904 "parser.y"
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


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2915 "parser.y"
    {
  (yyval.value).c = (yyvsp[(1) - (4)].value).c;
  (yyval.value).c = code_append((yyval.value).c, (yyvsp[(3) - (4)].value).c);

  MULTINAME_LATE(m, (yyvsp[(1) - (4)].value).t?(yyvsp[(1) - (4)].value).t->access:ACCESS_PACKAGE, "");
  (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
  (yyval.value).t = 0; // array elements have unknown type
}
    }
    break;


  
    case 269:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2924 "parser.y"
    {
    (yyval.value).c = code_new();
    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(2) - (3)].value_list).cc);
    (yyval.value).c = abc_newarray((yyval.value).c, (yyvsp[(2) - (3)].value_list).len);
    (yyval.value).t = registry_getarrayclass();
}
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2931 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2932 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2934 "parser.y"
    {
    (yyval.value_list).cc = 0;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(1) - (3)].value).c);
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (3)].value).c);
    (yyval.value_list).len = 2;
}
    }
    break;


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2940 "parser.y"
    {
    (yyval.value_list).cc = (yyvsp[(1) - (5)].value_list).cc;
    (yyval.value_list).len = (yyvsp[(1) - (5)].value_list).len+2;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (5)].value).c);
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(5) - (5)].value).c);
}
    }
    break;


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2949 "parser.y"
    {
    (yyval.value).c = code_new();
    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(2) - (3)].value_list).cc);
    (yyval.value).c = abc_newobject((yyval.value).c, (yyvsp[(2) - (3)].value_list).len/2);
    (yyval.value).t = registry_getobjectclass();
}
    }
    break;


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2956 "parser.y"
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


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2968 "parser.y"
    { 
               code_t*c = abc_modulo((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '%'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2974 "parser.y"
    { 
               code_t*c = abc_lshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '<'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 278:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2980 "parser.y"
    { 
               code_t*c = abc_rshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '>'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 279:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2986 "parser.y"
    { 
               code_t*c = abc_urshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, 'U'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 280:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2992 "parser.y"
    { 
               code_t*c = abc_divide((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '/'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 281:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2998 "parser.y"
    { 
               code_t*c = abc_bitor((yyvsp[(3) - (3)].value).c);
               c=converttype(c, TYPE_INT, (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3004 "parser.y"
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


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3017 "parser.y"
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


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3028 "parser.y"
    { code_t*c = 0;
              c = code_append(c, (yyvsp[(3) - (3)].value).c);
              c = converttype(c, (yyvsp[(3) - (3)].value).t, (yyvsp[(1) - (3)].value).t);
              (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 1, 0);
              (yyval.value).t = (yyvsp[(1) - (3)].value).t;
            }
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3035 "parser.y"
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


  
    case 286:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3048 "parser.y"
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


  
    case 287:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3075 "parser.y"
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


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3089 "parser.y"
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


  
    case 289:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3103 "parser.y"
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


  
    case 290:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3118 "parser.y"
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


  
    case 291:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3132 "parser.y"
    {
              // attribute TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored @ operator");
           }
    }
    break;


  
    case 292:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3139 "parser.y"
    {
              // child attribute  TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored .@ operator");
           }
    }
    break;


  
    case 293:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3146 "parser.y"
    {
              // namespace declaration TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored :: operator");
           }
    }
    break;


  
    case 294:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3153 "parser.y"
    {
              // descendants TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored .. operator");
           }
    }
    break;


  
    case 295:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3160 "parser.y"
    {
              // filter TODO
              (yyval.value).c = abc_pushundefined(0);
              (yyval.value).t = 0;
              as3_warning("ignored .() operator");
           }
    }
    break;


  
    case 296:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3172 "parser.y"
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


  
    case 297:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3205 "parser.y"
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


  
    case 298:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3307 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 299:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3308 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 300:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3309 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 301:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3311 "parser.y"
    {
    PASS12
    tokenizer_register_namespace((yyvsp[(3) - (3)].id));
    (yyval.code)=0;
}
    }
    break;




/* Line 1464 of skeleton.m4  */
#line 7679 "parser.tab.c"
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



