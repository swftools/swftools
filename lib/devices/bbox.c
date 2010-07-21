/* bbox.c

   Part of the swftools package.

   Copyright (c) 2006 Matthias Kramm <kramm@quiss.org> 
 
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
#include <stdio.h>
#include <stdarg.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <memory.h>
#include <string.h>
#include "../types.h"
#include "../mem.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"

typedef struct _internal {
    gfxbbox_t bbox;
    int do_graphics;
    int do_text;
} internal_t;

void measuregfxline(internal_t*i, gfxline_t*line)
{
    gfxbbox_t b = gfxline_getbbox(line);
    if(b.xmin==0 && b.ymin==0 && b.xmax==0 && b.ymax==0) {
	return;
    }
    i->bbox = gfxbbox_expand_to_point(i->bbox, b.xmin, b.ymin);
    i->bbox = gfxbbox_expand_to_point(i->bbox, b.xmax, b.ymax);
}

int bbox_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!strcmp(key, "graphics")) {
	i->do_graphics = atoi(value);
	return 1;
    } else if(!strcmp(key, "text")) {
	i->do_text = atoi(value);
	return 1;
    }
    return 0;
}

void bbox_startpage(gfxdevice_t*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    i->bbox.xmin = 0;
    i->bbox.ymin = 0;
    i->bbox.xmax = 0;
    i->bbox.ymax = 0;
}

void bbox_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
}

void bbox_endclip(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
}

void bbox_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->do_graphics)
	measuregfxline(i, line);
}

void bbox_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->do_graphics)
	measuregfxline(i, line);
}

void bbox_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->do_graphics)
	measuregfxline(i, line);
}

void bbox_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->do_graphics)
	measuregfxline(i, line);
}

void bbox_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
}

void bbox_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!font)
	return;

    if(i->do_text) {
	gfxglyph_t*glyph = &font->glyphs[glyphnr];
	gfxline_t*line2 = gfxline_clone(glyph->line);
	gfxline_transform(line2, matrix);
	measuregfxline(i, line2);
	gfxline_free(line2);
    }
}

void bbox_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text)
{
    internal_t*i = (internal_t*)dev->internal;
}

void bbox_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
}

gfxresult_t* bbox_finish(gfxdevice_t*dev)
{
    free(dev->internal);dev->internal = 0;
    return 0;
}

gfxbbox_t gfxdevice_bbox_getbbox(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    return i->bbox;
}

void gfxdevice_bbox_init(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));

    dev->name = "bbox";

    dev->internal = i;

    dev->setparameter = bbox_setparameter;
    dev->startpage = bbox_startpage;
    dev->startclip = bbox_startclip;
    dev->endclip = bbox_endclip;
    dev->stroke = bbox_stroke;
    dev->fill = bbox_fill;
    dev->fillbitmap = bbox_fillbitmap;
    dev->fillgradient = bbox_fillgradient;
    dev->addfont = bbox_addfont;
    dev->drawchar = bbox_drawchar;
    dev->drawlink = bbox_drawlink;
    dev->endpage = bbox_endpage;
    dev->finish = bbox_finish;

    i->do_graphics = 1;
    i->do_text = 1;
}

