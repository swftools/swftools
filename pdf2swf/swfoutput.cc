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
}

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

int config_opennewwindow=0;
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
float config_minlinewidth=0.05;

typedef struct _swfoutput_internal
{
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

    int frameno;
    int lastframeno;
    
    char fillstylechanged;
    
    int bboxrectpos;
    SRECT bboxrect;

    TAG*cliptag;
 
    chardata_t chardata[CHARDATAMAX];
    int chardatapos;
    int firstpage;
    char pagefinished;
} swfoutput_internal;

static swfoutput_internal* init_internal_struct()
{
    swfoutput_internal*i = (swfoutput_internal*)malloc(sizeof(swfoutput_internal));
    memset(i, 0, sizeof(swfoutput_internal));
   

    i->storefont = 0;
    i->currentswfid = 0;
    i->depth = 1;
    i->startdepth = 1;
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

    return i;
};

static void startshape(struct swfoutput* obj);
static void starttext(struct swfoutput* obj);
static void endshape(struct swfoutput* obj,int clip);
static void endtext(struct swfoutput* obj);

// matrix multiplication. changes p0
static void transform (plotxy*p0,struct swfmatrix*m)
{
    double x,y;
    x = m->m11*p0->x+m->m12*p0->y;
    y = m->m21*p0->x+m->m22*p0->y;
    p0->x = x + m->m13;
    p0->y = y + m->m23;
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
static int moveto(struct swfoutput*obj, TAG*tag, float x, float y)
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

// write a line-to command into the swf
static void lineto(struct swfoutput*obj, TAG*tag, plotxy p0)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    int px = (int)(p0.x*20);
    int py = (int)(p0.y*20);
    int rx = (px-i->swflastx);
    int ry = (py-i->swflasty);
    /* we can't skip this for rx=0,ry=0, those
       are plots */
    swf_ShapeSetLine (tag, i->shape, rx,ry);

    addPointToBBox(obj, i->swflastx,i->swflasty);
    addPointToBBox(obj, px,py);

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
    }
    i->shapeisempty = 0;
}

/* write a line, given two points and the transformation
   matrix. */
static void line(struct swfoutput*obj, TAG*tag, plotxy p0, plotxy p1, struct swfmatrix*m)
{
    transform(&p0,m);
    transform(&p1,m);
    moveto(obj, tag, p0);
    lineto(obj, tag, p1);
}

/* write a cubic (!) spline. This involves calling the approximate()
   function out of spline.cc to convert it to a quadratic spline.  */
static void spline(struct swfoutput*obj, TAG*tag,plotxy p0,plotxy p1,plotxy p2,plotxy p3,struct swfmatrix*m)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    double d;
    struct qspline q[128];
    int num;
    int t;
    transform(&p0,m);
    transform(&p1,m);
    transform(&p2,m);
    transform(&p3,m);
    cspline c;
    c.start = p3;
    c.control1 = p2;
    c.control2 = p1;
    c.end = p0;

    if(i->storefont) {
	/* fonts use a different approximation than shapes */
	num = cspline_approximate(&c, q, config_fontsplinemaxerror/20.0, APPROXIMATE_RECURSIVE_BINARY);
	//num = cspline_approximate(&c, q, 10.0, APPROXIMATE_INFLECTION);
    } else {
	num = cspline_approximate(&c, q, config_splinemaxerror/20.0, APPROXIMATE_RECURSIVE_BINARY);
    }
    for(t=0;t<num;t++) {
	if(!t) 
	    moveto(obj, tag,q[t].start);
        splineto(obj, tag,q[t].control, q[t].end);
    }
}

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
void drawpath(struct swfoutput*obj, SWF_OUTLINE*outline, struct swfmatrix*m, int log)
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
	    //if(!init && fill && obj->drawmode != DRAWMODE_EOFILL && !ignoredraworder) {
	    if(config_filloverlap && !init && i->fill && obj->drawmode != DRAWMODE_EOFILL) {
		/* drawmode=FILL (not EOFILL) means that
		   seperate shapes do not cancel each other out.
		   On SWF side, we need to start a new shape for each
		   closed polygon, because SWF only knows EOFILL.
		*/
		endshape(obj,0);
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
                line(obj,i->tag, p0, p1, m);
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
            line(obj,i->tag, p0,p1,m);
        }
        else if(outline->type == SWF_PATHTYPE_BEZIER)
        {
            plotxy p0;
            plotxy p1;
            plotxy p2;
            plotxy p3;
            SWF_BEZIERSEGMENT*o2 = (SWF_BEZIERSEGMENT*)outline;
            p0.x=x; 
            p0.y=y;
            p1.x=o2->C.x/(float)0xffff+lastx;
            p1.y=o2->C.y/(float)0xffff+lasty;
            p2.x=o2->B.x/(float)0xffff+lastx;
            p2.y=o2->B.y/(float)0xffff+lasty;
            p3.x=lastx;
            p3.y=lasty;
            if(log) printf("spline: %f,%f -> %f,%f\n",p3.x,p3.y,p0.x,p0.y);
            spline(obj,i->tag,p0,p1,p2,p3,m);
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
        line(obj, i->tag, p0, p1, m);
    }
}

plotxy getPivot(struct swfoutput*obj, SWF_OUTLINE*outline, int dir, double line_width, int end, int trytwo)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    SWF_PATHPOINT next, next2;
    double xv=0,yv=0, xv2=0, yv2=0;
    plotxy p;
    int two = 0;

    if(!end) {
	if(outline->type == SWF_PATHTYPE_LINE) {
	    next = outline->dest;
	} else {
	    next = ((SWF_BEZIERSEGMENT*)outline)->B;
	    if(next.x==0 && next.y==0) {
		next = ((SWF_BEZIERSEGMENT*)outline)->C;
	    }
	    if(next.x==0 && next.y==0) {
		next = ((SWF_BEZIERSEGMENT*)outline)->dest;
	    }
	}
	next2 = next;
	if(trytwo && outline->last && outline->last->type != SWF_PATHTYPE_MOVE) {
	    if(outline->type == SWF_PATHTYPE_LINE) {
		next2 = outline->last->dest;
	    } else {
		SWF_PATHPOINT c = ((SWF_BEZIERSEGMENT*)(outline->last))->C;
		SWF_PATHPOINT b = ((SWF_BEZIERSEGMENT*)(outline->last))->B;
		next2.x = outline->last->dest.x - c.x;
		next2.y = outline->last->dest.y - c.y;
		if(next2.x==0 && next2.y==0) {
		    next2.x = outline->last->dest.x - b.x;
		    next2.y = outline->last->dest.y - b.y;
		}
		if(next2.x==0 && next2.y==0) {
		    next2.x = outline->last->dest.x;
		    next2.y = outline->last->dest.y;
		}
	    }
	    two = 1;
	}
    } else {
	if(outline->type == SWF_PATHTYPE_LINE) {
	    next = outline->dest;
	} else {
	    SWF_PATHPOINT c = ((SWF_BEZIERSEGMENT*)outline)->C;
	    SWF_PATHPOINT b = ((SWF_BEZIERSEGMENT*)outline)->B;
	    next.x = outline->dest.x - c.x;
	    next.y = outline->dest.y - c.y;
	    if(next.x==0 && next.y==0) {
		next.x = outline->dest.x - b.x;
		next.y = outline->dest.y - b.y;
	    }
	    if(next.x==0 && next.y==0) {
		next.x = outline->dest.x;
		next.y = outline->dest.y;
	    }
	}
	next2 = next;
	if(trytwo && outline->link && outline->link->type != SWF_PATHTYPE_MOVE) {
	    if(outline->type == SWF_PATHTYPE_LINE) {
		next2 = outline->link->dest;
	    } else {
		next2 = ((SWF_BEZIERSEGMENT*)(outline->link))->B;
		if(next2.x==0 && next2.y==0) {
		    next2 = ((SWF_BEZIERSEGMENT*)outline->link)->C;
		}
		if(next2.x==0 && next2.y==0) {
		    next2 = ((SWF_BEZIERSEGMENT*)outline->link)->dest;
		}
	    }
	    two = 1;
	}
    }

    if(dir) {
	xv =  next.y/(float)0xffff;
	yv = -next.x/(float)0xffff;
    } else {
	xv = -next.y/(float)0xffff;
	yv =  next.x/(float)0xffff;
    }

    double r = (line_width/2)/sqrt(xv*xv+yv*yv);
    xv*=r;
    yv*=r;

    if(two) {
	if(dir) {
	    xv2 =  next2.y/(float)0xffff;
	    yv2 = -next2.x/(float)0xffff;
	} else {
	    xv2 = -next2.y/(float)0xffff;
	    yv2 =  next2.x/(float)0xffff;
	}

	double r2 = (line_width/2)/sqrt(xv2*xv2+yv2*yv2);
	xv2*=r2;
	yv2*=r2;
	xv = (xv+xv2)/2;
	yv = (yv+yv2)/2;
	double r3 = (line_width/2)/sqrt(xv*xv+yv*yv);
	xv *= r3;
	yv *= r3;
    }

    p.x = xv;
    p.y = yv;
    return p;
}

void drawShortPath(struct swfoutput*obj, double x, double y, struct swfmatrix* m, SWF_OUTLINE*outline)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    double lastx=x, lasty=y;
    while (outline && outline->type != SWF_PATHTYPE_MOVE)
    {
        x += (outline->dest.x/(float)0xffff);
        y += (outline->dest.y/(float)0xffff);

        if(outline->type == SWF_PATHTYPE_LINE)
        {
            plotxy p0, p1;
            p0.x=lastx;
	    p0.y=lasty;
            p1.x= x; 
	    p1.y= y;
            line(obj, i->tag, p0, p1, m);
        }
        else if(outline->type == SWF_PATHTYPE_BEZIER)
        {
            plotxy p0,p1,p2,p3;
            SWF_BEZIERSEGMENT*o2 = (SWF_BEZIERSEGMENT*)outline;
            p3.x=lastx;
	    p3.y=lasty;
            p1.x=o2->C.x/(float)0xffff+lastx;
            p1.y=o2->C.y/(float)0xffff+lasty;
            p2.x=o2->B.x/(float)0xffff+lastx;
            p2.y=o2->B.y/(float)0xffff+lasty;
            p0.x=x; 
	    p0.y=y;
            spline(obj, i->tag,p0,p1,p2,p3,m);
        } 
        lastx=x;
        lasty=y;
        outline = outline->link;
    }
}

void drawShortPathWithEnds(struct swfoutput*obj, double x, double y, struct swfmatrix* m, SWF_OUTLINE*outline, int num, int line_cap, int line_join, double line_width)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    plotxy d,d2;
    int back = 0;

    if(line_cap == LINE_CAP_BUTT || line_cap == LINE_CAP_SQUARE) {
	endshape(obj,0);
	startshape(obj);
	SWF_OUTLINE *last, *tmp=outline;
	plotxy s,e,p0,p1,p2,p3,m0,m1,m2,m3;
	double x2 = x;
	double y2 = y;
	double lx=x,ly=y;
	double ee = 1.0;
	int nr;
	while(tmp && tmp->type != SWF_PATHTYPE_MOVE) {
	    last = tmp;
	    lx += (tmp->dest.x/(float)0xffff);
	    ly += (tmp->dest.y/(float)0xffff);
	    tmp = tmp->link;
	}
	s = getPivot(obj, outline, 0, line_width, 0, 0);
	e = getPivot(obj, last, 0, line_width, 1, 0);

	if(line_cap == LINE_CAP_BUTT) {
	    /* make the clipping rectangle slighly bigger
	       than the line ending, so that it get's clipped
	       propertly */
	    //ee = 1.01;
	    ee=1.0;
	}

	p0.x = x2 + s.x*ee; 
	p0.y = y2 + s.y*ee;
	p1.x = x2 - s.x*ee; 
	p1.y = y2 - s.y*ee;
	p2.x = x2 - s.y - s.x*ee; 
	p2.y = y2 + s.x - s.y*ee;
	p3.x = x2 - s.y + s.x*ee; 
	p3.y = y2 + s.x + s.y*ee;
	m0.x = lx + e.x*ee; 
	m0.y = ly + e.y*ee;
	m1.x = lx - e.x*ee; 
	m1.y = ly - e.y*ee;
	m2.x = lx + e.y - e.x*ee; 
	m2.y = ly - e.x - e.y*ee;
	m3.x = lx + e.y + e.x*ee; 
	m3.y = ly - e.x + e.y*ee;

	for(nr=0;nr<2;nr++) {
	    int dir=0;
	    struct plotxy q0,q1,q2,q3,q4,q5;

	    startFill(obj);
	    if(line_cap == LINE_CAP_BUTT) {
		if(dir) {
                    /* FIXME: box should be smaller */
		    q0.x = 0; q0.y = 0;
		    q1.x = i->max_x; q1.y = 0;
		    q2.x = i->max_x; q2.y = i->max_y;
		    q3.x = 0; q3.y = i->max_y;
		} else {
                    /* FIXME: box should be smaller */
		    q0.x = i->max_x; q0.y = i->max_y;
		    q1.x = 0; q1.y = i->max_y;
		    q2.x = 0; q2.y = 0;
		    q3.x = i->max_x; q3.y = 0;
		}
		q4.x = p0.x; 
		q4.y = p0.y;
		moveto(obj, i->tag, q0);
		lineto(obj, i->tag, q1);
		lineto(obj, i->tag, q2);
		lineto(obj, i->tag, q3);
		lineto(obj, i->tag, q0);

		transform(&q4,m);
		lineto(obj, i->tag, q4);
	    }

	    line(obj, i->tag, p0, p1, m);
	    line(obj, i->tag, p1, p2, m);
	    line(obj, i->tag, p2, p3, m);
	    line(obj, i->tag, p3, p0, m);

	    if(line_cap == LINE_CAP_BUTT) {
		lineto(obj, i->tag, q0);
		endshape(obj, i->depth+2-nr);
		startshape(obj);
	    }
	    p0 = m0;
	    p1 = m1;
	    p2 = m2;
	    p3 = m3;
	}

	stopFill(obj);
    }

    drawShortPath(obj,x,y,m,outline);

    if(line_cap == LINE_CAP_BUTT) {
	endshape(obj,0);
	startshape(obj);
    }
}

void drawT1toRect(struct swfoutput*obj, double x, double y, struct swfmatrix* m, SWF_OUTLINE*outline, int num, int line_cap, int line_join, double line_width)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    plotxy d1,d2,p1,p2,p3,p4;

    d1.x = (outline->dest.x/(float)0xffff);
    d1.y = (outline->dest.y/(float)0xffff);
    d2 = getPivot(obj, outline, 0, line_width, 0, 0);

    assert(line_cap != LINE_CAP_ROUND);
    if(line_cap == LINE_CAP_SQUARE) {
	x -= +d2.y;
	y -= -d2.x;
	d1.x += +2*d2.y;
	d1.y += -2*d2.x;
    }

    p1.x = x + d2.x;
    p1.y = y + d2.y;
    p2.x = x + d2.x + d1.x;
    p2.y = y + d2.y + d1.y;
    p3.x = x - d2.x + d1.x;
    p3.y = y - d2.y + d1.y;
    p4.x = x - d2.x;
    p4.y = y - d2.y;

    line(obj, i->tag, p1,p2, m);
    line(obj, i->tag, p2,p3, m);
    line(obj, i->tag, p3,p4, m);
    line(obj, i->tag, p4,p1, m);
}

void drawShortPathWithStraightEnds(struct swfoutput*obj, double x, double y, struct swfmatrix* m, SWF_OUTLINE*outline, int num, int line_cap, int line_join, double line_width)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    SWF_OUTLINE*tmp=outline;
    double xx=x,yy=y;
    int stop=0;
    assert(i->shapeid>=0);

    startFill(obj);
    drawT1toRect(obj, x, y, m,outline, num, line_cap, line_join, line_width);

    while(tmp->link && tmp->link->type!=SWF_PATHTYPE_MOVE) {
        xx += (tmp->dest.x/(float)0xffff);
        yy += (tmp->dest.y/(float)0xffff);
	tmp = tmp->link;
    }
    
    assert(tmp->type == SWF_PATHTYPE_LINE);
    assert(outline->type == SWF_PATHTYPE_LINE);
    
    if(tmp!=outline) {
   
	if(outline->link == tmp) {
	    /* the two straight line segments (which are everything we
	       need to draw) are very likely to overlap. To avoid that
	       they cancel each other out at the end points, start a new
	       shape for the second one */
	    endshape(obj,0);startshape(obj);
	    startFill(obj);
	}

	drawT1toRect(obj, xx, yy, m, tmp, num, line_cap, line_join, line_width);

	if(outline->link != tmp)
	{
	    stopFill(obj);stop=1;
	    int save= tmp->type;
	    tmp->type = SWF_PATHTYPE_MOVE;
	    x += (outline->dest.x/(float)0xffff);
	    y += (outline->dest.y/(float)0xffff);
	    outline = outline->link;
	    drawShortPath(obj, x, y, m, outline);
	    tmp->type = save;
	}
    }
    if(!stop)
	stopFill(obj);
}

static int t1len(struct swfoutput*obj, SWF_OUTLINE*line)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    int num=0;
    while(line && line->type != SWF_PATHTYPE_MOVE) {
	num++;
	line = line->link;
    }
    return num;
}

static float t1linelen(struct swfoutput*obj, SWF_OUTLINE*line)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    float x,y;
    x = (line->dest.x/(float)0xffff);
    y = (line->dest.y/(float)0xffff);
    return sqrt(x*x+y*y);
}

void drawpath2poly(struct swfoutput *obj, SWF_OUTLINE*outline, struct swfmatrix*m, int log, int line_join, int line_cap, double line_width, double miter_limit)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if( i->tag->id != ST_DEFINESHAPE &&
        i->tag->id != ST_DEFINESHAPE2 &&
        i->tag->id != ST_DEFINESHAPE3) {
        msg("<error> internal error: drawpath needs a shape tag, not %d\n",i->tag->id);
        exit(1);
    }
    assert(i->shapeid>=0);
    double x=0,y=0;
    double lastx=0,lasty=0;
    int valid = 0;
    int lastwasline = 0;
    SWF_OUTLINE*tmp = outline, *last = 0;
    tmp->last = 0;

    while(1) {
	double previousx = x, previousy = y;
	if(tmp) {
	    x += (tmp->dest.x/(float)0xffff);
	    y += (tmp->dest.y/(float)0xffff);
	}
        if(!tmp || tmp->type == SWF_PATHTYPE_MOVE) {
	    if(valid && last) {
		if(fabs(lastx-previousx)<0.001 && fabs(lasty-previousy)<0.001) {
		    /* endpoints match- the path is closed.
		       Don't bother to draw endings */
		    drawShortPath(obj, lastx, lasty, m, last);
		}
		if(last->type == SWF_PATHTYPE_LINE && t1linelen(obj,last)>line_width*2 &&
		   lastwasline && line_cap != LINE_CAP_ROUND)
		    drawShortPathWithStraightEnds(obj, lastx, lasty, m, last, valid, line_cap, line_join, line_width);
		else
		    drawShortPathWithEnds(obj, lastx, lasty, m, last, valid, line_cap, line_join, line_width);
	    }
	    if(!tmp)
		break;
	    valid = 0;
	    last = 0;
	    lastx = x;
	    lasty = y;
	} else {
	    if(!last)
		last = tmp; //remember last stroke start (first segment after moveto)
	    valid++;
	}

	if(tmp && tmp->type == SWF_PATHTYPE_LINE && t1linelen(obj,tmp)>line_width*2)
	    lastwasline = 1;
	else
	    lastwasline = 0;

	if(tmp->link)
	    tmp->link->last = tmp; // make sure list is properly linked in both directions
	tmp = tmp->link;
    }
}

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
	b.xmin /= 1024;
	b.ymin /= 1024;
	b.xmax /= 1024;
	b.ymax /= 1024;
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

static void putcharacter(struct swfoutput*obj, int fontid, int charid, 
                    int x,int y, int size)
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
    i->chardata[i->chardatapos].color = obj->fillrgb;
    i->chardata[i->chardatapos].size = size;
    i->chardatapos++;
}

/* Notice: we can only put chars in the range -1639,1638 (-32768/20,32768/20).
   So if we set this value to high, the char coordinates will overflow.
   If we set it to low, however, the char positions will be inaccurate */
#define FONT_INTERNAL_SIZE 4

/* process a character. */
static int drawchar(struct swfoutput*obj, SWFFONT *swffont, char*character, int charnr, int u, swfmatrix*m)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(!swffont) {
	msg("<warning> Font is NULL");
	return 0;
    }

    int charid = getCharID(swffont, charnr, character, u); 
    
    if(charid<0) {
	msg("<warning> Didn't find character '%s' (c=%d,u=%d) in current charset (%s, %d characters)", 
		FIXNULL(character),charnr, u, FIXNULL((char*)swffont->name), swffont->numchars);
	return 0;
    }

    if(i->shapeid>=0)
	endshape(obj,0);
    if(i->textid<0)
	starttext(obj);

    float x = m->m13;
    float y = m->m23;
    float det = ((m->m11*m->m22)-(m->m21*m->m12));
    if(fabs(det) < 0.0005) { 
	/* x direction equals y direction- the text is invisible */
	return 1;
    }
    det = 20*FONT_INTERNAL_SIZE / det;

    SPOINT p;
    p.x = (SCOORD)((  x * m->m22 - y * m->m12)*det);
    p.y = (SCOORD)((- x * m->m21 + y * m->m11)*det);

    putcharacter(obj, swffont->id, charid,p.x,p.y,FONT_INTERNAL_SIZE);
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

    putcharacters(obj, i->tag);
    swf_SetU8(i->tag,0);
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    //swf_ObjectPlace(i->tag,i->textid,/*depth*/i->depth++,&i->page_matrix,NULL,NULL);
    MATRIX m2;
    swf_MatrixJoin(&m2,&obj->fontmatrix, &i->page_matrix);

    swf_ObjectPlace(i->tag,i->textid,/*depth*/i->depth++,&m2,NULL,NULL);
    i->textid = -1;
}


/* draw a curved polygon. */
void swfoutput_drawpath(swfoutput*obj, SWF_OUTLINE*outline, 
                            struct swfmatrix*m)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->textid>=0)
        endtext(obj);

    /* Multiple polygons in one shape don't overlap correctly, 
       so we better start a new shape here if the polygon is filled
     */
    if(i->shapeid>=0 && i->fill && !config_ignoredraworder) {
	endshape(obj,0);
    }

    if(i->shapeid<0) {
        startshape(obj);
    }

    if(!i->fill)
	stopFill(obj);
    else
	startFill(obj);

    drawpath(obj, outline,m, 0); 
}

void swfoutput_drawpath2poly(struct swfoutput*obj, SWF_OUTLINE*outline, struct swfmatrix*m, int line_join, int line_cap, double line_width, double miter_limit)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->textid>=0)
        endtext(obj);
    if(i->shapeid>=0)
	endshape(obj,0);
    assert(i->shapeid<0);
    startshape(obj);
    stopFill(obj);

    drawpath2poly(obj, outline, m, 0, line_join, line_cap, line_width, miter_limit); 
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
	    msg("<warning> Font %s has bad unicode mapping", swffont->name);
            swffont->encoding = 255;
        }
    }

    swf_FontSetID(swffont, ++i->currentswfid);
    
    if(getScreenLogLevel() >= LOGLEVEL_DEBUG)  {
	// print font information
	msg("<debug> Font %s (%s)",swffont->name, filename);
	msg("<debug> |   ID: %d", swffont->id);
	msg("<debug> |   Version: %d", swffont->version);
	msg("<debug> |   Name: %s", fontid);
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
void swfoutput_setfontmatrix(struct swfoutput*obj,double m11,double m12,
                                                  double m21,double m22)
{
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
int swfoutput_drawchar(struct swfoutput* obj,double x,double y,char*character, int charnr, int u) 
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    swfmatrix m;
    m.m11 = obj->fontm11;
    m.m12 = obj->fontm12;
    m.m21 = obj->fontm21;
    m.m22 = obj->fontm22;
    m.m13 = x;
    m.m23 = y;
    return drawchar(obj, obj->swffont, character, charnr, u, &m);
}

static void endpage(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->shapeid>=0)
      endshape(obj,0);
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
    
    for(i->depth--;i->depth>=i->startdepth;i->depth--) {
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
    int shapeid = ++i->currentswfid;
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
    swf_ObjectPlace(i->tag,shapeid,i->depth++,0,0,0);
    i->tag = swf_InsertTag(i->tag, ST_PLACEOBJECT2);
    swf_ObjectPlaceClip(i->tag,shapeid,i->depth++,0,0,0,65535);
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
        obj.clipdepth = i->depth++;
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

    swfoutput_internal*i = (swfoutput_internal*)obj->internal;

    SRECT r;
    RGBA rgb;

    msg("<verbose> initializing swf output for size %d*%d\n", i->max_x,i->max_y);

    obj->swffont = 0;
    obj->drawmode = -1;
    
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

    i->startdepth = i->depth = 3; /* leave room for clip and background rectangle */
    
    if(config_protect)
      i->tag = swf_InsertTag(i->tag, ST_PROTECT);
}

static void startshape(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    RGBA rgb;
    SRECT r;

    if(i->textid>=0)
        endtext(obj);

    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE);

    swf_ShapeNew(&i->shape);
    i->linestyleid = swf_ShapeAddLineStyle(i->shape,i->linewidth,&obj->strokergb);
    rgb.r = obj->fillrgb.r;
    rgb.g = obj->fillrgb.g;
    rgb.b = obj->fillrgb.b;
    i->fillstyleid = swf_ShapeAddSolidFillStyle(i->shape,&obj->fillrgb);

    i->shapeid = ++i->currentswfid;
    swf_SetU16(i->tag,i->shapeid);  // ID

    i->bboxrectpos = i->tag->len;
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
        endshape(obj,0);
      
    i->textid = ++i->currentswfid;

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
}

static void endshape(swfoutput*obj, int clipdepth)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->shapeid<0) 
        return;

    if(!clipdepth)
	fixAreas(obj);
	
    if(i->shapeisempty ||
       /*bbox empty?*/
       (i->bboxrect.xmin == i->bboxrect.xmax && 
        i->bboxrect.ymin == i->bboxrect.ymax))
    {
	// delete the shape again, we didn't do anything
	cancelshape(obj);
	return;
    }
    
    swf_ShapeSetEnd(i->tag);

    changeRect(obj, i->tag, i->bboxrectpos, &i->bboxrect);

    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);

    if(clipdepth)
	swf_ObjectPlaceClip(i->tag,i->shapeid,i->depth++,&i->page_matrix,NULL,NULL,clipdepth);
    else
	swf_ObjectPlace(i->tag,i->shapeid,/*depth*/i->depth++,&i->page_matrix,NULL,NULL);

    swf_ShapeFree(i->shape);
    i->shape = 0;
    i->shapeid = -1;
    i->bboxrectpos = -1;
}

void swfoutput_finalize(struct swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;

    if(i->tag && i->tag->id == ST_END)
        return; //already done

    if(i->frameno == i->lastframeno) // fix: add missing pagefeed
        swfoutput_pagefeed(obj);

    endpage(obj);
    fontlist_t *tmp,*iterator = i->fontlist;
    while(iterator) {
	TAG*mtag = i->swf.firstTag;
	if(iterator->swffont) {
	    mtag = swf_InsertTag(mtag, ST_DEFINEFONT2);
	    /*if(!storeallcharacters)
		swf_FontReduce(iterator->swffont);*/
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
    msg("<notice> SWF written\n");
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

void swfoutput_setdrawmode(swfoutput* obj, int mode)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    obj->drawmode = mode;
    if(mode == DRAWMODE_FILL)
     i->fill = 1;
    else if(mode == DRAWMODE_EOFILL)
     i->fill = 1;
    else if(mode == DRAWMODE_STROKE)
     i->fill = 0;
    else if(mode == DRAWMODE_CLIP)
     i->fill = 1;
    else if(mode == DRAWMODE_EOCLIP)
     i->fill = 1;
}

void swfoutput_setfillcolor(swfoutput* obj, U8 r, U8 g, U8 b, U8 a)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(obj->fillrgb.r == r &&
       obj->fillrgb.g == g &&
       obj->fillrgb.b == b &&
       obj->fillrgb.a == a) return;
    if(i->shapeid>=0)
     endshape(obj,0);

    obj->fillrgb.r = r;
    obj->fillrgb.g = g;
    obj->fillrgb.b = b;
    obj->fillrgb.a = a;
}

void swfoutput_setstrokecolor(swfoutput* obj, U8 r, U8 g, U8 b, U8 a)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(obj->strokergb.r == r &&
       obj->strokergb.g == g &&
       obj->strokergb.b == b &&
       obj->strokergb.a == a) return;

    if(i->shapeid>=0)
     endshape(obj,0);
    obj->strokergb.r = r;
    obj->strokergb.g = g;
    obj->strokergb.b = b;
    obj->strokergb.a = a;
}

void swfoutput_setlinewidth(struct swfoutput*obj, double _linewidth)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->linewidth == (U16)(_linewidth*20))
        return;

    if(i->shapeid>=0)
	endshape(obj,0);
    i->linewidth = (U16)(_linewidth*20);
}


void swfoutput_startclip(swfoutput*obj, SWF_OUTLINE*outline, struct swfmatrix*m)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->textid>=0)
     endtext(obj);
    if(i->shapeid>=0)
     endshape(obj,0);

    if(i->clippos >= 127)
    {
        msg("<warning> Too many clip levels.");
        i->clippos --;
    } 
    
    startshape(obj);
    int olddrawmode = obj->drawmode;
    swfoutput_setdrawmode(obj, DRAWMODE_CLIP);
    swfoutput_drawpath(obj, outline, m);
    swf_ShapeSetEnd(i->tag);
    swfoutput_setdrawmode(obj, olddrawmode);

    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);
    i->cliptags[i->clippos] = i->tag;
    i->clipshapes[i->clippos] = i->shapeid;
    i->clipdepths[i->clippos] = i->depth++;
    i->clippos++;
    
    endshape_noput(obj);
}

void swfoutput_endclip(swfoutput*obj)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(i->textid>=0)
	endtext(obj);
    if(i->shapeid>=0)
	endshape(obj,0);

    if(!i->clippos) {
        msg("<error> Invalid end of clipping region");
        return;
    }
    i->clippos--;
    swf_ObjectPlaceClip(i->cliptags[i->clippos],i->clipshapes[i->clippos],i->clipdepths[i->clippos],&i->page_matrix,NULL,NULL,i->depth++);
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
	endshape(obj,0);
    if(i->textid>=0)
	endtext(obj);
    
    if(config_opennewwindow)
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
	endshape(obj,0);
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
	endshape(obj,0);
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
    int buttonid = ++i->currentswfid;
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
    myshapeid = ++i->currentswfid;
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
    myshapeid2 = ++i->currentswfid;
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
	swf_ObjectPlace(i->tag, buttonid, i->depth++,&m,0,0);
    }
    else {
	swf_ObjectPlace(i->tag, buttonid, i->depth++,&i->page_matrix,0,0);
    }
}

static void drawimage(struct swfoutput*obj, int bitid, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    RGBA rgb;
    SRECT r;
    int lsid=0;
    int fsid;
    struct plotxy p1,p2,p3,p4;
    int myshapeid;
    double xmax=x1,ymax=y1,xmin=x1,ymin=y1;
    if(x2>xmax) xmax=x2;
    if(y2>ymax) ymax=y2;
    if(x2<xmin) xmin=x2;
    if(y2<ymin) ymin=y2;
    if(x3>xmax) xmax=x3;
    if(y3>ymax) ymax=y3;
    if(x3<xmin) xmin=x3;
    if(y3<ymin) ymin=y3;
    if(x4>xmax) xmax=x4;
    if(y4>ymax) ymax=y4;
    if(x4<xmin) xmin=x4;
    if(y4<ymin) ymin=y4;
    p1.x=x1; p1.y=y1;
    p2.x=x2; p2.y=y2;
    p3.x=x3; p3.y=y3;
    p4.x=x4; p4.y=y4;

    {p1.x = (int)(p1.x*20)/20.0;
     p1.y = (int)(p1.y*20)/20.0;
     p2.x = (int)(p2.x*20)/20.0;
     p2.y = (int)(p2.y*20)/20.0;
     p3.x = (int)(p3.x*20)/20.0;
     p3.y = (int)(p3.y*20)/20.0;
     p4.x = (int)(p4.x*20)/20.0;
     p4.y = (int)(p4.y*20)/20.0;}
    
    MATRIX m;
    m.sx = (int)(65536*20*(p4.x-p1.x)/sizex);
    m.r1 = -(int)(65536*20*(p4.y-p1.y)/sizex);
    m.r0 = (int)(65536*20*(p1.x-p2.x)/sizey);
    m.sy = -(int)(65536*20*(p1.y-p2.y)/sizey);

    m.tx = (int)(p1.x*20);
    m.ty = (int)(p1.y*20);
  
    /* shape */
    myshapeid = ++i->currentswfid;
    i->tag = swf_InsertTag(i->tag,ST_DEFINESHAPE);
    SHAPE*shape;
    swf_ShapeNew(&shape);
    //lsid = ShapeAddLineStyle(shape,linewidth,&obj->strokergb);
    //fsid = ShapeAddSolidFillStyle(shape,&obj->fillrgb);
    fsid = swf_ShapeAddBitmapFillStyle(shape,&m,bitid,1);
    swf_SetU16(i->tag, myshapeid);
    r.xmin = (int)(xmin*20);
    r.ymin = (int)(ymin*20);
    r.xmax = (int)(xmax*20);
    r.ymax = (int)(ymax*20);
    swf_SetRect(i->tag,&r);
    swf_SetShapeStyles(i->tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(i->tag,shape);
    swf_ShapeSetAll(i->tag,shape,/*x*/0,/*y*/0,lsid,fsid,0);
    i->swflastx = i->swflasty = 0;
    moveto(obj, i->tag, p1);
    lineto(obj, i->tag, p2);
    lineto(obj, i->tag, p3);
    lineto(obj, i->tag, p4);
    lineto(obj, i->tag, p1);
    /*
    ShapeMoveTo  (tag, shape, (int)(x1*20),(int)(y1*20));
    ShapeSetLine (tag, shape, (int)(x1*20);
    ShapeSetLine (tag, shape, x*20,0);
    ShapeSetLine (tag, shape, 0,-y*20);
    ShapeSetLine (tag, shape, -x*20,0);*/
    swf_ShapeSetEnd(i->tag);
    swf_ShapeFree(shape);

    /* instance */
    i->tag = swf_InsertTag(i->tag,ST_PLACEOBJECT2);

    swf_ObjectPlace(i->tag,myshapeid,/*depth*/i->depth++,&i->page_matrix,NULL,NULL);
}

int swfoutput_drawimagejpeg_old(struct swfoutput*obj, char*filename, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    TAG*oldtag;
    if(i->shapeid>=0)
	endshape(obj,0);
    if(i->textid>=0)
	endtext(obj);

    int bitid = ++i->currentswfid;
    oldtag = i->tag;
    i->tag = swf_InsertTag(i->tag,ST_DEFINEBITSJPEG2);
    swf_SetU16(i->tag, bitid);
    if(swf_SetJPEGBits(i->tag, filename, config_jpegquality)<0) {
	swf_DeleteTag(i->tag);
	i->tag = oldtag;
	return -1;
    }

    drawimage(obj, bitid, sizex, sizey, x1,y1,x2,y2,x3,y3,x4,y4);
    return bitid;
}

int swfoutput_drawimagejpeg(struct swfoutput*obj, RGBA*mem, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    TAG*oldtag;
    JPEGBITS*jpeg;

    if(i->shapeid>=0)
	endshape(obj,0);
    if(i->textid>=0)
	endtext(obj);

    int bitid = ++i->currentswfid;
    oldtag = i->tag;
    i->tag = swf_InsertTag(i->tag,ST_DEFINEBITSJPEG2);
    swf_SetU16(i->tag, bitid);
    swf_SetJPEGBits2(i->tag,sizex,sizey,mem,config_jpegquality);
    drawimage(obj, bitid, sizex, sizey, x1,y1,x2,y2,x3,y3,x4,y4);
    return bitid;
}

int swfoutput_drawimagelossless(struct swfoutput*obj, RGBA*mem, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    TAG*oldtag;
    if(i->shapeid>=0)
	endshape(obj,0);
    if(i->textid>=0)
	endtext(obj);

    int bitid = ++i->currentswfid;
    oldtag = i->tag;
    i->tag = swf_InsertTag(i->tag,ST_DEFINEBITSLOSSLESS);
    swf_SetU16(i->tag, bitid);
    if(swf_SetLosslessBits(i->tag,sizex,sizey,mem, BMF_32BIT)<0) {
	swf_DeleteTag(i->tag);
	i->tag = oldtag;
	return -1;
    }
    
    drawimage(obj, bitid, sizex, sizey, x1,y1,x2,y2,x3,y3,x4,y4);
    return bitid;
}

int swfoutput_drawimagelosslessN(struct swfoutput*obj, U8*mem, RGBA*pal, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4, int n)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    TAG*oldtag;
    U8*mem2 = 0;
    if(i->shapeid>=0)
	endshape(obj,0);
    if(i->textid>=0)
	endtext(obj);

    if(sizex&3)
    { 
	/* SWF expects scanlines to be 4 byte aligned */
	int x,y;
	U8*ptr;
	mem2 = (U8*)malloc(BYTES_PER_SCANLINE(sizex)*sizey);
	ptr = mem2;
	for(y=0;y<sizey;y++)
	{
	    for(x=0;x<sizex;x++)
		*ptr++ = mem[y*sizex+x];
	    ptr+= BYTES_PER_SCANLINE(sizex)-sizex;
	}
	mem = mem2;
    }

    int bitid = ++i->currentswfid;
    oldtag = i->tag;
    i->tag = swf_InsertTag(i->tag,ST_DEFINEBITSLOSSLESS2);
    swf_SetU16(i->tag, bitid);
    if(swf_SetLosslessBitsIndexed(i->tag,sizex,sizey,mem, pal, n)<0) {
	swf_DeleteTag(i->tag);
	i->tag = oldtag;
	return -1;
    }
    if(mem2)
	free(mem2);
  
    drawimage(obj, bitid, sizex, sizey, x1,y1,x2,y2,x3,y3,x4,y4);
    return bitid;
}

void swfoutput_drawimageagain(struct swfoutput*obj, int id, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4)
{
    swfoutput_internal*i = (swfoutput_internal*)obj->internal;
    if(id<0) return;
    if(i->shapeid>=0)
	endshape(obj,0);
    if(i->textid>=0)
	endtext(obj);

    drawimage(obj, id, sizex, sizey, x1,y1,x2,y2,x3,y3,x4,y4);
}

void swfoutput_setparameter(char*name, char*value)
{
    if(!strcmp(name, "drawonlyshapes")) {
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
    } else if(!strcmp(name, "insertstop")) {
	config_insertstoptag = atoi(value);
    } else if(!strcmp(name, "protected")) {
	config_protect = atoi(value);
    } else if(!strcmp(name, "flashversion")) {
	config_flashversion = atoi(value);
    } else if(!strcmp(name, "minlinewidth")) {
	config_minlinewidth = atof(value);
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

