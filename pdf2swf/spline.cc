/* spline.cc
   Routine to convert cubic splines into quadratic ones.

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "spline.h"

static int solve(double a,double b,double c, double*dd)
{
    double det=b*b-4*a*c;
    int pos = 0;
    if(det<0) return 0; // we don't do imaginary. not today.
    if(det==0) { // unlikely, but we have to deal with it.
     dd[0]=-b/2*a;
     return (dd[0]>0 && dd[0]<1);
    }

    dd[pos]=(-b+sqrt(det))/(2*a);
    if(dd[pos]>0 && dd[pos]<1)
     pos++;
    dd[pos]=(-b-sqrt(det))/(2*a);
    if(dd[pos]>0 && dd[pos]<1)
     pos++;
    return pos;
}

struct plotxy splinepos(struct plotxy p0, struct plotxy p1, struct plotxy p2, struct plotxy p3, double d) {
    struct plotxy p;
    p.x = (p0.x * d*d*d + p1.x * 3*(1-d)*d*d + p2.x * 3*(1-d)*(1-d)*d + p3.x * (1-d)*(1-d)*(1-d));
    p.y = (p0.y * d*d*d + p1.y * 3*(1-d)*d*d + p2.y * 3*(1-d)*(1-d)*d + p3.y * (1-d)*(1-d)*(1-d));
    return p;
}

inline double plotxy_dist(struct plotxy a, struct plotxy b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx+dy*dy);
}


int wp(double p0,double p1,double p2,double p3,double*dd)
{
    double div= (6*p0-18*p1+18*p2-6*p3);
    if(!div) return 0;
    dd[0] = -(6*p1-12*p2+6*p3)/div;
    return (dd[0]>0 && dd[0]<1);
}

int approximate(struct plotxy p0, struct plotxy p1, struct plotxy p2, struct plotxy p3, struct qspline*q)
{
    double roots[12];
    int pos = 0;
    int s,t;
    struct plotxy myxy[12];
    struct plotxy last;
    // the parameters for the solve function are the 1st deviation of a cubic spline
    roots[pos] = 0;pos++;
    pos += solve(3*p0.x-9*p1.x+9*p2.x-3*p3.x, 6*p1.x-12*p2.x+6*p3.x,3*p2.x-3*p3.x, &roots[pos]);
    pos += solve(3*p0.y-9*p1.y+9*p2.y-3*p3.y, 6*p1.y-12*p2.y+6*p3.y,3*p2.y-3*p3.y, &roots[pos]);
    pos += wp(p0.x,p1.x,p2.x,p3.x,&roots[pos]);
    pos += wp(p0.x,p1.x,p2.x,p3.x,&roots[pos]);
    roots[pos] = 1;pos++;
  
    // bubblesort - fast enough for 4-6 parameters
    for(s=0;s<pos;s++)
    for(t=s+1;t<pos;t++)
    if(roots[s]>roots[t])
    {
       double tmp=roots[s];
       roots[s]=roots[t];
       roots[t]=tmp;
    }
    for(t=0;t<pos;t++)
     myxy[t] = splinepos(p0,p1,p2,p3,roots[t]);
    
    s=1;
    last = myxy[0];
    for(t=1;t<pos;t++)
    {
       double dist=plotxy_dist(myxy[t],last);
       myxy[s]=myxy[t];
       roots[s]=roots[t];
       if(dist>0.01 || t==pos-1) 
       {
	s++;
	last=myxy[t];
       }
    }
    pos = s;

    // try 1:curve through 3 points, using the middle of the cubic spline.
    for(t=0;t<pos-1;t++) {
//     circle(myxy[t].x,myxy[t].y,5);
     struct plotxy control;
     struct plotxy midpoint = splinepos(p0,p1,p2,p3,(roots[t]+roots[t+1])/2);
     control.x = midpoint.x + (midpoint.x-(myxy[t].x+myxy[t+1].x)/2);
     control.y = midpoint.y + (midpoint.y-(myxy[t].y+myxy[t+1].y)/2);
     //qspline(myxy[t],control,myxy[t+1]);
     q[t].start=myxy[t];
     q[t].control=control;
     q[t].end=myxy[t+1];
    }

    /*
    for(t=0;t<pos-1;t++) {
     plotxy control;
     vga.setcolor(0xffffff);
     circle(myxy[t].x,myxy[t].y,5);
     if(t==0) {
       //double lenmain = distance(p3,p0);
       //double lenq = distance(myxy[0],myxy[1]);
       //control.x = myxy[0].x + (p2.x-p3.x);// /lenmain*lenq;
       //control.y = myxy[0].y + (p2.y-p3.y);// /lenmain*lenq;
       plotxy midpoint = splinepos(p0,p1,p2,p3,(roots[t]+roots[t+1])/2);
       control.x = midpoint.x + (midpoint.x-(myxy[t].x+myxy[t+1].x)/2);
       control.y = midpoint.y + (midpoint.y-(myxy[t].y+myxy[t+1].y)/2);
       qspline(myxy[0], control, myxy[1]);
     } else {
       control.x = 2*myxy[t].x - last.x;
       control.y = 2*myxy[t].y - last.y;
       qspline(myxy[t], control, myxy[t+1]);
     }
     last = control;
    }*/
    return pos-1;
}

/* move the control point so that the spline runs through the original
   control point */
void fixcp(qspline*s)
{
    plotxy mid,dir;
    mid.x = (s->end.x + s->start.x)/2;
    mid.y = (s->end.y + s->start.y)/2;
    dir.x = s->control.x - mid.x;
    dir.y = s->control.y - mid.y;
    s->control.x = mid.x + 2*dir.x;
    s->control.y = mid.y + 2*dir.y;
}

int approximate2(struct cspline*s, struct qspline*q, double quality, double start, double end);

void check(struct cspline*s, struct qspline*q, int num)
{
    int t;
    plotxy p = s->start;
    for(t=0;t<num;t++) {
	plotxy p2 = q[t].start;
	if(plotxy_dist(p,p2) > 0.005) {
	    printf("--\n");
	    exit(1);
	}
	p = q[t].end;
    }
    if(plotxy_dist(p, s->end) > 0.005) {
	    printf("--\n");
	    exit(1);
    }
}

int cspline_approximate(struct cspline*s, struct qspline*q, double quality, approximate_method method)
{
    if(method==0) {
	return approximate(s->start, s->control1, s->control2, s->end, q);
    } else  {
	return approximate2(s, q, quality, 0.0, 1.0);
    }
}
inline plotxy cspline_getpoint(cspline*s, double t)
{
    plotxy p;
    p.x= s->end.x*t*t*t + 3*s->control2.x*t*t*(1-t) 
	    + 3*s->control1.x*t*(1-t)*(1-t) + s->start.x*(1-t)*(1-t)*(1-t);
    p.y= s->end.y*t*t*t + 3*s->control2.y*t*t*(1-t) 
	    + 3*s->control1.y*t*(1-t)*(1-t) + s->start.y*(1-t)*(1-t)*(1-t);
    return p;
}
plotxy cspline_getderivative(cspline*s, double t)
{
    plotxy d;
    d.x = s->end.x*(3*t*t) + 3*s->control2.x*(2*t-3*t*t) + 
		3*s->control1.x*(1-4*t+3*t*t) + s->start.x*(-3+6*t-3*t*t);
    d.y = s->end.y*(3*t*t) + 3*s->control2.y*(2*t-3*t*t) + 
		3*s->control1.y*(1-4*t+3*t*t) + s->start.y*(-3+6*t-3*t*t);
    return d;
}
plotxy cspline_getderivative2(cspline*s, double t)
{
    plotxy d;
    d.x = s->end.x*(6*t) + 3*s->control2.x*(2-6*t) + 
		3*s->control1.x*(-4+6*t) + s->start.x*(6-6*t);
    d.y = s->end.y*(6*t) + 3*s->control2.y*(2-6*t) + 
		3*s->control1.y*(-4+6*t) + s->start.y*(6-6*t);
    return d;
}
plotxy cspline_getderivative3(cspline*s, double t)
{
    plotxy d;
    d.x = 6*s->end.x - 18*s->control2.x + 18*s->control1.x - 6*s->start.x;
    d.y = 6*s->end.y - 18*s->control2.y + 18*s->control1.y - 6*s->start.y;
    return d;
}
void cspline_getequalspacedpoints(cspline*s, float**p, int*num, double dist)
{
    plotxy d,next;
    double t = 0;
    int end = 0;
    int pos = 0;
    float*positions = (float*)malloc(1048576);
    do
    {
	if(t>=1.0) {
	    t = 1.0;
	    end = 1;
	}

	plotxy d = cspline_getderivative(s, t);
	plotxy d2 = cspline_getderivative2(s, t);

	double dl = sqrt(d.x*d.x+d.y*d.y);
	double dl2 = sqrt(d2.x*d2.x+d2.y*d2.y);
    
	double rdl = dist/dl;

	if(rdl>1.0-t)
	    rdl = 1.0-t;

	plotxy p = cspline_getpoint(s, t);
	while(plotxy_dist(cspline_getpoint(s, t+rdl), p) > dist) {
	    /* we were ask to divide the spline into dist long fragments,
	       but for the value we estimated even the geometric distance
	       is bigger than 'dist'. Approximate a better value.
	    */
	    rdl = rdl*0.9;
	}
	
	positions[pos] = t;
	t+=rdl;
	pos++;
    }
    while(!end);
    *num = pos;
    *p = positions;
}

plotxy qspline_getpoint(qspline*s, double t)
{
    plotxy p;
    p.x= s->end.x*t*t + 2*s->control.x*t*(1-t) + s->start.x*(1-t)*(1-t);
    p.y= s->end.y*t*t + 2*s->control.y*t*(1-t) + s->start.y*(1-t)*(1-t);
    return p;
}
plotxy qspline_getderivative(qspline*s, double t)
{
    plotxy p;
    p.x= s->end.x*2*t + 2*s->control.x*(1-2*t) + s->start.x*(-2+2*t);
    p.y= s->end.y*2*t + 2*s->control.y*(1-2*t) + s->start.y*(-2+2*t);
    return p;
}
plotxy qspline_getderivative2(qspline*s, double t)
{
    plotxy p;
    p.x= s->end.x*2 + 2*s->control.x*(-2) + s->start.x*(2);
    p.y= s->end.y*2 + 2*s->control.y*(-2) + s->start.y*(2);
    return p;
}
double qspline_getlength(qspline*s)
{
    double t = 0;
    int end = 0;
    double len;
    plotxy last = qspline_getpoint(s, 0.0);
    do {
	if(t>=1.0) {
	    t = 1.0;
	    end = 1;
	}
	plotxy d2 = qspline_getderivative2(s, t);
	double dl2 = sqrt(d2.x*d2.x+d2.y*d2.y);
	double rdl = 1.0/dl2;
	if(rdl>0.01)
	    rdl = 0.01;
	t+=rdl;
	plotxy here = qspline_getpoint(s, t);
	len += plotxy_dist(last, here);
	last = here;
    }
    while(!end);
    return len;
}
void qsplines_getequalspacedpoints(qspline**s, int num, float**p, int*pnum, double acc)
{
/*    int t;
    float r[128];
    for(t=0;t<num;t++) {
	qspline_getlength();
    }*/
    return;
}

void qsplines_getdrawpoints(qspline*s, int num, float**p, int*pnum, double acc)
{
    plotxy d,next;
    double t = 0;
    int end = 0;
    int pos = 0;
    float*positions = (float*)malloc(1048576);
    do
    {
	if(t>=1.0) {
	    t = 1.0;
	    end = 1;
	}

	plotxy d = qspline_getderivative(s, t);
	double dl = sqrt(d.x*d.x+d.y*d.y);
	double rdl = acc/dl;

	if(rdl>acc)
	    rdl = acc;

	positions[pos] = t;
	t+=rdl;
	pos++;
    }
    while(!end);
    *pnum = pos;
    *p = positions;
}

int approximate2(struct cspline*s, struct qspline*q, double quality, double start, double end)
{
    int num=0;
    plotxy qr1,qr2,cr1,cr2;
    double dist1,dist2;
    int t;
    int recurse = 0;
    int probes = 15;
    qspline test;
    test.start = cspline_getpoint(s, start);
    test.control = cspline_getpoint(s, (start+end)/2);
    test.end = cspline_getpoint(s, end);
    fixcp(&test);
    for(t=0;t<probes;t++) {
	double pos = 0.5/(probes*2)*(t*2+1);
	qr1 = qspline_getpoint(&test, pos);
	cr1 = cspline_getpoint(s, start+pos*(end-start));
	dist1 = plotxy_dist(qr1, cr1);
	if(dist1>quality) {
	    recurse=1;break;
	}
	qr2 = qspline_getpoint(&test, (1-pos));
	cr2 = cspline_getpoint(s, start+(1-pos)*(end-start));
	dist2 = plotxy_dist(qr2, cr2);
	if(dist2>quality) {
	    recurse=1;break;
	}
    }

    if(recurse && (end-start)>1.0/120) {
	/* quality is too bad, split it up recursively */
	num += approximate2(s, q, quality, start, (start+end)/2);
	q+=num;
	num += approximate2(s, q, quality, (start+end)/2, end);
	return num;
    } else {
	*q = test;
	return 1;
    }
}

