/* swfc.c
   Compiles swf code (.sc) files into .swf files.

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <errno.h>
#define logf logarithmf // logf is also used by ../lib/log.h
#include <math.h>
#undef logf
#include "../config.h"
#include "../lib/rfxswf.h"
#include "../lib/log.h"
#include "../lib/args.h"
#include "q.h"
#include "parser.h"

//#define DEBUG

static char * filename = 0;
static char * outputname = "output.swf";
static int verbose = 2;
static int override_outputname = 0;

static struct options_t options[] =
{
 {"o","output"},
 {"v","verbose"},
 {"V","version"},
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
void args_callback_usage(char*name)
{
    printf("Usage: %s [-o filename] file.wav\n", name);
    printf("\t-v , --verbose\t\t\t Be more verbose\n");
    printf("\t-o , --output filename\t\t set output filename (default: output.swf)\n");
    printf("\t-V , --version\t\t\t Print program version and exit\n");
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
   int type; //0=swf, 1=sprite, 2=clip

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

} stack[256];
static int stackpos = 0;

static dictionary_t characters;
static char idmap[65536];
static TAG*tag = 0; //current tag

static int id; //current character id
static int currentframe; //current frame in current level
static SRECT currentrect; //current bounding box in current level
static U16 currentdepth;
static dictionary_t instances;
static dictionary_t fonts;

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
    p->pin.x = 1; p->pin.y = 0;
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

static MATRIX s_instancepos(instance_t*i, parameters_t*p)
{
    MATRIX m;
    SRECT r;
    makeMatrix(&m, p);
    r = swf_TurnRect(i->character->size, &m);
    if(currentrect.xmin == 0 && currentrect.ymin == 0 && 
       currentrect.xmax == 0 && currentrect.ymax == 0)
	currentrect = r;
    else
	swf_ExpandRect2(&currentrect, &r);
    return m;
}

void s_swf(char*name, SRECT r, int version, int fps, int compress)
{
    SWF*swf = (SWF*)malloc(sizeof(SWF));
    RGBA rgb;

    if(stackpos)
	syntaxerror(".swf blocks can't be nested");

    memset(swf, 0, sizeof(swf));
    swf->fileVersion = version;
    swf->movieSize = r;
    swf->frameRate = fps;
    swf->firstTag = tag = swf_InsertTag(0, ST_SETBACKGROUNDCOLOR);
    swf->compressed = compress;
    rgb.r = 0x00;rgb.g = 0x00;rgb.b = 0x00;
    swf_SetRGB(tag,&rgb);
    
    if(stackpos==sizeof(stack)/sizeof(stack[0]))
	syntaxerror("too many levels of recursion");
    
    dictionary_init(&characters);
    dictionary_init(&instances);
    dictionary_init(&fonts);

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

static void s_endSprite()
{
    SRECT r = currentrect;
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
    stackpos--;

    swf = stack[stackpos].swf;
    filename = stack[stackpos].filename;
    
    tag = swf_InsertTag(tag, ST_SHOWFRAME);
    tag = swf_InsertTag(tag, ST_END);

    swf_OptimizeTagOrder(swf);

    if(!(swf->movieSize.xmax-swf->movieSize.xmin) || !(swf->movieSize.ymax-swf->movieSize.ymin))
	swf->movieSize = currentrect; /* "autocrop" */

    fi = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);
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
    dictionary_clear(&fonts);

    swf_FreeTags(swf);
    free(swf);
    free(filename);
}
void s_close()
{
    if(stackpos) {
	if(stack[stackpos-1].type == 0)
	    syntaxerror("End of file encountered in .swf block");
	if(stack[stackpos-1].type == 1)
	    syntaxerror("End of file encountered in .sprite block");
	if(stack[stackpos-1].type == 2)
	    syntaxerror("End of file encountered in .clip block");
    }
}

int s_getframe()
{
    return currentframe;
}

void s_frame(int nr)
{
    int t;
    for(t=currentframe;t<nr;t++) {
	tag = swf_InsertTag(tag, ST_SHOWFRAME);
    }
    currentframe = nr;
}
	
RGBA black={r:0,g:0,b:0,a:0};
void s_box(char*name, int width, int height, RGBA color, int linewidth, RGBA fill, int dofill)
{
    SRECT r;
    SHAPE* s;
    int ls1,fs1=0;
    tag = swf_InsertTag(tag, ST_DEFINESHAPE);
    swf_ShapeNew(&s);
    ls1 = swf_ShapeAddLineStyle(s,linewidth,&color);
    if(dofill)
	fs1 = swf_ShapeAddSolidFillStyle(s,&fill);
    swf_SetU16(tag,id);
    r.xmin = -linewidth-linewidth/2;
    r.ymin = -linewidth-linewidth/2;
    r.xmax = width+linewidth+linewidth/2;
    r.ymax = height+linewidth+linewidth/2;
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

void s_circle(char*name, int r, RGBA color, int linewidth, RGBA fill, int dofill)
{
    SRECT rect;
    SHAPE* s;
    int ls1,fs1=0;
    tag = swf_InsertTag(tag, ST_DEFINESHAPE);
    swf_ShapeNew(&s);
    ls1 = swf_ShapeAddLineStyle(s,linewidth,&color);
    if(dofill)
	fs1 = swf_ShapeAddSolidFillStyle(s,&fill);
    swf_SetU16(tag,id);
    rect.xmin = -linewidth-linewidth/2;
    rect.ymin = -linewidth-linewidth/2;
    rect.xmax = 2*r+linewidth+linewidth/2;
    rect.ymax = 2*r+linewidth+linewidth/2;

    swf_SetRect(tag,&rect);
    swf_SetShapeHeader(tag,s);
    swf_ShapeSetAll(tag,s,0,0,ls1,fs1,0);
    swf_ShapeSetCircle(tag, s, r,r,r,r);
    swf_ShapeSetEnd(tag);
    swf_ShapeFree(s);
   
    s_addcharacter(name, id, tag, rect);
    incrementid();
}

void s_textshape(char*name, char*fontname, char*_text, RGBA color, int linewidth, RGBA fill, int dofill)
{
    SRECT rect;
    SHAPE* s;
    int ls1,fs1=0;
    int g;
    U8*text = (U8*)_text;

    SWFFONT*font;
    font = dictionary_lookup(&fonts, fontname);
    if(!font)
	syntaxerror("font \"%s\" not known!", fontname);
    if(!dofill)
	syntaxerror("textshapes must be filled", fontname);

    if(text[0] >= font->maxascii || font->ascii2glyph[text[0]]<0) {
	warning("no character 0x%02x (%c) in font \"%s\"", text[0], text[0], fontname);
	s_box(name, 0, 0, black, 20, black, 0);
	return;
    }
    g = font->ascii2glyph[text[0]];
    rect = font->layout->bounds[g];

    swf_ShapeNew(&s);
    ls1 = swf_ShapeAddLineStyle(s,linewidth,&color);
    if(dofill)
	fs1 = swf_ShapeAddSolidFillStyle(s,&fill);
     
    tag = swf_InsertTag(tag, ST_DEFINESHAPE);
    swf_SetU16(tag,id);
    swf_SetRect(tag, &rect);
    swf_SetShapeStyles(tag, s);
    swf_SetSimpleShape(tag, font->glyph[g].shape);
    swf_ShapeFree(s);
    
    s_addcharacter(name, id, tag, rect);
    incrementid();
}
void s_text(char*name, char*fontname, char*text, int size, RGBA color)
{
    SRECT r;

    SWFFONT*font;
    font = dictionary_lookup(&fonts, fontname);
    if(!font)
	syntaxerror("font \"%s\" not known!", fontname);
    
    tag = swf_InsertTag(tag, ST_DEFINETEXT2);
    swf_SetU16(tag, id);
    if(!font->numchars) {
	s_box(name, 0, 0, black, 20, black, 0);
	return;
    }
    r = swf_SetDefineText(tag, font, &color, text, size);
   
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
    int f;
    SWF swf;
    SWFFONT* font;
    f = open(filename,O_RDONLY);
    if (f<0) { 
	warning("Couldn't open file \"%s\": %s", filename, strerror(errno));
	font = (SWFFONT*)malloc(sizeof(SWFFONT));
	memset(font, 0, sizeof(SWFFONT));
	dictionary_put2(&fonts, name, font);
	return;
    }
    font = 0;
    if (swf_ReadSWF(f,&swf)>=0) { 
	swf_FontExtract(&swf, 0x4e46, &font);
	swf_FreeTags(&swf);
    }
    close(f);
    if (font==0) { 
	syntaxerror("File \"%s\" isn't a valid rfxswf font file", filename);
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

    font->id = id;
    tag = swf_InsertTag(tag, ST_DEFINEFONT2);
    swf_FontSetDefine2(tag, font);
    incrementid();

    if(dictionary_lookup(&fonts, name))
	syntaxerror("font %s defined twice", name);
    dictionary_put2(&fonts, name, font);
}

void s_shape(char*name, char*filename)
{
    int f;
    SWF swf;
    TAG* ftag;
    SRECT r;
    TAG* s;
    int level = 0;
    U16 cutout[] = {ST_SETBACKGROUNDCOLOR, ST_PROTECT, ST_FREEALL, ST_REFLEX};
    f = open(filename,O_RDONLY);
    if (f<0) { 
	warning("Couldn't open file \"%s\": %s", filename, strerror(errno));
	s_box(name, 0, 0, black, 20, black, 0);
	return;
    }
    if (swf_ReadSWF(f,&swf)<0) { 
	warning("Only SWF files supported in .shape for now. File \"%s\" wasn't SWF.", filename);
	s_box(name, 0, 0, black, 20, black, 0);
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
    m = s_instancepos(i, &p);
    
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
    m = s_instancepos(i, &p);
    
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
    m = s_instancepos(i, &p);

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
    
    m = s_instancepos(i, &p2);
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
	    m = s_instancepos(i, &p); //needed?
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
    char*dot = strchr(str, '.');
    if(!dot) {
	int l=strlen(str);
	int t;
	return parseInt(str)*20;
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
	    return atoi(str)*20;
	if(l==1)
	    return atoi(str)*20+atoi(dot)*2;
	if(l==2)
	    return atoi(str)*20+atoi(dot)/5;
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
    char*names[8] = {"black", "blue", "green", "cyan",
	             "red", "magenta", "yellow", "white"};
    a=255;
    r=g=b=0;
    if(str[0]=='#' && (l==7 || l==9)) {
	if(l == 7 && !(sscanf(str, "#%02x%02x%02x", &r, &g, &b)))
	    return 0;
	if(l == 9 && !(sscanf(str, "#%02x%02x%02x%02x", &r, &g, &b, &a)))
	    return 0;
	color->r = r; color->g = g; color->b = b; color->a = a;
	return 1;
    }
    for(t=0;t<8;t++)
	if(!strcmp(str, names[t])) {
	    if(t&1)
		b = 255;
	    if(t&2)
		g = 255;
	    if(t&4)
		r = 255;
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
	syntaxerror("internal error 2: value %s should be set", name);
    }
    return value;
}

static int c_swf(map_t*args) 
{
    char* name = lu(args, "name");
    char* compressstr = lu(args, "compress");
    SRECT bbox = parseBox(lu(args, "bbox"));
    int version = parseInt(lu(args, "version"));
    int fps = (int)(parseFloat(lu(args, "fps"))*256);
    int compress = 0;
    if(!strcmp(name, "!default!") || override_outputname)
	name = outputname;
    
    if(!strcmp(compressstr, "default"))
	compress = version==6;
    else if(!strcmp(compressstr, "yes") || !strcmp(compressstr, "compress"))
	compress = 1;
    else if(!strcmp(compressstr, "no"))
	compress = 0;
    else syntaxerror("value \"%s\" not supported for the compress argument", compressstr);

    s_swf(name, bbox, version, fps, compress);
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
    MULADD r,g,b,a;
    float oldwidth;
    float oldheight;
    SRECT oldbbox;
    MULADD luminance;
    parameters_t p;

    if(type==5) {
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
    if(type == 1)
	s_change(instance, p);
    if(type == 2)
	s_qchange(instance, p);
    if(type == 3)
	s_jump(instance, p);
    if(type == 4)
	s_startclip(instance, character, p);
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
    int frame;
    if(isRelative(framestr)) {
	frame = s_getframe();
	if(getSign(framestr)<0)
	    syntaxerror("relative frame expressions must be positive");
	frame += parseInt(getOffset(framestr));
    }
    else {
	frame = parseInt(framestr);
	if(s_getframe() >= frame
		&& !(frame==0 && s_getframe()==frame)) // equality is o.k. for frame 0
	    syntaxerror("frame expression must be >%d (is:%s)", s_getframe(), framestr);
    }
    s_frame(frame);
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
    RGBA fill;
    if(!strcmp(command, "circle"))
	type = 1;
    else if(!strcmp(command, "textshape"))
	type = 2;
   
    if(type==0) {
	width = parseTwip(lu(args, "width"));
	height = parseTwip(lu(args, "height"));
    } else if (type==1) {
	r = parseTwip(lu(args, "r"));
    } else if (type==2) {
	text = lu(args, "text");
	font = lu(args, "font");
    }

    if(!strcmp(fillstr, "fill"))
	fillstr = colorstr;
    if(!strcmp(fillstr, "none"))
	dofill = 0;
    if(width<0 || height<0 || linewidth<0 || r<0)
	syntaxerror("values width, height, line, r must be positive");
    if(!dofill || isColor(fillstr)) {
	if(dofill) 
	    fill = parseColor(fillstr);
    } else {
	/* FIXME - texture fill */
	fill.r = fill.g = 0;
	fill.b = fill.a = 255;
	warning("texture fill not supported yet. Filling with black.");
    }
    if(type == 0) s_box(name, width, height, color, linewidth, fill, dofill);
    else if(type==1) s_circle(name, r, color, linewidth, fill, dofill);
    else if(type==2) s_textshape(name, font, text, color, linewidth, fill, dofill);
    return 0;
}

static int c_shape(map_t*args) 
{
    char*name = lu(args, "name");
    char*filename = lu(args, "filename");
    s_shape(name, filename);
    return 0;
}

static int c_font(map_t*args) 
{
    char*name = lu(args, "name");
    char*filename = lu(args, "filename");
    s_font(name, filename);
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

int fakechar(map_t*args)
{
    char*name = lu(args, "name");
    s_box(name, 0, 0, black, 20, black, 0);
    return 0;
}
static int c_circle(map_t*args) {return fakechar(args);}

static int c_egon(map_t*args) {return fakechar(args);}
static int c_button(map_t*args) {return fakechar(args);}
static int c_edittext(map_t*args) {return fakechar(args);}

static int c_morphshape(map_t*args) {return fakechar(args);}
static int c_image(map_t*args) {return fakechar(args);}
static int c_movie(map_t*args) {return fakechar(args);}
static int c_sound(map_t*args) {return fakechar(args);}

static int c_play(map_t*args) {return 0;}
static int c_stop(map_t*args) {return 0;}

static int c_soundtrack(map_t*args) {return 0;}
static int c_buttonsounds(map_t*args) {return 0;}
static int c_buttonput(map_t*args) {return 0;}
static int c_texture(map_t*args) {return 0;}
static int c_action(map_t*args) {return 0;}

static struct {
    char*command;
    command_func_t* func;
    char*arguments;
} arguments[] =
{{"swf", c_swf, "bbox=autocrop version=5 fps=50 name=!default! @compress=default"},
 {"frame", c_frame, "n=<plus>1"},

    // "import" type stuff
 {"shape", c_shape, "name filename"},
 {"morphshape", c_morphshape, "name start end"},
 {"jpeg", c_image, "name filename quality=80%"},
 {"png", c_image, "name filename"},
 {"movie", c_movie, "name filename"},
 {"sound", c_sound, "name filename"},
 {"font", c_font, "name filename"},
 {"soundtrack", c_soundtrack, "filename"},

    // character generators
 {"box", c_primitive, "name width height color=white line=1 @fill=none"},
 {"circle", c_primitive, "name r color=white line=1 @fill=none"},
 {"textshape", c_primitive, "name text font color=white line=1 @fill=none"},
 {"egon", c_egon, "name vertices color=white line=1 @fill=none"},
 {"button", c_button, "name shape over=*shape press=*shape area=*shape"},
 {"text", c_text, "name text font size=100% color=white"},
 {"edittext", c_edittext, "name font size width height text="" color=black maxlength=0 variable="" @password=0 @wordwrap=0 @multiline=0 @html=0 @noselect=0 @readonly=0"},
 
 {"buttonsounds", c_buttonsounds, "name press=0 release=0 enter=0 leave=0"},

    // control tags
 {"play", c_play, "sound loop=0 @nomultiple=0"},
 {"stop", c_stop, "sound"},

    // object placement tags
 {"put", c_put,             "<i> x=0 y=0 red=+0 green=+0 blue=+0 alpha=+0 luminance= scale= scalex= scaley= pivot= pin= shear= rotate= above= below="},
 {"startclip", c_startclip, "<i> x=0 y=0 red=+0 green=+0 blue=+0 alpha=+0 luminance= scale= scalex= scaley= pivot= pin= shear= rotate= above= below="},
 {"change", c_change,   "name x= y= red= green= blue= alpha= luminance= scale= scalex= scaley= pivot= pin= shear= rotate= above= below="},
 {"arcchange", c_arcchange,   "name pivot= angle= red= green= blue= alpha= luminance= scale= scalex= scaley= pivot= pin= shear= rotate= above= below="},
 {"qchange", c_qchange, "name x= y= red= green= blue= alpha= luminance= scale= scalex= scaley= pivot= pin= shear= rotate= above= below="},
 {"jump", c_jump,       "name x= y= red= green= blue= alpha= luminance= scale= scalex= scaley= pivot= pin= shear= rotate= above= below="},
 {"del", c_del, "name"},
    // virtual object placement
 {"buttonput", c_buttonput, "<i> x=0 y=0 red=+0 green=+0 blue=+0 alpha=+0 luminance= scale= scalex=100% scaley=100% shear=0 rotate=0 above= below="},
 {"texture", c_texture, "<i> x=0 y=0 scale= scalex=100% scaley=100% shear=0 rotate=0"},
 {"point", c_point, "name x=0 y=0"},

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
	if(type == COMMAND || type == LABEL) {
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
	printf("(%d) %s=%s %s\n", t, strndup(name[t], namelen[t]), strndup(value[t], valuelen[t]),
		isboolean[t]?"(boolean)":"");
    }*/

    while(1) {
	readToken();
	if(type == LABEL || type == COMMAND) {
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
			strndup(name[pos], namelen[pos]),
			strndup(value[pos], valuelen[pos]));*/
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
		    syntaxerror("value %s set twice (old value:%s)", text, strndup(value[pos].str, value[pos].len));
		}
		if(type == ASSIGNMENT)
		    readToken();
		set[pos] = 1;
		value[pos].str = text;
		value[pos].len = strlen(text);
#if 0//def DEBUG
		printf("setting parameter %s (to %s)\n",
			strndup(name[pos].str, name[pos].len),
			strndup(value[pos].str, value[pos].len));
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
	printf("%s=%s\n", strndup(name[t].str, name[t].len), strndup(value[t].str, value[t].len));
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
	    syntaxerror("value for parameter %s missing (no default)", strndup(name[t].str, name[t].len));
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
    for(t=0;t<sizeof(arguments)/sizeof(arguments[0]);t++) {
	if(!strcmp(arguments[t].command, command)) {
	    args = parseArguments(command, arguments[t].arguments);
	    nr = t;
	}
    }
    if(nr<0)
	syntaxerror("command %s not known", command);

#ifdef DEBUG
    printf(".%s\n", command);fflush(stdout);
    map_dump(&args, stdout, "\t");fflush(stdout);
#endif

    (*arguments[nr].func)(&args);

    if(!strcmp(command, "button") ||
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
    }

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

