/*
    Ming, an SWF output library
    Copyright (C) 2002  Opaque Industries - http://www.opaque.net/

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "libming.h"
#include "compile.h"
#include "action.h"

#include "actioncompiler.h"

static void print_error(const char*format,...)
{
    char buf[1024];
    int len;
    va_list arglist;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);

    len = strlen(buf);
    while(len>0 && buf[len-1]=='\n') len--;
    buf[len] = '\n';
    buf[len+1] = 0;
    fprintf(stderr, "error: %s", buf);
}

static void print_warn(const char*format,...)
{
    char buf[1024];
    int len;
    va_list arglist;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);

    len = strlen(buf);
    while(len>0 && buf[len-1]=='\n') len--;
    buf[len] = '\n';
    buf[len+1] = 0;
    printf("%s", buf);
    fprintf(stderr, "warning: %s", buf);
}

int compileSWFActionCode(const char *script, int version, void**data, int*len)
{
	Buffer b;

	*data = 0;
	*len = 0;

	SWF_versionNum = version;
	if(!SWF_error) 
	    SWF_error = print_error;
	if(!SWF_warn) 
	    SWF_warn = print_warn;
	
	/* yydebug = 1; */

	if(version == 4)
	{
		swf4ParseInit(script, 0);

		if(swf4parse((void *)&b) != 0)
			return 0;
	}
	else
	{
		swf5ParseInit(script, 0);

		if(swf5parse((void *)&b) != 0)
			return 0;
	}
	    
	*data = b->buffer;
	*len = bufferLength(b);

	//destroyBuffer(b);
	free(b);
	
	return 1;
}

