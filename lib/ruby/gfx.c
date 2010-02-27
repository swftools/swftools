#include <ruby.h>
#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../gfxtools.h"
#include "../gfximage.h"
#include "../gfxfont.h"
#include "../gfxfilter.h"
#include "../devices/pdf.h"
#include "../readers/swf.h"
#include "../readers/image.h"
#include "../pdf/pdf.h"
#include "../mem.h"
#include "../types.h"
#include "../log.h"
#include "../args.h"

#define RUBY_GFX_VERSION  "0.9.0"

static VALUE GFX;
static VALUE Font, Glyph, Bitmap, Document, DocumentPage, PDFClass, SWFClass, ImageClass, Device;
static ID id_doc;

typedef struct doc_internal {
    VALUE self;
    gfxsource_t*driver; // filled by alloc
    gfxdocument_t*doc;
    gfxfontlist_t*fontlist;
} doc_internal_t;

typedef struct page_internal {
    doc_internal_t*doc;
    gfxpage_t*page;
} page_internal_t;

typedef struct image_internal {
    doc_internal_t*doc;
    gfximage_t*image;
} image_internal_t;

typedef struct font_internal {
    VALUE self;
    VALUE glyph_array;
    gfxfont_t*font;
} font_internal_t;

typedef struct glyph_internal {
    font_internal_t*font;
    int nr;
} glyph_internal_t;

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
    doc->fontlist = gfxfontlist_create();
    doc->doc = pdfdriver->open(pdfdriver, filename);
    if(!doc->doc) {
	rb_raise(rb_eIOError, "couldn't open %s", filename);
    }
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
    page->doc = doc;
    if(!page->page) {
	rb_raise(rb_eArgError, "No page %d in document", nr);
	return Qnil;
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
	page->doc = doc;
	rb_yield(v);
    }
    return cls;
}

static void doc_mark(doc_internal_t*doc)
{
    gfxfontlist_t*l = doc->fontlist;
    while(l) {
	if(l->user) 
	    rb_gc_mark((VALUE)l->user);
	l = l->next;
    }
}

static void doc_free(doc_internal_t*doc)
{
    gfxfontlist_free(doc->fontlist, 0);
    if(doc->doc) {
	doc->doc->destroy(doc->doc);
    }
    doc->doc = 0;
    free(doc);
}

static VALUE doc_allocate(VALUE cls, gfxsource_t*driver)
{
    doc_internal_t*doc = 0;
    VALUE v = Data_Make_Struct(cls, doc_internal_t, doc_mark, doc_free, doc);
    doc->self = v;
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
static void page_mark(page_internal_t*page)
{
    rb_gc_mark(page->doc->self);
}
static VALUE page_allocate(VALUE cls)
{
    page_internal_t*page = 0;
    VALUE v = Data_Make_Struct(cls, page_internal_t, page_mark, page_free, page);
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
static void image_mark(image_internal_t*image)
{
    rb_gc_mark(image->doc->self);
}
static VALUE image_allocate(VALUE cls)
{
    image_internal_t*image = 0;
    VALUE v = Data_Make_Struct(cls, image_internal_t, image_mark, image_free, image);
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
static VALUE image_rescale(VALUE cls, VALUE _width, VALUE _height)
{
    Get_Image(image,cls)
    Check_Type(_width, T_FIXNUM);
    Check_Type(_height, T_FIXNUM);
    int width = FIX2INT(_width);
    int height = FIX2INT(_height);
    volatile VALUE v_image2 = image_allocate(Bitmap);
    Get_Image(image2,v_image2)
    image2->doc = image->doc;
    image2->image = gfximage_rescale(image->image, width, height);
    if(!image2->image) {
	rb_raise(rb_eArgError, "Can't rescale to size %dx%d", width, height);
    }
    return v_image2;
}
static VALUE image_has_alpha(VALUE cls)
{
    Get_Image(image,cls)
    int size = image->image->width * image->image->height;
    gfxcolor_t*data = image->image->data;
    int t;
    for(t=0;t<size;t++) {
	if(data->a!=255) 
	    return Qtrue;
    }
    return Qfalse;
}
static VALUE image_save_jpeg(VALUE cls, VALUE _filename, VALUE quality)
{
    Get_Image(image,cls)
    Check_Type(_filename, T_STRING);
    Check_Type(quality, T_FIXNUM);
    const char*filename = StringValuePtr(_filename);
    gfximage_save_jpeg(image->image, filename, FIX2INT(quality));
    return cls;
}
static VALUE image_save_png(VALUE cls, VALUE _filename)
{
    Get_Image(image,cls)
    Check_Type(_filename, T_STRING);
    const char*filename = StringValuePtr(_filename);
    gfximage_save_png(image->image, filename);
    return cls;
}
VALUE convert_image(doc_internal_t*doc,gfximage_t*_image)
{
    VALUE v = image_allocate(Bitmap);
    Get_Image(image,v)
    image->image = _image;
    image->doc = doc;
    return v;
}
void invalidate_image(VALUE v)
{
    Get_Image(image,v)
    image->image = 0;
}

// ------------------------ glyphs ------------------------------------------

static VALUE convert_line(gfxline_t*line);

#define Get_Glyph(glyph,cls) glyph_internal_t*glyph=0;Data_Get_Struct(cls, glyph_internal_t, glyph);

static void glyph_free(glyph_internal_t*glyph)
{
    free(glyph);
}

static void glyph_mark(glyph_internal_t*glyph)
{
    rb_gc_mark(glyph->font->self);
}

static VALUE glyph_allocate(VALUE cls)
{
    glyph_internal_t*glyph = 0;
    VALUE v = Data_Make_Struct(cls, glyph_internal_t, glyph_mark, glyph_free, glyph);
    memset(glyph, 0, sizeof(glyph_internal_t));
    return v;
}

static VALUE glyph_polygon(VALUE cls)
{
    Get_Glyph(glyph,cls);
    return convert_line(glyph->font->font->glyphs[glyph->nr].line);
}

static VALUE glyph_advance(VALUE cls)
{
    Get_Glyph(glyph,cls);
    return rb_float_new(glyph->font->font->glyphs[glyph->nr].advance);
}

static VALUE glyph_bbox(VALUE cls)
{
    Get_Glyph(glyph,cls);
    gfxbbox_t bbox = gfxline_getbbox(glyph->font->font->glyphs[glyph->nr].line);
    return rb_ary_new3(4, rb_float_new(bbox.xmin), 
	                  rb_float_new(bbox.ymin), 
			  rb_float_new(bbox.xmax), 
			  rb_float_new(bbox.ymax));
}

static VALUE glyph_unicode(VALUE cls)
{
    Get_Glyph(glyph,cls);
    return INT2FIX(glyph->font->font->glyphs[glyph->nr].unicode);
}

// ------------------------ font --------------------------------------------

#define Get_Font(font,cls) font_internal_t*font=0;Data_Get_Struct(cls, font_internal_t, font);

static void font_mark(font_internal_t*font)
{
    rb_gc_mark(font->glyph_array);
}

static void font_free(font_internal_t*font)
{
    free(font);
}

static VALUE font_allocate(VALUE cls)
{
    font_internal_t*font = 0;
    VALUE v = Data_Make_Struct(cls, font_internal_t, font_mark, font_free, font);
    memset(font, 0, sizeof(font_internal_t));
    font->self = v;
    return v;
}

static VALUE font_ascent(VALUE cls)
{
    Get_Font(font,cls);
    return rb_float_new(font->font->ascent);
}

static VALUE font_descent(VALUE cls)
{
    Get_Font(font,cls);
    return rb_float_new(font->font->descent);
}

static VALUE font_name(VALUE cls)
{
    Get_Font(font,cls);
    return rb_tainted_str_new2(font->font->id);
}

static VALUE font_glyphs(VALUE cls)
{
    Get_Font(font,cls);
    return font->glyph_array;
}

static VALUE font_save_ttf(VALUE cls, VALUE _filename)
{
    Get_Font(font,cls);
    Check_Type(_filename, T_STRING);
    const char*filename = StringValuePtr(_filename);
    gfxfont_save(font->font, filename);
    return Qnil;
}

static VALUE font_kerning(VALUE cls)
{
    Get_Font(font,cls);
    gfxkerning_t*kerning = font->font->kerning;
    int kerning_size = font->font->kerning_size;
    volatile VALUE a = rb_ary_new2(kerning_size);
    int t;
    for(t=0;t<kerning_size;t++) {
	volatile VALUE tuple = rb_ary_new2(3);
	rb_ary_store(tuple, 0, INT2FIX(kerning[t].c1));
	rb_ary_store(tuple, 1, INT2FIX(kerning[t].c2));
	rb_ary_store(tuple, 2, INT2FIX(kerning[t].advance));
	rb_ary_store(a, t, tuple);
    }
    return a;
}

// ------------------------ gfx device --------------------------------------

typedef struct device_internal {
    doc_internal_t*doc;
    VALUE v;
} device_internal_t;

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
static ID id_remove_font_transforms = 0;
static ID id_maketransparent = 0;

static VALUE noop(int argc, VALUE *argv, VALUE obj) {return obj;}

#define forward(v,id,args...) rb_respond_to((v), (id))?rb_funcall((v), (id), args):0

VALUE convert_line(gfxline_t*line)
{
    int len = 0;
    gfxline_t*l = line;
    while(l) {l=l->next;len++;}

    volatile VALUE array = rb_ary_new2(len);

    int pos = 0;
    l = line;
    while(l) {
	volatile VALUE e;
	if(l->type == gfx_moveTo) {
	    e = rb_ary_new3(3, ID2SYM(id_move), Qfalse, Qfalse);
	    rb_ary_store(array, pos, e);
	    rb_ary_store(e, 1, rb_float_new(l->x));
	    rb_ary_store(e, 2, rb_float_new(l->y));
	} else if(l->type == gfx_lineTo) {
	    e = rb_ary_new3(3, ID2SYM(id_line), Qfalse, Qfalse);
	    rb_ary_store(array, pos, e);
	    rb_ary_store(e, 1, rb_float_new(l->x));
	    rb_ary_store(e, 2, rb_float_new(l->y));
	} else {
	    e = rb_ary_new3(5, ID2SYM(id_spline), Qfalse, Qfalse, Qfalse, Qfalse);
	    rb_ary_store(array, pos, e);
	    rb_ary_store(e, 1, rb_float_new(l->x));
	    rb_ary_store(e, 2, rb_float_new(l->y));
	    rb_ary_store(e, 3, rb_float_new(l->sx));
	    rb_ary_store(e, 4, rb_float_new(l->sy));
	}
	pos++;
	l=l->next;
    }
    return array;
}
VALUE convert_color(gfxcolor_t*color)
{
    return rb_ary_new3(4, INT2FIX(color->a), INT2FIX(color->r), INT2FIX(color->g), INT2FIX(color->b));
}
VALUE convert_matrix(gfxmatrix_t*matrix)
{
    volatile VALUE array = rb_ary_new2(3);
    volatile VALUE a = rb_ary_new2(2);
    rb_ary_store(array, 0, a);
    rb_ary_store(a, 0, rb_float_new(matrix->m00));
    rb_ary_store(a, 1, rb_float_new(matrix->m01));
    a = rb_ary_new2(2);
    rb_ary_store(array, 1, a);
    rb_ary_store(a, 0, rb_float_new(matrix->m10));
    rb_ary_store(a, 1, rb_float_new(matrix->m11));
    a = rb_ary_new2(2);
    rb_ary_store(array, 2, a);
    rb_ary_store(a, 0, rb_float_new(matrix->tx));
    rb_ary_store(a, 1, rb_float_new(matrix->ty));
    return array;
}
static VALUE font_is_cached(device_internal_t*i, gfxfont_t*font)
{
    return (VALUE)gfxfontlist_getuserdata(i->doc->fontlist, font->id);
}
static void cache_font(device_internal_t*i, gfxfont_t*font, VALUE v)
{
    i->doc->fontlist = gfxfontlist_addfont2(i->doc->fontlist, font, (void*)v);
}
static VALUE convert_font(gfxfont_t*font)
{
    volatile VALUE v2 = font_allocate(Font);
    Get_Font(f, v2);
    f->font = font;
    f->glyph_array = rb_ary_new2(font->num_glyphs);

    int t;
    for(t=0;t<font->num_glyphs;t++) {
	volatile VALUE a = glyph_allocate(Glyph);
	rb_ary_store(f->glyph_array, t, a);
	Get_Glyph(g, a);
	g->font = f;
	g->nr = t;
    }
    return v2;
}
static VALUE convert_gradient(gfxgradient_t*gradient)
{
    return Qnil; //TODO
}
#define HEAD \
    device_internal_t*i = (device_internal_t*)dev->internal; \
    VALUE v = i->v;
int rb_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    HEAD
    volatile VALUE v_key = rb_tainted_str_new2(key);
    volatile VALUE v_value = rb_tainted_str_new2(value);
    VALUE ret = forward(v,id_setparameter,2,v_key,v_value);
    return 0;
}
void rb_startpage(gfxdevice_t*dev, int width, int height)
{
    HEAD
    VALUE ret = forward(v,id_startpage,2,INT2FIX(width),INT2FIX(height));
}
void rb_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    HEAD
    volatile VALUE v_line = convert_line(line);
    VALUE ret = forward(v,id_startclip,1,v_line);
}
void rb_endclip(gfxdevice_t*dev)
{
    HEAD
    VALUE ret = forward(v,id_endclip,0);
}
void rb_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    HEAD
    
    ID cap = 0;
    if(cap_style == gfx_capButt) cap = id_butt;
    else if(cap_style == gfx_capRound) cap = id_round;
    else if(cap_style == gfx_capSquare) cap = id_square;
    
    ID joint = 0;
    if(joint_style == gfx_joinRound) joint = id_round;
    else if(joint_style == gfx_joinMiter) joint = id_miter;
    else if(joint_style == gfx_joinBevel) joint = id_bevel;

    volatile VALUE v_line = convert_line(line);
    volatile VALUE v_width = rb_float_new(width);
    volatile VALUE v_color = convert_color(color);
    volatile VALUE v_miter = rb_float_new(miterLimit);
    forward(v, id_stroke, 6, v_line, v_width, v_color, ID2SYM(cap), ID2SYM(joint), v_miter);
}
void rb_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    HEAD
    
    volatile VALUE v_line = convert_line(line);
    volatile VALUE v_color = convert_color(color);
    forward(v, id_fill, 2, v_line, v_color);
}
void rb_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    HEAD
    volatile VALUE v_image = convert_image(i->doc, img);
    volatile VALUE v_line = convert_line(line);
    volatile VALUE v_matrix = convert_matrix(matrix);
    forward(v, id_fillbitmap, 4, v_line, v_image, v_matrix, Qnil);
    invalidate_image(v_image);
}
void rb_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    HEAD
    ID typeid = (type == gfxgradient_linear)? id_linear : id_radial;
    
    volatile VALUE v_line = convert_line(line);
    volatile VALUE v_matrix = convert_matrix(matrix);
    volatile VALUE v_gradient = convert_gradient(gradient);
    forward(v, id_fillgradient, 4, v_line, v_gradient, ID2SYM(typeid), v_matrix);
}
void rb_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    HEAD

    volatile VALUE f = font_is_cached(i, font);
    if(!f) {f=convert_font(font);cache_font(i,font,f);}

    forward(v, id_addfont, 1, f);
}
void rb_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    HEAD
    volatile VALUE f = font_is_cached(i, font);
    if(!f) {f=convert_font(font);cache_font(i,font,f);}

    volatile VALUE v_color = convert_color(color);
    volatile VALUE v_matrix = convert_matrix(matrix);
    forward(v, id_drawchar, 4, f, INT2FIX(glyphnr), v_color, v_matrix);
}
void rb_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action)
{
    HEAD
    volatile VALUE v_line = convert_line(line);
    volatile VALUE v_action = rb_tainted_str_new2(action);
    forward(v, id_drawlink, v_line, v_action);
}
void rb_endpage(gfxdevice_t*dev)
{
    HEAD
    forward(v, id_endpage, 0);
}
void gfxresult_rb_destroy(gfxresult_t*r)
{
    free(r);
}
gfxresult_t* rb_finish(gfxdevice_t*dev)
{
    HEAD
    VALUE ret = forward(v, id_finish, 0);
    gfxresult_t*r = (gfxresult_t*)rfx_calloc(sizeof(gfxresult_t));
    r->destroy = gfxresult_rb_destroy;
    r->internal = (void*)(ptroff_t)ret;
    return r;
}

#define make_device(dev, idoc, device) \
    gfxdevice_t dev; \
    device_internal_t i; \
    i.v = device; \
    i.doc = idoc; \
    dev.internal = &i; \
    dev.setparameter = rb_setparameter; \
    dev.startpage = rb_startpage; \
    dev.startclip = rb_startclip; \
    dev.endclip = rb_endclip; \
    dev.stroke = rb_stroke; \
    dev.fill = rb_fill; \
    dev.fillbitmap = rb_fillbitmap; \
    dev.fillgradient = rb_fillgradient; \
    dev.addfont = rb_addfont; \
    dev.drawchar = rb_drawchar; \
    dev.drawlink = rb_drawlink; \
    dev.endpage = rb_endpage; \
    dev.finish = rb_finish;

static VALUE page_render(VALUE cls, VALUE device)
{
    Check_Type(device, T_OBJECT);
    Get_Page(page,cls)

    make_device(dev, page->doc, device);

    dev.startpage(&dev, page->page->width, page->page->height);
    page->page->render(page->page, &dev);
    dev.endpage(&dev);

    return cls;
}

static VALUE doc_render(VALUE cls, VALUE device, VALUE _range, VALUE filters)
{
    const char*range = 0;
    if(!NIL_P(_range)) {
	Check_Type(_range, T_STRING);
	range = StringValuePtr(_range);
    }
    Get_Doc(doc,cls);

    make_device(_dev, doc, device);
    gfxdevice_t*dev = &_dev;

    if(!NIL_P(filters)) {
	if(TYPE(filters) != T_ARRAY)
	    rb_raise(rb_eArgError, "third argument of doc->render must be an array of symbols");

	int len = RARRAY(filters)->len;
	int t=0;
	while(t<len) {
	    VALUE filter = RARRAY(filters)->ptr[t++];
	    Check_Type(filter, T_SYMBOL);
	    ID id = SYM2ID(filter);
#           define PARAM(x) VALUE x;if(t==len) rb_raise(rb_eArgError, "End of array while parsing arguments for filter %s", rb_id2name(id)); \
	                    else x = RARRAY(filters)->ptr[t++];
	    if(id == id_remove_font_transforms) {
		wrap_filter2(dev, remove_font_transforms);
	    } else if(id == id_maketransparent) {
		PARAM(alpha);
		wrap_filter(dev, maketransparent, FIX2INT(alpha));
	    } else {
		rb_raise(rb_eArgError, "unknown filter %s", rb_id2name(id));
	    }
	}
    }

    int pagenr;
    for(pagenr=1;pagenr<=doc->doc->num_pages;pagenr++) {
	if(is_in_range(pagenr, (char*)range)) {
	    gfxpage_t*page = doc->doc->getpage(doc->doc, pagenr);
	    dev->startpage(dev, page->width, page->height);
	    page->render(page, dev);
	    dev->endpage(dev);
	    page->destroy(page);
	}
    }
   

    gfxresult_t*r = dev->finish(dev);
    r->destroy(r);

    return Qnil;
}

static VALUE doc_prepare(VALUE cls, VALUE device)
{
    Get_Doc(doc,cls);
    make_device(dev, doc, device);
    doc->doc->prepare(doc->doc, &dev);
    return cls;
}


// ---------------------- global functions ----------------------------------

VALUE gfx_setparameter(VALUE module, VALUE _key, VALUE _value)
{
    Check_Type(_key, T_STRING);
    Check_Type(_value, T_STRING);
    const char*key = StringValuePtr(_key);
    const char*value = StringValuePtr(_value);
    pdfdriver->setparameter(pdfdriver, key, value);
    swfdriver->setparameter(swfdriver, key, value);
    imagedriver->setparameter(imagedriver, key, value);
    return GFX;
}

// --------------------------------------------------------------------------

void Init_gfx()
{
    initLog(0,0,0,0,0,2);
    pdfdriver = gfxsource_pdf_create();
    swfdriver = gfxsource_swf_create();
    imagedriver = gfxsource_image_create();

    GFX = rb_define_module("GFX");
    
    rb_define_module_function(GFX, "setparameter", gfx_setparameter, 2);
    
    DocumentPage = rb_define_class_under(GFX, "DocumentPage", rb_cObject);
    rb_define_method(DocumentPage, "width", page_width, 0);
    rb_define_method(DocumentPage, "height", page_height, 0);
    rb_define_method(DocumentPage, "nr", page_nr, 0);
    rb_define_method(DocumentPage, "render", page_render, 1);

    Document = rb_define_class_under(GFX, "Document", rb_cObject);
    rb_define_method(Document, "initialize", doc_initialize, 1);
    rb_define_method(Document, "page", doc_get_page, 1);
    rb_define_method(Document, "each_page", doc_each_page, 0);
    rb_define_method(Document, "prepare", doc_prepare, 1);
    rb_define_method(Document, "render", doc_render, 3);
    
    Bitmap = rb_define_class_under(GFX, "Bitmap", rb_cObject);
    rb_define_method(Bitmap, "save_jpeg", image_save_jpeg, 2);
    rb_define_method(Bitmap, "save_png", image_save_png, 1);
    rb_define_method(Bitmap, "width", image_width, 0);
    rb_define_method(Bitmap, "height", image_height, 0);
    rb_define_method(Bitmap, "rescale", image_rescale, 2);
    rb_define_method(Bitmap, "has_alpha", image_has_alpha, 0);
    
    Glyph = rb_define_class_under(GFX, "Glyph", rb_cObject);
    rb_define_method(Glyph, "polygon", glyph_polygon, 0);
    rb_define_method(Glyph, "unicode", glyph_unicode, 0);
    rb_define_method(Glyph, "advance", glyph_advance, 0);
    rb_define_method(Glyph, "bbox", glyph_bbox, 0);
    
    Font = rb_define_class_under(GFX, "Font", rb_cObject);
    rb_define_method(Font, "name", font_name, 0);
    rb_define_method(Font, "ascent", font_ascent, 0);
    rb_define_method(Font, "descent", font_descent, 0);
    rb_define_method(Font, "glyphs", font_glyphs, 0);
    rb_define_method(Font, "kerning", font_kerning, 0);
    rb_define_method(Font, "get_kerning_table", font_kerning, 0);
    rb_define_method(Font, "save_ttf", font_save_ttf, 1);
    
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
    id_remove_font_transforms = rb_intern("remove_font_transforms");
    id_maketransparent = rb_intern("maketransparent");
}

