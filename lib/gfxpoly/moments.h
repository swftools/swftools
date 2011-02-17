#ifndef __moments_h__
#define __moments_h__

#include "poly.h"
#include "active.h"

void moments_update(moments_t*moments, actlist_t*actlist, int32_t y1, int32_t y2);
void moments_normalize(moments_t*moments, double gridsize);
#endif
