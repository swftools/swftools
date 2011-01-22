/* render.c

   Part of the swftools package.

   Copyright (c) 2005/2006/2007 Matthias Kramm <kramm@quiss.org> 
 
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../mem.h"
#include "../types.h"
#include "../png.h"
#include "../log.h"
#include "render.h"

typedef gfxcolor_t RGBA;

typedef struct _renderpoint
{
    float x;
} renderpoint_t;

typedef struct _renderline
{
    renderpoint_t*points;
    int size;
    int num;
} renderline_t;

typedef struct _internal_result {
    gfximage_t img;
    struct _internal_result*next;
    char palette;
} internal_result_t;

typedef struct _clipbuffer {
    U32*data;
    struct _clipbuffer*next;
} clipbuffer_t;

typedef struct _internal {
    int width;
    int height;
    int width2;
    int height2;
    int bitwidth;
    int multiply;
    int antialize;
    int zoom;
    int ymin, ymax;
    int fillwhite;

    char palette;

    RGBA* img;

    clipbuffer_t*clipbuf;

    renderline_t*lines;

    internal_result_t*results;
    internal_result_t*result_next;
} internal_t;

typedef enum {filltype_solid,filltype_clip,filltype_bitmap,filltype_gradient} filltype_t;

typedef struct _fillinfo {
    filltype_t type;
    gfxcolor_t*color;
    gfximage_t*image;
    gfxmatrix_t*matrix;
    gfxcxform_t*cxform;
    RGBA*gradient;
    char linear_or_radial;
} fillinfo_t;


static inline void add_pixel(internal_t*i, float x, int y)
{
    renderpoint_t p;

    if(x >= i->width2 || y >= i->height2 || y<0) return;
    p.x = x;
    if(y<i->ymin) i->ymin = y;
    if(y>i->ymax) i->ymax = y;

    renderline_t*l = &i->lines[y];

    if(l->num == l->size) {
	l->size += 32;
	l->points = (renderpoint_t*)rfx_realloc(l->points, l->size * sizeof(renderpoint_t));
    }
    l->points[l->num] = p;
    l->num++;
}

/* set this to 0.777777 or something if the "both fillstyles set while not inside shape"
   problem appears to often */
#define CUT 0.5

#define INT(x) ((int)((x)+16)-16)

static void add_line(gfxdevice_t*dev , double x1, double y1, double x2, double y2)
{
    internal_t*i = (internal_t*)dev->internal;
    double diffx, diffy;
    double ny1, ny2, stepx;
/*    if(DEBUG&4) {
        int l = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
        printf(" l[%d - %.2f/%.2f -> %.2f/%.2f]\n", l, x1/20.0, y1/20.0, x2/20.0, y2/20.0);
    }*/

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
	    add_pixel(i, xx ,posy);
	    posx+=stepx;
	    posy++;
	}
    }
}
#define PI 3.14159265358979
static void add_solidline(gfxdevice_t*dev, double x1, double y1, double x2, double y2, double width)
{
    /* TODO: handle cap styles */

    internal_t*i = (internal_t*)dev->internal;

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
    width += 1.0;
#else
    /* That's what Macromedia's Player seems to do at zoom level 0.  */
    /* TODO: needs testing */

    /* TODO: how does this interact with scaling? */
    if(width * i->multiply < 1.0)
	width = 1.0 / i->multiply;
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

    segments = width/2;
    if(segments < 2)
        segments = 2;

    segments = 8;

    vx=vx*width*0.5;
    vy=vy*width*0.5;

    xx = x2+vx;
    yy = y2+vy;
    add_line(dev, x1+vx, y1+vy, xx, yy);
    lastx = xx;
    lasty = yy;
    for(t=1;t<segments;t++) {
        double s = sin(t*PI/segments);
        double c = cos(t*PI/segments);
        xx = (x2 + vx*c - vy*s);
        yy = (y2 + vx*s + vy*c);
        add_line(dev, lastx, lasty, xx, yy);
        lastx = xx;
        lasty = yy;
    }
    
    xx = (x2-vx);
    yy = (y2-vy);
    add_line(dev, lastx, lasty, xx, yy);
    lastx = xx;
    lasty = yy;
    xx = (x1-vx);
    yy = (y1-vy);
    add_line(dev, lastx, lasty, xx, yy);
    lastx = xx;
    lasty = yy;
    for(t=1;t<segments;t++) {
        double s = sin(t*PI/segments);
        double c = cos(t*PI/segments);
        xx = (x1 - vx*c + vy*s);
        yy = (y1 - vx*s - vy*c);
        add_line(dev, lastx, lasty, xx, yy);
        lastx = xx;
        lasty = yy;
    }
    add_line(dev, lastx, lasty, (x1+vx), (y1+vy));
}

static int compare_renderpoints(const void * _a, const void * _b)
{
    renderpoint_t*a = (renderpoint_t*)_a;
    renderpoint_t*b = (renderpoint_t*)_b;
    if(a->x < b->x) return -1;
    if(a->x > b->x) return 1;
    return 0;
}

static void fill_line_solid(RGBA*line, U32*z, int y, int x1, int x2, RGBA col)
{
    int x = x1;

    U32 bit = 1<<(x1&31);
    int bitpos = (x1/32);

    if(col.a!=255) {
        int ainv = 255-col.a;
        col.r = (col.r*col.a)/255;
        col.g = (col.g*col.a)/255;
        col.b = (col.b*col.a)/255;
        do {
	    if(z[bitpos]&bit) {
		line[x].r = ((line[x].r*ainv)/255)+col.r;
		line[x].g = ((line[x].g*ainv)/255)+col.g;
		line[x].b = ((line[x].b*ainv)/255)+col.b;
		//line[x].a = 255;
		line[x].a = ((line[x].a*ainv)/255)+col.a;
	    }
	    bit <<= 1;
	    if(!bit) {
		bit = 1;bitpos++;
	    }
        } while(++x<x2);
    } else {
        do {
	    if(z[bitpos]&bit) {
		line[x] = col;
	    }
	    bit <<= 1;
	    if(!bit) {
		bit = 1;bitpos++;
	    }
        } while(++x<x2);
    }
}

static void fill_line_bitmap(RGBA*line, U32*z, int y, int x1, int x2, fillinfo_t*info)
{
    int x = x1;

    gfxmatrix_t*m = info->matrix;
    gfximage_t*b = info->image;
    
    if(!b || !b->width || !b->height) {
	gfxcolor_t red = {255,255,0,0};
        fill_line_solid(line, z, y, x1, x2, red);
        return;
    }
    
    double det = m->m00*m->m11 - m->m01*m->m10;
    if(fabs(det) < 0.0005) { 
	/* x direction equals y direction- the image is invisible */
	return;
    }
    det = 1.0/det;
    double xx1 =  (  (-m->tx) * m->m11 - (y - m->ty) * m->m10) * det;
    double yy1 =  (- (-m->tx) * m->m01 + (y - m->ty) * m->m00) * det;
    double xinc1 = m->m11 * det;
    double yinc1 = m->m01 * det;
    
    U32 bit = 1<<(x1&31);
    int bitpos = (x1/32);

    do {
	if(z[bitpos]&bit) {
	    RGBA col;
	    int xx = (int)(xx1 + x * xinc1);
	    int yy = (int)(yy1 - x * yinc1);
	    int ainv;

	    if(info->linear_or_radial) {
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

	    /* needs bitmap with premultiplied alpha */
	    line[x].r = ((line[x].r*ainv)/255)+col.r;
	    line[x].g = ((line[x].g*ainv)/255)+col.g;
	    line[x].b = ((line[x].b*ainv)/255)+col.b;
	    line[x].a = 255;
	}
	bit <<= 1;
	if(!bit) {
	    bit = 1;bitpos++;
	}
    } while(++x<x2);
}

static void fill_line_gradient(RGBA*line, U32*z, int y, int x1, int x2, fillinfo_t*info)
{
    int x = x1;

    gfxmatrix_t*m = info->matrix;
    RGBA*g= info->gradient;
    
    double det = m->m00*m->m11 - m->m01*m->m10;
    if(fabs(det) < 0.0005) { 
	/* x direction equals y direction */
	return;
    }
    
    det = 1.0/det;
    double xx1 =  (  (-m->tx) * m->m11 - (y - m->ty) * m->m10) * det;
    double yy1 =  (- (-m->tx) * m->m01 + (y - m->ty) * m->m00) * det;
    double xinc1 = m->m11 * det;
    double yinc1 = m->m01 * det;
    
    U32 bit = 1<<(x1&31);
    int bitpos = (x1/32);

    do {
	if(z[bitpos]&bit) {
	    RGBA col;
	    int ainv;

            int pos = 0;
            if(info->linear_or_radial) {
                double xx = xx1 + x * xinc1;
                double yy = yy1 + y * yinc1;
                double r = sqrt(xx*xx + yy*yy);
                if(r>1) r = 1;
                pos = (int)(r*255.999);
            } else {
                double r = xx1 + x * xinc1;
                if(r>1) r = 1;
                if(r<-1) r = -1;
                pos = (int)((r+1)*127.999);
            }
	    col = g[pos];
	    ainv = 255-col.a;

	    /* needs bitmap with premultiplied alpha */
	    line[x].r = ((line[x].r*ainv)/255)+col.r;
	    line[x].g = ((line[x].g*ainv)/255)+col.g;
	    line[x].b = ((line[x].b*ainv)/255)+col.b;
	    line[x].a = 255;
	}
	bit <<= 1;
	if(!bit) {
	    bit = 1;bitpos++;
	}
    } while(++x<x2);
}

static void fill_line_clip(RGBA*line, U32*z, int y, int x1, int x2)
{
    int x = x1;

    U32 bit = 1<<(x1&31);
    int bitpos = (x1/32);

    do {
	z[bitpos]|=bit;
	bit <<= 1;
	if(!bit) {
	    bit = 1;bitpos++;
	}
    } while(++x<x2);
}

void fill_line(gfxdevice_t*dev, RGBA*line, U32*zline, int y, int startx, int endx, fillinfo_t*fill)
{
    if(fill->type == filltype_solid)
	fill_line_solid(line, zline, y, startx, endx, *fill->color);
    else if(fill->type == filltype_clip)
	fill_line_clip(line, zline, y, startx, endx);
    else if(fill->type == filltype_bitmap)
	fill_line_bitmap(line, zline, y, startx, endx, fill);
    else if(fill->type == filltype_gradient)
	fill_line_gradient(line, zline, y, startx, endx, fill);
}

void fill(gfxdevice_t*dev, fillinfo_t*fill)
{
    internal_t*i = (internal_t*)dev->internal;
    int y;
    U32 clipdepth = 0;
    for(y=i->ymin;y<=i->ymax;y++) {
	renderpoint_t*points = i->lines[y].points;
        RGBA*line = &i->img[i->width2*y];
        U32*zline = &i->clipbuf->data[i->bitwidth*y];

	int n;
	int num = i->lines[y].num;
	int lastx;
        qsort(points, num, sizeof(renderpoint_t), compare_renderpoints);

        for(n=0;n<num;n++) {
            renderpoint_t*p = &points[n];
            renderpoint_t*next= n<num-1?&points[n+1]:0;
            int startx = p->x;
            int endx = next?next->x:i->width2;
            if(endx > i->width2)
                endx = i->width2;
            if(startx < 0)
                startx = 0;
            if(endx < 0)
                endx = 0;

	    if(!(n&1))
		fill_line(dev, line, zline, y, startx, endx, fill);

	    lastx = endx;
            if(endx == i->width2)
                break;
        }
	if(fill->type == filltype_clip) {
	    if(i->clipbuf->next) {
		U32*line2 = &i->clipbuf->next->data[i->bitwidth*y];
		int x;
		for(x=0;x<i->bitwidth;x++)
		    zline[x] &= line2[x];
	    }
	}

	i->lines[y].num = 0;
    }
}

void fill_solid(gfxdevice_t*dev, gfxcolor_t* color)
{
    fillinfo_t info;
    memset(&info, 0, sizeof(info));
    info.type = filltype_solid;
    info.color = color;
    fill(dev, &info);
}

int render_setparameter(struct _gfxdevice*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!strcmp(key, "antialize") || !strcmp(key, "antialise")) {
	i->antialize = atoi(value);
	i->zoom = i->antialize * i->multiply;
	return 1;
    } else if(!strcmp(key, "multiply")) {
	i->multiply = atoi(value);
	i->zoom = i->antialize * i->multiply;
	fprintf(stderr, "Warning: multiply not implemented yet\n");
	return 1;
    } else if(!strcmp(key, "fillwhite")) {
	i->fillwhite = atoi(value);
	return 1;
    } else if(!strcmp(key, "palette")) {
	i->palette = atoi(value);
	return 1;
    }
    return 0;
}

void newclip(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    
    clipbuffer_t*c = (clipbuffer_t*)rfx_calloc(sizeof(clipbuffer_t));
    c->data = (U32*)rfx_calloc(sizeof(U32) * i->bitwidth * i->height2);
    c->next = i->clipbuf;
    i->clipbuf = c;
    memset(c->data, 0, sizeof(U32)*i->bitwidth*i->height2);
}

void endclip(struct _gfxdevice*dev, char removelast)
{
    internal_t*i = (internal_t*)dev->internal;
   
    /* test for at least one cliplevel (the one we created ourselves) */
    if(!i->clipbuf || (!i->clipbuf->next && !removelast)) {
	fprintf(stderr, "endclip without any active clip buffers\n");
	return;
    }

    clipbuffer_t*c = i->clipbuf;
    i->clipbuf = i->clipbuf->next;
    c->next = 0;
    free(c->data);c->data = 0;
    free(c);
}

void render_stroke(struct _gfxdevice*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    internal_t*i = (internal_t*)dev->internal;
    double x,y;
    
    /*if(cap_style != gfx_capRound || joint_style != gfx_joinRound) {
	fprintf(stderr, "Warning: cap/joint style != round not yet supported\n");
    }*/

    while(line) {
        if(line->type == gfx_moveTo) {
        } else if(line->type == gfx_lineTo) {
	    double x1=x*i->zoom,y1=y*i->zoom;
	    double x3=line->x*i->zoom,y3=line->y*i->zoom;
	    add_solidline(dev, x1, y1, x3, y3, width * i->zoom);
	    fill_solid(dev, color);
        } else if(line->type == gfx_splineTo) {
	    int t,parts;
	    double xx,yy;
           
	    double x1=x*i->zoom,y1=y*i->zoom;
	    double x2=line->sx*i->zoom,y2=line->sy*i->zoom;
	    double x3=line->x*i->zoom,y3=line->y*i->zoom;
            
            double c = abs(x3-2*x2+x1) + abs(y3-2*y2+y1);
            xx=x1;
	    yy=y1;

            parts = (int)(sqrt(c)/3);
            if(!parts) parts = 1;

            for(t=1;t<=parts;t++) {
                double nx = (double)(t*t*x3 + 2*t*(parts-t)*x2 + (parts-t)*(parts-t)*x1)/(double)(parts*parts);
                double ny = (double)(t*t*y3 + 2*t*(parts-t)*y2 + (parts-t)*(parts-t)*y1)/(double)(parts*parts);
                
		add_solidline(dev, xx, yy, nx, ny, width * i->zoom);
		fill_solid(dev, color);
                xx = nx;
                yy = ny;
            }
        }
        x = line->x;
        y = line->y;
        line = line->next;
    }
}

static void draw_line(gfxdevice_t*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    double x=0,y=0;

    while(line)
    {
        int x1,y1,x2,y2,x3,y3;

        if(line->type == gfx_moveTo) {
        } else if(line->type == gfx_lineTo) {
	    double x1=x*i->zoom,y1=y*i->zoom;
	    double x3=line->x*i->zoom,y3=line->y*i->zoom;
            
            add_line(dev, x1, y1, x3, y3);
        } else if(line->type == gfx_splineTo) {
	    int c,t,parts,qparts;
	    double xx,yy;
            
	    double x1=x*i->zoom,y1=y*i->zoom;
	    double x2=line->sx*i->zoom,y2=line->sy*i->zoom;
	    double x3=line->x*i->zoom,y3=line->y*i->zoom;
            
            c = abs(x3-2*x2+x1) + abs(y3-2*y2+y1);
            xx=x1;
	    yy=y1;

            parts = (int)(sqrt(c));
            if(!parts) parts = 1;

            for(t=1;t<=parts;t++) {
                double nx = (double)(t*t*x3 + 2*t*(parts-t)*x2 + (parts-t)*(parts-t)*x1)/(double)(parts*parts);
                double ny = (double)(t*t*y3 + 2*t*(parts-t)*y2 + (parts-t)*(parts-t)*y1)/(double)(parts*parts);
                
                add_line(dev, xx, yy, nx, ny);
                xx = nx;
                yy = ny;
            }
        }
        x = line->x;
        y = line->y;
        line = line->next;
    }
}

void render_startclip(struct _gfxdevice*dev, gfxline_t*line)
{
    internal_t*i = (internal_t*)dev->internal;
    fillinfo_t info;
    memset(&info, 0, sizeof(info));
    newclip(dev);
    info.type = filltype_clip;
    draw_line(dev, line);
    fill(dev, &info);
}

void render_endclip(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    endclip(dev, 0);
}

void render_fill(struct _gfxdevice*dev, gfxline_t*line, gfxcolor_t*color)
{
    internal_t*i = (internal_t*)dev->internal;

    draw_line(dev, line);
    fill_solid(dev, color);
}

void render_fillbitmap(struct _gfxdevice*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    internal_t*i = (internal_t*)dev->internal;

    gfxmatrix_t m2 = *matrix;

    draw_line(dev, line);

    fillinfo_t info;
    memset(&info, 0, sizeof(info));
    info.type = filltype_bitmap;
    info.image = img;
    info.matrix = &m2;
    info.cxform = cxform;

    m2.m00 *= i->zoom; m2.m01 *= i->zoom; m2.tx *= i->zoom;
    m2.m10 *= i->zoom; m2.m11 *= i->zoom; m2.ty *= i->zoom;

    fill(dev, &info);
}

void render_fillgradient(struct _gfxdevice*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    
    gfxmatrix_t m2 = *matrix;

    draw_line(dev, line);

    RGBA g[256];
    fillinfo_t info;
    memset(&info, 0, sizeof(info));
    info.type = filltype_gradient;
    info.gradient = g;
    info.matrix = &m2;

    m2.m00 *= i->zoom; m2.m01 *= i->zoom; m2.tx *= i->zoom;
    m2.m10 *= i->zoom; m2.m11 *= i->zoom; m2.ty *= i->zoom;

    info.linear_or_radial = type == gfxgradient_radial;

    int pos = 0;
    gfxcolor_t color = {0,0,0,0};
    pos=0;
    while(gradient) {
        int nextpos = gradient->pos*256;
        int t;
        if(nextpos>256) {
            msg("<error> Invalid gradient- contains values > 1.0");
            return;
        }
        
        gfxcolor_t nextcolor = gradient->color;
        if(nextpos!=pos) {
            double p0 = 1.0;
            double p1 = 0.0;
            double step = 1.0/(nextpos-pos);
            int t;
            for(t=pos;t<nextpos;t++) {
                g[t].r = color.r*p0 + nextcolor.r*p1;
                g[t].g = color.g*p0 + nextcolor.g*p1;
                g[t].b = color.b*p0 + nextcolor.b*p1;
                g[t].a = color.a*p0 + nextcolor.a*p1;
                p0 -= step;
                p1 += step;
            }
        }
        color=nextcolor;

        pos = nextpos;
        gradient = gradient->next;
    }
    if(pos!=256) {
        msg("<error> Invalid gradient- doesn't end with 1.0");
    }

    fill(dev, &info);
}

void render_addfont(struct _gfxdevice*dev, gfxfont_t*font)
{
}

void render_drawchar(struct _gfxdevice*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!font)
	return;

    /* align characters to whole pixels */
    matrix->tx = (int)(matrix->tx * i->antialize) / i->antialize;
    matrix->ty = (int)(matrix->ty * i->antialize) / i->antialize;

    gfxglyph_t*glyph = &font->glyphs[glyphnr];
    gfxline_t*line2 = gfxline_clone(glyph->line);
    gfxline_transform(line2, matrix);
    draw_line(dev, line2);
    fill_solid(dev, color);
    gfxline_free(line2);
    
    return;
}

void render_result_write(gfxresult_t*r, int filedesc)
{
    internal_result_t*i= (internal_result_t*)r->internal;
}
int render_result_save(gfxresult_t*r, const char*filename)
{
    internal_result_t*i= (internal_result_t*)r->internal;
    if(!i) {
	return 0; // no pages drawn
    }
    if(i->next) {
	int nr=0;
	char filenamebuf[256];
	char*origname = strdup(filename);
	int l = strlen(origname);
	if(l>3 && strchr("gG",origname[l-1]) && strchr("nN",filename[l-2]) &&
		strchr("pP",origname[l-3]) && filename[l-4]=='.') {
	    origname[l-4] = 0;
	}
	while(i->next) {
	    sprintf(filenamebuf, "%s.%d.png", origname, nr);
            if(!i->palette) {
	        png_write(filename, (unsigned char*)i->img.data, i->img.width, i->img.height);
            } else {
	        png_write_palette_based_2(filename, (unsigned char*)i->img.data, i->img.width, i->img.height);
            }
	    nr++;
	}
	free(origname);
    } else {
        if(!i->palette) {
	    png_write(filename, (unsigned char*)i->img.data, i->img.width, i->img.height);
	} else {
	    png_write_palette_based_2(filename, (unsigned char*)i->img.data, i->img.width, i->img.height);
	}
    }
    return 1;
}
char*gfximage_asXPM(gfximage_t*img, int depth)
{
    int d= 256/depth;
    char*str = (char*)malloc(img->width*img->height*4 + 500 + 16*depth*depth*depth);
    char*p = str;
    p+= sprintf(p, "static char *noname[] = {\n\"%d %d 262144 3\",\n", img->width, img->height);
    int r,g,b;
    for(r=0;r<depth;r++)
    for(g=0;g<depth;g++)
    for(b=0;b<depth;b++) {
	p += sprintf(p, "\"%c%c%c c #%02x%02x%02x\",\n", r+32,g+32,b+32, r*d,g*d,b*d);
    }
    int y;
    for(y=0;y<img->height;y++)  {
	p+=sprintf(p, "\"");
	gfxcolor_t*col = &img->data[y*img->height];
	int x;
	for(x=0;x<img->width;x++) {
	    p+=sprintf(p, "%c%c%c", 32+(col->r/d), 32+(col->g/d), 32+(col->b/d));
	}
	p+=sprintf(p, "\",\n");
    }
    *p = 0;
    return p;
}
void*render_result_get(gfxresult_t*r, const char*name)
{
    internal_result_t*i= (internal_result_t*)r->internal;
    if(!strncmp(name,"xpm",3)) {
	int pagenr = atoi(&name[3]);
	if(pagenr<0)
	    pagenr=0;
	while(pagenr>0) {
	    i = i->next;
	    if(!i)
		return 0;
            pagenr--;
	}
	return gfximage_asXPM(&i->img, 64);
    } else if(!strncmp(name,"page",4)) {
	int pagenr = atoi(&name[4]);
	if(pagenr<0)
	    pagenr=0;
	while(pagenr>0) {
	    i = i->next;
	    if(!i)
		return 0;
            pagenr--;
	}
	return &i->img;
    }
    return 0;
}
void render_result_destroy(gfxresult_t*r)
{
    internal_result_t*i= (internal_result_t*)r->internal;
    r->internal = 0;
    while(i) {
	internal_result_t*next = i->next;
	free(i->img.data);i->img.data = 0;

        /* FIXME memleak
           the following rfx_free causes a segfault on WIN32 machines,
           if executed */
        //rfx_free(i);

	i = next;
    }
    rfx_free(r);
}

gfxresult_t* render_finish(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    
    gfxresult_t* res = (gfxresult_t*)rfx_calloc(sizeof(gfxresult_t));
    
    res->internal = i->results;i->results = 0;
    res->write = render_result_write;
    res->save = render_result_save;
    res->get = render_result_get;
    res->destroy = render_result_destroy;

    free(dev->internal); dev->internal = 0; i = 0;

    return res;
}

void render_startpage(struct _gfxdevice*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    int y;

    if(i->width2 || i->height2) {
	fprintf(stderr, "Error: startpage() called twice (no endpage()?)\n");
	exit(1);
    }
    
    i->width = width*i->multiply;
    i->height = height*i->multiply;
    i->width2 = width*i->zoom;
    i->height2 = height*i->zoom;
    i->bitwidth = (i->width2+31)/32;

    i->lines = (renderline_t*)rfx_alloc(i->height2*sizeof(renderline_t));
    for(y=0;y<i->height2;y++) {
	memset(&i->lines[y], 0, sizeof(renderline_t));
        i->lines[y].points = 0;
        i->lines[y].num = 0;
    }
    i->img = (RGBA*)rfx_calloc(sizeof(RGBA)*i->width2*i->height2);
    if(i->fillwhite) {
	memset(i->img, 0xff, sizeof(RGBA)*i->width2*i->height2);
    }

    i->ymin = 0x7fffffff;
    i->ymax = -0x80000000;


    /* initialize initial clipping field, which doesn't clip anything yet */
    newclip(dev);
    memset(i->clipbuf->data, 255, sizeof(U32)*i->bitwidth*i->height2);
}

static void store_image(internal_t*i, internal_result_t*ir)
{
    ir->img.data = (gfxcolor_t*)malloc(i->width*i->height*sizeof(gfxcolor_t));
    ir->img.width = i->width;
    ir->img.height = i->height;

    gfxcolor_t*dest = ir->img.data;

    if(i->antialize <= 1) /* no antializing */ {
	int y;
	for(y=0;y<i->height;y++) {
	    RGBA*line = &i->img[y*i->width];
	    memcpy(&dest[y*i->width], line, sizeof(RGBA)*i->width);
	}
    } else {
	RGBA**lines = (RGBA**)rfx_calloc(sizeof(RGBA*)*i->antialize);
	int q = i->antialize*i->antialize;
	int ypos = 0;
	int y;
	int y2=0;
	for(y=0;y<i->height2;y++) {
	    int n;
	    ypos = y % i->antialize;
	    lines[ypos] = &i->img[y*i->width2];
	    if(ypos == i->antialize-1) {
		RGBA*out = &dest[(y2++)*i->width];
		int x;
		int r,g,b,a;
		for(x=0;x<i->width;x++) {
		    int xpos = x*i->antialize;
		    int yp;
		    U32 r=0,g=0,b=0,a=0;
		    for(yp=0;yp<i->antialize;yp++) {
			RGBA*lp = &lines[yp][xpos];
			int xp;
			for(xp=0;xp<i->antialize;xp++) {
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
}

void render_endpage(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    
    if(!i->width2 || !i->height2) {
	fprintf(stderr, "Error: endpage() called without corresponding startpage()\n");
	exit(1);
    }

    endclip(dev, 1);
    int unclosed = 0;
    while(i->clipbuf) {
	endclip(dev, 1);
        unclosed++;
    }

    if(unclosed) {
        fprintf(stderr, "Warning: %d unclosed clip(s) while processing endpage()\n", unclosed);
    }
    
    internal_result_t*ir= (internal_result_t*)rfx_calloc(sizeof(internal_result_t));
    ir->palette = i->palette;

    int y,x;

    store_image(i, ir);

    ir->next = 0;
    if(i->result_next) {
	i->result_next->next = ir;
    }
    if(!i->results) {
	i->results = ir;
    }
    i->result_next = ir;

    for(y=0;y<i->height2;y++) {
	rfx_free(i->lines[y].points); i->lines[y].points = 0;
    }
    rfx_free(i->lines);i->lines=0;

    if(i->img) {rfx_free(i->img);i->img = 0;}

    i->width2 = 0;
    i->height2 = 0;
}

void render_drawlink(struct _gfxdevice*dev, gfxline_t*line, const char*action, const char*text)
{
    /* not supported for this output device */
}

void gfxdevice_render_init(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));
    
    dev->name = "render";

    dev->internal = i;
    
    i->width = 0;
    i->width2 = 0;
    i->height = 0;
    i->height2 = 0;
    i->antialize = 1;
    i->multiply = 1;
    i->zoom = 1;

    dev->setparameter = render_setparameter;
    dev->startpage = render_startpage;
    dev->startclip = render_startclip;
    dev->endclip = render_endclip;
    dev->stroke = render_stroke;
    dev->fill = render_fill;
    dev->fillbitmap = render_fillbitmap;
    dev->fillgradient = render_fillgradient;
    dev->addfont = render_addfont;
    dev->drawchar = render_drawchar;
    dev->drawlink = render_drawlink;
    dev->endpage = render_endpage;
    dev->finish = render_finish;
}


gfxdevice_t* gfxdevice_render_new()
{
    gfxdevice_t* d = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    gfxdevice_render_init(d);
    return d;
}
