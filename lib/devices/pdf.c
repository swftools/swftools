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
#include "../log.h"
#include "../jpeg.h"
#include "../types.h"
#include "../mem.h"
#include "../log.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../gfximage.h"
#include "../gfxfont.h"

typedef struct _internal {
    PDF* p;
    
    char config_addblankpages;
    double config_xpad;
    double config_ypad;
    int config_maxdpi;
    int config_mindpi;

    int width,height;
    int num_pages;

    char*tempfile;
    char*page_opts;
    double lastx,lasty;
    gfxfontlist_t*fontlist;

    char has_matrix;
    double m00, m01, m10, m11;
} internal_t;

static void restore_matrix(internal_t*i)
{
    if(i->has_matrix) {
	PDF_restore(i->p);
	i->has_matrix=0;
	i->m00 = 0;
	i->m01 = 0;
	i->m10 = 0;
	i->m11 = 0;
    }
}
static void set_matrix(internal_t*i, double m00, double m01, double m10, double m11)
{
    restore_matrix(i);

    i->m00 = m00;
    i->m01 = m01;
    i->m10 = m10;
    i->m11 = m11;

    PDF_save(i->p);
    PDF_setmatrix(i->p, m00, -m01, m10, -m11, 0, i->height+2*i->config_ypad);
    i->has_matrix = 1;
}
static void reset_matrix(internal_t*i)
{
    set_matrix(i, 1.0, 0.0, 0.0, 1.0);
}
static void transform_back(internal_t*i, double x, double y, double *ox, double *oy)
{
    double det = i->m00*i->m11 - i->m10*i->m01;
    if(!det) {
	msg("<warning> Codependent text matrix");
	*ox=*oy=0;
	return;
    }
    *ox = (x*i->m11 - i->m10*y)/det;
    *oy = (i->m00*y - x*i->m01)/det;
}

void pdf_startpage(gfxdevice_t*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;

    if(!i->tempfile) {
	i->tempfile = strdup(mktempname(0, "pdf"));

	PDF_begin_document(i->p, i->tempfile, 0, "");
	//PDF_set_value(i->p, "compress", 0);
	PDF_set_parameter(i->p, "usercoordinates", "true");
	PDF_set_parameter(i->p, "topdown", "true");
    }

    int width_plus_pad = width+floor(i->config_xpad*2);
    int height_plus_pad = height+floor(i->config_ypad*2);
    PDF_begin_page_ext(i->p, width_plus_pad, height_plus_pad, i->page_opts);
    PDF_set_value(i->p, "CropBox/llx", 0);
    PDF_set_value(i->p, "CropBox/lly", 0);
    PDF_set_value(i->p, "CropBox/urx", width_plus_pad);
    PDF_set_value(i->p, "CropBox/ury", height_plus_pad);
    if(i->config_xpad || i->config_ypad) {
	PDF_set_value(i->p, "TrimBox/llx", i->config_xpad);
	PDF_set_value(i->p, "TrimBox/lly", i->config_ypad);
	PDF_set_value(i->p, "TrimBox/urx", i->config_xpad+width);
	PDF_set_value(i->p, "TrimBox/ury", i->config_ypad+height);
    }

    PDF_set_parameter(i->p, "fillrule", "evenodd");
    i->width = width;
    i->height = height;
    i->num_pages++;

    reset_matrix(i);
}

int pdf_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!strcmp(key, "addblankpages")) {
	i->config_addblankpages = atoi(value);
    } else if(!strcmp(key, "maxdpi")) {
	i->config_maxdpi = atoi(value);
    } else if(!strcmp(key, "mindpi")) {
	i->config_mindpi = atoi(value);
    } else if(!strcmp(key, "xpad")) {
	i->config_xpad = atof(value);
    } else if(!strcmp(key, "ypad")) {
	i->config_ypad = atof(value);
    }
    return 0;
}


static int mkline(gfxline_t*line, PDF*p, double mx, double my, double scale, char fill)
{
    double x=0,y=0;
    char first = 1;
    int ret = 0;
    gfxline_t*free_line = 0;
    if(fill) {
	line = gfxline_restitch(gfxline_clone(line));
	free_line = line;
    }
    while(line) {
	if(line->type == gfx_moveTo && (x!=line->x || y!=line->y || first)) {
	    first = 0;
	    PDF_moveto(p, line->x*scale+mx, line->y*scale+my);
	} else if(line->type == gfx_lineTo) {
	    PDF_lineto(p, line->x*scale+mx, line->y*scale+my);
	    ret = 1;
	} else {
	    /* when converting a quadratic bezier to a cubic bezier, the
	       two new control points are both 2/3 the way from the
	       endpoints to the old control point */
	    double c1x = (x + line->sx*2)/3;
	    double c1y = (y + line->sy*2)/3;
	    double c2x = (line->x + line->sx*2)/3;
	    double c2y = (line->y + line->sy*2)/3;
	    PDF_curveto(p, c1x*scale+mx, c1y*scale+my, 
		           c2x*scale+mx, c2y*scale+my, 
			   line->x*scale+mx, line->y*scale+my);
	    ret = 1;
	}
	x = line->x;
	y = line->y;
	line = line->next;
    }
    if(free_line)
	gfxline_free(free_line);
    return ret;
}

void pdf_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    
    restore_matrix(i);
    PDF_save(i->p);
    
    if(mkline(line, i->p, i->config_xpad, i->config_ypad, 1.0, 1))
	PDF_clip(i->p);
    else   
	; // TODO: strictly speaking, an empty clip clears everything
    
    reset_matrix(i);
}
void pdf_endclip(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    restore_matrix(i);
    PDF_restore(i->p);
}
void pdf_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    if(width<1e-6)
	return;
    reset_matrix(i);
    PDF_setlinewidth(i->p, width);
    PDF_setlinecap(i->p, cap_style==gfx_capButt?0:(cap_style==gfx_capRound?1:2));
    PDF_setlinejoin(i->p, joint_style==gfx_joinMiter?0:(joint_style==gfx_joinRound?1:2));
    
    PDF_setrgbcolor_stroke(i->p, color->r/255.0, color->g/255.0, color->b/255.0);

    if(joint_style==gfx_joinMiter)
	PDF_setmiterlimit(i->p, miterLimit);
    if(mkline(line, i->p, i->config_xpad, i->config_ypad, 1.0, 0))
	PDF_stroke(i->p);
}

void pdf_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;
    reset_matrix(i);
    PDF_setrgbcolor_fill(i->p, color->r/255.0, color->g/255.0, color->b/255.0);
    /*
       pdf-x (pdf 1.3) doesn't support opacityfill
    if(color->a!=255) {
	char opacityfill[80];
	sprintf(opacityfill, "opacityfill %f", color->a/256.0);
	int gstate = PDF_create_gstate(i->p, opacityfill);
	PDF_set_gstate(i->p, gstate);
    }*/
	
    if(mkline(line, i->p, i->config_xpad, i->config_ypad, 1.0, 1)) {
	PDF_fill(i->p);
    }
}

void pdf_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;

    int t,size=img->width*img->height;
    int has_alpha=0;
    for(t=0;t<size;t++) {
	if(img->data[t].a!=255) {
	    has_alpha=1;
	    break;
	}
    }

    double w = sqrt(matrix->m00*matrix->m00+matrix->m01*matrix->m01);
    double h = sqrt(matrix->m10*matrix->m10+matrix->m11*matrix->m11);
    double l1 = w*img->width;
    double l2 = h*img->height;

    double dpi_x = 72.0 / w;
    double dpi_y = 72.0 / h;
    double dpi = dpi_x>dpi_y?dpi_x:dpi_y;
    gfximage_t*rescaled_image = 0;
    if(i->config_maxdpi && dpi > i->config_maxdpi) {
	int newwidth = img->width*i->config_maxdpi/dpi;
	int newheight = img->height*i->config_maxdpi/dpi;
	rescaled_image = gfximage_rescale(img, newwidth, newheight);
	msg("<notice> Downscaling %dx%d image (dpi %f, %.0fx%.0f on page) to %dx%d (dpi %d)", 
		img->width, img->height, dpi, l1, l2, newwidth, newheight, i->config_maxdpi);
	img = rescaled_image;
    }
    if(i->config_mindpi && dpi < i->config_mindpi && img->width>1 && img->height>1) {
	msg("<error> Found image of size %dx%d with dpi %f, minimum allowed dpi is %d", 
		img->width, img->height, dpi, i->config_mindpi);
	exit(1);
    }

    char tempfile[128];
    mktempname(tempfile, "jpg");

    gfximage_save_jpeg(img, tempfile, 96);

    int imgid=-1;
    if(has_alpha) {
	char tempfile2[128];
	mktempname(tempfile2, "jpg");
	int t;
	int size = img->width*img->height;
	unsigned char*alpha = malloc(size);
	for(t=0;t<size;t++) {
	    alpha[t] = img->data[t].a;
	}
	jpeg_save_gray(alpha, img->width, img->height, 97, tempfile2);
	free(alpha);
	int maskid = PDF_load_image(i->p, "jpeg", tempfile2, 0, "mask");
	unlink(tempfile2);
	char masked[80];
	if(maskid<0) {
	    msg("<error> Couldn't process mask jpeg of size %dx%d: error code %d", img->width, img->height, maskid);
	    return;
	}
	sprintf(masked, "masked %d", maskid);
	imgid = PDF_load_image(i->p, "jpeg", tempfile, 0, masked);
    } else {
	imgid = PDF_load_image(i->p, "jpeg", tempfile, 0, "");
    }

    if(imgid<0) {
	msg("<error> Couldn't process jpeg of size %dx%d: error code %d, file %s", img->width, img->height, imgid, tempfile);
	return;
    }
    unlink(tempfile);
    
    char options[80];
    set_matrix(i, matrix->m00, matrix->m01, matrix->m10, matrix->m11);
    /* an image's (0,0) is at the lower left corner */
    double x=matrix->tx + i->config_xpad + matrix->m10*img->height;
    double y=matrix->ty + i->config_ypad + matrix->m11*img->height;
    double tx,ty;
    transform_back(i, x, y, &tx, &ty);
    PDF_place_image(i->p, imgid, tx, ty, 1.0);
    PDF_close_image(i->p, imgid);

    if(rescaled_image)
	gfximage_free(rescaled_image);
}

void pdf_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
}

static const char type3 = 0;
static const char ttf = 1;

void pdf_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;

    int num = font->num_glyphs<256-32?font->num_glyphs:256-32;
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

	    PDF_begin_font(i->p, fontname2, l*2, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, "");
	    for(t=0;t<num;t++) {
		gfxglyph_t*g = &font->glyphs[t];
		gfxbbox_t bbox = gfxline_getbbox(g->line);
		char name[32];
		sprintf(name, "chr%d", t+32);
		PDF_encoding_set_char(i->p, fontname, t+32, name, 0);
		PDF_begin_glyph(i->p, name, g->advance, bbox.xmin/64.0, bbox.ymin/64.0, bbox.xmax/64.0, bbox.ymax/64.0);
		if(mkline(g->line, i->p, 0, 0, 1.0/64.0, 1))
		    PDF_fill(i->p);
		PDF_end_glyph(i->p);
	    }
	    PDF_end_font(i->p);
	    fontid = PDF_load_font(i->p, fontname2, l*2, fontname, "");
	    
	    i->fontlist = gfxfontlist_addfont2(i->fontlist, font, (void*)(ptroff_t)fontid);
	}
    } else if(ttf) {
	int fontid = 0;
	if(!gfxfontlist_hasfont(i->fontlist, font)) {
	    char fontname[32],filename[32],fontname2[64];
	    static int fontnr = 1;
	    sprintf(fontname, "font%d", fontnr);
	    sprintf(filename, "font%d.ttf", fontnr);
	    fontnr++;
	    const char*old_id = font->id;
	    font->id = fontname;
	    int t;
	    for(t=0;t<num;t++) {
		font->glyphs[t].unicode = 32+t;
	    }
	    font->max_unicode = 0;
	    font->unicode2glyph = 0;
	    gfxfont_save(font, filename);
	    font->id=old_id;
	    
#ifdef RUN_TTX
	    /* for testing the generated fonts: run everything through ttx (fonttools) */
	    char cmd[256];
	    sprintf(cmd, "mv %s.ttf test.ttf", fontname);system(cmd);
	    system("rm -f test.ttx");
	    if(system("ttx test.ttf")&0xff00) exit(1);
	    sprintf(cmd, "mv test.ttf %s.old.ttf", fontname, fontname);system(cmd);
	    sprintf(cmd, "ttx test.ttx;mv test.ttf %s.ttf", fontname);system(cmd);
	    sprintf(cmd, "rm -f test.ttx");system(cmd);
#endif
	   
	    int l = strlen(fontname);
	    for(t=0;t<l+1;t++) {
		fontname2[t*2+0] = fontname[t];
		fontname2[t*2+1] = 0;
	    }
	    
	    fontid = PDF_load_font(i->p, fontname2, l*2, "host", "embedding=true");
	    i->fontlist = gfxfontlist_addfont2(i->fontlist, font, (void*)(ptroff_t)fontid);
	    unlink(filename);
	}
    }
}

void pdf_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;

    if(!font)
	return;

    gfxglyph_t*glyph = &font->glyphs[glyphnr];
    char as_shape = 0;
    if(!type3 && !ttf) {msg("<warning> No type3 enabled. Drawing char %d as shape", glyphnr);as_shape=1;}
    if(glyphnr>256-32) {msg("<warning> Drawing char %d as shape (not < 224)", glyphnr);as_shape=1;}
	
    if(as_shape) {
	reset_matrix(i);
	PDF_setrgbcolor_fill(i->p, color->r/255.0, color->g/255.0, color->b/255.0);
	gfxline_t*line2 = gfxline_clone(glyph->line);
	gfxline_transform(line2, matrix);
	if(mkline(line2, i->p, i->config_xpad, i->config_ypad, 1.0, 1)) {
	    PDF_fill(i->p);
	}
	gfxline_free(line2);
    } else {
	assert(gfxfontlist_hasfont(i->fontlist, font));
	int fontid = (int)(ptroff_t)gfxfontlist_getuserdata(i->fontlist, font->id);

	gfxmatrix_t m = *matrix;

	m.m00*=64;
	m.m01*=64;
	m.m10*=64;
	m.m11*=64;
	if(ttf) {
	    m.m10 = -m.m10;
	    m.m11 = -m.m11;
	}

	if(!(fabs(m.m00 - i->m00) < 1e-6 &&
	     fabs(m.m01 - i->m01) < 1e-6 &&
	     fabs(m.m10 - i->m10) < 1e-6 &&
	     fabs(m.m11 - i->m11) < 1e-6)) {
	    set_matrix(i, m.m00, m.m01, m.m10, m.m11);
	}
	double tx, ty;
	transform_back(i, m.tx+i->config_xpad, m.ty+i->config_ypad, &tx, &ty);
	
	PDF_setfont(i->p, fontid, ttf?16.0:1.0);
	PDF_setrgbcolor_fill(i->p, color->r/255.0, color->g/255.0, color->b/255.0);

	char name[32];
	sprintf(name, "%c", glyphnr+32);

	if(fabs(tx - i->lastx) > 0.001 || ty != i->lasty) {
	    PDF_show_xy2(i->p, name, strlen(name), tx, ty);
	} else {
	    PDF_show2(i->p, name, strlen(name));
	}

	i->lastx = tx + glyph->advance;
	i->lasty = ty;
    }
}

void pdf_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text)
{
    internal_t*i = (internal_t*)dev->internal;
}

void pdf_endpage(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    restore_matrix(i);
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

    if(i->config_addblankpages) {
	int mod = i->num_pages%i->config_addblankpages;
	if(mod) {
	    int count = i->config_addblankpages - mod;
	    int t;
	    for(t=0;t<count;t++) {
		dev->startpage(dev, i->width, i->height);
		dev->endpage(dev);
	    }
	}
    }
   
    PDF_end_document(i->p, "");
    //PDF_close(i->p);
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

    i->page_opts = "";
    i->lastx = -1e38;
    i->lasty = -1e38;
    i->has_matrix = 0;
    i->p = PDF_new();
}
