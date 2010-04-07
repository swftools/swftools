#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "poly.h"
#include "wind.h"
#include "convert.h"

/* notice: left/right for a coordinate system where y goes up, not down */
typedef enum {LEFT=0, RIGHT=1} leftright_t;

/* factor that determines into how many line fragments a spline is converted */
#define SUBFRACTION (2.4)

// spline equation:
// s(t) = t*t*x2 + 2*t*(1-t)*cx + (1-t)*(1-t)*x1
//
// s(0.5) = 0.25*x2 + 0.5*cx + 0.25*x1
// ds(t)/dt = 2*t*x2 + (2-2t)*cx + (2t-2)*x1
// ds(0) = 2*(cx-x1)

static void draw_arc(gfxdrawer_t*draw, double x, double y, double a1, double a2, double r)
{
    if(a2<a1) a2+=M_PI*2;

    double d = (a2-a1);
    int steps = ceil(8*d/(M_PI*2)); // we use 8 splines for a full circle
    if(!steps) return;

    int t;
    double step = (a2-a1)/steps;
    double lastx = x+cos(a1)*r;
    double lasty = y+sin(a1)*r;

    /* we could probably build a table for this- there are only 8
       possible values for step */
    double r2 = r*(2-sqrt(0.5+0.5*cos(step)));

    draw->lineTo(draw, x+cos(a1)*r,y+sin(a1)*r);
    for(t=1;t<=steps;t++) {
	double a = a1 + t*step;
	double c = cos(a)*r;
	double s = sin(a)*r;
	double xx = c + x;
	double yy = s + y;
	double dx = x + cos(a-step/2)*r2;
	double dy = y + sin(a-step/2)*r2;
	draw->splineTo(draw, dx, dy, xx, yy);
	lastx = xx;
	lasty = yy;
    }
}

static void draw_single_stroke(gfxpoint_t*p, int num, gfxdrawer_t*draw, double width, gfx_capType cap, gfx_joinType join, double limit)
{
    width/=2;
    if(width<=0) 
	width = 0.05;

    /* remove duplicate points */
    int s=1,t;
    gfxpoint_t last = p[0];
    for(t=1;t<num;t++) {
	if(p[t].x != last.x || p[t].y != last.y) {
	    p[s++] = last = p[t];
	}
    }
    num = s;
    
    char closed = (num>2 && p[0].x == p[num-1].x && p[0].y == p[num-1].y);
    
    int start = 0;
    int end = num-1;
    int incr = 1;
    int pos = 0;

    double lastw=0;
    /* iterate through the points two times: first forward, then backward,
       adding a stroke outline to the right side and line caps after each
       pass */
    int pass;
    for(pass=0;pass<2;pass++) {
	if(closed) {
	    double dx = p[end].x - p[end-incr].x;
	    double dy = p[end].y - p[end-incr].y;
	    lastw = atan2(dy,dx);
	    if(lastw<0) lastw+=M_PI*2;
	}
    
	int pos;
	for(pos=start;pos!=end;pos+=incr) {
	    //printf("%d) %.2f %.2f\n", pos, p[pos].x, p[pos].y);
	    double dx = p[pos+incr].x - p[pos].x;
	    double dy = p[pos+incr].y - p[pos].y;
	    double w = atan2(dy,dx);
	    if(w<0) w+=M_PI*2;
	    
	    if(closed || pos!=start) {
		double d = w-lastw;
		leftright_t turn;
		if(d>=0 && d<M_PI) turn=LEFT;
		else if(d<0 && d>-M_PI) turn=RIGHT;
		else if(d>=M_PI) {turn=RIGHT;}
		else if(d<=-M_PI) {turn=LEFT;d+=M_PI*2;}
		else {assert(0);}
		if(turn!=LEFT || join==gfx_joinBevel) {
		    // nothing to do. bevel joins are easy
		} else if(join==gfx_joinRound) {
		    draw_arc(draw, p[pos].x, p[pos].y, lastw-M_PI/2, w-M_PI/2, width);
		} else if(join==gfx_joinMiter) {
		    double xw = M_PI/2 - d/2;
		    if(xw>0) {
			double r2 = 1.0 / sin(M_PI/2-d/2);
			if(r2 < limit) {
			    r2 *= width;
			    double addx = cos(lastw-M_PI/2+d/2)*r2;
			    double addy = sin(lastw-M_PI/2+d/2)*r2;
			    draw->lineTo(draw, p[pos].x+addx, p[pos].y+addy);
			}
		    } 
		}
	    }

	    double addx = cos(w-M_PI/2)*width;
	    double addy = sin(w-M_PI/2)*width;
	    draw->lineTo(draw, p[pos].x+addx, p[pos].y+addy);
	    double px2 = p[pos+incr].x + addx;
	    double py2 = p[pos+incr].y + addy;
	    draw->lineTo(draw, p[pos+incr].x+addx, p[pos+incr].y+addy);
	    lastw = w;
	}

	if(closed) {
	    draw->close(draw);
	} else {
	    /* draw stroke ends. We draw duplicates of some points here. The drawer
	       implementation should be smart enough to remove them. */
	    double c = cos(lastw-M_PI/2)*width;
	    double s = sin(lastw-M_PI/2)*width;
	    if(cap == gfx_capButt) {
		draw->lineTo(draw, p[pos].x+c, p[pos].y+s);
		draw->lineTo(draw, p[pos].x-c, p[pos].y-s);
	    } else if(cap == gfx_capRound) {
		draw_arc(draw, p[pos].x, p[pos].y, lastw-M_PI/2, lastw+M_PI/2, width);
	    } else if(cap == gfx_capSquare) {
		double c = cos(lastw-M_PI/2)*width;
		double s = sin(lastw-M_PI/2)*width;
		draw->lineTo(draw, p[pos].x+c, p[pos].y+s);
		draw->lineTo(draw, p[pos].x+c-s, p[pos].y+s+c);
		draw->lineTo(draw, p[pos].x-c-s, p[pos].y-s+c);
		draw->lineTo(draw, p[pos].x-c, p[pos].y-s);
	    }
	    lastw += M_PI; // for dots
	}
	start=num-1;
	end=0;
	incr=-1;
    }
    if(!closed)
	draw->close(draw);
}

void draw_stroke(gfxline_t*start, gfxdrawer_t*draw, double width, gfx_capType cap, gfx_joinType join, double miterLimit)
{
    if(!start) 
	return;
    assert(start->type == gfx_moveTo);
    gfxline_t*line = start;
    // measure array size
    int size = 0;
    int pos = 0;
    double lastx,lasty;
    while(line) {
	if(line->type == gfx_moveTo) {
	    if(pos>size) size = pos;
	    pos++;
	} else if(line->type == gfx_lineTo) {
	    pos++;
	} else if(line->type == gfx_splineTo) {
            int parts = (int)(sqrt(fabs(line->x-2*line->sx+lastx) + fabs(line->y-2*line->sy+lasty))*SUBFRACTION);
            if(!parts) parts = 1;
	    pos+=parts+1;
	}
	lastx = line->x;
	lasty = line->y;
	line = line->next;
    }
    if(pos>size) size = pos;
    if(!size) return;

    gfxpoint_t* points = malloc(sizeof(gfxpoint_t)*size);
    line = start;
    pos = 0;
    while(line) {
	if(line->type == gfx_moveTo) {
	    if(pos)
		draw_single_stroke(points, pos, draw, width, cap, join, miterLimit);
	    pos = 0;
	} else if(line->type == gfx_splineTo) {
            int parts = (int)(sqrt(fabs(line->x-2*line->sx+lastx) + fabs(line->y-2*line->sy+lasty))*SUBFRACTION);
            if(!parts) parts = 1;
	    double stepsize = 1.0/parts;
            int i;
	    for(i=0;i<parts;i++) {
		double t = (double)i*stepsize;
		points[pos].x = (line->x*t*t + 2*line->sx*t*(1-t) + lastx*(1-t)*(1-t));
		points[pos].y = (line->y*t*t + 2*line->sy*t*(1-t) + lasty*(1-t)*(1-t));
		pos++;
	    }
	}
	lastx = points[pos].x = line->x;
	lasty = points[pos].y = line->y;
	pos++;
	line = line->next;
    }
    if(pos) draw_single_stroke(points, pos, draw, width, cap, join, miterLimit);
    free(points);
}

static windcontext_t onepolygon = {1};
gfxpoly_t* gfxpoly_from_stroke(gfxline_t*line, gfxcoord_t width, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit, double gridsize)
{
    gfxdrawer_t d;
    gfxdrawer_target_poly(&d, gridsize);
    draw_stroke(line, &d, width, cap_style, joint_style, miterLimit);
    gfxpoly_t*poly = (gfxpoly_t*)d.result(&d);
    assert(gfxpoly_check(poly, 1));
    gfxpoly_t*poly2 = gfxpoly_process(poly, 0, &windrule_circular, &onepolygon);
    gfxpoly_destroy(poly);
    return poly2;
}

