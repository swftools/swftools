/* gfxpoly.h 

   Various boolean polygon functions.

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

#ifndef __gfxpoly_h__
#define __gfxpoly_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "mem.h"
#include "gfxdevice.h"
#include "gfxtools.h"

/* A "grid" value is the granularity at which polygon intersection operates.
   It usually makes sense to set this to the smallest value that can actually be represented
   in the output device (like 0.05 = 1 twip for SWF).  */
#define DEFAULT_GRID (0.05)

typedef struct _gfxpoly {
} gfxpoly_t;

void gfxpoly_destroy(gfxpoly_t*poly);

/* constructors */
gfxpoly_t* gfxpoly_from_fill(gfxline_t*line, double gridsize);
gfxpoly_t* gfxpoly_from_stroke(gfxline_t*line, gfxcoord_t width, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit, double gridsize);

/* operators */
gfxpoly_t* gfxpoly_intersect(gfxpoly_t*p1, gfxpoly_t*p2);
gfxpoly_t* gfxpoly_union(gfxpoly_t*p1, gfxpoly_t*p2);

/* area functions */
double gfxpoly_area(gfxpoly_t*p);
double gfxpoly_intersection_area(gfxpoly_t*p1, gfxpoly_t*p2);

/* conversion functions */
gfxpoly_t* gfxpoly_createbox(double x1, double y1,double x2, double y2, double gridsize);
gfxline_t* gfxline_from_gfxpoly(gfxpoly_t*poly);
gfxline_t* gfxline_from_gfxpoly_with_direction(gfxpoly_t*poly);
gfxline_t* gfxpoly_circular_to_evenodd(gfxline_t*line, double gridsize);

#ifdef __cplusplus
}
#endif

#endif //__gfxpoly_h__

