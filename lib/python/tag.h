/* tag.h

   Python wrapper for librfxswf- tag handlers

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

#ifndef __tag_h__
#define __tag_h__

#include <Python.h>
#undef HAVE_STAT
#include "../rfxswf.h"

#include "tagmap.h"

typedef struct _tag_internals
{
    int (*parse)(struct _tag_internals*);
    int (*fillTAG)(struct _tag_internals*);
    void (*dealloc)(struct _tag_internals*);
    int datasize;
    void*data;
    PyObject* (*getattr)(struct _tag_internals*, char* a);
    int (*setattr)(struct _tag_internals*, char* a, PyObject*val);
    PyMethodDef* tagfunctions;
    TAG*tag;
    PyObject* tagmap;
} tag_internals_t;

PyObject* tag_new(tag_internals_t*tag_internals);
PyObject* tag_new2(TAG*_tag, PyObject* tagmap);
TAG* tag_getTAG(PyObject*self, TAG*prevTag, PyObject*tagmap);
PyObject* tag_getDependencies(PyObject*self);
tag_internals_t* tag_getinternals(PyObject*tag);
void register_tag(int id, tag_internals_t*spec);

PyMethodDef* tag_getMethods();

extern PyTypeObject TagClass;

#endif
