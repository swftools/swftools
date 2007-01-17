/* rescale.c

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
#include <unistd.h>
#include <memory.h>
#include "../types.h"
#include "../mem.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"

typedef struct _internal {
    gfxdevice_t*out;
    int targetwidth;
    int targetheight;
    gfxmatrix_t matrix;
    double zoomwidth;
} internal_t;

static int verbose = 1;
static void dbg(char*format, ...)
{
    if(!verbose)
	return;
    char buf[1024];
    int l;
    va_list arglist;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    printf("(device-rescale) %s\n", buf);
    fflush(stdout);
}

gfxline_t*transformgfxline(internal_t*i, gfxline_t*line)
{
    gfxline_t*line2 = gfxline_clone(line);
    gfxline_transform(line2, &i->matrix);
    return line2;
}

int rescale_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    return i->out->setparameter(i->out,key,value);
}

void rescale_startpage(gfxdevice_t*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    i->matrix.m00 = (double)i->targetwidth / (double)width;
    i->matrix.m11 = (double)i->targetheight / (double)height;
    i->out->startpage(i->out,i->targetwidth,i->targetheight);
}

void rescale_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxline_t*line2 = transformgfxline(i, line);
    i->out->startclip(i->out,line2);
    gfxline_free(line2);
}

void rescale_endclip(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->endclip(i->out);
}

void rescale_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxline_t*line2 = transformgfxline(i, line);
    i->out->stroke(i->out, line2, width*i->zoomwidth, color, cap_style, joint_style, miterLimit);
    gfxline_free(line2);
}

void rescale_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxline_t*line2 = transformgfxline(i, line);
    i->out->fill(i->out, line2, color);
    gfxline_free(line2);
}

void rescale_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxline_t*line2 = transformgfxline(i, line);
    gfxmatrix_t m2;
    gfxmatrix_multiply(&i->matrix, matrix, &m2);
    i->out->fillbitmap(i->out, line2, img, &m2, cxform);
    gfxline_free(line2);
}

void rescale_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxline_t*line2 = transformgfxline(i, line);
    i->out->fillgradient(i->out, line, gradient, type, matrix);
    gfxline_free(line2);
}

void rescale_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->addfont(i->out, font);
}

void rescale_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxmatrix_t m2;
    gfxmatrix_multiply(&i->matrix, matrix, &m2);
    i->out->drawchar(i->out, font, glyphnr, color, &m2);
}

void rescale_drawlink(gfxdevice_t*dev, gfxline_t*line, char*action)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxline_t*line2 = transformgfxline(i, line);
    i->out->drawlink(i->out, line, action);
    gfxline_free(line2);
}

void rescale_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->endpage(i->out);
}

gfxresult_t* rescale_finish(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxdevice_t*out = i->out;
    free(dev->internal);dev->internal = 0;i=0;
    return out->finish(out);
}

void gfxdevice_rescale_init(gfxdevice_t*dev, gfxdevice_t*out, int width, int height)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));

    dev->name = "rescale";

    dev->internal = i;

    dev->setparameter = rescale_setparameter;
    dev->startpage = rescale_startpage;
    dev->startclip = rescale_startclip;
    dev->endclip = rescale_endclip;
    dev->stroke = rescale_stroke;
    dev->fill = rescale_fill;
    dev->fillbitmap = rescale_fillbitmap;
    dev->fillgradient = rescale_fillgradient;
    dev->addfont = rescale_addfont;
    dev->drawchar = rescale_drawchar;
    dev->drawlink = rescale_drawlink;
    dev->endpage = rescale_endpage;
    dev->finish = rescale_finish;

    gfxmatrix_unit(&i->matrix);
    i->targetwidth = width;
    i->targetheight = height;
    i->zoomwidth = 1.0;

    i->out = out;
}

