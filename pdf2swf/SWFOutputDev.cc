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
//xpdf header files
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
#include "Params.h"
#include "Error.h"
#include "config.h"
#include "OutputDev.h"
#include "GfxState.h"
#include "GfxFont.h"
#include "FontFile.h"
//swftools header files
#include "swfoutput.h"
extern "C" {
#include "../lib/log.h"
}

static char* swffilename = 0;

static void printInfoString(Dict *infoDict, char *key, char *fmt);
static void printInfoDate(Dict *infoDict, char *key, char *fmt);

static char userPassword[33] = "";
static GBool printVersion = gFalse;
static GBool printHelp = gFalse;

double fontsizes[] = 
{
 0.833,0.833,0.889,0.889,0.788,0.722,0.833,0.778,0.600,0.600,0.600,0.600,0.576,0.576,0.576,0.576
};
char*fontnames[]={
"Helvetica",             
"Helvetica-Bold",        
"Helvetica-BoldOblique", 
"Helvetica-Oblique",     
"Times-Roman",           
"Times-Bold",            
"Times-BoldItalic",      
"Times-Italic",          
"Courier",               
"Courier-Bold",          
"Courier-BoldOblique",   
"Courier-Oblique",       
"Symbol",                
"Symbol",                
"Symbol",                
"Symbol",
"ZapfDingBats"
};

struct mapping {
    char*pdffont;
    char*filename;
    int id;
} pdf2t1map[] ={
{"Times-Roman",           "n021003l.pfb"},
{"Times-Italic",          "n021023l.pfb"},
{"Times-Bold",            "n021004l.pfb"},
{"Times-BoldItalic",      "n021024l.pfb"},
{"Helvetica",             "n019003l.pfb"},
{"Helvetica-Oblique",     "n019023l.pfb"},
{"Helvetica-Bold",        "n019004l.pfb"},
{"Helvetica-BoldOblique", "n019024l.pfb"},
{"Courier",               "n022003l.pfb"},
{"Courier-Oblique",       "n022023l.pfb"},
{"Courier-Bold",          "n022004l.pfb"},
{"Courier-BoldOblique",   "n022024l.pfb"},
{"Symbol",                "s050000l.pfb"},
{"ZapfDingbats",          "d050000l.pfb"}};

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

class GfxState;
class GfxImageColorMap;

class SWFOutputDev:  public OutputDev {
  struct swfoutput output;
  int outputstarted;
public:

  // Constructor.
  SWFOutputDev();

  // Destructor.
  virtual ~SWFOutputDev() ;

  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown();

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar();

  //----- initialization and control

  // Start a page.
  virtual void startPage(int pageNum, GfxState *state) ;

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
  
  virtual void updateAll(GfxState *state) 
  {
      updateFont(state);
      updateFillColor(state);
      updateStrokeColor(state);
      updateLineWidth(state);
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
			double dx, double dy, Guchar c) ;
  virtual void drawChar16(GfxState *state, double x, double y,
			  double dx, double dy, int c) ;

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);
  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 GBool inlineImg);

  private:
  int clipping[32];
  int clippos;

  int setT1Font(char*name,FontEncoding*enc);
  int t1id;
};

char mybuf[1024];
char* gfxstate2str(GfxState *state)
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

  if(state->getFont() && state->getFont()->getName() && state->getFont()->getName()->getCString())
  bufpos+=sprintf(bufpos,"F\"%s\" ",((state->getFont())->getName())->getCString());
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

void dumpFontInfo(char*loglevel, GfxFont*font);
int lastdumps[1024];
int lastdumppos = 0;
/* nr = 0  unknown
   nr = 1  substituting
   nr = 2  type 3
 */
void showFontError(GfxFont*font, int nr) 
{  
    Ref r=font->getID();
    int t;
    for(t=0;t<lastdumppos;t++)
	if(lastdumps[t] == r.num)
	    break;
    if(t < lastdumppos)
      return;
    if(lastdumppos<sizeof(lastdumps)/sizeof(int))
    lastdumps[lastdumppos++] = r.num;
    if(nr == 0)
      logf("<error> The following font caused problems:");
    else if(nr == 1)
      logf("<error> The following font caused problems (substituting):");
    else if(nr == 2)
      logf("<error> This document contains Type 3 Fonts: (some text may be incorrectly displayed)");

    dumpFontInfo("<error>", font);
}

void dumpFontInfo(char*loglevel, GfxFont*font)
{
  GString *gstr;
  char*name;
  gstr = font->getName();
  Ref r=font->getID();
  logf("%s=========== %s (ID:%d,%d) ==========\n", loglevel, gstr?gstr->getCString():"(unknown font)", r.num,r.gen);

  gstr  = font->getTag();
  if(gstr) 
   logf("%sTag: %s\n", loglevel, gstr->getCString());
  if(font->is16Bit()) logf("%sis 16 bit\n", loglevel);

  GfxFontType type=font->getType();
  switch(type) {
    case fontUnknownType:
     logf("%sType: unknown\n",loglevel);
    break;
    case fontType0:
     logf("%sType: 0\n",loglevel);
    break;
    case fontType1:
     logf("%sType: 1\n",loglevel);
    break;
    case fontType1C:
     logf("%sType: 1C\n",loglevel);
    break;
    case fontType3:
     logf("%sType: 3\n",loglevel);
    break;
    case fontTrueType:
     logf("%sType: TrueType\n",loglevel);
    break;
  }
  
  Ref embRef;
  GBool embedded = font->getEmbeddedFontID(&embRef);
  name = font->getEmbeddedFontName();
  if(embedded)
   logf("%sEmbedded name: %s id: %d\n",loglevel, name, embRef.num);

  gstr = font->getExtFontFile();
  if(gstr)
   logf("%sExternal Font file: %s\n", loglevel, gstr->getCString());

  // Get font descriptor flags.
  if(font->isFixedWidth()) logf("%sis fixed width\n", loglevel);
  if(font->isSerif()) logf("%sis serif\n", loglevel);
  if(font->isSymbolic()) logf("%sis symbolic\n", loglevel);
  if(font->isItalic()) logf("%sis italic\n", loglevel);
  if(font->isBold()) logf("%sis bold\n", loglevel);
}

//void SWFOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) {printf("void SWFOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) \n");}
//void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) {printf("void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) \n");}

SWFOutputDev::SWFOutputDev() 
{
    clippos = 0;
    clipping[clippos] = 0;
    outputstarted = 0;
//    printf("SWFOutputDev::SWFOutputDev() \n");
};

T1_OUTLINE* gfxPath_to_T1_OUTLINE(GfxState*state, GfxPath*path)
{
    int num = path->getNumSubpaths();
    int s,t;
    bezierpathsegment*start,*last;
    bezierpathsegment*outline = start = new bezierpathsegment();
    int cpos = 0;
    double lastx=0,lasty=0;
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
		outline->type = T1_PATHTYPE_MOVE;
		outline->dest.x = x;
		outline->dest.y = y;
		outline->link = (T1_OUTLINE*)new bezierpathsegment();
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
		outline->type = cpos?T1_PATHTYPE_BEZIER:T1_PATHTYPE_LINE;
		outline->link = 0;
		outline->link = (T1_OUTLINE*)new bezierpathsegment();
		outline = (bezierpathsegment*)outline->link;
		cpos = 0;
		lastx = nx;
		lasty = ny;
	   }
	}
    }
    last->link = 0;
    return (T1_OUTLINE*)start;
}
/*----------------------------------------------------------------------------
 * Primitive Graphic routines
 *----------------------------------------------------------------------------*/

void SWFOutputDev::stroke(GfxState *state) 
{
    logf("<debug> %s stroke\n",gfxstate2str(state));
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m21 = 0; m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_setdrawmode(&output, DRAWMODE_STROKE);
    swfoutput_drawpath(&output, outline, &m);
}
void SWFOutputDev::fill(GfxState *state) 
{
    logf("<debug> %s fill\n",gfxstate2str(state));
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m21 = 0; m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_setdrawmode(&output, DRAWMODE_FILL);
    swfoutput_drawpath(&output, outline, &m);
}
void SWFOutputDev::eoFill(GfxState *state) 
{
    logf("<debug> %s eofill\n",gfxstate2str(state));
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m21 = 0; m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_setdrawmode(&output, DRAWMODE_EOFILL);
    swfoutput_drawpath(&output, outline, &m);
}
void SWFOutputDev::clip(GfxState *state) 
{
    logf("<debug> %s clip\n",gfxstate2str(state));
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m21 = 0; m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_startclip(&output, outline, &m);
    clipping[clippos] = 1;
}
void SWFOutputDev::eoClip(GfxState *state) 
{
    logf("<debug> %s eoclip\n",gfxstate2str(state));
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m21 = 0; m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_startclip(&output, outline, &m);
    clipping[clippos] = 1;
}

SWFOutputDev::~SWFOutputDev() 
{
    swfoutput_destroy(&output);
    outputstarted = 0;
};
GBool SWFOutputDev::upsideDown() 
{
    logf("<debug> upsidedown?");
    return gTrue;
};
GBool SWFOutputDev::useDrawChar() 
{
    logf("<debug> usedrawchar?");
    return gTrue;
}

void SWFOutputDev::beginString(GfxState *state, GString *s) 
{ 
    double m11,m21,m12,m22;
    logf("<debug> %s beginstring \"%s\"\n", gfxstate2str(state), s->getCString());
    state->getFontTransMat(&m11, &m12, &m21, &m22);
    m11 *= state->getHorizScaling();
    m21 *= state->getHorizScaling();
    swfoutput_setfontmatrix(&output, m11, -m12, m21, -m22);
}

int charcounter = 0;
void SWFOutputDev::drawChar(GfxState *state, double x, double y, double dx, double dy, Guchar c) 
{
    logf("<debug> %s drawChar(%f,%f,%f,%f,'%c')\n",gfxstate2str(state), x,y,dx,dy,c);
    // check for invisible text -- this is used by Acrobat Capture
    if ((state->getRender() & 3) != 3)
    {
       FontEncoding*enc=state->getFont()->getEncoding();

       double x1,y1;
       x1 = x;
       y1 = y;
       state->transform(x, y, &x1, &y1);

       swfoutput_drawchar(&output, x1, y1, enc->getCharName(c));
    }
}

void SWFOutputDev::drawChar16(GfxState *state, double x, double y, double dx, double dy, int c) 
{
    printf("<error> %s drawChar16(%f,%f,%f,%f,%08x)\n",gfxstate2str(state), x,y,dx,dy,c);
    exit(1);
}

void SWFOutputDev::endString(GfxState *state) 
{ 
    logf("<debug> %s endstring\n", gfxstate2str(state));
}    

void SWFOutputDev::startPage(int pageNum, GfxState *state) 
{
  double x1,y1,x2,y2;
  logf("<debug> %s, startPage %d\n", gfxstate2str(state), pageNum);
  logf("<notice> processing page %d", pageNum);

  state->transform(state->getX1(),state->getY1(),&x1,&y1);
  state->transform(state->getX2(),state->getY2(),&x2,&y2);
  if(!outputstarted) {
    swfoutput_init(&output, swffilename, abs((int)(x2-x1)),abs((int)(y2-y1)));
    outputstarted = 1;
  }
  else
    swfoutput_newpage(&output);
}

void SWFOutputDev::drawLink(Link *link, Catalog *catalog) 
{
  logf("<debug> drawlink\n");
  double x1, y1, x2, y2, w;
  GfxRGB rgb;
  swfcoord points[5];
  int x, y;

  link->getBorder(&x1, &y1, &x2, &y2, &w);
  if (w > 0) {
    rgb.r = 0;
    rgb.g = 0;
    rgb.b = 1;
    cvtUserToDev(x1, y1, &x, &y);
    points[0].x = points[4].x = x;
    points[0].y = points[4].y = y;
    cvtUserToDev(x2, y1, &x, &y);
    points[1].x = x;
    points[1].y = y;
    cvtUserToDev(x2, y2, &x, &y);
    points[2].x = x;
    points[2].y = y;
    cvtUserToDev(x1, y2, &x, &y);
    points[3].x = x;
    points[3].y = y;
    //PDF: draw rect
    LinkAction*action=link->getAction();
    char*s;
    switch(action->getKind())
    {
	case actionGoTo: {
	    LinkGoTo*l = (LinkGoTo*)action;
	    s = l->getNamedDest()->getCString();
	}
        break;
	case actionGoToR: {
	    LinkGoToR*l = (LinkGoToR*)action;
	    s = l->getNamedDest()->getCString();
	}
        break;
	case actionLaunch: {
	    LinkLaunch*l = (LinkLaunch*)action;
	    GString * str = new GString(l->getFileName());
	    str->append(l->getParams());
	    s = str->getCString();
	}
        break;
	case actionURI: {
	    LinkURI*l = (LinkURI*)action;
	    s = l->getURI()->getCString();
	}
        break;
	case actionNamed: {
	    LinkNamed*l = (LinkNamed*)action;
	    s = l->getName()->getCString();
	}
        break;
	case actionUnknown: {
	    LinkUnknown*l = (LinkUnknown*)action;
	    s = "";
	}
        break;
    }
    logf("<verbose> link to \"%s\"\n", s);
  }
}

void SWFOutputDev::saveState(GfxState *state) {
  logf("<debug> %s saveState\n", gfxstate2str(state));
  updateAll(state);
  clippos ++;
  clipping[clippos] = 0;
};

void SWFOutputDev::restoreState(GfxState *state) {
  logf("<debug> %s restoreState\n", gfxstate2str(state));
  updateAll(state);
  if(clipping[clippos])
      swfoutput_endclip(&output);
  clippos--;
}

char type3Warning=0;

int SWFOutputDev::setT1Font(char*name, FontEncoding*encoding) 
{	
    int i;
    
    int id=-1;
    int mapid=-1;
    char*filename=0;
    for(i=0;i<sizeof(pdf2t1map)/sizeof(mapping);i++) 
    {
	if(!strcmp(name, pdf2t1map[i].pdffont))
	{
	    filename = pdf2t1map[i].filename;
	    mapid = i;
	}
    }
    if(filename)
    for(i=0; i<T1_Get_no_fonts(); i++)
    {
	char*fontfilename = T1_GetFontFileName (i);
	if(strstr(fontfilename, filename))
	{
		id = i;
		pdf2t1map[i].id = mapid;
	}
    }
    if(id<0)
     return 0;

    this->t1id = id;
}

void SWFOutputDev::updateLineWidth(GfxState *state)
{
    double width = state->getLineWidth();
    swfoutput_setlinewidth(&output, width);
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

char*writeEmbeddedFontToFile(GfxFont*font)
{
      char*tmpFileName = NULL;
      char*fileName = NULL;
      FILE *f;
      int c;
      char *fontBuf;
      int fontLen;
      Type1CFontConverter *cvt;
      Ref embRef;
      Object refObj, strObj;
      tmpFileName = "tmpfont";
      font->getEmbeddedFontID(&embRef);

      f = fopen(tmpFileName, "wb");
      if (!f) {
	logf("<error> Couldn't create temporary Type 1 font file");
	return 0;
      }
      if (font->getType() == fontType1C) {
	if (!(fontBuf = font->readEmbFontFile(&fontLen))) {
	  fclose(f);
	  logf("<error> Couldn't read embedded font file");
	  return 0;
	}
	cvt = new Type1CFontConverter(fontBuf, fontLen, f);
	cvt->convert();
	delete cvt;
	gfree(fontBuf);
      } else {
	font->getEmbeddedFontID(&embRef);
	refObj.initRef(embRef.num, embRef.gen);
	refObj.fetch(&strObj);
	refObj.free();
	strObj.streamReset();
	while ((c = strObj.streamGetChar()) != EOF) {
	  fputc(c, f);
	}
	strObj.streamClose();
	strObj.free();
      }
      fclose(f);
      fileName = tmpFileName;
      if(!fileName) {
	  logf("<error> Embedded font writer didn't create a file");
	  return 0;
      }
      return fileName;
}

char* gfxFontName(GfxFont* gfxFont)
{
      GString *gstr;
      gstr = gfxFont->getName();
      if(gstr) {
	  return gstr->getCString();
      }
      else {
	  char buf[32];
	  Ref r=gfxFont->getID();
	  sprintf(buf, "UFONT%d", r.num);
	  return strdup(buf);
      }
}

void SWFOutputDev::updateFont(GfxState *state) 
{
  double m11, m12, m21, m22;
  char * fontname = 0;
  GfxFont*gfxFont = state->getFont();
  char * fileName = 0;

  if (!gfxFont) {
    return;
  }  

  if(swfoutput_queryfont(&output, gfxFontName(gfxFont)))
  {
      swfoutput_setfont(&output, gfxFontName(gfxFont), -1, 0);
      return;
  }

  // look for Type 3 font
  if (!type3Warning && gfxFont->getType() == fontType3) {
    type3Warning = gTrue;
    showFontError(gfxFont, 2);
  }
  //dumpFontInfo ("<notice>", gfxFont);

  Ref embRef;
  GBool embedded = gfxFont->getEmbeddedFontID(&embRef);
  if(embedded) {
    if (!gfxFont->is16Bit() &&
	(gfxFont->getType() == fontType1 ||
	 gfxFont->getType() == fontType1C)) {
	
	fileName = writeEmbeddedFontToFile(gfxFont);
	if(!fileName)
	  return ;
    }
    else {
	showFontError(gfxFont,0);
	return ;
    }
    
    t1id = T1_AddFont(fileName);
  } else {
    fontname = NULL;
    if(gfxFont->getName()) {
      fontname = gfxFont->getName()->getCString();
      //logf("<notice> Processing font %s", fontname);
    }
    if(!fontname || !setT1Font(state->getFont()->getName()->getCString(), gfxFont->getEncoding()))
    { //substitute font
      int index;
      int code;
      double w,w1,w2;
      double*fm;
      double v;
      showFontError(gfxFont, 1);
      if (!gfxFont->is16Bit()) {
	if (gfxFont->isFixedWidth()) {
	  index = 8;
	} else if (gfxFont->isSerif()) {
	  index = 4;
	} else {
	  index = 0;
	}
	if (gfxFont->isBold())
	  index += 2;
	if (gfxFont->isItalic())
	  index += 1;
	fontname = fontnames[index];
	// get width of 'm' in real font and substituted font
	if ((code = gfxFont->getCharCode("m")) >= 0)
	  w1 = gfxFont->getWidth(code);
	else
	  w1 = 0;
	w2 = fontsizes[index];
	if (gfxFont->getType() == fontType3) {
	  // This is a hack which makes it possible to substitute for some
	  // Type 3 fonts.  The problem is that it's impossible to know what
	  // the base coordinate system used in the font is without actually
	  // rendering the font.  This code tries to guess by looking at the
	  // width of the character 'm' (which breaks if the font is a
	  // subset that doesn't contain 'm').
	  if (w1 > 0 && (w1 > 1.1 * w2 || w1 < 0.9 * w2)) {
	    w1 /= w2;
	    m11 *= w1;
	    m12 *= w1;
	    m21 *= w1;
	    m22 *= w1;
	  }
	  fm = gfxFont->getFontMatrix();
	  v = (fm[0] == 0) ? 1 : (fm[3] / fm[0]);
	  m21 *= v;
	  m22 *= v;
	} else if (!gfxFont->isSymbolic()) {
	  // if real font is substantially narrower than substituted
	  // font, reduce the font size accordingly
	  if (w1 > 0.01 && w1 < 0.9 * w2) {
	    w1 /= w2;
	    if (w1 < 0.8) {
	      w1 = 0.8;
	    }
	    m11 *= w1;
	    m12 *= w1;
	    m21 *= w1;
	    m22 *= w1;
	  }
	}
      }
      if(fontname)
        setT1Font(fontname, gfxFont->getEncoding());
    }
  }

  swfoutput_setfont(&output,gfxFontName(gfxFont),t1id, fileName);
  if(fileName)
      unlink(fileName);
}

void SWFOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool inlineImg) {
  FILE *fi;
  int c;
  char fileName[128];
  double x1,y1,x2,y2,x3,y3,x4,y4;
  state->transform(0, 1, &x1, &y1);
  state->transform(0, 0, &x2, &y2);
  state->transform(1, 0, &x3, &y3);
  state->transform(1, 1, &x4, &y4);

  if (str->getKind() == strDCT) {
    sprintf(fileName, "/tmp/tmp%08x.jpg",lrand48());
    logf("<notice> Found picture. Temporary storage is %s", fileName);
    if (!(fi = fopen(fileName, "wb"))) {
      logf("<error> Couldn't open temporary image file '%s'", fileName);
      return;
    }
    str = ((DCTStream *)str)->getRawStream();
    str->reset();
    while ((c = str->getChar()) != EOF)
      fputc(c, fi);
    fclose(fi);
    swfoutput_drawimagefile(&output, fileName, width, height, x1,y1,x2,y2,x3,y3,x4,y4);
    unlink(fileName);
  } else {
      logf("<notice> File contains pbm pictures.");
  }
}

void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap, GBool inlineImg) {
  FILE *fi;
  int c;
  char fileName[128];
  double x1,y1,x2,y2,x3,y3,x4,y4;
  state->transform(0, 1, &x1, &y1);
  state->transform(0, 0, &x2, &y2);
  state->transform(1, 0, &x3, &y3);
  state->transform(1, 1, &x4, &y4);

  if (str->getKind() == strDCT &&
      colorMap->getNumPixelComps() == 3) {
    sprintf(fileName, "/tmp/tmp%08x.jpg", lrand48());
    logf("<notice> Found picture. Temporary storage is %s", fileName);
    if (!(fi = fopen(fileName, "wb"))) {
      error(-1, "Couldn't open temporary image file '%s'", fileName);
      return;
    }
    str = ((DCTStream *)str)->getRawStream();
    str->reset();
    while ((c = str->getChar()) != EOF)
      fputc(c, fi);
    fclose(fi);
    swfoutput_drawimagefile(&output, fileName, width, height, x1,y1,x2,y2,x3,y3,x4,y4);
    unlink(fileName);
  } else {
      logf("<notice> File contains pbm pictures.");
  }
}

PDFDoc*doc = 0;
SWFOutputDev*output = 0; 

void pdfswf_init(char*filename, char*userPassword) 
{
  GString *fileName = new GString(filename);
  GString *userPW;
  Object info;
  // init error file
  errorInit();

  // read config file
  initParams(xpdfConfigFile);

  // open PDF file
  xref = NULL;
  if (userPassword && userPassword[0]) {
    userPW = new GString(userPassword);
  } else {
    userPW = NULL;
  }
  doc = new PDFDoc(fileName, userPW);
  if (userPW) {
    delete userPW;
  }
  if (!doc->isOk()) {
    exit(1);
  }

  // print doc info
  doc->getDocInfo(&info);
  if (info.isDict()) {
    printInfoString(info.getDict(), "Title",        "Title:        %s\n");
    printInfoString(info.getDict(), "Subject",      "Subject:      %s\n");
    printInfoString(info.getDict(), "Keywords",     "Keywords:     %s\n");
    printInfoString(info.getDict(), "Author",       "Author:       %s\n");
    printInfoString(info.getDict(), "Creator",      "Creator:      %s\n");
    printInfoString(info.getDict(), "Producer",     "Producer:     %s\n");
    printInfoDate(info.getDict(),   "CreationDate", "CreationDate: %s\n");
    printInfoDate(info.getDict(),   "ModDate",      "ModDate:      %s\n");
  }
  info.free();

  // print page count
  printf("Pages:        %d\n", doc->getNumPages());
  
  // print linearization info
  printf("Linearized:   %s\n", doc->isLinearized() ? "yes" : "no");

  // print encryption info
  printf("Encrypted:    ");
  if (doc->isEncrypted()) {
    printf("yes (print:%s copy:%s change:%s addNotes:%s)\n",
	   doc->okToPrint() ? "yes" : "no",
	   doc->okToCopy() ? "yes" : "no",
	   doc->okToChange() ? "yes" : "no",
	   doc->okToAddNotes() ? "yes" : "no");
	/*ERROR: This pdf is encrypted, and disallows copying.
	  Due to the DMCA, paragraph 1201, (2) A-C, circumventing
	  a technological measure that efficively controls access to
	  a protected work is violating American law. 
	  See www.eff.org for more information about DMCA issues.
	 */
	if(!doc->okToCopy()) {
	    printf("PDF disallows copying. Bailing out.\n");
	    exit(1); //bail out
	}
	if(!doc->okToChange() || !doc->okToAddNotes())
	    swfoutput_setprotected();
    }
  else {
    printf("no\n");
  }


  output = new SWFOutputDev();
}

void pdfswf_setoutputfilename(char*_filename)
{
    swffilename = _filename;
}

void pdfswf_convertpage(int page)
{
    doc->displayPage((OutputDev*)output, page, /*zoom*/100, /*rotate*/0, /*doLinks*/(int)1);
}

int pdfswf_numpages()
{
  return doc->getNumPages();
}

void pdfswf_close()
{
    delete doc;
    delete output;
    
    freeParams();
    // check for memory leaks
    Object::memCheck(stderr);
    gMemReport(stderr);
}

