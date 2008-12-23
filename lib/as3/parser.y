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
    tokenptr_t token;

    class_signature_t*class_signature;
    class_signature_list_t*class_signature_list;

    int number_int;
    unsigned int number_uint;
    double number_float;
    code_t*code;
    typedcode_t value;
    typedcode_list_t*value_list;
    param_t* param;
    param_list_t* param_list;
    writeable_t writeable;
    char*string;
}


%token<token> T_IDENTIFIER
%token<string> T_STRING
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
%token<token> KW_FOR "for"
%token<token> KW_CLASS "class"
%token<token> KW_CONST "const"
%token<token> KW_SET "set"
%token<token> KW_STATIC
%token<token> KW_IMPORT "import"
%token<token> KW_RETURN "return"
%token<token> KW_INTERFACE "interface"
%token<token> KW_NULL
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
%token<token> KW_IF "if"
%token<token> KW_ELSE  "else"
%token<token> KW_BREAK   "break"
%token<token> KW_IS "is"
%token<token> KW_AS "as"

%token<token> T_EQEQ "=="
%token<token> T_EQEQEQ "==="
%token<token> T_NE "!="
%token<token> T_LE "<="
%token<token> T_GE ">="
%token<token> T_DIVBY "/=" 
%token<token> T_MODBY "%="
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
%token<token> T_SHL "<<"
%token<token> T_USHR ">>>"
%token<token> T_SHR ">>"
%token<token> T_SEMICOLON ';'
%token<token> T_STAR '*'
%token<token> T_DOT '.'

%type <code> CODE
%type <code> CODEPIECE
%type <code> CODEBLOCK MAYBECODE
%type <token> PACKAGE_DECLARATION
%type <token> FUNCTION_DECLARATION
%type <code> VARIABLE_DECLARATION
%type <token> CLASS_DECLARATION
%type <token> NAMESPACE_DECLARATION
%type <token> INTERFACE_DECLARATION
%type <code> VOIDEXPRESSION
%type <value> EXPRESSION
%type <value> MAYBEEXPRESSION
%type <value> E
%type <writeable> LH
%type <value> CONSTANT
%type <code> FOR IF WHILE MAYBEELSE BREAK RETURN
%type <token> USE_NAMESPACE
%type <code> ASSIGNMENT FOR_INIT
%type <token> IMPORT
%type <class_signature> MAYBETYPE
%type <token> GETSET
%type <param> PARAM
%type <param_list> PARAM_LIST
%type <param_list> MAYBE_PARAM_LIST
%type <token> MODIFIERS
%type <token> MODIFIER_LIST
%type <class_signature_list> IMPLEMENTS_LIST
%type <class_signature> EXTENDS
%type <class_signature_list> EXTENDS_LIST
%type <class_signature> PACKAGEANDCLASS
%type <class_signature_list> PACKAGEANDCLASS_LIST
%type <token> MULTILEVELIDENTIFIER
%type <class_signature> TYPE
%type <token> VAR
//%type <token> VARIABLE
%type <value> VAR_READ
%type <value> NEW
%type <token> X_IDENTIFIER
%type <token> MODIFIER
%type <token> PACKAGE
%type <value> FUNCTIONCALL
%type <value_list> MAYBE_EXPRESSION_LIST EXPRESSION_LIST MAYBE_PARAM_VALUES

// precedence: from low to high
// http://livedocs.adobe.com/flash/9.0/main/wwhelp/wwhimpl/common/html/wwhelp.htm?context=LiveDocs_Parts&file=00000012.html

%left prec_none
%right '?' ':'
%nonassoc '='
%nonassoc "/=" "%="
%nonassoc "+=" "-="
%nonassoc ">>="
%nonassoc "<<="
%nonassoc ">>>="
%nonassoc "||"
%nonassoc "&&"
%nonassoc '|'
%nonassoc '^'
%nonassoc '&'
%nonassoc "!=" "==" "===" "<=" '<' ">=" '>' // TODO: support "a < b < c" syntax?
%nonassoc "is"
%left '-'
%left '+'
%left "<<"
%left ">>>"
%left ">>"
%left '%'
%left '/'
%left '*'
%left '!'
%left '~'
%left "--" "++"
%left '['
%nonassoc "as"
%left '.' ".." "::"
%nonassoc T_IDENTIFIER
%left below_semicolon
%left ';'
%nonassoc "else"
%left '('
%left prec_highest

     
%{

static int yyerror(char*s)
{
   syntaxerror("%s", s); 
}
static token_t* concat2(token_t* t1, token_t* t2)
{
    NEW(token_t,t);
    int l1 = strlen(t1->text);
    int l2 = strlen(t2->text);
    t->text = malloc(l1+l2+1);
    memcpy(t->text   , t1->text, l1);
    memcpy(t->text+l1, t2->text, l2);
    t->text[l1+l2] = 0;
    return t;
}
static token_t* concat3(token_t* t1, token_t* t2, token_t* t3)
{
    NEW(token_t,t);
    int l1 = strlen(t1->text);
    int l2 = strlen(t2->text);
    int l3 = strlen(t3->text);
    t->text = malloc(l1+l2+l3+1);
    memcpy(t->text   , t1->text, l1);
    memcpy(t->text+l1, t2->text, l2);
    memcpy(t->text+l1+l2, t3->text, l3);
    t->text[l1+l2+l3] = 0;
    return t;
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

    abc_method_body_t*m;
    
    import_list_t*wildcard_imports;
    dict_t*imports;
    char has_own_imports;
   
    /* class data */
    char*classname;
    abc_class_t*cls;

    array_t*vars;
    int local_var_base;
} state_t;

static state_t* state = 0;

DECLARE_LIST(state);

#define MULTINAME(m,x) multiname_t m;namespace_t m##_ns;registry_fill_multiname(&m, &m##_ns, x);

static state_list_t*state_stack=0;

static void new_state()
{
    NEW(state_t, s);
    NEW(state_list_t, sl);

    state_t*oldstate = state;
    if(state)
        memcpy(s, state, sizeof(state_t)); //shallow copy
    sl->next = state_stack;
    sl->state = s;
    if(oldstate) {
        s->local_var_base = array_length(oldstate->vars) + oldstate->local_var_base;
    }
    if(!s->imports) {
        s->imports = dict_new();
    }
    state_stack = sl;
    state = s;
    state->level++;
    state->vars = array_new();
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
    new_state();

    state->file = abc_file_new();
    state->file->flags &= ~ABCFILE_LAZY;
    
    state->init = abc_initscript(state->file, 0, 0);
    abc_method_body_t*m = state->init->method->body;
    __ getlocal_0(m);
    __ pushscope(m);
    __ findpropstrict(m, "[package]::trace");
    __ pushstring(m, "[entering global init function]");
    __ callpropvoid(m, "[package]::trace", 1);
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


static void startpackage(token_t*t) 
{
    if(state->package) {
        syntaxerror("Packages can not be nested."); 
    } 
    new_state();
    char*name = t?t->text:"";
    /*printf("entering package \"%s\"\n", name);*/
    state->package = name;
}
static void endpackage()
{
    /*printf("leaving package \"%s\"\n", state->package);*/
    old_state();
}

char*globalclass=0;
static void startclass(token_t*modifiers, token_t*name, class_signature_t*extends, class_signature_list_t*implements, char interface)
{
    if(state->cls) {
        syntaxerror("inner classes now allowed"); 
    }
    new_state();
    state->classname = name->text;

    token_list_t*t=0;
    class_signature_list_t*mlist=0;
    /*printf("entering class %s\n", name->text);
    printf("  modifiers: ");for(t=modifiers->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    if(extends) 
        printf("  extends: %s.%s\n", extends->package, extends->name);
    printf("  implements (%d): ", list_length(implements));
    for(mlist=implements;mlist;mlist=mlist->next)  {
        printf("%s ", mlist->class_signature?mlist->class_signature->name:0);
    }
    printf("\n");
    */

    char public=0,internal=0,final=0,sealed=1;
    for(t=modifiers->tokens;t;t=t->next) {
        if(t->token->type == KW_INTERNAL) {
            /* the programmer is being explicit- 
               being internal is the default anyway */
            internal = 1;
        } else if(t->token->type == KW_PUBLIC) {
            public = 1;
        } else if(t->token->type == KW_FINAL) {
            final = 1;
        } else {
            syntaxerror("modifier \"%s\" not supported in class declaration", t->token->text);
        }
    }
    if(public&&internal)
        syntaxerror("public and internal not supported at the same time.");

    /* create the class name, together with the proper attributes */
    int access=0;
    char*package=0;

    if(!public && !state->package) {
        access = ACCESS_PRIVATE; package = current_filename;
    } else if(!public && state->package) {
        access = ACCESS_PACKAGEINTERNAL; package = state->package;
    } else if(state->package) {
        access = ACCESS_PACKAGE; package = state->package;
    } else {
        syntaxerror("public classes only allowed inside a package");
    }

    if(registry_findclass(package, state->classname)) {
        syntaxerror("Package \"%s\" already contains a class called \"%s\"", package, state->classname);
    }
    
    class_signature_t* classname = class_signature_register(access, package, state->classname);

    MULTINAME(classname2,classname);
    
    multiname_t*extends2 = sig2mname(extends);

    state->cls = abc_class_new(state->file, &classname2, extends2);
    if(final) abc_class_final(state->cls);
    if(sealed) abc_class_sealed(state->cls);
    if(interface) abc_class_interface(state->cls);

    for(mlist=implements;mlist;mlist=mlist->next) {
        MULTINAME(m, mlist->class_signature);
        abc_class_add_interface(state->cls, &m);
    }

    /* now write the construction code for this class */
    int slotindex = abc_initscript_addClassTrait(state->init, &classname2, state->cls);

    abc_method_body_t*m = state->init->method->body;
    __ getglobalscope(m);
    class_signature_t*s = extends;

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
    if(!globalclass && public && class_signature_equals(registry_getMovieClip(),extends)) {
        if(state->package && state->package[0]) {
            globalclass = concat3str(state->package, ".", state->classname);
        } else {
            globalclass = strdup(state->classname);
        }
    }
    multiname_destroy(extends2);
}

static void endclass()
{
    /*printf("leaving class %s\n", state->classname);*/
    old_state();
}
static void startfunction(token_t*ns, token_t*mod, token_t*getset, token_t*name,
                          param_list_t*params, class_signature_t*type)
{
    token_list_t*t;
    new_state();
    state->function = name->text;
    
    /*printf("entering function %s\n", name->text);
    if(ns)
        printf("  namespace: %s\n", ns->text);
    printf("  getset: %s\n", getset->text);
    printf("  params: ");for(t=params->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    printf("  mod: ");for(t=mod->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    if(type)
        printf("  type: %s.%s\n", type->package, type->name);
    print_imports();*/
    
    if(state->m) {
        syntaxerror("not able to start another method scope");
    }

    multiname_t*type2 = sig2mname(type);

    if(!strcmp(state->classname,name->text)) {
        state->m = abc_class_constructor(state->cls, type2, 0);
    } else {
        state->m = abc_class_method(state->cls, type2, name->text, 0);
    }
    param_list_t*p;
    for(p=params;p;p=p->next) {
        multiname_t*m = sig2mname(p->param->type);
	list_append(state->m->method->parameters, m);
    }

    /* state->vars is initialized by state_new */
    array_append(state->vars, "this", 0);
    for(p=params;p;p=p->next) {
        array_append(state->vars, p->param->name, 0);
    }

    __ getlocal_0(state->m);
    __ pushscope(state->m);

    multiname_destroy(type2);
}
static void endfunction()
{
    /*printf("leaving function %s\n", state->function);*/
    __ returnvoid(state->m);

    old_state();
}


static token_t* empty_token()
{
    NEW(token_t,t);
    t->type=T_EMPTY;
    t->text=0;
    return t;
}

void extend(token_t*list, token_t*add) {
    list_append(list->tokens,add);
    if(!list->text)
        list->text = add->text;
}
void extend_s(token_t*list, char*seperator, token_t*add) {
    list_append(list->tokens,add);
    char*t1 = list->text;
    char*t2 = seperator;
    char*t3 = add->text;
    int l1 = strlen(t1);
    int l2 = strlen(t2);
    int l3 = strlen(t3);
    list->text = malloc(l1+l2+l3+1);
    strcpy(list->text, t1);
    strcpy(list->text+l1, t2);
    strcpy(list->text+l1+l2, t3);
    list->text[l1+l2+l3]=0;
}

static int find_variable(char*name, class_signature_t**m)
{
    state_list_t* s = state_stack;
    while(s) {
        int i = array_find(s->state->vars, name);
        if(i>=0) {
            if(m) {
                *m = array_getvalue(s->state->vars, i);
            }
            return i + s->state->local_var_base;
        }
        s = s->next;
    }
    syntaxerror("undefined variable: %s", name);
} 
static char variable_exists(char*name) 
{
    return array_contains(state->vars, name);
}
static int new_variable(char*name, class_signature_t*type)
{
    return array_append(state->vars, name, type) + state->local_var_base;
}
code_t* killvars(code_t*c) 
{
    int t;
    for(t=0;t<state->vars->num;t++) {
        class_signature_t*type = array_getvalue(state->vars, t);
        //do this always, otherwise register types don't match
        //in the verifier when doing nested loops
        //if(!TYPE_IS_BUILTIN_SIMPLE(type)) {
            c = abc_kill(c, t+state->local_var_base);
        //}
    }
    return c;
}

char is_subtype_of(class_signature_t*type, class_signature_t*supertype)
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

class_signature_t*join_types(class_signature_t*type1, class_signature_t*type2, char op)
{
    return registry_getanytype(); // FIXME
}
code_t*converttype(code_t*c, class_signature_t*from, class_signature_t*to)
{
    if(from==to)
        return c;
    if(!to) {
        /*TODO: can omit this if from is zero? */
        return abc_coerce_a(c);
    }
    if(TYPE_IS_NUMBER(from) && TYPE_IS_UINT(to)) {
        MULTINAME(m, TYPE_UINT);
        return abc_coerce2(c, &m);
    }
    if(TYPE_IS_NUMBER(from) && TYPE_IS_INT(to)) {
        MULTINAME(m, TYPE_INT);
        return abc_coerce2(c, &m);
    }
    return c;
}

code_t*defaultvalue(code_t*c, class_signature_t*type)
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

%}


%%

/* ------------ code blocks / statements ---------------- */

PROGRAM: MAYBECODE

MAYBECODE: CODE {$$=$1;}
MAYBECODE:      {$$=code_new();}

CODE: CODE CODEPIECE {$$=code_append($1,$2);}
CODE: CODEPIECE {$$=$1;}

CODEPIECE: PACKAGE_DECLARATION   {$$=code_new();/*enters a scope*/}
CODEPIECE: CLASS_DECLARATION     {$$=code_new();/*enters a scope*/}
CODEPIECE: INTERFACE_DECLARATION {/*TODO*/$$=code_new();}
CODEPIECE: IMPORT                {$$=code_new();/*adds imports to current scope*/}
CODEPIECE: ';'                   {$$=code_new();}
CODEPIECE: VARIABLE_DECLARATION  {$$=$1}
CODEPIECE: VOIDEXPRESSION        {$$=$1}
CODEPIECE: FOR                   {$$=$1}
CODEPIECE: WHILE                 {$$=$1}
CODEPIECE: BREAK                 {$$=$1}
CODEPIECE: RETURN                {$$=$1}
CODEPIECE: IF                    {$$=$1}
CODEPIECE: ASSIGNMENT            {$$=$1}
CODEPIECE: NAMESPACE_DECLARATION {/*TODO*/$$=code_new();}
CODEPIECE: FUNCTION_DECLARATION  {/*TODO*/$$=code_new();}
CODEPIECE: USE_NAMESPACE         {/*TODO*/$$=code_new();}

CODEBLOCK :  '{' MAYBECODE '}' {$$=$2;}
CODEBLOCK :  CODEPIECE ';'             {$$=$1;}
CODEBLOCK :  CODEPIECE %prec below_semicolon {$$=$1;}

/* ------------ functions --------------------------- */

FUNCTION_DECLARATION: MODIFIERS "function" GETSET T_IDENTIFIER '(' MAYBE_PARAM_LIST ')' 
                      MAYBETYPE '{' {startfunction(0,$1,$3,$4,$6,$8)} MAYBECODE '}' {
    if(!state->m) syntaxerror("internal error: undefined function");
    state->initcode = abc_nop(state->initcode);
    state->initcode = abc_nop(state->initcode);
    state->initcode = abc_nop(state->initcode);
    state->m->code = code_append(state->initcode, $11);state->initcode=0;
    endfunction()
}

/* ------------ variables --------------------------- */

MAYBEEXPRESSION : '=' EXPRESSION {$$=$2;}
                |                {$$.c=abc_pushundefined(0);
                                  $$.t=TYPE_ANY;
                                 }

VAR : "const" | "var"
VARIABLE_DECLARATION : VAR T_IDENTIFIER MAYBETYPE MAYBEEXPRESSION {
    if(variable_exists($2->text))
        syntaxerror("Variable %s already defined", $2->text);
   
    if(!is_subtype_of($4.t, $3)) {
        syntaxerror("Can't convert %s to %s", $4.t->name, 
                                              $3->name);
    }

    int index = new_variable($2->text, $3);
    
    if($3) {
        if($4.c->prev || $4.c->opcode != OPCODE_PUSHUNDEFINED) {
            $$ = $4.c;
            $$ = converttype($$, $4.t, $3);
            $$ = abc_setlocal($$, index);
        } else {
            $$ = defaultvalue(0, $3);
            $$ = abc_setlocal($$, index);
        }

        /* push default value for type on stack */
        state->initcode = defaultvalue(state->initcode, $3);
        state->initcode = abc_setlocal(state->initcode, index);
    } else {
        /* only bother to actually set this variable if its syntax is either
            var x:type;
           or
            var x=expr;
        */
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
    printf("variable %s -> %d (%s)\n", $2->text, index, $4.t?$4.t->name:"");
}
ASSIGNMENT :           T_IDENTIFIER '=' EXPRESSION {
    class_signature_t*type=0;
    int i = find_variable($1->text, &type);
    $$ = $3.c;
    if(!type && $3.t) {
        // convert to "any" type, the register is untyped
        $$ = abc_coerce_a($$);
    } else {
        // TODO: convert ints to strings etc.
    }
    $$ = abc_setlocal($$, i);
}

/* ------------ control flow ------------------------- */

MAYBEELSE:  %prec prec_none {$$ = code_new();}
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
FOR_INIT : ASSIGNMENT | VARIABLE_DECLARATION | VOIDEXPRESSION

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

PACKAGE_DECLARATION : "package" MULTILEVELIDENTIFIER '{' {startpackage($2)} MAYBECODE '}' {endpackage()}
PACKAGE_DECLARATION : "package" '{' {startpackage(0)} MAYBECODE '}' {endpackage()}

PACKAGE: PACKAGE '.' X_IDENTIFIER {$$ = concat3($1,$2,$3);}
PACKAGE: X_IDENTIFIER             {$$=$1;}

IMPORT : "import" PACKAGE '.' X_IDENTIFIER {
       class_signature_t*c = registry_findclass($2->text, $4->text);
       if(!c) 
            syntaxerror("Couldn't import %s.%s\n", $2->text, $4->text);
       state_has_imports();
       dict_put(state->imports, $4->text, c);
       $$=0;
}
IMPORT : "import" PACKAGE '.' '*' {
       NEW(import_t,i);
       i->package = $2->text;
       state_has_imports();
       list_append(state->wildcard_imports, i);
       $$=0;
}

/* ------------ classes and interfaces -------------- */

MODIFIERS : {$$=empty_token();}
MODIFIERS : MODIFIER_LIST {$$=$1}
MODIFIER_LIST : MODIFIER MODIFIER_LIST {extend($2,$1);$$=$2;}
MODIFIER_LIST : MODIFIER               {$$=empty_token();extend($$,$1);}
MODIFIER : KW_PUBLIC | KW_PRIVATE | KW_PROTECTED | KW_STATIC | KW_DYNAMIC | KW_FINAL | KW_OVERRIDE | KW_NATIVE | KW_INTERNAL

EXTENDS : {$$=registry_getobjectclass();}
EXTENDS : KW_EXTENDS PACKAGEANDCLASS {$$=$2;}

EXTENDS_LIST : {$$=list_new();}
EXTENDS_LIST : KW_EXTENDS PACKAGEANDCLASS_LIST {$$=$2;}

IMPLEMENTS_LIST : {$$=list_new();}
IMPLEMENTS_LIST : KW_IMPLEMENTS PACKAGEANDCLASS_LIST {$$=$2;}

CLASS_DECLARATION : MODIFIERS "class" T_IDENTIFIER 
                              EXTENDS IMPLEMENTS_LIST 
                              '{' {startclass($1,$3,$4,$5, 0);} 
                              MAYBE_DECLARATION_LIST 
                              '}' {endclass();}
INTERFACE_DECLARATION : MODIFIERS "interface" T_IDENTIFIER 
                              EXTENDS_LIST 
                              '{' {startclass($1,$3,0,$4,1);}
                              MAYBE_IDECLARATION_LIST 
                              '}' {endclass();}

/* ------------- package + class ids --------------- */

PACKAGEANDCLASS : T_IDENTIFIER {

    /* try current package */
    $$ = registry_findclass(state->package, $1->text);

    /* try explicit imports */
    dictentry_t* e = dict_get_slot(state->imports, $1->text);
    while(e) {
        if($$)
            break;
        if(!strcmp(e->key, $1->text)) {
            $$ = (class_signature_t*)e->data;
        }
        e = e->next;
    }

    /* try package.* imports */
    import_list_t*l = state->wildcard_imports;
    while(l) {
        if($$)
            break;
        //printf("does package %s contain a class %s?\n", l->import->package, $1->text);
        $$ = registry_findclass(l->import->package, $1->text);
        l = l->next;
    }

    /* try global package */
    if(!$$) {
        $$ = registry_findclass("", $1->text);
    }

    if(!$$) syntaxerror("Could not find class %s\n", $1->text);
}
PACKAGEANDCLASS : PACKAGE '.' T_IDENTIFIER {
    $$ = registry_findclass($1->text, $3->text);
    if(!$$) syntaxerror("Couldn't find class %s.%s\n", $1->text, $3->text);
}


/* ----------function calls, constructor calls ------ */

MAYBE_PARAM_VALUES :  %prec prec_none {$$=0;}
MAYBE_PARAM_VALUES : '(' MAYBE_EXPRESSION_LIST ')' {$$=$2}

MAYBE_EXPRESSION_LIST : {$$=0;}
MAYBE_EXPRESSION_LIST : EXPRESSION_LIST
EXPRESSION_LIST : EXPRESSION                     {$$=list_new();
                                                  typedcode_t*t = malloc(sizeof(typedcode_t));
                                                  *t = $1;
                                                  list_append($$, t);}
EXPRESSION_LIST : EXPRESSION_LIST ',' EXPRESSION {$$=$1;
                                                  typedcode_t*t = malloc(sizeof(typedcode_t));
                                                  *t = $3;
                                                  list_append($$, t);}

NEW : "new" PACKAGEANDCLASS MAYBE_PARAM_VALUES {
    MULTINAME(m, $2);
    $$.c = code_new();
    $$.c = abc_findpropstrict2($$.c, &m);
    typedcode_list_t*l = $3;
    int len = 0;
    while(l) {
        $$.c = code_append($$.c, l->typedcode->c); // push parameters on stack
        l = l->next;
        len ++;
    }
    $$.c = abc_constructprop2($$.c, &m, len);
    $$.t = $2;
}

FUNCTIONCALL : T_IDENTIFIER '(' MAYBE_EXPRESSION_LIST ')' {
    /* TODO: use abc_call (for calling local variables),
             abc_callstatic (for calling own methods) */
    $$.c = code_new();
    $$.c = abc_findpropstrict($$.c, $1->text);
    typedcode_list_t*l = $3;
    int len = 0;
    while(l) {
        $$.c = code_append($$.c, l->typedcode->c); // push parameters on stack
        l = l->next;
        len ++;
    }
    $$.c = abc_callproperty($$.c, $1->text, len);
    /* TODO: look up the functions's return value */
    $$.t = TYPE_ANY;
}

RETURN: "return" %prec prec_none {
    $$ = abc_returnvoid(0);
}
RETURN: "return" EXPRESSION {
    $$ = $2.c;
    $$ = abc_returnvalue($$);
}

TYPE : PACKAGEANDCLASS {$$=$1;}
     | '*'        {$$=registry_getanytype();}
     |  "String"  {$$=registry_getstringclass();}
     |  "int"     {$$=registry_getintclass();}
     |  "uint"    {$$=registry_getuintclass();}
     |  "Boolean" {$$=registry_getbooleanclass();}
     |  "Number"  {$$=registry_getnumberclass();}

MAYBETYPE: ':' TYPE {$$=$2;}
MAYBETYPE:          {$$=0;}

//FUNCTION_HEADER:      NAMESPACE MODIFIERS T_FUNCTION GETSET T_IDENTIFIER '(' PARAMS ')' 
FUNCTION_HEADER:      MODIFIERS "function" GETSET T_IDENTIFIER '(' MAYBE_PARAM_LIST ')' 
                      MAYBETYPE

NAMESPACE_DECLARATION : MODIFIERS KW_NAMESPACE T_IDENTIFIER
NAMESPACE_DECLARATION : MODIFIERS KW_NAMESPACE T_IDENTIFIER '=' T_IDENTIFIER
NAMESPACE_DECLARATION : MODIFIERS KW_NAMESPACE T_IDENTIFIER '=' T_STRING

//NAMESPACE :              {$$=empty_token();}
//NAMESPACE : T_IDENTIFIER {$$=$1};

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
CONSTANT : T_STRING {$$.c = abc_pushstring(0, $1);
                     $$.t = TYPE_STRING;
                    }
CONSTANT : KW_TRUE {$$.c = abc_pushtrue(0);
                    $$.t = TYPE_BOOLEAN;
                   }
CONSTANT : KW_FALSE {$$.c = abc_pushfalse(0);
                     $$.t = TYPE_BOOLEAN;
                    }
CONSTANT : KW_NULL {$$.c = abc_pushnull(0);
                    $$.t = TYPE_NULL;
                   }

USE_NAMESPACE : "use" "namespace" T_IDENTIFIER


EXPRESSION : E %prec prec_none  /*precedence below '-x'*/ {$$ = $1;}
VOIDEXPRESSION : E %prec prec_none {$$=$1.c;/*calculate and discard*/$$=abc_pop($$);}

E : CONSTANT
E : VAR_READ %prec T_IDENTIFIER {$$ = $1;}
E : NEW                         {$$ = $1;}
E : T_REGEXP                    {$$.c = abc_pushundefined(0); /* FIXME */
                                 $$.t = TYPE_ANY;
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
E : E "!=" E {$$.c = code_append($1.c,$3.c);$$.c = abc_equals($$.c);$$.c = abc_not($$.c);
              $$.t = TYPE_BOOLEAN;
             }

E : E "||" E {$$.t = join_types($1.t, $3.t, 'O');
              $$.c = $1.c;
              $$.c = converttype($$.c, $1.t, $$.t);
              $$.c = abc_dup($$.c);
              code_t*jmp = $$.c = abc_iftrue($$.c, 0);
              $$.c = abc_pop($$.c);
              $$.c = code_append($$.c,$3.c);
              $$.c = converttype($$.c, $1.t, $$.t);
              code_t*label = $$.c = abc_label($$.c);
              jmp->branch = label;
             }
E : E "&&" E {$$.t = join_types($1.t, $3.t, 'A');
              $$.c = $1.c;
              $$.c = converttype($$.c, $1.t, $$.t);
              $$.c = abc_dup($$.c);
              code_t*jmp = $$.c = abc_iffalse($$.c, 0);
              $$.c = abc_pop($$.c);
              $$.c = code_append($$.c,$3.c);
              $$.c = converttype($$.c, $1.t, $$.t);
              code_t*label = $$.c = abc_label($$.c);
              jmp->branch = label;              
             }

E : '!' E    {$$.c=$2.c;
              $$.c = abc_not($$.c);
              $$.t = TYPE_BOOLEAN;
             }

E : E '-' E
E : E '/' E
E : E '+' E {$$.c = code_append($1.c,$3.c);$$.c = abc_add($$.c);$$.c=abc_coerce_a($$.c);
             $$.t = join_types($1.t, $3.t, '+');
            }
E : E '%' E {$$.c = code_append($1.c,$3.c);$$.c = abc_modulo($$.c);$$.c=abc_coerce_a($$.c);
             $$.t = join_types($1.t, $3.t, '%');
            }
E : E '*' E {$$.c = code_append($1.c,$3.c);$$.c = abc_multiply($$.c);$$.c=abc_coerce_a($$.c);
             $$.t = join_types($1.t, $3.t, '*');
            }

E : E "as" TYPE
E : E "is" TYPE
E : '(' E ')' {$$=$2;}
E : '-' E {$$=$2;}

E : LH "+=" E {$$.c = $1.read;$$.c=code_append($$.c,$3.c);$$.c=abc_add($$.c);
               class_signature_t*type = join_types($1.type, $3.t, '+');
               $$.c=converttype($$.c, type, $1.type);
               $$.c=abc_dup($$.c);$$.c=code_append($$.c,$1.write);
               $$.t = $1.type;
              }
E : LH "-=" E {$$.c = $1.read;$$.c=code_append($$.c,$3.c);$$.c=abc_add($$.c);
               class_signature_t*type = join_types($1.type, $3.t, '-');
               $$.c=converttype($$.c, type, $1.type);
               $$.c=abc_dup($$.c);$$.c=code_append($$.c,$1.write);
               $$.t = $1.type;
              }

// TODO: use inclocal where appropriate
E : LH "++" {$$.c = $1.read;
             class_signature_t*type = $1.type;
             if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                 $$.c=abc_increment_i($$.c);
             } else {
                 $$.c=abc_increment($$.c);
                 type = TYPE_NUMBER;
             }
             $$.c=converttype($$.c, type, $1.type);
             $$.c=abc_dup($$.c);$$.c=code_append($$.c,$1.write);
             $$.t = $1.type;
            }
E : LH "--" {$$.c = $1.read;
             class_signature_t*type = $1.type;
             if(TYPE_IS_INT(type) || TYPE_IS_UINT(type)) {
                 $$.c=abc_decrement_i($$.c);
             } else {
                 $$.c=abc_decrement($$.c);
                 type = TYPE_NUMBER;
             }
             $$.c=converttype($$.c, type, $1.type);
             $$.c=abc_dup($$.c);$$.c=code_append($$.c,$1.write);
             $$.t = $1.type;
            }

LH: T_IDENTIFIER {
  int i = find_variable($1->text, &$$.type);
  $$.read = abc_getlocal(0, i);
  $$.write = abc_setlocal(0, i);
}


VAR_READ : T_IDENTIFIER {
    int i = find_variable($1->text, &$$.t);
    $$.c = abc_getlocal(0, i);
}

//VARIABLE : T_IDENTIFIER
//VARIABLE : VARIABLE '.' T_IDENTIFIER
//VARIABLE : VARIABLE ".." T_IDENTIFIER // descendants
//VARIABLE : VARIABLE "::" VARIABLE // namespace declaration
//VARIABLE : VARIABLE "::" '[' EXPRESSION ']' // qualified expression
//VARIABLE : VARIABLE '[' EXPRESSION ']' // unqualified expression

// keywords which also may be identifiers
X_IDENTIFIER : T_IDENTIFIER | KW_PACKAGE

GETSET : "get" {$$=$1;}
       | "set" {$$=$1;}
       |       {$$=empty_token();}

MAYBE_PARAM_LIST: {$$=list_new();}
MAYBE_PARAM_LIST: PARAM_LIST {$$=$1;}
PARAM_LIST: PARAM_LIST ',' PARAM {$$ =$1;         list_append($$, $3);}
PARAM_LIST: PARAM                {$$ = list_new();list_append($$, $1);}
PARAM:  T_IDENTIFIER ':' TYPE {$$ = malloc(sizeof(param_t));
                               $$->name=$1->text;$$->type = $3;}
PARAM:  T_IDENTIFIER          {$$ = malloc(sizeof(param_t));
                               $$->name=$1->text;$$->type = TYPE_ANY;}

DECLARATION : VARIABLE_DECLARATION
DECLARATION : FUNCTION_DECLARATION

IDECLARATION : VARIABLE_DECLARATION
IDECLARATION : FUNCTION_DECLARATION

//IDENTIFIER_LIST : T_IDENTIFIER ',' IDENTIFIER_LIST {extend($3,$1);$$=$3;}
//IDENTIFIER_LIST : T_IDENTIFIER                     {$$=empty_token();extend($$,$1);}

MULTILEVELIDENTIFIER : MULTILEVELIDENTIFIER '.' X_IDENTIFIER {$$=$1;extend_s($$, ".", $3)}
MULTILEVELIDENTIFIER : T_IDENTIFIER                 {$$=$1;extend($$,$1)};

PACKAGEANDCLASS_LIST : PACKAGEANDCLASS {$$=list_new();list_append($$, $1);}
PACKAGEANDCLASS_LIST : PACKAGEANDCLASS_LIST ',' PACKAGEANDCLASS {$$=$1;list_append($$,$3);}

MAYBE_DECLARATION_LIST : 
MAYBE_DECLARATION_LIST : DECLARATION_LIST
DECLARATION_LIST : DECLARATION
DECLARATION_LIST : DECLARATION_LIST DECLARATION

MAYBE_IDECLARATION_LIST : 
MAYBE_IDECLARATION_LIST : IDECLARATION_LIST
IDECLARATION_LIST : IDECLARATION
IDECLARATION_LIST : IDECLARATION_LIST FUNCTION_HEADER

// chapter 14
// keywords: as break case catch class const continue default delete do else extends false finally for function if implements import in instanceof interface internal is native new null package private protected public return super switch this throw to true try typeof use var void while with
// syntactic keywords: each get set namespace include dynamic final native override static

