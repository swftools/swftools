/* InfoOutputDev.h

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
#include "BitmapOutputDev.h"
#include "GFXOutputDev.h"
#include "SplashBitmap.h"
#include "../log.h"

static SplashColor splash_white = {255,255,255};
static SplashColor splash_black = {255,255,255};

BitmapOutputDev::BitmapOutputDev(InfoOutputDev*info, PDFDoc*doc)
{
    this->info = info;
    this->doc = doc;
    this->xref = doc->getXRef();
    this->rgbdev = new SplashOutputDev(splashModeRGB8, 1, gFalse, splash_white, gTrue, gTrue);
    this->alphadev = new SplashOutputDev(splashModeMono1, 1, gFalse, splash_black, gTrue, gTrue);
    this->gfxdev = new GFXOutputDev(info, this->doc);
    this->rgbdev->startDoc(this->xref);
    this->alphadev->startDoc(this->xref);
    memset(rgbdev->getBitmap()->getAlphaPtr(), 0, rgbdev->getBitmap()->getWidth()*rgbdev->getBitmap()->getHeight());

    this->config_bitmapfonts = 0;
}
BitmapOutputDev::~BitmapOutputDev()
{
}

void BitmapOutputDev::setDevice(gfxdevice_t*dev)
{
    this->dev = dev;
    this->gfxdev->setDevice(dev);
}
void BitmapOutputDev::setMove(int x,int y)
{
}
void BitmapOutputDev::setClip(int x1,int y1,int x2,int y2)
{
}
void BitmapOutputDev::setParameter(const char*key, const char*value)
{
}
void BitmapOutputDev::preparePage(int pdfpage, int outputpage)
{
}

void getBitmapBBox(Guchar*alpha, int width, int height, int*xmin, int*ymin, int*xmax, int*ymax)
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

void BitmapOutputDev::flush()
{
    int width = rgbdev->getBitmapWidth();
    int height = rgbdev->getBitmapHeight();
    SplashColorPtr rgb = rgbdev->getBitmap()->getDataPtr();
    Guchar*alpha = rgbdev->getBitmap()->getAlphaPtr();

    int xmin,ymin,xmax,ymax;
    getBitmapBBox(alpha, width, height, &xmin,&ymin,&xmax,&ymax);

    msg("<verbose> Flushing graphics (bbox: %d,%d,%d,%d)", xmin,ymin,xmax,ymax);
    
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
	Guchar*ain = &alpha[(y+ymin)*width+xmax];
	for(x=0;x<rangex;x++) {
	    out[x].r = in[x*3+0];
	    out[x].g = in[x*3+1];
	    out[x].b = in[x*3+2];
	    out[x].a = ain[x];
	}
    }

    gfxmatrix_t m;
    m.tx = xmin;
    m.ty = ymin;
    m.m00 = m.m11 = 1;
    m.m10 = m.m01 = 0;

    gfxline_t* line = gfxline_makerectangle(xmin, ymin, xmax, ymax);
    dev->fillbitmap(dev, line, img, &m, 0);
    gfxline_free(line);

    memset(rgbdev->getBitmap()->getAlphaPtr(), 0, rgbdev->getBitmap()->getWidth()*rgbdev->getBitmap()->getHeight());
    memset(rgbdev->getBitmap()->getDataPtr(), 0, rgbdev->getBitmap()->getWidth()*rgbdev->getBitmap()->getHeight()*sizeof(SplashColor));

    free(img->data);img->data=0;free(img);img=0;
}

void BitmapOutputDev::startPage(int pageNum, GfxState *state, double x1, double y1, double x2, double y2)
{
    msg("<verbose> startPage");
    this->width = (int)x2 - (int)x1; //not used yet
    this->height = (int)y2 - (int)y1;
    rgbdev->startPage(pageNum, state, x1, y1, x2, y2);
    alphadev->startPage(pageNum, state, x1, y1, x2, y2);
}

void BitmapOutputDev::endPage()
{
    msg("<verbose> endPage");
    this->flush();

    /* splash will now destroy alpha, and paint the 
       background color into the "holes" in the bitmap */
    rgbdev->endPage();
    alphadev->endPage();
}

GBool BitmapOutputDev::upsideDown()
{
    rgbdev->upsideDown();
    return alphadev->upsideDown();
}

GBool BitmapOutputDev::useDrawChar()
{
    rgbdev->useDrawChar();
    return alphadev->useDrawChar();
}

GBool BitmapOutputDev::useTilingPatternFill()
{
    rgbdev->useTilingPatternFill();
    return alphadev->useTilingPatternFill();
}

GBool BitmapOutputDev::useShadedFills()
{
    rgbdev->useTilingPatternFill();
    return alphadev->useTilingPatternFill();
}

GBool BitmapOutputDev::useDrawForm()
{
    rgbdev->useDrawForm();
    return alphadev->useDrawForm();
}

GBool BitmapOutputDev::interpretType3Chars()
{
    if(config_bitmapfonts) {
	rgbdev->interpretType3Chars();
	return alphadev->interpretType3Chars();
    } else {
	return gfxdev->interpretType3Chars();
    }
}

GBool BitmapOutputDev::needNonText() 
{
    rgbdev->needNonText();
    return alphadev->needNonText();
}
/*GBool BitmapOutputDev::checkPageSlice(Page *page, double hDPI, double vDPI,
			   int rotate, GBool useMediaBox, GBool crop,
			   int sliceX, int sliceY, int sliceW, int sliceH,
			   GBool printing, Catalog *catalog,
			   GBool (*abortCheckCbk)(void *data),
			   void *abortCheckCbkData)
{
    return gTrue;
}*/
void BitmapOutputDev::setDefaultCTM(double *ctm) 
{
    rgbdev->setDefaultCTM(ctm);
    alphadev->setDefaultCTM(ctm);
}
void BitmapOutputDev::saveState(GfxState *state) 
{
    rgbdev->saveState(state);
    alphadev->saveState(state);
}
void BitmapOutputDev::restoreState(GfxState *state) 
{
    rgbdev->restoreState(state);
    alphadev->restoreState(state);
}
void BitmapOutputDev::updateAll(GfxState *state)
{
    rgbdev->updateAll(state);
    alphadev->updateAll(state);
}
void BitmapOutputDev::updateCTM(GfxState *state, double m11, double m12, double m21, double m22, double m31, double m32)
{
    rgbdev->updateCTM(state,m11,m12,m21,m22,m31,m32);
    alphadev->updateCTM(state,m11,m12,m21,m22,m31,m32);
}
void BitmapOutputDev::updateLineDash(GfxState *state)
{
    rgbdev->updateLineDash(state);
    alphadev->updateLineDash(state);
}
void BitmapOutputDev::updateFlatness(GfxState *state)
{
    rgbdev->updateFlatness(state);
    alphadev->updateFlatness(state);
}
void BitmapOutputDev::updateLineJoin(GfxState *state)
{
    rgbdev->updateLineJoin(state);
    alphadev->updateLineJoin(state);
}
void BitmapOutputDev::updateLineCap(GfxState *state)
{
    rgbdev->updateLineCap(state);
    alphadev->updateLineCap(state);
}
void BitmapOutputDev::updateMiterLimit(GfxState *state)
{
    rgbdev->updateMiterLimit(state);
    alphadev->updateMiterLimit(state);
}
void BitmapOutputDev::updateLineWidth(GfxState *state)
{
    rgbdev->updateLineWidth(state);
    alphadev->updateLineWidth(state);
}
void BitmapOutputDev::updateStrokeAdjust(GfxState *state)
{
    rgbdev->updateStrokeAdjust(state);
    alphadev->updateStrokeAdjust(state);
}
void BitmapOutputDev::updateFillColorSpace(GfxState *state)
{
    rgbdev->updateFillColorSpace(state);
    alphadev->updateFillColorSpace(state);
}
void BitmapOutputDev::updateStrokeColorSpace(GfxState *state)
{
    rgbdev->updateStrokeColorSpace(state);
    alphadev->updateStrokeColorSpace(state);
}
void BitmapOutputDev::updateFillColor(GfxState *state)
{
    rgbdev->updateFillColor(state);
    alphadev->updateFillColor(state);
}
void BitmapOutputDev::updateStrokeColor(GfxState *state)
{
    rgbdev->updateStrokeColor(state);
    alphadev->updateStrokeColor(state);
}
void BitmapOutputDev::updateBlendMode(GfxState *state)
{
    rgbdev->updateBlendMode(state);
    alphadev->updateBlendMode(state);
}
void BitmapOutputDev::updateFillOpacity(GfxState *state)
{
    rgbdev->updateFillOpacity(state);
    alphadev->updateFillOpacity(state);
}
void BitmapOutputDev::updateStrokeOpacity(GfxState *state)
{
    rgbdev->updateStrokeOpacity(state);
    alphadev->updateStrokeOpacity(state);
}
void BitmapOutputDev::updateFillOverprint(GfxState *state)
{
    rgbdev->updateFillOverprint(state);
    alphadev->updateFillOverprint(state);
}
void BitmapOutputDev::updateStrokeOverprint(GfxState *state)
{
    rgbdev->updateStrokeOverprint(state);
    alphadev->updateStrokeOverprint(state);
}
void BitmapOutputDev::updateTransfer(GfxState *state)
{
    rgbdev->updateTransfer(state);
    alphadev->updateTransfer(state);
}
void BitmapOutputDev::updateFont(GfxState *state)
{
    rgbdev->updateFont(state);
    alphadev->updateFont(state);
    gfxdev->updateFont(state);
}
void BitmapOutputDev::updateTextMat(GfxState *state)
{
    rgbdev->updateTextMat(state);
    alphadev->updateTextMat(state);
    gfxdev->updateTextMat(state);
}
void BitmapOutputDev::updateCharSpace(GfxState *state)
{
    rgbdev->updateCharSpace(state);
    alphadev->updateCharSpace(state);
    gfxdev->updateTextMat(state);
}
void BitmapOutputDev::updateRender(GfxState *state)
{
    rgbdev->updateRender(state);
    alphadev->updateRender(state);
    gfxdev->updateTextMat(state);
}
void BitmapOutputDev::updateRise(GfxState *state)
{
    rgbdev->updateRise(state);
    alphadev->updateRise(state);
    gfxdev->updateTextMat(state);
}
void BitmapOutputDev::updateWordSpace(GfxState *state)
{
    rgbdev->updateWordSpace(state);
    alphadev->updateWordSpace(state);
    gfxdev->updateTextMat(state);
}
void BitmapOutputDev::updateHorizScaling(GfxState *state)
{
    rgbdev->updateHorizScaling(state);
    alphadev->updateHorizScaling(state);
    gfxdev->updateTextMat(state);
}
void BitmapOutputDev::updateTextPos(GfxState *state)
{
    rgbdev->updateTextPos(state);
    alphadev->updateTextPos(state);
    gfxdev->updateTextMat(state);
}
void BitmapOutputDev::updateTextShift(GfxState *state, double shift)
{
    rgbdev->updateTextShift(state, shift);
    alphadev->updateTextShift(state, shift);
    gfxdev->updateTextMat(state);
}

void BitmapOutputDev::stroke(GfxState *state)
{
    msg("<verbose> stroke");
    rgbdev->stroke(state);
    alphadev->stroke(state);
}
void BitmapOutputDev::fill(GfxState *state)
{
    msg("<verbose> fill");
    rgbdev->fill(state);
    alphadev->fill(state);
}
void BitmapOutputDev::eoFill(GfxState *state)
{
    msg("<verbose> eoFill");
    rgbdev->eoFill(state);
    alphadev->eoFill(state);
}
void BitmapOutputDev::tilingPatternFill(GfxState *state, Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep)
{
    msg("<verbose> tilingPatternFill");
    rgbdev->tilingPatternFill(state, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
    alphadev->tilingPatternFill(state, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
}
GBool BitmapOutputDev::functionShadedFill(GfxState *state, GfxFunctionShading *shading) 
{
    msg("<verbose> functionShadedFill");
    rgbdev->functionShadedFill(state, shading);
    return alphadev->functionShadedFill(state, shading);
}
GBool BitmapOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading)
{
    msg("<verbose> axialShadedFill");
    rgbdev->axialShadedFill(state, shading);
    return alphadev->axialShadedFill(state, shading);
}
GBool BitmapOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading)
{
    msg("<verbose> radialShadedFill");
    rgbdev->radialShadedFill(state, shading);
    return alphadev->radialShadedFill(state, shading);
}
void BitmapOutputDev::clip(GfxState *state)
{
    msg("<verbose> clip");
    rgbdev->clip(state);
    alphadev->clip(state);
}
void BitmapOutputDev::eoClip(GfxState *state)
{
    msg("<verbose> eoClip");
    rgbdev->eoClip(state);
    alphadev->eoClip(state);
}
void BitmapOutputDev::clipToStrokePath(GfxState *state)
{
    msg("<verbose> clipToStrokePath");
    rgbdev->clipToStrokePath(state);
    alphadev->clipToStrokePath(state);
}

void BitmapOutputDev::beginStringOp(GfxState *state)
{
    msg("<verbose> beginStringOp");
    if(this->config_bitmapfonts) {
	rgbdev->beginStringOp(state);
	alphadev->beginStringOp(state);
    } else {
	gfxdev->beginStringOp(state);
    }
}
void BitmapOutputDev::endStringOp(GfxState *state)
{
    msg("<verbose> endStringOp");
    if(this->config_bitmapfonts) {
	rgbdev->endStringOp(state);
	alphadev->endStringOp(state);
    } else {
	gfxdev->endStringOp(state);
    }
}
void BitmapOutputDev::beginString(GfxState *state, GString *s)
{
    msg("<verbose> beginString");
    if(this->config_bitmapfonts) {
	rgbdev->beginString(state, s);
	alphadev->beginString(state, s);
    } else {
	gfxdev->beginString(state, s);
    }
}
void BitmapOutputDev::endString(GfxState *state)
{
    msg("<verbose> endString");
    if(this->config_bitmapfonts) {
	rgbdev->endString(state);
	alphadev->endString(state);
    } else {
	gfxdev->endString(state);
    }
}
void BitmapOutputDev::drawChar(GfxState *state, double x, double y,
		      double dx, double dy,
		      double originX, double originY,
		      CharCode code, int nBytes, Unicode *u, int uLen)
{
    msg("<verbose> drawChar");
    flush();
    if(this->config_bitmapfonts) {
	rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	alphadev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
    } else {
	gfxdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
    }
}
void BitmapOutputDev::drawString(GfxState *state, GString *s)
{
    msg("<verbose> drawString");
    if(this->config_bitmapfonts) {
	rgbdev->drawString(state, s);
	alphadev->drawString(state, s);
    } else {
	gfxdev->drawString(state, s);
    }
}
void BitmapOutputDev::endTextObject(GfxState *state)
{
    msg("<verbose> endTextObject");
    if(this->config_bitmapfonts) {
	rgbdev->endTextObject(state);
	alphadev->endTextObject(state);
    } else {
	gfxdev->endType3Char(state);
    }
}
GBool BitmapOutputDev::beginType3Char(GfxState *state, double x, double y,
			     double dx, double dy,
			     CharCode code, Unicode *u, int uLen)
{
    msg("<verbose> beginType3Char");
    if(this->config_bitmapfonts) {
	rgbdev->beginType3Char(state, x, y, dx, dy, code, u, uLen);
	return alphadev->beginType3Char(state, x, y, dx, dy, code, u, uLen);
    } else {
	return gfxdev->beginType3Char(state, x, y, dx, dy, code, u, uLen);
    }
}
void BitmapOutputDev::type3D0(GfxState *state, double wx, double wy)
{
    msg("<verbose> type3D0");
    if(this->config_bitmapfonts) {
	rgbdev->type3D0(state, wx, wy);
	alphadev->type3D0(state, wx, wy);
    } else {
	return gfxdev->type3D0(state, wx, wy);
    }
}
void BitmapOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury)
{
    msg("<verbose> type3D1");
    if(this->config_bitmapfonts) {
	rgbdev->type3D1(state, wx, wy, llx, lly, urx, ury);
	alphadev->type3D1(state, wx, wy, llx, lly, urx, ury);
    } else {
	return gfxdev->type3D1(state, wx, wy, llx, lly, urx, ury);
    }
}
void BitmapOutputDev::endType3Char(GfxState *state)
{
    msg("<verbose> endType3Char");
    if(this->config_bitmapfonts) {
	rgbdev->endType3Char(state);
	alphadev->endType3Char(state);
    } else {
	gfxdev->endType3Char(state);
    }
}
void BitmapOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
			   int width, int height, GBool invert,
			   GBool inlineImg)
{
    msg("<verbose> drawImageMask");
    rgbdev->drawImageMask(state, ref, str, width, height, invert, inlineImg);
    alphadev->drawImageMask(state, ref, str, width, height, invert, inlineImg);
}
void BitmapOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
		       int width, int height, GfxImageColorMap *colorMap,
		       int *maskColors, GBool inlineImg)
{
    msg("<verbose> drawImage");
    rgbdev->drawImage(state, ref, str, width, height, colorMap, maskColors, inlineImg);
    alphadev->drawImage(state, ref, str, width, height, colorMap, maskColors, inlineImg);
}
void BitmapOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			     int width, int height,
			     GfxImageColorMap *colorMap,
			     Stream *maskStr, int maskWidth, int maskHeight,
			     GBool maskInvert)
{
    msg("<verbose> drawMaskedImage");
    rgbdev->drawMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskInvert);
    alphadev->drawMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskInvert);
}
void BitmapOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				 int width, int height,
				 GfxImageColorMap *colorMap,
				 Stream *maskStr,
				 int maskWidth, int maskHeight,
				 GfxImageColorMap *maskColorMap)
{
    msg("<verbose> drawSoftMaskedImage");
    rgbdev->drawSoftMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskColorMap);
    alphadev->drawSoftMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskColorMap);
}
void BitmapOutputDev::drawForm(Ref id)
{
    msg("<verbose> drawForm");
    rgbdev->drawForm(id);
    alphadev->drawForm(id);
}
void BitmapOutputDev::beginTransparencyGroup(GfxState *state, double *bbox,
				    GfxColorSpace *blendingColorSpace,
				    GBool isolated, GBool knockout,
				    GBool forSoftMask)
{
    msg("<verbose> beginTransparencyGroup");
    rgbdev->beginTransparencyGroup(state, bbox, blendingColorSpace, isolated, knockout, forSoftMask);
    alphadev->beginTransparencyGroup(state, bbox, blendingColorSpace, isolated, knockout, forSoftMask);
}
void BitmapOutputDev::endTransparencyGroup(GfxState *state)
{
    msg("<verbose> endTransparencyGroup");
    rgbdev->endTransparencyGroup(state);
    alphadev->endTransparencyGroup(state);
}
void BitmapOutputDev::paintTransparencyGroup(GfxState *state, double *bbox)
{
    msg("<verbose> paintTransparencyGroup");
    rgbdev->paintTransparencyGroup(state,bbox);
    alphadev->paintTransparencyGroup(state,bbox);
}
void BitmapOutputDev::setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *backdropColor)
{
    msg("<verbose> setSoftMask");
    rgbdev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
    alphadev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
}
void BitmapOutputDev::clearSoftMask(GfxState *state)
{
    msg("<verbose> clearSoftMask");
    rgbdev->clearSoftMask(state);
    alphadev->clearSoftMask(state);
}
