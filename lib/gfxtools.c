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
#include <ctype.h>
#include "gfxtools.h"
#include "gfxfont.h"
#include "jpeg.h"
#include "q.h"

typedef struct _linedraw_internal
{
    gfxline_t*start;
    gfxline_t*next;
    gfxcoord_t x0,y0;
    char has_moveto;
} linedraw_internal_t;

static void linedraw_moveTo(gfxdrawer_t*d, gfxcoord_t x, gfxcoord_t y)
{
    linedraw_internal_t*i = (linedraw_internal_t*)d->internal;
    gfxline_t*l = (gfxline_t*)rfx_alloc(sizeof(gfxline_t));
    l->type = gfx_moveTo;
    i->has_moveto = 1;
    i->x0 = x;
    i->y0 = y;
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
    if(!i->has_moveto) {
	/* starts with a line, not with a moveto. As this is the first
	   entry in the list, this is probably *meant* to be a moveto */
	linedraw_moveTo(d, x, y);
	return;
    }
    
    gfxline_t*l = (gfxline_t*)rfx_alloc(sizeof(gfxline_t));
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
    if(!i->has_moveto) {
	linedraw_moveTo(d, x, y);
	return;
    }

    gfxline_t*l = (gfxline_t*)rfx_alloc(sizeof(gfxline_t));
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
static void linedraw_close(gfxdrawer_t*d)
{
    linedraw_internal_t*i = (linedraw_internal_t*)d->internal;
    if(!i->has_moveto) 
	return;
    linedraw_lineTo(d, i->x0, i->y0);
    i->has_moveto = 0;
    i->x0 = 0;
    i->y0 = 0;
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
    d->close = linedraw_close;
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
    double linepos = 0,nextpos = 0;
    char on = 0;
    int apos=0;

    if(line && line->type != gfx_moveTo) {
	fprintf(stderr, "gfxtool: outline doesn't start with a moveTo");
	return;
    }
    
    int i;
    double dashlen=0;
    for(i=0;r[i]>=0;i++) {
        dashlen+=r[i];
    }
    if(!r || (r[0]<=0 && r[0]>-0.01) || dashlen<0.001) {
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

    if(phase < 0) {
	phase = -phase;
    }

    if(r[0]<0 || phase<0) {
	fprintf(stderr, "gfxtool: invalid (negative) dashes: %f, phase=%f\n", r[0], phase);
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

static char* getToken(const char**p)
{
    const char*start;
    char*result;
    while(**p && strchr(" ,()\t\n\r", **p)) {
	(*p)++;
    } 
    start = *p;
    if (strchr("LMlm", **p) && (isdigit(*(*p+1))||strchr("+-", *(*p+1)))) {
	(*p)++;
    } else while(**p && !strchr(" ,()\t\n\r", **p)) {
	(*p)++;
    }
    result = (char*)malloc((*p)-start+1);
    memcpy(result,start,(*p)-start+1);
    result[(*p)-start] = 0;
    return result;
}

static float getFloat(const char** p)
{
    char* token = getToken(p);
    float result = atof(token);
    free(token);
    return result;
}

gfxline_t*gfxline_fromstring(const char*string)
{
    gfxdrawer_t d;
    gfxdrawer_target_gfxline(&d);

    const char*p = string;
    while(*p) {
	char*token = getToken(&p);
	if(!token)
	    break;
	if (!*token) {
	    free(token);
	    break;
	}
	if(!strcmp(token, "M")) {
	    double x = getFloat(&p);
	    double y = getFloat(&p);
	    d.moveTo(&d, x, y);
	} else if(!strncmp(token, "L", 1)) {
	    double x = getFloat(&p);
	    double y = getFloat(&p);
	    d.lineTo(&d, x, y);
	} else if(!strncmp(token, "C", 1)) {
	    double x1 = getFloat(&p);
	    double y1 = getFloat(&p);
	    double x2 = getFloat(&p);
	    double y2 = getFloat(&p);
	    double x3 = getFloat(&p);
	    double y3 = getFloat(&p);
	    gfxdraw_cubicTo(&d, x1,y1, x2,y2, x3,y3, 0.9);
	} else if(!strncmp(token, "z", 1)) {
	    //ignore
	} else    
	    fprintf(stderr, "gfxdraw: Warning: unknown primitive '%s'\n", token);
	free(token);
    }
    gfxline_t*line = d.result(&d);
    return line;
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

static char splineIsStraight(double x, double y, gfxline_t*l)
{
    if(l->type == gfx_moveTo)
        return 0;
    if(l->type == gfx_lineTo)
        return 1;
    double dx = l->x-x;
    double dy = l->y-y;
    double sx = l->sx-x;
    double sy = l->sy-y;
    if(fabs(dx*sy - dy*sx) < 0.000001 && (dx*sx + dy*sy) >= 0) {
        return 1;
    }
    return 0;
}

void gfxline_optimize(gfxline_t*line)
{
    gfxline_t*l = line;
    /* step 1: convert splines to lines, where possible */
    double x=0,y=0;
    while(l) {
	if(l->type == gfx_splineTo && splineIsStraight(x,y,l)) {
	    l->type = gfx_lineTo;
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

gfxbbox_t gfxbbox_expand_to_bbox(gfxbbox_t box, gfxbbox_t box2)
{
    box = gfxbbox_expand_to_point(box, box2.xmin, box2.ymin);
    box = gfxbbox_expand_to_point(box, box2.xmax, box2.ymax);
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
    if(box2->ymax < box1->ymax)
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
    fprintf(fi, "%s%f %f | %f\n", prefix, m->m00, m->m10, m->tx);
    fprintf(fi, "%s%f %f | %f\n", prefix, m->m01, m->m11, m->ty);
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
void*gfxfontlist_getuserdata(gfxfontlist_t*list, const char*id)
{
    gfxfontlist_t*l = list;
    while(l) {
	if(!strcmp((char*)l->font->id, id)) {
	    return l->user;
	}
	l = l->next;
    }
    return 0;
}
gfxfontlist_t*gfxfontlist_addfont2(gfxfontlist_t*list, gfxfont_t*font, void*user)
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
    l->user = user;
    l->next = 0;
    if(last) {
	last->next = l;
	return list;
    } else {
	return l;
    }
}
gfxfontlist_t*gfxfontlist_addfont(gfxfontlist_t*list, gfxfont_t*font)
{
    return gfxfontlist_addfont2(list, font, 0);
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

gfxline_t*gfxline_makerectangle(double x1,double y1,double x2, double y2)
{
    gfxline_t* line = (gfxline_t*)rfx_calloc(sizeof(gfxline_t)*5);
    line[0].x = x1;line[0].y = y1;line[0].type = gfx_moveTo;line[0].next = &line[1];
    line[1].x = x2;line[1].y = y1;line[1].type = gfx_lineTo;line[1].next = &line[2];
    line[2].x = x2;line[2].y = y2;line[2].type = gfx_lineTo;line[2].next = &line[3];
    line[3].x = x1;line[3].y = y2;line[3].type = gfx_lineTo;line[3].next = &line[4];
    line[4].x = x1;line[4].y = y1;line[4].type = gfx_lineTo;
    return line;
}

gfxline_t*gfxline_makecircle(double x,double y,double rx, double ry)
{
    double C1 = 0.2930;    
    double C2 = 0.4140;   
    double begin = 0.7070; 
    gfxline_t** line = (gfxline_t**)rfx_calloc(sizeof(gfxline_t*)*9);
    int t;
    for(t=0;t<9;t++) {
	line[t] = rfx_calloc(sizeof(gfxline_t));
    }
    line[0]->type = gfx_moveTo;
    line[0]->x = x+begin*rx;
    line[0]->y = y+begin*ry;
    for(t=1;t<9;t++) {
	line[t-1]->next = line[t];
	line[t]->type = gfx_splineTo;
    }
    line[8]->next = 0;
#define R(nr,cx,cy,mx,my) \
    line[nr]->sx = line[nr-1]->x + (cx); \
    line[nr]->sy = line[nr-1]->y + (cy); \
    line[nr]->x = line[nr]->sx + (mx); \
    line[nr]->y = line[nr]->sy + (my);
    R(1, -C1*rx,  C1*ry, -C2*rx,      0);
    R(2, -C2*rx,      0, -C1*rx, -C1*ry);
    R(3, -C1*rx, -C1*ry,      0, -C2*ry);
    R(4,      0, -C2*ry,  C1*rx, -C1*ry);
    R(5,  C1*rx, -C1*ry,  C2*rx,      0);
    R(6,  C2*rx,      0,  C1*rx,  C1*ry);
    R(7,  C1*rx,  C1*ry,      0,  C2*ry);
    R(8,      0,  C2*ry, -C1*rx,  C1*ry);
    gfxline_t*l = line[0];
    free(line);
    return l;
}

gfxbbox_t* gfxline_isrectangle(gfxline_t*_l)
{
    if(!_l)
        return 0;

    gfxline_t*l = gfxline_clone(_l);
    gfxline_optimize(l);

    double x1=0,x2=0,y1=0,y2=0;
    int xc=0,yc=0;
    char corners=0;

    char prev=0;
    char fail=0;
    for(;l; l=l->next) {
        double x = l->x;
        double y = l->y;

        char top=0,left=0;

        if(xc==2 && x!=x1 && x!=x2) {fail=1;break;}
        else if(xc>=1 && x==x1) {left=0;}
        else if(xc==2 && x==x2) {left=1;}
        else if(xc==1 && x!=x1) {x2 = x; xc=2; left=1;}
        else if(xc==0) {x1 = x; xc=1;left=0;}
        else {fprintf(stderr, "Internal error in rectangle detection\n");}

        if(yc==2 && y!=y1 && y!=y2) {fail=1;break;}
        else if(yc>=1 && y==y1) {top=0;}
        else if(yc==2 && y==y2) {top=1;}
        else if(yc==1 && y!=y1) {y2 = y; yc=2; top=1;}
        else if(yc==0) {y1 = y; yc=1;top=0;}
        else {fprintf(stderr, "Internal error in rectangle detection\n");}

        char pos=top<<1|left;

        if((pos^prev)==3) {
            /* diagonal lines not allowed */
            fail=1;break;
        }
        prev = pos;

        /* no corner except the first one may be touched twice */
        if(pos && (corners & 1<<pos)) {
            fail=1;break;
        }
        /* mark which corners have been touched so far */
        corners |= 1<<pos;
    }
    if(fail) {
        gfxline_free(l);
        return 0;
    }

    if(corners!=0x0f) return 0; // not all 4 corners reached

    if(x2<x1) {double x = x2;x2=x1;x1=x;}
    if(y2<y1) {double y = y2;y2=y1;y1=y;}

    gfxbbox_t*r = malloc(sizeof(gfxbbox_t));
    r->xmin = x1; r->ymin = y1;
    r->xmax = x2; r->ymax = y2;
    return r;
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

static char gfxpoint_equals(void*c1, void*c2)
{
    return !memcmp(c1, c2, sizeof(gfxpoint_t));
}
static unsigned int gfxpoint_hash(void*c)
{
    return string_hash3(c, sizeof(gfxpoint_t));
}
static void* gfxpoint_clone(void*c)
{
    void*n = malloc(sizeof(gfxpoint_t));
    memcpy(n, c, sizeof(gfxpoint_t));
    return n;
}
static void gfxpoint_destroy(void*c)
{
    free(c);
}
static type_t gfxpoint_type = {
    hash: (hash_func)gfxpoint_hash,
    equals: (equals_func)gfxpoint_equals,
    dup: (dup_func)gfxpoint_clone,
    free: (free_func)gfxpoint_destroy,
};

gfxline_t* gfxline_restitch(gfxline_t*line)
{
    dict_t*ff = dict_new2(&gfxpoint_type);
    dict_t*rev = dict_new2(&gfxpoint_type);
    
    gfxline_t*prev=0;
    while(line) {
	gfxline_t*next = line->next;
	if(line->type == gfx_moveTo && (line->next && line->next->type != gfx_moveTo)) {
	    gfxpoint_t xy = {line->x, line->y};
	    dict_put(ff, &xy, line);
	    prev = line;
	} else if(!line->next || line->next->type == gfx_moveTo) {
	    if(prev) {
		gfxpoint_t xy = {line->x, line->y};
		dict_put(rev, &xy, prev);
		line->next = 0;
		prev=0;
	    }
	}
	line = next;
    }
   
    gfxpoint_t pos = {0,0};

    gfxline_t*result = 0;
    gfxline_t*last = 0;
   
    char first = 1;
    while(dict_count(ff)) {
	char reverse = 0, stitch = 1;
	gfxline_t*l = dict_lookup(ff, &pos);
	if(l) {
	    char d = dict_del2(ff,&pos,l);assert(d);
	} else {
	    l = dict_lookup(rev, &pos);
	    if(l) {
		reverse = 1;
		char d = dict_del2(rev,&pos,l);assert(d);
	    }
	}
	if(!l) {
	    /* try to find *any* entry. this is costly, but
	       doesn't happen too often */
	    stitch = 0;
	    DICT_ITERATE_DATA(ff, gfxline_t*, l2) {
		l = l2;
		break;
	    }
	    assert(l);
	    gfxpoint_t xy = {l->x,l->y};
	    char d = dict_del2(ff,&xy,l);assert(d);
	}
	
	gfxline_t*end = l;
	if(!reverse) {
	    while(end->next) end = end->next;
	    pos.x = end->x;
	    pos.y = end->y;
	    char d = dict_del2(rev,&pos,l);assert(d);
	} else {
	    l = gfxline_reverse(l);
	    pos.x = end->x;
	    pos.y = end->y;
	    char d = dict_del2(ff,&pos,end);assert(d);
	}

	assert(l->type == gfx_moveTo);
	if(stitch && !first) {
	    /* cut away the moveTo */
	    gfxline_t*next = l->next;
	    free(l);
	    l = next;
	}

	if(!last) {
	    result = l;
	    last = end;
	} else {
	    last->next = l;
	    last = end;
	}
	first = 0;
    }
    dict_destroy(ff);
    dict_destroy(rev);
    return result;
}

gfxline_t* gfxline_reverse(gfxline_t*line)
{
    gfxline_t*b = 0;
    while(line) {
	gfxline_t*next = line->next;
	if(next && next->type != gfx_moveTo) {
	    line->type = next->type;
	    line->sx = next->sx;
	    line->sy = next->sy;
	} else {
	    line->type = gfx_moveTo;
	}
	line->next = b;
	b = line;
	line = next;
    }
    return b;
}

void gfxgradient_destroy(gfxgradient_t*gradient)
{
    while(gradient) {
	gfxgradient_t*next = gradient->next;
	free(gradient);
	gradient = next;
    }
}

gfxparams_t* gfxparams_new()
{
    return (gfxparams_t*)rfx_calloc(sizeof(gfxparams_t));
}

void gfxparams_store(gfxparams_t*params, const char*key, const char*value)
{
    gfxparam_t*o = params->params;
    while(o) {
        if(!strcmp(key, o->key)) {
            /* overwrite old value */
            free((void*)o->value);
            o->value = strdup(value);
            return;
        }
        o = o->next;
    }
    gfxparam_t*p = (gfxparam_t*)malloc(sizeof(gfxparam_t));
    p->key = strdup(key);
    p->value = strdup(value);
    p->next = 0;

    if(params->last) {
	params->last->next = p;
	params->last = p;
    } else {
	params->params = p;
	params->last = p;
    }
}

void gfxparams_free(gfxparams_t*params)
{
    gfxparam_t*p = params->params;
    while(p) {
	gfxparam_t*next = p->next;
	free((void*)p->key);
	if(p->value) free((void*)p->value);
	free(p);
	p = next;
    }
    free(params);
}

