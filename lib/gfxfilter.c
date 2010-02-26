/* gfxfilter.c

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
#include <string.h>
#include <assert.h>
#include "mem.h"
#include "gfxfilter.h"
#include "devices/record.h"

typedef struct _internal {
    gfxfilter_t*filter;
    gfxdevice_t*out;
    gfxdevice_t*final_out;

    /* for two pass filters: */
    int pass;
    int num_passes;
    gfxdevice_t record;
    gfxtwopassfilter_t*twopass;
} internal_t;

static int filter_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    return i->filter->setparameter(i->filter, key, value, i->out);
}
static void filter_startpage(gfxdevice_t*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->startpage(i->filter, width, height, i->out);
}
static void filter_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->startclip(i->filter, line, i->out);
}
static void filter_endclip(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->endclip(i->filter, i->out);
}
static void filter_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->stroke(i->filter, line, width, color, cap_style, joint_style, miterLimit, i->out);
}
static void filter_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->fill(i->filter, line, color, i->out);
}
static void filter_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->fillbitmap(i->filter, line, img, matrix, cxform, i->out);
}
static void filter_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->fillgradient(i->filter, line, gradient, type, matrix, i->out);
}
static void filter_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->addfont(i->filter, font, i->out);
}
static void filter_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->drawchar(i->filter, font, glyphnr, color, matrix, i->out);
}
static void filter_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->drawlink(i->filter, line, action, i->out);
}
static void filter_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->endpage(i->filter, i->out);
}
static gfxresult_t* filter_finish(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxresult_t*r = i->filter->finish(i->filter, i->out);
    free(dev->internal);dev->internal=0;free(dev);
    return r;
}


static int passthrough_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    return i->out->setparameter(i->out, key, value);
}
static void passthrough_startpage(gfxdevice_t*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->startpage(i->out, width, height);
}
static void passthrough_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->startclip(i->out, line);
}
static void passthrough_endclip(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->endclip(i->out);
}
static void passthrough_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->stroke(i->out, line, width, color, cap_style, joint_style, miterLimit);
}
static void passthrough_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->fill(i->out, line, color);
}
static void passthrough_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->fillbitmap(i->out, line, img, matrix, cxform);
}
static void passthrough_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->fillgradient(i->out, line, gradient, type, matrix);
}
static void passthrough_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->addfont(i->out, font);
}
static void passthrough_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->drawchar(i->out, font, glyphnr, color, matrix);
}
static void passthrough_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->drawlink(i->out, line, action);
}
static void passthrough_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->endpage(i->out);
}
gfxresult_t* passthrough_finish(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxdevice_t*out = i->out;
    free(dev->internal);dev->internal=0;free(dev);
    return out->finish(out);
}

int discard_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    return 0;
}
static void discard_startpage(gfxdevice_t*dev, int width, int height)
{
}
static void discard_startclip(gfxdevice_t*dev, gfxline_t*line)
{
}
static void discard_endclip(gfxdevice_t*dev)
{
}
static void discard_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
}
static void discard_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
}
static void discard_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
}
static void discard_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
}
static void discard_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
}
static void discard_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
}
static void discard_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action)
{
}
static void discard_endpage(gfxdevice_t*dev)
{
}
static gfxresult_t* discard_finish(gfxdevice_t*dev)
{
    return 0;
}

gfxdevice_t*gfxfilter_apply(gfxfilter_t*filter, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    gfxdevice_t*dev = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));
    
    i->out = out;
    i->filter = filter;
    i->pass = 1;

    dev->internal = i;
    dev->name = filter->name?filter->name:"filter";
    dev->setparameter = filter->setparameter?filter_setparameter:passthrough_setparameter;
    dev->startpage = filter->startpage?filter_startpage:passthrough_startpage;
    dev->startclip = filter->startclip?filter_startclip:passthrough_startclip;
    dev->endclip = filter->endclip?filter_endclip:passthrough_endclip;
    dev->stroke = filter->stroke?filter_stroke:passthrough_stroke;
    dev->fill = filter->fill?filter_fill:passthrough_fill;
    dev->fillbitmap = filter->fillbitmap?filter_fillbitmap:passthrough_fillbitmap;
    dev->fillgradient = filter->fillgradient?filter_fillgradient:passthrough_fillgradient;
    dev->addfont = filter->addfont?filter_addfont:passthrough_addfont;
    dev->drawchar = filter->drawchar?filter_drawchar:passthrough_drawchar;
    dev->drawlink = filter->drawlink?filter_drawlink:passthrough_drawlink;
    dev->endpage = filter->endpage?filter_endpage:passthrough_endpage;
    dev->finish = filter->finish?filter_finish:passthrough_finish;
    return dev;
}

static void setup_twopass(gfxdevice_t*, gfxfilter_t*filter, char passthrough);

static gfxresult_t* twopass_finish(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
  
    assert(!strcmp(i->out->name, "record"));
   
    gfxresult_t*r;
    if(i->filter->finish) {
	r = i->filter->finish(i->filter, i->out);
    } else {
	r = i->out->finish(i->out);
    }

    if(i->pass == i->num_passes) {
	/* this output device was not a record device, so we don't have
	   to do anything here (like cleanup) */
	return r;
    }

    /* switch to next pass filter */
    i->filter = &i->twopass->pass2;

    if(i->pass == i->num_passes-1) {
	/* we don't record in the final pass- we just stream out to the 
	   next output device */
	i->out = i->final_out;
    } else {
        // this only happens for 3 passes or more
	assert(i->num_passes>2);
	gfxdevice_record_init(&i->record, /*use tempfile*/1);
	i->out = &i->record;
    }

    i->pass++;
    gfxresult_record_replay(r, i->out);
    r = i->out->finish(i->out);

    return r;
}

gfxdevice_t*gfxtwopassfilter_apply(gfxtwopassfilter_t*twopass, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    gfxdevice_t*dev = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));
   
    gfxdevice_record_init(&i->record, /*use tempfile*/1);

    i->out = &i->record;
    i->final_out = out;
    i->filter = &twopass->pass1;
    i->twopass = twopass;
    i->pass = 1;
    i->num_passes = 2;
	
    dev->setparameter = i->filter->setparameter?filter_setparameter:passthrough_setparameter;
    dev->startpage = i->filter->startpage?filter_startpage:passthrough_startpage;
    dev->startclip = i->filter->startclip?filter_startclip:passthrough_startclip;
    dev->endclip = i->filter->endclip?filter_endclip:passthrough_endclip;
    dev->stroke = i->filter->stroke?filter_stroke:passthrough_stroke;
    dev->fill = i->filter->fill?filter_fill:passthrough_fill;
    dev->fillbitmap = i->filter->fillbitmap?filter_fillbitmap:passthrough_fillbitmap;
    dev->fillgradient = i->filter->fillgradient?filter_fillgradient:passthrough_fillgradient;
    dev->addfont = i->filter->addfont?filter_addfont:passthrough_addfont;
    dev->drawchar = i->filter->drawchar?filter_drawchar:passthrough_drawchar;
    dev->drawlink = i->filter->drawlink?filter_drawlink:passthrough_drawlink;
    dev->endpage = i->filter->endpage?filter_endpage:passthrough_endpage;
    dev->finish = twopass_finish;

    dev->internal = i;
    dev->name = i->filter->name?i->filter->name:"filter";
    dev->finish = twopass_finish;
    return dev;
}

