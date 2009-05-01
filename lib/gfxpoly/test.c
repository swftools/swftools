#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <math.h>
#include "../gfxtools.h"
#include "poly.h"
#include "convert.h"
#include "renderpoly.h"

gfxline_t*mkstar(int x1, int y1, int x2, int y2)
{
    gfxline_t*l=0,*line = 0;
    int x;
    for(x=x1;x<=x2;x+=50) {
        l = rfx_calloc(sizeof(gfxline_t));
        l->type = gfx_moveTo;
        l->x = x;l->y = y1;
        line = gfxline_append(line, l);

        l = rfx_calloc(sizeof(gfxline_t));
        l->type = gfx_lineTo;
        l->x = x2-x;l->y = y2;
        line = gfxline_append(line, l);
    }
    return line;
}

int test1()
{
    gfxline_t*box1 = gfxline_makerectangle(50,50,150,150);
    // put box2 and box3 on top of each other *snicker*
    gfxline_t*box2 = gfxline_makerectangle(100,100,200,200);
    gfxline_t*box3 = gfxline_makerectangle(100,100,200,200);
    gfxline_t*star = mkstar(50,50, 150,150);
    gfxline_t*b = 0;
    b = gfxline_append(b, box1);
    b = gfxline_append(b, box2);
    b = gfxline_append(b, box3);
    b = gfxline_append(b, star);

    gfxmatrix_t matrix;
    memset(&matrix, 0, sizeof(gfxmatrix_t));
    double ua=0.1;
    matrix.m00=cos(ua);matrix.m10=sin(ua);
    matrix.m01=-sin(ua);matrix.m11=cos(ua);

    gfxline_transform(b, &matrix);
    gfxpoly_t*poly = gfxpoly_fillToPoly(b, 0.05);
    gfxline_free(box1);
    gfxline_free(box2);
    gfxline_free(box3);
    gfxline_free(star);

    gfxpoly_dump(poly);
    gfxpoly_process(poly, &windrule_evenodd);
}

int test_square(int width, int height, int num, double gridsize, char bitmaptest)
{
    int t;
    gfxline_t* line = malloc(sizeof(gfxline_t)*num);
    for(t=0;t<num;t++) {
        line[t].type = t?gfx_lineTo:gfx_moveTo;
        line[t].x = (lrand48()%width);
        line[t].y = (lrand48()%height);
        line[t].next = &line[t+1];
    }
    line[num-1].x = line[0].x;
    line[num-1].y = line[0].y;
    line[num-1].next = 0;
    
    windrule_t*rule = &windrule_circular;
    
    gfxpoly_t*poly = gfxpoly_fillToPoly(line, gridsize);
    gfxpoly_t*poly2 = gfxpoly_process(poly, rule);
    gfxline_free(line);

    if(bitmaptest) {
        intbbox_t bbox = intbbox_new(0, 0, width, height);
        unsigned char*bitmap1 = render_polygon(poly, &bbox, 1.0, rule);
        unsigned char*bitmap2 = render_polygon(poly2, &bbox, 1.0, &windrule_evenodd);
        if(!compare_bitmaps(&bbox, bitmap1, bitmap2)) {
            save_two_bitmaps(&bbox, bitmap1, bitmap2, "error.png");
            assert(!"bitmaps don't match");
        }
    }

    gfxpoly_destroy(poly);
    gfxpoly_destroy(poly2);
}

int test2()
{
    test_square(400,400, 3, 0.05, 1);

    int t;
    for(t=0;t<400;t++) {
        test_square(400,400, 50, 0.05, 1);
        test_square(200,3, 1000, 1.0, 0);
        test_square(3,200, 1000, 1.0, 0);
        test_square(10,10, 200, 1.0, 0);
    }
}

#include "../rfxswf.h"
void test3()
{
#undef N
#undef RANGE
#define N 100
#define RANGE 400

    int i;
    gfxline_t* line = malloc(sizeof(gfxline_t)*N*2);
    for(i=0;i<N;i++) {
        line[i].type = i?gfx_lineTo:gfx_moveTo;
        line[i].x = lrand48()%RANGE - RANGE/2;
        line[i].y = lrand48()%RANGE - RANGE/2;
        line[i].next = &line[i+1];
        line[N*2-i-1].type = gfx_lineTo;
        line[N*2-i-1].x = line[i].x;
        line[N*2-i-1].y = line[i].y;
        line[N*2-i-1].next = &line[N*2-i];
    }
    line[N*2-1].next = 0;

    line[N-1].x = line[0].x;
    line[N-1].y = line[0].y;
    line[N-1].next = 0;

    gfxmatrix_t m;
    memset(&m, 0, sizeof(m));

    SWF swf;
    memset(&swf, 0, sizeof(SWF));
    swf.movieSize.xmax = RANGE*20*1.41;
    swf.movieSize.ymax = RANGE*20*1.41;
    swf.fileVersion = 9;
    swf.frameRate = 25*0x100;
    TAG * tag = swf.firstTag = swf_InsertTag(0, ST_SETBACKGROUNDCOLOR);
    swf_SetU8(tag, 0);
    swf_SetU8(tag, 0);
    swf_SetU8(tag, 0);

    int t;
    for(t=0;t<360;t++) {
        m.m00 = cos(t*M_PI/180.0);
        m.m01 = sin(t*M_PI/180.0);
        m.m10 = -sin(t*M_PI/180.0);
        m.m11 = cos(t*M_PI/180.0);
        m.tx = RANGE*1.41/2;
        m.ty = RANGE*1.41/2;
        gfxline_t*l = gfxline_clone(line);
        gfxline_transform(l, &m);
        
        gfxpoly_t*poly = gfxpoly_fillToPoly(l, 0.05);
        gfxpoly_t*poly2 = gfxpoly_process(poly, &windrule_circular);

        tag = swf_InsertTag(tag, ST_DEFINESHAPE);
        SHAPE* s;
        swf_ShapeNew(&s);
        RGBA rgb;
        rgb.r = rgb.g = 0x00; rgb.b = 0xff;
        rgb.a = 255;
        int fs = swf_ShapeAddSolidFillStyle(s,&rgb);
        int ls = swf_ShapeAddLineStyle(s,20,&rgb);
        swf_SetU16(tag,t+1);
        swf_SetRect(tag,&swf.movieSize);
        swf_SetShapeHeader(tag,s);

#define FILL
#ifdef FILL
        swf_ShapeSetAll(tag,s,0,0,0,fs,0);
        edge_t*e = poly2->edges;
        while(e) {
            swf_ShapeSetMove(tag, s, e->a.x, e->a.y);
            swf_ShapeSetLine(tag, s, e->b.x - e->a.x, e->b.y - e->a.y);
            e = e->next;
        }
#else
        swf_ShapeSetAll(tag,s,0,0,ls,0,0);
        edge_t*e = poly2->edges;
        while(e) {
            swf_ShapeSetMove(tag, s, e->a.x, e->a.y);
            swf_ShapeSetLine(tag, s, e->b.x - e->a.x, e->b.y - e->a.y);
            
            swf_ShapeSetCircle(tag, s, e->a.x, e->a.y, 5*20, 5*20);
            swf_ShapeSetCircle(tag, s, e->b.x, e->b.y, 5*20, 5*20);
            e = e->next;
        }
#endif

        swf_ShapeSetEnd(tag);
        swf_ShapeFree(s);

        gfxpoly_destroy(poly);
        gfxpoly_destroy(poly2);

        gfxline_free(l);
   
        if(t) {
            tag = swf_InsertTag(tag,ST_REMOVEOBJECT2);
            swf_SetU16(tag, t);
        }
        tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
        swf_ObjectPlace(tag,t+1,t+1,NULL,NULL,NULL);

        tag = swf_InsertTag(tag, ST_SHOWFRAME);
    }
    tag = swf_InsertTag(tag, ST_END);

    swf_SaveSWF(&swf, "test.swf");
}

int main()
{
    test3();
}
