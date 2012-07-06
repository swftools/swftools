#ifndef __popplercompat_h__
#define __popplercompat_h__

#include "../../config.h"

char* mktmpname(char*ptr);

#ifdef HAVE_POPPLER
  #define int type int type
  #define type type
#else
  #define int type
  #define type
#endif

#ifdef HAVE_POPPLER
  #define Catalog *cat, Catalog *cat,
  #define cat, cat,
  #define double *pmat, double *pmat,
  #define pmat, pmat,
  #define int tilingType, int tilingType,
  #define tilingType, tilingType,
#else
  #define Catalog *cat,
  #define cat,
  #define double *pmat,
  #define pmat,
  #define int tilingType,
  #define tilingType,
#endif

#ifdef HAVE_POPPLER
  #define , x0, y0, &clip, &clipRes , x0, y0, &clip, &clipRes
#else
  #define , x0, y0, &clip, &clipRes
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
  #define gTrue gTrue
#else
  #define gTrue gFalse
#endif

#ifdef HAVE_POPPLER
  #define GFXGlobalParams GlobalParams
#endif

#include <goo/GooString.h>
#include <goo/gfile.h>
#include <Object.h>
#include <Stream.h>
#include <Array.h>
#include <Dict.h>
#include <XRef.h>
#include <Catalog.h>
#include <Page.h>
#include <PDFDoc.h>
#include <Error.h>
#include <Link.h>
#include <OutputDev.h>
#include <GfxFont.h>
#include <GfxState.h>
#include <GlobalParams.h>

#endif
