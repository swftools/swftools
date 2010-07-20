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

#define KD_RIGHT 0
#define KD_DOWN 1
#define KD_LEFT 2
#define KD_UP 3

typedef enum {KD_LEAF, KD_BRANCH} kdtype_t;

typedef struct _kdbbox {
    int32_t xmin, ymin, xmax, ymax;
} kdbbox_t;

typedef struct _kdbranch {
    kdtype_t type;
    struct _kdbranch*side[2];
    int32_t r;
    unsigned char angle;
} kdbranch_t;

typedef struct _kdleaf {
    kdtype_t type;
    kdbranch_t*neighbor[4];
    void*data;
} kdleaf_t;

typedef struct _kdtree {
    kdbranch_t*root;
} kdtree_t;

/* usually a subset of the tree, e.g. caused by
   intersecting the tree with a halfplane */
typedef struct _kdbranch_list {
    struct _kdbranch_list*next;
    struct _kdbranch_list*prev;
    kdbranch_t*branch;
} kdbranch_list_t;


#endif
