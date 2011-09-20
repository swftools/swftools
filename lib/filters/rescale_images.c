/* rescale_images.c

   Part of the swftools package.

   Copyright (c) 2011 Matthias Kramm <kramm@quiss.org> 
 
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
#include <memory.h>
#include <string.h>
#include <math.h>
#include "../mem.h"
#include "../gfxdevice.h"
#include "../gfxfilter.h"
#include "../gfxtools.h"
#include "../gfximage.h"
#include "../log.h"

typedef struct _internal {
    double config_subpixels;
} internal_t;

int rescale_images_setparameter(gfxfilter_t*dev, const char*key, const char*value, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!strcmp(key, "subpixels")) {
        i->config_subpixels  = atof(value);
    }
    return out->setparameter(out,key,value);
}

void rescale_images_startpage(gfxfilter_t*dev, int width, int height, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)dev->internal;
    out->startpage(out,width,height);
}

void rescale_images_fillbitmap(gfxfilter_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)dev->internal;

    if(img->width<=0 || img->height<=0)
        return;

    int target_width = (int)(sqrt(matrix->m00*matrix->m00 + matrix->m01*matrix->m01)*img->width);
    int target_height = (int)(sqrt(matrix->m10*matrix->m10 + matrix->m11*matrix->m11)*img->height);

    int new_width = (int)ceil(target_width*i->config_subpixels);
    int new_height = (int)ceil(target_height*i->config_subpixels);

    if(new_width<=0)
	new_width = 1;
    if(new_height<=0)
	new_height = 1;

    if(new_width < img->width || new_height < img->height) {
	msg("<verbose> Scaling %dx%d image to %dx%d", img->width, img->height, new_width, new_height);
	gfximage_t*new_image = gfximage_rescale(img, new_width, new_height);
        gfxmatrix_t m = *matrix;
        m.m00 = (m.m00 * img->width) / new_width;
        m.m01 = (m.m01 * img->width) / new_width;
        m.m10 = (m.m10 * img->height) / new_height;
        m.m11 = (m.m11 * img->height) / new_height;
        out->fillbitmap(out, line, new_image, &m, cxform);
        gfximage_free(new_image);
    } else {
        out->fillbitmap(out, line, img, matrix, cxform);
    }
}

gfxresult_t* rescale_images_finish(gfxfilter_t*dev, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)dev->internal;
    free(i);dev->internal = 0;
    return out->finish(out);
}

void gfxfilter_rescale_images_init(gfxfilter_t*f)
{
    memset(f, 0, sizeof(gfxfilter_t));
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));

    i->config_subpixels = 1.0;

    f->internal = i;
    f->name = "rescale_images";
    f->type = gfxfilter_onepass;

    f->fillbitmap = rescale_images_fillbitmap;
    f->finish = rescale_images_finish;
}
