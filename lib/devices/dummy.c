/* dummy.c

   Part of the swftools package.

   Copyright (c) 2007 Matthias Kramm <kramm@quiss.org> 
 
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
#include <unistd.h>
#include <memory.h>
#include "../types.h"
#include "../mem.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"

typedef struct _internal {
    gfxdevice_t*out;
} internal_t;

int dummy_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out) {
	return i->out->setparameter(i->out,key,value);
    } else {
	return 0;
    }
}

void dummy_startpage(gfxdevice_t*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->startpage(i->out,width,height);
}
void dummy_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->startclip(i->out,line);
}
void dummy_endclip(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->endclip(i->out);
}
void dummy_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->stroke(i->out, line, width, color, cap_style, joint_style, miterLimit);
}

void dummy_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->fill(i->out, line, color);
}

void dummy_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->fillbitmap(i->out, line, img, matrix, cxform);
}

void dummy_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->fillgradient(i->out, line, gradient, type, matrix);
}

void dummy_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->addfont(i->out, font);
}

void dummy_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->drawchar(i->out, font, glyphnr, color, matrix);
}

void dummy_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->drawlink(i->out, line, action, text);
}

void dummy_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out)
	i->out->endpage(i->out);
}

gfxresult_t* dummy_finish(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    if(i->out) {
	gfxdevice_t*out = i->out;
	free(dev->internal);dev->internal = 0;i=0;
	return out->finish(out);
    } else {
	free(dev->internal);dev->internal = 0;i=0;
	return 0;
    }
}

void gfxdevice_dummy_init(gfxdevice_t*dev, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));

    dev->name = "dummy";

    dev->internal = i;

    dev->setparameter = dummy_setparameter;
    dev->startpage = dummy_startpage;
    dev->startclip = dummy_startclip;
    dev->endclip = dummy_endclip;
    dev->stroke = dummy_stroke;
    dev->fill = dummy_fill;
    dev->fillbitmap = dummy_fillbitmap;
    dev->fillgradient = dummy_fillgradient;
    dev->addfont = dummy_addfont;
    dev->drawchar = dummy_drawchar;
    dev->drawlink = dummy_drawlink;
    dev->endpage = dummy_endpage;
    dev->finish = dummy_finish;

    i->out = out;
}

