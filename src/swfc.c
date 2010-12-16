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
#include "../lib/mp3.h"
#include "../lib/wav.h"
#include "parser.h"
#include "../lib/png.h"
#include "swfc-feedback.h"
#include "swfc-interpolation.h"
#include "swfc-history.h"

//#define DEBUG
static char * outputname = "output.swf";
static int verbose = 2;
static int optimize = 0;
static int override_outputname = 0;
static int do_cgi = 0;
static int change_sets_all = 0;
static int do_exports = 0;
static char * mainclass = "";

static struct options_t options[] = {
{"h", "help"},
{"V", "version"},
{"C", "cgi"},
{"v", "verbose"},
{"o", "output"},
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
    else if(!strcmp(name, "C")) {
	do_cgi = 1;
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
    printf("-C , --cgi                     Output to stdout (for use in CGI environments)\n");
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

enum
{
    PT_PUT = 0,
    PT_CHANGE = 1,
    PT_SCHANGE = 2,
    PT_MOVE = 3,
    PT_SMOVE = 4,
    PT_SWEEP = 5,
    PT_JUMP = 6,
    PT_STARTCLIP = 7,
    PT_BUTTON = 8
};

// ------------------------------ swf routines ----------------------------
struct _character;
static struct level
{
   int type; //0=swf, 1=sprite, 2=clip, 3=button

   /* for swf (0): */
   SWF*swf;
   char*filename;
   char as3;

   /* for sprites (1): */
   TAG*tag;
   U16 id;
   char*name;
   char*as3name;
   U16 olddepth;
   int oldframe;
   dict_t oldinstances;
   SRECT oldrect;
   TAG* cut;

   SRECT scalegrid;

} stack[256];
static int stackpos = 0;

static dict_t characters;
static dict_t images;
static dict_t textures;
static dict_t outlines;
static dict_t gradients;
static dict_t filters;
static dict_t interpolations;
static char idmap[65536];
static TAG*tag = 0; //current tag

static int id; //current character id
static int currentframe; //current frame in current level
static SRECT currentrect; //current bounding box in current level
static U16 currentdepth;
static dict_t instances;
static dict_t fonts;
static dict_t sounds;
static dict_t fontUsage;

typedef struct _parameters {
    int x,y;
    float scalex, scaley;
    CXFORM cxform;
    float rotate;
    float shear;
    SPOINT pivot;
    SPOINT pin;
    U8 blendmode; //not interpolated
    FILTERLIST* filters;
    U16 set; // bits indicating wether a parameter was set in the c_placement function
    U16 flags; // bits to toggle anything you may care to implement as a toggle
    int noinstancename;
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
    history_t* history;
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

typedef struct _filter {
    FILTER filter;
} filter_t;

typedef struct _texture {
    FILLSTYLE fs;
} texture_t;

char* interpolationFunctions[] = {"linear", \
        "quadIn", "quadOut", "quadInOut", \
        "cubicIn", "cubicOut", "cubicInOut", \
        "quartIn", "quartOut", "quartInOut", \
        "quintIn", "quintOut", "quintInOut", \
        "circleIn", "circleOut", "circleInOut", \
        "exponentialIn", "exponentialOut", "exponentialInOut", \
        "sineIn", "sineOut", "sineInOut", \
        "elasticIn", "elasticOut", "elasticInOut", \
        "backIn", "backOut", "backInOut", \
        "bounceIn", "bounceOut", "bounceInOut", \
        "fastBounceIn", "fastBounceOut", "fastBounceInOut"};

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
    i->history = history_new();
}

static void instance_free(instance_t* i)
{
    history_free(i->history);
    free(i);
}

static instance_t* instance_new()
{
    instance_t*c;
    c = (instance_t*)malloc(sizeof(instance_t));
    instance_init(c);
    return c;
}

static void free_instance(void* i)
{
    instance_free((instance_t*)i);
}

static void free_font(void* f)
{
    swf_FontFree((SWFFONT*)f);
}

static void gradient_free(GRADIENT* grad)
{
    free(grad->ratios);
    free(grad->rgba);
    free(grad);
}

static void free_gradient(void* grad)
{
    gradient_free((GRADIENT*) grad);
}

static void outline_free(outline_t* o)
{
    free(o->shape->data);
    free(o->shape);
    free(o);
}

static void free_outline(void* o)
{
    outline_free((outline_t*)o);
}

static void freeDictionaries()
{
    dict_free_all(&instances, 1, free_instance);
    dict_free_all(&characters, 1, free);
    dict_free_all(&images, 1, free);
    dict_free_all(&textures, 1, free);
    dict_free_all(&outlines, 1, free_outline);
    dict_free_all(&gradients, 1, free_gradient);
    dict_free_all(&filters, 1, free);
    dict_free_all(&fonts, 1, free_font);
    dict_free_all(&sounds, 1, free);
    dict_free_all(&interpolations, 1, free);
    cleanUp = 0;
}

static void freeFontDictionary()
{
    dict_free_all(&fonts, 1, free_font);
}

static void incrementid()
{
    while(id<65536 && idmap[id]) {
        id++;
    }
    if(id>=65536)
	syntaxerror("Out of character ids.");
    idmap[id] = 1;
}

static void s_addcharacter(const char*name, U16 id, TAG*ctag, SRECT r)
{
    if(dict_lookup(&characters, name))
        syntaxerror("character %s defined twice", name);
    character_t* c = character_new();

    c->definingTag = ctag;
    c->id = id;
    c->size = r;
    dict_put(&characters, name, c);

    if(do_exports) {
	tag = swf_InsertTag(tag, ST_NAMECHARACTER);
	swf_SetU16(tag, id);
	swf_SetString(tag, name);
	tag = swf_InsertTag(tag, ST_EXPORTASSETS);
	swf_SetU16(tag, 1);
	swf_SetU16(tag, id);
	swf_SetString(tag, name);
    }
}
static void s_addimage(const char*name, U16 id, TAG*ctag, SRECT r)
{
    if(dict_lookup(&images, name))
        syntaxerror("image %s defined twice", name);

    character_t* c = character_new();
    c->definingTag = ctag;
    c->id = id;
    c->size = r;
    dict_put(&images, name, c);
}
static instance_t* s_addinstance(const char*name, character_t*c, U16 depth)
{
    if(dict_lookup(&instances, name))
        syntaxerror("object %s defined twice", name);
    instance_t* i = instance_new();
    i->character = c;
    i->depth = depth;
    //swf_GetMatrix(0, &i->matrix);
    dict_put(&instances, name, i);
    return i;
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
    p->blendmode = 0;
    p->filters = 0;
    p->noinstancename = 0;
    swf_GetCXForm(0, &p->cxform, 1);
}

static void makeMatrix(MATRIX*m, parameters_t*p)
{
    SPOINT h;
    float sx,r1,r0,sy;

    /*	      /sx r1\ /x\
     *	      \r0 sy/ \y/
     */

    sx =  p->scalex*cos(p->rotate/360*2*M_PI);
    r1 = -p->scalex*sin(p->rotate/360*2*M_PI)+sx*p->shear;
    r0 =  p->scaley*sin(p->rotate/360*2*M_PI);
    sy =  p->scaley*cos(p->rotate/360*2*M_PI)+r0*p->shear;

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

void initBuiltIns()
{
    interpolation_t* new;
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_LINEAR;
    dict_put(&interpolations, "linear", new);

    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_QUAD_IN;
    new->slope = 0;
    dict_put(&interpolations, "quadIn", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_QUAD_OUT;
    new->slope = 0;
    dict_put(&interpolations, "quadOut", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_QUAD_IN_OUT;
    new->slope = 0;
    dict_put(&interpolations, "quadInOut", new);

    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_CUBIC_IN;
    new->slope = 0;
    dict_put(&interpolations, "cubicIn", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_CUBIC_OUT;
    new->slope = 0;
    dict_put(&interpolations, "cubicOut", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_CUBIC_IN_OUT;
    new->slope = 0;
    dict_put(&interpolations, "cubicInOut", new);

    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_QUART_IN;
    new->slope = 0;
    dict_put(&interpolations, "quartIn", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_QUART_OUT;
    new->slope = 0;
    dict_put(&interpolations, "quartOut", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_QUART_IN_OUT;
    new->slope = 0;
    dict_put(&interpolations, "quartInOut", new);

    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_QUINT_IN;
    new->slope = 0;
    dict_put(&interpolations, "quintIn", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_QUINT_OUT;
    new->slope = 0;
    dict_put(&interpolations, "quintOut", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_QUINT_IN_OUT;
    new->slope = 0;
    dict_put(&interpolations, "quintInOut", new);

    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_CIRCLE_IN;
    dict_put(&interpolations, "circleIn", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_CIRCLE_OUT;
    dict_put(&interpolations, "circleOut", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_CIRCLE_IN_OUT;
    dict_put(&interpolations, "circleInOut", new);

    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_EXPONENTIAL_IN;
    dict_put(&interpolations, "exponentialIn", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_EXPONENTIAL_OUT;
    dict_put(&interpolations, "exponentialOut", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_EXPONENTIAL_IN_OUT;
    dict_put(&interpolations, "exponentialInOut", new);

    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_SINE_IN;
    dict_put(&interpolations, "sineIn", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_SINE_OUT;
    dict_put(&interpolations, "sineOut", new);
    new = (interpolation_t*)malloc(sizeof(interpolation_t));
    new->function = IF_SINE_IN_OUT;
    dict_put(&interpolations, "sineInOut", new);

    RGBA c;
    memset(&c, 0, sizeof(RGBA));
    gradient_t* noGradient = (gradient_t*)malloc(sizeof(gradient_t));
    noGradient->gradient.ratios = (U8*)malloc(16 * sizeof(U8));
    noGradient->gradient.rgba = (RGBA*)malloc(16 * sizeof(RGBA));
    noGradient->gradient.num = 2;
    noGradient->gradient.rgba[0] = c;
    noGradient->gradient.ratios[0] = 0;
    noGradient->gradient.rgba[1] = c;
    noGradient->gradient.ratios[1] = 255;
    noGradient->radial = 0;
    noGradient->rotate = 0;
    dict_put(&gradients, "no_gradient", noGradient);

    noFilters = 0;
// put a no_filters entry in the filters dictionary to provoce a message when a user tries
// to define a no_filters filter. The real filter=no_filters case is handled in parseFilters.
    FILTER* dummy = (FILTER*)malloc(sizeof(FILTER));
    dict_put(&filters, "no_filters", dummy);
    noBlur = (FILTER_BLUR*) swf_NewFilter(FILTERTYPE_BLUR);
    noBlur->passes = 1;
    dict_put(&filters, "no_blur", noBlur);
    noBevel = (FILTER_BEVEL*) swf_NewFilter(FILTERTYPE_BEVEL);
    noBevel->passes = 1;
    noBevel->composite = 1;
    dict_put(&filters, "no_bevel", noBevel);
    noDropshadow = (FILTER_DROPSHADOW*) swf_NewFilter(FILTERTYPE_DROPSHADOW);
    noDropshadow->passes = 1;
    noDropshadow->composite = 1;
    dict_put(&filters, "no_dropshadow", noDropshadow);
    noGradientGlow = (FILTER_GRADIENTGLOW*) swf_NewFilter(FILTERTYPE_GRADIENTGLOW);
    noGradientGlow->passes = 1;
    noGradientGlow->composite = 1;
    noGradientGlow->gradient = &noGradient->gradient;
    dict_put(&filters, "no_gradientglow", noGradientGlow);
}

void s_swf(const char*name, SRECT r, int version, int fps, int compress, RGBA background)
{
    if(stackpos)
        syntaxerror(".swf blocks can't be nested");
    if(stackpos==sizeof(stack)/sizeof(stack[0]))
        syntaxerror("too many levels of recursion");

    SWF*swf = (SWF*)malloc(sizeof(SWF));

    memset(swf, 0, sizeof(swf));
    swf->fileVersion = version;
    swf->movieSize = r;
    swf->frameRate = fps;
    swf->firstTag = tag = swf_InsertTag(0, ST_SETBACKGROUNDCOLOR);
    swf->compressed = compress;
    swf_SetRGB(tag,&background);

    dict_init(&characters, 16);
    dict_init(&images, 16);
    dict_init(&textures, 16);
    dict_init(&outlines, 16);
    dict_init(&gradients, 16);
    dict_init(&filters, 16);
    dict_init(&instances, 16);
    dict_init(&sounds, 16);
    dict_init(&interpolations, 16);
    initBuiltIns();
    cleanUp = &freeDictionaries;

    memset(&stack[stackpos], 0, sizeof(stack[0]));
    stack[stackpos].type = 0;
    stack[stackpos].filename = strdup(name);
    stack[stackpos].swf = swf;
    stack[stackpos].oldframe = -1;
    stackpos++;

    currentframe = 0;
    memset(&currentrect, 0, sizeof(currentrect));
    currentdepth = 1;

    memset(idmap, 0, sizeof(idmap));
    idmap[0]=1; //main movie has ID 0

    incrementid();
}

void s_sprite(const char*name, SRECT*scalegrid, const char*as3name)
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
    stack[stackpos].as3name = strdup(as3name);
    if(scalegrid) {
	stack[stackpos].scalegrid = *scalegrid;
    } else {
	memset(&stack[stackpos].scalegrid, 0, sizeof(SRECT));
    }

    /* FIXME: those four fields should be bundled together */
    dict_init(&instances, 16);
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

void s_button(const char*name, const char*as3name)
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
    stack[stackpos].as3name = strdup(as3name);
    stack[stackpos].oldrect = currentrect;
    memset(&currentrect, 0, sizeof(currentrect));

    stackpos++;
    incrementid();
}
void s_buttonput(const char*character, const char*as, parameters_t p)
{
    character_t* c = dict_lookup(&characters, character);
    MATRIX m;
    int flags = 0;
    const char*o = as,*s = as;
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

void s_buttonaction(int flags, const char*action)
{
    ActionTAG* a = 0;
    if(flags==0) {
	return;
    }
    if(!stackpos || !stack[stackpos-1].tag ||
            stack[stackpos-1].tag->id != ST_DEFINEBUTTON2) {
        syntaxerror("Need to be inside a button for .on_* commands");
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

    if(*stack[stackpos].as3name) {
        tag = swf_InsertTag(tag, ST_SYMBOLCLASS);
        swf_SetU16(tag, 1);
        swf_SetU16(tag, stack[stackpos].id);
        swf_SetString(tag, stack[stackpos].as3name);
    }

    free(stack[stackpos].name);
}

TAG* removeFromTo(TAG*from, TAG*to)
{
    TAG*save = from->prev;
    while(from!=to) {
	TAG*next = from->next;
	if(swf_isAllowedSpriteTag(from))
	    swf_DeleteTag(0, from);
	from = next;
    }
    save->next = 0;
    return save;
}

static int parametersChange(history_t* history, int frame)
{
    int willChange = 0;

    willChange = willChange || history_change(history, frame, "x");
    willChange = willChange || history_change(history, frame, "y");
    willChange = willChange || history_change(history, frame, "scalex");
    willChange = willChange || history_change(history, frame, "scaley");
    willChange = willChange || history_change(history, frame, "cxform.r0");
    willChange = willChange || history_change(history, frame, "cxform.g0");
    willChange = willChange || history_change(history, frame, "cxform.b0");
    willChange = willChange || history_change(history, frame, "cxform.a0");
    willChange = willChange || history_change(history, frame, "cxform.r1");
    willChange = willChange || history_change(history, frame, "cxform.g1");
    willChange = willChange || history_change(history, frame, "cxform.b1");
    willChange = willChange || history_change(history, frame, "cxform.a1");
    willChange = willChange || history_change(history, frame, "rotate");
    willChange = willChange || history_change(history, frame, "shear");
    willChange = willChange || history_change(history, frame, "pivot.x");
    willChange = willChange || history_change(history, frame, "pivot.y");
    willChange = willChange || history_change(history, frame, "pin.x");
    willChange = willChange || history_change(history, frame, "pin.y");
    willChange = willChange || history_change(history, frame, "blendmode");
    willChange = willChange || history_changeFilter(history, frame);

    return willChange;
}

static void free_filterlist(FILTERLIST* f_list)
{
    int i;
    for (i = 0; i < f_list->num; i++)
    {
        if(f_list->filter[i]->type == FILTERTYPE_GRADIENTGLOW)
            gradient_free(((FILTER_GRADIENTGLOW*)f_list->filter[i])->gradient);
        free(f_list->filter[i]);
    }
    free(f_list);
}

static void readParameters(history_t* history, parameters_t* p, int frame)
{
    p->x = history_value(history, frame, "x");
    p->y = history_value(history, frame, "y");
    p->scalex = history_value(history, frame, "scalex");
    p->scaley = history_value(history, frame, "scaley");
    p->cxform.r0 = history_value(history, frame, "cxform.r0");
    p->cxform.g0 = history_value(history, frame, "cxform.g0");
    p->cxform.b0 = history_value(history, frame, "cxform.b0");
    p->cxform.a0 = history_value(history, frame, "cxform.a0");
    p->cxform.r1 = history_value(history, frame, "cxform.r1");
    p->cxform.g1 = history_value(history, frame, "cxform.g1");
    p->cxform.b1 = history_value(history, frame, "cxform.b1");
    p->cxform.a1 = history_value(history, frame, "cxform.a1");
    p->rotate = history_rotateValue(history, frame);
    p->shear = history_value(history, frame, "shear");
    p->pivot.x = history_value(history, frame, "pivot.x");
    p->pivot.y = history_value(history, frame, "pivot.y");
    p->pin.x = history_value(history, frame, "pin.x");
    p->pin.y = history_value(history, frame, "pin.y");
    p->blendmode = history_value(history, frame, "blendmode");
    p->filters = history_filterValue(history, frame);
}

void setPlacement(TAG*tag, U16 id, U16 depth, MATRIX m, const char*name, parameters_t*p, char move)
{
    SWFPLACEOBJECT po;
    FILTERLIST flist;
    swf_GetPlaceObject(NULL, &po);
    po.id = id;
    po.depth = depth;
    po.matrix = m;
    po.cxform = p->cxform;
    po.name = (char*)name;
    po.move = move;
    if(move)
    po.id = 0;
    if(p->blendmode) {
    po.blendmode = p->blendmode;
    }
    if(p->filters)
    	po.filters = p->filters;
    swf_SetPlaceObject(tag, &po);
}

static void writeInstance(void* _i)
{
    instance_t*i = (instance_t*)_i;
    parameters_t p;
    MATRIX m;
    int frame = i->history->firstFrame;
    TAG* tag = i->history->firstTag;
    history_processFlags(i->history);
    while (tag && frame < currentframe)
    {
        frame++;
        while (tag && tag->id != ST_SHOWFRAME)
            tag = tag->next;
        if(parametersChange(i->history, frame))
        {
            readParameters(i->history, &p, frame);
            m = s_instancepos(i->character->size, &p);

            if(p.blendmode || p.filters)
        	tag = swf_InsertTag(tag, ST_PLACEOBJECT3);
            else
        	tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
            setPlacement(tag, 0, i->depth, m, 0, &p, 1);
            if(p.filters)
            	free_filterlist(p.filters);
        } else if(tag) {
            tag = tag->next;
        }
    }
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

static void s_endSprite()
{
    SRECT r = currentrect;

    stackpos--;
    instance_t *i;

    dict_foreach_value(&instances, writeInstance);

    if(stack[stackpos].cut)
	tag = removeFromTo(stack[stackpos].cut, tag);

    // the writeInstance loop above may have inserted tags after what used to be the current tag,
    // so let's make sure 'tag' point to the current tag again.
    while (tag->next)
	tag = tag->next;

    tag = swf_InsertTag(tag, ST_SHOWFRAME);
    tag = swf_InsertTag(tag, ST_END);

    tag = stack[stackpos].tag;
    swf_FoldSprite(tag);

    if(stack[stackpos].scalegrid.xmin | stack[stackpos].scalegrid.ymin |
       stack[stackpos].scalegrid.xmax | stack[stackpos].scalegrid.ymax) 
    {
	tag = swf_InsertTag(tag, ST_DEFINESCALINGGRID);
	swf_SetU16(tag, stack[stackpos].id);
	swf_SetRect(tag, &stack[stackpos].scalegrid);
    }

    if(tag->next != 0)
        syntaxerror("internal error(7)");
    /* TODO: before clearing, prepend "<spritename>." to names and
             copy into old instances dict */
    dict_free_all(&instances, 1, free_instance);

    currentframe = stack[stackpos].oldframe;
    currentrect = stack[stackpos].oldrect;
    currentdepth = stack[stackpos].olddepth;
    instances = stack[stackpos].oldinstances;

    s_addcharacter(stack[stackpos].name, stack[stackpos].id, stack[stackpos].tag, r);

    if(*stack[stackpos].as3name) {
        tag = swf_InsertTag(tag, ST_SYMBOLCLASS);
        swf_SetU16(tag, 1);
        swf_SetU16(tag, stack[stackpos].id);
        swf_SetString(tag, stack[stackpos].as3name);
    }   


    free(stack[stackpos].name);
}

static void s_endSWF()
{
    int fi;
    SWF* swf;
    char*filename;
    char*mc="";

    dict_foreach_value(&instances, writeInstance);

    if(stack[stackpos].cut)
	tag = removeFromTo(stack[stackpos].cut, tag);

    stackpos--;

    swf = stack[stackpos].swf;
    filename = stack[stackpos].filename;

    // the writeInstance loop above may have inserted tags after what used yo be the current tag,
    // so let's make sure 'tag' point to the current tag again.
    while (tag->next)
	tag = tag->next;

    //if(tag->prev && tag->prev->id != ST_SHOWFRAME)
    //    tag = swf_InsertTag(tag, ST_SHOWFRAME);
    tag = swf_InsertTag(tag, ST_SHOWFRAME);

    if(stack[0].as3) {
        TAG*tag = swf->firstTag;
        tag = swf_InsertTag(tag, ST_DOABC);
        void*code = as3_getcode();
        swf_WriteABC(tag, code);
        if(*mainclass)
            mc = mainclass;
        else if(as3_getglobalclass())
            mc = as3_getglobalclass();
        if(*mc) {
            tag = swf_InsertTag(tag, ST_SYMBOLCLASS);
            swf_SetU16(tag, 1);
            swf_SetU16(tag, 0);
            swf_SetString(tag, mc);
        } else {
            warning("no global public MovieClip subclass");
        }
        as3_destroy();
    }

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
	warning("Empty bounding box for movie");
    }

    if(do_cgi || !strcmp(filename, "-"))
	fi = fileno(stdout);
    else
	fi = open(filename, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
    if(fi<0) {
	syntaxerror("couldn't create output file %s", filename);
    }
    if(do_cgi)
	{if(swf_WriteCGI(swf)<0) syntaxerror("WriteCGI() failed.\n");}
    else
	{if(swf_WriteSWF(fi, swf)<0) syntaxerror("WriteSWF() failed.\n");}

    close(fi);

    freeDictionaries();

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

void s_frame(int nr, int cut, const char*name, char anchor)
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
	    if(anchor)
		swf_SetU8(tag, 1); //make this an anchor
	}
    }
    if(nr == 0 && currentframe == 0 && name && *name) {
        tag = swf_InsertTag(tag, ST_FRAMELABEL);
        swf_SetString(tag, name);
	if(anchor)
	    swf_SetU8(tag, 1); //make this an anchor
    }

    if(cut) {
	if(now == tag) {
	    syntaxerror("Can't cut, frame empty");
	}
	stack[stackpos].cut = tag;
    }

    currentframe = nr;
}

int parseColor2(const char*str, RGBA*color);

int addFillStyle(SHAPE*s, SRECT*r, const char*name)
{
    RGBA color;
    character_t*image;
    gradient_t*gradient;
    texture_t*texture;
    if(name[0] == '#') {
	parseColor2(name, &color);
	return swf_ShapeAddSolidFillStyle(s, &color);
    } else if((texture = dict_lookup(&textures, name))) {
	return swf_ShapeAddFillStyle2(s, &texture->fs);
    } else if((image = dict_lookup(&images, name))) {
	MATRIX m;
	swf_GetMatrix(0, &m);
	m.sx = 65536.0*20.0*(r->xmax - r->xmin)/image->size.xmax;
	m.sy = 65536.0*20.0*(r->ymax - r->ymin)/image->size.ymax;
	m.tx = r->xmin;
	m.ty = r->ymin;
	return swf_ShapeAddBitmapFillStyle(s, &m, image->id, 0);
    }  else if((gradient = dict_lookup(&gradients, name))) {
	SRECT r2;
	MATRIX rot,m;
	double ccos,csin;
	swf_GetMatrix(0, &rot);
    	ccos = cos(-gradient->rotate*2*M_PI/360);
    	csin = sin(-gradient->rotate*2*M_PI/360);
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
    }  else if(parseColor2(name, &color)) {
	return swf_ShapeAddSolidFillStyle(s, &color);
    } else {
	syntaxerror("not a color/fillstyle: %s", name);
	return 0;
    }
}

RGBA black={r:0,g:0,b:0,a:0};
void s_box(const char*name, int width, int height, RGBA color, int linewidth, const char*texture)
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
    if(linewidth) {
	linewidth = linewidth>=20?linewidth-20:0;
        ls1 = swf_ShapeAddLineStyle(s,linewidth,&color);
    }
    if(texture)
	fs1 = addFillStyle(s, &r2, texture);

    swf_SetU16(tag,id);
    r.xmin = r2.xmin-linewidth/2;
    r.ymin = r2.ymin-linewidth/2;
    r.xmax = r2.xmax+linewidth/2;
    r.ymax = r2.ymax+linewidth/2;
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

void s_filled(const char*name, const char*outlinename, RGBA color, int linewidth, const char*texture)
{
    SRECT rect,r2;
    SHAPE* s;
    int ls1,fs1=0;
    outline_t* outline;
    outline = dict_lookup(&outlines, outlinename);
    if(!outline) {
	syntaxerror("outline %s not defined", outlinename);
    }
    r2 = outline->bbox;

    tag = swf_InsertTag(tag, ST_DEFINESHAPE3);
    swf_ShapeNew(&s);
    if(linewidth) {
	linewidth = linewidth>=20?linewidth-20:0;
        ls1 = swf_ShapeAddLineStyle(s,linewidth,&color);
    }
    if(texture)
	fs1 = addFillStyle(s, &r2, texture);

    swf_SetU16(tag,id);
    rect.xmin = r2.xmin-linewidth/2;
    rect.ymin = r2.ymin-linewidth/2;
    rect.xmax = r2.xmax+linewidth/2;
    rect.ymax = r2.ymax+linewidth/2;

    swf_SetRect(tag,&rect);
    swf_SetShapeStyles(tag, s);
    swf_ShapeCountBits(s,0,0);
    swf_RecodeShapeData(outline->shape->data, outline->shape->bitlen, outline->shape->bits.fill, outline->shape->bits.line,
                        &s->data,             &s->bitlen,             s->bits.fill,              s->bits.line);
    swf_SetShapeBits(tag, s);
    swf_SetBlock(tag, s->data, (s->bitlen+7)/8);
    swf_ShapeFree(s);

    s_addcharacter(name, id, tag, rect);
    incrementid();
}

void s_circle(const char*name, int r, RGBA color, int linewidth, const char*texture)
{
    SRECT rect,r2;
    SHAPE* s;
    int ls1=0,fs1=0;
    r2.xmin = r2.ymin = 0;
    r2.xmax = 2*r;
    r2.ymax = 2*r;

    tag = swf_InsertTag(tag, ST_DEFINESHAPE3);
    swf_ShapeNew(&s);
    if(linewidth) {
	linewidth = linewidth>=20?linewidth-20:0;
        ls1 = swf_ShapeAddLineStyle(s,linewidth,&color);
    }
    if(texture)
	fs1 = addFillStyle(s, &r2, texture);
    swf_SetU16(tag,id);
    rect.xmin = r2.xmin-linewidth/2;
    rect.ymin = r2.ymin-linewidth/2;
    rect.xmax = r2.xmax+linewidth/2;
    rect.ymax = r2.ymax+linewidth/2;

    swf_SetRect(tag,&rect);
    swf_SetShapeHeader(tag,s);
    swf_ShapeSetAll(tag,s,0,0,ls1,fs1,0);
    swf_ShapeSetCircle(tag, s, r,r,r,r);
    swf_ShapeSetEnd(tag);
    swf_ShapeFree(s);

    s_addcharacter(name, id, tag, rect);
    incrementid();
}

void s_textshape(const char*name, const char*fontname, float size, const char*_text)
{
    int g;
    U8*text = (U8*)_text;
    outline_t* outline;

    SWFFONT*font;
    font = dict_lookup(&fonts, fontname);
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
	swf_DrawText(&draw, font, (int)(size*100), (char*)_text);
	draw.finish(&draw);
	outline->shape = swf_ShapeDrawerToShape(&draw);
	outline->bbox = swf_ShapeDrawerGetBBox(&draw);
	draw.dealloc(&draw);
    }

    if(dict_lookup(&outlines, name))
	syntaxerror("outline %s defined twice", name);
    dict_put(&outlines, name, outline);
}

void s_text(const char*name, const char*fontname, const char*text, int size, RGBA color)
{
    SRECT r;
    MATRIX _m,*m=0;
    SWFFONT*font;
    font = dict_lookup(&fonts, fontname);
    if(!font)
	syntaxerror("font \"%s\" not known!", fontname);

    tag = swf_InsertTag(tag, ST_DEFINETEXT2);
    swf_SetU16(tag, id);
    if(!font->numchars) {
	s_box(name, 0, 0, black, 20, 0);
	return;
    }
    r = swf_SetDefineText(tag, font, &color, (char*)text, size);

    if(stack[0].swf->fileVersion >= 8) {
	tag = swf_InsertTag(tag, ST_CSMTEXTSETTINGS);
	swf_SetU16(tag, id);
	swf_SetU8(tag, /*grid*/(1<<3)|/*flashtype*/0x40);
	swf_SetU32(tag, 0);//thickness
	swf_SetU32(tag, 0);//sharpness
	swf_SetU8(tag, 0);//reserved
    }

    s_addcharacter(name, id, tag, r);
    incrementid();
}

void s_quicktime(const char*name, const char*url)
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

void s_video(const char *name, int width, int height)
{
    SRECT r;

    memset(&r, 0, sizeof(r));

    tag = swf_InsertTag(tag, ST_DEFINEVIDEOSTREAM);
    swf_SetU16(tag, id);
    swf_SetU16(tag, 0); // numframes
    swf_SetU16(tag, width);
    swf_SetU16(tag, height);
    swf_SetU8(tag, 0); // videoflags
    swf_SetU8(tag, 0); // codecid

    s_addcharacter(name, id, tag, r);
    incrementid();
}

void s_edittext(const char*name, const char*fontname, int size, int width, int height, const char*text, RGBA*color, int maxlength, const char*variable, int flags, int align)
{
    SWFFONT*font = 0;
    EditTextLayout layout;
    SRECT r;

    if(fontname && *fontname) {
	flags |= ET_USEOUTLINES;
	font = dict_lookup(&fonts, fontname);
	if(!font)
	    syntaxerror("font \"%s\" not known!", fontname);
    }
    tag = swf_InsertTag(tag, ST_DEFINEEDITTEXT);
    swf_SetU16(tag, id);
    layout.align = align;
    layout.leftmargin = 0;
    layout.rightmargin = 0;
    layout.indent = 0;
    layout.leading = 0;
    r.xmin = 0;
    r.ymin = 0;
    r.xmax = width;
    r.ymax = height;

    swf_SetEditText(tag, flags, r, (char*)text, color, maxlength, font?font->id:0, size, &layout, (char*)variable);

    s_addcharacter(name, id, tag, r);
    incrementid();
}

/* type: either "jpeg" or "png"
 */
void s_image(const char*name, const char*type, const char*filename, int quality)
{
    /* an image is actually two folded: 1st bitmap, 2nd character.
       Both of them can be used separately */

    /* step 1: the bitmap */
    SRECT r;
    int imageID = id;
    unsigned width, height;
    if(!strcmp(type,"jpeg")) {
#ifndef HAVE_JPEGLIB
	warning("no jpeg support compiled in");
	s_box(name, 0, 0, black, 20, 0);
	return;
#else
	tag = swf_InsertTag(tag, ST_DEFINEBITSJPEG2);
	swf_SetU16(tag, imageID);

	if(swf_SetJPEGBits(tag, (char*)filename, quality) < 0) {
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
    } else if(!strcmp(type,"png")) {
	RGBA*data = 0;
	swf_SetU16(tag, imageID);

	png_load(filename, &width, &height, (unsigned char**)&data);

	if(!data) {
	    syntaxerror("Image \"%s\" not found, or contains errors", filename);
	}

	/*tag = swf_AddImage(tag, imageID, data, width, height, quality)*/
	tag = swf_InsertTag(tag, ST_DEFINEBITSLOSSLESS);
	swf_SetU16(tag, imageID);
	swf_SetLosslessImage(tag, data, width, height);
    free(data);

	r.xmin = 0;
	r.ymin = 0;
	r.xmax = width*20;
	r.ymax = height*20;
	s_addimage(name, id, tag, r);
	incrementid();
    } else {
	warning("image type \"%s\" not supported yet!", type);
	s_box(name, 0, 0, black, 20, 0);
	return;
    }

    /* step 2: the character */
    tag = swf_InsertTag(tag, ST_DEFINESHAPE); // todo: should be defineshape2/3 once images can be transparent.(?)
    swf_SetU16(tag, id);
    swf_ShapeSetBitmapRect(tag, imageID, width, height);

    s_addcharacter(name, id, tag, r);
    incrementid();
}

void s_getBitmapSize(const char*name, int*width, int*height)
{
    character_t* image = dict_lookup(&images, name);
    gradient_t* gradient = dict_lookup(&gradients,name);
    if(image) {
	*width = image->size.xmax;
	*height = image->size.ymax;
	return;
    }
    if(gradient) {
	/* internal SWF gradient size */
	if(gradient->radial) {
	    *width = 16384;
	    *height = 16384;
	} else {
	    *width = 32768;
	    *height = 32768;
	}
	return;
    }
    syntaxerror("No such bitmap/gradient: %s", name);
}

void s_texture(const char*name, const char*object, int x, int y, float scalex, float scaley, float rotate, float shear)
{
    if(dict_lookup(&textures, name))
        syntaxerror("texture %s defined twice", name);
    gradient_t* gradient = dict_lookup(&gradients, object);
    character_t* bitmap = dict_lookup(&images, object);
    texture_t* texture = (texture_t*)rfx_calloc(sizeof(texture_t));
    parameters_t p;
    FILLSTYLE*fs = &texture->fs;

    memset(&p, 0, sizeof(parameters_t));

    if(bitmap) {
	fs->type = FILL_TILED;
	fs->id_bitmap = bitmap->id;
    } else if(gradient) {
	fs->type = gradient->radial?FILL_RADIAL:FILL_LINEAR;
	fs->gradient = gradient->gradient;
    }
    p.x = x;p.y = y;p.scalex = scalex;p.scaley = scaley;p.rotate=rotate;p.shear=shear;
    makeMatrix(&fs->m, &p);
    if(gradient && !gradient->radial) {
	MATRIX m = fs->m;
	SPOINT p1,p2;
	m.tx = 0;
	m.ty = 0;
	p1.x = 16384;
	p1.y = 16384;
	p2 = swf_TurnPoint(p1, &m);
	fs->m.tx += p2.x;
	fs->m.ty += p2.y;
    }
    if(bitmap) {
	fs->m.sx *= 20;
	fs->m.sy *= 20;
    }

    dict_put(&textures, name, texture);
}

void s_createfont(const char*name, const char*filename, const char*glyphs, char flashtype)
{
    if(dict_lookup(&fonts, name))
	syntaxerror("font %s defined twice", name);

    SWFFONT* font = swf_LoadFont(filename, flashtype);
    if(font == 0) {
	warning("Couldn't open font file \"%s\"", filename);
	font = (SWFFONT*)malloc(sizeof(SWFFONT));
	memset(font, 0, sizeof(SWFFONT));
	dict_put(&fonts, name, font);
	return;
    }
    swf_FontPrepareForEditText(font);

    if(!strcmp(glyphs, "all")) {
	swf_FontUseAll(font);
	font->use->glyphs_specified = 1;
    } else {
	if(!glyphs[0]) {
	    swf_FontInitUsage(font);
	} else {
	    swf_FontUseUTF8(font, (const U8*)glyphs, 0xffff);
	    font->use->glyphs_specified = 1;
	}
    }
    dict_put(&fonts, name, font);
}

void s_font(const char*name, const char*filename)
{
    SWFFONT* font;
    font = dict_lookup(&fonts, name);
    font->id = id;
    swf_FontReduce_swfc(font);
	
    if(font->version>=3 && stack[0].swf->fileVersion < 8) {
	warning("flashtype not supported for flash versions 8 and below");
    }

    tag = swf_InsertTag(tag, font->version==3?ST_DEFINEFONT3:ST_DEFINEFONT2);
    swf_FontSetDefine2(tag, font);

    if(do_exports) {
	tag = swf_InsertTag(tag, ST_EXPORTASSETS);
	swf_SetU16(tag, 1);
	swf_SetU16(tag, id);
	swf_SetString(tag, name);
    }

    incrementid();
}



typedef struct _sound_t
{
    U16 id;
    TAG*tag;
} sound_t;

void s_sound(const char*name, const char*filename)
{
    struct WAV wav, wav2;
    struct MP3 mp3;
    sound_t* sound;
    U16*samples = NULL;
    unsigned numsamples = 1;
    unsigned blocksize = 1152;
    int is_mp3 = 0;

    if(dict_lookup(&sounds, name))
        syntaxerror("sound %s defined twice", name);

    if(wav_read(&wav, filename))
    {
        int t;
        wav_convert2mono(&wav, &wav2, 44100);
        samples = (U16*)wav2.data;
        numsamples = wav2.size/2;
        free(wav.data);
#ifdef WORDS_BIGENDIAN
	/* swap bytes */
        for(t=0;t<numsamples;t++)
            samples[t] = (samples[t]>>8)&0xff | (samples[t]<<8)&0xff00;
#endif
    }
    else
        if(mp3_read(&mp3, filename))
        {
            fprintf(stderr, "\"%s\" seems to work as a MP3 file...\n", filename);
            blocksize = 1;
            is_mp3 = 1;
        }
        else
        {
            warning("Couldn't read WAV/MP3 file \"%s\"", filename);
            samples = 0;
            numsamples = 0;
        }

    if(numsamples%blocksize != 0)
    {
	// apply padding, so that block is a multiple of blocksize
        int numblocks = (numsamples+blocksize-1)/blocksize;
        int numsamples2;
        U16* samples2;
        numsamples2 = numblocks * blocksize;
        samples2 = malloc(sizeof(U16)*numsamples2);
        memcpy(samples2, samples, numsamples*sizeof(U16));
        memset(&samples2[numsamples], 0, sizeof(U16)*(numsamples2 - numsamples));
        numsamples = numsamples2;
        free(samples);
        samples = samples2;
    }

    tag = swf_InsertTag(tag, ST_DEFINESOUND);
    swf_SetU16(tag, id); //id
    if(is_mp3)
    {
        swf_SetSoundDefineMP3(
                tag, mp3.data, mp3.size,
                mp3.SampRate,
                mp3.Channels,
                mp3.NumFrames);
        mp3_clear(&mp3);
    }
    else
        swf_SetSoundDefine(tag, samples, numsamples);

    if(do_exports) {
	tag = swf_InsertTag(tag, ST_NAMECHARACTER);
	swf_SetU16(tag, id);
	swf_SetString(tag, name);
	tag = swf_InsertTag(tag, ST_EXPORTASSETS);
	swf_SetU16(tag, 1);
	swf_SetU16(tag, id);
	swf_SetString(tag, name);
    }

    sound = (sound_t*)malloc(sizeof(sound_t)); /* mem leak */
    sound->tag = tag;
    sound->id = id;

    dict_put(&sounds, name, sound);

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

float parsePercent(const char*str);
RGBA parseColor(const char*str);

GRADIENT parseGradient(const char*str)
{
    GRADIENT gradient;
    int lastpos = -1;
    const char* p = str;
    memset(&gradient, 0, sizeof(GRADIENT));
    gradient.ratios = rfx_calloc(16*sizeof(U8));
    gradient.rgba = rfx_calloc(16*sizeof(RGBA));

    while(*p)
    {
        char*posstr,*colorstr;
        int pos;
        RGBA color;
        posstr = gradient_getToken(&p);
        if(!*posstr)
        {
            free(posstr);
            break;
        }
        pos = (int)(parsePercent(posstr)*255.0);
        if(pos == lastpos)
            pos++;
        if(!*p)
        {
            rfx_free(gradient.ratios);
            rfx_free(gradient.rgba);
            free(posstr);
            syntaxerror("Error in shape data: Color expected after %s", posstr);
        }
        colorstr = gradient_getToken(&p);
        color = parseColor(colorstr);
        if(gradient.num == 16)
        {
            warning("gradient record too big- max size is 16, rest ignored");
            break;
        }
        gradient.ratios[gradient.num] = pos;
        gradient.rgba[gradient.num] = color;
        gradient.num++;
        free(posstr);
        free(colorstr);
        lastpos = pos;
	}
    return gradient;
}

FILTERLIST* parseFilters(char* list)
{
    if(!strcmp(list, "no_filters"))
    	return 0;
    FILTER* f;
    FILTERLIST* f_list = (FILTERLIST*)malloc(sizeof(FILTERLIST));
    f_list->num = 0;
    char* f_start = list;
    char* f_end;
    while (f_start)
    {
    	f_end = strchr(f_start, ',');
    	if(f_end)
    	    *f_end = '\0';
    	f = dict_lookup(&filters, f_start);
    	if(!f)
    	{
    	    free(f_list);
    	    syntaxerror("unknown filter %s", f_start);
    	}
    	if(f_list->num == 8)
    	{
    	    warning("too many filters in filterlist, no more than 8 please, rest ignored");
    	    break;
    	}
    	f_list->filter[f_list->num] = f;
    	f_list->num++;
    	if(f_end)
    	{
    	    *f_end = ',';
    	    f_start = f_end + 1;
    	}
    	else
    	    f_start = 0;
    }
    return f_list;
}

void s_gradient(const char*name, const char*text, int radial, int rotate)
{
    gradient_t* gradient;
    gradient = malloc(sizeof(gradient_t));
    memset(gradient, 0, sizeof(gradient_t));
    gradient->gradient = parseGradient(text);
    gradient->radial = radial;
    gradient->rotate = rotate;

    dict_put(&gradients, name, gradient);
}

void s_gradientglow(const char*name, const char*gradient, float blurx, float blury,
		    float angle, float distance, float strength, char innershadow,
		    char knockout, char composite, char ontop, int passes)
{
    if(dict_lookup(&filters, name))
        syntaxerror("filter %s defined twice", name);

    gradient_t* g = dict_lookup(&gradients, gradient);
    if(!g)
	syntaxerror("unknown gradient %s", gradient);

    composite = 1;

    FILTER_GRADIENTGLOW* filter = rfx_calloc(sizeof(FILTER_GRADIENTGLOW));
    filter->type = FILTERTYPE_GRADIENTGLOW;
    filter->gradient = &g->gradient;
    filter->blurx = blurx;
    filter->blury = blury;
    filter->strength = strength;
    filter->angle = angle;
    filter->distance = distance;
    filter->innershadow = innershadow;
    filter->knockout = knockout;
    filter->composite = composite;
    filter->ontop = ontop;
    filter->passes = passes;

    dict_put(&filters, name, filter);
}

void s_dropshadow(const char*name, RGBA color, double blurx, double blury, double angle, double distance, double strength, char innershadow, char knockout, char composite, int passes)
{
    if(dict_lookup(&filters, name))
        syntaxerror("filter %s defined twice", name);

    composite = 1;
    FILTER_DROPSHADOW* filter = rfx_calloc(sizeof(FILTER_DROPSHADOW));
    filter->type = FILTERTYPE_DROPSHADOW;
    filter->color= color;
    filter->blurx = blurx;
    filter->blury = blury;
    filter->strength = strength;
    filter->angle = angle;
    filter->distance = distance;
    filter->innershadow = innershadow;
    filter->knockout = knockout;
    filter->composite = composite;
    filter->passes = passes;

    dict_put(&filters, name, filter);
}

void s_bevel(const char*name, RGBA shadow, RGBA highlight, double blurx, double blury, double angle, double distance, double strength, char innershadow, char knockout, char composite, char ontop, int passes)
{
    if(dict_lookup(&filters, name))
        syntaxerror("filter %s defined twice", name);

    composite = 1;
    FILTER_BEVEL* filter = rfx_calloc(sizeof(FILTER_BEVEL));
    filter->type = FILTERTYPE_BEVEL;
    filter->shadow = shadow;
    filter->highlight = highlight;
    filter->blurx = blurx;
    filter->blury = blury;
    filter->strength = strength;
    filter->angle = angle;
    filter->distance = distance;
    filter->innershadow = innershadow;
    filter->knockout = knockout;
    filter->composite = composite;
    filter->ontop = ontop;
    filter->passes = passes;

    dict_put(&filters, name, filter);
}

void s_blur(const char*name, double blurx, double blury, int passes)
{
    if(dict_lookup(&filters, name))
        syntaxerror("filter %s defined twice", name);

    FILTER_BLUR* filter = rfx_calloc(sizeof(FILTER_BLUR));
    filter->type = FILTERTYPE_BLUR;
    filter->blurx = blurx;
    filter->blury = blury;
    filter->passes = passes;

    dict_put(&filters, name, filter);
}

void s_action(const char*text)
{
    if(stack[0].swf->fileVersion < 9) {
        ActionTAG* a = 0;
        a = swf_ActionCompile(text, stack[0].swf->fileVersion);
        if(!a) {
            swf_ActionFree(a);
            syntaxerror("Couldn't compile ActionScript");
        }
        tag = swf_InsertTag(tag, ST_DOACTION);
        swf_ActionSet(tag, a);
        swf_ActionFree(a);
    } else {
        as3_parse_bytearray(stack[0].filename, text, strlen(text));
        stack[0].as3 = 1;
    }
}

void s_initaction(const char*character, const char*text)
{
    ActionTAG* a = 0;
    character_t*c = 0;
    a = swf_ActionCompile(text, stack[0].swf->fileVersion);
    if(!a)
    {
        swf_ActionFree(a);
        syntaxerror("Couldn't compile ActionScript");
    }

    c = (character_t*)dict_lookup(&characters, character);

    tag = swf_InsertTag(tag, ST_DOINITACTION);
    swf_SetU16(tag, c->id);
    swf_ActionSet(tag, a);

    swf_ActionFree(a);
}

int s_swf3action(const char*name, const char*action)
{
    ActionTAG* a = 0;
    instance_t* object = 0;
    if(name)
	object = (instance_t*)dict_lookup(&instances, name);
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

void s_outline(const char*name, const char*format, const char*source)
{
    if(dict_lookup(&outlines, name))
        syntaxerror("outline %s defined twice", name);

    outline_t* outline;

    drawer_t draw;
    SHAPE* shape;
    SHAPE2* shape2;
    SRECT bounds;

    //swf_Shape10DrawerInit(&draw, 0);
    swf_Shape11DrawerInit(&draw, 0);

    draw_string(&draw, source);
    draw.finish(&draw);
    shape = swf_ShapeDrawerToShape(&draw);
    bounds = swf_ShapeDrawerGetBBox(&draw);
    draw.dealloc(&draw);

    outline = (outline_t*)rfx_calloc(sizeof(outline_t));
    outline->shape = shape;
    outline->bbox = bounds;

    dict_put(&outlines, name, outline);
}

int s_playsound(const char*name, int loops, int nomultiple, int stop)
{
    sound_t* sound;
    SOUNDINFO info;
    if(!name)
	return 0;
    sound = dict_lookup(&sounds, name);
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

void s_includeswf(const char*name, const char*filename, const char*as3name)
{
    int f;
    SWF swf;
    TAG* ftag;
    SRECT r;
    TAG* s;
    int level = 0;
    U16 cutout[] = {ST_SETBACKGROUNDCOLOR, ST_PROTECT, ST_FREEALL, ST_REFLEX};
    f = open(filename,O_RDONLY|O_BINARY);
    if(f<0) {
	warning("Couldn't open file \"%s\": %s", filename, strerror(errno));
	s_box(name, 0, 0, black, 20, 0);
	return;
    }
    if(swf_ReadSWF(f,&swf)<0) {
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
    swf_SetU16(tag, swf.frameCount);

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

	if(ftag->id != ST_SETBACKGROUNDCOLOR) {
	    /* We simply dump all tags right after the sprite
	       header, relying on the fact that swf_OptimizeTagOrder() will
	       sort things out for us later.
	       We also rely on the fact that the imported SWF is well-formed.
	     */
	    tag = swf_InsertTag(tag, ftag->id);
	    swf_SetBlock(tag, ftag->data, ftag->len);
	}

	ftag = ftag->next;
    }
    if(!ftag)
	syntaxerror("Included file %s contains errors", filename);
    tag = swf_InsertTag(tag, ST_END);

    swf_FreeTags(&swf);

    s_addcharacter(name, id, tag, r);
    
    if(as3name) {
        tag = swf_InsertTag(tag, ST_SYMBOLCLASS);
        swf_SetU16(tag, 1);
        swf_SetU16(tag, id);
        swf_SetString(tag, as3name);
    }
    incrementid();
}
SRECT s_getCharBBox(const char*name)
{
    character_t* c = dict_lookup(&characters, name);
    if(!c) syntaxerror("character '%s' unknown(2)", name);
    return c->size;
}
SRECT s_getInstanceBBox(const char*name)
{
    instance_t * i = dict_lookup(&instances, name);
    character_t * c;
    if(!i) syntaxerror("instance '%s' unknown(4)", name);
    c = i->character;
    if(!c) syntaxerror("internal error(5)");
    return c->size;
}
void s_getParameters(const char*name, parameters_t* p)
{
    instance_t * i = dict_lookup(&instances, name);
    if(!i)
    	syntaxerror("instance '%s' unknown(10)", name);
    if(change_sets_all)
        readParameters(i->history, p, currentframe);
    else
    	*p = i->parameters;
}

void setStartparameters(instance_t* i, parameters_t* p, TAG* tag)
{
    history_begin(i->history, "x", currentframe, tag, p->x);
    history_begin(i->history, "y", currentframe, tag, p->y);
    history_begin(i->history, "scalex", currentframe, tag, p->scalex);
    history_begin(i->history, "scaley", currentframe, tag, p->scaley);
    history_begin(i->history, "cxform.r0", currentframe, tag, p->cxform.r0);
    history_begin(i->history, "cxform.g0", currentframe, tag, p->cxform.g0);
    history_begin(i->history, "cxform.b0", currentframe, tag, p->cxform.b0);
    history_begin(i->history, "cxform.a0", currentframe, tag, p->cxform.a0);
    history_begin(i->history, "cxform.r1", currentframe, tag, p->cxform.r1);
    history_begin(i->history, "cxform.g1", currentframe, tag, p->cxform.g1);
    history_begin(i->history, "cxform.b1", currentframe, tag, p->cxform.b1);
    history_begin(i->history, "cxform.a1", currentframe, tag, p->cxform.a1);
    history_begin(i->history, "rotate", currentframe, tag, p->rotate);
    history_begin(i->history, "shear", currentframe, tag, p->shear);
    history_begin(i->history, "pivot.x", currentframe, tag, p->pivot.x);
    history_begin(i->history, "pivot.y", currentframe, tag, p->pivot.y);
    history_begin(i->history, "pin.x", currentframe, tag, p->pin.x);
    history_begin(i->history, "pin.y", currentframe, tag, p->pin.y);
    history_begin(i->history, "blendmode", currentframe, tag, p->blendmode);
    history_beginFilter(i->history, currentframe, tag, p->filters);
    history_begin(i->history, "flags", currentframe, tag, 0);
}

void s_startclip(const char*instance, const char*character, parameters_t p)
{
    character_t* c = dict_lookup(&characters, character);
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

    stack[stackpos].tag = tag;
    stack[stackpos].type = 2;
    stackpos++;

    setStartparameters(i, &p, tag);
    currentdepth++;
}
void s_endClip()
{
    SWFPLACEOBJECT p;
    stackpos--;
    swf_SetTagPos(stack[stackpos].tag, 0);
    swf_GetPlaceObject(stack[stackpos].tag, &p);
    p.clipdepth = currentdepth;
    //p.name = 0;
    swf_ClearTag(stack[stackpos].tag);
    swf_SetPlaceObject(stack[stackpos].tag, &p);
    currentdepth++;
}

void s_put(const char*instance, const char*character, parameters_t p)
{
    character_t* c = dict_lookup(&characters, character);
    instance_t* i;
    MATRIX m;
    if(!c)
        syntaxerror("character %s not known (in .put %s=%s)", character, instance, character);

    i = s_addinstance(instance, c, currentdepth);
    i->parameters = p;
    m = s_instancepos(i->character->size, &p);

    if(p.blendmode || p.filters)
    {
        if(stack[0].swf->fileVersion < 8)
        {
            if(p.blendmode)
                warning("blendmodes only supported for flash version>=8");
            else
                warning("filters only supported for flash version>=8");
        }
        tag = swf_InsertTag(tag, ST_PLACEOBJECT3);
	}
    else
        tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
    setPlacement(tag, c->id, currentdepth, m, p.noinstancename ? NULL : instance, &p, 0);
    setStartparameters(i, &p, tag);
    currentdepth++;
}

void recordChanges(history_t* history, parameters_t p, int changeFunction, interpolation_t* inter)
{
    if(p.set & SF_X)
        history_remember(history, "x", currentframe, changeFunction, p.x, inter);
    if(p.set & SF_Y)
        history_remember(history, "y", currentframe, changeFunction, p.y, inter);
    if(p.set & SF_SCALEX)
        history_remember(history, "scalex", currentframe, changeFunction, p.scalex, inter);
    if(p.set & SF_SCALEY)
        history_remember(history, "scaley", currentframe, changeFunction, p.scaley, inter);
    if(p.set & SF_CX_R)
    {
        history_remember(history, "cxform.r0", currentframe, changeFunction, p.cxform.r0, inter);
        history_remember(history, "cxform.r1", currentframe, changeFunction, p.cxform.r1, inter);
    }
    if(p.set & SF_CX_G)
    {
        history_remember(history, "cxform.g0", currentframe, changeFunction, p.cxform.g0, inter);
        history_remember(history, "cxform.g1", currentframe, changeFunction, p.cxform.g1, inter);
    }
    if(p.set & SF_CX_B)
    {
        history_remember(history, "cxform.b0", currentframe, changeFunction, p.cxform.b0, inter);
        history_remember(history, "cxform.b1", currentframe, changeFunction, p.cxform.b1, inter);
    }
    if(p.set & SF_CX_A)
    {
        history_remember(history, "cxform.a0", currentframe, changeFunction, p.cxform.a0, inter);
        history_remember(history, "cxform.a1", currentframe, changeFunction, p.cxform.a1, inter);
    }
    if(p.set & SF_ROTATE)
        history_remember(history, "rotate", currentframe, changeFunction, p.rotate, inter);
    if(p.set & SF_SHEAR)
        history_remember(history, "shear", currentframe, changeFunction, p.shear, inter);
    if(p.set & SF_PIVOT)
    {
        history_remember(history, "pivot.x", currentframe, changeFunction, p.pivot.x, inter);
        history_remember(history, "pivot.y", currentframe, changeFunction, p.pivot.y, inter);
    }
    if(p.set & SF_PIN)
    {
        history_remember(history, "pin.x", currentframe, changeFunction, p.pin.x, inter);
        history_remember(history, "pin.y", currentframe, changeFunction, p.pin.y, inter);
    }
    if(p.set & SF_BLEND)
        history_remember(history, "blendmode", currentframe, changeFunction, p.blendmode, inter);
    if(p.set & SF_FILTER)
        history_rememberFilter(history, currentframe, changeFunction, p.filters, inter);
}

void s_jump(const char* instance, parameters_t p)
{
    instance_t* i = dict_lookup(&instances, instance);
    if(!i)
        syntaxerror("instance %s not known", instance);
    recordChanges(i->history, p, CF_JUMP, 0);
}

void s_change(const char*instance, parameters_t p, interpolation_t* inter)
{
    instance_t* i = dict_lookup(&instances, instance);
    if(!i)
        syntaxerror("instance %s not known", instance);
    recordChanges(i->history, p, CF_CHANGE, inter);
}

void s_sweep(const char* instance, parameters_t p, float radius, int clockwise, int short_arc, interpolation_t* inter)
{
    instance_t* i = dict_lookup(&instances, instance);
    if(!i)
        syntaxerror("instance %s not known", instance);
    history_rememberSweep(i->history, currentframe, p.x, p.y, radius, clockwise, short_arc, inter);
}

void s_toggle(const char* instance, U16 flagsOn, U16 flagsOff)
{
    instance_t* i = dict_lookup(&instances, instance);
    if(!i)
        syntaxerror("instance %s not known", instance);
    U16 flags = (U16)history_value(i->history, currentframe, "flags");
    flags |= flagsOn;
    flags &= flagsOff;
    history_remember(i->history, "flags", currentframe, CF_JUMP, flags, 0);
}

void s_delinstance(const char*instance)
{
    instance_t* i = dict_lookup(&instances, instance);
    if(!i)
        syntaxerror("instance %s not known", instance);
    writeInstance(i);
    tag = swf_InsertTag(tag, ST_REMOVEOBJECT2);
    swf_SetU16(tag, i->depth);
    dict_del(&instances, instance);
    free(i);
}

void s_schange(const char*instance, parameters_t p, interpolation_t* inter)
{
    instance_t* i = dict_lookup(&instances, instance);
    if(!i)
        syntaxerror("instance %s not known", instance);
    recordChanges(i->history, p, CF_SCHANGE, inter);
}

void s_end()
{
    if(!stackpos)
	syntaxerror(".end unexpected");
    switch (stack[stackpos-1].type)
    {
        case 0:
            s_endSWF();
            break;
        case 1:
            s_endSprite();
            break;
        case 2:
            s_endClip();
            break;
        case 3:
            s_endButton();
            break;
        default:
            syntaxerror("internal error 1");
    }
}

// ------------------------------------------------------------------------

typedef int command_func_t(map_t*args);

SRECT parseBox(const char*str)
{
    SRECT r = {0,0,0,0};
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
float parseFloat(const char*str)
{
    return atof(str);
}
int parseInt(const char*str)
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
static double parseRawTwip(const char*str)
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
	return sign*parseInt(str);
    } else {
	char* old = strdup(str);
	int l=strlen(dot+1);
	const char*s;
	*dot++ = 0;
	for(s=str;s<dot-1;s++) {
            if(*s<'0' || *s>'9')
            {
                free(old);
                syntaxerror("Not a coordinate: \"%s\"", str);
            }
        }
        for(s=dot;*s;s++) {
            if(*s<'0' || *s>'9')
            {
                free(old);
                syntaxerror("Not a coordinate: \"%s\"", str);
            }
        }
	if(l>2 || (l==2 && (dot[1]!='0' && dot[1]!='5'))) {
	    dot[1] = ((dot[1]-0x30)/5)*5 + 0x30;
	    dot[2] = 0;
	    l=2;
	    warning("precision loss: %s converted to twip: %s.%s", old, str, dot);
	}
	free(old);
	if(l==0)
	    return sign*(atoi(str));
	if(l==1)
	    return sign*(atoi(str)+0.1*atoi(dot));
	if(l==2)
	    return sign*(atoi(str)+0.01*atoi(dot));
    }
    return 0;
}

static dict_t defines;
static int defines_initialized = 0;
static mem_t define_values;

static double parseNameOrTwip(const char*s)
{
    int l = 0;
    double v;
    if(defines_initialized) {
        l = PTR_AS_INT(dict_lookup(&defines, s));
    }
    if(l) {
        return *(int*)&define_values.buffer[l-1];
    } else {
        return parseRawTwip(s);
    }
}

/* automatically generated by yiyiyacc, http://www.quiss.org/yiyiyacc/ */
static double parseExpression(char*s)
{
    int chr2index[256];
    memset(chr2index, -1, sizeof(chr2index));
    chr2index['+'] = 0;
    chr2index['-'] = 1;
    chr2index['*'] = 2;
    chr2index['/'] = 3;
    chr2index['('] = 5;
    chr2index[')'] = 6;
    chr2index['\0'] = 7;

    int stackpos = 1;
    int stack[256];
    double values[256];
    stack[0]=0;
    values[0]=0;
    int accept = 18;
    int left[10]={11,8,8,8,8,9,9,9,10,10}; //production left side
    int plen[10]={1,3,2,3,1,3,3,1,1,3}; //production size
    int table[18][12] = {
        {0, 4, 0, 0, 5, 6, 0, 0, 1, 2, 3, 0},
        {7, 8, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0},
        {-4, -4, 9, 10, 0, 0, -4, -4, 0, 0, 0, 0},
        {-7, -7, -7, -7, 0, 0, -7, -7, 0, 0, 0, 0},
        {0, 0, 0, 0, 5, 6, 0, 0, 0, 11, 3, 0},
        {-8, -8, -8, -8, 0, 0, -8, -8, 0, 0, 0, 0},
        {0, 4, 0, 0, 5, 6, 0, 0, 12, 2, 3, 0},
        {0, 0, 0, 0, 5, 6, 0, 0, 0, 13, 3, 0},
        {0, 0, 0, 0, 5, 6, 0, 0, 0, 14, 3, 0},
        {0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 15, 0},
        {0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 16, 0},
        {-2, -2, 9, 10, 0, 0, -2, -2, 0, 0, 0, 0},
        {7, 8, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0},
        {-1, -1, 9, 10, 0, 0, -1, -1, 0, 0, 0, 0},
        {-3, -3, 9, 10, 0, 0, -3, -3, 0, 0, 0, 0},
        {-5, -5, -5, -5, 0, 0, -5, -5, 0, 0, 0, 0},
        {-6, -6, -6, -6, 0, 0, -6, -6, 0, 0, 0, 0},
        {-9, -9, -9, -9, 0, 0, -9, -9, 0, 0, 0, 0}};

    char*p = s;
    while(1) {
        char*pnext = p+1;
        int action;
        double value = 0;
        if(!stackpos) {
            fprintf(stderr, "Error in expression\n");
            return 0.0;
        }

        if(chr2index[*p]<0) {
            action = table[stack[stackpos-1]][4];
            if(action>0) {
                while(chr2index[*pnext]<0) 
                    pnext++;
                char save = *pnext;
                *pnext = 0;
                value = parseNameOrTwip(p);
                *pnext = save;
            }
        } else {
            action = table[stack[stackpos-1]][chr2index[*p]];
        }

        if(action == accept) {
            return values[stack[stackpos-1]];
        } else if(action>0) { // shift
            if(stackpos>254) {
                fprintf(stderr, "Stack overflow while parsing expression\n");
                return 0.0;
            }
            values[stackpos]=value;
            stack[stackpos++]=action;
            p=pnext;
        } else if(action<0) { // reduce
            stackpos-=plen[-action];
            stack[stackpos] = table[stack[stackpos-1]][left[-action]];
            switch(-action) {
              case 1:
                values[stackpos] = values[stackpos+0] + values[stackpos+2];
              break;
              case 2:
                values[stackpos] = 0 - values[stackpos+1];
              break;
              case 3:
                values[stackpos] = values[stackpos+0] - values[stackpos+2];
              break;
              case 5:
                values[stackpos] = values[stackpos+0] * values[stackpos+2];
              break;
              case 6:
                values[stackpos] = values[stackpos+0] / values[stackpos+2];
              break;
              case 9:
                values[stackpos] = values[stackpos+1];
              break;
            }
            stackpos++;
        } else {
            fprintf(stderr, "Syntax error in expression\n");
            return 0.0;
        }
    }
}

int parseTwip(const char*str)
{
    char*str2 = (char*)str;
    int v = (int)(parseExpression(str2)*20);
    return v;
}

int parseArc(const char* str)
{
    if(!strcmp(str, "short"))
    	return 1;
    if(!strcmp(str, "long"))
    	return 0;
    syntaxerror("invalid value for the arc parameter: %s", str);
    return 1;
}

int parseDir(const char* str)
{
    if(!strcmp(str, "clockwise"))
    	return 1;
    if(!strcmp(str, "counterclockwise"))
    	return 0;
    syntaxerror("invalid value for the dir parameter: %s", str);
    return 1;
}

int isPoint(const char*str)
{
    if(strchr(str, '('))
	return 1;
    else
	return 0;
}

SPOINT parsePoint(const char*str)
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

int parseColor2(const char*str, RGBA*color)
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
    int len=strlen(str);
    int alpha = 255;
    if(strchr(str, '/')) {
	len = strchr(str, '/')-str;
	sscanf(str+len+1,"%02x", &alpha);
    }
    for(t=0;t<sizeof(colors)/sizeof(colors[0]);t++)
	if(!strncmp(str, colors[t].name, len)) {
	    r = colors[t].r;
	    g = colors[t].g;
	    b = colors[t].b;
	    a = alpha;
	    color->r = r; color->g = g; color->b = b; color->a = a;
	    return 1;
	}
    return 0;

}
RGBA parseColor(const char*str)
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

MULADD parseMulAdd(const char*str)
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

float parsePxOrPercent(const char*fontname, const char*str)
{
    int l = strlen(str);
    if(strchr(str, '%'))
	return parsePercent(str);
    if(l>2 && str[l-2]=='p' && str[l-1]=='t') {
	float p = atof(str);
	return p/64.0; /*64 = FT_SUBPIXELS- see ../lib/modules/swffont.c */
    }
    syntaxerror("Expression '%s' is neither a point size (?pt) nor a percentage (?%)", str);
    return 0;
}

float parsePercent(const char*str)
{
    int l = strlen(str);
    if(!l)
	return 1.0;
    if(str[l-1]=='%') {
	return atof(str)/100.0;
    }
    syntaxerror("Expression '%s' is not a percentage", str);
    return 0;
}
int isPercent(const char*str)
{
    return str[strlen(str)-1]=='%';
}
int parseNewSize(const char*str, int size)
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

static const char* lu(map_t* args, char*name)
{
    const char* value = map_lookup(args, name);
    if(!value) {
	map_dump(args, stdout, "");
	syntaxerror("internal error 2: value %s should be set", name);
    }
    return value;
}

static int c_flash(map_t*args)
{
    const char* filename = map_lookup(args, "filename");
    const char* compressstr = lu(args, "compress");
    const char* change_modestr = lu(args, "change-sets-all");
    const char* exportstr = lu(args, "export");
    SRECT bbox = parseBox(lu(args, "bbox"));
    int version = parseInt(lu(args, "version"));
    int fps = (int)(parseFloat(lu(args, "fps"))*256);
    RGBA color = parseColor(lu(args, "background"));
    int compress = 0;

    if(!filename || !*filename) {
	/* for compatibility */
	filename = map_lookup(args, "name");
	if(!filename || !*filename) {
	    filename = 0;
	} else {
	    //msg("<warning> line %d: .flash name=... is deprecated, use .flash filename=...", line);
	    msg("<notice> line %d: .flash name=... is deprecated, use .flash filename=...", line);
	}
    }

    if(!filename || override_outputname)
	filename = outputname;

    if(!strcmp(compressstr, "default"))
	compress = version>=6;
    else if(!strcmp(compressstr, "yes") || !strcmp(compressstr, "compress"))
	compress = 1;
    else if(!strcmp(compressstr, "no"))
	compress = 0;
    else syntaxerror("value \"%s\" not supported for the compress argument", compressstr);

    if(!strcmp(change_modestr, "yes"))
	change_sets_all = 1;
    else
	if(strcmp(change_modestr, "no"))
	    syntaxerror("value \"%s\" not supported for the change-sets-all argument", change_modestr);

    do_exports=atoi(exportstr);
    mainclass=strdup(lu(args, "mainclass"));

    s_swf(filename, bbox, version, fps, compress, color);
    return 0;
}
int isRelative(const char*str)
{
    return !strncmp(str, "<plus>", 6) ||
	   !strncmp(str, "<minus>", 7);
}
const char* getOffset(const char*str)
{
    if(!strncmp(str, "<plus>", 6))
	return str+6;
    if(!strncmp(str, "<minus>", 7))
	return str+7;
    syntaxerror("internal error (347)");
    return 0;
}
int getSign(const char*str)
{
    if(!strncmp(str, "<plus>", 6))
	return 1;
    if(!strncmp(str, "<minus>", 7))
	return -1;
    syntaxerror("internal error (348)");
    return 0;
}

static dict_t points;
static mem_t mpoints;
static int points_initialized = 0;

static int c_interpolation(map_t *args)
{
    int i;
    const char* name = lu(args, "name");
    if(dict_lookup(&interpolations, name))
        syntaxerror("interpolation %s defined twice", name);

    interpolation_t* inter = (interpolation_t*)malloc(sizeof(interpolation_t));
    const char* functionstr = lu(args, "function");
    inter->function = 0;
    for (i = 0; i < sizeof(interpolationFunctions) / sizeof(interpolationFunctions[0]); i++)
        if(!strcmp(functionstr,interpolationFunctions[i]))
        {
            inter->function = i + 1;
            break;
        }
    if(!inter->function)
        syntaxerror("unkown interpolation function %s", functionstr);
    inter->speed = parseFloat(lu(args, "speed"));
    inter->amplitude = parseTwip(lu(args, "amplitude"));
    inter->growth = parseFloat(lu(args, "growth"));
    inter->bounces = parseInt(lu(args, "bounces"));
    inter->damping = parseFloat(lu(args, "damping"));
    inter->slope = parseFloat(lu(args, "slope"));

    dict_put(&interpolations, name, inter);
    return 0;
}

SPOINT getPoint(SRECT r, const char*name)
{
    int l=0;
    if(!strcmp(name, "center")) {
        SPOINT p;
        p.x = (r.xmin + r.xmax)/2;
        p.y = (r.ymin + r.ymax)/2;
        return p;
    }
    if(!strcmp(name, "bottom-center")) {
        SPOINT p;
        p.x = (r.xmin + r.xmax)/2;
        p.y = r.ymax;
        return p;
    }
    if(!strcmp(name, "top-center")) {
        SPOINT p;
        p.x = (r.xmin + r.xmax)/2;
        p.y = r.ymin;
        return p;
    }
    if(!strcmp(name, "top-left")) {
        SPOINT p;
        p.x = r.xmin;
        p.y = r.ymin;
        return p;
    }
    if(!strcmp(name, "top-right")) {
        SPOINT p;
        p.x = r.xmax;
        p.y = r.ymin;
        return p;
    }
    if(!strcmp(name, "bottom-right")) {
        SPOINT p;
        p.x = r.xmax;
        p.y = r.ymax;
        return p;
    }
    if(!strcmp(name, "bottom-left")) {
        SPOINT p;
        p.x = r.xmin;
        p.y = r.ymax;
        return p;
    }
    if(!strcmp(name, "left-center")) {
        SPOINT p;
        p.x = r.xmin;
        p.y = (r.ymin + r.ymax)/2;
        return p;
    }
    if(!strcmp(name, "right-center")) {
        SPOINT p;
        p.x = r.xmax;
        p.y = (r.ymin + r.ymax)/2;
        return p;
    }


    if(points_initialized)
        l = PTR_AS_INT(dict_lookup(&points, name));
    if(l==0) {
        syntaxerror("Invalid point: \"%s\".", name);
    }
    return *(SPOINT*)&mpoints.buffer[l-1];
}


static int texture2(const char*name, const char*object, map_t*args, int errors)
{
    SPOINT pos,size;
    const char*xstr = map_lookup(args, "x");
    const char*ystr = map_lookup(args, "y");
    const char*widthstr = map_lookup(args, "width");
    const char*heightstr = map_lookup(args, "height");
    const char*scalestr = map_lookup(args, "scale");
    const char*scalexstr = map_lookup(args, "scalex");
    const char*scaleystr = map_lookup(args, "scaley");
    const char*rotatestr = map_lookup(args, "rotate");
    const char* shearstr = map_lookup(args, "shear");
    const char* radiusstr = map_lookup(args, "r");
    float x=0,y=0;
    float scalex = 1.0, scaley = 1.0;
    float rotate=0, shear=0;
    float r = 0;
    if(!*xstr && !*ystr) {
	if(errors)
	    syntaxerror("x and y must be set");
	return 0;
    }
    if(*scalestr && (*scalexstr || *scaleystr)) {
	syntaxerror("scale and scalex/scaley can't both be set");
	return 0;
    }
    if((*widthstr || *heightstr) && *radiusstr) {
	syntaxerror("width/height and radius can't both be set");
    }
    if(*radiusstr) {
	widthstr = radiusstr;
	heightstr = radiusstr;
    }
    if(!*xstr) xstr="0";
    if(!*ystr) ystr="0";
    if(!*rotatestr) rotatestr="0";
    if(!*shearstr) shearstr="0";

    if(*scalestr) {
	scalex = scaley = parsePercent(scalestr);
    } else if(*scalexstr || *scaleystr) {
	if(scalexstr) scalex = parsePercent(scalexstr);
	if(scaleystr) scaley = parsePercent(scaleystr);
    } else if(*widthstr || *heightstr) {
	int width=0;
	int height=0;
	s_getBitmapSize(object, &width, &height);
	if(*widthstr)
	    scalex = (float)parseTwip(widthstr)/(float)width;
	if(*heightstr)
	    scaley = (float)parseTwip(heightstr)/(float)height;
    }
    x = parseTwip(xstr);
    y = parseTwip(ystr);
    rotate = parseFloat(rotatestr);
    shear = parseFloat(shearstr);

    s_texture(name, object, x,y,scalex,scaley,rotate, shear);

    return 0;
}

static int c_texture(map_t*args)
{
    const char*name = lu(args, "instance");
    const char*object = lu(args, "character");
    return texture2(name, object, args, 1);
}

static int c_gradient(map_t*args)
{
    const char*name = lu(args, "name");
    int radial= strcmp(lu(args, "radial"), "radial")?0:1;
    int rotate = parseInt(lu(args, "rotate"));

    readToken();
    if(type != RAWDATA)
        syntaxerror("colon (:) expected");

    if(dict_lookup(&gradients, name))
        syntaxerror("gradient %s defined twice", name);

    s_gradient(name, text, radial, rotate);

    /* check whether we also have placement information,
       which would make this a positioned gradient.
       If there is placement information, texture2() will
       add a texture, which has priority over the gradient.
     */
    texture2(name, name, args, 0);
    return 0;
}

static const char* checkFiltername(map_t* args)
{
    const char* name = lu(args, "name");
    if(strchr(name, ','))
    	syntaxerror("the comma (,) is used to separate filters in filterlists. Please do not use in filternames.");
    return name;
}

static int c_blur(map_t*args)
{
    const char*name = checkFiltername(args);
    const char*blurstr = lu(args, "blur");
    const char*blurxstr = lu(args, "blurx");
    const char*blurystr = lu(args, "blury");
    float blurx=1.0, blury=1.0;
    if(blurstr[0]) {
	blurx = parseFloat(blurstr);
	blury = parseFloat(blurstr);
    }
    if(blurxstr[0])
	blurx = parseFloat(blurxstr);
    if(blurystr[0])
	blury = parseFloat(blurystr);
    int passes = parseInt(lu(args, "passes"));
    s_blur(name, blurx, blury, passes);
    return 0;
}

static int c_gradientglow(map_t*args)
{
    const char*name = checkFiltername(args);
    const char*gradient = lu(args, "gradient");
    const char*blurstr = lu(args, "blur");
    const char*blurxstr = lu(args, "blurx");
    const char*blurystr = lu(args, "blury");
    float blurx=1.0, blury=1.0;
    if(blurstr[0]) {
	blurx = parseFloat(blurstr);
	blury = parseFloat(blurstr);
    }
    if(blurxstr[0])
	blurx = parseFloat(blurxstr);
    if(blurystr[0])
	blury = parseFloat(blurystr);

    float angle = parseFloat(lu(args, "angle")) / 180 * M_PI;
    float distance = parseFloat(lu(args, "distance"));
    float strength = parseFloat(lu(args, "strength"));
    char innershadow = strcmp(lu(args, "innershadow"),"innershadow")?0:1;
    char knockout = strcmp(lu(args, "knockout"),"knockout")?0:1;
    char composite = strcmp(lu(args, "composite"),"composite")?0:1;
    char ontop = strcmp(lu(args, "ontop"),"ontop")?0:1;
    int passes = parseInt(lu(args, "passes"));

    s_gradientglow(name, gradient, blurx, blury, angle, distance, strength, innershadow, knockout, composite, ontop, passes);
    return 0;
}

static int c_dropshadow(map_t*args)
{
    const char*name = checkFiltername(args);
    RGBA color = parseColor(lu(args, "color"));
    const char*blurstr = lu(args, "blur");
    const char*blurxstr = lu(args, "blurx");
    const char*blurystr = lu(args, "blury");
    float blurx=1.0, blury=1.0;
    if(blurstr[0]) {
	blurx = parseFloat(blurstr);
	blury = parseFloat(blurstr);
    }
    if(blurxstr[0])
	blurx = parseFloat(blurxstr);
    if(blurystr[0])
	blury = parseFloat(blurystr);

    float angle = parseFloat(lu(args, "angle")) / 180 * M_PI;
    float distance = parseFloat(lu(args, "distance"));
    float strength = parseFloat(lu(args, "strength"));
    char innershadow = strcmp(lu(args, "innershadow"),"innershadow")?0:1;
    char knockout = strcmp(lu(args, "knockout"),"knockout")?0:1;
    char composite = strcmp(lu(args, "composite"),"composite")?0:1;
    int passes = parseInt(lu(args, "passes"));

    s_dropshadow(name, color, blurx, blury, angle, distance, strength, innershadow, knockout, composite, passes);
    return 0;
}

static int c_bevel(map_t*args)
{
    const char*name = checkFiltername(args);
    RGBA shadow = parseColor(lu(args, "shadow"));
    RGBA highlight = parseColor(lu(args, "highlight"));
    const char*blurstr = lu(args, "blur");
    const char*blurxstr = lu(args, "blurx");
    const char*blurystr = lu(args, "blury");
    float blurx=1.0, blury=1.0;
    if(blurstr[0]) {
	blurx = parseFloat(blurstr);
	blury = parseFloat(blurstr);
    }
    if(blurxstr[0])
	blurx = parseFloat(blurxstr);
    if(blurystr[0])
	blury = parseFloat(blurystr);

    float angle = parseFloat(lu(args, "angle")) / 180 * M_PI;
    float distance = parseFloat(lu(args, "distance"));
    float strength = parseFloat(lu(args, "strength"));
    char innershadow = strcmp(lu(args, "innershadow"),"innershadow")?0:1;
    char knockout = strcmp(lu(args, "knockout"),"knockout")?0:1;
    char composite = strcmp(lu(args, "composite"),"composite")?0:1;
    char ontop = strcmp(lu(args, "ontop"),"ontop")?0:1;
    int passes = parseInt(lu(args, "passes"));

    s_bevel(name, shadow, highlight, blurx, blury, angle, distance, strength, innershadow, knockout, composite, ontop, passes);
    return 0;
}

static int c_define(map_t*args)
{
    const char*name = lu(args, "name");
    const char*value = lu(args, "value");
    
    if(!defines_initialized) {
	dict_init(&defines, 16);
	mem_init(&define_values);
	defines_initialized = 1;
    }
    int val = parseTwip(value);
    int pos = mem_put(&define_values, &val, sizeof(val));
    dict_put(&defines, name, INT_AS_PTR(pos + 1));
    return 0;
}
static int c_point(map_t*args)
{
    const char*name = lu(args, "name");
    int pos;
    SPOINT p;
    if(!points_initialized) {
	dict_init(&points, 16);
	mem_init(&mpoints);
	points_initialized = 1;
    }
    p.x = parseTwip(lu(args, "x"));
    p.y = parseTwip(lu(args, "y"));
    pos = mem_put(&mpoints, &p, sizeof(p));
    dict_put(&points, name, INT_AS_PTR(pos+1));
    return 0;
}
static int c_play(map_t*args)
{
    const char*name = lu(args, "name");
    const char*loop = lu(args, "loop");
    const char*nomultiple = lu(args, "nomultiple");
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
    const char*name = map_lookup(args, "name");

    if(s_playsound(name, 0,0,1))
        return 0;
    else if(s_swf3action(name, "stop"))
        return 0;
    syntaxerror("I don't know anything about sound/movie \"%s\"", name);
	return 0;
}

static int c_nextframe(map_t*args)
{
    const char*name = lu(args, "name");

    if(s_swf3action(name, "nextframe")) {
	return 0;
    }
    syntaxerror("I don't know anything about movie \"%s\"", name);
    return 0;
}

static int c_previousframe(map_t*args)
{
    const char*name = lu(args, "name");

    if(s_swf3action(name, "previousframe")) {
	return 0;
    }
    syntaxerror("I don't know anything about movie \"%s\"", name);
    return 0;
}

static int c_movement(map_t*args, int type)
{
    const char*instance = lu(args, "name");

    const char* xstr="";
    const char* ystr="";
    SRECT oldbbox;
    parameters_t p;
    U16 set = 0x0000;

    xstr = lu(args, "x");
    ystr = lu(args, "y");

    s_getParameters(instance, &p);

    /* x,y position */
    if(xstr[0])
    {
        if(isRelative(xstr))
        {
            if(type == PT_PUT || type == PT_STARTCLIP)
                syntaxerror("relative x values not allowed for initial put or startclip");
            p.x += parseTwip(getOffset(xstr))*getSign(xstr);
        }
        else
        {
            p.x = parseTwip(xstr);
        }
        set = set | SF_X;
     }
    if(ystr[0])
    {
        if(isRelative(ystr))
        {
            if(type == PT_PUT || type == PT_STARTCLIP)
                syntaxerror("relative y values not allowed for initial put or startclip");
            p.y += parseTwip(getOffset(ystr))*getSign(ystr);
        }
        else
        {
            p.y = parseTwip(ystr);
        }
        set = set | SF_Y;
    }

    if(change_sets_all)
	set = SF_ALL;
    p.set = set;

    switch (type)
    {
        case PT_MOVE:
            {
                const char* interstr = lu(args, "interpolation");
                interpolation_t* inter = (interpolation_t*)dict_lookup(&interpolations, interstr);
                if(!inter)
                    syntaxerror("unkown interpolation %s", interstr);
                s_change(instance, p, inter);
            }
            break;
        case PT_SMOVE:
            {
                const char* interstr = lu(args, "interpolation");
                interpolation_t* inter = (interpolation_t*)dict_lookup(&interpolations, interstr);
                if(!inter)
                    syntaxerror("unkown interpolation %s", interstr);
            	s_schange(instance, p, inter);
            }
            break;
        case PT_SWEEP:
            {
            	const char* rstr = lu(args, "r");
            	int radius = parseTwip(rstr);
            	if(radius <= 0)
            		syntaxerror("sweep not possible: radius must be greater than 0.");
            	const char* dirstr = lu(args, "dir");
            	int clockwise = parseDir(dirstr);
            	const char* arcstr = lu(args, "arc");
            	int short_arc = parseArc(arcstr);
                const char* interstr = lu(args, "interpolation");
                interpolation_t* inter = (interpolation_t*)dict_lookup(&interpolations, interstr);
                if(!inter)
                    syntaxerror("unkown interpolation %s", interstr);
            	s_sweep(instance, p, radius, clockwise, short_arc, inter);
            }
            break;
	}
    return 0;
}

static int c_placement(map_t*args, int type)
{
    const char*instance = lu(args, (type==PT_PUT||type==PT_STARTCLIP)?"instance":"name");
    const char*character = 0;

    const char* luminancestr = lu(args, "luminance");
    const char* scalestr = lu(args, "scale");
    const char* scalexstr = lu(args, "scalex");
    const char* scaleystr = lu(args, "scaley");
    const char* rotatestr = lu(args, "rotate");
    const char* shearstr = lu(args, "shear");
    const char* xstr="", *pivotstr="";
    const char* ystr="", *anglestr="";
    const char*above = lu(args, "above"); /*FIXME*/
    const char*below = lu(args, "below");
    const char* rstr = lu(args, "red");
    const char* gstr = lu(args, "green");
    const char* bstr = lu(args, "blue");
    const char* astr = lu(args, "alpha");
    const char* pinstr = lu(args, "pin");
    const char* as = map_lookup(args, "as");
    const char* blendmode = lu(args, "blend");
    const char* filterstr = lu(args, "filter");
    const char* noinstancenamestr = lu(args, "noinstancename");
    U8 blend;
    MULADD r,g,b,a;
    float oldwidth;
    float oldheight;
    SRECT oldbbox;
    MULADD luminance;
    parameters_t p;
    U16 set = 0x0000;

    if(type==9)
    { // (?) .rotate  or .arcchange
        pivotstr = lu(args, "pivot");
        anglestr = lu(args, "angle");
    }
    else
    {
        xstr = lu(args, "x");
        ystr = lu(args, "y");
    }

    if(luminancestr[0])
        luminance = parseMulAdd(luminancestr);
    else
    {
        luminance.add = 0;
        luminance.mul = 256;
    }

    if(scalestr[0])
    {
        if(scalexstr[0]||scaleystr[0])
            syntaxerror("scalex/scaley and scale cannot both be set");
        scalexstr = scaleystr = scalestr;
    }

    if(type == PT_PUT || type == PT_STARTCLIP)  {
	// put or startclip
	character = lu(args, "character");
	parameters_clear(&p);
    } else if(type == PT_BUTTON) {
	character = lu(args, "name");
	parameters_clear(&p);
	// button's show
    } else {
	s_getParameters(instance, &p);
    }

    /* noinstancename */
    p.noinstancename = !strcmp(noinstancenamestr, "noinstancename");

    /* x,y position */
    if(xstr[0])
    {
        if(isRelative(xstr))
        {
            if(type == PT_PUT || type == PT_STARTCLIP)
                syntaxerror("relative x values not allowed for initial put or startclip");
            p.x += parseTwip(getOffset(xstr))*getSign(xstr);
        }
        else
        {
            p.x = parseTwip(xstr);
        }
        set = set | SF_X;
     }
    if(ystr[0])
    {
        if(isRelative(ystr))
        {
            if(type == PT_PUT || type == PT_STARTCLIP)
                syntaxerror("relative y values not allowed for initial put or startclip");
            p.y += parseTwip(getOffset(ystr))*getSign(ystr);
        }
        else
        {
            p.y = parseTwip(ystr);
        }
        set = set | SF_Y;
	}

    /* scale, scalex, scaley */
    if(character)
        oldbbox = s_getCharBBox(character);
    else
        oldbbox = s_getInstanceBBox(instance);
    oldwidth = oldbbox.xmax - oldbbox.xmin;
    oldheight = oldbbox.ymax - oldbbox.ymin;
    if(scalexstr[0])
    {
        if(oldwidth==0)
            p.scalex = 1.0;
        else
            if(scalexstr[0])
                p.scalex = (float)(parseNewSize(scalexstr, oldwidth))/oldwidth;
        set = set | SF_SCALEX;
    }
   if(scaleystr[0])
   {
        if(oldheight==0)
            p.scaley = 1.0;
        else
            if(scaleystr[0])
                p.scaley = (float)(parseNewSize(scaleystr, oldheight))/oldheight;
        set = set | SF_SCALEY;
   }

    /* rotation */
    if(rotatestr[0])
    {
        if(isRelative(rotatestr))
            p.rotate += parseFloat(getOffset(rotatestr))*getSign(rotatestr);
        else
            p.rotate = parseFloat(rotatestr);
        set = set | SF_ROTATE;
	}

    /* shearing */
    if(shearstr[0])
    {
        if(isRelative(shearstr))
            p.shear += parseFloat(getOffset(shearstr))*getSign(shearstr);
        else
            p.shear = parseFloat(shearstr);
        set = set | SF_SHEAR;
    }

    if(pivotstr[0])
    {
        if(isPoint(pivotstr))
            p.pivot = parsePoint(pivotstr);
        else
            p.pivot = getPoint(oldbbox, pivotstr);
        set = set | SF_PIVOT;
    }

    if(pinstr[0])
    {
        if(isPoint(pinstr))
            p.pin = parsePoint(pinstr);
        else
            p.pin = getPoint(oldbbox, pinstr);
        set = set | SF_PIN;
    }

    /* color transform */

    if(rstr[0] || luminancestr[0])
    {
        MULADD r;
        if(rstr[0])
            r = parseMulAdd(rstr);
        else
        {
            r.add = p.cxform.r0;
            r.mul = p.cxform.r1;
        }
        r = mergeMulAdd(r, luminance);
        p.cxform.r0 = r.mul;
        p.cxform.r1 = r.add;
        set = set | SF_CX_R;
	}
    if(gstr[0] || luminancestr[0])
    {
        MULADD g;
        if(gstr[0])
            g = parseMulAdd(gstr);
        else
        {
            g.add = p.cxform.g0;
            g.mul = p.cxform.g1;
        }
        g = mergeMulAdd(g, luminance);
        p.cxform.g0 = g.mul;
        p.cxform.g1 = g.add;
        set = set | SF_CX_G;
    }
    if(bstr[0] || luminancestr[0])
    {
        MULADD b;
        if(bstr[0])
            b = parseMulAdd(bstr);
        else
        {
            b.add = p.cxform.b0;
            b.mul = p.cxform.b1;
        }
        b = mergeMulAdd(b, luminance);
        p.cxform.b0 = b.mul;
        p.cxform.b1 = b.add;
        set = set | SF_CX_B;
    }
    if(astr[0])
    {
        MULADD a = parseMulAdd(astr);
        p.cxform.a0 = a.mul;
        p.cxform.a1 = a.add;
        set = set | SF_CX_A;
    }

    if(blendmode[0])
    {
        int t;
        blend = 255;
        for(t = 0; blendModeNames[t]; t++)
        {
            if(!strcmp(blendModeNames[t], blendmode))
            {
                blend = t;
                break;
            }
        }
        if(blend == 255)
        {
            syntaxerror("unknown blend mode: '%s'", blendmode);
        }
        p.blendmode = blend;
        set = set | SF_BLEND;
	}

    if(filterstr[0])
    {
        p.filters = parseFilters((char*)filterstr);
        set = set | SF_FILTER;
    }

    if(type == PT_CHANGE && set & (SF_X | SF_Y))
	warning("As of version 0.8.2 using the .change command to modify an \
object's position on the stage is considered deprecated. Future \
versions may consider x and y parameters for the .change command \
to be illegal; please use the .move command.");

    if(change_sets_all)
	set = SF_ALL;
    p.set = set;

    switch (type)
    {
        case PT_PUT:
            s_put(instance, character, p);
            break;
        case PT_CHANGE:
            {
                const char* interstr = lu(args, "interpolation");
                interpolation_t* inter = (interpolation_t*)dict_lookup(&interpolations, interstr);
                if(!inter)
                    syntaxerror("unkown interpolation %s", interstr);
                s_change(instance, p, inter);
            }
            break;
        case PT_SCHANGE:
            {
                const char* interstr = lu(args, "interpolation");
                interpolation_t* inter = (interpolation_t*)dict_lookup(&interpolations, interstr);
                if(!inter)
                    syntaxerror("unkown interpolation %s", interstr);
            	s_schange(instance, p, inter);
            }
            break;
        case PT_JUMP:
            s_jump(instance, p);
            break;
        case PT_STARTCLIP:
            s_startclip(instance, character, p);
            break;
        case PT_BUTTON:
            if(as && as[0])
                s_buttonput(character, as, p);
            else
                s_buttonput(character, "shape", p);
            break;
//        default:
	}
    return 0;
}
static int c_put(map_t*args)
{
    c_placement(args, PT_PUT);
    return 0;
}
static int c_change(map_t*args)
{
    if(currentframe == 0)
        warning("change commands in frame 1 will be ignored, please use the put command to set object parameters");
    c_placement(args, PT_CHANGE);
    return 0;
}
static int c_schange(map_t*args)
{
    c_placement(args, PT_SCHANGE);
    return 0;
}
static int c_move(map_t* args)
{
    c_movement(args, PT_MOVE);
    return 0;
}
static int c_smove(map_t* args)
{
    c_movement(args, PT_SMOVE);
    return 0;
}
static int c_sweep(map_t* args)
{
    c_movement(args, PT_SWEEP);
    return 0;
}
static int c_arcchange(map_t*args)
{
    c_placement(args, 0);
    return 0;
}
static int c_jump(map_t*args)
{
    c_placement(args, PT_JUMP);
    return 0;
}
static int c_startclip(map_t*args)
{
    c_placement(args, PT_STARTCLIP);
    return 0;
}
static int c_show(map_t*args)
{
    c_placement(args, PT_BUTTON);
    return 0;
}
static int c_toggle(map_t* args)
{
    const char*instance = lu(args, "name");
    U16 flagsOn = 0x0000, flagsOff = 0xffff;
    const char* alignstr = lu(args, "fixed_alignment");
    if(!strcmp(alignstr, "on"))
    	flagsOn += IF_FIXED_ALIGNMENT;
    else
    	if(!strcmp(alignstr, "off"))
    	    flagsOff -= IF_FIXED_ALIGNMENT;
    	else
    	    syntaxerror("values for toggle must be \"on\" or \"off\". %s is not legal.", alignstr);
    s_toggle(instance, flagsOn, flagsOff);
    return 0;
}
static int c_del(map_t*args)
{
    const char*instance = lu(args, "name");
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
    const char* name = lu(args, "name");
    const char* scalinggrid = lu(args, "scalinggrid");
    const char* as3name = lu(args, "as3name");

    if(scalinggrid && *scalinggrid) {
	SRECT r = parseBox(scalinggrid);
	s_sprite(name, &r, as3name);
    } else {
	s_sprite(name, 0, as3name);
    }
    return 0;
}
static int c_frame(map_t*args)
{
    const char*framestr = lu(args, "n");
    const char*cutstr = lu(args, "cut");

    const char*name = lu(args, "name");
    const char*anchor = lu(args, "anchor");
    char buf[40];

    if(!strcmp(anchor, "anchor") && !*name)
	name = framestr;

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
    s_frame(frame, cut, name, !strcmp(anchor, "anchor"));
    return 0;
}
static int c_primitive(map_t*args)
{
    const char*name = lu(args, "name");
    const char*command = lu(args, "commandname");
    int width=0, height=0, r=0;
    int linewidth = parseTwip(lu(args, "line"));
    const char*colorstr = lu(args, "color");
    RGBA color = parseColor(colorstr);
    const char*fillstr = lu(args, "fill");
    int dofill = 1;
    int type=0;
    const char* font;
    const char* text;
    const char* outline=0;
    RGBA fill;
    if(!strcmp(command, "circle"))
	type = 1;
    else if(!strcmp(command, "filled"))
	type = 2;

    if(type==0) {
	width = parseTwip(lu(args, "width"));
	height = parseTwip(lu(args, "height"));
    } else if(type==1) {
	r = parseTwip(lu(args, "r"));
    } else if(type==2) {
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
    const char*name = lu(args, "name");
    const char*text = lu(args, "text");
    const char*font = lu(args, "font");
    float size = parsePxOrPercent(font, lu(args, "size"));

    s_textshape(name, font, size, text);
    return 0;
}

static int c_swf(map_t*args)
{
    const char*name = lu(args, "name");
    const char*filename = lu(args, "filename");
    const char*command = lu(args, "commandname");
    const char*as3name = lu(args, "as3name");

    if(!strcmp(command, "shape"))
	warning("Please use .swf instead of .shape");
    s_includeswf(name, filename, as3name);
    return 0;
}

static int c_font(map_t*args)
{
    const char*name = lu(args, "name");
    const char*filename = lu(args, "filename");
    s_font(name, filename);
    return 0;
}

static int c_sound(map_t*args)
{
    const char*name = lu(args, "name");
    const char*filename = lu(args, "filename");
    s_sound(name, filename);
    return 0;
}

static int c_text(map_t*args)
{
    const char*name = lu(args, "name");
    const char*text = lu(args, "text");
    const char*font = lu(args, "font");
    float size = parsePxOrPercent(font, lu(args, "size"));
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
    const char*name = lu(args, "name");
    const char*url = lu(args, "url");
    s_quicktime(name, url);
    return 0;
}

static int c_video(map_t*args)
{
    const char*name = lu(args, "name");
    int width = parseInt(lu(args, "width"));
    int height = parseInt(lu(args, "height"));
    s_video(name, width, height);
    return 0;
}

static int c_image(map_t*args)
{
    const char*command = lu(args, "commandname");
    const char*name = lu(args, "name");
    const char*filename = lu(args, "filename");
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
    const char*name = lu(args, "name");
    const char*format = lu(args, "format");

    readToken();
    if(type != RAWDATA)
	syntaxerror("colon (:) expected");

    s_outline(name, format, text);
    return 0;
}

int fakechar(map_t*args)
{
    const char*name = lu(args, "name");
    s_box(name, 0, 0, black, 20, 0);
    return 0;
}

static int c_egon(map_t*args) {return fakechar(args);}
static int c_button(map_t*args) {
    const char*name = lu(args, "name");
    const char*as3name = lu(args, "as3name");
    s_button(name, as3name);
    return 0;
}
static int current_button_flags = 0;
static int c_on_press(map_t*args)
{
    const char*position = lu(args, "position");
    const char*action = "";
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
    const char*position = lu(args, "position");
    const char*action = "";
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
    const char*position = lu(args, "state");
    const char*action = "";
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
    const char*position = lu(args, "state");
    const char*action = "";
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
    const char*key = lu(args, "key");
    const char*action = "";
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
 //"name font size width height text="" color=black maxlength=0 variable="" @password=0 @wordwrap=0 @multiline=0 @html=0 @noselect=0 @readonly=0 @autosize=0"},
    const char*name = lu(args, "name");
    const char*font = lu(args, "font");
    int size = (int)(1024*parsePxOrPercent(font, lu(args, "size")));
    int width = parseTwip(lu(args, "width"));
    int height = parseTwip(lu(args, "height"));
    const char*text  = lu(args, "text");
    RGBA color = parseColor(lu(args, "color"));
    int maxlength = parseInt(lu(args, "maxlength"));
    const char*variable = lu(args, "variable");
    const char*passwordstr = lu(args, "password");
    const char*wordwrapstr = lu(args, "wordwrap");
    const char*multilinestr = lu(args, "multiline");
    const char*htmlstr = lu(args, "html");
    const char*noselectstr = lu(args, "noselect");
    const char*readonlystr = lu(args, "readonly");
    const char*borderstr = lu(args, "border");
    const char*autosizestr = lu(args, "autosize");
    const char*alignstr = lu(args, "align");
    int align = -1;

    int flags = 0;
    if(!strcmp(passwordstr, "password")) flags |= ET_PASSWORD;
    if(!strcmp(wordwrapstr, "wordwrap")) flags |= ET_WORDWRAP;
    if(!strcmp(multilinestr, "multiline")) flags |= ET_MULTILINE;
    if(!strcmp(readonlystr, "readonly")) flags |= ET_READONLY;
    if(!strcmp(htmlstr, "html")) flags |= ET_HTML;
    if(!strcmp(noselectstr, "noselect")) flags |= ET_NOSELECT;
    if(!strcmp(borderstr, "border")) flags |= ET_BORDER;
    if(!strcmp(autosizestr, "autosize")) flags |= ET_AUTOSIZE;
    if(!strcmp(alignstr, "left") || !*alignstr) align = ET_ALIGN_LEFT;
    else if(!strcmp(alignstr, "right")) align = ET_ALIGN_RIGHT;
    else if(!strcmp(alignstr, "center")) align = ET_ALIGN_CENTER;
    else if(!strcmp(alignstr, "justify")) align = ET_ALIGN_JUSTIFY;
    else syntaxerror("Unknown alignment: %s", alignstr);

    s_edittext(name, font, size, width, height, text, &color, maxlength, variable, flags, align);
    return 0;
}

static int c_morphshape(map_t*args) {return fakechar(args);}
static int c_movie(map_t*args) {return fakechar(args);}

static char* readfile(char*filename)
{
    FILE*fi = fopen(filename, "rb");
    int l;
    char*text;
    if(!fi)
	syntaxerror("Couldn't find file %s: %s", filename, strerror(errno));
    fseek(fi, 0, SEEK_END);
    l = ftell(fi);
    fseek(fi, 0, SEEK_SET);
    text = rfx_alloc(l+1);
    fread(text, l, 1, fi);
    text[l]=0;
    fclose(fi);
    return text;
}

static int c_action(map_t*args)
{
    const char* filename  = map_lookup(args, "filename");
    if(!filename ||!*filename) {
	readToken();
	if(type != RAWDATA) {
	    syntaxerror("colon (:) expected");
	}
	s_action(text);
    } else {
	s_action(readfile((char*)filename));
    }

    return 0;
}

static int c_initaction(map_t*args)
{
    const char* character = lu(args, "name");
    const char* filename  = map_lookup(args, "filename");
    if(!filename ||!*filename) {
	readToken();
	if(type != RAWDATA) {
	    syntaxerror("colon (:) expected");
	}
	s_initaction(character, text);
    } else {
	s_initaction(character, readfile((char*)filename));
    }

    return 0;
}

static struct {
    char*command;
    command_func_t* func;
    char*arguments;
} arguments[] =
{{"flash", c_flash, "bbox=autocrop background=black version=6 fps=50 name= filename= @compress=default @change-sets-all=no @export=1 @mainclass="},
 {"frame", c_frame, "n=<plus>1 name= @cut=no @anchor=no"},
 // "import" type stuff
 {"swf", c_swf, "name filename as3name="},
 {"shape", c_swf, "name filename"},
 {"jpeg", c_image, "name filename quality=80%"},
 {"png", c_image, "name filename"},
 {"movie", c_movie, "name filename"},
 {"sound", c_sound, "name filename"},
 {"font", c_font, "name filename glyphs= @flashtype="},
 {"soundtrack", c_soundtrack, "filename"},
 {"quicktime", c_quicktime, "url"},
 {"video", c_video, "name width= height="},

    // generators of primitives

 {"define", c_define, "name value=0"},
 {"point", c_point, "name x=0 y=0"},
 {"gradient", c_gradient, "name @radial=0 rotate=0 scale= scalex= scaley= x= y= width= height= r= shear="}, //extra parameters like .texture
 {"interpolation", c_interpolation, "name function=linear speed=1.3 amplitude=0 bounces=2 growth=1.5 damping=2 slope=0"},
 {"outline", c_outline, "name format=simple"},
 {"textshape", c_textshape, "name font size=100% text"},

    // filters
 {"blur", c_blur, "name blur= blurx= blury= passes=1"},
 {"gradientglow", c_gradientglow, "name gradient blur= blurx= blury= angle=0.0 distance=0.0 strength=1.0 @innershadow=0 @knockout=0 @composite=0 @ontop=0 passes=1"},
 {"dropshadow", c_dropshadow, "name color blur= blurx= blury= angle=0.0 distance=0.0 strength=1.0 @innershadow=0 @knockout=0 @composite=0 passes=1"},
 {"bevel", c_bevel, "name shadow highlight blur= blurx= blury= angle=0.0 distance=0.0 strength=1.0 @innershadow=0 @knockout=0 @composite=0 @ontop=0 passes=1"},

    // character generators
 {"box", c_primitive, "name width height color=white line=1 @fill=none"},
 {"circle", c_primitive, "name r color=white line=1 @fill=none"},
 {"filled", c_primitive, "name outline color=white line=1 @fill=none"},

 {"egon", c_egon, "name vertices color=white line=1 @fill=none"},
 {"text", c_text, "name text font size=100% color=white"},
 {"edittext", c_edittext, "name font= size=100% width height text="" color=white maxlength=0 variable="" @password=0 @wordwrap=0 @multiline=0 @html=0 @noselect=0 @readonly=0 @border=0 @autosize=0 align="},
 {"morphshape", c_morphshape, "name start end"},
 {"button", c_button, "name as3name="},
    {"show", c_show,             "name x=0 y=0 red=+0 green=+0 blue=+0 alpha=+0 luminance= scale= scalex= scaley= blend= filter= pivot= pin= shear= rotate= ratio= above= below= as="},
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
 {"put", c_put,             "<i> x=0 y=0 red=+0 green=+0 blue=+0 alpha=+0 luminance= scale= scalex= scaley= blend= filter= pivot= pin= shear= rotate= ratio= above= below= @noinstancename=0"},
 {"startclip", c_startclip, "<i> x=0 y=0 red=+0 green=+0 blue=+0 alpha=+0 luminance= scale= scalex= scaley= blend= filter= pivot= pin= shear= rotate= ratio= above= below= @noinstancename=0"},
 {"move", c_move,	"name x= y= interpolation=linear"},
 {"smove", c_smove, 	"name x= y= interpolation=linear"},
 {"sweep", c_sweep,	"name x= y= r= dir=counterclockwise arc=short interpolation=linear"},
 {"change", c_change,   "name x= y= red= green= blue= alpha= luminance= scale= scalex= scaley= blend= filter= pivot= pin= shear= rotate= ratio= above= below= interpolation=linear"},
 //{"arcchange", c_arcchange,   "name pivot= angle= red= green= blue= alpha= luminance= scale= scalex= scaley= blend= filter= pivot= pin= shear= rotate= ratio= above= below="},
 {"schange", c_schange, "name red= green= blue= alpha= luminance= scale= scalex= scaley= blend= filter= pivot= pin= shear= rotate= ratio= above= below= interpolation=linear"},
 {"jump", c_jump,       "name x= y= red= green= blue= alpha= luminance= scale= scalex= scaley= blend= filter= pivot= pin= shear= rotate= ratio= above= below="},
 {"del", c_del, "name"},
    // virtual object placement
 {"texture", c_texture, "<i> x=0 y=0 width= height= scale= scalex= scaley= r= shear= rotate="},
    // switching
 {"toggle", c_toggle, "name fixed_alignment="},

    // commands which start a block
//startclip (see above)
 {"sprite", c_sprite, "name scalinggrid= as3name="},
 {"action", c_action, "filename="},
 {"initaction", c_initaction, "name filename="},

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
	    syntaxerror("Illegal argument \"%s\" to .%s", text, command);
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

#ifndef EMPTY
    // catch missing .flash directives at the beginning of a file
    if(strcmp(command, "flash") && !stackpos)
    {
        syntaxerror("No movie defined- use .flash first");
    }
#endif

#ifdef DEBUG
    printf(".%s\n", command);fflush(stdout);
    map_dump(&args, stdout, "\t");fflush(stdout);
#endif

#ifndef EMPTY
    (*arguments[nr].func)(&args);
#else
    if(!strcmp(command, "action")  || !strcmp(command, "initaction") ||
       !strcmp(command, "outline") || !strcmp(command, "gradient")) {
	readToken();
	if(type != RAWDATA) {
	    syntaxerror("colon (:) expected");
	}
    }
#endif
    map_clear(&args);
    return;
}


/* for now only intended to find what glyphs of each font are to be included in the swf file.
 * Therefore some knowledge about the command is assumed i.e. it is font/text-related
 * No syntax checking is done */
static void analyseArgumentsForCommand(char*command)
{
    int t;
    map_t args;
    int nr = -1;
    U8* glyphs_to_include;
    msg("<verbose> analyse Command: %s (line %d)", command, line);

    for(t=0;t<sizeof(arguments)/sizeof(arguments[0]);t++)
    {
        if(!strcmp(arguments[t].command, command))
        {
            args = parseArguments(command, arguments[t].arguments);
            nr = t;
            break;
        }
    }
#ifdef DEBUG
    printf(".%s\n", command);fflush(stdout);
    map_dump(&args, stdout, "\t");fflush(stdout);
#endif
    const char* name = lu(&args, "name");
    if(!strcmp(command, "font"))
    {
	const char* fontfile = lu(&args, "filename");
	const char* glyphs = lu(&args, "glyphs");
	const char* flashtype = lu(&args, "flashtype");
	s_createfont(name, fontfile, glyphs, flashtype[0]);
    } else {
        SWFFONT* font = dict_lookup(&fonts, lu(&args, "font"));
        if(!font) {
	    //that's ok... it might be an edittext with a system font
            //syntaxerror("font %s is not known in line %d", lu(&args, "font"), line);
	} else
            if(font->use && !font->use->glyphs_specified) {
		if(!strcmp(command, "edittext"))
		{
            	    swf_FontUseAll(font);
            	    font->use->glyphs_specified = 1;
		}
            	else
            	    swf_FontUseUTF8(font, (U8*)lu(&args, "text"), 0xffff);
            }
    }
    map_clear(&args);
    return;
}

void skipParameters()
{
	do
		readToken();
	while (type != COMMAND);
	pushBack();
}

void findFontUsage()
{
    char* fontRelated = "font;text;textshape;edittext;";
    while(!noMoreTokens())
    {
        readToken();
        if(type != COMMAND)
            syntaxerror("command expected");
        if(strstr(fontRelated, text))
            analyseArgumentsForCommand(text);
        else
            if(strcmp(text, "end"))
                skipParameters();
    }
}

void firstPass()
{
    pos = 0;
    id = 0;
    dict_init(&fonts, 16);
    cleanUp = &freeFontDictionary;
    findFontUsage();
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
	fprintf(stderr, "parser returned error.\n");
	return 1;
    }
    firstPass();
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

