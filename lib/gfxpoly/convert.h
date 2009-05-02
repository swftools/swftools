#ifndef __poly_convert_h__
#define __poly_convert_h__

#include "poly.h"

gfxpoly_t* gfxpoly_from_gfxline(gfxline_t*line, double gridsize);
gfxpoly_t* gfxpoly_from_file(const char*filename, double gridsize);

#endif //__poly_convert_h__
