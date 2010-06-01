#ifndef __wind_h__
#define __wind_h__

/* Every segment has an original direction, which is the direction
   the segment had in the input data.
   as our scanline moves from minimum y to maximum y, "DOWN" means
   the the (original) segment's y2 is larger than its y1 */
typedef enum {DIR_UP, DIR_DOWN, DIR_UNKNOWN} segment_dir_t;

#define DIR_INVERT(d) ((d)^(DIR_UP^DIR_DOWN))

typedef struct _edgestyle {
    void*internal;
} edgestyle_t;

extern edgestyle_t edgestyle_default;

typedef struct _windstate
{
    char is_filled;
    int wind_nr;
} windstate_t;

typedef struct _windcontext
{
    int num_polygons;
} windcontext_t;

typedef struct _windrule
{
    windstate_t (*start)(windcontext_t* num_polygons);
    windstate_t (*add)(windcontext_t*context, windstate_t left, edgestyle_t*edge, segment_dir_t dir, int polygon_nr);
    edgestyle_t* (*diff)(windstate_t*left, windstate_t*right);
} windrule_t;

extern windrule_t windrule_evenodd;
extern windrule_t windrule_circular;
extern windrule_t windrule_intersect;
extern windrule_t windrule_union;

#endif
