#include <Python.h>
#undef HAVE_STAT
#include "../rfxswf.h"
#include "../log.h"
#include "./pyutils.h"
#include "primitives.h"
#include "action.h"
#include "tag.h"
#include "taglist.h"

//----------------------------------------------------------------------------
typedef struct {
    PyObject_HEAD
    TAG*firstTag;
    TAG*searchTag;
    TAG*lastTag;
    PyDictObject* char2id;
    PyDictObject* id2char;
    U16 currentID;
} TagListObject;
//----------------------------------------------------------------------------
PyObject * taglist_new()
{
    TagListObject* taglist = PyObject_New(TagListObject, &TagListClass);
    taglist->firstTag = 0;
    taglist->searchTag = 0;
    taglist->lastTag = 0;
    taglist->currentID = 1;
    taglist->char2id = (PyDictObject*)PyDict_New();
    taglist->id2char = (PyDictObject*)PyDict_New();
    return (PyObject*)taglist;
}
PyObject * taglist_new2(TAG*tag)
{
    TagListObject* taglist = PyObject_New(TagListObject, &TagListClass);
    taglist->firstTag = tag;
    taglist->searchTag = tag;
    taglist->lastTag = tag;
    while(taglist->lastTag->next)
	taglist->lastTag = taglist->lastTag->next;
    taglist->currentID = 1;
    taglist->char2id = (PyDictObject*)PyDict_New();
    taglist->id2char = (PyDictObject*)PyDict_New();
    return (PyObject*)taglist;
}
TAG* taglist_getTAGs(PyObject*taglist)
{
    // TODO: checking!
    return ((TagListObject*)taglist)->firstTag;
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
    PyObject*tag = 0;
    TagListObject*taglist = (TagListObject*)self;
    mylog("taglist_concat %08x(%d) %08x", (int)self, self->ob_refcnt, list);

    /* TODO: rewrite */
	
    if (PyArg_Parse(list, "O!", &TagClass, &tag)) {
	/* copy tag, so we don't have to do INCREF(tag) (and don't
	   get problems if the tag is appended to more than one
	   taglist) */
	/* TODO: handle IDs */
	mylog("taglist_concat: Tag", (int)self, self->ob_refcnt);
	taglist->lastTag = swf_InsertTag(taglist->lastTag, tag_getTAG(tag)->id);
	swf_SetBlock(taglist->lastTag, tag_getTAG(tag)->data, tag_getTAG(tag)->len);
	if(!taglist->firstTag) {
	    taglist->firstTag = taglist->searchTag = taglist->lastTag;
	}
	if(swf_isDefiningTag(tag_getTAG(tag))) {
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
static PyObject * taglist_item(PyObject * self, int index)
{
    TagListObject*taglist = (TagListObject*)self;
    TAG*tag;
    PyObject*tagobject;
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

    tagobject = tag_new2(tag);
    return (PyObject*)tagobject;
}
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
