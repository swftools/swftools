/* common.c

   Routines for handling Flash2 AVM2 ABC Actionscript

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2008,2009 Matthias Kramm <kramm@quiss.org>
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

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
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    fprintf(stderr, "%s:%d:%d: error: %s\n", current_filename, current_line, current_column, buf);
    fprintf(stderr, "%s\n", current_filename_long);
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
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
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
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    fprintf(stderr, "%s:%d:%d: warning: %s\n", current_filename, current_line, current_column, buf);
    fflush(stderr);
}

void internal_error(const char*file, int line, const char*f)
{
    syntaxerror("internal error in %s, %s:%d", f, file, line);
}

int a3_error(char*s)
{
   syntaxerror("%s", s); 
   return 0; //make gcc happy
}

