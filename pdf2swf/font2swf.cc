/* makefonts.cc

   Utility for converting Type 1 fonts to SWF.
   
   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "swfoutput.h"
#include "spline.h"

static char * filenames[256];
static int filenum;
static char * destfilename = "output.swf";
int all=0;
int verbose=0;

struct options_t options[] =
{
 {"o","output"},
 {"v","verbose"},
 {"V","version"},
 {0,0}
};
int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("font2swf - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    }
    else if(!strcmp(name, "o")) {
	destfilename = val;
	return 1;
    }
    else if(!strcmp(name, "v")) {
	verbose ++;
	return 0;
    }
    else {
        printf("Unknown option: -%s\n", name);
	exit(1);
    }
    return 0;
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char*name)
{    
    printf("Usage: %s file.afm [...]\n", name);
    printf("   OR: %s all\n", name);
    printf("\n");
    printf("\tIf \"all\" is given instead of font names, all standard fonts\n");
    printf("\t(Courier, Arial etc.) will be created\n");
    printf("\n");
    printf("Options:\n");
    printf("\n");
    printf("\t-h , --help\t\t Print help and exit\n");
    printf("\t-o , --output filename\t set output filename\n");
    printf("\t-v , --verbose\t\t Be more verbose\n");
    printf("\t-V , --version\t\t Print program version and exit\n");
}
int args_callback_command(char*name,char*val)
{
    if(!strcmp(name, "all"))
	all = 1;
    else {
	filenames[filenum++] = strdup(name);
    }
    return 0;
}


#define standardEncodingSize 256
#define symbolEncodingSize 256
#define zapfDingbatsEncodingSize 256
#define macRomanEncodingSize 256
#define winAnsiEncodingSize 256

extern char *winAnsiEncoding[winAnsiEncodingSize];
extern char *standardEncoding[standardEncodingSize];
extern char *symbolEncoding[symbolEncodingSize];
extern char *zapfDingbatsEncoding[zapfDingbatsEncodingSize];
extern char *macRomanEncoding[macRomanEncodingSize];

extern void drawpath(TAG*tag, T1_OUTLINE*outline, struct swfmatrix*m, int log);
extern void resetdrawer();
extern void moveto(TAG*tag, plotxy p0);
extern void lineto(TAG*tag, plotxy p0);

SWFFONT * t1font2swffont(int i)
{
    T1_LoadFont(i);

    float angle = T1_GetItalicAngle(i);
    char*fontname = T1_GetFontName(i);
    char*fullname = T1_GetFullName(i);
    char*familyname = T1_GetFamilyName(i);
    float underline = T1_GetUnderlinePosition(i);
    BBox bbox = T1_GetFontBBox(i);

    /* if "all" is given, translate the font names in something more
       readable */
    if(!strcmp(fullname, "Nimbus Roman No9 L Regular")) fontname = "Helvetica";
    if(!strcmp(fullname, "Nimbus Roman No9 L Regular Italic")) fontname = "HelveticaItalic";
    if(!strcmp(fullname, "Nimbus Roman No9 L Medium")) fontname = "HelveticaBold";
    if(!strcmp(fullname, "Nimbus Roman No9 L Medium Italic")) fontname = "HelveticaBoldItalic";
    if(!strcmp(fullname, "Nimbus Sans L Regular")) fontname = "Times";
    if(!strcmp(fullname, "Nimbus Sans L Regular Italic")) fontname = "TimesItalic";
    if(!strcmp(fullname, "Nimbus Sans L Bold")) fontname = "TimesBold";
    if(!strcmp(fullname, "Nimbus Sans L Bold Italic")) fontname = "TimesBoldItalic";
    if(!strcmp(fullname, "Nimbus Mono L Regular")) fontname = "Courier";
    if(!strcmp(fullname, "Nimbus Mono L Regular Oblique")) fontname = "CourierItalic";
    if(!strcmp(fullname, "Nimbus Mono L Bold")) fontname = "CourierBold";
    if(!strcmp(fullname, "Nimbus Mono L Bold Oblique")) fontname = "CourierBoldItalic";
    if(!strcmp(fullname, "Standard Symbols L")) fontname = "Symbol";

    char ** encoding = winAnsiEncoding;
    int encodingsize = winAnsiEncodingSize;

    printf("processing \"%s\" (\"%s\")...\n", fullname, fontname);

    if(strstr(fullname, "Dingbats")) {// Zapf Dingbats
	encoding = zapfDingbatsEncoding;
	encodingsize = zapfDingbatsEncodingSize;
    }
    else if(strstr(fullname, "Symbol")) {// Symbol
	encoding = symbolEncoding;
	encodingsize = symbolEncodingSize;
    }

    SWFFONT * wfont = (SWFFONT*)malloc(sizeof(SWFFONT));
    SWFFont * font = new SWFFont("", i, "");

    memset(wfont, 0, sizeof(SWFFONT));

    wfont->version = 2;
    wfont->name = (U8*)strdup(fontname);
    wfont->layout = (SWFLAYOUT*)malloc(sizeof(SWFLAYOUT));
    memset(wfont->layout, 0, sizeof(SWFLAYOUT));

    int s,num;
    num = 0;
    for(s=0;s<encodingsize;s++)
    {
	char charinfont=0;
	if(encoding[s]) {
	    T1_OUTLINE*outline = font->getOutline(encoding[s], 0);
	    if(outline) {num++;charinfont=1;}
	}
	/*if(charinfont) printf("x");
	else printf(".");
	if((s&15) == 15)
	    printf("\n");*/
    }

    wfont->maxascii = encodingsize;
    wfont->numchars = num;
    
    wfont->style = (/*bold*/0?FONT_STYLE_BOLD:0) + (angle>0.05?FONT_STYLE_ITALIC:0);

    wfont->glyph = (SWFGLYPH*)malloc(num*sizeof(SWFGLYPH));
    memset(wfont->glyph, 0, num*sizeof(SWFGLYPH));
    wfont->glyph2ascii = (U16*)malloc(num*sizeof(U16));
    memset(wfont->glyph2ascii, 0, num*sizeof(U16));
    wfont->ascii2glyph = (int*)malloc(encodingsize*sizeof(int));
    memset(wfont->ascii2glyph, -1, encodingsize*sizeof(int));
    wfont->layout->ascent = (U16)(underline - bbox.lly);
    wfont->layout->descent = (U16)(bbox.ury - underline);
    wfont->layout->leading = (U16)(wfont->layout->ascent - 
	                     wfont->layout->descent -
			     (bbox.lly - bbox.ury));
    wfont->layout->bounds = (SRECT*)malloc(sizeof(SRECT)*num);
    memset(wfont->layout->bounds, 0, sizeof(SRECT)*num);
    wfont->layout->kerningcount = 0;
    wfont->layout->kerning = 0;
  
    num = 0;
    for(s=0;s<encodingsize;s++)
    {
	if(encoding[s]) {
	    T1_OUTLINE*outline = font->getOutline(encoding[s],0);
	    int width = font->getWidth(encoding[s]);

	    if(outline) {
		int log = 0;
		wfont->ascii2glyph[s] = num;
		wfont->glyph2ascii[num] = s;
		swf_ShapeNew(&wfont->glyph[num].shape);
		SHAPE*shape = wfont->glyph[num].shape;
		int firstx = outline->dest.x/0xffff;
		
		TAG*tag = swf_InsertTag(0,ST_DEFINESHAPE);

		swfmatrix m;
		m.m11 = 1.0;
		m.m22 = 1.0;
		m.m21 = 0;
		m.m12 = 0;
		m.m13 = 0;
		m.m23 = 0;

		swf_SetU8(tag,0);

		shape->bits.fill = 1;
		shape->bits.line = 0;
		swf_ShapeSetStyle(tag,shape,0,1,0);
		resetdrawer();

		drawpath(tag, outline, &m, log);

		/*uncomment this to mark the glyph sizes:
		plotxy p1,p2; p1.x=0; p1.y=0; p2.x=width/8; p2.y=-width/8;
		moveto(tag, p1); lineto(tag, p2); p1.x += 2; p2.x += 2;
		lineto(tag, p2); lineto(tag, p1); p1.x -= 2; lineto(tag, p1);// */

		swf_ShapeSetEnd(tag);

		wfont->glyph[num].shape->bitlen = (tag->len-1)*8;
		wfont->glyph[num].shape->data = (U8*)malloc(tag->len-1);
		memcpy(wfont->glyph[num].shape->data, &tag->data[1], tag->len-1);
		swf_DeleteTag(tag);

		/* fix bounding box */
		SHAPE2*shape2;
		SRECT bbox;
		shape2 = swf_ShapeToShape2(shape);
		if(!shape2) { fprintf(stderr, "Shape parse error\n");exit(1);}
		bbox = swf_GetShapeBoundingBox(shape2);
		swf_Shape2Free(shape2);
		wfont->layout->bounds[num] = bbox;
		//wfont->glyph[num].advance = (int)(width/6.4); // 128/20
		wfont->glyph[num].advance = bbox.xmax/20;
		if(!wfont->glyph[num].advance) {
		    wfont->glyph[num].advance = firstx;
		}
		
		num++;
	    }
	}
    }
    return wfont;
}

int main(int argc, char ** argv)
{
  char cwd[128];
  getcwd(cwd, 128);
  processargs(argc, argv);
  if(!all && !filenum) {
      fprintf(stderr, "You must supply a filename.\n");
      exit(1);
  }
  //TODO: use tempnam here. Check if environment already contains a
  //T1LIB_CONFIG.
  putenv( "T1LIB_CONFIG=/tmp/t1lib.config.tmp");
  FILE*fi = fopen("/tmp/t1lib.config.tmp", "wb");
  if(all) {
      fprintf(fi, "FONTDATABASE=/tmp/FontDataBase\n");
      fprintf(fi, "ENCODING=%s/fonts:.\n", SWFTOOLS_DATADIR);
      fprintf(fi, "AFM=%s/fonts:.\n", SWFTOOLS_DATADIR);
      fprintf(fi, "TYPE1=%s/fonts:.\n", SWFTOOLS_DATADIR);
  } else {
      fprintf(fi, "FONTDATABASE=/tmp/FontDataBase\n");
      fprintf(fi, "ENCODING=%s:.\n", cwd);
      fprintf(fi, "AFM=%s:.\n", cwd);
      fprintf(fi, "TYPE1=%s:.\n", cwd);
  }
  fclose(fi);
  fi = fopen("/tmp/FontDataBase", "wb");
  if(all) {
      fprintf(fi, "14\n");             
      fprintf(fi, "%s/fonts/n021003l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n021023l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n021004l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n021024l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n019003l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n019023l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n019004l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n019024l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n022003l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n022023l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n022004l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/n022024l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/s050000l.afm\n", SWFTOOLS_DATADIR);
      fprintf(fi, "%s/fonts/d050000l.afm\n", SWFTOOLS_DATADIR);
  } else {
    fprintf(fi, "%d\n", filenum);
    int t;
    for(t=0;t<filenum;t++) {
	fprintf(fi, "%s/%s\n", cwd, filenames[t]);
    }
  }
  fclose(fi);
  /* initialize t1lib */
  T1_SetBitmapPad( 16);
  if ((T1_InitLib(NO_LOGFILE)==NULL)){
      fprintf(stderr, "Initialization of t1lib failed\n");
      exit(1);
  }
  unlink("/tmp/t1lib.config.tmp");

  int i,num;
  for( i=0; i<T1_Get_no_fonts(); i++)
  {
    SWFFONT * font = t1font2swffont(i);
    
    char filename[128];
    sprintf(filename, "%s.swf", font->name);
    swf_WriteFont(font, filename);
    swf_FontFree(font);
  }
  unlink("/tmp/FontDataBase");
}


