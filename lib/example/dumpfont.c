/* dumpfont.c

   Example for font handling

   This tool dumps font information into C files
   which can be included to rfxswf applications.
   This is an easy way for developers to use
   fonts in applications without dealing with
   external files.

   Usage: ./dumpfont filename.swf > myfont.c
   
   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
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
#include <fcntl.h>
#include <unistd.h>
#include "../rfxswf.h"

#define PRINTABLE(a) (((a>0x20)&&(a<0xff)&&(a!='\\'))?a:0x20)

SWF swf;

void DumpFont(SWFFONT * f,char * name)
{ int n,i;
  int*gpos = malloc(sizeof(int*)*f->numchars);
  memset(gpos,0,sizeof(int*)*f->numchars);

  // Glyph Shapes Data

  n = 0;
  printf("U8 Glyphs_%s[] = {\n\t  ",name);
  
  for (i=0;i<f->numchars;i++)
    if (f->glyph[i].shape)
    { SHAPE * s = f->glyph[i].shape;
      int j,l = (s->bitlen+7)/8;
      gpos[i] = n;

      for (j=0;j<l;j++)
      { printf("0x%02x, ",s->data[j]);
        if ((n&7)==7) printf("\n\t  ");
        n++;
      }
    }
  printf("0x00 };\n");
  
  // SWFFONT function

  printf("\nSWFFONT * Font_%s(U16 id)\n",name);
  printf("{ SWFFONT * f;\n  int i;\n\n");
  printf("  if (!(f=malloc(sizeof(SWFFONT)))) return NULL;\n");
  printf("  memset(f,0x00,sizeof(SWFFONT));\n");
  printf("  f->id       = id;\n");
  printf("  f->version  = %d;\n", f->version);
  printf("  f->name     = strdup(\"%s\");\n",f->name);
  printf("  f->style	= 0x%02x;\n",f->style);
  printf("  f->encoding = 0x%02x;\n",f->encoding);
  printf("  f->numchars = %d;\n",f->numchars);
  printf("  f->maxascii = %d;\n",f->maxascii);
  printf("  f->glyph    = (SWFGLYPH*)malloc(sizeof(SWFGLYPH)*%d);\n",f->numchars);
  printf("  f->glyph2ascii = (U16*)malloc(sizeof(U16)*%d);\n",f->numchars);
  printf("  f->ascii2glyph = (int*)malloc(sizeof(int)*%d);\n",f->maxascii);
  printf("  memset(f->ascii2glyph, -1, sizeof(int)*%d);\n\n", f->maxascii);
  if(f->layout) {
      printf("  f->layout = (SWFLAYOUT*)malloc(sizeof(SWFLAYOUT));\n");
      printf("  f->layout->ascent = %d;\n", f->layout->ascent);
      printf("  f->layout->descent = %d;\n", f->layout->descent);
      printf("  f->layout->leading = %d;\n", f->layout->leading);
      printf("  f->layout->kerningcount = 0;\n");
      printf("  f->layout->kerning = 0;\n");
      printf("  f->layout->bounds = (SRECT*)malloc(sizeof(SRECT)*%d);\n", f->numchars);
      printf("  memset(f->layout->bounds, 0, sizeof(SRECT)*%d);\n\n", f->numchars);
  }

  for (i=0;i<f->numchars;i++)
    if (f->glyph[i].shape)
    { 
	printf("  addGlyph(f,%3i, 0x%03x,%4i, &Glyphs_%s[0x%04x], %4i, ",
	       i, f->glyph2ascii[i], f->glyph[i].advance, name, gpos[i],
	       f->glyph[i].shape->bitlen);
	if(f->layout && f->layout->bounds) {
	    printf("%d, %d, %d, %d);",
		    f->layout->bounds[i].xmin,
		    f->layout->bounds[i].ymin,
		    f->layout->bounds[i].xmax,
		    f->layout->bounds[i].ymax);
        } else {
	    printf("/* bbox not set */ 0, 0, 0, 0);");
	}
	printf(" // %c\n", PRINTABLE(f->glyph2ascii[i]));
    }

  printf("  return f;\n}\n\n");
  free(gpos);
}

void DumpGlobal(char * funcname)
{ printf("\nvoid %s(SWFFONT * f,int i,U16 ascii,U16 advance,U8 * data,U32 bitlen,int xmin,int ymin,int xmax, int ymax)\n",funcname);
  printf("{ SHAPE * s;\n  U32 l = (bitlen+7)/8;\n\n");
  printf("  if (FAILED(swf_ShapeNew(&s))) return;\n");
  printf("  s->data = malloc(l);\n");
  printf("  if (!s->data) { swf_ShapeFree(s); return; }\n\n");
  printf("  f->glyph2ascii[i]     = ascii;\n");
  printf("  f->ascii2glyph[ascii] = i;\n");
  printf("  f->glyph[i].advance   = advance;\n");
  printf("  f->glyph[i].shape     = s;\n");
  printf("  s->bitlen             = bitlen;\n");
  printf("  s->bits.fill          = 1;\n");
  printf("  if(f->layout && f->layout->bounds)\n");
  printf("  {  f->layout->bounds[i].xmin = xmin;\n");
  printf("     f->layout->bounds[i].ymin = ymin;\n");
  printf("     f->layout->bounds[i].xmax = xmax;\n");
  printf("     f->layout->bounds[i].ymax = ymax;\n");
  printf("  }\n");
  printf("  memcpy(s->data,data,l);\n}\n\n");
}


void fontcallback(U16 id,U8 * name,void*data)
{ SWFFONT * font;

  int f;
  char s[500],*ss;
  
  swf_FontExtract(&swf,id,&font);
  sprintf(s,"%s%s%s",name,swf_FontIsBold(font)?"_bold":"",swf_FontIsItalic(font)?"_italic":"");
  
  ss = s;
  while(*ss)
  { 
    if(!((*ss>='a' && *ss<='z') ||
         (*ss>='A' && *ss<='Z') ||
         (*ss>='0' && *ss<='9' && ss!=s) ||
         (*ss=='_')))
		*ss = '_';
    ss++;
  }

  DumpFont(font,s);
  
  swf_FontFree(font);
}

int main(int argc,char ** argv)
{ int f;

  if (argc>1)
  { f = open(argv[1],O_RDONLY);
    if (f>=0)
    { if FAILED(swf_ReadSWF(f,&swf))
      { fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",argv[1]);
        close(f);
      }
      else
      { char fn[200];
        close(f);
        sprintf(fn,"fn%04x",getpid()); // avoid name conflicts @ using multiple fonts
        printf("#define addGlyph %s\n",fn);
        DumpGlobal(fn);
        swf_FontEnumerate(&swf,&fontcallback,0);
        swf_FreeTags(&swf);
        printf("#undef addGlyph\n");
      }
    } else fprintf(stderr,"File not found: %s\n",argv[1]);
  }
  else fprintf(stderr,"\nreflex SWF Font Dump Utility\n(w) 2000 by Rainer Boehme <rb@reflex-studio.de>\n\nUsage: dumpfont filename.swf\n");
  
  return 0;
}

