
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
#define yyparse         avm2_parse
#define yylex           avm2_lex
#define yyerror         avm2_error
#define yylval          avm2_lval
#define yychar          avm2_char
#define yydebug         avm2_debug
#define yynerrs         avm2_nerrs


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



/* Line 198 of skeleton.m4  */
#line 96 "parser.tab.c"

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
     T_BYTE = 264,
     T_SHORT = 265,
     T_FLOAT = 266,
     T_FOR = 267,
     T_WHILE = 268,
     T_DO = 269,
     T_SWITCH = 270,
     KW_IMPLEMENTS = 271,
     KW_NAMESPACE = 272,
     KW_PACKAGE = 273,
     KW_PROTECTED = 274,
     KW_PUBLIC = 275,
     KW_PRIVATE = 276,
     KW_USE = 277,
     KW_INTERNAL = 278,
     KW_NEW = 279,
     KW_NATIVE = 280,
     KW_FUNCTION = 281,
     KW_FINALLY = 282,
     KW_UNDEFINED = 283,
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
     T_EQEQ = 324,
     T_EQEQEQ = 325,
     T_NE = 326,
     T_NEE = 327,
     T_LE = 328,
     T_GE = 329,
     T_ORBY = 330,
     T_DIVBY = 331,
     T_MODBY = 332,
     T_MULBY = 333,
     T_PLUSBY = 334,
     T_MINUSBY = 335,
     T_SHRBY = 336,
     T_SHLBY = 337,
     T_USHRBY = 338,
     T_OROR = 339,
     T_ANDAND = 340,
     T_COLONCOLON = 341,
     T_MINUSMINUS = 342,
     T_PLUSPLUS = 343,
     T_DOTDOT = 344,
     T_DOTDOTDOT = 345,
     T_SHL = 346,
     T_USHR = 347,
     T_SHR = 348,
     prec_none = 349,
     below_semicolon = 350,
     below_assignment = 351,
     below_minus = 354,
     minusminus_prefix = 355,
     plusplus_prefix = 356,
     below_curly = 357,
     above_identifier = 358,
     below_else = 359,
     above_function = 360
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union 
/* Line 223 of skeleton.m4  */
#line 41 "parser.y"
tokenunion
{

/* Line 223 of skeleton.m4  */
#line 41 "parser.y"

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
#line 268 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 251 "parser.y"


static int yyerror(char*s)
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

typedef struct _methodstate {
    /* method data */
    memberinfo_t*info;
    char late_binding;
    char is_constructor;
    char has_super;
    char is_global;
    abc_exception_list_t*exceptions;
} methodstate_t;

typedef struct _state {
    struct _state*old;
    int level;
    
    char*package;     
    import_list_t*wildcard_imports;
    dict_t*imports;
    char has_own_imports;
  
    classstate_t*cls;   
    methodstate_t*method;

    char*exception_name;
    
    dict_t*vars;
} state_t;

typedef struct _global {
    abc_file_t*file;
    abc_script_t*init;

    int variable_count;
} global_t;

static global_t*global = 0;
static state_t* state = 0;

DECLARE_LIST(state);

#define MULTINAME(m,x) \
    multiname_t m;\
    namespace_t m##_ns;\
    registry_fill_multiname(&m, &m##_ns, x);
                    
#define MEMBER_MULTINAME(m,f,n) \
    multiname_t m;\
    namespace_t m##_ns;\
    if(f) { \
        m##_ns = flags2namespace(f->flags, ""); \
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
    
    if(leaving->method && leaving->method != state->method) {
        free(leaving->method);
        leaving->method=0;
    }
    if(leaving->cls && leaving->cls != state->cls) {
        free(leaving->cls);
        leaving->cls=0;
    }
    
    state_destroy(leaving);
}

void initialize_parser()
{
    global = rfx_calloc(sizeof(global_t));
    global->file = abc_file_new();
    global->file->flags &= ~ABCFILE_LAZY;
    global->variable_count = 1;
    global->init = abc_initscript(global->file);
    code_t*c = global->init->method->body->code;
    c = abc_getlocal_0(c);
    c = abc_pushscope(c);
    /*c = abc_findpropstrict(c, "[package]::trace");
    c = abc_pushstring(c, "[entering global init function]");
    c = abc_callpropvoid(c, "[package]::trace", 1);*/
    global->init->method->body->code = c;
}

void initialize_file(char*filename)
{
    new_state();
    state->package = filename;
    // needed for state->method->late_binding:
    state->method = rfx_calloc(sizeof(methodstate_t));
}
void finish_file()
{
    if(!state || state->level!=1) {
        syntaxerror("unexpected end of file");
    }
    state_destroy(state);state=0;
}

void* finish_parser()
{
    code_t*c = global->init->method->body->code;
    /*c = abc_findpropstrict(c, "[package]::trace");
      c = abc_pushstring(c, "[leaving global init function]");
      c = abc_callpropvoid(c, "[package]::trace", 1);*/
    c = abc_returnvoid(c);
    global->init->method->body->code = c;
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
static int new_variable(char*name, classinfo_t*type, char init)
{
    NEW(variable_t, v);
    v->index = global->variable_count;
    v->type = type;
    v->init = init;
    
    dict_put(state->vars, name, v);

    return global->variable_count++;
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
    global->variable_count = 1;
}
static void endpackage()
{
    /*printf("leaving package \"%s\"\n", state->package);*/

    //used e.g. in classinfo_register:
    //free(state->package);state->package=0;

    old_state();
}

void parserassert(int b)
{
    if(!b) syntaxerror("internal error: assertion failed");
}


char*as3_globalclass=0;
static void startclass(int flags, char*classname, classinfo_t*extends, classinfo_list_t*implements, char interface)
{
    if(state->cls) {
        syntaxerror("inner classes now allowed"); 
    }
    new_state();
    global->variable_count = 1;
    state->cls = rfx_calloc(sizeof(classstate_t));
    state->method = rfx_calloc(sizeof(methodstate_t)); // method state, for static constructor

    token_list_t*t=0;
    classinfo_list_t*mlist=0;

    if(flags&~(FLAG_PACKAGEINTERNAL|FLAG_PUBLIC|FLAG_FINAL|FLAG_DYNAMIC))
        syntaxerror("invalid modifier(s)");

    if((flags&(FLAG_PUBLIC|FLAG_PACKAGEINTERNAL)) == (FLAG_PUBLIC|FLAG_PACKAGEINTERNAL))
        syntaxerror("public and internal not supported at the same time.");

    /* create the class name, together with the proper attributes */
    int access=0;
    char*package=0;

    if(!(flags&FLAG_PUBLIC) && !state->package) {
        access = ACCESS_PRIVATE; package = current_filename;
    } else if(!(flags&FLAG_PUBLIC) && state->package) {
        access = ACCESS_PACKAGEINTERNAL; package = state->package;
    } else if(state->package) {
        access = ACCESS_PACKAGE; package = state->package;
    } else {
        syntaxerror("public classes only allowed inside a package");
    }

    if(as3_pass==1) {
        if(registry_findclass(package, classname)) {
            syntaxerror("Package \"%s\" already contains a class called \"%s\"", package, classname);
        }
        /* build info struct */
        int num_interfaces = (list_length(implements));
        state->cls->info = classinfo_register(access, package, classname, num_interfaces);
    }
    
    if(as3_pass == 2) {
        state->cls->info = registry_findclass(package, classname);
        parserassert((int)state->cls->info);

        /* fill out interfaces and extends (we couldn't resolve those during the first pass) */
        state->cls->info->superclass = extends?extends:TYPE_OBJECT;
        int pos = 0;
        classinfo_list_t*l = implements;
        for(l=implements;l;l=l->next) {
            state->cls->info->interfaces[pos++] = l->classinfo;
        }

        /* generate the abc code for this class */
        MULTINAME(classname2,state->cls->info);
        multiname_t*extends2 = sig2mname(extends);

        state->cls->abc = abc_class_new(global->file, &classname2, extends2);
        if(flags&FLAG_FINAL) abc_class_final(state->cls->abc);
        if(!(flags&FLAG_DYNAMIC)) abc_class_sealed(state->cls->abc);
        if(interface) {
            state->cls->info->flags |= CLASS_INTERFACE;
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

        if(!as3_globalclass && (flags&FLAG_PUBLIC) && classinfo_equals(registry_getMovieClip(),extends)) {
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
        if(!state->cls->has_constructor && !(state->cls->info->flags&CLASS_INTERFACE)) {
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


static memberinfo_t*registerfunction(enum yytokentype getset, int flags, char*name, params_t*params, classinfo_t*return_type, int slot)
{
    memberinfo_t*minfo = 0;
    if(!state->cls) {
        //package method
        minfo = memberinfo_register_global(flags2access(flags), state->package, name, MEMBER_METHOD);
        minfo->return_type = return_type;
    } else if(getset != KW_GET && getset != KW_SET) {
        //class method
        if((minfo = registry_findmember(state->cls->info, name, 0))) {
            if(minfo->parent == state->cls->info) {
                syntaxerror("class already contains a member/method called '%s'", name);
            } else if(!minfo->parent) {
                syntaxerror("internal error: overriding method %s, which doesn't have parent", name);
            } else {
                if(!(minfo->flags&(FLAG_STATIC|FLAG_PRIVATE)))
                    syntaxerror("function %s already exists in superclass. Did you forget the 'override' keyword?");
            }
        }
        minfo = memberinfo_register(state->cls->info, name, MEMBER_METHOD);
        minfo->return_type = return_type;
        // getslot on a member slot only returns "undefined", so no need
        // to actually store these
        //state->minfo->slot = state->method->abc->method->trait->slot_id;
    } else {
        //class getter/setter
        int gs = getset==KW_GET?MEMBER_GET:MEMBER_SET;
        classinfo_t*type=0;
        if(getset == KW_GET)
            type = return_type;
        else if(params->list)
            type = params->list->param->type;
        // not sure wether to look into superclasses here, too
        if((minfo=registry_findmember(state->cls->info, name, 0))) {
            if(minfo->kind & ~(MEMBER_GET|MEMBER_SET))
                syntaxerror("class already contains a member or method called '%s'", name);
            if(minfo->kind & gs)
                syntaxerror("getter/setter for '%s' already defined", name);
            /* make a setter or getter into a getset */
            minfo->kind |= gs;
            if(!minfo->type) 
                minfo->type = type;
            else
                if(type && minfo->type != type)
                    syntaxerror("different type in getter and setter");
        } else {
            minfo = memberinfo_register(state->cls->info, name, gs);
            minfo->type = type;
        }
        /* can't assign a slot as getter and setter might have different slots */
        //minfo->slot = slot;
    }
    if(flags&FLAG_STATIC) minfo->flags |= FLAG_STATIC;
    if(flags&FLAG_PUBLIC) minfo->flags |= FLAG_PUBLIC;
    if(flags&FLAG_PRIVATE) minfo->flags |= FLAG_PRIVATE;
    if(flags&FLAG_PROTECTED) minfo->flags |= FLAG_PROTECTED;
    if(flags&FLAG_PACKAGEINTERNAL) minfo->flags |= FLAG_PACKAGEINTERNAL;
    if(flags&FLAG_OVERRIDE) minfo->flags |= FLAG_OVERRIDE;
    return minfo;
}

static void startfunction(token_t*ns, int flags, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type)
{
    if(state->method && state->method->info) {
        syntaxerror("not able to start another method scope");
    }
    new_state();
    state->method = rfx_calloc(sizeof(methodstate_t));
    state->method->has_super = 0;

    if(state->cls) {
        state->method->is_constructor = !strcmp(state->cls->info->name,name);
        state->cls->has_constructor |= state->method->is_constructor;
    } else {
        state->method->is_global = 1;
        state->method->late_binding = 1; // for global methods, always push local_0 on the scope stack
    }
    if(state->method->is_constructor)
        name = "__as3_constructor__";

    if(as3_pass == 1) {
        return_type = 0;
        state->method->info = registerfunction(getset, flags, name, params, return_type, 0);
    }

    if(as3_pass == 2) {
        /* retrieve the member info that we stored in the first pass.
           TODO: better getter/setter support? */
        if(!state->cls) state->method->info = registry_findclass(state->package, name)->function;
        else            state->method->info = registry_findmember(state->cls->info, name, 0);
        state->method->info->return_type = return_type;

        global->variable_count = 0;
        /* state->vars is initialized by state_new */
        if(!state->method->is_global)
            new_variable((flags&FLAG_STATIC)?"class":"this", state->cls->info, 0);
        else
            new_variable("globalscope", 0, 0);
        param_list_t*p=0;
        for(p=params->list;p;p=p->next) {
            new_variable(p->param->name, p->param->type, 0);
        }
    } 
}

static void endfunction(token_t*ns, int flags, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type, code_t*body)
{
    if(as3_pass==1) {
        old_state();
        return;
    }

    abc_method_t*f = 0;

    multiname_t*type2 = sig2mname(return_type);
    int slot = 0;
    if(state->method->is_constructor) {
        f = abc_class_getconstructor(state->cls->abc, type2);
    } else if(!state->method->is_global) {
        namespace_t mname_ns = flags2namespace(flags, "");
        multiname_t mname = {QNAME, &mname_ns, 0, name};

        if(flags&FLAG_STATIC)
            f = abc_class_staticmethod(state->cls->abc, type2, &mname);
        else
            f = abc_class_method(state->cls->abc, type2, &mname);
        slot = f->trait->slot_id;
    } else {
        namespace_t mname_ns = flags2namespace(flags, state->package);
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

static classinfo_t* find_class(char*name)
{
    classinfo_t*c=0;

    c = registry_findclass(state->package, name);
    if(c) return c;

    /* try explicit imports */
    dictentry_t* e = dict_get_slot(state->imports, name);
    if(c) return c;
    while(e) {
        if(!strcmp(e->key, name)) {
            c = (classinfo_t*)e->data;
            if(c) return c;
        }
        e = e->next;
    }

    /* try package.* imports */
    import_list_t*l = state->wildcard_imports;
    while(l) {
        //printf("does package %s contain a class %s?\n", l->import->package, name);
        c = registry_findclass(l->import->package, name);
        if(c) return c;
        l = l->next;
    }

    /* try global package */
    c = registry_findclass("", name);
    if(c) return c;
   
    /* try local "filename" package */
    c = registry_findclass(current_filename_short, name);
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
#line 1545 "parser.tab.c"

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
#define YYFINAL  119
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2463

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  130
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  285
/* YYNRULES -- Number of states.  */
#define YYNSTATES  478

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   360

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   116,     2,     2,     2,   114,   106,     2,
     126,   129,   113,   111,    97,   110,   123,   112,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   103,    96,
     107,    99,   108,   102,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   120,     2,   121,   105,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   122,   104,   128,   115,     2,     2,     2,
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
      95,    98,   100,   101,   109,   117,   118,   119,   124,   125,
     127
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    10,    13,    15,    17,
      19,    21,    23,    25,    27,    28,    30,    32,    35,    37,
      39,    41,    43,    45,    47,    49,    50,    53,    55,    57,
      59,    61,    63,    65,    67,    69,    71,    73,    75,    77,
      79,    81,    83,    85,    87,    89,    91,    93,    97,   100,
     103,   105,   107,   110,   111,   114,   117,   119,   123,   127,
     128,   131,   132,   140,   141,   143,   145,   149,   151,   154,
     158,   167,   174,   175,   182,   183,   191,   193,   196,   198,
     201,   202,   204,   206,   209,   211,   214,   219,   223,   224,
     233,   234,   244,   245,   251,   253,   256,   258,   261,   263,
     264,   271,   274,   276,   282,   284,   286,   290,   292,   293,
     300,   301,   307,   310,   315,   316,   318,   320,   323,   325,
     327,   329,   331,   333,   335,   337,   339,   341,   342,   345,
     346,   349,   350,   353,   354,   364,   365,   374,   375,   377,
     379,   382,   384,   386,   388,   390,   391,   393,   395,   398,
     400,   403,   412,   414,   416,   422,   423,   426,   428,   430,
     432,   434,   436,   438,   440,   442,   443,   445,   448,   453,
     457,   459,   464,   467,   469,   471,   472,   473,   486,   488,
     489,   499,   501,   505,   507,   509,   511,   515,   517,   519,
     521,   524,   525,   526,   530,   531,   533,   535,   539,   543,
     548,   553,   556,   558,   561,   563,   565,   569,   571,   573,
     575,   577,   579,   581,   583,   585,   587,   589,   591,   593,
     595,   597,   599,   601,   603,   605,   609,   613,   617,   621,
     625,   629,   633,   637,   641,   645,   648,   651,   655,   659,
     663,   667,   671,   675,   679,   683,   687,   691,   695,   699,
     703,   707,   711,   716,   719,   721,   725,   728,   733,   737,
     738,   740,   744,   750,   754,   758,   762,   766,   770,   774,
     778,   782,   786,   790,   794,   800,   803,   806,   809,   812,
     816,   820,   822,   826,   832,   838
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     131,     0,    -1,   132,    -1,    -1,   133,    -1,   134,    -1,
     133,   134,    -1,   180,    -1,   192,    -1,   190,    -1,   208,
      -1,   201,    -1,   143,    -1,    96,    -1,    -1,   136,    -1,
     137,    -1,   136,   137,    -1,   192,    -1,   190,    -1,   208,
      -1,   201,    -1,   143,    -1,    96,    -1,   139,    -1,    -1,
     139,   141,    -1,   141,    -1,   183,    -1,   154,    -1,   155,
      -1,   156,    -1,   158,    -1,   166,    -1,   149,    -1,   177,
      -1,   174,    -1,   227,    -1,    96,    -1,   140,    -1,   145,
      -1,   160,    -1,   161,    -1,   224,    -1,   176,    -1,   233,
      -1,   234,    -1,   122,   139,   128,    -1,   122,   128,    -1,
     141,    96,    -1,   141,    -1,   140,    -1,    99,   225,    -1,
      -1,    45,   146,    -1,    31,   146,    -1,   147,    -1,   146,
      97,   147,    -1,     3,   217,   144,    -1,    -1,    64,   142,
      -1,    -1,    63,   126,   150,   226,   129,   142,   148,    -1,
      -1,   145,    -1,   227,    -1,    45,     3,   217,    -1,     3,
      -1,    12,   126,    -1,    12,    49,   126,    -1,   153,   151,
      96,   226,    96,   227,   129,   142,    -1,   153,   152,    67,
     226,   129,   142,    -1,    -1,    13,   126,   157,   226,   129,
     142,    -1,    -1,    14,   159,   142,    13,   126,   226,   129,
      -1,    65,    -1,    65,     3,    -1,    29,    -1,    29,     3,
      -1,    -1,   163,    -1,   165,    -1,   163,   165,    -1,   164,
      -1,   163,   164,    -1,    33,   228,   103,   138,    -1,    61,
     103,   138,    -1,    -1,    15,   126,   167,   228,   129,   122,
     162,   128,    -1,    -1,    32,   126,     3,   217,   129,   169,
     122,   138,   128,    -1,    -1,    27,   122,   171,   138,   128,
      -1,   168,    -1,   172,   168,    -1,   172,    -1,   172,   170,
      -1,   170,    -1,    -1,    51,   122,   175,   138,   128,   173,
      -1,    36,   226,    -1,    36,    -1,    38,   126,   226,   129,
     142,    -1,     3,    -1,    18,    -1,   179,   123,   178,    -1,
     178,    -1,    -1,    18,   179,   122,   181,   135,   128,    -1,
      -1,    18,   122,   182,   135,   128,    -1,    40,   214,    -1,
      40,   179,   123,   113,    -1,    -1,   185,    -1,   186,    -1,
     185,   186,    -1,    20,    -1,    21,    -1,    19,    -1,    37,
      -1,    46,    -1,    48,    -1,    47,    -1,    25,    -1,    23,
      -1,    -1,    53,   214,    -1,    -1,    53,   215,    -1,    -1,
      16,   215,    -1,    -1,   184,    30,     3,   187,   189,   122,
     191,   194,   128,    -1,    -1,   184,    43,     3,   188,   122,
     193,   197,   128,    -1,    -1,   195,    -1,   196,    -1,   195,
     196,    -1,    96,    -1,   201,    -1,   208,    -1,   140,    -1,
      -1,   198,    -1,   199,    -1,   198,   199,    -1,    96,    -1,
      45,     3,    -1,   184,    26,   207,     3,   126,   204,   129,
     217,    -1,    45,    -1,    31,    -1,   184,   200,     3,   217,
     144,    -1,    -1,    99,   203,    -1,     9,    -1,     7,    -1,
       8,    -1,    11,    -1,     4,    -1,    55,    -1,    54,    -1,
      44,    -1,    -1,   205,    -1,    90,   206,    -1,   205,    97,
      90,   206,    -1,   205,    97,   206,    -1,   206,    -1,     3,
     103,   216,   202,    -1,     3,   202,    -1,    50,    -1,    34,
      -1,    -1,    -1,   184,    26,   207,     3,   126,   204,   129,
     217,   122,   209,   138,   128,    -1,     3,    -1,    -1,    26,
     210,   126,   204,   129,   217,   122,   138,   128,    -1,     3,
      -1,   179,   123,     3,    -1,   213,    -1,   212,    -1,   214,
      -1,   215,    97,   214,    -1,   214,    -1,   113,    -1,    35,
      -1,   103,   216,    -1,    -1,    -1,   126,   219,   129,    -1,
      -1,   220,    -1,   225,    -1,   220,    97,   225,    -1,    24,
     212,   218,    -1,   228,   126,   219,   129,    -1,    52,   126,
     219,   129,    -1,    62,   228,    -1,    41,    -1,    41,   226,
      -1,   228,    -1,   228,    -1,   226,    97,   228,    -1,   226,
      -1,   211,    -1,   229,    -1,   232,    -1,   221,    -1,   223,
      -1,     5,    -1,     9,    -1,    10,    -1,     7,    -1,     8,
      -1,    11,    -1,     4,    -1,    28,    -1,    55,    -1,    54,
      -1,    44,    -1,   222,    -1,   228,   107,   228,    -1,   228,
     108,   228,    -1,   228,    73,   228,    -1,   228,    74,   228,
      -1,   228,    69,   228,    -1,   228,    70,   228,    -1,   228,
      72,   228,    -1,   228,    71,   228,    -1,   228,    84,   228,
      -1,   228,    85,   228,    -1,   116,   228,    -1,   115,   228,
      -1,   228,   106,   228,    -1,   228,   105,   228,    -1,   228,
     104,   228,    -1,   228,    93,   228,    -1,   228,    92,   228,
      -1,   228,    91,   228,    -1,   228,   112,   228,    -1,   228,
     114,   228,    -1,   228,   111,   228,    -1,   228,   110,   228,
      -1,   228,   113,   228,    -1,   228,    67,   228,    -1,   228,
      68,   228,    -1,   228,    39,   228,    -1,   228,    66,   228,
      -1,    42,   126,   228,   129,    -1,    35,   228,    -1,    35,
      -1,   126,   226,   129,    -1,   110,   228,    -1,   228,   120,
     228,   121,    -1,   120,   219,   121,    -1,    -1,   231,    -1,
     225,   103,   225,    -1,   231,    97,   225,   103,   225,    -1,
     122,   230,   128,    -1,   228,    78,   228,    -1,   228,    77,
     228,    -1,   228,    82,   228,    -1,   228,    81,   228,    -1,
     228,    83,   228,    -1,   228,    76,   228,    -1,   228,    75,
     228,    -1,   228,    79,   228,    -1,   228,    80,   228,    -1,
     228,    99,   228,    -1,   228,   102,   228,   103,   228,    -1,
     228,    88,    -1,   228,    87,    -1,    88,   228,    -1,    87,
     228,    -1,    52,   123,     3,    -1,   228,   123,     3,    -1,
       3,    -1,   184,    17,     3,    -1,   184,    17,     3,    99,
       3,    -1,   184,    17,     3,    99,     4,    -1,    22,    17,
       3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1519,  1519,  1521,  1521,  1522,  1523,  1525,  1526,  1527,
    1528,  1529,  1530,  1531,  1533,  1533,  1534,  1535,  1537,  1538,
    1539,  1540,  1541,  1542,  1544,  1545,  1547,  1548,  1551,  1552,
    1553,  1554,  1555,  1556,  1557,  1558,  1559,  1560,  1563,  1564,
    1565,  1566,  1567,  1568,  1569,  1571,  1572,  1574,  1575,  1576,
    1577,  1581,  1588,  1589,  1593,  1594,  1596,  1597,  1599,  1640,
    1641,  1644,  1644,  1663,  1664,  1665,  1668,  1671,  1675,  1676,
    1678,  1698,  1741,  1741,  1760,  1760,  1775,  1778,  1781,  1784,
    1788,  1789,  1790,  1791,  1792,  1793,  1795,  1806,  1809,  1809,
    1838,  1838,  1858,  1858,  1875,  1876,  1877,  1878,  1886,  1895,
    1895,  1940,  1944,  1955,  1964,  1965,  1967,  1968,  1970,  1970,
    1972,  1972,  1975,  1983,  1993,  1994,  1995,  1996,  1998,  1999,
    2000,  2001,  2002,  2003,  2004,  2005,  2006,  2008,  2009,  2011,
    2012,  2014,  2015,  2019,  2017,  2025,  2023,  2031,  2032,  2033,
    2034,  2035,  2036,  2037,  2039,  2045,  2046,  2047,  2048,  2049,
    2050,  2053,  2065,  2065,  2067,  2126,  2127,  2129,  2130,  2131,
    2132,  2133,  2135,  2136,  2137,  2142,  2145,  2150,  2155,  2162,
    2166,  2171,  2177,  2183,  2184,  2185,  2188,  2187,  2208,  2209,
    2210,  2218,  2226,  2234,  2235,  2237,  2238,  2240,  2241,  2242,
    2251,  2252,  2256,  2257,  2259,  2260,  2261,  2264,  2269,  2293,
    2341,  2361,  2382,  2385,  2392,  2393,  2394,  2400,  2406,  2408,
    2410,  2412,  2414,  2416,  2433,  2438,  2441,  2444,  2447,  2450,
    2453,  2456,  2459,  2462,  2466,  2467,  2470,  2473,  2476,  2479,
    2482,  2485,  2488,  2492,  2503,  2521,  2526,  2531,  2536,  2541,
    2546,  2550,  2554,  2559,  2563,  2567,  2576,  2585,  2595,  2600,
    2612,  2618,  2623,  2629,  2635,  2639,  2641,  2652,  2661,  2668,
    2669,  2671,  2677,  2686,  2693,  2705,  2711,  2717,  2723,  2729,
    2735,  2741,  2754,  2765,  2772,  2785,  2812,  2826,  2840,  2854,
    2869,  2903,  3001,  3002,  3003,  3005
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IDENTIFIER", "T_STRING", "T_REGEXP",
  "T_EMPTY", "T_INT", "T_UINT", "T_BYTE", "T_SHORT", "T_FLOAT", "\"for\"",
  "\"while\"", "\"do\"", "\"switch\"", "\"implements\"", "\"namespace\"",
  "\"package\"", "\"protected\"", "\"public\"", "\"private\"", "\"use\"",
  "\"internal\"", "\"new\"", "\"native\"", "\"function\"", "\"finally\"",
  "\"undefined\"", "\"continue\"", "\"class\"", "\"const\"", "\"catch\"",
  "\"case\"", "\"set\"", "\"void\"", "\"throw\"", "\"static\"", "\"with\"",
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
  "plusplus_prefix", "below_curly", "'['", "']'", "'{'", "'.'",
  "above_identifier", "below_else", "'('", "above_function", "'}'", "')'",
  "$accept", "PROGRAM", "MAYBE_PROGRAM_CODE_LIST", "PROGRAM_CODE_LIST",
  "PROGRAM_CODE", "MAYBE_INPACKAGE_CODE_LIST", "INPACKAGE_CODE_LIST",
  "INPACKAGE_CODE", "MAYBECODE", "CODE", "CODE_STATEMENT", "CODEPIECE",
  "CODEBLOCK", "PACKAGE_INITCODE", "MAYBEEXPRESSION",
  "VARIABLE_DECLARATION", "VARIABLE_LIST", "ONE_VARIABLE", "MAYBEELSE",
  "IF", "$@1", "FOR_INIT", "FOR_IN_INIT", "FOR_START", "FOR", "FOR_IN",
  "WHILE", "$@2", "DO_WHILE", "$@3", "BREAK", "CONTINUE",
  "MAYBE_CASE_LIST", "CASE_LIST", "CASE", "DEFAULT", "SWITCH", "$@4",
  "CATCH", "$@5", "FINALLY", "$@6", "CATCH_LIST", "CATCH_FINALLY_LIST",
  "TRY", "$@7", "THROW", "WITH", "X_IDENTIFIER", "PACKAGE",
  "PACKAGE_DECLARATION", "$@8", "$@9", "IMPORT", "MAYBE_MODIFIERS",
  "MODIFIER_LIST", "MODIFIER", "EXTENDS", "EXTENDS_LIST",
  "IMPLEMENTS_LIST", "CLASS_DECLARATION", "$@10", "INTERFACE_DECLARATION",
  "$@11", "MAYBE_CLASS_BODY", "CLASS_BODY", "CLASS_BODY_ITEM",
  "MAYBE_INTERFACE_BODY", "INTERFACE_BODY", "IDECLARATION", "VARCONST",
  "SLOT_DECLARATION", "MAYBESTATICCONSTANT", "STATICCONSTANT",
  "MAYBE_PARAM_LIST", "PARAM_LIST", "PARAM", "GETSET",
  "FUNCTION_DECLARATION", "$@12", "MAYBE_IDENTIFIER", "INNERFUNCTION",
  "CLASS", "PACKAGEANDCLASS", "QNAME", "QNAME_LIST", "TYPE", "MAYBETYPE",
  "MAYBE_PARAM_VALUES", "MAYBE_EXPRESSION_LIST", "EXPRESSION_LIST", "NEW",
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
     345,   346,   347,   348,   349,   350,    59,    44,   351,    61,
     352,   353,    63,    58,   124,    94,    38,    60,    62,   354,
      45,    43,    47,    42,    37,   126,    33,   355,   356,   357,
      91,    93,   123,    46,   358,   359,    40,   360,   125,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   130,   131,   132,   132,   133,   133,   134,   134,   134,
     134,   134,   134,   134,   135,   135,   136,   136,   137,   137,
     137,   137,   137,   137,   138,   138,   139,   139,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   142,   142,   142,
     142,   143,   144,   144,   145,   145,   146,   146,   147,   148,
     148,   150,   149,   151,   151,   151,   152,   152,   153,   153,
     154,   155,   157,   156,   159,   158,   160,   160,   161,   161,
     162,   162,   162,   162,   163,   163,   164,   165,   167,   166,
     169,   168,   171,   170,   172,   172,   173,   173,   173,   175,
     174,   176,   176,   177,   178,   178,   179,   179,   181,   180,
     182,   180,   183,   183,   184,   184,   185,   185,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   187,   187,   188,
     188,   189,   189,   191,   190,   193,   192,   194,   194,   195,
     195,   196,   196,   196,   196,   197,   197,   198,   198,   199,
     199,   199,   200,   200,   201,   202,   202,   203,   203,   203,
     203,   203,   203,   203,   203,   204,   204,   204,   204,   205,
     205,   206,   206,   207,   207,   207,   209,   208,   210,   210,
     211,   212,   213,   214,   214,   215,   215,   216,   216,   216,
     217,   217,   218,   218,   219,   219,   220,   220,   221,   222,
     222,   223,   224,   224,   225,   226,   226,   227,   228,   228,
     228,   228,   228,   228,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   230,
     230,   231,   231,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   232,   233,   233,   233,   234
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     2,     2,
       1,     1,     2,     0,     2,     2,     1,     3,     3,     0,
       2,     0,     7,     0,     1,     1,     3,     1,     2,     3,
       8,     6,     0,     6,     0,     7,     1,     2,     1,     2,
       0,     1,     1,     2,     1,     2,     4,     3,     0,     8,
       0,     9,     0,     5,     1,     2,     1,     2,     1,     0,
       6,     2,     1,     5,     1,     1,     3,     1,     0,     6,
       0,     5,     2,     4,     0,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     2,     0,
       2,     0,     2,     0,     9,     0,     8,     0,     1,     1,
       2,     1,     1,     1,     1,     0,     1,     1,     2,     1,
       2,     8,     1,     1,     5,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     2,     4,     3,
       1,     4,     2,     1,     1,     0,     0,    12,     1,     0,
       9,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       2,     0,     0,     3,     0,     1,     1,     3,     3,     4,
       4,     2,     1,     2,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     2,     1,     3,     2,     4,     3,     0,
       1,     3,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     5,     2,     2,     2,     2,     3,
       3,     1,     3,     5,     5,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     114,   281,   219,   213,   216,   217,   214,   215,   218,     0,
       0,    74,     0,     0,   120,   118,   119,   126,     0,   125,
     179,   220,   254,   121,     0,     0,     0,   223,   122,   124,
     123,     0,     0,   222,   221,     0,     0,     0,     0,    13,
       0,     0,     0,   194,   259,     0,     0,     2,   114,     5,
      51,    12,    34,    63,    29,    30,    31,    32,    33,    36,
      35,     7,    28,     0,   115,   116,     9,     8,    11,    10,
     208,   211,   224,   212,   207,    37,   205,   209,   210,     0,
      68,    72,   114,    88,   104,   105,   110,   107,     0,   181,
     192,   178,     0,   253,     0,   181,     0,   184,   183,   112,
       0,    99,     0,   194,   201,    61,   278,   277,   256,   236,
     235,     0,   195,   196,   204,     0,     0,   260,     0,     1,
       6,   281,     0,     0,    64,     0,     0,    65,   175,     0,
     153,     0,   152,     0,   117,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   194,    69,     0,     0,
      78,   102,   202,     0,    76,    38,   114,    39,    50,     0,
      40,    41,    42,    44,     0,    43,    45,    46,     0,   114,
     108,     0,   194,   198,   165,     0,     0,     0,    25,   279,
       0,     0,   258,     0,     0,   263,     0,   255,   191,    55,
      56,   191,    54,     0,     0,   174,   173,     0,   127,   129,
     191,   206,   250,   251,   248,   249,   229,   230,   232,   231,
     227,   228,   270,   269,   265,   264,   271,   272,   267,   266,
     268,   233,   234,   242,   241,   240,   273,     0,   239,   238,
     237,   225,   226,   246,   245,   243,   247,   244,     0,   280,
       0,     0,     0,    79,   101,   203,    77,    48,   114,    27,
     205,    49,     0,     0,     0,    23,     0,   114,    16,    22,
      19,    18,    21,    20,   114,   106,     0,   155,     0,     0,
     166,   170,   114,   182,   113,   252,     0,    24,   200,     0,
     197,   261,     0,     0,    53,     0,    53,     0,     0,     0,
       0,   131,     0,     0,    53,     0,   257,   199,   114,   285,
      47,    26,     0,   282,     0,   111,    17,     0,   193,     0,
       0,   172,   167,   191,     0,   103,     0,   114,     0,   189,
     188,     0,   187,   190,     0,    58,    57,     0,   114,   165,
     128,     0,     0,   185,   130,   135,   154,   274,    73,     0,
       0,    80,   109,   161,   158,   159,   157,   160,   164,   163,
     162,   156,   155,     0,     0,   169,     0,     0,    94,    98,
      96,   100,    59,   262,     0,    52,     0,    71,     0,   132,
     133,     0,   114,    75,   283,   284,     0,     0,     0,    81,
      84,    82,   171,    25,   168,    92,     0,    95,    97,   114,
      62,   114,   191,   114,   186,     0,   149,     0,     0,   114,
     147,     0,    25,    89,    85,    83,     0,    25,   191,    60,
      70,     0,   141,   144,     0,     0,   114,   139,   142,   143,
     150,   175,   136,   148,    25,    87,   180,     0,     0,   176,
     134,   140,     0,    86,    93,    90,    25,     0,     0,     0,
     165,    25,   177,     0,     0,   191,    91,   151
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,    47,    48,    49,   286,   287,   288,   306,   307,
     187,   279,   189,   289,   355,   190,   222,   220,   420,    52,
     211,   125,   126,    53,    54,    55,    56,   178,    57,    82,
     191,   192,   408,   409,   410,   411,    58,   198,   388,   468,
     389,   437,   390,   391,    59,   208,   193,    60,    87,   351,
      61,   294,   199,    62,   194,    64,    65,   321,   323,   362,
     290,   423,   291,   402,   445,   446,   447,   428,   429,   430,
     133,   292,   341,   381,   299,   300,   301,   227,   293,   466,
      92,    70,    97,    98,   352,   364,   353,   314,   203,   111,
     112,    71,    72,    73,   195,   113,    74,    75,    76,    77,
     116,   117,    78,   196,   197
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -308
static const yytype_int16 yypact[] =
{
     349,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,   -38,
     -86,  -308,   -72,    35,  -308,  -308,  -308,  -308,    30,  -308,
      97,  -308,    52,  -308,   -14,    18,    36,  -308,  -308,  -308,
    -308,    57,    44,  -308,  -308,  1538,    40,  1538,  1538,  -308,
    1538,  1538,  1538,  1538,  1538,  1538,   102,  -308,   473,  -308,
    -308,  -308,  -308,   211,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,   275,   640,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,    96,  -308,  1971,  -308,  -308,    82,
    -308,  -308,  1469,  -308,  -308,  -308,  -308,  -308,   -32,  -308,
      83,  -308,    84,   104,  1538,    88,    89,  -308,  -308,  -308,
    1538,  -308,   186,  1538,   104,  -308,   104,   104,   163,   104,
     104,   108,   116,  -308,  1971,   114,   105,   128,   -88,  -308,
    -308,   165,   231,   233,  -308,   144,   174,  -308,    -5,   240,
    -308,   241,  -308,   244,  -308,  1538,  1538,  1538,  1538,  1538,
    1538,  1538,  1538,  1538,  1538,  1538,  1538,  1538,  1538,  1538,
    1538,  1538,  1538,  1538,  1538,  1538,  1538,  -308,  -308,  1538,
    1538,  1538,  1538,  1538,  1538,  1538,  1538,  1538,  1538,  1538,
    1538,  1538,  1538,  1538,  1538,   245,  1538,  -308,  1538,   232,
     255,  1538,  1538,   231,   256,  -308,   593,  -308,   164,   248,
    -308,  -308,  -308,  -308,   247,  -308,  -308,  -308,  1538,   845,
    -308,    90,  1538,  -308,     9,   -82,    68,  1599,  1349,  -308,
     138,  1538,  -308,  1538,  1538,  -308,  1538,  -308,   166,   171,
    -308,   166,   171,  1538,  1538,  -308,  -308,   267,   219,   221,
     166,  1971,   962,   836,   836,   836,  2337,  2337,  2337,  2337,
     962,   962,  1971,  1971,  1971,  1971,  1971,  1971,  1971,  1971,
    1971,  2032,  2093,   340,   340,   340,  1971,  1727,  2154,  2215,
    2276,   962,   962,   163,   163,   104,   104,   104,  1788,  -308,
     149,   -62,   276,  -308,    96,    96,  -308,  -308,   719,  -308,
    1849,  -308,   154,   278,  1663,  -308,   156,   971,  -308,  -308,
    -308,  -308,  -308,  -308,   845,  -308,   161,   -69,   282,   167,
     195,  -308,  1469,    88,  -308,  -308,   169,  1349,  -308,   -55,
    -308,  -308,   190,    63,   204,   231,   -24,    86,    74,   178,
      18,   292,    18,   187,   204,  1538,  -308,  -308,  1469,  -308,
    -308,  -308,  1538,   213,   188,  -308,  -308,   185,  -308,    61,
      63,  -308,  -308,   166,    11,  -308,    62,  1469,  1538,  -308,
    -308,   191,  -308,  -308,  1538,  -308,  -308,  1538,  1469,     9,
    -308,    18,   194,  -308,   220,  -308,  -308,  1971,  -308,    76,
     182,   136,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,   224,   197,   282,  -308,   202,   199,  -308,  -308,
      62,  -308,   265,  -308,    92,  -308,   203,  -308,   205,   220,
    -308,    18,   394,  -308,  -308,  -308,  1538,   235,   207,   136,
    -308,  -308,  -308,  1349,  -308,  -308,   336,  -308,  -308,  1469,
    -308,  1469,   166,  1097,  -308,   337,  -308,   316,   216,   529,
    -308,  1910,  1349,  -308,  -308,  -308,   218,  1349,   166,  -308,
    -308,   225,  -308,  -308,    66,   222,  1223,  -308,  -308,  -308,
    -308,    -5,  -308,  -308,  1349,  -308,  -308,   223,   226,  -308,
    -308,  -308,   345,  -308,  -308,  -308,  1349,   239,   249,   238,
       9,  1349,  -308,   246,   250,   166,  -308,  -308
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -308,  -308,  -308,  -308,   328,    85,  -308,    93,  -209,   196,
       1,   -76,  -244,    16,    59,   332,   268,    77,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,   -15,   -11,  -308,  -308,    15,  -308,
      17,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -193,    19,
    -308,  -308,  -308,  -308,     0,  -308,   335,  -308,  -308,  -308,
      31,  -308,    39,  -308,  -308,  -308,   -40,  -308,  -308,   -21,
    -308,     2,    27,  -308,  -307,  -308,  -271,   -41,     3,  -308,
    -308,  -308,   398,  -308,   -20,    60,    78,  -184,  -308,   -93,
    -308,  -308,  -308,  -308,  -308,   -26,   -17,   -46,   -18,  -308,
    -308,  -308,  -308,  -308,  -308
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -205
static const yytype_int16 yytable[] =
{
      63,    50,    68,    69,    93,    99,   188,   127,   295,   135,
     210,    79,   297,   295,   297,   135,    51,   104,   115,   106,
     107,    95,   108,   109,   110,   114,   114,   342,   118,   225,
     339,    66,    88,    89,   340,   135,    85,   316,    84,    67,
      81,   217,   135,   -66,    96,   226,   324,   302,    63,    50,
      68,    69,   398,    85,    83,     1,     2,     3,   345,     4,
       5,     6,     7,     8,    51,   373,    95,   328,   374,   375,
     376,   303,   377,   385,   347,   354,    18,   205,    20,    66,
      21,    85,   207,   270,   368,   114,    85,    67,    80,   386,
     200,   201,   128,    84,   387,   303,    27,   130,   349,   298,
      91,   384,   119,   392,    32,   378,    33,    34,    85,   296,
      85,   132,    94,   414,   397,   379,   380,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,    37,
      38,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,    86,   114,   383,
     115,   271,   100,   473,   274,   275,   105,   102,   280,   406,
     103,   135,    43,   135,    44,   439,   350,   440,    45,   101,
     284,   304,   357,   135,   114,   404,   405,   310,   311,   209,
     312,   157,   158,   135,   309,   114,   114,   407,   114,    63,
      50,   295,   331,   358,   436,   403,   317,   318,   177,   202,
     204,  -104,   206,   213,   121,     2,     3,   214,     4,     5,
       6,     7,     8,   455,   174,   216,   188,   175,   457,   212,
     176,   331,   -67,   215,   218,    18,   221,    20,   441,    21,
     223,   224,   122,   228,   229,   463,    22,   230,   269,   272,
     157,   158,   188,    26,   458,    27,   123,   469,   273,   276,
     281,   282,   474,    32,   283,    33,    34,   308,   315,   313,
     319,   188,   320,    35,   322,   171,   172,   173,   327,   329,
     332,   333,   188,   174,   335,   297,   175,    63,    50,   176,
     338,   477,   344,   348,    63,    50,   343,   346,    37,    38,
     360,   128,   363,   354,   359,   129,   130,   367,   361,   365,
     371,   396,   370,   372,   394,   369,   400,   401,   131,   413,
     132,    40,   393,   339,   415,   416,    41,    42,   395,   419,
     114,    43,   421,    44,   422,   433,   114,    45,   432,   438,
     450,   363,   451,   188,   452,   188,   456,   459,   467,    -3,
     460,   464,     1,     2,     3,   465,     4,     5,     6,     7,
       8,     9,    10,    11,    12,   470,   472,    13,    14,    15,
      16,   471,    17,    18,    19,   475,   120,    21,   476,   337,
     336,   424,   278,   366,    22,   124,    23,    24,   431,    25,
     219,    26,   356,    27,   434,    28,    29,    30,   435,   134,
      31,    32,   427,    33,    34,   417,   461,   418,   453,   412,
     462,    35,    36,    14,    15,    16,    90,    17,   382,    19,
       0,   399,     0,   444,   443,   448,   449,   157,   158,   427,
       0,    23,     0,     0,     0,     0,    37,    38,     0,   425,
      28,    29,    30,     0,     0,    39,   444,   443,   448,   449,
     169,   170,   171,   172,   173,     0,     0,     0,     0,    40,
     174,     0,     0,   175,    41,    42,   176,     0,     0,    43,
       0,    44,     0,    -4,     0,    45,     1,     2,     3,     0,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
     426,    13,    14,    15,    16,     0,    17,    18,    19,     0,
       0,    21,     0,     0,     0,     0,     0,     0,    22,     0,
      23,    24,     0,    25,     0,    26,     0,    27,     0,    28,
      29,    30,  -145,     0,    31,    32,     0,    33,    34,     0,
       0,     0,     0,     0,     0,    35,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    14,    15,
      16,     0,    17,     0,    19,     0,     0,     0,     0,     0,
      37,    38,     0,     0,     0,     0,    23,     0,     0,    39,
       0,     0,     0,     0,   425,    28,    29,    30,     0,     0,
       0,     0,     0,    40,     0,     0,     0,     0,    41,    42,
       0,     0,     0,    43,     0,    44,     1,     2,     3,    45,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    14,    15,    16,   179,    17,    18,    19,    20,
       0,    21,   180,     0,   122,   426,     0,     0,    22,   181,
      23,    24,     0,    25,   182,    26,     0,    27,   183,    28,
      29,    30,     0,     0,    31,    32,     0,    33,    34,     0,
       0,     0,     0,     0,     0,    35,    36,  -146,   184,    14,
      15,    16,     0,    17,     0,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     0,     0,
      37,    38,     0,     0,     0,     0,    28,    29,    30,   185,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,     0,     0,     0,     0,    41,    42,
       0,     0,     0,    43,     0,    44,     0,     0,     0,    45,
       0,   277,     1,     2,     3,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    14,    15,
      16,   179,    17,    18,    19,    20,     0,    21,   180,     0,
     122,     0,     0,     0,    22,   181,    23,    24,     0,    25,
     182,    26,     0,    27,   183,    28,    29,    30,     0,     0,
      31,    32,     0,    33,    34,     0,     0,     0,     0,     0,
       0,    35,    36,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,    38,     0,     0,
       0,     0,     0,     0,     0,   185,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
       0,     0,     0,     0,    41,    42,     0,     0,     0,    43,
       0,    44,     0,     0,     0,    45,     0,   330,     1,     2,
       3,     0,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    14,    15,    16,     0,    17,    18,
      19,     0,     0,    21,     0,   136,     0,     0,     0,     0,
      22,     0,    23,    24,     0,    25,     0,    26,     0,    27,
       0,    28,    29,    30,     0,     0,    31,    32,     0,    33,
      34,     0,  -205,  -205,  -205,     0,     0,    35,    36,   144,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,   158,     0,     0,   159,   160,   161,
       0,     0,    37,    38,     0,     0,     0,     0,     0,     0,
       0,   285,     0,   167,   168,     0,   169,   170,   171,   172,
     173,     0,     0,     0,     0,    40,   174,     0,     0,   175,
      41,    42,   176,     0,     0,    43,     0,    44,     0,     0,
       0,    45,     0,   -14,     1,     2,     3,     0,     4,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      14,    15,    16,     0,    17,    18,    19,     0,     0,    21,
       0,  -205,     0,     0,     0,     0,    22,     0,    23,    24,
       0,    25,     0,    26,     0,    27,     0,    28,    29,    30,
       0,     0,    31,    32,     0,    33,    34,     0,     0,     0,
       0,     0,     0,    35,    36,  -205,  -205,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   157,
     158,     0,     0,   159,   160,   161,     0,     0,    37,    38,
       0,     0,     0,     0,     0,     0,     0,   285,     0,  -205,
    -205,     0,   169,   170,   171,   172,   173,     0,     0,     0,
       0,    40,   174,     0,     0,   175,    41,    42,   176,     0,
       0,    43,     0,    44,     0,     0,     0,    45,     0,   -15,
       1,     2,     3,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    14,    15,    16,     0,
      17,    18,    19,     0,     0,    21,     0,     0,     0,     0,
       0,     0,    22,     0,    23,    24,     0,    25,     0,    26,
       0,    27,     0,    28,    29,    30,     0,     0,    31,    32,
       0,    33,    34,     0,     0,     0,     0,     0,     0,    35,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    38,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,     0,     0,
       0,     0,    41,    42,     0,     0,     0,    43,     0,    44,
       0,     0,     0,    45,     0,  -137,     1,     2,     3,     0,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    14,    15,    16,     0,    17,    18,    19,     0,
       0,    21,     0,     0,     0,     0,     0,     0,    22,     0,
      23,    24,     0,    25,     0,    26,     0,    27,     0,    28,
      29,    30,     0,     0,    31,    32,     0,    33,    34,     0,
       0,     0,     0,     0,     0,    35,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,     0,     0,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,     0,     0,     0,     0,    41,    42,
       0,     0,     0,    43,     0,    44,     0,     0,     0,    45,
       0,  -138,     1,     2,     3,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,     0,  -114,     0,    14,    15,
      16,   179,    17,    18,    19,    20,     0,    21,   180,     0,
     122,     0,     0,     0,    22,   181,    23,    24,     0,    25,
     182,    26,     0,    27,   183,    28,    29,    30,     0,     0,
      31,    32,     0,    33,    34,     0,     0,     0,     0,     0,
       0,    35,    36,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,    38,     0,     0,
       0,     0,     0,     0,     0,   185,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
       0,     0,     0,     0,    41,    42,     0,     0,     0,    43,
       0,    44,     1,     2,     3,    45,     4,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    14,    15,
      16,   179,    17,    18,    19,    20,     0,    21,   180,     0,
     122,     0,     0,     0,    22,   181,    23,    24,     0,    25,
     182,    26,     0,    27,   183,    28,    29,    30,     0,     0,
      31,    32,     0,    33,    34,     0,     0,     0,     0,     0,
       0,    35,    36,     0,   184,     0,     0,     0,     0,     0,
       0,     1,     2,     3,     0,     4,     5,     6,     7,     8,
       0,     0,     0,     0,     0,     0,    37,    38,     0,     0,
       0,     0,    18,     0,    20,   185,    21,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,    40,
      26,     0,    27,     0,    41,    42,     0,     0,     0,    43,
      32,   186,    33,    34,     0,    45,     0,     0,     0,     0,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,    41,    42,     0,     0,     0,    43,     0,
      44,     0,     0,     0,    45,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,     0,   157,   158,     0,     0,
     159,   160,   161,     0,     0,     0,     0,     0,   162,     0,
       0,   163,   136,   164,   165,   166,   167,   168,     0,   169,
     170,   171,   172,   173,     0,     0,     0,     0,     0,   174,
       0,     0,   175,     0,     0,   176,     0,     0,   305,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,     0,
     157,   158,     0,     0,   159,   160,   161,     0,     0,     0,
       0,     0,   162,     0,     0,   163,   136,   164,   165,   166,
     167,   168,     0,   169,   170,   171,   172,   173,     0,     0,
       0,     0,     0,   174,     0,     0,   175,     0,     0,   176,
       0,     0,   334,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,     0,   157,   158,     0,     0,   159,   160,
     161,     0,     0,     0,     0,     0,   162,   136,     0,   163,
     325,   164,   165,   166,   167,   168,     0,   169,   170,   171,
     172,   173,     0,     0,     0,     0,     0,   174,     0,     0,
     175,     0,     0,   176,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,     0,   157,   158,     0,     0,   159,
     160,   161,     0,     0,     0,     0,     0,   162,   136,     0,
     163,     0,   164,   165,   166,   167,   168,     0,   169,   170,
     171,   172,   173,     0,     0,     0,     0,     0,   174,   326,
       0,   175,     0,     0,   176,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,     0,   157,   158,     0,     0,
     159,   160,   161,     0,     0,     0,     0,     0,   162,   136,
       0,   163,  -204,   164,   165,   166,   167,   168,     0,   169,
     170,   171,   172,   173,     0,     0,     0,     0,     0,   174,
       0,     0,   175,     0,     0,   176,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,     0,   157,   158,     0,
       0,   159,   160,   161,     0,     0,     0,     0,     0,   162,
     136,     0,   163,   454,   164,   165,   166,   167,   168,     0,
     169,   170,   171,   172,   173,     0,     0,     0,     0,     0,
     174,     0,     0,   175,     0,     0,   176,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,     0,   157,   158,
       0,     0,   159,   160,   161,     0,     0,     0,     0,     0,
     162,   136,     0,   163,     0,   164,   165,   166,   167,   168,
       0,   169,   170,   171,   172,   173,     0,     0,     0,     0,
       0,   174,     0,     0,   175,     0,     0,   176,   137,   138,
     139,   140,   141,   142,   143,   144,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   156,     0,   157,
     158,     0,     0,   159,   160,   161,     0,     0,     0,     0,
       0,     0,   136,     0,     0,     0,   164,   165,   166,   167,
     168,     0,   169,   170,   171,   172,   173,     0,     0,     0,
       0,     0,   174,     0,     0,   175,     0,     0,   176,   137,
     138,   139,   140,   141,   142,   143,   144,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     157,   158,     0,     0,   159,   160,   161,     0,     0,     0,
       0,     0,     0,   136,     0,     0,     0,   164,   165,   166,
     167,   168,     0,   169,   170,   171,   172,   173,     0,     0,
       0,     0,     0,   174,     0,     0,   175,     0,     0,   176,
     137,   138,   139,   140,   141,   142,   143,   144,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   157,   158,     0,     0,   159,   160,   161,     0,     0,
       0,     0,     0,     0,   136,     0,     0,     0,     0,   165,
     166,   167,   168,     0,   169,   170,   171,   172,   173,     0,
       0,     0,     0,     0,   174,     0,     0,   175,     0,     0,
     176,   137,   138,   139,   140,   141,   142,   143,   144,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   157,   158,     0,     0,   159,   160,   161,     0,
       0,     0,     0,     0,     0,   136,     0,     0,     0,     0,
       0,   166,   167,   168,     0,   169,   170,   171,   172,   173,
       0,     0,     0,     0,     0,   174,     0,     0,   175,     0,
       0,   176,   137,   138,   139,   140,   141,   142,   143,   144,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,   158,     0,     0,   159,   160,   161,
       0,     0,     0,     0,     0,     0,   136,     0,     0,     0,
       0,     0,  -205,   167,   168,     0,   169,   170,   171,   172,
     173,     0,     0,     0,     0,     0,   174,     0,     0,   175,
       0,     0,   176,   137,   138,   139,  -205,  -205,  -205,  -205,
     144,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   157,   158,     0,     0,   159,   160,
     161,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,   168,     0,   169,   170,   171,
     172,   173,     0,     0,     0,     0,     0,   174,     0,     0,
     175,     0,     0,   176
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,    22,    25,    82,    53,   201,    97,
     103,    49,     3,   206,     3,    97,     0,    35,    44,    37,
      38,     3,    40,    41,    42,    43,    44,   298,    45,    34,
      99,     0,    13,     3,   103,    97,    18,   221,     3,     0,
     126,   129,    97,    67,    25,    50,   230,   129,    48,    48,
      48,    48,   359,    18,   126,     3,     4,     5,   302,     7,
       8,     9,    10,    11,    48,     4,     3,   129,     7,     8,
       9,     3,    11,   344,   129,    99,    24,    94,    26,    48,
      28,    18,   100,   176,   328,   103,    18,    48,   126,    27,
     122,   123,    26,     3,    32,     3,    44,    31,    35,    90,
       3,    90,     0,   347,    52,    44,    54,    55,    18,   202,
      18,    45,   126,   384,   358,    54,    55,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,    87,
      88,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   122,   176,   343,
     186,   178,   126,   470,   181,   182,   126,   123,   186,    33,
     126,    97,   120,    97,   122,   419,   113,   421,   126,   122,
     198,   113,    96,    97,   202,     3,     4,   213,   214,     3,
     216,    87,    88,    97,   211,   213,   214,    61,   216,   199,
     199,   394,   278,   129,   413,   129,   223,   224,   126,   126,
     126,   123,   123,    97,     3,     4,     5,   103,     7,     8,
       9,    10,    11,   432,   120,    97,   302,   123,   437,   121,
     126,   307,    67,   128,     3,    24,     3,    26,   422,    28,
      96,    67,    31,     3,     3,   454,    35,     3,     3,    17,
      87,    88,   328,    42,   438,    44,    45,   466,     3,     3,
      96,    13,   471,    52,    17,    54,    55,   129,    97,   103,
       3,   347,    53,    62,    53,   112,   113,   114,   129,     3,
     126,     3,   358,   120,   128,     3,   123,   287,   287,   126,
     129,   475,    97,   103,   294,   294,   129,   128,    87,    88,
     320,    26,   322,    99,   126,    30,    31,   325,    16,   122,
     122,   357,    99,   128,   123,   332,   122,    97,    43,   122,
      45,   110,   348,    99,   122,   126,   115,   116,   354,    64,
     348,   120,   129,   122,   129,   128,   354,   126,   103,     3,
       3,   361,    26,   419,   128,   421,   128,   122,     3,     0,
     128,   128,     3,     4,     5,   129,     7,     8,     9,    10,
      11,    12,    13,    14,    15,   126,   128,    18,    19,    20,
      21,   122,    23,    24,    25,   129,    48,    28,   128,   294,
     287,   401,   186,   324,    35,    53,    37,    38,   406,    40,
     122,    42,   315,    44,   409,    46,    47,    48,   409,    64,
      51,    52,   402,    54,    55,   390,   446,   390,   429,   382,
     451,    62,    63,    19,    20,    21,    18,    23,   340,    25,
      -1,   361,    -1,   423,   423,   423,   423,    87,    88,   429,
      -1,    37,    -1,    -1,    -1,    -1,    87,    88,    -1,    45,
      46,    47,    48,    -1,    -1,    96,   446,   446,   446,   446,
     110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,   110,
     120,    -1,    -1,   123,   115,   116,   126,    -1,    -1,   120,
      -1,   122,    -1,     0,    -1,   126,     3,     4,     5,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    -1,
      96,    18,    19,    20,    21,    -1,    23,    24,    25,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    40,    -1,    42,    -1,    44,    -1,    46,
      47,    48,   128,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    37,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    45,    46,    47,    48,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,   120,    -1,   122,     3,     4,     5,   126,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    -1,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    28,    29,    -1,    31,    96,    -1,    -1,    35,    36,
      37,    38,    -1,    40,    41,    42,    -1,    44,    45,    46,
      47,    48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,   128,    65,    19,
      20,    21,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    46,    47,    48,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,   120,    -1,   122,    -1,    -1,    -1,   126,
      -1,   128,     3,     4,     5,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    29,    -1,
      31,    -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    -1,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,   120,
      -1,   122,    -1,    -1,    -1,   126,    -1,   128,     3,     4,
       5,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    -1,    -1,    -1,    19,    20,    21,    -1,    23,    24,
      25,    -1,    -1,    28,    -1,    39,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    40,    -1,    42,    -1,    44,
      -1,    46,    47,    48,    -1,    -1,    51,    52,    -1,    54,
      55,    -1,    66,    67,    68,    -1,    -1,    62,    63,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    91,    92,    93,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,   110,   120,    -1,    -1,   123,
     115,   116,   126,    -1,    -1,   120,    -1,   122,    -1,    -1,
      -1,   126,    -1,   128,     3,     4,     5,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    -1,    -1,    -1,
      19,    20,    21,    -1,    23,    24,    25,    -1,    -1,    28,
      -1,    39,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,
      -1,    40,    -1,    42,    -1,    44,    -1,    46,    47,    48,
      -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    91,    92,    93,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,   107,
     108,    -1,   110,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,   110,   120,    -1,    -1,   123,   115,   116,   126,    -1,
      -1,   120,    -1,   122,    -1,    -1,    -1,   126,    -1,   128,
       3,     4,     5,    -1,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    -1,    -1,    -1,    19,    20,    21,    -1,
      23,    24,    25,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,    42,
      -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,   120,    -1,   122,
      -1,    -1,    -1,   126,    -1,   128,     3,     4,     5,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    -1,
      -1,    -1,    19,    20,    21,    -1,    23,    24,    25,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    40,    -1,    42,    -1,    44,    -1,    46,
      47,    48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,   120,    -1,   122,    -1,    -1,    -1,   126,
      -1,   128,     3,     4,     5,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    -1,    17,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    29,    -1,
      31,    -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    -1,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,   120,
      -1,   122,     3,     4,     5,   126,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    29,    -1,
      31,    -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    -1,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    24,    -1,    26,    96,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,   110,
      42,    -1,    44,    -1,   115,   116,    -1,    -1,    -1,   120,
      52,   122,    54,    55,    -1,   126,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    -1,   120,    -1,
     122,    -1,    -1,    -1,   126,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    -1,    -1,
      91,    92,    93,    -1,    -1,    -1,    -1,    -1,    99,    -1,
      -1,   102,    39,   104,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,   123,    -1,    -1,   126,    -1,    -1,   129,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    -1,    -1,    91,    92,    93,    -1,    -1,    -1,
      -1,    -1,    99,    -1,    -1,   102,    39,   104,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,   123,    -1,    -1,   126,
      -1,    -1,   129,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    -1,    -1,    91,    92,
      93,    -1,    -1,    -1,    -1,    -1,    99,    39,    -1,   102,
     103,   104,   105,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
     123,    -1,    -1,   126,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    -1,    -1,    91,
      92,    93,    -1,    -1,    -1,    -1,    -1,    99,    39,    -1,
     102,    -1,   104,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,   121,
      -1,   123,    -1,    -1,   126,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    -1,    -1,
      91,    92,    93,    -1,    -1,    -1,    -1,    -1,    99,    39,
      -1,   102,   103,   104,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,   123,    -1,    -1,   126,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    -1,
      -1,    91,    92,    93,    -1,    -1,    -1,    -1,    -1,    99,
      39,    -1,   102,   103,   104,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,   123,    -1,    -1,   126,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      -1,    -1,    91,    92,    93,    -1,    -1,    -1,    -1,    -1,
      99,    39,    -1,   102,    -1,   104,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,   123,    -1,    -1,   126,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    87,
      88,    -1,    -1,    91,    92,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,   104,   105,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,   123,    -1,    -1,   126,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    91,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,   104,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,   123,    -1,    -1,   126,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    91,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,   123,    -1,    -1,
     126,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    91,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,   123,    -1,
      -1,   126,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    91,    92,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,   123,
      -1,    -1,   126,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    91,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,    -1,   110,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
     123,    -1,    -1,   126
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    18,    19,    20,    21,    23,    24,    25,
      26,    28,    35,    37,    38,    40,    42,    44,    46,    47,
      48,    51,    52,    54,    55,    62,    63,    87,    88,    96,
     110,   115,   116,   120,   122,   126,   131,   132,   133,   134,
     140,   143,   149,   153,   154,   155,   156,   158,   166,   174,
     177,   180,   183,   184,   185,   186,   190,   192,   201,   208,
     211,   221,   222,   223,   226,   227,   228,   229,   232,    49,
     126,   126,   159,   126,     3,    18,   122,   178,   179,     3,
     212,     3,   210,   228,   126,     3,   179,   212,   213,   214,
     126,   122,   123,   126,   228,   126,   228,   228,   228,   228,
     228,   219,   220,   225,   228,   225,   230,   231,   226,     0,
     134,     3,    31,    45,   145,   151,   152,   227,    26,    30,
      31,    43,    45,   200,   186,    97,    39,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    87,    88,    91,
      92,    93,    99,   102,   104,   105,   106,   107,   108,   110,
     111,   112,   113,   114,   120,   123,   126,   126,   157,    22,
      29,    36,    41,    45,    65,    96,   122,   140,   141,   142,
     145,   160,   161,   176,   184,   224,   233,   234,   167,   182,
     122,   123,   126,   218,   126,   226,   123,   228,   175,     3,
     219,   150,   121,    97,   103,   128,    97,   129,     3,   146,
     147,     3,   146,    96,    67,    34,    50,   207,     3,     3,
       3,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,     3,
     219,   226,    17,     3,   226,   226,     3,   128,   139,   141,
     228,    96,    13,    17,   228,    96,   135,   136,   137,   143,
     190,   192,   201,   208,   181,   178,   219,     3,    90,   204,
     205,   206,   129,     3,   113,   129,   138,   139,   129,   226,
     225,   225,   225,   103,   217,    97,   217,   226,   226,     3,
      53,   187,    53,   188,   217,   103,   121,   129,   129,     3,
     128,   141,   126,     3,   129,   128,   137,   135,   129,    99,
     103,   202,   206,   129,    97,   142,   128,   129,   103,    35,
     113,   179,   214,   216,    99,   144,   147,    96,   129,   126,
     214,    16,   189,   214,   215,   122,   144,   228,   142,   226,
      99,   122,   128,     4,     7,     8,     9,    11,    44,    54,
      55,   203,   216,   217,    90,   206,    27,    32,   168,   170,
     172,   173,   142,   225,   123,   225,   227,   142,   204,   215,
     122,    97,   193,   129,     3,     4,    33,    61,   162,   163,
     164,   165,   202,   122,   206,   122,   126,   168,   170,    64,
     148,   129,   129,   191,   214,    45,    96,   184,   197,   198,
     199,   228,   103,   128,   164,   165,   138,   171,     3,   142,
     142,   217,    96,   140,   184,   194,   195,   196,   201,   208,
       3,    26,   128,   199,   103,   138,   128,   138,   217,   122,
     128,   196,   207,   138,   128,   129,   209,     3,   169,   138,
     126,   122,   128,   204,   138,   129,   128,   217
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
        
    case 24:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1544 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 25:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1545 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 26:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1547 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1548 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 38:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1563 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 45:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1571 "parser.y"
    {/*TODO*/(yyval.code)=0;}
    }
    break;


  
    case 46:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1572 "parser.y"
    {/*TODO*/(yyval.code)=0;}
    }
    break;


  
    case 47:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1574 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 48:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1575 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 49:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1576 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 50:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1577 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 51:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1581 "parser.y"
    {
    code_t**cc = &global->init->method->body->code;
    *cc = code_append(*cc, (yyvsp[(1) - (1)].code));
}
    }
    break;


  
    case 52:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1588 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (2)].value);}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1589 "parser.y"
    {(yyval.value).c=abc_pushundefined(0);
                                  (yyval.value).t=TYPE_ANY;
                                 }
    }
    break;


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1593 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 55:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1594 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1596 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1597 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1600 "parser.y"
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
            (yyval.code) = defaultvalue(0, (yyvsp[(2) - (3)].classinfo));
            (yyval.code) = abc_setlocal((yyval.code), index);
        }
    } else {
        if((yyvsp[(3) - (3)].value).c->prev || (yyvsp[(3) - (3)].value).c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = (yyvsp[(3) - (3)].value).c;
            (yyval.code) = abc_coerce_a((yyval.code));
            (yyval.code) = abc_setlocal((yyval.code), index);
        } else {
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


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1640 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1641 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1644 "parser.y"
    {new_state();}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1644 "parser.y"
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


  
    case 63:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1663 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1668 "parser.y"
    {
    (yyval.id)=(yyvsp[(2) - (3)].id);new_variable((yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1);
}
    }
    break;


  
    case 67:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1671 "parser.y"
    {
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 68:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1675 "parser.y"
    {new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 69:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1676 "parser.y"
    {new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1678 "parser.y"
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


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1698 "parser.y"
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


  
    case 72:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1741 "parser.y"
    {new_state();}
    }
    break;


  
    case 73:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1741 "parser.y"
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


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1760 "parser.y"
    {new_state();}
    }
    break;


  
    case 75:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1760 "parser.y"
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


  
    case 76:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1775 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1778 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 78:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1781 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1784 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 80:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1788 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1789 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1790 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1791 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1792 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1793 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1795 "parser.y"
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


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1806 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1809 "parser.y"
    {new_state();}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1809 "parser.y"
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


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1838 "parser.y"
    {new_state();state->exception_name=(yyvsp[(3) - (5)].id);new_variable((yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0);}
    }
    break;


  
    case 91:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1839 "parser.y"
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


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1858 "parser.y"
    {new_state();state->exception_name=0;}
    }
    break;


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1858 "parser.y"
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


  
    case 94:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1875 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 95:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1876 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 96:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1877 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1878 "parser.y"
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


  
    case 98:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1886 "parser.y"
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


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1895 "parser.y"
    {new_state();}
    }
    break;


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1895 "parser.y"
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


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1940 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 102:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1944 "parser.y"
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


  
    case 103:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1955 "parser.y"
    {
     (yyval.code) = (yyvsp[(3) - (5)].value).c;
     (yyval.code) = abc_pushscope((yyval.code));
     (yyval.code) = code_append((yyval.code), (yyvsp[(5) - (5)].code));
     (yyval.code) = abc_popscope((yyval.code));
}
    }
    break;


  
    case 105:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1965 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1967 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 107:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1968 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 108:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1970 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 109:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1971 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 110:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1972 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 111:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1973 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1975 "parser.y"
    {
       classinfo_t*c = (yyvsp[(2) - (2)].classinfo);
       if(!c) 
            syntaxerror("Couldn't import class\n");
       state_has_imports();
       dict_put(state->imports, c->name, c);
       (yyval.code)=0;
}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1983 "parser.y"
    {
       NEW(import_t,i);
       i->package = (yyvsp[(2) - (4)].id);
       state_has_imports();
       list_append(state->wildcard_imports, i);
       (yyval.code)=0;
}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1993 "parser.y"
    {PASS12 (yyval.flags)=0;}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1994 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1995 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].token);}
    }
    break;


  
    case 117:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1996 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (2)].flags)|(yyvsp[(2) - (2)].token);}
    }
    break;


  
    case 118:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1998 "parser.y"
    {PASS12 (yyval.token)=FLAG_PUBLIC;}
    }
    break;


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1999 "parser.y"
    {PASS12 (yyval.token)=FLAG_PRIVATE;}
    }
    break;


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2000 "parser.y"
    {PASS12 (yyval.token)=FLAG_PROTECTED;}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2001 "parser.y"
    {PASS12 (yyval.token)=FLAG_STATIC;}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2002 "parser.y"
    {PASS12 (yyval.token)=FLAG_DYNAMIC;}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2003 "parser.y"
    {PASS12 (yyval.token)=FLAG_FINAL;}
    }
    break;


  
    case 124:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2004 "parser.y"
    {PASS12 (yyval.token)=FLAG_OVERRIDE;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2005 "parser.y"
    {PASS12 (yyval.token)=FLAG_NATIVE;}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2006 "parser.y"
    {PASS12 (yyval.token)=FLAG_PACKAGEINTERNAL;}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2008 "parser.y"
    {(yyval.classinfo)=registry_getobjectclass();}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2009 "parser.y"
    {(yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2011 "parser.y"
    {(yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2012 "parser.y"
    {(yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2014 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2015 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2019 "parser.y"
    {PASS12 startclass((yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list), 0);}
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2021 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2025 "parser.y"
    {PASS12 startclass((yyvsp[(1) - (5)].flags),(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list),1);}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2027 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 144:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2039 "parser.y"
    {
    code_t*c = state->cls->static_init;
    c = code_append(c, (yyvsp[(1) - (1)].code));  
    state->cls->static_init = c;
}
    }
    break;


  
    case 150:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2050 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 151:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2053 "parser.y"
    {
    PASS12
    (yyvsp[(1) - (8)].flags) |= FLAG_PUBLIC;
    if((yyvsp[(1) - (8)].flags)&(FLAG_PRIVATE|FLAG_PACKAGEINTERNAL|FLAG_PROTECTED)) {
        syntaxerror("invalid method modifiers: interface methods always need to be public");
    }
    startfunction(0,(yyvsp[(1) - (8)].flags),(yyvsp[(3) - (8)].token),(yyvsp[(4) - (8)].id),&(yyvsp[(6) - (8)].params),(yyvsp[(8) - (8)].classinfo));
    endfunction(0,(yyvsp[(1) - (8)].flags),(yyvsp[(3) - (8)].token),(yyvsp[(4) - (8)].id),&(yyvsp[(6) - (8)].params),(yyvsp[(8) - (8)].classinfo), 0);
}
    }
    break;


  
    case 154:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2067 "parser.y"
    {
    int flags = (yyvsp[(1) - (5)].flags);
    memberinfo_t* info = state->cls?
            memberinfo_register(state->cls->info, (yyvsp[(3) - (5)].id), MEMBER_SLOT):
            memberinfo_register_global(flags2access((yyvsp[(1) - (5)].flags)), state->package, (yyvsp[(3) - (5)].id), MEMBER_SLOT);

    info->type = (yyvsp[(4) - (5)].classinfo);
    info->flags = flags;

    /* slot name */
    namespace_t mname_ns = {flags2access(flags), ""};
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


  
    case 155:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2126 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 156:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2127 "parser.y"
    {(yyval.constant)=(yyvsp[(2) - (2)].constant);}
    }
    break;


  
    case 157:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2129 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_uint));}
    }
    break;


  
    case 158:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2130 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 159:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2131 "parser.y"
    {(yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));}
    }
    break;


  
    case 160:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2132 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 161:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2133 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);}
    }
    break;


  
    case 162:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2135 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 163:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2136 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2137 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 165:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2142 "parser.y"
    {
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 166:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2145 "parser.y"
    {
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2150 "parser.y"
    {
    memset(&(yyval.params),0,sizeof((yyval.params)));
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(2) - (2)].param));
}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2155 "parser.y"
    {
    (yyval.params) =(yyvsp[(1) - (4)].params);
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(4) - (4)].param));
}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2162 "parser.y"
    {
    (yyval.params) = (yyvsp[(1) - (3)].params);
    list_append((yyval.params).list, (yyvsp[(3) - (3)].param));
}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2166 "parser.y"
    {
    memset(&(yyval.params),0,sizeof((yyval.params)));
    list_append((yyval.params).list, (yyvsp[(1) - (1)].param));
}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2171 "parser.y"
    {
     (yyval.param) = malloc(sizeof(param_t));
     (yyval.param)->name=(yyvsp[(1) - (4)].id);
     (yyval.param)->type = (yyvsp[(3) - (4)].classinfo);
     (yyval.param)->value = (yyvsp[(4) - (4)].constant);
}
    }
    break;


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2177 "parser.y"
    {
     (yyval.param) = malloc(sizeof(param_t));
     (yyval.param)->name=(yyvsp[(1) - (2)].id);
     (yyval.param)->type = TYPE_ANY;
     (yyval.param)->value = (yyvsp[(2) - (2)].constant);
}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2183 "parser.y"
    {(yyval.token)=(yyvsp[(1) - (1)].token);}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2184 "parser.y"
    {(yyval.token)=(yyvsp[(1) - (1)].token);}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2185 "parser.y"
    {(yyval.token)=0;}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2188 "parser.y"
    {PASS12 startfunction(0,(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2189 "parser.y"
    {
    PASS1 old_state();
    PASS2
    code_t*c = 0;
    if(state->method->late_binding) {
        c = abc_getlocal_0(c);
        c = abc_pushscope(c);
    }
    if(state->method->is_constructor && !state->method->has_super) {
        // call default constructor
        c = abc_getlocal_0(c);
        c = abc_constructsuper(c, 0);
    }
    c = wrap_function(c, 0, (yyvsp[(11) - (12)].code));

    endfunction(0,(yyvsp[(1) - (12)].flags),(yyvsp[(3) - (12)].token),(yyvsp[(4) - (12)].id),&(yyvsp[(6) - (12)].params),(yyvsp[(8) - (12)].classinfo),c);
    (yyval.code)=0;
}
    }
    break;


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2209 "parser.y"
    {(yyval.id)=0;}
    }
    break;


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2211 "parser.y"
    {
    syntaxerror("nested functions not supported yet");
}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2218 "parser.y"
    {
    PASS1 (yyval.classinfo)=0;
    PASS2
    /* try current package */
    (yyval.classinfo) = find_class((yyvsp[(1) - (1)].id));
    if(!(yyval.classinfo)) syntaxerror("Could not find class %s\n", (yyvsp[(1) - (1)].id));
}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2226 "parser.y"
    {
    PASS1 (yyval.classinfo)=0;
    PASS2
    (yyval.classinfo) = registry_findclass((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id));
    if(!(yyval.classinfo)) syntaxerror("Couldn't find class %s.%s\n", (yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id));
    free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;
}
    }
    break;


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2237 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 186:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2238 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2240 "parser.y"
    {(yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2241 "parser.y"
    {(yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2242 "parser.y"
    {(yyval.classinfo)=registry_getanytype();}
    }
    break;


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2251 "parser.y"
    {(yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 191:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2252 "parser.y"
    {(yyval.classinfo)=0;}
    }
    break;


  
    case 192:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2256 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 193:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2257 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 194:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2259 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2261 "parser.y"
    {(yyval.value_list).len=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2264 "parser.y"
    {
                                                  (yyval.value_list).len= (yyvsp[(1) - (3)].value_list).len+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (3)].value_list).cc, (yyvsp[(3) - (3)].value).c);
                                                  }
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2269 "parser.y"
    {
    MULTINAME(m, (yyvsp[(2) - (3)].classinfo));
    (yyval.value).c = code_new();

    if((yyvsp[(2) - (3)].classinfo)->slot) {
        (yyval.value).c = abc_getglobalscope((yyval.value).c);
        (yyval.value).c = abc_getslot((yyval.value).c, (yyvsp[(2) - (3)].classinfo)->slot);
    } else {
        (yyval.value).c = abc_findpropstrict2((yyval.value).c, &m);
    }

    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(3) - (3)].value_list).cc);

    if((yyvsp[(2) - (3)].classinfo)->slot)
        (yyval.value).c = abc_construct((yyval.value).c, (yyvsp[(3) - (3)].value_list).len);
    else
        (yyval.value).c = abc_constructprop2((yyval.value).c, &m, (yyvsp[(3) - (3)].value_list).len);
    (yyval.value).t = (yyvsp[(2) - (3)].classinfo);
}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2293 "parser.y"
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
        (yyval.value).c = abc_getlocal_0((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_call((yyval.value).c, (yyvsp[(3) - (4)].value_list).len);
    }
   
    memberinfo_t*f = 0;
   
    if(TYPE_IS_FUNCTION((yyvsp[(1) - (4)].value).t) && (yyvsp[(1) - (4)].value).t->function) {
        (yyval.value).t = (yyvsp[(1) - (4)].value).t->function->return_type;
    } else {
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_ANY;
    }

}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2341 "parser.y"
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


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2361 "parser.y"
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


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2382 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 203:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2385 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 204:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2392 "parser.y"
    {(yyval.value)=(yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 205:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2393 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2394 "parser.y"
    {
    (yyval.value).c = (yyvsp[(1) - (3)].value).c;
    (yyval.value).c = cut_last_push((yyval.value).c);
    (yyval.value).c = code_append((yyval.value).c,(yyvsp[(3) - (3)].value).c);
    (yyval.value).t = (yyvsp[(3) - (3)].value).t;
}
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2400 "parser.y"
    {
    (yyval.code)=cut_last_push((yyvsp[(1) - (1)].value).c);
}
    }
    break;


  
    case 208:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2406 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2410 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 211:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2412 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2414 "parser.y"
    {(yyval.value) = (yyvsp[(1) - (1)].value);}
    }
    break;


  
    case 213:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2416 "parser.y"
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


  
    case 214:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2433 "parser.y"
    {(yyval.value).c = abc_pushbyte(0, (yyvsp[(1) - (1)].number_uint));
                   //MULTINAME(m, registry_getintclass());
                   //$$.c = abc_coerce2($$.c, &m); // FIXME
                   (yyval.value).t = TYPE_INT;
                  }
    }
    break;


  
    case 215:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2438 "parser.y"
    {(yyval.value).c = abc_pushshort(0, (yyvsp[(1) - (1)].number_uint));
                    (yyval.value).t = TYPE_INT;
                   }
    }
    break;


  
    case 216:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2441 "parser.y"
    {(yyval.value).c = abc_pushint(0, (yyvsp[(1) - (1)].number_int));
                  (yyval.value).t = TYPE_INT;
                 }
    }
    break;


  
    case 217:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2444 "parser.y"
    {(yyval.value).c = abc_pushuint(0, (yyvsp[(1) - (1)].number_uint));
                   (yyval.value).t = TYPE_UINT;
                  }
    }
    break;


  
    case 218:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2447 "parser.y"
    {(yyval.value).c = abc_pushdouble(0, (yyvsp[(1) - (1)].number_float));
                    (yyval.value).t = TYPE_FLOAT;
                   }
    }
    break;


  
    case 219:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2450 "parser.y"
    {(yyval.value).c = abc_pushstring2(0, &(yyvsp[(1) - (1)].str));
                     (yyval.value).t = TYPE_STRING;
                    }
    }
    break;


  
    case 220:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2453 "parser.y"
    {(yyval.value).c = abc_pushundefined(0);
                    (yyval.value).t = TYPE_ANY;
                   }
    }
    break;


  
    case 221:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2456 "parser.y"
    {(yyval.value).c = abc_pushtrue(0);
                    (yyval.value).t = TYPE_BOOLEAN;
                   }
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2459 "parser.y"
    {(yyval.value).c = abc_pushfalse(0);
                     (yyval.value).t = TYPE_BOOLEAN;
                    }
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2462 "parser.y"
    {(yyval.value).c = abc_pushnull(0);
                    (yyval.value).t = TYPE_NULL;
                   }
    }
    break;


  
    case 225:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2467 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2470 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);
             (yyval.value).t = TYPE_BOOLEAN;
            }
    }
    break;


  
    case 227:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2473 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterthan((yyval.value).c);(yyval.value).c=abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 228:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2476 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_greaterequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 229:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2479 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 230:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2482 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
              }
    }
    break;


  
    case 231:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2485 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_strictequals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2488 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);(yyval.value).c = abc_equals((yyval.value).c);(yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2492 "parser.y"
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


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2503 "parser.y"
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


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2521 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_not((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2526 "parser.y"
    {(yyval.value).c=(yyvsp[(2) - (2)].value).c;
              (yyval.value).c = abc_bitnot((yyval.value).c);
              (yyval.value).t = TYPE_INT;
             }
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2531 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitand((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2536 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitxor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2541 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_bitor((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2546 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_rshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2550 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_urshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 242:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2554 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_lshift((yyval.value).c);
             (yyval.value).t = TYPE_INT;
            }
    }
    break;


  
    case 243:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2559 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_divide((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2563 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
             (yyval.value).c = abc_modulo((yyval.value).c);
             (yyval.value).t = TYPE_NUMBER;
            }
    }
    break;


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2567 "parser.y"
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


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2576 "parser.y"
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


  
    case 247:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2585 "parser.y"
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


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2595 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c,(yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_in((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2600 "parser.y"
    {char use_astype=0; // flash player's astype works differently than astypelate
              if(use_astype && TYPE_IS_CLASS((yyvsp[(3) - (3)].value).t)) {
                MULTINAME(m,(yyvsp[(3) - (3)].value).t->cls);
                (yyval.value).c = abc_astype2((yyvsp[(1) - (3)].value).c, &m);
                (yyval.value).t = (yyvsp[(3) - (3)].value).t->cls;
              } else {
                (yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
                (yyval.value).c = abc_astypelate((yyval.value).c);
                (yyval.value).t = TYPE_ANY;
              }
             }
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2613 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_instanceof((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2618 "parser.y"
    {(yyval.value).c = code_append((yyvsp[(1) - (3)].value).c, (yyvsp[(3) - (3)].value).c);
              (yyval.value).c = abc_istypelate((yyval.value).c);
              (yyval.value).t = TYPE_BOOLEAN;
             }
    }
    break;


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2623 "parser.y"
    {
              (yyval.value).c = (yyvsp[(3) - (4)].value).c;
              (yyval.value).c = abc_typeof((yyval.value).c);
              (yyval.value).t = TYPE_STRING;
             }
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2629 "parser.y"
    {
              (yyval.value).c = cut_last_push((yyvsp[(2) - (2)].value).c);
              (yyval.value).c = abc_pushundefined((yyval.value).c);
              (yyval.value).t = TYPE_ANY;
             }
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2635 "parser.y"
    { (yyval.value).c = abc_pushundefined(0);
             (yyval.value).t = TYPE_ANY;
           }
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2639 "parser.y"
    {(yyval.value)=(yyvsp[(2) - (3)].value);}
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2641 "parser.y"
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


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2652 "parser.y"
    {
  (yyval.value).c = (yyvsp[(1) - (4)].value).c;
  (yyval.value).c = code_append((yyval.value).c, (yyvsp[(3) - (4)].value).c);

  MULTINAME_LATE(m, (yyvsp[(1) - (4)].value).t?(yyvsp[(1) - (4)].value).t->access:ACCESS_PACKAGE, "");
  (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
  (yyval.value).t = 0; // array elements have unknown type
}
    }
    break;


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2661 "parser.y"
    {
    (yyval.value).c = code_new();
    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(2) - (3)].value_list).cc);
    (yyval.value).c = abc_newarray((yyval.value).c, (yyvsp[(2) - (3)].value_list).len);
    (yyval.value).t = registry_getarrayclass();
}
    }
    break;


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2668 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).len=0;}
    }
    break;


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2669 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2671 "parser.y"
    {
    (yyval.value_list).cc = 0;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(1) - (3)].value).c);
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (3)].value).c);
    (yyval.value_list).len = 2;
}
    }
    break;


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2677 "parser.y"
    {
    (yyval.value_list).cc = (yyvsp[(1) - (5)].value_list).cc;
    (yyval.value_list).len = (yyvsp[(1) - (5)].value_list).len+2;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (5)].value).c);
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(5) - (5)].value).c);
}
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2686 "parser.y"
    {
    (yyval.value).c = code_new();
    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(2) - (3)].value_list).cc);
    (yyval.value).c = abc_newobject((yyval.value).c, (yyvsp[(2) - (3)].value_list).len/2);
    (yyval.value).t = registry_getobjectclass();
}
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2693 "parser.y"
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


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2705 "parser.y"
    { 
               code_t*c = abc_modulo((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '%'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2711 "parser.y"
    { 
               code_t*c = abc_lshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '<'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2717 "parser.y"
    { 
               code_t*c = abc_rshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '>'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2723 "parser.y"
    { 
               code_t*c = abc_urshift((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, 'U'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 269:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2729 "parser.y"
    { 
               code_t*c = abc_divide((yyvsp[(3) - (3)].value).c);
               c=converttype(c, join_types((yyvsp[(1) - (3)].value).t, (yyvsp[(3) - (3)].value).t, '/'), (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2735 "parser.y"
    { 
               code_t*c = abc_bitor((yyvsp[(3) - (3)].value).c);
               c=converttype(c, TYPE_INT, (yyvsp[(1) - (3)].value).t);
               (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 0, 0);
               (yyval.value).t = (yyvsp[(1) - (3)].value).t;
              }
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2741 "parser.y"
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


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2754 "parser.y"
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


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2765 "parser.y"
    { code_t*c = 0;
              c = code_append(c, (yyvsp[(3) - (3)].value).c);
              c = converttype(c, (yyvsp[(3) - (3)].value).t, (yyvsp[(1) - (3)].value).t);
              (yyval.value).c = toreadwrite((yyvsp[(1) - (3)].value).c, c, 1, 0);
              (yyval.value).t = (yyvsp[(1) - (3)].value).t;
            }
    }
    break;


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2772 "parser.y"
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


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2785 "parser.y"
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


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2812 "parser.y"
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


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2826 "parser.y"
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


  
    case 278:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2840 "parser.y"
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


  
    case 279:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2855 "parser.y"
    { if(!state->cls->info)
                  syntaxerror("super keyword not allowed outside a class");
              classinfo_t*t = state->cls->info->superclass;
              if(!t) t = TYPE_OBJECT;

              memberinfo_t*f = registry_findmember(t, (yyvsp[(3) - (3)].id), 1);
              namespace_t ns = flags2namespace(f->flags, "");
              MEMBER_MULTINAME(m, f, (yyvsp[(3) - (3)].id));
              (yyval.value).c = 0;
              (yyval.value).c = abc_getlocal_0((yyval.value).c);
              (yyval.value).c = abc_getsuper2((yyval.value).c, &m);
              (yyval.value).t = memberinfo_gettype(f);
           }
    }
    break;


  
    case 280:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2870 "parser.y"
    {(yyval.value).c = (yyvsp[(1) - (3)].value).c;
             classinfo_t*t = (yyvsp[(1) - (3)].value).t;
             char is_static = 0;
             if(TYPE_IS_CLASS(t) && t->cls) {
                 t = t->cls;
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
                 (yyval.value).t = memberinfo_gettype(f);
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


  
    case 281:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2903 "parser.y"
    {
    (yyval.value).t = 0;
    (yyval.value).c = 0;
    classinfo_t*a = 0;
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

        if(f->kind == MEMBER_METHOD) {
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
            namespace_t ns = {flags2access(f->flags), ""};
            multiname_t m = {QNAME, &ns, 0, (yyvsp[(1) - (1)].id)};
            (yyval.value).c = abc_findpropstrict2((yyval.value).c, &m);
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
            break;
        } else if(f->slot>0) {
            (yyval.value).c = abc_getlocal_0((yyval.value).c);
            (yyval.value).c = abc_getslot((yyval.value).c, f->slot);
            break;
        } else {
            namespace_t ns = {flags2access(f->flags), ""};
            multiname_t m = {QNAME, &ns, 0, (yyvsp[(1) - (1)].id)};
            (yyval.value).c = abc_getlocal_0((yyval.value).c);
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
            break;
        }
    } 
    
    /* look at actual classes, in the current package and imported */
    if((a = find_class((yyvsp[(1) - (1)].id)))) {
        if(a->flags & FLAG_METHOD) {
            MULTINAME(m, a);
            (yyval.value).c = abc_findpropstrict2((yyval.value).c, &m);
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
            if(a->function->kind == MEMBER_METHOD) {
                (yyval.value).t = TYPE_FUNCTION(a->function);
            } else {
                (yyval.value).t = a->function->type;
            }
        } else {
            if(a->slot) {
                (yyval.value).c = abc_getglobalscope((yyval.value).c);
                (yyval.value).c = abc_getslot((yyval.value).c, a->slot);
            } else {
                MULTINAME(m, a);
                (yyval.value).c = abc_getlex2((yyval.value).c, &m);
            }
            (yyval.value).t = TYPE_CLASS(a);
        }
        break;
    }

    /* unknown object, let the avm2 resolve it */
    if(1) {
        if(strcmp((yyvsp[(1) - (1)].id),"trace"))
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


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3001 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3002 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3003 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 3005 "parser.y"
    {(yyval.token)=0;}
    }
    break;




/* Line 1464 of skeleton.m4  */
#line 7117 "parser.tab.c"
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



