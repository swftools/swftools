/* pdfswf.cc
   implements a pdf output device (OutputDev).

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

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include "../config.h"
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_FONTCONFIG_H
#include <fontconfig.h>
#endif
//xpdf header files
#include "config.h"
#include "gfile.h"
#include "GString.h"
#include "gmem.h"
#include "Object.h"
#include "Stream.h"
#include "Array.h"
#include "Dict.h"
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"
#include "PDFDoc.h"
#include "Error.h"
#include "OutputDev.h"
#include "GfxState.h"
#include "GfxFont.h"
#include "CharCodeToUnicode.h"
#include "NameToUnicodeTable.h"
#include "GlobalParams.h"
//#define XPDF_101
#ifdef XPDF_101
#include "FontFile.h"
#else
#include "FoFiType1C.h"
#include "FoFiTrueType.h"
#endif
#include "SWFOutputDev.h"

//swftools header files
#include "swfoutput.h"
#include "../lib/log.h"

#include <math.h>

typedef struct _fontfile
{
    char*filename;
    int used;
} fontfile_t;

// for pdfswf_addfont
static fontfile_t fonts[2048];
static int fontnum = 0;

// swf <-> pdf pages
// TODO: move into pdf_doc_t
static int*pages = 0;
static int pagebuflen = 0;
static int pagepos = 0;

/* config */
static double caplinewidth = 3.0;
static int zoom = 72; /* xpdf: 86 */

static void printInfoString(Dict *infoDict, char *key, char *fmt);
static void printInfoDate(Dict *infoDict, char *key, char *fmt);

struct mapping {
    char*pdffont;
    char*filename;
} pdf2t1map[] ={
{"Times-Roman",           "n021003l"},
{"Times-Italic",          "n021023l"},
{"Times-Bold",            "n021004l"},
{"Times-BoldItalic",      "n021024l"},
{"Helvetica",             "n019003l"},
{"Helvetica-Oblique",     "n019023l"},
{"Helvetica-Bold",        "n019004l"},
{"Helvetica-BoldOblique", "n019024l"},
{"Courier",               "n022003l"},
{"Courier-Oblique",       "n022023l"},
{"Courier-Bold",          "n022004l"},
{"Courier-BoldOblique",   "n022024l"},
{"Symbol",                "s050000l"},
{"ZapfDingbats",          "d050000l"}};

class SWFOutputDev:  public OutputDev {
  int outputstarted;
  struct swfoutput output;
public:

  // Constructor.
  SWFOutputDev();

  // Destructor.
  virtual ~SWFOutputDev() ;

  void setMove(int x,int y);
  void setClip(int x1,int y1,int x2,int y2);
  
  int save(char*filename);

  void getDimensions(int*x1,int*y1,int*x2,int*y2);

  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown();

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar();
  
  // Can this device draw gradients?
  virtual GBool useGradients();
  
  virtual GBool interpretType3Chars() {return gTrue;}

  //----- initialization and control

  void setXRef(PDFDoc*doc, XRef *xref);

  // Start a page.
  virtual void startPage(int pageNum, GfxState *state, double x1, double y1, double x2, double y2) ;

  //----- link borders
  virtual void drawLink(Link *link, Catalog *catalog) ;

  //----- save/restore graphics state
  virtual void saveState(GfxState *state) ;
  virtual void restoreState(GfxState *state) ;

  //----- update graphics state

  virtual void updateFont(GfxState *state);
  virtual void updateFillColor(GfxState *state);
  virtual void updateStrokeColor(GfxState *state);
  virtual void updateLineWidth(GfxState *state);
  virtual void updateLineJoin(GfxState *state);
  virtual void updateLineCap(GfxState *state);
  
  virtual void updateAll(GfxState *state) 
  {
      updateFont(state);
      updateFillColor(state);
      updateStrokeColor(state);
      updateLineWidth(state);
      updateLineJoin(state);
      updateLineCap(state);
  };

  //----- path painting
  virtual void stroke(GfxState *state) ;
  virtual void fill(GfxState *state) ;
  virtual void eoFill(GfxState *state) ;

  //----- path clipping
  virtual void clip(GfxState *state) ;
  virtual void eoClip(GfxState *state) ;

  //----- text drawing
  virtual void beginString(GfxState *state, GString *s) ;
  virtual void endString(GfxState *state) ;
  virtual void drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode code, Unicode *u, int uLen);

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);
  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg);
  
  virtual GBool beginType3Char(GfxState *state,
			       CharCode code, Unicode *u, int uLen);
  virtual void endType3Char(GfxState *state);

  private:
  void drawGeneralImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GfxImageColorMap*colorMap, GBool invert,
				   GBool inlineImg, int mask);
  int clipping[64];
  int clippos;

  int currentpage;

  PDFDoc*doc;
  XRef*xref;

  char* searchFont(char*name);
  char* substituteFont(GfxFont*gfxFont, char*oldname);
  char* writeEmbeddedFontToFile(XRef*ref, GfxFont*font);
  int t1id;
  int jpeginfo; // did we write "File contains jpegs" yet?
  int pbminfo; // did we write "File contains jpegs" yet?
  int linkinfo; // did we write "File contains links" yet?
  int ttfinfo; // did we write "File contains TrueType Fonts" yet?
  int gradientinfo; // did we write "File contains Gradients yet?

  int type3active; // are we between beginType3()/endType3()?

  GfxState *laststate;

  int pic_xids[1024];
  int pic_yids[1024];
  int pic_ids[1024];
  int pic_width[1024];
  int pic_height[1024];
  int picpos;
  int pic_id;
  char type3Warning;

  char* substitutetarget[256];
  char* substitutesource[256];
  int substitutepos;

  int user_movex,user_movey;
  int user_clipx1,user_clipx2,user_clipy1,user_clipy2;
};

static char*getFontID(GfxFont*font);

class InfoOutputDev:  public OutputDev 
{
  public:
  int x1,y1,x2,y2;
  int num_links;
  int num_images;
  int num_fonts;

  InfoOutputDev() 
  {
      num_links = 0;
      num_images = 0;
      num_fonts = 0;
  }
  virtual ~InfoOutputDev() 
  {
  }
  virtual GBool upsideDown() {return gTrue;}
  virtual GBool useDrawChar() {return gTrue;}
  virtual GBool useGradients() {return gTrue;}
  virtual GBool interpretType3Chars() {return gTrue;}
  virtual void startPage(int pageNum, GfxState *state, double crop_x1, double crop_y1, double crop_x2, double crop_y2)
  {
      double x1,y1,x2,y2;
      state->transform(crop_x1,crop_y1,&x1,&y1);
      state->transform(crop_x2,crop_y2,&x2,&y2);
      if(x2<x1) {double x3=x1;x1=x2;x2=x3;}
      if(y2<y1) {double y3=y1;y1=y2;y2=y3;}
      this->x1 = (int)x1;
      this->y1 = (int)y1;
      this->x2 = (int)x2;
      this->y2 = (int)y2;
  }
  virtual void drawLink(Link *link, Catalog *catalog) 
  {
      num_links++;
  }
  virtual void updateFont(GfxState *state) 
  {
      GfxFont*font = state->getFont();
      if(!font)
          return;
      char*id = getFontID(font);
      /* FIXME*/
      num_fonts++;
  }
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg) 
  {
      num_images++;
  }
  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg)
  {
      num_images++;
  }
};

SWFOutputDev::SWFOutputDev()
{
    jpeginfo = 0;
    ttfinfo = 0;
    linkinfo = 0;
    pbminfo = 0;
    type3active = 0;
    clippos = 0;
    clipping[clippos] = 0;
    outputstarted = 0;
    xref = 0;
    picpos = 0;
    pic_id = 0;
    substitutepos = 0;
    type3Warning = 0;
    user_movex = 0;
    user_movey = 0;
    user_clipx1 = 0;
    user_clipy1 = 0;
    user_clipx2 = 0;
    user_clipy2 = 0;
    memset(&output, 0, sizeof(output));
//    printf("SWFOutputDev::SWFOutputDev() \n");
};
  
void SWFOutputDev::setMove(int x,int y)
{
    this->user_movex = x;
    this->user_movey = y;
}

void SWFOutputDev::setClip(int x1,int y1,int x2,int y2)
{
    if(x2<x1) {int x3=x1;x1=x2;x2=x3;}
    if(y2<y1) {int y3=y1;y1=y2;y2=y3;}

    this->user_clipx1 = x1;
    this->user_clipy1 = y1;
    this->user_clipx2 = x2;
    this->user_clipy2 = y2;
}
void SWFOutputDev::getDimensions(int*x1,int*y1,int*x2,int*y2)
{
    if(x1) *x1 = output.swf.movieSize.xmin/20;
    if(y1) *y1 = output.swf.movieSize.ymin/20;
    if(x2) *x2 = output.swf.movieSize.xmax/20;
    if(y2) *y2 = output.swf.movieSize.ymax/20;
}

static char*getFontID(GfxFont*font)
{
    GString*gstr = font->getName();
    char* fontname = gstr==0?0:gstr->getCString();
    if(fontname==0) {
	char buf[32];
	Ref*r=font->getID();
	sprintf(buf, "UFONT%d", r->num);
	return strdup(buf);
    }
    return strdup(fontname);
}

static char*getFontName(GfxFont*font)
{
    char*fontid = getFontID(font);
    char*fontname= 0;
    char* plus = strchr(fontid, '+');
    if(plus && plus < &fontid[strlen(fontid)-1]) {
	fontname = strdup(plus+1);
    } else {
        fontname = strdup(fontid);
    }
    free(fontid);
    return fontname;
}

static char mybuf[1024];
static char* gfxstate2str(GfxState *state)
{
  char*bufpos = mybuf;
  GfxRGB rgb;
  bufpos+=sprintf(bufpos,"CTM[%.3f/%.3f/%.3f/%.3f/%.3f/%.3f] ",
				    state->getCTM()[0],
				    state->getCTM()[1],
				    state->getCTM()[2],
				    state->getCTM()[3],
				    state->getCTM()[4],
				    state->getCTM()[5]);
  if(state->getX1()!=0.0)
  bufpos+=sprintf(bufpos,"X1-%.1f ",state->getX1());
  if(state->getY1()!=0.0)
  bufpos+=sprintf(bufpos,"Y1-%.1f ",state->getY1());
  bufpos+=sprintf(bufpos,"X2-%.1f ",state->getX2());
  bufpos+=sprintf(bufpos,"Y2-%.1f ",state->getY2());
  bufpos+=sprintf(bufpos,"PW%.1f ",state->getPageWidth());
  bufpos+=sprintf(bufpos,"PH%.1f ",state->getPageHeight());
  /*bufpos+=sprintf(bufpos,"FC[%.1f/%.1f] ",
	  state->getFillColor()->c[0], state->getFillColor()->c[1]);
  bufpos+=sprintf(bufpos,"SC[%.1f/%.1f] ",
	  state->getStrokeColor()->c[0], state->getFillColor()->c[1]);*/
/*  bufpos+=sprintf(bufpos,"FC[%.1f/%.1f/%.1f/%.1f/%.1f/%.1f/%.1f/%.1f]",
	  state->getFillColor()->c[0], state->getFillColor()->c[1],
	  state->getFillColor()->c[2], state->getFillColor()->c[3],
	  state->getFillColor()->c[4], state->getFillColor()->c[5],
	  state->getFillColor()->c[6], state->getFillColor()->c[7]);
  bufpos+=sprintf(bufpos,"SC[%.1f/%.1f/%.1f/%.1f/%.1f/%.1f/%.1f/%.1f]",
	  state->getStrokeColor()->c[0], state->getFillColor()->c[1],
	  state->getStrokeColor()->c[2], state->getFillColor()->c[3],
	  state->getStrokeColor()->c[4], state->getFillColor()->c[5],
	  state->getStrokeColor()->c[6], state->getFillColor()->c[7]);*/
  state->getFillRGB(&rgb);
  if(rgb.r || rgb.g || rgb.b)
  bufpos+=sprintf(bufpos,"FR[%.1f/%.1f/%.1f] ", rgb.r,rgb.g,rgb.b);
  state->getStrokeRGB(&rgb);
  if(rgb.r || rgb.g || rgb.b)
  bufpos+=sprintf(bufpos,"SR[%.1f/%.1f/%.1f] ", rgb.r,rgb.g,rgb.b);
  if(state->getFillColorSpace()->getNComps()>1)
  bufpos+=sprintf(bufpos,"CS[[%d]] ",state->getFillColorSpace()->getNComps());
  if(state->getStrokeColorSpace()->getNComps()>1)
  bufpos+=sprintf(bufpos,"SS[[%d]] ",state->getStrokeColorSpace()->getNComps());
  if(state->getFillPattern())
  bufpos+=sprintf(bufpos,"FP%08x ", state->getFillPattern());
  if(state->getStrokePattern())
  bufpos+=sprintf(bufpos,"SP%08x ", state->getStrokePattern());
 
  if(state->getFillOpacity()!=1.0)
  bufpos+=sprintf(bufpos,"FO%.1f ", state->getFillOpacity());
  if(state->getStrokeOpacity()!=1.0)
  bufpos+=sprintf(bufpos,"SO%.1f ", state->getStrokeOpacity());

  bufpos+=sprintf(bufpos,"LW%.1f ", state->getLineWidth());
 
  double * dash;
  int length;
  double start;
  state->getLineDash(&dash, &length, &start);
  int t;
  if(length)
  {
      bufpos+=sprintf(bufpos,"DASH%.1f[",start);
      for(t=0;t<length;t++) {
	  bufpos+=sprintf(bufpos,"D%.1f",dash[t]);
      }
      bufpos+=sprintf(bufpos,"]");
  }

  if(state->getFlatness()!=1)
  bufpos+=sprintf(bufpos,"F%d ", state->getFlatness());
  if(state->getLineJoin()!=0)
  bufpos+=sprintf(bufpos,"J%d ", state->getLineJoin());
  if(state->getLineJoin()!=0)
  bufpos+=sprintf(bufpos,"C%d ", state->getLineCap());
  if(state->getLineJoin()!=0)
  bufpos+=sprintf(bufpos,"ML%d ", state->getMiterLimit());

  if(state->getFont() && getFontID(state->getFont()))
  bufpos+=sprintf(bufpos,"F\"%s\" ",getFontID(state->getFont()));
  bufpos+=sprintf(bufpos,"FS%.1f ", state->getFontSize());
  bufpos+=sprintf(bufpos,"MAT[%.1f/%.1f/%.1f/%.1f/%.1f/%.1f] ", state->getTextMat()[0],state->getTextMat()[1],state->getTextMat()[2],
	                           state->getTextMat()[3],state->getTextMat()[4],state->getTextMat()[5]);
  if(state->getCharSpace())
  bufpos+=sprintf(bufpos,"CS%.5f ", state->getCharSpace());
  if(state->getWordSpace())
  bufpos+=sprintf(bufpos,"WS%.5f ", state->getWordSpace());
  if(state->getHorizScaling()!=1.0)
  bufpos+=sprintf(bufpos,"SC%.1f ", state->getHorizScaling());
  if(state->getLeading())
  bufpos+=sprintf(bufpos,"L%.1f ", state->getLeading());
  if(state->getRise())
  bufpos+=sprintf(bufpos,"R%.1f ", state->getRise());
  if(state->getRender())
  bufpos+=sprintf(bufpos,"R%d ", state->getRender());
  bufpos+=sprintf(bufpos,"P%08x ", state->getPath());
  bufpos+=sprintf(bufpos,"CX%.1f ", state->getCurX());
  bufpos+=sprintf(bufpos,"CY%.1f ", state->getCurY());
  if(state->getLineX())
  bufpos+=sprintf(bufpos,"LX%.1f ", state->getLineX());
  if(state->getLineY())
  bufpos+=sprintf(bufpos,"LY%.1f ", state->getLineY());
  bufpos+=sprintf(bufpos," ");
  return mybuf;
}

static void dumpFontInfo(char*loglevel, GfxFont*font);
static int lastdumps[1024];
static int lastdumppos = 0;
/* nr = 0  unknown
   nr = 1  substituting
   nr = 2  type 3
 */
static void showFontError(GfxFont*font, int nr) 
{  
    Ref*r=font->getID();
    int t;
    for(t=0;t<lastdumppos;t++)
	if(lastdumps[t] == r->num)
	    break;
    if(t < lastdumppos)
      return;
    if(lastdumppos<sizeof(lastdumps)/sizeof(int))
    lastdumps[lastdumppos++] = r->num;
    if(nr == 0)
      msg("<warning> The following font caused problems:");
    else if(nr == 1)
      msg("<warning> The following font caused problems (substituting):");
    else if(nr == 2)
      msg("<warning> The following Type 3 Font will be rendered as bitmap:");
    dumpFontInfo("<warning>", font);
}

static void dumpFontInfo(char*loglevel, GfxFont*font)
{
  char* name = getFontID(font);
  Ref* r=font->getID();
  msg("%s=========== %s (ID:%d,%d) ==========\n", loglevel, getFontName(font), r->num,r->gen);

  GString*gstr  = font->getTag();
   
  msg("%s| Tag: %s\n", loglevel, name);
  
  if(font->isCIDFont()) msg("%s| is CID font\n", loglevel);

  GfxFontType type=font->getType();
  switch(type) {
    case fontUnknownType:
     msg("%s| Type: unknown\n",loglevel);
    break;
    case fontType1:
     msg("%s| Type: 1\n",loglevel);
    break;
    case fontType1C:
     msg("%s| Type: 1C\n",loglevel);
    break;
    case fontType3:
     msg("%s| Type: 3\n",loglevel);
    break;
    case fontTrueType:
     msg("%s| Type: TrueType\n",loglevel);
    break;
    case fontCIDType0:
     msg("%s| Type: CIDType0\n",loglevel);
    break;
    case fontCIDType0C:
     msg("%s| Type: CIDType0C\n",loglevel);
    break;
    case fontCIDType2:
     msg("%s| Type: CIDType2\n",loglevel);
    break;
  }
  
  Ref embRef;
  GBool embedded = font->getEmbeddedFontID(&embRef);
  if(font->getEmbeddedFontName())
    name = font->getEmbeddedFontName()->getCString();
  if(embedded)
   msg("%s| Embedded name: %s id: %d\n",loglevel, FIXNULL(name), embRef.num);

  gstr = font->getExtFontFile();
  if(gstr)
   msg("%s| External Font file: %s\n", loglevel, FIXNULL(gstr->getCString()));

  // Get font descriptor flags.
  if(font->isFixedWidth()) msg("%s| is fixed width\n", loglevel);
  if(font->isSerif()) msg("%s| is serif\n", loglevel);
  if(font->isSymbolic()) msg("%s| is symbolic\n", loglevel);
  if(font->isItalic()) msg("%s| is italic\n", loglevel);
  if(font->isBold()) msg("%s| is bold\n", loglevel);
}

//void SWFOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) {printf("void SWFOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) \n");}
//void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) {printf("void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) \n");}

static void free_outline(SWF_OUTLINE*outline)
{
    while(outline) {
        SWF_OUTLINE*next = outline->link;
        free(outline);
        outline = next;
    }
}

static void dump_outline(SWF_OUTLINE*outline)
{
    double x=0,y=0;
    while(outline) {
        double lastx=x,lasty=y;
        x += (outline->dest.x/(float)0xffff);
        y += (outline->dest.y/(float)0xffff);
        if(outline->type == SWF_PATHTYPE_MOVE) {
            msg("<trace> | moveto %f,%f", x,y);
        } else if(outline->type == SWF_PATHTYPE_LINE) {
            msg("<trace> | lineto: %f,%f\n",x,y);
        } else if(outline->type == SWF_PATHTYPE_BEZIER) {
            SWF_BEZIERSEGMENT*o2 = (SWF_BEZIERSEGMENT*)outline;
            float x1 = o2->C.x/(float)0xffff+lastx;
            float y1 = o2->C.y/(float)0xffff+lasty;
            float x2 = o2->B.x/(float)0xffff+lastx;
            float y2 = o2->B.y/(float)0xffff+lasty;
            msg("<trace> | spline: %f,%f -> %f,%f -> %f,%f\n",x1,y1,x2,y2,x,y);
        } 
        outline = outline->link;
    }
}

SWF_OUTLINE* gfxPath_to_SWF_OUTLINE(GfxState*state, GfxPath*path)
{
    int num = path->getNumSubpaths();
    int s,t;
    bezierpathsegment*start,*last=0;
    bezierpathsegment*outline = start = (bezierpathsegment*)malloc(sizeof(bezierpathsegment));
    int cpos = 0;
    double lastx=0,lasty=0;
    if(!num) {
	msg("<warning> empty path");
	outline->type = SWF_PATHTYPE_MOVE;
	outline->dest.x = 0;
	outline->dest.y = 0;
	outline->link = 0;
	return (SWF_OUTLINE*)outline;
    }
    for(t = 0; t < num; t++) {
	GfxSubpath *subpath = path->getSubpath(t);
	int subnum = subpath->getNumPoints();

	for(s=0;s<subnum;s++) {
	   double nx,ny;
	   state->transform(subpath->getX(s),subpath->getY(s),&nx,&ny);
	   int x = (int)((nx-lastx)*0xffff);
	   int y = (int)((ny-lasty)*0xffff);
	   if(s==0) 
	   {
		last = outline;
		outline->type = SWF_PATHTYPE_MOVE;
		outline->dest.x = x;
		outline->dest.y = y;
		outline->link = (SWF_OUTLINE*)malloc(sizeof(bezierpathsegment));
		outline = (bezierpathsegment*)outline->link;
		cpos = 0;
		lastx = nx;
		lasty = ny;
	   }
	   else if(subpath->getCurve(s) && !cpos)
	   {
		outline->B.x = x;
		outline->B.y = y;
		cpos = 1;
	   } 
	   else if(subpath->getCurve(s) && cpos)
	   {
		outline->C.x = x;
		outline->C.y = y;
		cpos = 2;
	   }
	   else
	   {
		last = outline;
		outline->dest.x = x;
		outline->dest.y = y;
		outline->type = cpos?SWF_PATHTYPE_BEZIER:SWF_PATHTYPE_LINE;
		outline->link = (SWF_OUTLINE*)malloc(sizeof(bezierpathsegment));
		outline = (bezierpathsegment*)outline->link;
		cpos = 0;
		lastx = nx;
		lasty = ny;
	   }
	}
    }
    if(last->link) {free(last->link);}
    last->link = 0;

    return (SWF_OUTLINE*)start;
}
/*----------------------------------------------------------------------------
 * Primitive Graphic routines
 *----------------------------------------------------------------------------*/

void SWFOutputDev::stroke(GfxState *state) 
{
    GfxPath * path = state->getPath();
    int lineCap = state->getLineCap(); // 0=butt, 1=round 2=square
    int lineJoin = state->getLineJoin(); // 0=miter, 1=round 2=bevel
    double miterLimit = state->getMiterLimit();
    double width = state->getTransformedLineWidth();
    struct swfmatrix m;
    GfxRGB rgb;
    double opaq = state->getStrokeOpacity();
    state->getStrokeRGB(&rgb);

    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m12 = 0; m.m13 = 0; m.m23 = 0;
    SWF_OUTLINE*outline = gfxPath_to_SWF_OUTLINE(state, path);
    
    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> stroke\n");
        dump_outline(outline);
    }

    lineJoin = 1; // other line joins are not yet supported by the swf encoder
                  // TODO: support bevel joints

    if(((lineCap==1) && (lineJoin==1)) || width<=caplinewidth) {
	/* FIXME- if the path is smaller than 2 segments, we could ignore
	   lineJoin */
	swfoutput_setdrawmode(&output, DRAWMODE_STROKE);
	swfoutput_drawpath(&output, outline, &m);
    } else {
	swfoutput_setfillcolor(&output, (char)(rgb.r*255), (char)(rgb.g*255), 
					(char)(rgb.b*255), (char)(opaq*255));

	//swfoutput_setlinewidth(&output, 1.0); //only for debugging
	//swfoutput_setstrokecolor(&output, 0, 255, 0, 255); //likewise, see below
	//swfoutput_setfillcolor(&output, 255, 0, 0, 255); //likewise, see below

	swfoutput_drawpath2poly(&output, outline, &m, lineJoin, lineCap, width, miterLimit);
	updateLineWidth(state);  //reset
	updateStrokeColor(state); //reset
	updateFillColor(state);  //reset
    }
    free_outline(outline);
}
void SWFOutputDev::fill(GfxState *state) 
{
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m12 = 0; m.m13 = 0; m.m23 = 0;
    SWF_OUTLINE*outline = gfxPath_to_SWF_OUTLINE(state, path);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> fill\n");
        dump_outline(outline);
    }

    swfoutput_setdrawmode(&output, DRAWMODE_FILL);
    swfoutput_drawpath(&output, outline, &m);
    free_outline(outline);
}
void SWFOutputDev::eoFill(GfxState *state) 
{
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m12 = 0; m.m13 = 0; m.m23 = 0;
    SWF_OUTLINE*outline = gfxPath_to_SWF_OUTLINE(state, path);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> eofill\n");
        dump_outline(outline);
    }

    swfoutput_setdrawmode(&output, DRAWMODE_EOFILL);
    swfoutput_drawpath(&output, outline, &m);
    free_outline(outline);
}
void SWFOutputDev::clip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m22 = 1;
    m.m12 = 0; m.m21 = 0; 
    m.m13 = 0; m.m23 = 0;
    SWF_OUTLINE*outline = gfxPath_to_SWF_OUTLINE(state, path);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> clip\n");
        dump_outline(outline);
    }

    swfoutput_startclip(&output, outline, &m);
    clipping[clippos] ++;
    free_outline(outline);
}
void SWFOutputDev::eoClip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m12 = 0; m.m13 = 0; m.m23 = 0;
    SWF_OUTLINE*outline = gfxPath_to_SWF_OUTLINE(state, path);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> eoclip\n");
        dump_outline(outline);
    }

    swfoutput_startclip(&output, outline, &m);
    clipping[clippos] ++;
    free_outline(outline);
}
int SWFOutputDev::save(char*filename)
{
    return swfoutput_save(&output, filename);
}

SWFOutputDev::~SWFOutputDev() 
{
    swfoutput_destroy(&output);
    outputstarted = 0;
};
GBool SWFOutputDev::upsideDown() 
{
    msg("<debug> upsidedown? yes");
    return gTrue;
};
GBool SWFOutputDev::useDrawChar() 
{
    return gTrue;
}
GBool SWFOutputDev::useGradients()
{
    if(!gradientinfo)
    {
	msg("<notice> File contains gradients");
	gradientinfo = 1;
    }
    return gTrue;
}

void SWFOutputDev::beginString(GfxState *state, GString *s) 
{ 
    double m11,m21,m12,m22;
//    msg("<debug> %s beginstring \"%s\"\n", gfxstate2str(state), s->getCString());
    state->getFontTransMat(&m11, &m12, &m21, &m22);
    m11 *= state->getHorizScaling();
    m21 *= state->getHorizScaling();
    swfoutput_setfontmatrix(&output, m11, -m21, m12, -m22);
}

void SWFOutputDev::drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode c, Unicode *_u, int uLen)
{
    // check for invisible text -- this is used by Acrobat Capture
    if ((state->getRender() & 3) == 3)
	return;

    GfxFont*font = state->getFont();

    if(font->getType() == fontType3) {
	/* type 3 chars are passed as graphics */
	return;
    }
    double x1,y1;
    x1 = x;
    y1 = y;
    state->transform(x, y, &x1, &y1);
    
    Unicode u=0;
    if(_u && uLen) 
	u = *_u;

    /* find out the character name */
    char*name=0;
    if(font->isCIDFont() && u) {
	GfxCIDFont*cfont = (GfxCIDFont*)font;
	int t;
	for(t=0;t<sizeof(nameToUnicodeTab)/sizeof(nameToUnicodeTab[0]);t++) {
	    /* todo: should be precomputed */
	    if(nameToUnicodeTab[t].u == u) {
		name = nameToUnicodeTab[t].name;
		break;
	    }
	}
    } else {
	Gfx8BitFont*font8;
	font8 = (Gfx8BitFont*)font;
	char**enc=font8->getEncoding();
	if(enc && enc[c])
	   name = enc[c];
    }
    
    msg("<debug> drawChar(%f,%f,c='%c' (%d),u=%d <%d>) CID=%d name=\"%s\"\n",x,y,(c&127)>=32?c:'?',c,u, uLen, font->isCIDFont(), FIXNULL(name));

    /*x1 = (int)(x1+0.5);
    y1 = (int)(y1+0.5);*/
    
    int ret = swfoutput_drawchar(&output, x1, y1, name, c, u);
}

void SWFOutputDev::endString(GfxState *state) { 
}    

 
GBool SWFOutputDev::beginType3Char(GfxState *state,
			       CharCode code, Unicode *u, int uLen)
{
    msg("<debug> beginType3Char %d, %08x, %d", code, *u, uLen);
    type3active = 1;
    /* the character itself is going to be passed using
       drawImageMask() */
    return gFalse; /* gTrue= is_in_cache? */
}

void SWFOutputDev::endType3Char(GfxState *state)
{
    type3active = 0;
    msg("<debug> endType3Char");
}

void SWFOutputDev::startPage(int pageNum, GfxState *state, double crop_x1, double crop_y1, double crop_x2, double crop_y2) 
{
    this->currentpage = pageNum;
    double x1,y1,x2,y2;
    int rot = doc->getPageRotate(1);
    laststate = state;
    msg("<verbose> startPage %d (%f,%f,%f,%f)\n", pageNum, crop_x1, crop_y1, crop_x2, crop_y2);
    if(rot!=0)
        msg("<verbose> page is rotated %d degrees\n", rot);

    /* state->transform(state->getX1(),state->getY1(),&x1,&y1);
    state->transform(state->getX2(),state->getY2(),&x2,&y2);
    Use CropBox, not MediaBox, as page size
    */
    
    /*x1 = crop_x1;
    y1 = crop_y1;
    x2 = crop_x2;
    y2 = crop_y2;*/
    state->transform(crop_x1,crop_y1,&x1,&y1);
    state->transform(crop_x2,crop_y2,&x2,&y2);

    if(x2<x1) {double x3=x1;x1=x2;x2=x3;}
    if(y2<y1) {double y3=y1;y1=y2;y2=y3;}

    /* apply user clip box */
    if(user_clipx1|user_clipy1|user_clipx2|user_clipy2) {
        if(user_clipx1 > x1) x1 = user_clipx1;
        if(user_clipx2 < x2) x2 = user_clipx2;
        if(user_clipy1 > y1) y1 = user_clipy1;
        if(user_clipy2 < y2) y2 = user_clipy2;
    }

    if(!outputstarted) {
        msg("<verbose> Bounding box is (%f,%f)-(%f,%f)", x1,y1,x2,y2);
        swfoutput_init(&output);
        outputstarted = 1;
    }
      
    swfoutput_newpage(&output, pageNum, user_movex, user_movey, (int)x1, (int)y1, (int)x2, (int)y2);
}

void SWFOutputDev::drawLink(Link *link, Catalog *catalog) 
{
    msg("<debug> drawlink\n");
    double x1, y1, x2, y2, w;
    GfxRGB rgb;
    swfcoord points[5];
    int x, y;

#ifdef XPDF_101
    link->getBorder(&x1, &y1, &x2, &y2, &w);
#else
    link->getRect(&x1, &y1, &x2, &y2);
#endif
    rgb.r = 0;
    rgb.g = 0;
    rgb.b = 1;
    cvtUserToDev(x1, y1, &x, &y);
    points[0].x = points[4].x = (int)x;
    points[0].y = points[4].y = (int)y;
    cvtUserToDev(x2, y1, &x, &y);
    points[1].x = (int)x;
    points[1].y = (int)y;
    cvtUserToDev(x2, y2, &x, &y);
    points[2].x = (int)x;
    points[2].y = (int)y;
    cvtUserToDev(x1, y2, &x, &y);
    points[3].x = (int)x;
    points[3].y = (int)y;

    LinkAction*action=link->getAction();
    char buf[128];
    char*s = 0;
    char*type = "-?-";
    char*url = 0;
    char*named = 0;
    int page = -1;
    switch(action->getKind())
    {
        case actionGoTo: {
            type = "GoTo";
            LinkGoTo *ha=(LinkGoTo *)link->getAction();
            LinkDest *dest=NULL;
            if (ha->getDest()==NULL) 
                dest=catalog->findDest(ha->getNamedDest());
            else dest=ha->getDest();
            if (dest){ 
              if (dest->isPageRef()){
                Ref pageref=dest->getPageRef();
                page=catalog->findPage(pageref.num,pageref.gen);
              }
              else  page=dest->getPageNum();
              sprintf(buf, "%d", page);
              s = strdup(buf);
            }
        }
        break;
        case actionGoToR: {
            type = "GoToR";
            LinkGoToR*l = (LinkGoToR*)action;
            GString*g = l->getNamedDest();
            if(g)
             s = strdup(g->getCString());
        }
        break;
        case actionNamed: {
            type = "Named";
            LinkNamed*l = (LinkNamed*)action;
            GString*name = l->getName();
            if(name) {
                s = strdup(name->lowerCase()->getCString());
                named = name->getCString();
                if(!strchr(s,':')) 
                {
                    if(strstr(s, "next") || strstr(s, "forward"))
                    {
                        page = currentpage + 1;
                    }
                    else if(strstr(s, "prev") || strstr(s, "back"))
                    {
                        page = currentpage - 1;
                    }
                    else if(strstr(s, "last") || strstr(s, "end"))
                    {
                        page = pagepos>0?pages[pagepos-1]:0;
                    }
                    else if(strstr(s, "first") || strstr(s, "top"))
                    {
                        page = 1;
                    }
                }
            }
        }
        break;
        case actionLaunch: {
            type = "Launch";
            LinkLaunch*l = (LinkLaunch*)action;
            GString * str = new GString(l->getFileName());
            str->append(l->getParams());
            s = strdup(str->getCString());
            delete str;
        }
        break;
        case actionURI: {
            type = "URI";
            LinkURI*l = (LinkURI*)action;
            GString*g = l->getURI();
            if(g) {
             url = g->getCString();
             s = strdup(url);
            }
        }
        break;
        case actionUnknown: {
            type = "Unknown";
            LinkUnknown*l = (LinkUnknown*)action;
            s = strdup("");
        }
        break;
        default: {
            msg("<error> Unknown link type!\n");
            break;
        }
    }
    if(!s) s = strdup("-?-");

    if(!linkinfo && (page || url))
    {
        msg("<notice> File contains links");
        linkinfo = 1;
    }
    if(page>0)
    {
        int t;
        for(t=0;t<pagepos;t++)
            if(pages[t]==page)
                break;
        if(t!=pagepos)
            swfoutput_linktopage(&output, t, points);
    }
    else if(url)
    {
        swfoutput_linktourl(&output, url, points);
    }
    else if(named)
    {
        swfoutput_namedlink(&output, named, points);
    }
    msg("<verbose> \"%s\" link to \"%s\" (%d)\n", type, FIXNULL(s), page);
    free(s);s=0;
}

void SWFOutputDev::saveState(GfxState *state) {
  msg("<debug> saveState\n");
  updateAll(state);
  if(clippos<64)
    clippos ++;
  else
    msg("<error> Too many nested states in pdf.");
  clipping[clippos] = 0;
};

void SWFOutputDev::restoreState(GfxState *state) {
  msg("<debug> restoreState\n");
  updateAll(state);
  while(clipping[clippos]) {
      swfoutput_endclip(&output);
      clipping[clippos]--;
  }
  clippos--;
}

char* SWFOutputDev::searchFont(char*name) 
{	
    int i;
    char*filename=0;
    int is_standard_font = 0;
	
    msg("<verbose> SearchFont(%s)", name);

    /* see if it is a pdf standard font */
    for(i=0;i<sizeof(pdf2t1map)/sizeof(mapping);i++) 
    {
	if(!strcmp(name, pdf2t1map[i].pdffont))
	{
	    name = pdf2t1map[i].filename;
	    is_standard_font = 1;
	    break;
	}
    }
    /* look in all font files */
    for(i=0;i<fontnum;i++) 
    {
	if(strstr(fonts[i].filename, name))
	{
	    if(!fonts[i].used) {

		fonts[i].used = 1;
		if(!is_standard_font)
		    msg("<notice> Using %s for %s", fonts[i].filename, name);
	    }
	    return strdup(fonts[i].filename);
	}
    }
    return 0;
}

void SWFOutputDev::updateLineWidth(GfxState *state)
{
    double width = state->getTransformedLineWidth();
    swfoutput_setlinewidth(&output, width);
}

void SWFOutputDev::updateLineCap(GfxState *state)
{
    int c = state->getLineCap();
}

void SWFOutputDev::updateLineJoin(GfxState *state)
{
    int j = state->getLineJoin();
}

void SWFOutputDev::updateFillColor(GfxState *state) 
{
    GfxRGB rgb;
    double opaq = state->getFillOpacity();
    state->getFillRGB(&rgb);

    swfoutput_setfillcolor(&output, (char)(rgb.r*255), (char)(rgb.g*255), 
	                            (char)(rgb.b*255), (char)(opaq*255));
}

void SWFOutputDev::updateStrokeColor(GfxState *state) 
{
    GfxRGB rgb;
    double opaq = state->getStrokeOpacity();
    state->getStrokeRGB(&rgb);

    swfoutput_setstrokecolor(&output, (char)(rgb.r*255), (char)(rgb.g*255), 
	                              (char)(rgb.b*255), (char)(opaq*255));
}

void FoFiWrite(void *stream, char *data, int len)
{
   fwrite(data, len, 1, (FILE*)stream);
}

char*SWFOutputDev::writeEmbeddedFontToFile(XRef*ref, GfxFont*font)
{
    char*tmpFileName = NULL;
    FILE *f;
    int c;
    char *fontBuf;
    int fontLen;
    Ref embRef;
    Object refObj, strObj;
    char namebuf[512];
    tmpFileName = mktmpname(namebuf);
    int ret;

    ret = font->getEmbeddedFontID(&embRef);
    if(!ret) {
	msg("<verbose> Didn't get embedded font id");
	/* not embedded- the caller should now search the font
	   directories for this font */
	return 0;
    }

    f = fopen(tmpFileName, "wb");
    if (!f) {
      msg("<error> Couldn't create temporary Type 1 font file");
	return 0;
    }

    /*if(font->isCIDFont()) {
	GfxCIDFont* cidFont = (GfxCIDFont *)font;
	GString c = cidFont->getCollection();
	msg("<notice> Collection: %s", c.getCString());
    }*/

    if (font->getType() == fontType1C ||
	font->getType() == fontCIDType0C) {
      if (!(fontBuf = font->readEmbFontFile(xref, &fontLen))) {
	fclose(f);
	msg("<error> Couldn't read embedded font file");
	return 0;
      }
#ifdef XPDF_101
      Type1CFontFile *cvt = new Type1CFontFile(fontBuf, fontLen);
      cvt->convertToType1(f);
#else
      FoFiType1C *cvt = FoFiType1C::make(fontBuf, fontLen);
      cvt->convertToType1(NULL, gTrue, FoFiWrite, f);
#endif
      //cvt->convertToCIDType0("test", f);
      //cvt->convertToType0("test", f);
      delete cvt;
      gfree(fontBuf);
    } else if(font->getType() == fontTrueType) {
      msg("<verbose> writing font using TrueTypeFontFile::writeTTF");
      if (!(fontBuf = font->readEmbFontFile(xref, &fontLen))) {
	fclose(f);
	msg("<error> Couldn't read embedded font file");
	return 0;
      }
#ifdef XPDF_101
      TrueTypeFontFile *cvt = new TrueTypeFontFile(fontBuf, fontLen);
      cvt->writeTTF(f);
#else
      FoFiTrueType *cvt = FoFiTrueType::make(fontBuf, fontLen);
      cvt->writeTTF(FoFiWrite, f);
#endif
      delete cvt;
      gfree(fontBuf);
    } else {
      font->getEmbeddedFontID(&embRef);
      refObj.initRef(embRef.num, embRef.gen);
      refObj.fetch(ref, &strObj);
      refObj.free();
      strObj.streamReset();
      int f4[4];
      char f4c[4];
      int t;
      for(t=0;t<4;t++) {
	  f4[t] = strObj.streamGetChar();
	  f4c[t] = (char)f4[t];
	  if(f4[t] == EOF)
	      break;
      }
      if(t==4) {
	  if(!strncmp(f4c, "true", 4)) {
	      /* some weird TTF fonts don't start with 0,1,0,0 but with "true".
		 Change this on the fly */
	      f4[0] = f4[2] = f4[3] = 0;
	      f4[1] = 1;
	  }
	  fputc(f4[0], f);
	  fputc(f4[1], f);
	  fputc(f4[2], f);
	  fputc(f4[3], f);

	  while ((c = strObj.streamGetChar()) != EOF) {
	    fputc(c, f);
	  }
      }
      strObj.streamClose();
      strObj.free();
    }
    fclose(f);

    return strdup(tmpFileName);
}
    
char* searchForSuitableFont(GfxFont*gfxFont)
{
    char*name = getFontName(gfxFont);
    char*fontname = 0;
    char*filename = 0;
    
#ifdef HAVE_FONTCONFIG
    FcPattern *pattern, *match;
    FcResult result;
    FcChar8 *v;

    static int fcinitcalled = false; 
    
    // call init ony once
    if (!fcinitcalled) {
        fcinitcalled = true;
	FcInit(); //leaks
    }
   
    pattern = FcPatternBuild(NULL, FC_FAMILY, FcTypeString, name, NULL);
    if (gfxFont->isItalic()) // check for italic
	FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ITALIC);
    if (gfxFont->isBold()) // check for bold
        FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);

    // configure and match using the original font name 
    FcConfigSubstitute(0, pattern, FcMatchPattern); 
    FcDefaultSubstitute(pattern);
    match = FcFontMatch(0, pattern, &result);
    
    if (FcPatternGetString(match, "family", 0, &v) == FcResultMatch) {
        // if we get an exact match
        if (strcmp((char *)v, name) == 0) {
	    if (FcPatternGetString(match, "file", 0, &v) == FcResultMatch) {
		filename = strdup((char*)v);
		char *nfn = strrchr(filename, '/');
		if(nfn) fontname = strdup(nfn+1);
		else    fontname = filename;
            }
        } else {
            // initialize patterns
            FcPatternDestroy(pattern);
    	    FcPatternDestroy(match);

            // now match against serif etc.
	    if (gfxFont->isSerif()) {
                pattern = FcPatternBuild (NULL, FC_FAMILY, FcTypeString, "serif", NULL);
            } else if (gfxFont->isFixedWidth()) {
                pattern = FcPatternBuild (NULL, FC_FAMILY, FcTypeString, "monospace", NULL);
            } else {
                pattern = FcPatternBuild (NULL, FC_FAMILY, FcTypeString, "sans", NULL);
            }

            // check for italic
            if (gfxFont->isItalic()) {
                int bb = FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ITALIC);
            }
            // check for bold
            if (gfxFont->isBold()) {
                int bb = FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);
            }

            // configure and match using serif etc
    	    FcConfigSubstitute (0, pattern, FcMatchPattern);
            FcDefaultSubstitute (pattern);
            match = FcFontMatch (0, pattern, &result);
            
            if (FcPatternGetString(match, "file", 0, &v) == FcResultMatch) {
		filename = strdup((char*)v);
		char *nfn = strrchr(filename, '/');
		if(nfn) fontname = strdup(nfn+1);
		else    fontname = filename;
    	    }
        }        
    }

    //printf("FONTCONFIG: pattern");
    //FcPatternPrint(pattern);
    //printf("FONTCONFIG: match");
    //FcPatternPrint(match);
 
    FcPatternDestroy(pattern);
    FcPatternDestroy(match);

    pdfswf_addfont(filename);
    return fontname;
#else
    return 0;
#endif
}

char* SWFOutputDev::substituteFont(GfxFont*gfxFont, char* oldname)
{
    char*fontname = 0, *filename = 0;
    msg("<notice> subsituteFont(%s)", oldname);

    if(!(fontname = searchForSuitableFont(gfxFont))) {
	fontname = "Times-Roman";
    }
    filename = searchFont(fontname);

    if(substitutepos>=sizeof(substitutesource)/sizeof(char*)) {
	msg("<fatal> Too many fonts in file.");
	exit(1);
    }
    if(oldname) {
	substitutesource[substitutepos] = oldname;
	substitutetarget[substitutepos] = fontname;
	msg("<notice> substituting %s -> %s", FIXNULL(oldname), FIXNULL(fontname));
	substitutepos ++;
    }
    return strdup(filename);
}

void unlinkfont(char* filename)
{
    int l;
    if(!filename)
	return;
    l=strlen(filename);
    unlink(filename);
    if(!strncmp(&filename[l-4],".afm",4)) {
	memcpy(&filename[l-4],".pfb",4);
	unlink(filename);
	memcpy(&filename[l-4],".pfa",4);
	unlink(filename);
	memcpy(&filename[l-4],".afm",4);
	return;
    } else 
    if(!strncmp(&filename[l-4],".pfa",4)) {
	memcpy(&filename[l-4],".afm",4);
	unlink(filename);
	memcpy(&filename[l-4],".pfa",4);
	return;
    } else 
    if(!strncmp(&filename[l-4],".pfb",4)) {
	memcpy(&filename[l-4],".afm",4);
	unlink(filename);
	memcpy(&filename[l-4],".pfb",4);
	return;
    }
}

void SWFOutputDev::setXRef(PDFDoc*doc, XRef *xref) 
{
    this->doc = doc;
    this->xref = xref;
}


void SWFOutputDev::updateFont(GfxState *state) 
{
    GfxFont*gfxFont = state->getFont();
      
    if (!gfxFont) {
	return;
    }  
    char * fontid = getFontID(gfxFont);
    
    int t;
    /* first, look if we substituted this font before-
       this way, we don't initialize the T1 Fonts
       too often */
    for(t=0;t<substitutepos;t++) {
	if(!strcmp(fontid, substitutesource[t])) {
	    fontid = substitutetarget[t];
	    break;
	}
    }

    /* second, see if swfoutput already has this font
       cached- if so, we are done */
    if(swfoutput_queryfont(&output, fontid))
    {
	swfoutput_setfont(&output, fontid, 0);
	
	msg("<debug> updateFont(%s) [cached]", fontid);
	return;
    }

    // look for Type 3 font
    if (gfxFont->getType() == fontType3) {
	if(!type3Warning) {
	    type3Warning = gTrue;
	    showFontError(gfxFont, 2);
	}
	return;
    }

    /* now either load the font, or find a substitution */

    Ref embRef;
    GBool embedded = gfxFont->getEmbeddedFontID(&embRef);

    char*fileName = 0;
    int del = 0;
    if(embedded &&
       (gfxFont->getType() == fontType1 ||
	gfxFont->getType() == fontType1C ||
	//gfxFont->getType() == fontCIDType0C ||
	gfxFont->getType() == fontTrueType ||
	gfxFont->getType() == fontCIDType2
       ))
    {
      fileName = writeEmbeddedFontToFile(xref, gfxFont);
      if(!fileName) showFontError(gfxFont,0);
      else del = 1;
    } else {
      char * fontname = getFontName(gfxFont);
      fileName = searchFont(fontname);
      if(!fileName) showFontError(gfxFont,0);
    }
    if(!fileName) {
	char * fontname = getFontName(gfxFont);
	msg("<warning> Font %s %scould not be loaded.", fontname, embedded?"":"(not embedded) ");
	msg("<warning> Try putting a TTF version of that font (named \"%s.ttf\") into /swftools/fonts", fontname);
	fileName = substituteFont(gfxFont, fontid);
	if(fontid) { fontid = substitutetarget[substitutepos-1]; /*ugly hack*/};
	msg("<notice> Font is now %s (%s)", fontid, fileName);
    }

    if(!fileName) {
	msg("<error> Couldn't set font %s\n", fontid);
	return;
    }
	
    msg("<verbose> updateFont(%s) -> %s", fontid, fileName);
    dumpFontInfo("<verbose>", gfxFont);

    swfoutput_setfont(&output, fontid, fileName);
   
    if(fileName && del)
	unlinkfont(fileName);
    if(fileName)
        free(fileName);
}

#define SQR(x) ((x)*(x))

unsigned char* antialize(unsigned char*data, int width, int height, int newwidth, int newheight, int palettesize)
{
    if((newwidth<2 || newheight<2) ||
       (width<=newwidth || height<=newheight))
	return 0;
    unsigned char*newdata;
    int x,y;
    newdata= (unsigned char*)malloc(newwidth*newheight);
    int t;
    double fx = (double)(width)/newwidth;
    double fy = (double)(height)/newheight;
    double px = 0;
    int blocksize = (int)(8192/(fx*fy));
    int r = 8192*256/palettesize;
    for(x=0;x<newwidth;x++) {
	double ex = px + fx;
	int fromx = (int)px;
	int tox = (int)ex;
	int xweight1 = (int)(((fromx+1)-px)*256);
	int xweight2 = (int)((ex-tox)*256);
	double py =0;
	for(y=0;y<newheight;y++) {
	    double ey = py + fy;
	    int fromy = (int)py;
	    int toy = (int)ey;
	    int yweight1 = (int)(((fromy+1)-py)*256);
	    int yweight2 = (int)((ey-toy)*256);
	    int a = 0;
	    int xx,yy;
	    for(xx=fromx;xx<=tox;xx++)
	    for(yy=fromy;yy<=toy;yy++) {
		int b = 1-data[width*yy+xx];
		int weight=256;
		if(xx==fromx) weight = (weight*xweight1)/256;
		if(xx==tox) weight = (weight*xweight2)/256;
		if(yy==fromy) weight = (weight*yweight1)/256;
		if(yy==toy) weight = (weight*yweight2)/256;
		a+=b*weight;
	    }
	    //if(a) a=(palettesize-1)*r/blocksize;
	    newdata[y*newwidth+x] = (a*blocksize)/r;
	    py = ey;
	}
	px = ex;
    }
    return newdata;
}

void SWFOutputDev::drawGeneralImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GfxImageColorMap*colorMap, GBool invert,
				   GBool inlineImg, int mask)
{
  FILE *fi;
  int c;
  char fileName[128];
  double x1,y1,x2,y2,x3,y3,x4,y4;
  ImageStream *imgStr;
  Guchar pixBuf[4];
  GfxRGB rgb;
  int ncomps = 1;
  int bits = 1;
				 
  if(colorMap) {
    ncomps = colorMap->getNumPixelComps();
    bits = colorMap->getBits();
  }
  imgStr = new ImageStream(str, width, ncomps,bits);
  imgStr->reset();

  if(!width || !height || (height<=1 && width<=1))
  {
      msg("<verbose> Ignoring %d by %d image", width, height);
      unsigned char buf[8];
      int x,y;
      for (y = 0; y < height; ++y)
      for (x = 0; x < width; ++x) {
	  imgStr->getPixel(buf);
      }
      delete imgStr;
      return;
  }
  
  state->transform(0, 1, &x1, &y1);
  state->transform(0, 0, &x2, &y2);
  state->transform(1, 0, &x3, &y3);
  state->transform(1, 1, &x4, &y4);

  if(!pbminfo && !(str->getKind()==strDCT)) {
      if(!type3active) {
	  msg("<notice> file contains pbm pictures %s",mask?"(masked)":"");
	  pbminfo = 1;
      }
      if(mask)
      msg("<verbose> drawing %d by %d masked picture\n", width, height);
  }
  if(!jpeginfo && (str->getKind()==strDCT)) {
      msg("<notice> file contains jpeg pictures");
      jpeginfo = 1;
  }

  if(mask) {
      int yes=0,i,j;
      unsigned char buf[8];
      int xid = 0;
      int yid = 0;
      int x,y;
      unsigned char*pic = new unsigned char[width*height];
      RGBA pal[256];
      GfxRGB rgb;
      state->getFillRGB(&rgb);
      memset(pal,255,sizeof(pal));
      pal[0].r = (int)(rgb.r*255); pal[0].g = (int)(rgb.g*255); 
      pal[0].b = (int)(rgb.b*255); pal[0].a = 255;
      pal[1].r = 0; pal[1].g = 0; pal[1].b = 0; pal[1].a = 0;
      int numpalette = 2;
      xid += pal[1].r*3 + pal[1].g*11 + pal[1].b*17;
      yid += pal[1].r*7 + pal[1].g*5 + pal[1].b*23;
      int realwidth = (int)sqrt(SQR(x2-x3) + SQR(y2-y3));
      int realheight = (int)sqrt(SQR(x1-x2) + SQR(y1-y2));
      for (y = 0; y < height; ++y)
      for (x = 0; x < width; ++x)
      {
	    imgStr->getPixel(buf);
	    if(invert) 
		buf[0]=1-buf[0];
	    pic[width*y+x] = buf[0];
	    xid+=x*buf[0]+1;
	    yid+=y*buf[0]*3+1;
      }
      
      /* the size of the drawn image is added to the identifier
	 as the same image may require different bitmaps if displayed
	 at different sizes (due to antialiasing): */
      if(type3active) {
	  xid += realwidth;
	  yid += realheight;
      }
      int t,found = -1;
      for(t=0;t<picpos;t++)
      {
	  if(pic_xids[t] == xid &&
	     pic_yids[t] == yid) {
	      /* if the image was antialiased, the size has changed: */
	      width = pic_width[t];
	      height = pic_height[t];
	      found = t;break;
	  }
      }
      if(found<0) {
	  if(type3active) {
	      numpalette = 16;
	      unsigned char*pic2 = 0;
	      
	      pic2 = antialize(pic,width,height,realwidth,realheight, numpalette);

	      if(pic2) {
		  width = realwidth;
		  height = realheight;
		  free(pic);
		  pic = pic2;
		  /* make a black/white palette */
		  int t;
		  GfxRGB rgb2;
		  rgb2.r = 1 - rgb.r;
		  rgb2.g = 1 - rgb.g;
		  rgb2.b = 1 - rgb.b;

		  float r = 255/(numpalette-1);
		  for(t=0;t<numpalette;t++) {
		      /*pal[t].r = (U8)(t*r*rgb.r+(numpalette-1-t)*r*rgb2.r);
		      pal[t].g = (U8)(t*r*rgb.g+(numpalette-1-t)*r*rgb2.g);
		      pal[t].b = (U8)(t*r*rgb.b+(numpalette-1-t)*r*rgb2.b);
		      pal[t].a = 255; */
		      pal[t].r = (U8)(255*rgb.r);
		      pal[t].g = (U8)(255*rgb.g);
		      pal[t].b = (U8)(255*rgb.b);
		      pal[t].a = (U8)(t*r);
		  }
	      }
	  }
	  pic_ids[picpos] = swfoutput_drawimagelosslessN(&output, pic, pal, width, height, 
		  x1,y1,x2,y2,x3,y3,x4,y4, numpalette);
	  pic_xids[picpos] = xid;
	  pic_yids[picpos] = yid;
	  pic_width[picpos] = width;
	  pic_height[picpos] = height;
	  if(picpos<1024)
	      picpos++;
      } else {
	  swfoutput_drawimageagain(&output, pic_ids[found], width, height,
		  x1,y1,x2,y2,x3,y3,x4,y4);
      }
      free(pic);
      delete imgStr;
      return;
  } 

  int x,y;
  
  if(colorMap->getNumPixelComps()!=1 || str->getKind()==strDCT)
  {
      RGBA*pic=new RGBA[width*height];
      int xid = 0;
      int yid = 0;
      for (y = 0; y < height; ++y) {
	for (x = 0; x < width; ++x) {
	  int r,g,b,a;
	  imgStr->getPixel(pixBuf);
	  colorMap->getRGB(pixBuf, &rgb);
	  pic[width*y+x].r = r = (U8)(rgb.r * 255 + 0.5);
	  pic[width*y+x].g = g = (U8)(rgb.g * 255 + 0.5);
	  pic[width*y+x].b = b = (U8)(rgb.b * 255 + 0.5);
	  pic[width*y+x].a = a = 255;//(U8)(rgb.a * 255 + 0.5);
	  xid += x*r+x*b*3+x*g*7+x*a*11;
	  yid += y*r*3+y*b*17+y*g*19+y*a*11;
	}
      }
      int t,found = -1;
      for(t=0;t<picpos;t++)
      {
	  if(pic_xids[t] == xid &&
	     pic_yids[t] == yid) {
	      found = t;break;
	  }
      }
      if(found<0) {
	  if(str->getKind()==strDCT)
	      pic_ids[picpos] = swfoutput_drawimagejpeg(&output, pic, width, height, 
		      x1,y1,x2,y2,x3,y3,x4,y4);
	  else
	      pic_ids[picpos] = swfoutput_drawimagelossless(&output, pic, width, height, 
		      x1,y1,x2,y2,x3,y3,x4,y4);
	  pic_xids[picpos] = xid;
	  pic_yids[picpos] = yid;
	  pic_width[picpos] = width;
	  pic_height[picpos] = height;
	  if(picpos<1024)
	      picpos++;
      } else {
	  swfoutput_drawimageagain(&output, pic_ids[found], width, height,
		  x1,y1,x2,y2,x3,y3,x4,y4);
      }
      delete pic;
      delete imgStr;
      return;
  }
  else
  {
      U8*pic = new U8[width*height];
      RGBA pal[256];
      int t;
      int xid=0,yid=0;
      for(t=0;t<256;t++)
      {
	  int r,g,b,a;
	  pixBuf[0] = t;
	  colorMap->getRGB(pixBuf, &rgb);
	  pal[t].r = r = (U8)(rgb.r * 255 + 0.5);
	  pal[t].g = g = (U8)(rgb.g * 255 + 0.5);
	  pal[t].b = b = (U8)(rgb.b * 255 + 0.5);
	  pal[t].a = a = 255;//(U8)(rgb.b * 255 + 0.5);
	  xid += t*r+t*b*3+t*g*7+t*a*11;
	  xid += (~t)*r+t*b*3+t*g*7+t*a*11;
      }
      for (y = 0; y < height; ++y) {
	for (x = 0; x < width; ++x) {
	  imgStr->getPixel(pixBuf);
	  pic[width*y+x] = pixBuf[0];
	  xid += x*pixBuf[0]*7;
	  yid += y*pixBuf[0]*3;
	}
      }
      int found = -1;
      for(t=0;t<picpos;t++)
      {
	  if(pic_xids[t] == xid &&
	     pic_yids[t] == yid) {
	      found = t;break;
	  }
      }
      if(found<0) {
	  pic_ids[picpos] = swfoutput_drawimagelosslessN(&output, pic, pal, width, height, 
		  x1,y1,x2,y2,x3,y3,x4,y4,256);
	  pic_xids[picpos] = xid;
	  pic_yids[picpos] = yid;
	  pic_width[picpos] = width;
	  pic_height[picpos] = height;
	  if(picpos<1024)
	      picpos++;
      } else {
	  swfoutput_drawimageagain(&output, pic_ids[found], width, height,
		  x1,y1,x2,y2,x3,y3,x4,y4);
      }
      delete pic;
      delete imgStr;
      return;
  }
}

void SWFOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool inlineImg) 
{
  msg("<verbose> drawImageMask %dx%d, invert=%d inline=%d", width, height, invert, inlineImg);
  drawGeneralImage(state,ref,str,width,height,0,invert,inlineImg,1);
}

void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg)
{
  msg("<verbose> drawImage %dx%d, %s %s, inline=%d", width, height, 
	  colorMap?"colorMap":"no colorMap", 
	  maskColors?"maskColors":"no maskColors",
	  inlineImg);
  if(colorMap)
      msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d\n", colorMap->getNumPixelComps(),
	      colorMap->getBits(),colorMap->getColorSpace()->getMode());
  drawGeneralImage(state,ref,str,width,height,colorMap,0,inlineImg,0);
}

SWFOutputDev*output = 0; 

static void printInfoString(Dict *infoDict, char *key, char *fmt) {
  Object obj;
  GString *s1, *s2;
  int i;

  if (infoDict->lookup(key, &obj)->isString()) {
    s1 = obj.getString();
    if ((s1->getChar(0) & 0xff) == 0xfe &&
	(s1->getChar(1) & 0xff) == 0xff) {
      s2 = new GString();
      for (i = 2; i < obj.getString()->getLength(); i += 2) {
	if (s1->getChar(i) == '\0') {
	  s2->append(s1->getChar(i+1));
	} else {
	  delete s2;
	  s2 = new GString("<unicode>");
	  break;
	}
      }
      printf(fmt, s2->getCString());
      delete s2;
    } else {
      printf(fmt, s1->getCString());
    }
  }
  obj.free();
}

static void printInfoDate(Dict *infoDict, char *key, char *fmt) {
  Object obj;
  char *s;

  if (infoDict->lookup(key, &obj)->isString()) {
    s = obj.getString()->getCString();
    if (s[0] == 'D' && s[1] == ':') {
      s += 2;
    }
    printf(fmt, s);
  }
  obj.free();
}

void pdfswf_setparameter(char*name, char*value)
{
    msg("<verbose> setting parameter %s to \"%s\"", name, value);
    if(!strcmp(name, "caplinewidth")) {
	caplinewidth = atof(value);
    } else if(!strcmp(name, "zoom")) {
	zoom = atoi(value);
    } else if(!strcmp(name, "fontdir")) {
        pdfswf_addfontdir(value);
    } else {
	swfoutput_setparameter(name, value);
    }
}
void pdfswf_addfont(char*filename)
{
    fontfile_t f;
    memset(&f, 0, sizeof(fontfile_t));
    f.filename = filename;
    if(fontnum < sizeof(fonts)/sizeof(fonts[0])) {
        fonts[fontnum++] = f;
    } else {
        msg("<error> Too many external fonts. Not adding font file \"%s\".", filename);
    }
}

void pdfswf_addfontdir(char*dirname)
{
#ifdef HAVE_DIRENT_H
    msg("<notice> Adding %s to font directories", dirname);
    DIR*dir = opendir(dirname);
    if(!dir) {
	msg("<warning> Couldn't open directory %s\n", dirname);
	return;
    }
    struct dirent*ent;
    while(1) {
	ent = readdir (dir);
	if (!ent) 
	    break;
	int l;
	char*name = ent->d_name;
	char type = 0;
	if(!name) continue;
	l=strlen(name);
	if(l<4)
	    continue;
	if(!strncasecmp(&name[l-4], ".pfa", 4)) 
	    type=1;
	if(!strncasecmp(&name[l-4], ".pfb", 4)) 
	    type=3;
	if(!strncasecmp(&name[l-4], ".ttf", 4)) 
	    type=2;
	if(type)
	{
	    char*fontname = (char*)malloc(strlen(dirname)+strlen(name)+2);
	    strcpy(fontname, dirname);
#ifdef WIN32
		strcat(fontname, "\\");
#else
		strcat(fontname, "/");
#endif
	    strcat(fontname, name);
	    msg("<verbose> Adding %s to fonts", fontname);
	    pdfswf_addfont(fontname);
	}
    }
    closedir(dir);
#else
    msg("<warning> No dirent.h- unable to add font dir %s");
#endif
}


typedef struct _pdf_doc_internal
{
    int protect;
    PDFDoc*doc;
} pdf_doc_internal_t;
typedef struct _pdf_page_internal
{
} pdf_page_internal_t;
typedef struct _swf_output_internal
{
    SWFOutputDev*outputDev;
} swf_output_internal_t;

pdf_doc_t* pdf_init(char*filename, char*userPassword)
{
    pdf_doc_t*pdf_doc = (pdf_doc_t*)malloc(sizeof(pdf_doc_t));
    memset(pdf_doc, 0, sizeof(pdf_doc_t));
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)malloc(sizeof(pdf_doc_internal_t));
    memset(i, 0, sizeof(pdf_doc_internal_t));
    pdf_doc->internal = i;
    
    GString *fileName = new GString(filename);
    GString *userPW;
    Object info;

    // read config file
    globalParams = new GlobalParams("");

    // open PDF file
    if (userPassword && userPassword[0]) {
      userPW = new GString(userPassword);
    } else {
      userPW = NULL;
    }
    i->doc = new PDFDoc(fileName, userPW);
    if (userPW) {
      delete userPW;
    }
    if (!i->doc->isOk()) {
        return 0;
    }

    // print doc info
    i->doc->getDocInfo(&info);
    if (info.isDict() &&
      (getScreenLogLevel()>=LOGLEVEL_NOTICE)) {
      printInfoString(info.getDict(), "Title",        "Title:        %s\n");
      printInfoString(info.getDict(), "Subject",      "Subject:      %s\n");
      printInfoString(info.getDict(), "Keywords",     "Keywords:     %s\n");
      printInfoString(info.getDict(), "Author",       "Author:       %s\n");
      printInfoString(info.getDict(), "Creator",      "Creator:      %s\n");
      printInfoString(info.getDict(), "Producer",     "Producer:     %s\n");
      printInfoDate(info.getDict(),   "CreationDate", "CreationDate: %s\n");
      printInfoDate(info.getDict(),   "ModDate",      "ModDate:      %s\n");
      printf("Pages:        %d\n", i->doc->getNumPages());
      printf("Linearized:   %s\n", i->doc->isLinearized() ? "yes" : "no");
      printf("Encrypted:    ");
      if (i->doc->isEncrypted()) {
        printf("yes (print:%s copy:%s change:%s addNotes:%s)\n",
               i->doc->okToPrint() ? "yes" : "no",
               i->doc->okToCopy() ? "yes" : "no",
               i->doc->okToChange() ? "yes" : "no",
               i->doc->okToAddNotes() ? "yes" : "no");
      } else {
        printf("no\n");
      }
    }
    info.free();
                   
    pdf_doc->num_pages = i->doc->getNumPages();
    i->protect = 0;
    if (i->doc->isEncrypted()) {
          if(!i->doc->okToCopy()) {
              printf("PDF disallows copying.\n");
              return 0;
          }
          if(!i->doc->okToChange() || !i->doc->okToAddNotes())
              i->protect = 1;
    }
   
    return pdf_doc;
}

void pdfswf_preparepage(int page)
{
    /*FIXME*/
    if(!pages) {
	pages = (int*)malloc(1024*sizeof(int));
	pagebuflen = 1024;
    } else {
	if(pagepos == pagebuflen)
	{
	    pagebuflen+=1024;
	    pages = (int*)realloc(pages, pagebuflen);
	}
    }
    pages[pagepos++] = page;
}

class MemCheck
{
    public: ~MemCheck()
    {
        delete globalParams;
        Object::memCheck(stderr);
        gMemReport(stderr);
    }
} myMemCheck;

void pdf_destroy(pdf_doc_t*pdf_doc)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)pdf_doc->internal;

    msg("<debug> pdfswf.cc: pdfswf_close()");
    delete i->doc; i->doc=0;
    
    free(pages); pages = 0; //FIXME

    free(pdf_doc->internal);pdf_doc->internal=0;
    free(pdf_doc);pdf_doc=0;
}

pdf_page_t* pdf_getpage(pdf_doc_t*pdf_doc, int page)
{
    pdf_doc_internal_t*di= (pdf_doc_internal_t*)pdf_doc->internal;

    if(page < 1 || page > pdf_doc->num_pages)
        return 0;
    
    pdf_page_t* pdf_page = (pdf_page_t*)malloc(sizeof(pdf_page_t));
    pdf_page_internal_t*pi= (pdf_page_internal_t*)malloc(sizeof(pdf_page_internal_t));
    memset(pi, 0, sizeof(pdf_page_internal_t));
    pdf_page->internal = pi;

    pdf_page->parent = pdf_doc;
    pdf_page->nr = page;
    return pdf_page;
}

void pdf_page_destroy(pdf_page_t*pdf_page)
{
    pdf_page_internal_t*i= (pdf_page_internal_t*)pdf_page->internal;
    free(pdf_page->internal);pdf_page->internal = 0;
    free(pdf_page);pdf_page=0;
}

swf_output_t* swf_output_init() 
{
    swf_output_t*swf_output = (swf_output_t*)malloc(sizeof(swf_output_t));
    memset(swf_output, 0, sizeof(swf_output_t));
    swf_output_internal_t*i= (swf_output_internal_t*)malloc(sizeof(swf_output_internal_t));
    memset(i, 0, sizeof(swf_output_internal_t));
    swf_output->internal = i;

    i->outputDev = new SWFOutputDev();
    return swf_output;
}

void swf_output_setparameter(swf_output_t*swf_output, char*name, char*value)
{
    /* FIXME */
    pdfswf_setparameter(name, value);
}

int swf_output_save(swf_output_t*swf, char*filename)
{
    swf_output_internal_t*i= (swf_output_internal_t*)swf->internal;
    int ret = i->outputDev->save(filename);
    i->outputDev->getDimensions(&swf->x1, &swf->y1, &swf->x2, &swf->y2);
    return ret;
}

void swf_output_destroy(swf_output_t*output)
{
    swf_output_internal_t*i = (swf_output_internal_t*)output->internal;
    delete i->outputDev; i->outputDev=0;
    free(output->internal);output->internal=0;
    free(output);
}

void pdf_page_render2(pdf_page_t*page, swf_output_t*swf)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    swf_output_internal_t*si = (swf_output_internal_t*)swf->internal;

    if(pi->protect) {
        swfoutput_setparameter("protect", "1");
    }
    si->outputDev->setXRef(pi->doc, pi->doc->getXRef());
#ifdef XPDF_101
    pi->doc->displayPage((OutputDev*)si->outputDev, page->nr, /*zoom*/zoom, /*rotate*/0, /*doLinks*/(int)1);
#else
    pi->doc->displayPage((OutputDev*)si->outputDev, page->nr, zoom, zoom, /*rotate*/0, true, /*doLinks*/(int)1);
#endif
    si->outputDev->getDimensions(&swf->x1, &swf->y1, &swf->x2, &swf->y2);
}

void pdf_page_rendersection(pdf_page_t*page, swf_output_t*output, int x, int y, int x1, int y1, int x2, int y2)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    swf_output_internal_t*si = (swf_output_internal_t*)output->internal;

    si->outputDev->setMove(x,y);
    if((x1|y1|x2|y2)==0) x2++;
    si->outputDev->setClip(x1,y1,x2,y2);

    pdf_page_render2(page, output);
}
void pdf_page_render(pdf_page_t*page, swf_output_t*output)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    swf_output_internal_t*si = (swf_output_internal_t*)output->internal;
    
    si->outputDev->setMove(0,0);
    si->outputDev->setClip(0,0,0,0);
    
    pdf_page_render2(page, output);
}


pdf_page_info_t* pdf_page_getinfo(pdf_page_t*page)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    pdf_page_internal_t*i= (pdf_page_internal_t*)page->internal;
    pdf_page_info_t*info = (pdf_page_info_t*)malloc(sizeof(pdf_page_info_t));
    memset(info, 0, sizeof(pdf_page_info_t));

    InfoOutputDev*output = new InfoOutputDev;
    
#ifdef XPDF_101
    pi->doc->displayPage((OutputDev*)output, page->nr, /*zoom*/zoom, /*rotate*/0, /*doLinks*/(int)1);
#else
    pi->doc->displayPage((OutputDev*)output, page->nr, zoom, zoom, /*rotate*/0, true, /*doLinks*/(int)1);
#endif

    info->xMin = output->x1;
    info->yMin = output->y1;
    info->xMax = output->x2;
    info->yMax = output->y2;
    info->number_of_images = output->num_images;
    info->number_of_links = output->num_links;
    info->number_of_fonts = output->num_fonts;

    delete output;

    return info;
}

void pdf_page_info_destroy(pdf_page_info_t*info)
{
    free(info);
}
