#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../gfxtools.h"
#include "../log.h"
#include "../mem.h"
#include "../rfxswf.h"
#include "swf.h"

typedef struct _map16_t
{
    void** ids;
} map16_t;

typedef struct _swf_page_internal
{
    int frame;
} swf_page_internal_t;

typedef struct _swf_doc_internal
{
    map16_t*id2char;
    int clips;
    SWF swf;
    int width,height;
    MATRIX m;
} swf_doc_internal_t;

#define TYPE_SHAPE 1
#define TYPE_BITMAP 2
#define TYPE_SPRITE 3
#define TYPE_FONT 4
#define TYPE_TEXT 5

typedef struct _character
{
    U16 id;
    TAG*tag;
    char type;
    void*data;
} character_t;

typedef struct _placement
{
    SWFPLACEOBJECT po;
    int age;
    int startFrame;
} placement_t;

typedef struct _sprite
{
    int frameCount;
} sprite_t;

static void placement_free(placement_t*p)
{
    swf_PlaceObjectFree(&p->po);
    free(p);
}

//---- object/depth handling ----

map16_t* map16_new()
{
    map16_t*map = rfx_calloc(sizeof(map16_t));
    /* TODO: replace this by a more sophisticated data structure */
    map->ids = (void**)rfx_calloc(sizeof(character_t)*65536);
    return map;
}
character_t*map16_get_id(map16_t*map, int id)
{
    if(id<0 || id>=65536)
	return 0;
    return map->ids[id];
}
void map16_free(map16_t*map)
{
    free(map->ids);
}
void map16_add_id(map16_t*map, int nr, void*id)
{
    if(map->ids[nr])
	fprintf(stderr, "Warning: ID %d defined more than once\n");
    map->ids[nr] = id;
}
void map16_remove_id(map16_t*map, int nr)
{
    map->ids[nr] = 0;
}
void map16_enumerate(map16_t*map, void (*f)(void*self, int id, void*data), void*self)
{
    int t;
    for(t=0;t<65536;t++) {
	if(map->ids[t]) {
	    f(self, t, map->ids[t]);
	}
    }
}

//---- bitmap handling ----

typedef struct _bitmap
{
    int width, height;
    RGBA*data;
} bitmap_t;

bitmap_t* bitmap_new(RGBA*data, int width, int height)
{
    bitmap_t* b = (bitmap_t*)rfx_calloc(sizeof(bitmap_t));
    b->data = data;
    b->width = width;
    b->height = height;
    return b;
}

void bitmap_free(bitmap_t*b)
{
    free(b->data); //!
    b->data = 0;
    free(b);
}

//---- handling ----

static map16_t* extractDefinitions(SWF*swf)
{
    map16_t*map = map16_new();
    TAG*tag = swf->firstTag;
    while(tag)
    {
	int id = 0;
	if(swf_isDefiningTag(tag)) {
	    id = swf_GetDefineID(tag);
	}

	if(tag->id == ST_DEFINESPRITE) {
	    character_t*c = rfx_calloc(sizeof(character_t));
	    sprite_t*s = rfx_calloc(sizeof(sprite_t));
	    swf_SetTagPos(tag, 0);
	    swf_GetU16(tag); //id
	    s->frameCount = swf_GetU16(tag); //frameno
	    c->tag = tag;
	    c->type = TYPE_SPRITE;
	    c->data = s;
	    map16_add_id(map, id, c);
	}
	else if(tag->id == ST_DEFINESHAPE ||
		tag->id == ST_DEFINESHAPE2 ||
		tag->id == ST_DEFINESHAPE3) {
	    character_t*c = rfx_calloc(sizeof(character_t));
	    c->tag = tag;
	    c->type = TYPE_SHAPE;
	    map16_add_id(map, id, c);
	}
	else if(tag->id == ST_DEFINEFONT ||
		tag->id == ST_DEFINEFONT2) {
	    character_t*c = rfx_calloc(sizeof(character_t));
	    SWFFONT*font = 0;
	    swf_FontExtract(swf, id, &font);
	    c->tag = tag;
	    c->type = TYPE_FONT;
	    c->data = font;
	    map16_add_id(map, id, c);
	}
	else if(tag->id == ST_DEFINEBITSJPEG || 
		tag->id == ST_DEFINEBITSJPEG2 || 
		tag->id == ST_DEFINEBITSJPEG3 ||
		tag->id == ST_DEFINEBITSLOSSLESS || 
		tag->id == ST_DEFINEBITSLOSSLESS2) {
	    character_t*c = rfx_calloc(sizeof(character_t));
	    int width, height;
	    void*data = swf_ExtractImage(tag, &width, &height);
	    bitmap_t*b = bitmap_new(data, width, height);
	    c->tag = tag;
	    c->type = TYPE_BITMAP;
	    c->data = b;
	    map16_add_id(map, id, c);
	}

	tag = tag->next;
    }
    return map;
}

void swf_FreeTaglist(TAG*tag)
{ 
    while(tag)
    { 
	TAG * tnew = tag->next;
	if (tag->data) 
	    rfx_free(tag->data);
	rfx_free(tag);
	tag = tnew;
    }
}

static void increaseAge(void*self, int id, void*data)
{
    placement_t*p = (placement_t*)data;
    p->age++;
}

static map16_t* extractFrame(TAG*startTag, int frame_to_extract)
{
    map16_t*depthmap = map16_new();
    TAG*tag = startTag;
    int frame = 1;
    int insprite = 0;
    for(;tag;tag = tag->next) {
	if(tag->id == ST_END) 
	    break;
	if(tag->id == ST_DEFINESPRITE) {
	    while(tag->id != ST_END)
		tag = tag->next;
	    continue;
	}
	if(tag->id == ST_PLACEOBJECT ||
	   tag->id == ST_PLACEOBJECT2) {
            placement_t* p = rfx_calloc(sizeof(placement_t));
	    p->age = 1;
	    p->startFrame = frame;
            swf_GetPlaceObject(tag, &p->po);
	    if(p->po.move) {
		placement_t*old = (placement_t*)map16_get_id(depthmap, p->po.depth);
		p->po.id = old->po.id;
		map16_remove_id(depthmap, p->po.depth);
		placement_free(p);
	    } else {
		map16_add_id(depthmap, p->po.depth, p);
	    }
	}
	if(tag->id == ST_REMOVEOBJECT ||
	   tag->id == ST_REMOVEOBJECT2) {
	    U16 depth = swf_GetDepth(tag);
	    map16_remove_id(depthmap, depth);
	}
	if(tag->id == ST_SHOWFRAME || tag->id == ST_END) {
	    if(frame == frame_to_extract) {
		return depthmap;
	    }
	    if(tag->id == ST_SHOWFRAME) {
		frame++;
		map16_enumerate(depthmap, increaseAge, 0);
	    }
	}
    }
    fprintf(stderr, "gfxsource_swf: frame %d not found\n", frame_to_extract);
    return depthmap;
}

// ---- render handling ----

typedef struct _render
{
    map16_t*id2char;
    gfxdevice_t*device;
    MATRIX m;
    int clips;
} render_t;

static void stopClippings(int from, render_t*r)
{
    int t;
    for(t=from;t<r->clips;t++)
	r->device->endclip(r->device);
    r->clips = from;
}

gfxline_t* swfline_to_gfxline(SHAPELINE*line, int linestyle, int fillstyle0)
{
    gfxdrawer_t d;
    SCOORD x=0,y=0;
    gfxline_t*l;
    gfxdrawer_target_gfxline(&d);
    if(line->type != moveTo) {
	fprintf(stderr, "Warning: Shape doesn't start with a moveTo\n");
    }
    while(line) {
	if(line->fillstyle0 == fillstyle0 || line->fillstyle1 == fillstyle0 || 
	   line->linestyle == linestyle) {
	    if(line->type == lineTo) {
		d.moveTo(&d, x/20.0,y/20.0);
		d.lineTo(&d, line->x/20.0,line->y/20.0);
	    } else if(line->type == splineTo) {
		d.moveTo(&d, x/20.0,y/20.0);
		d.splineTo(&d, line->sx/20.0, line->sy/20.0, line->x/20.0,line->y/20.0);
	    }
	}
	x = line->x;
	y = line->y;
	line = line->next;
    }
    l = d.result(&d);    
    return l;
}

void swf_ShapeApplyMatrix(SHAPE2*shape, MATRIX*m)
{
}

static void convertMatrix(MATRIX*from, gfxmatrix_t*to)
{
    to->m00 = from->sx / 65536.0; to->m10 = from->r1 / 65536.0;
    to->m01 = from->r0 / 65536.0; to->m11 = from->sy / 65536.0;
    to->tx = from->tx/20.0;
    to->ty = from->ty/20.0;
}

static void convertCXForm(CXFORM*from, gfxcxform_t*to)
{
    memset(to, 0, sizeof(gfxcxform_t));
    to->aa = from->a0 / 256.0;
    to->rr = from->r0 / 256.0;
    to->gg = from->g0 / 256.0;
    to->bb = from->b0 / 256.0;
    to->ta = from->a1;
    to->tr = from->r1;
    to->tg = from->g1;
    to->tb = from->b1;
}

static gfxgradient_t* convertGradient(GRADIENT*from)
{
    gfxgradient_t*g = rfx_calloc(from->num * sizeof(gfxgradient_t));
    int t;
    for(t=0;t<from->num;t++) {
	g[t].pos = from->ratios[t] / 255.0;
	g[t].color = *(gfxcolor_t*)&from->rgba[t];
	if(t<from->num-1)
	    g[t].next = &g[t+1];
	else
	    g[t].next = 0;
    }
    return g;
}

gfximage_t* findimage(render_t*r)
{
    return 0;
}

static void renderFilled(render_t*r, gfxline_t*line, FILLSTYLE*f, CXFORM*cx)
{
    if(f->type == FILL_SOLID) {
	gfxcolor_t c = *(gfxcolor_t*)&f->color;
	r->device->fill(r->device, line, &c);
    } else if(f->type == FILL_TILED || f->type == FILL_CLIPPED) {
	gfximage_t* img = findimage(r);
	gfxmatrix_t m;
	gfxcxform_t gfxcx;
	convertCXForm(cx, &gfxcx);
	convertMatrix(&f->m, &m);
	/* TODO: handle clipped */
	r->device->fillbitmap(r->device, line, img, &m, &gfxcx);
    } else if(f->type == FILL_LINEAR || f->type == FILL_RADIAL) {
	gfxmatrix_t m;
	gfxgradient_t* g;
	convertMatrix(&f->m, &m);
	g = convertGradient(&f->gradient);
	r->device->fillgradient(r->device, line, g, f->type == FILL_LINEAR ? gfxgradient_linear : gfxgradient_radial, &m);
	free(g);
    }
}

RGBA swf_ColorTransform(RGBA*color, CXFORM*cx)
{
    RGBA dest;
    dest.r = (cx->r0*color->r + cx->r1*256) >> 8;
    dest.g = (cx->g0*color->g + cx->g1*256) >> 8;
    dest.b = (cx->b0*color->b + cx->b1*256) >> 8;
    dest.a = (cx->a0*color->a + cx->a1*256) >> 8;
    return dest;
}

void renderOutline(render_t*r, gfxline_t*line, LINESTYLE*l, CXFORM*cx)
{
    RGBA c = swf_ColorTransform(&l->color, cx);
    gfxcoord_t width = l->width/20.0;
    r->device->stroke(r->device, line, width, (gfxcolor_t*)&c, gfx_capRound, gfx_joinRound, 0.0);
}

void swf_ApplyMatrixToShape(SHAPE2*shape, MATRIX*m)
{
    SHAPELINE*line = shape->lines;
    while(line) {
	SPOINT p;
	p.x = line->x; p.y = line->y;
	p = swf_TurnPoint(p, m);
	line->x = p.x; line->y = p.y;
	line = line->next;
    }
}

static void renderCharacter(render_t*r, placement_t*p, character_t*c)
{
    if(c->type == TYPE_SHAPE) {
	SHAPE2 shape;
	swf_ParseDefineShape(c->tag, &shape);
	MATRIX m;
	swf_MatrixJoin(&m, &r->m, &p->po.matrix);
	swf_ApplyMatrixToShape(&shape, &m);
	SHAPELINE*line = shape.lines;
	int t;
	for(t=1;t<=shape.numfillstyles;t++) {
	   gfxline_t*line;
	   line = swfline_to_gfxline(shape.lines, -1, t);
	   if(line) {
	       if(!p->po.clipdepth) {
		   renderFilled(r, line, &shape.fillstyles[t-1], &p->po.cxform);
	       } else { 
		   r->device->startclip(r->device, line);
		   r->clips++;
	       }
	   }
	   gfxline_free(line);
	   /*line = swfline_to_gfxline(shape.lines, -1, -1, t);
	   if(line) renderFilled(r, line, &shape.fillstyles[t-1], &p->po.cxform);
	   gfxline_free(line);*/
	}
	for(t=1;t<=shape.numlinestyles;t++) {
	   gfxline_t*line = swfline_to_gfxline(shape.lines, t, -1);
	   if(line) renderOutline(r, line, &shape.linestyles[t-1], &p->po.cxform);
	   gfxline_free(line);
	}
    }
}

// ---- main ----

static void placeObject(void*self, int id, void*data)
{
    render_t*r = (render_t*)self;
    placement_t*p = (placement_t*)data;
    character_t*c = map16_get_id(r->id2char, p->po.id);
    if(!c)  {
	fprintf(stderr, "Error: ID %d unknown\n", p->po.id);
    }
    if(c->type == TYPE_SPRITE) {
	int oldclip = r->clips;
	sprite_t* s = (sprite_t*)c->data;
	map16_t* depths = extractFrame(c->tag, p->age % s->frameCount);
	map16_enumerate(depths, placeObject, r);
	stopClippings(oldclip, r);
	return;
    }
    renderCharacter(r, p, c);
}

void swfpage_destroy(gfxpage_t*swf_page)
{
    swf_page_internal_t*i= (swf_page_internal_t*)swf_page->internal;
    free(swf_page->internal);swf_page->internal = 0;
    free(swf_page);swf_page=0;
}

void swfpage_render(gfxpage_t*page, gfxdevice_t*output)
{
    swf_page_internal_t*i = (swf_page_internal_t*)page->internal;
    swf_doc_internal_t*pi = (swf_doc_internal_t*)page->parent->internal;
    map16_t* depths = extractFrame(pi->swf.firstTag, i->frame);
    render_t r;
    r.id2char = pi->id2char;
    r.clips = 0;
    r.device = output;
    r.m = pi->m;
    map16_enumerate(depths, placeObject, &r);
}

void swfpage_rendersection(gfxpage_t*page, gfxdevice_t*output, gfxcoord_t x, gfxcoord_t y, gfxcoord_t _x1, gfxcoord_t _y1, gfxcoord_t _x2, gfxcoord_t _y2)
{
    swf_doc_internal_t*pi = (swf_doc_internal_t*)page->parent->internal;
    /* FIXME */
    swfpage_render(page,output);
}

void swf_doc_destroy(gfxdocument_t*gfx)
{
    swf_doc_internal_t*i= (swf_doc_internal_t*)gfx->internal;
    swf_FreeTags(&i->swf);
    free(gfx->internal);gfx->internal=0;
    free(gfx);gfx=0;
}

void swf_doc_set_parameter(gfxdocument_t*gfx, const char*name, const char*value)
{
    swf_doc_internal_t*i= (swf_doc_internal_t*)gfx->internal;
}

gfxpage_t* swf_doc_getpage(gfxdocument_t*doc, int page)
{
    swf_doc_internal_t*di= (swf_doc_internal_t*)doc->internal;
    if(page < 1 || page > doc->num_pages)
        return 0;
    
    gfxpage_t* swf_page = (gfxpage_t*)malloc(sizeof(gfxpage_t));
    swf_page_internal_t*pi= (swf_page_internal_t*)malloc(sizeof(swf_page_internal_t));
    memset(pi, 0, sizeof(swf_page_internal_t));

    pi->frame = page;

    swf_page->internal = pi;
    swf_page->destroy = swfpage_destroy;
    swf_page->render = swfpage_render;
    swf_page->rendersection = swfpage_rendersection;
    swf_page->width = di->width;
    swf_page->height = di->height;
    swf_page->parent = doc;
    swf_page->nr = page;
    return swf_page;
}

void swf_set_parameter(gfxsource_t*src, const char*name, const char*value)
{
    msg("<verbose> setting parameter %s to \"%s\"", name, value);
}

gfxdocument_t*swf_open(gfxsource_t*src, const char*filename)
{
    gfxdocument_t*swf_doc = (gfxdocument_t*)malloc(sizeof(gfxdocument_t));
    memset(swf_doc, 0, sizeof(gfxdocument_t));
    swf_doc_internal_t*i= (swf_doc_internal_t*)malloc(sizeof(swf_doc_internal_t));
    memset(i, 0, sizeof(swf_doc_internal_t));

    TAG*tag = 0;
    int f;
    int frame;
    render_t r;
    gfxdevice_t d;
    
    if(!filename) {
        return 0;
    }
    f = open(filename,O_RDONLY|O_BINARY);
    if (f<0) { 
        perror("Couldn't open file: ");
	return 0;
    }
    if FAILED(swf_ReadSWF(f,&i->swf)) { 
        fprintf(stderr, "%s is not a valid SWF file or contains errors.\n",filename);
        close(f);
	return 0;
    }
    swf_UnFoldAll(&i->swf);
    
    i->id2char = extractDefinitions(&i->swf);
    i->width = (i->swf.movieSize.xmax - i->swf.movieSize.xmin) / 20;
    i->height = (i->swf.movieSize.ymax - i->swf.movieSize.ymin) / 20;
    
    swf_GetMatrix(0, &i->m);
    i->m.tx = -i->swf.movieSize.xmin;
    i->m.ty = -i->swf.movieSize.ymin;

    swf_doc->num_pages = i->swf.frameCount;
    swf_doc->internal = i;
    swf_doc->get = 0;
    swf_doc->destroy = swf_doc_destroy;
    swf_doc->set_parameter = swf_doc_set_parameter;
    swf_doc->getpage = swf_doc_getpage;

    return swf_doc;
}

gfxsource_t*gfxsource_swf_create()
{
    gfxsource_t*src = (gfxsource_t*)malloc(sizeof(gfxsource_t));
    memset(src, 0, sizeof(gfxsource_t));
    src->set_parameter = swf_set_parameter;
    src->open = swf_open;
    return src;
}
