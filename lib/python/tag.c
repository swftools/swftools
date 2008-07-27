#include <Python.h>
#undef HAVE_STAT
#include "../rfxswf.h"
#include "../log.h"
#include "./pyutils.h"
#include "primitives.h"
#include "action.h"
#include "tag.h"
#include "tagmap.h"

//----------------------------------------------------------------------------

typedef struct _TagObject {
    PyObject_HEAD
    tag_internals_t internals;
} TagObject;

//----------------------------------------------------------------------------
static PyMethodDef generic_methods[] = 
{
  {NULL, NULL, 0, NULL}
};
static tag_internals_t generic_tag =
{
    parse: 0,
    dealloc: 0,
    fillTAG: 0,
    tagfunctions: generic_methods,
    datasize: 0,
};
//----------------------------------------------------------------------------

static struct tag_parser {
    int id;
    tag_internals_t*spec;
    struct tag_parser* next;
} tag_parsers[1024];
static char parsers_initialized = 0;

void register_tag(int id, tag_internals_t*spec)
{
    assert(id>=0 && id<1024);
    if(!parsers_initialized) {
	memset(tag_parsers, 0, sizeof(tag_parsers));
	parsers_initialized = 1;
    }
    tag_parsers[id].id = id;
    tag_parsers[id].spec = spec;
};

static tag_internals_t* get_parser(int id)
{
    if(parsers_initialized<2) {
	int t;
	struct tag_parser*last = &tag_parsers[0];
	for(t=0;t<1024;t++) {
	    if(tag_parsers[t].spec) {
		last->next = &tag_parsers[t];
		last = &tag_parsers[t];
	    }
	}
	parsers_initialized = 2;
    }
    assert(id>=0 && id<1024);
    return tag_parsers[id].spec;
}

//----------------------------------------------------------------------------
static void tag_dealloc(PyObject * self)
{
    TagObject*tag = (TagObject*)self;
    if(tag->internals.tag)
	mylog("-%08x(%d) tag_dealoc [%s]\n", (int)self, self->ob_refcnt, swf_TagGetName(tag->internals.tag));
    else
	mylog("-%08x(%d) tag_dealoc [?]\n", (int)self, self->ob_refcnt);
    if(tag->internals.dealloc) {
	if(!tag->internals.data)
	    mylog("-%08x(%d) tag_dealoc: Warning: calling dealloc without any data(?)\n", (int)self, self->ob_refcnt);
	tag->internals.dealloc(&tag->internals);
    }
    if(tag->internals.data) {
	free(tag->internals.data);
	tag->internals.data = 0;
    }
    if(tag->internals.tag) {
	swf_DeleteTag(0, tag->internals.tag);
	tag->internals.tag = 0;
    }
    Py_DECREF(tag->internals.tagmap);
    tag->internals.tagmap = 0;
    PyObject_Del(self);
}
//----------------------------------------------------------------------------
static int fillTAG(PyObject*self) 
{
    TagObject*tag = (TagObject*)self;
    if(tag->internals.tag)
	return 1;
    if(!tag->internals.fillTAG) {
	PyErr_SetString(PyExc_Exception, setError("No way to fill TAG with data"));
	return 0;
    }
    if(!tag->internals.fillTAG(&tag->internals)) {
	return 0; // pass through exception
    }
    if(!tag->internals.tag) {
	PyErr_SetString(PyExc_Exception, setError("Couldn't fill tag"));
	return 0;
    }
    return 1;
}
//----------------------------------------------------------------------------
static PyObject* tag_isShape(PyObject * _self, PyObject*args)
{
    TagObject*self = (TagObject*)_self;
    if(!PyArg_ParseTuple(args, "")) return NULL;
    if(!fillTAG((PyObject*)self))   return NULL;
    return PyInt_FromLong(swf_isShapeTag(self->internals.tag));
}
static PyObject* tag_isFont(PyObject * _self, PyObject*args)
{
    TagObject*self = (TagObject*)_self;
    if(!PyArg_ParseTuple(args, "")) return NULL;
    if(!fillTAG((PyObject*)self))   return NULL;
    int id = self->internals.tag->id;
    int isfont=0;
    if(id == ST_DEFINEFONT || id == ST_DEFINEFONT2)
	isfont = 1;
    return PyInt_FromLong(isfont);
}
static PyObject* tag_isImage(PyObject * _self, PyObject*args)
{
    TagObject*self = (TagObject*)_self;
    if(!PyArg_ParseTuple(args, "")) return NULL;
    if(!fillTAG((PyObject*)self))   return NULL;
    return PyInt_FromLong(swf_isImageTag(self->internals.tag));
}
static PyObject* tag_isDefiningTag(PyObject * _self, PyObject*args)
{
    TagObject*self = (TagObject*)_self;
    if(!PyArg_ParseTuple(args, "")) return NULL;
    if(!fillTAG((PyObject*)self))   return NULL;
    return PyInt_FromLong(swf_isDefiningTag(self->internals.tag));
}
static PyObject* tag_isPlacement(PyObject * _self, PyObject*args)
{
    TagObject*self = (TagObject*)_self;
    if(!PyArg_ParseTuple(args, "")) return NULL;
    if(!fillTAG((PyObject*)self))   return NULL;
    return PyInt_FromLong((self->internals.tag->id == ST_PLACEOBJECT ||
	                   self->internals.tag->id == ST_PLACEOBJECT2));
}
static PyObject* tag_getBBox(PyObject * _self, PyObject*args)
{
    TagObject*self = (TagObject*)_self;
    if(!PyArg_ParseTuple(args, "")) return NULL;
    if(!fillTAG((PyObject*)self))   return NULL;
    return f_BBox2(swf_GetDefineBBox(self->internals.tag));
}
static PyObject* tag_setBBox(PyObject * _self, PyObject*args)
{
    TagObject*self = (TagObject*)_self;
    PyObject*bbox = 0;
    if(!PyArg_ParseTuple(args, "O!", &BBoxClass, &bbox)) return NULL;
    if(!fillTAG((PyObject*)self))   return NULL;
    swf_SetDefineBBox(self->internals.tag, bbox_getSRECT(bbox));
    return PY_NONE;
}
//----------------------------------------------------------------------------
static PyMethodDef common_tagfunctions[] =
{{"isShape", tag_isShape, METH_VARARGS, "tests whether the tag is a shape tag"},
 {"isImage", tag_isImage, METH_VARARGS, "tests whether the tag is an image"},
 {"isFont", tag_isFont, METH_VARARGS, "tests whether the tag is a font"},
 {"isDefiningTag", tag_isDefiningTag, METH_VARARGS, "tests whether the tag is a defining tag"},
 {"isPlacement", tag_isPlacement, METH_VARARGS, "tests whether the tag is a placement"},
 {"getBBox", tag_getBBox, METH_VARARGS, "get's the tags bounding box"},
 {"setBBox", tag_setBBox, METH_VARARGS, "set's the tags bounding box"},
 {NULL, NULL, 0, NULL}
};

static PyObject* tag_getattr(PyObject * self, char* a)
{
    TagObject*tag = (TagObject*)self;
    PyObject* ret = NULL;
    int t;

    /* -- fields -- */
    if(!strcmp(a, "tagid")) {
	if(!fillTAG(self))
	    return 0;
	return Py_BuildValue("i", tag->internals.tag->id);
    }
    if(!strcmp(a, "name")) {
	if(!fillTAG(self))
	    return 0;
	char* name = swf_TagGetName(tag->internals.tag);
	return Py_BuildValue("s", name);
    }
    if(!strcmp(a, "data")) {
	if(!fillTAG(self))
	    return 0;
	return Py_BuildValue("s#", tag->internals.tag->data, tag->internals.tag->len);
    }
    if(tag->internals.getattr) {
	PyObject* ret = tag->internals.getattr(&tag->internals, a);
	if(ret) return ret;
    }
    
    /* search for a tag specific function */
    if(tag->internals.tagfunctions) {
	mylog(" %08x(%d) tag_getattr: tag has specific functions\n", (int)self, self->ob_refcnt);
	ret = Py_FindMethod(tag->internals.tagfunctions, self, a);
	if(ret) return ret;
	PyErr_Clear();
	ret = FindMethodMore(ret, common_tagfunctions, self, a);
	mylog(" %08x(%d) tag_getattr %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
	if(ret) return ret;
	PyErr_Clear();
    }
  
    ret = Py_FindMethod(common_tagfunctions, self, a);

    mylog(" %08x(%d) tag_getattr %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
    return ret;
}
static int tag_setattr(PyObject * _self, char* a, PyObject * o)
{
    TagObject*self= (TagObject*)_self;
    /* a setattr will almost certainly change the tag data,
       so delete the tag */
    if(self->internals.tag) {
	swf_DeleteTag(0, self->internals.tag);
	self->internals.tag = 0;
    }
    if(self->internals.setattr) {
	int ret = self->internals.setattr(&self->internals, a, o);
	return ret;
    }
    return 1;
}
//----------------------------------------------------------------------------
//                     Tag Constructors
//----------------------------------------------------------------------------
PyObject* tag_new(tag_internals_t*tag_internals)
{
    TagObject*tag = PyObject_New(TagObject, &TagClass);
    mylog("+%08x(%d) tag_new\n", (int)tag, tag->ob_refcnt);
    memcpy(&tag->internals, tag_internals, sizeof(tag_internals_t));
    if(tag->internals.datasize) {
	tag->internals.data = malloc(tag->internals.datasize);
	memset(tag->internals.data , 0, tag->internals.datasize);
    } else {
	tag->internals.data = 0;
    }
    tag->internals.tag = 0;
    tag->internals.tagmap = tagmap_new();

    return (PyObject*)tag;
}
PyObject* tag_new2(TAG*t, PyObject* tagmap)
{
    TagObject*tag = PyObject_New(TagObject, &TagClass);
    mylog("+%08x(%d) tag_new2 tag=%08x id=%d (%s)\n", (int)tag, tag->ob_refcnt, t, t->id, swf_TagGetName(t));
    
    PyObject*mytagmap = tagmap_new();

    int num = swf_GetNumUsedIDs(t);
    if(num) { // tag has dependencies
	int * positions = malloc(num*sizeof(int));
	swf_GetUsedIDs(t, positions);
	int i;
	for(i=0;i<num;i++) {
	    int id = GET16(&t->data[positions[i]]);
	    PyObject*obj = tagmap_id2obj(tagmap, id);
	    if(obj==NULL) {
		PyErr_SetString(PyExc_Exception, setError("TagID %d not defined", id));
		return NULL;
	    }
	    //mylog("+%08x(%d) tag_new2 handling id %d at %d/%d\n", (int)tag, tag->ob_refcnt, id, positions[i], t->len);
	    //mylog("+%08x(%d) tag_new2 add dependency %d to id %d, object %08x(%d)\n", (int)tag, tag->ob_refcnt, i, id, obj, obj->ob_refcnt);
	    tagmap_addMapping(mytagmap, id, obj);
	}
	free(positions);
    }

    tag_internals_t*spec = get_parser(t->id);
    if(spec) {
	memcpy(&tag->internals, spec, sizeof(tag_internals_t));
    } else {
	memcpy(&tag->internals, &generic_tag, sizeof(tag_internals_t));
    }
    if(tag->internals.datasize) {
	tag->internals.data = malloc(tag->internals.datasize);
	memset(tag->internals.data, 0, tag->internals.datasize);
    } else {
	tag->internals.data = 0;
    }
    tag->internals.tag = swf_InsertTag(0, t->id);
    swf_SetBlock(tag->internals.tag, t->data, t->len);
    tag->internals.tagmap = mytagmap;

    // call tag->internals.init()?

    return (PyObject*)tag;
}
//----------------------------------------------------------------------------
/* serialize */
TAG* tag_getTAG(PyObject*self, TAG*prevTag, PyObject*tagmap)
{
    TagObject*tag = (TagObject*)self;

    if(!fillTAG(self))
	return 0;
    mylog(" %08x(%d) tag_getTAG: tag=%08x id=%d (%s)", (int)self, self->ob_refcnt, tag->internals.tag, tag->internals.tag->id, swf_TagGetName(tag->internals.tag));

    TAG* t = swf_InsertTag(prevTag, tag->internals.tag->id);
    swf_SetBlock(t, tag->internals.tag->data, tag->internals.tag->len);
    
    if(swf_isDefiningTag(t)) {
	int newid = tagmap_add(tagmap, self);
	swf_SetDefineID(t, newid);
    }

    int num = swf_GetNumUsedIDs(t);
    if(num) { // tag has dependencies
	int * positions = malloc(num*sizeof(int));
	swf_GetUsedIDs(t, positions);
	int i;
	for(i=0;i<num;i++) {
	    int id = GET16(&t->data[positions[i]]);
	    PyObject* obj =  tagmap_id2obj(tag->internals.tagmap, id);
	    if(obj==NULL) {
		PyErr_SetString(PyExc_Exception, setError("Internal error: id %d not known in taglist:"));
		free(positions);
		return 0;
	    }
	    //int newid = tagmap_obj2id(tag->internals.tagmap, obj);
	    int newid = tagmap_obj2id(tagmap, obj);
	    if(newid>=0) {
		mylog(" %08x(%d) tag_getTAG: dependency %d) %d->%08x -> assigning(%08x) id %d", (int)self, self->ob_refcnt, i, id, obj, tagmap, newid);
	    } else {
		/* TODO: this is only needed for sprites, so maybe it should throw an
		   exception otherwise */
		newid = tagmap_add(tagmap, obj);
		mylog(" %08x(%d) tag_getTAG: added dependency %d) %d->%08x -> assigning(%08x) id %d", (int)self, self->ob_refcnt, i, id, obj, tagmap, newid);
	    }
	    PUT16(&t->data[positions[i]], newid);
	}
	free(positions);
    }
    return t;
}
//----------------------------------------------------------------------------
tag_internals_t* tag_getinternals(PyObject*self)
{
    TagObject*tag = (TagObject*)self;
    mylog(" %08x(%d) tag_getInternals\n", (int)self, self->ob_refcnt);
    return &tag->internals;
}
//----------------------------------------------------------------------------
PyObject* tag_getDependencies(PyObject*self)
{
    TagObject*tag = (TagObject*)self;
    mylog(" %08x(%d) tag_getDependencies\n", (int)self, self->ob_refcnt);
    return tagmap_getObjectList(tag->internals.tagmap);
}
//----------------------------------------------------------------------------
int tag_print(PyObject * self, FILE * fi, int flags)
{
    TagObject*tag = (TagObject*)self;
    mylog(" %08x(%d) tag_print flags=%08x\n", (int)self, self->ob_refcnt, flags);
    if(!fillTAG(self))
	return -1;
    //fprintf(fi, "tag-%08x-%d-%s", (int)tag->internals.tag, tag->internals.tag->id, swf_TagGetName(tag->internals.tag));
    fprintf(fi, "%s", swf_TagGetName(tag->internals.tag));
    return 0;
}
//----------------------------------------------------------------------------
PyTypeObject TagClass = 
{
    PyObject_HEAD_INIT(NULL)
    0,
    tp_name: "Tag",
    tp_basicsize: sizeof(TagObject),
    tp_itemsize: 0,
    tp_dealloc: tag_dealloc,
    tp_print: tag_print,
    tp_getattr: tag_getattr,
    tp_setattr: tag_setattr,
};
