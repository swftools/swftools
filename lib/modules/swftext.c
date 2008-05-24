/* swftext.c

   Text and font routines

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
   Copyright (c) 2003,2004 Matthias Kramm

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

#include "../rfxswf.h"

U32 readUTF8char(U8 ** text)
{
    U32 c = 0;
    if (!(*(*text) & 0x80))
	return *((*text)++);

    /* 0000 0080-0000 07FF   110xxxxx 10xxxxxx */
    if (((*text)[0] & 0xe0) == 0xc0 && (*text)[1]) {
	c = ((*text)[0] & 0x1f) << 6 | ((*text)[1] & 0x3f);
	(*text) += 2;
	return c;
    }
    /* 0000 0800-0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx */
    if (((*text)[0] & 0xf0) == 0xe0 && (*text)[1] && (*text)[2]) {
	c = ((*text)[0] & 0x0f) << 12 | ((*text)[1] & 0x3f) << 6 | ((*text)[2] & 0x3f);
	(*text) += 3;
	return c;
    }
    /* 0001 0000-001F FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
    if (((*text)[0] & 0xf8) == 0xf0 && (*text)[1] && (*text)[2]
	&& (*text)[3]) {
	c = ((*text)[0] & 0x07) << 18 | ((*text)[1] & 0x3f) << 12 | ((*text)[2] & 0x3f) << 6 | ((*text)[3] & 0x3f);
	(*text) += 4;
	return c;
    }
    /* 0020 0000-03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
    if (((*text)[0] & 0xfc) == 0xf8 && (*text)[1] && (*text)[2]
	&& (*text)[3]
	&& (*text)[4]) {
	c = ((*text)[0] & 0x03) << 24 | ((*text)[1] & 0x3f) << 18 | ((*text)[2] & 0x3f) << 12 | ((*text)[3] & 0x3f) << 6 | ((*text)[4] & 0x3f);
	(*text) += 5;
	return c;
    }
    /* 0400 0000-7FFF FFFF   1111110x 10xxxxxx ... 10xxxxxx */
    if (((*text)[0] & 0xfe) == 0xfc && (*text)[1] && (*text)[2]
	&& (*text)[3]
	&& (*text)[4] && (*text)[5]) {
	c = ((*text)[0] & 0x01) << 30 | ((*text)[1] & 0x3f) << 24 |
	    ((*text)[2] & 0x3f) << 18 | ((*text)[3] & 0x3f) << 12 | ((*text)[4] & 0x3f) << 6 | ((*text)[5] & 0x3f) << 6;
	(*text) += 6;
	return c;
    }
    return *((*text)++);
}

#define TF_TEXTCONTROL  0x80
#define TF_HASFONT      0x08
#define TF_HASCOLOR     0x04
#define TF_HASYOFFSET   0x02
#define TF_HASXOFFSET   0x01

#define FF_WIDECODES    0x01
#define FF_BOLD         0x02
#define FF_ITALIC       0x04
#define FF_ANSI         0x08
#define FF_SHIFTJIS     0x10
#define FF_UNICODE      0x20

#define FF2_BOLD         0x01
#define FF2_ITALIC       0x02
#define FF2_WIDECODES    0x04
#define FF2_WIDEOFFSETS  0x08
#define FF2_ANSI         0x10
#define FF2_UNICODE      0x20
#define FF2_SHIFTJIS     0x40
#define FF2_LAYOUT	 0x80

int swf_FontIsItalic(SWFFONT * f)
{
    return f->style & FONT_STYLE_ITALIC;
}

int swf_FontIsBold(SWFFONT * f)
{
    return f->style & FONT_STYLE_BOLD;
}

static const int WRITEFONTID = 0x4e46;	// font id for WriteFont and ReadFont

int swf_FontEnumerate(SWF * swf, void (*FontCallback) (void*, U16, U8 *), void*self)
{
    int n;
    TAG *t;
    if (!swf)
	return -1;
    t = swf->firstTag;
    n = 0;

    while (t) {
	if (swf_GetTagID(t) == ST_DEFINEFONT2 || swf_GetTagID(t) == ST_DEFINEFONT) {
	    n++;
	    if (FontCallback) {
		U16 id;
		int l;
		U8 s[257];
		s[0] = 0;
		swf_SetTagPos(t, 0);

		id = swf_GetU16(t);
		if (swf_GetTagID(t) == ST_DEFINEFONT2 || swf_GetTagID(t) == ST_DEFINEFONTINFO || swf_GetTagID(t) == ST_DEFINEFONTINFO2) {
		    swf_GetU16(t);
		    l = swf_GetU8(t);
		    swf_GetBlock(t, s, l);
		    s[l] = 0;
		}

		(FontCallback) (self, id, s);
	    }
	}
	t = swf_NextTag(t);
    }
    return n;
}

int swf_FontExtract_DefineFont(int id, SWFFONT * f, TAG * t)
{
    U16 fid;
    swf_SetTagPos(t, 0);

    fid = swf_GetU16(t);
    if ((!id) || (id == fid)) {
	U16 of;
	int n, i;

	id = fid;
	f->version = 1;
	f->id = fid;

	of = swf_GetU16(t);
	n = of / 2;
	f->numchars = n;
	f->glyph = (SWFGLYPH*)rfx_calloc(sizeof(SWFGLYPH) * n);

	for (i = 1; i < n; i++)
	    swf_GetU16(t);
	for (i = 0; i < n; i++)
	    swf_GetSimpleShape(t, &f->glyph[i].shape);
    }
    return id;
}

int swf_FontExtract_DefineFontInfo(int id, SWFFONT * f, TAG * t)
{
    U16 fid;
    U16 maxcode;
    U8 flags;
    swf_SetTagPos(t, 0);

    fid = swf_GetU16(t);
    if (fid == id) {
	U8 l = swf_GetU8(t);
	int i;

	if (f->version > 1) {
	    /* Especially with Flash MX, DefineFont2 may have FontInfo fields,
	       too. However, they only add little information to what's already
	       inside the DefineFont2 tag */
	    return id;
	}

	if (f->name)
	    rfx_free(f->name);

	f->name = (U8 *) rfx_alloc(l + 1);
	swf_GetBlock(t, f->name, l);
	f->name[l] = 0;

	flags = swf_GetU8(t);
	if (flags & 2)
	    f->style |= FONT_STYLE_BOLD;
	if (flags & 4)
	    f->style |= FONT_STYLE_ITALIC;
	if (flags & 8)
	    f->encoding |= FONT_ENCODING_ANSI;
	if (flags & 16)
	    f->encoding |= FONT_ENCODING_SHIFTJIS;
	if (flags & 32)
	    f->encoding |= FONT_ENCODING_UNICODE;

	if (t->id == ST_DEFINEFONTINFO2) {
	    f->language = swf_GetU8(t);
	}

	f->glyph2ascii = (U16 *) rfx_alloc(sizeof(U16) * f->numchars);
	maxcode = 0;
	for (i = 0; i < f->numchars; i++) {
	    f->glyph2ascii[i] = ((flags & FF_WIDECODES) ? swf_GetU16(t) : swf_GetU8(t));
	    if (f->glyph2ascii[i] > maxcode)
		maxcode = f->glyph2ascii[i];
	}
	maxcode++;
	if (maxcode < 256)
	    maxcode = 256;
	f->maxascii = maxcode;
	f->ascii2glyph = (int *) rfx_alloc(sizeof(int) * maxcode);
	memset(f->ascii2glyph, -1, sizeof(int) * maxcode);

	for (i = 0; i < f->numchars; i++)
	    f->ascii2glyph[f->glyph2ascii[i]] = i;
    }
    return id;
}

int swf_FontExtract_GlyphNames(int id, SWFFONT * f, TAG * tag)
{
    U16 fid;
    swf_SetTagPos(tag, 0);

    fid = swf_GetU16(tag);

    if (fid == id) {
	int num = swf_GetU16(tag);
	int t;
	f->glyphnames = (char**)rfx_alloc(sizeof(char *) * num);
	for (t = 0; t < num; t++) {
	    f->glyphnames[t] = strdup(swf_GetString(tag));
	}
    }
    return id;
}


int swf_FontExtract_DefineFont2(int id, SWFFONT * font, TAG * tag)
{
    int t, glyphcount;
    int maxcode;
    int fid;
    U32 offset_start;
    U32 *offset;
    U8 flags1, flags2, namelen;
    swf_SetTagPos(tag, 0);
    font->version = 2;
    fid = swf_GetU16(tag);
    if (id && id != fid)
	return id;
    font->id = fid;
    flags1 = swf_GetU8(tag);
    flags2 = swf_GetU8(tag);	//reserved flags

    if (flags1 & 1)
	font->style |= FONT_STYLE_BOLD;
    if (flags1 & 2)
	font->style |= FONT_STYLE_ITALIC;
    if (flags1 & 16)
	font->encoding |= FONT_ENCODING_ANSI;
    if (flags1 & 32)
	font->encoding |= FONT_ENCODING_UNICODE;
    if (flags1 & 64)
	font->encoding |= FONT_ENCODING_SHIFTJIS;

    namelen = swf_GetU8(tag);
    font->name = (U8 *) rfx_alloc(namelen + 1);
    font->name[namelen] = 0;
    swf_GetBlock(tag, font->name, namelen);
    font->version = 2;
    glyphcount = swf_GetU16(tag);
    font->numchars = glyphcount;

    font->glyph = (SWFGLYPH *) rfx_calloc(sizeof(SWFGLYPH) * glyphcount);
    font->glyph2ascii = (U16 *) rfx_calloc(sizeof(U16) * glyphcount);

    offset = (U32*)rfx_calloc(sizeof(U32)*(glyphcount+1));
    offset_start = tag->pos;

    if (flags1 & 8) {		// wide offsets
	for (t = 0; t < glyphcount; t++)
	    offset[t] = swf_GetU32(tag);	//offset[t]

	if (glyphcount)		/* this _if_ is not in the specs */
	    offset[glyphcount] = swf_GetU32(tag);	// fontcodeoffset
	else
	    offset[glyphcount] = tag->pos;
    } else {
	for (t = 0; t < glyphcount; t++)
	    offset[t] = swf_GetU16(tag);	//offset[t]

	if (glyphcount)		/* this _if_ is not in the specs */
	    offset[glyphcount] = swf_GetU16(tag);	// fontcodeoffset
	else
	    offset[glyphcount] = tag->pos;
    }
    for (t = 0; t < glyphcount; t++) {
	swf_SetTagPos(tag, offset[t]+offset_start);
	swf_GetSimpleShape(tag, &(font->glyph[t].shape));
    }

    swf_SetTagPos(tag, offset[glyphcount]+offset_start);

    free(offset);

    maxcode = 0;
    for (t = 0; t < glyphcount; t++) {
	int code;
	if (flags1 & 4)		// wide codes
	    code = swf_GetU16(tag);
	else
	    code = swf_GetU8(tag);
	font->glyph2ascii[t] = code;
	if (code > maxcode)
	    maxcode = code;
    }
    maxcode++;
    if (maxcode < 256)
	maxcode = 256;
    font->maxascii = maxcode;
    font->ascii2glyph = (int *) rfx_alloc(sizeof(int) * maxcode);
    memset(font->ascii2glyph, -1, sizeof(int) * maxcode);
    for (t = 0; t < glyphcount; t++) {
	font->ascii2glyph[font->glyph2ascii[t]] = t;
    }

    if (flags1 & 128) {		// has layout
	U16 kerningcount;
	font->layout = (SWFLAYOUT *) rfx_alloc(sizeof(SWFLAYOUT));
	font->layout->ascent = swf_GetU16(tag);
	font->layout->descent = swf_GetU16(tag);
	font->layout->leading = swf_GetU16(tag);
	for (t = 0; t < glyphcount; t++) {
	    S16 advance = swf_GetS16(tag);
	    font->glyph[t].advance = advance;
	}
	font->layout->bounds = (SRECT*)rfx_alloc(glyphcount * sizeof(SRECT));
	for (t = 0; t < glyphcount; t++) {
	    swf_ResetReadBits(tag);
	    swf_GetRect(tag, &font->layout->bounds[t]);
	}

	kerningcount = swf_GetU16(tag);
	font->layout->kerningcount = kerningcount;

	font->layout->kerning = (SWFKERNING *) rfx_alloc(sizeof(SWFKERNING) * kerningcount);
	if (kerningcount) {
	    font->layout->kerning = (SWFKERNING*)rfx_alloc(sizeof(*font->layout->kerning) * kerningcount);
	    for (t = 0; t < kerningcount; t++) {
		if (flags1 & 4) {	// wide codes
		    font->layout->kerning[t].char1 = swf_GetU16(tag);
		    font->layout->kerning[t].char2 = swf_GetU16(tag);
		} else {
		    font->layout->kerning[t].char1 = swf_GetU8(tag);
		    font->layout->kerning[t].char2 = swf_GetU8(tag);
		}
		font->layout->kerning[t].adjustment = swf_GetS16(tag);
	    }
	}
    }
    return font->id;
}


#define FEDTJ_PRINT  0x01
#define FEDTJ_MODIFY 0x02
#define FEDTJ_CALLBACK 0x04

static int
swf_FontExtract_DefineTextCallback(int id, SWFFONT * f, TAG * t, int jobs,
				   void (*callback) (void *self,
						     int *chars, int *xpos, int nr, int fontid, int fontsize, int xstart, int ystart, RGBA * color), void *self)
{
    U16 cid;
    SRECT r;
    MATRIX m;
    U8 gbits, abits;
    int fid = -1;
    RGBA color;
    int x = 0, y = 0;
    int fontsize = 0;

    memset(&color, 0, sizeof(color));

    swf_SetTagPos(t, 0);

    cid = swf_GetU16(t);
    swf_GetRect(t, &r);
    swf_GetMatrix(t, &m);
    gbits = swf_GetU8(t);
    abits = swf_GetU8(t);

    while (1) {
	int flags, num;
	flags = swf_GetU8(t);
	if (!flags)
	    break;

	if (flags & TF_TEXTCONTROL) {
	    if (flags & TF_HASFONT)
		fid = swf_GetU16(t);
	    if (flags & TF_HASCOLOR) {
		color.r = swf_GetU8(t);	// rgb
		color.g = swf_GetU8(t);
		color.b = swf_GetU8(t);
		if (swf_GetTagID(t) == ST_DEFINETEXT2)
		    color.a = swf_GetU8(t);
		else
		    color.a = 255;
	    }
	    if (flags & TF_HASXOFFSET)
		x = swf_GetS16(t);
	    if (flags & TF_HASYOFFSET)
		y = swf_GetS16(t);
	    if (flags & TF_HASFONT)
		fontsize = swf_GetU16(t);
	}

	num = swf_GetU8(t);
	if (!num)
	    break;

	{
	    int i;
	    int buf[256];
	    int advance[256];
	    int xpos = 0;
	    for (i = 0; i < num; i++) {
		int glyph;
		int adv = 0;
		advance[i] = xpos;
		glyph = swf_GetBits(t, gbits);
		adv = swf_GetBits(t, abits);
		xpos += adv;

		if (id == fid) {
		    if (jobs & FEDTJ_PRINT) {
			int code = f->glyph2ascii[glyph];
			printf("%lc", code);
		    }
		    if (jobs & FEDTJ_MODIFY)
			f->glyph[glyph].advance = adv * 20;	//?
		}

		buf[i] = glyph;
	    }
	    if ((id == fid) && (jobs & FEDTJ_PRINT))
		printf("\n");
	    if (jobs & FEDTJ_CALLBACK)
		callback(self, buf, advance, num, fid, fontsize, x, y, &color);
	    x += xpos;
	}
    }

    return id;
}

int swf_ParseDefineText(TAG * tag,
		    void (*callback) (void *self, int *chars, int *ypos, int nr, int fontid, int fontsize, int xstart, int ystart, RGBA * color), void *self)
{
    return swf_FontExtract_DefineTextCallback(-1, 0, tag, FEDTJ_CALLBACK, callback, self);
}

int swf_FontExtract_DefineText(int id, SWFFONT * f, TAG * t, int jobs)
{
    return swf_FontExtract_DefineTextCallback(id, f, t, jobs, 0, 0);
}

int swf_FontExtract(SWF * swf, int id, SWFFONT * *font)
{
    TAG *t;
    SWFFONT *f;

    if ((!swf) || (!font))
	return -1;

    f = (SWFFONT *) rfx_calloc(sizeof(SWFFONT));

    t = swf->firstTag;

    while (t) {
	int nid = 0;
	switch (swf_GetTagID(t)) {
	case ST_DEFINEFONT:
	    nid = swf_FontExtract_DefineFont(id, f, t);
	    break;

	case ST_DEFINEFONT2:
	    nid = swf_FontExtract_DefineFont2(id, f, t);
	    break;

	case ST_DEFINEFONTINFO:
	case ST_DEFINEFONTINFO2:
	    nid = swf_FontExtract_DefineFontInfo(id, f, t);
	    break;

	case ST_DEFINETEXT:
	case ST_DEFINETEXT2:
	    nid = swf_FontExtract_DefineText(id, f, t, f->layout ? 0 : FEDTJ_MODIFY);
	    break;

	case ST_GLYPHNAMES:
	    nid = swf_FontExtract_GlyphNames(id, f, t);
	    break;
	}
	if (nid > 0)
	    id = nid;
	t = swf_NextTag(t);
    }
    if (f->id != id) {
	rfx_free(f);
	f = 0;
    }
    font[0] = f;
    return 0;
}

int swf_FontSetID(SWFFONT * f, U16 id)
{
    if (!f)
	return -1;
    f->id = id;
    return 0;
}

void swf_LayoutFree(SWFLAYOUT * l)
{
    if (l) {
	if (l->kerning)
	    rfx_free(l->kerning);
	l->kerning = NULL;
	if (l->bounds)
	    rfx_free(l->bounds);
	l->bounds = NULL;
    }
    rfx_free(l);
}


static void font_freeglyphnames(SWFFONT*f)
{
    if (f->glyphnames)
    {
        int t;
        for (t = 0; t < f->numchars; t++)
        {
            if (f->glyphnames[t])
            {
                rfx_free(f->glyphnames[t]);
                f->glyphnames[t] = 0;
            }
        }
        rfx_free(f->glyphnames);
        f->glyphnames = 0;
	}
}
static void font_freeusage(SWFFONT*f)
{
    if (f->use) {
	if(f->use->chars) {
	    rfx_free(f->use->chars);f->use->chars = 0;
	}
	rfx_free(f->use); f->use = 0;
    }
}
static void font_freelayout(SWFFONT*f)
{
    if (f->layout) {
	swf_LayoutFree(f->layout);
	f->layout = 0;
    }
}
static void font_freename(SWFFONT*f)
{
    if (f->name) {
	rfx_free(f->name);
	f->name = 0;
    }
}

int swf_FontReduce_old(SWFFONT * f)
{
    int i, j;
    int max_unicode = 0;
    if ((!f) || (!f->use) || f->use->is_reduced)
	return -1;

    j = 0;

    for (i = 0; i < f->numchars; i++) {
	if (f->glyph[i].shape && f->use->chars[i]) {
	    f->glyph2ascii[j] = f->glyph2ascii[i];
	    f->glyph[j] = f->glyph[i];
	    f->use->chars[i] = j;
	    j++;
	} else {
	    f->glyph2ascii[i] = 0;
	    if(f->glyph[i].shape) {
		swf_ShapeFree(f->glyph[i].shape);
		f->glyph[i].shape = 0;
		f->glyph[i].advance = 0;
	    }
	    f->use->chars[i] = -1;
	    j++; //TODO: remove
	}
    }
    for (i = 0; i < f->maxascii; i++) {
	if(f->use->chars[f->ascii2glyph[i]]<0) {
	    f->ascii2glyph[i] = -1;
	} else {
	    f->ascii2glyph[i] = f->use->chars[f->ascii2glyph[i]];
	    max_unicode = i;
	}
    }
    f->maxascii = max_unicode;
    f->use->is_reduced = 1;
    f->numchars = j;
    font_freelayout(f);
    font_freeglyphnames(f);
    font_freename(f);
    return j;
}

int swf_FontReduce_swfc(SWFFONT * f)
{
    int i, j;
    int max_unicode = 0;
    if ((!f) || (!f->use) || f->use->is_reduced)
	return -1;

    font_freeglyphnames(f);

    j = 0;
    for (i = 0; i < f->numchars; i++) {
	if (f->glyph[i].shape && f->use->chars[i]) {
	    f->glyph2ascii[j] = f->glyph2ascii[i];
	    if (f->layout)
	    	f->layout->bounds[j] = f->layout->bounds[i];
	    f->glyph[j] = f->glyph[i];
	    f->use->chars[i] = j;
	    j++;
	} else {
	    f->glyph2ascii[i] = 0;
	    if(f->glyph[i].shape) {
		swf_ShapeFree(f->glyph[i].shape);
		f->glyph[i].shape = 0;
		f->glyph[i].advance = 0;
	    }
	    f->use->chars[i] = -1;
	}
    }
    f->use->used_glyphs = j;
    for (i = 0; i < f->maxascii; i++) {
	if(f->ascii2glyph[i] > -1) {
	    if (f->use->chars[f->ascii2glyph[i]]<0) {
	    	f->use->chars[f->ascii2glyph[i]] = 0;
	    	f->ascii2glyph[i] = -1;
	    } else {
	    	f->ascii2glyph[i] = f->use->chars[f->ascii2glyph[i]];
	    	f->use->chars[f->ascii2glyph[i]] = 1;
	    	max_unicode = i + 1;
	    }
	}
    }
    f->maxascii = max_unicode;
    f->use->is_reduced = 1;
    f->numchars = j;
    font_freename(f);
    return j;
}

int swf_FontReduce(SWFFONT * f)
{
    int i;
    int max_unicode = 0;
    int max_glyph = 0;
    if ((!f) || (!f->use) || f->use->is_reduced)
	return -1;

    font_freelayout(f);
    font_freeglyphnames(f);

    f->use->used_glyphs= 0;
    for (i = 0; i < f->numchars; i++) {
	if(!f->use->chars[i]) {
	    if(f->glyph2ascii) {
		f->glyph2ascii[i] = 0;
	    }
	    if(f->glyph[i].shape) {
		swf_ShapeFree(f->glyph[i].shape);
		f->glyph[i].shape = 0;
		f->glyph[i].advance = 0;
	    }
//	    f->use->used_glyphs++;
	} else {
	    f->use->used_glyphs++;
	    max_glyph = i+1;
	}
    }
    for (i = 0; i < f->maxascii; i++) {
	if(f->ascii2glyph[i] > -1 && !f->use->chars[f->ascii2glyph[i]]) {
	    if(f->ascii2glyph) {
		f->ascii2glyph[i] = -1;
	    }
	} else {
	    max_unicode = i+1;
	}
    }
    f->maxascii = max_unicode;
    f->numchars = max_glyph;
    font_freename(f);
    return 0;
}

void swf_FontSort(SWFFONT * font)
{
    int i, j;
    int *newplace;
    int *newpos;
    if (!font)
	return;

    newplace = (int*)rfx_alloc(sizeof(int) * font->numchars);

    for (i = 0; i < font->numchars; i++) {
	newplace[i] = i;
    }
    for (i = 0; i < font->numchars; i++)
	for (j = 0; j < i; j++) {
	    if (font->glyph2ascii[i] < font->glyph2ascii[j]) {
		int n1, n2;
		char *c1, *c2;
		SWFGLYPH g1, g2;
		SRECT r1, r2;
		n1 = newplace[i];
		n2 = newplace[j];
		newplace[j] = n1;
		newplace[i] = n2;
		n1 = font->glyph2ascii[i];
		n2 = font->glyph2ascii[j];
		font->glyph2ascii[j] = n1;
		font->glyph2ascii[i] = n2;
		g1 = font->glyph[i];
		g2 = font->glyph[j];
		font->glyph[j] = g1;
		font->glyph[i] = g2;
		if (font->glyphnames) {
		    c1 = font->glyphnames[i];
		    c2 = font->glyphnames[j];
		    font->glyphnames[j] = c1;
		    font->glyphnames[i] = c2;
		}
		if (font->layout) {
		    r1 = font->layout->bounds[i];
		    r2 = font->layout->bounds[j];
		    font->layout->bounds[j] = r1;
		    font->layout->bounds[i] = r2;
		}
	    }
	}
    newpos = (int*)rfx_alloc(sizeof(int) * font->numchars);
    for (i = 0; i < font->numchars; i++) {
	newpos[newplace[i]] = i;
    }
    for (i = 0; i < font->maxascii; i++) {
	if (font->ascii2glyph[i] >= 0)
	    font->ascii2glyph[i] = newpos[font->ascii2glyph[i]];
    }

    rfx_free(newpos);
    rfx_free(newplace);
}

void swf_FontPrepareForEditText(SWFFONT * font)
{
    if (!font->layout)
	swf_FontCreateLayout(font);
    swf_FontSort(font);
}

int swf_FontInitUsage(SWFFONT * f)
{
    if (!f)
	return -1;
    if(f->use) {
	fprintf(stderr, "Usage initialized twice");
	return -1;
    }
    f->use = (FONTUSAGE*)rfx_alloc(sizeof(FONTUSAGE));
    f->use->is_reduced = 0;
    f->use->used_glyphs = 0;
    f->use->chars = (int*)rfx_calloc(sizeof(f->use->chars[0]) * f->numchars);
    f->use->glyphs_specified = 0;
    return 0;
}

void swf_FontClearUsage(SWFFONT * f)
{
    if (!f || !f->use)
	return;
    rfx_free(f->use->chars); f->use->chars = 0;
    rfx_free(f->use); f->use = 0;
}

int swf_FontUse(SWFFONT * f, U8 * s)
{
    if( (!s))
	return -1;
    while (*s) {
	if(*s < f->maxascii && f->ascii2glyph[*s]>=0)
	    swf_FontUseGlyph(f, f->ascii2glyph[*s]);
	s++;
    }
    return 0;
}

int swf_FontUseUTF8(SWFFONT * f, U8 * s)
{
    if( (!s))
	return -1;
    int ascii;
    while (*s)
    {
    	ascii = readUTF8char(&s);
	if(ascii < f->maxascii && f->ascii2glyph[ascii]>=0)
	    swf_FontUseGlyph(f, f->ascii2glyph[ascii]);
    }
    return 0;
}

int swf_FontUseAll(SWFFONT* f)
{
    int i;

    if (!f->use)
	swf_FontInitUsage(f);
    for (i = 0; i < f->numchars; i++)
    	f->use->chars[i] = 1;
    f->use->used_glyphs = f->numchars;
    return 0;
}

int swf_FontUseGlyph(SWFFONT * f, int glyph)
{
    if (!f->use)
	swf_FontInitUsage(f);
    if(glyph < 0 || glyph >= f->numchars)
	return -1;
    if(!f->use->chars[glyph])
	f->use->used_glyphs++;
    f->use->chars[glyph] = 1;
    return 0;
}

int swf_FontSetDefine(TAG * t, SWFFONT * f)
{
    U16 *ofs = (U16 *) rfx_alloc(f->numchars * 2);
    int p, i, j;

    if ((!t) || (!f))
	return -1;
    swf_ResetWriteBits(t);
    swf_SetU16(t, f->id);

    p = 0;
    j = 0;
    for (i = 0; i < f->numchars; i++)
	if (f->glyph[i].shape) {
	    ofs[j++] = p;
	    p += swf_SetSimpleShape(NULL, f->glyph[i].shape);
	}

    for (i = 0; i < j; i++)
	swf_SetU16(t, ofs[i] + j * 2);
    if (!j) {
	fprintf(stderr, "rfxswf: warning: Font is empty\n");
	swf_SetU16(t, 0);
    }

    for (i = 0; i < f->numchars; i++)
	if (f->glyph[i].shape)
	    swf_SetSimpleShape(t, f->glyph[i].shape);

    swf_ResetWriteBits(t);
    rfx_free(ofs);
    return 0;
}

static inline int fontSize(SWFFONT * font)
{
    int t;
    int size = 0;
    for (t = 0; t < font->numchars; t++) {
	int l = 0;
	if(font->glyph[t].shape)
	    l = (font->glyph[t].shape->bitlen + 7) / 8;
	else
	    l = 8;
	size += l + 1;
    }
    return size + (font->numchars + 1) * 2;
}

int swf_FontSetDefine2(TAG * tag, SWFFONT * f)
{
    U8 flags = 0;
    int t;
    int pos;
    swf_SetU16(tag, f->id);

    if (f->layout) flags |= 128;		// haslayout
    if (f->numchars > 256)
	flags |= 4;		// widecodes
    if (f->style & FONT_STYLE_BOLD)
	flags |= 1;		// bold
    if (f->style & FONT_STYLE_ITALIC)
	flags |= 2;		// italic
    if (f->maxascii >= 256)
	flags |= 4;		//wide codecs
    if (fontSize(f) > 65535)
	flags |= 8;		//wide offsets
    flags |= 8 | 4;		//FIXME: the above check doesn't work

    if (f->encoding & FONT_ENCODING_ANSI)
	flags |= 16;		// ansi
    if (f->encoding & FONT_ENCODING_UNICODE)
	flags |= 32;		// unicode
    if (f->encoding & FONT_ENCODING_SHIFTJIS)
	flags |= 64;		// shiftjis

    swf_SetU8(tag, flags);
    swf_SetU8(tag, 0);		//reserved flags
    if (f->name) {
	/* font name */
	swf_SetU8(tag, strlen((const char*)f->name)+1);
	swf_SetBlock(tag, f->name, strlen((const char*)f->name)+1);
    } else {
	/* font name (="") */
	swf_SetU8(tag, 1);
	swf_SetU8(tag, 0);
    }
    /* number of glyphs */
    swf_SetU16(tag, f->numchars);
    /* font offset table */
    pos = tag->len;
    for (t = 0; t <= f->numchars; t++) {
	if (flags & 8)
	    swf_SetU32(tag, /* fontoffset */ 0);	/*placeholder */
	else
	    swf_SetU16(tag, /* fontoffset */ 0);	/*placeholder */
    }

    for (t = 0; t <= f->numchars; t++) {
	if (flags & 8) {
	    tag->data[pos + t * 4] = (tag->len - pos);
	    tag->data[pos + t * 4 + 1] = (tag->len - pos) >> 8;
	    tag->data[pos + t * 4 + 2] = (tag->len - pos) >> 16;
	    tag->data[pos + t * 4 + 3] = (tag->len - pos) >> 24;
	} else {
	    if (tag->len - pos > 65535) {
		fprintf(stderr, "Internal error: Font too big and WideOffsets flag not set\n");
		exit(1);
	    }
	    tag->data[pos + t * 2] = (tag->len - pos);
	    tag->data[pos + t * 2 + 1] = (tag->len - pos) >> 8;
	}
	if (t < f->numchars) {
	    if(f->glyph[t].shape) {
		swf_SetSimpleShape(tag, f->glyph[t].shape);
	    } else {
		swf_SetU8(tag, 0); //non-edge(1) + edge flags(5)
	    }
	}
    }


    /* font code table */
    for (t = 0; t < f->numchars; t++) {
	if (flags & 4) {		/* wide codes */
	    if(f->glyph2ascii[t]) {
		swf_SetU16(tag, f->glyph2ascii[t]);
	    } else {
		swf_SetU16(tag, 0);
	    }
	} else {
	    if(f->glyph2ascii[t]) {
		swf_SetU8(tag, f->glyph2ascii[t]);
	    } else {
		swf_SetU8(tag, 0);
	    }
	}
    }

    if (f->layout) {
	swf_SetU16(tag, f->layout->ascent);
	swf_SetU16(tag, f->layout->descent);
	swf_SetU16(tag, f->layout->leading);
	for (t = 0; t < f->numchars; t++)
	    swf_SetU16(tag, f->glyph[t].advance);
	for (t = 0; t < f->numchars; t++) {
	    swf_ResetWriteBits(tag);
	    swf_SetRect(tag, &f->layout->bounds[t]);
	}
	swf_SetU16(tag, f->layout->kerningcount);
	for (t = 0; t < f->layout->kerningcount; t++) {
	    if (flags & 4) {	/* wide codes */
		swf_SetU16(tag, f->layout->kerning[t].char1);
		swf_SetU16(tag, f->layout->kerning[t].char2);
	    } else {
		swf_SetU8(tag, f->layout->kerning[t].char1);
		swf_SetU8(tag, f->layout->kerning[t].char2);
	    }
	    swf_SetU16(tag, f->layout->kerning[t].adjustment);
	}
    }
    return 0;
}

void swf_FontAddLayout(SWFFONT * f, int ascent, int descent, int leading)
{
    f->layout = (SWFLAYOUT *) rfx_alloc(sizeof(SWFLAYOUT));
    f->layout->ascent = ascent;
    f->layout->descent = descent;
    f->layout->leading = leading;
    f->layout->kerningcount = 0;
    f->layout->kerning = 0;
    f->layout->bounds = (SRECT *) rfx_calloc(sizeof(SRECT) * f->numchars);
}

int swf_FontSetInfo(TAG * t, SWFFONT * f)
{
    int l, i;
    U8 wide = 0;
    U8 flags = 0;
    if ((!t) || (!f))
	return -1;
    swf_ResetWriteBits(t);
    swf_SetU16(t, f->id);
    l = f->name ? strlen((const char *)f->name) : 0;
    if (l > 255)
	l = 255;
    swf_SetU8(t, l);
    if (l)
	swf_SetBlock(t, f->name, l);
    if (f->numchars >= 256)
	wide = 1;

    if (f->style & FONT_STYLE_BOLD)
	flags |= 2;
    if (f->style & FONT_STYLE_ITALIC)
	flags |= 4;
    if (f->style & FONT_ENCODING_ANSI)
	flags |= 8;
    if (f->style & FONT_ENCODING_SHIFTJIS)
	flags |= 16;
    if (f->style & FONT_ENCODING_UNICODE)
	flags |= 32;

    swf_SetU8(t, (flags & 0xfe) | wide);

    for (i = 0; i < f->numchars; i++) {
	if (f->glyph[i].shape) {
	    int g2a = f->glyph2ascii?f->glyph2ascii[i]:0;
	    wide ? swf_SetU16(t, g2a) : swf_SetU8(t, g2a);
	}
    }

    return 0;
}

int swf_TextPrintDefineText(TAG * t, SWFFONT * f)
{
    int id = swf_GetTagID(t);
    if ((id == ST_DEFINETEXT) || (id == ST_DEFINETEXT2))
	swf_FontExtract_DefineText(f->id, f, t, FEDTJ_PRINT);
    else
	return -1;
    return 0;
}

void swf_FontFree(SWFFONT * f)
{
    int i;
    if (!f)
        return;

    if (f->glyph)
    {
        for (i = 0; i < f->numchars; i++)
            if (f->glyph[i].shape)
            {
                swf_ShapeFree(f->glyph[i].shape);
                f->glyph[i].shape = NULL;
            }
            rfx_free(f->glyph);
            f->glyph = NULL;
    }
    if (f->ascii2glyph)
    {
        rfx_free(f->ascii2glyph);
        f->ascii2glyph = NULL;
    }
    if (f->glyph2ascii)
    {
        rfx_free(f->glyph2ascii);
        f->glyph2ascii = NULL;
    }
    font_freename(f);
    font_freelayout(f);
    font_freeglyphnames(f);
    font_freeusage(f);

    rfx_free(f);
}

int swf_TextSetInfoRecord(TAG * t, SWFFONT * font, U16 size, RGBA * color, int x, int y)
{
    U8 flags;
    if (!t)
	return -1;

    flags = TF_TEXTCONTROL | (font ? TF_HASFONT : 0) | (color ? TF_HASCOLOR : 0) | (x ? TF_HASXOFFSET : 0)
	| (y ? TF_HASYOFFSET : 0);

    swf_SetU8(t, flags);
    if (font)
	swf_SetU16(t, font->id);
    if (color) {
	if (swf_GetTagID(t) == ST_DEFINETEXT2)
	    swf_SetRGBA(t, color);
	else
	    swf_SetRGB(t, color);
    }
    if (x) {
	if(x != SET_TO_ZERO) {
	    if(x>32767 || x<-32768)
		fprintf(stderr, "Warning: Horizontal char position overflow: %d\n", x);
	    swf_SetS16(t, x);
	} else {
	    swf_SetS16(t, 0);
	}
    }
    if (y) {
	if(y != SET_TO_ZERO) {
	    if(y>32767 || y<-32768)
		fprintf(stderr, "Warning: Vertical char position overflow: %d\n", y);
	    swf_SetS16(t, y);
	} else {
	    swf_SetS16(t, 0);
	}
    }
    if (font)
	swf_SetU16(t, size);

    return 0;
}

static int swf_TextCountBits2(SWFFONT * font, U8 * s, int scale, U8 * gbits, U8 * abits, char *encoding)
{
    U16 g, a;
    char utf8 = 0;
    if ((!s) || (!font) || ((!gbits) && (!abits)) || (!font->ascii2glyph))
	return -1;
    g = a = 0;

    if (!strcmp(encoding, "UTF8"))
	utf8 = 1;
    else if (!strcmp(encoding, "iso-8859-1"))
	utf8 = 0;
    else
	fprintf(stderr, "Unknown encoding: %s", encoding);

    while (*s) {
	int glyph = -1, c;

	if (!utf8)
	    c = *s++;
	else
	    c = readUTF8char(&s);

	if (c < font->maxascii)
	    glyph = font->ascii2glyph[c];
	if (glyph >= 0) {
	    g = swf_CountUBits(glyph, g);
	    a = swf_CountBits(((((U32) font->glyph[glyph].advance) * scale) / 20) / 100, a);
	}
    }

    if (gbits)
	gbits[0] = (U8) g;
    if (abits)
	abits[0] = (U8) a;
    return 0;
}

static int swf_TextSetCharRecord2(TAG * t, SWFFONT * font, U8 * s, int scale, U8 gbits, U8 abits, char *encoding)
{
    int l = 0, pos;
    char utf8 = 0;

    if ((!t) || (!font) || (!s) || (!font->ascii2glyph))
	return -1;

    if (!strcmp(encoding, "UTF8"))
	utf8 = 1;
    else if (!strcmp(encoding, "iso-8859-1"))
	utf8 = 0;
    else
	fprintf(stderr, "Unknown encoding: %s", encoding);

    pos = t->len;
    swf_SetU8(t, l);		//placeholder

    while (*s) {
	int g = -1, c;

	if (!utf8)
	    c = *s++;
	else
	    c = readUTF8char(&s);

	if (c < font->maxascii)
	    g = font->ascii2glyph[c];
	if (g >= 0) {
	    swf_SetBits(t, g, gbits);
	    swf_SetBits(t, ((((U32) font->glyph[g].advance) * scale) / 20) / 100, abits);
	    l++;
	    /* We split into 127 characters per text field.
	       We could do 255, by the (formerly wrong) flash specification,
	       but some SWF parsing code out there still assumes that char blocks
	       are at max 127 characters, and it would save only a few bits.
	    */
	    if (l == 0x7f)
		break;
	}
    }

    PUT8(&t->data[pos], l);

    swf_ResetWriteBits(t);
    return 0;
}

int swf_TextCountBits(SWFFONT * font, U8 * s, int scale, U8 * gbits, U8 * abits)
{
    return swf_TextCountBits2(font, s, scale, gbits, abits, "iso-8859-1");
}

int swf_TextSetCharRecord(TAG * t, SWFFONT * font, U8 * s, int scale, U8 gbits, U8 abits)
{
    return swf_TextSetCharRecord2(t, font, s, scale, gbits, abits, "iso-8859-1");
}

int swf_TextCountBitsUTF8(SWFFONT * font, U8 * s, int scale, U8 * gbits, U8 * abits)
{
    return swf_TextCountBits2(font, s, scale, gbits, abits, "UTF8");
}

int swf_TextSetCharRecordUTF8(TAG * t, SWFFONT * font, U8 * s, int scale, U8 gbits, U8 abits)
{
    return swf_TextSetCharRecord2(t, font, s, scale, gbits, abits, "UTF8");
}

U32 swf_TextGetWidth(SWFFONT * font, U8 * s, int scale)
{
    U32 res = 0;

    if (font && s) {
	while (s[0]) {
	    int g = -1;
	    if (*s < font->maxascii)
		g = font->ascii2glyph[*s];
	    if (g >= 0)
		res += font->glyph[g].advance / 20;
	    s++;
	}
	if (scale)
	    res = (res * scale) / 100;
    }
    return res;
}

SRECT swf_TextCalculateBBoxUTF8(SWFFONT * font, U8 * s, int scale)
{
    int xpos = 0;
    int ypos = 0;
    SRECT r;
    swf_GetRect(0, &r);
    while (*s) {
	int c = readUTF8char(&s);
	if(c==13 || c==10) {
	    if(s[0] == 10) {
		s++;
	    }
	    xpos=0;
	    ypos+=font->layout->leading;
	    continue;
	}
	if (c < font->maxascii) {
	    int g = font->ascii2glyph[c];
	    if (g >= 0) {
		SRECT rn = font->layout->bounds[g];
		rn.xmin = (rn.xmin * scale) / 20 / 100 + xpos;
		rn.xmax = (rn.xmax * scale) / 20 / 100 + xpos;
		rn.ymin = (rn.ymin * scale) / 20 / 100 + ypos;
		rn.ymax = (rn.ymax * scale) / 20 / 100 + ypos;
		swf_ExpandRect2(&r, &rn);
		xpos += (font->glyph[g].advance * scale) / 20 / 100;
	    }
	}
    }
    return r;
}


SWFFONT *swf_ReadFont(char *filename)
{
    int f;
    SWF swf;
    if (!filename)
	return 0;
    f = open(filename, O_RDONLY|O_BINARY);

    if (f < 0 || swf_ReadSWF(f, &swf) < 0) {
	fprintf(stderr, "%s is not a valid SWF font file or contains errors.\n", filename);
	close(f);
	return 0;
    } else {
	SWFFONT *font;
	close(f);
	if (swf_FontExtract(&swf, WRITEFONTID, &font) < 0)
	    return 0;
	swf_FreeTags(&swf);
	return font;
    }
}

void swf_WriteFont(SWFFONT * font, char *filename)
{
    SWF swf;
    TAG *t;
    SRECT r;
    RGBA rgb;
    int f;
    int useDefineFont2 = 0;
    int storeGlyphNames = 1;

    if (font->layout)
	useDefineFont2 = 1;	/* the only thing new in definefont2
				   is layout information. */

    font->id = WRITEFONTID;	//"FN"

    memset(&swf, 0x00, sizeof(SWF));

    swf.fileVersion = 4;
    swf.frameRate = 0x4000;

    /* if we use DefineFont1 to store the characters,
       we have to build a textfield to store the
       advance values. While at it, we can also
       make the whole .swf viewable */

    /* we now always create viewable swfs, even if we
       did use definefont2 -mk */
    t = swf_InsertTag(NULL, ST_SETBACKGROUNDCOLOR);
    swf.firstTag = t;
    rgb.r = 0xef;
    rgb.g = 0xef;
    rgb.b = 0xff;
    swf_SetRGB(t, &rgb);
    if (!useDefineFont2) {
	t = swf_InsertTag(t, ST_DEFINEFONT);
	swf_FontSetDefine(t, font);
	t = swf_InsertTag(t, ST_DEFINEFONTINFO);
	swf_FontSetInfo(t, font);
    } else {
	t = swf_InsertTag(t, ST_DEFINEFONT2);
	swf_FontSetDefine2(t, font);
    }

    if (storeGlyphNames && font->glyphnames) {
	int c;
	t = swf_InsertTag(t, ST_GLYPHNAMES);
	swf_SetU16(t, WRITEFONTID);
	swf_SetU16(t, font->numchars);
	for (c = 0; c < font->numchars; c++) {
	    if (font->glyphnames[c])
		swf_SetString(t, (U8*)font->glyphnames[c]);
	    else
		swf_SetString(t, (U8*)"");
	}
    }

    if (1)			//neccessary only for df1, but pretty to look at anyhow, so do it always
    {
	int textscale = 400;
	int s;
	int xmax = 0;
	int ymax = 0;
	int ypos = 1;
	U8 gbits, abits;
	int x, y, c;
	int range = font->maxascii;

	c = 0;
	if (useDefineFont2 && range > 256) {
	    range = 256;
	}

	for (s = 0; s < range; s++) {
	    int g = font->ascii2glyph[s];
	    if (g >= 0) {
		if ((font->glyph[g].advance * textscale / 20) / 64 > xmax) {
		    xmax = (font->glyph[g].advance * textscale / 20) / 64;
		}
		c++;
	    }
	    if ((s & 15) == 0) {
		if (c) {
		    ypos++;
		}
		c = 0;
	    }
	}
	ymax = ypos * textscale * 2;

	swf.movieSize.xmax = xmax * 20;
	swf.movieSize.ymax = ymax;

	t = swf_InsertTag(t, ST_DEFINETEXT);

	swf_SetU16(t, font->id + 1);	// ID

	r.xmin = 0;
	r.ymin = 0;
	r.xmax = swf.movieSize.xmax;
	r.ymax = swf.movieSize.ymax;

	swf_SetRect(t, &r);

	swf_SetMatrix(t, NULL);

	abits = swf_CountBits(xmax * 16, 0);
	gbits = 8;

	swf_SetU8(t, gbits);
	swf_SetU8(t, abits);

	rgb.r = 0x00;
	rgb.g = 0x00;
	rgb.b = 0x00;
	ypos = 1;
	for (y = 0; y < ((range + 15) / 16); y++) {
	    int c = 0, lastx = -1;
	    for (x = 0; x < 16; x++) {
		int g = (y * 16 + x < range) ? font->ascii2glyph[y * 16 + x] : -1;
		if (g >= 0 && font->glyph[g].shape) {
		    c++;
		    if (lastx < 0)
			lastx = x * xmax;
		}
	    }
	    if (c) {
		swf_TextSetInfoRecord(t, font, textscale, &rgb, lastx + 1, textscale * ypos * 2);
		for (x = 0; x < 16; x++) {
		    int g = (y * 16 + x < range) ? font->ascii2glyph[y * 16 + x] : -1;
		    if (g >= 0 && font->glyph[g].shape) {
			if (lastx != x * xmax) {
			    swf_TextSetInfoRecord(t, 0, 0, 0, x * xmax + 1, 0);
			}
			swf_SetU8(t, 1);
			swf_SetBits(t, g, gbits);
			swf_SetBits(t, font->glyph[g].advance / 20, abits);
			lastx = x * xmax + (font->glyph[g].advance / 20);
			swf_ResetWriteBits(t);
		    }
		}
		ypos++;
	    }
	}
	swf_SetU8(t, 0);


	t = swf_InsertTag(t, ST_PLACEOBJECT2);

	swf_ObjectPlace(t, font->id + 1, 1, NULL, NULL, NULL);

	t = swf_InsertTag(t, ST_SHOWFRAME);

    }

    t = swf_InsertTag(t, ST_END);

    f = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0644);
    if FAILED
	(swf_WriteSWF(f, &swf)) fprintf(stderr, "WriteSWF() failed in writeFont().\n");
    close(f);

    swf_FreeTags(&swf);
}


void swf_SetEditText(TAG * tag, U16 flags, SRECT r, char *text, RGBA * color, int maxlength, U16 font, U16 height, EditTextLayout * layout, char *variable)
{
    swf_SetRect(tag, &r);
    swf_ResetWriteBits(tag);

    flags &= ~(ET_HASTEXT | ET_HASTEXTCOLOR | ET_HASMAXLENGTH | ET_HASFONT | ET_HASLAYOUT);
    if (text)
	flags |= ET_HASTEXT;
    if (color)
	flags |= ET_HASTEXTCOLOR;
    if (maxlength)
	flags |= ET_HASMAXLENGTH;
    if (font)
	flags |= ET_HASFONT;
    if (layout)
	flags |= ET_HASLAYOUT;

    swf_SetBits(tag, flags, 16);

    if (flags & ET_HASFONT) {
	swf_SetU16(tag, font);	//font
	swf_SetU16(tag, height);	//fontheight
    }
    if (flags & ET_HASTEXTCOLOR) {
	swf_SetRGBA(tag, color);
    }
    if (flags & ET_HASMAXLENGTH) {
	swf_SetU16(tag, maxlength);	//maxlength
    }
    if (flags & ET_HASLAYOUT) {
	swf_SetU8(tag, layout->align);	//align
	swf_SetU16(tag, layout->leftmargin);	//left margin
	swf_SetU16(tag, layout->rightmargin);	//right margin
	swf_SetU16(tag, layout->indent);	//indent
	swf_SetU16(tag, layout->leading);	//leading
    }
    swf_SetString(tag, (U8*)variable);
    if (flags & ET_HASTEXT)
	swf_SetString(tag, (U8*)text);
}

SRECT swf_SetDefineText(TAG * tag, SWFFONT * font, RGBA * rgb, char *text, int scale)
{
    SRECT r;
    U8 gbits, abits;
    U8 *utext = (U8 *) strdup(text);
    U8 *upos = utext;
    int x = 0, y = 0;
    int pos = 0;
    int ystep = 0;
    if (font->layout) {
	r = swf_TextCalculateBBoxUTF8(font, (U8*)text, scale * 20);
	ystep = font->layout->leading;
    } else {
	fprintf(stderr, "No layout information- can't compute text bbox accurately");
	/* Hm, without layout information, we can't compute a bounding
	   box. We could call swf_FontCreateLayout to create a layout,
	   but the caller probably doesn't want us to mess up his font
	   structure.
	 */
	r.xmin = r.ymin = 0;
	r.xmax = r.ymax = 1024 * 20;
	ystep = 100;
    }

    swf_SetRect(tag, &r);

    /* The text matrix is pretty boring, as it doesn't apply to
       individual characters, but rather whole text objects (or
       at least whole char records- haven't tested).
       So it can't do anything which we can't already do with
       the placeobject tag we use for placing the text on the scene.
     */
    swf_SetMatrix(tag, 0);

    swf_TextCountBitsUTF8(font, (U8*)text, scale * 20, &gbits, &abits);
    swf_SetU8(tag, gbits);
    swf_SetU8(tag, abits);

    while(*upos) {
	U8*next = upos;
	int count = 0;

	swf_TextSetInfoRecord(tag, font, (scale * 1024) / 100, rgb, x, y);	//scale
	x = 0;

	while(*next && *next!=13 && *next!=10 && count<127) {
	    readUTF8char(&next);
	    count++;
	}
	if(next[0] == 13 || next[0] == 10) {
	    x = SET_TO_ZERO;
	    y += ystep;
	}

	if(next[0] == 13 && next[1] == 10)
	    next++;

	if(next[0] == 13 || next[0] == 10) {
	    *next = 0;
	    next++;
	}

	/* now set the text params- notice that a font size of
	   1024 means that the glyphs will be displayed exactly
	   as they would be in/with a defineshape. (Try to find
	   *that* in the flash specs)
	 */
	/* set the actual text- notice that we just pass our scale
	   parameter over, as TextSetCharRecord calculates with
	   percent, too */
	swf_TextSetCharRecordUTF8(tag, font, upos, scale * 20, gbits, abits);

	upos= next;
    }
    free(utext);

    swf_SetU8(tag, 0);
    return r;
}

void swf_FontCreateLayout(SWFFONT * f)
{
    S16 leading = 0;
    int t;
    if (f->layout)
	return;
    if (!f->numchars)
	return;

    f->layout = (SWFLAYOUT *) rfx_calloc(sizeof(SWFLAYOUT));
    f->layout->bounds = (SRECT *) rfx_alloc(f->numchars * sizeof(SRECT));
    f->layout->ascent = -32767;
    f->layout->descent = -32767;

    for (t = 0; t < f->numchars; t++) {
	SHAPE2 *shape2;
	SRECT bbox;
	int width;
	shape2 = swf_ShapeToShape2(f->glyph[t].shape);
	if (!shape2) {
	    fprintf(stderr, "Shape parse error\n");
	    exit(1);
	}
	bbox = swf_GetShapeBoundingBox(shape2);
	swf_Shape2Free(shape2);
	f->layout->bounds[t] = bbox;

	width = (bbox.xmax);

	/* The following is a heuristic- it may be that extractfont_DefineText
	   has already found out some widths for individual characters (from the way
	   they are used)- we now have to guess whether that width might be possible,
	   which is the case if it isn't either much too big or much too small */
	if (width > f->glyph[t].advance * 3 / 2 || width < f->glyph[t].advance / 2)
	    f->glyph[t].advance = width;

	if (-bbox.ymin > f->layout->ascent)
	    f->layout->ascent = bbox.ymin;
	if (bbox.ymax > f->layout->descent)
	    f->layout->descent = bbox.ymax;
    }
}

void swf_DrawText(drawer_t * draw, SWFFONT * font, int size, char *text)
{
    U8 *s = (U8 *) text;
    int advance = 0;
    while (*s) {
	SHAPE *shape;
	SHAPE2 *shape2;
	SHAPELINE *l;
	U32 c = readUTF8char(&s);
	int g = font->ascii2glyph[c];
	shape = font->glyph[g].shape;
	if (((int) g) < 0) {
	    fprintf(stderr, "No char %d in font %s\n", c, font->name ? (char *) font->name : "?");
	    continue;
	}
	shape2 = swf_ShapeToShape2(shape);
	l = shape2->lines;
	while (l) {
	    if (l->type == moveTo) {
		FPOINT to;
		to.x = l->x * size / 100.0 / 20.0 + advance;
		to.y = l->y * size / 100.0 / 20.0;
		draw->moveTo(draw, &to);
	    } else if (l->type == lineTo) {
		FPOINT to;
		to.x = l->x * size / 100.0 / 20.0 + advance;
		to.y = l->y * size / 100.0 / 20.0;
		draw->lineTo(draw, &to);
	    } else if (l->type == splineTo) {
		FPOINT mid, to;
		mid.x = l->sx * size / 100.0 / 20.0 + advance;
		mid.y = l->sy * size / 100.0 / 20.0;
		to.x = l->x * size / 100.0 / 20.0 + advance;
		to.y = l->y * size / 100.0 / 20.0;
		draw->splineTo(draw, &mid, &to);
	    }
	    l = l->next;
	}
	swf_Shape2Free(shape2);
	advance += font->glyph[g].advance * size / 100.0 / 20.0;
    }
}
