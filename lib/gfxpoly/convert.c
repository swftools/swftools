#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../gfxdevice.h"
#include "../mem.h"
#include "poly.h"
#include "convert.h"
#include "wind.h"

/* factor that determines into how many line fragments a spline is converted */
#define SUBFRACTION (2.4)

static inline int32_t convert_coord(double x, double z)
{
    /* we clamp to 26 bit because: 
       a) we use a (x1-x2) shortcut when comparing coordinates
       b) we need to be able to multiply two coordinates and store them in a double w/o loss of precision
    */
    x *= z;
    if(x < -0x2000000) x = -0x2000000;
    if(x >  0x1ffffff) x =  0x1ffffff;
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
        if(sscanf(line, "%lf %lf %s", &x, &y, (char*)&s) == 3) {
            if(!strcmp(s,"moveto")) {
		w->moveto(w, convert_coord(x,z), convert_coord(y,z));
                count++;
            } else if(!strcmp(s,"lineto")) {
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

typedef struct _compactpoly {
    gfxpoly_t*poly;
    point_t last;
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
    point_t*p = malloc(sizeof(point_t)*data->num_points);
    gfxpolystroke_t*s = rfx_calloc(sizeof(gfxpolystroke_t));
    s->fs = &edgestyle_default;
    s->next = data->poly->strokes;
    data->poly->strokes = s;
    s->num_points = s->points_size = data->num_points;
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
}
static void compactmoveto(polywriter_t*w, int32_t x, int32_t y)
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

static inline int direction(point_t p1, point_t p2)
{
    int diff = p1.y - p2.y;
    if(diff) return diff;
    return p1.x - p2.x;
}

static void compactlineto(polywriter_t*w, int32_t x, int32_t y)
{
    compactpoly_t*data = (compactpoly_t*)w->internal;
    point_t p;
    p.x = x;
    p.y = y;

    int diff = direction(p, data->last);
    if(!diff)
	return;
    segment_dir_t dir = diff<0?DIR_UP:DIR_DOWN;

    if(dir!=data->dir || data->new) {
	finish_segment(data);
	data->dir = dir;
	data->points[0] = data->last;
	data->num_points = 1;
    }
    data->new = 0;

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
    //qsort(data->poly->strokes, data->poly->num_strokes, sizeof(gfxpolystroke_t), compare_stroke);
    free(data->points);
    gfxpoly_t*poly = data->poly;
    free(w->internal);w->internal = 0;
    return (void*)poly;
}
void gfxpolywriter_init(polywriter_t*w)
{
    w->moveto = compactmoveto;
    w->lineto = compactlineto;
    w->setgridsize = compactsetgridsize;
    w->finish = compactfinish;
    compactpoly_t*data = w->internal = rfx_calloc(sizeof(compactpoly_t));
    data->poly = rfx_calloc(sizeof(gfxpoly_t));
    data->poly->gridsize = 1.0;
    data->last.x = data->last.y = 0;
    data->num_points = 0;
    data->points_size = 16;
    data->new = 1;
    data->dir = DIR_UNKNOWN;
    data->points = (point_t*)rfx_alloc(sizeof(point_t)*data->points_size);
    data->poly->strokes = 0;
}

gfxpoly_t* gfxpoly_from_fill(gfxline_t*line, double gridsize)
{
    polywriter_t writer;
    gfxpolywriter_init(&writer);
    writer.setgridsize(&writer, gridsize);
    convert_gfxline(line, &writer, gridsize);
    return (gfxpoly_t*)writer.finish(&writer);
}
gfxpoly_t* gfxpoly_from_file(const char*filename, double gridsize)
{
    polywriter_t writer;
    gfxpolywriter_init(&writer);
    writer.setgridsize(&writer, gridsize);
    convert_file(filename, &writer, gridsize);
    return (gfxpoly_t*)writer.finish(&writer);
}
void gfxpoly_destroy(gfxpoly_t*poly)
{
    int t;
    gfxpolystroke_t*stroke = poly->strokes;
    while(stroke) {
	gfxpolystroke_t*next = stroke->next;
	free(stroke->points);
	free(stroke);
	stroke = next;
    }
    free(poly);
}

typedef struct _polydraw_internal
{
    double lx, ly;
    int32_t lastx, lasty;
    int32_t x0, y0;
    double z;
    char last;
    polywriter_t writer;
} polydraw_internal_t;

static void polydraw_moveTo(gfxdrawer_t*d, gfxcoord_t _x, gfxcoord_t _y)
{
    polydraw_internal_t*i = (polydraw_internal_t*)d->internal;
    int32_t x = convert_coord(_x, i->z);
    int32_t y = convert_coord(_y, i->z);
    if(i->lastx != x || i->lasty != y) {
	i->writer.moveto(&i->writer, x, y);
    }
    i->lx = _x;
    i->ly = _y;
    i->x0 = x;
    i->y0 = y;
    i->lastx = x;
    i->lasty = y;
    i->last = 1;
}
static void polydraw_lineTo(gfxdrawer_t*d, gfxcoord_t _x, gfxcoord_t _y)
{
    polydraw_internal_t*i = (polydraw_internal_t*)d->internal;
    if(!i->last) {
	polydraw_moveTo(d, _x, _y);
	return;
    }
    int32_t x = convert_coord(_x, i->z);
    int32_t y = convert_coord(_y, i->z);
    if(i->lastx != x || i->lasty != y) {
	i->writer.lineto(&i->writer, x, y);
    }
    i->lx = _x;
    i->ly = _y;
    i->lastx = x;
    i->lasty = y;
    i->last = 1;
}
static void polydraw_splineTo(gfxdrawer_t*d, gfxcoord_t sx, gfxcoord_t sy, gfxcoord_t x, gfxcoord_t y)
{
    polydraw_internal_t*i = (polydraw_internal_t*)d->internal;
    if(!i->last) {
	polydraw_moveTo(d, x, y);
	return;
    }
    double c = fabs(x-2*sx+i->lx) + fabs(y-2*sy+i->ly);
    int parts = (int)(sqrt(c)*SUBFRACTION);
    if(!parts) parts = 1;
    int t;
    int32_t nx,ny;
    for(t=0;t<parts;t++) {
	nx = convert_coord((double)(t*t*x + 2*t*(parts-t)*sx + (parts-t)*(parts-t)*i->lx)/(double)(parts*parts), i->z);
	ny = convert_coord((double)(t*t*y + 2*t*(parts-t)*sy + (parts-t)*(parts-t)*i->ly)/(double)(parts*parts), i->z);
	if(nx != i->lastx || ny != i->lasty) {
	    i->writer.lineto(&i->writer, nx, ny);
	    i->lastx = nx; i->lasty = ny;
	}
    }
    nx = convert_coord(x,i->z);
    ny = convert_coord(y,i->z);
    if(nx != i->lastx || ny != i->lasty) {
	i->writer.lineto(&i->writer, nx, ny);
    }
    i->lx = x;
    i->ly = y;
    i->lastx = nx; 
    i->lasty = ny;
    i->last = 1;
}
static void polydraw_close(gfxdrawer_t*d)
{
    polydraw_internal_t*i = (polydraw_internal_t*)d->internal;
    assert(!(i->last && (i->x0 == INVALID_COORD || i->y0 == INVALID_COORD)));
    if(!i->last)
	return;
    if(i->lastx != i->x0 || i->lasty != i->y0) {
	i->writer.lineto(&i->writer, i->x0, i->y0);
	i->lastx = i->x0;
	i->lasty = i->y0;
    }
    i->last = 0;
    i->x0 = INVALID_COORD;
    i->y0 = INVALID_COORD;
}
static void* polydraw_result(gfxdrawer_t*d)
{
    polydraw_internal_t*i = (polydraw_internal_t*)d->internal;
    void*result = i->writer.finish(&i->writer);
    rfx_free(i);
    memset(d, 0, sizeof(gfxdrawer_t));
    return result;
}

void gfxdrawer_target_poly(gfxdrawer_t*d, double gridsize)
{
    polydraw_internal_t*i = (polydraw_internal_t*)rfx_calloc(sizeof(polydraw_internal_t));
    d->internal = i;
    i->lastx = INVALID_COORD; // convert_coord can never return this value
    i->lasty = INVALID_COORD;
    i->x0 = INVALID_COORD;
    i->y0 = INVALID_COORD;
    d->moveTo = polydraw_moveTo;
    d->lineTo = polydraw_lineTo;
    d->splineTo = polydraw_splineTo;
    d->close = polydraw_close;
    d->result = polydraw_result;
    gfxpolywriter_init(&i->writer);
    i->writer.setgridsize(&i->writer, gridsize);
    i->z = 1.0 / gridsize;
}

#if 0
gfxline_t*gfxline_from_gfxpoly(gfxpoly_t*poly)
{
    gfxpolystroke_t*stroke;
    int count = 0;
    for(stroke=poly->strokes;stroke;stroke=stroke->next) {
	assert(stroke->num_points);
	count += stroke->num_points;
    }
    if(!count) return 0;
    gfxline_t*l = malloc(sizeof(gfxline_t)*count);
    count = 0;
    /* TODO: it might make sense to concatenate strokes */
    for(stroke=poly->strokes;stroke;stroke=stroke->next) {
	int t;
	for(t=0;t<stroke->num_points;t++) {
	    l[count+t].x = stroke->points[t].x * poly->gridsize;
	    l[count+t].y = stroke->points[t].y * poly->gridsize;
	    l[count+t].type = gfx_lineTo;
	    l[count+t].next = &l[count+t+1];
	}
	l[count].type = gfx_moveTo;
	count+=stroke->num_points;
    }
    l[count-1].next = 0;
    return l;
}
#endif

static gfxline_t*mkgfxline(gfxpoly_t*poly, char preserve_direction)
{
    gfxpolystroke_t*stroke;
    int count = 0;
    if(!poly->strokes)
	return 0;
    dict_t*d = dict_new2(&point_type);
    dict_t*todo = dict_new2(&ptr_type);
    gfxpolystroke_t*stroke_min= poly->strokes;
    int32_t x_min=stroke_min->points[0].x;
    int32_t y_min=stroke_min->points[0].y;
    for(stroke=poly->strokes;stroke;stroke=stroke->next) {
	dict_put(todo, stroke, stroke);
	assert(stroke->num_points>1);
	count += stroke->num_points;
	if(stroke->dir == DIR_UP) {
	    dict_put(d, &stroke->points[stroke->num_points-1], stroke);
	    if(!preserve_direction)
		dict_put(d, &stroke->points[0], stroke);
	} else {
	    dict_put(d, &stroke->points[0], stroke);
	    if(!preserve_direction)
		dict_put(d, &stroke->points[stroke->num_points-1], stroke);
	}
	if(stroke->points[0].y < y_min ||
	   (stroke->points[0].y == y_min && stroke->points[0].x < x_min)) {
	    y_min = stroke->points[0].y;
	    stroke_min = stroke;
	}
    }
    gfxpolystroke_t*next_todo = poly->strokes;
    gfxline_t*l = malloc(sizeof(gfxline_t)*count);
    count = 0;
    stroke = stroke_min;
    
    point_t last = {INVALID_COORD, INVALID_COORD};
    char should_connect = 0;
    while(stroke) {
	if(stroke && !preserve_direction) {
	    char del1 = dict_del2(d, &stroke->points[0], stroke);
	    char del2 = dict_del2(d, &stroke->points[stroke->num_points-1], stroke);
	    assert(del1 && del2);
	}
	assert(dict_contains(todo, stroke));
	int t;
	int pos = 0;
	int incr = 1;
	if(preserve_direction) {
	    if(stroke->dir == DIR_UP) {
		pos = stroke->num_points-1;
		incr = -1;
	    }
	} else {
	    // try to find matching point on either end.
	    // Prefer downward.
	    if(last.x == stroke->points[stroke->num_points-1].x &&
	       last.y == stroke->points[stroke->num_points-1].y) {
		pos = stroke->num_points-1;
		incr = -1;
	    }
	}
	if(last.x != stroke->points[pos].x || last.y != stroke->points[pos].y) {
	    l[count].x = stroke->points[pos].x * poly->gridsize;
	    l[count].y = stroke->points[pos].y * poly->gridsize;
	    l[count].type = gfx_moveTo;
	    l[count].next = &l[count+1];
	    count++;
	    assert(!should_connect);
	}
	pos += incr;
	for(t=1;t<stroke->num_points;t++) {
	    l[count].x = stroke->points[pos].x * poly->gridsize;
	    l[count].y = stroke->points[pos].y * poly->gridsize;
	    l[count].type = gfx_lineTo;
	    l[count].next = &l[count+1];
	    count++;
	    pos += incr;
	}
	last = stroke->points[pos-incr];
	char del = dict_del(todo, stroke);
	assert(del);
	assert(!dict_contains(todo, stroke));

	/* try to find a poly which starts at the point we drew last */
	stroke = dict_lookup(d, &last);
	should_connect = 1;
	while(!dict_contains(todo, stroke)) {
	    should_connect = 0;
	    stroke = next_todo;
	    if(!next_todo) {
		stroke = 0;
		break;
	    }
	    next_todo = next_todo->next;
	}
    }
    l[count-1].next = 0;
    dict_destroy(todo);
    dict_destroy(d);
    return l;
}

gfxline_t*gfxline_from_gfxpoly(gfxpoly_t*poly)
{
    return mkgfxline(poly, 0);
}

gfxline_t*gfxline_from_gfxpoly_with_direction(gfxpoly_t*poly)
{
    return mkgfxline(poly, 1);
}

static windcontext_t onepolygon = {1};
gfxline_t* gfxpoly_circular_to_evenodd(gfxline_t*line, double gridsize)
{
    gfxpoly_t*poly = gfxpoly_from_fill(line, gridsize);
    gfxpoly_t*poly2 = gfxpoly_process(poly, 0, &windrule_circular, &onepolygon, 0);
    gfxline_t*line2 = gfxline_from_gfxpoly(poly2);
    gfxpoly_destroy(poly);
    gfxpoly_destroy(poly2);
    return line2;
}

gfxpoly_t* gfxpoly_createbox(double x1, double y1,double x2, double y2, double gridsize)
{
    gfxline_t* line = gfxline_makerectangle(x1, y1, x2, y2);
    gfxpoly_t* poly = gfxpoly_from_fill(line, gridsize);
    gfxline_free(line);
    return poly;
}

