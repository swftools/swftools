/* This file is only for testing purposes. it replaces swfoutput.cc, and dumps 
   the information it gets into an X11 Window, not into an swf.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include <string.h>
#include "swfoutput.h"
#include "kramm/xwindows.h"
#include "spline.h"
#include "log.h"

modexwindow*vga;

void inline pp(int x,int y)
{
    if(x<0) return;
    if(y<0) return;
    vga->putpixel(x,y);
}

void circle(int mx,int my,int r)
{
   int d=3-(2*r);
   int x=0;
   int y=r;


   while(x<=y)
   {
    pp(mx+x,my+y);
    pp(mx+x,my-y);
    pp(mx-x,my+y);
    pp(mx-x,my-y);
    pp(mx+y,my+x);
    pp(mx+y,my-x);
    pp(mx-y,my+x);
    pp(mx-y,my-x);

    if(d<0)
    {
     d=d+(4*x)+6;
    }
    else
    {y--;
     d=d+4*(x-y)+10;
    };
    x++;
   }
}

void qspline(plotxy p0,plotxy p1,plotxy p2)
{
    double d;
    //vga->setcolor(0x0000ff);
    for(d=0.00;d<=1.00;d+=0.001) {
	int x = (int)(p0.x * d*d + p1.x * 2*(1-d)*d + p2.x * (1-d)*(1-d));
	int y = (int)(p0.y * d*d + p1.y * 2*(1-d)*d + p2.y * (1-d)*(1-d));
	pp(x,y);
    }
}
void transform (plotxy*p0,struct swfmatrix*m)
{
    double x,y;
    x = m->m11*p0->x+m->m12*p0->y;
    y = m->m21*p0->x+m->m22*p0->y;
    p0->x = x + m->m13;
    p0->y = y + m->m23;
//    p0->x *= 3;
//    p0->y *= 3;
}

void spline(plotxy p0,plotxy p1,plotxy p2,plotxy p3,struct swfmatrix*m)
{
    double d;
    struct qspline q[16];
    int num;
    int t;
    transform(&p0,m);
    transform(&p1,m);
    transform(&p2,m);
    transform(&p3,m);
    //vga->setcolor(0xffff00);
    //vga->drawline(p3.x,p3.y,p2.x,p2.y);
    //vga->drawline(p2.x,p2.y,p1.x,p1.y);
    //vga->drawline(p1.x,p1.y,p0.x,p0.y);

/*  vga->setcolor(0x00ff00);
    circle(p0.x,p0.y,5);
    circle(p1.x,p1.y,5);
    circle(p2.x,p2.y,5);
    circle(p3.x,p3.y,5);*/

/*    vga->setcolor(0xff00ff);
    for(d=0.00;d<1.00;d+=0.001) {
	int x = (int)(p0.x * d*d*d + p1.x * 3*(1-d)*d*d + p2.x * 3*(1-d)*(1-d)*d + p3.x * (1-d)*(1-d)*(1-d));
	int y = (int)(p0.y * d*d*d + p1.y * 3*(1-d)*d*d + p2.y * 3*(1-d)*(1-d)*d + p3.y * (1-d)*(1-d)*(1-d));
	pp(x,y);
    }*/

    num = approximate(p0,p1,p2,p3,q);
    for(t=0;t<num;t++)
    {
	qspline(q[t].start,q[t].control,q[t].end);
    }
}
void line(plotxy p0, plotxy p1, struct swfmatrix*m)
{
    transform(&p0,m);
    transform(&p1,m);
    vga->drawline((int)p0.x,(int)p0.y,(int)p1.x,(int)p1.y);
}

void swfoutput_drawpath(swfoutput*output, T1_OUTLINE*outline, struct swfmatrix*m)
{
    double x=0,y=0;
    double lastx=0,lasty=0;

    vga->setcolor(0xffffff);
    while (outline)
    {
	logf("<debug> Pathtype:%s",outline->type == T1_PATHTYPE_MOVE?"MOVE":
						    (outline->type == T1_PATHTYPE_LINE?"LINE"
										      :"BEZIER"));
	logf("<debug> relative coordinates: %08x,%08x", outline->dest.x, outline->dest.y);
	x += (outline->dest.x/(float)0xffff);
	y += (outline->dest.y/(float)0xffff);
	logf("<debug> coordinates: %f,%f", x, y);
	if(outline->type == T1_PATHTYPE_MOVE)
	{
	}
	else if(outline->type == T1_PATHTYPE_LINE) 
	{
	    plotxy p0;
	    plotxy p1;
	    p0.x=lastx;
	    p0.y=lasty;
	    p1.x=x;
	    p1.y=y;
	    line(p0,p1,m);
	}
	else if(outline->type == T1_PATHTYPE_BEZIER)
	{
	    plotxy p0;
	    plotxy p1;
	    plotxy p2;
	    plotxy p3;
	    T1_BEZIERSEGMENT*o2 = (T1_BEZIERSEGMENT*)outline;
	    p0.x=x; 
	    p0.y=y;
	    p1.x=o2->C.x/(float)0xffff+lastx;
	    p1.y=o2->C.y/(float)0xffff+lasty;
	    p2.x=o2->B.x/(float)0xffff+lastx;
	    p2.y=o2->B.y/(float)0xffff+lasty;
	    p3.x=lastx;
	    p3.y=lasty;
	    spline(p0,p1,p2,p3,m);
//	    vga->drawline(320+lastx, 240+lasty, 320+x, 240+y);
	} 
	else {
	 printf("outline type:%d\n", outline->type);
	}
	
	lastx=x;
	lasty=y;
	outline = outline->link;
    }
    XFlush(X.d);
}

void processchar(struct swfoutput*obj, int i, char c, swfmatrix*m)
{
    T1_OUTLINE*outline;
    int width = T1_GetCharWidth(i, c);
    BBox bbox = T1_GetCharBBox(i, c);
    char*charname= T1_GetCharName(i, c);
    logf("<debug> Font name is %s", T1_GetFontFileName(i));
    logf("<debug> char 0x%02x is named %s\n",c,charname);
    logf("<debug> bbox: %d %d %d %d\n",bbox.llx,bbox.lly,bbox.urx,bbox.ury);
    if(!charname || charname[0] == '.') 
    {
     logf("<error> Char to set is not defined!");
     logf("<error> -  font file is %s\n", T1_GetFontFileName(i));
     logf("<error> -  char 0x%02x is named %s\n",c,charname);
    }
    swfmatrix m2=*m;    
    m2.m11/=100;
    m2.m21/=100;
    m2.m12/=100;
    m2.m22/=100;
    outline =  T1_GetCharOutline( i, c, 100.0, 0);
    swfoutput_drawpath(obj, outline, &m2);
}

void swfoutput_init(struct swfoutput* obj, int sizex, int sizey) 
{
  GLYPH *glyph;
  int i;

  logf("<verbose> initializing swf output for size %d*%d\n", sizex,sizey);

  vga=new modexwindow(sizex,sizey,"");
  vga->setdirect();
  if(!vga->on()) exit(1);
  vga->setcolor(0xff00ff);
  
  obj->t1font = 0;
  obj->t1fontsize = 0.0025;
}

int swfoutput_setfont(struct swfoutput*obj, int t1id)
{
    obj->t1font = t1id;
}

void swfoutput_setfontsize(struct swfoutput* obj, double size)
{
    obj->t1fontsize = size; 
}

void swfoutput_setfontmatrix(struct swfoutput*obj,double m11,double m12,
                                                  double m21,double m22)
{
    obj->fontm11 = m11;
    obj->fontm12 = m12;
    obj->fontm21 = m21;
    obj->fontm22 = m22;
}

void swfoutput_drawchar(struct swfoutput* obj,double x,double y,char a) 
{
    swfmatrix m;
    m.m11 = obj->fontm11*obj->t1fontsize;
    m.m12 = obj->fontm12*obj->t1fontsize;
    m.m21 = obj->fontm21*obj->t1fontsize;
    m.m22 = obj->fontm22*obj->t1fontsize;
    m.m13 = x;
    m.m23 = y;
    processchar(obj, obj->t1font, a, &m);
}

void swfoutput_destroy(struct swfoutput* obj) 
{
    T1_CloseLib();
    vga->off();
    delete vga;
}

