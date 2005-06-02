/* box.c 

   Example for drawing 3D grind objects
   
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

#define BANNER_TEXT     "reflex" // banner is disabled due to new font handling
#define ID_FONT         2000
#define ID_BANNER       2001
#define ID_HIGHLIGHT    2002
#define ID_BUTTON       2003

#define a 200
int sX[] = { a,-a, a,-a, a,-a, a,-a};
int sY[] = { a, a,-a,-a, a, a,-a,-a};
int sZ[] = { a, a, a, a,-a,-a,-a,-a};
#undef a
  
int dX[8];
int dY[8];

int sin_[512],cos_[512];

void calcTables()
{ int i;
  double d;
  for (i=0;i<512;i++)
  { d = ((double)i)/128*3.14159;
    sin_[i] = (int)(sin(d)*256);
    cos_[i] = (int)(cos(d)*256);
  }
}

void ShapeSquare(TAG* t,SHAPE* s,int p1,int p2,int p3,int p4,int dx,int dy)
{ 
  // Hidden-Line-Check
    if (((dX[p2]-dX[p1])*(dY[p3]-dY[p1])-(dX[p3]-dX[p1])*(dY[p2]-dY[p1]))<0) return;

  swf_ShapeSetMove(t,s,dX[p1]+dx,dY[p1]+dy);
  swf_ShapeSetLine(t,s,dX[p2]-dX[p1],dY[p2]-dY[p1]);
  swf_ShapeSetLine(t,s,dX[p3]-dX[p2],dY[p3]-dY[p2]);
  swf_ShapeSetLine(t,s,dX[p4]-dX[p3],dY[p4]-dY[p3]);
  swf_ShapeSetLine(t,s,dX[p1]-dX[p4],dY[p1]-dY[p4]);
}


void mapBox(int xw,int yw,int zw)
// simple trigonometry without using transformation matrices
{ int i;
  int x1,y1,z1,x2,y2,z2,x3,y3,z3;
  int y,x,z;
  
  xw &= 255;
  yw &= 255;
  zw &= 255;

  for (i=0;i<8;i++)
  { x = sX[i];
    y = sY[i];
    z = sZ[i];
                  
    y1 = ( y*cos_[xw]- z*sin_[xw])>>8;
    z1 = ( y*sin_[xw]+ z*cos_[xw])>>8;
    x1 = x;

    x2 = (x1*cos_[yw]+z1*sin_[yw])>>8;
    y2 = y1;

    x3 = (x2*cos_[zw]-y2*sin_[zw])>>8;
    y3 = (x2*sin_[zw]+y2*cos_[zw])>>8;
    z3 = (z1*cos_[yw]-x1*sin_[yw])>>8;

    dX[i] = x3*4000/(z3+950);
    dY[i] = y3*4000/(z3+950);
        
  }
}
               
int main (int argc,char ** argv)
{ SWF swf;
  TAG* t;
  RGBA rgb;
  SRECT r;
  SHAPE* s;
  S32 width = 800,height = 800;
  U8 gbits,abits;
  CXFORM cx1,cx2;
  MATRIX m;
  
  int f,i,j,frame;
  
/*  f = open("Arial.efont",O_RDONLY);
  if (f>=0)
  { if (FAILED(swf_FontImport(f,&font)))
    { fprintf(stderr,"Font import failed\n");
      close(f);
      return -1;
    }
  }
  else
  { fprintf(stderr,"Font not found\n");
    return -1;
  }
  close(f);

  swf_FontSetID(font,ID_FONT);
  swf_FontInitUsage(&use);
  swf_FontUse(&use,BANNER_TEXT);
  swf_FontReduce(font,&use);
*/
  
  calcTables();
  
  memset(&swf,0x00,sizeof(SWF));

  swf.fileVersion    = 4;
  swf.frameRate      = 0x4000;
  swf.movieSize.xmax = 4*width;
  swf.movieSize.ymax = 4*height;

  swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.firstTag;

        rgb.r = 0xff;
        rgb.g = 0xff;
        rgb.b = 0xff;
        swf_SetRGB(t,&rgb);
        
/* This part can't be used because of missing fonts. Nevertheless it shows, how
   rfxswf handles fonts, fontusages (in order to include only the used glyphs
   into the output swf) and how to make a simple button


  t = swf_InsertTag(t,ST_DEFINEFONT);

        swf_FontSetDefine(t,font);

  t = swf_InsertTag(t,ST_DEFINEFONTINFO);

        swf_FontSetInfo(t,font);

  t = swf_InsertTag(t,ST_DEFINETEXT);

        swf_SetU16(t,ID_BANNER);            // ID
        
        r.xmin = 0;
        r.ymin = 0;
        r.xmax = 400;
        r.ymax = 400;
        
        swf_SetRect(t,&r);

        swf_SetMatrix(t,NULL);

        swf_TextCountBits(font,BANNER_TEXT,80,&gbits,&abits);
        
        swf_SetU8(t,gbits);
        swf_SetU8(t,abits);

        rgb.r = 0xc0;
        rgb.g = 0xc0;
        rgb.b = 0xc0;

        swf_TextSetInfoRecord(t,font,height/4,&rgb,0,200);
        swf_TextSetCharRecord(t,font,BANNER_TEXT,80,gbits,abits);

        swf_SetU8(t,0);
        
  t = swf_InsertTag(t,ST_DEFINETEXT);

        swf_SetU16(t,ID_HIGHLIGHT);            // ID
        
        r.xmin = 0;
        r.ymin = 0;
        r.xmax = 800;
        r.ymax = 400;
        
        swf_SetRect(t,&r);

        swf_SetMatrix(t,NULL);

        swf_TextCountBits(font,BANNER_TEXT,80,&gbits,&abits);
        
        swf_SetU8(t,gbits);
        swf_SetU8(t,abits);

        rgb.r = 0x20;
        rgb.g = 0x20;
        rgb.b = 0x20;

        swf_TextSetInfoRecord(t,font,height/4,&rgb,0,200);
        swf_TextSetCharRecord(t,font,BANNER_TEXT,80,gbits,abits);

        swf_SetU8(t,0);
        
    t = swf_InsertTag(t,ST_DEFINEBUTTON);
    
        swf_GetMatrix(NULL,&m);
        
        m.tx = 3*width;
        m.ty = 7*height/2;
        
        swf_SetU16(t,ID_BUTTON); // ID
        swf_ButtonSetRecord(t,BS_UP,ID_BANNER,1,&m,NULL);
        swf_ButtonSetRecord(t,BS_DOWN|BS_HIT|BS_OVER,ID_HIGHLIGHT,1,&m,NULL);
        swf_SetU8(t,0); // End of Button Records
        swf_SetU8(t,0); // End of Action Records
    
      
    t = swf_InsertTag(t,ST_PLACEOBJECT2);

         swf_ObjectPlace(t,ID_BUTTON,1,NULL,NULL,NULL);
         
*/
        
  for (frame=0;frame<256;frame+=2)
  { int id = frame +1;
    
    t = swf_InsertTag(t,ST_DEFINESHAPE);

        swf_ShapeNew(&s);
        rgb.r = rgb.g = 0x00; rgb.b = 0xff;
        j = swf_ShapeAddLineStyle(s,40,&rgb);

        swf_SetU16(t,id);  // ID

        r.xmin = 0;
        r.ymin = 0;
        r.xmax = 4*width;
        r.ymax = 4*height;
        
        swf_SetRect(t,&r);
        swf_SetShapeHeader(t,s);
        swf_ShapeSetAll(t,s,0,0,j,0,0);
        
        mapBox(frame,frame,frame>>1);

        ShapeSquare(t,s,0,2,3,1,2*width,2*height);
        ShapeSquare(t,s,4,5,7,6,2*width,2*height);
        ShapeSquare(t,s,0,4,6,2,2*width,2*height);
        ShapeSquare(t,s,1,3,7,5,2*width,2*height);
        ShapeSquare(t,s,0,1,5,4,2*width,2*height);
        ShapeSquare(t,s,2,6,7,3,2*width,2*height);
        
        swf_ShapeSetEnd(t);
        swf_ShapeFree(s);
  }
  
  swf_GetCXForm(NULL,&cx1,1);  // get default ColorTransforms
  swf_GetCXForm(NULL,&cx2,1);
    
  cx1.r1 = cx1.g1 = 0x80;  // set alpha for blur effect
  cx2.r1 = cx2.g1 = 0xc0;     
    
  for (frame=0;frame<256;frame+=2)
  { int id = frame +1;
    int id2 = ((frame-2)&255)+1;
    int id3 = ((frame-4)&255)+1;

    if (frame)
    { t = swf_InsertTag(t,ST_REMOVEOBJECT2); swf_SetU16(t,2); // depth
      t = swf_InsertTag(t,ST_REMOVEOBJECT2); swf_SetU16(t,3); // depth
      t = swf_InsertTag(t,ST_REMOVEOBJECT2); swf_SetU16(t,4); // depth
    }

    t = swf_InsertTag(t,ST_PLACEOBJECT2);

        swf_ObjectPlace(t,id,4,NULL,NULL,NULL);
        
    t = swf_InsertTag(t,ST_PLACEOBJECT2);

        swf_ObjectPlace(t,id2,3,NULL,&cx1,NULL);
  
    t = swf_InsertTag(t,ST_PLACEOBJECT2);

        swf_ObjectPlace(t,id3,2,NULL,&cx2,NULL);
  
    t = swf_InsertTag(t,ST_SHOWFRAME);
  }
  
  t = swf_InsertTag(t,ST_END);
 
//  swf_WriteCGI(&swf);
 

  f = open("box.swf",O_RDWR|O_CREAT|O_TRUNC|O_BINARY,0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  swf_FreeTags(&swf);

#ifdef __NT__
  system("start ..\\box.swf");
#endif
  
  return 0;
}


