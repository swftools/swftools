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
#include <unistd.h>
#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(a)
#endif
#include <math.h>
#include "../log.h"
#include "../rfxswf.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../art/libart.h"
#include "artsutils.c"

#define CHARDATAMAX 8192
#define CHARMIDX 0
#define CHARMIDY 0

typedef struct _chardata {
    int charid;
    int fontid; /* TODO: use a SWFFONT instead */
    int x;
    int y;
    int size;
    RGBA color;
} chardata_t;

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
    int config_opennewwindow;
    int config_ignoredraworder;
    int config_drawonlyshapes;
    int config_jpegquality;
    int config_storeallcharacters;
    int config_generate_fake_tags;
    int config_enablezlib;
    int config_insertstoptag;
    int config_flashversion;
    int config_reordertags;
    int config_splinemaxerror;
    int config_fontsplinemaxerror;
    int config_filloverlap;
    int config_protect;
    int config_bboxvars;
    int config_disable_polygon_conversion;
    RGBA config_linkcolor;
    float config_minlinewidth;
    double config_caplinewidth;
    char* config_linktarget;
    char*config_internallinkfunction;
    char*config_externallinkfunction;

    SWF* swf;

    fontlist_t* fontlist;

    char storefont;

    MATRIX page_matrix;

    TAG *tag;
    int currentswfid;
    int depth;
    int startdepth;
    int linewidth;
    
    SHAPE* shape;
    int shapeid;
    int textid;
    
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
    int pic_xids[1024];
    int pic_yids[1024];
    int pic_ids[1024];
    int pic_width[1024];
    int pic_height[1024];
    int picpos;

    int frameno;
    int lastframeno;
    
    char fillstylechanged;

    int jpeg; //next image type
    
    int bboxrectpos;
    SRECT bboxrect;

    chardata_t chardata[CHARDATAMAX];
    int chardatapos;
    int firstpage;
    char pagefinished;

    char overflow;

    MATRIX fontmatrix;
    double fontm11,fontm12,fontm21,fontm22;
    SWFFONT *swffont;
    RGBA strokergb;
    RGBA fillrgb;
    int drawmode;

    int shapeposx;
    int shapeposy;

    char* mark;

} swfoutput_internal;
    
static void swf_fillbitmap(gfxdevice_t*driver, gfxline_t*line, gfximage_t*img, gfxmatrix_t*move, gfxcxform_t*cxform);
static int  swf_setparameter(gfxdevice_t*driver, const char*key, const char*value);
static void swf_drawstroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit);
static void swf_startclip(gfxdevice_t*dev, gfxline_t*line);
static void swf_endclip(gfxdevice_t*dev);
static void swf_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit);
static void swf_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color);
static void swf_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform);
static void swf_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix);
static void swf_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix);
static void swf_addfont(gfxdevice_t*dev, gfxfont_t*font);
static void swf_drawlink(gfxdevice_t*dev, gfxline_t*line, char*action);
static void swf_startframe(gfxdevice_t*dev, int width, int height);
static void swf_endframe(gfxdevice_t*dev);
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
    i->textid = -1;
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
    i->chardatapos = 0;
    i->firstpage = 1;
    i->pagefinished = 1;

    i->config_dumpfonts=0;
    i->config_ppmsubpixels=0;
    i->config_jpegsubpixels=0;
    i->config_opennewwindow=1;
    i->config_ignoredraworder=0;
    i->config_drawonlyshapes=0;
    i->config_jpegquality=85;
    i->config_storeallcharacters=0;
    i->config_enablezlib=0;
    i->config_generate_fake_tags=0;
    i->config_insertstoptag=0;
    i->config_flashversion=6;
    i->config_splinemaxerror=1;
    i->config_fontsplinemaxerror=1;
    i->config_filloverlap=0;
    i->config_protect=0;
    i->config_bboxvars=0;
    i->config_minlinewidth=0.05;
    i->config_caplinewidth=1;
    i->config_linktarget=0;
    i->config_internallinkfunction=0;
    i->config_externallinkfunction=0;
    i->config_reordertags=1;

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
    if(i->depth == 65535) {
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

// write a move-to command into the swf
static int movetoxy(gfxdevice_t*dev, TAG*tag, plotxy_t p0)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    int rx = (int)(p0.x*20);
    int ry = (int)(p0.y*20);
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
    int px = (int)(p0.x*20);
    int py = (int)(p0.y*20);
    int rx = (px-i->swflastx);
    int ry = (py-i->swflasty);
    if(rx|ry) {
	swf_ShapeSetLine (tag, i->shape, rx,ry);
	addPointToBBox(dev, i->swflastx,i->swflasty);
	addPointToBBox(dev, px,py);
    }/* else if(!i->fill) {
       // treat lines of length 0 as plots, making them
       // at least 1 twip wide so Flash will display them
	plot(dev, i->swflastx, i->swflasty, tag);
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

    int cx = ((int)(control.x*20)-i->swflastx);
    int cy = ((int)(control.y*20)-i->swflasty);
    i->swflastx += cx;
    i->swflasty += cy;
    int ex = ((int)(end.x*20)-i->swflastx);
    int ey = ((int)(end.y*20)-i->swflasty);
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

static inline int colorcompare(gfxdevice_t*dev, RGBA*a,RGBA*b)
{

    if(a->r!=b->r ||
       a->g!=b->g ||
       a->b!=b->b ||
       a->a!=b->a) {
        return 0;
    }
    return 1;
}

static SRECT getcharacterbbox(gfxdevice_t*dev, SWFFONT*font, MATRIX* m)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    SRECT r;
    char debug = 0;
    memset(&r, 0, sizeof(r));

    int t;
    if(debug) printf("\n");
    for(t=0;t<i->chardatapos;t++)
    {
	if(i->chardata[t].fontid != font->id) {
	    msg("<error> Internal error: fontid %d != fontid %d", i->chardata[t].fontid, font->id);
	    exit(1);
	}
	SRECT b = font->layout->bounds[i->chardata[t].charid];
	b.xmin *= i->chardata[t].size;
	b.ymin *= i->chardata[t].size;
	b.xmax *= i->chardata[t].size;
	b.ymax *= i->chardata[t].size;

	/* divide by 1024, rounding xmax/ymax up */
	b.xmax += 1023; b.ymax += 1023; b.xmin /= 1024; b.ymin /= 1024; b.xmax /= 1024; b.ymax /= 1024;

	b.xmin += i->chardata[t].x;
	b.ymin += i->chardata[t].y;
	b.xmax += i->chardata[t].x;
	b.ymax += i->chardata[t].y;

	/* until we solve the INTERNAL_SCALING problem (see below)
	   make sure the bounding box is big enough */
	b.xmin -= 20;
	b.ymin -= 20;
	b.xmax += 20;
	b.ymax += 20;

	b = swf_TurnRect(b, m);

	if(debug) printf("(%f,%f,%f,%f) -> (%f,%f,%f,%f) [font %d/%d, char %d]\n",
		font->layout->bounds[i->chardata[t].charid].xmin/20.0,
		font->layout->bounds[i->chardata[t].charid].ymin/20.0,
		font->layout->bounds[i->chardata[t].charid].xmax/20.0,
		font->layout->bounds[i->chardata[t].charid].ymax/20.0,
		b.xmin/20.0,
		b.ymin/20.0,
		b.xmax/20.0,
		b.ymax/20.0,
		i->chardata[t].fontid,
		font->id,
		i->chardata[t].charid
		);
	swf_ExpandRect2(&r, &b);
    }
    if(debug) printf("-----> (%f,%f,%f,%f)\n",
	    r.xmin/20.0,
	    r.ymin/20.0,
	    r.xmax/20.0,
	    r.ymax/20.0);
    return r;
}

static void putcharacters(gfxdevice_t*dev, TAG*tag)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    int t;
    SWFFONT font;
    RGBA color;
    color.r = i->chardata[0].color.r^255;
    color.g = 0;
    color.b = 0;
    color.a = 0;
    int lastfontid;
    int lastx;
    int lasty;
    int lastsize;
    int charids[128];
    int charadvance[128];
    int charstorepos;
    int pass;
    int glyphbits=1; //TODO: can this be zero?
    int advancebits=1;

    if(tag->id != ST_DEFINETEXT &&
        tag->id != ST_DEFINETEXT2) {
        msg("<error> internal error: putcharacters needs an text tag, not %d\n",tag->id);
        exit(1);
    }
    if(!i->chardatapos) {
        msg("<warning> putcharacters called with zero characters");
    }

    for(pass = 0; pass < 2; pass++)
    {
        charstorepos = 0;
        lastfontid = -1;
        lastx = CHARMIDX;
        lasty = CHARMIDY;
        lastsize = -1;

        if(pass==1)
        {
            advancebits++; // add sign bit
            swf_SetU8(tag, glyphbits);
            swf_SetU8(tag, advancebits);
        }

        for(t=0;t<=i->chardatapos;t++)
        {
            if(lastfontid != i->chardata[t].fontid || 
                    lastx!=i->chardata[t].x ||
                    lasty!=i->chardata[t].y ||
                    !colorcompare(dev,&color, &i->chardata[t].color) ||
                    charstorepos==127 ||
                    lastsize != i->chardata[t].size ||
                    t == i->chardatapos)
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

                if(pass == 1 && t<i->chardatapos)
                {
                    RGBA*newcolor=0;
                    SWFFONT*newfont=0;
                    int newx = 0;
                    int newy = 0;
                    if(lastx != i->chardata[t].x ||
                       lasty != i->chardata[t].y)
                    {
                        newx = i->chardata[t].x;
                        newy = i->chardata[t].y;
			if(newx == 0)
			    newx = SET_TO_ZERO;
			if(newy == 0)
			    newy = SET_TO_ZERO;
                    }
                    if(!colorcompare(dev,&color, &i->chardata[t].color)) 
                    {
                        color = i->chardata[t].color;
                        newcolor = &color;
                    }
                    font.id = i->chardata[t].fontid;
                    if(lastfontid != i->chardata[t].fontid || lastsize != i->chardata[t].size)
                        newfont = &font;

                    tag->writeBit = 0; // Q&D
                    swf_TextSetInfoRecord(tag, newfont, i->chardata[t].size, newcolor, newx,newy);
                }

                lastfontid = i->chardata[t].fontid;
                lastx = i->chardata[t].x;
                lasty = i->chardata[t].y;
                lastsize = i->chardata[t].size;
            }

            if(t==i->chardatapos)
                    break;

            int advance;
            int nextt = t==i->chardatapos-1?t:t+1;
            int rel = i->chardata[nextt].x-i->chardata[t].x;
            if(rel>=0 && (rel<(1<<(advancebits-1)) || pass==0)) {
               advance = rel;
               lastx=i->chardata[nextt].x;
            }
            else {
               advance = 0;
               lastx=i->chardata[t].x;
            }
            charids[charstorepos] = i->chardata[t].charid;
            charadvance[charstorepos] = advance;
            charstorepos ++;
        }
    }
    i->chardatapos = 0;
}

static void putcharacter(gfxdevice_t*dev, int fontid, int charid, int x,int y, int size, RGBA color)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->chardatapos == CHARDATAMAX)
    {
	msg("<warning> Character buffer too small. SWF will be slightly bigger");
        endtext(dev);
        starttext(dev);
    }
    i->chardata[i->chardatapos].fontid = fontid;
    i->chardata[i->chardatapos].charid = charid;
    i->chardata[i->chardatapos].x = x;
    i->chardata[i->chardatapos].y = y;
    i->chardata[i->chardatapos].color = color;
    i->chardata[i->chardatapos].size = size;
    i->chardatapos++;
}

/* Notice: we can only put chars in the range -1639,1638 (-32768/20,32768/20).
   So if we set this value to high, the char coordinates will overflow.
   If we set it to low, however, the char positions will be inaccurate */
#define FONT_INTERNAL_SIZE 4

/* process a character. */
static int drawchar(gfxdevice_t*dev, SWFFONT *swffont, int charid, swfmatrix_t*m, gfxcolor_t*col)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(!swffont) {
	msg("<warning> Font is NULL");
	return 0;
    }

    if(charid<0 || charid>=swffont->numchars) {
	msg("<warning> No character %d in font %s ", charid, FIXNULL((char*)swffont->name));
	return 0;
    }
    /*if(swffont->glyph[charid].shape->bitlen <= 16) {
	msg("<warning> Glyph %d in current charset (%s, %d characters) is empty", 
		charid, FIXNULL((char*)swffont->name), swffont->numchars);
	return 1;
    }*/

    if(i->shapeid>=0)
	endshape(dev);
    if(i->textid<0)
	starttext(dev);

    float x = m->m31;
    float y = m->m32;
    float det = ((m->m11*m->m22)-(m->m21*m->m12));
    if(fabs(det) < 0.0005) { 
	/* x direction equals y direction- the text is invisible */
	return 1;
    }
    det = 20*FONT_INTERNAL_SIZE / det;

    SPOINT p;
    p.x = (SCOORD)((  x * m->m22 - y * m->m12)*det);
    p.y = (SCOORD)((- x * m->m21 + y * m->m11)*det);

    RGBA rgba = *(RGBA*)col;
    putcharacter(dev, swffont->id, charid,p.x,p.y,FONT_INTERNAL_SIZE, rgba);
    swf_FontUseGlyph(swffont, charid);
    return 1;
}

static void endtext(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->textid<0)
        return;

    i->tag = swf_InsertTag(i->tag,ST_DEFINETEXT);
    swf_SetU16(i->tag, i->textid);

    SRECT r;
    r = getcharacterbbox(dev, i->swffont, &i->fontmatrix);
    
    swf_SetRect(i->tag,&r);

    swf_SetMatrix(i->tag,&i->fontmatrix);

    msg("<trace> Placing text (%d characters) as ID %d", i->chardatapos, i->textid);

    putcharacters(dev, i->tag);
    swf_SetU8(i->tag,0);

    if(i->swf->fileVersion >= 8) {
	i->tag = swf_InsertTag(i->tag, ST_CSMTEXTSETTINGS);
	swf_SetU16(i->tag, i->textid);
	//swf_SetU8(i->tag, /*subpixel grid*/(2<<3)|/*flashtype*/0x40);
	//swf_SetU8(i->tag, /*grid*/(1<<3)|/*flashtype*/0x40);
	//swf_SetU8(i->tag, /*grid*/(0<<3)|/*flashtype*/0x40);
	swf_SetU8(i->tag, /*grid*/(1<<3)|/*no flashtype*/0x00);
	swf_SetU32(i->tag, 0);//thickness
	swf_SetU32(i->tag, 0);//sharpness
	swf_SetU8(i->tag, 0);//reserved
    }
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);

    swf_ObjectPlace(i->tag,i->textid,getNewDepth(dev),&i->page_matrix,NULL,NULL);
    i->textid = -1;
}

/* set's the matrix which is to be applied to characters drawn by swfoutput_drawchar() */
static void swfoutput_setfontmatrix(gfxdevice_t*dev,double m11,double m21,
                                                  double m12,double m22)
{
    m11 *= 1024;
    m12 *= 1024;
    m21 *= 1024;
    m22 *= 1024;
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->fontm11 == m11 &&
       i->fontm12 == m12 &&
       i->fontm21 == m21 &&
       i->fontm22 == m22)
        return;
   if(i->textid>=0)
	endtext(dev);
    i->fontm11 = m11;
    i->fontm12 = m12;
    i->fontm21 = m21;
    i->fontm22 = m22;
    
    MATRIX m;
    m.sx = (U32)(((i->fontm11)*65536)/FONT_INTERNAL_SIZE); m.r1 = (U32)(((i->fontm12)*65536)/FONT_INTERNAL_SIZE);
    m.r0 = (U32)(((i->fontm21)*65536)/FONT_INTERNAL_SIZE); m.sy = (U32)(((i->fontm22)*65536)/FONT_INTERNAL_SIZE); 
    m.tx = 0;
    m.ty = 0;
    i->fontmatrix = m;
}

static void endpage(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->shapeid>=0)
      endshape(dev);
    if(i->textid>=0)
      endtext(dev);
    while(i->clippos)
        dev->endclip(dev);
    i->pagefinished = 1;
}

void swf_startframe(gfxdevice_t*dev, int width, int height)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
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

    /* set clipping/background rectangle */
    /* TODO: this should all be done in SWFOutputDev */
    //setBackground(dev, x1, y1, x2, y2);

    /* increase SWF's bounding box */
    SRECT r;
    r.xmin = 0;
    r.ymin = 0;
    r.xmax = width*20;
    r.ymax = height*20;
    swf_ExpandRect2(&i->swf->movieSize, &r);

    i->lastframeno = i->frameno;
    i->firstpage = 0;
    i->pagefinished = 0;
}

void swf_endframe(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    
    if(!i->pagefinished)
        endpage(dev);

    if(i->config_insertstoptag) {
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

    dev->internal = init_internal_struct();

    dev->startpage = swf_startframe;
    dev->endpage = swf_endframe;
    dev->finish = swf_finish;
    dev->fillbitmap = swf_fillbitmap;
    dev->setparameter = swf_setparameter;
    dev->stroke = swf_stroke;
    dev->startclip = swf_startclip;
    dev->endclip = swf_endclip;
    dev->fill = swf_fill;
    dev->fillbitmap = swf_fillbitmap;
    dev->fillgradient = swf_fillgradient;
    dev->addfont = swf_addfont;
    dev->drawchar = swf_drawchar;
    dev->drawlink = swf_drawlink;

    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    i->dev = dev;

    SRECT r;
    RGBA rgb;

    msg("<verbose> initializing swf output\n", i->max_x,i->max_y);

    i->swffont = 0;
   
    i->swf = (SWF*)rfx_calloc(sizeof(SWF));
    i->swf->fileVersion    = i->config_flashversion;
    i->swf->frameRate      = 0x0040; // 1 frame per 4 seconds
    i->swf->movieSize.xmin = 0;
    i->swf->movieSize.ymin = 0;
    i->swf->movieSize.xmax = 0;
    i->swf->movieSize.ymax = 0;
    
    i->swf->firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    i->tag = i->swf->firstTag;
    rgb.a = rgb.r = rgb.g = rgb.b = 0xff;
    swf_SetRGB(i->tag,&rgb);

    i->startdepth = i->depth = 0;
    
    if(i->config_protect)
      i->tag = swf_InsertTag(i->tag, ST_PROTECT);
}

static void startshape(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    SRECT r;

    if(i->shapeid>=0)
	return;

    if(i->textid>=0)
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
    r.xmin = 0;
    r.ymin = 0;
    r.xmax = 20*i->max_x;
    r.ymax = 20*i->max_y;
    swf_SetRect(i->tag,&r);
   
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
      
    i->textid = getNewID(dev);

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
    swf_DeleteTag(todel);
    if(i->shape) {swf_ShapeFree(i->shape);i->shape=0;}
    i->shapeid = -1;
    i->bboxrectpos = -1;

//    i->currentswfid--; // this was an *exceptionally* bad idea
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

    changeRect(dev, i->tag, i->bboxrectpos, &i->bboxrect);

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
	    swf_DeleteTag(tag);
	}
	tag = next;
    }
}


void swfoutput_finalize(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    if(i->tag && i->tag->id == ST_END)
        return; //already done

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

    if(i->frameno == i->lastframeno) // fix: add missing pagefeed
        dev->endpage(dev);

    if(i->mark) {
	free(i->mark);i->mark = 0;
    }

    endpage(dev);
    fontlist_t *tmp,*iterator = i->fontlist;
    while(iterator) {
	TAG*mtag = i->swf->firstTag;
	if(iterator->swffont) {
	    if(!i->config_storeallcharacters) {
		msg("<debug> Reducing font %s", iterator->swffont->name);
		swf_FontReduce(iterator->swffont);
	    }
	    int used = iterator->swffont->use && iterator->swffont->use->used_glyphs;
	    if(i->config_storeallcharacters || used) {
		mtag = swf_InsertTag(mtag, ST_DEFINEFONT2);
		swf_FontSetDefine2(mtag, iterator->swffont);
	    }
	}

        iterator = iterator->next;
    }
    i->tag = swf_InsertTag(i->tag,ST_END);
    TAG* tag = i->tag->prev;

    /* remove the removeobject2 tags between the last ST_SHOWFRAME
       and the ST_END- they confuse the flash player  */
    while(tag->id == ST_REMOVEOBJECT2) {
        TAG* prev = tag->prev;
        swf_DeleteTag(tag);
        tag = prev;
    }
    
    if(i->overflow) {
	wipeSWF(i->swf);
    }
    if(i->config_enablezlib || i->config_flashversion>=6) {
	i->swf->compressed = 1;
    }

    if(i->config_reordertags)
	swf_Optimize(i->swf);
}

int swfresult_save(gfxresult_t*gfx, char*filename)
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
    
    if(swf->compressed) {
	if FAILED(swf_WriteSWC(fi,swf)) 
	    msg("<error> WriteSWC() failed.\n");
    } else {
	if FAILED(swf_WriteSWF(fi,swf)) 
	    msg("<error> WriteSWF() failed.\n");
    }

    if(filename)
     close(fi);
    return 0;
}
void* swfresult_get(gfxresult_t*gfx, char*name)
{
    SWF*swf = (SWF*)gfx->internal;
    if(!strcmp(name, "swf")) {
	return (void*)swf_CopySWF(swf);
    } else if(!strcmp(name, "xmin")) {
	return (void*)(swf->movieSize.xmin/20);
    } else if(!strcmp(name, "ymin")) {
	return (void*)(swf->movieSize.ymin/20);
    } else if(!strcmp(name, "xmax")) {
	return (void*)(swf->movieSize.xmax/20);
    } else if(!strcmp(name, "ymax")) {
	return (void*)(swf->movieSize.ymax/20);
    } else if(!strcmp(name, "width")) {
	return (void*)((swf->movieSize.xmax - swf->movieSize.xmin)/20);
    } else if(!strcmp(name, "height")) {
	return (void*)((swf->movieSize.ymax - swf->movieSize.ymin)/20);
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


static void drawlink(gfxdevice_t*dev, ActionTAG*,ActionTAG*, gfxline_t*points, char mouseover);
static void swfoutput_namedlink(gfxdevice_t*dev, char*name, gfxline_t*points);
static void swfoutput_linktopage(gfxdevice_t*dev, int page, gfxline_t*points);
static void swfoutput_linktourl(gfxdevice_t*dev, char*url, gfxline_t*points);

void swfoutput_drawlink(gfxdevice_t*dev, char*url, gfxline_t*points)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    dev->drawlink(dev, points, url);
}

void swf_drawlink(gfxdevice_t*dev, gfxline_t*points, char*url)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

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
void swfoutput_linktourl(gfxdevice_t*dev, char*url, gfxline_t*points)
{
    ActionTAG* actions = 0;
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    if(i->shapeid>=0)
	endshape(dev);
    if(i->textid>=0)
	endtext(dev);
    
    if(i->config_externallinkfunction) {
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
    
    drawlink(dev, actions, 0, points,0);
}
void swfoutput_linktopage(gfxdevice_t*dev, int page, gfxline_t*points)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    ActionTAG* actions = 0;

    if(i->shapeid>=0)
	endshape(dev);
    if(i->textid>=0)
	endtext(dev);
  
    if(!i->config_internallinkfunction) {
	actions = action_GotoFrame(actions, page-1);
	actions = action_End(actions);
    } else {
	actions = action_PushInt(actions, page); //parameter
	actions = action_PushInt(actions, 1); //number of parameters (1)
	actions = action_PushString(actions, i->config_internallinkfunction); //function name
	actions = action_CallFunction(actions);
	actions = action_End(actions);
    }

    drawlink(dev, actions, 0, points,0);
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
    if(i->textid>=0)
	endtext(dev);

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
    }

    drawlink(dev, actions1, actions2, points,mouseover);

    swf_ActionFree(actions1);
    swf_ActionFree(actions2);
    free(tmp);
}

static void drawgfxline(gfxdevice_t*dev, gfxline_t*line)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    gfxcoord_t lastx=0,lasty=0,px=0,py=0;
    char lastwasmoveto;
    int lines= 0, splines=0;
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


static void drawlink(gfxdevice_t*dev, ActionTAG*actions1, ActionTAG*actions2, gfxline_t*points, char mouseover)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    RGBA rgb;
    SRECT r;
    int lsid=0;
    int fsid;
    plotxy_t p1,p2,p3,p4;
    int myshapeid;
    int myshapeid2;
    double posx = 0;
    double posy = 0;
    int t;
    int buttonid = getNewID(dev);
    gfxbbox_t bbox = gfxline_getbbox(points);

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
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,i->shape);
    swf_ShapeCountBits(i->shape,NULL,NULL);
    swf_SetShapeBits(i->tag,i->shape);
    swf_ShapeSetAll(i->tag,i->shape,/*x*/0,/*y*/0,0,fsid,0);
    i->swflastx = i->swflasty = 0;
    drawgfxline(dev, points);
    swf_ShapeSetEnd(i->tag);

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
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,i->shape);
    swf_ShapeCountBits(i->shape,NULL,NULL);
    swf_SetShapeBits(i->tag,i->shape);
    swf_ShapeSetAll(i->tag,i->shape,/*x*/0,/*y*/0,0,fsid,0);
    i->swflastx = i->swflasty = 0;
    drawgfxline(dev, points);
    swf_ShapeSetEnd(i->tag);

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
	swf_ObjectPlace(i->tag, buttonid, getNewDepth(dev),&m,0,0);
    } else {
	swf_ObjectPlace(i->tag, buttonid, getNewDepth(dev),&i->page_matrix,0,0);
    }
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
    } else if(!strcmp(name, "reordertags")) {
	i->config_reordertags = atoi(value);
    } else if(!strcmp(name, "internallinkfunction")) {
	i->config_internallinkfunction = strdup(value);
    } else if(!strcmp(name, "externallinkfunction")) {
	i->config_externallinkfunction = strdup(value);
    } else if(!strcmp(name, "disable_polygon_conversion")) {
	i->config_disable_polygon_conversion = atoi(value);
    } else if(!strcmp(name, "insertstop")) {
	i->config_insertstoptag = atoi(value);
    } else if(!strcmp(name, "protect")) {
	i->config_protect = atoi(value);
	if(i->config_protect && i->tag) {
	    i->tag = swf_InsertTag(i->tag, ST_PROTECT);
	}
    } else if(!strcmp(name, "faketags")) {
	i->config_generate_fake_tags = atoi(value);
    } else if(!strcmp(name, "flashversion")) {
	i->config_flashversion = atoi(value);
	if(i->swf) {
	    i->swf->fileVersion = i->config_flashversion;
	}
    } else if(!strcmp(name, "minlinewidth")) {
	i->config_minlinewidth = atof(value);
    } else if(!strcmp(name, "caplinewidth")) {
	i->config_caplinewidth = atof(value);
    } else if(!strcmp(name, "linktarget")) {
	i->config_linktarget = strdup(value);
    } else if(!strcmp(name, "dumpfonts")) {
	i->config_dumpfonts = atoi(value);
    } else if(!strcmp(name, "next_bitmap_is_jpeg")) {
	i->jpeg = 1;
    } else if(!strcmp(name, "jpegquality")) {
	int val = atoi(value);
	if(val<0) val=0;
	if(val>100) val=100;
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
	printf("%02x%02x%02x%02x\n", 
	    i->config_linkcolor.r,
	    i->config_linkcolor.g,
	    i->config_linkcolor.b,
	    i->config_linkcolor.a);

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
	newpic = swf_ImageScale(mem, sizex, sizey, newsizex, newsizey);
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

int line_is_empty(gfxline_t*line)
{
    while(line) {
	if(line->type != gfx_moveTo)
	    return 0;
	line = line->next;
    }
    return 1;
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
    float m00,m10,tx;
    float m01,m11,ty;
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
    swf_SetU16(i->tag, myshapeid);
    SRECT r = gfxline_getSWFbbox(line);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(i->tag,shape);
    swf_ShapeSetAll(i->tag,shape,UNDEFINED_COORD,UNDEFINED_COORD,0,fsid,0);
    i->swflastx = i->swflasty = UNDEFINED_COORD;
    drawgfxline(dev, line);
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(shape);

    msg("<trace> Placing bitmap ID %d", myshapeid);
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    CXFORM cxform2 = gfxcxform_to_cxform(cxform);
    swf_ObjectPlace(i->tag,myshapeid,getNewDepth(dev),&i->page_matrix,&cxform2,NULL);
}

static void swf_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    endtext(dev);
    endshape(dev);

    if(i->clippos >= 127)
    {
        msg("<warning> Too many clip levels.");
        i->clippos --;
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
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(i->tag,shape);
    swf_ShapeSetAll(i->tag,shape,UNDEFINED_COORD,UNDEFINED_COORD,0,fsid,0);
    i->swflastx = i->swflasty = UNDEFINED_COORD;
    drawgfxline(dev, line);
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
    if(i->textid>=0)
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

    /* TODO: * split line into segments, and perform this check for all segments */

    if(i->config_disable_polygon_conversion || type>=5 ||
       (!has_dots &&
        (width <= i->config_caplinewidth 
        || (cap_style == gfx_capRound && joint_style == gfx_joinRound)
        || (cap_style == gfx_capRound && type<=2)))) {} else 
    {
	/* convert line to polygon */
	msg("<trace> draw as polygon, type=%d dots=%d", type, has_dots);
	if(has_dots)
	    gfxline_fix_short_edges(line);
	/* we need to convert the line into a polygon */
	ArtSVP* svp = gfxstrokeToSVP(line, width, cap_style, joint_style, miterLimit);
	gfxline_t*gfxline = SVPtogfxline(svp);
	dev->fill(dev, gfxline, color);
	free(gfxline);
	art_svp_free(svp);
	return;
    }

    msg("<trace> draw as stroke, type=%d dots=%d", type, has_dots);
    endtext(dev);

#ifdef NORMALIZE_POLYGON_POSITIONS
    endshape(dev);
    double startx = 0, starty = 0;
    if(line && line->type == gfx_moveTo) {
	startx = line->x;
	starty = line->y;
    }
    line = gfxline_move(line, -startx, -starty);
    i->shapeposx = (int)(startx*20);
    i->shapeposy = (int)(starty*20);
#endif

    swfoutput_setstrokecolor(dev, color->r, color->g, color->b, color->a);
    swfoutput_setlinewidth(dev, width);
    startshape(dev);
    stopFill(dev);
    drawgfxline(dev, line);

#ifdef NORMALIZE_POLYGON_POSITIONS
    free(line); //account for _move
#endif

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

    endtext(dev);
    if(!i->config_ignoredraworder)
	endshape(dev);

#ifdef NORMALIZE_POLYGON_POSITIONS
    endshape(dev);
    double startx = 0, starty = 0;
    if(line && line->type == gfx_moveTo) {
	startx = line->x;
	starty = line->y;
    }
    line = gfxline_move(line, -startx, -starty);
    i->shapeposx = (int)(startx*20);
    i->shapeposy = (int)(starty*20);
#endif

    swfoutput_setfillcolor(dev, color->r, color->g, color->b, color->a);
    startshape(dev);
    startFill(dev);
    i->fill=1;
    drawgfxline(dev, line);
    msg("<trace> end of swf_fill (shapeid=%d)", i->shapeid);

#ifdef NORMALIZE_POLYGON_POSITIONS
    free(line); //account for _move
#endif
}
static void swf_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    if(line_is_empty(line))
	return;
    msg("<error> Gradient filling not implemented yet");
}

static SWFFONT* gfxfont_to_swffont(gfxfont_t*font, char* id)
{
    SWFFONT*swffont = (SWFFONT*)rfx_calloc(sizeof(SWFFONT));
    int t;
    SRECT bounds = {0,0,0,0};
    swffont->id = -1;
    swffont->version = 2;
    swffont->name = (U8*)strdup(id);
    swffont->layout = (SWFLAYOUT*)rfx_calloc(sizeof(SWFLAYOUT));
    swffont->layout->ascent = 0; /* ? */
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
    for(t=0;t<font->max_unicode;t++) {
	swffont->ascii2glyph[t] = font->unicode2glyph[t];
    }
    for(t=0;t<font->num_glyphs;t++) {
	drawer_t draw;
	gfxline_t*line;
	int advance = 0;
	swffont->glyph2ascii[t] = font->glyphs[t].unicode;
	if(font->glyphs[t].name) {
	    swffont->glyphnames[t] = strdup(font->glyphs[t].name);
	} else {
	    swffont->glyphnames[t] = 0;
	}
	advance = (int)(font->glyphs[t].advance);

	swf_Shape01DrawerInit(&draw, 0);
	line = font->glyphs[t].line;
	while(line) {
	    FPOINT c,to;
	    c.x = line->sx; c.y = line->sy;
	    to.x = line->x; to.y = line->y;
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
	swffont->layout->bounds[t] = swf_ShapeDrawerGetBBox(&draw);

	if(swffont->layout->bounds[t].xmax && swffont->layout->bounds[t].xmax*2 < advance) {
	    printf("fix bad advance value: bbox=%d, advance=%d (%f)\n", swffont->layout->bounds[t].xmax, advance, font->glyphs[t].advance);
	    advance = swffont->layout->bounds[t].xmax;
	}
	    
	if(advance<32768) {
	    swffont->glyph[t].advance = advance;
	} else {
	    swffont->glyph[t].advance = 32767;
	}

	draw.dealloc(&draw);

	swf_ExpandRect2(&bounds, &swffont->layout->bounds[t]);
    }
    if(bounds.ymin < 0 && bounds.ymax > 0) {
	swffont->layout->ascent = -bounds.ymin;
	swffont->layout->descent = bounds.ymax;
	swffont->layout->leading = bounds.ymax - bounds.ymin;
    } else {
	swffont->layout->ascent = (bounds.ymax - bounds.ymin)/2;
	swffont->layout->descent = (bounds.ymax - bounds.ymin)/2;
	swffont->layout->leading = bounds.ymax - bounds.ymin;
    }

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
    l->swffont = gfxfont_to_swffont(font, font->id);
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
	for(iii=0; iii<l->swffont->numchars;iii++) {
	    msg("<debug> |   Glyph %d) name=%s, unicode=%d size=%d bbox=(%.2f,%.2f,%.2f,%.2f)\n", iii, l->swffont->glyphnames?l->swffont->glyphnames[iii]:"<nonames>", l->swffont->glyph2ascii[iii], l->swffont->glyph[iii].shape->bitlen, 
		    l->swffont->layout->bounds[iii].xmin/20.0,
		    l->swffont->layout->bounds[iii].ymin/20.0,
		    l->swffont->layout->bounds[iii].xmax/20.0,
		    l->swffont->layout->bounds[iii].ymax/20.0
		    );
	    int t;
	    for(t=0;t<l->swffont->maxascii;t++) {
		if(l->swffont->ascii2glyph[t] == iii)
		    msg("<debug> | - maps to %d",t);
	    }
	}
    }
}

static void swf_switchfont(gfxdevice_t*dev, char*fontid)
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

static void swf_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
	
    if(!i->swffont || !i->swffont->name || strcmp((char*)i->swffont->name,font->id)) // not equal to current font
    {
	/* TODO: remove the need for this (enhance getcharacterbbox so that it can cope
		 with multiple fonts */
	endtext(dev);

	swf_switchfont(dev, font->id); // set the current font
    }
    swfoutput_setfontmatrix(dev, matrix->m00, matrix->m01, matrix->m10, matrix->m11);
   
    swfmatrix_t m;
    m.m11 = i->fontm11;
    m.m12 = i->fontm12;
    m.m21 = i->fontm21;
    m.m22 = i->fontm22;
    m.m31 = matrix->tx;
    m.m32 = matrix->ty;
  
/*    printf("%f %f\n", m.m31,  m.m32);
    {
    static int xpos = 40;
    static int ypos = 200;
    m.m31 = xpos;
    m.m32 = ypos;
    xpos += 10;
    }*/


    drawchar(dev, i->swffont, glyph, &m, color);
}
