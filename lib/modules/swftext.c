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

#define FF2_BOLD         0x01
#define FF2_ITALIC       0x02
#define FF2_WIDECODES    0x04
#define FF2_WIDEOFFSETS  0x08
#define FF2_ANSI         0x10
#define FF2_UNICODE      0x20
#define FF2_SHIFTJIS     0x40
#define FF2_LAYOUT	 0x80

int swf_FontIsItalic(SWFFONT * f) { return f->style&FONT_STYLE_ITALIC;}
int swf_FontIsBold(SWFFONT * f)   { return f->style&FONT_STYLE_BOLD;}

static const int WRITEFONTID = 0x4e46; // font id for WriteFont and ReadFont

int swf_FontEnumerate(SWF * swf,void (*FontCallback) (U16,U8*))
{ int n;
  TAG * t;
  if (!swf) return -1;
  t = swf->firstTag;
  n = 0;

  while (t)
  { if (swf_GetTagID(t)==ST_DEFINEFONTINFO ||
	  swf_GetTagID(t)==ST_DEFINEFONT2)
    { n++;
      if (FontCallback)
      { U16 id;
        int l;
        U8 s[257];
        swf_SaveTagPos(t);
        swf_SetTagPos(t,0);
	
        id  = swf_GetU16(t);
	if(swf_GetTagID(t) == ST_DEFINEFONT2)
	    swf_GetU16(t);
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

int swf_FontExtract_DefineFont(int id,SWFFONT * f,TAG * t)
{ U16 fid;
  swf_SaveTagPos(t);
  swf_SetTagPos(t,0);

  fid = swf_GetU16(t);
  if ((!id)||(id==fid))
  { U16 of,*ofs;
    int n,i;
      
    id = fid;
    f->version = 1;
    f->id = fid;

    of = swf_GetU16(t);
    n = of/2;
    f->numchars = n;
    f->glyph = malloc(sizeof(SWFGLYPH)*n);
    memset(f->glyph, 0, sizeof(SWFGLYPH)*n);

    for (i=1;i<n;i++) swf_GetU16(t);
    for (i=0;i<n;i++) swf_GetSimpleShape(t,&f->glyph[i].shape);
  }

  swf_RestoreTagPos(t);
  return id;
}

int swf_FontExtract_DefineFontInfo(int id,SWFFONT * f,TAG * t)
{ U16 fid;
  U16 maxcode;
  U8 flags;
  swf_SaveTagPos(t);
  swf_SetTagPos(t,0);

  fid = swf_GetU16(t);
  if (fid==id)
  { U8 l = swf_GetU8(t);
    int i;
  
    if(f->version>1) {
      // DefineFont2 doesn't have FontInfo fields
      fprintf(stderr, "fixme: FontInfo field for DefineFont2 encountered\n");
      return -1;
    }
    
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
    flags = swf_GetU8(t);
    if(flags & 2)
	f->style |= FONT_STYLE_BOLD;
    if(flags & 4)
	f->style |= FONT_STYLE_ITALIC;
    if(flags & 8)
	f->encoding |= FONT_ENCODING_ANSI;
    if(flags & 16)
	f->encoding |= FONT_ENCODING_SHIFTJIS;
    if(flags & 32)
	f->encoding |= FONT_ENCODING_UNICODE;

    f->glyph2ascii = (U16*)malloc(sizeof(U16)*f->numchars);
    maxcode = 0;
    for(i=0; i < f->numchars; i++) {
      f->glyph2ascii[i] = ((flags&FF_WIDECODES)?swf_GetU16(t):swf_GetU8(t));
      if(f->glyph2ascii[i] > maxcode)
	  maxcode = f->glyph2ascii[i];
    }
    maxcode++;
    if(maxcode<256)
	maxcode=256;
    f->maxascii = maxcode;
    f->ascii2glyph = (int*)malloc(sizeof(int)*maxcode);
    memset(f->ascii2glyph, -1, sizeof(int)*maxcode);
     
    for(i = 0; i < f->numchars; i++)
      f->ascii2glyph[f->glyph2ascii[i]] = i;
  }

  swf_RestoreTagPos(t);
  return id;
}

int swf_FontExtract_DefineFont2(int id,SWFFONT * font,TAG * tag)
{
    int t, glyphcount;
    int maxcode;
    int fid;
    U8 flags1,flags2,namelen;
    swf_SaveTagPos(tag);
    swf_SetTagPos(tag,0);
    font->version=2;
    fid = swf_GetU16(tag);
    if(id && id!=fid)
	return id;
    font->id = fid;
    flags1 = swf_GetU8(tag);
    flags2 = swf_GetU8(tag); //reserved flags

    if(flags1 & 1)
	font->style |= FONT_STYLE_BOLD;
    if(flags1 & 2)
	font->style |= FONT_STYLE_ITALIC;
    if(flags1 & 16)
	font->encoding |= FONT_ENCODING_ANSI;
    if(flags1 & 32)
	font->encoding |= FONT_ENCODING_UNICODE;
    if(flags1 & 64)
	font->encoding |= FONT_ENCODING_SHIFTJIS;

    namelen = swf_GetU8(tag);
    font->name = (U8*)malloc(namelen+1);
    font->name[namelen]=0;
    swf_GetBlock(tag, font->name, namelen);
    font->version = 2;
    glyphcount = swf_GetU16(tag);
    font->numchars = glyphcount;
    
    font->glyph = (SWFGLYPH*)malloc(sizeof(SWFGLYPH)*glyphcount);
    memset(font->glyph, 0, sizeof(SWFGLYPH)*glyphcount);
    font->glyph2ascii = (U16*)malloc(sizeof(U16)*glyphcount);
    memset(font->glyph2ascii, 0, sizeof(U16)*glyphcount);

    if(flags1&8) { // wide offsets
	for(t=0;t<glyphcount;t++)
	    swf_GetU32(tag); //offset[t]
	swf_GetU32(tag); // fontcodeoffset
    } else {
	for(t=0;t<glyphcount;t++)
	    swf_GetU16(tag); //offset[t]
	swf_GetU16(tag); // fontcodeoffset
    }
    for(t=0;t<glyphcount;t++)
	swf_GetSimpleShape(tag,&(font->glyph[t].shape));

    maxcode = 0;
    for(t=0;t<glyphcount;t++) {
	int code;
	if(flags1&4) // wide codes
	    code = swf_GetU16(tag);
	else
	    code = swf_GetU8(tag);
	font->glyph2ascii[t] = code;
	if(code > maxcode)
	    maxcode = code;
    }
    maxcode++;
    if(maxcode<256)
	maxcode=256;
    font->maxascii = maxcode;
    font->ascii2glyph = (int*)malloc(sizeof(int)*maxcode);
    memset(font->ascii2glyph, -1, sizeof(int)*maxcode);
    for(t=0;t<glyphcount;t++) 
    {
	font->ascii2glyph[font->glyph2ascii[t]] = t;
    }

    if(flags1&128) { // has layout
	U16 kerningcount;
	font->layout = (SWFLAYOUT*)malloc(sizeof(SWFLAYOUT));
	font->layout->ascent=swf_GetU16(tag);
	font->layout->descent=swf_GetU16(tag);
	font->layout->leading=swf_GetU16(tag);
	for(t=0;t<glyphcount;t++) {
	    S16 advance = swf_GetS16(tag);
	    font->glyph[t].advance = advance;
	}
	font->layout->bounds = malloc(glyphcount*sizeof(SRECT));
	for(t=0;t<glyphcount;t++) {
	    swf_ResetReadBits(tag);
	    swf_GetRect(tag, font->layout->bounds);
	}
	kerningcount = swf_GetU16(tag);
	font->layout->kerningcount = kerningcount;
	font->layout->kerning = (SWFKERNING*)malloc(sizeof(SWFKERNING)*kerningcount);
	if(kerningcount) {
	    font->layout->kerning = 
		malloc(sizeof(*font->layout->kerning)* kerningcount);
	    for(t=0;t<kerningcount;t++)
	    {
		if(flags1&4) { // wide codes
		    font->layout->kerning[t].char1 = swf_GetU16(tag);
		    font->layout->kerning[t].char2 = swf_GetU16(tag);
		} else {
		    font->layout->kerning[t].char1 = swf_GetU8(tag);
		    font->layout->kerning[t].char2 = swf_GetU8(tag);
		}
		font->layout->kerning[t].adjustment = swf_GetS16(tag);
	    }
	}
    }
    swf_RestoreTagPos(t);
    return font->id;
}


#define FEDTJ_PRINT  0x01
#define FEDTJ_MODIFY 0x02
#define FEDTJ_CALLBACK 0x04

int swf_FontExtract_DefineTextCallback(int id,SWFFONT * f,TAG * t,int jobs, 
	void(*callback)(int*chars, int nr, int fontid))
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
      int buf[256];
      for (i=0;i<flags;i++)
      { int glyph;
        int adv;
        glyph = swf_GetBits(t,gbits);
        adv = swf_GetBits(t,abits);
        if (id==fid)                    // mitlesen ?
          if (jobs&FEDTJ_PRINT) {
	    { int code = f->glyph2ascii[glyph];
	      printf("%c",code);
	  }
          if (jobs&FEDTJ_MODIFY)
            /*if (!f->glyph[code].advance)*/ f->glyph[glyph].advance = adv;
        }
	buf[i] = glyph;
      }
      if ((id==fid)&&(jobs&FEDTJ_PRINT)) printf("\n");
      if (jobs&FEDTJ_CALLBACK)
	  callback(buf, flags, fid);
    }
    flags = swf_GetU8(t);
  }
  
  swf_RestoreTagPos(t);
  return id;
}  

int swf_FontExtract_DefineText(int id,SWFFONT * f,TAG * t,int jobs)
{
    return swf_FontExtract_DefineTextCallback(id,f,t,jobs,0);
}

int swf_FontExtract(SWF * swf,int id,SWFFONT * * font)
{ TAG * t;
  SWFFONT * f;
    
  if ((!swf)||(!font)) return -1;

  f = (SWFFONT *)malloc(sizeof(SWFFONT)); font[0] = f;
  if (!f) return -1;
  
  memset(f,0x00,sizeof(SWFFONT));

  t = swf->firstTag;

  while (t)
  { int nid = 0;
    switch (swf_GetTagID(t))
    { case ST_DEFINEFONT:
        nid = swf_FontExtract_DefineFont(id,f,t);
        break;
    
      case ST_DEFINEFONT2:
        nid = swf_FontExtract_DefineFont2(id,f,t);
        break;
        
      case ST_DEFINEFONTINFO:
        nid = swf_FontExtract_DefineFontInfo(id,f,t);
        break;
        
      case ST_DEFINETEXT:
      case ST_DEFINETEXT2:
        nid = swf_FontExtract_DefineText(id,f,t,f->layout?0:FEDTJ_MODIFY);
        break;
    }
    if (nid>0) id = nid;
    t = swf_NextTag(t);
  }
  return 0;
}

int swf_FontSetID(SWFFONT * f,U16 id) { if (!f) return -1; f->id = id; return 0; }

int swf_FontReduce(SWFFONT * f,FONTUSAGE * use)
{ int i,j,num;
  if ((!f)||(!use)) return -1;

  j = 0;
  for (i=0;i<f->numchars;i++)
    if (f->glyph[i].shape)
    { if (f->glyph2ascii[i]<MAX_CHAR_PER_FONT && 
	    use->code[f->glyph2ascii[i]])
      { f->ascii2glyph[f->glyph2ascii[i]] = j;
        f->glyph2ascii[j] = f->glyph2ascii[i];
	f->glyph[j] = f->glyph[i];
        j++;
      }
      else
      { swf_ShapeFree(f->glyph[i].shape);
        f->ascii2glyph[f->glyph2ascii[i]] = -1;
	f->glyph2ascii[i] = 0;
        f->glyph[i].shape   = NULL;
        f->glyph[i].advance = 0;
      }
    } else f->ascii2glyph[f->glyph2ascii[i]] = -1;

  f->numchars = j;
    
  return j;
}

int swf_FontInitUsage(FONTUSAGE * use)
{ if (!use) return -1;
  memset(use->code,0,sizeof(use->code[0])*MAX_CHAR_PER_FONT);
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
{ U16*ofs = (U16*)malloc(f->numchars*2);
  int p,i,j;
    
  if ((!t)||(!f)) return -1;
  swf_ResetWriteBits(t);
  swf_SetU16(t,f->id);

  p = 0; j = 0;
  for (i=0;i<f->numchars;i++)
    if (f->glyph[i].shape)
    { ofs[j++] = p;
      p+=swf_SetSimpleShape(NULL,f->glyph[i].shape);
    }

  for (i=0;i<j;i++) swf_SetU16(t,ofs[i]+j*2);
  
  for (i=0;i<f->numchars;i++)
    if (f->glyph[i].shape)
      swf_SetSimpleShape(t,f->glyph[i].shape);
  
  swf_ResetWriteBits(t);
  free(ofs);
  return 0;
}

int swf_FontSetDefine2(TAG *tag, SWFFONT * f)
{
    U8 flags = 0;
    int t;
    int pos;
    int pos2;
    swf_SetU16(tag, f->id);
    if(f->layout) 
	flags |= 128; // haslayout
    if(f->numchars>256)
	flags |= 4; // widecodes
    if(f->style & FONT_STYLE_BOLD)
	flags |= 1; // bold
    if(f->style & FONT_STYLE_ITALIC)
	flags |= 2; // italic
    /* wideoffs 8 */
    if(f->encoding & FONT_ENCODING_ANSI)
	flags |= 16; // ansi
    if(f->encoding & FONT_ENCODING_UNICODE)
	flags |= 32; // unicode
    if(f->encoding & FONT_ENCODING_SHIFTJIS)
	flags |= 64; // shiftjis

    swf_SetU8(tag, flags);
    swf_SetU8(tag, 0); //reserved flags
    if(f->name) {
	/* font name */
	swf_SetU8(tag, strlen(f->name));
	swf_SetBlock(tag, f->name, strlen(f->name));
    } else {
	/* font name (="") */
	swf_SetU8(tag, 0); /*placeholder*/
    }
    /* number of glyphs */
    swf_SetU16(tag, f->numchars);
    /* font offset table */
    pos = tag->len;
    for(t=0;t<f->numchars;t++)
    {
	swf_SetU16(tag, /* fontoffset */ 0); /*placeholder*/
    }
    pos2 = tag->len;
    swf_SetU16(tag, 0); //fontcode-fontoffset
    for(t=0;t<f->numchars;t++) {
	tag->data[pos + t*2] = (tag->len-pos);
	tag->data[pos + t*2 + 1] = (tag->len-pos) >> 8;
	swf_SetSimpleShape(tag, f->glyph[t].shape);
    }

    tag->data[pos2] = tag->len - pos;
    tag->data[pos2 + 1] = (tag->len - pos) >> 8;
    
    /* font code table */
    if(flags & 4) /* wide codes */ {
	for(t=0;t<f->numchars;t++)
	    swf_SetU16(tag,f->glyph2ascii[t]);
    } else {
	for(t=0;t<f->numchars;t++)
	    swf_SetU8(tag,f->glyph2ascii[t]);
    }
    if(f->layout) 
    {
	swf_SetU16(tag,f->layout->ascent);
	swf_SetU16(tag,f->layout->descent);
	swf_SetU16(tag,f->layout->leading);
	for(t=0;t<f->numchars;t++)
	    swf_SetU16(tag,f->glyph[t].advance);
	for(t=0;t<f->numchars;t++) {
	    swf_ResetWriteBits(tag);
	    swf_SetRect(tag,&f->layout->bounds[t]);
	}
	swf_SetU16(tag, f->layout->kerningcount);
	for(t=0;t<f->layout->kerningcount;t++) {
	    if(flags & 4) /* wide codes */ {
		swf_SetU8(tag,f->layout->kerning[t].char1);
		swf_SetU8(tag,f->layout->kerning[t].char2);
	    } else {
		swf_SetU16(tag,f->layout->kerning[t].char1);
		swf_SetU16(tag,f->layout->kerning[t].char2);
	    }
	    swf_SetU16(tag,f->layout->kerning[t].adjustment);
	}
    }
    return 0;
}
    
void swf_FontAddLayout(SWFFONT * f, int ascent, int descent, int leading)
{
    f->layout = (SWFLAYOUT*)malloc(sizeof(SWFLAYOUT));
    f->layout->ascent = ascent;
    f->layout->descent = descent;
    f->layout->leading = leading;
    f->layout->kerningcount = 0;
    f->layout->kerning = 0;
    f->layout->bounds = (SRECT*)malloc(sizeof(SRECT)*f->numchars);
    memset(f->layout->bounds, 0, sizeof(SRECT)*f->numchars);
}

int swf_FontSetInfo(TAG * t,SWFFONT * f)
{ int l,i;
  U8 wide=0;
  U8 flags = 0;
  if ((!t)||(!f)) return -1;
  swf_ResetWriteBits(t);
  swf_SetU16(t,f->id);
  l = strlen(f->name); if (l>255) l = 255;
  swf_SetU8(t,l);
  swf_SetBlock(t,f->name,l);
  if(f->numchars>=256)
      wide=1;

  if(f->style & FONT_STYLE_BOLD)
      flags |= 2;
  if(f->style & FONT_STYLE_ITALIC)
      flags |= 4;
  if(f->style & FONT_ENCODING_ANSI)
      flags |= 8;
  if(f->style & FONT_ENCODING_SHIFTJIS)
      flags |= 16;
  if(f->style & FONT_ENCODING_UNICODE)
      flags |= 32;
    
  swf_SetU8(t,(flags&0xfe)|wide);

  for (i=0;i<f->numchars;i++) {
    if (f->glyph[i].shape)
      wide?swf_SetU16(t,f->glyph2ascii[i]):
	   swf_SetU8(t,f->glyph2ascii[i]);
  }
  
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
/*    new kerning struct. hope commenting this out doesn't break things
      if (f->layout->kerning.data)
    { l+=f->layout->kerning.count*4;
      if (handle>=0)
        if (write(handle,f->layout->kerning.data,f->layout->kerning.count*4)!=f->layout->kerning.count*4) return -1;
    }*/
  }

  for (i=0;i<f->numchars;i++)
  { if (f->glyph[i].shape)
    { int ll = swf_ShapeExport(handle,f->glyph[i].shape);
      if (ll<0) return -1;
      l+=ll;
    }  
  }

  return l;
}

int swf_FontImport(int handle,SWFFONT * * font)
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
    /* new kerning struct. hope commenting this out doesn't break things
    if (f->layout->kerning.data)
    { int l = f->layout->kerning.count*4;
      f->layout->kerning.data = (U8*)malloc(l);
      if (!f->layout->kerning.data) goto fehler;
      if (read(handle,f->layout->kerning.data,l)!=l) goto fehler;
    } */
  }

  for (i=0;i<f->numchars;i++)
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
  { if (l->kerning) free(l->kerning);
    l->kerning = NULL;
    if (l->bounds) free(l->bounds);
    l->bounds = NULL;
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

    if(f->glyph) {
      for (i=0;i<f->numchars;i++)
	if (f->glyph[i].shape)
	{ swf_ShapeFree(f->glyph[i].shape);
	  f->glyph[i].shape = NULL;
	}
      free(f->glyph);
      f->glyph = NULL;
    }
    if(f->ascii2glyph) {
      free(f->ascii2glyph);
      f->ascii2glyph = NULL;
    }
    if(f->glyph2ascii) {
      free(f->glyph2ascii);
      f->glyph2ascii = NULL;
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
  if ((!s)||(!font)||((!gbits)&&(!abits))||(!font->ascii2glyph)) return -1;
  g = a = 0;

  while(s[0])
  { 
    int glyph = font->ascii2glyph[s[0]];
    if(glyph>=0) {
       g = swf_CountBits(glyph,g);
       a = swf_CountBits((((U32)font->glyph[glyph].advance)*scale)/100,a);
    }
    s++;
  }

  if (gbits) gbits[0] = (U8)g;
  if (abits) abits[0] = (U8)a;

  return 0;
}

int swf_TextSetCharRecord(TAG * t,SWFFONT * font,U8 * s,int scale,U8 gbits,U8 abits)
{ int l,i;
    
  if ((!t)||(!font)||(!s)||(!font->ascii2glyph)) return -1;

  l = strlen(s);
  if (l>0x7f) l = 0x7f;
  swf_SetU8(t,l);

  for (i=0;i<l;i++)
  { 
    int g = font->ascii2glyph[s[i]];
    if(g>=0) {
      swf_SetBits(t,g,gbits);
      swf_SetBits(t,(((U32)font->glyph[g].advance)*scale)/100,abits);
    }
  }

  swf_ResetWriteBits(t);
  return 0;
}

U32 swf_TextGetWidth(SWFFONT * font,U8 * s,int scale)
{ U32 res = 0;

  if (font&&s)
  { while (s[0])
    { 
      int g = font->ascii2glyph[*s];
      if(g>=0)
        res += font->glyph[g].advance;
      s++;
    }
    if (scale) res = (res*scale)/100;
  }
  return res;
}

SWFFONT* swf_ReadFont(char* filename)
{
  int f;
  SWF swf;
  if(!filename)
      return 0;
  f = open(filename,O_RDONLY);
  
  if (f<0 || swf_ReadSWF(f,&swf)<0)
  { fprintf(stderr,"%s is not a valid SWF font file or contains errors.\n",filename);
    close(f);
    return 0;
  }
  else
  { SWFFONT*font;
    close(f);
    if(swf_FontExtract(&swf, WRITEFONTID, &font) < 0)
       return 0;
    swf_FreeTags(&swf);
    return font;
  }
}

void swf_WriteFont(SWFFONT*font, char* filename)
{ SWF swf;
  TAG * t;
  SRECT r;
  RGBA rgb;
  int f;
  int useDefineFont2 = 0;

  if(font->layout)
      useDefineFont2 = 1; /* the only thing new in definefont2 
			     is layout information. */

  font->id = WRITEFONTID; //"FN"

  memset(&swf,0x00,sizeof(SWF));

  swf.fileVersion    = 4;
  swf.frameRate      = 0x4000;

  /* if we use DefineFont1 to store the characters,
     we have to build a textfield to store the
     advance values. While at it, we can also
     make the whole .swf viewable */

  /* we now always create viewable swfs, even if we
     did use definefont2 -mk*/
  t = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
  swf.firstTag = t;
	rgb.r = 0xff;
	rgb.g = 0xff;
	rgb.b = 0xff;
	swf_SetRGB(t,&rgb);
  if(!useDefineFont2) {
    t = swf_InsertTag(t,ST_DEFINEFONT);
    swf_FontSetDefine(t,font);
    t = swf_InsertTag(t,ST_DEFINEFONTINFO);
    swf_FontSetInfo(t,font);
  } else {
    t = swf_InsertTag(t,ST_DEFINEFONT2);
    swf_FontSetDefine2(t,font);
  }

  if(1) //useDefineFont2
  {     int textscale = 400;
	int s;
	int xmax = 0;
	int ymax = textscale * 2 * (font->maxascii/16+1);
	U8 gbits,abits;
	char text[MAX_CHAR_PER_FONT+1];
	int x,y;
	text[MAX_CHAR_PER_FONT]=0;
	for(s=0;s<font->maxascii;s++)
	{
	    int g = font->ascii2glyph[s];
	    text[s] = s;
	    if(g>=0) {
	       if(font->glyph[g].advance*textscale/200 > xmax)
		   xmax = font->glyph[g].advance*textscale/200;
	    }
	}
	swf.movieSize.xmax = xmax*20;
	swf.movieSize.ymax = ymax;

	t = swf_InsertTag(t,ST_DEFINETEXT);

	    swf_SetU16(t,font->id+1);            // ID

	    r.xmin = 0;
	    r.ymin = 0;
	    r.xmax = swf.movieSize.xmax*20;
	    r.ymax = swf.movieSize.ymax;
	    
	    swf_SetRect(t,&r);

	    swf_SetMatrix(t,NULL);

	    abits = swf_CountBits(xmax*16, 0);
	    gbits = 8;
	    
	    swf_SetU8(t,gbits);
	    swf_SetU8(t,abits);

	    rgb.r = 0x00;
	    rgb.g = 0x00;
	    rgb.b = 0x00;
	    for(y=0;y<=((font->maxascii-1)/16);y++)
	    {
		int c=0,lastx=-1, firstx=0;
		for(x=0;x<16;x++) {
		    int g = (y*16+x<font->maxascii)?font->ascii2glyph[y*16+x]:-1;
		    if(g>=0 && font->glyph[g].shape) {
			c++;
			if(lastx<0) 
			    lastx = x*xmax;
		    }
		}
		if(c) {
		  swf_TextSetInfoRecord(t,font,textscale,&rgb,lastx+1,textscale*y*2);
		  for(x=0;x<16;x++)
		  {
		      int g = (y*16+x<font->maxascii)?font->ascii2glyph[y*16+x]:-1;
		      if(g>=0 && font->glyph[g].shape) {
			if(lastx != x*xmax) {
			    swf_TextSetInfoRecord(t,0,0,0,x*xmax+1,0);
			}
			swf_SetU8(t,1);
			swf_SetBits(t, g, gbits);
		        swf_SetBits(t, font->glyph[g].advance, abits);
			lastx = x*xmax+font->glyph[g].advance;
			swf_ResetWriteBits(t);
		      }
		  }
		} 
	    }
	    swf_SetU8(t,0);

        
        t = swf_InsertTag(t,ST_PLACEOBJECT2);

            swf_ObjectPlace(t,font->id+1,1,NULL,NULL,NULL);
     
        t = swf_InsertTag(t,ST_SHOWFRAME);
  }
  
  t = swf_InsertTag(t,ST_END);

  f = open(filename, O_RDWR|O_CREAT|O_TRUNC,0644);
  if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed in writeFont().\n");
  close(f);

  swf_FreeTags(&swf);
}


void swf_SetEditText(TAG*tag, U16 flags, SRECT r, char*text, RGBA*color, 
	int maxlength, U16 font, U16 height, EditTextLayout*layout, char*variable)
{
    swf_SetRect(tag,&r);
    swf_ResetWriteBits(tag);

    flags &= ~(ET_HASTEXT|ET_HASTEXTCOLOR|ET_HASMAXLENGTH|ET_HASFONT|ET_HASLAYOUT);
    if(text) flags |= ET_HASTEXT;
    if(color) flags |= ET_HASTEXTCOLOR;
    if(maxlength) flags |= ET_HASMAXLENGTH;
    if(font) flags |= ET_HASFONT;
    if(layout) flags |= ET_HASLAYOUT;

    swf_SetBits(tag, flags, 16);

    if(flags & ET_HASFONT) {
	swf_SetU16(tag, font); //font
	swf_SetU16(tag, height); //fontheight
    }
    if(flags & ET_HASTEXTCOLOR) {
	swf_SetRGBA(tag, color);
    }
    if(flags & ET_HASMAXLENGTH) {
	swf_SetU16(tag, maxlength); //maxlength
    }
    if(flags & ET_HASLAYOUT) {
	swf_SetU8(tag,layout->align); //align
	swf_SetU16(tag,layout->leftmargin); //left margin
	swf_SetU16(tag,layout->rightmargin); //right margin
	swf_SetU16(tag,layout->indent); //indent
	swf_SetU16(tag,layout->leading); //leading
    }
    swf_SetString(tag, variable);
    if(flags & ET_HASTEXT)
	swf_SetString(tag,text);
}

