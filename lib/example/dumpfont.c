/* dumpfont.c

   Example for font handling

   This tool dumps font information into C files
   which can be included to rfxswf applications.
   This is an easy way for developers to use
   fonts in applications without dealing with
   external files.

   Usage: ./dumpfont filename.swf > myfont.c

   Limits: does not parse ST_DEFINEFONT2 
   
   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "../rfxswf.h"

#define PRINTABLE(a) ((a>='A')&&(a<='Z'))||((a>='a')&&(a<='z'))||((a>='0')&&(a<='9'))

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
  printf("  f->flags    = 0x%02x;\n",f->flags);
  printf("  f->numchars = %d;\n",f->numchars);
  printf("  f->glyph    = (SWFGLYPH*)malloc(sizeof(SWFGLYPH)*%d);\n\n",f->numchars);
  printf("  f->codes    = (U16*)malloc(sizeof(U16)*%d);\n\n",f->numchars);

  for (i=0;i<f->numchars;i++)
    if (f->glyph[i].shape)
    { printf("  addGlyph(f,%3i, 0x%02x,%4i,%3i, &Glyphs_%s[0x%04x],%4i); // %c\n",
             i, f->codes[i], f->glyph[i].advance, f->glyph[i].gid, name, gpos[i],
             f->glyph[i].shape->bitlen,(i!='\\')?i:0x20);
    }

  printf("  return f;\n}\n\n");
  free(gpos);
}

void DumpGlobal(char * funcname)
{ printf("\nvoid %s(SWFFONT * f,int i,U16 code,U16 advance,U16 gid,U8 * data,U32 bitlen)\n",funcname);
  printf("{ SHAPE * s;\n  U32 l = (bitlen+7)/8;\n\n");
  printf("  if (FAILED(swf_ShapeNew(&s))) return;\n");
  printf("  s->data = malloc(l);\n");
  printf("  if (!s->data) { swf_ShapeFree(s); return; }\n\n");
  printf("  f->codes[i]         = code;\n");
  printf("  f->glyph[i].advance = advance;\n");
  printf("  f->glyph[i].gid     = gid;\n");
  printf("  f->glyph[i].shape   = s;\n");
  printf("  s->bitlen           = bitlen;\n");
  printf("  s->bits.fill        = 1;\n");
  printf("  memcpy(s->data,data,l);\n}\n\n");
}


void fontcallback(U16 id,U8 * name)
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
        swf_FontEnumerate(&swf,&fontcallback);
        swf_FreeTags(&swf);
        printf("#undef addGlyph\n");
      }
    } else fprintf(stderr,"File not found: %s\n",argv[1]);
  }
  else fprintf(stderr,"\nreflex SWF Font Dump Utility\n(w) 2000 by Rainer Boehme <rb@reflex-studio.de>\n\nUsage: dumpfont filename.swf\n");
  
  return 0;
}

