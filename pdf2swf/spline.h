/* spline.h
   Header file for spline.cc

   Part of the swftools package.

   Copyright (c) 2001,2002,2003 Matthias Kramm <kramm@quiss.org> 
 
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
