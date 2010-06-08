#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include "../types.h"
#include "../mem.h"

typedef struct _ibbox {
    int xmin,ymin,xmax,ymax;
    struct _ibbox*next;
} ibbox_t;

ibbox_t* ibbox_new(int x1, int y1, int x2, int y2)
{
    ibbox_t*b = (ibbox_t*)rfx_calloc(sizeof(ibbox_t)); 
    b->xmin = x1;
    b->ymin = y1;
    b->xmax = x2;
    b->ymax = y2;
    return b;
}

void ibbox_destroy(ibbox_t*b)
{
    while(b) {
	ibbox_t*next = b->next;
	free(b);
	b = next;
    }
}

ibbox_t*get_bitmap_bboxes_simple(unsigned char*alpha, int width, int height, int rowsize)
{
    int ymin = -1;
    int ymax = -1;
    int xmin = width;
    int xmax = 0;

    int x,y;
    for(y=0;y<height;y++) {
	unsigned char*a = &alpha[y*rowsize];
	for(x=0;x<width;x++) {
	    if(a[x]) break;
	}
	int left = x; //first occupied pixel from left
	int right = x+1; //last non-occupied pixel from right
	for(;x<width;x++) {
	    if(a[x]) right=x+1;
	}

	if(left!=width) {
	    if(ymin<0) 
		ymin=y;
	    ymax=y+1;
	    if(left<xmin) xmin = left;
	    if(right>xmax) xmax = right;
	}
    }
    ibbox_t* bbox = 0;
    if(xmin<xmax || ymin<ymax) {
	bbox = ibbox_new(xmin, ymin, xmax, ymax);
    }
    return bbox;
}

typedef struct _head {
    ptroff_t magic;
    ibbox_t bbox;
    int nr;
    int pos;
    int rank;
    int x,y;
    char seen;
    struct _head*next;
    struct _head*prev;
} head_t;

typedef struct _context {
    void**group;
    unsigned char*alpha;
    int rowsize;
    int width;
    int height;
    head_t*heads;
    int count;
} context_t;

#define HEAD_MAGIC ((ptroff_t)-1)

static head_t*head_new(context_t*context, int x, int y)
{
    int pos = context->width*y+x;
    head_t*h = rfx_calloc(sizeof(head_t));
    h->magic = HEAD_MAGIC;
    h->nr = context->count++;
    h->pos = pos;
    h->x = x;
    h->y = y;
    h->bbox.xmin = h->bbox.xmax = x;
    h->bbox.ymin = h->bbox.ymax = y;
    h->next = context->heads;
    context->heads = h;
    if(h->next) {
	h->next->prev = h;
    }
    return h;
}

static void head_delete(context_t*context, head_t*h)
{
    if(h->prev) {
	h->prev->next = h->next;
    }
    if(h->next) {
	h->next->prev = h->prev;
    }
    if(h==context->heads) {
	assert(!h->prev);
	context->heads = h->next;
    }
    free(h);
}

#define POINTS_TO_HEAD(ptr) (((head_t*)(ptr))->magic==HEAD_MAGIC)

static inline void link_to(context_t*context, int from, int to)
{
    // path compression
    void**data = context->group;
    int head = to;
    assert(data[head]);
    while(!POINTS_TO_HEAD(data[head])) {
	assert(data[head]!=(void*)&data[head]); // check that we're not in an infinite loop
	head=(void**)data[head]-(void**)data;
    }
    head_t*h = (head_t*)data[head];
    int x = from%context->width;
    int y = from/context->width;
    if(x < h->bbox.xmin) h->bbox.xmin = x;
    if(y < h->bbox.ymin) h->bbox.ymin = y;
    if(x > h->bbox.xmax) h->bbox.xmax = x;
    if(y > h->bbox.ymax) h->bbox.ymax = y;
    
    data[from] = (void*)&data[head];
}
static char ibbox_does_overlap(ibbox_t*b1, ibbox_t*b2)
{
    if(b1->xmax < b2->xmin) return 0;
    if(b2->xmax < b1->xmin) return 0;
    if(b1->ymax < b2->ymin) return 0;
    if(b2->ymax < b1->ymin) return 0;
    return 1;
}
static void ibbox_expand(ibbox_t*src, ibbox_t*add) 
{
    if(add->xmin < src->xmin)
	src->xmin = add->xmin;
    if(add->ymin < src->ymin)
	src->ymin = add->ymin;
    if(add->xmax > src->xmax)
	src->xmax = add->xmax;
    if(add->ymax > src->ymax)
	src->ymax = add->ymax;
}
static inline void merge(context_t*context, int set1, int set2)
{
    void**data = context->group;
    assert(data[set1]);
    assert(data[set2]);
    int head1 = set1;
    int head2 = set2;
    while(!POINTS_TO_HEAD(data[head1])) {
	head1=(void**)data[head1]-(void**)data;
    }
    while(!POINTS_TO_HEAD(data[head2])) {
	head2=(void**)data[head2]-(void**)data;
    }
    head_t*h1 = (head_t*)data[head1];
    head_t*h2 = (head_t*)data[head2];
    if(h1==h2)
	return;

    if(h1->rank>h2->rank) {
	h1->rank++;
	ibbox_expand(&h1->bbox,&h2->bbox);
	data[head2] = (void*)&data[head1];
	head_delete(context, h2);
    } else {
	h2->rank++;
	ibbox_expand(&h2->bbox,&h1->bbox);
	data[head1] = (void*)&data[head2];
	head_delete(context, h1);
    }
}

ibbox_t ibbox_clip(ibbox_t* outer, ibbox_t* inner)
{
    ibbox_t i = {inner->xmin, inner->ymin, inner->xmax, inner->ymax, 0};
    if(i.xmax > outer->xmax) i.xmax = outer->xmax;
    if(i.ymax > outer->ymax) i.ymax = outer->ymax;
    if(i.xmax < outer->xmin) i.xmax = outer->xmin;
    if(i.ymax < outer->ymin) i.ymax = outer->ymin;
    
    if(i.xmin > outer->xmax) i.xmin = outer->xmax;
    if(i.ymin > outer->ymax) i.ymin = outer->ymax;
    if(i.xmin < outer->xmin) i.xmin = outer->xmin;
    if(i.ymin < outer->ymin) i.ymin = outer->ymin;
    return i;
}

static void** annotate(context_t*context)
{
    unsigned char*alpha = context->alpha;
    int width = context->width;
    int height = context->height;
    void** group = rfx_calloc(width*height*sizeof(void*));
    context->group = group;
    int x,y;

    for(x=1;x<width;x++) {
	if(alpha[x]) {
	    if(group[x-1])
		link_to(context,x,x-1);
	    else
		group[x]=head_new(context,x,0);
	}
    }
    int pos = 0;
    int apos = 0;
    for(y=1;y<height;y++) {
	pos += width;
	apos += context->rowsize;
	if(alpha[apos]) {
	    if(group[pos-width])
		link_to(context,pos,pos-width);
	    else
		group[pos]=head_new(context,0,y);
	}
	for(x=1;x<width;x++) {
	    /* once this code is stable we should copy&paste it
	       out of the loop, change the loop end to width-1 and
	       add the pos-width+1 case */
	    if(alpha[apos+x]) {
		if(group[pos+x-width]) {
		    link_to(context,pos+x,pos+x-width);
		    if(group[pos+x-1])
			merge(context,pos+x,pos+x-1);
		} else if(group[pos+x-1]) {
		    link_to(context,pos+x,pos+x-1);
		} else if(group[pos+x-width-1]) {
		    link_to(context,pos+x,pos+x-width-1);
		} else {
		    group[pos+x]=head_new(context,x,y);
		}
	    }
	}
    }
    return group;
}

static void overlap_bboxes(context_t*context)
{
    char changed;
    do {
	head_t*h1 = context->heads;
	changed = 0;
	while(h1) {
	    head_t*next = h1->next;
	    head_t*h2 = context->heads;
	    while(h2) {
		if(h1!=h2) {
		    if(ibbox_does_overlap(&h1->bbox, &h2->bbox)) {
			merge(context, h1->pos, h2->pos);
			changed = 1;
			break;
		    }
		}
		h2 = h2->next;
	    }
	    h1 = next;
	}
    } while(changed);
}

typedef struct _circle_coord {
    S16 x,y;
} circle_coord_t;

static int compare_circle_coord(const void *_v1, const void *_v2)
{
    circle_coord_t*v1=(circle_coord_t*)_v1;
    circle_coord_t*v2=(circle_coord_t*)_v2;
    return (v1->x*v1->x + v1->y*v1->y) - (v2->x*v2->x + v2->y*v2->y);
}

static head_t* search_vicinity(context_t*context, head_t*h, int max_radius, double*cos, double*sin)
{
    static circle_coord_t*circle_order = 0;
    static int circle_order_size = 0;
    
    if(!circle_order) {
	circle_order_size = (max_radius*(max_radius+1))/2;
	circle_order = malloc(sizeof(circle_coord_t)*circle_order_size);
	int x,y;
	int i = 0;
	for(y=0;y<max_radius;y++) {
	    for(x=0;x<=y;x++) {
		circle_order[i].x=x;
		circle_order[i].y=y;
		i++;
	    }
	}
	assert(i==circle_order_size);
	qsort(circle_order, circle_order_size, sizeof(circle_coord_t), compare_circle_coord);
    }

    int t;
    void**data = context->group;
    int signx[4] = {-1,1,-1,1};
    int signy[4] = {-1,-1,1,1};
    for(t=1;t<circle_order_size;t++) {
	int xx = circle_order[t].x;
	int yy = circle_order[t].y;
	int s;
	for(s=0;s<4;s++) {
	    int x=h->x+xx*signx[s];
	    int y=h->y+yy*signy[s];
	    if(x>=0 && y>=0 && x<context->width && y<context->height) {
		int pos = y*context->width+x;
		if(data[pos]) {
		    while(!POINTS_TO_HEAD(data[pos])) {
			pos=(void**)data[pos]-(void**)data;
		    }
		    head_t*new_head = (head_t*)data[pos];
		    if(new_head != h) {
			return new_head;
		    }
		}
	    }
	}
    }
    return 0;
}

static void fix_small_boxes(context_t*context)
{
    double sintab[256];
    double costab[256];
    int t;
    for(t=0;t<256;t++) {
	sintab[t] = sin(t*M_PI/128);
	costab[t] = cos(t*M_PI/128);
    }
	
    head_t*h = context->heads;
    while(h) {
	h->seen = 0;
	h = h->next;
    }

    char changed;
    do {
	changed = 0;
	head_t*h = context->heads;
	while(h) {
	    head_t*next = h->next;
	    if(!h->seen) {
		if(h->bbox.xmax - h->bbox.xmin < 32
		|| h->bbox.ymax - h->bbox.ymin < 32) {
		    head_t*other = search_vicinity(context, h, 64, costab, sintab);
		    if(other) {
			merge(context, h->pos, other->pos);
			changed = 1;
			break;
		    } else {
			//printf("nothing in the vicinity of %d,%d,%d,%d\n", h->bbox);
			h->seen = 1;
		    }
		} /*else {
		    printf("area %d,%d,%d,%d is large enough (%dx%d)\n", 
			    h->bbox.xmin,
			    h->bbox.ymin,
			    h->bbox.xmax,
			    h->bbox.ymax,
			    h->bbox.xmax - h->bbox.xmin,
			    h->bbox.ymax - h->bbox.ymin);
		} */
	    }
	    h = next;
	}
    } while(changed);
}

static void display(context_t*context)
{
    int width = context->width;
    int height = context->height;
    void**group = context->group;

    int x,y;
    for(y=0;y<height;y++) {
	for(x=0;x<width;x++) {
	    if(!group[y*width+x]) {
		printf(" -- ");
	    } else if(POINTS_TO_HEAD(group[y*width+x])) {
		printf("g%02d ", ((head_t*)group[y*width+x])->nr);
	    } else {
		printf("x%02d ", (void**)group[y*width+x]-(void**)group);
	    }
	}
	printf("\n");
    }

    head_t*h = context->heads;
    while(h) {
	printf("head: %d\n", h->nr);
	printf(" pos: %d/%d\n", h->pos%width, h->pos/width);
	printf(" bbox: [%d/%d,%d/%d]\n", h->bbox.xmin, h->bbox.ymin, h->bbox.xmax, h->bbox.ymax);
	h = h->next;
    }
}

ibbox_t*get_bitmap_bboxes(unsigned char*alpha, int width, int height, int rowsize)
{
    int size = width*height;
    if(width<=1 || height<=1)
	return get_bitmap_bboxes_simple(alpha, width, height, rowsize);
    
    context_t context;
    context.alpha = alpha;
    context.rowsize = rowsize;
    context.width = width;
    context.height = height;
    context.heads = 0;
    context.count = 1;

    void**group = annotate(&context);
    fix_small_boxes(&context);
    overlap_bboxes(&context);
#ifdef MAIN
    display(&context);
#endif

    ibbox_t*bboxes = 0;

    head_t*h = context.heads;
    while(h) {
	head_t*next = h->next;
	ibbox_t*bbox = malloc(sizeof(ibbox_t));
	memcpy(bbox, &h->bbox, sizeof(ibbox_t));

	/* ibbox_t defines the open upper bound */
	bbox->xmax++;
	bbox->ymax++;

	bbox->next = bboxes;
	bboxes = bbox;
	free(h);
	h = next;
    }
    free(context.group);
    return bboxes;
}

#ifdef MAIN
int main(int argn, char*argv[])
{
    unsigned char alpha[8*8]=
	"\0\0\1\0\0\0\0\0"
	"\1\0\0\1\0\1\0\0"
	"\0\0\0\0\0\0\1\0"
	"\0\0\1\0\1\0\0\0"
	"\1\0\1\0\1\0\0\0"
	"\1\0\1\1\1\0\0\1"
	"\1\0\0\0\0\0\1\0"
	"\1\1\1\0\0\0\0\0";

    get_bitmap_bboxes(alpha, 8,8, 8);
}
#endif
