/* remove_font_transform.c

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
#include "../gfxfont.h"
#include "../types.h"
#include "../mem.h"

typedef struct _internal {
    gfxfontlist_t*fonts;
    gfxfont_t*font;
    int num_glyphs;
} internal_t;

typedef struct _fontdata {
    gfxfont_t*font;
    int start;
} fontdata_t;

static void pass1_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    fontdata_t*d = gfxfontlist_getuserdata(i->fonts, font->id);
    if(!d) {
	d = rfx_calloc(sizeof(fontdata_t));
	d->font = font;
	d->start = i->num_glyphs;
	i->num_glyphs  += font->num_glyphs;
	i->fonts = gfxfontlist_addfont2(i->fonts, font, d);
    }
    out->drawchar(out, font, glyphnr, color, matrix);
}
static gfxresult_t*pass1_finish(gfxfilter_t*f, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    gfxfont_t*font = i->font = rfx_calloc(sizeof(gfxfont_t));
    font->id = strdup("onebigfont");
    font->num_glyphs = i->num_glyphs;
    font->glyphs = rfx_calloc(sizeof(gfxglyph_t)*i->num_glyphs);
    gfxfontlist_t*l = i->fonts;
    while(l) {
	gfxfont_t*old = l->font;
	fontdata_t*d = l->user;
	memcpy(font->glyphs + d->start, old->glyphs, sizeof(gfxglyph_t)*old->num_glyphs);
	if(old->ascent > font->ascent)
	    font->ascent = old->ascent;
	if(old->descent > font->descent)
	    font->descent = old->descent;
	l = l->next;
    }
    gfxfont_fix_unicode(font, 0);
    return out->finish(out);
}

static void pass2_addfont(gfxfilter_t*f, gfxfont_t*font, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    out->addfont(out, i->font);
}
static void pass2_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    fontdata_t*d = gfxfontlist_getuserdata(i->fonts, font->id);
    out->drawchar(out, i->font, glyphnr + d->start, color, matrix);
}
static gfxresult_t*pass2_finish(gfxfilter_t*f, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    // clean up
    gfxfontlist_t*l = i->fonts;
    while(l) {
	free(l->user);l->user=0;
	l=l->next;
    }
    gfxfontlist_free(i->fonts, 0);i->fonts=0;
    return out->finish(out);
}

void gfxtwopassfilter_one_big_font_init(gfxtwopassfilter_t*f)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    
    memset(f, 0, sizeof(gfxtwopassfilter_t));
    f->type = gfxfilter_twopass;

    f->pass1.name = "filter \"one big font\" pass 1";
    f->pass1.drawchar = pass1_drawchar;
    f->pass1.finish = pass1_finish;
    f->pass1.internal = i;

    f->pass2.name = "filter \"one big font\" pass 2";
    f->pass2.addfont = pass2_addfont;
    f->pass2.drawchar = pass2_drawchar;
    f->pass2.finish = pass2_finish;
    f->pass2.internal = i;
}

