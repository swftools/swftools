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
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    return strdup(buf);
}

static int verbose = 0;
void mylog(char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(!verbose)
	return;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
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

PyMethodDef* addMethods(PyMethodDef*obj1, PyMethodDef*obj2) 
{
    int num1=0,num2=0;
    if(obj1) for(num1=0;obj1[num1].ml_name;num1++);
    if(obj2) for(num2=0;obj2[num2].ml_name;num2++);
    PyMethodDef* result = malloc(sizeof(PyMethodDef)*(num1+num2+1));
    if(obj1)
	memcpy(result, obj1, num1*sizeof(PyMethodDef));
    if(obj2)
	memcpy(&result[num1], obj2, (num2+1)*sizeof(PyMethodDef));
    if(obj1)
	free(obj1);
    return result;
}
void setVerbosity(int _verbose)
{
    verbose = _verbose;
    mylog("setting verbosity to %d", verbose);
}
