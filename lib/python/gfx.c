/* gfx.c

   Python wrapper for gfx convert

   Part of the swftools package.

   Copyright (c) 2003 Matthias Kramm <kramm@quiss.org>
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include <Python.h>
#include <stdarg.h>
#include <setjmp.h>
#undef HAVE_STAT
#include "../../config.h"
#include "../gfxtools.h"
#include "../devices/swf.h"
#include "../devices/render.h"
#include "../devices/ocr.h"
#include "../devices/rescale.h"
#include "../devices/text.h"
#ifdef USE_OPENGL
#include "../devices/opengl.h"
#endif
#include "../pdf/pdf.h"
#include "../readers/swf.h"
#include "../readers/image.h"
#include "../log.h"
#include "../kdtree.h"
#include "../utf8.h"
#include "../gfxdevice.h"
#include "../gfximage.h"

#define PYTHON_GFX_VERSION VERSION

#if PY_MAJOR_VERSION >= 3
#define PYTHON3
#define M_FLAGS (METH_VARARGS|METH_KEYWORDS)
#else
#define M_FLAGS (METH_KEYWORDS)
#endif

typedef struct _state {
    gfxsource_t*pdfdriver;
    gfxsource_t*swfdriver;
    gfxsource_t*imagedriver;
} state_t;

#ifdef PYTHON3
#define STATE(m) ((state_t*)PyModule_GetState(m))
#else
static state_t global_state = {0,0,0};
#define STATE(m) &global_state;
#endif
 

static PyTypeObject OutputClass;
static PyTypeObject PageClass;
static PyTypeObject DocClass;
static PyTypeObject FontClass;
static PyTypeObject GlyphClass;
static PyTypeObject CharClass;
static PyTypeObject KDTreeClass;
static PyTypeObject BitmapClass;

typedef struct {
    PyObject_HEAD
    gfxdevice_t*output_device;
    PyObject*pyobj; //only for passthrough
} OutputObject;

typedef struct {
    PyObject_HEAD
    PyObject*parent;
    gfxpage_t*page;
    int nr;
} PageObject;

typedef struct {
    PyObject_HEAD
    gfxdocument_t*doc;
    char*filename;
    int page_pos;
} DocObject;

typedef struct {
    PyObject_HEAD
    gfxfont_t*gfxfont;
} FontObject;

typedef struct {
    PyObject_HEAD
    FontObject*font;
    int nr;
} GlyphObject;

typedef struct {
    PyObject_HEAD
    FontObject*font;
    int nr;
    gfxmatrix_t matrix;
    int size;
    gfxcolor_t color;
} CharObject;

typedef struct {
    PyObject_HEAD
    kdtree_t*kdtree;
} KDTreeObject;

typedef struct {
    PyObject_HEAD
    gfximage_t*image;
} BitmapObject;

static char* strf(char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    return strdup(buf);
}
static inline PyObject*pystring_fromstring(const char*s)
{
#ifdef PYTHON3
    return PyUnicode_FromString(s);
#else
    return PyString_FromString(s);
#endif
}
static inline int pystring_check(PyObject*o)
{
#ifdef PYTHON3
    return PyUnicode_Check(o);
#else
    return PyString_Check(o);
#endif
}
static inline PyObject*pyint_fromlong(long l)
{
#ifdef PYTHON3
    return PyLong_FromLong(l);
#else
    return PyInt_FromLong(l);
#endif
}
static inline const char*pystring_asstring(PyObject*s)
{
#ifdef PYTHON3
    return PyUnicode_AS_DATA(s);
#else
    return PyString_AsString(s);
#endif
}
PyObject*forward_getattr(PyObject*self, char *a)
{
    PyObject*o = pystring_fromstring(a);
    PyObject*ret = PyObject_GenericGetAttr(self, o);
    Py_DECREF(o);
    return ret;
}

#define PY_ERROR(s,args...) (PyErr_SetString(PyExc_Exception, strf(s, ## args)),(void*)NULL)
#define PY_NONE Py_BuildValue("s", 0)

//---------------------------------------------------------------------
PyDoc_STRVAR(output_save_doc, \
"save(filename)\n\n"
"Saves the contents of an output device to a file\n"
"Depending on what the output device is, the contents\n"
"of the file may be plain text, an image, an SWF file,\n"
"etc.\n"
"For the ImageList device, several files (named\n"
"filename.1.png, filename.2.png etc.) might be created)\n"
);
static PyObject* output_save(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    OutputObject* self = (OutputObject*)_self;
    char*filename = 0;
    static char *kwlist[] = {"filename", NULL};
    int ret;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filename))
	return NULL;

    Py_BEGIN_ALLOW_THREADS
    gfxresult_t*result = self->output_device->finish(self->output_device);
    self->output_device = 0;
    ret = result->save(result, filename);
    result->destroy(result);
    Py_END_ALLOW_THREADS
    if(ret < 0) {
	return PY_ERROR("Couldn't write to %s", filename);
    }
    return PY_NONE;
}

PyDoc_STRVAR(output_startpage_doc, \
"startpage(width, height)\n\n"
"Starts a new page/frame in the output device.\n"
"The usual way to render documents is to start a new page in the\n"
"device for each page in the document:\n"
"\n"
"for pagenr in range(1,doc.pages+1):\n"
"    page = doc.getPage(pagenr)\n"
"    output.startpage(page.width, page.height)\n"
"    page.render(output)\n"
"    output.endpage()\n"
"\n"
"It is, however, also possible to render more than one document page\n"
"to a single output page. E.g. for side-by-side or book views.\n"
);
static PyObject* output_startpage(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    OutputObject* self = (OutputObject*)_self;
    int width=0, height=0;
    if (!PyArg_ParseTuple(args, "ii", &width, &height))
	return NULL;
    Py_BEGIN_ALLOW_THREADS
    self->output_device->startpage(self->output_device, width, height);
    Py_END_ALLOW_THREADS
    return PY_NONE;
}

/* as the definition of the python image type comes from another module (not
   included here, reproduce the necessary structure and extract the image
   without using the type definition */
typedef struct {
    PyObject_HEAD
    gfximage_t*image;
    PyObject* strrepr;
} ImageObject;
static gfximage_t*toImage(PyObject*_bitmap)
{
    if(!_bitmap || !_bitmap->ob_type->tp_name || strcmp(_bitmap->ob_type->tp_name, "Image")) {
        PY_ERROR("Second argument to fillbitmap must be an image");
        return 0;
    }
    ImageObject*bitmap = (ImageObject*)_bitmap;
    return bitmap->image;
}

static gfxline_t*toLine(PyObject*_line)
{
    int t;
    int num = PyList_Size(_line);
    gfxline_t first;
    first.next = 0;
    gfxline_t*last=&first;
    for(t=0;t<num;t++) {
        PyObject*p= PySequence_GetItem(_line, t);
        if(!PyTuple_Check(p)) {
            return PY_ERROR("each point must be a tuple");
	}
        PyObject*_type = PyTuple_GetItem(p, 0);
        if(!pystring_check(_type))
            return PY_ERROR("point tuples must start with a string");
        const char*type = pystring_asstring(_type);
        int s;
        int size = PyTuple_Size(p);
        for(s=1;s<size;s++) {
            if(!PyFloat_Check(PyTuple_GetItem(p,s))) {
                return PY_ERROR("coordinates must be floats");
            }
        }
        gfxline_t*l = (gfxline_t*)malloc(sizeof(gfxline_t));
        memset(l, 0, sizeof(gfxline_t));
        last->next = l;
        last = l;
        if(type[0]=='m') {
            l->type = gfx_moveTo;
            if(size!=3)
                return PY_ERROR("need 2 values for move");
            l->x = PyFloat_AsDouble(PyTuple_GetItem(p, 1));
            l->y = PyFloat_AsDouble(PyTuple_GetItem(p, 2));
        } else if(type[0]=='l') {
            l->type = gfx_lineTo;
            if(size!=3)
                return PY_ERROR("need 2 values for line");
            l->x = PyFloat_AsDouble(PyTuple_GetItem(p, 1));
            l->y = PyFloat_AsDouble(PyTuple_GetItem(p, 2));
        } else if(type[0]=='s') {
            l->type = gfx_splineTo;
            if(size!=5)
                return PY_ERROR("need 4 values for spline");
            l->x = PyFloat_AsDouble(PyTuple_GetItem(p, 1));
            l->y = PyFloat_AsDouble(PyTuple_GetItem(p, 2));
            l->sx = PyFloat_AsDouble(PyTuple_GetItem(p, 3));
            l->sy = PyFloat_AsDouble(PyTuple_GetItem(p, 4));
        } else {
            return PY_ERROR("Unknown line code '%s'", type);
        }
    }
    return first.next;
}

PyDoc_STRVAR(output_fillbitmap_doc, \
"fillbitmap()\n\n"
"fill a polygon with a bitmap pattern\n"
);
static PyObject* output_fillbitmap(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    OutputObject* self = (OutputObject*)_self;
    PyObject*_line=0;
    PyObject*_bitmap=0;
    static char *kwlist[] = {"line", "bitmap", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O", kwlist, &PyList_Type, &_line, &_bitmap))
	return NULL;

    gfximage_t*image = toImage(_bitmap);
    if(!image)
        return PY_ERROR("invalid image");

    gfxline_t*line = toLine(_line);
    if(!line) 
        return 0;

    /* TODO */
    gfxmatrix_t m;
    memset(&m, 0, sizeof(gfxmatrix_t));
    m.m00 = m.m11 = 1.0;

    self->output_device->fillbitmap(self->output_device, line, image, &m, 0);
    gfxline_free(line);
    return PY_NONE;
}

PyDoc_STRVAR(output_fill_doc, \
"fill()\n\n"
"fill a polygon with a color\n"
);
static PyObject* output_fill(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    OutputObject* self = (OutputObject*)_self;
    PyObject*_line=0;
    PyObject*_bitmap=0;
    static char *kwlist[] = {"line", "color", NULL};

    PyObject* color=0;

    int a=255,r=0,g=0,b=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O", kwlist, &PyList_Type, &_line, &color))
	return NULL;

    if(!PyArg_ParseTuple(color, "iiii:color",  &a, &r, &g, &b)) {
        return NULL;
    }

    gfxcolor_t c;
    c.r = r; c.g = g; c.b = b; c.a = a;

    gfxline_t*line = toLine(_line);
    if(!line) 
        return 0;

    /* TODO */
    gfxmatrix_t m;
    memset(&m, 0, sizeof(gfxmatrix_t));
    m.m00 = m.m11 = 1.0;

    self->output_device->fill(self->output_device, line, &c);
    gfxline_free(line);
    return PY_NONE;
}

PyDoc_STRVAR(output_stroke_doc, \
"stroke()\n\n"
"stroke a polygon with a color\n"
);
static PyObject* output_stroke(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    OutputObject* self = (OutputObject*)_self;
    PyObject*_line=0;
    PyObject*_bitmap=0;
    static char *kwlist[] = {"line", "width", "color", NULL};

    PyObject* color=0;

    int a=255,r=0,g=0,b=0;
    float width = 1.0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!fO", kwlist, &PyList_Type, &_line, &width, &color))
	return NULL;

    if(!PyArg_ParseTuple(color, "iiii:color",  &a, &r, &g, &b)) {
        return NULL;
    }

    gfxcolor_t c;
    c.r = r; c.g = g; c.b = b; c.a = a;

    gfxline_t*line = toLine(_line);
    if(!line) 
        return 0;

    self->output_device->stroke(self->output_device, line, width, &c, 
            /*TODO*/ gfx_capRound, gfx_joinRound, 0.0);
    gfxline_free(line);
    return PY_NONE;
}

PyDoc_STRVAR(output_endpage_doc, \
"endpage()\n\n"
"Ends a page in the output device. This function should be called\n"
"once for every startpage()\n"
);
static PyObject* output_endpage(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    OutputObject* self = (OutputObject*)_self;
    if (!PyArg_ParseTuple(args, ""))
	return NULL;
    Py_BEGIN_ALLOW_THREADS
    self->output_device->endpage(self->output_device);
    Py_END_ALLOW_THREADS
    return PY_NONE;
}
PyDoc_STRVAR(output_setparameter_doc, \
"setparameter(key, value)\n\n"
"Set a output-device dependent parameter"
);
static PyObject* output_setparameter(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    OutputObject* self = (OutputObject*)_self;
    static char *kwlist[] = {"key", "value", NULL};
    char*key=0,*value=0;
    if (args && !PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwlist, &key, &value))
	return NULL;
    self->output_device->setparameter(self->output_device, key, value);
    return PY_NONE;
}
PyDoc_STRVAR(f_createSWF_doc, \
"SWF()\n\n"
"Creates a device which renders documents to SWF (Flash) files.\n"
"Depending on the way the document parser behaves (see the poly2bitmap\n"
"and bitmap parameters), the resulting SWF might use vector operations\n"
"and Flash Texts to display the document, or just a single bitmap.\n"
);
static PyObject* f_createSWF(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {NULL};
    if (args && !PyArg_ParseTupleAndKeywords(args, kwargs, "", kwlist))
	return NULL;
    OutputObject*self = PyObject_New(OutputObject, &OutputClass);
    
    self->output_device = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    gfxdevice_swf_init(self->output_device);
    return (PyObject*)self;
}

PyDoc_STRVAR(f_createOCR_doc, \
"OCR()\n\n"
"Creates a device which processes documents using OCR (optical\n"
"character recognition).\n"
"This is handy for e.g. extracting fulltext from PDF documents\n"
"which have broken fonts, and where hence the \"PlainText\"\n"
"device doesn't work.\n"
);
static PyObject* f_createOCR(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {NULL};
    if (args && !PyArg_ParseTupleAndKeywords(args, kwargs, "", kwlist))
	return NULL;
    OutputObject*self = PyObject_New(OutputObject, &OutputClass);
    
    self->output_device = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    gfxdevice_ocr_init(self->output_device);
    return (PyObject*)self;
}


PyDoc_STRVAR(f_createImageList_doc, \
"ImageList()\n\n"
"Creates a device which renders documents to bitmaps.\n"
"Each page that is rendered will create new bitmap.\n"
"Using save(), you can save the images to a number\n"
"of files\n"
);
static PyObject* f_createImageList(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "", kwlist))
	return NULL;
    OutputObject*self = PyObject_New(OutputObject, &OutputClass);
    
    self->output_device = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    gfxdevice_render_init(self->output_device);
    return (PyObject*)self;
}

PyDoc_STRVAR(f_createPlainText_doc, \
"PlainText()\n\n"
"Creates a device which can be used to extract text from documents,\n"
"by passing it as parameter to page.render().\n"
"The extracted text can be saved by plaintext.save(filename).\n"
);
static PyObject* f_createPlainText(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "", kwlist))
	return NULL;
    OutputObject*self = PyObject_New(OutputObject, &OutputClass);
    
    self->output_device = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    gfxdevice_text_init(self->output_device);
    return (PyObject*)self;
}

#ifdef USE_OPENGL
PyDoc_STRVAR(f_createOpenGL_doc, \
"OpenGL()\n\n"
"Creates a device which renders everything to OpenGL.\n"
"Can be used for desktop display and debugging.\n"
"This device is not available on all systems.\n"
);
static PyObject* f_createOpenGL(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "", kwlist))
	return NULL;
    OutputObject*self = PyObject_New(OutputObject, &OutputClass);
    
    self->output_device = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    gfxdevice_opengl_init(self->output_device);
    return (PyObject*)self;
}
#endif

static jmp_buf backjump;
static int has_backjump = 0;

static PyObject* convert_gfxline(gfxline_t*line)
{
    gfxline_t*l;
    int len = 0, i = 0;
    l = line;
    while(l) {l=l->next;len++;}
    PyObject*list = PyList_New(len);
    l = line;
    while(l) {
	PyObject*point=0;
	if(l->type == gfx_moveTo) {
	    point = PyTuple_New(3);
	    PyTuple_SetItem(point, 0, pystring_fromstring("m"));
	    PyTuple_SetItem(point, 1, PyFloat_FromDouble(l->x));
	    PyTuple_SetItem(point, 2, PyFloat_FromDouble(l->y));
	} else if(l->type == gfx_lineTo) {
	    point = PyTuple_New(3);
	    PyTuple_SetItem(point, 0, pystring_fromstring("l"));
	    PyTuple_SetItem(point, 1, PyFloat_FromDouble(l->x));
	    PyTuple_SetItem(point, 2, PyFloat_FromDouble(l->y));
	} else if(l->type == gfx_splineTo) {
	    point = PyTuple_New(5);
	    PyTuple_SetItem(point, 0, pystring_fromstring("s"));
	    PyTuple_SetItem(point, 1, PyFloat_FromDouble(l->x));
	    PyTuple_SetItem(point, 2, PyFloat_FromDouble(l->y));
	    PyTuple_SetItem(point, 3, PyFloat_FromDouble(l->sx));
	    PyTuple_SetItem(point, 4, PyFloat_FromDouble(l->sy));
	} else {
	    point = PY_NONE;
	}
	PyList_SetItem(list, i, point);
	l = l->next;
	i++;
    }
    return list;
}
static PyObject* convert_matrix(gfxmatrix_t*m)
{
    PyObject*columns = PyTuple_New(3);
    PyObject*column0 = PyTuple_New(2);
    PyTuple_SetItem(column0, 0, PyFloat_FromDouble(m->m00));
    PyTuple_SetItem(column0, 1, PyFloat_FromDouble(m->m10));
    PyTuple_SetItem(columns, 0, column0);
    PyObject*column1 = PyTuple_New(2);
    PyTuple_SetItem(column1, 0, PyFloat_FromDouble(m->m01));
    PyTuple_SetItem(column1, 1, PyFloat_FromDouble(m->m11));
    PyTuple_SetItem(columns, 1, column1);
    PyObject*column2 = PyTuple_New(2);
    PyTuple_SetItem(column2, 0, PyFloat_FromDouble(m->tx));
    PyTuple_SetItem(column2, 1, PyFloat_FromDouble(m->ty));
    PyTuple_SetItem(columns, 2, column2);
    return columns;
}
static PyObject* convert_color(gfxcolor_t*col)
{
    PyObject*obj = PyTuple_New(4);
    PyTuple_SetItem(obj, 0, pyint_fromlong(col->a));
    PyTuple_SetItem(obj, 1, pyint_fromlong(col->r));
    PyTuple_SetItem(obj, 2, pyint_fromlong(col->g));
    PyTuple_SetItem(obj, 3, pyint_fromlong(col->b));
    return obj;
}

static PyObject* lookup_font(gfxfont_t*font);
static PyObject* char_new(gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix);
static PyObject* create_bitmap(gfximage_t*img);

static gfxfontlist_t* global_fonts;
static char callback_python(char*function, gfxdevice_t*dev, const char*format, ...)
{
    OutputObject*self = (OutputObject*)dev->internal;

    if(!PyObject_HasAttrString(self->pyobj, function))
        return 0;

    va_list ap;
    va_start(ap, format);

    PyObject*tuple = PyTuple_New(strlen(format));
    int pos = 0;
    while(format[pos]) {
        char p = format[pos];
	PyObject*obj = 0;
	switch(p) {
	    case 's': {
		char*s = va_arg(ap, char*);
		obj = pystring_fromstring(s);
		break;
	    }
	    case 'i': {
		int i = va_arg(ap, int);
		obj = pyint_fromlong(i);
		break;
	    }
	    case 'd': {
		int i = va_arg(ap, double);
		obj = PyFloat_FromDouble(i);
		break;
	    }
	    case 'c': {
		void* ptr = va_arg(ap, void*);
		gfxcolor_t*col = (gfxcolor_t*)ptr;
		obj = convert_color(col);
		break;
	    }
	    case 'f': {
		void* ptr = va_arg(ap, void*);
		obj = lookup_font((gfxfont_t*)ptr);
		break;
	    }
	    case 'l': {
		void* ptr = va_arg(ap, void*);
		gfxline_t*line = (gfxline_t*)ptr;
		obj = convert_gfxline(line);
		break;
	    }
	    case 'I': {
		void* ptr = va_arg(ap, void*);
		obj = create_bitmap((gfximage_t*)ptr);
		break;
	    }
	    case 'O': {
		void* ptr = va_arg(ap, void*);
		obj = (PyObject*)ptr;
		break;
	    }
	    case 'm': {
		void* ptr = va_arg(ap, void*);
		obj = convert_matrix((gfxmatrix_t*)ptr);
		break;
	    }
	    default: {
		obj = PY_NONE;
		break;
	    }
	}
        PyTuple_SetItem(tuple, pos, obj);
        pos++;
    }
    va_end(ap);
    PyObject*f = PyObject_GetAttrString(self->pyobj, function);
    if(!f)
        return 0;
    PyErr_Clear();
    PyObject* result = PyObject_CallObject(f, tuple);
    Py_DECREF(tuple);

    if(!result) { 
	if(!has_backjump) {
	    PyErr_Print();
	    PyErr_Clear();
	    return 1;
	} else {
	    longjmp(backjump, 1);
	}
    } else {
        Py_DECREF(result);
        return 1;
    }
}
    
static int my_setparameter(gfxdevice_t*dev, const char*key, const char*value)
{
    callback_python("setparameter", dev, "ss", key, value);
    return 1;
}
static void my_startpage(gfxdevice_t*dev, int width, int height)
{
    callback_python("startpage", dev, "ii", width, height);
}
static void my_startclip(gfxdevice_t*dev, gfxline_t*line)
{
    callback_python("startclip", dev, "l", line);
}
static void my_endclip(gfxdevice_t*dev)
{
    callback_python("endclip", dev, "");
}
static void my_stroke(gfxdevice_t*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    char*cap = 0;
    char*joint = 0;
    if(cap_style == gfx_capButt)
        cap = "butt";
    else if(cap_style == gfx_capRound)
        cap = "round";
    else if(cap_style == gfx_capSquare)
        cap = "square";
    if(joint_style == gfx_joinMiter)
        joint = "miter";
    else if(joint_style == gfx_joinRound)
        joint = "round";
    else if(joint_style == gfx_joinBevel)
        joint = "bevel";
    callback_python("stroke", dev, "ldcssi", line, width, color, cap, joint, miterLimit);
}
static void my_fill(gfxdevice_t*dev, gfxline_t*line, gfxcolor_t*color)
{
    callback_python("fill", dev, "lc", line, color);
}
static void my_fillbitmap(gfxdevice_t*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*imgcoord2devcoord, gfxcxform_t*cxform)
{
    callback_python("fillbitmap", dev, "lImx", line, img, imgcoord2devcoord, cxform);
}
static void my_fillgradient(gfxdevice_t*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    callback_python("fillgradient", dev, "lgsm", line, gradient, type, matrix);
}
static void my_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    callback_python("addfont", dev, "f", font);
}
static void my_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    PyMethodObject*obj = (PyMethodObject*)PyObject_GetAttrString(((OutputObject*)(dev->internal))->pyobj, "drawchar");
    if(!obj) {
	/* if the device doesn't support chars, try drawing a polygon instead */
	if(!font)
	    return;
	gfxglyph_t*glyph = &font->glyphs[glyphnr];
	gfxline_t*line2 = gfxline_clone(glyph->line);
	gfxline_transform(line2, matrix);
	my_fill(dev, line2, color);
	gfxline_free(line2);
	return;
    }
    PyFunctionObject*f = (PyFunctionObject*)obj->im_func;
    PyCodeObject*c = (PyCodeObject*)f->func_code;
    if(c->co_argcount == 2) {
	/* new style drawchar method */
	CharObject*chr = (CharObject*)char_new(font, glyphnr, color, matrix);
	callback_python("drawchar", dev, "O", chr);
    } else {
	callback_python("drawchar", dev, "ficm", font, glyphnr, color, matrix);
    }
}
static void my_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action, const char*text)
{
    callback_python("drawlink", dev, "lss", line, action, text);
}
static void my_endpage(gfxdevice_t*dev)
{
    callback_python("endpage", dev, "");
}
static gfxresult_t* my_finish(gfxdevice_t*dev)
{
    callback_python("finish", dev, "");
    return 0;
}

PyObject* passthrough_create(PyObject*obj)
{
    OutputObject*self = PyObject_New(OutputObject, &OutputClass);
    self->pyobj = obj;
    Py_INCREF(obj);
    self->output_device = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
    memset(self->output_device, 0, sizeof(gfxdevice_t));
    self->output_device->name = strdup("passthrough");

    self->output_device->setparameter = my_setparameter;
    self->output_device->startpage = my_startpage;
    self->output_device->startclip = my_startclip;
    self->output_device->addfont = my_addfont;
    self->output_device->endclip = my_endclip;
    self->output_device->stroke = my_stroke;
    self->output_device->fill = my_fill;
    self->output_device->fillbitmap = my_fillbitmap;
    self->output_device->fillgradient = my_fillgradient;
    self->output_device->drawchar = my_drawchar;
    self->output_device->drawlink = my_drawlink;
    self->output_device->endpage = my_endpage;
    self->output_device->finish = my_finish;
    self->output_device->internal = self;

    return (PyObject*)self;
}

PyDoc_STRVAR(f_createPassThrough_doc, \
"PassThrough(device)\n\n"
"Creates a PassThrough device, which can be used as parameter in calls\n"
"to page.render().\n"
"device needs to be a class implementing at least the following functions:\n\n"
"setparameter(key,value)\n"
"startclip(outline)\n"
"endclip()\n"
"stroke(outline, width, color, capstyle, jointstyle, miterLimit)\n"
"fill(outline, color)\n"
"fillbitmap(outline, image, matrix, colortransform)\n"
"fillgradient(outline, gradient, gradienttype, matrix)\n"
"addfont(font)\n"
"drawchar(font, glyph, color, matrix)\n"
"drawlink(outline, url)\n"
"If any of these functions are not defined, a error message will be printed,\n"
"however the rendering process will *not* be aborted.\n"
);
static PyObject* f_createPassThrough(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"device", NULL};
    PyObject*obj;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", kwlist, &obj))
	return NULL;
    return passthrough_create(obj);
}

static PyMethodDef output_methods[] =
{
    /* Output functions */
    {"save", (PyCFunction)output_save, M_FLAGS, output_save_doc},
    {"startpage", (PyCFunction)output_startpage, M_FLAGS, output_startpage_doc},
    {"fill", (PyCFunction)output_fill, M_FLAGS, output_fill_doc},
    {"fillbitmap", (PyCFunction)output_fillbitmap, M_FLAGS, output_fillbitmap_doc},
    {"stroke", (PyCFunction)output_stroke, M_FLAGS, output_stroke_doc},
    {"endpage", (PyCFunction)output_endpage, M_FLAGS, output_endpage_doc},
    {"setparameter", (PyCFunction)output_setparameter, M_FLAGS, output_setparameter_doc},
    {0,0,0,0}
};

static void output_dealloc(PyObject* _self) {
    OutputObject* self = (OutputObject*)_self;

    if(self->output_device) {
        gfxresult_t*result = self->output_device->finish(self->output_device);
        if(result) {
	    result->destroy(result);result=0;
        }
        self->output_device = 0;
    }
    
    PyObject_Del(self);
}
static PyObject* output_getattr(PyObject * _self, char* a)
{
    OutputObject*self = (OutputObject*)_self;
   
/*    if(!strcmp(a, "x1")) {
        return pyint_fromlong(self->output_device->x1);
    } else if(!strcmp(a, "y1")) {
        return pyint_fromlong(self->output_device->y1);
    } else if(!strcmp(a, "x2")) {
        return pyint_fromlong(self->output_device->x2);
    } else if(!strcmp(a, "y2")) {
        return pyint_fromlong(self->output_device->y2);
    }*/
    
    return forward_getattr(_self, a);
}
static int output_setattr(PyObject * _self, char* a, PyObject * o) 
{
    OutputObject*self = (OutputObject*)_self;
    if(!pystring_check(o))
        return -1;
    const char*value = pystring_asstring(o);
    self->output_device->setparameter(self->output_device, a, value);
    return -1;
}
static int output_print(PyObject * _self, FILE *fi, int flags)
{
    OutputObject*self = (OutputObject*)_self;
    fprintf(fi, "<output object at %p(%d)>", _self, _self?_self->ob_refcnt:0);
    return 0;
}

//---------------------------------------------------------------------
static PyMethodDef glyph_methods[];

static void glyph_dealloc(PyObject* _self) {
    GlyphObject* self = (GlyphObject*)_self;
    Py_DECREF(self->font);
    PyObject_Del(self);
}
static PyObject* glyph_getattr(PyObject * _self, char* a)
{
    GlyphObject*self = (GlyphObject*)_self;
    FontObject*font = self->font;
    if(!strcmp(a, "unicode")) {
        return pyint_fromlong(font->gfxfont->glyphs[self->nr].unicode);
    } else if(!strcmp(a, "advance")) {
        return PyFloat_FromDouble(font->gfxfont->glyphs[self->nr].advance);
    } else if(!strcmp(a, "polygon")) {
        return convert_gfxline(font->gfxfont->glyphs[self->nr].line);
    }
    return forward_getattr(_self, a);
}
static int glyph_setattr(PyObject * self, char* a, PyObject * o) {
    return -1;
}
static PyObject* glyph_new(FontObject*font, int nr) {
    GlyphObject*glyph = PyObject_New(GlyphObject, &GlyphClass);
    glyph->font = font;
    Py_INCREF(glyph->font);
    glyph->nr = nr;
    return (PyObject*)glyph;
}
static int glyph_print(PyObject * _self, FILE *fi, int flags)
{
    GlyphObject*self = (GlyphObject*)_self;
    fprintf(fi, "<glyph object at %p(%d)>", _self, _self?_self->ob_refcnt:0);
    return 0;
}
static PyMethodDef glyph_methods[] =
{
    /* Glyph functions */
    {0,0,0,0}
};

//---------------------------------------------------------------------
static PyMethodDef font_methods[];

static void font_dealloc(PyObject* _self) {
    FontObject* self = (FontObject*)_self; 
    PyObject_Del(self);
}
static PyObject* font_new(gfxfont_t*gfxfont)
{
    FontObject*font = PyObject_New(FontObject, &FontClass);
    font->gfxfont = gfxfont;
    return (PyObject*)font;
}
static gfxfontlist_t* global_fonts = 0;

static PyObject* lookup_font(gfxfont_t*font)
{
    PyObject*fontobj = gfxfontlist_getuserdata(global_fonts, font->id);
    if(!fontobj) {
	fontobj = font_new(font);
	global_fonts = gfxfontlist_addfont2(global_fonts, font, fontobj);
    }
    Py_INCREF(fontobj);
    return fontobj;
}
static PyObject* font_getattr(PyObject * _self, char* a)
{
    FontObject*self = (FontObject*)_self;
    if(!strcmp(a, "num_glyphs")) {
        return pyint_fromlong(self->gfxfont->num_glyphs);
    } else if(!strcmp(a, "name")) {
        return pystring_fromstring(self->gfxfont->id);
    }
    return forward_getattr(_self, a);
}
static int font_setattr(PyObject * self, char* a, PyObject * o) {
    return -1;
}
static PyObject* font_glyph(PyObject * _self, PyObject* args, PyObject* kwargs) {
    FontObject*self = (FontObject*)_self;
    static char *kwlist[] = {"nr", NULL};
    static long nr = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &nr))
	return NULL;
    return glyph_new(self, nr);
}
static int font_print(PyObject * _self, FILE *fi, int flags)
{
    FontObject*self = (FontObject*)_self;
    fprintf(fi, "<font object %s at %p(%d)>", self->gfxfont->id, _self, _self?_self->ob_refcnt:0);
    return 0;
}
static PyMethodDef font_methods[] =
{
    /* Font functions */
    {"get_glyph", (PyCFunction)font_glyph, M_FLAGS, "get a glyph from this font"},
    {0,0,0,0}
};

//---------------------------------------------------------------------
static PyMethodDef char_methods[];

static void char_dealloc(PyObject* _self) {
    CharObject* self = (CharObject*)_self;
    Py_DECREF(self->font);
    PyObject_Del(self);
}
static PyObject* char_getattr(PyObject * _self, char* a)
{
    CharObject*self = (CharObject*)_self;
    FontObject*font = self->font;
    gfxglyph_t*glyph = &font->gfxfont->glyphs[self->nr];
    if(!strcmp(a, "unicode")) {
        return pyint_fromlong(glyph->unicode);
    } else if(!strcmp(a, "advance")) {
        return PyFloat_FromDouble((self->matrix.m00 * glyph->advance));
    } else if(!strcmp(a, "matrix")) {
        return convert_matrix(&self->matrix);
    } else if(!strcmp(a, "color")) {
        return convert_color(&self->color);
    } else if(!strcmp(a, "size")) {
        return pyint_fromlong(self->size);
    } else if(!strcmp(a, "glyph")) {
        return glyph_new(font, self->nr);
    } else if(!strcmp(a, "font")) {
        Py_INCREF(font);
        return (PyObject*)font;
    } else if(!strcmp(a, "x")) {
    	int x = self->matrix.tx;
        return pyint_fromlong(x);
    } else if(!strcmp(a, "y")) {
    	int y = self->matrix.ty;
        return pyint_fromlong(y);
    }
    
    int lsb = 0; //left side bearing
    int x1 = self->matrix.tx - (self->matrix.m00) * lsb;
    int y1 = self->matrix.ty - (-self->matrix.m11) * font->gfxfont->ascent;
    int x2 = self->matrix.tx + (self->matrix.m00) * font->gfxfont->glyphs[self->nr].advance;
    int y2 = self->matrix.ty + (-self->matrix.m11) * font->gfxfont->descent;
    if(!strcmp(a, "bbox")) {
        PyObject*bbox = PyTuple_New(4);
        PyTuple_SetItem(bbox, 0, pyint_fromlong(x1));
        PyTuple_SetItem(bbox, 1, pyint_fromlong(y1));
        PyTuple_SetItem(bbox, 2, pyint_fromlong(x2));
        PyTuple_SetItem(bbox, 3, pyint_fromlong(y2));
        return bbox;
    } else if(!strcmp(a, "x1")) {
        return pyint_fromlong(x1);
    } else if(!strcmp(a, "y1")) {
        return pyint_fromlong(y1);
    } else if(!strcmp(a, "x2")) {
        return pyint_fromlong(x2);
    } else if(!strcmp(a, "y2")) {
        return pyint_fromlong(y2);
    }
    return forward_getattr(_self, a);
}
static int char_setattr(PyObject * self, char* a, PyObject * o) {
    return -1;
}
static PyObject* char_new(gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix) 
{
    FontObject*fontobj = (FontObject*)lookup_font(font);
    CharObject*chr = PyObject_New(CharObject, &CharClass);
    chr->font = fontobj;
    Py_INCREF(fontobj);
    chr->nr = glyphnr;
    chr->matrix = *matrix;
    chr->size = ceil(1024*fabs(matrix->m00 + matrix->m10)); //horizontal size
    chr->color = *color;
    return (PyObject*)chr;
    return 0;
}
static int char_print(PyObject * _self, FILE *fi, int flags)
{
    CharObject*self = (CharObject*)_self;
    fprintf(fi, "<char object at %p(%d)>", _self, _self?_self->ob_refcnt:0);
    return 0;
}
static PyMethodDef char_methods[] =
{
    /* char functions */
    {0,0,0,0}
};

//---------------------------------------------------------------------
static PyObject* page_render(PyObject* _self, PyObject* args, PyObject* kwargs);
static PyObject* page_asImage(PyObject* _self, PyObject* args, PyObject* kwargs);

PyDoc_STRVAR(page_render_doc, \
"render(output, move=(0,0), clip=None)\n\n"
"Renders a page to the rendering backend specified by the output\n"
"parameter. Rendering consists of calling a number of functions on the\n"
"output device, see the description of the \"PassThrough\" device.\n"
"The page may be shifted to a given position using the move parameter,\n"
"and may also be clipped to a specific size using the clip parameter.\n"
"The clipping operation is applied after the move operation.\n"
"If you don't need to specify custom page sizes or clippings, use\n"
"page.draw instead.\n"
);
static PyObject* page_render(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    PageObject* self = (PageObject*)_self; 
    
    static char *kwlist[] = {"dev", "move", "clip", NULL};
    OutputObject*output = 0;
    PyObject*move=0;
    PyObject*clip=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!|OO", kwlist, &OutputClass, &output,
                &move,&clip
                ))
	return NULL;

    int x=0,y=0;
    int cx1=0,cy1=0,cx2=0,cy2=0;

    if(move) {
	if (!PyArg_ParseTuple(move, "ii", &x,&y))
	    return NULL;
    }
    if(clip) {
	if (!PyArg_ParseTuple(clip, "iiii", &cx1,&cy1,&cx2,&cy2))
	    return NULL;
    }

    Py_BEGIN_ALLOW_THREADS
    if(x|y|cx1|cx2|cy1|cy2)
        self->page->rendersection(self->page, output->output_device,x,y,cx1,cy1,cx2,cy2);
    else
        self->page->render(self->page, output->output_device);
    Py_END_ALLOW_THREADS
    return PY_NONE;
}

PyDoc_STRVAR(page_draw_doc, \
"draw(output)\n\n"
"Renders a page to the rendering backend specified by the output\n"
"parameter, with the default for page width and height.\n"
);
static PyObject* page_draw(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    PageObject* self = (PageObject*)_self; 
    static char *kwlist[] = {"dev", NULL};
    PyObject*output = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", kwlist, &output))
	return NULL;

    PyObject*passthrough = 0;
    if(output->ob_type != &OutputClass) {
	passthrough = passthrough_create(output);
	output = passthrough;
    }
    gfxdevice_t*device = ((OutputObject*)passthrough)->output_device;

    if(setjmp(backjump)) {
	/* exception in the code below*/
	has_backjump = 0;
	//FIXME: this clear the exception, for some reason
	//if(passthrough) {
	//    Py_DECREF(passthrough);
	//}
	return 0;
    }
    has_backjump = 1;
    device->startpage(device, self->page->width, self->page->height);
    self->page->render(self->page, device);
    device->endpage(device);
    has_backjump = 0;

    if(passthrough) {
	Py_DECREF(passthrough);
    }
    return PY_NONE;
}

PyDoc_STRVAR(page_asImage_doc, \
"asImage(width, height)\n\n"
"Creates a bitmap from a page. The bitmap will be returned as a string\n"
"containing RGB triplets. The bitmap will be rescaled to the specified width and\n"
"height. The aspect ratio of width and height doesn't need to be the same\n"
"as the page.\n"
);
static PyObject* page_asImage(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    PageObject* self = (PageObject*)_self; 
    
    static char *kwlist[] = {"width", "height", "allow_threads", NULL};
    int width=0,height=0;
    int allow_threads=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|i", kwlist, &width, &height, &allow_threads))
	return NULL;

    if(!width || !height) {
	return PY_ERROR("invalid dimensions: %dx%d", width,height);
    }

    PyThreadState *_save=0;
    if (allow_threads) {
        Py_UNBLOCK_THREADS
    }
    gfxdevice_t dev1,dev2;
    gfxdevice_render_init(&dev1);
    dev1.setparameter(&dev1, "antialise", "2");
    dev1.setparameter(&dev1, "fillwhite", "1");
    gfxdevice_rescale_init(&dev2, &dev1, width, height, 0);
    dev2.startpage(&dev2, self->page->width, self->page->height);
    self->page->render(self->page, &dev2);
    dev2.endpage(&dev2);
    gfxresult_t*result = dev2.finish(&dev2);
    gfximage_t*img = (gfximage_t*)result->get(result,"page0");
    int l = img->width*img->height;
    int ll = l*3;
    unsigned char*data = (unsigned char*)malloc(ll);
    int s,t;
    for(t=0,s=0;t<l;s+=3,t++) {
	data[s+0] = img->data[t].r;
	data[s+1] = img->data[t].g;
	data[s+2] = img->data[t].b;
    }
    result->destroy(result); result=0;
    free(img->data); free(img);

    PyObject *ret;
#ifdef PYTHON3
    ret = PyByteArray_FromStringAndSize((char*)data,ll);
#else
    ret = PyString_FromStringAndSize((char*)data,ll);
#endif
    free(data);

    if (allow_threads) {
        Py_BLOCK_THREADS
    }
    return ret;
}

static PyMethodDef page_methods[] =
{
    /* Page functions */
    {"render", (PyCFunction)page_render, M_FLAGS, page_render_doc},
    {"draw", (PyCFunction)page_draw, M_FLAGS, page_draw_doc},
    {"asImage", (PyCFunction)page_asImage, M_FLAGS, page_asImage_doc},
    {0,0,0,0}
};
static void page_dealloc(PyObject* _self) {
    PageObject* self = (PageObject*)_self; 
    if(self->page) {
        self->page->destroy(self->page);
        self->page=0;
    }
    if(self->parent) {
	Py_DECREF(self->parent);
	self->parent=0;
    }
    
    PyObject_Del(self);
}

static PyObject* page_getattr(PyObject * _self, char* a)
{
    PageObject*self = (PageObject*)_self;
    
    if(!strcmp(a, "size")) {
        return Py_BuildValue("(ii)", self->page->width, self->page->height);
    } if(!strcmp(a, "doc")) {
	Py_INCREF(self->parent);
        return self->parent;
    } if(!strcmp(a, "nr")) {
        return pyint_fromlong(self->nr);
    } else if(!strcmp(a, "width")) {
        return pyint_fromlong(self->page->width);
    } else if(!strcmp(a, "height")) {
        return pyint_fromlong(self->page->height);
    }
    return forward_getattr(_self, a);
}

static int page_setattr(PyObject * self, char* a, PyObject * o) {
    return -1;
}
static int page_print(PyObject * _self, FILE *fi, int flags)
{
    PageObject*self = (PageObject*)_self;
    fprintf(fi, "<page object at %p(%d)>", _self, _self?_self->ob_refcnt:0);
    return 0;
}

//---------------------------------------------------------------------

PyDoc_STRVAR(doc_getPage_doc,
"getPage(nr)\n\n"
"Get one page from a document file. The nr parameter specifies\n"
"which page to retrieve. Counting starts at 1, so the first page\n"
"can be retrieved by\n"
"    page = doc.getPage(1)\n"
".\n"
"You can find out how many pages a document contains by querying\n"
"its pages field (doc.pages)\n"
);
static PyObject*page_new(DocObject*doc, int pagenr)
{
    PageObject*page = PyObject_New(PageObject, &PageClass);
    page->page = doc->doc->getpage(doc->doc, pagenr);
    page->nr = pagenr;
    page->parent = (PyObject*)doc;
    Py_INCREF(page->parent);
    if(!page->page) {
        PyObject_Del(page);
        return PY_ERROR("Couldn't extract page %d", pagenr);
    }
    return (PyObject*)page;
}

static PyObject* doc_getPage(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    DocObject* self = (DocObject*)_self;

    static char *kwlist[] = {"nr", NULL};
    int pagenr = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &pagenr))
	return NULL;
    return page_new(self, pagenr);
}
static PyObject* doc_getiter(PyObject* _self)
{
    DocObject* self = (DocObject*)_self;
    self->page_pos = 1;
    Py_INCREF(self);
    return (PyObject*)self;
}
static PyObject* doc_iternext(PyObject* _self)
{
    DocObject* self = (DocObject*)_self;
    if(self->page_pos > self->doc->num_pages) {
	return NULL;
    }
    return page_new(self, self->page_pos++);
}


PyDoc_STRVAR(doc_getInfo_doc,
"getInfo(key)\n\n"
"Retrieve some information about a document. For PDF files, key\n"
"can have the following values:\n\n"
"\"title\", \"subject\", \"keywords\", \"author\", \"creator\", \"producer\",\n"
"\"creationdate\", \"moddate\", \"linearized\", \"tagged\", \"encrypted\",\n"
"\"oktoprint\", \"oktocopy\", \"oktochange\", \"oktoaddnotes\", \"version\".\n\n"
"If the \"oktocopy\" digital rights management flag is set to \"no\", then the\n"
"pdf parser won't allow you to access the PDF file. Trying to extract pages\n"
"from it will raise an exception.\n"
);
static PyObject* doc_getInfo(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    DocObject* self = (DocObject*)_self;

    static char *kwlist[] = {"key", NULL};
    char*key = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &key))
	return NULL;

    char*s = self->doc->getinfo(self->doc, key);
    return pystring_fromstring(s);
}

PyDoc_STRVAR(doc_setparameter_doc,
"setparameter(key, value)\n\n"
"Pass a parameter or setting to the document parser. Unlike\n"
"the module level setparameter() function, the parameters set\n"
"using setparameter will only be valid for the object itself\n"
"during its lifetime.\n"
);
static PyObject* doc_setparameter(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    DocObject* self = (DocObject*)_self;

    static char *kwlist[] = {"key", "value", NULL};
    char*key = 0, *value=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwlist, &key,&value))
	return NULL;

    self->doc->setparameter(self->doc, key, value);
    return PY_NONE;
}

PyDoc_STRVAR(f_open_doc,
"open(type, filename) -> object\n\n"
"Open a PDF, SWF or image file. The type argument should be \"pdf\",\n"
"\"swf\" or \"image\" accordingly. It returns a doc object which can be\n"
"used to process the file contents.\n"
"E.g.\n"
"    doc = open(\"pdf\", \"document.pdf\")\n"
"    doc = open(\"swf\", \"flashfile.swf\")\n"
"    doc = open(\"image\", \"image.png\")\n"
"If the file could not be loaded, or is a encrypted PDF file without\n"
"a proper password specified, an exception is being raised.\n"
"If the filename argument contains a '|' char, everything behind\n"
"the '|' is treated as password used for opening the file.\n"
"E.g.\n"
"    doc = open(\"pdf\", \"document.pdf|mysecretpassword\")\n"
".\n"
"Notice that for image files, the only supported file formats right now\n"
"are jpeg and png.\n"
);
static PyObject* f_open(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"type", "filename", NULL};
    char*filename=0;
    char*type=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwlist, &type, &filename)) {
	static char *kwlist2[] = {"filename", NULL};
        type = 0;
	PyErr_Clear();
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist2, &filename))
	    return NULL;
    }

    DocObject*self = PyObject_New(DocObject, &DocClass);

    if(!type) { //autodetect
	type = "pdf"; //default
	int l = strlen(filename);
	if(l>4) {
	    if(filename[l-4]=='.') {
		if(strchr("pP", filename[l-3]) && strchr("dD", filename[l-2]) && strchr("fF", filename[l-1]))
		    type = "pdf";
		if(strchr("jJ", filename[l-3]) && strchr("pP", filename[l-2]) && strchr("gG", filename[l-1]))
		    type = "image";
		if(strchr("pP", filename[l-3]) && strchr("nN", filename[l-2]) && strchr("gG", filename[l-1]))
		    type = "image";
		if(strchr("sS", filename[l-3]) && strchr("wW", filename[l-2]) && strchr("fF", filename[l-1]))
		    type = "swf";
	    } else if(filename[l-5]=='.') {
		type = "image";
	    }
	}
    }
   
    state_t*state = STATE(module);
    if(!strcmp(type,"pdf")) {
        Py_BEGIN_ALLOW_THREADS
        self->doc = state->pdfdriver->open(state->pdfdriver,filename);
        Py_END_ALLOW_THREADS
    }
    else if(!strcmp(type, "image") || !strcmp(type, "img")) {
        Py_BEGIN_ALLOW_THREADS
        self->doc = state->imagedriver->open(state->imagedriver, filename);
        Py_END_ALLOW_THREADS
    }
    else if(!strcmp(type, "swf") || !strcmp(type, "SWF")) {
        Py_BEGIN_ALLOW_THREADS
        self->doc = state->swfdriver->open(state->imagedriver, filename);
        Py_END_ALLOW_THREADS
    }
    else
	return PY_ERROR("Unknown type %s", type);

    if(!self->doc) {
        PyObject_Del(self);
        return PY_ERROR("Couldn't open %s", filename);
    }
    self->filename = strdup(filename);
    return (PyObject*)self;
}

static PyMethodDef doc_methods[] =
{
    /* PDF functions */
    {"getPage", (PyCFunction)doc_getPage, METH_KEYWORDS, doc_getPage_doc},
    {"getInfo", (PyCFunction)doc_getInfo, METH_KEYWORDS, doc_getInfo_doc},
    {"setparameter", (PyCFunction)doc_setparameter, METH_KEYWORDS, doc_setparameter_doc},
    {0,0,0,0}
};

static void doc_dealloc(PyObject* _self) {
    DocObject* self = (DocObject*)_self;
    if(self->doc) {
        self->doc->destroy(self->doc);
        self->doc=0;
    }
    if(self->filename) {
	free(self->filename);self->filename=0;
    }
    PyObject_Del(self);
}
static PyObject* doc_getattr(PyObject * _self, char* a)
{
    DocObject*self = (DocObject*)_self;
    if(!strcmp(a, "pages")) {
        return pyint_fromlong(self->doc->num_pages);
    }
    if(!strcmp(a, "filename")) {
        return pystring_fromstring(self->filename);
    }
    return forward_getattr(_self, a);
}
static int doc_setattr(PyObject * self, char* a, PyObject * o) {
    return -1;
}
static int doc_print(PyObject * _self, FILE *fi, int flags)
{
    DocObject*self = (DocObject*)_self;
    fprintf(fi, "<doc object at %p(%d)>", _self, _self?_self->ob_refcnt:0);
    return 0;
}

//---------------------------------------------------------------------
PyDoc_STRVAR(f_createKDTree_doc, \
"KDTree()\n\n"
"Creates a KDTree, which can be used to store bounding boxes\n"
);
static PyObject* f_createKDTree(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "", kwlist))
	return NULL;
    KDTreeObject*self = PyObject_New(KDTreeObject, &KDTreeClass);
    self->kdtree = kdtree_new();
    return (PyObject*)self;
}
static void gfx_kdtree_dealloc(PyObject* _self) {
    KDTreeObject* self = (KDTreeObject*)_self;
    /* FIXME: we still need to Py_DECREF all PyObjects in the tree */
    kdtree_destroy(self->kdtree);
    PyObject_Del(self);
}
static PyObject* gfx_kdtree_getattr(PyObject * _self, char* a)
{
    KDTreeObject*self = (KDTreeObject*)_self;
    return forward_getattr(_self, a);
}
static int gfx_kdtree_setattr(PyObject * self, char* a, PyObject * o) {
    return -1;
}
static int gfx_kdtree_print(PyObject * _self, FILE *fi, int flags)
{
    KDTreeObject*self = (KDTreeObject*)_self;
    fprintf(fi, "<kdtree object at %p(%d)>", _self, _self?_self->ob_refcnt:0);
    return 0;
}
PyDoc_STRVAR(gfx_kdtree_add_box_doc,
"put(bbox, data)\n\n"
"Add a rectangular area to the tree. All queries within that area\n"
"will subsequently return 'data'\n"
);
static PyObject* gfx_kdtree_add_box(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    KDTreeObject* self = (KDTreeObject*)_self;

    static char *kwlist[] = {"bbox", "data", NULL};
    int x1=0,y1=0,x2=0,y2=0;
    PyObject*value=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(iiii)O", kwlist, &x1, &y1, &x2, &y2, &value))
	return NULL;
    
    kdtree_add_box(self->kdtree, x1,y1,x2,y2, value);
    Py_INCREF(value);

    return PY_NONE;
}

PyDoc_STRVAR(gfx_kdtree_find_doc,
"find(x,y)\n\n"
"Look for a coordinate in the kdtree. It will return last inserted object covering that position, or None.\n"
);
static PyObject* gfx_kdtree_find(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    KDTreeObject* self = (KDTreeObject*)_self;

    static char *kwlist[] = {"x", "y", NULL};
    int x=0,y=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii", kwlist, &x, &y))
	return NULL;
    PyObject*value = (PyObject*)kdtree_find(self->kdtree, x,y);
    if(!value) {
        return PY_NONE;
    } else {
        Py_INCREF(value);
        return value;
    }
}

static PyMethodDef gfx_kdtree_methods[] =
{
    {"add_box", (PyCFunction)gfx_kdtree_add_box, METH_KEYWORDS, gfx_kdtree_add_box_doc},
    {"find", (PyCFunction)gfx_kdtree_find, METH_KEYWORDS, gfx_kdtree_find_doc},
    {0,0,0,0}
};

//---------------------------------------------------------------------
PyDoc_STRVAR(f_createBitmap_doc, \
"Bitmap()\n\n"
"Creates a Bitmap, which can be used to store bounding boxes\n"
);
static PyObject* create_bitmap(gfximage_t*img)
{
    BitmapObject*self = PyObject_New(BitmapObject, &BitmapClass);
    self->image = malloc(sizeof(gfximage_t));
    self->image->data = malloc(sizeof(gfxcolor_t)*img->width*img->height);
    memcpy(self->image->data, img->data, sizeof(gfxcolor_t)*img->width*img->height);
    self->image->width = img->width;
    self->image->height = img->height;
    return (PyObject*)self;
}
static void gfx_bitmap_dealloc(PyObject* _self) {
    BitmapObject* self = (BitmapObject*)_self;
    free(self->image->data);
    free(self->image);
    PyObject_Del(self);
}
static PyObject* gfx_bitmap_getattr(PyObject * _self, char* a)
{
    BitmapObject*self = (BitmapObject*)_self;
    if(!strcmp(a, "width")) {
        return pyint_fromlong(self->image->width);
    } else if(!strcmp(a, "height")) {
        return pyint_fromlong(self->image->height);
    }
    return forward_getattr(_self, a);
}
static int gfx_bitmap_setattr(PyObject * self, char* a, PyObject * o) {
    return -1;
}
PyDoc_STRVAR(gfx_bitmap_save_png_doc,
"save_jpeg(filename, quality)\n\n"
"Save bitmap to a png file.\n"
);
static PyObject* gfx_bitmap_save_png(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    ImageObject* self = (ImageObject*)_self;
    static char *kwlist[] = {"filename", NULL};
    char*filename=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filename))
	return NULL;
    gfximage_save_png_quick(self->image, filename);
    return PY_NONE;
}
PyDoc_STRVAR(gfx_bitmap_save_jpeg_doc,
"save_jpeg(filename, quality)\n\n"
"Save bitmap to a jpeg file. The quality parameter is optional.\n"
);
static PyObject* gfx_bitmap_save_jpeg(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    ImageObject* self = (ImageObject*)_self;
    static char *kwlist[] = {"filename", "quality", NULL};
    char*filename=0;
    int quality=95;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|i", kwlist, &filename, &quality))
	return NULL;
    gfximage_save_jpeg(self->image, filename, quality);
    return PY_NONE;
}
static int gfx_bitmap_print(PyObject * _self, FILE *fi, int flags)
{
    BitmapObject*self = (BitmapObject*)_self;
    fprintf(fi, "<bitmap object at %p(%d)>", _self, _self?_self->ob_refcnt:0);
    return 0;
}
static PyMethodDef gfx_bitmap_methods[] =
{
    {"save_png", (PyCFunction)gfx_bitmap_save_png, METH_KEYWORDS, gfx_bitmap_save_png_doc},
    {"save_jpeg", (PyCFunction)gfx_bitmap_save_jpeg, METH_KEYWORDS, gfx_bitmap_save_jpeg_doc},
    {0,0,0,0}
};

//---------------------------------------------------------------------

#ifndef PYTHON3
#define PYTHON23_HEAD_INIT \
    PyObject_HEAD_INIT(NULL) \
    0,
#else
#define PYTHON23_HEAD_INIT \
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
#endif

PyDoc_STRVAR(output_doc,
"An Output object can be used as parameter to the render()\n"
"call of a page. It's not possible to create this type of\n"
"object directly (i.e., from a class), however you can\n"
"use a PassThrough() device to pass things over to Python.\n"
"Examples for classes implementing the Output class are: \n"
"ImageList, SWF, PlainText and PassThrough.\n"
);
static PyTypeObject OutputClass =
{
    PYTHON23_HEAD_INIT
    tp_name: "gfx.Output",
    tp_basicsize: sizeof(OutputObject),
    tp_itemsize: 0,
    tp_dealloc: output_dealloc,
    tp_print: output_print,
    tp_getattr: output_getattr,
    tp_setattr: output_setattr,
    tp_doc: output_doc,
    tp_methods: output_methods
};
PyDoc_STRVAR(page_doc,
"A Page object contains a single page of a document.\n"
"page.width and page.height (or page.size) contain the\n"
"page dimensions. page.nr is the number of the page, and\n"
"page.doc is the parent document.\n"
);
static PyTypeObject PageClass =
{
    PYTHON23_HEAD_INIT
    tp_name: "gfx.Page",
    tp_basicsize: sizeof(PageObject),
    tp_itemsize: 0,
    tp_dealloc: page_dealloc,
    tp_print: page_print,
    tp_getattr: page_getattr,
    tp_setattr: page_setattr,
    tp_doc: page_doc,
    tp_methods: page_methods
};
PyDoc_STRVAR(doc_doc,
"A Doc object is used for storing a document (like a PDF).\n"
"doc.pages contains the number of pages in the document,\n"
"and doc.filename the name of the file the document was\n"
"created (loaded) from. If the document was created from\n"
"an image file, the number of pages is always 1\n"
);
static PyTypeObject DocClass =
{
    PYTHON23_HEAD_INIT
    tp_name: "gfx.Doc",
    tp_basicsize: sizeof(DocObject),
    tp_itemsize: 0,
    tp_dealloc: doc_dealloc,

    tp_print: doc_print,
    tp_getattr: doc_getattr,
    tp_setattr: doc_setattr,
    tp_doc: doc_doc,
    tp_methods: doc_methods,
	
    tp_iter: doc_getiter,
    tp_iternext: doc_iternext,

#ifndef PYTHON3
    tp_flags: Py_TPFLAGS_HAVE_ITER,
#endif
};
PyDoc_STRVAR(font_doc,
"A font is a list of glyphs\n"
);
static PyTypeObject FontClass =
{
    PYTHON23_HEAD_INIT
    tp_name: "gfx.Font",
    tp_basicsize: sizeof(FontObject),
    tp_itemsize: 0,
    tp_dealloc: font_dealloc,
    tp_print: font_print,
    tp_getattr: font_getattr,
    tp_setattr: font_setattr,
    tp_doc: font_doc,
    tp_methods: font_methods,
};
PyDoc_STRVAR(glyph_doc,
"A glyph is a polygon and a unicode index\n"
);
static PyTypeObject GlyphClass =
{
    PYTHON23_HEAD_INIT
    tp_name: "gfx.Glyph",
    tp_basicsize: sizeof(GlyphObject),
    tp_itemsize: 0,
    tp_dealloc: glyph_dealloc,
    tp_print: glyph_print,
    tp_getattr: glyph_getattr,
    tp_setattr: glyph_setattr,
    tp_doc: glyph_doc,
    tp_methods: glyph_methods,
};
PyDoc_STRVAR(char_doc,
"A char is a glyph at a given position (in a given color)\n"
);
static PyTypeObject CharClass =
{
    PYTHON23_HEAD_INIT
    tp_name: "gfx.Char",
    tp_basicsize: sizeof(CharObject),
    tp_itemsize: 0,
    tp_dealloc: char_dealloc,
    tp_print: char_print,
    tp_getattr: char_getattr,
    tp_setattr: char_setattr,
    tp_doc: char_doc,
    tp_methods: char_methods,
};

PyDoc_STRVAR(gfx_bitmap_doc,
"A bitmap.\n"
);
static PyTypeObject BitmapClass =
{
    PYTHON23_HEAD_INIT
    tp_name: "gfx.Bitmap",
    tp_basicsize: sizeof(BitmapObject),
    tp_itemsize: 0,
    tp_dealloc: gfx_bitmap_dealloc,
    tp_print: gfx_bitmap_print,
    tp_getattr: gfx_bitmap_getattr,
    tp_setattr: gfx_bitmap_setattr,
    tp_doc: gfx_bitmap_doc,
    tp_methods: gfx_bitmap_methods,
};


PyDoc_STRVAR(gfx_kdtree_doc,
"A kdtree is a two dimensional tree for storing bounding box data\n"
);
static PyTypeObject KDTreeClass =
{
    PYTHON23_HEAD_INIT
    tp_name: "gfx.KDTree",
    tp_basicsize: sizeof(KDTreeObject),
    tp_itemsize: 0,
    tp_dealloc: gfx_kdtree_dealloc,
    tp_print: gfx_kdtree_print,
    tp_getattr: gfx_kdtree_getattr,
    tp_setattr: gfx_kdtree_setattr,
    tp_doc: gfx_kdtree_doc,
    tp_methods: gfx_kdtree_methods,
};


//=====================================================================

PyDoc_STRVAR(f_setparameter_doc, \
"setparameter(key,value)\n\n"
"Set a parameter in the gfx module (which might affect the PDF\n"
"parser or any of the rendering backends). This is a parameter\n"
"which would usually be passed with the \"-s\" option to pdf2swf.\n"
"For a list of all parameters, see the output of\n"
"    pdf2swf -s help\n"
"and\n"
"    pdf2swf somefile.pdf -s help\n"
".\n"
);
static PyObject* f_setparameter(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"key", "value", NULL};
    char*key=0,*value=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwlist, &key, &value))
	return NULL;
    state_t*state = STATE(module);
    state->pdfdriver->setparameter(state->pdfdriver,key,value);
    return PY_NONE;
}

PyDoc_STRVAR(f_verbose_doc, \
"verbose(level)\n\n"
"Set the logging verbosity of the gfx module. Log levels are:\n"
"level=-1          Log nothing\n"
"level=0 (fatal)   Log only fatal errors\n"
"level=1 (error)   Log only fatal errors and errors\n"
"level=2 (warn)    Log all errors and warnings\n"
"level=3 (notice)  Log also some rudimentary data about the parsing/conversion\n"
"level=4 (verbose) Log some additional parsing information\n"
"level=5 (debug)   Log debug statements\n"
"level=6 (trace)   Log extended debug statements\n"
"All logging messages are written to stdout.\n"
);
static PyObject* f_verbose(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"val", NULL};
    int val;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &val))
	return NULL;
    setConsoleLogging(val);
    return PY_NONE;
}

PyDoc_STRVAR(f_addfont_doc, \
"addfont(filename)\n\n"
"Passes an additional font file to the PDF parser. If a PDF contains\n"
"external fonts (i.e. fonts which are not contained in the PDF itself)\n"
"then the files added by addfont() will be searched.\n"
);

static PyObject* f_addfont(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"filename", NULL};
    char*filename=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filename))
	return NULL;
    state_t*state = STATE(module);
    state->pdfdriver->setparameter(state->pdfdriver,"font", filename);
    return PY_NONE;
}

PyDoc_STRVAR(f_addfontdir_doc, \
"addfontdir(dirname)\n\n"
"Passes a complete directory containing fonts to the PDF parser. Any\n"
"font file within this directory might be used to resolve external fonts\n"
"in PDF files\n"
);
static PyObject* f_addfontdir(PyObject* module, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"filename", NULL};
    char*filename=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filename))
	return NULL;
    state_t*state = STATE(module);
    state->pdfdriver->setparameter(state->pdfdriver,"fontdir", filename);
    return PY_NONE;
}

static PyMethodDef gfx_methods[] =
{
    /* sources */
    {"open", (PyCFunction)f_open, M_FLAGS, f_open_doc},
    {"addfont", (PyCFunction)f_addfont, M_FLAGS, f_addfont_doc},
    {"addfontdir", (PyCFunction)f_addfontdir, M_FLAGS, f_addfontdir_doc},
    {"setparameter", (PyCFunction)f_setparameter, M_FLAGS, f_setparameter_doc},
    {"verbose", (PyCFunction)f_verbose, M_FLAGS, f_verbose_doc},

    /* devices */
    {"SWF", (PyCFunction)f_createSWF, M_FLAGS, f_createSWF_doc},
    {"OCR", (PyCFunction)f_createOCR, M_FLAGS, f_createOCR_doc},
    {"ImageList", (PyCFunction)f_createImageList, M_FLAGS, f_createImageList_doc},
    {"PlainText", (PyCFunction)f_createPlainText, M_FLAGS, f_createPlainText_doc},
    {"PassThrough", (PyCFunction)f_createPassThrough, M_FLAGS, f_createPassThrough_doc},
#ifdef USE_OPENGL
    {"OpenGL", (PyCFunction)f_createOpenGL, M_FLAGS, f_createOpenGL_doc},
#endif
    {"KDTree", (PyCFunction)f_createKDTree, M_FLAGS, f_createKDTree_doc},

    /* sentinel */
    {0, 0, 0, 0}
};

PyDoc_STRVAR(gfx_doc, \
"This module contains a PDF parser (based on xpdf) and a number of\n"
"rendering backends. In particular, it can extract text from PDF pages,\n"
"create bitmaps from them, or convert PDF files to SWF.\n" 
"The latter functionality is similar to what is offered by swftools'\n" 
"(http://www.swftools.org) pdf2swf utility, however more powerful-\n" 
"You can also create individual SWF files from single pages of the PDF\n" 
"or mix pages from different PDF files.\n"
);

void gfx_free(void*module)
{
    state_t*state = STATE(module);
    if(state->pdfdriver && state->pdfdriver->destroy)
	state->pdfdriver->destroy(state->pdfdriver);
    if(state->swfdriver && state->swfdriver->destroy)
	state->swfdriver->destroy(state->swfdriver);
    if(state->imagedriver && state->imagedriver->destroy)
	state->imagedriver->destroy(state->imagedriver);
    memset(state, 0, sizeof(state_t));
}

#ifdef PYTHON3
static struct PyModuleDef gfx_moduledef = {
        PyModuleDef_HEAD_INIT,
        "gfx",
        gfx_doc,
        sizeof(state_t),
        gfx_methods,
        /*reload*/NULL,
        /*traverse*/NULL,
        /*clear*/NULL,
        gfx_free,
};
#endif

PyObject * PyInit_gfx(void)
{
    initLog(0,0,0,0,0,2);
#ifdef PYTHON3
    PyObject*module = PyModule_Create(&gfx_moduledef);
#else
    PyObject*module = Py_InitModule3("gfx", gfx_methods, gfx_doc);
    OutputClass.ob_type = &PyType_Type;
    PageClass.ob_type = &PyType_Type;
    DocClass.ob_type = &PyType_Type;
    FontClass.ob_type = &PyType_Type;
    CharClass.ob_type = &PyType_Type;
    KDTreeClass.ob_type = &PyType_Type;
    BitmapClass.ob_type = &PyType_Type;
#endif
    
    state_t* state = STATE(module);
    memset(state, 0, sizeof(state_t));
    state->pdfdriver = gfxsource_pdf_create();
    state->swfdriver = gfxsource_swf_create();
    state->imagedriver = gfxsource_image_create();

    PyObject*module_dict = PyModule_GetDict(module);
    PyDict_SetItemString(module_dict, "Doc", (PyObject*)&DocClass);
    PyDict_SetItemString(module_dict, "Page", (PyObject*)&PageClass);
    PyDict_SetItemString(module_dict, "Output", (PyObject*)&OutputClass);
    PyDict_SetItemString(module_dict, "Char", (PyObject*)&CharClass);
    PyDict_SetItemString(module_dict, "Font", (PyObject*)&FontClass);
    PyDict_SetItemString(module_dict, "KDTree", (PyObject*)&KDTreeClass);
    PyDict_SetItemString(module_dict, "Bitmap", (PyObject*)&BitmapClass);
    PyDict_SetItemString(module_dict, "VERSION", (PyObject*)pystring_fromstring(PYTHON_GFX_VERSION));

    return module;
}
#ifndef PYTHON3
void initgfx(void) {
    PyInit_gfx();
}
#endif
