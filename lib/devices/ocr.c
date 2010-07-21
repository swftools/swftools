/* ocr.c

   Part of the swftools package.

   Copyright (c) 2007 Matthias Kramm <kramm@quiss.org> 
 
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
#include <stdarg.h>
#include <unistd.h>
#include <memory.h>
#include "../types.h"
#include "../mem.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "render.h"

#include "../gocr/pnm.h"
#include "../gocr/pgm2asc.h"

typedef struct _textpage {
    char*text;
    int textpos;
    struct _textpage*next;
} textpage_t;

typedef struct _internal {
    gfxdevice_t*render;
    int pages;
    
    textpage_t*first_page;
    textpage_t*current_page;
} internal_t;

int ocr_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    return i->render->setparameter(i->render,key,value);
}

void ocr_startpage(gfxdevice_t*dev, int width, int height) 
{ 
    internal_t*i = (internal_t*)dev->internal;
    if(i->render) {
        fprintf(stderr, "Call endpage() before calling startpage()\n");
        return;
    }
    i->render = malloc(sizeof(gfxdevice_t));
    gfxdevice_render_init(i->render);
    i->render->startpage(i->render,width,height); 
    i->pages++;
}
/* passthrough */
void ocr_startclip(gfxdevice_t*dev, gfxline_t*line) { ((internal_t*)dev->internal)->render->startclip(((internal_t*)dev->internal)->render,line); }
void ocr_endclip(gfxdevice_t*dev) { ((internal_t*)dev->internal)->render->endclip(((internal_t*)dev->internal)->render); }
void ocr_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit) { ((internal_t*)dev->internal)->render->stroke(((internal_t*)dev->internal)->render, line, width, color, cap_style, joint_style, miterLimit); }
void ocr_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color) { ((internal_t*)dev->internal)->render->fill(((internal_t*)dev->internal)->render, line, color); }
void ocr_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform) { ((internal_t*)dev->internal)->render->fillbitmap(((internal_t*)dev->internal)->render, line, img, matrix, cxform); }
void ocr_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix) { ((internal_t*)dev->internal)->render->fillgradient(((internal_t*)dev->internal)->render, line, gradient, type, matrix); }
void ocr_addfont(gfxdevice_t*dev, gfxfont_t*font) { ((internal_t*)dev->internal)->render->addfont(((internal_t*)dev->internal)->render, font); }
void ocr_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix) { ((internal_t*)dev->internal)->render->drawchar(((internal_t*)dev->internal)->render, font, glyphnr, color, matrix); }
void ocr_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text) { ((internal_t*)dev->internal)->render->drawlink(((internal_t*)dev->internal)->render, line, action, text); }

void ocr_result_write(gfxresult_t*r, int filedesc)
{
    textpage_t*i= (textpage_t*)r->internal;
}
int ocr_result_save(gfxresult_t*r, const char*filename)
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

void*ocr_result_get(gfxresult_t*r, const char*name)
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
            pagenr++;
	}
	i->text[i->textpos] = 0;
	return strdup(i->text);
    }
    return 0;
}
void ocr_result_destroy(gfxresult_t*r)
{
    textpage_t*i= (textpage_t*)r->internal;
    int t;
    r->internal = 0;
    while(i) {
	textpage_t*next = i->next;
	free(i->text);i->text = 0;
	free(i);
	i = next;
    }
    free(r);
}

job_t*JOB;

void ocr_endpage(gfxdevice_t*dev) 
{ 
    internal_t*i = (internal_t*)dev->internal;
    i->render->endpage(i->render); 

    gfxdevice_t*out = i->render;
    gfxresult_t* r = out->finish(out);
    free(i->render);i->render = 0;

    gfximage_t*img = (gfximage_t*)r->get(r, "page");

    job_t job;
    JOB = &job;
    
    job_init(&job);
    job.cfg.out_format=UTF8;

    job.src.fname = "<none>";
    job.src.p.p = malloc(img->width*img->height);
    job.src.p.bpp = 1;
    job.src.p.x = img->width;
    job.src.p.y = img->height;
    int size=img->width*img->height;
    int t;
    for(t=0;t<size;t++) {
        job.src.p.p[t] = (img->data[t].r+img->data[t].g+img->data[t].b)/3;
    }

    pgm2asc(&job);

    int linecounter;
    const char *line = 0;
    int len = 0;
    linecounter = 0;
    line = getTextLine(linecounter++);
    while (line) {
      len += strlen(line)+1;
      line = getTextLine(linecounter++);
    }

    textpage_t*page = malloc(sizeof(textpage_t));
    page->next = 0;
    page->text = malloc(len+1);
    page->textpos = 0;
    if(!i->first_page) {
        i->first_page = i->current_page = page;
    } else {
        i->current_page->next = page;
        i->current_page = page;
    }

    linecounter = 0;
    line = getTextLine(linecounter++);
    while (line) {
      int l = strlen(line);
      memcpy(&page->text[page->textpos], line, l);
      page->textpos += l;
      page->text[page->textpos++] = '\n';

      line = getTextLine(linecounter++);
    }
    page->text[page->textpos++] = 0;

    free_textlines();

    job_free(&job);JOB=0;
    
    r->destroy(r);
}

gfxresult_t* ocr_finish(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    
    gfxresult_t*r = (gfxresult_t*)rfx_calloc(sizeof(gfxresult_t));
    
    r->internal = i->first_page;
    r->write = ocr_result_write;
    r->save = ocr_result_save;
    r->get = ocr_result_get;
    r->destroy = ocr_result_destroy;

    free(dev->internal); dev->internal = 0; i = 0;

    return r;
}

void gfxdevice_ocr_init(gfxdevice_t*dev, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));

    dev->name = "ocr";

    dev->internal = i;

    dev->setparameter = ocr_setparameter;
    dev->startpage = ocr_startpage;
    dev->startclip = ocr_startclip;
    dev->endclip = ocr_endclip;
    dev->stroke = ocr_stroke;
    dev->fill = ocr_fill;
    dev->fillbitmap = ocr_fillbitmap;
    dev->fillgradient = ocr_fillgradient;
    dev->addfont = ocr_addfont;
    dev->drawchar = ocr_drawchar;
    dev->drawlink = ocr_drawlink;
    dev->endpage = ocr_endpage;
    dev->finish = ocr_finish;

    i->pages = 0;
}

