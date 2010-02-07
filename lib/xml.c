/* xml.c
   Lightweight and fast xml parser.

   Part of the swftools package.
   
   Copyright (c) 2010 Matthias Kramm <kramm@quiss.org> 
 
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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "xml.h"

/*
group: 0=data 1=whitespace 2='"' 3='<' 4='>' 5='&' 6=';' 7='?' 8='/' 9='=' 10='!' 11=EOF
*/

static int group[256] =
{
// 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
//                            \t \n       \r
   13, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
// 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f
//
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f
//    !  "  #  $  %  &  '  (  )  *  +  ,  -  .  /
   1,10, 2, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 8,
// 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f
// 0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 3, 9, 4, 7,
// 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f
// @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f
// P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,11, 0,12, 0, 0,
// 60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f
// `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f
// p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f
//
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 90 91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f
//
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af
//
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf
//
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf
//
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df
//
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef
//
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff
//
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const char*errors[]=
{
  0,
#define E1 -0x41
 /*E1*/"xml file must start with <?",
#define E2 -0x42
 /*E2*/"<,; or & not allowed inside tags",
#define E3 -0x43
 /*E3*/"syntax error",
#define E4 -0x44
 /*E4*/"internal error",
#define E5 -0x45
 /*E5*/"attribute definition without =\"",
};

static int new_state[][16]=
{        /*  dt ws  "  <  >  &  ;  ?  /  =  !  [  ]   - EOB*/
 /*  0 */{   E1, 0,E1, 1,E1,E1,E1,E1,E1,E1,E3,E1,E1,-63}, // .<
 /*  1 */{   E1,E1,E1,E1,E1,E1,E1, 9,E1,E1,E3,E1,E1,-63}, // <.?
 /*  2 */{   -3, 2,E3,E2,E2,E2,E2,E2,12,E2,16,E2,E2,-63}, // <.
 /*  3 */{   E3,E3,E3,E3,-1,E3,E3,E3,E3,E1,E3,E3,E3,-63}, // < /.>
 /*  4 */{   E3,E3,E3,E3,-2,E3,E3,E3,E3,E1,E1,E3,E3,-63}, // < .>
 /*  5 */{    5, 5, 5,-4, 5, 5, 5, 5, 5, 5, 5,E3,E3,-63}, // da.ta
 /*  6 */{    6,-7,E3,E2,-6,E2,E2,E3,-9,E3,E3,E3,E3,-63}, // <na.me
 /*  7 */{   -8, 7,E3,E2,-2,E2,E2, 7, 3,E3,E3,E3,E3,-63}, // <name .
 /*  8 */{    8,-12,E3,E2,E3,E2,E2,E3,E3,-10,E3,E3,E3,-63}, // att.r
 /*  9 */{    9, 7,E3,E3,E3,E3,E3,E3,E3,E3,E3,E3,E3,-63}, // <?x.ml
 /* 10 */{   E5,10,-11,E5,E5,E5,E5,E5,E5,E5,E3,E3,E3,-63},// attr=."
 /* 11 */{   11,11,-5 ,11,11,11,11,11,11,11,E3,E3,E3,-63},// attr="va.l
 /* 12 */{  -13,12,E3,E3,E3,E3,E3,E3,E3,E3,E3,E3,E3,-63}, // </ . >
 /* 13 */{  13,-14,E3,E3,-16,E3,E3,E3,E3,E3,E3,E3,E3,-63},// </ na.me>
 /* 14 */{   E3,14,E3,E3,-15,E3,E3,E3,E3,E3,E3,E3,E3,-63},// </ name. >
 /* 15 */{   E3,15,E3,E2,E3,E3,E3,E3,E3,10,E3,E3,E3,-63}, // attr .=
 /* 16 */{   E3,E3,E3,E2,E3,E3,E3,E3,E3,E3,E3,17,E3,-63}, // <!.[CDATA[ ]]>
 /* 17 */{   17,E3,E3,E3,E3,E3,E3,E3,E3,E3,E3,18,E3,-63}, // <![C.DATA[ ]]>
 /* 18 */{   18,18,18,18,18,18,18,18,18,18,18,18,19,-63}, // <![CDATA[ . ]]>
 /* 19 */{   18,18,18,18,-20,18,18,18,18,18,18,18,19,-63}, // <![CDATA[ ].]>
 /* 20 */{0,0,0,0,0,0,0,0,0,0,0,-63},
 /* 21 */{0,0,0,0,0,0,0,0,0,0,0,-63},
 /* 22 */{0,0,0,0,0,0,0,0,0,0,0,-63},
 /* 23 */{0,0,0,0,0,0,0,0,0,0,0,-63},
 /* 24 */{0,0,0,0,0,0,0,0,0,0,0,-63},
};

typedef struct _tag_stack {
    const char*name;
    struct _tag_stack*prev;
} tag_stack_t;

typedef struct _stringstate {
    char*current;
    int len;
    char*start;
    char*result;
} stringstate_t;

static void stringstate_start(stringstate_t*s, char*buffer, int pos)
{
    assert(!s->current);
    s->start = &buffer[pos];
    s->len = 0;
    s->result = 0;
}
static void stringstate_save(stringstate_t*s, char*buffer, int pos)
{
    if(!s->start) 
	return;
    int add = &buffer[pos] - s->start;
    if(!s->current) {
	assert(!s->len);
	s->current = malloc(add+1);
	memcpy(s->current, s->start, add);
    } else {
	s->current = realloc(s->current, s->len + add + 1);
	memcpy(s->current+s->len, s->start, add);
    }
    s->len += add;
    s->current[s->len] = 0;
    s->start = buffer;
}
static void stringstate_finish(stringstate_t*s, char*buffer, int pos)
{
    stringstate_save(s, buffer, pos);
    s->result = s->current;
    s->current = 0;
    s->start = 0;
}
static void stringstate_clear(stringstate_t*s)
{
    if(s->result) free(s->result);
    if(s->current) free(s->current);
    memset(s, 0, sizeof(stringstate_t));
}
static xmlattribute_t*attributes_reverse(xmlattribute_t*attr)
{
    xmlattribute_t*prev = 0;
    while(attr) {
	xmlattribute_t*next = attr->next;
	attr->next = prev;
	prev = attr;
	attr = next;
    }
    return prev;
}

static void attributes_free(xmlattribute_t*attributes)
{
    while(attributes) {
	xmlattribute_t*next = attributes->next;
	free((void*)attributes->name);
	free((void*)attributes->value);
	free(attributes);
	attributes = next;
    }
}

int xml_parse(reader_t*reader, xmlconsumer_t*out)
{
    char buffer[4097];
    int old = 0, state = 0;
    char first=1;
    tag_stack_t*stack = 0;

    stringstate_t tagname = {0,0,0,0};
    stringstate_t attr_name = {0,0,0,0};
    stringstate_t attr_value = {0,0,0,0};
    stringstate_t data = {0,0,0,0};
    xmlattribute_t*attributes = 0;

    while(1) {
	int num = reader->read(reader, buffer, 4096);
	if(!num) break;
	buffer[num]=0;
	int pos = 0;
	while(pos<num) {
	    /*printf("%c, state %d->%d\n", 
		    buffer[pos], state, new_state[state][group[buffer[pos]]]);*/

	    /* inner loop */
	    do {
		state = new_state[old=state][group[(unsigned char)buffer[pos++]]];
	    } while(state>=0);

	    switch(state) {
		tag_stack_t*st;
		xmlattribute_t*a;
		case -63: // end of buffer
		    if(pos!=num+1) {
			// we could backtrace, but the spec says this is indeed illegal
			fprintf(stderr, "error: xml contains \\0 chars\n");
			return 0;
		    }
		    // undo
		    pos = num;
		    state = old;
		break;
		case -1: // self closing tag
		    attributes = attributes_reverse(attributes);
		    out->start_tag(out, tagname.result, attributes);
		    out->end_tag(out, tagname.result);
		    stringstate_clear(&tagname);
		    attributes_free(attributes);attributes = 0;
		    stringstate_start(&data, buffer, pos);
		    state = 5;
		break;
		case -6: // after <tagname, at >
		    stringstate_finish(&tagname, buffer, pos-1);
		// fallthrough
		case -2: // <tag >.
		    st = malloc(sizeof(tag_stack_t));
		    st->name = tagname.result;
		    st->prev = stack;
		    stack = st;
		    attributes = attributes_reverse(attributes);
		    if(!first) out->start_tag(out, tagname.result, attributes);
		    attributes_free(attributes);attributes = 0;
		    stringstate_start(&data, buffer, pos);
		    state = 5;
		break;
		case -3: case -13: // after <, start of tag name
		    first=0;
		    stringstate_start(&tagname, buffer, pos-1);
		    state = state==-3?6:13;
		break;
		case -14: // after </, end of tag name, begin of white space
		    stringstate_finish(&tagname, buffer, pos-1);
		    state = 14;
		    break;
		case -16: // after </, at >, end of tag name 
		    stringstate_finish(&tagname, buffer, pos-1);
		// fallthrough
		case -15: // after </, at >
		    out->end_tag(out, tagname.result);
		    stringstate_clear(&tagname);
		    stringstate_start(&data, buffer, pos);
		    state = 5;
		break;
		case -4: // end of data
		    stringstate_finish(&data, buffer, pos-1);
		    if(!first) out->data(out, data.result, data.len);
		    stringstate_clear(&data);
		    state = 2;
		break;
		case -7: // after <, at whitespace, end of tag name
		    stringstate_finish(&tagname, buffer, pos-1);
		    state = 7;
		break;
		case -8: // inside tag, start of attribute name
		    stringstate_start(&attr_name, buffer, pos-1);
		    state = 8;
		break;
		case -9:
		    stringstate_finish(&tagname, buffer, pos-1);
		    state = 3;	
		break;
		case -12: // end of attribute name, at ws
		    stringstate_finish(&attr_name, buffer, pos-1);
		    state = 15;
		break;
		case -10: // end of attribute name, at =
		    stringstate_finish(&attr_name, buffer, pos-1);
		    state = 10;
		break;
		case -11: // start of attribute value
		    stringstate_start(&attr_value, buffer, pos);
		    state = 11;
		break;
		case -5: // end of attribute value
		    stringstate_finish(&attr_value, buffer, pos-1);
		    a = malloc(sizeof(xmlattribute_t));
		    a->name = attr_name.result;attr_name.result=0;
		    a->value = attr_value.result;attr_value.result=0;
		    a->next = attributes;
		    attributes = a;
		    state = 7;
		break;
		case -20:
		    state = 5;
		break;
		default:
		    if(-state&0x40) {
			fprintf(stderr, "%s (state %d, char '%c')\n", errors[(-state)&0x3f], old, buffer[pos-1]);
			return 0;
		    } else {
			fprintf(stderr, "internal error: no action %d\n", state);
		    }
		    return 0;
		break;
	    }
	}
	stringstate_save(&tagname, buffer, pos);
	stringstate_save(&attr_name, buffer, pos);
	stringstate_save(&attr_value, buffer, pos);
	stringstate_save(&data, buffer, pos);
    }

    /* note: any of these except data *has* to be empty for a well formed xml */
    stringstate_clear(&tagname);
    stringstate_clear(&attr_name);
    stringstate_clear(&attr_value);
    stringstate_clear(&data);

    while(stack) {
	tag_stack_t*next = stack->prev;
	free((void*)stack->name);
	free(stack);
	stack = next;
    }
    return 1;
}

#ifdef MAIN
void my_start_tag(xmlconsumer_t*c, char*name, xmlattribute_t*attr)
{
    printf("<%s", name);
    for(;attr;attr=attr->next) {
	printf(" %s=\"%s\"", attr->name, attr->value);
    }
    printf(">");
}
void my_data(xmlconsumer_t*c, char*data, int len)
{
    printf("%s", data);
}
void my_end_tag(xmlconsumer_t*c, char*name)
{
    printf("</%s>", name);
}
int main()
{
    xmlconsumer_t c = {my_start_tag, my_data, my_end_tag, 0};

    reader_t r;
    reader_init_filereader2(&r, "test.xml");
    xml_parse(&r, &c);
    r.dealloc(&r);
    printf("\n");
}
#endif
