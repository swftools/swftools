/* simple_viewer.c

   Creates the swf file PreLoaderTemplate.swf. 
   
   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
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
#include "rfxswf.h"

TAG* tag;

int main (int argc,char ** argv)
{ SWF swf;
  RGBA rgb;
  SRECT r;
  SHAPE* s;
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

    a1 = action_PushFloat(0, 12.0);
    a1 = action_PushString(a1, "");
    a1 = action_GetProperty(a1);
    a1 = action_PushFloat(a1, 2.0);
    a1 = action_Less(a1);
    a1 = action_If(a1, 2);
    a1 = action_GotoFrame(a1, 1);
    a1 = action_End(a1);

    a2 = action_Stop(0);
    a2 = action_End(a2);

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

  f = open("PreLoaderTemplate.swf",O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);                       // cleanup

  swf_ActionFree(a1);
  swf_ActionFree(a2);

  return 0;
}


