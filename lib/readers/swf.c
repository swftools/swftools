#include <stdlib.h>
#include <assert.h>
#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../gfxtools.h"
#include "../log.h"
#include "../mem.h"
#include "../png.h"
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

typedef struct _render
{
    map16_t*id2char;
    gfxdevice_t*device;
    MATRIX m;
    int clips;
    int*clips_waiting;

    placement_t*current_placement;
} render_t;


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
	fprintf(stderr, "Warning: ID %d defined more than once\n", nr);
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
//---- placements ----

placement_t* placement_unit()
{
    placement_t*placement = rfx_calloc(sizeof(placement_t));
    swf_GetPlaceObject(0, &placement->po);
    return placement;
}

placement_t* placement_join(placement_t*p1, placement_t*p2)
{
    placement_t*placement = rfx_alloc(sizeof(placement_t));
    *placement = *p1;
    swf_MatrixJoin(&placement->po.matrix, &p1->po.matrix, &p2->po.matrix);
    return placement;
}

static void placement_free(placement_t*p)
{
    swf_PlaceObjectFree(&p->po);
    free(p);
}

//---- conversion stuff ----

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

gfxline_t* swfline_to_gfxline(SHAPELINE*line, int linestyle, int fillstyle0)
{
    gfxdrawer_t d;
    SCOORD x=0,y=0,xx=0,yy=0;
    gfxline_t*l;
    gfxdrawer_target_gfxline(&d);
    if(line && line->type != moveTo) {
	fprintf(stderr, "Warning: Shape doesn't start with a moveTo\n");
    }
    xx = line?line->x+1:0;
    while(line) {
	if(line->fillstyle0 == fillstyle0 || 
	   line->fillstyle1 == fillstyle0 || 
	   line->linestyle == linestyle) {
	    if(line->type == lineTo) {
		if(xx!=x || yy!=y) d.moveTo(&d, x/20.0,y/20.0);
		d.lineTo(&d, line->x/20.0,line->y/20.0);
		xx = line->x;
		yy = line->y;
	    } else if(line->type == splineTo) {
		if(xx!=x || yy!=y) d.moveTo(&d, x/20.0,y/20.0);
		d.splineTo(&d, line->sx/20.0, line->sy/20.0, line->x/20.0,line->y/20.0);
		xx = line->x;
		yy = line->y;
	    }
	}
	x = line->x;
	y = line->y;
	line = line->next;
    }
    l = d.result(&d);    
    return l;
}


//---- bitmap handling ----

static gfximage_t* gfximage_new(RGBA*data, int width, int height)
{
    gfximage_t* b = (gfximage_t*)rfx_calloc(sizeof(gfximage_t));
    b->data = (gfxcolor_t*)data;
    b->width = width;
    b->height = height;
    return b;
}

static gfximage_t* findimage(render_t*r, U16 id)
{
    character_t*c = (character_t*)map16_get_id(r->id2char, id);
    assert(c && c->type == TYPE_BITMAP);
    gfximage_t*img = (gfximage_t*)c->data;

    /*char filename[80];
    sprintf(filename, "bitmap%d.png", id);
    png_write(filename, (unsigned char*)img->data, img->width, img->height);
    printf("saving bitmap %d to %s\n", id, filename);*/

    return c->data;
}
//---- shape handling ----

static void renderFilled(render_t*r, gfxline_t*line, FILLSTYLE*f, CXFORM*cx, MATRIX*po_m)
{
    if(f->type == FILL_SOLID) {
	gfxcolor_t c = *(gfxcolor_t*)&f->color;
	r->device->fill(r->device, line, &c);
    } else if(f->type == FILL_TILED || f->type == FILL_CLIPPED) {
	gfximage_t* img = findimage(r, f->id_bitmap);
	gfxmatrix_t m;
	gfxcxform_t gfxcx;
	convertCXForm(cx, &gfxcx);
	MATRIX m2;
	swf_MatrixJoin(&m2, po_m, &f->m);
	convertMatrix(&m2, &m);
        m.m00/=20.0; m.m10/=20.0;
        m.m01/=20.0; m.m11/=20.0;
	/* TODO: handle clipped */
	r->device->fillbitmap(r->device, line, img, &m, &gfxcx);
    } else if(f->type == FILL_LINEAR || f->type == FILL_RADIAL) {
	gfxmatrix_t m;
	gfxgradient_t* g;
	MATRIX* m2 = &f->m;
	//swf_MatrixJoin(&m2, po_m, &f->m);

	double z = f->type==FILL_RADIAL?4:4;
	m.m00 = m2->sx/z/20.0; m.m10 = m2->r1/z/20.0;
	m.m01 = m2->r0/z/20.0; m.m11 = m2->sy/z/20.0;
	m.tx = m2->tx/20.0;
	m.ty = m2->ty/20.0;

	g = convertGradient(&f->gradient);
	r->device->fillgradient(r->device, line, g, f->type == FILL_LINEAR ? gfxgradient_linear : gfxgradient_radial, &m);
	free(g);
    }
}

//---- font handling ----

typedef struct
{
    int numchars;
    gfxline_t**glyphs;
} font_t;
        
typedef struct textcallbackblock
{
    render_t*r;
    MATRIX m;
} textcallbackblock_t;

static void textcallback(void*self, int*chars, int*xpos, int nr, int fontid, int fontsize, 
		    int xstart, int ystart, RGBA* color)
{
    textcallbackblock_t * info = (textcallbackblock_t*)self;
    font_t*font = 0;
    int t;
    character_t*cfont = map16_get_id(info->r->id2char, fontid);
    if(!cfont) {
	fprintf(stderr, "Font %d unknown\n", fontid);
        return;
    }
    if(cfont->type != TYPE_FONT) {
	fprintf(stderr, "ID %d is not a font\n", fontid);
	return;
    }
    font = cfont->data;

    for(t=0;t<nr;t++) {
	int x = xstart + xpos[t];
	int y = ystart;
	MATRIX m = info->m;
	SPOINT p;
	
	p.x = x; p.y = y; 
	p = swf_TurnPoint(p, &m);
	
	m.sx = (m.sx * fontsize) / 1024;
	m.sy = (m.sy * fontsize) / 1024;
	m.r0 = (m.r0 * fontsize) / 1024;
	m.r1 = (m.r1 * fontsize) / 1024;
	m.tx = p.x;
	m.ty = p.y;

        gfxmatrix_t gm;
        convertMatrix(&m, &gm);

	if(chars[t]<0 || chars[t]>= font->numchars) {
	    fprintf(stderr, "Character out of range: %d\n", chars[t]);
	} else {
            gfxline_t*line = gfxline_clone(font->glyphs[chars[t]]);
            gfxline_transform(line, &gm);
            FILLSTYLE f;
            f.type = FILL_SOLID;
            f.color = *color;
            renderFilled(info->r, line, &f, 0, 0);
            gfxline_free(line);
	}
    }
}


//---- tag handling ----

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
		tag->id == ST_DEFINEFONT2 ||
		tag->id == ST_DEFINEFONT3) {
	    character_t*c = rfx_calloc(sizeof(character_t));
	    SWFFONT*swffont = 0;
	    font_t*font = (font_t*)rfx_calloc(sizeof(font_t));
	    swf_FontExtract(swf, id, &swffont);
            font->numchars = swffont->numchars;
            font->glyphs = (gfxline_t**)rfx_calloc(sizeof(gfxline_t*)*font->numchars);
            int t;
            RGBA color_white = {255,255,255,255};
            for(t=0;t<font->numchars;t++) {
                if(!swffont->glyph[t].shape->fillstyle.n) {
                    swf_ShapeAddSolidFillStyle(swffont->glyph[t].shape, &color_white);
                }
                SHAPE2*s2 = swf_ShapeToShape2(swffont->glyph[t].shape);
                font->glyphs[t] = swfline_to_gfxline(s2->lines, 0, 1);
		if(tag->id==ST_DEFINEFONT3) {
		    gfxmatrix_t m = {1/20.0,0,0, 0,1/20.0,0};
		    gfxline_transform(font->glyphs[t], &m);
		}
                swf_Shape2Free(s2);
            }
            swf_FontFree(swffont);

	    c->tag = tag;
	    c->type = TYPE_FONT;
	    c->data = font;
	    map16_add_id(map, id, c);
	}
	else if(tag->id == ST_DEFINETEXT ||
		tag->id == ST_DEFINETEXT2) {
	    character_t*c = rfx_calloc(sizeof(character_t));
	    c->tag = tag;
	    c->type = TYPE_TEXT;
	    c->data = 0;
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
	    gfximage_t*b = gfximage_new(data, width, height);
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
	
		if(!(p->po.flags&PF_CHAR)) p->po.id = old->po.id;
		if(!(p->po.flags&PF_MATRIX)) p->po.matrix = old->po.matrix;
		if(!(p->po.flags&PF_CXFORM)) p->po.cxform = old->po.cxform;
		if(!(p->po.flags&PF_RATIO)) p->po.ratio = old->po.ratio;

		map16_remove_id(depthmap, old->po.depth);
		placement_free(old);
	    }
	    map16_add_id(depthmap, p->po.depth, p);
	}
	if(tag->id == ST_REMOVEOBJECT ||
	   tag->id == ST_REMOVEOBJECT2) {
	    U16 depth = swf_GetDepth(tag);
	    map16_remove_id(depthmap, depth);
	}
	if(tag->id == ST_SHOWFRAME || tag->id == ST_END || !tag->next) {
	    if(frame == frame_to_extract) {
		return depthmap;
	    }
	    if(tag->id == ST_SHOWFRAME) {
		frame++;
		map16_enumerate(depthmap, increaseAge, 0);
	    }
	}
	if(tag->id == ST_END) 
	    break;
    }
    return depthmap;
}

// ---- rendering ----

void swf_ShapeApplyMatrix(SHAPE2*shape, MATRIX*m)
{
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
	
	p.x = line->sx; p.y = line->sy;
	p = swf_TurnPoint(p, m);
	line->sx = p.x; line->sy = p.y;

	line = line->next;
    }
}

static void renderCharacter(render_t*r, placement_t*p, character_t*c)
{
    if(c->type == TYPE_SHAPE) {
	SHAPE2 shape;
	swf_ParseDefineShape(c->tag, &shape);

	MATRIX m,m2;
	swf_MatrixJoin(&m2, &r->m, &r->current_placement->po.matrix);
	swf_MatrixJoin(&m, &m2, &p->po.matrix);

	swf_ApplyMatrixToShape(&shape, &m);

	SHAPELINE*line = shape.lines;
	int t;
	
	for(t=1;t<=shape.numlinestyles;t++) {
	   gfxline_t*line = swfline_to_gfxline(shape.lines, t, -1);
	   if(line) renderOutline(r, line, &shape.linestyles[t-1], &p->po.cxform);
	   gfxline_free(line);
	}

	for(t=1;t<=shape.numfillstyles;t++) {
	   gfxline_t*line;
	   line = swfline_to_gfxline(shape.lines, -1, t);
	   if(line) {
	       if(!p->po.clipdepth) {
		   renderFilled(r, line, &shape.fillstyles[t-1], &p->po.cxform, &p->po.matrix);
	       } else { 
		   r->device->startclip(r->device, line);
                   r->clips_waiting[p->po.clipdepth]++;
	       }
	   }
	   gfxline_free(line);
	   /*line = swfline_to_gfxline(shape.lines, -1, -1, t);
	   if(line) renderFilled(r, line, &shape.fillstyles[t-1], &p->po.cxform);
	   gfxline_free(line);*/
	}
	
    } else if(c->type == TYPE_TEXT) {
        TAG* tag = c->tag;
        textcallbackblock_t info;
        MATRIX mt,mt2;
        swf_SetTagPos(tag, 0);
        swf_GetU16(tag);
        swf_GetRect(tag,0);
        swf_GetMatrix(tag,&mt);

        swf_MatrixJoin(&mt2, &r->m, &mt);
        swf_MatrixJoin(&info.m, &mt2, &p->po.matrix);
        info.r = r;
        swf_ParseDefineText(tag, textcallback, &info);
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
        return;
    }
    
    if(c->type == TYPE_SPRITE) {
	placement_t*oldp = r->current_placement;
	r->current_placement = placement_join(oldp, p);

        int*old_clips_waiting = r->clips_waiting;
        r->clips_waiting = rfx_calloc(sizeof(r->clips_waiting[0])*65536);

        sprite_t* s = (sprite_t*)c->data;

	TAG*spritetags = c->tag->next;
        map16_t* depths = extractFrame(spritetags, s->frameCount>0? p->age % s->frameCount : 0);
        map16_enumerate(depths, placeObject, r);
       
        int t;
        for(t=0;t<65536;t++) {
            int i;
            for(i=0; i<r->clips_waiting[t]; i++) {
                r->device->endclip(r->device);
            }
        }
        free(r->clips_waiting);
        r->clips_waiting = old_clips_waiting;

	placement_free(r->current_placement);
	r->current_placement = oldp;
    } else {
	renderCharacter(r, p, c);
    }
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
    r.clips_waiting = malloc(sizeof(r.clips_waiting[0])*65536);
    r.current_placement = placement_unit();
    memset(r.clips_waiting, 0, sizeof(r.clips_waiting[0])*65536);

    int t;
    for(t=0;t<65536;t++) {
	if(depths->ids[t]) {
	    placeObject(&r, t, depths->ids[t]);
	}
        int i;
        for(i=0; i<r.clips_waiting[t]; i++) {
            output->endclip(output);
        }
    }
    free(r.clips_waiting);
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

void swf_doc_setparameter(gfxdocument_t*gfx, const char*name, const char*value)
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

void swf_setparameter(gfxsource_t*src, const char*name, const char*value)
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
    swf_doc->setparameter = swf_doc_setparameter;
    swf_doc->getpage = swf_doc_getpage;

    return swf_doc;
}

static void swf_destroy(gfxsource_t*src)
{
    memset(src, 0, sizeof(*src));
    free(src);
}


gfxsource_t*gfxsource_swf_create()
{
    gfxsource_t*src = (gfxsource_t*)malloc(sizeof(gfxsource_t));
    memset(src, 0, sizeof(gfxsource_t));
    src->setparameter = swf_setparameter;
    src->open = swf_open;
    src->destroy = swf_destroy;
    return src;
}

