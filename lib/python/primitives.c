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

PyObject* f_Color(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"r", "g", "b", "a", NULL};
    ColorObject* color;
    int r=0,g=0,b=0,a=255;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iii|i", kwlist, &r,&g,&b,&a))
	return NULL;
    color = PyObject_New(ColorObject, &ColorClass);
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
//----------------------------------------------------------------------------
PyObject* f_BBox(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"xmin", "ymin", "xmax", "ymax", NULL};
    BBoxObject* bbox;
    SRECT box;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iiii", kwlist, 
		&box.xmin,
		&box.ymin,
		&box.xmax,
		&box.ymax));
	return NULL;
    bbox = PyObject_New(BBoxObject, &BBoxClass);
    bbox->bbox = box;
    return (PyObject*)bbox;
}
static PyObject* bbox_getattr(PyObject * self, char* a)
{
    BBoxObject*bbox = (BBoxObject*)self;
    if(!strcmp(a, "xmin")) {
	return Py_BuildValue("i", bbox->bbox.xmin);
    } else if(!strcmp(a, "ymin")) {
	return Py_BuildValue("i", bbox->bbox.ymin);
    } else if(!strcmp(a, "xmax")) {
	return Py_BuildValue("i", bbox->bbox.xmax);
    } else if(!strcmp(a, "ymax")) {
	return Py_BuildValue("i", bbox->bbox.ymax);
    }
    return NULL;
}
static int bbox_setattr(PyObject * self, char* a, PyObject* o)
{
    BBoxObject*bbox= (BBoxObject*)self;
    if(!strcmp(a, "xmin")) {
	if (!PyArg_Parse(o, "i", &bbox->bbox.xmin)) goto err;
	return 0;
    } else if(!strcmp(a, "ymin")) {
	if (!PyArg_Parse(o, "i", &bbox->bbox.ymin)) goto err;
	return 0;
    } else if(!strcmp(a, "xmax")) {
	if (!PyArg_Parse(o, "i", &bbox->bbox.xmax)) goto err;
	return 0;
    } else if(!strcmp(a, "ymax")) {
	if (!PyArg_Parse(o, "i", &bbox->bbox.ymax)) goto err;
	return 0;
    } 
err:
    mylog("swf_setattr %08x(%d) %s = ? (%08x)\n", (int)self, self->ob_refcnt, a, o);
    return 1;
}
//----------------------------------------------------------------------------
PyObject* f_Matrix(PyObject* self, PyObject* args, PyObject* kwargs)
{
    return NULL;
}
static PyObject* matrix_getattr(PyObject * self, char* a)
{
    return NULL;
}
static int matrix_setattr(PyObject * self, char* a, PyObject* o)
{
    return 0;
}
//----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------

PyTypeObject ColorClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "Color",
    tp_basicsize: sizeof(ColorObject),
    tp_itemsize: 0,
    tp_dealloc: dummy_dealloc,
    tp_print: 0,
    tp_getattr: color_getattr,
    tp_setattr: color_setattr,
};
PyTypeObject BBoxClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "BBox",
    tp_basicsize: sizeof(BBoxObject),
    tp_itemsize: 0,
    tp_dealloc: dummy_dealloc,
    tp_print: 0,
    tp_getattr: bbox_getattr,
    tp_setattr: bbox_setattr,
};
PyTypeObject GradientClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "Gradient",
    tp_basicsize: sizeof(GradientObject),
    tp_itemsize: 0,
    tp_dealloc: dummy_dealloc,
    tp_print: 0,
    tp_getattr: gradient_getattr,
    tp_setattr: gradient_setattr,
};
PyTypeObject CXFormClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "ColorTransform",
    tp_basicsize: sizeof(CXFormObject),
    tp_itemsize: 0,
    tp_dealloc: dummy_dealloc,
    tp_print: 0,
    tp_getattr: colortransform_getattr,
    tp_setattr: colortransform_setattr,
};
PyTypeObject MatrixClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "Matrix",
    tp_basicsize: sizeof(MatrixObject),
    tp_itemsize: 0,
    tp_dealloc: dummy_dealloc,
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
