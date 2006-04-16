/* os.c

operating system dependent functions

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

#include "os.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif

#if defined(CYGWIN)
static char seperator = '/';
#elif defined(WIN32)
static char seperator = '\\';
#else
static char seperator = '/';
#endif

#ifdef WIN32
char* getRegistryEntry(char*path)
{
    int res = 0;
    HKEY key;
    long rc;
    long size = 0;
    DWORD type;
    char*buf;
    rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_ALL_ACCESS/* KEY_READ*/, &key);
    if (rc != ERROR_SUCCESS) {
	fprintf(stderr, "RegOpenKeyEx failed\n");
	return 0;
    }
    rc = RegQueryValueEx(key, NULL, 0, 0, 0, &size) ;
    if(rc != ERROR_SUCCESS) {
	fprintf(stderr, "RegQueryValueEx(1) failed: %d\n", rc);
	return 0;
    }
    buf = malloc(size+1);
    rc = RegQueryValueEx(key, NULL, 0, &type, (BYTE*)buf, &size);
    if(rc != ERROR_SUCCESS) {
	fprintf(stderr, "RegQueryValueEx(2) failed: %d\n", rc);
	return 0;
    }
    if(type == REG_SZ || type == REG_EXPAND_SZ) {
	while(size && buf[size-1] == '\0')
          --size;
	buf[size] = 0;
	/* TODO: convert */
	return buf;
    } else if(type == REG_BINARY) {
	return buf;
    }
}

int setRegistryEntry(char*key,char*value)
{
    HKEY hkey;
    int ret = 0;
    ret = RegCreateKey(HKEY_LOCAL_MACHINE, key, &hkey);
    if(ret != ERROR_SUCCESS) {
	fprintf(stderr, "registry: CreateKey %s failed\n", key);
	return 0;
    }
    ret = RegSetValue(hkey, NULL, REG_SZ, value, strlen(value)+1);
    if(ret != ERROR_SUCCESS) {
	fprintf(stderr, "registry: SetValue %s failed\n", key);
	return 0;
    }
    return 1;
}


#endif

//HINSTANCE me =  GetModuleHandle(NULL);

char* getInstallationPath()
{
#if defined(WIN32)
    char* path = getRegistryEntry("Software\\quiss.org\\swftools\\InstallPath");
    if(path)
	return path;
    else
	return 0;
#elif defined(CYGWIN)
    return SWFTOOLS_DATADIR;
#else
    return SWFTOOLS_DATADIR;
#endif
}

char* concatPaths(char*base, char*add)
{
    int l1 = strlen(base);
    int l2 = strlen(add);
    int pos = 0;
    char*n = 0;
    while(l1 && base[l1-1] == seperator)
	l1--;
    while(pos < l2 && add[pos] == seperator)
	pos++;

    n = malloc(l1 + (l2-pos) + 2);
    memcpy(n,base,l1);
    n[l1]=seperator;
    strcpy(&n[l1+1],&add[pos]);
    return n;
}

char* stripFilename(char*filename, char*newext)
{
    char*last1 = strrchr(filename, '/');
    char*last2 = strrchr(filename, '\\');
    char*pos = filename;
    char*name;
    char*dot;
    if(last1>pos) pos = last1 + 1;
    if(last2>pos) pos = last2 + 1;
    name = (char*)malloc(strlen(pos)+2+(newext?strlen(newext):3));
    strcpy(name, pos);
    dot = strrchr(name, '.');
    if(dot) {
	*dot = 0;
    }
    if(newext)
	strcat(name, newext);
    return name;
}

