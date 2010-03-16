/* pdf2jpeg.c
   Converts a pdf page to a jpeg.

   Copyright (c) 2010 Matthias Kramm
   Copyright (c) 1998-2009 Derek Noonburg

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

#include <aconf.h>
#include <stdio.h>
#include "parseargs.h"
#include "gmem.h"
#include "GString.h"
#include "GlobalParams.h"
#include "Object.h"
#include "PDFDoc.h"
#include "SplashBitmap.h"
#include "Splash.h"
#include "SplashOutputDev.h"
#include "config.h"
#include "../jpeg.h"

static int page = 1;
static int width = 0;
static int resolution = 150;
static int quality = 95;
static char ownerPassword[33] = "";
static char userPassword[33] = "";
static char cfgFileName[256] = "";
static GBool printVersion = gFalse;
static GBool printHelp = gFalse;
static char output[256] = "output.jpg";

static ArgDesc argDesc[] = {
  {"-p",      argInt,      &page,     0,
   "first page to print"},
  {"-r",      argInt,      &resolution,    0,
   "resolution, in DPI (default is 150)"},
  {"-q",      argInt,      &quality,    0,
   "jpeg conversion quality"},
  {"-w",      argInt,      &width,    0,
   "zoom to this width"},
  {"-o",      argString,   &output,    sizeof(output),
   "resolution, in DPI (default is 150)"},
  {"-opw",    argString,   ownerPassword,  sizeof(ownerPassword),
   "owner password (for encrypted files)"},
  {"-upw",    argString,   userPassword,   sizeof(userPassword),
   "user password (for encrypted files)"},
  {"-cfg",        argString,      cfgFileName,    sizeof(cfgFileName),
   "configuration file to use in place of .xpdfrc"},
  {"-v",      argFlag,     &printVersion,  0,
   "print copyright and version info"},
  {"-h",      argFlag,     &printHelp,     0,
   "print usage information"},
  {"-help",   argFlag,     &printHelp,     0,
   "print usage information"},
  {"--help",  argFlag,     &printHelp,     0,
   "print usage information"},
  {"-?",      argFlag,     &printHelp,     0,
   "print usage information"},
  {NULL}
};

int main(int argc, char *argv[]) {
  PDFDoc *doc;
  GString *fileName;
  GString *ownerPW, *userPW;
  SplashColor paperColor;
  SplashOutputDev *splashOut;
  GBool ok;
  int exitCode;
  int pg;

  exitCode = 99;

  // parse args
  ok = parseArgs(argDesc, &argc, argv);
  
  if (!ok || argc != 2 || printVersion || printHelp) {
    fprintf(stderr, "pdf2jpeg version %s\n", xpdfVersion);
    fprintf(stderr, "%s\n", xpdfCopyright);
    if (!printVersion) {
      printUsage("pdf2jpeg", "<PDF-file> -o <jpegfile>", argDesc);
    }
    goto err0;
  }
  fileName = new GString(argv[1]);

  // read config file
  globalParams = new GlobalParams(cfgFileName);
  globalParams->setupBaseFonts(NULL);
  
  // open PDF file
  if (ownerPassword[0]) {
    ownerPW = new GString(ownerPassword);
  } else {
    ownerPW = NULL;
  }
  if (userPassword[0]) {
    userPW = new GString(userPassword);
  } else {
    userPW = NULL;
  }
  doc = new PDFDoc(fileName, ownerPW, userPW);
  if (userPW) {
    delete userPW;
  }
  if (ownerPW) {
    delete ownerPW;
  }
  if (!doc->isOk()) {
    exitCode = 1;
    goto err1;
  }

  paperColor[0] = paperColor[1] = paperColor[2] = 0xff;
  splashOut = new SplashOutputDev(splashModeRGB8, 1, gFalse, paperColor);
  
  splashOut->startDoc(doc->getXRef());

  if(page>=1 && page<=doc->getNumPages()) {
      double r = resolution;
      if(width) {
	int old_width = doc->getPageCropWidth(page);
	r = 72.0*width/old_width;
      }
      doc->displayPage(splashOut, page, r, r, 0, gFalse, gTrue, gFalse);
      SplashBitmap*bitmap = splashOut->getBitmap();
      if(bitmap) {
	Guchar*rgb = bitmap->getDataPtr();
	int width = bitmap->getWidth();
	int height = bitmap->getHeight();
	jpeg_save(rgb, width, height, quality, output);
      }
  }
  delete splashOut;

  exitCode = 0;

  // clean up
 err1:
  delete doc;
  delete globalParams;
 err0:

  // check for memory leaks
  Object::memCheck(stderr);
  gMemReport(stderr);

  return exitCode;
}
