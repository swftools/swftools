/* swfobject.c

   Object place and move routines
      
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
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

#define PF_MOVE         0x01
#define PF_CHAR         0x02
#define PF_MATRIX       0x04
#define PF_CXFORM       0x08
#define PF_RATIO        0x10
#define PF_NAME         0x20
#define PF_CLIPACTION   0x40
#define PF_ACTIONEVENT  0x80

int isUnitMatrix(MATRIX* m)
{
    /* a matrix with all zeros is also considered
       "unit matrix", as a zeroed out MATRIX structure
       usually means that the caller doesn't want to
       set the matrix */
    if((   (m->sx == 0x10000 && m->sy == 0x10000) 
	|| (m->sx == 0 && m->sy == 0))
		&& ((m->r0|m->r1|m->tx|m->ty) == 0)
      )
	return 1;
    return 0;
}

int isUnitCXForm(CXFORM* cx)
{
    if((cx->a0==256 && cx->r0==256 && cx->g0==256 && cx->b0==256) &&
       (cx->a1==0 && cx->r1==0 && cx->g1==0 && cx->b1==0))
	return 1;
    /* A CXForm of all zeros is, unfortunately, not as unlikely
       as a matrix of all zeros. However, we still treat it
       as non-existent/uniform transform */
    if((cx->a0==0 && cx->r0==0 && cx->g0==0 && cx->b0==0) &&
       (cx->a1==0 && cx->r1==0 && cx->g1==0 && cx->b1==0))
	return 1;
    return 0;
}

int swf_ObjectPlace(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,U8 * name)
{ U8 flags;
  if (!t) return -1;

  if(cx && cx->r1==0 && cx->g1==0 && cx->b1==0 && cx->a1==0
	&& cx->r0==256 && cx->g0==256 && cx->b0==256 && cx->a0==256)
      cx = 0;

  if(m && isUnitMatrix(m)) 
      m = 0;

  flags = (id?PF_CHAR:0)|(m?PF_MATRIX:0)|(cx?PF_CXFORM:0)|(name?PF_NAME:0)|((m||cx)&&(!id)?PF_MOVE:0);

  swf_SetU8(t,flags);
  swf_SetU16(t,depth);
  if (flags&PF_CHAR) swf_SetU16(t,id);
  if (flags&PF_MATRIX) swf_SetMatrix(t,m);
  if (flags&PF_CXFORM) swf_SetCXForm(t,cx,1);
  if (flags&PF_RATIO) swf_SetU16(t,0);
  if (flags&PF_NAME) swf_SetString(t,name);
  
  return 0; 
}

int swf_ObjectPlaceClip(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,U8 * name, U16 clipaction)
{ U8 flags;
  if (!t) return -1;
  
  if(cx && cx->r1==0 && cx->g1==0 && cx->b1==0 && cx->a1==0
	&& cx->r0==256 && cx->g0==256 && cx->b0==256 && cx->a0==256)
      cx = 0;
  
  if(m && isUnitMatrix(m)) 
      m = 0;

  flags = (id?PF_CHAR:0)|(m?PF_MATRIX:0)|(cx?PF_CXFORM:0)|(name?PF_NAME:0)|
          ((m||cx)&&(!id)?PF_MOVE:0)|(clipaction?PF_CLIPACTION:0);

  swf_SetU8(t,flags);
  swf_SetU16(t,depth);
  if (flags&PF_CHAR) swf_SetU16(t,id);
  if (flags&PF_MATRIX) swf_SetMatrix(t,m);
  if (flags&PF_CXFORM) swf_SetCXForm(t,cx,1);
  if (flags&PF_RATIO) swf_SetU16(t,0);

  /* ??? The spec states that name comes first? */
  if (flags&PF_CLIPACTION) swf_SetU16(t, clipaction);
  if (flags&PF_NAME) swf_SetString(t,name);
  return 0; 
}

int swf_ObjectMove(TAG * t,U16 depth,MATRIX * m,CXFORM * cx)
{ return swf_ObjectPlace(t,0,depth,m,cx,NULL);
}

void swf_SetPlaceObject(TAG * t,SWFPLACEOBJECT* obj)
{ 
    U8 flags;
    if (!t) return ;
    if(t->id == ST_PLACEOBJECT) {
	swf_SetU16(t, obj->id);
	swf_SetU16(t, obj->depth);	
	swf_SetMatrix(t, &obj->matrix);
	swf_SetCXForm(t, &obj->cxform, 0);
    } else {
	int m = !isUnitMatrix(&obj->matrix);
	int cx = !isUnitCXForm(&obj->cxform);

	flags = (obj->id?PF_CHAR:0)|(m?PF_MATRIX:0)|(cx?PF_CXFORM:0)|(obj->ratio?PF_RATIO:0)|
		(obj->name?PF_NAME:0)|(obj->move?PF_MOVE:0)|
		(obj->clipdepth?PF_CLIPACTION:0);

	swf_SetU8(t,flags);
	swf_SetU16(t,obj->depth);
	if (flags&PF_CHAR) swf_SetU16(t,obj->id);
	if (flags&PF_MATRIX) swf_SetMatrix(t,&obj->matrix);
	if (flags&PF_CXFORM) swf_SetCXForm(t,&obj->cxform,1);
	if (flags&PF_RATIO) swf_SetU16(t,obj->ratio);
  
	/* ??? The spec states that name comes first? */
	if (flags&PF_CLIPACTION) swf_SetU16(t,obj->clipdepth);
	if (flags&PF_NAME) swf_SetString(t,obj->name);
	if (flags&PF_ACTIONEVENT) {
	    // ...
	}
    }
}

void swf_GetPlaceObject(TAG * tag,SWFPLACEOBJECT* obj)
{
    U8 flags;
    if(!tag) {
	memset(obj, 0, sizeof(SWFPLACEOBJECT));
	swf_GetMatrix(0, &obj->matrix);
	swf_GetCXForm(0, &obj->cxform, 1);
	//obj->internal = PF_CHAR|PF_MATRIX|PF_CXFORM;
	return;
    }
    swf_SetTagPos(tag, 0);
    
    if(tag->id == ST_PLACEOBJECT) {
	obj->id = swf_GetU16(tag);
	obj->depth = swf_GetU16(tag);
	swf_GetMatrix(tag, &obj->matrix);
	swf_GetCXForm(tag, &obj->cxform, 0);
	//obj->internal = PF_CHAR|PF_MATRIX|PF_CXFORM;
    } else if(tag->id == ST_PLACEOBJECT2) {
        flags = swf_GetU8(tag);
        memset(obj,0,sizeof(SWFPLACEOBJECT));
            
        swf_GetMatrix(0,&obj->matrix);
        swf_GetCXForm(0,&obj->cxform,1);

        obj->depth = swf_GetU16(tag);
	//obj->internal = flags;
        if(flags&1) obj->move = 1;
        if(flags&2) obj->id = swf_GetU16(tag);
        if(flags&4) swf_GetMatrix(tag, &obj->matrix);
        if(flags&8) swf_GetCXForm(tag, &obj->cxform,1);
        if(flags&16) obj->ratio = swf_GetU16(tag);
        /* if you modify the order of these operations, also
           modify it in ../src/swfcombine.c */
        if(flags&64) 
            obj->clipdepth = swf_GetU16(tag); //clip
        if(flags&32) {
            int l,t;
            U8*data;
            swf_ResetReadBits(tag);
            l = strlen(&tag->data[tag->pos]);
            t = 0;
            data = rfx_alloc(l+1);
            obj->name = data;
            while((data[t++] = swf_GetU8(tag))); 
        }

        /* Actionscript ignored (for now) */
        obj->actions = 0;
    } else {
        fprintf(stderr, "rfxswf: Bad Tag: %d not a placeobject\n", tag->id);
    }
}

void swf_PlaceObjectFree(SWFPLACEOBJECT* obj)
{
    if(obj->name)
	rfx_free(obj->name);
}

