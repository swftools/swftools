
// linux/gcc: cc transtest.c ../rfxswf.c -funsigned-char -o transtest -lm -ljpeg; cp transtest /home/www/cgi-bin/transtest

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


#define S64 long long
SFIXED SP(SFIXED a1,SFIXED a2,SFIXED b1,SFIXED b2)
{ S64 a;
  a = (S64)a1*(S64)b1+(S64)a2*(S64)b2;
  return (SFIXED)(a>>16);
}
SFIXED QFIX(int zaehler,int nenner) // bildet Quotient von zwei INTs in SFIXED
{ S64 z = zaehler<<16;
  S64 a = z/(S64)nenner;
  return (SFIXED)a;
}
#undef S64

LPMATRIX MatrixJoin(LPMATRIX d,LPMATRIX s1,LPMATRIX s2)
{        
  if (!d) return NULL;
  if (!s1) return (s2)?(LPMATRIX)memcpy(d,s2,sizeof(MATRIX)):NULL;
  if (!s2) return (LPMATRIX)memcpy(d,s1,sizeof(MATRIX));
  
  d->tx = s1->tx + s2->tx;
  d->ty = s1->ty + s2->ty;
  
  d->sx = SP(s1->sx,s1->r1,s2->sx,s2->r0);
  d->sy = SP(s1->r0,s1->sy,s2->r1,s2->sy);
  d->r0 = SP(s1->r0,s1->sy,s2->sx,s2->r0);
  d->r1 = SP(s1->sx,s1->r1,s2->r1,s2->sy);

  //DumpMatrix(NULL,d);
  
  return d;
}

LPMATRIX MatrixMapTriangle(LPMATRIX m,int dx,int dy,int x0,int y0,
	                       int x1,int y1,int x2,int y2)
{ int dx1 = x1 - x0;
  int dy1 = y1 - y0;
  int dx2 = x2 - x0;
  int dy2 = y2 - y0;
  
  if (!m) return NULL;
  if ((!dx)||(!dy)) return NULL; // check DIV by zero

  m->tx = x0;
  m->ty = y0;
  m->sx = QFIX(dx1,dx);
  m->sy = QFIX(dy2,dy);
  m->r0 = QFIX(dy1,dx);
  m->r1 = QFIX(dx2,dy);
  
  return m;
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

    t = InsertTag(t,ST_DEFINEBITSJPEG2);

         SetU16(t,ID_BITMAP);
         SetJPEGBits(t,"eye.jpg",QUALITY);
         
    t = InsertTag(t,ST_DEFINESHAPE);
    
         NewShape(&s);
         rgb.b = rgb.g = rgb.r = 0x00;
        // ls = ShapeAddLineStyle(s,40,&rgb);	
         ls = 0;
         rgb.b = 0xff;
  
         fs = ShapeAddBitmapFillStyle(s,&m,ID_BITMAP,0);
    
         SetU16(t,ID_SHAPE);	// ID	
         
         r.xmin = 0;
         r.ymin = 0;
         r.xmax = 2*width;
         r.ymax = 2*height;
         
         SetRect(t,&r);

         SetShapeHeader(t,s);
         
         ShapeSetAll(t,s,0,0,ls,fs,0);
         ShapeSetLine(t,s,width,0);
         ShapeSetLine(t,s,-width,height);
         ShapeSetLine(t,s,0,-height);
         ShapeSetEnd(t);

         ShapeFree(s);
    

  for (frame=0;frame<64;frame++)
  { /*MATRIX m1,m2;
     
    m1.sy = m1.sx = (int)(cos(((float)(frame))/32*3.141)*0x10000);
    m1.r0 = (int)(sin(((float)(frame))/32*3.141)*0x10000);
    m1.r1 = -m1.r0;
    m1.tx = width+frame*4; m1.ty = height;

    m2.sy = m2.sx = (int)(cos(((float)(64-frame))/32*3.141)*0x10000);
    m2.r0 = (int)(sin(((float)(64-frame))/32*3.141)*0x10000);
    m2.r1 = -m2.r0;
    m2.tx = width; m2.ty = height;

    MatrixJoin(&m,&m1,&m2); */

    int dx0 = width;  // Konstanten der Shapes
    int dy0 = width;

    int px0 = 2*width;  // Zielpunkte des Mappings
    int py0 = 2*width;

    int px1 = 3*width;
    int py1 = 2*width-frame*4;

    int px2 = 2*width-frame*8;
    int py2 = 3*width;

    MatrixMapTriangle(&m,dx0,dy0,px0,py0,px1,py1,px2,py2);

    t = InsertTag(t,ST_PLACEOBJECT2);

        if (!frame)
         ObjectPlace(t,ID_SHAPE,1,&m,NULL,NULL);
        else
          ObjectMove(t,1,&m,NULL);
 
    t = InsertTag(t,ST_SHOWFRAME);
  }
  
  
  t = InsertTag(t,ST_END);
  
  WriteCGI(&swf);
  FreeTags(&swf);
  return 0;
}


