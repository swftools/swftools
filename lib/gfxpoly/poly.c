#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "../mem.h"
#include "../types.h"
#include "../q.h"
#include "poly.h"
#include "active.h"
#include "xrow.h"
#include "wind.h"

static gfxpoly_t*current_polygon = 0;
void gfxpoly_fail(char*expr, char*file, int line, const char*function)
{
    fprintf(stderr, "assert(%s) failed in %s in line %d: %s\n", expr, file, line, function);
    fprintf(stderr, "I'm saving a debug file \"poly.ps\" to the current directory.\n");
    gfxpoly_save(current_polygon, "poly.ps");
    exit(1);
}

char point_equals(const void*o1, const void*o2)
{
    const point_t*p1 = o1;
    const point_t*p2 = o2;
    return p1->x == p2->x && p1->y == p2->y;
}
unsigned int point_hash(const void*o)
{
    const point_t*p = o;
    return p->x^p->y;
}
void* point_dup(const void*o)
{
    const point_t*p = o;
    point_t*n = malloc(sizeof(point_t));
    n->x = p->x;
    n->y = p->y;
    return n;
}
void point_free(void*o)
{
    point_t*p = o;
    p->x = 0;
    p->y = 0;
    free(p);
}
type_t point_type = {
    equals: point_equals,
    hash: point_hash,
    dup: point_dup,
    free: point_free,
};

typedef struct _status {
    int y;
    int num_polygons;
    actlist_t*actlist;
    heap_t*queue;
    edge_t*output;
    xrow_t*xrow;
    windrule_t*windrule;
    segment_t*ending_segments;
#ifdef CHECKS
    dict_t*seen_crossings; //list of crossing we saw so far
    dict_t*intersecting_segs; //list of segments intersecting in this scanline
    dict_t*segs_with_point; //lists of segments that received a point in this scanline
#endif
} status_t;

/* compare_events_simple differs from compare_events in that it schedules
   events from left to right regardless of type. It's only used in horizontal
   processing, in order to get an x-wise sorting of the current scanline */
static int compare_events_simple(const void*_a,const void*_b)
{
    event_t* a = (event_t*)_a;
    event_t* b = (event_t*)_b;
    if(a->p.y < b->p.y) {
        return 1;
    } else if(a->p.y > b->p.y) {
        return -1;
    } else if(a->p.x < b->p.x) {
        return 1;
    } else if(a->p.x > b->p.x) {
        return -1;
    } else
        return 0;
}

static int compare_events(const void*_a,const void*_b)
{
    event_t* a = (event_t*)_a;
    event_t* b = (event_t*)_b;
    int d = b->p.y - a->p.y;
    if(d) return d;
    /* we need to schedule end before intersect (so that a segment about
       to end has a chance to tear up a few other segs first) and start
       events after intersect (so that start segments don't position themselves
       between two segments about to intersect (not a problem as such, but makes
       things slower)). Horizontal lines come last, because the only purpose
       they have is to create snapping coordinates for the segments (still)
       existing in this scanline */
    d = b->type - a->type;
    if(d) return d;
    d = b->p.x - a->p.x;
    return d;
}

gfxpoly_t* gfxpoly_new(double gridsize)
{
    gfxpoly_t*p = (gfxpoly_t*)rfx_calloc(sizeof(gfxpoly_t));
    p->gridsize = gridsize;
    return p;
}
void gfxpoly_destroy(gfxpoly_t*poly)
{
    edge_t* s = poly->edges;
    while(s) {
        edge_t*next  = s->next;
        free(s);
        s = next;
    }
    free(poly);
}
int gfxpoly_size(gfxpoly_t*poly)
{
    edge_t* s = poly->edges;
    int t=0;
    while(s) {
        s = s->next;t++;
    }
    return t;
}
char gfxpoly_check(gfxpoly_t*poly)
{
    edge_t* s = poly->edges;
    dict_t*d = dict_new2(&point_type);
    while(s) {
        if(!dict_contains(d, &s->a)) {
            dict_put(d, &s->a, (void*)(ptroff_t)1);
        } else {
            int count = (ptroff_t)dict_lookup(d, &s->a);
            dict_del(d, &s->a);
            count++;
            dict_put(d, &s->a, (void*)(ptroff_t)count);
        }
        if(!dict_contains(d, &s->b)) {
            dict_put(d, &s->b, (void*)(ptroff_t)1);
        } else {
            int count = (ptroff_t)dict_lookup(d, &s->b);
            dict_del(d, &s->b);
            count++;
            dict_put(d, &s->b, (void*)(ptroff_t)count);
        }
        s = s->next;
    }
    DICT_ITERATE_ITEMS(d, point_t*, p, void*, c) {
        int count = (ptroff_t)c;
        if(count&1) {
            fprintf(stderr, "Point (%f,%f) occurs %d times\n", p->x*poly->gridsize, p->y*poly->gridsize, count);
            dict_destroy(d);
            return 0;
        }
    }
    dict_destroy(d);
    return 1;
}

void gfxpoly_dump(gfxpoly_t*poly)
{
    edge_t* s = poly->edges;
    double g = poly->gridsize;
    while(s) {
        fprintf(stderr, "(%f,%f) -> (%f,%f)\n", s->a.x*g, s->a.y*g, s->b.x*g, s->b.y*g);
        s = s->next;
    }
}

gfxpoly_t* gfxpoly_save(gfxpoly_t*poly, const char*filename)
{
    FILE*fi = fopen(filename, "wb");
    fprintf(fi, "%% gridsize %f\n", poly->gridsize);
    fprintf(fi, "%% begin\n");
    edge_t* s = poly->edges;
    while(s) {
        fprintf(fi, "%g setgray\n", s->b.y < s->a.y ? 0.7 : 0);
        fprintf(fi, "%d %d moveto\n", s->a.x, s->a.y);
        fprintf(fi, "%d %d lineto\n", s->b.x, s->b.y);
        fprintf(fi, "stroke\n");
        s = s->next;
    }
    fprintf(fi, "showpage\n");
    fclose(fi);
}

inline static event_t event_new()
{
    event_t e;
    memset(&e, 0, sizeof(e));
    return e;
}

void event_dump(event_t*e)
{
    if(e->type == EVENT_HORIZONTAL) {
        fprintf(stderr, "Horizontal [%d] (%d,%d) -> (%d,%d)\n", e->s1->nr, e->s1->a.x, e->s1->a.y, e->s1->b.x, e->s1->b.y);
    } else if(e->type == EVENT_START) {
        fprintf(stderr, "event: segment [%d] starts at (%d,%d)\n", e->s1->nr, e->p.x, e->p.y);
    } else if(e->type == EVENT_END) {
        fprintf(stderr, "event: segment [%d] ends at (%d,%d)\n", e->s1->nr, e->p.x, e->p.y);
    } else if(e->type == EVENT_CROSS) {
        fprintf(stderr, "event: segment [%d] and [%d] intersect at (%d,%d)\n", e->s1->nr, e->s2->nr, e->p.x, e->p.y);
    } else {
        assert(0);
    }
}

static inline max32(int32_t v1, int32_t v2) {return v1>v2?v1:v2;}
static inline min32(int32_t v1, int32_t v2) {return v1<v2?v1:v2;}

void segment_dump(segment_t*s)
{
    fprintf(stderr, "(%d,%d)->(%d,%d) ", s->a.x, s->a.y, s->b.x, s->b.y);
    fprintf(stderr, " dx:%d dy:%d k:%f dx/dy=%f\n", s->delta.x, s->delta.y, s->k,
            (double)s->delta.x / s->delta.y);
}

void segment_init(segment_t*s, int x1, int y1, int x2, int y2, windstate_t windstate, int polygon_nr)
{
    if(y1<y2) {
        s->dir = DIR_DOWN;
    } else if(y1>y2) {
        int x = x1;x1=x2;x2=x;
        int y = y1;y1=y2;y2=y;
        s->dir = DIR_UP;
    } else {
        /* up/down for horizontal segments is handled by "rotating"
           them 90° anticlockwise in screen coordinates (tilt your head to
           the right) */
        s->dir = DIR_UP;
        if(x1>x2) {
            s->dir = DIR_DOWN;
            int x = x1;x1=x2;x2=x;
            int y = y1;y1=y2;y2=y;
        }
    }
    s->a.x = x1;
    s->a.y = y1;
    s->b.x = x2;
    s->b.y = y2;
    s->k = (double)x1*y2-(double)x2*y1;
    s->left = s->right = 0;
    s->delta.x = x2-x1;
    s->delta.y = y2-y1;
    s->minx = min32(x1,x2);
    s->maxx = max32(x1,x2);

    s->pos = s->a;
    s->polygon_nr = polygon_nr;
#define XDEBUG
#ifdef XDEBUG
    static int segment_count=0;
    s->nr = segment_count++;
#endif

    assert(LINE_EQ(s->a, s) == 0);
    assert(LINE_EQ(s->b, s) == 0);

    /* check that all signs are in order:
       a        a
       |\      /|
       | \    / |
     minx-b  b--maxx
     < 0        > 0
    */
    point_t p = s->b;
    p.x = min32(s->a.x, s->b.x);
    assert(LINE_EQ(p, s) <= 0);
    p.x = max32(s->a.x, s->b.x);
    assert(LINE_EQ(p, s) >= 0);

    dict_init2(&s->scheduled_crossings, &ptr_type, 0);
}

segment_t* segment_new(int32_t x1, int32_t y1, int32_t x2, int32_t y2, windstate_t initial, int polygon_nr)
{
    segment_t*s = (segment_t*)rfx_calloc(sizeof(segment_t));
    segment_init(s, x1, y1, x2, y2, initial, polygon_nr);
    return s;
}
void segment_destroy(segment_t*s)
{
    dict_clear(&s->scheduled_crossings);
    free(s);
}

void gfxpoly_enqueue(edge_t*list, heap_t*queue, windstate_t initial, int polygon_nr)
{
    edge_t*l;
    for(l=list;l;l=l->next) {
        if(l->a.x == l->b.x &&
           l->a.y == l->b.y) {
            fprintf(stderr, "Warning: intersector input contains zero-length segments\n");
            continue;
        }
        segment_t*s = segment_new(l->a.x, l->a.y, l->b.x, l->b.y, initial, polygon_nr);
#ifdef DEBUG
        if(l->tmp)
            s->nr = l->tmp;
        fprintf(stderr, "[%d] (%d,%d) -> (%d,%d) %s\n",
                s->nr, s->a.x, s->a.y, s->b.x, s->b.y,
                s->dir==DIR_UP?"up":"down");
#endif
        event_t e = event_new();
        e.type = s->delta.y ? EVENT_START : EVENT_HORIZONTAL;
        e.p = s->a;
        e.s1 = s;
        e.s2 = 0;
        heap_put(queue, &e);
    }
}

void schedule_endpoint(status_t*status, segment_t*s)
{
    // schedule end point of segment
    assert(s->b.y > status->y);
    event_t e;
    e.type = EVENT_END;
    e.p = s->b;
    e.s1 = s;
    e.s2 = 0;
    heap_put(status->queue, &e);
}

void schedule_crossing(status_t*status, segment_t*s1, segment_t*s2)
{
    /* the code that's required (and the checks you can perform) before
       it can be said with 100% certainty that we indeed have a valid crossing
       amazes me every time. -mk */

#ifdef CHECKS
    assert(s1!=s2);
    assert(s1->right == s2);
    assert(s2->left == s1);
    int32_t miny1 = min32(s1->a.y,s1->b.y);
    int32_t maxy1 = max32(s1->a.y,s1->b.y);
    int32_t miny2 = min32(s2->a.y,s2->b.y);
    int32_t maxy2 = max32(s2->a.y,s2->b.y);
    int32_t minx1 = min32(s1->a.x,s1->b.x);
    int32_t minx2 = min32(s2->a.x,s2->b.x);
    int32_t maxx1 = max32(s1->a.x,s1->b.x);
    int32_t maxx2 = max32(s2->a.x,s2->b.x);
    /* check that precomputation is sane */
    assert(minx1 == s1->minx && minx2 == s2->minx);
    assert(maxx1 == s1->maxx && maxx2 == s2->maxx);
    /* both segments are active, so this can't happen */
    assert(!(maxy1 <= miny2 || maxy2 <= miny1));
    /* we know that right now, s2 is to the right of s1, so there's
       no way the complete bounding box of s1 is to the right of s1 */
    assert(!(s1->minx > s2->maxx));
    assert(s1->minx != s2->maxx || (!s1->delta.x && !s2->delta.x));
#endif

    if(s1->maxx <= s2->minx) {
        /* bounding boxes don't intersect */
        return;
    }

    if(dict_contains(&s1->scheduled_crossings, s2)) {
        /* FIXME: this whole segment hashing thing is really slow */
        //fprintf(stderr, "Encountered crossing between [%d] and [%d] twice\n", s1->nr, s2->nr);
        return; // we already know about this one
    }

    double adx = s1->delta.x;
    double ady = s1->delta.y;
    double bdx = s2->delta.x;
    double bdy = s2->delta.y;
    double det = adx*bdy - ady*bdx;
    if(!det) {
        if(s1->k == s2->k) {
            // lines are exactly on top of each other (ignored)
#ifdef DEBUG
            fprintf(stderr, "Notice: segments [%d] and [%d] are exactly on top of each other\n", s1->nr, s2->nr);
#endif
            return;
        } else {
            /* lines are parallel */
            return;
        }
    }
    double asign2 = LINE_EQ(s1->a, s2);
    double bsign2 = LINE_EQ(s1->b, s2);
    if(asign2<0 && bsign2<0) {
        // segment1 is completely to the left of segment2
        return;
    }
    if(asign2>0 && bsign2>0)  {
        // segment2 is completely to the left of segment1
        return;
    }
    if(asign2==0) {
        // segment1 touches segment2 in a single point (ignored)
#ifdef DEBUG
        fprintf(stderr, "Notice: segment [%d]'s start point touches segment [%d]\n", s1->nr, s2->nr);
#endif
        return;
    }
    if(bsign2==0) {
        // segment1 touches segment2 in a single point (ignored)
#ifdef DEBUG
        fprintf(stderr, "Notice: segment [%d]'s end point touches segment [%d]\n", s1->nr, s2->nr);
#endif
        return;
    }
    double asign1 = LINE_EQ(s2->a, s1);
    double bsign1 = LINE_EQ(s2->b, s1);
    if(asign1<0 && bsign1<0) {
        // segment1 is completely to the left of segment2
        return;
    }
    if(asign1>0 && bsign1>0)  {
        // segment2 is completely to the left of segment1
        return;
    }
    if(asign1==0) {
        // segment2 touches segment1 in a single point (ignored)
#ifdef DEBUG
        fprintf(stderr, "Notice: segment [%d]'s start point touches segment [%d]\n", s2->nr, s1->nr);
#endif
        return;
    }
    if(asign2==0) {
        // segment2 touches segment1 in a single point (ignored)
#ifdef DEBUG
        fprintf(stderr, "Notice: segment [%d]'s end point touches segment [%d]\n", s2->nr, s1->nr);
#endif
        return;
    }

    double la = (double)s1->a.x*(double)s1->b.y - (double)s1->a.y*(double)s1->b.x;
    double lb = (double)s2->a.x*(double)s2->b.y - (double)s2->a.y*(double)s2->b.x;

    point_t p;
    p.x = (int32_t)ceil((-la*bdx +lb*adx) / det);
    p.y = (int32_t)ceil((+lb*ady -la*bdy) / det);

    assert(p.y >= status->y);
#ifdef CHECKS
    assert(p.x >= s1->minx && p.x <= s1->maxx);
    assert(p.x >= s2->minx && p.x <= s2->maxx);

    point_t pair;
    pair.x = s1->nr;
    pair.y = s2->nr;
    assert(!dict_contains(status->seen_crossings, &pair));
    dict_put(status->seen_crossings, &pair, 0);
#endif
#ifdef DEBUG
    fprintf(stderr, "schedule crossing between [%d] and [%d] at (%d,%d)\n", s1->nr, s2->nr, p.x, p.y);
#endif

    /* we insert into each other's intersection history because these segments might switch
       places and we still want to look them up quickly after they did */
    dict_put(&s1->scheduled_crossings, s2, 0);
    dict_put(&s2->scheduled_crossings, s1, 0);

    event_t e = event_new();
    e.type = EVENT_CROSS;
    e.p = p;
    e.s1 = s1;
    e.s2 = s2;
    heap_put(status->queue, &e);
    return;
}

void exchange_two(status_t*status, event_t*e)
{
    //exchange two segments in list
    segment_t*s1 = e->s1;
    segment_t*s2 = e->s2;
#ifdef CHECKS
    if(!dict_contains(status->intersecting_segs, s1))
        dict_put(status->intersecting_segs, s1, 0);
    if(!dict_contains(status->intersecting_segs, s2))
        dict_put(status->intersecting_segs, s2, 0);
#endif
    segment_t*left = actlist_left(status->actlist, s2);
    segment_t*right = actlist_right(status->actlist, s1);
    assert(left == s1);
    assert(right == s2);
    actlist_swap(status->actlist, s1, s2);
    assert(actlist_right(status->actlist, s2) == s1);
    assert(actlist_left(status->actlist, s1) == s2);
    left = actlist_left(status->actlist, s2);
    right = actlist_right(status->actlist, s1);
    if(left)
        schedule_crossing(status, left, s2);
    if(right)
        schedule_crossing(status, s1, right);
}

typedef struct _box {
    point_t left1, left2, right1, right2;
} box_t;
static inline box_t box_new(int x, int y)
{
    box_t box;
    box.right1.x = box.right2.x = x;
    box.left1.x = box.left2.x = x-1;
    box.left1.y = box.right1.y = y-1;
    box.left2.y = box.right2.y = y;
    return box;
}


static void insert_point_into_segment(status_t*status, segment_t*s, point_t p)
{
    assert(s->pos.x != p.x || s->pos.y != p.y);

#ifdef CHECKS
    if(!dict_contains(status->segs_with_point, s))
        dict_put(status->segs_with_point, s, 0);
    assert(s->fs_out_ok);
#endif

    if(s->fs_out) {
#ifdef DEBUG
        fprintf(stderr, "[%d] receives next point (%d,%d)->(%d,%d) (drawing)\n", s->nr,
                s->pos.x, s->pos.y, p.x, p.y);
#endif
        // omit horizontal lines
        if(s->pos.y != p.y) {
            edge_t*e = rfx_calloc(sizeof(edge_t));
#ifdef DEBUG
            e->tmp = s->nr;
#endif
            e->a = s->pos;
            e->b = p;
            assert(e->a.y != e->b.y);
            e->next = status->output;
            status->output = e;
        }
    } else {
#ifdef DEBUG
        fprintf(stderr, "[%d] receives next point (%d,%d) (omitting)\n", s->nr, p.x, p.y);
#endif
    }
    s->pos = p;
}

typedef struct _segrange {
    double xmin;
    segment_t*segmin;
    double xmax;
    segment_t*segmax;
} segrange_t;

void segrange_adjust_endpoints(segrange_t*range, int y)
{
#define XPOS_EQ(s1,s2,ypos) (XPOS((s1),(ypos))==XPOS((s2),(ypos)))
#ifdef CHECK
    /* this would mean that the segment left/right of the minimum/maximum
       intersects the current segment exactly at the scanline, but somehow
       wasn't found to be passing through the same snapping box */
    assert(!min || !min->left || !XPOS_EQ(min, min->left, y));
    assert(!max || !max->right || !XPOS_EQ(max, max->right, y));
#endif

    /* this doesn't actually ever happen anymore (see checks above) */
    segment_t*min = range->segmin;
    segment_t*max = range->segmax;
    if(min) while(min->left && XPOS_EQ(min, min->left, y)) {
        min = min->left;
    }
    if(max) while(max->right && XPOS_EQ(max, max->right, y)) {
        max = max->right;
    }
    range->segmin = min;
    range->segmax = max;
}
void segrange_test_segment_min(segrange_t*range, segment_t*seg, int y)
{
    if(!seg) return;
    /* we need to calculate the xpos anew (and can't use start coordinate or
       intersection coordinate), because we need the xpos exactly at the end of
       this scanline.
       TODO: might be faster to use XPOS_COMPARE here (see also _max)
     */
    double x = XPOS(seg, y);
    if(!range->segmin || x<range->xmin) {
        range->segmin = seg;
        range->xmin = x;
    }
}
void segrange_test_segment_max(segrange_t*range, segment_t*seg, int y)
{
    if(!seg) return;
    double x = XPOS(seg, y);
    if(!range->segmax || x>range->xmax) {
        range->segmax = seg;
        range->xmax = x;
    }
}

/*
   SLOPE_POSITIVE:
      \+     \ +
------ I      \I
      -I\----  I
       I \   --I\---
       I  \    I \  -------
       +   \   +  \
*/
static void add_points_to_positively_sloped_segments(status_t*status, int32_t y, segrange_t*range)
{
    segment_t*first=0, *last = 0;
    int t;
    for(t=0;t<status->xrow->num;t++) {
        box_t box = box_new(status->xrow->x[t], y);
        segment_t*seg = actlist_find(status->actlist, box.left2, box.left2);

        seg = actlist_right(status->actlist, seg);
        while(seg) {
            if(seg->a.y == y) {
                // this segment started in this scanline, ignore it
                seg->changed = 1;last = seg;if(!first) {first=seg;}
            } else if(seg->delta.x <= 0) {
                // ignore segment w/ negative slope
            } else {
                last = seg;if(!first) {first=seg;}
                double d1 = LINE_EQ(box.right1, seg);
                double d2 = LINE_EQ(box.right2, seg);
                if(d1>0 || d2>=0) {
                    seg->changed = 1;
                    insert_point_into_segment(status, seg, box.right2);
                } else {
                    /* we unfortunately can't break here- the active list is sorted according
                       to the *bottom* of the scanline. hence pretty much everything that's still
                       coming might reach into our box */
                    //break;
                }
            }
            seg = actlist_right(status->actlist, seg);
        }
    }
    segrange_test_segment_min(range, first, y);
    segrange_test_segment_max(range, last, y);
}
/* SLOPE_NEGATIVE:
   |   +   /|  +  /    /
   |   I  / |  I /    /
   |   I /  |  I/    /
   |   I/   |  I    /
   |   I    | /I   /
   |  /+    |/ +  /
*/
static void add_points_to_negatively_sloped_segments(status_t*status, int32_t y, segrange_t*range)
{
    segment_t*first=0, *last = 0;
    int t;
    for(t=status->xrow->num-1;t>=0;t--) {
        box_t box = box_new(status->xrow->x[t], y);
        segment_t*seg = actlist_find(status->actlist, box.right2, box.right2);

        while(seg) {
            if(seg->a.y == y) {
                // this segment started in this scanline, ignore it
                seg->changed = 1;last = seg;if(!first) {first=seg;}
            } else if(seg->delta.x > 0) {
                // ignore segment w/ positive slope
            } else {
                last = seg;if(!first) {first=seg;}
                double d1 = LINE_EQ(box.left1, seg);
                double d2 = LINE_EQ(box.left2, seg);
                if(d1<0 || d2<0) {
                    seg->changed = 1;
                    insert_point_into_segment(status, seg, box.right2);
                } else {
                    //break;
                }
            }
            seg = actlist_left(status->actlist, seg);
        }
    }
    segrange_test_segment_min(range, last, y);
    segrange_test_segment_max(range, first, y);
}

/* segments ending in the current scanline need xrow treatment like everything else.
   (consider an intersection taking place just above a nearly horizontal segment
   ending on the current scanline- the intersection would snap down *below* the
   ending segment if we don't add the intersection point to the latter right away)
   we need to treat ending segments seperately, however. we have to delete them from
   the active list right away to make room for intersect operations (which might
   still be in the current scanline- consider two 45° polygons and a vertical polygon
   intersecting on an integer coordinate). but once they're no longer in the active list,
   we can't use the add_points_to_*_sloped_segments() functions anymore, and re-adding
   them to the active list just for point snapping would be overkill.
   (One other option to consider, however, would be to create a new active list only
    for ending segments)
*/
void add_points_to_ending_segments(status_t*status, int32_t y)
{
    segment_t*seg = status->ending_segments;
    while(seg) {
        segment_t*next = seg->right;seg->right=0;

        assert(seg->b.y == status->y);

        if(status->xrow->num == 1) {
            // shortcut
            point_t p = {status->xrow->x[0], y};
            insert_point_into_segment(status, seg, p);
        } else {
            int t;
            int start=0,end=status->xrow->num,dir=1;
            if(seg->delta.x < 0) {
                start = status->xrow->num-1;
                end = dir = -1;
            }
            for(t=start;t!=end;t+=dir) {
                box_t box = box_new(status->xrow->x[t], y);
                double d0 = LINE_EQ(box.left1, seg);
                double d1 = LINE_EQ(box.left2, seg);
                double d2 = LINE_EQ(box.right1, seg);
                double d3 = LINE_EQ(box.right2, seg);
                if(!(d0>=0 && d1>=0 && d2>=0 && d3>0 ||
                     d0<=0 && d1<=0 && d2<=0 && d3<0)) {
                    insert_point_into_segment(status, seg, box.right2);
                    break;
                }
            }

#ifdef CHECKS
            /* we *need* to find a point to insert. the segment's own end point
               is in that list, for Pete's sake. */
            assert(t!=end);
#endif
        }
        // now that this is done, too, we can also finally free this segment
        segment_destroy(seg);
        seg = next;
    }
    status->ending_segments = 0;
}

static void recalculate_windings(status_t*status, segrange_t*range)
{
    segrange_adjust_endpoints(range, status->y);

    segment_t*s = range->segmin;
    segment_t*end = range->segmax;
    segment_t*last = 0;

#ifdef DEBUG
    s = actlist_leftmost(status->actlist);
    while(s) {
        fprintf(stderr, "[%d]%d%s ", s->nr, s->changed,
            s == range->segmin?"S":(
            s == range->segmax?"E":""));
        s = s->right;
    }
    fprintf(stderr, "\n");
    s = range->segmin;
#endif
#ifdef CHECKS
    /* test sanity: check that we don't have changed segments
       outside of the given range */
    s = actlist_leftmost(status->actlist);
    while(s && s!=range->segmin) {
        assert(!s->changed);
        s = s->right;
    }
    s = actlist_rightmost(status->actlist);
    while(s && s!=range->segmax) {
        assert(!s->changed);
        s = s->left;
    }
    /* in check mode, go through the whole interval so we can test
       that all polygons where the fillstyle changed also have seg->changed=1 */
    s = actlist_leftmost(status->actlist);
    end = 0;
#endif

    if(end)
        end = actlist_right(status->actlist, end);
    while(s!=end) {
#ifndef CHECK
        if(s->changed)
#endif
        {
            segment_t* left = actlist_left(status->actlist, s);
            windstate_t wind = left?left->wind:status->windrule->start(status->num_polygons);
            s->wind = status->windrule->add(wind, s->fs, s->dir, s->polygon_nr);
            fillstyle_t*fs_old = s->fs_out;
            s->fs_out = status->windrule->diff(&wind, &s->wind);

            assert(!(!s->changed && fs_old!=s->fs_out));
            s->changed = 0;

            s->fs_out_ok = 1;
#ifdef DEBUG
            fprintf(stderr, "[%d] %s/%d/%s/%s ", s->nr, s->dir==DIR_UP?"up":"down", s->wind.wind_nr, s->wind.is_filled?"fill":"nofill", s->fs_out?"draw":"omit");
#endif
        }
        s = s->right;
    }
#ifdef DEBUG
    fprintf(stderr, "\n");
#endif
}

/* we need to handle horizontal lines in order to add points to segments
   we otherwise would miss during the windrule re-evaluation */
void intersect_with_horizontal(status_t*status, segment_t*h)
{
    segment_t* left = actlist_find(status->actlist, h->a, h->a);
    segment_t* right = actlist_find(status->actlist, h->b, h->b);

    /* not strictly necessary, also done by the event */
    xrow_add(status->xrow, h->a.x);
    point_t o = h->a;

    if(!right) {
        assert(!left);
        return;
    }

    left = actlist_right(status->actlist, left); //first seg to the right of h->a
    right = right->right; //first seg to the right of h->b
    segment_t* s = left;

    while(s!=right) {
        assert(s);
        int x = XPOS_INT(s, status->y);
#ifdef DEBUG
        fprintf(stderr, "...into [%d] (%d,%d) -> (%d,%d) at (%d,%d)\n", s->nr,
                s->a.x, s->a.y,
                s->b.x, s->b.y,
                x, status->y
                );
#endif
        assert(x >= h->a.x);
        assert(x <= h->b.x);
        assert(s->delta.x > 0 && x >= s->a.x || s->delta.x <= 0 && x <= s->a.x);
        assert(s->delta.x > 0 && x <= s->b.x || s->delta.x <= 0 && x >= s->b.x);
        xrow_add(status->xrow, x);

        s = s->right;
    }
}

void event_apply(status_t*status, event_t*e)
{
    switch(e->type) {
        case EVENT_HORIZONTAL: {
#ifdef DEBUG
            event_dump(e);
#endif
            intersect_with_horizontal(status, e->s1);
            segment_destroy(e->s1);e->s1=0;
            break;
        }
        case EVENT_END: {
            //delete segment from list
            segment_t*s = e->s1;
#ifdef DEBUG
            event_dump(e);
#endif
#ifdef CHECKS
            dict_del(status->intersecting_segs, s);
            dict_del(status->segs_with_point, s);
            assert(!dict_contains(status->intersecting_segs, s));
            assert(!dict_contains(status->segs_with_point, s));
#endif
            segment_t*left = actlist_left(status->actlist, s);
            segment_t*right = actlist_right(status->actlist, s);
            actlist_delete(status->actlist, s);
            if(left && right)
                schedule_crossing(status, left, right);

            s->left = 0; s->right = status->ending_segments;
            status->ending_segments = s;
            break;
        }
        case EVENT_START: {
            //insert segment into list
#ifdef DEBUG
            event_dump(e);
#endif
            segment_t*s = e->s1;
	    assert(e->p.x == s->a.x && e->p.y == s->a.y);
            actlist_insert(status->actlist, s->a, s->b, s);
            segment_t*left = actlist_left(status->actlist, s);
            segment_t*right = actlist_right(status->actlist, s);
            if(left)
                schedule_crossing(status, left, s);
            if(right)
                schedule_crossing(status, s, right);
            schedule_endpoint(status, e->s1);
            break;
        }
        case EVENT_CROSS: {
            // exchange two segments
#ifdef DEBUG
            event_dump(e);
#endif
            if(actlist_right(status->actlist, e->s1) == e->s2 &&
               actlist_left(status->actlist, e->s2) == e->s1) {
                exchange_two(status, e);
            } else {
                /* ignore this crossing for now (there are some line segments in between).
                   it'll get rescheduled as soon as the "obstacles" are gone */
                char del1 = dict_del(&e->s1->scheduled_crossings, e->s2);
                char del2 = dict_del(&e->s2->scheduled_crossings, e->s1);
                assert(del1 && del2);
#ifdef CHECKS
                point_t pair;
                pair.x = e->s1->nr;
                pair.y = e->s2->nr;
                assert(dict_contains(status->seen_crossings, &pair));
                dict_del(status->seen_crossings, &pair);
#endif
            }
        }
    }
}

#ifdef CHECKS
void check_status(status_t*status)
{
    DICT_ITERATE_KEY(status->intersecting_segs, segment_t*, s) {
        if((s->pos.x != s->b.x ||
            s->pos.y != s->b.y) &&
           !dict_contains(status->segs_with_point, s)) {
            fprintf(stderr, "Error: segment [%d] (%sslope) intersects in scanline %d, but it didn't receive a point\n",
                    s->nr,
                    s->delta.x<0?"-":"+",
                    status->y);
            assert(0);
        }
    }
}
#endif

static void add_horizontals(gfxpoly_t*poly, windrule_t*windrule)
{
    /*
          |..|        |...........|                 |           |
          |..|        |...........|                 |           |
          |..+        +        +..|                 +--+     +--+
          |...........|        |..|                    |     |
          |...........|        |..|                    |     |
     */

#ifdef DEBUG
    fprintf(stderr, "========================================================================\n");
#endif
    heap_t* queue = heap_new(sizeof(event_t), compare_events_simple);
    gfxpoly_enqueue(poly->edges, queue, windrule->start(1), 0);

    actlist_t* actlist = actlist_new();

    event_t*e = heap_chopmax(queue);
    while(e) {
        int y = e->p.y;
        int x = 0;
        char fill = 0;
#ifdef DEBUG
        fprintf(stderr, "----------------------------------- %d\n", y);
        actlist_dump(actlist, y-1);
#endif
#ifdef CHECKS
        actlist_verify(actlist, y-1);
#endif
        do {
            if(fill && x != e->p.x) {
#ifdef DEBUG
                fprintf(stderr, "%d) draw horizontal line from %d to %d\n", y, x, e->p.x);
#endif
                edge_t*l= malloc(sizeof(edge_t));
                l->a.y = l->b.y = y;
                l->a.x = x;
                l->b.x = e->p.x;
                l->next = poly->edges;
                poly->edges = l;
            }
            segment_t*left = 0;
            segment_t*s = e->s1;

            windstate_t before,after;
            switch(e->type) {
                case EVENT_START: {
		    assert(e->p.x == s->a.x && e->p.y == s->a.y);
                    actlist_insert(actlist, s->a, s->b, s);
                    event_t e;
                    e.type = EVENT_END;
                    e.p = s->b;
                    e.s1 = s;
                    e.s2 = 0;
                    heap_put(queue, &e);
                    left = actlist_left(actlist, s);

                    before = left?left->wind:windrule->start(1);
                    after = s->wind = windrule->add(before, s->fs, s->dir, s->polygon_nr);
                    break;
                }
                case EVENT_END: {
                    left = actlist_left(actlist, s);
                    actlist_delete(actlist, s);

                    before = s->wind;
                    after = left?left->wind:windrule->start(1);
                    break;
                }
                default: assert(0);
            }

            x = e->p.x;
            fill ^= 1;//(before.is_filled != after.is_filled);
#ifdef DEBUG
            fprintf(stderr, "%d) event=%s[%d] left:[%d] x:%d before:%d after:%d\n",
                    y, e->type==EVENT_START?"start":"end",
                    s->nr,
                    left?left->nr:-1,
                    x,
                    before.is_filled, after.is_filled);
#endif

            if(e->type == EVENT_END)
                segment_destroy(s);

            free(e);
            e = heap_chopmax(queue);
        } while(e && y == e->p.y);

        assert(!fill); // check that polygon is not bleeding
    }
    actlist_destroy(actlist);
    heap_destroy(queue);
}

gfxpoly_t* gfxpoly_process(gfxpoly_t*poly, windrule_t*windrule)
{
    current_polygon = poly;
    heap_t* queue = heap_new(sizeof(event_t), compare_events);

    gfxpoly_enqueue(poly->edges, queue, windrule->start(1), /*polygon nr*/0);

    status_t status;
    memset(&status, 0, sizeof(status_t));
    status.num_polygons = 1;
    status.queue = queue;
    status.windrule = windrule;
    status.actlist = actlist_new();
#ifdef CHECKS
    status.seen_crossings = dict_new2(&point_type);
#endif

    status.xrow = xrow_new();

    event_t*e = heap_chopmax(queue);
    while(e) {
        status.y = e->p.y;
#ifdef CHECKS
        status.intersecting_segs = dict_new2(&ptr_type);
        status.segs_with_point = dict_new2(&ptr_type);
#endif

#ifdef DEBUG
        fprintf(stderr, "----------------------------------- %d\n", status.y);
        actlist_dump(status.actlist, status.y-1);
#endif
#ifdef CHECKS
        actlist_verify(status.actlist, status.y-1);
#endif
        xrow_reset(status.xrow);
        do {
            xrow_add(status.xrow, e->p.x);
            event_apply(&status, e);
            free(e);
            e = heap_chopmax(queue);
        } while(e && status.y == e->p.y);

        xrow_sort(status.xrow);
        segrange_t range;
        memset(&range, 0, sizeof(range));
#ifdef DEBUG
        actlist_dump(status.actlist, status.y);
#endif
        add_points_to_positively_sloped_segments(&status, status.y, &range);
        add_points_to_negatively_sloped_segments(&status, status.y, &range);
        add_points_to_ending_segments(&status, status.y);

        recalculate_windings(&status, &range);
#ifdef CHECKS
        check_status(&status);
        dict_destroy(status.intersecting_segs);
        dict_destroy(status.segs_with_point);
#endif
    }
#ifdef CHECKS
    dict_destroy(status.seen_crossings);
#endif
    actlist_destroy(status.actlist);
    heap_destroy(queue);
    xrow_destroy(status.xrow);

    gfxpoly_t*p = gfxpoly_new(poly->gridsize);
    p->edges = status.output;

    add_horizontals(p, &windrule_evenodd); // output is always even/odd
    return p;
}
