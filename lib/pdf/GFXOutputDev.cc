/* GFXOutputDev.cc
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
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "../../config.h"
#include "../os.h"
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
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
#include "GfxFont.h"
#include "GfxState.h"
#include "CharCodeToUnicode.h"
#include "NameToUnicodeTable.h"
#include "GlobalParams.h"
#include "FoFiType1C.h"
#include "FoFiTrueType.h"
#include "GHash.h"
#include "GFXOutputDev.h"

//  swftools header files
#include "../log.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../gfxfont.h"
#include "../devices/record.h"
#include "../devices/ops.h"
#include "../devices/arts.h"
#include "../devices/render.h"

#include "../art/libart.h"
#include "../devices/artsutils.h"

#include "../png.h"
#include "fonts.h"

#include <math.h>

typedef struct _fontfile
{
    const char*filename;
    int len; // basename length
    int used;
    struct _fontfile*next;
} fontfile_t;

// for pdfswf_addfont

static fontfile_t* global_fonts = 0;
static fontfile_t* global_fonts_next = 0;

static int fontnum = 0;

/* config */

static char* lastfontdir = 0;

struct fontentry {
    const char*pdffont;
    const char*filename;
    char*afm;
    int afmlen;
    char*pfb;
    int pfblen;
    char*fullfilename;
} pdf2t1map[] ={
{"Times-Roman",           "n021003l", n021003l_afm, n021003l_afm_len, n021003l_pfb, n021003l_pfb_len},
{"Times-Italic",          "n021023l", n021023l_afm, n021023l_afm_len, n021023l_pfb, n021023l_pfb_len},
{"Times-Bold",            "n021004l", n021004l_afm, n021004l_afm_len, n021004l_pfb, n021004l_pfb_len},
{"Times-BoldItalic",      "n021024l", n021024l_afm, n021024l_afm_len, n021024l_pfb, n021024l_pfb_len},
{"Helvetica",             "n019003l", n019003l_afm, n019003l_afm_len, n019003l_pfb, n019003l_pfb_len},
{"Helvetica-Oblique",     "n019023l", n019023l_afm, n019023l_afm_len, n019023l_pfb, n019023l_pfb_len},
{"Helvetica-Bold",        "n019004l", n019004l_afm, n019004l_afm_len, n019004l_pfb, n019004l_pfb_len},
{"Helvetica-BoldOblique", "n019024l", n019024l_afm, n019024l_afm_len, n019024l_pfb, n019024l_pfb_len},
{"Courier",               "n022003l", n022003l_afm, n022003l_afm_len, n022003l_pfb, n022003l_pfb_len},
{"Courier-Oblique",       "n022023l", n022023l_afm, n022023l_afm_len, n022023l_pfb, n022023l_pfb_len},
{"Courier-Bold",          "n022004l", n022004l_afm, n022004l_afm_len, n022004l_pfb, n022004l_pfb_len},
{"Courier-BoldOblique",   "n022024l", n022024l_afm, n022024l_afm_len, n022024l_pfb, n022024l_pfb_len},
{"Symbol",                "s050000l", s050000l_afm, s050000l_afm_len, s050000l_pfb, s050000l_pfb_len},
{"ZapfDingbats",          "d050000l", d050000l_afm, d050000l_afm_len, d050000l_pfb, d050000l_pfb_len}};


static int verbose = 0;
static int dbgindent = 0;
static void dbg(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(!verbose)
	return;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    printf("(pdf) ");
    int indent = dbgindent;
    while(indent) {
	printf(" ");
	indent--;
    }
    printf("%s\n", buf);
    fflush(stdout);
}


typedef struct _feature
{
    char*string;
    struct _feature*next;
} feature_t;
feature_t*featurewarnings = 0;

void GFXOutputDev::showfeature(const char*feature,char fully, char warn)
{
    feature_t*f = featurewarnings;
    while(f) {
	if(!strcmp(feature, f->string))
	    return;
	f = f->next;
    }
    f = (feature_t*)malloc(sizeof(feature_t));
    f->string = strdup(feature);
    f->next = featurewarnings;
    featurewarnings = f;
    if(warn) {
	msg("<warning> %s not yet %ssupported!",feature,fully?"fully ":"");
	if(this->config_break_on_warning) {
	    msg("<fatal> Aborting conversion due to unsupported feature");
	    exit(1);
	}
    } else {
	msg("<notice> File contains %s",feature);
    }
}
void GFXOutputDev::warnfeature(const char*feature,char fully)
{
    showfeature(feature,fully,1);
}
void GFXOutputDev::infofeature(const char*feature)
{
    showfeature(feature,0,0);
}

GFXOutputState::GFXOutputState() {
    this->clipping = 0;
    this->createsoftmask = 0;
    this->transparencygroup = 0;
    this->softmask = 0;
    this->grouprecording = 0;
    this->isolated = 0;
}

GBool GFXOutputDev::interpretType3Chars() 
{
    return gTrue;
}

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
    
char* writeOutStdFont(fontentry* f)
{
    FILE*fi;
    char namebuf1[512];
    char namebuf2[512];
    char* tmpFileName = mktmpname(namebuf1);

    sprintf(namebuf2, "%s.afm", tmpFileName);
    fi = fopen(namebuf2, "wb");
    if(!fi)
        return 0;
    fwrite(f->afm, 1, f->afmlen, fi);
    fclose(fi);

    sprintf(namebuf2, "%s.pfb", tmpFileName);
    fi = fopen(namebuf2, "wb");
    if(!fi)
        return 0;
    fwrite(f->pfb, 1, f->pfblen, fi);
    fclose(fi);
    return strdup(namebuf2);
}
void unlinkfont(char* filename)
{
    int l;
    if(!filename)
	return;
    msg("<verbose> Removing temporary font file %s", filename);
    l=strlen(filename);
    unlink(filename);
    if(!strncmp(&filename[l-4],".afm",4)) {
	memcpy(&filename[l-4],".pfb",4); unlink(filename);
	memcpy(&filename[l-4],".pfa",4); unlink(filename);
	memcpy(&filename[l-4],".afm",4);
	return;
    } else 
    if(!strncmp(&filename[l-4],".pfa",4)) {
	memcpy(&filename[l-4],".afm",4); unlink(filename);
	memcpy(&filename[l-4],".pfa",4);
	return;
    } else 
    if(!strncmp(&filename[l-4],".pfb",4)) {
	memcpy(&filename[l-4],".afm",4); unlink(filename);
	memcpy(&filename[l-4],".pfb",4);
	return;
    }
}


GFXGlobalParams::GFXGlobalParams()
: GlobalParams("")
{
    //setupBaseFonts(char *dir); //not tested yet
}
GFXGlobalParams::~GFXGlobalParams()
{
    msg("<verbose> Performing cleanups");
    int t;
    for(t=0;t<sizeof(pdf2t1map)/sizeof(fontentry);t++) {
	if(pdf2t1map[t].fullfilename) {
	    unlinkfont(pdf2t1map[t].fullfilename);
	}
    }
}
DisplayFontParam *GFXGlobalParams::getDisplayFont(GString *fontName)
{
    msg("<verbose> looking for font %s in global params\n", fontName->getCString());

    char*name = fontName->getCString();
    /* see if it is a pdf standard font */
    int t;
    for(t=0;t<sizeof(pdf2t1map)/sizeof(fontentry);t++) {
	if(!strcmp(name, pdf2t1map[t].pdffont)) {
	    if(!pdf2t1map[t].fullfilename) {
		pdf2t1map[t].fullfilename = writeOutStdFont(&pdf2t1map[t]);
		if(!pdf2t1map[t].fullfilename) {
		    msg("<error> Couldn't save default font- is the Temp Directory writable?");
		} else {
		    msg("<verbose> Storing standard PDF font %s at %s", name, pdf2t1map[t].fullfilename);
		}
	    }
	    DisplayFontParam *dfp = new DisplayFontParam(new GString(fontName), displayFontT1);
	    dfp->t1.fileName = new GString(pdf2t1map[t].fullfilename);
	    return dfp;
	}
    }
    
    int bestlen = 0x7fffffff;
    const char*bestfilename = 0;
    
    fontfile_t*f = 0;
    while(f) {
	if(strstr(f->filename, name)) {
            if(f->len < bestlen) {
                bestlen = f->len;
                bestfilename = f->filename;
            }
        }
        f = f->next;
    }
    if(bestfilename) {
        DisplayFontParam *dfp = new DisplayFontParam(new GString(fontName), displayFontT1);
        dfp->t1.fileName = new GString(bestfilename);
        return dfp;
    }
    return GlobalParams::getDisplayFont(fontName);
}

GFXOutputDev::GFXOutputDev(InfoOutputDev*info, PDFDoc*doc)
{
    this->info = info;
    this->doc = doc;
    this->xref = doc->getXRef();

    this->jpeginfo = 0;
    this->textmodeinfo = 0;
    this->linkinfo = 0;
    this->pbminfo = 0;
    this->type3active = 0;
    this->statepos = 0;
    this->xref = 0;
    this->substitutepos = 0;
    this->type3Warning = 0;
    this->user_movex = 0;
    this->user_movey = 0;
    this->clipmovex = 0;
    this->clipmovey = 0;
    this->user_clipx1 = 0;
    this->user_clipy1 = 0;
    this->user_clipx2 = 0;
    this->user_clipy2 = 0;
    this->current_text_stroke = 0;
    this->current_text_clip = 0;
    this->outer_clip_box = 0;
    this->pages = 0;
    this->pagebuflen = 0;
    this->pagepos = 0;
    this->config_break_on_warning=0;
    this->config_remapunicode=0;
    this->config_transparent=0;
    this->config_extrafontdata = 0;
    this->config_fontquality = 10;

    this->gfxfontlist = gfxfontlist_create();
  
    memset(states, 0, sizeof(states));
};

void GFXOutputDev::setParameter(const char*key, const char*value)
{
    if(!strcmp(key,"breakonwarning")) {
	this->config_break_on_warning = atoi(value);
    } else if(!strcmp(key,"remapunicode")) {
        this->config_remapunicode = atoi(value);
    } else if(!strcmp(key,"transparent")) {
        this->config_transparent = atoi(value);
    } else if(!strcmp(key,"extrafontdata")) {
        this->config_extrafontdata = atoi(value);
    } else if(!strcmp(key,"fontquality")) {
        this->config_fontquality = atof(value);
	if(this->config_fontquality<=1)
	    this->config_fontquality=1;
    } else if(!strcmp(key,"help")) {
	printf("\nPDF layer options:\n");
	printf("breakonwarning=0/1  Abort conversion if graphic objects are found which\n");
	printf("                    are not 100%% supported\n");
	printf("transparent=0/1     Make PDF transparent (alpha background)\n");
	printf("extrafontdata=0/1   Store Type3 characters and capture characters\n");
	printf("fontquality=1..100  Curve approximation quality of the fonts\n");
    }
    
}
  
void GFXOutputDev::setDevice(gfxdevice_t*dev)
{
    this->device = dev;
}
  
void GFXOutputDev::setMove(int x,int y)
{
    this->user_movex = x;
    this->user_movey = y;
}

void GFXOutputDev::setClip(int x1,int y1,int x2,int y2)
{
    if(x2<x1) {int x3=x1;x1=x2;x2=x3;}
    if(y2<y1) {int y3=y1;y1=y2;y2=y3;}

    this->user_clipx1 = x1;
    this->user_clipy1 = y1;
    this->user_clipx2 = x2;
    this->user_clipy2 = y2;
}

static char*getFontName(GfxFont*font)
{
    char*fontid;
    GString*gstr = font->getName();
    char* fname = gstr==0?0:gstr->getCString();
    if(fname==0) {
	char buf[32];
	Ref*r=font->getID();
	sprintf(buf, "UFONT%d", r->num);
	fontid = strdup(buf);
    } else {
	fontid = strdup(fname);
    }

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

static void dumpFontInfo(const char*loglevel, GfxFont*font);
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
      msg("<warning> The following Type 3 Font will be rendered as graphics:");
    dumpFontInfo("<warning>", font);
}

static void dumpFontInfo(const char*loglevel, GfxFont*font)
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

//void GFXOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) {printf("void GFXOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) \n");}
//void GFXOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) {printf("void GFXOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) \n");}

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

    gfxline_optimize(result);

    return result;
}

GBool GFXOutputDev::useTilingPatternFill()
{
    infofeature("tiled patterns");
    return gFalse;
}

GBool GFXOutputDev::useShadedFills()
{
    infofeature("shaded fills");
    return gFalse;
}
  
GBool GFXOutputDev::useDrawForm() 
{ 
    infofeature("forms");
    return gFalse; 
}
void GFXOutputDev::drawForm(Ref id) 
{
    msg("<error> drawForm not implemented");
}
GBool GFXOutputDev::needNonText() 
{ 
    return gTrue; 
}
void GFXOutputDev::endPage() 
{
    msg("<verbose> endPage");
    if(outer_clip_box) {
	device->endclip(device);
	outer_clip_box = 0;
    }
}

#define STROKE_FILL 1
#define STROKE_CLIP 2
void GFXOutputDev::strokeGfxline(GfxState *state, gfxline_t*line, int flags)
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
	msg("<trace> %d dashes", dashnum);
	msg("<trace> |  phase: %f", dashphase);
	for(t=0;t<dashnum;t++) {
	    dash[t] = (float)ldash[t];
	    msg("<trace> |  d%-3d: %f", t, ldash[t]);
	}
	dash[dashnum] = -1;
	if(getLogLevel() >= LOGLEVEL_TRACE) {
	    dump_outline(line);
	}

	line2 = gfxtool_dash_line(line, dash, (float)dashphase);
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

    if(flags&STROKE_FILL) {
        ArtSVP* svp = gfxstrokeToSVP(line, width, capType, joinType, miterLimit);
        gfxline_t*gfxline = SVPtogfxline(svp);
	if(getLogLevel() >= LOGLEVEL_TRACE)  {
	    dump_outline(gfxline);
	}
	if(!gfxline) {
	    msg("<warning> Empty polygon (resulting from stroked line)");
	}
        if(flags&STROKE_CLIP) {
            device->startclip(device, gfxline);
            states[statepos].clipping++;
        } else {
            device->fill(device, gfxline, &col);
        }
        free(gfxline);
	art_svp_free(svp);
    } else {
        if(flags&STROKE_CLIP) 
            msg("<error> Stroke&clip not supported at the same time");
        device->stroke(device, line, width, &col, capType, joinType, miterLimit);
    }
    
    if(line2)
	gfxline_free(line2);
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

void GFXOutputDev::fillGfxLine(GfxState *state, gfxline_t*line) 
{
    gfxcolor_t col = getFillColor(state);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> fill %02x%02x%02x%02x\n", col.r, col.g, col.b, col.a);
        dump_outline(line);
    }
    device->fill(device, line, &col);
}

void GFXOutputDev::clipToGfxLine(GfxState *state, gfxline_t*line) 
{
    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> clip\n");
        dump_outline(line);
    }

    device->startclip(device, line);
    states[statepos].clipping++;
}

void GFXOutputDev::clip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    gfxline_t*line = gfxPath_to_gfxline(state, path, 1, user_movex + clipmovex, user_movey + clipmovey);
    clipToGfxLine(state, line);
    gfxline_free(line);
}

void GFXOutputDev::eoClip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    gfxline_t*line = gfxPath_to_gfxline(state, path, 1, user_movex + clipmovex, user_movey + clipmovey);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> eoclip\n");
        dump_outline(line);
    }

    device->startclip(device, line);
    states[statepos].clipping++;
    gfxline_free(line);
}
void GFXOutputDev::clipToStrokePath(GfxState *state)
{
    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 0, user_movex + clipmovex, user_movey + clipmovey);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> cliptostrokepath\n");
        dump_outline(line);
    }

    strokeGfxline(state, line, STROKE_FILL|STROKE_CLIP);
    gfxline_free(line);
}

void GFXOutputDev::finish()
{
    if(outer_clip_box) {
	if(device) {
	    device->endclip(device);
	}
	outer_clip_box = 0;
    }
}

GFXOutputDev::~GFXOutputDev() 
{
    finish();

    if(this->pages) {
	free(this->pages); this->pages = 0;
    }

    gfxfontlist_free(this->gfxfontlist, 1);this->gfxfontlist = 0;
};
GBool GFXOutputDev::upsideDown() 
{
    return gTrue;
};
GBool GFXOutputDev::useDrawChar() 
{
    return gTrue;
}

const char*renderModeDesc[]= {"fill", "stroke", "fill+stroke", "invisible",
                      "clip+fill", "stroke+clip", "fill+stroke+clip", "clip"};

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
#define INTERNAL_FONT_SIZE 1024.0
void GFXOutputDev::updateFontMatrix(GfxState*state)
{
    double* ctm = state->getCTM();
    double fontSize = state->getFontSize();
    double*textMat = state->getTextMat();

    /*  taking the absolute value of horizScaling seems to be required for
	some italic fonts. FIXME: SplashOutputDev doesn't need this- why? */
    double hscale = fabs(state->getHorizScaling());
   
    // from xpdf-3.02/SplashOutputDev:updateFont
    double mm11 = textMat[0] * fontSize * hscale;
    double mm12 = textMat[1] * fontSize * hscale;
    double mm21 = textMat[2] * fontSize;
    double mm22 = textMat[3] * fontSize;

    // multiply with ctm, like state->getFontTransMat() does
    this->current_font_matrix.m00 = (ctm[0]*mm11 + ctm[2]*mm12) / INTERNAL_FONT_SIZE;
    this->current_font_matrix.m01 = (ctm[1]*mm11 + ctm[3]*mm12) / INTERNAL_FONT_SIZE;
    this->current_font_matrix.m10 = (ctm[0]*mm21 + ctm[2]*mm22) / INTERNAL_FONT_SIZE;
    this->current_font_matrix.m11 = (ctm[1]*mm21 + ctm[3]*mm22) / INTERNAL_FONT_SIZE;
    this->current_font_matrix.tx = 0;
    this->current_font_matrix.ty = 0;
}

void GFXOutputDev::beginString(GfxState *state, GString *s) 
{ 
    int render = state->getRender();
    if(current_text_stroke) {
	msg("<error> Error: Incompatible change of text rendering to %d while inside cliptext", render);
    }

    msg("<trace> beginString(%s) render=%d", makeStringPrintable(s->getCString()), render);
}

static gfxline_t* mkEmptyGfxShape(double x, double y)
{
    gfxline_t*line = (gfxline_t*)malloc(sizeof(gfxline_t));
    line->x = x;line->y = y;line->type = gfx_moveTo;line->next = 0;
    return line;
}

static char isValidUnicode(int c)
{
    if(c>=32 && c<0x2fffe)
	return 1;
    return 0;
}

void GFXOutputDev::drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode charid, int nBytes, Unicode *_u, int uLen)
{
    if(!current_fontinfo || (unsigned)charid >= current_fontinfo->num_glyphs || !current_fontinfo->glyphs[charid]) {
	msg("<error> Invalid charid %d for font (%d characters)", charid, current_fontinfo?current_fontinfo->num_glyphs:0);
	return;
    }
  
    CharCode glyphid = current_fontinfo->glyphs[charid]->glyphid;

    int render = state->getRender();
    gfxcolor_t col = getFillColor(state);

    // check for invisible text -- this is used by Acrobat Capture
    if (render == RENDER_INVISIBLE) {
	col.a = 0;
	if(!config_extrafontdata)
	    return;
    }

    GfxFont*font = state->getFont();

    if(font->getType() == fontType3) {
	/* type 3 chars are passed as graphics */
	msg("<debug> type3 char at %f/%f", x, y);
	return;
    }

    Unicode u = uLen?(_u[0]):0;
    msg("<debug> drawChar(%f,%f,c='%c' (%d), u=%d <%d>) CID=%d render=%d glyphid=%d\n",x,y,(charid&127)>=32?charid:'?', charid, u, uLen, font->isCIDFont(), render, glyphid);

    gfxmatrix_t m = this->current_font_matrix;
    state->transform(x, y, &m.tx, &m.ty);
    m.tx += user_movex + clipmovex;
    m.ty += user_movey + clipmovey;

    if(render == RENDER_FILL || render == RENDER_INVISIBLE) {
	device->drawchar(device, current_gfxfont, glyphid, &col, &m);
    } else {
	msg("<debug> Drawing glyph %d as shape", charid);
	if(!textmodeinfo) {
	    msg("<notice> Some texts will be rendered as shape");
	    textmodeinfo = 1;
	}
	gfxline_t*glyph = current_gfxfont->glyphs[glyphid].line;
	gfxline_t*tglyph = gfxline_clone(glyph);
	gfxline_transform(tglyph, &m);
	if((render&3) != RENDER_INVISIBLE) {
	    gfxline_t*add = gfxline_clone(tglyph);
	    current_text_stroke = gfxline_append(current_text_stroke, add);
	}
	if(render&RENDER_CLIP) {
	    gfxline_t*add = gfxline_clone(tglyph);
	    current_text_clip = gfxline_append(current_text_clip, add);
	    if(!current_text_clip) {
		current_text_clip = mkEmptyGfxShape(m.tx, m.ty);
	    }
	}
	gfxline_free(tglyph);
    }
}

void GFXOutputDev::endString(GfxState *state) 
{ 
    int render = state->getRender();
    msg("<trace> endString() render=%d textstroke=%08x", render, current_text_stroke);
    
    if(current_text_stroke) {
	/* fillstroke and stroke text rendering objects we can process right
	   now (as there may be texts of other rendering modes in this
	   text object)- clipping objects have to wait until endTextObject,
	   however */
	device->setparameter(device, "mark","TXT");
	if((render&3) == RENDER_FILL) {
	    fillGfxLine(state, current_text_stroke);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	} else if((render&3) == RENDER_FILLSTROKE) {
	    fillGfxLine(state, current_text_stroke);
	    strokeGfxline(state, current_text_stroke,0);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	} else if((render&3) == RENDER_STROKE) {
	    strokeGfxline(state, current_text_stroke,0);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	}
	device->setparameter(device, "mark","");
    }
}    

void GFXOutputDev::endTextObject(GfxState *state)
{
    int render = state->getRender();
    msg("<trace> endTextObject() render=%d textstroke=%08x clipstroke=%08x", render, current_text_stroke, current_text_clip);
    
    if(current_text_clip) {
	device->setparameter(device, "mark","TXT");
	clipToGfxLine(state, current_text_clip);
	device->setparameter(device, "mark","");
	gfxline_free(current_text_clip);
	current_text_clip = 0;
    }
}

/* the logic seems to be as following:
   first, beginType3Char is called, with the charcode and the coordinates.
   if this function returns true, it already knew about the char and has now drawn it.
   if the function returns false, it's a new char, and type3D0 and/or type3D1 might be 
   called with some parameters.
   Afterwards, all draw operations until endType3Char are part of the char (which in this moment is
   at the position first passed to beginType3Char). the char ends with endType3Char.

   The drawing operations between beginType3Char and endType3Char are somewhat different to
   the normal ones. For example, the fillcolor equals the stroke color. (Because the stroke
   color determines the color of a font)
*/

GBool GFXOutputDev::beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode charid, Unicode *u, int uLen)
{
    msg("<debug> beginType3Char %d u=%d", charid, uLen?u[0]:0);
    type3active = 1;
    
    if(config_extrafontdata && current_fontinfo) {

	gfxmatrix_t m = this->current_font_matrix;
	state->transform(0, 0, &m.tx, &m.ty);
	m.m00*=INTERNAL_FONT_SIZE;
	m.m01*=INTERNAL_FONT_SIZE;
	m.m10*=INTERNAL_FONT_SIZE;
	m.m11*=INTERNAL_FONT_SIZE;
	m.tx += user_movex + clipmovex;
	m.ty += user_movey + clipmovey;

	if(!current_fontinfo || (unsigned)charid >= current_fontinfo->num_glyphs || !current_fontinfo->glyphs[charid]) {
	    msg("<error> Invalid charid %d for font", charid);
	    return gFalse;
	}
	gfxcolor_t col={0,0,0,0};
	CharCode glyphid = current_fontinfo->glyphs[charid]->glyphid;
	device->drawchar(device, current_gfxfont, glyphid, &col, &m);
    }


    /* the character itself is going to be passed using the draw functions */
    return gFalse; /* gTrue= is_in_cache? */
}

void GFXOutputDev::type3D0(GfxState *state, double wx, double wy) {
}
void GFXOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury) {
}

void GFXOutputDev::endType3Char(GfxState *state)
{
    type3active = 0;
    msg("<debug> endType3Char");
}

void GFXOutputDev::startPage(int pageNum, GfxState *state, double crop_x1, double crop_y1, double crop_x2, double crop_y2) 
{
    this->currentpage = pageNum;
    double x1,y1,x2,y2;
    int rot = doc->getPageRotate(1);
    gfxcolor_t white = {255,255,255,255};
    gfxcolor_t black = {255,0,0,0};
    laststate = state;
    gfxline_t clippath[5];

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

    this->clipmovex = -(int)x1;
    this->clipmovey = -(int)y1;
    
    /* apply user clip box */
    if(user_clipx1|user_clipy1|user_clipx2|user_clipy2) {
        /*if(user_clipx1 > x1)*/ x1 = user_clipx1;
        /*if(user_clipx2 < x2)*/ x2 = user_clipx2;
        /*if(user_clipy1 > y1)*/ y1 = user_clipy1;
        /*if(user_clipy2 < y2)*/ y2 = user_clipy2;
	msg("<verbose> Using user clip box %f/%f/%f/%f",x1,y1,x2,y2);
    }

    //msg("<verbose> Bounding box is (%f,%f)-(%f,%f) [shifted by %d/%d]", x1,y1,x2,y2, user_movex, user_movey);
    
    msg("<notice> processing PDF page %d (%dx%d:%d:%d) (move:%d:%d)", pageNum, (int)x2-(int)x1,(int)y2-(int)y1, (int)x1, (int)y1, user_movex + clipmovex, user_movey + clipmovey);
    if(rot!=0)
        msg("<verbose> page is rotated %d degrees\n", rot);

    clippath[0].type = gfx_moveTo;clippath[0].x = x1; clippath[0].y = y1; clippath[0].next = &clippath[1];
    clippath[1].type = gfx_lineTo;clippath[1].x = x2; clippath[1].y = y1; clippath[1].next = &clippath[2];
    clippath[2].type = gfx_lineTo;clippath[2].x = x2; clippath[2].y = y2; clippath[2].next = &clippath[3];
    clippath[3].type = gfx_lineTo;clippath[3].x = x1; clippath[3].y = y2; clippath[3].next = &clippath[4];
    clippath[4].type = gfx_lineTo;clippath[4].x = x1; clippath[4].y = y1; clippath[4].next = 0;
    device->startclip(device, clippath); outer_clip_box = 1;
    if(!config_transparent) {
        device->fill(device, clippath, &white);
    }
}


void GFXOutputDev::processLink(Link *link, Catalog *catalog)
{
    double x1, y1, x2, y2;
    gfxline_t points[5];
    int x, y;
    
    msg("<debug> drawlink\n");

    link->getRect(&x1, &y1, &x2, &y2);
    cvtUserToDev(x1, y1, &x, &y);
    points[0].type = gfx_moveTo;
    points[0].x = points[4].x = x + user_movex + clipmovex;
    points[0].y = points[4].y = y + user_movey + clipmovey;
    points[0].next = &points[1];
    cvtUserToDev(x2, y1, &x, &y);
    points[1].type = gfx_lineTo;
    points[1].x = x + user_movex + clipmovex;
    points[1].y = y + user_movey + clipmovey;
    points[1].next = &points[2];
    cvtUserToDev(x2, y2, &x, &y);
    points[2].type = gfx_lineTo;
    points[2].x = x + user_movex + clipmovex;
    points[2].y = y + user_movey + clipmovey;
    points[2].next = &points[3];
    cvtUserToDev(x1, y2, &x, &y);
    points[3].type = gfx_lineTo;
    points[3].x = x + user_movex + clipmovex;
    points[3].y = y + user_movey + clipmovey;
    points[3].next = &points[4];
    cvtUserToDev(x1, y1, &x, &y);
    points[4].type = gfx_lineTo;
    points[4].x = x + user_movex + clipmovex;
    points[4].y = y + user_movey + clipmovey;
    points[4].next = 0;
    
    msg("<trace> drawlink %.2f/%.2f %.2f/%.2f %.2f/%.2f %.2f/%.2f\n",
	    points[0].x, points[0].y,
	    points[1].x, points[1].y,
	    points[2].x, points[2].y,
	    points[3].x, points[3].y); 

    LinkAction*action=link->getAction();
    char buf[128];
    char*s = 0;
    const char*type = "-?-";
    char*named = 0;
    int page = -1;
    msg("<trace> drawlink action=%d\n", action->getKind());
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
	    GString*g = l->getFileName();
	    if(g)
             s = strdup(g->getCString());
	    if(!s) {
		/* if the GoToR link has no filename, then
		   try to find a refernce in the *local*
		   file */
		GString*g = l->getNamedDest();
		if(g)
		 s = strdup(g->getCString());
	    }
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
	    GString * params = l->getParams();
	    if(params)
		str->append(params);
            s = strdup(str->getCString());
            delete str;
        }
        break;
        case actionURI: {
	    char*url = 0;
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
    
    msg("<trace> drawlink s=%s\n", s);

    if(!linkinfo && (page || s))
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
	device->drawlink(device, points, buf);
    }
    else if(s)
    {
        device->drawlink(device, points, s);
    }

    msg("<verbose> \"%s\" link to \"%s\" (%d)\n", type, FIXNULL(s), page);
    free(s);s=0;
}

void GFXOutputDev::saveState(GfxState *state) {
    dbg("saveState");dbgindent+=2;

    msg("<trace> saveState\n");
    updateAll(state);
    if(statepos>=64) {
      msg("<error> Too many nested states in pdf.");
      return;
    }
    statepos ++;
    states[statepos].createsoftmask = states[statepos-1].createsoftmask;
    states[statepos].transparencygroup = states[statepos-1].transparencygroup;
    states[statepos].clipping = 0;
};

void GFXOutputDev::restoreState(GfxState *state) {
  dbgindent-=2; dbg("restoreState");

  if(statepos==0) {
      msg("<error> Invalid restoreState");
      return;
  }
  msg("<trace> restoreState%s%s", states[statepos].softmask?" (end softmask)":"",
	                          states[statepos].clipping?" (end clipping)":"");
  if(states[statepos].softmask) {
      clearSoftMask(state);
  }
  updateAll(state);
  while(states[statepos].clipping) {
      device->endclip(device);
      states[statepos].clipping--;
  }
  statepos--;
}

void GFXOutputDev::updateLineWidth(GfxState *state)
{
    double width = state->getTransformedLineWidth();
    //swfoutput_setlinewidth(&device, width);
}

void GFXOutputDev::updateLineCap(GfxState *state)
{
    int c = state->getLineCap();
}

void GFXOutputDev::updateLineJoin(GfxState *state)
{
    int j = state->getLineJoin();
}

void GFXOutputDev::updateFillColor(GfxState *state) 
{
    GfxRGB rgb;
    double opaq = state->getFillOpacity();
    state->getFillRGB(&rgb);
}
void GFXOutputDev::updateFillOpacity(GfxState *state)
{
    GfxRGB rgb;
    double opaq = state->getFillOpacity();
    state->getFillRGB(&rgb);
    dbg("update fillopaq %f", opaq);
}
void GFXOutputDev::updateStrokeOpacity(GfxState *state)
{
    double opaq = state->getFillOpacity();
    dbg("update strokeopaq %f", opaq);
}
void GFXOutputDev::updateFillOverprint(GfxState *state)
{
    double opaq = state->getFillOverprint();
    dbg("update filloverprint %f", opaq);
}
void GFXOutputDev::updateStrokeOverprint(GfxState *state)
{
    double opaq = state->getStrokeOverprint();
    dbg("update strokeoverprint %f", opaq);
}
void GFXOutputDev::updateTransfer(GfxState *state)
{
    dbg("update transfer");
}


void GFXOutputDev::updateStrokeColor(GfxState *state) 
{
    GfxRGB rgb;
    double opaq = state->getStrokeOpacity();
    state->getStrokeRGB(&rgb);
}


gfxfont_t* createGfxFont(GfxFont*xpdffont, FontInfo*src, double config_fontquality)
{
    gfxfont_t*font = (gfxfont_t*)malloc(sizeof(gfxfont_t));
    memset(font, 0, sizeof(gfxfont_t));

    font->glyphs = (gfxglyph_t*)malloc(sizeof(gfxglyph_t)*src->num_glyphs);
    memset(font->glyphs, 0, sizeof(gfxglyph_t)*src->num_glyphs);
    font->id = strdup(getFontID(xpdffont));
    int t;
    
    double quality = (INTERNAL_FONT_SIZE * 200 / config_fontquality) / src->max_size;
    double scale = 1;
    //printf("%d glyphs\n", font->num_glyphs);
    font->num_glyphs = 0;
    for(t=0;t<src->num_glyphs;t++) {
	if(src->glyphs[t]) {
	    SplashPath*path = src->glyphs[t]->path;
	    int len = path?path->getLength():0;
	    //printf("glyph %d) %08x (%d line segments)\n", t, path, len);
	    gfxglyph_t*glyph = &font->glyphs[font->num_glyphs];
	    src->glyphs[t]->glyphid = font->num_glyphs;
	    glyph->unicode = src->glyphs[t]->unicode;
	    if(glyph->unicode >= font->max_unicode)
		font->max_unicode = glyph->unicode+1;
	    gfxdrawer_t drawer;
	    gfxdrawer_target_gfxline(&drawer);
	    int s;
	    int count = 0;
	    double xmax = 0;
	    for(s=0;s<len;s++) {
		Guchar f;
		double x, y;
		path->getPoint(s, &x, &y, &f);
		if(!s || x > xmax)
		    xmax = x;
		if(f&splashPathFirst) {
		    drawer.moveTo(&drawer, x*scale, y*scale);
		}
		if(f&splashPathCurve) {
		    double x2,y2;
		    path->getPoint(++s, &x2, &y2, &f);
		    if(f&splashPathCurve) {
			double x3,y3;
			path->getPoint(++s, &x3, &y3, &f);
			gfxdraw_cubicTo(&drawer, x*scale, y*scale, x2*scale, y2*scale, x3*scale, y3*scale, quality);
		    } else {
			drawer.splineTo(&drawer, x*scale, y*scale, x2*scale, y2*scale);
		    }
		} else {
		    drawer.lineTo(&drawer, x*scale, y*scale);
		}
	     //   printf("%f %f %s %s\n", x, y, (f&splashPathCurve)?"curve":"",
	     //       			  (f&splashPathFirst)?"first":"",
	     //       			  (f&splashPathLast)?"last":"");
	    }
	    glyph->line = (gfxline_t*)drawer.result(&drawer);
	    glyph->advance = xmax*scale; // we don't know the real advance value, so this'll have to do
	    font->num_glyphs++;
	}
    }
    font->unicode2glyph = (int*)malloc(sizeof(int)*font->max_unicode);
    memset(font->unicode2glyph, -1, sizeof(int)*font->max_unicode);
    for(t=0;t<font->num_glyphs;t++) {
	if(font->glyphs[t].unicode>0 && font->glyphs[t].unicode<font->max_unicode) {
	    font->unicode2glyph[font->glyphs[t].unicode] = t;
	}

    }
    msg("<trace> %d glyphs.", t, font->num_glyphs);
    return font;
}

void GFXOutputDev::updateFont(GfxState *state) 
{
    GfxFont* gfxFont = state->getFont();
    if (!gfxFont) {
	return; 
    }  
    char*id = getFontID(gfxFont);
    msg("<verbose> Updating font to %s", id);
    if(gfxFont->getType() == fontType3) {
	infofeature("Type3 fonts");
	if(!config_extrafontdata) {
	    return;
	}
    }
    if(!id) {
	msg("<error> Internal Error: FontID is null");
	return; 
    }

    this->current_fontinfo = this->info->getFont(id);
    if(!this->current_fontinfo) {
	msg("<error> Internal Error: no fontinfo for font %s\n", id);
	return;
    }
    if(!this->current_fontinfo->seen) {
	dumpFontInfo("<verbose>", gfxFont);
    }
    
    gfxfont_t*font = gfxfontlist_findfont(this->gfxfontlist,id);
    if(!font) {
	font = createGfxFont(gfxFont, current_fontinfo, this->config_fontquality);
        font->id = strdup(id);
	this->gfxfontlist = gfxfontlist_addfont(this->gfxfontlist, font);
	device->addfont(device, font);
    }
    current_gfxfont = font;
    free(id);

    updateFontMatrix(state);
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


void GFXOutputDev::drawGeneralImage(GfxState *state, Object *ref, Stream *str,
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
      maskStr->close();
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

  state->transform(0, 1, &x1, &y1); x1 += user_movex + clipmovex; y1 += user_movey + clipmovey;
  state->transform(0, 0, &x2, &y2); x2 += user_movex + clipmovex; y2 += user_movey + clipmovey;
  state->transform(1, 0, &x3, &y3); x3 += user_movex + clipmovex; y3 += user_movey + clipmovey;
  state->transform(1, 1, &x4, &y4); x4 += user_movex + clipmovex; y4 += user_movey + clipmovey;

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
      int found = -1;
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

	  float r = 255./(float)(numpalette-1);
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
      drawimagelossless(device, pic2, width, height, x1,y1,x2,y2,x3,y3,x4,y4);
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
	  drawimagejpeg(device, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4);
      else
	  drawimagelossless(device, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4);
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
      drawimagelossless(device, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4);

      delete pic;
      delete imgStr;
      if(maskbitmap) free(maskbitmap);
      return;
  }
}

void GFXOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool inlineImg) 
{
    dbg("drawImageMask %dx%d, invert=%d inline=%d", width, height, invert, inlineImg);
    msg("<verbose> drawImageMask %dx%d, invert=%d inline=%d", width, height, invert, inlineImg);
    drawGeneralImage(state,ref,str,width,height,0,invert,inlineImg,1, 0, 0,0,0,0, 0);
}

void GFXOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg)
{
    dbg("drawImage %dx%d, %s, %s, inline=%d", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskColors?"maskColors":"no maskColors",
	    inlineImg);
    msg("<verbose> drawImage %dx%d, %s, %s, inline=%d", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskColors?"maskColors":"no maskColors",
	    inlineImg);
    if(colorMap)
	msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d\n", colorMap->getNumPixelComps(),
		colorMap->getBits(),colorMap->getColorSpace()->getMode());
    drawGeneralImage(state,ref,str,width,height,colorMap,0,inlineImg,0,maskColors, 0,0,0,0, 0);
}
  
void GFXOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       Stream *maskStr, int maskWidth, int maskHeight,
			       GBool maskInvert)
{
    dbg("drawMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskWidth, maskHeight);
    msg("<verbose> drawMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskWidth, maskHeight);
    if(colorMap)
	msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d\n", colorMap->getNumPixelComps(),
		colorMap->getBits(),colorMap->getColorSpace()->getMode());
    drawGeneralImage(state,ref,str,width,height,colorMap,0,0,0,0, maskStr, maskWidth, maskHeight, maskInvert, 0);
}

void GFXOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap)
{
    dbg("drawSoftMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskWidth, maskHeight);
    msg("<verbose> drawSoftMaskedImage %dx%d, %s, %dx%d mask", width, height, 
	    colorMap?"colorMap":"no colorMap", 
	    maskWidth, maskHeight);
    if(colorMap)
	msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d\n", colorMap->getNumPixelComps(),
		colorMap->getBits(),colorMap->getColorSpace()->getMode());
    drawGeneralImage(state,ref,str,width,height,colorMap,0,0,0,0, maskStr, maskWidth, maskHeight, 0, maskColorMap);
}

void GFXOutputDev::stroke(GfxState *state) 
{
    dbg("stroke");

    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 0, user_movex + clipmovex, user_movey + clipmovey);
    strokeGfxline(state, line, 0);
    gfxline_free(line);
}

void GFXOutputDev::fill(GfxState *state) 
{
    gfxcolor_t col = getFillColor(state);
    dbg("fill %02x%02x%02x%02x",col.r,col.g,col.b,col.a);

    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 1, user_movex + clipmovex, user_movey + clipmovey);
    fillGfxLine(state, line);
    gfxline_free(line);
}

void GFXOutputDev::eoFill(GfxState *state) 
{
    gfxcolor_t col = getFillColor(state);
    dbg("eofill %02x%02x%02x%02x",col.r,col.g,col.b,col.a);

    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 1, user_movex + clipmovex, user_movey + clipmovey);
    fillGfxLine(state, line);
    gfxline_free(line);
}


static const char* dirseparator()
{
#ifdef WIN32
    return "\\";
#else
    return "/";
#endif
}

void addGlobalFont(const char*filename)
{
    fontfile_t* f = (fontfile_t*)malloc(sizeof(fontfile_t));
    memset(f, 0, sizeof(fontfile_t));
    f->filename = filename;
    int len = strlen(filename);
    char*r1 = strrchr(filename, '/');
    char*r2 = strrchr(filename, '\\');
    if(r2>r1)
        r1 = r2;
    if(r1) {
        len = strlen(r1+1);
    }
    f->len = len;

    msg("<notice> Adding font \"%s\".", filename);
    if(global_fonts_next) {
        global_fonts_next->next = f;
        global_fonts_next = global_fonts_next->next;
    } else {
        global_fonts_next = global_fonts = f;
    }
}

void addGlobalLanguageDir(const char*dir)
{
    msg("<notice> Adding %s to language pack directories", dir);

    FILE*fi = 0;
    char* config_file = (char*)malloc(strlen(dir) + 1 + sizeof("add-to-xpdfrc") + 1);
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

void addGlobalFontDir(const char*dirname)
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
	if(type) {
	    char*fontname = (char*)malloc(strlen(dirname)+strlen(name)+2);
	    strcpy(fontname, dirname);
            strcat(fontname, dirseparator());
	    strcat(fontname, name);
	    addGlobalFont(fontname);
	}
    }
    closedir(dir);
#else
    msg("<warning> No dirent.h- unable to add font dir %s", dirname);
#endif
}

void GFXOutputDev::preparePage(int pdfpage, int outputpage)
{
    if(pdfpage < 0)
	return;

    if(!this->pages) {
	this->pagebuflen = 1024;
	this->pages = (int*)malloc(this->pagebuflen*sizeof(int));
	memset(this->pages, -1, this->pagebuflen*sizeof(int));
    } else {
	while(pdfpage >= this->pagebuflen)
	{
	    int oldlen = this->pagebuflen;
	    this->pagebuflen+=1024;
	    this->pages = (int*)realloc(this->pages, this->pagebuflen*sizeof(int));
	    memset(&this->pages[oldlen], -1, (this->pagebuflen-oldlen)*sizeof(int));
	}
    }
    this->pages[pdfpage] = outputpage;
    if(pdfpage>this->pagepos)
	this->pagepos = pdfpage;
}

struct BBox
{
    double posx,posy;
    double width,height;
};

BBox mkBBox(GfxState*state, double*bbox, double width, double height)
{
    double xMin, yMin, xMax, yMax, x, y;
    double tx, ty, w, h;
    // transform the bbox
    state->transform(bbox[0], bbox[1], &x, &y);
    xMin = xMax = x;
    yMin = yMax = y;
    state->transform(bbox[0], bbox[3], &x, &y);
    if (x < xMin) {
      xMin = x;
    } else if (x > xMax) {
      xMax = x;
    }
    if (y < yMin) {
      yMin = y;
    } else if (y > yMax) {
      yMax = y;
    }
    state->transform(bbox[2], bbox[1], &x, &y);
    if (x < xMin) {
      xMin = x;
    } else if (x > xMax) {
      xMax = x;
    }
    if (y < yMin) {
      yMin = y;
    } else if (y > yMax) {
      yMax = y;
    }
    state->transform(bbox[2], bbox[3], &x, &y);
    if (x < xMin) {
      xMin = x;
    } else if (x > xMax) {
      xMax = x;
    }
    if (y < yMin) {
      yMin = y;
    } else if (y > yMax) {
      yMax = y;
    }
    tx = (int)floor(xMin);
    if (tx < 0) {
      tx = 0;
    } else if (tx > width) {
      tx = width;
    }
    ty = (int)floor(yMin);
    if (ty < 0) {
      ty = 0;
    } else if (ty > height) {
      ty = height;
    }
    w = (int)ceil(xMax) - tx + 1;
    if (tx + w > width) {
      w = width - tx;
    }
    if (w < 1) {
      w = 1;
    }
    h = (int)ceil(yMax) - ty + 1;
    if (ty + h > height) {
      h = height - ty;
    }
    if (h < 1) {
      h = 1;
    }
    BBox nbbox;
    nbbox.posx = xMin;
    nbbox.posx = yMin;
    nbbox.width = w;
    nbbox.height = h;
    return nbbox;
}

void GFXOutputDev::beginTransparencyGroup(GfxState *state, double *bbox,
				      GfxColorSpace *blendingColorSpace,
				      GBool isolated, GBool knockout,
				      GBool forSoftMask)
{
    const char*colormodename = "";

    if(blendingColorSpace) {
	colormodename = GfxColorSpace::getColorSpaceModeName(blendingColorSpace->getMode());
    }
    dbg("beginTransparencyGroup %.1f/%.1f/%.1f/%.1f %s isolated=%d knockout=%d forsoftmask=%d", bbox[0],bbox[1],bbox[2],bbox[3], colormodename, isolated, knockout, forSoftMask);
    msg("<verbose> beginTransparencyGroup %.1f/%.1f/%.1f/%.1f %s isolated=%d knockout=%d forsoftmask=%d", bbox[0],bbox[1],bbox[2],bbox[3], colormodename, isolated, knockout, forSoftMask);
    
    states[statepos].createsoftmask |= forSoftMask;
    states[statepos].transparencygroup = !forSoftMask;
    states[statepos].isolated = isolated;

    states[statepos].olddevice = this->device;
    this->device = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));

    gfxdevice_record_init(this->device);
    
    /*if(!forSoftMask) { ////???
	state->setFillOpacity(0.0);
    }*/
    dbgindent+=2;
}

void GFXOutputDev::endTransparencyGroup(GfxState *state)
{
    dbgindent-=2;
    dbg("endTransparencyGroup");
    msg("<verbose> endTransparencyGroup");

    gfxdevice_t*r = this->device;

    this->device = states[statepos].olddevice;

    if(states[statepos].createsoftmask) {
	states[statepos-1].softmaskrecording = r->finish(r);
    } else {
	states[statepos-1].grouprecording = r->finish(r);
    }
    
    states[statepos].createsoftmask = 0;
    states[statepos].transparencygroup = 0;
    free(r);
}

void GFXOutputDev::paintTransparencyGroup(GfxState *state, double *bbox)
{
    const char*blendmodes[] = {"normal","multiply","screen","overlay","darken", "lighten",
                               "colordodge","colorburn","hardlight","softlight","difference",
                               "exclusion","hue","saturation","color","luminosity"};

    dbg("paintTransparencyGroup blend=%s softmaskon=%d", blendmodes[state->getBlendMode()], states[statepos].softmask);
    msg("<verbose> paintTransparencyGroup blend=%s softmaskon=%d", blendmodes[state->getBlendMode()], states[statepos].softmask);
   
    if(state->getBlendMode() == gfxBlendNormal)
	infofeature("transparency groups");
    else {
	char buffer[80];
	sprintf(buffer, "%s blended transparency groups", blendmodes[state->getBlendMode()]);
	warnfeature(buffer, 0);
    }

    gfxresult_t*grouprecording = states[statepos].grouprecording;
   
    if(state->getBlendMode() == gfxBlendNormal) {
	gfxdevice_t ops;
	gfxdevice_ops_init(&ops, this->device, (unsigned char)(state->getFillOpacity()*255));
	gfxresult_record_replay(grouprecording, &ops);
	ops.finish(&ops);
    }
    grouprecording->destroy(grouprecording);

    states[statepos].grouprecording = 0;
}

void GFXOutputDev::setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *rgb)
{
    /* alpha = 1: retrieve mask values from alpha layer
       alpha = 0: retrieve mask values from luminance */
    dbg("setSoftMask %.1f/%.1f/%.1f/%.1f alpha=%d backdrop=%02x%02x%02x",
	    bbox[0], bbox[1], bbox[2], bbox[3], alpha, colToByte(rgb->c[0]), colToByte(rgb->c[1]), colToByte(rgb->c[2]));
    msg("<verbose> setSoftMask %.1f/%.1f/%.1f/%.1f alpha=%d backdrop=%02x%02x%02x",
	    bbox[0], bbox[1], bbox[2], bbox[3], alpha, colToByte(rgb->c[0]), colToByte(rgb->c[1]), colToByte(rgb->c[2]));
    if(!alpha)
	infofeature("soft masks");
    else
	warnfeature("soft masks from alpha channel",0);
    
    states[statepos].olddevice = this->device;
    this->device = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));
    gfxdevice_record_init(this->device);

    dbg("softmaskrecording is %08x at statepos %d\n", states[statepos].softmaskrecording, statepos);
    
    states[statepos].softmask = 1;
    states[statepos].softmask_alpha = alpha;
}

static inline Guchar div255(int x) {
  return (Guchar)((x + (x >> 8) + 0x80) >> 8);
}

static unsigned char clampU8(unsigned char c, unsigned char min, unsigned char max)
{
    if(c < min) c = min;
    if(c > max) c = max;
    return c;
}

void GFXOutputDev::clearSoftMask(GfxState *state)
{
    if(!states[statepos].softmask)
	return;
    states[statepos].softmask = 0;
    dbg("clearSoftMask statepos=%d", statepos);
    msg("<verbose> clearSoftMask");
    
    if(!states[statepos].softmaskrecording || strcmp(this->device->name, "record")) {
	msg("<error> Error in softmask/tgroup ordering");
	return;
    }
  
    gfxresult_t*mask = states[statepos].softmaskrecording;
    gfxresult_t*below = this->device->finish(this->device);
    this->device = states[statepos].olddevice;

    /* get outline of all objects below the soft mask */
    gfxdevice_t uniondev;
    gfxdevice_union_init(&uniondev, 0);
    gfxresult_record_replay(below, &uniondev);
    gfxline_t*belowoutline = gfxdevice_union_getunion(&uniondev);
    uniondev.finish(&uniondev);

    gfxbbox_t bbox = gfxline_getbbox(belowoutline);
#if 0 
    this->device->startclip(this->device, belowoutline);
    gfxresult_record_replay(below, this->device);
    gfxresult_record_replay(mask, this->device);
    this->device->endclip(this->device);
    gfxline_free(belowoutline);
#endif
    
    int width = (int)bbox.xmax,height = (int)bbox.ymax;
    if(width<=0 || height<=0)
        return;

    gfxdevice_t belowrender;
    gfxdevice_render_init(&belowrender);
    if(states[statepos+1].isolated) {
	belowrender.setparameter(&belowrender, "fillwhite", "1");
    }
    belowrender.setparameter(&belowrender, "antialize", "2");
    belowrender.startpage(&belowrender, width, height);
    gfxresult_record_replay(below, &belowrender);
    belowrender.endpage(&belowrender);
    gfxresult_t* belowresult = belowrender.finish(&belowrender);
    gfximage_t* belowimg = (gfximage_t*)belowresult->get(belowresult,"page0");
    //writePNG("below.png", (unsigned char*)belowimg->data, belowimg->width, belowimg->height);

    gfxdevice_t maskrender;
    gfxdevice_render_init(&maskrender);
    maskrender.startpage(&maskrender, width, height);
    gfxresult_record_replay(mask, &maskrender);
    maskrender.endpage(&maskrender);
    gfxresult_t* maskresult = maskrender.finish(&maskrender);
    gfximage_t* maskimg = (gfximage_t*)maskresult->get(maskresult,"page0");

    if(belowimg->width != maskimg->width || belowimg->height != maskimg->height) {
	msg("<fatal> Internal error in mask drawing");
	return;
    }

    int y,x;
    for(y=0;y<height;y++) {
	gfxcolor_t* l1 = &maskimg->data[maskimg->width*y];
	gfxcolor_t* l2 = &belowimg->data[belowimg->width*y];
	for(x=0;x<width;x++) {
	    int alpha;
	    if(states[statepos].softmask_alpha) {
		alpha = l1->a;
	    } else {
		alpha = (77*l1->r + 151*l1->g + 28*l1->b) >> 8;
	    }

	    l2->a = div255(alpha*l2->a);

	    /* DON'T premultiply alpha- this is done by fillbitmap,
	       depending on the output device */
	    //l2->r = div255(alpha*l2->r);
	    //l2->g = div255(alpha*l2->g);
	    //l2->b = div255(alpha*l2->b);

	    l1++;
	    l2++;
	}
    }
    gfxline_t*line = gfxline_makerectangle(0,0,width,height);

    gfxmatrix_t matrix;
    matrix.m00 = 1.0; matrix.m10 = 0.0; matrix.tx = 0.0;
    matrix.m01 = 0.0; matrix.m11 = 1.0; matrix.ty = 0.0;

    this->device->fillbitmap(this->device, line, belowimg, &matrix, 0);

    mask->destroy(mask);
    below->destroy(below);
    maskresult->destroy(maskresult);
    belowresult->destroy(belowresult);
    states[statepos].softmaskrecording = 0;
}
  
//class MemCheck
//{
//    public: ~MemCheck()
//    {
//        delete globalParams;globalParams=0;
//        Object::memCheck(stderr);
//        gMemReport(stderr);
//    }
//} myMemCheck;

