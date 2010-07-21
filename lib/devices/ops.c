/* ops.c

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
#include "ops.h"

typedef struct _internal {
    gfxdevice_t*out;
    U8 alpha;
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
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    printf("(device-ops) %s\n", buf);
    fflush(stdout);
}

inline gfxcolor_t transform_color(internal_t*i, gfxcolor_t*col)
{
    gfxcolor_t col2;
    /*col2.r = (col->r * i->alpha)>>8;
    col2.g = (col->g * i->alpha)>>8;
    col2.b = (col->b * i->alpha)>>8;*/
    col2.r = col->r;
    col2.g = col->g;
    col2.b = col->b;
    col2.a = (col->a * i->alpha)/255;
    return col2;
}

int ops_setparameter(struct _gfxdevice*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    return i->out->setparameter(i->out,key,value);
}

void ops_startpage(struct _gfxdevice*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->startpage(i->out,width,height);
}

void ops_startclip(struct _gfxdevice*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->startclip(i->out,line);
}

void ops_endclip(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->endclip(i->out);
}

void ops_stroke(struct _gfxdevice*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxcolor_t color2 = transform_color(i, color);
    i->out->stroke(i->out, line, width, &color2, cap_style, joint_style, miterLimit);
}

void ops_fill(struct _gfxdevice*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxcolor_t color2 = transform_color(i, color);
    i->out->fill(i->out, line, &color2);
}

void ops_fillbitmap(struct _gfxdevice*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    gfximage_t img2;
    img2.width = img->width;
    img2.height = img->height;
    img2.data = (gfxcolor_t*)malloc(img->width*img->height*4);
    int x,y; 
    for(y=0;y<img->height;y++)  {
	gfxcolor_t*in = &img->data[y*img->width];
	gfxcolor_t*out = &img2.data[y*img->width];
	for(x=0;x<img->width;x++) {
	    out[x] = transform_color(i, &in[x]);
	}
    }
    i->out->fillbitmap(i->out, line, &img2, matrix, cxform);
    free(img2.data);
}

void ops_fillgradient(struct _gfxdevice*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->fillgradient(i->out, line, gradient, type, matrix);
}

void ops_addfont(struct _gfxdevice*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->addfont(i->out, font);
}

void ops_drawchar(struct _gfxdevice*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxcolor_t color2 = transform_color(i, color);
    i->out->drawchar(i->out, font, glyphnr, color, matrix);
}

void ops_drawlink(struct _gfxdevice*dev, gfxline_t*line, const char*action, const char*text)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->drawlink(i->out, line, action, text);
}

void ops_endpage(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->endpage(i->out);
}

gfxresult_t* ops_finish(struct _gfxdevice*dev)
{
    if(dev->internal) 
	free(dev->internal);
    dev->internal = 0;
    return 0;
}

void gfxdevice_ops_init(gfxdevice_t*dev, gfxdevice_t*out, U8 alpha)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));

    dev->name = "ops";

    dev->internal = i;

    dev->setparameter = ops_setparameter;
    dev->startpage = ops_startpage;
    dev->startclip = ops_startclip;
    dev->endclip = ops_endclip;
    dev->stroke = ops_stroke;
    dev->fill = ops_fill;
    dev->fillbitmap = ops_fillbitmap;
    dev->fillgradient = ops_fillgradient;
    dev->addfont = ops_addfont;
    dev->drawchar = ops_drawchar;
    dev->drawlink = ops_drawlink;
    dev->endpage = ops_endpage;
    dev->finish = ops_finish;

    i->out = out;
    i->alpha = alpha;
}

