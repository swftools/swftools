%{
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "abc.h"
#include "pool.h"
#include "files.h"
#include "tokenizer.h"
#include "registry.h"
%}

//%glr-parser
//%expect-rr 1
%error-verbose

%union tokenunion {
    tokenptr_t token;
    multiname_t*multiname;
    multiname_list_t*multiname_list;
}


%token<token> T_IDENTIFIER
%token<token> T_STRING
%token<token> T_REGEXP
%token<token> T_IMPLEMENTS
%token<token> T_NAMESPACE
%token<token> T_PACKAGE
%token<token> T_PROTECTED
%token<token> T_PUBLIC
%token<token> T_PRIVATE
%token<token> T_UINT
%token<token> T_USE
%token<token> T_INTERNAL
%token<token> T_INT
%token<token> T_NEW
%token<token> T_NATIVE
%token<token> T_FUNCTION
%token<token> T_FOR
%token<token> T_CLASS
%token<token> T_CONST
%token<token> T_SET
%token<token> T_STATIC
%token<token> T_IMPORT
%token<token> T_INTERFACE
%token<token> T_NUMBER
%token<token> T_NULL
%token<token> T_FALSE
%token<token> T_TRUE
%token<token> T_BOOLEAN
%token<token> T_VAR
%token<token> T_DYNAMIC
%token<token> T_OVERRIDE
%token<token> T_FINAL
%token<token> T_GET
%token<token> T_EXTENDS
%token<token> T_EMPTY
%token<token> T_EQEQ "=="
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
%token<token> T_IS "is"
%token<token> T_AS "as"
%token<token> T_SEMICOLON ';'
%token<token> T_STAR '*'
%token<token> T_DOT '.'

// precendence: from low to high
// http://livedocs.adobe.com/flash/9.0/main/wwhelp/wwhimpl/common/html/wwhelp.htm?context=LiveDocs_Parts&file=00000012.html

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
%left '('

%type <token> CODE
%type <token> CODEPIECE
%type <token> PACKAGE_DECLARATION
%type <token> FUNCTION_DECLARATION
%type <token> VARIABLE_DECLARATION
%type <token> CLASS_DECLARATION
%type <token> NAMESPACE_DECLARATION
%type <token> INTERFACE_DECLARATION
%type <token> EXPRESSION
%type <token> E
%type <token> CONSTANT
%type <token> FOR
%type <token> USE
%type <token> ASSIGNMENT
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
%type <token> VARIABLE
%type <token> NEW
%type <token> FUNCTIONCALL
%type <token> X_IDENTIFIER
%type <token> MODIFIER
%type <token> PACKAGE

     
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
    import_list_t*imports;
   
    /* class data */
    char*classname;
    abc_class_t*cls;

} state_t;

static state_t* state = 0;

DECLARE_LIST(state);

static state_list_t*state_stack=0;

void initialize_state()
{
    NEW(state_t, s);
    NEW(state_list_t, sl);
    state_stack = sl;
    state = sl->state = s;

    state->file = abc_file_new();
    state->level = 0;
    
    state->init = abc_initscript(state->file, 0, 0);
    abc_method_body_t*m = state->init->method->body;
    __ getlocal_0(m);
    __ pushscope(m);
}
void* finalize_state()
{
    if(state->level) {
        syntaxerror("unexpected end of file");
    }
    abc_method_body_t*m = state->init->method->body;
    //__ popscope(m);
    __ returnvoid(m);
    return state->file;
}

static void new_state()
{
    NEW(state_t, s);
    NEW(state_list_t, sl);
    memcpy(s, state, sizeof(state_t)); //shallow copy
    sl->next = state_stack;
    sl->state = s;
    state_stack = sl;
    state = s;
    state->level++;
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
        if(t->token->type == T_INTERNAL) {
            /* the programmer is being explicit- 
               being internal is the default anyway */
            internal = 1;
        } else if(t->token->type == T_PUBLIC) {
            public = 1;
        } else if(t->token->type == T_FINAL) {
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
        //TODO: invert
        //TODO: take a look at the current scope stack, maybe 
        //      we can re-use something
        __ getlex2(m, s);
        __ pushscope(m);
        s = registry_getsuperclass(s);
        count++;
    }
    /* TODO: if this is one of *our* classes, we can also 
             do a getglobalscope/getslot <nr> (which references
             the init function's slots) */
    __ getlex2(m, extends);
    __ newclass(m,state->cls);

    while(count--) {
        __ popscope(m);
    }
    __ setslot(m, slotindex);
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

    abc_method_body_t* m=0;
    if(!strcmp(state->classname,name->text)) {
        m = abc_class_constructor(state->cls, type, 0);
    } else {
        m = abc_class_method(state->cls, type, name->text, 0);
    }

}
static void endfunction()
{
    printf("leaving function %s\n", state->function);
    old_state();
}
static int newvariable(token_t*mod, token_t*varconst, token_t*name, multiname_t*type)
{
    token_list_t*t;
    printf("defining new variable %s\n", name->text);
    printf("  mod: ");for(t=mod->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    printf("  access: ");printf("%s\n", varconst->text);
    printf("  type: ");printf("%s\n", multiname_tostring(type));
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

%}

%%

PROGRAM: MAYBECODE

MAYBECODE: CODE
MAYBECODE: 

CODE: CODE CODEPIECE {$$=$1;}
CODE: CODEPIECE {$$=empty_token();}

CODEPIECE: ';'
CODEPIECE: VARIABLE_DECLARATION {$$=$1;}
CODEPIECE: PACKAGE_DECLARATION
CODEPIECE: IMPORT
CODEPIECE: NAMESPACE_DECLARATION
CODEPIECE: CLASS_DECLARATION
CODEPIECE: INTERFACE_DECLARATION
CODEPIECE: FUNCTION_DECLARATION
CODEPIECE: EXPRESSION
CODEPIECE: FOR
CODEPIECE: USE
CODEPIECE: ASSIGNMENT

PACKAGE_DECLARATION : T_PACKAGE MULTILEVELIDENTIFIER '{' {startpackage($2)} MAYBECODE '}' {endpackage()}
PACKAGE_DECLARATION : T_PACKAGE '{' {startpackage(0)} MAYBECODE '}' {endpackage()}

IMPORT : T_IMPORT PACKAGESPEC {addimport($2);}

TYPE : PACKAGEANDCLASS {$$=$1;}
     | '*'        {$$=registry_getanytype();}
     |  T_STRING  {$$=registry_getstringclass();}
     |  T_INT     {$$=registry_getintclass();}
     |  T_UINT    {$$=registry_getuintclass();}
     |  T_BOOLEAN {$$=registry_getbooleanclass();}
     |  T_NUMBER  {$$=registry_getnumberclass();}

MAYBETYPE: ':' TYPE {$$=$2;}
MAYBETYPE:          {$$=0;}

//FUNCTION_HEADER:      NAMESPACE MODIFIERS T_FUNCTION GETSET T_IDENTIFIER '(' PARAMS ')' 
FUNCTION_HEADER:      MODIFIERS T_FUNCTION GETSET T_IDENTIFIER '(' PARAMS ')' 
                      MAYBETYPE
FUNCTION_DECLARATION: MODIFIERS T_FUNCTION GETSET T_IDENTIFIER '(' PARAMS ')' 
                      MAYBETYPE '{' {startfunction(0,$1,$3,$4,$6,$8)} MAYBECODE '}' {endfunction()}

NAMESPACE_DECLARATION : MODIFIERS T_NAMESPACE T_IDENTIFIER
NAMESPACE_DECLARATION : MODIFIERS T_NAMESPACE T_IDENTIFIER '=' T_IDENTIFIER
NAMESPACE_DECLARATION : MODIFIERS T_NAMESPACE T_IDENTIFIER '=' T_STRING

//NAMESPACE :              {$$=empty_token();}
//NAMESPACE : T_IDENTIFIER {$$=$1};

CONSTANT : T_NUMBER
CONSTANT : T_STRING
CONSTANT : T_TRUE
CONSTANT : T_FALSE
CONSTANT : T_NULL

VAR : T_CONST | T_VAR

// type annotation
// TODO: NAMESPACE

VARIABLE_DECLARATION : MODIFIERS VAR T_IDENTIFIER MAYBETYPE {
    int i = newvariable($1,$2,$3,$4);
}
VARIABLE_DECLARATION : MODIFIERS VAR T_IDENTIFIER MAYBETYPE '=' EXPRESSION {
    int i = newvariable($1,$2,$3,$4);
    //setvariable(i,$6);
}

EXPRESSION : E

E : CONSTANT
E : VARIABLE
E : NEW
E : T_REGEXP
E : FUNCTIONCALL
E : E '<' E
E : E '>' E
E : E "<=" E
E : E ">=" E
E : E "==" E
E : E '+' E
E : E '-' E
E : E '/' E
E : E '%' E
E : E '*' E
E : E "++"
E : E "--"
E : E "as" TYPE
E : E "is" TYPE

//E : '(' E ')'  // conflicts with function calls: "a=f(c++);"<->"a=f;(c++)"
//E : '-' E      // conflicts with non-assignment statements: "a=3-1;"<->"a=3;-1"

NEW : T_NEW T_IDENTIFIER
    | T_NEW T_IDENTIFIER '(' ')'
    | T_NEW T_IDENTIFIER '(' EXPRESSION_LIST ')'

FUNCTIONCALL : VARIABLE '(' EXPRESSION_LIST ')'
FUNCTIONCALL : VARIABLE '(' ')'

EXPRESSION_LIST : EXPRESSION
EXPRESSION_LIST : EXPRESSION_LIST ',' EXPRESSION

VARIABLE : T_IDENTIFIER
VARIABLE : VARIABLE '.' T_IDENTIFIER
VARIABLE : VARIABLE ".." T_IDENTIFIER // descendants
VARIABLE : VARIABLE "::" VARIABLE // namespace declaration
VARIABLE : VARIABLE "::" '[' EXPRESSION ']' // qualified expression
VARIABLE : VARIABLE '[' EXPRESSION ']' // unqualified expression

ASSIGNMENT :           VARIABLE           '=' EXPRESSION
NEW_ASSIGNMENT : T_VAR VARIABLE MAYBETYPE '=' EXPRESSION

FOR : T_FOR '(' NEW_ASSIGNMENT ';' EXPRESSION ';' EXPRESSION ')' '{' MAYBECODE '}'
FOR : T_FOR '(' ASSIGNMENT     ';' EXPRESSION ';' EXPRESSION ')' '{' MAYBECODE '}'

USE : T_USE T_NAMESPACE T_IDENTIFIER

// keywords which also may be identifiers
X_IDENTIFIER : T_IDENTIFIER | T_PACKAGE

PACKAGESPEC : PACKAGESPEC '.' PACKAGESPEC {if($1->text[0]=='*') syntaxerror("wildcard in the middle of path");
                                           $$ = concat3($1,$2,$3);}
PACKAGESPEC : X_IDENTIFIER                {$$=$1;}
PACKAGESPEC : '*'                         {$$=$1;}

GETSET : T_GET {$$=$1;}
       | T_SET {$$=$1;}
       |       {$$=empty_token();}

CLASS_DECLARATION : MODIFIERS T_CLASS T_IDENTIFIER EXTENDS IMPLEMENTS_LIST '{' {startclass($1,$3,$4,$5);} MAYBE_DECLARATION_LIST '}' {endclass();}
INTERFACE_DECLARATION : MODIFIERS T_INTERFACE T_IDENTIFIER EXTENDS_LIST '{' MAYBE_IDECLARATION_LIST '}'

PARAMS: {$$=empty_token();}
PARAMS: PARAM_LIST {$$=$1;}
PARAM_LIST: PARAM_LIST ',' PARAM {extend($1,$3);$$=$1;}
PARAM_LIST: PARAM                {$$=empty_token();extend($$,$1);}
PARAM:  T_IDENTIFIER ':' TYPE {$$=$1;}

MODIFIERS : {$$=empty_token();}
MODIFIERS : MODIFIER_LIST {$$=$1}
MODIFIER_LIST : MODIFIER MODIFIER_LIST {extend($2,$1);$$=$2;}
MODIFIER_LIST : MODIFIER               {$$=empty_token();extend($$,$1);}
MODIFIER : T_PUBLIC | T_PRIVATE | T_PROTECTED | T_STATIC | T_DYNAMIC | T_FINAL | T_OVERRIDE | T_NATIVE | T_INTERNAL

DECLARATION : VARIABLE_DECLARATION
DECLARATION : FUNCTION_DECLARATION

IDECLARATION : VARIABLE_DECLARATION
IDECLARATION : FUNCTION_DECLARATION

IMPLEMENTS_LIST : {$$=list_new();}
IMPLEMENTS_LIST : T_IMPLEMENTS PACKAGEANDCLASS_LIST {$$=$2;}

EXTENDS : {$$=registry_getobjectclass();}
EXTENDS : T_EXTENDS PACKAGEANDCLASS {$$=$2;}

EXTENDS_LIST : {$$=list_new();}
EXTENDS_LIST : T_EXTENDS PACKAGEANDCLASS_LIST {$$=$2;}

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

