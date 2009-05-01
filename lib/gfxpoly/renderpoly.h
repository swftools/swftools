#ifndef __renderpoly_h__
#define __renderpoly_h__

#include "poly.h"
#include "wind.h"

typedef struct {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
    int width;
    int height;
} intbbox_t;

unsigned char* render_polygon(gfxpoly_t*polygon, intbbox_t*bbox, double zoom, windrule_t*rule);

intbbox_t intbbox_new(int x1, int y1, int x2, int y2);
intbbox_t intbbox_from_polygon(gfxpoly_t*polygon, double zoom);

#endif
