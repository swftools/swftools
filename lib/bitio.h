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

#ifndef __rfxswf_bitio_h__
#define __rfxswf_bitio_h__

#define READER_TYPE_FILE 0
#define READER_TYPE_MEM  1
#define READER_TYPE_ZLIB 2
#define WRITER_TYPE_FILE 0
#define WRITER_TYPE_MEM  1
#define WRITER_TYPE_ZLIB 2
#define WRITER_TYPE_NULL 3

struct reader_t
{
    int (*read)(struct reader_t*, void*data, int len);

    void *internal;
    int type;
    unsigned char mybyte;
    unsigned char bitpos;
    int pos;
};

struct writer_t
{
    int (*write)(struct writer_t*, void*data, int len);
    void (*finish)(struct writer_t*);

    void *internal;
    int type;
    unsigned char mybyte;
    unsigned char bitpos;
    int pos;
};

void reader_resetbits(struct reader_t*r);
unsigned int reader_readbit(struct reader_t*r);
unsigned int reader_readbits(struct reader_t*r, int num);

void writer_resetbits(struct writer_t*w);
void writer_writebit(struct writer_t*w, int bit);
void writer_writebits(struct writer_t*w, unsigned int data, int bits);

/* standard readers / writers */

void reader_init_filereader(struct reader_t*r, int handle);
void reader_init_zlibinflate(struct reader_t*r, struct reader_t*input);
void reader_init_memreader(struct reader_t*r, void*data, int length);

void writer_init_filewriter(struct writer_t*w, int handle);
void writer_init_filewriter2(struct writer_t*w, char*filename);
void writer_init_zlibdeflate(struct writer_t*w, struct writer_t*output);
void writer_init_memwriter(struct writer_t*r, void*data, int length);
void writer_init_nullwriter(struct writer_t*w);

#endif //__rfxswf_bitio_h__
