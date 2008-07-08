#include "Object.h"
#include "SplashTypes.h"
#include "SplashOutputDev.h"
#include "SplashPath.h"
#include "SplashFontFile.h"
#include "InfoOutputDev.h"
#include "GfxState.h"
#include "../log.h"
#include <math.h>

InfoOutputDev::InfoOutputDev(XRef*xref) 
{
    num_links = 0;
    num_images = 0;
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
FontInfo::FontInfo()
{
    this->charid2glyph = 0;
    this->seen = 0;
    this->num_glyphs = 0;
    this->glyphs = 0;
    this->splash_font = 0;
}
FontInfo::~FontInfo()
{
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
	currentfont = new FontInfo;
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
    currentfont->grow(code+1);
    GlyphInfo*g = currentfont->glyphs[code];
    if(!g) {
	g = currentfont->glyphs[code] = new GlyphInfo();
	g->path = currentfont->splash_font->getGlyphPath(code);
	g->unicode = 0;
    }
    if(uLen && (u[0]>=32 && u[0]<g->unicode || !g->unicode)) {
	g->unicode = u[0];
    }

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
	currentfont = new FontInfo;
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
    num_images++;
    OutputDev::drawImageMask(state,ref,str,width,height,invert,inlineImg);
}
void InfoOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
		       int width, int height, GfxImageColorMap *colorMap,
		       int *maskColors, GBool inlineImg)
{
    num_images++;
    OutputDev::drawImage(state,ref,str,width,height,colorMap,maskColors,inlineImg);
}
void InfoOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
				int width, int height,
				GfxImageColorMap *colorMap,
				Stream *maskStr,
				int maskWidth, int maskHeight,
				GBool maskInvert) 
{
    OutputDev::drawMaskedImage(state,ref,str,width,height,colorMap,maskStr,maskWidth,maskHeight,maskInvert);
}

void InfoOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				    int width, int height,
				    GfxImageColorMap *colorMap,
				    Stream *maskStr,
				    int maskWidth, int maskHeight,
				    GfxImageColorMap *maskColorMap) 
{
    OutputDev::drawSoftMaskedImage(state,ref,str,width,height,colorMap,maskStr,maskWidth,maskHeight,maskColorMap);
}
