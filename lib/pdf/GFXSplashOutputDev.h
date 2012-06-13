/* GFXSplashOutputDev.h
   Output device derived from SplashOutputDev.

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

#ifndef __GFXSplashOutputDev_h__
#define __GFXSplashOutputDev_h__

#include "SplashOutputDev.h"

#ifdef HAVE_POPPLER

#include "../../config.h"
#include <splash/SplashTypes.h>
#include <splash/SplashPath.h>
#include <splash/SplashFont.h>
#include <splash/SplashFontFile.h>

class GFXSplashOutputDev: public SplashOutputDev {
public:
    GFXSplashOutputDev(SplashColorMode colorModeA, int bitmapRowPadA,
                       GBool reverseVideoA, SplashColorPtr paperColorA,
                       GBool bitmapTopDownA = gTrue,
                       GBool allowAntialiasA = gTrue)
        : SplashOutputDev(colorModeA, bitmapRowPadA, reverseVideoA, paperColorA, bitmapTopDownA, allowAntialiasA)
    {
        needFontUpdate = gFalse;
    }

    // OutputDev:
    virtual void restoreState(GfxState *state) { SplashOutputDev::restoreState(state); needFontUpdate = gTrue; }

    virtual void updateAll(GfxState *state) { SplashOutputDev::updateAll(state); needFontUpdate = gTrue; }
    virtual void updateFont(GfxState *state) { SplashOutputDev::updateFont(state); needFontUpdate = gTrue; }

    // SplashOutputDev:
    void doUpdateFont(GfxState *state) { SplashOutputDev::doUpdateFont(state); needFontUpdate = gFalse; }

    GBool needFontUpdate;         // set when the font needs to be updated
};

#endif // HAVE_POPPLER

#endif
