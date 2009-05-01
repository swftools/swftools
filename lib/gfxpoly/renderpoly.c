#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "renderpoly.h"

typedef struct _renderpoint
{
    double x;
    segment_dir_t dir;
    fillstyle_t*fs;
    int polygon_nr;
} renderpoint_t;

typedef struct _renderline
{
    renderpoint_t*points;
    int size;
    int num;
} renderline_t;

typedef struct _renderbuf
{
    intbbox_t bbox;
    int width;
    int height;
    double zoom;
    renderline_t*lines;
} renderbuf_t;

static inline void add_pixel(renderbuf_t*buf, double x, int y, segment_dir_t dir, fillstyle_t*fs, int polygon_nr)
{
    renderpoint_t p;
    p.x = x;
    p.dir = dir;
    p.fs = fs;
    p.polygon_nr = polygon_nr;

    if(x >= buf->bbox.xmax || y >= buf->bbox.ymax || y < buf->bbox.ymin) 
        return;
    renderline_t*l = &buf->lines[y-buf->bbox.ymin];

    if(l->num == l->size) {
	l->size += 32;
	l->points = (renderpoint_t*)rfx_realloc(l->points, l->size * sizeof(renderpoint_t));
    }
    l->points[l->num] = p;
    l->num++;
}
#define CUT 0.5
#define INT(x) ((int)((x)+16)-16)
static void add_line(renderbuf_t*buf, double x1, double y1, double x2, double y2, fillstyle_t*fs, int polygon_nr)
{
    x1 *= buf->zoom;
    y1 *= buf->zoom;
    x2 *= buf->zoom;
    y2 *= buf->zoom;
    double diffx, diffy;
    double ny1, ny2, stepx;
    segment_dir_t dir = DIR_DOWN;
    if(y2 < y1) {
        dir = DIR_UP;
        double x,y;
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

    int posy=INT(ny1);
    int endy=INT(ny2);
    double posx=0;
    double startx = x1;

    while(posy<=endy) {
        double xx = startx + posx;
        add_pixel(buf, xx, posy, dir, fs, polygon_nr);
        posx+=stepx;
        posy++;
    }
}

static int compare_renderpoints(const void * _a, const void * _b)
{
    renderpoint_t*a = (renderpoint_t*)_a;
    renderpoint_t*b = (renderpoint_t*)_b;
    if(a->x < b->x) return -1;
    if(a->x > b->x) return 1;
    return 0;
}

static void fill_bitwise(unsigned char*line, int x1, int x2)
{
    int p1 = x1>>3;
    int p2 = x2>>3;
    int b1 = 0xff >> (x1&7);
    int b2 = 0xff << (1+7-(x2&7));
    if(p1==p2) {
        line[p1] |= b1&b2;
    } else {
        line[p1] |= b1;
        memset(&line[p1+1], 255, p2-(p1+1));
        line[p2] = b2;
    }
}

unsigned char* render_polygon(gfxpoly_t*polygon, intbbox_t*bbox, double zoom, windrule_t*rule)
{
    renderbuf_t _buf, *buf=&_buf;
    buf->width = (bbox->xmax - bbox->xmin);
    buf->height = (bbox->ymax - bbox->ymin);
    buf->bbox = *bbox;
    buf->zoom = zoom * polygon->gridsize;
    int width8 = (buf->width+7) >> 3;
    unsigned char* image = (unsigned char*)malloc(width8*buf->height);
    memset(image, 0, width8*buf->height);
    
    buf->lines = (renderline_t*)rfx_alloc(buf->height*sizeof(renderline_t));
    int y;
    for(y=0;y<buf->height;y++) {
	memset(&buf->lines[y], 0, sizeof(renderline_t));
        buf->lines[y].points = 0;
        buf->lines[y].num = 0;
    }

    edge_t*e;
    int polygon_nr = 0;
    for(e=polygon->edges;e;e=e->next) {
        add_line(buf, e->a.x, e->a.y, e->b.x, e->b.y, e->style, polygon_nr);
    }

    for(y=0;y<buf->height;y++) {
	renderpoint_t*points = buf->lines[y].points;
        unsigned char*line = &image[width8*y];
	int n;
	int num = buf->lines[y].num;
        qsort(points, num, sizeof(renderpoint_t), compare_renderpoints);
        int lastx = 0;

        windstate_t fill = rule->start(1);
        for(n=0;n<num;n++) {
            renderpoint_t*p = &points[n];
            int x = (int)(p->x - bbox->xmin);
            
            if(x < lastx)
                x = lastx;
            if(x > buf->width) {
                break;
            }
            if(fill.is_filled && x!=lastx) {
                fill_bitwise(line, lastx, x);
            }
            fill = rule->add(fill, p->fs, p->dir, p->polygon_nr);
            lastx = x;
        }
        if(fill.is_filled && lastx!=buf->width) {
            
            return 0;// return an error, we're bleeding

            fill_bitwise(line, lastx, buf->width);
        }
    }
    
    for(y=0;y<buf->height;y++) {
        if(buf->lines[y].points) {
            free(buf->lines[y].points);
        }
        memset(&buf->lines[y], 0, sizeof(renderline_t));
    }
    free(buf->lines);buf->lines=0;
    return image;
}

#define MAX_WIDTH 8192
#define MAX_HEIGHT 4096

static inline max(double a, double b) {return a>b?a:b;}
static inline min(double a, double b) {return a<b?a:b;}

intbbox_t intbbox_new(int x1, int y1, int x2, int y2)
{
    intbbox_t b;
    b.xmin = x1;
    b.ymin = y1;
    b.xmax = x2;
    b.ymax = y2;
    b.width = x2-x1;
    b.height = y2-y1;
    return b;
}

intbbox_t intbbox_from_polygon(gfxpoly_t*polygon, double zoom)
{
    int t;
    intbbox_t b = {0,0,0,0};
    edge_t*e = polygon->edges;

    zoom *= polygon->gridsize;

    if(e) {
        b.xmin = e->a.x*zoom;
        b.ymin = e->a.y*zoom;
        b.xmax = e->a.x*zoom;
        b.ymax = e->a.y*zoom;
    }
    for(e=polygon->edges;e;e=e->next) {
        double x_min = min(e->a.x,e->b.x)*zoom;
        double y_min = min(e->a.y,e->b.y)*zoom;
        double x_max = max(e->a.x,e->b.x)*zoom;
        double y_max = max(e->a.y,e->b.y)*zoom;
        int x1 = floor(x_min);
        int y1 = floor(y_min);
        int x2 = ceil(x_max);
        int y2 = ceil(y_max);
        if(x1 < b.xmin) b.xmin = x1;
        if(y1 < b.ymin) b.ymin = y1;
        if(x2 > b.xmax) b.xmax = x2;
        if(y2 > b.xmax) b.ymax = y2;
    }
    if(b.xmax > (int)(MAX_WIDTH*zoom))
	b.xmax = (int)(MAX_WIDTH*zoom);
    if(b.ymax > (int)(MAX_HEIGHT*zoom))
	b.ymax = (int)(MAX_HEIGHT*zoom);
    if(b.xmin < -(int)(MAX_WIDTH*zoom))
	b.xmin = -(int)(MAX_WIDTH*zoom);
    if(b.ymin < -(int)(MAX_HEIGHT*zoom))
	b.ymin = -(int)(MAX_HEIGHT*zoom);
    
    if(b.xmin > b.xmax) 
	b.xmin = b.xmax;
    if(b.ymin > b.ymax) 
	b.ymin = b.ymax;

    b.width = b.xmax - b.xmin;
    b.height = b.ymax - b.ymin;
    return b;
}

#define B11111000 0xf8
#define B01111100 0x7c
#define B00111110 0x3e
#define B00011111 0x1f
#define B11100000 0xe0
#define B01110000 0x70
#define B00111000 0x38
#define B00011100 0x1c
#define B00001110 0x0e
#define B00000111 0x07
#define B10000000 0x80
#define B01000000 0x40
#define B00100000 0x20
#define B00010000 0x10
#define B00001000 0x08
#define B00000100 0x04
#define B00000010 0x02
#define B00000001 0x01

int compare_bitmaps(intbbox_t*bbox, unsigned char*data1, unsigned char*data2)
{
    int x,y;
    int height = bbox->height;
    int width = bbox->width;
    int width8 = (width+7) >> 3;
    unsigned char*l1 = &data1[width8*2];
    unsigned char*l2 = &data2[width8*2];
    for(y=2;y<height-2;y++) {
        for(x=0;x<width8;x++) {
            unsigned char a1 = l1[x-width8*2] & l1[x-width8] & l1[x] & 
                              l1[x+width8] & l1[x+width8*2];
            unsigned char b1 = l2[x-width8*2] & l2[x-width8] & l2[x] & 
                              l2[x+width8] & l2[x+width8*2];
            unsigned char a2 = l1[x-width8*2] | l1[x-width8] | l1[x] | 
                              l1[x+width8] | l1[x+width8*2];
            unsigned char b2 = l2[x-width8*2] | l2[x-width8] | l2[x] | 
                              l2[x+width8] | l2[x+width8*2];

            char fail = 0;
            if((a1&B11111000)==B11111000 && (b2&B11111000)==0) fail=2;
            if((a1&B01111100)==B01111100 && (b2&B01111100)==0) fail=3;
            if((a1&B00111110)==B00111110 && (b2&B00111110)==0) fail=4;
            if((a1&B00011111)==B00011111 && (b2&B00011111)==0) fail=5;
            if((b1&B11111000)==B11111000 && (a2&B11111000)==0) fail=2;
            if((b1&B01111100)==B01111100 && (a2&B01111100)==0) fail=3;
            if((b1&B00111110)==B00111110 && (a2&B00111110)==0) fail=4;
            if((b1&B00011111)==B00011111 && (a2&B00011111)==0) fail=5;
            if(fail) {
                return 0;
            }
        }
        l1 += width8;
        l2 += width8;
    }
    return 1;
}

#include "../png.h"
void save_two_bitmaps(intbbox_t*b, unsigned char*data1, unsigned char*data2, char*filename)
{
    int width8 = (b->width+7) >> 3;
    unsigned char*data = malloc(b->width*b->height*4*4);
    unsigned char*p = data;
    int x,y;
    unsigned char*b1 = data1;
    unsigned char*b2 = data2;
    compare_bitmaps(b, data1, data2);
//#   define MARK ((abs(x-badx)<3 && abs(y-bady)<3)*255)
#define MARK 0
    for(y=0;y<b->height;y++) {
        for(x=0;x<b->width;x++) {
            unsigned char c1 = (b1[x>>3]&(0x80>>(x&7)))?255:0;
            unsigned char c2 = (b2[x>>3]&(0x80>>(x&7)))?255:0;
            *p++ = 255;
            *p++ = c1^c2;
            *p++ = c1^c2;
            *p++ = MARK;
        }
        for(x=0;x<b->width;x++) {
            unsigned char c = (b2[x>>3]&(0x80>>(x&7)))?255:0;
            *p++ = 255;
            *p++ = c;
            *p++ = c;
            *p++ = MARK;
        }
        b1 += width8;
        b2 += width8;
    }
    b1 = data1;
    for(y=0;y<b->height;y++) {
        for(x=0;x<b->width;x++) {
            unsigned char c = (b1[x>>3]&(0x80>>(x&7)))?255:0;
            *p++ = 255;
            *p++ = c;
            *p++ = c;
            *p++ = MARK;
        }
        for(x=0;x<b->width;x++) {
            *p++ = 255;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
        }
        b1 += width8;
    }
    writePNG(filename, data, b->width*2, b->height*2);
    free(data);
}
