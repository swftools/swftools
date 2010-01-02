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
#include "../utf8.h"

static gfxsource_t*pdfdriver = 0;
static gfxsource_t*swfdriver = 0;
static gfxsource_t*imagedriver = 0;

staticforward PyTypeObject OutputClass;
staticforward PyTypeObject PageClass;
staticforward PyTypeObject DocClass;

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
} DocObject;

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
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filename))
	return NULL;

    gfxresult_t*result = self->output_device->finish(self->output_device);
    self->output_device = 0;
    if(result->save(result, filename) < 0) {
	return PY_ERROR("Couldn't write to %s", filename);
    }
    result->destroy(result);
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
    self->output_device->startpage(self->output_device, width, height);
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
        if(!PyString_Check(_type))
            return PY_ERROR("point tuples must start with a string");
        char*type = PyString_AsString(_type);
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
    self->output_device->endpage(self->output_device);
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
static PyObject* f_createSWF(PyObject* parent, PyObject* args, PyObject* kwargs)
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
static PyObject* f_createOCR(PyObject* parent, PyObject* args, PyObject* kwargs)
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
static PyObject* f_createImageList(PyObject* parent, PyObject* args, PyObject* kwargs)
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
static PyObject* f_createPlainText(PyObject* parent, PyObject* args, PyObject* kwargs)
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
static PyObject* f_createOpenGL(PyObject* parent, PyObject* args, PyObject* kwargs)
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
        if(p=='s') {
            char*s = va_arg(ap, char*);
            PyTuple_SetItem(tuple, pos, PyString_FromString(s));
        } else if(p=='i') {
            int i = va_arg(ap, int);
            PyTuple_SetItem(tuple, pos, PyInt_FromLong(i));
        } else if(p=='d') {
            int i = va_arg(ap, double);
            PyTuple_SetItem(tuple, pos, PyFloat_FromDouble(i));
        } else if(p=='c') {
            void* ptr = va_arg(ap, void*);
            gfxcolor_t*col = (gfxcolor_t*)ptr;
            PyObject*colobj = PyTuple_New(4);
            PyTuple_SetItem(colobj, 0, PyInt_FromLong(col->a));
            PyTuple_SetItem(colobj, 1, PyInt_FromLong(col->r));
            PyTuple_SetItem(colobj, 2, PyInt_FromLong(col->g));
            PyTuple_SetItem(colobj, 3, PyInt_FromLong(col->b));
            PyTuple_SetItem(tuple, pos, colobj);
        } else if(p=='l') {
            void* ptr = va_arg(ap, void*);
            gfxline_t*line = (gfxline_t*)ptr;
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
                    PyTuple_SetItem(point, 0, PyString_FromString("m"));
                    PyTuple_SetItem(point, 1, PyFloat_FromDouble(l->x));
                    PyTuple_SetItem(point, 2, PyFloat_FromDouble(l->y));
                } else if(l->type == gfx_lineTo) {
                    point = PyTuple_New(3);
                    PyTuple_SetItem(point, 0, PyString_FromString("l"));
                    PyTuple_SetItem(point, 1, PyFloat_FromDouble(l->x));
                    PyTuple_SetItem(point, 2, PyFloat_FromDouble(l->y));
                } else if(l->type == gfx_splineTo) {
                    point = PyTuple_New(5);
                    PyTuple_SetItem(point, 0, PyString_FromString("s"));
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
            PyTuple_SetItem(tuple, pos, list);
        } else {
            PyTuple_SetItem(tuple, pos, PY_NONE);
        }
        pos++;
    }
    va_end(ap);
    PyObject*f = PyObject_GetAttrString(self->pyobj, function);
    if(!f)
        return 0;
    PyErr_Clear();
    PyObject* result = PyObject_CallObject(f, tuple);

    if(!result) { 
        PyErr_Print();
        PyErr_Clear();
        return 1;
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
    if(!callback_python("drawchar", dev, "ficm", font, glyphnr, color, matrix))
    {
        if(!font)
            return;
        gfxglyph_t*glyph = &font->glyphs[glyphnr];
        gfxline_t*line2 = gfxline_clone(glyph->line);
        gfxline_transform(line2, matrix);
        my_fill(dev, line2, color);
        gfxline_free(line2);
        return;
    }
}
static void my_drawlink(gfxdevice_t*dev, gfxline_t*line, const char*action)
{
    callback_python("drawlink", dev, "ls", line, action);
}
static void my_endpage(gfxdevice_t*dev)
{
    callback_python("drawlink", dev, "");
}
static gfxresult_t* my_finish(gfxdevice_t*dev)
{
    callback_python("finish", dev, "");
    return 0;
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
static PyObject* f_createPassThrough(PyObject* parent, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"device", NULL};
    PyObject*obj;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", kwlist, &obj))
	return NULL;
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

static PyMethodDef output_methods[] =
{
    /* Output functions */
    {"save", (PyCFunction)output_save, METH_KEYWORDS, output_save_doc},
    {"startpage", (PyCFunction)output_startpage, METH_KEYWORDS, output_startpage_doc},
    {"fill", (PyCFunction)output_fill, METH_KEYWORDS, output_fill_doc},
    {"fillbitmap", (PyCFunction)output_fillbitmap, METH_KEYWORDS, output_fillbitmap_doc},
    {"stroke", (PyCFunction)output_stroke, METH_KEYWORDS, output_stroke_doc},
    {"endpage", (PyCFunction)output_endpage, METH_KEYWORDS, output_endpage_doc},
    {"setparameter", (PyCFunction)output_setparameter, METH_KEYWORDS, output_setparameter_doc},
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
        return PyInt_FromLong(self->output_device->x1);
    } else if(!strcmp(a, "y1")) {
        return PyInt_FromLong(self->output_device->y1);
    } else if(!strcmp(a, "x2")) {
        return PyInt_FromLong(self->output_device->x2);
    } else if(!strcmp(a, "y2")) {
        return PyInt_FromLong(self->output_device->y2);
    }*/
    
    return Py_FindMethod(output_methods, _self, a);
}
static int output_setattr(PyObject * _self, char* a, PyObject * o) 
{
    OutputObject*self = (OutputObject*)_self;
    if(!PyString_Check(o))
        return -1;
    char*value = PyString_AsString(o);
    self->output_device->setparameter(self->output_device, a, value);
    return -1;
}
static int output_print(PyObject * _self, FILE *fi, int flags)
{
    OutputObject*self = (OutputObject*)_self;
    fprintf(fi, "%08x(%d)", (int)_self, _self?_self->ob_refcnt:0);
    return 0;
}

//---------------------------------------------------------------------
staticforward PyObject* page_render(PyObject* _self, PyObject* args, PyObject* kwargs);
staticforward PyObject* page_asImage(PyObject* _self, PyObject* args, PyObject* kwargs);

PyDoc_STRVAR(page_render_doc, \
"render(output, move=(0,0), clip=None)\n\n"
"Renders a page to the rendering backend specified by the output\n"
"parameter. Rendering consists of calling a number of functions on the\n"
"output device, see the description of the \"PassThrough\" device.\n"
"The page may be shifted to a given position using the move parameter,\n"
"and may also be clipped to a specific size using the clip parameter.\n"
"The clipping operation is applied after the move operation.\n"
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

    if(x|y|cx1|cx2|cy1|cy2)
        self->page->rendersection(self->page, output->output_device,x,y,cx1,cy1,cx2,cy2);
    else
        self->page->render(self->page, output->output_device);
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
    
    static char *kwlist[] = {"width", "height", NULL};
    int width=0,height=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii", kwlist, &width, &height))
	return NULL;

    if(!width || !height) {
	return PY_ERROR("invalid dimensions: %dx%d", width,height);
    }

    gfxdevice_t dev1,dev2;
    gfxdevice_render_init(&dev1);
    dev1.setparameter(&dev1, "antialise", "2");
    gfxdevice_rescale_init(&dev2, &dev1, width, height, 0);
    dev2.startpage(&dev2, self->page->width, self->page->height);
    self->page->render(self->page, &dev2);
    dev2.endpage(&dev2);
    gfxresult_t*result = dev2.finish(&dev2);
    gfximage_t*img = (gfximage_t*)result->get(result,"page0");
    int l = img->width*img->height;
    unsigned char*data = (unsigned char*)malloc(img->width*img->height*3);
    int s,t;
    for(t=0,s=0;t<l;s+=3,t++) {
	data[s+0] = img->data[t].r;
	data[s+1] = img->data[t].g;
	data[s+2] = img->data[t].b;
    }
    result->destroy(result);
    return PyString_FromStringAndSize((char*)data,img->width*img->height*3);
}

static PyMethodDef page_methods[] =
{
    /* Page functions */
    {"render", (PyCFunction)page_render, METH_KEYWORDS, page_render_doc},
    {"asImage", (PyCFunction)page_asImage, METH_KEYWORDS, page_asImage_doc},
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
        return PyInt_FromLong(self->nr);
    } else if(!strcmp(a, "width")) {
        return PyInt_FromLong(self->page->width);
    } else if(!strcmp(a, "height")) {
        return PyInt_FromLong(self->page->height);
    }
    return Py_FindMethod(page_methods, _self, a);
}

static int page_setattr(PyObject * self, char* a, PyObject * o) {
    return -1;
}
static int page_print(PyObject * _self, FILE *fi, int flags)
{
    PageObject*self = (PageObject*)_self;
    fprintf(fi, "%08x(%d)", (int)_self, _self?_self->ob_refcnt:0);
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
static PyObject* doc_getPage(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    DocObject* self = (DocObject*)_self;

    static char *kwlist[] = {"nr", NULL};
    int pagenr = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &pagenr))
	return NULL;

    PageObject*page = PyObject_New(PageObject, &PageClass);
    page->page = self->doc->getpage(self->doc, pagenr);
    page->nr = pagenr;
    page->parent = _self;
    Py_INCREF(page->parent);
    if(!page->page) {
        PyObject_Del(page);
        return PY_ERROR("Couldn't extract page %d", pagenr);
    }
    return (PyObject*)page;
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
    return PyString_FromString(s);
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
static PyObject* f_open(PyObject* parent, PyObject* args, PyObject* kwargs)
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
   
    if(!strcmp(type,"pdf"))
	self->doc = pdfdriver->open(pdfdriver,filename);
    else if(!strcmp(type, "image") || !strcmp(type, "img"))  
	self->doc = imagedriver->open(imagedriver, filename);
    else if(!strcmp(type, "swf") || !strcmp(type, "SWF"))
	self->doc = swfdriver->open(imagedriver, filename);
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
        return PyInt_FromLong(self->doc->num_pages);
    }
    if(!strcmp(a, "filename")) {
        return PyString_FromString(self->filename);
    }
    return Py_FindMethod(doc_methods, _self, a);
}
static int doc_setattr(PyObject * self, char* a, PyObject * o) {
    return -1;
}
static int doc_print(PyObject * _self, FILE *fi, int flags)
{
    DocObject*self = (DocObject*)_self;
    fprintf(fi, "%08x(%d)", (int)_self, _self?_self->ob_refcnt:0);
    return 0;
}

//---------------------------------------------------------------------

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
    PyObject_HEAD_INIT(NULL)
    0,
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
    PyObject_HEAD_INIT(NULL)
    0,
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
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "gfx.Doc",
    tp_basicsize: sizeof(DocObject),
    tp_itemsize: 0,
    tp_dealloc: doc_dealloc,
    tp_print: doc_print,
    tp_getattr: doc_getattr,
    tp_setattr: doc_setattr,
    tp_doc: doc_doc,
    tp_methods: doc_methods,
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
static PyObject* f_setparameter(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"key", "value", NULL};
    char*key=0,*value=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwlist, &key, &value))
	return NULL;
    pdfdriver->setparameter(pdfdriver,key,value);
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

static PyObject* f_addfont(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"filename", NULL};
    char*filename=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filename))
	return NULL;
    pdfdriver->setparameter(pdfdriver,"font", filename);
    return PY_NONE;
}

PyDoc_STRVAR(f_addfontdir_doc, \
"addfontdir(dirname)\n\n"
"Passes a complete directory containing fonts to the PDF parser. Any\n"
"font file within this directory might be used to resolve external fonts\n"
"in PDF files\n"
);
static PyObject* f_addfontdir(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"filename", NULL};
    char*filename=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filename))
	return NULL;
    pdfdriver->setparameter(pdfdriver,"fontdir", filename);
    return PY_NONE;
}

static PyMethodDef pdf2swf_methods[] =
{
    /* sources */
    {"open", (PyCFunction)f_open, METH_KEYWORDS, f_open_doc},
    {"addfont", (PyCFunction)f_addfont, METH_KEYWORDS, f_addfont_doc},
    {"addfontdir", (PyCFunction)f_addfontdir, METH_KEYWORDS, f_addfontdir_doc},
    {"setparameter", (PyCFunction)f_setparameter, METH_KEYWORDS, f_setparameter_doc},
    {"verbose", (PyCFunction)f_verbose, METH_KEYWORDS, f_verbose_doc},

    /* devices */
    {"SWF", (PyCFunction)f_createSWF, METH_KEYWORDS, f_createSWF_doc},
    {"OCR", (PyCFunction)f_createOCR, METH_KEYWORDS, f_createOCR_doc},
    {"ImageList", (PyCFunction)f_createImageList, METH_KEYWORDS, f_createImageList_doc},
    {"PlainText", (PyCFunction)f_createPlainText, METH_KEYWORDS, f_createPlainText_doc},
    {"PassThrough", (PyCFunction)f_createPassThrough, METH_KEYWORDS, f_createPassThrough_doc},
#ifdef USE_OPENGL
    {"OpenGL", (PyCFunction)f_createOpenGL, METH_KEYWORDS, f_createOpenGL_doc},
#endif

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

void initgfx(void)
{
    initLog(0,0,0,0,0,2);
    OutputClass.ob_type = &PyType_Type;
    PageClass.ob_type = &PyType_Type;
    DocClass.ob_type = &PyType_Type;

    pdfdriver = gfxsource_pdf_create();
    swfdriver = gfxsource_swf_create();
    imagedriver = gfxsource_image_create();
    
    PyObject*module = Py_InitModule3("gfx", pdf2swf_methods, gfx_doc);
    PyObject*module_dict = PyModule_GetDict(module);

    PyDict_SetItemString(module_dict, "Doc", (PyObject*)&DocClass);
    PyDict_SetItemString(module_dict, "Page", (PyObject*)&PageClass);
    PyDict_SetItemString(module_dict, "Output", (PyObject*)&OutputClass);
}
