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


typedef struct {
    PyObject_HEAD
    ActionTAG*action;
} ActionObject;

PyObject* f_Action(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"code", NULL};
    ActionObject* action;
    char*code = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &code))
	return NULL;
    action = PyObject_New(ActionObject, &ActionClass);
    action->action = 0;
    return (PyObject*)action;
}
static PyObject* action_getattr(PyObject * self, char* a)
{
    ActionObject*action = (ActionObject*)self;
/*    if(!strcmp(a, "r")) {
	return Py_BuildValue("r", action->rgba.r);
    } else if(!strcmp(a, "g")) {
	return Py_BuildValue("g", action->rgba.g);
    } else if(!strcmp(a, "b")) {
	return Py_BuildValue("b", action->rgba.b);
    } else if(!strcmp(a, "a")) {
	return Py_BuildValue("a", action->rgba.a);
    }*/
    return NULL;
}
static int action_setattr(PyObject * self, char* attr, PyObject* o)
{
    ActionObject*action = (ActionObject*)self;
/*    if(!strcmp(attr, "r")) {
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
    mylog("swf_setattr %08x(%d) %s = ? (%08x)\n", (int)self, self->ob_refcnt, attr, o);*/
    return 1;
}

ActionTAG* action_getAction(PyObject*self)
{
    ActionObject*action= 0;
    if (!PyArg_Parse(self, "O!", &ActionClass, &action)) {
	return 0;
    }
    return action->action;
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

static PyMethodDef action_methods[] = 
{
    {"Action", (PyCFunction)f_Action, METH_KEYWORDS, "Create a new action object."},
    {NULL, NULL, 0, NULL}
};

PyMethodDef* action_getMethods()
{
    ActionClass.ob_type = &PyType_Type;
    return action_methods;
}

