/* vectors_to_fonts.c

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
#include <math.h>
#include "../gfxfilter.h"
#include "../gfxtools.h"
#include "../gfxpoly.h"
#include "../gfxfont.h"
#include "../types.h"
#include "../mem.h"
#include "../q.h"

typedef struct _vglyph {
    gfxglyph_t glyph;
    struct _vglyph*next;
} vglyph_t;

typedef struct _internal {
    vglyph_t*first_glyph;
    vglyph_t*last_glyph;
    gfxfont_t*font;
    char first;
    int glyph_nr;
} internal_t;

#define FONT_SIZE 1024.0

static void pass1_fill(gfxfilter_t*f, gfxline_t*line, gfxcolor_t*color, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    gfxbbox_t bbox = gfxline_getbbox(line);
    double height = bbox.ymax - bbox.ymin;
    double width = bbox.xmax - bbox.xmin;
    double size = fmax(width,height);
    if(fabs(size) < 1e-3) return;
    double scale = FONT_SIZE/size;
    gfxmatrix_t m = {scale, 0,     -bbox.xmin*scale,
                     0,     scale, -bbox.ymax*scale};
    line = gfxline_clone(line);
    gfxline_transform(line, &m);
    line = gfxline_restitch(line);
    vglyph_t*v = rfx_calloc(sizeof(vglyph_t));
    v->glyph.line = line;
    v->glyph.advance = width*scale;
    v->glyph.unicode = 32+i->glyph_nr;

    i->last_glyph->next = v;
    i->last_glyph = v;
    
    gfxmatrix_t m2 = {1.0/scale, 0,         bbox.xmin,
                      0,         1.0/scale, bbox.ymax};
    out->drawchar(out, 0, i->glyph_nr, color, &m2);

    i->glyph_nr++;
}
static void pass1_stroke(gfxfilter_t*f, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit, gfxdevice_t*out)
{
    return;
    internal_t*i = (internal_t*)f->internal;
    gfxpoly_t*poly = gfxpoly_from_stroke(line, width, cap_style, joint_style, miterLimit, DEFAULT_GRID);
    gfxline_t*line2 = gfxline_from_gfxpoly_with_direction(poly);
    pass1_fill(f, line2, color, out);
    gfxline_free(line2);
    gfxpoly_destroy(poly);
}

static void make_font(internal_t*i)
{
    i->font = rfx_calloc(sizeof(gfxfont_t));
    i->font->id = "SWFTools-Vector-Magic";
    i->font->num_glyphs = i->glyph_nr;
    i->font->glyphs = malloc(sizeof(gfxglyph_t)*i->font->num_glyphs);
    i->font->ascent = FONT_SIZE;
    i->font->descent = 0;
    int pos = 0;
    vglyph_t*g = i->first_glyph;
    while(g) {
	i->font->glyphs[pos] = g->glyph;
	pos++;
	g = g->next;
    }
    gfxfont_fix_unicode(i->font, 1);
}

static void pass2_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    if(i->first) {
	make_font(i);
	out->addfont(out, i->font);
	i->first = 0;
    }
    if(!font) {
	font = i->font;
	
	gfxcolor_t red = {255,254,1,1};
	int x;
	gfxmatrix_t m = {1.0,  0,0,
			 0,  1.0,0};
	out->drawchar(out, font, 1, &red, &m);

	/*gfxmatrix_t m2 = {1.0,  0,600,
			 0,  1.0,600};
	out->drawchar(out, font, 1, &red, &m2);*/
	gfxline_t*line = gfxline_makerectangle(0, 0, 1, 1);
	gfximage_t img;
	img.data = color;
	img.width = 1;
	img.height = 1;
	out->fillbitmap(out, line, &img, &m, 0);
    }
    out->drawchar(out, font, glyphnr, color, matrix);
}

void gfxtwopassfilter_vectors_to_glyphs_init(gfxtwopassfilter_t*f)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));

    memset(f, 0, sizeof(gfxtwopassfilter_t));
    f->type = gfxfilter_twopass;

    f->pass1.name = "vectors to glyphs pass 1";
    f->pass1.fill = pass1_fill;
    f->pass1.stroke = pass1_stroke;
    f->pass1.internal = i;

    f->pass2.name = "vectors to glyphs pass 2";
    f->pass2.drawchar = pass2_drawchar;
    f->pass2.internal = i;

    i->first = 1;
    
    i->first_glyph = rfx_calloc(sizeof(vglyph_t));
    i->first_glyph->glyph.line = 0;
    i->first_glyph->glyph.advance = FONT_SIZE;
    i->first_glyph->glyph.unicode = 32;
    i->last_glyph = rfx_calloc(sizeof(vglyph_t));
    // dummy char
    i->first_glyph->next = i->last_glyph;
    i->last_glyph->glyph.line = 0;
    i->last_glyph->glyph.advance = FONT_SIZE/8;
    i->last_glyph->glyph.unicode = 33;
    i->glyph_nr = 2;
}


