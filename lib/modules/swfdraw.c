#define SCOORD_MAX 0x7fffffff
#define SCOORD_MIN -0x80000000

// swfdraw.c
void swf_DrawerInit(SWFSHAPEDRAWER*draw, TAG*tag)
{
    memset(draw, 0, sizeof(SWFSHAPEDRAWER));
    draw->tagfree = 0;
    if(tag == 0) {
	tag = swf_InsertTag(0, ST_DEFINESHAPE);
	draw->tagfree = 1;
    }
    draw->tag = tag;
    swf_ShapeNew(&draw->shape);
    draw->pos.x = 0;
    draw->pos.y = 0;

    swf_SetU8(draw->tag,0);
    draw->shape->bits.fill = 1;
    draw->shape->bits.line = 1;
    
    draw->bbox.xmin = draw->bbox.ymin = SCOORD_MAX;
    draw->bbox.xmax = draw->bbox.ymax = SCOORD_MIN;

    draw->isfinished = 0;

    swf_ShapeSetStyle(draw->tag,draw->shape,1,1,0);
}
void swf_DrawerMoveTo(SWFSHAPEDRAWER*draw, FPOINT * to)
{
    int x = to->x*20;
    int y = to->y*20;
    swf_ShapeSetMove(draw->tag,draw->shape,x,y);
    draw->lastx = x;
    draw->lasty = y;
    draw->pos = *to;
}
void swf_DrawerLineTo(SWFSHAPEDRAWER*draw, FPOINT * to)
{
    int x = to->x*20;
    int y = to->y*20;
    if(draw->lastx < draw->bbox.xmin) draw->bbox.xmin = draw->lastx;
    if(draw->lasty < draw->bbox.ymin) draw->bbox.ymin = draw->lasty;
    if(draw->lastx > draw->bbox.xmax) draw->bbox.xmax = draw->lastx;
    if(draw->lasty > draw->bbox.ymax) draw->bbox.ymax = draw->lasty;
    if(x < draw->bbox.xmin) draw->bbox.xmin = x;
    if(y < draw->bbox.ymin) draw->bbox.ymin = y;
    if(x > draw->bbox.xmax) draw->bbox.xmax = x;
    if(y > draw->bbox.ymax) draw->bbox.ymax = y;
    swf_ShapeSetLine(draw->tag,draw->shape,x-draw->lastx,y-draw->lasty);
    draw->lastx = x;
    draw->lasty = y;
    draw->pos = *to;
}
void swf_DrawerSplineTo(SWFSHAPEDRAWER*draw, FPOINT * c1, FPOINT*  to)
{
    int tx = c1->x*20;
    int ty = c1->y*20;
    int x = to->x*20;
    int y = to->y*20;
    if(draw->lastx < draw->bbox.xmin) draw->bbox.xmin = draw->lastx;
    if(draw->lasty < draw->bbox.ymin) draw->bbox.ymin = draw->lasty;
    if(draw->lastx > draw->bbox.xmax) draw->bbox.xmax = draw->lastx;
    if(draw->lasty > draw->bbox.ymax) draw->bbox.ymax = draw->lasty;
    if(x < draw->bbox.xmin) draw->bbox.xmin = x;
    if(y < draw->bbox.ymin) draw->bbox.ymin = y;
    if(x > draw->bbox.xmax) draw->bbox.xmax = x;
    if(y > draw->bbox.ymax) draw->bbox.ymax = y;
    if(tx < draw->bbox.xmin) draw->bbox.xmin = tx;
    if(ty < draw->bbox.ymin) draw->bbox.ymin = ty;
    if(tx > draw->bbox.xmax) draw->bbox.xmax = tx;
    if(ty > draw->bbox.ymax) draw->bbox.ymax = ty;
    swf_ShapeSetCurve(draw->tag,draw->shape, tx-draw->lastx,ty-draw->lasty, x-tx,y-ty);
    draw->lastx = x;
    draw->lasty = y;
    draw->pos = *to;
}
void swf_DrawerCubicTo(SWFSHAPEDRAWER*draw, FPOINT*  c1, FPOINT* c2, FPOINT*  to)
{
    /*TODO*/
    draw->pos = *to;
}
void swf_DrawerConicTo(SWFSHAPEDRAWER*draw, FPOINT* c, FPOINT*  to)
{
    FPOINT* pos = &draw->pos;
    FPOINT c1,c2;
    c1.x = (pos->x + 2 * c->x) / 3;
    c1.y = (pos->y + 2 * c->y) / 3;
    c2.x = (2 * c->x + to->x) / 3;
    c2.y = (2 * c->y + to->y) / 3;
    swf_DrawerCubicTo(draw, &c1,&c2,to);
    draw->pos = *to;
}
void swf_DrawerFinish(SWFSHAPEDRAWER*draw)
{
    if(draw->bbox.xmin == SCOORD_MAX) {
	/* no points at all -> empty bounding box */
	draw->bbox.xmin = draw->bbox.ymin = 
	draw->bbox.xmax = draw->bbox.ymax = 0;
    }
    draw->isfinished = 1;
    swf_ShapeSetEnd(draw->tag);
}
SHAPE* swf_DrawerToShape(SWFSHAPEDRAWER*draw)
{
    SHAPE* shape = malloc(sizeof(SHAPE));
    if(!draw->isfinished) {
	fprintf(stderr, "Warning: you should Finish() your drawer before calling DrawerToShape");
	swf_DrawerFinish(draw);
    }
    memcpy(shape, draw->shape, sizeof(SHAPE));
    shape->bitlen = (draw->tag->len-1)*8;
    shape->data = (U8*)malloc(draw->tag->len-1);
    memcpy(shape->data, &draw->tag->data[1], draw->tag->len-1);
    return shape;
}
void swf_DrawerFree(SWFSHAPEDRAWER*draw)
{
    if(draw->tagfree) {
	swf_DeleteTag(draw->tag);
	draw->tag = 0;
    }
//    swf_ShapeFree(draw->s);
 //   draw->s = 0;
}

static char* getToken(const char**p)
{
    const char*start;
    char*result;
    while(**p && strchr(" ,\t\n\r", **p)) {
	(*p)++;
    } 
    start = *p;
    while(**p && !strchr(" ,\t\n\r", **p)) {
	(*p)++;
    }
    result = malloc((*p)-start+1);
    memcpy(result,start,(*p)-start+1);
    result[(*p)-start] = 0;
    return result;
}

void swf_DrawString(SWFSHAPEDRAWER*draw, const char*string)
{
    const char*p = string;
    while(*p) {
	char*token = getToken(&p);
	if(!strncmp(token, "moveTo", 6)) {
	    FPOINT to;
	    to.x = atoi(getToken(&p));
	    to.y = atoi(getToken(&p));
	    swf_DrawerMoveTo(draw, &to);
	}
	else if(!strncmp(token, "lineTo", 6)) {
	    FPOINT to;
	    to.x = atoi(getToken(&p));
	    to.y = atoi(getToken(&p));
	    swf_DrawerLineTo(draw, &to);
	}
	else if(!strncmp(token, "curveTo", 7) || !strncmp(token, "splineTo", 8)) {
	    FPOINT mid,to;
	    mid.x = atoi(getToken(&p));
	    mid.y = atoi(getToken(&p));
	    to.x = atoi(getToken(&p));
	    to.y = atoi(getToken(&p));
	    swf_DrawerSplineTo(draw, &mid, &to);
	}
	free(token);
    }
    //swf_DrawerFinish(draw); // must be done by application.
}
