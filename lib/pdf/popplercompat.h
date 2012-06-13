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
#else
  #define POPPLER_TILING_PATERN_RETURN void
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
#define AnnotLink Link
#endif

#ifdef HAVE_POPPLER
  #define POPPLER_STARTDOC_ARG this->doc
  #define INFO_OUTPUT_DEV_STARTDOC_ARG doc
#else
  #define POPPLER_STARTDOC_ARG this->xref
  #define INFO_OUTPUT_DEV_STARTDOC_ARG doc->getXRef()
#endif

#ifdef HAVE_POPPLER
  #define POPPLER_SHADED_FILL_TYPE int type
  #define POPPLER_SHADED_FILL_TYPE_ARG type
#else
  #define POPPLER_SHADED_FILL_TYPE
  #define POPPLER_SHADED_FILL_TYPE_ARG
#endif

#ifdef HAVE_POPPLER
  #define POPPLER_TILING_PATTERN_CATALOG Catalog *cat,
  #define POPPLER_TILING_PATTERN_CATALOG_ARG cat,
  #define POPPLER_TILING_PATTERN_PMAT double *pmat,
  #define POPPLER_TILING_PATTERN_PMAT_ARG pmat,
  #define POPPLER_TILING_PATTERN_TILING_TYPE int tilingType,
  #define POPPLER_TILING_PATTERN_TILING_TYPE_ARG tilingType,
#else
  #define POPPLER_TILING_PATTERN_CATALOG
  #define POPPLER_TILING_PATTERN_CATALOG_ARG
  #define POPPLER_TILING_PATTERN_PMAT
  #define POPPLER_TILING_PATTERN_PMAT_ARG
  #define POPPLER_TILING_PATTERN_TILING_TYPE
  #define POPPLER_TILING_PATTERN_TILING_TYPE_ARG
#endif

#ifdef HAVE_POPPLER
  #define POPPLER_GET_GLYPH_ARGS , x0, y0, &clip, &clipRes
#else
  #define POPPLER_GET_GLYPH_ARGS
#endif

#ifndef HAVE_POPPLER
  #define GFXSplashOutputDev SplashOutputDev
#endif

#ifdef HAVE_POPPLER
  #define POPPLER_CHECK_PAGE_SLICE_ANNOT_CBK_H , GBool (*annotDisplayDecideCbk)(Annot *annot, void *user_data) = NULL
  #define POPPLER_CHECK_PAGE_SLICE_ANNOT_CBK , GBool (*annotDisplayDecideCbk)(Annot *annot, void *user_data)
  #define POPPLER_CHECK_PAGE_SLICE_ANNOT_CBK_DATA_H , void *annotDisplayDecideCbkData = NULL
  #define POPPLER_CHECK_PAGE_SLICE_ANNOT_CBK_DATA , void *annotDisplayDecideCbkData
#else
  #define POPPLER_CHECK_PAGE_SLICE_ANNOT_CBK_H
  #define POPPLER_CHECK_PAGE_SLICE_ANNOT_CBK
  #define POPPLER_CHECK_PAGE_SLICE_ANNOT_CBK_DATA_H
  #define POPPLER_CHECK_PAGE_SLICE_ANNOT_CBK_DATA
#endif

#ifdef HAVE_POPPLER
  #define GFXGlobalParams GlobalParams
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
