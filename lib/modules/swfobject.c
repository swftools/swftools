/* swfobject.c

   Object place and move routines
      
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#define PF_MOVE         0x01
#define PF_CHAR         0x02
#define PF_MATRIX       0x04
#define PF_CXFORM       0x08
#define PF_RATIO        0x10
#define PF_NAME         0x20
#define PF_CLIPACTION   0x40

int swf_ObjectPlace(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,U8 * name)
{ U8 flags;
  if (!t) return -1;

  flags = (id?PF_CHAR:0)|(m?PF_MATRIX:0)|(cx?PF_CXFORM:0)|(name?PF_NAME:0)|((m||cx)&&(!id)?PF_MOVE:0);

  swf_SetU8(t,flags);
  swf_SetU16(t,depth);
  if (flags&PF_CHAR) swf_SetU16(t,id);
  if (flags&PF_MATRIX) swf_SetMatrix(t,m);
  if (flags&PF_CXFORM) swf_SetCXForm(t,cx,(cx->a0!=256)||(cx->a1));
  if (flags&PF_RATIO) swf_SetU16(t,0);
  if (flags&PF_NAME) swf_SetString(t,name);
  
  return 0; 
}

int swf_ObjectPlaceClip(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,U8 * name, U16 clipaction)
{ U8 flags;
  if (!t) return -1;

  flags = (id?PF_CHAR:0)|(m?PF_MATRIX:0)|(cx?PF_CXFORM:0)|(name?PF_NAME:0)|
          ((m||cx)&&(!id)?PF_MOVE:0)|(clipaction?PF_CLIPACTION:0);

  swf_SetU8(t,flags);
  swf_SetU16(t,depth);
  if (flags&PF_CHAR) swf_SetU16(t,id);
  if (flags&PF_MATRIX) swf_SetMatrix(t,m);
  if (flags&PF_CXFORM) swf_SetCXForm(t,cx,(cx->a0!=256)||(cx->a1));
  if (flags&PF_RATIO) swf_SetU16(t,0);
  if (flags&PF_NAME) swf_SetString(t,name);
  if (flags&PF_CLIPACTION) swf_SetU16(t, clipaction);
  return 0; 
}

int swf_ObjectMove(TAG * t,U16 depth,MATRIX * m,CXFORM * cx)
{ return swf_ObjectPlace(t,0,depth,m,cx,NULL);
}
