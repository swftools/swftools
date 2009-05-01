#ifndef __poly_h__
#define __poly_h__

#include <stdint.h>
#include "../q.h"

typedef enum {DIR_UP, DIR_DOWN} segment_dir_t;
typedef enum {EVENT_CROSS, EVENT_END, EVENT_HORIZONTAL, EVENT_START} eventtype_t;
typedef enum {SLOPE_POSITIVE, SLOPE_NEGATIVE} slope_t;

typedef struct _point {
    int32_t x;
    int32_t y;
} point_t;

typedef struct _fillstyle {
    char is_filled;
} fillstyle_t;

typedef struct _edge {
    point_t a;
    point_t b;
    fillstyle_t*style;
    struct _edge *next;
} edge_t;

typedef struct _windstate
{
    char is_filled;
    int wind_nr;
    int num_polygons;
} windstate_t;

typedef struct _windrule
{
    windstate_t (*start)(int num_polygons);
    windstate_t (*add)(windstate_t left, fillstyle_t*edge, segment_dir_t dir, int polygon_nr);
    fillstyle_t* (*diff)(windstate_t*left, windstate_t*right);
} windrule_t;

typedef struct _segment {
    point_t a;
    point_t b;
    point_t delta;
    double k; //k = a.x*b.y-a.y*b.x = delta.y*a.x - delta.x*a.y (=0 for points on the segment)
    
    segment_dir_t dir;
    fillstyle_t*fs;
    fillstyle_t*fs_out;
    char fs_out_ok;
    
    int polygon_nr;
    windstate_t wind;
    int nr;

    struct _segment*left;
    struct _segment*right;
    
    point_t pos;

    dict_t scheduled_crossings;
} segment_t;

#define LINE_EQ(p,s) ((double)(s)->delta.y*(p).x - (double)(s)->delta.x*(p).y - (s)->k)
#define XPOS(s,ypos) ((s)->a.x + ceil(((s)->delta.x * (double)((ypos) - (s)->a.y)) / (s)->delta.y))

typedef struct _gfxpoly {
    double gridsize;
    edge_t*edges;
} gfxpoly_t;

gfxpoly_t* gfxpoly_new(double gridsize);
char gfxpoly_check(gfxpoly_t*poly);
void gfxpoly_dump(gfxpoly_t*poly);
gfxpoly_t* gfxpoly_process(gfxpoly_t*poly, windrule_t*windrule);

typedef struct _event {
    eventtype_t type;
    point_t p;
    segment_t*s1;
    segment_t*s2;
} event_t;

#endif
