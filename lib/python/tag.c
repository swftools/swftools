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
typedef struct {
    PyObject_HEAD
    PyObject* tagmap;
    TAG*tag;
    /* ST_DEFINEFONT*/
    SWFFONT* font;
    /* ST_PLACEOBJECT, ST_PLACEOBJECT2*/
    SWFPLACEOBJECT* placeobject;
    PyObject* character;
} TagObject;

//----------------------------------------------------------------------------
static void tag_dealloc(PyObject * self)
{
    TagObject*tag = (TagObject*)self;
    mylog("-%08x(%d) tag_dealoc\n", (int)self, self->ob_refcnt);
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
    if(tag->tag) {
	swf_DeleteTag(tag->tag);
	tag->tag = 0;
    }
    Py_DECREF(tag->tagmap);
    tag->tagmap = 0;
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

    /* -- fields -- */
    if(!strcmp(a, "id")) {
	return Py_BuildValue("i", id);
    }
    if(!strcmp(a, "name")) {
	char* name = swf_TagGetName(tag->tag);
	return Py_BuildValue("s", name);
    }
/*    if(!strcmp(a, "swf")) {
	if(tag->swf == 0)
	    return Py_BuildValue("s", 0);
	else {
	    Py_INCREF(tag->swf);
	    return tag->swf;
	}
    }*/
    /* ------------ */
   
    /* search for a tag specific function */
    for(t=0;t<sizeof(tagfunctions)/sizeof(tagfunctions[0]);t++)
    {
	if(id==tagfunctions[t].id) {
	    mylog(" %08x(%d) tag_getattr: id %d found\n", (int)self, self->ob_refcnt, id);
	    ret = Py_FindMethod(tagfunctions[t].f, self, a);
	    if(!ret) return ret;
	    ret = FindMethodMore(ret, common_tagfunctions, self, a);
	    mylog(" %08x(%d) tag_getattr %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
	    return ret;
	}
    }
   
    ret = Py_FindMethod(common_tagfunctions, self, a);

    mylog(" %08x(%d) tag_getattr %s: %08x\n", (int)self, self->ob_refcnt, a, ret);
    return ret;
}
//----------------------------------------------------------------------------
//                     Tag Contructors
//----------------------------------------------------------------------------
PyObject* tag_new()
{
    TagObject*tag = PyObject_New(TagObject, &TagClass);
    mylog("+%08x(%d) tag_new\n", (int)tag, tag->ob_refcnt);
    tag->font = 0;
    tag->character = 0;
    tag->placeobject = 0;
    tag->tag = 0;
    tag->tagmap = tagmap_new();
    return (PyObject*)tag;
}
PyObject* tag_new2(TAG*t, PyObject* tagmap)
{
    TagObject*tag = PyObject_New(TagObject, &TagClass);
    mylog("+%08x(%d) tag_new tag=%08x\n", (int)tag, tag->ob_refcnt, t);
    tag->font = 0;
    tag->character = 0;
    tag->placeobject = 0;
    tag->tagmap = tagmap_new();
    // copy tag
    tag->tag = swf_InsertTag(0, t->id);
    swf_SetBlock(tag->tag, t->data, t->len);
    t = tag->tag;
    
    int num = swf_GetNumUsedIDs(t);
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
	tagmap_addMapping(tag->tagmap, id, obj);
    }
    free(positions);
    return (PyObject*)tag;
}

static PyObject* f_SetBackgroundColor(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"color", NULL};
    int r=0,g=0,b=0;
    TagObject*tag;
    PyObject*color;
    
    tag = (TagObject*)tag_new();

    /* 1st try- copy constructor */
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", kwlist, &ColorClass, &color)) {
	/* 2nd try- color's contructor */
	color = f_Color(NULL, args, kwargs);
    }
    if(!color)
	return NULL;

    tag->tag = swf_InsertTag(0, ST_SETBACKGROUNDCOLOR);
    RGBA rgba = color_getRGBA(color);
    swf_SetU8(tag->tag, rgba.r);
    swf_SetU8(tag->tag, rgba.g);
    swf_SetU8(tag->tag, rgba.b);
    mylog(" %08x(%d) SetBackgroundColor(%02x,%02x,%02x)\n", (int)tag, tag->ob_refcnt, rgba.r, rgba.g, rgba.b);
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

    font = swf_LoadFont(filename);
    mylog("font=%08x",font);
    if(!font) {
	PyErr_SetString(PyExc_Exception, setError("Could not load %s", filename));
	return NULL;
    }

    tag = (TagObject*)tag_new();
    tag->font = font;
    tag->tag = swf_InsertTag(0, ST_DEFINEFONT2);
    tag->font->id = 0;
    swf_FontSetDefine2(tag->tag, tag->font);
    mylog("+%08x(%d) DefineFont\n", (int)tag, tag->ob_refcnt);
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

    tag = (TagObject*)tag_new();
    tag->tag = swf_InsertTag(0, ST_PROTECT);
    if(password) {
	swf_SetPassword(tag->tag, password);
    }
    mylog("+%08x(%d) f_Protect", (int)tag, tag->ob_refcnt);
    return (PyObject*)tag;
}
//----------------------------------------------------------------------------
static PyObject* f_DefineText(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"font", "text", "size", "color", NULL};
    TagObject*tag = 0;
    char*text = 0;
    int textlen = 0;
    PyObject*unicode16;
    PyObject*unicode8;
    int size = 0;
    RGBA rgba = {255,0,0,0};
    PyObject*color = 0;
    TagObject*font = 0;
    SRECT r;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!u#i|O!", kwlist, &TagClass, &font, &text, &textlen, &size, &ColorClass, &color))
	return NULL;
    
    unicode16 = PyUnicode_DecodeUTF16(text, textlen*2, NULL, NULL);
    unicode8 = PyUnicode_AsUTF8String(unicode16);
    text = PyString_AS_STRING(unicode8);

    if(color)
	rgba = color_getRGBA(color);

    mylog("DefineText: text = %s", text);
    
    tag = (TagObject*)tag_new();

    /* notice: we do modify the (passed) font object here, 
       for the swf_SetDefineText call, who will write out the font id. */
    font->font->id = tagmap_add(tag->tagmap,(PyObject*)font); // add dependency on font

    tag ->tag= swf_InsertTag(0, ST_DEFINETEXT2);
    swf_SetU16(tag->tag, /*ID*/0);
    r = swf_SetDefineText(tag->tag, font->font, &rgba, text, size);
    mylog("+%08x(%d) DefineText %08x -> %08x\n", (int)tag, tag->ob_refcnt);

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
    PyObject*matrix = 0;
    PyObject*cxform = 0;
    PyObject*action = 0;
    int ratio = 0;
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
    if(clipdepth) po->clipdepth = clipdepth;
    if(matrix) po->matrix = matrix_getMatrix(matrix);
    if(cxform) po->cxform = colortransform_getCXForm(cxform);
    if(action) po->actions = action_getAction(action);

    tag = (TagObject*)tag_new();
    tag->placeobject = po;
    Py_INCREF(character);
    tag->character = (PyObject*)character;
    tag->tag= swf_InsertTag(0, ST_PLACEOBJECT2);

    po->id = tagmap_add(tag->tagmap,(PyObject*)character);

    swf_SetPlaceObject(tag->tag, po);
    mylog("+%08x(%d) PlaceObject %08x\n", (int)tag, tag->ob_refcnt, character);
    return (PyObject*)tag;
}

/* serialize */
TAG* tag_getTAG(PyObject*self, TAG*prevTag, PyObject*tagmap)
{
    TagObject*tag = (TagObject*)self;
    
    TAG* t = swf_InsertTag(prevTag, tag->tag->id);
    swf_SetBlock(t, tag->tag->data, tag->tag->len);

    mylog(" %08x(%d) tag_getTAG tagmap=%08x tag=%08x/%08x\n", (int)self, self->ob_refcnt, tagmap, tag->tag, t);

    if(swf_isDefiningTag(t)) {
	int newid = tagmap_add(tagmap, self);
	swf_SetDefineID(t, newid);
    }

    int num = swf_GetNumUsedIDs(t);
    int * positions = malloc(num*sizeof(int));
    swf_GetUsedIDs(t, positions);
    int i;
    for(i=0;i<num;i++) {
	int id = GET16(&t->data[positions[i]]);
	PyObject* obj =  tagmap_id2obj(tag->tagmap, id);
	if(obj==NULL) {
	    PyErr_SetString(PyExc_Exception, setError("Internal error: id %d not known in taglist:"));
	    return 0;
	}
	int newid = tagmap_obj2id(tagmap, obj);
	PUT16(&t->data[positions[i]], newid);
    }
    return t;
}

PyObject* tag_getDependencies(PyObject*self)
{
    mylog(" %08x(%d) tag_getDependencies\n", (int)self, self->ob_refcnt);
    TagObject*tag = (TagObject*)self;
    return tagmap_getObjectList(tag->tagmap);
}

PyTypeObject TagClass = 
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
static PyMethodDef TagMethods[] = 
{
    /* TAGS */
    {"SetBackgroundColor", (PyCFunction)f_SetBackgroundColor, METH_KEYWORDS, "Create a SetBackGroundColor Tag."},
    {"Protect", (PyCFunction)f_Protect, METH_KEYWORDS, "Create a Protect Tag."},
    {"DefineFont", (PyCFunction)f_DefineFont, METH_KEYWORDS, "Create a DefineFont Tag."},
    {"DefineText", (PyCFunction)f_DefineText, METH_KEYWORDS, "Create a DefineText Tag."},
    {"PlaceObject", (PyCFunction)f_PlaceObject, METH_KEYWORDS, "Create a PlaceObject Tag."},
    {NULL, NULL, 0, NULL}
};
PyMethodDef* tag_getMethods()
{
    TagClass.ob_type = &PyType_Type;
    return TagMethods;
}

