/* InfoOutputDev.h
   A special output device which collects information about a PDF file,
   like fonts, Type3 glyphs and so on.

   This file is part of swftools.

   Swftools is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Swftools is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with swftools; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifndef __infooutputdev_h__
#define __infooutputdev_h__

#include "GfxFont.h"
#include "OutputDev.h"
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

#ifdef HAVE_POPPLER
#include <goo/GooHash.h>
#else
#include "GHash.h"
#endif
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../gfxfont.h"
#include "../q.h"

#define INTERNAL_FONT_SIZE 1024.0
#define GLYPH_IS_SPACE(g) ((!(g)->line || ((g)->line->type==gfx_moveTo && !(g)->line->next)) && (g)->advance)

struct GlyphInfo
{
    SplashPath*path;
    int unicode;
    int glyphid;
    double advance;
    double x1,y1,x2,y2;

    double advance_max;
};

class FontInfo
{
    gfxfont_t*gfxfont;

    char*id;
public:
    FontInfo(char*id);
    ~FontInfo();

    gfxfont_t* getGfxFont();

    double lastx,lasty;
    int lastchar;
    int lastadvance;

    double ascender,descender;

    void grow(int size);

    GfxFont*font;
    double max_size;
    int num_glyphs;
    GlyphInfo**glyphs;
    dict_t**kerning;

    SplashFont*splash_font;
    char seen;
    int space_char;
    float average_advance;
};

extern char*getFontID(GfxFont*font);

class InfoOutputDev: public OutputDev 
{
    GHash* id2font;
    FontInfo* currentfont;
    GlyphInfo* currentglyph;
    SplashOutputDev*splash;

    public:
    int x1,y1,x2,y2;
    int num_links;
    int num_ppm_images;
    int num_jpeg_images;
    int num_fonts;
    int num_polygons;
    int num_textfields;

    void dumpfonts(gfxdevice_t*dev);

    InfoOutputDev(XRef*xref);
    virtual ~InfoOutputDev(); 
    virtual GBool useTilingPatternFill();
    virtual GBool upsideDown();
    virtual GBool useDrawChar();
    virtual GBool interpretType3Chars();
    virtual void startPage(int pageNum, GfxState *state, double crop_x1, double crop_y1, double crop_x2, double crop_y2);
    virtual void endPage();
    virtual void drawLink(Link *link, Catalog *catalog);
    virtual double getMaximumFontSize(char*id);
    virtual void updateFont(GfxState *state);
  
    virtual void saveState(GfxState *state);
    virtual void restoreState(GfxState *state);

    virtual GBool beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen);
    virtual void type3D0(GfxState *state, double wx, double wy);
    virtual void type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury);
    virtual void endType3Char(GfxState *state);
  
    virtual void fill(GfxState *state);
    virtual void eoFill(GfxState *state);

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
