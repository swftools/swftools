/* vi: set sts=2 sw=2 :*/
/* rfxswf.c 

   Library for creating and reading SWF files or parts of it.
   There's a module directory which provides some extended functionality.
   Most modules are included at the bottom of this file.

   Part of the swftools package.

   Copyright (c) 2000-2003 Rainer Böhme <rfxswf@reflex-studio.de>
   Copyright (c) 2003 Matthias Kramm <kramm@quiss.org> 

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

#include "mem.h"
#include "rfxswf.h"
#ifdef HAVE_ZLIB
#include <zlib.h>
#endif // HAVE_ZLIB

#ifndef RFXSWF_DISABLESOUND
#ifdef HAVE_LAME
#include "lame/lame.h"
#endif
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifdef HAVE_IO_H
#include <io.h>
#endif

#include "./bitio.h"
#include "./MD5.h"

// internal constants

#define MALLOC_SIZE     128
#define INSERT_RFX_TAG

#define MEMSIZE(l) (((l/MALLOC_SIZE)+1)*MALLOC_SIZE)

// inline wrapper functions

TAG * swf_NextTag(TAG * t) { return t->next; }
TAG * swf_PrevTag(TAG * t) { return t->prev; }
U16   swf_GetTagID(TAG * t)    { return t->id; }
U32   swf_GetTagLen(TAG * t) { return t->len; }
U8*   swf_GetTagLenPtr(TAG * t) { return &(t->data[t->len]); }
U32   swf_GetTagPos(TAG * t)   { return t->pos; }

void swf_SetTagPos(TAG * t,U32 pos)
{ swf_ResetReadBits(t);
  if (pos<=t->len) t->pos = pos;
  else { 
#ifdef DEBUG_RFXSWF
    fprintf(stderr,"SetTagPos(%d) out of bounds: TagID = %i\n",pos, t->id);
#endif
  }
}

char* swf_GetString(TAG*t)
{
    int pos = t->pos;
    while(t->pos < t->len && swf_GetU8(t));
    /* make sure we always have a trailing zero byte */
    if(t->pos == t->len) {
      if(t->len == t->memsize) {
	swf_ResetWriteBits(t);
	swf_SetU8(t, 0);
	t->len = t->pos;
      }
      t->data[t->len] = 0;
    }
    return (char*)&(t->data[pos]);
}

U8 swf_GetU8(TAG * t)
{ swf_ResetReadBits(t);
  #ifdef DEBUG_RFXSWF
    if (t->pos>=t->len) 
    { fprintf(stderr,"GetU8() out of bounds: TagID = %i\n",t->id);
      return 0;
    }
  #endif
  return t->data[t->pos++];
}

U16 swf_GetU16(TAG * t)
{ U16 res;
  swf_ResetReadBits(t);
  #ifdef DEBUG_RFXSWF
    if (t->pos>(t->len-2)) 
    { fprintf(stderr,"GetU16() out of bounds: TagID = %i\n",t->id);
      return 0;
    }
  #endif
  res = t->data[t->pos] | (t->data[t->pos+1]<<8);
  t->pos+=2;
  return res;
}

U32 swf_GetU32(TAG * t)
{ U32 res;
  swf_ResetReadBits(t);
  #ifdef DEBUG_RFXSWF
    if (t->pos>(t->len-4)) 
    { fprintf(stderr,"GetU32() out of bounds: TagID = %i\n",t->id);
      return 0;
    }
  #endif
  res = t->data[t->pos]        | (t->data[t->pos+1]<<8) | 
       (t->data[t->pos+2]<<16) | (t->data[t->pos+3]<<24);
  t->pos+=4;
  return res;
}

int swf_GetBlock(TAG * t,U8 * b,int l)
// returns number of bytes written (<=l)
// b = NULL -> skip data
{ swf_ResetReadBits(t);
  if ((t->len-t->pos)<l) l=t->len-t->pos;
  if (b && l) memcpy(b,&t->data[t->pos],l);
  t->pos+=l;
  return l;
}

int swf_SetBlock(TAG * t,const U8 * b,int l)
// Appends Block to the end of Tagdata, returns size
{ U32 newlen = t->len + l;
  swf_ResetWriteBits(t);
  if (newlen>t->memsize)
  { U32  newmem  = MEMSIZE(newlen);  
    U8 * newdata = (U8*)(rfx_realloc(t->data,newmem));
    t->memsize = newmem;
    t->data    = newdata;
  }
  if (b) memcpy(&t->data[t->len],b,l);
  else memset(&t->data[t->len],0x00,l);
  t->len+=l;
  return l;
}

int swf_SetU8(TAG * t,U8 v)
{ swf_ResetWriteBits(t);
  if ((t->len+1)>t->memsize) return (swf_SetBlock(t,&v,1)==1)?0:-1;
  t->data[t->len++] = v;
  return 0;
}

int swf_SetU16(TAG * t,U16 v)
{ U8 a[2];
  a[0] = v&0xff;
  a[1] = v>>8;
  
  swf_ResetWriteBits(t);
  if ((t->len+2)>t->memsize) return (swf_SetBlock(t,a,2)==2)?0:-1;
  t->data[t->len++] = a[0];
  t->data[t->len++] = a[1];
  return 0;
}
void swf_SetS16(TAG * t,int v)
{
    if(v>32767 || v<-32768) {
	fprintf(stderr, "Warning: S16 overflow: %d\n", v);
    }
    swf_SetU16(t, (S16)v);
}

int swf_SetU32(TAG * t,U32 v)
{ U8 a[4];
  a[0] = v&0xff;        // to ensure correct handling of non-intel byteorder
  a[1] = (v>>8)&0xff;
  a[2] = (v>>16)&0xff;
  a[3] = (v>>24)&0xff;
  
  swf_ResetWriteBits(t);
  if ((t->len+4)>t->memsize) return (swf_SetBlock(t,a,4)==4)?0:-1;
  t->data[t->len++] = a[0];
  t->data[t->len++] = a[1];
  t->data[t->len++] = a[2];
  t->data[t->len++] = a[3];
  return 0;
}

U32 swf_GetBits(TAG * t,int nbits)
{ U32 res = 0;
  if (!nbits) return 0;
  if (!t->readBit) t->readBit = 0x80;
  while (nbits)
  { res<<=1;
    if (t->data[t->pos]&t->readBit) res|=1;
    t->readBit>>=1;
    nbits--;
    if (!t->readBit)
    { if (nbits) t->readBit = 0x80;
      #ifdef DEBUG_RFXSWF
      if (t->pos>=t->len) 
      { fprintf(stderr,"GetBits() out of bounds: TagID = %i\n",t->id);
        return res;
      }
      #endif
      t->pos++;
    }
  }
  return res;
}

S32 swf_GetSBits(TAG * t,int nbits)
{ U32 res = swf_GetBits(t,nbits);
  if (res&(1<<(nbits-1))) res|=(0xffffffff<<nbits);  
  return (S32)res;
}

U32 reader_GetBits(reader_t*reader, int nbits)
{ return reader_readbits(reader, nbits);
}
S32 reader_GetSBits(reader_t*reader, int nbits)
{ U32 res = reader_readbits(reader, nbits);
  if (res&(1<<(nbits-1))) res|=(0xffffffff<<nbits);  
  return (S32)res;
}

int swf_SetBits(TAG * t,U32 v,int nbits)
{ U32 bm = 1<<(nbits-1);

  while (nbits)
  { if (!t->writeBit)
    { if (FAILED(swf_SetU8(t,0))) return -1;
      t->writeBit = 0x80;
    }
    if (v&bm) t->data[t->len-1] |= t->writeBit;
    bm>>=1;
    t->writeBit>>=1;
    nbits--;
  }
  return 0;
}

// Advanced Data Access Functions

double swf_GetFixed(TAG * t)
{
  U16 low =  swf_GetU16(t);
  U16 high = swf_GetU16(t);
  return high + low*(1/65536.0);
}
void swf_SetFixed(TAG * t, double f)
{
  U16 fr = (U16)((f-(int)f)*65536);
  swf_SetU16(t, fr);
  swf_SetU16(t, (U16)f - (f<0 && fr!=0));
}
float swf_GetFixed8(TAG * t)
{
  U8 low =  swf_GetU8(t);
  U8 high = swf_GetU8(t);
  return (float)(high + low*(1/256.0));
}
void swf_SetFixed8(TAG * t, float f)
{
  U8 fr = (U8)((f-(int)f)*256);
  swf_SetU8(t, fr);
  swf_SetU8(t, (U8)f - (f<0 && fr!=0));
}

int swf_SetRGB(TAG * t,RGBA * col)
{ if (!t) return -1;
  if (col)
  { swf_SetU8(t,col->r);
    swf_SetU8(t,col->g);
    swf_SetU8(t,col->b);
  } else swf_SetBlock(t,NULL,3);
  return 0;
}
void swf_GetRGB(TAG * t, RGBA * col)
{
    RGBA dummy;
    if(!col)
	col = &dummy;
    col->r = swf_GetU8(t);
    col->g = swf_GetU8(t);
    col->b = swf_GetU8(t);
    col->a = 255;
}

int swf_SetRGBA(TAG * t,RGBA * col)
{ if (!t) return -1;
  if (col)
  { swf_SetU8(t,col->r);
    swf_SetU8(t,col->g);
    swf_SetU8(t,col->b);
    swf_SetU8(t,col->a);
  } else swf_SetBlock(t,NULL,4);
  return 0;
}
void swf_GetRGBA(TAG * t, RGBA * col)
{
    RGBA dummy;
    if(!col)
	col = &dummy;
    col->r = swf_GetU8(t);
    col->g = swf_GetU8(t);
    col->b = swf_GetU8(t);
    col->a = swf_GetU8(t);
}

void swf_GetGradient(TAG * tag, GRADIENT * gradient, char alpha)
{
    int t;
    if(!tag) {
      memset(gradient, 0, sizeof(GRADIENT));
      return;
    }
    U8 num = swf_GetU8(tag) & 15;
    if(gradient) {
	gradient->num = num;
	gradient->rgba = (RGBA*)rfx_calloc(sizeof(RGBA)*gradient->num);
	gradient->ratios = (U8*)rfx_calloc(sizeof(gradient->ratios[0])*gradient->num);
    }
    for(t=0;t<num;t++)
    {
	U8 ratio = swf_GetU8(tag);
	RGBA color;
	if(!alpha)
	    swf_GetRGB(tag, &color);
	else
	    swf_GetRGBA(tag, &color);
	if(gradient) {
	  gradient->ratios[t] = ratio;
	  gradient->rgba[t] = color;
	}
    }
}

void swf_SetGradient(TAG * tag, GRADIENT * gradient, char alpha)
{
    int t;
    if(!tag) {
      memset(gradient, 0, sizeof(GRADIENT));
      return;
    }
    swf_SetU8(tag, gradient->num);
    for(t=0; t<8 && t<gradient->num; t++)
    {
	swf_SetU8(tag, gradient->ratios[t]);
	if(!alpha)
	    swf_SetRGB(tag, &gradient->rgba[t]);
	else
	    swf_SetRGBA(tag, &gradient->rgba[t]);
    }
}

void swf_FreeGradient(GRADIENT* gradient)
{
  if(gradient->ratios)
    rfx_free(gradient->ratios);
  if(gradient->rgba)
    rfx_free(gradient->rgba);
  memset(gradient, 0, sizeof(GRADIENT));
}

int swf_CountUBits(U32 v,int nbits)
{ int n = 32;
  U32 m = 0x80000000;
  if(v == 0x00000000) n = 0; 
  else
    while (!(v&m))
    { n--;
      m>>=1;
    } 
  return (n>nbits)?n:nbits;
}

int swf_CountBits(U32 v,int nbits)
{ int n = 33;
  U32 m = 0x80000000;
  if (v&m)
  { if(v == 0xffffffff) n = 1;
    else 
    while (v&m)
    { n--;
      m>>=1;
    } 
  }
  else
  { if(v == 0x00000000) n = 0; 
    else
    while (!(v&m))
    { n--;
      m>>=1;
    } 
  }
  return (n>nbits)?n:nbits;
}

int swf_GetRect(TAG * t,SRECT * r)
{ int nbits;
  SRECT dummy;
  if(!t) {r->xmin=r->xmax=r->ymin=r->ymax=0;return 0;}
  if (!r) r = &dummy;
  nbits = (int) swf_GetBits(t,5);
  r->xmin = swf_GetSBits(t,nbits);
  r->xmax = swf_GetSBits(t,nbits);
  r->ymin = swf_GetSBits(t,nbits);
  r->ymax = swf_GetSBits(t,nbits);
  return 0;
}

int reader_GetRect(reader_t*reader,SRECT * r)
{ int nbits;
  SRECT dummy;
  if (!r) r = &dummy;
  nbits = (int) reader_GetBits(reader,5);
  r->xmin = reader_GetSBits(reader,nbits);
  r->xmax = reader_GetSBits(reader,nbits);
  r->ymin = reader_GetSBits(reader,nbits);
  r->ymax = reader_GetSBits(reader,nbits);
  return 0;
}

int swf_SetRect(TAG * t,SRECT * r)
{ int nbits;
    
  nbits = swf_CountBits(r->xmin,0);
  nbits = swf_CountBits(r->xmax,nbits);
  nbits = swf_CountBits(r->ymin,nbits);
  nbits = swf_CountBits(r->ymax,nbits);
  if(nbits>=32) {
    fprintf(stderr, "rfxswf: Warning: num_bits overflow in swf_SetRect\n");
    nbits=31;
  }

  swf_SetBits(t,nbits,5);
  swf_SetBits(t,r->xmin,nbits);
  swf_SetBits(t,r->xmax,nbits);
  swf_SetBits(t,r->ymin,nbits);
  swf_SetBits(t,r->ymax,nbits);

  return 0;
}

SRECT swf_ClipRect(SRECT border, SRECT r)
{
    if(r.xmax > border.xmax) r.xmax = border.xmax;
    if(r.ymax > border.ymax) r.ymax = border.ymax;
    if(r.xmax < border.xmin) r.xmax = border.xmin;
    if(r.ymax < border.ymin) r.ymax = border.ymin;
    
    if(r.xmin > border.xmax) r.xmin = border.xmax;
    if(r.ymin > border.ymax) r.ymin = border.ymax;
    if(r.xmin < border.xmin) r.xmin = border.xmin;
    if(r.ymin < border.ymin) r.ymin = border.ymin;
    return r;
}

void swf_ExpandRect(SRECT*src, SPOINT add)
{
    if((src->xmin | src->ymin | src->xmax | src->ymax)==0) {
	src->xmin = add.x;
	src->ymin = add.y;
	src->xmax = add.x;
	src->ymax = add.y;
	if((add.x|add.y) == 0) src->xmax++; //make sure the bbox is not NULL anymore
	return;
    }
    if(add.x < src->xmin)
	src->xmin = add.x;
    if(add.x > src->xmax)
	src->xmax = add.x;
    if(add.y < src->ymin)
	src->ymin = add.y;
    if(add.y > src->ymax)
	src->ymax = add.y;
}
void swf_ExpandRect2(SRECT*src, SRECT*add)
{
    if((add->xmin | add->ymin | add->xmax | add->ymax)==0)
	return;
    if((src->xmin | src->ymin | src->xmax | src->ymax)==0)
	*src = *add;
    if(add->xmin < src->xmin)
	src->xmin = add->xmin;
    if(add->ymin < src->ymin)
	src->ymin = add->ymin;
    if(add->xmax > src->xmax)
	src->xmax = add->xmax;
    if(add->ymax > src->ymax)
	src->ymax = add->ymax;
}
void swf_ExpandRect3(SRECT*src, SPOINT center, int radius)
{
    if((src->xmin | src->ymin | src->xmax | src->ymax)==0) {
	src->xmin = center.x-radius;
	src->ymin = center.y-radius;
	src->xmax = center.x+radius;
	src->ymax = center.y+radius;
	if((center.x|center.y|radius) == 0) src->xmax++; //make sure the bbox is not NULL anymore
	return;
    }
    if(center.x - radius < src->xmin)
	src->xmin = center.x - radius;
    if(center.x + radius > src->xmax)
	src->xmax = center.x + radius;
    if(center.y - radius < src->ymin)
	src->ymin = center.y - radius;
    if(center.y + radius > src->ymax)
	src->ymax = center.y + radius;
}
SPOINT swf_TurnPoint(SPOINT p, MATRIX* m)
{
    SPOINT r;
    r.x = (int)(m->sx*(1/65536.0)*p.x + m->r1*(1/65536.0)*p.y + 0.5) + m->tx;
    r.y = (int)(m->r0*(1/65536.0)*p.x + m->sy*(1/65536.0)*p.y + 0.5) + m->ty;
    return r;
}
SRECT swf_TurnRect(SRECT r, MATRIX* m)
{
    SRECT g;
    SPOINT p1,p2,p3,p4,pp1,pp2,pp3,pp4;
    if(!m)
      return r;
    p1.x = r.xmin;p1.y = r.ymin;
    p2.x = r.xmax;p2.y = r.ymin;
    p3.x = r.xmin;p3.y = r.ymax;
    p4.x = r.xmax;p4.y = r.ymax;
    pp1 = swf_TurnPoint(p1, m);
    pp2 = swf_TurnPoint(p2, m);
    pp3 = swf_TurnPoint(p3, m);
    pp4 = swf_TurnPoint(p4, m);
    g.xmin = g.xmax = pp1.x;
    g.ymin = g.ymax = pp1.y;
    swf_ExpandRect(&g, pp2);
    swf_ExpandRect(&g, pp3);
    swf_ExpandRect(&g, pp4);
    return g;
}
	

int swf_GetMatrix(TAG * t,MATRIX * m)
{ MATRIX dummy;
  int nbits;
    
  if (!m) m = &dummy;
  
  if (!t)
  { m->sx = m->sy = 0x10000;
    m->r0 = m->r1 = 0;
    m->tx = m->ty = 0;
    return -1;
  }

  swf_ResetReadBits(t);
  
  if (swf_GetBits(t,1))
  { nbits = swf_GetBits(t,5);
    m->sx = swf_GetSBits(t,nbits);
    m->sy = swf_GetSBits(t,nbits);
  }
  else m->sx = m->sy = 0x10000;
  
  if (swf_GetBits(t,1))
  { nbits = swf_GetBits(t,5);
    m->r0 = swf_GetSBits(t,nbits);
    m->r1 = swf_GetSBits(t,nbits);
  }
  else m->r0 = m->r1 = 0x0;

  nbits = swf_GetBits(t,5);
  m->tx = swf_GetSBits(t,nbits);
  m->ty = swf_GetSBits(t,nbits);
  
  return 0;
}

int swf_SetMatrix(TAG * t,MATRIX * m)
{ int nbits;
  MATRIX ma;

  if (!m)
  { m = &ma;
    ma.sx = ma.sy = 0x10000;
    ma.r0 = ma.r1 = 0;
    ma.tx = ma.ty = 0;
  }

  swf_ResetWriteBits(t);

  if ((m->sx==0x10000)&&(m->sy==0x10000)) swf_SetBits(t,0,1);
  else
  { swf_SetBits(t,1,1);
    nbits = swf_CountBits(m->sx,0);
    nbits = swf_CountBits(m->sy,nbits);
    if(nbits>=32) {
        /* TODO: happens on AMD64 systems for normal values? */
	fprintf(stderr,"rfxswf: Error: matrix values too large\n");
	nbits = 31;
    }
    swf_SetBits(t,nbits,5);
    swf_SetBits(t,m->sx,nbits);
    swf_SetBits(t,m->sy,nbits);
  }

  if ((!m->r0)&&(!m->r1)) swf_SetBits(t,0,1);
  else
  { swf_SetBits(t,1,1);
    nbits = swf_CountBits(m->r0,0);
    nbits = swf_CountBits(m->r1,nbits);
    if(nbits>=32) {
	fprintf(stderr,"rfxswf: Error: matrix values too large\n");
	nbits = 31;
    }
    swf_SetBits(t,nbits,5);
    swf_SetBits(t,m->r0,nbits);
    swf_SetBits(t,m->r1,nbits);
  }

  nbits = swf_CountBits(m->tx,0);
  nbits = swf_CountBits(m->ty,nbits);
  if(nbits>=32) {
      fprintf(stderr,"rfxswf: Error: matrix values too large\n");
      nbits = 31;
  }
  swf_SetBits(t,nbits,5);
  swf_SetBits(t,m->tx,nbits);
  swf_SetBits(t,m->ty,nbits);

  return 0;
}

int swf_GetCXForm(TAG * t,CXFORM * cx,U8 alpha)
{ CXFORM cxf;
  int hasadd;
  int hasmul;
  int nbits;
    
  if (!cx) cx = &cxf;
  
  cx->a0 = cx->r0 = cx->g0 = cx->b0 = 256;
  cx->a1 = cx->r1 = cx->g1 = cx->b1 = 0;

  if (!t) return 0;
  
  swf_ResetReadBits(t);
  hasadd = swf_GetBits(t,1);
  hasmul = swf_GetBits(t,1);
  nbits  = swf_GetBits(t,4);

  if (hasmul)
  { cx->r0 = (S16)swf_GetSBits(t,nbits);
    cx->g0 = (S16)swf_GetSBits(t,nbits);
    cx->b0 = (S16)swf_GetSBits(t,nbits);
    if (alpha)
      cx->a0 = (S16)swf_GetSBits(t,nbits);
  }

  if (hasadd)
  { cx->r1 = (S16)swf_GetSBits(t,nbits);
    cx->g1 = (S16)swf_GetSBits(t,nbits);
    cx->b1 = (S16)swf_GetSBits(t,nbits);
    if (alpha)
      cx->a1 = (S16)swf_GetSBits(t,nbits);
  }
  
  return 0;
}

int swf_SetCXForm(TAG * t,CXFORM * cx,U8 alpha)
{ CXFORM cxf;
  int hasadd;
  int hasmul;
  int nbits;
    
  if (!cx)
  { cx = &cxf;
    cx->a0 = cx->r0 = cx->g0 = cx->b0 = 256;
    cx->a1 = cx->r1 = cx->g1 = cx->b1 = 0;
  }

  if (!alpha)
  { cx->a0 = 256;
    cx->a1 = 0;
  }

  nbits = 0;

  hasmul = (cx->a0!=256)||(cx->r0!=256)||(cx->g0!=256)||(cx->b0!=256);
  hasadd = cx->a1|cx->r1|cx->g1|cx->b1;

  if (hasmul)
  { if (alpha) nbits = swf_CountBits((S32)cx->a0,nbits);
    nbits = swf_CountBits((S32)cx->r0,nbits);
    nbits = swf_CountBits((S32)cx->g0,nbits);
    nbits = swf_CountBits((S32)cx->b0,nbits);
  }

  if (hasadd)
  { if (alpha) nbits = swf_CountBits((S32)cx->a1,nbits);
    nbits = swf_CountBits((S32)cx->r1,nbits);
    nbits = swf_CountBits((S32)cx->g1,nbits);
    nbits = swf_CountBits((S32)cx->b1,nbits);
  }
  
  swf_ResetWriteBits(t);
  swf_SetBits(t,hasadd?1:0,1);
  swf_SetBits(t,hasmul?1:0,1);
  swf_SetBits(t,nbits,4);

  if (hasmul)
  { swf_SetBits(t,cx->r0,nbits);
    swf_SetBits(t,cx->g0,nbits);
    swf_SetBits(t,cx->b0,nbits);
    if (alpha) swf_SetBits(t,cx->a0,nbits);
  }

  if (hasadd)
  { swf_SetBits(t,cx->r1,nbits);
    swf_SetBits(t,cx->g1,nbits);
    swf_SetBits(t,cx->b1,nbits);
    if (alpha) swf_SetBits(t,cx->a1,nbits);
  }
  
  return 0;
}

//int swf_GetPoint(TAG * t,SPOINT * p) { return 0; }
//int swf_SetPoint(TAG * t,SPOINT * p) { return 0; }

void  swf_SetPassword(TAG * t, const char * password)
{
    /* WARNING: crypt_md5 is not reentrant */
    char salt[3];
    char* md5string;

#if defined(HAVE_LRAND48) && defined(HAVE_SRAND48) && defined(HAVE_TIME_H) && defined(HAVE_TIME)
    srand48(time(0));
    salt[0] = "abcdefghijklmnopqrstuvwxyz0123456789"[lrand48()%36];
    salt[1] = "abcdefghijklmnopqrstuvwxyz0123456789"[lrand48()%36];
#else
    salt[0] = 'l';
    salt[1] = '8';
    fprintf(stderr, "rfxswf: Warning- no usable random generator found\n");
    fprintf(stderr, "Your password will be vulnerable to dictionary attacks\n");
#endif
    salt[2] = 0;
    
    md5string = crypt_md5(password, salt);

    swf_SetU16(t,0);
    swf_SetString(t, (U8*)md5string);
}

int swf_VerifyPassword(TAG * t, const char * password)
{
    char*md5string1, *md5string2;
    char*x;
    char*salt;
    int n;

    if(t->len >= 5 && t->pos==0 && 
       t->data[0] == 0 &&
       t->data[1] == 0) {
      swf_GetU16(t);
    } else {
      printf("%d %d %d %d\n", t->len, t->pos, t->data[0], t->data[1]);
    }

    md5string1 = swf_GetString(t);

    if(strncmp(md5string1, "$1$",3 )) {
        fprintf(stderr, "rfxswf: no salt in pw string\n");
	return 0;
    }
    x = strchr(md5string1+3, '$');
    if(!x) {
        fprintf(stderr, "rfxswf: invalid salt format in pw string\n");
	return 0;
    }
    n = x-(md5string1+3);
    salt = (char*)rfx_alloc(n+1);
    memcpy(salt, md5string1+3, n);
    salt[n] = 0;

    md5string2 = crypt_md5(password, salt);
    rfx_free(salt);
    if(strcmp(md5string1, md5string2) != 0)
	return 0;
    return 1;
}

// Tag List Manipulating Functions

TAG * swf_InsertTag(TAG * after,U16 id)
{ TAG * t;

  t = (TAG *)rfx_calloc(sizeof(TAG));
  t->id = id;
  
  if (after)
  {
    t->prev  = after;
    t->next  = after->next;
    after->next = t;
    if (t->next) t->next->prev = t;
  }
  return t;
}

TAG * swf_InsertTagBefore(SWF* swf, TAG * before,U16 id)
{ TAG * t;

  t = (TAG *)rfx_calloc(sizeof(TAG));
  t->id = id;
  
  if (before)
  {
    t->next  = before;
    t->prev  = before->prev;
    before->prev = t;
    if (t->prev) t->prev->next = t;
  }
  if(swf && swf->firstTag == before) {
    swf->firstTag = t;
  }
  return t;
}

void swf_ClearTag(TAG * t)
{
  if (t->data) rfx_free(t->data);
  t->data = 0;
  t->pos = 0;
  t->len = 0;
  t->readBit = 0;
  t->writeBit = 0;
  t->memsize = 0;
}

void swf_ResetTag(TAG*tag, U16 id)
{
    tag->len = tag->pos = tag->readBit = tag->writeBit = 0;
    tag->id = id;
}

TAG* swf_CopyTag(TAG*tag, TAG*to_copy)
{
    tag = swf_InsertTag(tag, to_copy->id);
    swf_SetBlock(tag, to_copy->data, to_copy->len);
    return tag;
}

TAG* swf_DeleteTag(SWF*swf, TAG * t)
{
  TAG*next = t->next;

  if (swf && swf->firstTag==t) 
    swf->firstTag = t->next;
  if (t->prev) t->prev->next = t->next;
  if (t->next) t->next->prev = t->prev;

  if (t->data) rfx_free(t->data);
  rfx_free(t);
  return next;
}

TAG * swf_ReadTag(reader_t*reader, TAG * prev)
{ TAG * t;
  U16 raw;
  U32 len;
  int id;

  if (reader->read(reader, &raw, 2) !=2 ) return NULL;
  raw = SWAP16(raw);

  len = raw&0x3f;
  id  = raw>>6;

  if (len==0x3f)
  {
      if (reader->read(reader, &len, 4) != 4) return NULL;
      len = SWAP32(len);
  }

  if (id==ST_DEFINESPRITE) len = 2*sizeof(U16);
  // Sprite handling fix: Flatten sprite tree

  t = (TAG *)rfx_calloc(sizeof(TAG));
  
  t->len = len;
  t->id  = id;

  if (t->len)
  { t->data = (U8*)rfx_alloc(t->len);
    t->memsize = t->len;
    if (reader->read(reader, t->data, t->len) != t->len) {
      fprintf(stderr, "rfxswf: Warning: Short read (tagid %d). File truncated?\n", t->id);
      free(t->data);t->data=0;
      free(t);
      return NULL;
    }
  }

  if (prev)
  {
    t->prev  = prev;
    prev->next = t;
  }

  return t;
}

int swf_DefineSprite_GetRealSize(TAG * t);

int swf_WriteTag2(writer_t*writer, TAG * t)
// returns tag length in bytes (incl. Header), -1 = Error
// writer = 0 -> no output
{ U16 raw[3];
  U32 len;
  int short_tag;

  if (!t) return -1;

  len = (t->id==ST_DEFINESPRITE)?swf_DefineSprite_GetRealSize(t):t->len;

  short_tag = len<0x3f&&
    (t->id!=ST_DEFINEBITSLOSSLESS&&t->id!=ST_DEFINEBITSLOSSLESS2&&t->id!=ST_SOUNDSTREAMBLOCK&&
     t->id!=ST_DEFINEBITSJPEG&&t->id!=ST_DEFINEBITSJPEG2&&t->id!=ST_DEFINEBITSJPEG3);

  if (writer)
  {
#ifdef MEASURE
  int oldpos = writer->pos;
#endif
    
    if (short_tag)
    { raw[0] = SWAP16(len|((t->id&0x3ff)<<6));
      if (writer->write(writer,raw,2)!=2)
      {
        #ifdef DEBUG_RFXSWF
          fprintf(stderr,"WriteTag() failed: Short Header.\n");
        #endif
        return -1;
      }
    }
    else
    {
      raw[0] = SWAP16((t->id<<6)|0x3f);
      if (writer->write(writer,raw,2)!=2)
      {
#ifdef DEBUG_RFXSWF
          fprintf(stderr,"WriteTag() failed: Long Header (1).\n");
#endif
	  return -1;
      }
      
      len = SWAP32(len);
      if (writer->write(writer,&len,4)!=4)
      {
        #ifdef DEBUG_RFXSWF
          fprintf(stderr,"WriteTag() failed: Long Header (2).\n");
        #endif
        return -1;
      }
    }
    
    if (t->data)
    { if (writer->write(writer,t->data,t->len)!=t->len)
      {
        #ifdef DEBUG_RFXSWF
          fprintf(stderr,"WriteTag() failed: Data.\n");
        #endif
        return -1;
      }
    }
    #ifdef DEBUG_RFXSWF
      else if (t->len) fprintf(stderr,"WriteTag(): Tag Data Error, id=%i\n",t->id);
    #endif

#ifdef MEASURE
  writer->flush(writer);
  printf("TAG %s costs %d bytes\n", swf_TagGetName(t), writer->pos-oldpos);
#endif
  }

  return t->len+(short_tag?2:6);
}

int swf_WriteTag(int handle, TAG * t)
{
  writer_t writer;
  int len = 0;
  if(handle<0)
    return swf_WriteTag2(0, t);
  writer_init_filewriter(&writer, handle);
  len = swf_WriteTag2(&writer, t);
  writer.finish(&writer);
  return len;
}

int swf_DefineSprite_GetRealSize(TAG * t)
// Sprite Handling: Helper function to pack DefineSprite-Tag
{ U32 len = t->len;
  if(len>4) { // folded sprite
      return t->len;
  }
  do
  { t = swf_NextTag(t);
    if (t && t->id!=ST_DEFINESPRITE) len += swf_WriteTag(-1, t);
    else t = NULL;
  } while (t&&(t->id!=ST_END));
  return len;
}

void swf_UnFoldSprite(TAG * t)
{
  U16 id,tmp;
  U32 len;
  TAG*next = t;
  U16 spriteid,spriteframes;
  int level;
  if(t->id!=ST_DEFINESPRITE)
    return;
  if(t->len<=4) // not folded
    return;

  swf_SetTagPos(t,0);

  spriteid = swf_GetU16(t); //id
  spriteframes = swf_GetU16(t); //frames

  level = 1;

  while(1)
  {
    TAG*it = 0;
    tmp = swf_GetU16(t);
    len = tmp&0x3f;
    id  = tmp>>6;
    if(id == ST_END)
	level--;
    if(id == ST_DEFINESPRITE && len<=4)
	level++;

    if (len==0x3f)
	len = swf_GetU32(t);
    it = swf_InsertTag(next, id);
    next = it;
    it->len = len;
    it->id  = id;
    if (it->len)
    { it->data = (U8*)rfx_alloc(it->len);
      it->memsize = it->len;
      swf_GetBlock(t, it->data, it->len);
    }

    if(!level)
	break;
  }
  
  rfx_free(t->data); t->data = 0;
  t->memsize = t->len = t->pos = 0;

  swf_SetU16(t, spriteid);
  swf_SetU16(t, spriteframes);
}

void swf_FoldSprite(TAG * t)
{
  TAG*sprtag=t,*tmp;
  U16 id,frames;
  int level;
  if(t->id!=ST_DEFINESPRITE)
      return;
  if(!t->len) {
      fprintf(stderr, "Error: Sprite has no ID!");
      return;
  }
  if(t->len>4) {
    /* sprite is already folded */
      return;
  }

  t->pos = 0;
  id = swf_GetU16(t);
  rfx_free(t->data);
  t->len = t->pos = t->memsize = 0;
  t->data = 0;

  frames = 0;

  t = swf_NextTag(sprtag);
  level = 1;

  do 
  { 
    if(t->id==ST_SHOWFRAME) frames++;
    if(t->id == ST_DEFINESPRITE && t->len<=4)
	level++;
    if(t->id == ST_END)
	level--;
    t = swf_NextTag(t);
  } while(t && level);
  if(level)
    fprintf(stderr, "rfxswf error: sprite doesn't end(1)\n");

  swf_SetU16(sprtag, id);
  swf_SetU16(sprtag, frames);

  t = swf_NextTag(sprtag);
  level = 1;

  do
  { 
    if(t->len<0x3f&&
	(t->id!=ST_DEFINEBITSLOSSLESS&&t->id!=ST_DEFINEBITSLOSSLESS2&&t->id!=ST_SOUNDSTREAMBLOCK&&
	 t->id!=ST_DEFINEBITSJPEG&&t->id!=ST_DEFINEBITSJPEG2&&t->id!=ST_DEFINEBITSJPEG3)
      ) {
	swf_SetU16(sprtag,t->len|(t->id<<6));
    } else {
	swf_SetU16(sprtag,0x3f|(t->id<<6));
	swf_SetU32(sprtag,t->len);
    }
    if(t->len)
	swf_SetBlock(sprtag,t->data, t->len);
    tmp = t;
    if(t->id == ST_DEFINESPRITE && t->len<=4)
	level++;
    if(t->id == ST_END)
	level--;
    t = swf_NextTag(t);
    swf_DeleteTag(0, tmp);
  } 
  while (t && level);
  if(level)
    fprintf(stderr, "rfxswf error: sprite doesn't end(2)\n");

//  sprtag->next = t;
//  t->prev = sprtag;
}

int swf_IsFolded(TAG * t)
{
    return (t->id == ST_DEFINESPRITE && t->len>4);
}

void swf_FoldAll(SWF*swf)
{
    TAG*tag = swf->firstTag;
    //swf_DumpSWF(stdout, swf);
    while(tag) {
	if(tag->id == ST_DEFINESPRITE) {
	    swf_FoldSprite(tag);
	    //swf_DumpSWF(stdout, swf);
	}
	tag = swf_NextTag(tag);
    }
}

void swf_UnFoldAll(SWF*swf)
{
    TAG*tag = swf->firstTag;
    while(tag) {
	if(tag->id == ST_DEFINESPRITE)
	    swf_UnFoldSprite(tag);
	tag = tag->next;
    }
}

void swf_OptimizeTagOrder(SWF*swf)
{
  TAG*tag,*next;
  TAG*level0;
  int level;
  int changes;
  swf_UnFoldAll(swf);
  /* at the moment, we don't actually do optimizing,
     only fixing of non-spec-conformant things like
     sprite tags */

  do {
    changes = 0;
    level = 0;
    level0 = 0;
    tag = swf->firstTag;
    while(tag) {
      next = tag->next;
      if(tag->id == ST_DEFINESPRITE) {
	if(tag->len>4) {
	  /* ??? all sprites are supposed to be unfolded */
	  fprintf(stderr, "librfxswf error - internal error in OptimizeTagOrder/UnfoldAll\n");
	}
	level++;
	if(level==1) {
	  level0 = tag;
	  tag = next;
	  continue;
	}
      }
      if(level>=1) {
	/* move non-sprite tags out of sprite */
	if(!swf_isAllowedSpriteTag(tag) || level>=2) {
	  /* remove tag from current position */
	  tag->prev->next = tag->next;
	  if(tag->next)
	    tag->next->prev = tag->prev;

	  /* insert before tag level0 */
	  tag->next = level0;
	  tag->prev = level0->prev;
	  level0->prev = tag;
          if(tag->prev)
	    tag->prev->next = tag;
          else
            swf->firstTag = tag;
	  changes = 1;
	}
      }
      if(tag->id == ST_END) {
	level--;
      }

      tag = next;
    }
  } while(changes);
}

// Movie Functions

int swf_ReadSWF2(reader_t*reader, SWF * swf)   // Reads SWF to memory (malloc'ed), returns length or <0 if fails
{     
  if (!swf) return -1;
  memset(swf,0x00,sizeof(SWF));

  { char b[32];                         // read Header
    int len;
    TAG * t;
    TAG t1;
    reader_t zreader;
    
    if ((len = reader->read(reader ,b,8))<8) return -1;

    if (b[0]!='F' && b[0]!='C') return -1;
    if (b[1]!='W') return -1;
    if (b[2]!='S') return -1;
    swf->fileVersion = b[3];
    swf->compressed  = (b[0]=='C')?1:0;
    swf->fileSize    = GET32(&b[4]);
    
    if(swf->compressed) {
	reader_init_zlibinflate(&zreader, reader);
	reader = &zreader;
    }
    swf->compressed = 0; // derive from version number from now on

    reader_GetRect(reader, &swf->movieSize);
    reader->read(reader, &swf->frameRate, 2);
    swf->frameRate = SWAP16(swf->frameRate);
    reader->read(reader, &swf->frameCount, 2);
    swf->frameCount = SWAP16(swf->frameCount);

    /* read tags and connect to list */
    t = &t1;
    while (t) t = swf_ReadTag(reader,t);
    swf->firstTag = t1.next;
    t1.next->prev = NULL;
  }
  
  return reader->pos;
}

int swf_ReadSWF(int handle, SWF * swf)
{
  reader_t reader;
  reader_init_filereader(&reader, handle);
  return swf_ReadSWF2(&reader, swf);
}

int  swf_WriteSWF2(writer_t*writer, SWF * swf)     // Writes SWF to file, returns length or <0 if fails
{ U32 len;
  TAG * t;
  int frameCount=0;
  writer_t zwriter;
  int fileSize = 0;
  int inSprite = 0;
  int ret;
  writer_t*original_writer = writer;
  int writer_lastpos = 0;
    
  if (!swf) return -1;
  if (!writer) return -1; // the caller should provide a nullwriter, not 0, for querying SWF size

  if(original_writer) writer_lastpos = original_writer->pos;

  // Insert REFLEX Tag

#ifdef INSERT_RFX_TAG

  if ((swf->firstTag && swf->firstTag->id != ST_REFLEX) &&
      (!swf->firstTag->next || (swf->firstTag->next->id != ST_REFLEX &&
      (!swf->firstTag->next->next || (swf->firstTag->next->next->id!=ST_REFLEX)))))
  {
      swf_SetBlock(swf_InsertTagBefore(swf, swf->firstTag,ST_REFLEX),(U8*)"rfx",3);
  }

#endif // INSERT_RFX_TAG

  if(swf->fileVersion >= 9) {
    if ((!swf->firstTag || swf->firstTag->id != ST_SCENEDESCRIPTION) &&
	(!swf->firstTag || 
	 !swf->firstTag->next || swf->firstTag->next->id != ST_SCENEDESCRIPTION) &&
	(!swf->firstTag || 
	 !swf->firstTag->next || 
	 !swf->firstTag->next->next || swf->firstTag->next->next->id != ST_SCENEDESCRIPTION))
    {
	TAG*scene = swf_InsertTagBefore(swf, swf->firstTag,ST_SCENEDESCRIPTION);
	swf_SetU16(scene, 1);
	swf_SetString(scene, (U8*)"Scene 1");
	swf_SetU8(scene, 0);
    }
  }
  
  if(swf->fileVersion >= 9) {
      TAG*tag = swf->firstTag;
      U32 flags = 0x08; // | 128 = usenetwork, | 16 = hasmetadata | 8 = actionscript3
      int has_version_8_action=0;
      int has_version_9_action=0;
      while(tag) {
	/* FIXME: this doesn't find actionscript in buttons */
	if(tag->id == ST_DOACTION || tag->id == ST_DOINITACTION) 
	  has_version_8_action=1;
	if(tag->id == ST_DOABC) 
	  has_version_9_action=1;
	tag = tag->next;
      }
      if(has_version_8_action && !has_version_9_action)
	flags = 0x00;

      if (swf->firstTag && swf->firstTag->id != ST_FILEATTRIBUTES)
      {
	  swf_SetU32(swf_InsertTagBefore(swf, swf->firstTag,ST_FILEATTRIBUTES),flags);
      }
  }

  // Count Frames + File Size

  len = 0;
  t = swf->firstTag;
  frameCount = 0;

  while(t) {
      len += swf_WriteTag(-1,t);
      if(t->id == ST_DEFINESPRITE && !swf_IsFolded(t)) inSprite++;
      else if(t->id == ST_END && inSprite) inSprite--;
      else if(t->id == ST_END && !inSprite) {
	if(t->prev && t->prev->id!=ST_SHOWFRAME)
	  frameCount++;
      }
      else if(t->id == ST_SHOWFRAME && !inSprite) frameCount++;
      t = swf_NextTag(t);
  }
  
  { TAG t1;
    char b[64],b4[4];
    U32 l;

    memset(&t1,0x00,sizeof(TAG));
    t1.data    = (U8*)b;
    t1.memsize = 64;
    
    { // measure header file size
      TAG t2;
      char b2[64];
      memset(&t2,0x00,sizeof(TAG));
      t2.data    = (U8*)b2;
      t2.memsize = 64;
      swf_SetRect(&t2, &swf->movieSize);
      swf_SetU16(&t2, swf->frameRate);
      swf_SetU16(&t2, swf->frameCount);
      l = swf_GetTagLen(&t2)+8;
    }
    if(swf->compressed == 8) {
      l -= 8;
    }

    fileSize = l+len;
    if(len) {// don't touch headers without tags
	swf->fileSize = fileSize;
	swf->frameCount = frameCount;
    }

    if(swf->compressed != 8) {
    /* compressed flag set to 8 means "skip first 8 
       header bytes". This is necessary if the caller wants to
       create compressed SWFs himself .
       It also means that we don't initialize our own zlib
       writer, but assume the caller provided one.
     */
      if(swf->compressed==1 || (swf->compressed==0 && swf->fileVersion>=6)) {
	char*id = "CWS";
	writer->write(writer, id, 3);
      } else {
	char*id = "FWS";
	writer->write(writer, id, 3);
      }

      writer->write(writer, &swf->fileVersion, 1);
      PUT32(b4, swf->fileSize);
      writer->write(writer, b4, 4);
      
      if(swf->compressed==1 || (swf->compressed==0 && swf->fileVersion>=6)) {
	writer_init_zlibdeflate(&zwriter, writer);
	writer = &zwriter;
      }
    }

    swf_SetRect(&t1,&swf->movieSize);
    swf_SetU16(&t1,swf->frameRate);
    swf_SetU16(&t1,swf->frameCount);

    ret = writer->write(writer,b,swf_GetTagLen(&t1));
    if (ret!=swf_GetTagLen(&t1))
    {
      #ifdef DEBUG_RFXSWF
	fprintf(stderr, "ret:%d\n",ret);
	perror("write:");
	fprintf(stderr,"WriteSWF() failed: Header.\n");
      #endif
      return -1;
    }

    t = swf->firstTag;
    while (t)
    { if (swf_WriteTag2(writer, t)<0) return -1;
      t = swf_NextTag(t);
    }
    if(swf->compressed==1 || (swf->compressed==0 && swf->fileVersion>=6) || swf->compressed==8) {
      if(swf->compressed != 8) {
	zwriter.finish(&zwriter);
	return original_writer->pos - writer_lastpos;
      }
      return (int)fileSize;
    } else {
      return (int)fileSize;
    }
  }
}

int  swf_WriteSWF(int handle, SWF * swf)     // Writes SWF to file, returns length or <0 if fails
{
  writer_t writer;
  int len = 0;
  
  if(handle<0) {
    writer_init_nullwriter(&writer);
    len = swf_WriteSWF2(&writer, swf);
    return len;
  }
  writer_init_filewriter(&writer, handle);
  len = swf_WriteSWF2(&writer, swf);
  writer.finish(&writer);
  return len;
}

int swf_WriteHeader2(writer_t*writer,SWF * swf)
{
  SWF myswf;
  memcpy(&myswf,swf,sizeof(SWF));
  myswf.firstTag = 0;
  return swf_WriteSWF2(writer, &myswf);
}

int swf_WriteHeader(int handle,SWF * swf)
{
  SWF myswf;
  memcpy(&myswf,swf,sizeof(SWF));
  myswf.firstTag = 0;
  return swf_WriteSWF(handle, &myswf);
}

int swf_WriteCGI(SWF * swf)
{ int len;
  char s[1024];
    
  len = swf_WriteSWF(-1,swf);

  if (len<0) return -1;

  sprintf(s,"Content-type: application/x-shockwave-flash\n"
            "Accept-Ranges: bytes\n"
            "Content-Length: %lu\n"
            "Expires: Thu, 13 Apr 2000 23:59:59 GMT\n"
            "\n",len);
            
  write(fileno(stdout),s,strlen(s));
  return swf_WriteSWF(fileno(stdout),swf);
}

SWF* swf_CopySWF(SWF*swf)
{
    SWF*nswf = (SWF*)rfx_alloc(sizeof(SWF));
    TAG*tag, *ntag;
    memcpy(nswf, swf, sizeof(SWF));
    nswf->firstTag = 0;
    tag = swf->firstTag;
    ntag = 0;
    while(tag) {
        ntag = swf_CopyTag(ntag, tag);
        if(!nswf->firstTag)
            nswf->firstTag = ntag;
        tag = tag->next;
    }
    return nswf;
}

void swf_FreeTags(SWF * swf)                 // Frees all malloc'ed memory for tags
{ TAG * t = swf->firstTag;

  while (t)
  { TAG * tnew = t->next;
    if (t->data) rfx_free(t->data);
    rfx_free(t);
    t = tnew;
  }
  swf->firstTag = 0;
}

// include advanced functions

//#include "modules/swfdump.c"
//#include "modules/swfshape.c"
//#include "modules/swftext.c"
//#include "modules/swffont.c"
//#include "modules/swfobject.c"
//#include "modules/swfbutton.c"
//#include "modules/swftools.c"
//#include "modules/swfcgi.c"
//#include "modules/swfbits.c"
//#include "modules/swfaction.c"
//#include "modules/swfabc.c"
//#include "modules/swfsound.c"
//#include "modules/swfdraw.c"
//#include "modules/swfrender.c"
//#include "modules/swffilter.c"
