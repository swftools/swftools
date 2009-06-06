#ifndef __stroke_h__
#define __stroke_h__
#include "../gfxdevice.h"
#include "poly.h"
gfxpoly_t* gfxpoly_from_stroke(gfxline_t*line, gfxcoord_t width, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit, double gridsize);
void draw_stroke(gfxline_t*start, gfxdrawer_t*draw, double width, gfx_capType cap, gfx_joinType join, double miterLimit);
#endif
