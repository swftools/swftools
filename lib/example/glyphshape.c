/* glyphshape.c

   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

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

#include "demofont.c"  // six glyphs only:  f, l, o, s, t, w
//#include "stdfonts.c"

#define BANNER_TEXT     "swftools" 
#define ID_FONT         1000
#define ID_SHAPES       2000 // to 2255

TAG * PlaceGlyph(TAG * t,SWFFONT * font,U8 glyph,U16 depth,int x,int y,int angle)
{ MATRIX m;
  t = swf_InsertTag(t,ST_PLACEOBJECT2);

  swf_GetMatrix(NULL,&m);
  
  if (angle)
  { m.sy = m.sx = (int)(cos(((double)(angle))/128*3.14159)*0x10000);
    m.r0 = (int)(sin(((double)(angle))/128*3.14159)*0x10000);
    m.r1 = -m.r0;
  }
  else m.sx = m.sy = 0x10000;

  // move origin

  m.tx = x+((m.r1-m.sx)/0x100); // this is correct
  m.ty = y+((m.sx-m.r0)/0x100);

//  m.tx = x+((m.r1-m.sx)/0x40); // this looks crazier ;-)
//  m.ty = y+((m.sx-m.r0)/0x40);
  

  if (font)
    swf_ObjectPlace(t,ID_SHAPES+glyph,depth,&m,NULL,NULL);
  else swf_ObjectMove(t,depth,&m,NULL);
        
  return t;
}

// some helper functions which define the figure

int posX(int count)
{ return (int)(-sin(((double)count)/1024*3.14159)*3000);
}
int posY(int count)
{ return (int)(sin(((double)count)/2048*3.14159)*3200);
}
int angle(int count)
{ return (int)(pow(cos(((double)count)/2048*3.14159),3)*110);
  // this is not the mathematical correct solution, but it works & looks nice
}

int main(int argc, char ** argv)
{ SWF swf;
  TAG * t;
  SRECT r;
  RGBA rgb;
  MATRIX m;
  U8 abits, gbits;

  int f,i,frame;
  int width = 400;
  int height = 400;
  char * s = BANNER_TEXT;

  FONTUSAGE use;
  SWFFONT * font = Font_Demo_Font(ID_FONT); // change font name here

  swf_FontInitUsage(font,&use);
  swf_FontUse(&use,s);        
  swf_FontReduce(font,&use);            // make sure that gid's point to the specific glyphs
  
  memset(&swf,0x00,sizeof(SWF));

  swf.fileVersion    = 4;
  swf.frameRate      = 0x1000;
  swf.movieSize.xmax = 20*width;
  swf.movieSize.ymax = 20*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.firstTag;

        rgb.r = 0xff;
        rgb.g = 0xff;
        rgb.b = 0xff;
        swf_SetRGB(t,&rgb);

  // define Glyph Shapes

  for (i=0;i<font->numchars;i++)
  { if (font->glyph[i].shape)
    { SHAPE * s;
      int fs;
        
      t = swf_InsertTag(t,ST_DEFINESHAPE);

        swf_ShapeNew(&s);
        
        rgb.r = rgb.g = rgb.b = i;
        
        swf_ShapeAddSolidFillStyle(s,&rgb);

        swf_SetU16(t,ID_SHAPES+i);

        r.xmin = -50*20;     r.ymin = -50*20;  // Glyph Shapes are placed into a 2000*2000 box
        r.ymax =   50*20;    r.xmax =  50*20;  // where (1000,1000) is the origin

        swf_SetRect(t,&r);

        swf_SetShapeStyles(t,s);        // rest of header is set by swf_SetSimpleShape
        swf_SetSimpleShape(t,font->glyph[i].shape);
        swf_ShapeFree(s);
        
    }
  }

  for (frame=0;frame<128;frame++)
  { int cnt = 4096-frame*32;
    for (i=0;i<strlen(s);i++)
    { int glyph = font->ascii2glyph[s[i]];
      int advance = font->glyph[glyph].advance/2;
      int cnt2 = cnt+=advance/2;
      t = PlaceGlyph(t,frame?NULL:font,glyph,i+1,
                     200*20+posX(cnt2),
                     200*20+posY(cnt2),
                     angle(cnt2));
      cnt+=advance;
    }
    t = swf_InsertTag(t,ST_SHOWFRAME);
  }
  
  
    t = swf_InsertTag(t,ST_END);
 
//  swf_WriteCGI(&swf);
 
  f = open("glyphshape.swf",O_RDWR|O_CREAT|O_TRUNC,0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);

#ifdef __NT__
  system("start ..\\glyphshape.swf");
#endif
  
  return 0;
}


