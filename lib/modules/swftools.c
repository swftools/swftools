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
    case ST_DEFINEBITS:
    case ST_DEFINEBITSJPEG2:
    case ST_DEFINEBITSJPEG3:
    case ST_DEFINEBITSLOSSLESS:
    case ST_DEFINEBITSLOSSLESS2:
    case ST_DEFINEBUTTON:
    case ST_DEFINEBUTTON2:
    case ST_DEFINEBUTTONCXFORM:
    case ST_DEFINEBUTTONSOUND:
    case ST_DEFINEFONT:
    case ST_DEFINEFONT2:
    case ST_DEFINEFONTINFO:
    case ST_DEFINETEXT:
    case ST_DEFINETEXT2:
    case ST_DEFINESOUND:
    case ST_DEFINESPRITE:
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

int definingtagids[] =
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
int spritetagids[] =
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

U8 swf_isAllowedSpriteTag(TAG * tag)
{
    int id = tag->id;
    int t=0;
    while(spritetagids[t]>=0)
    {
        if(spritetagids[t] == id) 
            return 1;
        t++;
    }
    return 0; 
}

U8 swf_isDefiningTag(TAG * tag)
{
    int id = tag->id;
    int t=0;
    while(definingtagids[t]>=0)
    {
        if(definingtagids[t] == id) 
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

