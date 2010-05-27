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

void xrow_add(xrow_t*xrow, int32_t x);
void xrow_sort(xrow_t*xrow);
int xrow_find(xrow_t*r, int32_t x);
char xrow_contains(xrow_t*xrow, int32_t x);
void xrow_dump(xrow_t*xrow, double gridsize);
void xrow_reset(xrow_t*xrow);
void xrow_destroy(xrow_t*xrow);

#endif
