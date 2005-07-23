/* gfxtools.h 

   Various utility functions for dealing with gfxdevices (header file).

   Part of the swftools package.

   Copyright (c) 2005 Matthias Kramm <kramm@quiss.org> 

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifndef __gfxtools_h__
#define __gfxtools_h__

#ifdef __cplusplus
extern "C" {
#endif

//#include "../lib/mem.h"
#include "../lib/rfxswf.h"
#include "../lib/gfxdevice.h"

typedef struct _gfxdrawer
{
    void*internal;
    gfxcoord_t x,y;
    void (*moveTo)(struct _gfxdrawer*d, gfxcoord_t x, gfxcoord_t y);
    void (*lineTo)(struct _gfxdrawer*d, gfxcoord_t x, gfxcoord_t y);
    void (*splineTo)(struct _gfxdrawer*d, gfxcoord_t sx, gfxcoord_t sy, gfxcoord_t x, gfxcoord_t y);
    void* (*result)(struct _gfxdrawer*d);
} gfxdrawer_t;

typedef struct _gfxpoint
{
    gfxcoord_t x,y;
} gfxpoint_t;

void gfxdrawer_target_gfxline(gfxdrawer_t*d);

void gfxtool_draw_dashed_line(gfxdrawer_t*d, gfxline_t*line, float*dashes, float phase);
gfxline_t* gfxtool_dash_line(gfxline_t*line, float*dashes, float phase);

void gfxline_dump(gfxline_t*l, FILE*fi, char*prefix);
gfxline_t* gfxline_append(gfxline_t*line1, gfxline_t*line2);
void gfxline_free(gfxline_t*l);
gfxline_t* gfxline_clone(gfxline_t*line);

void gfxdraw_cubicTo(gfxdrawer_t*draw, double c1x, double c1y, double c2x, double c2y, double x, double y, double quality);
void gfxdraw_conicTo(gfxdrawer_t*draw, double cx, double cy, double tox, double toy, double quality);

gfxbbox_t gfxline_getbbox(gfxline_t*line);
gfxbbox_t gfxbbox_expand_to_point(gfxbbox_t box, gfxcoord_t x, gfxcoord_t y);

void gfxline_transform(gfxline_t*line, gfxmatrix_t*matrix);

void gfxmatrix_dump(gfxmatrix_t*l, FILE*fi, char*prefix);

#ifdef __cplusplus
}
#endif

#endif //__gfxtools_h__
