/* flatten.c

   Part of the swftools package.

   Copyright (c) 2008-2011 Matthias Kramm <kramm@quiss.org> 
 
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
#include <assert.h>
#include <string.h>
#include <math.h>
#include "../mem.h"
#include "../gfxdevice.h"
#include "../gfxfilter.h"
#include "../gfxtools.h"
#include "../gfxpoly.h"
#include "../log.h"

typedef struct _clip {
    gfxpoly_t*poly;
    int openclips;
    struct _clip*next;
} clip_t;

typedef struct _internal {
    clip_t*clip;

    char config_dont_clip_characters;

    int good_polygons;
    int bad_polygons;
} internal_t;

static int verbose = 0;

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
    printf("(device-polyops) %s\n", buf);
    fflush(stdout);
}

int flatten_setparameter(gfxfilter_t*dev, const char*key, const char*value, gfxdevice_t*out)
{
    dbg("flatten_setparameter");
    internal_t*i = (internal_t*)dev->internal;
    if(!strcmp(key, "dont_clip_characters")) {
        i->config_dont_clip_characters = atoi(value);
    }
    return out->setparameter(out,key,value);
}

void flatten_startpage(gfxfilter_t*dev, int width, int height, gfxdevice_t*out)
{
    dbg("flatten_startpage");
    internal_t*i = (internal_t*)dev->internal;
    out->startpage(out,width,height);
}

void flatten_startclip(gfxfilter_t*dev, gfxline_t*line, gfxdevice_t*out)
{
    dbg("flatten_startclip");
    internal_t*i = (internal_t*)dev->internal;

    gfxpoly_t* oldclip = i->clip?i->clip->poly:0;
    gfxpoly_t* poly = gfxpoly_from_fill(line, DEFAULT_GRID);
    if(poly)
        i->good_polygons++;
    else
        i->bad_polygons++;

    gfxpoly_t* currentclip = 0;
    int type = 0;

    /* we can't rely on gfxpoly actually being able to convert
       a gfxline into a gfxpoly- for polygons which are too
       complex or just degenerate, this might fail. So handle
       all the cases where polygon conversion or intersection
       might go awry 
       UPDATE: this is not needed anymore. The new gfxpoly
       implementation is stable enough so it always returns
       a valid result. Still, it's good practice.
     */
    if(!poly && !oldclip) {
	out->startclip(out,line);
	currentclip = 0;
	type = 1;
    } else if(!poly && oldclip) {
	gfxline_t*oldclipline = gfxline_from_gfxpoly(oldclip);
	out->startclip(out,oldclipline);
	out->startclip(out,line);
	currentclip = 0;
	type = 2;
    } else if(poly && oldclip) {
	gfxpoly_t*intersection = gfxpoly_intersect(poly, oldclip);
	if(intersection) {
            i->good_polygons++;
	    // this case is what usually happens 
	    gfxpoly_destroy(poly);poly=0;
	    currentclip = intersection;
	    type = 0;
	} else {
            i->bad_polygons++;
	    gfxline_t*oldclipline = gfxline_from_gfxpoly(oldclip);
	    out->startclip(out, oldclipline);
	    currentclip = poly;
	    type = 1;
	}
    } else if(poly && !oldclip) {
	currentclip = poly;
	type = 0;
    }

    clip_t*n = i->clip;
    i->clip = (clip_t*)rfx_calloc(sizeof(clip_t));
    i->clip->next = n;
    i->clip->poly = currentclip;
    i->clip->openclips = type;
}

void flatten_endclip(gfxfilter_t*dev, gfxdevice_t*out)
{
    dbg("flatten_endclip");
    internal_t*i = (internal_t*)dev->internal;

    if(!i->clip) {
	msg("<error> endclip without startclip (in: polyops)\n");
	return;
    }

    clip_t*old = i->clip;
    i->clip = i->clip->next;
    if(old->poly) {
	gfxpoly_destroy(old->poly);old->poly = 0;
    }
    int t;
    for(t=0;t<old->openclips;t++)
	out->endclip(out);

    old->next = 0;free(old);
}

static gfxline_t* handle_poly(internal_t*i, gfxpoly_t*poly, char*ok, gfxdevice_t*out)
{
    if(i->clip && i->clip->poly) {
	gfxpoly_t*old = poly;
	if(poly) {
	    poly = gfxpoly_intersect(poly, i->clip->poly);
	    gfxpoly_destroy(old);
	}
    }

    if(poly)
        i->good_polygons++;
    else
        i->bad_polygons++;

    gfxline_t*gfxline = 0;
    if(poly) {
	// this is the case where everything went right
	gfxline_t*line = gfxline_from_gfxpoly(poly);
	gfxpoly_destroy(poly);
        *ok = 1;
	return line;
    } else {
	if(i->clip && i->clip->poly) {
	    /* convert current clipping from a polygon to an
	       actual "startclip" written to the output */
	    assert(i->clip->openclips <= 1);
	    gfxline_t*clipline = gfxline_from_gfxpoly(i->clip->poly);
	    out->startclip(out, clipline);
	    gfxline_free(clipline);
	    gfxpoly_destroy(i->clip->poly);i->clip->poly = 0;
	    i->clip->openclips++;
	    return 0;
	} else {
	    return 0;
	}
    }
}

void flatten_stroke(gfxfilter_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit, gfxdevice_t*out)
{
    dbg("flatten_stroke");
    internal_t*i = (internal_t*)dev->internal;

    gfxpoly_t* poly = gfxpoly_from_stroke(line, width, cap_style, joint_style, miterLimit, DEFAULT_GRID);
    char ok = 0;
    gfxline_t*line2 = handle_poly(i, poly, &ok, out);

    if(ok) {
	if(out && line2) out->fill(out, line2, color);
	gfxline_free(line2);
    } else {
        msg("<error> ..");
	out->stroke(out, line, width, color, cap_style, joint_style, miterLimit);
    }
}

void flatten_fill(gfxfilter_t*dev, gfxline_t*line, gfxcolor_t*color, gfxdevice_t*out)
{
    dbg("flatten_fill");
    internal_t*i = (internal_t*)dev->internal;

    gfxpoly_t*poly = gfxpoly_from_fill(line, DEFAULT_GRID);
    char ok = 0;
    gfxline_t*line2 = handle_poly(i, poly, &ok, out);

    if(ok) {
	if(out && line2) out->fill(out, line2, color);
	gfxline_free(line2);
    } else {
	out->fill(out, line, color);
    }
}

void flatten_fillbitmap(gfxfilter_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform, gfxdevice_t*out)
{
    dbg("flatten_fillbitmap");
    internal_t*i = (internal_t*)dev->internal;

    gfxpoly_t*poly = gfxpoly_from_fill(line, DEFAULT_GRID);
    char ok = 0;
    gfxline_t*line2 = handle_poly(i, poly, &ok, out);

    if(ok) {
	if(out && line2) out->fillbitmap(out, line2, img, matrix, cxform);
	gfxline_free(line2);
    } else {
	out->fillbitmap(out, line, img, matrix, cxform);
    }
}

void flatten_fillgradient(gfxfilter_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    dbg("flatten_fillgradient");
    internal_t*i = (internal_t*)dev->internal;

    gfxpoly_t*poly = gfxpoly_from_fill(line, DEFAULT_GRID);
    char ok = 0;
    gfxline_t*line2 = handle_poly(i, poly, &ok, out);

    if(ok) {
	if(out && line2) out->fillgradient(out, line2, gradient, type, matrix);
	gfxline_free(line2);
    } else {
	out->fillgradient(out, line, gradient, type, matrix);
    }
}

void flatten_addfont(gfxfilter_t*dev, gfxfont_t*font, gfxdevice_t*out)
{
    dbg("flatten_addfont");
    internal_t*i = (internal_t*)dev->internal;
    out->addfont(out, font);
}

void flatten_drawchar(gfxfilter_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    dbg("flatten_drawchar");
    if(!font)
	return;
    internal_t*i = (internal_t*)dev->internal;
    gfxline_t*glyph = gfxline_clone(font->glyphs[glyphnr].line);
    gfxline_transform(glyph, matrix);

    if(i->clip && i->clip->poly) {
	gfxbbox_t bbox = gfxline_getbbox(glyph);
	gfxpoly_t*dummybox = gfxpoly_createbox(bbox.xmin,bbox.ymin,bbox.xmax,bbox.ymax, DEFAULT_GRID);
	gfxline_t*dummybox2 = gfxline_from_gfxpoly(dummybox);
	bbox = gfxline_getbbox(dummybox2);
	gfxline_free(dummybox2);

        char ok=0;
	gfxline_t*gfxline = handle_poly(i, dummybox, &ok, out);
	if(ok) {
	    gfxbbox_t bbox2 = gfxline_getbbox(gfxline);
	    double w = bbox2.xmax - bbox2.xmin;
	    double h = bbox2.ymax - bbox2.ymin;
	    if(fabs((bbox.xmax - bbox.xmin) - w) > DEFAULT_GRID*2 ||
	       fabs((bbox.ymax - bbox.ymin) - h) > DEFAULT_GRID*2) {
		/* notable change in character size: character was clipped 
		   TODO: how to deal with diagonal cuts?
		 */
		msg("<trace> Character %d was clipped: (%f,%f,%f,%f) -> (%f,%f,%f,%f)",
			glyphnr, 
			bbox.xmin,bbox.ymin,bbox.xmax,bbox.ymax,
			bbox2.xmin,bbox2.ymin,bbox2.xmax,bbox2.ymax);
                if(!i->config_dont_clip_characters) {
		    flatten_fill(dev, glyph, color, out);
                } else {
                    if(bbox2.xmin < bbox2.xmax &&
                       bbox2.ymin < bbox2.ymax) {
		        out->drawchar(out, font, glyphnr, color, matrix);
                    }
                }
	    } else {
		out->drawchar(out, font, glyphnr, color, matrix);
	    }
	} else {
	    out->drawchar(out, font, glyphnr, color, matrix);
	}
	gfxline_free(gfxline);
    } else {
	out->drawchar(out, font, glyphnr, color, matrix);
    }

    gfxline_free(glyph);
}

void flatten_drawlink(gfxfilter_t*dev, gfxline_t*line, const char*action, const char*text, gfxdevice_t*out)
{
    dbg("flatten_drawlink");
    internal_t*i = (internal_t*)dev->internal;
    out->drawlink(out, line, action, text);
}

void flatten_endpage(gfxfilter_t*dev, gfxdevice_t*out)
{
    dbg("flatten_endpage");
    internal_t*i = (internal_t*)dev->internal;
    out->endpage(out);
}

gfxresult_t* flatten_finish(gfxfilter_t*dev, gfxdevice_t*out)
{
    dbg("flatten_finish");
    internal_t*i = (internal_t*)dev->internal;

    if(i->bad_polygons) {
        msg("<notice> --flatten success rate: %.1f%% (%d failed polygons)", i->good_polygons*100.0 / (i->good_polygons + i->bad_polygons), i->bad_polygons);
    }

    free(i);dev->internal = 0;
    return out->finish(out);
}

void gfxfilter_flatten_init(gfxfilter_t*f)
{
    dbg("flatten filter");
    memset(f, 0, sizeof(gfxfilter_t));
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));

    f->internal = i;
    f->name = "flatten";
    f->type = gfxfilter_onepass;

    f->setparameter = flatten_setparameter;
    f->startpage = flatten_startpage;
    f->startclip = flatten_startclip;
    f->endclip = flatten_endclip;
    f->stroke = flatten_stroke;
    f->fill = flatten_fill;
    f->fillbitmap = flatten_fillbitmap;
    f->fillgradient = flatten_fillgradient;
    f->addfont = flatten_addfont;
    f->drawchar = flatten_drawchar;
    f->drawlink = flatten_drawlink;
    f->endpage = flatten_endpage;
    f->finish = flatten_finish;
}
