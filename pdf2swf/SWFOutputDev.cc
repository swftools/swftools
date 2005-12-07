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
#ifdef HAVE_FONTCONFIG
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
#include "Link.h"
#include "OutputDev.h"
#include "GfxState.h"
#include "GfxFont.h"
#include "CharCodeToUnicode.h"
#include "NameToUnicodeTable.h"
#include "GlobalParams.h"
#include "FoFiType1C.h"
#include "FoFiTrueType.h"
#include "GHash.h"
#include "SWFOutputDev.h"

//swftools header files
#include "../lib/devices/swf.h"
#include "../lib/log.h"
#include "../lib/gfxdevice.h"
#include "../lib/gfxtools.h"
#include "../lib/gfxfont.h"

#include <math.h>

typedef struct _fontfile
{
    char*filename;
    int used;
} fontfile_t;

typedef struct _parameter
{
    char*name;
    char*value;
    struct _parameter*next;
} parameter_t;

static parameter_t* device_config = 0;
static parameter_t* device_config_next = 0;

// for pdfswf_addfont
static fontfile_t fonts[2048];
static int fontnum = 0;

static int config_use_fontconfig = 1;

/* config */
static double caplinewidth = 3.0;
static double zoom = 72; /* xpdf: 86 */
static int forceType0Fonts = 1;

static void printInfoString(Dict *infoDict, char *key, char *fmt);
static void printInfoDate(Dict *infoDict, char *key, char *fmt);

static char* lastfontdir = 0;

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

class SWFOutputState {
    public:
    int clipping;
    int textRender;
    SWFOutputState() {
	this->clipping = 0;
	this->textRender = 0;
    }
};

typedef struct _fontlist
{
    char*id;
    char*filename;
    gfxfont_t*font;
    _fontlist*next;
} fontlist_t;

class InfoOutputDev;

class SWFOutputDev:  public OutputDev {
public:
  gfxdevice_t* output;

  // Constructor.
  SWFOutputDev();

  // Destructor.
  virtual ~SWFOutputDev() ;

  void setMove(int x,int y);
  void setClip(int x1,int y1,int x2,int y2);

  void setInfo(InfoOutputDev*info) {this->info = info;}
  
  int save(char*filename);
    
  // Start a page.
  void startFrame(int width, int height);

  virtual void startPage(int pageNum, GfxState *state, double x1, double y1, double x2, double y2) ;

  void endframe();
  void* get(char*name);

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
  virtual void endTextObject(GfxState *state);
  virtual void drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode code, int nBytes, Unicode *u, int uLen);

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);
  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg);
  virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       Stream *maskStr, int maskWidth, int maskHeight,
			       GBool maskInvert);
  virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap);
  
  virtual GBool beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen);
  virtual void endType3Char(GfxState *state);

  virtual void type3D0(GfxState *state, double wx, double wy);
  virtual void type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury);

  private:
  void drawGeneralImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GfxImageColorMap*colorMap, GBool invert,
				   GBool inlineImg, int mask, int *maskColors,
				   Stream *maskStr, int maskWidth, int maskHeight, GBool maskInvert, GfxImageColorMap*maskColorMap);
  int SWFOutputDev::setGfxFont(char*id, char*filename, double quality);
  void strokeGfxline(GfxState *state, gfxline_t*line);
  void clipToGfxLine(GfxState *state, gfxline_t*line);
  void fillGfxLine(GfxState *state, gfxline_t*line);

  void finish();

  gfxresult_t*result; //filled when complete

  char outer_clip_box; //whether the page clip box is still on

  InfoOutputDev*info;
  SWFOutputState states[64];
  int statepos;

  int currentpage;

  PDFDoc*doc;
  XRef*xref;

  char* searchFont(char*name);
  char* substituteFont(GfxFont*gfxFont, char*oldname);
  char* writeEmbeddedFontToFile(XRef*ref, GfxFont*font);
  int t1id;
  int textmodeinfo; // did we write "Text will be rendered as polygon" yet?
  int jpeginfo; // did we write "File contains jpegs" yet?
  int pbminfo; // did we write "File contains jpegs" yet?
  int linkinfo; // did we write "File contains links" yet?
  int ttfinfo; // did we write "File contains TrueType Fonts" yet?
  int gradientinfo; // did we write "File contains Gradients yet?

  int type3active; // are we between beginType3()/endType3()?

  GfxState *laststate;

  char type3Warning;

  char* substitutetarget[256];
  char* substitutesource[256];
  int substitutepos;

  int user_movex,user_movey;
  int user_clipx1,user_clipx2,user_clipy1,user_clipy2;

  gfxline_t* current_text_stroke;
  gfxline_t* current_text_clip;
  char* current_font_id;
  gfxfont_t* current_gfxfont;
  gfxmatrix_t current_font_matrix;

  fontlist_t* fontlist;

  int*pages;
  int pagebuflen;
  int pagepos;

  friend void swf_output_preparepage(swf_output_t*swf, int pdfpage, int outputpage);
};

typedef struct _drawnchar
{
    gfxcoord_t x,y;
    int charid;
    gfxcolor_t color;
} drawnchar_t;

class CharBuffer
{
    drawnchar_t * chars;
    int buf_size;
    int num_chars;

public:

    CharBuffer()
    {
	buf_size = 32;
	chars = (drawnchar_t*)malloc(sizeof(drawnchar_t)*buf_size);
	memset(chars, 0, sizeof(drawnchar_t)*buf_size);
	num_chars = 0;
    }
    ~CharBuffer()
    {
	free(chars);chars = 0;
    }

    void grow(int size)
    {
	if(size>=buf_size) {
	    buf_size += 32;
	    chars = (drawnchar_t*)realloc(chars, sizeof(drawnchar_t)*buf_size);
	}
    }

    void addChar(int charid, gfxcoord_t x, gfxcoord_t y, gfxcolor_t color)
    {
	grow(num_chars);
	chars[num_chars].x = x;
	chars[num_chars].y = y;
	chars[num_chars].color = color;
	chars[num_chars].charid = charid;
    }
};

static char*getFontID(GfxFont*font);

struct FontInfo
{
    GfxFont*font;
    double max_size;
};

class InfoOutputDev:  public OutputDev 
{
  GHash* id2font;
  FontInfo* currentfont;
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
      id2font = new GHash();
  }
  virtual ~InfoOutputDev() 
  {
      delete id2font;
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
  virtual double getMaximumFontSize(char*id)
  {
      FontInfo*info = (FontInfo*)id2font->lookup(id);
      if(!info) {
	  msg("<error> Unknown font id: %s", id);
	  return 0.0;
      }
      return info->max_size;
  }

  virtual void updateFont(GfxState *state) 
  {
      GfxFont*font = state->getFont();
      if(!font)
          return;
      char*id = getFontID(font);
      
      FontInfo*info = (FontInfo*)id2font->lookup(id);
      if(!info) {
	GString* idStr = new GString(id);
	info = new FontInfo;
	info->font = font;
	info->max_size = 0;
	id2font->add(idStr, (void*)info);
	free(id);
	num_fonts++;
      }
      currentfont = info;
  }
  
  virtual void drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode code, int nBytes, Unicode *u, int uLen)
  {
      int render = state->getRender();
      if (render == 3)
	  return;
      double m11,m21,m12,m22;
      state->getFontTransMat(&m11, &m12, &m21, &m22);
      m11 *= state->getHorizScaling();
      m21 *= state->getHorizScaling();
      double lenx = sqrt(m11*m11 + m12*m12);
      double leny = sqrt(m21*m21 + m22*m22);
      double len = lenx>leny?lenx:leny;
      if(currentfont && currentfont->max_size < len) {
	  currentfont->max_size = len;
      }
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
    textmodeinfo = 0;
    ttfinfo = 0;
    linkinfo = 0;
    pbminfo = 0;
    type3active = 0;
    statepos = 0;
    xref = 0;
    substitutepos = 0;
    type3Warning = 0;
    user_movex = 0;
    user_movey = 0;
    user_clipx1 = 0;
    user_clipy1 = 0;
    user_clipx2 = 0;
    user_clipy2 = 0;
    current_text_stroke = 0;
    current_text_clip = 0;
    fontlist = 0;
    result = 0;
    outer_clip_box = 0;
    pages = 0;
    pagebuflen = 0;
    pagepos = 0;
    output = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    gfxdevice_swf_init(output);
    /* configure device */
    parameter_t*p = device_config;
    while(p) {
	output->setparameter(output, p->name, p->value);
	p = p->next;
    }
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
  char* id = getFontID(font);
  char* name = getFontName(font);
  Ref* r=font->getID();
  msg("%s=========== %s (ID:%d,%d) ==========\n", loglevel, name, r->num,r->gen);

  GString*gstr  = font->getTag();
   
  msg("%s| Tag: %s\n", loglevel, id);
  
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
  char*embeddedName=0;
  if(font->getEmbeddedFontName()) {
    embeddedName = font->getEmbeddedFontName()->getCString();
  }
  if(embedded)
   msg("%s| Embedded id: %s id: %d\n",loglevel, FIXNULL(embeddedName), embRef.num);

  gstr = font->getExtFontFile();
  if(gstr)
   msg("%s| External Font file: %s\n", loglevel, FIXNULL(gstr->getCString()));

  // Get font descriptor flags.
  if(font->isFixedWidth()) msg("%s| is fixed width\n", loglevel);
  if(font->isSerif()) msg("%s| is serif\n", loglevel);
  if(font->isSymbolic()) msg("%s| is symbolic\n", loglevel);
  if(font->isItalic()) msg("%s| is italic\n", loglevel);
  if(font->isBold()) msg("%s| is bold\n", loglevel);

  free(id);
  free(name);
}

//void SWFOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) {printf("void SWFOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) \n");}
//void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) {printf("void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) \n");}


void dump_outline(gfxline_t*line)
{
    while(line) {
	if(line->type == gfx_moveTo) {
	    msg("<debug> |     moveTo %.2f %.2f", line->x,line->y);
	} else if(line->type == gfx_lineTo) {
	    msg("<debug> |     lineTo %.2f %.2f", line->x,line->y);
	} else if(line->type == gfx_splineTo) {
	    msg("<debug> |     splineTo (%.2f %.2f) %.2f %.2f", line->sx,line->sy, line->x, line->y);
	}
	line = line->next;
    }
}

gfxline_t* gfxPath_to_gfxline(GfxState*state, GfxPath*path, int closed, int user_movex, int user_movey)
{
    int num = path->getNumSubpaths();
    int s,t;
    int cpos = 0;
    double lastx=0,lasty=0,posx=0,posy=0;
    int needsfix=0;
    if(!num) {
	msg("<warning> empty path");
	return 0;
    }
    gfxdrawer_t draw;
    gfxdrawer_target_gfxline(&draw);

    for(t = 0; t < num; t++) {
	GfxSubpath *subpath = path->getSubpath(t);
	int subnum = subpath->getNumPoints();
	double bx=0,by=0,cx=0,cy=0;

	for(s=0;s<subnum;s++) {
	   double x,y;
	   
	   state->transform(subpath->getX(s),subpath->getY(s),&x,&y);
	   x += user_movex;
	   y += user_movey;

	   if(s==0) {
		if(closed && needsfix && (fabs(posx-lastx)+fabs(posy-lasty))>0.001) {
		    draw.lineTo(&draw, lastx, lasty);
		}
		draw.moveTo(&draw, x,y);
		posx = lastx = x; 
		posy = lasty = y;
		cpos = 0;
		needsfix = 0;
	   } else if(subpath->getCurve(s) && cpos==0) {
		bx = x;
		by = y;
		cpos = 1;
	   } else if(subpath->getCurve(s) && cpos==1) {
		cx = x;
		cy = y;
		cpos = 2;
	   } else {
	        posx = x;
	        posy = y;
	        if(cpos==0) {
		    draw.lineTo(&draw, x,y);
		} else {
		    gfxdraw_cubicTo(&draw, bx,by, cx,cy, x,y, 0.05);
		}
		needsfix = 1;
		cpos = 0;
	   }
	}
    }
    /* fix non-closed lines */
    if(closed && needsfix && (fabs(posx-lastx)+fabs(posy-lasty))>0.001) {
	draw.lineTo(&draw, lastx, lasty);
    }
    gfxline_t*result = (gfxline_t*)draw.result(&draw);
    return result;
}

/*----------------------------------------------------------------------------
 * Primitive Graphic routines
 *----------------------------------------------------------------------------*/

void SWFOutputDev::stroke(GfxState *state) 
{
    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 0, user_movex, user_movey);
    strokeGfxline(state, line);
    gfxline_free(line);
}

void SWFOutputDev::strokeGfxline(GfxState *state, gfxline_t*line)
{
    int lineCap = state->getLineCap(); // 0=butt, 1=round 2=square
    int lineJoin = state->getLineJoin(); // 0=miter, 1=round 2=bevel
    double miterLimit = state->getMiterLimit();
    double width = state->getTransformedLineWidth();

    GfxRGB rgb;
    double opaq = state->getStrokeOpacity();
    if(type3active)
	state->getFillRGB(&rgb);
    else
	state->getStrokeRGB(&rgb);
    gfxcolor_t col;
    col.r = colToByte(rgb.r);
    col.g = colToByte(rgb.g);
    col.b = colToByte(rgb.b);
    col.a = (unsigned char)(opaq*255);
   
    gfx_capType capType = gfx_capRound;
    if(lineCap == 0) capType = gfx_capButt;
    else if(lineCap == 1) capType = gfx_capRound;
    else if(lineCap == 2) capType = gfx_capSquare;

    gfx_joinType joinType = gfx_joinRound;
    if(lineJoin == 0) joinType = gfx_joinMiter;
    else if(lineJoin == 1) joinType = gfx_joinRound;
    else if(lineJoin == 2) joinType = gfx_joinBevel;

    int dashnum = 0;
    double dashphase = 0;
    double * ldash = 0;
    state->getLineDash(&ldash, &dashnum, &dashphase);

    gfxline_t*line2 = 0;

    if(dashnum && ldash) {
	float * dash = (float*)malloc(sizeof(float)*(dashnum+1));
	int t;
	double cut = 0;
	int fixzero = 0;
	msg("<trace> %d dashes", dashnum);
	msg("<trace> |  phase: %f", dashphase);
	for(t=0;t<dashnum;t++) {
	    dash[t] = ldash[t];
	    msg("<trace> |  d%-3d: %f", t, ldash[t]);
	}
	dash[dashnum] = -1;
	if(getLogLevel() >= LOGLEVEL_TRACE) {
	    dump_outline(line);
	}

	line2 = gfxtool_dash_line(line, dash, dashphase);
	line = line2;
	free(dash);
	msg("<trace> After dashing:");
    }
    
    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> stroke width=%f join=%s cap=%s dashes=%d color=%02x%02x%02x%02x\n",
		width,
		lineJoin==0?"miter": (lineJoin==1?"round":"bevel"),
		lineCap==0?"butt": (lineJoin==1?"round":"square"),
		dashnum,
		col.r,col.g,col.b,col.a
		);
        dump_outline(line);
    }
   
    //swfoutput_drawgfxline(output, line, width, &col, capType, joinType, miterLimit);
    output->stroke(output, line, width, &col, capType, joinType, miterLimit);
    
    if(line2)
	gfxline_free(line2);
}

void convertRGB()
{
}

gfxcolor_t getFillColor(GfxState * state)
{
    GfxRGB rgb;
    double opaq = state->getFillOpacity();
    state->getFillRGB(&rgb);
    gfxcolor_t col;
    col.r = colToByte(rgb.r);
    col.g = colToByte(rgb.g);
    col.b = colToByte(rgb.b);
    col.a = (unsigned char)(opaq*255);
    return col;
}

void SWFOutputDev::fillGfxLine(GfxState *state, gfxline_t*line) 
{
    gfxcolor_t col = getFillColor(state);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> fill %02x%02x%02x%02x\n", col.r, col.g, col.b, col.a);
        dump_outline(line);
    }
    output->fill(output, line, &col);
}
void SWFOutputDev::fill(GfxState *state) 
{
    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 1, user_movex, user_movey);
    fillGfxLine(state, line);
    gfxline_free(line);
}
void SWFOutputDev::eoFill(GfxState *state) 
{
    GfxPath * path = state->getPath();
    gfxcolor_t col = getFillColor(state);

    gfxline_t*line= gfxPath_to_gfxline(state, path, 1, user_movex, user_movey);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> eofill\n");
        dump_outline(line);
    }

    output->fill(output, line, &col);
    gfxline_free(line);
}

void SWFOutputDev::clip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    gfxline_t*line = gfxPath_to_gfxline(state, path, 1, user_movex, user_movey);
    clipToGfxLine(state, line);
    gfxline_free(line);
}

void SWFOutputDev::clipToGfxLine(GfxState *state, gfxline_t*line) 
{
    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> clip\n");
        dump_outline(line);
    }

    output->startclip(output, line);
    states[statepos].clipping++;
}
void SWFOutputDev::eoClip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    gfxline_t*line = gfxPath_to_gfxline(state, path, 1, user_movex, user_movey);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> eoclip\n");
        dump_outline(line);
    }

    output->startclip(output, line);
    states[statepos].clipping++;
    gfxline_free(line);
}

void SWFOutputDev::endframe()
{
    if(outer_clip_box) {
	output->endclip(output);
	outer_clip_box = 0;
    }

    output->endpage(output);
}

void SWFOutputDev::finish()
{
    if(outer_clip_box) {
	if(output) {
	    output->endclip(output);
	}
	outer_clip_box = 0;
    }
    if(output) {
	this->result = output->finish(output);
	free(output);output=0;
    }
}

int SWFOutputDev::save(char*filename)
{
    finish();
    return result->save(result, filename);
}
void* SWFOutputDev::get(char*name)
{
    finish();
    return result->get(result, name);
}

SWFOutputDev::~SWFOutputDev() 
{
    finish();

    if(this->result) {
	this->result->destroy(this->result);
	this->result = 0;
    }
    
    if(this->pages) {
	free(this->pages); this->pages = 0;
    }

    fontlist_t*l = this->fontlist;
    while(l) {
	fontlist_t*next = l->next;
	l->next = 0;
	gfxfont_free(l->font);
	free(l->id);
	free(l->filename);
	free(l);
	l = next;
    }
    this->fontlist = 0;
};
GBool SWFOutputDev::upsideDown() 
{
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

char*renderModeDesc[]= {"fill", "stroke", "fill+stroke", "invisible",
                      "clip+fill", "stroke+clip", "fill+stroke+clip", "clip"};

#define RENDER_FILL 0
#define RENDER_STROKE 1
#define RENDER_FILLSTROKE 2
#define RENDER_INVISIBLE 3
#define RENDER_CLIP 4

static char tmp_printstr[4096];
char* makeStringPrintable(char*str)
{
    int len = strlen(str);
    int dots = 0;
    if(len>=80) {
	len = 80;
	dots = 1;
    }
    int t;
    for(t=0;t<len;t++) {
	char c = str[t];
	if(c<32 || c>124) {
	    c = '.';
	}
	tmp_printstr[t] = c;
    }
    if(dots) {
	tmp_printstr[len++] = '.';
	tmp_printstr[len++] = '.';
	tmp_printstr[len++] = '.';
    }
    tmp_printstr[len] = 0;
    return tmp_printstr;
}


int getGfxCharID(gfxfont_t*font, int charnr, char *charname, int u)
{
    char*uniname = 0;
    if(u>0) {
	int t;
	/* find out char name from unicode index 
	   TODO: should be precomputed
	 */
	for(t=0;t<sizeof(nameToUnicodeTab)/sizeof(nameToUnicodeTab[0]);t++) {
	    if(nameToUnicodeTab[t].u == u) {
		uniname = nameToUnicodeTab[t].name;
		break;
	    }
	}
    }

    if(charname) {
	int t;
	for(t=0;t<font->num_glyphs;t++) {
	    if(font->glyphs[t].name && !strcmp(font->glyphs[t].name,charname)) {
		msg("<debug> Char [%d,>%s<,%d] maps to %d\n", charnr, charname, u, t);
		return t;
	    }
	}
	/* if we didn't find the character, maybe
	   we can find the capitalized version */
	for(t=0;t<font->num_glyphs;t++) {
	    if(font->glyphs[t].name && !strcasecmp(font->glyphs[t].name,charname)) {
		msg("<debug> Char [%d,>>%s<<,%d] maps to %d\n", charnr, charname, u, t);
		return t;
	    }
	}
    }

    if(uniname) {
	int t;
	for(t=0;t<font->num_glyphs;t++) {
	    if(font->glyphs[t].name && !strcmp(font->glyphs[t].name,uniname)) {
		msg("<debug> Char [%d,%s,>%d(%s)<] maps to %d\n", charnr, charname, u, uniname, t);
		return t;
	    }
	}
	/* if we didn't find the character, maybe
	   we can find the capitalized version */
	for(t=0;t<font->num_glyphs;t++) {
	    if(font->glyphs[t].name && !strcasecmp(font->glyphs[t].name,uniname)) {
		msg("<debug> Char [%d,%s,>>%d(%s)<<] maps to %d\n", charnr, charname, u, uniname, t);
		return t;
	    }
	}
    }

    /* try to use the unicode id */
    if(u>=0 && u<font->max_unicode && font->unicode2glyph[u]>=0) {
	msg("<debug> Char [%d,%s,>%d<] maps to %d\n", charnr, charname, u, font->unicode2glyph[u]);
	return font->unicode2glyph[u];
    }

    if(charnr>=0 && charnr<font->num_glyphs) {
	msg("<debug> Char [>%d<,%s,%d] maps to %d\n", charnr, charname, u, charnr);
	return charnr;
    }
    
    return -1;
}


void SWFOutputDev::beginString(GfxState *state, GString *s) 
{ 
    int render = state->getRender();
    if(current_text_stroke) {
	msg("<error> Error: Incompatible change of text rendering to %d while inside cliptext", render);
    }

    msg("<trace> beginString(%s) render=%d", makeStringPrintable(s->getCString()), render);
    double m11,m21,m12,m22;
//    msg("<debug> %s beginstring \"%s\"\n", gfxstate2str(state), s->getCString());
    state->getFontTransMat(&m11, &m12, &m21, &m22);
    m11 *= state->getHorizScaling();
    m21 *= state->getHorizScaling();

    this->current_font_matrix.m00 = m11 / 1024.0;
    this->current_font_matrix.m01 = m12 / 1024.0;
    this->current_font_matrix.m10 = -m21 / 1024.0;
    this->current_font_matrix.m11 = -m22 / 1024.0;
    this->current_font_matrix.tx = 0;
    this->current_font_matrix.ty = 0;

    gfxmatrix_t m = this->current_font_matrix;

    /*if(render != 3 && render != 0)
	msg("<warning> Text rendering mode %d (%s) not fully supported yet (for text \"%s\")", render, renderModeDesc[render&7], makeStringPrintable(s->getCString()));*/
    states[statepos].textRender = render;
}

void SWFOutputDev::drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode c, int nBytes, Unicode *_u, int uLen)
{
    int render = state->getRender();
    // check for invisible text -- this is used by Acrobat Capture
    if (render == 3) {
	msg("<debug> Ignoring invisible text: char %d at %f,%f", c, x, y);
	return;
    }

    if(states[statepos].textRender != render)
	msg("<error> Internal error: drawChar.render!=beginString.render");

    gfxcolor_t col = getFillColor(state);

    Gushort *CIDToGIDMap = 0;
    GfxFont*font = state->getFont();

    if(font->getType() == fontType3) {
	/* type 3 chars are passed as graphics */
	msg("<debug> type3 char at %f/%f", x, y);
	return;
    }
    
    Unicode u=0;
    char*name=0;

    if(font->isCIDFont()) {
	GfxCIDFont*cfont = (GfxCIDFont*)font;

	if(font->getType() == fontCIDType2)
	    CIDToGIDMap = cfont->getCIDToGID();
    } else {
	Gfx8BitFont*font8;
	font8 = (Gfx8BitFont*)font;
	char**enc=font8->getEncoding();
	name = enc[c];
    }
    if (CIDToGIDMap) {
	msg("<debug> drawChar(%f, %f, c='%c' (%d), GID=%d, u=%d <%d>) CID=%d name=\"%s\" render=%d\n", x, y, (c&127)>=32?c:'?', c, CIDToGIDMap[c], u, uLen, font->isCIDFont(), FIXNULL(name), render);
	c = CIDToGIDMap[c];
    } else {
	msg("<debug> drawChar(%f,%f,c='%c' (%d), u=%d <%d>) CID=%d name=\"%s\" render=%d\n",x,y,(c&127)>=32?c:'?',c,u, uLen, font->isCIDFont(), FIXNULL(name), render);
    }

    int charid = -1;
   
    if(uLen<=1) {
	charid = getGfxCharID(current_gfxfont, c, name, u);
    } else {
	charid = getGfxCharID(current_gfxfont, c, name, -1);

	if(charid < 0) {
	    /* multiple unicodes- should usually map to a ligature.
	       if the ligature doesn't exist, we need to draw
	       the characters one-by-one. */
	    int t;
	    msg("<warning> ligature %d missing in font %s\n", c, current_font_id);
	    for(t=0;t<uLen;t++) {
		drawChar(state, x, y, dx, dy, originX, originY, c, nBytes, _u+t, 1);
	    }
	    return;
	}
    }

    if(charid<0) {
	if(!name) {
	    msg("<warning> Didn't find character '%s' (c=%d,u=%d) in current charset (%s, %d characters)", 
		    FIXNULL(name),c, u, FIXNULL((char*)current_font_id), current_gfxfont->num_glyphs);
	}
	return;
    }

    gfxmatrix_t m = this->current_font_matrix;
    state->transform(x, y, &m.tx, &m.ty);
    m.tx += user_movex;
    m.ty += user_movey;

    if(render == RENDER_FILL) {
	output->drawchar(output, current_font_id, charid, &col, &m);
    } else {
	msg("<debug> Drawing glyph %d as shape", charid);
	if(!textmodeinfo) {
	    msg("<notice> Some texts will be rendered as shape");
	    textmodeinfo = 1;
	}
	gfxline_t*glyph = current_gfxfont->glyphs[charid].line;
	gfxline_t*tglyph = gfxline_clone(glyph);
	gfxline_transform(tglyph, &m);
	if((render&3) != RENDER_INVISIBLE) {
	    gfxline_t*add = gfxline_clone(tglyph);
	    current_text_stroke = gfxline_append(current_text_stroke, add);
	}
	if(render&RENDER_CLIP) {
	    gfxline_t*add = gfxline_clone(tglyph);
	    current_text_clip = gfxline_append(current_text_clip, add);
	}
	gfxline_free(tglyph);
    }
}

void SWFOutputDev::endString(GfxState *state) 
{ 
    int render = state->getRender();
    msg("<trace> endString() render=%d textstroke=%08x", render, current_text_stroke);
    if(states[statepos].textRender != render)
	msg("<error> Internal error: drawChar.render!=beginString.render");
    
    if(current_text_stroke) {
	/* fillstroke and stroke text rendering objects we can process right
	   now (as there may be texts of other rendering modes in this
	   text object)- clipping objects have to wait until endTextObject,
	   however */
	if((render&3) == RENDER_FILL) {
	    fillGfxLine(state, current_text_stroke);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	} else if((render&3) == RENDER_FILLSTROKE) {
	    fillGfxLine(state, current_text_stroke);
	    strokeGfxline(state, current_text_stroke);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	} else if((render&3) == RENDER_STROKE) {
	    strokeGfxline(state, current_text_stroke);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	}
    }
}    

void SWFOutputDev::endTextObject(GfxState *state)
{
    int render = state->getRender();
    msg("<trace> endTextObject() render=%d textstroke=%08x clipstroke=%08x", render, current_text_stroke, current_text_clip);
    if(states[statepos].textRender != render)
	msg("<error> Internal error: drawChar.render!=beginString.render");
    
    if(current_text_clip) {
	clipToGfxLine(state, current_text_clip);
	gfxline_free(current_text_clip);
	current_text_clip = 0;
    }
}

/* the logic seems to be as following:
   first, beginType3Char is called, with the charcode and the coordinates.
   if this function returns true, it already knew about the char and has now drawn it.
   if the function returns false, it's a new char, and type3D1 is called with some parameters-
   the all draw operations until endType3Char are part of the char (which in this moment is
   at the position first passed to beginType3Char). the char ends with endType3Char.

   The drawing operations between beginType3Char and endType3Char are somewhat different to
   the normal ones. For example, the fillcolor equals the stroke color.
*/

GBool SWFOutputDev::beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen)
{
    msg("<debug> beginType3Char %d, %08x, %d", code, *u, uLen);
    type3active = 1;
    /* the character itself is going to be passed using the draw functions */
    return gFalse; /* gTrue= is_in_cache? */
}

void SWFOutputDev::type3D0(GfxState *state, double wx, double wy) {
    msg("<debug> type3D0 width=%f height=%f", wx, wy);
}
void SWFOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury) {
    msg("<debug> type3D1 width=%f height=%f bbox=(%f,%f,%f,%f)", wx, wy,
	    llx,lly,urx,ury);
}

void SWFOutputDev::endType3Char(GfxState *state)
{
    type3active = 0;
    msg("<debug> endType3Char");
}

void SWFOutputDev::startFrame(int width, int height) 
{
    output->startpage(output, width, height);
}

void SWFOutputDev::startPage(int pageNum, GfxState *state, double crop_x1, double crop_y1, double crop_x2, double crop_y2) 
{
    this->currentpage = pageNum;
    double x1,y1,x2,y2;
    int rot = doc->getPageRotate(1);
    gfxcolor_t white;
    laststate = state;
    gfxline_t clippath[5];

    white.r = white.g = white.b = white.a = 255;

    /* state->transform(state->getX1(),state->getY1(),&x1,&y1);
    state->transform(state->getX2(),state->getY2(),&x2,&y2);
    Use CropBox, not MediaBox, as page size
    */
    
    /*x1 = crop_x1;
    y1 = crop_y1;
    x2 = crop_x2;
    y2 = crop_y2;*/
    state->transform(crop_x1,crop_y1,&x1,&y1); //x1 += user_movex; y1 += user_movey;
    state->transform(crop_x2,crop_y2,&x2,&y2); //x2 += user_movex; y2 += user_movey;

    if(x2<x1) {double x3=x1;x1=x2;x2=x3;}
    if(y2<y1) {double y3=y1;y1=y2;y2=y3;}


    /* apply user clip box */
    if(user_clipx1|user_clipy1|user_clipx2|user_clipy2) {
        /*if(user_clipx1 > x1)*/ x1 = user_clipx1;
        /*if(user_clipx2 < x2)*/ x2 = user_clipx2;
        /*if(user_clipy1 > y1)*/ y1 = user_clipy1;
        /*if(user_clipy2 < y2)*/ y2 = user_clipy2;
    }

    //msg("<verbose> Bounding box is (%f,%f)-(%f,%f) [shifted by %d/%d]", x1,y1,x2,y2, user_movex, user_movey);
    
    if(outer_clip_box) {
	output->endclip(output);
	outer_clip_box = 0;
    }

    msg("<notice> processing PDF page %d (%dx%d:%d:%d) (move:%d:%d)", pageNum, (int)x2-(int)x1,(int)y2-(int)y1, (int)x1, (int)y1, user_movex, user_movey);
    if(rot!=0)
        msg("<verbose> page is rotated %d degrees\n", rot);

    clippath[0].type = gfx_moveTo;clippath[0].x = x1; clippath[0].y = y1; clippath[0].next = &clippath[1];
    clippath[1].type = gfx_lineTo;clippath[1].x = x2; clippath[1].y = y1; clippath[1].next = &clippath[2];
    clippath[2].type = gfx_lineTo;clippath[2].x = x2; clippath[2].y = y2; clippath[2].next = &clippath[3];
    clippath[3].type = gfx_lineTo;clippath[3].x = x1; clippath[3].y = y2; clippath[3].next = &clippath[4];
    clippath[4].type = gfx_lineTo;clippath[4].x = x1; clippath[4].y = y1; clippath[4].next = 0;
    output->startclip(output, clippath); outer_clip_box = 1;
    output->fill(output, clippath, &white);
}

void SWFOutputDev::drawLink(Link *link, Catalog *catalog) 
{
    msg("<debug> drawlink\n");
    double x1, y1, x2, y2, w;
    gfxline_t points[5];
    int x, y;

    link->getRect(&x1, &y1, &x2, &y2);
    cvtUserToDev(x1, y1, &x, &y);
    points[0].type = gfx_moveTo;
    points[0].x = points[4].x = x + user_movex;
    points[0].y = points[4].y = y + user_movey;
    points[0].next = &points[1];
    cvtUserToDev(x2, y1, &x, &y);
    points[1].type = gfx_lineTo;
    points[1].x = x + user_movex;
    points[1].y = y + user_movey;
    points[1].next = &points[2];
    cvtUserToDev(x2, y2, &x, &y);
    points[2].type = gfx_lineTo;
    points[2].x = x + user_movex;
    points[2].y = y + user_movey;
    points[2].next = &points[3];
    cvtUserToDev(x1, y2, &x, &y);
    points[3].type = gfx_lineTo;
    points[3].x = x + user_movex;
    points[3].y = y + user_movey;
    points[3].next = &points[4];
    cvtUserToDev(x1, y1, &x, &y);
    points[4].type = gfx_lineTo;
    points[4].x = x + user_movex;
    points[4].y = y + user_movey;
    points[4].next = 0;

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
			if(pages && pagepos>0)
			    page = pages[pagepos-1];
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
	int lpage = -1;
        for(t=1;t<=pagepos;t++) {
            if(pages[t]==page) {
		lpage = t;
                break;
	    }
	}
        if(lpage<0) {
	    lpage = page;
	}
	char buf[80];
	sprintf(buf, "page%d", lpage);
	output->drawlink(output, points, buf);
    }
    else if(url)
    {
        output->drawlink(output, points, url);
    }

    msg("<verbose> \"%s\" link to \"%s\" (%d)\n", type, FIXNULL(s), page);
    free(s);s=0;
}

void SWFOutputDev::saveState(GfxState *state) {
  msg("<trace> saveState\n");
  updateAll(state);
  if(statepos>=64) {
    msg("<error> Too many nested states in pdf.");
    return;
  }
  statepos ++;
  states[statepos].clipping = 0; //? shouldn't this be the current value?
  states[statepos].textRender = states[statepos-1].textRender;
};

void SWFOutputDev::restoreState(GfxState *state) {
  msg("<trace> restoreState\n");
  updateAll(state);
  while(states[statepos].clipping) {
      output->endclip(output);
      states[statepos].clipping--;
  }
  statepos--;
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
    //swfoutput_setlinewidth(&output, width);
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

    //swfoutput_setfillcolor(&output, (char)(rgb.r*255), (char)(rgb.g*255), (char)(rgb.b*255), (char)(opaq*255));
}

void SWFOutputDev::updateStrokeColor(GfxState *state) 
{
    GfxRGB rgb;
    double opaq = state->getStrokeOpacity();
    state->getStrokeRGB(&rgb);
    //swfoutput_setstrokecolor(&output, (char)(rgb.r*255), (char)(rgb.g*255), (char)(rgb.b*255), (char)(opaq*255));
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

    //if (font->getType() == fontType1C) {
    if (0) { //font->getType() == fontType1C) {
      if (!(fontBuf = font->readEmbFontFile(xref, &fontLen))) {
	fclose(f);
	msg("<error> Couldn't read embedded font file");
	return 0;
      }
      FoFiType1C *cvt = FoFiType1C::make(fontBuf, fontLen);
      if(!cvt) return 0;
      cvt->convertToType1(NULL, gTrue, FoFiWrite, f);
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
      FoFiTrueType *cvt = FoFiTrueType::make(fontBuf, fontLen);
      cvt->writeTTF(FoFiWrite, f);
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

    if(!config_use_fontconfig)
        return 0;
    
#ifdef HAVE_FONTCONFIG
    FcPattern *pattern, *match;
    FcResult result;
    FcChar8 *v;

    static int fcinitcalled = false; 
        
    msg("<debug> searchForSuitableFont(%s)", name);
    
    // call init ony once
    if (!fcinitcalled) {
        msg("<debug> Initializing FontConfig...");
        fcinitcalled = true;
	if(!FcInit()) {
            msg("<debug> FontConfig Initialization failed. Disabling.");
            config_use_fontconfig = 0;
            return 0;
        }
        msg("<debug> ...initialized FontConfig");
    }
   
    msg("<debug> FontConfig: Create \"%s\" Family Pattern", name);
    pattern = FcPatternBuild(NULL, FC_FAMILY, FcTypeString, name, NULL);
    if (gfxFont->isItalic()) // check for italic
        msg("<debug> FontConfig: Adding Italic Slant");
	FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ITALIC);
    if (gfxFont->isBold()) // check for bold
        msg("<debug> FontConfig: Adding Bold Weight");
        FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);

    msg("<debug> FontConfig: Try to match...");
    // configure and match using the original font name 
    FcConfigSubstitute(0, pattern, FcMatchPattern); 
    FcDefaultSubstitute(pattern);
    match = FcFontMatch(0, pattern, &result);
    
    if (FcPatternGetString(match, "family", 0, &v) == FcResultMatch) {
        msg("<debug> FontConfig: family=%s", (char*)v);
        // if we get an exact match
        if (strcmp((char *)v, name) == 0) {
	    if (FcPatternGetString(match, "file", 0, &v) == FcResultMatch) {
		filename = strdup((char*)v); // mem leak
		char *nfn = strrchr(filename, '/');
		if(nfn) fontname = strdup(nfn+1);
		else    fontname = filename;
            }
            msg("<debug> FontConfig: Returning \"%s\"", fontname);
        } else {
            // initialize patterns
            FcPatternDestroy(pattern);
    	    FcPatternDestroy(match);

            // now match against serif etc.
	    if (gfxFont->isSerif()) {
                msg("<debug> FontConfig: Create Serif Family Pattern");
                pattern = FcPatternBuild (NULL, FC_FAMILY, FcTypeString, "serif", NULL);
            } else if (gfxFont->isFixedWidth()) {
                msg("<debug> FontConfig: Create Monospace Family Pattern");
                pattern = FcPatternBuild (NULL, FC_FAMILY, FcTypeString, "monospace", NULL);
            } else {
                msg("<debug> FontConfig: Create Sans Family Pattern");
                pattern = FcPatternBuild (NULL, FC_FAMILY, FcTypeString, "sans", NULL);
            }

            // check for italic
            if (gfxFont->isItalic()) {
                msg("<debug> FontConfig: Adding Italic Slant");
                int bb = FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ITALIC);
            }
            // check for bold
            if (gfxFont->isBold()) {
                msg("<debug> FontConfig: Adding Bold Weight");
                int bb = FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);
            }

            msg("<debug> FontConfig: Try to match... (2)");
            // configure and match using serif etc
    	    FcConfigSubstitute (0, pattern, FcMatchPattern);
            FcDefaultSubstitute (pattern);
            match = FcFontMatch (0, pattern, &result);
            
            if (FcPatternGetString(match, "file", 0, &v) == FcResultMatch) {
		filename = strdup((char*)v); // mem leak
		char *nfn = strrchr(filename, '/');
		if(nfn) fontname = strdup(nfn+1);
		else    fontname = filename;
    	    }
            msg("<debug> FontConfig: Returning \"%s\"", fontname);
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
    msg("<notice> substituteFont(%s)", oldname);

    if(!(fontname = searchForSuitableFont(gfxFont))) {
	fontname = "Times-Roman";
    }
    filename = searchFont(fontname);
    if(!filename) {
	msg("<error> Couldn't find font %s- did you install the default fonts?", fontname);
	return 0;
    }

    if(substitutepos>=sizeof(substitutesource)/sizeof(char*)) {
	msg("<fatal> Too many fonts in file.");
	exit(1);
    }
    if(oldname) {
	substitutesource[substitutepos] = strdup(oldname); //mem leak
	substitutetarget[substitutepos] = fontname;
	msg("<notice> substituting %s -> %s", FIXNULL(oldname), FIXNULL(fontname));
	substitutepos ++;
    }
    return strdup(filename); //mem leak
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

int SWFOutputDev::setGfxFont(char*id, char*filename, double maxSize)
{
    gfxfont_t*font = 0;
    fontlist_t*last=0,*l = this->fontlist;

    /* TODO: should this be part of the state? */
    while(l) {
	last = l;
	if(!strcmp(l->id, id)) {
	    current_font_id = l->id;
	    current_gfxfont = l->font;
	    font = l->font;
	    output->addfont(output, id, current_gfxfont);
	    return 1;
	}
	l = l->next;
    }
    if(!filename) return 0;

    /* A font size of e.g. 9 means the font will be scaled down by
       1024 and scaled up by 9. So to have a maximum error of 1/20px,
       we have to divide 0.05 by (fontsize/1024)
     */
    double quality = (1024 * 0.05) / maxSize;
   
    msg("<verbose> Loading %s...", filename);
    font = gfxfont_load(filename, quality);
    msg("<verbose> Font %s loaded successfully", filename);

    l = new fontlist_t;
    l->font = font;
    l->filename = strdup(filename);
    l->id = strdup(id);
    l->next = 0;
    current_font_id = l->id;
    current_gfxfont = l->font;
    if(last) {
	last->next = l;
    } else {
	this->fontlist = l;
    }
    output->addfont(output, id, current_gfxfont);
    return 1;
}

void SWFOutputDev::updateFont(GfxState *state) 
{
    GfxFont*gfxFont = state->getFont();
      
    if (!gfxFont) {
	return;
    }  
    char * fontid = getFontID(gfxFont);
    double maxSize = 1.0;

    if(this->info) {
	maxSize = this->info->getMaximumFontSize(fontid);
    }
    
    int t;
    /* first, look if we substituted this font before-
       this way, we don't initialize the T1 Fonts
       too often */
    for(t=0;t<substitutepos;t++) {
	if(!strcmp(fontid, substitutesource[t])) {
	    free(fontid);fontid=0;
	    fontid = strdup(substitutetarget[t]);
	    break;
	}
    }

    /* second, see if this is a font which was used before-
       if so, we are done */
    if(setGfxFont(fontid, 0, 0)) {
	free(fontid);
	return;
    }
/*    if(swfoutput_queryfont(&output, fontid))
	swfoutput_setfont(&output, fontid, 0);
	
	msg("<debug> updateFont(%s) [cached]", fontid);
	return;
    }*/

    // look for Type 3 font
    if (gfxFont->getType() == fontType3) {
	if(!type3Warning) {
	    type3Warning = gTrue;
	    showFontError(gfxFont, 2);
	}
	free(fontid);
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
       (gfxFont->getType() == fontCIDType0C && forceType0Fonts) ||
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
      free(fontname);
    }
    if(!fileName) {
	char * fontname = getFontName(gfxFont);
	msg("<warning> Font %s %scould not be loaded.", fontname, embedded?"":"(not embedded) ");
	
	if(lastfontdir)
	    msg("<warning> Try putting a TTF version of that font (named \"%s.ttf\") into %s", fontname, lastfontdir);
	else
	    msg("<warning> Try specifying one or more font directories");

	fileName = substituteFont(gfxFont, fontid);
	if(fontid) { free(fontid);fontid = strdup(substitutetarget[substitutepos-1]); /*ugly hack*/};
	msg("<notice> Font is now %s (%s)", fontid, fileName);
    }

    if(!fileName) {
	msg("<error> Couldn't set font %s\n", fontid);
	free(fontid);
	return;
    }
	
    msg("<verbose> updateFont(%s) -> %s (max size: %f)", fontid, fileName, maxSize);
    dumpFontInfo("<verbose>", gfxFont);

    //swfoutput_setfont(&output, fontid, fileName);
    
    if(!setGfxFont(fontid, 0, 0)) {
	setGfxFont(fontid, fileName, maxSize);
    }
   
    if(fileName && del)
	unlinkfont(fileName);

    if(fileName)
        free(fileName);
    free(fontid);

    msg("<verbose> |");
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

#define IMAGE_TYPE_JPEG 0
#define IMAGE_TYPE_LOSSLESS 1

static void drawimage(gfxdevice_t*dev, gfxcolor_t* data, int sizex,int sizey, 
        double x1,double y1,
        double x2,double y2,
        double x3,double y3,
        double x4,double y4, int type)
{
    gfxcolor_t*newpic=0;
    
    double l1 = sqrt((x4-x1)*(x4-x1) + (y4-y1)*(y4-y1));
    double l2 = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
   
    gfxline_t p1,p2,p3,p4,p5;
    p1.type=gfx_moveTo;p1.x=x1; p1.y=y1;p1.next=&p2;
    p2.type=gfx_lineTo;p2.x=x2; p2.y=y2;p2.next=&p3;
    p3.type=gfx_lineTo;p3.x=x3; p3.y=y3;p3.next=&p4;
    p4.type=gfx_lineTo;p4.x=x4; p4.y=y4;p4.next=&p5;
    p5.type=gfx_lineTo;p5.x=x1; p5.y=y1;p5.next=0;

    {p1.x = (int)(p1.x*20)/20.0;
     p1.y = (int)(p1.y*20)/20.0;
     p2.x = (int)(p2.x*20)/20.0;
     p2.y = (int)(p2.y*20)/20.0;
     p3.x = (int)(p3.x*20)/20.0;
     p3.y = (int)(p3.y*20)/20.0;
     p4.x = (int)(p4.x*20)/20.0;
     p4.y = (int)(p4.y*20)/20.0;
     p5.x = (int)(p5.x*20)/20.0;
     p5.y = (int)(p5.y*20)/20.0;
    }
    
    float m00,m10,tx;
    float m01,m11,ty;
    
    gfxmatrix_t m;
    m.m00 = (p4.x-p1.x)/sizex; m.m10 = (p2.x-p1.x)/sizey;
    m.m01 = (p4.y-p1.y)/sizex; m.m11 = (p2.y-p1.y)/sizey;
    m.tx = p1.x - 0.5;
    m.ty = p1.y - 0.5;

    gfximage_t img;
    img.data = (gfxcolor_t*)data;
    img.width = sizex;
    img.height = sizey;
  
    if(type == IMAGE_TYPE_JPEG)
	/* TODO: pass image_dpi to device instead */
	dev->setparameter(dev, "next_bitmap_is_jpeg", "1");

    dev->fillbitmap(dev, &p1, &img, &m, 0);
}

void drawimagejpeg(gfxdevice_t*dev, gfxcolor_t*mem, int sizex,int sizey, 
        double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4)
{
    drawimage(dev,mem,sizex,sizey,x1,y1,x2,y2,x3,y3,x4,y4, IMAGE_TYPE_JPEG);
}

void drawimagelossless(gfxdevice_t*dev, gfxcolor_t*mem, int sizex,int sizey, 
        double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4)
{
    drawimage(dev,mem,sizex,sizey,x1,y1,x2,y2,x3,y3,x4,y4, IMAGE_TYPE_LOSSLESS);
}


void SWFOutputDev::drawGeneralImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GfxImageColorMap*colorMap, GBool invert,
				   GBool inlineImg, int mask, int*maskColors,
				   Stream *maskStr, int maskWidth, int maskHeight, GBool maskInvert, GfxImageColorMap*maskColorMap)
{
  double x1,y1,x2,y2,x3,y3,x4,y4;
  ImageStream *imgStr;
  Guchar pixBuf[4];
  GfxRGB rgb;
  int ncomps = 1;
  int bits = 1;
  unsigned char* maskbitmap = 0;
				 
  if(colorMap) {
    ncomps = colorMap->getNumPixelComps();
    bits = colorMap->getBits();
  }
  
  if(maskStr) {
      int x,y;
      unsigned char buf[8];
      maskbitmap = (unsigned char*)malloc(maskHeight*maskWidth);
      if(maskColorMap) {
	  ImageStream*imgMaskStr = new ImageStream(maskStr, maskWidth, maskColorMap->getNumPixelComps(), maskColorMap->getBits());
      	  imgMaskStr->reset();
	  unsigned char pal[256];
	  int n = 1 << colorMap->getBits();
	  int t;
	  for(t=0;t<n;t++) {
	      GfxGray gray;
	      pixBuf[0] = t;
	      maskColorMap->getGray(pixBuf, &gray);
	      pal[t] = colToByte(gray);
	  }
	  for (y = 0; y < maskHeight; y++) {
	      for (x = 0; x < maskWidth; x++) {
		  imgMaskStr->getPixel(buf);
		  maskbitmap[y*maskWidth+x] = pal[buf[0]];
	      }
	  }
	  delete imgMaskStr;
      } else {
	  ImageStream*imgMaskStr = new ImageStream(maskStr, maskWidth, 1, 1);
      	  imgMaskStr->reset();
	  for (y = 0; y < maskHeight; y++) {
	      for (x = 0; x < maskWidth; x++) {
		  imgMaskStr->getPixel(buf);
		  buf[0]^=maskInvert;
		  maskbitmap[y*maskWidth+x] = (buf[0]^1)*255;
	      }
	  }
	  delete imgMaskStr;
      }
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
      if(maskbitmap)
	  free(maskbitmap);
      return;
  }

  state->transform(0, 1, &x1, &y1); x1 += user_movex; y1 += user_movey;
  state->transform(0, 0, &x2, &y2); x2 += user_movex; y2 += user_movey;
  state->transform(1, 0, &x3, &y3); x3 += user_movex; y3 += user_movey;
  state->transform(1, 1, &x4, &y4); x4 += user_movex; y4 += user_movey;

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
      int i,j;
      unsigned char buf[8];
      int x,y;
      unsigned char*pic = new unsigned char[width*height];
      gfxcolor_t pal[256];
      GfxRGB rgb;
      state->getFillRGB(&rgb);

      memset(pal,255,sizeof(pal));
      pal[0].r = (int)(colToByte(rgb.r)); pal[1].r = 0;
      pal[0].g = (int)(colToByte(rgb.g)); pal[1].g = 0;
      pal[0].b = (int)(colToByte(rgb.b)); pal[1].b = 0;
      pal[0].a = 255;              pal[1].a = 0;
    
      int numpalette = 2;
      int realwidth = (int)sqrt(SQR(x2-x3) + SQR(y2-y3));
      int realheight = (int)sqrt(SQR(x1-x2) + SQR(y1-y2));
      for (y = 0; y < height; ++y)
      for (x = 0; x < width; ++x)
      {
	    imgStr->getPixel(buf);
	    if(invert) 
		buf[0]=1-buf[0];
	    pic[width*y+x] = buf[0];
      }
      
      /* the size of the drawn image is added to the identifier
	 as the same image may require different bitmaps if displayed
	 at different sizes (due to antialiasing): */
      int t,found = -1;
      if(type3active) {
	  unsigned char*pic2 = 0;
	  numpalette = 16;
	  
	  pic2 = antialize(pic,width,height,realwidth,realheight,numpalette);

	  if(!pic2) {
	    delete pic;
	    delete imgStr;
	    return;
	  }

	  width = realwidth;
	  height = realheight;
	  free(pic);
	  pic = pic2;
	  
	  /* make a black/white palette */

	  float r = 255/(numpalette-1);
	  int t;
	  for(t=0;t<numpalette;t++) {
	      pal[t].r = colToByte(rgb.r);
	      pal[t].g = colToByte(rgb.g);
	      pal[t].b = colToByte(rgb.b);
	      pal[t].a = (unsigned char)(t*r);
	  }
      }

      gfxcolor_t*pic2 = new gfxcolor_t[width*height];
      for (y = 0; y < height; ++y) {
	for (x = 0; x < width; ++x) {
	  pic2[width*y+x] = pal[pic[y*width+x]];
	}
      }
      drawimagelossless(output, pic2, width, height, x1,y1,x2,y2,x3,y3,x4,y4);
      free(pic2);
      free(pic);
      delete imgStr;
      if(maskbitmap) free(maskbitmap);
      return;
  }

  int x,y;

  if(colorMap->getNumPixelComps()!=1 || str->getKind()==strDCT) {
      gfxcolor_t*pic=new gfxcolor_t[width*height];
      for (y = 0; y < height; ++y) {
	for (x = 0; x < width; ++x) {
	  imgStr->getPixel(pixBuf);
	  colorMap->getRGB(pixBuf, &rgb);
	  pic[width*y+x].r = (unsigned char)(colToByte(rgb.r));
	  pic[width*y+x].g = (unsigned char)(colToByte(rgb.g));
	  pic[width*y+x].b = (unsigned char)(colToByte(rgb.b));
	  pic[width*y+x].a = 255;//(U8)(rgb.a * 255 + 0.5);
	  if(maskbitmap) {
	      pic[width*y+x].a = maskbitmap[(y*maskHeight/height)*maskWidth+(x*maskWidth/width)];
	  }
	}
      }
      if(str->getKind()==strDCT)
	  drawimagejpeg(output, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4);
      else
	  drawimagelossless(output, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4);
      delete pic;
      delete imgStr;
      if(maskbitmap) free(maskbitmap);
      return;
  } else {
      gfxcolor_t*pic=new gfxcolor_t[width*height];
      gfxcolor_t pal[256];
      int n = 1 << colorMap->getBits();
      int t;
      for(t=0;t<256;t++) {
	  pixBuf[0] = t;
	  colorMap->getRGB(pixBuf, &rgb);

	  {/*if(maskColors && *maskColors==t) {
	      msg("<notice> Color %d is transparent", t);
	      if (imgData->maskColors) {
		*alpha = 0;
		for (i = 0; i < imgData->colorMap->getNumPixelComps(); ++i) {
		  if (pix[i] < imgData->maskColors[2*i] ||
		      pix[i] > imgData->maskColors[2*i+1]) {
		    *alpha = 1;
		    break;
		  }
		}
	      } else {
		*alpha = 1;
	      }
	      if(!*alpha) {
		    pal[t].r = 0;
		    pal[t].g = 0;
		    pal[t].b = 0;
		    pal[t].a = 0;
	      }
	  } else {*/
	      pal[t].r = (unsigned char)(colToByte(rgb.r));
	      pal[t].g = (unsigned char)(colToByte(rgb.g));
	      pal[t].b = (unsigned char)(colToByte(rgb.b));
	      pal[t].a = 255;//(U8)(rgb.b * 255 + 0.5);
	  }
      }
      for (y = 0; y < height; ++y) {
	for (x = 0; x < width; ++x) {
	  imgStr->getPixel(pixBuf);
	  pic[width*y+x] = pal[pixBuf[0]];
	  if(maskbitmap) {
	      pic[width*y+x].a = maskbitmap[(y*maskHeight/height)*maskWidth+(x*maskWidth/width)];
	  }
	}
      }
      drawimagelossless(output, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4);

      delete pic;
      delete imgStr;
      if(maskbitmap) free(maskbitmap);
      return;
  }
}

void SWFOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool inlineImg) 
{
  if(states[statepos].textRender & 4) //clipped
      return;
  msg("<verbose> drawImageMask %dx%d, invert=%d inline=%d", width, height, invert, inlineImg);
  drawGeneralImage(state,ref,str,width,height,0,invert,inlineImg,1, 0, 0,0,0,0, 0);
}

void SWFOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg)
{
  if(states[statepos].textRender & 4) //clipped
      return;

  msg("<verbose> drawImage %dx%d, %s, %s, inline=%d", width, height, 
	  colorMap?"colorMap":"no colorMap", 
	  maskColors?"maskColors":"no maskColors",
	  inlineImg);
  if(colorMap)
      msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d\n", colorMap->getNumPixelComps(),
	      colorMap->getBits(),colorMap->getColorSpace()->getMode());
  drawGeneralImage(state,ref,str,width,height,colorMap,0,inlineImg,0,maskColors, 0,0,0,0, 0);
}
  
void SWFOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       Stream *maskStr, int maskWidth, int maskHeight,
			       GBool maskInvert)
{
  if(states[statepos].textRender & 4) //clipped
      return;

  msg("<verbose> drawMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	  colorMap?"colorMap":"no colorMap", 
	  maskWidth, maskHeight);
  if(colorMap)
      msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d\n", colorMap->getNumPixelComps(),
	      colorMap->getBits(),colorMap->getColorSpace()->getMode());
  drawGeneralImage(state,ref,str,width,height,colorMap,0,0,0,0, maskStr, maskWidth, maskHeight, maskInvert, 0);
}

void SWFOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap)
{
  if(states[statepos].textRender & 4) //clipped
      return;

  msg("<verbose> drawSoftMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	  colorMap?"colorMap":"no colorMap", 
	  maskWidth, maskHeight);
  if(colorMap)
      msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d\n", colorMap->getNumPixelComps(),
	      colorMap->getBits(),colorMap->getColorSpace()->getMode());
  drawGeneralImage(state,ref,str,width,height,colorMap,0,0,0,0, maskStr, maskWidth, maskHeight, 0, maskColorMap);
}

//SWFOutputDev*output = 0; 

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

int jpeg_dpi = 0;
int ppm_dpi = 0;

void storeDeviceParameter(char*name, char*value)
{
    parameter_t*p = new parameter_t();
    p->name = strdup(name);
    p->value = strdup(value);
    p->next = 0;
    if(device_config_next) {
	device_config_next->next = p;
	device_config_next = p;
    } else {
	device_config = p;
	device_config_next = p;
    }
}

void pdfswf_setparameter(char*name, char*value)
{
    msg("<verbose> setting parameter %s to \"%s\"", name, value);
    if(!strcmp(name, "caplinewidth")) {
	caplinewidth = atof(value);
    } else if(!strcmp(name, "zoom")) {
	char buf[80];
	zoom = atof(value);
	sprintf(buf, "%f", (double)jpeg_dpi/(double)zoom);
	storeDeviceParameter("jpegsubpixels", buf);
	sprintf(buf, "%f", (double)ppm_dpi/(double)zoom);
	storeDeviceParameter("ppmsubpixels", buf);
    } else if(!strcmp(name, "jpegdpi")) {
	char buf[80];
	jpeg_dpi = atoi(value);
	sprintf(buf, "%f", (double)jpeg_dpi/(double)zoom);
	storeDeviceParameter("jpegsubpixels", buf);
    } else if(!strcmp(name, "ppmdpi")) {
	char buf[80];
	ppm_dpi = atoi(value);
	sprintf(buf, "%f", (double)ppm_dpi/(double)zoom);
	storeDeviceParameter("ppmsubpixels", buf);
    } else if(!strcmp(name, "forceType0Fonts")) {
	forceType0Fonts = atoi(value);
    } else if(!strncmp(name, "fontdir", strlen("fontdir"))) {
        pdfswf_addfontdir(value);
    } else if(!strncmp(name, "languagedir", strlen("languagedir"))) {
        pdfswf_addlanguagedir(value);
    } else if(!strcmp(name, "fontconfig")) {
        config_use_fontconfig = atoi(value);
    } else {
	storeDeviceParameter(name,value);
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

static char* dirseparator()
{
#ifdef WIN32
    return "\\";
#else
    return "/";
#endif
}

void pdfswf_addlanguagedir(char*dir)
{
    if(!globalParams)
        globalParams = new GlobalParams("");
    
    msg("<notice> Adding %s to language pack directories", dir);

    int l;
    FILE*fi = 0;
    char* config_file = (char*)malloc(strlen(dir) + 1 + sizeof("add-to-xpdfrc"));
    strcpy(config_file, dir);
    strcat(config_file, dirseparator());
    strcat(config_file, "add-to-xpdfrc");

    fi = fopen(config_file, "rb");
    if(!fi) {
        msg("<error> Could not open %s", config_file);
        return;
    }
    globalParams->parseFile(new GString(config_file), fi);
    fclose(fi);
}

void pdfswf_addfontdir(char*dirname)
{
#ifdef HAVE_DIRENT_H
    msg("<notice> Adding %s to font directories", dirname);
    lastfontdir = strdup(dirname);
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
            strcat(fontname, dirseparator());
	    strcat(fontname, name);
	    msg("<verbose> Adding %s to fonts", fontname);
	    pdfswf_addfont(fontname);
	}
    }
    closedir(dir);
#else
    msg("<warning> No dirent.h- unable to add font dir %s", dir);
#endif
}


typedef struct _pdf_doc_internal
{
    int protect;
    PDFDoc*doc;
    InfoOutputDev*info;
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
    if(!globalParams)
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

    InfoOutputDev*io = new InfoOutputDev();
    int t;
    for(t=1;t<=pdf_doc->num_pages;t++) {
	i->doc->displayPage((OutputDev*)io, t, zoom, zoom, /*rotate*/0, /*usemediabox*/true, /*crop*/true, /*doLinks*/(int)1);
    }
    i->info = io;

    return pdf_doc;
}

class MemCheck
{
    public: ~MemCheck()
    {
        delete globalParams;globalParams=0;
        Object::memCheck(stderr);
        gMemReport(stderr);
    }
} myMemCheck;

void pdf_destroy(pdf_doc_t*pdf_doc)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)pdf_doc->internal;

    delete i->doc; i->doc=0;
    
    if(i->info) {
	delete i->info;i->info=0;
    }

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

void swf_output_setparameter(swf_output_t*swf, char*name, char*value)
{
    pdfswf_setparameter(name, value);
}

void swf_output_startframe(swf_output_t*swf, int width, int height)
{
    swf_output_internal_t*i= (swf_output_internal_t*)swf->internal;
    i->outputDev->startFrame(width, height);
}

void swf_output_endframe(swf_output_t*swf)
{
    swf_output_internal_t*i= (swf_output_internal_t*)swf->internal;
    i->outputDev->endframe();
}

void swf_output_preparepage(swf_output_t*swf, int pdfpage, int outputpage)
{
    swf_output_internal_t*i= (swf_output_internal_t*)swf->internal;
    SWFOutputDev*o = i->outputDev;

    if(pdfpage < 0)
	return;

    if(!o->pages) {
	o->pagebuflen = 1024;
	o->pages = (int*)malloc(o->pagebuflen*sizeof(int));
	memset(o->pages, -1, o->pagebuflen*sizeof(int));
    } else {
	while(pdfpage >= o->pagebuflen)
	{
	    int oldlen = o->pagebuflen;
	    o->pagebuflen+=1024;
	    o->pages = (int*)realloc(o->pages, o->pagebuflen*sizeof(int));
	    memset(&o->pages[oldlen], -1, (o->pagebuflen-oldlen)*sizeof(int));
	}
    }
    o->pages[pdfpage] = outputpage;
    if(pdfpage>o->pagepos)
	o->pagepos = pdfpage;
}

int swf_output_save(swf_output_t*swf, char*filename)
{
    swf_output_internal_t*i= (swf_output_internal_t*)swf->internal;
    int ret = i->outputDev->save(filename);
    return ret;
}

void* swf_output_get(swf_output_t*swf,char*name)
{
    swf_output_internal_t*i= (swf_output_internal_t*)swf->internal;
    void* ret = i->outputDev->get(name);
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

    if(!pi) {
	msg("<fatal> pdf_page_render: Parent PDF this page belongs to doesn't exist yet/anymore");
	return;
    }

    if(pi->protect) {
	gfxdevice_t*dev = si->outputDev->output;
        dev->setparameter(dev, "protect", "1");
    }
    si->outputDev->setInfo(pi->info);
    si->outputDev->setXRef(pi->doc, pi->doc->getXRef());
    pi->doc->displayPage((OutputDev*)si->outputDev, page->nr, zoom, zoom, /*rotate*/0, true, true, /*doLinks*/(int)1);
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
    
    pi->doc->displayPage((OutputDev*)output, page->nr, zoom, zoom, /*rotate*/0, true, true, /*doLinks*/(int)1);

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
