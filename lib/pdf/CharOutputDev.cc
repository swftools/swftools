/* CharOutputDev.cc
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
#include <math.h>

#include "../../config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_FONTCONFIG
#include <fontconfig.h>
#endif

// xpdf header files
#include "popplercompat.h"
#include "CharOutputDev.h"

// swftools header files
#include "../os.h"
#include "../log.h"
#include "../mem.h"
#include "../utf8.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../gfxfont.h"
#include "../gfxpoly.h"
#include "../devices/record.h"
#include "../devices/ops.h"
#include "../devices/polyops.h"
#include "../devices/render.h"
#include "../png.h"

// linked-in font data
#include "fonts.h"

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
struct fontentry {
    const char*pdffont;
    const char*filename;
    char*afm;
    int afmlen;
    char*pfb;
    int pfblen;
    char*fullfilename;
    DisplayFontParam *dfp;
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
    int written = fwrite(f->afm, 1, f->afmlen, fi);
    if(written<0)
        return 0;
    fclose(fi);

    sprintf(namebuf2, "%s.pfb", tmpFileName);
    fi = fopen(namebuf2, "wb");
    if(!fi)
        return 0;
    written = fwrite(f->pfb, 1, f->pfblen, fi);
    if(written<0)
        return 0;
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

static int config_use_fontconfig = 1;
static int fcinitcalled = 0; 

GFXGlobalParams::GFXGlobalParams()
: GlobalParams((char*)"")
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
#ifdef HAVE_FONTCONFIG
    if(config_use_fontconfig && fcinitcalled)
	FcFini();
#endif
}
#ifdef HAVE_FONTCONFIG
static char stralphacmp(const char*s1, const char*s2)
{
    while(*s1 && *s2) {
	/* skip over space, minus, comma etc. */
	while(*s1>=32 && *s1<=63) s1++;
	while(*s2>=32 && *s2<=63) s2++;
	if(*s1!=*s2)
	    break;
	s1++;s2++;
    }
    return *s1 - *s2;
}

static char fc_ismatch(FcPattern*match, char*family, char*style)
{
    char*fcfamily=0,*fcstyle=0,*fcfullname=0,*filename=0;
    FcBool scalable=FcFalse, outline=FcFalse;
    FcPatternGetString(match, "family", 0, (FcChar8**)&fcfamily);
    FcPatternGetString(match, "style", 0, (FcChar8**)&fcstyle);
    FcPatternGetString(match, "file", 0, (FcChar8**)&filename);
    FcPatternGetBool(match, "outline", 0, &outline);
    FcPatternGetBool(match, "scalable", 0, &scalable);

    if(scalable!=FcTrue || outline!=FcTrue)
	return 0;

    if (!stralphacmp(fcfamily, family)) {
	msg("<debug> Font %s-%s (%s) is a match for %s%s%s", fcfamily, fcstyle, filename, family, style?"-":"", style?style:"");
	return 1;
    } else {
	//msg("<debug> Font %s-%s (%s) is NOT a match for %s%s%s", fcfamily, fcstyle, filename, family, style?"-":"", style?style:"");
	return 0;
    }
}
#endif

static inline char islowercase(char c) 
{
    return (c>='a' && c<='z');
}

char* fontconfig_searchForFont(char*name)
{
#ifdef HAVE_FONTCONFIG
    if(!config_use_fontconfig)
	return 0;
    
    // call init ony once
    if (!fcinitcalled) {
        fcinitcalled = 1;

	// check whether we have a config file
	char* configfile = (char*)FcConfigFilename(0);
	int configexists = 0;
	FILE*fi = fopen(configfile, "rb");
	if(fi) {
	    configexists = 1;fclose(fi);
	    msg("<debug> Initializing FontConfig (configfile=%s)", configfile);
	} else {
	    msg("<debug> Initializing FontConfig (no configfile)");
	}

	if(!configexists) {
	    /* A fontconfig instance which didn't find a configfile is unbelievably
	       cranky, so let's just write out a small xml file and make fontconfig
	       happy */
	    FcConfig*c = FcConfigCreate();
	    char namebuf[512];
	    char* tmpFileName = mktmpname(namebuf);
	    FILE*fi = fopen(tmpFileName, "wb");
	    fprintf(fi, "<?xml version=\"1.0\"?>\n<fontconfig>\n");//<!DOCTYPE fontconfig SYSTEM "fonts.dtd">
#ifdef WIN32
	    fprintf(fi, "<dir>WINDOWSFONTDIR</dir>\n");
#endif
	    fprintf(fi, "<dir>~/.fonts</dir>\n");
#ifdef WIN32
	    fprintf(fi, "<cachedir>WINDOWSTEMPDIR_FONTCONFIG_CACHE</cachedir>\n");
#endif
	    fprintf(fi, "<cachedir>~/.fontconfig</cachedir>\n");
	    fprintf(fi, "</fontconfig>\n");
	    fclose(fi);
	    FcConfigParseAndLoad(c, (FcChar8*)tmpFileName, 1);
	    FcConfigBuildFonts(c);
	    FcConfigSetCurrent(c);
	}

	if(!FcInit()) {
            msg("<debug> FontConfig Initialization failed. Disabling.");
            config_use_fontconfig = 0;
            return 0;
        }
	FcConfig * config = FcConfigGetCurrent();
	if(!config) {
            msg("<debug> FontConfig Config Initialization failed. Disabling.");
            config_use_fontconfig = 0;
            return 0;
        }

        /* add external fonts to fontconfig's config, too. */
        fontfile_t*fd = global_fonts;
        while(fd) {
            FcConfigAppFontAddFile(config, (FcChar8*)fd->filename);
            msg("<debug> Adding font %s to fontconfig", fd->filename);
            fd = fd->next;
        }

	FcFontSet * set =  FcConfigGetFonts(config, FcSetSystem);
        msg("<verbose> FontConfig initialized. Found %d fonts", set?set->nfont:0);
	if(!set || !set->nfont) {
            msg("<debug> FontConfig has zero fonts. Disabling.");
            config_use_fontconfig = 0;
            return 0;
        }

	if(getLogLevel() >= LOGLEVEL_TRACE) {
	    int t;
	    int p;
	    for(p=0;p<2;p++) {
		if(set) {
		    for(t=0;t<set->nfont;t++) {
			char*fcfamily=0,*fcstyle=0,*filename=0;
			FcBool scalable=FcFalse, outline=FcFalse;
			FcPatternGetString(set->fonts[t], "family", 0, (FcChar8**)&fcfamily);
			FcPatternGetString(set->fonts[t], "style", 0, (FcChar8**)&fcstyle);
			FcPatternGetString(set->fonts[t], "file", 0, (FcChar8**)&filename);
			FcPatternGetBool(set->fonts[t], "outline", 0, &outline);
			FcPatternGetBool(set->fonts[t], "scalable", 0, &scalable);
			if(scalable && outline) {
			    msg("<trace> %s (%s) -> %s", fcfamily, fcstyle, filename);
			}
		    }
		}
		set =  FcConfigGetFonts(config, FcSetApplication);
	    }
	}
    }

    char*family = strdup(name);
    int len = strlen(family);

    const char*styles[] = {"Medium", "Regular", "Bold", "Italic", "Black", "Narrow"};
    const char*style = 0;
    int t;
    for(t=0;t<sizeof(styles)/sizeof(styles[0]);t++) {
	int l = strlen(styles[t]);
	if(len>l+1 && !strcmp(family+len-l, styles[t]) && islowercase(family[len-l-1])) {
	    style = styles[t];
	    family[len-l]=0;
	    break;
	}
    }
    if(!style) {
	char*dash = strchr(family, '-');
	if(!dash) dash = strchr(family, ',');
	if(dash) {
	    *dash = 0;
	    style = dash+1;
	}
    }
    FcPattern*pattern = 0;
    if(style) {
	msg("<debug> FontConfig: Looking for font %s (family=%s style=%s)", name, family, style);
	pattern = FcPatternBuild(NULL, FC_OUTLINE, FcTypeBool, FcTrue, FC_SCALABLE, FcTypeBool, FcTrue, FC_FAMILY, FcTypeString, family, FC_STYLE, FcTypeString, style, NULL);
    } else {
	msg("<debug> FontConfig: Looking for font %s (family=%s)", name, family);
	pattern = FcPatternBuild(NULL, FC_OUTLINE, FcTypeBool, FcTrue, FC_SCALABLE, FcTypeBool, FcTrue, FC_FAMILY, FcTypeString, family, NULL);
    }
    pattern = FcPatternBuild(NULL, FC_OUTLINE, FcTypeBool, FcTrue, FC_SCALABLE, FcTypeBool, FcTrue, FC_FAMILY, FcTypeString, family, NULL);

    FcResult result;
    FcConfigSubstitute(0, pattern, FcMatchPattern); 
    FcDefaultSubstitute(pattern);

    FcFontSet*set = FcFontSort(0, pattern, 1, 0, &result);
    if(set) {
	int t;
	for(t=0;t<set->nfont;t++) {
	    FcPattern*match = set->fonts[t];
	    //FcPattern*match = FcFontMatch(0, pattern, &result);
	    if(fc_ismatch(match, family, (char*)style)) {
		char*filename=0;
		if(FcPatternGetString(match, "file", 0, (FcChar8**)&filename) != FcResultMatch) {
		    msg("<debug> FontConfig: Couldn't get fontconfig's filename for font %s", name);
		    filename=0;
		}
		//FcPatternDestroy(match);
		msg("<debug> fontconfig: returning filename %s", filename);
		free(family);
		FcPatternDestroy(pattern);
		FcFontSetDestroy(set);
		return filename?strdup(filename):0;
	    }
	}
    }
    free(family);
    FcPatternDestroy(pattern);
    FcFontSetDestroy(set);
    return 0;
#else
    return 0;
#endif
}

static DisplayFontParamKind detectFontType(const char*filename)
{
    if(strstr(filename, ".ttf") || strstr(filename, ".TTF"))
	return displayFontTT;
    if(strstr(filename, ".pfa") || strstr(filename, ".PFA") || strstr(filename, ".pfb"))
	return displayFontT1;
    return displayFontTT;
}

DisplayFontParam *GFXGlobalParams::getDisplayFont(GString *fontName)
{
    msg("<verbose> looking for font %s", fontName->getCString());

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
		DisplayFontParam *dfp = new DisplayFontParam(new GString(fontName), displayFontT1);
		dfp->t1.fileName = new GString(pdf2t1map[t].fullfilename);
		pdf2t1map[t].dfp = dfp;
	    }
	    return pdf2t1map[t].dfp;
	}
    }
    
    int bestlen = 0x7fffffff;
    const char*bestfilename = 0;
   
#ifndef HAVE_FONTCONFIG
    /* if we don't have fontconfig, try a simple filename-comparison approach */
    fontfile_t*f = global_fonts;
    while(f) {
	if(strstr(f->filename, name)) {
            if(f->len < bestlen) {
                bestlen = f->len;
                bestfilename = f->filename;
            }
        }
        f = f->next;
    }
#endif

    /* if we didn't find anything up to now, try looking for the
       font via fontconfig */
    char*filename = 0;
    if(!bestfilename) {
	filename = fontconfig_searchForFont(name);
    } else {
	filename = strdup(bestfilename);
    }

    if(filename) {
        msg("<verbose> Font %s maps to %s\n", name, filename);
	DisplayFontParamKind kind = detectFontType(filename);
        DisplayFontParam *dfp = new DisplayFontParam(new GString(fontName), kind);
	if(kind == displayFontTT) {
	    dfp->tt.fileName = new GString(filename);
	} else {
	    dfp->t1.fileName = new GString(filename);
	}
	free(filename);
        return dfp;
    } else {
	msg("<verbose> Font %s not found\n", name);
	return GlobalParams::getDisplayFont(fontName);
    }
}

DisplayFontParam *GFXGlobalParams::getDisplayCIDFont(GString *fontName, GString *collection)
{
    DisplayFontParam*dfp = GlobalParams::getDisplayCIDFont(fontName, collection);
    if(!dfp) {
        dfp = this->getDisplayFont(fontName);
    }
    return dfp;
}

CharOutputDev::CharOutputDev(InfoOutputDev*info, PDFDoc*doc, int*page2page, int num_pages, int x, int y, int x1, int y1, int x2, int y2)
:CommonOutputDev(info, doc, page2page, num_pages, x, y, x1, y1, x2, y2)
{
    this->type3active = 0;
    this->xref = 0;
    this->current_text_stroke = 0;
    this->current_text_clip = 0;
    this->config_bigchar=0;
    this->config_extrafontdata = 0;
    this->config_detectspaces = 1;
    this->config_space_between_lines = 0;
    this->config_linkdatafile = 0;
    this->page2page = 0;
    this->num_pages = 0;
    this->links = 0;
    this->last_link = 0;
};

CharOutputDev::~CharOutputDev()
{
}

void CharOutputDev::setParameter(const char*key, const char*value)
{
    if(!strcmp(key,"detectspaces")) {
        this->config_detectspaces = atoi(value);
    } else if(!strcmp(key,"space_between_lines")) {
        this->config_space_between_lines = atoi(value);
    } else if(!strcmp(key,"extrafontdata")) {
        this->config_extrafontdata = atoi(value);
    } else if(!strcmp(key,"linkdatafile")) {
        this->config_linkdatafile = strdup(value);
    }
}
  
void CharOutputDev::setDevice(gfxdevice_t*dev)
{
    this->device = dev;
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
  msg("%s=========== %s (ID:%d,%d) ==========", loglevel, name, r->num,r->gen);

  GString*gstr  = font->getTag();
   
  msg("%s| Tag: %s", loglevel, id);
  
  if(font->isCIDFont()) msg("%s| is CID font", loglevel);

  GfxFontType type=font->getType();
  switch(type) {
    case fontUnknownType:
     msg("%s| Type: unknown",loglevel);
    break;
    case fontType1:
     msg("%s| Type: 1",loglevel);
    break;
    case fontType1C:
     msg("%s| Type: 1C",loglevel);
    break;
    case fontType3:
     msg("%s| Type: 3",loglevel);
    break;
    case fontTrueType:
     msg("%s| Type: TrueType",loglevel);
    break;
    case fontCIDType0:
     msg("%s| Type: CIDType0",loglevel);
    break;
    case fontCIDType0C:
     msg("%s| Type: CIDType0C",loglevel);
    break;
    case fontCIDType2:
     msg("%s| Type: CIDType2",loglevel);
    break;
  }
  
  Ref embRef;
  GBool embedded = font->getEmbeddedFontID(&embRef);
  char*embeddedName=0;
  if(font->getEmbeddedFontName()) {
    embeddedName = font->getEmbeddedFontName()->getCString();
  }
  if(embedded)
   msg("%s| Embedded id: %s id: %d",loglevel, FIXNULL(embeddedName), embRef.num);

  gstr = font->getExtFontFile();
  if(gstr)
   msg("%s| External Font file: %s", loglevel, FIXNULL(gstr->getCString()));

  // Get font descriptor flags.
  if(font->isFixedWidth()) msg("%s| is fixed width", loglevel);
  if(font->isSerif()) msg("%s| is serif", loglevel);
  if(font->isSymbolic()) msg("%s| is symbolic", loglevel);
  if(font->isItalic()) msg("%s| is italic", loglevel);
  if(font->isBold()) msg("%s| is bold", loglevel);

  free(id);
  free(name);
}

GBool CharOutputDev::needNonText() 
{ 
    return gFalse; 
}

void CharOutputDev::endPage() 
{
    msg("<verbose> endPage (GfxOutputDev)");
    
    if(this->links) {
	kdtree_destroy(this->links);
	this->links = 0;
    }
    GFXLink*l = this->last_link;
    while(l) {
	GFXLink*last = l->last;
	l->draw(this,device);
	delete l;
	l = last;
    }
    this->last_link = 0;
}

static inline double sqr(double x) {return x*x;}

GBool CharOutputDev::upsideDown() 
{
    return gTrue;
};
GBool CharOutputDev::useDrawChar() 
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

void CharOutputDev::updateTextMat(GfxState*state)
{
}

void CharOutputDev::beginString(GfxState *state, GString *s) 
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

void CharOutputDev::drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode charid, int nBytes, Unicode *_u, int uLen)
{
    FontInfo*current_fontinfo = this->info->getFontInfo(state);

    if(!current_fontinfo || (unsigned)charid >= current_fontinfo->num_glyphs || !current_fontinfo->glyphs[charid]) {
	msg("<error> Invalid charid %d for font %p (%d characters)", charid, current_fontinfo, current_fontinfo?current_fontinfo->num_glyphs:0);
	return;
    }

    gfxfont_t*current_gfxfont = current_fontinfo->getGfxFont();
    if(!current_fontinfo->seen) {
	dumpFontInfo("<verbose>", state->getFont());
	device->addfont(device, current_gfxfont);
        current_fontinfo->seen = 1;
    }

    CharCode glyphid = current_fontinfo->glyphs[charid]->glyphid;

    int render = state->getRender();
    gfxcolor_t col = gfxstate_getfillcolor(state);

    GFXLink*link = 0;
    if(links) {
	kdarea_t*a = kdtree_find(this->links, x+dx/2,y+dy/2);
	if(a) {
	    link = (GFXLink*)a->data;
#if 0
            if(link) {
                printf("area [%d %d %d %d] (link [%f %f %f %f]) contains (%f,%f)\n", 
                        a->bbox.xmin, a->bbox.ymin, a->bbox.xmax, a->bbox.ymax,
                        link->x1, link->y1, link->x2, link->y2,
                        x+dx/2, y+dy/2
                        );
            }
#endif
	}
        if(link != previous_link) {
            previous_link = link;
            device->setparameter(device, "link", link?link->action:"");
        }
    }

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

    gfxmatrix_t m = current_fontinfo->get_gfxmatrix(state);
    this->transformXY(state, x-originX, y-originY, &m.tx, &m.ty);

    gfxbbox_t bbox;

    msg("<debug> drawChar(%f,%f,c='%c' (%d), u=%d <%d> '%c') CID=%d render=%d glyphid=%d font=%p size=%f",
            m.tx, m.ty, (charid&127)>=32?charid:'?', charid, u, uLen, u,
            font->isCIDFont(), render, glyphid, current_gfxfont,
            m.m00);

    gfxglyph_t* gfxglyph = &current_gfxfont->glyphs[glyphid];

    int space = current_fontinfo->space_char;
    if(config_extrafontdata && config_detectspaces && space>=0 && m.m00 && !m.m01) {
	/* space char detection */
	//bool different_y = last_char_y - m.ty;
	bool different_y = m.ty < last_char_y - last_ascent*last_char_y_fontsize
	                || m.ty > last_char_y + last_descent*last_char_y_fontsize;
        double expected_x = last_char_x + last_char_advance*last_char_x_fontsize;
        double rightx = m.tx + gfxglyph->advance * m.m00;
        if(different_y) {
            expected_x = m.tx - width/2;
        }

	if((!different_y || config_space_between_lines) &&
	   !last_char_was_space && !current_fontinfo->usesSpaces()) {
	    int space = current_fontinfo->space_char;
	    float width = fmax(m.m00*current_fontinfo->average_advance, last_char_x_fontsize*last_average_advance);
	    if(m.tx - expected_x >= width*4/10) {
		msg("<debug> There's a %f pixel gap between char %d and char %d (expected no more than %f), I'm inserting a space here", 
			m.tx-expected_x,
			last_char, glyphid,
			width*4/10
			);
#ifdef VISUALIZE_CHAR_GAPS
		bbox = gfxline_getbbox(gfxglyph->line);
		gfxline_t*rect = gfxline_makerectangle(last_char_x,m.ty,m.tx,m.ty+10);
		gfxcolor_t red = {255,255,0,0};
		device->fill(device, rect, &red);
		gfxline_free(rect);
#endif
		gfxmatrix_t m2 = m;
		m2.tx = expected_x + (m.tx - expected_x - current_gfxfont->glyphs[space].advance*m.m00)/2;
		if(m2.tx < expected_x) m2.tx = expected_x;
		device->drawchar(device, current_gfxfont, space, &col, &m2);
		if(link) {
		    link->addchar(32);
		}
	    }
	}
	last_average_advance = current_fontinfo->average_advance;
	last_char_advance = gfxglyph->advance;
	last_char_x_fontsize = m.m00;
	last_char_y_fontsize = -m.m11;
	last_char = glyphid;
	last_char_x = m.tx;
	last_char_y = m.ty;
	last_ascent = current_gfxfont->ascent;
	last_descent = fmax(current_gfxfont->descent, current_gfxfont->ascent/3);
	last_char_was_space = GLYPH_IS_SPACE(gfxglyph);

        if(m.tx < expected_x && rightx < expected_x + 1 && GLYPH_IS_SPACE(gfxglyph)) {
            msg("<debug> Dropping dedented space char at %f-%f (before %f)",
                    m.tx, rightx, expected_x);
            return;
        }

    }
    device->drawchar(device, current_gfxfont, glyphid, &col, &m);
    
    if(link) {
	link->addchar(current_gfxfont->glyphs[glyphid].unicode);
    }
}

void CharOutputDev::endString(GfxState *state) 
{ 
}    

void CharOutputDev::endTextObject(GfxState *state)
{
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

GBool CharOutputDev::beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode charid, Unicode *u, int uLen)
{
    msg("<debug> beginType3Char %d u=%d", charid, uLen?u[0]:0);
    type3active = 1;
    
    if(config_extrafontdata) {

	FontInfo*current_fontinfo = info->getFontInfo(state);
	if(!current_fontinfo) {
	    msg("<error> Couldn't find font info");
	    return gFalse;
	}
	gfxfont_t*current_gfxfont = current_fontinfo->getGfxFont();

	/*m.m00*=INTERNAL_FONT_SIZE;
	m.m01*=INTERNAL_FONT_SIZE;
	m.m10*=INTERNAL_FONT_SIZE;
	m.m11*=INTERNAL_FONT_SIZE;*/

	if(!current_fontinfo || (unsigned)charid >= current_fontinfo->num_glyphs || !current_fontinfo->glyphs[charid]) {
	    msg("<error> Invalid type3 charid %d for font %p", charid, current_fontinfo);
	    return gFalse;
	}
	gfxcolor_t col={0,0,0,0};
	CharCode glyphid = current_fontinfo->glyphs[charid]->glyphid;
	gfxmatrix_t m = current_fontinfo->get_gfxmatrix(state);
	this->transformXY(state, 0, 0, &m.tx, &m.ty);
	device->drawchar(device, current_gfxfont, glyphid, &col, &m);
    }


    /* the character itself is going to be passed using the draw functions */
    return gFalse; /* gTrue= is_in_cache? */
}

void CharOutputDev::type3D0(GfxState *state, double wx, double wy) {
}
void CharOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury) {
}

void CharOutputDev::endType3Char(GfxState *state)
{
    type3active = 0;
    msg("<debug> endType3Char");
}

void CharOutputDev::beginPage(GfxState *state, int pageNum)
{
    this->currentpage = pageNum;
    this->last_char_was_space = 1;
    this->last_char_y = 0;
    this->last_char_y_fontsize = 0;
    this->last_ascent = 0;
    this->last_descent = 0;
    this->previous_link = 0;
}

void GFXLink::draw(CharOutputDev*out, gfxdevice_t*dev)
{
    int x1,y1,x2,y2;
    out->transformXY_stateless(this->x1, this->y1, &x1, &y1);
    out->transformXY_stateless(this->x2, this->y2, &x2, &y2);

    gfxline_t points[5];
    points[0].type = gfx_moveTo;
    points[0].x = x1;
    points[0].y = y1;
    points[0].next = &points[1];
    points[1].type = gfx_lineTo;
    points[1].x = x1;
    points[1].y = y2;
    points[1].next = &points[2];
    points[2].type = gfx_lineTo;
    points[2].x = x2;
    points[2].y = y2;
    points[2].next = &points[3];
    points[3].type = gfx_lineTo;
    points[3].x = x2;
    points[3].y = y1;
    points[3].next = &points[4];
    points[4].type = gfx_lineTo;
    points[4].x = x1;
    points[4].y = y1;
    points[4].next = 0;
    msg("<trace> drawing link %.2f/%.2f %.2f/%.2f %.2f/%.2f %.2f/%.2f to %s (\"%s\")",
	    points[0].x, points[0].y,
	    points[1].x, points[1].y,
	    points[2].x, points[2].y,
	    points[3].x, points[3].y, action, text); 
    
    dev->drawlink(dev, points, action, text);
}

void GFXLink::addchar(int unicode)
{
    msg("<trace> Adding '%c' (%d) to link %s", unicode, unicode, action);
    char buf[8];
    int l = writeUTF8(unicode, buf);
    while(size+l+1>=buf_size) {
	buf_size += 32;
	text = (char*)rfx_realloc(text, buf_size);
    }
    strcpy(text+size, buf);
    size += l;
}

GFXLink::GFXLink(GFXLink*last, const char*action, double x1, double y1, double x2, double y2)
{
    this->buf_size = 0;
    this->size = 0;
    this->text = 0;
    this->last = last;
    this->action = strdup(action);
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}

GFXLink::~GFXLink()
{
    free((void*)this->action);
    if(this->text)
	free(this->text);
    this->text = 0;
    this->action = 0;
}


void CharOutputDev::processLink(Link *link, Catalog *catalog)
{
    double x1, y1, x2, y2;
    
    msg("<debug> drawlink");

    link->getRect(&x1, &y1, &x2, &y2);

    LinkAction*actionobj=link->getAction();
    char buf[128];
    char*s = 0;
    const char*type = "-?-";
    char*named = 0;
    int page = -1;
    msg("<trace> drawlink actionobj=%d", actionobj->getKind());
    switch(actionobj->getKind())
    {
        case actionGoTo: {
            type = "GoTo";
            LinkGoTo *ha=(LinkGoTo *)link->getAction();
            LinkDest *dest=NULL;
            if (ha->getDest()==NULL) 
                dest=catalog->findDest(ha->getNamedDest());
            else 
                dest=ha->getDest()->copy();
            if (dest){ 
              if (dest->isPageRef()){
                Ref pageref=dest->getPageRef();
                page=catalog->findPage(pageref.num,pageref.gen);
              }
              else  page=dest->getPageNum();
              sprintf(buf, "%d", page);
              s = strdup(buf);
              delete dest;
            }
        }
        break;
        case actionGoToR: {
            type = "GoToR";
            LinkGoToR*l = (LinkGoToR*)actionobj;
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
            LinkNamed*l = (LinkNamed*)actionobj;
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
                        if(this->page2page && this->num_pages) {
			    page = this->page2page[this->num_pages-1];
                        }
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
            LinkLaunch*l = (LinkLaunch*)actionobj;
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
            LinkURI*l = (LinkURI*)actionobj;
            GString*g = l->getURI();
            if(g) {
             url = g->getCString();
             s = strdup(url);
            }
        }
        break;
        case actionUnknown: {
            type = "Unknown";
            LinkUnknown*l = (LinkUnknown*)actionobj;
            s = strdup("");
        }
        break;
        default: {
            msg("<error> Unknown link type!");
            break;
        }
    }

    if(!s) s = strdup("-?-");
    
    if(!getGfxGlobals()->linkinfo && (page || s))
    {
        msg("<notice> File contains links");
        getGfxGlobals()->linkinfo = 1;
    }
   
    char*action = 0;
    if(page>0) {
        int t;
	int lpage = -1;
        for(t=1;t<=this->num_pages;t++) {
            if(this->page2page[t]==page) {
		lpage = t;
                break;
	    }
	}
        if(lpage<0) {
	    lpage = page;
        }

	char buf[80];
	sprintf(buf, "page%d", lpage);
	action = buf;
    }
    else if(s)
    {
	action = s;
	if(this->config_linkdatafile) {
	    FILE*fi = fopen(config_linkdatafile, "ab+");
	    fprintf(fi, "%s\n", s);
	    fclose(fi);
	}
    }
    
    this->last_link = new GFXLink(this->last_link, action, x1, y1, x2, y2);
    if(!this->links) {
	this->links = kdtree_new();
    }
    kdtree_add_box(this->links, x1,y1,x2,y2, this->last_link);
#if 0
    printf("adding link %p at %f %f %f %f to tree\n", this->last_link, x1, y1, x2, y2);
#endif

    msg("<verbose> storing \"%s\" link to \"%s\"", type, FIXNULL(action));
    free(s);s=0;
}

void CharOutputDev::saveState(GfxState *state) {
    msg("<trace> saveState %p", state);
    updateAll(state);
};

void CharOutputDev::restoreState(GfxState *state) 
{
  updateAll(state);
}
 
void CharOutputDev::updateFont(GfxState *state) 
{
    GfxFont* gfxFont = state->getFont();
    if (!gfxFont) {
	return; 
    }  
    
    char*id = getFontID(gfxFont);
    msg("<verbose> Updating font to %s", FIXNULL(id));
    free(id);id=0;

    if(gfxFont->getType() == fontType3) {
	infofeature("Type3 fonts");
    }
    updateTextMat(state);
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
    char*r1 = strrchr((char*)filename, '/');
    char*r2 = strrchr((char*)filename, '\\');
    if(r2>r1)
        r1 = r2;
    if(r1) {
        len = strlen(r1+1);
    }
    f->len = len;

    msg("<verbose> Adding font \"%s\".", filename);
    if(global_fonts_next) {
        global_fonts_next->next = f;
        global_fonts_next = global_fonts_next->next;
    } else {
        global_fonts_next = global_fonts = f;
    }
}

void addGlobalLanguageDir(const char*dir)
{
#ifdef HAVE_POPPLER
    msg("<notice> NOT adding %s to language pack directories (not implemented with poppler)", dir);
#else
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
#endif
}

void addGlobalFontDir(const char*dirname)
{
#ifdef HAVE_DIRENT_H
    DIR*dir = opendir(dirname);
    if(!dir) {
	msg("<warning> Couldn't open directory %s", dirname);
	return;
    }
    struct dirent*ent;
    int fonts = 0;
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
            fonts++;
	}
    }
    msg("<notice> Added %s to font directories (%d fonts)", dirname, fonts);
    closedir(dir);
#else
    msg("<warning> No dirent.h");
#endif
}

