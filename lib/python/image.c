#include <Python.h>
#undef HAVE_STAT
#include "../rfxswf.h"

int image_getWidth(PyObject*image)
{
    return 0;
}

int image_getHeight(PyObject*image)
{
    return 0;
}

int image_getBPP(PyObject*image)
{
    return 0;
}

RGBA* image_toRGBA(PyObject*image)
{
    return 0;
}
