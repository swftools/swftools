/* swfc.c
   Compiles swf code (.sc) files into .swf files.

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <errno.h>
#include <math.h>
#include "../config.h"
#include "../lib/rfxswf.h"
#include "../lib/drawer.h"
#include "../lib/log.h"
#include "../lib/args.h"
#include "../lib/q.h"
#include "parser.h"
#include "wav.h"

//#define DEBUG

static char * filename = 0;
static char * outputname = "output.swf";
static int verbose = 2;
static int optimize = 0;
static int override_outputname = 0;

static struct options_t options[] = {
{"h", "help"},
{"V", "version"},
{"v", "verbose"},
{"o", "output"},
{"O", "optimize"},
{0,0}
};
    
int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfc - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    }
    else if(!strcmp(name, "o")) {
	outputname = val;
	override_outputname = 1;
	return 1;
    }
    else if(!strcmp(name, "O")) {
	optimize = 1;
	return 0;
    }
    else if(!strcmp(name, "v")) {
	verbose ++;
	return 0;
    }
    else {
        printf("Unknown option: -%s\n", name);
	exit(1);
    }
    return 0;
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char *name)
{
    printf("\n");
    printf("Usage: %s [-o file.swf] file.sc\n", name);
    printf("\n");
    printf("-h , --help                    Print short help message and exit\n");
    printf("-V , --version                 Print version info and exit\n");
    printf("-v , --verbose                 Increase verbosity. \n");
    printf("-o , --output <filename>       Set output file to <filename>.\n");
    printf("\n");
}
int args_callback_command(char*name,char*val)
{
    if(filename) {
        fprintf(stderr, "Only one file allowed. You supplied at least two. (%s and %s)\n",
                 filename, name);
    }
    filename = name;
    return 0;
}

static struct token_t* file;

static int pos;
static char*text;
static int textlen;
static int type;
static int line;
static int column;

static void syntaxerror(char*format, ...)
{
    char buf[1024];
    va_list arglist;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    printf("\"%s\", line %d column %d: error- %s\n", filename, line, column, buf);
    exit(1);
}

static void warning(char*format, ...)
{
    char buf[1024];
    va_list arglist;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    printf("\"%s\", line %d column %d: warning- %s\n", filename, line, column, buf);
}

static void readToken()
{
    type = file[pos].type;
    if(type == END) {
	syntaxerror("unexpected end of file");
    }
    text = file[pos].text;
    textlen = strlen(text);
    line = file[pos].line;
    column = file[pos].column;
    pos++;
    //printf("---> %d(%s) %s\n", type, type_names[type], text);
}

static void pushBack()
{
    int p;
    if(!pos) syntaxerror("internal error 3");
    pos--;
    p = pos;
    if(p) p--;
    text = file[p].text;
    textlen = strlen(text);
    type = file[p].type;
    line = file[p].line;
    column = file[p].column;
}

static int noMoreTokens()
{
    if(file[pos].type == END)
	return 1;
    return 0;
}

// ------------------------------ swf routines ----------------------------
struct _character;
static struct level
{
   int type; //0=swf, 1=sprite, 2=clip, 3=button

   /* for swf (0): */
   SWF*swf;
   char*filename; 

   /* for sprites (1): */
   TAG*tag;
   U16 id;
   char*name;
   U16 olddepth;
   int oldframe;
   dictionary_t oldinstances;
   SRECT oldrect;
   TAG* cut;

} stack[256];
static int stackpos = 0;

static dictionary_t characters;
static dictionary_t images;
static dictionary_t outlines;
static dictionary_t gradients;
static char idmap[65536];
static TAG*tag = 0; //current tag

static int id; //current character id
static int currentframe; //current frame in current level
static SRECT currentrect; //current bounding box in current level
static U16 currentdepth;
static dictionary_t instances;
static dictionary_t fonts;
static dictionary_t sounds;

typedef struct _parameters {
    int x,y; 
    float scalex, scaley; 
    CXFORM cxform;
    float rotate;
    float shear;
    SPOINT pivot;
    SPOINT pin;
} parameters_t;

typedef struct _character {
    TAG*definingTag;
    U16 id;
    SRECT size;
} character_t;

typedef struct _instance {
    character_t*character;
    U16 depth;
    parameters_t parameters;
    TAG* lastTag; //last tag which set the object
    U16 lastFrame; //frame lastTag is in
} instance_t;

typedef struct _outline {
    SHAPE* shape;
    SRECT bbox;
} outline_t;

typedef struct _gradient {
    GRADIENT gradient;
    char radial;
    int rotate;
} gradient_t;

static void character_init(character_t*c)
{
    memset(c, 0, sizeof(character_t));
}
static character_t* character_new()
{
    character_t*c;
    c = (character_t*)malloc(sizeof(character_t));
    character_init(c);
    return c;
}
static void instance_init(instance_t*i)
{
    memset(i, 0, sizeof(instance_t));
}
static instance_t* instance_new()
{
    instance_t*c;
    c = (instance_t*)malloc(sizeof(instance_t));
    instance_init(c);
    return c;
}

static void incrementid()
{
    while(idmap[++id]) {
	if(id==65535)
	    syntaxerror("Out of character ids.");
    }
    idmap[id] = 1;
}

static void s_addcharacter(char*name, U16 id, TAG*ctag, SRECT r)
{
    character_t* c = character_new();
    
    c->definingTag = ctag;
    c->id = id;
    c->size = r;
    if(dictionary_lookup(&characters, name))
	syntaxerror("character %s defined twice", name);
    dictionary_put2(&characters, name, c);

    tag = swf_InsertTag(tag, ST_NAMECHARACTER);
    swf_SetU16(tag, id);
    swf_SetString(tag, name);
    tag = swf_InsertTag(tag, ST_EXPORTASSETS);
    swf_SetU16(tag, 1);
    swf_SetU16(tag, id);
    swf_SetString(tag, name);
}
static void s_addimage(char*name, U16 id, TAG*ctag, SRECT r)
{
    character_t* c = character_new();
    c->definingTag = ctag;
    c->id = id;
    c->size = r;

    if(dictionary_lookup(&images, name))
	syntaxerror("image %s defined twice", name);
    dictionary_put2(&images, name, c);
}
static instance_t* s_addinstance(char*name, character_t*c, U16 depth)
{
    instance_t* i = instance_new();
    i->character = c;
    i->depth = depth;
    //swf_GetMatrix(0, &i->matrix);
    if(dictionary_lookup(&instances, name))
	syntaxerror("object %s defined twice", name);
    dictionary_put2(&instances, name, i);
    return i;
}

static void parameters_set(parameters_t*p, int x,int y, float scalex, float scaley, float rotate, float shear, SPOINT pivot, SPOINT pin, CXFORM cxform)
{
    p->x = x; p->y = y; 
    p->scalex = scalex; p->scaley = scaley;
    p->pin    = pin; p->pivot = pivot;
    p->rotate = rotate; p->cxform = cxform;
    p->shear = shear;
}

static void parameters_clear(parameters_t*p)
{
    p->x = 0; p->y = 0; 
    p->scalex = 1.0; p->scaley = 1.0;
    p->pin.x = 0;  //1??
    p->pin.y = 0;
    p->pivot.x = 0; p->pivot.y = 0;
    p->rotate = 0; 
    p->shear = 0; 
    swf_GetCXForm(0, &p->cxform, 1);
}

static void makeMatrix(MATRIX*m, parameters_t*p)
{
    SPOINT h;
    float sx,r1,r0,sy;

    /*	      /sx r1\ /x\ 
     *	      \r0 sy/ \y/
     */

    sx =  p->scalex*cos(p->rotate/360*2*3.14159265358979);
    r1 = -p->scalex*sin(p->rotate/360*2*3.14159265358979)+sx*p->shear;
    r0 =  p->scaley*sin(p->rotate/360*2*3.14159265358979);
    sy =  p->scaley*cos(p->rotate/360*2*3.14159265358979)+r0*p->shear;

    m->sx = (int)(sx*65536+0.5);
    m->r1 = (int)(r1*65536+0.5);
    m->r0 = (int)(r0*65536+0.5);
    m->sy = (int)(sy*65536+0.5);

    m->tx = m->ty = 0;
   
    h = swf_TurnPoint(p->pin, m);
    m->tx = p->x - h.x;
    m->ty = p->y - h.y;
}

static MATRIX s_instancepos(SRECT rect, parameters_t*p)
{
    MATRIX m;
    SRECT r;
    makeMatrix(&m, p);
    r = swf_TurnRect(rect, &m);
    if(currentrect.xmin == 0 && currentrect.ymin == 0 && 
       currentrect.xmax == 0 && currentrect.ymax == 0)
	currentrect = r;
    else
	swf_ExpandRect2(&currentrect, &r);
    return m;
}

void s_swf(char*name, SRECT r, int version, int fps, int compress, RGBA background)
{
    SWF*swf = (SWF*)malloc(sizeof(SWF));

    if(stackpos)
	syntaxerror(".swf blocks can't be nested");

    memset(swf, 0, sizeof(swf));
    swf->fileVersion = version;
    swf->movieSize = r;
    swf->frameRate = fps;
    swf->firstTag = tag = swf_InsertTag(0, ST_SETBACKGROUNDCOLOR);
    swf->compressed = compress;
    swf_SetRGB(tag,&background);
    
    if(stackpos==sizeof(stack)/sizeof(stack[0]))
	syntaxerror("too many levels of recursion");
    
    dictionary_init(&characters);
    dictionary_init(&images);
    dictionary_init(&outlines);
    dictionary_init(&gradients);
    dictionary_init(&instances);
    dictionary_init(&fonts);
    dictionary_init(&sounds);

    memset(&stack[stackpos], 0, sizeof(stack[0]));
    stack[stackpos].type = 0;
    stack[stackpos].filename = strdup(name);
    stack[stackpos].swf = swf;
    stack[stackpos].oldframe = -1;
    stackpos++;
    id = 0;

    currentframe = 0;
    memset(&currentrect, 0, sizeof(currentrect));
    currentdepth = 1;
    
    memset(idmap, 0, sizeof(idmap));
    incrementid();
}

void s_sprite(char*name)
{
    tag = swf_InsertTag(tag, ST_DEFINESPRITE);
    swf_SetU16(tag, id); //id
    swf_SetU16(tag, 0); //frames

    memset(&stack[stackpos], 0, sizeof(stack[0]));
    stack[stackpos].type = 1;
    stack[stackpos].oldframe = currentframe;
    stack[stackpos].olddepth = currentdepth;
    stack[stackpos].oldrect = currentrect;
    stack[stackpos].oldinstances = instances;
    stack[stackpos].tag = tag;
    stack[stackpos].id = id;
    stack[stackpos].name = strdup(name);
   
    /* FIXME: those four fields should be bundled together */
    dictionary_init(&instances);
    currentframe = 0;
    currentdepth = 1;
    memset(&currentrect, 0, sizeof(currentrect));

    stackpos++;
    incrementid();
}

typedef struct _buttonrecord
{
    U16 id;
    MATRIX matrix;
    CXFORM cxform;
    char set;
} buttonrecord_t;

typedef struct _button
{
    int endofshapes;
    int nr_actions;
    buttonrecord_t records[4];
} button_t;

static button_t mybutton;

void s_button(char*name)
{
    tag = swf_InsertTag(tag, ST_DEFINEBUTTON2);
    swf_SetU16(tag, id); //id
    swf_ButtonSetFlags(tag, 0); //menu=no

    memset(&mybutton, 0, sizeof(mybutton));

    memset(&stack[stackpos], 0, sizeof(stack[0]));
    stack[stackpos].type = 3;
    stack[stackpos].tag = tag;
    stack[stackpos].id = id;
    stack[stackpos].name = strdup(name);
    stack[stackpos].oldrect = currentrect;
    memset(&currentrect, 0, sizeof(currentrect));

    stackpos++;
    incrementid();
}
void s_buttonput(char*character, char*as, parameters_t p)
{
    character_t* c = dictionary_lookup(&characters, character);
    MATRIX m;
    int flags = 0;
    char*o = as,*s = as;
    buttonrecord_t r;
    if(!stackpos || (stack[stackpos-1].type != 3))  {
	syntaxerror(".show may only appear in .button");
    }
    if(!c) {
	syntaxerror("character %s not known (in .shape %s)", character, character);
    }
    if(mybutton.endofshapes) {
	syntaxerror("a .do may not precede a .show", character, character);
    }
   
    m = s_instancepos(c->size, &p);

    r.id = c->id;
    r.matrix = m;
    r.cxform = p.cxform;
    r.set = 1;

    while(1) {
	if(*s==',' || *s==0) {
	    if(!strncmp(o,"idle",s-o)) {mybutton.records[0]=r;o=s+1;}
	    else if(!strncmp(o,"shape",s-o)) {mybutton.records[0]=r;o=s+1;}
	    else if(!strncmp(o,"hover",s-o)) {mybutton.records[1]=r;o=s+1;}
	    else if(!strncmp(o,"pressed",s-o)) {mybutton.records[2]=r;o=s+1;}
	    else if(!strncmp(o,"area",s-o)) {mybutton.records[3]=r;o=s+1;}
	    else syntaxerror("unknown \"as\" argument: \"%s\"", strdup_n(o,s-o));
	}
	if(!*s)
	    break;
	s++;
    }
}
static void setbuttonrecords(TAG*tag)
{
    int flags[] = {BS_UP,BS_OVER,BS_DOWN,BS_HIT};
    if(!mybutton.endofshapes) {
	int t;
		
	if(!mybutton.records[3].set) {
	    memcpy(&mybutton.records[3], &mybutton.records[0], sizeof(buttonrecord_t));
	}

	for(t=0;t<4;t++) {
	    if(mybutton.records[t].set) {
		swf_ButtonSetRecord(tag,flags[t],mybutton.records[t].id,1,&mybutton.records[t].matrix,&mybutton.records[t].cxform);
	    }
	}
	swf_SetU8(tag,0); // end of button records
	mybutton.endofshapes = 1;
    }
}

void s_buttonaction(int flags, char*action)
{
    ActionTAG* a = 0;
    if(flags==0) {
	return;
    }
    setbuttonrecords(stack[stackpos-1].tag);
    
    a = swf_ActionCompile(text, stack[0].swf->fileVersion);
    if(!a) {
	syntaxerror("Couldn't compile ActionScript");
    }

    swf_ButtonSetCondition(stack[stackpos-1].tag, flags);
    swf_ActionSet(stack[stackpos-1].tag, a);
    mybutton.nr_actions++;

    swf_ActionFree(a);
}

static void setactionend(TAG*tag)
{
    if(!mybutton.nr_actions) {
	/* no actions means we didn't have an actionoffset,
	   which means we can't signal the end of the
	   buttonaction records, so, *sigh*, we have
	   to insert a dummy record */
	swf_SetU16(tag, 0); //offset
	swf_SetU16(tag, 0); //condition
	swf_SetU8(tag, 0); //action
    }
}

static void s_endButton()
{
    SRECT r;
    setbuttonrecords(stack[stackpos-1].tag);
    setactionend(stack[stackpos-1].tag);
    stackpos--;
      
    swf_ButtonPostProcess(stack[stackpos].tag, mybutton.nr_actions);

    r = currentrect;

    tag = stack[stackpos].tag;
    currentrect = stack[stackpos].oldrect;

    s_addcharacter(stack[stackpos].name, stack[stackpos].id, stack[stackpos].tag, r);
    free(stack[stackpos].name);
}

TAG* removeFromTo(TAG*from, TAG*to)
{
    TAG*save = from->prev;
    while(from!=to) {
	TAG*next = from->next;
	swf_DeleteTag(from);
	from = next;
    }
    save->next = 0;
    return save;
}

static void s_endSprite()
{
    SRECT r = currentrect;
    
    if(stack[stackpos].cut)
	tag = removeFromTo(stack[stackpos].cut, tag);

    stackpos--;
   
    /* TODO: before clearing, prepend "<spritename>." to names and
             copy into old instances dict */
    dictionary_clear(&instances);

    currentframe = stack[stackpos].oldframe;
    currentrect = stack[stackpos].oldrect;
    currentdepth = stack[stackpos].olddepth;
    instances = stack[stackpos].oldinstances;

    tag = swf_InsertTag(tag, ST_END);

    tag = stack[stackpos].tag;
    swf_FoldSprite(tag);
    if(tag->next != 0)
	syntaxerror("internal error(7)");

    s_addcharacter(stack[stackpos].name, stack[stackpos].id, stack[stackpos].tag, r);
    free(stack[stackpos].name);
}

static void s_endSWF()
{
    int fi;
    SWF* swf;
    char*filename;
    
    if(stack[stackpos].cut)
	tag = removeFromTo(stack[stackpos].cut, tag);

    stackpos--;

    swf = stack[stackpos].swf;
    filename = stack[stackpos].filename;
  
    //if(tag->prev && tag->prev->id != ST_SHOWFRAME)
    //    tag = swf_InsertTag(tag, ST_SHOWFRAME);
    tag = swf_InsertTag(tag, ST_SHOWFRAME);

    tag = swf_InsertTag(tag, ST_END);

    swf_OptimizeTagOrder(swf);
   
    if(optimize) {
	swf_Optimize(swf);
    }
    
    if(!(swf->movieSize.xmax-swf->movieSize.xmin) || !(swf->movieSize.ymax-swf->movieSize.ymin)) {
	swf->movieSize = currentrect; /* "autocrop" */
    }
    
    if(!(swf->movieSize.xmax-swf->movieSize.xmin) || !(swf->movieSize.ymax-swf->movieSize.ymin)) {
	swf->movieSize.xmax += 20; /* 1 by 1 pixels */
	swf->movieSize.ymax += 20;
    }

    fi = open(filename, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
    if(fi<0) {
	syntaxerror("couldn't create output file %s", filename);
    }
    if(swf->compressed) 
	{if(swf_WriteSWC(fi, swf)<0) syntaxerror("WriteSWC() failed.\n");}
    else
	{if(swf_WriteSWF(fi, swf)<0) syntaxerror("WriteSWF() failed.\n");}

    close(fi);
    
    dictionary_clear(&instances);
    dictionary_clear(&characters);
    dictionary_clear(&images);
    dictionary_clear(&outlines);
    dictionary_clear(&gradients);
    dictionary_clear(&fonts);
    dictionary_clear(&sounds);

    swf_FreeTags(swf);
    free(swf);
    free(filename);
}

void s_close()
{
    if(stackpos) {
	if(stack[stackpos-1].type == 0)
	    syntaxerror("End of file encountered in .flash block");
	if(stack[stackpos-1].type == 1)
	    syntaxerror("End of file encountered in .sprite block");
	if(stack[stackpos-1].type == 2)
	    syntaxerror("End of file encountered in .clip block");
    }
}

int s_getframe()
{
    return currentframe+1;
}

void s_frame(int nr, int cut, char*name)
{
    int t;
    TAG*now = tag;

    if(nr<1) 
	syntaxerror("Illegal frame number");
    nr--; // internally, frame 1 is frame 0

    for(t=currentframe;t<nr;t++) {
	tag = swf_InsertTag(tag, ST_SHOWFRAME);
	if(t==nr-1 && name && *name) {
	    tag = swf_InsertTag(tag, ST_FRAMELABEL);
	    swf_SetString(tag, name);
	}
    }
    if(nr == 0 && currentframe == 0 && name) {
        tag = swf_InsertTag(tag, ST_FRAMELABEL);
        swf_SetString(tag, name);
    }

    if(cut) {
	if(now == tag) {
	    syntaxerror("Can't cut, frame empty");
	}
	stack[stackpos].cut = tag;
    }

    currentframe = nr;
}

int parseColor2(char*str, RGBA*color);

int addFillStyle(SHAPE*s, SRECT*r, char*texture)
{
    RGBA color;
    character_t*image;
    gradient_t*gradient;
    if(texture[0] == '#') {
	parseColor2(texture, &color);
	return swf_ShapeAddSolidFillStyle(s, &color);
    } else if((image = dictionary_lookup(&images, texture))) {
	MATRIX m;
	swf_GetMatrix(0, &m);
	m.sx = 65536.0*20.0*(r->xmax - r->xmin)/image->size.xmax;
	m.sy = 65536.0*20.0*(r->ymax - r->ymin)/image->size.ymax;
	m.tx = r->xmin;
	m.ty = r->ymin;
	return swf_ShapeAddBitmapFillStyle(s, &m, image->id, 0);
    } /*else if ((texture = dictionary_lookup(&textures, texture))) {
    } */ else if ((gradient = dictionary_lookup(&gradients, texture))) {
	SRECT r2;
	MATRIX rot,m;
	double ccos,csin;
	swf_GetMatrix(0, &rot);
	ccos = cos(-gradient->rotate*2*3.14159265358979/360);
	csin = sin(-gradient->rotate*2*3.14159265358979/360);
	rot.sx =  ccos*65536;
	rot.r1 = -csin*65536;
	rot.r0 =  csin*65536;
	rot.sy =  ccos*65536;
	r2 = swf_TurnRect(*r, &rot);
	swf_GetMatrix(0, &m);
	m.sx =  (r2.xmax - r2.xmin)*2*ccos;
	m.r1 = -(r2.xmax - r2.xmin)*2*csin;
	m.r0 =  (r2.ymax - r2.ymin)*2*csin;
	m.sy =  (r2.ymax - r2.ymin)*2*ccos;
	m.tx = r->xmin + (r->xmax - r->xmin)/2;
	m.ty = r->ymin + (r->ymax - r->ymin)/2;
	return swf_ShapeAddGradientFillStyle(s, &m, &gradient->gradient, gradient->radial);
    }  else if (parseColor2(texture, &color)) {
	return swf_ShapeAddSolidFillStyle(s, &color);
    } else {
	syntaxerror("not a color/fillstyle: %s", texture);
	return 0;
    }
}
	
RGBA black={r:0,g:0,b:0,a:0};
void s_box(char*name, int width, int height, RGBA color, int linewidth, char*texture)
{
    SRECT r,r2;
    SHAPE* s;
    int ls1=0,fs1=0;
    r2.xmin = 0;
    r2.ymin = 0;
    r2.xmax = width;
    r2.ymax = height;
    tag = swf_InsertTag(tag, ST_DEFINESHAPE3);
    swf_ShapeNew(&s);
    if(linewidth)
        ls1 = swf_ShapeAddLineStyle(s,linewidth,&color);
    if(texture)
	fs1 = addFillStyle(s, &r2, texture);

    swf_SetU16(tag,id);
    r.xmin = r2.xmin-linewidth-linewidth/2;
    r.ymin = r2.ymin-linewidth-linewidth/2;
    r.xmax = r2.xmax+linewidth+linewidth/2;
    r.ymax = r2.ymax+linewidth+linewidth/2;
    swf_SetRect(tag,&r);
    swf_SetShapeHeader(tag,s);
    swf_ShapeSetAll(tag,s,0,0,ls1,fs1,0);
    swf_ShapeSetLine(tag,s,width,0);
    swf_ShapeSetLine(tag,s,0,height);
    swf_ShapeSetLine(tag,s,-width,0);
    swf_ShapeSetLine(tag,s,0,-height);
    swf_ShapeSetEnd(tag);
    swf_ShapeFree(s);
   
    s_addcharacter(name, id, tag, r);
    incrementid();
}

void swf_RecodeShapeData(U8*data, int bitlen, int in_bits_fill, int in_bits_line, 
                         U8**destdata, U32*destbitlen, int out_bits_fill, int out_bits_line)
{
    SHAPE2 s2;
    SHAPE s;
    SHAPELINE*line;
    memset(&s2, 0, sizeof(s2));
    s2.lines = swf_ParseShapeData(data, bitlen, in_bits_fill, in_bits_line);
    s2.numfillstyles = out_bits_fill?1<<(out_bits_fill-1):0;
    s2.numlinestyles = out_bits_line?1<<(out_bits_line-1):0;
    s2.fillstyles = rfx_calloc(sizeof(FILLSTYLE)*s2.numfillstyles);
    s2.linestyles = rfx_calloc(sizeof(LINESTYLE)*s2.numlinestyles);

    line = s2.lines;
    while(line) {
        if(line->fillstyle0 > s2.numfillstyles) line->fillstyle0 = 0;
        if(line->fillstyle1 > s2.numfillstyles) line->fillstyle1 = 0;
        if(line->linestyle > s2.numlinestyles) line->linestyle = 0;
        line = line->next;
    }

    swf_Shape2ToShape(&s2,&s);

    free(s2.fillstyles);
    free(s2.linestyles);
    free(s.fillstyle.data);
    free(s.linestyle.data);
    *destdata = s.data;
    *destbitlen = s.bitlen;
}

void s_filled(char*name, char*outlinename, RGBA color, int linewidth, char*texture)
{
    SRECT rect,r2;
    SHAPE* s;
    int ls1,fs1=0;
    outline_t* outline;
    outline = dictionary_lookup(&outlines, outlinename);
    if(!outline) {
	syntaxerror("outline %s not defined", outlinename);
    }
    r2 = outline->bbox;

    tag = swf_InsertTag(tag, ST_DEFINESHAPE3);
    swf_ShapeNew(&s);
    if(linewidth)
        ls1 = swf_ShapeAddLineStyle(s,linewidth,&color);
    if(texture)
	fs1 = addFillStyle(s, &r2, texture);
    
    swf_SetU16(tag,id);
    rect.xmin = r2.xmin-linewidth-linewidth/2;
    rect.ymin = r2.ymin-linewidth-linewidth/2;
    rect.xmax = r2.xmax+linewidth+linewidth/2;
    rect.ymax = r2.ymax+linewidth+linewidth/2;

    swf_SetRect(tag,&rect);
    swf_SetShapeStyles(tag, s);
    swf_ShapeCountBits(s,0,0);
    swf_RecodeShapeData(outline->shape->data, outline->shape->bitlen, 1,            1, 
                        &s->data,             &s->bitlen,             s->bits.fill, s->bits.line);
    swf_SetShapeBits(tag, s);
    swf_SetBlock(tag, s->data, (s->bitlen+7)/8);
    swf_ShapeFree(s);

    s_addcharacter(name, id, tag, rect);
    incrementid();
}

void s_circle(char*name, int r, RGBA color, int linewidth, char*texture)
{
    SRECT rect,r2;
    SHAPE* s;
    int ls1=0,fs1=0;
    r2.xmin = r2.ymin = 0;
    r2.xmax = 2*r;
    r2.ymax = 2*r;

    tag = swf_InsertTag(tag, ST_DEFINESHAPE3);
    swf_ShapeNew(&s);
    if(linewidth)
        ls1 = swf_ShapeAddLineStyle(s,linewidth,&color);
    if(texture)
	fs1 = addFillStyle(s, &r2, texture);
    swf_SetU16(tag,id);
    rect.xmin = r2.xmin-linewidth-linewidth/2;
    rect.ymin = r2.ymin-linewidth-linewidth/2;
    rect.xmax = r2.xmax+linewidth+linewidth/2;
    rect.ymax = r2.ymax+linewidth+linewidth/2;

    swf_SetRect(tag,&rect);
    swf_SetShapeHeader(tag,s);
    swf_ShapeSetAll(tag,s,0,0,ls1,fs1,0);
    swf_ShapeSetCircle(tag, s, r,r,r,r);
    swf_ShapeSetEnd(tag);
    swf_ShapeFree(s);
   
    s_addcharacter(name, id, tag, rect);
    incrementid();
}

void s_textshape(char*name, char*fontname, float size, char*_text)
{
    int g;
    U8*text = (U8*)_text;
    outline_t* outline;

    SWFFONT*font;
    font = dictionary_lookup(&fonts, fontname);
    if(!font)
	syntaxerror("font \"%s\" not known!", fontname);
    
    if(text[0] >= font->maxascii || font->ascii2glyph[text[0]]<0) {
	warning("no character 0x%02x (%c) in font \"%s\"", text[0], text[0], fontname);
	s_box(name, 0, 0, black, 20, 0);
	return;
    }
    g = font->ascii2glyph[text[0]];

    outline = malloc(sizeof(outline_t));
    memset(outline, 0, sizeof(outline_t));
    outline->shape = font->glyph[g].shape;
    outline->bbox = font->layout->bounds[g];
    
    {
	drawer_t draw;
	swf_Shape11DrawerInit(&draw, 0);
	swf_DrawText(&draw, font, (int)(size*100), _text);
	draw.finish(&draw);
	outline->shape = swf_ShapeDrawerToShape(&draw);
	outline->bbox = swf_ShapeDrawerGetBBox(&draw);
	draw.dealloc(&draw);
    }

    if(dictionary_lookup(&outlines, name))
	syntaxerror("outline %s defined twice", name);
    dictionary_put2(&outlines, name, outline);
}

void s_text(char*name, char*fontname, char*text, int size, RGBA color)
{
    SRECT r;
    MATRIX _m,*m=0;
    SWFFONT*font;
    font = dictionary_lookup(&fonts, fontname);
    if(!font)
	syntaxerror("font \"%s\" not known!", fontname);
    
    tag = swf_InsertTag(tag, ST_DEFINETEXT2);
    swf_SetU16(tag, id);
    if(!font->numchars) {
	s_box(name, 0, 0, black, 20, 0);
	return;
    }
    r = swf_SetDefineText(tag, font, &color, text, size);
   
    s_addcharacter(name, id, tag, r);
    incrementid();
}

void s_quicktime(char*name, char*url)
{
    SRECT r;
    MATRIX _m,*m=0;

    memset(&r, 0, sizeof(r));
    
    tag = swf_InsertTag(tag, ST_DEFINEMOVIE);
    swf_SetU16(tag, id);
    swf_SetString(tag, url);
    
    s_addcharacter(name, id, tag, r);
    incrementid();
}

void s_edittext(char*name, char*fontname, int size, int width, int height, char*text, RGBA*color, int maxlength, char*variable, int flags)
{
    SWFFONT*font;
    EditTextLayout layout;
    SRECT r;

    font = dictionary_lookup(&fonts, fontname);
    if(!font)
	syntaxerror("font \"%s\" not known!", fontname);
    tag = swf_InsertTag(tag, ST_DEFINEEDITTEXT);
    swf_SetU16(tag, id);
    layout.align = 0;
    layout.leftmargin = 0;
    layout.rightmargin = 0;
    layout.indent = 0;
    layout.leading = 0;
    r.xmin = 0;
    r.ymin = 0;
    r.xmax = width;
    r.ymax = height;
    swf_SetEditText(tag, flags|ET_USEOUTLINES, r, text, color, maxlength, font->id, size, &layout, variable);

    s_addcharacter(name, id, tag, r);
    incrementid();
}

/* type: either "jpeg" or "png"
 */
void s_image(char*name, char*type, char*filename, int quality)
{
    /* an image is actually two folded: 1st bitmap, 2nd character.
       Both of them can be used separately */
    
    /* step 1: the bitmap */
    SRECT r;
    int imageID = id;
    int width, height;
    if(type=="png") {
	warning("image type \"png\" not supported yet!");
	s_box(name, 0, 0, black, 20, 0);
	return;
    }
    if(type=="jpeg") {
#ifndef HAVE_LIBJPEG
	warning("no jpeg support compiled in");
	s_box(name, 0, 0, black, 20, 0);
	return;
#else
	tag = swf_InsertTag(tag, ST_DEFINEBITSJPEG2);
	swf_SetU16(tag, imageID);

	if(swf_SetJPEGBits(tag, filename, quality) < 0) {
	    syntaxerror("Image \"%s\" not found, or contains errors", filename);
	}

	swf_GetJPEGSize(filename, &width, &height);

	r.xmin = 0;
	r.ymin = 0;
	r.xmax = width*20;
	r.ymax = height*20;

	s_addimage(name, id, tag, r);
	incrementid();
#endif
    }

    /* step 2: the character */
    tag = swf_InsertTag(tag, ST_DEFINESHAPE); // todo: should be defineshape2/3 once images can be transparent.(?)
    swf_SetU16(tag, id);
    swf_ShapeSetBitmapRect(tag, imageID, width, height);

    s_addcharacter(name, id, tag, r);
    incrementid();
}

void dumpSWF(SWF*swf)
{
    TAG* tag = swf->firstTag;
    printf("vvvvvvvvvvvvvvvvvvvvv\n");
    while(tag) {
	printf("%8d %s\n", tag->len, swf_TagGetName(tag));
	tag = tag->next;
    }
    printf("^^^^^^^^^^^^^^^^^^^^^\n");
}
    
void s_font(char*name, char*filename)
{
    SWFFONT* font;
    font = swf_LoadFont(filename);
   
    if(font == 0) {
	warning("Couldn't open font file \"%s\"", filename);
	font = (SWFFONT*)malloc(sizeof(SWFFONT));
	memset(font, 0, sizeof(SWFFONT));
	dictionary_put2(&fonts, name, font);
	return;
    }

    if(0)
    {
	/* fix the layout. Only needed for old fonts */
	int t;
	for(t=0;t<font->numchars;t++) {
	    font->glyph[t].advance = 0;
	}
	font->layout = 0;
	swf_FontCreateLayout(font);
    }
    /* just in case this thing is used in .edittext later on */
    swf_FontPrepareForEditText(font);

    font->id = id;
    tag = swf_InsertTag(tag, ST_DEFINEFONT2);
    swf_FontSetDefine2(tag, font);
    incrementid();

    if(dictionary_lookup(&fonts, name))
	syntaxerror("font %s defined twice", name);
    dictionary_put2(&fonts, name, font);
}



typedef struct _sound_t
{
    U16 id;
    TAG*tag;
} sound_t;

void s_sound(char*name, char*filename)
{
    struct WAV wav, wav2;
    sound_t* sound;
    U16*samples;
    int numsamples;

    if(!readWAV(filename, &wav)) {
	warning("Couldn't read wav file \"%s\"", filename);
	samples = 0;
	numsamples = 0;
    } else {
	convertWAV2mono(&wav, &wav2, 44100);
	samples = (U16*)wav2.data;
	numsamples = wav2.size/2;
	free(wav.data);
    }

    tag = swf_InsertTag(tag, ST_DEFINESOUND);
    swf_SetU16(tag, id); //id
    swf_SetSoundDefine(tag, samples, numsamples);
   
    sound = (sound_t*)malloc(sizeof(sound_t)); /* mem leak */
    sound->tag = tag;
    sound->id = id;

    if(dictionary_lookup(&sounds, name))
	syntaxerror("sound %s defined twice", name);
    dictionary_put2(&sounds, name, sound);
    
    incrementid();

    if(samples)
	free(samples);
}

static char* gradient_getToken(const char**p)
{
    const char*start;
    char*result;
    while(**p && strchr(" \t\n\r", **p)) {
	(*p)++;
    } 
    start = *p;
    while(**p && !strchr(" \t\n\r", **p)) {
	(*p)++;
    }
    result = malloc((*p)-start+1);
    memcpy(result,start,(*p)-start+1);
    result[(*p)-start] = 0;
    return result;
}

float parsePercent(char*str);
RGBA parseColor(char*str);

GRADIENT parseGradient(const char*str)
{
    GRADIENT gradient;
    const char* p = str;
    memset(&gradient, 0, sizeof(GRADIENT));
    while(*p) {
	char*posstr,*colorstr;
	float pos;
	RGBA color;
	posstr = gradient_getToken(&p);
	if(!*posstr)
	    break;
	pos = parsePercent(posstr);
	if(!*p) syntaxerror("Error in shape data: Color expected after %s", posstr);
	colorstr = gradient_getToken(&p);
	color = parseColor(colorstr);
	if(gradient.num == sizeof(gradient.ratios)/sizeof(gradient.ratios[0])) {
	    warning("gradient record too big- max size is 8, rest ignored");
	    break;
	}
	gradient.ratios[gradient.num] = (int)(pos*255.0);
	gradient.rgba[gradient.num] = color;
	gradient.num++;
	free(posstr);
	free(colorstr);
    }
    return gradient;
}

void s_gradient(char*name, const char*text, int radial, int rotate)
{
    gradient_t* gradient;
    gradient = malloc(sizeof(gradient_t));
    memset(gradient, 0, sizeof(gradient_t));
    gradient->gradient = parseGradient(text);
    gradient->radial = radial;
    gradient->rotate = rotate;

    if(dictionary_lookup(&gradients, name))
	syntaxerror("gradient %s defined twice", name);
    dictionary_put2(&gradients, name, gradient);
}

void s_action(const char*text)
{
    ActionTAG* a = 0;
    a = swf_ActionCompile(text, stack[0].swf->fileVersion);
    if(!a) {
	syntaxerror("Couldn't compile ActionScript");
    }

    tag = swf_InsertTag(tag, ST_DOACTION);

    swf_ActionSet(tag, a);

    swf_ActionFree(a);
}

int s_swf3action(char*name, char*action)
{
    ActionTAG* a = 0;
    instance_t* object = 0;
    if(name) 
	dictionary_lookup(&instances, name);
    if(!object && name && *name) {
	/* we have a name, but couldn't find it. Abort. */
	return 0;
    }
    a = action_SetTarget(0, name);
    if(!strcmp(action, "nextframe")) a = action_NextFrame(a);
    else if(!strcmp(action, "previousframe")) a = action_PreviousFrame(a);
    else if(!strcmp(action, "stop")) a = action_Stop(a);
    else if(!strcmp(action, "play")) a = action_Play(a);
    a = action_SetTarget(a, "");
    a = action_End(a);

    tag = swf_InsertTag(tag, ST_DOACTION);
    swf_ActionSet(tag, a);
    swf_ActionFree(a);
    return 1;
}

void s_outline(char*name, char*format, char*source)
{
    outline_t* outline;

    drawer_t draw;
    SHAPE* shape;
    SHAPE2* shape2;
    SRECT bounds;
    
    swf_Shape11DrawerInit(&draw, 0);
    draw_string(&draw, source);
    draw.finish(&draw);
    shape = swf_ShapeDrawerToShape(&draw);
    bounds = swf_ShapeDrawerGetBBox(&draw);
    draw.dealloc(&draw);
    
    outline = (outline_t*)rfx_calloc(sizeof(outline_t));
    outline->shape = shape;
    outline->bbox = bounds;
    
    if(dictionary_lookup(&outlines, name))
	syntaxerror("outline %s defined twice", name);
    dictionary_put2(&outlines, name, outline);
}

int s_playsound(char*name, int loops, int nomultiple, int stop)
{
    sound_t* sound;
    if(!name)
	return 0;
    sound = dictionary_lookup(&sounds, name);
    SOUNDINFO info;
    if(!sound)
	return 0;

    tag = swf_InsertTag(tag, ST_STARTSOUND);
    swf_SetU16(tag, sound->id); //id
    memset(&info, 0, sizeof(info));
    info.stop = stop;
    info.loops = loops;
    info.nomultiple = nomultiple;
    swf_SetSoundInfo(tag, &info);
    return 1;
}

void s_includeswf(char*name, char*filename)
{
    int f;
    SWF swf;
    TAG* ftag;
    SRECT r;
    TAG* s;
    int level = 0;
    U16 cutout[] = {ST_SETBACKGROUNDCOLOR, ST_PROTECT, ST_FREEALL, ST_REFLEX};
    f = open(filename,O_RDONLY|O_BINARY);
    if (f<0) { 
	warning("Couldn't open file \"%s\": %s", filename, strerror(errno));
	s_box(name, 0, 0, black, 20, 0);
	return;
    }
    if (swf_ReadSWF(f,&swf)<0) { 
	warning("Only SWF files supported in .shape for now. File \"%s\" wasn't SWF.", filename);
	s_box(name, 0, 0, black, 20, 0);
	return;
    }
    close(f);

    /* FIXME: The following sets the bounding Box for the character. 
              It is wrong for two reasons:
	      a) It may be too small (in case objects in the movie clip at the borders)
	      b) it may be too big (because the poor movie never got autocropped)
    */
    r = swf.movieSize;
    
    s = tag = swf_InsertTag(tag, ST_DEFINESPRITE);
    swf_SetU16(tag, id);
    swf_SetU16(tag, 0);

    swf_Relocate(&swf, idmap);

    ftag = swf.firstTag;
    level = 1;
    while(ftag) {
	int t;
	for(t=0;t<sizeof(cutout)/sizeof(cutout[0]);t++)
	    if(cutout[t] == ftag->id) {
		ftag = ftag->next;
		continue;
	    }
	if(ftag->id == ST_DEFINESPRITE && !swf_IsFolded(ftag))
	    level++;
	if(ftag->id == ST_END)
	    level--;
	if(!level)
	    break;
	/* We simply dump all tags right after the sprite
	   header, relying on the fact that swf_OptimizeTagOrder() will
	   sort things out for us later. 
	   We also rely on the fact that the imported SWF is well-formed.
	 */
	tag = swf_InsertTag(tag, ftag->id);
	swf_SetBlock(tag, ftag->data, ftag->len);
	ftag = ftag->next;
    }
    if(!ftag)
	syntaxerror("Included file %s contains errors", filename);
    tag = swf_InsertTag(tag, ST_END);

    swf_FreeTags(&swf);

    s_addcharacter(name, id, tag, r);
    incrementid();
}
SRECT s_getCharBBox(char*name)
{
    character_t* c = dictionary_lookup(&characters, name);
    if(!c) syntaxerror("character '%s' unknown(2)", name);
    return c->size;
}
SRECT s_getInstanceBBox(char*name)
{
    instance_t * i = dictionary_lookup(&instances, name);
    character_t * c;
    if(!i) syntaxerror("instance '%s' unknown(4)", name);
    c = i->character;
    if(!c) syntaxerror("internal error(5)");
    return c->size;
}
parameters_t s_getParameters(char*name)
{
    instance_t * i = dictionary_lookup(&instances, name);
    if(!i) syntaxerror("instance '%s' unknown(10)", name);
    return i->parameters;
}
void s_startclip(char*instance, char*character, parameters_t p)
{
    character_t* c = dictionary_lookup(&characters, character);
    instance_t* i;
    MATRIX m;
    if(!c) {
	syntaxerror("character %s not known", character);
    }
    i = s_addinstance(instance, c, currentdepth);
    i->parameters = p;
    m = s_instancepos(i->character->size, &p);
    
    tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
    /* TODO: should be ObjectPlaceClip, with clipdepth backpatched */
    swf_ObjectPlace(tag, c->id, currentdepth, &m, &p.cxform, instance);
    i->lastTag = tag;
    i->lastFrame= currentframe;

    stack[stackpos].tag = tag;
    stack[stackpos].type = 2;
    stackpos++;

    currentdepth++;
}
void s_endClip()
{
    SWFPLACEOBJECT p;
    stackpos--;
    swf_SetTagPos(stack[stackpos].tag, 0);
    swf_GetPlaceObject(stack[stackpos].tag, &p);
    p.clipdepth = currentdepth;
    p.name = 0;
    swf_ClearTag(stack[stackpos].tag);
    swf_SetPlaceObject(stack[stackpos].tag, &p);
    currentdepth++;
}

void s_put(char*instance, char*character, parameters_t p)
{
    character_t* c = dictionary_lookup(&characters, character);
    instance_t* i;
    MATRIX m;
    if(!c) {
	syntaxerror("character %s not known (in .put %s=%s)", character, instance, character);
    }
    
    i = s_addinstance(instance, c, currentdepth);
    i->parameters = p;
    m = s_instancepos(i->character->size, &p);
    
    tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
    swf_ObjectPlace(tag, c->id, currentdepth, &m, &p.cxform, instance);
    i->lastTag = tag;
    i->lastFrame = currentframe;
    currentdepth++;
}

void s_jump(char*instance, parameters_t p)
{
    instance_t* i = dictionary_lookup(&instances, instance);
    MATRIX m;
    if(!i) {
	syntaxerror("instance %s not known", instance);
    }

    i->parameters = p;
    m = s_instancepos(i->character->size, &p);

    tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
    swf_ObjectMove(tag, i->depth, &m, &p.cxform);
    i->lastTag = tag;
    i->lastFrame = currentframe;
}

parameters_t s_interpolate(parameters_t*p1, parameters_t*p2, int pos, int num)
{
    parameters_t p;
    float ratio;
    if(num==0 || num==1)
	return *p1;
    ratio = (float)pos/(float)num;
    
    p.x = (p2->x-p1->x)*ratio + p1->x;
    p.y = (p2->y-p1->y)*ratio + p1->y;
    p.scalex = (p2->scalex-p1->scalex)*ratio + p1->scalex;
    p.scaley = (p2->scaley-p1->scaley)*ratio + p1->scaley;
    p.rotate = (p2->rotate-p1->rotate)*ratio + p1->rotate;
    p.shear = (p2->shear-p1->shear)*ratio + p1->shear;

    p.cxform.r0 = ((float)p2->cxform.r0-(float)p1->cxform.r0)*ratio + p1->cxform.r0;
    p.cxform.g0 = ((float)p2->cxform.g0-(float)p1->cxform.g0)*ratio + p1->cxform.g0;
    p.cxform.b0 = ((float)p2->cxform.b0-(float)p1->cxform.b0)*ratio + p1->cxform.b0;
    p.cxform.a0 = ((float)p2->cxform.a0-(float)p1->cxform.a0)*ratio + p1->cxform.a0;

    p.cxform.r1 = (p2->cxform.r1-p1->cxform.r1)*ratio + p1->cxform.r1;
    p.cxform.g1 = (p2->cxform.g1-p1->cxform.g1)*ratio + p1->cxform.g1;
    p.cxform.b1 = (p2->cxform.b1-p1->cxform.b1)*ratio + p1->cxform.b1;
    p.cxform.a1 = (p2->cxform.a1-p1->cxform.a1)*ratio + p1->cxform.a1;

    p.pivot.x = (p2->pivot.x-p1->pivot.x)*ratio + p1->pivot.x;
    p.pivot.y = (p2->pivot.y-p1->pivot.y)*ratio + p1->pivot.y;
    p.pin.x = (p2->pin.x-p1->pin.x)*ratio + p1->pin.x;
    p.pin.y = (p2->pin.y-p1->pin.y)*ratio + p1->pin.y;
    return p;
}

void s_change(char*instance, parameters_t p2)
{
    instance_t* i = dictionary_lookup(&instances, instance);
    MATRIX m;
    parameters_t p1;
    TAG*t;
    int frame, allframes;
    if(!i) {
	syntaxerror("instance %s not known", instance);
    }
    p1 = i->parameters;
    
    allframes = currentframe - i->lastFrame - 1;
    if(allframes < 0) {
	warning(".change ignored. can only .put/.change an object once per frame.");
	return;
    }
    
    m = s_instancepos(i->character->size, &p2);
    tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
    swf_ObjectMove(tag, i->depth, &m, &p2.cxform);
    i->parameters = p2;

    /* o.k., we got the start and end point set. Now iterate though all the
       tags in between, inserting object changes after each new frame */
    t = i->lastTag;
    i->lastTag = tag;
    if(!t) syntaxerror("internal error(6)");
    frame = 0;
    while(frame < allframes) {
	if(t->id == ST_SHOWFRAME) {
	    parameters_t p;
	    MATRIX m;
	    TAG*lt;
	    frame ++;
	    p = s_interpolate(&p1, &p2, frame, allframes);
	    m = s_instancepos(i->character->size, &p); //needed?
	    lt = swf_InsertTag(t, ST_PLACEOBJECT2);
	    i->lastFrame = currentframe;
	    swf_ObjectMove(lt, i->depth, &m, &p.cxform);
	    t = lt;
	    if(frame == allframes)
		break;
	}
	t = t->next;
	if(!t) 
	    syntaxerror("internal error(8) (frame=%d/%d)", frame, allframes);
    }
}

void s_delinstance(char*instance)
{
    instance_t* i = dictionary_lookup(&instances, instance);
    if(!i) {
	syntaxerror("instance %s not known", instance);
    }
    tag = swf_InsertTag(tag, ST_REMOVEOBJECT2);
    swf_SetU16(tag, i->depth);
    dictionary_del(&instances, instance);
}

void s_qchange(char*instance, parameters_t p)
{
}

void s_end()
{
    if(!stackpos)
	syntaxerror(".end unexpected");
    if(stack[stackpos-1].type == 0)
	s_endSWF();
    else if(stack[stackpos-1].type == 1)
	s_endSprite();
    else if(stack[stackpos-1].type == 2)
	s_endClip();
    else if(stack[stackpos-1].type == 3)
	s_endButton();
    else syntaxerror("internal error 1");
}

// ------------------------------------------------------------------------

typedef int command_func_t(map_t*args);

SRECT parseBox(char*str)
{
    SRECT r;
    float xmin, xmax, ymin, ymax;
    char*x = strchr(str, 'x');
    char*d1=0,*d2=0;
    if(!strcmp(str, "autocrop")) {
	r.xmin = r.ymin = r.xmax = r.ymax = 0;
	return r;
    }
    if(!x) goto error;
    d1 = strchr(x+1, ':');
    if(d1)
	d2 = strchr(d1+1, ':');
    if(!d1) {
	if(sscanf(str, "%fx%f", &xmax, &ymax) < 2)
	    goto error;
	xmin = ymin = 0;
    }
    else if(d1 && !d2) {
	if(sscanf(str, "%fx%f:%f", &xmax, &ymax, &xmin) < 3)
	    goto error;
	xmax += xmin;
	ymin = 0;
    }
    else {
	if(sscanf(str, "%fx%f:%f:%f", &xmax, &ymax, &xmin, &ymin) < 4)
	    goto error;
	xmax += xmin;
	ymax += ymin;
    }
    r.xmin = (SCOORD)(xmin*20);
    r.ymin = (SCOORD)(ymin*20);
    r.xmax = (SCOORD)(xmax*20);
    r.ymax = (SCOORD)(ymax*20);
    return r;
error:
    syntaxerror("expression %s is not a valid bound Box.\nE.g. 1024x768 or 1024x768:30:30 would have been valid bounding Boxes.", str);
    return r;
}
float parseFloat(char*str)
{
    return atof(str);
}
int parseInt(char*str)
{
    int t;
    int l=strlen(str);
    int s=0;
    if(str[0]=='+' || str[0]=='-')
	s++;

    for(t=s;t<l;t++)
	if(str[t]<'0' || str[t]>'9')
	    syntaxerror("Not an Integer: \"%s\"", str);
    return atoi(str);
}
int parseTwip(char*str)
{
    char*dot;
    int sign=1;
    if(str[0]=='+' || str[0]=='-') {
	if(str[0]=='-')
	    sign = -1;
	str++;
    }
    dot = strchr(str, '.');
    if(!dot) {
	int l=strlen(str);
	int t;
	return sign*parseInt(str)*20;
    } else {
	int l=strlen(++dot);
	char*s;
	for(s=str;s<dot-1;s++)
	    if(*s<'0' || *s>'9')
		syntaxerror("Not a coordinate: \"%s\"", str);
	for(s=dot;*s;s++) {
	    if(*s<'0' || *s>'9')
		syntaxerror("Not a coordinate: \"%s\"", str);
	}
	if(l>2 || (l==2 && (dot[1]!='0' || dot[1]!='5'))) {
	    warning("precision loss: %s converted to twip", str);
	    dot[2] = 0;
	    l=2;
	}
	if(l==0)
	    return sign*atoi(str)*20;
	if(l==1)
	    return sign*atoi(str)*20+atoi(dot)*2;
	if(l==2)
	    return sign*atoi(str)*20+atoi(dot)/5;
    }
    return 0;
}

int isPoint(char*str)
{
    if(strchr(str, '('))
	return 1;
    else
	return 0;
}

SPOINT parsePoint(char*str)
{
    SPOINT p;
    char tmp[80];
    int l = strlen(str);
    char*comma = strchr(str, ',');
    if(str[0]!='(' || str[l-1]!=')' || !comma || l>70)
	syntaxerror("\"%s\" is not a valid point of the form (x,y)", str);
    strncpy(tmp, str+1, comma-(str+1));tmp[comma-(str+1)]=0;
    p.x = parseTwip(tmp);
    strncpy(tmp, comma+1, l-1-(comma+1-str));tmp[l-1-(comma+1-str)]=0;
    p.y = parseTwip(tmp);
    return p;
}

int parseColor2(char*str, RGBA*color)
{
    int l = strlen(str);
    int r,g,b,a;
    int t;

    struct {unsigned char r,g,b;char*name;} colors[] =
    {{255,250,250,"snow"},{220,220,220,"gainsboro"},{250,240,230,"linen"},{255,228,196,"bisque"},
    {255,228,181,"moccasin"},{255,248,220,"cornsilk"},{255,255,240,"ivory"},{255,245,238,"seashell"},
    {240,255,240,"honeydew"},{240,255,255,"azure"},{230,230,250,"lavender"},{255,255,255,"white"},
    {0,0,0,"black"},{190,190,190,"gray"},{190,190,190,"grey"},{0,0,128,"navy"},{0,0,255,"blue"},
    {64,224,208,"turquoise"},{0,255,255,"cyan"},{127,255,212,"aquamarine"}, {0,255,0,"green"},
    {127,255,0,"chartreuse"},{240,230,140,"khaki"},{255,255,0,"yellow"},
    {255,215,0,"gold"},{218,165,32,"goldenrod"},{160,82,45,"sienna"},{205,133,63,"peru"},
    {222,184,135,"burlywood"},{245,245,220,"beige"},{245,222,179,"wheat"},{210,180,140,"tan"},
    {210,105,30,"chocolate"},{178,34,34,"firebrick"},{165,42,42,"brown"},{250,128,114,"salmon"},
    {255,165,0,"orange"},{255,127,80,"coral"},{255,99,71,"tomato"},{255,0,0,"red"},
    {255,192,203,"pink"},{176,48,96,"maroon"},{255,0,255,"magenta"},{238,130,238,"violet"},
    {221,160,221,"plum"},{218,112,214,"orchid"},{160,32,240,"purple"},{216,191,216,"thistle"}};

    a=255;r=g=b=0;

    if(str[0]=='#' && (l==7 || l==9)) {
	if(l == 7 && !(sscanf(str, "#%02x%02x%02x", &r, &g, &b)))
	    return 0;
	if(l == 9 && !(sscanf(str, "#%02x%02x%02x%02x", &r, &g, &b, &a)))
	    return 0;
	color->r = r; color->g = g; color->b = b; color->a = a;
	return 1;
    }
    for(t=0;t<sizeof(colors)/sizeof(colors[0]);t++)
	if(!strcmp(str, colors[t].name)) {
	    r = colors[t].r;
	    g = colors[t].g;
	    b = colors[t].b;
	    a = 255;
	    color->r = r; color->g = g; color->b = b; color->a = a;
	    return 1;
	}
    return 0;

}
RGBA parseColor(char*str)
{
    RGBA c;
    if(!parseColor2(str, &c))
	syntaxerror("Expression '%s' is not a color", str);
    return c;
}

typedef struct _muladd {
    S16 mul;
    S16 add;
} MULADD;

MULADD parseMulAdd(char*str)
{
    float add, mul;
    char* str2 = (char*)malloc(strlen(str)+5);
    int i;
    MULADD m;
    strcpy(str2, str);
    strcat(str2, " 0");
    add = 0;
    mul = 1.0;
    if(sscanf(str2, "%f%f %d", &mul, &add, &i)==2+1) { add/=256.0; i=1;}
    else if(sscanf(str2, "%f%%%f %d", &mul, &add, &i)==2+1) { mul/=100.0; add/=256.0; i=2;}
    else if(sscanf(str2, "%f%f%% %d", &mul, &add, &i)==2+1) { add/=100.0; i=3;}
    else if(sscanf(str2, "%f%%%f%% %d", &mul, &add, &i)==2+1) { mul/=100.0; add/=100.0; i=4;}
    else if(sscanf(str2, "+%f%% %d", &add, &i)==1+1) { mul=1.0;add/=100.0; i=5;}
    else if(sscanf(str2, "+%f %d", &add, &i)==1+1) { mul=1.0;add/=256.0; i=6;}
    else if(sscanf(str2, "-%f%% %d", &add, &i)==1+1) { mul=1.0;add/=-100.0; i=7;}
    else if(sscanf(str2, "-%f %d", &add, &i)==1+1) { mul=1.0;add/=-256.0; i=8;}
    else if(sscanf(str2, "%f%% %d", &mul, &i)==1+1) { mul/=100.0;add=0; i=9;}
    else if(sscanf(str2, "%f %d", &mul, &i)==1+1) { add=0; i=10;}
    else {
	syntaxerror("'%s' is not a valid color transform expression", str);
    }
    m.add = (int)(add*256);
    m.mul = (int)(mul*256);
    free(str2);
    return m;
}

MULADD mergeMulAdd(MULADD m1, MULADD m2)
{
    int a = (int)((double)m2.add+((double)m1.add*(double)m2.mul)/256.0);
    double m = ((double)m1.mul*(double)m2.mul)/256.0;
    MULADD r;
    if(a<-32768) a=-32768;
    if(a>32767) a=32767;
    if(m<-32768) m=-32768;
    if(m>32767) m=32767;
    r.add = a;
    r.mul = (int)m;
    return r;
}

float parsePercent(char*str)
{
    int l = strlen(str);
    if(!l)
	return 1.0;
    if(str[l-1]=='%') {
	return atoi(str)/100.0;
    }
    syntaxerror("Expression '%s' is not a percentage", str);
    return 0;
}
int isPercent(char*str)
{
    return str[strlen(str)-1]=='%';
}
int parseNewSize(char*str, int size)
{
    if(isPercent(str))
	return parsePercent(str)*size;
    else
	return (int)(atof(str)*20);
}

int isColor(char*str)
{
    RGBA c;
    return parseColor2(str, &c);
}

static char* lu(map_t* args, char*name)
{
    char* value = map_lookup(args, name);
    if(!value) {
	map_dump(args, stdout, "");
	syntaxerror("internal error 2: value %s should be set", name);
    }
    return value;
}

static int c_flash(map_t*args) 
{
    char* name = lu(args, "name");
    char* compressstr = lu(args, "compress");
    SRECT bbox = parseBox(lu(args, "bbox"));
    int version = parseInt(lu(args, "version"));
    int fps = (int)(parseFloat(lu(args, "fps"))*256);
    int compress = 0;
    RGBA color = parseColor(lu(args, "background"));
    if(!strcmp(name, "!default!") || override_outputname)
	name = outputname;
    
    if(!strcmp(compressstr, "default"))
	compress = version==6;
    else if(!strcmp(compressstr, "yes") || !strcmp(compressstr, "compress"))
	compress = 1;
    else if(!strcmp(compressstr, "no"))
	compress = 0;
    else syntaxerror("value \"%s\" not supported for the compress argument", compressstr);

    s_swf(name, bbox, version, fps, compress, color);
    return 0;
}
int isRelative(char*str)
{
    return !strncmp(str, "<plus>", 6) ||
	   !strncmp(str, "<minus>", 7);
}
char* getOffset(char*str)
{
    if(!strncmp(str, "<plus>", 6))
	return str+6;
    if(!strncmp(str, "<minus>", 7))
	return str+7;
    syntaxerror("internal error (347)");
    return 0;
}
int getSign(char*str)
{
    if(!strncmp(str, "<plus>", 6))
	return 1;
    if(!strncmp(str, "<minus>", 7))
	return -1;
    syntaxerror("internal error (348)");
    return 0;
}
static dictionary_t points;
static mem_t mpoints;
int points_initialized = 0;

SPOINT getPoint(SRECT r, char*name)
{
    int l=0;
    if(!strcmp(name, "center")) {
	SPOINT p;
	p.x = (r.xmin + r.xmax)/2;
	p.y = (r.ymin + r.ymax)/2;
	return p;
    }

    if(points_initialized)
	l = (int)dictionary_lookup(&points, name);
    if(l==0) {
	syntaxerror("Invalid point: \"%s\".", name);
    }
    l--;
    return *(SPOINT*)&mpoints.buffer[l];
}
static int c_gradient(map_t*args) 
{
    char*name = lu(args, "name");
    int radial= strcmp(lu(args, "radial"), "radial")?0:1;
    int rotate = parseInt(lu(args, "rotate"));

    readToken();
    if(type != RAWDATA)
	syntaxerror("colon (:) expected");

    s_gradient(name, text, radial,rotate);
    return 0;
}
static int c_point(map_t*args) 
{
    char*name = lu(args, "name");
    int pos;
    string_t s1;
    SPOINT p;
    if(!points_initialized) {
	dictionary_init(&points);
	mem_init(&mpoints);
	points_initialized = 1;
    }
    p.x = parseTwip(lu(args, "x"));
    p.y = parseTwip(lu(args, "y"));
    pos = mem_put(&mpoints, &p, sizeof(p));
    string_set(&s1, name);
    pos++;
    dictionary_put(&points, s1, (void*)pos);
    return 0;
}
static int c_play(map_t*args) 
{
    char*name = lu(args, "name");
    char*loop = lu(args, "loop");
    char*nomultiple = lu(args, "nomultiple");
    int nm = 0;
    if(!strcmp(nomultiple, "nomultiple"))
	nm = 1;
    else
	nm = parseInt(nomultiple);

    if(s_playsound(name, parseInt(loop), nm, 0)) {
	return 0;
    } else if(s_swf3action(name, "play")) {
	return 0;
    }
    return 0;
}

static int c_stop(map_t*args) 
{
    char*name = map_lookup(args, "name");

    if(s_playsound(name, 0,0,1)) {
	return 0;
    } else if(s_swf3action(name, "stop")) {
	return 0;
    }
    syntaxerror("I don't know anything about sound/movie \"%s\"", name);
    return 0;
}

static int c_nextframe(map_t*args) 
{
    char*name = lu(args, "name");

    if(s_swf3action(name, "nextframe")) {
	return 0;
    }
    syntaxerror("I don't know anything about movie \"%s\"", name);
    return 0;
}

static int c_previousframe(map_t*args) 
{
    char*name = lu(args, "name");

    if(s_swf3action(name, "previousframe")) {
	return 0;
    }
    syntaxerror("I don't know anything about movie \"%s\"", name);
    return 0;
}

static int c_placement(map_t*args, int type)
{
    char*instance = lu(args, (type==0||type==4)?"instance":"name");
    char*character = 0;

    char* luminancestr = lu(args, "luminance");
    char* scalestr = lu(args, "scale");
    char* scalexstr = lu(args, "scalex");
    char* scaleystr = lu(args, "scaley");
    char* rotatestr = lu(args, "rotate");
    char* shearstr = lu(args, "shear");
    char* xstr="", *pivotstr="";
    char* ystr="", *anglestr="";
    char*above = lu(args, "above"); /*FIXME*/
    char*below = lu(args, "below");
    char* rstr = lu(args, "red");
    char* gstr = lu(args, "green");
    char* bstr = lu(args, "blue");
    char* astr = lu(args, "alpha");
    char* pinstr = lu(args, "pin");
    char* as = map_lookup(args, "as");
    MULADD r,g,b,a;
    float oldwidth;
    float oldheight;
    SRECT oldbbox;
    MULADD luminance;
    parameters_t p;

    if(type==9) { // (?) .rotate  or .arcchange
	pivotstr = lu(args, "pivot");
	anglestr = lu(args, "angle");
    } else {
	xstr = lu(args, "x");
	ystr = lu(args, "y");
    }
    if(luminancestr[0])
	luminance = parseMulAdd(luminancestr);
    else {
	luminance.add = 0;
	luminance.mul = 256;
    }

    if(scalestr[0]) {
	if(scalexstr[0]||scaleystr[0])
	    syntaxerror("scalex/scaley and scale cannot both be set");
	scalexstr = scaleystr = scalestr;
    }
    
    if(type == 0 || type == 4)  {
	// put or startclip
	character = lu(args, "character");
	parameters_clear(&p);
    } else if (type == 5) {
	character = lu(args, "name");
	parameters_clear(&p);
	// button's show
    } else {
	p = s_getParameters(instance);
    }

    /* x,y position */
    if(xstr[0]) {
	if(isRelative(xstr)) {
	    if(type == 0 || type == 4)
		syntaxerror("relative x values not allowed for initial put or startclip");
	    p.x += parseTwip(getOffset(xstr))*getSign(xstr);
	} else {
	    p.x = parseTwip(xstr);
	}
    }
    if(ystr[0]) {
	if(isRelative(ystr)) {
	    if(type == 0 || type == 4)
		syntaxerror("relative y values not allowed for initial put or startclip");
	    p.y += parseTwip(getOffset(ystr))*getSign(ystr);
	} else {
	    p.y = parseTwip(ystr);
	}
    }

    /* scale, scalex, scaley */
    if(character) {
	oldbbox = s_getCharBBox(character);
    } else {
	oldbbox = s_getInstanceBBox(instance);
    }
    oldwidth = oldbbox.xmax - oldbbox.xmin;
    oldheight = oldbbox.ymax - oldbbox.ymin;
    if(scalexstr[0]) {
	if(oldwidth==0) p.scalex = 1.0;
	else {	    
	    if(scalexstr[0])
		p.scalex = (float)(parseNewSize(scalexstr, oldwidth))/oldwidth;
	}
    }
    if(scaleystr[0]) {
	if(oldheight==0) p.scaley = 1.0;
	else {
	    if(scaleystr[0])
		p.scaley = (float)(parseNewSize(scaleystr, oldheight))/oldheight;
	}
    }
   
    /* rotation */
    if(rotatestr[0]) {
	if(isRelative(rotatestr)) {
	    p.rotate += parseFloat(getOffset(rotatestr))*getSign(rotatestr);
	} else {
	    p.rotate = parseFloat(rotatestr);
	}
    }

    /* shearing */
    if(shearstr[0]) {
	if(isRelative(shearstr)) {
	    p.shear += parseFloat(getOffset(shearstr))*getSign(shearstr);
	} else {
	    p.shear = parseFloat(shearstr);
	}
    }

    if(pivotstr[0]) {
	if(isPoint(pivotstr)) 
	    p.pivot = parsePoint(pivotstr);
	else 
	    p.pivot = getPoint(oldbbox, pivotstr);
    }
    if(pinstr[0]) {
	if(isPoint(pinstr))
	    p.pin = parsePoint(pinstr);
	else
	    p.pin = getPoint(oldbbox, pinstr);
    }
	
    /* color transform */

    if(rstr[0] || luminancestr[0]) {
	MULADD r;
	if(rstr[0])
	    r = parseMulAdd(rstr);
	else {
	    r.add = p.cxform.r0;
	    r.mul = p.cxform.r1;
	}
	r = mergeMulAdd(r, luminance);
	p.cxform.r0 = r.mul;p.cxform.r1 = r.add;
    }
    if(gstr[0] || luminancestr[0]) {
	MULADD g;
	if(gstr[0])
 	    g = parseMulAdd(gstr);
	else {
	    g.add = p.cxform.g0;
	    g.mul = p.cxform.g1;
	}
	g = mergeMulAdd(g, luminance);
	p.cxform.g0 = g.mul;p.cxform.g1 = g.add;
    }
    if(bstr[0] || luminancestr[0]) {
	MULADD b;
	if(bstr[0])
 	    b = parseMulAdd(bstr);
	else {
	    b.add = p.cxform.b0;
	    b.mul = p.cxform.b1;
	}
	b = mergeMulAdd(b, luminance);
	p.cxform.b0 = b.mul;p.cxform.b1 = b.add;
    }
    if(astr[0]) {
	MULADD a = parseMulAdd(astr);
	p.cxform.a0 = a.mul;p.cxform.a1 = a.add;
    }

    if(type == 0)
	s_put(instance, character, p);
    else if(type == 1)
	s_change(instance, p);
    else if(type == 2)
	s_qchange(instance, p);
    else if(type == 3)
	s_jump(instance, p);
    else if(type == 4)
	s_startclip(instance, character, p);
    else if(type == 5) {
	if(as && as[0]) {
	    s_buttonput(character, as, p);
	} else {
	    s_buttonput(character, "shape", p);
	}
    }
    return 0;
}
static int c_put(map_t*args) 
{
    c_placement(args, 0);
    return 0;
}
static int c_change(map_t*args) 
{
    c_placement(args, 1);
    return 0;
}
static int c_qchange(map_t*args) 
{
    c_placement(args, 2);
    return 0;
}
static int c_arcchange(map_t*args) 
{
    c_placement(args, 2);
    return 0;
}
static int c_jump(map_t*args) 
{
    c_placement(args, 3);
    return 0;
}
static int c_startclip(map_t*args) 
{
    c_placement(args, 4);
    return 0;
}
static int c_show(map_t*args) 
{
    c_placement(args, 5);
    return 0;
}
static int c_del(map_t*args) 
{
    char*instance = lu(args, "name");
    s_delinstance(instance);
    return 0;
}
static int c_end(map_t*args) 
{
    s_end();
    return 0;
}
static int c_sprite(map_t*args) 
{
    char* name = lu(args, "name");
    s_sprite(name);
    return 0;
}
static int c_frame(map_t*args) 
{
    char*framestr = lu(args, "n");
    char*cutstr = lu(args, "cut");
    char*name = lu(args, "name");
    int frame;
    int cut = 0;
    if(strcmp(cutstr, "no"))
	cut = 1;
    if(isRelative(framestr)) {
	frame = s_getframe();
	if(getSign(framestr)<0)
	    syntaxerror("relative frame expressions must be positive");
	frame += parseInt(getOffset(framestr));
    }
    else {
	frame = parseInt(framestr);
	if(s_getframe() >= frame
		&& !(frame==1 && s_getframe()==frame)) // equality is o.k. for frame 0
	    syntaxerror("frame expression must be >%d (is:%s)", s_getframe(), framestr);
    }
    s_frame(frame, cut, name);
    return 0;
}
static int c_primitive(map_t*args) 
{
    char*name = lu(args, "name");
    char*command = lu(args, "commandname");
    int width=0, height=0, r=0;
    int linewidth = parseTwip(lu(args, "line"));
    char*colorstr = lu(args, "color");
    RGBA color = parseColor(colorstr);
    char*fillstr = lu(args, "fill");
    int dofill = 1;
    int type=0;
    char* font;
    char* text;
    char* outline=0;
    RGBA fill;
    if(!strcmp(command, "circle"))
	type = 1;
    else if(!strcmp(command, "filled"))
	type = 2;
   
    if(type==0) {
	width = parseTwip(lu(args, "width"));
	height = parseTwip(lu(args, "height"));
    } else if (type==1) {
	r = parseTwip(lu(args, "r"));
    } else if (type==2) {
	outline = lu(args, "outline");
    }

    if(!strcmp(fillstr, "fill"))
	fillstr = colorstr;
    if(!strcmp(fillstr, "none"))
	fillstr = 0;
    if(width<0 || height<0 || linewidth<0 || r<0)
	syntaxerror("values width, height, line, r must be positive");
    
    if(type == 0) s_box(name, width, height, color, linewidth, fillstr);
    else if(type==1) s_circle(name, r, color, linewidth, fillstr);
    else if(type==2) s_filled(name, outline, color, linewidth, fillstr);
    return 0;
}

static int c_textshape(map_t*args) 
{
    char*name = lu(args, "name");
    char*text = lu(args, "text");
    char*font = lu(args, "font");
    float size = parsePercent(lu(args, "size"));

    s_textshape(name, font, size, text);
    return 0;
}

static int c_swf(map_t*args) 
{
    char*name = lu(args, "name");
    char*filename = lu(args, "filename");
    char*command = lu(args, "commandname");
    if(!strcmp(command, "shape"))
	warning("Please use .swf instead of .shape");
    s_includeswf(name, filename);
    return 0;
}

static int c_font(map_t*args) 
{
    char*name = lu(args, "name");
    char*filename = lu(args, "filename");
    s_font(name, filename);
    return 0;
}

static int c_sound(map_t*args) 
{
    char*name = lu(args, "name");
    char*filename = lu(args, "filename");
    s_sound(name, filename);
    return 0;
}

static int c_text(map_t*args) 
{
    char*name = lu(args, "name");
    char*text = lu(args, "text");
    char*font = lu(args, "font");
    float size = parsePercent(lu(args, "size"));
    RGBA color = parseColor(lu(args, "color"));
    s_text(name, font, text, (int)(size*100), color);
    return 0;
}

static int c_soundtrack(map_t*args) 
{
    return 0;
}

static int c_quicktime(map_t*args) 
{
    char*name = lu(args, "name");
    char*url = lu(args, "url");
    s_quicktime(name, url);
    return 0;
}

static int c_image(map_t*args) 
{
    char*command = lu(args, "commandname");
    char*name = lu(args, "name");
    char*filename = lu(args, "filename");
    if(!strcmp(command,"jpeg")) {
	int quality = (int)(parsePercent(lu(args, "quality"))*100);
	s_image(name, "jpeg", filename, quality);
    } else {
	s_image(name, "png", filename, 0);
    }
    return 0;
}

static int c_outline(map_t*args) 
{
    char*name = lu(args, "name");
    char*format = lu(args, "format");

    readToken();
    if(type != RAWDATA)
	syntaxerror("colon (:) expected");

    s_outline(name, format, text);
    return 0;
}

int fakechar(map_t*args)
{
    char*name = lu(args, "name");
    s_box(name, 0, 0, black, 20, 0);
    return 0;
}

static int c_egon(map_t*args) {return fakechar(args);}
static int c_button(map_t*args) {
    char*name = lu(args, "name");
    s_button(name);
    return 0;
}
static int current_button_flags = 0;
static int c_on_press(map_t*args)
{
    char*position = lu(args, "position");
    char*action = "";
    if(!strcmp(position, "inside")) {
	current_button_flags |= BC_OVERUP_OVERDOWN;
    } else if(!strcmp(position, "outside")) {
	//current_button_flags |= BC_IDLE_OUTDOWN;
	syntaxerror("IDLE_OVERDOWN not supported by SWF");
    } else if(!strcmp(position, "anywhere")) {
	current_button_flags |= /*BC_IDLE_OUTDOWN|*/BC_OVERUP_OVERDOWN|BC_IDLE_OVERDOWN;
    }
    readToken();
    if(type == RAWDATA) {
	action = text;
	s_buttonaction(current_button_flags, action);
	current_button_flags = 0;
    }
    else
	pushBack();
    return 0;
}
static int c_on_release(map_t*args)
{
    char*position = lu(args, "position");
    char*action = "";
    if(!strcmp(position, "inside")) {
	current_button_flags |= BC_OVERDOWN_OVERUP;
    } else if(!strcmp(position, "outside")) {
	current_button_flags |= BC_OUTDOWN_IDLE;
    } else if(!strcmp(position, "anywhere")) {
	current_button_flags |= BC_OVERDOWN_OVERUP|BC_OUTDOWN_IDLE|BC_OVERDOWN_IDLE;
    }
    readToken();
    if(type == RAWDATA) {
	action = text;
	s_buttonaction(current_button_flags, action);
	current_button_flags = 0;
    }
    else
	pushBack();
    return 0;
}
static int c_on_move_in(map_t*args)
{
    char*position = lu(args, "state");
    char*action = "";
    if(!strcmp(position, "pressed")) {
	current_button_flags |= BC_OUTDOWN_OVERDOWN;
    } else if(!strcmp(position, "not_pressed")) {
	current_button_flags |= BC_IDLE_OVERUP;
    } else if(!strcmp(position, "any")) {
	current_button_flags |= BC_OUTDOWN_OVERDOWN|BC_IDLE_OVERUP|BC_IDLE_OVERDOWN;
    }
    readToken();
    if(type == RAWDATA) {
	action = text;
	s_buttonaction(current_button_flags, action);
	current_button_flags = 0;
    }
    else
	pushBack();
    return 0;
}
static int c_on_move_out(map_t*args)
{
    char*position = lu(args, "state");
    char*action = "";
    if(!strcmp(position, "pressed")) {
	current_button_flags |= BC_OVERDOWN_OUTDOWN;
    } else if(!strcmp(position, "not_pressed")) {
	current_button_flags |= BC_OVERUP_IDLE;
    } else if(!strcmp(position, "any")) {
	current_button_flags |= BC_OVERDOWN_OUTDOWN|BC_OVERUP_IDLE|BC_OVERDOWN_IDLE;
    }
    readToken();
    if(type == RAWDATA) {
	action = text;
	s_buttonaction(current_button_flags, action);
	current_button_flags = 0;
    }
    else
	pushBack();
    return 0;
}
static int c_on_key(map_t*args)
{
    char*key = lu(args, "key");
    char*action = "";
    if(strlen(key)==1) {
	/* ascii */
	if(key[0]>=32) {
	    current_button_flags |= 0x4000 + (key[0]*0x200);
	} else {
	    syntaxerror("invalid character: %c"+key[0]);
	    return 1;
	}
    } else {
	/* TODO: 
	   <ctrl-x> = 0x200*(x-'a')
	   esc = = 0x3600
	   space = = 0x4000;
	*/
	syntaxerror("invalid key: %s",key);
    }
    readToken();
    if(type == RAWDATA) {
	action = text;
	s_buttonaction(current_button_flags, action);
	current_button_flags = 0;
    }
    else
	pushBack();
    return 0;
}

static int c_edittext(map_t*args) 
{
 //"name font size width height text="" color=black maxlength=0 variable="" @password=0 @wordwrap=0 @multiline=0 @html=0 @noselect=0 @readonly=0"},
    char*name = lu(args, "name");
    char*font = lu(args, "font");
    int size = (int)(1024*parsePercent(lu(args, "size")));
    int width = parseTwip(lu(args, "width"));
    int height = parseTwip(lu(args, "height"));
    char*text  = lu(args, "text");
    RGBA color = parseColor(lu(args, "color"));
    int maxlength = parseInt(lu(args, "maxlength"));
    char*variable = lu(args, "variable");
    char*passwordstr = lu(args, "password");
    char*wordwrapstr = lu(args, "wordwrap");
    char*multilinestr = lu(args, "multiline");
    char*htmlstr = lu(args, "html");
    char*noselectstr = lu(args, "noselect");
    char*readonlystr = lu(args, "readonly");
    char*borderstr = lu(args, "border");

    int flags = 0;
    if(!strcmp(passwordstr, "password")) flags |= ET_PASSWORD;
    if(!strcmp(wordwrapstr, "wordwrap")) flags |= ET_WORDWRAP;
    if(!strcmp(multilinestr, "multiline")) flags |= ET_MULTILINE;
    if(!strcmp(readonlystr, "readonly")) flags |= ET_READONLY;
    if(!strcmp(htmlstr, "html")) flags |= ET_HTML;
    if(!strcmp(noselectstr, "noselect")) flags |= ET_NOSELECT;
    if(!strcmp(borderstr, "border")) flags |= ET_BORDER;

    s_edittext(name, font, size, width, height, text, &color, maxlength, variable, flags);
    return 0;
}

static int c_morphshape(map_t*args) {return fakechar(args);}
static int c_movie(map_t*args) {return fakechar(args);}

static int c_texture(map_t*args) {return 0;}

static int c_action(map_t*args) 
{
    readToken();
    if(type != RAWDATA) {
	syntaxerror("colon (:) expected");
    }

    s_action(text);
   
    return 0;
}

static struct {
    char*command;
    command_func_t* func;
    char*arguments;
} arguments[] =
{{"flash", c_flash, "bbox=autocrop background=black version=5 fps=50 name=!default! @compress=default"},
 {"frame", c_frame, "n=<plus>1 name= @cut=no"},
 // "import" type stuff
 {"swf", c_swf, "name filename"},
 {"shape", c_swf, "name filename"},
 {"jpeg", c_image, "name filename quality=80%"},
 {"png", c_image, "name filename"},
 {"movie", c_movie, "name filename"},
 {"sound", c_sound, "name filename"},
 {"font", c_font, "name filename"},
 {"soundtrack", c_soundtrack, "filename"},
 {"quicktime", c_quicktime, "url"},

    // generators of primitives

 {"point", c_point, "name x=0 y=0"},
 {"gradient", c_gradient, "name @radial=0 rotate=0"},
 {"outline", c_outline, "name format=simple"},
 {"textshape", c_textshape, "name font size=100% text"},

    // character generators
 {"box", c_primitive, "name width height color=white line=1 @fill=none"},
 {"circle", c_primitive, "name r color=white line=1 @fill=none"},
 {"filled", c_primitive, "name outline color=white line=1 @fill=none"},

 {"egon", c_egon, "name vertices color=white line=1 @fill=none"},
 {"text", c_text, "name text font size=100% color=white"},
 {"edittext", c_edittext, "name font size=100% width height text="" color=white maxlength=0 variable="" @password=0 @wordwrap=0 @multiline=0 @html=0 @noselect=0 @readonly=0 @border=0"},
 {"morphshape", c_morphshape, "name start end"},
 {"button", c_button, "name"},
    {"show", c_show,             "name x=0 y=0 red=+0 green=+0 blue=+0 alpha=+0 luminance= scale= scalex= scaley= pivot= pin= shear= rotate= ratio= above= below= as="},
    {"on_press", c_on_press, "position=inside"},
    {"on_release", c_on_release, "position=anywhere"},
    {"on_move_in", c_on_move_in, "state=not_pressed"},
    {"on_move_out", c_on_move_out, "state=not_pressed"},
    {"on_key", c_on_key, "key=any"},
 
    // control tags
 {"play", c_play, "name loop=0 @nomultiple=0"},
 {"stop", c_stop, "name= "},
 {"nextframe", c_nextframe, "name"},
 {"previousframe", c_previousframe, "name"},

    // object placement tags
 {"put", c_put,             "<i> x=0 y=0 red=+0 green=+0 blue=+0 alpha=+0 luminance= scale= scalex= scaley= pivot= pin= shear= rotate= ratio= above= below="},
 {"startclip", c_startclip, "<i> x=0 y=0 red=+0 green=+0 blue=+0 alpha=+0 luminance= scale= scalex= scaley= pivot= pin= shear= rotate= ratio= above= below="},
 {"change", c_change,   "name x= y= red= green= blue= alpha= luminance= scale= scalex= scaley= pivot= pin= shear= rotate= ratio= above= below="},
 {"arcchange", c_arcchange,   "name pivot= angle= red= green= blue= alpha= luminance= scale= scalex= scaley= pivot= pin= shear= rotate= ratio= above= below="},
 {"qchange", c_qchange, "name x= y= red= green= blue= alpha= luminance= scale= scalex= scaley= pivot= pin= shear= rotate= ratio= above= below="},
 {"jump", c_jump,       "name x= y= red= green= blue= alpha= luminance= scale= scalex= scaley= pivot= pin= shear= rotate= ratio= above= below="},
 {"del", c_del, "name"},
    // virtual object placement
 {"texture", c_texture, "<i> x=0 y=0 scale= scalex=100% scaley=100% shear=0 rotate=0"},

    // commands which start a block
//startclip (see above)
 {"sprite", c_sprite, "name"},
 {"action", c_action, ""},

 {"end", c_end, ""}
};


static map_t parseArguments(char*command, char*pattern)
{
    char*x;
    char*d,*e;
   
    string_t name[64];
    string_t value[64];
    int set[64];
    int isboolean[64];
    int pos;
    int len;
    int t;
    string_t t1,t2;
    map_t result;
    map_init(&result);

    string_set(&t1, "commandname");
    string_set(&t2, command);
    map_put(&result, t1, t2);

    if(!pattern || !*pattern)
	return result;

    x = pattern;

    pos = 0;

    if(!strncmp("<i> ", x, 3)) {
	readToken();
	if(type == COMMAND || type == RAWDATA) {
	    pushBack();
	    syntaxerror("character name expected");
	}
	name[pos].str = "instance";
	name[pos].len = 8;
	value[pos].str = text;
	value[pos].len = strlen(text);
	set[pos] = 1;
	pos++;

	if(type == ASSIGNMENT)
	    readToken();

	name[pos].str = "character";
	name[pos].len = 9;
	value[pos].str = text;
	value[pos].len = strlen(text);
	set[pos] = 1;
	pos++;

	x+=4;
    }

    while(*x) {
	isboolean[pos] = (x[0] =='@');
	if(isboolean[pos])
	    x++;

	d = strchr(x, ' ');
	e = strchr(x, '=');
	if(!d)
	    d=&x[strlen(x)];
	set[pos] = 0;

	if(!e || d<e) { 
	    // no default
	    name[pos].str = x;
	    name[pos].len = d-x;
	    value[pos].str = 0;
	    value[pos].len = 0;
	} else {
	    name[pos].str = x;
	    name[pos].len = e-x;
	    value[pos].str = e+1;
	    value[pos].len = d-e-1;
	}
	pos++;
	if(!*d) break;
	x=d+1;
    }
    len = pos;

/*    for(t=0;t<len;t++) {
	printf("(%d) %s=%s %s\n", t, strdup_n(name[t], namelen[t]), strdup_n(value[t], valuelen[t]),
		isboolean[t]?"(boolean)":"");
    }*/

    while(1) {
	readToken();
	if(type == RAWDATA || type == COMMAND) {
	    pushBack();
	    break;
	}

	// first, search for boolean arguments
	for(pos=0;pos<len;pos++)
	{
	    if(!set[pos] && isboolean[pos] && !strncmp(name[pos].str, text, name[pos].len>textlen?name[pos].len:textlen)) {
		set[pos] = 1;
		if(type == ASSIGNMENT)
		    readToken();
		value[pos].str = text;
		value[pos].len = strlen(text);
		/*printf("setting boolean parameter %s (to %s)\n",
			strdup_n(name[pos], namelen[pos]),
			strdup_n(value[pos], valuelen[pos]));*/
		break;
	    }
	}

	// second, search for normal arguments
	if(pos==len)
	for(pos=0;pos<len;pos++)
	{
	    if((type == ASSIGNMENT && !strncmp(name[pos].str, text, name[pos].len>textlen?name[pos].len:textlen)) ||
	       (type != ASSIGNMENT && !set[pos])) {
		if(set[pos]) {
		    syntaxerror("value %s set twice (old value:%s)", text, strdup_n(value[pos].str, value[pos].len));
		}
		if(type == ASSIGNMENT)
		    readToken();
		set[pos] = 1;
		value[pos].str = text;
		value[pos].len = strlen(text);
#if 0//def DEBUG
		printf("setting parameter %s (to %s)\n",
			strdup_n(name[pos].str, name[pos].len),
			strdup_n(value[pos].str, value[pos].len));
#endif
		break;
	    }
	}
	if(pos==len) {
	    syntaxerror("don't know what to do with \"%s\". (All parameters for .%s already set)", text, command);
	}
    }
#if 0//def DEBUG
    for(t=0;t<len;t++) {
	printf("%s=%s\n", strdup_n(name[t].str, name[t].len), strdup_n(value[t].str, value[t].len));
    }
#endif
    for(t=0;t<len;t++) {
	if(value[t].str && value[t].str[0] == '*') {
	    //relative default- take value from some other parameter
	    int s;
	    for(s=0;s<len;s++) {
		if(value[s].len == value[t].len-1 &&
		   !strncmp(&value[t].str[1], value[s].str, value[s].len))
		    value[t].str = value[s].str;
	    }
	}
	if(value[t].str == 0) {
	    pushBack();
	    syntaxerror("value for parameter %s missing (no default)", strdup_n(name[t].str, name[t].len));
	}
    }

    /* ok, now construct the dictionary from the parameters */

    for(t=0;t<len;t++) 
    {
	map_put(&result, name[t], value[t]);
    }
    return result;
}
static void parseArgumentsForCommand(char*command)
{
    int t;
    map_t args;
    int nr = -1;
    msg("<verbose> parse Command: %s (line %d)", command, line);

    for(t=0;t<sizeof(arguments)/sizeof(arguments[0]);t++) {
	if(!strcmp(arguments[t].command, command)) {

	    /* ugly hack- will be removed soon (once documentation and .sc generating
	       utilities have been changed) */
	    if(!strcmp(command, "swf") && !stackpos) {
		warning("Please use .flash instead of .swf- this will be mandatory soon");	
		command = "flash";
		t = 0;
	    }

	    args = parseArguments(command, arguments[t].arguments);
	    nr = t;
	    break;
	}
    }
    if(nr<0)
	syntaxerror("command %s not known", command);
   
    // catch missing .flash directives at the beginning of a file
    if(strcmp(command, "flash") && !stackpos)
    {
	syntaxerror("No movie defined- use .flash first");
    }

#ifdef DEBUG
    printf(".%s\n", command);fflush(stdout);
    map_dump(&args, stdout, "\t");fflush(stdout);
#endif

    (*arguments[nr].func)(&args);

    /*if(!strcmp(command, "button") ||
       !strcmp(command, "action")) {
	while(1) {
	    readToken();
	    if(type == COMMAND) {
		if(!strcmp(text, "end"))
		    break;
		else {
		    pushBack();
		    break;
		}
	    }
	}
    }*/

    map_clear(&args);
    return;
}

int main (int argc,char ** argv)
{ 
    int t;
    processargs(argc, argv);
    initLog(0,-1,0,0,-1,verbose);

    if(!filename) {
	args_callback_usage(argv[0]);
	exit(1);
    }
    
    file = generateTokens(filename);
    if(!file) {
	printf("parser returned error.\n");
	return 1;
    }
    pos=0;
    t=0;

    while(!noMoreTokens()) {
	readToken();
	if(type != COMMAND)
	    syntaxerror("command expected");
	parseArgumentsForCommand(text);
    }

    s_close();
    freeTokens(file);

    return 0;
}

