/* rfxswf.c

   Library for creating and reading SWF files or parts of it.
   There's a module directory which provides some extended functionality.
   Most modules are included at the bottom of this file.

   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#include "rfxswf.h"

#include <jpeglib.h>
#define _JPEGLIB_INCLUDED_

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

LPTAG NextTag(LPTAG t) { return t->next; }
LPTAG PrevTag(LPTAG t) { return t->prev; }
int   GetFrameNo(LPTAG t)  { return t->frame; }
U16   GetTagID(LPTAG t)    { return t->id; }
U32   GetDataSize(LPTAG t) { return t->len; }
U8*   GetDataSizePtr(LPTAG t) { return &(t->data[t->len]); }
U32   GetTagPos(LPTAG t)   { return t->pos; }

// Basic Data Access Functions

#define ResetBitmask(tag)   if (tag->bitmask)  { tag->pos++; tag->bitmask = 0; }
#define ResetBitcount(tag)  if (tag->bitcount) { tag->bitcount = 0; }

// for future purpose: avoid high level lib functions to change tagpos/bitcount

#define SaveTagPos(tag)
#define RestoreTagPos(tag)

void SetTagPos(LPTAG t,U32 pos)
{ ResetBitmask(t);
  if (pos<=t->len) t->pos = pos;
  #ifdef DEBUG_RFXSWF
  else fprintf(stderr,"SetTagPos() out of bounds: TagID = %i\n",t->id);
  #endif
}

U8 GetU8(LPTAG t)
{ ResetBitmask(t);
  #ifdef DEBUG_RFXSWF
    if (t->pos>=t->len) 
    { fprintf(stderr,"GetU8() out of bounds: TagID = %i\n",t->id);
      return 0;
    }
  #endif
  return t->data[t->pos++];
}

U16 GetU16(LPTAG t)
{ U16 res;
  ResetBitmask(t);
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

U32 GetU32(LPTAG t)
{ U32 res;
  ResetBitmask(t);
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

int GetBlock(LPTAG t,U8 * b,int l)
// returns number of bytes written (<=l)
// b = NULL -> skip data
{ ResetBitmask(t);
  if ((t->len-t->pos)<l) l=t->len-t->pos;
  if (b && l) memcpy(b,&t->data[t->pos],l);
  t->pos+=l;
  return l;
}

int SetBlock(LPTAG t,U8 * b,int l)
// Appends Block to the end of Tagdata, returns size
{ U32 newlen = t->len + l;
  ResetBitcount(t);
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

int SetU8(LPTAG t,U8 v)
{ ResetBitcount(t);
  if ((t->len+1)>t->memsize) return (SetBlock(t,&v,1)==1)?0:-1;
  t->data[t->len++] = v;
  return 0;
}

int SetU16(LPTAG t,U16 v)
{ U8 a[2];
  a[0] = v&0xff;
  a[1] = v>>8;
  
  ResetBitcount(t);
  if ((t->len+2)>t->memsize) return (SetBlock(t,a,2)==2)?0:-1;
  t->data[t->len++] = a[0];
  t->data[t->len++] = a[1];
  return 0;
}

int SetU32(LPTAG t,U32 v)
{ U8 a[4];
  a[0] = v&0xff;        // to ensure correct handling of non-intel byteorder
  a[1] = (v>>8)&0xff;
  a[2] = (v>>16)&0xff;
  a[3] = (v>>24)&0xff;
  
  ResetBitcount(t);
  if ((t->len+4)>t->memsize) return (SetBlock(t,a,4)==4)?0:-1;
  t->data[t->len++] = a[0];
  t->data[t->len++] = a[1];
  t->data[t->len++] = a[2];
  t->data[t->len++] = a[3];
  return 0;
}

U32 GetBits(LPTAG t,int nbits)
{ U32 res = 0;
  if (!nbits) return 0;
  if (!t->bitmask) t->bitmask = 0x80;
  while (nbits)
  { res<<=1;
    if (t->data[t->pos]&t->bitmask) res|=1;
    t->bitmask>>=1;
    nbits--;
    if (!t->bitmask)
    { if (nbits) t->bitmask = 0x80;
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

S32 GetSBits(LPTAG t,int nbits)
{ U32 res = GetBits(t,nbits);
  if (res&(1<<(nbits-1))) res|=(0xffffffff<<nbits);  
  return (S32)res;
}

int SetBits(LPTAG t,U32 v,int nbits)
{ U32 bm = 1<<(nbits-1);

  while (nbits)
  { if (!t->bitcount)
    { if (FAILED(SetU8(t,0))) return -1;
      t->bitcount = 0x80;
    }
    if (v&bm) t->data[t->len-1] |= t->bitcount;
    bm>>=1;
    t->bitcount>>=1;
    nbits--;
  }
  return 0;
}

// Advanced Data Access Functions

int SetRGB(LPTAG t,LPRGBA col)
{ if (!t) return -1;
  if (col)
  { SetU8(t,col->r);
    SetU8(t,col->g);
    SetU8(t,col->b);
  } else SetBlock(t,NULL,3);
  return 0;
}

int SetRGBA(LPTAG t,LPRGBA col)
{ if (!t) return -1;
  if (col)
  { SetU8(t,col->r);
    SetU8(t,col->g);
    SetU8(t,col->b);
    SetU8(t,col->a);
  } else SetBlock(t,NULL,4);
  return 0;
}

int CountBits(U32 v,int nbits)
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

int GetRect(LPTAG t,LPSRECT r)
{ int nbits;
  SRECT dummy;
  if (!r) r = &dummy;
  nbits = (int) GetBits(t,5);
  r->xmin = GetSBits(t,nbits);
  r->xmax = GetSBits(t,nbits);
  r->ymin = GetSBits(t,nbits);
  r->ymax = GetSBits(t,nbits);
  return 0;
}

int SetRect(LPTAG t,LPSRECT r)
{ int nbits;
    
  nbits = CountBits(r->xmin,0);
  nbits = CountBits(r->xmax,nbits);
  nbits = CountBits(r->ymin,nbits);
  nbits = CountBits(r->ymax,nbits);

  SetBits(t,nbits,5);
  SetBits(t,r->xmin,nbits);
  SetBits(t,r->xmax,nbits);
  SetBits(t,r->ymin,nbits);
  SetBits(t,r->ymax,nbits);

  return 0;
}

int GetMatrix(LPTAG t,LPMATRIX m)
{ MATRIX dummy;
  int nbits;
    
  if (!m) m = &dummy;
  
  if (!t)
  { m->sx = m->sy = 0x10000;
    m->r0 = m->r1 = 0;
    m->tx = m->ty = 0;
    return -1;
  }

  ResetBitmask(t);
  
  if (GetBits(t,1))
  { nbits = GetBits(t,5);
    m->sx = GetSBits(t,nbits);
    m->sy = GetSBits(t,nbits);
  }
  else m->sx = m->sy = 0x10000;
  
  if (GetBits(t,1))
  { nbits = GetBits(t,5);
    m->r0 = GetSBits(t,nbits);
    m->r1 = GetSBits(t,nbits);
  }
  else m->r0 = m->r1 = 0x0;

  nbits = GetBits(t,5);
  m->tx = GetSBits(t,nbits);
  m->ty = GetSBits(t,nbits);
  
  return 0;
}

int SetMatrix(LPTAG t,LPMATRIX m)
{ int nbits;
  MATRIX ma;

  if (!m)
  { m = &ma;
    ma.sx = ma.sy = 0x10000;
    ma.r0 = ma.r1 = 0;
    ma.tx = ma.ty = 0;
  }

  ResetBitcount(t);

  if ((m->sx==0x10000)&&(m->sy==0x10000)) SetBits(t,0,1);
  else
  { SetBits(t,1,1);
    nbits = CountBits(m->sx,0);
    nbits = CountBits(m->sy,nbits);
    SetBits(t,nbits,5);
    SetBits(t,m->sx,nbits);
    SetBits(t,m->sy,nbits);
  }

  if ((!m->r0)&&(!m->r1)) SetBits(t,0,1);
  else
  { SetBits(t,1,1);
    nbits = CountBits(m->r0,0);
    nbits = CountBits(m->r1,nbits);
    SetBits(t,nbits,5);
    SetBits(t,m->r0,nbits);
    SetBits(t,m->r1,nbits);
  }

  nbits = CountBits(m->tx,0);
  nbits = CountBits(m->ty,nbits);
  SetBits(t,nbits,5);
  SetBits(t,m->tx,nbits);
  SetBits(t,m->ty,nbits);

  return 0;
}

int GetCXForm(LPTAG t,LPCXFORM cx,U8 alpha) //FIXME: alpha should be type bool
{ CXFORM cxf;
  int hasadd;
  int hasmul;
  int nbits;
    
  if (!cx) cx = &cxf;
  
  cx->a0 = cx->r0 = cx->g0 = cx->b0 = 256;
  cx->a1 = cx->r1 = cx->g1 = cx->b1 = 0;

  if (!t) return 0;
  
  ResetBitmask(t);
  hasadd = GetBits(t,1);
  hasmul = GetBits(t,1);
  nbits  = GetBits(t,4);

  if (hasmul)
  { cx->r0 = (S16)GetSBits(t,nbits);
    cx->g0 = (S16)GetSBits(t,nbits);
    cx->b0 = (S16)GetSBits(t,nbits);
    if (alpha)
      cx->a0 = (S16)GetSBits(t,nbits);
  }

  if (hasadd)
  { cx->r1 = (S16)GetSBits(t,nbits);
    cx->g1 = (S16)GetSBits(t,nbits);
    cx->b1 = (S16)GetSBits(t,nbits);
    if (alpha)
      cx->a1 = (S16)GetSBits(t,nbits);
  }
  
  return 0;
}

int SetCXForm(LPTAG t,LPCXFORM cx,U8 alpha)
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
  { if (alpha) nbits = CountBits((S32)cx->a0,nbits);
    nbits = CountBits((S32)cx->r0,nbits);
    nbits = CountBits((S32)cx->g0,nbits);
    nbits = CountBits((S32)cx->b0,nbits);
  }

  if (hasadd)
  { if (alpha) nbits = CountBits((S32)cx->a1,nbits);
    nbits = CountBits((S32)cx->r1,nbits);
    nbits = CountBits((S32)cx->g1,nbits);
    nbits = CountBits((S32)cx->b1,nbits);
  }
  
  ResetBitcount(t);
  SetBits(t,hasadd?1:0,1);
  SetBits(t,hasmul?1:0,1);
  SetBits(t,nbits,4);

  if (hasmul)
  { SetBits(t,cx->r0,nbits);
    SetBits(t,cx->g0,nbits);
    SetBits(t,cx->b0,nbits);
    if (alpha) SetBits(t,cx->a0,nbits);
  }

  if (hasadd)
  { SetBits(t,cx->r1,nbits);
    SetBits(t,cx->g1,nbits);
    SetBits(t,cx->b1,nbits);
    if (alpha) SetBits(t,cx->a1,nbits);
  }
  
  return 0;
}

int GetPoint(LPTAG t,LPSPOINT p) { return 0; }
int SetPoint(LPTAG t,LPSPOINT p) { return 0; }

// Tag List Manipulating Functions

int RFXSWF_UpdateFrame(LPTAG t,S8 delta)
// returns number of frames
{ int res = -1;
  while (t)
  { t->frame+=delta;
    res = t->frame;
    t = t->next;
  }
  return res;
}

#define UpdateFrame(a,b) RFXSWF_UpdateFrame(a,b)

LPTAG InsertTag(LPTAG after,U16 id)     // updates frames, if nescessary
{ LPTAG t;

  t = (LPTAG)malloc(sizeof(TAG));
  if (t)
  { memset(t,0x00,sizeof(TAG));
    t->id = id;
    t->bitcount = 0x80;
    
    if (after)
    { t->frame = after->frame;
      t->prev  = after;
      t->next  = after->next;
      after->next = t;
      if (t->next) t->next->prev = t;
      
      if (id==ST_SHOWFRAME) UpdateFrame(t->next,+1);
    }
  }
  return t;
}

int DeleteTag(LPTAG t)
{ if (!t) return -1;

  if (t->id==ST_SHOWFRAME) UpdateFrame(t->next,-1);
    
  if (t->prev) t->prev->next = t->next;
  if (t->next) t->next->prev = t->prev;

  if (t->data) free(t->data);
  free(t);
  return 0;
}

LPTAG RFXSWF_ReadTag(int handle,LPTAG prev)
{ LPTAG t;
  U16 raw;
  U32 len;
  int id;

  if (read(handle,&raw,2)!=2) return NULL;

  len = raw&0x3f;
  id  = raw>>6;

  if (len==0x3f)
  { if (read(handle,&len,4)!=4) return NULL;
  }

  if (id==ST_DEFINESPRITE) len = 2*sizeof(U16);
  // Sprite handling fix: Flaten sprite tree

  t = (LPTAG)malloc(sizeof(TAG));
  
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

int DefineSprite_GetRealSize(LPTAG t);

int RFXSWF_WriteTag(int handle,LPTAG t)
// returns tag length in bytes (incl. Header), -1 = Error
// handle = -1 -> no output
{ U16 raw[3];
  U32 len;
  int short_tag;

  if (!t) return -1;

  len = (t->id==ST_DEFINESPRITE)?DefineSprite_GetRealSize(t):t->len;

  short_tag = len<0x3f;

  if (handle>=0)
  { if (short_tag)
    { raw[0] = len|((t->id&0x3ff)<<6);
      if (write(handle,raw,2)!=2)
      {
        #ifdef DEBUG_RFXSWF
          fprintf(stderr,"WriteTag() failed: Short Header.\n");
        #endif
        return -1;
      }
    }
    else
    { raw[0] = (t->id<<6)|0x3f;
      raw[1] = (U16)(len&0xffff);
      raw[2] = (U16)(len>>16);
      if (write(handle,raw,6)!=6)
      {
        #ifdef DEBUG_RFXSWF
          fprintf(stderr,"WriteTag() failed: Long Header.\n");
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

int DefineSprite_GetRealSize(LPTAG t)
// Sprite Handling: Helper function to pack DefineSprite-Tag
{ U32 len = t->len;
  do
  { t = NextTag(t);
    if (t->id!=ST_DEFINESPRITE) len += RFXSWF_WriteTag(-1,t);
    else t = NULL;
  } while (t&&(t->id!=ST_END));
  return len;
}

#define ReadTag(a,b)  RFXSWF_ReadTag(a,b)
#define WriteTag(a,b) RFXSWF_WriteTag(a,b)

// Movie Functions

int ReadSWF(int handle,LPSWF swf)       // Reads SWF to memory (malloc'ed), returns length or <0 if fails
{     
  if (!swf) return -1;
  memset(swf,0x00,sizeof(SWF));

  { char b[32];                         // Header lesen
    TAG t1;
    LPTAG t;
    
    memset(&t1,0x00,sizeof(TAG));
    
    if ((t1.len=read(handle,b,32))<21) return -1;
    t1.data = (U8*)b;

    if (GetU8(&t1)!=(U8)'F') return -1;
    if (GetU8(&t1)!=(U8)'W') return -1;
    if (GetU8(&t1)!=(U8)'S') return -1;

    swf->FileVersion = GetU8(&t1);
    swf->FileSize    = GetU32(&t1);
    GetRect(&t1,&swf->MovieSize);
    swf->FrameRate   = GetU16(&t1);
    swf->FrameCount  = GetU16(&t1);

    GetU8(&t1);
    lseek(handle,GetTagPos(&t1)-1,SEEK_SET);
  
                                        // Tags lesen und verketten
    t = &t1;
    while (t) t = ReadTag(handle,t);
    swf->FirstTag = t1.next;
    t1.next->prev = NULL;
  }
  
  return 0;
}
int  WriteSWF(int handle,LPSWF swf)     // Writes SWF to file, returns length or <0 if fails
{ U32 len;
  LPTAG t;
    
  if (!swf) return -1;

  // Insert REFLEX Tag

#ifdef INSERT_RFX_TAG

  if (NextTag(swf->FirstTag))
    if (GetTagID(NextTag(swf->FirstTag))!=ST_REFLEX)
      SetBlock(InsertTag(swf->FirstTag,ST_REFLEX),"rfx",3);

#endif // INSERT_RFX_TAG

  // Count Frames + File Size

  len = 0;
  t = swf->FirstTag;
  swf->FrameCount = 0;

  while(t)
  { len += WriteTag(-1,t);
    if (t->id==ST_SHOWFRAME) swf->FrameCount++;
    t = NextTag(t);
  }
  
  { TAG t1;
    char b[64];
    U32 l;

    memset(&t1,0x00,sizeof(TAG));
    t1.data    = (U8*)b;
    t1.memsize = 64;
    
    SetU8(&t1,'F');      
    SetU8(&t1,'W');      
    SetU8(&t1,'S');
    SetU8(&t1,swf->FileVersion);
    
    SetU32(&t1,0);                      // Keep space for filesize
    SetRect(&t1,&swf->MovieSize);
    SetU16(&t1,swf->FrameRate);
    SetU16(&t1,swf->FrameCount);

    l = GetDataSize(&t1);
    swf->FileSize = l+len;
    t1.len = 4;                         // bad & ugly trick !
    SetU32(&t1,swf->FileSize);

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

      t = swf->FirstTag;
      while (t)
      { if (WriteTag(handle,t)<0) return -1;
        t = NextTag(t);
      }
    }
  }
  return (int)swf->FileSize;
}

int WriteCGI(LPSWF swf)
{ int len;
  char s[1024];
    
  len = WriteSWF(-1,swf);

  if (len<0) return -1;

  sprintf(s,"Content-type: application/x-shockwave-flash\n"
            "Accept-Ranges: bytes\n"
            "Content-Length: %lu\n"
            "Expires: Thu, 13 Apr 2000 23:59:59 GMT\n"
            "\n",len);
            
  write(fileno(stdout),s,strlen(s));
  return WriteSWF(fileno(stdout),swf);
}

void FreeTags(LPSWF swf)                 // Frees all malloc'ed memory for tags
{ LPTAG t = swf->FirstTag;

  while (t)
  { LPTAG tnew = t->next;
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
#include "modules\swfbits.c"
#include "modules\swftools.c"
#include "modules\swfcgi.c"

#else

#include "modules/swfdump.c"
#include "modules/swfshape.c"
#include "modules/swftext.c"
#include "modules/swfobject.c"
#include "modules/swfbutton.c"
#include "modules/swfbits.c"
#include "modules/swftools.c"
#include "modules/swfcgi.c"

#endif


