/* archive.c

   Part of the swftools installer.
   
   Copyright (c) 2004 Matthias Kramm <kramm@quiss.org> 
 
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
#ifdef WIN32
#include <windows.h>
#include <shlwapi.h>
#else
#include <sys/stat.h>
#endif
#include "archive.h"
#include "depack.h"


typedef struct _reader
{
    unsigned char*mem;
    int readpos;
} reader_t;

static void myread(void*self, void*mem, int len)
{
    /*if(readpos+len >= sizeof(crndata)) {
	len = sizeof(crndata) - readpos;
    }*/
    reader_t*r = (reader_t*)self;
    memcpy(mem, r->mem+r->readpos, len);
    r->readpos += len;
    //return len;
}

typedef  struct _writer
{
    unsigned char*mem;
    int memlen;
} writer_t;

static void mywrite(void*self, void*buf, int len)
{
    writer_t*w = (writer_t*)self;
    memcpy(w->mem+w->memlen, buf, len);
    w->memlen += len;
}

static int verbose = 0;
static void msg(char*format, ...)
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
    printf("(archive) %s\n", buf);
    fflush(stdout);
}

static int create_directory(char*path,statusfunc_t f)
{
    if(!path || !*path || (*path=='.' && (!path[1] || path[1]=='.')))
	return 1; //nothing to do
    while(path[0]=='.' && (path[1]=='/' || path[1]=='\\'))
	path+=2;

#ifdef WIN32
    if(PathIsDirectoryA(path))
	return 1;
#else
    struct stat st;
    if(stat(path, &st)>=0) {
	if(S_ISDIR(st.st_mode)) {
	    return 1; /* already exists */
	}
    }
#endif

    if(mkdir(path,0755)<0) {
	char buf[1024];
	sprintf(buf, "create directory \"%s\" FAILED", path);
	f(-1, buf);
	return 0;
    }
    return 1;
}
static int goto_directory(char*path,statusfunc_t f)
{
    if(chdir(path)<0) {
	char buf[1024];
	sprintf(buf, "changing to directory \"%s\" FAILED", path);
	f(-1, buf);
	return 0;
    }
    return 1;
}
static char basenamebuf[256];
static char*get_directory(char*filename)
{
    char*r1 = strrchr(filename, '\\');
    char*r2 = strrchr(filename, '/');
    char*r = r1>r2?r1:r2;
    if(!r)
	return "";
    memcpy(basenamebuf, filename, r-filename);
    basenamebuf[r-filename] = 0;
    //msg("directory name of \"%s\" is \"%s\"", filename, basenamebuf);
    return basenamebuf;
}
static int write_file(char*filename, void*mem, int len,statusfunc_t f)
{
    while(filename[0]=='.' && (filename[1]=='/' || filename[1]=='\\'))
	filename+=2;

    filename=strdup(filename);

    char*p = filename;
    while(*p) {
	if(*p=='/') *p='\\';
	p++;
    }

    msg("create file \"%s\" (%d bytes)", filename, len);
    FILE*fo = fopen(filename, "wb");
    if(!fo) {
	char buf[1024];
	sprintf(buf, "Couldn't create file %s", filename);
	f(-1, buf);
	free(filename);
	return 0;
    }
    fwrite(mem, len, 1, fo);
    fclose(fo);
    free(filename);
    return 1;
}

int unpack_archive(void*data, char*destdir, statusfunc_t f)
{
    depack_t d;

    reader_t r;
    r.mem = (unsigned char*)data;
    r.readpos = 0;
    
    f(0, "Reading compressed data");
    depack_init(&d,&r,myread);
    
    writer_t w;
    w.mem = malloc(d.size);
    w.memlen = 0;

    f(0, "Decompressing data");
    depack_process(&d,&w,mywrite);
    depack_destroy(&d);

    f(0, "Creating installation directory");
    if(!create_directory(destdir,f)) return 0;
    f(0, "Changing to installation directory");
    if(!goto_directory(destdir,f)) return 0;

    f(0, "Copying files...");
    int pos = 0;
    while(pos<w.memlen) {
	unsigned char*mem = w.mem;
	/* read id */
	unsigned char id[4];
	id[3] = 0;
	memcpy(id, &mem[pos], 3);
	pos += 3;

	/* read size */
	int len = mem[pos]|mem[pos+1]<<8|mem[pos+2]<<16|mem[pos+3]<<24;
	pos += 4;
	
	/* read filename */
	char*filename = &mem[pos];
	int l = strlen(filename);
	pos+=l+1;

	if(verbose) printf("[%s] %s %d\n", id, filename, len);
	char buf[2048];
	sprintf(buf, "[%s] %s (%d bytes)", id, filename, len);
	f(0, buf);
	if(*(U32*)id == *(U32*)"DIR") {
	    if(!create_directory(filename,f)) return 0;
	} else {
	    if(!create_directory(get_directory(filename),f)) return 0;
	    if(!write_file(filename,mem+pos,len,f)) return 0;
	}
	
	pos += len;
    }
    f(0, "Installation finished");
    return 1;
}


