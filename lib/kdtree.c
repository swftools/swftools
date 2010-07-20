#include <stdio.h>
#include <stdlib.h>
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

static double vx[4] = {1.0, 0, -1.0, 0};
static double vy[4] = {0, 1.0, 0, -1.0};

kdtree_t* kdtree_new()
{
    NEW(kdtree_t,tree);
    return tree;
}

#define IS_LEAF(l) ((l)->type == KD_LEAF)

static inline int32_t max32(int32_t v1, int32_t v2) {return v1>v2?v1:v2;}
static inline int32_t min32(int32_t v1, int32_t v2) {return v1<v2?v1:v2;}

kdleaf_t*kdleaf_new(void*data)
{
    NEW(kdleaf_t,l);
    l->type = KD_LEAF;
    l->data = data;
    return l;
}

kdbranch_t*kdbranch_new(int32_t r, int angle, kdbranch_t*side1, kdbranch_t*side2)
{
    NEW(kdbranch_t,b);
    b->type = KD_BRANCH;
    b->r = r;
    b->angle = angle;
    b->side[0] = side1;
    b->side[1] = side2;
    return b;
}

kdbranch_t**kdtree_branch(kdbranch_t*tree, int32_t x, int32_t y)
{
    int a = tree->angle >> 6;
    int side = vx[a]*x + vy[a]*y < tree->r;
    return &tree->side[side];
}

kdbranch_list_t* kdleaf_list_new(kdbranch_t**branch)
{
    NEW(kdbranch_list_t,b);
    b->branch = branch;
    b->next = b->prev = b;
    return b;
}

kdbranch_list_t*kdbranch_list_concatenate(kdbranch_list_t*l1, kdbranch_list_t*l2)
{
/*
   l1->prev       l2->prev   |   l1->prev       l2->prev   
    |             |          |    |             |          
    V             V          |    V      +------+          
   l1             l2         |   l1  -------->  l2
    |             |          |           |      |          
    |             |          |    +------+      |          
    V             V          |    V             V          
   l1->next       l2->next   |   l1->next       l2->next   
*/
    l2->prev->next = l1->next;
    l1->next->prev = l2->prev;
    l2->prev = l1;
    l1->next = l2;
    return l1;
}

void add_line(kdbranch_t**branch, int r, int angle,
				  int32_t x1, int32_t y1, 
				  int32_t x2, int32_t y2, 
				  void*data)
{
    if(!*branch || IS_LEAF(*branch)) {
	kdleaf_t*l = kdleaf_new(data);
	*branch = kdbranch_new(r, angle, (kdbranch_t*)l, *branch);
	return kdleaf_list_new(&(*branch)->side[0]);
    } else {
	kdbranch_t**first = kdtree_branch(*branch, x1, y1);
	kdbranch_t**second = kdtree_branch(*branch, x2, y2);
	if(first == second) {
	    /* we don't split parts of the tree that don't contain the line (x1,y1)->(x2,y2).
	       Hence, we only further process nodes on the branch that contained 
	       both (x1,y1) as well as (x2,y2).
	     */
	    add_line(first, r, angle, x1,y1, x2,y2, data);
	} else {
	    add_line(first, r, angle, x1,y1, x2,y2, data);
	    add_line(second, r, angle, x1,y1, x2,y2, data);
	}
    }
}

kdbranch_list_t* filter_tree(kdbranch_t*branch, int r, int angle, void*data)
{
    if(!branch)
	return;

    if(IS_LEAF(branch)) {
	return kdleaf_list_new(branch);
    } else {
	if((branch->angle == angle && branch->r < r)
	|| (branch->angle == (angle^128)) && branch->r > r) {
	    return filter_tree(branch->side[0], r, angle, data);
	} else {
	    kdbranch_list_t*l1 = filter_tree(first, r, angle, x1,y1, x2,y2, data);
	    kdbranch_list_t*l2 = filter_tree(second, r, angle, x1,y1, x2,y2, data);
	    return kdbranch_list_concatenate(l1,l2);
	}
    }
}

void kdtree_add_box(kdtree_t*tree, int32_t x1, int32_t y1, int32_t x2, int32_t y2, void*data)
{
    kdbranch_list_t*branches1 = add_branch(&tree->root, x2, 0, x2,y1, x2,y2, data);
    kdbranch_list_t*i;
    for(i=branches1;i;i=i->next) {
	kdbranch_list_t*branches2 = add_branch(i->branch, y2, 64, x1,y2, x2,y2, data);
	kdbranch_list_t*j;
	for(j=branches2;j;j=j->next) {
	    kdbranch_list_t*branches3 = add_branch(i->branch, x1, 128, x1,y1, x1,y2, data);
	    kdbranch_list_t*k;
	    for(k=branches3;k;k=k->next) {
		kdbranch_list_t*branches4 = add_branch(i->branch, y1, 192, x1,y1, x2,y1, data);
		kdbranch_list_t*l;
		for(l=branches4;l;l=l->next) {
		}
	    }
	}
    }
}

static void do_indent(int l)
{
    int i;
    for(i=0;i<l;i++) printf(" ");
}

void kdbranch_print(kdbranch_t*branch, int indent)
{
    int i;
    if(IS_LEAF(branch)) {
	kdleaf_t*leaf = (kdleaf_t*)branch;
	do_indent(indent);printf("[%08x] leaf (l:%08x r:%08x u:%08x d:%08x)\n", branch,
		leaf->neighbor[KD_LEFT],
		leaf->neighbor[KD_RIGHT],
		leaf->neighbor[KD_UP],
		leaf->neighbor[KD_DOWN]);
    } else {
	do_indent(indent);printf("[%08x] branch\n", branch);
	kdbranch_print(branch->side[0], indent+4);
	kdbranch_print(branch->side[1], indent+4);
    }
}

void kdtree_print(kdtree_t*tree)
{
    kdbranch_print(tree->root, 0);
}

#ifdef MAIN
int main()
{
    kdtree_t*tree = kdtree_new();
    kdtree_add_box(tree, 10,12,14,16, "hello world");
    kdtree_print(tree);
}
#endif
