/* swfbbox.c
   Tool for playing around with SWF bounding boxes.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#include "../config.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"

static char * filename = 0;
static char * outfilename = "output.swf";
static int optimize = 0;
static int swifty = 0;
static int verbose = 0;

struct options_t options[] =
{
 {"V","version"},
 {"O","optimize"},
 {"o","output"},
 {"S","swifty"},
 {"v","verbose"},
 {0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfbbox - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } 
    else if(!strcmp(name, "O")) {
	optimize = 1;
	return 0;
    } 
    else if(!strcmp(name, "S")) {
	swifty = 1;
	return 0;
    } 
    else if(!strcmp(name, "v")) {
	verbose = 1;
	return 0;
    } 
    else if(!strcmp(name, "o")) {
	outfilename = val;
	return 1;
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
    printf("Usage: %s [-at] file.swf\n", name);
    printf("\t-h , --help\t\t Print help and exit\n");
    printf("\t-O , --optimize\t\t Print help and exit\n");
    printf("\t-S , --swifty\t\t Print out transformed bounding boxes\n");
    printf("\t-o , --output\t\t Set output filename (for -O)\n");
    printf("\t-V , --version\t\t Print program version and exit\n");
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

#define swf_ResetReadBits(tag)   if (tag->readBit)  { tag->pos++; tag->readBit = 0; }

void parseFillStyleArray(TAG*tag, SHAPE2*shape)
{
    U16 count;
    int t;
    int num=0;
    if(tag->id == ST_DEFINESHAPE)
	num = 1;
    else if(tag->id == ST_DEFINESHAPE2)
	num = 2;
    else if(tag->id == ST_DEFINESHAPE3)
	num = 3;

    count = swf_GetU8(tag);
    if(count == 0xff && num>1) // defineshape2,3 only
	count = swf_GetU16(tag);

    if(verbose) printf("num: %d\n", count);
    shape->numfillstyles = count;
    shape->fillstyles = malloc(sizeof(FILLSTYLE)*count);

    for(t=0;t<count;t++)
    {
	int type;
	U8*pos;
	FILLSTYLE*dest = &shape->fillstyles[t];
	swf_ResetReadBits(tag);
	type = swf_GetU8(tag); //type
	shape->fillstyles[t].type = type;
	if(type == 0) {
	    /* plain color */
	    if(num == 3)
		swf_GetRGBA(tag, &dest->color);
	    else 
		swf_GetRGB(tag, &dest->color);
	}
	else if(type == 0x10 || type == 0x12)
	{
	    /* linear/radial gradient fill */
	    swf_ResetReadBits(tag);
	    swf_GetMatrix(tag, &dest->m);
	    swf_ResetReadBits(tag);
	    dest->gradient = malloc(sizeof(GRADIENT)); // TODO: free this again
	    swf_GetGradient(tag, dest->gradient, num>=3?1:0);
	}
	else if(type == 0x40 || type == 0x41)
	{
	    /* bitmap fill */
	    swf_ResetReadBits(tag);
	    dest->id_bitmap = swf_GetU16(tag); //id
	    swf_ResetReadBits(tag); //?
	    swf_GetMatrix(tag, &dest->m);
	}
	else {
	    fprintf(stderr, "rfxswf:swftools.c Unknown fillstyle:0x%02x\n",type);
	}
    }
    swf_ResetReadBits(tag);
    count = swf_GetU8(tag); // line style array
    if(count == 0xff)
	count = swf_GetU16(tag);

    if(verbose) printf("lnum: %d\n", count);

    shape->numlinestyles = count;
    shape->linestyles = malloc(sizeof(LINESTYLE)*count);
    /* TODO: should we start with 1 and insert a correct definition of the
       "built in" linestyle 0? */
    for(t=0;t<count;t++) 
    {
	shape->linestyles[t].width = swf_GetU16(tag);
	if(num == 3)
	    swf_GetRGBA(tag, &shape->linestyles[t].color);
	else
	    swf_GetRGB(tag, &shape->linestyles[t].color);
    }
    return;
}

void swf_ParseDefineShape(TAG*tag, SHAPE2*shape)
{
    int num = 0, id;
    U16 fill,line;
    SRECT r;
    SRECT r2;
    SHAPELINE*l;
    if(tag->id == ST_DEFINESHAPE)
	num = 1;
    else if(tag->id == ST_DEFINESHAPE2)
	num = 2;
    else if(tag->id == ST_DEFINESHAPE3)
	num = 3;
    else {
	fprintf(stderr, "parseDefineShape must be called with a shape tag");
    }

    id = swf_GetU16(tag); //id
    memset(shape, 0, sizeof(SHAPE2));
    shape->bbox = malloc(sizeof(SRECT));
    swf_GetRect(tag, &r);

    memcpy(shape->bbox, &r, sizeof(SRECT));
    parseFillStyleArray(tag, shape);

    swf_ResetReadBits(tag); 
    fill = (U16)swf_GetBits(tag,4);
    line = (U16)swf_GetBits(tag,4);

    shape->lines = swf_ParseShapeData(&tag->data[tag->pos], (tag->len - tag->pos)*8, fill, line);

    l = shape->lines;
}

void swf_Shape2Optimize(SHAPE2*shape)
{
    if(!shape->bbox)
	shape->bbox = malloc(sizeof(SRECT));
    *(shape->bbox) = swf_GetShapeBoundingBox(shape);
}

void swf_Shape2ToShape(SHAPE2*shape2, SHAPE*shape)
{
    TAG*tag = swf_InsertTag(0,0);
    SHAPELINE*l,*next;
    int newx=0,newy=0,lastx=0,lasty=0,oldls=0,oldfs0=0,oldfs1=0;

    memset(shape, 0, sizeof(SHAPE));

    shape->linestyle.n = shape2->numlinestyles;
    shape->linestyle.data = (LINESTYLE*)malloc(sizeof(LINESTYLE)*shape->linestyle.n);
    memcpy(shape->linestyle.data, shape2->linestyles, sizeof(LINESTYLE)*shape->linestyle.n);
    
    shape->fillstyle.n =  shape2->numfillstyles;
    shape->fillstyle.data = (FILLSTYLE*)malloc(sizeof(FILLSTYLE)*shape->fillstyle.n);
    memcpy(shape->fillstyle.data, shape2->fillstyles, sizeof(FILLSTYLE)*shape->fillstyle.n);

    swf_ShapeCountBits(shape,NULL,NULL);

    l = shape2->lines;

    while(l) {
	int ls=0,fs0=0,fs1=0;

	if(l->type != moveTo) {
	    if(oldls != l->linestyle) {oldls = ls = l->linestyle;if(!ls) ls=0x8000;}
	    if(oldfs0 != l->fillstyle0) {oldfs0 = fs0 = l->fillstyle0;if(!fs0) fs0=0x8000;}
	    if(oldfs1 != l->fillstyle1) {oldfs1 = fs1 = l->fillstyle1;if(!fs1) fs1=0x8000;}

	    if(ls || fs0 || fs1 || newx!=0x7fffffff || newy!=0x7fffffff) {
		swf_ShapeSetAll(tag,shape,newx,newy,ls,fs0,fs1);
		newx = 0x7fffffff;
		newy = 0x7fffffff;
	    }
	}

	if(l->type == lineTo) {
	    swf_ShapeSetLine(tag,shape,l->x-lastx,l->y-lasty);
	} else if(l->type == splineTo) {
	    swf_ShapeSetCurve(tag,shape, l->sx-lastx,l->sy-lasty, l->x-l->sx,l->y-l->sy);
	}
	if(l->type == moveTo) {
	    newx = l->x;
	    newy = l->y;
	}

	lastx = l->x;
	lasty = l->y;
	l = l->next;
    }
    swf_ShapeSetEnd(tag);
    shape->data = tag->data;
    shape->bitlen = tag->len*8;
}

void swf_SetShape2(TAG*tag, SHAPE2*shape2)
{
    SHAPE shape;
    swf_Shape2ToShape(shape2, &shape);

    swf_SetRect(tag,shape2->bbox);
    swf_SetShapeStyles(tag, &shape);
    swf_ShapeCountBits(&shape,NULL,NULL);
    swf_SetShapeBits(tag,&shape);

    swf_SetBlock(tag, shape.data, (shape.bitlen+7)/8);
}

/*
   {char {x1 y1 x2 y2 x3 y3 x4 y4]]
*/

SRECT bboxes[65536];
U16 depth2id[65536];
char*depth2name[65536];

int hasid(TAG*tag)
{
    if(tag->id == ST_PLACEOBJECT)
	return 1;
    if(tag->id == ST_PLACEOBJECT2 && (tag->data[0] & 2))
	return 1;
    return 0;
}

int hasname(TAG*tag)
{
    if(tag->id == ST_PLACEOBJECT)
	return 0;
    if(tag->id == ST_PLACEOBJECT2 && (tag->data[0] & 0x20))
	return 1;
    return 0;
}

char* getname(TAG*tag)
{
    if(tag->id == ST_PLACEOBJECT)
	return 0;
    if(tag->id == ST_PLACEOBJECT2 && (tag->data[0] & 0x20)) {
	SWFPLACEOBJECT o;
	tag->pos = 0;tag->readBit = 0;
	swf_GetPlaceObject(tag, &o);
	return o.name;
    }
    return 0;
}

MATRIX getmatrix(TAG*tag)
{
    SWFPLACEOBJECT o;
    tag->pos = 0;tag->readBit = 0;
    swf_GetPlaceObject(tag, &o);
    return o.matrix;
}

int main (int argc,char ** argv)
{ 
    TAG*tag;
    SWF swf;
    int fi;
    int frame=0;
    memset(bboxes, 0, sizeof(bboxes));
    memset(depth2name, 0, sizeof(depth2name));

    processargs(argc, argv);

    if(!filename) {
        fprintf(stderr, "You must supply a filename.\n");
        return 1;
    }

    fi = open(filename,O_RDONLY|O_BINARY);

    if (fi<0)
    { 
        perror("Couldn't open file: ");
        exit(1);
    }
    if FAILED(swf_ReadSWF(fi,&swf))
    { 
        fprintf(stderr, "%s is not a valid SWF file or contains errors.\n",filename);
        close(fi);
        exit(1);
    }
    close(fi);

    swf_FoldAll(&swf);
    tag = swf.firstTag;

    if(swifty) {
	printf("{\n");
	printf("{frame %d}\n", frame++);
    }

    while (tag) {
	if (tag->id == ST_DEFINESHAPE ||
	    tag->id == ST_DEFINESHAPE2 ||
	    tag->id == ST_DEFINESHAPE3) {
	    SHAPE2 s;
	    if(verbose) printf("%s\n", swf_TagGetName(tag));
	    swf_ParseDefineShape(tag, &s);
	    swf_Shape2Optimize(&s);
	    tag->len = 2;
	    swf_SetShape2(tag, &s);
	}
	if(swf_isDefiningTag(tag) && tag->id != ST_DEFINESPRITE) {
	    bboxes[swf_GetDefineID(tag)] = swf_GetDefineBBox(tag);
	}
	if(swifty) {
	    if (tag->id == ST_SHOWFRAME) {
		printf("{frame %d}\n", frame++);
	    }
	    if (tag->id == ST_PLACEOBJECT || tag->id == ST_PLACEOBJECT2) {
		if(hasid(tag)) {
		    depth2id[swf_GetDepth(tag)] = swf_GetPlaceID(tag);
		}
		if(hasname(tag)) {
		    depth2name[swf_GetDepth(tag)] = getname(tag);
		}
	    }
	    if (tag->id == ST_PLACEOBJECT || tag->id == ST_PLACEOBJECT2) {
		MATRIX m = getmatrix(tag);
		U16 id = depth2id[swf_GetDepth(tag)];
		char*name = depth2name[swf_GetDepth(tag)];
		char buf[40];
		SRECT bbox = bboxes[id];
		SPOINT p1,p2,p3,p4;
		p1.x = bbox.xmin; p1.y = bbox.ymin;
		p2.x = bbox.xmax; p2.y = bbox.ymin;
		p3.x = bbox.xmin; p3.y = bbox.ymax;
		p4.x = bbox.xmax; p4.y = bbox.ymax;
		p1 = swf_TurnPoint(p1, &m);
		p2 = swf_TurnPoint(p2, &m);
		p3 = swf_TurnPoint(p3, &m);
		p4 = swf_TurnPoint(p4, &m);
		if(!name) {
		    sprintf(buf, "ID%d", id);name = buf;
		}
		printf("{%s {%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f}}\n", name, 
			p1.x/20.0, p1.y/20.0, p2.x/20.0, p2.y/20.0,
			p3.x/20.0, p3.y/20.0, p4.x/20.0, p4.y/20.0);
	    }
	}
	tag = tag->next;
    }
    if(swifty) {
	printf("}\n");
    }

    if(optimize) {
	fi = open(outfilename, O_BINARY | O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(swf_WriteSWF(fi, &swf) < 0) {
	    fprintf(stderr, "Error writing file %s", outfilename);
	    close(fi);
	    exit(1);
	}
	close(fi);
    }

    swf_FreeTags(&swf);
    return 0;
}
