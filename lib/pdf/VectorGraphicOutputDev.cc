/* VectorGraphicOutputDev.cc
   implements a pdf output device (OutputDev).

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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <math.h>

#include "../../config.h"

//xpdf header files
#include "popplercompat.h"
#include "VectorGraphicOutputDev.h"

//  swftools header files
#include "../os.h"
#include "../log.h"
#include "../mem.h"
#include "../utf8.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../gfxfont.h"
#include "../gfxpoly.h"
#include "../devices/record.h"
#include "../devices/ops.h"
#include "../devices/polyops.h"
#include "../devices/render.h"

#include "../png.h"

/* config */
static int verbose = 0;
static int dbgindent = 1;
static void dbg(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(!verbose)
	return;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    printf("(pdf) ");
    int indent = dbgindent;
    while(indent) {
	printf(" ");
	indent--;
    }
    printf("%s\n", buf);
    fflush(stdout);
}

GFXOutputState::GFXOutputState() {
    this->clipping = 0;
    this->createsoftmask = 0;
    this->transparencygroup = 0;
    this->softmask = 0;
    this->grouprecording = 0;
    this->isolated = 0;
}

VectorGraphicOutputDev::VectorGraphicOutputDev(InfoOutputDev*info, PDFDoc*doc, int*page2page, int num_pages, int x, int y, int x1, int y1, int x2, int y2)
:CommonOutputDev(info, doc, page2page, num_pages, x, y, x1, y1, x2, y2)
{
    this->type3active = 0;
    this->statepos = 0;
    this->xref = 0;
    this->current_gfxfont = 0;
    this->current_fontinfo = 0;
    this->current_text_stroke = 0;
    this->current_text_clip = 0;
    this->outer_clip_box = 0;
    this->config_convertgradients=1;
    this->config_transparent=0;
    this->config_disable_polygon_conversion = 0;
    this->config_multiply = 1;
    this->config_textonly = 0;

    /* for processing drawChar events */
    this->charDev = new CharOutputDev(info, doc, page2page, num_pages, x, y, x1, y1, x2, y2);
    memset(&this->char_output_dev, 0, sizeof(gfxdevice_t));
    this->char_output_dev.internal = this;
    this->char_output_dev.drawchar = drawchar_callback;
    this->char_output_dev.addfont = addfont_callback;
  
    memset(states, 0, sizeof(states));
};

void VectorGraphicOutputDev::setParameter(const char*key, const char*value)
{
    if(!strcmp(key,"transparent")) {
        this->config_transparent = atoi(value);
    } else if(!strcmp(key,"convertgradients")) {
        this->config_convertgradients = atoi(value);
    } else if(!strcmp(key,"textonly")) {
        this->config_textonly = atoi(value);
    } else if(!strcmp(key,"multiply")) {
        this->config_multiply = atoi(value);
        if(this->config_multiply<1) 
            this->config_multiply=1;
    } else if(!strcmp(key,"disable_polygon_conversion")) {
        this->config_disable_polygon_conversion = atoi(value);
    }
    this->charDev->setParameter(key, value);
}
  
void VectorGraphicOutputDev::setDevice(gfxdevice_t*dev)
{
    this->device = dev;
    charDev->setDevice(dev);
}

//void VectorGraphicOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) {printf("void VectorGraphicOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) \n");}
//void VectorGraphicOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) {printf("void VectorGraphicOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) \n");}

void dump_outline(gfxline_t*line)
{
    /*gfxbbox_t*r = gfxline_isrectangle(line);
    if(!r)
        printf("is not a rectangle\n");
    else
        printf("is a rectangle: (%f,%f)-(%f-%f)\n", r->xmin, r->ymin, r->xmax, r->ymax);
    */

    while(line) {
	if(line->type == gfx_moveTo) {
	    msg("<debug> |     moveTo %.2f %.2f", line->x,line->y);
	} else if(line->type == gfx_lineTo) {
	    msg("<debug> |     lineTo %.2f %.2f", line->x,line->y);
	} else if(line->type == gfx_splineTo) {
	    msg("<debug> |     splineTo (%.2f %.2f) %.2f %.2f", line->sx,line->sy, line->x, line->y);
	}
	line = line->next;
    }
}

void gfxPath_dump(GfxPath*path)
{
    int num = path->getNumSubpaths();
    int t;
    int cpos=0;
    for(t = 0; t < num; t++) {
	GfxSubpath *subpath = path->getSubpath(t);
	int subnum = subpath->getNumPoints();
        int s;	
        for(s=0;s<subnum;s++) {
	   double x=subpath->getX(s);
           double y=subpath->getY(s);
	   if(s==0 && !subpath->getCurve(s)) {
                printf("M %f %f\n", x, y);
           } else if(s==0 && subpath->getCurve(s)) {
                printf("E %f %f\n", x, y);
	   } else if(subpath->getCurve(s)) {
                printf("C %f %f\n", x, y);
	   } else {
                printf("T %f %f\n", x, y);
	   }
	}
    }
}

gfxline_t* VectorGraphicOutputDev::gfxPath_to_gfxline(GfxState*state, GfxPath*path, int closed)
{
    int num = path->getNumSubpaths();
    int s,t;
    int cpos = 0;
    double lastx=0,lasty=0,posx=0,posy=0;
    int needsfix=0;
    if(!num) {
	msg("<warning> empty path");
	return 0;
    }
    gfxdrawer_t draw;
    gfxdrawer_target_gfxline(&draw);

    for(t = 0; t < num; t++) {
	GfxSubpath *subpath = path->getSubpath(t);
	int subnum = subpath->getNumPoints();
	double bx=0,by=0,cx=0,cy=0;

	for(s=0;s<subnum;s++) {
	   double x,y;
	   
	   this->transformXY(state, subpath->getX(s),subpath->getY(s),&x,&y);

	   if(s==0) {
		if(closed && needsfix && (fabs(posx-lastx)+fabs(posy-lasty))>0.001) {
		    draw.lineTo(&draw, lastx, lasty);
		}
		draw.moveTo(&draw, x,y);
		posx = lastx = x; 
		posy = lasty = y;
		cpos = 0;
		needsfix = 0;
	   } else if(subpath->getCurve(s) && cpos==0) {
		bx = x;
		by = y;
		cpos = 1;
	   } else if(subpath->getCurve(s) && cpos==1) {
		cx = x;
		cy = y;
		cpos = 2;
	   } else {
	        posx = x;
	        posy = y;
	        if(cpos==0) {
		    draw.lineTo(&draw, x,y);
		} else {
		    gfxdraw_cubicTo(&draw, bx,by, cx,cy, x,y, 0.05);
		}
		needsfix = 1;
		cpos = 0;
	   }
	}
    }
    /* fix non-closed lines */
    if(closed && needsfix && (fabs(posx-lastx)+fabs(posy-lasty))>0.001) {
	draw.lineTo(&draw, lastx, lasty);
    }
    gfxline_t*result = (gfxline_t*)draw.result(&draw);

    gfxline_optimize(result);

    return result;
}

GBool VectorGraphicOutputDev::useTilingPatternFill()
{
    infofeature("tiled patterns");
//    if(config_convertgradients)
//	return gTrue;
    return gFalse;
}
GBool VectorGraphicOutputDev::useShadedFills()
{
    infofeature("shaded fills");
    if(config_convertgradients)
	return gTrue;
    return gFalse;
}

POPPLER_TILING_PATERN_RETURN VectorGraphicOutputDev::tilingPatternFill(GfxState *state,
			       POPPLER_TILING_PATERN_GFX
			       Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep) 
{
    msg("<debug> tilingPatternFill");
    infofeature("tiling pattern fills");
#ifdef HAVE_POPPLER
    // since we don't implement this method yet,
    // reduce it to a series of other drawing operations.
    return gFalse;
#endif
}

GBool VectorGraphicOutputDev::functionShadedFill(GfxState *state, GfxFunctionShading *shading) 
{
    msg("<error> functionShadedFill not supported yet");
    infofeature("function shaded fills");
    return gFalse;
}
static gfxcolor_t col2col(GfxColorSpace*colspace, GfxColor* col)
{
    gfxcolor_t c;
    GfxRGB rgb;
    colspace->getRGB(col, &rgb);
    c.r = colToByte(rgb.r);
    c.g = colToByte(rgb.g);
    c.b = colToByte(rgb.b);
    c.a = 255;
    return c;
}

GBool VectorGraphicOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading)
{
    if(config_textonly) {return gTrue;}

    double x0,y0,r0,x1,y1,x2,y2,x9,y9,r1;
    shading->getCoords(&x0,&y0,&r0,&x9,&y9,&r1);
    x1=x0+r1;y1=y0;
    x2=x0;   y2=y0+r1;
    this->transformXY(state, x0,y0, &x0,&y0);
    this->transformXY(state, x1,y1, &x1,&y1);
    this->transformXY(state, x2,y2, &x2,&y2);
    
    GfxColor color0;
    GfxColor color1;
    GfxColor color2;
    shading->getColor(0.0, &color0);
    shading->getColor(0.5, &color1);
    shading->getColor(1.0, &color2);
  
    GfxColorSpace* colspace = shading->getColorSpace();
    
    msg("<verbose> radialShadedFill %f %f %f %f %f %f %02x%02x%02x->%02x%02x%02x", x0, y0, x1, y1, x2, y2,
	    colToByte(color0.c[0]), colToByte(color0.c[1]), colToByte(color0.c[2]), 
	    colToByte(color1.c[0]), colToByte(color1.c[1]), colToByte(color1.c[2]),
	    colToByte(color2.c[0]), colToByte(color2.c[1]), colToByte(color2.c[2]));
    infofeature("radial shaded fills");

    gfxgradient_t gr[3];
    gfxgradient_t*g = &gr[0];
    g[0].next = &g[1];
    g[1].next = &g[2];
    g[2].next = 0;
    g[0].color = col2col(colspace, &color0);
    g[1].color = col2col(colspace, &color1);
    g[2].color = col2col(colspace, &color2);
    g[0].pos = 0.0;
    g[1].pos = 0.5;
    g[2].pos = 1.0;

    gfxbbox_t b = states[statepos].clipbbox;
    gfxline_t p1,p2,p3,p4,p5;
    p1.type=gfx_moveTo;p1.x=b.xmin; p1.y=b.ymin; p1.next=&p2;
    p2.type=gfx_lineTo;p2.x=b.xmin; p2.y=b.ymax; p2.next=&p3;
    p3.type=gfx_lineTo;p3.x=b.xmax; p3.y=b.ymax; p3.next=&p4;
    p4.type=gfx_lineTo;p4.x=b.xmax; p4.y=b.ymin; p4.next=&p5;
    p5.type=gfx_lineTo;p5.x=b.xmin; p5.y=b.ymin; p5.next=0;
    
    gfxmatrix_t m;
    //m.m00 = (x3-x0); m.m10 = (x1-x0);
    //m.m01 = (y3-y0); m.m11 = (y1-y0);
    //x3/y3 specifies another (the ending) circle, not the second radius of an ellipse
    m.m00 = (x1-x0); m.m10 = (x2-x0);
    m.m01 = (y1-y0); m.m11 = (y2-y0);
    m.tx = x0 - 0.5;
    m.ty = y0 - 0.5;

    device->fillgradient(device, &p1, g, gfxgradient_radial, &m);
    return gTrue;
}

GBool VectorGraphicOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading)
{
    if(config_textonly) {return gTrue;}

    double x0,y0,x1,y1;
    shading->getCoords(&x0,&y0,&x1,&y1);
    this->transformXY(state, x0,y0,&x0,&y0);
    this->transformXY(state, x1,y1,&x1,&y1);

    GfxColor color0;
    GfxColor color1;
    GfxColor color2;
    shading->getColor(0.0, &color0);
    shading->getColor(0.5, &color1);
    shading->getColor(1.0, &color2);
    
    GfxColorSpace* colspace = shading->getColorSpace();
    
    msg("<verbose> axialShadedFill %f %f %f %f %02x%02x%02x->%02x%02x%02x->%02x%02x%02x", x0, y0, x1, y1,
	    colToByte(color0.c[0]), colToByte(color0.c[1]), colToByte(color0.c[2]), 
	    colToByte(color1.c[0]), colToByte(color1.c[1]), colToByte(color1.c[2]),
	    colToByte(color2.c[0]), colToByte(color2.c[1]), colToByte(color2.c[2])
	    );
    infofeature("axial shaded fills");

    gfxgradient_t*g = (gfxgradient_t*)malloc(sizeof(gfxgradient_t)*3);
    g[0].next = &g[1];
    g[1].next = &g[2];
    g[2].next = 0;
    g[0].color = col2col(colspace, &color0);
    g[1].color = col2col(colspace, &color1);
    g[2].color = col2col(colspace, &color2);
    g[0].pos = 0.0;
    g[1].pos = 0.5;
    g[2].pos = 1.0;
 
    double xMin,yMin,xMax,yMax;
    state->getUserClipBBox(&xMin, &yMin, &xMax, &yMax);
    this->transformXY(state, xMin, yMin, &xMin, &yMin);
    msg("<verbose> userClipBox %f %f %f %f", xMin, yMin, xMax, yMax);

    xMin = 0; yMin = 0;
    xMin = 1024; yMin = 1024;

    gfxbbox_t b = states[statepos].clipbbox;
    gfxline_t p1,p2,p3,p4,p5;
    p1.type=gfx_moveTo;p1.x=b.xmin; p1.y=b.ymin; p1.next=&p2;
    p2.type=gfx_lineTo;p2.x=b.xmin; p2.y=b.ymax; p2.next=&p3;
    p3.type=gfx_lineTo;p3.x=b.xmax; p3.y=b.ymax; p3.next=&p4;
    p4.type=gfx_lineTo;p4.x=b.xmax; p4.y=b.ymin; p4.next=&p5;
    p5.type=gfx_lineTo;p5.x=b.xmin; p5.y=b.ymin; p5.next=0;
   
    /* the gradient starts at (-1.0,0.0), so move (0,0) to
       the middle of the two control points */
    gfxmatrix_t m;
    m.m00 = (x1-x0)/2; m.m10 = -(y1-y0)/2;
    m.m01 = (y1-y0)/2; m.m11 =  (x1-x0)/2;
    m.tx = (x0 + x1)/2 - 0.5;
    m.ty = (y0 + y1)/2 - 0.5;

    device->fillgradient(device, &p1, g, gfxgradient_linear, &m);

    free(g);
    return gTrue;
}
  
GBool VectorGraphicOutputDev::useDrawForm() 
{ 
    infofeature("forms");
    return gFalse; 
}
void VectorGraphicOutputDev::drawForm(Ref id) 
{
    msg("<error> drawForm not implemented");
}
GBool VectorGraphicOutputDev::needNonText() 
{ 
    return gTrue; 
}
void VectorGraphicOutputDev::endPage() 
{
    msg("<verbose> endPage (VectorGraphicOutputDev)");
    charDev->endPage(); // link postprocessing
    if(outer_clip_box) {
	device->endclip(device);
	outer_clip_box = 0;
    }
}
void VectorGraphicOutputDev::setDefaultCTM(double *ctm)
{
    charDev->setDefaultCTM(ctm);
    OutputDev::setDefaultCTM(ctm);
}

static inline double sqr(double x) {return x*x;}

#define STROKE_FILL 1
#define STROKE_CLIP 2
void VectorGraphicOutputDev::strokeGfxline(GfxState *state, gfxline_t*line, int flags)
{
    int lineCap = state->getLineCap(); // 0=butt, 1=round 2=square
    int lineJoin = state->getLineJoin(); // 0=miter, 1=round 2=bevel
    double miterLimit = state->getMiterLimit();
    double width = state->getTransformedLineWidth();

    GfxRGB rgb;
    double opaq = state->getStrokeOpacity();
    if(type3active)
	state->getFillRGB(&rgb);
    else
	state->getStrokeRGB(&rgb);
    gfxcolor_t col;
    col.r = colToByte(rgb.r);
    col.g = colToByte(rgb.g);
    col.b = colToByte(rgb.b);
    col.a = (unsigned char)(opaq*255);

    gfx_capType capType = gfx_capRound;
    if(lineCap == 0) capType = gfx_capButt;
    else if(lineCap == 1) capType = gfx_capRound;
    else if(lineCap == 2) capType = gfx_capSquare;
    else msg("<error> Invalid line cap type");

    gfx_joinType joinType = gfx_joinRound;
    if(lineJoin == 0) joinType = gfx_joinMiter;
    else if(lineJoin == 1) joinType = gfx_joinRound;
    else if(lineJoin == 2) joinType = gfx_joinBevel;
    else msg("<error> Invalid line join type");

    gfxline_t*line2 = 0;

    int dashLength = states[statepos].dashLength;
    double*dashPattern = states[statepos].dashPattern;
    double dashStart = states[statepos].dashStart;
    if(dashLength && dashPattern) {
	float * dash = (float*)malloc(sizeof(float)*(dashLength+1));
	int t;

        /* try to find out how much the transformation matrix would
           stretch the dashes, and factor that into the dash lengths.
           This is not the entirely correct approach- it would be 
           better to first convert the path to an unscaled version,
           then apply dashing, and then transform the path using
           the current transformation matrix. However there are few
           PDFs which actually stretch a dashed path in a non-orthonormal
           way */
        double tx1, ty1, tx2, ty2, tx3, ty3;
	this->transformXY(state, 0, 0, &tx1, &ty1);
	this->transformXY(state, 0, 1, &tx2, &ty2);
	this->transformXY(state, 1, 0, &tx3, &ty3);
        double d1 = sqrt(sqr(tx2-tx1)+sqr(ty2-ty1));
        double d2 = sqrt(sqr(tx3-tx1)+sqr(ty3-ty1));
        if(fabs(d1-d2)>0.5)
            warnfeature("non-ortogonally dashed strokes", 0);
        double f = (d1+d2)/2;

	msg("<trace> %d dashes", dashLength);
	msg("<trace> |  phase: %f", dashStart);
	for(t=0;t<dashLength;t++) {
	    dash[t] = (float)dashPattern[t] * f;
            if(!dash[t]) {
                dash[t] = 1e-37;
            }
	    msg("<trace> |  d%-3d: %f", t, dashPattern[t]);
	}
	dash[dashLength] = -1;
	if(getLogLevel() >= LOGLEVEL_TRACE) {
	    dump_outline(line);
	}
        
        line2 = gfxtool_dash_line(line, dash, (float)(dashStart*f));
        line = line2;

	free(dash);
	msg("<trace> After dashing:");
    }
    
    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> stroke width=%f join=%s cap=%s dashes=%d color=%02x%02x%02x%02x",
		width,
		lineJoin==0?"miter": (lineJoin==1?"round":"bevel"),
		lineCap==0?"butt": (lineCap==1?"round":"square"),
		dashLength,
		col.r,col.g,col.b,col.a
		);
        dump_outline(line);
    }

    if(flags&STROKE_FILL) {
        gfxpoly_t* poly = gfxpoly_from_stroke(line, width, capType, joinType, miterLimit, DEFAULT_GRID);
        gfxline_t*gfxline = gfxline_from_gfxpoly(poly);
	if(getLogLevel() >= LOGLEVEL_TRACE)  {
	    dump_outline(gfxline);
	}
	if(!gfxline) {
	    msg("<warning> Empty polygon (resulting from stroked line)");
	}
        if(flags&STROKE_CLIP) {
            device->startclip(device, gfxline);
            states[statepos].clipping++;
        } else {
            device->fill(device, gfxline, &col);
        }
        gfxline_free(gfxline);
	gfxpoly_destroy(poly);
    } else {
        if(flags&STROKE_CLIP) 
            msg("<error> Stroke&clip not supported at the same time");
        device->stroke(device, line, width, &col, capType, joinType, miterLimit);
    }
    
    if(line2)
	gfxline_free(line2);
}

void VectorGraphicOutputDev::fillGfxLine(GfxState *state, gfxline_t*line, char evenodd) 
{
    gfxcolor_t col = gfxstate_getfillcolor(state);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> %sfill %02x%02x%02x%02x", evenodd?"eo":"", col.r, col.g, col.b, col.a);
        dump_outline(line);
    }
    device->fill(device, line, &col);
}

void VectorGraphicOutputDev::clipToGfxLine(GfxState *state, gfxline_t*line, char evenodd) 
{
    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> %sclip", evenodd?"eo":"");
        dump_outline(line);
    }
    gfxbbox_t bbox = gfxline_getbbox(line);
    gfxbbox_intersect(&states[statepos].clipbbox, &bbox);

    device->startclip(device, line);
    states[statepos].clipping++;
}

void VectorGraphicOutputDev::clip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    msg("<trace> clip");
    gfxline_t*line = gfxPath_to_gfxline(state, path, 1);
    if(!config_disable_polygon_conversion) {
	gfxline_t*line2 = gfxpoly_circular_to_evenodd(line, DEFAULT_GRID);
	gfxline_free(line);
	line = line2;
    }
    clipToGfxLine(state, line, 0);
    gfxline_free(line);
}

void VectorGraphicOutputDev::eoClip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    gfxline_t*line = gfxPath_to_gfxline(state, path, 1);
    clipToGfxLine(state, line, 1);
    gfxline_free(line);
}
void VectorGraphicOutputDev::clipToStrokePath(GfxState *state)
{
    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 0);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        double width = state->getTransformedLineWidth();
        msg("<trace> cliptostrokepath width=%f", width);
        dump_outline(line);
    }

    strokeGfxline(state, line, STROKE_FILL|STROKE_CLIP);
    gfxline_free(line);
}

void VectorGraphicOutputDev::finish()
{
    if(outer_clip_box) {
	if(device) {
	    device->endclip(device);
	}
	outer_clip_box = 0;
    }
}

VectorGraphicOutputDev::~VectorGraphicOutputDev() 
{
    finish();
    delete charDev;charDev=0;
};
GBool VectorGraphicOutputDev::upsideDown() 
{
    return gTrue;
};
GBool VectorGraphicOutputDev::useDrawChar() 
{
    return gTrue;
}

void VectorGraphicOutputDev::updateFontMatrix(GfxState*state)
{
    this->current_font_matrix = gfxmatrix_from_state(state);
    charDev->updateTextMat(state);
}

void VectorGraphicOutputDev::updateFont(GfxState*state)
{
    charDev->updateFont(state);
}

void VectorGraphicOutputDev::processLink(Link *link, Catalog *catalog)
{
    charDev->processLink(link, catalog);
}

void VectorGraphicOutputDev::beginString(GfxState *state, GString *s) 
{ 
    int render = state->getRender();
    if(current_text_stroke) {
	msg("<error> Error: Incompatible change of text rendering to %d while inside cliptext", render);
    }
    charDev->beginString(state, s);
}

static gfxline_t* mkEmptyGfxShape(double x, double y)
{
    gfxline_t*line = (gfxline_t*)malloc(sizeof(gfxline_t));
    line->x = x;line->y = y;line->type = gfx_moveTo;line->next = 0;
    return line;
}
    
void addfont_callback(gfxdevice_t*dev, gfxfont_t*font)
{
    VectorGraphicOutputDev*self = (VectorGraphicOutputDev*)dev->internal;
    self->device->addfont(self->device, font);
}
void drawchar_callback(gfxdevice_t*dev, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    VectorGraphicOutputDev*self = (VectorGraphicOutputDev*)dev->internal;
    self->gfxfont_from_callback = font;
    self->glyphnr_from_callback = glyph;
    memcpy(&self->textcolor_from_callback, color, sizeof(gfxcolor_t));
    memcpy(&self->textmatrix_from_callback, matrix, sizeof(gfxmatrix_t));
}

void VectorGraphicOutputDev::drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode charid, int nBytes, Unicode *_u, int uLen)
{
    int render = state->getRender();
    if(((render == RENDER_FILL) ||
       (render == RENDER_FILLSTROKE && state->getTransformedLineWidth()<1.0) ||
       (render == RENDER_INVISIBLE))) {
	charDev->drawChar(state, x, y, dx, dy, originX, originY, charid, nBytes, _u, uLen);
	return;
    }
	
    msg("<debug> Drawing glyph %d as shape", charid);
    infofeature("text rendered as shape");

    charDev->setDevice(&char_output_dev);
    this->gfxfont_from_callback = 0;
    this->glyphnr_from_callback = 0;
    charDev->drawChar(state, x, y, dx, dy, originX, originY, charid, nBytes, _u, uLen);
    charDev->setDevice(device);

    if(!gfxfont_from_callback)  {
	// some chars are ignored by CharOutputDev
	return;
    }
    gfxline_t*glyph = gfxfont_from_callback->glyphs[glyphnr_from_callback].line;

    gfxline_t*tglyph = gfxline_clone(glyph);
    gfxline_transform(tglyph, &textmatrix_from_callback);
    if((render&3) != RENDER_INVISIBLE) {
	gfxline_t*add = gfxline_clone(tglyph);
	current_text_stroke = gfxline_append(current_text_stroke, add);
    }
    if(render&RENDER_CLIP) {
	gfxline_t*add = gfxline_clone(tglyph);
	current_text_clip = gfxline_append(current_text_clip, add);
	if(!current_text_clip) {
	    current_text_clip = mkEmptyGfxShape(textmatrix_from_callback.tx, textmatrix_from_callback.ty);
	}
    }
    gfxline_free(tglyph);
}

void VectorGraphicOutputDev::endString(GfxState *state) 
{ 
    int render = state->getRender();
    msg("<trace> endString() render=%d textstroke=%p", render, current_text_stroke);
    
    if(current_text_stroke) {
	/* fillstroke and stroke text rendering objects we can process right
	   now (as there may be texts of other rendering modes in this
	   text object)- clipping objects have to wait until endTextObject,
	   however */
	device->setparameter(device, "mark","TXT");
	if((render&3) == RENDER_FILL) {
	    fillGfxLine(state, current_text_stroke, 0);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	} else if((render&3) == RENDER_FILLSTROKE) {
	    fillGfxLine(state, current_text_stroke, 0);
	    strokeGfxline(state, current_text_stroke,0);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	} else if((render&3) == RENDER_STROKE) {
	    strokeGfxline(state, current_text_stroke,0);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	}
	device->setparameter(device, "mark","");
    }
}    

void VectorGraphicOutputDev::endTextObject(GfxState *state)
{
    int render = state->getRender();
    msg("<trace> endTextObject() render=%d textstroke=%p clipstroke=%p", render, current_text_stroke, current_text_clip);
    
    if(current_text_clip) {
	device->setparameter(device, "mark","TXT");
	clipToGfxLine(state, current_text_clip, 0);
	device->setparameter(device, "mark","");
	gfxline_free(current_text_clip);
	current_text_clip = 0;
    }
}

/* the logic seems to be as following:
   first, beginType3Char is called, with the charcode and the coordinates.
   if this function returns true, it already knew about the char and has now drawn it.
   if the function returns false, it's a new char, and type3D0 and/or type3D1 might be 
   called with some parameters.
   Afterwards, all draw operations until endType3Char are part of the char (which in this moment is
   at the position first passed to beginType3Char). the char ends with endType3Char.

   The drawing operations between beginType3Char and endType3Char are somewhat different to
   the normal ones. For example, the fillcolor equals the stroke color. (Because the stroke
   color determines the color of a font)
*/

GBool VectorGraphicOutputDev::beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode charid, Unicode *u, int uLen)
{
    return charDev->beginType3Char(state, x, y, dx, dy, charid, u, uLen);
}

void VectorGraphicOutputDev::type3D0(GfxState *state, double wx, double wy) 
{
    charDev->type3D0(state, wx, wy);
}
void VectorGraphicOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury) 
{
    charDev->type3D1(state, wx, wy, llx, lly, urx, ury);
}

void VectorGraphicOutputDev::endType3Char(GfxState *state)
{
    charDev->endType3Char(state);
}

GBool VectorGraphicOutputDev::checkPageSlice(Page *page, double hDPI, double vDPI,
			   int rotate, GBool useMediaBox, GBool crop,
			   int sliceX, int sliceY, int sliceW, int sliceH,
			   GBool printing, Catalog *catalog,
			   GBool (*abortCheckCbk)(void *data),
			   void *abortCheckCbkData)
{
    this->setPage(page);
    charDev->setPage(page);
    return gTrue;
}


void VectorGraphicOutputDev::beginPage(GfxState *state, int pageNum)
{
    this->currentpage = pageNum;
    int rot = doc->getPageRotate(1);
    gfxcolor_t white = {255,255,255,255};
    gfxcolor_t black = {255,0,0,0};
    laststate = state;
    gfxline_t clippath[5];

    msg("<notice> processing PDF page %d (%dx%d:%d:%d)", pageNum, this->width, this->height, -this->movex, -this->movey);
    if(rot!=0)
        msg("<verbose> page is rotated %d degrees", rot);

    clippath[0].type = gfx_moveTo;clippath[0].x = 0;     clippath[0].y = 0;      clippath[0].next = &clippath[1];
    clippath[1].type = gfx_lineTo;clippath[1].x = width; clippath[1].y = 0;      clippath[1].next = &clippath[2];
    clippath[2].type = gfx_lineTo;clippath[2].x = width; clippath[2].y = height; clippath[2].next = &clippath[3];
    clippath[3].type = gfx_lineTo;clippath[3].x = 0;     clippath[3].y = height; clippath[3].next = &clippath[4];
    clippath[4].type = gfx_lineTo;clippath[4].x = 0;     clippath[4].y = 0;      clippath[4].next = 0;

    device->startclip(device, clippath); outer_clip_box = 1;
    if(!config_transparent) {
        device->fill(device, clippath, &white);
    }
    states[statepos].clipbbox.xmin = 0;
    states[statepos].clipbbox.ymin = 0;
    states[statepos].clipbbox.xmax = this->width;
    states[statepos].clipbbox.ymax = this->height;
    
    states[statepos].dashPattern = 0;
    states[statepos].dashLength = 0;
    states[statepos].dashStart = 0;
    
    charDev->startPage(pageNum, state);
}

void VectorGraphicOutputDev::saveState(GfxState *state) {
    dbg("saveState %p", state); dbgindent+=2;

    msg("<trace> saveState %p", state);
    updateAll(state);
    if(statepos>=64) {
      msg("<fatal> Too many nested states in pdf.");
      exit(1);
    }
    statepos ++;
    states[statepos].state = state;
    states[statepos].createsoftmask = states[statepos-1].createsoftmask;
    states[statepos].transparencygroup = states[statepos-1].transparencygroup;
    states[statepos].clipping = 0;
    states[statepos].olddevice = 0;
    states[statepos].clipbbox = states[statepos-1].clipbbox;

    states[statepos].dashPattern = states[statepos-1].dashPattern;
    states[statepos].dashStart = states[statepos-1].dashStart;
    states[statepos].dashLength = states[statepos-1].dashLength;
};

void VectorGraphicOutputDev::restoreState(GfxState *state) {
  dbgindent-=2; dbg("restoreState %p", state);

  if(statepos==0) {
      msg("<fatal> Invalid restoreState");
      exit(1);
  }
  msg("<trace> restoreState %p%s%s", state,
	                          states[statepos].softmask?" (end softmask)":"",
	                          states[statepos].clipping?" (end clipping)":"");
  if(states[statepos].softmask) {
      clearSoftMask(state);
  }

  if(states[statepos].dashPattern) {
      if(!statepos || states[statepos-1].dashPattern != states[statepos].dashPattern) {
          free(states[statepos].dashPattern);
          states[statepos].dashPattern = 0;
      }
  }

  updateAll(state);
  
  while(states[statepos].clipping) {
      device->endclip(device);
      states[statepos].clipping--;
  }
  if(states[statepos].state!=state) {
      msg("<fatal> bad state nesting");
      if(verbose) {
	  int t;
	  for(t=0;t<=statepos;t++) {
	      printf("%p ", states[t].state);
	  }
	  printf("\n");
      }
      exit(1);
  }
  states[statepos].state=0;
  statepos--;
}
 
void VectorGraphicOutputDev::updateLineDash(GfxState *state) 
{
    if(states[statepos].dashPattern &&
       (!statepos || states[statepos-1].dashPattern != states[statepos].dashPattern)) {
        free(states[statepos].dashPattern);
        states[statepos].dashPattern = 0;
    }
    double *pattern = 0;
    int dashLength;
    double dashStart;
    state->getLineDash(&pattern, &dashLength, &dashStart);
    msg("<debug> updateLineDash, %d dashes", dashLength);
    if(!dashLength) {
        states[statepos].dashPattern = 0;
        states[statepos].dashLength = 0;
    } else {
        double*p = (double*)malloc(dashLength*sizeof(states[statepos].dashPattern[0]));
        memcpy(p, pattern, dashLength*sizeof(states[statepos].dashPattern[0]));
        states[statepos].dashPattern = p;
        states[statepos].dashLength = dashLength;
        states[statepos].dashStart = dashStart;
    }
}
  
#define SQR(x) ((x)*(x))
unsigned char* antialize(unsigned char*data, int width, int height, int newwidth, int newheight, int palettesize)
{
    if((newwidth<1 || newheight<1) ||
       (width<=newwidth || height<=newheight))
	return 0;
    unsigned char*newdata;
    int x,y;
    newdata= (unsigned char*)malloc(newwidth*newheight);
    double fx = ((double)width)/newwidth;
    double fy = ((double)height)/newheight;
    double px = 0;
    int blocksize = (int)(8192/(fx*fy));
    int r = 8192*256/palettesize;
    for(x=0;x<newwidth;x++) {
	double ex = px + fx;
	int fromx = (int)px;
	int tox = (int)ex;
	int xweight1 = (int)((1-(px-fromx))*256);
	int xweight2 = (int)((ex-tox)*256);
	double py =0;
	for(y=0;y<newheight;y++) {
	    double ey = py + fy;
	    int fromy = (int)py;
	    int toy = (int)ey;
	    int yweight1 = (int)((1-(py-fromy))*256);
	    int yweight2 = (int)((ey-toy)*256);
	    int a = 0;
	    int xx,yy;
            if(tox>=width) 
                tox = width-1;
            if(toy>=height) 
                toy = height-1;
	    for(xx=fromx;xx<=tox;xx++)
	    for(yy=fromy;yy<=toy;yy++) {
		int b = 1-data[width*yy+xx];
		int weight=256;
		if(xx==fromx) weight = (weight*xweight1)/256;
		if(xx==tox) weight = (weight*xweight2)/256;
		if(yy==fromy) weight = (weight*yweight1)/256;
		if(yy==toy) weight = (weight*yweight2)/256;
		a+=b*weight;
	    }
	    //if(a) a=(palettesize-1)*r/blocksize;
	    newdata[y*newwidth+x] = (a*blocksize)/r;
	    py = ey;
	}
	px = ex;
    }
    return newdata;
}

#define IMAGE_TYPE_JPEG 0
#define IMAGE_TYPE_LOSSLESS 1

static void drawimage(gfxdevice_t*dev, gfxcolor_t* data, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4, int type, int multiply)
{
    gfxcolor_t*newpic=0;
    
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

    gfxmatrix_t m;
    m.m00 = (p4.x-p1.x)/sizex; m.m10 = (p2.x-p1.x)/sizey;
    m.m01 = (p4.y-p1.y)/sizex; m.m11 = (p2.y-p1.y)/sizey;
        
    m.tx = p1.x - 0.5*multiply;
    m.ty = p1.y - 0.5*multiply;

    gfximage_t img;
    img.data = (gfxcolor_t*)data;
    img.width = sizex;
    img.height = sizey;
  
    if(type == IMAGE_TYPE_JPEG)
	/* TODO: pass image_dpi to device instead */
	dev->setparameter(dev, "next_bitmap_is_jpeg", "1");

    dump_outline(&p1);
    dev->fillbitmap(dev, &p1, &img, &m, 0);
}

void drawimagejpeg(gfxdevice_t*dev, gfxcolor_t*mem, int sizex,int sizey, 
        double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4, int multiply)
{
    drawimage(dev,mem,sizex,sizey,x1,y1,x2,y2,x3,y3,x4,y4, IMAGE_TYPE_JPEG, multiply);
}

void drawimagelossless(gfxdevice_t*dev, gfxcolor_t*mem, int sizex,int sizey, 
        double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4, int multiply)
{
    drawimage(dev,mem,sizex,sizey,x1,y1,x2,y2,x3,y3,x4,y4, IMAGE_TYPE_LOSSLESS, multiply);
}


void VectorGraphicOutputDev::drawGeneralImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GfxImageColorMap*colorMap, GBool invert,
				   GBool inlineImg, int mask, int*maskColors,
				   Stream *maskStr, int maskWidth, int maskHeight, GBool maskInvert, GfxImageColorMap*maskColorMap)
{
  /* the code in this function is *old*. It's not pretty, but it works. */

  double x1,y1,x2,y2,x3,y3,x4,y4;
  ImageStream *imgStr;
  Guchar pixBuf[4];
  GfxRGB rgb;
  int ncomps = 1;
  int bits = 1;
  unsigned char* maskbitmap = 0;
				 
  if(colorMap) {
    ncomps = colorMap->getNumPixelComps();
    bits = colorMap->getBits();
  }
      
  if(maskStr) {
      int x,y;
      unsigned char buf[8];
      maskbitmap = (unsigned char*)malloc(maskHeight*maskWidth);
      if(maskColorMap) {
	  ImageStream*imgMaskStr = new ImageStream(maskStr, maskWidth, maskColorMap->getNumPixelComps(), maskColorMap->getBits());
      	  imgMaskStr->reset();
	  unsigned char pal[256];
	  int n = 1 << colorMap->getBits();
	  int t;
	  for(t=0;t<n;t++) {
	      GfxGray gray;
	      pixBuf[0] = t;
	      maskColorMap->getGray(pixBuf, &gray);
	      pal[t] = colToByte(gray);
	  }
	  for (y = 0; y < maskHeight; y++) {
	      for (x = 0; x < maskWidth; x++) {
		  imgMaskStr->getPixel(buf);
		  maskbitmap[y*maskWidth+x] = pal[buf[0]];
	      }
	  }
	  delete imgMaskStr;
      } else {
	  ImageStream*imgMaskStr = new ImageStream(maskStr, maskWidth, 1, 1);
      	  imgMaskStr->reset();
	  for (y = 0; y < maskHeight; y++) {
	      for (x = 0; x < maskWidth; x++) {
		  imgMaskStr->getPixel(buf);
		  buf[0]^=maskInvert;
		  maskbitmap[y*maskWidth+x] = (buf[0]^1)*255;
	      }
	  }
	  delete imgMaskStr;
      }
      maskStr->close();
  }
  
  imgStr = new ImageStream(str, width, ncomps,bits);
  imgStr->reset();

  if(!width || !height || ((height+width)<=1 && (maskWidth+maskHeight)<=1))
  {
      msg("<verbose> Ignoring %d by %d image", width, height);
      unsigned char buf[8];
      int x,y;
      for (y = 0; y < height; ++y)
      for (x = 0; x < width; ++x) {
	  imgStr->getPixel(buf);
      }
      delete imgStr;
      if(maskbitmap)
	  free(maskbitmap);
      return;
  }

  this->transformXY(state, 0, 1, &x1, &y1);
  this->transformXY(state, 0, 0, &x2, &y2);
  this->transformXY(state, 1, 0, &x3, &y3);
  this->transformXY(state, 1, 1, &x4, &y4);

  if(type3active) {
      /* as type 3 bitmaps are antialized, we need to place them
	 at integer coordinates, otherwise flash player's antializing
	 will kick in and make everything blurry */
      x1 = (int)(x1);y1 = (int)(y1);
      x2 = (int)(x2);y2 = (int)(y2);
      x3 = (int)(x3);y3 = (int)(y3);
      x4 = (int)(x4);y4 = (int)(y4);
  }

  if(!(str->getKind()==strDCT)) {
      if(!type3active) {
	  if(mask) infofeature("masked pbm pictures");
	  else infofeature("pbm pictures");
      }
      if(mask)
	msg("<verbose> drawing %d by %d masked picture", width, height);
  }
  if(str->getKind()==strDCT) {
      infofeature("jpeg pictures");
  }

  if(mask) {
      unsigned char buf[8];
      int x,y;
      unsigned char*pic = new unsigned char[width*height];
      gfxcolor_t pal[256];
      GfxRGB rgb;
      state->getFillRGB(&rgb);

      memset(pal,255,sizeof(pal));
      pal[0].r = (int)(colToByte(rgb.r)); pal[1].r = 0;
      pal[0].g = (int)(colToByte(rgb.g)); pal[1].g = 0;
      pal[0].b = (int)(colToByte(rgb.b)); pal[1].b = 0;
      pal[0].a = 255;              pal[1].a = 0;
    
      int numpalette = 2;
      int realwidth = (int)sqrt(SQR(x2-x3) + SQR(y2-y3));
      int realheight = (int)sqrt(SQR(x1-x2) + SQR(y1-y2));
      for (y = 0; y < height; ++y)
      for (x = 0; x < width; ++x)
      {
	    imgStr->getPixel(buf);
	    if(invert) 
		buf[0]=1-buf[0];
	    pic[width*y+x] = buf[0];
      }
     
      if(type3active) {
	  unsigned char*pic2 = 0;
	  numpalette = 16;
	  
	  pic2 = antialize(pic,width,height,realwidth,realheight,numpalette);

	  if(!pic2) {
	    delete[] pic;
	    delete imgStr;
	    return;
	  }
          
	  width = realwidth;
	  height = realheight;
	  delete[] pic;
	  pic = pic2;
	  
	  /* make a black/white palette */

	  float r = 255./(float)(numpalette-1);
	  int t;
	  for(t=0;t<numpalette;t++) {
	      pal[t].r = colToByte(rgb.r);
	      pal[t].g = colToByte(rgb.g);
	      pal[t].b = colToByte(rgb.b);
	      pal[t].a = (unsigned char)(t*r);
	  }
          
      }

      gfxcolor_t*pic2 = new gfxcolor_t[width*height];
      for (y = 0; y < height; ++y) {
	for (x = 0; x < width; ++x) {
	  pic2[width*y+x] = pal[pic[y*width+x]];
	}
      }
      drawimagelossless(device, pic2, width, height, x1,y1,x2,y2,x3,y3,x4,y4, config_multiply);
      delete[] pic2;
      delete[] pic;
      delete imgStr;
      if(maskbitmap) free(maskbitmap);
      return;
  }

  int x,y;

  if(colorMap->getNumPixelComps()!=1 || str->getKind()==strDCT) {
      gfxcolor_t*pic=new gfxcolor_t[width*height];
      for (y = 0; y < height; ++y) {
	for (x = 0; x < width; ++x) {
	  imgStr->getPixel(pixBuf);
	  colorMap->getRGB(pixBuf, &rgb);
	  pic[width*y+x].r = (unsigned char)(colToByte(rgb.r));
	  pic[width*y+x].g = (unsigned char)(colToByte(rgb.g));
	  pic[width*y+x].b = (unsigned char)(colToByte(rgb.b));
	  pic[width*y+x].a = 255;//(U8)(rgb.a * 255 + 0.5);
	  if(maskbitmap) {
              int x1 = x*maskWidth/width;
              int y1 = y*maskHeight/height;
              int x2 = (x+1)*maskWidth/width;
              int y2 = (y+1)*maskHeight/height;
              int xx,yy;
              unsigned int alpha=0;
              unsigned int count=0;
              for(xx=x1;xx<x2;xx++)
              for(yy=y1;yy<y2;yy++) {
                  alpha += maskbitmap[yy*maskWidth+xx];
                  count ++;
              }
              if(count) {
                pic[width*y+x].a = alpha / count;
              } else {
                pic[width*y+x].a = maskbitmap[y1*maskWidth+x1];
              }
	  }
	}
      }
      if(str->getKind()==strDCT)
	  drawimagejpeg(device, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4, config_multiply);
      else
	  drawimagelossless(device, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4, config_multiply);
      delete[] pic;
      delete imgStr;
      if(maskbitmap) free(maskbitmap);
      return;
  } else {
      gfxcolor_t*pic=new gfxcolor_t[width*height];
      gfxcolor_t pal[256];
      int n = 1 << colorMap->getBits();
      int t;
      for(t=0;t<256;t++) {
	  pixBuf[0] = t;
	  colorMap->getRGB(pixBuf, &rgb);
	  pal[t].r = (unsigned char)(colToByte(rgb.r));
	  pal[t].g = (unsigned char)(colToByte(rgb.g));
	  pal[t].b = (unsigned char)(colToByte(rgb.b));
	  pal[t].a = 255;//(U8)(rgb.b * 255 + 0.5);
      }
      for (y = 0; y < height; ++y) {
	for (x = 0; x < width; ++x) {
	  imgStr->getPixel(pixBuf);
	  pic[width*y+x] = pal[pixBuf[0]];
	  if(maskColors && *maskColors==pixBuf[0]) {
	      pic[width*y+x].a = 0;
	  }
	}
      }
      if(maskbitmap) {
	  if(maskWidth < width && maskHeight < height) {
	      for(y = 0; y < height; y++) {
		  for (x = 0; x < width; x++) {
		      pic[width*y+x].a = maskbitmap[(y*maskHeight/height)*maskWidth+(x*maskWidth/width)];
		  }
	      }
	  } else {
	      msg("<verbose> resampling %dx%d to mask size (%dx%d)", width, height, maskWidth, maskHeight);
	      gfxcolor_t*newpic=new gfxcolor_t[maskWidth*maskHeight];
	      double dx = width / (double)maskWidth;
	      double dy = height / (double)maskHeight;
	      double yy = 0;
	      for(y = 0; y < maskHeight; y++) {
		  double xx = 0;
		  for (x = 0; x < maskWidth; x++) {
		      newpic[maskWidth*y+x] = pic[int(yy)*width+int(xx)];
		      newpic[maskWidth*y+x].a = maskbitmap[maskWidth*y+x];
		      xx += dx;
		  }
		  yy += dy;
	      }
	      delete[] pic;
	      pic = newpic;
	      width = maskWidth;
	      height = maskHeight;
	  }
      }
      drawimagelossless(device, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4, config_multiply);

      delete[] pic;
      delete imgStr;
      if(maskbitmap) free(maskbitmap);
      return;
  }
}

void VectorGraphicOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool inlineImg) 
{
    if(config_textonly) {
	OutputDev::drawImageMask(state,ref,str,width,height,invert,inlineImg);
	return;
    }
    dbg("drawImageMask %dx%d, invert=%d inline=%d", width, height, invert, inlineImg);
    msg("<verbose> drawImageMask %dx%d, invert=%d inline=%d", width, height, invert, inlineImg);
    drawGeneralImage(state,ref,str,width,height,0,invert,inlineImg,1, 0, 0,0,0,0, 0);
}

void VectorGraphicOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg)
{
    if(config_textonly) {
	OutputDev::drawImage(state,ref,str,width,height,colorMap,maskColors,inlineImg);
	return;
    }
    dbg("drawImage %dx%d, %s, %s, inline=%d", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskColors?"maskColors":"no maskColors",
	    inlineImg);
    msg("<verbose> drawImage %dx%d, %s, %s, inline=%d", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskColors?"maskColors":"no maskColors",
	    inlineImg);
    if(colorMap)
	msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d", colorMap->getNumPixelComps(),
		colorMap->getBits(),colorMap->getColorSpace()->getMode());
    drawGeneralImage(state,ref,str,width,height,colorMap,0,inlineImg,0,maskColors, 0,0,0,0, 0);
}
  
void VectorGraphicOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       Stream *maskStr, int maskWidth, int maskHeight,
			       GBool maskInvert)
{
    if(config_textonly) {
	OutputDev::drawMaskedImage(state,ref,str,width,height,colorMap,maskStr,maskWidth,maskHeight,maskInvert);
	return;
    }
    dbg("drawMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskWidth, maskHeight);
    msg("<verbose> drawMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskWidth, maskHeight);
    if(colorMap)
	msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d", colorMap->getNumPixelComps(),
		colorMap->getBits(),colorMap->getColorSpace()->getMode());
    drawGeneralImage(state,ref,str,width,height,colorMap,0,0,0,0, maskStr, maskWidth, maskHeight, maskInvert, 0);
}

void VectorGraphicOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap)
{
    if(config_textonly) {
	OutputDev::drawSoftMaskedImage(state,ref,str,width,height,colorMap,maskStr,maskWidth,maskHeight,maskColorMap);
	return;
    }
    dbg("drawSoftMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskWidth, maskHeight);
    msg("<verbose> drawSoftMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskWidth, maskHeight);
    if(colorMap)
	msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d", colorMap->getNumPixelComps(),
		colorMap->getBits(),colorMap->getColorSpace()->getMode());
    drawGeneralImage(state,ref,str,width,height,colorMap,0,0,0,0, maskStr, maskWidth, maskHeight, 0, maskColorMap);
}


void VectorGraphicOutputDev::stroke(GfxState *state) 
{
    if(config_textonly) {return;}

    dbg("stroke");

    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 0);
    strokeGfxline(state, line, 0);
    gfxline_free(line);
}

void VectorGraphicOutputDev::fill(GfxState *state) 
{
    if(config_textonly) {return;}

    gfxcolor_t col = gfxstate_getfillcolor(state);
    dbg("fill %02x%02x%02x%02x",col.r,col.g,col.b,col.a);

    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 1);
    if(!config_disable_polygon_conversion) {
        gfxline_t*line2 = gfxpoly_circular_to_evenodd(line, DEFAULT_GRID);
        gfxline_free(line);
        line = line2;
    }
    fillGfxLine(state, line, 0);
    gfxline_free(line);
}

void VectorGraphicOutputDev::eoFill(GfxState *state) 
{
    if(config_textonly) {return;}

    gfxcolor_t col = gfxstate_getfillcolor(state);
    dbg("eofill %02x%02x%02x%02x",col.r,col.g,col.b,col.a);

    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 1);
    fillGfxLine(state, line, 1);
    gfxline_free(line);
}


void VectorGraphicOutputDev::beginTransparencyGroup(GfxState *state, double *bbox,
				      GfxColorSpace *blendingColorSpace,
				      GBool isolated, GBool knockout,
				      GBool forSoftMask)
{
    const char*colormodename = "";
  
    if(blendingColorSpace) {
	colormodename = GfxColorSpace::getColorSpaceModeName(blendingColorSpace->getMode());
    }
    dbg("beginTransparencyGroup device=%p %.1f/%.1f/%.1f/%.1f %s isolated=%d knockout=%d forsoftmask=%d", device, bbox[0],bbox[1],bbox[2],bbox[3], colormodename, isolated, knockout, forSoftMask);
    msg("<verbose> beginTransparencyGroup %.1f/%.1f/%.1f/%.1f %s isolated=%d knockout=%d forsoftmask=%d", bbox[0],bbox[1],bbox[2],bbox[3], colormodename, isolated, knockout, forSoftMask);
    
    //states[statepos].createsoftmask |= forSoftMask;
    states[statepos].createsoftmask = forSoftMask;
    states[statepos].transparencygroup = !forSoftMask;
    states[statepos].isolated = isolated;

    states[statepos].olddevice = this->device;
    this->device = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));
    dbg("this->device now %p (old: %p)", this->device, states[statepos].olddevice);

    gfxdevice_record_init(this->device, 0);
    
    /*if(!forSoftMask) { ////???
	state->setFillOpacity(0.0);
    }*/
    dbgindent+=2;
}

void VectorGraphicOutputDev::endTransparencyGroup(GfxState *state)
{
    dbgindent-=2;
    gfxdevice_t*r = this->device;

    dbg("endTransparencyGroup this->device now back to %p (destroying %p)", states[statepos].olddevice, this->device);
    
    this->device = states[statepos].olddevice;
    if(!this->device) {
	msg("<error> Invalid state nesting");
    }
    states[statepos].olddevice = 0;

    gfxresult_t*recording = r->finish(r);
    
    dbg("                     forsoftmask=%d recording=%p/%p", states[statepos].createsoftmask, r, recording);
    msg("<verbose> endTransparencyGroup forsoftmask=%d recording=%p/%p", states[statepos].createsoftmask, r, recording);

    if(states[statepos].createsoftmask) {
	states[statepos-1].softmaskrecording = recording;
    } else {
	states[statepos-1].grouprecording = recording;
    }
    
    states[statepos].createsoftmask = 0;
    states[statepos].transparencygroup = 0;
    free(r);
}

void VectorGraphicOutputDev::paintTransparencyGroup(GfxState *state, double *bbox)
{
    const char*blendmodes[] = {"normal","multiply","screen","overlay","darken", "lighten",
                               "colordodge","colorburn","hardlight","softlight","difference",
                               "exclusion","hue","saturation","color","luminosity"};

    dbg("paintTransparencyGroup blend=%s softmaskon=%d recording=%p", blendmodes[state->getBlendMode()], states[statepos].softmask, states[statepos].grouprecording);
    msg("<verbose> paintTransparencyGroup blend=%s softmaskon=%d", blendmodes[state->getBlendMode()], states[statepos].softmask);

    if(state->getBlendMode() == gfxBlendNormal)
	infofeature("transparency groups");
    else {
	char buffer[80];
	sprintf(buffer, "%s blended transparency groups", blendmodes[state->getBlendMode()]);
	warnfeature(buffer, 0);
    }

    gfxresult_t*grouprecording = states[statepos].grouprecording;
  
    int blendmode = state->getBlendMode();
    if(blendmode == gfxBlendNormal || blendmode == gfxBlendMultiply) {
	int alpha = (int)(state->getFillOpacity()*255);
	if(blendmode == gfxBlendMultiply && alpha>200)
	    alpha = 128;
	gfxdevice_t ops;
	dbg("this->device=%p, this->device->name=%s\n", this->device, this->device->name);
	gfxdevice_ops_init(&ops, this->device, alpha);
	gfxresult_record_replay(grouprecording, &ops, 0);
	ops.finish(&ops);
    }
    grouprecording->destroy(grouprecording);

    states[statepos].grouprecording = 0;
}

void VectorGraphicOutputDev::setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *rgb)
{
    if(states[statepos].softmask) {
	/* shouldn't happen, but *does* happen */
	clearSoftMask(state);
    }

    /* alpha = 1: retrieve mask values from alpha layer
       alpha = 0: retrieve mask values from luminance */

    dbg("setSoftMask %.1f/%.1f/%.1f/%.1f alpha=%d backdrop=%02x%02x%02x",
	    bbox[0], bbox[1], bbox[2], bbox[3], alpha, colToByte(rgb->c[0]), colToByte(rgb->c[1]), colToByte(rgb->c[2]));
    msg("<verbose> setSoftMask %.1f/%.1f/%.1f/%.1f alpha=%d backdrop=%02x%02x%02x",
	    bbox[0], bbox[1], bbox[2], bbox[3], alpha, colToByte(rgb->c[0]), colToByte(rgb->c[1]), colToByte(rgb->c[2]));
    if(!alpha)
	infofeature("soft masks");
    else
	warnfeature("soft masks from alpha channel",0);
   
    if(states[statepos].olddevice) {
	msg("<fatal> Internal error: badly balanced softmasks/transparency groups");
	exit(1);
    }
    states[statepos].olddevice = this->device;
    this->device = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));
    gfxdevice_record_init(this->device, 0);

    dbg("softmaskrecording is %p (dev=%p) at statepos %d\n", states[statepos].softmaskrecording, this->device, statepos);
    
    states[statepos].softmask = 1;
    states[statepos].softmask_alpha = alpha;
}

static inline Guchar div255(int x) {
  return (Guchar)((x + (x >> 8) + 0x80) >> 8);
}

static unsigned char clampU8(unsigned char c, unsigned char min, unsigned char max)
{
    if(c < min) c = min;
    if(c > max) c = max;
    return c;
}

void VectorGraphicOutputDev::clearSoftMask(GfxState *state)
{
    if(!states[statepos].softmask)
	return;
    states[statepos].softmask = 0;
    dbg("clearSoftMask statepos=%d", statepos);
    msg("<verbose> clearSoftMask statepos=%d", statepos);
    
    if(!states[statepos].softmaskrecording || strcmp(this->device->name, "record")) {
	msg("<error> Error in softmask/tgroup ordering");
	return;
    }
  
    gfxresult_t*mask = states[statepos].softmaskrecording;
    gfxresult_t*below = this->device->finish(this->device);free(this->device);
    this->device = states[statepos].olddevice;

    /* get outline of all objects below the soft mask */
    gfxdevice_t uniondev;
    gfxdevice_union_init(&uniondev, 0);
    gfxresult_record_replay(below, &uniondev, 0);
    gfxline_t*belowoutline = gfxdevice_union_getunion(&uniondev);
    uniondev.finish(&uniondev);
    gfxbbox_t bbox = gfxline_getbbox(belowoutline);
    gfxline_free(belowoutline);belowoutline=0;
#if 0 
    this->device->startclip(this->device, belowoutline);
    gfxresult_record_replay(below, this->device, 0);
    gfxresult_record_replay(mask, this->device, 0);
    this->device->endclip(this->device);
#endif
    
    int width = (int)bbox.xmax,height = (int)bbox.ymax;
    if(width<=0 || height<=0)
        return;

    gfxdevice_t belowrender;
    gfxdevice_render_init(&belowrender);
    if(states[statepos+1].isolated) {
	belowrender.setparameter(&belowrender, "fillwhite", "1");
    }
    belowrender.setparameter(&belowrender, "antialize", "2");
    belowrender.startpage(&belowrender, width, height);
    gfxresult_record_replay(below, &belowrender, 0);
    belowrender.endpage(&belowrender);
    gfxresult_t* belowresult = belowrender.finish(&belowrender);
    gfximage_t* belowimg = (gfximage_t*)belowresult->get(belowresult,"page0");
    //png_write("below.png", (unsigned char*)belowimg->data, belowimg->width, belowimg->height);

    gfxdevice_t maskrender;
    gfxdevice_render_init(&maskrender);
    maskrender.startpage(&maskrender, width, height);
    gfxresult_record_replay(mask, &maskrender, 0);
    maskrender.endpage(&maskrender);
    gfxresult_t* maskresult = maskrender.finish(&maskrender);
    gfximage_t* maskimg = (gfximage_t*)maskresult->get(maskresult,"page0");

    if(belowimg->width != maskimg->width || belowimg->height != maskimg->height) {
	msg("<fatal> Internal error in mask drawing");
	return;
    }

    int y,x;
    for(y=0;y<height;y++) {
	gfxcolor_t* l1 = &maskimg->data[maskimg->width*y];
	gfxcolor_t* l2 = &belowimg->data[belowimg->width*y];
	for(x=0;x<width;x++) {
	    int alpha;
	    if(states[statepos].softmask_alpha) {
		alpha = l1->a;
	    } else {
		alpha = (77*l1->r + 151*l1->g + 28*l1->b) >> 8;
	    }

	    l2->a = div255(alpha*l2->a);

	    /* DON'T premultiply alpha- this is done by fillbitmap,
	       depending on the output device */
	    //l2->r = div255(alpha*l2->r);
	    //l2->g = div255(alpha*l2->g);
	    //l2->b = div255(alpha*l2->b);

	    l1++;
	    l2++;
	}
    }
    gfxline_t*line = gfxline_makerectangle(0,0,width,height);

    gfxmatrix_t matrix;
    matrix.m00 = 1.0; matrix.m10 = 0.0; matrix.tx = 0.0;
    matrix.m01 = 0.0; matrix.m11 = 1.0; matrix.ty = 0.0;

    if(!config_textonly) {
	this->device->fillbitmap(this->device, line, belowimg, &matrix, 0);
    }

    mask->destroy(mask);
    below->destroy(below);
    maskresult->destroy(maskresult);
    belowresult->destroy(belowresult);
    states[statepos].softmaskrecording = 0;
}
  
//class MemCheck
//{
//    public: ~MemCheck()
//    {
//        delete globalParams;globalParams=0;
//        Object::memCheck(stderr);
//        gMemReport(stderr);
//    }
//} myMemCheck;

