/* bitio.h 
   Header file for bitio.c.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __bitio_h__
#define __bitio_h__

#include "types.h"

void resetbits();
void reader_init(uchar*newdata, int newlength);
void skip(int length);
void input1(void*target);
void input2(void*target);
void input4(void*target);

uchar*getinputpos();
int getinputlength();
void setinputpos(uchar*pos);

u32 readbit();
void readbits(u32*val,int num);
void readsbits(s32*val,int num);
u32 getbits(int num);
s32 getsbits(int num);
u8 readu8();
u16 readu16();
u32 readu32();

struct writer_t {
    u8* data;
    int maxlength;
    int pos;
    int bitpos;
    u8 mybyte;
};

void writer_init(struct writer_t*w, u8*data, int maxlength);
void writer_write(struct writer_t*w, void*data, int length);
void* writer_getpos(struct writer_t*w);
void writer_writebit(struct writer_t*w, int bit);
void writer_writebits(struct writer_t*w, u32 data, int bits);
void writer_writeu8(struct writer_t*w, u8 value);
void writer_writeu16(struct writer_t*w, u16 value);
void writer_resetbits(struct writer_t*w);

#endif //__bitio_h__
