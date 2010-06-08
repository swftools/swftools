/* DummyOutputDev.h

   Output Device which serves as "passthrough"
   
   Copyright (c) 2007 Matthias Kramm <kramm@quiss.org>

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
#include "config.h"
#include "DummyOutputDev.h"
#include "GFXOutputDev.h"
#include "SplashBitmap.h"
#include "SplashPattern.h"
#include "Splash.h"
#include "../log.h"
  
DummyOutputDev::DummyOutputDev(OutputDev*target)
{
    this->rgbdev = target;
}
DummyOutputDev::~DummyOutputDev()
{
    if(this->rgbdev) {
	delete this->rgbdev;this->rgbdev= 0;
    }
}
void DummyOutputDev::startPage(int pageNum, GfxState *state)
{
    rgbdev->startPage(pageNum, state);
}

void DummyOutputDev::endPage()
{
    rgbdev->endPage();
}

GBool DummyOutputDev::upsideDown()
{
    return rgbdev->upsideDown();
}

GBool DummyOutputDev::useDrawChar()
{
    return rgbdev->useDrawChar();
}

GBool DummyOutputDev::useTilingPatternFill()
{
    return rgbdev->useTilingPatternFill();
}

GBool DummyOutputDev::useShadedFills()
{
    return rgbdev->useShadedFills();
}

GBool DummyOutputDev::useDrawForm()
{
    return rgbdev->useDrawForm();
}

GBool DummyOutputDev::interpretType3Chars()
{
    return rgbdev->interpretType3Chars();
}

GBool DummyOutputDev::needNonText() 
{
    return rgbdev->needNonText();
}
void DummyOutputDev::setDefaultCTM(double *ctm) 
{
    rgbdev->setDefaultCTM(ctm);
}
void DummyOutputDev::saveState(GfxState *state) 
{
    rgbdev->saveState(state);
}
void DummyOutputDev::restoreState(GfxState *state) 
{
    rgbdev->restoreState(state);
}
void DummyOutputDev::updateAll(GfxState *state)
{
    rgbdev->updateAll(state);
}
void DummyOutputDev::updateCTM(GfxState *state, double m11, double m12, double m21, double m22, double m31, double m32)
{
    rgbdev->updateCTM(state,m11,m12,m21,m22,m31,m32);
}
void DummyOutputDev::updateLineDash(GfxState *state)
{
    rgbdev->updateLineDash(state);
}
void DummyOutputDev::updateFlatness(GfxState *state)
{
    rgbdev->updateFlatness(state);
}
void DummyOutputDev::updateLineJoin(GfxState *state)
{
    rgbdev->updateLineJoin(state);
}
void DummyOutputDev::updateLineCap(GfxState *state)
{
    rgbdev->updateLineCap(state);
}
void DummyOutputDev::updateMiterLimit(GfxState *state)
{
    rgbdev->updateMiterLimit(state);
}
void DummyOutputDev::updateLineWidth(GfxState *state)
{
    rgbdev->updateLineWidth(state);
}
void DummyOutputDev::updateStrokeAdjust(GfxState *state)
{
    rgbdev->updateStrokeAdjust(state);
}
void DummyOutputDev::updateFillColorSpace(GfxState *state)
{
    rgbdev->updateFillColorSpace(state);
}
void DummyOutputDev::updateStrokeColorSpace(GfxState *state)
{
    rgbdev->updateStrokeColorSpace(state);
}
void DummyOutputDev::updateFillColor(GfxState *state)
{
    rgbdev->updateFillColor(state);
}
void DummyOutputDev::updateStrokeColor(GfxState *state)
{
    rgbdev->updateStrokeColor(state);
}
void DummyOutputDev::updateBlendMode(GfxState *state)
{
    rgbdev->updateBlendMode(state);
}
void DummyOutputDev::updateFillOpacity(GfxState *state)
{
    rgbdev->updateFillOpacity(state);
}
void DummyOutputDev::updateStrokeOpacity(GfxState *state)
{
    rgbdev->updateStrokeOpacity(state);
}
void DummyOutputDev::updateFillOverprint(GfxState *state)
{
    rgbdev->updateFillOverprint(state);
}
void DummyOutputDev::updateStrokeOverprint(GfxState *state)
{
    rgbdev->updateStrokeOverprint(state);
}
void DummyOutputDev::updateTransfer(GfxState *state)
{
    rgbdev->updateTransfer(state);
}
void DummyOutputDev::updateFont(GfxState *state)
{
    rgbdev->updateFont(state);
}
void DummyOutputDev::updateTextMat(GfxState *state)
{
    rgbdev->updateTextMat(state);
}
void DummyOutputDev::updateCharSpace(GfxState *state)
{
    rgbdev->updateCharSpace(state);
}
void DummyOutputDev::updateRender(GfxState *state)
{
    rgbdev->updateRender(state);
}
void DummyOutputDev::updateRise(GfxState *state)
{
    rgbdev->updateRise(state);
}
void DummyOutputDev::updateWordSpace(GfxState *state)
{
    rgbdev->updateWordSpace(state);
}
void DummyOutputDev::updateHorizScaling(GfxState *state)
{
    rgbdev->updateHorizScaling(state);
}
void DummyOutputDev::updateTextPos(GfxState *state)
{
    rgbdev->updateTextPos(state);
}
void DummyOutputDev::updateTextShift(GfxState *state, double shift)
{
    rgbdev->updateTextShift(state, shift);
}
void DummyOutputDev::stroke(GfxState *state)
{
    rgbdev->stroke(state);
}
void DummyOutputDev::fill(GfxState *state)
{
    rgbdev->fill(state);
}
void DummyOutputDev::eoFill(GfxState *state)
{
    rgbdev->eoFill(state);
}
#if (xpdfMajorVersion < 3) || (xpdfMinorVersion < 2) || (xpdfUpdateVersion < 7)
void DummyOutputDev::tilingPatternFill(GfxState *state, Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep)
{
    rgbdev->tilingPatternFill(state, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
}
#else
void DummyOutputDev::tilingPatternFill(GfxState *state, Gfx *gfx, Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep) 
{
    rgbdev->tilingPatternFill(state, gfx, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
}
#endif

GBool DummyOutputDev::functionShadedFill(GfxState *state, GfxFunctionShading *shading) 
{
    return rgbdev->functionShadedFill(state, shading);
}
GBool DummyOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading)
{
    return rgbdev->axialShadedFill(state, shading);
}
GBool DummyOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading)
{
    return rgbdev->radialShadedFill(state, shading);
}

void DummyOutputDev::clip(GfxState *state)
{
    rgbdev->clip(state);
}
void DummyOutputDev::eoClip(GfxState *state)
{
    rgbdev->eoClip(state);
}
void DummyOutputDev::clipToStrokePath(GfxState *state)
{
    rgbdev->clipToStrokePath(state);
}

void DummyOutputDev::beginStringOp(GfxState *state)
{
    rgbdev->beginStringOp(state);
}
void DummyOutputDev::endStringOp(GfxState *state)
{
    rgbdev->endStringOp(state);
}
void DummyOutputDev::beginString(GfxState *state, GString *s)
{
    rgbdev->beginString(state, s);
}
void DummyOutputDev::endString(GfxState *state)
{
    rgbdev->endString(state);
}
void DummyOutputDev::drawChar(GfxState *state, double x, double y,
		      double dx, double dy,
		      double originX, double originY,
		      CharCode code, int nBytes, Unicode *u, int uLen)
{
    rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
}
void DummyOutputDev::drawString(GfxState *state, GString *s)
{
    rgbdev->drawString(state, s);
}
void DummyOutputDev::endTextObject(GfxState *state)
{
    rgbdev->endTextObject(state);
}
GBool DummyOutputDev::beginType3Char(GfxState *state, double x, double y,
			     double dx, double dy,
			     CharCode code, Unicode *u, int uLen)
{
    return rgbdev->beginType3Char(state, x, y, dx, dy, code, u, uLen);
}
void DummyOutputDev::type3D0(GfxState *state, double wx, double wy)
{
    rgbdev->type3D0(state, wx, wy);
}
void DummyOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury)
{
    rgbdev->type3D1(state, wx, wy, llx, lly, urx, ury);
}
void DummyOutputDev::endType3Char(GfxState *state)
{
    rgbdev->endType3Char(state);
}
void DummyOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
			   int width, int height, GBool invert,
			   GBool inlineImg)
{
    rgbdev->drawImageMask(state, ref, str, width, height, invert, inlineImg);
}
void DummyOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
		       int width, int height, GfxImageColorMap *colorMap,
		       int *maskColors, GBool inlineImg)
{
    rgbdev->drawImage(state, ref, str, width, height, colorMap, maskColors, inlineImg);
}
void DummyOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			     int width, int height,
			     GfxImageColorMap *colorMap,
			     Stream *maskStr, int maskWidth, int maskHeight,
			     GBool maskInvert)
{
    rgbdev->drawMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskInvert);
}
void DummyOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				 int width, int height,
				 GfxImageColorMap *colorMap,
				 Stream *maskStr,
				 int maskWidth, int maskHeight,
				 GfxImageColorMap *maskColorMap)
{
    rgbdev->drawSoftMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskColorMap);
}
void DummyOutputDev::drawForm(Ref id)
{
    rgbdev->drawForm(id);
}

void DummyOutputDev::processLink(Link *link, Catalog *catalog)
{
}

void DummyOutputDev::beginTransparencyGroup(GfxState *state, double *bbox,
				    GfxColorSpace *blendingColorSpace,
				    GBool isolated, GBool knockout,
				    GBool forSoftMask)
{
    rgbdev->beginTransparencyGroup(state, bbox, blendingColorSpace, isolated, knockout, forSoftMask);
}
void DummyOutputDev::endTransparencyGroup(GfxState *state)
{
    rgbdev->endTransparencyGroup(state);
}
void DummyOutputDev::paintTransparencyGroup(GfxState *state, double *bbox)
{
    rgbdev->paintTransparencyGroup(state,bbox);
}
void DummyOutputDev::setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *backdropColor)
{
    rgbdev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
}
void DummyOutputDev::clearSoftMask(GfxState *state)
{
    rgbdev->clearSoftMask(state);
}
