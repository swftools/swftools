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

int approximate(struct plotxy p0, struct plotxy p1,
                struct plotxy p2, struct plotxy p3, struct qspline*q);

#endif// __spline_h__
