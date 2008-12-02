%{
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "../utf8.h"
#include "tokenizer.h"
#include "files.h"

static void countlines(char*text, int len) {
    int t;
    for(t=0;t<len;t++) {
	if(text[t]=='\n') {
	    current_line++;
	    current_column=0;
	} else {
	    current_column++;
	}
    }
}

static int verbose = 1;
static void dbg(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(!verbose)
	return;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    printf("(tokenizer) ");
    printf("%s\n", buf);
    fflush(stdout);
}

void syntaxerror(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(!verbose)
	return;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    fprintf(stderr, "%s:%d:%d: error: %s\n", current_filename, current_line, current_column, buf);
    fflush(stderr);
    exit(1);
}


#ifndef YY_CURRENT_BUFFER
#define YY_CURRENT_BUFFER yy_current_buffer
#endif

void handleInclude(char*text, int len, char quotes)
{
    char*filename = 0;
    if(quotes) {
        char*p1 = strchr(text, '"');
        char*p2 = strrchr(text, '"');
        if(!p1 || !p2 || p1==p2) {
            syntaxerror("Invalid include in line %d\n", current_line);
        }
        *p2 = 0;
        filename = strdup(p1+1);
    } else {
        int i1=0,i2=len;
        // find start
        while(!strchr(" \n\r\t", text[i1])) i1++;
        // strip
        while(strchr(" \n\r\t", text[i1])) i1++;
        while(strchr(" \n\r\t", text[i2-1])) i2--;
        if(i2!=len) text[i2]=0;
        filename = strdup(&text[i1]);
    }
    
    char*fullfilename = enter_file(filename, YY_CURRENT_BUFFER);
    yyin = fopen(fullfilename, "rb");
    if (!yyin) {
	syntaxerror("Couldn't open include file \"%s\"\n", fullfilename);
    }

    yy_switch_to_buffer(yy_create_buffer( yyin, YY_BUF_SIZE ) );
    //BEGIN(INITIAL); keep context
}

char start_of_expression;

static inline int m(int type)
{
    char*s = malloc(yyleng+1);
    memcpy(s, yytext, yyleng);
    s[yyleng]=0;

    NEW(token_t,t);
    t->type = type;
    t->text = s;
    avm2_lval.token = t;
    return type;
}

void initialize_scanner();
#define YY_USER_INIT initialize_scanner();

#define c() {countlines(yytext, yyleng);}

%}

%s REGEXPOK
%s BEGINNING

NAME	 [a-zA-Z_][a-zA-Z0-9_\\]*
NUMBER	 -?[0-9]+(\.[0-9]*)?
STRING   ["](\\[\x00-\xff]|[^\\"\n])*["]|['](\\[\x00-\xff]|[^\\'\n])*[']
S 	 [ \n\r\t]
MULTILINE_COMMENT [/][*]([*][^/]|[^*]|[\x00-\x31])*[*]+[/]
SINGLELINE_COMMENT \/\/[^\n]*\n
REGEXP   [/]([^/\n]|\\[/])*[/][a-zA-Z]*
%%


{SINGLELINE_COMMENT}         {c(); /* single line comment */}
{MULTILINE_COMMENT}          {c(); /* multi line comment */}
[/][*]                       {syntaxerror("syntax error: unterminated comment", yytext);}

^include{S}+{STRING}{S}*/\n    {c();handleInclude(yytext, yyleng, 1);}
^include{S}+[^" \t\r\n][\x20-\xff]*{S}*/\n    {c();handleInclude(yytext, yyleng, 0);}
{STRING}                     {c(); return m(T_STRING);BEGIN(INITIAL);}

<BEGINNING,REGEXPOK>{
{REGEXP}                     {c(); return m(T_REGEXP);BEGIN(INITIAL);} 
}

\xef\xbb\xbf                 {/* utf 8 bom */}
{S}                          {c();}

{NUMBER}                     {c();return m(T_NUMBER);BEGIN(INITIAL);}
[>][=]                       {return m(T_GE);}
[<][=]                       {return m(T_LE);}
[-][-]                       {return m(T_MINUSMINUS);BEGIN(INITIAL);}
[+][+]                       {return m(T_PLUSPLUS);BEGIN(INITIAL);}
==                           {return m(T_EQEQ);BEGIN(REGEXPOK);}
\.\.                         {return m(T_DOTDOT);}
\.                           {return m('.');}
::                           {return m(T_COLONCOLON);}
:                            {return m(':');}
implements                   {return m(T_IMPLEMENTS);}
interface                    {return m(T_INTERFACE);}
namespace                    {return m(T_NAMESPACE);}
protected                    {return m(T_PROTECTED);}
override                     {return m(T_OVERRIDE);}
internal                     {return m(T_INTERNAL);}
function                     {return m(T_FUNCTION);}
package                      {return m(T_PACKAGE);}
private                      {return m(T_PRIVATE);}
Boolean                      {return m(T_BOOLEAN);}
dynamic                      {return m(T_DYNAMIC);}
extends                      {return m(T_EXTENDS);}
public                       {return m(T_PUBLIC);}
native                       {return m(T_NATIVE);}
static                       {return m(T_STATIC);}
import                       {return m(T_IMPORT);}
number                       {return m(T_NUMBER);}
class                        {return m(T_CLASS);}
const                        {return m(T_CONST);}
final                        {return m(T_FINAL);}
False                        {return m(T_FALSE);}
True                         {return m(T_TRUE);}
uint                         {return m(T_UINT);}
null                         {return m(T_NULL);}
use                          {return m(T_USE);}
int                          {return m(T_INT);}
new                          {return m(T_NEW);}
get                          {return m(T_GET);}
for                          {return m(T_FOR);}
set                          {return m(T_SET);}
var                          {return m(T_VAR);}
is                           {return m(T_IS) ;}
as                           {return m(T_AS);}
{NAME}                       {c();BEGIN(INITIAL);return m(T_IDENTIFIER);}

[+-\/*^~@$!%&\(=\[\]\{\}|?:;,.<>] {c();BEGIN(REGEXPOK);return m(yytext[0]);}
[\)\]]                            {c();BEGIN(INITIAL);return m(yytext[0]);}

.		             {char c1=yytext[0];
                              char buf[128];
                              buf[0] = yytext[0];
                              int t;
                              for(t=1;t<128;t++) {
		                  char c = buf[t]=input();
		                  if(c=='\n' || c==EOF)  {
                                      buf[t] = 0;
		                      break;
                                  }
		              }
			      if(c1>='0' && c1<='9')
			          syntaxerror("syntax error: %s (identifiers must not start with a digit)");
                              else
		                  syntaxerror("syntax error: %s", buf);
		              printf("\n");
			      exit(1);
		              yyterminate();
		             }
<<EOF>>		             {c();
                              void*b = leave_file();
			      if (!b) {
			         yyterminate();
                                 yy_delete_buffer(YY_CURRENT_BUFFER);
                                 return m(T_EOF);
			      } else {
			          yy_delete_buffer(YY_CURRENT_BUFFER);
			          yy_switch_to_buffer(b);
			      }
			     }

%%

int yywrap()
{
    return 1;
}

static char mbuf[256];
char*token2string(token_t*t)
{
    int nr=t->type;
    if(nr==T_STRING)     return "STRING";
    else if(nr==T_NUMBER)     return "NUMBER";
    else if(nr==T_REGEXP)     return "REGEXP";
    else if(nr==T_EOF)        return "***END***";
    else if(nr==T_GE)         return ">=";
    else if(nr==T_LE)         return "<=";
    else if(nr==T_MINUSMINUS) return "--";
    else if(nr==T_PLUSPLUS)   return "++";
    else if(nr==T_IMPLEMENTS) return "implements";
    else if(nr==T_INTERFACE)  return "interface";
    else if(nr==T_NAMESPACE)  return "namespace";
    else if(nr==T_PROTECTED)  return "protected";
    else if(nr==T_OVERRIDE)   return "override";
    else if(nr==T_INTERNAL)   return "internal";
    else if(nr==T_FUNCTION)   return "function";
    else if(nr==T_PACKAGE)    return "package";
    else if(nr==T_PRIVATE)    return "private";
    else if(nr==T_BOOLEAN)    return "Boolean";
    else if(nr==T_DYNAMIC)    return "dynamic";
    else if(nr==T_EXTENDS)    return "extends";
    else if(nr==T_PUBLIC)     return "public";
    else if(nr==T_NATIVE)     return "native";
    else if(nr==T_STATIC)     return "static";
    else if(nr==T_IMPORT)     return "import";
    else if(nr==T_NUMBER)     return "number";
    else if(nr==T_CLASS)      return "class";
    else if(nr==T_CONST)      return "const";
    else if(nr==T_FINAL)      return "final";
    else if(nr==T_FALSE)      return "False";
    else if(nr==T_TRUE)       return "True";
    else if(nr==T_UINT)       return "uint";
    else if(nr==T_NULL)       return "null";
    else if(nr==T_USE)        return "use";
    else if(nr==T_INT)        return "int";
    else if(nr==T_NEW)        return "new";
    else if(nr==T_GET)        return "get";
    else if(nr==T_FOR)        return "for";
    else if(nr==T_SET)        return "set";
    else if(nr==T_VAR)        return "var";
    else if(nr==T_IS)         return "is";
    else if(nr==T_AS)         return "as";
    else if(nr==T_IDENTIFIER) {
        if(strlen(t->text)>sizeof(mbuf)-1)
            return "ID(...)";
        sprintf(mbuf, "ID(%s)", t->text);
        return mbuf;
    } else {
        sprintf(mbuf, "%d", nr);
        return mbuf;
    }
}

void initialize_scanner()
{
    BEGIN(BEGINNING);
}

