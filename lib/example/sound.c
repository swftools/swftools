/* sound.c

   Generates a sine wave and tries to create an swf which
   plays it as mp3 sound.
   
   Part of the swftools package.

   Copyright (c) 2002 Matthias Kramm <kramm@quiss.org>
 
   This file is distributed under the GPL, see file COPYING for details 
*/

#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "../rfxswf.h"

int main (int argc,char ** argv)
{ SWF swf;
  RGBA rgb;
  SRECT r;
  S32 width=300,height = 300;
  U16 block[1152*2];
  TAG * tag;
  
  int f,i,ls1,fs1;
  int count;
  int t;

  memset(&swf,0x00,sizeof(SWF));        // set global movie parameters

  swf.fileVersion    = 4;               // make flash 4 compatible swf
  swf.frameRate      = 19*256;          // about 19 frames per second
  
  swf.movieSize.xmax = 20*width;        // flash units: 1 pixel = 20 units ("twips")
  swf.movieSize.ymax = 20*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  tag = swf.firstTag;
  rgb.r = 0xff;
  rgb.g = 0xff;
  rgb.b = 0xff;
  swf_SetRGB(tag,&rgb);
  
  tag = swf_InsertTag(tag, ST_SOUNDSTREAMHEAD2);
  swf_SetSoundStreamHead(tag, 1152);

  for(t=0;t<64;t++) {
      int s;
      tag = swf_InsertTag(tag, ST_SOUNDSTREAMBLOCK);
      for(s=0;s<1152*2;s++) {
	  block[s] = (int)(32767*sin((s*16*3.14159)/1152));
      }
      swf_SetSoundStreamBlock(tag, block, 1152,1);
      swf_SetSoundStreamBlock(tag, &block[1152], 1152,0);
      tag = swf_InsertTag(tag, ST_SHOWFRAME);
  }

  f = open("sound.swf",O_WRONLY|O_CREAT|O_TRUNC, 0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);                       // cleanup
  return 0;
}


