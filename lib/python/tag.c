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

struct _TagObject;

typedef struct _tag_internals
{
    int size;
    int (*parse)(struct _TagObject*);
    int (*fillTAG)(struct _TagObject*);
    void (*dealloc)(struct _TagObject*);
    void*data;
    PyMethodDef* tagfunctions;
} tag_internals_t;

typedef struct _TagObject {
    PyObject_HEAD
    
    PyObject* tagmap;
    TAG*tag;
    tag_internals_t internals;
} TagObject;

//----------------------------------------------------------------------------

PyObject* tag_new(tag_internals_t*tag_internals);
   
typedef struct _font_internal
{
    SWFFONT* font;
} font_internal_t;
staticforward tag_internals_t font_tag;

static int font_parse(TagObject*self)
{
    font_internal_t*font = (font_internal_t*)self->internals.data;
    /* TODO */
    PyErr_SetString(PyExc_Exception, setError("Font parsing not implemented yet"));
    return 0;
}
static void font_dealloc(TagObject*self)
{
    font_internal_t*font = (font_internal_t*)self->internals.data;
    if(font->font) {
	swf_FontFree(font->font);
	font->font = 0;
    }
}
static int font_fillTAG(TagObject*self)
{
    font_internal_t*fi = (font_internal_t*)self->internals.data;
    if(self->tag)
	return 1;
    self->tag = swf_InsertTag(0, ST_DEFINEFONT2);
    swf_FontSetDefine2(self->tag, fi->font);
    return 1;
}
static PyObject* f_DefineFont(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"filename", NULL};
    char*filename = 0;
    TagObject*tag;
    SWFFONT* font;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &filename))
	return NULL;

    font = swf_LoadFont(filename);
    if(!font) {
	PyErr_SetString(PyExc_Exception, setError("Could not load %s", filename));
	return NULL;
    }

    tag = (TagObject*)tag_new(&font_tag);
    font_internal_t*fi = (font_internal_t*)tag->internals.data;
    font->id = 0;
    fi->font = font;
    return (PyObject*)tag;
}
static SWFFONT* font_getSWFFONT(PyObject*self)
{
    PY_ASSERT_TYPE(self, &TagClass);
    TagObject*tag = (TagObject*)self;
    font_internal_t*fi = (font_internal_t*)tag->internals.data;
    return fi->font;
}
static tag_internals_t font_tag =
{
    size: sizeof(font_internal_t),
    parse: font_parse,
    fillTAG: font_fillTAG,
    dealloc: font_dealloc,
    tagfunctions: 0
};
//----------------------------------------------------------------------------

typedef struct _placeobject_internal
{
    SWFPLACEOBJECT* po;
} placeobject_internal_t;
staticforward tag_internals_t placeobject_tag;

static void po_dealloc(TagObject*self)
{
    placeobject_internal_t*pi = (placeobject_internal_t*)self->internals.data;
    if(pi->po) {
	swf_PlaceObjectFree(pi->po);
	pi->po = 0;
    }
}
static int po_parse(TagObject*self)
{
    placeobject_internal_t*pi = (placeobject_internal_t*)self->internals.data;
    /* TODO */
    PyErr_SetString(PyExc_Exception, setError("Font parsing not implemented yet"));
    return 0;
}
static int po_fillTAG(TagObject*self)
{
    placeobject_internal_t*pi = (placeobject_internal_t*)self->internals.data;
    self->tag = swf_InsertTag(0, ST_PLACEOBJECT2);
    swf_SetPlaceObject(self->tag, pi->po);
    return 1;
}
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
		&ActionClass, &action
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

    tag = (TagObject*)tag_new(&placeobject_tag);
    placeobject_internal_t*pi = (placeobject_internal_t*)tag->internals.data;
    pi->po = po;
    pi->po->id = tagmap_add(tag->tagmap,(PyObject*)character);
    
    mylog("+%08x(%d) PlaceObject %08x(%d)\n", (int)tag, tag->ob_refcnt, character, character->ob_refcnt);

    return (PyObject*)tag;
}
static tag_internals_t placeobject_tag =
{
    size: sizeof(placeobject_internal_t),
    parse: po_parse,
    fillTAG: po_fillTAG,
    dealloc: po_dealloc,
    tagfunctions: 0
};
//----------------------------------------------------------------------------
staticforward tag_internals_t bgcolor_tag;
static PyObject* tag_setbackgroundcolor_getrgb(PyObject * self, PyObject*other)
{
    TagObject*tag = (TagObject*)self;
    int r,g,b;
    r = tag->tag->data[0];
    g = tag->tag->data[1];
    b = tag->tag->data[2];
    return Py_BuildValue("(iii)", r,g,b);
}
static PyMethodDef setbgcolor_methods[] = 
{{"getRGB", tag_setbackgroundcolor_getrgb, METH_VARARGS, "get's the color set by this tag"},
 {NULL, NULL, 0, NULL}
};
static PyObject* f_SetBackgroundColor(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"color", NULL};
    int r=0,g=0,b=0;
    TagObject*tag;
    PyObject*color;
    
    tag = (TagObject*)tag_new(&bgcolor_tag);

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
    mylog(" %08x(%d) SetBackgroundColor(%02x,%02x,%02x) (colorobj=%08x(%d))\n", (int)tag, tag->ob_refcnt, rgba.r, rgba.g, rgba.b, color, color->ob_refcnt);
    return (PyObject*)tag;
}
static tag_internals_t bgcolor_tag =
{
    size: 0,
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    tagfunctions: setbgcolor_methods
};
//----------------------------------------------------------------------------
staticforward tag_internals_t protect_tag;
static PyObject* f_Protect(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"password", NULL};
    char*password = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &password))
	return NULL;

    TagObject*tag = (TagObject*)tag_new(&protect_tag);
    tag->tag = swf_InsertTag(0, ST_PROTECT);
    if(password) {
	swf_SetPassword(tag->tag, password);
    }
    mylog("+%08x(%d) f_Protect", (int)tag, tag->ob_refcnt);
    return (PyObject*)tag;
}
static tag_internals_t protect_tag =
{
    size: 0,
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    tagfunctions: 0
};
//----------------------------------------------------------------------------
staticforward tag_internals_t text_tag;

typedef struct _text_internal // not used yet
{
    int font_id;
    char*text;
    SWFFONT* swffont;
    RGBA rgba;
    int size;
} text_internal_t;
staticforward tag_internals_t placeobject_tag;

static int text_fillTAG(TagObject*self) //not used yet
{
    text_internal_t*ti = (text_internal_t*)self->internals.data;
    self->tag= swf_InsertTag(0, ST_DEFINETEXT2);
    swf_SetU16(self->tag, /*ID*/0);
    SRECT r = swf_SetDefineText(self->tag, ti->swffont, &ti->rgba, ti->text, ti->size);
    return 0;
}
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
    PyObject*font = 0;
    SRECT r;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!u#i|O!", kwlist, &TagClass, &font, &text, &textlen, &size, &ColorClass, &color))
	return NULL;
    
    unicode16 = PyUnicode_DecodeUTF16(text, textlen*2, NULL, NULL);
    unicode8 = PyUnicode_AsUTF8String(unicode16);
    text = PyString_AS_STRING(unicode8);

    if(color)
	rgba = color_getRGBA(color);

    mylog("DefineText: text = %s", text);
    
    tag = (TagObject*)tag_new(&text_tag);
    text_internal_t*ti = (text_internal_t*)tag->internals.data;

    SWFFONT* swffont = font_getSWFFONT(font);
    int font_id = tagmap_add(tag->tagmap, font); // add dependency on font
    swffont->id = font_id; // for swf_SetDefineText

    /* todo: remove, store data in ti */
    tag->tag= swf_InsertTag(0, ST_DEFINETEXT2);
    swf_SetU16(tag->tag, /*ID*/0);
    r = swf_SetDefineText(tag->tag, swffont, &rgba, text, size);
    mylog("+%08x(%d) DefineText tag=%08x \n", (int)tag, tag->ob_refcnt);
    
    //Py_DECREF(unicode16);
    //Py_DECREF(unicode8);
    //free(text);

    return (PyObject*)tag;
}
static tag_internals_t text_tag =
{
    size: sizeof(text_internal_t),
    parse: 0,
    fillTAG: text_fillTAG,
    dealloc: 0,
    tagfunctions: 0
};
//----------------------------------------------------------------------------
static PyMethodDef generic_methods[] = 
{
  {NULL, NULL, 0, NULL}
};
static tag_internals_t generic_tag =
{
    size: 0,
    parse: 0,
    dealloc: 0,
    fillTAG: 0,
    tagfunctions: generic_methods
};
//----------------------------------------------------------------------------

struct tag_parser {
    int id;
    tag_internals_t*spec;
} tag_parsers[] = {
    {ST_PLACEOBJECT,&placeobject_tag},
    {ST_PLACEOBJECT2,&placeobject_tag},
    {ST_SETBACKGROUNDCOLOR,&bgcolor_tag},
    {ST_DEFINEFONT,&font_tag},
    {ST_PROTECT,&protect_tag},
    {ST_DEFINETEXT,&text_tag},
    {-1,0}
};
//----------------------------------------------------------------------------
static void tag_dealloc(PyObject * self)
{
    TagObject*tag = (TagObject*)self;
    if(tag->tag)
	mylog("-%08x(%d) tag_dealoc [%s]\n", (int)self, self->ob_refcnt, swf_TagGetName(tag->tag));
    else
	mylog("-%08x(%d) tag_dealoc [?]\n", (int)self, self->ob_refcnt);
    if(tag->internals.dealloc) {
	if(!tag->internals.data)
	    mylog("-%08x(%d) tag_dealoc: Warning: calling dealloc without any data(?)\n", (int)self, self->ob_refcnt);
	tag->internals.dealloc(tag);
    }
    if(tag->internals.data) {
	free(tag->internals.data);
	tag->internals.data = 0;
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
static PyMethodDef common_tagfunctions[] =
{{"setU8", tag_setU8, METH_VARARGS, "sets a byte to the tag data"},
 {NULL, NULL, 0, NULL}
};

static int fillTAG(PyObject*self) 
{
    TagObject*tag = (TagObject*)self;
    if(tag->tag)
	return 1;
    if(!tag->internals.fillTAG) {
	PyErr_SetString(PyExc_Exception, setError("No way to fill TAG with data"));
	return 0;
    }
    if(!tag->internals.fillTAG(tag)) {
	return 0; // pass through exception
    }
    if(!tag->tag) {
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
    if(!strcmp(a, "id")) {
	if(!fillTAG(self))
	    return 0;
	return Py_BuildValue("i", tag->tag->id);
    }
    if(!strcmp(a, "name")) {
	if(!fillTAG(self))
	    return 0;
	char* name = swf_TagGetName(tag->tag);
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
    if(tag->internals.size) {
	tag->internals.data = malloc(tag->internals.size);
	memset(tag->internals.data , 0, tag->internals.size);
    } else {
	tag->internals.data = 0;
    }
    tag->tag = 0;
    tag->tagmap = tagmap_new();

    return (PyObject*)tag;
}
PyObject* tag_new2(TAG*t, PyObject* tagmap)
{
    TagObject*tag = PyObject_New(TagObject, &TagClass);
    mylog("+%08x(%d) tag_new2 tag=%08x id=%d (%s)\n", (int)tag, tag->ob_refcnt, t, t->id, swf_TagGetName(t));
    
    tag->tagmap = tagmap_new();

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
	    tagmap_addMapping(tag->tagmap, id, obj);
	}
	free(positions);
    }

    tag->tag = swf_InsertTag(0, t->id);
    swf_SetBlock(tag->tag, t->data, t->len);

    int i=0,found=0;
    while(tag_parsers[i].id>=0) {
	if(tag_parsers[i].id == t->id) {
	    memcpy(&tag->internals, tag_parsers[i].spec, sizeof(tag_internals_t));
	    found=1;
	    break;
	}
	i++;
    }
    if(!found) {
	memcpy(&tag->internals, &generic_tag, sizeof(tag_internals_t));
    }
    if(tag->internals.size) {
	tag->internals.data = malloc(tag->internals.size);
	memset(tag->internals.data, 0, tag->internals.size);
    } else {
	tag->internals.data = 0;
    }
    return (PyObject*)tag;
}
//----------------------------------------------------------------------------
/* serialize */
TAG* tag_getTAG(PyObject*self, TAG*prevTag, PyObject*tagmap)
{
    TagObject*tag = (TagObject*)self;

    if(!fillTAG(self))
	return 0;
    mylog(" %08x(%d) tag_getTAG: tag=%08x id=%d (%s)", (int)self, self->ob_refcnt, tag->tag, tag->tag->id, swf_TagGetName(tag->tag));

    TAG* t = swf_InsertTag(prevTag, tag->tag->id);
    swf_SetBlock(t, tag->tag->data, tag->tag->len);
    
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
	    PyObject* obj =  tagmap_id2obj(tag->tagmap, id);
	    if(obj==NULL) {
		PyErr_SetString(PyExc_Exception, setError("Internal error: id %d not known in taglist:"));
		free(positions);
		return 0;
	    }
	    int newid = tagmap_obj2id(tagmap, obj);
	    PUT16(&t->data[positions[i]], newid);
	}
	free(positions);
    }
    return t;
}
PyObject* tag_getDependencies(PyObject*self)
{
    TagObject*tag = (TagObject*)self;
    mylog(" %08x(%d) tag_getDependencies\n", (int)self, self->ob_refcnt);
    return tagmap_getObjectList(tag->tagmap);
}
int tag_print(PyObject * self, FILE * fi, int flags)
{
    TagObject*tag = (TagObject*)self;
    mylog(" %08x(%d) tag_print flags=%08x\n", (int)self, self->ob_refcnt, flags);
    if(!fillTAG(self))
	return -1;
    fprintf(fi, "tag-%08x-%d-%s", (int)tag->tag, tag->tag->id, swf_TagGetName(tag->tag));
    return 0;
}
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

