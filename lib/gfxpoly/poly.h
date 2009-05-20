#ifndef __poly_h__
#define __poly_h__

#include <stdint.h>
#include "../q.h"

//#define DEBUG
#define CHECKS
#define SPLAY

typedef enum {DIR_UP, DIR_DOWN} segment_dir_t;
typedef enum {EVENT_CROSS, EVENT_END, EVENT_CORNER, EVENT_START, EVENT_HORIZONTAL} eventtype_t;
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
#ifdef DEBUG
    int tmp;
#endif
    struct _edge *next;
} edge_t;

typedef struct _windstate
{
    char is_filled;
    int wind_nr;
} windstate_t;

/* TODO: maybe we should merge windcontext and windrule */
typedef struct _windcontext
{
    int num_polygons;
} windcontext_t;

typedef struct _windrule
{
    windstate_t (*start)(windcontext_t* num_polygons);
    windstate_t (*add)(windcontext_t*context, windstate_t left, fillstyle_t*edge, segment_dir_t dir, int polygon_nr);
    fillstyle_t* (*diff)(windstate_t*left, windstate_t*right);
} windrule_t;

#define SEGNR(s) ((s)?(s)->nr:-1)

typedef struct _segment {
    point_t a;
    point_t b;
    point_t delta;
    double k; //k = a.x*b.y-a.y*b.x = delta.y*a.x - delta.x*a.y (=0 for points on the segment)
    int32_t minx, maxx;
    
    segment_dir_t dir;
    fillstyle_t*fs;
    fillstyle_t*fs_out;
#ifdef CHECKS
    char fs_out_ok;
#endif
    
    int polygon_nr;
    windstate_t wind;
    int nr;

#ifdef SPLAY
    struct _segment*parent;
    struct _segment*leftchild;
    struct _segment*rightchild;
#endif
    struct _segment*left;
    struct _segment*right;
    char changed;

    point_t pos;

    dict_t scheduled_crossings;
} segment_t;

#define LINE_EQ(p,s) ((double)(s)->delta.y*(p).x - (double)(s)->delta.x*(p).y - (s)->k)

/* x1 + ((x2-x1)*(y-y1)) / dy = 
   (x1*(y2-y1) + (x2-x1)*(y-y1)) / dy =
   (x1*(y2-y)  +  x2    *(y-y1)) / dy =
   (x1*y2 - x2*y1 + x2*y - y*x1) / dy =
   (k + x2*y - x1*y) / dy
   (k + dx*y) / dy
*/
//#define XPOS(s,ypos) ((s)->a.x + ((s)->delta.x * (double)((ypos) - (s)->a.y)) / (s)->delta.y)
#define XPOS(s,ypos) (((s)->k + (double)(s)->delta.x*ypos) / (s)->delta.y)

#define XPOS_INT(s,ypos) ((int)ceil(XPOS((s),ypos)))
#define XDIFF(s1,s2,ypos) (((s1)->k + (double)(s1)->delta.x*ypos)*(s2)->delta.y - \
                           ((s2)->k + (double)(s2)->delta.x*ypos)*(s1)->delta.y)

typedef struct _gfxpoly {
    double gridsize;
    edge_t*edges;
} gfxpoly_t;

void gfxpoly_fail(char*expr, char*file, int line, const char*function);

gfxpoly_t* gfxpoly_new(double gridsize);
char gfxpoly_check(gfxpoly_t*poly);
int gfxpoly_size(gfxpoly_t*poly);
void gfxpoly_dump(gfxpoly_t*poly);
gfxpoly_t* gfxpoly_save(gfxpoly_t*poly, const char*filename);
gfxpoly_t* gfxpoly_process(gfxpoly_t*poly, windrule_t*windrule, windcontext_t*context);

typedef struct _event {
    eventtype_t type;
    point_t p;
    segment_t*s1;
    segment_t*s2;
} event_t;

#ifndef CHECKS
#ifdef assert
#undef assert
#endif
#define assert(x)
#else
#define assert(x) ((x)?0:gfxpoly_fail(__STRING(x), __FILE__, __LINE__, __PRETTY_FUNCTION__))
#endif


#endif
