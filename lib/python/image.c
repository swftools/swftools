#include <Python.h>
//#include "/usr/include/python2.3/Imaging.h"
#include <Imaging.h>
#include "pyutils.h"
#undef HAVE_STAT
#include "../rfxswf.h"

/* redefine the ImagingObject struct defined in _imagingmodule.c */
/* there should be a better way to do this... */
typedef struct {
    PyObject_HEAD
    Imaging image;
} ImagingObject;

int image_getWidth(PyObject*_image) {
    if(strcmp(_image->ob_type->tp_name, "ImagingCore")) {
	PyErr_SetString(PyExc_Exception, setError("not an image: %s", _image->ob_type->tp_name));
	return 0;
    }
    ImagingObject*image = (ImagingObject*)_image;
    return image->image->xsize;
}

int image_getHeight(PyObject*_image) {
    if(strcmp(_image->ob_type->tp_name, "ImagingCore")) {
	PyErr_SetString(PyExc_Exception, setError("not an image: %s", _image->ob_type->tp_name));
	return 0;
    }
    ImagingObject*image = (ImagingObject*)_image;
    return image->image->ysize;
}

int image_getBPP(PyObject*_image) {
    if(strcmp(_image->ob_type->tp_name, "ImagingCore")) {
	PyErr_SetString(PyExc_Exception, setError("not an image: %s", _image->ob_type->tp_name));
	return 0;
    }
    ImagingObject*image = (ImagingObject*)_image;
    if(!strcmp(image->image->mode, "1") ||
       !strcmp(image->image->mode, "L") ||
       !strcmp(image->image->mode, "P")) {
	return 8;
    }
    if(!strcmp(image->image->mode, "I") ||
       !strcmp(image->image->mode, "F")) {
	return 32;
    }
    if(!strcmp(image->image->mode, "RGB") ||
       !strcmp(image->image->mode, "RGBA") ||
       !strcmp(image->image->mode, "CMYK") ||
       !strcmp(image->image->mode, "YCbCr")) {
	return 32;
    }
    PyErr_SetString(PyExc_Exception, setError("Unknown image format (%s).", image->image->mode));
    return 0;
}

RGBA* image_toRGBA(PyObject*_image) 
{
    if(strcmp(_image->ob_type->tp_name, "ImagingCore")) {
	PyErr_SetString(PyExc_Exception, setError("not an image: %s", _image->ob_type->tp_name));
	return 0;
    }
    ImagingObject*image = (ImagingObject*)_image;
    printf("mode: %s\n", image->image->mode);
    printf("depth: %d\n", image->image->depth);
    printf("bands: %d\n", image->image->bands);
    printf("xsize: %d\n", image->image->xsize);
    printf("ysize: %d\n", image->image->ysize);
    int bpp = image_getBPP(_image);
    if(!bpp)
	return 0;
	
    RGBA*rgba = (RGBA*)malloc(image->image->xsize * image->image->ysize * sizeof(RGBA));

    if(!strcmp(image->image->mode, "RGBA")) {
	int y,ymax=image->image->ysize;
	int width = image->image->xsize;
	RGBA*dest = rgba;
	for(y=0;y<ymax;y++) {
	    U8* src = (U8*)(image->image->image32[y]);
	    int x;
	    for(x=0;x<width;x++) {
		dest[x].r = src[x*4+0];
		dest[x].g = src[x*4+1];
		dest[x].b = src[x*4+2];
		dest[x].a = src[x*4+3];
	    }
	    dest+=width;
	}
	return rgba;	
    }
	
    PyErr_SetString(PyExc_Exception, setError("Unsupported image format: %s (try .convert(\"RGBA\")", image->image->mode));
    return 0;
}

extern PyObject*PyImagingNew(Imaging imOut);

PyObject* rgba_to_image(RGBA*rgba, int width, int height)
{
#ifndef WIN32
    Imaging img = ImagingNew("RGBA", width, height);
    int y;
    if(!img->image32) {
	fprintf(stderr, "No array allocated!\n");
	return 0;
    }
    for(y=0;y<height;y++) {
	U8* dest = (U8*)(img->image32[y]);
	RGBA* src = &rgba[width*y];
	int x;
	for(x=0;x<width;x++) {
	    dest[x+0] = src[x].r;
	    dest[x+1] = src[x].g;
	    dest[x+2] = src[x].b;
	    dest[x+3] = src[x].a;
	}
    }

    return PyImagingNew(img);
#else
    fprintf(stderr, "This image extraction is not yet supported on non-linux systems\n");
    return 0;
#endif
}
