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

PyObject* f_Color2(U8 r, U8 g, U8 b, U8 a)
{
    ColorObject* color = PyObject_New(ColorObject, &ColorClass);
    color->rgba.r = r;
    color->rgba.g = g;
    color->rgba.b = b;
    color->rgba.a = a;
    return (PyObject*)color;
}
PyObject* f_Color(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"r", "g", "b", "a", NULL};
    ColorObject* color;
    int r=0,g=0,b=0,a=255;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iii|i", kwlist, &r,&g,&b,&a)) {
	char*s= 0;
	int mya = -1;
	PyErr_Clear();
	static char *kwlist[] = {"col", "alpha", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|i", kwlist, &s, &mya))
	    return NULL;
	if(mya>=0) a=mya;
	sscanf(s, "%02x%02x%02x%02x",&r,&g,&b,&a);
    }
    color = PyObject_New(ColorObject, &ColorClass);
    mylog("+%08x(%d) color_new(%d,%d,%d,%d)\n", (int)color, color->ob_refcnt, r,g,b,a);
    return f_Color2(r,g,b,a);
}
static PyObject* color_getattr(PyObject * self, char* a)
{
    ColorObject*color = (ColorObject*)self;
    if(!strcmp(a, "r")) {
	return Py_BuildValue("i", color->rgba.r);
    } else if(!strcmp(a, "g")) {
	return Py_BuildValue("i", color->rgba.g);
    } else if(!strcmp(a, "b")) {
	return Py_BuildValue("i", color->rgba.b);
    } else if(!strcmp(a, "a")) {
	return Py_BuildValue("i", color->rgba.a);
    } else if(!strcmp(a, "alpha")) {
	return Py_BuildValue("i", color->rgba.a);
    } else if(!strcmp(a, "rgb")) {
	char text[80];
	sprintf(text, "%02x%02x%02x", color->rgba.r, color->rgba.g, color->rgba.b);
	return PyString_FromString(text);
    } else if(!strcmp(a, "rgba")) {
	char text[80];
	sprintf(text, "%02x%02x%02x%02x", color->rgba.r, color->rgba.g, color->rgba.b, color->rgba.a);
	return PyString_FromString(text);
    }
    return PY_ERROR("bad attribute");
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
//void swf_ExpandRect(SRECT*src, SPOINT add);
//void swf_ExpandRect2(SRECT*src, SRECT*add);

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
PyObject* f_BBox2(SRECT box)
{
    BBoxObject* bbox;
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
	if (!PyArg_Parse(o, "f", &xmin)) goto err;
	bbox->bbox.xmin = (int)(xmin*20);
	return 0;
    } else if(!strcmp(a, "ymin")) {
	float ymin;
	if (!PyArg_Parse(o, "f", &ymin)) goto err;
	bbox->bbox.ymin = (int)(ymin*20);
	return 0;
    } else if(!strcmp(a, "xmax")) {
	float xmax;
	if (!PyArg_Parse(o, "f", &xmax)) goto err;
	bbox->bbox.xmax = (int)(xmax*20);
	return 0;
    } else if(!strcmp(a, "ymax")) {
	float ymax;
	if (!PyArg_Parse(o, "f", &ymax)) goto err;
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

PyObject* f_Matrix2(MATRIX* m)
{
    PyObject*self = (PyObject*)PyObject_New(MatrixObject, &MatrixClass);
    MatrixObject*matrix = (MatrixObject*)self;
    matrix->matrix = *m;
    return self;
}

PyObject* f_Matrix(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    PyObject*self = (PyObject*)PyObject_New(MatrixObject, &MatrixClass);
    MatrixObject*matrix = (MatrixObject*)self;
    mylog("+%08x(%d) f_Matrix", self, self->ob_refcnt);
    static char *kwlist[] = {"x", "y", "scale", "rotate", "pivotx", "pivoty", NULL};
    float x=0,y=0,scale=1.0,rotate=0,pivotx=0,pivoty=0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|ffffff", kwlist, &x,&y,&scale,&rotate,&pivotx,&pivoty))
	return NULL;
    mylog(" %08x(%d) f_Matrix: x=%f y=%f scale=%f rotate=%f", self, self->ob_refcnt, x,y,scale,rotate);
    swf_GetMatrix(0, &matrix->matrix);

    matrix->matrix.tx = (int)(x*20);
    matrix->matrix.ty = (int)(y*20);

    if(!rotate) {
    	matrix->matrix.sx = (int)(scale*65536);
    	matrix->matrix.sy = (int)(scale*65536);
    } else {
	matrix->matrix.sx = (int)(scale*cos(rotate)*65536);
	matrix->matrix.sy = (int)(scale*cos(rotate)*65536);
	matrix->matrix.r0 = (int)(scale*sin(rotate)*65536);
	matrix->matrix.r1 = (int)(-scale*sin(rotate)*65536);
    }
    if(pivotx || pivoty) {
	SPOINT p,d;
	p.x = (int)(pivotx*20);
	p.y = (int)(pivoty*20);
	p = swf_TurnPoint(p, &matrix->matrix);
	matrix->matrix.tx += matrix->matrix.tx-p.x;
	matrix->matrix.ty += matrix->matrix.ty-p.y;
    }

    /* TODO: rotate */
    return self;
}
static PyObject* matrix_getattr(PyObject * self, char* a)
{
    PY_ASSERT_TYPE(self,&MatrixClass);
    MatrixObject*matrix = (MatrixObject*)self;
    if(!strcmp(a, "entries")) {
	return Py_BuildValue("(ffffff)",
		matrix->matrix.sx/65536.0,
		matrix->matrix.r0/65536.0,
		matrix->matrix.r1/65536.0,
		matrix->matrix.sy/65536.0,
		matrix->matrix.tx/20.0,
		matrix->matrix.ty/20.0
		);
    }
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
//SPOINT swf_TurnPoint(SPOINT p, MATRIX* m);
//SRECT swf_TurnRect(SRECT r, MATRIX* m);
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

PyObject* f_ColorTransform(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    int r0=256,g0=256,b0=256,a0=256,r1=0,g1=0,b1=0,a1=0;
    static char *kwlist[] = {"r_mul", "g_mul", "b_mul", "a_mul", "r_add", "g_add", "b_add", "a_add", NULL};
    PyObject*color;
    if(!kwargs) {
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|iiiiiiii", kwlist,
		&r0,&g0,&b0,&a0,
		&r1,&g1,&b1,&a1))
	    return NULL;
    }

    CXFORM c;
    c.r0 = r0; c.g0 = g0; c.b0 = b0; c.a0 = a0;
    c.r1 = r1; c.g1 = g1; c.b1 = b1; c.a1 = a1;

    CXFormObject*self = PyObject_New(CXFormObject, &CXFormClass);
    self->cxform = c;
    return (PyObject*)self;
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

typedef struct {
    PyObject_HEAD
    LINESTYLE ls;
} LineStyleObject;

PyObject* f_LineStyle2(RGBA color, int width) 
{
    LineStyleObject* self = PyObject_New(LineStyleObject, &LineStyleClass);
    self->ls.color = color;
    self->ls.width = width;
    return (PyObject*)self;
}
PyObject* f_LineStyle3(LINESTYLE ls)
{
    LineStyleObject* self = PyObject_New(LineStyleObject, &LineStyleClass);
    self->ls = ls;
    return (PyObject*)self;
}
PyObject* f_LineStyle(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"line", "color", NULL};
    float linewidth;
    PyObject*color;
    if(!kwargs) {
	if (!PyArg_ParseTuple(args, "fO!", &linewidth, &ColorClass, &color))
	    return NULL;
    }
    return f_LineStyle2(color_getRGBA(color), (int)(linewidth*20));
}
LINESTYLE linestyle_getLineStyle(PyObject*_self)
{
    LineStyleObject* self = (LineStyleObject*)_self;
    return self->ls;
}
static PyObject* linestyle_getattr(PyObject * _self, char* a)
{
    LineStyleObject*self = (LineStyleObject*)_self;
    if(!strcmp(a, "width")) {
	return Py_BuildValue("i", self->ls.width);
    } else if(!strcmp(a, "color")) {
	return f_Color2(self->ls.color.r, self->ls.color.g, self->ls.color.b, self->ls.color.a);
    }
    return NULL;
}
static int linestyle_setattr(PyObject * _self, char* a, PyObject* o)
{
    LineStyleObject*self = (LineStyleObject*)_self;
    if(!strcmp(a, "color")) {
	self->ls.color = color_getRGBA(o);
	return 0;
    }
    return -1;
}
static LINESTYLE linestyle_getlinestyle(PyObject*_self)
{
    LineStyleObject*self = (LineStyleObject*)_self;
    return self->ls;
}
static void linestyle_dealloc(PyObject* self)
{
    mylog("-%08x(%d) linestyle_dealloc", self, self->ob_refcnt);
    PyObject_Del(self);
}
static int linestyle_print(PyObject * _self, FILE *fi, int flags) //flags&Py_PRINT_RAW
{
    LineStyleObject* self = (LineStyleObject*)_self;
    fprintf(fi, "line-%d-%02x%02x%02x%02x", self->ls.width, self->ls.color.r, self->ls.color.g, self->ls.color.b, self->ls.color.a);
    return 0;
}
PyTypeObject LineStyleClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "linestyle",
    tp_basicsize: sizeof(LineStyleObject),
    tp_itemsize: 0,
    tp_dealloc: linestyle_dealloc,
    tp_print: linestyle_print,
    tp_getattr: linestyle_getattr,
    tp_setattr: linestyle_setattr,
};
//----------------------------------------------------------------------------

typedef struct {
    PyObject_HEAD
    FILLSTYLE fs;
} FillStyleObject;

PyObject* f_FillStyle2(FILLSTYLE fs)
{
    FillStyleObject* self = PyObject_New(FillStyleObject, &FillStyleClass);
    self->fs = fs;
    return (PyObject*)self;
}
PyObject* f_SolidFillStyle2(RGBA color)
{
    FillStyleObject* self = PyObject_New(FillStyleObject, &FillStyleClass);
    self->fs.type = FILL_SOLID;
    self->fs.color = color;
    return (PyObject*)self;
}
PyObject* f_SolidFillStyle(PyObject* _self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"color", NULL};
    PyObject*color;
    if(!kwargs) {
	if (!PyArg_ParseTuple(args, "O!", &ColorClass, &color))
	    return NULL;
    }
    return f_SolidFillStyle2(color_getRGBA(color));
}
FILLSTYLE fillstyle_getFillStyle(PyObject*_self)
{
    FillStyleObject* self = (FillStyleObject*)_self;
    return self->fs;
}
static void fillstyle_dealloc(PyObject* self)
{
    mylog("-%08x(%d) linestyle_dealloc", self, self->ob_refcnt);
    PyObject_Del(self);
}
static int fillstyle_print(PyObject * _self, FILE *fi, int flags) //flags&Py_PRINT_RAW
{
    FillStyleObject* self = (FillStyleObject*)_self;
    if(self->fs.type == FILL_SOLID)
	fprintf(fi, "fill-solid(%02x%02x%02x%02x)", self->fs.color.r, self->fs.color.g, self->fs.color.b, self->fs.color.a);
    else
	fprintf(fi, "fill-%02x", self->fs.type);
    return 0;
}
PyObject* fillstyle_issolid(PyObject*_self, PyObject*args)
{
    FillStyleObject* self = (FillStyleObject*)_self;
    int b = self->fs.type == FILL_SOLID;
    return PyInt_FromLong(b);
}
static PyMethodDef FillStyleMethods[] = 
{
    /* Module functions */
    {"isSolid", fillstyle_issolid, METH_VARARGS, "Queries whether this is a solid fill"},
    {0,0,0,0}
};
static PyObject* fillstyle_getattr(PyObject * _self, char* a)
{
    FillStyleObject* self = (FillStyleObject*)_self;
    if(!strcmp(a, "color")) {
	return f_Color2(self->fs.color.r, self->fs.color.g, self->fs.color.b, self->fs.color.a);
    }
    return Py_FindMethod(FillStyleMethods, _self, a);
}
static int fillstyle_setattr(PyObject * _self, char* a, PyObject* o)
{
    FillStyleObject*self = (FillStyleObject*)_self;
    if(!strcmp(a, "color")) {
	self->fs.color = color_getRGBA(o);
	return 0;
    }
    return -1;
}

PyTypeObject FillStyleClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "fillstyle",
    tp_basicsize: sizeof(FillStyleObject),
    tp_itemsize: 0,
    tp_dealloc: fillstyle_dealloc,
    tp_print: fillstyle_print,
    tp_getattr: fillstyle_getattr,
    tp_setattr: fillstyle_setattr,
};
//----------------------------------------------------------------------------
static PyMethodDef primitive_methods[] = 
{
    {"Color", (PyCFunction)f_Color, METH_KEYWORDS, "Create a new color object."},
    {"Gradient", (PyCFunction)f_Gradient, METH_KEYWORDS, "Create a new gradient object."},
    {"ColorTransform", (PyCFunction)f_ColorTransform, METH_KEYWORDS, "Create a new colortransform object."},
    {"Matrix", (PyCFunction)f_Matrix, METH_KEYWORDS, "Create a new matrix object."},
    {"BBox", (PyCFunction)f_BBox, METH_KEYWORDS, "Create a new bounding box object."},
    {"SolidFillStyle", (PyCFunction)f_SolidFillStyle, METH_KEYWORDS, "Creates a new solid fill style."},
    {"LineStyle", (PyCFunction)f_SolidFillStyle, METH_KEYWORDS, "Creates a new line style."},
    {NULL, NULL, 0, NULL}
};

PyMethodDef* primitive_getMethods()
{
    GradientClass.ob_type = &PyType_Type;
    CXFormClass.ob_type = &PyType_Type;
    BBoxClass.ob_type = &PyType_Type;
    MatrixClass.ob_type = &PyType_Type;
    FillStyleClass.ob_type = &PyType_Type;
    LineStyleClass.ob_type = &PyType_Type;
    return primitive_methods;
}


