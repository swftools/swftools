#include <Python.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* setError(char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    return strdup(buf);
}

static int verbose = 1;
void mylog(char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(!verbose)
	return;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    fprintf(stderr, "[SWF] %s\n", buf);
    fflush(stderr);
}

#define PY_NONE Py_BuildValue("s", 0)

PyObject* FindMethodMore(PyObject*ret, PyMethodDef f[], PyObject*self, char* a)
{
    if(ret==NULL) {
	ret = Py_FindMethod(f, self, a);
    } else {
	if(!strcmp(a, "__methods__")) {
	    /* we are being dir()ed. Complete the function table */
	    PyObject* add = Py_FindMethod(f, self, a);
	    int t;
	    mylog("taglist_getattr: append common funtions %08x %08x\n", ret, add);
	    for(t=0;t<PyList_Size(add);t++)
		PyList_Append(ret, PyList_GetItem(add, t));
	}
    }
    return ret;
}

void dummy_dealloc(PyObject* self)
{
    PyObject_Del(self);
}

