#include "InfoOutputDev.h"
#include "GfxState.h"
#include "../log.h"
#include <math.h>

InfoOutputDev::InfoOutputDev() 
{
    num_links = 0;
    num_images = 0;
    num_fonts = 0;
    id2font = new GHash();
}
InfoOutputDev::~InfoOutputDev() 
{
    delete id2font;
}
GBool InfoOutputDev::upsideDown() {return gTrue;}
GBool InfoOutputDev::useDrawChar() {return gTrue;}
GBool InfoOutputDev::interpretType3Chars() {return gTrue;}
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

static char*getFontID(GfxFont*font)
{
    Ref*ref = font->getID();
    GString*gstr = font->getName();
    char* fname = gstr==0?0:gstr->getCString();
    char buf[128];
    if(fname==0) {
	sprintf(buf, "font-%d-%d", ref->num, ref->gen);
    } else {
	sprintf(buf, "%s-%d-%d", fname, ref->num, ref->gen);
    }
    return strdup(buf);
}


void InfoOutputDev::updateFont(GfxState *state) 
{
    GfxFont*font = state->getFont();
    if(!font)
	return;
    char*id = getFontID(font);
    
    FontInfo*info = (FontInfo*)id2font->lookup(id);
    if(!info) {
      GString* idStr = new GString(id);
      info = new FontInfo;
      info->font = font;
      info->max_size = 0;
      id2font->add(idStr, (void*)info);
      free(id);
      num_fonts++;
    }
    currentfont = info;
}

void InfoOutputDev::drawChar(GfxState *state, double x, double y,
		      double dx, double dy,
		      double originX, double originY,
		      CharCode code, int nBytes, Unicode *u, int uLen)
{
    int render = state->getRender();
    if (render == 3)
	return;
    double m11,m21,m12,m22;
    state->getFontTransMat(&m11, &m12, &m21, &m22);
    m11 *= state->getHorizScaling();
    m21 *= state->getHorizScaling();
    double lenx = sqrt(m11*m11 + m12*m12);
    double leny = sqrt(m21*m21 + m22*m22);
    double len = lenx>leny?lenx:leny;
    if(currentfont && currentfont->max_size < len) {
	currentfont->max_size = len;
    }
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
