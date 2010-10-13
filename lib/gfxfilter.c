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
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mem.h"
#include "gfxfilter.h"
#include "devices/record.h"
#include "q.h"

typedef struct _internal {
    gfxfilter_t*filter;
    gfxdevice_t*out;

    /* for two pass filters: */
    gfxdevice_t*final_out;
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
static void filter_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->drawlink(i->filter, line, action, text, i->out);
}
static void filter_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->filter->endpage(i->filter, i->out);
}
static gfxresult_t* filter_finish(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxresult_t*r;
    if(i->filter->finish) {
	r = i->filter->finish(i->filter, i->out);
    } else {
	r = i->out->finish(i->out);
    }
    if(i->filter->internal) {
	free(i->filter->internal);
	i->filter->internal = 0;
    }
    free(i->filter);i->filter=0;
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
static void passthrough_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->drawlink(i->out, line, action, text);
}
static void passthrough_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    i->out->endpage(i->out);
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
static void discard_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text)
{
}
static void discard_endpage(gfxdevice_t*dev)
{
}
static gfxresult_t* discard_finish(gfxdevice_t*dev)
{
    return 0;
}

gfxdevice_t*gfxfilter_apply(gfxfilter_t*_filter, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    gfxdevice_t*dev = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));
    gfxfilter_t*filter = (gfxfilter_t*)rfx_alloc(sizeof(gfxfilter_t));
    memcpy(filter, _filter, sizeof(gfxfilter_t));
    
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
    dev->finish = filter_finish;
    return dev;
}

static void setup_twopass(gfxdevice_t*dev, gfxfilter_t*filter)
{
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
}

static gfxresult_t* twopass_finish(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;

    gfxresult_t*r;
    if(i->filter->finish) {
	r = i->filter->finish(i->filter, i->out);
    } else {
	r = i->out->finish(i->out);
    }

    if(i->pass == i->num_passes) {
	free(i->twopass);
	i->twopass = 0;
	i->filter = 0;
	free(i);
	dev->internal=0;
	free(dev);
	return r;
    }

    /* switch to next pass filter */
    i->filter = &i->twopass->pass2;
    setup_twopass(dev, i->filter);
    dev->finish = twopass_finish;

    if(i->pass == i->num_passes-1) {
	/* we don't record in the final pass- we just stream out to the 
	   next output device */
	i->out = i->final_out;
    } else {
        // switch to a new tempfile- this only happens for 3 passes or more
	assert(i->num_passes>2);
	gfxdevice_record_init(&i->record, /*use tempfile*/1);
	i->out = &i->record;
    }

    i->pass++;
    gfxresult_record_replay(r, dev, 0);
    r->destroy(r);

    return twopass_finish(dev);
}

gfxdevice_t*gfxtwopassfilter_apply(gfxtwopassfilter_t*_twopass, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    gfxdevice_t*dev = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));

    gfxtwopassfilter_t*twopass = (gfxtwopassfilter_t*)rfx_alloc(sizeof(gfxtwopassfilter_t));
    memcpy(twopass, _twopass, sizeof(gfxtwopassfilter_t));
   
    gfxdevice_record_init(&i->record, /*use tempfile*/1);

    i->out = &i->record;
    i->final_out = out;
    i->twopass = twopass;
    i->pass = 1;
    i->num_passes = 2;
    
    dev->internal = i;
   
    i->filter = &twopass->pass1;
    setup_twopass(dev, i->filter);
    dev->finish = twopass_finish;

    return dev;
}

gfxfilterchain_t* gfxfilterchain_parse(const char*_filterexpr)
{
    char*filterexpr = strdup(_filterexpr);    
    char*f = filterexpr;
    char*end = filterexpr+strlen(filterexpr);
    dict_t* params = dict_new2(&charptr_type);
    char*cmd = 0;

    gfxfilterchain_t*chain = 0;
    gfxfilterchain_t*next = 0;

    if(!*f)
	return 0;

    while(1) {
	char* eq = strchr(f, '=');
	char* colon = strchr(f, ':');
	char lastitem = 0;
	if(!colon) {
	    colon = end;
	    lastitem = 1;
	}
	*colon = 0;
	char*newcmd = 0;
	char param = 0;
	
	/* fixme: change this from a dict_t to gfxparams_t? */
	if(eq && eq < colon) { // parameter
	    *eq = 0;
	    if(!cmd) {
		fprintf(stderr, "Error: need a filter before specifying parameters (%s=%s)\n", f, eq+1);
		return 0;
	    }
	    dict_put(params, f, strdup(eq+1));
	    param = 1;
	} else {
	    newcmd = f;
	}
	if(!param || lastitem) {
	    if(!cmd && lastitem) 
		cmd = newcmd;
	    assert(cmd);
	    gfxfilterbase_t*f = 0;
	    if(!strcmp(cmd, "maketransparent")) {
		char*alphastr = dict_lookup(params, "alpha");
		int alpha = 255;
		if(alphastr) alpha=atoi(alphastr);
		f = malloc(sizeof(gfxfilter_t));
		gfxfilter_maketransparent_init((gfxfilter_t*)f, alpha);
	    } else if(!strcmp(cmd, "remove_font_transforms")) {
		f = malloc(sizeof(gfxtwopassfilter_t));
		gfxtwopassfilter_remove_font_transforms_init((gfxtwopassfilter_t*)f);
	    } else if(!strcmp(cmd, "vectors_to_glyphs")) {
		f = malloc(sizeof(gfxtwopassfilter_t));
		gfxtwopassfilter_vectors_to_glyphs_init((gfxtwopassfilter_t*)f);
	    } else if(!strcmp(cmd, "one_big_font")) {
		f = malloc(sizeof(gfxtwopassfilter_t));
		gfxtwopassfilter_one_big_font_init((gfxtwopassfilter_t*)f);
	    } else {
		fprintf(stderr, "Unknown filter: %s\n", cmd);
		return 0;
	    }
	    dict_clear(params);
	    gfxfilterchain_t*n = rfx_calloc(sizeof(gfxfilterchain_t));
	    if(!chain) {
		chain = next = n;
	    } else {
		next->next = n;
		next = n;
	    }
	    n->filter = f;

	    cmd = newcmd;
	    if(lastitem) break;
	}
	f = colon+1;
    }
    dict_destroy(params);
    return chain;
}

gfxdevice_t* gfxfilterchain_apply(gfxfilterchain_t*chain, gfxdevice_t*dev)
{
    while(chain) {
	if(chain->filter->type == gfxfilter_onepass) {
	    dev = gfxfilter_apply((gfxfilter_t*)chain->filter, dev);
	} else if(chain->filter->type == gfxfilter_twopass) {
	    dev = gfxtwopassfilter_apply((gfxtwopassfilter_t*)chain->filter, dev);
	} else {
	    fprintf(stderr, "Internal error in gfxfilterchain_apply- unknown filter type %d\n", chain->filter->type);
	}
	chain = chain->next;
    }
    return dev;
}

void gfxfilterchain_destroy(gfxfilterchain_t*chain)
{
    while(chain) {
	gfxfilterchain_t*next = chain->next;
	free(chain);
	chain = next;
    }
}
