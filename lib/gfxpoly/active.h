#ifndef __active_h__
#define __active_h__

#include "poly.h"
#include "splay.h"

typedef struct _actlist
{
    //SPLAY_HEAD(root, actnode_t);
    segment_t*list;
} actlist_t;

actlist_t* actlist_new();
void actlist_verify_and_dump(actlist_t*a, int32_t y);
segment_t* actlist_find(actlist_t*a, point_t p1, point_t p2);  // finds segment immediately to the left of p1 (breaking ties w/ p2)
void actlist_insert(actlist_t*a, point_t p, segment_t*s);
void actlist_delete(actlist_t*a, segment_t*s);
void actlist_swap(actlist_t*a, segment_t*s1, segment_t*s2);
void actlist_invert_fromto(actlist_t*a, segment_t*s1, segment_t*s2);
segment_t* actlist_left(actlist_t*a, segment_t*s);
segment_t* actlist_leftmost(actlist_t*a);
segment_t* actlist_right(actlist_t*a, segment_t*s);

#endif
