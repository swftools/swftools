#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <math.h>
#include "../mem.h"
#include "../types.h"
#include "../q.h"
#include "poly.h"
#include "active.h"
#include "xrow.h"
#include "wind.h"

//#define DEBUG
//#undef assert
//#define assert(x)

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
#ifdef DEBUG
    dict_t*seen_crossings; //list of crossing we saw so far
    dict_t*intersecting_segs; //list of segments intersecting in this scanline
    dict_t*segs_with_point; //lists of segments that received a point in this scanline
#endif
} status_t;

int compare_events_simple(const void*_a,const void*_b)
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

int compare_events(const void*_a,const void*_b)
{
    event_t* a = (event_t*)_a;
    event_t* b = (event_t*)_b;
    int d = b->p.y - a->p.y;
    if(d) return d;
    /* we should schedule start events after end/intersect.
       The order of end/intersect doesn't actually matter, however,
       so this might be doing too much */
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
            return 0;
        }
    }
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
    assert(s1!=s2);

    /* we probably could precompute these */
    int32_t minx1 = min32(s1->a.x,s1->b.x);
    int32_t miny1 = min32(s1->a.y,s1->b.y);
    int32_t maxx1 = max32(s1->a.x,s1->b.x);
    int32_t maxy1 = max32(s1->a.y,s1->b.y);
    int32_t minx2 = min32(s2->a.x,s2->b.x);
    int32_t miny2 = min32(s2->a.y,s2->b.y);
    int32_t maxx2 = max32(s2->a.x,s2->b.x);
    int32_t maxy2 = max32(s2->a.y,s2->b.y);

    /* both segments are active, so this can't happen */
    assert(!(maxy1 <= miny2 || maxy2 <= miny1));

    /* TODO: optimize this. remove y, precompute the two x values */
    if(maxx1 <= minx2 || maxx2 <= minx1 ||
       maxy1 <= miny2 || maxy2 <= miny1) {
        /* bounding boxes don't intersect */
        return;
    }

    if(dict_contains(&s1->scheduled_crossings, s2)) {
        /* FIXME: this whole segment hashing thing is really slow */
        //fprintf(stderr, "Encountered crossing between [%d] and [%d] twice\n", s1->nr, s2->nr);

        // we already know about this one
        return;
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
#ifdef DEBUG
    point_t pair;
    pair.x = s1->nr;
    pair.y = s2->nr;
    assert(!dict_contains(status->seen_crossings, &pair));
    dict_put(status->seen_crossings, &pair, 0);
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
#ifdef DEBUG
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

#ifdef DEBUG
    if(!dict_contains(status->segs_with_point, s))
        dict_put(status->segs_with_point, s, 0);
#endif

    assert(s->fs_out_ok);
    if(s->fs_out) {
#ifdef DEBUG
        fprintf(stderr, "[%d] receives next point (%d,%d) (drawing)\n", s->nr, p.x, p.y);
#endif
        // omit horizontal lines
        if(s->pos.y != p.y) {
            edge_t*e = malloc(sizeof(edge_t));
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

/* possible optimizations:
   1.) keep two different active lists around, one for negative and one for
       positive slopes
   2.) delay starting events until after this function (tricky, because we *do*
       need the start coordinates)
*/
/*
   SLOPE_POSITIVE:
      \+     \ +
------ I      \I
      -I\----  I
       I \   --I\---
       I  \    I \  -------
       +   \   +  \
*/
static void add_points_to_positively_sloped_segments(status_t*status, int32_t y)
{
    int t;
    for(t=0;t<status->xrow->num;t++) {
        box_t box = box_new(status->xrow->x[t], y);
        segment_t*seg = actlist_find(status->actlist, box.left2, box.left2);
        seg = actlist_right(status->actlist, seg);
        while(seg) {
            if(seg->a.y == y) {
                // this segment just started, ignore it
            } else if(seg->delta.x < 0) {
                // ignore segment w/ negative slope
            } else {
                double d1 = LINE_EQ(box.right1, seg);
                double d2 = LINE_EQ(box.right2, seg);
                if(d1>=0 || d2>=0) {
                    insert_point_into_segment(status, seg, box.right2);
                } else {
                    break;
                }
            }
            seg = actlist_right(status->actlist, seg);
        }
    }
}
/* SLOPE_NEGATIVE:
   |   +   /|  +  /    /
   |   I  / |  I /    /
   |   I /  |  I/    /
   |   I/   |  I    /
   |   I    | /I   /
   |  /+    |/ +  /
*/
static void add_points_to_negatively_sloped_segments(status_t*status, int32_t y)
{
    int t;
    for(t=status->xrow->num-1;t>=0;t--) {
        box_t box = box_new(status->xrow->x[t], y);
        segment_t*seg = actlist_find(status->actlist, box.right2, box.right2);
        while(seg) {
            if(seg->a.y == y) {
                // this segment just started, ignore it
            } else if(seg->delta.x >= 0) {
                // ignore segment w/ positive slope
            } else {
                double d1 = LINE_EQ(box.left1, seg);
                double d2 = LINE_EQ(box.left2, seg);
                if(d1<0 || d2<0) {
                    insert_point_into_segment(status, seg, box.right2);
                } else {
                    break;
                }
            }
            seg = actlist_left(status->actlist, seg);
        }
    }
}

static void recalculate_windings(status_t*status)
{
    /* TODO: we could use some clever second linked list structure so that we
             only need to process points we know we marked */

    segment_t*s = actlist_leftmost(status->actlist);
    segment_t*last = 0;
    while(s) {
        windstate_t wind = last?last->wind:status->windrule->start(status->num_polygons);
        s->wind = status->windrule->add(wind, s->fs, s->dir, s->polygon_nr);
        s->fs_out = status->windrule->diff(&wind, &s->wind);
        s->fs_out_ok = 1;
#ifdef DEBUG
        fprintf(stderr, "[%d] %s/%d/%s/%s ", s->nr, s->dir==DIR_UP?"up":"down", s->wind.wind_nr, s->wind.is_filled?"fill":"nofill", s->fs_out?"draw":"omit");
#endif
        last = s;
        s = actlist_right(status->actlist, s);
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

    left = actlist_right(status->actlist, left);
    right = actlist_right(status->actlist, right);
    segment_t* s = left;

    while(s!=right) {
        assert(s);
        /*
           x1 + ((x2-x1)*(y-y1)) / dy =
           (x1*(y2-y) + x2*(y-y1)) / dy
        */
        point_t p;
        p.y = status->y;
        p.x = XPOS(s, p.y);
#ifdef DEBUG
        fprintf(stderr, "...into [%d] (%d,%d) -> (%d,%d) at (%d,%d)\n", s->nr,
                s->a.x, s->a.y,
                s->b.x, s->b.y,
                p.x, p.y
                );
#endif
        assert(p.x >= h->a.x);
        assert(p.x <= h->b.x);
        assert(s->delta.x > 0 && p.x >= s->a.x || s->delta.x <= 0 && p.x <= s->a.x);
        assert(s->delta.x > 0 && p.x <= s->b.x || s->delta.x <= 0 && p.x >= s->b.x);
        xrow_add(status->xrow, p.x);

        s = actlist_right(status->actlist, s);
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
            break;
        }
        case EVENT_END: {
            //delete segment from list
            segment_t*s = e->s1;
#ifdef DEBUG
            event_dump(e);
            dict_del(status->intersecting_segs, s);
            dict_del(status->segs_with_point, s);
            assert(!dict_contains(status->intersecting_segs, s));
            assert(!dict_contains(status->segs_with_point, s));
#endif
            insert_point_into_segment(status, s, s->b);
            segment_t*left = actlist_left(status->actlist, s);
            segment_t*right = actlist_right(status->actlist, s);
            actlist_delete(status->actlist, s);
            if(left && right)
                schedule_crossing(status, left, right);
            segment_destroy(s);e->s1=0;
            break;
        }
        case EVENT_START: {
            //insert segment into list
#ifdef DEBUG
            event_dump(e);
#endif
            segment_t*s = e->s1;
            actlist_insert(status->actlist, e->p, s);
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
#ifdef DEBUG
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

#ifdef DEBUG
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
        actlist_verify_and_dump(actlist, y-1);
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
                    actlist_insert(actlist, e->p, s);
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

            e = heap_chopmax(queue);
        } while(e && y == e->p.y);

        if(fill) {
            fprintf(stderr, "Error: polygon is bleeding\n");
            exit(0);
        }
    }
}

gfxpoly_t* gfxpoly_process(gfxpoly_t*poly, windrule_t*windrule)
{
    heap_t* queue = heap_new(sizeof(event_t), compare_events);

    gfxpoly_enqueue(poly->edges, queue, windrule->start(1), /*polygon nr*/0);

    status_t status;
    memset(&status, 0, sizeof(status_t));
    status.num_polygons = 1;
    status.queue = queue;
    status.windrule = windrule;
    status.actlist = actlist_new();
#ifdef DEBUG
    status.seen_crossings = dict_new2(&point_type);
#endif

    status.xrow = xrow_new();

    event_t*e = heap_chopmax(queue);
    while(e) {
        status.y = e->p.y;
#ifdef DEBUG
        status.intersecting_segs = dict_new2(&ptr_type);
        status.segs_with_point = dict_new2(&ptr_type);
        fprintf(stderr, "----------------------------------- %d\n", status.y);
        actlist_verify_and_dump(status.actlist, status.y-1);
#endif
        xrow_reset(status.xrow);
        do {
            xrow_add(status.xrow, e->p.x);
            event_apply(&status, e);
            free(e);
            e = heap_chopmax(queue);
        } while(e && status.y == e->p.y);

        xrow_sort(status.xrow);
        add_points_to_positively_sloped_segments(&status, status.y);
        add_points_to_negatively_sloped_segments(&status, status.y);
        recalculate_windings(&status);
#ifdef DEBUG
        check_status(&status);
        dict_destroy(status.intersecting_segs);
        dict_destroy(status.segs_with_point);
#endif
    }
#ifdef DEBUG
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
