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
#include <math.h>
#include <memory.h>
#include "../gfxfilter.h"
#include "../gfxtools.h"
#include "../gfxfont.h"
#include "../types.h"
#include "../mem.h"
#include "../q.h"
#include "../log.h"

typedef struct _mymatrix {
    float m00,m01,m10,m11;
    char*id;
    unsigned char alpha;
} mymatrix_t;

static void* mymatrix_clone(const void*_m) {
    if(_m==0) 
        return 0;
    const mymatrix_t*m1=_m;
    mymatrix_t*m2 = malloc(sizeof(mymatrix_t));
    *m2 = *m1;
    m2->id = strdup(m1->id);
    return m2;
}
static unsigned int mymatrix_hash(const void*_m) {
    if(!_m)
        return 0;
    const mymatrix_t*m = (mymatrix_t*)_m;
    unsigned int h=0;
    h = crc32_add_bytes(h, (char*)&m->m00, sizeof(m->m00));
    h = crc32_add_bytes(h, (char*)&m->m01, sizeof(m->m01));
    h = crc32_add_bytes(h, (char*)&m->m10, sizeof(m->m10));
    h = crc32_add_bytes(h, (char*)&m->m11, sizeof(m->m11));
    h = crc32_add_bytes(h, (char*)&m->alpha, 1);
    h = crc32_add_string(h, m->id);
    return h;
}
static void mymatrix_destroy(void*_m) {
    mymatrix_t*m = (mymatrix_t*)_m;
    free(m->id);m->id=0;
    free(m);
}
static char mymatrix_equals(const void*_m1, const void*_m2) {
    const mymatrix_t*m1=_m1;
    const mymatrix_t*m2=_m2;
    if(!m1 || !m2) 
        return m1==m2;
    
    /* we do a binary comparison of the float32
       bits here instead of a numerical comparison
       to prevent the compiler from e.g. removing the
       (float) cast during optimization, which would break
       the equivalence between equals() and hash() (as
       the hash is derived from the float32 values) */
    return *(U32*)&m1->m00 == *(U32*)&m2->m00 &&
           *(U32*)&m1->m01 == *(U32*)&m2->m01 &&
           *(U32*)&m1->m10 == *(U32*)&m2->m10 &&
           *(U32*)&m1->m11 == *(U32*)&m2->m11 &&
           m1->alpha == m2->alpha &&
	   !strcmp(m1->id, m2->id);
}
type_t mymatrix_type = {
    dup: mymatrix_clone,
    hash: mymatrix_hash,
    free: mymatrix_destroy,
    equals: mymatrix_equals
};

typedef struct _internal {
    dict_t*matrices;
    char first;
} internal_t;


#ifdef __GNUC__
int __attribute__((noinline)) 
     matrix_convert(gfxmatrix_t*in, const char*id, mymatrix_t*out, gfxmatrix_t*scalematrix, unsigned char alpha)
#else
int matrix_convert(gfxmatrix_t*in, const char*id, mymatrix_t*out, gfxmatrix_t*scalematrix, unsigned char alpha)
#endif
{
    double l1 = sqrt(in->m00 * in->m00 + in->m01 * in->m01);
    double l2 = sqrt(in->m10 * in->m10 + in->m11 * in->m11);
    double l = (l1+l2)/2.0;
    if(l < 1e-10) {
	memset(out, 0, sizeof(*out));
	return 0;
    }
    out->m00 = in->m00 / l;
    out->m10 = in->m10 / l;
    out->m01 = -in->m01 / l;
    out->m11 = -in->m11 / l;
    out->id = (char*)id;
    out->alpha = alpha?1:0;

    if(scalematrix) {
	scalematrix->m00 = l;
	scalematrix->m01 = 0;
	scalematrix->m10 = 0;
	scalematrix->m11 = -l;
	scalematrix->tx = in->tx;
	scalematrix->ty = in->ty;
    }
    return 1;
}

typedef struct _matrixdata {
    gfxfontlist_t*fonts;
} matrixdata_t;

typedef struct _transformedfont {
    gfxfont_t*orig;
    gfxfont_t*font;
    mymatrix_t matrix;
    int*used;
    double dx;
} transformedfont_t;

static transformedfont_t* transformedfont_new(gfxfont_t*orig, mymatrix_t*m)
{
    transformedfont_t*f = rfx_calloc(sizeof(transformedfont_t));
    f->orig = orig;
    f->matrix = *m;
    f->used = rfx_calloc(sizeof(f->used[0])*orig->num_glyphs);
    int t;
    for(t=0;t<orig->num_glyphs;t++) {
	if(orig->glyphs[t].unicode==32 && 
	   (!orig->glyphs[t].line || 
	    !orig->glyphs[t].line->next || 
	    !orig->glyphs[t].line->next->next))
	    f->used[t]=1; //always preserve the space char in fonts
    }
    return f;
}

static void pass1_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    mymatrix_t m;
    if(!font->id) 
	msg("<error> Font has no ID");
    if(!matrix_convert(matrix, font->id?font->id:"unknown", &m, 0, color->a))
	return;
    transformedfont_t*fd = dict_lookup(i->matrices, &m);
    if(!fd) {
	fd = transformedfont_new(font, &m);
	dict_put(i->matrices, &m, fd);
    }
    fd->used[glyphnr]=1;
    out->drawchar(out, font, glyphnr, color, matrix);
}

static void glyph_transform(gfxglyph_t*g, mymatrix_t*mm)
{
    gfxmatrix_t m;
    m.m00 = mm->m00;
    m.m01 = mm->m01;
    m.m10 = mm->m10;
    m.m11 = mm->m11;
    m.tx = 0;
    m.ty = 0;
    if(m.m00>0)
	g->advance *= m.m00;
    g->line = gfxline_clone(g->line);
    gfxline_transform(g->line, &m);
}

static gfxresult_t* pass1_finish(gfxfilter_t*f, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    DICT_ITERATE_DATA(i->matrices, transformedfont_t*, fd) {
	gfxfont_t*font = fd->font = rfx_calloc(sizeof(gfxfont_t));
	char id[80];
	static int fontcount=0;
	sprintf(id, "font%d", fontcount++);
	font->id = strdup(id);
	int t;
	int count=0;
	for(t=0;t<fd->orig->num_glyphs;t++) {
	    if(fd->used[t]) 
		count++;
	}
	font->num_glyphs = count;
	font->glyphs = rfx_calloc(sizeof(gfxglyph_t)*font->num_glyphs);
	count = 0;
	for(t=0;t<fd->orig->num_glyphs;t++) {
	    if(fd->used[t]) {
		font->glyphs[count] = fd->orig->glyphs[t];
		glyph_transform(&font->glyphs[count], &fd->matrix);
		fd->used[t] = count + 1;
		count++;
	    }
	}

	/* adjust the origin so that every character is to the
	   right of the origin */
	gfxbbox_t total = {0,0,0,0};
	double average_xmax = 0;
	for(t=0;t<count;t++) {
	    gfxline_t*line = font->glyphs[t].line;
	    gfxbbox_t b = gfxline_getbbox(line);
	    total = gfxbbox_expand_to_bbox(total, b);
	}
	if(count) 
	    average_xmax /= count;

	fd->dx = 0;//-total.xmin;

	font->ascent = total.ymax;
	font->descent = -total.ymin;

	for(t=0;t<count;t++) {
	    gfxglyph_t*g = &font->glyphs[t];
	    gfxline_t*line = font->glyphs[t].line;

	    if(fd->matrix.alpha) {
		while(line) {
		    line->x += fd->dx;
		    line->sx += fd->dx;
		    line = line->next;
		}
	    } else {
		gfxline_free(g->line);
		/* for OCR: remove the outlines of characters that are only
		   ever displayed with alpha=0 */
		g->line = (gfxline_t*)rfx_calloc(sizeof(gfxline_t));
		g->line->type = gfx_moveTo;
		g->line->x = g->advance;
	    }
	}

	gfxfont_fix_unicode(font, 1);
    }
    return out->finish(out);
}

static void pass2_addfont(gfxfilter_t*f, gfxfont_t*font, gfxdevice_t*out)
{
    /* we throw away original fonts, and do the addfont() for the transformed
       fonts in the first drawchar() */
}

static void pass2_drawchar(gfxfilter_t*f, gfxfont_t*font, int glyphnr, gfxcolor_t*_color, gfxmatrix_t*matrix, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    gfxcolor_t color = *_color;

    if(i->first) {
	i->first = 0;
	DICT_ITERATE_DATA(i->matrices, transformedfont_t*, fd) {
	    out->addfont(out, fd->font);
	}
    }

    mymatrix_t m;
    gfxmatrix_t scalematrix;
    matrix_convert(matrix, font->id?font->id:"unknown", &m, &scalematrix, color.a);
    transformedfont_t*d = dict_lookup(i->matrices, &m);
    scalematrix.tx -= d->dx*scalematrix.m00;

    /* if this character is invisible (alpha=0), then we will have removed the
       outline, so we make set the alpha color channel to "fully visible" again to allow
       output devices to be more performant (transparency is expensive) */
    if(!m.alpha) 
	color.a = 255;

    out->drawchar(out, d->font, d->used[glyphnr]-1, &color, &scalematrix);
}

static gfxresult_t* pass2_finish(gfxfilter_t*f, gfxdevice_t*out)
{
    internal_t*i = (internal_t*)f->internal;
    DICT_ITERATE_DATA(i->matrices, transformedfont_t*, fd) {
	if(fd->used) {
	    free(fd->used);fd->used=0;
	}
	free(fd);
    }
    dict_destroy(i->matrices);i->matrices=0;
    return out->finish(out);
}

void gfxtwopassfilter_remove_font_transforms_init(gfxtwopassfilter_t*f)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));

    memset(f, 0, sizeof(gfxtwopassfilter_t));
    f->type = gfxfilter_twopass;

    f->pass1.name = "remove font transforms pass 1";
    f->pass1.drawchar = pass1_drawchar;
    f->pass1.finish = pass1_finish;
    f->pass1.internal = i;

    f->pass2.name = "remove font transforms pass 2";
    f->pass2.addfont = pass2_addfont;
    f->pass2.drawchar = pass2_drawchar;
    f->pass2.finish = pass2_finish;
    f->pass2.internal = i;

    i->matrices = dict_new2(&mymatrix_type);
    i->first = 1;
}

