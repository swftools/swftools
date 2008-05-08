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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "gfxtools.h"
#include "gfxfont.h"

typedef struct _linedraw_internal
{
    gfxline_t*start;
    gfxline_t*next;
} linedraw_internal_t;

static void linedraw_moveTo(gfxdrawer_t*d, gfxcoord_t x, gfxcoord_t y)
{
    linedraw_internal_t*i = (linedraw_internal_t*)d->internal;
    gfxline_t*l = (gfxline_t*)rfx_alloc(sizeof(gfxline_t));
    l->type = gfx_moveTo;
    if((int)((d->x * 5120) == (int)(x * 5120)) &&
       (int)((d->y * 5120) == (int)(y * 5120))) {
	/* never mind- we're already there */
	return;

    }
    l->sx = l->sy = 0;
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
    gfxline_t*l = (gfxline_t*)rfx_alloc(sizeof(gfxline_t));

    if(!i->start) {
	/* starts with a line, not with a moveto. That needs we first
	   need an explicit moveto to (0,0) */
	linedraw_moveTo(d, 0, 0);
    }

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
    gfxline_t*l = (gfxline_t*)rfx_alloc(sizeof(gfxline_t));

    if(!i->start) {
	/* starts with a line, not with a moveto. That needs we first
	   need an explicit moveto to (0,0) */
	linedraw_moveTo(d, 0, 0);
    }

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
    d->x = 0x7fffffff;
    d->y = 0x7fffffff;
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
    int parts = (int)(sqrt(fabs(s->ax) + fabs(s->ay))*3);
    int i;
    double len = 0;
    double r;
    double r2;
    if(parts < 3) parts = 3;
    r = 1.0/parts;
    r2 = 1.0/(parts*parts);
    for(i=0;i<parts;i++)
    {
	double dx = s->ax*(2*i+1)*r2 + s->bx*r;
	double dy = s->ay*(2*i+1)*r2 + s->by*r;
	len += sqrt(dx*dx+dy*dy);
    }
    /*printf("Spline from %f,%f to %f,%f has len %f (%f)\n", s->cx, s->cy, 
	    s->cx + s->bx + s->ax,
	    s->cy + s->by + s->ay, len,
	    sqrt((s->bx + s->ax)*(s->bx + s->ax) + (s->by + s->ay)*(s->by + s->ay))
	    );
    assert(len+0.5 >= sqrt((s->bx + s->ax)*(s->bx + s->ax) + (s->by + s->ay)*(s->by + s->ay)));
     */
    return len;
}

void gfxtool_draw_dashed_line(gfxdrawer_t*d, gfxline_t*line, float*r, float phase)
{
    double x=0,y=0;
    double linepos,nextpos;
    char on;
    int apos=0;

    if(line && line->type != gfx_moveTo) {
	fprintf(stderr, "gfxtool: outline doesn't start with a moveTo");
	return;
    }
    if(!r || (r[0]<=0 && r[0]>-0.01)) {
        // no dashing. just draw the thing
        while(line) {
            if(line->type == gfx_moveTo) {
		d->moveTo(d, line->x, line->y);
            } else if(line->type == gfx_lineTo) {
		d->lineTo(d, line->x, line->y);
            } else if(line->type == gfx_splineTo) {
		d->splineTo(d, line->sx, line->sy, line->x, line->y);
            }
            line = line->next;
        }
        return;
    }
    if(r[0]<0 || phase<0) {
	fprintf(stderr, "gfxtool: invalid (negative) dashes: %f, phase=%f", r[0], phase);
	return;
    }


    for(;line;line=line->next) {
	if(line->type == gfx_moveTo) {
	    d->moveTo(d, line->x, line->y);
	    on = 1; nextpos = r[0]; apos = 0; linepos = 0;
	    x = line->x; y = line->y;
	    while(linepos < phase) {
		//printf("[+] linepos: %f, phase: %f, on:%d, apos:%d nextpos:%f\n", linepos, phase, on, apos, nextpos);
		linepos += r[apos];
		if(linepos < phase) {
		    on ^= 1;
		    if(r[++apos]<0)
			apos = 0;
		    nextpos += r[apos];
		}
	    }
	    linepos = phase;
	    //printf("[k] linepos: %f, phase: %f, on:%d, apos:%d nextpos:%f \n", linepos, phase, on, apos, nextpos);
	} else if(line->type == gfx_lineTo) {
	    double dx = line->x - x;
	    double dy = line->y - y;
	    double len = sqrt(dx*dx+dy*dy);
	    double vx;
	    double vy;
	    double lineend = linepos+len;
	    if(len==0)
		continue;
	    vx = dx/len;
	    vy = dy/len;
	    assert(nextpos>=linepos);
	    //printf("(line) on:%d apos: %d nextpos: %f, line pos: %f, line end: %f\n", on, apos, nextpos, linepos, linepos+len);
	    while(nextpos<lineend) {
		double nx = x + vx*(nextpos-linepos);
		double ny = y + vy*(nextpos-linepos);
		if(on) {d->lineTo(d, nx,ny);/*printf("lineTo %f\n", nextpos);*/}
		else   {d->moveTo(d, nx,ny);/*printf("moveTo %f\n", nextpos);*/}
		on^=1;
		if(r[++apos]<0)
		    apos = 0;
		nextpos+=r[apos];
	    }
	    linepos = lineend;
	    if(on) {
		//printf("lineTo %f\n", 1.0);
		d->lineTo(d, line->x,line->y);
	    }
	    x = line->x; y = line->y;
	} else if(line->type == gfx_splineTo) {
	    qspline_abc_t q;
	    double len, lineend,lastt;
	    mkspline(&q, x, y, line);

	    len = get_spline_len(&q);
	    //printf("%f %f -> %f %f, len: %f\n", x, y, line->x, line->y, len);
	    if(len==0)
		continue;
	    lineend = linepos+len;
	    lastt = 0;
	    if(nextpos<linepos)
		printf("%f !< %f\n", nextpos, linepos);
	    assert(nextpos>=linepos);
	    //printf("(spline) on:%d apos: %d nextpos: %f, line pos: %f, line end: %f\n", on, apos, nextpos, linepos, linepos+len);
	    while(nextpos<lineend) {
		double t = (nextpos-linepos)/len;
		//printf("%f (%f-%f) apos=%d r[apos]=%f\n", t, nextpos, linepos, apos, r[apos]);
		double nx = q.ax*t*t+q.bx*t+q.cx;
		double ny = q.ay*t*t+q.by*t+q.cy;
		if(on) {
		    double sx,sy;
		    spline_get_controlpoint(&q, lastt, t, &sx, &sy);
		    d->splineTo(d, sx, sy, nx,ny);
		    //printf("splineTo %f\n", nextpos);
		} else  {
		    d->moveTo(d, nx,ny);
		    //printf("moveTo %f\n", nextpos);
		}
		lastt =  t;
		on^=1;
		if(r[++apos]<0)
		    apos = 0;
		nextpos+=r[apos];
	    }
	    linepos = lineend;
	    if(on) {
		double sx,sy;
		spline_get_controlpoint(&q, lastt, 1, &sx, &sy);
		d->splineTo(d, sx, sy, line->x,line->y);
		//printf("splineTo %f\n", 1.0);
	    }
	    x = line->x; y = line->y;
	}
    }
}

gfxline_t * gfxline_clone(gfxline_t*line)
{
    gfxline_t*dest = 0;
    gfxline_t*pos = 0;
    while(line) {
	gfxline_t*n = (gfxline_t*)rfx_calloc(sizeof(gfxline_t));
	*n = *line;
	n->next = 0;
	if(!pos) {
	    dest = pos = n;
	} else {
	    pos->next = n;
	    pos = n;
	}
	line = line->next;
    }
    return dest;
}
void gfxline_optimize(gfxline_t*line)
{
    gfxline_t*l = line;
    /* step 1: convert splines to lines, where possible */
    double x=0,y=0;
    while(l) {
	if(l->type == gfx_splineTo) {
	    double dx = l->x-x;
	    double dy = l->y-y;
	    double sx = l->sx-x;
	    double sy = l->sy-y;
	    if(fabs(dx*sy - dy*sx) < 0.000001 && (dx*sx + dy*sy) >= 0) {
		l->type = gfx_lineTo;
	    }
	}
	x = l->x;
	y = l->y;
	l = l->next;
    }
    /* step 2: combine adjacent lines and splines, where possible */
    l = line;
    while(l && l->next) {
	gfxline_t*next = l->next;
	char combine = 0;
	double sx=0,sy=0;
	if(l->type == gfx_lineTo && next->type == gfx_lineTo) {
	    double dx = l->x-x;
	    double dy = l->y-y;
	    double nx = next->x-l->x;
	    double ny = next->y-l->y;
	    if(fabs(dx*ny - dy*nx) < 0.000001 && (dx*nx + dy*ny) >= 0) {
		combine = 1;
	    }
	} else if(l->type == gfx_splineTo && next->type == gfx_splineTo) {
	    /* TODO */
	}
	if(combine) {
	    l->next = next->next;
	    next->next = 0;
	    l->x = next->x;
	    l->y = next->y;
	    l->sx = sx;
	    l->sy = sy;
	    rfx_free(next);
	} else {
	    x = l->x;
	    y = l->y;
	    l = l->next;
	}
    }
}

gfxline_t* gfxtool_dash_line(gfxline_t*line, float*dashes, float phase)
{
    gfxdrawer_t d;
    gfxline_t*result;
    gfxdrawer_target_gfxline(&d);
    gfxtool_draw_dashed_line(&d, line, dashes, phase);
    result= (gfxline_t*)d.result(&d);
    return result;
}

void gfxline_show(gfxline_t*l, FILE*fi)
{
    while(l) {
	if(l->type == gfx_moveTo) {
	    fprintf(fi, "moveTo %.2f,%.2f\n", l->x, l->y);
	}
	if(l->type == gfx_lineTo) {
	    fprintf(fi, "lineTo %.2f,%.2f\n", l->x, l->y);
	}
	if(l->type == gfx_splineTo) {
	    fprintf(fi, "splineTo %.2f,%.2f %.2f,%.2f\n", l->sx, l->sy, l->x, l->y);
	}
	l = l->next;
    }
}

void gfxline_free(gfxline_t*l)
{
    if(l && (l+1) == l->next) {
	/* flattened */
	rfx_free(l);
    } else {
	gfxline_t*next;
	while(l) {
	    next = l->next;
	    l->next = 0;
	    rfx_free(l);
	    l = next;
	}
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

//#define PROBES
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
	   (it boils down to const*d^2) */
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

void gfxdraw_conicTo(gfxdrawer_t*draw, double cx, double cy, double tox, double toy, double quality)
{
    double c1x = (draw->x + 2 * cx) / 3;
    double c1y = (draw->y + 2 * cy) / 3;
    double c2x = (2 * cx + tox) / 3;
    double c2y = (2 * cy + toy) / 3;
    gfxdraw_cubicTo(draw, c1x, c1y, c2x, c2y, tox, toy, quality);
}


void gfxdraw_cubicTo(gfxdrawer_t*draw, double c1x, double c1y, double c2x, double c2y, double x, double y, double quality)
{
    qspline_t q[128];
    cspline_t c;
    double maxerror = quality>0 ? quality : 1.0;
    int t,num;

    c.start.x = draw->x;
    c.start.y = draw->y;
    c.control1.x = c1x;
    c.control1.y = c1y;
    c.control2.x = c2x;
    c.control2.y = c2y;
    c.end.x = x;
    c.end.y = y;
    
    num = approximate3(&c, q, 128, maxerror);

    for(t=0;t<num;t++) {
	gfxpoint_t mid;
	gfxpoint_t to;
	mid.x = q[t].control.x;
	mid.y = q[t].control.y;
	to.x = q[t].end.x;
	to.y = q[t].end.y;
	draw->splineTo(draw, mid.x, mid.y, to.x, to.y);
    }
}

gfxbbox_t gfxbbox_expand_to_point(gfxbbox_t box, gfxcoord_t x, gfxcoord_t y)
{
    if(box.xmin==0 && box.ymin==0 && box.xmax==0 && box.ymax==0) {
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

void gfxbbox_intersect(gfxbbox_t*box1, gfxbbox_t*box2)
{
    if(box2->xmin > box1->xmin)
	box1->xmin = box2->xmin;
    if(box2->ymin > box1->ymin)
	box1->ymin = box2->ymin;
    if(box2->xmax < box1->xmax)
	box1->xmax = box2->xmax;
    if(box2->ymax > box1->ymax)
	box1->ymax = box2->ymax;
    if(box1->xmin > box1->xmax)
	box1->xmax = box1->xmin;
    if(box1->ymin > box1->ymax)
	box1->ymax = box1->ymin;
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
	y = line->y;
	line = line->next;
    }
    return bbox;
}

void gfxline_dump(gfxline_t*line, FILE*fi, char*prefix)
{
    while(line) {
	if(line->type == gfx_moveTo) {
	    fprintf(fi, "%smoveTo %.2f %.2f\n", prefix, line->x, line->y);
	} else if(line->type == gfx_lineTo) {
	    fprintf(fi, "%slineTo %.2f %.2f\n", prefix, line->x, line->y);
	} else if(line->type == gfx_splineTo) {
	    fprintf(fi, "%ssplineTo (%.2f %.2f) %.2f %.2f\n", prefix, line->sx, line->sy, line->x, line->y);
	}
	line = line->next;
    }
}

gfxline_t* gfxline_append(gfxline_t*line1, gfxline_t*line2)
{
    gfxline_t*l = line1;;
    if(!l)
	return line2;
    while(l->next) {
	l = l->next;
    }
    l->next = line2;
    return line1;
}

void gfxline_transform(gfxline_t*line, gfxmatrix_t*matrix)
{
    while(line) {
	double x = matrix->m00*line->x + matrix->m10*line->y + matrix->tx;
	double y = matrix->m01*line->x + matrix->m11*line->y + matrix->ty;
	line->x = x;
	line->y = y;
	if(line->type == gfx_splineTo) {
	    double sx = matrix->m00*line->sx + matrix->m10*line->sy + matrix->tx;
	    double sy = matrix->m01*line->sx + matrix->m11*line->sy + matrix->ty;
	    line->sx = sx;
	    line->sy = sy;
	}
	line = line->next;
    }
}

void gfxmatrix_dump(gfxmatrix_t*m, FILE*fi, char*prefix)
{
    fprintf(fi, "%f %f | %f\n", m->m00, m->m10, m->tx);
    fprintf(fi, "%f %f | %f\n", m->m01, m->m11, m->ty);
}

void gfxmatrix_transform(gfxmatrix_t*m, double* v, double*dest)
{
    dest[0] = m->m00*v[0] + m->m10*v[1] + m->tx;
    dest[1] = m->m01*v[0] + m->m11*v[1] + m->ty;
}
void gfxmatrix_invert(gfxmatrix_t*m, gfxmatrix_t*dest)
{
    double det = m->m00 * m->m11 - m->m10 * m->m01;
    if(!det) {
	memset(dest, 0, sizeof(gfxmatrix_t));
	return;
    }
    det = 1/det;
    dest->m00 = m->m11 * det;
    dest->m01 = -m->m01 * det;
    dest->m10 = -m->m10 * det;
    dest->m11 = m->m00 * det;
    dest->tx = -(dest->m00 * m->tx + dest->m10 * m->ty);
    dest->ty = -(dest->m01 * m->tx + dest->m11 * m->ty);
}
void gfxmatrix_unit(gfxmatrix_t*m)
{
    memset(m, 0, sizeof(gfxmatrix_t));
    m->m00 = 1.0;
    m->m11 = 1.0;
}
void gfxmatrix_multiply(gfxmatrix_t*m1, gfxmatrix_t*m2, gfxmatrix_t*dest)
{
    dest->m00 = m1->m00*m2->m00 + m1->m10*m2->m01;
    dest->m01 = m1->m01*m2->m00 + m1->m11*m2->m01;
    dest->m10 = m1->m00*m2->m10 + m1->m10*m2->m11;
    dest->m11 = m1->m01*m2->m10 + m1->m11*m2->m11;
    dest->tx = m1->m00*m2->tx + m1->m10*m2->ty + m1->tx;
    dest->ty = m1->m01*m2->tx + m1->m11*m2->ty + m1->ty;
}

gfxfontlist_t* gfxfontlist_create()
{
    /* Initial list ist empty */
    return 0;
}

gfxfont_t*gfxfontlist_findfont(gfxfontlist_t*list, char*id)
{
    gfxfontlist_t*l = list;
    while(l) {
	if(!strcmp((char*)l->font->id, id)) {
	    return l->font;
	}
	l = l->next;
    }
    return 0;
}
char gfxfontlist_hasfont(gfxfontlist_t*list, gfxfont_t*font)
{
    gfxfontlist_t*l = list;
    while(l) {
	if(!strcmp((char*)l->font->id, font->id)) {
	    return 1;
	}
	l = l->next;
    }
    return 0;
}
gfxfontlist_t*gfxfontlist_addfont(gfxfontlist_t*list, gfxfont_t*font)
{
    gfxfontlist_t*last=0,*l = list;
    while(l) {
	last = l;
	if(l->font == font) {
	    return list; // we already know this font
	}
	l = l->next;
    }
    if(!font) {
	fprintf(stderr, "Tried to add zero font\n");
    }
    l = (gfxfontlist_t*)rfx_calloc(sizeof(gfxfontlist_t));
    l->font = font;
    l->next = 0;
    if(last) {
	last->next = l;
	return list;
    } else {
	return l;
    }
}
void gfxfontlist_free(gfxfontlist_t*list, char deletefonts)
{
    gfxfontlist_t*l = list;
    while(l) {
	gfxfontlist_t*next = l->next;
	if(deletefonts && l->font) {
	    gfxfont_free(l->font);l->font=0;
	}
	l->next = 0;
	free(l);
	l = next;
    }
}

gfxline_t*gfxline_makerectangle(int x1,int y1,int x2, int y2)
{
    gfxline_t* line = (gfxline_t*)rfx_calloc(sizeof(gfxline_t)*5);
    line[0].x = x1;line[0].y = y1;line[0].type = gfx_moveTo;line[0].next = &line[1];
    line[1].x = x2;line[1].y = y1;line[1].type = gfx_lineTo;line[1].next = &line[2];
    line[2].x = x2;line[2].y = y2;line[2].type = gfx_lineTo;line[2].next = &line[3];
    line[3].x = x1;line[3].y = y2;line[3].type = gfx_lineTo;line[3].next = &line[4];
    line[4].x = x1;line[4].y = y1;line[4].type = gfx_lineTo;
    return line;
}

void gfximage_transform(gfximage_t*img, gfxcxform_t*cxform)
{
    int t;
    int size = img->width*img->height;

    int rr,rg,rb,ra, tr;
    int gr,gg,gb,ga, tg;
    int br,bg,bb,ba, tb;
    int ar,ag,ab,aa, ta;
    rr = (int)(cxform->rr*256);gr = (int)(cxform->gr*256);
    rg = (int)(cxform->rg*256);gg = (int)(cxform->gg*256);
    rb = (int)(cxform->rb*256);gb = (int)(cxform->gb*256);
    ra = (int)(cxform->ra*256);ga = (int)(cxform->ga*256);
    br = (int)(cxform->br*256);ar = (int)(cxform->ar*256);tr = (int)(cxform->tr*256);
    bg = (int)(cxform->bg*256);ag = (int)(cxform->ag*256);tg = (int)(cxform->tg*256);
    bb = (int)(cxform->bb*256);ab = (int)(cxform->ab*256);tb = (int)(cxform->tb*256);
    ba = (int)(cxform->ba*256);aa = (int)(cxform->aa*256);ta = (int)(cxform->ta*256);

    for(t=0;t<size;t++) {
	gfxcolor_t*pixel = &img->data[t];
	unsigned char r = (pixel->r * rr + pixel->g * rg + pixel->b * rb + pixel->a * ra + tr) / 256;
	unsigned char g = (pixel->r * gr + pixel->g * gg + pixel->b * gb + pixel->a * ga + tg) / 256;
	unsigned char b = (pixel->r * br + pixel->g * bg + pixel->b * bb + pixel->a * ba + tb) / 256;
	unsigned char a = (pixel->r * ar + pixel->g * ag + pixel->b * ab + pixel->a * aa + ta) / 256;
	pixel->r = r;
	pixel->g = g;
	pixel->b = b;
	pixel->a = a;
    }
}
