#ifndef __pyutil_h__
#define __pyutil_h__
#include <Python.h>

#define PY_NONE Py_BuildValue("s", 0)

extern int verbose;

void setVerbosity(int verbose);

char* setError(char*format, ...);
void mylog(char*format, ...);
PyObject* FindMethodMore(PyObject*ret, PyMethodDef f[], PyObject*self, char* a);
void dummy_dealloc(PyObject* self);
PyMethodDef* addMethods(PyMethodDef*obj1, PyMethodDef*obj2);

#define PY_CHECK_TYPE(o,c) ((o)->ob_type == (c))
#define PY_ASSERT_TYPE(o,c) {if((o)->ob_type != (c)) {printf("Invalid type: %08x(%d)", (int)(o), (o)->ob_refcnt);exit(1);}}
	
#define PY_ERROR(s,args...) (PyErr_SetString(PyExc_Exception, setError(s, ## args)),NULL)
#define PY_TYPE(o) ((o)->ob_type->tp_name)
#endif
