#ifndef __artsutils_h__
#define __artsutils_h__

#include "../../config.h"
#include "../art/libart.h"

ArtVpath* gfxline_to_ArtVpath(gfxline_t*line);
ArtSVP* gfxfillToSVP(gfxline_t*line, int perturb);
void show_path(ArtSVP*path);
ArtSVP* boxToSVP(double x1, double y1,double x2, double y2);
ArtSVP* gfxstrokeToSVP(gfxline_t*line, gfxcoord_t width, gfx_capType cap_style, gfx_joinType joint_style, double miterLimit);
gfxline_t* SVPtogfxline(ArtSVP*svp);

#endif
