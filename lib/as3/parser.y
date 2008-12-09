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
    multiname_t*multiname;
    multiname_list_t*multiname_list;
    int number_int;
    unsigned int number_uint;
    double number_float;
    struct _code*code;
    struct _typedcode value;
    struct _typedcode_list*value_list;
    struct _writeable writeable;
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
%type <code> FOR IF WHILE MAYBEELSE BREAK
%type <token> USE_NAMESPACE
%type <code> ASSIGNMENT FOR_INIT
%type <token> IMPORT
%type <multiname> MAYBETYPE
%type <token> PACKAGESPEC
%type <token> GETSET
%type <token> PARAM
%type <token> PARAMS
%type <token> PARAM_LIST
%type <token> MODIFIERS
%type <token> MODIFIER_LIST
%type <multiname_list> IMPLEMENTS_LIST
%type <multiname> EXTENDS
%type <multiname_list> EXTENDS_LIST
%type <multiname> PACKAGEANDCLASS
%type <multiname_list> PACKAGEANDCLASS_LIST
%type <token> MULTILEVELIDENTIFIER
%type <multiname> TYPE
%type <token> VAR
//%type <token> VARIABLE
%type <value> VAR_READ
%type <value> NEW
%type <token> X_IDENTIFIER
%type <token> MODIFIER
%type <token> PACKAGE
%type <value> FUNCTIONCALL
%type <value_list> MAYBE_EXPRESSION_LIST EXPRESSION_LIST

// precendence: from low to high
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
%nonassoc "!=" "==" "<=" '<' ">=" '>' // TODO: support "a < b < c" syntax?
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
    char*path;
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
    import_list_t*imports;
   
    /* class data */
    char*classname;
    abc_class_t*cls;

    array_t*vars;
    int local_var_base;
} state_t;

static state_t* state = 0;

DECLARE_LIST(state);

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
    if(oldstate)
        s->local_var_base = array_length(oldstate->vars) + oldstate->local_var_base;
    state_stack = sl;
    state = s;
    state->level++;
    state->vars = array_new();
    state->initcode = 0;
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
    printf("entering package \"%s\"\n", name);
    state->package = name;
}
static void endpackage()
{
    printf("leaving package \"%s\"\n", state->package);
    old_state();
}

char*globalclass=0;
static void startclass(token_t*modifiers, token_t*name, multiname_t*extends, multiname_list_t*implements)
{
    if(state->cls) {
        syntaxerror("inner classes now allowed"); 
    }
    new_state();
    state->classname = name->text;
    printf("entering class %s\n", name->text);
    token_list_t*t=0;
    printf("  modifiers: ");for(t=modifiers->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    printf("  extends: %s\n", multiname_tostring(extends));

    multiname_list_t*mlist=0;
    printf("  implements (%d): ", list_length(implements));
    for(mlist=implements;mlist;mlist=mlist->next)  {
        printf("%s ", multiname_tostring(mlist->multiname));
    }
    printf("\n");

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
    multiname_t* classname = 0;
    if(!public && !state->package)
        classname = multiname_new(namespace_new_private(current_filename), state->classname);
    else if(!public && state->package)
        classname = multiname_new(namespace_new_packageinternal(state->package), state->classname);
    else if(state->package)
        classname = multiname_new(namespace_new_package(state->package), state->classname);
    else
        syntaxerror("public classes only allowed inside a package");

    state->cls = abc_class_new(state->file, classname, extends);
    if(final) abc_class_final(state->cls);
    if(sealed) abc_class_sealed(state->cls);

    for(mlist=implements;mlist;mlist=mlist->next) {
        abc_class_add_interface(state->cls, mlist->multiname);
    }

    /* now write the construction code for this class */
    int slotindex = abc_initscript_addClassTrait(state->init, classname, state->cls);

    abc_method_body_t*m = state->init->method->body;
    __ getglobalscope(m);
    multiname_t*s = extends;

    int count=0;
    
    while(s) {
        //TODO: take a look at the current scope stack, maybe 
        //      we can re-use something
        s = registry_getsuperclass(s);
        if(!s) 
        break;
        __ getlex2(m, s);
        __ pushscope(m);
        m->code = m->code->prev->prev; // invert
        count++;
    }
    /* continue appending after last op end */
    while(m->code && m->code->next) m->code = m->code->next; 

    /* TODO: if this is one of *our* classes, we can also 
             do a getglobalscope/getslot <nr> (which references
             the init function's slots) */
    __ getlex2(m, extends);
    __ dup(m);
    __ pushscope(m); // we get a Verify Error #1107 if this is not the top scope
    __ newclass(m,state->cls);
    while(count--) {
        __ popscope(m);
    }
    __ setslot(m, slotindex);

    if(!globalclass && public && multiname_equals(registry_getMovieClip(),extends)) {
        if(state->package && state->package[0]) {
            globalclass = concat3str(state->package, ".", state->classname);
        } else {
            globalclass = strdup(state->classname);
        }
    }
}

static void endclass()
{
    printf("leaving class %s\n", state->classname);
    old_state();
}
static void addimport(token_t*t)
{
    NEW(import_t,i);
    i->path = t->text;
    list_append(state->imports, i);
}
static void print_imports()
{
    import_list_t*l = state->imports;
    while(l) {
        printf("  import %s\n", l->import->path);
        l = l->next;
    }
}
static void startfunction(token_t*ns, token_t*mod, token_t*getset, token_t*name,
                          token_t*params, multiname_t*type)
{
    token_list_t*t;
    new_state();
    state->function = name->text;
    printf("entering function %s\n", name->text);
    if(ns)
        printf("  namespace: %s\n", ns->text);
    printf("  getset: %s\n", getset->text);
    printf("  params: ");for(t=params->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    printf("  mod: ");for(t=mod->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    printf("  type: %s\n", multiname_tostring(type));
    print_imports();
    
    if(state->m) {
        syntaxerror("not able to start another method scope");
    }

    if(!strcmp(state->classname,name->text)) {
        state->m = abc_class_constructor(state->cls, type, 0);
    } else {
        state->m = abc_class_method(state->cls, type, name->text, 0);
    }
    array_append(state->vars, "this", 0);

    __ getlocal_0(state->m);
    __ pushscope(state->m);
}
static void endfunction()
{
    printf("leaving function %s\n", state->function);
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

int find_variable(char*name, multiname_t**m)
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

multiname_t*join_types(multiname_t*type1, multiname_t*type2, char op)
{
    return registry_getanytype(); // FIXME
}
char is_subtype_of(multiname_t*type, multiname_t*supertype)
{
    return 1; // FIXME
}

#define TYPE_ANY                  registry_getanytype()
#define TYPE_IS_ANY(t)    ((t) == registry_getanytype())
#define TYPE_INT                  registry_getintclass()
#define TYPE_IS_INT(t)    ((t) == registry_getintclass())
#define TYPE_UINT                 registry_getuintclass()
#define TYPE_IS_UINT(t)   ((t) == registry_getuintclass())
#define TYPE_FLOAT                registry_getnumberclass()
#define TYPE_IS_FLOAT(t)  ((t) == registry_getnumberclass())
#define TYPE_BOOLEAN              registry_getbooleanclass()
#define TYPE_IS_BOOLEAN(t)((t) == registry_getbooleanclass())
#define TYPE_STRING               registry_getstringclass()
#define TYPE_IS_STRING(t) ((t) == registry_getstringclass())
#define TYPE_NULL                 registry_getnullclass()
#define TYPE_IS_NULL(t)   ((t) == registry_getnullclass())

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
CODEPIECE: IF                    {$$=$1}
CODEPIECE: ASSIGNMENT            {$$=$1}
CODEPIECE: NAMESPACE_DECLARATION {/*TODO*/$$=code_new();}
CODEPIECE: FUNCTION_DECLARATION  {/*TODO*/$$=code_new();}
CODEPIECE: USE_NAMESPACE         {/*TODO*/$$=code_new();}

CODEBLOCK :  '{' MAYBECODE '}' {$$=$2;}
CODEBLOCK :  CODEPIECE ';'             {$$=$1;}
CODEBLOCK :  CODEPIECE %prec below_semicolon {$$=$1;}

/* ------------ functions --------------------------- */

FUNCTION_DECLARATION: MODIFIERS "function" GETSET T_IDENTIFIER '(' PARAMS ')' 
                      MAYBETYPE '{' {startfunction(0,$1,$3,$4,$6,$8)} MAYBECODE '}' {
    if(!state->m) syntaxerror("internal error: undefined function");
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
    if(array_contains(state->vars, $2->text))
        syntaxerror("Variable %s already defined", $2->text);
    $$ = $4.c;
   
    if(!is_subtype_of($4.t, $3)) {
        syntaxerror("Can't convert %s to %s", multiname_tostring($4.t), multiname_tostring($3));
    }

    int index = array_append(state->vars, $2->text, $3) + state->local_var_base;
    $$ = abc_setlocal($$, index);

    if($3) {
        if(TYPE_IS_INT($3) || TYPE_IS_UINT($3) || TYPE_IS_FLOAT($3)) {
            state->initcode = abc_pushbyte(state->initcode, 32);
        } else if(TYPE_IS_BOOLEAN($3)) {
            state->initcode = abc_pushfalse(state->initcode);
        } else {
            state->initcode = abc_pushnull(state->initcode);
        }
        state->initcode = abc_setlocal(state->initcode, index);
    } /*else {
        // that's the default for a local register, anyway
        state->initcode = abc_pushundefined(state->initcode);
        state->initcode = abc_setlocal(state->initcode, index);
    }*/
    printf("variable %s -> %d (%s)\n", $2->text, index, $4.t->name);
}
ASSIGNMENT :           T_IDENTIFIER '=' EXPRESSION {
    multiname_t*type=0;
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
    $$=$4.c;
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
    old_state();
}

FOR_INIT : {$$=code_new();}
FOR_INIT : ASSIGNMENT | VARIABLE_DECLARATION | VOIDEXPRESSION

FOR : "for" '(' {new_state();} FOR_INIT ';' EXPRESSION ';' VOIDEXPRESSION ')' CODEBLOCK {
    $$ = $4;
    code_t*loopstart = $$ = abc_label($$);
    $$ = code_append($$, $6.c);
    code_t*myif = $$ = abc_iffalse($$, 0);
    $$ = code_append($$, $10);
    $$ = code_append($$, $8);
    $$ = abc_jump($$, loopstart);
    $$ = abc_label($$);
    myif->branch = $$;
    old_state();
}

WHILE : "while" '(' {new_state();} EXPRESSION ')' CODEBLOCK {
    code_t*myjmp = $$ = abc_jump(0, 0);
    code_t*loopstart = $$ = abc_label($$);
    $$ = code_append($$, $6);
    myjmp->branch = $$ = abc_label($$);
    $$ = code_append($$, $4.c);
    $$ = abc_iftrue($$, loopstart);
    old_state();
}

BREAK : "break" {
    $$ = abc___break__(0);
}

/* ------------ packages and imports ---------------- */

PACKAGE_DECLARATION : "package" MULTILEVELIDENTIFIER '{' {startpackage($2)} MAYBECODE '}' {endpackage()}
PACKAGE_DECLARATION : "package" '{' {startpackage(0)} MAYBECODE '}' {endpackage()}

IMPORT : "import" PACKAGESPEC {addimport($2);}

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
FUNCTION_HEADER:      MODIFIERS "function" GETSET T_IDENTIFIER '(' PARAMS ')' 
                      MAYBETYPE

NAMESPACE_DECLARATION : MODIFIERS KW_NAMESPACE T_IDENTIFIER
NAMESPACE_DECLARATION : MODIFIERS KW_NAMESPACE T_IDENTIFIER '=' T_IDENTIFIER
NAMESPACE_DECLARATION : MODIFIERS KW_NAMESPACE T_IDENTIFIER '=' T_STRING

//NAMESPACE :              {$$=empty_token();}
//NAMESPACE : T_IDENTIFIER {$$=$1};

CONSTANT : T_BYTE {$$.c = abc_pushbyte(0, $1);
                   $$.c = abc_coerce2($$.c, registry_getintclass()); // FIXME
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


EXPRESSION : E %prec prec_none  /*precendence below '-x'*/ {$$ = $1;}
VOIDEXPRESSION : E %prec prec_none {$$=$1.c;/*calculate and discard*/$$=abc_pop($$);}

E : CONSTANT
E : VAR_READ %prec T_IDENTIFIER {$$ = $1;}
E : NEW                         {$$.c = abc_pushundefined(0); /* FIXME */
                                 $$.t = TYPE_ANY;
                                }
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
               $$.c=abc_coerce2($$.c, registry_getintclass()); // FIXME
               $$.c=abc_dup($$.c);$$.c=code_append($$.c,$1.write);
               $$.t = $1.type;
              }
E : LH "-=" E {$$.c = $1.read;$$.c=code_append($$.c,$3.c);$$.c=abc_add($$.c);
               $$.c=abc_coerce2($$.c, registry_getintclass()); // FIXME
               $$.c=abc_dup($$.c);$$.c=code_append($$.c,$1.write);
               $$.t = $1.type;
              }

// TODO: use inclocal where appropriate
E : LH "++" {$$.c = $1.read;$$.c=abc_increment($$.c);
             $$.c=abc_coerce2($$.c, registry_getintclass()); //FIXME
             $$.c=abc_dup($$.c);$$.c=code_append($$.c,$1.write);
             $$.t = $1.type;
            }
E : LH "--" {$$.c = $1.read;$$.c=abc_decrement($$.c);
             $$.c=abc_coerce2($$.c, registry_getintclass()); //FIXME
             $$.c=abc_dup($$.c);$$.c=code_append($$.c,$1.write);
             $$.t = $1.type;
            }

LH: T_IDENTIFIER {
  int i = find_variable($1->text, &$$.type);
  $$.read = abc_getlocal(0, i);
  $$.write = abc_setlocal(0, i);
}

NEW : "new" T_IDENTIFIER                         {$$.c=0;$$.t=0;/*FIXME*/}
    | "new" T_IDENTIFIER '(' ')'                 {$$.c=0;$$.t=0;/*FIXME*/}
    | "new" T_IDENTIFIER '(' EXPRESSION_LIST ')' {$$.c=0;$$.t=0;/*FIXME*/}

FUNCTIONCALL : T_IDENTIFIER '(' MAYBE_EXPRESSION_LIST ')' {
        /* TODO: use abc_call (for calling local variables),
                 abc_callstatic (for calling own methods) */
        $$.c = code_new();
        $$.c = abc_findpropstrict($$.c, $1->text);
        typedcode_list_t*l = $3;
        // push parameters on stack
        int len = 0;
        while(l) {
            $$.c = code_append($$.c, l->typedcode.c);
            l = l->nxt;
            len ++;
        }
        $$.c = abc_callproperty($$.c, $1->text, len);
}

MAYBE_EXPRESSION_LIST : {$$=0;}
MAYBE_EXPRESSION_LIST : EXPRESSION_LIST
EXPRESSION_LIST : EXPRESSION                     {$$=malloc(sizeof(typedcode_list_t));
                                                  $$->nxt = 0;
                                                  $$->typedcode = $1;}
EXPRESSION_LIST : EXPRESSION_LIST ',' EXPRESSION {$$=malloc(sizeof(typedcode_list_t));
                                                  $$->nxt = $1;
                                                  $$->typedcode = $3;
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

PACKAGESPEC : PACKAGESPEC '.' PACKAGESPEC {if($1->text[0]=='*') syntaxerror("wildcard in the middle of path");
                                           $$ = concat3($1,$2,$3);}
PACKAGESPEC : X_IDENTIFIER                {$$=$1;}
PACKAGESPEC : '*'                         {$$=$1;}

GETSET : "get" {$$=$1;}
       | "set" {$$=$1;}
       |       {$$=empty_token();}

CLASS_DECLARATION : MODIFIERS "class" T_IDENTIFIER EXTENDS IMPLEMENTS_LIST '{' {startclass($1,$3,$4,$5);} MAYBE_DECLARATION_LIST '}' {endclass();}
INTERFACE_DECLARATION : MODIFIERS "interface" T_IDENTIFIER EXTENDS_LIST '{' MAYBE_IDECLARATION_LIST '}'

PARAMS: {$$=empty_token();}
PARAMS: PARAM_LIST {$$=$1;}
PARAM_LIST: PARAM_LIST ',' PARAM {extend($1,$3);$$=$1;}
PARAM_LIST: PARAM                {$$=empty_token();extend($$,$1);}
PARAM:  T_IDENTIFIER ':' TYPE {$$=$1;}

MODIFIERS : {$$=empty_token();}
MODIFIERS : MODIFIER_LIST {$$=$1}
MODIFIER_LIST : MODIFIER MODIFIER_LIST {extend($2,$1);$$=$2;}
MODIFIER_LIST : MODIFIER               {$$=empty_token();extend($$,$1);}
MODIFIER : KW_PUBLIC | KW_PRIVATE | KW_PROTECTED | KW_STATIC | KW_DYNAMIC | KW_FINAL | KW_OVERRIDE | KW_NATIVE | KW_INTERNAL

DECLARATION : VARIABLE_DECLARATION
DECLARATION : FUNCTION_DECLARATION

IDECLARATION : VARIABLE_DECLARATION
IDECLARATION : FUNCTION_DECLARATION

IMPLEMENTS_LIST : {$$=list_new();}
IMPLEMENTS_LIST : KW_IMPLEMENTS PACKAGEANDCLASS_LIST {$$=$2;}

EXTENDS : {$$=registry_getobjectclass();}
EXTENDS : KW_EXTENDS PACKAGEANDCLASS {$$=$2;}

EXTENDS_LIST : {$$=list_new();}
EXTENDS_LIST : KW_EXTENDS PACKAGEANDCLASS_LIST {$$=$2;}

//IDENTIFIER_LIST : T_IDENTIFIER ',' IDENTIFIER_LIST {extend($3,$1);$$=$3;}
//IDENTIFIER_LIST : T_IDENTIFIER                     {$$=empty_token();extend($$,$1);}

PACKAGEANDCLASS : T_IDENTIFIER {$$ = registry_findclass(state->package, $1->text);}
PACKAGEANDCLASS : PACKAGE '.' T_IDENTIFIER {$$ = registry_findclass($1->text, $3->text);}
PACKAGE : X_IDENTIFIER
PACKAGE : PACKAGE '.' X_IDENTIFIER {$$=$1;extend_s($$,".",$3);}

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

