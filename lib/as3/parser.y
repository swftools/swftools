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
    struct _code_list*code_list;
}


%token<token> T_IDENTIFIER
%token<token> T_STRING
%token<token> T_REGEXP
%token<token> T_EMPTY
%token<number_int> T_INT
%token<number_uint> T_UINT
%token<number_uint> T_BYTE
%token<number_uint> T_SHORT
%token<number_float> T_FLOAT

%token<token> KW_IMPLEMENTS
%token<token> KW_NAMESPACE
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
%token<token> KW_FALSE "False"
%token<token> KW_TRUE "True"
%token<token> KW_BOOLEAN "Boolean"
%token<token> KW_UINT "uint"
%token<token> KW_INT "int"
%token<token> KW_NUMBER "Number"
%token<token> KW_STRING "String"
%token<token> KW_IS "is"
%token<token> KW_AS "as"

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
%token<token> T_SEMICOLON ';'
%token<token> T_STAR '*'
%token<token> T_DOT '.'

%type <token> CODE
%type <code> CODEPIECE CODEPIECE2
%type <token> PACKAGE_DECLARATION
%type <token> FUNCTION_DECLARATION
%type <code> VARIABLE_DECLARATION
%type <token> CLASS_DECLARATION
%type <token> NAMESPACE_DECLARATION
%type <token> INTERFACE_DECLARATION
%type <code> EXPRESSION
%type <code> MAYBEEXPRESSION
%type <code> E
%type <code> CONSTANT
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
%type <code> VAR_READ
%type <token> NEW
%type <token> X_IDENTIFIER
%type <token> MODIFIER
%type <token> PACKAGE
%type <code> FUNCTIONCALL
%type <code_list> MAYBE_EXPRESSION_LIST EXPRESSION_LIST

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
%left '('

     
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
    abc_method_body_t*m;
    import_list_t*imports;
   
    /* class data */
    char*classname;
    abc_class_t*cls;

    array_t*vars;

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
    state->file->flags &= ~ABCFILE_LAZY;
    state->level = 0;
    
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
    if(state->level) {
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

static void new_state()
{
    NEW(state_t, s);
    NEW(state_list_t, sl);

    if(state->m) {
        syntaxerror("not able to start another method scope");
    }

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

    if(!strcmp(state->classname,name->text)) {
        state->m = abc_class_constructor(state->cls, type, 0);
    } else {
        state->m = abc_class_method(state->cls, type, name->text, 0);
    }
    state->vars = array_new();
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
static int newvariable(token_t*mod, token_t*varconst, token_t*name, multiname_t*type)
{
    token_list_t*t;
    printf("defining new variable %s\n", name->text);
    printf("  mod: ");for(t=mod->tokens;t;t=t->next) printf("%s ", t->token->text);printf("\n");
    printf("  access: ");printf("%s\n", varconst->text);
    printf("  type: ");printf("%s\n", multiname_tostring(type));

    if(!state->vars) 
        syntaxerror("not allowed to defined variables outside a method");

    int index = array_find(state->vars, name->text);
    if(index>=0) {
        syntaxerror("Variable %s already defined", name->text);
    } else {
        index = array_append(state->vars, name->text, 0);
    }
    return index;
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

CODE: CODE CODEPIECE2 {$$=$1;}
CODE: CODEPIECE2 {$$=empty_token();}

CODEPIECE2: CODEPIECE {
    if(state->m) {
        state->m->code = code_append(state->m->code, $1);
    }
}

CODEPIECE: ';'                   {/*TODO*/$$=code_new();}
CODEPIECE: VARIABLE_DECLARATION  {$$=$1}
CODEPIECE: PACKAGE_DECLARATION   {/*TODO*/$$=code_new();}
CODEPIECE: IMPORT                {/*TODO*/$$=code_new();}
CODEPIECE: NAMESPACE_DECLARATION {/*TODO*/$$=code_new();}
CODEPIECE: CLASS_DECLARATION     {/*TODO*/$$=code_new();}
CODEPIECE: INTERFACE_DECLARATION {/*TODO*/$$=code_new();}
CODEPIECE: FUNCTION_DECLARATION  {/*TODO*/$$=code_new();}
CODEPIECE: EXPRESSION            {/*calculate and discard*/$$=$1;$$=abc_pop($$);}
CODEPIECE: FOR                   {/*TODO*/$$=code_new();}
CODEPIECE: USE                   {/*TODO*/$$=code_new();}
CODEPIECE: ASSIGNMENT            {/*TODO*/$$=code_new();}

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
FUNCTION_DECLARATION: MODIFIERS "function" GETSET T_IDENTIFIER '(' PARAMS ')' 
                      MAYBETYPE '{' {startfunction(0,$1,$3,$4,$6,$8)} MAYBECODE '}' {endfunction()}

NAMESPACE_DECLARATION : MODIFIERS KW_NAMESPACE T_IDENTIFIER
NAMESPACE_DECLARATION : MODIFIERS KW_NAMESPACE T_IDENTIFIER '=' T_IDENTIFIER
NAMESPACE_DECLARATION : MODIFIERS KW_NAMESPACE T_IDENTIFIER '=' T_STRING

//NAMESPACE :              {$$=empty_token();}
//NAMESPACE : T_IDENTIFIER {$$=$1};

CONSTANT : T_BYTE {$$ = abc_pushbyte(0, $1);}
CONSTANT : T_SHORT {$$ = abc_pushshort(0, $1);}
CONSTANT : T_INT {$$ = abc_pushint(0, $1);}
CONSTANT : T_UINT {$$ = abc_pushuint(0, $1);}
CONSTANT : T_FLOAT {$$ = abc_pushdouble(0, $1);}
CONSTANT : T_STRING {$$ = abc_pushstring(0, $1->text);}
CONSTANT : KW_TRUE {$$ = abc_pushtrue(0);}
CONSTANT : KW_FALSE {$$ = abc_pushfalse(0);}
CONSTANT : KW_NULL {$$ = abc_pushnull(0);}

VAR : "const" | "var"

// type annotation
// TODO: NAMESPACE

VARIABLE_DECLARATION : MODIFIERS VAR T_IDENTIFIER MAYBETYPE MAYBEEXPRESSION {
    int i = newvariable($1,$2,$3,$4);
    $$=code_new();
    $$ = $5;
    $$ = abc_setlocal($$, i);
}

MAYBEEXPRESSION : '=' EXPRESSION {$$=$2;}
                |                {$$=code_new();}

EXPRESSION : E %prec prec_none  /*precendence below '-x'*/ {$$ = $1;}

E : CONSTANT
E : VAR_READ %prec T_IDENTIFIER {$$ = $1;}
E : NEW                         {$$ = abc_pushundefined(0); /* FIXME */}
E : T_REGEXP                    {$$ = abc_pushundefined(0); /* FIXME */}
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
E : '(' E ')' {$$=$2;}
E : '-' E {$$=$2;}

NEW : "new" T_IDENTIFIER
    | "new" T_IDENTIFIER '(' ')'
    | "new" T_IDENTIFIER '(' EXPRESSION_LIST ')'

FUNCTIONCALL : T_IDENTIFIER '(' MAYBE_EXPRESSION_LIST ')' {
        /* TODO: use abc_call (for calling local variables),
                 abc_callstatic (for calling own methods) */
        $$ = code_new();
        $$ = abc_findproperty($$, $1->text);
        code_list_t*l = $3;
        // push parameters on stack
        while(l) {
            $$ = code_append($$, l->code);
            l = l->next;
        }
        $$ = abc_callproperty($$, $1->text, list_length($3));
}

MAYBE_EXPRESSION_LIST : {}
MAYBE_EXPRESSION_LIST : EXPRESSION_LIST
EXPRESSION_LIST : EXPRESSION                     {$$=list_new();list_append($$,$1);}
EXPRESSION_LIST : EXPRESSION_LIST ',' EXPRESSION {list_append($$,$3);}

VAR_READ : T_IDENTIFIER {
        int i = array_find(state->vars, $1->text);
        if(i<0)
            syntaxerror("unknown variable");
        $$ = abc_getlocal(0, i);
}
VARIABLE : T_IDENTIFIER
VARIABLE : VARIABLE '.' T_IDENTIFIER
VARIABLE : VARIABLE ".." T_IDENTIFIER // descendants
VARIABLE : VARIABLE "::" VARIABLE // namespace declaration
VARIABLE : VARIABLE "::" '[' EXPRESSION ']' // qualified expression
VARIABLE : VARIABLE '[' EXPRESSION ']' // unqualified expression

ASSIGNMENT :           VARIABLE           '=' EXPRESSION
NEW_ASSIGNMENT : "var" VARIABLE MAYBETYPE '=' EXPRESSION

FOR : "for" '(' NEW_ASSIGNMENT ';' EXPRESSION ';' EXPRESSION ')' '{' MAYBECODE '}'
FOR : "for" '(' ASSIGNMENT     ';' EXPRESSION ';' EXPRESSION ')' '{' MAYBECODE '}'

USE : "use" KW_NAMESPACE T_IDENTIFIER

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

