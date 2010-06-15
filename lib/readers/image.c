#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../gfxtools.h"
#include "../log.h"
#include "../mem.h"
#include "../jpeg.h"
#include "../png.h"
#include "image.h"

typedef struct _image_page_internal
{
} image_page_internal_t;

typedef struct _image_doc_internal
{
    gfximage_t img;
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

    gfxcxform_t cxform;
    memset(&cxform, 0, sizeof(cxform));
    cxform.rr = 1;
    cxform.gg = 1;
    cxform.bb = 1;
    cxform.aa = 1;

    gfxmatrix_t m;
    memset(&m, 0, sizeof(m));
    m.m00 = 1;
    m.m11 = 1;

    gfxline_t* rect = gfxline_makerectangle(0, 0, pi->img.width, pi->img.height);
    output->fillbitmap(output, rect, &pi->img, &m, &cxform);
    gfxline_free(rect);
}

void imagepage_rendersection(gfxpage_t*page, gfxdevice_t*output, gfxcoord_t x, gfxcoord_t y, gfxcoord_t _x1, gfxcoord_t _y1, gfxcoord_t _x2, gfxcoord_t _y2)
{
    image_page_internal_t*i = (image_page_internal_t*)page->internal;
    image_doc_internal_t*pi = (image_doc_internal_t*)page->parent->internal;

    gfxcxform_t cxform;
    memset(&cxform, 0, sizeof(cxform));
    cxform.rr = 1;
    cxform.gg = 1;
    cxform.bb = 1;
    cxform.aa = 1;

    gfxmatrix_t m;
    memset(&m, 0, sizeof(m));
    m.m00 = 1;
    m.m11 = 1;
    m.tx = x;
    m.ty = y;
    
    gfxline_t* rect = gfxline_makerectangle(0, 0, pi->img.width, pi->img.height);
    gfxline_t* rect2 = gfxline_makerectangle(_x1, _y1, _x2, _y2);

    output->startclip(output, rect2);
    output->fillbitmap(output, rect, &pi->img, &m, &cxform);
    output->endclip(output);
    gfxline_free(rect);
    gfxline_free(rect2);
}

void image_doc_destroy(gfxdocument_t*gfx)
{
    image_doc_internal_t*i= (image_doc_internal_t*)gfx->internal;

    free(i->img.data);i->img.data = 0;

    free(gfx->internal);gfx->internal=0;
    free(gfx);gfx=0;
}

void image_doc_setparameter(gfxdocument_t*gfx, const char*name, const char*value)
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
    image_page->width = di->img.width;
    image_page->height = di->img.height;
    image_page->parent = doc;
    image_page->nr = page;
    return image_page;
}

static void image_setparameter(gfxsource_t*src, const char*name, const char*value)
{
    msg("<verbose> (gfxsource_image) setting parameter %s to \"%s\"", name, value);
}

static gfxdocument_t*image_open(gfxsource_t*src, const char*filename)
{
    gfxdocument_t*image_doc = (gfxdocument_t*)malloc(sizeof(gfxdocument_t));
    memset(image_doc, 0, sizeof(gfxdocument_t));
    image_doc_internal_t*i= (image_doc_internal_t*)malloc(sizeof(image_doc_internal_t));
    memset(i, 0, sizeof(image_doc_internal_t));

    gfxcolor_t*data = 0;
    unsigned width = 0;
    unsigned height = 0;

    if(!png_load(filename, &width, &height, (unsigned char**)&data)) {
	if(!jpeg_load(filename, (unsigned char**)&data, &width, &height)) {
	    msg("<error> Couldn't load image %s", filename);
	    return 0;
	}
    }
    i->img.data = data;
    i->img.width = width;
    i->img.height = height;

    image_doc->num_pages = 1;
    image_doc->internal = i;
    image_doc->get = 0;
    image_doc->destroy = image_doc_destroy;
    image_doc->setparameter = image_doc_setparameter;
    image_doc->getpage = image_doc_getpage;

    return image_doc;
}

gfxsource_t*gfxsource_image_create()
{
    gfxsource_t*src = (gfxsource_t*)malloc(sizeof(gfxsource_t));
    memset(src, 0, sizeof(gfxsource_t));
    src->setparameter = image_setparameter;
    src->open = image_open;
    return src;
}

