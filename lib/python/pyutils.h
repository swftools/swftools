#ifndef __pyutil_h__
#define __pyutil_h__
#include <Python.h>

#define PY_NONE Py_BuildValue("s", 0)

extern int verbose;

char* setError(char*format, ...);
void mylog(char*format, ...);
PyObject* FindMethodMore(PyObject*ret, PyMethodDef f[], PyObject*self, char* a);
void dummy_dealloc(PyObject* self);
#endif
