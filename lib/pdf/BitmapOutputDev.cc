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
#include "config.h"
#include "BitmapOutputDev.h"
#include "GFXOutputDev.h"
#include "SplashBitmap.h"
#include "SplashPattern.h"
#include "Splash.h"
#include "../log.h"

static SplashColor splash_white = {255,255,255};
static SplashColor splash_black = {255,255,255};
    
ClipState::ClipState()
{
    this->next = 0;
    this->clipbitmap = 0;
    this->written = 0;
}

BitmapOutputDev::BitmapOutputDev(InfoOutputDev*info, PDFDoc*doc)
{
    this->info = info;
    this->doc = doc;
    this->xref = doc->getXRef();
    this->rgbdev = new SplashOutputDev(splashModeRGB8, 1, gFalse, splash_white, gTrue, gTrue);
    this->clip0dev = new SplashOutputDev(splashModeMono1, 1, gFalse, splash_black, gTrue, gFalse);
    this->clip1dev = new SplashOutputDev(splashModeMono1, 1, gFalse, splash_black, gTrue, gFalse);
    this->gfxdev = new GFXOutputDev(info, this->doc);
    this->rgbdev->startDoc(this->xref);
    this->clip0dev->startDoc(this->xref);
    this->clip1dev->startDoc(this->xref);
    
    this->config_bitmapfonts = 0;
    this->config_extrafontdata = 0;
    this->bboxpath = 0;
    //this->clipdev = 0;
    //this->clipstates = 0;
}
BitmapOutputDev::~BitmapOutputDev()
{
    if(this->bboxpath) {
	delete this->bboxpath;this->bboxpath = 0;
    }
    if(this->rgbdev) {
	delete this->rgbdev;this->rgbdev= 0;
    }
    if(this->clip0dev) {
	delete this->clip0dev;this->clip0dev= 0;
    }
    if(this->clip1dev) {
	delete this->clip1dev;this->clip1dev= 0;
    }
    //if(this->clipbitmap) {
    //    delete this->clipbitmap;this->clipbitmap = 0;
    //}
    //if(this->clipdev) {
    //    delete this->clipdev;this->clipdev = 0;
    //}

}

void BitmapOutputDev::setDevice(gfxdevice_t*dev)
{
    this->dev = dev;
    this->gfxdev->setDevice(dev);
}
void BitmapOutputDev::setMove(int x,int y)
{
    this->gfxdev->setMove(x,y);
    this->user_movex = x;
    this->user_movey = y;
}
void BitmapOutputDev::setClip(int x1,int y1,int x2,int y2)
{
    this->gfxdev->setClip(x1,y1,x2,y2);
    this->user_clipx1 = x1;
    this->user_clipy1 = y1;
    this->user_clipx2 = x2;
    this->user_clipy2 = y2;
}
void BitmapOutputDev::setParameter(const char*key, const char*value)
{
    if(!strcmp(key, "extrafontdata")) {
	this->config_extrafontdata = atoi(value);
    } else if(!strcmp(key, "bitmapfonts")) {
	this->config_bitmapfonts = atoi(value);
    }
    this->gfxdev->setParameter(key, value);
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

    /* clip against (-movex, -movey, -movex+width, -movey+height) */
    if(xmin < -this->movex) xmin = -this->movex;
    if(ymin < -this->movey) ymin = -this->movey;
    if(xmax > -this->movex + width) xmax = -this->movex+this->width;
    if(ymax > -this->movey + height) ymax = -this->movey+this->height;

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
	Guchar*ain = &alpha[(y+ymin)*width+xmin];
	for(x=0;x<rangex;x++) {
	    out[x].r = in[x*3+0];
	    out[x].g = in[x*3+1];
	    out[x].b = in[x*3+2];
	    out[x].a = ain[x];
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

    memset(rgbdev->getBitmap()->getAlphaPtr(), 0, rgbdev->getBitmap()->getWidth()*rgbdev->getBitmap()->getHeight());
    memset(rgbdev->getBitmap()->getDataPtr(), 0, rgbdev->getBitmap()->getWidth()*rgbdev->getBitmap()->getHeight()*sizeof(SplashColor));

    free(img->data);img->data=0;free(img);img=0;
}

void BitmapOutputDev::startPage(int pageNum, GfxState *state, double crop_x1, double crop_y1, double crop_x2, double crop_y2)
{
    double x1,y1,x2,y2;
    state->transform(crop_x1,crop_y1,&x1,&y1);
    state->transform(crop_x2,crop_y2,&x2,&y2);
    if(x2<x1) {double x3=x1;x1=x2;x2=x3;}
    if(y2<y1) {double y3=y1;y1=y2;y2=y3;}
    
    this->movex = -(int)x1 - user_movex;
    this->movey = -(int)y1 - user_movey;
    
    if(user_clipx1|user_clipy1|user_clipx2|user_clipy2) {
        x1 = user_clipx1;
        x2 = user_clipx2;
        y1 = user_clipy1;
        y2 = user_clipy2;
    }
    this->width = (int)(x2-x1);
    this->height = (int)(y2-y1);

    msg("<verbose> startPage");
    rgbdev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    clip0dev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    clip1dev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    gfxdev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);

    /*int width = this->rgbdev->getBitmap()->getWidth();
    int height = this->rgbdev->getBitmap()->getHeight();
    this->bboxpath = new SplashPath();
    this->bboxpath->moveTo(0,0);
    this->bboxpath->lineTo(width,0);
    this->bboxpath->lineTo(width,height);
    this->bboxpath->lineTo(0,height);
    this->bboxpath->lineTo(0,0);

    if(this->clipbitmap) {
	delete this->clipbitmap;this->clipbitmap = 0;
    }
    this->clipbitmap = new SplashBitmap(width, height, 1, splashModeMono1, gFalse, gTrue);
    if(this->clipdev) {
	delete this->clipdev;this->clipdev = 0;
    }
    SplashScreenParams params;
    params.type = splashScreenDispersed;
    params.size = 0;
    params.dotRadius = 0;
    this->clipdev = new Splash(this->clipbitmap, 0, (SplashScreenParams*)0);*/
}

void BitmapOutputDev::endPage()
{
    msg("<verbose> endPage");
    this->flush();

    /* splash will now destroy alpha, and paint the 
       background color into the "holes" in the bitmap */
    rgbdev->endPage();
    clip0dev->endPage();
    clip1dev->endPage();
}

GBool BitmapOutputDev::upsideDown()
{
    clip0dev->upsideDown();
    clip1dev->upsideDown();
    return rgbdev->upsideDown();
}

GBool BitmapOutputDev::useDrawChar()
{
    clip0dev->useDrawChar();
    clip1dev->useDrawChar();
    return rgbdev->useDrawChar();
}

GBool BitmapOutputDev::useTilingPatternFill()
{
    clip0dev->useTilingPatternFill();
    clip1dev->useTilingPatternFill();
    return rgbdev->useTilingPatternFill();
}

GBool BitmapOutputDev::useShadedFills()
{
    clip0dev->useTilingPatternFill();
    clip1dev->useTilingPatternFill();
    return rgbdev->useTilingPatternFill();
}

GBool BitmapOutputDev::useDrawForm()
{
    clip0dev->useDrawForm();
    clip1dev->useDrawForm();
    return rgbdev->useDrawForm();
}

GBool BitmapOutputDev::interpretType3Chars()
{
    if(!config_bitmapfonts) {
	clip0dev->interpretType3Chars();
	clip1dev->interpretType3Chars();
	return rgbdev->interpretType3Chars();
    } else {
	return gfxdev->interpretType3Chars();
    }
}

GBool BitmapOutputDev::needNonText() 
{
    clip0dev->needNonText();
    clip1dev->needNonText();
    return rgbdev->needNonText();
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
    clip0dev->setDefaultCTM(ctm);
    clip1dev->setDefaultCTM(ctm);
}
void BitmapOutputDev::saveState(GfxState *state) 
{
    rgbdev->saveState(state);
    clip0dev->saveState(state);
    clip1dev->saveState(state);

    /*ClipState*cstate = new ClipState();
    cstate->next = this->clipstates;
    this->clipstates = cstate;*/
}
void BitmapOutputDev::restoreState(GfxState *state) 
{
    rgbdev->restoreState(state);
    clip0dev->restoreState(state);
    clip1dev->restoreState(state);

    /*if(this->clipstates) {
	ClipState*old = this->clipstates;
	if(old->written) {
	    gfxdev->restoreState(state);
	}
	this->clipstates = this->clipstates->next;
	delete(old);
    } else {
	msg("<error> invalid restoreState()");
    }*/
}
void BitmapOutputDev::updateAll(GfxState *state)
{
    rgbdev->updateAll(state);
    clip0dev->updateAll(state);
    clip1dev->updateAll(state);
}
void BitmapOutputDev::updateCTM(GfxState *state, double m11, double m12, double m21, double m22, double m31, double m32)
{
    rgbdev->updateCTM(state,m11,m12,m21,m22,m31,m32);
    clip0dev->updateCTM(state,m11,m12,m21,m22,m31,m32);
    clip1dev->updateCTM(state,m11,m12,m21,m22,m31,m32);
}
void BitmapOutputDev::updateLineDash(GfxState *state)
{
    rgbdev->updateLineDash(state);
    clip0dev->updateLineDash(state);
    clip1dev->updateLineDash(state);
}
void BitmapOutputDev::updateFlatness(GfxState *state)
{
    rgbdev->updateFlatness(state);
    clip0dev->updateFlatness(state);
    clip1dev->updateFlatness(state);
}
void BitmapOutputDev::updateLineJoin(GfxState *state)
{
    rgbdev->updateLineJoin(state);
    clip0dev->updateLineJoin(state);
    clip1dev->updateLineJoin(state);
}
void BitmapOutputDev::updateLineCap(GfxState *state)
{
    rgbdev->updateLineCap(state);
    clip0dev->updateLineCap(state);
    clip1dev->updateLineCap(state);
}
void BitmapOutputDev::updateMiterLimit(GfxState *state)
{
    rgbdev->updateMiterLimit(state);
    clip0dev->updateMiterLimit(state);
    clip1dev->updateMiterLimit(state);
}
void BitmapOutputDev::updateLineWidth(GfxState *state)
{
    rgbdev->updateLineWidth(state);
    clip0dev->updateLineWidth(state);
    clip1dev->updateLineWidth(state);
}
void BitmapOutputDev::updateStrokeAdjust(GfxState *state)
{
    rgbdev->updateStrokeAdjust(state);
    clip0dev->updateStrokeAdjust(state);
    clip1dev->updateStrokeAdjust(state);
}
void BitmapOutputDev::updateFillColorSpace(GfxState *state)
{
    rgbdev->updateFillColorSpace(state);
    clip0dev->updateFillColorSpace(state);
    clip1dev->updateFillColorSpace(state);
}
void BitmapOutputDev::updateStrokeColorSpace(GfxState *state)
{
    rgbdev->updateStrokeColorSpace(state);
    clip0dev->updateStrokeColorSpace(state);
    clip1dev->updateStrokeColorSpace(state);
}
void BitmapOutputDev::updateFillColor(GfxState *state)
{
    rgbdev->updateFillColor(state);
    clip0dev->updateFillColor(state);
    clip0dev->updateFillColor(state);
    clip1dev->updateFillColor(state);
}
void BitmapOutputDev::updateStrokeColor(GfxState *state)
{
    rgbdev->updateStrokeColor(state);
    clip0dev->updateStrokeColor(state);
    clip1dev->updateStrokeColor(state);
}
void BitmapOutputDev::updateBlendMode(GfxState *state)
{
    rgbdev->updateBlendMode(state);
    clip0dev->updateBlendMode(state);
    clip1dev->updateBlendMode(state);
}
void BitmapOutputDev::updateFillOpacity(GfxState *state)
{
    rgbdev->updateFillOpacity(state);
    clip0dev->updateFillOpacity(state);
    clip1dev->updateFillOpacity(state);
}
void BitmapOutputDev::updateStrokeOpacity(GfxState *state)
{
    rgbdev->updateStrokeOpacity(state);
    clip0dev->updateStrokeOpacity(state);
    clip1dev->updateStrokeOpacity(state);
}
void BitmapOutputDev::updateFillOverprint(GfxState *state)
{
    rgbdev->updateFillOverprint(state);
    clip0dev->updateFillOverprint(state);
    clip1dev->updateFillOverprint(state);
}
void BitmapOutputDev::updateStrokeOverprint(GfxState *state)
{
    rgbdev->updateStrokeOverprint(state);
    clip0dev->updateStrokeOverprint(state);
    clip1dev->updateStrokeOverprint(state);
}
void BitmapOutputDev::updateTransfer(GfxState *state)
{
    rgbdev->updateTransfer(state);
    clip0dev->updateTransfer(state);
    clip1dev->updateTransfer(state);
}
void BitmapOutputDev::updateFont(GfxState *state)
{
    rgbdev->updateFont(state);
    clip0dev->updateFont(state);
    clip1dev->updateFont(state);
    if(!config_bitmapfonts)
	gfxdev->updateFont(state);
}
void BitmapOutputDev::updateTextMat(GfxState *state)
{
    rgbdev->updateTextMat(state);
    clip0dev->updateTextMat(state);
    clip1dev->updateTextMat(state);
    if(!config_bitmapfonts)
	gfxdev->updateTextMat(state);
}
void BitmapOutputDev::updateCharSpace(GfxState *state)
{
    rgbdev->updateCharSpace(state);
    clip0dev->updateCharSpace(state);
    clip1dev->updateCharSpace(state);
    if(!config_bitmapfonts)
	gfxdev->updateCharSpace(state);
}
void BitmapOutputDev::updateRender(GfxState *state)
{
    rgbdev->updateRender(state);
    clip0dev->updateRender(state);
    clip1dev->updateRender(state);
    if(!config_bitmapfonts)
	gfxdev->updateRender(state);
}
void BitmapOutputDev::updateRise(GfxState *state)
{
    rgbdev->updateRise(state);
    clip0dev->updateRise(state);
    clip1dev->updateRise(state);
    if(!config_bitmapfonts)
	gfxdev->updateRise(state);
}
void BitmapOutputDev::updateWordSpace(GfxState *state)
{
    rgbdev->updateWordSpace(state);
    clip0dev->updateWordSpace(state);
    clip1dev->updateWordSpace(state);
    if(!config_bitmapfonts)
	gfxdev->updateWordSpace(state);
}
void BitmapOutputDev::updateHorizScaling(GfxState *state)
{
    rgbdev->updateHorizScaling(state);
    clip0dev->updateHorizScaling(state);
    clip1dev->updateHorizScaling(state);
    if(!config_bitmapfonts)
	gfxdev->updateHorizScaling(state);
}
void BitmapOutputDev::updateTextPos(GfxState *state)
{
    rgbdev->updateTextPos(state);
    clip0dev->updateTextPos(state);
    clip1dev->updateTextPos(state);
    if(!config_bitmapfonts)
	gfxdev->updateTextPos(state);
}
void BitmapOutputDev::updateTextShift(GfxState *state, double shift)
{
    rgbdev->updateTextShift(state, shift);
    clip0dev->updateTextShift(state, shift);
    clip1dev->updateTextShift(state, shift);
}

void BitmapOutputDev::stroke(GfxState *state)
{
    msg("<verbose> stroke");
    rgbdev->stroke(state);
}
void BitmapOutputDev::fill(GfxState *state)
{
    msg("<verbose> fill");
    rgbdev->fill(state);
}
void BitmapOutputDev::eoFill(GfxState *state)
{
    msg("<verbose> eoFill");
    rgbdev->eoFill(state);
}
#if (xpdfMajorVersion < 3) || (xpdfMinorVersion < 2) || (xpdfUpdateVersion < 7)
void BitmapOutputDev::tilingPatternFill(GfxState *state, Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep)
{
    msg("<verbose> tilingPatternFill");
    rgbdev->tilingPatternFill(state, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
}
#else
void BitmapOutputDev::tilingPatternFill(GfxState *state, Gfx *gfx, Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep) 
{
    msg("<verbose> tilingPatternFill");
    rgbdev->tilingPatternFill(state, gfx, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
}
#endif

GBool BitmapOutputDev::functionShadedFill(GfxState *state, GfxFunctionShading *shading) 
{
    msg("<verbose> functionShadedFill");
    return rgbdev->functionShadedFill(state, shading);
}
GBool BitmapOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading)
{
    msg("<verbose> axialShadedFill");
    return rgbdev->axialShadedFill(state, shading);
}
GBool BitmapOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading)
{
    msg("<verbose> radialShadedFill");
    return rgbdev->radialShadedFill(state, shading);
}

SplashColor black = {0,0,0};
SplashColor white = {255,255,255};

void BitmapOutputDev::doClip(GfxState *state, GBool eo)
{
    if(!eo) {
	rgbdev->clip(state);
	clip1dev->clip(state);
    } else {
	rgbdev->eoClip(state);
	clip1dev->eoClip(state);
    }

    //SplashPattern *pblack = new SplashSolidColor(black);
    //SplashPattern *pwhite = new SplashSolidColor(white);
    //SplashPath* path = rgbdev->convertPath(state, state->getPath());
    //clipdev->clear(black, 0);
    //clipdev->clipToPath(path, eo);
    //clipdev->setFillPattern(pwhite);
    //clipdev->fill(bboxpath,gFalse);
    //delete path;
}

void BitmapOutputDev::clip(GfxState *state)
{
    msg("<verbose> clip");
    doClip(state, gFalse);
}
void BitmapOutputDev::eoClip(GfxState *state)
{
    msg("<verbose> eoClip");
    doClip(state, gTrue);
}
void BitmapOutputDev::clipToStrokePath(GfxState *state)
{
    msg("<verbose> clipToStrokePath");
    rgbdev->clipToStrokePath(state);
    clip1dev->clipToStrokePath(state);
}

void BitmapOutputDev::beginStringOp(GfxState *state)
{
    msg("<verbose> beginStringOp");
    if(this->config_bitmapfonts) {
	rgbdev->beginStringOp(state);
	clip0dev->beginStringOp(state);
	clip1dev->beginStringOp(state);
    } else {
	gfxdev->beginStringOp(state);
    }
}
void BitmapOutputDev::endStringOp(GfxState *state)
{
    msg("<verbose> endStringOp");
    if(this->config_bitmapfonts) {
	rgbdev->endStringOp(state);
	clip0dev->endStringOp(state);
	clip1dev->endStringOp(state);
    } else {
	gfxdev->endStringOp(state);
    }
}
void BitmapOutputDev::beginString(GfxState *state, GString *s)
{
    msg("<verbose> beginString");
    if(this->config_bitmapfonts) {
	rgbdev->beginString(state, s);
	clip0dev->beginString(state, s);
	clip1dev->beginString(state, s);
    } else {
	gfxdev->beginString(state, s);
    }
}
void BitmapOutputDev::endString(GfxState *state)
{
    msg("<verbose> endString");
    if(this->config_bitmapfonts) {
	rgbdev->endString(state);
	clip0dev->endString(state);
	clip1dev->endString(state);
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
    if(this->config_bitmapfonts || (state->getRender()&4) /*clip*/ ) {
	rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
    } else {
	SplashBitmap*clip0 = clip0dev->getBitmap();
	SplashBitmap*clip1 = clip1dev->getBitmap();
	int width8 = (clip0->getWidth()+7)/8;
	int width = clip0->getWidth();
	int height = clip0->getHeight();
	memset(clip0->getDataPtr(), 0, width8*height);
	memset(clip1->getDataPtr(), 0, width8*height);
	clip0dev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	clip1dev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);

	/* if this character is affected somehow by the various clippings (i.e., it looks
	   different on a device without clipping), then draw it on the bitmap, not as
	   text */
	if(memcmp(clip0->getDataPtr(), clip1->getDataPtr(), width8*height)) {
	    msg("<verbose> Char %d is affected by clipping", code);

	    rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	    if(config_extrafontdata) {
		int oldrender = state->getRender();
		state->setRender(3); //invisible
		gfxdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
		state->setRender(oldrender);
	    }
	} else {
	    /* this char is not at all affected by clipping. Now just find out whether the
	       bitmap we're currently working on needs to be dumped out first */
    
	    Guchar*alpha = rgbdev->getBitmap()->getAlphaPtr();
	    Guchar*charpixels = clip1dev->getBitmap()->getDataPtr();
	    int xx,yy;
	    for(yy=0;yy<height;yy++) {
		Guchar*aline = &alpha[yy*width];
		Guchar*cline = &charpixels[yy*width8];
		for(xx=0;xx<width;xx++) {
		    int bit = xx&7;
		    int bytepos = xx>>3;
		    /* TODO: is the bit order correct? */
		    if(aline[xx] && (cline[bytepos]&(1<<bit))) 
			break;
		}
		if(xx!=width)
		    break;
	    }

	    if(yy!=height) {
		/* yes, the graphic data and the characters overlap (the char is
		   above the current bitmap. Flush the bitmap to the output. */
		msg("<verbose> Char %d is above current bitmap data", code);
		flush();
	    } 
	    gfxdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	}
    }
}
void BitmapOutputDev::drawString(GfxState *state, GString *s)
{
    msg("<verbose> drawString");
    if(this->config_bitmapfonts) {
	rgbdev->drawString(state, s);
	clip0dev->drawString(state, s);
	clip1dev->drawString(state, s);
    } else {
	gfxdev->drawString(state, s);
    }
}
void BitmapOutputDev::endTextObject(GfxState *state)
{
    msg("<verbose> endTextObject");
    if(this->config_bitmapfonts) {
	rgbdev->endTextObject(state);
	clip0dev->endTextObject(state);
	clip1dev->endTextObject(state);
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
	return rgbdev->beginType3Char(state, x, y, dx, dy, code, u, uLen);
    } else {
	return gfxdev->beginType3Char(state, x, y, dx, dy, code, u, uLen);
    }
}
void BitmapOutputDev::type3D0(GfxState *state, double wx, double wy)
{
    msg("<verbose> type3D0");
    if(this->config_bitmapfonts) {
	rgbdev->type3D0(state, wx, wy);
    } else {
	return gfxdev->type3D0(state, wx, wy);
    }
}
void BitmapOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury)
{
    msg("<verbose> type3D1");
    if(this->config_bitmapfonts) {
	rgbdev->type3D1(state, wx, wy, llx, lly, urx, ury);
    } else {
	return gfxdev->type3D1(state, wx, wy, llx, lly, urx, ury);
    }
}
void BitmapOutputDev::endType3Char(GfxState *state)
{
    msg("<verbose> endType3Char");
    if(this->config_bitmapfonts) {
	rgbdev->endType3Char(state);
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
}
void BitmapOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
		       int width, int height, GfxImageColorMap *colorMap,
		       int *maskColors, GBool inlineImg)
{
    msg("<verbose> drawImage");
    rgbdev->drawImage(state, ref, str, width, height, colorMap, maskColors, inlineImg);
}
void BitmapOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			     int width, int height,
			     GfxImageColorMap *colorMap,
			     Stream *maskStr, int maskWidth, int maskHeight,
			     GBool maskInvert)
{
    msg("<verbose> drawMaskedImage");
    rgbdev->drawMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskInvert);
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
}
void BitmapOutputDev::drawForm(Ref id)
{
    msg("<verbose> drawForm");
    rgbdev->drawForm(id);
}

void BitmapOutputDev::processLink(Link *link, Catalog *catalog)
{
    msg("<verbose> processLink");
    gfxdev->processLink(link, catalog);
}

void BitmapOutputDev::beginTransparencyGroup(GfxState *state, double *bbox,
				    GfxColorSpace *blendingColorSpace,
				    GBool isolated, GBool knockout,
				    GBool forSoftMask)
{
    msg("<verbose> beginTransparencyGroup");
    rgbdev->beginTransparencyGroup(state, bbox, blendingColorSpace, isolated, knockout, forSoftMask);
    clip1dev->beginTransparencyGroup(state, bbox, blendingColorSpace, isolated, knockout, forSoftMask);
}
void BitmapOutputDev::endTransparencyGroup(GfxState *state)
{
    msg("<verbose> endTransparencyGroup");
    rgbdev->endTransparencyGroup(state);
    clip1dev->endTransparencyGroup(state);
}
void BitmapOutputDev::paintTransparencyGroup(GfxState *state, double *bbox)
{
    msg("<verbose> paintTransparencyGroup");
    rgbdev->paintTransparencyGroup(state,bbox);
}
void BitmapOutputDev::setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *backdropColor)
{
    msg("<verbose> setSoftMask");
    rgbdev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
    clip1dev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
}
void BitmapOutputDev::clearSoftMask(GfxState *state)
{
    msg("<verbose> clearSoftMask");
    rgbdev->clearSoftMask(state);
    clip1dev->clearSoftMask(state);
}
