/* spline.cc
   Routine to convert cubic splines into quadratic ones.

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

/* At the moment, we convert non-recursively into at max. 6 quadratic splines, 
   trying to maintain the general structure of the curve, rather than it's 
   exact path. 
   To accomplish this, we split each curve at it's X,Y extremas as well as
   its deviation's X,Y extremas.
 */

#include <math.h>
#include "spline.h"

int solve(double a,double b,double c, double*dd)
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

double distance(struct plotxy p0, struct plotxy p1)
{
 double xd=p1.x-p0.x;
 double yd=p1.y-p0.y;
 return sqrt(xd*xd+yd*yd);
}

int wp(double p0,double p1,double p2,double p3,double*dd)
{
    double div= (6*p0-18*p1+18*p2-6*p3);
    if(!div) return 0;
    dd[0] = -(6*p1-12*p2+6*p3)/div;
    return (dd[0]>0 && dd[0]<1);
}

int approximate(struct plotxy p0, struct plotxy p1,
                struct plotxy p2, struct plotxy p3, struct qspline*q)
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
       double dist=distance(myxy[t],last);
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

