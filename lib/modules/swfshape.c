/* swfshape.c

   shape functions
      
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

#include "../rfxswf.h"

#define SF_MOVETO       0x01
#define SF_FILL0        0x02
#define SF_FILL1        0x04
#define SF_LINE         0x08
#define SF_NEWSTYLE     0x10

void swf_ShapeFree(SHAPE * s)
{ 
    if(!s)
        return;
    if (s->linestyle.data) rfx_free(s->linestyle.data);
    s->linestyle.data = NULL;
    s->linestyle.n    = 0;
    if (s->fillstyle.data) rfx_free(s->fillstyle.data);
    s->fillstyle.data = NULL;
    s->fillstyle.n    = 0;
    if (s->data) rfx_free(s->data);
    s->data = NULL;
    rfx_free(s);
}

int swf_ShapeNew(SHAPE * * s)
{ 
    SHAPE * sh;
    if (!s) return -1;
    sh = (SHAPE *)rfx_calloc(sizeof(SHAPE)); 
    *s = sh;
    return 0;
}

int swf_GetSimpleShape(TAG * t,SHAPE * * s) // without Linestyle/Fillstyle Record
{ SHAPE * sh;
  int bitl, len;
  int end;
  U32 pos;
  
  if (FAILED(swf_ShapeNew(s))) return -1;
  sh = s[0];

  swf_ResetReadBits(t); 
  sh->bits.fill = (U16)swf_GetBits(t,4);
  sh->bits.line = (U16)swf_GetBits(t,4);
  bitl = 0; end = 0; pos = swf_GetTagPos(t);

  while (!end)
  { int edge = swf_GetBits(t,1); bitl+=1;
    if (edge)
    { bitl+=1;
      if (swf_GetBits(t,1))                 // Line
      { U16 nbits = swf_GetBits(t,4)+2;
        bitl+=5;

        if (swf_GetBits(t,1))               // x/y Line
        { swf_GetBits(t,nbits);
          swf_GetBits(t,nbits);
          bitl+=nbits*2;
        }
        else                            // hline/vline
        { swf_GetBits(t,nbits+1);
          bitl+=nbits+1;
        }
      }
      else                              // Curve
      { U16 nbits = swf_GetBits(t,4)+2;
        bitl+=4;

        swf_GetBits(t,nbits);
        swf_GetBits(t,nbits);
        swf_GetBits(t,nbits);
        swf_GetBits(t,nbits);

        bitl+=4*nbits;
      }
    }
    else
    { U16 flags = swf_GetBits(t,5); bitl+=5;
      if (flags)
      {
        if (flags&SF_MOVETO)
        { U16 nbits = swf_GetBits(t,5); bitl+=5;
          swf_GetBits(t,nbits);
          swf_GetBits(t,nbits);
          bitl+=2*nbits;
        }
        
        if (flags&SF_FILL0)
        { swf_GetBits(t,sh->bits.fill);
          bitl+=sh->bits.fill;
        }
        
        if (flags&SF_FILL1)
        { swf_GetBits(t,sh->bits.fill);
          bitl+=sh->bits.fill;
        }

        if (flags&SF_LINE)
        { swf_GetBits(t,sh->bits.line);
          bitl+=sh->bits.line;
        }

        if (flags&SF_NEWSTYLE)
        { fprintf(stderr,"RFXSWF: Can't process extended styles in shape.\n");
        }
      }
      else end = 1;
    }
  }
  swf_SetTagPos(t,pos);
  len = (bitl+7)/8;
  
  if (sh->data) rfx_free(sh->data);
  sh->data = (U8*)rfx_alloc(len);
  
  if (sh->data)
  { sh->bitlen = bitl;
    swf_GetBlock(t,sh->data,len);
  }
  else return -1;
  
  return len;
}

int swf_SetSimpleShape(TAG * t,SHAPE * s) // without Linestyle/Fillstyle Record
{ int l;

  if (!s) return -1;
  l = (s->bitlen+7)/8;

  if (t)
  { swf_ResetWriteBits(t);

    swf_SetBits(t,s->bits.fill,4);
    swf_SetBits(t,s->bits.line,4);
    swf_SetBlock(t,s->data,l);

    swf_ResetWriteBits(t);
  }
  return l+1;
}

int swf_SetFillStyle(TAG * t,FILLSTYLE * f)
{ if ((!t)||(!f)) return -1;
  swf_SetU8(t,f->type);
  
  switch (f->type)
  { case FILL_SOLID:
      if (swf_GetTagID(t)!=ST_DEFINESHAPE3) swf_SetRGB(t,&f->color);
      else swf_SetRGBA(t,&f->color);
      break;

    case FILL_TILED:
    case FILL_CLIPPED:
      swf_SetU16(t,f->id_bitmap);
      swf_SetMatrix(t,&f->m);
      break;
    case FILL_LINEAR:
    case FILL_RADIAL:
      swf_SetMatrix(t,&f->m);
      swf_SetGradient(t,&f->gradient,/*alpha?*/t->id==ST_DEFINESHAPE3?1:0);
      break;
  }
  
  return 0;
}

int swf_SetLineStyle(TAG * t,LINESTYLE * l)
{ if ((!l)||(!t)) return -1;
  swf_SetU16(t,l->width);

  if (swf_GetTagID(t)!=ST_DEFINESHAPE3) swf_SetRGB(t,&l->color);
  else swf_SetRGBA(t,&l->color);
  
  return 0;
}

int swf_SetShapeStyleCount(TAG * t,U16 n)
{ if (n>254)
  { swf_SetU8(t,0xff);
    swf_SetU16(t,n);
    return 3;
  }
  else
  { swf_SetU8(t,(U8)n);
    return 1;
  }
}

int swf_SetShapeStyles(TAG * t,SHAPE * s)
{ int i,l;
  if (!s) return -1;

  l = 0;
  l += swf_SetShapeStyleCount(t,s->fillstyle.n);

  for (i=0;i<s->fillstyle.n;i++)
    l+=swf_SetFillStyle(t,&s->fillstyle.data[i]);

  l += swf_SetShapeStyleCount(t,s->linestyle.n);

  for (i=0;i<s->linestyle.n;i++)
    l+=swf_SetLineStyle(t,&s->linestyle.data[i]);

  return l;
}

int swf_ShapeCountBits(SHAPE * s,U8 * fbits,U8 * lbits)
{ if (!s) return -1;
  s->bits.fill = swf_CountUBits(s->fillstyle.n, 0);
  s->bits.line = swf_CountUBits(s->linestyle.n, 0);
  if (fbits) fbits[0] = s->bits.fill;
  if (lbits) lbits[0] = s->bits.line;
  return 0;    
}

int swf_SetShapeBits(TAG * t,SHAPE * s)
{ if ((!t)||(!s)) return -1;
  swf_ResetWriteBits(t);
  swf_SetBits(t,s->bits.fill,4);
  swf_SetBits(t,s->bits.line,4);
  return 0;
}

int swf_SetShapeHeader(TAG * t,SHAPE * s)
{ int res;
  res = swf_SetShapeStyles(t,s);
  if (res>=0) res = swf_ShapeCountBits(s,NULL,NULL);
  if (res>=0) res = swf_SetShapeBits(t,s);
  return res;
}

int swf_ShapeAddFillStyle(SHAPE * s,U8 type,MATRIX * m,RGBA * color,U16 id_bitmap, GRADIENT*gradient)
{ RGBA def_c;
  MATRIX def_m;    
  GRADIENT def_g;

  // handle defaults
  
  if (!s) return -1;
  if (!color)
  { color = &def_c;
    def_c.a = 0xff;
    def_c.r = def_c.g = def_c.b = 0;
  }
  if (!m)
  { m = &def_m;
    swf_GetMatrix(NULL,m);
  }
  if(!gradient)
  {
    gradient = &def_g;
    swf_GetGradient(NULL, gradient, 1);
  }

  // handle memory
  
  if (s->fillstyle.data)
  { FILLSTYLE * xnew = (FILLSTYLE *)rfx_realloc(s->fillstyle.data,(s->fillstyle.n+1)*sizeof(FILLSTYLE));
    if (!xnew) return -1;
    s->fillstyle.data = xnew;
  }
  else
  { s->fillstyle.data = (FILLSTYLE *)rfx_alloc(sizeof(FILLSTYLE));
    s->fillstyle.n = 0;
    if (!s->fillstyle.data) return -1;
  }

  // set fillstyle
  
  s->fillstyle.data[s->fillstyle.n].type = type; 
  s->fillstyle.data[s->fillstyle.n].id_bitmap = id_bitmap;
  memcpy(&s->fillstyle.data[s->fillstyle.n].m,m,sizeof(MATRIX));
  memcpy(&s->fillstyle.data[s->fillstyle.n].color,color,sizeof(RGBA));
  memcpy(&s->fillstyle.data[s->fillstyle.n].gradient,gradient,sizeof(GRADIENT));
          
  return (++s->fillstyle.n);
}
int swf_ShapeAddFillStyle2(SHAPE * s,FILLSTYLE*fs)
{
    return swf_ShapeAddFillStyle(s, fs->type, &fs->m, &fs->color, fs->id_bitmap, &fs->gradient);
}

int swf_ShapeAddSolidFillStyle(SHAPE * s,RGBA * color)
{ return swf_ShapeAddFillStyle(s,FILL_SOLID,NULL,color,0,0);
}

int swf_ShapeAddBitmapFillStyle(SHAPE * s,MATRIX * m,U16 id_bitmap,int clip)
{ return swf_ShapeAddFillStyle(s,clip?FILL_CLIPPED:FILL_TILED,m,NULL,id_bitmap,0);
}

int swf_ShapeAddGradientFillStyle(SHAPE * s,MATRIX * m,GRADIENT* gradient,int radial)
{ return swf_ShapeAddFillStyle(s,radial?FILL_RADIAL:FILL_LINEAR,m,NULL,0,gradient);
}

int swf_ShapeAddLineStyle(SHAPE * s,U16 width,RGBA * color)
{ RGBA def;
  if (!s) return -1;
  if (!color)
  { color = &def;
    def.a = 0xff;
    def.r = def.g = def.b = 0; 
  }
  if (s->linestyle.data)
  { LINESTYLE * xnew = (LINESTYLE *)rfx_realloc(s->linestyle.data,(s->linestyle.n+1)*sizeof(LINESTYLE));
    if (!xnew) return -1;
    s->linestyle.data = xnew;
  }
  else
  { s->linestyle.data = (LINESTYLE *)rfx_alloc(sizeof(LINESTYLE));
    s->linestyle.n = 0;
    if (!s->linestyle.data) return -1;
  }
  
  s->linestyle.data[s->linestyle.n].width = width;
  memcpy(&s->linestyle.data[s->linestyle.n].color,color,sizeof(RGBA));

  return (++s->linestyle.n);
}

int swf_ShapeSetMove(TAG * t,SHAPE * s,S32 x,S32 y)
{ U8 b;
  if (!t) return -1;
  swf_SetBits(t,0,1);
  swf_SetBits(t,SF_MOVETO,5);
  
  b = swf_CountBits(x,0);
  b = swf_CountBits(y,b);

  if(b>31) {
      fprintf(stderr, "Warning: bad moveTo (%f,%f)\n", x/20.0, y/20.0);
      b=31;
  }
    
  swf_SetBits(t,b,5);
  swf_SetBits(t,x,b);
  swf_SetBits(t,y,b);

  return 0;
}

int swf_ShapeSetStyle(TAG * t,SHAPE * s,int line,int fill0,int fill1)
{ if ((!t)||(!s)) return -1;
    
  swf_SetBits(t,0,1);
  swf_SetBits(t,(line?SF_LINE:0)|(fill0?SF_FILL0:0)|(fill1?SF_FILL1:0),5);

  if (fill0) swf_SetBits(t,fill0,s->bits.fill);
  if (fill1) swf_SetBits(t,fill1,s->bits.fill);
  if (line)  swf_SetBits(t,line ,s->bits.line);
  
  return 0;
}

/* TODO: sometimes we want to set fillstyle 0, as that's the empty fill
   used for line drawings. At the moment, we can't, as 0 fill be considered
   nonexistent and therefore not set.
   these defines are a workaround (they also reduce the maximal number of
   fill styles to 32768)
 */
#define UNDEFINED_COORD 0x7fffffff

int swf_ShapeSetAll(TAG * t,SHAPE * s,S32 x,S32 y,int line,int fill0,int fill1)
{ U8 b;
  U8 hasmove = 0;
  if ((!t)||(!s)) return -1;

  if(x!=UNDEFINED_COORD || y!=UNDEFINED_COORD)
      hasmove=1;

  swf_SetBits(t,0,1);
  swf_SetBits(t,(hasmove?SF_MOVETO:0)|(line?SF_LINE:0)|(fill0?SF_FILL0:0)|(fill1?SF_FILL1:0),5);

  if(hasmove) {
    b = swf_CountBits(x,0);
    b = swf_CountBits(y,b);
    swf_SetBits(t,b,5);
    swf_SetBits(t,x,b);
    swf_SetBits(t,y,b);
  }

  if (fill0) swf_SetBits(t,fill0,s->bits.fill);
  if (fill1) swf_SetBits(t,fill1,s->bits.fill);
  if (line)  swf_SetBits(t,line ,s->bits.line);
  
  return 0;
}

int swf_ShapeSetEnd(TAG * t)
{ if (!t) return -1;
  swf_SetBits(t,0,6);
  swf_ResetWriteBits(t);
  return 0;
}

int swf_ShapeSetLine(TAG * t,SHAPE * s,S32 x,S32 y)
{ 
    U8 b;
    if (!t) return -1;
   
    b = swf_CountBits(x,2);
    b = swf_CountBits(y,b);
    if (b<2) b=2;
    if(b >= 18) {
        if(b > 18 + 6) {
            /* do not split into more than 64 segments. If the line is *that* long, something's broken */
            fprintf(stderr, "Warning: Line to %.2f,%.2f is too long (%d bits)\n", (double)x,(double)y, b);
            return -1;
        } else {
            /* split line */
            int x1,y1,x2,y2;
            if(x>=0) { x1 = x/2;x2 = (x+1)/2;} 
            else     { x1 = x/2;x2 = (x-1)/2;}
            if(y>=0) { y1 = y/2;y2 = (y+1)/2;} 
            else     { y1 = y/2;y2 = (y-1)/2;}
            swf_ShapeSetLine(t, s, x1,y1);
            swf_ShapeSetLine(t, s, x2,y2);
            return 0;
        }
    }

    if(x!=0 && y!=0) { //(!s)||((x!=0)&&(y!=0)))
        swf_SetBits(t,3,2); // Straight Edge
        swf_SetBits(t, b-2, 4); //Number of Bits in x/y
        swf_SetBits(t,1,1); // Diagonal
        swf_SetBits(t,x,b);
        swf_SetBits(t,y,b);
    } else if (x==0) {
        swf_SetBits(t,3,2); // Straight Edge
        swf_SetBits(t, b-2, 4); //Number of Bits in y
        swf_SetBits(t,1,2); // Vertical
        swf_SetBits(t,y,b);
    } else {
        swf_SetBits(t,3,2); // Straight Edge
        swf_SetBits(t, b-2, 4); //Number of Bits in x
        swf_SetBits(t,0,2); // Horizontal
        swf_SetBits(t,x,b);
    }
    return 0;
}

int swf_ShapeSetCurve(TAG * t,SHAPE * s,S32 x,S32 y,S32 ax,S32 ay)
{ 
    U8 b;
    if (!t) return -1;

    b = swf_CountBits(ax,2);
    b = swf_CountBits(ay,b);
    b = swf_CountBits(x,b);
    b = swf_CountBits(y,b);

    if(b >= 18) {
          fprintf(stderr, "Bit overflow in swf_ShapeSetCurve- %d (%d,%d,%d,%d)\n", b, ax,ay,x,y);
	  return swf_ShapeSetLine(t, s, x+ax, y+ay);
    }

    swf_SetBits(t,2,2);
    swf_SetBits(t,b-2,4);
    swf_SetBits(t,x,b);
    swf_SetBits(t,y,b);
    swf_SetBits(t,ax,b);
    swf_SetBits(t,ay,b);
    return 0;
}

int swf_ShapeSetCircle(TAG * t,SHAPE * s,S32 x,S32 y,S32 rx,S32 ry)
{ double C1 = 0.2930;    
  double C2 = 0.4140;   
  double begin = 0.7070; 

  if (!t) return -1;
  
  swf_ShapeSetMove(t,s,x+begin*rx,y+begin*ry);
  swf_ShapeSetCurve(t,s, -C1*rx,  C1*ry, -C2*rx,      0);
  swf_ShapeSetCurve(t,s, -C2*rx,      0, -C1*rx, -C1*ry);
  swf_ShapeSetCurve(t,s, -C1*rx, -C1*ry,      0, -C2*ry);
  swf_ShapeSetCurve(t,s,      0, -C2*ry,  C1*rx, -C1*ry);
  swf_ShapeSetCurve(t,s,  C1*rx, -C1*ry,  C2*rx,      0);
  swf_ShapeSetCurve(t,s,  C2*rx,      0,  C1*rx,  C1*ry);
  swf_ShapeSetCurve(t,s,  C1*rx,  C1*ry,      0,  C2*ry);
  swf_ShapeSetCurve(t,s,      0,  C2*ry, -C1*rx,  C1*ry);
  
  return 0;
}

void dummycallback1(TAG*tag, int x, void*y)
{
}

// from swftools.c:
void enumerateUsedIDs_styles(TAG * tag, void (*callback)(TAG*, int, void*), void*callback_data, int num, int morph);

static void parseFillStyle(FILLSTYLE*dest, TAG*tag, int num)
{
    int type = swf_GetU8(tag); //type
    dest->type = type;
    if(type == 0) {
	/* plain color */
	if(num >= 3)
	    swf_GetRGBA(tag, &dest->color);
	else 
	    swf_GetRGB(tag, &dest->color);
    }
    else if(type == 0x10 || type == 0x11 || type == 0x12 || type == 0x13)
    {
	/* linear/radial gradient fill */
	swf_ResetReadBits(tag);
	swf_GetMatrix(tag, &dest->m);
	swf_ResetReadBits(tag);
	swf_GetGradient(tag, &dest->gradient, num>=3?1:0);
	if(type == 0x13)
	    swf_GetU16(tag);
    }
    else if(type == 0x40 || type == 0x41 || type == 0x42 || type == 0x43)
    {
	/* bitmap fill */
	swf_ResetReadBits(tag);
	dest->id_bitmap = swf_GetU16(tag); //id
	swf_ResetReadBits(tag); //?
	swf_GetMatrix(tag, &dest->m);
    }
    else {
	fprintf(stderr, "rfxswf:swfshape.c Unknown fillstyle:0x%02x in tag %02d\n",type, tag->id);
    }
}
static int parseFillStyleArray(TAG*tag, SHAPE2*shape)
{
    U16 count;
    int t;
    int num=0;
    int fillstylestart = shape->numfillstyles;
    int linestylestart = shape->numlinestyles;

    if(tag->id == ST_DEFINESHAPE)
	num = 1;
    else if(tag->id == ST_DEFINESHAPE2)
	num = 2;
    else if(tag->id == ST_DEFINESHAPE3)
	num = 3;
    else if(tag->id == ST_DEFINESHAPE4)
	num = 4;

    count = swf_GetU8(tag);
    if(count == 0xff && num>1) // defineshape2,3 only
	count = swf_GetU16(tag);

    shape->numfillstyles += count;
    if(shape->numfillstyles) {
	shape->fillstyles = (FILLSTYLE*)rfx_realloc(shape->fillstyles, sizeof(FILLSTYLE)*shape->numfillstyles);

        for(t=fillstylestart;t<shape->numfillstyles;t++) {
	    parseFillStyle(&shape->fillstyles[t], tag, num);
        }
    }

    swf_ResetReadBits(tag);
    count = swf_GetU8(tag); // line style array
    if(count == 0xff)
	count = swf_GetU16(tag);

    shape->numlinestyles += count;
    if(count) {
	shape->linestyles = (LINESTYLE*)rfx_realloc(shape->linestyles, sizeof(LINESTYLE)*shape->numlinestyles);
        /* TODO: should we start with 1 and insert a correct definition of the
           "built in" linestyle 0? */
        for(t=linestylestart;t<shape->numlinestyles;t++) 
        {
	    char fill = 0;
            shape->linestyles[t].width = swf_GetU16(tag);

	    if(num >= 4) {
		U16 flags = swf_GetU16(tag);
		if((flags & 0x30) == 0x20)
		    swf_GetU16(tag); // miter limit
		if(flags & 0x08) {
		    fprintf(stderr, "Warning: Filled strokes parsing not yet fully supported\n");
		    fill = 1;
		}
	    }

	    if(fill) {
		FILLSTYLE f;
		parseFillStyle(&f, tag, num);
		shape->linestyles[t].color = f.color;
	    } else {
		if(num >= 3)
		    swf_GetRGBA(tag, &shape->linestyles[t].color);
		else
		    swf_GetRGB(tag, &shape->linestyles[t].color);
	    }
        }
    }
    return 1;
}

char swf_ShapeIsEmpty(SHAPE*s)
{
    if(!s || !s->data) return 1;
    TAG _tag;
    TAG* tag = &_tag;
    memset(tag, 0, sizeof(TAG));
    tag->data = s->data;
    tag->len = tag->memsize = (s->bitlen+7)/8;
    tag->pos = 0;
    
    while(1) {
	if(!swf_GetBits(tag, 1)) {
	    U16 flags = swf_GetBits(tag, 5);
	    if(!flags) break;
	    if(flags&1) { //move
		int n = swf_GetBits(tag, 5); 
		swf_GetSBits(tag, n); //x
		swf_GetSBits(tag, n); //y
	    }
	    if(flags&2) swf_GetBits(tag, s->bits.fill);
	    if(flags&4) swf_GetBits(tag, s->bits.fill);
	    if(flags&8) swf_GetBits(tag, s->bits.line);
	    if(flags&16) {return 0;}
	} else {
	    return 0;
	}
    }
    return 1;
}

/* todo: merge this with swf_GetSimpleShape */
static SHAPELINE* swf_ParseShapeData(U8*data, int bits, int fillbits, int linebits, int version, SHAPE2*shape2)
{
    SHAPELINE _lines;
    SHAPELINE*lines = &_lines;

    TAG _tag;
    TAG* tag = &_tag;
    int fill0 = 0;
    int fill1 = 0;
    int line = 0;
    int x=0,y=0;
    int linestyleadd=0;
    int fillstyleadd=0;
    
    memset(tag, 0, sizeof(TAG));
    tag->data = data;
    tag->len = tag->memsize = (bits+7)/8;
    tag->pos = 0;
    tag->id = version==1?ST_DEFINESHAPE:(version==2?ST_DEFINESHAPE2:(version==3?ST_DEFINESHAPE3:ST_DEFINESHAPE4));

    lines->next = 0;
    while(1) {
	int flags;
	flags = swf_GetBits(tag, 1);
	if(!flags) { //style change
	    flags = swf_GetBits(tag, 5);
	    if(!flags)
		break;
	    if(flags&1) { //move
		int n = swf_GetBits(tag, 5); 
		x = swf_GetSBits(tag, n); //x
		y = swf_GetSBits(tag, n); //y
	    }
	    if(flags&2)
		fill0 = swf_GetBits(tag, fillbits) + fillstyleadd; 
	    if(flags&4)
		fill1 = swf_GetBits(tag, fillbits) + fillstyleadd; 
	    if(flags&8)
		line = swf_GetBits(tag, linebits) + linestyleadd; 
	    if(flags&16) {
		if(!shape2) {
		    fprintf(stderr, "rfxswf: Error: Additional fillstyles not supported\n");fflush(stderr);
		    enumerateUsedIDs_styles(tag, dummycallback1, 0, version, 0);
		} else {
		    linestyleadd = shape2->numlinestyles;
		    fillstyleadd = shape2->numfillstyles;
		    if(!parseFillStyleArray(tag, shape2))
			return 0;
		}
		fillbits = swf_GetBits(tag, 4);
		linebits = swf_GetBits(tag, 4);
	    }
	    if(flags&1) { //move
		lines->next = (SHAPELINE*)rfx_alloc(sizeof(SHAPELINE));
		lines = lines->next;
		lines->type = moveTo;
		lines->x = x; 
		lines->y = y; 
		lines->sx = lines->sy = 0;
		lines->fillstyle0 = fill0;
		lines->fillstyle1 = fill1;
		lines->linestyle = line;
		lines->next = 0;
	    }
	} else {
	    flags = swf_GetBits(tag, 1);
	    if(flags) { //straight edge
		int n = swf_GetBits(tag, 4) + 2;
		if(swf_GetBits(tag, 1)) { //line flag
		    x += swf_GetSBits(tag, n); //delta x
		    y += swf_GetSBits(tag, n); //delta y
		} else {
		    int v=swf_GetBits(tag, 1);
		    int d;
		    d = swf_GetSBits(tag, n); //vert/horz
		    if(v) y += d;
		    else  x += d;
		}
		lines->next = (SHAPELINE*)rfx_alloc(sizeof(SHAPELINE));
		lines = lines->next;
		lines->type = lineTo;
		lines->x = x; 
		lines->y = y; 
		lines->sx = lines->sy = 0;
		lines->fillstyle0 = fill0;
		lines->fillstyle1 = fill1;
		lines->linestyle = line;
		lines->next = 0;
	    } else { //curved edge
		int n = swf_GetBits(tag, 4) + 2;
		int x1,y1;
		x += swf_GetSBits(tag, n);
		y += swf_GetSBits(tag, n);
		x1 = x;
		y1 = y;
		x += swf_GetSBits(tag, n);
		y += swf_GetSBits(tag, n);

		lines->next = (SHAPELINE*)rfx_alloc(sizeof(SHAPELINE));
		lines = lines->next;
		lines->type = splineTo;
		lines->sx = x1; 
		lines->sy = y1; 
		lines->x = x; 
		lines->y = y; 
		lines->fillstyle0 = fill0;
		lines->fillstyle1 = fill1;
		lines->linestyle = line;
		lines->next = 0;
	    }
	}
    }
    return _lines.next;
}

SRECT swf_GetShapeBoundingBox(SHAPE2*shape2)
{
    SRECT r;
    SHAPELINE*l = shape2->lines;
    int lastx=0,lasty=0;
    int valid = 0;
    r.xmin = r.ymin = SCOORD_MAX;
    r.xmax = r.ymax = SCOORD_MIN;

    while(l) {
	int t1;
	if(l->linestyle>0) {
	    t1 = shape2->linestyles[l->linestyle - 1].width*3/2;
	} else {
	    t1 = 0;
	}

	if(l->type == lineTo || l->type == splineTo)
	{
	    valid = 1;
	    if(lastx - t1 < r.xmin) r.xmin = lastx - t1;
	    if(lasty - t1 < r.ymin) r.ymin = lasty - t1;
	    if(lastx + t1 > r.xmax) r.xmax = lastx + t1;
	    if(lasty + t1 > r.ymax) r.ymax = lasty + t1;
	    if(l->x - t1 < r.xmin) r.xmin = l->x - t1;
	    if(l->y - t1 < r.ymin) r.ymin = l->y - t1;
	    if(l->x + t1 > r.xmax) r.xmax = l->x + t1;
	    if(l->y + t1 > r.ymax) r.ymax = l->y + t1;
	    if(l->type == splineTo) {
		if(l->sx - t1 < r.xmin) r.xmin = l->sx - t1;
		if(l->sy - t1 < r.ymin) r.ymin = l->sy - t1;
		if(l->sx + t1 > r.xmax) r.xmax = l->sx + t1;
		if(l->sy + t1 > r.ymax) r.ymax = l->sy + t1;
	    }
	}
	lastx = l->x;
	lasty = l->y;
	l = l->next;
    }
    if(!valid) memset(&r, 0, sizeof(SRECT));
    return r;
}

void swf_Shape2Free(SHAPE2 * s)
{
    SHAPELINE*line = s->lines;
    s->lines = 0;
    while(line) {
	SHAPELINE*next = line->next;
	line->next = 0;
	rfx_free(line);
	line = next;
    }

    if(s->linestyles) {
	rfx_free(s->linestyles);
	s->linestyles = 0;
    }
    if(s->fillstyles) {
	rfx_free(s->fillstyles);
	s->fillstyles = 0;
    }
    if(s->bbox) {
	rfx_free(s->bbox);
	s->bbox = 0;
    }
}

SHAPE2* swf_Shape2Clone(SHAPE2 * s)
{
    SHAPELINE*line = s->lines;
    SHAPELINE*prev = 0;
    SHAPE2*s2 = (SHAPE2*)rfx_alloc(sizeof(SHAPE2));
    memcpy(s2,s,sizeof(SHAPE2));
    s2->linestyles = (LINESTYLE*)rfx_alloc(sizeof(LINESTYLE)*s->numlinestyles);
    memcpy(s2->linestyles, s->linestyles, sizeof(LINESTYLE)*s->numlinestyles);
    s2->fillstyles = (FILLSTYLE*)rfx_alloc(sizeof(FILLSTYLE)*s->numfillstyles);
    memcpy(s2->fillstyles, s->fillstyles, sizeof(FILLSTYLE)*s->numfillstyles);

    while(line) {
	SHAPELINE*line2 = (SHAPELINE*)rfx_alloc(sizeof(SHAPELINE));
        memcpy(line2, line, sizeof(SHAPELINE));
        line2->next = 0;
        if(prev)
            prev->next = line2;
        else
            s2->lines = line2;
        prev = line2;
	line = line->next;
    }
    if(s->bbox) {
        s2->bbox = (SRECT*)rfx_alloc(sizeof(SRECT));
        memcpy(s2->bbox, s->bbox, sizeof(SRECT));
    }
    return s2;
}

SHAPE2* swf_ShapeToShape2(SHAPE*shape) {

    SHAPE2*shape2 = (SHAPE2*)rfx_calloc(sizeof(SHAPE2));
    
    shape2->numlinestyles = shape->linestyle.n;
    if(shape2->numlinestyles) {
        shape2->linestyles = (LINESTYLE*)rfx_alloc(sizeof(LINESTYLE)*shape->linestyle.n);
        memcpy(shape2->linestyles, shape->linestyle.data, sizeof(LINESTYLE)*shape->linestyle.n);
    }
    
    shape2->numfillstyles = shape->fillstyle.n;
    if(shape2->numfillstyles) {
        shape2->fillstyles = (FILLSTYLE*)rfx_alloc(sizeof(FILLSTYLE)*shape->fillstyle.n);
        memcpy(shape2->fillstyles, shape->fillstyle.data, sizeof(FILLSTYLE)*shape->fillstyle.n);
    }

    shape2->lines = swf_ParseShapeData(shape->data, shape->bitlen, shape->bits.fill, shape->bits.line, 1, 0);
    shape2->bbox = 0;
    return shape2;
};

void swf_ShapeSetBitmapRect(TAG*tag, U16 gfxid, int width, int height)
{
    SHAPE*shape;
    MATRIX m;
    RGBA rgb;
    SRECT r;
    int lines = 0;
    int ls=0,fs;
    swf_ShapeNew(&shape);
    rgb.b = rgb.g = rgb.r = 0xff;
    if(lines)
	ls = swf_ShapeAddLineStyle(shape,20,&rgb);  
    swf_GetMatrix(NULL,&m);
    m.sx = 20*65536;
    m.sy = 20*65536;

    fs = swf_ShapeAddBitmapFillStyle(shape,&m,gfxid,0);
    r.xmin = 0;
    r.ymin = 0;
    r.xmax = width*20;
    r.ymax = height*20;
    swf_SetRect(tag,&r);

    swf_SetShapeStyles(tag,shape);
    swf_ShapeCountBits(shape,NULL,NULL);
    swf_SetShapeBits(tag,shape);

    swf_ShapeSetAll(tag,shape,0,0,lines?ls:0,fs,0);

    swf_ShapeSetLine(tag,shape,width*20,0);
    swf_ShapeSetLine(tag,shape,0,height*20);
    swf_ShapeSetLine(tag,shape,-width*20,0);
    swf_ShapeSetLine(tag,shape,0,-height*20);
    swf_ShapeSetEnd(tag);
    swf_ShapeFree(shape);
}

void swf_ShapeSetRectangle(TAG*tag, U16 shapeid, int width, int height, RGBA*rgba)
{
    RGBA white={255,255,255,255};
    if(!rgba) {
	rgba = &white;
    }
    SHAPE* s;
    swf_ShapeNew(&s);
    int fs = swf_ShapeAddSolidFillStyle(s, rgba);
    swf_SetU16(tag,shapeid);
    SRECT r;
    r.xmin = 0;
    r.xmax = 0;
    r.ymin = width;
    r.ymax = height;
    swf_SetRect(tag,&r);
    swf_SetShapeHeader(tag,s);
    swf_ShapeSetAll(tag,s,0,0,0,fs,0);
    swf_ShapeSetLine(tag,s,width,0);
    swf_ShapeSetLine(tag,s,0,height);
    swf_ShapeSetLine(tag,s,-width,0);
    swf_ShapeSetLine(tag,s,0,-height);
    swf_ShapeSetEnd(tag);
    swf_ShapeFree(s);
}

void swf_ShapeSetRectangleWithBorder(TAG*tag, U16 shapeid, int width, int height, RGBA*rgba, int linewidth, RGBA*linecolor)
{
    RGBA white={255,255,255,255};
    if(!rgba) {
	rgba = &white;
    }
    SHAPE* s;
    swf_ShapeNew(&s);
    int fs = swf_ShapeAddSolidFillStyle(s, rgba);
    int ls = swf_ShapeAddLineStyle(s, linewidth, linecolor);
    swf_SetU16(tag,shapeid);
    SRECT r;
    r.xmin = 0;
    r.xmax = 0;
    r.ymin = width;
    r.ymax = height;
    swf_SetRect(tag,&r);
    swf_SetShapeHeader(tag,s);
    swf_ShapeSetAll(tag,s,0,0,ls,fs,0);
    swf_ShapeSetLine(tag,s,width,0);
    swf_ShapeSetLine(tag,s,0,height);
    swf_ShapeSetLine(tag,s,-width,0);
    swf_ShapeSetLine(tag,s,0,-height);
    swf_ShapeSetEnd(tag);
    swf_ShapeFree(s);
}

void swf_Shape2ToShape(SHAPE2*shape2, SHAPE*shape)
{
    TAG*tag = swf_InsertTag(0,0);
    SHAPELINE*l;
    int newx=0,newy=0,lastx=0,lasty=0,oldls=0,oldfs0=0,oldfs1=0;

    memset(shape, 0, sizeof(SHAPE));

    shape->linestyle.n = shape2->numlinestyles;
    shape->linestyle.data = (LINESTYLE*)rfx_alloc(sizeof(LINESTYLE)*shape->linestyle.n);
    memcpy(shape->linestyle.data, shape2->linestyles, sizeof(LINESTYLE)*shape->linestyle.n);
    
    shape->fillstyle.n =  shape2->numfillstyles;
    shape->fillstyle.data = (FILLSTYLE*)rfx_alloc(sizeof(FILLSTYLE)*shape->fillstyle.n);
    memcpy(shape->fillstyle.data, shape2->fillstyles, sizeof(FILLSTYLE)*shape->fillstyle.n);

    swf_ShapeCountBits(shape,NULL,NULL);

    l = shape2->lines;

    while(l) {
	int ls=0,fs0=0,fs1=0;

	if(l->type != moveTo) {
	    if(oldls != l->linestyle) {oldls = ls = l->linestyle;if(!ls) ls=0x8000;}
	    if(oldfs0 != l->fillstyle0) {oldfs0 = fs0 = l->fillstyle0;if(!fs0) fs0=0x8000;}
	    if(oldfs1 != l->fillstyle1) {oldfs1 = fs1 = l->fillstyle1;if(!fs1) fs1=0x8000;}

	    if(ls || fs0 || fs1 || newx!=0x7fffffff || newy!=0x7fffffff) {
		swf_ShapeSetAll(tag,shape,newx,newy,ls,fs0,fs1);
		newx = 0x7fffffff;
		newy = 0x7fffffff;
	    }
	}

	if(l->type == lineTo) {
	    swf_ShapeSetLine(tag,shape,l->x-lastx,l->y-lasty);
	} else if(l->type == splineTo) {
	    swf_ShapeSetCurve(tag,shape, l->sx-lastx,l->sy-lasty, l->x-l->sx,l->y-l->sy);
	}
	if(l->type == moveTo) {
	    newx = l->x;
	    newy = l->y;
	}

	lastx = l->x;
	lasty = l->y;
	l = l->next;
    }
    swf_ShapeSetEnd(tag);
    shape->data = tag->data;
    shape->bitlen = tag->len*8;
    free(tag);
}

void swf_SetShape2(TAG*tag, SHAPE2*shape2)
{
    SHAPE shape;
    swf_Shape2ToShape(shape2, &shape);

    swf_SetRect(tag,shape2->bbox);
    swf_SetShapeStyles(tag, &shape);
    //swf_ShapeCountBits(&shape,NULL,NULL); // done in swf_Shape2ToShape()
    swf_SetShapeBits(tag,&shape);

    swf_SetBlock(tag, shape.data, (shape.bitlen+7)/8);
}

void swf_ParseDefineShape(TAG*tag, SHAPE2*shape)
{
    int num = 0, id;
    U16 fill,line;
    SHAPELINE*l;
    if(tag->id == ST_DEFINESHAPE)
	num = 1;
    else if(tag->id == ST_DEFINESHAPE2)
	num = 2;
    else if(tag->id == ST_DEFINESHAPE3)
	num = 3;
    else if(tag->id == ST_DEFINESHAPE4)
	num = 4;
    else {
	fprintf(stderr, "parseDefineShape must be called with a shape tag");
    }
    swf_SetTagPos(tag, 0);

    id = swf_GetU16(tag); //id
    memset(shape, 0, sizeof(SHAPE2));
    shape->bbox = (SRECT*)rfx_alloc(sizeof(SRECT));
    swf_GetRect(tag, shape->bbox);
    if(num>=4) {
	SRECT r2;
	swf_ResetReadBits(tag);
	swf_GetRect(tag, &r2); // edge bounds
	U8 flags = swf_GetU8(tag); // flags, &1: contains scaling stroke, &2: contains non-scaling stroke
    }

    if(!parseFillStyleArray(tag, shape)) {
	return;
    }

    swf_ResetReadBits(tag); 
    fill = (U16)swf_GetBits(tag,4);
    line = (U16)swf_GetBits(tag,4);
    if(!fill && !line) {
	fprintf(stderr, "fill/line bits are both zero\n");
    }

    shape->lines = swf_ParseShapeData(&tag->data[tag->pos], (tag->len - tag->pos)*8, fill, line, num, shape);

    l = shape->lines;
}

static void free_lines(SHAPELINE* lines)
{
    if (lines->next)
        free_lines(lines->next);
    free(lines);
}

void swf_RecodeShapeData(U8*data, int bitlen, int in_bits_fill, int in_bits_line, 
                         U8**destdata, U32*destbitlen, int out_bits_fill, int out_bits_line)
{
    SHAPE2 s2;
    SHAPE s;
    SHAPELINE*line;
    memset(&s2, 0, sizeof(s2));
    s2.lines = swf_ParseShapeData(data, bitlen, in_bits_fill, in_bits_line, 1, 0);
    s2.numfillstyles = out_bits_fill?1<<(out_bits_fill-1):0;
    s2.numlinestyles = out_bits_line?1<<(out_bits_line-1):0;
    s2.fillstyles = (FILLSTYLE*)rfx_calloc(sizeof(FILLSTYLE)*s2.numfillstyles);
    s2.linestyles = (LINESTYLE*)rfx_calloc(sizeof(LINESTYLE)*s2.numlinestyles);

    line = s2.lines;
    while(line) {
        if(line->fillstyle0 > s2.numfillstyles) line->fillstyle0 = 0;
        if(line->fillstyle1 > s2.numfillstyles) line->fillstyle1 = 0;
        if(line->linestyle > s2.numlinestyles) line->linestyle = 0;
        line = line->next;
    }

    swf_Shape2ToShape(&s2,&s);

    free_lines(s2.lines);
    free(s2.fillstyles);
    free(s2.linestyles);
    free(s.fillstyle.data);
    free(s.linestyle.data);
    *destdata = s.data;
    *destbitlen = s.bitlen;
}

