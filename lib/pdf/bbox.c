#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
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

ibbox_t*get_bitmap_bboxes_old(unsigned char*alpha, int width, int height)
{
    int ymin = -1;
    int ymax = -1;
    int xmin = width;
    int xmax = 0;

    int x,y;
    for(y=0;y<height;y++) {
	unsigned char*a = &alpha[y*width];
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
    struct _head*next;
    struct _head*prev;
} head_t;

typedef struct _context {
    void**group;
    unsigned char*alpha;
    int width;
    int height;
    head_t*heads;
    int count;
} context_t;

#define HEAD_MAGIC ((ptroff_t)-1)

head_t*head_new(context_t*context, int x, int y)
{
    int pos = context->width*y+x;
    head_t*h = rfx_calloc(sizeof(head_t));
    h->magic = HEAD_MAGIC;
    h->nr = context->count++;
    h->pos = pos;
    h->bbox.xmin = h->bbox.xmax = x;
    h->bbox.ymin = h->bbox.ymax = y;
    h->next = context->heads;
    context->heads = h;
    if(h->next) {
	h->next->prev = h;
    }
    return h;
}

void head_delete(context_t*context, head_t*h)
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
}

#define POINTS_TO_HEAD(ptr) (((head_t*)(ptr))->magic==HEAD_MAGIC)

static inline link_to(context_t*context, int from, int to)
{
    // path compression
    void**data = context->group;
    int head = to;
    assert(data[head]);
    while(!POINTS_TO_HEAD(data[head])) {
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
    if(b1->xmax < b1->xmin) return 0;
    if(b2->xmax < b2->xmin) return 0;
    if(b1->ymax < b1->ymin) return 0;
    if(b2->ymax < b2->ymin) return 0;
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
static inline merge(context_t*context, int set1, int set2)
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

void** annotate(context_t*context)
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
    for(y=1;y<height;y++) {
	pos += width;
	if(alpha[pos]) {
	    if(group[pos-width])
		link_to(context,pos,pos-width);
	    else
		group[pos]=head_new(context,0,y);
	}
	for(x=1;x<width;x++) {
	    /* once this code is stable we should copy&paste it
	       out of the loop, change the loop end to width-1 and
	       add the pos-width+1 case */
	    if(alpha[pos+x]) {
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

ibbox_t*get_bitmap_bboxes(unsigned char*alpha, int width, int height)
{
    int size = width*height;
    if(width<=1 || height<=1)
	return get_bitmap_bboxes_old(alpha, width, height);
    
    context_t context;
    context.alpha = alpha;
    context.width = width;
    context.height = height;
    context.heads = 0;
    context.count = 1;

    void**group = annotate(&context);
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

    head_t*h1 = context.heads;
    while(h1) {
	head_t*next = h1->next;
	head_t*h2 = context.heads;
	while(h2) {
	    if(ibbox_does_overlap(&h1->bbox, &h2->bbox)) {
		merge(&context, h1->pos, h2->pos);
		break;
	    }
	    h2 = h2->next;
	}
	h1 = next;
    }

    head_t*h = context.heads;
    while(h) {
	printf("head: %d\n", h->nr);
	printf(" pos: %d/%d\n", h->pos%width, h->pos/width);
	printf(" bbox: [%d/%d,%d/%d]\n", h->bbox.xmin, h->bbox.ymin, h->bbox.xmax, h->bbox.ymax);
	h = h->next;
    }
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

    get_bitmap_bboxes(alpha, 8,8);
}
#endif
