/* kdtree.c
   Implementation of 2d kd trees.

   Part of the swftools package.
   
   Copyright (c) 2010 Matthias Kramm <kramm@quiss.org> 
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <limits.h>
#include "q.h"
#include "kdtree.h"

/* 0=right 1=down 2=left 3=up */
static int vx_and[4] = {INT_MAX, 0, INT_MAX, 0};
static int vy_and[4] = {0, INT_MAX, 0, INT_MAX};
static int vx[4] = {1, 0, -1, 0};
static int vy[4] = {0, 1, 0, -1};
static int vsign[4] = {1,1,-1,-1};
static char* vname[4] = {"right", "down", "left", "up"};

kdarea_t* kdarea_new(void*data)
{
    NEW(kdarea_t,area);
    area->bbox.xmin = INT_MIN;
    area->bbox.ymin = INT_MIN;
    area->bbox.xmax = INT_MAX;
    area->bbox.ymax = INT_MAX;
    area->data = data;
    return area;
}

kdtree_t* kdtree_new()
{
    NEW(kdtree_t,tree);
    tree->root = kdarea_new(0);
    return tree;
}

static inline int32_t max32(int32_t v1, int32_t v2) {return v1>v2?v1:v2;}
static inline int32_t min32(int32_t v1, int32_t v2) {return v1<v2?v1:v2;}

kdbranch_t*kdbranch_new(int32_t xy, kdtype_t type)
{
    NEW(kdbranch_t,b);
    b->type = type;
    b->xy = xy;
    return b;
}

kdarea_t*kdbranch_follow(const kdbranch_t*tree, int32_t x, int32_t y)
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
    int32_t v = tree->xy*vsign[tree->type];
    return tree->side[s < v];
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
    if(!l1) return l2;
    if(!l2) return l1;
    l2->prev->next = l1->next;
    l1->next->prev = l2->prev;
    l2->prev = l1;
    l1->next = l2;
    return l1;
}

static kdbbox_t bbox_for_halfplane(int xy, int dir)
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

static kdbbox_t intersect_bbox(const kdbbox_t*box1, const kdbbox_t*box2)
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

static void kdarea_split(kdarea_t*area, int xy, int dir,
			     int32_t x1, int32_t y1, 
			     int32_t x2, int32_t y2)
{
    if(!area->split) {
	kdbranch_t*b = area->split = kdbranch_new(xy, dir);
	kdbbox_t b1 = bbox_for_halfplane(xy, dir);
	kdbbox_t b2 = bbox_for_halfplane(xy, dir^2);
	b->side[0] = kdarea_new(area->data);
	b->side[1] = kdarea_new(area->data);
	b->side[0]->bbox = intersect_bbox(&area->bbox,&b1);
	b->side[1]->bbox = intersect_bbox(&area->bbox,&b2);
	memcpy(b->side[0]->neighbors, area->neighbors, sizeof(area->neighbors));
	memcpy(b->side[1]->neighbors, area->neighbors, sizeof(area->neighbors));
	b->side[0]->neighbors[dir^2] = b->side[1];
	b->side[1]->neighbors[dir] = b->side[0];
	area->data = 0;
    } else {
	kdbranch_t*split = area->split;
	kdarea_t*first = kdbranch_follow(split, x1,y1);
	kdarea_t*second = kdbranch_follow(split, x2,y2);

	if(!first) {
	    if(!second) {
		/* line is on top of an already existing segment */
		return;
	    } else {
		/* first point is directly on the split */
		kdarea_split(second, xy, dir, x1,y1, x2,y2);
		return;
	    }
	} else {
	    if(!second) {
		/* second point is directly on the split */
		kdarea_split(first, xy, dir, x1,y1, x2,y2);
		return;
	    } else if(first == second) {
		/* both points are to the same side of this split */
		kdarea_split(first, xy, dir, x1,y1, x2,y2);
		return;
	    } else {
		kdarea_split(first, xy, dir, x1,y1, x2,y2);
		kdarea_split(second, xy, dir, x1,y1, x2,y2);
		return;
	    }
	}
    }
}

static kdarea_list_t* kdarea_filter(kdarea_t*area, int xy, int dir)
{
    if(!area->split) {
	return kdarea_list_new(area);
    } else {
	kdbranch_t*branch = area->split;
	if((branch->type^dir) == 0) {
	    /* both filter as well as branch point into the same direction */
	    if(xy*vsign[dir] >= branch->xy*vsign[dir]) {
		/* filter splits the primary node. We can skip the other one. */
#ifdef DEBUG
                printf("%p: using %p, skipping %p (looking to %s of %d)\n", area, branch->side[0], branch->side[1], vname[dir], xy);
#endif
		return kdarea_filter(branch->side[0], xy, dir);
	    } else {
		/* filter splits the secondary node. the primary node is left completely intact, 
		   and returned as such */
#ifdef DEBUG
                printf("%p: expanding %p, filtering %p (looking to %s of %d)\n", area, branch->side[0], branch->side[1], vname[dir], xy);
#endif
		kdarea_list_t*l1 = kdarea_list_new(branch->side[0]);
		kdarea_list_t*l2 = kdarea_filter(branch->side[1], xy, dir);
		return kdarea_list_concatenate(l1,l2);
	    }
	} else if((branch->type^dir) == 2) {
	    /* filter and branch point into opposite directions */
	    if(xy*vsign[dir] >= branch->xy*vsign[dir]) {
		// filter splits the secondary node. We can skip the primary node.
#ifdef DEBUG
                printf("%p: skipping %p, using %p (looking to %s of %d)\n", area, branch->side[0], branch->side[1], vname[dir], xy);
#endif
		return kdarea_filter(branch->side[1], xy, dir);
	    } else {
		/* filter splits the primary node. the secondary node is left completely intact, 
		   and returned as such */
#ifdef DEBUG
                printf("%p: filtering %p, expanding %p (looking to %s of %d)\n", area, branch->side[0], branch->side[1], vname[dir], xy);
#endif
		kdarea_list_t*l1 = kdarea_filter(branch->side[0], xy, dir);
		kdarea_list_t*l2 = kdarea_list_new(branch->side[1]);
		return kdarea_list_concatenate(l1,l2);
	    }
	} else {
            /* filter segment is perpendicular to the node */
            return kdarea_list_new(area);
        }
    }
}

static kdarea_t* kdarea_find(kdarea_t*node, int x, int y)
{
    while(node) {
	if(!node->split)
	    break;
	node = kdbranch_follow(node->split, x,y);
    }
    return node;
}

kdarea_t*kdtree_find(kdtree_t*tree, int x, int y)
{
    return kdarea_find(tree->root, x,y);
}

void kdarea_list_destroy(kdarea_list_t*list)
{
    kdarea_list_t*i = list;
    if(i) do {
	kdarea_list_t*next = i->next;
	free(i);
	i = next;
    } while(i!=list);
}

static kdarea_list_t* kdarea_list_add(kdarea_list_t*l, kdarea_t*area)
{
    return kdarea_list_concatenate(l,kdarea_list_new(area));
}

static kdarea_list_t* kdarea_all_children(kdarea_t*area, int32_t x1, int32_t y1, int32_t x2, int32_t y2, kdarea_list_t*result)
{
    if(!area->split) {
        if(area->bbox.xmin >= x1 &&
           area->bbox.ymin >= y1 &&
           area->bbox.xmax <= x2 &&
           area->bbox.ymax <= y2) {
	    result = kdarea_list_add(result, area);
        }
    } else {
	result = kdarea_all_children(area->split->side[0], x1, y1, x2, y2, result);
	result = kdarea_all_children(area->split->side[1], x1, y1, x2, y2, result);
    }
    return result;
}

/* return all areas that are contained in, or partly intersect, the given bounding box */
kdarea_list_t* kdtree_filter(kdtree_t*tree, int32_t x1, int32_t y1, int32_t x2, int32_t y2, char leafs)
{
    kdarea_list_t*result = 0;
    kdarea_list_t*branches1 = kdarea_filter(tree->root, x2, KD_LEFT);
    kdarea_list_t*i = branches1;

#ifdef DEBUG
    kdarea_list_t*u = branches1;
    if(u) do {printf("%p [%d %d %d %d] is to the left of %d\n", u->area, u->area->bbox.xmin, u->area->bbox.ymin, u->area->bbox.xmax, u->area->bbox.ymax, x2);u = u->next;} while(u!=branches1);
#endif
    if(i) do {
	kdarea_list_t*branches2 = kdarea_filter(i->area, y2, KD_UP);
	kdarea_list_t*j = branches2;

#ifdef DEBUG
        kdarea_list_t*u = branches2;
        if(u) do {printf("%p [%d %d %d %d] is above %d\n", u->area, u->area->bbox.xmin, u->area->bbox.ymin, u->area->bbox.xmax, u->area->bbox.ymax, y2);u = u->next;} while(u!=branches2);
#endif
	if(j) do {
	    kdarea_list_t*branches3 = kdarea_filter(j->area, x1, KD_RIGHT);
	    kdarea_list_t*k = branches3;
#ifdef DEBUG
            kdarea_list_t*u = branches3;
            if(u) do {printf("%p [%d %d %d %d] is to the right of %d\n", u->area, u->area->bbox.xmin, u->area->bbox.ymin, u->area->bbox.xmax, u->area->bbox.ymax, x1);u = u->next;} while(u!=branches3);
#endif
	    if(k) do {
		kdarea_list_t*branches4 = kdarea_filter(k->area, y1, KD_DOWN);
		kdarea_list_t*l = branches4;
#ifdef DEBUG
                kdarea_list_t*u = branches4;
                if(u) do {printf("%p [%d %d %d %d] is below %d\n", u->area, u->area->bbox.xmin, u->area->bbox.ymin, u->area->bbox.xmax, u->area->bbox.ymax, y1);u = u->next;} while(u!=branches4);
#endif
		if(leafs) {
		    if(l) do {
			result = kdarea_list_concatenate(result, kdarea_all_children(l->area, x1, y1, x2, y2, 0));
			l = l->next;
		    } while(l!=branches4);
		    kdarea_list_destroy(branches4);
		} else {
		    result = kdarea_list_concatenate(result, l);
		}
		k = k->next;
	    } while(k!=branches3);
	    kdarea_list_destroy(branches3);
	    j = j->next;
	} while(j!=branches2);
	kdarea_list_destroy(branches2);
	i = i->next;
    } while(i!=branches1);
    kdarea_list_destroy(branches1);
    return result;
}

static void kdtree_modify_box(kdtree_t*tree, int32_t x1, int32_t y1, int32_t x2, int32_t y2, void*(*f)(void*user,void*data), void*user)
{
    kdarea_split(tree->root, x2, KD_LEFT,  x2,y1, x2,y2);
    kdarea_split(tree->root, y2, KD_UP,    x1,y2, x2,y2);
    kdarea_split(tree->root, x1, KD_RIGHT, x1,y1, x1,y2);
    kdarea_split(tree->root, y1, KD_DOWN,  x1,y1, x2,y1);
#ifdef DEBUG
    printf("inserting (%d,%d,%d,%d) %p\n", x1, y1, x2, y2, user);
#endif
    kdarea_list_t*l = kdtree_filter(tree, x1, y1, x2, y2, 1);
    kdarea_list_t*i = l;
    if(l) do {
#ifdef DEBUG
        printf("%p [%d,%d,%d,%d], is contained in [%d %d %d %d]\n", i->area,
                i->area->bbox.xmin,
                i->area->bbox.ymin,
                i->area->bbox.xmax,
                i->area->bbox.ymax,
                x1, y1, x2, y2);
#endif
	i->area->data = f(user, i->area->data);
	i = i->next;
    } while(i!=l);
    kdarea_list_destroy(l);
}

static void* overwrite(void*user, void*data)
{
    return user;
}

void kdtree_add_box(kdtree_t*tree, int32_t x1, int32_t y1, int32_t x2, int32_t y2, void*data)
{
    kdtree_modify_box(tree, x1, y1, x2, y2, overwrite, data);
}

static void* add_to_dict(void*user, void*data)
{
    dict_t*items = (dict_t*)user;
    if(!dict_contains(items, data)) {
        dict_put(items, data, data);
    }
    return data;
}

kdresult_list_t*kdtree_find_in_box(kdtree_t*tree, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    dict_t*items = dict_new2(&ptr_type);
    kdtree_modify_box(tree, x1, y1, x2, y2, add_to_dict, items);
    kdresult_list_t*list = 0;
    DICT_ITERATE_KEY(items, void*, d) {
        if(d) {
            NEW(kdresult_list_t,r);
            r->data = d;
            r->next = list;
            list = r;
        }
    };
    dict_destroy(items);
    return list;
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
    do_indent(indent);printf("[%p] branch (%s, %d)\n", branch, vname[branch->type], branch->xy);
    kdbbox_t b = bbox_for_halfplane(branch->xy, branch->type);
    kdarea_print(branch->side[0], indent+4);
    kdarea_print(branch->side[1], indent+4);
}

void kdarea_print(kdarea_t*area, int indent)
{
    int i;
    assert(area);
    do_indent(indent);printf("[%p] area (%d,%d,%d,%d) %p (l:%p r:%p u:%p d:%p)\n", area,
	    area->bbox.xmin,
	    area->bbox.ymin,
	    area->bbox.xmax,
	    area->bbox.ymax,
	    area->data,
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


void kdbranch_destroy(kdbranch_t*b)
{
    if(b->side[0]) {
	kdarea_destroy(b->side[0]);
	b->side[0] = 0;
    }
    if(b->side[1]) {
	kdarea_destroy(b->side[1]);
	b->side[1] = 0;
    }
    free(b);
}

void kdarea_destroy(kdarea_t*area)
{
    if(area->split) {
	kdbranch_destroy(area->split);
    }
    free(area);
}

void kdtree_destroy(kdtree_t*tree)
{
    kdarea_destroy(tree->root);
    tree->root = 0;
    free(tree);
}

#ifdef MAIN
int main()
{
    assert((1^vx[2]) < 0);

    kdtree_t*tree = kdtree_new();
    kdtree_add_box(tree, 10,30,20,40, "hello world");
    kdtree_add_box(tree, 12,50,15,60, "hello world");
    //kdtree_print(tree);
    kdarea_t*a = kdtree_find(tree, 15,35);
    kdarea_t*left = kdarea_neighbor(a, KD_LEFT, /*y*/35);
    kdarea_t*right = kdarea_neighbor(a, KD_RIGHT, /*y*/35);
    kdarea_t*up = kdarea_neighbor(a, KD_UP, /*x*/15);
    kdarea_t*down = kdarea_neighbor(a, KD_DOWN, /*x*/15);

    a = kdtree_find(tree, 15,25);
    assert(!a || !a->data);
    a = kdtree_find(tree, 15,45);
    assert(!a || !a->data);
    a = kdtree_find(tree, 5,35);
    assert(!a || !a->data);
    a = kdtree_find(tree, 45,35);
    assert(!a || !a->data);
    
    kdtree_destroy(tree);
}
#endif
