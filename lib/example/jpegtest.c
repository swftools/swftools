/* jpegtest.c

   Example for including and mapping jpeg images to swf shapes
   
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
#include <math.h>       
#include "../rfxswf.h"

#define WIDTH           256
#define HEIGHT          256
#define QUALITY         85

#define ID_BITS         1
#define ID_SHAPE        2

int main( int argc, char ** argv)
{ SWF swf;
  TAG* t;
  RGBA rgb;
  SHAPE* s;
  MATRIX m;
  SRECT r;
  JPEGBITS* jpeg;

  int f; // file handle
  
  int ls; // line style
  int fs; // fill style
  int frame;

  memset(&swf,0x00,sizeof(SWF));

  swf.fileVersion       = 4;
  swf.frameRate         = 0x1800;
  swf.movieSize.xmax    = 20*WIDTH;
  swf.movieSize.ymax    = 20*HEIGHT;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.firstTag;

    rgb.r = 0xff;
    rgb.b = 0xff;
    rgb.g = 0xff;
    swf_SetRGB(t,&rgb);

  t = swf_InsertTag(t,ST_DEFINEBITSJPEG2);

    swf_SetU16(t,ID_BITS);
//    swf_SetJPEGBits(t,"test.jpg",QUALITY);  <- use this to include an image from disk
    
//  That's the way to use memory bitmaps (24bit,RGB)

    jpeg = swf_SetJPEGBitsStart(t,WIDTH,HEIGHT,QUALITY);
    { int y;
      for (y=0;y<HEIGHT;y++)
      { U8 scanline[3*WIDTH];
        int x,p = 0;
        for (x=0;x<WIDTH;x++) 
        { scanline[p++] = x;    // R
          scanline[p++] = y;    // G
          scanline[p++] = 0x80; // B          
        }
        swf_SetJPEGBitsLine(jpeg,scanline);
      }
    }
    swf_SetJPEGBitsFinish(jpeg);

// do some rotation animation

    for (frame=0;frame<64;frame++)
    {
      t = swf_InsertTag(t,ST_DEFINESHAPE);
    
          swf_ShapeNew(&s);
          
          rgb.b = rgb.g = rgb.r = 0x00;
          ls = swf_ShapeAddLineStyle(s,40,&rgb);
          
          swf_GetMatrix(NULL,&m);
          
          m.sy = m.sx = (int)(cos(((float)(frame))/32*3.141)*0x80000);
          m.r0 = (int)(sin(((float)(frame))/32*3.141)*0x80000);
          m.r1 = -m.r0;
       
          fs = swf_ShapeAddBitmapFillStyle(s,&m,ID_BITS,0);
    
          swf_SetU16(t,ID_SHAPE+frame);   // ID   

          r.xmin = 0;
          r.ymin = 0;
          r.xmax = 10*WIDTH;
          r.ymax = 10*HEIGHT;

          swf_SetRect(t,&r);

          swf_SetShapeHeader(t,s);

          swf_ShapeSetAll(t,s,0,0,ls,fs,0);

          swf_ShapeSetLine(t,s,10*WIDTH,0);
          swf_ShapeSetLine(t,s,-10*WIDTH,10*HEIGHT);
//    swf_ShapeSetLine(t,s,-10*WIDTH,-10*WIDTH);
          swf_ShapeSetLine(t,s,0,-10*HEIGHT);
          swf_ShapeSetEnd(t);

          swf_ShapeFree(s);

      if (frame) 
      { t = swf_InsertTag(t,ST_REMOVEOBJECT2); swf_SetU16(t,1);
        t = swf_InsertTag(t,ST_REMOVEOBJECT2); swf_SetU16(t,2);
      }
         
      t = swf_InsertTag(t,ST_PLACEOBJECT2);
          swf_ObjectPlace(t,ID_SHAPE+frame,1,NULL,NULL,NULL); 

      t = swf_InsertTag(t,ST_PLACEOBJECT2);
          swf_GetMatrix(NULL,&m); // get default matrix with no transformation
          
          m.tx = m.ty = 10*WIDTH+frame*10;
          m.sx = m.sy = 0xfffeffff;
          swf_ObjectPlace(t,ID_SHAPE+frame,2,&m,NULL,NULL);


      t = swf_InsertTag(t,ST_SHOWFRAME);
      
    } // frame loop

  t = swf_InsertTag(t,ST_END);

//  swf_WriteCGI(&swf);

  f = open("jpegtest.swf",O_WRONLY|O_CREAT||O_BINARY, 0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);
  
  swf_FreeTags(&swf); // cleanup
  
  return 0;
}
