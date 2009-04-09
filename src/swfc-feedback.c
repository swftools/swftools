/* swfc- Compiles swf code (.sc) files into .swf files.

   Part of the swftools package.

   Copyright (c) 2007 Huub Schaeks <huub@h-schaeks.speedlinq.nl>
   Copyright (c) 2007 Matthias Kramm <kramm@quiss.org>
 
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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "swfc-feedback.h"

void syntaxerror(char*format, ...)
{
    char buf[1024];
    va_list arglist;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    fprintf(stderr, "\"%s\", line %d column %d: error- %s\n", filename, line, column, buf);
    if (cleanUp)
	    cleanUp();
    exit(1);
}

void warning(char*format, ...)
{
    char buf[1024];
    va_list arglist;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    fprintf(stderr, "\"%s\", line %d column %d: warning- %s\n", filename, line, column, buf);
}
