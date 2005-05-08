/* swffont.c

   Functions for loading external fonts.

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2003, 2004, 2005 Matthias Kramm
 
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

#include "gfxdevice.h"
#include "gfxtools.h"

static int loadfont_scale = 64;
static int skip_unused = 0;
static int full_unicode = 1;

#ifdef HAVE_FREETYPE

#ifdef HAVE_FT2BUILD_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_SIZES_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H
#include FT_OUTLINE_H
#else
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftsizes.h>
#include <freetype/ftsnames.h>
#include <freetype/ttnameid.h>
#include <freetype/ftoutln.h>
#endif

/* Setting subpixels to 64 also means that the "point size" of the
   font outlines will be 64. So the font, when rendered at original
   size (i.e., the swf fontsize is 1024) will have the same size as
   if it was rendered at 64pt */

#define FT_SCALE 1
#define FT_SUBPIXELS 64

static int ft_move_to(FT_Vector* _to, void* user) 
{
    gfxdrawer_t* draw = (gfxdrawer_t*)user;
    double x = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    double y = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    draw->moveTo(draw, x,y);
    return 0;
}
static int ft_line_to(FT_Vector* _to, void* user) 
{
    gfxdrawer_t* draw = (gfxdrawer_t*)user;
    double x = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    double y = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    draw->lineTo(draw, x,y);
    return 0;
}
static int ft_cubic_to(FT_Vector* _c1, FT_Vector* _c2, FT_Vector* _to, void* user)
{
    gfxdrawer_t* draw = (gfxdrawer_t*)user;
    double tox = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    double toy = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    double c1x = _c1->x*FT_SCALE/(float)FT_SUBPIXELS;
    double c1y = -_c1->y*FT_SCALE/(float)FT_SUBPIXELS;
    double c2x = _c2->x*FT_SCALE/(float)FT_SUBPIXELS;
    double c2y = -_c2->y*FT_SCALE/(float)FT_SUBPIXELS;
    gfxdraw_cubicTo(draw, c1x, c1y, c2x, c2y, tox, toy);
    return 0;
}
static int ft_conic_to(FT_Vector* _c, FT_Vector* _to, void* user) 
{
    gfxdrawer_t* draw = (gfxdrawer_t*)user;
    double tox = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    double toy = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    double cx = _c->x*FT_SCALE/(float)FT_SUBPIXELS;
    double cy = -_c->y*FT_SCALE/(float)FT_SUBPIXELS;
    gfxdraw_conicTo(draw, cx,cy, tox,toy);
    return 0;
}
static FT_Outline_Funcs outline_functions =
{
  ft_move_to,
  ft_line_to,
  ft_conic_to,
  ft_cubic_to,
  0,0
};

static FT_Library ftlibrary = 0;

static gfxglyph_t cloneGlyph(gfxglyph_t*src)
{
    gfxglyph_t dest;
    memset(&dest, 0, sizeof(dest));
    if(src->name)
	dest.name = strdup(src->name);
    dest.advance = src->advance;
    dest.unicode = src->unicode;
    dest.line = gfxline_clone(src->line);
    return dest;
}

static void glyph_clear(gfxglyph_t*g)
{
    gfxline_t*line;
    if(g->name) {
	free(g->name); g->name = 0;
    }
    gfxline_free(g->line);g->line = 0;
}

void gfxfont_free(gfxfont_t*font)
{
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	glyph_clear(&font->glyphs[t]);
    }
    if(font->glyphs) {
	free(font->glyphs);font->glyphs = 0;
    }
    font->num_glyphs = 0;
    if(font->unicode2glyph) {
	free(font->unicode2glyph);font->unicode2glyph = 0;
    }
    free(font);
}

gfxfont_t* gfxfont_load(char*filename)
{
    FT_Face face;
    FT_Error error;
    const char* name = 0;
    FT_ULong charcode;
    FT_UInt gindex;
    gfxfont_t* font;
    int t;
    int*glyph2glyph = 0;
    int*glyph2unicode = 0;
    FT_Size size;
    int max_unicode = 0;
    int charmap = -1;
    int isunicode = 1;
   
    if(ftlibrary == 0) {
	if(FT_Init_FreeType(&ftlibrary)) {
	    fprintf(stderr, "Couldn't init freetype library!\n");
	    exit(1);
	}
    }
    error = FT_New_Face(ftlibrary, filename, 0, &face);
    FT_Set_Pixel_Sizes (face, 16*loadfont_scale, 16*loadfont_scale);

    if(error) {
	fprintf(stderr, "Couldn't load file %s- not a TTF file?\n", filename);
	return 0;
    }
    if(face->num_glyphs <= 0) {
	fprintf(stderr, "File %s contains %d glyphs\n", face->num_glyphs);
	return 0;
    }

    font = rfx_calloc(sizeof(gfxfont_t));
    //font->style =  ((face->style_flags&FT_STYLE_FLAG_ITALIC)?FONT_STYLE_ITALIC:0) |((face->style_flags&FT_STYLE_FLAG_BOLD)?FONT_STYLE_BOLD:0);
    //font->ascent = abs(face->ascender)*FT_SCALE*loadfont_scale*20/FT_SUBPIXELS/2; //face->bbox.xMin;
    //font->descent = abs(face->descender)*FT_SCALE*loadfont_scale*20/FT_SUBPIXELS/2; //face->bbox.xMax;
    //font->leading = font->layout->ascent + font->layout->descent;
    //font->encoding = FONT_ENCODING_UNICODE;
    font->max_unicode = 0;
    
    font->glyphs = rfx_calloc(face->num_glyphs*sizeof(gfxglyph_t));
    glyph2unicode = rfx_calloc(face->num_glyphs*sizeof(int));
    glyph2glyph = rfx_calloc(face->num_glyphs*sizeof(int));

    if(FT_HAS_GLYPH_NAMES(face)) {
	//font->glyphnames = rfx_calloc(face->num_glyphs*sizeof(char*));
    }

    /*name = FT_Get_Postscript_Name(face);
    if(name && *name)
	font->name = strdup(name);*/

    while(1) 
    {
	charcode = FT_Get_First_Char(face, &gindex);
	while(gindex != 0)
	{
	    if(gindex >= 0 && gindex<face->num_glyphs) {
		if(!glyph2unicode[gindex]) {
		    glyph2unicode[gindex] = charcode;
		    if(charcode + 1 > font->max_unicode) {
			font->max_unicode = charcode + 1;
		    }
		}
	    }
	    charcode = FT_Get_Next_Char(face, charcode, &gindex);
	}

	/* if we didn't find a single encoding character, try
	   the font's charmaps instead. That usually means that
	   the encoding is no longer unicode. 
	   TODO: find a way to convert the encoding to unicode
	 */
	if(font->max_unicode == 0 && charmap < face->num_charmaps - 1) {
	    charmap++;
	    FT_Set_Charmap(face, face->charmaps[charmap]);
	    //font->encoding = 0;//anything but unicode FIXME
	    isunicode = 0;
	} else 
	    break;
    }

    /* TODO: if isunicode is 1, we now need to permutate the character
             order so that each character is at it's encoding position */

    if(full_unicode)
	font->max_unicode = 65535;
    
    font->unicode2glyph = rfx_calloc(font->max_unicode*sizeof(int));
    
    for(t=0;t<font->max_unicode;t++) {
	int g = FT_Get_Char_Index(face, t);
	if(!g || g>=face->num_glyphs)
	    g = -1;
	font->unicode2glyph[t] = g;
	if(g>=0) {
	    max_unicode = t+1;
	    if(!glyph2unicode[g]) {
		glyph2unicode[g] = t;
	    }
	}
    }
    font->max_unicode = max_unicode;

    font->num_glyphs = 0;

    for(t=0; t < face->num_glyphs; t++) {
	FT_Glyph glyph;
	FT_BBox bbox;
	FT_Matrix matrix;
	char name[128];
	gfxdrawer_t draw;
	int ret;
	char hasname = 0;
	name[0]=0;
	if(FT_HAS_GLYPH_NAMES(face)) {
	    error = FT_Get_Glyph_Name(face, t, name, 127);
	    if(!error && name[0] && !strstr(name, "notdef")) {
		font->glyphs[font->num_glyphs].name = strdup(name);
		hasname = 1;
	    }
	}
	if(!glyph2unicode[t] && !hasname && skip_unused) {
	    continue;
	}
	error = FT_Load_Glyph(face, t, FT_LOAD_NO_BITMAP);
	if(error) {
	    fprintf(stderr, "Couldn't load glyph %d, error:%d\n", t, error);
	    continue;
	}
	error = FT_Get_Glyph(face->glyph, &glyph);
	if(error) {
	    fprintf(stderr, "Couldn't get glyph %d, error:%d\n", t, error);
	    continue;
	}

	FT_Glyph_Get_CBox(glyph, ft_glyph_bbox_unscaled, &bbox);
	bbox.yMin = -bbox.yMin;
	bbox.yMax = -bbox.yMax;
	if(bbox.xMax < bbox.xMin) {
	    // swap
	    bbox.xMax ^= bbox.xMin;
	    bbox.xMin ^= bbox.xMax;
	    bbox.xMax ^= bbox.xMin;
	}
	if(bbox.yMax < bbox.yMin) {
	    // swap
	    bbox.yMax ^= bbox.yMin;
	    bbox.yMin ^= bbox.yMax;
	    bbox.yMax ^= bbox.yMin;
	}

	gfxdrawer_target_gfxline(&draw);

	//error = FT_Outline_Decompose(&face->glyph->outline, &outline_functions, &draw);
	error = FT_Outline_Decompose(&face->glyph->outline, &outline_functions, &draw);
	
	if(error) {
	    fprintf(stderr, "Couldn't decompose glyph %d\n", t);
	    gfxline_free((gfxline_t*)draw.result(&draw));
	    continue;
	}

#if 0
	if(bbox.xMin > 0) {
	    font->glyph[font->num_glyphs].advance = (bbox.xMax*20*FT_SCALE)/FT_SUBPIXELS;
	} else {
	    font->glyph[font->num_glyphs].advance = ((bbox.xMax - bbox.xMin)*20*FT_SCALE)/FT_SUBPIXELS;
	}
#else
	font->glyphs[font->num_glyphs].advance = glyph->advance.x*20/65536;
#endif
	
	font->glyphs[font->num_glyphs].line = (gfxline_t*)draw.result(&draw);
	
	/*font->glyphs[font->num_glyphs].bbox.xmin = (bbox.xMin*FT_SCALE*20)/FT_SUBPIXELS;
	font->glyphs[font->num_glyphs].bbox.ymin = (bbox.yMin*FT_SCALE*20)/FT_SUBPIXELS;
	font->glyphs[font->num_glyphs].bbox.xmax = (bbox.xMax*FT_SCALE*20)/FT_SUBPIXELS;
	font->glyphs[font->num_glyphs].bbox.ymax = (bbox.yMax*FT_SCALE*20)/FT_SUBPIXELS;*/

	FT_Done_Glyph(glyph);
	font->glyphs[font->num_glyphs].unicode = glyph2unicode[t];
	glyph2glyph[t] = font->num_glyphs;
	font->num_glyphs++;
    }
    /* notice: if skip_unused is true, font->glyph2unicode, font->glyphnames and font->layout->bounds will 
	       have more memory allocated than just font->num_glyphs, but only the first font->numchars 
	       are used/valid */

    for(t=0;t<font->max_unicode;t++) {
	if(font->unicode2glyph[t]>=0) {
	    font->unicode2glyph[t] = glyph2glyph[font->unicode2glyph[t]];
	}
    }
    rfx_free(glyph2glyph);
    rfx_free(glyph2unicode);

    FT_Done_Face(face);
    FT_Done_FreeType(ftlibrary);ftlibrary=0;
  
    if(!isunicode && font->num_glyphs>0) {
	/* if the encoding isn't unicode, remap the font
	   so that the encoding equals the char position, and
	   remove the unicode table */
	int t;
	gfxglyph_t*newglyphs = rfx_calloc(font->max_unicode*sizeof(gfxglyph_t));

	for(t=0;t<max_unicode;t++) {
	    int c = font->unicode2glyph[t];
	    if(c>=font->num_glyphs || c<0)
		c = 0;
	    newglyphs[t] = cloneGlyph(&font->glyphs[c]);
	    newglyphs[t].unicode = -1;
	}
	for(t=0;t<font->num_glyphs;t++) {
	    glyph_clear(&font->glyphs[t]);
	}
	free(font->glyphs);
	font->glyphs = newglyphs;
	font->num_glyphs = font->max_unicode;

	free(font->unicode2glyph);font->unicode2glyph = 0;
	font->max_unicode = 0;
    }

    if(font->unicode2glyph) {
        int t;
        int bad = 0;
        /* check whether the Unicode indices look o.k.
           If they don't, disable the unicode lookup by setting
           the unicode map to -1 everywhere */
        for(t=0;t<font->num_glyphs;t++) {
            int c = font->glyphs[t].unicode;
	    gfxline_t* line = font->glyphs[t].line;
            if(c && c < 32 && (line && line->next && line->next->next)) {
                // the character maps into the unicode control character range
                // between 0001-001f. Yet it is not empty. Treat the one
                // mapping as broken, and look how many of those we find.
                bad ++;
            }
        }
        if(bad>5) {
	    free(font->unicode2glyph);font->unicode2glyph = 0;
	    font->max_unicode = 0;
	    for(t=0;t<font->num_glyphs;t++) {
		font->glyphs[t].unicode = -1;
	    }
        }
    }

    return font;
}
#else

gfxfont_t* gfxfont_load(char*filename)
{
    fprintf(stderr, "No freetype support compiled in! Not able to load %s\n", filename);
}

#endif

