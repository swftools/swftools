/* bitio.c 
   Various routines for reading and writing bit- and bytewise, from and to memory.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include "bitio.h"


void reader_init(struct reader_t*r, uchar*newdata, int newlength)
{
    r->data = newdata;
    r->datalength = newlength;
    r->datapos = 0;
    reader_resetbits(r);
}
void reader_skip(struct reader_t*r, int length)
{
    r->datapos += length;
}
void reader_resetbits(struct reader_t*r)
{
    r->bitpos = 8;
}

void reader_input1(struct reader_t*r, void*target)
{
    *(uchar*)target = *(uchar*)&r->data[r->datapos];
    r->datapos ++;
}
void reader_input2(struct reader_t*r, void*target)
{
    *(u16*)target = GET16(&r->data[r->datapos]);
    r->datapos += 2;
}
void reader_input4(struct reader_t*r, void*target)
{
    *(u32*)target = GET32(&r->data[r->datapos]);
    r->datapos += 4;
}
uchar*reader_getinputpos(struct reader_t*r)
{
    return &r->data[r->datapos];
}
int reader_getinputlength(struct reader_t*r)
{
    return r->datalength;
}
void reader_setinputpos(struct reader_t*r, uchar*pos)
{
    r->datapos = pos-r->data;
}
u32 reader_readbit(struct reader_t*r)
{
    if(r->bitpos==8) 
    {
	r->bitpos=0;
        reader_input1(r, &r->mybyte);
    }
    return (r->mybyte>>(7-r->bitpos++))&1;
}
void reader_readbits(struct reader_t*r, u32*val,int num)
{
    int t;
    *val=0;
    for(t=0;t<num;t++)
    {
	*val<<=1;
	*val|=reader_readbit(r);
    }
}

void reader_readsbits(struct reader_t*r, s32*val,int num)
{
    u32 x;
    reader_readbits(r, &x, num);
    if((x>>(num-1))&1)
    {
	x|=(0xffffffff<<num);
    }
    *val=x;
}

u32 reader_getbits(struct reader_t*r, int num)
{
    u32 x;
    reader_readbits(r,&x,num);
    return x;
}

s32 reader_getsbits(struct reader_t*r, int num)
{
    s32 x;
    reader_readsbits(r, &x,num);
    return x;
}

u8 reader_readu8(struct reader_t*r)
{
    u8 a;
    reader_input1(r, &a);
    return a;
}

u16 reader_readu16(struct reader_t*r)
{
    u8 a,b;
    reader_input1(r, &a);
    reader_input1(r, &b);
    return ((u16)b)*256+a;
}

u32 reader_readu32(struct reader_t*r)
{
    u8 a,b,c,d;
    reader_input1(r, &a);
    reader_input1(r, &b);
    reader_input1(r, &c);
    reader_input1(r, &d);
    return (((((u32)d)*256+(u32)c)*256+(u32)b)*256+(u32)a);
}

void writer_init(struct writer_t*w, u8*data, int maxlength)
{
    w->data = data;
    w->maxlength = maxlength;
    w->pos = 0;
    w->bitpos = 0;
    w->mybyte = 0;
}

void writer_write(struct writer_t*w, void*data, int length)
{
    memcpy(&w->data[w->pos], data, length);
    w->pos += length;
}

void writer_writeu8(struct writer_t*w, u8 value)
{
    writer_resetbits(w);
    writer_write(w, &value, 1);
}

void writer_writeu16(struct writer_t*w, u16 value)
{
    u16 value2 = SWAP16(value);
    writer_resetbits(w);
    writer_write(w, &value2, 2);
}

void writer_writeu32(struct writer_t*w, u32 value)
{
    u16 value2 = SWAP32(value);
    writer_resetbits(w);
    writer_write(w, &value2, 4);
}

void* writer_getpos(struct writer_t*w)
{
    return &w->data[w->pos];
}

void writer_resetbits(struct writer_t*w)
{
    if(w->bitpos)
	writer_write(w, &w->mybyte, 1);
    w->bitpos = 0;
    w->mybyte = 0;
}

void writer_writebit(struct writer_t*w, int bit)
{    
    if(w->bitpos==8) 
    {
        writer_write(w, &w->mybyte, 1);
	w->bitpos = 0;
	w->mybyte = 0;
    }
    if(bit&1)
	w->mybyte |= 1 << (7 - w->bitpos);
    w->bitpos ++;
}

void writer_writebits(struct writer_t*w, u32 data, int bits)
{
    int t;
    for(t=0;t<bits;t++)
    {
	writer_writebit(w, (data >> (bits-t-1))&1);
    }
}

