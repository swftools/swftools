/*  drawer.c 
    part of swftools

    A generic structure for providing vector drawing.
    (Helper routines, spline approximation, simple text drawers)

    Copyright (C) 2003 Matthias Kramm <kramm@quiss.org>

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
#include <string.h>
#include <memory.h>
#include <math.h>
#include <ctype.h>
#include "drawer.h"

static char* getToken(const char**p)
{
    const char*start;
    char*result;
    while(**p && strchr(" ,()\t\n\r", **p)) {
	(*p)++;
    } 
    start = *p;

     /*
        SVF pathdata can exclude whitespace after L and M commands.
        Ref:  http://www.w3.org/TR/SVG11/paths.html#PathDataGeneralInformation
        This allows us to use svg files output from gnuplot.
        Also checks for relative MoveTo and LineTo (m and l).
        051106 Magnus Lundin, lundin@mlu.mine.nu
     */
    if (strchr("LMlm", **p) && (isdigit(*(*p+1))||strchr("+-", *(*p+1)))) {
	(*p)++;
    }
    else while(**p && !strchr(" ,()\t\n\r", **p)) {
	(*p)++;
    }
    result = (char*)malloc((*p)-start+1);
    memcpy(result,start,(*p)-start+1);
    result[(*p)-start] = 0;
    return result;
}

void draw_conicTo(drawer_t*draw, FPOINT*  c, FPOINT*  to)
{
    FPOINT* pos = &draw->pos;
    FPOINT c1,c2;
    c1.x = (pos->x + 2 * c->x) / 3;
    c1.y = (pos->y + 2 * c->y) / 3;
    c2.x = (2 * c->x + to->x) / 3;
    c2.y = (2 * c->y + to->y) / 3;
    draw_cubicTo(draw, &c1,&c2,to);

    draw->pos = *to;
}

/* convenience routine */
static void draw_conicTo2(drawer_t*draw, double x1, double y1, double  x2, double y2)
{
    FPOINT c1,c2;
    c1.x = x1;
    c1.y = y1;
    c2.x = x2;
    c2.y = y2;
    draw_conicTo(draw, &c1, &c2);
}
/* convenience routine */
static void draw_moveTo2(drawer_t*draw, double x, double y)
{
    FPOINT c;
    c.x = x; c.y = y;
    draw->moveTo(draw, &c);
}
/* convenience routine */
static void draw_lineTo2(drawer_t*draw, double x, double y)
{
    FPOINT c;
    c.x = x; c.y = y;
    draw->lineTo(draw, &c);
}

static float getFloat(const char** p)
{
    char* token = getToken(p);
    float result = atof(token);
    free(token);
    return result;
}

void draw_string(drawer_t*draw, const char*string)
{
    const char*p = string;
    while(*p) {
	char*token = getToken(&p);
    if(!token)
        break;
    if (!*token) 
    {
        free(token);
	    break;
    }
	if(!strncmp(token, "moveTo", 6) ||
	   !strncmp(token, "M", 1) //svg
	   ) {
	    FPOINT to;
        to.x = getFloat(&p);
        to.y = getFloat(&p);
	    draw->moveTo(draw, &to);
	}
	else if(!strncmp(token, "lineTo", 6) ||
	        !strncmp(token, "L", 1) //svg
	     ) {
	    FPOINT to;
        to.x = getFloat(&p);
        to.y = getFloat(&p);
	    draw->lineTo(draw, &to);
	}
	else if(!strncmp(token, "curveTo", 7) || !strncmp(token, "splineTo", 8)) {
	    FPOINT mid,to;
        mid.x = getFloat(&p);
        mid.y = getFloat(&p);
        to.x = getFloat(&p);
        to.y = getFloat(&p);
	    draw->splineTo(draw, &mid, &to);
	}
	else if(!strncmp(token, "conicTo", 5)) {
	    FPOINT mid,to;
        mid.x = getFloat(&p);
        mid.y = getFloat(&p);
        to.x = getFloat(&p);
        to.y = getFloat(&p);
	    draw_conicTo(draw, &mid, &to);
	}
	else if(!strncmp(token, "circle", 6)) {
	    int mx,my,r;
	    double r2;
        mx = getFloat(&p);
        my = getFloat(&p);
        r = getFloat(&p);
	    r2 = 0.70710678118654757*r;
	    draw_moveTo2(draw, mx, my-r);
	    draw_conicTo2(draw, mx+r2, my-r2, mx+r, my);
	    draw_conicTo2(draw, mx+r2, my+r2, mx, my+r);
	    draw_conicTo2(draw, mx-r2, my+r2, mx-r, my);
	    draw_conicTo2(draw, mx-r2, my-r2, mx, my-r);
	}
	else if(!strncmp(token, "box", 3)) {
	    int x1,y1,x2,y2;
        x1 = getFloat(&p);
        y1 = getFloat(&p);
        x2 = getFloat(&p);
        y2 = getFloat(&p);
	    draw_moveTo2(draw, x1, y1);
	    draw_lineTo2(draw, x1, y2);
	    draw_lineTo2(draw, x2, y2);
	    draw_lineTo2(draw, x2, y1);
	    draw_lineTo2(draw, x1, y1);
	}
	else if(!strncmp(token, "cubicTo", 5) ||
	        !strncmp(token, "C", 1) //svg
		) {
	    FPOINT mid1,mid2,to;
        mid1.x = getFloat(&p);
        mid1.y = getFloat(&p);
        mid2.x = getFloat(&p);
        mid2.y = getFloat(&p);
        to.x = getFloat(&p);
        to.y = getFloat(&p);
	    draw_cubicTo(draw, &mid1, &mid2, &to);
	}
	else if(!strncmp(token, "z", 1) //svg
	       ) {
	    // ignore
	}
	else    
	    fprintf(stderr, "drawer: Warning: unknown primitive '%s'\n", token);
	
	free(token);
    }
}

struct SPLINEPOINT
{
    double x,y;
};

struct qspline
{
    struct SPLINEPOINT start;
    struct SPLINEPOINT control;
    struct SPLINEPOINT end;
};

struct cspline
{
    struct SPLINEPOINT start;
    struct SPLINEPOINT control1;
    struct SPLINEPOINT control2;
    struct SPLINEPOINT end;
};

static inline struct SPLINEPOINT cspline_getpoint(const struct cspline*s, double t)
{
    struct SPLINEPOINT p;
    double tt = t*t;
    double ttt = tt*t;
    double mt = (1-t);
    double mtmt = mt*(1-t);
    double mtmtmt = mtmt*(1-t);
    p.x= s->end.x*ttt + 3*s->control2.x*tt*mt
	    + 3*s->control1.x*t*mtmt + s->start.x*mtmtmt;
    p.y= s->end.y*ttt + 3*s->control2.y*tt*mt
	    + 3*s->control1.y*t*mtmt + s->start.y*mtmtmt;
    return p;
}
static struct SPLINEPOINT qspline_getpoint(const struct qspline*s, double t)
{
    struct SPLINEPOINT p;
    p.x= s->end.x*t*t + 2*s->control.x*t*(1-t) + s->start.x*(1-t)*(1-t);
    p.y= s->end.y*t*t + 2*s->control.y*t*(1-t) + s->start.y*(1-t)*(1-t);
    return p;
}

static int approximate3(const struct cspline*s, struct qspline*q, int size, double quality2)
{
    unsigned int gran = 0;
    unsigned int istep = 0x80000000;
    unsigned int istart = 0;
    int num = 0;
    int level = 0;
    
    while(istart<0x80000000)
    {
	unsigned int iend = istart + istep;
	double start = istart/(double)0x80000000;
	double end = iend/(double)0x80000000;
	struct qspline test;
	double pos,qpos;
	char left = 0,recurse=0;
	int t;
	int probes = 15;

	/* create simple approximation: a qspline which run's through the
	   qspline point at 0.5 */
	test.start = cspline_getpoint(s, start);
	test.control = cspline_getpoint(s, (start+end)/2);
	test.end = cspline_getpoint(s, end);
	/* fix the control point:
	   move it so that the new spline does runs through it */
	test.control.x = -(test.end.x + test.start.x)/2 + 2*(test.control.x);
	test.control.y = -(test.end.y + test.start.y)/2 + 2*(test.control.y);

	/* depending on where we are in the spline, we either try to match
	   the left or right tangent */
	if(start<0.5) 
	    left=1;
	/* get derivative */
	pos = left?start:end;
	qpos = pos*pos;
	test.control.x = s->end.x*(3*qpos) + 3*s->control2.x*(2*pos-3*qpos) + 
		    3*s->control1.x*(1-4*pos+3*qpos) + s->start.x*(-3+6*pos-3*qpos);
	test.control.y = s->end.y*(3*qpos) + 3*s->control2.y*(2*pos-3*qpos) + 
		    3*s->control1.y*(1-4*pos+3*qpos) + s->start.y*(-3+6*pos-3*qpos);
	if(left) {
	    test.control.x *= (end-start)/2;
	    test.control.y *= (end-start)/2;
	    test.control.x += test.start.x;
	    test.control.y += test.start.y;
	} else {
	    test.control.x *= -(end-start)/2;
	    test.control.y *= -(end-start)/2;
	    test.control.x += test.end.x;
	    test.control.y += test.end.y;
	}

#define PROBES
#ifdef PROBES
	/* measure the spline's accurancy, by taking a number of probes */
	for(t=0;t<probes;t++) {
	    struct SPLINEPOINT qr1,qr2,cr1,cr2;
	    double pos = 0.5/(probes*2)*(t*2+1);
	    double dx,dy;
	    double dist1,dist2;
	    qr1 = qspline_getpoint(&test, pos);
	    cr1 = cspline_getpoint(s, start+pos*(end-start));

	    dx = qr1.x - cr1.x;
	    dy = qr1.y - cr1.y;
	    dist1 = dx*dx+dy*dy;

	    if(dist1>quality2) {
		recurse=1;break;
	    }
	    qr2 = qspline_getpoint(&test, (1-pos));
	    cr2 = cspline_getpoint(s, start+(1-pos)*(end-start));

	    dx = qr2.x - cr2.x;
	    dy = qr2.y - cr2.y;
	    dist2 = dx*dx+dy*dy;

	    if(dist2>quality2) {
		recurse=1;break;
	    }
	}
#else // quadratic error: *much* faster!

	/* convert control point representation to 
	   d*x^3 + c*x^2 + b*x + a */
	double dx,dy;
	dx= s->end.x  - s->control2.x*3 + s->control1.x*3 - s->start.x;
	dy= s->end.y  - s->control2.y*3 + s->control1.y*3 - s->start.y;
	
	/* we need to do this for the subspline between [start,end], not [0,1] 
	   as a transformation of t->a*t+b does nothing to highest coefficient
	   of the spline except multiply it with a^3, we just need to modify
	   d here. */
	{double m = end-start;
	 dx*=m*m*m;
	 dy*=m*m*m;
	}
	
	/* use the integral over (f(x)-g(x))^2 between 0 and 1
	   to measure the approximation quality. 
	   (it boils down to const*d^2)
	 */
	recurse = (dx*dx + dy*dy > quality2);
#endif

	if(recurse && istep>1 && size-level > num) {
	    istep >>= 1;
	    level++;
	} else {
	    *q++ = test;
	    num++;
	    istart += istep;
	    while(!(istart & istep)) {
		level--;
		istep <<= 1;
	    }
	}
    }
    return num;
}

void draw_cubicTo(drawer_t*draw, FPOINT*  control1, FPOINT* control2, FPOINT*  to)
{
    struct qspline q[128];
    struct cspline c;
    //double quality = 80;
    double maxerror = 1;//(500-(quality*5)>1?500-(quality*5):1)/20.0;
    int t,num;

    c.start.x = draw->pos.x;
    c.start.y = draw->pos.y;
    c.control1.x = control1->x;
    c.control1.y = control1->y;
    c.control2.x = control2->x;
    c.control2.y = control2->y;
    c.end.x = to->x;
    c.end.y = to->y;
    
    num = approximate3(&c, q, 128, maxerror*maxerror);

    for(t=0;t<num;t++) {
	FPOINT mid;
	FPOINT to;
	mid.x = q[t].control.x;
	mid.y = q[t].control.y;
	to.x = q[t].end.x;
	to.y = q[t].end.y;
	draw->splineTo(draw, &mid, &to);
    }
}


