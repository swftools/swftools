#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <sys/times.h>
#include "../gfxtools.h"
#include "poly.h"
#include "convert.h"
#include "renderpoly.h"
#include "stroke.h"

#ifdef CHECKS
#error "speedtest must be compiled without CHECKS"
#endif

#ifdef DEBUG
#error "speedtest must be compiled without DEBUG"
#endif

gfxline_t* mkchessboard()
{
    gfxline_t*b = 0;
    int x,y;
    unsigned int r = 0;
    int spacing = 20;

    int num_caros = 40;
    int l = 5;
    char do_centerpiece=1;

    //int num_caros = 4;
    //int l=1;
    //char do_centerpiece=0;

    for(x=-l;x<=l;x++) 
    for(y=-l;y<=l;y++) {
        /* pseudo random */ 
        r = crc32_add_byte(r, x);r = crc32_add_byte(r, y);
        if(r&1) {
            gfxline_t*box;
            if(r&2) {
                box = gfxline_makerectangle(x*spacing,y*spacing,(x+1)*spacing,(y+1)*spacing);
            } else {
                box = gfxline_makerectangle((x+1)*spacing,y*spacing,x*spacing,(y+1)*spacing);
            }
            b = gfxline_append(b, box);
        }
    }

    int t;
    for(t=0;t<num_caros;t++) {
        r = crc32_add_byte(r, t);
        int x=(r%10-5)*spacing;
        int y=((r>>4)%10-5)*spacing;
        int sizex = ((r>>8)%4)*spacing;
        int sizey = sizex;
        if(r&65536)
            sizex = -sizex;
        gfxline_t*l = malloc(sizeof(gfxline_t)*5);
        l[0].type = gfx_moveTo;l[0].next = &l[1];
        l[1].type = gfx_lineTo;l[1].next = &l[2];
        l[2].type = gfx_lineTo;l[2].next = &l[3];
        l[3].type = gfx_lineTo;l[3].next = &l[4];
        l[4].type = gfx_lineTo;l[4].next = 0;
        l[0].x = x;
        l[0].y = y-sizey;
        l[1].x = x+sizex;
        l[1].y = y;
        l[2].x = x;
        l[2].y = y+sizey;
        l[3].x = x-sizex;
        l[3].y = y;
        l[4].x = x;
        l[4].y = y-sizey;
        gfxline_append(b, l);
    }
    if(do_centerpiece) {
	for(t=0;t<5;t++) {
	    gfxline_t*l = gfxline_makerectangle(-9*spacing,-10,9*spacing,10);
	    gfxmatrix_t matrix;
	    memset(&matrix, 0, sizeof(gfxmatrix_t));
	    double ua=t*0.43;
	    matrix.m00=cos(ua);matrix.m10=sin(ua);
	    matrix.m01=-sin(ua);matrix.m11=cos(ua);
	    gfxline_transform(l, &matrix);
	    gfxline_append(b, l);
	}
	gfxline_append(b, gfxline_makecircle(100,100,100,100));
    }
    return b;
}

gfxline_t* make_circles(gfxline_t*b, int n)
{
    unsigned int c = 0;
    int t;
    for(t=0;t<n;t++) {
        c = crc32_add_byte(c, t);
	int x = c%200;
	c = crc32_add_byte(c, t);
	int y = c%200;;
	c = crc32_add_byte(c, t^0x55);
	int r = c%100;
	gfxline_t*c = gfxline_makecircle(x,y,r,r);
	b = gfxline_append(b, c);
	//b = gfxline_append(b, gfxline_makerectangle(10,10,100,100));
    }
    return b;
}

static windcontext_t onepolygon = {1};
static windcontext_t twopolygons = {2};

int test_speed()
{
    gfxline_t* b = mkchessboard();
    b = make_circles(b, 30);

    gfxmatrix_t m;
    memset(&m, 0, sizeof(gfxmatrix_t));
    int t;
    for(t=0;t<360;t++) {
	//printf("%d\n", t);
	m.m00 = cos(t*M_PI/180.0);
	m.m01 = sin(t*M_PI/180.0);
	m.m10 = -sin(t*M_PI/180.0);
	m.m11 = cos(t*M_PI/180.0);
	m.tx = 400*1.41/2;
	m.ty = 400*1.41/2;
	gfxline_t*l = gfxline_clone(b);
	gfxline_transform(l, &m);
	gfxpoly_t*poly = gfxpoly_from_fill(b, 0.05);

	gfxpoly_t*poly2 = gfxpoly_process(poly, 0, &windrule_evenodd, &onepolygon);
	gfxpoly_destroy(poly);
	gfxpoly_destroy(poly2);
	gfxline_free(l);
    }
    gfxline_free(b);
}

int main(int argn, char*argv[])
{
    struct tms t1,t2;
    times(&t1);
    test_speed();
    times(&t2);
    printf("%d\n", t2.tms_utime - t1.tms_utime);
}

