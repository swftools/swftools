/* tagmap.c

   Python wrapper for librfxswf.

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
#include "pyutils.h"
#include "tagmap.h"

typedef struct {
    PyObject_HEAD
    PyObject* obj2id;
    PyObject* id2obj;
    PyObject* objlist;
    int currentID;
} TagMapObject;

//----------------------------------------------------------------------------
PyObject* tagmap_new()
{
    PyObject* self = (PyObject*)PyObject_New(TagMapObject, &TagMapClass);
    mylog("+%08x(%d) tagmap_new", (int)self, self->ob_refcnt);
    TagMapObject*tagmap = (TagMapObject*)self;
    tagmap->obj2id = PyDict_New();
    tagmap->id2obj = PyDict_New();
    tagmap->objlist = PyList_New(0);
    tagmap->currentID = 0; //IDs start at 1
    return self;
}

//----------------------------------------------------------------------------
int tagmap_obj2id(PyObject* self, PyObject* obj)
{
    mylog(" %08x(%d) tagmap_obj2id %08x", (int)self, self->ob_refcnt, obj);
    TagMapObject*tagmap = (TagMapObject*)self;
    PyObject*id = PyDict_GetItem(tagmap->obj2id, obj);
    if(id == 0)
	return -1;
    int _id = PyLong_AsLong(id);
    Py_DECREF(id);
    return _id;
}

//----------------------------------------------------------------------------
PyObject* tagmap_id2obj(PyObject* self, int _id)
{
    TagMapObject*tagmap = (TagMapObject*)self;
    PyObject*id = PyLong_FromLong(_id);
    PyObject*obj = PyDict_GetItem(tagmap->id2obj, id);
    mylog(" %08x(%d) tagmap_id2obj %d->%08x", (int)self, self->ob_refcnt, _id, obj);
    Py_DECREF(id);
    return obj;
}
//----------------------------------------------------------------------------
int tagmap_getFreeID(PyObject*self)
{
    mylog(" %08x(%d) tagmap_getFreeID", (int)self, self->ob_refcnt);
    TagMapObject*tagmap = (TagMapObject*)self;
    int last = tagmap->currentID;
    do {
	tagmap->currentID++;
	PyObject*id = PyLong_FromLong(tagmap->currentID);
	PyObject*test = PyDict_GetItem(tagmap->id2obj,id);
	Py_DECREF(id);
	if(test == 0) {
	    PyErr_Clear();
	    mylog(" %08x(%d) tagmap_getFreeID -> %d", (int)self, self->ob_refcnt, tagmap->currentID);
	    return tagmap->currentID;
	}
    } while(last != tagmap->currentID);
    mylog(" %08x(%d) tagmap_getFreeID -> -1", (int)self, self->ob_refcnt);
    return -1;
}
//----------------------------------------------------------------------------
static void tagmap_add_mapping(PyObject*self, int id, PyObject* obj)
{
    TagMapObject*tagmap = (TagMapObject*)self;
    PyList_Append(tagmap->objlist, obj);//Py_INCREF(obj); done by PyList_Append
    PyObject*id_obj = PyLong_FromLong(id);
    PyDict_SetItem(tagmap->obj2id, obj, id_obj);//Py_INCREF(id_obj);Py_INCREF(obj); done by PyDict_SetItem
    PyDict_SetItem(tagmap->id2obj, id_obj, obj);//Py_INCREF(id_obj);Py_INCREF(obj); done by PyDict_SetItem
    Py_DECREF(id_obj);
}
//----------------------------------------------------------------------------
void tagmap_addMapping(PyObject*self, int id, PyObject* obj)
{
    TagMapObject*tagmap = (TagMapObject*)self;
    int id2 = tagmap_obj2id(self, obj);
    if(id2>=0) {
	assert(id==id2);
	return;
    }
    tagmap_add_mapping(self, id, obj);
}
//----------------------------------------------------------------------------
int tagmap_add(PyObject* self, PyObject* obj)
{
    TagMapObject*tagmap = (TagMapObject*)self;
    int id = tagmap_obj2id(self, obj);
    if(id>=0) {
	mylog(" %08x(%d) tagmap_add %08x->%d (again)", (int)self, self->ob_refcnt, (int)obj, id);
	return id;
    }
    id = tagmap_getFreeID(self);
    
    tagmap_add_mapping(self, id, obj);

    mylog(" %08x(%d) tagmap_add %08x->%d", (int)self, self->ob_refcnt, (int)obj, id);
    return id;
}

//----------------------------------------------------------------------------
void tagmap_dealloc(PyObject* self)
{
    mylog("-%08x(%d) tagmap_dealloc", (int)self, self->ob_refcnt);
    TagMapObject*tagmap = (TagMapObject*)self;
    Py_DECREF(tagmap->obj2id);
    tagmap->obj2id = 0;
    Py_DECREF(tagmap->id2obj);
    tagmap->id2obj = 0;
    Py_DECREF(tagmap->objlist);
    tagmap->objlist = 0;
    PyObject_Del(self);
}
//----------------------------------------------------------------------------
PyObject* tagmap_getObjectList(PyObject* self)
{
    mylog(" %08x(%d) tagmap_getObjectList", (int)self, self->ob_refcnt);
    TagMapObject*tagmap = (TagMapObject*)self;
    Py_INCREF(tagmap->objlist);
    return tagmap->objlist;
}
//----------------------------------------------------------------------------
PyTypeObject TagMapClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "TagMap",
    tp_basicsize: sizeof(TagMapObject),
    tp_itemsize: 0,
    tp_dealloc: tagmap_dealloc,
    tp_print: 0,
    tp_getattr: 0,
    tp_setattr: 0,
};
