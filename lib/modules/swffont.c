/* swffont.c

   Functions for loading external fonts.

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2003, 2004 Matthias Kramm
 
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

#include <stdio.h>
#include <stdlib.h>
#include "../rfxswf.h"

static int loadfont_scale = 4;
static int skip_unused = 1;
static int full_unicode = 0;

void swf_SetLoadFontParameters(int _scale, int _skip_unused, int _full_unicode)
{
    if(_scale) loadfont_scale = _scale;
    skip_unused = _skip_unused;
    full_unicode = _full_unicode;
}

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

static int ft_move_to(const FT_Vector* _to, void* user) 
{
    drawer_t* draw = (drawer_t*)user;
    FPOINT to;
    to.x = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    to.y = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    draw->moveTo(draw, &to);
    return 0;
}
static int ft_line_to(const FT_Vector* _to, void* user) 
{
    drawer_t* draw = (drawer_t*)user;
    FPOINT to;
    to.x = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    to.y = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    draw->lineTo(draw, &to);
    return 0;
}
static int ft_conic_to(const FT_Vector* _c, const FT_Vector* _to, void* user) 
{
    drawer_t* draw = (drawer_t*)user;
    FPOINT c,to;
    to.x = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    to.y = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    c.x = _c->x*FT_SCALE/(float)FT_SUBPIXELS;
    c.y = -_c->y*FT_SCALE/(float)FT_SUBPIXELS;
    draw_conicTo(draw, &c, &to);
    return 0;
}
static int ft_cubic_to(const FT_Vector* _c1, const FT_Vector* _c2, const FT_Vector* _to, void* user)
{
    drawer_t* draw = (drawer_t*)user;
    FPOINT c1,c2,to;
    to.x = _to->x*FT_SCALE/(float)FT_SUBPIXELS;
    to.y = -_to->y*FT_SCALE/(float)FT_SUBPIXELS;
    c1.x = _c1->x*FT_SCALE/(float)FT_SUBPIXELS;
    c1.y = -_c1->y*FT_SCALE/(float)FT_SUBPIXELS;
    c2.x = _c2->x*FT_SCALE/(float)FT_SUBPIXELS;
    c2.y = -_c2->y*FT_SCALE/(float)FT_SUBPIXELS;
    draw_cubicTo(draw, &c1, &c2, &to);
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

SWFFONT* swf_LoadTrueTypeFont(const char*filename, char flashtype)
{
    FT_Face face;
    FT_Error error;
    const char* name = 0;
    FT_ULong charcode;
    FT_UInt gindex;
    SWFFONT* font;
    int t;
    int*glyph2glyph;
    int max_unicode = 0;
    int charmap = -1;

    if(ftlibrary == 0) {
	if(FT_Init_FreeType(&ftlibrary)) {
	    fprintf(stderr, "Couldn't init freetype library!\n");
	    exit(1);
	}
    }
    error = FT_New_Face(ftlibrary, filename, 0, &face);

    if(error || !face) {
	fprintf(stderr, "Couldn't load file %s- not a TTF file?\n", filename);
	return 0;
    }
   
    int scale = flashtype?20:1;
    FT_Set_Pixel_Sizes (face, 16*loadfont_scale*scale, 16*loadfont_scale*scale);

    if(face->num_glyphs <= 0) {
	fprintf(stderr, "File %s contains %d glyphs\n", filename, (int)face->num_glyphs);
	return 0;
    }

    font = (SWFFONT*)rfx_calloc(sizeof(SWFFONT));
    font->id = -1;
    font->version = flashtype?3:2;

    font->layout = (SWFLAYOUT*)rfx_calloc(sizeof(SWFLAYOUT));
    font->layout->bounds = (SRECT*)rfx_calloc(face->num_glyphs*sizeof(SRECT));
    font->style =  ((face->style_flags&FT_STYLE_FLAG_ITALIC)?FONT_STYLE_ITALIC:0)
	          |((face->style_flags&FT_STYLE_FLAG_BOLD)?FONT_STYLE_BOLD:0);
    font->encoding = FONT_ENCODING_UNICODE;
    font->glyph2ascii = (U16*)rfx_calloc(face->num_glyphs*sizeof(U16));
    font->maxascii = 0;
    font->glyph = (SWFGLYPH*)rfx_calloc(face->num_glyphs*sizeof(SWFGLYPH));
    if(FT_HAS_GLYPH_NAMES(face)) {
	font->glyphnames = (char**)rfx_calloc(face->num_glyphs*sizeof(char*));
    }
    
    font->layout->kerningcount = 0;
    
    name = face->family_name;
    if(!(name && *name))
        name = FT_Get_Postscript_Name(face);
    if(name && *name)
	font->name = (U8*)strdup(name);

    while(1) 
    {
    /*    // Map Glyphs to Unicode, version 1 (quick and dirty):
	int t;
	for(t=0;t<65536;t++) {
	    int index = FT_Get_Char_Index(face, t);
	    if(index>=0 && index<face->num_glyphs) {
		if(font->glyph2ascii[index]<0)
		    font->glyph2ascii[index] = t;
	    }
	}*/
	
	// Map Glyphs to Unicode, version 2 (much nicer):
	// (The third way would be the AGL algorithm, as proposed
	//  by Werner Lemberg on freetype@freetype.org)

	charcode = FT_Get_First_Char(face, &gindex);
	while(gindex != 0)
	{
	    if(gindex >= 0 && gindex<face->num_glyphs) {
		if(!font->glyph2ascii[gindex]) {
		    font->glyph2ascii[gindex] = charcode;
		    if(charcode + 1 > font->maxascii) {
			font->maxascii = charcode + 1;
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
	if(font->maxascii == 0 && charmap < face->num_charmaps - 1) {
	    charmap++;
	    FT_Set_Charmap(face, face->charmaps[charmap]);
	    font->encoding = 0;//anything but unicode FIXME
	} else 
	    break;
    }

    if(full_unicode)
	font->maxascii = 65535;
    
    font->ascii2glyph = (int*)rfx_calloc(font->maxascii*sizeof(int));
    
    for(t=0;t<font->maxascii;t++) {
	int g = FT_Get_Char_Index(face, t);
	if(!g || g>=face->num_glyphs)
	    g = -1;
	font->ascii2glyph[t] = g;
	if(g>=0) {
	    max_unicode = t+1;
	    if(!font->glyph2ascii[g]) {
		font->glyph2ascii[g] = t;
	    }
	}
    }
    font->maxascii = max_unicode;

    font->numchars = 0;

    glyph2glyph = (int*)rfx_calloc(face->num_glyphs*sizeof(int));

    SRECT fontbbox = {0,0,0,0};

    for(t=0; t < face->num_glyphs; t++) {
	FT_Glyph glyph;
	FT_BBox bbox;
	char name[128];
	drawer_t draw;
	char hasname = 0;
	name[0]=0;
	if(FT_HAS_GLYPH_NAMES(face)) {
	    error = FT_Get_Glyph_Name(face, t, name, 127);
	    if(!error && name[0] && !strstr(name, "notdef")) {
		font->glyphnames[font->numchars] = strdup(name);
		hasname = 1;
	    }
	}
	if(!font->glyph2ascii[t] && !hasname && skip_unused) {
	    continue;
	}
	error = FT_Load_Glyph(face, t, FT_LOAD_NO_BITMAP);
	if(error) {
	    //tends to happen with some pdfs
	    fprintf(stderr, "Warning: Glyph %d has return code %d\n", t, error);
	    glyph=0;
	    if(skip_unused) 
		continue;
	} else {
	    error = FT_Get_Glyph(face->glyph, &glyph);
	    if(error) {
		fprintf(stderr, "Couldn't get glyph %d, error:%d\n", t, error);
		glyph=0;
		if(skip_unused) 
		    continue;
	    }
	}

	if(glyph)
	    FT_Glyph_Get_CBox(glyph, ft_glyph_bbox_unscaled, &bbox);
	else
	    memset(&bbox, 0, sizeof(bbox));

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

	swf_Shape01DrawerInit(&draw, 0);

	//error = FT_Outline_Decompose(&face->glyph->outline, &outline_functions, &draw);
	if(glyph)
	    error = FT_Outline_Decompose(&face->glyph->outline, &outline_functions, &draw);
	else
	    error = 0;
	draw.finish(&draw);
	
	if(error) {
	    fprintf(stderr, "Couldn't decompose glyph %d\n", t);
	    draw.dealloc(&draw);
	    continue;
	}

#if 0
	if(bbox.xMin > 0) {
	    font->glyph[font->numchars].advance = (bbox.xMax*20*FT_SCALE)/FT_SUBPIXELS;
	} else {
	    font->glyph[font->numchars].advance = ((bbox.xMax - bbox.xMin)*20*FT_SCALE)/FT_SUBPIXELS;
	}
#else
	if(glyph)
	    font->glyph[font->numchars].advance = glyph->advance.x*20/65536;
	else
	    font->glyph[font->numchars].advance = 0;
#endif
	
	SRECT b = swf_ShapeDrawerGetBBox(&draw);

	//font->layout->bounds[font->numchars].xmin = (bbox.xMin*FT_SCALE*20)/FT_SUBPIXELS;
	//font->layout->bounds[font->numchars].ymin = (bbox.yMin*FT_SCALE*20)/FT_SUBPIXELS;
	//font->layout->bounds[font->numchars].xmax = (bbox.xMax*FT_SCALE*20)/FT_SUBPIXELS;
	//font->layout->bounds[font->numchars].ymax = (bbox.yMax*FT_SCALE*20)/FT_SUBPIXELS;

	font->layout->bounds[font->numchars] = b;
	font->glyph[font->numchars].shape = swf_ShapeDrawerToShape(&draw);

	swf_ExpandRect2(&fontbbox, &font->layout->bounds[font->numchars]);

	draw.dealloc(&draw);

	if(glyph)
	    FT_Done_Glyph(glyph);
	font->glyph2ascii[font->numchars] = font->glyph2ascii[t];
	glyph2glyph[t] = font->numchars;
	font->numchars++;
    }
    
    //font->layout->ascent = abs(face->ascender)*FT_SCALE*loadfont_scale*20/FT_SUBPIXELS/2; //face->bbox.xMin;
    //font->layout->descent = abs(face->descender)*FT_SCALE*loadfont_scale*20/FT_SUBPIXELS/2; //face->bbox.xMax;
    //font->layout->leading = font->layout->ascent + font->layout->descent;

    if(-fontbbox.ymin < 0)
        font->layout->ascent = 0;
    else
	font->layout->ascent = -fontbbox.ymin;

    if(fontbbox.ymax < 0)
        font->layout->descent = 0;
    else
	font->layout->descent = fontbbox.ymax;

    int leading = fontbbox.ymax - fontbbox.ymin;
    font->layout->leading = leading>0x7fff?0x7fff:leading;

    /* notice: if skip_unused is true, font->glyph2ascii, font->glyphnames and font->layout->bounds will 
	       have more memory allocated than just font->numchars, but only the first font->numchars 
	       are used/valid */

    for(t=0;t<font->maxascii;t++) {
	if(font->ascii2glyph[t]>=0) {
	    font->ascii2glyph[t] = glyph2glyph[font->ascii2glyph[t]];
	}
    }
    rfx_free(glyph2glyph);

    FT_Done_Face(face);
    FT_Done_FreeType(ftlibrary);ftlibrary=0;

    return font;
}
#else  //HAVE_FREETYPE

SWFFONT* swf_LoadTrueTypeFont(const char*filename, char flashtype)
{
    fprintf(stderr, "Warning: no freetype library- not able to load %s\n", filename);
    return 0;
}

#endif

#ifdef HAVE_T1LIB

#include <t1lib.h>

static int t1lib_initialized = 0;

static int counter = 0;

SWFFONT* swf_LoadT1Font(const char*filename)
{
    SWFFONT * font;
    int nr;
    float angle,underline;
    char*fontname,*fullname,*familyname;
    BBox bbox;
    int s,num;
    char**charnames;
    char**charname;
    char*encoding[256];
    int c;
    int t;

    if(!t1lib_initialized) {
	T1_SetBitmapPad(16);
	if ((T1_InitLib(NO_LOGFILE)==NULL)){
	    fprintf(stderr, "Initialization of t1lib failed\n");
	    return 0;
	}
	t1lib_initialized = 1;
    }
    nr = T1_AddFont(filename);
    T1_LoadFont(nr);

    charnames = T1_GetAllCharNames(nr);
    if(!charnames) {
	fprintf(stderr, "No Charnames record- not a Type1 Font?\n");
	return 0;
    }

    angle = T1_GetItalicAngle(nr);
    fontname = T1_GetFontName(nr);
    fullname = T1_GetFullName(nr);
    familyname = T1_GetFamilyName(nr);
    underline = T1_GetUnderlinePosition(nr);
    bbox = T1_GetFontBBox(nr);

    font = (SWFFONT*)rfx_calloc(sizeof(SWFFONT));

    font->version = 2;
    if(fontname) 
	font->name = (U8*)strdup(fontname);
    else 
	font->name = 0;
    font->layout = (SWFLAYOUT*)rfx_calloc(sizeof(SWFLAYOUT));

    num = 0;
    charname = charnames;
    while(*charname) {
	charname++;
	if(num<256) {
	    if(*charname) encoding[num] = strdup(*charname);
	    else          encoding[num] = strdup(".notdef");
	}
	num++;
    }
    for(t=num;t<256;t++)
	encoding[t] = strdup(".notdef");
    
    //T1_ReencodeFont(nr, encoding);

    font->maxascii = num;
    font->numchars = num;
    
    font->style = (/*bold*/0?FONT_STYLE_BOLD:0) + (angle>0.05?FONT_STYLE_ITALIC:0);

    font->glyph = (SWFGLYPH*)rfx_calloc(num*sizeof(SWFGLYPH));
    font->glyph2ascii = (U16*)rfx_calloc(num*sizeof(U16));
    font->ascii2glyph = (int*)rfx_calloc(font->maxascii*sizeof(int));
    font->layout->ascent = (U16)(underline - bbox.lly);
    font->layout->descent = (U16)(bbox.ury - underline);
    font->layout->leading = (U16)(font->layout->ascent - 
	                     font->layout->descent -
			     (bbox.lly - bbox.ury));
    font->layout->bounds = (SRECT*)rfx_calloc(sizeof(SRECT)*num);
    font->layout->kerningcount = 0;
    font->layout->kerning = 0;
    font->glyphnames = rfx_calloc(num*sizeof(char*));
  
    num = 0;

    charname = charnames;
    for(c=0;c<font->numchars;c++) {
	drawer_t draw;
	SRECT bbox;
	T1_OUTLINE * outline;
	FPOINT pos,last;
	int firstx;
	
	outline = T1_GetCharOutline(nr, c, 100.0, 0);
	firstx = outline->dest.x/0xffff;

	pos.x = 0;
	pos.y = 0;
	last = pos;
	
	font->glyphnames[c] = strdup(*charname);

	if(c<font->maxascii)
	    font->ascii2glyph[c] = c;
	font->glyph2ascii[c] = c;
	
	swf_Shape01DrawerInit(&draw, 0);

	while(outline) {
	    pos.x += (outline->dest.x/(float)0xffff);
	    pos.y += (outline->dest.y/(float)0xffff);

	    if(outline->type == T1_PATHTYPE_MOVE) {
		draw.moveTo(&draw,&pos);
	    } else if(outline->type == T1_PATHTYPE_LINE) {
		draw.lineTo(&draw,&pos);
	    } else if(outline->type == T1_PATHTYPE_BEZIER) {
		T1_BEZIERSEGMENT*o2 = (T1_BEZIERSEGMENT*)outline;
		FPOINT b,c;
		b.x = o2->B.x/(float)0xffff+last.x;
		b.y = o2->B.y/(float)0xffff+last.y;
		c.x = o2->C.x/(float)0xffff+last.x;
		c.y = o2->C.y/(float)0xffff+last.y;
		draw_cubicTo(&draw,&b,&c,&pos);
	    } else {
		fprintf(stderr, "loadT1Font: unknown outline type:%d\n", outline->type);
	    }
	    last = pos;
	    outline = outline->link;
	}
	
	draw.finish(&draw);

	font->glyph[c].shape = swf_ShapeDrawerToShape(&draw);
	bbox = swf_ShapeDrawerGetBBox(&draw);
	draw.dealloc(&draw);
	    
	font->layout->bounds[c] = bbox;
	font->glyph[c].advance = bbox.xmax;
	if(!font->glyph[c].advance) {
	    font->glyph[c].advance = firstx;
	}
	charname++;
    }
    T1_DeleteFont(nr);

    for(t=0;t<256;t++)
	rfx_free(encoding[t]);
    return font;
}

#else

SWFFONT* swf_LoadT1Font(const char*filename)
{
    fprintf(stderr, "Warning: no t1lib- not able to load %s\n", filename);
    return 0;
}

#endif

SWFFONT* swf_DummyFont()
{
    SWFFONT*font = (SWFFONT*)rfx_calloc(sizeof(SWFFONT));
    return font;
}

static int isSWF(const char*filename)
{
    FILE*fi = fopen(filename, "rb");
    char a[8];
    int l;
    if(!fi) {
	perror(filename);
	return -1;
    }
    memset(a, 0, sizeof(a));
    l = fread(a, 4, 1, fi);
    fclose(fi);
    if(l != 1) return 0;
    if(!strncmp(a, "FWS", 3) || !strncmp(a, "CWS", 3)) {
	return 1;
    }
    return 0;
}

SWFFONT* swf_LoadFont(const char*filename, char flashtype)
{
    int is_swf;
    if(filename == 0)
	return swf_DummyFont();
    is_swf = isSWF(filename);
    if(is_swf<0)
	return 0;
    if(is_swf) {
	SWFFONT*font = swf_ReadFont(filename);
	if(flashtype && font->version==2)
	    fprintf(stderr, "Warning: Can't load font v2 file as flashtype (%s)\n", filename);
	return font;
    }

#if defined(HAVE_FREETYPE)
    return swf_LoadTrueTypeFont(filename, flashtype);
#elif defined(HAVE_T1LIB)
    return swf_LoadT1Font(filename);
#else
    fprintf(stderr, "Error: Neither T1lib nor FreeType support compiled in. Could not load %s\n", filename);
    return 0;
#endif
}

