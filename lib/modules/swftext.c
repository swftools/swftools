/* swftext.c

   Text and font routines
      
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#define TF_TEXTCONTROL  0x80
#define TF_HASFONT      0x08
#define TF_HASCOLOR     0x04
#define TF_HASYOFFSET   0x02
#define TF_HASXOFFSET   0x01

#define FF_WIDECODES    0x01
#define FF_BOLD         0x02
#define FF_ITALIC       0x04
#define FF_ANSI         0x08
#define FF_SHIFTJIS     0x10
#define FF_UNICODE      0x20

int swf_FontEnumerate(SWF * swf,void (*FontCallback) (U16,U8*))
{ int n;
  TAG * t;
  if (!swf) return -1;
  t = swf->firstTag;
  n = 0;

  while (t)
  { if (swf_GetTagID(t)==ST_DEFINEFONTINFO)
    { n++;
      if (FontCallback)
      { U16 id;
        int l;
        U8 s[257];
        swf_SaveTagPos(t);
        swf_SetTagPos(t,0);

        id  = swf_GetU16(t);
        l   = swf_GetU8(t);
        swf_GetBlock(t,s,l);
        s[l] = 0;

        (FontCallback)(id,s); 
      
        swf_RestoreTagPos(t);
      }
    }
    t = swf_NextTag(t);
  }
  return n;
}

int swf_FontExtract_DefineFont(int id,SWFFONT * f,TAG * t,SHAPE * * shapes)
{ U16 fid;
  swf_SaveTagPos(t);
  swf_SetTagPos(t,0);

  fid = swf_GetU16(t);
  if ((!id)||(id==fid))
  { U16 ofs[MAX_CHAR_PER_FONT];
    int n,i;
      
    id = fid;
    f->id = fid;

    ofs[0] = swf_GetU16(t);
    n = ofs[0]/2;

    for (i=1;i<n;i++) if (i<MAX_CHAR_PER_FONT) ofs[i] = swf_GetU16(t); else swf_GetU16(t);
    for (i=0;i<n;i++) if (i<MAX_CHAR_PER_FONT) swf_GetSimpleShape(t,&shapes[i]);
    
  }

  swf_RestoreTagPos(t);
  return id;
}

int swf_FontExtract_DefineFontInfo(int id,SWFFONT * f,TAG * t,SHAPE * * shapes)
{ U16 fid;
  swf_SaveTagPos(t);
  swf_SetTagPos(t,0);

  fid = swf_GetU16(t);
  if (fid==id)
  { U8 l = swf_GetU8(t);
    int i;
    
    if (l)
    { if (f->name) free(f->name);
      f->name = (U8*)malloc(l+1);
      if (f->name)
      { swf_GetBlock(t,f->name,l);
        f->name[l] = 0;
      }
      else
      { swf_RestoreTagPos(t);
        return -1;
      }
    }
    f->flags = swf_GetU8(t);

    i = 0;
    while (shapes[i])
    { U16 code = ((f->flags&FF_WIDECODES)?swf_GetU16(t):swf_GetU8(t))%MAX_CHAR_PER_FONT;
        
      f->glyph[code].shape = shapes[i];
      f->glyph[code].gid   = i;
      if (i<MAX_CHAR_PER_FONT) f->codes[i] = code;

      i++;
    }
  }

  swf_RestoreTagPos(t);
  return id;
}

#define FEDTJ_PRINT  0x01
#define FEDTJ_MODIFY 0x02

int swf_FontExtract_DefineText(int id,SWFFONT * f,TAG * t,int jobs)
{ U16    cid;
  SRECT  r;
  MATRIX m;
  U8     gbits, abits, flags;
  int    fid;

  fid = 0;

  swf_SaveTagPos(t);
  swf_SetTagPos(t,0);

  cid = swf_GetU16(t);
  swf_GetRect(t,&r);
  swf_GetMatrix(t,&m);
  gbits = swf_GetU8(t);
  abits = swf_GetU8(t);

  flags = swf_GetU8(t);
  
  while(flags)
  { if (flags&TF_TEXTCONTROL)
    { if (flags&TF_HASFONT) fid = swf_GetU16(t);
      if (flags&TF_HASCOLOR)
      { swf_GetU8(t); // rgb
        swf_GetU8(t);
        swf_GetU8(t);
        if (swf_GetTagID(t)==ST_DEFINETEXT2) swf_GetU8(t);
      }
      if (flags&TF_HASXOFFSET) swf_GetS16(t);
      if (flags&TF_HASYOFFSET) swf_GetS16(t);
      if (flags&TF_HASFONT) swf_GetU16(t);
    }
    else
    { int i;
      for (i=0;i<flags;i++)
      { int glyph;
        int adv;
        glyph = swf_GetBits(t,gbits);
        adv = swf_GetBits(t,abits);
        if (id==fid)                    // mitlesen ?
        { int code = f->codes[glyph];
          if (jobs&FEDTJ_PRINT) printf("%c",code);
          if (jobs&FEDTJ_MODIFY)
            /*if (f->glyph[code].advance)*/ f->glyph[code].advance = adv;
        }
      }
      if ((id==fid)&&(jobs&FEDTJ_PRINT)) printf("\n");
    }
    flags = swf_GetU8(t);
  }
  
  swf_RestoreTagPos(t);
  return id;
}  

int swf_FontExtract(SWF * swf,int id,SWFFONT * * font)
{ TAG * t;
  SWFFONT * f;
  SHAPE * shapes[MAX_CHAR_PER_FONT];
    
  if ((!swf)||(!font)) return -1;

  f = (SWFFONT *)malloc(sizeof(SWFFONT)); font[0] = f;
  if (!f) return -1;
  
  memset(shapes,0x00,sizeof(shapes));
  memset(f,0x00,sizeof(SWFFONT));

  t = swf->firstTag;

  while (t)
  { int nid = 0;
    switch (swf_GetTagID(t))
    { case ST_DEFINEFONT:
        nid = swf_FontExtract_DefineFont(id,f,t,shapes);
        break;
        
      case ST_DEFINEFONTINFO:
        nid = swf_FontExtract_DefineFontInfo(id,f,t,shapes);
        break;
        
      case ST_DEFINETEXT:
      case ST_DEFINETEXT2:
        nid = swf_FontExtract_DefineText(id,f,t,FEDTJ_MODIFY);
        break;
    }
    if (nid>0) id = nid;
    t = swf_NextTag(t);
  }
  return 0;
}

int swf_FontIsItalic(SWFFONT * f) { return f->flags&FF_ITALIC; }
int swf_FontIsBold(SWFFONT * f)   { return f->flags&FF_BOLD; }

int swf_FontSetID(SWFFONT * f,U16 id) { if (!f) return -1; f->id = id; return 0; }

int swf_FontReduce(SWFFONT * f,FONTUSAGE * use)
{ int i,j;
  if ((!f)||(!use)) return -1;

  memset(&f->codes,0x00,sizeof(f->codes));

  j = 0;
  for (i=0;i<MAX_CHAR_PER_FONT;i++)
    if (f->glyph[i].shape)
    { if (use->code[i])
      { f->glyph[i].gid = j;
        f->codes[j] = i;
        j++;
      }
      else
      { swf_ShapeFree(f->glyph[i].shape);
        f->glyph[i].shape   = 0;
        f->glyph[i].gid     = 0;
        f->glyph[i].advance = 0;
      }
    } else f->glyph[i].gid = 0;
    
  return j;
}

int swf_FontInitUsage(FONTUSAGE * use)
{ if (!use) return -1;
  memset(&use->code,0x00,sizeof(use->code));
  return 0;
}

int swf_FontUse(FONTUSAGE * use,U8 * s)
{ if ((!use)||(!s)) return -1;
  while (s[0])
  { use->code[s[0]] = 1;
    s++;
  }
  return 0;  
}

int swf_FontSetDefine(TAG * t,SWFFONT * f)
{ U16 ofs[MAX_CHAR_PER_FONT];
  int p,i,j;
    
  if ((!t)||(!f)) return -1;
  swf_ResetBitcount(t);
  swf_SetU16(t,f->id);

  p = 0; j = 0;
  for (i=0;i<MAX_CHAR_PER_FONT;i++)
    if (f->glyph[i].shape)
    { ofs[j++] = p;
      p+=swf_SetSimpleShape(NULL,f->glyph[i].shape);
    }

  for (i=0;i<j;i++) swf_SetU16(t,ofs[i]+j*2);
  
  for (i=0;i<MAX_CHAR_PER_FONT;i++)
    if (f->glyph[i].shape)
      swf_SetSimpleShape(t,f->glyph[i].shape);
  
  swf_ResetBitcount(t);
  return 0;
}

int swf_FontSetInfo(TAG * t,SWFFONT * f)
{ int l,i;
  if ((!t)||(!f)) return -1;
  swf_ResetBitcount(t);
  swf_SetU16(t,f->id);
  l = strlen(f->name); if (l>255) l = 255;
  swf_SetU8(t,l);
  swf_SetBlock(t,f->name,l);
  swf_SetU8(t,f->flags&0xfe); // no Wide-Codes

  for (i=0;i<MAX_CHAR_PER_FONT;i++)
    if (f->glyph[i].shape)
      swf_SetU8(t,i);
  
  return 0;
}

int swf_FontExport(int handle,SWFFONT * f)
{ int l;
  int i;
  if (!f) return 0;

  l = sizeof(SWFFONT);
  if (handle>=0)
    if (write(handle,f,sizeof(SWFFONT))!=sizeof(SWFFONT)) return -1;

  if (f->name)
  { U16 ln = strlen(f->name);
    l+=2+ln;
    if (handle>=0)
    { if (write(handle,&ln,2)!=2) return -1;
      if (write(handle,f->name,ln)!=ln) return -1;
    }
  }

  if (f->layout)
  { l+=sizeof(SWFLAYOUT);
    if (handle>=0)
      if (write(handle,f->layout,sizeof(SWFLAYOUT))!=sizeof(SWFLAYOUT)) return -1;
    if (f->layout->kerning.data)
    { l+=f->layout->kerning.count*4;
      if (handle>=0)
        if (write(handle,f->layout->kerning.data,f->layout->kerning.count*4)!=f->layout->kerning.count*4) return -1;
    }
  }

  for (i=0;i<MAX_CHAR_PER_FONT;i++)
  { if (f->glyph[i].shape)
    { int ll = swf_ShapeExport(handle,f->glyph[i].shape);
      if (ll<0) return -1;
      l+=ll;
    }  
  }

  return l;
}

int FontImport(int handle,SWFFONT * * font)
{ SWFFONT * f;
  int layout;
  int i = 0;

  if ((!font)||(handle<0)) return -1;

  f = (SWFFONT *)malloc(sizeof(SWFFONT)); font[0] = f;
  if (!f) return -1;

  memset(f,0x00,sizeof(SWFFONT));
  
  if (read(handle,f,sizeof(SWFFONT))!=sizeof(SWFFONT)) goto fehler;

  layout = (f->layout)?1:0;             // avoid illegal free()
  f->layout = NULL;

  if (f->name)
  { U16 ln;
    f->name = NULL;
    if (read(handle,&ln,2)!=2) goto fehler;
    f->name = (U8*)malloc(ln+1);
    if (!f->name) goto fehler;
    if (read(handle,f->name,ln)!=ln) goto fehler;
    f->name[ln] = 0;
  }

  if (f->layout)
  { f->layout = (SWFLAYOUT *)malloc(sizeof(SWFLAYOUT));
    if (!f->layout) goto fehler;
    if (read(handle,f->layout,sizeof(SWFLAYOUT))!=sizeof(SWFLAYOUT)) goto fehler;
    if (f->layout->kerning.data)
    { int l = f->layout->kerning.count*4;
      f->layout->kerning.data = (U8*)malloc(l);
      if (!f->layout->kerning.data) goto fehler;
      if (read(handle,f->layout->kerning.data,l)!=l) goto fehler;
    }
  }

  for (i=0;i<MAX_CHAR_PER_FONT;i++)
  { if (f->glyph[i].shape)
    { if (swf_ShapeImport(handle,&f->glyph[i].shape)<0) goto fehler;
    }
  }

  f->id = 0;
  
  return 0;
  
fehler:
  if (f) for (;i<MAX_CHAR_PER_FONT;i++) f->glyph[i].shape = NULL;
  swf_FontFree(f);
  font[0] = NULL;
  return -1;
}

int swf_TextPrintDefineText(TAG * t,SWFFONT * f)
{ int id = swf_GetTagID(t);
  if ((id==ST_DEFINETEXT)||(id==ST_DEFINETEXT2)) swf_FontExtract_DefineText(f->id,f,t,FEDTJ_PRINT);
    else return -1;
  return 0;
}

void swf_LayoutFree(SWFLAYOUT * l)
{ if (l)
  { if (l->kerning.data) free(l->kerning.data);
    l->kerning.data = NULL;
  }
  free(l);
}

void swf_FontFree(SWFFONT * f)
{ if (f)
  { int i;
      
    if (f->name) free(f->name);
    if (f->layout) swf_LayoutFree(f->layout);

    f->name = NULL;
    f->layout = NULL;

    for (i=0;i<MAX_CHAR_PER_FONT;i++)
      if (f->glyph[i].shape)
      { swf_ShapeFree(f->glyph[i].shape);
        f->glyph[i].shape = NULL;
      }
  }
  free(f);
}

int swf_TextSetInfoRecord(TAG * t,SWFFONT * font,U16 size,RGBA * color,S16 dx,S16 dy)
{ U8 flags;
  if (!t) return -1;

  flags = TF_TEXTCONTROL|(font?TF_HASFONT:0)|(color?TF_HASCOLOR:0)|(dx?TF_HASXOFFSET:0)|(dy?TF_HASYOFFSET:0);

  swf_SetU8(t,flags);
  if (font) swf_SetU16(t,font->id);
  if (color)
  { if (swf_GetTagID(t)==ST_DEFINETEXT2) swf_SetRGBA(t,color);
    else swf_SetRGB(t,color);
  }
  if (dx) swf_SetS16(t,dx);
  if (dy) swf_SetS16(t,dy);
  if (font) swf_SetU16(t,size);
  
  return 0;
}

int swf_TextCountBits(SWFFONT * font,U8 * s,int scale,U8 * gbits,U8 * abits)
{ U16 g,a;
  if ((!s)||(!font)||((!gbits)&&(!abits))) return -1;
  g = a = 0;

  while(s[0])
  { g = swf_CountBits(font->glyph[s[0]].gid,g);
    a = swf_CountBits((((U32)font->glyph[s[0]].advance)*scale)/100,a);
    s++;
  }

  if (gbits) gbits[0] = (U8)g;
  if (abits) abits[0] = (U8)a;

  return 0;
}

int swf_TextSetCharRecord(TAG * t,SWFFONT * font,U8 * s,int scale,U8 gbits,U8 abits)
{ int l,i;
    
  if ((!t)||(!font)||(!s)) return -1;

  l = strlen(s);
  if (l>0x7f) l = 0x7f;
  swf_SetU8(t,l);

  for (i=0;i<l;i++)
  { swf_SetBits(t,font->glyph[s[i]].gid,gbits);
    swf_SetBits(t,(((U32)font->glyph[s[i]].advance)*scale)/100,abits);
  }

  swf_ResetBitcount(t);
  return 0;
}

U32 swf_TextGetWidth(SWFFONT * font,U8 * s,int scale)
{ U32 res = 0;

  if (font&&s)
  { while (s[0])
    { res += font->glyph[s[0]].advance;
      s++;
    }
    if (scale) res = (res*scale)/100;
  }
  return res;
}
