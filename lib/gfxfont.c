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

#include "../config.h"
#include "gfxdevice.h"
#include "gfxtools.h"
#include "gfxfont.h"

static int loadfont_scale = 64;
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

typedef struct _gfxdrawinfo_t {
    gfxdrawer_t* draw;
    double quality;
} gfxdrawinfo_t;

static int ft_move_to(const FT_Vector* _to, void* user) 
{
    gfxdrawinfo_t* info = (gfxdrawinfo_t*)user;
    gfxdrawer_t* draw = info->draw;
    double x = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    double y = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    draw->moveTo(draw, x,y);
    return 0;
}
static int ft_line_to(const FT_Vector* _to, void* user) 
{
    gfxdrawinfo_t* info = (gfxdrawinfo_t*)user;
    gfxdrawer_t* draw = info->draw;
    double x = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    double y = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    draw->lineTo(draw, x,y);
    return 0;
}
static int ft_cubic_to(const FT_Vector* _c1, const FT_Vector* _c2, const FT_Vector* _to, void* user)
{
    gfxdrawinfo_t* info = (gfxdrawinfo_t*)user;
    gfxdrawer_t* draw = info->draw;
    double tox = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    double toy = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    double c1x = _c1->x*FT_SCALE/(float)FT_SUBPIXELS;
    double c1y = -_c1->y*FT_SCALE/(float)FT_SUBPIXELS;
    double c2x = _c2->x*FT_SCALE/(float)FT_SUBPIXELS;
    double c2y = -_c2->y*FT_SCALE/(float)FT_SUBPIXELS;
    gfxdraw_cubicTo(draw, c1x, c1y, c2x, c2y, tox, toy, info->quality);
    return 0;
}
static int ft_conic_to(const FT_Vector* _c, const FT_Vector* _to, void* user) 
{
    gfxdrawinfo_t* info = (gfxdrawinfo_t*)user;
    gfxdrawer_t* draw = info->draw;
    double tox = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    double toy = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    double cx = _c->x*FT_SCALE/(float)FT_SUBPIXELS;
    double cy = -_c->y*FT_SCALE/(float)FT_SUBPIXELS;
    gfxdraw_conicTo(draw, cx,cy, tox,toy, info->quality);
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
	free((void*)g->name); g->name = 0;
    }
    gfxline_free(g->line);g->line = 0;
}

static int errorno = 0;

//#define DEBUG 1

gfxfont_t* gfxfont_load(char*id, char*filename, unsigned int flags, double quality)
{
    FT_Face face;
    FT_Error error;
    const char* fontname = 0;
    FT_ULong charcode;
    FT_UInt gindex;
    gfxfont_t* font;
    int t;
    int*glyph2glyph = 0;
    int*glyph2unicode = 0;
    int max_unicode = 0;
    int charmap = -1;
    int isunicode = 1;
    int has_had_errors = 0;
    int num_names = 0;

    if(ftlibrary == 0) {
	if(FT_Init_FreeType(&ftlibrary)) {
	    fprintf(stderr, "Couldn't init freetype library!\n");
	    exit(1);
	}
    }
    error = FT_New_Face(ftlibrary, filename, 0, &face);
    FT_Set_Pixel_Sizes (face, 16*loadfont_scale, 16*loadfont_scale);
#ifdef DEBUG
    printf("gfxfont_load(%s, %s, %f)\n", id, filename, quality);
#endif

    if(error) {
	fprintf(stderr, "Couldn't load file %s- not a TTF file? (error=%02x)\n", filename, error);
	return 0;
    }
    if(face->num_glyphs <= 0) {
	fprintf(stderr, "File %s contains %d glyphs\n", face->num_glyphs);
	return 0;
    }

    font = (gfxfont_t*)rfx_calloc(sizeof(gfxfont_t));
    //font->style =  ((face->style_flags&FT_STYLE_FLAG_ITALIC)?FONT_STYLE_ITALIC:0) |((face->style_flags&FT_STYLE_FLAG_BOLD)?FONT_STYLE_BOLD:0);
    //font->ascent = abs(face->ascender)*FT_SCALE*loadfont_scale*20/FT_SUBPIXELS/2; //face->bbox.xMin;
    //font->descent = abs(face->descender)*FT_SCALE*loadfont_scale*20/FT_SUBPIXELS/2; //face->bbox.xMax;
    //font->leading = font->layout->ascent + font->layout->descent;
    //font->encoding = FONT_ENCODING_UNICODE;
    font->max_unicode = 0;
    font->id = strdup(id);
    
    font->glyphs = (gfxglyph_t*)rfx_calloc(face->num_glyphs*sizeof(gfxglyph_t));
    glyph2unicode = (int*)rfx_calloc(face->num_glyphs*sizeof(int));
    glyph2glyph = (int*)rfx_calloc(face->num_glyphs*sizeof(int));

    if(FT_HAS_GLYPH_NAMES(face)) {
	//font->glyphnames = rfx_calloc(face->num_glyphs*sizeof(char*));
    }

    fontname = FT_Get_Postscript_Name(face);

#ifdef DEBUG
    for(t=0;t<face->num_charmaps;t++) {
        printf("possible encoding: %c%c%c%c (%d of %d)\n", 
                (face->charmaps[t]->encoding >> 24)&255,
                (face->charmaps[t]->encoding >> 16)&255,
                (face->charmaps[t]->encoding >> 8)&255,
                (face->charmaps[t]->encoding >> 0)&255,
                t+1, face->num_charmaps
                );
    }
#endif

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

#ifdef DEBUG
        if(face->charmap) {
            printf("ENCODING: %c%c%c%c (%d of %d) max_unicode=%d\n", 
                    (face->charmap->encoding >> 24)&255,
                    (face->charmap->encoding >> 16)&255,
                    (face->charmap->encoding >> 8)&255,
                    (face->charmap->encoding >> 0)&255,
                    charmap, face->num_charmaps, font->max_unicode
                    );
        } else {
            printf("ENCODING: NONE (%d of %d) max_unicode=%d\n",
                    charmap, face->num_charmaps, font->max_unicode
                    );
        }
#endif

	/* if we didn't find a single encoding character, try
	   the font's charmaps instead. That usually means that
	   the encoding is no longer unicode. 
	   TODO: find a way to convert the encoding to unicode
	 */
	if(font->max_unicode == 0 && charmap < face->num_charmaps-1
		&& face->charmaps[charmap+1]->encoding != 0x41444243 /* adbc, custom */
		&& face->charmaps[charmap+1]->encoding != 0x61726d6e    /* armn */
                )
		{
	    charmap++;
	    FT_Set_Charmap(face, face->charmaps[charmap]);
	    isunicode = 0;
	} else
	    break;
    }
    /* TODO: if isunicode is 1, we now need to permutate the character
             order so that each character is at it's encoding position */

    if(full_unicode)
	font->max_unicode = 65535;
    
    font->unicode2glyph = (int*)rfx_calloc(font->max_unicode*sizeof(int));
    
    for(t=0;t<font->max_unicode;t++) {
	int g = FT_Get_Char_Index(face, t);
	if(!g || g>=face->num_glyphs)
	    g = -1;
	font->unicode2glyph[t] = g;
	if(g>=0) {
#ifdef DEBUG
	    printf("u%d ->%d\n", t, g);
#endif
	    max_unicode = t+1;
	    if(!glyph2unicode[g]) {
		glyph2unicode[g] = t;
	    }
	}
    }
    font->max_unicode = max_unicode;

    font->num_glyphs = 0;


    for(t=0; t < face->num_glyphs; t++) {
	if(FT_HAS_GLYPH_NAMES(face)) {
	    char name[128];
	    error = FT_Get_Glyph_Name(face, t, name, 127);
	    if(!error && name[0] && !strstr(name, "notdef")) {
		num_names++;
	    }
	}
    }


    for(t=0; t < face->num_glyphs; t++) {
	FT_Glyph glyph;
	char name[128];
	gfxdrawer_t draw;
	gfxdrawinfo_t info;
	char hasname = 0;
	int omit = 0;
	name[0]=0;
	
	font->glyphs[font->num_glyphs].advance = 0;
	font->glyphs[font->num_glyphs].line = 0;
	font->glyphs[font->num_glyphs].unicode = glyph2unicode[t];
	font->glyphs[font->num_glyphs].name = 0;

	if(FT_HAS_GLYPH_NAMES(face) && (num_names >= face->num_glyphs/10 || num_names > 2)) {
	    name[0] = 0;
	    error = FT_Get_Glyph_Name(face, t, name, 127);
	    if(!error && name[0] && !strstr(name, "notdef")) {
		font->glyphs[font->num_glyphs].name = strdup(name);
		hasname = 1;
	    }
	}

#if 0 // some cantonese pdfs fail to work if this is activated

	if(has_had_errors && (isunicode && !glyph2unicode[t]) && !hasname && t>=256) {
	    /* some freetype versions crash or corrupt memory if we try to load
	       characters (without unicode index or name) above 256 for some fonts.
	       So skip those characters once the first error occured */
	    omit = 1;
	}
#endif

	if(!omit) {
	    error = FT_Load_Glyph(face, t, FT_LOAD_NO_BITMAP);
	    if(error) {
		if(hasname)
		    fprintf(stderr, "Warning: glyph %d/%d (unicode %d, name %s) has return code %d\n", t, face->num_glyphs, glyph2unicode[t], name, error);
		else
		    fprintf(stderr, "Warning: glyph %d/%d (unicode %d) has return code %d\n", t, face->num_glyphs, glyph2unicode[t], error);
		omit = 2;

#if 0
		if(!has_had_errors) {
		    char buf[256];
		    if(fontname && *fontname) {
			fprintf(stderr, "font has been copied to %s.ttf\n", fontname);
			sprintf(buf, "cp %s %s.ttf", filename, fontname);
		    } else {
			fprintf(stderr, "font has been copied to badfont%d.ttf\n", errorno);
			sprintf(buf, "cp %s badfont%d.ttf", filename, errorno);
			errorno++;
		    }
		    system(buf);
		}
#endif
		has_had_errors = 1;
	    }
	}
	if(!omit) {
	    error = FT_Get_Glyph(face->glyph, &glyph);
	    if(error) {
		fprintf(stderr, "Couldn't get glyph %d/%d, error:%d\n", t, face->num_glyphs, error);
		omit = 3;
	    }
	}

	if(!omit) {
	    gfxline_t*l;
	    int ok=0;
	    gfxdrawer_target_gfxline(&draw);
	    info.draw = &draw;
	    info.quality = quality;

	    //error = FT_Outline_Decompose(&face->glyph->outline, &outline_functions, &info);
	    error = FT_Outline_Decompose(&face->glyph->outline, &outline_functions, &info);
	    
	    if(error) {
		fprintf(stderr, "Couldn't decompose glyph %d\n", t);
		gfxline_free((gfxline_t*)draw.result(&draw));
		FT_Done_Glyph(glyph);
		omit = 4;
	    } else {
		font->glyphs[font->num_glyphs].advance = glyph->advance.x/65536;
		font->glyphs[font->num_glyphs].line = (gfxline_t*)draw.result(&draw);
	    }
	    l = font->glyphs[font->num_glyphs].line;
	    while(l) {
		if(l->type != gfx_moveTo) {
		    ok = 1;
		}
		l = l->next;
	    }
	    if(!ok && !name) {
		gfxline_free(font->glyphs[font->num_glyphs].line);
		font->glyphs[font->num_glyphs].line = 0;
		font->glyphs[font->num_glyphs].advance = 0;

		/* Some PDFs created e.g. by InDesign tend to create
		   fonts with reduced (empty) characters, which still
		   have unicode indices attached to them.
		   Remove that information, in order to not confuse
		   any converter applications.
		    */
		font->glyphs[font->num_glyphs].unicode = 0;
		if(font->glyphs[font->num_glyphs].name) {
		    free((void*)font->glyphs[font->num_glyphs].name);
		    font->glyphs[font->num_glyphs].name = 0;
		}
		FT_Done_Glyph(glyph);
		omit = 5;
	    }
	}

	if(!omit) {
	    FT_Done_Glyph(glyph);
	    font->glyphs[font->num_glyphs].unicode = glyph2unicode[t];
	}

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
 
    if(!isunicode && font->num_glyphs>0 && font->max_unicode) {
	/* if the encoding isn't unicode, remap the font
	   so that the encoding equals the char position, and
	   remove the unicode table */
	int t;
	gfxglyph_t*newglyphs = (gfxglyph_t*)rfx_calloc(font->max_unicode*sizeof(gfxglyph_t));

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

gfxfont_t* gfxfont_load(char*id, char*filename, unsigned int flags, double quality)
{
    fprintf(stderr, "No freetype support compiled in! Not able to load %s\n", filename);
}

#endif

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
    if(font->id) {
	free((void*)font->id);font->id=0;
    }

    free(font);
}

