
// linux/gcc: cc texbox.c ../rfxswf.c -funsigned-char -o texbox -lm -ljpeg; cp texbox /home/www/cgi-bin/texbox

#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "../rfxswf.h"

// Box

#define BANNER_TEXT 	"reflex"
#define QUALITY	80
#define ID_FONT   	2000
#define ID_BANNER 	2001
#define ID_HIGHLIGHT 	2002
#define ID_BUTTON	2003
#define ID_BITMAP	2004
#define ID_SHAPE	2005
#define ID_SHAPE2	2006

#define a 200
int sX[] = { a,-a, a,-a, a,-a, a,-a};
int sY[] = { a, a,-a,-a, a, a,-a,-a};
int sZ[] = { a, a, a, a,-a,-a,-a,-a};
#undef a

#define PRECISION 16

int SHADE = 1;

int dX[8];
int dY[8];
int square_visible[6]  = {0,0,0,0,0,0};
int square_depth[6][2] = {{2,3},{4,5},{6,7},{8,9},{10,11},{12,13}};
int square_light[6]    = {0,0,64,64,-64,-64};

int sin_[512],cos_[512];

void calcTables()	
{ int i;
  double d;
  for (i=0;i<512;i++)
  { d = ((double)i)/128*3.14159;
    sin_[i] = (int)(sin(d)*(1<<PRECISION));
    cos_[i] = (int)(cos(d)*(1<<PRECISION));
  }
}

LPTAG MapSquare(LPTAG t,int dx0,int dy0,int p1,int p2,int p3,int p4,int dx,int dy,int square)
{ MATRIX m;
  CXFORM cx;

  if (SHADE)
  { GetCXForm(NULL,&cx,1);
    cx.r1 = cx.b1 = cx.g1 = square_light[square];
  }
        
  if (((dX[p2]-dX[p1])*(dY[p3]-dY[p1])-(dX[p3]-dX[p1])*(dY[p2]-dY[p1]))<0) 
  { if (square_visible[square])
    { t = InsertTag(t,ST_REMOVEOBJECT2); SetU16(t,square_depth[square][0]);
      t = InsertTag(t,ST_REMOVEOBJECT2); SetU16(t,square_depth[square][1]);
    }
    square_visible[square] = 0;
    return t; 
  }

  t = InsertTag(t,ST_PLACEOBJECT2);
  
  MatrixMapTriangle(&m,dx0,dy0,dX[p1]+dx,dY[p1]+dy,
	                dX[p2]+dx,dY[p2]+dy,
		 dX[p4]+dx,dY[p4]+dy);

  if (!square_visible[square])
    ObjectPlace(t,ID_SHAPE,square_depth[square][0],&m,SHADE?&cx:NULL,NULL);
  else
    ObjectMove(t,square_depth[square][0],&m,SHADE?&cx:NULL);

  t = InsertTag(t,ST_PLACEOBJECT2);
  
  MatrixMapTriangle(&m,dx0,dy0,dX[p3]+dx,dY[p3]+dy,
	                dX[p4]+dx,dY[p4]+dy,
		 dX[p2]+dx,dY[p2]+dy);

  if (!square_visible[square])
    ObjectPlace(t,ID_SHAPE2,square_depth[square][1],&m,SHADE?&cx:NULL,NULL);
  else
    ObjectMove(t,square_depth[square][1],&m,SHADE?&cx:NULL);

  square_visible[square] = 1;
 
  return t;
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
		  
    y1 = ( y*cos_[xw]- z*sin_[xw])>>PRECISION;
    z1 = ( y*sin_[xw]+ z*cos_[xw])>>PRECISION;
    x1 = x;

    x2 = (x1*cos_[yw]+z1*sin_[yw])>>PRECISION;
    y2 = y1;

    x3 = (x2*cos_[zw]-y2*sin_[zw])>>PRECISION;
    y3 = (x2*sin_[zw]+y2*cos_[zw])>>PRECISION;
    z3 = (z1*cos_[yw]-x1*sin_[yw])>>PRECISION;

    dX[i] = x3*4000/(z3+950);
    dY[i] = y3*4000/(z3+950);
	
  }
}

void mapLights(int xw,int yw,int zw)
{ int i;
  int x1,y1,z1,x2,y2,z2,x3,y3,z3;

  int y[] = {0,0,0,0,256,-256};
  int x[] = {0,0,256,-256,0,0};
  int z[] = {256,-256,0,0,0,0};

  int lz = 256;	// lightvector
  int lx = 200, ly = 200;

  for (i=0;i<6;i++)
  { y1 = (y[i]*cos_[xw]-z[i]*sin_[xw])>>PRECISION;
    z1 = (y[i]*sin_[xw]+z[i]*cos_[xw])>>PRECISION;
    x1 = x[i];

    x2 = (x1*cos_[yw]+z1*sin_[yw])>>PRECISION;
    y2 = y1;
    z2 = (z1*cos_[yw]-x1*sin_[yw])>>PRECISION;

    x3 = (x2*cos_[zw]-y2*sin_[zw])>>PRECISION;
    y3 = (x2*sin_[zw]+y2*cos_[zw])>>PRECISION;
    z3 = z2;

    square_light[i] = -128-((x3*lx + y3*ly + z3*lz)>>9);
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
  int fs,ls; // line & fillstyle
  LPSWFFONT font;
  FONTUSAGE use;
  LPJPEGBITS jpeg;
  MATRIX m;  
  
  int f,i,j,frame;
  
  f = open("Arial.efont",O_RDONLY);
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
  FontReduce(font,&use);
  
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
        
        r.xmin = 0; r.ymin = 0; r.xmax = 800; r.ymax = 400;
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

    t = InsertTag(t,ST_DEFINEBITSJPEG2);

         SetU16(t,ID_BITMAP);
         SetJPEGBits(t,"eye.jpg",QUALITY);
         
    t = InsertTag(t,ST_DEFINESHAPE);
    
         NewShape(&s);
         rgb.b = rgb.g = rgb.r = 0x00;
         ls = ShapeAddLineStyle(s,10,&rgb);	
         rgb.b = 0xff;

         m.tx = m.ty = 0;
         m.r0 = m.r1 = 0;
         m.sx = m.sy = width<<8;
  
         fs = ShapeAddBitmapFillStyle(s,&m,ID_BITMAP,0);
    
         SetU16(t,ID_SHAPE);	// ID	
         
         r.xmin = 0; r.ymin = 0;
         r.xmax = 2*width; r.ymax = 2*height;
         SetRect(t,&r);

         SetShapeHeader(t,s);
         ShapeSetAll(t,s,width,0,0,fs,0);
         ShapeSetLine(t,s,-width,height);
         ShapeSetStyle(t,s,ls,fs,0);
         ShapeSetLine(t,s,0,-height);
         ShapeSetLine(t,s,width,0);
         ShapeSetEnd(t);

         ShapeFree(s);
         
    t = InsertTag(t,ST_DEFINESHAPE);
    
         NewShape(&s);
         rgb.b = rgb.g = rgb.r = 0x00;
         ls = ShapeAddLineStyle(s,10,&rgb);	
         rgb.b = 0xff;

         m.tx = m.ty = 0;
         m.r0 = m.r1 = 0;
         m.sx = m.sy = -(width<<8);
  
         fs = ShapeAddBitmapFillStyle(s,&m,ID_BITMAP,0);
    
         SetU16(t,ID_SHAPE2);	// ID	
         
         r.xmin = 0; r.ymin = 0;
         r.xmax = 2*width; r.ymax = 2*height;
         SetRect(t,&r);

         SetShapeHeader(t,s);
         ShapeSetAll(t,s,width,0,0,fs,0);
         ShapeSetLine(t,s,-width,height);
         ShapeSetStyle(t,s,ls,fs,0);
         ShapeSetLine(t,s,0,-height);
         ShapeSetLine(t,s,width,0);
         ShapeSetEnd(t);

         ShapeFree(s);
   
  for (frame=0;frame<256;frame++)
  { int dc = 3; // whitespace correction

    mapBox(frame<<1,frame<<1,frame);
    if (SHADE) mapLights(frame<<1,frame<<1,frame);
    
    t = MapSquare(t,width-dc,height-dc,0,2,3,1,2*width,2*height,0);
    t = MapSquare(t,width-dc,height-dc,4,5,7,6,2*width,2*height,1);
    t = MapSquare(t,width-dc,height-dc,0,4,6,2,2*width,2*height,2);
    t = MapSquare(t,width-dc,height-dc,1,3,7,5,2*width,2*height,3);
    t = MapSquare(t,width-dc,height-dc,0,1,5,4,2*width,2*height,4);
    t = MapSquare(t,width-dc,height-dc,2,6,7,3,2*width,2*height,5);
        
    t = InsertTag(t,ST_SHOWFRAME);
  }
  
  
  t = InsertTag(t,ST_END);
  
  WriteCGI(&swf);
  FreeTags(&swf);
  return 0;
}


