#include <assert.h>
#include "../q.h"
#include "active.h"

actlist_t* actlist_new()
{
    NEW(actlist_t, a);
    return a;
}

void actlist_verify_and_dump(actlist_t*a, int32_t y)
{
    segment_t*s = a->list;
    assert(!s || !s->left);
    double lastx;
    while(s) {
        if(y) {
            double x = ((double)s->delta.x*(y-s->a.y)/s->delta.y)+s->a.x;
            if(s!=a->list) {
                if(lastx>x) printf("?%f<->%f? ", lastx, x);
            }
            lastx = x;
        }
        assert(!s->left || s->left->right == s);
        assert(!s->right || s->right->left == s);
        printf("[%d]", s->nr);
        s = s->right;
        if(s) printf(" ");
        else printf("\n");
    }
}

segment_t* actlist_find(actlist_t*a, point_t p1, point_t p2)
{
    /* this runs in O(n) right now, and should be optimized using a splay
       tree to run in ammortized O(log(n)) */
    segment_t*last=0, *s = a->list;
    if(!s) return last;
    while(s) {
        double d = LINE_EQ(p1, s);
        if(d==0) {
            d = LINE_EQ(p2, s);
            if(d==0) {
                /* We default to always inserting the new segment to the right of the old segment.
                   We do this so that we don't place new segments into the middle of already
                   overlapping lines which may have intersections scheduled.
                 */
                //fprintf(stderr, "Notice: actlist_find: both points (%d,%d) and (%d,%d) exactly on segment [%d]\n", p1.x, p1.y, p2.x, p2.y, s->nr);
            }
        }
        if(d<0)
            break;
        last = s;
        s = s->right;
    }
    return last;
}

static void actlist_insert_after(actlist_t*a, segment_t*left, segment_t*s)
{
    s->left = left;
    if(left) {
        s->right = left->right;
    } else {
        s->right = a->list;
        a->list = s;
    }
    if(s->left) 
        s->left->right = s;
    if(s->right) 
        s->right->left = s;
}

void actlist_insert(actlist_t*a, point_t p, segment_t*s)
{
    segment_t*left = actlist_find(a, p, s->b);
    actlist_insert_after(a, left, s);
}

void actlist_delete(actlist_t*a, segment_t*s)
{
    if(s->left) {
        s->left->right = s->right;
    } else {
        a->list = s->right;
    }
    if(s->right) {
        s->right->left = s->left;
    }
    s->left = s->right = 0;
}

segment_t* actlist_leftmost(actlist_t*a)
{
    return a->list;
}

segment_t* actlist_left(actlist_t*a, segment_t*s)
{
    return s->left;
}

segment_t* actlist_right(actlist_t*a, segment_t*s)
{
    return s->right;
}

void actlist_swap(actlist_t*a, segment_t*s1, segment_t*s2)
{
    actlist_delete(a, s1);
    actlist_insert_after(a, s2, s1);
}

void actlist_invert_fromto(actlist_t*a, segment_t*s1, segment_t*s2)
{
    segment_t*left = s1->left;
    segment_t*right = s2->right;
    segment_t*s = s1;
    assert(s!=s2);
    while(1) {
        assert(s);
        segment_t*l = s->left;
        segment_t*r = s->right;
        s->left = r;
        s->right = l;
        if(s==s2)
            break;
        s = r;
    }
    s2->left = left;
    s1->right = right;
    if(left) {
        left->right = s2;
    } else {
        assert(a->list == s1);
        a->list = s2; 
    }
    if(right) {
        right->left = s1;
    }
}
