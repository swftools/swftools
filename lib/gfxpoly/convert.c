#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../gfxdevice.h"
#include "../mem.h"
#include "poly.h"

/* factor that determines into how many line fragments a spline is converted */
#define SUBFRACTION (2.4)

static edge_t*edge_new(int x1, int y1, int x2, int y2)
{
    edge_t*s = rfx_calloc(sizeof(edge_t));
    s->a.x = x1;
    s->a.y = y1;
    s->b.x = x2;
    s->b.y = y2;
    return s;
}

static inline int32_t convert_coord(double x)
{
    /* we clamp to 31 bit instead of 32 bit because we use
       a (x1-x2) shortcut when comparing coordinates
    */
    if(x < -0x40000000) x = -0x40000000;
    if(x >  0x3fffffff) x =  0x3fffffff;
    return ceil(x);
}

static inline void gfxpoly_add_edge(gfxpoly_t*poly, double _x1, double _y1, double _x2, double _y2)
{
    int x1 = convert_coord(_x1);
    int y1 = convert_coord(_y1);
    int x2 = convert_coord(_x2);
    int y2 = convert_coord(_y2);

    if(x1!=x2 || y1!=y2) {
        edge_t*s = edge_new(x1, y1, x2, y2);
        s->next = poly->edges;
        poly->edges = s;
    }
}

static void convert_gfxline(gfxline_t*line, void*data, void(*moveto)(void*data, double x, double y), void(*lineto)(void*data, double x, double y), void(*setgridsize)(void*data, double g))
{
    assert(!line || line[0].type == gfx_moveTo);
    double lastx=0,lasty=0;
    while(line) {
        if(line->type == gfx_moveTo) {
	    if(line->next && line->next->type != gfx_moveTo && (line->x!=lastx || line->y!=lasty)) {
		moveto(data, line->x, line->y);
	    }
        } else if(line->type == gfx_lineTo) {
            lineto(data, line->x, line->y);
	} else if(line->type == gfx_splineTo) {
            int parts = (int)(sqrt(fabs(line->x-2*line->sx+lastx) + 
                                   fabs(line->y-2*line->sy+lasty))*SUBFRACTION);
            if(!parts) parts = 1;
	    double stepsize = 1.0/parts;
            int i;
	    for(i=0;i<parts;i++) {
		double t = (double)i*stepsize;
		double sx = (line->x*t*t + 2*line->sx*t*(1-t) + lastx*(1-t)*(1-t));
		double sy = (line->y*t*t + 2*line->sy*t*(1-t) + lasty*(1-t)*(1-t));
		lineto(data, sx, sy);
	    }
	    lineto(data, line->x, line->y);
        }
	lastx = line->x;
	lasty = line->y;
        line = line->next;
    }
}

static char* readline(FILE*fi)
{
    char c;
    while(1) {
        int l = fread(&c, 1, 1, fi);
        if(!l)
            return 0;
        if(c!=10 || c!=13)
            break;
    }
    char line[256];
    int pos = 0;
    while(1) {
        line[pos++] = c;
        line[pos] = 0;
        int l = fread(&c, 1, 1, fi);
        if(!l || c==10 || c==13) {
            return strdup(line);
        }
    }
}

static void convert_file(const char*filename, void*data, void(*moveto)(void*data, double x, double y), void(*lineto)(void*data, double x, double y),void(*setgridsize)(void*data, double gridsize))
{
    FILE*fi = fopen(filename, "rb");
    if(!fi) {
        perror(filename);
    }
    int count = 0;
    double g = 0;
    double lastx=0,lasty=0;
    while(1) {
        char*line = readline(fi);
        if(!line)
            break;
        double x,y;
        char s[256];
        if(sscanf(line, "%lf %lf %s", &x, &y, &s) == 3) {
            if(s && !strcmp(s,"moveto")) {
		moveto(data, x, y);
                count++;
            } else if(s && !strcmp(s,"lineto")) {
		lineto(data, x, y);
                count++;
            } else {
                fprintf(stderr, "invalid command: %s\n", s);
            }
        } else if(sscanf(line, "%% gridsize %lf", &g) == 1) {
	    setgridsize(data, g);
        }
        free(line);
    }
    fclose(fi);
    if(g) {
        fprintf(stderr, "loaded %d points from %s (gridsize %f)\n", count, filename, g);
    } else {
        fprintf(stderr, "loaded %d points from %s\n", count, filename);
    }
}

typedef struct _stdpoly {
    gfxpoly_t*poly;
    double lastx,lasty;
    double z;
} stdpoly_t;
static void stdmoveto(void*data, double x, double y)
{
    stdpoly_t*d = (stdpoly_t*)data;
    x *= d->z;
    y *= d->z;
    d->lastx = x;d->lasty = y;
}
static void stdlineto(void*data, double x, double y)
{
    stdpoly_t*d = (stdpoly_t*)data;
    x *= d->z;
    y *= d->z;
    gfxpoly_add_edge(d->poly, d->lastx, d->lasty, x, y);
    d->lastx = x;d->lasty = y;
}
static void stdsetgridsize(void*data, double gridsize)
{
    stdpoly_t*d = (stdpoly_t*)data;
    d->poly->gridsize = gridsize;
}
gfxpoly_t* gfxpoly_from_gfxline(gfxline_t*line, double gridsize)
{
    stdpoly_t data;
    data.poly = gfxpoly_new(gridsize);
    data.z = 1.0 / gridsize;
    data.lastx = data.lasty = 0;
    convert_gfxline(line, &data, stdmoveto, stdlineto, stdsetgridsize);
    return data.poly;
}
gfxpoly_t* gfxpoly_from_file(const char*filename, double gridsize)
{
    stdpoly_t data;
    data.poly = gfxpoly_new(gridsize);
    data.z = 1.0 / gridsize;
    data.lastx = data.lasty = 0;
    convert_file(filename, &data, stdmoveto, stdlineto, stdsetgridsize);
    return data.poly;
}

typedef struct _compactpoly {
    gfxcompactpoly_t*poly;
    point_t last;
    double z;
    int strokes_size;
    point_t*points;
    int num_points;
    int points_size;
    segment_dir_t dir;
} compactpoly_t;

void add_segment(compactpoly_t*data, point_t start, segment_dir_t dir)
{
    if(data->poly->num_strokes == data->strokes_size) {
	data->strokes_size <<= 1;
	assert(data->strokes_size > data->poly->num_strokes);
	data->poly->strokes = rfx_realloc(data->poly->strokes, data->strokes_size);
    }
    data->poly->strokes[data->poly->num_strokes].dir = dir;
    data->points[0] = start;
    data->num_points = 1;
    data->dir = dir;
}
void finish_segment(compactpoly_t*data)
{
    if(data->num_points <= 1)
	return;
    point_t*p = malloc(sizeof(point_t)*data->num_points);
    data->poly->strokes[data->poly->num_strokes-1].points = p;
    if(data->dir == DIR_UP) {
	int t;
	int s = data->num_points;
	for(t=0;t<data->num_points;t++) {
	    p[--s] = data->points[t];
	}
    } else {
	memcpy(p, data->points, sizeof(point_t)*data->num_points);
    }
    data->poly->num_strokes++;
}
static void compactmoveto(void*_data, double _x, double _y)
{
    compactpoly_t*data = (compactpoly_t*)_data;
    data->dir = DIR_UNKNOWN;
    point_t p;
    p.x = convert_coord(_x);
    p.y = convert_coord(_y);
    data->last = p;
}
static void compactlineto(void*_data, double _x, double _y)
{
    compactpoly_t*data = (compactpoly_t*)_data;
    point_t p;
    p.x = convert_coord(_x);
    p.y = convert_coord(_y);
    if(p.y < data->last.y && data->dir != DIR_UP ||
       p.y > data->last.y && data->dir != DIR_DOWN) {
	data->dir = p.y > data->last.y ? DIR_DOWN : DIR_UP;
	finish_segment(data);
	add_segment(data, data->last, data->dir);
    }
    if(data->points_size == data->num_points) {
	data->points_size <<= 1;
	assert(data->points_size > data->num_points);
	data->points = rfx_realloc(data->points, data->points_size);
    }
    data->points[data->num_points++] = p;
}
static void compactsetgridsize(void*data, double gridsize)
{
    compactpoly_t*d = (compactpoly_t*)data;
    d->poly->gridsize = gridsize;
}
static void compactinit(compactpoly_t*data, double gridsize)
{
    data->poly = rfx_calloc(sizeof(gfxcompactpoly_t));
    data->poly->gridsize = gridsize;
    data->z = 1.0 / gridsize;
    data->last.x = data->last.y = 0;
    data->strokes_size = 16;
    data->num_points = 0;
    data->points_size = 16;
    data->points = (point_t*)rfx_alloc(sizeof(point_t)*data->points_size);
    data->poly->strokes = (gfxstroke_t*)rfx_alloc(sizeof(gfxstroke_t)*data->strokes_size);
}
static gfxcompactpoly_t*compactfinish(compactpoly_t*data)
{
    finish_segment(data);
    data->poly->strokes = rfx_realloc(data->poly->strokes, sizeof(gfxstroke_t)*data->poly->num_strokes);
    free(data->points);
    return data->poly;
}
gfxcompactpoly_t* gfxcompactpoly_from_gfxline(gfxline_t*line, double gridsize)
{
    compactpoly_t data;
    compactinit(&data, gridsize);
    data.poly->gridsize = gridsize;
    convert_gfxline(line, &data, compactmoveto, compactlineto, compactsetgridsize);
    return compactfinish(&data);
}
gfxcompactpoly_t* gfxcompactpoly_from_file(const char*filename, double gridsize)
{
    compactpoly_t data;
    compactinit(&data, gridsize);
    data.poly->gridsize = gridsize;
    convert_file(filename, &data, compactmoveto, compactlineto, compactsetgridsize);
    return compactfinish(&data);
}
void gfxcompactpoly_free(gfxcompactpoly_t*poly)
{
    int t;
    for(t=0;t<poly->num_strokes;t++) {
	free(poly->strokes[t].points);
    }
    free(poly->strokes);
    free(poly);
}

