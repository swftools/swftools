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
#include <memory.h>
#include <math.h>
#include "drawer.h"

static char* getToken(const char**p)
{
    const char*start;
    char*result;
    while(**p && strchr(" ,\t\n\r", **p)) {
	(*p)++;
    } 
    start = *p;
    while(**p && !strchr(" ,\t\n\r", **p)) {
	(*p)++;
    }
    result = malloc((*p)-start+1);
    memcpy(result,start,(*p)-start+1);
    result[(*p)-start] = 0;
    return result;
}

void draw_conicto(drawer_t*draw, FPOINT*  c, FPOINT*  to)
{
    FPOINT* pos = &draw->pos;
    FPOINT c1,c2;
    c1.x = (pos->x + 2 * c->x) / 3;
    c1.y = (pos->y + 2 * c->y) / 3;
    c2.x = (2 * c->x + to->x) / 3;
    c2.y = (2 * c->y + to->y) / 3;
    draw_cubicto(draw, &c1,&c2,to);

    draw->pos = *to;
}

void draw_string(drawer_t*draw, const char*string)
{
    const char*p = string;
    while(*p) {
	char*token = getToken(&p);
	if(!strncmp(token, "moveTo", 6)) {
	    FPOINT to;
	    to.x = atoi(getToken(&p));
	    to.y = atoi(getToken(&p));
	    draw->moveTo(draw, &to);
	}
	else if(!strncmp(token, "lineTo", 6)) {
	    FPOINT to;
	    to.x = atoi(getToken(&p));
	    to.y = atoi(getToken(&p));
	    draw->lineTo(draw, &to);
	}
	else if(!strncmp(token, "curveTo", 7) || !strncmp(token, "splineTo", 8)) {
	    FPOINT mid,to;
	    mid.x = atoi(getToken(&p));
	    mid.y = atoi(getToken(&p));
	    to.x = atoi(getToken(&p));
	    to.y = atoi(getToken(&p));
	    draw->splineTo(draw, &mid, &to);
	}
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

/* move the control point so that the spline runs through the original
   control point */
static void fixcp(struct qspline*s)
{
    struct SPLINEPOINT mid,dir;
    mid.x = (s->end.x + s->start.x)/2;
    mid.y = (s->end.y + s->start.y)/2;
    dir.x = s->control.x - mid.x;
    dir.y = s->control.y - mid.y;
    s->control.x = mid.x + 2*dir.x;
    s->control.y = mid.y + 2*dir.y;
}

static inline struct SPLINEPOINT cspline_getpoint(struct cspline*s, double t)
{
    struct SPLINEPOINT p;
    p.x= s->end.x*t*t*t + 3*s->control2.x*t*t*(1-t) 
	    + 3*s->control1.x*t*(1-t)*(1-t) + s->start.x*(1-t)*(1-t)*(1-t);
    p.y= s->end.y*t*t*t + 3*s->control2.y*t*t*(1-t) 
	    + 3*s->control1.y*t*(1-t)*(1-t) + s->start.y*(1-t)*(1-t)*(1-t);
    return p;
}
static struct SPLINEPOINT qspline_getpoint(struct qspline*s, double t)
{
    struct SPLINEPOINT p;
    p.x= s->end.x*t*t + 2*s->control.x*t*(1-t) + s->start.x*(1-t)*(1-t);
    p.y= s->end.y*t*t + 2*s->control.y*t*(1-t) + s->start.y*(1-t)*(1-t);
    return p;
}

static struct SPLINEPOINT cspline_getderivative(struct cspline*s, double t)
{
    struct SPLINEPOINT d;
    d.x = s->end.x*(3*t*t) + 3*s->control2.x*(2*t-3*t*t) + 
		3*s->control1.x*(1-4*t+3*t*t) + s->start.x*(-3+6*t-3*t*t);
    d.y = s->end.y*(3*t*t) + 3*s->control2.y*(2*t-3*t*t) + 
		3*s->control1.y*(1-4*t+3*t*t) + s->start.y*(-3+6*t-3*t*t);
    return d;
}

static int approximate2(struct cspline*s, struct qspline*q, double quality, double start, double end, int max, int depth)
{
    int num=0;
    struct SPLINEPOINT qr1,qr2,cr1,cr2;
    double dist1,dist2;
    double qquality = quality*quality;
    int t;
    int recurse = 0;
    int probes = 15;
    struct qspline test;
    char left = 0;
    test.start = cspline_getpoint(s, start);
    test.control = cspline_getpoint(s, (start+end)/2);
    test.end = cspline_getpoint(s, end);
    fixcp(&test);

    if(start< 0.5) {
	test.control = cspline_getderivative(s, start);
	test.control.x *= (end-start)/2;
	test.control.y *= (end-start)/2;
	test.control.x += test.start.x;
	test.control.y += test.start.y;
    } else {
	test.control = cspline_getderivative(s, end);
	test.control.x *= -(end-start)/2;
	test.control.y *= -(end-start)/2;
	test.control.x += test.end.x;
	test.control.y += test.end.y;
    }

    for(t=0;t<probes;t++) {
	double pos = 0.5/(probes*2)*(t*2+1);
	double dx,dy;
	qr1 = qspline_getpoint(&test, pos);
	cr1 = cspline_getpoint(s, start+pos*(end-start));

	dx = qr1.x - cr1.x;
	dy = qr1.y - cr1.y;
	dist1 = dx*dx+dy*dy;

	if(dist1>qquality) {
	    recurse=1;break;
	}
	qr2 = qspline_getpoint(&test, (1-pos));
	cr2 = cspline_getpoint(s, start+(1-pos)*(end-start));

	dx = qr2.x - cr2.x;
	dy = qr2.y - cr2.y;
	dist2 = dx*dx+dy*dy;

	if(dist2>qquality) {
	    recurse=1;break;
	}
    }

    if(recurse && (end-start)>1.0/120 && max-depth > 0) {
	/* quality is too bad, split it up recursively */
	num += approximate2(s, q, quality, start, (start+end)/2, max, depth+1);
	q+=num;
	max-=num;
	num += approximate2(s, q, quality, (start+end)/2, end, max, depth+1);
	return num;
    } else {
	*q = test;
	return 1;
    }
}

void draw_cubicto(drawer_t*draw, FPOINT*  control1, FPOINT* control2, FPOINT*  to)
{
    struct qspline q[128];
    struct cspline c;
    c.start.x = draw->pos.x;
    c.start.y = draw->pos.y;
    c.control1.x = control1->x;
    c.control1.y = control1->y;
    c.control2.x = control2->x;
    c.control2.y = control2->y;
    c.end.x = to->x;
    c.end.y = to->y;
    double quality = 0.8;
    double maxerror = (500-(quality*5)>1?500-(quality*5):1)/20.0;

    int num = approximate2(&c, q, maxerror, 0.0, 1.0, 128, 0);
    int t;
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


