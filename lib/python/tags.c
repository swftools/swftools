#include "pyutils.h"
#include "primitives.h"
#include "action.h"
#include "taglist.h"
#include "tag.h"
#include "tags.h"
#include "image.h"

//----------------------------------------------------------------------------

typedef struct _font_internal
{
    SWFFONT* font;
} font_internal_t;
staticforward tag_internals_t font_tag;

static int font_parse(tag_internals_t*self)
{
    font_internal_t*font = (font_internal_t*)self->data;
    /* TODO */
    PyErr_SetString(PyExc_Exception, setError("Font parsing not implemented yet"));
    return 0;
}
static void font_dealloc(tag_internals_t*self)
{
    font_internal_t*font = (font_internal_t*)self->data;
    if(font->font) {
	swf_FontFree(font->font);
	font->font = 0;
    }
}
static int font_fillTAG(tag_internals_t*self)
{
    font_internal_t*fi = (font_internal_t*)self->data;
    if(self->tag)
	return 1;
    self->tag = swf_InsertTag(0, ST_DEFINEFONT2);
    swf_FontSetDefine2(self->tag, fi->font);
    return 1;
}
static PyObject* f_DefineFont(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"filename", NULL};
    char*filename = 0;
    PyObject*tag;
    SWFFONT* font;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &filename))
	return NULL;

    font = swf_LoadFont(filename);
    if(!font) {
	PyErr_SetString(PyExc_Exception, setError("Could not load %s", filename));
	return NULL;
    }

    tag = tag_new(&font_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    font_internal_t*fi = (font_internal_t*)itag->data;
    font->id = 0;
    fi->font = font;
    return (PyObject*)tag;
}
static SWFFONT* font_getSWFFONT(PyObject*self)
{
    PY_ASSERT_TYPE(self, &TagClass);
    tag_internals_t*itag = tag_getinternals(self);
    font_internal_t*fi = (font_internal_t*)itag->data;
    return fi->font;
}
static tag_internals_t font_tag =
{
    parse: font_parse,
    fillTAG: font_fillTAG,
    dealloc: font_dealloc,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: sizeof(font_internal_t),
};
//----------------------------------------------------------------------------

typedef struct _placeobject_internal
{
    SWFPLACEOBJECT* po;
} placeobject_internal_t;
staticforward tag_internals_t placeobject_tag;

static void po_dealloc(tag_internals_t*self)
{
    placeobject_internal_t*pi = (placeobject_internal_t*)self->data;
    if(pi->po) {
	swf_PlaceObjectFree(pi->po);
	pi->po = 0;
    }
}
static int po_parse(tag_internals_t*self)
{
    placeobject_internal_t*pi = (placeobject_internal_t*)self->data;
    /* TODO */
    PyErr_SetString(PyExc_Exception, setError("Font parsing not implemented yet"));
    return 0;
}
static int po_fillTAG(tag_internals_t*self)
{
    placeobject_internal_t*pi = (placeobject_internal_t*)self->data;
    self->tag = swf_InsertTag(0, ST_PLACEOBJECT2);
    swf_SetPlaceObject(self->tag, pi->po);
    return 1;
}
static PyObject* po_create(PyObject* self, PyObject* args, PyObject* kwargs,char move)
{
    static char *kwlist[] = {"character", "depth", "matrix", "colortransform", "ratio", "name", "clipdepth", "action", NULL};
    
    PyObject*character = 0;
    int depth;
    int clipdepth = 0;
    PyObject*matrix = 0;
    PyObject*cxform = 0;
    PyObject*action = 0;
    int ratio = 0;
    char* name = 0;
    SWFPLACEOBJECT* po;
    po = malloc(sizeof(SWFPLACEOBJECT));
    memset(po, 0, sizeof(SWFPLACEOBJECT));

    swf_GetPlaceObject(0, po);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Oi|O!O!isiO!", kwlist, 
		&character, 
		&depth, 
		&MatrixClass, &matrix, 
		&CXFormClass, &cxform,
		&ratio,
		&name,
		&clipdepth,
		&ActionClass, &action
		))
	return NULL;
    po->depth = depth;
    po->clipdepth = clipdepth;
    po->ratio = ratio;
    po->name = name;
    po->move = move;
    if(clipdepth) po->clipdepth = clipdepth;
    if(matrix) po->matrix = matrix_getMatrix(matrix);
    if(cxform) po->cxform = colortransform_getCXForm(cxform);
    if(action) po->actions = action_getAction(action);

    PyObject*tag;
    tag = tag_new(&placeobject_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    placeobject_internal_t*pi = (placeobject_internal_t*)itag->data;
    pi->po = po;
    if(!move) {
	pi->po->id = tagmap_add(itag->tagmap,(PyObject*)character);
    } else {
	pi->po->id = 0;
    }
    
    mylog("+%08x(%d) PlaceObject %08x(%d)\n", (int)tag, tag->ob_refcnt, character, character->ob_refcnt);

    return (PyObject*)tag;
}
static PyObject* f_PlaceObject(PyObject* self, PyObject* args, PyObject* kwargs)
{
    return po_create(self, args, kwargs, 0);
}
static PyObject* f_MoveObject(PyObject* self, PyObject* args, PyObject* kwargs)
{
    return po_create(self, args, kwargs, 1);
}
static tag_internals_t placeobject_tag =
{
    parse: po_parse,
    fillTAG: po_fillTAG,
    dealloc: po_dealloc,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: sizeof(placeobject_internal_t),
};
//----------------------------------------------------------------------------
staticforward tag_internals_t bgcolor_tag;
static PyObject* tag_setbackgroundcolor_getrgb(PyObject * self, PyObject*other)
{
    tag_internals_t*itag = tag_getinternals(self);
    int r,g,b;
    r = itag->tag->data[0];
    g = itag->tag->data[1];
    b = itag->tag->data[2];
    return Py_BuildValue("(iii)", r,g,b);
}
static PyMethodDef setbgcolor_methods[] = 
{{"getRGB", tag_setbackgroundcolor_getrgb, METH_VARARGS, "get's the color set by this tag"},
 {NULL, NULL, 0, NULL}
};
static PyObject* f_SetBackgroundColor(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"color", NULL};
    int r=0,g=0,b=0;
    PyObject*tag;
    PyObject*color;
    
    tag = tag_new(&bgcolor_tag);
    tag_internals_t*itag = tag_getinternals(tag);

    /* 1st try- copy constructor */
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", kwlist, &ColorClass, &color)) {
	PyErr_Clear();
	/* 2nd try- color's contructor */
	color = f_Color(NULL, args, kwargs);
    }
    if(!color)
	return NULL;

    itag->tag = swf_InsertTag(0, ST_SETBACKGROUNDCOLOR);
    RGBA rgba = color_getRGBA(color);
    swf_SetU8(itag->tag, rgba.r);
    swf_SetU8(itag->tag, rgba.g);
    swf_SetU8(itag->tag, rgba.b);
    mylog(" %08x(%d) SetBackgroundColor(%02x,%02x,%02x) (colorobj=%08x(%d))\n", (int)tag, tag->ob_refcnt, rgba.r, rgba.g, rgba.b, color, color->ob_refcnt);
    Py_DECREF(color);
    return (PyObject*)tag;
}
static tag_internals_t bgcolor_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: setbgcolor_methods,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t protect_tag;
static PyObject* f_Protect(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"password", NULL};
    char*password = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &password))
	return NULL;

    PyObject*tag = tag_new(&protect_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    itag->tag = swf_InsertTag(0, ST_PROTECT);
    if(password) {
	swf_SetPassword(itag->tag, password);
    }
    mylog("+%08x(%d) f_Protect", (int)tag, tag->ob_refcnt);
    return (PyObject*)tag;
}
static tag_internals_t protect_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t showframe_tag;
static PyObject* f_ShowFrame(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"name", NULL};
    char*name= 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &name))
	return NULL;

    PyObject*tag = tag_new(&showframe_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    itag->tag = swf_InsertTag(0, ST_SHOWFRAME);
    mylog("+%08x(%d) f_ShowFrame", (int)tag, tag->ob_refcnt);
    return (PyObject*)tag;
}
static tag_internals_t showframe_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t sprite_tag;
typedef struct _sprite_internal
{
    PyObject* taglist;
} sprite_internal_t;
    
static int sprite_fillTAG(tag_internals_t*self)
{
    mylog("+%08x(?) sprite_fillTAG", (int)self);

    sprite_internal_t*si = (sprite_internal_t*)self->data;

    TAG*sprite = swf_InsertTag(0, ST_DEFINESPRITE);
    swf_SetU16(sprite, 0); //id
    swf_SetU16(sprite, 0); //frames

    TAG*tag = taglist_getTAGs2(si->taglist, self->tagmap, 0);
    if(!tag) {
	/* pass through exception */
	return 0;
    }
    TAG*tag2 = tag;
    while(tag2->next) tag2 = tag2->next;
    swf_InsertTag(tag2, ST_END);

    sprite->next = tag;
    tag->prev = sprite;

    swf_FoldSprite(sprite);
    self->tag = sprite;
    return 1;
}

static PyObject* f_Sprite(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"name", NULL};
    char*name= 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &name))
	return NULL;

    PyObject*tag = tag_new(&sprite_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    sprite_internal_t*si = (sprite_internal_t*)itag->data;
    si->taglist = taglist_new();
    mylog("+%08x(%d) f_DefineSprite", (int)tag, tag->ob_refcnt);
    return (PyObject*)tag;
}
static PyObject* sprite_getattr(tag_internals_t*self,char*a)
{
    sprite_internal_t*si = (sprite_internal_t*)self->data;
    if(!strcmp(a, "tags")) {
	Py_INCREF(si->taglist);
	return si->taglist;
    }
    return 0;
}
static int sprite_setattr(tag_internals_t*self,char*a, PyObject*obj)
{
    sprite_internal_t*si = (sprite_internal_t*)self->data;
    if(self->tag) {
	swf_DeleteTag(self->tag);
	self->tag = 0;
    }
    if(!strcmp(a, "tags")) {
	PY_ASSERT_TYPE(obj,&TagListClass);
	Py_DECREF(si->taglist);
	si->taglist = obj;
	Py_INCREF(si->taglist);
	return 0;
    }
    return 1;
}
static tag_internals_t sprite_tag =
{
    parse: 0,
    fillTAG: sprite_fillTAG,
    dealloc: 0,
    getattr: sprite_getattr, 
    setattr: sprite_setattr,
    tagfunctions: 0,
    datasize: sizeof(sprite_internal_t),
};
//----------------------------------------------------------------------------
staticforward tag_internals_t end_tag;
static tag_internals_t end_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t text_tag;

typedef struct _text_internal
{
    char*text;
    SWFFONT* swffont;
    RGBA rgba;
    int size;
} text_internal_t;
staticforward tag_internals_t placeobject_tag;

static int text_fillTAG(tag_internals_t*self)
{
    text_internal_t*ti = (text_internal_t*)self->data;
    self->tag= swf_InsertTag(0, ST_DEFINETEXT2);
    swf_SetU16(self->tag, /*ID*/0);
    SRECT r = swf_SetDefineText(self->tag, ti->swffont, &ti->rgba, ti->text, ti->size);
    return 1;
}
static PyObject* f_DefineText(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"font", "text", "size", "color", NULL};
    PyObject*tag = 0;
    PyObject*otext;
    char*text = 0;
    int size = 0;
    RGBA rgba = {255,0,0,0};
    PyObject*color = 0;
    PyObject*font = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!Oi|O!", kwlist, &TagClass, &font, &otext, &size, &ColorClass, &color))
	return NULL;
    text = PyString_AS_STRING(PyUnicode_AsUTF8String(otext));

    if(color)
	rgba = color_getRGBA(color);

    mylog("DefineText: text = %s", text);
    
    tag = tag_new(&text_tag);
    tag_internals_t* itag = tag_getinternals(tag);
    text_internal_t*ti = (text_internal_t*)itag->data;

    ti->swffont = font_getSWFFONT(font);
    int font_id = tagmap_add(itag->tagmap, font); // add dependency on font
    ti->swffont->id = font_id; // for swf_SetDefineTexts
    ti->text = strdup(text);
    ti->rgba = rgba;
    ti->size = size;

    return (PyObject*)tag;
}
static tag_internals_t text_tag =
{
    parse: 0,
    fillTAG: text_fillTAG,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: sizeof(text_internal_t),
};
//----------------------------------------------------------------------------
staticforward tag_internals_t image_tag;

typedef struct _image_internal
{
    RGBA*rgba;
    int size;
    int width;
    int height;
    int bpp;
    char isindexed;
    char islossless;
} image_internal_t;
staticforward tag_internals_t image_tag;

static int image_fillTAG(tag_internals_t*self)
{
    image_internal_t*ti = (image_internal_t*)self->data;
    self->tag= swf_InsertTag(0, ST_DEFINEBITSLOSSLESS2);
    swf_SetU16(self->tag, /*ID*/0);
    swf_SetLosslessBits(self->tag, ti->width, ti->height, ti->rgba, BMF_32BIT);
    return 1;
}
static void image_dealloc(tag_internals_t*self)
{
    image_internal_t*pi = (image_internal_t*)self->data;
    if(pi->rgba) {
	free(pi->rgba);pi->rgba = 0;
    }
}
static int imagetag_getWidth(PyObject* self)
{
    tag_internals_t*itag = tag_getinternals(self);
    image_internal_t*pi = (image_internal_t*)itag->data;
    return pi->width;
}
static int imagetag_getHeight(PyObject* self)
{
    tag_internals_t*itag = tag_getinternals(self);
    image_internal_t*pi = (image_internal_t*)itag->data;
    return pi->height;
}
static PyObject* f_DefineImage(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"image"};
    PyObject*image = 0;
    PyObject*tag = tag_new(&image_tag);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", kwlist, &image))
	return NULL;
    
    tag = tag_new(&image_tag);
    tag_internals_t* itag = tag_getinternals(tag);
    image_internal_t*ti = (image_internal_t*)itag->data;

    ti->rgba = image_toRGBA(image);
    if(!ti->rgba) // pass through exception
	return 0;
    ti->width = image_getWidth(image);
    ti->height = image_getHeight(image);
    ti->isindexed = 0;
    ti->islossless = 1;
    ti->bpp = 32;
    ti->size = ti->width*ti->height;

    return (PyObject*)tag;
}
static tag_internals_t image_tag =
{
    parse: 0,
    fillTAG: image_fillTAG,
    dealloc: image_dealloc,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: sizeof(image_internal_t),
};
//----------------------------------------------------------------------------
staticforward tag_internals_t shape_tag;

typedef struct _shape_internal
{
    SHAPE2*shape;
} shape_internal_t;
staticforward tag_internals_t shape_tag;

static int shape_fillTAG(tag_internals_t*self)
{
    shape_internal_t*ti = (shape_internal_t*)self->data;
    self->tag= swf_InsertTag(0, ST_DEFINESHAPE3);
    swf_SetU16(self->tag, /*ID*/0);
    swf_SetShape2(self->tag, ti->shape);
    return 1;
}
static void shape_dealloc(tag_internals_t*self)
{
    shape_internal_t*pi = (shape_internal_t*)self->data;
    if(pi->shape) {
	swf_Shape2Free(pi->shape);
	pi->shape = 0;
    }
}
static PyObject* f_DefineImageShape(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"image"};
    PyObject*shape = 0;
    PyObject*tag = tag_new(&shape_tag);
    PyObject*image = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", kwlist, &TagClass, &image))
	return NULL;
    
    tag = tag_new(&shape_tag);
    tag_internals_t* itag = tag_getinternals(tag);
    shape_internal_t*ti = (shape_internal_t*)itag->data;
    ti->shape = 0; /*HACK*/

    int width = imagetag_getWidth(image);
    int height = imagetag_getHeight(image);
    int id = tagmap_add(itag->tagmap, image);
    itag->tag= swf_InsertTag(0, ST_DEFINESHAPE3);
    swf_SetU16(itag->tag, 0);
    swf_ShapeSetBitmapRect(itag->tag, id, width, height);
    return (PyObject*)tag;
}
static tag_internals_t shape_tag =
{
    parse: 0,
    fillTAG: shape_fillTAG,
    dealloc: shape_dealloc,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: sizeof(shape_internal_t),
};
//----------------------------------------------------------------------------

typedef struct _videostream_internal
{
    VIDEOSTREAM* stream;
    int lastiframe;
} videostream_internal_t;
staticforward tag_internals_t videostream_tag;
staticforward tag_internals_t videoframe_tag;

static int videostream_parse(tag_internals_t*self)
{
    videostream_internal_t*videostream = (videostream_internal_t*)self->data;
    /* TODO */
    PyErr_SetString(PyExc_Exception, setError("videostream parsing not implemented yet"));
    return 0;
}
static void videostream_dealloc(tag_internals_t*self)
{
    videostream_internal_t*videostream = (videostream_internal_t*)self->data;
    if(videostream->stream) {
	swf_VideoStreamClear(videostream->stream);
	free(videostream->stream);
	videostream->stream = 0;
    }
}
static int videostream_fillTAG(tag_internals_t*self)
{
    videostream_internal_t*fi = (videostream_internal_t*)self->data;
    if(self->tag)
	return 1;
    PyErr_SetString(PyExc_Exception, setError("videostream filling not implemented"));
    return 0;
}
static PyObject* f_DefineVideoStream(PyObject* self, PyObject* args, PyObject* kwargs)
{
    PyObject*tag = tag_new(&videostream_tag);
   
    int width=0,height=0,frames=65535;
    static char *kwlist[] = {"width", "height", "frames", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|i", kwlist, &width, &height, &frames))
	return NULL;

    printf(": %d %d\n", width, height);
    
    tag_internals_t*itag = tag_getinternals(tag);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    fi->stream = malloc(sizeof(VIDEOSTREAM));
    memset(fi->stream, 0, sizeof(VIDEOSTREAM));

    TAG*t = swf_InsertTag(0, ST_DEFINEVIDEOSTREAM);
    swf_SetU16(t, 0); /* id */
    swf_SetVideoStreamDefine(t, fi->stream, frames, width, height);
    itag->tag = t;
    fi->lastiframe = -65536;
    return (PyObject*)tag;
}
static VIDEOSTREAM* videostream_getVIDEOSTREAM(PyObject*self)
{
    PY_ASSERT_TYPE(self, &TagClass);
    tag_internals_t*itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    return fi->stream;
}
static PyObject* videostream_getbwidth(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    int width = fi->stream->bbx;
    return Py_BuildValue("i", width);
}
static PyObject* videostream_getbheight(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    int height = fi->stream->bby;
    return Py_BuildValue("i", height);
}
static PyObject* videostream_addFrame(PyObject*self, PyObject*args, PyObject*kwargs)
{
    tag_internals_t*_itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)_itag->data;
   
    PyObject*image = 0;
    char*type=0; // none, "i", "p"
    int quant=7;
    static char *kwlist[] = {"image", "quant", "type", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|is", kwlist, &image, &quant, &type))
	return NULL;
    if(fi->stream->width != image_getWidth(image)) {
	PyErr_SetString(PyExc_Exception, setError("bad image width %d!=%d", image_getWidth(image), fi->stream->width));return 0;
    }
    if(fi->stream->height != image_getHeight(image)) {
	PyErr_SetString(PyExc_Exception, setError("bad image width %d!=%d", image_getHeight(image), fi->stream->height));return 0;
    }
    PyObject*tag = tag_new(&videoframe_tag);
    tag_internals_t*itag = tag_getinternals(tag);

    RGBA*pic = image_toRGBA(image);
    if(!pic)
	return 0;

    
{  int f,j=0,i=0,rr,gg,bb;
   FILE *o;
   RGBA*it = pic;
   char*filename="test.ppm";
   printf("Creating %s %dx%d\n",filename, 512,512);
   o=fopen(filename, "wb");
   fprintf(o,"P6\n%d %d\n255\n",512, 512);
   
   while(j<512*512) {
    rr=it->r;
    gg=it->g;
    bb=it->b;
    fprintf(o,"%c%c%c",rr,gg,bb);
    it++;
    j++;
   }
   fclose(o);
}

    TAG* t = swf_InsertTag(0, ST_VIDEOFRAME);
    if((type && (type[0]=='I' || type[0]=='i')) || (type==0 && fi->lastiframe+64 < fi->stream->frame)) {
	swf_SetU16(t,0); /* id */
	swf_SetVideoStreamIFrame(t, fi->stream, pic, quant);
	fi->lastiframe = fi->stream->frame;
    } else {
	swf_SetU16(t,0);
	swf_SetVideoStreamPFrame(t, fi->stream, pic, quant);
    }
    itag->tag = t;
    tagmap_addMapping(itag->tagmap, 0, self);
    free(pic);
    return tag;
}
static PyObject* videostream_addDistortionFrame(PyObject*self, PyObject*args, PyObject*kwargs)
{
    tag_internals_t*_itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)_itag->data;
    
    static char *kwlist[] = {"image", "quant", NULL};
    int quant=7;
    PyObject* array = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|i", kwlist, &array, &quant))
	return NULL;

    signed char* movex = malloc(fi->stream->bbx * fi->stream->bby);
    signed char* movey = malloc(fi->stream->bbx * fi->stream->bby);
    signed char* itx=movex;
    signed char* ity=movey;
    int x,y;
    if(!array || !PySequence_Check(array))
	return PY_ERROR("Not an array");
    if(PySequence_Length(array) < fi->stream->bby)
	return PY_ERROR("Array (y) has to have at least %d elements, but has only %d ", fi->stream->bby, PySequence_Length(array));
    for(y=0;y<fi->stream->bby;y++) {
	PyObject*line = PySequence_GetItem(array, y);
	if(!line || !PySequence_Check(line))
	    return PY_ERROR("Not an array of arrays");
	if(PySequence_Length(line) < fi->stream->bbx)
	    return PY_ERROR("Inner arrays (x) have to be at least %d long- %dth is only %d", fi->stream->bbx, y, PySequence_Length(line));

	for(x=0;x<fi->stream->bbx;x++) {
	    PyObject*pixel = PySequence_GetItem(line, x);
	    if(!pixel) {
		*itx = 0;
		*ity = 0;
	    } else {
		if(!PyComplex_Check(pixel)) {
		    return PY_ERROR("Not an array of arrays of complex numbers");
		}
		*itx = (signed char)PyComplex_RealAsDouble(pixel);
		*ity = (signed char)PyComplex_ImagAsDouble(pixel);
	    }
	    itx++;
	    ity++;
	}
    }
    
    PyObject*tag = tag_new(&videoframe_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    
    TAG* t = swf_InsertTag(0, ST_VIDEOFRAME);
    swf_SetU16(t,0); /* id */
    swf_SetVideoStreamMover(t, fi->stream, movex, movey, quant);

    itag->tag = t;
    tagmap_addMapping(itag->tagmap, 0, self);

    free(movex);
    free(movey);

    return tag;
}
static PyMethodDef videostream_methods[] = 
{{"xblocks", videostream_getbwidth, METH_VARARGS, "get's the number of horizontal blocks"},
 {"yblocks", videostream_getbheight, METH_VARARGS, "get's the number of vertical blocks"},
 {"addFrame", (PyCFunction)videostream_addFrame, METH_KEYWORDS, "add a Video Frame"},
 {"addDistortionFrame", (PyCFunction)videostream_addDistortionFrame, METH_KEYWORDS, "add a MVD frame"},
 {NULL, NULL, 0, NULL}
};

static tag_internals_t videostream_tag =
{
    parse: videostream_parse,
    fillTAG: videostream_fillTAG,
    dealloc: videostream_dealloc,
    getattr: 0, 
    setattr: 0,
    tagfunctions: videostream_methods,
    datasize: sizeof(videostream_internal_t),
};

//============================================================================

static tag_internals_t videoframe_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: 0
};

//============================================================================

static PyMethodDef TagMethods[] = 
{
    /* TAGS */
    {"BackgroundColor", (PyCFunction)f_SetBackgroundColor, METH_KEYWORDS, "Create a SetBackGroundColor Tag."},
    {"Protect", (PyCFunction)f_Protect, METH_KEYWORDS, "Create a Protect Tag."},
    {"Font", (PyCFunction)f_DefineFont, METH_KEYWORDS, "Create a DefineFont Tag."},
    {"Text", (PyCFunction)f_DefineText, METH_KEYWORDS, "Create a DefineText Tag."},
    {"PlaceObject", (PyCFunction)f_PlaceObject, METH_KEYWORDS, "Create a PlaceObject Tag."},
    {"MoveObject", (PyCFunction)f_MoveObject, METH_KEYWORDS, "Create a PlaceObject Move Tag."},
    {"VideoStream", (PyCFunction)f_DefineVideoStream, METH_KEYWORDS, "Create a Videostream."},
    {"Image", (PyCFunction)f_DefineImage, METH_KEYWORDS, "Create an SWF Image Tag."},
    {"ImageShape", (PyCFunction)f_DefineImageShape, METH_KEYWORDS, "Create an SWF Image Shape Tag."},
    {"ShowFrame", (PyCFunction)f_ShowFrame, METH_KEYWORDS, "Create an SWF Show Frame Tag."},
    {"Sprite", (PyCFunction)f_Sprite, METH_KEYWORDS, "Create an SWF Sprite Tag."},
    {NULL, NULL, 0, NULL}
};
PyMethodDef* tags_getMethods()
{
    TagClass.ob_type = &PyType_Type;
    
    register_tag(ST_PLACEOBJECT,&placeobject_tag);
    register_tag(ST_PLACEOBJECT2,&placeobject_tag);
    register_tag(ST_SETBACKGROUNDCOLOR,&bgcolor_tag);
    register_tag(ST_DEFINEFONT,&font_tag);
    register_tag(ST_PROTECT,&protect_tag);
    register_tag(ST_DEFINETEXT,&text_tag);
    register_tag(ST_DEFINEBITSJPEG,&image_tag);
    register_tag(ST_DEFINEBITSJPEG2,&image_tag);
    register_tag(ST_DEFINEBITSJPEG3,&image_tag);
    register_tag(ST_DEFINEBITSLOSSLESS,&image_tag);
    register_tag(ST_DEFINEBITSLOSSLESS2,&image_tag);
    register_tag(ST_SHOWFRAME,&showframe_tag);
    register_tag(ST_DEFINEVIDEOSTREAM,&videostream_tag);
    register_tag(ST_VIDEOFRAME,&videoframe_tag);
    register_tag(ST_DEFINESPRITE,&sprite_tag);
    register_tag(ST_END,&end_tag);

    return TagMethods;
}
