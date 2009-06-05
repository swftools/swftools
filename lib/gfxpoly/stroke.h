#ifndef __stroke_h__
#define __stroke_h__
#include "../gfxdevice.h"
#include "poly.h"
gfxpoly_t* gfxpoly_from_stroke(gfxline_t*line, gfxcoord_t width, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit, double gridsize);
#endif
