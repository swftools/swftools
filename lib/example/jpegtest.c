// linux/gcc cc jpegtest.c ../rfxswf.c -funsigned-char -o jpegtest -lm -ljpeg; cp jpegtest /home/www/cgi-bin/jpegtest

#include <stdio.h>
#include <math.h>	
#include "../rfxswf.h"

#define WIDTH 		256
#define HEIGHT 	256
#define QUALITY 	85

#define ID_BITS 	1
#define ID_SHAPE 	2

int main ( int argc, char ** argv)
{ SWF swf;
  LPTAG t;
  RGBA rgb;
  LPSHAPE s;
  MATRIX m;
  SRECT r;
  LPJPEGBITS jpeg;
  
  int ls; // line style
  int fs; // fill style
  int frame;

  memset(&swf,0x00,sizeof(SWF));

  swf.FileVersion 	= 4;
  swf.FrameRate		= 0x1800;
  swf.MovieSize.xmax	= 20*WIDTH;
  swf.MovieSize.ymax	= 20*HEIGHT;

  swf.FirstTag = InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.FirstTag;

    rgb.r = 0xff;
    rgb.b = 0xff;
    rgb.g = 0xff;
    SetRGB(t,&rgb);

  t = InsertTag(t,ST_DEFINEBITSJPEG2);

    SetU16(t,ID_BITS);
    SetJPEGBits(t,"eye.jpg",QUALITY);
    
/*    jpeg = SetJPEGBitsStart(t,WIDTH,HEIGHT,QUALITY);
    { int y;
      for (y=0;y<HEIGHT;y++)
      { U8 scanline[3*WIDTH];
        int x,p = 0;
        for (x=0;x<WIDTH;x++) 
        { scanline[p++] = x;    // R
          scanline[p++] = y;    // G
          scanline[p++] = 0x80; // B          
        }
        SetJPEGBitsLine(jpeg,scanline);
      }
    }
    SetJPEGBitsFinish(jpeg);
*/

for (frame=0;frame<64;frame++)
{
  t = InsertTag(t,ST_DEFINESHAPE);
    
    NewShape(&s);
    rgb.b = rgb.g = rgb.r = 0x00;
    ls = ShapeAddLineStyle(s,40,&rgb);	
    rgb.b = 0xff;
  //  fs = ShapeAddSolidFillStyle(s,&rgb);
    //  
    GetMatrix(NULL,&m);
    m.sy = m.sx = (int)(cos(((float)(frame))/32*3.141)*0x80000);
    m.r0 = (int)(sin(((float)(frame))/32*3.141)*0x80000);
    m.r1 = -m.r0;
       
    fs = ShapeAddBitmapFillStyle(s,&m,ID_BITS,0);
    
    SetU16(t,ID_SHAPE+frame);	// ID	

    r.xmin = 0;
    r.ymin = 0;
    r.xmax = 10*WIDTH;
    r.ymax = 10*HEIGHT;

    SetRect(t,&r);

    SetShapeStyles(t,s);
    ShapeCountBits(s,NULL,NULL);
    SetShapeBits(t,s);

    ShapeSetAll(t,s,0,0,ls,fs,0);

    ShapeSetLine(t,s,10*WIDTH,0);
    ShapeSetLine(t,s,-10*WIDTH,10*HEIGHT);
//    ShapeSetLine(t,s,-10*WIDTH,-10*WIDTH);
    ShapeSetLine(t,s,0,-10*HEIGHT);
    ShapeSetEnd(t);

  if (frame) 
  { t = InsertTag(t,ST_REMOVEOBJECT2); SetU16(t,1);
    t = InsertTag(t,ST_REMOVEOBJECT2); SetU16(t,2);
  }
	 
  t = InsertTag(t,ST_PLACEOBJECT2);
    ObjectPlace(t,ID_SHAPE+frame,1,NULL,NULL,NULL); 

  t = InsertTag(t,ST_PLACEOBJECT2);
    GetMatrix(NULL,&m);
    m.tx = m.ty = 10*WIDTH+frame*10;
    m.sx = m.sy = 0xfffeffff;
    ObjectPlace(t,ID_SHAPE+frame,2,&m,NULL,NULL);


  t = InsertTag(t,ST_SHOWFRAME);
}

  t = InsertTag(t,ST_END);

  WriteCGI(&swf);
  FreeTags(&swf);
  return 0;
}
