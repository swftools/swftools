/* rfxswf.c

   Library for creating and reading SWF files or parts of it.
   There's a module directory which provides some extended functionality.
   Most modules are included at the bottom of this file.

   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#include "rfxswf.h"

#ifdef HAVE_LIBJPEG
#ifdef HAVE_JPEGLIB_H
#define HAVE_BOOLEAN
#include <jpeglib.h>
#define _JPEGLIB_INCLUDED_
#endif // HAVE_JPEGLIB_H
#endif // HAVE_LIBJPEG

#ifdef HAVE_LIBZ
#ifdef HAVE_ZLIB_H
#include <zlib.h>
#define _ZLIB_INCLUDED_
#endif // HAVE_ZLIB_H
#endif // HAVE_LIBZ

// Win32 support may be broken since it was only tested in an older version for Watcom C
#ifdef __NT__
#  include <io.h>
#  include <malloc.h>
#  include <string.h>
#  ifdef DEBUG_RFXSWF
#    include <stdio.h>
#  endif
#else
#endif

// internal constants

#define MALLOC_SIZE     128
#define INSERT_RFX_TAG

#define MEMSIZE(l) (((l/MALLOC_SIZE)+1)*MALLOC_SIZE)

// inline wrapper functions

TAG * swf_NextTag(TAG * t) { return t->next; }
TAG * swf_PrevTag(TAG * t) { return t->prev; }
int   swf_GetFrameNo(TAG * t)  { return t->frame; }
U16   swf_GetTagID(TAG * t)    { return t->id; }
U32   swf_GetTagLen(TAG * t) { return t->len; }
U8*   swf_GetTagLenPtr(TAG * t) { return &(t->data[t->len]); }
U32   swf_GetTagPos(TAG * t)   { return t->pos; }

// Basic Data Access Functions

#define swf_ResetReadBits(tag)   if (tag->readBit)  { tag->pos++; tag->readBit = 0; }
#define swf_ResetWriteBits(tag)  if (tag->writeBit) { tag->writeBit = 0; }

// for future purpose: avoid high level lib functions to change tagpos/bitpos

#define swf_SaveTagPos(tag)
#define swf_RestoreTagPos(tag)

void swf_SetTagPos(TAG * t,U32 pos)
{ swf_ResetReadBits(t);
  if (pos<=t->len) t->pos = pos;
  #ifdef DEBUG_RFXSWF
  else fprintf(stderr,"SetTagPos() out of bounds: TagID = %i\n",t->id);
  #endif
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

int swf_SetBlock(TAG * t,U8 * b,int l)
// Appends Block to the end of Tagdata, returns size
{ U32 newlen = t->len + l;
  swf_ResetWriteBits(t);
  if (newlen>t->memsize)
  { U32  newmem  = MEMSIZE(newlen);  
    U8 * newdata = (U8*)((t->data)?realloc(t->data,newmem):malloc(newmem));
    if (!newdata)
    {
      #ifdef DEBUG_RFXSWF
        fprintf(stderr,"Fatal Error: malloc()/realloc() failed.\n");
      #endif
      return 0;
    }
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

int swf_SetRGB(TAG * t,RGBA * col)
{ if (!t) return -1;
  if (col)
  { swf_SetU8(t,col->r);
    swf_SetU8(t,col->g);
    swf_SetU8(t,col->b);
  } else swf_SetBlock(t,NULL,3);
  return 0;
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

int swf_CountBits(U32 v,int nbits)
{ int n = 33;
  U32 m = 0x80000000;
  if (!v) n = 0; else
  if (v&m)
  { while (v&m)
    { n--;
      m>>=1;
      if (!m) break;
    } 
  }
  else
  { while (!(v&m))
    { n--;
      m>>=1;
      if (!m) break;
    } 
  }
  return (n>nbits)?n:nbits;
}

int swf_GetRect(TAG * t,SRECT * r)
{ int nbits;
  SRECT dummy;
  if (!r) r = &dummy;
  nbits = (int) swf_GetBits(t,5);
  r->xmin = swf_GetSBits(t,nbits);
  r->xmax = swf_GetSBits(t,nbits);
  r->ymin = swf_GetSBits(t,nbits);
  r->ymax = swf_GetSBits(t,nbits);
  return 0;
}

int swf_SetRect(TAG * t,SRECT * r)
{ int nbits;
    
  nbits = swf_CountBits(r->xmin,0);
  nbits = swf_CountBits(r->xmax,nbits);
  nbits = swf_CountBits(r->ymin,nbits);
  nbits = swf_CountBits(r->ymax,nbits);

  swf_SetBits(t,nbits,5);
  swf_SetBits(t,r->xmin,nbits);
  swf_SetBits(t,r->xmax,nbits);
  swf_SetBits(t,r->ymin,nbits);
  swf_SetBits(t,r->ymax,nbits);

  return 0;
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

int swf_GetCXForm(TAG * t,CXFORM * cx,U8 alpha) //FIXME: alpha should be type bool
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

int swf_GetPoint(TAG * t,SPOINT * p) { return 0; }
int swf_SetPoint(TAG * t,SPOINT * p) { return 0; }

// Tag List Manipulating Functions

int RFXSWF_UpdateFrame(TAG * t,S8 delta)
// returns number of frames
{ int res = -1;
  while (t)
  { t->frame+=delta;
    res = t->frame;
    t = t->next;
  }
  return res;
}

#define swf_UpdateFrame(a,b) RFXSWF_UpdateFrame(a,b)

TAG * swf_InsertTag(TAG * after,U16 id)     // updates frames, if nescessary
{ TAG * t;

  t = (TAG *)malloc(sizeof(TAG));
  if (t)
  { memset(t,0x00,sizeof(TAG));
    t->id = id;
    
    if (after)
    { t->frame = after->frame;
      t->prev  = after;
      t->next  = after->next;
      after->next = t;
      if (t->next) t->next->prev = t;
      
      if (id==ST_SHOWFRAME) swf_UpdateFrame(t->next,+1);
    }
  }
  return t;
}

int swf_DeleteTag(TAG * t)
{ if (!t) return -1;

  if (t->id==ST_SHOWFRAME) swf_UpdateFrame(t->next,-1);
    
  if (t->prev) t->prev->next = t->next;
  if (t->next) t->next->prev = t->prev;

  if (t->data) free(t->data);
  free(t);
  return 0;
}

TAG * RFXSWF_ReadTag(int handle,TAG * prev)
{ TAG * t;
  U16 raw;
  U32 len;
  int id;

  if (read(handle,&raw,2)!=2) return NULL;
  raw = SWAP16(raw);

  len = raw&0x3f;
  id  = raw>>6;

  if (len==0x3f)
  {
      if (read(handle,&len,4)!=4) return NULL;
      len = SWAP32(len);
  }

  if (id==ST_DEFINESPRITE) len = 2*sizeof(U16);
  // Sprite handling fix: Flaten sprite tree

  t = (TAG *)malloc(sizeof(TAG));
  
  if (!t)
  {
    #ifdef DEBUG_RFXSWF
      fprintf(stderr,"Fatal Error: malloc()/realloc() failed.\n");
    #endif
    return NULL;
  }

  memset(t,0x00,sizeof(TAG));
  
  t->len = len;
  t->id  = id;

  if (t->len)
  { t->data = (U8*)malloc(t->len);
    if (!t->data)
    {
      #ifdef DEBUG_RFXSWF
        fprintf(stderr,"Fatal Error: malloc()/realloc() failed.\n");
      #endif
      return NULL;
    }
    t->memsize = t->len;
    if (read(handle,t->data,t->len)!=t->len) return NULL;
  }

  if (prev)
  { t->frame = prev->frame+((prev->id==ST_SHOWFRAME)?1:0);
    t->prev  = prev;
    prev->next = t;
  }

  return t;
}

int RFXSWF_DefineSprite_GetRealSize(TAG * t);

int RFXSWF_WriteTag(int handle,TAG * t)
// returns tag length in bytes (incl. Header), -1 = Error
// handle = -1 -> no output
{ U16 raw[3];
  U32 len;
  int short_tag;

  if (!t) return -1;

  len = (t->id==ST_DEFINESPRITE)?RFXSWF_DefineSprite_GetRealSize(t):t->len;

  short_tag = len<0x3f;

  if (handle>=0)
  { if (short_tag)
    { raw[0] = SWAP16(len|((t->id&0x3ff)<<6));
      if (write(handle,raw,2)!=2)
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
      if (write(handle,raw,2)!=2)
      {
#ifdef DEBUG_RFXSWF
          fprintf(stderr,"WriteTag() failed: Long Header (1).\n");
#endif
	  return -1;
      }
      
      len = SWAP32(len);
      if (write(handle,&len,4)!=4)
      {
        #ifdef DEBUG_RFXSWF
          fprintf(stderr,"WriteTag() failed: Long Header (2).\n");
        #endif
        return -1;
      }
    }
    
    if (t->data)
    { if (write(handle,t->data,t->len)!=t->len)
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
  }

  return t->len+(short_tag?2:6);
}

int swf_WriteTag(int handle,TAG * t)
{
    return RFXSWF_WriteTag(handle, t);
}

int RFXSWF_DefineSprite_GetRealSize(TAG * t)
// Sprite Handling: Helper function to pack DefineSprite-Tag
{ U32 len = t->len;
  if(len>4) { // folded sprite
      return t->len;
  }
  do
  { t = swf_NextTag(t);
    if (t->id!=ST_DEFINESPRITE) len += RFXSWF_WriteTag(-1,t);
    else t = NULL;
  } while (t&&(t->id!=ST_END));
  return len;
}

void swf_FoldSprite(TAG * t)
{
  TAG*sprtag=t,*tmp;
  U16 id,frames,tmpid;
  if(t->id!=ST_DEFINESPRITE)
      return;
  if(!t->len) {
      fprintf(stderr, "Error: Sprite has no ID!");
      return;
  }

  t->pos = 0;
  id = swf_GetU16(t);
  //frames = swf_GetU16(t);
  free(t->data);
  t->len = t->pos = t->memsize = 0;
  t->data = 0;

  frames = 0;

  do 
  { 
    if(t->id==ST_SHOWFRAME) frames++;
    t = swf_NextTag(t);
  } while(t&&t!=ST_END);

  t = swf_NextTag(sprtag);
  swf_SetU16(sprtag, id);
  swf_SetU16(sprtag, frames);

  do
  { 
    tmpid= t->id;
    if(t->len<0x3f) {
	swf_SetU16(sprtag,t->len|(t->id<<6));
    } else {
	swf_SetU16(sprtag,0x3f|(t->id<<6));
	swf_SetU32(sprtag,t->len);
    }
    if(t->len)
	swf_SetBlock(sprtag,t->data, t->len);
    tmp = t;
    t = swf_NextTag(t);
    swf_DeleteTag(tmp);
  } 
  while (t&&(tmpid!=ST_END));

//  sprtag->next = t;
//  t->prev = sprtag;
}

void swf_FoldAll(SWF*swf)
{
    TAG*tag = swf->firstTag;
    while(tag) {
	if(tag->id == ST_DEFINESPRITE)
	    swf_FoldSprite(tag);
	tag = swf_NextTag(tag);
    }
}

#define swf_ReadTag(a,b)  RFXSWF_ReadTag(a,b)
#define swf_WriteTag(a,b)  RFXSWF_WriteTag(a,b)

// Movie Functions

int swf_InitSWF(void*data, int length, SWF * swf) /* copy a swf in memory into SWF struct */
{
  TAG reader;
    /* 
	unfinished!
     */
  *(int*)0=0xDEAD;
  if (!swf) return -1;
  memset(swf,0x00,sizeof(SWF));
  memset(&reader,0x00,sizeof(TAG));
  reader.data = data;
  reader.len = reader.memsize = length;

  { char b[32];                         // read Header
    TAG * t;
    
    if (swf_GetU8(&reader)!=(U8)'F') return -1;
    if (swf_GetU8(&reader)!=(U8)'W') return -1;
    if (swf_GetU8(&reader)!=(U8)'S') return -1;

    swf->fileVersion = swf_GetU8(&reader);
    swf->fileSize    = swf_GetU32(&reader);
    swf_GetRect(&reader,&swf->movieSize);
    swf->frameRate   = swf_GetU16(&reader);
    swf->frameCount  = swf_GetU16(&reader);

    /*t = &t1;
    while (t) t = swf_ReadTag(handle,t);
    swf->firstTag = t1.next;
    t1.next->prev = NULL;*/
  }
}

int swf_ReadSWF(int handle,SWF * swf)   // Reads SWF to memory (malloc'ed), returns length or <0 if fails
{     
  if (!swf) return -1;
  memset(swf,0x00,sizeof(SWF));

  { char b[32];                         // read Header
    TAG t1;
    TAG * t;
    
    memset(&t1,0x00,sizeof(TAG));
    
    if ((t1.len=read(handle,b,32))<21) return -1;
    t1.data = (U8*)b;

    if (swf_GetU8(&t1)!=(U8)'F') return -1;
    if (swf_GetU8(&t1)!=(U8)'W') return -1;
    if (swf_GetU8(&t1)!=(U8)'S') return -1;

    swf->fileVersion = swf_GetU8(&t1);
    swf->fileSize    = swf_GetU32(&t1);
    swf_GetRect(&t1,&swf->movieSize);
    swf->frameRate   = swf_GetU16(&t1);
    swf->frameCount  = swf_GetU16(&t1);

    swf_GetU8(&t1);
    lseek(handle,swf_GetTagPos(&t1)-1,SEEK_SET);
  
                                        // reda tags and connect to list
    t = &t1;
    while (t) t = swf_ReadTag(handle,t);
    swf->firstTag = t1.next;
    t1.next->prev = NULL;
  }
  
  return 0;
}

int  swf_WriteSWF(int handle,SWF * swf)     // Writes SWF to file, returns length or <0 if fails
{ U32 len;
  TAG * t;
    
  if (!swf) return -1;

  // Insert REFLEX Tag

#ifdef INSERT_RFX_TAG

  if (swf->firstTag && swf_NextTag(swf->firstTag))
    if (swf_GetTagID(swf_NextTag(swf->firstTag))!=ST_REFLEX)
      swf_SetBlock(swf_InsertTag(swf->firstTag,ST_REFLEX),"rfx",3);

#endif // INSERT_RFX_TAG

  // Count Frames + File Size

  len = 0;
  t = swf->firstTag;
  swf->frameCount = 0;

  while(t)
  { len += swf_WriteTag(-1,t);
    if (t->id==ST_SHOWFRAME) swf->frameCount++;
    t = swf_NextTag(t);
  }
  
  { TAG t1;
    char b[64];
    U32 l;

    memset(&t1,0x00,sizeof(TAG));
    t1.data    = (U8*)b;
    t1.memsize = 64;
    
    swf_SetU8(&t1,'F');      
    swf_SetU8(&t1,'W');      
    swf_SetU8(&t1,'S');
    swf_SetU8(&t1,swf->fileVersion);
    
    swf_SetU32(&t1,swf->fileSize);         // Keep space for filesize
    swf_SetRect(&t1,&swf->movieSize);
    swf_SetU16(&t1,swf->frameRate);
    swf_SetU16(&t1,swf->frameCount);

    l = swf_GetTagLen(&t1);
    swf->fileSize = l+len;
    if(swf->firstTag) {
	t1.len = 4;                         // bad & ugly trick !
	swf_SetU32(&t1,swf->fileSize);
    }

    if (handle>=0)
    { 
      int ret = write(handle,b,l);
      if (ret!=l)
      {
        #ifdef DEBUG_RFXSWF
          printf("ret:%d (fd:%d)\n",ret, handle);
          perror("write:");
          fprintf(stderr,"WriteSWF() failed: Header.\n");
        #endif
        return -1;
      }

      t = swf->firstTag;
      while (t)
      { if (swf_WriteTag(handle,t)<0) return -1;
        t = swf_NextTag(t);
      }
    }
  }
  return (int)swf->fileSize;
}

int swf_WriteHeader(int handle,SWF * swf)
{
    SWF myswf;
    memcpy(&myswf,swf,sizeof(SWF));
    myswf.firstTag = 0;
    swf_WriteSWF(handle, &myswf);
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

void swf_FreeTags(SWF * swf)                 // Frees all malloc'ed memory for tags
{ TAG * t = swf->firstTag;

  while (t)
  { TAG * tnew = t->next;
    if (t->data) free(t->data);
    free(t);
    t = tnew;
  }
}

// include advanced functions

#ifdef __NT__

#include "modules\swfdump.c"
#include "modules\swfshape.c"
#include "modules\swftext.c"
#include "modules\swfobject.c"
#include "modules\swfbutton.c"
#include "modules\swftools.c"
#include "modules\swfcgi.c"
#include "modules\swfbits.c"
#include "modules\swfaction.c"
#include "modules\swfsound.c"

#else

#include "modules/swfdump.c"
#include "modules/swfshape.c"
#include "modules/swftext.c"
#include "modules/swfobject.c"
#include "modules/swfbutton.c"
#include "modules/swftools.c"
#include "modules/swfcgi.c"
#include "modules/swfbits.c"
#include "modules/swfaction.c"
#include "modules/swfsound.c"

#endif


