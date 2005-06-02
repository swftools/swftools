/* shape1.c

   Example implementation for drawing a shape with rfxswf
   
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

               
int main (int argc,char ** argv)
{ SWF swf;
  TAG* t;
  RGBA rgb;
  SRECT r;
  SHAPE* s;
  S32 width=300,height = 300;
  
  int f,i,ls1,ls2;

  memset(&swf,0x00,sizeof(SWF));        // set global movie parameters

  swf.fileVersion    = 4;               // make flash 4 compatible swf
  swf.frameRate      = 0x1900;          // about 0x19 frames per second
  
  swf.movieSize.xmax = 20*width;        // flash units: 1 pixel = 20 units
  swf.movieSize.ymax = 20*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);

                                        // now create a tag list be connecting one after another
  
  t = swf.firstTag;

        rgb.r = 0xff;
        rgb.g = 0xff;
        rgb.b = 0xff;
        swf_SetRGB(t,&rgb);

  t = swf_InsertTag(t,ST_DEFINESHAPE);

        swf_ShapeNew(&s);               // create new shape instance

                                        // add two different linestyles
        rgb.b = rgb.g = 0x00;           
        ls1 = swf_ShapeAddLineStyle(s,40,&rgb);
        
        rgb.r = 0; rgb.b = 0xff;
        ls2 = swf_ShapeAddLineStyle(s,40,&rgb);

        swf_SetU16(t,1);                // now set character ID

        r.xmin = 0;
        r.ymin = 0;
        r.xmax = 20*width;
        r.ymax = 20*height;
        
        swf_SetRect(t,&r);              // set shape bounds


        swf_SetShapeHeader(t,s);        // write all styles to tag

        swf_ShapeSetAll(t,s,0,0,ls1,0,0); // move to (0,0), select linestyle ls1 and no fillstyle

                                        
        swf_ShapeSetLine(t,s,10*width,10*height);    // draw something
        swf_ShapeSetStyle(t,s,ls2,0,0);            // change to second linestyle 
        
        for (i=1;i<10;i++)
          swf_ShapeSetCircle(t,s,10*width,10*height,i*width,i*height);
          
        swf_ShapeSetEnd(t);                 // finish drawing

        swf_ShapeFree(s);                   // clean shape structure (which isn't needed anymore after writing the tag)

  t = swf_InsertTag(t,ST_PLACEOBJECT2);     // append tag to place your shape into the scene

        swf_ObjectPlace(t,1,1,NULL,NULL,NULL); // set character with id 1 (our shape) to depth 1 (upper most layer)

  t = swf_InsertTag(t,ST_SHOWFRAME);        // finish current frame

  t = swf_InsertTag(t,ST_END);              // finish current movie (which has just one frame)
  
//  swf_WriteCGI(&swf);    <- use this to create direct CGI output
  
                                            // write movie to file

  f = open("shape1.swf",O_WRONLY|O_CREAT|O_TRUNC, 0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);                       // cleanup

#ifdef __NT__                               // start flash player to show result on windows systems
  system("start ..\\shape1.swf");
#endif
  
  return 0;
}


