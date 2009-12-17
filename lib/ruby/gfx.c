#include <ruby.h>
#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../devices/pdf.h"
#include "../readers/swf.h"
#include "../readers/image.h"
#include "../pdf/pdf.h"
#include "../mem.h"
#include "../types.h"

#define RUBY_GFX_VERSION  "0.9.0"

static VALUE GFX;
static VALUE Bitmap, Document, DocumentPage, PDFClass, SWFClass, ImageClass, Device;
static ID id_doc;

typedef struct doc_internal {
    gfxsource_t*driver; // filled by alloc
    gfxdocument_t*doc;
    gfxfontlist_t*list;
} doc_internal_t;

typedef struct page_internal {
    gfxpage_t*page;
} page_internal_t;

typedef struct image_internal {
    gfximage_t*image;
} image_internal_t;

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
    if(!page->page) {
	rb_raise(rb_eArgError, "No page %d in document", nr);
	return;
    }
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
static VALUE imgdrv_allocate(VALUE cls) {return doc_allocate(cls, imagedriver);}

// ------------------------ doc pages ---------------------------------------

static void page_free(page_internal_t*page)
{
    if(!page) return;
    if(page->page) {
	page->page->destroy(page->page);
	page->page = 0;
    }
    free(page);
}
static VALUE page_allocate(VALUE cls)
{
    page_internal_t*page = 0;
    VALUE v = Data_Make_Struct(cls, page_internal_t, 0, page_free, page);
    memset(page, 0, sizeof(page_internal_t));
    return v;
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

// ------------------------ image -------------------------------------------

#define Get_Image(image,cls) image_internal_t*image=0;Data_Get_Struct(cls, image_internal_t, image);

static void image_free(image_internal_t*image)
{
    free(image);
}
static VALUE image_allocate(VALUE cls)
{
    image_internal_t*image = 0;
    VALUE v = Data_Make_Struct(cls, image_internal_t, 0, image_free, image);
    memset(image, 0, sizeof(image_internal_t));
    return v;
}
static VALUE image_width(VALUE cls)
{
    Get_Image(image,cls)
    return INT2FIX(image->image->width);
}
static VALUE image_height(VALUE cls)
{
    Get_Image(image,cls)
    return INT2FIX(image->image->height);
}
static VALUE image_save_jpeg(VALUE cls, VALUE _filename, VALUE quality)
{
    Get_Image(image,cls)
    Check_Type(_filename, T_STRING);
    Check_Type(quality, T_FIXNUM);
    const char*filename = StringValuePtr(_filename);
    int l = strlen(filename);
    char jpeg = 0;
    if(l>=4 && !strcmp(&filename[l-4], ".jpg")) jpeg = 1;
    if(l>=5 && !strcmp(&filename[l-5], ".jpeg")) jpeg = 1;
    if(l>=4 && !strcmp(&filename[l-4], ".JPG")) jpeg = 1;
    jpeg_save(image->image->data, image->image->width, image->image->height, FIX2INT(quality), filename);
    return cls;
}
VALUE convert_image(gfximage_t*_image)
{
    VALUE v = image_allocate(Bitmap);
    Get_Image(image,v)
    image->image = _image;
    return v;
}
void invalidate_image(VALUE v)
{
    Get_Image(image,v)
    image->image = 0;
}

// ------------------------ gfx device --------------------------------------

static ID id_setparameter = 0;
static ID id_startpage = 0;
static ID id_startclip = 0;
static ID id_endclip = 0;
static ID id_stroke = 0;
static ID id_fill = 0;
static ID id_fillbitmap = 0;
static ID id_fillgradient = 0;
static ID id_addfont = 0;
static ID id_drawchar = 0;
static ID id_drawlink = 0;
static ID id_endpage = 0;
static ID id_geterror = 0;
static ID id_finish = 0;
static ID id_butt = 0;
static ID id_round = 0;
static ID id_square = 0;
static ID id_bevel = 0;
static ID id_miter = 0;
static ID id_move = 0;
static ID id_line = 0;
static ID id_spline = 0;
static ID id_radial = 0;
static ID id_linear = 0;

static VALUE noop(int argc, VALUE *argv, VALUE obj) {return obj;}

#define forward(v,id,args...) rb_respond_to((v), (id))?rb_funcall((v), (id), args):0

VALUE convert_line(gfxline_t*line)
{
    int len = 0;
    gfxline_t*l = line;
    while(l) {l=l->next;len++;}

    VALUE*a = malloc(sizeof(VALUE)*len);
    int pos = 0;
    l = line;
    while(l) {
	if(l->type == gfx_moveTo) {
	    a[pos] = rb_ary_new3(3, ID2SYM(id_move), rb_float_new(l->x), rb_float_new(l->y));
	} else if(l->type == gfx_lineTo) {
	    a[pos] = rb_ary_new3(3, ID2SYM(id_line), rb_float_new(l->x), rb_float_new(l->y));
	} else {
	    a[pos] = rb_ary_new3(5, ID2SYM(id_spline), rb_float_new(l->x), rb_float_new(l->y), rb_float_new(l->sx), rb_float_new(l->sy));
	}
	pos++;
	l=l->next;
    }
    return rb_ary_new4(len, a);
}
VALUE convert_color(gfxcolor_t*color)
{
    return rb_ary_new3(4, INT2FIX(color->a), INT2FIX(color->r), INT2FIX(color->g), INT2FIX(color->b));
}
VALUE convert_matrix(gfxmatrix_t*matrix)
{
    return rb_ary_new3(3, 
	    rb_ary_new3(2, rb_float_new(matrix->m00), rb_float_new(matrix->m01)),
	    rb_ary_new3(2, rb_float_new(matrix->m10), rb_float_new(matrix->m11)),
	    rb_ary_new3(2, rb_float_new(matrix->tx), rb_float_new(matrix->ty)));
}
int rb_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    VALUE ret = forward(v,id_setparameter,2,rb_tainted_str_new2(key),rb_tainted_str_new2(value));
    return 0;
}
void rb_startpage(gfxdevice_t*dev, int width, int height)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    VALUE ret = forward(v,id_startpage,2,INT2FIX(width),INT2FIX(height));
}
void rb_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    VALUE ret = forward(v,id_startclip,1,convert_line(line));
}
void rb_endclip(gfxdevice_t*dev)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    VALUE ret = forward(v,id_endclip,0);
}
void rb_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    
    ID cap = 0;
    if(cap_style == gfx_capButt) cap = id_butt;
    else if(cap_style == gfx_capRound) cap = id_round;
    else if(cap_style == gfx_capSquare) cap = id_square;
    
    ID joint = 0;
    if(joint_style == gfx_joinRound) joint = id_round;
    else if(joint_style == gfx_joinMiter) joint = id_miter;
    else if(joint_style == gfx_joinBevel) joint = id_bevel;

    forward(v, id_stroke, 6, convert_line(line), rb_float_new(width), convert_color(color), ID2SYM(cap), ID2SYM(joint), rb_float_new(miterLimit));
}
void rb_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    forward(v, id_fill, 2, convert_line(line), convert_color(color));
}
void rb_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    VALUE image = convert_image(img);
    forward(v, id_fillbitmap, 4, convert_line(line), image, convert_matrix(matrix), Qnil);
    invalidate_image(image);
}
void rb_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    ID typeid = (type == gfxgradient_linear)? id_linear : id_radial;
    forward(v, id_fillgradient, 4, convert_line(line), convert_gradient(gradient), ID2SYM(typeid), convert_matrix(matrix));
}
void rb_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    //forward(v, id_addfont, 1, convert_font(font));
}
void rb_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    //forward(v, id_drawchar, 1, convert_font(font));
}
void rb_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    forward(v, id_drawlink, convert_line(line), rb_tainted_str_new2(action));
}
void rb_endpage(gfxdevice_t*dev)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    forward(v, id_endpage, 0);
}
gfxresult_t* rb_finish(gfxdevice_t*dev)
{
    VALUE v = (VALUE)(ptroff_t)dev->internal;
    VALUE ret = forward(v, id_endpage, 0);
    gfxresult_t*r = (gfxresult_t*)rfx_calloc(sizeof(gfxresult_t));
    r->internal = (void*)(ptroff_t)ret;
    return r;
}

static VALUE page_render(VALUE cls, VALUE device)
{
    Check_Type(device, T_OBJECT);
    Get_Page(page,cls)

    gfxdevice_t dev;
    dev.internal = (void*)(ptroff_t)device;
    dev.setparameter = rb_setparameter;
    dev.startpage = rb_startpage;
    dev.startclip = rb_startclip;
    dev.endclip = rb_endclip;
    dev.stroke = rb_stroke;
    dev.fill = rb_fill;
    dev.fillbitmap = rb_fillbitmap;
    dev.fillgradient = rb_fillgradient;
    dev.addfont = rb_addfont;
    dev.drawchar = rb_drawchar;
    dev.drawlink = rb_drawlink;
    dev.endpage = rb_endpage;
    dev.finish = rb_finish;
    
    page->page->render(page->page, &dev);

    return cls;
}


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
    rb_define_method(DocumentPage, "render", page_render, 1);

    Document = rb_define_class_under(GFX, "Document", rb_cObject);
    rb_define_method(Document, "initialize", doc_initialize, 1);
    rb_define_method(Document, "page", doc_get_page, 1);
    rb_define_method(Document, "each_page", doc_each_page, 0);
    
    Bitmap = rb_define_class_under(GFX, "Bitmap", rb_cObject);
    rb_define_alloc_func(Bitmap, image_allocate);
    rb_define_method(Bitmap, "save_jpeg", image_save_jpeg, 2);
    
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
    
    ImageClass = rb_define_class_under(GFX, "ImageRead", Document);
    rb_define_alloc_func(ImageClass, imgdrv_allocate);

    id_setparameter = rb_intern("setparameter");
    id_startpage = rb_intern("startpage") ;
    id_startclip = rb_intern("startclip") ;
    id_endclip = rb_intern("endclip") ;
    id_stroke = rb_intern("stroke") ;
    id_fill = rb_intern("fill") ;
    id_fillbitmap = rb_intern("fillbitmap") ;
    id_fillgradient = rb_intern("fillgradient") ;
    id_addfont = rb_intern("addfont") ;
    id_drawchar = rb_intern("drawchar") ;
    id_drawlink = rb_intern("drawlink") ;
    id_endpage = rb_intern("endpage") ;
    id_geterror = rb_intern("geterror") ;
    id_finish = rb_intern("finish") ;
    id_butt = rb_intern("butt");
    id_round = rb_intern("round");
    id_square = rb_intern("square");
    id_miter = rb_intern("miter");
    id_bevel = rb_intern("bevel");
    id_move = rb_intern("move");
    id_line = rb_intern("line");
    id_spline = rb_intern("spline");
    id_radial = rb_intern("radial");
    id_linear = rb_intern("linear");
}

