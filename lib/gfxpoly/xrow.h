#ifndef __xrow_h__
#define __xrow_h__

#include <stdint.h>

#include "poly.h"

typedef struct _xrow {
    int32_t*x;
    int num;
    int size;
    int32_t lastx;
} xrow_t;

xrow_t* xrow_new();

/* so the idea is this:
   Before processing any events for a given y, store the list+tree of the current active
   list (that might be useful anyway for adding xrow points to segments).
   Then, for every starting segment and for every segment that receives a point, add a (x,segment)
   tuple to a list which we later sort.
   These segments will at that point have their *old* fill styles set.
   Then, order that list, and also merge identical x values using left<->right comparisons on
   the *old* active list, so that for every x coordinate we have the rightmost (old) segment. (For
   small active lists, it might be faster to just mark the segments (and store the x in them), and
   then walk the active list from left to right, collecting marked segments)
   This list now gives us the information about what the fill areas look like above the scanline and to
   the right of segments which received a point.
   Now, apply fill style changes and resort the list, this time using the *new* active
   list for merging identical x values. That gives us the information on what fill areas look like
   *below* the scanline and to the right of segments which received a point.
   Every time an "above" fillstyle differs from a "below" fillstyle, we need to draw a horizontal
   line (from right to left, using the below fillstyle).
*/

void xrow_add(xrow_t*xrow, int32_t x);

void xrow_sort(xrow_t*xrow);
void xrow_dump(xrow_t*xrow);
void xrow_reset(xrow_t*xrow);
void xrow_destroy(xrow_t*xrow);

#endif
