/* alpha.c

   Part of the swftools package.

   Copyright (c) 2010 Matthias Kramm <kramm@quiss.org> 
 
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

#include <stdlib.h>
#include <memory.h>
#include "../gfxfilter.h"
#include "../gfxtools.h"
#include "../types.h"
#include "../mem.h"

typedef struct _internal {
    U8 alpha;
} internal_t;

static inline gfxcolor_t transform_color(internal_t*i, gfxcolor_t*col)
{
    gfxcolor_t col2;
    col2.r = col->r;
    col2.g = col->g;
    col2.b = col->b;
    col2.a = (col->a * i->alpha)/255;
    return col2;
}

static void maketransparent_stroke(gfxfilter_t*f, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    gfxcolor_t color2 = transform_color(i, color);
    out->stroke(out, line, width, &color2, cap_style, joint_style, miterLimit);
}
static void maketransparent_fill(gfxfilter_t*f, gfxline_t*line, gfxcolor_t*color, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    gfxcolor_t color2 = transform_color(i, color);
    out->fill(out, line, &color2);
}
static void maketransparent_fillbitmap(gfxfilter_t*f, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    gfximage_t img2;
    img2.width = img->width;
    img2.height = img->height;
    img2.data = (gfxcolor_t*)rfx_alloc(img->width*img->height*4);
    int x,y; 
    for(y=0;y<img->height;y++)  {
	gfxcolor_t*in = &img->data[y*img->width];
	gfxcolor_t*out = &img2.data[y*img->width];
	for(x=0;x<img->width;x++) {
	    out[x] = transform_color(i, &in[x]);
	}
    }
    out->fillbitmap(out, line, &img2, matrix, cxform);
    rfx_free(img2.data);
}
static void maketransparent_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    gfxcolor_t color2 = transform_color(i, color);
    out->drawchar(out, font, glyphnr, color, matrix);
}
static void maketransparent_fillgradient(gfxfilter_t*f, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    
    gfxgradient_t*g = 0, *prev = 0;

    while(gradient) {
	gfxgradient_t*n = rfx_alloc(sizeof(gfxgradient_t));
	n->pos = gradient->pos;
	n->color = transform_color(i, &gradient->color);
	n->next = 0;
	if(prev) {
	    prev->next = n;
	    prev = n;
	} else {
	    g = prev = n;
	}
	gradient = gradient->next;
    }

    out->fillgradient(out, line, g, type, matrix);
    gfxgradient_destroy(g);
}

void gfxfilter_maketransparent_init(gfxfilter_t*f, U8 alpha)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    i->alpha = alpha;

    memset(f, 0, sizeof(gfxfilter_t));
    f->type = gfxfilter_onepass;
    f->name = "maketransparent";
    f->internal = i;
    f->stroke = maketransparent_stroke;
    f->fill = maketransparent_fill;
    f->fillbitmap = maketransparent_fillbitmap;
    f->fillgradient = maketransparent_fillgradient;
    f->drawchar = maketransparent_drawchar;
}

