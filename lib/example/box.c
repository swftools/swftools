
// linux/gcc: cc box.c ../rfxswf.c -funsigned-char -o box -lm; cp box /home/www/cgi-bin/box

#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "../rfxswf.h"

// Box

#define BANNER_TEXT 	"reflex"
#define ID_FONT   	2000
#define ID_BANNER 	2001
#define ID_HIGHLIGHT 	2002
#define ID_BUTTON	2003

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

void ShapeSquare(LPTAG t,LPSHAPE s,int p1,int p2,int p3,int p4,int dx,int dy)
{ 
  // Hidden-Line-Check
    if (((dX[p2]-dX[p1])*(dY[p3]-dY[p1])-(dX[p3]-dX[p1])*(dY[p2]-dY[p1]))<0) return;

  ShapeSetMove(t,s,dX[p1]+dx,dY[p1]+dy);
  ShapeSetLine(t,s,dX[p2]-dX[p1],dY[p2]-dY[p1]);
  ShapeSetLine(t,s,dX[p3]-dX[p2],dY[p3]-dY[p2]);
  ShapeSetLine(t,s,dX[p4]-dX[p3],dY[p4]-dY[p3]);
  ShapeSetLine(t,s,dX[p1]-dX[p4],dY[p1]-dY[p4]);
}


void mapBox(int xw,int yw,int zw)
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
  LPTAG t;
  RGBA rgb;
  SRECT r;
  LPSHAPE s;
  S32 width = 800,height = 800;
  U8 gbits,abits;
  LPSWFFONT font;
  FONTUSAGE use;
  CXFORM cx1,cx2;
  MATRIX m;
  
  int f,i,j,frame;
  
/*  f = open("Arial.efont",O_RDONLY);
  if (f>=0)
  { if (FAILED(FontImport(f,&font)))
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

  FontSetID(font,ID_FONT);
  FontInitUsage(&use);
  FontUse(&use,BANNER_TEXT);
  FontReduce(font,&use);*/
  
  calcTables();
  
  memset(&swf,0x00,sizeof(SWF));

  swf.FileVersion    = 4;
  swf.FrameRate      = 0x4000;
  swf.MovieSize.xmax = 4*width;
  swf.MovieSize.ymax = 4*height;

  swf.FirstTag = InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  t = swf.FirstTag;

        rgb.r = 0xff;
        rgb.g = 0xff;
        rgb.b = 0xff;
        SetRGB(t,&rgb);

  t = InsertTag(t,ST_DEFINEFONT);

        FontSetDefine(t,font);

  t = InsertTag(t,ST_DEFINEFONTINFO);

        FontSetInfo(t,font);

  t = InsertTag(t,ST_DEFINETEXT);

        SetU16(t,ID_BANNER);            // ID
        
        r.xmin = 0;
        r.ymin = 0;
        r.xmax = 400;
        r.ymax = 400;
        SetRect(t,&r);

        SetMatrix(t,NULL);

        TextCountBits(font,BANNER_TEXT,80,&gbits,&abits);
        
        SetU8(t,gbits);
        SetU8(t,abits);

        rgb.r = 0xc0;
        rgb.g = 0xc0;
        rgb.b = 0xc0;

        TextSetInfoRecord(t,font,height/4,&rgb,0,200);
        TextSetCharRecord(t,font,BANNER_TEXT,80,gbits,abits);

        SetU8(t,0);
        
  t = InsertTag(t,ST_DEFINETEXT);

        SetU16(t,ID_HIGHLIGHT);            // ID
        
        r.xmin = 0;
        r.ymin = 0;
        r.xmax = 800;
        r.ymax = 400;
        SetRect(t,&r);

        SetMatrix(t,NULL);

        TextCountBits(font,BANNER_TEXT,80,&gbits,&abits);
        
        SetU8(t,gbits);
        SetU8(t,abits);

        rgb.r = 0x20;
        rgb.g = 0x20;
        rgb.b = 0x20;

        TextSetInfoRecord(t,font,height/4,&rgb,0,200);
        TextSetCharRecord(t,font,BANNER_TEXT,80,gbits,abits);

        SetU8(t,0);
	
    t = InsertTag(t,ST_DEFINEBUTTON);
    
        GetMatrix(NULL,&m);
	
	m.tx = 3*width;
	m.ty = 7*height/2;
        
        SetU16(t,ID_BUTTON); // ID
	ButtonSetRecord(t,BS_UP,ID_BANNER,1,&m,NULL);
	ButtonSetRecord(t,BS_DOWN|BS_HIT|BS_OVER,ID_HIGHLIGHT,1,&m,NULL);
	SetU8(t,0); // End of Button Records
	SetU8(t,0); // End of Action Records
    
      
    t = InsertTag(t,ST_PLACEOBJECT2);

         ObjectPlace(t,ID_BUTTON,1,NULL,NULL,NULL);
	 
    GetCXForm(NULL,&cx1,1);
    GetCXForm(NULL,&cx2,1);
    
//    cx1.a1 = -(0x40*1);
//    cx2.a1 = -(0x40*2);
    
    cx1.r1 = cx1.g1 = 0x80;
    cx2.r1 = cx2.g1 = 0xc0;	
    

	
  for (frame=0;frame<256;frame+=2)
  { int id = frame +1;
    
    t = InsertTag(t,ST_DEFINESHAPE);

        NewShape(&s);
        rgb.r = rgb.g = 0x00; rgb.b = 0xff;
        j = ShapeAddLineStyle(s,40,&rgb);

        SetU16(t,id);  // ID

        r.xmin = 0;
        r.ymin = 0;
        r.xmax = 4*width;
        r.ymax = 4*height;
        
        SetRect(t,&r);

        SetShapeStyles(t,s);
        ShapeCountBits(s,NULL,NULL);
        SetShapeBits(t,s);

        ShapeSetAll(t,s,0,0,j,0,0);
	
	mapBox(frame,frame,frame>>1);

	ShapeSquare(t,s,0,2,3,1,2*width,2*height);
	ShapeSquare(t,s,4,5,7,6,2*width,2*height);
	ShapeSquare(t,s,0,4,6,2,2*width,2*height);
	ShapeSquare(t,s,1,3,7,5,2*width,2*height);
	ShapeSquare(t,s,0,1,5,4,2*width,2*height);
	ShapeSquare(t,s,2,6,7,3,2*width,2*height);
	
        ShapeSetEnd(t);
  }
  
  for (frame=0;frame<256;frame+=2)
  { int id = frame +1;
    int id2 = ((frame-2)&255)+1;
    int id3 = ((frame-4)&255)+1;

    if (frame)
    { t = InsertTag(t,ST_REMOVEOBJECT2); SetU16(t,2); // depth
      t = InsertTag(t,ST_REMOVEOBJECT2); SetU16(t,3); // depth
      t = InsertTag(t,ST_REMOVEOBJECT2); SetU16(t,4); // depth
    }

    t = InsertTag(t,ST_PLACEOBJECT2);

        ObjectPlace(t,id,4,NULL,NULL,NULL);
	
    t = InsertTag(t,ST_PLACEOBJECT2);

        ObjectPlace(t,id2,3,NULL,&cx1,NULL);
  
    t = InsertTag(t,ST_PLACEOBJECT2);

        ObjectPlace(t,id3,2,NULL,&cx2,NULL);
 
    
    t = InsertTag(t,ST_SHOWFRAME);
  }
  
  

  t = InsertTag(t,ST_END);
 
//  WriteCGI(&swf);
  

  f = open("shape1.swf",O_RDWR|O_CREAT|O_TRUNC);
  if FAILED(WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(f);

  FreeTags(&swf);

#ifdef __NT__
  system("start ..\\shape1.swf");
#endif
  
  return 0;
}


