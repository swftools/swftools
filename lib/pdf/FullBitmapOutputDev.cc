/* FullBitmapOutputDev.cc

   Output Device which creates a bitmap.

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

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "FullBitmapOutputDev.h"
#include "CharOutputDev.h"

#ifdef HAVE_POPPLER
  #include "splash/SplashBitmap.h"
  #include "splash/SplashPattern.h"
  #include "splash/Splash.h"
#else
  #include "xpdf/config.h"
  #include "SplashBitmap.h"
  #include "SplashPattern.h"
  #include "Splash.h"
#endif

#include "../log.h"
#include "../png.h"
#include "../devices/record.h"

static SplashColor splash_white = {255,255,255};
static SplashColor splash_black = {0,0,0};
    
FullBitmapOutputDev::FullBitmapOutputDev(InfoOutputDev*info, PDFDoc*doc, int*page2page, int num_pages, int x, int y, int x1, int y1, int x2, int y2)
:CommonOutputDev(info, doc, page2page, num_pages, x, y, x1, y1, x2, y2)
{
    this->doc = doc;
    this->xref = doc->getXRef();
    
    msg("<verbose> Rendering everything to a bitmap");
    
    /* color graphic output device, for creating bitmaps */
    this->rgbdev = new SplashOutputDev(splashModeRGB8, 1, gFalse, splash_white, gTrue, gTrue);
  
    /* device for handling links */
    this->gfxdev = new CharOutputDev(info, this->doc, page2page, num_pages, x, y, x1, y1, x2, y2);

    this->rgbdev->startDoc(this->xref);
}
FullBitmapOutputDev::~FullBitmapOutputDev()
{
    if(this->rgbdev) {
	delete this->rgbdev;this->rgbdev = 0;
    }
    if(this->gfxdev) {
	delete this->gfxdev;this->gfxdev= 0;
    }
}

GBool FullBitmapOutputDev::getVectorAntialias()
{
    return this->rgbdev->getVectorAntialias();
}
void FullBitmapOutputDev::setVectorAntialias(GBool vaa)
{
    this->rgbdev->setVectorAntialias(vaa);
}
void FullBitmapOutputDev::setDevice(gfxdevice_t*dev)
{
    this->dev = dev;
    gfxdev->setDevice(dev);
}

void FullBitmapOutputDev::setParameter(const char*key, const char*value)
{
}
static void getBitmapBBox(Guchar*alpha, int width, int height, int*xmin, int*ymin, int*xmax, int*ymax)
{
    *ymin = -1;
    *xmin = width;
    *xmax = 0;
    int x,y;
    for(y=0;y<height;y++) {
	Guchar*a = &alpha[y*width];
	for(x=0;x<width;x++) {
	    if(a[x]) break;
	}
	int left = x; //first occupied pixel from left
	int right = x+1; //last non-occupied pixel from right
	for(;x<width;x++) {
	    if(a[x]) right=x+1;
	}

	if(left!=width) {
	    if(*ymin<0) 
		*ymin=y;
	    *ymax=y+1;
	    if(left<*xmin) *xmin = left;
	    if(right>*xmax) *xmax = right;
	}
    }
    if(*xmin>=*xmax || *ymin>=*ymax) {
	*xmin = 0;
	*ymin = 0;
	*xmax = 0;
	*ymax = 0;
    }
}

void FullBitmapOutputDev::flushBitmap()
{
    int width = rgbdev->getBitmapWidth();
    int height = rgbdev->getBitmapHeight();
    
    SplashColorPtr rgb = rgbdev->getBitmap()->getDataPtr();
    Guchar*alpha = rgbdev->getBitmap()->getAlphaPtr();

    int xmin,ymin,xmax,ymax;
    getBitmapBBox(alpha, width, height, &xmin,&ymin,&xmax,&ymax);

    /* clip against (-movex, -movey, -movex+width, -movey+height) */
    if(xmin < -this->movex) xmin = -this->movex;
    if(ymin < -this->movey) ymin = -this->movey;
    if(xmax > -this->movex + width) xmax = -this->movex+this->width;
    if(ymax > -this->movey + height) ymax = -this->movey+this->height;

    msg("<verbose> Flushing bitmap (bbox: %d,%d,%d,%d)", xmin,ymin,xmax,ymax);
    
    if((xmax-xmin)<=0 || (ymax-ymin)<=0) // no bitmap, nothing to do
	return;

    if(sizeof(SplashColor)!=3) {
	msg("<error> sizeof(SplashColor)!=3");
	return;
    }
    //xmin = ymin = 0;
    //xmax = width;
    //ymax = height;

    int rangex = xmax-xmin;
    int rangey = ymax-ymin;
    gfximage_t*img = (gfximage_t*)malloc(sizeof(gfximage_t)); 
    img->data = (gfxcolor_t*)malloc(rangex * rangey * 4);
    img->width = rangex;
    img->height = rangey;
    int x,y;
    for(y=0;y<rangey;y++) {
	SplashColorPtr in=&rgb[((y+ymin)*width+xmin)*sizeof(SplashColor)];
	gfxcolor_t*out = &img->data[y*rangex];
	Guchar*ain = &alpha[(y+ymin)*width+xmin];
	for(x=0;x<rangex;x++) {
	    // blend against a white background
	    out[x].r = (in[x*3+0]*ain[x])/255 + 255-ain[x];
	    out[x].g = (in[x*3+1]*ain[x])/255 + 255-ain[x];
	    out[x].b = (in[x*3+2]*ain[x])/255 + 255-ain[x];
	    out[x].a = 255;//ain[x];
	}
    }
    /* transform bitmap rectangle to "device space" */
    xmin += movex;
    ymin += movey;
    xmax += movex;
    ymax += movey;

    gfxmatrix_t m;
    m.tx = xmin;
    m.ty = ymin;
    m.m00 = m.m11 = 1;
    m.m10 = m.m01 = 0;

    gfxline_t* line = gfxline_makerectangle(xmin, ymin, xmax, ymax);
    dev->fillbitmap(dev, line, img, &m, 0);
    gfxline_free(line);

    free(img->data);img->data=0;free(img);img=0;
}

GBool FullBitmapOutputDev::checkPageSlice(Page *page, double hDPI, double vDPI,
             int rotate, GBool useMediaBox, GBool crop,
             int sliceX, int sliceY, int sliceW, int sliceH,
             GBool printing, Catalog *catalog,
             GBool (*abortCheckCbk)(void *data),
             void *abortCheckCbkData)
{
    this->setPage(page);
    gfxdev->setPage(page);
    return gTrue;
}

void FullBitmapOutputDev::beginPage(GfxState *state, int pageNum)
{
    msg("<debug> startPage");
    rgbdev->startPage(pageNum, state);
    gfxdev->startPage(pageNum, state);
}

void FullBitmapOutputDev::endPage()
{
    msg("<verbose> endPage (FullBitmapOutputDev)");
    flushBitmap();
    rgbdev->endPage();
    gfxdev->endPage();
}
GBool FullBitmapOutputDev::upsideDown()
{
    return rgbdev->upsideDown();
}
GBool FullBitmapOutputDev::useDrawChar()
{
    return rgbdev->useDrawChar();
}
GBool FullBitmapOutputDev::useTilingPatternFill()
{
    return rgbdev->useTilingPatternFill();
}
GBool FullBitmapOutputDev::useShadedFills()
{
    return rgbdev->useShadedFills();
}
GBool FullBitmapOutputDev::useDrawForm()
{
    return rgbdev->useDrawForm();
}
GBool FullBitmapOutputDev::interpretType3Chars()
{
    return rgbdev->interpretType3Chars();
}
GBool FullBitmapOutputDev::needNonText() 
{
    return rgbdev->needNonText();
}
void FullBitmapOutputDev::setDefaultCTM(double *ctm) 
{
    rgbdev->setDefaultCTM(ctm);
    gfxdev->setDefaultCTM(ctm);
}
void FullBitmapOutputDev::saveState(GfxState *state) 
{
    rgbdev->saveState(state);
}
void FullBitmapOutputDev::restoreState(GfxState *state) 
{
    rgbdev->restoreState(state);
}
void FullBitmapOutputDev::updateAll(GfxState *state)
{
    rgbdev->updateAll(state);
}
void FullBitmapOutputDev::updateCTM(GfxState *state, double m11, double m12, double m21, double m22, double m31, double m32)
{
    rgbdev->updateCTM(state,m11,m12,m21,m22,m31,m32);
    gfxdev->updateCTM(state,m11,m12,m21,m22,m31,m32);
}
void FullBitmapOutputDev::updateLineDash(GfxState *state)
{
    rgbdev->updateLineDash(state);
}
void FullBitmapOutputDev::updateFlatness(GfxState *state)
{
    rgbdev->updateFlatness(state);
}
void FullBitmapOutputDev::updateLineJoin(GfxState *state)
{
    rgbdev->updateLineJoin(state);
}
void FullBitmapOutputDev::updateLineCap(GfxState *state)
{
    rgbdev->updateLineCap(state);
}
void FullBitmapOutputDev::updateMiterLimit(GfxState *state)
{
    rgbdev->updateMiterLimit(state);
}
void FullBitmapOutputDev::updateLineWidth(GfxState *state)
{
    rgbdev->updateLineWidth(state);
}
void FullBitmapOutputDev::updateStrokeAdjust(GfxState *state)
{
    rgbdev->updateStrokeAdjust(state);
}
void FullBitmapOutputDev::updateFillColorSpace(GfxState *state)
{
    rgbdev->updateFillColorSpace(state);
}
void FullBitmapOutputDev::updateStrokeColorSpace(GfxState *state)
{
    rgbdev->updateStrokeColorSpace(state);
}
void FullBitmapOutputDev::updateFillColor(GfxState *state)
{
    rgbdev->updateFillColor(state);
}
void FullBitmapOutputDev::updateStrokeColor(GfxState *state)
{
    rgbdev->updateStrokeColor(state);
}
void FullBitmapOutputDev::updateBlendMode(GfxState *state)
{
    rgbdev->updateBlendMode(state);
}
void FullBitmapOutputDev::updateFillOpacity(GfxState *state)
{
    rgbdev->updateFillOpacity(state);
}
void FullBitmapOutputDev::updateStrokeOpacity(GfxState *state)
{
    rgbdev->updateStrokeOpacity(state);
}
void FullBitmapOutputDev::updateFillOverprint(GfxState *state)
{
    rgbdev->updateFillOverprint(state);
}
void FullBitmapOutputDev::updateStrokeOverprint(GfxState *state)
{
    rgbdev->updateStrokeOverprint(state);
}
void FullBitmapOutputDev::updateTransfer(GfxState *state)
{
    rgbdev->updateTransfer(state);
}
void FullBitmapOutputDev::updateFont(GfxState *state)
{
    rgbdev->updateFont(state);
}
void FullBitmapOutputDev::updateTextMat(GfxState *state)
{
    rgbdev->updateTextMat(state);
}
void FullBitmapOutputDev::updateCharSpace(GfxState *state)
{
    rgbdev->updateCharSpace(state);
}
void FullBitmapOutputDev::updateRender(GfxState *state)
{
    rgbdev->updateRender(state);
}
void FullBitmapOutputDev::updateRise(GfxState *state)
{
    rgbdev->updateRise(state);
}
void FullBitmapOutputDev::updateWordSpace(GfxState *state)
{
    rgbdev->updateWordSpace(state);
}
void FullBitmapOutputDev::updateHorizScaling(GfxState *state)
{
    rgbdev->updateHorizScaling(state);
}
void FullBitmapOutputDev::updateTextPos(GfxState *state)
{
    rgbdev->updateTextPos(state);
}
void FullBitmapOutputDev::updateTextShift(GfxState *state, double shift)
{
    rgbdev->updateTextShift(state, shift);
}

void FullBitmapOutputDev::stroke(GfxState *state)
{
    msg("<debug> stroke");
    rgbdev->stroke(state);
}
void FullBitmapOutputDev::fill(GfxState *state)
{
    msg("<debug> fill");
    rgbdev->fill(state);
}
void FullBitmapOutputDev::eoFill(GfxState *state)
{
    msg("<debug> eoFill");
    rgbdev->eoFill(state);
}
POPPLER_TILING_PATERN_RETURN FullBitmapOutputDev::tilingPatternFill(GfxState *state, POPPLER_TILING_PATERN_GFX
             Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep)
{
    msg("<debug> tilingPatternFill");
#ifdef HAVE_POPPLER
    return
#endif
    rgbdev->tilingPatternFill(state, POPPLER_TILING_PATERN_GFX_ARG str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
}

GBool FullBitmapOutputDev::functionShadedFill(GfxState *state, GfxFunctionShading *shading) 
{
    msg("<debug> functionShadedFill");
    return rgbdev->functionShadedFill(state, shading);
}
GBool FullBitmapOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading POPPLER_RAXIAL_MIN_MAX)
{
    msg("<debug> axialShadedFill");
    return rgbdev->axialShadedFill(state, shading POPPLER_RAXIAL_MIN_MAX_ARG);
}
GBool FullBitmapOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading POPPLER_RAXIAL_MIN_MAX)
{
    msg("<debug> radialShadedFill");
    return rgbdev->radialShadedFill(state, shading POPPLER_RAXIAL_MIN_MAX_ARG);
}

void FullBitmapOutputDev::clip(GfxState *state)
{
    msg("<debug> clip");
    rgbdev->clip(state);
}
void FullBitmapOutputDev::eoClip(GfxState *state)
{
    msg("<debug> eoClip");
    rgbdev->eoClip(state);
}
void FullBitmapOutputDev::clipToStrokePath(GfxState *state)
{
    msg("<debug> clipToStrokePath");
    rgbdev->clipToStrokePath(state);
}

void FullBitmapOutputDev::beginStringOp(GfxState *state)
{
    msg("<debug> beginStringOp");
    rgbdev->beginStringOp(state);
}
void FullBitmapOutputDev::endStringOp(GfxState *state)
{
    msg("<debug> endStringOp");
    rgbdev->endStringOp(state);
}
void FullBitmapOutputDev::beginString(GfxState *state, GString *s)
{
    msg("<debug> beginString");
    rgbdev->beginString(state, s);
}
void FullBitmapOutputDev::endString(GfxState *state)
{
    msg("<debug> endString");
    rgbdev->endString(state);
}
void FullBitmapOutputDev::drawChar(GfxState *state, double x, double y,
		      double dx, double dy,
		      double originX, double originY,
		      CharCode code, int nBytes, Unicode *u, int uLen)
{
    msg("<debug> drawChar");
    rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
}
void FullBitmapOutputDev::drawString(GfxState *state, GString *s)
{
    msg("<error> internal error: drawString not implemented");
    rgbdev->drawString(state, s);
}
void FullBitmapOutputDev::endTextObject(GfxState *state)
{
    /* FIXME: the below might render things (stroke outlines etc.) to gfxdev which
              might end up unflushed- should be handled similarily as
	      drawChar() above
     */
    msg("<debug> endTextObject");
    rgbdev->endTextObject(state);
}

/* TODO: these four operations below *should* do nothing, as type3
         chars are drawn using operations like fill() */
GBool FullBitmapOutputDev::beginType3Char(GfxState *state, double x, double y,
			     double dx, double dy,
			     CharCode code, Unicode *u, int uLen)
{
    msg("<debug> beginType3Char");
    return rgbdev->beginType3Char(state, x, y, dx, dy, code, u, uLen);
}
void FullBitmapOutputDev::type3D0(GfxState *state, double wx, double wy)
{
    msg("<debug> type3D0");
    rgbdev->type3D0(state, wx, wy);
}
void FullBitmapOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury)
{
    msg("<debug> type3D1");
    rgbdev->type3D1(state, wx, wy, llx, lly, urx, ury);
}
void FullBitmapOutputDev::endType3Char(GfxState *state)
{
    msg("<debug> endType3Char");
    rgbdev->endType3Char(state);
}
void FullBitmapOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
			   int width, int height, GBool invert, POPPLER_INTERPOLATE
			   GBool inlineImg)
{
    msg("<debug> drawImageMask");
    rgbdev->drawImageMask(state, ref, str, width, height, invert, POPPLER_INTERPOLATE_ARG inlineImg);
}
void FullBitmapOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
		       int width, int height, GfxImageColorMap *colorMap, POPPLER_INTERPOLATE
		       int *maskColors, GBool inlineImg)
{
    msg("<debug> drawImage");
    rgbdev->drawImage(state, ref, str, width, height, colorMap,
      POPPLER_INTERPOLATE_ARG maskColors, inlineImg);
}
void FullBitmapOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			     int width, int height,
			     GfxImageColorMap *colorMap, POPPLER_INTERPOLATE
			     Stream *maskStr, int maskWidth, int maskHeight,
			     GBool maskInvert POPPLER_MASK_INTERPOLATE)
{
    msg("<debug> drawMaskedImage");
    rgbdev->drawMaskedImage(state, ref, str, width, height, colorMap,
      POPPLER_INTERPOLATE_ARG maskStr, maskWidth, maskHeight,
      maskInvert POPPLER_MASK_INTERPOLATE_ARG);
}
void FullBitmapOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				 int width, int height,
				 GfxImageColorMap *colorMap, POPPLER_INTERPOLATE
				 Stream *maskStr, int maskWidth, int maskHeight,
				 GfxImageColorMap *maskColorMap POPPLER_MASK_INTERPOLATE)
{
    msg("<debug> drawSoftMaskedImage");
    rgbdev->drawSoftMaskedImage(state, ref, str, width, height, colorMap,
      POPPLER_INTERPOLATE_ARG maskStr, maskWidth, maskHeight,
      maskColorMap POPPLER_MASK_INTERPOLATE_ARG);
}
void FullBitmapOutputDev::drawForm(Ref id)
{
    msg("<debug> drawForm");
    rgbdev->drawForm(id);
}

void FullBitmapOutputDev::processLink(Link *link, Catalog *catalog)
{
    msg("<debug> processLink");
    gfxdev->processLink(link, catalog);
}

void FullBitmapOutputDev::beginTransparencyGroup(GfxState *state, double *bbox,
				    GfxColorSpace *blendingColorSpace,
				    GBool isolated, GBool knockout,
				    GBool forSoftMask)
{
    msg("<debug> beginTransparencyGroup");
    rgbdev->beginTransparencyGroup(state, bbox, blendingColorSpace, isolated, knockout, forSoftMask);
}
void FullBitmapOutputDev::endTransparencyGroup(GfxState *state)
{
    msg("<debug> endTransparencyGroup");
    rgbdev->endTransparencyGroup(state);
}
void FullBitmapOutputDev::paintTransparencyGroup(GfxState *state, double *bbox)
{
    msg("<debug> paintTransparencyGroup");
    rgbdev->paintTransparencyGroup(state,bbox);
}
void FullBitmapOutputDev::setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *backdropColor)
{
    msg("<debug> setSoftMask");
    rgbdev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
}
void FullBitmapOutputDev::clearSoftMask(GfxState *state)
{
    msg("<debug> clearSoftMask");
    rgbdev->clearSoftMask(state);
}



