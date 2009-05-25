#include <stdlib.h>
#include <stdio.h>
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

gfxline_t* make_circles()
{
    gfxline_t*b = 0;
    unsigned int c = 0;
    int t;
    for(t=0;t<30;t++) {
        c = crc32_add_byte(c, t);
	int x = c%200;
	c = crc32_add_byte(c, t);
	int y = c%200;;
	c = crc32_add_byte(c, t^0x55);
	int r = c%100;
	b = gfxline_append(b, gfxline_makecircle(x,y,r,r));
    }
    return b;
}

static windcontext_t onepolygon = {1};

int test0()
{
    //gfxline_t* b = mkchessboard();
    //gfxline_t* b = mkrandomshape(100,7);
    gfxline_t* b = gfxline_makecircle(100,100,100,100);

    gfxmatrix_t m;
    memset(&m, 0, sizeof(gfxmatrix_t));
    int t;
    for(t=0;t<360;t++) {
	m.m00 = cos(t*M_PI/180.0);
	m.m01 = sin(t*M_PI/180.0);
	m.m10 = -sin(t*M_PI/180.0);
	m.m11 = cos(t*M_PI/180.0);
	m.tx = 400*1.41/2;
	m.ty = 400*1.41/2;
	gfxline_transform(b, &m);
	gfxpoly_t*poly = gfxpoly_from_gfxline(b, 0.05);

	gfxpoly_t*poly2 = gfxpoly_process(poly, &windrule_evenodd, &onepolygon);
	gfxpoly_destroy(poly2);
	gfxpoly_destroy(poly);
    }
    gfxline_free(b);
}

int test1(int argn, char*argv[])
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
    //b = gfxline_append(b, star);

    gfxmatrix_t matrix;
    memset(&matrix, 0, sizeof(gfxmatrix_t));
    double ua=0.1;
    matrix.m00=cos(ua);matrix.m10=sin(ua);
    matrix.m01=-sin(ua);matrix.m11=cos(ua);

    //gfxline_transform(b, &matrix);

    gfxpoly_t*poly = gfxpoly_from_gfxline(b, 0.05);
    gfxline_free(box1);
    gfxline_free(box2);
    gfxline_free(box3);
    gfxline_free(star);

    gfxpoly_dump(poly);
    gfxpoly_t*poly2 = gfxpoly_process(poly, &windrule_evenodd, &onepolygon);
    gfxpoly_destroy(poly2);
    gfxpoly_destroy(poly);
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
    
    gfxpoly_t*poly = gfxpoly_from_gfxline(line, gridsize);
    gfxline_free(line);

    windrule_t*rule = &windrule_circular;
    gfxpoly_t*poly2 = gfxpoly_process(poly, rule, &onepolygon);
    if(bitmaptest) {
        intbbox_t bbox = intbbox_new(0, 0, width, height);
        unsigned char*bitmap1 = render_polygon(poly, &bbox, 1.0, rule, &onepolygon);
        unsigned char*bitmap2 = render_polygon(poly2, &bbox, 1.0, &windrule_evenodd, &onepolygon);
        if(!compare_bitmaps(&bbox, bitmap1, bitmap2)) {
            save_two_bitmaps(&bbox, bitmap1, bitmap2, "error.png");
            assert(!"bitmaps don't match");
        }
    }
    gfxpoly_destroy(poly2);
    gfxpoly_destroy(poly);
}

int test2(int argn, char*argv[])
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
void test3(int argn, char*argv[])
{
#undef N
#undef RANGE
#define N 100
#define RANGE 400

    //gfxline_t*line = mkrandomshape(RANGE, N);
    //windrule_t*rule = &windrule_circular;
    //gfxline_t*line = mkchessboard();
    gfxline_t*line = make_circles();
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
        
        gfxpoly_t*poly = gfxpoly_from_gfxline(l, 0.05);
        gfxpoly_t*poly2 = gfxpoly_process(poly, rule, &onepolygon);

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
#define ROTATE
#ifdef ROTATE
            swf_ShapeSetMove(tag, s, e->a.y, e->a.x);
            swf_ShapeSetLine(tag, s, e->b.y - e->a.y, e->b.x - e->a.x);
#else
            swf_ShapeSetMove(tag, s, e->a.x, e->a.y);
            swf_ShapeSetLine(tag, s, e->b.x - e->a.x, e->b.y - e->a.y);
#endif
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

void rotate90(gfxpoly_t*poly)
{
    edge_t*e = poly->edges;
    while(e) {
	point_t a = e->a;
	point_t b = e->b;
	e->a.x = a.y;
	e->a.y = a.x;
	e->b.x = b.y;
	e->b.y = b.x;
	e = e->next;
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
        gfxpoly_t*poly = gfxpoly_from_file(filename, 1.0);//0.01);

        if(argn!=2)
            free(filename);

        double zoom = 1.0;

        if(!gfxpoly_check(poly)) {
            printf("bad polygon\n");
            continue;
        }

        gfxpoly_t*poly2 = gfxpoly_process(poly, rule, &onepolygon);

	int pass;
	for(pass=0;pass<2;pass++) {
	    intbbox_t bbox = intbbox_from_polygon(poly, zoom);
	    unsigned char*bitmap1 = render_polygon(poly, &bbox, zoom, rule, &onepolygon);
	    unsigned char*bitmap2 = render_polygon(poly2, &bbox, zoom, &windrule_evenodd, &onepolygon);
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
	    rotate90(poly);
	    rotate90(poly2);
	}

        gfxpoly_destroy(poly);
        gfxpoly_destroy(poly2);
        if(argn==2) 
            break;
    }
    closedir(_dir);
}

#include "../gfxdevice.h"
#include "../pdf/pdf.h"

void extract_polygons_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color) 
{
    //gfxcompactpoly_t*c = gfxcompactpoly_from_gfxline(line, 0.05);
    //gfxcompactpoly_free(c);

    gfxpoly_t*poly = gfxpoly_from_gfxline(line, 0.05);

    gfxline_dump(line, stderr, "");
    gfxpoly_dump(poly);

    if(gfxpoly_size(poly)>100000) {
	fprintf(stderr, "%d segments (skipping)\n", gfxpoly_size(poly));
	return;
    } else {
	//fprintf(stderr, "%d segments\n", gfxpoly_size(poly));
    }

    if(!gfxpoly_check(poly)) {
        gfxpoly_destroy(poly);
        fprintf(stderr, "bad polygon\n");
        return;
    }

    windrule_t*rule = &windrule_evenodd;
        
    double zoom = 1.0;
    intbbox_t bbox = intbbox_from_polygon(poly, zoom);
    unsigned char*bitmap1 = render_polygon(poly, &bbox, zoom, rule, &onepolygon);
    if(!bitmap_ok(&bbox, bitmap1)) {
        fprintf(stderr, "bad polygon or error in renderer\n");
        return;
    }
    gfxpoly_t*poly2 = gfxpoly_process(poly, rule, &onepolygon);
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

    gfxpoly_destroy(poly);
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
void extract_polygons_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action)
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

int main(int argn, char*argv[])
{
    test3(argn, argv);
}

