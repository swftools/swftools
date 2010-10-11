#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "../mem.h"
#include "../types.h"
#include "poly.h"
#include "active.h"
#include "xrow.h"
#include "wind.h"
#include "convert.h"
#include "heap.h"

#ifdef HAVE_MD5
#include "MD5.h"
#endif

static gfxpoly_t*current_polygon = 0;
void gfxpoly_fail(char*expr, char*file, int line, const char*function)
{
    if(!current_polygon) {
	fprintf(stderr, "assert(%s) failed in %s in line %d: %s\n", expr, file, line, function);
	exit(1);
    }

    char filename[32+4+1];
#ifdef HAVE_MD5
    void*md5 = initialize_md5();
   
    int s,t;
    gfxpolystroke_t*stroke = current_polygon->strokes;
    for(;stroke;stroke=stroke->next) {
	for(t=0;t<stroke->num_points;t++) {
	    update_md5(md5, (unsigned char*)&stroke->points[t].x, sizeof(stroke->points[t].x));
	    update_md5(md5, (unsigned char*)&stroke->points[t].y, sizeof(stroke->points[t].y));
	}
    }
    unsigned char h[16];
    finish_md5(md5, h);
    sprintf(filename, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x.ps",
	    h[0],h[1],h[2],h[3],h[4],h[5],h[6],h[7],h[8],h[9],h[10],h[11],h[12],h[13],h[14],h[15]);
#else
    sprintf(filename, "%d", time(0));
#endif

    fprintf(stderr, "assert(%s) failed in %s in line %d: %s\n", expr, file, line, function);
    fprintf(stderr, "I'm saving a debug file \"%s\" to the current directory.\n", filename);

    gfxpoly_save(current_polygon, filename);
    exit(1);
}

static char point_equals(const void*o1, const void*o2)
{
    const point_t*p1 = o1;
    const point_t*p2 = o2;
    return p1->x == p2->x && p1->y == p2->y;
}
static unsigned int point_hash(const void*o)
{
    const point_t*p = o;
    return p->x^p->y;
}
static void* point_dup(const void*o)
{
    const point_t*p = o;
    point_t*n = malloc(sizeof(point_t));
    n->x = p->x;
    n->y = p->y;
    return n;
}
static void point_free(void*o)
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

typedef struct _event {
    eventtype_t type;
    point_t p;
    segment_t*s1;
    segment_t*s2;
} event_t;

/* compare_events_simple differs from compare_events in that it schedules
   events from left to right regardless of type. It's only used in horizontal
   processing, in order to get an x-wise sorting of the current scanline */
static inline int compare_events_simple(const void*_a,const void*_b)
{
    event_t* a = (event_t*)_a;
    event_t* b = (event_t*)_b;
    int d = b->p.y - a->p.y;
    if(d) return d;
    d = b->p.x - a->p.x;
    if(d) return d;
    return 0;
}

static inline int compare_events(const void*_a,const void*_b)
{
    event_t* a = (event_t*)_a;
    event_t* b = (event_t*)_b;
    int d = b->p.y - a->p.y;
    if(d) return d;
    /* we need to schedule end after intersect (so that a segment about
       to end has a chance to tear up a few other segs first) and start
       events after end (in order not to confuse the intersection check, which
       assumes there's an actual y overlap between active segments, and 
       because ending segments in the active list make it difficult to insert
       starting segments at the right position)). 
       Horizontal lines come last, because the only purpose
       they have is to create snapping coordinates for the segments (still)
       existing in this scanline.
    */
    d = b->type - a->type;
    if(d) return d;
    return 0;

    /* I don't see any reason why we would need to order by x- at least as long
       as we do horizontal lines in a seperate pass */
    //d = b->p.x - a->p.x;
    //return d;
}

#define COMPARE_EVENTS(x,y) (compare_events(x,y)>0)
#define COMPARE_EVENTS_SIMPLE(x,y) (compare_events_simple(x,y)>0)
HEAP_DEFINE(queue,event_t,COMPARE_EVENTS);
HEAP_DEFINE(hqueue,event_t,COMPARE_EVENTS_SIMPLE);

typedef struct _horizontal {
    int32_t y;
    int32_t x1, x2;
    edgestyle_t*fs;
    segment_dir_t dir;
    int polygon_nr;
    int xpos;
    int pos;
} horizontal_t;

typedef struct _horizdata {
    horizontal_t*data;
    int num;
    int size;
} horizdata_t;

typedef struct _status {
    int32_t y;
    double gridsize;
    actlist_t*actlist;
    queue_t queue;
    xrow_t*xrow;
    windrule_t*windrule;
    windcontext_t*context;
    segment_t*ending_segments;

    horizdata_t horiz;

    gfxpolystroke_t*strokes;
#ifdef CHECKS
    dict_t*seen_crossings; //list of crossing we saw so far
    dict_t*intersecting_segs; //list of segments intersecting in this scanline
    dict_t*segs_with_point; //lists of segments that received a point in this scanline
#endif
} status_t;


int gfxpoly_num_segments(gfxpoly_t*poly)
{
    gfxpolystroke_t*stroke = poly->strokes;
    int count = 0;
    for(;stroke;stroke=stroke->next) {
	count++;
    }
    return count;
}
int gfxpoly_size(gfxpoly_t*poly)
{
    int s,t;
    int edges = 0;
    gfxpolystroke_t*stroke = poly->strokes;
    for(;stroke;stroke=stroke->next) {
	edges += stroke->num_points-1;
    }
    return edges;
}

char gfxpoly_check(gfxpoly_t*poly, char updown)
{
    dict_t*d1 = dict_new2(&point_type);
    dict_t*d2 = dict_new2(&point_type);
    int s,t;
    gfxpolystroke_t*stroke = poly->strokes;
    for(;stroke;stroke=stroke->next) {
	/* In order to not confuse the fill/wind logic, existing segments must have
	   a non-zero edge style */
	assert(stroke->fs);

	/* put all the segments into dictionaries so that we can check
	   that the endpoint multiplicity is two */
	for(s=0;s<stroke->num_points;s++) {
	    point_t p = stroke->points[s];
	    int num_xor = (s>=1 && s<stroke->num_points-1)?2:1; // mid points are two points (start+end)
	    int num_circ = (s>=1 && s<stroke->num_points-1)?0:(s==0?1:-1);
	    if(stroke->dir==DIR_UP)
		num_circ=-num_circ;

	    if(!dict_contains(d1, &p)) {
		dict_put(d1, &p, (void*)(ptroff_t)num_xor);
		if(updown) {
		    assert(!dict_contains(d2, &p));
		    dict_put(d2, &p, (void*)(ptroff_t)num_circ);
		}
	    } else {
		int count = (ptroff_t)dict_lookup(d1, &p);
		dict_del(d1, &p);
		count+=num_xor;
		dict_put(d1, &p, (void*)(ptroff_t)count);

		if(updown) {
		    assert(dict_contains(d2, &p));
		    count = (ptroff_t)dict_lookup(d2, &p);
		    dict_del(d2, &p);
		    count+=num_circ;
		    dict_put(d2, &p, (void*)(ptroff_t)count);
		}
	    }
	}
    }
    DICT_ITERATE_ITEMS(d1, point_t*, p1, void*, c1) {
        int count = (ptroff_t)c1;
        if(count&1) {
            fprintf(stderr, "Error: Point (%.2f,%.2f) occurs %d times\n", p1->x * poly->gridsize, p1->y * poly->gridsize, count);
            dict_destroy(d1);
            dict_destroy(d2);
	    return 0;
        }
    }
    if(updown) {
	DICT_ITERATE_ITEMS(d2, point_t*, p2, void*, c2) {
	    int count = (ptroff_t)c2;
	    assert(dict_contains(d1, p2));
	    int ocount = (ptroff_t)dict_lookup(d1, p2);
	    if(count!=0) {
		if(count>0) fprintf(stderr, "Error: Point (%.2f,%.2f) has %d more incoming than outgoing segments (%d incoming, %d outgoing)\n", p2->x * poly->gridsize, p2->y * poly->gridsize, count, (ocount+count)/2, (ocount-count)/2);
		if(count<0) fprintf(stderr, "Error: Point (%.2f,%.2f) has %d more outgoing than incoming segments (%d incoming, %d outgoing)\n", p2->x * poly->gridsize, p2->y * poly->gridsize, -count, (ocount+count)/2, (ocount-count)/2);
		gfxpolystroke_t*stroke = poly->strokes;
		for(;stroke;stroke=stroke->next) {
		    for(s=0;s<stroke->num_points-1;s++) {
			point_t a = stroke->points[s];
			point_t b = stroke->points[s+1];
			if(a.x == p2->x && a.y == p2->y ||
			   b.x == p2->x && b.y == p2->y) {
			    fprintf(stderr, "%.2f,%.2f -> %.2f,%.2f\n", 
				    a.x * poly->gridsize, 
				    a.y * poly->gridsize, 
				    b.x * poly->gridsize, 
				    b.y * poly->gridsize);
			}
		    }
		}
		dict_destroy(d2);
		return 0;
	    }
	}
    }
    dict_destroy(d1);
    dict_destroy(d2);
    return 1;
}

void gfxpoly_dump(gfxpoly_t*poly)
{
    int s,t;
    double g = poly->gridsize;
    fprintf(stderr, "polyon %p (gridsize: %.2f)\n", poly, poly->gridsize);
    gfxpolystroke_t*stroke = poly->strokes;
    for(;stroke;stroke=stroke->next) {
	fprintf(stderr, "%11p", stroke);
	if(stroke->dir==DIR_UP) {
	    for(s=stroke->num_points-1;s>=1;s--) {
		point_t a = stroke->points[s];
		point_t b = stroke->points[s-1];
		fprintf(stderr, "%s (%.2f,%.2f) -> (%.2f,%.2f)%s%s\n", s!=stroke->num_points-1?"           ":"", a.x*g, a.y*g, b.x*g, b.y*g,
							    s==1?"]":"", a.y==b.y?"H":"");
	    }
	} else {
	    for(s=0;s<stroke->num_points-1;s++) {
		point_t a = stroke->points[s];
		point_t b = stroke->points[s+1];
		fprintf(stderr, "%s (%.2f,%.2f) -> (%.2f,%.2f)%s%s\n", s?"           ":"", a.x*g, a.y*g, b.x*g, b.y*g,
							    s==stroke->num_points-2?"]":"", a.y==b.y?"H":"");
	    }
	}
    }
}

void gfxpoly_save(gfxpoly_t*poly, const char*filename)
{
    FILE*fi = fopen(filename, "wb");
    fprintf(fi, "%% gridsize %f\n", poly->gridsize);
    fprintf(fi, "%% begin\n");
    int s,t;
    gfxpolystroke_t*stroke = poly->strokes;
    for(;stroke;stroke=stroke->next) {
	    fprintf(fi, "%g setgray\n", stroke->dir==DIR_UP ? 0.7 : 0);
	point_t p = stroke->points[0];
	fprintf(fi, "%d %d moveto\n", p.x, p.y);
	for(s=1;s<stroke->num_points;s++) {
	    p = stroke->points[s];
	    fprintf(fi, "%d %d lineto\n", p.x, p.y);
	}
	fprintf(fi, "stroke\n");
    }
    fprintf(fi, "showpage\n");
    fclose(fi);
}

void gfxpoly_save_arrows(gfxpoly_t*poly, const char*filename)
{
    FILE*fi = fopen(filename, "wb");
    fprintf(fi, "%% gridsize %f\n", poly->gridsize);
    fprintf(fi, "%% begin\n");
    int t;
    double l = 5.0 / poly->gridsize;
    double g = poly->gridsize;
    gfxpolystroke_t*stroke = poly->strokes;
    for(;stroke;stroke=stroke->next) {
	fprintf(fi, "0 setgray\n");

	int s = stroke->dir==DIR_UP?stroke->num_points-1:0;
	int end = stroke->dir==DIR_UP?-1:stroke->num_points;
	int dir = stroke->dir==DIR_UP?-1:1;

	point_t p = stroke->points[s];
	s+=dir;
	point_t o = p;
	fprintf(fi, "%f %f moveto\n", p.x * g, p.y * g);
	for(;s!=end;s+=dir) {
	    p = stroke->points[s];
	    int lx = p.x - o.x;
	    int ly = p.y - o.y;
	    double d = sqrt(lx*lx+ly*ly);
	    if(!d) d=1;
	    else   d = l / d;
	    double d2 = d*1.5;
	    fprintf(fi, "%f %f lineto\n", (p.x - lx*d2) * g, (p.y - ly*d2) * g);
	    fprintf(fi, "%f %f lineto\n", (p.x - lx*d2 + (ly*d))*g,
		                          (p.y - ly*d2 - (lx*d))*g);
	    fprintf(fi, "%f %f lineto\n", p.x * g, p.y * g);
	    fprintf(fi, "%f %f lineto\n", (p.x - lx*d2 - (ly*d))*g, 
		                          (p.y - ly*d2 + (lx*d))*g);
	    fprintf(fi, "%f %f lineto\n", (p.x - lx*d2) * g, (p.y - ly*d2) * g);
	    fprintf(fi, "%f %f moveto\n", p.x * g, p.y * g);
	    o = p;
	}
	fprintf(fi, "stroke\n");
    }
    fprintf(fi, "showpage\n");
    fclose(fi);
}

inline static event_t* event_new()
{
    event_t*e = rfx_calloc(sizeof(event_t));
    return e;
}
inline static void event_free(event_t*e)
{
    free(e);
}

static void event_dump(status_t*status, event_t*e)
{
    if(e->type == EVENT_HORIZONTAL) {
        fprintf(stderr, "Horizontal [%d] (%.2f,%.2f) -> (%.2f,%.2f)\n", (int)e->s1->nr, 
		e->s1->a.x * status->gridsize, e->s1->a.y * status->gridsize, e->s1->b.x * status->gridsize, e->s1->b.y * status->gridsize);
    } else if(e->type == EVENT_START) {
        fprintf(stderr, "event: segment [%d] starts at (%.2f,%.2f)\n", (int)e->s1->nr, 
		e->p.x * status->gridsize, e->p.y * status->gridsize);
    } else if(e->type == EVENT_END) {
        fprintf(stderr, "event: segment [%d] ends at (%.2f,%.2f)\n", (int)e->s1->nr, 
		e->p.x * status->gridsize, e->p.y * status->gridsize);
    } else if(e->type == EVENT_CROSS) {
        fprintf(stderr, "event: segment [%d] and [%d] intersect at (%.2f,%.2f)\n", (int)e->s1->nr, (int)e->s2->nr, 
		e->p.x * status->gridsize, e->p.y * status->gridsize);
    } else {
        assert(0);
    }
}

static inline int32_t max32(int32_t v1, int32_t v2) {return v1>v2?v1:v2;}
static inline int32_t min32(int32_t v1, int32_t v2) {return v1<v2?v1:v2;}

static void segment_dump(segment_t*s)
{
    fprintf(stderr, "[%d] (%d,%d)->(%d,%d) ", (int)s->nr, s->a.x, s->a.y, s->b.x, s->b.y);
    fprintf(stderr, " dx:%d dy:%d k:%f dx/dy=%f fs=%p\n", s->delta.x, s->delta.y, s->k,
            (double)s->delta.x / s->delta.y, s->fs);
}

static void segment_init(segment_t*s, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int polygon_nr, segment_dir_t dir)
{
    static int segment_count=0;
    s->nr = segment_count++;
    s->dir = dir;
    if(y1!=y2) {
	assert(y1<y2);
    } else {
        /* We need to make sure horizontal segments always go from left to right.
	   "up/down" for horizontal segments is handled by "rotating"
           them 90° counterclockwise in screen coordinates (tilt your head to
           the right). In other words, the "normal" direction (what's positive dy for
	   vertical segments) is positive dx for horizontal segments ("down" is right).
	 */
        if(x1>x2) {
            s->dir = DIR_INVERT(s->dir);
            int32_t x = x1;x1=x2;x2=x;
            int32_t y = y1;y1=y2;y2=y;
        }
#ifdef DEBUG
	fprintf(stderr, "Scheduling horizontal segment [%d] (%.2f,%.2f) -> (%.2f,%.2f) %s\n",
		segment_count,
		x1 * 0.05, y1 * 0.05, x2 * 0.05, y2 * 0.05, s->dir==DIR_UP?"up":"down");
#endif
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

#ifdef CHECKS
    /* notice: on some systems (with some compilers), for the line 
       (1073741823,-1073741824)->(1073741823,1073741823)
       we get LINE_EQ(s->a, s) == 1. 
       That's why we now clamp to 26 bit.
    */
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
#endif

#ifndef DONT_REMEMBER_CROSSINGS
    dict_init2(&s->scheduled_crossings, &ptr_type, 0);
#endif
}

static segment_t* segment_new(point_t a, point_t b, int polygon_nr, segment_dir_t dir)
{
    segment_t*s = (segment_t*)rfx_calloc(sizeof(segment_t));
    segment_init(s, a.x, a.y, b.x, b.y, polygon_nr, dir);
    return s;
}

static void segment_clear(segment_t*s)
{
#ifndef DONT_REMEMBER_CROSSINGS
    dict_clear(&s->scheduled_crossings);
#endif
}
static void segment_destroy(segment_t*s)
{
    segment_clear(s);
    free(s);
}

static void advance_stroke(queue_t*queue, hqueue_t*hqueue, gfxpolystroke_t*stroke, int polygon_nr, int pos, double gridsize)
{
    if(!stroke) 
	return;
    segment_t*s = 0;
    /* we need to queue multiple segments at once because we need to process start events
       before horizontal events */
    while(pos < stroke->num_points-1) {
	assert(stroke->points[pos].y <= stroke->points[pos+1].y);
	s = segment_new(stroke->points[pos], stroke->points[pos+1], polygon_nr, stroke->dir);
	s->fs = stroke->fs;
	pos++;
	s->stroke = 0;
	s->stroke_pos = 0;
#ifdef DEBUG
	/*if(l->tmp)
	    s->nr = l->tmp;*/
	fprintf(stderr, "[%d] (%.2f,%.2f) -> (%.2f,%.2f) %s (stroke %p, %d more to come)\n",
		s->nr, s->a.x * gridsize, s->a.y * gridsize, 
		s->b.x * gridsize, s->b.y * gridsize,
		s->dir==DIR_UP?"up":"down", stroke, stroke->num_points - 1 - pos);
#endif
	event_t* e = event_new();
	e->type = s->delta.y ? EVENT_START : EVENT_HORIZONTAL;
	e->p = s->a;
	e->s1 = s;
	e->s2 = 0;
	
	if(queue) queue_put(queue, e);
	else hqueue_put(hqueue, e);

	if(e->type != EVENT_HORIZONTAL) {
	    break;
	}
    }
    if(s) {
	s->stroke = stroke;
	s->stroke_pos = pos;
    }
}

static void gfxpoly_enqueue(gfxpoly_t*p, queue_t*queue, hqueue_t*hqueue, int polygon_nr)
{
    int t;
    gfxpolystroke_t*stroke = p->strokes;
    for(;stroke;stroke=stroke->next) {
	assert(stroke->num_points > 1);

#ifdef CHECKS
	int s;
	for(s=0;s<stroke->num_points-1;s++) {
	    assert(stroke->points[s].y <= stroke->points[s+1].y);
	}
#endif
	advance_stroke(queue, hqueue, stroke, polygon_nr, 0, p->gridsize);
    }
}

static void schedule_endpoint(status_t*status, segment_t*s)
{
    // schedule end point of segment
    assert(s->b.y > status->y);
    event_t*e = event_new();
    e->type = EVENT_END;
    e->p = s->b;
    e->s1 = s;
    e->s2 = 0;
    queue_put(&status->queue, e);
}

static void schedule_crossing(status_t*status, segment_t*s1, segment_t*s2)
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
#ifdef DEBUG
            fprintf(stderr, "[%d] doesn't intersect with [%d] because: bounding boxes don't intersect\n", s1->nr, s2->nr);
#endif
        /* bounding boxes don't intersect */
        return;
    }

#ifndef DONT_REMEMBER_CROSSINGS
    if(dict_contains(&s1->scheduled_crossings, (void*)(ptroff_t)s2->nr)) {
        /* FIXME: this whole segment hashing thing is really slow */
#ifdef DEBUG
        fprintf(stderr, "[%d] doesn't intersect with [%d] because: we already scheduled this intersection\n", s1->nr, s2->nr);
//	DICT_ITERATE_KEY(&s1->scheduled_crossings, void*, x) {
//	    fprintf(stderr, "[%d]<->[%d]\n", s1->nr, (int)(ptroff_t)x);
//	}
#endif
        return; // we already know about this one
    }
#endif

    double det = (double)s1->delta.x*s2->delta.y - (double)s1->delta.y*s2->delta.x;
    if(!det) {
        if(s1->k == s2->k) {
            // lines are exactly on top of each other (ignored)
#ifdef DEBUG
            fprintf(stderr, "Notice: segments [%d] and [%d] are exactly on top of each other\n", s1->nr, s2->nr);
#endif
            return;
        } else {
#ifdef DEBUG
            fprintf(stderr, "[%d] doesn't intersect with [%d] because: they are parallel to each other\n", s1->nr, s2->nr);
#endif
            /* lines are parallel */
            return;
        }
    }

    double asign2 = LINE_EQ(s1->a, s2);
    if(asign2==0) {
        // segment1 touches segment2 in a single point (ignored)
#ifdef DEBUG
        fprintf(stderr, "Notice: segment [%d]'s start point touches segment [%d]\n", s1->nr, s2->nr);
#endif
        return;
    }
    double bsign2 = LINE_EQ(s1->b, s2);
    if(bsign2==0) {
        // segment1 touches segment2 in a single point (ignored)
#ifdef DEBUG
        fprintf(stderr, "Notice: segment [%d]'s end point touches segment [%d]\n", s1->nr, s2->nr);
#endif
        return;
    }

    if(asign2<0 && bsign2<0) {
        // segment1 is completely to the left of segment2
#ifdef DEBUG
            fprintf(stderr, "[%d] doesn't intersect with [%d] because: [%d] is completely to the left of [%d]\n", s1->nr, s2->nr, s1->nr, s2->nr);
#endif
        return;
    }
    if(asign2>0 && bsign2>0)  {
        // segment1 is completely to the right of segment2
#ifndef DONT_REMEMBER_CROSSINGS
	assert(0);
#endif
#ifdef DEBUG
            fprintf(stderr, "[%d] doesn't intersect with [%d] because: [%d] is completely to the left of [%d]\n", s1->nr, s2->nr, s2->nr, s1->nr);
#endif
        return;
    }
    
    double asign1 = LINE_EQ(s2->a, s1);
    if(asign1==0) {
        // segment2 touches segment1 in a single point (ignored)
#ifdef DEBUG
        fprintf(stderr, "Notice: segment [%d]'s start point touches segment [%d]\n", s2->nr, s1->nr);
#endif
        return;
    }
    double bsign1 = LINE_EQ(s2->b, s1);
    if(asign2==0) {
        // segment2 touches segment1 in a single point (ignored)
#ifdef DEBUG
        fprintf(stderr, "Notice: segment [%d]'s end point touches segment [%d]\n", s2->nr, s1->nr);
#endif
        return;
    }

    if(asign1<0 && bsign1<0) {
        // segment2 is completely to the left of segment1
#ifndef DONT_REMEMBER_CROSSINGS
	assert(0);
#endif
#ifdef DEBUG
            fprintf(stderr, "[%d] doesn't intersect with [%d] because: [%d] is completely to the left of [%d]\n", s1->nr, s2->nr, s1->nr, s2->nr);
#endif
        return;
    }
    if(asign1>0 && bsign1>0)  {
        // segment2 is completely to the right of segment1
#ifdef DEBUG
            fprintf(stderr, "[%d] doesn't intersect with [%d] because: [%d] is completely to the left of [%d]\n", s1->nr, s2->nr, s2->nr, s1->nr);
#endif
        return;
    }

#ifdef DONT_REMEMBER_CROSSINGS
    /* s2 crosses s1 from *left* to *right*. This is a crossing we already processed- 
       there's not way s2 would be to the left of s1 otherwise */
    if(asign1<0 && bsign1>0) return;
    if(asign2>0 && bsign2<0) return;
#endif

    assert(!(asign1<0 && bsign1>0));
    assert(!(asign2>0 && bsign2<0));

    /* TODO: should we precompute these? */
    double la = (double)s1->a.x*(double)s1->b.y - (double)s1->a.y*(double)s1->b.x;
    double lb = (double)s2->a.x*(double)s2->b.y - (double)s2->a.y*(double)s2->b.x;

    point_t p;
    p.x = (int32_t)ceil((-la*s2->delta.x + lb*s1->delta.x) / det);
    p.y = (int32_t)ceil((+lb*s1->delta.y - la*s2->delta.y) / det);

    assert(p.y >= status->y);
#ifdef CHECKS
    assert(p.x >= s1->minx && p.x <= s1->maxx);
    assert(p.x >= s2->minx && p.x <= s2->maxx);

    point_t pair;
    pair.x = s1->nr;
    pair.y = s2->nr;
#ifndef DONT_REMEMBER_CROSSINGS
    assert(!dict_contains(status->seen_crossings, &pair));
    dict_put(status->seen_crossings, &pair, 0);
#endif
#endif
#ifdef DEBUG
    fprintf(stderr, "schedule crossing between [%d] and [%d] at (%d,%d)\n", s1->nr, s2->nr, p.x, p.y);
#endif

#ifndef DONT_REMEMBER_CROSSINGS
    /* we insert into each other's intersection history because these segments might switch
       places and we still want to look them up quickly after they did */
    dict_put(&s1->scheduled_crossings, (void*)(ptroff_t)(s2->nr), 0);
    dict_put(&s2->scheduled_crossings, (void*)(ptroff_t)(s1->nr), 0);
#endif

    event_t* e = event_new();
    e->type = EVENT_CROSS;
    e->p = p;
    e->s1 = s1;
    e->s2 = s2;
    queue_put(&status->queue, e);
    return;
}

static void exchange_two(status_t*status, event_t*e)
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
    assert(s2->left == s1);
    assert(s1->right == s2);
    actlist_swap(status->actlist, s1, s2);
    assert(s2->right  == s1);
    assert(s1->left == s2);
    segment_t*left = s2->left;
    segment_t*right = s1->right;
    if(left)
        schedule_crossing(status, left, s2);
    if(right)
        schedule_crossing(status, s1, right);
}

typedef struct _box {
    point_t left1, left2, right1, right2;
} box_t;
static inline box_t box_new(int32_t x, int32_t y)
{
    box_t box;
    box.right1.x = box.right2.x = x;
    box.left1.x = box.left2.x = x-1;
    box.left1.y = box.right1.y = y-1;
    box.left2.y = box.right2.y = y;
    return box;
}

static void store_horizontal(status_t*status, point_t p1, point_t p2, edgestyle_t*fs, segment_dir_t dir, int polygon_nr);

static void append_stroke(status_t*status, point_t a, point_t b, segment_dir_t dir, edgestyle_t*fs)
{
    gfxpolystroke_t*stroke = status->strokes;
    /* find a stoke to attach this segment to. It has to have an endpoint
       matching our start point, and a matching edgestyle */
    while(stroke) {
	point_t p = stroke->points[stroke->num_points-1];
	if(p.x == a.x && p.y == a.y && stroke->fs == fs && stroke->dir == dir)
	    break;
	stroke = stroke->next;
    }
    if(!stroke) {
	stroke = rfx_calloc(sizeof(gfxpolystroke_t));
	stroke->dir = dir;
	stroke->fs = fs;
	stroke->next = status->strokes;
	status->strokes = stroke;
	stroke->points_size = 2;
	stroke->points = rfx_calloc(sizeof(point_t)*stroke->points_size);
	stroke->points[0] = a;
	stroke->num_points = 1;
    } else if(stroke->num_points == stroke->points_size) {
	assert(stroke->fs);
	stroke->points_size *= 2;
	stroke->points = rfx_realloc(stroke->points, sizeof(point_t)*stroke->points_size);
    }
    stroke->points[stroke->num_points++] = b;
}

static void insert_point_into_segment(status_t*status, segment_t*s, point_t p)
{
    assert(s->pos.x != p.x || s->pos.y != p.y);

#ifdef CHECKS
    if(!dict_contains(status->segs_with_point, s))
        dict_put(status->segs_with_point, s, 0);
    assert(s->fs_out_ok);
#endif

    if(s->pos.y != p.y) {
	/* non horizontal line- copy to output */
	if(s->fs_out) {
	    segment_dir_t dir = s->wind.is_filled?DIR_DOWN:DIR_UP;
#ifdef DEBUG
	    fprintf(stderr, "[%d] receives next point (%.2f,%.2f)->(%.2f,%.2f) (drawing (%s))\n", s->nr,
		    s->pos.x * status->gridsize, s->pos.y * status->gridsize, 
		    p.x * status->gridsize, p.y * status->gridsize,
		    dir==DIR_UP?"up":"down"
		    );
#endif
	    assert(s->pos.y != p.y);
	    append_stroke(status, s->pos, p, dir, s->fs_out);
	} else {
#ifdef DEBUG
	    fprintf(stderr, "[%d] receives next point (%.2f,%.2f) (omitting)\n", s->nr, 
		    p.x * status->gridsize, 
		    p.y * status->gridsize);
#endif
	}
    } else {
	/* horizontal line. we need to look at this more closely at the end of this
	   scanline */
	store_horizontal(status, s->pos, p, s->fs, s->dir, s->polygon_nr);
    }

    s->pos = p;
}

typedef struct _segrange {
    double xmin;
    segment_t*segmin;
    double xmax;
    segment_t*segmax;
} segrange_t;

static void segrange_adjust_endpoints(segrange_t*range, int32_t y)
{
#define XPOS_EQ(s1,s2,ypos) (XPOS((s1),(ypos))==XPOS((s2),(ypos)))
    segment_t*min = range->segmin;
    segment_t*max = range->segmax;

    /* we need this because if two segments intersect exactly on
       the scanline, segrange_test_segment_{min,max} can't tell which
       one is smaller/larger */
    if(min) while(min->left && XPOS_EQ(min, min->left, y)) {
        min = min->left;
    }
    if(max) while(max->right && XPOS_EQ(max, max->right, y)) {
        max = max->right;
    }
    range->segmin = min;
    range->segmax = max;
}
static void segrange_test_segment_min(segrange_t*range, segment_t*seg, int32_t y)
{
    if(!seg) return;
    /* we need to calculate the xpos anew (and can't use start coordinate or
       intersection coordinate), because we need the xpos exactly at the end of
       this scanline.
     */
    double x = XPOS(seg, y);
    if(!range->segmin || x<range->xmin) {
        range->segmin = seg;
        range->xmin = x;
    }
}
static void segrange_test_segment_max(segrange_t*range, segment_t*seg, int32_t y)
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
            seg = seg->right;
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
            seg = seg->left;
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
static void add_points_to_ending_segments(status_t*status, int32_t y)
{
    segment_t*seg = status->ending_segments;
    while(seg) {
        segment_t*next = seg->right;seg->right=0;

        assert(seg->b.y == status->y);

        if(status->xrow->num == 1) {
            // shortcut
	    assert(seg->b.x == status->xrow->x[0]);
            point_t p = {status->xrow->x[0], y};
            insert_point_into_segment(status, seg, p);
        } else {
            int t;
            int start=0,end=status->xrow->num,dir=1;
            if(seg->delta.x < 0) {
                start = status->xrow->num-1;
                end = dir = -1;
            }
#ifdef CHECKS
	    char ok = 0;
#endif
            for(t=start;t!=end;t+=dir) {
                box_t box = box_new(status->xrow->x[t], y);
                double d0 = LINE_EQ(box.left1, seg);
                double d1 = LINE_EQ(box.left2, seg);
                double d2 = LINE_EQ(box.right1, seg);
                double d3 = LINE_EQ(box.right2, seg);
                if(!(d0>=0 && d1>=0 && d2>=0 && d3>0 ||
                     d0<=0 && d1<=0 && d2<=0 && d3<0)) {
                    insert_point_into_segment(status, seg, box.right2);
                    //break;
#ifdef CHECKS
		    ok = 1;
#endif
                }
            }

#ifdef CHECKS
            /* we *need* to find a point to insert. the segment's own end point
               is in that list, for Pete's sake. */
            assert(ok);
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
#ifdef DEBUG
    fprintf(stderr, "range: [%d]..[%d]\n", SEGNR(range->segmin), SEGNR(range->segmax));
#endif
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
    /* test sanity: verify that we don't have changed segments
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
       that all polygons where the edgestyle changed also have seg->changed=1 */
    s = actlist_leftmost(status->actlist);
    end = 0;
#endif

    if(end)
        end = end->right;
    while(s!=end) {
#ifndef CHECKS
        if(s->changed)
#endif
        {
            segment_t* left = actlist_left(status->actlist, s);
            windstate_t wind = left?left->wind:status->windrule->start(status->context);
            s->wind = status->windrule->add(status->context, wind, s->fs, s->dir, s->polygon_nr);
            edgestyle_t*fs_old = s->fs_out;
            s->fs_out = status->windrule->diff(&wind, &s->wind);

#ifdef DEBUG
            fprintf(stderr, "[%d] dir=%s wind=%d wind.filled=%s fs_old/new=%s/%s %s\n", s->nr, s->dir==DIR_UP?"up":"down", s->wind.wind_nr, s->wind.is_filled?"fill":"nofill", 
		    fs_old?"draw":"omit", s->fs_out?"draw":"omit",
		    fs_old!=s->fs_out?"CHANGED":"");
#endif
            assert(!(!s->changed && fs_old!=s->fs_out));
            s->changed = 0;

#ifdef CHECKS
            s->fs_out_ok = 1;
#endif
        }
        s = s->right;
    }
}

/* we need to handle horizontal lines in order to add points to segments
   we otherwise would miss during the windrule re-evaluation */
static void intersect_with_horizontal(status_t*status, segment_t*h)
{
    segment_t* left = actlist_find(status->actlist, h->a, h->a);
    segment_t* right = actlist_find(status->actlist, h->b, h->b);

    /* h->a.x is not strictly necessary, as it's also done by the event */
    xrow_add(status->xrow, h->a.x);
    xrow_add(status->xrow, h->b.x);

    if(!right) {
        assert(!left);
        return;
    }

    left = actlist_right(status->actlist, left); //first seg to the right of h->a
    right = right->right; //first seg to the right of h->b
    segment_t* s = left;

    point_t o = h->a;
    while(s!=right) {
        assert(s);
        int32_t x = XPOS_INT(s, status->y);
	point_t p = {x, status->y};
#ifdef DEBUG
        fprintf(stderr, "...intersecting with [%d] (%.2f,%.2f) -> (%.2f,%.2f) at (%.2f,%.2f)\n", 
		s->nr,
                s->a.x * status->gridsize, s->a.y * status->gridsize,
                s->b.x * status->gridsize, s->b.y * status->gridsize,
                x * status->gridsize, status->y * status->gridsize
                );
#endif
        assert(x >= h->a.x);
        assert(x <= h->b.x);
        assert(s->delta.x > 0 && x >= s->a.x || s->delta.x <= 0 && x <= s->a.x);
        assert(s->delta.x > 0 && x <= s->b.x || s->delta.x <= 0 && x >= s->b.x);
        xrow_add(status->xrow, x);

	o = p;
        s = s->right;
    }
}

/* while, for a scanline, we need both starting as well as ending segments in order
   to *reconstruct* horizontal lines, we only need one or the other to *process*
   horizontal lines from the input data.

   So horizontal lines are processed twice: first they create hotpixels by intersecting
   all segments on the scanline (EVENT_HORIZTONAL). Secondly, they are processed for
   their actual content. The second also happens for all segments that received more than
   one point in this scanline.
*/
void horiz_reset(horizdata_t*horiz)
{
    horiz->num = 0;
}

void horiz_destroy(horizdata_t*horiz)
{
    if(horiz->data) rfx_free(horiz->data);
    horiz->data = 0;
}

static windstate_t get_horizontal_first_windstate(status_t*status, int x1, int x2)
{
    point_t p1 = {x1,status->y};
    point_t p2 = {x2,status->y};
    segment_t*left = actlist_find(status->actlist, p1, p2);

    segment_t*a = actlist_right(status->actlist, left);
    while(a) {
	if(a->pos.y == status->y) {
	    /* we need to iterate through all segments that received a point in this
	       scanline, as actlist_find above will miss (positively sloped) segments 
	       that are to the right of (x1,y) only as long as we don't take the 
	       hotpixel re-routing into account 
               TODO: this is inefficient, we should probably be iterating through the
	       hotpixels on this scanline.
	     */
	    if(a->pos.x == x1)
		left = a;
	    if(a->pos.x > x1)
		break;
	}
	a = a->right;
    }

    assert(!left || left->fs_out_ok);
#ifdef DEBUG
    fprintf(stderr, "  fragment %.2f..%.2f\n", 
	    x1 * status->gridsize,
	    x2 * status->gridsize);
    if(left) {
	fprintf(stderr, "    segment [%d] (%.2f,%.2f -> %.2f,%2f, at %.2f,%.2f) is to the left\n", 
		SEGNR(left), 
		left->a.x * status->gridsize,
		left->a.y * status->gridsize,
		left->b.x * status->gridsize,
		left->b.y * status->gridsize,
		left->pos.x * status->gridsize,
		left->pos.y * status->gridsize
		);
	/* this segment might be a distance away from the left point
	   of the horizontal line if the horizontal line belongs to a stroke
	   with segments that just ended (so this horizontal line appears to
	   be "floating in space" from our current point of view)
	assert(left->pos.y == h->y && left->pos.x == h->x1);
	*/
    }
#endif
    return left?left->wind:status->windrule->start(status->context);
}

static windstate_t process_horizontal_fragment(status_t*status, horizontal_t*h, int x1, int x2, windstate_t below)
{
    windstate_t above = status->windrule->add(status->context, below, h->fs, h->dir, h->polygon_nr);
    edgestyle_t*fs = status->windrule->diff(&above, &below);
	
    segment_dir_t dir = above.is_filled?DIR_DOWN:DIR_UP;
    point_t p1 = {x1,h->y};
    point_t p2 = {x2,h->y};

    if(fs) {
	//append_stroke(status, p1, p2, DIR_INVERT(h->dir), fs);
	append_stroke(status, p1, p2, dir, fs);
    }
#ifdef DEBUG
    fprintf(stderr, "    ...%s (below: (wind_nr=%d, filled=%d), above: (wind_nr=%d, filled=%d) %s %d-%d\n",
	    fs?"storing":"ignoring",
	    below.wind_nr, below.is_filled,
	    above.wind_nr, above.is_filled, 
	    dir==DIR_UP?"up":"down", x1, x2);
#endif
    return above;
}

typedef enum {hevent_hotpixel,hevent_end,hevent_start} horizontal_event_type_t;
typedef struct _hevent {
    int32_t x;
    horizontal_t*h;
    horizontal_event_type_t type;
} hevent_t;

typedef struct _hevents {
    hevent_t*events;
    int num;
} hevents_t;

static int compare_hevents(const void *_e1, const void *_e2)
{
    hevent_t*e1 = (hevent_t*)_e1;
    hevent_t*e2 = (hevent_t*)_e2;
    int diff = e1->x - e2->x;
    if(diff) return diff;
    return e1->type - e2->type; //schedule hotpixel before hend
}

static hevents_t hevents_fill(status_t*status)
{
    horizdata_t*horiz = &status->horiz;
    xrow_t*xrow = status->xrow;

    hevents_t e;
    e.events = malloc(sizeof(hevent_t)*(horiz->num*2 + xrow->num));
    e.num = 0;

    int t;
    for(t=0;t<horiz->num;t++) {
	assert(horiz->data[t].x1 != horiz->data[t].x2);
	e.events[e.num].x = horiz->data[t].x1;
	e.events[e.num].h = &horiz->data[t];
	e.events[e.num].type = hevent_start;
	e.num++;
	e.events[e.num].x = horiz->data[t].x2;
	e.events[e.num].h = &horiz->data[t];
	e.events[e.num].type = hevent_end;
	e.num++;
    }
    for(t=0;t<xrow->num;t++) {
	e.events[e.num].x = status->xrow->x[t];
	e.events[e.num].h = 0;
	e.events[e.num].type = hevent_hotpixel;
	e.num++;
    }
    qsort(e.events, e.num, sizeof(hevent_t), compare_hevents);
    return e;

}

static void process_horizontals(status_t*status)
{
    horizdata_t*horiz = &status->horiz;

    if(!horiz->num)
	return;

    hevents_t events = hevents_fill(status);
    int num_open = 0;
    horizontal_t**open = malloc(sizeof(horizontal_t*)*horiz->num);

    int s,t;
    for(t=0;t<events.num;t++) {
	hevent_t*e = &events.events[t];
	switch(e->type) {
	    case hevent_start:
		e->h->pos = num_open;
		open[num_open++] = e->h;
#ifdef DEBUG
		fprintf(stderr, "horizontal (y=%.2f): %.2f -> %.2f dir=%s fs=%p\n", 
			e->h->y * status->gridsize,
			e->h->x1 * status->gridsize,
			e->h->x2 * status->gridsize,
			e->h->dir==DIR_UP?"up":"down", e->h->fs);
#endif
		assert(e->h->y == status->y);
		assert(xrow_contains(status->xrow, e->h->x1));
		assert(xrow_contains(status->xrow, e->h->x2));
	    break;
	    case hevent_end:
		num_open--;
		if(num_open) {
		    open[num_open]->pos = e->h->pos;
		    open[e->h->pos] = open[num_open];
		}
	    break;
	    case hevent_hotpixel:
	        {
		    windstate_t	below;
		    for(s=0;s<num_open;s++) {
			int x1 = open[s]->xpos;
			int x2 = e->x;
			assert(status->y == open[s]->y);
			if(!s)
			    below = get_horizontal_first_windstate(status, x1, x2);
			open[s]->xpos = e->x;
			assert(x1 < x2);
			below = process_horizontal_fragment(status, open[s], x1, x2, below);
		    }
		}
	    break;
	}
    }
    free(open);
    free(events.events);
}

static void store_horizontal(status_t*status, point_t p1, point_t p2, edgestyle_t*fs, segment_dir_t dir, int polygon_nr)
{
    assert(p1.y == p2.y);
    assert(p1.x != p2.x); // TODO: can this happen?

    if(p1.x > p2.x) {
	dir = DIR_INVERT(dir);
	point_t p_1 = p1;
	point_t p_2 = p2;
	p1 = p_2;
	p2 = p_1;
    }

    /* TODO: convert this into a linked list */
    if(status->horiz.size == status->horiz.num) {
	if(!status->horiz.size)
	    status->horiz.size = 16;
	status->horiz.size *= 2;
	status->horiz.data = rfx_realloc(status->horiz.data, sizeof(status->horiz.data[0])*status->horiz.size);
    }
    horizontal_t*h = &status->horiz.data[status->horiz.num++];
    h->y = p1.y;
    h->xpos = p1.x;
    h->x1 = p1.x;
    h->x2 = p2.x;
    h->fs = fs;
    h->dir = dir;
    h->polygon_nr = polygon_nr;
}


static void event_apply(status_t*status, event_t*e)
{
#ifdef DEBUG
    event_dump(status, e);
#endif

    switch(e->type) {
        case EVENT_HORIZONTAL: {
            segment_t*s = e->s1;
            intersect_with_horizontal(status, s);
	    store_horizontal(status, s->a, s->b, s->fs, s->dir, s->polygon_nr);
	    advance_stroke(&status->queue, 0, s->stroke, s->polygon_nr, s->stroke_pos, status->gridsize);
            segment_destroy(s);e->s1=0;
            break;
        }
        case EVENT_END: {
            //delete segment from list
            segment_t*s = e->s1;
#ifdef CHECKS
            dict_del(status->intersecting_segs, s);
            dict_del(status->segs_with_point, s);
            assert(!dict_contains(status->intersecting_segs, s));
            assert(!dict_contains(status->segs_with_point, s));
#endif
            segment_t*left = s->left;
            segment_t*right = s->right;
            actlist_delete(status->actlist, s);
            if(left && right)
                schedule_crossing(status, left, right);

	    /* schedule segment for xrow handling */
            s->left = 0; s->right = status->ending_segments;
            status->ending_segments = s;
	    advance_stroke(&status->queue, 0, s->stroke, s->polygon_nr, s->stroke_pos, status->gridsize);
            break;
        }
        case EVENT_START: {
            //insert segment into list
            segment_t*s = e->s1;
	    assert(e->p.x == s->a.x && e->p.y == s->a.y);
            actlist_insert(status->actlist, s->a, s->b, s);
            segment_t*left = s->left;
            segment_t*right = s->right;
            if(left)
                schedule_crossing(status, left, s);
            if(right)
                schedule_crossing(status, s, right);
            schedule_endpoint(status, s);
            break;
        }
        case EVENT_CROSS: {
            // exchange two segments
            if(e->s1->right == e->s2) {
		assert(e->s2->left == e->s1);
                exchange_two(status, e);
            } else {
		assert(e->s2->left != e->s1);
#ifdef DEBUG
		fprintf(stderr, "Ignore this crossing ([%d] not next to [%d])\n", e->s1->nr, e->s2->nr);
#endif
#ifndef DONT_REMEMBER_CROSSINGS
                /* ignore this crossing for now (there are some line segments in between).
                   it'll get rescheduled as soon as the "obstacles" are gone */
                char del1 = dict_del(&e->s1->scheduled_crossings, (void*)(ptroff_t)e->s2->nr);
                char del2 = dict_del(&e->s2->scheduled_crossings, (void*)(ptroff_t)e->s1->nr);
                assert(del1 && del2);
#endif
#ifdef CHECKS
                point_t pair;
                pair.x = e->s1->nr;
                pair.y = e->s2->nr;
#ifndef DONT_REMEMBER_CROSSINGS
                assert(dict_contains(status->seen_crossings, &pair));
                dict_del(status->seen_crossings, &pair);
#endif
#endif
            }
        }
    }
}

#ifdef CHECKS
static void check_status(status_t*status)
{
    DICT_ITERATE_KEY(status->intersecting_segs, segment_t*, s) {
        if((s->pos.x != s->b.x ||
            s->pos.y != s->b.y) &&
           !dict_contains(status->segs_with_point, s)) {
            fprintf(stderr, "Error: segment [%d] (%sslope) intersects in scanline %d, but it didn't receive a point\n",
                    SEGNR(s),
                    s->delta.x<0?"-":"+",
                    status->y);
            assert(0);
        }
    }
}
#endif

gfxpoly_t* gfxpoly_process(gfxpoly_t*poly1, gfxpoly_t*poly2, windrule_t*windrule, windcontext_t*context)
{
    current_polygon = poly1;

    status_t status;
    memset(&status, 0, sizeof(status_t));
    status.gridsize = poly1->gridsize;
    
    queue_init(&status.queue);
    gfxpoly_enqueue(poly1, &status.queue, 0, /*polygon nr*/0);
    if(poly2) {
	assert(poly1->gridsize == poly2->gridsize);
	gfxpoly_enqueue(poly2, &status.queue, 0, /*polygon nr*/1);
    }

    status.windrule = windrule;
    status.context = context;
    status.actlist = actlist_new();

#ifdef CHECKS
    status.seen_crossings = dict_new2(&point_type);
    int32_t lasty=-0x80000000;
#endif

    status.xrow = xrow_new();

    event_t*e = queue_get(&status.queue);
    while(e) {
	assert(e->s1->fs);
        status.y = e->p.y;
#ifdef CHECKS
	assert(status.y>=lasty);
	lasty = status.y;
        status.intersecting_segs = dict_new2(&ptr_type);
        status.segs_with_point = dict_new2(&ptr_type);
#endif

#ifdef DEBUG
        fprintf(stderr, "----------------------------------- %.2f\n", status.y * status.gridsize);
        actlist_dump(status.actlist, status.y-1, status.gridsize);
#endif
#ifdef CHECKS
        actlist_verify(status.actlist, status.y-1);
#endif
        xrow_reset(status.xrow);
	horiz_reset(&status.horiz);

        do {
            xrow_add(status.xrow, e->p.x);
            event_apply(&status, e);
	    event_free(e);
            e = queue_get(&status.queue);
        } while(e && status.y == e->p.y);

        xrow_sort(status.xrow);
        segrange_t range;
        memset(&range, 0, sizeof(range));
#ifdef DEBUG
        actlist_dump(status.actlist, status.y, status.gridsize);
	xrow_dump(status.xrow, status.gridsize);
#endif
        add_points_to_positively_sloped_segments(&status, status.y, &range);
        add_points_to_negatively_sloped_segments(&status, status.y, &range);
        add_points_to_ending_segments(&status, status.y);

        recalculate_windings(&status, &range);
        
	actlist_verify(status.actlist, status.y);
	process_horizontals(&status);
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
    queue_destroy(&status.queue);
    horiz_destroy(&status.horiz);
    xrow_destroy(status.xrow);

    gfxpoly_t*p = (gfxpoly_t*)malloc(sizeof(gfxpoly_t));
    p->gridsize = poly1->gridsize;
    p->strokes = status.strokes;

#ifdef CHECKS
    /* we only add segments with non-empty edgestyles to strokes in
       recalculate_windings, but better safe than sorry */
    gfxpolystroke_t*stroke = p->strokes;
    while(stroke) {
	assert(stroke->fs);
	stroke = stroke->next;
    }
#endif
    return p;
}

static windcontext_t twopolygons = {2};
gfxpoly_t* gfxpoly_intersect(gfxpoly_t*p1, gfxpoly_t*p2)
{
    return gfxpoly_process(p1, p2, &windrule_intersect, &twopolygons);
}
gfxpoly_t* gfxpoly_union(gfxpoly_t*p1, gfxpoly_t*p2)
{
    return gfxpoly_process(p1, p2, &windrule_union, &twopolygons);
}
