#include "../../config.h"
#include "Object.h"
#include "InfoOutputDev.h"
#include "SplashOutputDev.h"
#include "GfxState.h"
#include "CommonOutputDev.h"
#include "../log.h"
#include "../types.h"
#include "../q.h"
#include "../gfxdevice.h"
#include "../gfxfont.h"
#include <math.h>
#include <assert.h>

int config_unique_unicode = 1;
int config_poly2bitmap_pass1  = 0;
int config_skewedtobitmap_pass1 = 0;
int config_addspace = 1;
int config_fontquality = 10;
int config_bigchar = 0;
int config_marker_glyph = 0;
int config_normalize_fonts = 0;
int config_remove_font_transforms = 0;
int config_remove_invisible_outlines = 0;

static void* fontclass_clone(const void*_m) {
    if(_m==0) 
        return 0;
    const fontclass_t*m1=(const fontclass_t*)_m;
    fontclass_t*m2 = (fontclass_t*)malloc(sizeof(fontclass_t));
    *m2 = *m1;
    m2->id = strdup(m1->id);
    return m2;
}
static unsigned int fontclass_hash(const void*_m) {
    if(!_m)
        return 0;
    const fontclass_t*m = (fontclass_t*)_m;
    unsigned int h=0;
    if(config_remove_font_transforms) {
	U32 m00 = (*(U32*)&m->m00)&0xfff00000;
	U32 m01 = (*(U32*)&m->m01)&0xfff00000;
	U32 m10 = (*(U32*)&m->m10)&0xfff00000;
	U32 m11 = (*(U32*)&m->m11)&0xfff00000;
	h = crc32_add_bytes(h, (char*)&m00, sizeof(m00));
	h = crc32_add_bytes(h, (char*)&m01, sizeof(m01));
	h = crc32_add_bytes(h, (char*)&m10, sizeof(m10));
	h = crc32_add_bytes(h, (char*)&m11, sizeof(m11));
    }
    if(config_remove_invisible_outlines) {
	h = crc32_add_bytes(h, (char*)&m->alpha, 1);
    }
    return crc32_add_string(h, m->id);
}
static void fontclass_destroy(void*_m) {
    fontclass_t*m = (fontclass_t*)_m;
    free(m->id);m->id=0;
    free(m);
}
static char fontclass_equals(const void*_m1, const void*_m2) {
    const fontclass_t*m1=(const fontclass_t*)_m1;
    const fontclass_t*m2=(const fontclass_t*)_m2;
    if(!m1 || !m2) 
        return m1==m2;

    if(config_remove_font_transforms) {
	/* we do a binary comparison of the float32
	   bits here instead of a numerical comparison
	   to prevent the compiler from e.g. removing the
	   (float) cast during optimization, which would break
	   the equivalence between equals() and hash() (as
	   the hash is derived from the float32 values) */
	if(((*(U32*)&m1->m00 ^ *(U32*)&m2->m00)&0xfff00000) ||
           ((*(U32*)&m1->m01 ^ *(U32*)&m2->m01)&0xfff00000) ||
           ((*(U32*)&m1->m10 ^ *(U32*)&m2->m10)&0xfff00000) ||
           ((*(U32*)&m1->m11 ^ *(U32*)&m2->m11)&0xfff00000))
	    return 0;
    }
    if(config_remove_invisible_outlines) {
	if(m1->alpha != m2->alpha)
	    return 0;
    }
    return !strcmp(m1->id, m2->id);
}

static type_t fontclass_type = {
    fontclass_equals,
    fontclass_hash,
    fontclass_clone,
    fontclass_destroy
};

InfoOutputDev::InfoOutputDev(XRef*xref) 
{
    num_links = 0;
    num_jpeg_images = 0;
    num_ppm_images = 0;
    num_chars = 0;
    num_fonts = 0;
    num_polygons= 0;
    num_layers = 0;
    num_text_breaks = 0;
    currentglyph = 0;
    previous_was_char = 0;
    SplashColor white = {255,255,255};
    splash = new SplashOutputDev(splashModeRGB8,320,0,white,0,0);
    splash->startDoc(xref);
    last_font = 0;
    current_type3_font = 0;
    fontcache = dict_new2(&fontclass_type);
}
InfoOutputDev::~InfoOutputDev() 
{
    GHashIter*i;
    
    DICT_ITERATE_DATA(this->fontcache, FontInfo*, fd) {
	delete fd;
    }
    dict_destroy(this->fontcache);this->fontcache=0;

    delete splash;splash=0;
}

void FontInfo::grow(int size)
{
    if(size >= this->num_glyphs) {
	this->glyphs = (GlyphInfo**)realloc(this->glyphs, sizeof(GlyphInfo*)*(size));
	memset(&this->glyphs[this->num_glyphs], 0, sizeof(SplashPath*)*((size)-this->num_glyphs));
	this->num_glyphs = size;
    }
}
FontInfo::FontInfo(fontclass_t*fontclass)
{
    if(config_remove_font_transforms) {
	char buf[128];
	static int counter=1;
	sprintf(buf, "font%d", counter++);
	this->id = strdup(buf);
    } else {
	this->id = strdup(fontclass->id);
    }

    this->fontclass = (fontclass_t*)fontclass_type.dup(fontclass);
    this->seen = 0;
    this->num_glyphs = 0;
    this->glyphs = 0;
    this->gfxfont = 0;
    this->space_char = -1;
    this->ascender = 0;
    this->descender = 0;
    this->scale = 1.0;
    resetPositioning();
}
FontInfo::~FontInfo()
{
    if(this->id) {free(this->id);this->id=0;}
    this->font = 0;
    int t;
    for(t=0;t<num_glyphs;t++) {
	if(glyphs[t]) {
	    delete glyphs[t]->path;glyphs[t]->path = 0;
	    delete glyphs[t];
	    glyphs[t]=0;
	}
    }
    free(glyphs);glyphs=0;
    if(this->gfxfont)
        gfxfont_free(this->gfxfont);

    if(this->fontclass) {
	fontclass_type.free(this->fontclass);
	this->fontclass=0;
    }
}

void FontInfo::resetPositioning()
{
    this->lastchar = -1;
    this->lastx = 0;
    this->lasty = 0;
    this->lastadvance = 0;
}

static int findSpace(gfxfont_t*font)
{
    int first_space = -1;
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	gfxglyph_t*g = &font->glyphs[t];
	if(GLYPH_IS_SPACE(g)) {
	    if(g->unicode == 32) {
		/* now that we have found a space char, make sure it's unique */
		int s;
		for(s=0;s<font->num_glyphs;s++) {
		    if(s!=t && font->glyphs[s].unicode==32)
			font->glyphs[s].unicode=0;
		}
		return t;
	    }
	}
    }
    return -1;
}

static int addSpace(gfxfont_t*font)
{
    /* first, make sure the new space char is the only char that'll use unicode 32 */
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	if(font->glyphs[t].unicode==32)
	    font->glyphs[t].unicode=0;
    }
    // createGfxFont reserves space for up to two extra characters, so we don't need to realloc.
    int space_glyph = font->num_glyphs++;
    gfxglyph_t*g = &font->glyphs[space_glyph];
    memset(g, 0, sizeof(*g));
    g->unicode = 32;
    g->advance = fabs(font->ascent + font->descent) / 5.0;
    if(font->max_unicode > 32)
	font->unicode2glyph[32] = space_glyph;
#if 0
    g->line = gfxline_makerectangle(0, -font->ascent, g->advance, font->descent);
#endif
    return space_glyph;
}

static void transform_glyph(gfxglyph_t*g, fontclass_t*mm, double scale)
{
    gfxmatrix_t m;
    m.m00 = mm->m00 * scale;
    m.m01 = mm->m01 * scale;
    m.m10 = mm->m10 * scale;
    m.m11 = mm->m11 * scale;
    m.tx = 0;
    m.ty = 0;
    if(m.m00>0)
	g->advance *= m.m00;
    g->line = gfxline_clone(g->line);
    gfxline_transform(g->line, &m);
}

void gfxfont_transform(gfxfont_t*font, gfxmatrix_t*m)
{
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	gfxglyph_t*g = &font->glyphs[t];
	gfxline_t*line = g->line;
	gfxline_transform(line, m);
	if(m->m00>0)
	    g->advance *= m->m00;
    }
}

gfxbbox_t gfxfont_bbox(gfxfont_t*font)
{
    gfxbbox_t tmp = {0,0,0,0};
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	gfxline_t*line = font->glyphs[t].line;
	gfxbbox_t b = gfxline_getbbox(line);
	tmp = gfxbbox_expand_to_bbox(tmp, b);
    }
    return tmp;
}

gfxfont_t* FontInfo::createGfxFont()
{
    gfxfont_t*font = (gfxfont_t*)rfx_calloc(sizeof(gfxfont_t));

    font->glyphs = (gfxglyph_t*)malloc(sizeof(gfxglyph_t)*(this->num_glyphs+2));
    memset(font->glyphs, 0, sizeof(gfxglyph_t)*this->num_glyphs);
    font->id = 0;
    int t;

    double quality = (INTERNAL_FONT_SIZE * 200 / config_fontquality) / this->max_size;
    //printf("%d glyphs\n", font->num_glyphs);
    font->num_glyphs = 0;
    font->ascent = fabs(this->ascender);
    font->descent = fabs(this->descender);

    for(t=0;t<this->num_glyphs;t++) {
	if(this->glyphs[t]) {
	    SplashPath*path = this->glyphs[t]->path;
	    int len = path?path->getLength():0;
	    //printf("glyph %d) %08x (%d line segments)\n", t, path, len);
	    gfxglyph_t*glyph = &font->glyphs[font->num_glyphs];
	    this->glyphs[t]->glyphid = font->num_glyphs;
	    glyph->unicode = this->glyphs[t]->unicode;
	    gfxdrawer_t drawer;
	    gfxdrawer_target_gfxline(&drawer);
	    int s;
	    int count = 0;
	    double xmax = 0;
	    for(s=0;s<len;s++) {
		Guchar f;
		double x, y;
		path->getPoint(s, &x, &y, &f);
		if(!s || x > xmax)
		    xmax = x;
		if(f&splashPathFirst) {
		    drawer.moveTo(&drawer, x, y);
		}
		if(f&splashPathCurve) {
		    double x2,y2;
		    path->getPoint(++s, &x2, &y2, &f);
		    if(f&splashPathCurve) {
			double x3,y3;
			path->getPoint(++s, &x3, &y3, &f);
			gfxdraw_cubicTo(&drawer, x, y, x2, y2, x3, y3, quality);
		    } else {
			drawer.splineTo(&drawer, x, y, x2, y2);
		    }
		} else {
		    drawer.lineTo(&drawer, x, y);
		}
	     //   printf("%f %f %s %s\n", x, y, (f&splashPathCurve)?"curve":"",
	     //       			  (f&splashPathFirst)?"first":"",
	     //       			  (f&splashPathLast)?"last":"");
	    }

	    glyph->line = (gfxline_t*)drawer.result(&drawer);
	    if(this->glyphs[t]->advance>0) {
		glyph->advance = this->glyphs[t]->advance;
	    } else {
		glyph->advance = fmax(xmax, 0);
	    }
	    if(config_bigchar) {
		double max = this->glyphs[t]->advance_max;
		if(max>0 && max > glyph->advance) {
		    glyph->advance = max;
		}
	    }

	    font->num_glyphs++;
	}
    }
  
    if(config_remove_font_transforms) {
	gfxmatrix_t glyph_transform;
	glyph_transform.m00 = fontclass->m00;
	glyph_transform.m01 = fontclass->m01;
	glyph_transform.m10 = fontclass->m10;
	glyph_transform.m11 = fontclass->m11;
	glyph_transform.tx = 0;
	glyph_transform.ty = 0;
	/* apply the font transformation to the font */
	gfxfont_transform(font, &glyph_transform);

	gfxbbox_t total = gfxfont_bbox(font);
	font->ascent = total.ymax;
	font->descent = -total.ymin;
    }

    if(config_normalize_fonts) {
	/* make all chars 1024 high */
	gfxbbox_t bbox = gfxfont_bbox(font);
	double height = bbox.ymax - bbox.ymin;
	double scale = 1.0;
	if(height>1e-5) {
	    scale = 1024.0 / height;
	}
	this->scale = 1.0 / scale;
	gfxmatrix_t scale_matrix = {scale,0,0,
	                            0,scale,0};
	gfxfont_transform(font, &scale_matrix);
	font->ascent *= scale;
	font->descent *= scale;
    }
    
    if(config_remove_invisible_outlines) {
	/* for OCR docs: remove the outlines of characters that are only
	   ever displayed with alpha=0 */
	if(!fontclass->alpha) {
	    for(t=0;t<font->num_glyphs;t++) {
		gfxglyph_t*g = &font->glyphs[t];
		gfxline_t*line = font->glyphs[t].line;
		gfxline_free(g->line);
		g->line = (gfxline_t*)rfx_calloc(sizeof(gfxline_t));
		g->line->type = gfx_moveTo;
		g->line->x = g->advance;
	    }
	}
    }

    return font;
}

static float find_average_glyph_advance(gfxfont_t*f)
{
    if(!f->num_glyphs)
	return 0.0;

    float*values = (float*)malloc(sizeof(float)*f->num_glyphs);
    int t;
    for(t=0;t<f->num_glyphs;t++) {
	values[t] = f->glyphs[t].advance;
    }
    float m = medianf(values, f->num_glyphs);
    free(values);
    return m;
}

gfxfont_t* FontInfo::getGfxFont()
{
    if(!this->gfxfont) {
        this->gfxfont = this->createGfxFont();
        this->gfxfont->id = strdup(this->id);
	this->space_char = findSpace(this->gfxfont);
	this->average_advance = find_average_glyph_advance(this->gfxfont);

	if(this->space_char>=0) {
	    msg("<debug> Font %s has space char %d (unicode=%d)", 
		    this->id, this->space_char, 
		    this->gfxfont->glyphs[this->space_char].unicode);
	} else if(config_addspace) {
	    this->space_char = addSpace(this->gfxfont);
	    msg("<debug> Appending space char to font %s, position %d, width %f", this->gfxfont->id, this->space_char, this->gfxfont->glyphs[this->space_char].advance);
	}
	gfxfont_fix_unicode(this->gfxfont, config_unique_unicode);
    
	/* optionally append a marker glyph */
	if(config_marker_glyph) {
	    msg("<debug> Appending marker char to font %s, position %d, unicode %d", this->gfxfont->id, this->gfxfont->num_glyphs, config_marker_glyph);
	    gfxglyph_t*g = &this->gfxfont->glyphs[this->gfxfont->num_glyphs++];
	    g->name = 0;
	    g->unicode = config_marker_glyph;
	    g->advance = 2048;
	    g->line = (gfxline_t*)rfx_calloc(sizeof(gfxline_t));
	    g->line->type = gfx_moveTo;
	    g->line->x = g->advance;
	}
    }
    return this->gfxfont;
}

GBool InfoOutputDev::upsideDown() {return gTrue;}
GBool InfoOutputDev::useDrawChar() {return gTrue;}
GBool InfoOutputDev::interpretType3Chars() {return gTrue;}
GBool InfoOutputDev::useTilingPatternFill() {return gFalse;}

GBool InfoOutputDev::checkPageSlice(Page *page, double hDPI, double vDPI,
             int rotate, GBool useMediaBox, GBool crop,
             int sliceX, int sliceY, int sliceW, int sliceH,
             GBool printing, Catalog *catalog,
             GBool (*abortCheckCbk)(void *data),
             void *abortCheckCbkData)
{
    this->page = page;
    return gTrue;
}

void InfoOutputDev::startPage(int pageNum, GfxState *state)
{
    PDFRectangle *r = this->page->getCropBox();
    double x1,y1,x2,y2;
    state->transform(r->x1,r->y1,&x1,&y1);
    state->transform(r->x2,r->y2,&x2,&y2);
    if(x2<x1) {double x3=x1;x1=x2;x2=x3;}
    if(y2<y1) {double y3=y1;y1=y2;y2=y3;}
    this->x1 = (int)x1;
    this->y1 = (int)y1;
    this->x2 = (int)x2;
    this->y2 = (int)y2;
    msg("<verbose> Generating info structure for page %d", pageNum);
    num_links = 0;
    num_jpeg_images = 0;
    num_ppm_images = 0;
    num_chars = 0;
    num_fonts = 0;
    num_polygons= 0;
    num_layers = 0;
    average_char_size = 0;
}
void InfoOutputDev::endPage()
{
    if(num_chars) 
	average_char_size /= num_chars;
}
void InfoOutputDev::drawLink(Link *link, Catalog *catalog) 
{
    num_links++;
}
   
/*  } else if(!strcmp(key,"fontquality")) {
        this->config_fontquality = atof(value);
	if(this->config_fontquality<=1)
	    this->config_fontquality=1;
    } else if(!strcmp(key,"bigchar")) {
        this->config_bigchar = atoi(value);
    }
 */

char*getFontID(GfxFont*font)
{
    Ref*ref = font->getID();
    GString*gstr = font->getName();
    char* fname = gstr==0?0:gstr->getCString();
    char buf[128];
    if(fname==0) {
	if(font->getType() == fontType3) {
	    sprintf(buf, "t3font-%d-%d", ref->num, ref->gen);
	} else {
	    sprintf(buf, "font-%d-%d", ref->num, ref->gen);
	}
    } else {
	sprintf(buf, "%s-%d-%d", fname, ref->num, ref->gen);
    }
    return strdup(buf);
}

gfxmatrix_t gfxmatrix_from_state(GfxState*state)
{
    double* ctm = state->getCTM();
    double fontSize = state->getFontSize();
    double*textMat = state->getTextMat();

    /*  taking the absolute value of horizScaling seems to be required for
       some italic fonts. FIXME: SplashOutputDev doesn't need this- why? */
    double hscale = fabs(state->getHorizScaling());
   
    // from xpdf-3.02/SplashOutputDev:updateFont
    double mm11 = textMat[0] * fontSize * hscale;
    double mm12 = textMat[1] * fontSize * hscale;
    double mm21 = textMat[2] * fontSize;
    double mm22 = textMat[3] * fontSize;

    // multiply with ctm, like state->getFontTransMat() does
    gfxmatrix_t m;
    m.m00 = (ctm[0]*mm11 + ctm[2]*mm12) / INTERNAL_FONT_SIZE;
    m.m01 = (ctm[1]*mm11 + ctm[3]*mm12) / INTERNAL_FONT_SIZE;
    m.m10 = (ctm[0]*mm21 + ctm[2]*mm22) / INTERNAL_FONT_SIZE;
    m.m11 = (ctm[1]*mm21 + ctm[3]*mm22) / INTERNAL_FONT_SIZE;
    m.tx = 0;
    m.ty = 0;
    return m;
}

void InfoOutputDev::updateTextMat(GfxState*state)
{
}

GBool InfoOutputDev::needNonText() 
{ 
    /* this switches off certain expensive operations, like
       pattern fill and forms */
    return gFalse; 
}

void InfoOutputDev::updateFont(GfxState *state) 
{
    GfxFont*font = state->getFont();
    if(!font) {
	current_splash_font = 0;
	return;
    }
    if(font->getType() == fontType3) {
	current_splash_font = 0;
	return;
    }
    GfxState* state2 = state->copy();
    state2->setPath(0);
    state2->setCTM(1.0,0,0,1.0,0,0);
    splash->updateCTM(state2, 0,0,0,0,0,0);
    state2->setTextMat(1.0,0,0,1.0,0,0);
    state2->setFont(font, 1024.0);
    splash->doUpdateFont(state2);

    current_splash_font = splash->getCurrentFont();
    delete state2;
}

double matrix_scale_factor(gfxmatrix_t*m)
{
    double l1 = sqrt(m->m00 * m->m00 + m->m01 * m->m01);
    double l2 = sqrt(m->m10 * m->m10 + m->m11 * m->m11);
    return (l1+l2)/2.0;
}

#ifdef __GNUC__
int __attribute__((noinline)) 
     font_classify(fontclass_t*out, gfxmatrix_t*in, const char*id, gfxcolor_t* color)
#else
int font_classify(fontclass_t*out, gfxmatrix_t*in, const char*id, gfxcolor_t* color)
#endif
{
    out->id = (char*)id;

    if(!config_remove_font_transforms) {
	out->m00 = 1.0;
	out->m11 = 1.0;
	out->m01 = 0.0;
	out->m10 = 0.0;
    } else {
	double l = matrix_scale_factor(in);
	if(l < 1e-10) {
	    /* treat all singularity characters the same */
	    memset(out, 0, sizeof(*out));
	    l = 0;
	} else {
	    out->m00 = in->m00 / l;
	    out->m10 = in->m10 / l;
	    out->m01 = -in->m01 / l;
	    out->m11 = -in->m11 / l;
	}
	
	if(!color->a) {
	/* for invisible characters, transforms don't need to be that
	   precise- use only 3 bits precision for mantissa. */
	
	/* 0x80000000 //sign
	   0x78000000 //exponent
	   0x07ffffff //mantissa */
	    *(U32*)&out->m00 = (*(U32*)&out->m00)&0xffe00000;
	    *(U32*)&out->m01 = (*(U32*)&out->m01)&0xffe00000;
	    *(U32*)&out->m10 = (*(U32*)&out->m10)&0xffe00000;
	    *(U32*)&out->m11 = (*(U32*)&out->m11)&0xffe00000;
	}
    }
    out->alpha = color->a?1:0;

    return 1;
}

void fontclass_print(fontclass_t*cls)
{
    printf("[%f %f %f %f] %s alpha=%d\n", 
	    cls->m00, cls->m01, cls->m10, cls->m11,
	    cls->id,
	    cls->alpha
	    );
}

gfxcolor_t gfxstate_getfontcolor(GfxState*state)
{
    /* FIXME: instead of duplicating BitmapOutputDev's and VectorOutputDev's transparent
              character logic here, we should move this code to CommonOutputDev and
	      call it from all three places */

    gfxcolor_t col = gfxstate_getfillcolor(state);
    /* HACK: if skewedtobitmap is on, weirdly rotated characters will 
       be drawn transparently in BitmapOutputDev. In order to anticipate this,
       we duplicate the logic here */
    if(config_remove_invisible_outlines && 
       config_skewedtobitmap_pass1 && 
       text_matrix_is_skewed(state)) {
    	col.a = 0;
    }
    if(state->getRender() == RENDER_INVISIBLE) {
	col.a = 0;
    }
    if(config_poly2bitmap_pass1 && (state->getRender()&3)) {
	/* with poly2bitmap, stroke or stroke+fill characters are drawn
	   to the bitmap and potentially overlaid with a transparent character.
	   duplicate that logic here. */
	col.a = 0;
    }
    return col;
}

static inline fontclass_t fontclass_from_state(GfxState*state)
{
    fontclass_t cls;
    gfxcolor_t col = gfxstate_getfontcolor(state);
    char*id = getFontID(state->getFont());
    gfxmatrix_t m = gfxmatrix_from_state(state);
    font_classify(&cls, &m, id, &col);
    return cls;
}
static inline void fontclass_clear(fontclass_t*cls)
{
    free(cls->id);cls->id=0;
}

FontInfo* InfoOutputDev::getOrCreateFontInfo(GfxState*state)
{
    GfxFont*font = state->getFont();
    fontclass_t fontclass = fontclass_from_state(state);

    FontInfo* fontinfo = (FontInfo*)dict_lookup(this->fontcache, &fontclass);
    if(!fontinfo) {
	fontinfo = new FontInfo(&fontclass);
	dict_put(this->fontcache, &fontclass, fontinfo);
	fontinfo->font = font;
	fontinfo->max_size = 0;
	if(current_splash_font) {
	    fontinfo->ascender = current_splash_font->ascender;
	    fontinfo->descender = current_splash_font->descender;
	} else {
	    fontinfo->ascender = fontinfo->descender = 0;
	}
	num_fonts++;
    }

    if(last_font && fontinfo!=last_font) {
	last_font->resetPositioning();
    }

    this->last_font = fontinfo;
    fontclass_clear(&fontclass);
    return fontinfo;
}

FontInfo* InfoOutputDev::getFontInfo(GfxState*state)
{
    fontclass_t fontclass = fontclass_from_state(state);
    FontInfo*result = (FontInfo*)dict_lookup(this->fontcache, &fontclass);
    if(!result) {
	printf("NOT FOUND: ");
	fontclass_print(&fontclass);
    }
    fontclass_clear(&fontclass);
    return result;
}

gfxmatrix_t FontInfo::get_gfxmatrix(GfxState*state)
{
    gfxmatrix_t m = gfxmatrix_from_state(state);
    if(!config_remove_font_transforms) {
	return m;
    } else {
	double scale = matrix_scale_factor(&m) * this->scale;
	gfxmatrix_t m = {scale,  0, 0, 
	                 0, -scale, 0};
	return m;
    }
}

void InfoOutputDev::drawChar(GfxState *state, double x, double y,
		      double dx, double dy,
		      double originX, double originY,
		      CharCode code, int nBytes, Unicode *u, int uLen)
{
    double m11,m21,m12,m22;
    state->getFontTransMat(&m11, &m12, &m21, &m22);
    m11 *= state->getHorizScaling();
    m21 *= state->getHorizScaling();
    double lenx = sqrt(m11*m11 + m12*m12);
    double leny = sqrt(m21*m21 + m22*m22);
    double len = lenx>leny?lenx:leny;

    FontInfo*fontinfo = getOrCreateFontInfo(state);

    if(!fontinfo) {
	msg("<error> Internal error: No fontinfo for font");
	return; //error
    }
    if(!current_splash_font) {
	msg("<error> Internal error: No current splash fontinfo");
	return; //error
    }
    if(fontinfo && fontinfo->max_size < len) {
	fontinfo->max_size = len;
    }
    
    average_char_size += fmax(lenx,leny);
    num_chars++;

    if(!previous_was_char)
	num_layers++;
    previous_was_char=1;

    fontinfo->grow(code+1);
    GlyphInfo*g = fontinfo->glyphs[code];
    if(!g) {
	g = fontinfo->glyphs[code] = new GlyphInfo();
	g->advance_max = 0;
	current_splash_font->last_advance = -1;
	g->path = current_splash_font->getGlyphPath(code);
	g->advance = current_splash_font->last_advance;
	g->unicode = 0;
    }
    if(uLen && ((u[0]>=32 && u[0]<g->unicode) || !g->unicode)) {
	g->unicode = u[0];
    }
    if(fontinfo->lastchar>=0 && fontinfo->lasty == y) {
	double xshift = (x - fontinfo->lastx);
	if(xshift>=0 && xshift > g->advance_max) {
	    g->advance_max = xshift;
	}
    } else {
	num_text_breaks++;
    }

    fontinfo->lastx = x;
    fontinfo->lasty = y;
    fontinfo->lastchar = code;
    fontinfo->lastadvance = g->advance;
}

static char path_is_rectangular(GfxState* state)
{
    GfxPath * path = state->getPath();
    int num = path->getNumSubpaths();
    if(num!=1) return 0;
    GfxSubpath*subpath = path->getSubpath(0);
    int subnum = path->getSubpath(0)->getNumPoints();

    if(subnum>5) return 0;
    int s;
    for(s=1;s<subnum;s++) {
	if(subpath->getCurve(s)) 
	    return 0;
	if(subpath->getX(s) != subpath->getX(s-1) &&
           subpath->getY(s) != subpath->getY(s-1)) {
	    return 0;
	}
    }
    return 1;
}

void InfoOutputDev::fill(GfxState *state)
{
    if(!path_is_rectangular(state))
	previous_was_char=0;
    num_polygons++;
}

void InfoOutputDev::eoFill(GfxState *state)
{
    if(!path_is_rectangular(state))
	previous_was_char=0;
    num_polygons++;
}

GBool InfoOutputDev::beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen)
{
    GfxFont*font = state->getFont();
    if(!font)
	return gTrue;
    if(font->getType() != fontType3)
	return gTrue;

    current_splash_font = 0;

    fontclass_t fontclass = fontclass_from_state(state);
    FontInfo* fontinfo = (FontInfo*)dict_lookup(this->fontcache, &fontclass);
    if(!fontinfo) {
	fontinfo = new FontInfo(&fontclass);
	dict_put(this->fontcache, &fontclass, fontinfo);
	fontinfo->font = font;
	fontinfo->max_size = 0;
	num_fonts++;
    }
    fontclass_clear(&fontclass);

    current_type3_font = fontinfo;
    fontinfo->grow(code+1);
    if(!fontinfo->glyphs[code]) {
	currentglyph = fontinfo->glyphs[code] = new GlyphInfo();
	currentglyph->unicode = uLen?u[0]:0;
	currentglyph->path = new SplashPath();
	currentglyph->x1=0;
	currentglyph->y1=0;
	currentglyph->x2=dx;
	currentglyph->y2=dy;
	currentglyph->advance=dx;
	return gFalse;
    } else {
	return gTrue;
    }
}

void InfoOutputDev::type3D0(GfxState *state, double wx, double wy)
{
    currentglyph->x1=0;
    currentglyph->y1=0;
    currentglyph->x2=wx;
    currentglyph->y2=wy;
}

void InfoOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury)
{
    if(-lly>current_type3_font->descender)
	current_type3_font->descender = -lly;
    if(ury>current_type3_font->ascender)
	current_type3_font->ascender = ury;

    currentglyph->x1=llx;
    currentglyph->y1=lly;
    currentglyph->x2=urx;
    currentglyph->y2=ury;
}

void InfoOutputDev::endType3Char(GfxState *state)
{
    double x1 = currentglyph->x1;
    double y1 = currentglyph->y1;
    double x2 = currentglyph->x2;
    double y2 = currentglyph->y2;
    currentglyph->path->moveTo(x1,y1);
    currentglyph->path->lineTo(x2,y1);
    currentglyph->path->lineTo(x2,y2);
    currentglyph->path->lineTo(x1,y2);
    currentglyph->path->close();
}
    
void InfoOutputDev::saveState(GfxState *state)
{
    updateAll(state);
}

void InfoOutputDev::restoreState(GfxState *state)
{
    updateAll(state);
}

void InfoOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
			   int width, int height, GBool invert,
			   POPPLER_INTERPOLATE
			   GBool inlineImg) 
{
    previous_was_char=0;
    if(str->getKind()==strDCT) num_jpeg_images++; else num_ppm_images++;

    OutputDev::drawImageMask(state,ref,str,width,height,invert, POPPLER_INTERPOLATE_ARG inlineImg);
}
void InfoOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
		       int width, int height, GfxImageColorMap *colorMap,
		       POPPLER_INTERPOLATE
		       int *maskColors, GBool inlineImg)
{
    previous_was_char=0;
    if(str->getKind()==strDCT) num_jpeg_images++; else num_ppm_images++;

    OutputDev::drawImage(state,ref,str,width,height,colorMap, POPPLER_INTERPOLATE_ARG maskColors,inlineImg);
}
void InfoOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
				int width, int height,
				GfxImageColorMap *colorMap,
				POPPLER_INTERPOLATE
				Stream *maskStr,
				int maskWidth, int maskHeight,
				GBool maskInvert
				POPPLER_MASK_INTERPOLATE)
{
    previous_was_char=0;
    if(str->getKind()==strDCT) num_jpeg_images++; else num_ppm_images++;

    OutputDev::drawMaskedImage(state,ref,str,width,height,colorMap, POPPLER_INTERPOLATE_ARG maskStr,maskWidth,maskHeight,maskInvert POPPLER_MASK_INTERPOLATE_ARG);
}

void InfoOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				    int width, int height,
				    GfxImageColorMap *colorMap,
				    POPPLER_INTERPOLATE
				    Stream *maskStr,
				    int maskWidth, int maskHeight,
				    GfxImageColorMap *maskColorMap
				    POPPLER_MASK_INTERPOLATE)
{
    previous_was_char=0;
    if(str->getKind()==strDCT) num_jpeg_images++; else num_ppm_images++;

    OutputDev::drawSoftMaskedImage(state,ref,str,width,height,colorMap, POPPLER_INTERPOLATE_ARG maskStr,maskWidth,maskHeight,maskColorMap POPPLER_MASK_INTERPOLATE_ARG);
}
    
void InfoOutputDev::dumpfonts(gfxdevice_t*dev)
{
    GHashIter*i;
    GString*key;

    DICT_ITERATE_DATA(fontcache, FontInfo*, info) {
        dev->addfont(dev, info->getGfxFont());
    }
}
