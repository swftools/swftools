/* bitio.h
   Generic object-oriented reading/writing.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 
 
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
#include "types.h"
#include "../config.h"

#ifdef HAVE_ZZIP
#include "zzip/lib.h"
#endif

#ifndef __rfxswf_bitio_h__
#define __rfxswf_bitio_h__

#define READER_TYPE_FILE 1
#define READER_TYPE_MEM  2
#define READER_TYPE_ZLIB_U 3
#define READER_TYPE_ZLIB_C 4
#define READER_TYPE_ZLIB READER_TYPE_ZLIB_U
#define READER_TYPE_NULL 5
#define READER_TYPE_FILE2 6
#define READER_TYPE_ZZIP 7

#define WRITER_TYPE_FILE 1
#define WRITER_TYPE_MEM  2
#define WRITER_TYPE_ZLIB_C 3
#define WRITER_TYPE_ZLIB_U 4
#define WRITER_TYPE_NULL 5
#define WRITER_TYPE_GROWING_MEM  6
#define WRITER_TYPE_ZLIB WRITER_TYPE_ZLIB_C

typedef struct _reader
{
    int (*read)(struct _reader*, void*data, int len);
    int (*seek)(struct _reader*, int pos);
    void (*dealloc)(struct _reader*);

    union {
        int fd;
        int null;
        void *ptr;
    };
    int type;
    unsigned char mybyte;
    unsigned char bitpos;
    int pos;
} reader_t;

typedef struct _writer
{
    int (*write)(struct _writer*, void*data, int len);
    void (*flush)(struct _writer*);
    void (*finish)(struct _writer*);

    union {
        int fd;
        int null;
        void *ptr;
    };
    int type;
    unsigned char mybyte;
    unsigned char bitpos;
    int pos;
} writer_t;

void reader_resetbits(reader_t*r);
unsigned int reader_readbit(reader_t*r);
unsigned int reader_readbits(reader_t*r, int num);

U8 reader_readU8(reader_t*r);
U16 reader_readU16(reader_t*r);
U32 reader_readU32(reader_t*r);
float reader_readFloat(reader_t*r);
double reader_readDouble(reader_t*r);
char*reader_readString(reader_t*r);

unsigned int read_compressed_uint(reader_t*r);
int read_compressed_int(reader_t*r);

void writer_resetbits(writer_t*w);
void writer_writebit(writer_t*w, int bit);
void writer_writebits(writer_t*w, unsigned int data, int bits);

void writer_writeU8(writer_t*w, unsigned char b);
void writer_writeU16(writer_t*w, unsigned short v);
void writer_writeU32(writer_t*w, unsigned long v);
void writer_writeFloat(writer_t*w, float f);
void writer_writeDouble(writer_t*w, double f);
void writer_writeString(writer_t*w, const char*s);

void write_compressed_uint(writer_t*w, unsigned int u);
void write_compressed_int(writer_t*w, int i);

/* standard readers / writers */

void reader_init_filereader(reader_t*r, int handle);
int reader_init_filereader2(reader_t*r, const char*filename);
void reader_init_zlibinflate(reader_t*r, reader_t*input);
void reader_init_memreader(reader_t*r, void*data, int length);
void reader_init_nullreader(reader_t*r);
#ifdef HAVE_ZZIP
void reader_init_zzipreader(reader_t*r,ZZIP_FILE*z);
#endif

void writer_init_filewriter(writer_t*w, int handle);
void writer_init_filewriter2(writer_t*w, char*filename);
void writer_init_zlibdeflate(writer_t*w, writer_t*output);
void writer_init_memwriter(writer_t*r, void*data, int length);
void writer_init_nullwriter(writer_t*w);

void writer_init_growingmemwriter(writer_t*r, U32 grow);
void* writer_growmemwrite_memptr(writer_t*w, int*len);
void* writer_growmemwrite_getmem(writer_t*w);
void writer_growmemwrite_reset(writer_t*w);

#endif //__rfxswf_bitio_h__
