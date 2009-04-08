#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "files.h"
#include "common.h"

int as3_pass = 0;
int as3_verbosity = 1;

void as3_error(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(as3_verbosity<0)
        exit(1);
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    fprintf(stderr, "%s:%d:%d: error: %s\n", current_filename, current_line, current_column, buf);
    fflush(stderr);
    exit(1);
}
void as3_warning(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(as3_verbosity<1)
        return;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    fprintf(stdout, "%s:%d:%d: warning: %s\n", current_filename, current_line, current_column, buf);
    fflush(stdout);
}
void as3_softwarning(const char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(as3_verbosity<2)
	return;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);
    fprintf(stderr, "%s:%d:%d: warning: %s\n", current_filename, current_line, current_column, buf);
    fflush(stderr);
}
