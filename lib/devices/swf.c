/* gfxdevice_swf.c

   Part of the swftools package.

   Copyright (c) 2001,2002,2003,2004,2005 Matthias Kramm <kramm@quiss.org> 

   Swftools is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Swftools is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with swftools; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../config.h"
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(a)
#endif
#include <math.h>
#include "../mem.h"
#include "../log.h"
#include "../rfxswf.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "swf.h"
#include "../gfxpoly.h"
#include "../gfximage.h"

#define CHARDATAMAX 1024
#define CHARMIDX 0
#define CHARMIDY 0

typedef struct _charatposition {
    int charid;
    SWFFONT*font;
    int x;
    int y;
    int size;
    RGBA color;
} charatposition_t;

typedef struct _chararray {
    charatposition_t chr[CHARDATAMAX+1];
    int pos;
    struct _chararray *next;
} chararray_t;

typedef struct _charbuffer {
    MATRIX matrix;
    chararray_t*array;
    chararray_t*last;
    struct _charbuffer *next;
} charbuffer_t;

typedef struct _fontlist
{
    SWFFONT *swffont;
    struct _fontlist*next;
} fontlist_t;

typedef long int twip;

typedef struct _swfmatrix {
    double m11,m12,m21,m22,m31,m32;
} swfmatrix_t;

typedef struct _swfoutput_internal
{
    gfxdevice_t*dev; // the gfxdevice object where this internal struct resides

    double config_dumpfonts;
    double config_ppmsubpixels;
    double config_jpegsubpixels;
    char hasbuttons;
    int config_invisibletexttofront;
    int config_dots;
    int config_simpleviewer;
    int config_opennewwindow;
    int config_ignoredraworder;
    int config_drawonlyshapes;
    int config_frameresets;
    int config_linknameurl;
    int config_jpegquality;
    int config_storeallcharacters;
    int config_enablezlib;
    int config_insertstoptag;
    int config_showimages;
    int config_watermark;
    int config_noclips;
    int config_flashversion;
    int config_reordertags;
    int config_showclipshapes;
    int config_splinemaxerror;
    int config_fontsplinemaxerror;
    int config_filloverlap;
    int config_local_with_network;
    int config_local_with_filesystem;
    int config_protect;
    int config_bboxvars;
    int config_disable_polygon_conversion;
    int config_normalize_polygon_positions;
    int config_alignfonts;
    double config_override_line_widths;
    double config_remove_small_polygons;
    char config_disablelinks;
    RGBA config_linkcolor;
    float config_minlinewidth;
    double config_caplinewidth;
    char* config_linktarget;
    char*config_internallinkfunction;
    char*config_externallinkfunction;
    char config_animate;
    double config_framerate;

    SWF* swf;

    fontlist_t* fontlist;

    char storefont;

    MATRIX page_matrix;

    TAG *tag;
    int currentswfid;
    int startids;
    int depth;
    int startdepth;
    int linewidth;
    
    SHAPE* shape;
    int shapeid;
    int textmode;

    int watermarks;
    
    int fillstyleid;
    int linestyleid;
    int swflastx;
    int swflasty;
    int lastwasfill;
    int shapeisempty;
    char fill;
    int min_x,max_x;
    int min_y,max_y;
    TAG* cliptags[128];
    int clipshapes[128];
    U32 clipdepths[128];
    int clippos;

    /* image cache */
    /*
    int pic_xids[1024];
    int pic_yids[1024];
    int pic_ids[1024];
    int pic_width[1024];
    int pic_height[1024];
    int picpos;
    */

    int frameno;
    int lastframeno;
    
    char fillstylechanged;

    int jpeg; //next image type
    
    int bboxrectpos;
    SRECT bboxrect;

    SRECT pagebbox;

    gfxline_t*stored_clipshapes; //for config_showclipshapes

    charbuffer_t* chardata;
    charbuffer_t* topchardata; //chars supposed to be above everything else

    int firstpage;
    char pagefinished;

    char overflow;

    int current_font_size;
    MATRIX fontmatrix;
    double lastfontm11,lastfontm12,lastfontm21,lastfontm22;
    SWFFONT *swffont;
    RGBA strokergb;
    RGBA fillrgb;
    int drawmode;

    int shapeposx;
    int shapeposy;

    char* mark;

} swfoutput_internal;

static const int NO_FONT3=0;
    
static void swf_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform);
static int  swf_setparameter(gfxdevice_t*driver, const char*key, const char*value);
static void swf_drawstroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit);
static void swf_startclip(gfxdevice_t*dev, gfxline_t*line);
static void swf_endclip(gfxdevice_t*dev);
static void swf_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit);
static void swf_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color);
static void swf_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix);
static void swf_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix);
static void swf_addfont(gfxdevice_t*dev, gfxfont_t*font);
static void swf_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text);
static void swf_startframe(gfxdevice_t*dev, int width, int height);
static void swf_endframe(gfxdevice_t*dev);
static void swfoutput_namedlink(gfxdevice_t*dev, char*name, gfxline_t*points);
static void swfoutput_linktopage(gfxdevice_t*dev, int page, gfxline_t*points);
static void swfoutput_linktourl(gfxdevice_t*dev, const char*url, gfxline_t*points);

static gfxresult_t* swf_finish(gfxdevice_t*driver);

static swfoutput_internal* init_internal_struct()
{
    swfoutput_internal*i = (swfoutput_internal*)malloc(sizeof(swfoutput_internal));
    memset(i, 0, sizeof(swfoutput_internal));

    i->storefont = 0;
    i->currentswfid = 0;
    i->depth = 0;
    i->overflow = 0;
    i->startdepth = 0;
    i->linewidth = 0;
    i->shapeid = -1;
    i->textmode = 0;
    i->frameno = 0;
    i->lastframeno = 0;

    i->mark = 0;

    i->fillstyleid;
    i->linestyleid;
    i->swflastx=0;
    i->swflasty=0;
    i->lastwasfill = 0;
    i->shapeisempty = 1;
    i->fill = 0;
    i->clippos = 0;

    i->fillstylechanged = 0;

    i->bboxrectpos = -1;
    i->chardata = 0;
    i->firstpage = 1;
    i->pagefinished = 1;

    i->config_disablelinks=0;
    i->config_dumpfonts=0;
    i->config_ppmsubpixels=0;
    i->config_jpegsubpixels=0;
    i->config_opennewwindow=1;
    i->config_ignoredraworder=0;
    i->config_drawonlyshapes=0;
    i->config_jpegquality=85;
    i->config_storeallcharacters=0;
    i->config_dots=1;
    i->config_enablezlib=0;
    i->config_insertstoptag=0;
    i->config_flashversion=6;
    i->config_framerate=0.25;
    i->config_splinemaxerror=1;
    i->config_fontsplinemaxerror=1;
    i->config_filloverlap=0;
    i->config_local_with_network=0;
    i->config_local_with_filesystem=0;
    i->config_protect=0;
    i->config_bboxvars=0;
    i->config_override_line_widths=0;
    i->config_showclipshapes=0;
    i->config_minlinewidth=0.05;
    i->config_caplinewidth=1;
    i->config_linktarget=0;
    i->config_internallinkfunction=0;
    i->config_externallinkfunction=0;
    i->config_reordertags=1;
    i->config_linknameurl=0;

    i->config_linkcolor.r = i->config_linkcolor.g = i->config_linkcolor.b = 255;
    i->config_linkcolor.a = 0x40;

    return i;
};

static int id_error = 0;

static U16 getNewID(gfxdevice_t* dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->currentswfid == 65535) {
	if(!id_error) {
	    msg("<error> ID Table overflow");
	    msg("<error> This file is too complex to render- SWF only supports 65536 shapes at once");
	}
	id_error=1;
	i->overflow = 1;
	exit(1);
    }
    return ++i->currentswfid;
}
static U16 getNewDepth(gfxdevice_t* dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->depth == 65520) {
	if(!id_error) {
	    msg("<error> Depth Table overflow");
	    msg("<error> This file is too complex to render- SWF only supports 65536 shapes at once");
	}
	id_error=1;
	i->overflow = 1;
	exit(1);
    }
    return ++i->depth;
}

static void startshape(gfxdevice_t* dev);
static void starttext(gfxdevice_t* dev);
static void endshape(gfxdevice_t* dev);
static void endtext(gfxdevice_t* dev);

typedef struct _plotxy
{
    double x,y;
} plotxy_t;

static inline int twipsnap(double f)
{
    /* if(f < -0x40000000/20.0) {
	fprintf(stderr, "Warning: Coordinate underflow (%f)\n", f);
	f = -0x40000000/20.0;
    } else if(f>0x3fffffff/20.0) {
	fprintf(stderr, "Warning: Coordinate overflow (%f)\n", f);
	f = 0x3fffffff/20.0;
    }*/

    /* clamp coordinates to a rectangle with the property that we
       can represent a line from the upper left corner to the upper
       right corner using no more than 64 strokes */
    const double min = -(1<<(18+4))/20.0;
    const double max = ((1<<(18+4))-1)/20.0;
    if(f < min) {
	fprintf(stderr, "Warning: Coordinate underflow (%f)\n", f);
	f = min;
    } else if(f>max) {
	fprintf(stderr, "Warning: Coordinate overflow (%f)\n", f);
	f = max;
    }

    return (int)(f*20);
}

// write a move-to command into the swf
static int movetoxy(gfxdevice_t*dev, TAG*tag, plotxy_t p0)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    int rx = twipsnap(p0.x);
    int ry = twipsnap(p0.y);
    if(rx!=i->swflastx || ry!=i->swflasty || i->fillstylechanged) {
      swf_ShapeSetMove (tag, i->shape, rx,ry);
      i->fillstylechanged = 0;
      i->swflastx=rx;
      i->swflasty=ry;
      return 1;
    }
    return 0;
}
static int moveto(gfxdevice_t*dev, TAG*tag, double  x, double y)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    plotxy_t p;
    p.x = x;
    p.y = y;
    return movetoxy(dev, tag, p);
}
static void addPointToBBox(gfxdevice_t*dev, int px, int py) 
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    SPOINT p;
    p.x = px;
    p.y = py;
    if(i->fill) {
	swf_ExpandRect(&i->bboxrect, p);
    } else {
	swf_ExpandRect3(&i->bboxrect, p, i->linewidth*3/2);
    }
}

/*static void plot(gfxdevice_t*dev, int x, int y, TAG*tag)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    int width = i->linewidth/4;
    if(width > 5)
	width = 5;
    ////square
    //swf_ShapeSetLine(tag, i->shape,-width,-width);
    //swf_ShapeSetLine(tag, i->shape,width*2,0);
    //swf_ShapeSetLine(tag, i->shape,0,width*2);
    //swf_ShapeSetLine(tag, i->shape,-width*2,0);
    //swf_ShapeSetLine(tag, i->shape,0,-width*2);
    //swf_ShapeSetLine(tag, i->shape,width,width);
   
    // diamond
    swf_ShapeSetLine(tag, i->shape,-width,0);
    swf_ShapeSetLine(tag, i->shape,width,-width);
    swf_ShapeSetLine(tag, i->shape,width,width);
    swf_ShapeSetLine(tag, i->shape,-width,width);
    swf_ShapeSetLine(tag, i->shape,-width,-width);
    swf_ShapeSetLine(tag, i->shape,width,0);

    addPointToBBox(dev, x-width ,y-width);
    addPointToBBox(dev, x+width ,y+width);
}*/

// write a line-to command into the swf
static void linetoxy(gfxdevice_t*dev, TAG*tag, plotxy_t p0)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    int px = twipsnap(p0.x);
    int py = twipsnap(p0.y);
    int rx = (px-i->swflastx);
    int ry = (py-i->swflasty);
    if(rx|ry) {
	swf_ShapeSetLine (tag, i->shape, rx,ry);
	addPointToBBox(dev, i->swflastx,i->swflasty);
	addPointToBBox(dev, px,py);
    } /* this is a nice idea, but doesn't work with current flash
         players (the pixel will be invisible if they're not
         precisely on a pixel boundary) 
         Besides, we should only do this if this lineto itself
         is again followed by a "move".
         else if(!i->fill && i->config_dots) {
       // treat lines of length 0 as plots, making them
       // at least 1 twip wide so Flash will display them
	//plot(dev, i->swflastx, i->swflasty, tag);
	swf_ShapeSetLine (tag, i->shape, rx+1,ry);
    }*/

    i->shapeisempty = 0;
    i->swflastx+=rx;
    i->swflasty+=ry;
}
static void lineto(gfxdevice_t*dev, TAG*tag, double x, double y)
{
    plotxy_t p;
    p.x = x;
    p.y = y;
    linetoxy(dev,tag, p);
}

// write a spline-to command into the swf
static void splineto(gfxdevice_t*dev, TAG*tag, plotxy_t control,plotxy_t end)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    int lastlastx = i->swflastx;
    int lastlasty = i->swflasty;

    int cx = (twipsnap(control.x)-i->swflastx);
    int cy = (twipsnap(control.y)-i->swflasty);
    i->swflastx += cx;
    i->swflasty += cy;
    int ex = (twipsnap(end.x)-i->swflastx);
    int ey = (twipsnap(end.y)-i->swflasty);
    i->swflastx += ex;
    i->swflasty += ey;
    
    if((cx || cy) && (ex || ey)) {
        swf_ShapeSetCurve(tag, i->shape, cx,cy,ex,ey);
        addPointToBBox(dev, lastlastx   ,lastlasty   );
        addPointToBBox(dev, lastlastx+cx,lastlasty+cy);
        addPointToBBox(dev, lastlastx+cx+ex,lastlasty+cy+ey);
    } else if(cx || cy || ex || ey) {
        swf_ShapeSetLine(tag, i->shape, cx+ex,cy+ey);
        addPointToBBox(dev, lastlastx   ,lastlasty   );
        addPointToBBox(dev, lastlastx+cx,lastlasty+cy);
        addPointToBBox(dev, lastlastx+cx+ex,lastlasty+cy+ey);
    }

    i->shapeisempty = 0;
}

/* write a line, given two points and the transformation
   matrix. */
/*static void line(gfxdevice_t*dev, TAG*tag, plotxy_t p0, plotxy_t p1)
{
    moveto(dev, tag, p0);
    lineto(dev, tag, p1);
}*/

void resetdrawer(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    i->swflastx = 0;
    i->swflasty = 0;
}

static void stopFill(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->lastwasfill!=0)
    {
	swf_ShapeSetStyle(i->tag,i->shape,i->linestyleid,0x8000,0);
	i->fillstylechanged = 1;
	i->lastwasfill = 0;
    }
}
static void startFill(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->lastwasfill!=1)
    {
	swf_ShapeSetStyle(i->tag,i->shape,0x8000,i->fillstyleid,0);
	i->fillstylechanged = 1;
	i->lastwasfill = 1;
    }
}

static inline int colorcompare(RGBA*a,RGBA*b)
{

    if(a->r!=b->r ||
       a->g!=b->g ||
       a->b!=b->b ||
       a->a!=b->a) {
        return 0;
    }
    return 1;
}

static SRECT getcharacterbbox(chararray_t*chardata, MATRIX* m, int flashversion)
{
    SRECT r;
    char debug = 0;
    memset(&r, 0, sizeof(r));

    int t;
    if(debug) printf("\n");

    double div = 1.0 / 1024.0;
    if(flashversion>=8 && !NO_FONT3) {
	div = 1.0 / 20480.0;
    }

    while(chardata) {
	for(t=0;t<chardata->pos;t++) {
	    charatposition_t*chr = &chardata->chr[t];
	    SRECT b = chr->font->layout->bounds[chr->charid];
	    b.xmin = floor((b.xmin*(double)chr->size) *div);
	    b.ymin = floor((b.ymin*(double)chr->size) *div);
	    b.xmax = ceil((b.xmax*(double)chr->size)  *div);
	    b.ymax = ceil((b.ymax*(double)chr->size)  *div);

	    b.xmin += chr->x;
	    b.ymin += chr->y;
	    b.xmax += chr->x;
	    b.ymax += chr->y;

	    /* until we solve the INTERNAL_SCALING problem (see below)
	       make sure the bounding box is big enough */
	    b.xmin -= 20;
	    b.ymin -= 20;
	    b.xmax += 20;
	    b.ymax += 20;

	    b = swf_TurnRect(b, m);

	    if(debug) printf("(%f,%f,%f,%f) -> (%f,%f,%f,%f) [font %d, char %d]\n",
		    chr->font->layout->bounds[chr->charid].xmin/20.0,
		    chr->font->layout->bounds[chr->charid].ymin/20.0,
		    chr->font->layout->bounds[chr->charid].xmax/20.0,
		    chr->font->layout->bounds[chr->charid].ymax/20.0,
		    b.xmin/20.0,
		    b.ymin/20.0,
		    b.xmax/20.0,
		    b.ymax/20.0,
		    chr->font->id,
		    chr->charid);
	    swf_ExpandRect2(&r, &b);
	}
	chardata = chardata->next;
    }
    if(debug) printf("-----> (%f,%f,%f,%f)\n",
	    r.xmin/20.0,
	    r.ymin/20.0,
	    r.xmax/20.0,
	    r.ymax/20.0);
    return r;
}

static chararray_t*chararray_reverse(chararray_t*buf)
{
    chararray_t*prev = 0;
    while(buf) {
	chararray_t*next = buf->next;
	buf->next = prev;
	prev = buf;
	buf = next;
    }
    return prev;
}

static void chararray_writetotag(chararray_t*_chardata, TAG*tag)
{
    SWFFONT font;
    RGBA color;
    color.r = _chardata?_chardata->chr[0].color.r^255:0;
    color.g = 0;
    color.b = 0;
    color.a = 0;
    SWFFONT*lastfont;
    int lastx;
    int lasty;
    int lastsize;
    int lastchar;
    int charids[128];
    int charadvance[128];
    int charstorepos;
    int pass;
    int glyphbits=1; //TODO: can this be zero?
    int advancebits=1;

    if(tag->id != ST_DEFINETEXT &&
        tag->id != ST_DEFINETEXT2) {
        msg("<error> internal error: charbuffer_put needs an text tag, not %d\n",tag->id);
        exit(1);
    }
    if(!_chardata) {
        msg("<warning> charbuffer_put called with zero characters");
    }

    for(pass = 0; pass < 2; pass++)
    {
        charstorepos = 0;
        lastfont = 0;
        lastx = CHARMIDX;
        lasty = CHARMIDY;
        lastsize = -1;
	lastchar = -1;

        if(pass==1)
        {
            advancebits++; // add sign bit
            swf_SetU8(tag, glyphbits);
            swf_SetU8(tag, advancebits);
        }

	chararray_t*chardata = _chardata;

	while(chardata) {
	    int t;
	    
	    assert(!chardata->next || chardata->pos == CHARDATAMAX);
	    assert(chardata->pos);

	    int to = chardata->next?chardata->pos-1:chardata->pos;

	    for(t=0;t<=to;t++)
	    {
		char islast = t==chardata->pos;

		charatposition_t*chr = &chardata->chr[t];

		if(lastfont != chr->font || 
			lastx!=chr->x ||
			lasty!=chr->y ||
			!colorcompare(&color, &chardata->chr[t].color) ||
			charstorepos==127 ||
			lastsize != chardata->chr[t].size ||
			islast)
		{
		    if(charstorepos && pass==0)
		    {
			int s;
			for(s=0;s<charstorepos;s++)
			{
			    while(charids[s]>=(1<<glyphbits))
				glyphbits++;
			    while(charadvance[s]>=(1<<advancebits))
				advancebits++;
			}
		    }
		    if(charstorepos && pass==1)
		    {
			tag->writeBit = 0; // Q&D
			swf_SetBits(tag, 0, 1); // GLYPH Record
			swf_SetBits(tag, charstorepos, 7); // number of glyphs
			int s;
			for(s=0;s<charstorepos;s++)
			{
			    swf_SetBits(tag, charids[s], glyphbits);
			    swf_SetBits(tag, charadvance[s], advancebits);
			}
		    }
		    charstorepos = 0;

		    if(pass == 1 && !islast)
		    {
			RGBA*newcolor=0;
			SWFFONT*newfont=0;
			int newx = 0;
			int newy = 0;
			if(lastx != chr->x ||
			   lasty != chr->y)
			{
			    newx = chr->x;
			    newy = chr->y;
			    if(newx == 0)
				newx = SET_TO_ZERO;
			    if(newy == 0)
				newy = SET_TO_ZERO;
			}
			if(!colorcompare(&color, &chr->color)) 
			{
			    color = chr->color;
			    newcolor = &color;
			}
			font.id = chr->font->id;
			if(lastfont != chr->font || lastsize != chr->size)
			    newfont = &font;

			tag->writeBit = 0; // Q&D
			swf_TextSetInfoRecord(tag, newfont, chr->size, newcolor, newx, newy);
		    }

		    lastfont = chr->font;
		    lastx = chr->x;
		    lasty = chr->y;
		    lastsize = chr->size;
		}

		if(islast)
			break;

		int nextx = chr->x;
		if(t<chardata->pos-1) nextx = chardata->chr[t+1].x;
		if(t==chardata->pos-1 && chardata->next) nextx = chardata->next->chr[0].x;
		int dx = nextx-chr->x;
		
		int advance;
		if(dx>=0 && (dx<(1<<(advancebits-1)) || pass==0)) {
		   advance = dx;
		   lastx=nextx;
		} else {
		   advance = 0;
		   lastx=chr->x;
		}

		charids[charstorepos] = chr->charid;
		charadvance[charstorepos] = advance;
		lastchar = chr->charid;
		charstorepos ++;
	    }
	    chardata = chardata->next;
	}
    }
}

static void chararray_destroy(chararray_t*chr)
{
    while(chr) {
	chararray_t*next = chr->next;
	chr->next = 0;
	free(chr);
	chr = next;
    }
}

static inline int matrix_diff(MATRIX*m1, MATRIX*m2)
{
    return memcmp(m1,m2,sizeof(MATRIX));
}
static charbuffer_t*charbuffer_append(charbuffer_t*buf, SWFFONT*font, int charid, int x,int y, int size, RGBA color, MATRIX*m)
{
    if(!buf || matrix_diff(&buf->matrix,m)) {
	charbuffer_t*n = rfx_calloc(sizeof(charbuffer_t));
	n->matrix = *m;
	n->next = buf;
	buf = n;
    }
    if(!buf->last || buf->last->pos == CHARDATAMAX) {
	chararray_t*n = rfx_calloc(sizeof(chararray_t));
	if(!buf->array) {
	    buf->array = buf->last = n;
	} else {
	    buf->last->next = n;
	    buf->last = n;
	}
    }
    chararray_t*a = buf->last;
    a->chr[a->pos].font = font;
    a->chr[a->pos].charid = charid;
    a->chr[a->pos].x = x;
    a->chr[a->pos].y = y;
    a->chr[a->pos].color = color;
    a->chr[a->pos].size = size;
    a->pos++;
    return buf;
}

/* Notice: we can only put chars in the range -1639,1638 (-32768/20,32768/20).
   So if we set this value to high, the char coordinates will overflow.
   If we set it to low, however, the char positions will be inaccurate */
#define GLYPH_SCALE 1

static void chararray_writetodev(gfxdevice_t*dev, chararray_t*array, MATRIX*matrix, char invisible)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
   
    int textid = getNewID(dev);
    i->tag = swf_InsertTag(i->tag,ST_DEFINETEXT2);
    swf_SetU16(i->tag, textid);
    SRECT r;
    r = getcharacterbbox(array, matrix, i->config_flashversion);
    r = swf_ClipRect(i->pagebbox, r);
    swf_SetRect(i->tag,&r);
    swf_SetMatrix(i->tag, matrix);
    msg("<trace> Placing text as ID %d", textid);
    chararray_writetotag(array, i->tag);
    i->chardata = 0;

    swf_SetU8(i->tag,0);

    if(i->swf->fileVersion >= 8) {
	i->tag = swf_InsertTag(i->tag, ST_CSMTEXTSETTINGS);
	swf_SetU16(i->tag, textid);

	//swf_SetU8(i->tag, /*subpixel grid*/(2<<3)|/*flashtype*/0x40);
	swf_SetU8(i->tag, /*grid*/(1<<3)|/*flashtype*/0x40);
	//swf_SetU8(i->tag, /*no grid*/(0<<3)|/*flashtype*/0x40);

	swf_SetU32(i->tag, 0);//thickness
	swf_SetU32(i->tag, 0);//sharpness
	//swf_SetU32(i->tag, 0x20000);//thickness
	//swf_SetU32(i->tag, 0x800000);//sharpness
	swf_SetU8(i->tag, 0);//reserved
    }
    if(invisible && i->config_flashversion>=8) {
	i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT3);
	swf_ObjectPlaceBlend(i->tag,textid,getNewDepth(dev),&i->page_matrix,NULL,NULL,BLENDMODE_MULTIPLY);
    } else {
	i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
	swf_ObjectPlace(i->tag,textid,getNewDepth(dev),&i->page_matrix,NULL,NULL);
    }
}

static void charbuffer_writetodevandfree(gfxdevice_t*dev, charbuffer_t*buf, char invisible)
{
    while(buf) {
	charbuffer_t*next = buf->next;buf->next = 0;
	chararray_writetodev(dev, buf->array, &buf->matrix, invisible);
	chararray_destroy(buf->array);
	free(buf);
	buf = next;
    }
}

static void endtext(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(!i->textmode)
        return;
    charbuffer_writetodevandfree(dev, i->chardata, 0);i->chardata = 0;
    i->textmode = 0;
}

static int watermark2_width=47;
static int watermark2_height=11;
static int watermark2[47] = {95,1989,71,0,2015,337,1678,0,2015,5,1921,320,1938,25,2006,1024,
                             1042,21,13,960,1039,976,8,2000,1359,1088,31,1989,321,1728,0,1152,
                             1344,832,0,1984,0,896,1088,1088,896,0,1984,128,256,512,1984};

static void draw_watermark(gfxdevice_t*dev, gfxbbox_t r, char drawall)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    double wx = r.xmax / 5.0;
    double tx = r.xmax*4.0 / 5.0;
    double ty = r.ymax-wx*watermark2_height/watermark2_width;
    double sx = (r.xmax - tx) / watermark2_width;
    double sy = (r.ymax - ty) / watermark2_height;
    double px = sx-0.5;
    double py = sy-0.5;
    if(ty > 0 && px > 1.0 && py > 1.0) {
	int x,y;
	for(y=0;y<watermark2_height;y++)
	for(x=0;x<watermark2_width;x++) {
	    if(((watermark2[x]>>y)&1)) {
		if(!drawall && rand()%5)
		    continue;
		unsigned int b = rand();
		moveto(dev, i->tag, x*sx+tx+((b>>1)&1)/20.0, y*sy+ty+((b>>3)&1)/20.0);
		lineto(dev, i->tag, x*sx+px+tx+((b>>2)&1)/20.0, y*sy+ty+((b>>3)&1)/20.0);
		lineto(dev, i->tag, x*sx+px+tx+((b>>2)&1)/20.0, y*sy+py+ty+((b>>4)&1)/20.0);
		lineto(dev, i->tag, x*sx+tx+((b>>1)&1)/20.0, y*sy+py+ty+((b>>4)&1)/20.0);
		lineto(dev, i->tag, x*sx+tx+((b>>1)&1)/20.0, y*sy+ty+((b>>3)&1)/20.0);
	    }
	}
    }
}

static void swfoutput_setfillcolor(gfxdevice_t* dev, U8 r, U8 g, U8 b, U8 a)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->fillrgb.r == r &&
       i->fillrgb.g == g &&
       i->fillrgb.b == b &&
       i->fillrgb.a == a) return;
    if(i->shapeid>=0)
     endshape(dev);

    i->fillrgb.r = r;
    i->fillrgb.g = g;
    i->fillrgb.b = b;
    i->fillrgb.a = a;
}
static void insert_watermark(gfxdevice_t*dev, char drawall)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(!drawall && i->watermarks>20)
	return;
    endshape(dev);
    endtext(dev);
   
    if(drawall) {
	swfoutput_setfillcolor(dev, 0,0,255,192);
    } else {
	swfoutput_setfillcolor(dev, rand(),rand(),rand(),(rand()&127)+128);
    }
    startshape(dev);
    startFill(dev);

    gfxbbox_t r; r.xmin = r.ymin = 0;
    r.xmax = i->max_x;
    r.ymax = i->max_y;
    draw_watermark(dev, r, drawall);
    endshape(dev);
    i->watermarks++;
}

static void drawoutline(gfxdevice_t*dev, gfxline_t*line);

static void endpage(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->pagefinished)
	return;
    
    if(i->shapeid>=0)
	endshape(dev);
    if(i->textmode)
	endtext(dev);
    if(i->topchardata) {
	charbuffer_writetodevandfree(dev, i->topchardata, 1);
	i->topchardata=0;
    }
    
    while(i->clippos)
        dev->endclip(dev);

    if(i->stored_clipshapes) {
        // in case of config_showclipshapes
	drawoutline(dev, i->stored_clipshapes);
    }

    if(i->config_watermark) {
	insert_watermark(dev, 1);
    }

    i->pagefinished = 1;
}

static void addViewer(gfxdevice_t* dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    SHAPE*s;
    RGBA button_colors[3]= {{0xbf,0x00,0x00,0x80},{0xbf,0x20,0x20,0xc0}, {0xbf,0xc0,0xc0,0xff}};
    int ids[6];
    int button_sizex = 20;
    int button_sizey = 20; 
    int t;
    RGBA black = {255,0,0,0};
    for(t=0;t<6;t++) {
	i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE3);
	swf_ShapeNew(&s);
	int ls1 = swf_ShapeAddLineStyle(s,40,&black);
	int fs1 = swf_ShapeAddSolidFillStyle(s,&button_colors[t/2]);
	int shapeid = ids[t] = getNewID(dev);
	swf_SetU16(i->tag,shapeid);
	SRECT r;
	r.xmin = -20*button_sizex;
	r.xmax = 20*button_sizex; 
	r.ymin = 0;
	r.ymax = 40*button_sizey;
	swf_SetRect(i->tag,&r);              // set shape bounds
	swf_SetShapeHeader(i->tag,s);        // write all styles to tag
	swf_ShapeSetAll(i->tag,s,0*button_sizex,0,ls1,fs1,0);
	swf_ShapeSetLine(i->tag,s,(1-(t&1)*2)*20*button_sizex,20*button_sizey);
	swf_ShapeSetLine(i->tag,s,-(1-(t&1)*2)*20*button_sizex,20*button_sizey);
	swf_ShapeSetLine(i->tag,s,0,-40*button_sizey);
	swf_ShapeSetEnd(i->tag);   // finish drawing
	swf_ShapeFree(s);   // clean shape structure (which isn't needed anymore after writing the tag)
    }
    ActionTAG*a1=0,*a2=0,*a3=0;
    a1 = action_NextFrame(a1);
    a1 = action_Stop(a1);
    a1 = action_End(a1);
    
    a2 = action_PreviousFrame(a2);
    a2 = action_Stop(a2);
    a2 = action_End(a2);
    
    a3 = action_Stop(a3);
    a3 = action_End(a3);

    i->tag = swf_InsertTag(i->tag, ST_DOACTION);
    swf_ActionSet(i->tag,a3);

    i->tag = swf_InsertTag(i->tag,ST_DEFINEBUTTON);
    int buttonid1 = getNewID(dev);
    swf_SetU16(i->tag, buttonid1);
    swf_ButtonSetRecord(i->tag,BS_UP|BS_HIT,ids[0],1,NULL,NULL);
    swf_ButtonSetRecord(i->tag,BS_OVER,ids[2],1,NULL,NULL);
    swf_ButtonSetRecord(i->tag,BS_DOWN,ids[4],1,NULL,NULL);
    swf_SetU8(i->tag,0); // end of button records
    swf_ActionSet(i->tag,a1);
    
    i->tag = swf_InsertTag(i->tag,ST_DEFINEBUTTON);
    int buttonid2 = getNewID(dev);
    swf_SetU16(i->tag, buttonid2);
    swf_ButtonSetRecord(i->tag,BS_UP|BS_HIT,ids[1],1,NULL,NULL);
    swf_ButtonSetRecord(i->tag,BS_OVER,ids[3],1,NULL,NULL);
    swf_ButtonSetRecord(i->tag,BS_DOWN,ids[5],1,NULL,NULL);
    swf_SetU8(i->tag,0); // end of button records
    swf_ActionSet(i->tag,a2);
  
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    MATRIX m;
    swf_GetMatrix(0, &m);
    m.tx = button_sizex*20+200;
    swf_ObjectPlace(i->tag, buttonid2, 65534,&m,0,0);
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    m.tx = button_sizex*20+200+200;
    swf_ObjectPlace(i->tag, buttonid1, 65535,&m,0,0);
}


void swf_startframe(gfxdevice_t*dev, int width, int height)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->firstpage) {
	if(i->config_protect) {
	    i->tag = swf_InsertTag(i->tag, ST_PROTECT);
	    i->config_protect = 0;
	}
	if(i->config_simpleviewer) {
	    addViewer(dev);
	}
    }
    
    if(!i->firstpage && !i->pagefinished)
        endpage(dev);

    msg("<verbose> Starting new SWF page of size %dx%d", width, height);

    swf_GetMatrix(0, &i->page_matrix);
    i->page_matrix.tx = 0;
    i->page_matrix.ty = 0;
    i->min_x = 0;
    i->min_y = 0;
    i->max_x = width;
    i->max_y = height;
    i->watermarks = 0;

    /* create a bbox structure with the page size. This is used
       for clipping shape and text bounding boxes. As we don't want to
       generate bounding boxes which extend beyond the movie size (in
       order to not confuse Flash), we clip everything against i->pagebbox */
    i->pagebbox.xmin = 0;
    i->pagebbox.ymin = 0;
    i->pagebbox.xmax = width*20;
    i->pagebbox.ymax = height*20;

    /* increase SWF's bounding box */
    swf_ExpandRect2(&i->swf->movieSize, &i->pagebbox);

    i->lastframeno = i->frameno;
    i->firstpage = 0;
    i->pagefinished = 0;
    i->chardata = 0;
}

void swf_endframe(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    
    if(!i->pagefinished)
        endpage(dev);

    if( (i->swf->fileVersion <= 8) && (i->config_insertstoptag) ) {
	ActionTAG*atag=0;
	atag = action_Stop(atag);
	atag = action_End(atag);
	i->tag = swf_InsertTag(i->tag,ST_DOACTION);
	swf_ActionSet(i->tag,atag);
    }
    i->tag = swf_InsertTag(i->tag,ST_SHOWFRAME);
    i->frameno ++;
    
    for(i->depth;i->depth>i->startdepth;i->depth--) {
        i->tag = swf_InsertTag(i->tag,ST_REMOVEOBJECT2);
        swf_SetU16(i->tag,i->depth);
    }
    i->depth = i->startdepth;

    if(i->config_frameresets) {
	for(i->currentswfid;i->currentswfid>i->startids;i->currentswfid--) {
	    i->tag = swf_InsertTag(i->tag,ST_FREECHARACTER);
	    swf_SetU16(i->tag,i->currentswfid);
	}
	i->currentswfid = i->startids;
    }
}

static void setBackground(gfxdevice_t*dev, int x1, int y1, int x2, int y2)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    RGBA rgb;
    rgb.a = rgb.r = rgb.g = rgb.b = 0xff;
    SRECT r;
    SHAPE* s;
    int ls1=0,fs1=0;
    int shapeid = getNewID(dev);
    r.xmin = x1;
    r.ymin = y1;
    r.xmax = x2;
    r.ymax = y2;
    i->tag = swf_InsertTag(i->tag, ST_DEFINESHAPE);
    swf_ShapeNew(&s);
    fs1 = swf_ShapeAddSolidFillStyle(s, &rgb);
    swf_SetU16(i->tag,shapeid);
    swf_SetRect(i->tag,&r);
    swf_SetShapeHeader(i->tag,s);
    swf_ShapeSetAll(i->tag,s,x1,y1,ls1,fs1,0);
    swf_ShapeSetLine(i->tag,s,(x2-x1),0);
    swf_ShapeSetLine(i->tag,s,0,(y2-y1));
    swf_ShapeSetLine(i->tag,s,(x1-x2),0);
    swf_ShapeSetLine(i->tag,s,0,(y1-y2));
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(s);
    i->tag = swf_InsertTag(i->tag, ST_PLACEOBJECT2);
    swf_ObjectPlace(i->tag,shapeid,getNewDepth(dev),0,0,0);
    i->tag = swf_InsertTag(i->tag, ST_PLACEOBJECT2);
    swf_ObjectPlaceClip(i->tag,shapeid,getNewDepth(dev),0,0,0,65535);
}

/* initialize the swf writer */
void gfxdevice_swf_init(gfxdevice_t* dev)
{
    memset(dev, 0, sizeof(gfxdevice_t));
    
    dev->name = "swf";

    dev->internal = init_internal_struct(); // set config to default values

    dev->startpage = swf_startframe;
    dev->endpage = swf_endframe;
    dev->finish = swf_finish;
    dev->fillbitmap = swf_fillbitmap;
    dev->setparameter = swf_setparameter;
    dev->stroke = swf_stroke;
    dev->startclip = swf_startclip;
    dev->endclip = swf_endclip;
    dev->fill = swf_fill;
    dev->fillgradient = swf_fillgradient;
    dev->addfont = swf_addfont;
    dev->drawchar = swf_drawchar;
    dev->drawlink = swf_drawlink;

    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    i->dev = dev;

    msg("<verbose> initializing swf output\n", i->max_x,i->max_y);

    i->swffont = 0;
   
    i->swf = (SWF*)rfx_calloc(sizeof(SWF));
    i->swf->fileVersion    = 0;
    i->swf->frameRate      = 0x80;
    i->swf->movieSize.xmin = 0;
    i->swf->movieSize.ymin = 0;
    i->swf->movieSize.xmax = 0;
    i->swf->movieSize.ymax = 0;

    if(i->config_local_with_filesystem) {
        i->swf->fileAttributes = 8; // as3, local-with-filesystem
    } else {
        i->swf->fileAttributes = 9; // as3, local-with-network
    }

    i->swf->firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    i->tag = i->swf->firstTag;
    RGBA rgb;
    rgb.a = rgb.r = rgb.g = rgb.b = 0xff;
    //rgb.r = 0;
    swf_SetRGB(i->tag,&rgb);

    i->startdepth = i->depth = 0;
    i->startids = i->currentswfid = 0;
}

static void startshape(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    SRECT r;

    if(i->shapeid>=0)
	return;
    //if(i->chardatapos && i->chardata[i->chardatapos-1].color.a)
    endtext(dev);

    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE3);

    swf_ShapeNew(&i->shape);
    i->linestyleid = swf_ShapeAddLineStyle(i->shape,i->linewidth,&i->strokergb);
    i->fillstyleid = swf_ShapeAddSolidFillStyle(i->shape,&i->fillrgb);
    if(i->mark) {
	RGBA markcol = {0,i->mark[0],i->mark[1],i->mark[2]};
	swf_ShapeAddSolidFillStyle(i->shape,&markcol);
    }

    i->shapeid = getNewID(dev);
    
    msg("<debug> Using shape id %d", i->shapeid);

    swf_SetU16(i->tag,i->shapeid);  // ID

    i->bboxrectpos = i->tag->len;
    /* changed later */
    swf_SetRect(i->tag,&i->pagebbox);
   
    memset(&i->bboxrect, 0, sizeof(i->bboxrect));

    swf_SetShapeStyles(i->tag,i->shape);
    swf_ShapeCountBits(i->shape,NULL,NULL);
    swf_SetShapeBits(i->tag,i->shape);

    /* TODO: do we really need this? */
    //swf_ShapeSetAll(i->tag,i->shape,/*x*/0,/*y*/0,i->linestyleid,0,0);
    //swf_ShapeSetAll(i->tag,i->shape,/*x*/UNDEFINED_COORD,/*y*/UNDEFINED_COORD,i->linestyleid,0,0);
    i->swflastx=i->swflasty=UNDEFINED_COORD;
    i->lastwasfill = -1;
    i->shapeisempty = 1;
}

static void starttext(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->shapeid>=0)
        endshape(dev);

    if(i->config_watermark) {
	insert_watermark(dev, 0);
    }
    i->textmode = 1;
    i->swflastx=i->swflasty=0;
}
	    

/* TODO: move to ../lib/rfxswf */
void changeRect(gfxdevice_t*dev, TAG*tag, int pos, SRECT*newrect)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    /* determine length of old rect */
    tag->pos = pos;
    tag->readBit = 0;
    SRECT old;
    swf_GetRect(tag, &old);
    swf_ResetReadBits(tag);
    int pos_end = tag->pos;

    int len = tag->len - pos_end;
    U8*data = (U8*)malloc(len);
    memcpy(data, &tag->data[pos_end], len);
    tag->writeBit = 0;
    tag->len = pos;
    swf_SetRect(tag, newrect);
    swf_SetBlock(tag, data, len);
    free(data);
    tag->pos = tag->readBit = 0;
}

void cancelshape(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    /* delete old shape tag */
    TAG*todel = i->tag;
    i->tag = i->tag->prev;
    swf_DeleteTag(0, todel);
    if(i->shape) {swf_ShapeFree(i->shape);i->shape=0;}
    i->shapeid = -1;
    i->bboxrectpos = -1;

//    i->currentswfid--; // doesn't work, for some reason
}

void fixAreas(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(!i->shapeisempty && i->fill &&
       (i->bboxrect.xmin == i->bboxrect.xmax ||
        i->bboxrect.ymin == i->bboxrect.ymax) &&
        i->config_minlinewidth >= 0.001
       ) {
	msg("<debug> Shape has size 0: width=%.2f height=%.2f",
		(i->bboxrect.xmax-i->bboxrect.xmin)/20.0,
		(i->bboxrect.ymax-i->bboxrect.ymin)/20.0
		);
    
	SRECT r = i->bboxrect;
	
	if(r.xmin == r.xmax && r.ymin == r.ymax) {
	    /* this thing comes down to a single dot- nothing to fix here */
	    return;
	}

	cancelshape(dev);

	RGBA save_col = i->strokergb;
	int  save_width = i->linewidth;

	i->strokergb = i->fillrgb;
	i->linewidth = (int)(i->config_minlinewidth*20);
	if(i->linewidth==0) i->linewidth = 1;
	
	startshape(dev);
	stopFill(dev);

	moveto(dev, i->tag, r.xmin/20.0,r.ymin/20.0);
	lineto(dev, i->tag, r.xmax/20.0,r.ymax/20.0);

	i->strokergb = save_col;
	i->linewidth = save_width;
    }
    
}

static void endshape_noput(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->shapeid<0) 
        return;
    //changeRect(dev, i->tag, i->bboxrectpos, &i->bboxrect);
    i->shapeid = -1;
    if(i->shape) {
        swf_ShapeFree(i->shape);
        i->shape=0;
    }
    i->fill=0;
    i->shapeposx=0;
    i->shapeposy=0;
}

static void endshape(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->shapeid<0) 
        return;

    fixAreas(dev);
	
    if(i->shapeisempty ||
       /*bbox empty?*/
       (i->bboxrect.xmin == i->bboxrect.xmax && 
        i->bboxrect.ymin == i->bboxrect.ymax))
    {
	// delete the shape again, we didn't do anything
	msg("<debug> cancelling shape: bbox is (%f,%f,%f,%f)",
		i->bboxrect.xmin /20.0,
		i->bboxrect.ymin /20.0,
		i->bboxrect.xmax /20.0,
		i->bboxrect.ymax /20.0
		);
	cancelshape(dev);
	return;
    }
    
    swf_ShapeSetEnd(i->tag);

    SRECT r = swf_ClipRect(i->pagebbox, i->bboxrect);
    changeRect(dev, i->tag, i->bboxrectpos, &r);

    msg("<trace> Placing shape ID %d", i->shapeid);

    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    MATRIX m = i->page_matrix;
    m.tx += i->shapeposx;
    m.ty += i->shapeposy;
    swf_ObjectPlace(i->tag,i->shapeid,getNewDepth(dev),&m,NULL,NULL);

    swf_ShapeFree(i->shape);
    i->shape = 0;
    i->shapeid = -1;
    i->bboxrectpos = -1;

    i->fill=0;
    i->shapeposx=0;
    i->shapeposy=0;
}

void wipeSWF(SWF*swf)
{
    TAG*tag = swf->firstTag;
    while(tag) {
	TAG*next = tag->next;
	if(tag->id != ST_SETBACKGROUNDCOLOR &&
	   tag->id != ST_END &&
	   tag->id != ST_DOACTION &&
	   tag->id != ST_SHOWFRAME) {
	    swf_DeleteTag(swf, tag);
	}
	tag = next;
    }
}

void swfoutput_finalize(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    if(i->tag && i->tag->id == ST_END)
        return; //already done

    i->swf->fileVersion = i->config_flashversion;
    i->swf->frameRate = i->config_framerate*0x100;

    if(i->config_bboxvars) {
	TAG* tag = swf_InsertTag(i->swf->firstTag, ST_DOACTION);
	ActionTAG*a = 0;
	a = action_PushString(a, "xmin");
	a = action_PushFloat(a, i->swf->movieSize.xmin / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "ymin");
	a = action_PushFloat(a, i->swf->movieSize.ymin / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "xmax");
	a = action_PushFloat(a, i->swf->movieSize.xmax / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "ymax");
	a = action_PushFloat(a, i->swf->movieSize.ymax / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "width");
	a = action_PushFloat(a, (i->swf->movieSize.xmax - i->swf->movieSize.xmin) / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "height");
	a = action_PushFloat(a, (i->swf->movieSize.ymax - i->swf->movieSize.ymin) / 20.0);
	a = action_SetVariable(a);
	a = action_End(a);
	swf_ActionSet(tag, a);
	swf_ActionFree(a);
    }

    if(i->mark) {
	free(i->mark);i->mark = 0;
    }

    endpage(dev);
    fontlist_t *iterator = i->fontlist;
    char use_font3 = i->config_flashversion>=8 && !NO_FONT3;

    while(iterator) {
	TAG*mtag = i->swf->firstTag;
	if(iterator->swffont) {
	    if(!i->config_storeallcharacters) {
		msg("<debug> Reducing font %s", iterator->swffont->name);
		swf_FontReduce(iterator->swffont);
	    }
	    int used = iterator->swffont->use && iterator->swffont->use->used_glyphs;
	    if(used) {
		if(!use_font3) {
		    mtag = swf_InsertTag(mtag, ST_DEFINEFONT2);
		    swf_FontSetDefine2(mtag, iterator->swffont);
		} else {
		    mtag = swf_InsertTag(mtag, ST_DEFINEFONT3);
		    swf_FontSetDefine2(mtag, iterator->swffont);
		}
	    }
	}

        iterator = iterator->next;
    }

    i->tag = swf_InsertTag(i->tag,ST_END);
    TAG* tag = i->tag->prev;
   
    if(use_font3 && i->config_storeallcharacters && i->config_alignfonts) {
	swf_FontPostprocess(i->swf); // generate alignment information
    }

    /* remove the removeobject2 tags between the last ST_SHOWFRAME
       and the ST_END- they confuse the flash player  */
    while(tag->id == ST_REMOVEOBJECT2) {
        TAG* prev = tag->prev;
        swf_DeleteTag(i->swf, tag);
        tag = prev;
    }
    
    if(i->overflow) {
	wipeSWF(i->swf);
    }
    if(i->config_enablezlib || i->config_flashversion>=6) {
	i->swf->compressed = 1;
    }

    /* Add AVM2 actionscript */
    if(i->config_flashversion>=9 && 
            (i->config_insertstoptag || i->hasbuttons) && !i->config_linknameurl) {
        swf_AddButtonLinks(i->swf, i->config_insertstoptag, 
                i->config_internallinkfunction||i->config_externallinkfunction);
    }
//    if(i->config_reordertags)
//	swf_Optimize(i->swf);
}

int swfresult_save(gfxresult_t*gfx, const char*filename)
{
    SWF*swf = (SWF*)gfx->internal;
    int fi;
    if(filename)
     fi = open(filename, O_BINARY|O_CREAT|O_TRUNC|O_WRONLY, 0777);
    else
     fi = 1; // stdout
    
    if(fi<=0) {
	msg("<fatal> Could not create \"%s\". ", FIXNULL(filename));
	return -1;
    }
    
    if FAILED(swf_WriteSWF(fi,swf)) 
        msg("<error> WriteSWF() failed.\n");

    if(filename)
     close(fi);
    return 0;
}
void* swfresult_get(gfxresult_t*gfx, const char*name)
{
    SWF*swf = (SWF*)gfx->internal;
    if(!strcmp(name, "swf")) {
	return (void*)swf_CopySWF(swf);
    } else if(!strcmp(name, "xmin")) {
	return (void*)(ptroff_t)(swf->movieSize.xmin/20);
    } else if(!strcmp(name, "ymin")) {
	return (void*)(ptroff_t)(swf->movieSize.ymin/20);
    } else if(!strcmp(name, "xmax")) {
	return (void*)(ptroff_t)(swf->movieSize.xmax/20);
    } else if(!strcmp(name, "ymax")) {
	return (void*)(ptroff_t)(swf->movieSize.ymax/20);
    } else if(!strcmp(name, "width")) {
	return (void*)(ptroff_t)((swf->movieSize.xmax - swf->movieSize.xmin)/20);
    } else if(!strcmp(name, "height")) {
	return (void*)(ptroff_t)((swf->movieSize.ymax - swf->movieSize.ymin)/20);
    }
    return 0;
}
void swfresult_destroy(gfxresult_t*gfx)
{
    if(gfx->internal) {
	swf_FreeTags((SWF*)gfx->internal);
	free(gfx->internal);
	gfx->internal = 0;
    }
    memset(gfx, 0, sizeof(gfxresult_t));
    free(gfx);
}

static void swfoutput_destroy(gfxdevice_t* dev);

gfxresult_t* swf_finish(gfxdevice_t* dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    gfxresult_t*result;

    if(i->config_linktarget) {
	free(i->config_linktarget);
	i->config_linktarget = 0;
    }

    swfoutput_finalize(dev);
    SWF* swf = i->swf;i->swf = 0;
    swfoutput_destroy(dev);

    result = (gfxresult_t*)rfx_calloc(sizeof(gfxresult_t));
    result->internal = swf;
    result->save = swfresult_save;
    result->write = 0;
    result->get = swfresult_get;
    result->destroy = swfresult_destroy;
    return result;
}

/* Perform cleaning up */
static void swfoutput_destroy(gfxdevice_t* dev) 
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(!i) {
        /* not initialized yet- nothing to destroy */
        return;
    }

    fontlist_t *tmp,*iterator = i->fontlist;
    while(iterator) {
	if(iterator->swffont) {
	    swf_FontFree(iterator->swffont);iterator->swffont=0;
	}
        tmp = iterator;
        iterator = iterator->next;
        free(tmp);
    }
    if(i->swf) {swf_FreeTags(i->swf);free(i->swf);i->swf = 0;}

    free(i);i=0;
    memset(dev, 0, sizeof(gfxdevice_t));
}

static void swfoutput_setstrokecolor(gfxdevice_t* dev, U8 r, U8 g, U8 b, U8 a)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->strokergb.r == r &&
       i->strokergb.g == g &&
       i->strokergb.b == b &&
       i->strokergb.a == a) return;

    if(i->shapeid>=0)
     endshape(dev);
    i->strokergb.r = r;
    i->strokergb.g = g;
    i->strokergb.b = b;
    i->strokergb.a = a;
}

//#define ROUND_UP 19
//#define ROUND_UP 10

static void swfoutput_setlinewidth(gfxdevice_t*dev, double _linewidth)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->linewidth == (U16)(_linewidth*20+19.0/20.0))
        return;
    if(i->shapeid>=0)
	endshape(dev);
    i->linewidth = (U16)(_linewidth*20+19.0/20.0);
}


static void drawlink(gfxdevice_t*dev, ActionTAG*,ActionTAG*, gfxline_t*points, char mouseover, char*type, const char*url);
static void swfoutput_namedlink(gfxdevice_t*dev, char*name, gfxline_t*points);
static void swfoutput_linktopage(gfxdevice_t*dev, int page, gfxline_t*points);
static void swfoutput_linktourl(gfxdevice_t*dev, const char*url, gfxline_t*points);

/*void swfoutput_drawlink(gfxdevice_t*dev, char*url, gfxline_t*points)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    dev->drawlink(dev, points, url);
}*/

void swf_drawlink(gfxdevice_t*dev, gfxline_t*points, const char*url, const char*text)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    if(i->config_disablelinks)
        return;

    if(!strncmp("http://pdf2swf:", url, 15)) {
	char*tmp = strdup(url);
	int l = strlen(tmp);
	if(tmp[l-1] == '/')
	   tmp[l-1] = 0;
	swfoutput_namedlink(dev, tmp+15, points);
	free(tmp);
	return;
    } else if(!strncmp("page", url, 4)) {
	int t, nodigit=0;
	for(t=4;url[t];t++)
	    if(url[t]<'0' || url[t]>'9')
		nodigit = 1;
	if(!nodigit) {
	    int page = atoi(&url[4]);
	    if(page<0) page = 0;
	    swfoutput_linktopage(dev, page, points);
	}
    } else {
	swfoutput_linktourl(dev, url, points);
    }
}
void swfoutput_linktourl(gfxdevice_t*dev, const char*url, gfxline_t*points)
{
    ActionTAG* actions = 0;
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->shapeid>=0)
	endshape(dev);
    if(i->textmode)
	endtext(dev);

    /* TODO: escape special characters in url */
    
    if(i->config_externallinkfunction && i->config_flashversion<=8) {
	actions = action_PushString(actions, url); //parameter
	actions = action_PushInt(actions, 1); //number of parameters (1)
	actions = action_PushString(actions, i->config_externallinkfunction); //function name
	actions = action_CallFunction(actions);
    } else if(!i->config_linktarget) {
	if(!i->config_opennewwindow)
	  actions = action_GetUrl(actions, url, "_parent");
	else
	  actions = action_GetUrl(actions, url, "_this");
    } else {
	actions = action_GetUrl(actions, url, i->config_linktarget);
    }
    actions = action_End(actions);
   
    drawlink(dev, actions, 0, points, 0, "url", url);
    
    swf_ActionFree(actions);
}
void swfoutput_linktopage(gfxdevice_t*dev, int page, gfxline_t*points)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    ActionTAG* actions = 0;

    if(i->shapeid>=0)
	endshape(dev);
    if(i->textmode)
	endtext(dev);
  
    if(!i->config_internallinkfunction || i->config_flashversion>=9) {
	actions = action_GotoFrame(actions, page-1);
	actions = action_End(actions);
    } else {
	actions = action_PushInt(actions, page); //parameter
	actions = action_PushInt(actions, 1); //number of parameters (1)
	actions = action_PushString(actions, i->config_internallinkfunction); //function name
	actions = action_CallFunction(actions);
	actions = action_End(actions);
    }

    char name[80];
    sprintf(name, "page%d", page);

    drawlink(dev, actions, 0, points, 0, "page", name);
    
    swf_ActionFree(actions);
}

/* Named Links (a.k.a. Acrobatmenu) are used to implement various gadgets
   of the viewer objects, like subtitles, index elements etc.
*/
void swfoutput_namedlink(gfxdevice_t*dev, char*name, gfxline_t*points)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    ActionTAG *actions1,*actions2;
    char*tmp = strdup(name);
    char mouseover = 1;

    if(i->shapeid>=0)
	endshape(dev);
    if(i->textmode)
	endtext(dev);

    char*type = 0;
    if(!strncmp(tmp, "call:", 5))
    {
	char*x = strchr(&tmp[5], ':');
	if(!x) {
	    actions1 = action_PushInt(0, 0); //number of parameters (0)
	    actions1 = action_PushString(actions1, &tmp[5]); //function name
	    actions1 = action_CallFunction(actions1);
	    actions1 = action_End(actions1);
	} else {
	    *x = 0;
	    actions1 = action_PushString(0, x+1); //parameter
	    actions1 = action_PushInt(actions1, 1); //number of parameters (1)
	    actions1 = action_PushString(actions1, &tmp[5]); //function name
	    actions1 = action_CallFunction(actions1);
	    actions1 = action_End(actions1);
	}
	actions2 = action_End(0);
	mouseover = 0;
        type = "call";
    }
    else
    {
	actions1 = action_PushString(0, "/:subtitle");
	actions1 = action_PushString(actions1, name);
	actions1 = action_SetVariable(actions1);
	actions1 = action_End(actions1);

	actions2 = action_PushString(0, "/:subtitle");
	actions2 = action_PushString(actions2, "");
	actions2 = action_SetVariable(actions2);
	actions2 = action_End(actions2);
        type = "subtitle";
    }

    drawlink(dev, actions1, actions2, points, mouseover, type, name);

    swf_ActionFree(actions1);
    swf_ActionFree(actions2);
    free(tmp);
}

static void drawgfxline(gfxdevice_t*dev, gfxline_t*line, int fill)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    gfxcoord_t lastx=0,lasty=0,px=0,py=0;
    char lastwasmoveto;
    int lines= 0, splines=0;

    i->fill = fill;

    while(1) {
	if(!line)
	    break;
	/* check whether the next segment is zero */
	if(line->type == gfx_moveTo) {
	    moveto(dev, i->tag, line->x, line->y);
	    px = lastx = line->x;
	    py = lasty = line->y;
	    lastwasmoveto = 1;
	} if(line->type == gfx_lineTo) {
	    lineto(dev, i->tag, line->x, line->y);
	    px = line->x;
	    py = line->y;
	    lastwasmoveto = 0;
	    lines++;
	} else if(line->type == gfx_splineTo) {
	    plotxy_t s,p;
	    s.x = line->sx;p.x = line->x;
	    s.y = line->sy;p.y = line->y;
	    splineto(dev, i->tag, s, p);
	    px = line->x;
	    py = line->y;
	    lastwasmoveto = 0;
	    splines++;
	}
	line = line->next;
    }
    msg("<trace> drawgfxline, %d lines, %d splines", lines, splines);
}


static void drawlink(gfxdevice_t*dev, ActionTAG*actions1, ActionTAG*actions2, gfxline_t*points, char mouseover, char*type, const char*url)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    RGBA rgb;
    SRECT r;
    int lsid=0;
    int fsid;
    int myshapeid;
    int myshapeid2;
    double posx = 0;
    double posy = 0;
    int buttonid = getNewID(dev);
    gfxbbox_t bbox = gfxline_getbbox(points);
    
    if(i->config_linknameurl) {
        actions1 = 0;
        actions2 = 0;
    }
    
    i->hasbuttons = 1;

    /* shape */
    myshapeid = getNewID(dev);
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE3);
    swf_ShapeNew(&i->shape);
    rgb.r = rgb.b = rgb.a = rgb.g = 0; 
    fsid = swf_ShapeAddSolidFillStyle(i->shape,&rgb);
    swf_SetU16(i->tag, myshapeid);
    r.xmin = (int)(bbox.xmin*20);
    r.ymin = (int)(bbox.ymin*20);
    r.xmax = (int)(bbox.xmax*20);
    r.ymax = (int)(bbox.ymax*20);
    r = swf_ClipRect(i->pagebbox, r);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,i->shape);
    swf_ShapeCountBits(i->shape,NULL,NULL);
    swf_SetShapeBits(i->tag,i->shape);
    swf_ShapeSetAll(i->tag,i->shape,/*x*/0,/*y*/0,0,fsid,0);
    i->swflastx = i->swflasty = 0;
    drawgfxline(dev, points, 1);
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(i->shape);

    /* shape2 */
    myshapeid2 = getNewID(dev);
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE3);
    swf_ShapeNew(&i->shape);
    
    rgb = i->config_linkcolor;

    fsid = swf_ShapeAddSolidFillStyle(i->shape,&rgb);
    swf_SetU16(i->tag, myshapeid2);
    r.xmin = (int)(bbox.xmin*20);
    r.ymin = (int)(bbox.ymin*20);
    r.xmax = (int)(bbox.xmax*20);
    r.ymax = (int)(bbox.ymax*20);
    r = swf_ClipRect(i->pagebbox, r);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,i->shape);
    swf_ShapeCountBits(i->shape,NULL,NULL);
    swf_SetShapeBits(i->tag,i->shape);
    swf_ShapeSetAll(i->tag,i->shape,/*x*/0,/*y*/0,0,fsid,0);
    i->swflastx = i->swflasty = 0;
    drawgfxline(dev, points, 1);
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(i->shape);

    if(!mouseover)
    {
	i->tag = swf_InsertTag(i->tag,ST_DEFINEBUTTON);
	swf_SetU16(i->tag,buttonid); //id
	swf_ButtonSetFlags(i->tag, 0); //menu=no
	swf_ButtonSetRecord(i->tag,0x01,myshapeid,i->depth,0,0);
	swf_ButtonSetRecord(i->tag,0x02,myshapeid2,i->depth,0,0);
	swf_ButtonSetRecord(i->tag,0x04,myshapeid2,i->depth,0,0);
	swf_ButtonSetRecord(i->tag,0x08,myshapeid,i->depth,0,0);
	swf_SetU8(i->tag,0);
	swf_ActionSet(i->tag,actions1);
	swf_SetU8(i->tag,0);
    }
    else
    {
	i->tag = swf_InsertTag(i->tag,ST_DEFINEBUTTON2);
	swf_SetU16(i->tag,buttonid); //id
	swf_ButtonSetFlags(i->tag, 0); //menu=no
	swf_ButtonSetRecord(i->tag,0x01,myshapeid,i->depth,0,0);
	swf_ButtonSetRecord(i->tag,0x02,myshapeid2,i->depth,0,0);
	swf_ButtonSetRecord(i->tag,0x04,myshapeid2,i->depth,0,0);
	swf_ButtonSetRecord(i->tag,0x08,myshapeid,i->depth,0,0);
	swf_SetU8(i->tag,0); // end of button records
        swf_ButtonSetCondition(i->tag, BC_IDLE_OVERUP);
	swf_ActionSet(i->tag,actions1);
	if(actions2) {
	    swf_ButtonSetCondition(i->tag, BC_OVERUP_IDLE);
	    swf_ActionSet(i->tag,actions2);
	    swf_SetU8(i->tag,0);
	    swf_ButtonPostProcess(i->tag, 2);
	} else {
	    swf_SetU8(i->tag,0);
            swf_ButtonPostProcess(i->tag, 1);
	}
    }

    char buf[80];
    char*buf2 = 0;
    const char* name = 0;
    if(i->config_linknameurl) {
        buf2 = malloc(strlen(type)+strlen(url)+2);
        sprintf(buf2, "%s:%s", type, url);
        name = buf2;
    } else {
        name = buf;
        sprintf(buf, "button%d", buttonid);
    }
    
    msg("<trace> Placing link ID %d", buttonid);
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);

    if(posx!=0 || posy!=0) {
        SPOINT p;
        p.x = (int)(posx*20);
        p.y = (int)(posy*20);
        p = swf_TurnPoint(p, &i->page_matrix);
	MATRIX m;
        m = i->page_matrix;
        m.tx = p.x;
        m.ty = p.y;
	swf_ObjectPlace(i->tag, buttonid, getNewDepth(dev),&m,0,(U8*)name);
    } else {
	swf_ObjectPlace(i->tag, buttonid, getNewDepth(dev),&i->page_matrix,0,(U8*)name);
    }

    if(buf2)
	free(buf2);
}

      
///////////
/*
for(t=0;t<picpos;t++)
      {
	  if(pic_xids[t] == xid &&
	     pic_yids[t] == yid) {
	      width = pic_width[t];
	      height = pic_height[t];
	      found = t;break;
	  }
      }
	  pic_ids[picpos] = swfoutput_drawimagelosslessN(&output, pic, pal, width, height, x1,y1,x2,y2,x3,y3,x4,y4, numpalette);
	  pic_xids[picpos] = xid;
	  pic_yids[picpos] = yid;
	  pic_width[picpos] = width;
	  pic_height[picpos] = height;
	  if(picpos<1024)
	      picpos++;
	    pic[width*y+x] = buf[0];
	    xid+=x*buf[0]+1;
	    yid+=y*buf[0]*3+1;
      
	    xid += pal[1].r*3 + pal[1].g*11 + pal[1].b*17;
      yid += pal[1].r*7 + pal[1].g*5 + pal[1].b*23;
      
      int xid = 0;
      int yid = 0;
	  xid += x*r+x*b*3+x*g*7+x*a*11;
	  yid += y*r*3+y*b*17+y*g*19+y*a*11;
      int t,found = -1;
      for(t=0;t<picpos;t++)
      {
	  if(pic_xids[t] == xid &&
	     pic_yids[t] == yid) {
	      found = t;break;
	  }
      }
      if(found<0) {
*/
///////////


int swf_setparameter(gfxdevice_t*dev, const char*name, const char*value)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    msg("<trace> swfdevice: %s=%s", name, value);
    if(!strcmp(name, "jpegsubpixels")) {
	i->config_jpegsubpixels = atof(value);
    } else if(!strcmp(name, "ppmsubpixels")) {
	i->config_ppmsubpixels = atof(value);
    } else if(!strcmp(name, "subpixels")) {
	i->config_ppmsubpixels = i->config_jpegsubpixels = atof(value);
    } else if(!strcmp(name, "drawonlyshapes")) {
	i->config_drawonlyshapes = atoi(value);
    } else if(!strcmp(name, "ignoredraworder")) {
	i->config_ignoredraworder = atoi(value);
    } else if(!strcmp(name, "mark")) {
	if(!value || !value[0]) {
	    if(i->mark) free(i->mark);
	    i->mark = 0;
	} else {
	    int t;
	    i->mark = strdup("...");
	    for(t=0;t<3;t++) if(value[t]) i->mark[t] = value[t];
	}
    } else if(!strcmp(name, "filloverlap")) {
	i->config_filloverlap = atoi(value);
    } else if(!strcmp(name, "local_with_network")) {
	i->config_local_with_network = atoi(value);
	i->config_local_with_filesystem = !i->config_local_with_network;
    } else if(!strcmp(name, "local_with_filesystem")) {
	i->config_local_with_filesystem = atoi(value);
	i->config_local_with_network = !i->config_local_with_filesystem;
    } else if(!strcmp(name, "linksopennewwindow")) {
	i->config_opennewwindow = atoi(value);
    } else if(!strcmp(name, "opennewwindow")) {
	i->config_opennewwindow = atoi(value);
    } else if(!strcmp(name, "storeallcharacters")) {
	i->config_storeallcharacters = atoi(value);
    } else if(!strcmp(name, "enablezlib")) {
	i->config_enablezlib = atoi(value);
    } else if(!strcmp(name, "bboxvars")) {
	i->config_bboxvars = atoi(value);
    } else if(!strcmp(name, "dots")) {
	i->config_dots = atoi(value);
    } else if(!strcmp(name, "frameresets")) {
	i->config_frameresets = atoi(value);
    } else if(!strcmp(name, "showclipshapes")) {
	i->config_showclipshapes = atoi(value);
    } else if(!strcmp(name, "reordertags")) {
	i->config_reordertags = atoi(value);
    } else if(!strcmp(name, "internallinkfunction")) {
	i->config_internallinkfunction = strdup(value);
    } else if(!strcmp(name, "externallinkfunction")) {
	i->config_externallinkfunction = strdup(value);
    } else if(!strcmp(name, "linkfunction")) { //sets both internallinkfunction and externallinkfunction
	i->config_internallinkfunction = strdup(value);
	i->config_externallinkfunction = strdup(value);
    } else if(!strcmp(name, "disable_polygon_conversion")) {
	i->config_disable_polygon_conversion = atoi(value);
    } else if(!strcmp(name, "normalize_polygon_positions")) {
	i->config_normalize_polygon_positions = atoi(value);
    } else if(!strcmp(name, "wxwindowparams")) {
	i->config_watermark = atoi(value);
    } else if(!strcmp(name, "insertstop")) {
	i->config_insertstoptag = atoi(value);
    } else if(!strcmp(name, "protect")) {
	i->config_protect = atoi(value);
	if(i->config_protect && i->tag) {
	    i->tag = swf_InsertTag(i->tag, ST_PROTECT);
	}
    } else if(!strcmp(name, "flashversion")) {
	i->config_flashversion = atoi(value);
	if(i->swf) {
	    i->swf->fileVersion = i->config_flashversion;
	}
    } else if(!strcmp(name, "framerate")) {
	i->config_framerate = atof(value);
	if(i->swf) {
	    i->swf->frameRate = i->config_framerate*0x100;
	}
    } else if(!strcmp(name, "minlinewidth")) {
	i->config_minlinewidth = atof(value);
    } else if(!strcmp(name, "remove_small_polygons")) {
	i->config_remove_small_polygons = atof(value);
    } else if(!strcmp(name, "caplinewidth")) {
	i->config_caplinewidth = atof(value);
    } else if(!strcmp(name, "linktarget")) {
	i->config_linktarget = strdup(value);
    } else if(!strcmp(name, "invisibletexttofront")) {
	i->config_invisibletexttofront = atoi(value);
    } else if(!strcmp(name, "noclips")) {
	i->config_noclips = atoi(value);
    } else if(!strcmp(name, "dumpfonts")) {
	i->config_dumpfonts = atoi(value);
    } else if(!strcmp(name, "override_line_widths")) {
	i->config_override_line_widths = atof(value);
    } else if(!strcmp(name, "animate")) {
	i->config_animate = atoi(value);
	i->config_framerate = 25;
    } else if(!strcmp(name, "linknameurl")) {
	i->config_linknameurl = atoi(value);
    } else if(!strcmp(name, "showimages")) {
	i->config_showimages = atoi(value);
    } else if(!strcmp(name, "disablelinks")) {
	i->config_disablelinks = atoi(value);
    } else if(!strcmp(name, "simpleviewer")) {
	i->config_simpleviewer = atoi(value);
    } else if(!strcmp(name, "next_bitmap_is_jpeg")) {
	i->jpeg = 1;
    } else if(!strcmp(name, "jpegquality")) {
	int val = atoi(value);
	if(val<0) val=0;
	if(val>101) val=101;
	i->config_jpegquality = val;
    } else if(!strcmp(name, "splinequality")) {
	int v = atoi(value);
	v = 500-(v*5); // 100% = 0.25 pixel, 0% = 25 pixel
	if(v<1) v = 1;
	i->config_splinemaxerror = v;
    } else if(!strcmp(name, "fontquality")) {
	int v = atoi(value);
	v = 500-(v*5); // 100% = 0.25 pixel, 0% = 25 pixel
	if(v<1) v = 1;
	i->config_fontsplinemaxerror = v;
    } else if(!strcmp(name, "linkcolor")) {
	if(strlen(value)!=8) {
	    fprintf(stderr, "Unknown format for option 'linkcolor'. (%s <-> RRGGBBAA)\n", value);
	    return 1;
	}
#	define NIBBLE(s) (((s)>='0' && (s)<='9')?((s)-'0'):((s)&0x0f)+9)
	i->config_linkcolor.r = NIBBLE(value[0])<<4 | NIBBLE(value[1]);
	i->config_linkcolor.g = NIBBLE(value[2])<<4 | NIBBLE(value[3]);
	i->config_linkcolor.b = NIBBLE(value[4])<<4 | NIBBLE(value[5]);
	i->config_linkcolor.a = NIBBLE(value[6])<<4 | NIBBLE(value[7]);
    } else if(!strcmp(name, "help")) {
	printf("\nSWF layer options:\n");
        printf("jpegsubpixels=<pixels>      resolution adjustment for jpeg images (same as jpegdpi, but in pixels)\n");
        printf("ppmsubpixels=<pixels        resolution adjustment for  lossless images (same as ppmdpi, but in pixels)\n");
        printf("subpixels=<pixels>          shortcut for setting both jpegsubpixels and ppmsubpixels\n");
        printf("drawonlyshapes              convert everything to shapes (currently broken)\n");
        printf("ignoredraworder             allow to perform a few optimizations for creating smaller SWFs\n");
        printf("linksopennewwindow          make links open a new browser window\n");
        printf("linktarget                  target window name of new links\n");
        printf("linkcolor=<color)           color of links (format: RRGGBBAA)\n");
        printf("linknameurl		    Link buttons will be named like the URL they refer to (handy for iterating through links with actionscript)\n");
        printf("storeallcharacters          don't reduce the fonts to used characters in the output file\n");
        printf("enablezlib                  switch on zlib compression (also done if flashversion>=6)\n");
        printf("bboxvars                    store the bounding box of the SWF file in actionscript variables\n");
        printf("dots                        Take care to handle dots correctly\n");
        printf("reordertags=0/1             (default: 1) perform some tag optimizations\n");
        printf("internallinkfunction=<name> when the user clicks a internal link (to a different page) in the converted file, this actionscript function is called\n");
        printf("externallinkfunction=<name> when the user clicks an external link (e.g. http://www.foo.bar/) on the converted file, this actionscript function is called\n");
        printf("disable_polygon_conversion  never convert strokes to polygons (will remove capstyles and joint styles)\n");
        printf("caplinewidth=<width>        the minimum thichness a line needs to have so that capstyles become visible (and are converted)\n");
        printf("insertstop                  put an ActionScript \"STOP\" tag in every frame\n");
        printf("protect                     add a \"protect\" tag to the file, to prevent loading in the Flash editor\n");
        printf("flashversion=<version>      the SWF fileversion (6)\n");
        printf("framerate=<fps>		    SWF framerate\n");
        printf("minlinewidth=<width>        convert horizontal/vertical boxes smaller than this width to lines (0.05) \n");
        printf("simpleviewer                Add next/previous buttons to the SWF\n");
        printf("animate                     insert a showframe tag after each placeobject (animate draw order of PDF files)\n");
        printf("jpegquality=<quality>       set compression quality of jpeg images\n");
	printf("splinequality=<value>       Set the quality of spline convertion to value (0-100, default: 100).\n");
	printf("disablelinks                Disable links.\n");
    } else {
	return 0;
    }
    return 1;
}

// --------------------------------------------------------------------

static CXFORM gfxcxform_to_cxform(gfxcxform_t* c)
{
    CXFORM cx;
    swf_GetCXForm(0, &cx, 1);
    if(!c)
	return cx;
    if(c->rg!=0 || c->rb!=0 || c->ra!=0 ||
       c->gr!=0 || c->gb!=0 || c->ga!=0 ||
       c->br!=0 || c->bg!=0 || c->ba!=0 ||
       c->ar!=0 || c->ag!=0 || c->ab!=0)
	msg("<warning> CXForm not SWF-compatible");

    cx.a0 = (S16)(c->aa*256);
    cx.r0 = (S16)(c->rr*256);
    cx.g0 = (S16)(c->gg*256);
    cx.b0 = (S16)(c->bb*256);
    cx.a1 = c->ta;
    cx.r1 = c->tr;
    cx.g1 = c->tg;
    cx.b1 = c->tb;
    return cx;
}

/* TODO */
static int imageInCache(gfxdevice_t*dev, void*data, int width, int height)
{
    return -1;
}
static void addImageToCache(gfxdevice_t*dev, void*data, int width, int height)
{
}
    
static int add_image(swfoutput_internal*i, gfximage_t*img, int targetwidth, int targetheight, int* newwidth, int* newheight)
{
    gfxdevice_t*dev = i->dev;
    RGBA*newpic = 0;
    RGBA*mem = (RGBA*)img->data;
    
    int sizex = img->width;
    int sizey = img->height;
    int is_jpeg = i->jpeg;
    i->jpeg = 0;

    int newsizex=sizex, newsizey=sizey;

    /// {
    if(is_jpeg && i->config_jpegsubpixels) {
	newsizex = (int)(targetwidth*i->config_jpegsubpixels + 0.5);
	newsizey = (int)(targetheight*i->config_jpegsubpixels + 0.5);
    } else if(!is_jpeg && i->config_ppmsubpixels) {
	newsizex = (int)(targetwidth*i->config_ppmsubpixels + 0.5);
	newsizey = (int)(targetheight*i->config_ppmsubpixels + 0.5);
    }
    /// }

    if(sizex<=0 || sizey<=0)
	return -1;
    if(newsizex<=0)
	newsizex = 1;
    if(newsizey<=0)
	newsizey = 1;

    /* TODO: cache images */
    
    if(newsizex<sizex || newsizey<sizey) {
	msg("<verbose> Scaling %dx%d image to %dx%d", sizex, sizey, newsizex, newsizey);
	gfximage_t*ni = gfximage_rescale(img, newsizex, newsizey);
	newpic = (RGBA*)ni->data;
	free(ni);
	*newwidth = sizex = newsizex;
	*newheight  = sizey = newsizey;
	mem = newpic;
    } else {
	*newwidth = newsizex = sizex;
	*newheight = newsizey  = sizey;
    }

    int num_colors = swf_ImageGetNumberOfPaletteEntries(mem,sizex,sizey,0);
    int has_alpha = swf_ImageHasAlpha(mem,sizex,sizey);
    
    msg("<verbose> Drawing %dx%d %s%simage (id %d) at size %dx%d (%dx%d), %s%d colors",
	    sizex, sizey, 
	    has_alpha?(has_alpha==2?"semi-transparent ":"transparent "):"", 
	    is_jpeg?"jpeg-":"", i->currentswfid+1,
	    newsizex, newsizey,
	    targetwidth, targetheight,
	    /*newsizex, newsizey,*/
	    num_colors>256?">":"", num_colors>256?256:num_colors);

    /*RGBA* pal = (RGBA*)rfx_alloc(sizeof(RGBA)*num_colors);
    swf_ImageGetNumberOfPaletteEntries(mem,sizex,sizey,pal);
    int t;
    for(t=0;t<num_colors;t++) {
	printf("%02x%02x%02x%02x ",
		pal[t].r, pal[t].g, pal[t].b, pal[t].a);
	if((t&7)==7)
	    printf("\n");
    }
    printf("\n");*/

    int bitid = -1;
    int cacheid = imageInCache(dev, mem, sizex, sizey);

    if(cacheid<=0) {
	bitid = getNewID(dev);

	i->tag = swf_AddImage(i->tag, bitid, mem, sizex, sizey, i->config_jpegquality);
	addImageToCache(dev, mem, sizex, sizey);
    } else {
	bitid = cacheid;
    }

    if(newpic)
	free(newpic);
    return bitid;
}

int line_is_empty(gfxline_t*line)
{
    while(line) {
	if(line->type != gfx_moveTo)
	    return 0;
	line = line->next;
    }
    return 1;
}

static SRECT gfxline_getSWFbbox(gfxline_t*line)
{
    gfxbbox_t bbox = gfxline_getbbox(line);
    SRECT r;
    r.xmin = (int)(bbox.xmin*20);
    r.ymin = (int)(bbox.ymin*20);
    r.xmax = (int)(bbox.xmax*20);
    r.ymax = (int)(bbox.ymax*20);
    return r;
}

static void swf_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    
    if(line_is_empty(line))
	return;

    endshape(dev);
    endtext(dev);

    int targetx = (int)(sqrt(matrix->m00*matrix->m00 + matrix->m01*matrix->m01)*img->width);
    int targety = (int)(sqrt(matrix->m10*matrix->m10 + matrix->m11*matrix->m11)*img->height);

    int newwidth=0,newheight=0;
    int bitid = add_image(i, img, targetx, targety, &newwidth, &newheight);
    if(bitid<0)
	return;
    double fx = (double)img->width / (double)newwidth;
    double fy = (double)img->height / (double)newheight;

    MATRIX m;
    m.sx = (int)(65536*20*matrix->m00*fx); m.r1 = (int)(65536*20*matrix->m10*fy);
    m.r0 = (int)(65536*20*matrix->m01*fx); m.sy = (int)(65536*20*matrix->m11*fy);
    m.tx = (int)(matrix->tx*20);
    m.ty = (int)(matrix->ty*20);
  
    /* shape */
    int myshapeid = getNewID(dev);
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE);
    SHAPE*shape;
    swf_ShapeNew(&shape);
    int fsid = swf_ShapeAddBitmapFillStyle(shape,&m,bitid,1);
    int lsid = 0;
    if(i->config_showimages) {
	RGBA pink = {255,255,0,255};
	lsid = swf_ShapeAddLineStyle(shape, 20, &pink);
    }
    swf_SetU16(i->tag, myshapeid);
    SRECT r = gfxline_getSWFbbox(line);
    r = swf_ClipRect(i->pagebbox, r);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(i->tag,shape);
    swf_ShapeSetAll(i->tag,shape,UNDEFINED_COORD,UNDEFINED_COORD,lsid,fsid,0);
    i->swflastx = i->swflasty = UNDEFINED_COORD;
    drawgfxline(dev, line, 1);
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(shape);

    msg("<trace> Placing image, shape ID %d, bitmap ID %d", myshapeid, bitid);
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    CXFORM cxform2 = gfxcxform_to_cxform(cxform);
    swf_ObjectPlace(i->tag,myshapeid,getNewDepth(dev),&i->page_matrix,&cxform2,NULL);
}

static RGBA col_purple = {255,255,0,255};
static void drawoutline(gfxdevice_t*dev, gfxline_t*line)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    int myshapeid = getNewID(dev);
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE3);

    SHAPE*shape;
    swf_ShapeNew(&shape);
    int lsid = swf_ShapeAddLineStyle(shape,60,&col_purple);

    swf_SetU16(i->tag,myshapeid);
    SRECT r = gfxline_getSWFbbox(line);
    r = swf_ClipRect(i->pagebbox, r);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(i->tag,shape);
    swf_ShapeSetAll(i->tag,shape,UNDEFINED_COORD,UNDEFINED_COORD,lsid,0,0);
    drawgfxline(dev, line, 1);
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(shape);
	
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    swf_ObjectPlace(i->tag, myshapeid, getNewDepth(dev), 0,0,0);
}

static void swf_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->config_noclips)
	return;

    endtext(dev);
    endshape(dev);

    if(i->clippos >= 127)
    {
        msg("<warning> Too many clip levels.");
        i->clippos --;
    } 

    if(i->config_showclipshapes) {
        i->stored_clipshapes = gfxline_append(i->stored_clipshapes, gfxline_clone(line));
    }

    int myshapeid = getNewID(dev);
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE3);
    RGBA col;
    memset(&col, 0, sizeof(RGBA));
    col.a = 255;
    SHAPE*shape;
    swf_ShapeNew(&shape);
    int fsid = swf_ShapeAddSolidFillStyle(shape,&col);
    if(i->mark) {
	RGBA markcol = {0,i->mark[0],i->mark[1],i->mark[2]};
	swf_ShapeAddSolidFillStyle(shape,&markcol);
    }
    swf_SetU16(i->tag,myshapeid);
    SRECT r = gfxline_getSWFbbox(line);
    r = swf_ClipRect(i->pagebbox, r);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(i->tag,shape);
    swf_ShapeSetAll(i->tag,shape,UNDEFINED_COORD,UNDEFINED_COORD,0,fsid,0);
    i->swflastx = i->swflasty = UNDEFINED_COORD;
    i->shapeisempty = 1;
    drawgfxline(dev, line, 1);
    if(i->shapeisempty) {
	/* an empty clip shape is equivalent to a shape with no area */
	int x = line?line->x:0;
	int y = line?line->y:0;
	moveto(dev, i->tag, x,y);
	lineto(dev, i->tag, x,y);
	lineto(dev, i->tag, x,y);
    }
    if(!i->shapeisempty && i->currentswfid==1 && r.xmin==0 && r.ymin==0 && r.xmax==(int)(i->max_x*20) && r.ymax==(int)(i->max_y*20)) {
	if(i->config_watermark) {
	    gfxbbox_t r; r.xmin = r.ymin = 0;r.xmax = i->max_x;r.ymax = i->max_y;
	    draw_watermark(dev, r, 1);
	}
    }
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(shape);

    /* TODO: remember the bbox, and check all shapes against it */
    
    msg("<trace> Placing clip ID %d", myshapeid);
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    i->cliptags[i->clippos] = i->tag;
    i->clipshapes[i->clippos] = myshapeid;
    i->clipdepths[i->clippos] = getNewDepth(dev);
    i->clippos++;
}

static void swf_endclip(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->config_noclips)
	return;
    if(i->textmode)
	endtext(dev);
    if(i->shapeid>=0)
	endshape(dev);

    if(!i->clippos) {
        msg("<error> Invalid end of clipping region");
        return;
    }
    i->clippos--;
    /*swf_ObjectPlaceClip(i->cliptags[i->clippos],i->clipshapes[i->clippos],i->clipdepths[i->clippos],&i->page_matrix,NULL,NULL,
	    / * clip to depth: * / i->depth <= i->clipdepths[i->clippos]? i->depth : i->depth - 1);
    i->depth ++;*/
    swf_ObjectPlaceClip(i->cliptags[i->clippos],i->clipshapes[i->clippos],i->clipdepths[i->clippos],&i->page_matrix,NULL,NULL,i->depth);
}
static int gfxline_type(gfxline_t*line)
{
    int tmplines=0;
    int tmpsplines=0;
    int lines=0;
    int splines=0;
    int haszerosegments=0;
    int length=0;
    while(line) {
	if(line->type == gfx_moveTo) {
	    tmplines=0;
	    tmpsplines=0;
	} else if(line->type == gfx_lineTo) {
	    tmplines++;
	    if(tmplines>lines)
		lines=tmplines;
	} else if(line->type == gfx_splineTo) {
	    tmpsplines++;
	    if(tmpsplines>lines)
		splines=tmpsplines;
	}
	length++;
	line = line->next;
    }
    if(length>400)
	return 5;
    if(lines==0 && splines==0) return 0;
    else if(lines==1 && splines==0) return 1;
    else if(lines==0 && splines==1) return 2;
    else if(splines==0) return 3;
    else return 4;
}

static int gfxline_has_dots(gfxline_t*line)
{
    int tmplines=0;
    double x=0,y=0;
    double dist = 0;
    int isline = 0;
    int short_gap = 0;
    while(line) {
	if(line->type == gfx_moveTo) {
	    /* test the length of the preceding line, and assume it is a dot if
	       it's length is less than 1.0. But *only* if there's a noticable 
	       gap between the previous line and the next moveTo. (I've come
	       across a PDF where thousands of "dots" were stringed together,
	       forming a line) */
	    int last_short_gap = short_gap;
	    if((fabs(line->x - x) + fabs(line->y - y)) < 1.0) {
		short_gap = 1;
	    } else {
		short_gap = 0;
	    }
	    if(isline && dist < 1 && !short_gap && !last_short_gap) {
		return 1;
	    }
	    dist = 0;
	    isline = 0;
	} else if(line->type == gfx_lineTo) {
	    dist += fabs(line->x - x) + fabs(line->y - y);
	    isline = 1;
	} else if(line->type == gfx_splineTo) {
	    dist += fabs(line->sx - x) + fabs(line->sy - y) + 
		    fabs(line->x - line->sx) + fabs(line->y - line->sy);
	    isline = 1;
	}
	x = line->x;
	y = line->y;
	line = line->next;
    }
    if(isline && dist < 1 && !short_gap) {
	return 1;
    }
    return 0;
}

static int gfxline_fix_short_edges(gfxline_t*line)
{
    double x,y;
    while(line) {
	if(line->type == gfx_lineTo) {
	    if(fabs(line->x - x) + fabs(line->y - y) < 0.01) {
		line->x += 0.01;
	    }
	} else if(line->type == gfx_splineTo) {
	    if(fabs(line->sx - x) + fabs(line->sy - y) + 
	       fabs(line->x - line->sx) + fabs(line->y - line->sy) < 0.01) {
		line->x += 0.01;
	    }
	}
	x = line->x;
	y = line->y;
	line = line->next;
    }
    return 0;
}

static char is_inside_page(gfxdevice_t*dev, gfxcoord_t x, gfxcoord_t y)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(x<i->min_x || x>i->max_x) return 0;
    if(y<i->min_y || y>i->max_y) return 0;
    return 1;
}

gfxline_t* gfxline_move(gfxline_t*line, double x, double y)
{
    gfxline_t*l = line = gfxline_clone(line);

    while(l) {
	l->x += x;
	l->y += y;
	l->sx += x;
	l->sy += y;
	l = l->next;
    }
    return line;
}

//#define NORMALIZE_POLYGON_POSITIONS

static void swf_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(line_is_empty(line))
	return;
    int type = gfxline_type(line);
    int has_dots = gfxline_has_dots(line);
    gfxbbox_t r = gfxline_getbbox(line);
    int is_outside_page = !is_inside_page(dev, r.xmin, r.ymin) || !is_inside_page(dev, r.xmax, r.ymax);

    if(i->config_override_line_widths) {
	width = i->config_override_line_widths;
    }

    /* TODO: * split line into segments, and perform this check for all segments */

    if(i->config_disable_polygon_conversion || /*type>=5 ||*/
       (!has_dots &&
        (width <= i->config_caplinewidth 
        || (cap_style == gfx_capRound && joint_style == gfx_joinRound)
        || (cap_style == gfx_capRound && type<=2)))) 
    {
        // ...
    } else {
	/* convert line to polygon */
	msg("<trace> draw as polygon, type=%d dots=%d", type, has_dots);
	if(has_dots)
	    gfxline_fix_short_edges(line);
	/* we need to convert the line into a polygon */
	gfxpoly_t* poly = gfxpoly_from_stroke(line, width, cap_style, joint_style, miterLimit, DEFAULT_GRID);
	gfxline_t*gfxline = gfxline_from_gfxpoly(poly);
	dev->fill(dev, gfxline, color);
	gfxline_free(gfxline);
	gfxpoly_destroy(poly);
	return;
    }

    msg("<trace> draw as stroke, type=%d dots=%d", type, has_dots);
    endtext(dev);

    if(i->config_normalize_polygon_positions) {
	endshape(dev);
	double startx = 0, starty = 0;
	if(line && line->type == gfx_moveTo) {
	    startx = line->x;
	    starty = line->y;
	}
	line = gfxline_move(line, -startx, -starty);
	i->shapeposx = (int)(startx*20);
	i->shapeposy = (int)(starty*20);
    }

    swfoutput_setstrokecolor(dev, color->r, color->g, color->b, color->a);
    swfoutput_setlinewidth(dev, width);
    startshape(dev);
    stopFill(dev);
    drawgfxline(dev, line, 0);

    if(i->config_normalize_polygon_positions) {
	free(line); //account for _move
    }

}

static void swf_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(line_is_empty(line))
	return;
    if(!color->a)
	return;

    gfxbbox_t r = gfxline_getbbox(line);
    int is_outside_page = !is_inside_page(dev, r.xmin, r.ymin) || !is_inside_page(dev, r.xmax, r.ymax);

    if(r.xmax - r.xmin < i->config_remove_small_polygons &&
       r.ymax - r.ymin < i->config_remove_small_polygons) {
	msg("<verbose> Not drawing %.2fx%.2f polygon", r.xmax - r.xmin, r.ymax - r.ymin);
	return;
    }

    endtext(dev);

    if(!i->config_ignoredraworder)
	endshape(dev);

    if(i->config_normalize_polygon_positions) {
	endshape(dev);
	double startx = 0, starty = 0;
	if(line && line->type == gfx_moveTo) {
	    startx = line->x;
	    starty = line->y;
	}
	line = gfxline_move(line, -startx, -starty);
	i->shapeposx = (int)(startx*20);
	i->shapeposy = (int)(starty*20);
    }

    swfoutput_setfillcolor(dev, color->r, color->g, color->b, color->a);
    startshape(dev);
    startFill(dev);
    drawgfxline(dev, line, 1);
    
    if(i->currentswfid==2 && r.xmin==0 && r.ymin==0 && r.xmax==i->max_x && r.ymax==i->max_y) {
	if(i->config_watermark) {
	    draw_watermark(dev, r, 1);
	}
    }

    msg("<trace> end of swf_fill (shapeid=%d)", i->shapeid);

    if(i->config_normalize_polygon_positions) {
	free(line); //account for _move
    }
}

static GRADIENT* gfxgradient_to_GRADIENT(gfxgradient_t*gradient)
{
    int num = 0;
    gfxgradient_t*g = gradient;
    while(g) {
	num++;
	g = g->next;
    }
    GRADIENT* swfgradient = malloc(sizeof(GRADIENT));
    swfgradient->num = num;
    swfgradient->rgba = malloc(sizeof(swfgradient->rgba[0])*num);
    swfgradient->ratios = malloc(sizeof(swfgradient->ratios[0])*num);

    g = gradient;
    num = 0;
    while(g) {
	swfgradient->ratios[num] = g->pos*255;
	swfgradient->rgba[num] = *(RGBA*)&g->color;
	num++;
	g = g->next;
    }
    return swfgradient;
}

static void swf_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    if(line_is_empty(line))
	return;
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    
    if(line_is_empty(line))
	return;

    GRADIENT* swfgradient = gfxgradient_to_GRADIENT(gradient);
    if(!swfgradient)
	return;
  
    endshape(dev);
    endtext(dev);

    double f = type==gfxgradient_radial?4:4;
    MATRIX m;
    m.sx = (int)(matrix->m00*20*f); m.r1 = (int)(matrix->m10*20*f);
    m.r0 = (int)(matrix->m01*20*f); m.sy = (int)(matrix->m11*20*f);
    m.tx = (int)(matrix->tx*20);
    m.ty = (int)(matrix->ty*20);

    /* shape */
    int myshapeid = getNewID(dev);
    i->tag = swf_InsertTag(i->tag, ST_DEFINESHAPE2);
    SHAPE*shape;
    swf_ShapeNew(&shape);
    int fsid = swf_ShapeAddGradientFillStyle(shape,&m,swfgradient,type==gfxgradient_radial);
    swf_SetU16(i->tag, myshapeid);
    SRECT r = gfxline_getSWFbbox(line);
    r = swf_ClipRect(i->pagebbox, r);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(i->tag,shape);
    swf_ShapeSetAll(i->tag,shape,UNDEFINED_COORD,UNDEFINED_COORD,0,fsid,0);
    i->swflastx = i->swflasty = UNDEFINED_COORD;
    drawgfxline(dev, line, 1);
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(shape);

    int depth = getNewDepth(dev);
    msg("<trace> Placing gradient, shape ID %d, depth %d", myshapeid, depth);
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    swf_ObjectPlace(i->tag,myshapeid,depth,&i->page_matrix,NULL,NULL);

    swf_FreeGradient(swfgradient);free(swfgradient);
}

static SWFFONT* gfxfont_to_swffont(gfxfont_t*font, const char* id, int version)
{
    SWFFONT*swffont = (SWFFONT*)rfx_calloc(sizeof(SWFFONT));
    int t;
    SRECT bounds = {0,0,0,0};
    swffont->id = -1;
    swffont->version = version;
    swffont->name = (U8*)strdup(id);
    swffont->layout = (SWFLAYOUT*)rfx_calloc(sizeof(SWFLAYOUT));
    swffont->layout->ascent = 0;
    swffont->layout->descent = 0;
    swffont->layout->leading = 0;
    swffont->layout->bounds = (SRECT*)rfx_calloc(sizeof(SRECT)*font->num_glyphs);
    swffont->encoding = FONT_ENCODING_UNICODE;
    swffont->numchars = font->num_glyphs;
    swffont->maxascii = font->max_unicode;
    swffont->ascii2glyph = (int*)rfx_calloc(sizeof(int)*swffont->maxascii);
    swffont->glyph2ascii = (U16*)rfx_calloc(sizeof(U16)*swffont->numchars);
    swffont->glyph = (SWFGLYPH*)rfx_calloc(sizeof(SWFGLYPH)*swffont->numchars);
    swffont->glyphnames = (char**)rfx_calloc(sizeof(char*)*swffont->numchars);

    SRECT max = {0,0,0,0};
    for(t=0;t<font->num_glyphs;t++) {
	drawer_t draw;
	gfxline_t*line;
	double advance = 0;
	int u = font->glyphs[t].unicode;
	int s;
	char twice=0;
	for(s=0;s<font->num_glyphs;s++) {
	    if(swffont->glyph2ascii[s]==u) 
		twice=1;
	}
	if(u >= 0xd800 || u == 0x0000 || twice) {
	    /* flash 8 flashtype requires unique unicode IDs for each character.
	       We use the Unicode private user area to assign characters, hoping that
	       the font doesn't contain more than 8192 glyphs */
	    u = 0xe000 + (t&0x1fff);
	}
	swffont->glyph2ascii[t] = u;

	if(font->glyphs[t].name) {
	    swffont->glyphnames[t] = strdup(font->glyphs[t].name);
	} else {
	    swffont->glyphnames[t] = 0;
	}
	advance = font->glyphs[t].advance;

	swf_Shape01DrawerInit(&draw, 0);
	line = font->glyphs[t].line;

	const double scale = GLYPH_SCALE;
	while(line) {
	    FPOINT c,to;
	    c.x = line->sx * scale; c.y = -line->sy * scale;
	    //to.x = floor(line->x * scale); to.y = floor(-line->y * scale);
	    to.x = line->x * scale; to.y = -line->y * scale;

	    /*if(strstr(swffont->name, "BIRNU") && t==90) {
		to.x += 1;
	    }*/

	    if(line->type == gfx_moveTo) {
		draw.moveTo(&draw, &to);
	    } else if(line->type == gfx_lineTo) {
		draw.lineTo(&draw, &to);
	    } else if(line->type == gfx_splineTo) {
		draw.splineTo(&draw, &c, &to);
	    }
	    line = line->next;
	}
	draw.finish(&draw);
	swffont->glyph[t].shape = swf_ShapeDrawerToShape(&draw);

	SRECT bbox = swf_ShapeDrawerGetBBox(&draw);
	swf_ExpandRect2(&max, &bbox);

	swffont->layout->bounds[t] = bbox;
	    
	if(advance<32768.0/20) {
	    swffont->glyph[t].advance = (int)(advance*20);
	} else {
	    //msg("<warning> Advance value overflow in glyph %d", t);
	    swffont->glyph[t].advance = 32767;
	}

	draw.dealloc(&draw);

	swf_ExpandRect2(&bounds, &swffont->layout->bounds[t]);
    }

    for(t=0;t<font->num_glyphs;t++) {
	SRECT bbox = swffont->layout->bounds[t];

	/* if the glyph doesn't have a bounding box, use the
	   combined bounding box (necessary e.g. for space characters) */
	if(!(bbox.xmin|bbox.ymin|bbox.xmax|bbox.ymax)) {
	    swffont->layout->bounds[t] = bbox = max;
	}
	
	/* check that the advance value is reasonable, by comparing it
	   with the bounding box */
	if(bbox.xmax>0 && (bbox.xmax*10 < swffont->glyph[t].advance || !swffont->glyph[t].advance)) {
	    if(swffont->glyph[t].advance)
		msg("<warning> fix bad advance value for char %d: bbox=%.2f, advance=%.2f\n", t, bbox.xmax/20.0, swffont->glyph[t].advance/20.0);
	    swffont->glyph[t].advance = bbox.xmax;
	}
	//swffont->glyph[t].advance = bbox.xmax - bbox.xmin;
    }


    /* Flash player will use the advance value from the char, and the ascent/descent values
       from the layout for text selection.
       ascent will extend the char into negative y direction, from the baseline, while descent
       will extend in positive y direction, also from the baseline.
       The baseline is defined as the y-position zero 
     */

    swffont->layout->ascent = bounds.ymin<0?-bounds.ymin:0;
    swffont->layout->descent = bounds.ymax>0?bounds.ymax:0;
    swffont->layout->leading = bounds.ymax - bounds.ymin;

    /* if the font has proper ascent/descent values (>0) and those define
       greater (but not overly large) line spacing than what we estimated 
       from the bounding boxes, use the font's parameters */
    if(font->ascent*20 > swffont->layout->ascent && font->ascent*20*2 < swffont->layout->ascent)
	swffont->layout->ascent = font->ascent*20;
    if(font->descent*20 > swffont->layout->descent && font->descent*20*2 < swffont->layout->descent)
	swffont->layout->descent = font->descent*20;

    swf_FontSort(swffont);
    return swffont;
}

static void swf_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    if(i->swffont && i->swffont->name && !strcmp((char*)i->swffont->name,font->id))
	return; // the requested font is the current font
    
    fontlist_t*last=0,*l = i->fontlist;
    while(l) {
	last = l;
	if(!strcmp((char*)l->swffont->name, font->id)) {
	    return; // we already know this font
	}
	l = l->next;
    }
    l = (fontlist_t*)rfx_calloc(sizeof(fontlist_t));
    l->swffont = gfxfont_to_swffont(font, font->id, (i->config_flashversion>=8 && !NO_FONT3)?3:2);
    l->next = 0;
    if(last) {
	last->next = l;
    } else {
	i->fontlist = l;
    }
    swf_FontSetID(l->swffont, getNewID(i->dev));

    if(getScreenLogLevel() >= LOGLEVEL_DEBUG)  {
	int iii;
	// print font information
	msg("<debug> Font %s",font->id);
	msg("<debug> |   ID: %d", l->swffont->id);
	msg("<debug> |   Version: %d", l->swffont->version);
	msg("<debug> |   Name: %s", l->swffont->name);
	msg("<debug> |   Numchars: %d", l->swffont->numchars);
	msg("<debug> |   Maxascii: %d", l->swffont->maxascii);
	msg("<debug> |   Style: %d", l->swffont->style);
	msg("<debug> |   Encoding: %d", l->swffont->encoding);
	if(l->swffont->layout) {
	    msg("<debug> |   Ascent: %.2f", l->swffont->layout->ascent / 20.0);
	    msg("<debug> |   Descent: %.2f", l->swffont->layout->descent / 20.0);
	    msg("<debug> |   Leading: %.2f", l->swffont->layout->leading / 20.0);
	}

	for(iii=0; iii<l->swffont->numchars;iii++) {
	    msg("<debug> |   Glyph %d) name=%s, unicode=%d size=%d bbox=(%.2f,%.2f,%.2f,%.2f)\n", iii, l->swffont->glyphnames?l->swffont->glyphnames[iii]:"<nonames>", l->swffont->glyph2ascii[iii], l->swffont->glyph[iii].shape->bitlen, 
		    l->swffont->layout->bounds[iii].xmin/20.0,
		    l->swffont->layout->bounds[iii].ymin/20.0,
		    l->swffont->layout->bounds[iii].xmax/20.0,
		    l->swffont->layout->bounds[iii].ymax/20.0
		    );
	}
    }
}

static void swf_switchfont(gfxdevice_t*dev, const char*fontid)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    if(i->swffont && i->swffont->name && !strcmp((char*)i->swffont->name,fontid))
	return; // the requested font is the current font
    
    fontlist_t*l = i->fontlist;
    while(l) {
	if(!strcmp((char*)l->swffont->name, fontid)) {
	    i->swffont = l->swffont;
	    return; //done!
	}
	l = l->next;
    }
    msg("<error> Unknown font id: %s", fontid);
    return;
}

/* sets the matrix which is to be applied to characters drawn by swfoutput_drawchar() */
static void setfontscale(gfxdevice_t*dev,double m11,double m12, double m21,double m22,double x, double y, char force)
{
    m11 *= 1024;
    m12 *= 1024;
    m21 *= 1024;
    m22 *= 1024;
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->lastfontm11 == m11 &&
       i->lastfontm12 == m12 &&
       i->lastfontm21 == m21 &&
       i->lastfontm22 == m22 && !force)
        return;
   if(i->textmode)
	endtext(dev);
    
    i->lastfontm11 = m11;
    i->lastfontm12 = m12;
    i->lastfontm21 = m21;
    i->lastfontm22 = m22;

    double xsize = sqrt(m11*m11 + m12*m12);
    double ysize = sqrt(m21*m21 + m22*m22);

    int extrazoom = 1;
    if(i->config_flashversion>=8 && !NO_FONT3)
	extrazoom = 20;

    i->current_font_size = (xsize>ysize?xsize:ysize)*extrazoom;
    if(i->current_font_size < 1)
	i->current_font_size = 1;

    MATRIX m;
    swf_GetMatrix(0, &m);

    if(m21 || m12 || fabs(m11+m22)>0.001 || m11<0) {
	double ifs = (double)extrazoom/(i->current_font_size);
	m.sx =  (S32)((m11*ifs)*65536); m.r1 = -(S32)((m21*ifs)*65536);
	m.r0 =  (S32)((m12*ifs)*65536); m.sy = -(S32)((m22*ifs)*65536); 
    }

    /* this is the position of the first char to set a new fontmatrix-
       we hope that it's close enough to all other characters using the
       font, so we use its position as origin for the matrix */
    m.tx = x*20;
    m.ty = y*20;
    i->fontmatrix = m;
}


static void swf_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(!font) {
	msg("<error> swf_drawchar called (glyph %d) without font", glyph);
	return;
    }

    if(i->config_drawonlyshapes) {
        gfxglyph_t*g = &font->glyphs[glyph];
        gfxline_t*line2 = gfxline_clone(g->line);
        gfxline_transform(line2, matrix);
	dev->fill(dev, line2, color);
        gfxline_free(line2);
        return;
    }

    if(!i->swffont || !i->swffont->name || strcmp((char*)i->swffont->name,font->id)) // not equal to current font
    {
	swf_switchfont(dev, font->id); // set the current font
    }

    if(!i->swffont) {
	msg("<warning> swf_drawchar: Font is NULL");
	return;
    }
    if(glyph<0 || glyph>=i->swffont->numchars) {
	msg("<warning> No character %d in font %s (%d chars)", glyph, FIXNULL((char*)i->swffont->name), i->swffont->numchars);
	return;
    }
    glyph = i->swffont->glyph2glyph[glyph];
    
    setfontscale(dev, matrix->m00, matrix->m01, matrix->m10, matrix->m11, matrix->tx, matrix->ty, 0);
    
    double det = i->fontmatrix.sx/65536.0 * i->fontmatrix.sy/65536.0 - 
	         i->fontmatrix.r0/65536.0 * i->fontmatrix.r1/65536.0;
    if(fabs(det) < 0.0005) { 
	/* x direction equals y direction- the text is invisible */
	msg("<verbose> Not drawing invisible character %d (det=%f, m=[%f %f;%f %f]\n", glyph, 
		det,
		i->fontmatrix.sx/65536.0, i->fontmatrix.r1/65536.0, 
		i->fontmatrix.r0/65536.0, i->fontmatrix.sy/65536.0);
	return;
    }

    /*if(i->swffont->glyph[glyph].shape->bitlen <= 16) {
	msg("<warning> Glyph %d in current charset (%s, %d characters) is empty", 
		glyph, FIXNULL((char*)i->swffont->name), i->swffont->numchars);
	return 1;
    }*/

    /* calculate character position with respect to the current font matrix */
    double s = 20 * GLYPH_SCALE / det;
    double px = matrix->tx - i->fontmatrix.tx/20.0;
    double py = matrix->ty - i->fontmatrix.ty/20.0;
    int x = (SCOORD)((  px * i->fontmatrix.sy/65536.0 - py * i->fontmatrix.r1/65536.0)*s);
    int y = (SCOORD)((- px * i->fontmatrix.r0/65536.0 + py * i->fontmatrix.sx/65536.0)*s);
    if(x>32767 || x<-32768 || y>32767 || y<-32768) {
	msg("<verbose> Moving character origin to %f %f\n", matrix->tx, matrix->ty);
	endtext(dev);
	setfontscale(dev, matrix->m00, matrix->m01, matrix->m10, matrix->m11, matrix->tx, matrix->ty, 1);
        /* since we just moved the char origin to the current char's position, 
           it now has the relative position (0,0) */
        x = y = 0;
    }
    
    if(i->shapeid>=0)
	endshape(dev);
    
    if(i->config_animate) {
        endtext(dev);
	i->tag = swf_InsertTag(i->tag,ST_SHOWFRAME);
    }

    if(!i->textmode)
	starttext(dev);
    
    msg("<trace> Drawing char %d in font %d at %d,%d in color %02x%02x%02x%02x", 
	    glyph, i->swffont->id, x, y, color->r, color->g, color->b, color->a);

    if(color->a == 0 && i->config_invisibletexttofront) {
	RGBA color2 = *(RGBA*)color;
	if(i->config_flashversion>=8) {
	    // use "multiply" blend mode
	    color2.a = color2.r = color2.g = color2.b = 255;
	}
	i->topchardata = charbuffer_append(i->topchardata, i->swffont, glyph, x, y, i->current_font_size, color2, &i->fontmatrix);
    } else {
	i->chardata = charbuffer_append(i->chardata, i->swffont, glyph, x, y, i->current_font_size, *(RGBA*)color, &i->fontmatrix);
    }
    swf_FontUseGlyph(i->swffont, glyph, i->current_font_size);
    return;
}
