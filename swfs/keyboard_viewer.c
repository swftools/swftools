/* keyboard_viewer.c

   Creates the swf file keyboard_viewer.swf. 
   This can be used to navigate through frames using the keyboard (space,
   cursor left, cursor right)
   Notice that the movie has to clicked with the mouse once to get
   navigation activated in most browsers.
   
   Part of the swftools package.

   Copyright (c) 2000, 2001 Matthias Kramm <kramm@quiss.org>
 
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

#define SUBTITLES 1

int main (int argc,char ** argv)
{ SWF swf;
  RGBA rgb;
  SRECT r;
  SHAPE* s;
  MATRIX m;
  ActionTAG*a1,*a2,*a3,*a4,*a5;
  S32 width=1024,height = 768;
  
  int f,i,ls1,fs1;

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
 
  // initialize matrix
  m.sx = 65536; //scale
  m.sy = 65536;
  m.r0 = 0; //rotate
  m.r1 = 0;
  m.tx = 0; //move
  m.ty = 0;

  /* the "viewport" object will be replaced by swfcombine
     with the object to browse. It is placed at the
     upper left corner (0,0), by setting m.tx and m.ty
     to 0. Therefore, the buttons are "in" the viewport,
     not above it*/
  tag = swf_InsertTag(tag,ST_DEFINESPRITE);
  swf_SetU16(tag, 23); //id
  swf_SetU16(tag, 0); //frames
  tag = swf_InsertTag(tag,ST_END);
  tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
  m.tx = 0; //move
  m.ty = 0;
  swf_ObjectPlace(tag, 23, 1,&m,0,"viewport");

    a1 = action_SetTarget(0, "viewport");
    a1 = action_PreviousFrame(a1);
    a1 = action_SetTarget(a1, "");
    a1 = action_End(a1);

    a2 = action_SetTarget(0, "viewport");
    a2 = action_NextFrame(a2);
    a2 = action_SetTarget(a2,"");
    a2 = action_End(a2);

    a3 = action_Stop(0);
    a3 = action_SetTarget(a3,"viewport");
    a3 = action_Stop(a3);
    a3 = action_SetTarget(a3,"");
#ifdef SUBTITLES
    a3 = action_PushString(a3,"/:subtitle");
    a3 = action_PushString(a3,""); //reset variable
    a3 = action_SetVariable(a3);
#endif
    a3 = action_End(a3);

    a4 = action_GetUrl(0, "lichtfarbe.html", "_this");
    a4 = action_End(a4);
    
    a5 = action_GetUrl(0, "phybas.html","_this"); // load html in this window
    a5 = action_End(a5);

  tag = swf_InsertTag(tag,ST_DEFINESHAPE);
    swf_ShapeNew(&s);               // create new shape instance
    rgb.r = rgb.b = rgb.g = 0x00;           
    ls1 = swf_ShapeAddLineStyle(s,40,&rgb);
    fs1 = swf_ShapeAddSolidFillStyle(s,&rgb);
    swf_SetU16(tag,77);
    r.xmin = 0;
    r.ymin = 0;
    r.xmax = 20*width;
    r.ymax = 20*height;
    swf_SetRect(tag,&r);              // set shape bounds
    swf_SetShapeHeader(tag,s);        // write all styles to tag
	
    swf_ShapeSetAll(tag,s,0,0,ls1,fs1,0); // move to (0,0), select linestyle ls1 and no fillstyle
    /* SetLine coordinates are relative.
       It's important that the start and end points match, otherwise
       the Macromedia Flash player will crash. */
    swf_ShapeSetLine(tag,s,20*width,0*height);
    swf_ShapeSetLine(tag,s,0*width,20*height);
    swf_ShapeSetLine(tag,s,-20*width,0*height);
    swf_ShapeSetLine(tag,s,0*width,-20*height);

    swf_ShapeSetEnd(tag);   // finish drawing
    swf_ShapeFree(s);   // clean shape structure (which isn't needed anymore after writing the tag)

  tag = swf_InsertTag(tag,ST_DEFINEBUTTON2);
  swf_SetU16(tag,30); //id
  swf_ButtonSetFlags(tag, 0); //menu=no
  swf_ButtonSetRecord(tag,BS_HIT,77,1,NULL,NULL);
  swf_SetU8(tag,0); // end of button records
  
  swf_ButtonSetCondition(tag, BC_CURSORLEFT);
    swf_ActionSet(tag,a1);
  swf_ButtonSetCondition(tag, BC_CURSORRIGHT);
    swf_ActionSet(tag,a2);
  swf_ButtonSetCondition(tag, BC_SPACE);
    swf_ActionSet(tag,a2);
  swf_ButtonSetCondition(tag, BC_ESCAPE);
    swf_ActionSet(tag,a4);
  swf_ButtonSetCondition(tag, BC_ENTER);
    swf_ActionSet(tag,a5);
   
  swf_ButtonPostProcess(tag, 5); // don't forget!
  

  tag = swf_InsertTag(tag,ST_DOACTION);
  swf_ActionSet(tag,a3);

  m.tx = 0; //move
  m.ty = 0;
  tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
  swf_ObjectPlace(tag, 30, 2,&m,0,0);
  
  swf_ActionFree(a1);
  swf_ActionFree(a2);
  swf_ActionFree(a3);
  swf_ActionFree(a4);
  swf_ActionFree(a5);
  


#ifdef SUBTITLES
  tag = swf_InsertTag(tag,ST_DEFINEFONT2); {
      U8 data[] = {0x90, 0x00, 0x0f, 0x54, 0x69, 0x6d, 0x65, 0x73, 
		   0x20, 0x4e, 0x65, 0x77, 0x20, 0x52, 0x6f, 0x6d, 
		   0x61, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		   0x00, 0x00, 0x00, 0x00};
      swf_SetU16(tag, 0x76); //id
      swf_SetBlock(tag, data, sizeof(data));
  }
  tag = swf_InsertTag(tag,ST_DEFINEEDITTEXT); {
      EditTextLayout layout;
      layout.align = 0;
      layout.leftmargin = 0;
      layout.rightmargin = 0;
      layout.indent = 0;
      layout.leading = 0;

      swf_SetU16(tag, 0x77);//id
      r.xmin = 20*60;
      r.xmax = r.xmin+ 826*20;
      r.ymin = 0;
      r.ymax = 20*80;
      rgb.r = rgb.g = rgb.b = 0;
      rgb.a = 255;
      swf_SetEditText(tag, ET_MULTILINE|ET_READONLY, r, 0, &rgb, 0, 0x76, 20*40, &layout, "/:subtitle");
      m.tx = m.ty = 0;
  }
  tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
  swf_ObjectPlace(tag, 0x77, 4,&m,0,0);
#endif

  tag = swf_InsertTag(tag,ST_SHOWFRAME);
  tag = swf_InsertTag(tag,ST_END);
  
  f = open("keyboard_viewer.swf",O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);                       // cleanup
  return 0;
}


