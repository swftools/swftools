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
#include <unistd.h>
#include <memory.h>
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../types.h"
#include "../bitio.h"

typedef struct _internal {
    gfxfontlist_t* fontlist;
    writer_t w;
} internal_t;

typedef struct _internal_result {
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

#define OP_MOVETO 0x0e
#define OP_LINETO 0x0f
#define OP_SPLINETO 0x10

int record_setparameter(struct _gfxdevice*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_SETPARAM);
    writer_writeString(&i->w, key);
    writer_writeString(&i->w, value);
    return 1;
}

void dumpLine(writer_t*w, gfxline_t*line)
{
    while(line) {
	if(line->type == gfx_moveTo) {
	    writer_writeU8(w, OP_MOVETO);
	    writer_writeDouble(w, line->x);
	    writer_writeDouble(w, line->y);
	} else if(line->type == gfx_lineTo) {
	    writer_writeU8(w, OP_LINETO);
	    writer_writeDouble(w, line->x);
	    writer_writeDouble(w, line->y);
	} else if(line->type == gfx_splineTo) {
	    writer_writeU8(w, OP_SPLINETO);
	    writer_writeDouble(w, line->x);
	    writer_writeDouble(w, line->y);
	    writer_writeDouble(w, line->sx);
	    writer_writeDouble(w, line->sy);
	}
	line = line->next;
    }
    writer_writeU8(w, OP_END);
}
gfxline_t* readLine(reader_t*r)
{
    gfxline_t*start = 0, *pos = 0;
    while(1) {
	unsigned char op = reader_readU8(r);
	if(op == OP_END)
	    break;
	gfxline_t*line = rfx_calloc(sizeof(gfxline_t));
	if(!start) {
	    start = pos = line;
	} else {
	    pos->next = line;
	    pos = line;
	}
	if(op == OP_MOVETO) {
	    line->type = gfx_moveTo;
	    line->x = reader_readDouble(r);
	    line->y = reader_readDouble(r);
	} else if(op == OP_LINETO) {
	    line->type = gfx_lineTo;
	    line->x = reader_readDouble(r);
	    line->y = reader_readDouble(r);
	} else if(op == OP_SPLINETO) {
	    line->type = gfx_splineTo;
	    line->x = reader_readDouble(r);
	    line->y = reader_readDouble(r);
	    line->sx = reader_readDouble(r);
	    line->sy = reader_readDouble(r);
	}
    }
    return start;
}
gfximage_t readImage(reader_t*r)
{
    gfximage_t img;
    img.width = reader_readU16(r);
    img.height = reader_readU16(r);
    img.data = rfx_alloc(img.width*img.height*4);
    r->read(r, img.data, img.width*img.height*4);
    return img;
}
gfxmatrix_t readMatrix(reader_t*r)
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
gfxcolor_t readColor(reader_t*r)
{
    gfxcolor_t col;
    col.r = reader_readU8(r);
    col.g = reader_readU8(r);
    col.b = reader_readU8(r);
    col.a = reader_readU8(r);
    return col;
}
gfxgradient_t* readGradient(reader_t*r)
{
    gfxgradient_t*start = 0, *pos = 0;
    while(1) {
	U8 op = reader_readU8(r);
	if(!op)
	    break;
	gfxgradient_t*g = rfx_calloc(sizeof(gfxgradient_t));
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
gfxcxform_t* readCXForm(reader_t*r)
{
    U8 type = reader_readU8(r);
    if(!type)
	return 0;
    gfxcxform_t* c = rfx_calloc(sizeof(gfxcxform_t));
    c->rr = reader_readFloat(r); c->rg = reader_readFloat(r); c->rb = reader_readFloat(r); c->ra = reader_readFloat(r);
    c->gr = reader_readFloat(r); c->gg = reader_readFloat(r); c->gb = reader_readFloat(r); c->ga = reader_readFloat(r);
    c->br = reader_readFloat(r); c->bg = reader_readFloat(r); c->bb = reader_readFloat(r); c->ba = reader_readFloat(r);
    c->ar = reader_readFloat(r); c->ag = reader_readFloat(r); c->ab = reader_readFloat(r); c->aa = reader_readFloat(r);
    return c;
}

void dumpColor(writer_t*w, gfxcolor_t*color)
{
    writer_writeU8(w, color->r);
    writer_writeU8(w, color->g);
    writer_writeU8(w, color->b);
    writer_writeU8(w, color->a);
}
void dumpMatrix(writer_t*w, gfxmatrix_t*matrix)
{
    writer_writeDouble(w, matrix->m00);
    writer_writeDouble(w, matrix->m01);
    writer_writeDouble(w, matrix->m10);
    writer_writeDouble(w, matrix->m11);
    writer_writeDouble(w, matrix->tx);
    writer_writeDouble(w, matrix->ty);
}
void dumpGradient(writer_t*w, gfxgradient_t*gradient)
{
    while(gradient) {
	writer_writeU8(w, 1);
	dumpColor(w, &gradient->color);
	writer_writeFloat(w, gradient->pos);
	gradient = gradient->next;
    }
    writer_writeU8(w, 0);
}
void dumpImage(writer_t*w, gfximage_t*image)
{
    writer_writeU16(w, image->width);
    writer_writeU16(w, image->height);
    w->write(w, image->data, image->width*image->height*4);
}
void dumpCXForm(writer_t*w, gfxcxform_t*c)
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
void dumpFont(writer_t*w, gfxfont_t*font)
{
    writer_writeString(w, font->id);
    writer_writeU32(w, font->num_glyphs);
    writer_writeU32(w, font->max_unicode);
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
}
gfxfont_t*readFont(reader_t*r)
{
    gfxfont_t* font = rfx_calloc(sizeof(gfxfont_t));
    font->id = reader_readString(r);
    font->num_glyphs = reader_readU32(r);
    font->max_unicode = reader_readU32(r);
    font->glyphs = rfx_calloc(sizeof(gfxglyph_t)*font->num_glyphs);
    font->unicode2glyph = rfx_calloc(sizeof(font->unicode2glyph[0])*font->max_unicode);
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	font->glyphs[t].line = readLine(r);
	font->glyphs[t].advance = reader_readDouble(r);
	font->glyphs[t].unicode = reader_readU32(r);
	font->glyphs[t].name = reader_readString(r);
	if(!font->glyphs[t].name[0]) {
	    free(font->glyphs[t].name);
	    font->glyphs[t].name = 0;
	}
    }
    for(t=0;t<font->max_unicode;t++) {
	font->unicode2glyph[t] = reader_readU32(r);
    }
    return font;
}

void record_stroke(struct _gfxdevice*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_STROKE);
    writer_writeDouble(&i->w, width);
    writer_writeDouble(&i->w, miterLimit);
    dumpColor(&i->w, color);
    writer_writeU8(&i->w, cap_style);
    writer_writeU8(&i->w, joint_style);
    dumpLine(&i->w, line);
}

void record_startclip(struct _gfxdevice*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_STARTCLIP);
    dumpLine(&i->w, line);
}

void record_endclip(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_ENDCLIP);
}

void record_fill(struct _gfxdevice*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_FILL);
    dumpColor(&i->w, color);
    dumpLine(&i->w, line);
}

void record_fillbitmap(struct _gfxdevice*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_FILLBITMAP);
    dumpImage(&i->w, img);
    dumpMatrix(&i->w, matrix);
    dumpLine(&i->w, line);
    dumpCXForm(&i->w, cxform);
}

void record_fillgradient(struct _gfxdevice*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_FILLGRADIENT);
    writer_writeU8(&i->w, type);
    dumpGradient(&i->w, gradient);
    dumpMatrix(&i->w, matrix);
    dumpLine(&i->w, line);
}

void record_addfont(struct _gfxdevice*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_ADDFONT);
    dumpFont(&i->w, font);
    i->fontlist = gfxfontlist_addfont(i->fontlist, font);
}

void record_drawchar(struct _gfxdevice*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!gfxfontlist_hasfont(i->fontlist, font))
	record_addfont(dev, font);

    writer_writeU8(&i->w, OP_DRAWCHAR);
    writer_writeString(&i->w, font->id);
    writer_writeU32(&i->w, glyphnr);
    dumpColor(&i->w, color);
    dumpMatrix(&i->w, matrix);
}

void record_startpage(struct _gfxdevice*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_STARTPAGE);
    writer_writeU16(&i->w, width);
    writer_writeU16(&i->w, height);
}

void record_endpage(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_ENDPAGE);
}

void record_drawlink(struct _gfxdevice*dev, gfxline_t*line, char*action)
{
    internal_t*i = (internal_t*)dev->internal;
    writer_writeU8(&i->w, OP_DRAWLINK);
    dumpLine(&i->w, line);
    writer_writeString(&i->w, action);
}

void gfxresult_record_replay(gfxresult_t*result, gfxdevice_t*device)
{
    internal_result_t*i = (internal_result_t*)result->internal;
    reader_t r2;
    reader_t*r = &r2;
    reader_init_memreader(r, i->data, i->length);
    gfxfontlist_t* fontlist = gfxfontlist_create();

    while(1) {
	unsigned char op = reader_readU8(r);
	switch(op) {
	    case OP_END:
		return;
	    case OP_SETPARAM: {
		char*key;
		char*value;
		key = reader_readString(r);
		value = reader_readString(r);
		device->setparameter(device, key, value);
		free(key);
		free(value);
		break;
	    }
	    case OP_STARTPAGE: {
		U16 width = reader_readU16(r);
		U16 height = reader_readU16(r);
		device->startpage(device, width, height);
		break;
	    }
	    case OP_ENDPAGE: {
		break;
	    }
	    case OP_FINISH: {
		break;
	    }
	    case OP_STROKE: {
		double width = reader_readDouble(r);
		double miterlimit = reader_readDouble(r);
		gfxcolor_t color = readColor(r);
		gfx_capType captype = reader_readU8(r);
		gfx_joinType jointtype = reader_readU8(r);
		gfxline_t* line = readLine(r);
		device->stroke(device, line, width, &color, captype, jointtype,miterlimit);
		gfxline_free(line);
		break;
	    }
	    case OP_STARTCLIP: {
		gfxline_t* line = readLine(r);
		device->startclip(device, line);
		gfxline_free(line);
		break;
	    }
	    case OP_ENDCLIP: {
		device->endclip(device);
		break;
	    }
	    case OP_FILL: {
		gfxcolor_t color = readColor(r);
		gfxline_t* line = readLine(r);
		device->fill(device, line, &color);
		gfxline_free(line);
		break;
	    }
	    case OP_FILLBITMAP: {
		gfximage_t img = readImage(r);
		gfxmatrix_t matrix = readMatrix(r);
		gfxline_t* line = readLine(r);
		gfxcxform_t* cxform = readCXForm(r);
		device->fillbitmap(device, line, &img, &matrix, cxform);
		if(cxform)
		    free(cxform);
		break;
	    }
	    case OP_FILLGRADIENT: {
		gfxgradienttype_t type = reader_readU8(r);
		gfxgradient_t*gradient = readGradient(r);
		gfxmatrix_t matrix = readMatrix(r);
		gfxline_t* line = readLine(r);
		device->fillgradient(device, line, gradient, type, &matrix);
		break;
	    }
	    case OP_DRAWLINK: {
		gfxline_t* line = readLine(r);
		char* s = reader_readString(r);
		device->drawlink(device,line,s);
		gfxline_free(line);
		free(s);
		break;
	    }
	    case OP_ADDFONT: {
		gfxfont_t*font = readFont(r);
		fontlist = gfxfontlist_addfont(fontlist, font);
		device->addfont(device, font);
		break;
	    }
	    case OP_DRAWCHAR: {
		char* id = reader_readString(r);
		gfxfont_t*font = gfxfontlist_findfont(fontlist, id);
		U32 glyph = reader_readU32(r);
		gfxcolor_t color = readColor(r);
		gfxmatrix_t matrix = readMatrix(r);
		device->drawchar(device, font, glyph, &color, &matrix);
		free(id);
		break;
	    }
	}
    }
}

void record_result_write(gfxresult_t*r, int filedesc)
{
    internal_result_t*i = (internal_result_t*)r->internal;
    write(filedesc, i->data, i->length);
}
int record_result_save(gfxresult_t*r, char*filename)
{
    internal_result_t*i = (internal_result_t*)r->internal;
    FILE*fi = fopen(filename, "wb");
    if(!fi) {
	fprintf(stderr, "Couldn't open file %s for writing\n", filename);
	return -1;
    }
    fwrite(i->data, i->length, 1, fi);
    fclose(fi);
    return 0;
}
void*record_result_get(gfxresult_t*r, char*name)
{
    internal_result_t*i = (internal_result_t*)r->internal;
    if(!strcmp(name, "data")) {
	return i->data;
    } else if(!strcmp(name, "length")) {
	return &i->length;
    }
    return 0;
}
void record_result_destroy(gfxresult_t*r)
{
    internal_result_t*i = (internal_result_t*)r->internal;
    free(i->data);i->data = 0;
    free(r->internal);r->internal = 0;
    free(r);
}


gfxresult_t* record_finish(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    
    writer_writeU8(&i->w, OP_END);
   
    internal_result_t*ir = (internal_result_t*)rfx_calloc(sizeof(gfxresult_t));
    ir->data = writer_growmemwrite_getmem(&i->w);
    ir->length = i->w.pos;

    gfxresult_t*result= (gfxresult_t*)rfx_calloc(sizeof(gfxresult_t));
    result->save = record_result_save;
    result->get = record_result_get;
    result->destroy = record_result_destroy;
    result->internal = ir;

    free(dev->internal);memset(dev, 0, sizeof(gfxdevice_t));
    
    return result;
}
void gfxdevice_record_init(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));
    
    dev->name = "record";

    dev->internal = i;
    
    writer_init_growingmemwriter(&i->w, 1048576);
    i->fontlist = gfxfontlist_create();

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

