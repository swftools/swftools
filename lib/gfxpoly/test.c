#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "../gfxtools.h"
#include "poly.h"
#include "convert.h"

gfxline_t*mkstar(int x1, int y1, int x2, int y2)
{
    gfxline_t*l=0,*line = 0;
    int x;
    for(x=x1;x<=x2;x+=50) {
        l = malloc(sizeof(gfxline_t));
        l->type = gfx_moveTo;
        l->x = x;l->y = y1;
        line = gfxline_append(line, l);

        l = malloc(sizeof(gfxline_t));
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
    gfxpoly_t*poly = gfxpoly_fillToPoly(b);
    gfxline_free(box1);
    gfxline_free(box2);
    gfxline_free(box3);

    gfxpoly_dump(poly);
    gfxpoly_process(poly);
}

int test2()
{
#define N 1000
    int t;
    gfxline_t* line = malloc(sizeof(gfxline_t)*N);
    for(t=0;t<N;t++) {
        line[t].type = t?gfx_lineTo:gfx_moveTo;
        line[t].x = lrand48()%65536;
        line[t].y = lrand48()%65536;
        line[t].next = &line[t+1];
    }
    line[N-1].x = line[0].x;
    line[N-1].y = line[0].y;
    line[N-1].next = 0;
    
    gfxpoly_t*poly = gfxpoly_fillToPoly(line);
    gfxpoly_dump(poly);
    gfxpoly_process(poly);
    gfxline_free(line);
}

int main()
{
    test2();
}
