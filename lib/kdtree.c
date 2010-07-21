#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <limits.h>
#include "q.h"
#include "kdtree.h"

#define KDTREE_SIDE1(t,x,y) 

/*
              |          |
              |          |
              |          |
              |          |
              |          |------------
              |          |
--------------|          |
              |          |
              |          |
              |          |
              |          |
              |-----------------------
              |
              |
              |
              |

*/

/* right down left up */
static int vx_and[4] = {INT_MAX, 0, INT_MAX, 0};
static int vy_and[4] = {0, INT_MAX, 0, INT_MAX};
static int vx[4] = {1, 0, -1, 0};
static int vy[4] = {0, 1, 0, -1};
static int vsign[4] = {1,1,-1,-1};
static char* vname[4] = {"right", "down", "left", "up"};

kdarea_t* kdarea_new()
{
    NEW(kdarea_t,area);
    area->bbox.xmin = INT_MIN;
    area->bbox.ymin = INT_MIN;
    area->bbox.xmax = INT_MAX;
    area->bbox.ymax = INT_MAX;
    return area;
}

kdtree_t* kdtree_new()
{
    NEW(kdtree_t,tree);
    tree->root = kdarea_new();
    return tree;
}

#define IS_LEAF(l) ((l)->type == KD_LEAF)

static inline int32_t max32(int32_t v1, int32_t v2) {return v1>v2?v1:v2;}
static inline int32_t min32(int32_t v1, int32_t v2) {return v1<v2?v1:v2;}

kdbranch_t*kdbranch_new(int32_t xy, kdtype_t type)
{
    NEW(kdbranch_t,b);
    b->type = type;
    b->xy = xy;
    return b;
}

kdarea_t*kdbranch_follow(kdbranch_t*tree, int32_t x, int32_t y)
{
    /*
    int follow = 0;
    switch(tree->type) {
	case KD_LEFT:
	    follow = (x < tree->xy);
	break;
	case KD_RIGHT:
	    follow = (x > tree->xy);
	break;
	case KD_UP:
	    follow = (y < tree->xy);
	break;
	case KD_DOWN:
	    follow = (y > tree->xy);
	break;
    }
    return &tree->side[follow];
    */
    int32_t s = x*vx[tree->type] + y*vy[tree->type];
    if(s == tree->xy*vsign[tree->type])
	return 0;
    return tree->side[s < tree->xy*vsign[tree->type]];
}

static kdarea_list_t* kdarea_list_new(kdarea_t*area)
{
    NEW(kdarea_list_t,b);
    b->area = area;
    b->next = b->prev = b;
    return b;
}

static kdarea_list_t*kdarea_list_concatenate(kdarea_list_t*l1, kdarea_list_t*l2)
{
/*
   l1->prev       l2->prev   |   l1->prev       l2->prev   
    |             |          |    |       +-----+          
    V             V          |    V       |                
   l1             l2         |   l1  -------->  l2
    |             |          |            |     |          
    |             |          |    +-------+     |          
    V             V          |    V             V          
   l1->next       l2->next   |   l1->next       l2->next   
*/
    if(!l1) return l2;
    if(!l2) return l1;
    l2->prev->next = l1->next;
    l1->next->prev = l2->prev;
    l2->prev = l1;
    l1->next = l2;
    return l1;
}

kdbbox_t bbox_for_halfplane(int xy, int dir)
{
    kdbbox_t b = {INT_MIN,INT_MIN,INT_MAX,INT_MAX};
    switch(dir) {
	case KD_LEFT:
	    b.xmax = xy;
	break;
	case KD_RIGHT:
	    b.xmin = xy;
	break;
	case KD_UP:
	    b.ymax = xy;
	break;
	case KD_DOWN:
	    b.ymin = xy;
	break;
    }
    return b;
}

kdbbox_t intersect_bbox(kdbbox_t*box1, kdbbox_t*box2)
{
    kdbbox_t b = *box1;
    if(box2->xmin > b.xmin)
	b.xmin = box2->xmin;
    if(box2->ymin > b.ymin)
	b.ymin = box2->ymin;
    if(box2->xmax < b.xmax)
	b.xmax = box2->xmax;
    if(box2->ymax < b.ymax)
	b.ymax = box2->ymax;
    if(b.xmin > b.xmax)
	b.xmax = b.xmin;
    if(b.ymin > b.ymax)
	b.ymax = b.ymin;
    return b;
}

void add_line(kdarea_t*area, int xy, int dir,
			     int32_t x1, int32_t y1, 
			     int32_t x2, int32_t y2)
{
    if(!area->split) {
	kdbbox_t b1 = bbox_for_halfplane(xy, dir);
	kdbbox_t b2 = bbox_for_halfplane(xy, dir^2);
	kdbranch_t*b = area->split = kdbranch_new(xy, dir);
	b->side[0] = kdarea_new();
	b->side[1] = kdarea_new();
	b->side[0]->bbox = intersect_bbox(&area->bbox,&b1);
	b->side[1]->bbox = intersect_bbox(&area->bbox,&b2);
	kdbbox_t c = b->side[0]->bbox;
	/*printf("intersect (%d,%d,%d,%d) and (%d,%d,%d,%d) -> (%d,%d,%d,%d)\n", 
		area->bbox.xmin, area->bbox.ymin, area->bbox.xmax, area->bbox.ymax,
		b1.xmin, b1.ymin, b1.xmax, b1.ymax,
		c.xmin, c.ymin, c.xmax, c.ymax
		);*/
	memcpy(b->side[0]->neighbors, area->neighbors, sizeof(area->neighbors));
	memcpy(b->side[1]->neighbors, area->neighbors, sizeof(area->neighbors));
	b->side[0]->neighbors[dir^2] = b->side[1];
	b->side[1]->neighbors[dir] = b->side[0];
    } else {
	kdbranch_t*split = area->split;
	kdarea_t*first = kdbranch_follow(split, x1,y1);

	if(!first) {
	    kdarea_t*second = kdbranch_follow(split, x2,y2);
	    if(!second) {
		/* line is on top of an already existing segment */
		return;
	    } else {
		/* first point is directly on the split */
		add_line(second, xy, dir, x1,y1, x2,y2);
		return;
	    }
	} else {
	    kdarea_t*second = kdbranch_follow(split, x2,y2);
	    if(!second) {
		/* second point is directly on the split */
		add_line(first, xy, dir, x1,y1, x2,y2);
		return;
	    } else if(first == second) {
		/* both points are to the same side of this split */
		add_line(first, xy, dir, x1,y1, x2,y2);
		return;
	    } else {
		add_line(first, xy, dir, x1,y1, x2,y2);
		add_line(second, xy, dir, x1,y1, x2,y2);
		return;
	    }
	}
    }
}

kdarea_list_t* filter_tree(kdarea_t*area, int xy, int dir, void*data)
{
    if(!area->split) {
	return kdarea_list_new(area);
    } else {
	kdbranch_t*branch = area->split;
	if((branch->type^dir) == 0) {
	    /* both filter as well as branch point into the same direction */
	    if(xy*vsign[dir] >= branch->xy*vsign[dir]) {
		/* filter splits the primary node. We can skip the other one. */
		return filter_tree(branch->side[0], xy, dir, data);
	    } else {
		/* filter splits the secondary node. the primary node is left completely intact, 
		   and returned as such */
		kdarea_list_t*l1 = kdarea_list_new(branch->side[0]);
		kdarea_list_t*l2 = filter_tree(branch->side[1], xy, dir, data);
		return kdarea_list_concatenate(l1,l2);
	    }
	} else if((branch->type^dir) == 1) {
	    /* filter and branch point into opposite directions */
	    if(xy*vsign[dir] <= branch->xy*vsign[dir]) {
		// filter splits the secondary node. We can skip the primary node.
		return filter_tree(branch->side[1], xy, dir, data);
	    } else {
		/* filter splits the primary node. the secondary node is left completely intact, 
		   and returned as such */
		kdarea_list_t*l1 = filter_tree(branch->side[0], xy, dir, data);
		kdarea_list_t*l2 = kdarea_list_new(branch->side[1]);
		return kdarea_list_concatenate(l1,l2);
	    }
	}
	/* filter segment is perpendicular to the node */
	kdarea_list_t*l1 = filter_tree(branch->side[0], xy, dir, data);
	kdarea_list_t*l2 = filter_tree(branch->side[1], xy, dir, data);
	return kdarea_list_concatenate(l1,l2);
    }
}

void kdtree_add_box(kdtree_t*tree, int32_t x1, int32_t y1, int32_t x2, int32_t y2, void*data)
{
    add_line(tree->root, x2, KD_LEFT,  x2,y1, x2,y2);
    add_line(tree->root, y2, KD_UP,    x1,y2, x2,y2);
    add_line(tree->root, x1, KD_RIGHT, x1,y1, x1,y2);
    add_line(tree->root, y1, KD_DOWN,  x1,y1, x2,y1);

    kdarea_list_t*branches1 = filter_tree(tree->root, x2, KD_LEFT, data);
    kdarea_list_t*i = branches1;
    if(i) do {
	kdarea_list_t*branches2 = filter_tree(i->area, y2, KD_UP, data);
	kdarea_list_t*j = branches2;
	if(j) do {
	    kdarea_list_t*branches3 = filter_tree(i->area, x1, KD_RIGHT, data);
	    kdarea_list_t*k = branches3;
	    if(k) do {
		kdarea_list_t*branches4 = filter_tree(i->area, y1, KD_DOWN, data);
		kdarea_list_t*l = branches4;
		if(l) do {
		    // ...
		} while(l!=branches4);
		k = k->next;
	    } while(k!=branches3);
	    j = j->next;
	} while(j!=branches2);
	i = i->next;
    } while(i!=branches1);
}

kdarea_t*kdarea_find(kdarea_t*area, int x, int y)
{
    if(!area->split)
	return area;
    else
	return kdarea_find(kdbranch_follow(area->split, x,y), x,y);
}

kdarea_t*kdtree_find(kdtree_t*tree, int x, int y)
{
    return kdarea_find(tree->root, x,y);
}

kdarea_t*kdarea_neighbor(kdarea_t*area, int dir, int xy)
{
    int x,y;
    switch(dir) {
	case KD_LEFT:
	    x = area->bbox.xmin;
	    y = xy;
	break;
	case KD_RIGHT:
	    x = area->bbox.xmax;
	    y = xy;
	break;
	case KD_UP:
	    x = xy;
	    y = area->bbox.ymin;
	break;
	case KD_DOWN:
	    x = xy;
	    y = area->bbox.ymax;
	break;
    }
    kdarea_t*n = area->neighbors[dir];
    if(!n)
	return 0;
    return kdarea_find(n, x, y);
}

static void do_indent(int l)
{
    int i;
    for(i=0;i<l;i++) 
	printf(" ");
}

void kdarea_print(kdarea_t*area, int indent);
void kdbranch_print(kdbranch_t*branch, int indent)
{
    do_indent(indent);printf("[%08x] branch (%s, %d)\n", branch, vname[branch->type], branch->xy);
    kdbbox_t b = bbox_for_halfplane(branch->xy, branch->type);
    kdarea_print(branch->side[0], indent+4);
    kdarea_print(branch->side[1], indent+4);
}
void kdarea_print(kdarea_t*area, int indent)
{
    int i;
    assert(area);
    do_indent(indent);printf("[%08x] leaf (%d,%d,%d,%d) (l:%08x r:%08x u:%08x d:%08x)\n", area,
	    area->bbox.xmin,
	    area->bbox.ymin,
	    area->bbox.xmax,
	    area->bbox.ymax,
	    area->neighbors[KD_LEFT],
	    area->neighbors[KD_RIGHT],
	    area->neighbors[KD_UP],
	    area->neighbors[KD_DOWN]);
    if(area->split) {
	kdbranch_print(area->split, indent+4);
    }
}

void kdtree_print(kdtree_t*tree)
{
    kdarea_print(tree->root, 0);
}

#ifdef MAIN
int main()
{
    assert((1^vx[2]) < 0);

    kdtree_t*tree = kdtree_new();
    kdtree_add_box(tree, 10,30,20,40, "hello world");
    kdtree_print(tree);
    kdarea_t*a = kdtree_find(tree, 15,35);
    kdarea_t*left = kdarea_neighbor(a, KD_LEFT, /*y*/35);
    kdarea_t*right = kdarea_neighbor(a, KD_RIGHT, /*y*/35);
    kdarea_t*up = kdarea_neighbor(a, KD_UP, /*x*/15);
    kdarea_t*down = kdarea_neighbor(a, KD_DOWN, /*x*/15);
}
#endif
