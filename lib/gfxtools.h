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

#include <stdio.h>
#include "../lib/mem.h"
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

typedef struct _gfxfontlist
{
    gfxfont_t*font;
    struct _gfxfontlist*next;
} gfxfontlist_t;

void gfxdrawer_target_gfxline(gfxdrawer_t*d);

void gfxtool_draw_dashed_line(gfxdrawer_t*d, gfxline_t*line, float*dashes, float phase);
gfxline_t* gfxtool_dash_line(gfxline_t*line, float*dashes, float phase);

void gfxline_dump(gfxline_t*l, FILE*fi, char*prefix);
gfxline_t* gfxline_append(gfxline_t*line1, gfxline_t*line2);
void gfxline_free(gfxline_t*l);
gfxline_t* gfxline_clone(gfxline_t*line);
void gfxline_optimize(gfxline_t*line);

void gfxdraw_cubicTo(gfxdrawer_t*draw, double c1x, double c1y, double c2x, double c2y, double x, double y, double quality);
void gfxdraw_conicTo(gfxdrawer_t*draw, double cx, double cy, double tox, double toy, double quality);

gfxbbox_t gfxline_getbbox(gfxline_t*line);
gfxbbox_t gfxbbox_expand_to_point(gfxbbox_t box, gfxcoord_t x, gfxcoord_t y);
void gfxbbox_intersect(gfxbbox_t*box1, gfxbbox_t*box2);

void gfxline_transform(gfxline_t*line, gfxmatrix_t*matrix);

void gfxmatrix_dump(gfxmatrix_t*l, FILE*fi, char*prefix);
void gfxmatrix_transform(gfxmatrix_t*m, gfxcoord_t* v1, gfxcoord_t*dest);
void gfxmatrix_invert(gfxmatrix_t*src, gfxmatrix_t*dest);
void gfxmatrix_unit(gfxmatrix_t*m);
void gfxmatrix_multiply(gfxmatrix_t*m1, gfxmatrix_t*m2, gfxmatrix_t*dest);

gfxfontlist_t* gfxfontlist_create();
gfxfontlist_t*gfxfontlist_addfont(gfxfontlist_t*list, gfxfont_t*font);
gfxfont_t*gfxfontlist_findfont(gfxfontlist_t*list, char*id);
char gfxfontlist_hasfont(gfxfontlist_t*list, gfxfont_t*font);
void gfxfontlist_free(gfxfontlist_t*list, char deletefonts);

gfxline_t*gfxline_makerectangle(int x1, int y1, int x2, int y2);
void gfxline_show(gfxline_t*line, FILE*fi);

#ifdef __cplusplus
}
#endif

#endif //__gfxtools_h__
