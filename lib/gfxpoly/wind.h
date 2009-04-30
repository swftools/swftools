#ifndef __wind_h__
#define __wind_h__

#include "poly.h"

typedef struct _windstate
{
    char is_filled;
} windstate_t;

typedef struct _windrule
{
    windstate_t* (*add)(windstate_t*left, fillstyle_t*edge, segment_dir_t dir, gfxpoly_t*master);
    fillstyle_t* (*diff)(windstate_t*left, windstate_t*right);
} windrule_t;

windrule_t* windrule_new_evenodd();
windrule_t* windrule_new_circular();

#endif
