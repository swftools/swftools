#include <Python.h>

int main(int argn, char*argv[])
{
    void* dummy = malloc(4096);
    return Py_Main(argn, argv);
}
