#ifndef __poly_h__
#define __poly_h__

#include <stdint.h>
#include "../q.h"
#include "../types.h"
#include "wind.h"

/* features */
#define SPLAY
#define DONT_REMEMBER_CROSSINGS

typedef enum {EVENT_CROSS, EVENT_END, EVENT_START, EVENT_HORIZONTAL} eventtype_t;
typedef enum {SLOPE_POSITIVE, SLOPE_NEGATIVE} slope_t;

#define INVALID_COORD (0x7fffffff)
typedef struct _point {
    int32_t x;
    int32_t y;
} point_t;
type_t point_type;

#define SEGNR(s) ((int)((s)?(s)->nr:-1))

typedef struct _gfxpolystroke {
    segment_dir_t dir;
    edgestyle_t*fs;
    int points_size;
    int num_points;
    point_t*points;
    struct _gfxpolystroke*next;
} gfxpolystroke_t;
typedef struct _gfxpoly {
    double gridsize;
    gfxpolystroke_t*strokes;
} gfxpoly_t;

typedef struct _segment {
    point_t a;
    point_t b;
    point_t delta;
    double k; //k = a.x*b.y-a.y*b.x = delta.y*a.x - delta.x*a.y (=0 for points on the segment)
    int32_t minx, maxx;
    
    segment_dir_t dir;
    edgestyle_t*fs;
    edgestyle_t*fs_out;
#ifdef CHECKS
    char fs_out_ok;
#endif
    
    int polygon_nr;
    windstate_t wind;
    ptroff_t nr;

#ifdef SPLAY
    struct _segment*parent;
    struct _segment*leftchild;
    struct _segment*rightchild;
#endif
    struct _segment*left;
    struct _segment*right;
    char changed;

    point_t pos;

    gfxpolystroke_t*stroke;
    int stroke_pos;

#ifndef DONT_REMEMBER_CROSSINGS
    dict_t scheduled_crossings;
#endif
} segment_t;

typedef struct _moments {
    double area;
    double m[3][3];
} moments_t;

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

void gfxpoly_fail(char*expr, char*file, int line, const char*function);

char gfxpoly_check(gfxpoly_t*poly, char updown);
int gfxpoly_num_segments(gfxpoly_t*poly);
int gfxpoly_size(gfxpoly_t*poly);
void gfxpoly_dump(gfxpoly_t*poly);
void gfxpoly_save(gfxpoly_t*poly, const char*filename);
void gfxpoly_save_arrows(gfxpoly_t*poly, const char*filename);
gfxpoly_t* gfxpoly_process(gfxpoly_t*poly1, gfxpoly_t*poly2, windrule_t*windrule, windcontext_t*context, moments_t*moments);

gfxpoly_t* gfxpoly_intersect(gfxpoly_t*p1, gfxpoly_t*p2);
gfxpoly_t* gfxpoly_union(gfxpoly_t*p1, gfxpoly_t*p2);
double gfxpoly_area(gfxpoly_t*p);
double gfxpoly_intersection_area(gfxpoly_t*p1, gfxpoly_t*p2);

#ifndef CHECKS
#ifdef assert
#undef assert
#endif
#define assert(x)
#else
#define assert(x) ((x)?0:gfxpoly_fail(__STRING(x), __FILE__, __LINE__, __PRETTY_FUNCTION__))
#endif


#endif
