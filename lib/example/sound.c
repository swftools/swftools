/* sound.c

   Generates a sine wave and tries to create an swf which
   plays it as mp3 sound.
   
   Part of the swftools package.

   Copyright (c) 2002 Matthias Kramm <kramm@quiss.org>
 
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
  RGBA rgb;
  SRECT r;
  int blocksize = 2304;
  S32 width=300,height = 300;
  U16 block[blocksize];
  TAG * tag;
  
  int f,i,ls1,fs1;
  int count;
  int t;

  memset(&swf,0x00,sizeof(SWF));        // set global movie parameters

  swf.fileVersion    = 4;               // make flash 4 compatible swf
  swf.frameRate      = 18*256;          // about 18 frames per second
  
  swf.movieSize.xmax = 20*width;        // flash units: 1 pixel = 20 units ("twips")
  swf.movieSize.ymax = 20*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  tag = swf.firstTag;
  rgb.r = 0xff;
  rgb.g = 0xff;
  rgb.b = 0xff;
  swf_SetRGB(tag,&rgb);
  
  tag = swf_InsertTag(tag, ST_SOUNDSTREAMHEAD2);
  swf_SetSoundStreamHead(tag, 11025/18);

  for(t=256;t>=0;t--) {
      int s;
      tag = swf_InsertTag(tag, ST_SOUNDSTREAMBLOCK);
      for(s=0;s<blocksize;s++) {
	  block[s] = (int)(32767*sin(s*8*3.14159/blocksize));
	  if(t==0) block[s] = 0;
      }
      swf_SetSoundStreamBlock(tag, block, 1);
      tag = swf_InsertTag(tag, ST_SHOWFRAME);
  }
  
  tag = swf_InsertTag(tag, ST_END);

  f = open("sound.swf",O_WRONLY|O_CREAT|O_TRUNC, 0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);                       // cleanup
  return 0;
}


