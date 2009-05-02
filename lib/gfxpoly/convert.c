#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "../gfxdevice.h"
#include "../mem.h"
#include "poly.h"

static edge_t*edge_new(int x1, int y1, int x2, int y2)
{
    edge_t*s = rfx_calloc(sizeof(edge_t));
    s->a.x = x1;
    s->a.y = y1;
    s->b.x = x2;
    s->b.y = y2;
    return s;
}

static inline void gfxpoly_add_edge(gfxpoly_t*poly, double _x1, double _y1, double _x2, double _y2)
{
    int x1 = ceil(_x1);
    int y1 = ceil(_y1);
    int x2 = ceil(_x2);
    int y2 = ceil(_y2);
    if(x1!=x2 || y1!=y2) {
        edge_t*s = edge_new(x1, y1, x2, y2);
        s->next = poly->edges;
        poly->edges = s;
    }
}

gfxpoly_t* gfxpoly_from_gfxline(gfxline_t*line, double gridsize)
{
    gfxpoly_t*p = gfxpoly_new(gridsize);

    /* factor that determines into how many line fragments a spline is converted */
    double subfraction = 2.4;//0.3

    double z = 1.0 / gridsize;

    double lastx=0, lasty=0;
    assert(!line || line[0].type == gfx_moveTo);
    while(line) {
        double x = line->x*z;
        double y = line->y*z;
        if(line->type == gfx_moveTo) {
        } else if(line->type == gfx_lineTo) {
            gfxpoly_add_edge(p, lastx, lasty, x, y);
	} else if(line->type == gfx_splineTo) {
            int parts = (int)(sqrt(fabs(line->x-2*line->sx+lastx) + 
                                   fabs(line->y-2*line->sy+lasty))*subfraction);
            if(!parts) parts = 1;
	    double stepsize = 1.0/parts;
            int i;
	    for(i=0;i<parts;i++) {
		double t = (double)i*stepsize;
		double sx = (line->x*t*t + 2*line->sx*t*(1-t) + x*(1-t)*(1-t))*z;
		double sy = (line->y*t*t + 2*line->sy*t*(1-t) + y*(1-t)*(1-t))*z;
                gfxpoly_add_edge(p, lastx, lasty, sx, sy);
                lastx = sx;
                lasty = sy;
	    }
            gfxpoly_add_edge(p, lastx, lasty, x, y);
        }
        lastx = x;
        lasty = y;
        line = line->next;
    }

    gfxline_free(line);
    return p;
}

static char* readline(FILE*fi)
{
    char c;
    while(1) {
        int l = fread(&c, 1, 1, fi);
        if(!l)
            return 0;
        if(c!=10 || c!=13)
            break;
    }
    char line[256];
    int pos = 0;
    while(1) {
        line[pos++] = c;
        line[pos] = 0;
        int l = fread(&c, 1, 1, fi);
        if(!l || c==10 || c==13) {
            return strdup(line);
        }
    }
}

gfxpoly_t* gfxpoly_from_file(const char*filename, double gridsize)
{
    gfxpoly_t*p = gfxpoly_new(gridsize);

    double z = 1.0 / gridsize;

    FILE*fi = fopen(filename, "rb");
    if(!fi) {
        perror(filename);
        return 0;
    }
    int count = 0;
    double lastx=0,lasty=0;
    while(1) {
        char*line = readline(fi);
        if(!line)
            break;
        double x,y;
        char s[256];
        if(sscanf(line, "%lf %lf %s", &x, &y, &s) == 3) {
            x*=z;
            y*=z;
            if(s && !strcmp(s,"moveto")) {
                count++;
            } else if(s && !strcmp(s,"lineto")) {
                gfxpoly_add_edge(p, lastx, lasty, x, y);
                count++;
            } else {
                printf("invalid command: %s\n", s);
            }
            lastx = x;
            lasty = y;
        }
        free(line);
    }
    fclose(fi);
    printf("loaded %d points from %s\n", count, filename);
    return p;
}
