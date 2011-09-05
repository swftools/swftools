/* os.h

header file for operating system dependent functions

Part of the swftools package. 

Copyright (c) 2005 Matthias Kramm <kramm@quiss.org>

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


#ifndef __os_h__
#define __os_h__
#include "../config.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char path_seperator;

#ifdef WIN32
char* getRegistryEntry(char*path);
int setRegistryEntry(char*key,char*value);
#endif

typedef struct _memfile {
    void*data;
    int len;
} memfile_t;
memfile_t* memfile_open(const char*path);
void memfile_close(memfile_t*file);

char* getInstallationPath();
char* concatPaths(const char*base, const char*add);
char* stripFilename(const char*filename, const char*newext);

char* mktempname(char*buffer, const char*ext);

void move_file(const char*from, const char*to);
char file_exists(const char*filename);
int file_size(const char*filename);

#ifdef __cplusplus
}
#endif

#endif
