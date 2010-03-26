/* gfxdevice_record.cc

   Part of the swftools package.

   Copyright (c) 2005 Matthias Kramm <kramm@quiss.org> 
 
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
#include <math.h>
#include "../../config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <memory.h>
#ifdef HAVE_IO_H
#include <io.h>
#endif
#include <string.h>
#include <assert.h>
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../types.h"
#include "../bitio.h"
#include "../log.h"
#include "../os.h"
#include "record.h"

typedef struct _state {
    char*last_string[16];
    gfxcolor_t last_color[16];
    gfxmatrix_t last_matrix[16];
} state_t;

typedef struct _internal {
    gfxfontlist_t* fontlist;
    state_t state;

    writer_t w;
    int cliplevel;
    char use_tempfile;
    char*filename;
} internal_t;

typedef struct _internal_result {
    char use_tempfile;
    char*filename;
    void*data;
    int length;
} internal_result_t;

#define OP_END 0x00
#define OP_SETPARAM 0x01
#define OP_STROKE 0x02
#define OP_STARTCLIP 0x03
#define OP_ENDCLIP 0x04
#define OP_FILL 0x05
#define OP_FILLBITMAP 0x06
#define OP_FILLGRADIENT 0x07
#define OP_ADDFONT 0x08
#define OP_DRAWCHAR 0x09
#define OP_DRAWLINK 0x0a
#define OP_STARTPAGE 0x0b
#define OP_ENDPAGE 0x0c
#define OP_FINISH 0x0d

#define FLAG_SAME_AS_LAST 0x10

#define LINE_MOVETO 0x0e
#define LINE_LINETO 0x0f
#define LINE_SPLINETO 0x10

static int record_setparameter(struct _gfxdevice*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x SETPARAM %s %s\n", dev, key, value);
    writer_writeU8(&i->w, OP_SETPARAM);
    writer_writeString(&i->w, key);
    writer_writeString(&i->w, value);
    return 1;
}

static void dumpLine(writer_t*w, gfxline_t*line)
{
    while(line) {
	if(line->type == gfx_moveTo) {
	    writer_writeU8(w, LINE_MOVETO);
	    writer_writeDouble(w, line->x);
	    writer_writeDouble(w, line->y);
	} else if(line->type == gfx_lineTo) {
	    writer_writeU8(w, LINE_LINETO);
	    writer_writeDouble(w, line->x);
	    writer_writeDouble(w, line->y);
	} else if(line->type == gfx_splineTo) {
	    writer_writeU8(w, LINE_SPLINETO);
	    writer_writeDouble(w, line->x);
	    writer_writeDouble(w, line->y);
	    writer_writeDouble(w, line->sx);
	    writer_writeDouble(w, line->sy);
	}
	line = line->next;
    }
    writer_writeU8(w, OP_END);
}
static gfxline_t* readLine(reader_t*r)
{
    gfxline_t*start = 0, *pos = 0;
    while(1) {
	unsigned char op = reader_readU8(r);
	if(op == OP_END)
	    break;
	gfxline_t*line = (gfxline_t*)rfx_calloc(sizeof(gfxline_t));
	if(!start) {
	    start = pos = line;
	} else {
	    pos->next = line;
	    pos = line;
	}
	if(op == LINE_MOVETO) {
	    line->type = gfx_moveTo;
	    line->x = reader_readDouble(r);
	    line->y = reader_readDouble(r);
	} else if(op == LINE_LINETO) {
	    line->type = gfx_lineTo;
	    line->x = reader_readDouble(r);
	    line->y = reader_readDouble(r);
	} else if(op == LINE_SPLINETO) {
	    line->type = gfx_splineTo;
	    line->x = reader_readDouble(r);
	    line->y = reader_readDouble(r);
	    line->sx = reader_readDouble(r);
	    line->sy = reader_readDouble(r);
	}
    }
    return start;
}
static gfximage_t readImage(reader_t*r)
{
    gfximage_t img;
    img.width = reader_readU16(r);
    img.height = reader_readU16(r);
    img.data = (gfxcolor_t*)rfx_alloc(img.width*img.height*4);
    r->read(r, img.data, img.width*img.height*4);
    return img;
}
static gfxmatrix_t readMatrix(reader_t*r)
{
    gfxmatrix_t matrix;
    matrix.m00 = reader_readDouble(r);
    matrix.m01 = reader_readDouble(r);
    matrix.m10 = reader_readDouble(r);
    matrix.m11 = reader_readDouble(r);
    matrix.tx = reader_readDouble(r);
    matrix.ty = reader_readDouble(r);
    return matrix;
}
static gfxcolor_t readColor(reader_t*r)
{
    gfxcolor_t col;
    col.r = reader_readU8(r);
    col.g = reader_readU8(r);
    col.b = reader_readU8(r);
    col.a = reader_readU8(r);
    return col;
}
static void readXY(reader_t*r, gfxmatrix_t*m)
{
    m->tx = reader_readDouble(r);
    m->ty = reader_readDouble(r);
}

static gfxgradient_t* readGradient(reader_t*r)
{
    gfxgradient_t*start = 0, *pos = 0;
    while(1) {
	U8 op = reader_readU8(r);
	if(!op)
	    break;
	gfxgradient_t*g = (gfxgradient_t*)rfx_calloc(sizeof(gfxgradient_t));
	if(!start) {
	    start = pos = g;
	} else {
	    pos->next = g;
	    pos = g;
	}
	g->color = readColor(r);
	g->pos = reader_readFloat(r);
    }
    return start;
}
static gfxcxform_t* readCXForm(reader_t*r)
{
    U8 type = reader_readU8(r);
    if(!type)
	return 0;
    gfxcxform_t* c = (gfxcxform_t*)rfx_calloc(sizeof(gfxcxform_t));
    c->rr = reader_readFloat(r); c->rg = reader_readFloat(r); c->rb = reader_readFloat(r); c->ra = reader_readFloat(r);
    c->gr = reader_readFloat(r); c->gg = reader_readFloat(r); c->gb = reader_readFloat(r); c->ga = reader_readFloat(r);
    c->br = reader_readFloat(r); c->bg = reader_readFloat(r); c->bb = reader_readFloat(r); c->ba = reader_readFloat(r);
    c->ar = reader_readFloat(r); c->ag = reader_readFloat(r); c->ab = reader_readFloat(r); c->aa = reader_readFloat(r);
    return c;
}

static void dumpColor(writer_t*w, gfxcolor_t*color)
{
    writer_writeU8(w, color->r);
    writer_writeU8(w, color->g);
    writer_writeU8(w, color->b);
    writer_writeU8(w, color->a);
}
static void dumpMatrix(writer_t*w, gfxmatrix_t*matrix)
{
    writer_writeDouble(w, matrix->m00);
    writer_writeDouble(w, matrix->m01);
    writer_writeDouble(w, matrix->m10);
    writer_writeDouble(w, matrix->m11);
    writer_writeDouble(w, matrix->tx);
    writer_writeDouble(w, matrix->ty);
}
static void dumpXY(writer_t*w, gfxmatrix_t*matrix)
{
    writer_writeDouble(w, matrix->tx);
    writer_writeDouble(w, matrix->ty);
}
static void dumpGradient(writer_t*w, gfxgradient_t*gradient)
{
    while(gradient) {
	writer_writeU8(w, 1);
	dumpColor(w, &gradient->color);
	writer_writeFloat(w, gradient->pos);
	gradient = gradient->next;
    }
    writer_writeU8(w, 0);
}
static void dumpImage(writer_t*w, gfximage_t*image)
{
    writer_writeU16(w, image->width);
    writer_writeU16(w, image->height);
    w->write(w, image->data, image->width*image->height*4);
}
static void dumpCXForm(writer_t*w, gfxcxform_t*c)
{
    if(!c) {
	writer_writeU8(w, 0);
    } else {
	writer_writeU8(w, 1);
	writer_writeFloat(w, c->rr); writer_writeFloat(w, c->rg); writer_writeFloat(w, c->rb); writer_writeFloat(w, c->ra);
	writer_writeFloat(w, c->gr); writer_writeFloat(w, c->gg); writer_writeFloat(w, c->gb); writer_writeFloat(w, c->ga);
	writer_writeFloat(w, c->br); writer_writeFloat(w, c->bg); writer_writeFloat(w, c->bb); writer_writeFloat(w, c->ba);
	writer_writeFloat(w, c->ar); writer_writeFloat(w, c->ag); writer_writeFloat(w, c->ab); writer_writeFloat(w, c->aa);
    }
}
static void dumpFont(writer_t*w, gfxfont_t*font)
{
    writer_writeString(w, font->id);
    writer_writeU32(w, font->num_glyphs);
    writer_writeU32(w, font->max_unicode);
    writer_writeDouble(w, font->ascent);
    writer_writeDouble(w, font->descent);
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	dumpLine(w, font->glyphs[t].line);
	writer_writeDouble(w, font->glyphs[t].advance);
	writer_writeU32(w, font->glyphs[t].unicode);
	if(font->glyphs[t].name) {
	    writer_writeString(w,font->glyphs[t].name);
	} else {
	    writer_writeU8(w,0);
	}
    }
    for(t=0;t<font->max_unicode;t++) {
	writer_writeU32(w, font->unicode2glyph[t]);
    }
    writer_writeU32(w, font->kerning_size);
    for(t=0;t<font->kerning_size;t++) {
	writer_writeU32(w, font->kerning[t].c1);
	writer_writeU32(w, font->kerning[t].c2);
	writer_writeU32(w, font->kerning[t].advance);
    }
}
static gfxfont_t*readFont(reader_t*r)
{
    gfxfont_t* font = (gfxfont_t*)rfx_calloc(sizeof(gfxfont_t));
    font->id = reader_readString(r);
    font->num_glyphs = reader_readU32(r);
    font->max_unicode = reader_readU32(r);
    font->ascent = reader_readDouble(r);
    font->descent = reader_readDouble(r);
    font->glyphs = (gfxglyph_t*)rfx_calloc(sizeof(gfxglyph_t)*font->num_glyphs);
    font->unicode2glyph = (int*)rfx_calloc(sizeof(font->unicode2glyph[0])*font->max_unicode);
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	font->glyphs[t].line = readLine(r);
	font->glyphs[t].advance = reader_readDouble(r);
	font->glyphs[t].unicode = reader_readU32(r);
	font->glyphs[t].name = reader_readString(r);
	if(!font->glyphs[t].name[0]) {
	    free((void*)(font->glyphs[t].name));
	    font->glyphs[t].name = 0;
	}
    }
    for(t=0;t<font->max_unicode;t++) {
	font->unicode2glyph[t] = reader_readU32(r);
    }
    font->kerning_size = reader_readU32(r);
    if(font->kerning_size) {
	font->kerning = malloc(sizeof(gfxkerning_t)*font->kerning_size);
	for(t=0;t<font->kerning_size;t++) {
	    font->kerning[t].c1 = reader_readU32(r);
	    font->kerning[t].c2 = reader_readU32(r);
	    font->kerning[t].advance = reader_readU32(r);
	}
    }
    return font;
}

static void record_stroke(struct _gfxdevice*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x STROKE\n", dev);
    writer_writeU8(&i->w, OP_STROKE);
    writer_writeDouble(&i->w, width);
    writer_writeDouble(&i->w, miterLimit);
    dumpColor(&i->w, color);
    writer_writeU8(&i->w, cap_style);
    writer_writeU8(&i->w, joint_style);
    dumpLine(&i->w, line);
}

static void record_startclip(struct _gfxdevice*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x STARTCLIP\n", dev);
    writer_writeU8(&i->w, OP_STARTCLIP);
    dumpLine(&i->w, line);
    i->cliplevel++;
}

static void record_endclip(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x ENDCLIP\n", dev);
    writer_writeU8(&i->w, OP_ENDCLIP);
    i->cliplevel--;
    if(i->cliplevel<0) {
	msg("<error> record: endclip() without startclip()");
    }
}

static void record_fill(struct _gfxdevice*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x FILL\n", dev);
    writer_writeU8(&i->w, OP_FILL);
    dumpColor(&i->w, color);
    dumpLine(&i->w, line);
}

static void record_fillbitmap(struct _gfxdevice*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x FILLBITMAP\n", dev);
    writer_writeU8(&i->w, OP_FILLBITMAP);
    dumpImage(&i->w, img);
    dumpMatrix(&i->w, matrix);
    dumpLine(&i->w, line);
    dumpCXForm(&i->w, cxform);
}

static void record_fillgradient(struct _gfxdevice*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x FILLGRADIENT %08x\n", dev, gradient);
    writer_writeU8(&i->w, OP_FILLGRADIENT);
    writer_writeU8(&i->w, type);
    dumpGradient(&i->w, gradient);
    dumpMatrix(&i->w, matrix);
    dumpLine(&i->w, line);
}

static void record_addfont(struct _gfxdevice*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x ADDFONT %s\n", dev, font->id);
    if(font && !gfxfontlist_hasfont(i->fontlist, font)) {
	writer_writeU8(&i->w, OP_ADDFONT);
	dumpFont(&i->w, font);
	i->fontlist = gfxfontlist_addfont(i->fontlist, font);
    }
}

static void record_drawchar(struct _gfxdevice*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    if(font && !gfxfontlist_hasfont(i->fontlist, font)) {
	record_addfont(dev, font);
    }

    msg("<trace> record: %08x DRAWCHAR %d\n", glyphnr, dev);
    const char*font_id = font->id?font->id:"*NULL*";
    
    gfxmatrix_t*l = &i->state.last_matrix[OP_DRAWCHAR];

    char same_font = i->state.last_string[OP_DRAWCHAR] && !strcmp(i->state.last_string[OP_DRAWCHAR], font_id);
    char same_matrix = (l->m00 == matrix->m00) && (l->m01 == matrix->m01) && (l->m10 == matrix->m10) && (l->m11 == matrix->m11);
    char same_color = !memcmp(color, &i->state.last_color[OP_DRAWCHAR], sizeof(gfxcolor_t));
    U8 flags = 0;
    if(same_font && same_matrix && same_color)
	flags |= FLAG_SAME_AS_LAST;

    writer_writeU8(&i->w, OP_DRAWCHAR|flags);
    writer_writeU32(&i->w, glyphnr);
    if(!(flags&FLAG_SAME_AS_LAST)) {
	writer_writeString(&i->w, font_id);
	dumpColor(&i->w, color);
	dumpMatrix(&i->w, matrix);
    } else {
	dumpXY(&i->w, matrix);
    }
}

static void record_startpage(struct _gfxdevice*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x STARTPAGE\n", dev);
    writer_writeU8(&i->w, OP_STARTPAGE);
    writer_writeU16(&i->w, width);
    writer_writeU16(&i->w, height);
}

static void record_endpage(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x ENDPAGE\n", dev);
    writer_writeU8(&i->w, OP_ENDPAGE);
}

static void record_drawlink(struct _gfxdevice*dev, gfxline_t*line, const char*action)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x DRAWLINK\n", dev);
    writer_writeU8(&i->w, OP_DRAWLINK);
    dumpLine(&i->w, line);
    writer_writeString(&i->w, action);
}

static char* read_string(reader_t*r, state_t*state, U8 id, U8 flags)
{
    assert(id>=0 && id<16);
    if(flags&FLAG_SAME_AS_LAST) {
	assert(state->last_string[id]);
	return state->last_string[id];
    }
    char*s = reader_readString(r);
    state->last_string[id] = strdup(s);
    return s;
}
static gfxcolor_t read_color(reader_t*r, state_t*s, U8 id, U8 flags)
{
    assert(id>=0 && id<16);
    if(flags&FLAG_SAME_AS_LAST)
	return s->last_color[id];
    gfxcolor_t c = readColor(r);
    s->last_color[id] = c;
    return c;
}
static gfxmatrix_t read_matrix(reader_t*r, state_t*s, U8 id, U8 flags)
{
    assert(id>=0 && id<16);
    if(flags&FLAG_SAME_AS_LAST) {
	gfxmatrix_t m = s->last_matrix[id];
	readXY(r, &m);
	return m;
    }
    gfxmatrix_t m = readMatrix(r);
    s->last_matrix[id] = m;
    return m;
}

static void replay(struct _gfxdevice*dev, gfxdevice_t*out, reader_t*r)
{
    internal_t*i = 0;
    if(dev) {
	i = (internal_t*)dev->internal;
    }

    state_t state;
    memset(&state, 0, sizeof(state));

    gfxfontlist_t* fontlist = gfxfontlist_create();

    while(1) {
	unsigned char op;
	if(r->read(r, &op, 1)!=1)
	    break;
	unsigned char flags = op&0xf0;
	op&=0x0f;

	switch(op) {
	    case OP_END:
		goto finish;
	    case OP_SETPARAM: {
		msg("<trace> replay: SETPARAM");
		char*key;
		char*value;
		key = reader_readString(r);
		value = reader_readString(r);
		out->setparameter(out, key, value);
		free(key);
		free(value);
		break;
	    }
	    case OP_STARTPAGE: {
		msg("<trace> replay: STARTPAGE");
		U16 width = reader_readU16(r);
		U16 height = reader_readU16(r);
		out->startpage(out, width, height);
		break;
	    }
	    case OP_ENDPAGE: {
		msg("<trace> replay: ENDPAGE");
		out->endpage(out);
		break;
	    }
	    case OP_FINISH: {
		msg("<trace> replay: FINISH");
		break;
	    }
	    case OP_STROKE: {
		msg("<trace> replay: STROKE");
		double width = reader_readDouble(r);
		double miterlimit = reader_readDouble(r);
		gfxcolor_t color = readColor(r);
		gfx_capType captype;
		int v = reader_readU8(r);
		switch (v) {
		    case 0: captype = gfx_capButt; break;
		    case 1: captype = gfx_capRound; break;
		    case 2: captype = gfx_capSquare; break;
		}
		gfx_joinType jointtype;
		v = reader_readU8(r);
		switch (v) {
		    case 0: jointtype = gfx_joinMiter; break;
		    case 1: jointtype = gfx_joinRound; break;
		    case 2: jointtype = gfx_joinBevel; break;
		}
		gfxline_t* line = readLine(r);
		out->stroke(out, line, width, &color, captype, jointtype,miterlimit);
		gfxline_free(line);
		break;
	    }
	    case OP_STARTCLIP: {
		msg("<trace> replay: STARTCLIP");
		gfxline_t* line = readLine(r);
		out->startclip(out, line);
		gfxline_free(line);
		break;
	    }
	    case OP_ENDCLIP: {
		msg("<trace> replay: ENDCLIP");
		out->endclip(out);
		break;
	    }
	    case OP_FILL: {
		msg("<trace> replay: FILL");
		gfxcolor_t color = readColor(r);
		gfxline_t* line = readLine(r);
		out->fill(out, line, &color);
		gfxline_free(line);
		break;
	    }
	    case OP_FILLBITMAP: {
		msg("<trace> replay: FILLBITMAP");
		gfximage_t img = readImage(r);
		gfxmatrix_t matrix = readMatrix(r);
		gfxline_t* line = readLine(r);
		gfxcxform_t* cxform = readCXForm(r);
		out->fillbitmap(out, line, &img, &matrix, cxform);
		gfxline_free(line);
		if(cxform)
		    free(cxform);
		free(img.data);img.data=0;
		break;
	    }
	    case OP_FILLGRADIENT: {
		msg("<trace> replay: FILLGRADIENT");
		gfxgradienttype_t type;
		int v = reader_readU8(r);
		switch (v) {
		    case 0: 
		      type = gfxgradient_radial; break;
		    case 1:
		      type = gfxgradient_linear; break;
		}  
		gfxgradient_t*gradient = readGradient(r);
		gfxmatrix_t matrix = readMatrix(r);
		gfxline_t* line = readLine(r);
		out->fillgradient(out, line, gradient, type, &matrix);
		break;
	    }
	    case OP_DRAWLINK: {
		msg("<trace> replay: DRAWLINK");
		gfxline_t* line = readLine(r);
		char* s = reader_readString(r);
		out->drawlink(out,line,s);
		gfxline_free(line);
		free(s);
		break;
	    }
	    case OP_ADDFONT: {
		msg("<trace> replay: ADDFONT out=%08x(%s)", out, out->name);
		gfxfont_t*font = readFont(r);
		fontlist = gfxfontlist_addfont(fontlist, font);
		out->addfont(out, font);
		break;
	    }
	    case OP_DRAWCHAR: {
		U32 glyph = reader_readU32(r);
		gfxmatrix_t m = {1,0,0, 0,1,0};
		char* id = read_string(r, &state, op, flags);
		gfxcolor_t color = read_color(r, &state, op, flags);
		gfxmatrix_t matrix = read_matrix(r, &state, op, flags);

		gfxfont_t*font = id?gfxfontlist_findfont(fontlist, id):0;
		if(i && !font) {
		    font = gfxfontlist_findfont(i->fontlist, id);
		}
		msg("<trace> replay: DRAWCHAR font=%s glyph=%d", id, glyph);
		out->drawchar(out, font, glyph, &color, &matrix);
		free(id);
		break;
	    }
	}
    }
finish:
    r->dealloc(r);
    /* problem: if we just replayed into a device which stores the
       font for later use (the record device itself is a nice example),
       then we can't free it yet */
    //gfxfontlist_free(fontlist, 1);
    gfxfontlist_free(fontlist, 0);
}
void gfxresult_record_replay(gfxresult_t*result, gfxdevice_t*device)
{
    internal_result_t*i = (internal_result_t*)result->internal;
    
    reader_t r;
    if(i->use_tempfile) {
	reader_init_filereader2(&r, i->filename);
    } else {
	reader_init_memreader(&r, i->data, i->length);
    }

    replay(0, device, &r);
}

static void record_result_write(gfxresult_t*r, int filedesc)
{
    internal_result_t*i = (internal_result_t*)r->internal;
    if(i->data) {
	write(filedesc, i->data, i->length);
    }
}
static int record_result_save(gfxresult_t*r, const char*filename)
{
    internal_result_t*i = (internal_result_t*)r->internal;
    if(i->use_tempfile) {
	move_file(i->filename, filename);
    } else {
	FILE*fi = fopen(filename, "wb");
	if(!fi) {
	    fprintf(stderr, "Couldn't open file %s for writing\n", filename);
	    return -1;
	}
	fwrite(i->data, i->length, 1, fi);
	fclose(fi);
    }
    return 0;
}
static void*record_result_get(gfxresult_t*r, const char*name)
{
    internal_result_t*i = (internal_result_t*)r->internal;
    if(!strcmp(name, "data")) {
	return i->data;
    } else if(!strcmp(name, "length")) {
	return &i->length;
    }
    return 0;
}
static void record_result_destroy(gfxresult_t*r)
{
    internal_result_t*i = (internal_result_t*)r->internal;
    if(i->data) {
	free(i->data);i->data = 0;
    }
    if(i->filename) {
	unlink(i->filename);
	free(i->filename);
    }
    free(r->internal);r->internal = 0;
    free(r);
}

static unsigned char printable(unsigned char a)
{
    if(a<32 || a==127) return '.';
    else return a;
}

static void hexdumpMem(unsigned char*data, int len)
{
    int t;
    char ascii[32];
    for(t=0;t<len;t++) {
	printf("%02x ", data[t]);
	ascii[t&15] = printable(data[t]);
	if((t && ((t&15)==15)) || (t==len-1))
	{
	    int s,p=((t)&15)+1;
	    ascii[p] = 0;
	    for(s=p-1;s<16;s++) {
		printf("   ");
	    }
	    printf(" %s\n", ascii);
	}
    }
}

void gfxdevice_record_flush(gfxdevice_t*dev, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)dev->internal;
    if(out) {
	if(!i->use_tempfile) {
	    int len=0;
	    void*data = writer_growmemwrite_memptr(&i->w, &len);
	    reader_t r;
	    reader_init_memreader(&r, data, len);
	    replay(dev, out, &r);
	    writer_growmemwrite_reset(&i->w);
	} else {
	    msg("<fatal> Flushing not supported for file based record device");
	    exit(1);
	}
    }
}

static gfxresult_t* record_finish(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x END", dev);

    if(i->cliplevel) {
	msg("<error> Warning: unclosed cliplevels");
    }
    
    writer_writeU8(&i->w, OP_END);
    
    gfxfontlist_free(i->fontlist, 0);
   
    internal_result_t*ir = (internal_result_t*)rfx_calloc(sizeof(gfxresult_t));
   
    ir->use_tempfile = i->use_tempfile;
    if(i->use_tempfile) {
	ir->filename = i->filename;
    } else {
	ir->data = writer_growmemwrite_getmem(&i->w);
	ir->length = i->w.pos;
    }
    i->w.finish(&i->w);

    gfxresult_t*result= (gfxresult_t*)rfx_calloc(sizeof(gfxresult_t));
    result->save = record_result_save;
    result->get = record_result_get;
    result->destroy = record_result_destroy;
    result->internal = ir;

    free(dev->internal);memset(dev, 0, sizeof(gfxdevice_t));
    
    return result;
}

void gfxdevice_record_init(gfxdevice_t*dev, char use_tempfile)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));
    
    dev->name = "record";

    dev->internal = i;
  
    i->use_tempfile = use_tempfile;
    if(!use_tempfile) {
	writer_init_growingmemwriter(&i->w, 1048576);
    } else {
	char buffer[128];
	i->filename = strdup(mktempname(buffer, "gfx"));
	writer_init_filewriter2(&i->w, i->filename);
    }
    i->fontlist = gfxfontlist_create();
    i->cliplevel = 0;

    dev->setparameter = record_setparameter;
    dev->startpage = record_startpage;
    dev->startclip = record_startclip;
    dev->endclip = record_endclip;
    dev->stroke = record_stroke;
    dev->fill = record_fill;
    dev->fillbitmap = record_fillbitmap;
    dev->fillgradient = record_fillgradient;
    dev->addfont = record_addfont;
    dev->drawchar = record_drawchar;
    dev->drawlink = record_drawlink;
    dev->endpage = record_endpage;
    dev->finish = record_finish;
}

