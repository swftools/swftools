#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../gfxtools.h"
#include "../log.h"
#include "../mem.h"
#include "../rfxswf.h"
#include "image.h"

typedef struct _image_page_internal
{
} image_page_internal_t;

typedef struct _image_doc_internal
{
} image_doc_internal_t;

void imagepage_destroy(gfxpage_t*image_page)
{
    image_page_internal_t*i= (image_page_internal_t*)image_page->internal;
    free(image_page->internal);image_page->internal = 0;
    free(image_page);image_page=0;
}

void imagepage_render(gfxpage_t*page, gfxdevice_t*output)
{
    image_page_internal_t*i = (image_page_internal_t*)page->internal;
    image_doc_internal_t*pi = (image_doc_internal_t*)page->parent->internal;
}

void imagepage_rendersection(gfxpage_t*page, gfxdevice_t*output, gfxcoord_t x, gfxcoord_t y, gfxcoord_t _x1, gfxcoord_t _y1, gfxcoord_t _x2, gfxcoord_t _y2)
{
    image_doc_internal_t*pi = (image_doc_internal_t*)page->parent->internal;
}

void image_doc_destroy(gfxdocument_t*gfx)
{
    image_doc_internal_t*i= (image_doc_internal_t*)gfx->internal;
    // ...
    free(gfx->internal);gfx->internal=0;
    free(gfx);gfx=0;
}

void image_doc_set_parameter(gfxdocument_t*gfx, char*name, char*value)
{
    image_doc_internal_t*i= (image_doc_internal_t*)gfx->internal;
}

gfxpage_t* image_doc_getpage(gfxdocument_t*doc, int page)
{
    image_doc_internal_t*di= (image_doc_internal_t*)doc->internal;
    if(page != 1)
        return 0;
    
    gfxpage_t* image_page = (gfxpage_t*)malloc(sizeof(gfxpage_t));
    image_page_internal_t*pi= (image_page_internal_t*)malloc(sizeof(image_page_internal_t));
    memset(pi, 0, sizeof(image_page_internal_t));

    image_page->internal = pi;
    image_page->destroy = imagepage_destroy;
    image_page->render = imagepage_render;
    image_page->rendersection = imagepage_rendersection;
    image_page->width = di->width;
    image_page->height = di->height;
    image_page->parent = doc;
    image_page->nr = page;
    return image_page;
}

static void image_set_parameter(gfxsource_t*src, char*name, char*value)
{
    msg("<verbose> (gfxsource_image) setting parameter %s to \"%s\"", name, value);
}

static gfxdocument_t*image_open(gfxsource_t*src, char*filename)
{
    gfxdocument_t*image_doc = (gfxdocument_t*)malloc(sizeof(gfxdocument_t));
    memset(image_doc, 0, sizeof(gfxdocument_t));
    image_doc_internal_t*i= (image_doc_internal_t*)malloc(sizeof(image_doc_internal_t));
    memset(i, 0, sizeof(image_doc_internal_t));

    gfxcolor_t*data = 0;
    int width = 0;
    int height = 0;

    if(!getPNG(filename, &width, &height, &data)) {
    }

    image_doc->num_pages = i->image.frameCount;
    image_doc->internal = i;
    image_doc->get = 0;
    image_doc->destroy = image_doc_destroy;
    image_doc->set_parameter = image_doc_set_parameter;
    image_doc->getpage = image_doc_getpage;

    return image_doc;
}

gfxsource_t*gfxsource_image_create()
{
    gfxsource_t*src = (gfxsource_t*)malloc(sizeof(gfxsource_t));
    memset(src, 0, sizeof(gfxsource_t));
    src->set_parameter = image_set_parameter;
    src->open = image_open;
    return src;
}
