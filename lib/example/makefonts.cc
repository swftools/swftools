/* fontwrite.c

   Example for including and using fonts 
   
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

#define standardEncodingSize 335
#define symbolEncodingSize 257
#define zapfDingbatsEncodingSize 270
#define macRomanEncodingSize 256

extern char *standardEncodingNames[standardEncodingSize];
extern char *symbolEncodingNames[symbolEncodingSize];
extern char *zapfDingbatsEncodingNames[zapfDingbatsEncodingSize];
extern char *macRomanEncodingNames[macRomanEncodingSize];

char*DATADIR = "/usr/local/share/swftools";

extern void drawpath(TAG*tag, T1_OUTLINE*outline, struct swfmatrix*m);

SWFFONT * t1font2swffont(int i)
{
    T1_LoadFont(i);

    float angle = T1_GetItalicAngle(i);
    char*fontname = T1_GetFontName(i);
    char*fullname = T1_GetFullName(i);
    char*familyname = T1_GetFamilyName(i);
    float underline = T1_GetUnderlinePosition(i);
    BBox bbox = T1_GetFontBBox(i);

    char ** encoding = standardEncodingNames;
    int encodingsize = standardEncodingSize;

    printf("processing \"%s\"...\n", fullname);

    if(strstr(fullname, "Dingbats")) {// Zapf Dingbats
	encoding = zapfDingbatsEncodingNames;
	encodingsize = zapfDingbatsEncodingSize;
    }
    else if(strstr(fullname, "Symbol")) {// Symbol
	encoding = symbolEncodingNames;
	encodingsize = zapfDingbatsEncodingSize;
    }

    SWFFONT * wfont = (SWFFONT*)malloc(sizeof(SWFFONT));
    SWFFont * font = new SWFFont("", i, "");

    wfont->version = 2;
    wfont->name = (U8*)fontname;
    wfont->layout = (SWFLAYOUT*)malloc(sizeof(SWFLAYOUT));

    int s,num;
    num = 0;
    for(s=0;s<encodingsize;s++)
    {
	if(encoding[s]) {
	    T1_OUTLINE*outline = font->getOutline(encoding[s]);
	    if(outline && outline->link)
		num++;
	}
    }

    wfont->maxascii = encodingsize;
    wfont->numchars = num;
    wfont->flags = /*layout*/0x80 + /*bold*/0?0:1 + /*italic*/(angle>0.05)?2:0;
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
    wfont->layout->bounds = (SRECT*)malloc(sizeof(SRECT));
    wfont->layout->bounds->xmin = bbox.llx;
    wfont->layout->bounds->ymin = bbox.lly;
    wfont->layout->bounds->xmax = bbox.urx;
    wfont->layout->bounds->ymax = bbox.ury;
    wfont->layout->kerningcount = 0;
    wfont->layout->kerning = 0;
  
    num = 0;
    for(s=0;s<encodingsize;s++)
    {
	if(encoding[s]) {
	    T1_OUTLINE*outline = font->getOutline(encoding[s]);
	    if(outline && outline->link) {
		wfont->ascii2glyph[s] = num;
		wfont->glyph2ascii[num] = s;
		swf_ShapeNew(&wfont->glyph[num].shape);
		SHAPE*shape = wfont->glyph[num].shape;
		wfont->glyph[num].advance = font->getCharWidth(num)/8;
		
		TAG*tag = swf_InsertTag(0,ST_DEFINESHAPE);

		swfmatrix m;
		m.m11 = 1;
		m.m22 = 1;
		m.m21 = 0;
		m.m12 = 0;
		m.m13 = 0;
		m.m23 = 0;

		swf_SetU8(tag,0);

		shape->bits.fill = 1;
		shape->bits.line = 0;
		swf_ShapeSetStyle(tag,shape,0,1,0);
		drawpath(tag, outline, &m);
		swf_ShapeSetEnd(tag);

		wfont->glyph[num].shape->data = &tag->data[1];
		int t;
		for(t=0;t<8;t++)
		    if(tag->writeBit == (0x80>>t)) break;
		if(tag->writeBit == 0)
		    t = 8;
		wfont->glyph[num].shape->bitlen = (tag->len-1)*8;
		num++;
	    }
	}
    }
    return wfont;
}

int main(int argc, char ** argv)
{
  //TODO: use tempnam here. Check if environment already contains a
  //T1LIB_CONFIG.
  putenv( "T1LIB_CONFIG=/tmp/t1lib.config.tmp");
  FILE*fi = fopen("/tmp/t1lib.config.tmp", "wb");
  fprintf(fi, "FONTDATABASE=%s/FontDataBase\n", DATADIR);
  fprintf(fi, "ENCODING=%s:.\n", DATADIR);
  fprintf(fi, "AFM=%s:.\n", DATADIR);
  fprintf(fi, "TYPE1=%s:.\n", DATADIR);
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
//      i = 0;
  {
    SWFFONT * font = t1font2swffont(i);
    
    char filename[128];
    sprintf(filename, "%s.swf", font->name);
    swf_WriteFont(font, filename);
    free(font);
  }
}

