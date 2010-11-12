// swfdraw.c

#include "../rfxswf.h"

typedef struct _SWFSHAPEDRAWER
{
    SHAPE*shape;
    TAG*tag;
    int tagfree;
    SCOORD firstx;
    SCOORD firsty;
    SCOORD lastx;
    SCOORD lasty;
    SRECT bbox;
    char isfinished;
} SWFSHAPEDRAWER;

static void swf_ShapeDrawerSetLineStyle(drawer_t*draw, void*style);
static void swf_ShapeDrawerSetFillStyle(drawer_t*draw, void*style);
static void swf_ShapeDrawerMoveTo(drawer_t*draw, FPOINT * to);
static void swf_ShapeDrawerLineTo(drawer_t*draw, FPOINT * to);
static void swf_ShapeDrawerSplineTo(drawer_t*draw, FPOINT * c1, FPOINT*  to);
static void swf_ShapeDrawerFinish(drawer_t*draw);
static void swf_ShapeDrawerClear(drawer_t*draw);

static void swf_ShapeDrawerInit(drawer_t*draw, TAG*tag, int fillstylebits, int linestylebits)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)rfx_calloc(sizeof(SWFSHAPEDRAWER));
    draw->internal = sdraw;

    draw->setLineStyle = swf_ShapeDrawerSetLineStyle;
    draw->setFillStyle = swf_ShapeDrawerSetFillStyle;
    draw->moveTo = swf_ShapeDrawerMoveTo;
    draw->lineTo = swf_ShapeDrawerLineTo;
    draw->splineTo = swf_ShapeDrawerSplineTo;
    draw->finish = swf_ShapeDrawerFinish;
    draw->dealloc = swf_ShapeDrawerClear;
    
    sdraw->tagfree = 0;
    if(tag == 0) {
	tag = swf_InsertTag(0, ST_DEFINESHAPE);
	sdraw->tagfree = 1;
    }
    sdraw->tag = tag;
    swf_ShapeNew(&sdraw->shape);
    draw->pos.x = 0;
    draw->pos.y = 0;

    swf_SetU8(sdraw->tag,0);
    sdraw->shape->bits.fill = fillstylebits;
    sdraw->shape->bits.line = linestylebits;
    
    sdraw->bbox.xmin = sdraw->bbox.ymin = SCOORD_MAX;
    sdraw->bbox.xmax = sdraw->bbox.ymax = SCOORD_MIN;

    sdraw->isfinished = 0;

    swf_ShapeSetStyle(sdraw->tag,sdraw->shape,linestylebits?1:0,fillstylebits?1:0,0/*?*/);
}

void swf_Shape10DrawerInit(drawer_t*draw, TAG*tag)
{
    swf_ShapeDrawerInit(draw, tag, 0, 1);
}

void swf_Shape01DrawerInit(drawer_t*draw, TAG*tag)
{
    swf_ShapeDrawerInit(draw, tag, 1, 0);
}

void swf_Shape11DrawerInit(drawer_t*draw, TAG*tag)
{
    swf_ShapeDrawerInit(draw, tag, 1, 1);
}

static void swf_ShapeDrawerSetLineStyle(drawer_t*draw, void*style)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
}
static void swf_ShapeDrawerSetFillStyle(drawer_t*draw, void*style)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
}
static void fixEndPoint(drawer_t*draw)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
    if(   sdraw->firstx != sdraw->lastx 
       || sdraw->firsty != sdraw->lasty) {
	/* fix non-closing shapes */
	FPOINT to;
	to.x = sdraw->firstx/20.0;
	to.y = sdraw->firsty/20.0;
	if(sdraw->shape->bits.fill) // do this only if the shape is filled
	    draw->lineTo(draw, &to);
    }
}
static void swf_ShapeDrawerMoveTo(drawer_t*draw, FPOINT * to)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
    int x = floor(to->x*20);
    int y = floor(to->y*20);

    /* Flash will ignore a moveto (0,0) in glyphs. Hence, we map
       all (0,0)s to (0.05,0)s in moveto,lineto and splineto. */

    if(!x&&!y)
	x++;

    /* we need to write moveto always- it
       might be that it signals the end of a polygon, otherwise
       we would end up connecting two polygons which should
       be seperate 
	TODO: check if the last operation was a moveTo- if
	      yes we *can* skip it.
     */

    //if(sdraw->lastx != x || sdraw->lasty != y) {
	fixEndPoint(draw);
	swf_ShapeSetMove(sdraw->tag,sdraw->shape,x,y);
	sdraw->firstx = sdraw->lastx = x;
	sdraw->firsty = sdraw->lasty = y;
	draw->pos = *to;
    //}
}
static void swf_ShapeDrawerLineTo(drawer_t*draw, FPOINT * to)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
    int x = floor(to->x*20);
    int y = floor(to->y*20);
    if(!x&&!y)
	x++;
    if(sdraw->lastx < sdraw->bbox.xmin) sdraw->bbox.xmin = sdraw->lastx;
    if(sdraw->lasty < sdraw->bbox.ymin) sdraw->bbox.ymin = sdraw->lasty;
    if(sdraw->lastx > sdraw->bbox.xmax) sdraw->bbox.xmax = sdraw->lastx;
    if(sdraw->lasty > sdraw->bbox.ymax) sdraw->bbox.ymax = sdraw->lasty;
    if(x < sdraw->bbox.xmin) sdraw->bbox.xmin = x;
    if(y < sdraw->bbox.ymin) sdraw->bbox.ymin = y;
    if(x > sdraw->bbox.xmax) sdraw->bbox.xmax = x;
    if(y > sdraw->bbox.ymax) sdraw->bbox.ymax = y;
    swf_ShapeSetLine(sdraw->tag,sdraw->shape,x-sdraw->lastx,y-sdraw->lasty);
    sdraw->lastx = x;
    sdraw->lasty = y;
    draw->pos = *to;
}
static void swf_ShapeDrawerSplineTo(drawer_t*draw, FPOINT * c1, FPOINT*  to)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
    int tx = floor(c1->x*20);
    int ty = floor(c1->y*20);
    int x = floor(to->x*20);
    int y = floor(to->y*20);
    if(!x&&!y)
	x++;
    if(sdraw->lastx < sdraw->bbox.xmin) sdraw->bbox.xmin = sdraw->lastx;
    if(sdraw->lasty < sdraw->bbox.ymin) sdraw->bbox.ymin = sdraw->lasty;
    if(sdraw->lastx > sdraw->bbox.xmax) sdraw->bbox.xmax = sdraw->lastx;
    if(sdraw->lasty > sdraw->bbox.ymax) sdraw->bbox.ymax = sdraw->lasty;
    if(x < sdraw->bbox.xmin) sdraw->bbox.xmin = x;
    if(y < sdraw->bbox.ymin) sdraw->bbox.ymin = y;
    if(x > sdraw->bbox.xmax) sdraw->bbox.xmax = x;
    if(y > sdraw->bbox.ymax) sdraw->bbox.ymax = y;
    if(tx < sdraw->bbox.xmin) sdraw->bbox.xmin = tx;
    if(ty < sdraw->bbox.ymin) sdraw->bbox.ymin = ty;
    if(tx > sdraw->bbox.xmax) sdraw->bbox.xmax = tx;
    if(ty > sdraw->bbox.ymax) sdraw->bbox.ymax = ty;
    swf_ShapeSetCurve(sdraw->tag,sdraw->shape, tx-sdraw->lastx,ty-sdraw->lasty, x-tx,y-ty);
    sdraw->lastx = x;
    sdraw->lasty = y;
    draw->pos = *to;
}
static void swf_ShapeDrawerFinish(drawer_t*draw)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
    if(sdraw->isfinished)
	return;
	
    fixEndPoint(draw);

    if(sdraw->bbox.xmin == SCOORD_MAX) {
	/* no points at all -> empty bounding box */
	sdraw->bbox.xmin = sdraw->bbox.ymin = 
	sdraw->bbox.xmax = sdraw->bbox.ymax = 0;
    }
    sdraw->isfinished = 1;
    swf_ShapeSetEnd(sdraw->tag);
}

static void swf_ShapeDrawerClear(drawer_t*draw)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
    if(sdraw->tagfree) {
	swf_DeleteTag(0, sdraw->tag);
	sdraw->tag = 0;
    }
    swf_ShapeFree(sdraw->shape);
    sdraw->shape = 0;

    rfx_free(draw->internal);
    draw->internal = 0;
}

SRECT swf_ShapeDrawerGetBBox(drawer_t*draw)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
    return sdraw->bbox;
}

SHAPE* swf_ShapeDrawerToShape(drawer_t*draw)
{
    SWFSHAPEDRAWER*sdraw = (SWFSHAPEDRAWER*)draw->internal;
    SHAPE* shape = (SHAPE*)rfx_alloc(sizeof(SHAPE));
    if(!sdraw->isfinished) {
	fprintf(stderr, "Warning: you should Finish() your drawer before calling DrawerToShape");
	swf_ShapeDrawerFinish(draw);
    }
    memcpy(shape, sdraw->shape, sizeof(SHAPE));
    shape->bitlen = (sdraw->tag->len-1)*8;
    shape->data = (U8*)rfx_alloc(sdraw->tag->len-1);
    memcpy(shape->data, &sdraw->tag->data[1], sdraw->tag->len-1);
    return shape;
}
