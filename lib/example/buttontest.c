/* buttontest.c

   Example implementation for creating a button with rfxswf
   
   Part of the swftools package.

   Copyright (c) 2000, 2001 Matthias Kramm <kramm@quiss.org>
                            Rainer Böhme <rfxswf@reflex-studio.de>

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

TAG* t;

#define ID_BUTTON 31

int useDefineButton2 = 1; // set this to 1 to use DefineButton2 Tags
                          // instead of DefineButton1
               
int main (int argc,char ** argv)
{ SWF swf;
  RGBA rgb;
  SRECT r;
  SHAPE* s;
  ActionTAG*a1,*a2,*a3, *actiontoset;
  S32 width=300,height = 300;
  
  int f,i,ls1,fs1;
  int count;

  memset(&swf,0x00,sizeof(SWF));        // set global movie parameters

  swf.fileVersion    = 8;               // make flash 4 compatible swf
  swf.frameRate      = 0x1900;          // about 0x19 frames per second
  
  swf.movieSize.xmax = 20*width;        // flash units: 1 pixel = 20 units
  swf.movieSize.ymax = 20*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.firstTag;
  rgb.r = 0xff;
  rgb.g = 0xff;
  rgb.b = 0xff;
  swf_SetRGB(t,&rgb);

  for(count=1;count<4;count++)
  {
      t = swf_InsertTag(t,ST_DEFINESHAPE);
      swf_ShapeNew(&s);               // create new shape instance
      rgb.r = rgb.b = rgb.g = 0x00;           
      ls1 = swf_ShapeAddLineStyle(s,40,&rgb);
      rgb.r = count*0x40; rgb.b = 0xff;
      fs1 = swf_ShapeAddSolidFillStyle(s,&rgb);
      swf_SetU16(t,33+count);                // now set character ID
      r.xmin = 0;
      r.ymin = 0;
      r.xmax = 20*width;
      r.ymax = 20*height;
      swf_SetRect(t,&r);              // set shape bounds
      swf_SetShapeHeader(t,s);        // write all styles to tag
      swf_ShapeSetAll(t,s,0,0,ls1,fs1,0); // move to (0,0), select linestyle ls1 and no fillstyle
      swf_ShapeSetCircle(t,s,width*10,height*10,width*10,height*10);
      swf_ShapeSetEnd(t);                 // finish drawing
      swf_ShapeFree(s);                   // clean shape structure (which isn't needed anymore after writing the tag)
  }

  a1 = action_SetTarget(0, "movie");
  a1 = action_NextFrame(a1);
  a1 = action_SetTarget(a1, "");
  a1 = action_End(a1);
 
    //a2 = action_GetUrl(a2, "test.swf","_level0"); // load a swf
    //a2 = action_GetUrl(a2, "index.html","_this"); // load html in new window
    a2 = action_GetUrl(0, "http://www.quiss.org/swftools/index.html","_parent"); // load html in this window
    a2 = action_End(a2);

    a3 = action_GotoFrame(0,33);
    a3 = action_End(a3);

  actiontoset = a2;  

  if(!useDefineButton2)
  {
      t = swf_InsertTag(t,ST_DEFINEBUTTON);
      swf_SetU16(t,31); //id
      swf_ButtonSetRecord(t,BS_UP|BS_HIT,34,1,NULL,NULL);
      swf_ButtonSetRecord(t,BS_OVER,35,1,NULL,NULL);
      swf_ButtonSetRecord(t,BS_DOWN,36,1,NULL,NULL);
      swf_SetU8(t,0); // end of button records
      
      swf_ActionSet(t,actiontoset);
  }
  else
  {
      t = swf_InsertTag(t,ST_DEFINEBUTTON2);
      swf_SetU16(t,ID_BUTTON); //id
      swf_ButtonSetFlags(t, 0); //menu=no
      swf_ButtonSetRecord(t,BS_UP|BS_HIT,34,1,NULL,NULL);
      swf_ButtonSetRecord(t,BS_OVER,35,1,NULL,NULL);
      swf_ButtonSetRecord(t,BS_DOWN,36,1,NULL,NULL);
      swf_SetU8(t,0); // end of button records

      swf_ButtonSetCondition(t, BC_IDLE_OVERUP);
      swf_ActionSet(t,actiontoset);
       
      swf_ButtonPostProcess(t, 1); // don't forget!
  }

  swf_ActionFree(a1);
  swf_ActionFree(a2);
  swf_ActionFree(a3);

  t = swf_InsertTag(t,ST_PLACEOBJECT2);
  swf_ObjectPlace(t, ID_BUTTON, 2,0,0,0);

  t = swf_InsertTag(t,ST_SHOWFRAME);
  t = swf_InsertTag(t,ST_END);

  f = open("buttontest.swf",O_WRONLY|O_CREAT|O_BINARY, 0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);                       // cleanup
  return 0;
}


