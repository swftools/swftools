%{

#include <math.h>
#include <string.h>

#include "compile.h"
#include "action.h"
#include "blocks/error.h"
#include "swf5compiler.tab.h" /* defines token types */

static int swf5debug;

static const char *lexBuffer = NULL;
static int lexBufferLen = 0;

static int  sLineNumber = 0;
static char szLine[1024];
static char msgbufs[2][1024] = { {0}, {0} }, *msgline = {0};
static int  column = 0;

static void comment();
static void comment1();
static void count();
static void countline();
static void warning(char *msg);

#define YY_INPUT(buf,result,max_size) result=lexBufferInput(buf, max_size)

/* thanks to the prolific and brilliant Raff: */
static int lexBufferInput(char *buf, int max_size)
{
  int l = lexBufferLen > max_size ? max_size : lexBufferLen;

  if (lexBufferLen <= 0)
    return YY_NULL;

  memcpy(buf, lexBuffer, l);
  lexBuffer += l;
  lexBufferLen -= l;
  return l;
}

        /* very inefficient method of unescaping strings */
static void unescape(char *buf)
{
  char *p, *p1;

  for (p1=buf; (p=strchr(p1, '\\')) != 0; p1 = p+1) {
    switch(p[1])
    {
    case 'b' : p[1] = '\b'; break;
    case 'f' : p[1] = '\f'; break;
    case 'n' : p[1] = '\n'; break;
    case 'r' : p[1] = '\r'; break;
    case 't' : p[1] = '\t'; break;
    case 'x' :
    case 'u' : warning("unsupported escape sequence");
    }
    strcpy(p, p+1);
  }
}

void swf5ParseInit(const char *script, int debug)
{
  checkByteOrder();
  yyrestart(NULL);

  swf5debug = debug;

  lexBuffer = script;
  lexBufferLen = strlen(script);
  sLineNumber = 0;
  column = 0;
  msgline = msgbufs[0];
}

%}

%s asm

%{
 // forward declaration needed by the following function
#ifndef YY_PROTO
#ifdef YY_USE_PROTOS
#define YY_PROTO(proto) proto
#else
#define YY_PROTO(proto) ()
#endif
#endif
 static void yyunput YY_PROTO(( int c, char *buf_ptr ));

 void do_unput5(const char c) { unput(c); }
%}

DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*

%%

0x[0-9a-fA-F]+	{ count();	swf5lval.intVal = strtoul(yytext, NULL, 0);
					return INTEGER;		}
0[0-7]+			{ count();	swf5lval.intVal = strtoul(yytext, NULL, 0);
					return INTEGER;		}
{DIGIT}+		{ count();	swf5lval.intVal = atoi(yytext);
					return INTEGER;		}
{DIGIT}+"."{DIGIT}*	{ count();	swf5lval.doubleVal = atof(yytext);
					return DOUBLE; 		}
true			{ count();	swf5lval.intVal = 1;
					return BOOLEAN;		}
false			{ count();	swf5lval.intVal = 0;
					return BOOLEAN;		}
null			{ count();	return NULLVAL;		}
break			{ count();	return BREAK;		}
continue		{ count();	return CONTINUE;	}
function		{ count();	return FUNCTION;	}
else			{ count();	return ELSE;		}
switch			{ count();	return SWITCH;		}
case			{ count();	return CASE;		}
default			{ count();	return DEFAULT;		}
for			{ count();	return FOR;		}
in			{ count();	return IN;		}
if			{ count();	return IF;		}
while			{ count();	return WHILE;		}
do			{ count();	return DO;		}
var			{ count();	return VAR;		}
new			{ count();	return NEW;		}
delete			{ count();	return DELETE;		}
return			{ count();	return RETURN;		}
with			{ count();	return WITH;		}
asm			{ count();	BEGIN(asm); return ASM;		}
eval			{ count();	return EVAL;		}
typeof			{ count();	return TYPEOF; }
instanceof			{ count();	return INSTANCEOF; }

  /* legacy functions */
random			{ count();	return RANDOM;	}
getTimer		{ count();	return GETTIMER;	}
length			{ count();	return LENGTH;	}
concat			{ count();	return CONCAT;	}
substr			{ count();	return SUBSTR;	}
trace			{ count();	return TRACE;	}
int			{ count();	return INT;	}
ord			{ count();	return ORD;	}
chr			{ count();	return CHR;	}
getURL			{ count();	return GETURL;	}
getURL1			{ count();	return GETURL1;	}
nextFrame		{ count();	return NEXTFRAME;	}
prevFrame		{ count();	return PREVFRAME;	}
play			{ count();	return PLAY;		}
stop			{ count();	return STOP;		}
toggleQuality		{ count();	return TOGGLEQUALITY;	}
stopSounds		{ count();	return STOPSOUNDS;	}
callFrame		{ count();	return CALLFRAME;	}
gotoFrame		{ count();	return GOTOFRAME;	}
setTarget		{ count();	return SETTARGET;	}
loadVariables		{ count();	return LOADVARIABLES;	}
loadMovie		{ count();	return LOADMOVIE;	}
loadVariablesNum	{ count();	return LOADVARIABLESNUM;	}
loadMovieNum		{ count();	return LOADMOVIENUM;	}
duplicateMovieClip	{ count();	return DUPLICATEMOVIECLIP; }
removeMovieClip		{ count();	return REMOVEMOVIECLIP; }

  /* assembler ops */
<asm>{
dup			{ count();	return DUP; }
swap			{ count();	return SWAP; }
pop			{ count();	return POP; }
push			{ count();	return PUSH; }
setregister		{ count();	return SETREGISTER; }
callfunction		{ count();	return CALLFUNCTION; }
callmethod		{ count();	return CALLMETHOD; }
and			{ count();	return AND; }
or			{ count();	return OR; }
xor			{ count();	return XOR; }
modulo			{ count();	return MODULO; }
add			{ count();	return ADD; }
newadd			{ count();	return ADD; }
lessthan		{ count();	return LESSTHAN; }
newlessthan		{ count();	return LESSTHAN; }
equals			{ count();	return EQUALS; }
newequals		{ count();	return EQUALS; }
inc			{ count();	return INC; }
dec			{ count();	return DEC; }
enumerate		{ count();	return ENUMERATE; }
initobject		{ count();	return INITOBJECT; }
initarray		{ count();	return INITARRAY; }
getmember		{ count();	return GETMEMBER; }
setmember		{ count();	return SETMEMBER; }
shiftleft		{ count();	return SHIFTLEFT; }
shiftright		{ count();	return SHIFTRIGHT; }
shiftright2		{ count();	return SHIFTRIGHT2; }
varequals		{ count();	return VAREQUALS; }
oldadd			{ count();	return OLDADD; }
subtract		{ count();	return SUBTRACT; }
multiply		{ count();	return MULTIPLY; }
divide			{ count();	return DIVIDE; }
oldequals		{ count();	return OLDEQUALS; }
oldlessthan		{ count();	return OLDLESSTHAN; }
logicaland		{ count();	return LOGICALAND; }
logicalor		{ count();	return LOGICALOR; }
not			{ count();	return NOT; }
stringeq		{ count();	return STRINGEQ; }
stringlength		{ count();	return STRINGLENGTH; }
substring		{ count();	return SUBSTRING; }
getvariable		{ count();	return GETVARIABLE; }
setvariable		{ count();	return SETVARIABLE; }
settargetexpr		{ count();	return SETTARGETEXPRESSION; }
startdrag		{ count();	return STARTDRAG; }
stopdrag		{ count();	return STOPDRAG; }
stringlessthan		{ count();	return STRINGLESSTHAN; }
mblength		{ count();	return MBLENGTH; }
mbsubstring		{ count();	return MBSUBSTRING; }
mbord			{ count();	return MBORD; }
mbchr			{ count();	return MBCHR; }
branch			{ count();	return BRANCHALWAYS; }
branchalways		{ count();	return BRANCHALWAYS; }
branchiftrue		{ count();	return BRANCHIFTRUE; }
post			{ count();	return POST; }
get			{ count();	return GET; }
end			{ count();	return END;		}
}

r\:{DIGIT}+		{ count();	swf5lval.str = strdup(yytext+2);
					return REGISTER;	}


{ID}			{ count();	swf5lval.str = strdup(yytext);
					return IDENTIFIER;	}

\"(\\.|[^\\"])*\"	{ count();	swf5lval.str = strdup(yytext+1);
					swf5lval.str[strlen(swf5lval.str)-1]=0;
                                        unescape(swf5lval.str);
					return STRING;		}

\'(\\.|[^\\'])*\'	{ count();	swf5lval.str = strdup(yytext+1);
					swf5lval.str[strlen(swf5lval.str)-1]=0;
                                        unescape(swf5lval.str);
					return STRING; 		}

\"(\\.|[^\\"])*$	{ count();	swf5lval.str = strdup("");
					warning("Unterminated string!");
					return STRING;		}

\'(\\.|[^\\'])*$	{ count();	swf5lval.str = strdup("");
					warning("Unterminated string!");
					return STRING;		}

"/*"			{ count();	comment();		}
"//"			{ count();	comment1();		}
[ \t\v\f]		{ count(); }

"++"			{ count();	return INCR; }
"--"			{ count();	return DECR; }
"<="			{ count();	return LE; }
">="			{ count();	return GE; }
"==" 			{ count();	return EQ; }
"!=" 			{ count();	return NE; }
"&&" 			{ count();	return LAN; }
"||" 			{ count();	return LOR; }
"*="			{ count();	return MEQ; }
"/="			{ count();	return DEQ; }
"+="			{ count();	return IEQ; }
"-="			{ count();	return SEQ; }
"&="			{ count();	return AEQ; }
"|="			{ count();	return OEQ; }
"<<"			{ count();	return SHL; }
">>"			{ count();	return SHR; }
">>>"			{ count();	return SHR2; }
"<<="			{ count();	return SHLEQ; }
">>="			{ count();	return SHREQ; }
">>>="			{ count();	return SHR2EQ; }

"<"			{ count();	return '<'; }
">"			{ count();	return '>'; }
";"			{ count();	return ';'; }
"="			{ count();	return '='; }
"+"			{ count();	return '+'; }
"-"			{ count();	return '-'; }
"&"			{ count();	return '&'; }
"|"			{ count();	return '|'; }
"^"			{ count();	return '^'; }
"*"			{ count();	return '*'; }
"/"			{ count();	return '/'; }
"%"			{ count();	return '%'; }
"!"			{ count();	return '!'; }
"("			{ count();	return '('; }
")"			{ count();	return ')'; }
"["			{ count();	return '['; }
"]"			{ count();	return ']'; }
"{"			{ count();	return '{'; }
"}"			{ count();	BEGIN(0); return '}'; }
","			{ count();	return ','; }
"."			{ count();	return '.'; }
"?"			{ count();	return '?'; }
":"			{ count();	return ':'; }
"~"			{ count();	return '~'; }

\r?\n			{ count();	strcpy(szLine, yytext + 1);
					countline();	yyless(1);	}

.			SWF_error("Unrecognized character: %s\n", yytext);

%%
static int getinput() {
#ifdef __cplusplus
					return yyinput();
#else
					return input();
#endif
}

int swf5wrap()
{
  return 1;
}

static void countline()
{
  if(sLineNumber != 0)
    msgline[column] = 0;

  ++sLineNumber;
  column = 0;
  msgline = msgbufs[sLineNumber & 1];
}

static int LineNumber(void)
{
   return (sLineNumber + 1);
}

static int ColumnNumber(void)
{
   return column;
}

static char *LineText(void)
{
  msgline[column] = 0;
  return msgline;
}

static void comment()
{
   // Handle block comments

   int c, c1;

loop:
   // We have the start of a comment so look skip everything up to the
   // end of the comment character
   while ((c = getinput()) != '*' && c != EOF)
   {
      if(column < 1023)
         msgline[column] = c;

      ++column;

      // keep the line number in synch
      if (c == '\n')
      {
         // start the output (matches the algorithim in the lexx above)
	 countline();
      }

      if (swf5debug) putchar(c);
   }

   // is this the end of comment character
   if ((c1 = getinput()) != '/' && c != EOF)
   {
      // false start as this was no end of comment
      do_unput5(c1);
      goto loop;
   }

   // write out the start of the end of comment
   if (c != EOF)
      if (swf5debug) putchar(c);

   // write out the end of the end of comment
   if (c1 != EOF) 
      if (swf5debug) putchar(c1);
}

static void comment1()
{
   // Handle comment of type 1 (ie '//')

   int c;

   // this is a line comment
   while ((c = getinput()) != '\n' && c != EOF)
   {
      if (swf5debug) putchar(c);

      if(column < 1023)
         msgline[column] = c;

      ++column;
   };

   // keep the line number in synch
   if (c == '\n')
   {
      if (swf5debug) putchar(c);

      countline();
   }
}

static void count(void)
{
   int n;

   // Count the characters to maintain the current column position
   if (yytext[0] == '\n')
   {
      if (swf5debug) printf("\n");
   }
   else
   {
      if (swf5debug) printf("%s", yytext);

      for(n=0; n<yyleng; ++n, ++column)
      {
	if(column < 1023)
	  msgline[column] = yytext[n];
      }

      //-- keep writing the stuff to standard output
      //column += yyleng;
   }
}

static void printprog()
{
  if(sLineNumber)
    SWF_warn("\n%s", msgbufs[(sLineNumber-1)&1]);

  if(column < 1023)
    msgline[column] = 0;

  SWF_warn("\n%s", msgline);
}

static void warning(char *msg)
{
   // print a warning message
   printprog();
   SWF_warn("\n%*s", ColumnNumber(), "^");
   SWF_warn("\nLine %4.4d:  Reason: '%s' \n", LineNumber(), msg);
}

void swf5error(char *msg)
{
  // report a error
  if(strlen(yytext))
  {
    SWF_error("\n%s\n%*s\nLine %i:  Reason: '%s'\n",
	      LineText(), ColumnNumber(), "^", LineNumber(), msg);
  }
  else
  {
    SWF_error("\nLine %d: Reason: 'Unexpected EOF found while looking for input.'\n", LineNumber());
  }
}
