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

#ifdef SPLAY

#define LINK(node,side,child) (node)->side = (child);if(child) {(child)->parent = (node);};printf("%08x->%s now %08x\n", node, __STRING(side), child);

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
    segment_t*l = s->rightchild;
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
static void move_to_root(actlist_t*a, segment_t*s)
{
    /* this is a textbook implementation of the three splay operations
       zig, zig-zig and zig-zag */
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
	    if(p->leftchild == s && p->parent->leftchild == p->parent) {
		// zig-zig (left)
		rotate_right(a, pp);
		rotate_right(a, pp);
	    } else if(p->rightchild == s && p->parent->rightchild == p->parent) {
		// zig-zig (right)
		rotate_left(a, pp);
		rotate_left(a, pp);
	    } else if(p->leftchild == s && p->parent->rightchild == p->parent) {
		// zig-zag (left)
		rotate_right(a, p);
		rotate_left(a, pp);
	    } else if(p->rightchild == s && p->parent->leftchild == p->parent) {
		// zig-zag (right)
		rotate_right(a, p);
		rotate_left(a, pp);
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
    
    while(1) {
	if(cmp(a->root,p1,p2)<0) {
	    /* we're to the left of the root */
	    if(!a->root->leftchild)
		return -1;
	    if(cmp(a->root->leftchild,p1,p2)<0) {
		/* we're also to the left of the root's left child
		   -> rotate right, so that the left child is root */
		segment_t*s = a->root->leftchild;
		LINK(a->root, leftchild, s->rightchild);
		LINK(s, rightchild, a->root);
		a->root = s;
		if(!a->root->leftchild)
		    return -1;
	    }
	    LINK(right, leftchild, a->root);
	    right = a->root;
	    a->root = a->root->leftchild;
	} else /* cmp(a->root,p1,p2)>=0 */ {
	    /* we're to the right of the root */
	    if(!a->root->rightchild)
		return 1;
	    if(cmp(a->root->rightchild,p1,p2)>=0) {
		/* we're also to the right of the root's right child
		   -> rotate left, so that the right child is root */
		segment_t*s = a->root->rightchild;
		LINK(a->root, rightchild, s->leftchild);
		LINK(s, leftchild, a->root);
		a->root = s;
		if(!a->root->rightchild)
		    return 1;
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
}

#endif

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

#ifdef SPLAY
    // we insert nodes not trees 
    assert(!s->leftchild);
    assert(!s->rightchild);

    if(a->root) {
	//if(actlist_splay(a, s->a, s->b) < 0) {
	if(cmp(a->root, s->a, s->b) < 0) {
	    printf("new node %08x, link root (%08x) to the right\n", s, a->root);
	    LINK(s,rightchild,a->root);
	    // steal left child from (previous) root
	    LINK(s,leftchild,a->root->leftchild);
	    a->root->leftchild = 0;
	} else {
	    printf("new node %08x, link root (%08x) to the left\n", s, a->root);
	    LINK(s,leftchild,a->root);
	    // steal right child from (previous) root
	    LINK(s,rightchild,a->root->rightchild);
	    a->root->rightchild = 0;
	}
    }
    a->root = s;
    a->root->parent = 0;
#endif

    a->size++;
}

void actlist_insert(actlist_t*a, point_t p1, point_t p2, segment_t*s)
{
    segment_t*left = actlist_find(a, p1, p2);
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
    a->size--;
#ifdef SPLAY
    printf("delete %08x\n", s);
    move_to_root(a, s);
    assert(a->root == s);
    // delete root node
    if(!a->root->leftchild) {
	printf("shift %08x->rightchild (%08x) to root\n", a->root, a->root->rightchild);
	a->root = a->root->rightchild;
    } else if(!a->root->rightchild) {
	printf("shift %08x->leftchild (%08x) to root\n", a->root, a->root->leftchild);
	a->root = a->root->leftchild;
    } else {
	printf("freeing up %08x->left->right\n", a->root);
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
	LINK(a->root->leftchild,rightchild,a->root->right);
	a->root = a->root->leftchild;
    }
    if(a->root) 
	a->root->parent = 0;
    s->leftchild = s->rightchild = s->parent = 0;
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
    // for splaying this needs to be rewritten (can't use insert_after)
    actlist_delete(a, s1);
    actlist_insert_after(a, s2, s1);
}
