/* shape1.c

   Example implementation for creating a button with rfxswf
   
   Part of the swftools package.

   Copyright (c) 2000, 2001 Matthias Kramm <kramm@quiss.org>
                            Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 
*/

#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "../rfxswf.h"

TAG* t;

int useDefineButton2 = 0; // set this to 1 to use DefineButton2 Tags
			  // instead of DefineButton1
               
int main (int argc,char ** argv)
{ SWF swf;
  RGBA rgb;
  SRECT r;
  LPSHAPE s;
  ActionTAG*a1,*a2,*a3, *actiontoset;
  S32 width=300,height = 300;
  
  int f,i,ls1,fs1;
  int count;

  memset(&swf,0x00,sizeof(SWF));        // set global movie parameters

  swf.fileVersion    = 4;               // make flash 4 compatible swf
  swf.frameRate      = 0x1900;          // about 0x19 frames per second
  
  swf.movieSize.xmax = 20*width;        // flash units: 1 pixel = 20 units
  swf.movieSize.ymax = 20*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.firstTag;
  rgb.r = 0xff;
  rgb.g = 0xff;
  rgb.b = 0xff;
  swf_SetRGB(t,&rgb);

  for(count=0;count<4;count++)
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

  a1 = swf_ActionStart(t);
    action_SetTarget("movie");
    action_NextFrame();
    action_SetTarget("");
    action_End();
  swf_ActionEnd();
 
  a2 = swf_ActionStart(t);
    //action_GetUrl("test.swf","_level0"); // load a swf
    //action_GetUrl("index.html","_this"); // load html in new window
    action_GetUrl("http://www.quiss.org/swftools/index.html","_parent"); // load html in this window
    action_End();
  swf_ActionEnd();

  a3 = swf_ActionStart(t);
    action_GotoFrame(33);
    action_End();
  swf_ActionEnd();

  actiontoset = a2;  

  if(!useDefineButton2)
  {
      t = swf_InsertTag(t,ST_DEFINEBUTTON);
      swf_SetU16(t,31); //id
      swf_ButtonSetFlags(t, 0); //menu=no
      swf_ButtonSetRecord(t,0x01,33,1,0,0);
      swf_ButtonSetRecord(t,0x02,34,1,0,0);
      swf_ButtonSetRecord(t,0x04,35,1,0,0);
      swf_ButtonSetRecord(t,0x08,36,1,0,0);
      swf_SetU8(t,0);
      swf_SetActions(t,actiontoset);
      swf_SetU8(t,0);
  }
  else
  {
      t = swf_InsertTag(t,ST_DEFINEBUTTON2);
      swf_SetU16(t,31); //id
      swf_ButtonSetFlags(t, 0); //menu=no
      swf_ButtonSetRecord(t,0x01,33,1,0,0);
      swf_ButtonSetRecord(t,0x02,34,1,0,0);
      swf_ButtonSetRecord(t,0x04,35,1,0,0);
      swf_ButtonSetRecord(t,0x08,36,1,0,0);
      swf_SetU8(t,0);

      swf_ButtonSetCondition(t, 4);
       swf_SetActions(t,actiontoset);
       swf_SetU8(t,0);

      swf_ButtonSetCondition(t, 0);
       swf_SetU8(t,0);

      swf_SetU8(t,0);

      swf_ButtonPostProcess(t, 2);
  }

  t = swf_InsertTag(t,ST_PLACEOBJECT2);
  swf_ObjectPlace(t, 31, 2,0,0,0);

  t = swf_InsertTag(t,ST_SHOWFRAME);
  t = swf_InsertTag(t,ST_END);

  f = open("button.swf",O_WRONLY|O_CREAT, 0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);                       // cleanup
  return 0;
}


