/* kdtree.h
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

#ifndef __kdtree_h__
#define  __kdtree_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
          3
	  A
          |
          |
   2 <---- ----> 0
          |
          |
	  V
  	  1
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

typedef struct _kdresult_list {
    void*data;
    struct _kdresult_list*next;
} kdresult_list_t;

kdtree_t* kdtree_new();
void kdarea_destroy(kdarea_t*area);
void kdbranch_destroy(kdbranch_t*b);
void kdtree_destroy(kdtree_t*tree);
kdarea_t* kdtree_find(kdtree_t*tree, int x, int y);
void kdtree_add_box(kdtree_t*tree, int32_t x1, int32_t y1, int32_t x2, int32_t y2, void*data);
kdresult_list_t*kdtree_find_in_box(kdtree_t*tree, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

void kdtree_print(kdtree_t*tree);

kdarea_t*kdarea_neighbor(kdarea_t*area, int dir, int xy);

#define kdarea_left_neighbor(area, y) (kdarea_neighbor((area), KD_LEFT, (y)))
#define kdarea_right_neighbor(area, y) (kdarea_neighbor((area), KD_RIGHT, (y)))
#define kdarea_top_neighbor(area, x) (kdarea_neighbor((area), KD_TOP, (x)))
#define kdarea_bottom_neighbor(area, x) (kdarea_neighbor((area), KD_BOTTOM, (x)))

#ifdef __cplusplus
}
#endif

#endif
