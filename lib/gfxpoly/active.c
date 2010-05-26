#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "../../config.h"
#include "../q.h"
#include "../types.h"
#include "active.h"

actlist_t* actlist_new()
{
    NEW(actlist_t, a);
    return a;
}
void actlist_destroy(actlist_t*a)
{
    free(a);
}

void actlist_dump(actlist_t*a, int32_t y, double gridsize)
{
    segment_t*s = a->list;
    double lastx;
    char bad = 0;
    if(!s) fprintf(stderr, "(empty)\n");
    while(s) {
        if(y) {
            double x = ((double)s->delta.x*(y-s->a.y)/s->delta.y)+s->a.x;
            if(s!=a->list) {
                if(lastx>x) 
                    fprintf(stderr, "?%.2f<->%.2f? ", lastx * gridsize, x * gridsize);
            }
            lastx = x;
        }
        fprintf(stderr, "[%d]", (int)s->nr);
        s = s->right;
        if(s) fprintf(stderr, " ");
        else fprintf(stderr, " y=%.2f\n", y * gridsize);
    }
}
void actlist_verify(actlist_t*a, int32_t y)
{
    segment_t*s = a->list;
    assert(!s || !s->left);
    segment_t*l = 0;
    while(s) {
        if(y) {
            if(l) {
                /* we need to re-evaluate the x of the previous segment. if we
                   try to store it, it might end up being converted to a double,
                   which will make it non-equal to (and possibly larger than) the
                   "long double" the FPU has in its register. This only happens
                   when compiler optimizations are turned on. */
                assert((XPOS(s, y) - XPOS(l, y)) >= 0);
                assert(XDIFF(s,l,y) >= 0);
            }
            l = s;
        }
        assert(!s->left || s->left->right == s);
        assert(!s->right || s->right->left == s);
        s = s->right;
    }
}

static inline double single_cmp(segment_t*s, point_t p1)
{
    return LINE_EQ(p1, s);
}

static inline double cmp(segment_t*s, point_t p1, point_t p2)
{
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
    return d;
}

#ifdef SPLAY
static void actlist_splay_dump(actlist_t*a);
segment_t* actlist_find(actlist_t*a, point_t p1, point_t p2)
{
#ifdef CHECKS
    segment_t*t = a->list;
    char to_the_left = 0;
    char fail = 0;
    while(t) {
	/* this check doesn't work out with cmp() because during horizontal
	   processing, both segments ending as well as segments starting will
	   be active in this scanline */
	//double d = cmp(t, p1, p2);
	double d = single_cmp(t, p1);
	if(d>=0 && to_the_left) {
	    actlist_dump(a, p1.y, 1);
	    segment_t*s = a->list;
	    while(s) {
		fprintf(stderr, "[%d] %f/%f (%d,%d) -> (%d,%d)\n", SEGNR(s),
			single_cmp(s,p1), cmp(s,p1,p2),
			s->a.x, s->a.y, s->b.x, s->b.y);
		s = s->right;
	    }
	}
	assert(!(d>=0 && to_the_left));
	if(d<0) to_the_left=1;
	t = t->right;
    }
#if 0
    if(a->size > 100) {
	static actlist_t*last = 0;
	if(last != a) {
	    last = a;
	    actlist_splay_dump(a);
	}
    }
#endif
#endif
    segment_t*last=0, *s = a->root;
    if(!s) return 0;
    double d=0;
    int depth = 0;
    while(s) {
        last = s;
	depth++;
	d = single_cmp(s, p1);
        if(d<=0) {
	    s = s->leftchild;
	} else {
	    s = s->rightchild;
	}
    }
//#ifdef DEBUG
#if 0
    if(a->size > 1) {
	/* 80% hit, average cost per miss ~ 4 nodes */
	int expected_depth = (int)((double)log2((double)a->size+1))+1;
	static int hits = 0;
	static int misses = 0;
	static int miss_cost = 0;
	if(depth <= expected_depth) hits++;
	else {misses++;
	    miss_cost += depth - expected_depth;
	}
	if(hits && misses)
	    fprintf(stderr, "%02.2f%% %f\n", hits / (double)(hits+misses), miss_cost / (double)misses);
    }
#endif

    /* this can be optimized for (is not needed in) normal mode (as opposed to horizontal postprocess mode) */
    segment_t*out = last;
    if(d<0 || (d==0 && LINE_EQ(p2,last)<0)) {
	last = last->left;
	if(!last) {
	    assert(cmp(a->list, p1, p2)<0);
	    return 0;
	}
    } else {
	while(last->right && cmp(last->right, p1, p2)>=0) {
	    last = last->right;
	}
    }

#ifdef CHECKS
    segment_t*l=0;
    s = a->list;
    while(s) {
        if(cmp(s, p1, p2)<0)
            break;
        l = s;s = s->right;
    }
    if(l!=last) {
	printf("[%d]!=[%d]\n", SEGNR(l), SEGNR(last));
	printf("after tree: [%d]\n", SEGNR(out));
	actlist_splay_dump(a);
	s = a->list;
	while(s) {
	    double d1 = single_cmp(s,p1);
	    double d2 = cmp(s,p1,p2);
	    int x1 = d1<0?-1:(d1>0?1:0);
	    int x2 = d2<0?-1:(d2>0?1:0);
	    printf("[%d](%d,%d) ", SEGNR(s), x1, x2);
	    s = s->right;
	}
	printf("\n");
    }
    assert(l == last);
#endif

    return last;
}
#else
segment_t* actlist_find(actlist_t*a, point_t p1, point_t p2)
{
    segment_t*last=0, *s = a->list;
    if(!s) return last;
    while(s) {
	double d = cmp(s, p1, p2);
        if(d<0)
            break;
        last = s;
        s = s->right;
    }
    return last;
}
#endif

#ifdef SPLAY

#define LINK(node,side,child) (node)->side = (child);if(child) {(child)->parent = (node);}
 //;fprintf(stderr, "[%d]->%s now [%d]\n", SEGNR(node), __STRING(side), SEGNR(child));

// rotates segment's left node to the top
static inline segment_t*rotate_right(actlist_t*a, segment_t*s)
{
    /*      s             n
	   /               \
	  n      ->         s
	   \               /
	    l             l
    */
    assert(s->leftchild);
    segment_t*p = s->parent;
    segment_t*n = s->leftchild;
    segment_t*l = n->rightchild;
    LINK(n,rightchild,s);
    LINK(s,leftchild,l);
    n->parent = p;
    if(p) {
	if(p->leftchild == s)
	    p->leftchild = n;
	else if(p->rightchild == s)
	    p->rightchild = n;
    } else {
	a->root = n;
    }
    return n;
}
// rotates segment's right node to the top
static inline segment_t*rotate_left(actlist_t*a, segment_t*s)
{
    /*      s             n
	     \           /
	      n  ->     s
	     /           \
	    r             r
    */
    assert(s->rightchild);
    segment_t*p = s->parent;
    segment_t*n = s->rightchild;
    segment_t*r = n->leftchild;
    LINK(n,leftchild,s);
    LINK(s,rightchild,r);
    n->parent = p;
    if(p) {
	if(p->leftchild == s)
	    p->leftchild = n;
	else if(p->rightchild == s)
	    p->rightchild = n;
    } else {
	a->root = n;
    }
    return n;
}

static int actlist_splay_walk(actlist_t*a, segment_t*s, segment_t**ss, segment_t*parent)
{
    if(!s) return 1;
    if(parent != s->parent) {
	fprintf(stderr, "Parent mismatch in [%d]: [%d] != [%d]\n", SEGNR(s), SEGNR(parent), SEGNR(s->parent));
	return 0;
    }
    if(!actlist_splay_walk(a, s->leftchild, ss, s)) return 0;
    if(s != *ss) {
	fprintf(stderr, "[%d] != [%d]\n", SEGNR(s), SEGNR(*ss));
	return 0;
    }
    (*ss) = (*ss)->right; 
    if(!actlist_splay_walk(a, s->rightchild, ss, s)) return 0;
    return 1;
}

static int actlist_splay_verify(actlist_t*a)
{
    segment_t*c = a->list;
    if(!actlist_splay_walk(a, a->root, &c, 0)) return 0;
    if(c) return 0;
    return 1;
}
static void actlist_splay_dump2(actlist_t*a, segment_t*s, char*mid, char*up, char*down)
{
    if(!s) return;
    
    if(s->leftchild || s->rightchild) {
        int t;

	if(s->leftchild) {
	    char*o3 = malloc(strlen(up)+3);
	    strcpy(o3, up);strcat(o3, "+-");
	    char*newup = malloc(strlen(up)+3);
	    strcpy(newup, up);strcat(newup, "| ");
	    char*newup2 = malloc(strlen(up)+3);
	    strcpy(newup2, up);strcat(newup2, "  ");
	    actlist_splay_dump2(a, s->leftchild, o3, newup2, newup);
	    fprintf(stderr, "%s| \n", up);
	    free(newup);
	    free(newup2);
	    free(o3);
	}
        fprintf(stderr, "%s[%d]\n", mid, SEGNR(s));
	if(s->rightchild) {
	    char*o3 = malloc(strlen(down)+3);
	    strcpy(o3, down);strcat(o3, "+-");
	    char*newdown = malloc(strlen(down)+3);
	    strcpy(newdown, down);strcat(newdown, "| ");
	    char*newdown2 = malloc(strlen(down)+3);
	    strcpy(newdown2, down);strcat(newdown2, "  ");
	    fprintf(stderr, "%s| \n", down);
	    actlist_splay_dump2(a, s->rightchild, o3, newdown, newdown2);
	    free(newdown);
	    free(newdown2);
	    free(o3);
	}
    } else {
        fprintf(stderr, "%s[%d]\n", mid, SEGNR(s));
    }
}
static void actlist_splay_dump(actlist_t*a)
{
    actlist_splay_dump2(a, a->root, "", "", "");
}


static void move_to_root(actlist_t*a, segment_t*s)
{
    if(!s) return;
    /* this is a textbook implementation of the three splay operations
       zig, zig-zig and zig-zag */
    int nr=0;
    while(a->root != s) {
	assert(s->parent);
	segment_t*p = s->parent;
	if(p == a->root) {
	    // zig
	    if(a->root->leftchild == s) {
		rotate_right(a, a->root);
	    } else {
		rotate_left(a, a->root);
	    }
	    assert(a->root == s);
        } else {
	    segment_t*pp = p->parent;
	    if(p->leftchild == s && pp->leftchild == p) {
		// zig-zig (left)
		rotate_right(a, pp);
		rotate_right(a, s->parent);
	    } else if(p->rightchild == s && pp->rightchild == p) {
		// zig-zig (right)
		rotate_left(a, pp);
		rotate_left(a, s->parent);
	    } else if(p->leftchild == s && pp->rightchild == p) {
		// zig-zag (left)
		rotate_right(a, p);
		rotate_left(a, s->parent);
	    } else if(p->rightchild == s && pp->leftchild == p) {
		// zig-zag (right)
		rotate_left(a, p);
		rotate_right(a, s->parent);
	    } else {
		assert(0);
	    }
	}
    }
}

static void actlist_splay(actlist_t*a, point_t p1, point_t p2)
{
    if(!a->list) return;

    segment_t tmp;
    memset(&tmp, 0, sizeof(tmp));
    segment_t*left=&tmp,*right=&tmp;
   
    int c = 0;
    while(1) {
	if(cmp(a->root,p1,p2)<0) {
	    /* we're to the left of the root */
	    if(!a->root->leftchild) {
		c = -1;break;
	    }
	    if(cmp(a->root->leftchild,p1,p2)<0) {
		/* we're also to the left of the root's left child
		   -> rotate right, so that the left child is root */
		segment_t*s = a->root->leftchild;
		LINK(a->root, leftchild, s->rightchild);
		LINK(s, rightchild, a->root);
		a->root = s;
		if(!a->root->leftchild) {
		    c = -1;break;
		}
	    }
	    LINK(right, leftchild, a->root);
	    right = a->root;
	    a->root = a->root->leftchild;
	} else /* cmp(a->root,p1,p2)>=0 */ {
	    /* we're to the right of the root */
	    if(!a->root->rightchild) {
		c = 1;break;
	    }
	    if(cmp(a->root->rightchild,p1,p2)>=0) {
		/* we're also to the right of the root's right child
		   -> rotate left, so that the right child is root */
		segment_t*s = a->root->rightchild;
		LINK(a->root, rightchild, s->leftchild);
		LINK(s, leftchild, a->root);
		a->root = s;
		if(!a->root->rightchild)
		    c = 1;break;
	    }
	    LINK(left, rightchild, a->root);
	    left = a->root;
	    a->root = a->root->rightchild;
	}
    }
    LINK(left, rightchild, a->root->leftchild);
    LINK(right, leftchild, a->root->rightchild);
    LINK(a->root, leftchild, tmp.rightchild);
    LINK(a->root, rightchild, tmp.leftchild);
    a->root->parent = 0;
}

#endif

static void actlist_insert_after(actlist_t*a, segment_t*left, segment_t*s)
{
#ifdef SPLAY
    //fprintf(stderr, "insert [%d] after [%d]\n", SEGNR(s), SEGNR(left));
    //actlist_splay_dump(a);
    //actlist_dump(a, s->a.y);
#endif

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

#ifdef SPLAY
    // we insert nodes not trees 
    assert(!s->leftchild);
    assert(!s->rightchild);

    if(a->root) {
	move_to_root(a, left);
	if(left) {
	    LINK(s,leftchild,a->root);
	    // steal right child from (previous) root
	    LINK(s,rightchild,a->root->rightchild);
	    a->root->rightchild = 0;
	} else {
	    LINK(s,rightchild,a->root);
	}
    }
    a->root = s;
    a->root->parent = 0;
  
    assert(actlist_splay_verify(a));
#endif

    a->size++;
}

void actlist_delete(actlist_t*a, segment_t*s)
{
#ifdef SPLAY
    assert(actlist_splay_verify(a));
    move_to_root(a, s);
    assert(actlist_splay_verify(a));
#endif
    if(s->left) {
        s->left->right = s->right;
    } else {
        a->list = s->right;
    }
    if(s->right) {
        s->right->left = s->left;
    }
    s->left = s->right = 0;
    a->size--;
#ifdef SPLAY
    assert(a->root == s);
    // delete root node
    if(!a->root->leftchild) {
	a->root = a->root->rightchild;
    } else if(!a->root->rightchild) {
	a->root = a->root->leftchild;
    } else {
#ifdef HAVE_LRAND48
	if(lrand48()&1) {
#else
	if(((ptroff_t)s)&16) {
#endif
	    // free up root->left->right
	    segment_t*t = a->root->leftchild;
	    while(t->rightchild) {
		segment_t*r = t->rightchild;
		segment_t*l = r->leftchild;
		LINK(r, leftchild, t);
		LINK(t, rightchild, l);
		t = r;
	    }
	    LINK(a->root,leftchild,t);
	    assert(!a->root->leftchild->rightchild);
	    LINK(a->root->leftchild,rightchild,a->root->rightchild);
	    a->root = a->root->leftchild;
	} else {
	    // free up root->right->left
	    segment_t*t = a->root->rightchild;
	    while(t->leftchild) {
		segment_t*l = t->leftchild;
		segment_t*r = l->rightchild;
		LINK(l, rightchild, t);
		LINK(t, leftchild, r);
		t = l;
	    }
	    LINK(a->root,rightchild,t);
	    assert(!a->root->rightchild->leftchild);
	    LINK(a->root->rightchild,leftchild,a->root->leftchild);
	    a->root = a->root->rightchild;
	}
    }
    if(a->root) 
	a->root->parent = 0;
    s->leftchild = s->rightchild = s->parent = 0;
    
    assert(actlist_splay_verify(a));
#endif
}
int actlist_size(actlist_t*a)
{
    return a->size;
}

segment_t* actlist_leftmost(actlist_t*a)
{
    return a->list;
}

segment_t* actlist_rightmost(actlist_t*a)
{
    /* this is only used in checks, so it doesn't matter that it's slow */
#ifndef CHECKS
    fprintf(stderr, "Warning: actlist_rightmost should not be used\n");
#endif
    segment_t*s = a->list;
    segment_t*last = 0;
    while(s) {
        last = s;
        s = s->right;
    }
    return last;
}

void actlist_insert(actlist_t*a, point_t p1, point_t p2, segment_t*s)
{
    segment_t*left = actlist_find(a, p1, p2);
    actlist_insert_after(a, left, s);
}

void actlist_swap(actlist_t*a, segment_t*s1, segment_t*s2)
{
#ifdef SPLAY
    assert(actlist_splay_verify(a));
#endif
#ifdef CHECKS
    /* test that s1 is to the left of s2- our swap
       code depends on that */
    segment_t*s = s1;
    while(s && s!=s2) s = s->right;
    assert(s==s2);
#endif
/*#ifndef SPLAY
    actlist_delete(a, s1);
    actlist_insert_after(a, s2, s1);
#else*/
    segment_t*s1l = s1->left;
    segment_t*s1r = s1->right;
    segment_t*s2l = s2->left;
    segment_t*s2r = s2->right;
    if(s1l) s1l->right = s2;
    else    a->list = s2;
    s2->left = s1l;
    if(s2r) s2r->left = s1;
    s1->right = s2r;
    if(s2l!=s1) s1->left = s2l; 
    else        s1->left = s2;
    if(s1r!=s2) s2->right = s1r; 
    else        s2->right = s1;
   
#ifdef SPLAY
    if(s2->parent==s1) {
	/* 
	     s1            s2
	    /      ->     /
	  s2            s1
	*/
	segment_t*l = s2->leftchild;
	segment_t*r = s2->rightchild;
	assert(s1->rightchild == s2); // because s1 < s2
	segment_t*l1 = s1->leftchild;
	segment_t*p = s1->parent;
	s1->parent = s2;
	s2->parent = p;
	if(p) {
	    if(p->leftchild == s1) p->leftchild=s2;
	    else {assert(p->rightchild == s1);p->rightchild=s2;}
	} else {
	    a->root = s2;
	}
	s2->leftchild = l1;
	s2->rightchild = s1;
	s1->leftchild = l;
	s1->rightchild = r;
    } else if(s1->parent==s2) {
	/* 
	     s2            s1
	    /      ->     /
	  s1            s2
	*/
	segment_t*l = s1->leftchild;
	segment_t*r = s1->rightchild;
	segment_t*r2 = s2->rightchild;
	assert(s2->leftchild == s1); // because s1 < s2
	segment_t*p = s2->parent;
	s2->parent = s1;
	s1->parent = p;
	if(p) {
	    if(p->leftchild == s2) p->leftchild=s1;
	    else {assert(p->rightchild == s2);p->rightchild=s1;}
	} else {
	    a->root = s1;
	}
	s1->leftchild = s2;
	s1->rightchild = r2;
	s2->leftchild = l;
	s2->rightchild = r;
    } else {
	segment_t*s1p = s1->parent;
	segment_t*s1l = s1->leftchild;
	segment_t*s1r = s1->rightchild;
	segment_t*s2p = s2->parent;
	segment_t*s2l = s2->leftchild;
	segment_t*s2r = s2->rightchild;
	s2->parent = s1p;
	s2->leftchild = s1l;
	s2->rightchild = s1r;
	s1->parent = s2p;
	s1->leftchild = s2l;
	s1->rightchild = s2r;
	assert(s1p || s2p);
	if(s1p) {
	    if(s1p->leftchild == s1) s1p->leftchild=s2;
	    else {assert(s1p->rightchild == s1);s1p->rightchild=s2;}
	} else {
	    a->root = s2;
	}
	if(s2p) {
	    if(s2p->leftchild == s2) s2p->leftchild=s1;
	    else {assert(s2p->rightchild == s2);s2p->rightchild=s1;}
	} else {
	    a->root = s1;
	}
    }
    if(s1->leftchild) s1->leftchild->parent = s1;
    if(s2->leftchild) s2->leftchild->parent = s2;
    if(s1->rightchild) s1->rightchild->parent = s1;
    if(s2->rightchild) s2->rightchild->parent = s2;

    assert(actlist_splay_verify(a));
#endif

//#endif
}
