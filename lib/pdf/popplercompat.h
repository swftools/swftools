#ifndef __popplercompat_h__
#define __popplercompat_h__

#include "../../config.h"

#ifdef HAVE_POPPLER
  #define POPPLER_TILING_PATERN_RETURN GBool
  #define POPPLER_TILING_PATERN_GFX
  #define POPPLER_TILING_PATERN_GFX_ARG
#else
  #define POPPLER_TILING_PATERN_RETURN void
  #if (xpdfMajorVersion*10000 + xpdfMinorVersion*100 + xpdfUpdateVersion) < 30207
    #define POPPLER_TILING_PATERN_GFX
    #define POPPLER_TILING_PATERN_GFX_ARG
  #else
    #define POPPLER_TILING_PATERN_GFX Gfx *gfx,
    #define POPPLER_TILING_PATERN_GFX_ARG gfx,
  #endif
#endif

#ifdef HAVE_POPPLER
char* mktmpname(char*ptr);
#endif

#endif
