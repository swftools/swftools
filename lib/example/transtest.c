/* transtest.c

   Example for transforming a textured triangle
   
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
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

#define QUALITY 80
#define ID_BITMAP       2004
#define ID_SHAPE        2005

  
int main (int argc,char ** argv)
{ SWF swf;
  LPTAG t;
  RGBA rgb;
  SRECT r;
  LPSHAPE s;
  S32 width = 800,height = 800;
  int fs,ls; // line & fillstyle
  LPJPEGBITS jpeg;
  MATRIX m;  
  
  int f,i,j,frame;
  
  memset(&swf,0x00,sizeof(SWF));

  swf.fileVersion    = 4;
  swf.frameRate      = 0x4000;
  swf.movieSize.xmax = 4*width;
  swf.movieSize.ymax = 4*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.firstTag;

        rgb.r = 0xff;
        rgb.g = 0xff;
        rgb.b = 0xff;
        swf_SetRGB(t,&rgb);

    t = swf_InsertTag(t,ST_DEFINEBITSJPEG2);

         swf_SetU16(t,ID_BITMAP);
         
         if (FAILED(swf_SetJPEGBits(t,"texture.jpg",QUALITY)))
         { fprintf(stderr,"Error: texture.jpg (256x256) not found in work directory.\n");
           exit(1);
         }
         
    t = swf_InsertTag(t,ST_DEFINESHAPE);
    
         swf_ShapeNew(&s);
         rgb.b = rgb.g = rgb.r = 0x00;
         ls = 0;
         rgb.b = 0xff;
  
         fs = swf_ShapeAddBitmapFillStyle(s,NULL,ID_BITMAP,0);
    
         swf_SetU16(t,ID_SHAPE);    // ID   
         
         r.xmin = 0;
         r.ymin = 0;
         r.xmax = 2*width;
         r.ymax = 2*height;
         
         swf_SetRect(t,&r);

         swf_SetShapeHeader(t,s);
         
         swf_ShapeSetAll(t,s,0,0,ls,fs,0);
         swf_ShapeSetLine(t,s,width,0);
         swf_ShapeSetLine(t,s,-width,height);
         swf_ShapeSetLine(t,s,0,-height);
         swf_ShapeSetEnd(t);

         swf_ShapeFree(s);
    
  for (frame=0;frame<64;frame++)
  {

    /* Test procedure for swf_MatrixJoin

    MATRIX m1,m2;

    // set m1 to left rotation
     
    m1.sy = m1.sx = (int)(cos(((float)(frame))/32*3.141)*0x10000);
    m1.r0 = (int)(sin(((float)(frame))/32*3.141)*0x10000);
    m1.r1 = -m1.r0;
    m1.tx = width+frame*4; m1.ty = height;

    // set m2 to right rotation

    m2.sy = m2.sx = (int)(cos(((float)(64-frame))/32*3.141)*0x10000);
    m2.r0 = (int)(sin(((float)(64-frame))/32*3.141)*0x10000);
    m2.r1 = -m2.r0;
    m2.tx = width; m2.ty = height;

    // joining m1 and m2 should lead to no transformation

    swf_MatrixJoin(&m,&m1,&m2);

    */

    int dx0 = width;    // constants of shape 
    int dy0 = width;

    int px0 = 2*width;  // destination of mapping
    int py0 = 2*width;

    int px1 = 3*width;
    int py1 = 2*width-frame*4;

    int px2 = 2*width-frame*8;
    int py2 = 3*width;

    swf_MatrixMapTriangle(&m,dx0,dy0,px0,py0,px1,py1,px2,py2);

    t = swf_InsertTag(t,ST_PLACEOBJECT2);

        if (!frame)
         swf_ObjectPlace(t,ID_SHAPE,1,&m,NULL,NULL);
        else
          swf_ObjectMove(t,1,&m,NULL);
 
    t = swf_InsertTag(t,ST_SHOWFRAME);
  }
  
  
  t = swf_InsertTag(t,ST_END);
  
//  swf_WriteCGI(&swf);

  f = open("transtest.swf",O_RDWR|O_CREAT|O_TRUNC,0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);

#ifdef __NT__
  system("start ..\\transtest.swf");
#endif
  
  return 0;
}


