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
#include "../gfxdevice.h"

#define RENDER_FILL 0
#define RENDER_STROKE 1
#define RENDER_FILLSTROKE 2
#define RENDER_INVISIBLE 3
#define RENDER_CLIP 4

class CommonOutputDev: public OutputDev
{
    public:
    virtual void setDevice(gfxdevice_t*dev) = 0;
    virtual void setMove(int x,int y) = 0;
    virtual void setClip(int x1,int y1,int x2,int y2) = 0;
    virtual void setParameter(const char*key, const char*value) = 0;
    virtual void setPageMap(int*pagemap, int pagemap_len) = 0;
  
    virtual void setPage(Page *page) { this->page = page; }
    virtual void finishPage() {};

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
};
#endif //__deviceinterface_h__
