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
#include "primitives.h"
#include "action.h"

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
staticforward PyTypeObject TagListClass;
staticforward PyTypeObject TagClass;

/* Tags, Objects */

typedef struct {
    PyObject_HEAD
    TAG*tag;
    /* ST_DEFINEFONT*/
    SWFFONT* font;
    /* ST_PLACEOBJECT, ST_PLACEOBJECT2*/
    SWFPLACEOBJECT* placeobject;
    PyObject* character;
} TagObject;

typedef struct {
    PyObject_HEAD
    TAG*firstTag;
    TAG*searchTag;
    TAG*lastTag;
    PyDictObject* char2id;
    PyDictObject* id2char;
    U16 currentID;
} TagListObject;

typedef struct {
    PyObject_HEAD
    SWF swf; //swf.firstTag ist not used
    TagListObject*taglist;
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

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|idOs", 
		kwlist, &version, &framerate, 
		&obbox, filename))
	return NULL;

    if (!PyArg_Parse(obbox, "(iiii)", &bbox.xmin, &bbox.ymin, &bbox.xmax, &bbox.ymax))
	return NULL;

    swf = PyObject_New(SWFObject, &SWFClass);
    memset(&swf->swf, 0, sizeof(SWF));
    if(filename)
	swf->filename = strdup(filename);
    else
	swf->filename = 0;

    swf->swf.fileVersion = version;
    swf->swf.frameRate = (int)(framerate*0x100);
    swf->swf.movieSize = bbox;
    swf->taglist = PyObject_New(TagListObject, &TagListClass);
    swf->taglist->firstTag = 0;
    swf->taglist->searchTag = 0;
    swf->taglist->lastTag = 0;
    swf->taglist->currentID = 1;
    swf->taglist->char2id = (PyDictObject*)PyDict_New();
    swf->taglist->id2char = (PyDictObject*)PyDict_New();

    if(swf->swf.fileVersion>=6)
	swf->swf.compressed = 1;

    mylog("create %08x -> %08x\n", (int)self, (int)swf);
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
    memset(&swf->swf, 0, sizeof(SWF));
    swf->filename = strdup(filename);
    swf->taglist = PyObject_New(TagListObject, &TagListClass);

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
    
    swf->taglist->firstTag = swf->swf.firstTag;
    swf->taglist->searchTag = swf->swf.firstTag;
    swf->taglist->lastTag = swf->swf.firstTag;
    swf->taglist->currentID = 1;
    swf->taglist->char2id = (PyDictObject*)PyDict_New();
    swf->taglist->id2char = (PyDictObject*)PyDict_New();
    swf->swf.firstTag = 0;

    mylog("load %08x -> %08x\n", (int)self, (int)swf);
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

    // keyword arg compress (=1) forces compression
    if(compress)
	swf->compressed = 1;
    
    swf->firstTag = swfo->taglist->firstTag;

    // fix the file, in case it is empty or not terminated properly
    {
	TAG*tag = swf->firstTag;
	if(!tag)
	    tag = swf->firstTag = swf_InsertTag(0,ST_END);
	while(tag && tag->next)
	    tag = tag->next;
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
    
    swf->firstTag = 0;
    
    return PY_NONE;
}
//----------------------------------------------------------------------------
static PyObject * swf_writeCGI(PyObject* self, PyObject* args)
{
    SWFObject*swf = (SWFObject*)self;
    if(!self || !PyArg_ParseTuple(args,"")) 
	return NULL;
    swf->swf.firstTag = swf->taglist->firstTag;
    swf_WriteCGI(&swf->swf);
    swf->swf.firstTag = 0;
    return PY_NONE;
}
//----------------------------------------------------------------------------
static PyObject * taglist_foldAll(PyObject* self, PyObject* args)
{
    SWF swf;
    TagListObject*taglist = (TagListObject*)self;
    if(!self || !PyArg_ParseTuple(args,"")) 
	return NULL;
    swf.firstTag = taglist->firstTag;
    swf_FoldAll(&swf);
    taglist->firstTag = swf.firstTag;
    taglist->lastTag = 0; // FIXME
    taglist->searchTag = 0;
    return PY_NONE;
}
//----------------------------------------------------------------------------
static PyObject * taglist_unfoldAll(PyObject* self, PyObject* args)
{
    SWF swf;
    TagListObject*taglist = (TagListObject*)self;
    if(!self || !PyArg_ParseTuple(args,"")) 
	return NULL;
    swf.firstTag = taglist->firstTag;
    swf_UnFoldAll(&swf);
    taglist->firstTag = swf.firstTag;
    taglist->lastTag = 0; // FIXME
    taglist->searchTag = 0;
    return PY_NONE;
}
//----------------------------------------------------------------------------
static PyObject * taglist_optimizeOrder(PyObject* self, PyObject* args)
{
    SWF swf;
    TagListObject*taglist = (TagListObject*)self;
    if(!self || !PyArg_ParseTuple(args,"")) 
	return NULL;
    swf.firstTag = taglist->firstTag;
    swf_UnFoldAll(&swf);
    taglist->firstTag = swf.firstTag;
    taglist->lastTag = 0; // FIXME
    taglist->searchTag = 0;
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
    SWFObject*swfo;
    SWF*swf;
    swfo = (SWFObject*)self;
    swf = &swfo->swf;
    mylog("swf_dealloc %08x(%d)\n", (int)self, self->ob_refcnt);
    if(swfo->filename) {
	free(swfo->filename);
	swfo->filename = 0;
    }
    Py_DECREF(swfo->taglist);
    swfo->taglist = 0;
    PyObject_Del(self);
}
//----------------------------------------------------------------------------
static void taglist_dealloc(PyObject* self)
{
    TagListObject*taglist = (TagListObject*)self;
    SWF swf;
    mylog("taglist_dealloc %08x(%d)\n", (int)self, self->ob_refcnt);
    swf.firstTag = taglist->firstTag;
    swf_FreeTags(&swf);
    taglist->firstTag = 0;
    taglist->lastTag = 0;
    taglist->searchTag = 0;
    PyObject_Del(self);
}
//----------------------------------------------------------------------------
static int swf_print(PyObject * self, FILE *fi, int flags) //flags&Py_PRINT_RAW
{
    SWFObject*swf = (SWFObject*)self;
    swf_DumpHeader(fi, &swf->swf);
    //void swf_DumpSWF(FILE * f,SWF*swf);
    mylog("print %08x(%d)\n", (int)self, self->ob_refcnt);
    return 0;
}
//----------------------------------------------------------------------------
static PyObject* swf_getattr(PyObject * self, char* a)
{
    SWFObject*swf = (SWFObject*)self;
    PyObject* ret;

    if(!strcmp(a, "fps")) {
	double fps = swf->swf.frameRate/256.0;
	mylog("swf_getattr %08x(%d) %s = %f\n", (int)self, self->ob_refcnt, a, fps);
	return Py_BuildValue("d", fps);
    } else if(!strcmp(a, "version")) {
	int version = swf->swf.fileVersion;;
	mylog("swf_getattr %08x(%d) %s = %d\n", (int)self, self->ob_refcnt, a, version);
	return Py_BuildValue("i", version);
    } else if(!strcmp(a, "name")) {
	char*filename = swf->filename;
	mylog("swf_getattr %08x(%d) %s = %s\n", (int)self, self->ob_refcnt, a, filename);
	return Py_BuildValue("s", filename);
    } else if(!strcmp(a, "bbox")) {
	int xmin,ymin,xmax,ymax;
	xmin = swf->swf.movieSize.xmin;
	ymin = swf->swf.movieSize.ymin;
	xmax = swf->swf.movieSize.xmax;
	ymax = swf->swf.movieSize.ymax;
	mylog("swf_getattr %08x(%d) %s = (%d,%d,%d,%d)\n", (int)self, self->ob_refcnt, a, xmin,ymin,xmax,ymax);
	return Py_BuildValue("(iiii)", xmin, ymin, xmax, ymax); 
    } else if(!strcmp(a, "tags")) {
	PyObject*ret =  (PyObject*)(swf->taglist);
	Py_INCREF(ret);
	mylog("swf_getattr %08x(%d) %s = %08x(%d)\n", (int)self, self->ob_refcnt, a, ret, ret->ob_refcnt);
	return ret;
    }

    ret = Py_FindMethod(swf_functions, self, a);
    mylog("swf_getattr %08x(%d) %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
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
	mylog("swf_setattr %08x(%d) %s = %f\n", (int)self, self->ob_refcnt, a, fps);
	return 0;
    } else if(!strcmp(a, "version")) {
	int version;
	if (!PyArg_Parse(o, "i", &version)) 
	    goto err;
	swf->swf.fileVersion = version;
	mylog("swf_setattr %08x(%d) %s = %d\n", (int)self, self->ob_refcnt, a, version);
	return 0;
    } else if(!strcmp(a, "name")) {
	char*filename;
	if (!PyArg_Parse(o, "s", &filename)) 
	    goto err;
	if(swf->filename) {
	    free(swf->filename);swf->filename=0;
	}
	swf->filename = strdup(filename);
	mylog("swf_setattr %08x(%d) %s = %s\n", (int)self, self->ob_refcnt, a, filename);
	return 0;
    } else if(!strcmp(a, "bbox")) {
	int xmin=0,ymin=0,xmax=0,ymax=0;
	if (!PyArg_Parse(o, "(iiii)", &xmin, &ymin, &xmax, &ymax)) 
	    goto err;

	swf->swf.movieSize.xmin = xmin;
	swf->swf.movieSize.ymin = ymin;
	swf->swf.movieSize.xmax = xmax;
	swf->swf.movieSize.ymax = ymax;
	mylog("swf_setattr %08x(%d) %s = (%d,%d,%d,%d)\n", (int)self, self->ob_refcnt, a, xmin,ymin,xmax,ymax);
	return 0;
    } else if(!strcmp(a, "tags")) {
	TagListObject* taglist;
	/*if (!PyArg_Parse(o, "O!", &TagListClass, &taglist));
	    goto err;*/
	taglist = (TagListObject*)o;
	Py_DECREF(swf->taglist);
	swf->taglist = taglist;
	Py_INCREF(swf->taglist);
	mylog("swf_setattr %08x(%d) %s = %08x\n", (int)self, self->ob_refcnt, a, swf->taglist);
	return 0;
    }
err:
    mylog("swf_setattr %08x(%d) %s = ? (%08x)\n", (int)self, self->ob_refcnt, a, o);
    return 1;
}

//----------------------------------------------------------------------------
static PyMethodDef taglist_functions[] =
{{"foldAll", taglist_foldAll, METH_VARARGS, "fold all sprites (movieclips) in the list"},
 {"unfoldAll", taglist_unfoldAll, METH_VARARGS, "unfold (expand) all sprites (movieclips) in the list"},
 {"optimizeOrder", taglist_optimizeOrder, METH_VARARGS, "Reorder the Tag structure"},
 {NULL, NULL, 0, NULL}
};

static PyObject* taglist_getattr(PyObject * self, char* a)
{
    PyObject* ret = Py_FindMethod(taglist_functions, self, a);
    mylog("taglist_getattr %08x(%d) %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
    return ret;
}
//----------------------------------------------------------------------------
static int taglist_length(PyObject * self)
{
    TagListObject*tags = (TagListObject*)self;
    TAG*tag;
    int l = 0;
    mylog("taglist_length %08x(%d)", (int)self, self->ob_refcnt);
    tag = tags->firstTag;
    while(tag) {
	l++;
	tag = tag->next;
    }
    return l;
}
//----------------------------------------------------------------------------
static PyObject * taglist_concat(PyObject * self, PyObject* list)
{
    TagObject*tag;
    TagListObject*taglist = (TagListObject*)self;
    mylog("taglist_concat %08x(%d) %08x", (int)self, self->ob_refcnt, list);
	
    if (PyArg_Parse(list, "O!", &TagClass, &tag)) {
	/* copy tag, so we don't have to do INCREF(tag) (and don't
	   get problems if the tag is appended to more than one
	   taglist) */
	/* TODO: handle IDs */
	mylog("taglist_concat: Tag", (int)self, self->ob_refcnt);
	taglist->lastTag = swf_InsertTag(taglist->lastTag, tag->tag->id);
	swf_SetBlock(taglist->lastTag, tag->tag->data, tag->tag->len);
	if(!taglist->firstTag) {
	    taglist->firstTag = taglist->searchTag = taglist->lastTag;
	}
	if(swf_isDefiningTag(tag->tag)) {
	    PyObject*id = PyLong_FromLong(taglist->currentID);
	    PyDict_SetItem((PyObject*)(taglist->char2id), list, id);
	    Py_INCREF(id);
	    PyDict_SetItem((PyObject*)(taglist->id2char), id, list);
	    Py_INCREF(id);
	}
	Py_INCREF(self);
	return self;
    }
    PyErr_Clear();
    if (PyList_Check(list)) {
	int l = PyList_Size(list);
	int t;
	mylog("taglist_concat: PythonList", (int)self, self->ob_refcnt);
	for(t=0;t<l;t++) {
	    PyObject*item = PyList_GetItem(list, t);
	    self = taglist_concat(self, item);
	    if(!self)
		return 0;
	}
	Py_INCREF(self);
	return self;
    }
    PyErr_SetString(PyExc_Exception, setError("taglist concatenation only works with tags and lists (%08x).", list));
    return 0;
}
//----------------------------------------------------------------------------
staticforward TagObject* TagObject_New();
static PyObject * taglist_item(PyObject * self, int index)
{
    TagListObject*taglist = (TagListObject*)self;
    TAG*tag;
    TagObject*tagobject;
    int i = 0;
    mylog("taglist_item %08x(%d) [%d]", (int)self, self->ob_refcnt, index);

    if(index<0) {
	PyErr_SetString(PyExc_Exception, setError("Negative Indices not supported."));
	return NULL;
    }

    tag = taglist->firstTag;
    while(tag && i<index) {
	tag = tag->next;
	i++;
    }
    if(!tag || i != index) {
	if(index> i+10) {
	    PyErr_SetString(PyExc_Exception, setError("No Tag at position %d", index));
	    return NULL;
	}

	mylog("taglist_item %08x(%d)->IndexError (%d)", (int)self, self->ob_refcnt, index);

	Py_INCREF(PyExc_IndexError);
	PyErr_SetObject(PyExc_IndexError, Py_None);
	return NULL;
    }

    tagobject = TagObject_New();
    tagobject->tag = tag;

    return (PyObject*)tagobject;
}
//----------------------------------------------------------------------------
static void tag_dealloc(PyObject * self)
{
    TagObject*tag = (TagObject*)self;
    mylog("tag_dealoc %08x(%d)\n", (int)self, self->ob_refcnt);
    if(tag->placeobject) {
	swf_PlaceObjectFree(tag->placeobject);
	tag->placeobject = 0;
    }
    if(tag->font) {
	swf_FontFree(tag->font);
	tag->font = 0;
    }
    if(tag->character) {
	Py_DECREF(tag->character);
	tag->character = 0;
    }
    PyObject_Del(self);
}
//----------------------------------------------------------------------------
static PyObject* tag_setU8(PyObject * self, PyObject*other)
{
    return NULL;
}
//----------------------------------------------------------------------------
static PyObject* tag_setbackgroundcolor_getrgb(PyObject * self, PyObject*other)
{
    TagObject*tag = (TagObject*)self;
    int r,g,b;
    r = tag->tag->data[0];
    g = tag->tag->data[1];
    b = tag->tag->data[2];
    return Py_BuildValue("(iii)", r,g,b);
}
//----------------------------------------------------------------------------

static struct tagfunctions {
    int id;
    PyMethodDef f[8];
} tagfunctions[] =
{
 { 
   ST_SETBACKGROUNDCOLOR, 
   {{"getRGB", tag_setbackgroundcolor_getrgb, METH_VARARGS, "get's the color set by this tag"},
    {NULL, NULL, 0, NULL}
   }
 }
};
static PyMethodDef common_tagfunctions[] =
{{"setU8", tag_setU8, METH_VARARGS, "sets a byte to the tag data"},
 {NULL, NULL, 0, NULL}
};

static PyObject* tag_getattr(PyObject * self, char* a)
{
    TagObject*tag = (TagObject*)self;
    PyObject* ret = NULL;
    int id =  tag->tag->id;
    int t;
   
    /* search for a tag specific function */
    for(t=0;t<sizeof(tagfunctions)/sizeof(tagfunctions[0]);t++)
    {
	if(id==tagfunctions[t].id) {
	    mylog("tag_getattr: id %d found\n", id);
	    ret = Py_FindMethod(tagfunctions[t].f, self, a);
	    break;
	}
    }

    /* search in the functions common to all tags */
    FindMethodMore(ret, common_tagfunctions, self, a);

    mylog("tag_getattr %08x(%d) %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
    return ret;
}
//----------------------------------------------------------------------------
//                     Tag Contructors
//----------------------------------------------------------------------------
static TagObject* TagObject_New()
{
    TagObject*tag = PyObject_New(TagObject, &TagClass);
    tag->font = 0;
    tag->character = 0;
    tag->placeobject = 0;
    tag->tag = 0;
    return tag;
}

static PyObject* f_SetBackgroundColor(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"color", NULL};
    int r=0,g=0,b=0;
    TagObject*tag;
    ColorObject*color;

    /* 1st try- copy constructor */
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", kwlist, &ColorClass, &color)) {
	/* 2nd try- color's contructor */
	color = (ColorObject*)f_Color(NULL, args, kwargs);
    }
    if(!color)
	return NULL;

    tag = TagObject_New();
    tag->tag = swf_InsertTag(0, ST_SETBACKGROUNDCOLOR);
    swf_SetU8(tag->tag, color->rgba.r);
    swf_SetU8(tag->tag, color->rgba.g);
    swf_SetU8(tag->tag, color->rgba.b);
    mylog("SetBackgroundColor(%02x,%02x,%02x) %08x -> %08x\n", color->rgba.r, color->rgba.g, color->rgba.b, (int)self, tag);
    return (PyObject*)tag;
}
//----------------------------------------------------------------------------
static PyObject* f_DefineFont(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"filename", NULL};
    char*filename = 0;
    TagObject*tag;
    SWFFONT* font;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &filename))
	return NULL;

    font = swf_ReadFont(filename);
    mylog("font=%08x",font);
    if(!font) {
	PyErr_SetString(PyExc_Exception, setError("Could not load %s", filename));
	return NULL;
    }

    tag = TagObject_New();
    tag->font = font;
    tag->tag = swf_InsertTag(0, ST_DEFINEFONT2);
    tag->font->id = 0xabcd; // swf_SetU16(id);
    swf_FontSetDefine2(tag->tag, tag->font); // TODO: should this be done later, in taglist?
    mylog("DefineFont %08x -> %08x\n", (int)self, (int)tag);
    return (PyObject*)tag;
}
//----------------------------------------------------------------------------
static PyObject* f_Protect(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"password", NULL};
    char*password = 0;
    TagObject*tag;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &password))
	return NULL;

    tag = TagObject_New();
    tag->tag = swf_InsertTag(0, ST_PROTECT);
    if(password) {
	swf_SetPassword(tag->tag, password);
    }
    mylog("f_Protect %08x -> %08x\n", (int)self, (int)tag);
    return (PyObject*)tag;
}
//----------------------------------------------------------------------------
static PyObject* f_DefineText(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"font", "text", "size", "color", NULL};
    TagObject*tag;
    char*text = 0;
    int textlen = 0;
    PyObject*unicode16;
    PyObject*unicode8;
    int size = 0;
    RGBA rgba = {0,0,0,255};
    ColorObject*color = 0;
    TagObject*font = 0;
    SRECT r;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!u#i|O!", kwlist, &TagClass, &font, &text, &textlen, &size, &ColorClass, &color))
	return NULL;
    
    unicode16 = PyUnicode_DecodeUTF16(text, textlen*2, NULL, NULL);
    unicode8 = PyUnicode_AsUTF8String(unicode16);
    text = PyString_AS_STRING(unicode8);

    if(color)
	rgba = color->rgba;

    mylog("DefineText: text = %s", text);
    
    tag = TagObject_New();
    tag ->tag= swf_InsertTag(0, ST_DEFINETEXT2);
    swf_SetU16(tag->tag, /*ID*/0);
    r = swf_SetDefineText(tag->tag, font->font, &rgba, text, size);
    mylog("DefineText %08x -> %08x\n", (int)self, (int)tag);
    return (PyObject*)tag;
}
//----------------------------------------------------------------------------
static PyObject* f_PlaceObject(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"character", "depth", "matrix", "colortransform", "ratio", "name", "clipdepth", "action", NULL};
    TagObject*tag;
    
    TagObject*character = 0;
    int depth;
    int clipdepth = 0;
    MatrixObject*matrix = 0;
    CXFormObject*cxform = 0;
    int ratio = 0;
    ActionObject*action = 0;
    char* name = 0;
    SWFPLACEOBJECT* po;
    po = malloc(sizeof(SWFPLACEOBJECT));
    memset(po, 0, sizeof(SWFPLACEOBJECT));

    swf_GetPlaceObject(0, po);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!i|O!O!isiO!", kwlist, 
		&TagClass, &character, 
		&depth, 
		&MatrixClass, &matrix, 
		&CXFormClass, &cxform,
		&ratio,
		&name,
		&clipdepth,
		&action
		))
	return NULL;
    po->depth = depth;
    po->id = /*ID*/ 0;
    po->clipdepth = clipdepth;
    po->ratio = ratio;
    po->name = name;
    if(clipdepth) po->matrix = matrix->matrix;
    if(cxform) po->cxform = cxform->cxform;
    if(action) po->actions = action->action;

    tag = TagObject_New();
    tag->placeobject = po;
    Py_INCREF(character);
    tag->character = (PyObject*)character;
    tag->tag= swf_InsertTag(0, ST_PLACEOBJECT2);
    swf_SetPlaceObject(tag->tag, po);
    mylog("PlaceObject %08x -> %08x\n", (int)self, (int)tag);
    return (PyObject*)tag;
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
static PySequenceMethods taglist_as_sequence =
{
    sq_length: taglist_length, // len(obj)
    sq_concat: taglist_concat, // obj += [...], obj1+obj2
    sq_repeat: 0,            // x*n, intargfunc
    sq_item: taglist_item,  // obj[3]
    sq_slice: 0,             // x[i:j] intintargfunc
    sq_ass_item: 0,          // x[i] = y intobjargproc
    sq_ass_slice: 0,         // x[i:j] = v intintobjargproc
    sq_contains: 0,          //???
};
static PyTypeObject TagListClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "TagList",
    tp_basicsize: sizeof(TagListObject),
    tp_itemsize: 0,
    tp_dealloc: taglist_dealloc,
    tp_print: 0,                 // print x
    tp_getattr: taglist_getattr, // x.attr
    tp_setattr: 0,               // x.attr = v
    tp_compare: 0,               // x>y
    tp_repr: 0,                  // `x`, print x
    tp_as_number: 0,
    tp_as_sequence: &taglist_as_sequence,
};
static PyTypeObject TagClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "Tag",
    tp_basicsize: sizeof(TagObject),
    tp_itemsize: 0,
    tp_dealloc: tag_dealloc,
    tp_print: 0,
    tp_getattr: tag_getattr,
};
//----------------------------------------------------------------------------

static PyMethodDef SWFMethods[] = 
{
    /* SWF creation*/
    {"load", f_load, METH_VARARGS, "Load a SWF from disc."},
    {"create", (PyCFunction)f_create, METH_KEYWORDS, "Create a new SWF from scratch."},
    
    /* Primitives */
    {"Color", (PyCFunction)f_Color, METH_KEYWORDS, "Create a new color object."},
    {"Gradient", (PyCFunction)f_Gradient, METH_KEYWORDS, "Create a new gradient object."},
    {"ColorTransform", (PyCFunction)f_ColorTransform, METH_KEYWORDS, "Create a new colortransform object."},
    {"Matrix", (PyCFunction)f_Matrix, METH_KEYWORDS, "Create a new matrix object."},
    {"BBox", (PyCFunction)f_BBox, METH_KEYWORDS, "Create a new bounding box object."},

    /* TAGS */
    {"SetBackgroundColor", (PyCFunction)f_SetBackgroundColor, METH_KEYWORDS, "Create a SetBackGroundColor Tag."},
    {"Protect", (PyCFunction)f_Protect, METH_KEYWORDS, "Create a Protect Tag."},
    {"DefineFont", (PyCFunction)f_DefineFont, METH_KEYWORDS, "Create a DefineFont Tag."},
    {"DefineText", (PyCFunction)f_DefineText, METH_KEYWORDS, "Create a DefineText Tag."},
    {"PlaceObject", (PyCFunction)f_PlaceObject, METH_KEYWORDS, "Create a PlaceObject Tag."},
    {NULL, NULL, 0, NULL}
};

void initSWF(void)
{
    SWFClass.ob_type = &PyType_Type;

    initLog("test.log",8,0,0,0,0);

    (void)Py_InitModule("SWF", SWFMethods);
}
