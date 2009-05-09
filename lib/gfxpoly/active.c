#include <memory.h>
#include "../q.h"
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

void actlist_dump(actlist_t*a, int32_t y)
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
                    fprintf(stderr, "?%f<->%f? ", lastx, x);
            }
            lastx = x;
        }
        fprintf(stderr, "[%d]", s->nr);
        s = s->right;
        if(s) fprintf(stderr, " ");
        else fprintf(stderr, "\n");
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
                   "long double" the FPU has in it's register. This only happens
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

segment_t* actlist_find(actlist_t*a, point_t p1, point_t p2)
{
    /* this runs in O(n) right now, and should be optimized using a splay
       tree to run in ammortized O(log(n)) 
       (update: currently only 2.5% of the algorithm's running time is spent here,
        so maybe we don't need to bother)
     */
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

#define SEGNR(s) ((s)?(s)->nr:-1)
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
static void actlist_splay_dump2(actlist_t*a, segment_t*s, int indent)
{
    if(!s) return;
    int t;
    if(s->leftchild || s->rightchild) {
	fprintf(stderr, "(");
	fprintf(stderr, "[%d]", SEGNR(s->leftchild));
	if(s->leftchild) {
	    actlist_splay_dump2(a, s->leftchild, indent+8);
	}
	fprintf(stderr, ",");
	fprintf(stderr, "[%d]", SEGNR(s->rightchild));
	if(s->rightchild) {
	    actlist_splay_dump2(a, s->rightchild, indent+8);
	}
	fprintf(stderr, ")");
    }
}
static void actlist_splay_dump(actlist_t*a)
{
    fprintf(stderr, "[%d]", SEGNR(a->root));
    actlist_splay_dump2(a, a->root, 1);
    fprintf(stderr, "\n");
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

static int actlist_splay(actlist_t*a, point_t p1, point_t p2)
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

	/*if(cmp(a->root, s->a, s->b) < 0) {
	    LINK(s,rightchild,a->root);
	    // steal left child from (previous) root
	    LINK(s,leftchild,a->root->leftchild);
	    a->root->leftchild = 0;
	} else {
	    LINK(s,leftchild,a->root);
	    // steal right child from (previous) root
	    LINK(s,rightchild,a->root->rightchild);
	    a->root->rightchild = 0;
	}*/
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
	if(lrand48()&1) {
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


segment_t* actlist_left(actlist_t*a, segment_t*s)
{
    return s->left;
}

segment_t* actlist_right(actlist_t*a, segment_t*s)
{
    if(s) return s->right;
    else  return a->list;
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
    if(!actlist_splay_verify(a)) {
	actlist_splay_dump(a);
	actlist_dump(a, s1->a.y);
    }
#endif

//#endif
}
