/* gfxtools.c 

   Various utility functions for dealing with gfxdevices.

   Part of the swftools package.

   Copyright (c) 2005 Matthias Kramm <kramm@quiss.org> 

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

#include <memory.h>
#include <math.h>
#include <assert.h>
#include "gfxtools.h"

typedef struct _linedraw_internal
{
    gfxline_t*start;
    gfxline_t*next;
} linedraw_internal_t;

static void linedraw_moveTo(gfxdrawer_t*d, gfxcoord_t x, gfxcoord_t y)
{
    linedraw_internal_t*i = (linedraw_internal_t*)d->internal;
    gfxline_t*l = rfx_alloc(sizeof(gfxline_t));
    l->type = gfx_moveTo;
    d->x = l->x = x;
    d->y = l->y = y;
    l->next = 0;
    if(i->next)
	i->next->next = l;
    i->next = l;
    if(!i->start)
	i->start = l;
}
static void linedraw_lineTo(gfxdrawer_t*d, gfxcoord_t x, gfxcoord_t y)
{
    linedraw_internal_t*i = (linedraw_internal_t*)d->internal;
    gfxline_t*l = rfx_alloc(sizeof(gfxline_t));
    l->type = gfx_lineTo;
    d->x = l->x = x;
    d->y = l->y = y;
    l->next = 0;
    if(i->next)
	i->next->next = l;
    i->next = l;
    if(!i->start)
	i->start = l;
}
static void linedraw_splineTo(gfxdrawer_t*d, gfxcoord_t sx, gfxcoord_t sy, gfxcoord_t x, gfxcoord_t y)
{
    linedraw_internal_t*i = (linedraw_internal_t*)d->internal;
    gfxline_t*l = rfx_alloc(sizeof(gfxline_t));
    l->type = gfx_splineTo;
    d->x = l->x = x; 
    d->y = l->y = y;
    l->sx = sx; 
    l->sy = sy;
    l->next = 0;
    if(i->next)
	i->next->next = l;
    i->next = l;
    if(!i->start)
	i->start = l;
}
static void* linedraw_result(gfxdrawer_t*d)
{
    linedraw_internal_t*i = (linedraw_internal_t*)d->internal;
    void*result = (void*)i->start;
    rfx_free(i);
    memset(d, 0, sizeof(gfxdrawer_t));
    return result;
}

void gfxdrawer_target_gfxline(gfxdrawer_t*d)
{
    linedraw_internal_t*i = (linedraw_internal_t*)rfx_calloc(sizeof(linedraw_internal_t));
    d->internal = i;
    d->moveTo = linedraw_moveTo;
    d->lineTo = linedraw_lineTo;
    d->splineTo = linedraw_splineTo;
    d->result = linedraw_result;
}

typedef struct _qspline_abc
{
    double ax,bx,cx;
    double ay,by,cy;
} qspline_abc_t;

typedef struct qspline_t
{
    gfxpoint_t start;
    gfxpoint_t control;
    gfxpoint_t end;
} qspline_t;

typedef struct cspline_t
{
    gfxpoint_t start;
    gfxpoint_t control1;
    gfxpoint_t control2;
    gfxpoint_t end;
} cspline_t;

static void mkspline(qspline_abc_t*s, double x, double y, gfxline_t*l)
{
    /* 
       Form 1: x = t*t*l->x + 2*t*(1-t)*l->sx + (1-t)*(1-t)*x;
       Form 2: x = a*t*t + b*t + c
    */
    s->cx = x; s->bx = 2*l->sx - 2*x; s->ax = l->x - 2*l->sx + x;
    s->cy = y; s->by = 2*l->sy - 2*y; s->ay = l->y - 2*l->sy + y;
}

static void spline_get_controlpoint(qspline_abc_t*q, double t1, double t2, double*dx, double*dy)
{
    double dt = t2-t1;
    double nax = q->ax*dt*dt;
    double nay = q->ay*dt*dt;
    double nbx = 2*q->ax*dt*t1 + q->bx*dt;
    double nby = 2*q->ay*dt*t1 + q->by*dt;
    double ncx = q->ax*t1*t1 + q->bx*t1 + q->cx;
    double ncy = q->ay*t1*t1 + q->by*t1 + q->cy;
    *dx = ncx + nbx/2;
    *dy = ncy + nby/2;
}

static double get_spline_len(qspline_abc_t*s)
{
    //int parts = (int)(sqrt(abs(l2->x-2*l2->sx+x) + abs(l2->y-2*l2->sy+y)/3));
    int i;
    double len = 0;
    for(i=0;i<128;i++)
    {
	double t = i*(1/128.0);
	double dx = 2*s->ax*t + s->bx;
	double dy = 2*s->ay*t + s->by;
	len += sqrt(dx*dx+dy*dy);
    }
    return len / 128;
}

void gfxtool_draw_dashed_line(gfxdrawer_t*d, gfxline_t*line, float*r)
{
    double x=0,y=0;
    int apos = 0;
    double linepos = 0;
    double nextpos = 0;
    char on = 1;
    for(;line;line=line->next) {
	if(line->type == gfx_moveTo) {
	    d->moveTo(d, line->x, line->y);
	    apos = 0; nextpos = 0; on = 1; linepos = 0;
	    x = line->x; y = line->y;
	} else if(line->type == gfx_lineTo) {
	    double dx = line->x - x;
	    double dy = line->y - y;
	    double len = sqrt(dx*dx+dy*dy);
	    if(len==0)
		continue;
	    double vx = dx/len;
	    double vy = dy/len;
	    double lineend = linepos+len;
	    assert(nextpos>=linepos);
	    //printf("nextpos: %f, line end: %f\n", nextpos, linepos+len);
	    while(nextpos<lineend) {
		double nx = x + vx*(nextpos-linepos);
		double ny = y + vy*(nextpos-linepos);
		if(on) d->lineTo(d, nx,ny);
		else   d->moveTo(d, nx,ny);
		on^=1;
		nextpos+=r[apos];
		apos++;
		if(r[apos]==0)
		    apos = 1;
	    }
	    linepos = lineend;
	    if(on) {
		d->lineTo(d, line->x,line->y);
	    }
	    x = line->x; y = line->y;
	} else if(line->type == gfx_splineTo) {
	    qspline_abc_t q;
	    mkspline(&q, x, y, line);

	    double len = get_spline_len(&q);
	    //printf("%f %f -> %f %f, len: %f\n", x, y, line->x, line->y, len);
	    if(len==0)
		continue;
	    double lineend = linepos+len;
	    double lastt = 0;
	    if(nextpos<linepos)
		printf("%f !< %f\n", nextpos, linepos);
	    assert(nextpos>=linepos);
	    while(nextpos<lineend) {
		double t = (nextpos-linepos)/len;
		double nx = q.ax*t*t+q.bx*t+q.cx;
		double ny = q.ay*t*t+q.by*t+q.cy;
		if(on) {
		    double sx,sy;
		    spline_get_controlpoint(&q, lastt, t, &sx, &sy);
		    d->splineTo(d, sx, sy, nx,ny);
		} else  {
		    d->moveTo(d, nx,ny);
		}
		lastt =  t;
		on^=1;
		nextpos+=r[apos];
		apos++;
		if(r[apos]==0)
		    apos = 1;
	    }
	    linepos = lineend;
	    if(on) {
		double sx,sy;
		spline_get_controlpoint(&q, lastt, 1, &sx, &sy);
		d->splineTo(d, sx, sy, line->x,line->y);
	    }
	    x = line->x; y = line->y;
	}
    }
}

gfxline_t* gfxtool_dash_line(gfxline_t*line, float*dashes)
{
    gfxdrawer_t d;
    gfxdrawer_target_gfxline(&d);
    gfxtool_draw_dashed_line(&d, line, dashes);
    gfxline_t*result= (gfxline_t*)d.result(&d);
    return result;
}

void gfxline_show(gfxline_t*l, FILE*fi)
{
    while(l) {
	if(l->type == moveTo) {
	    fprintf(fi, "moveTo %.2f,%.2f\n", l->x, l->y);
	}
	if(l->type == lineTo) {
	    fprintf(fi, "lineTo %.2f,%.2f\n", l->x, l->y);
	}
	if(l->type == splineTo) {
	    fprintf(fi, "splineTo %.2f,%.2f %.2f,%.2f\n", l->sx, l->sy, l->x, l->y);
	}
	l = l->next;
    }
}

void gfxline_free(gfxline_t*l)
{
    gfxline_t*next;
    while(l) {
	next = l->next;
	l->next = 0;
	free(l);
	l = next;
    }
}

static inline gfxpoint_t cspline_getpoint(const struct cspline_t*s, double t)
{
    gfxpoint_t p;
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
static gfxpoint_t qspline_getpoint(const qspline_t*s, double t)
{
    gfxpoint_t p;
    p.x= s->end.x*t*t + 2*s->control.x*t*(1-t) + s->start.x*(1-t)*(1-t);
    p.y= s->end.y*t*t + 2*s->control.y*t*(1-t) + s->start.y*(1-t)*(1-t);
    return p;
}

static int approximate3(const cspline_t*s, qspline_t*q, int size, double quality2)
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
	qspline_t test;
	double pos,qpos;
	char left = 0,recurse=0;
	int t;
	int probes = 15;
	double dx,dy;

	/* create simple approximation: a qspline_t which run's through the
	   qspline_t point at 0.5 */
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
	    gfxpoint_t qr1,qr2,cr1,cr2;
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

	/* FIXME: we need to do this for the subspline between [start,end],
	   not [0,1] */
	dx= s->end.x  - s->control2.x*3 + s->control1.x*3 - s->start.x;
	dy= s->end.y  - s->control2.y*3 + s->control1.y*3 - s->start.y;
	
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

void gfxdraw_cubicTo(gfxdrawer_t*draw, double c1x, double c1y, double c2x, double c2y, double x, double y)
{
    qspline_t q[128];
    cspline_t c;
    double maxerror = 0.04;
    int t,num;

    c.start.x = draw->x;
    c.start.y = draw->y;
    c.control1.x = c1x;
    c.control1.y = c1y;
    c.control2.x = c2x;
    c.control2.y = c2y;
    c.end.x = x;
    c.end.y = y;
    
    num = approximate3(&c, q, 128, maxerror*maxerror);

    for(t=0;t<num;t++) {
	FPOINT mid;
	FPOINT to;
	mid.x = q[t].control.x;
	mid.y = q[t].control.y;
	to.x = q[t].end.x;
	to.y = q[t].end.y;
	draw->splineTo(draw, mid.x, mid.y, to.x, to.y);
    }
}

gfxbbox_t gfxbbox_expand_to_point(gfxbbox_t box, gfxcoord_t x, gfxcoord_t y)
{
    if(box.xmin==0 || box.ymin==0 || box.xmax==0 || box.ymax==0) {
	box.xmin = x;
	box.ymin = y;
	box.xmax = x;
	box.ymax = y;
	if(x==0 && y==0) box.xmax = 0.0000001;
	return box;
    }
    if(x < box.xmin)
	box.xmin = x;
    if(x > box.xmax)
	box.xmax = x;
    if(y < box.ymin)
	box.ymin = y;
    if(y > box.ymax)
	box.ymax = y;
    return box;
}

gfxbbox_t gfxline_getbbox(gfxline_t*line)
{
    gfxcoord_t x=0,y=0;
    gfxbbox_t bbox = {0,0,0,0};
    char last = 0;
    while(line) {
	if(line->type == gfx_moveTo) {
	    last = 1;
	} else if(line->type == gfx_lineTo) {
	    if(last) bbox = gfxbbox_expand_to_point(bbox, x, y);
	    bbox = gfxbbox_expand_to_point(bbox, line->x, line->y);
	    last = 0;
	} else if(line->type == gfx_splineTo) {
	    if(last) bbox = gfxbbox_expand_to_point(bbox, x, y);
	    bbox = gfxbbox_expand_to_point(bbox, line->sx, line->sy);
	    bbox = gfxbbox_expand_to_point(bbox, line->x, line->y);
	    last = 0;
	}
	x = line->x;
	y = line->x;
	line = line->next;
    }
    return bbox;
}

