#include <ruby.h>
#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../devices/pdf.h"
#include "../readers/swf.h"
#include "../readers/image.h"
#include "../pdf/pdf.h"
#include "../types.h"

#define RUBY_GFX_VERSION  "0.9.0"

static VALUE GFX;
static VALUE Document, DocumentPage, PDFClass, SWFClass, ImageClass, Device;
static ID id_doc;

typedef struct doc_internal {
    gfxsource_t*driver; // filled by alloc
    gfxdocument_t*doc;
} doc_internal_t;

typedef struct page_internal {
    gfxpage_t*page;
} page_internal_t;

static gfxsource_t* pdfdriver = 0;
static gfxsource_t* imagedriver = 0;
static gfxsource_t* swfdriver = 0;

#define Get_Doc(doc,cls) doc_internal_t*doc=0;Data_Get_Struct(cls, doc_internal_t, doc);
#define Get_Page(page,cls) page_internal_t*page=0;Data_Get_Struct(cls, page_internal_t, page);

static VALUE doc_allocate(VALUE cls, gfxsource_t*driver);
static VALUE page_allocate(VALUE cls);

// ------------------------ documents ---------------------------------------

static VALUE doc_initialize(VALUE cls, VALUE _filename)
{
    Check_Type(_filename, T_STRING);
    Get_Doc(doc,cls);
    const char*filename = StringValuePtr(_filename);
    doc->doc = pdfdriver->open(pdfdriver, filename);
    return cls;
}

static VALUE doc_num_pages(VALUE cls)
{
    Get_Doc(doc,cls)
    return INT2FIX(doc->doc->num_pages);
}


static VALUE doc_get_page(VALUE cls, VALUE _nr)
{
    Check_Type(_nr, T_FIXNUM);
    int nr = FIX2INT(_nr);
    Get_Doc(doc,cls);

    VALUE v = page_allocate(DocumentPage);
    Get_Page(page,v)
    page->page = doc->doc->getpage(doc->doc, nr);
    return v;
}

static VALUE doc_each_page(VALUE cls)
{
    Get_Doc(doc,cls);
    int t;
    for(t=1;t<=doc->doc->num_pages;t++) {
	VALUE v = page_allocate(DocumentPage);
	Get_Page(page,v)
	page->page = doc->doc->getpage(doc->doc, t);
	rb_yield(v);
    }
    return cls;
}

static void doc_mark(doc_internal_t*doc)
{
    // e.g. rb_gc_mark(z->page);
}

static void doc_free(doc_internal_t*doc)
{
    doc->doc->destroy(doc->doc);
    free(doc);
}

static VALUE doc_allocate(VALUE cls, gfxsource_t*driver)
{
    doc_internal_t*doc = 0;
    VALUE v = Data_Make_Struct(cls, doc_internal_t, 0, doc_free, doc);
    memset(doc, 0, sizeof(doc_internal_t));
    doc->driver = driver;
    return v;
}

static VALUE pdf_allocate(VALUE cls) {return doc_allocate(cls, pdfdriver);}
static VALUE swf_allocate(VALUE cls) {return doc_allocate(cls, swfdriver);}
static VALUE image_allocate(VALUE cls) {return doc_allocate(cls, imagedriver);}

// ------------------------ doc pages ---------------------------------------

static VALUE page_free(VALUE cls)
{
    Get_Page(page, cls);
    page->page->destroy(page->page);
    free(page);
    return cls;
}

static VALUE page_allocate(VALUE cls)
{
    page_internal_t*page = 0;
    VALUE v = Data_Make_Struct(cls, page_internal_t, 0, page_free, page);
    memset(page, 0, sizeof(page_internal_t));
    return v;
}

static VALUE page_render(VALUE cls, VALUE device)
{
    Check_Type(device, T_CLASS);
    Get_Page(page,cls)

    gfxdevice_t dev;
    dev.internal = (void*)(ptroff_t)device;

    page->page->render(page->page, 0);//device);
    //rb_yield(v);
}

static VALUE page_nr(VALUE cls)
{
    Get_Page(page,cls)
    return INT2FIX(page->page->nr);
}

static VALUE page_width(VALUE cls)
{
    Get_Page(page,cls)
    return INT2FIX(page->page->width);
}

static VALUE page_height(VALUE cls)
{
    Get_Page(page,cls)
    return INT2FIX(page->page->height);
}

// ------------------------ gfx device --------------------------------------

static VALUE noop(int argc, VALUE *argv, VALUE obj) {return obj;}

// --------------------------------------------------------------------------

void Init_gfx()
{
    initLog(0,0,0,0,0,2);
    pdfdriver = gfxsource_pdf_create();
    swfdriver = gfxsource_swf_create();
    imagedriver = gfxsource_image_create();

    GFX = rb_define_module("GFX");
    
    DocumentPage = rb_define_class_under(GFX, "DocumentPage", rb_cObject);
    rb_define_alloc_func(DocumentPage, page_allocate);
    rb_define_method(DocumentPage, "width", page_width, 0);
    rb_define_method(DocumentPage, "height", page_height, 0);
    rb_define_method(DocumentPage, "nr", page_nr, 0);
    rb_define_method(DocumentPage, "render", page_render, 0);

    Document = rb_define_class_under(GFX, "Document", rb_cObject);
    rb_define_method(Document, "initialize", doc_initialize, 1);
    rb_define_method(Document, "page", doc_get_page, 1);
    rb_define_method(Document, "each_page", doc_each_page, 0);
    
    Device = rb_define_class_under(GFX, "Device", rb_cObject);
    rb_define_method(Device, "startpage", noop, -1);
    rb_define_method(Device, "endpage", noop, -1);
    rb_define_method(Device, "startclip", noop, -1);
    rb_define_method(Device, "endclip", noop, -1);
    rb_define_method(Device, "stroke", noop, -1);
    rb_define_method(Device, "fill", noop, -1);
    rb_define_method(Device, "fillbitmap", noop, -1);
    rb_define_method(Device, "fillgradient", noop, -1);
    rb_define_method(Device, "addfont", noop, -1);
    rb_define_method(Device, "drawchar", noop, -1);
    rb_define_method(Device, "drawlink", noop, -1);
    rb_define_method(Device, "endpage", noop, -1);

    PDFClass = rb_define_class_under(GFX, "PDF", Document);
    rb_define_alloc_func(PDFClass, pdf_allocate);
    
    SWFClass = rb_define_class_under(GFX, "SWF", Document);
    rb_define_alloc_func(SWFClass, swf_allocate);
    
    ImageClass = rb_define_class_under(GFX, "Image", Document);
    rb_define_alloc_func(ImageClass, image_allocate);

    //id_doc = rb_intern("doc");
}

