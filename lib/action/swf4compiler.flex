%{

#include <math.h>
#include <string.h>

#include "compile.h"
#include "action.h"
#include "blocks/error.h"
#include "swf4compiler.tab.h" /* defines token types */

static int swf4debug;

static const char *lexBuffer = NULL;
static int lexBufferLen = 0;

static int  sLineNumber = 0;
static char szLine[1024];
static char msgbufs[2][1024] = { {0}, {0} }, *msgline = {0};
static int  column = 0;

static void comment();
static void comment1();
static void count();
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

void swf4ParseInit(const char *script, int debug)
{
  checkByteOrder();
  yyrestart(NULL);

  swf4debug = debug;

  lexBuffer = script;
  lexBufferLen = strlen(script);
  sLineNumber = 0;
  column = 0;
  msgline = msgbufs[0];
}

%}

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

 void do_unput4(const char c) { unput(c); }
%}

DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*
LEVEL	 \.\.?

%%

{DIGIT}+		{ count();	swf4lval.str = strdup(yytext);
					return NUMBER; 		}
{DIGIT}+"."{DIGIT}*	{ count();	swf4lval.str = strdup(yytext);
					return NUMBER; 		}
true			{ count();	swf4lval.str = strdup("1");
					return NUMBER;		}
false			{ count();	swf4lval.str = strdup("0");
					return NUMBER;		}
break			{ count();	return BREAK;		}
continue		{ count();	return CONTINUE;	}
else			{ count();	return ELSE;		}
for			{ count();	return FOR;		}
if			{ count();	return IF;		}
while			{ count();	return WHILE;		}
do			{ count();	return DO;		}
valueOf			{ count(); 	return EVAL;		}

  /* functions */
random		{ count();	return RANDOM;	}
time		{ count();	return TIME;	}
length		{ count();	return LENGTH;	}
int		{ count();	return INT;	}
concat		{ count();	return CONCAT;	}
duplicateClip	{ count();	return DUPLICATECLIP;	}
removeClip	{ count();	return REMOVECLIP;	}
trace		{ count();	return TRACE;	}
startDrag	{ count();	return STARTDRAG;	}
stopDrag	{ count();	return STOPDRAG;	}
ord		{ count();	return ORD;	}
chr		{ count();	return CHR;	}
callFrame	{ count();	return CALLFRAME;	}
getURL		{ count();	return GETURL;	}
getURL1		{ count();	return GETURL1; }
loadMovie	{ count();	return LOADMOVIE;	}
loadVariables	{ count();	return LOADVARIABLES;	}
substr		{ count();	return SUBSTR;	}

getProperty	{ count();	return GETPROPERTY;	}

  /* getURL2 methods */
post		{ count();	swf4lval.getURLMethod = GETURL_METHOD_POST;
				return GETURL_METHOD;	}
get		{ count();	swf4lval.getURLMethod = GETURL_METHOD_GET;
				return GETURL_METHOD;	}
nosend		{ count();	swf4lval.getURLMethod = GETURL_METHOD_NOSEND;
				return GETURL_METHOD;	}


  /* v3 functions */
nextFrame	{ count();	return NEXTFRAME;	}
prevFrame	{ count();	return PREVFRAME;	}
play		{ count();	return PLAY;		}
stop		{ count();	return STOP;		}
toggleQuality	{ count();	return TOGGLEQUALITY;	}
stopSounds	{ count();	return STOPSOUNDS;	}
gotoFrame	{ count();	return GOTOFRAME;	}
gotoAndPlay	{ count();	return GOTOANDPLAY;	}
frameLoaded	{ count();	return FRAMELOADED;	}
setTarget	{ count();	return SETTARGET;	}

  /* high level functions */
tellTarget	{ count();	return TELLTARGET;	}


this			{ count();      return THIS;	}

{ID}			{ count();	swf4lval.str = strdup(yytext);
					return IDENTIFIER;	}

{LEVEL}?("/"({ID}|{LEVEL}))+ { count();	swf4lval.str = strdup(yytext);
					return PATH;    }
                    
{ID}("/"({ID}|{LEVEL}))+ { count();	swf4lval.str = strdup(yytext);
					return PATH;    }

\"(\\.|[^\\"])*\"	{ count();	swf4lval.str = strdup(yytext+1);
					swf4lval.str[strlen(swf4lval.str)-1]=0;
                                        unescape(swf4lval.str);
					return STRING;		}

\'(\\.|[^\\'])*\'	{ count();	swf4lval.str = strdup(yytext+1);
					swf4lval.str[strlen(swf4lval.str)-1]=0;
                                        unescape(swf4lval.str);
					return STRING; 		}

\"(\\.|[^\\"])*$	{ count();	swf4lval.str = strdup("");
					warning("Unterminated string!");
					return STRING;		}

\'(\\.|[^\\'])*$	{ count();	swf4lval.str = strdup("");
					warning("Unterminated string!");
					return STRING;		}

"/*"			{ count();	comment();		}
"//"			{ count();	comment1();		}
[ \t\v\f]		{ count(); }

"++"			{ count();	return INC; }
"--"			{ count();	return DEC; }
"<"			{ count();	return '<'; }
">"			{ count();	return '>'; }
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
"==="			{ count();	return STREQ; }
"!=="			{ count();	return STRNE; }
"<=>"			{ count();	return STRCMP; }
".."			{ count();	return PARENT; }

";"			{ count();	return ';'; }
"="			{ count();	return '='; }
"+"			{ count();	return '+'; }
"-"			{ count();	return '-'; }
"&"			{ count();	return '&'; }
"*"			{ count();	return '*'; }
"/"			{ count();	return '/'; }
"!"			{ count();	return '!'; }
"("			{ count();	return '('; }
")"			{ count();	return ')'; }
"["			{ count();	return '['; }
"]"			{ count();	return ']'; }
"{"			{ count();	return '{'; }
"}"			{ count();	return '}'; }
","			{ count();	return ','; }
"."			{ count();	return '.'; }
"?"			{ count();	return '?'; }
":"			{ count();	return ':'; }

\r?\n			{ count();	column = 0;
					strcpy(szLine, yytext + 1);
					++sLineNumber;	yyless(1);	}

.			printf( "Unrecognized character: %s\n", yytext );

%%
static int getinput() {
#ifdef __cplusplus
					return yyinput();
#else
					return input();
#endif
}

int swf4wrap()
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

static void comment(void)
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

      if (swf4debug) putchar(c);
   }

   // is this the end of comment character
   if ((c1 = getinput()) != '/' && c != EOF)
   {
      // false start as this was no end of comment
      do_unput4(c1);
      goto loop;
   }

   // write out the start of the end of comment
   if (c != EOF)
      if (swf4debug) putchar(c);

   // write out the end of the end of comment
   if (c1 != EOF) 
      if (swf4debug) putchar(c1);
}

static void comment1(void)
{
   // Handle comment of type 1 (ie '//')

   int c;

   // this is a line comment
   while ((c = getinput()) != '\n' && c != EOF)
   {
      if (swf4debug) putchar(c);

      if(column < 1023)
         msgline[column] = c;

      ++column;
   };

   // keep the line number in synch
   if (c == '\n')
   {
      if (swf4debug) putchar(c);

      countline();
   }
}

static void count(void)
{
   int n;

   // Count the characters to maintain the current column position
   if (yytext[0] == '\n')
   {
      if (swf4debug) printf("\n");
   }
   else
   {
      if (swf4debug) printf("%s", yytext);

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

void swf4error(char *msg)
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
