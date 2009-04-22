#ifndef __poly_h__
#define __poly_h__

#include <stdint.h>
#include "../q.h"

typedef enum {DIR_UP, DIR_DOWN} segment_dir_t;
typedef enum {EVENT_CROSS, EVENT_END, EVENT_START} eventtype_t;
typedef enum {SLOPE_POSITIVE, SLOPE_NEGATIVE} slope_t;

typedef struct _point {
    int32_t x;
    int32_t y;
} point_t;

typedef struct _segment {
    point_t a;
    point_t b;
    point_t delta;
    double k; //k = a.x*b.y-a.y*b.x = delta.y*a.x - delta.x*a.y (=0 for points on the segment)
    segment_dir_t dir;
    int nr;
    struct _segment*left;
    struct _segment*right;
    int tmp;
    point_t pos;

    dict_t scheduled_crossings;
} segment_t;

#define LINE_EQ(p,s) ((double)(s)->delta.y*(p).x - (double)(s)->delta.x*(p).y - (s)->k)

typedef segment_t gfxpoly_t;
gfxpoly_t* gfxpoly_new();
void gfxpoly_dump(gfxpoly_t*poly);

typedef struct _event {
    eventtype_t type;
    point_t p;
    segment_t*s1;
    segment_t*s2;
} event_t;

void segment_init(segment_t*s, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
segment_t*segment_new(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void segment_insert_before(segment_t**list, segment_t*add);

#endif
