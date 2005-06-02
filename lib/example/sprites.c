/* sprites.c

   Demonstration of using different timelines (sprites)

   Remarks:
   
   * rfxswf uses a flat tag list
     All sprite tags between ST_DEFINESPRITE and ST_END
     will be packed into the data of the ST_DEFINESPRITE
     by the output routines

   * make sure, that all defining tags are placed
     outside the sub-timelines (i.e. before ST_DEFINESPRITE)

   * framerate is global (sad but true!)

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


RGBA rgba(U8 r,U8 g,U8 b,U8 a)
{ RGBA c; c.r = r; c.g = g; c.b = b; c.a = a;
  return c;
}

U16 last_id;
U16 last_depth;

#define GET_ID (last_id++)
#define GET_DEPTH (last_depth++)

int main(int argc, char ** argv)
{ SWF swf;
  RGBA rgb;

  TAG* t;
  SHAPE* s;
  SRECT r;

  U16 id_circle;
  U16 id_sprite;
  U16 id_sprite2;

  int ls,fs,i,j,f;

  U32 width  = 400;
  U32 height = 400;

  last_id = last_depth = 1;

  memset(&swf,0x00,sizeof(SWF));
  
  swf.fileVersion       = 4;
  swf.frameRate         = 0x1000;
  swf.movieSize.xmax    = width*20;
  swf.movieSize.ymax    = height*20;


  t = swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  
        rgb.r = rgb.g = rgb.b = rgb.a = 0xff;
        swf_SetRGB(t,&rgb);

  t = swf_InsertTag(t,ST_DEFINESHAPE);

        swf_ShapeNew(&s);
        rgb.b = rgb.g = 0x00;
        ls = swf_ShapeAddLineStyle(s,40,&rgb);

        id_circle = GET_ID;
        swf_SetU16(t,id_circle);

        r.xmin = 0;
        r.ymin = 0;
        r.xmax = width;
        r.ymax = height;
        swf_SetRect(t,&r);

        swf_SetShapeHeader(t,s);
        swf_ShapeSetAll(t,s,0,0,ls,0,0);
        swf_ShapeSetCircle(t,s,width/2,height/2,width/2,height/2);
        swf_ShapeSetEnd(t);

    // SPRITE #1 TIMELINE
        
    t = swf_InsertTag(t,ST_DEFINESPRITE);
  
        id_sprite = GET_ID;
        swf_SetU16(t,id_sprite);
        swf_SetU16(t,32); // frame count
        
        for (i=0;i<32;i++) // 32 frames
        { MATRIX m;
          swf_GetMatrix(NULL,&m);
          m.tx = width*2+(int)((float)width*2*sin((float)i/16*3.14152));
          m.ty = width*2+(int)((float)height*2*cos((float)i/16*3.14152));
          t = swf_InsertTag(t,ST_PLACEOBJECT2);
              swf_ObjectPlace(t,(i==0)?id_circle:0,1,&m,NULL,NULL);
          t = swf_InsertTag(t,ST_SHOWFRAME);
        }

    t = swf_InsertTag(t,ST_END);

    // SPRITE #2 TIMELINE

    t = swf_InsertTag(t,ST_DEFINESPRITE);
  
        id_sprite2 = GET_ID;
        swf_SetU16(t,id_sprite2);
        swf_SetU16(t,80); // frame count
        
        for (i=0;i<80;i++) // 80 frames
        { MATRIX m;
          swf_GetMatrix(NULL,&m);
          m.tx = width*4+(int)((float)width*4*sin((float)i/40*3.14152));
          m.ty = width*4+(int)((float)height*4*cos((float)i/40*3.14152));
          t = swf_InsertTag(t,ST_PLACEOBJECT2);
              swf_ObjectPlace(t,(i==0)?id_sprite:0,1,&m,NULL,NULL);
          m.sx *= -1;
          m.tx += 4*width;
          m.ty += 4*height;
          t = swf_InsertTag(t,ST_PLACEOBJECT2);
              swf_ObjectPlace(t,(i==0)?id_sprite:0,2,&m,NULL,NULL);
          t = swf_InsertTag(t,ST_SHOWFRAME);
        }

  t = swf_InsertTag(t,ST_END);

  // MAIN MOVIE TIMELINE

  t = swf_InsertTag(t,ST_PLACEOBJECT2);

        swf_ObjectPlace(t,id_sprite2,1,NULL,NULL,NULL);
        
  t = swf_InsertTag(t,ST_SHOWFRAME);    // just one frame

  t = swf_InsertTag(t,ST_END);

//  swf_WriteCGI(&swf);

  f = open("sprites.swf",O_RDWR|O_CREAT|O_TRUNC,0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);

#ifdef __NT__
  system("start ..\\sprites.swf");
#endif
  
  return(0);
}


