/* primitives.c

   Python wrapper for librfxswf- primitive objects (implementation)

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
#undef HAVE_STAT
#include "../rfxswf.h"
#include "../log.h"
#include "./pyutils.h"
#include "primitives.h"

//----------------------------------------------------------------------------
typedef struct {
    PyObject_HEAD
    RGBA rgba;
} ColorObject;

PyObject* f_Color(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"r", "g", "b", "a", NULL};
    ColorObject* color;
    int r=0,g=0,b=0,a=255;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iii|i", kwlist, &r,&g,&b,&a))
	return NULL;
    color = PyObject_New(ColorObject, &ColorClass);
    mylog("+%08x(%d) color_new(%d,%d,%d,%d)\n", (int)color, color->ob_refcnt, r,g,b,a);
    color->rgba.r = r;
    color->rgba.g = g;
    color->rgba.b = b;
    color->rgba.a = a;
    return (PyObject*)color;
}
static PyObject* color_getattr(PyObject * self, char* a)
{
    ColorObject*color = (ColorObject*)self;
    if(!strcmp(a, "r")) {
	return Py_BuildValue("r", color->rgba.r);
    } else if(!strcmp(a, "g")) {
	return Py_BuildValue("g", color->rgba.g);
    } else if(!strcmp(a, "b")) {
	return Py_BuildValue("b", color->rgba.b);
    } else if(!strcmp(a, "a")) {
	return Py_BuildValue("a", color->rgba.a);
    }
    return NULL;
}
static int color_setattr(PyObject * self, char* attr, PyObject* o)
{
    ColorObject*color = (ColorObject*)self;
    if(!strcmp(attr, "r")) {
	if (!PyArg_Parse(o, "d", &color->rgba.r)) goto err;
	return 0;
    } else if(!strcmp(attr, "g")) {
	if (!PyArg_Parse(o, "d", &color->rgba.g)) goto err;
	return 0;
    } else if(!strcmp(attr, "b")) {
	if (!PyArg_Parse(o, "d", &color->rgba.b)) goto err;
	return 0;
    } else if(!strcmp(attr, "a")) {
	if (!PyArg_Parse(o, "d", &color->rgba.a)) goto err;
	return 0;
    } 
err:
    mylog("swf_setattr %08x(%d) %s = ? (%08x)\n", (int)self, self->ob_refcnt, attr, o);
    return 1;
}
RGBA color_getRGBA(PyObject*self)
{
    ColorObject*color = 0;
    if (!PyArg_Parse(self, "O!", &ColorClass, &color)) {
	RGBA dummy;
	memset(&dummy, 0, sizeof(dummy));
	mylog("Error: wrong type for function color_getRGBA");
	return dummy;
    }
    return color->rgba;
}
void color_dealloc(PyObject* self)
{
    mylog("-%08x(%d) color_dealloc\n", (int)self, self->ob_refcnt);
    PyObject_Del(self);
}
PyTypeObject ColorClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "Color",
    tp_basicsize: sizeof(ColorObject),
    tp_itemsize: 0,
    tp_dealloc: color_dealloc,
    tp_print: 0,
    tp_getattr: color_getattr,
    tp_setattr: color_setattr,
};
//----------------------------------------------------------------------------
typedef struct {
    PyObject_HEAD
    SRECT bbox;
} BBoxObject;

PyObject* f_BBox(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"xmin", "ymin", "xmax", "ymax", NULL};
    BBoxObject* bbox;
    float xmin,ymin,xmax,ymax;
    if(!kwargs) {
	if (!PyArg_ParseTuple(args, "ffff", &xmin, &ymin, &xmax, &ymax))
	    return NULL;
    } else {
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ffff", kwlist, &xmin, &ymin, &xmax, &ymax))
	    return NULL;
    }
    SRECT box;
    box.xmin = (int)(xmin*20);
    box.ymin = (int)(ymin*20);
    box.xmax = (int)(xmax*20);
    box.ymax = (int)(ymax*20);
    mylog("+%08x(%d) bbox_new(%d,%d,%d,%d)\n", (int)self, self?self->ob_refcnt:0, box.xmin, box.ymin, box.xmax,box.ymax);
    bbox = PyObject_New(BBoxObject, &BBoxClass);
    bbox->bbox = box;
    return (PyObject*)bbox;
}
static PyObject* bbox_getattr(PyObject * self, char* a)
{
    BBoxObject*bbox = (BBoxObject*)self;
    if(!strcmp(a, "xmin")) {
	return Py_BuildValue("f", bbox->bbox.xmin/20.0);
    } else if(!strcmp(a, "ymin")) {
	return Py_BuildValue("f", bbox->bbox.ymin/20.0);
    } else if(!strcmp(a, "xmax")) {
	return Py_BuildValue("f", bbox->bbox.xmax/20.0);
    } else if(!strcmp(a, "ymax")) {
	return Py_BuildValue("f", bbox->bbox.ymax/20.0);
    }
    return NULL;
}
static int bbox_setattr(PyObject * self, char* a, PyObject* o)
{
    BBoxObject*bbox= (BBoxObject*)self;
    if(!strcmp(a, "xmin")) {
	float xmin;
	if (!PyArg_Parse(o, "i", &xmin)) goto err;
	bbox->bbox.xmin = (int)(xmin*20);
	return 0;
    } else if(!strcmp(a, "ymin")) {
	float ymin;
	if (!PyArg_Parse(o, "i", &ymin)) goto err;
	bbox->bbox.ymin = (int)(ymin*20);
	return 0;
    } else if(!strcmp(a, "xmax")) {
	float xmax;
	if (!PyArg_Parse(o, "i", &xmax)) goto err;
	bbox->bbox.xmax = (int)(xmax*20);
	return 0;
    } else if(!strcmp(a, "ymax")) {
	float ymax;
	if (!PyArg_Parse(o, "i", &ymax)) goto err;
	bbox->bbox.ymax = (int)(ymax*20);
	return 0;
    } 
err:
    mylog("swf_setattr %08x(%d) %s = ? (%08x)\n", (int)self, self->ob_refcnt, a, o);
    return 1;
}
void bbox_dealloc(PyObject* self)
{
    mylog("-%08x(%d) bbox_dealloc\n", (int)self, self->ob_refcnt);
    PyObject_Del(self);
}
SRECT bbox_getSRECT(PyObject*self)
{
    BBoxObject*bbox= 0;
    if (!PyArg_Parse(self, "O!", &BBoxClass, &bbox)) {
	SRECT dummy;
	memset(&dummy, 0, sizeof(dummy));
	mylog("Error: wrong type for function color_getRGBA");
	return dummy;
    }
    return bbox->bbox;
}
PyTypeObject BBoxClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "BBox",
    tp_basicsize: sizeof(BBoxObject),
    tp_itemsize: 0,
    tp_dealloc: bbox_dealloc,
    tp_print: 0,
    tp_getattr: bbox_getattr,
    tp_setattr: bbox_setattr,
};
SRECT bbox_getBBox(PyObject*self);
//----------------------------------------------------------------------------
typedef struct {
    PyObject_HEAD
    MATRIX matrix;
} MatrixObject;

PyObject* f_Matrix(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    PyObject*self = (PyObject*)PyObject_New(MatrixObject, &MatrixClass);
    MatrixObject*matrix = (MatrixObject*)self;
    mylog("+%08x(%d) f_Matrix", self, self->ob_refcnt);
    static char *kwlist[] = {"x", "y", "scale", "rotate", NULL};
    float x=0,y=0,scale=1.0,rotate=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|ffff", kwlist, &x,&y,&scale,&rotate))
	return NULL;
    mylog(" %08x(%d) f_Matrix: x=%f y=%f scale=%f rotate=%f", self, self->ob_refcnt, x,y,scale,rotate);
    swf_GetMatrix(0, &matrix->matrix);
    matrix->matrix.tx = (int)(x*20);
    matrix->matrix.ty = (int)(y*20);
    matrix->matrix.sx = (int)(scale*65536);
    matrix->matrix.sy = (int)(scale*65536);
    /* TODO: rotate */
    return self;
}
static PyObject* matrix_getattr(PyObject * self, char* a)
{
    PY_ASSERT_TYPE(self,&MatrixClass);
    return NULL;
}
static int matrix_setattr(PyObject * self, char* a, PyObject* o)
{
    PY_ASSERT_TYPE(self,&MatrixClass);
    return 0;
}
MATRIX matrix_getMatrix(PyObject*self)
{
    mylog(" %08x(%d) matrix_getMatrix", self, self->ob_refcnt);
    PY_ASSERT_TYPE(self,&MatrixClass);
    MatrixObject*matrix = (MatrixObject*)self;
    return matrix->matrix;
}
void matrix_dealloc(PyObject* self)
{
    mylog("-%08x(%d) matrix_dealloc", self, self->ob_refcnt);
    PyObject_Del(self);
}
PyTypeObject MatrixClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "Matrix",
    tp_basicsize: sizeof(MatrixObject),
    tp_itemsize: 0,
    tp_dealloc: matrix_dealloc,
    tp_print: 0,
    tp_getattr: matrix_getattr,
    tp_setattr: matrix_setattr,
    tp_compare: 0,
    tp_repr: 0,
    tp_as_number: 0,
    tp_as_sequence: 0,
    tp_as_mapping: 0,
    tp_hash: 0,	    // dict(x)
    tp_call: 0,     // x()
    tp_str: 0	    // str(x)
};
//----------------------------------------------------------------------------
typedef struct {
    PyObject_HEAD
    CXFORM cxform;
} CXFormObject;

PyObject* f_ColorTransform(PyObject* self, PyObject* args, PyObject* kwargs)
{
    return NULL;
}
static PyObject* colortransform_getattr(PyObject * self, char* a)
{
    return NULL;
}
static int colortransform_setattr(PyObject * self, char* a, PyObject* o)
{
    return 0;
}
CXFORM colortransform_getCXForm(PyObject*self)
{
    CXFormObject*cxform= 0;
    if (!PyArg_Parse(self, "O!", &CXFormClass, &cxform)) {
	CXFORM dummy;
	memset(&dummy, 0, sizeof(dummy));
	mylog("Error: wrong type for function color_getRGBA");
	return dummy;
    }
    return cxform->cxform;
}
void colortransform_dealloc(PyObject* self)
{
    mylog("-%08x(%d) colortransform_dealloc", self, self->ob_refcnt);
    PyObject_Del(self);
}
PyTypeObject CXFormClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "ColorTransform",
    tp_basicsize: sizeof(CXFormObject),
    tp_itemsize: 0,
    tp_dealloc: colortransform_dealloc,
    tp_print: 0,
    tp_getattr: colortransform_getattr,
    tp_setattr: colortransform_setattr,
};
//----------------------------------------------------------------------------
typedef struct {
    PyObject_HEAD
    GRADIENT gradient;
} GradientObject;

PyObject* f_Gradient(PyObject* self, PyObject* args, PyObject* kwargs)
{
    return NULL;
}
static PyObject* gradient_getattr(PyObject * self, char* a)
{
    return NULL;
}
static int gradient_setattr(PyObject * self, char* a, PyObject* o)
{
    return 0;
}
GRADIENT gradient_getGradient(PyObject*self)
{
    GradientObject*gradient = 0;
    if (!PyArg_Parse(self, "O!", &gradient, &gradient)) {
	GRADIENT dummy;
	memset(&dummy, 0, sizeof(dummy));
	mylog("Error: wrong type for function color_getRGBA");
	return dummy;
    }
    return gradient->gradient;
}
void gradient_dealloc(PyObject* self)
{
    mylog("-%08x(%d) gradient_dealloc", self, self->ob_refcnt);
    PyObject_Del(self);
}
PyTypeObject GradientClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "Gradient",
    tp_basicsize: sizeof(GradientObject),
    tp_itemsize: 0,
    tp_dealloc: gradient_dealloc,
    tp_print: 0,
    tp_getattr: gradient_getattr,
    tp_setattr: gradient_setattr,
};
//----------------------------------------------------------------------------

static PyMethodDef primitive_methods[] = 
{
    {"Color", (PyCFunction)f_Color, METH_KEYWORDS, "Create a new color object."},
    {"Gradient", (PyCFunction)f_Gradient, METH_KEYWORDS, "Create a new gradient object."},
    {"ColorTransform", (PyCFunction)f_ColorTransform, METH_KEYWORDS, "Create a new colortransform object."},
    {"Matrix", (PyCFunction)f_Matrix, METH_KEYWORDS, "Create a new matrix object."},
    {"BBox", (PyCFunction)f_BBox, METH_KEYWORDS, "Create a new bounding box object."},
    {NULL, NULL, 0, NULL}
};

PyMethodDef* primitive_getMethods()
{
    GradientClass.ob_type = &PyType_Type;
    CXFormClass.ob_type = &PyType_Type;
    BBoxClass.ob_type = &PyType_Type;
    MatrixClass.ob_type = &PyType_Type;
    return primitive_methods;
}


