/* text.c

   Example for including and using fonts 
   
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

#include "demofont.c"  // five glyphs only:  f, l, o, s, w

/*
    Due to copyright reasons we don't include full typesets into
    the swftools package. But you can easily create fontdumps
    of your desired typo:

    * Create a swf file with all characters of your desired fonts
      (with any tool that can output swf files)

    * use the dumpfont example in this directory to dump font code

    * include dump code and adjust Font_<Fontname>() calls.

    Note: pdf2swf (Version 0.1.0) doesn't write ST_DEFINEFONTINFO tags,
    so you can't extract fonts out of documents made with pdf2swf. 
   
*/

#define BANNER_TEXT     "swftools" 
#define ID_FONT         2000
#define ID_BANNER       2001

int main(int argc, char ** argv)
{ SWF swf;
  TAG * t;
  SRECT r;
  RGBA rgb;
  U8 abits, gbits;
  int definefont2 = 1;

  int f;
  int width = 170;
  int height = 60;

  int points = 50; // <- change global text size here
  
  int textsize = points*20;   // adjust height
  int textscale = points*10;  // adjust spacing
  
  FONTUSAGE use;
  SWFFONT * font = Font_Demo_Font(ID_FONT); // change font name here

  /* adding layout to a font has the side effect that the 
     advance information is stored in the font itself, not
     in accompanying textfields- this is needed e.g. for
     edittext tags */
  if(definefont2)
      swf_FontAddLayout(font,0,0,0);

  swf_FontInitUsage(&use);
  swf_FontUse(&use,BANNER_TEXT);        // SWF reduces font information to the used glyphs
  swf_FontReduce(font,&use);  
    
  memset(&swf,0x00,sizeof(SWF));

  swf.fileVersion    = 4;
  swf.frameRate      = 0x4000;
  swf.movieSize.xmax = 20*width;
  swf.movieSize.ymax = 20*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.firstTag;

        rgb.r = 0xff;
        rgb.g = 0xff;
        rgb.b = 0xff;
        swf_SetRGB(t,&rgb);
  
  if(definefont2) {
      t = swf_InsertTag(t,ST_DEFINEFONT2);
	    swf_FontSetDefine2(t, font);
  } else {
      t = swf_InsertTag(t,ST_DEFINEFONT);
	    swf_FontSetDefine(t, font);
      t = swf_InsertTag(t,ST_DEFINEFONTINFO);
            swf_FontSetInfo(t, font);
  }

  t = swf_InsertTag(t,ST_DEFINETEXT);

        swf_SetU16(t,ID_BANNER);            // ID

        r.xmin = 0;
        r.ymin = 0;
        r.xmax = swf_TextGetWidth(font,BANNER_TEXT,textscale);
        r.ymax = textsize;
        
        swf_SetRect(t,&r);

        swf_SetMatrix(t,NULL);

        swf_TextCountBits(font,BANNER_TEXT,textscale,&gbits,&abits);
        
        swf_SetU8(t,gbits);
        swf_SetU8(t,abits);

        rgb.r = 0x00;
        rgb.g = 0x00;
        rgb.b = 0x00;

        swf_TextSetInfoRecord(t,font,textsize,&rgb,0,textsize);
        swf_TextSetCharRecord(t,font,BANNER_TEXT,textscale,gbits,abits);

        swf_SetU8(t,0);
        
      
    t = swf_InsertTag(t,ST_PLACEOBJECT2);

        swf_ObjectPlace(t,ID_BANNER,1,NULL,NULL,NULL);
 
    t = swf_InsertTag(t,ST_SHOWFRAME);
  
    t = swf_InsertTag(t,ST_END);
 
//  swf_WriteCGI(&swf);
 

  f = open("text.swf",O_RDWR|O_CREAT|O_TRUNC,0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);
//  swf_FontFree(font);

#ifdef __NT__
  system("start ..\\text.swf");
#endif
  
  return 0;
}


