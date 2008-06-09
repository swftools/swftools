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
#include "../z/zlib.h"
#define ZLIB_BUFFER_SIZE 16384

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


typedef struct _reader
{
    int (*read)(struct _reader*, void*data, int len);
    void (*dealloc)(struct _reader*);
    void *internal;
    int pos;
} reader_t;

/* ---------------------------- mem reader ------------------------------- */

struct memread_t
{
    unsigned char*data;
    int length;
};
static int reader_memread(reader_t*reader, void* data, int _len) 
{
    struct memread_t*mr = (struct memread_t*)reader->internal;

    int len = _len;
    if(mr->length - reader->pos < len) {
	len = mr->length - reader->pos;
    }
    memcpy(data, &mr->data[reader->pos], len);
    msg("at pos %d, asked to read %d bytes, did read %d bytes\n", reader->pos, _len, len);
    reader->pos += len;
    return len;
}
static void reader_memread_dealloc(reader_t*reader)
{
    if(reader->internal)
	free(reader->internal);
    memset(reader, 0, sizeof(reader_t));
}
reader_t*reader_init_memreader(void*newdata, int newlength)
{
    reader_t*r = malloc(sizeof(reader_t));
    struct memread_t*mr = (struct memread_t*)malloc(sizeof(struct memread_t));
    mr->data = (unsigned char*)newdata;
    mr->length = newlength;
    r->read = reader_memread;
    r->dealloc = reader_memread_dealloc;
    r->internal = (void*)mr;
    r->pos = 0;
    return r;
} 


/* ---------------------------- zlibinflate reader -------------------------- */
struct zlibinflate_t
{
    z_stream zs;
    reader_t*input;
    unsigned char readbuffer[ZLIB_BUFFER_SIZE];
};

static void zlib_error(int ret, char* msg, z_stream*zs)
{
    fprintf(stderr, "%s: zlib error (%d): last zlib error: %s\n", msg, ret, zs->msg?zs->msg:"unknown");
    perror("errno:");
    exit(1);
}

static int reader_zlibinflate(reader_t*reader, void* data, int len) 
{
    struct zlibinflate_t*z = (struct zlibinflate_t*)reader->internal;
    int ret;
    if(!z) {
	return 0;
    }
    if(!len)
	return 0;
    
    z->zs.next_out = (Bytef *)data;
    z->zs.avail_out = len;

    while(1) {
	if(!z->zs.avail_in) {
	    z->zs.avail_in = z->input->read(z->input, z->readbuffer, ZLIB_BUFFER_SIZE);
	    z->zs.next_in = z->readbuffer;
	}
	if(z->zs.avail_in)
	    ret = inflate(&z->zs, Z_NO_FLUSH);
	else
	    ret = inflate(&z->zs, Z_FINISH);
    
	if (ret != Z_OK &&
	    ret != Z_STREAM_END) zlib_error(ret, "bitio:inflate_inflate", &z->zs);

	if (ret == Z_STREAM_END) {
		int pos = z->zs.next_out - (Bytef*)data;
		ret = inflateEnd(&z->zs);
		if (ret != Z_OK) zlib_error(ret, "bitio:inflate_end", &z->zs);
		free(reader->internal);
		reader->internal = 0;
		reader->pos += pos;
		return pos;
	}
	if(!z->zs.avail_out) {
	    break;
	}
    }
    reader->pos += len;
    return len;
}
static void reader_zlibinflate_dealloc(reader_t*reader)
{
    struct zlibinflate_t*z = (struct zlibinflate_t*)reader->internal;
    if(z) {
	if(z->input) {
	    z->input->dealloc(z->input);z->input = 0;
	}
	inflateEnd(&z->zs);
	free(reader->internal);
    }
    memset(reader, 0, sizeof(reader_t));
}
reader_t* reader_init_zlibinflate(reader_t*input)
{
    reader_t*r = malloc(sizeof(reader_t));
    struct zlibinflate_t*z;
    int ret;
    memset(r, 0, sizeof(reader_t));
    z = (struct zlibinflate_t*)malloc(sizeof(struct zlibinflate_t));
    memset(z, 0, sizeof(struct zlibinflate_t));
    r->internal = z;
    r->read = reader_zlibinflate;
    r->dealloc = reader_zlibinflate_dealloc;
    r->pos = 0;
    z->input = input;
    memset(&z->zs,0,sizeof(z_stream));
    z->zs.zalloc = Z_NULL;
    z->zs.zfree  = Z_NULL;
    z->zs.opaque = Z_NULL;
    ret = inflateInit(&z->zs);
    if (ret != Z_OK) zlib_error(ret, "bitio:inflate_init", &z->zs);
    return r;
}

/* -------------------------------------------------------------------------- */


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
	perror("mkdir");
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
static int write_file(char*filename, reader_t*r, int len,statusfunc_t f)
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
    int pos=0;
    char buf[4096];
    while(pos<len) {
	int l = 4096;
	if(pos+l>len)
	    l = len-pos;
	int n = r->read(r, buf, l);
	if(n < l) {
	    char buf[1024];
	    sprintf(buf, "Couldn't read byte %d (pos+%d) from input buffer for file %s", pos+n, n, filename);
	    f(-1, buf);
	    return 0;
	}
	fwrite(buf, l, 1, fo);
	pos+=l;
    }
    fclose(fo);
    free(filename);
    return 1;
}

int unpack_archive(void*data, int len, char*destdir, statusfunc_t f)
{
    reader_t*m = reader_init_memreader(data, len);
    reader_t*z = reader_init_zlibinflate(m);

    f(0, "Creating installation directory");
    if(!create_directory(destdir,f)) return 0;
    f(0, "Changing to installation directory");
    if(!goto_directory(destdir,f)) return 0;
    
    f(0, "Uncompressing files...");
    while(1) {
	/* read id */
	unsigned char id[4];
	id[3] = 0;
	if(z->read(z, id, 3)<3) {
	    f(-1, "Unexpected end of archive");
	    return 0;
	}
	if(!strcmp(id, "END"))
	    break;

	unsigned b1=0,b2=0,b3=0,b4=0;
	int l = 0;
	l+=z->read(z, &b1, 1);
	l+=z->read(z, &b2, 1);
	l+=z->read(z, &b3, 1);
	l+=z->read(z, &b4, 1);
	if(l<4)
	    return 0;

	/* read size */
	int len = b1|b2<<8|b3<<16|b4<<24;
	
	/* read filename */
	unsigned char filename_len;
	z->read(z, &filename_len, 1);
	char*filename = malloc(filename_len+1);
	z->read(z, filename, filename_len);
	filename[(int)filename_len] = 0;

	if(verbose) printf("[%s] %s %d\n", id, filename, len);
	char buf[2048];
	sprintf(buf, "[%s] %s (%d bytes)", id, filename, len);
	f(0, buf);
	if(!strcmp(id, "DIR")) {
	    if(!create_directory(filename,f)) return 0;
	} else {
	    if(!create_directory(get_directory(filename),f)) return 0;

	    if(!write_file(filename,z,len,f)) return 0;
	}
    }
    f(0, "Finishing Installation");
    return 1;
}

