#ifndef __kdtree_h__
#define  __kdtree_h__

#include <sys/types.h>

/*
         192
	  A
          |
          |
 128 <---- ----> 0
          |
          |
	  V
  	  64
*/

typedef enum {KD_RIGHT=0, KD_DOWN=1, KD_LEFT=2, KD_UP=3} kdtype_t;

typedef struct _kdbbox {
    int32_t xmin, ymin, xmax, ymax;
} kdbbox_t;

struct _kdbranch;
struct _kdarea;
typedef struct _kdbranch kdbranch_t;
typedef struct _kdarea kdarea_t;

struct _kdbranch {
    kdtype_t type;
    kdarea_t*side[2];
    int32_t xy;
};

struct _kdarea {
    kdarea_t*neighbors[4];
    kdbbox_t bbox;
    kdbranch_t*split;
    void*data;
};

typedef struct _kdtree {
    kdarea_t*root;
} kdtree_t;

/* usually a subset of the tree, e.g. caused by
   intersecting the tree with a halfplane */
typedef struct _kdarea_list {
    struct _kdarea_list*next;
    struct _kdarea_list*prev;
    kdarea_t*area;
} kdarea_list_t;


#endif
