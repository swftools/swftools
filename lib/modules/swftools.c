/* swftools.c

   Math and matrix functions, misc tools

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

// Matrix & Math tools for SWF files

#define S64 long long
SFIXED RFXSWF_SP(SFIXED a1,SFIXED a2,SFIXED b1,SFIXED b2)
{ S64 a;
  a = (S64)a1*(S64)b1+(S64)a2*(S64)b2;
  return (SFIXED)(a>>16);
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
  
  d->tx = s1->tx + s2->tx;
  d->ty = s1->ty + s2->ty;
  
  d->sx = RFXSWF_SP(s1->sx,s1->r1,s2->sx,s2->r0);
  d->sy = RFXSWF_SP(s1->r0,s1->sy,s2->r1,s2->sy);
  d->r0 = RFXSWF_SP(s1->r0,s1->sy,s2->sx,s2->r0);
  d->r1 = RFXSWF_SP(s1->sx,s1->r1,s2->r1,s2->sy);

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
    case ST_DEFINEMORPHSHAPE:
    case ST_DEFINEEDITTEXT:
    case ST_DEFINEBITS:
    case ST_DEFINEBITSJPEG2:
    case ST_DEFINEBITSJPEG3:
    case ST_DEFINEBITSLOSSLESS:
    case ST_DEFINEBITSLOSSLESS2:
    case ST_DEFINEBUTTON:
    case ST_DEFINEBUTTON2:
    case ST_DEFINEBUTTONCXFORM: //pseudodefine
    case ST_DEFINEBUTTONSOUND: //pseudodefine
    case ST_DEFINEFONT:
    case ST_DEFINEFONT2:
    case ST_DEFINEFONTINFO: //pseudodefine
    case ST_DEFINETEXT:
    case ST_DEFINETEXT2:
    case ST_DEFINESOUND:
    case ST_DEFINESPRITE:
    case ST_NAMECHARACTER: //pseudodefine
      id = swf_GetU16(t);
      break;
  }

  swf_SetTagPos(t,oldTagPos);

  return id;
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
    case ST_STARTSOUND:
      id = swf_GetU16(t);
      break;

    case ST_PLACEOBJECT2:
    { U8 flags = swf_GetU8(t);
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
 ST_DEFINEMORPHSHAPE,
 ST_DEFINEFONT,
 ST_DEFINEFONT2,
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
 -1
};

// tags which may be used inside a sprite definition
static int swf_spritetagids[] =
{ST_SHOWFRAME,
 ST_PLACEOBJECT,
 ST_PLACEOBJECT2,
 ST_REMOVEOBJECT,
 ST_REMOVEOBJECT2, //?
 ST_DOACTION,
 ST_STARTSOUND,
 ST_FRAMELABEL,
 ST_SOUNDSTREAMHEAD,
 ST_SOUNDSTREAMHEAD2,
 ST_SOUNDSTREAMBLOCK,
 ST_END,
 -1
};

static int swf_pseudodefiningtagids[] = 
{
 ST_DEFINEFONTINFO,
 ST_DEFINEBUTTONCXFORM,
 ST_DEFINEBUTTONSOUND,
 ST_NAMECHARACTER,
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

U16 swf_GetDepth(TAG * t)
// up to SWF 4.0
{ 
  U16 depth = 0;
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
  }
  swf_SetTagPos(t,oldTagPos);
  return depth;
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
            name = &t->data[swf_GetTagPos(t)];
        break;
        case ST_PLACEOBJECT2: {   
            U8 flags = swf_GetU8(t);
            swf_GetU16(t); //depth;
            if(flags&PF_CHAR) 
              swf_GetU16(t); //id
            if(flags&PF_MATRIX)
              swf_GetMatrix(t, &m);
            if(flags&PF_CXFORM)
              swf_GetCXForm(t, &c, 1);
            if(flags&PF_RATIO)
              swf_GetU16(t);
            if(flags&PF_NAME) {
              swf_ResetReadBits(t);
              name = &t->data[swf_GetTagPos(t)];
            }
        }
        break;
    }
    swf_SetTagPos(t,oldTagPos);
    return name;
}

void enumerateUsedIDs_styles(TAG * tag, void (*callback)(TAG*, int, void*), void*callback_data, int num)
{
    U16 count;
    int t;
    count = swf_GetU8(tag);
    if(count == 0xff && num>1) // defineshape2,3 only
	count = swf_GetU16(tag);

    for(t=0;t<count;t++)
    {
	int type;
	U8*pos;
	swf_ResetReadBits(tag);
	type = swf_GetU8(tag); //type
	if(type == 0) {
	    if(num == 3)
		swf_GetRGBA(tag, NULL);
	    else 
		swf_GetRGB(tag, NULL);
	}
	else if(type == 0x10 || type == 0x12)
	{
	    swf_ResetReadBits(tag);
	    swf_GetMatrix(tag, NULL);
	    swf_ResetReadBits(tag);
	    swf_GetGradient(tag, NULL, /*alpha*/ num>=3?1:0);
	}
	else if(type == 0x40 || type == 0x41)
	{
	    swf_ResetReadBits(tag);
	    // we made it.
	    if(tag->data[tag->pos] != 0xff ||
	       tag->data[tag->pos+1] != 0xff)
	    (callback)(tag, tag->pos, callback_data);

	    swf_GetU16(tag);
	    swf_ResetReadBits(tag);
	    swf_GetMatrix(tag, NULL);
	}
	else {
	    fprintf(stderr, "rfxswf:swftools.c Unknown fillstyle:0x%02x\n",type);
	}
    }
    swf_ResetReadBits(tag);
    count = swf_GetU8(tag); // line style array
    if(count == 0xff)
	count = swf_GetU16(tag);
    for(t=0;t<count;t++) 
    {
	swf_GetU16(tag);
	if(num == 3)
	    swf_GetRGBA(tag, NULL);
	else
	    swf_GetRGB(tag, NULL);
    }
}

void enumerateUsedIDs(TAG * tag, int base, void (*callback)(TAG*, int, void*), void*callback_data)
{
    int num = 1;
    swf_ResetReadBits(tag);
    tag->pos = 0;
    switch(tag->id)
    {
	case ST_DEFINEBUTTONCXFORM: {
	    int t;
	    callback(tag, tag->pos + base, callback_data);
	    for(t=0;t<4;t++) {
		int flags;
		callback(tag, tag->pos + base, callback_data);
		swf_GetU16(tag); //sound id
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
	case ST_DEFINEBUTTONSOUND:
	    callback(tag, tag->pos + base, callback_data); //button id
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
		tag2->data = malloc(len);
		memcpy(tag2->data, &tag->data[tag->pos], len);
		/* I never saw recursive sprites, but they are (theoretically) 
		   possible, so better add base here again */
		enumerateUsedIDs(tag2, tag->pos + base, callback, callback_data);
		swf_DeleteTag(tag2);
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
		U16 charid;
		if(!swf_GetU8(tag)) //flags
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
		swf_ResetReadBits(tag);
		flags = swf_GetBits(tag, 8);
		if(!flags) break;
		if(flags & 128) // text style record
		{
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
		} else { // glyph record
		    int t;
		    swf_ResetReadBits(tag);
		    for(t=0;t<flags;t++) {
			swf_GetBits(tag, glyphbits);
			swf_GetBits(tag, advancebits);
		    }
		}
	    }
	    break;
	}
	case ST_DEFINEFONTINFO:
	    callback(tag, tag->pos + base, callback_data);
	break;

	case ST_DEFINESHAPE3: // these thingies might have bitmap ids in their fillstyles
	num++; //fallthrough
	case ST_DEFINESHAPE2:
	num++; //fallthrough
	case ST_DEFINESHAPE: {
	    int fillbits;
	    int linebits;
	    int id; 
	    id = swf_GetU16(tag); // id;
	    swf_GetRect(tag, NULL); // bounds

	    enumerateUsedIDs_styles(tag, callback, callback_data, num);
	    fillbits = swf_GetBits(tag, 4);
	    linebits = swf_GetBits(tag, 4);
	    swf_ResetReadBits(tag);
	    while(1) {
		int flags;
		flags = swf_GetBits(tag, 1);
		if(!flags) { //style change
		    flags = swf_GetBits(tag, 5);
		    if(!flags)
			break;
		    if(flags&1) { //move
			int n = swf_GetBits(tag, 5); 
			swf_GetBits(tag, n); //x
			swf_GetBits(tag, n); //y
		    }
		    if(flags&2) { //fill0
			swf_GetBits(tag, fillbits); 
		    }
		    if(flags&4) { //fill1
			swf_GetBits(tag, fillbits); 
		    }
		    if(flags&8) { //linestyle
			swf_GetBits(tag, linebits); 
		    }
		    if(flags&16) {
			enumerateUsedIDs_styles(tag, callback, callback_data, num);
			fillbits = swf_GetBits(tag, 4);
			linebits = swf_GetBits(tag, 4);
		    }
		} else {
		    flags = swf_GetBits(tag, 1);
		    if(flags) { //straight edge
			int n = swf_GetBits(tag, 4) + 2;
			if(swf_GetBits(tag, 1)) { //line flag
			    swf_GetBits(tag, n); //delta x
			    swf_GetBits(tag, n); //delta y
			} else {
			    int v=swf_GetBits(tag, 1);
			    swf_GetBits(tag, n); //vert/horz
			}
		    } else { //curved edge
			int n = swf_GetBits(tag, 4) + 2;
			swf_GetBits(tag, n);
			swf_GetBits(tag, n);
			swf_GetBits(tag, n);
			swf_GetBits(tag, n);
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
}

void callbackFillin(TAG * t,int pos, void*ptr)
{
    **(int**)ptr = pos;
    (*(int**)ptr)++;
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

