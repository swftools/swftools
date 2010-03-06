/* remove_font_transform.c

   Part of the swftools package.

   Copyright (c) 2010 Matthias Kramm <kramm@quiss.org> 
 
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
#include <memory.h>
#include "../gfxfilter.h"
#include "../gfxtools.h"
#include "../types.h"
#include "../mem.h"

typedef struct _internal {
} internal_t;

static void pass1_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    out->drawchar(out, font, glyphnr, color, matrix);
}

static void pass2_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    out->drawchar(out, font, glyphnr, color, matrix);
}

void gfxtwopassfilter_remove_font_transforms_init(gfxtwopassfilter_t*f)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(f, 0, sizeof(gfxtwopassfilter_t));
    f->type = gfxfilter_twopass;
    f->pass1.name = "remove font transforms pass 1";
    f->pass1.drawchar = pass1_drawchar;
    f->pass1.internal = i;

    f->pass2.name = "remove font transforms pass 2";
    f->pass2.drawchar = pass2_drawchar;
    f->pass2.internal = i;
}

