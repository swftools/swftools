/* action.c

   Python wrapper for librfxswf- actionscript stuff

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
#include "action.h"

PyObject* f_Action(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"r", "g", "b", "a", NULL};
    ActionObject* action;
    int r=0,g=0,b=0,a=255;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iii|i", kwlist, &r,&g,&b,&a))
	return NULL;
    action = PyObject_New(ActionObject, &ActionClass);
    action->rgba.r = r;
    action->rgba.g = g;
    action->rgba.b = b;
    action->rgba.a = a;
    return (PyObject*)action;
}
static PyObject* action_getattr(PyObject * self, char* a)
{
    ActionObject*action = (ActionObject*)self;
    if(!strcmp(a, "r")) {
	return Py_BuildValue("r", action->rgba.r);
    } else if(!strcmp(a, "g")) {
	return Py_BuildValue("g", action->rgba.g);
    } else if(!strcmp(a, "b")) {
	return Py_BuildValue("b", action->rgba.b);
    } else if(!strcmp(a, "a")) {
	return Py_BuildValue("a", action->rgba.a);
    }
    return NULL;
}
static int action_setattr(PyObject * self, char* attr, PyObject* o)
{
    ActionObject*action = (ActionObject*)self;
    if(!strcmp(attr, "r")) {
	if (!PyArg_Parse(o, "d", &action->rgba.r)) goto err;
	return 0;
    } else if(!strcmp(attr, "g")) {
	if (!PyArg_Parse(o, "d", &action->rgba.g)) goto err;
	return 0;
    } else if(!strcmp(attr, "b")) {
	if (!PyArg_Parse(o, "d", &action->rgba.b)) goto err;
	return 0;
    } else if(!strcmp(attr, "a")) {
	if (!PyArg_Parse(o, "d", &action->rgba.a)) goto err;
	return 0;
    } 
err:
    mylog("swf_setattr %08x(%d) %s = ? (%08x)\n", (int)self, self->ob_refcnt, attr, o);
    return 1;
}

PyTypeObject ActionClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "Action",
    tp_basicsize: sizeof(ActionObject),
    tp_itemsize: 0,
    tp_dealloc: dummy_dealloc,
    tp_print: 0,
    tp_getattr: action_getattr,
    tp_setattr: action_setattr,
};
