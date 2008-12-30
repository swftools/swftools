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

%}

//%glr-parser
//%expect-rr 1
%error-verbose

%union tokenunion {
    enum yytokentype token;
    int flags;

    classinfo_t*classinfo;
    classinfo_list_t*classinfo_list;

    int number_int;
    unsigned int number_uint;
    double number_float;
    code_t*code;
    typedcode_t value;
    typedcode_list_t*value_list;
    param_t* param;
    params_t params;
    string_t str;
    char*id;
    constant_t*constant;
}


%token<id> T_IDENTIFIER
%token<str> T_STRING
%token<token> T_REGEXP
%token<token> T_EMPTY
%token<number_int> T_INT
%token<number_uint> T_UINT
%token<number_uint> T_BYTE
%token<number_uint> T_SHORT
%token<number_float> T_FLOAT

%token<token> KW_IMPLEMENTS
%token<token> KW_NAMESPACE "namespace"
%token<token> KW_PACKAGE "package"
%token<token> KW_PROTECTED
%token<token> KW_PUBLIC
%token<token> KW_PRIVATE
%token<token> KW_USE "use"
%token<token> KW_INTERNAL
%token<token> KW_NEW "new"
%token<token> KW_NATIVE
%token<token> KW_FUNCTION "function"
%token<token> KW_UNDEFINED "undefined"
%token<token> KW_FOR "for"
%token<token> KW_CLASS "class"
%token<token> KW_CONST "const"
%token<token> KW_SET "set"
%token<token> KW_VOID "void"
%token<token> KW_STATIC
%token<token> KW_IMPORT "import"
%token<token> KW_RETURN "return"
%token<token> KW_TYPEOF "typeof"
%token<token> KW_INTERFACE "interface"
%token<token> KW_NULL "null"
%token<token> KW_VAR "var"
%token<token> KW_DYNAMIC
%token<token> KW_OVERRIDE
%token<token> KW_FINAL
%token<token> KW_GET "get"
%token<token> KW_EXTENDS
%token<token> KW_FALSE "false"
%token<token> KW_TRUE "true"
%token<token> KW_BOOLEAN "Boolean"
%token<token> KW_UINT "uint"
%token<token> KW_INT "int"
%token<token> KW_WHILE "while"
%token<token> KW_NUMBER "Number"
%token<token> KW_STRING "String"
%token<token> KW_DELETE "delete"
%token<token> KW_IF "if"
%token<token> KW_ELSE  "else"
%token<token> KW_BREAK   "break"
%token<token> KW_IS "is"
%token<token> KW_AS "as"

%token<token> T_EQEQ "=="
%token<token> T_EQEQEQ "==="
%token<token> T_NE "!="
%token<token> T_NEE "!=="
%token<token> T_LE "<="
%token<token> T_GE ">="
%token<token> T_DIVBY "/=" 
%token<token> T_MODBY "%="
%token<token> T_MULBY "*="
%token<token> T_PLUSBY "+=" 
%token<token> T_MINUSBY "-="
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

%type <id> X_IDENTIFIER PACKAGE
%type <token> VARCONST
%type <code> CODE
%type <code> CODEPIECE
%type <code> CODEBLOCK MAYBECODE
%type <token> PACKAGE_DECLARATION
%type <token> FUNCTION_DECLARATION
%type <code> VARIABLE_DECLARATION ONE_VARIABLE VARIABLE_LIST
%type <token> CLASS_DECLARATION
%type <token> NAMESPACE_DECLARATION
%type <token> INTERFACE_DECLARATION
%type <code> VOIDEXPRESSION
%type <value> EXPRESSION NONCOMMAEXPRESSION
%type <value> MAYBEEXPRESSION
%type <value> E DELETE
%type <value> CONSTANT
%type <code> FOR IF WHILE MAYBEELSE BREAK RETURN
%type <token> USE_NAMESPACE
%type <code> FOR_INIT
%type <token> IMPORT
%type <classinfo> MAYBETYPE
%type <token> GETSET
%type <param> PARAM
%type <params> PARAM_LIST
%type <params> MAYBE_PARAM_LIST
%type <flags> MAYBE_MODIFIERS
%type <flags> MODIFIER_LIST
%type <constant> STATICCONSTANT MAYBESTATICCONSTANT
%type <classinfo_list> IMPLEMENTS_LIST
%type <classinfo> EXTENDS
%type <classinfo_list> EXTENDS_LIST
%type <classinfo> CLASS PACKAGEANDCLASS QNAME
%type <classinfo_list> QNAME_LIST
%type <classinfo> TYPE
%type <token> VAR
//%type <token> VARIABLE
%type <value> VAR_READ
%type <value> NEW
//%type <token> T_IDENTIFIER
%type <token> MODIFIER
%type <value> FUNCTIONCALL
%type <value_list> MAYBE_EXPRESSION_LIST EXPRESSION_LIST MAYBE_PARAM_VALUES

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
%nonassoc '|'
%nonassoc '^'
%nonassoc '&'
%nonassoc "==" "!=" "===" "!=="
%nonassoc "is" "as"
%nonassoc "<=" '<' ">=" '>' "instanceof" // TODO: support "a < b < c" syntax?
%left "<<" ">>" ">>>" 
%left below_minus
%left '-' '+'
%left '/' '*' '%'
%left plusplus_prefix minusminus_prefix '~' '!' "void" "delete" "typeof" //FIXME: *unary* + - should be here, too
%left "--" "++" 
%left '[' ']' '{' "new" '.' ".." "::"
%nonassoc T_IDENTIFIER
%left below_else
%nonassoc "else"
%left '('

// needed for "return" precedence:
%nonassoc T_STRING T_REGEXP
%nonassoc T_INT T_UINT T_BYTE T_SHORT T_FLOAT
%nonassoc "false" "true" "null" "undefined"

     
%{

static int yyerror(char*s)
{
   syntaxerror("%s", s); 
}
static char* concat3str(const char* t1, const char* t2, const char* t3)
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

typedef struct _state {
    abc_file_t*file;
    abc_script_t*init;

    int level;

    char*package;     
    char*function;
    /* code that needs to be executed at the start of
       a method (like initializing local registers) */
    code_t*initcode;

    import_list_t*wildcard_imports;
    dict_t*imports;
    char has_own_imports;
   
    /* class data */
    classinfo_t*clsinfo;
    abc_class_t*cls;
    code_t*cls_init;
    code_t*cls_static_init;
    
    /* method data */
    memberinfo_t*minfo;
    abc_method_t*m;

    dict_t*vars;
    char late_binding;
} state_t;

typedef struct _global {
    int variable_count;
} global_t;

static global_t*global = 0;
static state_t* state = 0;

DECLARE_LIST(state);

#define MULTINAME(m,x) multiname_t m;namespace_t m##_ns;registry_fill_multiname(&m, &m##_ns, x);

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

static state_list_t*state_stack=0;
    
static void init_globals()
{
    global = rfx_calloc(sizeof(global_t));
}

static void new_state()
{
    NEW(state_t, s);
    NEW(state_list_t, sl);

    state_t*oldstate = state;
    if(state)
        memcpy(s, state, sizeof(state_t)); //shallow copy
    sl->next = state_stack;
    sl->state = s;
    if(!s->imports) {
        s->imports = dict_new();
    }
    state_stack = sl;
    state = s;
    state->level++;
    state->vars = dict_new();
    state->initcode = 0;
    state->has_own_imports = 0;
}
static void state_has_imports()
{
    state->wildcard_imports = list_clone(state->wildcard_imports);
    state->imports = dict_clone(state->imports);
    state->has_own_imports = 1;
}

static void old_state()
{
    if(!state_stack || !state_stack->next)
        syntaxerror("invalid nesting");
    state_t*oldstate = state;
    state_list_t*old = state_stack;
    state_stack = state_stack->next;
    free(old);
    state = state_stack->state;
    /*if(state->initcode) {
        printf("residual initcode\n");
        code_dump(state->initcode, 0, 0, "", stdout);
    }*/
    if(oldstate->has_own_imports) {
        list_free(oldstate->wildcard_imports);
        dict_destroy(oldstate->imports);oldstate->imports=0;
    }
    state->initcode = code_append(state->initcode, oldstate->initcode);
}
void initialize_state()
{
    init_globals();
    new_state();

    state->file = abc_file_new();
    state->file->flags &= ~ABCFILE_LAZY;
    
    state->init = abc_initscript(state->file, 0);
    code_t*c = state->init->method->body->code;

    c = abc_getlocal_0(c);
    c = abc_pushscope(c);
  
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

    c = abc_findpropstrict(c, "[package]::trace");
    c = abc_pushstring(c, "[entering global init function]");
    c = abc_callpropvoid(c, "[package]::trace", 1);
    
    state->init->method->body->code = c;
}
void* finalize_state()
{
    if(state->level!=1) {
        syntaxerror("unexpected end of file");
    }
    abc_method_body_t*m = state->init->method->body;
    //__ popscope(m);
    
    __ findpropstrict(m, "[package]::trace");
    __ pushstring(m, "[leaving global init function]");
    __ callpropvoid(m, "[package]::trace", 1);
    __ returnvoid(m);
    return state->file;
}


static void startpackage(char*name)
{
    if(state->package) {
        syntaxerror("Packages can not be nested."); 
    } 
    new_state();
    /*printf("entering package \"%s\"\n", name);*/
    state->package = name;
}
static void endpackage()
{
    /*printf("leaving package \"%s\"\n", state->package);*/
    old_state();
}

char*globalclass=0;
static void startclass(int flags, char*classname, classinfo_t*extends, classinfo_list_t*implements, char interface)
{
    if(state->cls) {
        syntaxerror("inner classes now allowed"); 
    }
    new_state();

    token_list_t*t=0;
    classinfo_list_t*mlist=0;
    /*printf("entering class %s\n", name);
    printf("  modifiers: ");for(t=modifiers->tokens;t;t=t->next) printf("%s ", t->token);printf("\n");
    if(extends) 
        printf("  extends: %s.%s\n", extends->package, extends->name);
    printf("  implements (%d): ", list_length(implements));
    for(mlist=implements;mlist;mlist=mlist->next)  {
        printf("%s ", mlist->classinfo?mlist->classinfo->name:0);
    }
    printf("\n");
    */

    if(flags&~(FLAG_INTERNAL|FLAG_PUBLIC|FLAG_FINAL))
        syntaxerror("invalid modifier(s)");

    if((flags&(FLAG_PUBLIC|FLAG_INTERNAL)) == (FLAG_PUBLIC|FLAG_INTERNAL))
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

    if(registry_findclass(package, classname)) {
        syntaxerror("Package \"%s\" already contains a class called \"%s\"", package, classname);
    }
   
    /* build info struct */
    int num_interfaces = (list_length(implements));
    state->clsinfo = classinfo_register(access, package, classname, num_interfaces);
    state->clsinfo->superclass = extends;
    int pos = 0;
    classinfo_list_t*l = implements;
    for(l=implements;l;l=l->next) {
        state->clsinfo->interfaces[pos++] = l->classinfo;
    }

    MULTINAME(classname2,state->clsinfo);
    
    multiname_t*extends2 = sig2mname(extends);

    /*if(extends) {
        state->cls_init = abc_getlocal_0(state->cls_init);
        state->cls_init = abc_constructsuper(state->cls_init, 0);
    }*/

    state->cls = abc_class_new(state->file, &classname2, extends2);
    if(flags&FLAG_FINAL) abc_class_final(state->cls);
    if(flags&FLAG_DYNAMIC) abc_class_sealed(state->cls);
    if(interface) abc_class_interface(state->cls);

    for(mlist=implements;mlist;mlist=mlist->next) {
        MULTINAME(m, mlist->classinfo);
        abc_class_add_interface(state->cls, &m);
    }

    /* now write the construction code for this class */
    int slotindex = abc_initscript_addClassTrait(state->init, &classname2, state->cls);

    abc_method_body_t*m = state->init->method->body;
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
    __ newclass(m,state->cls);
    while(count--) {
        __ popscope(m);
    }
    __ setslot(m, slotindex);

    /* flash.display.MovieClip handling */
    if(!globalclass && (flags&FLAG_PUBLIC) && classinfo_equals(registry_getMovieClip(),extends)) {
        if(state->package && state->package[0]) {
            globalclass = concat3str(state->package, ".", classname);
        } else {
            globalclass = strdup(classname);
        }
    }
    multiname_destroy(extends2);
}

static void endclass()
{
    if(state->cls_init) {
        if(!state->cls->constructor) {
            abc_method_t*m = abc_class_constructor(state->cls, 0);
            m->body->code = code_append(m->body->code, state->cls_init);
            m->body->code = abc_returnvoid(m->body->code);
        } else {
            code_t*c = state->cls->constructor->body->code;
            c = code_append(state->cls_init, c);
            state->cls->constructor->body->code = c;

        }
    }
    if(state->cls_static_init) {
        if(!state->cls->static_constructor) {
            abc_method_t*m = abc_class_staticconstructor(state->cls, 0);
            m->body->code = code_append(m->body->code, state->cls_static_init);
            m->body->code = abc_returnvoid(m->body->code);
        } else {
            state->cls->static_constructor->body->code = 
                code_append(state->cls_static_init, state->cls->static_constructor->body->code);
        }
    }

    old_state();
}

typedef struct _variable {
    int index;
    classinfo_t*type;
} variable_t;

static int find_variable(char*name, classinfo_t**m)
{
    state_list_t* s = state_stack;
    while(s) {
        variable_t*v = dict_lookup(s->state->vars, name);
        if(v) {
            if(m) {
                *m = v->type;
            }
            return v->index;
        }
        s = s->next;
    }
    return -1;
} 
static int find_variable_safe(char*name, classinfo_t**m)
{
    int i = find_variable(name, m);
    if(i<0)
        syntaxerror("undefined variable: %s", name);
    return i;
}
static char variable_exists(char*name) 
{
    return dict_lookup(state->vars, name)!=0;
}
static int new_variable(char*name, classinfo_t*type)
{
    NEW(variable_t, v);
    v->index = global->variable_count;
    v->type = type;
    dict_put(state->vars, name, v);
    return global->variable_count++;
}
#define TEMPVARNAME "__as3_temp__"
static int gettempvar()
{
    int i = find_variable(TEMPVARNAME, 0);
    if(i<0) {
        return new_variable(TEMPVARNAME, 0);
    } else {
        return i;
    }
}

code_t* killvars(code_t*c) 
{
    int t;
    for(t=0;t<state->vars->hashsize;t++) {
        dictentry_t*e =state->vars->slots[t];
        while(e) {
            variable_t*v = (variable_t*)e->data;
            //do this always, otherwise register types don't match
            //in the verifier when doing nested loops
            //if(!TYPE_IS_BUILTIN_SIMPLE(type)) {
            c = abc_kill(c, v->index);
            e = e->next;
        }
    }
    return c;
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

static memberinfo_t*registerfunction(enum yytokentype getset, int flags, char*name, params_t*params, classinfo_t*return_type, int slot)
{
    memberinfo_t*minfo = 0;
    if(getset != KW_GET && getset != KW_SET) {
        if(registry_findmember(state->clsinfo, name)) {
            syntaxerror("class already contains a member/method called '%s'", name);
        }
        minfo = memberinfo_register(state->clsinfo, name, MEMBER_METHOD);
        minfo->return_type = return_type;
        // getslot on a member slot only returns "undefined", so no need
        // to actually store these
        //state->minfo->slot = state->m->method->trait->slot_id;
    } else {
        int gs = getset==KW_GET?MEMBER_GET:MEMBER_SET;
        classinfo_t*type=0;
        if(getset == KW_GET)
            type = return_type;
        else if(params->list)
            type = params->list->param->type;
        if((minfo=registry_findmember(state->clsinfo, name))) {
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
            minfo = memberinfo_register(state->clsinfo, name, gs);
            minfo->type = type;
        }
        /* can't assign a slot as getter and setter might have different slots */
        //minfo->slot = slot;
    }
    if(flags&FLAG_STATIC) minfo->flags |= FLAG_STATIC;
    if(flags&FLAG_PUBLIC) minfo->flags |= FLAG_PUBLIC;
    if(flags&FLAG_PRIVATE) minfo->flags |= FLAG_PRIVATE;
    if(flags&FLAG_PROTECTED) minfo->flags |= FLAG_PROTECTED;
    if(flags&FLAG_INTERNAL) minfo->flags |= FLAG_INTERNAL;
    return minfo;
}

static int flags2access(int flags)
{
    int access = 0;
    if(flags&FLAG_PUBLIC)  {
        if(access&(FLAG_PRIVATE|FLAG_PROTECTED|FLAG_INTERNAL)) syntaxerror("invalid combination of access levels");
        access = ACCESS_PACKAGE;
    } else if(flags&FLAG_PRIVATE) {
        if(access&(FLAG_PUBLIC|FLAG_PROTECTED|FLAG_INTERNAL)) syntaxerror("invalid combination of access levels");
        access = ACCESS_PRIVATE;
    } else if(flags&FLAG_PROTECTED) {
        if(access&(FLAG_PUBLIC|FLAG_PRIVATE|FLAG_INTERNAL)) syntaxerror("invalid combination of access levels");
        access = ACCESS_PROTECTED;
    } else {
        access = ACCESS_PACKAGEINTERNAL;
    }
    return access;
}

static void startfunction(token_t*ns, int flags, enum yytokentype getset, char*name,
                          params_t*params, classinfo_t*return_type)
{
    token_list_t*t;
    new_state();
    global->variable_count = 0;
    state->function = name;
    
    if(state->m) {
        syntaxerror("not able to start another method scope");
    }

    namespace_t mname_ns = {flags2access(flags), ""};
    multiname_t mname = {QNAME, &mname_ns, 0, name};

    multiname_t*type2 = sig2mname(return_type);
    int slot = 0;
    if(!strcmp(state->clsinfo->name,name)) {
        state->m = abc_class_constructor(state->cls, type2);
        name = "__as3_constructor__";
    } else {
        if(flags&FLAG_STATIC)
            state->m = abc_class_staticmethod(state->cls, type2, &mname);
        else
            state->m = abc_class_method(state->cls, type2, &mname);
        slot = state->m->trait->slot_id;
    }
    state->minfo = registerfunction(getset, flags, name, params, return_type, slot);

    if(getset == KW_GET) state->m->trait->kind = TRAIT_GETTER;
    if(getset == KW_SET) state->m->trait->kind = TRAIT_SETTER;
    if(params->varargs) state->m->flags |= METHOD_NEED_REST;

    char opt=0;
    param_list_t*p=0;
    for(p=params->list;p;p=p->next) {
        if(params->varargs && !p->next) {
            break; //varargs: omit last parameter in function signature
        }
        multiname_t*m = sig2mname(p->param->type);
	list_append(state->m->parameters, m);
        if(p->param->value) {
            check_constant_against_type(p->param->type, p->param->value);
            opt=1;list_append(state->m->optional_parameters, p->param->value);
        } else if(opt) {
            syntaxerror("non-optional parameter not allowed after optional parameters");
        }
    }

    /* state->vars is initialized by state_new */
    if(new_variable((flags&FLAG_STATIC)?"class":"this", state->clsinfo)!=0) syntaxerror("Internal error");

    for(p=params->list;p;p=p->next) {
        new_variable(p->param->name, p->param->type);
    }
}
static void endfunction(code_t*body)
{
        
    if(!(state->cls->flags & CLASS_INTERFACE)) {
        code_t*c = 0;
        if(state->late_binding) {
            c = abc_getlocal_0(c);
            c = abc_pushscope(c);
        }
        c = code_append(c, state->initcode);
        c = code_append(c, body);

        /* append return if necessary */
        if(!c || c->opcode != OPCODE_RETURNVOID && 
                 c->opcode != OPCODE_RETURNVALUE) {
            c = abc_returnvoid(c);
        }
        if(state->m->body->code) syntaxerror("internal error");
        state->m->body->code = c;
    }
    old_state();
}



char is_subtype_of(classinfo_t*type, classinfo_t*supertype)
{
    return 1; // FIXME
}

void breakjumpsto(code_t*c, code_t*jump) 
{
    while(c->prev) 
        c=c->prev;
    while(c) {
        if(c->opcode == OPCODE___BREAK__) {
            c->opcode = OPCODE_JUMP;
            c->branch = jump;
        }
        c = c->next;
    }
}

classinfo_t*join_types(classinfo_t*type1, classinfo_t*type2, char op)
{
    if(!type1 || !type2) 
        return registry_getanytype();
    if(TYPE_IS_ANY(type1) || TYPE_IS_ANY(type2))
        return registry_getanytype();
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
    
    if(TYPE_IS_NUMBER(from) && TYPE_IS_UINT(to)) {
        return abc_coerce2(c, &m);
    }
    if(TYPE_IS_NUMBER(from) && TYPE_IS_INT(to)) {
        return abc_coerce2(c, &m);
    }
    /* these are subject to overflow */
    if(TYPE_IS_INT(from) && TYPE_IS_UINT(to)) {
        return abc_coerce2(c, &m);
    }
    if(TYPE_IS_UINT(from) && TYPE_IS_INT(to)) {
        return abc_coerce2(c, &m);
    }

    classinfo_t*supertype = from;
    while(supertype) {
        if(supertype == to) {
             // target type is one of from's superclasses
             return abc_coerce2(c, &m);
        }
        int t=0;
        while(supertype->interfaces[t]) {
            if(supertype->interfaces[t]==to) {
                // to type is one of from's interfaces
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
}

code_t*defaultvalue(code_t*c, classinfo_t*type)
{
    if(TYPE_IS_INT(type) || TYPE_IS_UINT(type) || TYPE_IS_FLOAT(type)) {
       c = abc_pushbyte(c, 0);
    } else if(TYPE_IS_BOOLEAN(type)) {
       c = abc_pushfalse(c);
    } else {
       c = abc_pushnull(c);
    }
    return c;
}

char is_pushundefined(code_t*c)
{
    return (c && !c->prev && !c->next && c->opcode == OPCODE_PUSHUNDEFINED);
}

void parserassert(int b)
{
    if(!b) syntaxerror("internal error: assertion failed");
}

static classinfo_t* find_class(char*name)
{
    classinfo_t*c=0;

    c = registry_findclass(state->package, name);

    /* try explicit imports */
    dictentry_t* e = dict_get_slot(state->imports, name);
    while(e) {
        if(c)
            break;
        if(!strcmp(e->key, name)) {
            c = (classinfo_t*)e->data;
        }
        e = e->next;
    }

    /* try package.* imports */
    import_list_t*l = state->wildcard_imports;
    while(l) {
        if(c)
            break;
        //printf("does package %s contain a class %s?\n", l->import->package, name);
        c = registry_findclass(l->import->package, name);
        l = l->next;
    }

    /* try global package */
    if(!c) {
        c = registry_findclass("", name);
    }
    return c;
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
        code_dump(r, 0, 0, "", stdout);
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
                c = abc_dup(c);
            }
            c = code_append(c, middlepart);
            c = abc_dup(c);
            c = abc_setlocal(c, temp);
            c = code_append(c, write);
            c = abc_getlocal(c, temp);
        }
    }

    return c;
}

#define IS_INT(a) (TYPE_IS_INT((a).t) || TYPE_IS_UINT((a).t))
#define BOTH_INT(a,b) (IS_INT(a) && IS_INT(b))

%}


%%

/* ------------ code blocks / statements ---------------- */

PROGRAM: MAYBECODE

MAYBECODE: CODE {$$=$1;/*TODO: do something with this code if we're not in a function*/}
MAYBECODE:      {$$=code_new();}

CODE: CODE CODEPIECE {$$=code_append($1,$2);}
CODE: CODEPIECE {$$=$1;}

CODEPIECE: PACKAGE_DECLARATION   {$$=code_new();/*enters a scope*/}
CODEPIECE: CLASS_DECLARATION     {$$=code_new();/*enters a scope*/}
CODEPIECE: FUNCTION_DECLARATION  {$$=code_new();/*enters a scope*/}
CODEPIECE: INTERFACE_DECLARATION {$$=code_new();}
CODEPIECE: IMPORT                {$$=code_new();/*adds imports to current scope*/}
CODEPIECE: ';'                   {$$=code_new();}
CODEPIECE: VARIABLE_DECLARATION  {$$=$1}
CODEPIECE: VOIDEXPRESSION        {$$=$1}
CODEPIECE: FOR                   {$$=$1}
CODEPIECE: WHILE                 {$$=$1}
CODEPIECE: BREAK                 {$$=$1}
CODEPIECE: RETURN                {$$=$1}
CODEPIECE: IF                    {$$=$1}
CODEPIECE: NAMESPACE_DECLARATION {/*TODO*/$$=code_new();}
CODEPIECE: USE_NAMESPACE         {/*TODO*/$$=code_new();}

CODEBLOCK :  '{' MAYBECODE '}' {$$=$2;}
CODEBLOCK :  CODEPIECE ';'             {$$=$1;}
CODEBLOCK :  CODEPIECE %prec below_semicolon {$$=$1;}

/* ------------ variables --------------------------- */

MAYBEEXPRESSION : '=' NONCOMMAEXPRESSION {$$=$2;}
                |                {$$.c=abc_pushundefined(0);
                                  $$.t=TYPE_ANY;
                                 }

VAR : "const" | "var"
VARIABLE_DECLARATION : VAR VARIABLE_LIST {$$=$2;}

VARIABLE_LIST: ONE_VARIABLE                   {$$ = $1;}
VARIABLE_LIST: VARIABLE_LIST ',' ONE_VARIABLE {$$ = code_append($1, $3);}

ONE_VARIABLE: {} T_IDENTIFIER MAYBETYPE MAYBEEXPRESSION
{
    if(variable_exists($2))
        syntaxerror("Variable %s already defined", $2);
   
    if(!is_subtype_of($4.t, $3)) {
        syntaxerror("Can't convert %s to %s", $4.t->name, 
                                              $3->name);
    }

    int index = new_variable($2, $3);
    
    if($3) {
        if($4.c->prev || $4.c->opcode != OPCODE_PUSHUNDEFINED) {
            $$ = $4.c;
            $$ = converttype($$, $4.t, $3);
            $$ = abc_setlocal($$, index);
        } else {
            $$ = defaultvalue(0, $3);
            $$ = abc_setlocal($$, index);
        }

        /* if this is a typed variable:
           push default value for type on stack */
        if($3) {
            state->initcode = defaultvalue(state->initcode, $3);
            state->initcode = abc_setlocal(state->initcode, index);
        }
    } else {
        if($4.c->prev || $4.c->opcode != OPCODE_PUSHUNDEFINED) {
            $$ = $4.c;
            $$ = abc_coerce_a($$);
            $$ = abc_setlocal($$, index);
        } else {
            $$ = code_new();
        }
    }
    
    /* that's the default for a local register, anyway
        else {
        state->initcode = abc_pushundefined(state->initcode);
        state->initcode = abc_setlocal(state->initcode, index);
    }*/
    //printf("variable %s -> %d (%s)\n", $2->text, index, $4.t?$4.t->name:"");
}

/* ------------ control flow ------------------------- */

MAYBEELSE:  %prec below_else {$$ = code_new();}
MAYBEELSE: "else" CODEBLOCK {$$=$2;}
//MAYBEELSE: ';' "else" CODEBLOCK {$$=$3;}

IF  : "if" '(' {new_state();} EXPRESSION ')' CODEBLOCK MAYBEELSE {
    $$ = state->initcode;state->initcode=0;

    $$ = code_append($$, $4.c);
    code_t*myjmp,*myif = $$ = abc_iffalse($$, 0);
   
    $$ = code_append($$, $6);
    if($7) {
        myjmp = $$ = abc_jump($$, 0);
    }
    myif->branch = $$ = abc_label($$);
    if($7) {
        $$ = code_append($$, $7);
        myjmp->branch = $$ = abc_label($$);
    }
    
    $$ = killvars($$);old_state();
}

FOR_INIT : {$$=code_new();}
FOR_INIT : VARIABLE_DECLARATION
FOR_INIT : VOIDEXPRESSION

FOR : "for" '(' {new_state();} FOR_INIT ';' EXPRESSION ';' VOIDEXPRESSION ')' CODEBLOCK {
    $$ = state->initcode;state->initcode=0;

    $$ = code_append($$, $4);
    code_t*loopstart = $$ = abc_label($$);
    $$ = code_append($$, $6.c);
    code_t*myif = $$ = abc_iffalse($$, 0);
    $$ = code_append($$, $10);
    $$ = code_append($$, $8);
    $$ = abc_jump($$, loopstart);
    code_t*out = $$ = abc_label($$);
    breakjumpsto($$, out);
    myif->branch = out;

    $$ = killvars($$);old_state();
}

WHILE : "while" '(' {new_state();} EXPRESSION ')' CODEBLOCK {
    $$ = state->initcode;state->initcode=0;

    code_t*myjmp = $$ = abc_jump($$, 0);
    code_t*loopstart = $$ = abc_label($$);
    $$ = code_append($$, $6);
    myjmp->branch = $$ = abc_label($$);
    $$ = code_append($$, $4.c);
    $$ = abc_iftrue($$, loopstart);
    code_t*out = $$ = abc_label($$);
    breakjumpsto($$, out);

    $$ = killvars($$);old_state();
}

BREAK : "break" {
    $$ = abc___break__(0);
}

/* ------------ packages and imports ---------------- */

X_IDENTIFIER: T_IDENTIFIER
            | "package" {$$="package";}

PACKAGE: PACKAGE '.' X_IDENTIFIER {$$ = concat3str($1,".",$3);}
PACKAGE: X_IDENTIFIER             {$$=$1;}

PACKAGE_DECLARATION : "package" PACKAGE '{' {startpackage($2)} MAYBECODE '}' {endpackage()}
PACKAGE_DECLARATION : "package" '{' {startpackage("")} MAYBECODE '}' {endpackage()}

IMPORT : "import" QNAME {
       classinfo_t*c = $2;
       if(!c) 
            syntaxerror("Couldn't import class\n");
       state_has_imports();
       dict_put(state->imports, c->name, c);
       $$=0;
}
IMPORT : "import" PACKAGE '.' '*' {
       NEW(import_t,i);
       i->package = $2;
       state_has_imports();
       list_append(state->wildcard_imports, i);
       $$=0;
}

/* ------------ classes and interfaces (header) -------------- */

MAYBE_MODIFIERS : {$$=0;}
MAYBE_MODIFIERS : MODIFIER_LIST {$$=$1}
MODIFIER_LIST : MODIFIER               {$$=$1;}
MODIFIER_LIST : MODIFIER_LIST MODIFIER {$$=$1|$2;}

MODIFIER : KW_PUBLIC {$$=FLAG_PUBLIC;}
         | KW_PRIVATE {$$=FLAG_PRIVATE;}
         | KW_PROTECTED {$$=FLAG_PROTECTED;}
         | KW_STATIC {$$=FLAG_STATIC;}
         | KW_DYNAMIC {$$=FLAG_DYNAMIC;}
         | KW_FINAL {$$=FLAG_FINAL;}
         | KW_OVERRIDE {$$=FLAG_OVERRIDE;}
         | KW_NATIVE {$$=FLAG_NATIVE;}
         | KW_INTERNAL {$$=FLAG_INTERNAL;}

EXTENDS : {$$=registry_getobjectclass();}
EXTENDS : KW_EXTENDS QNAME {$$=$2;}

EXTENDS_LIST : {$$=list_new();}
EXTENDS_LIST : KW_EXTENDS QNAME_LIST {$$=$2;}

IMPLEMENTS_LIST : {$$=list_new();}
IMPLEMENTS_LIST : KW_IMPLEMENTS QNAME_LIST {$$=$2;}

CLASS_DECLARATION : MAYBE_MODIFIERS "class" T_IDENTIFIER 
                              EXTENDS IMPLEMENTS_LIST 
                              '{' {startclass($1,$3,$4,$5, 0);} 
                              MAYBE_DECLARATION_LIST 
                              '}' {endclass();}

INTERFACE_DECLARATION : MAYBE_MODIFIERS "interface" T_IDENTIFIER 
                              EXTENDS_LIST 
                              '{' {startclass($1,$3,0,$4,1);}
                              MAYBE_IDECLARATION_LIST 
                              '}' {endclass();}

/* ------------ classes and interfaces (body) -------------- */

MAYBE_DECLARATION_LIST : 
MAYBE_DECLARATION_LIST : DECLARATION_LIST
DECLARATION_LIST : DECLARATION
DECLARATION_LIST : DECLARATION_LIST DECLARATION
DECLARATION : ';'
DECLARATION : SLOT_DECLARATION
DECLARATION : FUNCTION_DECLARATION

MAYBE_IDECLARATION_LIST : 
MAYBE_IDECLARATION_LIST : IDECLARATION_LIST
IDECLARATION_LIST : IDECLARATION
IDECLARATION_LIST : IDECLARATION_LIST IDECLARATION
IDECLARATION : ';'
IDECLARATION : "var" T_IDENTIFIER {
    syntaxerror("variable declarations not allowed in interfaces");
}
IDECLARATION : MAYBE_MODIFIERS "function" GETSET T_IDENTIFIER '(' MAYBE_PARAM_LIST ')' MAYBETYPE {
    $1 |= FLAG_PUBLIC;
    if($1&(FLAG_PRIVATE|FLAG_INTERNAL|FLAG_PROTECTED)) {
        syntaxerror("invalid method modifiers: interface methods always need to be public");
    }
    startfunction(0,$1,$3,$4,&$6,$8);
    endfunction(0);
}

/* ------------ classes and interfaces (body, slots ) ------- */

VARCONST: "var" | "const"

SLOT_DECLARATION: MAYBE_MODIFIERS VARCONST T_IDENTIFIER MAYBETYPE MAYBEEXPRESSION {
    int flags = $1;
    memberinfo_t* info = memberinfo_register(state->clsinfo, $3, MEMBER_SLOT);
    info->type = $4;
    info->flags = flags;
    trait_t*t=0;

    namespace_t mname_ns = {flags2access(flags), ""};
    multiname_t mname = {QNAME, &mname_ns, 0, $3};

    if(!(flags&FLAG_STATIC)) {
        if($4) {
            MULTINAME(m, $4);
            t=abc_class_slot(state->cls, &mname, &m);
        } else {
            t=abc_class_slot(state->cls, &mname, 0);
        }
        info->slot = t->slot_id;
    } else {
        if($4) {
            MULTINAME(m, $4);
            t=abc_class_staticslot(state->cls, &mname, &m);
        } else {
            t=abc_class_staticslot(state->cls, &mname, 0);
        }
        info->slot = t->slot_id;
    }
    if($5.c && !is_pushundefined($5.c)) {
        code_t*c = 0;
        c = abc_getlocal_0(c);
        c = code_append(c, $5.c);
        c = converttype(c, $5.t, $4);
        c = abc_setslot(c, t->slot_id);
        if(!(flags&FLAG_STATIC))
            state->cls_init = code_append(state->cls_init, c);
        else
            state->cls_static_init = code_append(state->cls_static_init, c);
    }
    if($2==KW_CONST) {
        t->kind= TRAIT_CONST;
    }
}

/* ------------ constants -------------------------------------- */

MAYBESTATICCONSTANT: {$$=0;}
MAYBESTATICCONSTANT: '=' STATICCONSTANT {$$=$2;}

STATICCONSTANT : T_BYTE {$$ = constant_new_int($1);}
STATICCONSTANT : T_INT {$$ = constant_new_int($1);}
STATICCONSTANT : T_UINT {$$ = constant_new_uint($1);}
STATICCONSTANT : T_FLOAT {$$ = constant_new_float($1);}
STATICCONSTANT : T_STRING {$$ = constant_new_string2($1.str,$1.len);}
//STATICCONSTANT : T_NAMESPACE {$$ = constant_new_namespace($1);}
STATICCONSTANT : "true" {$$ = constant_new_true($1);}
STATICCONSTANT : "false" {$$ = constant_new_false($1);}
STATICCONSTANT : "null" {$$ = constant_new_null($1);}

/* ------------ classes and interfaces (body, functions) ------- */

// non-vararg version
MAYBE_PARAM_LIST: {
    memset(&$$,0,sizeof($$));
}
MAYBE_PARAM_LIST: PARAM_LIST {
    $$=$1;
}

// vararg version
MAYBE_PARAM_LIST: "..." PARAM {
    memset(&$$,0,sizeof($$));
    $$.varargs=1;
    list_append($$.list, $2);
}
MAYBE_PARAM_LIST: PARAM_LIST ',' "..." PARAM {
    $$ =$1;
    $$.varargs=1;
    list_append($$.list, $4);
}

// non empty
PARAM_LIST: PARAM_LIST ',' PARAM {
    $$ = $1;
    list_append($$.list, $3);
}
PARAM_LIST: PARAM {
    memset(&$$,0,sizeof($$));
    list_append($$.list, $1);
}

PARAM:  T_IDENTIFIER ':' TYPE MAYBESTATICCONSTANT {
     $$ = malloc(sizeof(param_t));
     $$->name=$1;
     $$->type = $3;
     $$->value = $4;
}
PARAM:  T_IDENTIFIER MAYBESTATICCONSTANT {
     $$ = malloc(sizeof(param_t));
     $$->name=$1;
     $$->type = TYPE_ANY;
     $$->value = $2;
}
GETSET : "get" {$$=$1;}
       | "set" {$$=$1;}
       |       {$$=0;}

FUNCTION_DECLARATION: MAYBE_MODIFIERS "function" GETSET T_IDENTIFIER '(' MAYBE_PARAM_LIST ')' 
                      MAYBETYPE '{' {startfunction(0,$1,$3,$4,&$6,$8)} MAYBECODE '}' 
{
    if(!state->m) syntaxerror("internal error: undefined function");
    endfunction($11);
}

/* ------------- package + class ids --------------- */

CLASS: T_IDENTIFIER {

    /* try current package */
    $$ = find_class($1);
    if(!$$) syntaxerror("Could not find class %s\n", $1);
}

PACKAGEANDCLASS : PACKAGE '.' T_IDENTIFIER {
    $$ = registry_findclass($1, $3);
    if(!$$) syntaxerror("Couldn't find class %s.%s\n", $1, $3);
}

QNAME: PACKAGEANDCLASS
     | CLASS

QNAME_LIST : QNAME {$$=list_new();list_append($$, $1);}
QNAME_LIST : QNAME_LIST ',' QNAME {$$=$1;list_append($$,$3);}

TYPE : QNAME      {$$=$1;}
     | '*'        {$$=registry_getanytype();}
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

MAYBE_PARAM_VALUES :  %prec prec_none {$$=0;}
MAYBE_PARAM_VALUES : '(' MAYBE_EXPRESSION_LIST ')' {$$=$2}

MAYBE_EXPRESSION_LIST : {$$=0;}
MAYBE_EXPRESSION_LIST : EXPRESSION_LIST
EXPRESSION_LIST : NONCOMMAEXPRESSION             {$$=list_new();
                                                  typedcode_t*t = malloc(sizeof(typedcode_t));
                                                  *t = $1;
                                                  list_append($$, t);}
EXPRESSION_LIST : EXPRESSION_LIST ',' NONCOMMAEXPRESSION {$$=$1;
                                                  typedcode_t*t = malloc(sizeof(typedcode_t));
                                                  *t = $3;
                                                  list_append($$, t);}

NEW : "new" CLASS MAYBE_PARAM_VALUES {
    MULTINAME(m, $2);
    $$.c = code_new();

    if($2->slot) {
        $$.c = abc_getglobalscope($$.c);
        $$.c = abc_getslot($$.c, $2->slot);
    } else {
        $$.c = abc_findpropstrict2($$.c, &m);
    }

    typedcode_list_t*l = $3;
    int len = 0;
    while(l) {
        $$.c = code_append($$.c, l->typedcode->c); // push parameters on stack
        l = l->next;
        len ++;
    }
    if($2->slot)
        $$.c = abc_construct($$.c, len);
    else
        $$.c = abc_constructprop2($$.c, &m, len);
    $$.t = $2;
}

/* TODO: use abc_call (for calling local variables),
         abc_callstatic (for calling own methods) 
         call (for closures)
*/
FUNCTIONCALL : E '(' MAYBE_EXPRESSION_LIST ')' {
    typedcode_list_t*l = $3;
    int len = 0;
    code_t*paramcode = 0;
    while(l) {
        paramcode = code_append(paramcode, l->typedcode->c); // push parameters on stack
        l = l->next;
        len ++;
    }
       
    $$.c = $1.c;
    if($$.c->opcode == OPCODE_COERCE_A) {
        $$.c = code_cutlast($$.c);
    }

    $$.t = TYPE_ANY;
    multiname_t*name = 0;
    if($$.c->opcode == OPCODE_GETPROPERTY) {
        name = multiname_clone($$.c->data[0]);
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_callproperty2($$.c, name, len);
    } else if($$.c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)$$.c->data[0];
        trait_t*t = abc_class_find_slotid(state->cls,slot);//FIXME
        if(t->kind!=TRAIT_METHOD) {
            //flash allows to assign closures to members.
            //syntaxerror("not a function");
        }
        name = t->name;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        //$$.c = abc_callmethod($$.c, t->method, len); //#1051 illegal early access binding
        $$.c = abc_callproperty2($$.c, name, len);
    } else {
        $$.c = abc_getlocal_0($$.c);
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_call($$.c, len);
    }
   
    memberinfo_t*f = 0;
   
    if(TYPE_IS_FUNCTION($1.t) && $1.t->function) {
        $$.t = $1.t->function->return_type;
    } else {
        $$.c = abc_coerce_a($$.c);
        $$.t = TYPE_ANY;
    }
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
        multiname_t*name = abc_class_find_slotid(state->cls,slot)->name;
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
VOIDEXPRESSION : EXPRESSION %prec below_minus {$$=cut_last_push($1.c);}

// ----------------------- expression evaluation -------------------------------------

E : CONSTANT
E : VAR_READ %prec T_IDENTIFIER {$$ = $1;}
E : NEW                         {$$ = $1;}
E : DELETE                      {$$ = $1;}
E : T_REGEXP                    {$$.c = abc_pushundefined(0); /* FIXME */
                                 $$.t = TYPE_ANY;
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
CONSTANT : T_STRING {$$.c = abc_pushstring2(0, &$1);
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

E : FUNCTIONCALL
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

E : E '-' E {$$.c = code_append($1.c,$3.c);
             if(BOTH_INT($1,$3)) {
                $$.c = abc_subtract_i($$.c);
                $$.t = TYPE_INT;
             } else {
                $$.c = abc_subtract($$.c);
                $$.t = TYPE_NUMBER;
             }
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
E : E '+' E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_add($$.c);
             $$.t = TYPE_NUMBER;
            }
E : E '%' E {$$.c = code_append($1.c,$3.c);
             $$.c = abc_modulo($$.c);
             $$.t = TYPE_NUMBER;
            }
E : E '*' E {$$.c = code_append($1.c,$3.c);
             if(BOTH_INT($1,$3)) {
                $$.c = abc_multiply_i($$.c);
                $$.t = TYPE_INT;
             } else {
                $$.c = abc_multiply($$.c);
                $$.t = TYPE_NUMBER;
             }
            }

E : E "as" E {char use_astype=0; // flash player's astype works differently than astypelate
              if(use_astype && TYPE_IS_CLASS($3.t)) {
                MULTINAME(m,$3.t->cls);
                $$.c = abc_astype2($1.c, &m);
                $$.t = $3.t->cls;
              } else {
                $$.c = code_append($1.c, $3.c);
                $$.c = abc_astypelate($$.c);
                $$.t = TYPE_ANY;
              }
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
  if(IS_INT($2)) {
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

E : E "*=" E { 
               code_t*c = $3.c;
               if(BOTH_INT($1,$3)) {
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
E : E "+=" E { 
               code_t*c = $3.c;
               if(TYPE_IS_INT($3.t) || TYPE_IS_UINT($3.t)) {
                c=abc_add_i(c);
               } else {
                c=abc_add(c);
               }
               c=converttype(c, join_types($1.t, $3.t, '+'), $1.t);
               
               $$.c = toreadwrite($1.c, c, 0, 0);
               $$.t = $1.t;
              }
E : E "-=" E { code_t*c = $3.c; 
               if(TYPE_IS_INT($3.t) || TYPE_IS_UINT($3.t)) {
                c=abc_subtract_i(c);
               } else {
                c=abc_subtract(c);
               }
               c=converttype(c, join_types($1.t, $3.t, '-'), $1.t);
               
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
              $$.c = $1.c;
              code_t*j1 = $$.c = abc_iffalse($$.c, 0);
              $$.c = code_append($$.c, $3.c);
              code_t*j2 = $$.c = abc_jump($$.c, 0);
              $$.c = j1->branch = abc_label($$.c);
              $$.c = code_append($$.c, $5.c);
              $$.c = j2->branch = abc_label($$.c);
              $$.t = join_types($3.t,$5.t,'?');
            }

// TODO: use inclocal where appropriate
E : E "++" { code_t*c = 0;
             classinfo_t*type = $1.t;
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

E : E '.' T_IDENTIFIER
            {$$.c = $1.c;
             classinfo_t*t = $1.t;
             char is_static = 0;
             if(TYPE_IS_CLASS(t)) {
                 memberinfo_t*m = registry_findmember($1.t, "prototype");
                 if(!m) syntaxerror("identifier '%s' not found in anonymous class", $3);
                 t = m->type;
                 is_static = 1;
             }
             if(t) {
                 memberinfo_t*f = registry_findmember(t, $3);
                 char noslot = 0;
                 if(f && !is_static != !(f->flags&FLAG_STATIC))
                    noslot=1;

                 if(f && f->slot && !noslot) {
                     $$.c = abc_getslot($$.c, f->slot);
                 } else {
                     if(f) {
                         namespace_t ns = {flags2access(f->flags), ""}; // needs to be "", not $$.t->package (!)
                         multiname_t m = {QNAME, &ns, 0, $3};
                         $$.c = abc_getproperty2($$.c, &m);
                     } else {
                         multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, $3};
                         $$.c = abc_getproperty2($$.c, &m);
                     }
                 }
                 /* determine type */
                 if(f) {
                    if(f->kind == MEMBER_METHOD) {
                        $$.t = TYPE_FUNCTION(f);
                    } else {
                        $$.t = f->type;
                    }
                 } else {
                    $$.c = abc_coerce_a($$.c);
                    $$.t = registry_getanytype();
                 }
             } else {
                 /* when resolving a property on an unknown type, we do know the
                    name of the property (and don't seem to need the package), but
                    we do need to make avm2 try out all access modes */
                 multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, $3};
                 $$.c = abc_getproperty2($$.c, &m);
                 $$.c = abc_coerce_a($$.c);
                 $$.t = registry_getanytype();
             }
            }

VAR_READ : T_IDENTIFIER {
    $$.t = 0;
    $$.c = 0;
    int i;
    classinfo_t*a = 0;
    memberinfo_t*f = 0;

    /* look at variables */
    if((i = find_variable($1, &$$.t)) >= 0) {
        // $1 is a local variable
        $$.c = abc_getlocal($$.c, i);

    /* look at current class' members */
    } else if((f = registry_findmember(state->clsinfo, $1))) {
        // $1 is a function in this class
        int var_is_static = (f->flags&FLAG_STATIC);
        int i_am_static = (state->minfo?(state->minfo->flags&FLAG_STATIC):FLAG_STATIC);
        if(var_is_static != i_am_static) {
            /* there doesn't seem to be any "static" way to access
               static properties of a class */
            state->late_binding = 1;
            $$.t = f->type;
            namespace_t ns = {flags2access(f->flags), ""};
            multiname_t m = {QNAME, &ns, 0, $1};
            $$.c = abc_findpropstrict2($$.c, &m);
            $$.c = abc_getproperty2($$.c, &m);
        } else {
            if(f->slot>0) {
                $$.c = abc_getlocal_0($$.c);
                $$.c = abc_getslot($$.c, f->slot);
            } else {
                namespace_t ns = {flags2access(f->flags), ""};
                multiname_t m = {QNAME, &ns, 0, $1};
                $$.c = abc_getlocal_0($$.c);
                $$.c = abc_getproperty2($$.c, &m);
            }
        }
        if(f->kind == MEMBER_METHOD) {
            $$.t = TYPE_FUNCTION(f);
        } else {
            $$.t = f->type;
        }
    
    /* look at classes in the current package and imported classes */
    } else if((a = find_class($1))) {
        if(a->slot) {
            $$.c = abc_getglobalscope($$.c);
            $$.c = abc_getslot($$.c, a->slot);
        } else {
            MULTINAME(m, a);
            $$.c = abc_getlex2($$.c, &m);
        }
        $$.t = TYPE_CLASS(a);

    /* unknown object, let the avm2 resolve it */
    } else {
        if(strcmp($1,"trace"))
            warning("Couldn't resolve '%s', doing late binding", $1);
        state->late_binding = 1;
                
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, $1};

        $$.t = 0;
        $$.c = abc_findpropstrict2($$.c, &m);
        $$.c = abc_getproperty2($$.c, &m);
    }
}

//TODO: 
//VARIABLE : VARIABLE ".." T_IDENTIFIER // descendants
//VARIABLE : VARIABLE "::" VARIABLE // namespace declaration
//VARIABLE : VARIABLE "::" '[' EXPRESSION ']' // qualified expression

// ----------------- namespaces -------------------------------------------------

NAMESPACE_DECLARATION : MAYBE_MODIFIERS "namespace" T_IDENTIFIER {$$=$2;}
NAMESPACE_DECLARATION : MAYBE_MODIFIERS "namespace" T_IDENTIFIER '=' T_IDENTIFIER {$$=$2;}
NAMESPACE_DECLARATION : MAYBE_MODIFIERS "namespace" T_IDENTIFIER '=' T_STRING {$$=$2;}

USE_NAMESPACE : "use" "namespace" T_IDENTIFIER

