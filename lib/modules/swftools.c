/* swftools.c

   Math and matrix functions, misc tools

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
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

// Matrix & Math tools for SWF files

#include "../rfxswf.h"

#define S64 long long
SFIXED RFXSWF_SP(SFIXED a1,SFIXED a2,SFIXED b1,SFIXED b2)
{ S64 a = ((S64)a1*(S64)b1+(S64)a2*(S64)b2)>>16;
  SFIXED result = (SFIXED)(a);
  if(a!=result) 
      fprintf(stderr, "Warning: overflow in matrix multiplication\n");
  return result;
}
SFIXED RFXSWF_QFIX(int zaehler,int nenner) // bildet Quotient von zwei INTs in SFIXED
{ S64 z = zaehler<<16;
  S64 a = z/(S64)nenner;
  return (SFIXED)a;
}
#undef S64

MATRIX * swf_MatrixJoin(MATRIX * d,MATRIX * s1,MATRIX * s2)
{        
  if (!d) return NULL;
  if (!s1) return (s2)?(MATRIX *)memcpy(d,s2,sizeof(MATRIX)):NULL;
  if (!s2) return (MATRIX *)memcpy(d,s1,sizeof(MATRIX));
  
  d->tx = s1->tx + RFXSWF_SP(s1->sx,s1->r1,s2->tx,s2->ty);
  d->ty = s1->ty + RFXSWF_SP(s1->r0,s1->sy,s2->tx,s2->ty);
  
  d->sx = RFXSWF_SP(s1->sx,s1->r1,s2->sx,s2->r0);
  d->r0 = RFXSWF_SP(s1->r0,s1->sy,s2->sx,s2->r0);

  d->r1 = RFXSWF_SP(s1->sx,s1->r1,s2->r1,s2->sy);
  d->sy = RFXSWF_SP(s1->r0,s1->sy,s2->r1,s2->sy);

  //DumpMatrix(NULL,d);
  
  return d;
}

MATRIX * swf_MatrixMapTriangle(MATRIX * m,int dx,int dy,int x0,int y0,
                               int x1,int y1,int x2,int y2)
{ int dx1 = x1 - x0;
  int dy1 = y1 - y0;
  int dx2 = x2 - x0;
  int dy2 = y2 - y0;
  
  if (!m) return NULL;
  if ((!dx)||(!dy)) return NULL; // check DIV by zero

  m->tx = x0;
  m->ty = y0;
  m->sx = RFXSWF_QFIX(dx1,dx);
  m->sy = RFXSWF_QFIX(dy2,dy);
  m->r0 = RFXSWF_QFIX(dy1,dx);
  m->r1 = RFXSWF_QFIX(dx2,dy);
  
  return m;
}

void swf_SetDefineID(TAG * tag, U16 newid)
{
  int oldlen = tag->len;
  tag->len = 0;
  swf_SetU16(tag, newid); /* set defining ID */
  tag->len = oldlen;
}

U16 swf_GetDefineID(TAG * t)
// up to SWF 4.0
{ U32 oldTagPos;
  U16 id = 0;

  oldTagPos = swf_GetTagPos(t);
  swf_SetTagPos(t,0);

  switch (swf_GetTagID(t))
  { case ST_DEFINESHAPE:
    case ST_DEFINESHAPE2:
    case ST_DEFINESHAPE3:
    case ST_DEFINESHAPE4:
    case ST_DEFINEMORPHSHAPE:
    case ST_DEFINEMORPHSHAPE2:
    case ST_DEFINEEDITTEXT:
    case ST_DEFINEBITS:
    case ST_DEFINEBITSJPEG2:
    case ST_DEFINEBITSJPEG3:
    case ST_DEFINEBITSLOSSLESS:
    case ST_DEFINEBITSLOSSLESS2:
    case ST_DEFINESCALINGGRID: //pseudodefine
    case ST_DEFINEBUTTON:
    case ST_DEFINEBUTTON2:
    case ST_DEFINEBUTTONCXFORM: //pseudodefine
    case ST_DEFINEBUTTONSOUND: //pseudodefine
    case ST_CSMTEXTSETTINGS: //pseudodefine
    case ST_DEFINEFONT:
    case ST_DEFINEFONT2:
    case ST_DEFINEFONT3:
    case ST_DEFINEFONTINFO: //pseudodefine
    case ST_DEFINEFONTINFO2: //pseudodefine
    case ST_DEFINEFONTALIGNZONES: //pseudodefine
    case ST_DEFINEFONTNAME: //pseudodefine
    case ST_DEFINETEXT:
    case ST_DEFINEBINARY:
    case ST_DEFINETEXT2:
    case ST_DEFINESOUND:
    case ST_DEFINESPRITE:
    case ST_DEFINEMOVIE:
    case ST_DEFINEVIDEOSTREAM:
    case ST_GLYPHNAMES: //pseudodefine
    case ST_VIDEOFRAME: //pseudodefine
    case ST_NAMECHARACTER: //pseudodefine
    case ST_DOINITACTION: //pseudodefine
      id = swf_GetU16(t);
      break;
    default:
      fprintf(stderr, "rfxswf: Error: tag %d (%s) has no id\n", t->id, swf_TagGetName(t));
  }

  swf_SetTagPos(t,oldTagPos);

  return id;
}

SRECT swf_GetDefineBBox(TAG * t)
{
  U32 oldTagPos;
  U16 id = 0;
  SRECT b1,b2;
  memset(&b1, 0, sizeof(b1));

  oldTagPos = swf_GetTagPos(t);
  swf_SetTagPos(t,0);

  swf_GetRect(0, &b1);

  switch (swf_GetTagID(t))
  { case ST_DEFINESHAPE:
    case ST_DEFINESHAPE2:
    case ST_DEFINESHAPE3:
    case ST_DEFINESHAPE4:
    case ST_DEFINEEDITTEXT:
    case ST_DEFINETEXT:
    case ST_DEFINETEXT2:
    case ST_DEFINEVIDEOSTREAM:
      id = swf_GetU16(t);
      swf_GetRect(t, &b1);
      break;
    case ST_DEFINEMORPHSHAPE:
      id = swf_GetU16(t);
      swf_GetRect(t, &b1);
      swf_GetRect(t, &b2);
      swf_ExpandRect2(&b1, &b2);
      break;
    case ST_DEFINEBITSLOSSLESS:
    case ST_DEFINEBITSLOSSLESS2:
    case ST_DEFINEBITS:
    case ST_DEFINEBITSJPEG2:
    case ST_DEFINEBITSJPEG3:
      // FIXME
      break;
  }

  swf_SetTagPos(t,oldTagPos);

  return b1;
}

U16 swf_GetPlaceID(TAG * t)
// up to SWF 4.0
{ U32 oldTagPos;
  U16 id = 0;

  oldTagPos = swf_GetTagPos(t);
  swf_SetTagPos(t,0);

  switch (swf_GetTagID(t))
  { case ST_PLACEOBJECT:
    case ST_REMOVEOBJECT:
    case ST_FREECHARACTER:
    case ST_STARTSOUND:
      id = swf_GetU16(t);
      break;

    case ST_PLACEOBJECT2:
    { U8 flags = swf_GetU8(t);
      U16 d = swf_GetU16(t);
      id = (flags&PF_CHAR)?swf_GetU16(t):id;
    } break;
    case ST_PLACEOBJECT3:
    { U8 flags = swf_GetU8(t);
      U8 flags2 = swf_GetU8(t);
      U16 d = swf_GetU16(t);
      id = (flags&PF_CHAR)?swf_GetU16(t):id;
    } break;

  }

  swf_SetTagPos(t,oldTagPos);

  return id;
}

static int swf_definingtagids[] =
{ST_DEFINESHAPE,
 ST_DEFINESHAPE2,
 ST_DEFINESHAPE3,
 ST_DEFINESHAPE4,
 ST_DEFINEMORPHSHAPE,
 ST_DEFINEMORPHSHAPE2,
 ST_DEFINEFONT,
 ST_DEFINEFONT2,
 ST_DEFINEFONT3,
 ST_DEFINETEXT,
 ST_DEFINETEXT2,
 ST_DEFINEEDITTEXT,
 ST_DEFINEBITS,
 ST_DEFINEBITSJPEG2,
 ST_DEFINEBITSJPEG3,
 ST_DEFINEBITSLOSSLESS,
 ST_DEFINEBITSLOSSLESS2,
 ST_DEFINEMOVIE,
 ST_DEFINESPRITE,
 ST_DEFINEBUTTON,
 ST_DEFINEBUTTON2,
 ST_DEFINESOUND,
 ST_DEFINEVIDEOSTREAM,
 ST_DEFINEBINARY,
 -1
};

// tags which may be used inside a sprite definition
static int swf_spritetagids[] =
{ST_SHOWFRAME,
 ST_PLACEOBJECT,
 ST_PLACEOBJECT2,
 ST_PLACEOBJECT3,
 ST_REMOVEOBJECT,
 ST_REMOVEOBJECT2,
 ST_DOACTION,
 ST_DOABC,
 ST_STARTSOUND,
 ST_FRAMELABEL,
 ST_SOUNDSTREAMHEAD,
 ST_SOUNDSTREAMHEAD2,
 ST_SOUNDSTREAMBLOCK,
 ST_END,
 -1
};

/* tags which add content or information to a character with a given ID */
static int swf_pseudodefiningtagids[] = 
{
 ST_DEFINEFONTINFO,
 ST_DEFINEFONTINFO2,
 ST_DEFINEFONTALIGNZONES,
 ST_DEFINEFONTNAME,
 ST_DEFINEBUTTONCXFORM,
 ST_DEFINEBUTTONSOUND,
 ST_DEFINESCALINGGRID,
 ST_CSMTEXTSETTINGS,
 ST_NAMECHARACTER,
 ST_DOINITACTION,
 ST_VIDEOFRAME,
 ST_GLYPHNAMES,
 -1
};

U8 swf_isAllowedSpriteTag(TAG * tag)
{
    int id = tag->id;
    int t=0;
    while(swf_spritetagids[t]>=0)
    {
        if(swf_spritetagids[t] == id) 
            return 1;
        t++;
    }
    return 0; 
}

U8 swf_isDefiningTag(TAG * tag)
{
    int id = tag->id;
    int t=0;
    while(swf_definingtagids[t]>=0)
    {
        if(swf_definingtagids[t] == id) 
            return 1;
        t++;
    }
    return 0; 
}

U8 swf_isPseudoDefiningTag(TAG * tag)
{
    int id = tag->id;
    int t=0;
    while(swf_pseudodefiningtagids[t]>=0)
    {
        if(swf_pseudodefiningtagids[t] == id) 
            return 1;
        t++;
    }
    return 0; 
}

int swf_GetDepth(TAG * t)
{ 
  int depth = -1;
  U32 oldTagPos;
  oldTagPos = swf_GetTagPos(t);
  swf_SetTagPos(t,0);

  switch (swf_GetTagID(t))
  { case ST_PLACEOBJECT:
    case ST_REMOVEOBJECT:
      swf_GetU16(t); //id
      depth = swf_GetU16(t);
      break;
    case ST_REMOVEOBJECT2:
      depth = swf_GetU16(t);
      break;
    case ST_PLACEOBJECT2:
    { U8 flags = swf_GetU8(t);
      depth = swf_GetU16(t);
    } break;
    case ST_PLACEOBJECT3:
    { U8 flags = swf_GetU8(t);
      U8 flags2 = swf_GetU8(t);
      depth = swf_GetU16(t);
    } break;
    case ST_SETTABINDEX:
    {
      depth = swf_GetU16(t);
    }
  }
  swf_SetTagPos(t,oldTagPos);
  return depth;
}

void swf_SetDepth(TAG * t, U16 depth)
{ 
  switch (swf_GetTagID(t))
  { case ST_PLACEOBJECT:
    case ST_REMOVEOBJECT:
      PUT16(t->data, depth);
      break;
    case ST_REMOVEOBJECT2:
      PUT16(t->data, depth);
      break;
    case ST_PLACEOBJECT2:
      PUT16(&t->data[1], depth);
      break;
    case ST_SETTABINDEX:
      PUT16(t->data, depth);
      break;
    default:
      fprintf(stderr, "rfxswf: Error: tag %d has no depth\n", t->id);
  }
}

char* swf_GetName(TAG * t)
{
    char* name = 0;
    U32 oldTagPos;
    MATRIX m;
    CXFORM c;
    oldTagPos = swf_GetTagPos(t);
    swf_SetTagPos(t,0);
    switch(swf_GetTagID(t))
    {
        case ST_FRAMELABEL:
            name = (char*)&t->data[swf_GetTagPos(t)];
        break;
        case ST_PLACEOBJECT3:
        case ST_PLACEOBJECT2: {   
            U8 flags = swf_GetU8(t);
	    if(t->id == ST_PLACEOBJECT3)
		swf_GetU8(t);
            swf_GetU16(t); //depth;
            if(flags&PF_CHAR) 
              swf_GetU16(t); //id
            if(flags&PF_MATRIX)
              swf_GetMatrix(t, &m);
            if(flags&PF_CXFORM)
              swf_GetCXForm(t, &c, 1);
            if(flags&PF_RATIO)
              swf_GetU16(t);
            if(flags&PF_CLIPDEPTH)
              swf_GetU16(t);
            if(flags&PF_NAME) {
              swf_ResetReadBits(t);
              name = (char*)&t->data[swf_GetTagPos(t)];
            }
        }
        break;
    }
    swf_SetTagPos(t,oldTagPos);
    return name;
}

/* used in enumerateUsedIDs */
void swf_GetMorphGradient(TAG * tag, GRADIENT * gradient1, GRADIENT * gradient2)
{
    int t;
    int num = swf_GetU8(tag) & 15;
    if(gradient1) gradient1->num = num;
    if(gradient2) gradient2->num = num;
    
    if(gradient1) {
	gradient1->num = num;
	gradient1->rgba = (RGBA*)rfx_calloc(sizeof(RGBA)*gradient1->num);
	gradient1->ratios = (U8*)rfx_calloc(sizeof(gradient1->ratios[0])*gradient1->num);
    }
    if(gradient2) {
	gradient2->num = num;
	gradient2->rgba = (RGBA*)rfx_calloc(sizeof(RGBA)*gradient2->num);
	gradient2->ratios = (U8*)rfx_calloc(sizeof(gradient2->ratios[0])*gradient2->num);
    }
    for(t=0;t<num;t++)
    {
	U8 ratio;
	RGBA color;
	
	ratio = swf_GetU8(tag);
	swf_GetRGBA(tag, &color);
	if(gradient1) {
	    gradient1->ratios[t] = ratio;
	    gradient1->rgba[t] = color;
	}

	ratio = swf_GetU8(tag);
	swf_GetRGBA(tag, &color);
	if(gradient2) {
	    gradient2->ratios[t] = ratio;
	    gradient2->rgba[t] = color;
	}
    }
}

#define DEBUG_ENUMERATE if(0)
//#define DEBUG_ENUMERATE

void enumerateUsedIDs_fillstyle(TAG * tag, int t, void (*callback)(TAG*, int, void*), void*callback_data, int num, int morph)
{
    int type;
    type = swf_GetU8(tag); //type
    DEBUG_ENUMERATE printf("fill style %d) type=%02x (tagpos=%d)\n", t, type, tag->pos);
    if(type == 0) {
	RGBA color;
	if(num >= 3)
	    {swf_GetRGBA(tag, &color);if(morph) swf_GetRGBA(tag, NULL);}
	else 
	    {swf_GetRGB(tag, &color);if(morph) swf_GetRGB(tag, NULL);}
	DEBUG_ENUMERATE printf("               %02x%02x%02x%02x\n", color.r,color.g,color.b,color.a);
    }
    else if(type == 0x10 || type == 0x12 || type == 0x13)
    {
	swf_ResetReadBits(tag);
	MATRIX m;
	swf_GetMatrix(tag, &m);
	DEBUG_ENUMERATE swf_DumpMatrix(stdout, &m);
	if(morph) {
	    swf_GetMatrix(tag, &m);
	    DEBUG_ENUMERATE swf_DumpMatrix(stdout, &m);
	}
	swf_ResetReadBits(tag);
	if(morph) {
	    swf_GetMorphGradient(tag, NULL, NULL);
	    if(type == 0x13) {
		swf_GetU16(tag);
		swf_GetU16(tag);
	    }
	} else {
	    GRADIENT g;
	    swf_GetGradient(tag, &g, /*alpha*/ num>=3?1:0);
	    DEBUG_ENUMERATE swf_DumpGradient(stdout, &g);
	    if(type == 0x13)
		swf_GetU16(tag);
	}
    }
    else if(type == 0x40 || type == 0x41 || type == 0x42 || type == 0x43)
    {
	swf_ResetReadBits(tag);
	if(tag->data[tag->pos] != 0xff ||
	   tag->data[tag->pos+1] != 0xff)
	(callback)(tag, tag->pos, callback_data);

	swf_GetU16(tag);
	swf_ResetReadBits(tag);
	swf_GetMatrix(tag, NULL);
	if(morph)
	    swf_GetMatrix(tag, NULL);
    }
    else {
	fprintf(stderr, "rfxswf:swftools.c Unknown fillstyle:0x%02x in tag %02d\n",type, tag->id);
    }
}

void enumerateUsedIDs_linestyle(TAG * tag, int t, void (*callback)(TAG*, int, void*), void*callback_data, int num, int morph)
{
    U16  width;
    RGBA color;
    width = swf_GetU16(tag);
    char fill=0;
    if(morph)
	swf_GetU16(tag);
    if(num >= 4) {
	U16 flags = swf_GetU16(tag);
	DEBUG_ENUMERATE printf("line style %d) flags: %08x\n", t, flags);
	if((flags & 0x30) == 0x20) {
	    U16 miter = swf_GetU16(tag); // miter limit
	    DEBUG_ENUMERATE printf("line style %d) miter join: %08x\n", t, miter);
	}
	if(flags & 0x08) {
	    fill = 1;
	}
    }
    if(!fill) {
	if(num >= 3)
	    {swf_GetRGBA(tag, &color);if(morph) swf_GetRGBA(tag, NULL);}
	else
	    {swf_GetRGB(tag, &color);if(morph) swf_GetRGB(tag, NULL);}
    } else {
	enumerateUsedIDs_fillstyle(tag, t, callback, callback_data, num, morph);
    }
    DEBUG_ENUMERATE printf("line style %d) width=%.2f color=%02x%02x%02x%02x \n", t, width/20.0, color.r,color.g,color.b,color.a);
}

void enumerateUsedIDs_styles(TAG * tag, void (*callback)(TAG*, int, void*), void*callback_data, int num, int morph)
{
    U16 count;
    int t;
    count = swf_GetU8(tag);
    if(count == 0xff && num>1) // defineshape2,3,4 only
	count = swf_GetU16(tag);

    DEBUG_ENUMERATE printf("%d fill styles\n", count);
    for(t=0;t<count;t++)
    {
	enumerateUsedIDs_fillstyle(tag, t, callback, callback_data, num, morph);
    }
    swf_ResetReadBits(tag);
    count = swf_GetU8(tag); // line style array
    if(count == 0xff)
	count = swf_GetU16(tag);
    DEBUG_ENUMERATE printf("%d line styles\n", count);
    for(t=0;t<count;t++) 
    {
	enumerateUsedIDs_linestyle(tag, t, callback, callback_data, num, morph);
    }
}

void enumerateUsedIDs(TAG * tag, int base, void (*callback)(TAG*, int, void*), void*callback_data)
{
    int num = 1;
    swf_ResetReadBits(tag);
    tag->pos = 0;
    switch(tag->id)
    {
	case ST_DEFINEBUTTONSOUND: {
	    int t;
	    callback(tag, tag->pos + base, callback_data);
	    swf_GetU16(tag); //button id
	    for(t=0;t<4;t++) {
		int flags;
		callback(tag, tag->pos + base, callback_data);
		U16 sound_id = swf_GetU16(tag); //sound id
		if(!sound_id)
		    continue;
		flags = swf_GetU8(tag);
		if(flags&1)
		    swf_GetU32(tag); // in point
		if(flags&2)
		    swf_GetU32(tag); // out points
		if(flags&4)
		    swf_GetU16(tag); // loop count
		if(flags&8)
		{
		    int npoints = swf_GetU8(tag);
		    int s;
		    for(s=0;s<npoints;s++)
		    {
			swf_GetU32(tag);
			swf_GetU16(tag);
			swf_GetU16(tag);
		    }
		}
	    }
        } break;
	case ST_DEFINEBUTTONCXFORM:
	    callback(tag, tag->pos + base, callback_data); //button id
	break;

	case ST_SYMBOLCLASS:
	case ST_EXPORTASSETS: {
	    int num =  swf_GetU16(tag);
	    int t;
	    for(t=0;t<num;t++) {
		callback(tag, tag->pos + base, callback_data); //button id
		swf_GetU16(tag); //id
		while(swf_GetU8(tag)); //name
	    }
	} break;

	case ST_IMPORTASSETS: 
	case ST_IMPORTASSETS2: {
	    swf_GetString(tag); //count
	    swf_GetU8(tag); //reserved
	    swf_GetU8(tag); //reserved
	    int num =  swf_GetU16(tag); //url
	    int t;
	    for(t=0;t<num;t++) {
		callback(tag, tag->pos + base, callback_data); //button id
		swf_GetU16(tag); //id
		while(swf_GetU8(tag)); //name
	    }
	} break;

        case ST_DOABC:
        case ST_RAWABC:
        break;

	case ST_FREECHARACTER: /* unusual tags, which all start with an ID */
	case ST_NAMECHARACTER:
	case ST_DEFINEFONTNAME:
	case ST_GENERATORTEXT:
	    callback(tag, tag->pos + base, callback_data);
        break;
	case ST_PLACEOBJECT:
	    callback(tag, tag->pos + base, callback_data);
        break;
	case ST_PLACEOBJECT2:
	    // only if placeflaghascharacter
	    if(!(tag->data[0]&2))
		break;
	    callback(tag, 3 + base, callback_data);
        break;
	case ST_PLACEOBJECT3:
	    // only if placeflaghascharacter
	    if(!(tag->data[0]&2))
		break;
	    callback(tag, 4 + base, callback_data);
        break;
	case ST_REMOVEOBJECT:
	    callback(tag, tag->pos + base, callback_data);
	break;
	case ST_STARTSOUND:
	    callback(tag, tag->pos + base, callback_data);
	break;
	case ST_DEFINESPRITE: {
	    if(tag->len <= 4)
		break; // sprite is expanded

	    swf_GetU16(tag); // id
	    swf_GetU16(tag); // framenum

	    while(1) {
		U16 flags = swf_GetU16(tag);
		U32 len;
		U16 id = flags>>6;
		TAG *tag2 = swf_InsertTag(NULL, id);
		len = flags&0x3f;
		if(len == 63)
		    len = swf_GetU32(tag);
		if(id == ST_END)
		    break;
		tag2->len = tag2->memsize = len;
		tag2->data = (U8*)rfx_alloc(len);
		memcpy(tag2->data, &tag->data[tag->pos], len);
		/* I never saw recursive sprites, but they are (theoretically) 
		   possible, so better add base here again */
		enumerateUsedIDs(tag2, tag->pos + base, callback, callback_data);
		swf_DeleteTag(0, tag2);
		swf_GetBlock(tag, NULL, len);
	    }
	} 
	break;
	case ST_DEFINEBUTTON2: // has some font ids in the button records
	    num++; 
	//fallthrough
	case ST_DEFINEBUTTON: {
	    swf_GetU16(tag); //button id
	    if(num>1)
	    { 
		int offset;
		swf_GetU8(tag); //flag
		offset = swf_GetU16(tag); //offset
	    }
	    while(1)
	    {
		U8 flags = swf_GetU8(tag);
		if(!flags) //flags
		    break; 
		callback(tag, tag->pos + base, callback_data);
		swf_GetU16(tag); //char
		swf_GetU16(tag); //layer
		swf_ResetReadBits(tag);
		swf_GetMatrix(tag, NULL);
		if(num>1) {
		  swf_ResetReadBits(tag);
		  swf_GetCXForm(tag, NULL, 1);
		}
		if(flags&0x10) {
		    U8 num = swf_GetU8(tag);
		    int t;
		    for(t=0;t<num;t++) {
			swf_DeleteFilter(swf_GetFilter(tag));
		    }
		}
		if(flags&0x20) {
		    U8 blendmode = swf_GetU8(tag);
		}
	    }
	    // ...
	}
	break;
	case ST_DEFINEEDITTEXT:  {
	    U8 flags1,flags2;
	    swf_GetU16(tag); //id
	    swf_GetRect(tag, NULL); //bounding box
	    swf_ResetReadBits(tag);
	    flags1 = swf_GetU8(tag);
	    flags2 = swf_GetU8(tag);
	    if(flags1 & 1)
		callback(tag, tag->pos + base, callback_data);
	}
	break;
	case ST_DEFINETEXT2:
	    num ++;
	case ST_DEFINETEXT: { 
	    int glyphbits, advancebits;
	    int id;
	    id = swf_GetU16(tag); //id
	    swf_GetRect(tag, NULL); //bounding box
	    swf_ResetReadBits(tag);
	    swf_GetMatrix(tag, NULL); //matrix
	    swf_ResetReadBits(tag);
	    glyphbits = swf_GetU8(tag); //glyphbits
	    advancebits = swf_GetU8(tag); //advancebits
	    while(1) {
		U16 flags;
		int t;
		swf_ResetReadBits(tag);
		flags = swf_GetBits(tag, 8);
		if(!flags) break;
		
		swf_ResetReadBits(tag);
		if(flags & 8) { // hasfont
		    callback(tag, tag->pos + base, callback_data);
		    id = swf_GetU16(tag);
		}
		if(flags & 4) { // hascolor
		    if(num==1) swf_GetRGB(tag, NULL);
		    else       swf_GetRGBA(tag, NULL);
		}
		if(flags & 2) { //has x offset
		    swf_ResetReadBits(tag);
		    swf_GetU16(tag);
		}
		if(flags & 1) { //has y offset
		    swf_ResetReadBits(tag);
		    swf_GetU16(tag);
		}
		if(flags & 8) { //has height
		    swf_ResetReadBits(tag);
		    swf_GetU16(tag);
		}
		
		flags = swf_GetBits(tag, 8);
		if(!flags) break;
		swf_ResetReadBits(tag);
		for(t=0;t<flags;t++) {
		    swf_GetBits(tag, glyphbits);
		    swf_GetBits(tag, advancebits);
		}
	    }
	    break;
	}
	case ST_DEFINEFONTALIGNZONES:
	case ST_DEFINESCALINGGRID:
	case ST_GLYPHNAMES:
	case ST_CSMTEXTSETTINGS:
	case ST_DEFINEFONTINFO:
	case ST_DEFINEFONTINFO2:
	case ST_VIDEOFRAME:
	    callback(tag, tag->pos + base, callback_data);
	break;
	case ST_DEFINEVIDEOSTREAM:
	break;

	case ST_DOINITACTION:
	    callback(tag, tag->pos + base, callback_data);
	break;

	case ST_DEFINEMORPHSHAPE2:
	case ST_DEFINESHAPE4:
	num++;
	case ST_DEFINEMORPHSHAPE:
	case ST_DEFINESHAPE3:
	num++; //fallthrough
	case ST_DEFINESHAPE2:
	num++; //fallthrough
	case ST_DEFINESHAPE: {
	    int fillbits;
	    int linebits;
	    int id; 
	    int numshapes = 1;
	    int morph = 0;
	    if(tag->id == ST_DEFINEMORPHSHAPE || tag->id==ST_DEFINEMORPHSHAPE2) {
		numshapes = 2;
		morph = 1;
	    }

	    id = swf_GetU16(tag); // id;
	    SRECT r={0,0,0,0},r2={0,0,0,0};
	    swf_GetRect(tag, &r); // shape bounds
	    if(morph) {
		swf_ResetReadBits(tag);
		swf_GetRect(tag, NULL); // shape bounds2
		if(num>=4) {
		    swf_ResetReadBits(tag);
		    swf_GetRect(tag, NULL); // edge bounds1
		}
	    }
	    if(num>=4) {
		swf_ResetReadBits(tag);
		swf_GetRect(tag, &r2); // edge bounds
		U8 flags = swf_GetU8(tag); // flags, &1: contains scaling stroke, &2: contains non-scaling stroke
		DEBUG_ENUMERATE printf("flags: %02x (1=scaling strokes, 2=non-scaling strokes)\n", flags);
	    }
	    if(morph) {
		swf_GetU32(tag); //offset to endedges
	    }
   
	    DEBUG_ENUMERATE printf("Tag:%d Name:%s ID:%d\n", tag->id, swf_TagGetName(tag), id);
	    DEBUG_ENUMERATE printf("BBox %.2f %.2f %.2f %.2f\n", r.xmin/20.0,r.ymin/20.0,r.xmax/20.0,r.ymax/20.0);
	    DEBUG_ENUMERATE printf("BBox %.2f %.2f %.2f %.2f\n", r2.xmin/20.0,r2.ymin/20.0,r2.xmax/20.0,r2.ymax/20.0);

	    DEBUG_ENUMERATE printf("style tag pos: %d\n", tag->pos);
	    enumerateUsedIDs_styles(tag, callback, callback_data, num, morph);
	    DEBUG_ENUMERATE printf("-------\n");
	    swf_ResetReadBits(tag);
	    while(--numshapes>=0) /* morph shapes define two shapes */
	    {
		DEBUG_ENUMERATE printf("shape:%d\n", numshapes);
		fillbits = swf_GetBits(tag, 4);
		linebits = swf_GetBits(tag, 4);
		DEBUG_ENUMERATE printf("fillbits=%d linebits=%d\n", fillbits, linebits);
		swf_ResetReadBits(tag);
		int x=0,y=0;
		while(1) {
		    int flags;
		    flags = swf_GetBits(tag, 1);
		    if(!flags) { //style change
			flags = swf_GetBits(tag, 5);
			if(!flags)
			    break;
			if(flags&1) { //move
			    int n = swf_GetBits(tag, 5); 
			    x = swf_GetBits(tag, n); //x
			    y = swf_GetBits(tag, n); //y
			    DEBUG_ENUMERATE printf("moveTo %.2f %.2f\n",x/20.0,y/20.0);
			}
			if(flags&2) { //fill0
			    int fill0;
			    fill0 = swf_GetBits(tag, fillbits); 
			    DEBUG_ENUMERATE printf("fill0 %d\n", fill0);
			}
			if(flags&4) { //fill1
			    int fill1;
			    fill1 = swf_GetBits(tag, fillbits); 
			    DEBUG_ENUMERATE printf("fill1 %d\n", fill1);
			}
			if(flags&8) { //linestyle
			    int line;
			    line = swf_GetBits(tag, linebits); 
			    DEBUG_ENUMERATE printf("linestyle %d\n",line);
			}
			if(flags&16) {
			    DEBUG_ENUMERATE printf("more fillstyles\n");
			    enumerateUsedIDs_styles(tag, callback, callback_data, num, 0);
			    fillbits = swf_GetBits(tag, 4);
			    linebits = swf_GetBits(tag, 4);
			}
		    } else {
			flags = swf_GetBits(tag, 1);
			if(flags) { //straight edge
			    int n = swf_GetBits(tag, 4) + 2;
			    if(swf_GetBits(tag, 1)) { //line flag
				x += swf_GetSBits(tag, n); //delta x
				y += swf_GetSBits(tag, n); //delta y
				DEBUG_ENUMERATE printf("lineTo %.2f %.2f\n",x/20.0,y/20.0);
			    } else {
				int v=swf_GetBits(tag, 1);
				int d;
				d = swf_GetSBits(tag, n); //vert/horz
				if(!v)
				    x += d;
				else
				    y += d;
				DEBUG_ENUMERATE printf("lineTo %.2f %.2f (%s)\n",x/20.0,y/20.0, v?"vertical":"horizontal");
			    }
			} else { //curved edge
			    int n = swf_GetBits(tag, 4) + 2;
			    int x1,y1,x2,y2;
			    x1 = swf_GetSBits(tag, n);
			    y1 = swf_GetSBits(tag, n);
			    x2 = swf_GetSBits(tag, n);
			    y2 = swf_GetSBits(tag, n);
			    DEBUG_ENUMERATE printf("splineTo %.2f %.2f %.2f %.2f\n", x1/20.0, y1/20.0, x2/20.0, y2/20.0);
			}
		    }
		}
	    }
	}
	break;
	default:
	break;
    }
}

void callbackCount(TAG * t,int pos, void*ptr)
{
    (*(int*)ptr)++;
    DEBUG_ENUMERATE printf("callback(%d) %d\n", pos, *(U16*)&t->data[pos]);
}

void callbackFillin(TAG * t,int pos, void*ptr)
{
    **(int**)ptr = pos;
    (*(int**)ptr)++;
    DEBUG_ENUMERATE printf("callback(%d) %d\n", pos, *(U16*)&t->data[pos]);
}

int swf_GetNumUsedIDs(TAG * t)
{
    int num = 0;
    enumerateUsedIDs(t, 0, callbackCount, &num);
    return num;
}

void swf_GetUsedIDs(TAG * t, int * positions)
{
    int * ptr = positions;
    enumerateUsedIDs(t, 0, callbackFillin, &ptr);
}

char swf_Relocate (SWF*swf, char*bitmap)
{
    TAG*tag;
    int slaveids[65536];
    memset(slaveids, -1, sizeof(slaveids));
    tag = swf->firstTag;
    char ok = 1;

    int current_id=0;
#define NEW_ID(n) \
		for(current_id++;current_id<65536;current_id++) { \
		    if(!bitmap[current_id]) { \
			n = current_id; \
			break; \
		    } \
		} \
                if(current_id==65536) { \
                    fprintf(stderr, "swf_Relocate: Couldn't relocate: Out of IDs\n"); \
                    return 0; \
                }

    while(tag)
    {
	int num; 
	int *ptr;

	if(swf_isDefiningTag(tag))
	{
	    int newid;
	    int id;
	    
	    id = swf_GetDefineID(tag); //own id

	    if(!bitmap[id]) { //free
		newid = id;
	    } else if(slaveids[id]>0) {
		newid = slaveids[id];
	    } else {
		NEW_ID(newid);
	    }

	    bitmap[newid] = 1;
	    slaveids[id] = newid;

	    swf_SetDefineID(tag, newid);
	} 
	
	num = swf_GetNumUsedIDs(tag);
	if(num) {
	    ptr = (int*)rfx_alloc(sizeof(int)*num);
	    swf_GetUsedIDs(tag, ptr);
	    int t;
	    for(t=0;t<num;t++) {
		int id = GET16(&tag->data[ptr[t]]);
		if(slaveids[id]<0) {
		    if(!id && bitmap[id]) {
			/* id 0 is only used in SWF versions >=9. It's the ID of
			   the main timeline. It's used in e.g. SYMBOLTAG tags, but
			   never defined, so if we're asked to reallocate it, we have 
			   to allocate an ID for it on the fly. */
			int newid;
			NEW_ID(newid);
			bitmap[newid] = 1;
			slaveids[id] = newid;
			id = newid;
		    } else if(!bitmap[id]) {
			/* well- we don't know this id, but it's not reserved anyway, so just
			   leave it alone */
		    } else {
			/* this actually happens with files created with Flash CS4 and never.
			   Apparently e.g. DefineButton tags are able to use forward declarations of objects. */
			fprintf(stderr, "warning: Mapping id (%d) never encountered before in %s\n", id,
				swf_TagGetName(tag));
			int newid;
			NEW_ID(newid);
			id = slaveids[id] = newid;
			ok = 0;
		    }
		} else {
		    id = slaveids[id];
		}
		PUT16(&tag->data[ptr[t]], id);
	    }
            free(ptr);
	}
	tag=tag->next;
    }
    return ok;
}

/* untested */
void swf_Relocate2(SWF*swf, int*id2id)
{
    TAG*tag;
    tag = swf->firstTag;
    while(tag) {
	if(swf_isDefiningTag(tag)) {
	    int id = swf_GetDefineID(tag);
	    id = id2id[id];
	    if(id>=0) {
		swf_SetDefineID(tag, id);
	    }
	}
	int num = swf_GetNumUsedIDs(tag);
	if(num) {
	    int *ptr;
	    int t;
	    ptr = (int*)rfx_alloc(sizeof(int)*num);
	    swf_GetUsedIDs(tag, ptr);
	    for(t=0;t<num;t++) {
		int id = GET16(&tag->data[ptr[t]]);
		id = id2id[id];
		if(id>=0) {
		    PUT16(&tag->data[ptr[t]], id);
		}
	    }
            free(ptr);
	}
    }
}

void swf_RelocateDepth(SWF*swf, char*bitmap)
{
    TAG*tag;
    int nr;
    tag = swf->firstTag;
    for(nr=65535;nr>=0;nr--) {
	if(bitmap[nr] != 0) 
	    break;
    }
    // now nr is the highest used depth. So we start
    // assigning depths at nr+1
    nr++;

    while(tag)
    {
	int depth;
	/* TODO * clip depths 
	        * sprites
	 */
	if(tag->id == ST_PLACEOBJECT2) {
	    SWFPLACEOBJECT obj;
	    swf_GetPlaceObject(tag, &obj);
	    if(obj.clipdepth) {
		int newdepth = obj.clipdepth+nr;
		if(newdepth>65535) {
		    fprintf(stderr, "Couldn't relocate depths: too large values\n");
		    newdepth = 65535;
		}
		obj.clipdepth = newdepth;
		swf_ResetTag(tag, ST_PLACEOBJECT2);
		swf_SetPlaceObject(tag, &obj);
	    }
	    swf_PlaceObjectFree(&obj);
	}

	depth = swf_GetDepth(tag);
	if(depth>=0) {
	    int newdepth = depth+nr;
	    if(newdepth>65535) {
		fprintf(stderr, "Couldn't relocate depths: too large values\n");
		newdepth = 65535;
	    }
	    swf_SetDepth(tag, newdepth);
	}
	tag=tag->next;
    }
}

U8 swf_isShapeTag(TAG*tag)
{
    if(tag->id == ST_DEFINESHAPE ||
       tag->id == ST_DEFINESHAPE2 ||
       tag->id == ST_DEFINESHAPE3 ||
       tag->id == ST_DEFINESHAPE4) 
        return 1;
    return 0;
}

U8 swf_isPlaceTag(TAG*tag)
{
    if(tag->id == ST_PLACEOBJECT ||
       tag->id == ST_PLACEOBJECT2 ||
       tag->id == ST_PLACEOBJECT3)
        return 1;
    return 0;
}
U8 swf_isTextTag(TAG*tag)
{
    if(tag->id == ST_DEFINETEXT ||
       tag->id == ST_DEFINETEXT2)
        return 1;
    return 0;
}

U8 swf_isFontTag(TAG*tag)
{
    if(tag->id == ST_DEFINEFONT ||
       tag->id == ST_DEFINEFONT2 ||
       tag->id == ST_DEFINEFONT3 ||
       tag->id == ST_DEFINEFONTINFO)
        return 1;
    return 0;
}

U8  swf_isImageTag(TAG*tag)
{
    if(tag->id == ST_DEFINEBITSJPEG || 
       tag->id == ST_DEFINEBITSJPEG2 || 
       tag->id == ST_DEFINEBITSJPEG3 ||
       tag->id == ST_DEFINEBITSLOSSLESS || 
       tag->id == ST_DEFINEBITSLOSSLESS2)
        return 1;
    return 0;
}

TAG* swf_Concatenate (TAG*list1,TAG*list2)
{
    TAG*tag=0,*lasttag=0;
    char bitmap[65536];
    char depthmap[65536];
    SWF swf1,swf2;
    memset(bitmap, 0, sizeof(bitmap));
    memset(depthmap, 0, sizeof(depthmap));
    memset(&swf1, 0, sizeof(swf1));
    memset(&swf2, 0, sizeof(swf2));

    swf1.firstTag = list1;
    swf_FoldAll(&swf1);
    swf2.firstTag = list2;
    swf_FoldAll(&swf2);

    tag = list1;
    while(tag) {
	if(!swf_isDefiningTag(tag)) {
	    int id = swf_GetDefineID(tag);
	    bitmap[id] = 1;
	}
	if(tag->id == ST_PLACEOBJECT ||
	   tag->id == ST_PLACEOBJECT2) {
	    int depth = swf_GetDepth(tag);
	    depthmap[depth] = 1;
	}
	if(tag->id == ST_REMOVEOBJECT ||
	   tag->id == ST_REMOVEOBJECT2) {
	    int depth = swf_GetDepth(tag);
	    depthmap[depth] = 0;
	}
	tag = tag->next;
	lasttag = tag;
    }
    swf_Relocate(&swf2, bitmap);
    swf_RelocateDepth(&swf2, depthmap);
    lasttag->next = swf2.firstTag;
    swf2.firstTag->prev = lasttag;

    return swf1.firstTag;
}

static int tagHash(TAG*tag)
{
    int t, h=0;
    unsigned int a = 0x6b973e5a;
    /* start at pos 2, as 0 and 1 are the id */
    for(t=2;t<tag->len;t++) {
        unsigned int b = a;
        a >>= 8;
        a += tag->data[t]*0xefbc35a5*b*(t+1);
    }
    return a&0x7fffffff; //always return positive number
}

void swf_Optimize(SWF*swf)
{
    const int hash_size = 131072;
    char* dontremap = (char*)rfx_calloc(sizeof(char)*65536);
    U16* remap = (U16*)rfx_alloc(sizeof(U16)*65536);
    TAG* id2tag = (TAG*)rfx_calloc(sizeof(TAG*)*65536);
    TAG** hashmap = (TAG**)rfx_calloc(sizeof(TAG*)*hash_size);
    TAG* tag;
    int t;
    for(t=0;t<65536;t++) {
        remap[t] = t;
    }

    swf_FoldAll(swf);

    tag = swf->firstTag;
    while(tag) {
        /* make sure we don't remap to this tag,
           as it might have different "helper tags" 
           FIXME: a better way would be to compare
                  the helper tags, too.
         */
        if(swf_isPseudoDefiningTag(tag) &&
           tag->id != ST_NAMECHARACTER) {
            dontremap[swf_GetDefineID(tag)] = 1;
        }
        tag=tag->next;
    }
    tag = swf->firstTag;
    while(tag) {
        TAG*next = tag->next;

        /* remap the tag */
        int num = swf_GetNumUsedIDs(tag);
        int*positions = (int*)rfx_alloc(sizeof(int)*num);
        int t;
        swf_GetUsedIDs(tag, positions);
        for(t=0;t<num;t++) {
            int id = GET16(&tag->data[positions[t]]);
            id = remap[id];
            PUT16(&tag->data[positions[t]], id);
        }
        rfx_free(positions);

        /* now look for previous tags with the same
           content */
        if(swf_isDefiningTag(tag)) {
            TAG*tag2;
            int id = swf_GetDefineID(tag);
            int hash = tagHash(tag);
            int match=0;
            if(!dontremap[id]) 
            while((tag2 = hashmap[hash%hash_size])) {
                if(tag2 != (TAG*)0 && tag->len == tag2->len) {
		    if(memcmp(&tag->data[2],&tag2->data[2],tag->len-2) == 0) {
			match = 1;
			break;
		    }
                }
                hash++;
            }
            if(!match) {
                while(hashmap[hash%hash_size]) hash++;
                hashmap[hash%hash_size] = tag;
            } else {
		/* we found two identical tags- remap one
		   of them */
                remap[id] = swf_GetDefineID(tag2);
                swf_DeleteTag(swf, tag);
            }
        } else if(swf_isPseudoDefiningTag(tag)) {
            int id = swf_GetDefineID(tag);
            if(remap[id]!=id) {
                /* if this tag was remapped, we don't
                   need the helper tag anymore. Discard
                   it. */
                swf_DeleteTag(swf, tag);
            }
        }

        tag = next;
    }
    
    rfx_free(dontremap);
    rfx_free(remap);
    rfx_free(id2tag);
    rfx_free(hashmap);
}

void swf_SetDefineBBox(TAG * tag, SRECT newbbox)
{
    U16 id = 0;
    SRECT b1;
    swf_SetTagPos(tag,0);

    switch (swf_GetTagID(tag))
    { 
	case ST_DEFINESHAPE:
	case ST_DEFINESHAPE2:
	case ST_DEFINESHAPE3:
	case ST_DEFINEEDITTEXT:
	case ST_DEFINETEXT:
	case ST_DEFINETEXT2:
	case ST_DEFINEVIDEOSTREAM: {
	      U32 after_bbox_offset = 0, len;
	      U8*data;
	      id = swf_GetU16(tag);
	      swf_GetRect(tag, &b1);
	      swf_ResetReadBits(tag);
	      after_bbox_offset = tag->pos;
	      len = tag->len - after_bbox_offset;
	      data = (U8*)malloc(len);
	      memcpy(data, &tag->data[after_bbox_offset], len);
	      tag->writeBit = 0;
	      tag->len = 2;
	      swf_SetRect(tag, &newbbox);
	      swf_SetBlock(tag, data, len);
	      free(data);
	      tag->pos = tag->readBit = 0;

	} break;
	default:
	    fprintf(stderr, "rfxswf: Tag %d (%s) has no bbox\n", tag->id, swf_TagGetName(tag));
    }
}

RGBA swf_GetSWFBackgroundColor(SWF*swf)
{
    TAG*t=swf->firstTag;
    RGBA color;
    color.r = color.b = color.g = 0;
    color.a = 255;
    while(t) {
	if(t->id == ST_SETBACKGROUNDCOLOR) {
	    swf_SetTagPos(t, 0);
	    color.r = swf_GetU8(t);
	    color.g = swf_GetU8(t);
	    color.b = swf_GetU8(t);
	    break;
	}
	t=t->next;
    }
    return color;
}

