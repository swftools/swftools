/* tokenizer.lex

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
    fprintf(stderr, "%s:%d:%d: error: %s\n", current_filename_short, current_line, current_column, buf);
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

static void handleString(char*s, int len)
{
    if(s[0]=='"') {
        if(s[len-1]!='"') syntaxerror("String doesn't end with '\"'");
        s++;len-=2;
    }
    else if(s[0]=='\'') {
        if(s[len-1]!='\'') syntaxerror("String doesn't end with '\"'");
        s++;len-=2;
    }
    else syntaxerror("String incorrectly terminated");
    s[len] = 0;
    avm2_lval.string = s;
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

static char numberbuf[64];
static inline int handlenumber()
{
    if(yyleng>sizeof(numberbuf)-1)
        syntaxerror("decimal number overflow");

    char*s = numberbuf;
    memcpy(s, yytext, yyleng);
    s[yyleng]=0;

    int t;
    char is_float=0;
    for(t=0;t<yyleng;t++) {
        if(yytext[t]=='.') {
            if(is_float)
                syntaxerror("Invalid number");
            is_float=1;
        } else if(!strchr("-0123456789", yytext[t])) {
            syntaxerror("Invalid number");
        }
    }
    if(is_float) {
        avm2_lval.number_float = atof(s);
        return T_FLOAT;
    } 
    char l = (yytext[0]=='-');

    char*max = l?"1073741824":"2147483647";
    if(yyleng-l>10)
        syntaxerror("integer overflow");
    if(yyleng-l==10) {
        int t;
        for(t=0;t<yyleng-l;t++) {
            if(yytext[l+t]>max[t])
                syntaxerror("integer overflow %s > %s", s+l,max);
            else if(yytext[l+t]<max[t])
                break;
        }
    }
    if(yytext[0]=='-') {
        int v = atoi(s);
        avm2_lval.number_int = v;
        if(v>-128)
            return T_BYTE;
        else if(v>=-32768)
            return T_SHORT;
        else
            return T_INT;
    } else {
        unsigned int v = 0;
        for(t=0;t<yyleng;t++) {
            v*=10;
            v+=yytext[t]-'0';
        }
        avm2_lval.number_uint = v;
        if(v<128)
            return T_BYTE;
        else if(v<32768)
            return T_SHORT;
        else
            return T_UINT;
    }
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
MULTILINE_COMMENT [/][*]+([*][^/]|[^/*]|[\x00-\x1f])*[*]+[/]
SINGLELINE_COMMENT \/\/[^\n]*\n
REGEXP   [/]([^/\n]|\\[/])*[/][a-zA-Z]*
%%


{SINGLELINE_COMMENT}         {c(); /* single line comment */}
{MULTILINE_COMMENT}          {c(); /* multi line comment */}
[/][*]                       {syntaxerror("syntax error: unterminated comment", yytext);}

^include{S}+{STRING}{S}*/\n    {c();handleInclude(yytext, yyleng, 1);}
^include{S}+[^" \t\r\n][\x20-\xff]*{S}*/\n    {c();handleInclude(yytext, yyleng, 0);}
{STRING}                     {c(); BEGIN(INITIAL);handleString(yytext, yyleng);return T_STRING;}

<BEGINNING,REGEXPOK>{
{REGEXP}                     {c(); BEGIN(INITIAL);return m(T_REGEXP);} 
}

\xef\xbb\xbf                 {/* utf 8 bom */}
{S}                          {c();}

{NUMBER}                     {c(); BEGIN(INITIAL);return handlenumber();}

3rr0r                        {/* for debugging: generates a tokenizer-level error */
                              syntaxerror("3rr0r");}

[&][&]                       {BEGIN(REGEXPOK);return m(T_ANDAND);}
[|][|]                       {BEGIN(REGEXPOK);return m(T_OROR);}
[!][=]                       {BEGIN(REGEXPOK);return m(T_NE);}
[=][=][=]                    {BEGIN(REGEXPOK);return m(T_EQEQEQ);}
[=][=]                       {BEGIN(REGEXPOK);return m(T_EQEQ);}
[>][=]                       {return m(T_GE);}
[<][=]                       {return m(T_LE);}
[+][=]                       {return m(T_PLUSBY);}
[-][=]                       {return m(T_MINUSBY);}
[-][-]                       {BEGIN(INITIAL);return m(T_MINUSMINUS);}
[+][+]                       {BEGIN(INITIAL);return m(T_PLUSPLUS);}
\.\.                         {return m(T_DOTDOT);}
\.                           {return m('.');}
::                           {return m(T_COLONCOLON);}
:                            {return m(':');}
implements                   {return m(KW_IMPLEMENTS);}
interface                    {return m(KW_INTERFACE);}
namespace                    {return m(KW_NAMESPACE);}
protected                    {return m(KW_PROTECTED);}
override                     {return m(KW_OVERRIDE);}
internal                     {return m(KW_INTERNAL);}
function                     {return m(KW_FUNCTION);}
package                      {return m(KW_PACKAGE);}
private                      {return m(KW_PRIVATE);}
Boolean                      {return m(KW_BOOLEAN);}
dynamic                      {return m(KW_DYNAMIC);}
extends                      {return m(KW_EXTENDS);}
return                       {return m(KW_RETURN);}
public                       {return m(KW_PUBLIC);}
native                       {return m(KW_NATIVE);}
static                       {return m(KW_STATIC);}
import                       {return m(KW_IMPORT);}
Number                       {return m(KW_NUMBER);}
while                        {return m(KW_WHILE);}
class                        {return m(KW_CLASS);}
const                        {return m(KW_CONST);}
final                        {return m(KW_FINAL);}
false                        {return m(KW_FALSE);}
break                        {return m(KW_BREAK);}
true                         {return m(KW_TRUE);}
uint                         {return m(KW_UINT);}
null                         {return m(KW_NULL);}
else                         {return m(KW_ELSE);}
use                          {return m(KW_USE);}
int                          {return m(KW_INT);}
new                          {return m(KW_NEW);}
get                          {return m(KW_GET);}
for                          {return m(KW_FOR);}
set                          {return m(KW_SET);}
var                          {return m(KW_VAR);}
is                           {return m(KW_IS) ;}
if                           {return m(KW_IF) ;}
as                           {return m(KW_AS);}
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
    if(nr==T_STRING)     return "<string>";
    else if(nr==T_INT)     return "<int>";
    else if(nr==T_UINT)     return "<uint>";
    else if(nr==T_FLOAT)     return "<float>";
    else if(nr==T_REGEXP)     return "REGEXP";
    else if(nr==T_EOF)        return "***END***";
    else if(nr==T_GE)         return ">=";
    else if(nr==T_LE)         return "<=";
    else if(nr==T_MINUSMINUS) return "--";
    else if(nr==T_PLUSPLUS)   return "++";
    else if(nr==KW_IMPLEMENTS) return "implements";
    else if(nr==KW_INTERFACE)  return "interface";
    else if(nr==KW_NAMESPACE)  return "namespace";
    else if(nr==KW_PROTECTED)  return "protected";
    else if(nr==KW_OVERRIDE)   return "override";
    else if(nr==KW_INTERNAL)   return "internal";
    else if(nr==KW_FUNCTION)   return "function";
    else if(nr==KW_PACKAGE)    return "package";
    else if(nr==KW_PRIVATE)    return "private";
    else if(nr==KW_BOOLEAN)    return "Boolean";
    else if(nr==KW_DYNAMIC)    return "dynamic";
    else if(nr==KW_EXTENDS)    return "extends";
    else if(nr==KW_PUBLIC)     return "public";
    else if(nr==KW_NATIVE)     return "native";
    else if(nr==KW_STATIC)     return "static";
    else if(nr==KW_IMPORT)     return "import";
    else if(nr==KW_NUMBER)     return "number";
    else if(nr==KW_CLASS)      return "class";
    else if(nr==KW_CONST)      return "const";
    else if(nr==KW_FINAL)      return "final";
    else if(nr==KW_FALSE)      return "False";
    else if(nr==KW_TRUE)       return "True";
    else if(nr==KW_UINT)       return "uint";
    else if(nr==KW_NULL)       return "null";
    else if(nr==KW_ELSE)       return "else";
    else if(nr==KW_USE)        return "use";
    else if(nr==KW_INT)        return "int";
    else if(nr==KW_NEW)        return "new";
    else if(nr==KW_GET)        return "get";
    else if(nr==KW_FOR)        return "for";
    else if(nr==KW_SET)        return "set";
    else if(nr==KW_VAR)        return "var";
    else if(nr==KW_IS)         return "is";
    else if(nr==KW_AS)         return "as";
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

