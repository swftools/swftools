/* swfshape.c

   shape functions
      
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#define SF_MOVETO       0x01
#define SF_FILL0        0x02
#define SF_FILL1        0x04
#define SF_LINE         0x08
#define SF_NEWSTYLE     0x10

#define FILL_SOLID      0x00
#define FILL_LINEAR     0x10  // Gradient
#define FILL_RADIAL     0x12
#define FILL_TILED      0x40  // Bitmap
#define FILL_CLIPPED    0x41

void swf_ShapeFree(SHAPE * s)
{ if (s)
  { if (s->linestyle.data) free(s->linestyle.data);
    s->linestyle.data = NULL;
    s->linestyle.n    = 0;
    if (s->fillstyle.data) free(s->fillstyle.data);
    s->fillstyle.data = NULL;
    s->fillstyle.n    = 0;
    if (s->data) free(s->data);
    s->data = NULL;
  }
  free(s);
}

int swf_ShapeNew(SHAPE * * s)
{ SHAPE * sh;
  if (!s) return -1;
  sh = (SHAPE *)malloc(sizeof(SHAPE)); s[0] = sh;
  if (sh) memset(sh,0x00,sizeof(SHAPE));
  return sh?0:-1;
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
  
  if (sh->data) free(sh->data);
  sh->data = (U8*)malloc(len);
  
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
  
  // no gradients yet!
  
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
  { swf_SetU8(t,n);
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
    
  s->bits.fill = swf_CountBits(s->fillstyle.n,0);
  s->bits.line = swf_CountBits(s->linestyle.n,0);

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

int swf_ShapeExport(int handle,SHAPE * s)  // without Linestyle/Fillstyle Record
{ int l;
  if (!s) return 0;

  l = sizeof(SHAPE);

  if (handle>=0)
    if (write(handle,s,sizeof(SHAPE))!=sizeof(SHAPE)) return -1;

  // Fillstyle, Linestyle ...

  if (s->data)
  { int ll = (s->bitlen+7)/8;
    l+=ll;
    if (handle>=0)
      if (write(handle,s->data,ll)!=ll) return -1;
  }

  return l;
}

int swf_ShapeImport(int handle,SHAPE * * shape)
{ SHAPE * s;

  if (handle<0) return -1;

  s = (SHAPE *)malloc(sizeof(SHAPE)); shape[0] = s;
  if (!s) return -1;

  if (read(handle,s,sizeof(SHAPE))!=sizeof(SHAPE))
  { shape[0] = NULL;
    free(s);
    return -1;
  }

  if (s->data)
  { int ll = (s->bitlen+7)/8;
    s->data = (U8*)malloc(ll);
    if (!s->data)
    { shape[0] = NULL;
      free(s);
      return -1;
    }
    if (read(handle,s->data,ll)!=ll)
    { free(s->data);
      free(s);
      shape[0] = NULL;
      return -1;
    }
  }

  return 0;
}

int swf_ShapeAddFillStyle(SHAPE * s,U8 type,MATRIX * m,RGBA * color,U16 id_bitmap)
{ RGBA def_c;
  MATRIX def_m;    

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

  // handle memory
  
  if (s->fillstyle.data)
  { FILLSTYLE * new = (FILLSTYLE *)realloc(s->fillstyle.data,(s->fillstyle.n+1)*sizeof(FILLSTYLE));
    if (!new) return -1;
    s->fillstyle.data = new;
  }
  else
  { s->fillstyle.data = (FILLSTYLE *)malloc(sizeof(FILLSTYLE));
    s->fillstyle.n = 0;
    if (!s->fillstyle.data) return -1;
  }

  // set fillstyle  (no gradients yet!)
  
  s->fillstyle.data[s->fillstyle.n].type = type; 
  s->fillstyle.data[s->fillstyle.n].id_bitmap = id_bitmap;
  memcpy(&s->fillstyle.data[s->fillstyle.n].m,m,sizeof(MATRIX));
  memcpy(&s->fillstyle.data[s->fillstyle.n].color,color,sizeof(RGBA));
          
  return (++s->fillstyle.n);
}

int swf_ShapeAddSolidFillStyle(SHAPE * s,RGBA * color)
{ return swf_ShapeAddFillStyle(s,FILL_SOLID,NULL,color,0);
}

int swf_ShapeAddBitmapFillStyle(SHAPE * s,MATRIX * m,U16 id_bitmap,int clip)
{ return swf_ShapeAddFillStyle(s,clip?FILL_CLIPPED:FILL_TILED,m,NULL,id_bitmap);
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
  { LINESTYLE * new = (LINESTYLE *)realloc(s->linestyle.data,(s->linestyle.n+1)*sizeof(LINESTYLE));
    if (!new) return -1;
    s->linestyle.data = new;
  }
  else
  { s->linestyle.data = (LINESTYLE *)malloc(sizeof(LINESTYLE));
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
  
  swf_SetBits(t,b,5);
  swf_SetBits(t,x,b);
  swf_SetBits(t,y,b);

  return 0;
}

int swf_ShapeSetStyle(TAG * t,SHAPE * s,U16 line,U16 fill0,U16 fill1)
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
#define FILL_RESET 0x8000
#define LINE_RESET 0x8000

int swf_ShapeSetAll(TAG * t,SHAPE * s,S32 x,S32 y,U16 line,U16 fill0,U16 fill1)
{ U8 b;
  if ((!t)||(!s)) return -1;

  swf_SetBits(t,0,1);
  swf_SetBits(t,SF_MOVETO|(line?SF_LINE:0)|(fill0?SF_FILL0:0)|(fill1?SF_FILL1:0),5);

  b = swf_CountBits(x,0);
  b = swf_CountBits(y,b);
  swf_SetBits(t,b,5);
  swf_SetBits(t,x,b);
  swf_SetBits(t,y,b);

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
{ U8 b;
  if (!t) return -1;
  swf_SetBits(t,3,2); // Straight Edge

  if ((!s)||((x!=0)&&(y!=0)))
  { b = swf_CountBits(x,2);
    b = swf_CountBits(y,b);
    if (b<2) b=2;
    swf_SetBits(t, b-2, 4);
    swf_SetBits(t,1,1);
    swf_SetBits(t,x,b);
    swf_SetBits(t,y,b);
    return 0;
  }

  if (x==0)
  { b = swf_CountBits(y,2);
    if(b<2) 
        b=2;
    swf_SetBits(t, b-2, 4);
    swf_SetBits(t,1,2);
    swf_SetBits(t,y,b);
  } 
  else
  { b = swf_CountBits(x,2);
    if(b<2) 
        b=2;
    swf_SetBits(t, b-2, 4);
    swf_SetBits(t,0,2);
    swf_SetBits(t,x,b);
  }
  return 0;
}

int swf_ShapeSetCurve(TAG * t,SHAPE * s,S32 x,S32 y,S32 ax,S32 ay)
{ U8 b;
  if (!t) return -1;

  swf_SetBits(t,2,2);

  b = swf_CountBits(ax,2);
  b = swf_CountBits(ay,b);
  b = swf_CountBits(x,b);
  b = swf_CountBits(y,b);

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

/* todo: merge this with swf_GetSimpleShape */
SHAPELINE* swf_ParseShapeData(U8*data, int bits, int fillbits, int linebits)
{
    SHAPELINE _lines;
    SHAPELINE*lines = &_lines;

    TAG _tag;
    TAG* tag = &_tag;
    int fill0 = 0;
    int fill1 = 0;
    int line = 0;
    int x=0,y=0;
    
    memset(tag, 0, sizeof(TAG));
    tag->data = data;
    tag->len = tag->memsize = (bits+7)/8;
    tag->pos = 0;

    lines->next = 0;
    while(1) {
	int flags;
	flags = swf_GetBits(tag, 1);
	if(!flags) { //style change
	    flags = swf_GetBits(tag, 5);
	    if(!flags)
		break;
	    if(flags&2)
		fill0 = swf_GetBits(tag, fillbits); 
	    if(flags&4)
		fill1 = swf_GetBits(tag, fillbits); 
	    if(flags&8)
		line = swf_GetBits(tag, linebits); 
	    if(flags&16) {
		return 0;
		//enumerateUsedIDs_styles(tag, callback, callback_data, num);
		fillbits = swf_GetBits(tag, 4);
		linebits = swf_GetBits(tag, 4);
	    }
	    if(flags&1) { //move
		int n = swf_GetBits(tag, 5); 
		x = swf_GetSBits(tag, n); //x
		y = swf_GetSBits(tag, n); //y

		lines->next = (SHAPELINE*)malloc(sizeof(SHAPELINE));
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
		lines->next = (SHAPELINE*)malloc(sizeof(SHAPELINE));
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

		lines->next = (SHAPELINE*)malloc(sizeof(SHAPELINE));
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

SRECT swf_GetShapeBoundingBox(SHAPELINE*shape)
{
    SRECT r;
    int SCOORD_MAX = 0x7fffffff;
    int SCOORD_MIN = -0x80000000;
    r.xmin = r.ymin = SCOORD_MAX;
    r.xmax = r.ymax = SCOORD_MIN;
    if(!shape) {
	memset(&r, 0, sizeof(SRECT));
	return r;
    }
    while(shape) {
	if(shape->x < r.xmin) r.xmin = shape->x;
	if(shape->y < r.ymin) r.ymin = shape->y;
	if(shape->x > r.xmax) r.xmax = shape->x;
	if(shape->y > r.ymax) r.ymax = shape->y;
	if(shape->type == splineTo) {
	    if(shape->sx < r.xmin) r.xmin = shape->sx;
	    if(shape->sy < r.ymin) r.ymin = shape->sy;
	    if(shape->sx > r.xmax) r.xmax = shape->sx;
	    if(shape->sy > r.ymax) r.ymax = shape->sy;
	}
	shape = shape->next;
    }
    return r;
}

void swf_Shape2Free(SHAPE2 * s)
{
    SHAPELINE*line = s->lines;
    while(line) {
	SHAPELINE*next = line->next;
	free(line);
	line = next;
    }
    if(s->linestyles)
	free(s->linestyles);
    if(s->fillstyles)
	free(s->fillstyles);
    if(s->bbox)
	free(s->bbox);
}

SHAPE2* swf_ShapeToShape2(SHAPE*shape) {

    SHAPE2*shape2 = (SHAPE2*)malloc(sizeof(SHAPE2));
    
    shape2->numlinestyles = shape->linestyle.n;
    shape2->linestyles = (LINESTYLE*)malloc(sizeof(LINESTYLE)*shape->linestyle.n);
    memcpy(shape2->linestyles, shape->linestyle.data, sizeof(LINESTYLE)*shape->linestyle.n);
    
    shape2->numfillstyles = shape->fillstyle.n;
    shape2->fillstyles = (FILLSTYLE*)malloc(sizeof(FILLSTYLE)*shape->fillstyle.n);
    memcpy(shape2->fillstyles, shape->fillstyle.data, sizeof(FILLSTYLE)*shape->fillstyle.n);

    shape2->lines = swf_ParseShapeData(shape->data, shape->bitlen, shape->bits.fill, shape->bits.line);
    shape2->bbox = 0;
    return shape2;
};

/*LINESTYLE* parseFillStyleArray(TAG*tag)
{
    U16 count;
    int t;
    int num=0;
    if(tag->id == ST_DEFINESHAPE)
	num = 1;
    else if(tag->id == ST_DEFINESHAPE2)
	num = 2;
    else if(tag->id == ST_DEFINESHAPE3)
	num = 3;
    else return 0;

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
	    swf_GetGradient(tag, NULL, alpha num>=3?1:0);
	}
	else if(type == 0x40 || type == 0x41)
	{
	    swf_ResetReadBits(tag);
	    // we made it.
	    if(tag->data[tag->pos] != 0xff ||
	       tag->data[tag->pos+1] != 0xff)
	    //(callback)(tag, tag->pos, callback_data);

	    swf_GetU16(tag);
	    swf_ResetReadBits(tag);
	    swf_GetMatrix(tag, NULL);
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
	swf_GetU16(tag);
	if(num == 3)
	    swf_GetRGBA(tag, NULL);
	else
	    swf_GetRGB(tag, NULL);
    }
}*/

/*SHAPE2* parseDefineShape(TAG*tag)
{
    int num = 0;
    SHAPE2*shape;
    U16 fill,line;
    if(tag->id == ST_DEFINESHAPE)
	num = 1;
    else if(tag->id == ST_DEFINESHAPE2)
	num = 2;
    else if(tag->id == ST_DEFINESHAPE3)
	num = 3;
    else return 0;

    swf_GetU16();

    shape = (SHAPE2*)malloc(sizeof(SHAPE2));

//    LINESTYLE * linestyles;
//    int numlinestyles;
//    FILLSTYLE* fillstyles;
//    int numfillstyles;
//    struct _SHAPELINE * lines;
//    SRECT* bbox; // may be NULL 

    swf_GetRect();
    swf_ResetReadBits(t); 
    fill = (U16)swf_GetBits(t,4);
    line = (U16)swf_GetBits(t,4);

    swf_Get

    




    return 0;
}*/


SHAPE*	   swf_Shape2ToShape(SHAPE2*shape)
{
    /* todo */
    fprintf(stderr, "Not implemented yet!\n");
    exit(1);
    return 0;
}

int	   swf_SetShape2(TAG*tag, SHAPE2*shape)
{
    /* todo */
    fprintf(stderr, "Not implemented yet!\n");
    exit(1);
    return 0;
}

