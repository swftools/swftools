#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "../rfxswf.h"

               
int main (int argc,char ** argv)
{ SWF swf;
  LPTAG t;
  RGBA rgb;
  SRECT r;
  LPSHAPE s;
  S32 width=300,height = 300;
  
  int f,i,j;

  memset(&swf,0x00,sizeof(SWF));

  swf.FileVersion    = 4;
  swf.FrameRate      = 0x1900;
  swf.MovieSize.xmax = 20*width;
  swf.MovieSize.ymax = 20*height;

  swf.FirstTag = InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.FirstTag;

        rgb.r = 0xff;
        rgb.g = 0xff;
        rgb.b = 0xff;
        SetRGB(t,&rgb);

  t = InsertTag(t,ST_DEFINESHAPE);

        NewShape(&s);
        rgb.b = rgb.g = 0x00;
        j = ShapeAddLineStyle(s,40,&rgb);
        rgb.r = 0; rgb.b = 0xff;
        ShapeAddLineStyle(s,40,&rgb);

        SetU16(t,1);  // ID

        r.xmin = 0;
        r.ymin = 0;
        r.xmax = 4*width;
        r.ymax = 4*height;
        
        SetRect(t,&r);

        SetShapeStyles(t,s);
        ShapeCountBits(s,NULL,NULL);
        SetShapeBits(t,s);

        ShapeSetAll(t,s,0,0,j,0,0);
//        ShapeSetCurve(t,s,4*width,0,0,4*height);
        ShapeSetLine(t,s,4*width,4*height);
        ShapeSetStyle(t,s,2,0,0);
        for (i=1;i<10;i++)
        ShapeSetCircle(t,s,4*width,4*height,i*width/2,i*height/2);
        ShapeSetEnd(t);

  t = InsertTag(t,ST_PLACEOBJECT2);

        ObjectPlace(t,1,1,NULL,NULL,NULL);

  t = InsertTag(t,ST_SHOWFRAME);

  t = InsertTag(t,ST_END);
  
//  WriteCGI(&swf);
  

  f = open("shape1.swf",O_WRONLY|O_CREAT, 0777);
//  f = 1;
  if FAILED(WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  FreeTags(&swf);

#ifdef __NT__
  system("start ..\\shape1.swf");
#endif
  
  return 0;
}


