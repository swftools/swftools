/* InfoOutputDev.h
   Superclass for BitmapOutputDev, GFXOutputDev etc.

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

#ifndef __commonoutputdev_h__
#define __commonoutputdev_h__

#include "OutputDev.h"
#include "InfoOutputDev.h"
#include "../gfxdevice.h"

#define RENDER_FILL 0
#define RENDER_STROKE 1
#define RENDER_FILLSTROKE 2
#define RENDER_INVISIBLE 3
#define RENDER_CLIP 4

typedef struct _feature
{
    char*string;
    struct _feature*next;
} feature_t;

class GFXOutputGlobals {
public:
  feature_t*featurewarnings;

  int textmodeinfo; // did we write "Text will be rendered as polygon" yet?
  int jpeginfo; // did we write "File contains jpegs" yet?
  int pbminfo; // did we write "File contains jpegs" yet?
  int linkinfo; // did we write "File contains links" yet?

  GFXOutputGlobals();
  ~GFXOutputGlobals();
};
extern GFXOutputGlobals* getGfxGlobals();

extern void warnfeature(const char*feature,char fully);
extern void infofeature(const char*feature);

gfxcolor_t gfxstate_getfillcolor(GfxState * state);
bool text_matrix_is_skewed(GfxState * state);

class CommonOutputDev: public OutputDev
{
    public:
    CommonOutputDev(InfoOutputDev*info, PDFDoc*doc, int*page2page, int num_pages, int x, int y, int x1, int y1, int x2, int y2);

    virtual void setDevice(gfxdevice_t*dev) = 0;
    virtual void setParameter(const char*key, const char*value) = 0;

    virtual void startPage(int pageNum, GfxState*state);
    virtual void beginPage(GfxState*state, int page) = 0;
  
    virtual void setPage(Page *page) { this->page = page; }
    virtual void finishPage() {};

    void transformXY(GfxState*state, double x, double y, double*nx, double*ny);
    void transformXY_stateless(double x, double y, int*xout, int*yout);

    virtual GBool interpretType3Chars();

    virtual GBool checkPageSlice(Page *page, double hDPI, double vDPI,
			       int rotate, GBool useMediaBox, GBool crop,
			       int sliceX, int sliceY, int sliceW, int sliceH,
			       GBool printing, Catalog *catalog,
			       GBool (*abortCheckCbk)(void *data) = NULL,
			       void *abortCheckCbkData = NULL)
    {
        this->setPage(page);
        return gTrue;
    }

    protected:
    Page *page;
    PDFDoc* doc;
    XRef* xref;
    InfoOutputDev* info;

    /* if set, will use a user bounding box instead of the PDF's bounding box */
    int user_movex,user_movey;
    int user_clipx1,user_clipx2,user_clipy1,user_clipy2;
    
    /* movex,movey is the upper left corner of clipping rectangle (cropbox)- 
       this needs to be added to all drawing coordinates to give the 
       impression that all pages start at (0,0)*/
    int movex, movey;
    int width, height;
  
    /* for page mapping */
    int* page2page;
    int num_pages;
};
#endif //__deviceinterface_h__
