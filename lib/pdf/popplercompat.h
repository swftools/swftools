#ifndef __popplercompat_h__
#define __popplercompat_h__

#include "../../config.h"

#ifdef HAVE_POPPLER
  #define POPPLER_INTERPOLATE GBool interpolate,
  #define POPPLER_INTERPOLATE_ARG interpolate,
  #define POPPLER_MASK_INTERPOLATE , GBool maskInterpolate
  #define POPPLER_MASK_INTERPOLATE_ARG , maskInterpolate
#else
  #define POPPLER_INTERPOLATE
  #define POPPLER_INTERPOLATE_ARG
  #define POPPLER_MASK_INTERPOLATE
  #define POPPLER_MASK_INTERPOLATE_ARG
#endif

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

#ifdef HAVE_POPPLER
  #define POPPLER_RAXIAL_MIN_MAX , double min, double max
  #define POPPLER_RAXIAL_MIN_MAX_ARG , min, max
#else
  #define POPPLER_RAXIAL_MIN_MAX
  #define POPPLER_RAXIAL_MIN_MAX_ARG
#endif

#ifdef HAVE_POPPLER
  #include <goo/GooString.h>
  #include <goo/gfile.h>
#else
  #include "xpdf/config.h"
  #include "gfile.h"
  #include "GString.h"
#endif
#include "Object.h"
#include "Stream.h"
#include "Array.h"
#include "Dict.h"
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"
#include "PDFDoc.h"
#include "Error.h"
#include "Link.h"
#include "OutputDev.h"
#include "GfxFont.h"
#include "GfxState.h"
//#include "NameToUnicodeTable.h"
#include "GlobalParams.h"

#endif
