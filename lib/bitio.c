/*  bitio.c 
    part of swftools
    implementation of bitio.h.

    Copyright (C) 2003 Matthias Kramm <kramm@quiss.org>

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
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>

#include "../config.h"

#ifdef HAVE_ZLIB
#include <zlib.h>
#define ZLIB_BUFFER_SIZE 16384
#endif
#include "./bitio.h"

/* ---------------------------- null reader ------------------------------- */

static int reader_nullread(struct reader_t*r, void* data, int len) 
{
    memset(data, 0, len);
    return len;
}
static void reader_nullread_dealloc(struct reader_t*r)
{
    memset(r, 0, sizeof(struct reader_t));
}
void reader_init_nullreader(struct reader_t*r)
{
    r->read = reader_nullread;
    r->dealloc = reader_nullread_dealloc;
    r->internal = 0;
    r->type = READER_TYPE_NULL;
    r->mybyte = 0;
    r->bitpos = 8;
    r->pos = 0;
}
/* ---------------------------- file reader ------------------------------- */

static int reader_fileread(struct reader_t*reader, void* data, int len) 
{
    int ret = read((int)reader->internal, data, len);
    if(ret>=0)
	reader->pos += ret;
    return ret;
}
void reader_init_filereader(struct reader_t*r, int handle)
{
    r->read = reader_fileread;
    r->internal = (void*)handle;
    r->type = READER_TYPE_FILE;
    r->mybyte = 0;
    r->bitpos = 8;
    r->pos = 0;
}

/* ---------------------------- mem reader ------------------------------- */

struct memread_t
{
    unsigned char*data;
    int length;
};
static int reader_memread(struct reader_t*reader, void* data, int len) 
{
    struct memread_t*mr = (struct memread_t*)reader->internal;

    if(mr->length - reader->pos > len) {
	memcpy(data, &mr->data[reader->pos], len);
	reader->pos += len;
	return len;
    } else {
	memcpy(data, &mr->data[reader->pos], mr->length - reader->pos);
	reader->pos = mr->length;
	return mr->length - reader->pos;
    }
}
static void reader_memread_dealloc(struct reader_t*reader)
{
    if(reader->internal)
	free(reader->internal);
    memset(reader, 0, sizeof(struct reader_t));
}
void reader_init_memreader(struct reader_t*r, void*newdata, int newlength)
{
    struct memread_t*mr = malloc(sizeof(struct memread_t));
    mr->data = newdata;
    mr->length = newlength;
    r->read = reader_memread;
    r->dealloc = reader_memread_dealloc;
    r->internal = (void*)mr;
    r->type = READER_TYPE_MEM;
    r->mybyte = 0;
    r->bitpos = 8;
    r->pos = 0;
} 

/* ---------------------------- mem writer ------------------------------- */

struct memwrite_t
{
    unsigned char*data;
    int length;
};

static int writer_memwrite_write(struct writer_t*w, void* data, int len) 
{
    struct memwrite_t*mw = (struct memwrite_t*)w->internal;
    if(mw->length - w->pos > len) {
	memcpy(&mw->data[w->pos], data, len);
	w->pos += len;
	return len;
    } else {
	memcpy(&mw->data[w->pos], data, mw->length - w->pos);
	w->pos = mw->length;
	return mw->length - w->pos;
    }
}
static void writer_memwrite_finish(struct writer_t*w)
{
    if(w->internal) 
	free(w->internal);
    w->internal = 0;
}
void writer_init_memwriter(struct writer_t*w, void*data, int len)
{
    struct memwrite_t *mr;
    mr = malloc(sizeof(struct memwrite_t));
    mr->data = data;
    mr->length = len;
    memset(w, 0, sizeof(struct writer_t));
    w->write = writer_memwrite_write;
    w->finish = writer_memwrite_finish;
    w->internal = (void*)mr;
    w->type = WRITER_TYPE_MEM;
    w->bitpos = 0;
    w->mybyte = 0;
    w->pos = 0;
}

/* ------------------------- growing mem writer ------------------------------- */

struct growmemwrite_t
{
    unsigned char*data;
    int length;
};
static int writer_growmemwrite_write(struct writer_t*w, void* data, int len) 
{
    struct growmemwrite_t*mw = (struct growmemwrite_t*)w->internal;
    if(!mw->data) {
	fprintf(stderr, "Illegal write operation: data already given away");
	exit(1);
    }
    if(mw->length - w->pos <= len) {
	unsigned char*newmem;
	int newlength = mw->length;
	while(newlength - w->pos < len) {
	    newlength += 4096;
	}
	newmem = malloc(newlength);
	memcpy(newmem, mw->data, mw->length);
	free(mw->data);
	mw->data = newmem;
	mw->length = newlength;
    }
    memcpy(&mw->data[w->pos], data, len);
    w->pos += len;
    return len;
}
static void writer_growmemwrite_finish(struct writer_t*w)
{
    struct growmemwrite_t*mw = (struct growmemwrite_t*)w->internal;
    if(mw->data)
	free(mw->data);
    mw->data = 0;
    mw->length = 0;
    free(w->internal);mw=0;
    memset(w, 0, sizeof(struct writer_t));
}
void* writer_growmemwrite_getmem(struct writer_t*w)
{
    struct growmemwrite_t*mw = (struct growmemwrite_t*)w->internal;
    void*ret = mw->data;
    /* remove own reference so that neither write() nor finish() can free it.
       It's property of the caller now.
    */
    mw->data = 0;
    return ret;
}
void writer_init_growingmemwriter(struct writer_t*w)
{
    struct memwrite_t *mr;
    mr = malloc(sizeof(struct memwrite_t));
    mr->length = 4096;
    mr->data = malloc(mr->length);
    memset(w, 0, sizeof(struct writer_t));
    w->write = writer_growmemwrite_write;
    w->finish = writer_growmemwrite_finish;
    w->internal = (void*)mr;
    w->type = WRITER_TYPE_GROWING_MEM;
    w->bitpos = 0;
    w->mybyte = 0;
    w->pos = 0;
}

/* ---------------------------- file writer ------------------------------- */

struct filewrite_t
{
    int handle;
    char free_handle;
};

static int writer_filewrite_write(struct writer_t*w, void* data, int len) 
{
    struct filewrite_t * fw= (struct filewrite_t*)w->internal;
    return write(fw->handle, data, len);
}
static void writer_filewrite_finish(struct writer_t*w)
{
    struct filewrite_t *mr = (struct filewrite_t*)w->internal;
    if(mr->free_handle)
	close(mr->handle);
    free(w->internal);
    memset(w, 0, sizeof(struct writer_t));
}
void writer_init_filewriter(struct writer_t*w, int handle)
{
    struct filewrite_t *mr = malloc(sizeof(struct filewrite_t));
    mr->handle = handle;
    mr->free_handle = 0;
    memset(w, 0, sizeof(struct writer_t));
    w->write = writer_filewrite_write;
    w->finish = writer_filewrite_finish;
    w->internal = mr;
    w->type = WRITER_TYPE_FILE;
    w->bitpos = 0;
    w->mybyte = 0;
    w->pos = 0;
}
void writer_init_filewriter2(struct writer_t*w, char*filename)
{
    int fi = open("movie.swf",
#ifdef O_BINARY
	    O_BINARY|
#endif
	    O_WRONLY|O_CREAT|O_TRUNC, 0644);
    writer_init_filewriter(w, fi);
    ((struct filewrite_t*)w->internal)->free_handle = 1;
}

/* ---------------------------- null writer ------------------------------- */

static int writer_nullwrite_write(struct writer_t*w, void* data, int len) 
{
    w->pos += len;
    return len;
}
static void writer_nullwrite_finish(struct writer_t*w)
{
    memset(w, 0, sizeof(struct writer_t));
}
void writer_init_nullwriter(struct writer_t*w)
{
    memset(w, 0, sizeof(struct writer_t));
    w->write = writer_nullwrite_write;
    w->finish = writer_nullwrite_finish;
    w->internal = 0;
    w->type = WRITER_TYPE_NULL;
    w->bitpos = 0;
    w->mybyte = 0;
    w->pos = 0;
}
/* ---------------------------- zlibinflate reader -------------------------- */

struct zlibinflate_t
{
#ifdef HAVE_ZLIB
    z_stream zs;
    struct reader_t*input;
    unsigned char readbuffer[ZLIB_BUFFER_SIZE];
#endif
};

#ifdef HAVE_ZLIB
static void zlib_error(int ret, char* msg, z_stream*zs)
{
    fprintf(stderr, "%s: zlib error (%d): last zlib error: %s\n",
	  msg,
	  ret,
	  zs->msg?zs->msg:"unknown");
    perror("errno:");
    exit(1);
}
#endif

static int reader_zlibinflate(struct reader_t*reader, void* data, int len) 
{
#ifdef HAVE_ZLIB
    struct zlibinflate_t*z = (struct zlibinflate_t*)reader->internal;
    int ret;
    if(!z) {
	return 0;
    }
    if(!len)
	return 0;
    
    z->zs.next_out = data;
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
#else
    fprintf(stderr, "Error: swftools was compiled without zlib support");
    exit(1);
#endif
}
static void reader_zlibinflate_dealloc(struct reader_t*reader)
{
#ifdef HAVE_ZLIB
    struct zlibinflate_t*z = (struct zlibinflate_t*)reader->internal;
    /* test whether read() already did basic deallocation */
    if(reader->internal) {
	inflateEnd(&z->zs);
	free(reader->internal);
    }
    memset(reader, 0, sizeof(struct reader_t));
#endif
}
void reader_init_zlibinflate(struct reader_t*r, struct reader_t*input)
{
#ifdef HAVE_ZLIB
    struct zlibinflate_t*z;
    int ret;
    memset(r, 0, sizeof(struct reader_t));
    z = (struct zlibinflate_t*)malloc(sizeof(struct zlibinflate_t));
    memset(z, 0, sizeof(struct zlibinflate_t));
    r->internal = z;
    r->read = reader_zlibinflate;
    r->dealloc = reader_zlibinflate_dealloc;
    r->type = READER_TYPE_ZLIB;
    r->pos = 0;
    z->input = input;
    memset(&z->zs,0,sizeof(z_stream));
    z->zs.zalloc = Z_NULL;
    z->zs.zfree  = Z_NULL;
    z->zs.opaque = Z_NULL;
    ret = inflateInit(&z->zs);
    if (ret != Z_OK) zlib_error(ret, "bitio:inflate_init", &z->zs);
    reader_resetbits(r);
#else
    fprintf(stderr, "Error: swftools was compiled without zlib support");
#endif
}

/* ---------------------------- zlibdeflate writer -------------------------- */

struct zlibdeflate_t
{
#ifdef HAVE_ZLIB
    z_stream zs;
    struct writer_t*output;
    unsigned char writebuffer[ZLIB_BUFFER_SIZE];
#endif
};

static int writer_zlibdeflate_write(struct writer_t*writer, void* data, int len) 
{
#ifdef HAVE_ZLIB
    struct zlibdeflate_t*z = (struct zlibdeflate_t*)writer->internal;
    int ret;
    if(writer->type != WRITER_TYPE_ZLIB) {
	fprintf(stderr, "Wrong writer ID (writer not initialized?)\n");
	return 0;
    }
    if(!z) {
	fprintf(stderr, "zlib not initialized!\n");
	return 0;
    }
    if(!len)
	return 0;
    
    z->zs.next_in = data;
    z->zs.avail_in = len;

    while(1) {
	ret = deflate(&z->zs, Z_NO_FLUSH);
	
	if (ret != Z_OK) zlib_error(ret, "bitio:deflate_deflate", &z->zs);

	if(z->zs.next_out != z->writebuffer) {
	    z->output->write(z->output, z->writebuffer, z->zs.next_out - (Bytef*)z->writebuffer);
	    z->zs.next_out = z->writebuffer;
	    z->zs.avail_out = ZLIB_BUFFER_SIZE;
	}

	if(!z->zs.avail_in) {
	    break;
	}
    }
    writer->pos += len;
    return len;
#else
    fprintf(stderr, "Error: swftools was compiled without zlib support");
    exit(1);
#endif
}
static void writer_zlibdeflate_finish(struct writer_t*writer)
{
#ifdef HAVE_ZLIB
    struct zlibdeflate_t*z = (struct zlibdeflate_t*)writer->internal;
    struct writer_t*output;
    int ret;
    if(writer->type != WRITER_TYPE_ZLIB) {
	fprintf(stderr, "Wrong writer ID (writer not initialized?)\n");
	return;
    }
    if(!z)
	return;
    output= z->output;
    while(1) {
	ret = deflate(&z->zs, Z_FINISH);
	if (ret != Z_OK &&
	    ret != Z_STREAM_END) zlib_error(ret, "bitio:deflate_finish", &z->zs);

	if(z->zs.next_out != z->writebuffer) {
	    z->output->write(z->output, z->writebuffer, z->zs.next_out - (Bytef*)z->writebuffer);
	    z->zs.next_out = z->writebuffer;
	    z->zs.avail_out = ZLIB_BUFFER_SIZE;
	}

	if (ret == Z_STREAM_END) {
	    break;

	}
    }
    ret = deflateEnd(&z->zs);
    if (ret != Z_OK) zlib_error(ret, "bitio:deflate_end", &z->zs);
    free(writer->internal);
    memset(writer, 0, sizeof(struct writer_t));
    //output->finish(output); 
#else
    fprintf(stderr, "Error: swftools was compiled without zlib support");
    exit(1);
#endif
}
void writer_init_zlibdeflate(struct writer_t*w, struct writer_t*output)
{
#ifdef HAVE_ZLIB
    struct zlibdeflate_t*z;
    int ret;
    memset(w, 0, sizeof(struct writer_t));
    z = (struct zlibdeflate_t*)malloc(sizeof(struct zlibdeflate_t));
    memset(z, 0, sizeof(struct zlibdeflate_t));
    w->internal = z;
    w->write = writer_zlibdeflate_write;
    w->finish = writer_zlibdeflate_finish;
    w->type = WRITER_TYPE_ZLIB;
    w->pos = 0;
    z->output = output;
    memset(&z->zs,0,sizeof(z_stream));
    z->zs.zalloc = Z_NULL;
    z->zs.zfree  = Z_NULL;
    z->zs.opaque = Z_NULL;
    ret = deflateInit(&z->zs, 9);
    if (ret != Z_OK) zlib_error(ret, "bitio:deflate_init", &z->zs);
    w->bitpos = 0;
    w->mybyte = 0;
    z->zs.next_out = z->writebuffer;
    z->zs.avail_out = ZLIB_BUFFER_SIZE;
#else
    fprintf(stderr, "Error: swftools was compiled without zlib support");
    exit(1);
#endif
}

/* ----------------------- bit handling routines -------------------------- */

void writer_writebit(struct writer_t*w, int bit)
{    
    if(w->bitpos==8) 
    {
        w->write(w, &w->mybyte, 1);
	w->bitpos = 0;
	w->mybyte = 0;
    }
    if(bit&1)
	w->mybyte |= 1 << (7 - w->bitpos);
    w->bitpos ++;
}
void writer_writebits(struct writer_t*w, unsigned int data, int bits)
{
    int t;
    for(t=0;t<bits;t++)
    {
	writer_writebit(w, (data >> (bits-t-1))&1);
    }
}
void writer_resetbits(struct writer_t*w)
{
    if(w->bitpos)
	w->write(w, &w->mybyte, 1);
    w->bitpos = 0;
    w->mybyte = 0;
}
 
unsigned int reader_readbit(struct reader_t*r)
{
    if(r->bitpos==8) 
    {
	r->bitpos=0;
        r->read(r, &r->mybyte, 1);
    }
    return (r->mybyte>>(7-r->bitpos++))&1;
}
unsigned int reader_readbits(struct reader_t*r, int num)
{
    int t;
    int val = 0;
    for(t=0;t<num;t++)
    {
	val<<=1;
	val|=reader_readbit(r);
    }
    return val;
}
void reader_resetbits(struct reader_t*r)
{
    r->mybyte = 0;
    r->bitpos = 8;

}
U8 readU8(struct reader_t*r)
{
    U8 b;
    r->read(r, &b, 1);
    return b;
}
U16 readU16(struct reader_t*r)
{
    U8 b[2];
    r->read(r, b, 2);
    return b[0]|b[1]<<8;
}
U32 readU32(struct reader_t*r)
{
    U8 b[4];
    r->read(r, b, 4);
    return b[0]|b[1]<<8|b[2]<<16|b[3]<<24;
}
void writeU8(struct writer_t*wr, U8 b)
{
    wr->write(wr, &b, 1);
}
void writeU16(struct writer_t*wr, U16 w)
{
    U8 b[2];
    b[0] = w;
    b[1] = w>>8;
    wr->write(wr, b, 2);
}
void writeU32(struct writer_t*wr, U32 d) 
{
    U8 b[4];
    b[0] = d;
    b[1] = d>>8;
    b[2] = d>>16;
    b[3] = d>>24;
    wr->write(wr, b, 4);
}

