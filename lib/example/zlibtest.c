/* zlibtest.c

   Little example for rfxswf's lossless bitmap functions.
   This program gives swf cgi output of three zlib compressed
   images: 8 bit indexed, 16 and 32 bit

   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

// There's no makefile so try to compile like this on linux/gcc:
// cc zlibtest.c ../rfxswf.c -funsigned-char -o zlibtest -lm -ljpeg -lz; cp zlibtest /home/www/cgi-bin/zlibtest

#include <stdio.h>
#include <math.h>
#include <zlib.h>     
#include "../rfxswf.h"

#define WIDTH           256
#define HEIGHT          256

#define ID_BITS         1
#define ID_SHAPE        16


int main ( int argc, char ** argv)
{ SWF swf;
  LPTAG t;
  RGBA rgb;
  LPSHAPE s;
  MATRIX m;
  SRECT r;
  LPJPEGBITS jpeg;
  int i;
  
  int ls; // line style
  int fs; // fill style

  int dx = 256; // bitmap size
  int dy = 256;
  int bps8, bps16, bps32;    // bytes per scanline

  U8 * bitmap8;
  U16 * bitmap16;
  RGBA * bitmap32;
  RGBA * pal;

  // create test texture

  bps8  = BYTES_PER_SCANLINE(dx*sizeof(U8));
  bps16 = BYTES_PER_SCANLINE(dx*sizeof(U16));
  bps32 = BYTES_PER_SCANLINE(dx*sizeof(U32));
  
  pal = malloc(256*sizeof(RGBA));

  bitmap8 = malloc(bps8*dy);
  bitmap16 = malloc(bps16*dy);
  bitmap32 = malloc(bps32*dy);
  
  if ((bitmap8) && (pal) && (bitmap16))
  { int x,y;
    for (y=0;y<dy;y++)
      for (x=0;x<dx;x++)
        bitmap8[y*bps8+x] = (y/16)*16+(x/16);

    for (x=0;x<256;x++)
    { pal[x].r = (x&0xf)*16;
      pal[x].g = (x*2)&0xff;
      pal[x].b = x&0xf0;
      pal[x].a = (x==0xff)?0:0xff;
    }

    for (y=0;y<dy;y++)
      for (x=0;x<dx;x++)
        bitmap16[y*(bps16>>1)+x] = ((x&0xf0)==(y&0xf0))?0xffff:(x&0x0f)<(y&0xf)?BM16_RED|BM16_GREEN:BM16_BLUE;

    for (y=0;y<dy;y++)
      for (x=0;x<dx;x++)
        { bitmap32[y*(bps32>>2)+x].r = /*((x&0x10)==(y&0x10))?*/((x&4)==(y&4))?y:x;
          bitmap32[y*(bps32>>2)+x].g = x;
          bitmap32[y*(bps32>>2)+x].b = y;
        }

  } 
  
  // put texture into flash movie

  memset(&swf,0x00,sizeof(SWF));

  swf.FileVersion       = 4;
  swf.FrameRate         = 0x1800;
  swf.MovieSize.xmax    = 20*WIDTH;
  swf.MovieSize.ymax    = 20*HEIGHT;

  swf.FirstTag = InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.FirstTag;

    rgb.r = 0xff;
    rgb.b = 0xff;
    rgb.g = 0xff;
    SetRGB(t,&rgb);

  t = InsertTag(t,ST_DEFINEBITSLOSSLESS);

    SetU16(t,ID_BITS);
    SetLosslessBits(t,dx,dy,bitmap32,BMF_32BIT);
    
  t = InsertTag(t,ST_DEFINEBITSLOSSLESS2);

    /* be careful with ST_DEFINEBITSLOSSLESS2, because
       the Flash player produces great bugs if you use too many
       alpha colors in your palette. The only sensible result that
       can be archeived is setting one color to r=0,b=0,g=0,a=0 to
       make transparent parts in sprites. That's the cause why alpha
       handling is implemented in lossless routines of rfxswf.
    */

    SetU16(t,ID_BITS+1);
    SetLosslessBitsIndexed(t,dx,dy,bitmap8,pal,256);
    
  t = InsertTag(t,ST_DEFINEBITSLOSSLESS);

    SetU16(t,ID_BITS+2);
    SetLosslessBits(t,dx,dy,bitmap16,BMF_16BIT);

  /* By the way: ST_DEFINELOSSLESS2 produces stange output on
     16 and 32 bits image data, too.... it seems that the
     ming developers deal with the same problem.
  */

  for (i=0;i<9;i++)
  {
    t = InsertTag(t,ST_DEFINESHAPE);
    
    NewShape(&s);
    rgb.b = rgb.g = rgb.r = 0x00;
    ls = ShapeAddLineStyle(s,10,&rgb);  

    GetMatrix(NULL,&m);
    m.sx = (6*WIDTH/dx)<<16;
    m.sy = (6*HEIGHT/dy)<<16;

    fs = ShapeAddBitmapFillStyle(s,&m,ID_BITS+((i+(i/3))%3),0);
    
    SetU16(t,ID_SHAPE+i);   // ID   

    r.xmin = 0;
    r.ymin = 0;
    r.xmax = 6*WIDTH;
    r.ymax = 6*HEIGHT;

    SetRect(t,&r);

    SetShapeStyles(t,s);
    ShapeCountBits(s,NULL,NULL);
    SetShapeBits(t,s);

    ShapeSetAll(t,s,0,0,ls,fs,0);

    ShapeSetLine(t,s,6*WIDTH,0);
    ShapeSetLine(t,s,0,6*HEIGHT);
    ShapeSetLine(t,s,-6*WIDTH,0);
    ShapeSetLine(t,s,0,-6*HEIGHT);
    ShapeSetEnd(t);

    GetMatrix(NULL,&m);
    m.tx = (i%3) * (6*WIDTH+60);
    m.ty = (i/3) * (6*HEIGHT+60);

  t = InsertTag(t,ST_PLACEOBJECT2);
    ObjectPlace(t,ID_SHAPE+i,1+i,&m,NULL,NULL);
  }

  t = InsertTag(t,ST_SHOWFRAME);

  t = InsertTag(t,ST_END);

  WriteCGI(&swf);
  FreeTags(&swf);

  free(pal);
  free(bitmap8);
  free(bitmap16);
  free(bitmap32);  
  return 0;
}
