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
#include <zlib.h>
#include <string.h>
#include <assert.h>
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../gfxfont.h"
#include "../types.h"
#include "../bitio.h"
#include "../log.h"
#include "../os.h"
#include "../png.h"
#ifdef HAVE_FASTLZ
#include "../fastlz.h"
#endif
#include "record.h"

//#define STATS
//#define COMPRESS_IMAGES
//#define FILTER_IMAGES

typedef struct _state {
    char*last_string[16];
    gfxcolor_t last_color[16];
    gfxmatrix_t last_matrix[16];

#ifdef STATS
    int size_matrices;
    int size_positions;
    int size_images;
    int size_lines;
    int size_colors;
    int size_fonts;
    int size_chars;
#endif
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
#define FLAG_ZERO_FONT 0x20

#define LINE_MOVETO 0x0e
#define LINE_LINETO 0x0f
#define LINE_SPLINETO 0x10

/* ----------------- reading/writing of low level primitives -------------- */

static void dumpLine(writer_t*w, state_t*state, gfxline_t*line)
{
    while(line) {
	if(line->type == gfx_moveTo) {
	    writer_writeU8(w, LINE_MOVETO);
	    writer_writeDouble(w, line->x);
	    writer_writeDouble(w, line->y);
#ifdef STATS
	    state->size_lines += 1+8+8;
#endif
	} else if(line->type == gfx_lineTo) {
	    writer_writeU8(w, LINE_LINETO);
	    writer_writeDouble(w, line->x);
	    writer_writeDouble(w, line->y);
#ifdef STATS
	    state->size_lines += 1+8+8;
#endif
	} else if(line->type == gfx_splineTo) {
	    writer_writeU8(w, LINE_SPLINETO);
	    writer_writeDouble(w, line->x);
	    writer_writeDouble(w, line->y);
	    writer_writeDouble(w, line->sx);
	    writer_writeDouble(w, line->sy);
#ifdef STATS
	    state->size_lines += 1+8+8+8+8;
#endif
	}
	line = line->next;
    }
    writer_writeU8(w, OP_END);
#ifdef STATS
    state->size_lines += 1;
#endif
}
static gfxline_t* readLine(reader_t*r, state_t*s)
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

static void dumpImage(writer_t*w, state_t*state, gfximage_t*img)
{
    int oldpos = w->pos;
    writer_writeU16(w, img->width);
    writer_writeU16(w, img->height);
#ifdef COMPRESS_IMAGES
    //35.3% images (2027305 bytes) (with filter, Z_BEST_COMPRESSION)
    //39.9% images (2458904 bytes) (with filter, Z_BEST_SPEED)
    //45.2% images (3055340 bytes) (without filter)
    //45.9% images (3149247 bytes) (without filter, 5)
    //48.0% images (3480495 bytes) (with filter, fastlz)
    //48.0% images (3488650 bytes) (without filter, Z_BEST_SPEED)
    //55.3% images (4665889 bytes) (without filter, fastlz level 2)
    //55.6% images (4726334 bytes) (without filter, fastlz level 1)
    //83.0% images (18091804 bytes) (no compression)

    gfxcolor_t*image;
#ifdef FILTER_IMAGES
    unsigned char*filter = malloc(img->height);
    int y;
    image = malloc(img->width*img->height*sizeof(gfxcolor_t));
    for(y=0;y<img->height;y++) {
	filter[y] = png_apply_filter_32(
		(void*)&image[y*img->width], 
		(void*)&img->data[y*img->width], img->width, y);
    }
#else
    image = img->data;
#endif
    int size = img->width*img->height;
    uLongf compressdata_size = compressBound(size*sizeof(gfxcolor_t));
    void*compressdata = malloc(compressdata_size);

#ifdef HAVE_FASTLZ
    compressdata_size = fastlz_compress_level(2, (void*)image, size*sizeof(gfxcolor_t), compressdata);
#else
    compress2(compressdata, &compressdata_size, (void*)image, sizeof(gfxcolor_t)*size, Z_BEST_SPEED);
#endif

    writer_writeU32(w, compressdata_size);
#ifdef FILTER_IMAGES
    w->write(w, filter, img->height);
    free(filter);
    free(image);
#endif
    w->write(w, compressdata, compressdata_size);
    free(compressdata);
#else
    w->write(w, img->data, img->width*img->height*sizeof(gfxcolor_t));
#endif
#ifdef STATS
    state->size_images += w->pos - oldpos;
#endif
}
static gfximage_t readImage(reader_t*r, state_t*state)
{
    gfximage_t img;
    img.width = reader_readU16(r);
    img.height = reader_readU16(r);
    uLongf size = img.width*img.height*sizeof(gfxcolor_t);
    img.data = malloc(size);
#ifdef COMPRESS_IMAGES
    uLongf compressdata_size = reader_readU32(r);
    void*compressdata = malloc(compressdata_size);
# ifdef FILTER_IMAGES
    unsigned char*filter = malloc(img.height);
    r->read(r, filter, img.height);
# endif
    r->read(r, compressdata, compressdata_size);
   
# ifdef HAVE_FASTLZ
    fastlz_decompress(compressdata, compressdata_size, (void*)img.data, size);
# else
    uncompress((void*)img.data, &size, compressdata, compressdata_size);
# endif
    free(compressdata);

# ifdef FILTER_IMAGES
    int y;
    unsigned char*line = malloc(img.width*sizeof(gfxcolor_t));
    for(y=0;y<img.height;y++) {
	png_inverse_filter_32(filter[y], (void*)&img.data[y*img.width], 
			      y?(void*)&img.data[(y-1)*img.width]:(void*)0, 
			      line, img.width);
	memcpy(&img.data[y*img.width], line, img.width*sizeof(gfxcolor_t));
    }
    free(line);
    free(filter);
# endif

#else
    r->read(r, img.data, size);
#endif
    return img;
}

static void dumpMatrix(writer_t*w, state_t*state, gfxmatrix_t*matrix)
{
    writer_writeDouble(w, matrix->m00);
    writer_writeDouble(w, matrix->m01);
    writer_writeDouble(w, matrix->m10);
    writer_writeDouble(w, matrix->m11);
    writer_writeDouble(w, matrix->tx);
    writer_writeDouble(w, matrix->ty);
#ifdef STATS
    state->size_matrices += 6*8;
#endif
}
static gfxmatrix_t readMatrix(reader_t*r, state_t*state)
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
static void dumpXY(writer_t*w, state_t*state, gfxmatrix_t*matrix)
{
    writer_writeDouble(w, matrix->tx);
    writer_writeDouble(w, matrix->ty);
#ifdef STATS
    state->size_positions += 2*8;
#endif
}
static void readXY(reader_t*r, state_t*state, gfxmatrix_t*m)
{
    m->tx = reader_readDouble(r);
    m->ty = reader_readDouble(r);
}

static void dumpColor(writer_t*w, state_t*state, gfxcolor_t*color)
{
    writer_writeU8(w, color->r);
    writer_writeU8(w, color->g);
    writer_writeU8(w, color->b);
    writer_writeU8(w, color->a);
#ifdef STATS
    state->size_colors += 4;
#endif
}
static gfxcolor_t readColor(reader_t*r, state_t*state)
{
    gfxcolor_t col;
    col.r = reader_readU8(r);
    col.g = reader_readU8(r);
    col.b = reader_readU8(r);
    col.a = reader_readU8(r);
    return col;
}

static void dumpGradient(writer_t*w, state_t*state, gfxgradient_t*gradient)
{
    while(gradient) {
	writer_writeU8(w, 1);
	dumpColor(w, state, &gradient->color);
	writer_writeFloat(w, gradient->pos);
	gradient = gradient->next;
    }
    writer_writeU8(w, 0);
}
static gfxgradient_t* readGradient(reader_t*r, state_t*state)
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
	g->color = readColor(r, state);
	g->pos = reader_readFloat(r);
    }
    return start;
}

static void dumpCXForm(writer_t*w, state_t*state, gfxcxform_t*c)
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
static gfxcxform_t* readCXForm(reader_t*r, state_t*state)
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

static void dumpFont(writer_t*w, state_t*state, gfxfont_t*font)
{
    int oldpos = w->pos;
#ifdef STATS
    int old_size_lines = state->size_lines;
#endif
    writer_writeString(w, font->id);
    writer_writeU32(w, font->num_glyphs);
    writer_writeU32(w, font->max_unicode);
    writer_writeDouble(w, font->ascent);
    writer_writeDouble(w, font->descent);
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	dumpLine(w, state, font->glyphs[t].line);
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
#ifdef STATS
    state->size_lines = old_size_lines;
    state->size_fonts += w->pos - oldpos;
#endif
}
static gfxfont_t*readFont(reader_t*r, state_t*state)
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
	font->glyphs[t].line = readLine(r, state);
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
    return font;
}

/* ----------------- reading/writing of primitives with caching -------------- */

void state_clear(state_t*state)
{
    int t;
    for(t=0;t<sizeof(state->last_string)/sizeof(state->last_string[0]);t++) {
	if(state->last_string[t]) {
	    free(state->last_string[t]);
	    state->last_string[t] = 0;
	}
    }
}

static char* read_string(reader_t*r, state_t*state, U8 id, U8 flags)
{
    assert(id>=0 && id<16);
    if(flags&FLAG_SAME_AS_LAST) {
	assert(state->last_string[id]);
	return strdup(state->last_string[id]);
    }
    char*s = reader_readString(r);
    if(state->last_string[id]) {
	free(state->last_string[id]);
    }
    state->last_string[id] = strdup(s);
    return s;
}
static gfxcolor_t read_color(reader_t*r, state_t*state, U8 id, U8 flags)
{
    assert(id>=0 && id<16);
    if(flags&FLAG_SAME_AS_LAST)
	return state->last_color[id];
    gfxcolor_t c = readColor(r, state);
    state->last_color[id] = c;
    return c;
}
static gfxmatrix_t read_matrix(reader_t*r, state_t*state, U8 id, U8 flags)
{
    assert(id>=0 && id<16);
    if(flags&FLAG_SAME_AS_LAST) {
	gfxmatrix_t m = state->last_matrix[id];
	readXY(r, state, &m);
	return m;
    }
    gfxmatrix_t m = readMatrix(r, state);
    state->last_matrix[id] = m;
    return m;
}

/* --------------------------- record device operations ---------------------- */

static int record_setparameter(struct _gfxdevice*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x SETPARAM %s %s\n", dev, key, value);
    writer_writeU8(&i->w, OP_SETPARAM);
    writer_writeString(&i->w, key);
    writer_writeString(&i->w, value);
    return 1;
}

static void record_stroke(struct _gfxdevice*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x STROKE\n", dev);
    writer_writeU8(&i->w, OP_STROKE);
    writer_writeDouble(&i->w, width);
    writer_writeDouble(&i->w, miterLimit);
    dumpColor(&i->w, &i->state, color);
    writer_writeU8(&i->w, cap_style);
    writer_writeU8(&i->w, joint_style);
    dumpLine(&i->w, &i->state, line);
}

static void record_startclip(struct _gfxdevice*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x STARTCLIP\n", dev);
    writer_writeU8(&i->w, OP_STARTCLIP);
    dumpLine(&i->w, &i->state, line);
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
    dumpColor(&i->w, &i->state, color);
    dumpLine(&i->w, &i->state, line);
}

static void record_fillbitmap(struct _gfxdevice*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x FILLBITMAP\n", dev);
    writer_writeU8(&i->w, OP_FILLBITMAP);
    dumpImage(&i->w, &i->state, img);
    dumpMatrix(&i->w, &i->state, matrix);
    dumpLine(&i->w, &i->state, line);
    dumpCXForm(&i->w, &i->state, cxform);
}

static void record_fillgradient(struct _gfxdevice*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x FILLGRADIENT %08x\n", dev, gradient);
    writer_writeU8(&i->w, OP_FILLGRADIENT);
    writer_writeU8(&i->w, type);
    dumpGradient(&i->w, &i->state, gradient);
    dumpMatrix(&i->w, &i->state, matrix);
    dumpLine(&i->w, &i->state, line);
}

static void record_addfont(struct _gfxdevice*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x ADDFONT %s\n", dev, font->id);
    if(font && !gfxfontlist_hasfont(i->fontlist, font)) {
	writer_writeU8(&i->w, OP_ADDFONT);
	dumpFont(&i->w, &i->state, font);
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
    const char*font_id = (font&&font->id)?font->id:"*NULL*";
    
    gfxmatrix_t*l = &i->state.last_matrix[OP_DRAWCHAR];

    U8 flags = 0;
    if(!font)
	flags |= FLAG_ZERO_FONT;

    char same_font = i->state.last_string[OP_DRAWCHAR] && !strcmp(i->state.last_string[OP_DRAWCHAR], font_id);
    char same_matrix = (l->m00 == matrix->m00) && (l->m01 == matrix->m01) && (l->m10 == matrix->m10) && (l->m11 == matrix->m11);
    char same_color = !memcmp(color, &i->state.last_color[OP_DRAWCHAR], sizeof(gfxcolor_t));

    /* FIXME
    if(same_font && same_matrix && same_color)
	flags |= FLAG_SAME_AS_LAST;
    */

    writer_writeU8(&i->w, OP_DRAWCHAR|flags);
    writer_writeU32(&i->w, glyphnr);
#ifdef STATS
    i->state.size_chars += 5;
#endif

    if(!(flags&FLAG_SAME_AS_LAST)) {
	if(!(flags&FLAG_ZERO_FONT))
	    writer_writeString(&i->w, font_id);
	dumpColor(&i->w, &i->state, color);
	dumpMatrix(&i->w, &i->state, matrix);

	if(i->state.last_string[OP_DRAWCHAR])
	    free(i->state.last_string[OP_DRAWCHAR]);
	i->state.last_string[OP_DRAWCHAR] = strdup(font_id);

	i->state.last_color[OP_DRAWCHAR] = *color;
	i->state.last_matrix[OP_DRAWCHAR] = *matrix;
    } else {
	dumpXY(&i->w, &i->state, matrix);
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

static void record_drawlink(struct _gfxdevice*dev, gfxline_t*line, const char*action, const char*text)
{
    internal_t*i = (internal_t*)dev->internal;
    msg("<trace> record: %08x DRAWLINK\n", dev);
    writer_writeU8(&i->w, OP_DRAWLINK);
    dumpLine(&i->w, &i->state, line);
    writer_writeString(&i->w, action?action:"");
    writer_writeString(&i->w, text?text:"");
}

/* ------------------------------- replaying --------------------------------- */

static void replay(struct _gfxdevice*dev, gfxdevice_t*out, reader_t*r, gfxfontlist_t**fontlist)
{
    internal_t*i = 0;
    if(dev) {
	i = (internal_t*)dev->internal;
    }
    gfxfontlist_t*_fontlist=0;
    if(!fontlist) {
	fontlist = &_fontlist;
    }

    state_t state;
    memset(&state, 0, sizeof(state));

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
		gfxcolor_t color = readColor(r, &state);
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
		gfxline_t* line = readLine(r, &state);
		out->stroke(out, line, width, &color, captype, jointtype,miterlimit);
		gfxline_free(line);
		break;
	    }
	    case OP_STARTCLIP: {
		msg("<trace> replay: STARTCLIP");
		gfxline_t* line = readLine(r, &state);
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
		gfxcolor_t color = readColor(r, &state);
		gfxline_t* line = readLine(r, &state);
		out->fill(out, line, &color);
		gfxline_free(line);
		break;
	    }
	    case OP_FILLBITMAP: {
		msg("<trace> replay: FILLBITMAP");
		gfximage_t img = readImage(r, &state);
		gfxmatrix_t matrix = readMatrix(r, &state);
		gfxline_t* line = readLine(r, &state);
		gfxcxform_t* cxform = readCXForm(r, &state);
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
		gfxgradient_t*gradient = readGradient(r, &state);
		gfxmatrix_t matrix = readMatrix(r, &state);
		gfxline_t* line = readLine(r, &state);
		out->fillgradient(out, line, gradient, type, &matrix);
		break;
	    }
	    case OP_DRAWLINK: {
		msg("<trace> replay: DRAWLINK");
		gfxline_t* line = readLine(r, &state);
		char* s = reader_readString(r);
		char* t = reader_readString(r);
		out->drawlink(out,line,s, t);
		gfxline_free(line);
		free(s);
		break;
	    }
	    case OP_ADDFONT: {
		msg("<trace> replay: ADDFONT out=%08x(%s)", out, out->name);
		gfxfont_t*font = readFont(r, &state);
		if(!gfxfontlist_hasfont(*fontlist, font)) {
		    *fontlist = gfxfontlist_addfont(*fontlist, font);
		    out->addfont(out, font);
		} else {
		    gfxfont_free(font);
		}
		break;
	    }
	    case OP_DRAWCHAR: {
		U32 glyph = reader_readU32(r);
		gfxmatrix_t m = {1,0,0, 0,1,0};
		char* id = 0;
		if(!(flags&FLAG_ZERO_FONT))
		    id = read_string(r, &state, op, flags);
		gfxcolor_t color = read_color(r, &state, op, flags);
		gfxmatrix_t matrix = read_matrix(r, &state, op, flags);

		gfxfont_t*font = id?gfxfontlist_findfont(*fontlist, id):0;
		if(i && !font) {
		    font = gfxfontlist_findfont(i->fontlist, id);
		}
		msg("<trace> replay: DRAWCHAR font=%s glyph=%d (flags=%d)", id, glyph, flags);
		out->drawchar(out, font, glyph, &color, &matrix);
		if(id)
		    free(id);
		break;
	    }
	}
    }
finish:
    state_clear(&state);
    r->dealloc(r);
    if(_fontlist)
	gfxfontlist_free(_fontlist, 0);
}
void gfxresult_record_replay(gfxresult_t*result, gfxdevice_t*device, gfxfontlist_t**fontlist)
{
    internal_result_t*i = (internal_result_t*)result->internal;
    
    reader_t r;
    if(i->use_tempfile) {
	reader_init_filereader2(&r, i->filename);
    } else {
	reader_init_memreader(&r, i->data, i->length);
    }

    replay(0, device, &r, fontlist);
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

void gfxdevice_record_flush(gfxdevice_t*dev, gfxdevice_t*out, gfxfontlist_t**fontlist)
{
    internal_t*i = (internal_t*)dev->internal;
    if(out) {
	if(!i->use_tempfile) {
	    int len=0;
	    void*data = writer_growmemwrite_memptr(&i->w, &len);
	    reader_t r;
	    reader_init_memreader(&r, data, len);
	    replay(dev, out, &r, fontlist);
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

    state_clear(&i->state);

#ifdef STATS
    int total = i->w.pos;
    if(total && i->use_tempfile) {
	state_t*s = &i->state;
	msg("<notice> record device finished. stats:");
	msg("<notice> %4.1f%% matrices (%d bytes)", s->size_matrices*100.0/total, s->size_matrices);
	msg("<notice> %4.1f%% positions (%d bytes)", s->size_positions*100.0/total, s->size_positions);
	msg("<notice> %4.1f%% colors (%d bytes)", s->size_colors*100.0/total, s->size_colors);
	msg("<notice> %4.1f%% lines (%d bytes)", s->size_lines*100.0/total, s->size_lines);
	msg("<notice> %4.1f%% fonts (%d bytes)", s->size_fonts*100.0/total, s->size_fonts);
	msg("<notice> %4.1f%% images (%d bytes)", s->size_images*100.0/total, s->size_images);
	msg("<notice> %4.1f%% characters (%d bytes)", s->size_chars*100.0/total, s->size_chars);
	msg("<notice> total: %d bytes", total);
    }
#endif
    
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

