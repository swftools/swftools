/* remove_invisible_characters.c

   Part of the swftools package.

   Copyright (c) 2011 Matthias Kramm <kramm@quiss.org> 
 
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
#include <stdint.h>
#include <memory.h>
#include "../gfxfilter.h"
#include "../gfxtools.h"
#include "../gfxfont.h"
#include "../gfxpoly.h"
#include "../gfximage.h"
#include "../types.h"
#include "../mem.h"
#include "../devices/render.h"

typedef struct _page {
    uint8_t* visible;
    int size8;
    int size;
    struct _page*next;
} page_t;

typedef struct _internal {
    gfxdevice_t render;
    int count;

    double area;

    page_t*first_page;
    page_t*last_page;

    page_t*current_page;
} internal_t;

static gfxcolor_t black = {255,0,0,0};

static void pass1_startpage(gfxfilter_t*f, int width, int height, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    gfxdevice_render_init(&i->render);
    i->render.startpage(&i->render, width, height);
    out->startpage(out, width, height);
    i->count = 0;
}
static void pass1_endpage(gfxfilter_t*f, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    i->render.endpage(&i->render);
    out->endpage(out);
    gfxresult_t*result = i->render.finish(&i->render);

    page_t*page = (page_t*)rfx_calloc(sizeof(page_t));
    if(!i->first_page) {
        i->first_page = i->last_page = i->current_page = page;
    } else {
        i->last_page->next = page;
        i->last_page = page;
    }
    page->size = i->count + 1;
    page->size8 = (page->size+7) >> 3;
    page->visible = (uint8_t*)rfx_calloc(page->size8);

    gfximage_t*img = (gfximage_t*)result->get(result, "page0");
    int size = img->width*img->height;
    int t;
    for(t=0;t<size;t++) {
        int index = ((*(uint32_t*)&img->data[t])>>8);
        page->visible[index>>3] |= (1<<(index&7));
    }
    result->destroy(result);
}
static void pass1_fill(gfxfilter_t*f, gfxline_t*line, gfxcolor_t*color, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    if(color->a == 255) {
        i->render.fill(&i->render, line, &black);
    }
    out->fill(out, line, color);
}
static void pass1_stroke(gfxfilter_t*f, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    if(color->a == 255) {
        i->render.stroke(&i->render, line, width, &black, cap_style, joint_style, miterLimit);
    }
    out->stroke(out, line, width, color, cap_style, joint_style, miterLimit);
}
static void pass1_fillbitmap(gfxfilter_t*f, gfxline_t*line, gfximage_t* bitmap, gfxmatrix_t*imgcoord2devcoord, gfxcxform_t*cxform, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    gfximage_t*img = gfximage_new(bitmap->width, bitmap->height);
    int size = img->width*img->height;
    memset(img->data, 0, size*sizeof(gfxcolor_t));
    int t;
    for(t=0;t<size;t++) {
        img->data[t].a = bitmap->data[t].a == 255 ? 255 : 0;
    }
    i->render.fillbitmap(&i->render, line, img, imgcoord2devcoord, 0);
    gfximage_free(img);
    out->fillbitmap(out, line, bitmap, imgcoord2devcoord, cxform);
}
static void pass1_fillgradient(gfxfilter_t*f, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*gradcoord2devcoord, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    i->render.fill(&i->render, line, &black);
    out->fillgradient(out, line, gradient, type, gradcoord2devcoord);
}
static void pass1_startclip(gfxfilter_t*f, gfxline_t*line, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    i->render.startclip(&i->render, line);
    out->startclip(out, line);
}
static void pass1_endclip(gfxfilter_t*f, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    i->render.endclip(&i->render);
    out->endclip(out);
}
static void pass1_addfont(gfxfilter_t*f, gfxfont_t*font, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    /* Don't pass this to device->render(). We're not in a page
       yet, and the render device doesn't need addfont() */
    //i->render.addfont(&i->render, font);
    out->addfont(out, font);
}
static void pass1_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    i->count++;
    gfxcolor_t c;
    *(uint32_t*)&c = i->count<<8|0xff;

    double advance = font->glyphs[glyphnr].advance;
    gfxline_t*box = gfxline_makerectangle(0,0,advance,font->ascent);
    gfxline_transform(box, matrix);
    i->render.fill(&i->render, box, &c);
    if(font->ascent < 512) {
        i->render.drawchar(&i->render, font, glyphnr, &c, matrix);
    }
    gfxline_free(box);

    out->drawchar(out, font, glyphnr, color, matrix);
}
static gfxresult_t*pass1_finish(gfxfilter_t*f, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    return out->finish(out);
}

static void pass2_startpage(gfxfilter_t*f, int width, int height, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    out->startpage(out, width, height);
    i->count = 0;
}
static void pass2_endpage(gfxfilter_t*f, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    i->current_page = i->current_page->next;
    out->endpage(out);
}
static void pass2_addfont(gfxfilter_t*f, gfxfont_t*font, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    out->addfont(out, font);
}
static void pass2_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    i->count++;
    if(i->current_page->visible[i->count>>3]&(1<<(i->count&7))) {
        out->drawchar(out, font, glyphnr, color, matrix);
    } else {
#ifdef DEBUG_AREA
        gfxline_t*line = gfxline_clone(font->glyphs[glyphnr].line);
        gfxline_transform(line, matrix);
        gfxpoly_t*poly = gfxpoly_from_fill(line, 0.05);
        double area = gfxpoly_area(poly);
        gfxpoly_destroy(poly);
        gfxline_free(line);
        i->area += area;
#endif
    }
}
static gfxresult_t*pass2_finish(gfxfilter_t*f, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
#ifdef DEBUG_AREA
    printf("%f\n", i->area);
#endif
    return out->finish(out);
}

void gfxtwopassfilter_remove_invisible_characters_init(gfxtwopassfilter_t*f)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));

    memset(f, 0, sizeof(gfxtwopassfilter_t));
    f->type = gfxfilter_twopass;

    f->pass1.name = "filter \"remove invisible characters\" pass 1";
    f->pass1.addfont = pass1_addfont;
    f->pass1.drawchar = pass1_drawchar;
    f->pass1.fill = pass1_fill;
    f->pass1.fillbitmap = pass1_fillbitmap;
    f->pass1.fillgradient = pass1_fillgradient;
    f->pass1.stroke = pass1_stroke;
    f->pass1.startclip= pass1_startclip;
    f->pass1.endclip = pass1_endclip;
    f->pass1.startpage = pass1_startpage;
    f->pass1.endpage = pass1_endpage;
    f->pass1.finish = pass1_finish;
    f->pass1.internal = i;

    f->pass2.name = "filter \"remove invisible characters\" pass 2";
    f->pass2.addfont = pass2_addfont;
    f->pass2.drawchar = pass2_drawchar;
    f->pass2.startpage = pass2_startpage;
    f->pass2.endpage = pass2_endpage;
    f->pass2.finish = pass2_finish;
    f->pass2.internal = i;
}

