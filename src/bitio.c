/* bitio.c 
   Various routines for reading and writing bit- and bytewise, from and to memory.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include "bitio.h"

static uchar*data;
static int datalength;
static int datapos;

void resetbits();

void reader_init(uchar*newdata, int newlength)
{
    data = newdata;
    datalength = newlength;
    datapos = 0;
    resetbits();
}
void skip(int length)
{
    datapos += length;
}
static u8 bitpos=8,mybyte;
static u8 bitmem=0;
void resetbits()
{
    bitpos=8;
}

void input1(void*target)
{
    *(uchar*)target = *(uchar*)&data[datapos];
    datapos ++;
}
void input2(void*target)
{
    *(unsigned short int*)target = *(unsigned short int*)&data[datapos];
    datapos += 2;
}
void input4(void*target)
{
    *(unsigned int*)target = *(unsigned int*)&data[datapos];
    datapos += 4;
}
uchar*getinputpos()
{
    return &data[datapos];
}
int getinputlength()
{
    return datalength;
}
void setinputpos(uchar*pos)
{
    datapos = pos-data;
}

u32 readbit()
{
    if(bitpos==8) 
    {
	bitpos=0;
        input1(&mybyte);
    }
    return (mybyte>>(7-bitpos++))&1;
}
void readbits(u32*val,int num)
{
    int t;
    *val=0;
    for(t=0;t<num;t++)
    {
	*val<<=1;
	*val|=readbit();
    }
}

void readsbits(s32*val,int num)
{
    u32 x;
    readbits(&x, num);
    if((x>>(num-1))&1)
    {
	x|=(0xffffffff<<num);
    }
    *(s32*)val=x;
}

u32 getbits(int num)
{
    u32 x;
    readbits(&x,num);
    return x;
}

s32 getsbits(int num)
{
    s32 x;
    readsbits(&x,num);
    return x;
}

u8 readu8()
{
    u8 a;
    input1(&a);
    return a;
}

u16 readu16()
{
    u8 a,b;
    input1(&a);
    input1(&b);
    return ((u16)b)*256+a;
}

u32 readu32()
{
    u8 a,b,c,d;
    input1(&a);
    input1(&b);
    input1(&c);
    input1(&d);
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
    writer_resetbits(w);
    writer_write(w, &value, 2);
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

