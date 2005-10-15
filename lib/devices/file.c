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
#include <memory.h>
#include "../gfxdevice.h"

typedef struct _internal {
    FILE*fi;
} internal_t;

int file_setparameter(struct _gfxdevice*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "setparameter %s=%s\n", key, value);
    return 0;
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
    dumpline(i->fi, line);
}

void file_fillgradient(struct _gfxdevice*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    fprintf(i->fi, "fillgradient\n");
    dumpline(i->fi, line);
}

void file_addfont(struct _gfxdevice*dev, char*fontid, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
}

void file_drawchar(struct _gfxdevice*dev, char*fontid, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
}

void file_drawlink(struct _gfxdevice*dev, gfxline_t*line, char*action)
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

gfxresult_t* file_finish(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    fclose(i->fi);
    i->fi = 0;
    free(dev->internal);
    dev->internal = 0;
    return 0;
}

void gfxdevice_file_init(gfxdevice_t*dev, char*filename)
{
    internal_t*i = malloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));
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
    if(!i->fi) {
	fprintf(stderr, "Couldn't open file %s\n", filename);
	exit(1);
    }
}

