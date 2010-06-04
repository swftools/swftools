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
#include "../gfxtools.h"
#include "../types.h"
#include "bbox.h"

#define UNKNOWN_BOUNDING_BOX 0,0,0,0

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
    SplashColorMode colorMode = splashModeMono1;

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
    gfxdevice_record_init(this->gfxoutput, 0);

    this->gfxdev->setDevice(this->gfxoutput);
    
    this->config_extrafontdata = 0;
    this->config_optimizeplaincolorfills = 0;
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
    if(this->stalepolybitmap) {
	delete this->stalepolybitmap;this->stalepolybitmap = 0;
    }
    if(this->staletextbitmap) {
	delete this->staletextbitmap;this->staletextbitmap = 0;
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
void BitmapOutputDev::setPageMap(int*page2page, int num_pages)
{
    this->gfxdev->setPageMap(page2page, num_pages);
}

void writeBitmap(SplashBitmap*bitmap, char*filename);
void writeAlpha(SplashBitmap*bitmap, char*filename);

static int dbg_btm_counter=1;

void BitmapOutputDev::flushBitmap()
{
    int width = rgbdev->getBitmapWidth();
    int height = rgbdev->getBitmapHeight();

    if(sizeof(SplashColor)!=3) {
	msg("<error> sizeof(SplashColor)!=3");
	return;
    }

    /*static int counter=0;
    if(!counter) {
	writeBitmap(rgbdev->getBitmap(), "test.png");
    } counter++;*/
    
    /*static int counter=0;
    char filename[160];
    sprintf(filename, "test%d.png", counter++);
    writeBitmap(rgbbitmap, filename);*/
    
    SplashColorPtr rgb = rgbbitmap->getDataPtr();
    Guchar*alpha = rgbbitmap->getAlphaPtr();
    
    Guchar*alpha2 = stalepolybitmap->getDataPtr();
    int width8 = (stalepolybitmap->getWidth()+7)/8;

    /*char filename[80];
    sprintf(filename, "flush%d_mask.png", dbg_btm_counter);
    writeAlpha(stalepolybitmap, filename);
    sprintf(filename, "flush%d_alpha.png", dbg_btm_counter);
    writeAlpha(rgbbitmap, filename);
    sprintf(filename, "flush%d_bitmap.png", dbg_btm_counter);
    writeBitmap(rgbbitmap, filename);*/

    ibbox_t* boxes = get_bitmap_bboxes((unsigned char*)alpha, width, height);
    ibbox_t*b;

    for(b=boxes;b;b=b->next) {
	int xmin = b->xmin;
	int ymin = b->ymin;
	int xmax = b->xmax;
	int ymax = b->ymax;

	/* clip against (-movex, -movey, -movex+width, -movey+height) */

	msg("<verbose> Flushing bitmap (bbox: %d,%d,%d,%d %dx%d) (clipped against %d,%d,%d,%d)", xmin,ymin,xmax,ymax, xmax-xmin, ymax-ymin,
		-this->movex, -this->movey, -this->movex+this->width, -this->movey+this->height);

	if(xmin < -this->movex) {
	    xmin = -this->movex;
	    if(xmax < -this->movex) continue;
	}
	if(ymin < -this->movey) {
	    ymin = -this->movey;
	    if(ymax < -this->movey) continue;
	}
	if(xmax >= -this->movex + this->width) {
	    xmax = -this->movex+this->width;
	    if(xmin >= -this->movex + this->width) continue;
	}
	if(ymax >= -this->movey + this->height) {
	    ymax = -this->movey+this->height;
	    if(ymin >= -this->movey + this->height) continue;
	}
	
	if((xmax-xmin)<=0 || (ymax-ymin)<=0) // no bitmap, nothing to do
	    continue;

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
	    Guchar*ain2 = &alpha2[(y+ymin)*width8];
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
		    if(!(ain2[(x+xmin)/8]&(0x80>>((x+xmin)&7)))) {
			/* cut away pixels that we don't remember drawing (i.e., that are
			   not in the monochrome bitmap). Prevents some "hairlines" showing
			   up to the left and right of bitmaps. */
			out[x].r = 0;out[x].g = 0;out[x].b = 0;out[x].a = 0;
		    } else {
			/* according to endPage()/compositeBackground() in xpdf/SplashOutputDev.cc, this
			   data has non-premultiplied alpha, which is exactly what the output device 
			   expects, so don't premultiply it here, either.
			*/
			out[x].r = in[x*3+0];
			out[x].g = in[x*3+1];
			out[x].b = in[x*3+2];
			out[x].a = ain[x];
		    }
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
	m.tx -= 0.5;
	m.ty -= 0.5;

	gfxline_t* line = gfxline_makerectangle(xmin, ymin, xmax, ymax);
	dev->fillbitmap(dev, line, img, &m, 0);
	gfxline_free(line);
    
	free(img->data);img->data=0;free(img);img=0;
    }
    ibbox_destroy(boxes);

    memset(rgbbitmap->getAlphaPtr(), 0, rgbbitmap->getWidth()*rgbbitmap->getHeight());
    memset(rgbbitmap->getDataPtr(), 0, rgbbitmap->getRowSize()*rgbbitmap->getHeight());

    this->emptypage = 0;
}

void BitmapOutputDev::flushText()
{
    msg("<verbose> Flushing text");

    static gfxfontlist_t*output_font_list = 0;
    static gfxdevice_t*last = 0;
    if(last != this->dev) {
	if(output_font_list)
	    gfxfontlist_free(output_font_list, 0);
	output_font_list = gfxfontlist_create();
    }
    gfxdevice_record_flush(this->gfxoutput, this->dev, &output_font_list);
    last = this->dev;
    
    this->emptypage = 0;
}

void writeMonoBitmap(SplashBitmap*btm, char*filename)
{
    int width8 = (btm->getWidth()+7)/8;
    int width = btm->getWidth();
    int height = btm->getHeight();
    gfxcolor_t*b = (gfxcolor_t*)malloc(sizeof(gfxcolor_t)*width*height);
    unsigned char*data = btm->getDataPtr();
    int x,y;
    for(y=0;y<height;y++) {
        unsigned char*l = &data[width8*y];
        gfxcolor_t*d = &b[width*y];
        for(x=0;x<width;x++) {
            if(l[x>>3]&(128>>(x&7))) {
                d[x].r = d[x].b = d[x].a = 255;
		d[x].g = 0;
            } else {
                d[x].r = d[x].g = d[x].b = d[x].a = 0;
            }
        }
    }
    writePNG(filename, (unsigned char*)b, width, height);
    free(b);
}

void writeBitmap(SplashBitmap*bitmap, char*filename)
{
    int y,x;
    
    int width = bitmap->getWidth();
    int height = bitmap->getHeight();

    gfxcolor_t*data = (gfxcolor_t*)malloc(sizeof(gfxcolor_t)*width*height);

    if(bitmap->getMode()==splashModeMono1) {
        writeMonoBitmap(bitmap, filename);
        return;
    }

    for(y=0;y<height;y++) {
	gfxcolor_t*line = &data[y*width];
	for(x=0;x<width;x++) {
            Guchar c[4] = {0,0,0,0};
	    bitmap->getPixel(x,y,c);
	    line[x].r = c[0];
	    line[x].g = c[1];
	    line[x].b = c[2];
	    line[x].a =  bitmap->getAlpha(x,y);
	}
    }
    writePNG(filename, (unsigned char*)data, width, height);
    free(data);
}

void writeAlpha(SplashBitmap*bitmap, char*filename)
{
    int y,x;
    
    int width = bitmap->getWidth();
    int height = bitmap->getHeight();
    
    if(bitmap->getMode()==splashModeMono1) {
        writeMonoBitmap(bitmap, filename);
        return;
    }

    gfxcolor_t*data = (gfxcolor_t*)malloc(sizeof(gfxcolor_t)*width*height);

    for(y=0;y<height;y++) {
	gfxcolor_t*line = &data[y*width];
	for(x=0;x<width;x++) {
	    int a = bitmap->getAlpha(x,y);
	    line[x].r = a;
	    line[x].g = 0;
	    line[x].b = a;
	    line[x].a = a;
	}
    }
    writePNG(filename, (unsigned char*)data, width, height);
    free(data);
}

static const char*STATE_NAME[] = {"parallel", "textabovebitmap", "bitmapabovetext"};

int checkAlphaSanity(SplashBitmap*boolbtm, SplashBitmap*alphabtm)
{
    assert(boolbtm->getWidth() == alphabtm->getWidth());
    assert(boolbtm->getHeight() == alphabtm->getHeight());
    if(boolbtm->getMode()==splashModeMono1) {
        return 1;
    }

    int width = boolbtm->getWidth();
    int height = boolbtm->getHeight();

    int bad=0;
    int x,y;
    for(y=0;y<height;y++) {
	for(x=0;x<width;x++) {
	    int a1 = alphabtm->getAlpha(x,y);
	    int a2 = boolbtm->getAlpha(x,y);
            if(a1!=a2) {
                bad++;
            }
	}
    }
    double badness = bad/(double)(width*height);
    if(badness>0.2) {
        msg("<error> Bitmaps don't correspond: %d out of %d pixels wrong (%.2f%%)", bad, width*height, 
                badness*100.0);
        return 0;
    }
    msg("<notice> %f", badness);
    return 1;
}

static inline GBool fixBBox(int*x1, int*y1, int*x2, int*y2, int width, int height)
{
    if(!(*x1|*y1|*x2|*y2)) {
        // undefined bbox
        *x1 = *y1 = 0;
        *x2 = width;
        *y2 = height;
        return gTrue;
    }
    if(*x2<=*x1) return gFalse;
    if(*x2<0) return gFalse;
    if(*x1<0) *x1 = 0;
    if(*x1>=width) return gFalse;
    if(*x2>width) *x2=width;

    if(*y2<=*y1) return gFalse;
    if(*y2<0) return gFalse;
    if(*y1<0) *y1 = 0;
    if(*y1>=height) return gFalse;
    if(*y2>height) *y2=height;
    return gTrue;
}

static void update_bitmap(SplashBitmap*bitmap, SplashBitmap*update, int x1, int y1, int x2, int y2, char overwrite)
{
    assert(bitmap->getMode()==splashModeMono1);
    assert(update->getMode()==splashModeMono1);

    int width8 = (bitmap->getWidth()+7)/8;
    assert(width8 == bitmap->getRowSize());
    assert(width8 == update->getRowSize());
    int height = bitmap->getHeight();
    assert(height == update->getHeight());

    if(!fixBBox(&x1, &y1, &x2, &y2, bitmap->getWidth(), bitmap->getHeight()))
	return;
    
    Guchar*b = bitmap->getDataPtr() + y1*width8 + x1/8;
    Guchar*u = update->getDataPtr() + y1*width8 + x1/8;
    int yspan = y2-y1;
    int xspan = (x2+7)/8 - x1/8;
    int size = (y2-y1)*width8;

    if(overwrite) {
	int y;
	for(y=0;y<yspan;y++) {
	    memcpy(b, u, xspan);
	    b += width8;
	    u += width8;
	}
    } else {
	if(((ptroff_t)b&7)==((ptroff_t)u&7)) {
	    int x,y;
	    for(y=0;y<yspan;y++) {
		Guchar*e1 = b+xspan-8;
		Guchar*e2 = b+xspan;
		while(((ptroff_t)b&7) && b<e1) {
		    *b |= *u;
		    b++;u++;
		}
		while(b<e1) {
		    *(long long*)b |= *(long long*)u;
		    b+=8;u+=8;
		}
		while(b<e2) {
		    *b |= *u;
		    b++;u++;
		}
		b += width8-xspan;
		u += width8-xspan;
	    }
	} else {
	    int x,y;
	    for(y=0;y<yspan;y++) {
		for(x=0;x<xspan;x++) {
		    b[x] |= u[x];
		}
		b += width8;
		u += width8;
	    }
	}
    }
}

static void clearBooleanBitmap(SplashBitmap*btm, int x1, int y1, int x2, int y2)
{
    if(!fixBBox(&x1, &y1, &x2, &y2, btm->getWidth(), btm->getHeight()))
	return;
    
    if(btm->getMode()==splashModeMono1) {
	int width8 = (btm->getWidth()+7)/8;
	assert(width8 == btm->getRowSize());
	int width = btm->getWidth();
	int height = btm->getHeight();
	Guchar*data = btm->getDataPtr();
	memset(data+y1*width8, 0, width8*(y2-y1));
    } else {
	int width = btm->getAlphaRowSize();
	int height = btm->getHeight();
	memset(btm->getAlphaPtr(), 0, width*height);
    }
}

void BitmapOutputDev::dbg_newdata(char*newdata)
{
    if(0) {
        char filename1[80];
        char filename2[80];
        char filename3[80];
        sprintf(filename1, "state%03dboolbitmap_after%s.png", dbg_btm_counter, newdata);
        sprintf(filename2, "state%03dbooltext_after%s.png", dbg_btm_counter, newdata);
        sprintf(filename3, "state%03dbitmap_after%s.png", dbg_btm_counter, newdata);
        msg("<verbose> %s %s %s", filename1, filename2, filename3);
	writeAlpha(stalepolybitmap, filename1);
	writeAlpha(booltextbitmap, filename2);
	writeBitmap(rgbdev->getBitmap(), filename3);
    }
    dbg_btm_counter++;
}

GBool BitmapOutputDev::checkNewText(int x1, int y1, int x2, int y2)
{
    /* called once some new text was drawn on booltextdev, and
       before the same thing is drawn on gfxdev */
   
    msg("<trace> Testing new text data against current bitmap data, state=%s, counter=%d\n", STATE_NAME[layerstate], dbg_btm_counter);
    
    GBool ret = false;
    if(intersection(booltextbitmap, stalepolybitmap, x1,y1,x2,y2)) {
	if(layerstate==STATE_PARALLEL) {
	    /* the new text is above the bitmap. So record that fact. */
	    msg("<verbose> Text is above current bitmap/polygon data");
	    layerstate=STATE_TEXT_IS_ABOVE;
	    update_bitmap(staletextbitmap, booltextbitmap, x1, y1, x2, y2, 0);
	} else if(layerstate==STATE_BITMAP_IS_ABOVE) {
	    /* there's a bitmap above the (old) text. So we need
	       to flush out that text, and record that the *new*
	       text is now *above* the bitmap
	     */
	    msg("<verbose> Text is above current bitmap/polygon data (which is above some other text)");
	    flushText();
	    layerstate=STATE_TEXT_IS_ABOVE;
	   
	    clearBoolTextDev();
	    /* re-apply the update (which we would otherwise lose) */
	    update_bitmap(staletextbitmap, booltextbitmap, x1, y1, x2, y2, 1);
            ret = true;
	} else {
	    /* we already know that the current text section is
	       above the current bitmap section- now just new
	       bitmap data *and* new text data was drawn, and
	       *again* it's above the current bitmap. */
	    msg("<verbose> Text is still above current bitmap/polygon data");
	    update_bitmap(staletextbitmap, booltextbitmap, x1, y1, x2, y2, 0);
	}
    }  else {
	update_bitmap(staletextbitmap, booltextbitmap, x1, y1, x2, y2, 0);
    }
    
    /* clear the thing we just drew from our temporary drawing bitmap */
    clearBooleanBitmap(booltextbitmap, x1, y1, x2, y2);

    return ret;
}
GBool BitmapOutputDev::checkNewBitmap(int x1, int y1, int x2, int y2)
{
    /* similar to checkNewText() above, only in reverse */
    msg("<trace> Testing new graphics data against current text data, state=%s, counter=%d\n", STATE_NAME[layerstate], dbg_btm_counter);

    GBool ret = false;
    if(intersection(boolpolybitmap, staletextbitmap, x1,y1,x2,y2)) {
	if(layerstate==STATE_PARALLEL) {
	    msg("<verbose> Bitmap is above current text data");
	    layerstate=STATE_BITMAP_IS_ABOVE;
	    update_bitmap(stalepolybitmap, boolpolybitmap, x1, y1, x2, y2, 0);
	} else if(layerstate==STATE_TEXT_IS_ABOVE) {
	    msg("<verbose> Bitmap is above current text data (which is above some bitmap)");
	    flushBitmap();
	    layerstate=STATE_BITMAP_IS_ABOVE;
	    clearBoolPolyDev();
	    update_bitmap(stalepolybitmap, boolpolybitmap, x1, y1, x2, y2, 1);
            ret = true;
	} else {
	    msg("<verbose> Bitmap is still above current text data");
	    update_bitmap(stalepolybitmap, boolpolybitmap, x1, y1, x2, y2, 0);
	}
    }  else {
	update_bitmap(stalepolybitmap, boolpolybitmap, x1, y1, x2, y2, 0);
    }
    
    /* clear the thing we just drew from our temporary drawing bitmap */
    clearBooleanBitmap(boolpolybitmap, x1, y1, x2, y2);

    return ret;
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

GBool BitmapOutputDev::clip0and1differ(int x1,int y1,int x2,int y2)
{
    if(clip0bitmap->getMode()==splashModeMono1) {
        int width = clip0bitmap->getWidth();
	int width8 = (width+7)/8;
	int height = clip0bitmap->getHeight();

        if(!fixBBox(&x1,&y1,&x2,&y2,width,height)) {
            /* area is outside or null */
            return gFalse;
        }
	
	SplashBitmap*clip0 = clip0bitmap;
	SplashBitmap*clip1 = clip1bitmap;
	int x18 = x1/8;
	int x28 = (x2+7)/8;
	int y;

	for(y=y1;y<y2;y++) {
	    unsigned char*row1 = &clip0bitmap->getDataPtr()[width8*y+x18];
	    unsigned char*row2 = &clip1bitmap->getDataPtr()[width8*y+x18];
	    if(memcmp(row1, row2, x28-x18)) {
		return gTrue;
            }
	}
	return gFalse;
    } else {
	SplashBitmap*clip0 = clip0bitmap;
	SplashBitmap*clip1 = clip1bitmap;
	int width = clip0->getAlphaRowSize();
	int height = clip0->getHeight();

        if(!fixBBox(&x1, &y1, &x2, &y2, width, height)) {
            x1=y1=0;x2=y2=1;
        }

        Guchar*a0 = clip0->getAlphaPtr();
        Guchar*a1 = clip1->getAlphaPtr();
        int x,y;
        char differs=0;
        for(y=y1;y<y2;y++) {
            for(x=x1;x<x2;x++) {
                if(a0[y*width+x]!=a1[y*width+x]) {
                    differs=1;
                    break;
                }
            }
            if(differs)
                break;
        }
	char differs2 = memcmp(a0, a1, width*height);
        if(differs && !differs2) 
            msg("<warning> Strange internal error (2)");
        else if(!differs && differs2) {
            msg("<warning> Bad Bounding Box: Difference in clip0 and clip1 outside bbox");
            msg("<warning> %d %d %d %d", x1, y1, x2, y2);
        }
        return differs2;
    }
}

GBool compare8(unsigned char*data1, unsigned char*data2, int len)
{
    if(!len)
        return 0;
    if(((ptroff_t)data1&7)==((ptroff_t)data2&7)) {
        // oh good, we can align both to 8 byte
        while((ptroff_t)data1&7) {
            if(*data1&*data2)
                return 1;
            data1++;
            data2++;
            if(!--len)
                return 0;
        }
    }
    /* use 64 bit for the (hopefully aligned) middle section */
    int l8 = len/8;
    long long unsigned int*d1 = (long long unsigned int*)data1;
    long long unsigned int*d2 = (long long unsigned int*)data2;
    long long unsigned int x = 0;
    int t;
    for(t=0;t<l8;t++) {
        x |= d1[t]&d2[t];
    }
    if(x)
        return 1;

    data1+=l8*8;
    data2+=l8*8;
    len -= l8*8;
    for(t=0;t<len;t++) {
        if(data1[t]&data2[t]) {
            return 1;
        }
    }
    return 0;
}

GBool BitmapOutputDev::intersection(SplashBitmap*boolpoly, SplashBitmap*booltext, int x1, int y1, int x2, int y2)
{
    if(boolpoly->getMode()==splashModeMono1) {
	/* alternative implementation, using one bit per pixel-
	   needs the no-dither patch in xpdf */
	
        int width = boolpoly->getWidth();
	int height = boolpoly->getHeight();

        if(!fixBBox(&x1,&y1,&x2,&y2, width, height)) {
            return gFalse;
        }

	Guchar*polypixels = boolpoly->getDataPtr();
	Guchar*textpixels = booltext->getDataPtr();

        int width8 = (width+7)/8;
        int runx = width8;
        int runy = height;
	
	if(x1|y1|x2|y2) {
            polypixels+=y1*width8+x1/8;
            textpixels+=y1*width8+x1/8;
            runx=(x2+7)/8 - x1/8;
            runy=y2-y1;
	}
	
	int t;
	unsigned char c=0;
        
        /*assert(sizeof(unsigned long long int)==8);
        if(((ptroff_t)polypixels&7) || ((ptroff_t)textpixels&7)) {
            //msg("<warning> Non-optimal alignment");
        }*/

        int x,y;
        unsigned char*data1 = (unsigned char*)polypixels;
        unsigned char*data2 = (unsigned char*)textpixels;
        msg("<verbose> Testing area (%d,%d,%d,%d), runx=%d,runy=%d", x1,y1,x2,y2, runx, runy);
        for(y=0;y<runy;y++) {
            if(compare8(data1,data2,runx))
                return gTrue;
            data1+=width8;
            data2+=width8;
        }
        return gFalse;
    } else {
	int width = boolpoly->getAlphaRowSize();
	int height = boolpoly->getHeight();

        if(!fixBBox(&x1, &y1, &x2, &y2, width, height)) {
            x1=y1=0;x2=y2=1;
        }
	Guchar*polypixels = boolpoly->getAlphaPtr();
	Guchar*textpixels = booltext->getAlphaPtr();

        int x,y;
        char overlap1 = 0;
        char overlap2 = 0;
        for(x=x1;x<x2;x++) {
            for(y=y1;y<y2;y++) {
                if(polypixels[width*y+x]&&textpixels[width*y+x])
                    overlap1 = 1;
            }
        }
        int ax1=0,ay1=0,ax2=0,ay2=0;
        for(y=0;y<height;y++) {
            for(x=0;x<width;x++) {
                if(polypixels[width*y+x]&&textpixels[width*y+x]) {
                    overlap2 = 1;
                    if(!(ax1|ay1|ax2|ay2)) {
                        ax1 = ax2 = x;
                        ay1 = ay2 = y;
                    } else {
                        ax1 = ax1<x?ax1:x;
                        ay1 = ay1<y?ay1:y;
                        ax2 = ax2>x?ax2:x;
                        ay2 = ay2>y?ay2:y;
                    }
                }
            }
        }
        if(overlap1 && !overlap2)
            msg("<warning> strange internal error");
        if(!overlap1 && overlap2) {
            msg("<warning> Bad bounding box: intersection outside bbox");
            msg("<warning> given bbox: %d %d %d %d", x1, y1, x2, y2);
            msg("<warning> changed area: %d %d %d %d", ax1, ay1, ax2, ay2);
        }
	return overlap2;
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

    rgbdev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    boolpolydev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    booltextdev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    clip0dev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    clip1dev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);
    gfxdev->startPage(pageNum, state, crop_x1, crop_y1, crop_x2, crop_y2);

    boolpolybitmap = boolpolydev->getBitmap();
    stalepolybitmap = new SplashBitmap(boolpolybitmap->getWidth(), boolpolybitmap->getHeight(), 1, boolpolybitmap->getMode(), 0);
    assert(stalepolybitmap->getRowSize() == boolpolybitmap->getRowSize());

    booltextbitmap = booltextdev->getBitmap();
    staletextbitmap = new SplashBitmap(booltextbitmap->getWidth(), booltextbitmap->getHeight(), 1, booltextbitmap->getMode(), 0);
    assert(staletextbitmap->getRowSize() == booltextbitmap->getRowSize());
    
    msg("<debug> startPage %dx%d (%dx%d)", this->width, this->height, booltextbitmap->getWidth(), booltextbitmap->getHeight());

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
   
    flushEverything();

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

double max(double x, double y) 
{
    return x>y?x:y;
}
double min(double x, double y) 
{
    return x<y?x:y;
}

gfxbbox_t BitmapOutputDev::getBBox(GfxState*state)
{
    GfxPath * path = state->getPath();
    int num = path->getNumSubpaths();
    gfxbbox_t bbox = {0,0,1,1};
    char valid=0;
    int t;
    for(t = 0; t < num; t++) {
	GfxSubpath *subpath = path->getSubpath(t);
	int subnum = subpath->getNumPoints();
        int s;
	for(s=0;s<subnum;s++) {
	   double x,y;
	   state->transform(subpath->getX(s),subpath->getY(s),&x,&y);
           if(!valid) {
               bbox.xmin = x; bbox.ymin = y;
               bbox.xmax = x; bbox.ymax = y;
               valid = 1;
           } else {
               bbox.xmin = min(bbox.xmin, x);
               bbox.ymin = min(bbox.ymin, y);
               bbox.xmax = max(bbox.xmax, x);
               bbox.ymax = max(bbox.ymax, y);
           }
        }
    }
    return bbox;
}

void BitmapOutputDev::stroke(GfxState *state)
{
    msg("<debug> stroke");
    boolpolydev->stroke(state);
    gfxbbox_t bbox = getBBox(state);
    double width = state->getTransformedLineWidth();
    bbox.xmin -= width; bbox.ymin -= width;
    bbox.xmax += width; bbox.ymax += width;
    checkNewBitmap(bbox.xmin, bbox.ymin, ceil(bbox.xmax), ceil(bbox.ymax));
    rgbdev->stroke(state);
    dbg_newdata("stroke");
}

extern gfxcolor_t getFillColor(GfxState * state);

char area_is_plain_colored(GfxState*state, SplashBitmap*boolpoly, SplashBitmap*rgbbitmap, int x1, int y1, int x2, int y2)
{
    int width = boolpoly->getWidth();
    int height = boolpoly->getHeight();
    if(!fixBBox(&x1, &y1, &x2, &y2, width, height)) {
	return 0;
    }
    gfxcolor_t color = getFillColor(state);
    SplashColorPtr rgb = rgbbitmap->getDataPtr() 
	               + (y1*width+x1)*sizeof(SplashColor);
    int width8 = (width+7)/8;
    unsigned char*bits = (unsigned char*)boolpoly->getDataPtr() 
	                 + (y1*width8+x1);
    int x,y;
    int w = x2-x1;
    int h = y2-y1;
    for(y=0;y<h;y++) {
	for(x=0;x<w;x++) {
	    if(rgb[x*3+0] != color.r ||
	       rgb[x*3+1] != color.g ||
	       rgb[x*3+2] != color.b)
		return 0;
	}
	rgb += width*sizeof(SplashColor);
    }
    return 1;
}

void BitmapOutputDev::fill(GfxState *state)
{
    msg("<debug> fill");
    boolpolydev->fill(state);
    gfxbbox_t bbox = getBBox(state);
    if(config_optimizeplaincolorfills) {
	if(area_is_plain_colored(state, boolpolybitmap, rgbbitmap, bbox.xmin, bbox.ymin, bbox.xmax, bbox.ymax)) {
	    return;
	}
    }
    checkNewBitmap(bbox.xmin, bbox.ymin, ceil(bbox.xmax), ceil(bbox.ymax));
    rgbdev->fill(state);
    dbg_newdata("fill");
}
void BitmapOutputDev::eoFill(GfxState *state)
{
    msg("<debug> eoFill");
    boolpolydev->eoFill(state);
    gfxbbox_t bbox = getBBox(state);
    checkNewBitmap(bbox.xmin, bbox.ymin, ceil(bbox.xmax), ceil(bbox.ymax));
    rgbdev->eoFill(state);
    dbg_newdata("eofill");
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
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
    rgbdev->tilingPatternFill(state, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
    dbg_newdata("tilingpatternfill");
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
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
    rgbdev->tilingPatternFill(state, gfx, str, paintType, resDict, mat, bbox, x0, y0, x1, y1, xStep, yStep);
    dbg_newdata("tilingpatternfill");
}
#endif

GBool BitmapOutputDev::functionShadedFill(GfxState *state, GfxFunctionShading *shading) 
{
    msg("<debug> functionShadedFill");
    boolpolydev->functionShadedFill(state, shading);
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
    return rgbdev->functionShadedFill(state, shading);
}
GBool BitmapOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading)
{
    msg("<debug> axialShadedFill");
    boolpolydev->axialShadedFill(state, shading);
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
    return rgbdev->axialShadedFill(state, shading);
}
GBool BitmapOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading)
{
    msg("<debug> radialShadedFill");
    boolpolydev->radialShadedFill(state, shading);
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
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
void BitmapOutputDev::beginString(GfxState *state, GString *s)
{
    msg("<debug> beginString");
    clip0dev->beginString(state, s);
    clip1dev->beginString(state, s);
    booltextdev->beginString(state, s);
    gfxdev->beginString(state, s);
}

void BitmapOutputDev::clearClips()
{
    clearBooleanBitmap(clip0bitmap, 0, 0, 0, 0);
    clearBooleanBitmap(clip1bitmap, 0, 0, 0, 0);
}
void BitmapOutputDev::clearBoolPolyDev()
{
    clearBooleanBitmap(stalepolybitmap, 0, 0, stalepolybitmap->getWidth(), stalepolybitmap->getHeight());
}
void BitmapOutputDev::clearBoolTextDev()
{
    clearBooleanBitmap(staletextbitmap, 0, 0, staletextbitmap->getWidth(), staletextbitmap->getHeight());
}
void BitmapOutputDev::drawChar(GfxState *state, double x, double y,
		      double dx, double dy,
		      double originX, double originY,
		      CharCode code, int nBytes, Unicode *u, int uLen)
{
    msg("<debug> drawChar render=%d", state->getRender());

    if(state->getRender()&RENDER_CLIP) {
	//char is just a clipping boundary
	rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
        boolpolydev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
        booltextdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
        clip1dev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
    } else if(state->getRender()&RENDER_STROKE) {
	// we're drawing as stroke
	boolpolydev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
    } else if(rgbbitmap != rgbdev->getBitmap()) {
	// we're doing softmasking or transparency grouping
	boolpolydev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	rgbdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
    } else {
	// we're drawing a regular char
	clearClips();
	clip0dev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	clip1dev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
   
	/* calculate the bbox of this character */
	int x1 = (int)x, x2 = (int)x+1, y1 = (int)y, y2 = (int)y+1;
        SplashFont*font = clip0dev->getCurrentFont();
	SplashPath*path = font?font->getGlyphPath(code):NULL;
        x-=originX;
        y-=originY;
	if(path) {
	    path->offset((SplashCoord)x, (SplashCoord)y);
	    int t;
	    for(t=0;t<path->getLength();t++) {
		double xx,yy;
		Guchar f;
		path->getPoint(t,&xx,&yy,&f);
		state->transform(xx,yy,&xx,&yy);
		if(xx<x1) x1=(int)xx;
		if(yy<y1) y1=(int)yy;
		if(xx>=x2) x2=(int)xx+1;
		if(yy>=y2) y2=(int)yy+1;
	    }
	    delete(path);path=0;
	}

	/* if this character is affected somehow by the various clippings (i.e., it looks
	   different on a device without clipping), then draw it on the bitmap, not as
	   text */
	if(clip0and1differ(x1,y1,x2,y2)) {
	    msg("<verbose> Char %d is affected by clipping", code);
	    boolpolydev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	    checkNewBitmap(x1,y1,x2,y2);
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
	    checkNewText(x1,y1,x2,y2);
	    /* use polygonal output device to do the actual text handling */
	    gfxdev->drawChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	}
    }
    dbg_newdata("text");
}
void BitmapOutputDev::drawString(GfxState *state, GString *s)
{
    msg("<error> internal error: drawString not implemented");
    return;
}
void BitmapOutputDev::endTextObject(GfxState *state)
{
    msg("<debug> endTextObject");
    rgbdev->endTextObject(state);
    clip0dev->endTextObject(state);
    clip1dev->endTextObject(state);
    booltextdev->endTextObject(state);
    /* the only thing "drawn" here is clipping */
    //checkNewText(UNKNOWN_BOUNDING_BOX);
    gfxdev->endTextObject(state);
    dbg_newdata("endtextobject");
}
void BitmapOutputDev::endString(GfxState *state)
{
    msg("<debug> endString");
    clip0dev->endString(state);
    clip1dev->endString(state);
    booltextdev->endString(state);
    int render = state->getRender();
    if(render != RENDER_INVISIBLE && render != RENDER_FILL) {
	/* xpdf draws things like stroke text or fill+stroke text in the
	   endString() method */
        checkNewText(UNKNOWN_BOUNDING_BOX);
    }
    gfxdev->endString(state);
    dbg_newdata("endstring");
}
void BitmapOutputDev::endStringOp(GfxState *state)
{
    msg("<debug> endStringOp");
    clip0dev->endStringOp(state);
    clip1dev->endStringOp(state);
    booltextdev->endStringOp(state);
    gfxdev->endStringOp(state);
    dbg_newdata("endstringop");
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

gfxbbox_t BitmapOutputDev::getImageBBox(GfxState*state)
{
    gfxbbox_t bbox;
    double x,y;
    state->transform(0, 1, &x, &y);
    bbox.xmin=bbox.xmax = x;
    bbox.ymin=bbox.ymax = y;
    state->transform(0, 0, &x, &y);
    bbox.xmin=min(bbox.xmin,x);
    bbox.ymin=min(bbox.ymin,y);
    bbox.xmax=max(bbox.xmax,x);
    bbox.ymax=max(bbox.ymax,y);
    state->transform(1, 0, &x, &y);
    bbox.xmin=min(bbox.xmin,x);
    bbox.ymin=min(bbox.ymin,y);
    bbox.xmax=max(bbox.xmax,x);
    bbox.ymax=max(bbox.ymax,y);
    state->transform(1, 1, &x, &y);
    bbox.xmin=min(bbox.xmin,x);
    bbox.ymin=min(bbox.ymin,y);
    bbox.xmax=max(bbox.xmax,x);
    bbox.ymax=max(bbox.ymax,y);
    return bbox;
}

GBool invalid_size(int width, int height)
{
    if((U64)width*(U64)height > 0x7fffffffll)
	return 1;
    return 0;
}

void BitmapOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
			   int width, int height, GBool invert,
			   GBool inlineImg)
{
    msg("<debug> drawImageMask streamkind=%d", str->getKind());
    if(invalid_size(width,height)) return;

    CopyStream*cpystr = new CopyStream(str, height * ((width + 7) / 8));
    str = cpystr->getStream();
    
    boolpolydev->drawImageMask(state, ref, str, width, height, invert, inlineImg);
    gfxbbox_t bbox = getImageBBox(state);
    checkNewBitmap(bbox.xmin, bbox.ymin, ceil(bbox.xmax), ceil(bbox.ymax));
    rgbdev->drawImageMask(state, ref, str, width, height, invert, inlineImg);
    delete cpystr;
    dbg_newdata("imagemask");
}
void BitmapOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
		       int width, int height, GfxImageColorMap *colorMap,
		       int *maskColors, GBool inlineImg)
{
    msg("<debug> drawImage streamkind=%d", str->getKind());
    if(invalid_size(width,height)) return;
	
    CopyStream*cpystr = new CopyStream(str, height * ((width * colorMap->getNumPixelComps() * colorMap->getBits() + 7) / 8));
    str = cpystr->getStream();

    boolpolydev->drawImage(state, ref, str, width, height, colorMap, maskColors, inlineImg);
    gfxbbox_t bbox=getImageBBox(state);
    checkNewBitmap(bbox.xmin, bbox.ymin, ceil(bbox.xmax), ceil(bbox.ymax));
    rgbdev->drawImage(state, ref, str, width, height, colorMap, maskColors, inlineImg);
    delete cpystr;
    dbg_newdata("image");
}
void BitmapOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			     int width, int height,
			     GfxImageColorMap *colorMap,
			     Stream *maskStr, int maskWidth, int maskHeight,
			     GBool maskInvert)
{
    msg("<debug> drawMaskedImage streamkind=%d", str->getKind());
    if(invalid_size(width,height)) return;
    
    CopyStream*cpystr = new CopyStream(str, height * ((width * colorMap->getNumPixelComps() * colorMap->getBits() + 7) / 8));
    str = cpystr->getStream();

    boolpolydev->drawMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskInvert);
    gfxbbox_t bbox=getImageBBox(state);
    checkNewBitmap(bbox.xmin, bbox.ymin, ceil(bbox.xmax), ceil(bbox.ymax));
    rgbdev->drawMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskInvert);
    delete cpystr;
    dbg_newdata("maskedimage");
}
void BitmapOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				 int width, int height,
				 GfxImageColorMap *colorMap,
				 Stream *maskStr,
				 int maskWidth, int maskHeight,
				 GfxImageColorMap *maskColorMap)
{
    msg("<debug> drawSoftMaskedImage %dx%d (%dx%d) streamkind=%d", width, height, maskWidth, maskHeight, str->getKind());
    if(invalid_size(width,height)) return;

    CopyStream*cpystr = new CopyStream(str, height * ((width * colorMap->getNumPixelComps() * colorMap->getBits() + 7) / 8));
    str = cpystr->getStream();

    boolpolydev->drawSoftMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskColorMap);
    gfxbbox_t bbox=getImageBBox(state);
    checkNewBitmap(bbox.xmin, bbox.ymin, ceil(bbox.xmax), ceil(bbox.ymax));
    rgbdev->drawSoftMaskedImage(state, ref, str, width, height, colorMap, maskStr, maskWidth, maskHeight, maskColorMap);
    delete cpystr;
    dbg_newdata("softmaskimage");
}
void BitmapOutputDev::drawForm(Ref id)
{
    msg("<debug> drawForm");
    boolpolydev->drawForm(id);
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
    rgbdev->drawForm(id);
}

void BitmapOutputDev::processLink(Link *link, Catalog *catalog)
{
    msg("<debug> processLink");
    flushEverything();
    gfxdev->processLink(link, catalog);
}
void BitmapOutputDev::flushEverything()
{
    if(layerstate == STATE_BITMAP_IS_ABOVE) {
	this->flushText();
	this->flushBitmap();
    } else {
	this->flushBitmap();
	this->flushText();
    }
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
    dbg_newdata("endtransparencygroup");
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
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
    rgbdev->endTransparencyGroup(state2);
    delete state1;
    delete state2;
    clip1dev->endTransparencyGroup(state);
    dbg_newdata("endtransparencygroup");
}
void BitmapOutputDev::paintTransparencyGroup(GfxState *state, double *bbox)
{
    msg("<debug> paintTransparencyGroup");
    boolpolydev->paintTransparencyGroup(state,bbox);
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
    rgbdev->paintTransparencyGroup(state,bbox);
    clip1dev->paintTransparencyGroup(state,bbox);
    dbg_newdata("painttransparencygroup");
}
void BitmapOutputDev::setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *backdropColor)
{
    msg("<debug> setSoftMask");
    boolpolydev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
    rgbdev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
    clip1dev->setSoftMask(state, bbox, alpha, transferFunc, backdropColor);
    dbg_newdata("setsoftmask");
}
void BitmapOutputDev::clearSoftMask(GfxState *state)
{
    msg("<debug> clearSoftMask");
    boolpolydev->clearSoftMask(state);
    checkNewBitmap(UNKNOWN_BOUNDING_BOX);
    rgbdev->clearSoftMask(state);
    clip1dev->clearSoftMask(state);
    dbg_newdata("clearsoftmask");
}
