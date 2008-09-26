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
#include <assert.h>
#include "config.h"
#include "BitmapOutputDev.h"
#include "GFXOutputDev.h"
#include "SplashBitmap.h"
#include "SplashPattern.h"
#include "Splash.h"
#include "../log.h"
#include "../png.h"
#include "../devices/record.h"

static SplashColor splash_white = {255,255,255};
static SplashColor splash_black = {0,0,0};
    
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
    
    /* color graphic output device, for creating bitmaps */
    this->rgbdev = new SplashOutputDev(splashModeRGB8, 1, gFalse, splash_white, gTrue, gTrue);
  
    /* color mode for binary bitmaps */
    SplashColorMode colorMode = splashModeMono8;

    /* two devices for testing things against clipping: one clips, the other doesn't */
    this->clip0dev = new SplashOutputDev(colorMode, 1, gFalse, splash_black, gTrue, gFalse);
    this->clip1dev = new SplashOutputDev(colorMode, 1, gFalse, splash_black, gTrue, gFalse);
    
    /* device indicating where polygonal pixels were drawn */
    this->boolpolydev = new SplashOutputDev(colorMode, 1, gFalse, splash_black, gTrue, gFalse);
    /* device indicating where text pixels were drawn */
    this->booltextdev = new SplashOutputDev(colorMode, 1, gFalse, splash_black, gTrue, gFalse);

    /* device for handling texts and links */
    this->gfxdev = new GFXOutputDev(info, this->doc);

    this->rgbdev->startDoc(this->xref);
    this->boolpolydev->startDoc(this->xref);
    this->booltextdev->startDoc(this->xref);
    this->clip0dev->startDoc(this->xref);
    this->clip1dev->startDoc(this->xref);

    this->gfxoutput = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    gfxdevice_record_init(this->gfxoutput);

    this->gfxdev->setDevice(this->gfxoutput);
    
    this->config_extrafontdata = 0;
    this->bboxpath = 0;
    //this->clipdev = 0;
    //this->clipstates = 0;
}
BitmapOutputDev::~BitmapOutputDev()
{
    if(this->gfxoutput) {
	gfxresult_t*r = this->gfxoutput->finish(this->gfxoutput);
	r->destroy(r);
	free(this->gfxoutput);this->gfxoutput = 0;
    }
    if(this->bboxpath) {
	delete this->bboxpath;this->bboxpath = 0;
    }
    if(this->rgbdev) {
	delete this->rgbdev;this->rgbdev = 0;
    }
    if(this->gfxdev) {
	delete this->gfxdev;this->gfxdev= 0;
    }
    if(this->boolpolydev) {
	delete this->boolpolydev;this->boolpolydev = 0;
    }
    if(this->booltextdev) {
	delete this->booltextdev;this->booltextdev = 0;
    }
    if(this->clip0dev) {
	delete this->clip0dev;this->clip0dev = 0;
    }
    if(this->clip1dev) {
	delete this->clip1dev;this->clip1dev = 0;
    }
    //if(this->clipbitmap) {
    //    delete this->clipbitmap;this->clipbitmap = 0;
    //}
    //if(this->clipdev) {
    //    delete this->clipdev;this->clipdev = 0;
    //}

}

GBool BitmapOutputDev::getVectorAntialias()
{
    return this->rgbdev->getVectorAntialias();
}
void BitmapOutputDev::setVectorAntialias(GBool vaa)
{
    this->rgbdev->setVectorAntialias(vaa);
}
void BitmapOutputDev::setDevice(gfxdevice_t*dev)
{
    this->dev = dev;
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
    }
    this->gfxdev->setParameter(key, value);
}
void BitmapOutputDev::preparePage(int pdfpage, int outputpage)
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

void BitmapOutputDev::flushBitmap()
{
    int width = rgbdev->getBitmapWidth();
    int height = rgbdev->getBitmapHeight();
    
    SplashColorPtr rgb = rgbbitmap->getDataPtr();
    Guchar*alpha = rgbbitmap->getAlphaPtr();

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
	if(this->emptypage) {
	    for(x=0;x<rangex;x++) {
		/* the first bitmap on the page doesn't need to have an alpha channel-
		   blend against a white background*/
		out[x].r = (in[x*3+0]*ain[x])/255 + 255-ain[x];
		out[x].g = (in[x*3+1]*ain[x])/255 + 255-ain[x];
		out[x].b = (in[x*3+2]*ain[x])/255 + 255-ain[x];
		out[x].a = 255;
	    }
	} else {
	    for(x=0;x<rangex;x++) {
		/* according to endPage()/compositeBackground() in xpdf/SplashOutputDev.cc, we
		   have to premultiply alpha (mix background and pixel according to the alpha channel).
		*/
		out[x].r = (in[x*3+0]*ain[x])/255;
		out[x].g = (in[x*3+1]*ain[x])/255;
		out[x].b = (in[x*3+2]*ain[x])/255;
		out[x].a = ain[x];
	    }
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

    memset(rgbbitmap->getAlphaPtr(), 0, rgbbitmap->getWidth()*rgbbitmap->getHeight());
    memset(rgbbitmap->getDataPtr(), 0, rgbbitmap->getRowSize()*rgbbitmap->getHeight());

    free(img->data);img->data=0;free(img);img=0;

    this->emptypage = 0;
}

void BitmapOutputDev::flushText()
{
    msg("<verbose> Flushing text/polygons");
    gfxdevice_record_flush(this->gfxoutput, this->dev);
    
    this->emptypage = 0;
}

void writeAlpha(SplashBitmap*bitmap, char*filename)
{
    int y,x;
    
    int width = bitmap->getWidth();
    int height = bitmap->getHeight();

    gfxcolor_t*data = (gfxcolor_t*)malloc(sizeof(gfxcolor_t)*width*height);

    for(y=0;y<height;y++) {
	gfxcolor_t*line = &data[y*width];
	for(x=0;x<width;x++) {
	    int a = bitmap->getAlpha(x,y);
	    line[x].r = a;
	    line[x].g = a;
	    line[x].b = a;
	    line[x].a = 255;
	}
    }
    writePNG(filename, (unsigned char*)data, width, height);
    free(data);
}
static int dbg_btm_counter=1;

static const char*STATE_NAME[] = {"parallel", "textabovebitmap", "bitmapabovetext"};

void BitmapOutputDev::checkNewText()
{
    /* called once some new text was drawn on booltextdev, and
       before the same thing is drawn on gfxdev */
   
    msg("<trace> Testing new text data against current bitmap data, state=%s, counter=%d\n", STATE_NAME[layerstate], dbg_btm_counter);
    
    char filename1[80];
    char filename2[80];
    sprintf(filename1, "state%dbitmap_afternewtext.png", dbg_btm_counter);
    sprintf(filename2, "state%dtext_afternewtext.png", dbg_btm_counter);
    if(0) {
	writeAlpha(boolpolybitmap, filename1);
	writeAlpha(booltextbitmap, filename2);
    }
    dbg_btm_counter++;

    if(intersection()) {
	if(layerstate==STATE_PARALLEL) {
	    /* the new text is above the bitmap. So record that fact,
	       and also clear the bitmap buffer, so we can check for
	       new intersections */
	    msg("<verbose> Text is above current bitmap/polygon data");
	    layerstate=STATE_TEXT_IS_ABOVE;
	    clearBoolPolyDev();
	} else if(layerstate==STATE_BITMAP_IS_ABOVE) {
	    /* there's a bitmap above the (old) text. So we need
	       to flush out that text, and record that the *new*
	       text is now *above* the bitmap
	     */
	    msg("<verbose> Text is above current bitmap/polygon data (which is above some other text)");
	    flushText();
	    layerstate=STATE_TEXT_IS_ABOVE;
	    /* clear both bool devices- the text device because
	       we just dumped out all the (old) text, and the
	       poly dev so we can check for new intersections */
	    clearBoolPolyDev();
	    clearBoolTextDev();
	} else {
	    /* we already know that the current text section is
	       above the current bitmap section- now just new
	       bitmap data *and* new text data was drawn, and
	       *again* it's above the current bitmap- so clear
	       the polygon bitmap again, so we can check for
	       new intersections */
	    msg("<verbose> Text is still above current bitmap/polygon data");
	    clearBoolPolyDev();
	}
    } 
}

void BitmapOutputDev::checkNewBitmap()
{
    /* similar to checkNewText() above, only in reverse */
    msg("<trace> Testing new graphics data against current text data, state=%s, counter=%d\n", STATE_NAME[layerstate], dbg_btm_counter);

    char filename1[80];
    char filename2[80];
    sprintf(filename1, "state%dbitmap_afternewgfx.png", dbg_btm_counter);
    sprintf(filename2, "state%dtext_afternewgfx.png", dbg_btm_counter);
    if(0) {
	writeAlpha(boolpolybitmap, filename1);
	writeAlpha(booltextbitmap, filename2);
    }
    dbg_btm_counter++;

    if(intersection()) {
	if(layerstate==STATE_PARALLEL) {
	    msg("<verbose> Bitmap is above current text data");
	    layerstate=STATE_BITMAP_IS_ABOVE;
	    clearBoolTextDev();
	} else if(layerstate==STATE_TEXT_IS_ABOVE) {
	    msg("<verbose> Bitmap is above current text data (which is above some bitmap)");
	    flushBitmap();
	    layerstate=STATE_BITMAP_IS_ABOVE;
	    clearBoolTextDev();
	    clearBoolPolyDev();
	} else {
	    msg("<verbose> Bitmap is still above current text data");
	    clearBoolTextDev();
	}
    } 
}

//void checkNewText() {
//    Guchar*alpha = rgbbitmap->getAlphaPtr();
//    Guchar*charpixels = clip1bitmap->getDataPtr();
//    int xx,yy;
//    for(yy=0;yy<height;yy++) {
//        Guchar*aline = &alpha[yy*width];
//        Guchar*cline = &charpixels[yy*width8];
//        for(xx=0;xx<width;xx++) {
//            int bit = xx&7;
//            int bytepos = xx>>3;
//            /* TODO: is the bit order correct? */
//            if(aline[xx] && (cline[bytepos]&(1<<bit))) 
//        	break;
//        }
//        if(xx!=width)
//            break;
//}

GBool BitmapOutputDev::clip0and1differ()
{
    if(clip0bitmap->getMode()==splashModeMono1) {
	SplashBitmap*clip0 = clip0bitmap;
	SplashBitmap*clip1 = clip1bitmap;
	int width8 = (clip0->getWidth()+7)/8;
	int height = clip0->getHeight();
	return memcmp(clip0->getDataPtr(), clip1->getDataPtr(), width8*height);
    } else {
	SplashBitmap*clip0 = clip0bitmap;
	SplashBitmap*clip1 = clip1bitmap;
	int width = clip0->getAlphaRowSize();
	int height = clip0->getHeight();
	return memcmp(clip0->getAlphaPtr(), clip1->getAlphaPtr(), width*height);
    }
}

static void clearBooleanBitmap(SplashBitmap*btm)
{
    if(btm->getMode()==splashModeMono1) {
	int width8 = (btm->getWidth()+7)/8;
	int width = btm->getWidth();
	int height = btm->getHeight();
	memset(btm->getDataPtr(), 0, width8*height);
    } else {
	int width = btm->getAlphaRowSize();
	int height = btm->getHeight();
	memset(btm->getAlphaPtr(), 0, width*height);
    }
}

long long unsigned int compare64(long long unsigned int*data1, long long unsigned int*data2, int len)
{
    long long unsigned int c;
    int t;
    for(t=0;t<len;t++) {
        c |= data1[t]&data2[t];
    }
    return c;
}

GBool BitmapOutputDev::intersection()
{
    SplashBitmap*boolpoly = boolpolybitmap;
    SplashBitmap*booltext = booltextbitmap;
	
    if(boolpoly->getMode()==splashModeMono1) {
	/* alternative implementation, using one bit per pixel-
	   would work if xpdf wouldn't try to dither everything */

	Guchar*polypixels = boolpoly->getDataPtr();
	Guchar*textpixels = booltext->getDataPtr();
    
	int width8 = (width+7)/8;
	int height = boolpoly->getHeight();
	
	int t;
	int len = height*width8;
	unsigned long long int c=0;
        assert(sizeof(unsigned long long int)==8);
	{
            if(((int)polypixels&7) || ((int)textpixels&7)) {
                msg("<warning> Non-optimal alignment");
            }
            int l2 = len;
	    len /= sizeof(unsigned long long int);
            c = compare64((unsigned long long int*)polypixels, (unsigned long long int*)textpixels, len);
            int l1 = len*sizeof(unsigned long long int);
	    for(t=l1;t<l2;t++) {
		c |= (unsigned long long int)(polypixels[t]&textpixels[t]);
	    }
	}
	if(c)
	    /* if graphic data and the characters overlap, they have common bits */
	    return gTrue;
	else
	    return gFalse;
    } else {
	Guchar*polypixels = boolpoly->getAlphaPtr();
	Guchar*textpixels = booltext->getAlphaPtr();
	
	int width = boolpoly->getAlphaRowSize();
	int height = boolpoly->getHeight();
	
	int t;
	int len = height*width;
	unsigned int c=0;
	if(len & (sizeof(unsigned int)-1)) {
	    Guchar c2=0;
	    for(t=0;t<len;t++) {
		if(polypixels[t]&&textpixels[t])
		    return gTrue;
	    }
	} else {
	    len /= sizeof(unsigned int);
	    for(t=0;t<len;t++) {
		if((((unsigned int*)polypixels)[t]) & (((unsigned int*)textpixels)[t]))
		    return gTrue;
	    }
	}
	return gFalse;
    }
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

    msg("<debug> startPage");
    rgbdev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    boolpolydev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    booltextdev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    clip0dev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    clip1dev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    gfxdev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);

    boolpolybitmap = boolpolydev->getBitmap();
    booltextbitmap = booltextdev->getBitmap();
    clip0bitmap = clip0dev->getBitmap();
    clip1bitmap = clip1dev->getBitmap();
    rgbbitmap = rgbdev->getBitmap();
    
    flushText(); // write out the initial clipping rectangle

    /* just in case any device did draw a white background rectangle 
       into the device */
    clearBoolTextDev();
    clearBoolPolyDev();

    this->layerstate = STATE_PARALLEL;
    this->emptypage = 1;
    msg("<debug> startPage done");
}

void BitmapOutputDev::endPage()
{
    msg("<verbose> endPage (BitmapOutputDev)");

    /* notice: we're not fully done yet with this page- there might still be 
       a few calls to drawLink() yet to come */
}
void BitmapOutputDev::finishPage()
{
    msg("<verbose> finishPage (BitmapOutputDev)");
    gfxdev->endPage();
   
    if(layerstate == STATE_BITMAP_IS_ABOVE) {
	this->flushText();
	this->flushBitmap();
    } else {
	this->flushBitmap();
	this->flushText();
    }

    /* splash will now destroy alpha, and paint the 
       background color into the "holes" in the bitmap */
    boolpolydev->endPage();
    booltextdev->endPage();
    rgbdev->endPage();
    clip0dev->endPage();
    clip1dev->endPage();
}

GBool BitmapOutputDev::upsideDown()
{
    boolpolydev->upsideDown();
    booltextdev->upsideDown();
    clip0dev->upsideDown();
    clip1dev->upsideDown();
    return rgbdev->upsideDown();
}

GBool BitmapOutputDev::useDrawChar()
{
    boolpolydev->useDrawChar();
    booltextdev->useDrawChar();
    clip0dev->useDrawChar();
    clip1dev->useDrawChar();
    return rgbdev->useDrawChar();
}

GBool BitmapOutputDev::useTilingPatternFill()
{
    boolpolydev->useTilingPatternFill();
    booltextdev->useTilingPatternFill();
    clip0dev->useTilingPatternFill();
    clip1dev->useTilingPatternFill();
    return rgbdev->useTilingPatternFill();
}

GBool BitmapOutputDev::useShadedFills()
{
    boolpolydev->useShadedFills();
    booltextdev->useShadedFills();
    clip0dev->useShadedFills();
    clip1dev->useShadedFills();
    return rgbdev->useShadedFills();
}

GBool BitmapOutputDev::useDrawForm()
{
    boolpolydev->useDrawForm();
    booltextdev->useDrawForm();
    clip0dev->useDrawForm();
    clip1dev->useDrawForm();
    return rgbdev->useDrawForm();
}

GBool BitmapOutputDev::interpretType3Chars()
{
    boolpolydev->interpretType3Chars();
    booltextdev->interpretType3Chars();
    clip0dev->interpretType3Chars();
    clip1dev->interpretType3Chars();
    return rgbdev->interpretType3Chars();
}

GBool BitmapOutputDev::needNonText() 
{
    boolpolydev->needNonText();
    booltextdev->needNonText();
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
    boolpolydev->setDefaultCTM(ctm);
    booltextdev->setDefaultCTM(ctm);
    rgbdev->setDefaultCTM(ctm);
    clip0dev->setDefaultCTM(ctm);
    clip1dev->setDefaultCTM(ctm);
    gfxdev->setDefaultCTM(ctm);
}
void BitmapOutputDev::saveState(GfxState *state) 
{
    boolpolydev->saveState(state);
    booltextdev->saveState(state);
    rgbdev->saveState(state);
    clip0dev->saveState(state);
    clip1dev->saveState(state);

    /*ClipState*cstate = new ClipState();
    cstate->next = this->clipstates;
    this->clipstates = cstate;*/
}
void BitmapOutputDev::restoreState(GfxState *state) 
{
    boolpolydev->restoreState(state);
    booltextdev->restoreState(state);
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
    boolpolydev->updateAll(state);
    booltextdev->updateAll(state);
    rgbdev->updateAll(state);
    clip0dev->updateAll(state);
    clip1dev->updateAll(state);
    gfxdev->updateAll(state);
}
void BitmapOutputDev::updateCTM(GfxState *state, double m11, double m12, double m21, double m22, double m31, double m32)
{
    boolpolydev->updateCTM(state,m11,m12,m21,m22,m31,m32);
    booltextdev->updateCTM(state,m11,m12,m21,m22,m31,m32);
    rgbdev->updateCTM(state,m11,m12,m21,m22,m31,m32);
    clip0dev->updateCTM(state,m11,m12,m21,m22,m31,m32);
    clip1dev->updateCTM(state,m11,m12,m21,m22,m31,m32);
    gfxdev->updateCTM(state,m11,m12,m21,m22,m31,m32);
}
void BitmapOutputDev::updateLineDash(GfxState *state)
{
    boolpolydev->updateLineDash(state);
    booltextdev->updateLineDash(state);
    rgbdev->updateLineDash(state);
    clip0dev->updateLineDash(state);
    clip1dev->updateLineDash(state);
    gfxdev->updateLineDash(state);
}
void BitmapOutputDev::updateFlatness(GfxState *state)
{
    boolpolydev->updateFlatness(state);
    booltextdev->updateFlatness(state);
    rgbdev->updateFlatness(state);
    clip0dev->updateFlatness(state);
    clip1dev->updateFlatness(state);
    gfxdev->updateFlatness(state);
}
void BitmapOutputDev::updateLineJoin(GfxState *state)
{
    boolpolydev->updateLineJoin(state);
    booltextdev->updateLineJoin(state);
    rgbdev->updateLineJoin(state);
    clip0dev->updateLineJoin(state);
    clip1dev->updateLineJoin(state);
    gfxdev->updateLineJoin(state);
}
void BitmapOutputDev::updateLineCap(GfxState *state)
{
    boolpolydev->updateLineCap(state);
    booltextdev->updateLineCap(state);
    rgbdev->updateLineCap(state);
    clip0dev->updateLineCap(state);
    clip1dev->updateLineCap(state);
    gfxdev->updateLineCap(state);
}
void BitmapOutputDev::updateMiterLimit(GfxState *state)
{
    boolpolydev->updateMiterLimit(state);
    booltextdev->updateMiterLimit(state);
    rgbdev->updateMiterLimit(state);
    clip0dev->updateMiterLimit(state);
    clip1dev->updateMiterLimit(state);
    gfxdev->updateMiterLimit(state);
}
void BitmapOutputDev::updateLineWidth(GfxState *state)
{
    boolpolydev->updateLineWidth(state);
    booltextdev->updateLineWidth(state);
    rgbdev->updateLineWidth(state);
    clip0dev->updateLineWidth(state);
    clip1dev->updateLineWidth(state);
    gfxdev->updateLineWidth(state);
}
void BitmapOutputDev::updateStrokeAdjust(GfxState *state)
{
    boolpolydev->updateStrokeAdjust(state);
    booltextdev->updateStrokeAdjust(state);
    rgbdev->updateStrokeAdjust(state);
    clip0dev->updateStrokeAdjust(state);
    clip1dev->updateStrokeAdjust(state);
    gfxdev->updateStrokeAdjust(state);
}
void BitmapOutputDev::updateFillColorSpace(GfxState *state)
{
    boolpolydev->updateFillColorSpace(state);
    booltextdev->updateFillColorSpace(state);
    rgbdev->updateFillColorSpace(state);
    clip0dev->updateFillColorSpace(state);
    clip1dev->updateFillColorSpace(state);
    gfxdev->updateFillColorSpace(state);
}
void BitmapOutputDev::updateStrokeColorSpace(GfxState *state)
{
    boolpolydev->updateStrokeColorSpace(state);
    booltextdev->updateStrokeColorSpace(state);
    rgbdev->updateStrokeColorSpace(state);
    clip0dev->updateStrokeColorSpace(state);
    clip1dev->updateStrokeColorSpace(state);
    gfxdev->updateStrokeColorSpace(state);
}
void BitmapOutputDev::updateFillColor(GfxState *state)
{
    boolpolydev->updateFillColor(state);
    booltextdev->updateFillColor(state);
    rgbdev->updateFillColor(state);
    clip0dev->updateFillColor(state);
    clip1dev->updateFillColor(state);
    gfxdev->updateFillColor(state);
}
void BitmapOutputDev::updateStrokeColor(GfxState *state)
{
    boolpolydev->updateStrokeColor(state);
    booltextdev->updateStrokeColor(state);
    rgbdev->updateStrokeColor(state);
    clip0dev->updateStrokeColor(state);
    clip1dev->updateStrokeColor(state);
    gfxdev->updateStrokeColor(state);
}
void BitmapOutputDev::updateBlendMode(GfxState *state)
{
    boolpolydev->updateBlendMode(state);
    booltextdev->updateBlendMode(state);
    rgbdev->updateBlendMode(state);
    clip0dev->updateBlendMode(state);
    clip1dev->updateBlendMode(state);
    gfxdev->updateBlendMode(state);
}
void BitmapOutputDev::updateFillOpacity(GfxState *state)
{
    boolpolydev->updateFillOpacity(state);
    booltextdev->updateFillOpacity(state);
    rgbdev->updateFillOpacity(state);
    clip0dev->updateFillOpacity(state);
    clip1dev->updateFillOpacity(state);
    gfxdev->updateFillOpacity(state);
}
void BitmapOutputDev::updateStrokeOpacity(GfxState *state)
{
    boolpolydev->updateStrokeOpacity(state);
    booltextdev->updateStrokeOpacity(state);
    rgbdev->updateStrokeOpacity(state);
    clip0dev->updateStrokeOpacity(state);
    clip1dev->updateStrokeOpacity(state);
    gfxdev->updateStrokeOpacity(state);
}
void BitmapOutputDev::updateFillOverprint(GfxState *state)
{
    boolpolydev->updateFillOverprint(state);
    booltextdev->updateFillOverprint(state);
    rgbdev->updateFillOverprint(state);
    clip0dev->updateFillOverprint(state);
    clip1dev->updateFillOverprint(state);
    gfxdev->updateFillOverprint(state);
}
void BitmapOutputDev::updateStrokeOverprint(GfxState *state)
{
    boolpolydev->updateStrokeOverprint(state);
    booltextdev->updateStrokeOverprint(state);
    rgbdev->updateStrokeOverprint(state);
    clip0dev->updateStrokeOverprint(state);
    clip1dev->updateStrokeOverprint(state);
    gfxdev->updateStrokeOverprint(state);
}
void BitmapOutputDev::updateTransfer(GfxState *state)
{
    boolpolydev->updateTransfer(state);
    booltextdev->updateTransfer(state);
    rgbdev->updateTransfer(state);
    clip0dev->updateTransfer(state);
    clip1dev->updateTransfer(state);
    gfxdev->updateTransfer(state);
}
void BitmapOutputDev::updateFont(GfxState *state)
{
    boolpolydev->updateFont(state);
    booltextdev->updateFont(state);
    rgbdev->updateFont(state);
    clip0dev->updateFont(state);
    clip1dev->updateFont(state);
    gfxdev->updateFont(state);
}
void BitmapOutputDev::updateTextMat(GfxState *state)
{
    boolpolydev->updateTextMat(state);
    booltextdev->updateTextMat(state);
    rgbdev->updateTextMat(state);
    clip0dev->updateTextMat(state);
    clip1dev->updateTextMat(state);
    gfxdev->updateTextMat(state);
}
void BitmapOutputDev::updateCharSpace(GfxState *state)
{
    boolpolydev->updateCharSpace(state);
    booltextdev->updateCharSpace(state);
    rgbdev->updateCharSpace(state);
    clip0dev->updateCharSpace(state);
    clip1dev->updateCharSpace(state);
    gfxdev->updateCharSpace(state);
}
void BitmapOutputDev::updateRender(GfxState *state)
{
    boolpolydev->updateRender(state);
    booltextdev->updateRender(state);
    rgbdev->updateRender(state);
    clip0dev->updateRender(state);
    clip1dev->updateRender(state);
    gfxdev->updateRender(state);
}
void BitmapOutputDev::updateRise(GfxState *state)
{
    boolpolydev->updateRise(state);
    booltextdev->updateRise(state);
    rgbdev->updateRise(state);
    clip0dev->updateRise(state);
    clip1dev->updateRise(state);
    gfxdev->updateRise(state);
}
void BitmapOutputDev::updateWordSpace(GfxState *state)
{
    boolpolydev->updateWordSpace(state);
    booltextdev->updateWordSpace(state);
    rgbdev->updateWordSpace(state);
    clip0dev->updateWordSpace(state);
    clip1dev->updateWordSpace(state);
    gfxdev->updateWordSpace(state);
}
void BitmapOutputDev::updateHorizScaling(GfxState *state)
{
    boolpolydev->updateHorizScaling(state);
    booltextdev->updateHorizScaling(state);
    rgbdev->updateHorizScaling(state);
    clip0dev->updateHorizScaling(state);
    clip1dev->updateHorizScaling(state);
    gfxdev->updateHorizScaling(state);
}
void BitmapOutputDev::updateTextPos(GfxState *state)
{
    boolpolydev->updateTextPos(state);
    booltextdev->updateTextPos(state);
    rgbdev->updateTextPos(state);
    clip0dev->updateTextPos(state);
    clip1dev->updateTextPos(state);
    gfxdev->updateTextPos(state);
}
void BitmapOutputDev::updateTextShift(GfxState *state, double shift)
{
    boolpolydev->updateTextShift(state, shift);
    booltextdev->updateTextShift(state, shift);
    rgbdev->updateTextShift(state, shift);
    clip0dev->updateTextShift(state, shift);
    clip1dev->updateTextShift(state, shift);
    gfxdev->updateTextShift(state, shift);
}

void BitmapOutputDev::stroke(GfxState *state)
{
    msg("<debug> stroke");
    boolpolydev->stroke(state);
    checkNewBitmap();
    rgbdev->stroke(state);
}
void BitmapOutputDev::fill(GfxState *state)
{
    msg("<debug> fill");
    boolpolydev->fill(state);
    checkNewBitmap();
    rgbdev->fill(state);
}
void BitmapOutputDev::eoFill(GfxState *state)
{
    msg("<debug> eoFill");
    boolpolydev->eoFill(state);
    checkNewBitmap();
    rgbdev->eoFill(state);
}
#if (xpdfMajorVersion*10000 + xpdfMinorVersion*100 + xpdfUpdateVersion) < 30207
void BitmapOutputDev::tilingPatternFill(GfxState *state, Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep)
{
    msg("<debug> tilingPatternFill");
    boolpolydev->tilingPatternFill(state, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
    checkNewBitmap();
    rgbdev->tilingPatternFill(state, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
}
#else
void BitmapOutputDev::tilingPatternFill(GfxState *state, Gfx *gfx, Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep) 
{
    msg("<debug> tilingPatternFill");
    boolpolydev->tilingPatternFill(state, gfx, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
    checkNewBitmap();
    rgbdev->tilingPatternFill(state, gfx, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
}
#endif

GBool BitmapOutputDev::functionShadedFill(GfxState *state, GfxFunctionShading *shading) 
{
    msg("<debug> functionShadedFill");
    boolpolydev->functionShadedFill(state, shading);
    checkNewBitmap();
    return rgbdev->functionShadedFill(state, shading);
}
GBool BitmapOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading)
{
    msg("<debug> axialShadedFill");
    boolpolydev->axialShadedFill(state, shading);
    checkNewBitmap();
    return rgbdev->axialShadedFill(state, shading);
}
GBool BitmapOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading)
{
    msg("<debug> radialShadedFill");
    boolpolydev->radialShadedFill(state, shading);
    checkNewBitmap();
    return rgbdev->radialShadedFill(state, shading);
}

SplashColor black = {0,0,0};
SplashColor white = {255,255,255};

void BitmapOutputDev::clip(GfxState *state)
{
    msg("<debug> clip");
    boolpolydev->clip(state);
    booltextdev->clip(state);
    rgbdev->clip(state);
    clip1dev->clip(state);
}
void BitmapOutputDev::eoClip(GfxState *state)
{
    msg("<debug> eoClip");
    boolpolydev->eoClip(state);
    booltextdev->eoClip(state);
    rgbdev->eoClip(state);
    clip1dev->eoClip(state);
}
void BitmapOutputDev::clipToStrokePath(GfxState *state)
{
    msg("<debug> clipToStrokePath");
    boolpolydev->clipToStrokePath(state);
    booltextdev->clipToStrokePath(state);
    rgbdev->clipToStrokePath(state);
    clip1dev->clipToStrokePath(state);
}

void BitmapOutputDev::beginStringOp(GfxState *state)
{
    msg("<debug> beginStringOp");
    clip0dev->beginStringOp(state);
    clip1dev->beginStringOp(state);
    booltextdev->beginStringOp(state);
    gfxdev->beginStringOp(state);
}
void BitmapOutputDev::endStringOp(GfxState *state)
{
    msg("<debug> endStringOp");
    clip0dev->endStringOp(state);
    clip1dev->endStringOp(state);
    booltextdev->endStringOp(state);
    checkNewText();
    gfxdev->endStringOp(state);
}
void BitmapOutputDev::beginString(GfxState *state, GString *s)
{
    msg("<debug> beginString");
    clip0dev->beginString(state, s);
    clip1dev->beginString(state, s);
    booltextdev->beginString(state, s);
    gfxdev->beginString(state, s);
}
void BitmapOutputDev::endString(GfxState *state)
{
    msg("<debug> endString");
    clip0dev->endString(state);
    clip1dev->endString(state);
    booltextdev->endString(state);
    checkNewText();
    gfxdev->endString(state);
}

void BitmapOutputDev::clearClips()
{
    clearBooleanBitmap(clip0bitmap);
    clearBooleanBitmap(clip1bitmap);
}
void BitmapOutputDev::clearBoolPolyDev()
{
    clearBooleanBitmap(boolpolybitmap);
}
void BitmapOutputDev::clearBoolTextDev()
{
    clearBooleanBitmap(booltextbitmap);
}
void BitmapOutputDev::drawChar(GfxState *state, double x, double y,
		      double dx, double dy,
		      double originX, double originY,
		      CharCode code, int nBytes, Unicode *u, int uLen)
{
    msg("<debug> drawChar");
    if(state->getRender()&RENDER_CLIP) {
	rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
    } else {
	clearClips();
	clip0dev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	clip1dev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);

	/* if this character is affected somehow by the various clippings (i.e., it looks
	   different on a device without clipping), then draw it on the bitmap, not as
	   text */
	if(clip0and1differ()) {
	    msg("<verbose> Char %d is affected by clipping", code);
	    boolpolydev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	    checkNewBitmap();
	    rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	    if(config_extrafontdata) {
		int oldrender = state->getRender();
		state->setRender(3); //invisible
		gfxdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
		state->setRender(oldrender);
	    }
	} else {
	    /* this char is not at all affected by clipping. 
	       Now just dump out the bitmap we're currently working on, if necessary. */
	    booltextdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	    checkNewText();
	    /* use polygonal output device to do the actual text handling */
	    gfxdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	}
    }
}
void BitmapOutputDev::drawString(GfxState *state, GString *s)
{
    msg("<error> internal error: drawString not implemented");
    return;
    clip0dev->drawString(state, s);
    clip1dev->drawString(state, s);
    booltextdev->drawString(state, s);
    gfxdev->drawString(state, s);
}
void BitmapOutputDev::endTextObject(GfxState *state)
{
    msg("<debug> endTextObject");
    rgbdev->endTextObject(state);
    clip0dev->endTextObject(state);
    clip1dev->endTextObject(state);
    booltextdev->endTextObject(state);
    checkNewText();
    gfxdev->endTextObject(state);
}

/* TODO: these four operations below *should* do nothing, as type3
         chars are drawn using operations like fill() */
GBool BitmapOutputDev::beginType3Char(GfxState *state, double x, double y,
			     double dx, double dy,
			     CharCode code, Unicode *u, int uLen)
{
    msg("<debug> beginType3Char");
    /* call gfxdev so that it can generate "invisible" characters
       on top of the actual graphic content, for text extraction */
    return gfxdev->beginType3Char(state, x, y, dx, dy, code, u, uLen);
}
void BitmapOutputDev::type3D0(GfxState *state, double wx, double wy)
{
    msg("<debug> type3D0");
    return gfxdev->type3D0(state, wx, wy);
}
void BitmapOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury)
{
    msg("<debug> type3D1");
    return gfxdev->type3D1(state, wx, wy, llx, lly, urx, ury);
}
void BitmapOutputDev::endType3Char(GfxState *state)
{
    msg("<debug> endType3Char");
    gfxdev->endType3Char(state);
}

class CopyStream: public Object
{
    Dict*dict;
    char*buf;
    MemStream*memstream;
    public:
    CopyStream(Stream*str, int len)
    {
	buf = 0;
	str->reset();
	if(len) {
	    buf = (char*)malloc(len);
	    int t;
	    for (t=0; t<len; t++)
	      buf[t] = str->getChar();
	}
	str->close();
	this->dict = str->getDict();
	this->memstream = new MemStream(buf, 0, len, this);
    }
    ~CopyStream() 
    {
	::free(this->buf);this->buf = 0;
	delete this->memstream;
    }
    Dict* getDict() {return dict;}
    Stream* getStream() {return this->memstream;};
};

void BitmapOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
			   int width, int height, GBool invert,
			   GBool inlineImg)
{
    msg("<debug> drawImageMask streamkind=%d", str->getKind());
    CopyStream*cpystr = 0;
    if(inlineImg) {
	cpystr = new CopyStream(str, height * ((width + 7) / 8));
	str = cpystr->getStream();
    }
    boolpolydev->drawImageMask(state, ref, str, width, height, invert, inlineImg);
    checkNewBitmap();
    rgbdev->drawImageMask(state, ref, str, width, height, invert, inlineImg);
    if(cpystr)
	delete cpystr;
}
void BitmapOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
		       int width, int height, GfxImageColorMap *colorMap,
		       int *maskColors, GBool inlineImg)
{
    msg("<debug> drawImage streamkind=%d", str->getKind());
    CopyStream*cpystr = 0;
    if(inlineImg) {
	cpystr = new CopyStream(str, height * ((width * colorMap->getNumPixelComps() * colorMap->getBits() + 7) / 8));
	str = cpystr->getStream();
    }
    boolpolydev->drawImage(state, ref, str, width, height, colorMap, maskColors, inlineImg);
    checkNewBitmap();
    rgbdev->drawImage(state, ref, str, width, height, colorMap, maskColors, inlineImg);
    if(cpystr)
	delete cpystr;
}
void BitmapOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			     int width, int height,
			     GfxImageColorMap *colorMap,
			     Stream *maskStr, int maskWidth, int maskHeight,
			     GBool maskInvert)
{
    msg("<debug> drawMaskedImage streamkind=%d", str->getKind());
    boolpolydev->drawMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskInvert);
    checkNewBitmap();
    rgbdev->drawMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskInvert);
}
void BitmapOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				 int width, int height,
				 GfxImageColorMap *colorMap,
				 Stream *maskStr,
				 int maskWidth, int maskHeight,
				 GfxImageColorMap *maskColorMap)
{
    msg("<debug> drawSoftMaskedImage %dx%d (%dx%d) streamkind=%d", width, height, maskWidth, maskHeight, str->getKind());
    boolpolydev->drawSoftMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskColorMap);
    checkNewBitmap();
    rgbdev->drawSoftMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskColorMap);
}
void BitmapOutputDev::drawForm(Ref id)
{
    msg("<debug> drawForm");
    boolpolydev->drawForm(id);
    checkNewBitmap();
    rgbdev->drawForm(id);
}

void BitmapOutputDev::processLink(Link *link, Catalog *catalog)
{
    msg("<debug> processLink");
    gfxdev->processLink(link, catalog);
}

void BitmapOutputDev::beginTransparencyGroup(GfxState *state, double *bbox,
				    GfxColorSpace *blendingColorSpace,
				    GBool isolated, GBool knockout,
				    GBool forSoftMask)
{
    msg("<debug> beginTransparencyGroup");
#if (xpdfMajorVersion*10000 + xpdfMinorVersion*100 + xpdfUpdateVersion) < 30207
    GfxState*state1 = state->copy();
    GfxState*state2 = state->copy();
    state1->setPath(0);
    state1->setPath(state->getPath()->copy());
    state2->setPath(0);
    state2->setPath(state->getPath()->copy());
#else
    GfxState*state1 = state->copy(gTrue);
    GfxState*state2 = state->copy(gTrue);
#endif
    boolpolydev->beginTransparencyGroup(state1, bbox, blendingColorSpace, isolated, knockout, forSoftMask);
    rgbdev->beginTransparencyGroup(state2, bbox, blendingColorSpace, isolated, knockout, forSoftMask);
    clip1dev->beginTransparencyGroup(state, bbox, blendingColorSpace, isolated, knockout, forSoftMask);
    delete state1;
    delete state2;
}
void BitmapOutputDev::endTransparencyGroup(GfxState *state)
{
    msg("<debug> endTransparencyGroup");
#if (xpdfMajorVersion*10000 + xpdfMinorVersion*100 + xpdfUpdateVersion) < 30207
    GfxState*state1 = state->copy();
    GfxState*state2 = state->copy();
    state1->setPath(0);
    state1->setPath(state->getPath()->copy());
    state2->setPath(0);
    state2->setPath(state->getPath()->copy());
#else
    GfxState*state1 = state->copy(gTrue);
    GfxState*state2 = state->copy(gTrue);
#endif
    boolpolydev->endTransparencyGroup(state1);
    checkNewBitmap();
    rgbdev->endTransparencyGroup(state2);
    delete state1;
    delete state2;
    clip1dev->endTransparencyGroup(state);
}
void BitmapOutputDev::paintTransparencyGroup(GfxState *state, double *bbox)
{
    msg("<debug> paintTransparencyGroup");
    boolpolydev->paintTransparencyGroup(state,bbox);
    checkNewBitmap();
    rgbdev->paintTransparencyGroup(state,bbox);
    clip1dev->paintTransparencyGroup(state,bbox);
}
void BitmapOutputDev::setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *backdropColor)
{
    msg("<debug> setSoftMask");
    boolpolydev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
    checkNewBitmap();
    rgbdev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
    clip1dev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
}
void BitmapOutputDev::clearSoftMask(GfxState *state)
{
    msg("<debug> clearSoftMask");
    boolpolydev->clearSoftMask(state);
    checkNewBitmap();
    rgbdev->clearSoftMask(state);
    clip1dev->clearSoftMask(state);
}
