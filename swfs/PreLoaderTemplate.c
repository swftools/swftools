/* simple_viewer.c

   Creates the swf file PreLoaderTemplate.swf. 
   
   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
   This file is distributed under the GPL, see file COPYING for details 
*/

#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "rfxswf.h"

TAG* tag;

int main (int argc,char ** argv)
{ SWF swf;
  RGBA rgb;
  SRECT r;
  LPSHAPE s;
  MATRIX m;
  ActionTAG*a1,*a2,*a3;
  S32 width = 826, height = 1169;
  
  int f,i,ls1,fs1;
  int count;
  int t;

  memset(&swf,0x00,sizeof(SWF));        // set global movie parameters

  swf.fileVersion    = 4;               // make flash 4 compatible swf
  swf.frameRate      = 0x1900;          // about 0x19 frames per second
  
  swf.movieSize.xmax = 20*width;        // flash units: 1 pixel = 20 units
  swf.movieSize.ymax = 20*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  tag = swf.firstTag;
  rgb.r = 0xff;
  rgb.g = 0xff;
  rgb.b = 0xff;
  swf_SetRGB(tag,&rgb);

  tag = swf_InsertTag(tag,ST_DEFINESPRITE);
  swf_SetU16(tag, 1); //id
  swf_SetU16(tag, 0); //frames
  tag = swf_InsertTag(tag,ST_END);
  tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
  swf_ObjectPlace(tag, 1, 1, 0, 0, "loader");
  
  tag = swf_InsertTag(tag,ST_SHOWFRAME);

  a1 = swf_ActionStart(tag);
    action_PushFloat(12.0);
    action_PushString("");
    action_GetProperty();
    action_PushFloat(2.0);
    action_Less();
    action_If(2);
    action_GotoFrame(1);
    action_End();
  swf_ActionEnd();

  a2 = swf_ActionStart(tag);
    action_Stop();
    action_End();
  swf_ActionEnd();

  tag = swf_InsertTag(tag,ST_DOACTION);
  swf_ActionSet(tag, a1);
  
  tag = swf_InsertTag(tag,ST_SHOWFRAME);
  
  tag = swf_InsertTag(tag,ST_DOACTION);
  swf_ActionSet(tag, a2);
  
  tag = swf_InsertTag(tag,ST_REMOVEOBJECT2);
  swf_SetU16(tag, 1);

  tag = swf_InsertTag(tag,ST_DEFINESPRITE);
  swf_SetU16(tag, 2); //id
  swf_SetU16(tag, 0); //frames
  tag = swf_InsertTag(tag,ST_END);
  tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
  swf_ObjectPlace(tag, 2, 2, 0, 0, "movie");
  
  tag = swf_InsertTag(tag,ST_SHOWFRAME);
  
  tag = swf_InsertTag(tag,ST_END);

  f = open("PreLoaderTemplate.swf",O_WRONLY|O_CREAT|O_TRUNC, 0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);                       // cleanup
  return 0;
}


