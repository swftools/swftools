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
	swf_DeleteTag(tag->internals.tag);
	tag->internals.tag = 0;
    }
    Py_DECREF(tag->internals.tagmap);
    tag->internals.tagmap = 0;
    PyObject_Del(self);
}
//----------------------------------------------------------------------------
static PyObject* tag_setU8(PyObject * self, PyObject*other)
{
    return NULL;
}
//----------------------------------------------------------------------------
static PyMethodDef common_tagfunctions[] =
{{"setU8", tag_setU8, METH_VARARGS, "sets a byte to the tag data"},
 {NULL, NULL, 0, NULL}
};

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
    
    /* search for a tag specific function */
    if(tag->internals.tagfunctions) {
	mylog(" %08x(%d) tag_getattr: tag has specific functions\n", (int)self, self->ob_refcnt);
	ret = Py_FindMethod(tag->internals.tagfunctions, self, a);
	if(!ret) return ret;
	ret = FindMethodMore(ret, common_tagfunctions, self, a);
	mylog(" %08x(%d) tag_getattr %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
	return ret;
    }
   
    ret = Py_FindMethod(common_tagfunctions, self, a);

    mylog(" %08x(%d) tag_getattr %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
    return ret;
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
	    if(newid<0) {
		PyErr_SetString(PyExc_Exception, setError("Unknown object %08x", obj));return 0;
	    }
	    mylog(" %08x(%d) tag_getTAG: dependency %d) %d->%08x -> assigning(%08x) id %d", (int)self, self->ob_refcnt, i, id, obj, tagmap, newid);
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
	fprintf(fi, "tag-%08x-%d-%s", (int)tag->internals.tag, tag->internals.tag->id, swf_TagGetName(tag->internals.tag));
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
};
