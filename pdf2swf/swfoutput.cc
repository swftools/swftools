/* swfoutput.cc
   Implements generation of swf files using the rfxswf lib. The routines
   in this file are called from pdf2swf.

   This file is part of swftools.

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
#include "../config.h"
#include <fcntl.h>
#include <unistd.h>
#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(a)
#endif
#include <math.h>
#include "swfoutput.h"
#include "spline.h"
extern "C" {
#include "../lib/log.h"
#include "../lib/rfxswf.h"
#include "../lib/gfxdevice.h"
#include "../lib/gfxtools.h"
}
#include "../lib/art/libart.h"

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

struct fontlist_t 
{
    SWFFONT *swffont;
    fontlist_t*next;
};

double config_dumpfonts=0;
double config_ppmsubpixels=0;
double config_jpegsubpixels=0;
int config_opennewwindow=1;
int config_ignoredraworder=0;
int config_drawonlyshapes=0;
int config_jpegquality=85;
int config_storeallcharacters=0;
int config_enablezlib=0;
int config_insertstoptag=0;
int config_flashversion=5;
int config_splinemaxerror=1;
int config_fontsplinemaxerror=1;
int config_filloverlap=0;
int config_protect=0;
int config_bboxvars=0;
float config_minlinewidth=0.05;
double config_caplinewidth=1;

typedef struct _swfoutput_internal
{
    swfoutput*obj; // the swfoutput object where this internal struct resides

    SWF swf;

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

    TAG*cliptag;
 
    chardata_t chardata[CHARDATAMAX];
    int chardatapos;
    int firstpage;
    char pagefinished;

    char overflow;

    /* during the transition to the gfxdevice interface:
       a device which uses this swfoutput as target */
    gfxdevice_t device;

} swfoutput_internal;
    
void swf_fillbitmap(gfxdevice_t*driver, gfxline_t*line, gfximage_t*img, gfxmatrix_t*move, gfxcxform_t*cxform);
int  swf_setparameter(gfxdevice_t*driver, const char*key, const char*value);
void swf_drawstroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit);
void swf_startclip(gfxdevice_t*dev, gfxline_t*line);
void swf_endclip(gfxdevice_t*dev);
void swf_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit);
void swf_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color);
void swf_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform);
void swf_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix);
void swf_drawchar(gfxdevice_t*dev, char*fontid, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix);
void swf_addfont(gfxdevice_t*dev, char*fontid, gfxfont_t*font);
void swf_drawlink(gfxdevice_t*dev, gfxline_t*line, char*action);

int getCharID(SWFFONT *font, int charnr, char *charname, int u);

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

    i->device.internal = (void*)i;
    i->device.fillbitmap = swf_fillbitmap;
    i->device.setparameter = swf_setparameter;
    i->device.stroke = swf_stroke;
    i->device.startclip = swf_startclip;
    i->device.endclip = swf_endclip;
    i->device.fill = swf_fill;
    i->device.fillbitmap = swf_fillbitmap;
    i->device.fillgradient = swf_fillgradient;
    i->device.addfont = swf_addfont;
    i->device.drawchar = swf_drawchar;
    i->device.drawlink = swf_drawlink;

    return i;
};

static int id_error = 0;

static U16 getNewID(struct swfoutput* obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->currentswfid == 65535) {
	if(!id_error)
	    msg("<error> ID Table overflow");
	id_error=1;
	i->overflow = 1;
    }
    return ++i->currentswfid;
}
static U16 getNewDepth(struct swfoutput* obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->depth == 65535) {
	if(!id_error)
	    msg("<error> Depth Table overflow");
	id_error=1;
	i->overflow = 1;
    }
    return ++i->depth;
}

static void startshape(struct swfoutput* obj);
static void starttext(struct swfoutput* obj);
static void endshape(struct swfoutput* obj);
static void endtext(struct swfoutput* obj);

// matrix multiplication. changes p0
static void transform (plotxy*p0,struct swfmatrix*m)
{
    double x,y;
    x = m->m11*p0->x+m->m12*p0->y;
    y = m->m21*p0->x+m->m22*p0->y;
    p0->x = x + m->m31;
    p0->y = y + m->m32;
}

// write a move-to command into the swf
static int moveto(struct swfoutput*obj, TAG*tag, plotxy p0)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
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
static int moveto(struct swfoutput*obj, TAG*tag, double  x, double y)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    plotxy p;
    p.x = x;
    p.y = y;
    return moveto(obj, tag, p);
}
static void addPointToBBox(struct swfoutput*obj, int px, int py) 
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;

    SPOINT p;
    p.x = px;
    p.y = py;
    if(i->fill) {
	swf_ExpandRect(&i->bboxrect, p);
    } else {
	swf_ExpandRect3(&i->bboxrect, p, i->linewidth*3/2);
    }
}

/*static void plot(struct swfoutput*obj, int x, int y, TAG*tag)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
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

    addPointToBBox(obj, x-width ,y-width);
    addPointToBBox(obj, x+width ,y+width);
}*/

// write a line-to command into the swf
static void lineto(struct swfoutput*obj, TAG*tag, plotxy p0)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    int px = (int)(p0.x*20);
    int py = (int)(p0.y*20);
    int rx = (px-i->swflastx);
    int ry = (py-i->swflasty);
    if(rx|ry) {
	swf_ShapeSetLine (tag, i->shape, rx,ry);
	addPointToBBox(obj, i->swflastx,i->swflasty);
	addPointToBBox(obj, px,py);
    }/* else if(!i->fill) {
       // treat lines of length 0 as plots, making them
       // at least 1 twip wide so Flash will display them
	plot(obj, i->swflastx, i->swflasty, tag);
    }*/

    i->shapeisempty = 0;
    i->swflastx+=rx;
    i->swflasty+=ry;
}
static void lineto(struct swfoutput*obj, TAG*tag, double x, double y)
{
    plotxy p;
    p.x = x;
    p.y = y;
    lineto(obj,tag, p);
}

// write a spline-to command into the swf
static void splineto(struct swfoutput*obj, TAG*tag, plotxy control,plotxy end)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
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
    
    if(cx || cy || ex || ey) {
	swf_ShapeSetCurve(tag, i->shape, cx,cy,ex,ey);
	addPointToBBox(obj, lastlastx   ,lastlasty   );
	addPointToBBox(obj, lastlastx+cx,lastlasty+cy);
	addPointToBBox(obj, lastlastx+cx+ex,lastlasty+cy+ey);
    }/* else if(!i->fill) {
	// treat splines of length 0 as plots
	plot(obj, lastlastx, lastlasty, tag);
    }*/
    i->shapeisempty = 0;
}

/* write a line, given two points and the transformation
   matrix. */
/*static void line(struct swfoutput*obj, TAG*tag, plotxy p0, plotxy p1)
{
    moveto(obj, tag, p0);
    lineto(obj, tag, p1);
}*/

void resetdrawer(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    i->swflastx = 0;
    i->swflasty = 0;
}

static void stopFill(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->lastwasfill)
    {
	swf_ShapeSetStyle(i->tag,i->shape,i->linestyleid,0x8000,0);
	i->fillstylechanged = 1;
	i->lastwasfill = 0;
    }
}
static void startFill(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(!i->lastwasfill)
    {
	swf_ShapeSetStyle(i->tag,i->shape,0x8000,i->fillstyleid,0);
	i->fillstylechanged = 1;
	i->lastwasfill = 1;
    }
}

/* draw an outline. These are generated by pdf2swf and by t1lib
   (representing characters). */
/*void drawpath(struct swfoutput*obj, SWF_OUTLINE*outline, struct swfmatrix*m, int log)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if( i->tag->id != ST_DEFINESHAPE &&
        i->tag->id != ST_DEFINESHAPE2 &&
        i->tag->id != ST_DEFINESHAPE3)
    {
        msg("<error> internal error: drawpath needs a shape tag, not %d\n",i->tag->id);
        exit(1);
    }
    double x=0,y=0;
    double lastx=0,lasty=0;
    double firstx=0,firsty=0;
    int init=1;

    while (outline)
    {
        x += (outline->dest.x/(float)0xffff);
        y += (outline->dest.y/(float)0xffff);
        if(outline->type == SWF_PATHTYPE_MOVE)
        {
	    //if(!init && fill && obj->drawmode != DRAWMODE_EOFILL && !ignoredraworder)
	    if(config_filloverlap && !init && i->fill && obj->drawmode != DRAWMODE_EOFILL) {
		// drawmode=FILL (not EOFILL) means that
		// seperate shapes do not cancel each other out.
		// On SWF side, we need to start a new shape for each
		// closed polygon, because SWF only knows EOFILL.
		//
		endshape(obj);
		startshape(obj);
		startFill(obj);
	    }

            if(((int)(lastx*20) != (int)(firstx*20) ||
                (int)(lasty*20) != (int)(firsty*20)) &&
                     i->fill && !init)
            {
                plotxy p0;
                plotxy p1;
                p0.x=lastx;
                p0.y=lasty;
                p1.x=firstx;
                p1.y=firsty;
                if(log) printf("fix: %f,%f -> %f,%f\n",p0.x,p0.y,p1.x,p1.y);
                line(obj,i->tag, p0, p1);
            }
            firstx=x;
            firsty=y;
            init = 0;
        }
        else if(outline->type == SWF_PATHTYPE_LINE) 
        {
            plotxy p0;
            plotxy p1;
            p0.x=lastx;
            p0.y=lasty;
            p1.x=x;
            p1.y=y;
            if(log) printf("line: %f,%f -> %f,%f\n",p0.x,p0.y,p1.x,p1.y);
            line(obj,i->tag, p0,p1);
        }
        else {
	    msg("<error> drawpath: unknown outline type:%d\n", outline->type);
        }
        lastx=x;
        lasty=y;
        outline = outline->link;
    }
    if(((int)(lastx*20) != (int)(firstx*20) ||
        (int)(lasty*20) != (int)(firsty*20)) &&
             i->fill)
    {
        plotxy p0;
        plotxy p1;
        p0.x=lastx;
        p0.y=lasty;
        p1.x=firstx;
        p1.y=firsty;
        if(log) printf("fix: %f,%f -> %f,%f\n",p0.x,p0.y,p1.x,p1.y);
        line(obj, i->tag, p0, p1);
    }
}*/

static inline int colorcompare(struct swfoutput*obj, RGBA*a,RGBA*b)
{

    if(a->r!=b->r ||
       a->g!=b->g ||
       a->b!=b->b ||
       a->a!=b->a) {
        return 0;
    }
    return 1;
}

static SRECT getcharacterbbox(struct swfoutput*obj, SWFFONT*font)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
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

	/* divide by 1024, rounding up */
	b.xmin += 1023; b.ymin += 1023; b.xmax += 1023; b.ymax += 1023;
	b.xmin /= 1024; b.ymin /= 1024; b.xmax /= 1024; b.ymax /= 1024;

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

static void putcharacters(struct swfoutput*obj, TAG*tag)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
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
                    !colorcompare(obj,&color, &i->chardata[t].color) ||
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
                    if(!colorcompare(obj,&color, &i->chardata[t].color)) 
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

static void putcharacter(struct swfoutput*obj, int fontid, int charid, int x,int y, int size, RGBA color)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->chardatapos == CHARDATAMAX)
    {
	msg("<warning> Character buffer too small. SWF will be slightly bigger");
        endtext(obj);
        starttext(obj);
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

static int font_active = 0;
static char* font_active_filename = 0;

/* process a character. */
static int drawchar(struct swfoutput*obj, SWFFONT *swffont, char*character, int charnr, int u, swfmatrix*m, gfxcolor_t*col)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(!swffont) {
	msg("<warning> Font is NULL");
	return 0;
    }

    int charid = getCharID(swffont, charnr, character, u); 
    if(font_active) {
	char buf[1024];
	sprintf(buf, "%s.usage", font_active_filename);
	FILE*fi = fopen(buf, "ab+");
	if(fi) {
	     fprintf(fi, "%d %d %d %s\n", charnr, u, charid, character);
	     fclose(fi);
	} else 
	    msg("<error> Couldn't write to %s", buf);
    }
    
    if(charid<0) {
	msg("<warning> Didn't find character '%s' (c=%d,u=%d) in current charset (%s, %d characters)", 
		FIXNULL(character),charnr, u, FIXNULL((char*)swffont->name), swffont->numchars);
	return 0;
    }
    /*if(swffont->glyph[charid].shape->bitlen <= 16) {
	msg("<warning> Character '%s' (c=%d,u=%d), glyph %d in current charset (%s, %d characters) is empty", 
		FIXNULL(character),charnr, u, charid, FIXNULL((char*)swffont->name), swffont->numchars);
	return 0;
    }*/


    if(i->shapeid>=0)
	endshape(obj);
    if(i->textid<0)
	starttext(obj);

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
    putcharacter(obj, swffont->id, charid,p.x,p.y,FONT_INTERNAL_SIZE, rgba);
    swf_FontUseGlyph(swffont, charid);
    return 1;

    /*else
    {
        SWF_OUTLINE*outline = font->getOutline(character, charnr);
        char* charname = character;

        if(!outline) {
         msg("<warning> Didn't find character '%s' (%d) in current charset (%s)", 
                 FIXNULL(character),charnr,FIXNULL(font->getName()));
         return;
        }
        
        swfmatrix m2=*m;    
        m2.m11/=100;
        m2.m21/=100;
        m2.m12/=100;
        m2.m22/=100;

        if(textid>=0)
            endtext(obj);
        if(shapeid<0)
            startshape(obj);

	startFill(obj);

        int lf = fill;
        fill = 1;
        drawpath(tag, outline, &m2, 0);
        fill = lf;
    }*/
}

static void endtext(swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->textid<0)
        return;

    i->tag = swf_InsertTag(i->tag,ST_DEFINETEXT);
    swf_SetU16(i->tag, i->textid);

    SRECT r;
    r = getcharacterbbox(obj, obj->swffont);
    
    swf_SetRect(i->tag,&r);

    MATRIX m;
    swf_GetMatrix(0, &m); /* set unit matrix- the real matrix is in the placeobject */
    swf_SetMatrix(i->tag,&m);

    msg("<trace> Placing text (%d characters) as ID %d", i->chardatapos, i->textid);

    putcharacters(obj, i->tag);
    swf_SetU8(i->tag,0);
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    MATRIX m2;
    swf_MatrixJoin(&m2,&obj->fontmatrix, &i->page_matrix);

    swf_ObjectPlace(i->tag,i->textid,getNewDepth(obj),&m2,NULL,NULL);
    i->textid = -1;
}

int getCharID(SWFFONT *font, int charnr, char *charname, int u)
{
    int t;
    if(charname && font->glyphnames) {
	for(t=0;t<font->numchars;t++) {
	    if(font->glyphnames[t] && !strcmp(font->glyphnames[t],charname)) {
		msg("<debug> Char [%d,>%s<,%d] maps to %d\n", charnr, charname, u, t);
		return t;
	    }
	}
	/* if we didn't find the character, maybe
	   we can find the capitalized version */
	for(t=0;t<font->numchars;t++) {
	    if(font->glyphnames[t] && !strcasecmp(font->glyphnames[t],charname)) {
		msg("<debug> Char [%d,>>%s<<,%d] maps to %d\n", charnr, charname, u, t);
		return t;
	    }
	}
    }

    if(u>0 && font->encoding != 255) {
	/* try to use the unicode id */
	if(u>=0 && u<font->maxascii && font->ascii2glyph[u]>=0) {
	    msg("<debug> Char [%d,%s,>%d<] maps to %d\n", charnr, charname, u, font->ascii2glyph[u]);
	    return font->ascii2glyph[u];
	}
    }

    if(font->encoding != FONT_ENCODING_UNICODE) {
	/* the following only works if the font encoding
	   is US-ASCII based. It's needed for fonts which return broken unicode
	   indices */
	if(charnr>=0 && charnr<font->maxascii && font->ascii2glyph[charnr]>=0) {
	    msg("<debug> Char [>%d<,%s,%d] maps to %d\n", charnr, charname, u, font->ascii2glyph[charnr]);
	    return font->ascii2glyph[charnr];
	}
    } 
    
    if(charnr>=0 && charnr<font->numchars) {
	msg("<debug> Char [>%d<,%s,%d] maps to %d\n", charnr, charname, u, charnr);
	return charnr;
    }
    
    return -1;
}

/* set's the t1 font index of the font to use for swfoutput_drawchar(). */
void swfoutput_setfont(struct swfoutput*obj, char*fontid, char*filename)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    font_active = 0;
    fontlist_t*last=0,*iterator;
    if(!fontid) {
	msg("<error> No fontid");
	return;
    }

    if(obj->swffont && obj->swffont->name && !strcmp((char*)obj->swffont->name,fontid))
        return;

    /* TODO: remove the need for this (enhance getcharacterbbox so that it can cope
             with multiple fonts */
    endtext(obj);

    iterator = i->fontlist;
    while(iterator) {
        if(!strcmp((char*)iterator->swffont->name,fontid)) {
	    obj->swffont = iterator->swffont; 
	    return;
	}
        last = iterator;
        iterator = iterator->next;
    }

    if(!filename) {
	msg("<error> No filename given for font- internal error?");
	return;
    }

    swf_SetLoadFontParameters(64,/*skip unused*/0,/*full unicode*/1);
    SWFFONT*swffont = swf_LoadFont(filename);

    if(config_dumpfonts) {
	font_active = 1;
	font_active_filename = strdup(filename);
    }

    if(swffont == 0) {
	msg("<warning> Couldn't load font %s (%s)", fontid, filename);
	swffont = swf_LoadFont(0);
    }
    
    if(swffont->glyph2ascii) {
        int t;
        int bad = 0;
        /* check whether the Unicode indices look o.k.
           If they don't, disable the unicode lookup by setting
           the encoding to 255 */
        for(t=0;t<swffont->numchars;t++) {
            int c = swffont->glyph2ascii[t];
            if(c && c < 32 && swffont->glyph[t].shape->bitlen > 16) {
                // the character maps into the unicode control character range
                // between 0001-001f. Yet it is not empty. Treat the one
                // mapping as broken, and look how many of those we find.
                bad ++;
            }
        }
        if(bad>5) {
	    msg("<warning> Font %s has bad unicode mapping", fontid);
            swffont->encoding = 255;
        }
    }
    
    if(swffont->encoding != FONT_ENCODING_UNICODE && swffont->encoding != 255) {
	msg("<warning> Non-unicode mapping for font %s (%s)", fontid, filename);
    }

    swf_FontSetID(swffont, getNewID(obj));
    
    if(getScreenLogLevel() >= LOGLEVEL_DEBUG)  {
	// print font information
	msg("<debug> Font %s (%s)",fontid, filename);
	msg("<debug> |   ID: %d", swffont->id);
	msg("<debug> |   Version: %d", swffont->version);
	msg("<debug> |   Name: %s", swffont->name);
	msg("<debug> |   Numchars: %d", swffont->numchars);
	msg("<debug> |   Maxascii: %d", swffont->maxascii);
	msg("<debug> |   Style: %d", swffont->style);
	msg("<debug> |   Encoding: %d", swffont->encoding);
	for(int iii=0; iii<swffont->numchars;iii++) {
	    msg("<debug> |   Glyph %d) name=%s, unicode=%d size=%d bbox=(%.2f,%.2f,%.2f,%.2f)\n", iii, swffont->glyphnames?swffont->glyphnames[iii]:"<nonames>", swffont->glyph2ascii[iii], swffont->glyph[iii].shape->bitlen, 
		    swffont->layout->bounds[iii].xmin/20.0,
		    swffont->layout->bounds[iii].ymin/20.0,
		    swffont->layout->bounds[iii].xmax/20.0,
		    swffont->layout->bounds[iii].ymax/20.0
		    );
	    int t;
	    for(t=0;t<swffont->maxascii;t++) {
		if(swffont->ascii2glyph[t] == iii)
		    msg("<debug> | - maps to %d",t);
	    }
	}
    }

    /* set the font name to the ID we use here */
    if(swffont->name) free(swffont->name);
    swffont->name = (U8*)strdup(fontid);

    iterator = new fontlist_t;
    iterator->swffont = swffont;
    iterator->next = 0;

    if(last) 
        last->next = iterator;
    else 
        i->fontlist = iterator;

    obj->swffont = swffont; 
}

int swfoutput_queryfont(struct swfoutput*obj, char*fontid)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    fontlist_t *iterator = i->fontlist;
    while(iterator) {
        if(!strcmp((char*)iterator->swffont->name,fontid))
            return 1;
        iterator = iterator->next;
    }
    return 0;
}

/* set's the matrix which is to be applied to characters drawn by
   swfoutput_drawchar() */
void swfoutput_setfontmatrix(struct swfoutput*obj,double m11,double m21,
                                                  double m12,double m22)
{
    m11 *= 1024;
    m12 *= 1024;
    m21 *= 1024;
    m22 *= 1024;
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(obj->fontm11 == m11 &&
       obj->fontm12 == m12 &&
       obj->fontm21 == m21 &&
       obj->fontm22 == m22)
        return;
   if(i->textid>=0)
	endtext(obj);
    obj->fontm11 = m11;
    obj->fontm12 = m12;
    obj->fontm21 = m21;
    obj->fontm22 = m22;
    
    MATRIX m;
    m.sx = (U32)(((obj->fontm11)*65536)/FONT_INTERNAL_SIZE); m.r1 = (U32)(((obj->fontm12)*65536)/FONT_INTERNAL_SIZE);
    m.r0 = (U32)(((obj->fontm21)*65536)/FONT_INTERNAL_SIZE); m.sy = (U32)(((obj->fontm22)*65536)/FONT_INTERNAL_SIZE); 
    m.tx = 0;
    m.ty = 0;
    obj->fontmatrix = m;
}

/* draws a character at x,y. */
int swfoutput_drawchar(struct swfoutput* obj,double x,double y,char*character, int charnr, int u, gfxcolor_t* color) 
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    swfmatrix m;
    m.m11 = obj->fontm11;
    m.m12 = obj->fontm12;
    m.m21 = obj->fontm21;
    m.m22 = obj->fontm22;
    m.m31 = x;
    m.m32 = y;
    return drawchar(obj, obj->swffont, character, charnr, u, &m, color);
}

static void endpage(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->shapeid>=0)
      endshape(obj);
    if(i->textid>=0)
      endtext(obj);
    while(i->clippos)
        swfoutput_endclip(obj);
    i->pagefinished = 1;
}

void swfoutput_pagefeed(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    
    if(!i->pagefinished)
        endpage(obj);

    if(config_insertstoptag) {
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

static void setBackground(struct swfoutput*obj, int x1, int y1, int x2, int y2)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    RGBA rgb;
    rgb.a = rgb.r = rgb.g = rgb.b = 0xff;
    SRECT r;
    SHAPE* s;
    int ls1=0,fs1=0;
    int shapeid = getNewID(obj);
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
    swf_ObjectPlace(i->tag,shapeid,getNewDepth(obj),0,0,0);
    i->tag = swf_InsertTag(i->tag, ST_PLACEOBJECT2);
    swf_ObjectPlaceClip(i->tag,shapeid,getNewDepth(obj),0,0,0,65535);
    i->cliptag = i->tag;
}

void swfoutput_newpage(struct swfoutput*obj, int pageNum, int movex, int movey, int x1, int y1, int x2, int y2)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(!i->firstpage && !i->pagefinished)
        endpage(obj);

    swf_GetMatrix(0, &i->page_matrix);
    i->page_matrix.tx = movex*20;
    i->page_matrix.ty = movey*20;

    if(i->cliptag && i->frameno == i->lastframeno) {
        SWFPLACEOBJECT obj;
        swf_GetPlaceObject(i->cliptag, &obj);
        obj.clipdepth = i->depth;
        swf_ResetTag(i->cliptag, i->cliptag->id);
        swf_SetPlaceObject(i->cliptag, &obj);
        swf_PlaceObjectFree(&obj);
    }

    i->min_x = x1;
    i->min_y = y1;
    i->max_x = x2;
    i->max_y = y2;
    
    msg("<notice> processing page %d (%dx%d:%d:%d)", pageNum,x2-x1,y2-y1, x1, y1);

    x1*=20;y1*=20;x2*=20;y2*=20;

    /* set clipping/background rectangle */
    /* TODO: this should all be done in SWFOutputDev */
    setBackground(obj, x1, y1, x2, y2);

    /* increase SWF's bounding box */
    SRECT r;
    r.xmin = x1;
    r.ymin = y1;
    r.xmax = x2;
    r.ymax = y2;
    swf_ExpandRect2(&i->swf.movieSize, &r);

    i->lastframeno = i->frameno;
    i->firstpage = 0;
    i->pagefinished = 0;
}

/* initialize the swf writer */
void swfoutput_init(struct swfoutput* obj)
{
    memset(obj, 0, sizeof(struct swfoutput));
    obj->internal = init_internal_struct();
    ((swfoutput_internal*)obj->internal)->obj = obj;

    swfoutput_internal*i = (swfoutput_internal*)obj->internal;

    SRECT r;
    RGBA rgb;

    msg("<verbose> initializing swf output for size %d*%d\n", i->max_x,i->max_y);

    obj->swffont = 0;
    
    memset(&i->swf,0x00,sizeof(SWF));

    i->swf.fileVersion    = config_flashversion;
    i->swf.frameRate      = 0x0040; // 1 frame per 4 seconds
    i->swf.movieSize.xmin = 0;
    i->swf.movieSize.ymin = 0;
    i->swf.movieSize.xmax = 0;
    i->swf.movieSize.ymax = 0;
    
    i->swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    i->tag = i->swf.firstTag;
    rgb.a = rgb.r = rgb.g = rgb.b = 0xff;
    swf_SetRGB(i->tag,&rgb);

    i->startdepth = i->depth = 0;
    
    if(config_protect)
      i->tag = swf_InsertTag(i->tag, ST_PROTECT);
}

static void startshape(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    SRECT r;

    if(i->shapeid>=0)
	return;

    if(i->textid>=0)
        endtext(obj);

    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE);

    swf_ShapeNew(&i->shape);
    i->linestyleid = swf_ShapeAddLineStyle(i->shape,i->linewidth,&obj->strokergb);
    i->fillstyleid = swf_ShapeAddSolidFillStyle(i->shape,&obj->fillrgb);

    i->shapeid = getNewID(obj);
    
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
    swf_ShapeSetAll(i->tag,i->shape,/*x*/0,/*y*/0,i->linestyleid,0,0);
    i->swflastx=i->swflasty=0;
    i->lastwasfill = 0;
    i->shapeisempty = 1;
}

static void starttext(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->shapeid>=0)
        endshape(obj);
      
    i->textid = getNewID(obj);

    i->swflastx=i->swflasty=0;
}
	    

/* TODO: move to ../lib/rfxswf */
void changeRect(struct swfoutput*obj, TAG*tag, int pos, SRECT*newrect)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
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

void cancelshape(swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    /* delete old shape tag */
    TAG*todel = i->tag;
    i->tag = i->tag->prev;
    swf_DeleteTag(todel);
    if(i->shape) {swf_ShapeFree(i->shape);i->shape=0;}
    i->shapeid = -1;
    i->bboxrectpos = -1;
}

void fixAreas(swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(!i->shapeisempty && i->fill &&
       (i->bboxrect.xmin == i->bboxrect.xmax ||
        i->bboxrect.ymin == i->bboxrect.ymax) &&
        config_minlinewidth >= 0.001
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

	cancelshape(obj);

	RGBA save_col = obj->strokergb;
	int  save_width = i->linewidth;

	obj->strokergb = obj->fillrgb;
	i->linewidth = (int)(config_minlinewidth*20);
	if(i->linewidth==0) i->linewidth = 1;
	
	startshape(obj);

	moveto(obj, i->tag, r.xmin/20.0,r.ymin/20.0);
	lineto(obj, i->tag, r.xmax/20.0,r.ymax/20.0);

	obj->strokergb = save_col;
	i->linewidth = save_width;
    }
    
}

static void endshape_noput(swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->shapeid<0) 
        return;
    //changeRect(obj, i->tag, i->bboxrectpos, &i->bboxrect);
    i->shapeid = -1;
    if(i->shape) {
        swf_ShapeFree(i->shape);
        i->shape=0;
    }
    i->fill=0;
}

static void endshape(swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->shapeid<0) 
        return;

    fixAreas(obj);
	
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
	cancelshape(obj);
	return;
    }
    
    swf_ShapeSetEnd(i->tag);

    changeRect(obj, i->tag, i->bboxrectpos, &i->bboxrect);

    msg("<trace> Placing shape id %d", i->shapeid);

    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    swf_ObjectPlace(i->tag,i->shapeid,getNewDepth(obj),&i->page_matrix,NULL,NULL);

    swf_ShapeFree(i->shape);
    i->shape = 0;
    i->shapeid = -1;
    i->bboxrectpos = -1;
    i->fill=0;

    /*int debug = 1;
    if(debug) {
	char text[80];
	sprintf(text, "id%d", i->shapeid);
	swfcoord points[4];
	points[0].x = i->bboxrect.xmin;
	points[0].y = i->bboxrect.ymin;
	points[1].x = i->bboxrect.xmax;
	points[1].y = i->bboxrect.ymin;
	points[2].x = i->bboxrect.xmax;
	points[2].y = i->bboxrect.ymax;
	points[3].x = i->bboxrect.xmin;
	points[3].y = i->bboxrect.ymax;
	swfoutput_namedlink(obj,text,points);
    }*/
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


void swfoutput_finalize(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;

    if(i->tag && i->tag->id == ST_END)
        return; //already done

    if(config_bboxvars) {
	TAG* tag = swf_InsertTag(i->swf.firstTag, ST_DOACTION);
	ActionTAG*a = 0;
	a = action_PushString(a, "xmin");
	a = action_PushFloat(a, i->swf.movieSize.xmin / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "ymin");
	a = action_PushFloat(a, i->swf.movieSize.ymin / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "xmax");
	a = action_PushFloat(a, i->swf.movieSize.xmax / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "ymax");
	a = action_PushFloat(a, i->swf.movieSize.ymax / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "width");
	a = action_PushFloat(a, (i->swf.movieSize.xmax - i->swf.movieSize.xmin) / 20.0);
	a = action_SetVariable(a);
	a = action_PushString(a, "height");
	a = action_PushFloat(a, (i->swf.movieSize.ymax - i->swf.movieSize.ymin) / 20.0);
	a = action_SetVariable(a);
	a = action_End(a);
	swf_ActionSet(tag, a);
	swf_ActionFree(a);
    }

    if(i->frameno == i->lastframeno) // fix: add missing pagefeed
        swfoutput_pagefeed(obj);

    endpage(obj);
    fontlist_t *tmp,*iterator = i->fontlist;
    while(iterator) {
	TAG*mtag = i->swf.firstTag;
	if(iterator->swffont) {
	    mtag = swf_InsertTag(mtag, ST_DEFINEFONT2);
	    if(!config_storeallcharacters)
		swf_FontReduce(iterator->swffont);
	    swf_FontSetDefine2(mtag, iterator->swffont);
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
	wipeSWF(&i->swf);
    }
}

SWF* swfoutput_get(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;

    swfoutput_finalize(obj);

    return swf_CopySWF(&i->swf);
}

void swfoutput_getdimensions(struct swfoutput*obj, int*x1, int*y1, int*x2, int*y2)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(x1) *x1 = i->swf.movieSize.xmin/20;
    if(y1) *y1 = i->swf.movieSize.ymin/20;
    if(x2) *x2 = i->swf.movieSize.xmax/20;
    if(y2) *y2 = i->swf.movieSize.ymax/20;
}

int swfoutput_save(struct swfoutput* obj, char*filename) 
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    swfoutput_finalize(obj);

    int fi;
    if(filename)
     fi = open(filename, O_BINARY|O_CREAT|O_TRUNC|O_WRONLY, 0777);
    else
     fi = 1; // stdout
    
    if(fi<=0) {
     msg("<fatal> Could not create \"%s\". ", FIXNULL(filename));
     return 0;
    }
    
    if(config_enablezlib || config_flashversion>=6) {
      if FAILED(swf_WriteSWC(fi,&i->swf)) 
       msg("<error> WriteSWC() failed.\n");
    } else {
      if FAILED(swf_WriteSWF(fi,&i->swf)) 
       msg("<error> WriteSWF() failed.\n");
    }

    if(filename)
     close(fi);
    return 1;
}

/* Perform cleaning up, complete the swf, and write it out. */
void swfoutput_destroy(struct swfoutput* obj) 
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
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
        delete tmp;
    }
    swf_FreeTags(&i->swf);

    free(i);i=0;
    memset(obj, 0, sizeof(swfoutput));
}

static void swfoutput_setfillcolor(swfoutput* obj, U8 r, U8 g, U8 b, U8 a)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(obj->fillrgb.r == r &&
       obj->fillrgb.g == g &&
       obj->fillrgb.b == b &&
       obj->fillrgb.a == a) return;
    if(i->shapeid>=0)
     endshape(obj);

    obj->fillrgb.r = r;
    obj->fillrgb.g = g;
    obj->fillrgb.b = b;
    obj->fillrgb.a = a;
}

static void swfoutput_setstrokecolor(swfoutput* obj, U8 r, U8 g, U8 b, U8 a)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(obj->strokergb.r == r &&
       obj->strokergb.g == g &&
       obj->strokergb.b == b &&
       obj->strokergb.a == a) return;

    if(i->shapeid>=0)
     endshape(obj);
    obj->strokergb.r = r;
    obj->strokergb.g = g;
    obj->strokergb.b = b;
    obj->strokergb.a = a;
}

static void swfoutput_setlinewidth(struct swfoutput*obj, double _linewidth)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->linewidth == (U16)(_linewidth*20))
        return;

    if(i->shapeid>=0)
	endshape(obj);
    i->linewidth = (U16)(_linewidth*20);
}


static void drawlink(struct swfoutput*obj, ActionTAG*,ActionTAG*, swfcoord*points, char mouseover);

void swfoutput_linktourl(struct swfoutput*obj, char*url, swfcoord*points)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    ActionTAG* actions;
    if(!strncmp("http://pdf2swf:", url, 15)) {
     char*tmp = strdup(url);
     int l = strlen(tmp);
     if(tmp[l-1] == '/')
        tmp[l-1] = 0;
     swfoutput_namedlink(obj, tmp+15, points);
     free(tmp);
     return;
    }
    
    if(i->shapeid>=0)
	endshape(obj);
    if(i->textid>=0)
	endtext(obj);
    
    if(!config_opennewwindow)
      actions = action_GetUrl(0, url, "_parent");
    else
      actions = action_GetUrl(0, url, "_this");
    actions = action_End(actions);
    
    drawlink(obj, actions, 0, points,0);
}
void swfoutput_linktopage(struct swfoutput*obj, int page, swfcoord*points)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    ActionTAG* actions;

    if(i->shapeid>=0)
	endshape(obj);
    if(i->textid>=0)
	endtext(obj);
   
      actions = action_GotoFrame(0, page);
      actions = action_End(actions);

    drawlink(obj, actions, 0, points,0);
}

/* Named Links (a.k.a. Acrobatmenu) are used to implement various gadgets
   of the viewer objects, like subtitles, index elements etc.
*/
void swfoutput_namedlink(struct swfoutput*obj, char*name, swfcoord*points)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    ActionTAG *actions1,*actions2;
    char*tmp = strdup(name);
    char mouseover = 1;

    if(i->shapeid>=0)
	endshape(obj);
    if(i->textid>=0)
	endtext(obj);

    if(!strncmp(tmp, "call:", 5))
    {
	char*x = strchr(&tmp[5], ':');
	if(!x) {
	    actions1 = action_PushInt(0, 0); //number of parameters (0)
	    actions1 = action_PushString(actions1, &tmp[5]); //function name
	    actions1 = action_CallFunction(actions1);
	} else {
	    *x = 0;
	    actions1 = action_PushString(0, x+1); //parameter
	    actions1 = action_PushInt(actions1, 1); //number of parameters (1)
	    actions1 = action_PushString(actions1, &tmp[5]); //function name
	    actions1 = action_CallFunction(actions1);
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

    drawlink(obj, actions1, actions2, points,mouseover);

    swf_ActionFree(actions1);
    swf_ActionFree(actions2);
    free(tmp);
}

static void drawlink(struct swfoutput*obj, ActionTAG*actions1, ActionTAG*actions2, swfcoord*points, char mouseover)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    RGBA rgb;
    SRECT r;
    int lsid=0;
    int fsid;
    struct plotxy p1,p2,p3,p4;
    int myshapeid;
    int myshapeid2;
    double xmin,ymin;
    double xmax=xmin=points[0].x,ymax=ymin=points[0].y;
    double posx = 0;
    double posy = 0;
    int t;
    int buttonid = getNewID(obj);
    for(t=1;t<4;t++)
    {
        if(points[t].x>xmax) xmax=points[t].x;
        if(points[t].y>ymax) ymax=points[t].y;
        if(points[t].x<xmin) xmin=points[t].x;
        if(points[t].y<ymin) ymin=points[t].y;
    }

    p1.x=points[0].x; p1.y=points[0].y; p2.x=points[1].x; p2.y=points[1].y; 
    p3.x=points[2].x; p3.y=points[2].y; p4.x=points[3].x; p4.y=points[3].y;
   
    /* the following code subtracts the upper left edge from all coordinates,
       and set's posx,posy so that ST_PLACEOBJECT is used with a matrix.
       Necessary for preprocessing with swfcombine. */
    posx = xmin; posy = ymin;
    p1.x-=posx;p2.x-=posx;p3.x-=posx;p4.x-=posx;
    p1.y-=posy;p2.y-=posy;p3.y-=posy;p4.y-=posy;
    xmin -= posx; ymin -= posy;
    xmax -= posx; ymax -= posy;
    
    /* shape */
    myshapeid = getNewID(obj);
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE3);
    swf_ShapeNew(&i->shape);
    rgb.r = rgb.b = rgb.a = rgb.g = 0; 
    fsid = swf_ShapeAddSolidFillStyle(i->shape,&rgb);
    swf_SetU16(i->tag, myshapeid);
    r.xmin = (int)(xmin*20);
    r.ymin = (int)(ymin*20);
    r.xmax = (int)(xmax*20);
    r.ymax = (int)(ymax*20);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,i->shape);
    swf_ShapeCountBits(i->shape,NULL,NULL);
    swf_SetShapeBits(i->tag,i->shape);
    swf_ShapeSetAll(i->tag,i->shape,/*x*/0,/*y*/0,0,fsid,0);
    i->swflastx = i->swflasty = 0;
    moveto(obj, i->tag, p1);
    lineto(obj, i->tag, p2);
    lineto(obj, i->tag, p3);
    lineto(obj, i->tag, p4);
    lineto(obj, i->tag, p1);
    swf_ShapeSetEnd(i->tag);

    /* shape2 */
    myshapeid2 = getNewID(obj);
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE3);
    swf_ShapeNew(&i->shape);
    rgb.r = rgb.b = rgb.a = rgb.g = 255;
    rgb.a = 40;
    fsid = swf_ShapeAddSolidFillStyle(i->shape,&rgb);
    swf_SetU16(i->tag, myshapeid2);
    r.xmin = (int)(xmin*20);
    r.ymin = (int)(ymin*20);
    r.xmax = (int)(xmax*20);
    r.ymax = (int)(ymax*20);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,i->shape);
    swf_ShapeCountBits(i->shape,NULL,NULL);
    swf_SetShapeBits(i->tag,i->shape);
    swf_ShapeSetAll(i->tag,i->shape,/*x*/0,/*y*/0,0,fsid,0);
    i->swflastx = i->swflasty = 0;
    moveto(obj, i->tag, p1);
    lineto(obj, i->tag, p2);
    lineto(obj, i->tag, p3);
    lineto(obj, i->tag, p4);
    lineto(obj, i->tag, p1);
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
	swf_ObjectPlace(i->tag, buttonid, getNewDepth(obj),&m,0,0);
    } else {
	swf_ObjectPlace(i->tag, buttonid, getNewDepth(obj),&i->page_matrix,0,0);
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

static void drawgfxline(struct swfoutput*obj, gfxline_t*line)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    gfxcoord_t lastx=0,lasty=0,px=0,py=0;
    char lastwasmoveto;
    while(1) {
	if(!line)
	    break;
	/* check whether the next segment is zero */
	if(line->type == gfx_moveTo) {
	    msg("<trace> ======== moveTo %.2f %.2f", line->x, line->y);
	    moveto(obj, i->tag, line->x, line->y);
	    px = lastx = line->x;
	    py = lasty = line->y;
	    lastwasmoveto = 1;
	} if(line->type == gfx_lineTo) {
	    msg("<trace> ======== lineTo %.2f %.2f", line->x, line->y);
	    lineto(obj, i->tag, line->x, line->y);
	    px = line->x;
	    py = line->y;
	    lastwasmoveto = 0;
	} else if(line->type == gfx_splineTo) {
	    msg("<trace> ======== splineTo  %.2f %.2f", line->x, line->y);
	    plotxy s,p;
	    s.x = line->sx;p.x = line->x;
	    s.y = line->sy;p.y = line->y;
	    splineto(obj, i->tag, s, p);
	    px = line->x;
	    py = line->y;
	    lastwasmoveto = 0;
	}
	line = line->next;
    }
}

void swfoutput_drawgfxline(struct swfoutput*obj, gfxline_t*line, gfxcoord_t width, gfxcolor_t*col, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    gfxdevice_t*dev = &i->device;
    dev->stroke(dev, line, width, col, cap_style, joint_style, miterLimit);
}
void swfoutput_fillgfxline(struct swfoutput*obj, gfxline_t*line, gfxcolor_t*col)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    gfxdevice_t*dev = &i->device;
    dev->fill(dev, line, col);
}
void swfoutput_startclip(struct swfoutput*obj, gfxline_t*line)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    gfxdevice_t*dev = &i->device;
    dev->startclip(dev, line);
}
void swfoutput_endclip(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    gfxdevice_t*dev = &i->device;
    dev->endclip(dev);
}
void swfoutput_gfxaddfont(struct swfoutput*obj, char*fontid, gfxfont_t*font)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    gfxdevice_t*dev = &i->device;
    dev->addfont(dev, fontid, font);
}
void swfoutput_gfxdrawchar(struct swfoutput*obj, char*fontid, int glyph, gfxcolor_t*c, gfxmatrix_t*m)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    gfxdevice_t*dev = &i->device;
    dev->drawchar(dev, fontid, glyph, c, m);
}

#define IMAGE_TYPE_JPEG 0
#define IMAGE_TYPE_LOSSLESS 1

static void swfoutput_drawimage(struct swfoutput*obj, RGBA* data, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4, int type)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;

    RGBA*newpic=0;
    
    double l1 = sqrt((x4-x1)*(x4-x1) + (y4-y1)*(y4-y1));
    double l2 = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
   
    gfxline_t p1,p2,p3,p4,p5;
    p1.type=gfx_moveTo;p1.x=x1; p1.y=y1;p1.next=&p2;
    p2.type=gfx_lineTo;p2.x=x2; p2.y=y2;p2.next=&p3;
    p3.type=gfx_lineTo;p3.x=x3; p3.y=y3;p3.next=&p4;
    p4.type=gfx_lineTo;p4.x=x4; p4.y=y4;p4.next=&p5;
    p5.type=gfx_lineTo;p5.x=x1; p5.y=y1;p5.next=0;

    {p1.x = (int)(p1.x*20)/20.0;
     p1.y = (int)(p1.y*20)/20.0;
     p2.x = (int)(p2.x*20)/20.0;
     p2.y = (int)(p2.y*20)/20.0;
     p3.x = (int)(p3.x*20)/20.0;
     p3.y = (int)(p3.y*20)/20.0;
     p4.x = (int)(p4.x*20)/20.0;
     p4.y = (int)(p4.y*20)/20.0;
     p5.x = (int)(p5.x*20)/20.0;
     p5.y = (int)(p5.y*20)/20.0;
    }
    
    float m00,m10,tx;
    float m01,m11,ty;
    
    gfxmatrix_t m;
    m.m00 = (p4.x-p1.x)/sizex; m.m10 = (p2.x-p1.x)/sizey;
    m.m01 = (p4.y-p1.y)/sizex; m.m11 = (p2.y-p1.y)/sizey;
    m.tx = p1.x - 0.5;
    m.ty = p1.y - 0.5;

    gfximage_t img;
    img.data = (gfxcolor_t*)data;
    img.width = sizex;
    img.height = sizey;
  
    if(type == IMAGE_TYPE_JPEG)
	/* TODO: pass image_dpi to device instead */
	i->device.setparameter(&i->device, "next_bitmap_is_jpeg", "1");

    i->device.fillbitmap(&i->device, &p1, &img, &m, 0);
}

void swfoutput_drawimagejpeg(struct swfoutput*obj, RGBA*mem, int sizex,int sizey, 
        double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4)
{
    swfoutput_drawimage(obj,mem,sizex,sizey,x1,y1,x2,y2,x3,y3,x4,y4, IMAGE_TYPE_JPEG);
}

void swfoutput_drawimagelossless(struct swfoutput*obj, RGBA*mem, int sizex,int sizey, 
        double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4)
{
    swfoutput_drawimage(obj,mem,sizex,sizey,x1,y1,x2,y2,x3,y3,x4,y4, IMAGE_TYPE_LOSSLESS);
}

void swfoutput_setparameter(char*name, char*value)
{
    if(!strcmp(name, "jpegsubpixels")) {
	config_jpegsubpixels = atof(value);
    } else if(!strcmp(name, "ppmsubpixels")) {
	config_ppmsubpixels = atof(value);
    } else if(!strcmp(name, "drawonlyshapes")) {
	config_drawonlyshapes = atoi(value);
    } else if(!strcmp(name, "ignoredraworder")) {
	config_ignoredraworder = atoi(value);
    } else if(!strcmp(name, "filloverlap")) {
	config_filloverlap = atoi(value);
    } else if(!strcmp(name, "linksopennewwindow")) {
	config_opennewwindow = atoi(value);
    } else if(!strcmp(name, "opennewwindow")) {
	config_opennewwindow = atoi(value);
    } else if(!strcmp(name, "storeallcharacters")) {
	config_storeallcharacters = atoi(value);
    } else if(!strcmp(name, "enablezlib")) {
	config_enablezlib = atoi(value);
    } else if(!strcmp(name, "bboxvars")) {
	config_bboxvars = atoi(value);
    } else if(!strcmp(name, "insertstop")) {
	config_insertstoptag = atoi(value);
    } else if(!strcmp(name, "protected")) {
	config_protect = atoi(value);
    } else if(!strcmp(name, "flashversion")) {
	config_flashversion = atoi(value);
    } else if(!strcmp(name, "minlinewidth")) {
	config_minlinewidth = atof(value);
    } else if(!strcmp(name, "caplinewidth")) {
	config_caplinewidth = atof(value);
    } else if(!strcmp(name, "dumpfonts")) {
	config_dumpfonts = atoi(value);
    } else if(!strcmp(name, "jpegquality")) {
	int val = atoi(value);
	if(val<0) val=0;
	if(val>100) val=100;
	config_jpegquality = val;
    } else if(!strcmp(name, "splinequality")) {
	int v = atoi(value);
	v = 500-(v*5); // 100% = 0.25 pixel, 0% = 25 pixel
	if(v<1) v = 1;
	config_splinemaxerror = v;
    } else if(!strcmp(name, "fontquality")) {
	int v = atoi(value);
	v = 500-(v*5); // 100% = 0.25 pixel, 0% = 25 pixel
	if(v<1) v = 1;
	config_fontsplinemaxerror = v;
    } else {
	fprintf(stderr, "unknown parameter: %s (=%s)\n", name, value);
    }
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
    cx.a1 = c->t.a;
    cx.r1 = c->t.r;
    cx.g1 = c->t.g;
    cx.b1 = c->t.b;
    return cx;
}

ArtSVP* gfxstrokeToSVP(gfxline_t*line, gfxcoord_t width, gfx_capType cap_style, gfx_joinType joint_style, double miterLimit)
{
    ArtVpath *vec = NULL;
    ArtSVP *svp = NULL;
    int pos=0,len=0;
    gfxline_t*l2;
    double x=0,y=0;

    l2 = line;
    while(l2) {
	if(l2->type == gfx_moveTo) {
	    pos ++;
	} if(l2->type == gfx_lineTo) {
	    pos ++;
	} if(l2->type == gfx_splineTo) {
            int parts = (int)(sqrt(fabs(l2->x-2*l2->sx+x) + fabs(l2->y-2*l2->sy+y))/3);
            if(!parts) parts = 1;
            pos += parts + 1;
	}
	x = l2->x;
	y = l2->y;
	l2 = l2->next;
    }
    pos++;
    len = pos;

    vec = art_new (ArtVpath, len);

    pos = 0;
    l2 = line;
    while(l2) {
	if(l2->type == gfx_moveTo) {
	    vec[pos].code = ART_MOVETO;
	    vec[pos].x = l2->x;
	    vec[pos].y = l2->y;
	    pos++; 
	    assert(pos<=len);
	} else if(l2->type == gfx_lineTo) {
	    vec[pos].code = ART_LINETO;
	    vec[pos].x = l2->x;
	    vec[pos].y = l2->y;
	    pos++; 
	    assert(pos<=len);
	} else if(l2->type == gfx_splineTo) {
	    int i;
            int parts = (int)(sqrt(fabs(l2->x-2*l2->sx+x) + fabs(l2->y-2*l2->sy+y))/3);
            if(!parts) parts = 1;
	    for(i=0;i<=parts;i++) {
		double t = (double)i/(double)parts;
		vec[pos].code = ART_LINETO;
		vec[pos].x = l2->x*t*t + 2*l2->sx*t*(1-t) + x*(1-t)*(1-t);
		vec[pos].y = l2->y*t*t + 2*l2->sy*t*(1-t) + y*(1-t)*(1-t);
		pos++;
		assert(pos<=len);
	    }
	}
	x = l2->x;
	y = l2->y;
	l2 = l2->next;
    }
    vec[pos].code = ART_END;
			
    svp = art_svp_vpath_stroke (vec,
			(joint_style==gfx_joinMiter)?ART_PATH_STROKE_JOIN_MITER:
			((joint_style==gfx_joinRound)?ART_PATH_STROKE_JOIN_ROUND:
			 ((joint_style==gfx_joinBevel)?ART_PATH_STROKE_JOIN_BEVEL:ART_PATH_STROKE_JOIN_BEVEL)),
			(cap_style==gfx_capButt)?ART_PATH_STROKE_CAP_BUTT:
			((cap_style==gfx_capRound)?ART_PATH_STROKE_CAP_ROUND:
			 ((cap_style==gfx_capSquare)?ART_PATH_STROKE_CAP_SQUARE:ART_PATH_STROKE_CAP_SQUARE)),
			width, //line_width
			miterLimit, //miter_limit
			0.05 //flatness
			);
    free(vec);
    return svp;
}

gfxline_t* SVPtogfxline(ArtSVP*svp)
{
    int size = 0;
    int t;
    int pos = 0;
    for(t=0;t<svp->n_segs;t++) {
	size += svp->segs[t].n_points + 1;
    }
    gfxline_t* lines = (gfxline_t*)rfx_alloc(sizeof(gfxline_t)*size);

    for(t=0;t<svp->n_segs;t++) {
	ArtSVPSeg* seg = &svp->segs[t];
	int p;
	for(p=0;p<seg->n_points;p++) {
	    lines[pos].type = p==0?gfx_moveTo:gfx_lineTo;
	    ArtPoint* point = &seg->points[p];
	    lines[pos].x = point->x;
	    lines[pos].y = point->y;
	    lines[pos].next = &lines[pos+1];
	    pos++;
	}
    }
    if(pos) {
	lines[pos-1].next = 0;
	return lines;
    } else {
	return 0;
    }
}

/* TODO */
static int imageInCache(swfoutput*obj, void*data, int width, int height)
{
    return -1;
}
static void addImageToCache(swfoutput*obj, void*data, int width, int height)
{
}
    
static int add_image(swfoutput_internal*i, gfximage_t*img, int targetwidth, int targetheight, int* newwidth, int* newheight)
{
    swfoutput*obj = i->obj;
    RGBA*newpic = 0;
    RGBA*mem = (RGBA*)img->data;
    
    int sizex = img->width;
    int sizey = img->height;
    int is_jpeg = i->jpeg;
    i->jpeg = 0;

    int newsizex=sizex, newsizey=sizey;

    /// {
    if(is_jpeg && config_jpegsubpixels) {
	newsizex = (int)(targetwidth*config_jpegsubpixels+0.5);
	newsizey = (int)(targetheight*config_jpegsubpixels+0.5);
    } else if(!is_jpeg && config_ppmsubpixels) {
	newsizex = (int)(targetwidth*config_ppmsubpixels+0.5);
	newsizey = (int)(targetheight*config_ppmsubpixels+0.5);
    }
    /// }

    if(sizex<=0 || sizey<=0 || newsizex<=0 || newsizey<=0)
	return -1;

    /* TODO: cache images */
    *newwidth = sizex;
    *newheight  = sizey;
    
    if(newsizex<sizex || newsizey<sizey) {
	msg("<verbose> Scaling %dx%d image to %dx%d", sizex, sizey, newsizex, newsizey);
	newpic = swf_ImageScale(mem, sizex, sizey, newsizex, newsizey);
	*newwidth = sizex = newsizex;
	*newheight  = sizey = newsizey;
	mem = newpic;
    
    }

    int num_colors = swf_ImageGetNumberOfPaletteEntries(mem,sizex,sizey,0);
    int has_alpha = swf_ImageHasAlpha(mem,sizex,sizey);
    
    msg("<verbose> Drawing %dx%d %s%simage at size %dx%d (%dx%d), %s%d colors",
	    sizex, sizey, 
	    has_alpha?(has_alpha==2?"semi-transparent ":"transparent "):"", 
	    is_jpeg?"jpeg-":"",
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
    int cacheid = imageInCache(obj, mem, sizex, sizey);

    if(cacheid<=0) {
	bitid = getNewID(obj);
	i->tag = swf_AddImage(i->tag, bitid, mem, sizex, sizey, config_jpegquality);
	addImageToCache(obj, mem, sizex, sizey);
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

void swf_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    swfoutput*obj = i->obj;

    endshape(obj);
    endtext(obj);

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
    int myshapeid = getNewID(obj);
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
    drawgfxline(obj, line);
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(shape);

    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    CXFORM cxform2 = gfxcxform_to_cxform(cxform);
    swf_ObjectPlace(i->tag,myshapeid,getNewDepth(obj),&i->page_matrix,&cxform2,NULL);
}

void swf_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    swfoutput*obj = i->obj;

    endtext(obj);
    endshape(obj);

    if(i->clippos >= 127)
    {
        msg("<warning> Too many clip levels.");
        i->clippos --;
    } 

    int myshapeid = getNewID(obj);
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE);
    RGBA col;
    memset(&col, 0, sizeof(RGBA));
    SHAPE*shape;
    swf_ShapeNew(&shape);
    int fsid = swf_ShapeAddSolidFillStyle(shape,&col);
    swf_SetU16(i->tag,myshapeid);
    SRECT r = gfxline_getSWFbbox(line);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(i->tag,shape);
    swf_ShapeSetAll(i->tag,shape,UNDEFINED_COORD,UNDEFINED_COORD,0,fsid,0);
    i->swflastx = i->swflasty = UNDEFINED_COORD;
    drawgfxline(obj, line);
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(shape);

    /* TODO: remember the bbox, and check all shapes against it */
    
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    i->cliptags[i->clippos] = i->tag;
    i->clipshapes[i->clippos] = myshapeid;
    i->clipdepths[i->clippos] = getNewDepth(obj);
    i->clippos++;
}

void swf_endclip(gfxdevice_t*dev)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    swfoutput*obj = i->obj;
    if(i->textid>=0)
	endtext(obj);
    if(i->shapeid>=0)
	endshape(obj);

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
int swf_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    if(!strcmp(key, "next_bitmap_is_jpeg")) {
	((swfoutput_internal*)dev->internal)->jpeg = 1;
	return 1;
    }
    return 0;
}

int gfxline_type(gfxline_t*line)
{
    int tmplines=0;
    int tmpsplines=0;
    int lines=0;
    int splines=0;
    int haszerosegments=0;
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
	line = line->next;
    }
    if(lines==0 && splines==0) return 0;
    else if(lines==1 && splines==0) return 1;
    else if(lines==0 && splines==1) return 2;
    else if(splines==0) return 3;
    else return 4;
}

int gfxline_has_dots(gfxline_t*line)
{
    int tmplines=0;
    double x,y;
    double dist = 0;
    int isline = 0;
    while(line) {
	if(line->type == gfx_moveTo) {
	    if(isline && dist < 1) {
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
    if(isline && dist < 1) {
	return 1;
    }
    return 0;
}

int gfxline_fix_short_edges(gfxline_t*line)
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

int shapenr = 0;

void swf_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    swfoutput*obj = i->obj;
    int type = gfxline_type(line);
    int has_dots = gfxline_has_dots(line);

    /* TODO: * split line into segments, and perform this check for all segments */
    if(!has_dots &&
       (width <= config_caplinewidth 
        || (cap_style == gfx_capRound && joint_style == gfx_joinRound)
        || (cap_style == gfx_capRound && type<=2))) {
	msg("<trace> draw as stroke, type=%d dots=%d", type, has_dots);
	endtext(obj);
	swfoutput_setstrokecolor(obj, color->r, color->g, color->b, color->a);
	swfoutput_setlinewidth(obj, width);
	startshape(obj);
	stopFill(obj);
	drawgfxline(obj, line);
    } else {
	msg("<trace> draw as polygon, type=%d dots=%d", type, has_dots);
	if(has_dots)
	    gfxline_fix_short_edges(line);
	/* we need to convert the line into a polygon */
	ArtSVP* svp = gfxstrokeToSVP(line, width, cap_style, joint_style, miterLimit);
	gfxline_t*gfxline = SVPtogfxline(svp);
	dev->fill(dev, gfxline, color);
	free(gfxline);
	art_svp_free(svp);
    }
}
void swf_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    swfoutput*obj = i->obj;
    endtext(obj);
    if(!config_ignoredraworder)
	endshape(obj);
    swfoutput_setfillcolor(obj, color->r, color->g, color->b, color->a);
    startshape(obj);
    startFill(obj);
    i->fill=1;
    drawgfxline(obj, line);
    msg("<trace> end of swf_fill (shapeid=%d)", i->shapeid);
}
void swf_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    msg("<error> Gradient filling not implemented yet");
}

void swf_drawlink(gfxdevice_t*dev, gfxline_t*line, char*action)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    swfoutput*obj = i->obj;

    endshape(obj);
    endtext(obj);

    /* shape */
    int myshapeid = getNewID(obj);
    RGBA black;
    black.r = black.g = black.b = black.a = 0;
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE);
    SHAPE*shape;
    swf_ShapeNew(&shape);
    int fsid = swf_ShapeAddSolidFillStyle(shape,&black);
    swf_SetU16(i->tag, myshapeid);
    SRECT r = gfxline_getSWFbbox(line);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(i->tag,shape);
    swf_ShapeSetAll(i->tag,shape,UNDEFINED_COORD,UNDEFINED_COORD,0,fsid,0);
    i->swflastx = i->swflasty = UNDEFINED_COORD;
    drawgfxline(obj, line);
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(shape);

    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    swf_ObjectPlace(i->tag,myshapeid,getNewDepth(obj),&i->page_matrix,0,NULL);
}

static SWFFONT* gfxfont_to_swffont(gfxfont_t*font, char* id)
{
    SWFFONT*swffont = (SWFFONT*)rfx_calloc(sizeof(SWFFONT));
    int t;
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
	swffont->glyph2ascii[t] = font->glyphs[t].unicode;
	if(font->glyphs[t].name) {
	    swffont->glyphnames[t] = strdup(font->glyphs[t].name);
	} else {
	    swffont->glyphnames[t] = 0;
	}
	swffont->glyph[t].advance = (int)(font->glyphs[t].advance * 20);

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
	draw.dealloc(&draw);
    }
    return swffont;
}

void swf_addfont(gfxdevice_t*dev, char*fontid, gfxfont_t*font)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;

    if(i->obj->swffont && i->obj->swffont->name && !strcmp((char*)i->obj->swffont->name,fontid))
	return; // the requested font is the current font
    
    fontlist_t*last=0,*l = i->fontlist;
    while(l) {
	last = l;
	if(!strcmp((char*)l->swffont->name, fontid)) {
	    return; // we already know this font
	}
	l = l->next;
    }
    l = (fontlist_t*)rfx_calloc(sizeof(fontlist_t));
    l->swffont = gfxfont_to_swffont(font, fontid);
    l->next = 0;
    if(last) {
	last->next = l;
    } else {
	i->fontlist = l;
    }
    swf_FontSetID(l->swffont, getNewID(i->obj));

    if(getScreenLogLevel() >= LOGLEVEL_DEBUG)  {
	// print font information
	msg("<debug> Font %s",fontid);
	msg("<debug> |   ID: %d", l->swffont->id);
	msg("<debug> |   Version: %d", l->swffont->version);
	msg("<debug> |   Name: %s", l->swffont->name);
	msg("<debug> |   Numchars: %d", l->swffont->numchars);
	msg("<debug> |   Maxascii: %d", l->swffont->maxascii);
	msg("<debug> |   Style: %d", l->swffont->style);
	msg("<debug> |   Encoding: %d", l->swffont->encoding);
	for(int iii=0; iii<l->swffont->numchars;iii++) {
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
    swfoutput*obj = i->obj;

    if(obj->swffont && obj->swffont->name && !strcmp((char*)obj->swffont->name,fontid))
	return; // the requested font is the current font
    
    fontlist_t*l = i->fontlist;
    while(l) {
	if(!strcmp((char*)l->swffont->name, fontid)) {
	    obj->swffont = l->swffont;
	    return; //done!
	}
	l = l->next;
    }
    msg("<error> Unknown font id: %s", fontid);
    return;
}

void swf_drawchar(gfxdevice_t*dev, char*fontid, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    swfoutput_internal*i = (swfoutput_internal*)dev->internal;
    swfoutput*obj = i->obj;
	
    if(!obj->swffont || !obj->swffont->name || strcmp((char*)obj->swffont->name,fontid)) // not equal to current font
    {
	/* TODO: remove the need for this (enhance getcharacterbbox so that it can cope
		 with multiple fonts */
	endtext(obj);

	swf_switchfont(dev, fontid); // set the current font
    }
    swfoutput_setfontmatrix(obj, matrix->m00, matrix->m01, matrix->m10, matrix->m11);
   
    swfmatrix m;
    m.m11 = obj->fontm11;
    m.m12 = obj->fontm12;
    m.m21 = obj->fontm21;
    m.m22 = obj->fontm22;
    m.m31 = matrix->tx;
    m.m32 = matrix->ty;
    drawchar(obj, obj->swffont, 0, glyph, -1, &m, color);
}
