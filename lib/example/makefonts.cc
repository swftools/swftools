/* makefonts.cc

   Utility for generating the standard fonts (arial, courier, etc.) in swf-format. 
   
   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
   This file is distributed under the GPL, see file COPYING for details 

*/


#include <stdio.h>
#include <fcntl.h>
#include <math.h>
extern "C" {
#include "../rfxswf.h"
}
#include "../../pdf2swf/swfoutput.h"
#include "../../pdf2swf/spline.h"

#define standardEncodingSize 256
#define symbolEncodingSize 256
#define zapfDingbatsEncodingSize 256
#define macRomanEncodingSize 256

extern char *standardEncoding[standardEncodingSize];
extern char *symbolEncoding[symbolEncodingSize];
extern char *zapfDingbatsEncoding[zapfDingbatsEncodingSize];
extern char *macRomanEncoding[macRomanEncodingSize];

char*DATADIR = "/usr/local/share/swftools";

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

    char ** encoding = standardEncoding;
    int encodingsize = standardEncodingSize;

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

    wfont->version = 2;
    wfont->name = (U8*)strdup(fontname);
    wfont->layout = (SWFLAYOUT*)malloc(sizeof(SWFLAYOUT));
    memset(wfont->layout, 0, sizeof(SWFLAYOUT));

    int s,num;
    num = 0;
    for(s=0;s<encodingsize;s++)
    {
	if(encoding[s]) {
	    T1_OUTLINE*outline = font->getOutline(encoding[s], 0);
	    if(outline && outline->link)
		num++;
	}
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
	    if(outline && outline->link) {
		int log = 0;
		wfont->ascii2glyph[s] = num;
		wfont->glyph2ascii[num] = s;
		swf_ShapeNew(&wfont->glyph[num].shape);
		SHAPE*shape = wfont->glyph[num].shape;
		wfont->glyph[num].advance = (int)(width/6.4); // 128/20
		
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
		bbox = swf_GetShapeBoundingBox(shape2->lines);
		swf_Shape2Free(shape2);
		wfont->layout->bounds[num] = bbox;
		
		num++;
	    }
	}
    }
    return wfont;
}

int main(int argc, char ** argv)
{
  int all=0;
  if(argc<=1) {
      printf("Usage: %s font.afm\n", argv[0]);
      printf("OR:    %s all\n", argv[0]);
      printf("\n");
      printf("\tIf \"all\" is given instead of font names, all standard fonts\n");
      printf("\t(Courier, Arial etc.) will be created\n");
      return 0;
  } else {
      if(!strcmp(argv[1],"all"))
	  all=1;
  }
  //TODO: use tempnam here. Check if environment already contains a
  //T1LIB_CONFIG.
  putenv( "T1LIB_CONFIG=/tmp/t1lib.config.tmp");
  FILE*fi = fopen("/tmp/t1lib.config.tmp", "wb");
  fprintf(fi, "FONTDATABASE=/tmp/FontDataBase\n", DATADIR);
  fprintf(fi, "ENCODING=%s/fonts:.\n", DATADIR);
  fprintf(fi, "AFM=%s/fonts:.\n", DATADIR);
  fprintf(fi, "TYPE1=%s/fonts:.\n", DATADIR);
  fclose(fi);
  fi = fopen("/tmp/FontDataBase", "wb");
  if(all) {
      fprintf(fi, "14\n");             
      fprintf(fi, "n021003l.afm\n"); //fixme
      fprintf(fi, "n021023l.afm\n");
      fprintf(fi, "n021004l.afm\n");
      fprintf(fi, "n021024l.afm\n");
      fprintf(fi, "n019003l.afm\n");
      fprintf(fi, "n019023l.afm\n");
      fprintf(fi, "n019004l.afm\n");
      fprintf(fi, "n019024l.afm\n");
      fprintf(fi, "n022003l.afm\n");
      fprintf(fi, "n022023l.afm\n");
      fprintf(fi, "n022004l.afm\n");
      fprintf(fi, "n022024l.afm\n");
      fprintf(fi, "s050000l.afm\n");
      fprintf(fi, "d050000l.afm\n");
  } else {
    fprintf(fi, "%d\n",argc-1);
    int t;
    for(t=1;t<argc;t++) {
	fprintf(fi, "%s\n",argv[t]);
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
//      i = 4;
  {
    SWFFONT * font = t1font2swffont(i);
    
    char filename[128];
    sprintf(filename, "%s.swf", font->name);
    swf_WriteFont(font, filename);
    swf_FontFree(font);
  }
}


