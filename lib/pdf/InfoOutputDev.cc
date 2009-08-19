#include "config.h"
#include "Object.h"
#include "InfoOutputDev.h"
#include "SplashOutputDev.h"
#ifdef HAVE_POPPLER
#include <splash/SplashTypes.h>
#include <splash/SplashPath.h>
#include <splash/SplashFont.h>
#include <splash/SplashFontFile.h>
#else
#include "SplashTypes.h"
#include "SplashPath.h"
#include "SplashFont.h"
#include "SplashFontFile.h"
#endif
#include "GfxState.h"
#include "../log.h"
#include <math.h>
#include <assert.h>

int config_addspace = 1;
int config_fontquality = 10;
int config_bigchar = 0;

InfoOutputDev::InfoOutputDev(XRef*xref) 
{
    num_links = 0;
    num_jpeg_images = 0;
    num_ppm_images = 0;
    num_textfields = 0;
    num_fonts = 0;
    num_polygons= 0;
    currentfont = 0;
    currentglyph = 0;
    id2font = new GHash(1);
    SplashColor white = {255,255,255};
    splash = new SplashOutputDev(splashModeRGB8,320,0,white,0,0);
    splash->startDoc(xref);
}
InfoOutputDev::~InfoOutputDev() 
{
    GHashIter*i;
    id2font->startIter(&i);
    GString*key;
    FontInfo*fontinfo;
    while(id2font->getNext(&i, &key, (void**)&fontinfo)) {
	delete fontinfo;
    }
    id2font->killIter(&i);

    delete id2font;id2font=0;
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
FontInfo::FontInfo(char*id)
{
    this->id = strdup(id);
    this->charid2glyph = 0;
    this->seen = 0;
    this->num_glyphs = 0;
    this->glyphs = 0;
    this->splash_font = 0;
    this->lastchar = -1;
    this->lastx = 0;
    this->lasty = 0;
    this->gfxfont = 0;
    this->space_char = -1;
}
FontInfo::~FontInfo()
{
    if(this->id) {free(this->id);this->id=0;}
    this->font = 0;
    if(this->charid2glyph) {
	free(this->charid2glyph);
	this->charid2glyph = 0;
    }
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
}

static int findSpace(gfxfont_t*font)
{
    int first_space = -1;
    int t;
    for(t=0;t<font->num_glyphs;t++) {
	gfxglyph_t*g = &font->glyphs[t];
	if(GLYPH_IS_SPACE(g)) {
	    if(g->unicode == 32) return t;
	    if(first_space<0)
		first_space = t;
	}
    }
    if(GLYPH_IS_SPACE(&font->glyphs[32])) {
	return 32;
    }
    return first_space;
}

static int addSpace(gfxfont_t*font)
{
    font->num_glyphs++;
    font->glyphs = (gfxglyph_t*)realloc(font->glyphs, sizeof(gfxglyph_t)*font->num_glyphs);
    gfxglyph_t*g = &font->glyphs[font->num_glyphs-1];
    memset(g, 0, sizeof(*g));
    g->unicode = 32;
    //g->advance = font->ascent;
    g->advance = fabs(font->ascent - font->descent)*2 / 3;
    if(font->max_unicode > 32)
	font->unicode2glyph[32] = font->num_glyphs-1;
#if 0
    g->line = gfxline_makerectangle(0, -font->ascent, g->advance, font->descent);
#endif
    return font->num_glyphs-1;
}

static gfxfont_t* createGfxFont(FontInfo*src)
{
    gfxfont_t*font = (gfxfont_t*)malloc(sizeof(gfxfont_t));
    memset(font, 0, sizeof(gfxfont_t));

    font->glyphs = (gfxglyph_t*)malloc(sizeof(gfxglyph_t)*src->num_glyphs);
    memset(font->glyphs, 0, sizeof(gfxglyph_t)*src->num_glyphs);
    font->id = 0;
    int t;

    double quality = (INTERNAL_FONT_SIZE * 200 / config_fontquality) / src->max_size;
    double scale = 1;
    //printf("%d glyphs\n", font->num_glyphs);
    font->num_glyphs = 0;
    font->ascent = fabs(src->descender);
    font->descent = fabs(src->ascender);
    
    for(t=0;t<src->num_glyphs;t++) {
	if(src->glyphs[t]) {
	    SplashPath*path = src->glyphs[t]->path;
	    int len = path?path->getLength():0;
	    //printf("glyph %d) %08x (%d line segments)\n", t, path, len);
	    gfxglyph_t*glyph = &font->glyphs[font->num_glyphs];
	    src->glyphs[t]->glyphid = font->num_glyphs;
	    glyph->unicode = src->glyphs[t]->unicode;
	    if(glyph->unicode >= font->max_unicode)
		font->max_unicode = glyph->unicode+1;
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
		    drawer.moveTo(&drawer, x*scale, y*scale);
		}
		if(f&splashPathCurve) {
		    double x2,y2;
		    path->getPoint(++s, &x2, &y2, &f);
		    if(f&splashPathCurve) {
			double x3,y3;
			path->getPoint(++s, &x3, &y3, &f);
			gfxdraw_cubicTo(&drawer, x*scale, y*scale, x2*scale, y2*scale, x3*scale, y3*scale, quality);
		    } else {
			drawer.splineTo(&drawer, x*scale, y*scale, x2*scale, y2*scale);
		    }
		} else {
		    drawer.lineTo(&drawer, x*scale, y*scale);
		}
	     //   printf("%f %f %s %s\n", x, y, (f&splashPathCurve)?"curve":"",
	     //       			  (f&splashPathFirst)?"first":"",
	     //       			  (f&splashPathLast)?"last":"");
	    }

	    glyph->line = (gfxline_t*)drawer.result(&drawer);
	    if(src->glyphs[t]->advance>0) {
		glyph->advance = src->glyphs[t]->advance;
	    } else {
		msg("<warning> Approximating advance value for glyph %d", t);
		glyph->advance = xmax*scale;
	    }
	    if(config_bigchar) {
		double max = src->glyphs[t]->advance_max;
		if(max>0 && max > glyph->advance) {
		    glyph->advance = max;
		}
	    }

	    font->num_glyphs++;
	}
    }
    font->unicode2glyph = (int*)malloc(sizeof(int)*font->max_unicode);
    memset(font->unicode2glyph, -1, sizeof(int)*font->max_unicode);
    for(t=0;t<font->num_glyphs;t++) {
	if(font->glyphs[t].unicode>0 && font->glyphs[t].unicode<font->max_unicode) {
	    font->unicode2glyph[font->glyphs[t].unicode] = t;
	}

    }
    return font;
}


gfxfont_t* FontInfo::getGfxFont()
{
    if(!this->gfxfont) {
        this->gfxfont = createGfxFont(this);
        this->gfxfont->id = strdup(this->id);
	this->space_char = findSpace(this->gfxfont);
	if(this->space_char>=0) {
	    msg("<debug> Font %s has space char %d (unicode=%d)", 
		    this->id, this->space_char, 
		    this->gfxfont->glyphs[this->space_char].unicode);
	} else if(config_addspace) {
	    this->space_char = addSpace(this->gfxfont);
	    msg("<debug> Appending space char to font %s, position %d", this->gfxfont->id, this->space_char);
	}
    }
    return this->gfxfont;
}

GBool InfoOutputDev::upsideDown() {return gTrue;}
GBool InfoOutputDev::useDrawChar() {return gTrue;}
GBool InfoOutputDev::interpretType3Chars() {return gTrue;}
GBool InfoOutputDev::useTilingPatternFill() {return gTrue;}

void InfoOutputDev::startPage(int pageNum, GfxState *state, double crop_x1, double crop_y1, double crop_x2, double crop_y2)
{
    double x1,y1,x2,y2;
    state->transform(crop_x1,crop_y1,&x1,&y1);
    state->transform(crop_x2,crop_y2,&x2,&y2);
    if(x2<x1) {double x3=x1;x1=x2;x2=x3;}
    if(y2<y1) {double y3=y1;y1=y2;y2=y3;}
    this->x1 = (int)x1;
    this->y1 = (int)y1;
    this->x2 = (int)x2;
    this->y2 = (int)y2;
    msg("<verbose> Generating info structure for page %d", pageNum);
}
void InfoOutputDev::endPage()
{
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

double InfoOutputDev::getMaximumFontSize(char*id)
{
    FontInfo*info = (FontInfo*)id2font->lookup(id);
    if(!info) {
	msg("<error> Unknown font id: %s", id);
	return 0.0;
    }
    return info->max_size;
}

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

void InfoOutputDev::updateFont(GfxState *state) 
{
    GfxFont*font = state->getFont();
    if(!font) {
	currentfont = 0;
	return;
    }
    if(font->getType() == fontType3) {
	currentfont = 0;
	return;
    }
    char*id = getFontID(font);

    if(currentfont)
	currentfont->splash_font = 0;

    currentfont = (FontInfo*)id2font->lookup(id);
    if(!currentfont) {
	currentfont = new FontInfo(id);
	currentfont->font = font;
	currentfont->max_size = 0;
	GString* idStr = new GString(id);
	id2font->add(idStr, (void*)currentfont);
	num_fonts++;
    }

    state->setCTM(1.0,0,0,1.0,0,0);
    splash->updateCTM(state, 0,0,0,0,0,0);
    state->setTextMat(1.0,0,0,1.0,0,0);
    state->setFont(font, 1024.0);
    splash->doUpdateFont(state);
    currentfont->splash_font = splash->getCurrentFont();
    if(currentfont->splash_font) {
        currentfont->ascender = currentfont->splash_font->ascender;
        currentfont->descender = currentfont->splash_font->descender;
    } else {
        currentfont->ascender = currentfont->descender = 0;
    }

    free(id);
}

void InfoOutputDev::fill(GfxState *state)
{
    num_polygons++;
}

void InfoOutputDev::eoFill(GfxState *state)
{
    num_polygons++;
}

FontInfo* InfoOutputDev::getFont(char*id)
{
    return (FontInfo*)id2font->lookup(id);
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
    if(!currentfont || !currentfont->splash_font) {
	return; //error
    }
    if(currentfont && currentfont->max_size < len) {
	currentfont->max_size = len;
    }
    
    num_textfields++;

    currentfont->grow(code+1);
    GlyphInfo*g = currentfont->glyphs[code];
    if(!g) {
	g = currentfont->glyphs[code] = new GlyphInfo();
	g->advance_max = 0;
	currentfont->splash_font->last_advance = -1;
	g->path = currentfont->splash_font->getGlyphPath(code);
	g->advance = currentfont->splash_font->last_advance;
	g->unicode = 0;
    }
    if(uLen && ((u[0]>=32 && u[0]<g->unicode) || !g->unicode)) {
	g->unicode = u[0];
    }
    if(currentfont->lastchar>=0 && currentfont->lasty == y) {
	double xshift = x - currentfont->lastx;
	if(xshift>=0 && xshift > g->advance_max) {
	    g->advance_max = xshift;
	}
    }

    currentfont->lastx = x;
    currentfont->lasty = y;
    currentfont->lastchar = code;
}

GBool InfoOutputDev::beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen)
{
    GfxFont*font = state->getFont();
    if(!font)
	return gTrue;
    if(font->getType() != fontType3)
	return gTrue;

    char*id = getFontID(font);
    currentfont = (FontInfo*)id2font->lookup(id);
    if(!currentfont) {
	currentfont = new FontInfo(id);
	currentfont->font = font;
	GString* idStr = new GString(id);
	id2font->add(idStr, (void*)currentfont);
	num_fonts++;
    }
    currentfont = currentfont;
    free(id);

    currentfont->grow(code+1);
    if(!currentfont->glyphs[code]) {
	currentglyph = currentfont->glyphs[code] = new GlyphInfo();
	currentglyph->unicode = uLen?u[0]:0;
	currentglyph->path = new SplashPath();
	currentglyph->x1=0;
	currentglyph->y1=0;
	currentglyph->x2=dx;
	currentglyph->y2=dy;
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

void InfoOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
			   int width, int height, GBool invert,
			   GBool inlineImg) 
{
    if(str->getKind()==strDCT) num_jpeg_images++; else num_ppm_images++;

    OutputDev::drawImageMask(state,ref,str,width,height,invert,inlineImg);
}
void InfoOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
		       int width, int height, GfxImageColorMap *colorMap,
		       int *maskColors, GBool inlineImg)
{
    if(str->getKind()==strDCT) num_jpeg_images++; else num_ppm_images++;

    OutputDev::drawImage(state,ref,str,width,height,colorMap,maskColors,inlineImg);
}
void InfoOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
				int width, int height,
				GfxImageColorMap *colorMap,
				Stream *maskStr,
				int maskWidth, int maskHeight,
				GBool maskInvert) 
{
    if(str->getKind()==strDCT) num_jpeg_images++; else num_ppm_images++;

    OutputDev::drawMaskedImage(state,ref,str,width,height,colorMap,maskStr,maskWidth,maskHeight,maskInvert);
}

void InfoOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				    int width, int height,
				    GfxImageColorMap *colorMap,
				    Stream *maskStr,
				    int maskWidth, int maskHeight,
				    GfxImageColorMap *maskColorMap) 
{
    if(str->getKind()==strDCT) num_jpeg_images++; else num_ppm_images++;

    OutputDev::drawSoftMaskedImage(state,ref,str,width,height,colorMap,maskStr,maskWidth,maskHeight,maskColorMap);
}
    
void InfoOutputDev::dumpfonts(gfxdevice_t*dev)
{
    GHashIter*i;
    GString*key;
    FontInfo*font;
    id2font->startIter(&i);
    while(id2font->getNext(&i, &key, (void**)&font)) {
        dev->addfont(dev, font->getGfxFont());
    }
}
