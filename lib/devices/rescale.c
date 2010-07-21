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
#ifndef WIN32
#include <unistd.h>
#endif
#include <memory.h>
#include <math.h>
#include <string.h>
#include "rescale.h"
#include "../types.h"
#include "../mem.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"

typedef struct _internal {
    gfxdevice_t*out;
    int origwidth;
    int origheight;
    int targetwidth;
    int targetheight;
    int centerx, centery;
    gfxmatrix_t matrix;
    double zoomwidth;
    int keepratio;
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
    printf("(device-rescale) %s\n", buf);
    fflush(stdout);
}

char gfxline_isRect(gfxline_t*line)
{
    return 1;
    if(!line)
	return 0;
    double x1=line->x,y1=line->x,x2=line->x,y2=line->y;
    int nx1=0,nx2=0,ny1=0,ny2=0;
    line = line->next;
    while(line) {
	if(line->type != gfx_lineTo)
	    return 0;
	if(line->x < x1) {
	    x1 = line->x;
	    nx1++;
	} else if(line->y < y1) {
	    y1 = line->y;
	    ny1++;
	} else if(line->x > x2) {
	    x2 = line->x;
	    nx2++;
	} else if(line->y > y1) {
	    y2 = line->y;
	    ny2++;
	}
	line = line->next;
    }
    return (nx1+nx2)==1 && (ny1+ny2)==1;
}

gfxline_t*transformgfxline(internal_t*i, gfxline_t*line)
{
    /* special case: transformed rectangle 
    if(gfxline_isRect(line)) {
	gfxbbox_t bbox = gfxline_getbbox(line);
	if(fabs(bbox.xmin)<0.1 && fabs(bbox.ymin)<0.1 &&
	   fabs(bbox.ymax-i->origwidth)<0.1 && fabs(bbox.ymax-i->origheight)<0.1) {
	    gfxline_t r[5];
	    r[0].x = 0;             r[0].y = 0;           r[0].type = gfx_moveTo;r[0].next = &r[1];
	    r[1].x = i->targetwidth;r[1].y = 0;           r[1].type = gfx_lineTo;r[1].next = &r[2];
	    r[2].x = i->targetwidth;r[2].y = i->targetheight;r[2].type = gfx_lineTo;r[2].next = &r[3];
	    r[3].x = 0;             r[3].y = i->targetheight;r[3].type = gfx_lineTo;r[3].next = &r[4];
	    r[4].x = 0;             r[4].y = 0;           r[4].type = gfx_lineTo;r[4].next = 0;
	    return gfxline_clone(r);
	}
    } */
    gfxline_t*line2 = gfxline_clone(line);
    gfxline_transform(line2, &i->matrix);
    return line2;
}

int rescale_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!strcmp(key, "keepratio")) {
	i->keepratio = atoi(value);
	return 1;
    } else if(!strcmp(key, "centerx")) {
	i->centerx = atoi(value);
	return 1;
    } else if(!strcmp(key, "centery")) {
	i->centery = atoi(value);
	return 1;
    } else {
	if(i->out) {
	    return i->out->setparameter(i->out,key,value);
	} else {
	    return 0;
	}
    }
}

void rescale_startpage(gfxdevice_t*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;

    i->origwidth = width;
    i->origheight = height;

    if(i->targetwidth || i->targetheight) {
        int targetwidth = i->targetwidth;
        if(!targetwidth)
            targetwidth = width*i->targetheight/height;
        int targetheight = i->targetheight;
        if(!targetheight)
            targetheight = height*i->targetwidth/width;
	if(i->keepratio) {
	    double rx = (double)targetwidth / (double)width;
	    double ry = (double)targetheight / (double)height;
	    if(rx<ry) {
		i->matrix.m00 = rx;
		i->matrix.m11 = rx;
		i->matrix.tx = 0;
		if(i->centery) {
		    i->matrix.ty = (targetheight - height*rx) / 2;
		}
	    } else {
		i->matrix.m00 = ry;
		i->matrix.m11 = ry;
		if(i->centerx) {
		    i->matrix.tx = (targetwidth - width*ry) / 2;
		}
		i->matrix.ty = 0;
	    }
	} else {
	    i->matrix.m00 = (double)targetwidth / (double)width;
	    i->matrix.m11 = (double)targetheight / (double)height;
	}
	i->zoomwidth = sqrt(i->matrix.m00*i->matrix.m11);
	i->out->startpage(i->out,targetwidth,targetheight);
    } else {
	i->out->startpage(i->out,(int)(width*i->matrix.m00),(int)(height*i->matrix.m11));
    }
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
    i->out->fillgradient(i->out, line2, gradient, type, matrix);
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

void rescale_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxline_t*line2 = transformgfxline(i, line);
    i->out->drawlink(i->out, line2, action, text);
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
    if(out) {
	return out->finish(out);
    } else {
	return 0;
    }
}

void gfxdevice_rescale_init(gfxdevice_t*dev, gfxdevice_t*out, int width, int height, double scale)
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
    i->centerx = 1;
	
    i->matrix.m00 = scale;
    i->matrix.m01 = 0;
    i->matrix.m11 = scale;
    i->matrix.m10 = 0;
    i->matrix.tx = 0;
    i->matrix.ty = 0;
    i->zoomwidth = scale;

    i->out = out;
}

void gfxdevice_rescale_setzoom(gfxdevice_t*dev, double scale)
{
    internal_t*i = (internal_t*)dev->internal;
    if(strcmp(dev->name, "rescale")) {
	fprintf(stderr, "Internal error: can't cast device %s to a rescale device\n", dev->name);
	return;
    }
    i->matrix.m00 = scale;
    i->matrix.m01 = 0;
    i->matrix.m11 = scale;
    i->matrix.m10 = 0;
    i->matrix.tx = 0;
    i->matrix.ty = 0;
    i->zoomwidth = scale;
}
void gfxdevice_rescale_setdevice(gfxdevice_t*dev, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)dev->internal;
    if(strcmp(dev->name, "rescale")) {
	fprintf(stderr, "Internal error: can't cast device %s to a rescale device\n", dev->name);
	return;
    }
    i->out = out;
}

gfxdevice_t* gfxdevice_rescale_new(gfxdevice_t*out, int width, int height, double scale)
{
    gfxdevice_t* d = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    gfxdevice_rescale_init(d, out, width, height, scale);
    return d;
}
