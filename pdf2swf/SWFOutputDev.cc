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
#include "FontFile.h"
#include "GlobalParams.h"
//swftools header files
#include "swfoutput.h"
extern "C" {
#include "../lib/log.h"
#include "ttf2pt1.h"
}

#define logf logarithmf // logf is also used by ../lib/log.h
#include <math.h>
#undef logf

static PDFDoc*doc = 0;
static char* swffilename = 0;
int numpages;
int currentpage;

// swf <-> pdf pages
int*pages = 0;
int pagebuflen = 0;
int pagepos = 0;

static void printInfoString(Dict *infoDict, char *key, char *fmt);
static void printInfoDate(Dict *infoDict, char *key, char *fmt);

double fontsizes[] = 
{
 0.833,0.833,0.889,0.889,
 0.788,0.722,0.833,0.778,
 0.600,0.600,0.600,0.600,
 0.576,0.576,0.576,0.576,
 0.733 //?
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
  
  virtual GBool interpretType3Chars() {return gTrue;}

  //----- initialization and control

  void startDoc(XRef *xref);

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

  XRef*xref;

  int searchT1Font(char*name);
  char* substituteFont(GfxFont*gfxFont, char*oldname);
  char* writeEmbeddedFontToFile(XRef*ref, GfxFont*font);
  int t1id;
  int jpeginfo; // did we write "File contains jpegs" yet?
  int pbminfo; // did we write "File contains jpegs" yet?
  int linkinfo; // did we write "File contains links" yet?
  int ttfinfo; // did we write "File contains TrueType Fonts" yet?

  int type3active; // are we between beginType3()/endType3()?

  GfxState *laststate;
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
      logf("<warning> The following font caused problems:");
    else if(nr == 1)
      logf("<warning> The following font caused problems (substituting):");
    else if(nr == 2)
      logf("<warning> The following Type 3 Font will be rendered as bitmap:");
    dumpFontInfo("<warning>", font);
}

void dumpFontInfo(char*loglevel, GfxFont*font)
{
  GString *gstr;
  char*name = 0;
  gstr = font->getName();
  Ref* r=font->getID();
  logf("%s=========== %s (ID:%d,%d) ==========\n", loglevel, gstr?FIXNULL(gstr->getCString()):"(unknown font)", r->num,r->gen);

  gstr  = font->getTag();
  if(gstr) 
   logf("%sTag: %s\n", loglevel, FIXNULL(gstr->getCString()));
  
  if(font->isCIDFont()) logf("%sis CID font\n", loglevel);

  GfxFontType type=font->getType();
  switch(type) {
    case fontUnknownType:
     logf("%sType: unknown\n",loglevel);
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
    case fontCIDType0:
     logf("%sType: CIDType0\n",loglevel);
    break;
    case fontCIDType0C:
     logf("%sType: CIDType0C\n",loglevel);
    break;
    case fontCIDType2:
     logf("%sType: CIDType2\n",loglevel);
    break;
  }
  
  Ref embRef;
  GBool embedded = font->getEmbeddedFontID(&embRef);
  if(font->getEmbeddedFontName())
    name = font->getEmbeddedFontName()->getCString();
  if(embedded)
   logf("%sEmbedded name: %s id: %d\n",loglevel, FIXNULL(name), embRef.num);

  gstr = font->getExtFontFile();
  if(gstr)
   logf("%sExternal Font file: %s\n", loglevel, FIXNULL(gstr->getCString()));

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
    jpeginfo = 0;
    ttfinfo = 0;
    linkinfo = 0;
    pbminfo = 0;
    type3active = 0;
    clippos = 0;
    clipping[clippos] = 0;
    outputstarted = 0;
    xref = 0;
//    printf("SWFOutputDev::SWFOutputDev() \n");
};

T1_OUTLINE* gfxPath_to_T1_OUTLINE(GfxState*state, GfxPath*path)
{
    int num = path->getNumSubpaths();
    int s,t;
    bezierpathsegment*start,*last=0;
    bezierpathsegment*outline = start = new bezierpathsegment();
    int cpos = 0;
    double lastx=0,lasty=0;
    if(!num) {
	logf("<warning> empty path");
	outline->type = T1_PATHTYPE_MOVE;
	outline->dest.x = 0;
	outline->dest.y = 0;
	outline->link = 0;
	return (T1_OUTLINE*)outline;
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
    logf("<debug> stroke\n");
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m12 = 0; m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_setdrawmode(&output, DRAWMODE_STROKE);
    swfoutput_drawpath(&output, outline, &m);
}
void SWFOutputDev::fill(GfxState *state) 
{
    logf("<debug> fill\n");
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m12 = 0; m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_setdrawmode(&output, DRAWMODE_FILL);
    swfoutput_drawpath(&output, outline, &m);
}
void SWFOutputDev::eoFill(GfxState *state) 
{
    logf("<debug> eofill\n");
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m12 = 0; m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_setdrawmode(&output, DRAWMODE_EOFILL);
    swfoutput_drawpath(&output, outline, &m);
}
void SWFOutputDev::clip(GfxState *state) 
{
    logf("<debug> clip\n");
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m22 = 1;
    m.m12 = 0; m.m21 = 0; 
    m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_startclip(&output, outline, &m);
    clipping[clippos] ++;
}
void SWFOutputDev::eoClip(GfxState *state) 
{
    logf("<debug> eoclip\n");
    GfxPath * path = state->getPath();
    struct swfmatrix m;
    m.m11 = 1; m.m21 = 0; m.m22 = 1;
    m.m12 = 0; m.m13 = 0; m.m23 = 0;
    T1_OUTLINE*outline = gfxPath_to_T1_OUTLINE(state, path);
    swfoutput_startclip(&output, outline, &m);
    clipping[clippos] ++;
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
//    logf("<debug> %s beginstring \"%s\"\n", gfxstate2str(state), s->getCString());
    state->getFontTransMat(&m11, &m12, &m21, &m22);
    m11 *= state->getHorizScaling();
    m21 *= state->getHorizScaling();
    swfoutput_setfontmatrix(&output, m11, -m21, m12, -m22);
}

int charcounter = 0;
void SWFOutputDev::drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode c, Unicode *_u, int uLen)
{
    logf("<debug> drawChar(%f,%f,%f,%f,'%c')\n",x,y,dx,dy,c);
    
    // check for invisible text -- this is used by Acrobat Capture
    if ((state->getRender() & 3) == 3)
	return;

    GfxFont*font = state->getFont();

    if(font->getType() == fontType3) {
	/* type 3 chars are passed primarily as graphics */
	return;
    }
    double x1,y1;
    x1 = x;
    y1 = y;
    state->transform(x, y, &x1, &y1);

    if(font->isCIDFont()) {
	GfxCIDFont*cfont = (GfxCIDFont*)font;
	Unicode u=0;
	char*name=0;
	if(_u) 
	    u = *_u;
	if(u) {
	    int t;
	    for(t=0;t<sizeof(nameToUnicodeTab)/sizeof(nameToUnicodeTab[0]);t++)
		/* todo: should be precomputed */
		if(nameToUnicodeTab[t].u == u) {
		    name = nameToUnicodeTab[t].name;
		    break;
		}
	}
/*	   printf("%02x %04x/%04x-%d \"%s\" %s %d\n", c,u, *_u, uLen, name, cfont->getName()->getCString(),
		cfont->getType());*/

	if(name)
	   swfoutput_drawchar(&output, x1, y1, name, c);
	else
	   logf("<warning> couldn't get name for CID character %02x from Encoding", c);
    } else {
	Gfx8BitFont*font8;
	font8 = (Gfx8BitFont*)font;
	char**enc=font8->getEncoding();

	if(enc && enc[c])
	   swfoutput_drawchar(&output, x1, y1, enc[c], c);
	else {
	   logf("<warning> couldn't get name for character %02x from Encoding", c);
	}
    }
}

void SWFOutputDev::endString(GfxState *state) 
{ 
    logf("<debug> endstring\n");
}    

 
GBool SWFOutputDev::beginType3Char(GfxState *state,
			       CharCode code, Unicode *u, int uLen)
{
    logf("<debug> beginType3Char %d, %08x, %d", code, *u, uLen);
    type3active = 1;
    /* the character itself is going to be passed using
       drawImageMask() */
    return gFalse; /* gTrue= is_in_cache? */
}

void SWFOutputDev::endType3Char(GfxState *state)
{
    type3active = 0;
    logf("<debug> endType3Char");
}

void SWFOutputDev::startPage(int pageNum, GfxState *state) 
{
  double x1,y1,x2,y2;
  laststate = state;
  logf("<debug> startPage %d\n", pageNum);
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
//  if (w > 0) 
  {
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
    char*s = "-?-";
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
	      s = buf;
	    }
	}
        break;
	case actionGoToR: {
	    type = "GoToR";
	    LinkGoToR*l = (LinkGoToR*)action;
	    GString*g = l->getNamedDest();
	    if(g)
	     s = g->getCString();
	}
        break;
	case actionNamed: {
	    type = "Named";
	    LinkNamed*l = (LinkNamed*)action;
	    GString*name = l->getName();
	    if(name) {
		s = name->lowerCase()->getCString();
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
			page = pages[pagepos-1]; //:)
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
	    s = str->getCString();
	}
        break;
	case actionURI: {
	    type = "URI";
	    LinkURI*l = (LinkURI*)action;
	    GString*g = l->getURI();
	    if(g) {
	     url = g->getCString();
	     s = url;
	    }
	}
        break;
	case actionUnknown: {
	    type = "Unknown";
	    LinkUnknown*l = (LinkUnknown*)action;
	    s = "";
	}
        break;
	default: {
	    logf("<error> Unknown link type!\n");
	    break;
	}
    }
    if(!linkinfo && (page || url))
    {
	logf("<notice> File contains links");
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
    logf("<verbose> \"%s\" link to \"%s\" (%d)\n", type, FIXNULL(s), page);
  }
}

void SWFOutputDev::saveState(GfxState *state) {
  logf("<debug> saveState\n");
  updateAll(state);
  if(clippos<64)
    clippos ++;
  else
    logf("<error> Too many nested states in pdf.");
  clipping[clippos] = 0;
};

void SWFOutputDev::restoreState(GfxState *state) {
  logf("<debug> restoreState\n");
  updateAll(state);
  while(clipping[clippos]) {
      swfoutput_endclip(&output);
      clipping[clippos]--;
  }
  clippos--;
}

char type3Warning=0;

int SWFOutputDev::searchT1Font(char*name) 
{	
    int i;
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
    if(filename) {
	for(i=0; i<T1_Get_no_fonts(); i++)
	{
	    char*fontfilename = T1_GetFontFileName (i);
	    if(strstr(fontfilename, filename))
	    {
		    pdf2t1map[i].id = mapid;
		    return i;
	    }
	}
    } else {
	for(i=0; i<T1_Get_no_fonts(); i++)
	{
	    char*fontname = T1_GetFontName (i);
	    if(!fontname) {
		T1_LoadFont(i);
		fontname = T1_GetFontName (i);
		logf("<verbose> Loading extra font %s from %s\n", FIXNULL(fontname), 
								  FIXNULL(T1_GetFontFileName(i)));
	    }

	    if(fontname && !strcmp(name, fontname)) {
		logf("<notice> Extra font %s is being used.\n", fontname);
		return i;
	    }
	    fontname = T1_GetFontFileName(i);
	    if(strrchr(fontname,'/'))
		    fontname = strrchr(fontname,'/')+1;
 
	    if(strstr(fontname, name)) {
		logf("<notice> Extra font %s is being used.\n", fontname);
		return i;
	    }
	}
    }
    return -1;
}

void SWFOutputDev::updateLineWidth(GfxState *state)
{
    double width = state->getTransformedLineWidth();
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

char*SWFOutputDev::writeEmbeddedFontToFile(XRef*ref, GfxFont*font)
{
      char*tmpFileName = NULL;
      FILE *f;
      int c;
      char *fontBuf;
      int fontLen;
      Type1CFontFile *cvt;
      Ref embRef;
      Object refObj, strObj;
      tmpFileName = "/tmp/tmpfont";
      int ret;

      ret = font->getEmbeddedFontID(&embRef);
      if(!ret) {
	  logf("<verbose> Didn't get embedded font id");
	  /* not embedded- the caller should now search the font
	     directories for this font */
	  return 0;
      }

      f = fopen(tmpFileName, "wb");
      if (!f) {
	logf("<error> Couldn't create temporary Type 1 font file");
	  return 0;
      }
      if (font->getType() == fontType1C) {
	if (!(fontBuf = font->readEmbFontFile(xref, &fontLen))) {
	  fclose(f);
	  logf("<error> Couldn't read embedded font file");
	  return 0;
	}
	cvt = new Type1CFontFile(fontBuf, fontLen);
	cvt->convertToType1(f);
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

      if(font->getType() == fontTrueType ||
	 font->getType() == fontCIDType2)
      {
	  if(!ttfinfo) {
	      logf("<notice> File contains TrueType fonts");
	      ttfinfo = 1;
	  }
	  char name2[80];
	  char*tmp;
	  tmp = strdup(mktmpname((char*)name2));
	  sprintf(name2, "%s", tmp);
	  char*a[] = {"./ttf2pt1","-pttf","-b", tmpFileName, name2};
	  logf("<verbose> Invoking ttf2pt1...");
	  ttf2pt1_main(5,a);
	  unlink(tmpFileName);
	  sprintf(name2,"%s.pfb",tmp);
	  tmpFileName = strdup(name2);
      }

    return tmpFileName;
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
	  Ref*r=gfxFont->getID();
	  sprintf(buf, "UFONT%d", r->num);
	  return strdup(buf);
      }
}

char* substitutetarget[256];
char* substitutesource[256];
int substitutepos = 0;

char* SWFOutputDev::substituteFont(GfxFont*gfxFont, char* oldname)
{
/* ------------------------------ V1 */

    char*fontname = "Times-Roman";
    this->t1id = searchT1Font(fontname);
    if(substitutepos>=sizeof(substitutesource)/sizeof(char*)) {
	logf("<fatal> Too many fonts in file.");
	exit(1);
    }
    if(oldname) {
	substitutesource[substitutepos] = oldname;
	substitutetarget[substitutepos] = fontname;
	logf("<verbose> substituting %s -> %s", FIXNULL(oldname), FIXNULL(fontname));
	substitutepos ++;
    }
    return fontname;

/* ------------------------------ V2 */

/*      //substitute font
      char* fontname = 0;
      double m11, m12, m21, m22;
      int index;
      int code;
      double w,w1,w2;
      double*fm;
      double v;
      if(gfxFont->getName()) {
	fontname = gfxFont->getName()->getCString();
      }

//	  printf("%d %s\n", t, gfxFont->getCharName(t));
      showFontError(gfxFont, 1);
      if(1) { //if (!gfxFont->isCIDFont()) { FIXME: xpdf 1.01 does not have is16Bit()
	if(gfxFont->isSymbolic()) {
	  if(fontname && (strstr(fontname,"ing"))) //Dingbats, Wingdings etc.
	   index = 16;
	  else 
	   index = 12;
        } else if (gfxFont->isFixedWidth()) {
	  index = 8;
	} else if (gfxFont->isSerif()) {
	  index = 4;
	} else {
	  index = 0;
	}
	if (gfxFont->isBold() && index!=16)
	  index += 2;
	if (gfxFont->isItalic() && index!=16)
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
      if(fontname) {
        this->t1id = searchT1Font(fontname);
      }
      if(substitutepos>=sizeof(substitutesource)/sizeof(char*)) {
	  logf("<fatal> Too many fonts in file.");
	  exit(1);
      }
      if(oldname) {
	  substitutesource[substitutepos] = oldname;
	  substitutetarget[substitutepos] = fontname;
	  logf("<verbose> substituting %s -> %s", FIXNULL(oldname), FIXNULL(fontname));
	  substitutepos ++;
      }
      return fontname;*/
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

void SWFOutputDev::startDoc(XRef *xref) 
{
  this->xref = xref;
}


void SWFOutputDev::updateFont(GfxState *state) 
{
  GfxFont*gfxFont = state->getFont();
  char * fileName = 0;
    
  if (!gfxFont) {
    return;
  }  
  char * fontname = gfxFontName(gfxFont);
 
  int t;
  /* first, look if we substituted this font before-
     this way, we don't initialize the T1 Fonts
     too often */
  for(t=0;t<substitutepos;t++) {
      if(!strcmp(fontname, substitutesource[t])) {
	  fontname = substitutetarget[t];
	  break;
      }
  }

  /* second, see if swfoutput already has this font
     cached- if so, we are done */

  if(swfoutput_queryfont(&output, fontname))
  {
      swfoutput_setfont(&output, fontname, -1, 0);
      return;
  }

  // look for Type 3 font
  if (!type3Warning && gfxFont->getType() == fontType3) {
    type3Warning = gTrue;
    showFontError(gfxFont, 2);
  }

  /* now either load the font, or find a substitution */

  Ref embRef;
  GBool embedded = gfxFont->getEmbeddedFontID(&embRef);
  if(embedded) {
    if (gfxFont->getType() == fontType1 ||
	gfxFont->getType() == fontType1C ||
	gfxFont->getType() == fontTrueType ||
	gfxFont->getType() == fontCIDType2) 
    {
	fileName = writeEmbeddedFontToFile(xref, gfxFont);
	if(!fileName) {
	  logf("<error> Couldn't write font to file");
	  showFontError(gfxFont,0);
	  return ;
	}
	this->t1id = T1_AddFont(fileName);
	if(this->t1id<0) {
	  logf("<error> Couldn't load font from file");
	  showFontError(gfxFont,0);
	  unlinkfont(fileName);
	  return ;
	}
    }
    else {
	/* in case the font is embedded, but has an
	   unsupported format, we just look through the
	   font directories */
	int newt1id = searchT1Font(fontname);
	if(newt1id<0) {
	    showFontError(gfxFont,0);
	    fontname = substituteFont(gfxFont, fontname);
	} else
	    this->t1id = newt1id;
    }
  } else {
    if(fontname) {
	int newt1id = searchT1Font(fontname);
	if(newt1id<0) {
	    fontname = substituteFont(gfxFont, fontname);
	} else
	    this->t1id = newt1id;
    }
    else
	fontname = substituteFont(gfxFont, fontname);
  }

  if(t1id<0) {
      showFontError(gfxFont,0);
      return;
  }
 
  /* we may have done some substitutions here, so check
     again if this font is cached. */
  if(swfoutput_queryfont(&output, fontname))
  {
      swfoutput_setfont(&output, fontname, -1, 0);
      return;
  }

  logf("<verbose> Creating new SWF font: t1id: %d, filename: %s name:%s", this->t1id, FIXNULL(fileName), FIXNULL(fontname));
  swfoutput_setfont(&output, fontname, this->t1id, fileName);
  if(fileName)
      unlinkfont(fileName);
}

int pic_xids[1024];
int pic_yids[1024];
int pic_ids[1024];
int pic_width[1024];
int pic_height[1024];
int picpos = 0;
int pic_id = 0;

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
      logf("<verbose> Ignoring %d by %d image", width, height);
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
	  logf("<notice> file contains pbm pictures %s",mask?"(masked)":"");
	  pbminfo = 1;
      }
      if(mask)
      logf("<verbose> drawing %d by %d masked picture\n", width, height);
  }
  if(!jpeginfo && (str->getKind()==strDCT)) {
      logf("<notice> file contains jpeg pictures");
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
  logf("<verbose> drawImageMask %dx%d, invert=%d inline=%d", width, height, invert, inlineImg);
  drawGeneralImage(state,ref,str,width,height,0,invert,inlineImg,1);
}

void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg)
{
  logf("<verbose> drawImage %dx%d, %s %s, inline=%d", width, height, 
	  colorMap?"colorMap":"no colorMap", 
	  maskColors?"maskColors":"no maskColors",
	  inlineImg);
  if(colorMap)
      logf("<verbose> colorMap pixcomps:%d bits:%d mode:%d\n", colorMap->getNumPixelComps(),
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

void pdfswf_init(char*filename, char*userPassword) 
{
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
  doc = new PDFDoc(fileName, userPW);
  if (userPW) {
    delete userPW;
  }
  if (!doc->isOk()) {
    exit(1);
  }

  // print doc info
  doc->getDocInfo(&info);
  if (info.isDict() &&
    (screenloglevel>=LOGLEVEL_NOTICE)) {
    printInfoString(info.getDict(), "Title",        "Title:        %s\n");
    printInfoString(info.getDict(), "Subject",      "Subject:      %s\n");
    printInfoString(info.getDict(), "Keywords",     "Keywords:     %s\n");
    printInfoString(info.getDict(), "Author",       "Author:       %s\n");
    printInfoString(info.getDict(), "Creator",      "Creator:      %s\n");
    printInfoString(info.getDict(), "Producer",     "Producer:     %s\n");
    printInfoDate(info.getDict(),   "CreationDate", "CreationDate: %s\n");
    printInfoDate(info.getDict(),   "ModDate",      "ModDate:      %s\n");
    printf("Pages:        %d\n", doc->getNumPages());
    printf("Linearized:   %s\n", doc->isLinearized() ? "yes" : "no");
    printf("Encrypted:    ");
    if (doc->isEncrypted()) {
      printf("yes (print:%s copy:%s change:%s addNotes:%s)\n",
	     doc->okToPrint() ? "yes" : "no",
	     doc->okToCopy() ? "yes" : "no",
	     doc->okToChange() ? "yes" : "no",
	     doc->okToAddNotes() ? "yes" : "no");
    } else {
      printf("no\n");
    }
  }
  info.free();

  numpages = doc->getNumPages();
  if (doc->isEncrypted()) {
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

  output = new SWFOutputDev();
  output->startDoc(doc->getXRef());
}

void pdfswf_drawonlyshapes()
{
    drawonlyshapes = 1;
}

void pdfswf_ignoredraworder()
{
    ignoredraworder = 1;
}

void pdfswf_linksopennewwindow()
{
    opennewwindow = 1;
}

void pdfswf_storeallcharacters()
{
    storeallcharacters = 1;
}

void pdfswf_enablezlib()
{
    enablezlib = 1;
}

void pdfswf_jpegquality(int val)
{
    if(val<0) val=0;
    if(val>100) val=100;
    jpegquality = val;
}

void pdfswf_setoutputfilename(char*_filename)
{
    swffilename = _filename;
}


void pdfswf_convertpage(int page)
{
    if(!pages)
    {
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

void pdfswf_performconversion()
{
    int t;
    for(t=0;t<pagepos;t++)
    {
       currentpage = pages[t];
       doc->displayPage((OutputDev*)output, currentpage, /*dpi*/72, /*rotate*/0, /*doLinks*/(int)1);
    }
}

int pdfswf_numpages()
{
  return doc->getNumPages();
}

void pdfswf_insertstop()
{
    insertstoptag = 1;
}

void pdfswf_setversion(int n)
{
    flashversion = n;
}

int closed=0;
void pdfswf_close()
{
    logf("<debug> pdfswf.cc: pdfswf_close()");
    delete output;
    delete doc;
    //freeParams();
    // check for memory leaks
    Object::memCheck(stderr);
    gMemReport(stderr);
}


