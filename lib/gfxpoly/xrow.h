#ifndef __xrow_h__
#define __xrow_h__

#include <stdint.h>

typedef struct _xrow {
    int32_t*x;
    int num;
    int size;
} xrow_t;

xrow_t* xrow_new();
void xrow_add(xrow_t*xrow, int32_t x);
void xrow_sort(xrow_t*xrow);
void xrow_reset(xrow_t*xrow);
void xrow_destroy(xrow_t*xrow);

#endif
