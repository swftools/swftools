/* pdf.c

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
#include <assert.h>
#include <memory.h>
#include <pdflib.h>
#include <math.h>
#include "../os.h"
#include "../q.h"
#include "../jpeg.h"
#include "../types.h"
#include "../mem.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../gfximage.h"

typedef struct _internal {
    PDF* p;
    char*tempfile;
    double lastx,lasty;
    gfxfontlist_t*fontlist;
} internal_t;

int pdf_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    return 0;
}

void pdf_startpage(gfxdevice_t*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;

    if(!i->tempfile) {
	i->tempfile = strdup(mktempname(0));
	PDF_open_file(i->p, i->tempfile);
	PDF_set_parameter(i->p, "usercoordinates", "true");
	PDF_set_parameter(i->p, "topdown", "true");
    }

    PDF_begin_page(i->p, width, height);
    PDF_set_parameter(i->p, "fillrule", "evenodd");
}

static int mkline(gfxline_t*line, PDF*p, char fill)
{
    double x,y;
    char first = 1;
    int ret = 0;
    char free_line = 0;
    if(fill) {
	line = gfxline_restitch(gfxline_clone(line));
	free_line = 1;
    }
    while(line) {
	if(line->type == gfx_moveTo && (x!=line->x || y!=line->y || first)) {
	    first = 0;
	    PDF_moveto(p, line->x, line->y);
	} else if(line->type == gfx_lineTo) {
	    PDF_lineto(p, line->x, line->y);
	    ret = 1;
	} else {
	    /* when converting a quadratic bezier to a cubic bezier, the
	       two new control points are both 2/3 the way from the
	       endpoints to the old control point */
	    double c1x = (x + line->sx*2)/3;
	    double c1y = (y + line->sy*2)/3;
	    double c2x = (line->x + line->sx*2)/3;
	    double c2y = (line->y + line->sy*2)/3;
	    PDF_curveto(p, c1x, c1y, c2x, c2y, line->x, line->y);
	    ret = 1;
	}
	x = line->x;
	y = line->y;
	line = line->next;
    }
    if(free_line)
	gfxline_free(line);
    return ret;
}

void pdf_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    PDF_save(i->p);
    if(mkline(line, i->p, 1))
	PDF_clip(i->p);
    else   
	; // TODO: strictly speaking, an empty clip clears everything

}
void pdf_endclip(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    PDF_restore(i->p);
}
void pdf_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    if(width<1e-6)
	return;
    PDF_setlinewidth(i->p, width);
    PDF_setlinecap(i->p, cap_style==gfx_capButt?0:(cap_style==gfx_capRound?1:2));
    PDF_setlinejoin(i->p, joint_style==gfx_joinMiter?0:(joint_style==gfx_joinRound?1:2));
    PDF_setrgbcolor_stroke(i->p, color->r/255.0, color->g/255.0, color->b/255.0);
    if(joint_style==gfx_joinMiter)
	PDF_setmiterlimit(i->p, miterLimit);
    if(mkline(line, i->p, 0))
	PDF_stroke(i->p);
}

void pdf_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    PDF_setrgbcolor_fill(i->p, color->r/255.0, color->g/255.0, color->b/255.0);
	
    if(mkline(line, i->p, 1)) {
	PDF_fill(i->p);
    }
}

void pdf_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;

    double l1 = sqrt(matrix->m00*matrix->m00+matrix->m01*matrix->m01)*img->width;
    double l2 = sqrt(matrix->m10*matrix->m10+matrix->m11*matrix->m11)*img->height;
    double r = atan2(matrix->m01, matrix->m00);

    /* fit_image needs the lower left corner of the image */
    double x = matrix->tx + matrix->m10*img->height;
    double y = matrix->ty + matrix->m11*img->height;

    char*tempfile = mktempname(0);
    char options[80];
    sprintf(options, "boxsize {%f %f} fitmethod meet rotate %f", l1, l2, r*180/M_PI);
    gfximage_save_jpeg(img, tempfile, 99);
    int imgid = PDF_load_image(i->p, "jpeg", tempfile, 0, "");
    PDF_fit_image(i->p, imgid, x, y, options);
}

void pdf_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
}

static char type3 = 1;

void pdf_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;

    if(type3) {
	int fontid = 0;
	if(!gfxfontlist_hasfont(i->fontlist, font)) {

	    static int fontnr = 1;
	    char fontname[32];
	    sprintf(fontname, "font%d", fontnr++);
	    int l = strlen(fontname);
	    char fontname2[64];
	    int t;
	    for(t=0;t<l+1;t++) {
		fontname2[t*2+0] = fontname[t];
		fontname2[t*2+1] = 0;
	    }

	    PDF_begin_font(i->p, fontname2, l*2, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, "");
	    int num = font->num_glyphs<256-32?font->num_glyphs:256-32;
	    for(t=0;t<num;t++) {
		gfxglyph_t*g = &font->glyphs[t];
		gfxbbox_t bbox = gfxline_getbbox(g->line);
		char name[32];
		sprintf(name, "chr%d", t+32);
		PDF_encoding_set_char(i->p, fontname, t+32, name, 0);
		PDF_begin_glyph(i->p, name, g->advance, bbox.xmin, bbox.ymin, bbox.xmax, bbox.ymax);
		if(mkline(g->line, i->p, 1))
		    PDF_fill(i->p);
		PDF_end_glyph(i->p);
	    }
	    PDF_end_font(i->p);
	    fontid = PDF_load_font(i->p, fontname2, l*2, fontname, "");
	    
	    i->fontlist = gfxfontlist_addfont2(i->fontlist, font, (void*)(ptroff_t)fontid);
	}
    }
}

void pdf_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;

    if(!font)
	return;

    gfxglyph_t*glyph = &font->glyphs[glyphnr];
    PDF_setrgbcolor_fill(i->p, color->r/255.0, color->g/255.0, color->b/255.0);
    char as_shape = 0;
    if(!type3) as_shape=1;
    if(glyphnr>256-32) as_shape=1;
    if(fabs(matrix->m00 + matrix->m11) > 0.01) as_shape=1;
    if(fabs(fabs(matrix->m01) + fabs(matrix->m10)) > 0.01) as_shape=1;
    if(fabs(matrix->m00) < 1e-6) as_shape=1;

    if(as_shape) {
	gfxline_t*line2 = gfxline_clone(glyph->line);
	gfxline_transform(line2, matrix);
	if(mkline(line2, i->p, 1)) {
	    PDF_fill(i->p);
	}
	gfxline_free(line2);
    } else {
	assert(gfxfontlist_hasfont(i->fontlist, font));
	int fontid = (int)(ptroff_t)gfxfontlist_getuserdata(i->fontlist, font->id);
	PDF_setfont(i->p, fontid, matrix->m00);
	char name[32];
	sprintf(name, "%c", glyphnr+32);

	if(fabs(matrix->tx - i->lastx) > 0.001 || matrix->ty != i->lasty) {
	    PDF_show_xy2(i->p, name, strlen(name), matrix->tx, matrix->ty);
	} else {
	    PDF_show2(i->p, name, strlen(name));
	}

	i->lastx = matrix->tx + glyph->advance*matrix->m00;
	i->lasty = matrix->ty;
    }
    return;
}

void pdf_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action)
{
    internal_t*i = (internal_t*)dev->internal;
}

void pdf_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    PDF_end_page(i->p);
}

typedef struct pdfresult_internal {
    char*tempfile;
} pdfresult_internal_t;

void pdfresult_destroy(gfxresult_t*gfx)
{
    pdfresult_internal_t*i = (pdfresult_internal_t*)gfx->internal;
    unlink(i->tempfile);
    free(i->tempfile);
    free(gfx->internal);gfx->internal = 0;
    free(gfx);
}

int pdfresult_save(gfxresult_t*gfx, const char*filename)
{
    pdfresult_internal_t*i = (pdfresult_internal_t*)gfx->internal;
    FILE*fi = fopen(i->tempfile, "rb");
    FILE*fo = fopen(filename, "wb");
    if(!fo) {
	perror(filename);
	return -1;
    }
    char buffer[4096];
    int size = 0;
    while((size = fread(buffer, 1, 4096, fi))) {
	fwrite(buffer, 1, size, fo);
    }
    fclose(fi);
    fclose(fo);
    return 0;
}

void* pdfresult_get(gfxresult_t*gfx, const char*name)
{
    return 0; 
}

gfxresult_t* pdf_finish(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    
    PDF_close(i->p);
    PDF_delete(i->p);

    gfxresult_t*result = (gfxresult_t*)malloc(sizeof(gfxresult_t));
    memset(result, 0, sizeof(gfxresult_t));
    result->save = pdfresult_save;
    result->get = pdfresult_get;
    result->destroy = pdfresult_destroy;
    result->internal = 0;
    result->internal = malloc(sizeof(pdfresult_internal_t));
    pdfresult_internal_t*ri = (pdfresult_internal_t*)result->internal;
    ri->tempfile = i->tempfile;i->tempfile=0;
    free(dev->internal);dev->internal = 0;i=0;
    return result;
}

void gfxdevice_pdf_init(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));

    dev->name = "pdf";

    dev->internal = i;

    dev->setparameter = pdf_setparameter;
    dev->startpage = pdf_startpage;
    dev->startclip = pdf_startclip;
    dev->endclip = pdf_endclip;
    dev->stroke = pdf_stroke;
    dev->fill = pdf_fill;
    dev->fillbitmap = pdf_fillbitmap;
    dev->fillgradient = pdf_fillgradient;
    dev->addfont = pdf_addfont;
    dev->drawchar = pdf_drawchar;
    dev->drawlink = pdf_drawlink;
    dev->endpage = pdf_endpage;
    dev->finish = pdf_finish;

    i->lastx = -1e38;
    i->lasty = -1e38;
    i->p = PDF_new();
}

