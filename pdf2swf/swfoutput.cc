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
#define standardEncodingSize 335
extern char *standardEncodingNames[standardEncodingSize];

int opennewwindow=0;
int ignoredraworder=0;
int drawonlyshapes=0;
int jpegquality=85;
int storeallcharacters=0;
int enablezlib=0;
int insertstoptag=0;
int flashversion=4;
int splinemaxerror=1;
int fontsplinemaxerror=1;
static int flag_protected = 0;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32;

static int fi;
static char* filename = 0;
static SWF swf;
static TAG *tag;
static int currentswfid = 0;
static int depth = 1;
static int startdepth = 1;

static SHAPE* shape;
static int shapeid = -1;
static int textid = -1;

static int drawmode = -1;
static char storefont = 0;
static int fillstyleid;
static int linestyleid;
static int swflastx=0;
static int swflasty=0;
static int lastwasfill = 0;
static char fill = 0;
static int sizex;
static int sizey;
TAG* cliptags[128];
int clipshapes[128];
u32 clipdepths[128];
int clippos = 0;

int CHARMIDX = 0;
int CHARMIDY = 0;

char fillstylechanged = 0;

static void startshape(struct swfoutput* obj);
static void starttext(struct swfoutput* obj);
static void endshape();
static void endtext();

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
static int moveto(TAG*tag, plotxy p0)
{
    int rx = (int)(p0.x*20);
    int ry = (int)(p0.y*20);
    if(rx!=swflastx || ry!=swflasty || fillstylechanged) {
      swf_ShapeSetMove (tag, shape, rx,ry);
      fillstylechanged = 0;
      swflastx=rx;
      swflasty=ry;
      return 1;
    }
    return 0;
}

// write a line-to command into the swf
static void lineto(TAG*tag, plotxy p0)
{
    int rx = ((int)(p0.x*20)-swflastx);
    int ry = ((int)(p0.y*20)-swflasty);
    /* we can't skip this for rx=0,ry=0, those
       are plots */
    swf_ShapeSetLine (tag, shape, rx,ry);
    swflastx+=rx;
    swflasty+=ry;
}

// write a spline-to command into the swf
static void splineto(TAG*tag, plotxy control,plotxy end)
{
    int cx = ((int)(control.x*20)-swflastx);
    int cy = ((int)(control.y*20)-swflasty);
    swflastx += cx;
    swflasty += cy;
    int ex = ((int)(end.x*20)-swflastx);
    int ey = ((int)(end.y*20)-swflasty);
    swflastx += ex;
    swflasty += ey;
    if(cx || cy || ex || ey)
	swf_ShapeSetCurve(tag, shape, cx,cy,ex,ey);
}

/* write a line, given two points and the transformation
   matrix. */
static void line(TAG*tag, plotxy p0, plotxy p1, struct swfmatrix*m)
{
    transform(&p0,m);
    transform(&p1,m);
    moveto(tag, p0);
    lineto(tag, p1);
}

/* write a cubic (!) spline. This involves calling the approximate()
   function out of spline.cc to convert it to a quadratic spline.  */
static void spline(TAG*tag,plotxy p0,plotxy p1,plotxy p2,plotxy p3,struct swfmatrix*m)
{
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

    if(storefont) {
	/* fonts use a different approximation than shapes */
	num = cspline_approximate(&c, q, fontsplinemaxerror/20.0, APPROXIMATE_RECURSIVE_BINARY);
	//num = cspline_approximate(&c, q, 10.0, APPROXIMATE_INFLECTION);
    } else {
	num = cspline_approximate(&c, q,     splinemaxerror/20.0, APPROXIMATE_RECURSIVE_BINARY);
    }
    for(t=0;t<num;t++) {
	if(!t) 
	    moveto(tag,q[t].start);
        splineto(tag,q[t].control, q[t].end);
    }
}

void resetdrawer()
{
    swflastx = 0;
    swflasty = 0;
}

static void stopFill()
{
    if(lastwasfill)
    {
     swf_ShapeSetStyle(tag,shape,linestyleid,0x8000,0);
     fillstylechanged = 1;
     lastwasfill = 0;
    }
}
static void startFill()
{
    if(!lastwasfill)
    {
     swf_ShapeSetStyle(tag,shape,0x8000,fillstyleid,0);
     fillstylechanged = 1;
     lastwasfill = 1;
    }
}

/* draw a T1 outline. These are generated by pdf2swf and by t1lib
   (representing characters). */
void drawpath(TAG*tag, T1_OUTLINE*outline, struct swfmatrix*m, int log)
{
    if(tag->id != ST_DEFINEFONT &&
        tag->id != ST_DEFINESHAPE &&
        tag->id != ST_DEFINESHAPE2 &&
        tag->id != ST_DEFINESHAPE3)
    {
        msg("<error> internal error: drawpath needs a shape tag, not %d\n",tag->id);
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
        if(outline->type == T1_PATHTYPE_MOVE)
        {
            if(((int)(lastx*20) != (int)(firstx*20) ||
                (int)(lasty*20) != (int)(firsty*20)) &&
                     fill && !init)
            {
                plotxy p0;
                plotxy p1;
                p0.x=lastx;
                p0.y=lasty;
                p1.x=firstx;
                p1.y=firsty;
                if(log) printf("fix: %f,%f -> %f,%f\n",p0.x,p0.y,p1.x,p1.y);
                line(tag, p0, p1, m);
            }
            firstx=x;
            firsty=y;
            init = 0;
        }
        else if(outline->type == T1_PATHTYPE_LINE) 
        {
            plotxy p0;
            plotxy p1;
            p0.x=lastx;
            p0.y=lasty;
            p1.x=x;
            p1.y=y;
            if(log) printf("line: %f,%f -> %f,%f\n",p0.x,p0.y,p1.x,p1.y);
            line(tag, p0,p1,m);
        }
        else if(outline->type == T1_PATHTYPE_BEZIER)
        {
            plotxy p0;
            plotxy p1;
            plotxy p2;
            plotxy p3;
            T1_BEZIERSEGMENT*o2 = (T1_BEZIERSEGMENT*)outline;
            p0.x=x; 
            p0.y=y;
            p1.x=o2->C.x/(float)0xffff+lastx;
            p1.y=o2->C.y/(float)0xffff+lasty;
            p2.x=o2->B.x/(float)0xffff+lastx;
            p2.y=o2->B.y/(float)0xffff+lasty;
            p3.x=lastx;
            p3.y=lasty;
            if(log) printf("spline: %f,%f -> %f,%f\n",p3.x,p3.y,p0.x,p0.y);
            spline(tag,p0,p1,p2,p3,m);
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
             fill)
    {
        plotxy p0;
        plotxy p1;
        p0.x=lastx;
        p0.y=lasty;
        p1.x=firstx;
        p1.y=firsty;
        if(log) printf("fix: %f,%f -> %f,%f\n",p0.x,p0.y,p1.x,p1.y);
        line(tag, p0, p1, m);
    }
}

plotxy getPivot(T1_OUTLINE*outline, int dir, double line_width, int end, int trytwo)
{
    T1_PATHPOINT next, next2;
    double xv=0,yv=0, xv2=0, yv2=0;
    plotxy p;
    int two = 0;

    if(!end) {
	if(outline->type == T1_PATHTYPE_LINE) {
	    next = outline->dest;
	} else {
	    next = ((T1_BEZIERSEGMENT*)outline)->B;
	    if(next.x==0 && next.y==0) {
		next = ((T1_BEZIERSEGMENT*)outline)->C;
	    }
	    if(next.x==0 && next.y==0) {
		next = ((T1_BEZIERSEGMENT*)outline)->dest;
	    }
	}
	next2 = next;
	if(trytwo && outline->last && outline->last->type != T1_PATHTYPE_MOVE) {
	    if(outline->type == T1_PATHTYPE_LINE) {
		next2 = outline->last->dest;
	    } else {
		T1_PATHPOINT c = ((T1_BEZIERSEGMENT*)(outline->last))->C;
		T1_PATHPOINT b = ((T1_BEZIERSEGMENT*)(outline->last))->B;
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
	if(outline->type == T1_PATHTYPE_LINE) {
	    next = outline->dest;
	} else {
	    T1_PATHPOINT c = ((T1_BEZIERSEGMENT*)outline)->C;
	    T1_PATHPOINT b = ((T1_BEZIERSEGMENT*)outline)->B;
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
	if(trytwo && outline->link && outline->link->type != T1_PATHTYPE_MOVE) {
	    if(outline->type == T1_PATHTYPE_LINE) {
		next2 = outline->link->dest;
	    } else {
		next2 = ((T1_BEZIERSEGMENT*)(outline->link))->B;
		if(next2.x==0 && next2.y==0) {
		    next2 = ((T1_BEZIERSEGMENT*)outline->link)->C;
		}
		if(next2.x==0 && next2.y==0) {
		    next2 = ((T1_BEZIERSEGMENT*)outline->link)->dest;
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

void drawShortPath(struct swfoutput*output, double x, double y, struct swfmatrix* m, T1_OUTLINE*outline)
{
    double lastx=x, lasty=y;
    while (outline && outline->type != T1_PATHTYPE_MOVE)
    {
        x += (outline->dest.x/(float)0xffff);
        y += (outline->dest.y/(float)0xffff);

        if(outline->type == T1_PATHTYPE_LINE)
        {
            plotxy p0, p1;
            p0.x=lastx;
	    p0.y=lasty;
            p1.x= x; 
	    p1.y= y;
            line(tag, p0, p1, m);
        }
        else if(outline->type == T1_PATHTYPE_BEZIER)
        {
            plotxy p0,p1,p2,p3;
            T1_BEZIERSEGMENT*o2 = (T1_BEZIERSEGMENT*)outline;
            p3.x=lastx;
	    p3.y=lasty;
            p1.x=o2->C.x/(float)0xffff+lastx;
            p1.y=o2->C.y/(float)0xffff+lasty;
            p2.x=o2->B.x/(float)0xffff+lastx;
            p2.y=o2->B.y/(float)0xffff+lasty;
            p0.x=x; 
	    p0.y=y;
            spline(tag,p0,p1,p2,p3,m);
        } 
        lastx=x;
        lasty=y;
        outline = outline->link;
    }
}

void drawShortPathWithEnds(struct swfoutput*output, double x, double y, struct swfmatrix* m, T1_OUTLINE*outline, int num, int line_cap, int line_join, double line_width)
{
    plotxy d,d2;
    int back = 0;

    if(line_cap == LINE_CAP_BUTT || line_cap == LINE_CAP_SQUARE) {
	endshape();
	startshape(output);
	T1_OUTLINE *last, *tmp=outline;
	plotxy s,e,p0,p1,p2,p3,m0,m1,m2,m3;
	double x2 = x;
	double y2 = y;
	double lx=x,ly=y;
	double ee = 1.0;
	int nr;
	while(tmp && tmp->type != T1_PATHTYPE_MOVE) {
	    last = tmp;
	    lx += (tmp->dest.x/(float)0xffff);
	    ly += (tmp->dest.y/(float)0xffff);
	    tmp = tmp->link;
	}
	s = getPivot(outline, 0, line_width, 0, 0);
	e = getPivot(last, 0, line_width, 1, 0);

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
	    startFill();
	    if(line_cap == LINE_CAP_BUTT) {
		if(dir) {
		    q0.x = 0; q0.y = 0;
		    q1.x = sizex; q1.y = 0;
		    q2.x = sizex; q2.y = sizey;
		    q3.x = 0; q3.y = sizey;
		} else {
		    q0.x = sizex; q0.y = sizey;
		    q1.x = 0; q1.y = sizey;
		    q2.x = 0; q2.y = 0;
		    q3.x = sizex; q3.y = 0;
		}
		q4.x = p0.x; 
		q4.y = p0.y;
		moveto(tag, q0);
		lineto(tag, q1);
		lineto(tag, q2);
		lineto(tag, q3);
		lineto(tag, q0);

		transform(&q4,m);
		lineto(tag, q4);
	    }

	    line(tag, p0, p1, m);
	    line(tag, p1, p2, m);
	    line(tag, p2, p3, m);
	    line(tag, p3, p0, m);

	    if(line_cap == LINE_CAP_BUTT) {
		lineto(tag, q0);
		swf_ShapeSetEnd(tag);
		tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
		swf_ObjectPlaceClip(tag,shapeid,depth,NULL,NULL,NULL,depth+2-nr);
		depth++;
		shapeid = -1;
		startshape(output);
	    }
	    p0 = m0;
	    p1 = m1;
	    p2 = m2;
	    p3 = m3;
	}

	stopFill();
    }

    drawShortPath(output,x,y,m,outline);

    if(line_cap == LINE_CAP_BUTT) {
	endshape();
	startshape(output);
    }
}

void drawT1toRect(struct swfoutput*output, double x, double y, struct swfmatrix* m, T1_OUTLINE*outline, int num, int line_cap, int line_join, double line_width)
{
    plotxy d1,d2,p1,p2,p3,p4;

    d1.x = (outline->dest.x/(float)0xffff);
    d1.y = (outline->dest.y/(float)0xffff);
    d2 = getPivot(outline, 0, line_width, 0, 0);

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

    line(tag, p1,p2, m);
    line(tag, p2,p3, m);
    line(tag, p3,p4, m);
    line(tag, p4,p1, m);
}

void drawShortPathWithStraightEnds(struct swfoutput*output, double x, double y, struct swfmatrix* m, T1_OUTLINE*outline, int num, int line_cap, int line_join, double line_width)
{
    T1_OUTLINE*tmp=outline;
    double xx=x,yy=y;
    int stop=0;
    assert(shapeid>=0);

    startFill();
    drawT1toRect(output, x, y, m,outline, num, line_cap, line_join, line_width);

    while(tmp->link && tmp->link->type!=T1_PATHTYPE_MOVE) {
        xx += (tmp->dest.x/(float)0xffff);
        yy += (tmp->dest.y/(float)0xffff);
	tmp = tmp->link;
    }
    
    assert(tmp->type == T1_PATHTYPE_LINE);
    assert(outline->type == T1_PATHTYPE_LINE);
    
    if(tmp!=outline) {
   
	if(outline->link == tmp) {
	    /* the two straight line segments (which are everything we
	       need to draw) are very likely to overlap. To avoid that
	       they cancel each other out at the end points, start a new
	       shape for the second one */
	    endshape();startshape(output);
	    startFill();
	}

	drawT1toRect(output, xx, yy, m, tmp, num, line_cap, line_join, line_width);

	if(outline->link != tmp)
	{
	    stopFill();stop=1;
	    int save= tmp->type;
	    tmp->type = T1_PATHTYPE_MOVE;
	    x += (outline->dest.x/(float)0xffff);
	    y += (outline->dest.y/(float)0xffff);
	    outline = outline->link;
	    drawShortPath(output, x, y, m, outline);
	    tmp->type = save;
	}
    }
    if(!stop)
	stopFill();
}

static int t1len(T1_OUTLINE*line)
{
    int num=0;
    while(line && line->type != T1_PATHTYPE_MOVE) {
	num++;
	line = line->link;
    }
    return num;
}

static float t1linelen(T1_OUTLINE*line)
{
    float x,y;
    x = (line->dest.x/(float)0xffff);
    y = (line->dest.y/(float)0xffff);
    return sqrt(x*x+y*y);
}

void drawpath2poly(struct swfoutput *output, T1_OUTLINE*outline, struct swfmatrix*m, int log, int line_join, int line_cap, double line_width, double miter_limit)
{
    if(tag->id != ST_DEFINEFONT &&
        tag->id != ST_DEFINESHAPE &&
        tag->id != ST_DEFINESHAPE2 &&
        tag->id != ST_DEFINESHAPE3) {
        msg("<error> internal error: drawpath needs a shape tag, not %d\n",tag->id);
        exit(1);
    }
    assert(shapeid>=0);
    double x=0,y=0;
    double lastx=0,lasty=0;
    int valid = 0;
    int lastwasline = 0;
    T1_OUTLINE*tmp = outline, *last = 0;
    tmp->last = 0;

    while(1) {
	if(tmp) {
	    x += (tmp->dest.x/(float)0xffff);
	    y += (tmp->dest.y/(float)0xffff);
	}
        if(!tmp || tmp->type == T1_PATHTYPE_MOVE) {
	    if(valid && last) {
		if(last->type == T1_PATHTYPE_LINE && t1linelen(last)>line_width*2 &&
		   lastwasline && line_cap != LINE_CAP_ROUND)
		    drawShortPathWithStraightEnds(output, lastx, lasty, m, last, valid, line_cap, line_join, line_width);
		else
		    drawShortPathWithEnds(output, lastx, lasty, m, last, valid, line_cap, line_join, line_width);
	    }
	    if(!tmp)
		break;
	    valid = 0;
	    last = 0;
	    lastx = x;
	    lasty = y;
	} else {
	    if(!last)
		last = tmp;
	    valid++;
	}

	if(tmp && tmp->type == T1_PATHTYPE_LINE && t1linelen(tmp)>line_width*2)
	    lastwasline = 1;
	else
	    lastwasline = 0;

	if(tmp->link)
	    tmp->link->last = tmp; // make sure list is properly linked in both directions
	tmp = tmp->link;
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

static const int CHARDATAMAX = 1024;
struct chardata {
    int charid;
    int fontid;
    int x;
    int y;
    int size;
    RGBA color;
} chardata[CHARDATAMAX];
int chardatapos = 0;

static void putcharacters(TAG*tag)
{
    int t;
    SWFFONT font;
    RGBA color;
    color.r = chardata[0].color.r^255;
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
    if(!chardatapos) {
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

        for(t=0;t<=chardatapos;t++)
        {
            if(lastfontid != chardata[t].fontid || 
                    lastx!=chardata[t].x ||
                    lasty!=chardata[t].y ||
                    !colorcompare(&color, &chardata[t].color) ||
                    charstorepos==127 ||
                    lastsize != chardata[t].size ||
                    t == chardatapos)
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

                if(pass == 1 && t<chardatapos)
                {
                    RGBA*newcolor=0;
                    SWFFONT*newfont=0;
                    int newx = 0;
                    int newy = 0;
                    if(lastx != chardata[t].x ||
                       lasty != chardata[t].y)
                    {
                        newx = chardata[t].x;
                        newy = chardata[t].y;
			if(newx == 0)
			    newx = SET_TO_ZERO;
			if(newy == 0)
			    newy = SET_TO_ZERO;
                    }
                    if(!colorcompare(&color, &chardata[t].color)) 
                    {
                        color = chardata[t].color;
                        newcolor = &color;
                    }
                    font.id = chardata[t].fontid;
                    if(lastfontid != chardata[t].fontid || lastsize != chardata[t].size)
                        newfont = &font;

                    tag->writeBit = 0; // Q&D
                    swf_TextSetInfoRecord(tag, newfont, chardata[t].size, newcolor, newx,newy);
                }

                lastfontid = chardata[t].fontid;
                lastx = chardata[t].x;
                lasty = chardata[t].y;
                lastsize = chardata[t].size;
            }

            if(t==chardatapos)
                    break;

            int advance;
            int nextt = t==chardatapos-1?t:t+1;
            int rel = chardata[nextt].x-chardata[t].x;
            if(rel>=0 && (rel<(1<<(advancebits-1)) || pass==0)) {
               advance = rel;
               lastx=chardata[nextt].x;
            }
            else {
               advance = 0;
               lastx=chardata[t].x;
            }
            charids[charstorepos] = chardata[t].charid;
            charadvance[charstorepos] = advance;
            charstorepos ++;
        }
    }
    chardatapos = 0;
}

static void putcharacter(struct swfoutput*obj, int fontid, int charid, 
                    int x,int y, int size)
{
    if(chardatapos == CHARDATAMAX)
    {
        endtext();
        starttext(obj);
    }
    chardata[chardatapos].fontid = fontid;
    chardata[chardatapos].charid = charid;
    chardata[chardatapos].x = x;
    chardata[chardatapos].y = y;
    chardata[chardatapos].color = obj->fillrgb;
    chardata[chardatapos].size = size;
    chardatapos++;
}


/* process a character. */
static void drawchar(struct swfoutput*obj, SWFFont*font, char*character, int charnr, swfmatrix*m)
{
    int usefonts=1;
    if(m->m12!=0 || m->m21!=0)
        usefonts=0;
    if(m->m11 != m->m22)
        usefonts=0;

    if(!font) {
	msg("<warning> Font is NULL");
    }

    if(usefonts && ! drawonlyshapes)
    {
        int charid = font->getSWFCharID(character, charnr);
	if(charid<0) {
	    msg("<warning> Didn't find character '%s' (%d) in current charset (%s)", 
		    FIXNULL(character),charnr, FIXNULL(font->getName()));
	    return;
	}
        if(shapeid>=0)
            endshape();
        if(textid<0)
            starttext(obj);
        putcharacter(obj, font->swfid, charid,(int)(m->m13*20),(int)(m->m23*20),
                (int)(m->m11*20/2+0.5)); //where does the /2 come from?
    }
    else
    {
        T1_OUTLINE*outline = font->getOutline(character, charnr);
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
            endtext();
        if(shapeid<0)
            startshape(obj);

	startFill();

        int lf = fill;
        fill = 1;
        drawpath(tag, outline, &m2, 0);
        fill = lf;
    }
}

/* draw a curved polygon. */
void swfoutput_drawpath(swfoutput*output, T1_OUTLINE*outline, 
                            struct swfmatrix*m)
{
    if(textid>=0)
        endtext();

    /* Multiple polygons in one shape don't overlap correctly, 
       so we better start a new shape here if the polygon is filled
     */
    if(shapeid>=0 && fill && !ignoredraworder) {
	endshape();
    }

    if(shapeid<0)
        startshape(output);

    if(!fill)
	stopFill();
    else
	startFill();

    drawpath(tag, outline,m, 0); 
}

void swfoutput_drawpath2poly(struct swfoutput*output, T1_OUTLINE*outline, struct swfmatrix*m, int line_join, int line_cap, double line_width, double miter_limit)
{
    if(textid>=0)
        endtext();
    if(shapeid>=0)
	endshape();
    assert(shapeid<0);
    startshape(output);
    stopFill();

    drawpath2poly(output, outline, m, 0, line_join, line_cap, line_width, miter_limit); 
}

/* SWFFont: copy all t1 font outlines to a local 
   array. */
SWFFont::SWFFont(char*name, int id, char*filename)
{
    if(!T1_GetFontName(id))
        T1_LoadFont(id);

    this->name = strdup(T1_GetFontFileName(id));
    this->fontid = strdup(name);
    this->t1id = id;
    
    char**charnamebase= T1_GetAllCharNames(id);
    char**a= charnamebase;
    int t, outlinepos=0;
    char*map[256];

    char*null = 0;
    if(!a)
	a=&null;

    t=0;
    while(a[t])
        t++;
    this->charnum = t;

    if(!charnum) {
	this->standardtablesize = 0;
    }
    msg("<verbose> Font %s(%d): Storing %d outlines.\n", FIXNULL(name), id, charnum);

    this->standardtablesize = 256;
    if(this->charnum < this->standardtablesize)
	this->standardtablesize = this->charnum;
    this->standardtable = (char**)malloc(standardtablesize*sizeof(char*));

    for(t = 0; t < this->standardtablesize; t++) {
	char*name = T1_GetCharName(id,t);
	char chh[2] = "";
	chh[0] = t;
	if(!name || !strstr(name, "notdef")) {
	    if(t<standardEncodingSize) {
		name = standardEncodingNames[t];
	    } 
	    if(!name) {
		name = ""; //TODO: store something like <%d>
	    }
	}
	standardtable[t] = strdup(name);
	msg("<debug> Char %d is named %s\n", t, name);
    }
    
    outline = (T1_OUTLINE**)malloc(charnum*sizeof(T1_OUTLINE*));
    charname = (char**)malloc(charnum*sizeof(char*));
    width = (int*)malloc(charnum*sizeof(int));
    memset(width, 0, charnum*sizeof(int));
    memset(charname, 0, charnum*sizeof(char*));
    used = (char*)malloc(charnum*sizeof(char));
    char2swfcharid = (U16*)malloc(charnum*2);
    swfcharid2char = (U16*)malloc(charnum*2);
    swfcharpos = 0;

    memset(used,0,charnum*sizeof(char));

    this->swfid = ++currentswfid;
    
    t=0;
    while(*a)
    {
        map[t] = strdup(*a);
        a++;
        t++;
        if(t==256 || !*a) {
            int s;
            for(s=t;s<256;s++)
                map[s] = ".notdef";

            int ret = T1_ReencodeFont(id, map);
            if(ret) {
             T1_DeleteFont(id);
             T1_LoadFont(id);
             int ret = T1_ReencodeFont(id, map);
             if(ret)
               fprintf(stderr,"Can't reencode font: (%s) ret:%d\n",filename, ret);
	     /* Deleting the font invalidates the charname array,
		so we have to ask for it again now. 
		We continue at the position we were, hoping the font
		didn't shrink in the meantime or something.
	      */
	     a = T1_GetAllCharNames(id) + (a - charnamebase);
            }

            // parsecharacters
            for(s=0;s<t;s++)
            {
		char* name = T1_GetCharName(id, s);
		if(!name) name = "";
                this->outline[outlinepos] = T1_CopyOutline(T1_GetCharOutline(id, s, 100.0, 0));
		this->width[outlinepos] = T1_GetCharWidth(id, s);
                this->charname[outlinepos] = strdup(name);
                outlinepos++;
            }

	    for(s=0;s<t;s++)
		free(map[s]);
            t=0;
        }
    }
}

/* free all tables, write out definefont tags */
SWFFont::~SWFFont()
{
    int t,usednum=0;
    int*ptr; 

    if(storeallcharacters)
    {
	int t;
	for(t=0;t<this->charnum;t++) 
	{
	    if(this->charname[t])
	      getSWFCharID(this->charname[t], -1);
	}
    }
    
    ptr = (int*)malloc(swfcharpos*sizeof(int));

    for(t=0;t<charnum;t++)
        if(used[t]) usednum++;

    if(usednum && !drawonlyshapes)
    {
        msg("<verbose> Font %s has %d used characters",FIXNULL(fontid), usednum);
        TAG*ftag = swf_InsertTag(swf.firstTag,ST_DEFINEFONT);
        swf_SetU16(ftag, this->swfid);
        int initpos = swf_GetTagLen(ftag);
        swfmatrix m;
        m.m11 = m.m22 = 1;
        m.m21 = m.m12 = 0;
        m.m13 = CHARMIDX;
        m.m23 = CHARMIDY;

        for(t=0;t<swfcharpos;t++) 
        {
            ptr[t] = swf_GetTagLen(ftag);
            swf_SetU16(ftag, 0x1234);
        }
        for(t=0;t<swfcharpos;t++)
        {
            *(U16*)&ftag->data[ptr[t]] = 
		SWAP16(swf_GetTagLen(ftag)-initpos);

            swflastx=0;
            swflasty=0;
            swf_SetU8(ftag,0x10); //1 fill bits, 0 linestyle bits
            SHAPE s;
            s.bits.fill = 1;
            s.bits.line = 0;
            swf_ShapeSetStyle(ftag,&s,0,1,0);
	    fillstylechanged = 1;
            int lastfill = fill;
            fill = 1;
            storefont = 1;
            drawpath(ftag, outline[swfcharid2char[t]],&m, 0);
            storefont = 0;
            fill = lastfill;
            swf_ShapeSetEnd(ftag);
        }
        ftag = swf_InsertTag(ftag,ST_DEFINEFONTINFO);
	swf_SetU16(ftag, this->swfid);
	if(this->fontid) {
	    swf_SetU8(ftag, strlen(this->fontid));
	    swf_SetBlock(ftag, (U8*)this->fontid, strlen(this->fontid));
	} else {
	    swf_SetU8(ftag, 0);
	}
	swf_SetU8(ftag, 0); //flags
	for(t=0;t<swfcharpos;t++)
	{
	    int s;
	    char * name = this->charname[this->swfcharid2char[t]];
	    for(s=0;s<256;s++) {
		if(standardEncodingNames[s] && 
			!strcmp(name,standardEncodingNames[s]))
		    break;
	    }
	    swf_SetU8(ftag, (U8)s);
	}
    }

    free(ptr);
    if(outline)
	free(outline);
    for(t=0;t<charnum;t++)
        free(charname[t]);
    for(t=0;t<standardtablesize;t++)
	if(standardtable[t]) {
	    free(standardtable[t]);
	}
    free(standardtable);
    free(charname);
    free(width);
    free(used);
    free(swfcharid2char);
    free(char2swfcharid);
}

T1_OUTLINE*SWFFont::getOutline(char*name, int charnr)
{
    int t;
    for(t=0;t<this->charnum;t++) {
        if(!strcmp(this->charname[t],name)) {
            return outline[t];
        }
    }
    
    /* if we didn't find the character, maybe
       we can find the capitalized version */
    for(t=0;t<this->charnum;t++) {
        if(!strcasecmp(this->charname[t],name))
	    return outline[t];
    }

    /* if we didn't find it by name, use the names of the first 256 characters
       of the font to try a new name based on charnr */
    if(this->standardtable && charnr>=0 && charnr < this->standardtablesize) {
	T1_OUTLINE*ret =  getOutline(this->standardtable[charnr], -1);
	if(ret) return ret;
    }

    return 0;
}

int SWFFont::getSWFCharID(char*name, int charnr)
{
    int t;
    for(t=0;t<this->charnum;t++) {
        if(!strcmp(this->charname[t],name)) {
            if(!used[t])
            {
                swfcharid2char[swfcharpos] = t;
                char2swfcharid[t] = swfcharpos++;
                used[t] = 1;
            }
            return char2swfcharid[t];
        }
    }

    /* if we didn't find the character, maybe
       we can find the capitalized version */
    for(t=0;t<this->charnum;t++) {
        if(!strcasecmp(this->charname[t],name)) {
            if(!used[t])
            {
                swfcharid2char[swfcharpos] = t;
                char2swfcharid[t] = swfcharpos++;
                used[t] = 1;
            }
            return char2swfcharid[t];
        }
    }

    /* if we didn't find it by name, use the names of the first 256 (or so) characters
       of the font to try a new name based on charnr */
    if(this->standardtable && charnr>=0 && charnr < this->standardtablesize) {
	int ret = getSWFCharID(this->standardtable[charnr], -1);
	if(ret) return ret;
    }
    return -1;
}

int SWFFont::getWidth(char*name)
{
    int t;
    for(t=0;t<this->charnum;t++) {
        if(!strcmp(this->charname[t],name)) {
            return this->width[t];
        }
    }
    return 0;
}

char*SWFFont::getName()
{
    return this->name;
}

struct fontlist_t 
{
    SWFFont * font;
    fontlist_t*next;
} *fontlist = 0;

/* set's the t1 font index of the font to use for swfoutput_drawchar(). */
void swfoutput_setfont(struct swfoutput*obj, char*fontid, int t1id, char*filename)
{
    fontlist_t*last=0,*iterator;
    if(obj->font && !strcmp(obj->font->fontid,fontid))
        return;

    iterator = fontlist;
    while(iterator) {
        if(!strcmp(iterator->font->fontid,fontid))
            break;
        last = iterator;
        iterator = iterator->next;
    }
    if(iterator) 
    {
        obj->font = iterator->font;
        return ;
    }

    if(t1id<0) {
        msg("<error> internal error: t1id:%d, fontid:%s\n", t1id,FIXNULL(fontid));
    }
    
    SWFFont*font = new SWFFont(fontid, t1id, filename);
    iterator = new fontlist_t;
    iterator->font = font;
    iterator->next = 0;

    if(last) 
        last->next = iterator;
    else 
        fontlist = iterator;
    obj->font = font;
}

int swfoutput_queryfont(struct swfoutput*obj, char*fontid)
{
    fontlist_t *iterator = fontlist;
    while(iterator) {
        if(!strcmp(iterator->font->fontid,fontid))
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
    if(obj->fontm11 == m11 &&
       obj->fontm12 == m12 &&
       obj->fontm21 == m21 &&
       obj->fontm22 == m22)
        return;
//    if(textid>=0)
//      endtext();
    obj->fontm11 = m11;
    obj->fontm12 = m12;
    obj->fontm21 = m21;
    obj->fontm22 = m22;
}

/* draws a character at x,y. */
void swfoutput_drawchar(struct swfoutput* obj,double x,double y,char*character, int charnr) 
{
    swfmatrix m;
    m.m11 = obj->fontm11;
    m.m12 = obj->fontm12;
    m.m21 = obj->fontm21;
    m.m22 = obj->fontm22;
    m.m13 = x;
    m.m23 = y;
    drawchar(obj, obj->font, character, charnr, &m);
}

/* initialize the swf writer */
void swfoutput_init(struct swfoutput* obj, char*_filename, int x1, int y1, int x2, int y2)
{
  GLYPH *glyph;
  RGBA rgb;
  SRECT r;
  memset(obj, 0, sizeof(struct swfoutput));
  filename = _filename;
  sizex = x2;
  sizey = y2;

  msg("<verbose> initializing swf output for size %d*%d\n", sizex,sizey);

  obj->font = 0;
  
  memset(&swf,0x00,sizeof(SWF));

  swf.fileVersion    = flashversion;
  swf.frameRate      = 0x0040; // 1 frame per 4 seconds
  swf.movieSize.xmin = 20*x1;
  swf.movieSize.ymin = 20*y1;
  swf.movieSize.xmax = 20*x2;
  swf.movieSize.ymax = 20*y2;
  
  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  tag = swf.firstTag;
  rgb.r = 0xff;
  rgb.g = 0xff;
  rgb.b = 0xff;
  swf_SetRGB(tag,&rgb);

  if(flag_protected)
    tag = swf_InsertTag(tag, ST_PROTECT);

  depth = 1;
  startdepth = depth;
}

void swfoutput_setprotected() //write PROTECT tag
{
  flag_protected = 1;
}

static void startshape(struct swfoutput*obj)
{
  RGBA rgb;
  SRECT r;

  if(textid>=0)
      endtext();

  tag = swf_InsertTag(tag,ST_DEFINESHAPE);

  swf_ShapeNew(&shape);
  linestyleid = swf_ShapeAddLineStyle(shape,obj->linewidth,&obj->strokergb);
  rgb.r = obj->fillrgb.r;
  rgb.g = obj->fillrgb.g;
  rgb.b = obj->fillrgb.b;
  fillstyleid = swf_ShapeAddSolidFillStyle(shape,&obj->fillrgb);

  shapeid = ++currentswfid;
  swf_SetU16(tag,shapeid);  // ID

  r.xmin = 0;
  r.ymin = 0;
  r.xmax = 20*sizex;
  r.ymax = 20*sizey;
  
  swf_SetRect(tag,&r);

  swf_SetShapeStyles(tag,shape);
  swf_ShapeCountBits(shape,NULL,NULL);
  swf_SetShapeBits(tag,shape);

  swf_ShapeSetAll(tag,shape,/*x*/0,/*y*/0,linestyleid,0,0);
  swflastx=swflasty=0;
  lastwasfill = 0;
}

static void starttext(struct swfoutput*obj)
{
  SRECT r;
  MATRIX m;
  if(shapeid>=0)
      endshape();
  tag = swf_InsertTag(tag,ST_DEFINETEXT);
  textid = ++currentswfid;
  swf_SetU16(tag, textid);

  r.xmin = 0;
  r.ymin = 0;
  r.xmax = 20*sizex;
  r.ymax = 20*sizey;
  
  swf_SetRect(tag,&r);

  m.sx = 65536;
  m.sy = 65536;
  m.r0 = 0;
  m.r1 = 0;
  m.tx = 0;
  m.ty = 0;
 
  swf_SetMatrix(tag,&m);
  swflastx=swflasty=0;
}

static void endshape()
{
    if(shapeid<0) 
        return;
    swf_ShapeSetEnd(tag);
    tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
    swf_ObjectPlace(tag,shapeid,/*depth*/depth++,NULL,NULL,NULL);
    shapeid = -1;
}

static void endtext()
{
    if(textid<0)
        return;
    putcharacters(tag);
    swf_SetU8(tag,0);
    tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
    swf_ObjectPlace(tag,textid,/*depth*/depth++,NULL,NULL,NULL);
    textid = -1;
}

static void endpage(struct swfoutput*obj)
{
    if(shapeid>=0)
      endshape();
    if(textid>=0)
      endtext();
    while(clippos)
        swfoutput_endclip(obj);

    if(insertstoptag) {
	ActionTAG*atag=0;
	atag = action_Stop(atag);
	atag = action_End(atag);
	tag = swf_InsertTag(tag,ST_DOACTION);
	swf_ActionSet(tag,atag);
    }
    tag = swf_InsertTag(tag,ST_SHOWFRAME);
}

void swfoutput_newpage(struct swfoutput*obj)
{
    endpage(obj);

    for(depth--;depth>=startdepth;depth--) {
        tag = swf_InsertTag(tag,ST_REMOVEOBJECT2);
        swf_SetU16(tag,depth);
    }

    depth = 1;
    startdepth = depth;
}

/* "destroy" like in (oo-terminology) "destructor". Perform cleaning
   up, complete the swf, and write it out. */
void swfoutput_destroy(struct swfoutput* obj) 
{
    endpage(obj);
    fontlist_t *tmp,*iterator = fontlist;
    while(iterator) {
        delete iterator->font;
        iterator->font = 0;
        tmp = iterator;
        iterator = iterator->next;
        delete tmp;
    }

    T1_CloseLib();
    if(!filename) 
        return;
    if(filename)
     fi = open(filename, O_BINARY|O_CREAT|O_TRUNC|O_WRONLY, 0777);
    else
     fi = 1; // stdout
    
    if(fi<=0) {
     msg("<fatal> Could not create \"%s\". ", FIXNULL(filename));
     exit(1);
    }
 
    tag = swf_InsertTag(tag,ST_END);

    if(enablezlib) {
      if FAILED(swf_WriteSWC(fi,&swf)) 
       msg("<error> WriteSWC() failed.\n");
    } else {
      if FAILED(swf_WriteSWF(fi,&swf)) 
       msg("<error> WriteSWF() failed.\n");
    }

    if(filename)
     close(fi);
    msg("<notice> SWF written\n");
}

void swfoutput_setdrawmode(swfoutput* obj, int mode)
{
    drawmode = mode;
    if(mode == DRAWMODE_FILL)
     fill = 1;
    else if(mode == DRAWMODE_EOFILL)
     fill = 1;
    else if(mode == DRAWMODE_STROKE)
     fill = 0;
    else if(mode == DRAWMODE_CLIP)
     fill = 1;
    else if(mode == DRAWMODE_EOCLIP)
     fill = 1;
}

void swfoutput_setfillcolor(swfoutput* obj, u8 r, u8 g, u8 b, u8 a)
{
    if(obj->fillrgb.r == r &&
       obj->fillrgb.g == g &&
       obj->fillrgb.b == b &&
       obj->fillrgb.a == a) return;
    if(shapeid>=0)
     endshape();

    obj->fillrgb.r = r;
    obj->fillrgb.g = g;
    obj->fillrgb.b = b;
    obj->fillrgb.a = a;
}

void swfoutput_setstrokecolor(swfoutput* obj, u8 r, u8 g, u8 b, u8 a)
{
    if(obj->strokergb.r == r &&
       obj->strokergb.g == g &&
       obj->strokergb.b == b &&
       obj->strokergb.a == a) return;

    if(shapeid>=0)
     endshape();
    obj->strokergb.r = r;
    obj->strokergb.g = g;
    obj->strokergb.b = b;
    obj->strokergb.a = a;
}

void swfoutput_setlinewidth(struct swfoutput*obj, double linewidth)
{
    if(obj->linewidth == (u16)(linewidth*20))
        return;

    if(shapeid>=0)
     endshape();
    obj->linewidth = (u16)(linewidth*20);
}


void swfoutput_startclip(swfoutput*obj, T1_OUTLINE*outline, struct swfmatrix*m)
{
    if(textid>=0)
     endtext();
    if(shapeid>=0)
     endshape();

    if(clippos >= 127)
    {
        msg("<warning> Too many clip levels.");
        clippos --;
    } 
    
    startshape(obj);
    int olddrawmode = drawmode;
    swfoutput_setdrawmode(obj, DRAWMODE_CLIP);
    swfoutput_drawpath(obj, outline, m);
    swf_ShapeSetEnd(tag);
    swfoutput_setdrawmode(obj, olddrawmode);

    tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
    cliptags[clippos] = tag;
    clipshapes[clippos] = shapeid;
    clipdepths[clippos] = depth++;
    clippos++;
    shapeid = -1;
}

void swfoutput_endclip(swfoutput*obj)
{
    if(textid>=0)
     endtext();
    if(shapeid>=0)
     endshape();

    if(!clippos) {
        msg("<error> Invalid end of clipping region");
        return;
    }
    clippos--;
    swf_ObjectPlaceClip(cliptags[clippos],clipshapes[clippos],clipdepths[clippos],NULL,NULL,NULL,depth++);
}

static void drawlink(struct swfoutput*obj, ActionTAG*,ActionTAG*, swfcoord*points, char mouseover);

void swfoutput_linktourl(struct swfoutput*obj, char*url, swfcoord*points)
{
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
    
    if(shapeid>=0)
     endshape();
    if(textid>=0)
     endtext();
    
    if(opennewwindow)
      actions = action_GetUrl(0, url, "_parent");
    else
      actions = action_GetUrl(0, url, "_this");
    actions = action_End(actions);
    
    drawlink(obj, actions, 0, points,0);
}
void swfoutput_linktopage(struct swfoutput*obj, int page, swfcoord*points)
{
    ActionTAG* actions;

    if(shapeid>=0)
     endshape();
    if(textid>=0)
     endtext();
   
      actions = action_GotoFrame(0, page);
      actions = action_End(actions);

    drawlink(obj, actions, 0, points,0);
}

/* Named Links (a.k.a. Acrobatmenu) are used to implement various gadgets
   of the viewer objects, like subtitles, index elements etc.
*/
void swfoutput_namedlink(struct swfoutput*obj, char*name, swfcoord*points)
{
    ActionTAG *actions1,*actions2;
    char*tmp = strdup(name);
    char mouseover = 1;

    if(shapeid>=0)
     endshape();
    if(textid>=0)
     endtext();

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
    int buttonid = ++currentswfid;
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
    myshapeid = ++currentswfid;
    tag = swf_InsertTag(tag,ST_DEFINESHAPE3);
    swf_ShapeNew(&shape);
    rgb.r = rgb.b = rgb.a = rgb.g = 0; 
    fsid = swf_ShapeAddSolidFillStyle(shape,&rgb);
    swf_SetU16(tag, myshapeid);
    r.xmin = (int)(xmin*20);
    r.ymin = (int)(ymin*20);
    r.xmax = (int)(xmax*20);
    r.ymax = (int)(ymax*20);
    swf_SetRect(tag,&r);
    swf_SetShapeStyles(tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(tag,shape);
    swf_ShapeSetAll(tag,shape,/*x*/0,/*y*/0,0,fsid,0);
    swflastx = swflasty = 0;
    moveto(tag, p1);
    lineto(tag, p2);
    lineto(tag, p3);
    lineto(tag, p4);
    lineto(tag, p1);
    swf_ShapeSetEnd(tag);

    /* shape2 */
    myshapeid2 = ++currentswfid;
    tag = swf_InsertTag(tag,ST_DEFINESHAPE3);
    swf_ShapeNew(&shape);
    rgb.r = rgb.b = rgb.a = rgb.g = 255;
    rgb.a = 40;
    fsid = swf_ShapeAddSolidFillStyle(shape,&rgb);
    swf_SetU16(tag, myshapeid2);
    r.xmin = (int)(xmin*20);
    r.ymin = (int)(ymin*20);
    r.xmax = (int)(xmax*20);
    r.ymax = (int)(ymax*20);
    swf_SetRect(tag,&r);
    swf_SetShapeStyles(tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(tag,shape);
    swf_ShapeSetAll(tag,shape,/*x*/0,/*y*/0,0,fsid,0);
    swflastx = swflasty = 0;
    moveto(tag, p1);
    lineto(tag, p2);
    lineto(tag, p3);
    lineto(tag, p4);
    lineto(tag, p1);
    swf_ShapeSetEnd(tag);

    if(!mouseover)
    {
	tag = swf_InsertTag(tag,ST_DEFINEBUTTON);
	swf_SetU16(tag,buttonid); //id
	swf_ButtonSetFlags(tag, 0); //menu=no
	swf_ButtonSetRecord(tag,0x01,myshapeid,depth,0,0);
	swf_ButtonSetRecord(tag,0x02,myshapeid2,depth,0,0);
	swf_ButtonSetRecord(tag,0x04,myshapeid2,depth,0,0);
	swf_ButtonSetRecord(tag,0x08,myshapeid,depth,0,0);
	swf_SetU8(tag,0);
	swf_ActionSet(tag,actions1);
	swf_SetU8(tag,0);
    }
    else
    {
	tag = swf_InsertTag(tag,ST_DEFINEBUTTON2);
	swf_SetU16(tag,buttonid); //id
	swf_ButtonSetFlags(tag, 0); //menu=no
	swf_ButtonSetRecord(tag,0x01,myshapeid,depth,0,0);
	swf_ButtonSetRecord(tag,0x02,myshapeid2,depth,0,0);
	swf_ButtonSetRecord(tag,0x04,myshapeid2,depth,0,0);
	swf_ButtonSetRecord(tag,0x08,myshapeid,depth,0,0);
	swf_SetU8(tag,0); // end of button records
        swf_ButtonSetCondition(tag, BC_IDLE_OVERUP);
	swf_ActionSet(tag,actions1);
	if(actions2) {
	    swf_ButtonSetCondition(tag, BC_OVERUP_IDLE);
	    swf_ActionSet(tag,actions2);
	    swf_SetU8(tag,0);
	    swf_ButtonPostProcess(tag, 2);
	} else {
	    swf_SetU8(tag,0);
            swf_ButtonPostProcess(tag, 1);
	}
    }
    
    tag = swf_InsertTag(tag,ST_PLACEOBJECT2);

    if(posx!=0 || posy!=0) {
	MATRIX m;
	swf_GetMatrix(0,&m);
	m.tx = (int)(posx*20);
	m.ty = (int)(posy*20);
	swf_ObjectPlace(tag, buttonid, depth++,&m,0,0);
    }
    else {
	swf_ObjectPlace(tag, buttonid, depth++,0,0,0);
    }
}

static void drawimage(struct swfoutput*obj, int bitid, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4)
{
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
    myshapeid = ++currentswfid;
    tag = swf_InsertTag(tag,ST_DEFINESHAPE);
    swf_ShapeNew(&shape);
    //lsid = ShapeAddLineStyle(shape,obj->linewidth,&obj->strokergb);
    //fsid = ShapeAddSolidFillStyle(shape,&obj->fillrgb);
    fsid = swf_ShapeAddBitmapFillStyle(shape,&m,bitid,1);
    swf_SetU16(tag, myshapeid);
    r.xmin = (int)(xmin*20);
    r.ymin = (int)(ymin*20);
    r.xmax = (int)(xmax*20);
    r.ymax = (int)(ymax*20);
    swf_SetRect(tag,&r);
    swf_SetShapeStyles(tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(tag,shape);
    swf_ShapeSetAll(tag,shape,/*x*/0,/*y*/0,lsid,fsid,0);
    swflastx = swflasty = 0;
    moveto(tag, p1);
    lineto(tag, p2);
    lineto(tag, p3);
    lineto(tag, p4);
    lineto(tag, p1);
    /*
    ShapeMoveTo  (tag, shape, (int)(x1*20),(int)(y1*20));
    ShapeSetLine (tag, shape, (int)(x1*20);
    ShapeSetLine (tag, shape, x*20,0);
    ShapeSetLine (tag, shape, 0,-y*20);
    ShapeSetLine (tag, shape, -x*20,0);*/
    swf_ShapeSetEnd(tag);

    /* instance */
    tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
    swf_ObjectPlace(tag,myshapeid,/*depth*/depth++,NULL,NULL,NULL);
}

int swfoutput_drawimagejpeg_old(struct swfoutput*obj, char*filename, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4)
{
    TAG*oldtag;
    if(shapeid>=0)
     endshape();
    if(textid>=0)
     endtext();

    int bitid = ++currentswfid;
    oldtag = tag;
    tag = swf_InsertTag(tag,ST_DEFINEBITSJPEG2);
    swf_SetU16(tag, bitid);
    if(swf_SetJPEGBits(tag, filename, jpegquality)<0) {
	swf_DeleteTag(tag);
	tag = oldtag;
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
    TAG*oldtag;
    JPEGBITS*jpeg;

    if(shapeid>=0)
     endshape();
    if(textid>=0)
     endtext();

    int bitid = ++currentswfid;
    oldtag = tag;
    tag = swf_InsertTag(tag,ST_DEFINEBITSJPEG2);
    swf_SetU16(tag, bitid);
    swf_SetJPEGBits2(tag,sizex,sizey,mem,jpegquality);
    drawimage(obj, bitid, sizex, sizey, x1,y1,x2,y2,x3,y3,x4,y4);
    return bitid;
}

int swfoutput_drawimagelossless(struct swfoutput*obj, RGBA*mem, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4)
{
    TAG*oldtag;
    if(shapeid>=0)
     endshape();
    if(textid>=0)
     endtext();

    int bitid = ++currentswfid;
    oldtag = tag;
    tag = swf_InsertTag(tag,ST_DEFINEBITSLOSSLESS);
    swf_SetU16(tag, bitid);
    if(swf_SetLosslessBits(tag,sizex,sizey,mem, BMF_32BIT)<0) {
	swf_DeleteTag(tag);
	tag = oldtag;
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
    TAG*oldtag;
    U8*mem2 = 0;
    if(shapeid>=0)
     endshape();
    if(textid>=0)
     endtext();

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

    int bitid = ++currentswfid;
    oldtag = tag;
    tag = swf_InsertTag(tag,ST_DEFINEBITSLOSSLESS2);
    swf_SetU16(tag, bitid);
    if(swf_SetLosslessBitsIndexed(tag,sizex,sizey,mem, pal, n)<0) {
	swf_DeleteTag(tag);
	tag = oldtag;
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
    if(id<0) return;
    if(shapeid>=0)
     endshape();
    if(textid>=0)
     endtext();

    drawimage(obj, id, sizex, sizey, x1,y1,x2,y2,x3,y3,x4,y4);
}

