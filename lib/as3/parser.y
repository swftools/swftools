//%glr-parser
//%expect-rr 1
%error-verbose

%token T_IDENTIFIER
%token T_STRING
%token T_REGEXP
%token T_DOTDOT ".."
%token T_COLONCOLON "::"
%token T_GE ">="
%token T_LE "<="
%token T_EQEQ "=="
%token T_PLUSPLUS "++"
%token T_MINUSMINUS "--"
%token T_IMPLEMENTS
%token T_NAMESPACE
%token T_PACKAGE
%token T_PROTECTED
%token T_PUBLIC
%token T_PRIVATE
%token T_UINT
%token T_USE
%token T_INTERNAL
%token T_INT
%token T_NEW
%token T_NATIVE
%token T_FUNCTION
%token T_FOR
%token T_CLASS
%token T_CONST
%token T_SET
%token T_STATIC
%token T_IMPORT
%token T_INTERFACE
%token T_NUMBER
%token T_NULL
%token T_FALSE
%token T_TRUE
%token T_BOOLEAN
%token T_VAR
%token T_AS
%token T_IS
%token T_DYNAMIC
%token T_OVERRIDE
%token T_FINAL
%token T_GET
%token T_EXTENDS


%token T_EMPTY
     
%{
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "abc.h"
#include "pool.h"
#include "files.h"
#include "tokenizer.h"
#include "registry.h"

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

static void initialize_state()
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
static void finalize_state()
{
    if(state->level) {
        syntaxerror("unexpected end of file");
    }
    abc_method_body_t*m = state->init->method->body;
    //__ popscope(m);
    __ returnvoid(m);
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

static void startclass(token_t*modifiers, token_t*name, token_t*extends, token_t*implements)
{
    token_list_t*t;
    if(state->cls) {
        syntaxerror("inner classes now allowed"); 
    }
    new_state();
    state->classname = name->text;
    printf("entering class %s\n", name->text);
    printf("  modifiers: ");for(t=modifiers->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    printf("  extends: %s\n", extends->text);
    printf("  implements (%d): ", list_length(implements->tokens));for(t=implements->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");

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

    /* try to find the superclass */
    multiname_t* superclass = 0;
    if(extends->type != T_EMPTY) {
        superclass = registry_findclass(extends->text);
    } else {
        superclass = registry_getobjectclass();
    }

    state->cls = abc_class_new(state->file, classname, superclass);
    if(final) abc_class_final(state->cls);
    if(sealed) abc_class_sealed(state->cls);

    for(t=implements->tokens;t;t=t->next) {
        abc_class_add_interface(state->cls, registry_findclass(t->token->text));
    }

    /* now write the construction code for this class */
    int slotindex = abc_initscript_addClassTrait(state->init, classname, state->cls);

    abc_method_body_t*m = state->init->method->body;
    __ getglobalscope(m);
    multiname_t*s = superclass;
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
    __ getlex2(m, superclass);
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
                          token_t*params, token_t*type)
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
    printf("  type: %s\n", type->text);
    print_imports();
}
static void endfunction()
{
    printf("leaving function %s\n", state->function);
    old_state();
}
static int newvariable(token_t*mod, token_t*varconst, token_t*name, token_t*type)
{
    token_list_t*t;
    printf("defining new variable %s\n", name->text);
    printf("  mod: ");for(t=mod->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    printf("  access: ");printf("%s\n", varconst->text);
    printf("  type: ");printf("%s\n", type->text);
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

PROGRAM: CODE PROGRAM
PROGRAM: 

//EMPTY: {
//    token_t* t = malloc(sizeof(token_t));
//    t->text = strdup("");
//    t->type = T_EMPTY;
//    $$ = t;
//}

CODE: CODE CODEPIECE {$$=$1;}
CODE: CODEPIECE {$$=empty_token();}

MAYBECODE: CODE
MAYBECODE: 

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

TYPE : BUILTIN_TYPE | '*' | T_IDENTIFIER
// TODO: do we need this? all it does it is clutter up our keyword space
BUILTIN_TYPE : T_STRING
BUILTIN_TYPE : T_NUMBER
BUILTIN_TYPE : T_INT
BUILTIN_TYPE : T_UINT
BUILTIN_TYPE : T_BOOLEAN
BUILTIN_TYPE : T_NULL

MAYBETYPE: ':' TYPE {$$=$2;}
MAYBETYPE:          {$$=empty_token();}

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
CONSTANT : T_TRUE | T_FALSE
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

// operator prescendence:
// http://livedocs.adobe.com/flash/9.0/main/wwhelp/wwhimpl/common/html/wwhelp.htm?context=LiveDocs_Parts&file=00000012.html
EXPRESSION : EXPRESSION '<' EXPRESSION
EXPRESSION : EXPRESSION '>' EXPRESSION
EXPRESSION : EXPRESSION "<=" EXPRESSION
EXPRESSION : EXPRESSION ">=" EXPRESSION
EXPRESSION : EXPRESSION "==" EXPRESSION
EXPRESSION : EXPRESSION '+' TERM
EXPRESSION : EXPRESSION '-' TERM
EXPRESSION : TERM
EXPRESSION : '-' TERM
TERM : TERM '*' FACTOR
TERM : TERM '/' FACTOR
TERM : EMOD
EMOD: FACTOR "++"
EMOD: FACTOR "--"
EMOD: FACTOR
FACTOR : '(' EXPRESSION ')'
FACTOR : CONSTANT
FACTOR : VARIABLE
FACTOR : FUNCTIONCALL
FACTOR : T_REGEXP
FACTOR : NEW
FACTOR : IS
FACTOR : AS

IS : EXPRESSION T_IS TYPE
AS : EXPRESSION T_AS TYPE
NEW : T_NEW T_IDENTIFIER | T_NEW T_IDENTIFIER '(' ')'
NEW : T_NEW T_IDENTIFIER '(' EXPRESSIONLIST ')'

FUNCTIONCALL : VARIABLE '(' EXPRESSIONLIST ')'
FUNCTIONCALL : VARIABLE '(' ')'

EXPRESSIONLIST : EXPRESSION
EXPRESSIONLIST : EXPRESSION ',' EXPRESSIONLIST

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

IMPLEMENTS_LIST : {$$=empty_token();}
IMPLEMENTS_LIST : T_IMPLEMENTS MIDENTIFIER_LIST {$$=$2;}

EXTENDS : {$$=empty_token();}
EXTENDS : T_EXTENDS MULTILEVELIDENTIFIER {$$=$2;}

EXTENDS_LIST : {$$=empty_token();}
EXTENDS_LIST : T_EXTENDS MIDENTIFIER_LIST {$$=$2;}

//IDENTIFIER_LIST : T_IDENTIFIER ',' IDENTIFIER_LIST {extend($3,$1);$$=$3;}
//IDENTIFIER_LIST : T_IDENTIFIER                     {$$=empty_token();extend($$,$1);}

MULTILEVELIDENTIFIER : T_IDENTIFIER                          {$$=empty_token();extend($$,$1);}
MULTILEVELIDENTIFIER : MULTILEVELIDENTIFIER '.' X_IDENTIFIER {extend_s($1,".",$3);$$=$1;}

MIDENTIFIER_LIST : MULTILEVELIDENTIFIER                      {$$=empty_token();extend($$,$1);}
MIDENTIFIER_LIST : MIDENTIFIER_LIST ',' MULTILEVELIDENTIFIER {extend($1,$3);$$=$1;}

MAYBE_DECLARATION_LIST : 
MAYBE_DECLARATION_LIST : DECLARATION_LIST
DECLARATION_LIST : DECLARATION
DECLARATION_LIST : DECLARATION_LIST DECLARATION

MAYBE_IDECLARATION_LIST : 
MAYBE_IDECLARATION_LIST : IDECLARATION_LIST
IDECLARATION_LIST : IDECLARATION
IDECLARATION_LIST : IDECLARATION_LIST FUNCTION_HEADER

// keywords: as break case catch class const continue default delete do else extends false finally for function if implements import in instanceof interface internal is native new null package private protected public return super switch this throw to true try typeof use var void while with
// syntactic keywords: each get set namespace include dynamic final native override static
// chapter 14


%%

#ifdef MAIN

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>

void test_lexer()
{
    while(1) {
        int token = yylex();
        if(token==T_EOF)
            break;
        if(token>=32 && token<256) {
            printf("'%c'\n", token);
        } else {
            printf("%s\n", token2string(avm2_lval));
        }
    }
}

int main(int argn, char*argv[])
{
    //FILE*fi = fopen("/home/kramm/c/flex/text.as", "rb");
    char*filename = "include.as";
    char buf[512];
    if(argn>1)
        filename=argv[1];

    add_include_dir(getcwd(buf, 512));
    char*fullfilename = enter_file(filename, 0);

    FILE*fi = fopen(fullfilename, "rb");
    if(!fi) {
        perror(fullfilename);
        return 1;
    }
    initialize_state();
    avm2_set_in(fi);

    if(argn>2 && !strcmp(argv[2], "-lex")) {
        test_lexer();
        return 0;
    }
    avm2_parse();

    finalize_state();

    SWF swf;
    memset(&swf, 0, sizeof(swf));
    swf.fileVersion = 9;
    swf.frameRate = 0x2500;
    swf.movieSize.xmin = swf.movieSize.ymin = 0;
    swf.movieSize.xmax = 1024*20;
    swf.movieSize.ymax = 768*20;
    TAG*tag = swf.firstTag = swf_InsertTag(0, ST_DOABC);
    swf_WriteABC(tag, state->file);
    swf_InsertTag(tag, ST_END);

    int f = open("abc.swf",O_RDWR|O_CREAT|O_TRUNC|O_BINARY,0644);
    swf_WriteSWF(f,&swf);
    close(f);

    return 0;
}
#endif
