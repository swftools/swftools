#ifndef __poly_convert_h__
#define __poly_convert_h__

#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "poly.h"

typedef struct _polywriter
{
    void(*moveto)(struct _polywriter*, int32_t x, int32_t y);
    void(*lineto)(struct _polywriter*, int32_t x, int32_t y);
    void(*setgridsize)(struct _polywriter*, double g);
    void*(*finish)(struct _polywriter*);
    void*internal;
} polywriter_t;

void gfxdrawer_target_poly(gfxdrawer_t*d, double gridsize);

void gfxpolywriter_init(polywriter_t*w);
gfxpoly_t* gfxpoly_from_fill(gfxline_t*line, double gridsize);
gfxpoly_t* gfxpoly_from_file(const char*filename, double gridsize);
void gfxpoly_destroy(gfxpoly_t*poly);

gfxline_t*gfxline_from_gfxpoly(gfxpoly_t*poly);
gfxline_t*gfxline_from_gfxpoly_with_direction(gfxpoly_t*poly); // preserves up/down

gfxline_t* gfxpoly_circular_to_evenodd(gfxline_t*line, double gridsize);
gfxpoly_t* gfxpoly_createbox(double x1, double y1,double x2, double y2, double gridsize);

#endif //__poly_convert_h__
