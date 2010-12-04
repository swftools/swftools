%{

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../lib/q.h"
#include "parser.h"
#include "../lib/utf8.h"

//RVALUE	 {NUMBER}|{PERCENT}|{NAME}|\"{STRING}\"|{DIM}
//<a>.                {printf("<a>%s\n", yytext);}
// %x: exclusive, %s: inclusive
char*type_names[] = {"twip","number","command","string","assignment","identifier","label","end"};
static int line=1;
static int column=1;

mem_t strings;
mem_t tokens;

static void count(char*text, int len, int condition)
{
    int t;
    for(t=0;t<len;t++) {
	if(text[t]=='\n') {
	    line++;
	    column=1;
	} else {
	    column++;
	}
    }
}

static char*prefix = 0;

static void unescapeString(string_t * tmp)
{
    char *p, *p1;
    /* fixme - this routine expects the string to be
       null-terminated */

    for (p1=(char*)tmp->str; (p=strchr(p1, '\\')); p1 = p+1) 
    {
	int nr=2;
	int new=1;
	switch(p[1])
	{
	    case '\\': p[0] = '\\'; break;
	    case '"': p[0] = '"'; break;
	    case 'b': p[0] = '\b'; break;
	    case 'f': p[0] = '\f'; break;
	    case 'n': p[0] = '\n'; break;
	    case 'r': p[0] = '\r'; break;
	    case 't': p[0] = '\t'; break;
	    case 'x':  case 'u': {
		int max=4;
		int num=0;
		char*utf8;
		char bracket = 0;
		if(p[1] == 'u')
		    max = 6;
		if(p[2] == '{')  {
		    bracket = 1;nr++;max++;
		}
		while(strchr("0123456789abcdefABCDEF", p[nr]) && (bracket || nr < max)) {
		    num <<= 4;
		    if(p[nr]>='0' && p[nr]<='9') num |= p[nr] - '0';
		    if(p[nr]>='a' && p[nr]<='f') num |= p[nr] - 'a' + 10;
		    if(p[nr]>='A' && p[nr]<='F') num |= p[nr] - 'A' + 10;
		    nr++;
		}
		if(bracket && p[nr]=='}') {
		    bracket = 0;
		    nr++;
		}
		utf8 = getUTF8(num);
		new = strlen(utf8);
		memcpy(p, utf8, new); // do not copy the terminating zero
		break;
	    }
	    default:
		continue;
	}
	tmp->len -= (nr-new); 
	{
	    int t;
	    char*to=p+new,*from=p+nr;
	    while(*from) {
		*to = *from;
		to++;
		from++;
	    }
	}
    }
}

static void store(enum type_t type, int line, int column, char*text, int length)
{
    struct token_t token;
    string_t tmp;
    token.type = type;
    token.line = line;
    token.column = column;
    //printf("->%d(%s) %s\n", type, type_names[type], text);fflush(stdout);

    token.text_pos = 0;
    token.text = 0;
    switch(type) {
	case END:
	    string_set2(&tmp, "", 0);
	    token.text_pos = mem_putstring(&strings, tmp);
	break;
	case STRING:
	    string_set2(&tmp, text+1, length-2);
	    unescapeString(&tmp);
	    token.text_pos = mem_putstring(&strings, tmp);
	break;
	case TWIP: 
	case NUMBER: 
	case IDENTIFIER:
	    string_set2(&tmp, text, length);
	    if(prefix) {
		//strcat
		token.text_pos = mem_put(&strings, prefix, strlen(prefix));
		mem_putstring(&strings, tmp);
	    } else {
		token.text_pos = mem_putstring(&strings, tmp);
	    }
	    prefix = 0;
	break;
	case RAWDATA:
	    string_set2(&tmp, text+1/*:*/, length-5/*.end*/);
	    token.text_pos = mem_putstring(&strings, tmp);
	break;
	case COMMAND:
	    string_set2(&tmp, text+1, length-1);
	    token.text_pos = mem_putstring(&strings, tmp);
	break;
	case ASSIGNMENT: {
	    char*x = &text[length-1];
	    if(x[-1] == '-' || x[-1] == '+')
		x--;
	    do{x--;} while(*x==32 || *x==10 || *x==13 || *x=='\t');
	    x++; //first space
	    string_set2(&tmp, text, x-text);
	    token.text_pos = mem_putstring(&strings, tmp);
	    /*char*y,*x = strchr(text, '=');
	    if(!x) exit(1);
	    y=x;
	    do{y--;} while(*y==32 || *y==10 || *y==13 || *y=='\t');
	    do{x++;} while(*x==32 || *x==10 || *x==13 || *x=='\t');
	    token.text1 = (char*)put(&strings, text, y-text + 1, 1);
	    token.text2 = (char*)put(&strings, x, length-(x-text), 1);*/
	} break;
    }

    mem_put(&tokens, &token, sizeof(struct token_t));
    prefix = 0;
}

#define MAX_INCLUDE_DEPTH 16
static YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
static int line_stack[MAX_INCLUDE_DEPTH];
static int column_stack[MAX_INCLUDE_DEPTH];
static int include_stack_ptr = 0;

static void handleInclude(char*text, int len)
{
    text+=9;len-=9;
    while(len >=1 && (text[0] == ' ' || text[0] == '\t')) {
	text++;len--;
    }
    while(len >= 1 && 
          (text[len-1] == ' ' || 
           text[len-1] == '\r' || 
           text[len-1] == '\n')) {
	len--;
    }
    if(len >= 2 && text[0] == '"' && text[len-1] == '"') {
	text++; len-=2;
    }
    text[len] = 0;
    if(include_stack_ptr >= MAX_INCLUDE_DEPTH) {
    	fprintf( stderr, "Includes nested too deeply" );
    	exit( 1 );
    }
    include_stack[include_stack_ptr] = YY_CURRENT_BUFFER;
    line_stack[include_stack_ptr] = line;
    column_stack[include_stack_ptr] = column;
    include_stack_ptr++;
    yyin = fopen(text, "rb");
    if (!yyin) {
	fprintf(stderr, "Couldn't open %s\n", text);
	exit(1);
    }
    yy_switch_to_buffer(yy_create_buffer( yyin, YY_BUF_SIZE ) );

#ifdef INITIAL
    BEGIN(INITIAL);
#else
    // best guess
    BEGIN(0);
#endif
}

#define c() {count(yytext, yyleng, YY_START);}
#define s(type) {store(type, line, column, yytext, yyleng);}
%}

%s R
%x BINARY

NAME	 [a-zA-Z_./](-*[a-zA-Z0-9_./])*
TWIP	 (-?[0-9]+(\.([0-9]([05])?)?)?)
NUMBER	 -?[0-9]+(\.[0-9]*)?
PERCENT	 {NUMBER}%
STRING   (\\.|[^\\"\n])*
S	 [ \n\r\t]
RVALUE	 \"{STRING}\"|([^ \n\r\t]+)

%%

<BINARY>\] {c();BEGIN(0);}
<BINARY>.  {c();}
<BINARY>\n {c();}
{TWIP}/[ \n\r\t]            {s(TWIP);c();BEGIN(0);}
{NUMBER}/[ \n\r\t]	    {s(NUMBER);c();BEGIN(0);}
^#[^\n]*\n                  {c();}
[ \t\r]#[^\n]*\n            {c();}
\"{STRING}\"	            {s(STRING);c();BEGIN(0);}
\"{STRING}$	            {c();printf("unterminated string in line %d: %s\n", line, yytext);exit(1);yyterminate();}
{NAME}{S}*\+=		    {s(ASSIGNMENT);prefix="<plus>";c();BEGIN(R);}
{NAME}{S}*-=		    {s(ASSIGNMENT);prefix="<minus>";c();BEGIN(R);}
{NAME}{S}*=		    {s(ASSIGNMENT);c();BEGIN(R);}
<R>{ /* values which appear only on the right-hand side of assignments, like: x=50% */
    [^ :\n\t\r]*		    {s(IDENTIFIER);c();BEGIN(0);}
}
\.include{S}.*\n		    {handleInclude(yytext, yyleng);}
\.{NAME}	            {s(COMMAND);c();}
:([^.]|\.[^e]|\.e[^n]|\.en[^d]|\.end[^ \n\r\t]|[ \n\r\t])*\.end	    {s(RAWDATA);c();}
{NAME}                      {s(IDENTIFIER);c();}
"["		            {c();BEGIN(BINARY);}
{S} 		            {c();}
.		            {char c,c1=yytext[0];
		             printf("Syntax error in line %d, %d: %s", line, column, yytext);
		             while(1) {
		                 c=input();
		                 if(c=='\n' || c==EOF) 
		                     break;
		                printf("%c", c);
		             }
			     if(c1>='0' && c1<='9')
				 printf(" (identifiers must not start with a digit)");
		             printf("\n");
			     exit(1);
		             yyterminate();
		            }
<<EOF>>		            {c();
			     if ( --include_stack_ptr < 0 ) {
				s(END);
				yyterminate();
			     } else {
				 yy_delete_buffer( YY_CURRENT_BUFFER );
				 yy_switch_to_buffer(
				      include_stack[include_stack_ptr] );
				 column = column_stack[include_stack_ptr];
				 line = line_stack[include_stack_ptr];
			     }
			    }

%%

int yywrap()
{
    return 1;
}

void freeTokens(struct token_t*file)
{
    mem_clear(&strings);
    mem_clear(&tokens);
}

struct token_t* generateTokens(char*filename)
{
    FILE*fi;
    int t;
    struct token_t*result;
    int num;

    if(!filename)
	return 0;

    if(!strcmp(filename,"-"))
	fi = stdin;
    else
	fi = fopen(filename, "rb");

    if(!fi) {
	printf("Couldn't find file %s\n", filename);
	return 0;
    }
    yyin = fi;

    mem_init(&strings);
    mem_init(&tokens);
    mem_put(&strings, &t, 1); //hack- make all valid strings start at position >0

    line=1;
    column=1;

    yylex();
#ifdef YY_CURRENT_BUFFER
    // some newer flex versions require it like this:
    yy_delete_buffer(YY_CURRENT_BUFFER);
#else
    yy_delete_buffer(yy_current_buffer);
#endif

    result = (struct token_t*)tokens.buffer;
    num = tokens.pos/sizeof(struct token_t);

    for(t=0;t<tokens.pos/sizeof(struct token_t);t++) {
	if(result[t].text_pos) {
	    result[t].text = &strings.buffer[result[t].text_pos];
	}
    }

    if(fi!=stdin)
	fclose(fi);
    return result;
}

