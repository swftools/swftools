/* bitio.h 
   Header file for bitio.c.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __bitio_h__
#define __bitio_h__

#include "types.h"

struct reader_t {
    u8* data;
    int datalength;
    int datapos;
    u8 bitpos,mybyte;
    u8 bitmem;
};

struct writer_t {
    u8* data;
    int maxlength;
    int pos;
    int bitpos;
    u8 mybyte;
};

void reader_resetbits(struct reader_t*);
void reader_init(struct reader_t*,uchar*newdata, int newlength);
void reader_skip(struct reader_t*,int length);
void reader_input1(struct reader_t*, void*target);
void reader_input2(struct reader_t*, void*target);
void reader_input4(struct reader_t*, void*target);
uchar*reader_getinputpos(struct reader_t*);
int reader_getinputlength(struct reader_t*);
void reader_setinputpos(struct reader_t*,uchar*pos);
u32 reader_readbit(struct reader_t*);
void reader_readbits(struct reader_t*,u32*val,int num);
void reader_readsbits(struct reader_t*,s32*val,int num);
u32 reader_getbits(struct reader_t*,int num);
s32 reader_getsbits(struct reader_t*,int num);
u8 reader_readu8(struct reader_t*);
u16 reader_readu16(struct reader_t*);
u32 reader_readu32(struct reader_t*);

void writer_init(struct writer_t*w, u8*data, int maxlength);
void writer_write(struct writer_t*w, void*data, int length);
void* writer_getpos(struct writer_t*w);
void writer_writebit(struct writer_t*w, int bit);
void writer_writebits(struct writer_t*w, u32 data, int bits);
void writer_writeu8(struct writer_t*w, u8 value);
void writer_writeu16(struct writer_t*w, u16 value);
void writer_writeu32(struct writer_t*w, u32 value);
void writer_resetbits(struct writer_t*w);

#endif //__bitio_h__
