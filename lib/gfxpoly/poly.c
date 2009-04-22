#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <math.h>
#include "../q.h"
#include "poly.h"
#include "active.h"
#include "xrow.h"

#define DEBUG

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
    actlist_t*actlist;
    heap_t*queue;
#ifdef DEBUG
    dict_t*seen_crossings; //list of crossing we saw so far
    dict_t*intersecting_segs; //list of segments intersecting in this scanline
    dict_t*segs_with_point; //lists of segments that received a point in this scanline
#endif
} status_t;

int compare_events(const void*_a,const void*_b)
{
    event_t* a = (event_t*)_a;
    event_t* b = (event_t*)_b; 
    if(a->p.y < b->p.y) {
        return 1;
    } else if(a->p.y > b->p.y) {
        return -1;
    } else if(a->type < b->type) {
        return 1;
    } else if(a->type > b->type) {
        return -1;
    } else if(a->p.x < b->p.x) {
        return 1;
    } else if(a->p.x > b->p.x) {
        return -1;
    } else
        return 0;
}

gfxpoly_t* gfxpoly_new()
{
    return 0;
}
void gfxpoly_destroy(gfxpoly_t*poly)
{
    segment_t* s = poly;
    while(s) {
        segment_t*right = s->right;
        free(s);
        s = right;
        if(s==poly)
            break;
    }
}

void gfxpoly_dump(gfxpoly_t*poly)
{
    segment_t* s = (segment_t*)poly;
    while(s) {
        printf("[%d] (%d,%d) -> (%d,%d) %s\n",
                s->nr, s->a.x, s->a.y, s->b.x, s->b.y,
                s->dir==DIR_UP?"up":"down");
        s = s->right;
        if(s==poly)
            break;
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
    if(e->type == EVENT_START) {
        printf("event: segment [%d] starts at (%d,%d)\n", e->s1->nr, e->p.x, e->p.y);
    } else if(e->type == EVENT_END) {
        printf("event: segment [%d] ends at (%d,%d)\n", e->s1->nr, e->p.x, e->p.y);
    } else if(e->type == EVENT_CROSS) {
        printf("event: segment [%d] and [%d] intersect at (%d,%d)\n", e->s1->nr, e->s2->nr, e->p.x, e->p.y);
    } else assert(0);
}

static inline max32(int32_t v1, int32_t v2) {return v1>v2?v1:v2;}
static inline min32(int32_t v1, int32_t v2) {return v1<v2?v1:v2;}

void segment_init(segment_t*s, int x1, int y1, int x2, int y2)
{
    assert(y1!=y2);
    if(y1<y2) {
        s->dir = DIR_DOWN;
    } else {
        int x = x1;x1=x2;x2=x;
        int y = y1;y1=y2;y2=y;
        s->dir = DIR_UP;
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
    s->tmp = -1;
#ifdef DEBUG
    static int segment_count=0; //for debugging
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

segment_t*segment_new(int x1, int y1, int x2, int y2)
{
    segment_t*s = malloc(sizeof(segment_t));
    segment_init(s, x1, y1, x2, y2);
    return s;
}

void segment_insert_before(segment_t**list, segment_t*add)
{
    if(!*list) {
        *list = add;
        add->left = add->right = add;
    } else {
        add->left = (*list)->left;
        add->right = (*list);
        add->right->left = add;
        add->left->right = add;
    }
}

void segments_enqueue(segment_t*list, heap_t*queue)
{
    segment_t*l = list;
    while(l) {
        segment_t*right = l->right;
        l->left = l->right = 0;
        event_t e = event_new();
        e.type = EVENT_START;
        e.p = l->a;e.s1 = l;e.s2 = 0;
        heap_put(queue, &e);
        e.type = EVENT_END;
        e.p = l->b;e.s1 = l;e.s2 = 0;
        heap_put(queue, &e);
        l = right;
        if(l==list) break;
    }
}

void schedule_crossing(status_t*status, segment_t*s1, segment_t*s2, int miny)
{
    /* the code that's required (and the checks you can perform) before
       it can be said with 100% certainty that we indeed have a valid crossing
       amazes me every time. -mk */
    assert(s1!=s2);

    if(dict_contains(&s1->scheduled_crossings, s2)) {
        // we already know about this one
        return;
    }

    /* we probably could precompute these */
    int32_t minx1 = min32(s1->a.x,s1->b.x);
    int32_t miny1 = min32(s1->a.y,s1->b.y);
    int32_t maxx1 = max32(s1->a.x,s1->b.x);
    int32_t maxy1 = max32(s1->a.y,s1->b.y);
    int32_t minx2 = min32(s2->a.x,s2->b.x);
    int32_t miny2 = min32(s2->a.y,s2->b.y);
    int32_t maxx2 = max32(s2->a.x,s2->b.x);
    int32_t maxy2 = max32(s2->a.y,s2->b.y);
    
    if(maxx1 <= minx2 || maxx2 <= minx1 ||
       maxy1 <= miny2 || maxy2 <= miny1) {
        /* bounding boxes don't intersect */
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
            fprintf(stderr, "Notice: segments [%d] and [%d] are exactly on top of each other\n", s1->nr, s2->nr);
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
    /* we only schedule crossings if they are *after* the current y. That way, we don't
       schedule the same crossing twice */
    if(p.y >= miny) {
#ifdef DEBUG
        point_t pair;
        pair.x = s1->nr;
        pair.y = s2->nr;
        assert(!dict_contains(status->seen_crossings, &pair));
        dict_put(status->seen_crossings, &pair, 0);
#endif
        event_t e = event_new();
        e.type = EVENT_CROSS;
        e.p = p;
        e.s1 = s1;
        e.s2 = s2;
#ifdef DEBUG
        printf("schedule crossing between [%d] and [%d] at (%d,%d)\n", s1->nr, s2->nr, e.p.x, e.p.y);
#endif
        /* we insert into each other's intersection history because these segments might switch
           places */
        dict_put(&s1->scheduled_crossings, s2, 0);
        dict_put(&s2->scheduled_crossings, s1, 0);

        heap_put(status->queue, &e);
    }
    return;
}

/* routine dealing with the special case of a number of line segments intersecting in exactly one
   point */
void exchange_many(status_t*status, event_t*e)
{
    int size = 16;
    int n = 0;
    segment_t**segs = (segment_t**)malloc(sizeof(segment_t*)*size);
    segs[n] = e->s1;e->s1->tmp = n;n++;
    segs[n] = e->s2;e->s2->tmp = n;n++;
#ifdef DEBUG
    dict_put(status->intersecting_segs, e->s1, 0);
    dict_put(status->intersecting_segs, e->s2, 0);
#endif
    while(1) {
        event_t*ee = heap_peek(status->queue);
        if(ee->p.x != e->p.x ||
           ee->p.y != e->p.y ||
           ee->type != e->type) {
            break;
        }
        if(n>=size-1) {
            size *= 2;
            segs = realloc(segs, sizeof(segment_t*)*size);
        }
        ee = heap_chopmax(status->queue);
        if(ee->s1->tmp<0) {
            segs[n] = ee->s1;ee->s1->tmp = n;n++;
        }
        if(ee->s2->tmp<0) {
            segs[n] = ee->s2;ee->s2->tmp = n;n++;
        }
#ifdef DEBUG
        dict_put(status->intersecting_segs, ee->s1, 0);
        dict_put(status->intersecting_segs, ee->s2, 0);
#endif
    }
    int t;
#ifdef DEBUG
    printf("event: multi-intersect: ");
    for(t=0;t<n;t++) {
        printf("[%d] ", segs[t]->nr);
    }
    printf("\n");
#endif
    segment_t*left = 0;
    segment_t*right = 0;
    char warn = 0;
    for(t=0;t<n;t++) {
        if(!segs[t]->left || segs[t]->left->tmp<0) {
            assert(!left);
            left = segs[t];
        }
        if(!segs[t]->right || segs[t]->right->tmp<0) {
            assert(!right);
            right = segs[t];
        }
        if(segs[t]->tmp<0)
            warn = 1;
    }
    if(warn)
        fprintf(stderr, "Warning: multi-cross section contains rogue segments\n");

    assert(left);
    assert(right);
    assert(left!=right);
    actlist_invert_fromto(status->actlist, left, right);
    for(t=0;t<n;t++) {
        segs[t]->tmp = -1;
    }
    free(segs);
    if(right->left)
        schedule_crossing(status, right->left, right, status->y);
    if(left->right)
        schedule_crossing(status, left, left->right, status->y);
}

void exchange_two(status_t*status, event_t*e)
{
    //exchange two segments in list
    segment_t*s1 = e->s1;
    segment_t*s2 = e->s2;
#ifdef DEBUG
    dict_put(status->intersecting_segs, s1, 0);
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
        schedule_crossing(status, left, s2, status->y);
    if(right)
        schedule_crossing(status, s1, right, status->y);
}

void insert_point_into_segment(segment_t*s, point_t p)
{
    if(s->pos.x == p.x && s->pos.y == p.y) {
#ifdef DEBUG
        fprintf(stderr, "Notice: tried to add (%d,%d) to segment [%d] twice\n", p.x, p.y, s->nr);
#endif
        return;
    }
#ifdef DEBUG
    printf("[%d] gets extra point (%d,%d)\n", s->nr, p.x, p.y);
#endif
    s->pos = p;
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
        
/* possible optimizations:
   1.) keep two different active lists around, one for negative and one for
       positive slopes
   2.) delay starting events until after this function (tricky, because we *do*
       need the start coordinates)
*/
/*
   SLOPE_POSITIVE:
       +     \ +        
------ I      \I       
      -I\----  I      
       I \   --I\---
       I  \    I \  -------
       +   \   +  \
*/
void add_points_to_positively_sloped_segments(status_t*status, xrow_t*xrow, int32_t y)
{
    int t;
    for(t=0;t<xrow->num;t++) {
        box_t box = box_new(xrow->x[t], y);
        segment_t*seg = actlist_find(status->actlist, box.left2, box.left2);
        if(seg) 
            seg = seg->right;
        else    
            seg = actlist_leftmost(status->actlist);
        while(seg) {
            if(seg->a.y == y) {
                // this segment just started, ignore it
            } else if(seg->delta.x < 0) {
                // ignore segment w/ negative slope
            } else {
                double d1 = LINE_EQ(box.right1, seg);
                double d2 = LINE_EQ(box.right2, seg);
                if(d1>=0 || d2>=0) {
#ifdef DEBUG
                    dict_put(status->segs_with_point, seg, 0);
#endif
                    insert_point_into_segment(seg, box.right2);
                } else {
                    break;
                }
            }
            seg = seg->right;
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
void add_points_to_negatively_sloped_segments(status_t*status, xrow_t*xrow, int32_t y)
{
    int t;
    for(t=xrow->num-1;t>=0;t--) {
        box_t box = box_new(xrow->x[t], y);
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
#ifdef DEBUG
                    dict_put(status->segs_with_point, seg, 0);
#endif
                    insert_point_into_segment(seg, box.right2);
                } else {
                    break;
                }
            }
            seg = seg->left;
        }
    }
}

void event_apply(status_t*status, event_t*e)
{
    switch(e->type) {
        case EVENT_END: {
            //delete segment from list
            event_dump(e);
            segment_t*s = e->s1;
            insert_point_into_segment(s, s->b);
            segment_t*left = actlist_left(status->actlist, s);
            segment_t*right = actlist_right(status->actlist, s);
            actlist_delete(status->actlist, s);
            dict_clear(&s->scheduled_crossings);
            if(left && right)
                schedule_crossing(status, left, right, status->y+1);
            break;
        }
        case EVENT_START: {
            //insert segment into list
            event_dump(e);
            segment_t*s = e->s1;
            actlist_insert(status->actlist, e->p, s);
            segment_t*left = actlist_left(status->actlist, s);
            segment_t*right = actlist_right(status->actlist, s);
            if(left)
                schedule_crossing(status, left, s, status->y+1);
            if(right)
                schedule_crossing(status, s, right, status->y+1);
            break;
        }
        case EVENT_CROSS: {
            // exchange two (or more) segments
            event_t*ee = heap_peek(status->queue);
            if(ee->p.x != e->p.x ||
               ee->p.y != e->p.y ||
               ee->type != e->type) {
                event_dump(e);
                exchange_two(status, e);
            } else {
                exchange_many(status, e);
            }
        }
    }
}

#ifdef DEBUG
void check_status(status_t*status)
{
    DICT_ITERATE_KEY(status->intersecting_segs, segment_t*, s) {
        if(!dict_contains(status->segs_with_point, s)) {
            fprintf(stderr, "Error: segment [%d] (%sslope) intersects in scanline %d, but it didn't receive a point\n", 
                    s->nr, 
                    s->delta.x<0?"-":"+",
                    status->y);
            assert(0);
        }
    }
}
#endif

void gfxpoly_process(segment_t*poly)
{
    heap_t* queue = heap_new(sizeof(event_t), compare_events);
    segments_enqueue(poly, queue);
    status_t status;
    memset(&status, 0, sizeof(status_t));
    status.queue = queue;
    status.actlist = actlist_new();
#ifdef DEBUG
    status.seen_crossings = dict_new2(&point_type);
#endif
    
    xrow_t*xrow = xrow_new();

    event_t*e = heap_chopmax(queue);
    while(e) {
        status.y = e->p.y;
#ifdef DEBUG
        status.intersecting_segs = dict_new2(&ptr_type);
        status.segs_with_point = dict_new2(&ptr_type);
        printf("----------------------------------- %d\n", status.y);
        actlist_verify_and_dump(status.actlist, status.y-1);
#endif
        xrow_reset(xrow);
        do {
            xrow_add(xrow, e->p.x);
            event_apply(&status, e);
            free(e);
            e = heap_chopmax(queue);
        } while(e && status.y == e->p.y);

        xrow_sort(xrow);
        add_points_to_positively_sloped_segments(&status, xrow, status.y);
        add_points_to_negatively_sloped_segments(&status, xrow, status.y);
#ifdef DEBUG
        check_status(&status);
        dict_destroy(status.intersecting_segs);
        dict_destroy(status.segs_with_point);
#endif
    }
#ifdef DEBUG
    dict_destroy(status.seen_crossings);
#endif
    heap_destroy(queue);
    gfxpoly_destroy(poly);
    xrow_destroy(xrow);
}
