/* action.h

   Python wrapper for librfxswf- actionscript stuff (header)

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

#ifndef __tagmap_h__
#define __tagmap_h__

#include <Python.h>
#undef HAVE_STAT
#include "../rfxswf.h"

extern PyTypeObject TagMapClass;
int tagmap_obj2id(PyObject* self, PyObject* obj);
PyObject* tagmap_id2obj(PyObject* self, int _id);
int tagmap_add(PyObject* self, PyObject* obj);
void tagmap_addMapping(PyObject*self, int id, PyObject*obj);
PyObject* tagmap_getObjectList(PyObject* self);

PyObject* tagmap_new();

#endif
