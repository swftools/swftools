/* text.c

   Part of the swftools package.

   Copyright (c) 2006 Matthias Kramm <kramm@quiss.org> 
 
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
#include <stdarg.h>
#include <unistd.h>
#include <memory.h>
#include "../types.h"
#include "../mem.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../utf8.h"

typedef struct _textpage {
    char*text;
    int textsize;
    int textpos;
    struct _textpage*next;
} textpage_t;

typedef struct _internal {
    textpage_t*first_page;
    textpage_t*current_page;
    double currentx;
    double currenty;
    double lastadvance;
} internal_t;

int text_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    return 0;
}
void text_startpage(gfxdevice_t*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!i->first_page) {
	i->first_page = i->current_page = (textpage_t*)malloc(sizeof(textpage_t));
    } else {
	i->current_page->next = (textpage_t*)malloc(sizeof(textpage_t));
	i->current_page = i->current_page->next;
    }
    i->current_page->textsize = 4096;
    i->current_page->text = (char*)malloc(i->current_page->textsize);
    i->current_page->textpos = 0;
    i->current_page->next = 0;
    i->currentx = 0;
    i->currenty = 0;
    i->lastadvance = 0;
}
void text_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
}
void text_endclip(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
}
void text_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
}
void text_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
}
void text_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
}
void text_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
}
void text_addfont(gfxdevice_t*dev, gfxfont_t*font) {}

static void addchar(gfxdevice_t*dev, int unicode)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!i->current_page) {
        text_startpage(dev, 0, 0);
    }
    if(i->current_page->textpos + 10 > i->current_page->textsize) {
	i->current_page->textsize += 4096;
	i->current_page->text = realloc(i->current_page->text, i->current_page->textsize);
    }
    writeUTF8(unicode, &i->current_page->text[i->current_page->textpos]);
    i->current_page->textpos += strlen(&i->current_page->text[i->current_page->textpos]);
}

void text_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    double xshift = matrix->tx - i->currentx;
    double yshift = matrix->ty - i->currenty;
    i->currentx = matrix->tx;
    i->currenty = matrix->ty;

    if(fabs(yshift)>1.0) {
        addchar(dev, 10);
    } else if(xshift > i->lastadvance*1.3 || xshift<0) {
        addchar(dev, 32);
    }
    int u;
    if(font) {
	i->lastadvance = font->glyphs[glyphnr].advance*matrix->m00;
	u = font->glyphs[glyphnr].unicode;
    } else {
	u = glyphnr;
	i->currentx = 0;i->currenty = 0;
    }
    if(u>13) {
        addchar(dev, u);
    }
}

void text_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*drawlink)
{
    internal_t*i = (internal_t*)dev->internal;
}

void text_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
}

void text_result_write(gfxresult_t*r, int filedesc)
{
    textpage_t*i= (textpage_t*)r->internal;
}
int text_result_save(gfxresult_t*r, const char*filename)
{
    textpage_t*i= (textpage_t*)r->internal;
    if(!i) {
	return 0; // no pages drawn
    }
    FILE*fi = fopen(filename, "wb");
    if(!fi)
	return 0;
    while(i) {
	fwrite(i->text, i->textpos, 1, fi);
	i = i->next;
    }
    fclose(fi);
    return 1;
}
void*text_result_get(gfxresult_t*r, const char*name)
{
    textpage_t*i= (textpage_t*)r->internal;
    if(!strcmp(name,"text")) {
	textpage_t*j = i;
	int len = 0;
	while(j) {
	    len += i->textpos;
	    j = j->next;
	}
	char*text = (char*)malloc(len);
	int pos = 0;
	j = i;
	while(j) {
	    memcpy(&text[pos], i->text, i->textpos);
	    pos += i->textpos;
	    j = j->next;
	}
	text[pos] = 0;
	return text;
    } else if(!strncmp(name,"page",4)) {
	int pagenr = atoi(&name[4]);
	if(pagenr<0)
	    pagenr=0;
	while(pagenr>0) {
	    i = i->next;
	    if(!i)
		return 0;
	}
	i->text[i->textpos] = 0;
	return strdup(i->text);
    }
    return 0;
}
void text_result_destroy(gfxresult_t*r)
{
    textpage_t*i= (textpage_t*)r->internal;
    r->internal = 0;
    while(i) {
	textpage_t*next = i->next;
	free(i->text);i->text = 0;
	free(i);
	i = next;
    }
    free(r);
}

gfxresult_t* text_finish(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    
    gfxresult_t* res = (gfxresult_t*)rfx_calloc(sizeof(gfxresult_t));
    
    res->internal = i->first_page;i->first_page = 0;i->current_page=0;
    res->write = text_result_write;
    res->save = text_result_save;
    res->get = text_result_get;
    res->destroy = text_result_destroy;

    free(dev->internal); dev->internal = 0; i = 0;

    return res;
}



void gfxdevice_text_init(gfxdevice_t*dev, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));

    dev->name = "text";

    dev->internal = i;

    dev->setparameter = text_setparameter;
    dev->startpage = text_startpage;
    dev->startclip = text_startclip;
    dev->endclip = text_endclip;
    dev->stroke = text_stroke;
    dev->fill = text_fill;
    dev->fillbitmap = text_fillbitmap;
    dev->fillgradient = text_fillgradient;
    dev->addfont = text_addfont;
    dev->drawchar = text_drawchar;
    dev->drawlink = text_drawlink;
    dev->endpage = text_endpage;
    dev->finish = text_finish;
}

