#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "../gfxdevice.h"
#include "poly.h"

static edge_t*edge_new(int x1, int y1, int x2, int y2)
{
    edge_t*s = malloc(sizeof(edge_t));
    s->a.x = x1;
    s->a.y = y1;
    s->b.x = x2;
    s->b.y = y2;
    s->next = 0;
    return s;
}

static inline void gfxpoly_add_edge(edge_t**list, double _x1, double _y1, double _x2, double _y2)
{
    int x1 = ceil(_x1);
    int y1 = ceil(_y1);
    int x2 = ceil(_x2);
    int y2 = ceil(_y2);
    if(x1!=x2 || y1!=y2) {
        edge_t*s = edge_new(x1, y1, x2, y2);
        s->next = *list;
        *list = s;
    }
}

gfxpoly_t* gfxpoly_fillToPoly(gfxline_t*line)
{
    edge_t*s = 0;

    /* factor that determines into how many line fragments a spline is converted */
    double subfraction = 2.4;//0.3

    double lastx=0, lasty=0;
    assert(!line || line[0].type == gfx_moveTo);
    while(line) {
        if(line->type == gfx_moveTo) {
        } else if(line->type == gfx_lineTo) {
            gfxpoly_add_edge(&s, lastx, lasty, line->x, line->y);
	} else if(line->type == gfx_splineTo) {
            int parts = (int)(sqrt(fabs(line->x-2*line->sx+lastx) + 
                                   fabs(line->y-2*line->sy+lasty))*subfraction);
            if(!parts) parts = 1;
	    double stepsize = 1.0/parts;
            int i;
	    for(i=0;i<parts;i++) {
		double t = (double)i*stepsize;
		double x = line->x*t*t + 2*line->sx*t*(1-t) + x*(1-t)*(1-t);
		double y = line->y*t*t + 2*line->sy*t*(1-t) + y*(1-t)*(1-t);
                gfxpoly_add_edge(&s, lastx, lasty, x, y);
                lastx = x;
                lasty = y;
	    }
            gfxpoly_add_edge(&s, lastx, lasty, line->x, line->y);
        }
        lastx = line->x;
        lasty = line->y;
        line = line->next;
    }

    gfxline_free(line);
    return s;
}

