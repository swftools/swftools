#ifndef __poly_convert_h__
#define __poly_convert_h__

#include "../gfxdevice.h"
#include "poly.h"

gfxpoly_t* gfxpoly_from_gfxline(gfxline_t*line, double gridsize);
gfxpoly_t* gfxpoly_from_file(const char*filename, double gridsize);
gfxpoly_t* gfxpoly_from_gfxcompactpoly(gfxcompactpoly_t*poly);

typedef struct _polywriter
{
    void(*moveto)(struct _polywriter*, int x, int y);
    void(*lineto)(struct _polywriter*, int x, int y);
    void(*setgridsize)(struct _polywriter*, double g);
    void*(*finish)(struct _polywriter*);
    void*internal;
} polywriter_t;

void gfxpolywriter_init(polywriter_t*w);
void gfxcompactpolywriter_init(polywriter_t*w);

gfxcompactpoly_t* gfxcompactpoly_from_gfxline(gfxline_t*line, double gridsize);
gfxcompactpoly_t* gfxcompactpoly_from_file(const char*filename, double gridsize);

void gfxcompactpoly_destroy(gfxcompactpoly_t*poly);

#endif //__poly_convert_h__
