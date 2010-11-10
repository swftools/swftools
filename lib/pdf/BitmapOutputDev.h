/* BitmapOutputDev.cc
   Output device which creates a bitmap.

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

#ifndef __BitmapOutputDev_h__
#define __BitmapOutputDev_h__

#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../gfxtools.h"

#include "../../config.h"
#include "CharOutputDev.h"
#include "InfoOutputDev.h"
#include "PDFDoc.h"
#include "CommonOutputDev.h"
#include "popplercompat.h"

struct ClipState
{
    ClipState*next;
    SplashBitmap*clipbitmap;
    char written;
    ClipState();
};

#define STATE_PARALLEL 0
#define STATE_TEXT_IS_ABOVE 1
#define STATE_BITMAP_IS_ABOVE 2

class BitmapOutputDev: public CommonOutputDev {
public:
    BitmapOutputDev(InfoOutputDev*info, PDFDoc*doc, int*page2page, int num_pages, int x, int y, int x1, int y1, int x2, int y2);
    virtual ~BitmapOutputDev();

    virtual void dbg_newdata(char*newdata);
   
    // CommonOutputDev:
    virtual void setDevice(gfxdevice_t*dev);
    virtual void setParameter(const char*key, const char*value);

    // OutputDev:
    virtual GBool upsideDown();
    virtual GBool useDrawChar();
    virtual GBool useTilingPatternFill();
    virtual GBool useShadedFills();
    virtual GBool useDrawForm();
    virtual GBool interpretType3Chars();
    virtual GBool needNonText();
    virtual void setDefaultCTM(double *ctm);
    virtual GBool checkPageSlice(Page *page, double hDPI, double vDPI,
			       int rotate, GBool useMediaBox, GBool crop,
			       int sliceX, int sliceY, int sliceW, int sliceH,
			       GBool printing, Catalog *catalog,
			       GBool (*abortCheckCbk)(void *data) = NULL,
			       void *abortCheckCbkData = NULL);

    virtual void beginPage(GfxState *state, int pageNum);
    virtual void endPage();
    virtual void finishPage();

    virtual void saveState(GfxState *state);
    virtual void restoreState(GfxState *state);

    virtual void updateAll(GfxState *state);
    virtual void updateCTM(GfxState *state, double m11, double m12, double m21, double m22, double m31, double m32);
    virtual void updateLineDash(GfxState *state);
    virtual void updateFlatness(GfxState *state);
    virtual void updateLineJoin(GfxState *state);
    virtual void updateLineCap(GfxState *state);
    virtual void updateMiterLimit(GfxState *state);
    virtual void updateLineWidth(GfxState *state);
    virtual void updateStrokeAdjust(GfxState *state);
    virtual void updateFillColorSpace(GfxState *state);
    virtual void updateStrokeColorSpace(GfxState *state);
    virtual void updateFillColor(GfxState *state);
    virtual void updateStrokeColor(GfxState *state);
    virtual void updateBlendMode(GfxState *state);
    virtual void updateFillOpacity(GfxState *state);
    virtual void updateStrokeOpacity(GfxState *state);
    virtual void updateFillOverprint(GfxState *state);
    virtual void updateStrokeOverprint(GfxState *state);
    virtual void updateTransfer(GfxState *state);
    virtual void updateFont(GfxState *state);
    virtual void updateTextMat(GfxState *state);
    virtual void updateCharSpace(GfxState *state);
    virtual void updateRender(GfxState *state);
    virtual void updateRise(GfxState *state);
    virtual void updateWordSpace(GfxState *state);
    virtual void updateHorizScaling(GfxState *state);
    virtual void updateTextPos(GfxState *state);
    virtual void updateTextShift(GfxState *state, double shift);

    virtual void stroke(GfxState *state);
    virtual void fill(GfxState *state);
    virtual void eoFill(GfxState *state);
    virtual POPPLER_TILING_PATERN_RETURN tilingPatternFill(
             GfxState *state, POPPLER_TILING_PATERN_GFX Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep);

    virtual GBool functionShadedFill(GfxState *state,
				     GfxFunctionShading *shading);
    virtual GBool axialShadedFill(GfxState *state, GfxAxialShading *shading POPPLER_RAXIAL_MIN_MAX);
    virtual GBool radialShadedFill(GfxState *state, GfxRadialShading *shading POPPLER_RAXIAL_MIN_MAX);

    virtual void clip(GfxState *state);
    virtual void eoClip(GfxState *state);
    virtual void clipToStrokePath(GfxState *state);

    virtual void beginStringOp(GfxState *state);
    virtual void endStringOp(GfxState *state);
    virtual void beginString(GfxState *state, GString *s);
    virtual void endString(GfxState *state);
    virtual void drawChar(GfxState *state, double x, double y,
			  double dx, double dy,
			  double originX, double originY,
			  CharCode code, int nBytes, Unicode *u, int uLen);
    virtual void drawString(GfxState *state, GString *s);
    virtual GBool beginType3Char(GfxState *state, double x, double y,
				 double dx, double dy,
				 CharCode code, Unicode *u, int uLen);
    virtual void endType3Char(GfxState *state);
    virtual void endTextObject(GfxState *state);

    virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			       int width, int height, GBool invert,
			       POPPLER_INTERPOLATE
			       GBool inlineImg);
    virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			   int width, int height, GfxImageColorMap *colorMap,
			   POPPLER_INTERPOLATE
			   int *maskColors, GBool inlineImg);
    virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
				 int width, int height,
				 GfxImageColorMap *colorMap,
				 POPPLER_INTERPOLATE
				 Stream *maskStr, int maskWidth, int maskHeight,
				 GBool maskInvert
				 POPPLER_MASK_INTERPOLATE);
    virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				     int width, int height,
				     GfxImageColorMap *colorMap,
				     POPPLER_INTERPOLATE
				     Stream *maskStr,
				     int maskWidth, int maskHeight,
				     GfxImageColorMap *maskColorMap
				     POPPLER_MASK_INTERPOLATE);

    virtual void type3D0(GfxState *state, double wx, double wy);
    virtual void type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury);

    virtual void drawForm(Ref id);

    virtual void beginTransparencyGroup(GfxState *state, double *bbox,
					GfxColorSpace *blendingColorSpace,
					GBool isolated, GBool knockout,
					GBool forSoftMask);
    virtual void endTransparencyGroup(GfxState *state);
    virtual void paintTransparencyGroup(GfxState *state, double *bbox);
    virtual void setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *backdropColor);
    virtual void clearSoftMask(GfxState *state);

    virtual void processLink(Link *link, Catalog *catalog);
  
    virtual void setVectorAntialias(GBool vaa);
    virtual GBool getVectorAntialias();

    
private:
    void flushEverything();
    void clearClips(int x1, int y1, int x2, int y2);
    void clearBoolPolyDev();
    void clearBoolTextDev();
    void flushText();
    void flushBitmap();
    GBool checkNewText(int x1, int y1, int x2, int y2);
    GBool checkNewBitmap(int x1, int y1, int x2, int y2);
    GBool clip0and1differ(int x1,int y1,int x2,int y2);
    GBool intersection(SplashBitmap*boolpoly, SplashBitmap*booltext, int x1, int y1, int x2, int y2);
    
    virtual gfxbbox_t getImageBBox(GfxState*state);
    virtual gfxbbox_t getBBox(GfxState*state);

    char config_extrafontdata;
    char config_optimizeplaincolorfills;
    char config_skewedtobitmap;
    char config_alphatobitmap;

    int text_x1,text_y1,text_x2,text_y2;

    int layerstate;
    GBool emptypage;

    SplashPath*bboxpath;

    SplashOutputDev*rgbdev;
    SplashOutputDev*clip0dev;
    SplashOutputDev*clip1dev;
    SplashOutputDev*boolpolydev;
    SplashOutputDev*booltextdev;

    SplashBitmap*rgbbitmap;
    SplashBitmap*clip0bitmap;
    SplashBitmap*clip1bitmap;
    SplashBitmap*boolpolybitmap;
    SplashBitmap*stalepolybitmap;
    SplashBitmap*booltextbitmap;
    SplashBitmap*staletextbitmap;

    gfxdevice_t* gfxoutput;
    gfxdevice_t* gfxoutput_string;
    CharOutputDev*gfxdev;
    gfxdevice_t*dev;

    //ClipState*clipstates;
};

#endif
