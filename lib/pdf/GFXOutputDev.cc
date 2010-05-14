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
#ifdef HAVE_FONTCONFIG
#include <fontconfig.h>
#endif
//xpdf header files
#include "config.h"
#ifdef HAVE_POPPLER
#include <goo/GooString.h>
#include <goo/gfile.h>
#else
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
#include "GFXOutputDev.h"

//  swftools header files
#include "../log.h"
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../gfxfont.h"
#include "../gfxpoly.h"
#include "../devices/record.h"
#include "../devices/ops.h"
#include "../devices/polyops.h"
#include "../devices/render.h"

#include "../png.h"
#include "fonts.h"

#include <math.h>

#define SQRT2 1.41421356237309504880

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


static int verbose = 0;
static int dbgindent = 1;
static void dbg(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(!verbose)
	return;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
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

GFXOutputGlobals*gfxglobals=0;

GFXOutputGlobals::GFXOutputGlobals()
{
    this->featurewarnings = 0;
    this->jpeginfo = 0;
    this->textmodeinfo = 0;
    this->linkinfo = 0;
    this->pbminfo = 0;
}
GFXOutputGlobals::~GFXOutputGlobals()
{
    feature_t*f = this->featurewarnings;
    while(f) {
	feature_t*next = f->next;
	if(f->string) {
	    free(f->string);f->string =0;
	}
	f->next = 0;
	free(f);
	f = next;
    }
    this->featurewarnings = 0;
}

void GFXOutputDev::showfeature(const char*feature, char fully, char warn)
{
    feature_t*f = gfxglobals->featurewarnings;
    while(f) {
	if(!strcmp(feature, f->string))
	    return;
	f = f->next;
    }
    f = (feature_t*)malloc(sizeof(feature_t));
    f->string = strdup(feature);
    f->next = gfxglobals->featurewarnings;
    gfxglobals->featurewarnings = f;
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

    char*styles[] = {"Medium", "Regular", "Bold", "Italic", "Black", "Narrow"};
    char*style = 0;
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
	    if(fc_ismatch(match, family, style)) {
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

GFXOutputDev::GFXOutputDev(InfoOutputDev*info, PDFDoc*doc)
{
    if(!gfxglobals)
        gfxglobals = new GFXOutputGlobals();

    this->info = info;
    this->doc = doc;
    this->xref = doc->getXRef();
    
    this->type3active = 0;
    this->statepos = 0;
    this->xref = 0;
    this->user_movex = 0;
    this->user_movey = 0;
    this->clipmovex = 0;
    this->clipmovey = 0;
    this->user_clipx1 = 0;
    this->user_clipy1 = 0;
    this->user_clipx2 = 0;
    this->user_clipy2 = 0;
    this->current_gfxfont = 0;
    this->current_fontinfo = 0;
    this->current_text_stroke = 0;
    this->current_text_clip = 0;
    this->outer_clip_box = 0;
    this->config_bigchar=0;
    this->config_convertgradients=1;
    this->config_break_on_warning=0;
    this->config_remapunicode=0;
    this->config_transparent=0;
    this->config_extrafontdata = 0;
    this->config_drawonlyshapes = 0;
    this->config_disable_polygon_conversion = 0;
    this->config_multiply = 1;
    this->config_detectspaces = 1;
    this->config_linkdatafile = 0;
    this->page2page = 0;
    this->num_pages = 0;
  
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
    } else if(!strcmp(key,"drawonlyshapes")) {
        this->config_drawonlyshapes = atoi(value);
    } else if(!strcmp(key,"detectspaces")) {
        this->config_detectspaces = atoi(value);
    } else if(!strcmp(key,"extrafontdata")) {
        this->config_extrafontdata = atoi(value);
    } else if(!strcmp(key,"linkdatafile")) {
        this->config_linkdatafile = strdup(value);
    } else if(!strcmp(key,"convertgradients")) {
        this->config_convertgradients = atoi(value);
    } else if(!strcmp(key,"multiply")) {
        this->config_multiply = atoi(value);
        if(this->config_multiply<1) 
            this->config_multiply=1;
    } else if(!strcmp(key,"disable_polygon_conversion")) {
        this->config_disable_polygon_conversion = atoi(value);
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

//void GFXOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) {printf("void GFXOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) \n");}
//void GFXOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) {printf("void GFXOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool inlineImg) \n");}

void dump_outline(gfxline_t*line)
{
    /*gfxbbox_t*r = gfxline_isrectangle(line);
    if(!r)
        printf("is not a rectangle\n");
    else
        printf("is a rectangle: (%f,%f)-(%f-%f)\n", r->xmin, r->ymin, r->xmax, r->ymax);
    */

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

void gfxPath_dump(GfxPath*path)
{
    int num = path->getNumSubpaths();
    int t;
    int cpos=0;
    for(t = 0; t < num; t++) {
	GfxSubpath *subpath = path->getSubpath(t);
	int subnum = subpath->getNumPoints();
        int s;	
        for(s=0;s<subnum;s++) {
	   double x=subpath->getX(s);
           double y=subpath->getY(s);
	   if(s==0 && !subpath->getCurve(s)) {
                printf("M %f %f\n", x, y);
           } else if(s==0 && subpath->getCurve(s)) {
                printf("E %f %f\n", x, y);
	   } else if(subpath->getCurve(s)) {
                printf("C %f %f\n", x, y);
	   } else {
                printf("T %f %f\n", x, y);
	   }
	}
    }
}

gfxline_t* GFXOutputDev::gfxPath_to_gfxline(GfxState*state, GfxPath*path, int closed, int user_movex, int user_movey)
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
	   
	   this->transformXY(state, subpath->getX(s),subpath->getY(s),&x,&y);

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
//    if(config_convertgradients)
//	return gTrue;
    return gFalse;
}
GBool GFXOutputDev::useShadedFills()
{
    infofeature("shaded fills");
    if(config_convertgradients)
	return gTrue;
    return gFalse;
}

void GFXOutputDev::transformXY(GfxState*state, double x, double y, double*nx, double*ny)
{
    state->transform(x,y,nx,ny);
    *nx += user_movex + clipmovex;
    *ny += user_movey + clipmovey;
}


#if (xpdfMajorVersion*10000 + xpdfMinorVersion*100 + xpdfUpdateVersion) < 30207
void GFXOutputDev::tilingPatternFill(GfxState *state, Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep)
#else
void GFXOutputDev::tilingPatternFill(GfxState *state, Gfx *gfx, Object *str,
			       int paintType, Dict *resDict,
			       double *mat, double *bbox,
			       int x0, int y0, int x1, int y1,
			       double xStep, double yStep) 
#endif
{
    msg("<debug> tilingPatternFill");
    infofeature("tiling pattern fills");
}

GBool GFXOutputDev::functionShadedFill(GfxState *state, GfxFunctionShading *shading) 
{
    msg("<error> functionShadedFill not supported yet");
    infofeature("function shaded fills");
    return gFalse;
}
static gfxcolor_t col2col(GfxColorSpace*colspace, GfxColor* col)
{
    gfxcolor_t c;
    GfxRGB rgb;
    colspace->getRGB(col, &rgb);
    c.r = colToByte(rgb.r);
    c.g = colToByte(rgb.g);
    c.b = colToByte(rgb.b);
    c.a = 255;
    return c;
}

GBool GFXOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading)
{
    double x0,y0,r0,x1,y1,x2,y2,x9,y9,r1;
    shading->getCoords(&x0,&y0,&r0,&x9,&y9,&r1);
    x1=x0+r1;y1=y0;
    x2=x0;   y2=y0+r1;
    this->transformXY(state, x0,y0, &x0,&y0);
    this->transformXY(state, x1,y1, &x1,&y1);
    this->transformXY(state, x2,y2, &x2,&y2);
    
    GfxColor color0;
    GfxColor color1;
    GfxColor color2;
    shading->getColor(0.0, &color0);
    shading->getColor(0.5, &color1);
    shading->getColor(1.0, &color2);
  
    GfxColorSpace* colspace = shading->getColorSpace();
    
    msg("<verbose> radialShadedFill %f %f %f %f %f %f %02x%02x%02x->%02x%02x%02x", x0, y0, x1, y1, x2, y2,
	    colToByte(color0.c[0]), colToByte(color0.c[1]), colToByte(color0.c[2]), 
	    colToByte(color1.c[0]), colToByte(color1.c[1]), colToByte(color1.c[2]),
	    colToByte(color2.c[0]), colToByte(color2.c[1]), colToByte(color2.c[2]));
    infofeature("radial shaded fills");

    gfxgradient_t gr[3];
    gfxgradient_t*g = &gr[0];
    g[0].next = &g[1];
    g[1].next = &g[2];
    g[2].next = 0;
    g[0].color = col2col(colspace, &color0);
    g[1].color = col2col(colspace, &color1);
    g[2].color = col2col(colspace, &color2);
    g[0].pos = 0.0;
    g[1].pos = 0.5;
    g[2].pos = 1.0;

    gfxbbox_t b = states[statepos].clipbbox;
    gfxline_t p1,p2,p3,p4,p5;
    p1.type=gfx_moveTo;p1.x=b.xmin; p1.y=b.ymin; p1.next=&p2;
    p2.type=gfx_lineTo;p2.x=b.xmin; p2.y=b.ymax; p2.next=&p3;
    p3.type=gfx_lineTo;p3.x=b.xmax; p3.y=b.ymax; p3.next=&p4;
    p4.type=gfx_lineTo;p4.x=b.xmax; p4.y=b.ymin; p4.next=&p5;
    p5.type=gfx_lineTo;p5.x=b.xmin; p5.y=b.ymin; p5.next=0;
    
    gfxmatrix_t m;
    //m.m00 = (x3-x0); m.m10 = (x1-x0);
    //m.m01 = (y3-y0); m.m11 = (y1-y0);
    //x3/y3 specifies another (the ending) circle, not the second radius of an ellipse
    m.m00 = (x1-x0); m.m10 = (x2-x0);
    m.m01 = (y1-y0); m.m11 = (y2-y0);
    m.tx = x0 - 0.5;
    m.ty = y0 - 0.5;

    device->fillgradient(device, &p1, g, gfxgradient_radial, &m);
    return gTrue;
}

GBool GFXOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading)
{
    double x0,y0,x1,y1;
    shading->getCoords(&x0,&y0,&x1,&y1);
    this->transformXY(state, x0,y0,&x0,&y0);
    this->transformXY(state, x1,y1,&x1,&y1);

    GfxColor color0;
    GfxColor color1;
    GfxColor color2;
    shading->getColor(0.0, &color0);
    shading->getColor(0.5, &color1);
    shading->getColor(1.0, &color2);
    
    GfxColorSpace* colspace = shading->getColorSpace();
    
    msg("<verbose> axialShadedFill %f %f %f %f %02x%02x%02x->%02x%02x%02x->%02x%02x%02x", x0, y0, x1, y1,
	    colToByte(color0.c[0]), colToByte(color0.c[1]), colToByte(color0.c[2]), 
	    colToByte(color1.c[0]), colToByte(color1.c[1]), colToByte(color1.c[2]),
	    colToByte(color2.c[0]), colToByte(color2.c[1]), colToByte(color2.c[2])
	    );
    infofeature("axial shaded fills");

    gfxgradient_t*g = (gfxgradient_t*)malloc(sizeof(gfxgradient_t)*3);
    g[0].next = &g[1];
    g[1].next = &g[2];
    g[2].next = 0;
    g[0].color = col2col(colspace, &color0);
    g[1].color = col2col(colspace, &color1);
    g[2].color = col2col(colspace, &color2);
    g[0].pos = 0.0;
    g[1].pos = 0.5;
    g[2].pos = 1.0;
 
    double xMin,yMin,xMax,yMax;
    state->getUserClipBBox(&xMin, &yMin, &xMax, &yMax);
    this->transformXY(state, xMin, yMin, &xMin, &yMin);
    msg("<verbose> userClipBox %f %f %f %f", xMin, yMin, xMax, yMax);

    xMin = 0; yMin = 0;
    xMin = 1024; yMin = 1024;

    gfxbbox_t b = states[statepos].clipbbox;
    gfxline_t p1,p2,p3,p4,p5;
    p1.type=gfx_moveTo;p1.x=b.xmin; p1.y=b.ymin; p1.next=&p2;
    p2.type=gfx_lineTo;p2.x=b.xmin; p2.y=b.ymax; p2.next=&p3;
    p3.type=gfx_lineTo;p3.x=b.xmax; p3.y=b.ymax; p3.next=&p4;
    p4.type=gfx_lineTo;p4.x=b.xmax; p4.y=b.ymin; p4.next=&p5;
    p5.type=gfx_lineTo;p5.x=b.xmin; p5.y=b.ymin; p5.next=0;
   
    /* the gradient starts at (-1.0,0.0), so move (0,0) to
       the middle of the two control points */
    gfxmatrix_t m;
    m.m00 = (x1-x0)/2; m.m10 = -(y1-y0)/2;
    m.m01 = (y1-y0)/2; m.m11 =  (x1-x0)/2;
    m.tx = (x0 + x1)/2 - 0.5;
    m.ty = (y0 + y1)/2 - 0.5;

    device->fillgradient(device, &p1, g, gfxgradient_linear, &m);

    free(g);
    return gTrue;
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
    msg("<verbose> endPage (GfxOutputDev)");
    if(outer_clip_box) {
	device->endclip(device);
	outer_clip_box = 0;
    }
    /* notice: we're not fully done yet with this page- there might still be 
       a few calls to drawLink() yet to come */
}

static inline double sqr(double x) {return x*x;}

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
    else msg("<error> Invalid line cap type");

    gfx_joinType joinType = gfx_joinRound;
    if(lineJoin == 0) joinType = gfx_joinMiter;
    else if(lineJoin == 1) joinType = gfx_joinRound;
    else if(lineJoin == 2) joinType = gfx_joinBevel;
    else msg("<error> Invalid line join type");

    gfxline_t*line2 = 0;

    int dashLength = states[statepos].dashLength;
    double*dashPattern = states[statepos].dashPattern;
    double dashStart = states[statepos].dashStart;
    if(dashLength && dashPattern) {
	float * dash = (float*)malloc(sizeof(float)*(dashLength+1));
	int t;

        /* try to find out how much the transformation matrix would
           stretch the dashes, and factor that into the dash lengths.
           This is not the entirely correct approach- it would be 
           better to first convert the path to an unscaled version,
           then apply dashing, and then transform the path using
           the current transformation matrix. However there are few
           PDFs which actually stretch a dashed path in a non-orthonormal
           way */
        double tx1, ty1, tx2, ty2, tx3, ty3;
	this->transformXY(state, 0, 0, &tx1, &ty1);
	this->transformXY(state, 0, 1, &tx2, &ty2);
	this->transformXY(state, 1, 0, &tx3, &ty3);
        double d1 = sqrt(sqr(tx2-tx1)+sqr(ty2-ty1));
        double d2 = sqrt(sqr(tx3-tx1)+sqr(ty3-ty1));
        if(fabs(d1-d2)>0.5)
            warnfeature("non-ortogonally dashed strokes", 0);
        double f = (d1+d2)/2;

	msg("<trace> %d dashes", dashLength);
	msg("<trace> |  phase: %f", dashStart);
	for(t=0;t<dashLength;t++) {
	    dash[t] = (float)dashPattern[t] * f;
            if(!dash[t]) {
                dash[t] = 1e-37;
            }
	    msg("<trace> |  d%-3d: %f", t, dashPattern[t]);
	}
	dash[dashLength] = -1;
	if(getLogLevel() >= LOGLEVEL_TRACE) {
	    dump_outline(line);
	}
        
        line2 = gfxtool_dash_line(line, dash, (float)(dashStart*f));
        line = line2;

	free(dash);
	msg("<trace> After dashing:");
    }
    
    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> stroke width=%f join=%s cap=%s dashes=%d color=%02x%02x%02x%02x",
		width,
		lineJoin==0?"miter": (lineJoin==1?"round":"bevel"),
		lineCap==0?"butt": (lineCap==1?"round":"square"),
		dashLength,
		col.r,col.g,col.b,col.a
		);
        dump_outline(line);
    }

    if(flags&STROKE_FILL) {
        gfxpoly_t* poly = gfxpoly_from_stroke(line, width, capType, joinType, miterLimit, DEFAULT_GRID);
        gfxline_t*gfxline = gfxline_from_gfxpoly(poly);
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
        gfxline_free(gfxline);
	gfxpoly_destroy(poly);
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

void GFXOutputDev::fillGfxLine(GfxState *state, gfxline_t*line, char evenodd) 
{
    gfxcolor_t col = getFillColor(state);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> %sfill %02x%02x%02x%02x", evenodd?"eo":"", col.r, col.g, col.b, col.a);
        dump_outline(line);
    }
    device->fill(device, line, &col);
}

void GFXOutputDev::clipToGfxLine(GfxState *state, gfxline_t*line, char evenodd) 
{
    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        msg("<trace> %sclip", evenodd?"eo":"");
        dump_outline(line);
    }
    gfxbbox_t bbox = gfxline_getbbox(line);
    gfxbbox_intersect(&states[statepos].clipbbox, &bbox);

    device->startclip(device, line);
    states[statepos].clipping++;
}

void GFXOutputDev::clip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    msg("<trace> clip");
    gfxline_t*line = gfxPath_to_gfxline(state, path, 1, user_movex + clipmovex, user_movey + clipmovey);
    if(!config_disable_polygon_conversion) {
	gfxline_t*line2 = gfxpoly_circular_to_evenodd(line, DEFAULT_GRID);
	gfxline_free(line);
	line = line2;
    }
    clipToGfxLine(state, line, 0);
    gfxline_free(line);
}

void GFXOutputDev::eoClip(GfxState *state) 
{
    GfxPath * path = state->getPath();
    gfxline_t*line = gfxPath_to_gfxline(state, path, 1, user_movex + clipmovex, user_movey + clipmovey);
    clipToGfxLine(state, line, 1);
    gfxline_free(line);
}
void GFXOutputDev::clipToStrokePath(GfxState *state)
{
    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 0, user_movex + clipmovex, user_movey + clipmovey);

    if(getLogLevel() >= LOGLEVEL_TRACE)  {
        double width = state->getTransformedLineWidth();
        msg("<trace> cliptostrokepath width=%f", width);
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

    gfxmatrix_t m = this->current_font_matrix;
    this->transformXY(state, x-originX, y-originY, &m.tx, &m.ty);
    //m.tx += originX; m.ty += originY;
    
    msg("<debug> drawChar(%f,%f,c='%c' (%d), u=%d <%d>) CID=%d render=%d glyphid=%d font=%p",m.tx,m.ty,(charid&127)>=32?charid:'?', charid, u, uLen, font->isCIDFont(), render, glyphid, current_gfxfont);

    if((render == RENDER_FILL && !config_drawonlyshapes) ||
       (render == RENDER_FILLSTROKE && state->getTransformedLineWidth()<1.0) ||
       (render == RENDER_INVISIBLE)) {

	int space = this->current_fontinfo->space_char;
	if(config_extrafontdata && config_detectspaces && space>=0 && m.m00 && !m.m01) {
	    /* space char detection */
	    if(last_char_gfxfont == current_gfxfont && 
	       last_char_y == m.ty &&
	       !last_char_was_space) {
		double expected_x = last_char_x + current_gfxfont->glyphs[last_char].advance*m.m00;
		int space = this->current_fontinfo->space_char;
		float width = this->current_fontinfo->average_advance;
		if(m.tx - expected_x >= m.m00*width*4/10) {
		    msg("<debug> There's a %f pixel gap between char %d and char %d (expected no more than %f), I'm inserting a space here", 
			    m.tx-expected_x, 
			    width*m.m00*4/10,
			    last_char, glyphid);
		    gfxmatrix_t m2 = m;
		    m2.tx = expected_x + (m.tx - expected_x - current_gfxfont->glyphs[space].advance*m.m00)/2;
		    if(m2.tx < expected_x) m2.tx = expected_x;
		    device->drawchar(device, current_gfxfont, space, &col, &m2);
		}
	    }
	    last_char_gfxfont = current_gfxfont;
	    last_char = glyphid;
	    last_char_x = m.tx;
	    last_char_y = m.ty;
	    last_char_was_space = GLYPH_IS_SPACE(&current_gfxfont->glyphs[glyphid]);
	}
	device->drawchar(device, current_gfxfont, glyphid, &col, &m);
    } else {
	msg("<debug> Drawing glyph %d as shape", charid);
	if(!gfxglobals->textmodeinfo) {
	    msg("<notice> Some texts will be rendered as shape");
	    gfxglobals->textmodeinfo = 1;
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
    msg("<trace> endString() render=%d textstroke=%p", render, current_text_stroke);
    
    if(current_text_stroke) {
	/* fillstroke and stroke text rendering objects we can process right
	   now (as there may be texts of other rendering modes in this
	   text object)- clipping objects have to wait until endTextObject,
	   however */
	device->setparameter(device, "mark","TXT");
	if((render&3) == RENDER_FILL) {
	    fillGfxLine(state, current_text_stroke, 0);
	    gfxline_free(current_text_stroke);
	    current_text_stroke = 0;
	} else if((render&3) == RENDER_FILLSTROKE) {
	    fillGfxLine(state, current_text_stroke, 0);
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
    msg("<trace> endTextObject() render=%d textstroke=%p clipstroke=%p", render, current_text_stroke, current_text_clip);
    
    if(current_text_clip) {
	device->setparameter(device, "mark","TXT");
	clipToGfxLine(state, current_text_clip, 0);
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
	this->transformXY(state, 0, 0, &m.tx, &m.ty);

	/*m.m00*=INTERNAL_FONT_SIZE;
	m.m01*=INTERNAL_FONT_SIZE;
	m.m10*=INTERNAL_FONT_SIZE;
	m.m11*=INTERNAL_FONT_SIZE;*/

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
    } else {
        x1 += this->clipmovex;
        y1 += this->clipmovey;
        x2 += this->clipmovex;
        y2 += this->clipmovey;
    }

    //msg("<verbose> Bounding box is (%f,%f)-(%f,%f) [shifted by %d/%d]", x1,y1,x2,y2, user_movex, user_movey);
    
    msg("<notice> processing PDF page %d (%dx%d:%d:%d) (move:%d:%d)", pageNum, (int)x2-(int)x1,(int)y2-(int)y1, (int)x1, (int)y1, user_movex + clipmovex, user_movey + clipmovey);
    if(rot!=0)
        msg("<verbose> page is rotated %d degrees", rot);

    clippath[0].type = gfx_moveTo;clippath[0].x = x1; clippath[0].y = y1; clippath[0].next = &clippath[1];
    clippath[1].type = gfx_lineTo;clippath[1].x = x2; clippath[1].y = y1; clippath[1].next = &clippath[2];
    clippath[2].type = gfx_lineTo;clippath[2].x = x2; clippath[2].y = y2; clippath[2].next = &clippath[3];
    clippath[3].type = gfx_lineTo;clippath[3].x = x1; clippath[3].y = y2; clippath[3].next = &clippath[4];
    clippath[4].type = gfx_lineTo;clippath[4].x = x1; clippath[4].y = y1; clippath[4].next = 0;
    device->startclip(device, clippath); outer_clip_box = 1;
    if(!config_transparent) {
        device->fill(device, clippath, &white);
    }
    states[statepos].clipbbox.xmin = x1;
    states[statepos].clipbbox.ymin = x1;
    states[statepos].clipbbox.xmax = x2;
    states[statepos].clipbbox.ymax = y2;
    
    states[statepos].dashPattern = 0;
    states[statepos].dashLength = 0;
    states[statepos].dashStart = 0;
    
    this->last_char_gfxfont = 0;
}


void GFXOutputDev::processLink(Link *link, Catalog *catalog)
{
    double x1, y1, x2, y2;
    gfxline_t points[5];
    int x, y;
    
    msg("<debug> drawlink");

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
    
    msg("<trace> drawlink %.2f/%.2f %.2f/%.2f %.2f/%.2f %.2f/%.2f",
	    points[0].x, points[0].y,
	    points[1].x, points[1].y,
	    points[2].x, points[2].y,
	    points[3].x, points[3].y); 
    
    if(getLogLevel() >= LOGLEVEL_TRACE)  {
	dump_outline(points);
    }

    LinkAction*action=link->getAction();
    char buf[128];
    char*s = 0;
    const char*type = "-?-";
    char*named = 0;
    int page = -1;
    msg("<trace> drawlink action=%d", action->getKind());
    switch(action->getKind())
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
            msg("<error> Unknown link type!");
            break;
        }
    }

    if(!s) s = strdup("-?-");
    
    msg("<trace> drawlink s=%s", s);

    if(!gfxglobals->linkinfo && (page || s))
    {
        msg("<notice> File contains links");
        gfxglobals->linkinfo = 1;
    }
    
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
	device->drawlink(device, points, buf);
    }
    else if(s)
    {
        device->drawlink(device, points, s);
	if(this->config_linkdatafile) {
	    FILE*fi = fopen(config_linkdatafile, "ab+");
	    fprintf(fi, "%s\n", s);
	    fclose(fi);
	}
    }
        
    msg("<verbose> \"%s\" link to \"%s\" (%d)", type, FIXNULL(s), page);
    free(s);s=0;
}

void GFXOutputDev::saveState(GfxState *state) {
    dbg("saveState %p", state); dbgindent+=2;

    msg("<trace> saveState %p", state);
    updateAll(state);
    if(statepos>=64) {
      msg("<fatal> Too many nested states in pdf.");
      exit(1);
    }
    statepos ++;
    states[statepos].state = state;
    states[statepos].createsoftmask = states[statepos-1].createsoftmask;
    states[statepos].transparencygroup = states[statepos-1].transparencygroup;
    states[statepos].clipping = 0;
    states[statepos].olddevice = 0;
    states[statepos].clipbbox = states[statepos-1].clipbbox;

    states[statepos].dashPattern = states[statepos-1].dashPattern;
    states[statepos].dashStart = states[statepos-1].dashStart;
    states[statepos].dashLength = states[statepos-1].dashLength;
};

void GFXOutputDev::restoreState(GfxState *state) {
  dbgindent-=2; dbg("restoreState %p", state);

  if(statepos==0) {
      msg("<fatal> Invalid restoreState");
      exit(1);
  }
  msg("<trace> restoreState %p%s%s", state,
	                          states[statepos].softmask?" (end softmask)":"",
	                          states[statepos].clipping?" (end clipping)":"");
  if(states[statepos].softmask) {
      clearSoftMask(state);
  }

  if(states[statepos].dashPattern) {
      if(!statepos || states[statepos-1].dashPattern != states[statepos].dashPattern) {
          free(states[statepos].dashPattern);
          states[statepos].dashPattern = 0;
      }
  }

  updateAll(state);
  
  while(states[statepos].clipping) {
      device->endclip(device);
      states[statepos].clipping--;
  }
  if(states[statepos].state!=state) {
      msg("<fatal> bad state nesting");
      if(verbose) {
	  int t;
	  for(t=0;t<=statepos;t++) {
	      printf("%p ", states[t].state);
	  }
	  printf("\n");
      }
      exit(1);
  }
  states[statepos].state=0;
  statepos--;
}
 
void GFXOutputDev::updateLineDash(GfxState *state) 
{
    if(states[statepos].dashPattern &&
       (!statepos || states[statepos-1].dashPattern != states[statepos].dashPattern)) {
        free(states[statepos].dashPattern);
        states[statepos].dashPattern = 0;
    }
    double *pattern = 0;
    int dashLength;
    double dashStart;
    state->getLineDash(&pattern, &dashLength, &dashStart);
    msg("<debug> updateLineDash, %d dashes", dashLength);
    if(!dashLength) {
        states[statepos].dashPattern = 0;
        states[statepos].dashLength = 0;
    } else {
        double*p = (double*)malloc(dashLength*sizeof(states[statepos].dashPattern[0]));
        memcpy(p, pattern, dashLength*sizeof(states[statepos].dashPattern[0]));
        states[statepos].dashPattern = p;
        states[statepos].dashLength = dashLength;
        states[statepos].dashStart = dashStart;
    }
}
  
void GFXOutputDev::setPageMap(int*page2page, int num_pages)
{
    this->page2page = page2page;
    this->num_pages = num_pages;
}

void GFXOutputDev::updateLineWidth(GfxState *state)
{
    double width = state->getTransformedLineWidth();
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
	msg("<error> Internal Error: no fontinfo for font %s", id);
	return;
    }
    if(!this->current_fontinfo->seen) {
	dumpFontInfo("<verbose>", gfxFont);
    }

    current_gfxfont = this->current_fontinfo->getGfxFont();
    device->addfont(device, current_gfxfont);
    free(id);
    
    updateFontMatrix(state);
}

#define SQR(x) ((x)*(x))

unsigned char* antialize(unsigned char*data, int width, int height, int newwidth, int newheight, int palettesize)
{
    if((newwidth<1 || newheight<1) ||
       (width<=newwidth || height<=newheight))
	return 0;
    unsigned char*newdata;
    int x,y;
    newdata= (unsigned char*)malloc(newwidth*newheight);
    double fx = ((double)width)/newwidth;
    double fy = ((double)height)/newheight;
    double px = 0;
    int blocksize = (int)(8192/(fx*fy));
    int r = 8192*256/palettesize;
    for(x=0;x<newwidth;x++) {
	double ex = px + fx;
	int fromx = (int)px;
	int tox = (int)ex;
	int xweight1 = (int)((1-(px-fromx))*256);
	int xweight2 = (int)((ex-tox)*256);
	double py =0;
	for(y=0;y<newheight;y++) {
	    double ey = py + fy;
	    int fromy = (int)py;
	    int toy = (int)ey;
	    int yweight1 = (int)((1-(py-fromy))*256);
	    int yweight2 = (int)((ey-toy)*256);
	    int a = 0;
	    int xx,yy;
            if(tox>=width) 
                tox = width-1;
            if(toy>=height) 
                toy = height-1;
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
        double x4,double y4, int type, int multiply)
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
        
    m.tx = p1.x - 0.5*multiply;
    m.ty = p1.y - 0.5*multiply;

    gfximage_t img;
    img.data = (gfxcolor_t*)data;
    img.width = sizex;
    img.height = sizey;
  
    if(type == IMAGE_TYPE_JPEG)
	/* TODO: pass image_dpi to device instead */
	dev->setparameter(dev, "next_bitmap_is_jpeg", "1");

    dump_outline(&p1);
    dev->fillbitmap(dev, &p1, &img, &m, 0);
}

void drawimagejpeg(gfxdevice_t*dev, gfxcolor_t*mem, int sizex,int sizey, 
        double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4, int multiply)
{
    drawimage(dev,mem,sizex,sizey,x1,y1,x2,y2,x3,y3,x4,y4, IMAGE_TYPE_JPEG, multiply);
}

void drawimagelossless(gfxdevice_t*dev, gfxcolor_t*mem, int sizex,int sizey, 
        double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4, int multiply)
{
    drawimage(dev,mem,sizex,sizey,x1,y1,x2,y2,x3,y3,x4,y4, IMAGE_TYPE_LOSSLESS, multiply);
}


void GFXOutputDev::drawGeneralImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GfxImageColorMap*colorMap, GBool invert,
				   GBool inlineImg, int mask, int*maskColors,
				   Stream *maskStr, int maskWidth, int maskHeight, GBool maskInvert, GfxImageColorMap*maskColorMap)
{
  /* the code in this function is *old*. It's not pretty, but it works. */

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

  if(!width || !height || ((height+width)<=1 && (maskWidth+maskHeight)<=1))
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

  this->transformXY(state, 0, 1, &x1, &y1);
  this->transformXY(state, 0, 0, &x2, &y2);
  this->transformXY(state, 1, 0, &x3, &y3);
  this->transformXY(state, 1, 1, &x4, &y4);

  if(type3active) {
      /* as type 3 bitmaps are antialized, we need to place them
	 at integer coordinates, otherwise flash player's antializing
	 will kick in and make everything blurry */
      x1 = (int)(x1);y1 = (int)(y1);
      x2 = (int)(x2);y2 = (int)(y2);
      x3 = (int)(x3);y3 = (int)(y3);
      x4 = (int)(x4);y4 = (int)(y4);
  }

  if(!gfxglobals->pbminfo && !(str->getKind()==strDCT)) {
      if(!type3active) {
	  msg("<notice> File contains pbm pictures %s",mask?"(masked)":"");
	  gfxglobals->pbminfo = 1;
      }
      if(mask)
      msg("<verbose> drawing %d by %d masked picture", width, height);
  }
  if(!gfxglobals->jpeginfo && (str->getKind()==strDCT)) {
      msg("<notice> File contains jpeg pictures");
      gfxglobals->jpeginfo = 1;
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
     
      if(type3active) {
	  unsigned char*pic2 = 0;
	  numpalette = 16;
	  
	  pic2 = antialize(pic,width,height,realwidth,realheight,numpalette);

	  if(!pic2) {
	    delete[] pic;
	    delete imgStr;
	    return;
	  }
          
	  width = realwidth;
	  height = realheight;
	  delete[] pic;
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
      drawimagelossless(device, pic2, width, height, x1,y1,x2,y2,x3,y3,x4,y4, config_multiply);
      delete[] pic2;
      delete[] pic;
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
              int x1 = x*maskWidth/width;
              int y1 = y*maskHeight/height;
              int x2 = (x+1)*maskWidth/width;
              int y2 = (y+1)*maskHeight/height;
              int xx,yy;
              unsigned int alpha=0;
              unsigned int count=0;
              for(xx=x1;xx<x2;xx++)
              for(yy=y1;yy<y2;yy++) {
                  alpha += maskbitmap[yy*maskWidth+xx];
                  count ++;
              }
              if(count) {
                pic[width*y+x].a = alpha / count;
              } else {
                pic[width*y+x].a = maskbitmap[y1*maskWidth+x1];
              }
	  }
	}
      }
      if(str->getKind()==strDCT)
	  drawimagejpeg(device, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4, config_multiply);
      else
	  drawimagelossless(device, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4, config_multiply);
      delete[] pic;
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
	  pal[t].r = (unsigned char)(colToByte(rgb.r));
	  pal[t].g = (unsigned char)(colToByte(rgb.g));
	  pal[t].b = (unsigned char)(colToByte(rgb.b));
	  pal[t].a = 255;//(U8)(rgb.b * 255 + 0.5);
      }
      for (y = 0; y < height; ++y) {
	for (x = 0; x < width; ++x) {
	  imgStr->getPixel(pixBuf);
	  pic[width*y+x] = pal[pixBuf[0]];
	  if(maskColors && *maskColors==pixBuf[0]) {
	      pic[width*y+x].a = 0;
	  }
	}
      }
      if(maskbitmap) {
	  if(maskWidth < width && maskHeight < height) {
	      for(y = 0; y < height; y++) {
		  for (x = 0; x < width; x++) {
		      pic[width*y+x].a = maskbitmap[(y*maskHeight/height)*maskWidth+(x*maskWidth/width)];
		  }
	      }
	  } else {
	      msg("<verbose> resampling %dx%d to mask size (%dx%d)", width, height, maskWidth, maskHeight);
	      gfxcolor_t*newpic=new gfxcolor_t[maskWidth*maskHeight];
	      double dx = width / (double)maskWidth;
	      double dy = height / (double)maskHeight;
	      double yy = 0;
	      for(y = 0; y < maskHeight; y++) {
		  double xx = 0;
		  for (x = 0; x < maskWidth; x++) {
		      newpic[maskWidth*y+x] = pic[int(yy)*width+int(xx)];
		      newpic[maskWidth*y+x].a = maskbitmap[maskWidth*y+x];
		      xx += dx;
		  }
		  yy += dy;
	      }
	      delete[] pic;
	      pic = newpic;
	      width = maskWidth;
	      height = maskHeight;
	  }
      }
      drawimagelossless(device, pic, width, height, x1,y1,x2,y2,x3,y3,x4,y4, config_multiply);

      delete[] pic;
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
	msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d", colorMap->getNumPixelComps(),
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
	msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d", colorMap->getNumPixelComps(),
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
	msg("<verbose> colorMap pixcomps:%d bits:%d mode:%d", colorMap->getNumPixelComps(),
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
    if(!config_disable_polygon_conversion) {
        gfxline_t*line2 = gfxpoly_circular_to_evenodd(line, DEFAULT_GRID);
        gfxline_free(line);
        line = line2;
    }
    fillGfxLine(state, line, 0);
    gfxline_free(line);
}

void GFXOutputDev::eoFill(GfxState *state) 
{
    gfxcolor_t col = getFillColor(state);
    dbg("eofill %02x%02x%02x%02x",col.r,col.g,col.b,col.a);

    GfxPath * path = state->getPath();
    gfxline_t*line= gfxPath_to_gfxline(state, path, 1, user_movex + clipmovex, user_movey + clipmovey);
    fillGfxLine(state, line, 1);
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

void GFXOutputDev::beginTransparencyGroup(GfxState *state, double *bbox,
				      GfxColorSpace *blendingColorSpace,
				      GBool isolated, GBool knockout,
				      GBool forSoftMask)
{
    const char*colormodename = "";
  
    if(blendingColorSpace) {
	colormodename = GfxColorSpace::getColorSpaceModeName(blendingColorSpace->getMode());
    }
    dbg("beginTransparencyGroup device=%p %.1f/%.1f/%.1f/%.1f %s isolated=%d knockout=%d forsoftmask=%d", device, bbox[0],bbox[1],bbox[2],bbox[3], colormodename, isolated, knockout, forSoftMask);
    msg("<verbose> beginTransparencyGroup %.1f/%.1f/%.1f/%.1f %s isolated=%d knockout=%d forsoftmask=%d", bbox[0],bbox[1],bbox[2],bbox[3], colormodename, isolated, knockout, forSoftMask);
    
    //states[statepos].createsoftmask |= forSoftMask;
    states[statepos].createsoftmask = forSoftMask;
    states[statepos].transparencygroup = !forSoftMask;
    states[statepos].isolated = isolated;

    states[statepos].olddevice = this->device;
    this->device = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));
    dbg("this->device now %p (old: %p)", this->device, states[statepos].olddevice);

    gfxdevice_record_init(this->device, 0);
    
    /*if(!forSoftMask) { ////???
	state->setFillOpacity(0.0);
    }*/
    dbgindent+=2;
}

void GFXOutputDev::endTransparencyGroup(GfxState *state)
{
    dbgindent-=2;
    gfxdevice_t*r = this->device;

    dbg("endTransparencyGroup this->device now back to %p (destroying %p)", states[statepos].olddevice, this->device);
    
    this->device = states[statepos].olddevice;
    if(!this->device) {
	msg("<error> Invalid state nesting");
    }
    states[statepos].olddevice = 0;

    gfxresult_t*recording = r->finish(r);
    
    dbg("                     forsoftmask=%d recording=%p/%p", states[statepos].createsoftmask, r, recording);
    msg("<verbose> endTransparencyGroup forsoftmask=%d recording=%p/%p", states[statepos].createsoftmask, r, recording);

    if(states[statepos].createsoftmask) {
	states[statepos-1].softmaskrecording = recording;
    } else {
	states[statepos-1].grouprecording = recording;
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

    dbg("paintTransparencyGroup blend=%s softmaskon=%d recording=%p", blendmodes[state->getBlendMode()], states[statepos].softmask, states[statepos].grouprecording);
    msg("<verbose> paintTransparencyGroup blend=%s softmaskon=%d", blendmodes[state->getBlendMode()], states[statepos].softmask);

    if(state->getBlendMode() == gfxBlendNormal)
	infofeature("transparency groups");
    else {
	char buffer[80];
	sprintf(buffer, "%s blended transparency groups", blendmodes[state->getBlendMode()]);
	warnfeature(buffer, 0);
    }

    gfxresult_t*grouprecording = states[statepos].grouprecording;
  
    int blendmode = state->getBlendMode();
    if(blendmode == gfxBlendNormal || blendmode == gfxBlendMultiply) {
	int alpha = (int)(state->getFillOpacity()*255);
	if(blendmode == gfxBlendMultiply && alpha>200)
	    alpha = 128;
	gfxdevice_t ops;
	dbg("this->device=%p, this->device->name=%s\n", this->device, this->device->name);
	gfxdevice_ops_init(&ops, this->device, alpha);
	gfxresult_record_replay(grouprecording, &ops, 0);
	ops.finish(&ops);
    }
    grouprecording->destroy(grouprecording);

    states[statepos].grouprecording = 0;
}

void GFXOutputDev::setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *rgb)
{
    if(states[statepos].softmask) {
	/* shouldn't happen, but *does* happen */
	clearSoftMask(state);
    }

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
   
    if(states[statepos].olddevice) {
	msg("<fatal> Internal error: badly balanced softmasks/transparency groups");
	exit(1);
    }
    states[statepos].olddevice = this->device;
    this->device = (gfxdevice_t*)rfx_calloc(sizeof(gfxdevice_t));
    gfxdevice_record_init(this->device, 0);

    dbg("softmaskrecording is %p (dev=%p) at statepos %d\n", states[statepos].softmaskrecording, this->device, statepos);
    
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
    msg("<verbose> clearSoftMask statepos=%d", statepos);
    
    if(!states[statepos].softmaskrecording || strcmp(this->device->name, "record")) {
	msg("<error> Error in softmask/tgroup ordering");
	return;
    }
  
    gfxresult_t*mask = states[statepos].softmaskrecording;
    gfxresult_t*below = this->device->finish(this->device);free(this->device);
    this->device = states[statepos].olddevice;

    /* get outline of all objects below the soft mask */
    gfxdevice_t uniondev;
    gfxdevice_union_init(&uniondev, 0);
    gfxresult_record_replay(below, &uniondev, 0);
    gfxline_t*belowoutline = gfxdevice_union_getunion(&uniondev);
    uniondev.finish(&uniondev);
    gfxbbox_t bbox = gfxline_getbbox(belowoutline);
    gfxline_free(belowoutline);belowoutline=0;
#if 0 
    this->device->startclip(this->device, belowoutline);
    gfxresult_record_replay(below, this->device, 0);
    gfxresult_record_replay(mask, this->device, 0);
    this->device->endclip(this->device);
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
    gfxresult_record_replay(below, &belowrender, 0);
    belowrender.endpage(&belowrender);
    gfxresult_t* belowresult = belowrender.finish(&belowrender);
    gfximage_t* belowimg = (gfximage_t*)belowresult->get(belowresult,"page0");
    //writePNG("below.png", (unsigned char*)belowimg->data, belowimg->width, belowimg->height);

    gfxdevice_t maskrender;
    gfxdevice_render_init(&maskrender);
    maskrender.startpage(&maskrender, width, height);
    gfxresult_record_replay(mask, &maskrender, 0);
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

