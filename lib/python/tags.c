#include "pyutils.h"
#include "primitives.h"
#include "action.h"
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
static PyObject* f_PlaceObject(PyObject* self, PyObject* args, PyObject* kwargs)
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
    po->id = /*ID*/ 0;
    po->clipdepth = clipdepth;
    po->ratio = ratio;
    po->name = name;
    if(clipdepth) po->clipdepth = clipdepth;
    if(matrix) po->matrix = matrix_getMatrix(matrix);
    if(cxform) po->cxform = colortransform_getCXForm(cxform);
    if(action) po->actions = action_getAction(action);

    PyObject*tag;
    tag = tag_new(&placeobject_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    placeobject_internal_t*pi = (placeobject_internal_t*)itag->data;
    pi->po = po;
    pi->po->id = tagmap_add(itag->tagmap,(PyObject*)character);
    
    mylog("+%08x(%d) PlaceObject %08x(%d)\n", (int)tag, tag->ob_refcnt, character, character->ob_refcnt);

    return (PyObject*)tag;
}
static tag_internals_t placeobject_tag =
{
    parse: po_parse,
    fillTAG: po_fillTAG,
    dealloc: po_dealloc,
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
    tagfunctions: 0,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t end_tag;
static tag_internals_t end_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
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
    char*text = 0;
    int textlen = 0;
    PyObject*unicode16;
    PyObject*unicode8;
    int size = 0;
    RGBA rgba = {255,0,0,0};
    PyObject*color = 0;
    PyObject*font = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!u#i|O!", kwlist, &TagClass, &font, &text, &textlen, &size, &ColorClass, &color))
	return NULL;
    
    unicode16 = PyUnicode_DecodeUTF16(text, textlen*2, NULL, NULL);
    unicode8 = PyUnicode_AsUTF8String(unicode16);
    text = PyString_AS_STRING(unicode8);

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
    /* TODO */
    PyErr_SetString(PyExc_Exception, setError("videostream filling not implemented yet"));return 0;
    return 1;
}
static PyObject* f_DefineVideoStream(PyObject* self, PyObject* args, PyObject* kwargs)
{
    PyObject*tag = tag_new(&videostream_tag);
   
    int width=0,height=0,frames=65535;
    static char *kwlist[] = {"width", "height", "frames", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|i", kwlist, &TagClass, &width, &height, &frames))
	return NULL;
    
    tag_internals_t*itag = tag_getinternals(tag);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    fi->stream = malloc(sizeof(VIDEOSTREAM));
    memset(fi->stream, 0, sizeof(VIDEOSTREAM));

    TAG*t = swf_InsertTag(0, ST_DEFINEVIDEOSTREAM);
    swf_SetVideoStreamDefine(t, fi->stream, frames, width, height);
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
    TAG* t = swf_InsertTag(0, ST_VIDEOFRAME);
    if((type && (type[0]=='I' || type[0]=='i')) || (type==0 && fi->lastiframe+64 < fi->stream->frame)) {
	swf_SetU16(t,0);
	swf_SetVideoStreamIFrame(t, fi->stream, pic, quant);
	fi->lastiframe = fi->stream->frame;
    } else {
	swf_SetU16(t,0);
	swf_SetVideoStreamPFrame(t, fi->stream, pic, quant);
    }
    itag->tag = t;
    tagmap_addMapping(itag->tagmap, 0, self);
    free(pic);
    return Py_BuildValue("s", 0);
}
static PyObject* videostream_addDistortionFrame(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    
    /* TODO */
    return Py_BuildValue("s", 0);
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
    tagfunctions: videostream_methods,
    datasize: sizeof(videostream_internal_t),
};

//============================================================================

static tag_internals_t videoframe_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
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
    {"VideoStream", (PyCFunction)f_DefineVideoStream, METH_KEYWORDS, "Create a Videostream."},
    {"Image", (PyCFunction)f_DefineImage, METH_KEYWORDS, "Create an SWF Image Tag."},
    {"ImageShape", (PyCFunction)f_DefineImageShape, METH_KEYWORDS, "Create an SWF Image Shape Tag."},
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
    register_tag(ST_END,&end_tag);

    return TagMethods;
}

