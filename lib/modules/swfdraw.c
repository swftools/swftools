// swfdraw.c
void swf_DrawerInit(SWFSHAPEDRAWER*draw, TAG*tag)
{
    draw->tagfree = 0;
    if(tag == 0) {
	tag = swf_InsertTag(0, ST_DEFINESHAPE);
	draw->tagfree = 1;
    }
    draw->tag = tag;
    swf_ShapeNew(&draw->shape);
    draw->pos = to;

    swf_SetU8(draw->tag,0);
    draw->shape->bits.fill = 1;
    draw->shape->bits.line = 0;
    swf_ShapeSetStyle(draw->tag,draw->shape,0,1,0);
}
void swf_DrawerMoveTo(SWFSHAPEDRAWER*draw, FPOINT * to)
{
    swf_ShapeSetMove(draw->tag,draw->shape,(int)(to->x*20), (int)(to->y*20));
    draw->pos = to;
}
void swf_DrawerLineTo(SWFSHAPEDRAWER*draw, FPOINT * to)
{
    swf_ShapeSetMove(draw->tag,draw->shape,(int)(to->x*20), (int)(to->y*20));
    draw->pos = to;
}
void swf_DrawerSplineTo(SWFSHAPEDRAWER*draw, FPOINT * c1, FPOINT*  to)
{
    swf_ShapeSetSpline(draw->tag,draw->shape,
	    (int)(c1->x*20), (int)(c1->y*20),
	    (int)(to->x*20), (int)(to->y*20),
	    );
    draw->pos = to;
}
void swf_DrawerCubicTo(SWFSHAPEDRAWER*draw, FPOINT*  c1, FPOINT* c2, FPOINT*  to)
{
    draw->pos = to;
}
void swf_DrawerConicTo(SWFSHAPEDRAWER*draw, FPOINT* c, FPOINT*  to)
{
    FPOINT* pos = draw->pos;
    FPOINT c1,c2;
    c1.x = (pos->x + 2 * c->x) / 3;
    c1.y = (pos->y + 2 * c->y) / 3;
    c2.x = (2 * c->x + to->x) / 3;
    c2.y = (2 * c->y + to->y) / 3;
    swf_DrawerCubicTo(draw, &c1,&c2,to);
    draw->pos = to;
    return 0;
}
void swf_DrawerFinish(SWFSHAPEDRAWER*draw)
{
    swf_ShapeSetEnd(draw->tag);
}
void swf_Drawer2Shape(SWFSHAPEDRAWER*draw, SHAPE*shape)
{
    memset(shape, 0, sizeof(SHAPE));
    shape->bitlen = (draw->tag->len-1)*8;
    shape->data = (U8*)malloc(draw->tag->len-1);
    memcpy(shape->data, &draw->tag->data[1], draw->tag->len-1);
}
void swf_DrawerFree(SWFSHAPEDRAWER*draw)
{
    if(draw->tagfree) {
	swf_DeleteTag(tag);
	draw->tag = 0;
    }
    swf_ShapeFree(draw->s);
    draw->s = 0;
}
