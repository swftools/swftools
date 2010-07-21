/* gfxdevice_file.cc

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
#ifndef WIN32
#include <unistd.h>
#endif
#include <string.h>
#include <memory.h>
#include "../gfxdevice.h"

typedef struct _internal {
    char*filename;
    FILE*fi;
} internal_t;

int file_setparameter(struct _gfxdevice*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "setparameter %s=%s\n", key, value);
    return 1;
}

void file_startpage(struct _gfxdevice*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "startpage %d %d\n", width, height);
}

static void dumpline(FILE*fi, gfxline_t*line)
{
    while(line) {
	if(line->type == gfx_moveTo) {
	    fprintf(fi, "\tmoveTo %f %f\n", line->x, line->y);
	} else if(line->type == gfx_lineTo) {
	    fprintf(fi, "\tlineTo %f %f\n", line->x, line->y);
	} else if(line->type == gfx_splineTo) {
	    fprintf(fi, "\tsplineTo %f %f %f %f\n", line->sx, line->sy, line->x, line->y);
	}
	line = line->next;
    }
}

static void dumpmatrix (FILE*fi, gfxmatrix_t*matrix)
{
    fprintf(fi, "| %5.2f %5.2f %5.2f\n", matrix->m00, matrix->m10, matrix->tx);
    fprintf(fi, "| %5.2f %5.2f %5.2f\n", matrix->m01, matrix->m11, matrix->ty);
}

static void dumpgradient (FILE*fi, gfxgradient_t*gradient)
{
    while(gradient) {
        fprintf(fi, "pos %f: %02x%02x%02x%02x\n", gradient->pos, gradient->color.r, gradient->color.g, gradient->color.b, gradient->color.a);
	gradient = gradient->next;
    }
}

void file_startclip(struct _gfxdevice*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "startclip\n");
    dumpline(i->fi, line);
}

void file_endclip(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "endclip\n");
}

void file_stroke(struct _gfxdevice*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    char* jointTypes[] = {"joinMiter", "joinRound", "joinBevel"};
    char* capTypes[] = {"capButt", "capRound", "capSquare"};

    fprintf(i->fi, "stroke %f %f %s %s %02x%02x%02x%02x\n", width, miterLimit, capTypes[cap_style], jointTypes[joint_style],
		color->r, color->g, color->b, color->a
	    );
    dumpline(i->fi, line);
}

void file_fill(struct _gfxdevice*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "fill %02x%02x%02x%02x\n", color->r, color->g, color->b, color->a);
    dumpline(i->fi, line);
}

void file_fillbitmap(struct _gfxdevice*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "fillbitmap\n");
    dumpmatrix(i->fi, matrix);
    dumpline(i->fi, line);
}

void file_fillgradient(struct _gfxdevice*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "fillgradient\n");
    dumpmatrix(i->fi, matrix);
    dumpgradient(i->fi, gradient);
    dumpline(i->fi, line);
}

void file_addfont(struct _gfxdevice*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "addfont %s\n", font->id);
}

void file_drawchar(struct _gfxdevice*dev, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
}

void file_drawlink(struct _gfxdevice*dev, gfxline_t*line, const char*action, const char*text)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "drawlink %s\n", action);
    dumpline(i->fi, line);
}

void file_endpage(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "endpage\n");
}

typedef struct gfxresult_internal
{
    FILE*fi;
    char*filename;
} gfxresult_internal_t;
    
void fileresult_destroy(struct _gfxresult*gfx)
{
    gfxresult_internal_t*i = (gfxresult_internal_t*)gfx->internal;
    unlink(i->filename);
    free(i->filename);i->filename = 0;
}

int fileresult_save(struct _gfxresult*gfx, const char*filename)
{
    gfxresult_internal_t*i = (gfxresult_internal_t*)gfx->internal;
    FILE*fi,*fo;
    fi = fopen(i->filename, "rb");
    if(!fi) {
	perror(i->filename);
	return 0;
    }
    fo = fopen(filename, "wb");
    if(!fo) {
	perror(filename);
	return 0;
    }

    char buf[4096];
    while(!feof(fi)) {
	int l = fread(buf, 1, 4096, fi);
	if(l>0) {
	    fwrite(buf, 1, l, fo);
	} else {
	    break;
	}
    }

    fclose(fi);
    fclose(fo);
    return 0;
}

void* fileresult_get(struct _gfxresult*gfx, const char*name)
{
    return 0; 
}

gfxresult_t* file_finish(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    char*filename = strdup(i->filename);
    gfxresult_t*result = (gfxresult_t*)malloc(sizeof(gfxresult_t));
    fclose(i->fi);
    i->fi = 0;
    if(i->filename) {
	free(i->filename);
	i->filename = 0;
    }
    free(dev->internal);
    dev->internal = 0;

    memset(result, 0, sizeof(gfxresult_t));
    result->save = fileresult_save;
    result->get = fileresult_get;
    result->destroy = fileresult_destroy;
    result->internal = malloc(sizeof(gfxresult_internal_t));
    ((gfxresult_internal_t*)result->internal)->filename = filename;

    return result;
}

void gfxdevice_file_init(gfxdevice_t*dev, char*filename)
{
    internal_t*i = (internal_t*)malloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));

    dev->name = "file";

    dev->internal = i;

    dev->setparameter = file_setparameter;
    dev->startpage = file_startpage;
    dev->startclip = file_startclip;
    dev->endclip = file_endclip;
    dev->stroke = file_stroke;
    dev->fill = file_fill;
    dev->fillbitmap = file_fillbitmap;
    dev->fillgradient = file_fillgradient;
    dev->addfont = file_addfont;
    dev->drawchar = file_drawchar;
    dev->drawlink = file_drawlink;
    dev->endpage = file_endpage;
    dev->finish = file_finish;

    i->fi = fopen(filename, "wb");
    i->filename = strdup(filename);
    if(!i->fi) {
	fprintf(stderr, "Couldn't open file %s\n", filename);
	exit(1);
    }
}

