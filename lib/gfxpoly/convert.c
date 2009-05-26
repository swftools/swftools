#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../gfxdevice.h"
#include "../mem.h"
#include "poly.h"
#include "convert.h"

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

static inline int32_t convert_coord(double x, double z)
{
    /* we clamp to 31 bit instead of 32 bit because we use
       a (x1-x2) shortcut when comparing coordinates
    */
    x *= z;
    if(x < -0x40000000) x = -0x40000000;
    if(x >  0x3fffffff) x =  0x3fffffff;
    return ceil(x);
}

static void convert_gfxline(gfxline_t*line, polywriter_t*w, double gridsize)
{
    assert(!line || line[0].type == gfx_moveTo);
    double lastx=0,lasty=0;
    double z = 1.0 / gridsize;
    while(line) {
        if(line->type == gfx_moveTo) {
	    if(line->next && line->next->type != gfx_moveTo && (line->x!=lastx || line->y!=lasty)) {
		w->moveto(w, convert_coord(line->x,z), convert_coord(line->y,z));
	    }
        } else if(line->type == gfx_lineTo) {
            w->lineto(w, convert_coord(line->x,z), convert_coord(line->y,z));
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
		w->lineto(w, convert_coord(sx,z), convert_coord(sy,z));
	    }
	    w->lineto(w, convert_coord(line->x,z), convert_coord(line->y,z));
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

static void convert_file(const char*filename, polywriter_t*w, double gridsize)
{
    FILE*fi = fopen(filename, "rb");
    if(!fi) {
        perror(filename);
    }
    double z = 1.0 / gridsize;
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
		w->moveto(w, convert_coord(x,z), convert_coord(y,z));
                count++;
            } else if(s && !strcmp(s,"lineto")) {
		w->lineto(w, convert_coord(x,z), convert_coord(y,z));
                count++;
            } else {
                fprintf(stderr, "invalid command: %s\n", s);
            }
        } else if(sscanf(line, "%% gridsize %lf", &g) == 1) {
	    gridsize = g;
	    z = 1.0 / gridsize;
	    w->setgridsize(w, g);
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
} stdpoly_t;

static void stdmoveto(polywriter_t*w, int x, int y)
{
    stdpoly_t*d = (stdpoly_t*)w->internal;
    d->lastx = x;d->lasty = y;
}
static void stdlineto(polywriter_t*w, int x, int y)
{
    stdpoly_t*d = (stdpoly_t*)w->internal;
    int x1 = d->lastx;
    int y1 = d->lasty;
    int x2 = x;
    int y2 = y;
    if(x1!=x2 || y1!=y2) {
        edge_t*s = edge_new(x1, y1, x2, y2);
        s->next = d->poly->edges;
        d->poly->edges = s;
    }
    d->lastx = x;d->lasty = y;
}
static void stdsetgridsize(polywriter_t*w, double gridsize)
{
    stdpoly_t*d = (stdpoly_t*)w->internal;
    d->poly->gridsize = gridsize;
}
static void* stdfinish(polywriter_t*w)
{
    stdpoly_t*d = (stdpoly_t*)w->internal;
    gfxpoly_t*poly =  d->poly;
    free(w->internal);w->internal = 0;
    return poly;
}
void gfxpolywriter_init(polywriter_t*w)
{
    w->moveto = stdmoveto;
    w->lineto = stdlineto;
    w->setgridsize = stdsetgridsize;
    w->finish = stdfinish;
    stdpoly_t*data = w->internal = malloc(sizeof(stdpoly_t));
    data->poly = gfxpoly_new(1.0);
    data->lastx = 0;
    data->lasty = 0;
}

typedef struct _compactpoly {
    gfxcompactpoly_t*poly;
    point_t last;
    int strokes_size;
    point_t*points;
    int num_points;
    int points_size;
    segment_dir_t dir;
    char new;
} compactpoly_t;

void finish_segment(compactpoly_t*data)
{
    if(data->num_points <= 1)
	return;
    if(data->poly->num_strokes == data->strokes_size) {
	data->strokes_size <<= 1;
	assert(data->strokes_size > data->poly->num_strokes);
	data->poly->strokes = rfx_realloc(data->poly->strokes, sizeof(gfxpolystroke_t)*data->strokes_size);
    }
    point_t*p = malloc(sizeof(point_t)*data->num_points);
    gfxpolystroke_t*s = &data->poly->strokes[data->poly->num_strokes];
    s->num_points = data->num_points;
    s->dir = data->dir;
    s->points = p;
    assert(data->dir != DIR_UNKNOWN);
    if(data->dir == DIR_UP) {
	int t;
	int s = data->num_points;
	for(t=0;t<data->num_points;t++) {
	    p[--s] = data->points[t];
	}
    } else {
	memcpy(p, data->points, sizeof(point_t)*data->num_points);
    }
#ifdef CHECKS
    int t;
    for(t=0;t<data->num_points-1;t++) {
	assert(p[t].y<=p[t+1].y);
    }
#endif
    data->poly->num_strokes++;
}
static void compactmoveto(polywriter_t*w, int x, int y)
{
    compactpoly_t*data = (compactpoly_t*)w->internal;
    point_t p;
    p.x = x;
    p.y = y;
    if(p.x != data->last.x || p.y != data->last.y) {
	data->new = 1;
    }
    data->last = p;
}
static void compactlineto(polywriter_t*w, int x, int y)
{
    compactpoly_t*data = (compactpoly_t*)w->internal;
    point_t p;
    p.x = x;
    p.y = y;
    if(p.x == data->last.x && p.y == data->last.y)
	return;

    if(p.y < data->last.y && data->dir != DIR_UP ||
       p.y > data->last.y && data->dir != DIR_DOWN || 
       data->new) {
	finish_segment(data);
	data->dir = p.y > data->last.y ? DIR_DOWN : DIR_UP;
	data->points[0] = data->last;
	data->num_points = 1;
    }

    if(data->points_size == data->num_points) {
	data->points_size <<= 1;
	assert(data->points_size > data->num_points);
	data->points = rfx_realloc(data->points, sizeof(point_t)*data->points_size);
    }
    data->points[data->num_points++] = p;
    data->last = p;
}
static void compactsetgridsize(polywriter_t*w, double gridsize)
{
    compactpoly_t*d = (compactpoly_t*)w->internal;
    d->poly->gridsize = gridsize;
}
/*static int compare_stroke(const void*_s1, const void*_s2)
{
    gfxpolystroke_t*s1 = (gfxpolystroke_t*)_s1;
    gfxpolystroke_t*s2 = (gfxpolystroke_t*)_s2;
    return s1->points[0].y - s2->points[0].y;
}*/
static void*compactfinish(polywriter_t*w)
{
    compactpoly_t*data = (compactpoly_t*)w->internal;
    finish_segment(data);
    data->poly->strokes = (gfxpolystroke_t*)rfx_realloc(data->poly->strokes, sizeof(gfxpolystroke_t)*data->poly->num_strokes);
    //qsort(data->poly->strokes, data->poly->num_strokes, sizeof(gfxpolystroke_t), compare_stroke);
    free(data->points);
    gfxcompactpoly_t*poly = data->poly;
    free(w->internal);w->internal = 0;
    return (void*)poly;
}
void gfxcompactpolywriter_init(polywriter_t*w)
{
    w->moveto = compactmoveto;
    w->lineto = compactlineto;
    w->setgridsize = compactsetgridsize;
    w->finish = compactfinish;
    compactpoly_t*data = w->internal = rfx_calloc(sizeof(compactpoly_t));
    data->poly = rfx_calloc(sizeof(gfxcompactpoly_t));
    data->poly->gridsize = 1.0;
    data->last.x = data->last.y = 0;
    data->strokes_size = 16;
    data->num_points = 0;
    data->points_size = 16;
    data->new = 1;
    data->dir = DIR_UNKNOWN;
    data->points = (point_t*)rfx_alloc(sizeof(point_t)*data->points_size);
    data->poly->strokes = (gfxpolystroke_t*)rfx_alloc(sizeof(gfxpolystroke_t)*data->strokes_size);
}

gfxpoly_t* gfxpoly_from_gfxline(gfxline_t*line, double gridsize)
{
    polywriter_t w;
    gfxpolywriter_init(&w);
    w.setgridsize(&w, gridsize);
    convert_gfxline(line, &w, gridsize);
    return w.finish(&w);
}
gfxpoly_t* gfxpoly_from_file(const char*filename, double gridsize)
{
    polywriter_t w;
    gfxpolywriter_init(&w);
    w.setgridsize(&w, gridsize);
    convert_file(filename, &w, gridsize);
    return w.finish(&w);
}
gfxcompactpoly_t* gfxcompactpoly_from_gfxline(gfxline_t*line, double gridsize)
{
    polywriter_t writer;
    gfxcompactpolywriter_init(&writer);
    writer.setgridsize(&writer, gridsize);
    convert_gfxline(line, &writer, gridsize);
    return (gfxcompactpoly_t*)writer.finish(&writer);
}
gfxcompactpoly_t* gfxcompactpoly_from_file(const char*filename, double gridsize)
{
    polywriter_t writer;
    gfxcompactpolywriter_init(&writer);
    writer.setgridsize(&writer, gridsize);
    convert_file(filename, &writer, gridsize);
    return (gfxcompactpoly_t*)writer.finish(&writer);
}
gfxpoly_t*gfxpoly_from_gfxcompactpoly(gfxcompactpoly_t*poly)
{
    int s,t;
    int pass;
    gfxpoly_t*poly2 = gfxpoly_new(poly->gridsize);
    for(t=0;t<poly->num_strokes;t++) {
	gfxpolystroke_t*stroke = &poly->strokes[t];
	for(s=0;s<stroke->num_points-1;s++) {
	    point_t a = stroke->points[s];
	    point_t b = stroke->points[s+1];
	    edge_t*e = 0;
	    if(stroke->dir == DIR_UP) {
		e = edge_new(a.x,a.y,b.x,b.y);
	    } else {
		e = edge_new(b.x,b.y,a.x,a.y);
	    }
	    e->style = stroke->fs;
	    e->next = poly2->edges;
	    poly2->edges = e;
	}
    }
    return poly2;
}
void gfxcompactpoly_destroy(gfxcompactpoly_t*poly)
{
    int t;
    for(t=0;t<poly->num_strokes;t++) {
	free(poly->strokes[t].points);
	poly->strokes[t].points = 0;
    }
    free(poly->strokes);
    free(poly);
}

