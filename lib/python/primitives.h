/* primitives.h

   Python wrapper for librfxswf- primitive objects (structs, prototypes)

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

#ifndef __primitives_h__
#define __primitives_h__

#include <Python.h>
#undef HAVE_STAT

#include "../rfxswf.h"

/* exported to allow casting to this type */
extern PyTypeObject ColorClass;
extern PyTypeObject BBoxClass;
extern PyTypeObject CXFormClass;
extern PyTypeObject GradientClass;
extern PyTypeObject MatrixClass;
extern PyTypeObject LineStyleClass;
extern PyTypeObject FillStyleClass;

PyObject* f_Color(PyObject* self, PyObject* args, PyObject* kwargs);
PyObject* f_ColorTransform(PyObject* self, PyObject* args, PyObject* kwargs);
PyObject* f_Gradient(PyObject* self, PyObject* args, PyObject* kwargs);
PyObject* f_BBox(PyObject* self, PyObject* args, PyObject* kwargs);
PyObject* f_BBox2(SRECT bbox);
PyObject* f_Matrix(PyObject* self, PyObject* args, PyObject* kwargs);
PyObject* f_Matrix2(MATRIX* m);
PyObject* f_FillStyle2(FILLSTYLE fs);
PyObject* f_SolidFillstyle(PyObject* self, PyObject* args, PyObject* kwargs);
PyObject* f_SolidFillstyle2(RGBA color);
PyObject* f_LineStyle(PyObject* self, PyObject* args, PyObject* kwargs);
PyObject* f_LineStyle2(RGBA color, int width);
PyObject* f_LineStyle3(LINESTYLE ls);

RGBA color_getRGBA(PyObject*self);
CXFORM colortransform_getCXForm(PyObject*self);
GRADIENT gradient_getGradient(PyObject*self);
SRECT bbox_getSRECT(PyObject*self);
MATRIX matrix_getMatrix(PyObject*self);
FILLSTYLE fillstyle_getFillStyle(PyObject*self);
LINESTYLE linestyle_getLineStyle(PyObject*self);

extern PyMethodDef* primitive_getMethods();

#endif
