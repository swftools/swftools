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

int as3_pass = 0;
int as3_verbosity = 1;
unsigned int as3_tokencount = 0;

void as3_error(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(as3_verbosity<0)
        exit(1);
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    fprintf(stderr, "%s:%d:%d: error: %s\n", current_filename_short, current_line, current_column, buf);
    fflush(stderr);
    exit(1);
}
void as3_warning(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(as3_verbosity<1)
        return;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    fprintf(stderr, "%s:%d:%d: warning: %s\n", current_filename_short, current_line, current_column, buf);
    fflush(stderr);
}
void as3_softwarning(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(as3_verbosity<2)
	return;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    fprintf(stderr, "%s:%d:%d: warning: %s\n", current_filename_short, current_line, current_column, buf);
    fflush(stderr);
}
static void dbg(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(as3_verbosity<3)
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

#ifndef YY_CURRENT_BUFFER
#define YY_CURRENT_BUFFER yy_current_buffer
#endif

static void*as3_buffer=0;
static int as3_buffer_pos=0;
static int as3_buffer_len=0;
void as3_file_input(FILE*fi)
{
    as3_in = fi;
    as3_buffer = 0;
}
void as3_buffer_input(void*buffer, int len)
{
    if(!buffer)
        syntaxerror("trying to parse zero bytearray");
    as3_buffer = buffer;
    as3_buffer_len = len;
    as3_buffer_pos = 0;
    as3_in = 0;
}

#define YY_INPUT(buf,result,max_size) { \
  if(!as3_buffer) { \
      errno=0; \
      while((result = fread(buf, 1, max_size, as3_in))==0 && ferror(as3_in)) \
      { if(errno != EINTR) {YY_FATAL_ERROR("input in flex scanner failed"); break;} \
        errno=0; clearerr(as3_in); \
      } \
  } else { \
      int to_read = max_size; \
      if(to_read + as3_buffer_pos > as3_buffer_len) \
          to_read = as3_buffer_len - as3_buffer_pos; \
      memcpy(buf, as3_buffer+as3_buffer_pos, to_read); \
      as3_buffer_pos += to_read; \
      result=to_read; \
  } \
}

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
    
    char*fullfilename = find_file(filename, 1);
    enter_file2(filename, fullfilename, YY_CURRENT_BUFFER);
    yyin = fopen(fullfilename, "rb");
    if (!yyin) {
	syntaxerror("Couldn't open include file \"%s\"\n", fullfilename);
    }

    yy_switch_to_buffer(yy_create_buffer( yyin, YY_BUF_SIZE ) );
    //BEGIN(INITIAL); keep context
}

static int do_unescape(const char*s, const char*end, char*n) 
{
    char*o = n;
    int len=0;
    while(s<end) {
        if(*s!='\\') {
            if(o) o[len] = *s;len++;
            s++;
            continue;
        }
        s++; //skip past '\'
        if(s==end) syntaxerror("invalid \\ at end of string");

        /* handle the various line endings (mac, dos, unix) */
        if(*s=='\r') { 
            s++; 
            if(s==end) break;
            if(*s=='\n') 
                s++;
            continue;
        }
        if(*s=='\n')  {
            s++;
            continue;
        }
        switch(*s) {
	    case '\\': if(o) o[len] = '\\';s++;len++; break;
	    case '"': if(o) o[len] = '"';s++;len++; break;
	    case '\'': if(o) o[len] = '\'';s++;len++; break;
	    case 'b': if(o) o[len] = '\b';s++;len++; break;
	    case 'f': if(o) o[len] = '\f';s++;len++; break;
	    case 'n': if(o) o[len] = '\n';s++;len++; break;
	    case 'r': if(o) o[len] = '\r';s++;len++; break;
	    case 't': if(o) o[len] = '\t';s++;len++; break;
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
                unsigned int num=0;
                int nr = 0;
		while(strchr("01234567", *s) && nr<3 && s<end) {
                    num <<= 3;
                    num |= *s-'0';
                    nr++;
                    s++;
                }
                if(num>256) 
                    syntaxerror("octal number out of range (0-255): %d", num);
                if(o) o[len] = num;len++;
                continue;
            }
	    case 'x': case 'u': {
		int max=2;
		char bracket = 0;
                char unicode = 0;
		if(*s == 'u') {
		    max = 6;
                    unicode = 1;
                }
                s++;
                if(s==end) syntaxerror("invalid \\u or \\x at end of string");
		if(*s == '{')  {
                    s++;
                    if(s==end) syntaxerror("invalid \\u{ at end of string");
		    bracket=1;
		}
		unsigned int num=0;
                int nr = 0;
		while(strchr("0123456789abcdefABCDEF", *s) && (bracket || nr < max) && s<end) {
		    num <<= 4;
		    if(*s>='0' && *s<='9') num |= *s - '0';
		    if(*s>='a' && *s<='f') num |= *s - 'a' + 10;
		    if(*s>='A' && *s<='F') num |= *s - 'A' + 10;
                    nr++;
		    s++;
		}
		if(bracket) {
                    if(*s=='}' && s<end) {
                        s++;
                    } else {
                        syntaxerror("missing terminating '}'");
                    }
		}
                if(unicode) {
                    char*utf8 = getUTF8(num);
                    while(*utf8) {
                        if(o) o[len] = *utf8;utf8++;len++;
                    }
                } else {
                    if(num>256) 
                        syntaxerror("byte out of range (0-255): %d", num);
                    if(o) o[len] = num;len++;
                }
		break;
	    }
            default: {
	        if(o) {
                    o[len+0] = '\\';
                    o[len+1] = *s;
                }
                s++;
                len+=2;
                break;
            }
        }
    }
    if(o) o[len]=0;
    return len;
}

static string_t string_unescape(const char*in, int l)
{
    const char*s = in;
    const char*end = &in[l];

    int len = do_unescape(s, end, 0);
    char*n = (char*)malloc(len+1);
    do_unescape(s, end, n);
    string_t out = string_new(n, len);
    return out; 
}

static void handleCData(char*s, int len)
{
    a3_lval.str.str = s+9;    // <![CDATA[
    a3_lval.str.len = len-9-3;// ]]>
    a3_lval.str.str = strdup_n(a3_lval.str.str, a3_lval.str.len);
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
    
    a3_lval.str = string_unescape(s, len);
}


char start_of_expression;

static inline int m(int type)
{
    a3_lval.token = type;
    return type;
}


static char numberbuf[64];
static char*nrbuf()
{
    if(yyleng>sizeof(numberbuf)-1)
        syntaxerror("decimal number overflow");
    char*s = numberbuf;
    memcpy(s, yytext, yyleng);
    s[yyleng]=0;
    return s;
}

static inline int setint(int v)
{
    a3_lval.number_int = v;
    if(v>-128)
        return T_BYTE;
    else if(v>=-32768)
        return T_SHORT;
    else
        return T_INT;
}
static inline int setuint(unsigned int v)
{
    a3_lval.number_uint = v;
    if(v<128)
        return T_BYTE;
    else if(v<32768)
        return T_SHORT;
    else
        return T_UINT;
}
static inline int setfloat(double v)
{
    a3_lval.number_float = v;
    return T_FLOAT;
}

static inline int handlefloat()
{
    char*s = nrbuf();
    a3_lval.number_float = atof(s);
    return T_FLOAT;
}

static inline int handleint()
{
    char*s = nrbuf();
    char l = (yytext[0]=='-');

    char*max = l?"1073741824":"2147483647";
    if(yyleng-l>10) {
        as3_softwarning("integer overflow: %s (converted to Number)", s);
        return handlefloat();
    }
    if(yyleng-l==10) {
        int t;
        for(t=0;t<yyleng-l;t++) {
            if(yytext[l+t]>max[t]) {
                as3_softwarning("integer overflow: %s (converted to Number)", s);
                return handlefloat();
            }
            else if(yytext[l+t]<max[t])
                break;
        }
    }
    if(yytext[0]=='-') {
        int v = atoi(s);
        return setint(v);
    } else {
        unsigned int v = 0;
        int t;
        for(t=0;t<yyleng;t++) {
            v*=10;
            v+=yytext[t]-'0';
        }
        return setuint(v);
    }
}

static inline int handlehexfloat()
{
    char l = (yytext[0]=='-')+2;
    double d=0;
    char dot=0;
    double base=1;
    int t;
    for(t=l;t<yyleng;t++) {
        char c = yytext[t];
        if(c=='.') {
            dot=1;
            continue;
        }
        if(!dot) {
            d*=16;
        } else {
            base*=1/16.0;
        }
        if(c>='0' && c<='9')
            d+=(c&15)*base;
        else if((c>='a' && c<='f') || (c>='A' && c<='F'))
            d+=((c&0x0f)+9)*base;
    }
    return setfloat(d);
}
static inline int handlehex()
{
    char l = (yytext[0]=='-')+2;
    int len = yyleng;

    if(len-l>8) {
        char*s = nrbuf();
        syntaxerror("integer overflow %s", s);
    }

    int t;
    unsigned int v = 0;
    for(t=l;t<len;t++) {
        v<<=4;
        char c = yytext[t];
        if(c>='0' && c<='9')
            v|=(c&15);
        else if((c>='a' && c<='f') || (c>='A' && c<='F'))
            v|=(c&0x0f)+9;
    }
    if(l && v>1073741824) {
        char*s = nrbuf();
        as3_softwarning("signed integer overflow: %s (converted to Number)", s);
        return setfloat(v);
    }
    if(!l && v>2147483647) {
        char*s = nrbuf();
        as3_softwarning("unsigned integer overflow: %s (converted to Number)", s);
        return setfloat(v);
    }

    if(l==3) {
        return setint(-(int)v);
    } else {
        return setuint(v);
    }
}

void handleLabel(char*text, int len)
{
    int t;
    for(t=len-1;t>=0;--t) {
        if(text[t]!=' ' &&
           text[t]!=':')
            break;
    }
    char*s = malloc(t+1);
    memcpy(s, yytext, t);
    s[t]=0;
    a3_lval.id = s;
}

static int handleregexp()
{
    char*s = malloc(yyleng);
    int len=yyleng-1;
    memcpy(s, yytext+1, len);
    s[len] = 0;
    int t;
    for(t=len;t>=0;--t) {
        if(s[t]=='/') {
            s[t] = 0;
            break;
        }
    }
    a3_lval.regexp.pattern = s;
    if(t==len) {
        a3_lval.regexp.options = 0;
    } else {
        a3_lval.regexp.options = s+t+1;
    }
    return T_REGEXP;
}

void initialize_scanner();
#define YY_USER_INIT initialize_scanner();

/* count the number of lines+columns consumed by this token */
static inline void l() {
    int t;
    for(t=0;t<yyleng;t++) {
	if(yytext[t]=='\n') {
	    current_line++;
	    current_column=0;
	} else {
	    current_column++;
	}
    }
}
/* count the number of columns consumed by this token */
static inline void c() {
    current_column+=yyleng;
}

trie_t*active_namespaces = 0;
/*void tokenizer_register_namespace(const char*id)
{
    trie_put(namespaces, id, 0);
}
void tokenizer_unregister_namespace(const char*id)
{
    trie_remove(namespaces, id);
}*/
static inline tokenizer_is_namespace(const char*id)
{
    return trie_contains(active_namespaces, id);
}

static inline int handleIdentifier()
{
    char*s = malloc(yyleng+1);
    memcpy(s, yytext, yyleng);
    s[yyleng]=0;
    a3_lval.id = s;
    if(tokenizer_is_namespace(s)) 
        return T_NAMESPACE;
    else
        return T_IDENTIFIER;
}


//Boolean                      {c();return m(KW_BOOLEAN);}
//int                          {c();return m(KW_INT);}
//uint                         {c();return m(KW_UINT);}
//Number                       {c();return m(KW_NUMBER);}

%}

%s REGEXPOK
%s BEGINNING

NAME	 [a-zA-Z_][a-zA-Z0-9_\\]*
_        [^a-zA-Z0-9_\\]

HEXINT    0x[a-zA-Z0-9]+
HEXFLOAT  0x[a-zA-Z0-9]*\.[a-zA-Z0-9]*
INT       [0-9]+
FLOAT     ([0-9]+(\.[0-9]*)?|\.[0-9]+)(e[0-9]+)?

HEXWITHSIGN [+-]?({HEXINT})
HEXFLOATWITHSIGN [+-]?({HEXFLOAT})
INTWITHSIGN [+-]?({INT})
FLOATWITHSIGN [+-]?({FLOAT})

CDATA    <!\[CDATA\[([^]]|\][^]]|\]\][^>])*\]*\]\]\>
STRING   ["](\\[\x00-\xff]|[^\\"\n])*["]|['](\\[\x00-\xff]|[^\\'\n])*[']
S 	 [ \n\r\t]
MULTILINE_COMMENT [/][*]+([*][^/]|[^/*]|[^*][/]|[\x00-\x1f])*[*]+[/]
SINGLELINE_COMMENT \/\/[^\n\r]*[\n\r]
REGEXP   [/]([^/\n]|\\[/])*[/][a-zA-Z]*
%%


{SINGLELINE_COMMENT}         {l(); /* single line comment */}
{MULTILINE_COMMENT}          {l(); /* multi line comment */}
[/][*]                       {syntaxerror("syntax error: unterminated comment", yytext);}

^include{S}+{STRING}{S}*/\n    {l();handleInclude(yytext, yyleng, 1);}
^include{S}+[^" \t\r\n][\x20-\xff]*{S}*/\n    {l();handleInclude(yytext, yyleng, 0);}
{STRING}                     {l(); BEGIN(INITIAL);handleString(yytext, yyleng);return T_STRING;}
{CDATA}                      {l(); BEGIN(INITIAL);handleCData(yytext, yyleng);return T_STRING;}

<BEGINNING,REGEXPOK>{
{REGEXP}                     {c(); BEGIN(INITIAL);return handleregexp();} 
{HEXWITHSIGN}/{_}            {c(); BEGIN(INITIAL);return handlehex();}
{HEXFLOATWITHSIGN}/{_}       {c(); BEGIN(INITIAL);return handlehexfloat();}
{INTWITHSIGN}/{_}            {c(); BEGIN(INITIAL);return handleint();}
{FLOATWITHSIGN}/{_}          {c(); BEGIN(INITIAL);return handlefloat();}
}

<REGEXPOK>[\{]               {c(); BEGIN(REGEXPOK);return m(T_DICTSTART);}
[\{]                         {c(); BEGIN(INITIAL); return m('{');}

\xef\xbb\xbf                 {/* utf 8 bom */}
{S}                          {l();}

{HEXINT}/{_}                 {c(); BEGIN(INITIAL);return handlehex();}
{HEXFLOAT}/{_}               {c(); BEGIN(INITIAL);return handlehexfloat();}
{INT}/{_}                    {c(); BEGIN(INITIAL);return handleint();}
{FLOAT}/{_}                  {c(); BEGIN(INITIAL);return handlefloat();}

3rr0r                        {/* for debugging: generates a tokenizer-level error */
                              syntaxerror("3rr0r");}

{NAME}{S}*:{S}*for/{_}       {l();BEGIN(INITIAL);handleLabel(yytext, yyleng-3);return T_FOR;}
{NAME}{S}*:{S}*do/{_}        {l();BEGIN(INITIAL);handleLabel(yytext, yyleng-2);return T_DO;}
{NAME}{S}*:{S}*while/{_}     {l();BEGIN(INITIAL);handleLabel(yytext, yyleng-5);return T_WHILE;}
{NAME}{S}*:{S}*switch/{_}    {l();BEGIN(INITIAL);handleLabel(yytext, yyleng-6);return T_SWITCH;}
for                          {c();BEGIN(INITIAL);a3_lval.id="";return T_FOR;}
do                           {c();BEGIN(INITIAL);a3_lval.id="";return T_DO;}
while                        {c();BEGIN(INITIAL);a3_lval.id="";return T_WHILE;}
switch                       {c();BEGIN(INITIAL);a3_lval.id="";return T_SWITCH;}

[&][&]                       {c();BEGIN(REGEXPOK);return m(T_ANDAND);}
[|][|]                       {c();BEGIN(REGEXPOK);return m(T_OROR);}
[!][=]                       {c();BEGIN(REGEXPOK);return m(T_NE);}
[!][=][=]                    {c();BEGIN(REGEXPOK);return m(T_NEE);}
[=][=][=]                    {c();BEGIN(REGEXPOK);return m(T_EQEQEQ);}
[=][=]                       {c();BEGIN(REGEXPOK);return m(T_EQEQ);}
[>][=]                       {c();BEGIN(REGEXPOK);return m(T_GE);}
[<][=]                       {c();BEGIN(REGEXPOK);return m(T_LE);}
[-][-]                       {c();BEGIN(INITIAL);return m(T_MINUSMINUS);}
[+][+]                       {c();BEGIN(INITIAL);return m(T_PLUSPLUS);}
[+][=]                       {c();BEGIN(REGEXPOK);return m(T_PLUSBY);}
[\^][=]                      {c();BEGIN(REGEXPOK);return m(T_XORBY);}
[-][=]                       {c();BEGIN(REGEXPOK);return m(T_MINUSBY);}
[/][=]                       {c();BEGIN(REGEXPOK);return m(T_DIVBY);}
[%][=]                       {c();BEGIN(REGEXPOK);return m(T_MODBY);}
[*][=]                       {c();BEGIN(REGEXPOK);return m(T_MULBY);}
[|][=]                       {c();BEGIN(REGEXPOK);return m(T_ORBY);}
[>][>][=]                    {c();BEGIN(REGEXPOK);return m(T_SHRBY);}
[<][<][=]                    {c();BEGIN(REGEXPOK);return m(T_SHLBY);}
[>][>][>][=]                 {c();BEGIN(REGEXPOK);return m(T_USHRBY);}
[<][<]                       {c();BEGIN(REGEXPOK);return m(T_SHL);}
[>][>][>]                    {c();BEGIN(REGEXPOK);return m(T_USHR);}
[>][>]                       {c();BEGIN(REGEXPOK);return m(T_SHR);}
\.\.\.                       {c();BEGIN(REGEXPOK);return m(T_DOTDOTDOT);}
\.\.                         {c();BEGIN(REGEXPOK);return m(T_DOTDOT);}
\.                           {c();BEGIN(REGEXPOK);return m('.');}
::                           {c();BEGIN(REGEXPOK);return m(T_COLONCOLON);}
:                            {c();BEGIN(REGEXPOK);return m(':');}
instanceof                   {c();BEGIN(REGEXPOK);return m(KW_INSTANCEOF);}
implements                   {c();BEGIN(REGEXPOK);return m(KW_IMPLEMENTS);}
interface                    {c();BEGIN(INITIAL);return m(KW_INTERFACE);}
namespace                    {c();BEGIN(INITIAL);return m(KW_NAMESPACE);}
protected                    {c();BEGIN(INITIAL);return m(KW_PROTECTED);}
undefined                    {c();BEGIN(INITIAL);return m(KW_UNDEFINED);}
continue                     {c();BEGIN(INITIAL);return m(KW_CONTINUE);}
override                     {c();BEGIN(INITIAL);return m(KW_OVERRIDE);}
internal                     {c();BEGIN(INITIAL);return m(KW_INTERNAL);}
function                     {c();BEGIN(INITIAL);return m(KW_FUNCTION);}
finally                      {c();BEGIN(INITIAL);return m(KW_FINALLY);}
default                      {c();BEGIN(INITIAL);return m(KW_DEFAULT);}
package                      {c();BEGIN(INITIAL);return m(KW_PACKAGE);}
private                      {c();BEGIN(INITIAL);return m(KW_PRIVATE);}
dynamic                      {c();BEGIN(INITIAL);return m(KW_DYNAMIC);}
extends                      {c();BEGIN(INITIAL);return m(KW_EXTENDS);}
delete                       {c();BEGIN(REGEXPOK);return m(KW_DELETE);}
return                       {c();BEGIN(REGEXPOK);return m(KW_RETURN);}
public                       {c();BEGIN(INITIAL);return m(KW_PUBLIC);}
native                       {c();BEGIN(INITIAL);return m(KW_NATIVE);}
static                       {c();BEGIN(INITIAL);return m(KW_STATIC);}
import                       {c();BEGIN(REGEXPOK);return m(KW_IMPORT);}
typeof                       {c();BEGIN(REGEXPOK);return m(KW_TYPEOF);}
throw                        {c();BEGIN(REGEXPOK);return m(KW_THROW);}
class                        {c();BEGIN(INITIAL);return m(KW_CLASS);}
const                        {c();BEGIN(INITIAL);return m(KW_CONST);}
catch                        {c();BEGIN(INITIAL);return m(KW_CATCH);}
final                        {c();BEGIN(INITIAL);return m(KW_FINAL);}
false                        {c();BEGIN(INITIAL);return m(KW_FALSE);}
break                        {c();BEGIN(INITIAL);return m(KW_BREAK);}
super                        {c();BEGIN(INITIAL);return m(KW_SUPER);}
each                         {c();BEGIN(INITIAL);return m(KW_EACH);}
void                         {c();BEGIN(INITIAL);return m(KW_VOID);}
true                         {c();BEGIN(INITIAL);return m(KW_TRUE);}
null                         {c();BEGIN(INITIAL);return m(KW_NULL);}
else                         {c();BEGIN(INITIAL);return m(KW_ELSE);}
case                         {c();BEGIN(REGEXPOK);return m(KW_CASE);}
with                         {c();BEGIN(REGEXPOK);return m(KW_WITH);}
use                          {c();BEGIN(REGEXPOK);return m(KW_USE);}
new                          {c();BEGIN(REGEXPOK);return m(KW_NEW);}
get                          {c();BEGIN(INITIAL);return m(KW_GET);}
set                          {c();BEGIN(INITIAL);return m(KW_SET);}
var                          {c();BEGIN(INITIAL);return m(KW_VAR);}
try                          {c();BEGIN(INITIAL);return m(KW_TRY);}
is                           {c();BEGIN(REGEXPOK);return m(KW_IS) ;}
in                           {c();BEGIN(REGEXPOK);return m(KW_IN) ;}
if                           {c();BEGIN(INITIAL);return m(KW_IF) ;}
as                           {c();BEGIN(REGEXPOK);return m(KW_AS);}
$?{NAME}                       {c();BEGIN(INITIAL);return handleIdentifier();}

[\]\}*]                       {c();BEGIN(INITIAL);return m(yytext[0]);}
[+-\/^~@$!%&\(=\[|?:;,<>]   {c();BEGIN(REGEXPOK);return m(yytext[0]);}
[\)\]]                           {c();BEGIN(INITIAL);return m(yytext[0]);}

.		             {/* ERROR */
                              char c1=yytext[0];
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
<<EOF>>		             {l();
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
char*token2string(enum yytokentype nr, YYSTYPE v)
{
    if(nr==T_STRING) {
        char*s = malloc(v.str.len+10);
        strcpy(s, "<string>");
        memcpy(s+8, v.str.str, v.str.len);
        sprintf(s+8+v.str.len, " (%d bytes)", v.str.len);
        return s;
    }
    else if(nr==T_REGEXP) {
        char*s = malloc(strlen(v.regexp.pattern)+10);
        sprintf(s, "<regexp>%s", v.regexp.pattern);
        return s;
    }
    else if(nr==T_IDENTIFIER) {
        char*s = malloc(strlen(v.id)+10);
        sprintf(s, "<ID>%s", v.id);
        return s;
    }
    else if(nr==T_INT)     return "<int>";
    else if(nr==T_UINT)     return "<uint>";
    else if(nr==T_BYTE)     return "<byte>";
    else if(nr==T_FLOAT)     return "<float>";
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
    else if(nr==KW_SET)        return "set";
    else if(nr==KW_VAR)        return "var";
    else if(nr==KW_IS)         return "is";
    else if(nr==KW_AS)         return "as";
    else {
        sprintf(mbuf, "%d", nr);
        return mbuf;
    }
}

void initialize_scanner()
{
    BEGIN(BEGINNING);
}


