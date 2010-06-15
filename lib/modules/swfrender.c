/* swfrender.c

   functions for rendering swf content
      
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2004 Mederra Oy <http://www.mederra.fi>
   Copyright (c) 2004 Matthias Kramm
 
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../rfxswf.h"

/* one bit flag: */
#define clip_type 0
#define fill_type 1

typedef struct _renderpoint
{
    float x;
    U32 depth;

    SHAPELINE*shapeline;
    SHAPE2*s;
    
} renderpoint_t;

/* 
    enum {clip_type, solidfill_type, texturefill_type, gradientfill_type} type;
    float fx;
    int x;
    U32 depth;
    U32 clipdepth;

    // solidfill;
    RGBA color; 
    
    // texturefill
    bitmap_t* bitmap;

    // gradientfill
    gradient_t* gradient;

    // texture- & gradientfill;
    U32 x,y;
    U32 dx,dy;

*/

typedef struct _renderline
{
    TAG*points; //incremented in 128 byte steps
    int num;
    U32 pending_clipdepth;
} renderline_t;

typedef struct _bitmap {
    int width;
    int height;
    RGBA*data;
    int id;
    struct _bitmap*next;
} bitmap_t;

typedef struct _renderbuf_internal
{
    renderline_t*lines;
    bitmap_t*bitmaps;
    int antialize;
    int multiply;
    int width2,height2;
    int shapes;
    int ymin, ymax;
    
    RGBA* img;
    int* zbuf; 
} renderbuf_internal;

#define DEBUG 0

static inline void add_pixel(RENDERBUF*dest, float x, int y, renderpoint_t*p)
{
    renderbuf_internal*i = (renderbuf_internal*)dest->internal;
    if(x >= i->width2 || y >= i->height2 || y<0) return;
    p->x = x;
    if(y<i->ymin) i->ymin = y;
    if(y>i->ymax) i->ymax = y;

    i->lines[y].num++;
    swf_SetBlock(i->lines[y].points, (U8*)p, sizeof(renderpoint_t));
}

/* set this to 0.777777 or something if the "both fillstyles set while not inside shape"
   problem appears to often */
#define CUT 0.77887789

#define INT(x) ((int)((x)+16)-16)

static void add_line(RENDERBUF*buf, double x1, double y1, double x2, double y2, renderpoint_t*p)
{
    renderbuf_internal*i = (renderbuf_internal*)buf->internal;
    double diffx, diffy;
    double ny1, ny2, stepx;
/*    if(DEBUG&4) {
        int l = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
        printf(" l[%d - %.2f/%.2f -> %.2f/%.2f]\n", l, x1/20.0, y1/20.0, x2/20.0, y2/20.0);
    }*/
    assert(p->shapeline);

    y1=y1*i->multiply;
    y2=y2*i->multiply;
    x1=x1*i->multiply;
    x2=x2*i->multiply;
    
    y1 = y1/20.0;
    y2 = y2/20.0;
    x1 = x1/20.0;
    x2 = x2/20.0;

    if(y2 < y1) {
        double x;
        double y;
	x = x1;x1 = x2;x2=x;
	y = y1;y1 = y2;y2=y;
    }
    
    diffx = x2 - x1;
    diffy = y2 - y1;
    
    ny1 = INT(y1)+CUT;
    ny2 = INT(y2)+CUT;

    if(ny1 < y1) {
        ny1 = INT(y1) + 1.0 + CUT;
    }
    if(ny2 >= y2) {
        ny2 = INT(y2) - 1.0 + CUT;
    }

    if(ny1 > ny2)
        return;

    stepx = diffx/diffy;
    x1 = x1 + (ny1-y1)*stepx;
    x2 = x2 + (ny2-y2)*stepx;

    {
	int posy=INT(ny1);
	int endy=INT(ny2);
	double posx=0;
	double startx = x1;

	while(posy<=endy) {
	    float xx = (float)(startx + posx);
	    add_pixel(buf, xx ,posy, p);
	    posx+=stepx;
	    posy++;
	}
    }
}
#define PI 3.14159265358979
static void add_solidline(RENDERBUF*buf, double x1, double y1, double x2, double y2, double width, renderpoint_t*p)
{
    renderbuf_internal*i = (renderbuf_internal*)buf->internal;

    double dx = x2-x1;
    double dy = y2-y1;
    double sd;
    double d;

    int t;
    int segments;
    double lastx,lasty;
    double vx,vy;
    double xx,yy;
  
    /* Make sure the line is always at least one pixel wide */
#ifdef LINEMODE1
    /* That's what Macromedia's Player does at least at zoom level >= 1.  */
    width += 20;
#else
    /* That's what Macromedia's Player seems to do at zoom level 0.  */
    /* TODO: needs testing */

    /* TODO: how does this interact with scaling? */
    if(width * i->multiply < 20)
	width = 20 / i->multiply;
#endif

    sd = (double)dx*(double)dx+(double)dy*(double)dy;
    d = sqrt(sd);

    if(!dx && !dy) {
        vx = 1;
        vy = 0;
    } else {
        vx = ( dy/d);
        vy = (-dx/d);
    }

    segments = (int)(width/2);
    if(segments < 2)
        segments = 2;

    segments = 8;

    vx=vx*width*0.5;
    vy=vy*width*0.5;

    xx = x2+vx;
    yy = y2+vy;
    add_line(buf, x1+vx, y1+vy, xx, yy, p);
    lastx = xx;
    lasty = yy;
    for(t=1;t<segments;t++) {
        double s = sin(t*PI/segments);
        double c = cos(t*PI/segments);
        xx = (x2 + vx*c - vy*s);
        yy = (y2 + vx*s + vy*c);
        add_line(buf, lastx, lasty, xx, yy, p);
        lastx = xx;
        lasty = yy;
    }
    
    xx = (x2-vx);
    yy = (y2-vy);
    add_line(buf, lastx, lasty, xx, yy, p);
    lastx = xx;
    lasty = yy;
    xx = (x1-vx);
    yy = (y1-vy);
    add_line(buf, lastx, lasty, xx, yy, p);
    lastx = xx;
    lasty = yy;
    for(t=1;t<segments;t++) {
        double s = sin(t*PI/segments);
        double c = cos(t*PI/segments);
        xx = (x1 - vx*c + vy*s);
        yy = (y1 - vx*s - vy*c);
        add_line(buf, lastx, lasty, xx, yy, p);
        lastx = xx;
        lasty = yy;
    }
    add_line(buf, lastx, lasty, (x1+vx), (y1+vy), p);
}

static inline void transform_point(MATRIX*m, int x, int y, int*dx, int*dy)
{
    SPOINT p,d;
    p.x = x;
    p.y = y;
    d = swf_TurnPoint(p, m);
    *dx = d.x;
    *dy = d.y;
}

static int compare_renderpoints(const void * _a, const void * _b)
{
    renderpoint_t*a = (renderpoint_t*)_a;
    renderpoint_t*b = (renderpoint_t*)_b;
    if(a->x < b->x) return -1;
    if(a->x > b->x) return 1;
    return 0;
}

void swf_Render_Init(RENDERBUF*buf, int posx, int posy, int width, int height, int antialize, int multiply)
{
    renderbuf_internal*i;
    int y;
    memset(buf, 0, sizeof(RENDERBUF));
    buf->width = width*multiply;
    buf->height = height*multiply;
    buf->posx = posx;
    buf->posy = posy;
    buf->internal = (renderbuf_internal*)rfx_calloc(sizeof(renderbuf_internal));
    i = (renderbuf_internal*)buf->internal;
    if(antialize < 1)
	antialize = 1;
    i->antialize = antialize;
    i->multiply = multiply*antialize;
    i->height2 = antialize*buf->height;
    i->width2 = antialize*buf->width;
    i->lines = (renderline_t*)rfx_alloc(i->height2*sizeof(renderline_t));
    for(y=0;y<i->height2;y++) {
	memset(&i->lines[y], 0, sizeof(renderline_t));
        i->lines[y].points = swf_InsertTag(0, 0);
        i->lines[y].num = 0;
    }
    i->zbuf = (int*)rfx_calloc(sizeof(int)*i->width2*i->height2);
    i->img = (RGBA*)rfx_calloc(sizeof(RGBA)*i->width2*i->height2);
    i->shapes = 0;
    i->ymin = 0x7fffffff;
    i->ymax = -0x80000000;
}
void swf_Render_SetBackground(RENDERBUF*buf, RGBA*img, int width, int height)
{
    renderbuf_internal*i = (renderbuf_internal*)buf->internal;
    int x,xx,y,yy;
    int xstep=width*65536/i->width2;
    int ystep=height*65536/i->height2;
    if(i->shapes) {
	fprintf(stderr, "rfxswf: Warning: swf_Render_SetBackground() called after drawing shapes\n");
    }
    for(y=0,yy=0;y<i->height2;y++,yy+=ystep) {
	RGBA*src = &img[(yy>>16) * width];
	RGBA*line = &i->img[y * i->width2];
	for(x=0,xx=0;x<i->width2;x++,xx+=xstep) {
	    line[x] = src[xx>>16];
	}
    }
}
void swf_Render_SetBackgroundColor(RENDERBUF*buf, RGBA color)
{
    swf_Render_SetBackground(buf, &color, 1, 1);
}
void swf_Render_AddImage(RENDERBUF*buf, U16 id, RGBA*img, int width, int height)
{
    renderbuf_internal*i = (renderbuf_internal*)buf->internal;

    bitmap_t*bm = (bitmap_t*)rfx_calloc(sizeof(bitmap_t));
    bm->id = id;
    bm->width = width;
    bm->height = height;
    bm->data = (RGBA*)rfx_alloc(width*height*4);
    memcpy(bm->data, img, width*height*4);

    bm->next = i->bitmaps;
    i->bitmaps = bm;
}
void swf_Render_ClearCanvas(RENDERBUF*dest)
{
    renderbuf_internal*i = (renderbuf_internal*)dest->internal;
    int y;
    for(y=0;y<i->height2;y++) {
        swf_ClearTag(i->lines[y].points);
    }
    memset(i->zbuf, 0, sizeof(int)*i->width2*i->height2);
    memset(i->img, 0, sizeof(RGBA)*i->width2*i->height2);
}
void swf_Render_Delete(RENDERBUF*dest)
{
    renderbuf_internal*i = (renderbuf_internal*)dest->internal;
    int y;
    bitmap_t*b = i->bitmaps;

    /* delete canvas */
    rfx_free(i->zbuf);
    rfx_free(i->img);

    /* delete line buffers */
    for(y=0;y<i->height2;y++) {
        swf_DeleteTag(0, i->lines[y].points);
        i->lines[y].points = 0;
    }

    /* delete bitmaps */
    while(b) {
        bitmap_t*next = b->next;
        free(b->data);b->data=0;
        rfx_free(b);
        b = next;
    }

    rfx_free(i->lines); i->lines = 0;
    rfx_free(dest->internal); dest->internal = 0;
}

static SHAPE2* linestyle2fillstyle(SHAPE2*shape)
{
    SHAPE2*s = (SHAPE2*)rfx_calloc(sizeof(SHAPE2));
    int t;
    s->numfillstyles = shape->numlinestyles;
    s->fillstyles = (FILLSTYLE*)rfx_calloc(sizeof(FILLSTYLE)*shape->numlinestyles);
    s->lines = (SHAPELINE*)rfx_calloc(sizeof(SHAPELINE)*shape->numlinestyles);
    for(t=0;t<shape->numlinestyles;t++) {
        s->lines[t].fillstyle0 = t+1;
        s->fillstyles[t].type = FILL_SOLID;
        s->fillstyles[t].color = shape->linestyles[t].color;
    }
    return s;
}

void swf_Process(RENDERBUF*dest, U32 clipdepth);

double matrixsize(MATRIX*m)
{
    double l1 = sqrt((m->sx /65536.0) * (m->sx /65536.0) + (m->r0 /65536.0) * (m->r0/65536.0) );
    double l2 = sqrt((m->r1 /65536.0) * (m->r1 /65536.0) + (m->sy /65536.0) * (m->sy/65536.0) );
    return sqrt(l1*l2);
}

void swf_RenderShape(RENDERBUF*dest, SHAPE2*shape, MATRIX*m, CXFORM*c, U16 _depth,U16 _clipdepth)
{
    renderbuf_internal*i = (renderbuf_internal*)dest->internal;
    
    SHAPELINE*line;
    int x=0,y=0;
    MATRIX mat = *m;
    SHAPE2* s2 = 0;
    SHAPE2* lshape = 0;
    renderpoint_t p, lp;
    U32 clipdepth;
    double widthmultiply = matrixsize(m);

    memset(&p, 0, sizeof(renderpoint_t));
    memset(&lp, 0, sizeof(renderpoint_t));
    
    clipdepth = _clipdepth? _clipdepth << 16 | 0xffff : 0;
    p.depth = _depth << 16;

    mat.tx -= dest->posx*20;
    mat.ty -= dest->posy*20;

    s2 = swf_Shape2Clone(shape);
    line = s2->lines;
    if(shape->numfillstyles) {
        int t;
        p.s = s2;
        /* multiply fillstyles matrices with placement matrix-
           important for texture and gradient fill */
        for(t=0;t<s2->numfillstyles;t++) {
            MATRIX nm;
            swf_MatrixJoin(&nm, &mat, &s2->fillstyles[t].m);
            /*nm.sx *= i->multiply;
            nm.sy *= i->multiply;
            nm.r0 *= i->multiply;
            nm.r1 *= i->multiply;
            nm.tx *= i->multiply;
            nm.ty *= i->multiply;*/
            s2->fillstyles[t].m = nm;
        }
    }

    if(shape->numlinestyles) {
        lshape = linestyle2fillstyle(shape);
        lp.s = lshape;
        lp.depth = (_depth << 16)+1;
    }


    while(line)
    {
        int x1,y1,x2,y2,x3,y3;

        if(line->type == moveTo) {
        } else if(line->type == lineTo) {
            transform_point(&mat, x, y, &x1, &y1);
            transform_point(&mat, line->x, line->y, &x3, &y3);
            
            if(line->linestyle && ! clipdepth) {
                lp.shapeline = &lshape->lines[line->linestyle-1];
                add_solidline(dest, x1, y1, x3, y3, shape->linestyles[line->linestyle-1].width * widthmultiply, &lp);
                lp.depth++;
            }
            if(line->fillstyle0 || line->fillstyle1) {
                assert(shape->numfillstyles);
		p.shapeline = line;
                add_line(dest, x1, y1, x3, y3, &p);
            }
        } else if(line->type == splineTo) {
	    int c,t,parts,qparts;
	    double xx,yy;
            
            transform_point(&mat, x, y, &x1, &y1);
            transform_point(&mat, line->sx, line->sy, &x2, &y2);
            transform_point(&mat, line->x, line->y, &x3, &y3);
            
            c = abs(x3-2*x2+x1) + abs(y3-2*y2+y1);
            xx=x1;
	    yy=y1;

            parts = (int)(sqrt((float)c)/3);
            if(!parts) parts = 1;

            for(t=1;t<=parts;t++) {
                double nx = (double)(t*t*x3 + 2*t*(parts-t)*x2 + (parts-t)*(parts-t)*x1)/(double)(parts*parts);
                double ny = (double)(t*t*y3 + 2*t*(parts-t)*y2 + (parts-t)*(parts-t)*y1)/(double)(parts*parts);
                
                if(line->linestyle && ! clipdepth) {
                    lp.shapeline = &lshape->lines[line->linestyle-1];
                    add_solidline(dest, xx, yy, nx, ny, shape->linestyles[line->linestyle-1].width * widthmultiply, &lp);
                    lp.depth++;
                }
                if(line->fillstyle0 || line->fillstyle1) {
                    assert(shape->numfillstyles);
		    p.shapeline = line;
                    add_line(dest, xx, yy, nx, ny, &p);
                }

                xx = nx;
                yy = ny;
            }
        }
        x = line->x;
        y = line->y;
        line = line->next;
    }
    
    swf_Process(dest, clipdepth);
    
    if(s2) {
	swf_Shape2Free(s2);rfx_free(s2);s2=0;
    }
    if(lshape) {
	swf_Shape2Free(lshape);rfx_free(lshape);lshape=0;
    }

}

static RGBA color_red = {255,255,0,0};
static RGBA color_white = {255,255,255,255};
static RGBA color_black = {255,0,0,0};

static void fill_clip(RGBA*line, int*z, int y, int x1, int x2, U32 depth)
{
    int x = x1;
    if(x1>=x2)
	return;
    do {
	if(depth > z[x]) {
	    z[x] = depth;
	}
    } while(++x<x2);
}


static void fill_solid(RGBA*line, int*z, int y, int x1, int x2, RGBA col, U32 depth)
{
    int x = x1;

    if(col.a!=255) {
        int ainv = 255-col.a;
        col.r = (col.r*col.a)>>8;
        col.g = (col.g*col.a)>>8;
        col.b = (col.b*col.a)>>8;
        col.a = 255;
        do {
	    if(depth >= z[x]) {
		line[x].r = ((line[x].r*ainv)>>8)+col.r;
		line[x].g = ((line[x].g*ainv)>>8)+col.g;
		line[x].b = ((line[x].b*ainv)>>8)+col.b;
		line[x].a = 255;
		z[x] = depth;
	    }
        } while(++x<x2);
    } else {
        do {
	    if(depth >= z[x]) {
		line[x] = col;
		z[x] = depth;
	    }
        } while(++x<x2);
    }
}

static int inline clamp(int v)
{
    if(v>255) return 255;
    else return v;
}

static void fill_bitmap(RGBA*line, int*z, int y, int x1, int x2, MATRIX*m, bitmap_t*b, int clipbitmap, U32 depth, double fmultiply)
{
    int x = x1;
    
    double m11= m->sx*fmultiply/65536.0, m21= m->r1*fmultiply/65536.0;
    double m12= m->r0*fmultiply/65536.0, m22= m->sy*fmultiply/65536.0;
    double rx = m->tx*fmultiply/20.0;
    double ry = m->ty*fmultiply/20.0;

    double det = m11*m22 - m12*m21;
    if(fabs(det) < 0.0005) { 
	/* x direction equals y direction- the image is invisible */
	return;
    }
    det = 20.0/det;

    if(!b->width || !b->height) {
        fill_solid(line, z, y, x1, x2, color_red, depth);
        return;
    }

    do {
	if(depth >= z[x]) {
	    RGBA col;
	    int xx = (int)((  (x - rx) * m22 - (y - ry) * m21)*det);
	    int yy = (int)((- (x - rx) * m12 + (y - ry) * m11)*det);
	    int ainv;

	    if(clipbitmap) {
		if(xx<0) xx=0;
		if(xx>=b->width) xx = b->width-1;
		if(yy<0) yy=0;
		if(yy>=b->height) yy = b->height-1;
	    } else {
		xx %= b->width;
		yy %= b->height;
		if(xx<0) xx += b->width;
		if(yy<0) yy += b->height;
	    }

	    col = b->data[yy*b->width+xx];
	    ainv = 255-col.a;

	    line[x].r = clamp(((line[x].r*ainv)>>8)+col.r);
	    line[x].g = clamp(((line[x].g*ainv)>>8)+col.g);
	    line[x].b = clamp(((line[x].b*ainv)>>8)+col.b);
	    line[x].a = 255;
	    
	    z[x] = depth;
	}
    } while(++x<x2);
}

static void fill_gradient(RGBA*line, int*z, int y, int x1, int x2, MATRIX*m, GRADIENT*g, int type, U32 depth, double fmultiply)
{
    int x = x1;
    
    double m11= m->sx*fmultiply/80, m21= m->r1*fmultiply/80;
    double m12= m->r0*fmultiply/80, m22= m->sy*fmultiply/80;
    double rx = m->tx*fmultiply/20.0;
    double ry = m->ty*fmultiply/20.0;

    double det = m11*m22 - m12*m21;
    if(fabs(det) < 0.0005) { 
	/* x direction equals y direction- the image is invisible */
	return;
    }
    det = 1.0/det;

    RGBA palette[512];
    RGBA oldcol = g->rgba[0];
    int r0 = g->ratios[0]*2;
    int t;
    for(t=0;t<r0;t++) 
	palette[t] = oldcol;
    for(t=1;t<g->num;t++) {
	int r1 = g->ratios[t]*2;
	RGBA newcol = g->rgba[t];
	if(r0 == r1)
	    continue;
	//printf("%d %d->%d %02x%02x%02x%02x->%02x%02x%02x%02x\n", 
	//	t, r0, r1, oldcol.r,oldcol.g,oldcol.b,oldcol.a,
	//	newcol.r,newcol.g,newcol.b,newcol.a);
	double f = 1.0 / (r1-r0);
	double p0 = 1;
	double p1 = 0;
	int s;
	for(;r0<=r1;r0++) {
	    palette[r0].r = oldcol.r*p0 + newcol.r*p1;
	    palette[r0].g = oldcol.g*p0 + newcol.g*p1;
	    palette[r0].b = oldcol.b*p0 + newcol.b*p1;
	    palette[r0].a = oldcol.a*p0 + newcol.a*p1;
	    p0 -= f;
	    p1 += f;
	}
	oldcol = newcol;
    }
    for(t=r0;t<512;t++) 
	palette[t] = oldcol;

    do {
	if(depth >= z[x]) {
	    RGBA col;
	    double xx = (  (x - rx) * m22 - (y - ry) * m21)*det;
	    double yy = (- (x - rx) * m12 + (y - ry) * m11)*det;

	    if(type == FILL_LINEAR) {
		int xr = xx*256;
		if(xr<-256)
		    xr = -256;
		if(xr>255)
		    xr = 255;
		col = palette[xr+256];
	    } else {
		int xr = sqrt(xx*xx+yy*yy)*511;
		if(xr<0)
		    xr = 0;
		if(xr>511)
		    xr = 511;
		col = palette[xr];
	    }
	    int ainv;
	    ainv = 255-col.a;
	    line[x].r = clamp(((line[x].r*ainv)>>8)+col.r);
	    line[x].g = clamp(((line[x].g*ainv)>>8)+col.g);
	    line[x].b = clamp(((line[x].b*ainv)>>8)+col.b);
	    line[x].a = 255;
	    
	    z[x] = depth;
	}
    } while(++x<x2);
}

typedef struct _layer {
    int fillid;
    renderpoint_t*p;
    struct _layer*next;
    struct _layer*prev;
} layer_t;

typedef struct {
    layer_t*layers;
} state_t;


static void fill(RENDERBUF*dest, RGBA*line, int*zline, int y, int x1, int x2, state_t*fillstate, U32 clipdepth)
{
    renderbuf_internal*i = (renderbuf_internal*)dest->internal;
    int clip=1;

    layer_t*l = fillstate->layers;

    if(x1>=x2) //zero width? nothing to do.
        return;
    
    while(l) {
        if(l->fillid == 0) {
            /* not filled. TODO: we should never add those in the first place */
            if(DEBUG&2)
                printf("(not filled)");
        } else if(l->fillid > l->p->s->numfillstyles) {
            fprintf(stderr, "Fill style out of bounds (%d>%d)", l->fillid, l->p->s->numlinestyles);
        } else if(clipdepth) {
	    /* filled region- not used for clipping */
	    clip = 0;
	} else {
            FILLSTYLE*f;
            if(DEBUG&2) 
                printf("(%d -> %d style %d)", x1, x2, l->fillid);

            f = &l->p->s->fillstyles[l->fillid-1];

	    if(f->type == FILL_SOLID) {
                /* plain color fill */
                fill_solid(line, zline, y, x1, x2, f->color, l->p->depth);
            } else if(f->type == FILL_TILED || f->type == FILL_CLIPPED || f->type == (FILL_TILED|2) || f->type == (FILL_CLIPPED|2)) {
                /* TODO: optimize (do this in add_pixel()?) */
                bitmap_t* b = i->bitmaps;
                while(b && b->id != f->id_bitmap) {
                    b = b->next;
                }
                if(!b) {
                    fprintf(stderr, "Shape references unknown bitmap %d\n", f->id_bitmap);
                    fill_solid(line, zline, y, x1, x2, color_red, l->p->depth);
                } else {
                    fill_bitmap(line, zline, y, x1, x2, &f->m, b, /*clipped?*/f->type&1, l->p->depth, i->multiply);
                }
            } else if(f->type == FILL_LINEAR || f->type == FILL_RADIAL) {
		fill_gradient(line, zline, y, x1, x2, &f->m, &f->gradient, f->type, l->p->depth, i->multiply);
            } else {
                fprintf(stderr, "Undefined fillmode: %02x\n", f->type);
	    }
        }
	l = l->next;
    }
    if(clip && clipdepth) {
	fill_clip(line, zline, y, x1, x2, clipdepth);
    }
}

static void search_layer(state_t*state, int depth, layer_t**before, layer_t**self, layer_t**after)
{
    layer_t*last=0,*l = state->layers;
    while(l && l->p->depth < depth) {
        last = l;
        l = l->next;
    }
    *before = last;
    if(l && l->p->depth == depth)
        *self = l;
    else
        *after = l;
}
static void delete_layer(state_t*state, layer_t*todel)
{
    layer_t*before=todel->prev;
    layer_t*next = todel->next;
    rfx_free(todel);
    if(!before) {
        state->layers = next;
        if(next)
            next->prev = 0;
    } else {
        before->next = next;
        if(before->next)
            before->next->prev = before;
    }
}
static void add_layer(state_t*state, layer_t*before, layer_t*toadd)
{
    if(!before) {
        toadd->next = state->layers;
        toadd->prev = 0;
        state->layers=toadd;
    } else {
        toadd->next = before->next;
        toadd->prev = before;
        before->next = toadd;
    }
    if(toadd->next)
        toadd->next->prev = toadd;
}
static void free_layers(state_t* state)
{
    layer_t*l = state->layers;
    while(l) {
        layer_t*next = l->next;
        rfx_free(l);
        l = next;
    }
}

static void change_state(int y, state_t* state, renderpoint_t*p)
{
    layer_t*before=0, *self=0, *after=0;

    if(DEBUG&2) { 
        printf("[(%f,%d)/%d/%d-%d]", p->x, y, p->depth, p->shapeline->fillstyle0, p->shapeline->fillstyle1);
    }

    search_layer(state, p->depth, &before, &self, &after);

    if(self) {
        /* shape update */
        if(self->fillid<0/*??*/ || !p->shapeline->fillstyle0 || !p->shapeline->fillstyle1) {
            /* filling ends */
            if(DEBUG&2) printf("<D>");
            
            delete_layer(state, self);
        } else { 
            /*both fill0 and fill1 are set- exchange the two, updating the layer */
            if(self->fillid == p->shapeline->fillstyle0) {
                self->fillid = p->shapeline->fillstyle1;
                self->p = p;
                if(DEBUG&2) printf("<X>");
            } else if(self->fillid == p->shapeline->fillstyle1) {
                self->fillid = p->shapeline->fillstyle0;
                self->p = p;
                if(DEBUG&2) printf("<X>");
            } else {
                /* buggy shape. keep everything as-is. */
                if(DEBUG&2) printf("<!>");
                //fprintf(stderr, "<line %d: bad swap>\n", y);
            }
        }
        return;
    } else {
        layer_t* n = 0;
        if(p->shapeline && p->shapeline->fillstyle0 && p->shapeline->fillstyle1) {
            /* this is a hack- a better way would be to make sure that
               we always get (0,32), (32, 33), (33, 0) in the right order if
               they happen to fall on the same pixel.
               (not: (0,32), (33, 0), (32, 33))
	       Notice: Weird fill styles appear if linestyles are involved, too.
            */
            fprintf(stderr, "<line %d: both fillstyles set while not inside shape>\n", y);
            return;
        }
        
        n = (layer_t*)rfx_calloc(sizeof(layer_t));

        if(DEBUG&2) printf("<+>");

	n->fillid = p->shapeline->fillstyle0 ? p->shapeline->fillstyle0 : p->shapeline->fillstyle1;
	n->p = p;

        add_layer(state, before, n);
    }
}

void swf_Process(RENDERBUF*dest, U32 clipdepth)
{
    renderbuf_internal*i = (renderbuf_internal*)dest->internal;
    int y;
    
    if(i->ymax < i->ymin) {
	/* shape is empty. return. 
	   only, if it's a clipshape, remember the clipdepth */
	if(clipdepth) {
	    for(y=0;y<i->height2;y++) {
		if(clipdepth > i->lines[y].pending_clipdepth)
		    i->lines[y].pending_clipdepth = clipdepth;
	    }
	}
	return; //nothing (else) to do
    }

    if(clipdepth) {
	/* lines outside the clip shape are not filled
	   immediately, only the highest clipdepth so far is
	   stored there. They will be clipfilled once there's
	   actually something about to happen in that line */
	for(y=0;y<i->ymin;y++) {
	    if(clipdepth > i->lines[y].pending_clipdepth)
		i->lines[y].pending_clipdepth = clipdepth;
	}
	for(y=i->ymax+1;y<i->height2;y++) {
	    if(clipdepth > i->lines[y].pending_clipdepth)
		i->lines[y].pending_clipdepth = clipdepth;
	}
    }
    
    for(y=i->ymin;y<=i->ymax;y++) {
        int n;
        TAG*tag = i->lines[y].points;
        int num = i->lines[y].num;
	renderpoint_t*points = (renderpoint_t*)tag->data;
        RGBA*line = &i->img[i->width2*y];
        int*zline = &i->zbuf[i->width2*y];
	int lastx = 0;
	state_t fillstate;
        memset(&fillstate, 0, sizeof(state_t));
        qsort(points, num, sizeof(renderpoint_t), compare_renderpoints);
	/* resort points */
	/*if(y==884) {
	    for(n=0;n<num;n++) {
		printf("%f (%d/%d) %d\n", points[n].x, 
			points[n].shapeline->fillstyle0,
			points[n].shapeline->fillstyle1,
			points[n].shapeline->linestyle);
	    }
	}*/

	if(i->lines[y].pending_clipdepth && !clipdepth) {
	    fill_clip(line, zline, y, 0, i->width2, i->lines[y].pending_clipdepth);
	    i->lines[y].pending_clipdepth=0;
	}

        for(n=0;n<num;n++) {
            renderpoint_t*p = &points[n];
            renderpoint_t*next= n<num-1?&points[n+1]:0;
            int startx = (int)p->x;
            int endx = (int)(next?next->x:i->width2);
            if(endx > i->width2)
                endx = i->width2;
            if(startx < 0)
                startx = 0;
            if(endx < 0)
                endx = 0;

	    if(clipdepth) {
		/* for clipping, the inverse is filled 
		   TODO: lastx!=startx only at the start of the loop, 
		         so this might be moved up
		 */
		fill_clip(line, zline, y, lastx, startx, clipdepth);
	    }
	    change_state(y, &fillstate, p);

	    fill(dest, line, zline, y, startx, endx, &fillstate, clipdepth);
/*	    if(y == 0 && startx == 232 && endx == 418) {
		printf("ymin=%d ymax=%d\n", i->ymin, i->ymax);
		for(n=0;n<num;n++) {
		    renderpoint_t*p = &points[n];
		    printf("x=%f depth=%08x\n", p->x, p->depth);
		}
	    }*/

	    lastx = endx;
            if(endx == i->width2)
                break;
        }
	if(clipdepth) {
	    /* TODO: is lastx *ever* != i->width2 here? */
	    fill_clip(line, zline, y, lastx, i->width2, clipdepth);
	}
        free_layers(&fillstate);
	
	i->lines[y].num = 0;
	swf_ClearTag(i->lines[y].points);
    }
    i->ymin = 0x7fffffff;
    i->ymax = -0x80000000;
}

RGBA* swf_Render(RENDERBUF*dest)
{
    renderbuf_internal*i = (renderbuf_internal*)dest->internal;
    RGBA* img = (RGBA*)rfx_alloc(sizeof(RGBA)*dest->width*dest->height);
    int y;
    int antialize = i->antialize;
   
    if(antialize <= 1) /* no antializing */ {
	for(y=0;y<i->height2;y++) {
	    RGBA*line = &i->img[y*i->width2];
	    memcpy(&img[y*dest->width], line, sizeof(RGBA)*dest->width);
	}
    } else {
	RGBA**lines = (RGBA**)rfx_calloc(sizeof(RGBA*)*antialize);
	int q = antialize*antialize;
	int ypos = 0;
	for(y=0;y<i->height2;y++) {
	    int n;
	    ypos = y % antialize;
	    lines[ypos] = &i->img[y*i->width2];
	    if(ypos == antialize-1) {
		RGBA*out = &img[(y / antialize)*dest->width];
		int x;
		int r,g,b,a;
		for(x=0;x<dest->width;x++) {
		    int xpos = x*antialize;
		    int yp;
		    U32 r=0,g=0,b=0,a=0;
		    for(yp=0;yp<antialize;yp++) {
			RGBA*lp = &lines[yp][xpos];
			int xp;
			for(xp=0;xp<antialize;xp++) {
			    RGBA*p = &lp[xp];
			    r += p->r;
			    g += p->g;
			    b += p->b;
			    a += p->a;
			}
		    }
		    out[x].r = r / q;
		    out[x].g = g / q;
		    out[x].b = b / q;
		    out[x].a = a / q;
		}
	    }
	}
	rfx_free(lines);
    }
    return img;
}

typedef struct
{
    int numchars;
    SHAPE2**glyphs;
} font_t;

enum CHARACTER_TYPE {none_type, shape_type, image_type, text_type, edittext_type, font_type, sprite_type};
typedef struct
{
    TAG*tag;
    SRECT*bbox;
    enum CHARACTER_TYPE type;
    union {
        SHAPE2*shape;
        font_t*font;
    } obj;
} character_t;

int compare_placements(const void *v1, const void *v2)
{
    SWFPLACEOBJECT*p1 = (SWFPLACEOBJECT*)v1;
    SWFPLACEOBJECT*p2 = (SWFPLACEOBJECT*)v2;
    if(p1->depth != p2->depth)
	return (int)p1->depth - (int)p2->depth;
    else 
	if(p2->clipdepth)
	    return 1; // do the clip first
	else
	    return -1;

/*    if(!p1->clipdepth) {
	if(!p2->clipdepth) {
	    // !p1->clipdepth && !p2->clipdepth
	    return (int)p1->depth - (int)p2->depth;
	} else {
	    // !p1->clipdepth && p2->clipdepth
	    if(p1->depth != p2->clipdepth)
		return (int)p1->depth - (int)p2->clipdepth;
	    else
		return 1; // do the clip first
	}
    } else {
	if(!p2->clipdepth) {
	    // p1->clipdepth && !p2->clipdepth
	    if(p1->clipdepth != p2->depth)
		return (int)p1->clipdepth - (int)p2->depth;
	    else
		return -1;// do the clip first
	} else {
	    if(p1->clipdepth != p2->clipdepth)
		return (int)p1->clipdepth - (int)p2->clipdepth;
	    else
		return (int)p1->depth - (int)p2->depth;
	}
    }*/
}

typedef struct textcallbackblock
{
    character_t*idtable;
    U16 depth;
    U16 clipdepth;
    CXFORM* cxform;
    MATRIX m;
    RENDERBUF*buf;
} textcallbackblock_t;

static void textcallback(void*self, int*chars, int*xpos, int nr, int fontid, int fontsize, 
		    int xstart, int ystart, RGBA* color)
{
    textcallbackblock_t * info = (textcallbackblock_t*)self;
    font_t*font = 0;
    int t;
    if(info->idtable[fontid].type != font_type) {
	fprintf(stderr, "ID %d is not a font\n", fontid);
	return;
    } else if(!info->idtable[fontid].obj.font) {
	fprintf(stderr, "Font %d unknown\n", fontid);
	return;
    } else {
	font  = info->idtable[fontid].obj.font;
    }
    for(t=0;t<nr;t++) {
	int x = xstart + xpos[t];
	int y = ystart;
	MATRIX m = info->m;
	SPOINT p;
	
	p.x = x; p.y = y; 
	p = swf_TurnPoint(p, &m);
	
	m.sx = (m.sx * fontsize) / 1024;
	m.sy = (m.sy * fontsize) / 1024;
	m.r0 = (m.r0 * fontsize) / 1024;
	m.r1 = (m.r1 * fontsize) / 1024;
	m.tx = p.x;
	m.ty = p.y;

	if(chars[t]<0 || chars[t]>= font->numchars) {
	    fprintf(stderr, "Character out of range: %d\n", chars[t]);
	} else {
	    SHAPE2*shape = font->glyphs[chars[t]];
	    shape->fillstyles[0].color = *color; //q&d
	    /*printf("Rendering char %d (size %d, x:%d, y:%d) color:%02x%02x%02x%02x\n", chars[t], fontsize, x, y,
		    color->a, color->r, color->g, color->b);
	    swf_DumpMatrix(stdout, &m);
	    swf_DumpShape(shape);*/
	    swf_RenderShape(info->buf, shape, &m, info->cxform, info->depth, info->clipdepth);
	}
    }
}

static void renderFromTag(RENDERBUF*buf, character_t*idtable, TAG*firstTag, MATRIX*m)
{
    TAG*tag = 0;
    int numplacements = 0;
    SWFPLACEOBJECT* placements;

    tag = firstTag;
    numplacements = 0;
    while(tag) {
        if(tag->id == ST_PLACEOBJECT || 
           tag->id == ST_PLACEOBJECT2) {
	    numplacements++;
	}
	if(tag->id == ST_SHOWFRAME || tag->id == ST_END)
	    break;
	tag = tag->next;
    }
    placements = (SWFPLACEOBJECT*)rfx_calloc(sizeof(SWFPLACEOBJECT)*numplacements);
    numplacements = 0;

    tag = firstTag;
    while(tag) {
	if(swf_isPlaceTag(tag)) {
	    SWFPLACEOBJECT p;
	    swf_GetPlaceObject(tag, &p);
	    /* TODO: add move and deletion */
	    placements[numplacements++] = p;
	    swf_PlaceObjectFree(&p); //dirty! but it only frees fields we don't use
	}
	if(tag->id == ST_SHOWFRAME || tag->id == ST_END)
	    break;
        tag = tag->next;
    }

    qsort(placements, numplacements, sizeof(SWFPLACEOBJECT), compare_placements);
     
    int t;
    for(t=0;t<numplacements;t++) {
        SWFPLACEOBJECT*p = &placements[t];
        int id = p->id;
	MATRIX m2;
	swf_MatrixJoin(&m2, m, &p->matrix);
            
        if(!idtable[id].tag) { 
            fprintf(stderr, "rfxswf: Id %d is unknown\n", id);
            continue;
        }

        if(idtable[id].type == shape_type) {
            //SRECT sbbox = swf_TurnRect(*idtable[id].bbox, &p->matrix);
            swf_RenderShape(buf, idtable[id].obj.shape, &m2, &p->cxform, p->depth, p->clipdepth);
	} else if(idtable[id].type == sprite_type) {
	    swf_UnFoldSprite(idtable[id].tag);
	    renderFromTag(buf, idtable, idtable[id].tag->next, &m2);
	    swf_FoldSprite(idtable[id].tag);
        } else if(idtable[id].type == text_type) {
	    TAG* tag = idtable[id].tag;
	    textcallbackblock_t info;
	    MATRIX mt;

	    swf_SetTagPos(tag, 0);
	    swf_GetU16(tag);
	    swf_GetRect(tag,0);
  	    swf_GetMatrix(tag,&mt);
	    swf_MatrixJoin(&info.m, &m2, &mt);
	    /*printf("Text matrix:\n");
	    swf_DumpMatrix(stdout, &m);
	    printf("Placement matrix:\n");
	    swf_DumpMatrix(stdout, &p->matrix);
	    printf("Final matrix:\n");
	    swf_DumpMatrix(stdout, &info.m);*/

	    info.idtable = idtable;
	    info.depth = p->depth;
	    info.cxform = &p->cxform;
	    info.clipdepth = p->clipdepth;
	    info.buf = buf;
	    
	    swf_ParseDefineText(tag, textcallback, &info);
        } else if(idtable[id].type == edittext_type) {
	    TAG* tag = idtable[id].tag;
            U16 flags = swf_GetBits(tag, 16);
	    if(flags & ET_HASTEXT) {
                fprintf(stderr, "edittext not supported yet (id %d)\n", id);
            }
        } else {
            fprintf(stderr, "Unknown/Unsupported Object Type for id %d: %s\n", id, swf_TagGetName(idtable[id].tag));
        }
    }

    free(placements);
}

void swf_RenderSWF(RENDERBUF*buf, SWF*swf)
{
    TAG*tag;
    int t;
    RGBA color;

    swf_OptimizeTagOrder(swf);
    swf_FoldAll(swf);
    
    character_t* idtable = (character_t*)rfx_calloc(sizeof(character_t)*65536);            // id to character mapping
    
    /* set background color */
    color = swf_GetSWFBackgroundColor(swf);
    swf_Render_SetBackgroundColor(buf, color);

    /* parse definitions */
    tag = swf->firstTag;
    while(tag) {
        if(swf_isDefiningTag(tag)) {
            int id = swf_GetDefineID(tag);
            idtable[id].tag = tag;
            idtable[id].bbox = (SRECT*)rfx_alloc(sizeof(SRECT));
            *idtable[id].bbox = swf_GetDefineBBox(tag);

            if(swf_isShapeTag(tag)) {
                SHAPE2* shape = (SHAPE2*)rfx_calloc(sizeof(SHAPE2));
                swf_ParseDefineShape(tag, shape);
                idtable[id].type = shape_type;
                idtable[id].obj.shape = shape;
            } else if(swf_isImageTag(tag)) {
		int width,height;
                RGBA*data = swf_ExtractImage(tag, &width, &height);
                idtable[id].type = image_type;
                swf_Render_AddImage(buf, id, data, width, height);
		free(data);
            } else if(tag->id == ST_DEFINEFONT ||
                      tag->id == ST_DEFINEFONT2) {
		int t;
		SWFFONT*swffont;
		font_t*font = (font_t*)rfx_calloc(sizeof(font_t));
		idtable[id].obj.font = font;
                swf_FontExtract(swf,id,&swffont);
		font->numchars = swffont->numchars;
		font->glyphs = (SHAPE2**)rfx_calloc(sizeof(SHAPE2*)*font->numchars);
		for(t=0;t<font->numchars;t++) {
		    if(!swffont->glyph[t].shape->fillstyle.n) {
			/* the actual fill color will be overwritten while rendering */
			swf_ShapeAddSolidFillStyle(swffont->glyph[t].shape, &color_white);
		    }
		    font->glyphs[t] = swf_ShapeToShape2(swffont->glyph[t].shape);
		}
		swf_FontFree(swffont);
                idtable[id].type = font_type;

            } else if(tag->id == ST_DEFINEFONTINFO ||
                      tag->id == ST_DEFINEFONTINFO2) {
                idtable[id].type = font_type;
            } else if(tag->id == ST_DEFINETEXT ||
                      tag->id == ST_DEFINETEXT2) {
                idtable[id].type = text_type;
            } else if(tag->id == ST_DEFINESPRITE) {
		idtable[id].type = sprite_type;
            } else if(tag->id == ST_DEFINEEDITTEXT) {
		idtable[id].type = edittext_type;
	    }
        }
	tag = tag->next;
    }
    MATRIX m;
    swf_GetMatrix(0, &m);
    renderFromTag(buf, idtable, swf->firstTag, &m);
    
    /* free id and depth tables again */
    for(t=0;t<65536;t++) {
        if(idtable[t].bbox) {
            free(idtable[t].bbox);
            idtable[t].bbox=0;
        }
        if(idtable[t].type == shape_type) {
            SHAPE2* shape = idtable[t].obj.shape;
            if(shape) {
                swf_Shape2Free(shape); // FIXME
                free(idtable[t].obj.shape);idtable[t].obj.shape = 0;
            }
        } else if(idtable[t].type == font_type) {
	    font_t* font = idtable[t].obj.font;
	    if(font) {
		if(font->glyphs) {
		    int t;
		    for(t=0;t<font->numchars;t++) {
			swf_Shape2Free(font->glyphs[t]);
			free(font->glyphs[t]); font->glyphs[t] = 0;
		    }
		    free(font->glyphs);
		    font->glyphs = 0;
		}
		free(idtable[t].obj.font); idtable[t].obj.font = 0;
		font = 0;
	    }
	}
    }
    free(idtable);
}
    
