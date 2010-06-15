#include "pyutils.h"
#include "primitives.h"
#include "action.h"
#include "taglist.h"
#include "tag.h"
#include "tags.h"
#include "image.h"
#include "../png.h"

//----------------------------------------------------------------------------

typedef struct _font_internal
{
    SWFFONT* font;
} font_internal_t;
staticforward tag_internals_t font_tag;

static int font_parse(tag_internals_t*self)
{
    font_internal_t*font = (font_internal_t*)self->data;
    /* TODO */
    PyErr_SetString(PyExc_Exception, setError("Font parsing not implemented yet"));
    return 0;
}
static void font_dealloc(tag_internals_t*self)
{
    font_internal_t*font = (font_internal_t*)self->data;
    if(font->font) {
	swf_FontFree(font->font);
	font->font = 0;
    }
}
static int font_fillTAG(tag_internals_t*self)
{
    font_internal_t*fi = (font_internal_t*)self->data;
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
    PyObject*tag;
    SWFFONT* font;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &filename))
	return NULL;

    font = swf_LoadFont(filename, 0);
    if(!font) {
	PyErr_SetString(PyExc_Exception, setError("Could not load %s", filename));
	return NULL;
    }

    tag = tag_new(&font_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    font_internal_t*fi = (font_internal_t*)itag->data;
    font->id = 0;
    fi->font = font;
    return (PyObject*)tag;
}
static SWFFONT* font_getSWFFONT(PyObject*self)
{
    PY_ASSERT_TYPE(self, &TagClass);
    tag_internals_t*itag = tag_getinternals(self);
    font_internal_t*fi = (font_internal_t*)itag->data;
    return fi->font;
}
static tag_internals_t font_tag =
{
    parse: font_parse,
    fillTAG: font_fillTAG,
    dealloc: font_dealloc,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: sizeof(font_internal_t),
};
//----------------------------------------------------------------------------

typedef struct _placeobject_internal
{
    SWFPLACEOBJECT* po;
    PyObject*character;
} placeobject_internal_t;
staticforward tag_internals_t placeobject_tag;

static void po_dealloc(tag_internals_t*self)
{
    placeobject_internal_t*pi = (placeobject_internal_t*)self->data;
    if(pi->po) {
	swf_PlaceObjectFree(pi->po);
	pi->po = 0;
    }
}
static int po_parse(tag_internals_t*self)
{
    placeobject_internal_t*i = (placeobject_internal_t*)self->data;
    if(i->po)
	return 1;
    if(!self->tag)
	return 0;
    SWFPLACEOBJECT* swfpo = malloc(sizeof(SWFPLACEOBJECT));
    swf_GetPlaceObject(self->tag, swfpo);
    i->po = swfpo;
    swf_DeleteTag(0, self->tag);self->tag = 0;
	
    if(i->po->id) {
	i->character = tagmap_id2obj(self->tagmap, i->po->id);
	if(i->character) {
	    Py_INCREF(i->character);
	} else {
	    //PyErr_Clear(); //?
	}
    }
    return 1;
}
static int po_fillTAG(tag_internals_t*self)
{
    placeobject_internal_t*pi = (placeobject_internal_t*)self->data;
    self->tag = swf_InsertTag(0, ST_PLACEOBJECT2);
    swf_SetPlaceObject(self->tag, pi->po);
    return 1;
}
static PyObject* po_getattr(tag_internals_t*self,char*a)
{
    placeobject_internal_t*i = (placeobject_internal_t*)self->data;
    if(!po_parse(self))
	return PY_ERROR("Couldn't parse placeobject");
    if(!strcmp(a, "character")) {
	if(!i->character)
	    return PY_NONE;
	Py_INCREF(i->character); //TODO: ??
	return i->character;
    } else if(!strcmp(a, "matrix")) {
	return f_Matrix2(&i->po->matrix);
    } else if(!strcmp(a, "cxform")) {
	/* TODO */
	return 0;
    }
    return 0;
}
static int po_setattr(tag_internals_t*self,char*a, PyObject*obj)
{
    placeobject_internal_t*si = (placeobject_internal_t*)self->data;
    if(!strcmp(a, "cxform")) {
	/* TODO */
	return 0;
    }
    return -1;
}
static PyObject* po_create(PyObject* self, PyObject* args, PyObject* kwargs,char move)
{
    static char *kwlist[] = {"character", "depth", "matrix", "colortransform", "ratio", "name", "clipdepth", "action", NULL};
    
    PyObject*character = 0;
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
   
    PyErr_Clear();
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Oi|O!O!isiO!", kwlist, 
		&character, 
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
    po->clipdepth = clipdepth;
    po->ratio = ratio;
    po->name = name;
    po->move = move;
    if(clipdepth) po->clipdepth = clipdepth;
    if(matrix) po->matrix = matrix_getMatrix(matrix);
    if(cxform) po->cxform = colortransform_getCXForm(cxform);
    if(action) po->actions = action_getAction(action);

    PyObject*tag;
    tag = tag_new(&placeobject_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    placeobject_internal_t*pi = (placeobject_internal_t*)itag->data;
    pi->po = po;
    if(!move) {
	pi->po->id = tagmap_add(itag->tagmap,(PyObject*)character);
    } else {
	pi->po->id = 0;
    }
    
    mylog("+%08x(%d) PlaceObject %08x(%d)\n", (int)tag, tag->ob_refcnt, character, character->ob_refcnt);

    return (PyObject*)tag;
}
static PyObject* f_PlaceObject(PyObject* self, PyObject* args, PyObject* kwargs)
{
    return po_create(self, args, kwargs, 0);
}
static PyObject* f_MoveObject(PyObject* self, PyObject* args, PyObject* kwargs)
{
    return po_create(self, args, kwargs, 1);
}
static tag_internals_t placeobject_tag =
{
    parse: po_parse,
    fillTAG: po_fillTAG,
    dealloc: po_dealloc,
    getattr: po_getattr, 
    setattr: po_setattr,
    tagfunctions: 0,
    datasize: sizeof(placeobject_internal_t),
};
//----------------------------------------------------------------------------
staticforward tag_internals_t bgcolor_tag;
static PyObject* tag_setbackgroundcolor_getrgb(PyObject * self, PyObject*other)
{
    tag_internals_t*itag = tag_getinternals(self);
    int r,g,b;
    r = itag->tag->data[0];
    g = itag->tag->data[1];
    b = itag->tag->data[2];
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
    PyObject*tag;
    PyObject*color;
    
    tag = tag_new(&bgcolor_tag);
    tag_internals_t*itag = tag_getinternals(tag);

    /* 1st try- copy constructor */
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", kwlist, &ColorClass, &color)) {
	PyErr_Clear();
	/* 2nd try- color's contructor */
	color = f_Color(NULL, args, kwargs);
    }
    if(!color)
	return NULL;

    itag->tag = swf_InsertTag(0, ST_SETBACKGROUNDCOLOR);
    RGBA rgba = color_getRGBA(color);
    swf_SetU8(itag->tag, rgba.r);
    swf_SetU8(itag->tag, rgba.g);
    swf_SetU8(itag->tag, rgba.b);
    mylog(" %08x(%d) SetBackgroundColor(%02x,%02x,%02x) (colorobj=%08x(%d))\n", (int)tag, tag->ob_refcnt, rgba.r, rgba.g, rgba.b, color, color->ob_refcnt);
    Py_DECREF(color);
    return (PyObject*)tag;
}
static tag_internals_t bgcolor_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: setbgcolor_methods,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t protect_tag;
static PyObject* f_Protect(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"password", NULL};
    char*password = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &password))
	return NULL;

    PyObject*tag = tag_new(&protect_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    itag->tag = swf_InsertTag(0, ST_PROTECT);
    if(password) {
	swf_SetPassword(itag->tag, password);
    }
    mylog("+%08x(%d) f_Protect", (int)tag, tag->ob_refcnt);
    return (PyObject*)tag;
}
static tag_internals_t protect_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t showframe_tag;
static PyObject* f_ShowFrame(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"name", NULL};
    char*name= 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &name))
	return NULL;

    PyObject*tag = tag_new(&showframe_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    itag->tag = swf_InsertTag(0, ST_SHOWFRAME);
    mylog("+%08x(%d) f_ShowFrame", (int)tag, tag->ob_refcnt);
    return (PyObject*)tag;
}
static tag_internals_t showframe_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t removeobject_tag;
static PyObject* f_RemoveObject(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"depth", NULL};
    int depth;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &depth))
	return NULL;

    PyObject*tag = tag_new(&removeobject_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    itag->tag = swf_InsertTag(0, ST_REMOVEOBJECT);
    swf_SetU16(itag->tag, depth);
    mylog("+%08x(%d) f_RemoveObject", (int)tag, tag->ob_refcnt);
    return (PyObject*)tag;
}
static tag_internals_t removeobject_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t sprite_tag;
typedef struct _sprite_internal
{
    PyObject* taglist;
} sprite_internal_t;
    
static int sprite_fillTAG(tag_internals_t*self)
{
    mylog("+%08x(?) sprite_fillTAG", (int)self);

    sprite_internal_t*si = (sprite_internal_t*)self->data;

    TAG*sprite = swf_InsertTag(0, ST_DEFINESPRITE);
    swf_SetU16(sprite, 0); //id
    swf_SetU16(sprite, 0); //frames

    TAG*tag = taglist_getTAGs2(si->taglist, self->tagmap, 0);
    if(!tag) {
	/* pass through exception */
	return 0;
    }
    TAG*tag2 = tag;
    while(tag2->next) tag2 = tag2->next;
    swf_InsertTag(tag2, ST_END);

    sprite->next = tag;
    tag->prev = sprite;

    swf_FoldSprite(sprite);
    self->tag = sprite;
    return 1;
}

static PyObject* f_Sprite(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"name", NULL};
    char*name= 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", kwlist, &name))
	return NULL;

    PyObject*tag = tag_new(&sprite_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    sprite_internal_t*si = (sprite_internal_t*)itag->data;
    si->taglist = taglist_new();
    mylog("+%08x(%d) f_DefineSprite", (int)tag, tag->ob_refcnt);
    return (PyObject*)tag;
}
static PyObject* sprite_getattr(tag_internals_t*self,char*a)
{
    sprite_internal_t*si = (sprite_internal_t*)self->data;
    if(!strcmp(a, "tags")) {
	Py_INCREF(si->taglist);
	return si->taglist;
    }
    return 0;
}
static int sprite_setattr(tag_internals_t*self,char*a, PyObject*obj)
{
    sprite_internal_t*si = (sprite_internal_t*)self->data;
    if(self->tag) {
	swf_DeleteTag(0, self->tag);
	self->tag = 0;
    }
    if(!strcmp(a, "tags")) {
	PY_ASSERT_TYPE(obj,&TagListClass);
	Py_DECREF(si->taglist);
	si->taglist = obj;
	Py_INCREF(si->taglist);
	return 0;
    }
    return 1;
}
static tag_internals_t sprite_tag =
{
    parse: 0,
    fillTAG: sprite_fillTAG,
    dealloc: 0,
    getattr: sprite_getattr, 
    setattr: sprite_setattr,
    tagfunctions: 0,
    datasize: sizeof(sprite_internal_t),
};
//----------------------------------------------------------------------------
staticforward tag_internals_t end_tag;
static tag_internals_t end_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: 0,
};
//----------------------------------------------------------------------------
staticforward tag_internals_t text_tag;

typedef struct _text_internal
{
    char*text;
    SWFFONT* swffont;
    RGBA rgba;
    int size;
    SRECT bbox;
} text_internal_t;
staticforward tag_internals_t placeobject_tag;

static int text_fillTAG(tag_internals_t*self)
{
    text_internal_t*ti = (text_internal_t*)self->data;
    self->tag= swf_InsertTag(0, ST_DEFINETEXT2);
    swf_SetU16(self->tag, /*ID*/0);
    ti->bbox = swf_SetDefineText(self->tag, ti->swffont, &ti->rgba, ti->text, ti->size);
    return 1;
}
static PyObject* text_getattr(tag_internals_t*self,char*a)
{
    text_internal_t*si = (text_internal_t*)self->data;
    if(!strcmp(a, "bbox")) {
	return f_BBox2(si->bbox);
    }
    return 0;
}
static PyObject* f_DefineText(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"font", "text", "size", "color", NULL};
    PyObject*tag = 0;
    PyObject*otext;
    char*text = 0;
    int size = 0;
    RGBA rgba = {255,0,0,0};
    PyObject*color = 0;
    PyObject*font = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!Oi|O!", kwlist, &TagClass, &font, &otext, &size, &ColorClass, &color))
	return NULL;
    if(PyUnicode_Check(otext)) {
	text = PyString_AS_STRING(PyUnicode_AsUTF8String(otext));
    } else if(PyString_Check(otext)) {
	text = PyString_AS_STRING(otext);
    }

    if(color)
	rgba = color_getRGBA(color);

    mylog("DefineText: text = %s", text);
    
    tag = tag_new(&text_tag);
    tag_internals_t* itag = tag_getinternals(tag);
    text_internal_t*ti = (text_internal_t*)itag->data;

    ti->swffont = font_getSWFFONT(font);
    int font_id = tagmap_add(itag->tagmap, font); // add dependency on font
    ti->swffont->id = font_id; // for swf_SetDefineTexts
    ti->text = strdup(text);
    ti->rgba = rgba;
    ti->size = size;

    return (PyObject*)tag;
}
static tag_internals_t text_tag =
{
    parse: 0,
    fillTAG: text_fillTAG,
    dealloc: 0,
    getattr: text_getattr, 
    setattr: 0,
    tagfunctions: 0,
    datasize: sizeof(text_internal_t),
};
//----------------------------------------------------------------------------
staticforward tag_internals_t image_tag;

typedef struct _image_internal
{
    RGBA*rgba;
    int size;
    int width;
    int height;
    int bpp;
    char isindexed;
    char islossless;
} image_internal_t;
staticforward tag_internals_t image_tag;

static int image_fillTAG(tag_internals_t*self)
{
    image_internal_t*ti = (image_internal_t*)self->data;
    self->tag= swf_InsertTag(0, ST_DEFINEBITSLOSSLESS2);
    swf_SetU16(self->tag, /*ID*/0);
    swf_SetLosslessBits(self->tag, ti->width, ti->height, ti->rgba, BMF_32BIT);
    return 1;
}
static void image_dealloc(tag_internals_t*self)
{
    image_internal_t*pi = (image_internal_t*)self->data;
    if(pi->rgba) {
	free(pi->rgba);pi->rgba = 0;
    }
}
static int image_parse(tag_internals_t*self)
{
    image_internal_t*i= (image_internal_t*)self->data;
    if(i->rgba)
	return 1;
    if(!self->tag)
	return 0;
   
    i->rgba = swf_ExtractImage(self->tag, &i->width, &i->height);
    i->bpp = 32;
    i->isindexed = 0;
    i->islossless = 1;

    swf_DeleteTag(0, self->tag);self->tag = 0;
    return 1;
}
static int imagetag_getWidth(PyObject* self)
{
    tag_internals_t*itag = tag_getinternals(self);
    image_internal_t*pi = (image_internal_t*)itag->data;
    return pi->width;
}
static int imagetag_getHeight(PyObject* self)
{
    tag_internals_t*itag = tag_getinternals(self);
    image_internal_t*pi = (image_internal_t*)itag->data;
    return pi->height;
}
static PyObject* f_DefineImage(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"image", NULL};
    PyObject*image = 0;
    PyObject*tag = tag_new(&image_tag);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", kwlist, &image))
	return NULL;
    
    tag = tag_new(&image_tag);
    tag_internals_t* itag = tag_getinternals(tag);
    image_internal_t*ti = (image_internal_t*)itag->data;

    ti->rgba = image_toRGBA(image);
    if(!ti->rgba) // pass through exception
	return 0;
    ti->width = image_getWidth(image);
    ti->height = image_getHeight(image);
    ti->isindexed = 0;
    ti->islossless = 1;
    ti->bpp = 32;
    ti->size = ti->width*ti->height;

    return (PyObject*)tag;
}
static PyObject* image_getattr(tag_internals_t*self,char*a)
{
    image_internal_t*i = (image_internal_t*)self->data;
    if(!strcmp(a, "image")) {
	if(!i->rgba) {
	    image_parse(self);
	}
	PyObject* image = rgba_to_image(i->rgba, i->width, i->height);
	return image;
    }
    return 0;
}

static PyObject* image_save(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    if(!image_parse(itag))
	return PY_ERROR("Couldn't parse image");
    image_internal_t*fi = (image_internal_t*)itag->data;
   
    char*filename = 0;
    if(!PyArg_ParseTuple(args, "s", &filename))
	return NULL;

    png_write(filename, (unsigned char*)fi->rgba ,fi->width, fi->height);
    
    return PY_NONE;
}

static PyMethodDef image_methods[] = 
{{"save", image_save, METH_VARARGS, "saves an image as PNG"},
 {NULL, NULL, 0, NULL}
};

static tag_internals_t image_tag =
{
    parse: image_parse,
    fillTAG: image_fillTAG,
    dealloc: image_dealloc,
    getattr: image_getattr, 
    setattr: 0,
    tagfunctions: image_methods,
    datasize: sizeof(image_internal_t),
};
//----------------------------------------------------------------------------
staticforward tag_internals_t shape_tag;

typedef struct _shape_internal
{
    SHAPE2*shape2;
} shape_internal_t;
staticforward tag_internals_t shape_tag;

static int shape_fillTAG(tag_internals_t*self)
{
    shape_internal_t*ti = (shape_internal_t*)self->data;
    self->tag= swf_InsertTag(0, ST_DEFINESHAPE3);
    swf_SetU16(self->tag, /*ID*/0);
    swf_SetShape2(self->tag, ti->shape2);
    return 1;
}
static int shape_parse(tag_internals_t*self)
{
    shape_internal_t*i= (shape_internal_t*)self->data;
    if(i->shape2)
	return 1;
    if(!self->tag)
	return 0;
    SHAPE2* shape2 = malloc(sizeof(SHAPE2));
    swf_ParseDefineShape(self->tag, shape2);
    i->shape2 = shape2;
    swf_DeleteTag(0, self->tag);self->tag = 0;
    return 1;
}
static void shape_dealloc(tag_internals_t*self)
{
    shape_internal_t*pi = (shape_internal_t*)self->data;
    if(pi->shape2) {
	swf_Shape2Free(pi->shape2);
	pi->shape2 = 0;
    }
}
static PyObject* f_DefineImageShape(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"image", NULL};
    PyObject*image = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", kwlist, &TagClass, &image))
	return NULL;
    
    PyObject*tag = tag_new(&shape_tag);
    tag_internals_t* itag = tag_getinternals(tag);
    shape_internal_t*ti = (shape_internal_t*)itag->data;
    ti->shape2 = 0; /*HACK*/

    int width = imagetag_getWidth(image);
    int height = imagetag_getHeight(image);
    int id = tagmap_add(itag->tagmap, image);
    itag->tag= swf_InsertTag(0, ST_DEFINESHAPE3);
    swf_SetU16(itag->tag, 0);
    swf_ShapeSetBitmapRect(itag->tag, id, width, height);
    return (PyObject*)tag;
}

/* TODO: move to lib/ */
SHAPE2*swf_StringToShape2(char*s,FILLSTYLE*f, LINESTYLE*l)
{
    drawer_t draw;
    swf_Shape11DrawerInit(&draw, 0);
    draw_string(&draw, s);
    draw.finish(&draw);
    SHAPE*s1 =  swf_ShapeDrawerToShape(&draw);
    SRECT r = swf_ShapeDrawerGetBBox(&draw);
    RGBA col;col.r=col.g=col.b=128;col.a=255;
    if(l) 
	swf_ShapeAddLineStyle(s1, 1, &col);
    if(f)
	swf_ShapeAddSolidFillStyle(s1, &col);
    draw.dealloc(&draw);
    SHAPE2*shape2 = swf_ShapeToShape2(s1);
    swf_ShapeFree(s1);
    shape2->bbox = malloc(sizeof(SRECT));
    *(shape2->bbox) = r;
    if(f && shape2->numfillstyles)
	shape2->fillstyles[0] = *f;
    if(l && shape2->numlinestyles)
	shape2->linestyles[0] = *l;
    return shape2;
}

static PyObject* f_DefineShape(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"s", "fill", "line", NULL};
    char*s = 0;
    PyObject*fillstyle=0,*linestyle=0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|OO", kwlist, &s,&fillstyle,&linestyle))
	return NULL;

    PyObject*tag = tag_new(&shape_tag);
    tag_internals_t* itag = tag_getinternals(tag);
    shape_internal_t*ti = (shape_internal_t*)itag->data;
    
    FILLSTYLE _f,*f=0;
    LINESTYLE _l,*l=0;

    if(fillstyle) {
	f = &_f;
	if(PY_CHECK_TYPE(fillstyle, &ColorClass)) {
	    f->type = /*FILL_SOLID*/ 0;
	    f->color = color_getRGBA(fillstyle);
	} else {
	    return PY_ERROR("Invalid Fillstyle");
	}
    }

    if(linestyle) {
	l = &_l;
	if(PyTuple_Check(linestyle) && PyTuple_GET_SIZE(linestyle)==2) {
	    float f = 0.0;
	    PyObject*color = 0;
	    if(!PyArg_ParseTuple(linestyle, "fO!", &f, &ColorClass, &color))
		return 0;

	    l->width = (int)(f*20);
	    l->color = color_getRGBA(color);
	} else {
	    return PY_ERROR("Invalid Linestyle");
	}
    }
    ti->shape2 = swf_StringToShape2(s,f,l);

    itag->tag = 0;
    
    return (PyObject*)tag;
}
static PyObject* shape_getfillstyles(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    if(!shape_parse(itag))
	return PY_ERROR("Couldn't parse shape");
    shape_internal_t*fi = (shape_internal_t*)itag->data;
    int num = fi->shape2->numfillstyles;
    return Py_BuildValue("i", num);
}
static PyObject* shape_getlinestyles(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    if(!shape_parse(itag))
	return PY_ERROR("Couldn't parse shape");
    shape_internal_t*fi = (shape_internal_t*)itag->data;
    int num = fi->shape2->numlinestyles;
    return Py_BuildValue("i", num);
}
static PyObject* shape_getfillstyle(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    if(!shape_parse(itag))
	return PY_ERROR("Couldn't parse shape");
    shape_internal_t*fi = (shape_internal_t*)itag->data;
    int nr = 0;
    if(!PyArg_ParseTuple(args, "i", &nr))
	return NULL;
    
    int num = fi->shape2->numfillstyles;
    if(nr < 0 || nr >=num)
	return PY_ERROR("fillstyle index out of range");
    return f_FillStyle2(fi->shape2->fillstyles[nr]);
}
static PyObject* shape_getlinestyle(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    if(!shape_parse(itag))
	return PY_ERROR("Couldn't parse shape");
    shape_internal_t*fi = (shape_internal_t*)itag->data;
    int nr = 0;
    if(!PyArg_ParseTuple(args, "i", &nr))
	return NULL;
    
    int num = fi->shape2->numfillstyles;
    if(nr < 0 || nr >=num)
	return PY_ERROR("fillstyle index out of range");
    return f_LineStyle3(fi->shape2->linestyles[nr]);
}
static PyObject* shape_setfillstyle(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    if(!shape_parse(itag))
	return PY_ERROR("Couldn't parse shape");
    shape_internal_t*fi = (shape_internal_t*)itag->data;
    int nr = 0;
    PyObject*fs = 0;
    if(!PyArg_ParseTuple(args, "iO!", &nr, &FillStyleClass, &fs))
	return NULL;
    
    int num = fi->shape2->numfillstyles;
    if(nr < 0 || nr >=num)
	return PY_ERROR("fillstyle index out of range");
    fi->shape2->fillstyles[nr] = fillstyle_getFillStyle(fs);
    return PY_NONE;
}
static PyObject* shape_setlinestyle(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    if(!shape_parse(itag))
	return PY_ERROR("Couldn't parse shape");
    shape_internal_t*fi = (shape_internal_t*)itag->data;
    int nr = 0;
    PyObject*ls = 0;
    if(!PyArg_ParseTuple(args, "iO!", &nr, &LineStyleClass, &ls))
	return NULL;
    
    int num = fi->shape2->numlinestyles;
    if(nr < 0 || nr >=num)
	return PY_ERROR("linestyle index out of range");
    fi->shape2->linestyles[nr] = linestyle_getLineStyle(ls);
    return PY_NONE;
}
static PyMethodDef shape_methods[] = 
{{"numfillstyles", shape_getfillstyles, METH_VARARGS, "get's the number of fillstyles"},
 {"numlinestyles", shape_getlinestyles, METH_VARARGS, "get's the number of linestyles"},
 {"getfillstyle", shape_getfillstyle, METH_VARARGS, "get's one fillstyle"},
 {"getlinestyle", shape_getlinestyle, METH_VARARGS, "get's one linestyle"},
 {"setfillstyle", shape_setfillstyle, METH_VARARGS, "set's one fillstyle"},
 {"setlinestyle", shape_setlinestyle, METH_VARARGS, "set's one linestyle"},
 {NULL, NULL, 0, NULL}
};

static tag_internals_t shape_tag =
{
    parse: shape_parse,
    fillTAG: shape_fillTAG,
    dealloc: shape_dealloc,
    getattr: 0, 
    setattr: 0,
    tagfunctions: shape_methods,
    datasize: sizeof(shape_internal_t),
};
//----------------------------------------------------------------------------

typedef struct _videostream_internal
{
    VIDEOSTREAM* stream;
    int lastiframe;
} videostream_internal_t;
staticforward tag_internals_t videostream_tag;
staticforward tag_internals_t videoframe_tag;

static int videostream_parse(tag_internals_t*self)
{
    videostream_internal_t*videostream = (videostream_internal_t*)self->data;
    /* TODO */
    PyErr_SetString(PyExc_Exception, setError("videostream parsing not implemented yet"));
    return 0;
}
static void videostream_dealloc(tag_internals_t*self)
{
    videostream_internal_t*videostream = (videostream_internal_t*)self->data;
    if(videostream->stream) {
	swf_VideoStreamClear(videostream->stream);
	free(videostream->stream);
	videostream->stream = 0;
    }
}
static int videostream_fillTAG(tag_internals_t*self)
{
    videostream_internal_t*fi = (videostream_internal_t*)self->data;
    if(self->tag)
	return 1;
    PyErr_SetString(PyExc_Exception, setError("videostream filling not implemented"));
    return 0;
}
static PyObject* f_DefineVideoStream(PyObject* self, PyObject* args, PyObject* kwargs)
{
    PyObject*tag = tag_new(&videostream_tag);
   
    int width=0,height=0,frames=65535;
    static char *kwlist[] = {"width", "height", "frames", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|i", kwlist, &width, &height, &frames))
	return NULL;

    tag_internals_t*itag = tag_getinternals(tag);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    fi->stream = malloc(sizeof(VIDEOSTREAM));
    memset(fi->stream, 0, sizeof(VIDEOSTREAM));

    TAG*t = swf_InsertTag(0, ST_DEFINEVIDEOSTREAM);
    swf_SetU16(t, 0); /* id */
    swf_SetVideoStreamDefine(t, fi->stream, frames, width, height);
    itag->tag = t;
    fi->lastiframe = -65536;
    return (PyObject*)tag;
}
static VIDEOSTREAM* videostream_getVIDEOSTREAM(PyObject*self)
{
    PY_ASSERT_TYPE(self, &TagClass);
    tag_internals_t*itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    return fi->stream;
}
static PyObject* videostream_getbwidth(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    int width = fi->stream->bbx;
    return Py_BuildValue("i", width);
}
static PyObject* videostream_getbheight(PyObject*self, PyObject*args)
{
    tag_internals_t*itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)itag->data;
    int height = fi->stream->bby;
    return Py_BuildValue("i", height);
}
static PyObject* videostream_addBlackFrame(PyObject*self, PyObject*args, PyObject*kwargs)
{
    tag_internals_t*_itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)_itag->data;
    
    TAG* t = swf_InsertTag(0, ST_VIDEOFRAME);
    
    PyObject*tag = tag_new(&videoframe_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    
    swf_SetU16(t,0); /* id */
    swf_SetVideoStreamBlackFrame(t, fi->stream);
    fi->lastiframe = fi->stream->frame;
    
    itag->tag = t;
    tagmap_addMapping(itag->tagmap, 0, self);
    return tag;
}
static PyObject* videostream_addFrame(PyObject*self, PyObject*args, PyObject*kwargs)
{
    tag_internals_t*_itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)_itag->data;
   
    PyObject*image = 0;
    char*type=0; // none, "i", "p"
    int quant=7;
    static char *kwlist[] = {"image", "quant", "type", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|is", kwlist, &image, &quant, &type))
	return NULL;
    if(fi->stream->owidth != image_getWidth(image)) {
	PyErr_SetString(PyExc_Exception, setError("bad image width %d!=%d", image_getWidth(image), fi->stream->width));return 0;
    }
    if(fi->stream->oheight != image_getHeight(image)) {
	PyErr_SetString(PyExc_Exception, setError("bad image width %d!=%d", image_getHeight(image), fi->stream->height));return 0;
    }
    PyObject*tag = tag_new(&videoframe_tag);
    tag_internals_t*itag = tag_getinternals(tag);

    RGBA*pic = image_toRGBA(image);
    if(!pic)
	return 0;

/*{  int f,j=0,i=0,rr,gg,bb;
   FILE *o;
   RGBA*it = pic;
   char*filename="test.ppm";
   printf("Creating %s %dx%d\n",filename, 512,512);
   o=fopen(filename, "wb");
   fprintf(o,"P6\n%d %d\n255\n",512, 512);
   
   while(j<512*512) {
    rr=it->r;
    gg=it->g;
    bb=it->b;
    fprintf(o,"%c%c%c",rr,gg,bb);
    it++;
    j++;
   }
   fclose(o);
}*/

    TAG* t = swf_InsertTag(0, ST_VIDEOFRAME);
    if((type && (type[0]=='I' || type[0]=='i')) || (type==0 && fi->lastiframe+64 < fi->stream->frame)) {
	swf_SetU16(t,0); /* id */
	swf_SetVideoStreamIFrame(t, fi->stream, pic, quant);
	fi->lastiframe = fi->stream->frame;
    } else {
	swf_SetU16(t,0);
	swf_SetVideoStreamPFrame(t, fi->stream, pic, quant);
    }
    itag->tag = t;
    tagmap_addMapping(itag->tagmap, 0, self);
    free(pic);
    return tag;
}
static PyObject* videostream_addDistortionFrame(PyObject*self, PyObject*args, PyObject*kwargs)
{
    tag_internals_t*_itag = tag_getinternals(self);
    videostream_internal_t*fi = (videostream_internal_t*)_itag->data;
    
    static char *kwlist[] = {"image", "quant", NULL};
    int quant=7;
    PyObject* array = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|i", kwlist, &array, &quant))
	return NULL;

    signed char* movex = malloc(fi->stream->bbx * fi->stream->bby * 1);
    signed char* movey = malloc(fi->stream->bbx * fi->stream->bby * 1);
    RGBA** pics = (RGBA**)malloc(fi->stream->bbx * fi->stream->bby * sizeof(void*));
    signed char* itx=movex;
    signed char* ity=movey;
    RGBA**pic=pics;
    int x,y;
    if(!array || !PySequence_Check(array))
	return PY_ERROR("Not an array");
    if(PySequence_Length(array) < fi->stream->bby)
	return PY_ERROR("Array (y) has to have at least %d elements, but has only %d ", fi->stream->bby, PySequence_Length(array));
    for(y=0;y<fi->stream->bby;y++) {
	PyObject*line = PySequence_GetItem(array, y);
	if(!line || !PySequence_Check(line))
	    return PY_ERROR("Not an array of arrays");
	if(PySequence_Length(line) < fi->stream->bbx)
	    return PY_ERROR("Inner arrays (x) have to be at least %d long- %dth is only %d", fi->stream->bbx, y, PySequence_Length(line));

	for(x=0;x<fi->stream->bbx;x++) {
	    PyObject*pixel = PySequence_GetItem(line, x);
	    PyObject*xy = 0;
	    PyObject*image = 0;

	    if(!pixel) {
		xy = image = 0;
	    } else {
		if(PyComplex_Check(pixel)) {
		    xy = pixel; image = 0;
		} else if(PyString_Check(pixel)) {
		    xy = 0; image = pixel;
		} else if(PyTuple_Check(pixel)) {
		    int size = PyTuple_GET_SIZE(pixel);
		    if(size!=2) return PY_ERROR("Tuples have to have size 2 (xy,img)");
		    xy = PyTuple_GetItem(pixel, 0);
		    if(!xy) return 0;
		    if(!PyComplex_Check(xy)) return PY_ERROR("Tuples must be (COMPLEX,string)");
		    image = PyTuple_GetItem(pixel, 1);
		    if(!image) return 0;
		    if(!PyString_Check(image)) return PY_ERROR("Tuples must be (complex,STRING)");
		}
	    }

	    *itx = *ity = 0;
	    *pic= 0;

	    if(xy) {
		*itx = (signed char)PyComplex_RealAsDouble(pixel);
		*ity = (signed char)PyComplex_ImagAsDouble(pixel);
	    }
	    if(image) {
		char*string;
		int size;
		PyString_AsStringAndSize(image,&string,&size);
		if(size<256*3) {
		    return PY_ERROR("image strings must be >= 256*3");
		}
		*pic = malloc(sizeof(RGBA)*16*16);
		int t;
		for(t=0;t<16*16;t++) {
		    (*pic)[t].r = string[t*3];
		    (*pic)[t].g = string[t*3+1];
		    (*pic)[t].b = string[t*3+2];
		    (*pic)[t].a = 255;
		}
	    }
	    itx++;
	    ity++;
	    pic++;
	}
    }
    
    PyObject*tag = tag_new(&videoframe_tag);
    tag_internals_t*itag = tag_getinternals(tag);
    
    TAG* t = swf_InsertTag(0, ST_VIDEOFRAME);
    swf_SetU16(t,0); /* id */
    swf_SetVideoStreamMover(t, fi->stream, movex, movey,(void**)pics, quant);

    itag->tag = t;
    tagmap_addMapping(itag->tagmap, 0, self);

    for(x=0;x<fi->stream->bbx*fi->stream->bby;x++) {
	if(pics[x]) {
	    free(pics[x]);pics[x] = 0;
	}
    }

    free(movex);
    free(movey);
    free(pics);

    return tag;
}
static PyMethodDef videostream_methods[] = 
{{"xblocks", videostream_getbwidth, METH_VARARGS, "get's the number of horizontal blocks"},
 {"yblocks", videostream_getbheight, METH_VARARGS, "get's the number of vertical blocks"},
 {"addFrame", (PyCFunction)videostream_addFrame, METH_KEYWORDS, "add a Video Frame"},
 {"addBlackFrame", (PyCFunction)videostream_addBlackFrame, METH_KEYWORDS, "add a black Video Frame"},
 {"addDistortionFrame", (PyCFunction)videostream_addDistortionFrame, METH_KEYWORDS, "add a MVD frame"},
 {NULL, NULL, 0, NULL}
};

static tag_internals_t videostream_tag =
{
    parse: videostream_parse,
    fillTAG: videostream_fillTAG,
    dealloc: videostream_dealloc,
    getattr: 0, 
    setattr: 0,
    tagfunctions: videostream_methods,
    datasize: sizeof(videostream_internal_t),
};

//============================================================================

static tag_internals_t videoframe_tag =
{
    parse: 0,
    fillTAG: 0,
    dealloc: 0,
    getattr: 0, 
    setattr: 0,
    tagfunctions: 0,
    datasize: 0
};

//============================================================================
static PyMethodDef TagMethods[] = 
{
    /* TAGS */
    {"BackgroundColor", (PyCFunction)f_SetBackgroundColor, METH_KEYWORDS, "Create a SetBackGroundColor Tag."},
    {"Protect", (PyCFunction)f_Protect, METH_KEYWORDS, "Create a Protect Tag."},
    {"Font", (PyCFunction)f_DefineFont, METH_KEYWORDS, "Create a DefineFont Tag."},
    {"Text", (PyCFunction)f_DefineText, METH_KEYWORDS, "Create a DefineText Tag."},
    {"PlaceObject", (PyCFunction)f_PlaceObject, METH_KEYWORDS, "Create a PlaceObject Tag."},
    {"RemoveObject", (PyCFunction)f_RemoveObject, METH_KEYWORDS, "Create a RemoveObject Tag."},
    {"MoveObject", (PyCFunction)f_MoveObject, METH_KEYWORDS, "Create a PlaceObject Move Tag."},
    {"VideoStream", (PyCFunction)f_DefineVideoStream, METH_KEYWORDS, "Create a Videostream."},
    {"Image", (PyCFunction)f_DefineImage, METH_KEYWORDS, "Create an SWF Image Tag."},
    {"ImageShape", (PyCFunction)f_DefineImageShape, METH_KEYWORDS, "Create an SWF Image Shape Tag."},
    {"Shape", (PyCFunction)f_DefineShape, METH_KEYWORDS, "Create an SWF Shape Tag."},
    {"ShowFrame", (PyCFunction)f_ShowFrame, METH_KEYWORDS, "Create an SWF Show Frame Tag."},
    {"Sprite", (PyCFunction)f_Sprite, METH_KEYWORDS, "Create an SWF Sprite Tag."},
    
    {NULL, NULL, 0, NULL}
};
PyMethodDef* tags_getMethods()
{
    TagClass.ob_type = &PyType_Type;
    
    register_tag(ST_PLACEOBJECT,&placeobject_tag);
    register_tag(ST_PLACEOBJECT2,&placeobject_tag);
    register_tag(ST_REMOVEOBJECT,&removeobject_tag);
    register_tag(ST_REMOVEOBJECT2,&removeobject_tag);
    register_tag(ST_SETBACKGROUNDCOLOR,&bgcolor_tag);
    register_tag(ST_DEFINEFONT,&font_tag);
    register_tag(ST_PROTECT,&protect_tag);
    register_tag(ST_DEFINETEXT,&text_tag);
    register_tag(ST_DEFINEBITSJPEG,&image_tag);
    register_tag(ST_DEFINEBITSJPEG2,&image_tag);
    register_tag(ST_DEFINEBITSJPEG3,&image_tag);
    register_tag(ST_DEFINEBITSLOSSLESS,&image_tag);
    register_tag(ST_DEFINEBITSLOSSLESS2,&image_tag);
    register_tag(ST_DEFINESHAPE,&shape_tag);
    register_tag(ST_DEFINESHAPE2,&shape_tag);
    register_tag(ST_DEFINESHAPE3,&shape_tag);
    register_tag(ST_SHOWFRAME,&showframe_tag);
    register_tag(ST_DEFINEVIDEOSTREAM,&videostream_tag);
    register_tag(ST_VIDEOFRAME,&videoframe_tag);
    register_tag(ST_DEFINESPRITE,&sprite_tag);
    register_tag(ST_END,&end_tag);

    return TagMethods;
}
