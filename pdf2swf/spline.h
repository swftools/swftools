/* spline.h
   Header file for spline.cc

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __spline_h__
#define __spline_h__

struct plotxy
{
    double x,y;
};

struct qspline
{
    struct plotxy start;
    struct plotxy control;
    struct plotxy end;
};

struct cspline
{
    struct plotxy start;
    struct plotxy control1;
    struct plotxy control2;
    struct plotxy end;
};

typedef enum {APPROXIMATE_INFLECTION=0, APPROXIMATE_RECURSIVE_BINARY=1} approximate_method;

int approximate(struct plotxy p0, struct plotxy p1, struct plotxy p2, struct plotxy p3, struct qspline*q);

struct plotxy splinepos(struct plotxy p0, struct plotxy p1, struct plotxy p2, struct plotxy p3, double d);
double plotxy_dist(struct plotxy a, struct plotxy b);

plotxy cspline_getpoint(cspline*s, double t);
plotxy cspline_getderivative(cspline*s, double t);
plotxy cspline_getderivative2(cspline*s, double t);
plotxy cspline_getderivative3(cspline*s, double t);
void cspline_getequalspacedpoints(cspline*s, float**p, int*num, double dist);
int cspline_approximate(struct cspline*s, struct qspline*q, double quality, approximate_method method);

plotxy qspline_getpoint(qspline*s, double t);
plotxy qspline_getderivative(qspline*s, double t);
plotxy qspline_getderivative2(qspline*s, double t);
double qspline_getlength(qspline*s);
void qsplines_getequalspacedpoints(qspline**s, int num, float**p, int*pnum, double t);

#endif// __spline_h__
