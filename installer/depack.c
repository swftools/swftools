/* depack.c

   Part of the swftools installer.
   
   Copyright (c) 1997-2004 Matthias Kramm <kramm@quiss.org> 
 
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

#include "stdlib.h"
#include "stdio.h"
#include "depack.h"

U32 dbittab[] = {
    0x1,       0x2,       0x4,       0x8,
    0x10,      0x20,      0x40,      0x80,
    0x100,     0x200,     0x400,     0x800,
    0x1000,    0x2000,    0x4000,    0x8000,
    0x10000,   0x20000,   0x40000,   0x80000, 
    0x100000,  0x200000,  0x400000,  0x800000,
    0x1000000, 0x2000000, 0x4000000, 0x8000000,
    0x10000000,0x20000000,0x40000000,0x80000000
};

static void* malloc_z(int len)
{
    void* buf = malloc(len);
    if(buf == 0) {
	fprintf(stderr, "\nout of memory\n");
	exit(1);
    }
    memset(buf, 0, len);
    return buf;
}

typedef struct _dpackead {
    int back;
    int len;
} dpackead_t;

typedef struct _tree {   
    int depth;
    U8  bits[17];
    U32 base[17];
} tree_t;

typedef struct _depack_internal
{
    int DMEMSIZE;
    int DMEMSIZEA;

    tree_t T_REP;
    tree_t T_STORE;
    tree_t T_CNUM;
    tree_t T_BACK;
    tree_t T_BACK2;
    tree_t T_BACK3;

    dpackead_t dpackead;

    void*reader;
    readfunc_t readfunc;
    void*writer;
    writefunc_t writefunc;

    U8* mem;
    int mempos;
    int pos;
    U8 c;
    U32 b;
} depack_internal_t;

void depack_destroy(depack_t*d)
{
    depack_internal_t*i = (depack_internal_t*)d->internal;
    free(i->mem);i->mem=0;
    free(d->internal);d->internal=0;i=0;
}

static readbytes(depack_t*d, int num)
{
    depack_internal_t*i = (depack_internal_t*)d->internal;

    if(i->mempos+num<=i->DMEMSIZE) {
	i->readfunc(i->reader, &i->mem[i->mempos],num);
	i->writefunc(i->writer, &i->mem[i->mempos],num);
    } else {
	i->readfunc(i->reader, &i->mem[i->mempos],i->DMEMSIZE - i->mempos);
	i->writefunc(i->writer, &i->mem[i->mempos],i->DMEMSIZE - i->mempos);
	i->readfunc(i->reader, &i->mem[0],num-(i->DMEMSIZE - i->mempos));
	i->writefunc(i->writer, &i->mem[0],num-(i->DMEMSIZE - i->mempos));
    }
    i->mempos=(i->mempos+num)&i->DMEMSIZEA;
    i->pos+=num;
}

int move(U8*dest, U8*src, int len)
{
    if(len) do {
	*dest=*src;
	src++;
	dest++;
    } while(--len);
}

static lookback(depack_t*d, int back,int len)
{
    depack_internal_t*i = (depack_internal_t*)d->internal;

    int x,z=(i->mempos-back)&i->DMEMSIZEA;
    char fail=0;

    if(i->mempos+len <= i->DMEMSIZE && 
       z+len <= i->DMEMSIZE) 
    {
	move(&i->mem[i->mempos],&i->mem[z],len);
	i->writefunc(i->writer, &i->mem[i->mempos],len);
	i->mempos=(i->mempos+len)&i->DMEMSIZEA;
    } else {
	for(x=0;x<len;x++) {
	    i->mem[i->mempos]=i->mem[z];
	    i->writefunc(i->writer, &i->mem[i->mempos],1);
	    i->mempos=(i->mempos+1)&i->DMEMSIZEA;
	    z=(z+1)&i->DMEMSIZEA;
	}
    }
    i->pos += len;
}

static inline U8 readbit(depack_internal_t*i)
{
    i->c&=31;
    if(i->c==0) i->readfunc(i->reader, &i->b,4);
    return (i->b>>(31-i->c++))&1;
}

static inline U32 readval(depack_internal_t*i,int b)
{
    unsigned v=0;
    signed char l;
    for(l=b-1;l>=0;l--)
	v|=readbit(i)<<l;
    return v;
}

static tree_t gettree(depack_t*d)
{
    depack_internal_t*i = (depack_internal_t*)d->internal;

    int l;
    int x,y,z=0,b=0,r;
    tree_t tree;
    memset(&tree, 0, sizeof(tree));
   
    tree.depth = readval(i,4)+1;

    for(x=0;x<tree.depth;x++)
    {
	tree.base[x]=z;
	r=0;while(!readbit(i)) r++;
	if(readbit(i)) r=-r;
	b+=r;
	tree.bits[x]=b;
	z+=dbittab[b];
    }
    tree.base[tree.depth]=z;
    tree.bits[tree.depth]=0;
    return tree;

/*    for(z=0;z<=l;z++)
    {
	if(i->ttree[n].bits[z]>16) 
	    printf(".");
	else                       
	    printf("%c","0123456789abcdefg"[ttree[n].bits[z]]);
    }
    printf("\n");*/
}

static void showtree(tree_t tree)
{
    int t;
    int last=0;
    int lastbits = 0;
    for(t=0;t<=tree.depth;t++)
    {
	if(t>0)
	    printf("[%d] %d: %05x-%05x (%d=%d+%d+1)\n",t,lastbits, last,tree.base[t],lastbits+(t-1)+1, lastbits,t-1);
	last=tree.base[t];
	lastbits = tree.bits[t];
    }
}

static int gettreeval(depack_t*d, tree_t*tree)
{
    depack_internal_t*i = (depack_internal_t*)d->internal;

    U8 x=0;
    while(!readbit(i)) x++;
    if(x<17) return tree->base[x]+readval(i, tree->bits[x]);
    else     return -1;
}

static int get_memsize(int b)
{
    int t,c=1;
    for(t=0;;t++) {
	if(c>=b) return c;
	c<<=1;
    }
    return 0;
}

void depack_init(depack_t*d, void*reader, readfunc_t readfunc)
{
    d->internal = malloc_z(sizeof(depack_internal_t));
    depack_internal_t*i = (depack_internal_t*)d->internal;
    if(i==0) {
	fprintf(stderr, "depacker not initialized yet"); exit(1);
    }

    i->reader = reader;
    i->readfunc = readfunc;

    i->mempos=0;
    i->pos=0;
    i->b=i->c=0;

    i->readfunc(i->reader,&i->dpackead.back, 4);
    i->readfunc(i->reader,&i->dpackead.len, 4);

    i->DMEMSIZE = get_memsize(i->dpackead.len);
    i->DMEMSIZEA = i->DMEMSIZE-1;
    i->mem = malloc_z(i->DMEMSIZE);

    i->T_REP = gettree(d);
    i->T_STORE = gettree(d);
    i->T_CNUM = gettree(d);
    //showtree(i->T_CNUM);
    i->T_BACK = gettree(d);
    //showtree(i->T_BACK);
    i->T_BACK2 = gettree(d);
    //showtree(i->T_BACK);
    i->T_BACK3 = gettree(d);
    //showtree(i->T_BACK3);

    d->size = i->dpackead.len;
}

void depack_process(depack_t*d, void*writer,writefunc_t writefunc)
{
    depack_internal_t*i = (depack_internal_t*)d->internal;
    i->writer = writer;
    i->writefunc = writefunc;
    while(i->pos<i->dpackead.len)
    {
	int store,rep,x;

	store=gettreeval(d,&i->T_STORE)+1;

	readbytes(d,store);

	if(i->pos<i->dpackead.len)
	{
	    rep = gettreeval(d,&i->T_REP)+1;

	    for(x=0;x<rep;x++)
	    {
		int back,len;
		len = gettreeval(d,&i->T_CNUM)+1;

		if(len == 1) back = readval(i,3)+1;
		if(len == 2) back = gettreeval(d,&i->T_BACK2)+1;
		if(len == 3) back = gettreeval(d,&i->T_BACK3)+1;
		if(len >= 4) back = gettreeval(d,&i->T_BACK)+1;

		lookback(d,back,len);
	    }
	}
    }

    if(i->pos!=i->dpackead.len) {
	fprintf(stderr, "Internal error");
	exit(1);
    }
}

/*

                mov eax,[b1]
                mov ebx,[b2]
                shld eax,ebx,cl
                mov [b1],eax
                shr ebx,cl
                mov [b2],ebx
                sub cnt,cl
                cmp cnt,32-8
                ja jj
mm:             xor eax,eax
                call read
                shl eax,[cnt]
                or  [b2],eax
                add cnt,8
                cmp cnt,32-8
                jb mm
jj:

*/


