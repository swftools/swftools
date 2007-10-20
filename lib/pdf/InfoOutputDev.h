#ifndef __infooutputdev_h__
#define __infooutputdev_h__

#include "GfxFont.h"
#include "OutputDev.h"
#include "SplashFont.h"
#include "SplashOutputDev.h"
#include "SplashPath.h"
#include "GHash.h"

struct GlyphInfo
{
    SplashPath*path;
    int unicode;
    int glyphid;
};

struct FontInfo
{
    FontInfo();
    ~FontInfo();

    GfxFont*font;
    double max_size;
    int num_glyphs;
    GlyphInfo**glyphs;
    int*charid2glyph;
    SplashFont*splash_font;
};

extern char*getFontID(GfxFont*font);

class InfoOutputDev: public OutputDev 
{
    GHash* id2font;
    FontInfo* currentfont;
    SplashOutputDev*splash;
    public:
    int x1,y1,x2,y2;
    int num_links;
    int num_images;
    int num_fonts;

    InfoOutputDev(XRef*xref);
    virtual ~InfoOutputDev(); 
    virtual GBool useTilingPatternFill();
    virtual GBool upsideDown();
    virtual GBool useDrawChar();
    virtual GBool interpretType3Chars();
    virtual void startPage(int pageNum, GfxState *state, double crop_x1, double crop_y1, double crop_x2, double crop_y2);
    virtual void drawLink(Link *link, Catalog *catalog);
    virtual double getMaximumFontSize(char*id);
    virtual void updateFont(GfxState *state);
    virtual void drawChar(GfxState *state, double x, double y,
			  double dx, double dy,
			  double originX, double originY,
			  CharCode code, int nBytes, Unicode *u, int uLen);
    virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			       int width, int height, GBool invert,
			       GBool inlineImg);
    virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			   int width, int height, GfxImageColorMap *colorMap,
			   int *maskColors, GBool inlineImg);
    virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
				  int width, int height,
				  GfxImageColorMap *colorMap,
				  Stream *maskStr,
				  int maskWidth, int maskHeight,
				  GBool maskInvert);
    virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				      int width, int height,
				      GfxImageColorMap *colorMap,
				      Stream *maskStr,
				      int maskWidth, int maskHeight,
				      GfxImageColorMap *maskColorMap);
    virtual FontInfo* getFont(char*id);
};

#endif //__infooutputdev_h__
