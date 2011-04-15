#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "../gfxtools.h"
#include "poly.h"
#include "convert.h"
#include "renderpoly.h"
#include "stroke.h"

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

gfxline_t* mkrandomshape(int range, int n)
{
    int i;
    gfxline_t* line = malloc(sizeof(gfxline_t)*n);
    for(i=0;i<n;i++) {
        line[i].type = i?gfx_lineTo:gfx_moveTo;
        line[i].x = lrand48()%range - range/2;
        line[i].y = lrand48()%range - range/2;
        line[i].next = &line[i+1];
    }
    line[n-1].x = line[0].x;
    line[n-1].y = line[0].y;
    line[n-1].next = 0;
    return line;
}

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

gfxline_t* make_circles(int n)
{
    gfxline_t*b = 0;
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
    //gfxline_t* b = mkchessboard();
    //gfxline_t* b = mkrandomshape(100,7);
    gfxline_t* b = make_circles(30);

    gfxmatrix_t m;
    memset(&m, 0, sizeof(gfxmatrix_t));
    int t;
    for(t=0;t<360;t++) {
	printf("%d\n", t);
	m.m00 = cos(t*M_PI/180.0);
	m.m01 = sin(t*M_PI/180.0);
	m.m10 = -sin(t*M_PI/180.0);
	m.m11 = cos(t*M_PI/180.0);
	m.tx = 400*1.41/2;
	m.ty = 400*1.41/2;
	gfxline_t*l = gfxline_clone(b);
	gfxline_transform(l, &m);
	gfxpoly_t*poly = gfxpoly_from_fill(b, 0.05);

	gfxpoly_t*poly2 = gfxpoly_process(poly, 0, &windrule_evenodd, &onepolygon, 0);
	gfxpoly_destroy(poly);
	gfxpoly_destroy(poly2);
	gfxline_free(l);
    }
    gfxline_free(b);
}

int testbox(int argn, char*argv[])
{
    gfxline_t*box1 = gfxline_makerectangle(-100,-100,100,100);
    gfxline_t*box2 = gfxline_makerectangle(-50,-50,150,150);
    gfxpoly_t*poly1 = gfxpoly_from_fill(box1, 0.05);
    gfxpoly_t*poly2 = gfxpoly_from_fill(box2, 0.05);
    gfxline_free(box1);
    gfxline_free(box2);
    
    gfxpoly_t*poly12 = gfxpoly_process(poly1, poly2, &windrule_intersect, &twopolygons, 0);
    gfxpoly_dump(poly12);
    assert(gfxpoly_check(poly12, 0));
    gfxpoly_destroy(poly12);
}

int teststroke(int argn, char*argv[])
{
    //gfxline_t*box1 = gfxline_makerectangle(-100,-100,100,100);
    gfxline_t*box1 = gfxline_makerectangle(100,100,200,200);
    gfxpoly_t*poly = gfxpoly_from_stroke(box1, 10.0, gfx_capRound, gfx_joinMiter, 1000, 0.05);
    assert(gfxpoly_check(poly, 1));
}

int test0(int argn, char*argv[])
{
    gfxline_t*box1 = gfxline_makerectangle(-100,-100,100,100);
    gfxline_t*box2 = gfxline_makerectangle(-100,-100,100,100);
    gfxline_t*box3 = gfxline_makerectangle(-100,-100,100,100);
    //gfxline_append(box2, box3);

    gfxmatrix_t matrix;
    memset(&matrix, 0, sizeof(gfxmatrix_t));
    double ua=M_PI/4;
    matrix.m00=cos(ua);matrix.m10=sin(ua);
    matrix.m01=-sin(ua);matrix.m11=cos(ua);
    gfxline_transform(box1, &matrix);
    
    //gfxline_t*b = 0;
    //b = gfxline_append(b, box1);
    //b = gfxline_append(b, box2);
    //gfxline_dump(b, stderr, "");

    gfxpoly_t*poly1 = gfxpoly_from_fill(box1, 0.05);
    gfxpoly_t*poly2 = gfxpoly_from_fill(box2, 0.05);
    
    gfxline_free(box1);
    gfxline_free(box2);
    
    //gfxpoly_t*poly3 = gfxpoly_process(poly1, poly2, &windrule_intersect, &twopolygons);
    gfxpoly_t*poly3 = gfxpoly_process(poly1, poly2, &windrule_intersect, &twopolygons, 0);
    gfxpoly_dump(poly3);
    
    gfxline_t*line = gfxline_from_gfxpoly(poly3);

    gfxline_dump(line, stdout, "");
    gfxline_free(line);
    gfxpoly_destroy(poly1);
    gfxpoly_destroy(poly2);
    gfxpoly_destroy(poly3);
}


int test1(int argn, char*argv[])
{
    gfxline_t*box1 = gfxline_makerectangle(50,50,150,150);
    gfxline_t*box2 = gfxline_makerectangle(100,100,200,200);
    gfxline_t*box3 = gfxline_makerectangle(200,100,300,200);
    gfxline_t*box4 = gfxline_makerectangle(300,200,400,400);
    gfxline_t* board = mkchessboard();
    gfxline_t*star = mkstar(50,50, 150,150);
    gfxline_t*b = 0;
    b = gfxline_append(b, box1);
    b = gfxline_append(b, box2);
    b = gfxline_append(b, box3);
    b = gfxline_append(b, box4);

    gfxmatrix_t matrix;
    memset(&matrix, 0, sizeof(gfxmatrix_t));
    matrix.m00 = 1.0;
    matrix.m11 = 1.0;
    matrix.tx = 200;
    matrix.ty = 200;
    gfxline_transform(board, &matrix);
    b = gfxline_append(b, board);

    //gfxline_transform(b, &matrix);

    //gfxline_dump(b, stderr, "");
    gfxpoly_t*poly = gfxpoly_from_fill(b, 0.05);
    
    gfxline_free(box1);
    gfxline_free(box2);
    gfxline_free(box3);
    gfxline_free(star);

    //gfxpoly_dump(poly);
    gfxpoly_t*poly2 = gfxpoly_process(poly, 0, &windrule_evenodd, &onepolygon, 0);
    //gfxpoly_dump(poly2);
    gfxpoly_save_arrows(poly2, "test.ps");
    gfxpoly_destroy(poly);
    gfxpoly_destroy(poly2);
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

    gfxpoly_t*poly1 = gfxpoly_from_fill(line, gridsize);
    gfxline_free(line);

    windrule_t*rule = &windrule_circular;
    gfxpoly_t*poly2 = gfxpoly_process(poly1, 0, rule, &onepolygon, 0);
    if(bitmaptest) {
        intbbox_t bbox = intbbox_new(0, 0, width, height);
        unsigned char*bitmap1 = render_polygon(poly1, &bbox, 1.0, rule, &onepolygon);
	assert(bitmap_ok(&bbox, bitmap1));
        unsigned char*bitmap2 = render_polygon(poly2, &bbox, 1.0, &windrule_evenodd, &onepolygon);
	assert(bitmap_ok(&bbox, bitmap2));
        if(!compare_bitmaps(&bbox, bitmap1, bitmap2)) {
            save_two_bitmaps(&bbox, bitmap1, bitmap2, "error.png");
            assert(!"bitmaps don't match");
        }
    }
    gfxpoly_destroy(poly1);
    gfxpoly_destroy(poly2);
}

int test_area()
{
    int t;
    for(t=0;t<50;t++) 
    {
        int x1 = lrand48()%200;
        int y1 = lrand48()%200;
        int x2 = x1 + 10+(lrand48()%200);
        int y2 = y1 + 10+(lrand48()%200);
        gfxline_t*line1 =
            gfxline_append(
                    gfxline_makerectangle(50, 50, 150, 150),
                    gfxline_makerectangle(75, 75, 125, 125)
            );
        gfxline_t*line2 = gfxline_makerectangle(x1, y1, x2, y2);

        gfxmatrix_t m;
        m.m00 = cos(t*M_PI/180.0);
        m.m01 = sin(t*M_PI/180.0);
        m.m10 = -sin(t*M_PI/180.0);
        m.m11 = cos(t*M_PI/180.0);
        m.tx = 0;
        m.ty = 0;
        gfxline_clone(line1);
        gfxline_clone(line2);
        gfxline_transform(line1, &m);
        gfxline_transform(line2, &m);

        gfxpoly_t*poly1 = gfxpoly_from_fill(line1, 0.05);
        gfxpoly_t*poly2 = gfxpoly_from_fill(line2, 0.05);

        gfxpoly_t*i = gfxpoly_intersect(poly1, poly2);
        double area1 = gfxpoly_intersection_area(poly1, poly2);
        double area2 = gfxpoly_area(i);
        printf("%f %f\n", area1, area2);

        gfxpoly_destroy(poly1);
        gfxpoly_destroy(poly2);
        gfxline_free(line1);
        gfxline_free(line2);
    }
}

int test2(int argn, char*argv[])
{
    test_square(400,400, 3, 0.05, 1);

    int t;
    for(t=0;t<400;t++) {
	fprintf(stderr, "%d\n", t);
        test_square(400,400, 50, 0.05, 1);
        test_square(200,3, 1000, 1.0, 0);
        test_square(3,200, 1000, 1.0, 0);
        test_square(10,10, 200, 1.0, 0);
    }
}

#include "../rfxswf.h"
void test3(int argn, char*argv[])
{
#undef N
#undef RANGE
#define N 100
#define RANGE 400

    //gfxline_t*line = mkrandomshape(RANGE, N);
    //windrule_t*rule = &windrule_circular;
    gfxline_t*line = mkchessboard();
    //gfxline_t*line = make_circles(30);
    windrule_t*rule = &windrule_evenodd;
    //windrule_t*rule = &windrule_circular;

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
	fprintf(stderr, "%d\n", t);
        m.m00 = cos(t*M_PI/180.0);
        m.m01 = sin(t*M_PI/180.0);
        m.m10 = -sin(t*M_PI/180.0);
        m.m11 = cos(t*M_PI/180.0);
        m.tx = RANGE*1.41/2;
        m.ty = RANGE*1.41/2;

        gfxline_t*l = gfxline_clone(line);
        gfxline_transform(l, &m);

        gfxpoly_t*poly1 = gfxpoly_from_fill(l, 0.05);

        gfxpoly_t*poly2 = gfxpoly_process(poly1, 0, rule, &onepolygon, 0);
	assert(gfxpoly_check(poly2, 0));

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
        swf_ShapeSetAll(tag,s,UNDEFINED_COORD,UNDEFINED_COORD,0,fs,0);

	int i,j;
	gfxpolystroke_t*stroke = poly2->strokes;
	for(;stroke;stroke=stroke->next) {
	    for(j=0;j<stroke->num_points-1;j++) {
		point_t a = stroke->points[j];
		point_t b = stroke->points[j+1];
#define ROTATE
#ifdef ROTATE
		swf_ShapeSetMove(tag, s, a.y, a.x);
		swf_ShapeSetLine(tag, s, b.y - a.y, b.x - a.x);
#else
		swf_ShapeSetMove(tag, s, a.x, a.y);
		swf_ShapeSetLine(tag, s, b.x - a.x, b.y - a.y);
#endif
	    }
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

        gfxpoly_destroy(poly1);
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

void rotate90(gfxpoly_t*poly)
{
    int i,j;
    gfxpolystroke_t*stroke = poly->strokes;
    for(;stroke;stroke=stroke->next) {
	for(j=0;j<stroke->num_points;j++) {
	    point_t a = stroke->points[j];
	    stroke->points[j].x = a.y;
	    stroke->points[j].y = a.x;
	}
    }
}

#include <dirent.h>
void test4(int argn, char*argv[])
{
    char*dir = "ps";
    DIR*_dir = opendir(dir);
    if(!_dir) return;
    struct dirent*file;
    while(1) {
        file = readdir(_dir);
        if (!file) 
            break;
        if(!strstr(file->d_name, ".ps")) 
            continue;

        char* filename;

        if(argn<2)
            filename = allocprintf("%s/%s", dir, file->d_name);
        else
            filename = argv[1];

        windrule_t*rule = &windrule_evenodd;
        gfxpoly_t*poly1 = gfxpoly_from_file(filename, 1.0);//0.01);

        if(argn!=2)
            free(filename);

        double zoom = 1.0;

        if(!gfxpoly_check(poly1, 0)) {
            printf("bad polygon\n");
	    goto end_of_loop;
        }

        gfxpoly_t*poly2 = gfxpoly_process(poly1, 0, rule, &onepolygon, 0);
	gfxpoly_dump(poly2);
	assert(gfxpoly_check(poly2, 1));

	int pass;
	for(pass=0;pass<2;pass++) {
	    intbbox_t bbox = intbbox_from_polygon(poly1, zoom);
	    unsigned char*bitmap1 = render_polygon(poly1, &bbox, zoom, rule, &onepolygon);
	    unsigned char*bitmap2 = render_polygon(poly2, &bbox, zoom, &windrule_circular, &onepolygon);
	    if(!bitmap_ok(&bbox, bitmap1) || !bitmap_ok(&bbox, bitmap2)) {
		save_two_bitmaps(&bbox, bitmap1, bitmap2, "error.png");
		assert(!"error in bitmaps");
	    }
	    if(!compare_bitmaps(&bbox, bitmap1, bitmap2)) {
		save_two_bitmaps(&bbox, bitmap1, bitmap2, "error.png");
		assert(!"bitmaps don't match");
	    }
	    free(bitmap1);
	    free(bitmap2);

	    // second pass renders the 90° rotated version
	    rotate90(poly1);
	    rotate90(poly2);
	}

        gfxpoly_destroy(poly1);
        gfxpoly_destroy(poly2);
	
	end_of_loop:
        if(argn==2) 
            break;
    }
    closedir(_dir);
}

#include "../gfxdevice.h"
#include "../pdf/pdf.h"

static int max_segments = 0;
static int max_any_segments = 0;
void extract_polygons_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color) 
{
    //gfxpoly_t*c = gfxpoly_from_fill(line, 0.05);
    //gfxpoly_free(c);

    //gfxpoly_t*poly1 = gfxpoly_from_fill(line, 0.05);
    gfxpoly_t*poly1 = gfxpoly_from_fill(line, 0.05);

    //gfxline_dump(line, stderr, "");
    //gfxpoly_dump(poly);

    int size = gfxpoly_size(poly1);
    if(size == 4) {
	//rectangles are boring.
	gfxpoly_destroy(poly1);
	return;
    }

    max_any_segments = size > max_any_segments? size : max_any_segments;
    if(size>100000) {
	fprintf(stderr, "%d segments (skipping)\n", size);
	return;
    } else {
	max_segments = size > max_segments? size : max_segments;
	fprintf(stderr, "%d segments (max so far: %d/%d)\n", size, max_segments, max_any_segments);
    }

    if(!gfxpoly_check(poly1, 0)) {
        gfxpoly_destroy(poly1);
        fprintf(stderr, "bad polygon\n");
        return;
    }

    windrule_t*rule = &windrule_evenodd;

    double zoom = 1.0;
    intbbox_t bbox = intbbox_from_polygon(poly1, zoom);
    unsigned char*bitmap1 = render_polygon(poly1, &bbox, zoom, rule, &onepolygon);
    if(!bitmap_ok(&bbox, bitmap1)) {
        fprintf(stderr, "bad polygon or error in renderer\n");
        return;
    }
    gfxpoly_t*poly2 = gfxpoly_process(poly1, 0, rule, &onepolygon, 0);
    unsigned char*bitmap2 = render_polygon(poly2, &bbox, zoom, &windrule_evenodd, &onepolygon);
    if(!bitmap_ok(&bbox, bitmap2)) {
        save_two_bitmaps(&bbox, bitmap1, bitmap2, "error.png");
        assert(!"error in bitmap");
    }
    if(!compare_bitmaps(&bbox, bitmap1, bitmap2)) {
        save_two_bitmaps(&bbox, bitmap1, bitmap2, "error.png");
        assert(!"bitmaps don't match");
    }
    free(bitmap1);
    free(bitmap2);

    gfxpoly_destroy(poly1);
    gfxpoly_destroy(poly2);
}
int extract_polygons_setparameter(gfxdevice_t*dev, const char*key, const char*value) {
    return 0;
}
void extract_polygons_startclip(gfxdevice_t*dev, gfxline_t*line) 
{
    extract_polygons_fill(dev, line, 0);
}
void extract_polygons_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*imgcoord2devcoord, gfxcxform_t*cxform)
{
    extract_polygons_fill(dev, line, 0);
}
void extract_polygons_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*gradcoord2devcoord)
{
    extract_polygons_fill(dev, line, 0);
}
void extract_polygons_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text)
{
    extract_polygons_fill(dev, line, 0);
}
void extract_polygons_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    int t;
    for(t=0;t<font->num_glyphs;t++) {
        //extract_polygons_fill(dev, font->glyphs[t].line, 0);
    }
}
void extract_polygons_endclip(gfxdevice_t*dev)
{
}
void extract_polygons_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
}
void extract_polygons_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix)
{
}
    
gfxdevice_t extract_polygons = 
{
name: "extract polygons",
setparameter:extract_polygons_setparameter,
startclip: extract_polygons_startclip,
endclip: extract_polygons_endclip,
stroke: extract_polygons_stroke,
fill: extract_polygons_fill,
fillbitmap: extract_polygons_fillbitmap,
fillgradient: extract_polygons_fillgradient,
addfont: extract_polygons_addfont,
drawchar: extract_polygons_drawchar,
drawlink: extract_polygons_drawlink,
startpage: 0,
endpage: 0,
geterror: 0,
finish: 0,
internal: 0
};

#if 0
void test5(int argn, char*argv[])
{
    gfxsource_t*driver = gfxsource_pdf_create();
    char*dir = "pdfs";
    DIR*_dir = opendir(dir);
    if(!_dir) return;
    struct dirent*file;
    while(1) {
        file = readdir(_dir);
        if (!file) 
            break;
        if(!strstr(file->d_name, ".pdf")) 
            continue;
        char* filename = allocprintf("%s/%s", dir, file->d_name);

	if(argn>1) 
	    filename = argv[1];

        gfxdocument_t*doc = driver->open(driver, filename);
        gfxdevice_t*out = &extract_polygons;
        int t;
        for(t=1;t<=doc->num_pages;t++) {
            fprintf(stderr, "%s (page %d)\n", filename, t);
            gfxpage_t* page = doc->getpage(doc, t);
            page->render(page, out);
            page->destroy(page);
        }
        doc->destroy(doc);
	if(argn>1) 
	    break;
        free(filename);
    }
    closedir(_dir);
    driver->destroy(driver);
}
#endif

int main(int argn, char*argv[])
{
    test_area(argn, argv);
}

