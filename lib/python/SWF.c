/* SWF.c

   Python wrapper for librfxswf- module core.

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
#include "./tag.h"
#include "./taglist.h"

/*
TODO:
    1) taglist is rfxswflib's linked list. It should maybe implemented as Python
       list, which would, however, mean that we would have to convert the list
       back and forth for the following functions:
	load, save, writeCGI, unfoldAll, foldAll, optimizeOrder
    2) taglist should have an ID handler. Every time a tag is inserted, it's ID
       is stored in a lookup list.
    3) 
*/

//-------------------------- Types -------------------------------------------

staticforward PyTypeObject SWFClass;

/* Tags, Objects */

typedef struct {
    PyObject_HEAD
    SWF swf; //swf.firstTag ist not used
    PyObject*taglist;
    char*filename;
} SWFObject;


//----------------------------------------------------------------------------
static PyObject* f_create(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"version", "fps", "bbox", "name", NULL};
    SWFObject* swf;
    int version = 6;
    double framerate = 25;
    PyObject * obbox = 0;
    SRECT bbox = {0,0,0,0};
    char* filename = 0;
    
    swf = PyObject_New(SWFObject, &SWFClass);
    mylog("+%08x(%d) create\n", (int)swf, swf->ob_refcnt);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|idOs", 
		kwlist, &version, &framerate, 
		&obbox, filename))
	return NULL;

    if (!PyArg_Parse(obbox, "(iiii)", &bbox.xmin, &bbox.ymin, &bbox.xmax, &bbox.ymax))
	return NULL;

    memset(&swf->swf, 0, sizeof(SWF));
    if(filename)
	swf->filename = strdup(filename);
    else
	swf->filename = 0;

    swf->swf.fileVersion = version;
    swf->swf.frameRate = (int)(framerate*0x100);
    swf->swf.movieSize = bbox;
    swf->taglist = taglist_new();

    if(swf->swf.fileVersion>=6)
	swf->swf.compressed = 1;

    mylog(" %08x(%d) create: done\n", (int)swf, swf->ob_refcnt);
    return (PyObject*)swf;
}
//----------------------------------------------------------------------------
static PyObject* f_load(PyObject* self, PyObject* args)
{
    char* filename;
    SWFObject* swf;
    int fi;

    if (!PyArg_ParseTuple(args,"s:load", &filename)) 
	return NULL;

    swf = PyObject_New(SWFObject, &SWFClass);
    mylog("+%08x(%d) f_load\n", (int)swf, swf->ob_refcnt);

    memset(&swf->swf, 0, sizeof(SWF));
    swf->filename = strdup(filename);

    if(!filename) {
	PyErr_SetString(PyExc_Exception, setError("Couldn't open file %s", filename));
        return 0;
    }
    fi = open(filename,O_RDONLY|O_BINARY);
    if (fi<0) { 
        PyErr_SetString(PyExc_Exception, setError("Couldn't open file %s", filename));
	return 0;
    }
    if(swf_ReadSWF(fi,&swf->swf)<0) { 
        close(fi);
        PyErr_SetString(PyExc_Exception, setError("%s is not a valid SWF file or contains errors",filename));
	return 0;
    }
    close(fi);

    swf->taglist = taglist_new2(swf->swf.firstTag);
    if(swf->taglist == NULL) {
	return NULL;
    }
    
    swf_FreeTags(&swf->swf);
    swf->swf.firstTag = 0;
    
    return (PyObject*)swf;
}
//----------------------------------------------------------------------------
static PyObject * swf_save(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"name", "compress", NULL};
    SWFObject*swfo;
    SWF*swf;
    int fi;
    char*filename = 0;
    int compress = 0;

    if(!self)
	return NULL;

    swfo = (SWFObject*)self;
    swf = &swfo->swf;
    
    filename = swfo->filename;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|si", kwlist, &filename, &compress))
	return NULL;
    
    mylog(" %08x(%d) f_save filename=%s compress=%d\n", (int)self, self->ob_refcnt, filename, compress);

    // keyword arg compress (=1) forces compression
    if(compress)
	swf->compressed = 1;
    
    swf->firstTag = taglist_getTAGs(swfo->taglist);
    if(!swf->firstTag)
	return NULL;

    // fix the file, in case it is empty or not terminated properly
    {
	TAG*tag = swf->firstTag;
	if(!tag)
	    tag = swf->firstTag = swf_InsertTag(0,ST_END);
	while(tag && tag->next) {
	    tag = tag->next;
	}
	if(tag->id != ST_END) {
	    tag = swf_InsertTag(tag,ST_END);
	}
    }

    fi = open(filename, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
    if(fi<0) {
	PyErr_SetString(PyExc_Exception, setError("couldn't create output file %s", filename));
	return 0;
    }
    if(swf->compressed) {
	    if(swf_WriteSWC(fi, swf)<0) {
		close(fi);
		PyErr_SetString(PyExc_Exception, setError("WriteSWC() failed."));
		return 0;
	    }
    } else {
	    if(swf_WriteSWF(fi, swf)<0) {
		close(fi);
		PyErr_SetString(PyExc_Exception, setError("WriteSWC() failed."));
		return 0;
	    }
    }
    close(fi);

    /* TODO: why is this segfaulting?? */
   /* swf_FreeTags(swf);
    swf->firstTag = 0;*/
    
    return PY_NONE;
}
//----------------------------------------------------------------------------
static PyObject * swf_writeCGI(PyObject* self, PyObject* args)
{
    SWFObject*swf = (SWFObject*)self;
    if(!self || !PyArg_ParseTuple(args,"")) 
	return NULL;
    swf->swf.firstTag = taglist_getTAGs(swf->taglist);
    if(!swf->swf.firstTag)
	return NULL;
    swf_WriteCGI(&swf->swf);
    swf->swf.firstTag = 0;
    return PY_NONE;
}
//----------------------------------------------------------------------------

//TODO: void swf_Relocate(SWF*swf, char*bitmap); // bitmap is 65536 bytes, bitmap[a]==0 means id a is free

static PyMethodDef swf_functions[] =
{{"save", (PyCFunction)swf_save, METH_KEYWORDS, "Save SWF to disk"},
 {"writeCGI", (PyCFunction)swf_writeCGI, METH_VARARGS, "print SWF as CGI to stdout"},
 {NULL, NULL, 0, NULL}
};
 
//----------------------------------------------------------------------------
static void swf_dealloc(PyObject* self)
{
    mylog("-%08x(%d) swf_dealloc\n", (int)self, self->ob_refcnt);
    SWFObject*swfo;
    SWF*swf;
    swfo = (SWFObject*)self;
    swf = &swfo->swf;
    if(swfo->filename) {
	free(swfo->filename);
	swfo->filename = 0;
    }
    Py_DECREF(swfo->taglist);
    swfo->taglist = 0;
    PyObject_Del(self);
}
//----------------------------------------------------------------------------
static int swf_print(PyObject * self, FILE *fi, int flags) //flags&Py_PRINT_RAW
{
    mylog(" %08x(%d) print \n", (int)self, self->ob_refcnt);
    SWFObject*swf = (SWFObject*)self;
    swf_DumpHeader(fi, &swf->swf);
    //void swf_DumpSWF(FILE * f,SWF*swf);
    return 0;
}
//----------------------------------------------------------------------------
static PyObject* swf_getattr(PyObject * self, char* a)
{
    SWFObject*swf = (SWFObject*)self;
    PyObject* ret;

    if(!strcmp(a, "fps")) {
	double fps = swf->swf.frameRate/256.0;
	mylog(" %08x(%d) swf_getattr %s = %f\n", (int)self, self->ob_refcnt, a, fps);
	return Py_BuildValue("d", fps);
    } else if(!strcmp(a, "version")) {
	int version = swf->swf.fileVersion;;
	mylog(" %08x(%d) swf_getattr %s = %d\n", (int)self, self->ob_refcnt, a, version);
	return Py_BuildValue("i", version);
    } else if(!strcmp(a, "name")) {
	char*filename = swf->filename;
	mylog(" %08x(%d) swf_getattr %s = %s\n", (int)self, self->ob_refcnt, a, filename);
	return Py_BuildValue("s", filename);
    } else if(!strcmp(a, "bbox")) {
	int xmin,ymin,xmax,ymax;
	xmin = swf->swf.movieSize.xmin;
	ymin = swf->swf.movieSize.ymin;
	xmax = swf->swf.movieSize.xmax;
	ymax = swf->swf.movieSize.ymax;
	mylog(" %08x(%d) swf_getattr %s = (%d,%d,%d,%d)\n", (int)self, self->ob_refcnt, a, xmin,ymin,xmax,ymax);
	return Py_BuildValue("(iiii)", xmin, ymin, xmax, ymax); 
    } else if(!strcmp(a, "tags")) {
	PyObject*ret =  (PyObject*)(swf->taglist);
	Py_INCREF(ret);
	mylog(" %08x(%d) swf_getattr %s = %08x(%d)\n", (int)self, self->ob_refcnt, a, ret, ret->ob_refcnt);
	return ret;
    }

    ret = Py_FindMethod(swf_functions, self, a);
    mylog(" %08x(%d) swf_getattr %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
    return ret;
}
//----------------------------------------------------------------------------
static int swf_setattr(PyObject * self, char* a, PyObject * o)
{
    SWFObject*swf = (SWFObject*)self;
    if(!strcmp(a, "fps")) {
	double fps;
	if (!PyArg_Parse(o, "d", &fps)) 
	    goto err;
	swf->swf.frameRate = (int)(fps*0x100);
	mylog(" %08x(%d) swf_setattr %s = %f\n", (int)self, self->ob_refcnt, a, fps);
	return 0;
    } else if(!strcmp(a, "version")) {
	int version;
	if (!PyArg_Parse(o, "i", &version)) 
	    goto err;
	swf->swf.fileVersion = version;
	mylog(" %08x(%d) swf_setattr %s = %d\n", (int)self, self->ob_refcnt, a, version);
	return 0;
    } else if(!strcmp(a, "name")) {
	char*filename;
	if (!PyArg_Parse(o, "s", &filename)) 
	    goto err;
	if(swf->filename) {
	    free(swf->filename);swf->filename=0;
	}
	swf->filename = strdup(filename);
	mylog(" %08x(%d) swf_setattr %s = %s\n", (int)self, self->ob_refcnt, a, filename);
	return 0;
    } else if(!strcmp(a, "bbox")) {
	int xmin=0,ymin=0,xmax=0,ymax=0;
	if (!PyArg_Parse(o, "(iiii)", &xmin, &ymin, &xmax, &ymax)) 
	    goto err;

	swf->swf.movieSize.xmin = xmin;
	swf->swf.movieSize.ymin = ymin;
	swf->swf.movieSize.xmax = xmax;
	swf->swf.movieSize.ymax = ymax;
	mylog(" %08x(%d) swf_setattr %s = (%d,%d,%d,%d)\n", (int)self, self->ob_refcnt, a, xmin,ymin,xmax,ymax);
	return 0;
    } else if(!strcmp(a, "tags")) {
	PyObject* taglist;
	/*if (!PyArg_Parse(o, "O!", &TagListClass, &taglist));
	    goto err;*/
	// TODO: check if it's really a taglist
	taglist = o;
	Py_DECREF(swf->taglist);
	swf->taglist = taglist;
	Py_INCREF(swf->taglist);
	mylog(" %08x(%d) swf_setattr %s = %08x\n", (int)self, self->ob_refcnt, a, swf->taglist);
	return 0;
    }
err:
    mylog(" %08x(%d) swf_setattr %s = ? (%08x)\n", (int)self, self->ob_refcnt, a, o);
    return 1;
}

//----------------------------------------------------------------------------
static PyTypeObject SWFClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "SWF",
    tp_basicsize: sizeof(SWFObject),
    tp_itemsize: 0,
    tp_dealloc: swf_dealloc,
    tp_print: swf_print,
    tp_getattr: swf_getattr,
    tp_setattr: swf_setattr,
};
//----------------------------------------------------------------------------

static PyMethodDef SWFMethods[] = 
{
    /* SWF creation*/
    {"load", f_load, METH_VARARGS, "Load a SWF from disc."},
    {"create", (PyCFunction)f_create, METH_KEYWORDS, "Create a new SWF from scratch."},
    {0,0,0,0}
    // save is a member function
};
PyMethodDef* swf_getMethods()
{
    SWFClass.ob_type = &PyType_Type;
    return SWFMethods;
}

// =============================================================================

#include "primitives.h"
#include "action.h"
#include "tag.h"
#include "taglist.h"

static PyObject* module_verbose(PyObject* self, PyObject* args, PyObject* kwargs)
{
    int _verbose = 0;
    static char *kwlist[] = {"verbosity", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &verbose))
	return NULL;
    setVerbosity(_verbose);

    return Py_BuildValue("s", 0);
}

static PyMethodDef LoggingMethods[] = 
{
    /* Module functions */
    {"verbose", (PyCFunction)module_verbose, METH_KEYWORDS, "Set the module verbosity"},
    {0,0,0,0}
};
    
void initSWF(void)
{
    PyObject*module;
    PyMethodDef* primitive_methods = primitive_getMethods();
    PyMethodDef* tag_methods = tag_getMethods();
    PyMethodDef* action_methods = action_getMethods();
    PyMethodDef* swf_methods = swf_getMethods();

    PyMethodDef* all_methods = 0;
    all_methods = addMethods(all_methods, primitive_methods);
    all_methods = addMethods(all_methods, tag_methods);
    all_methods = addMethods(all_methods, action_methods);
    all_methods = addMethods(all_methods, swf_methods);

    all_methods = addMethods(all_methods, LoggingMethods);

    module = Py_InitModule("SWF", all_methods);
}
