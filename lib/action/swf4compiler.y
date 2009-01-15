/* $Id: swf4compiler.y,v 1.1 2004/05/08 17:17:35 kramm Exp $ */

%start program

%{

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "compile.h"
#include "action.h"

#define YYPARSE_PARAM buffer

%}

%union {
  Buffer action;
  char *str;
  SWFActionFunction function;
  SWFGetUrl2Method getURLMethod;
}

/* tokens etc. */
%token BREAK
%token FOR
%token CONTINUE
%token IF
%token ELSE
%token DO
%token WHILE

%token THIS

/* functions */
%token EVAL
%token TIME
%token RANDOM
%token LENGTH
%token INT
%token CONCAT
%token DUPLICATECLIP
%token REMOVECLIP
%token TRACE
%token STARTDRAG
%token STOPDRAG
%token ORD
%token CHR
%token CALLFRAME
%token GETURL
%token GETURL1
%token LOADMOVIE
%token LOADVARIABLES
%token POSTURL
%token SUBSTR

%token GETPROPERTY

/* v3 functions */
%token NEXTFRAME
%token PREVFRAME
%token PLAY
%token STOP
%token TOGGLEQUALITY
%token STOPSOUNDS
%token GOTOFRAME
%token GOTOANDPLAY
%token FRAMELOADED
%token SETTARGET

/* high level functions */
%token TELLTARGET

/* these three are strdup'ed in compiler.flex, so free them here */
%token <str> STRING
%token <str> NUMBER
%token <str> IDENTIFIER
%token <str> PATH

%token <getURLMethod> GETURL_METHOD

%token EQ "=="
%token LE "<="
%token GE ">="
%token NE "!="
%token LAN "&&"
%token LOR "||"
%token INC "++"
%token DEC "--"
%token IEQ "+="
%token DEQ "/="
%token MEQ "*="
%token SEQ "-="
%token STREQ "==="
%token STRNE "!=="
%token STRCMP "<=>"
%token PARENT ".."

%token END "end"

/* ascending order of ops ..? */
%left ','
%right '=' "*=" "/=" "+=" "-="
%right '?' ':'
%left "&&" "||"
%left "==" "!=" "===" "!=="
%left '<' '>' "<=" ">=" "<=>"
%left '&'
%left '+' '-'
%left '*' '/'
%right "++" "--" UMINUS '!'
%right POSTFIX
%right NEGATE

%type <action> elem
%type <action> elems
%type <action> stmt
%type <action> statements
%type <action> if_stmt
%type <action> iter_stmt
%type <action> cont_stmt
%type <action> break_stmt
%type <action> expr_opt
%type <action> void_function_call
%type <action> function_call
%type <action> lhs_expr
%type <action> pf_expr
%type <action> rhs_expr
%type <action> assign_stmt
%type <action> assign_stmts
%type <action> assign_stmts_opt
%type <action> expr
%type <action> program

/* make sure to free these, too! */
%type <str>    sprite
%type <str>    variable

%%

/* rules */

program
        : elems
		{ *((Buffer *)buffer) = $1; }

	;

elems
	: elem
	| elems elem
                { bufferConcat($1, $2); }
	;

elem
	: stmt
	;

stmt
	: '{' '}'				{ $$ = NULL; }
	| '{' statements '}'			{ $$ = $2; }
	| ';'					{ $$ = NULL; }
	| assign_stmt ';'
	| if_stmt
	| iter_stmt
	| cont_stmt
	| break_stmt
	;

assign_stmts
	: assign_stmt
	| assign_stmts ',' assign_stmt		{ bufferConcat($1, $3); }
	;

statements
	: /* empty */	{ $$ = NULL; }
	| stmt
	| statements stmt
			{ bufferConcat($1, $2); }
	;

if_stmt
        /* XXX- I haven't tested the frameloaded() stuff yet.. */

	: IF '(' FRAMELOADED '(' NUMBER ')' ')' stmt ELSE stmt
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_WAITFORFRAME);
		  bufferWriteS16($$, 3);
		  bufferWriteS16($$, atoi($5));
		  free($5);
		  bufferWriteU8($$, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($10)+5);
		  bufferConcat($$, $10);			  /* ..here */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($8));
		  bufferConcat($$, $8); }

	| IF '(' FRAMELOADED '(' NUMBER ')' ')' stmt
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_WAITFORFRAME);
		  bufferWriteS16($$, 3);
		  bufferWriteS16($$, atoi($5));
		  free($5);
		  bufferWriteU8($$, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, 5);
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);	  /* ..here */
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($8));	  /* ..and then out */
		  bufferConcat($$, $8); }

	/* make this case cleaner.. */
	| IF '(' '!' FRAMELOADED '(' NUMBER ')' ')' stmt
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_WAITFORFRAME);
		  bufferWriteS16($$, 3);
		  bufferWriteS16($$, atoi($6));
		  free($6);
		  bufferWriteU8($$, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($9));
		  bufferConcat($$, $9); }			  /* ..here */

	| IF '(' FRAMELOADED '(' expr ')' ')' stmt ELSE stmt
		{ $$ = $5;
		  bufferWriteU8($$, SWFACTION_WAITFORFRAMEEXPRESSION);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($10)+5);
		  bufferConcat($$, $10);			  /* ..here */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($8));
		  bufferConcat($$, $8); }

	| IF '(' FRAMELOADED '(' expr ')' ')' stmt
		{ $$ = $5;
		  bufferWriteU8($$, SWFACTION_WAITFORFRAMEEXPRESSION);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, 5);
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);	  /* ..here */
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($8));	  /* ..and then out */
		  bufferConcat($$, $8); }

	/* make this case cleaner.. */
	| IF '(' '!' FRAMELOADED '(' expr ')' ')' stmt
		{ $$ = $6;
		  bufferWriteU8($$, SWFACTION_WAITFORFRAMEEXPRESSION);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($9));
		  bufferConcat($$, $9); }			  /* ..here */

	| IF '(' expr ')' stmt ELSE stmt
		{ bufferWriteU8($3, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16($3, 2);
		  bufferWriteS16($3, bufferLength($7)+5);
		  bufferConcat($3, $7);
		  bufferWriteU8($3, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($3, 2);
		  bufferWriteS16($3, bufferLength($5));
		  bufferConcat($3, $5);
		  $$ = $3; }

	| IF '(' expr ')' stmt
		{ bufferWriteU8($3, SWFACTION_LOGICALNOT);
		  bufferWriteU8($3, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16($3, 2);
		  bufferWriteS16($3, bufferLength($5));
		  bufferConcat($3, $5);
		  $$ = $3; }
	;

expr_opt
	: /* empty */	{ $$ = NULL; }
	| expr		{ $$ = $1; }
	;

/* not thought out yet..
switch_stmt
	: SWITCH '(' expr ')' '{'
		{ $$ = $3;
		  pushLoop(); }
	  switch_cases '}'
		{ bufferConcat($$, $7); }
	;

switch_cases
	: switch_cases switch_case
	| switch_case
	;

switch_case
	: CASE INTEGER ':' stmt
		{ $$ = newBuffer(); }
	;
*/

iter_stmt
	: WHILE '(' '!' FRAMELOADED '(' NUMBER ')' ')' stmt
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_WAITFORFRAME);
		  bufferWriteS16($$, 3);
		  bufferWriteS16($$, atoi($6));
		  free($6);
		  bufferWriteU8($$, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($9)+5);
		  bufferConcat($$, $9);				  /* ..here */
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, -(bufferLength($$)+2)); }

	| WHILE '(' expr ')' stmt
                { $$ = $3;
		  bufferWriteU8($$, SWFACTION_LOGICALNOT);
		  bufferWriteU8($$, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, bufferLength($5)+5);
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, -(bufferLength($$)+2));
		  bufferResolveJumps($$); }

	| DO stmt WHILE '(' expr ')'
		{ $$ = $2;
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, -(bufferLength($$)+2));
		  bufferResolveJumps($$); }

	| FOR '(' assign_stmts_opt ';' expr_opt ';' assign_stmts_opt ')' stmt
                { if (!$5)
                    $5 = newBuffer();
                  else {
                    bufferWriteU8($5, SWFACTION_LOGICALNOT);
                    bufferWriteU8($5, SWFACTION_BRANCHIFTRUE);
                    bufferWriteS16($5, 2);
                    bufferWriteS16($5, bufferLength($9)+bufferLength($7)+5);
                  }
                  bufferConcat($5, $9);
                  bufferConcat($5, $7);
                  bufferWriteU8($5, SWFACTION_BRANCHALWAYS);
                  bufferWriteS16($5, 2);
                  bufferWriteS16($5, -(bufferLength($5)+2));
                  bufferResolveJumps($5);
                  $$ = $3;
                  if(!$$) $$ = newBuffer();
                  bufferConcat($$, $5);
                }
	;

assign_stmts_opt
	: /* empty */				{ $$ = NULL; }
	| assign_stmts
	;

cont_stmt
	: CONTINUE ';'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, MAGIC_CONTINUE_NUMBER); }
	;

break_stmt
	: BREAK ';'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, MAGIC_BREAK_NUMBER); }
	;

void_function_call
	: STOPDRAG '(' ')' /* no args */
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_STOPDRAGMOVIE); }

	| CALLFRAME '(' variable ')'
		{ $$ = newBuffer();
		  bufferWriteString($$, $3, strlen($3)+1);
		  bufferWriteU8($$, SWFACTION_CALLFRAME);
		  bufferWriteS16($$, 0);
		  free($3); }

	| CALLFRAME '(' STRING ')'
		{ $$ = newBuffer();
		  bufferWriteString($$, $3, strlen($3)+1);
		  bufferWriteU8($$, SWFACTION_CALLFRAME);
		  bufferWriteS16($$, 0);
		  free($3); }

	| REMOVECLIP '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_REMOVECLIP); }

	| TRACE '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_TRACE); }

	/* getURL2(url, window, [method]) */
	| GETURL '(' expr ',' expr ')'
		{ $$ = $3;
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_GETURL2);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, GETURL_METHOD_NOSEND); }

	| GETURL '(' expr ',' expr ',' GETURL_METHOD ')'
		{ $$ = $3;
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_GETURL2);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, $7); }

	| GETURL1 '(' STRING ',' STRING ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_GETURL);
		  bufferWriteS16($$, strlen($3) + strlen($5) + 2);
		  bufferWriteHardString($$, (byte*)$3, strlen($3));
		  bufferWriteU8($$, 0);
		  bufferWriteHardString($$, (byte*)$5, strlen($5));
		  bufferWriteU8($$, 0); }

	| LOADMOVIE '(' expr ',' expr ')'
		{ $$ = $3;
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_GETURL2);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, GETURL_METHOD_NOSEND | GETURL_LOADMOVIE); }

	| LOADMOVIE '(' expr ',' expr ',' GETURL_METHOD ')'
		{ $$ = $3;
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_GETURL2);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, $7 | GETURL_LOADMOVIE); }

	| LOADVARIABLES '(' expr ',' expr ')'
		{ $$ = $3;
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_GETURL2);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, GETURL_METHOD_NOSEND | GETURL_LOADVARIABLES); }

	| LOADVARIABLES '(' expr ',' expr ',' GETURL_METHOD ')'
		{ $$ = $3;
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_GETURL2);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, $7 | GETURL_LOADVARIABLES); }

	/* startDrag(target, lock, [left, right, top, bottom]) */
	| STARTDRAG '(' expr ',' expr ')'
		{ $$ = newBuffer();
		  bufferWriteString($$, "0", 2); /* no constraint */
		  bufferConcat($$, $5);
		  bufferConcat($$, $3);
		  bufferWriteU8($$, SWFACTION_STARTDRAGMOVIE); }

	| STARTDRAG '(' expr ',' expr ',' expr ',' expr ',' expr ',' expr ')'
		{ $$ = newBuffer();
		  bufferConcat($$, $7);
		  bufferConcat($$, $11);
		  bufferConcat($$, $9);
		  bufferConcat($$, $13);
		  bufferWriteString($$, "1", 2); /* has constraint */
		  bufferConcat($$, $5);
		  bufferConcat($$, $3);
		  bufferWriteU8($$, SWFACTION_STARTDRAGMOVIE); }

	/* duplicateClip(target, new, depth) */
	| DUPLICATECLIP '(' expr ',' expr ',' expr ')'
		{ $$ = $3;
		  bufferConcat($$, $5);
		  bufferConcat($$, $7);
		  bufferWriteWTHITProperty($$);
		  bufferWriteU8($$, SWFACTION_ADD); /* see docs for explanation */
		  bufferWriteU8($$, SWFACTION_DUPLICATECLIP); }

	/* v3 actions */
	| NEXTFRAME '(' ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_NEXTFRAME); }
		
	| PREVFRAME '(' ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_PREVFRAME); }

	| PLAY '(' ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_PLAY); }

	| STOP '(' ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_STOP); }

	| TOGGLEQUALITY '(' ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_TOGGLEQUALITY); }

	| STOPSOUNDS '(' ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_STOPSOUNDS); }

	| GOTOFRAME '(' NUMBER ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_GOTOFRAME);
		  bufferWriteS16($$, 2);
		  bufferWriteS16($$, atoi($3));
		  free($3); }

	| GOTOFRAME '(' STRING ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_GOTOLABEL);
		  bufferWriteS16($$, strlen($3)+1);
		  bufferWriteHardString($$, (byte*)$3, strlen($3)+1);
		  free($3); }

	| GOTOFRAME '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_GOTOEXPRESSION);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, 0); } /* XXX - and stop */

	| GOTOANDPLAY '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_GOTOEXPRESSION);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, 1); } /* XXX - and play */

	| SETTARGET '(' STRING ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_SETTARGET);
		  bufferWriteS16($$, strlen($3)+1);
		  bufferWriteHardString($$, (byte*)$3, strlen($3)+1);
		  free($3); }

	| SETTARGET '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_SETTARGETEXPRESSION); }

	| TELLTARGET '(' STRING ')' stmt
		{ $$ = newBuffer();
			/* SetTarget(STRING) */
		  bufferWriteU8($$, SWFACTION_SETTARGET);
		  bufferWriteS16($$, strlen($3)+1);
		  bufferWriteHardString($$, (byte*)$3, strlen($3)+1);
			/* stmt */
		  bufferConcat($$, $5);
			/* SetTarget('') */
		  bufferWriteU8($$, SWFACTION_SETTARGET);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, 0);
		  free($3); }

	| TELLTARGET '(' expr ')' stmt
		{ $$ = $3;
			/* SetTarget(expr) */
		  bufferWriteU8($$, SWFACTION_SETTARGETEXPRESSION); 
			/* stmt */
		  bufferConcat($$, $5);
			/* SetTarget('') */
		  bufferWriteU8($$, SWFACTION_SETTARGET);
		  bufferWriteS16($$, 1);
		  bufferWriteU8($$, 0); }
	;

function_call
	: EVAL '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_GETVARIABLE); }

	| TIME '(' ')'
		{ $$ = newBuffer();
		  bufferWriteU8($$, SWFACTION_GETTIMER); }

	| RANDOM '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_RANDOM); }

	| LENGTH '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_STRINGLENGTH); }

	| INT '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_INT); }

	| ORD '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_ORD); }

	| CHR '(' expr ')'
		{ $$ = $3;
		  bufferWriteU8($$, SWFACTION_CHR); }

	| CONCAT '(' expr ',' expr ')'
		{ $$ = $3;
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_STRINGCONCAT); }

	| SUBSTR '(' expr ',' expr ',' expr ')'
		{ $$ = $3;
		  bufferConcat($$, $5);
		  bufferConcat($$, $7);
		  bufferWriteU8($$, SWFACTION_SUBSTRING); }

	| GETPROPERTY '(' expr ',' STRING ')'
		{ $$ = newBuffer();
		  bufferConcat($$, $3);
		  bufferWriteGetProperty($$, $5);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  free($5); }
	;

pf_expr
	: lhs_expr "++" %prec POSTFIX
		{ $$ = newBuffer();
		  bufferWriteBuffer($$, $1);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE);
		  bufferWriteBuffer($$, $1);
		  bufferConcat($$, $1);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_ADD);
		  bufferWriteU8($$, SWFACTION_SETVARIABLE); }

	| lhs_expr "--" %prec POSTFIX
		{ $$ = newBuffer();
		  bufferWriteBuffer($$, $1);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE);
		  bufferWriteBuffer($$, $1);
		  bufferConcat($$, $1);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_SUBTRACT);
		  bufferWriteU8($$, SWFACTION_SETVARIABLE); }
	;

/* these leave a value on the stack */
rhs_expr
	: function_call

	| '(' rhs_expr ')'
		{ $$ = $2; }

	| NUMBER
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  free($1); }

	| '-' NUMBER %prec NEGATE
		{ $$ = newBuffer();
		  bufferWriteString($$, "-", 2);
		  bufferWriteString($$, $2, strlen($2)+1);
		  free($2); }

	| STRING
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  free($1); }

	| variable
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE);
		  free($1); }

	| sprite
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  free($1); }

	| sprite '.' IDENTIFIER
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteGetProperty($$, $3);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  free($3);
		  free($1); }

	| "++" sprite '.' IDENTIFIER
		{ $$ = newBuffer();
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteGetProperty($$, $4);
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteSetProperty($$, $4);
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteGetProperty($$, $4);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_ADD);
		  bufferWriteU8($$, SWFACTION_SETPROPERTY);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  free($2);
		  free($4); }

	| "++" lhs_expr
		{ $$ = $2;
		  bufferWriteU8($$, SWFACTION_DUP);
		  bufferWriteU8($$, SWFACTION_DUP);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_ADD);
		  bufferWriteU8($$, SWFACTION_SETVARIABLE);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE); }

	| "--" sprite '.' IDENTIFIER
		{ $$ = newBuffer();
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteGetProperty($$, $4);
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteSetProperty($$, $4);
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteGetProperty($$, $4);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_ADD);
		  bufferWriteU8($$, SWFACTION_SETPROPERTY);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  free($2);
		  free($4); }

	| "--" lhs_expr
		{ $$ = $2;
		  bufferWriteU8($$, SWFACTION_DUP);
		  bufferWriteU8($$, SWFACTION_DUP);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_SUBTRACT);
		  bufferWriteU8($$, SWFACTION_SETVARIABLE);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE); }

	| '-' rhs_expr %prec UMINUS
		{ $$ = $2;
		  bufferWriteString($2, "-1", 3);
		  bufferWriteU8($2, SWFACTION_MULTIPLY); }

	| '!' rhs_expr
		{ $$ = $2;
		  bufferWriteU8($2, SWFACTION_LOGICALNOT); }

	| lhs_expr '=' rhs_expr /* assign and leave copy on stack */
		{ $$ = $1;
		  bufferConcat($$, $3);
		  bufferWriteU8($$, SWFACTION_DUP);
		  bufferWriteU8($$, SWFACTION_SETVARIABLE); }

	| rhs_expr '*' rhs_expr
		{ $$ = $1;
		  bufferConcat($$, $3);
		  bufferWriteU8($$, SWFACTION_MULTIPLY); }

	| rhs_expr '/' rhs_expr
		{ $$ = $1;
		  bufferConcat($$, $3);
		  bufferWriteU8($$, SWFACTION_DIVIDE); }

	| rhs_expr '+' rhs_expr
		{ $$ = $1;
		  bufferConcat($$, $3);
		  bufferWriteU8($$, SWFACTION_ADD); }

	| rhs_expr '-' rhs_expr
		{ $$ = $1;
		  bufferConcat($$, $3);
		  bufferWriteU8($$, SWFACTION_SUBTRACT); }

	| rhs_expr '&' rhs_expr
		{ $$ = $1;
		  bufferConcat($$, $3);
		  bufferWriteU8($$, SWFACTION_STRINGCONCAT); }

	| rhs_expr '<' rhs_expr
		{ $$ = $1;
		  bufferConcat($$, $3);
		  bufferWriteU8($$, SWFACTION_LESSTHAN); }

	| rhs_expr '>' rhs_expr
		{ $$ = $3;
		  bufferConcat($$, $1);
		  bufferWriteU8($$, SWFACTION_LESSTHAN); }

	| rhs_expr "<=" rhs_expr
		{ $$ = $3;
		  bufferConcat($$, $1);
		  bufferWriteU8($$, SWFACTION_LESSTHAN);
		  bufferWriteU8($$, SWFACTION_LOGICALNOT); }

	| rhs_expr ">=" rhs_expr
		{ bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_LESSTHAN);
		  bufferWriteU8($1, SWFACTION_LOGICALNOT); }

	| rhs_expr "!==" rhs_expr
		{ bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_STRINGEQ);
		  bufferWriteU8($1, SWFACTION_LOGICALNOT); }

	| rhs_expr "===" rhs_expr
		{ bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_STRINGEQ); }

	| rhs_expr "<=>" rhs_expr
		{ bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_STRINGCOMPARE); }

	| rhs_expr "==" rhs_expr
		{ bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_EQUAL); }

	| rhs_expr "!=" rhs_expr
		{ bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_EQUAL);
		  bufferWriteU8($1, SWFACTION_LOGICALNOT); }

	| rhs_expr "&&" rhs_expr
		{ bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_LOGICALAND); }

	| rhs_expr "||" rhs_expr
		{ bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_LOGICALOR); }

	| rhs_expr '?' rhs_expr ':' rhs_expr
		{ bufferWriteU8($1, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16($1, 2);
		  bufferWriteS16($1, bufferLength($5)+5);
		  bufferConcat($1, $5);
		  bufferWriteU8($1, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16($1, 2);
		  bufferWriteS16($1, bufferLength($3));
		  bufferConcat($1, $3); }
	;

variable
	: IDENTIFIER

	| sprite ':' IDENTIFIER
		{ $$ = $1;
		  $$ = stringConcat($$, strdup(":"));
		  $$ = stringConcat($$, $3); }
	;

sprite
	: THIS
		{ $$ = strdup(""); }

	| '.'
		{ $$ = strdup(""); }

	| '/'
		{ $$ = strdup("/"); }

	| PARENT
		{ $$ = strdup(".."); }

	| IDENTIFIER
		{ $$ = $1; }

	| PATH
		{ $$ = $1; }
	;

lhs_expr
	: variable
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  free($1); }

	| STRING
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  free($1); }

	| '(' rhs_expr ')'	{ $$ = $2; }
	;

assign_stmt
	: pf_expr

	| void_function_call

	| "++" lhs_expr
		{ $$ = $2;
		  bufferWriteBuffer($$, $2);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_ADD);
		  bufferWriteU8($$, SWFACTION_SETVARIABLE); }

	| "--" lhs_expr
                { $$ = $2;
		  bufferWriteBuffer($$, $2);
		  bufferWriteU8($$, SWFACTION_GETVARIABLE);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_SUBTRACT);
		  bufferWriteU8($$, SWFACTION_SETVARIABLE); }

	| "++" sprite '.' IDENTIFIER
		{ $$ = newBuffer();
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteGetProperty($$, $4);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_ADD);
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteSetProperty($$, $4);
		  bufferWriteU8($$, SWFACTION_SETPROPERTY);
		  free($2);
		  free($4); }

	| "--" sprite '.' IDENTIFIER
		{ $$ = newBuffer();
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteGetProperty($$, $4);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  bufferWriteString($$, "1", 2);
		  bufferWriteU8($$, SWFACTION_SUBTRACT);
		  bufferWriteString($$, $2, strlen($2)+1);
		  bufferWriteSetProperty($$, $4);
		  bufferWriteU8($$, SWFACTION_SETPROPERTY);
		  free($2);
		  free($4); }

	| lhs_expr '=' rhs_expr
		{ bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_SETVARIABLE); }

	| lhs_expr "*=" rhs_expr
		{ bufferWriteBuffer($1, $1);
		  bufferWriteU8($1, SWFACTION_GETVARIABLE);
		  bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_MULTIPLY);
		  bufferWriteU8($1, SWFACTION_SETVARIABLE); }

	| lhs_expr "/=" rhs_expr
		{ bufferWriteBuffer($1, $1);
		  bufferWriteU8($1, SWFACTION_GETVARIABLE);
		  bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_DIVIDE);
		  bufferWriteU8($1, SWFACTION_SETVARIABLE); }

	| lhs_expr "+=" rhs_expr
		{ bufferWriteBuffer($1, $1);
		  bufferWriteU8($1, SWFACTION_GETVARIABLE);
		  bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_ADD);
		  bufferWriteU8($1, SWFACTION_SETVARIABLE); }

	| lhs_expr "-=" rhs_expr
		{ bufferWriteBuffer($1, $1);
		  bufferWriteU8($1, SWFACTION_GETVARIABLE);
		  bufferConcat($1, $3);
		  bufferWriteU8($1, SWFACTION_SUBTRACT);
		  bufferWriteU8($1, SWFACTION_SETVARIABLE); }

	| sprite '.' IDENTIFIER '=' rhs_expr
                { $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteSetProperty($$, $3);
		  bufferConcat($$,$5);
		  bufferWriteU8($$, SWFACTION_SETPROPERTY);
		  free($1);
		  free($3); }

	| sprite '.' IDENTIFIER "*=" rhs_expr
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteSetProperty($$, $3);
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteGetProperty($$, $3);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_MULTIPLY);
		  bufferWriteU8($$, SWFACTION_SETPROPERTY);
		  free($1);
		  free($3); }

	| sprite '.' IDENTIFIER "/=" rhs_expr
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteSetProperty($$, $3);
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteGetProperty($$, $3);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_DIVIDE);
		  bufferWriteU8($$, SWFACTION_SETPROPERTY);
		  free($1);
		  free($3); }

	| sprite '.' IDENTIFIER "+=" rhs_expr
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteSetProperty($$, $3);
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteGetProperty($$, $3);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_ADD);
		  bufferWriteU8($$, SWFACTION_SETPROPERTY);
		  free($1);
		  free($3); }

	| sprite '.' IDENTIFIER "-=" rhs_expr
		{ $$ = newBuffer();
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteSetProperty($$, $3);
		  bufferWriteString($$, $1, strlen($1)+1);
		  bufferWriteGetProperty($$, $3);
		  bufferWriteU8($$, SWFACTION_GETPROPERTY);
		  bufferConcat($$, $5);
		  bufferWriteU8($$, SWFACTION_SUBTRACT);
		  bufferWriteU8($$, SWFACTION_SETPROPERTY);
		  free($1);
		  free($3); }
	;

expr
	: rhs_expr
	;

%%
