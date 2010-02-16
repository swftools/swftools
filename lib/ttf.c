/* ttf.c
   Parser and writer for truetype font files.

   Part of the swftools package.
   
   Copyright (c) 2010 Matthias Kramm <kramm@quiss.org> 
 
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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "bitio.h"
#include "log.h"
#include "os.h"
#include "mem.h"
#include "ttf.h"

#define TTCFTAG 0x74746366

#define OPENTYPE 0x4f54544f
#define TRUETYPE_MACOS 0x74727565
#define VERSION_1_0 0x00010000

#define TAG_OS2 0x4f532f32
#define TAG_CMAP 0x636d6170
#define TAG_GLYF 0x676c7966 //required for non opentype
#define TAG_HEAD 0x68656164 //required
#define TAG_HHEA 0x68686561 //required
#define TAG_HMTX 0x686d7478 //required
#define TAG_VHEA 0x86686561
#define TAG_VMTX 0x866d7478
#define TAG_KERN 0x6b65726e
#define TAG_LOCA 0x6c6f6361 //required for non opentype
#define TAG_MAXP 0x6d617870 //required
#define TAG_NAME 0x6e616d65
#define TAG_POST 0x706f7374
#define TAG_CFF  0x43464620 //required for opentype

/* TODO:
    fpgm - assembly instructions
    prep - assembly instructions
    cvt - constant value table
    gasp - gridfitting procedure
*/

static U32 checksum_block(U8*_data, int len)
{
    U32 sum = 0;
    U8*data = (U8*)_data;

    int pos;
    int len_minus_4 = len-4;
    for(pos=0;pos<=len_minus_4;pos+=4) {
        sum += data[pos]<<24|data[pos+1]<<16|data[pos+2]<<8|data[pos+3];
    }
    int left = len-pos;
    if(left == 1) sum+= data[pos+0]<<24;
    if(left == 2) sum+= data[pos+0]<<24|data[pos+1]<<16;
    if(left == 3) sum+= data[pos+0]<<24|data[pos+1]<<16|data[pos+2]<<8;
    return sum;
}

typedef struct _memreader {
    U8*mem;
    int pos;
    int size;
} memreader_t;

static U8 readU8(memreader_t*r)
{
    return r->mem[r->pos++];
}
static inline U16 readU16(memreader_t*r)
{
    if(r->pos+2>r->size) return 0;
    U16 val = r->mem[r->pos]<<8|
	      r->mem[r->pos+1];
    r->pos += 2;
    return val;
}
static S16 readS16(memreader_t*r)
{
    return (S16)readU16(r);
}
static U32 readU32(memreader_t*r)
{
    if(r->pos+4>r->size) return 0;
    U32 val = r->mem[r->pos]<<24|
	      r->mem[r->pos+1]<<16|
	      r->mem[r->pos+2]<<8|
	      r->mem[r->pos+3];
    r->pos += 4;
    return val;
}
static void readBlock(memreader_t*r, void*dest, int len)
{
    int remaining = r->size-r->pos;
    if(len > remaining) {
	memcpy(dest, r->mem+r->pos, remaining);
	memset(dest+remaining, 0, len - remaining);
	r->pos += remaining;
    } else {
	memcpy(dest, r->mem+r->pos, len);
	r->pos += len;
    }
}
#define INIT_READ(r,data,length,pos) memreader_t r = {(data),(pos),(length)};

static void expand(ttf_table_t*w, int newsize)
{
    int v1 = (newsize+63)&~63;
    int v2 = w->len + w->len / 2;
    w->memsize = v1>v2?v1:v2;
    w->data = rfx_realloc(w->data, w->memsize);
}
static void writeU8(ttf_table_t*w, unsigned char b)
{
    if(w->memsize<w->len+1)
	expand(w, w->len+1);
    w->data[w->len++] = b;
}
static void writeU16(ttf_table_t*w, unsigned short v)
{
    if(w->memsize<w->len+2)
	expand(w, w->len+2);
    w->data[w->len++] = v>>8;
    w->data[w->len++] = v;
}
#define writeS16 writeU16
static void writeU32(ttf_table_t*w, unsigned long v)
{
    if(w->memsize<w->len+4)
	expand(w, w->len+4);
    w->data[w->len++] = v>>24;
    w->data[w->len++] = v>>16;
    w->data[w->len++] = v>>8;
    w->data[w->len++] = v;
}
static void writeBlock(ttf_table_t*w, void*data, int len)
{
    if(w->memsize<w->len+len)
	expand(w, w->len+len);
    memcpy(w->data+w->len, data, len);
    w->len += len;
}

ttf_table_t*ttf_table_new(U32 id)
{
    ttf_table_t*t = rfx_calloc(sizeof(ttf_table_t));
    t->id = id;
    return t;
}

ttf_table_t*ttf_addtable(ttf_t*ttf, U32 id)
{
    ttf_table_t*t = ttf_table_new(id);

    ttf_table_t*before,*after=0;
    for(before=ttf->tables; before&&before->id<id; before=before->next) {
	after=before;
    }
    if(before && before->id == id) {
	msg("<error> Error: duplicate table %08x", id);
	free(before->data);
	before->data = 0;
	before->len = 0;
	return before;
    }
    
    if(!after) {
	t->next = ttf->tables;
	ttf->tables = t;
    } else {
	t->prev = after;
	t->next = after->next;
	after->next = t;
    }
    if(t->next)
	t->next->prev = t;
    return t;
}
ttf_table_t*ttf_find_table(ttf_t*ttf, U32 id)
{
    ttf_table_t*table = ttf->tables;
    while(table) {
	if(table->id == id) 
	    return table;
	table = table->next;
    }
    return 0;
}
void ttf_table_delete(ttf_t*ttf, ttf_table_t*table)
{
    if(ttf && ttf->tables == table) {
	ttf->tables = table->next;
    }
    if(table->prev) 
	table->prev->next = table->next;
    if(table->next) 
	table->next->prev = table->prev;
    free(table->data);
    free(table);
}
U32 ttf_table_checksum(ttf_table_t*t)
{
    U32 checksum = checksum_block(t->data, t->len);
    if(t->id==TAG_HEAD && t->len>=12) {
	/* the checksum for the HEAD table is calculated by masking out
	   the checksumadjust field */
	U32 adjust = t->data[8]<<24|t->data[9]<<16|t->data[10]<<8|t->data[11];
	checksum -= adjust;
    }
    return checksum;
}
static U8 printable(U8 a)
{
    if(a<32 || a==127) return '.';
    else return a;
}
static void hexdump(U8*data, int len, const char*prefix)
{
    int t;
    char ascii[32];
    printf("%s    -=> ",prefix);
    for(t=0;t<len;t++) {
	printf("%02x ", data[t]);
	ascii[t&15] = printable(data[t]);
	if((t && ((t&15)==15)) || (t==len-1))
	{
	    int s,p=((t)&15)+1;
	    ascii[p] = 0;
	    for(s=p-1;s<16;s++) {
		printf("   ");
	    }
	    if(t==len-1)
		printf(" %s\n", ascii);
	    else
		printf(" %s\n%s    -=> ",ascii,prefix);
	}
    }
}
static void ttf_table_dump(ttf_table_t*t, const char*prefix)
{
    if(!t) return;
    hexdump(t->data, t->len, prefix);
}

static table_os2_t*os2_parse(memreader_t*r)
{
    table_os2_t*os2 = rfx_calloc(sizeof(table_os2_t));
    U16 version = readU16(r);
    if(version!=0 && version!=1 && version!=2)
	msg("<warning> Unknown OS2 version: %04x", version);
    os2->xAvgCharWidth = readS16(r);
    os2->usWeightClass = readU16(r);
    os2->usWidthClass = readU16(r);
    os2->fsType = readU16(r);
    os2->ySubscriptXSize = readU16(r);
    os2->ySubscriptYSize = readU16(r);
    os2->ySubscriptXOffset = readU16(r);
    os2->ySubscriptYOffset = readU16(r);
    os2->ySuperscriptXSize = readU16(r);
    os2->ySuperscriptYSize = readU16(r);
    os2->ySuperscriptXOffset = readU16(r);
    os2->ySuperscriptYOffset = readU16(r);
    os2->yStrikeoutSize = readU16(r);
    os2->yStrikeoutPosition = readU16(r);
    os2->sFamilyClass = readU16(r);
    os2->panose_FamilyType = readU8(r);
    os2->panose_SerifStyle = readU8(r);
    os2->panose_Weight = readU8(r);
    os2->panose_Proportion = readU8(r);
    os2->panose_Contrast = readU8(r);
    os2->panose_StrokeVariation = readU8(r);
    os2->panose_ArmStyle = readU8(r);
    os2->panose_Letterform = readU8(r);
    os2->panose_Midline = readU8(r);
    os2->panose_XHeight = readU8(r);
    os2->ulCharRange[0] = readU32(r);
    os2->ulCharRange[1] = readU32(r);
    os2->ulCharRange[2] = readU32(r);
    os2->ulCharRange[3] = readU32(r);
    os2->achVendID[0] = readU8(r);
    os2->achVendID[1] = readU8(r);
    os2->achVendID[2] = readU8(r);
    os2->achVendID[3] = readU8(r);
    os2->fsSelection = readU16(r);
    os2->fsFirstCharIndex = readU16(r);
    os2->fsLastCharIndex = readU16(r);
    os2->sTypoAscender = readS16(r);
    os2->sTypoDescender = readS16(r);
    os2->sTypoLineGap = readS16(r);
    os2->usWinAscent = readU16(r);
    os2->usWinDescent = readU16(r);
    if(version<1) return os2;
    os2->ulCodePageRange1 = readU32(r);
    os2->ulCodePageRange2 = readU32(r);
    if(version<2) return os2;
    os2->sxHeight = readS16(r);
    os2->sCapHeight = readS16(r);
    os2->usDefaultChar = readU16(r);
    os2->usBreakChar = readU16(r);
    os2->usMaxContext = readU16(r);
    return os2;
}

static os2_write(ttf_t*ttf, ttf_table_t*w)
{
    table_os2_t*os2 = ttf->os2;
    U16 version=1;
    if(os2->sxHeight|os2->sCapHeight|os2->usDefaultChar|os2->usBreakChar|os2->usMaxContext) {
	version=2;
    }
    writeU16(w, version);
    writeS16(w, os2->xAvgCharWidth);
    writeU16(w, os2->usWeightClass);
    writeU16(w, os2->usWidthClass);
    writeU16(w, os2->fsType);
    writeU16(w, os2->ySubscriptXSize);
    writeU16(w, os2->ySubscriptYSize);
    writeU16(w, os2->ySubscriptXOffset);
    writeU16(w, os2->ySubscriptYOffset);
    writeU16(w, os2->ySuperscriptXSize);
    writeU16(w, os2->ySuperscriptYSize);
    writeU16(w, os2->ySuperscriptXOffset);
    writeU16(w, os2->ySuperscriptYOffset);
    writeU16(w, os2->yStrikeoutSize);
    writeU16(w, os2->yStrikeoutPosition);
    writeU16(w, os2->sFamilyClass);
    writeU8(w, os2->panose_FamilyType);
    writeU8(w, os2->panose_SerifStyle);
    writeU8(w, os2->panose_Weight);
    writeU8(w, os2->panose_Proportion);
    writeU8(w, os2->panose_Contrast);
    writeU8(w, os2->panose_StrokeVariation);
    writeU8(w, os2->panose_ArmStyle);
    writeU8(w, os2->panose_Letterform);
    writeU8(w, os2->panose_Midline);
    writeU8(w, os2->panose_XHeight);
    writeU32(w, os2->ulCharRange[0]);
    writeU32(w, os2->ulCharRange[1]);
    writeU32(w, os2->ulCharRange[2]);
    writeU32(w, os2->ulCharRange[3]);
    writeU8(w, os2->achVendID[0]);
    writeU8(w, os2->achVendID[1]);
    writeU8(w, os2->achVendID[2]);
    writeU8(w, os2->achVendID[3]);
    writeU16(w, os2->fsSelection);
    writeU16(w, os2->fsFirstCharIndex);
    writeU16(w, os2->fsLastCharIndex);
    writeS16(w, os2->sTypoAscender);
    writeS16(w, os2->sTypoDescender);
    writeS16(w, os2->sTypoLineGap);
    writeU16(w, os2->usWinAscent);
    writeU16(w, os2->usWinDescent);
    if(version<1) return;
    writeU32(w, os2->ulCodePageRange1);
    writeU32(w, os2->ulCodePageRange2);
    if(version<2) return;
    writeS16(w, os2->sxHeight);
    writeS16(w, os2->sCapHeight);
    writeU16(w, os2->usDefaultChar);
    writeU16(w, os2->usBreakChar);
    writeU16(w, os2->usMaxContext);
}
static void os2_delete(ttf_t*ttf)
{
    if(ttf->os2)
	free(ttf->os2);
    ttf->os2=0;
}

static os2_dump(ttf_t*ttf)
{
    table_os2_t*os2 = ttf->os2;
    if(!os2) return;
    printf("os2->xAvgCharWidth: %d\n", os2->xAvgCharWidth);
    printf("os2->usWeightClass: %d\n", os2->usWeightClass);
    printf("os2->usWidthClass: %d\n", os2->usWidthClass);
    printf("os2->fsType: %d\n", os2->fsType);
    printf("os2->ySubscriptXSize: %d\n", os2->ySubscriptXSize);
    printf("os2->ySubscriptYSize: %d\n", os2->ySubscriptYSize);
    printf("os2->ySubscriptXOffset: %d\n", os2->ySubscriptXOffset);
    printf("os2->ySubscriptYOffset: %d\n", os2->ySubscriptYOffset);
    printf("os2->ySuperscriptXSize: %d\n", os2->ySuperscriptXSize);
    printf("os2->ySuperscriptYSize: %d\n", os2->ySuperscriptYSize);
    printf("os2->ySuperscriptXOffset: %d\n", os2->ySuperscriptXOffset);
    printf("os2->ySuperscriptYOffset: %d\n", os2->ySuperscriptYOffset);
    printf("os2->yStrikeoutSize: %d\n", os2->yStrikeoutSize);
    printf("os2->yStrikeoutPosition: %d\n", os2->yStrikeoutPosition);
    printf("os2->sFamilyClass: %d\n", os2->sFamilyClass);
    printf("os2->panose_FamilyType: %d\n", os2->panose_FamilyType);
    printf("os2->panose_SerifStyle: %d\n", os2->panose_SerifStyle);
    printf("os2->panose_Weight: %d\n", os2->panose_Weight);
    printf("os2->panose_Proportion: %d\n", os2->panose_Proportion);
    printf("os2->panose_Contrast: %d\n", os2->panose_Contrast);
    printf("os2->panose_StrokeVariation: %d\n", os2->panose_StrokeVariation);
    printf("os2->panose_ArmStyle: %d\n", os2->panose_ArmStyle);
    printf("os2->panose_Letterform: %d\n", os2->panose_Letterform);
    printf("os2->panose_Midline: %d\n", os2->panose_Midline);
    printf("os2->panose_XHeight: %d\n", os2->panose_XHeight);
    printf("os2->ulCharRange[0]: %d\n", os2->ulCharRange[0]);
    printf("os2->ulCharRange[1]: %d\n", os2->ulCharRange[1]);
    printf("os2->ulCharRange[2]: %d\n", os2->ulCharRange[2]);
    printf("os2->ulCharRange[3]: %d\n", os2->ulCharRange[3]);
    printf("os2->achVendID[0]: %d\n", os2->achVendID[0]);
    printf("os2->achVendID[1]: %d\n", os2->achVendID[1]);
    printf("os2->achVendID[2]: %d\n", os2->achVendID[2]);
    printf("os2->achVendID[3]: %d\n", os2->achVendID[3]);
    printf("os2->fsSelection: %d\n", os2->fsSelection);
    printf("os2->fsFirstCharIndex: %d\n", os2->fsFirstCharIndex);
    printf("os2->fsLastCharIndex: %d\n", os2->fsLastCharIndex);
    printf("os2->sTypoAscender: %d\n", os2->sTypoAscender);
    printf("os2->sTypoDescender: %d\n", os2->sTypoDescender);
    printf("os2->sTypoLineGap: %d\n", os2->sTypoLineGap);
    printf("os2->usWinAscent: %d\n", os2->usWinAscent);
    printf("os2->usWinDescent: %d\n", os2->usWinDescent);
    printf("os2->ulCodePageRange1: %d\n", os2->ulCodePageRange1);
    printf("os2->ulCodePageRange2: %d\n", os2->ulCodePageRange2);
    printf("os2->sxHeight: %d\n", os2->sxHeight);
    printf("os2->sCapHeight: %d\n", os2->sCapHeight);
    printf("os2->usDefaultChar: %d\n", os2->usDefaultChar);
    printf("os2->usBreakChar: %d\n", os2->usBreakChar);
    printf("os2->usMaxContext: %d\n", os2->usMaxContext);
}

static int head_parse(ttf_t*ttf, memreader_t*r)
{
    ttf->head = rfx_calloc(sizeof(table_head_t));
    U32 version = readU32(r);
    if(version!=VERSION_1_0) 
	msg("<warning> Font HEAD has unknown version %08x", version);
    U32 revision = readU32(r);
    if(revision!=VERSION_1_0) 
	msg("<warning> Font HEAD has unknown revision %08x", revision);
    U32 checksum2 = readU32(r);
    U32 magic = readU32(r);
    if(magic!=0x5f0f3cf5) 
	msg("<warning> Font HEAD has unknown magic number %08x", magic);
    ttf->head->flags = readU16(r);
    ttf->head->units_per_em = readU16(r);
    readU32(r);readU32(r); //created
    readU32(r);readU32(r); //modified
    ttf->head->xmin = readU16(r);
    ttf->head->ymin = readU16(r);
    ttf->head->xmax = readU16(r);
    ttf->head->ymax = readU16(r);
    ttf->head->macStyle = readU16(r);
    ttf->head->lowest_readable_size = readU16(r); //in pixels
    ttf->head->dir_hint = readS16(r);
    int loc_index = readS16(r); //used in 'loca' table
    if(loc_index>1)
	msg("<warning> loca index format %d unknown", loc_index);
    U16 glyph_data_format = readS16(r);
    if(glyph_data_format!=0)
	msg("<warning> Font glyph data format unknown: %04x", glyph_data_format);
    return loc_index;
}

static void head_write(ttf_t*ttf, ttf_table_t*w, int loca_size)
{
    writeU32(w, 0x10000);
    writeU32(w, 0x10000);
    writeU32(w, 0); //checksum
    writeU32(w, 0x5f0f3cf5); //magic
    writeU16(w, ttf->head->flags);
    writeU16(w, ttf->head->units_per_em);
    writeU32(w, 0);writeU32(w, 0); //created
    writeU32(w, 0);writeU32(w, 0); //modified
    writeU16(w, ttf->head->xmin);
    writeU16(w, ttf->head->ymin);
    writeU16(w, ttf->head->xmax);
    writeU16(w, ttf->head->ymax);
    writeU16(w, ttf->head->macStyle);
    writeU16(w, ttf->head->lowest_readable_size);
    writeS16(w, ttf->head->dir_hint);
    writeS16(w, loca_size); //loca index size (32 bit)
    writeS16(w, 0); //glyph data format
}
static void head_dump(ttf_t*ttf)
{
    printf("head->flags: %d\n", ttf->head->flags);
    printf("head->units_per_em: %d\n", ttf->head->units_per_em);
    printf("head->xmin: %d\n", ttf->head->xmin);
    printf("head->ymin: %d\n", ttf->head->ymin);
    printf("head->xmax: %d\n", ttf->head->xmax);
    printf("head->ymax: %d\n", ttf->head->ymax);
    printf("head->macStyle: %d\n", ttf->head->macStyle);
    printf("head->lowest_readable_size: %d\n", ttf->head->lowest_readable_size);
    printf("head->dir_hint: %d\n", ttf->head->dir_hint);
}
static void head_delete(ttf_t*ttf)
{
    if(ttf->head) {
	free(ttf->head);
	ttf->head=0;
    }
}

static table_maxp_t* maxp_parse(ttf_t*ttf, memreader_t*r)
{
    U32 version = readU32(r);
    ttf->num_glyphs = readU16(r);
    /* according to freetype, older fonts (version<0x10000) 
       apparently only contain the number of glyphs. this is
       rather rare, though. */
    if(version<0x10000) return 0;

    table_maxp_t*maxp = rfx_calloc(sizeof(table_maxp_t));
    maxp->maxPoints = readU16(r);
    maxp->maxContours = readU16(r);
    maxp->maxComponentPoints = readU16(r);
    maxp->maxComponentContours = readU16(r);
    maxp->maxZones = readU16(r);
    maxp->maxTwilightPoints = readU16(r);
    maxp->maxStorage = readU16(r);
    maxp->maxFunctionDefs = readU16(r);
    maxp->maxInstructionDefs = readU16(r);
    maxp->maxStackElements = readU16(r);
    maxp->maxSizeOfInstructions = readU16(r);
    maxp->maxComponentElements = readU16(r);
    maxp->maxComponentDepth = readU16(r);
    return maxp;
}

static void maxp_write(ttf_t*ttf, ttf_table_t*w)
{
    table_maxp_t*maxp = ttf->maxp;
    writeU32(w, 0x10000); //version
    writeU16(w, ttf->num_glyphs);
    writeU16(w, maxp->maxPoints);
    writeU16(w, maxp->maxContours);
    writeU16(w, maxp->maxComponentPoints);
    writeU16(w, maxp->maxComponentContours);
    writeU16(w, maxp->maxZones);
    writeU16(w, maxp->maxTwilightPoints);
    writeU16(w, maxp->maxStorage);
    writeU16(w, maxp->maxFunctionDefs);
    writeU16(w, maxp->maxInstructionDefs);
    writeU16(w, maxp->maxStackElements);
    writeU16(w, maxp->maxSizeOfInstructions);
    writeU16(w, maxp->maxComponentElements);
    writeU16(w, maxp->maxComponentDepth);
}

static void maxp_dump(ttf_t*ttf)
{
    table_maxp_t*maxp = ttf->maxp;
    if(!maxp) return;
    printf("maxp->maxPoints: %d\n", maxp->maxPoints);
    printf("maxp->maxContours: %d\n", maxp->maxContours);
    printf("maxp->maxComponentPoints: %d\n", maxp->maxComponentPoints);
    printf("maxp->maxComponentContours: %d\n", maxp->maxComponentContours);
    printf("maxp->maxZones: %d\n", maxp->maxZones);
    printf("maxp->maxTwilightPoints: %d\n", maxp->maxTwilightPoints);
    printf("maxp->maxStorage: %d\n", maxp->maxStorage);
    printf("maxp->maxFunctionDefs: %d\n", maxp->maxFunctionDefs);
    printf("maxp->maxInstructionDefs: %d\n", maxp->maxInstructionDefs);
    printf("maxp->maxStackElements: %d\n", maxp->maxStackElements);
    printf("maxp->maxSizeOfInstructions: %d\n", maxp->maxSizeOfInstructions);
    printf("maxp->maxComponentElements: %d\n", maxp->maxComponentElements);
    printf("maxp->maxComponentDepth: %d\n", maxp->maxComponentDepth);
}

static void maxp_delete(ttf_t*ttf)
{
    if(ttf->maxp)
	free(ttf->maxp);
    ttf->maxp=0;
}


static int hea_parse(memreader_t*r, ttf_t*ttf)
{
    table_hea_t*hea = ttf->hea = rfx_calloc(sizeof(table_hea_t));
    U32 version = readU32(r);
    hea->ascent = readS16(r);
    hea->descent = readS16(r);
    hea->lineGap = readS16(r);
    hea->advanceWidthMax = readU16(r);
    hea->minLeftSideBearing = readS16(r);
    hea->minRightSideBearing = readS16(r);
    hea->xMaxExtent = readS16(r);
    hea->caretSlopeRise = readS16(r);
    hea->caretSlopeRun = readS16(r);
    hea->caretOffset = readS16(r);
    readS16(r); //reserved[0]
    readS16(r); //reserved[1]
    readS16(r); //reserved[2]
    readS16(r); //reserved[3]
    S16 metricDataFormat = readS16(r); //should be 0
    if(metricDataFormat!=0) {
	msg("<warning> Unknown metric format %d", metricDataFormat);
    }
    int num_advances = readU16(r);
    if(num_advances > ttf->num_glyphs) {
	msg("<warning> bad number of horizontal metrics: %d", num_advances);
	num_advances = ttf->num_glyphs;
    }
    return num_advances;
}

static table_hea_t*hea_write(ttf_t*ttf, ttf_table_t*w, int num_advances)
{
    table_hea_t*hea = ttf->hea;
    writeU32(w, 0x00010000);
    writeS16(w, hea->ascent);
    writeS16(w, hea->descent);
    writeS16(w, hea->lineGap);
    writeU16(w, hea->advanceWidthMax);
    writeS16(w, hea->minLeftSideBearing);
    writeS16(w, hea->minRightSideBearing);
    writeS16(w, hea->xMaxExtent);
    writeS16(w, hea->caretSlopeRise);
    writeS16(w, hea->caretSlopeRun);
    writeS16(w, hea->caretOffset);
    writeS16(w, 0); //reserved
    writeS16(w, 0); //reserved
    writeS16(w, 0); //reserved
    writeS16(w, 0); //reserved
    writeS16(w, 0); //metricDataFormat
    writeU16(w, num_advances);
    return hea;
}
static void hea_dump(ttf_t*ttf)
{
    table_hea_t*hea = ttf->hea;
    const char*dir = ttf->is_vertical?"v":"h";
    printf("%shea->ascent: %d\n", dir, hea->ascent);
    printf("%shea->descent: %d\n", dir, hea->descent);
    printf("%shea->lineGap: %d\n", dir, hea->lineGap);
    printf("%shea->advanceWidthMax: %d\n", dir, hea->advanceWidthMax);
    printf("%shea->minLeftSideBearing: %d\n", dir, hea->minLeftSideBearing);
    printf("%shea->minRightSideBearing: %d\n", dir, hea->minRightSideBearing);
    printf("%shea->xMaxExtent: %d\n", dir, hea->xMaxExtent);
    printf("%shea->caretSlopeRise: %d\n", dir, hea->caretSlopeRise);
    printf("%shea->caretSlopeRun: %d\n", dir, hea->caretSlopeRun);
    printf("%shea->caretOffset: %d\n", dir, hea->caretOffset);
}
static void hea_delete(ttf_t*ttf)
{
    if(ttf->hea) {
	free(ttf->hea);
	ttf->hea=0;
    }
}

static void hmtx_parse(memreader_t*r, ttf_t*ttf, int num_advances)
{
    U16 old_advance = 0;
    int t;
    if(num_advances > r->size/4)
	num_advances = r->size/4;
    for(t=0;t<num_advances;t++) {
	old_advance = ttf->glyphs[t].advance = readU16(r);
	ttf->glyphs[t].bearing = readS16(r);
    }
    int rest = (r->size - num_advances*4)/2;
    if(ttf->num_glyphs < num_advances+rest) {
	rest = ttf->num_glyphs-num_advances;
    }
    for(t=0;t<rest;t++) {
	ttf->glyphs[t].advance = old_advance; 
	ttf->glyphs[t].bearing = readS16(r);
    }
}
static int mtx_write(ttf_t*ttf, ttf_table_t*w)
{
    int num_advances = ttf->num_glyphs;
    if(ttf->num_glyphs>=2) {
	int t;
	for(t=ttf->num_glyphs-1;t>0;t--) {
	    if(ttf->glyphs[t-1].advance !=
	       ttf->glyphs[t].advance) break;
	}
	/* we need to store all individual advances as well
	   as one entry for the constant */
	num_advances = t+1;
    }

    int t;
    for(t=0;t<num_advances;t++) {
	writeU16(w, ttf->glyphs[t].advance);
	writeU16(w, ttf->glyphs[t].bearing);
    }
    for(;t<ttf->num_glyphs;t++) {
	writeU16(w, ttf->glyphs[t].bearing);
    }
    return num_advances;
}
static U32*loca_parse(memreader_t*r, ttf_t*ttf, int size)
{
    int t;
    int num = ttf->num_glyphs+1;
    U32*locations = rfx_calloc(num*sizeof(U32));
    if(size) {
	if(num*4 > r->size) {
	    msg("<warning> Short 'loca' table (32 bit)");
	    num=r->size/4;
	}
	if(num*4 < r->size) {
	    msg("<warning> Extraneous data (%d bytes) in 'loca' table (32 bit)", r->size-num*4);
	}
	for(t=0;t<num;t++) {
	    locations[t] = readU32(r);
	}
    } else {
	if(num*2 > r->size) {
	    msg("<warning> Short 'loca' table (16 bit)");
	    num=r->size/2;
	}
	if(num*2 < r->size) {
	    msg("<warning> Extraneous data (%d bytes) in 'loca' table (16 bit)", r->size-num*2);
	}
	for(t=0;t<num;t++) {
	    locations[t] = readU16(r)*2;
	}
    }
    return locations;
}
static int loca_write(ttf_t*ttf, ttf_table_t*w, U32*locations)
{
    int t;
    char use_32bit = 0;
    for(t=0;t<=ttf->num_glyphs;t++) {
	if(locations[t]>=0x20000 || (locations[t]&1)) {
	    use_32bit = 1;
	    break;
	}
    }

    if(use_32bit) {
	for(t=0;t<=ttf->num_glyphs;t++) {
	    writeU32(w, locations[t]);
	}
	return 1;
    } else {
	for(t=0;t<=ttf->num_glyphs;t++) {
	    writeU16(w, locations[t]/2);
	}
	return 0;
    }
}
static int parse_simple_glyph(ttf_t*ttf, memreader_t*r, int num_contours, int glyphnr)
{
    ttfglyph_t*glyph = &ttf->glyphs[glyphnr];

    U16*endpoints = 0;
    if(num_contours>0) {
	endpoints = malloc(sizeof(U16)*num_contours);
	int s;
	int lastpos = -1;
	for(s=0;s<num_contours;s++) {
	    int pos = endpoints[s] = readU16(r);
	    if(pos<=lastpos) {
		msg("<warning> Unsorted endpoints array (len:%d) last=%d now=%d", s, pos, lastpos);
	    }
	    lastpos = pos;
	}
    }
    U16 code_len = readU16(r);
    if(code_len) {
	glyph->code = malloc(sizeof(U16)*code_len);
	readBlock(r, glyph->code, code_len);
	glyph->code_size = code_len;
    }

    if(!endpoints) 
	return 1;

    /*msg("<notice> TTF Glyph %d) code_size=%d num_contours=%d glyph->num_points=%d %d/%d/%d/%d", 
	    glyphnr, code_len, num_contours, glyph->num_points,
	    xmin, ymin, xmax, ymax);*/
    INIT_READ(fx, r->mem, r->size, r->pos);
    INIT_READ(fy, r->mem, r->size, r->pos);
    
    glyph->num_points = endpoints[num_contours-1] + 1;
    glyph->points = rfx_calloc(sizeof(ttfpoint_t)*glyph->num_points);

    /* parse flag array (1st pass- to determine start of coordinates) */
    int num=0;
    while(num<glyph->num_points) {
	U8 flag = readU8(r);
	if(flag&0xc0) {
	    msg("<error> Bad flags in glyph outline: %02x (at pos %d)", flag, num);
	    free(glyph->points);
	    glyph->points = 0;
	    glyph->num_points = 0;
	    return 0;
	}
	int count = 1;
	if(flag & 0x08) 
	    count += readU8(r);
	if(count+num>glyph->num_points) {
	    msg("<warning> Bad count (%d) in glyph (%d) (at pos %d)", count, glyphnr, num);
	    count = glyph->num_points-num;
	}
	num+=count;
    }

    /* parse flag array (2nd pass) and x coordinates */
    num=0;
    int x = 0;
    char is_start=1;
    int contour_pos=0;
    int bytepos = r->pos;
    while(num<glyph->num_points) {
	U8 flag = readU8(&fx);
	int count = flag&8?readU8(&fx)+1:1;
	count=count>glyph->num_points-num?glyph->num_points-num:(count?count:1);
	do {
	    char is_end=0;
	    if(contour_pos<num_contours && num==endpoints[contour_pos]) {
		contour_pos++;
		is_end=1;
	    }
	    int oldx = x;
	    if((flag&0x12) == 0x12) x += readU8(r);
	    else if((flag&0x12) == 0x02) x -= readU8(r);
	    else if((flag&0x12) == 0x00) x += readS16(r);
	    
	    glyph->points[num].x = x;
	    U8 f = flag&GLYPH_ON_CURVE;
	    if(is_start) f|=GLYPH_CONTOUR_START;
	    if(is_end) f|=GLYPH_CONTOUR_END;
	    glyph->points[num].flags = f;
	    num++;
	    is_start = is_end;
	} while(--count);
    }
    
    /* parse flag array (3rd pass) and y coordinates */
    num=0;
    int y = 0;
    while(num<glyph->num_points) {
	U8 flag = readU8(&fy);
	int count = flag&8?readU8(&fy)+1:1;
	count=count>glyph->num_points-num?glyph->num_points-num:(count?count:1);
	do {
	    if((flag&0x24) == 0x24) y += readU8(r);
	    else if((flag&0x24) == 0x04) y -= readU8(r);
	    else if((flag&0x24) == 0x00) y += readS16(r);
	    glyph->points[num].y = y;
	    num++;
	} while(--count);
    }
    free(endpoints);
    return 1;
}
static void glyf_parse(memreader_t*rr, ttf_t*ttf, U32*loca)
{
    int t;
    char warn_about_compound_glyphs=1;
    for(t=0;t<ttf->num_glyphs;t++) {
	INIT_READ(r, rr->mem, rr->size, loca[t]);
	if(r.pos+10>r.size) {
	    msg("<warning> Unexpected end of glyph array (or bad loca entry %d/%d)", loca[t], r.size);
	    break;
	}
	S16 num_contours = readS16(&r);
	ttf->glyphs[t].xmin = readS16(&r);
	ttf->glyphs[t].ymin = readS16(&r);
	ttf->glyphs[t].xmax = readS16(&r);
	ttf->glyphs[t].ymax = readS16(&r);
	
	if(num_contours<0) {
	    if(warn_about_compound_glyphs)
		msg("<error> Compound glyphs not supported yet");
	    warn_about_compound_glyphs=0;
	} else {
	    if(!parse_simple_glyph(ttf, &r, num_contours, t))
		return;
	}
    }

}

void write_simple_glyph(ttf_table_t*w, ttfglyph_t*g)
{
    /* endpoints array */
    int s;
    for(s=0;s<g->num_points;s++) {
	if(g->points[s].flags&GLYPH_CONTOUR_END)
	    writeU16(w, s);
    }
    
    /* bytecode */
    writeU16(w, g->code_size);
    if(g->code_size)
	writeBlock(w, g->code, g->code_size);

    /* flags */
    int lastx=0;
    int lasty=0;
    int lastflag=-1;
    int flagcount=0;
    for(s=0;s<g->num_points;s++) {
	ttfpoint_t*p = &g->points[s];
	int dx = p->x - lastx;
	int dy = p->y - lasty;
	U8 flags = p->flags&GLYPH_ON_CURVE;
	if(!dx) {
	    flags|=0x10;
	} else if(dx<0 && dx>=-255) {
	    flags|=0x02;
	} else if(dx>0 && dx<=255) {
	    flags|=0x12;
	}
	if(!dy) {
	    flags|=0x20;
	} else if(dy<0 && dy>=-255) {
	    flags|=0x04;
	} else if(dy>0 && dy<=255) {
	    flags|=0x24;
	}
	if(flags == lastflag && flagcount<255) {
	    flagcount++;
	} else {
	    if(lastflag>=0) {
		if(flagcount) {
		    writeU8(w, lastflag|8);
		    writeU8(w, flagcount);
		} else {
		    writeU8(w, lastflag);
		}
	    }
	    lastflag = flags;
	    flagcount = 0;
	}
	lastx = p->x;
	lasty = p->y;
    }
    if(lastflag>=0) {
	if(flagcount) {
	    writeU8(w, lastflag|8);
	    writeU8(w, flagcount);
	} else {
	    writeU8(w, lastflag);
	}
    }
    /* coordinates */
    lastx=0;
    int bytepos = w->len;
    for(s=0;s<g->num_points;s++) {
	ttfpoint_t*p = &g->points[s];
	int dx = p->x - lastx;
	if(dx>32767 || dx<-32768) {
	    msg("<error> Coordinate overflow in glyph");
	}
	lastx = p->x;
	if(dx>0 && dx<=255) writeU8(w, dx);
	else if(dx<0 && dx>=-255) writeU8(w, -dx);
	else if(dx) writeS16(w, dx);
    }

    lasty=0;
    for(s=0;s<g->num_points;s++) {
	ttfpoint_t*p = &g->points[s];
	int dy = p->y - lasty;
	if(dy>32767 || dy<-32768) {
	    msg("<error> Coordinate overflow in glyph");
	}
	lasty = p->y;
	if(dy>0 && dy<=255) writeU8(w, dy);
	else if(dy<0 && dy>=-255) writeU8(w, -dy);
	else if(dy) writeS16(w, dy);
    }
}
    
U32* glyf_write(ttf_t* ttf, ttf_table_t*w)
{
    U32*locations = malloc(sizeof(U32)*(ttf->num_glyphs+1));
    int t;
    for(t=0;t<ttf->num_glyphs;t++) {
	locations[t] = w->len;
	ttfglyph_t*g = &ttf->glyphs[t];
	int s;
	int num_contours = 0;
	for(s=0;s<g->num_points;s++) {
	    if(g->points[s].flags&GLYPH_CONTOUR_END)
		num_contours++;
	}
	writeS16(w, num_contours?num_contours:1);
	writeS16(w, g->xmin);
	writeS16(w, g->ymin);
	writeS16(w, g->xmax);
	writeS16(w, g->ymax);
	
	if(!num_contours) {
	    /* some ttf parsers can't deal with zero contours, so in the case
	       of an empty glyph, write a single point (0,0) */
	    writeU16(w, 0); //endpoint of 1st contour
	    writeU16(w, g->code_size);
	    if(g->code_size)
		writeBlock(w, g->code, g->code_size);
	    writeU8(w, 0x31); //flag (xy=(0,0),on curve)
	} else {
	    write_simple_glyph(w, g);
	}
    }
    locations[t] = w->len;
    return locations;
}
void glyf_dump(ttf_t* ttf)
{
    int t;
    for(t=0;t<ttf->num_glyphs;t++) {
	ttfglyph_t*g = &ttf->glyphs[t];
	printf("glyph %d)\n", t);
	printf("  advance=%d\n", g->advance);
	printf("  bearing=%d\n", g->bearing);
	printf("  bbox=(%d/%d/%d/%d)\n", g->xmin, g->ymin, g->xmax, g->ymax);
	printf("  points=(");
	int s;
	for(s=0;s<g->num_points;s++) {
	    if(s) printf(",");
	    printf("%d/%d/0x%02x", g->points[s].x, g->points[s].y, g->points[s].flags);
	}
	printf(")\n");
	if(g->code_size) 
	    hexdump(g->code, g->code_size, "  ");
    }
}

void glyf_delete(ttf_t* ttf)
{
    if(!ttf->glyphs) 
	return;
    int t;
    for(t=0;t<ttf->num_glyphs;t++) {
	if(ttf->glyphs[t].code) {
	    free(ttf->glyphs[t].code);
	    ttf->glyphs[t].code = 0;
	}
	if(ttf->glyphs[t].points) {
	    free(ttf->glyphs[t].points);
	    ttf->glyphs[t].points = 0;
	}
    }
    free(ttf->glyphs);ttf->glyphs=0;
}

static int ttf_parse_tables(ttf_t*ttf)
{
    ttf_table_t*table;

    table = ttf_find_table(ttf, TAG_HEAD);
    if(!table) {
	msg("<error> Font has no head table");
	return 0;
    }
    INIT_READ(m, table->data, table->len, 0);
    int loc_index = head_parse(ttf, &m);
    ttf_table_delete(ttf, table);
    
    table = ttf_find_table(ttf, TAG_MAXP);
    if(!table) {
	msg("<error> Font has no maxp table");
	return 0;
    }
    INIT_READ(m2, table->data, table->len, 0);
    ttf->maxp = maxp_parse(ttf, &m2);
    ttf_table_delete(ttf, table);
    
    if(!ttf->num_glyphs) {
	msg("<error> Invalid number of characters");
	return 0;
    }
    ttf->glyphs = rfx_calloc(sizeof(ttfglyph_t)*ttf->num_glyphs);

    table = ttf_find_table(ttf, TAG_OS2);
    if(table) {
	INIT_READ(m, table->data, table->len, 0);
	ttf->os2 = os2_parse(&m);
        ttf_table_delete(ttf, table);
    }

    table = ttf_find_table(ttf, TAG_HHEA);
    if(table) {
	INIT_READ(m, table->data, table->len, 0);
	int num_advances = hea_parse(&m, ttf);
        ttf_table_delete(ttf, table);
	
	table = ttf_find_table(ttf, TAG_HMTX);
	if(table) {
	    INIT_READ(m, table->data, table->len, 0);
	    hmtx_parse(&m, ttf, num_advances);
            ttf_table_delete(ttf, table);
	}
    } else {
	table = ttf_find_table(ttf, TAG_VHEA);
	if(table) {
	    ttf->is_vertical=1;
	    INIT_READ(m, table->data, table->len, 0);
	    int num_advances = hea_parse(&m, ttf);
            ttf_table_delete(ttf, table);

	    table = ttf_find_table(ttf, TAG_VMTX);
	    if(table) {
		INIT_READ(m, table->data, table->len, 0);
		hmtx_parse(&m, ttf, num_advances);
		ttf_table_delete(ttf, table);
	    }
	} else {
	    msg("<error> Font contains neither HHEA nor VHEA");
	}
    }
    table = ttf_find_table(ttf, TAG_LOCA);
    if(table) {
	INIT_READ(m, table->data, table->len, 0);
	U32*loca = loca_parse(&m, ttf, loc_index);
	ttf_table_delete(ttf, table);
	table = ttf_find_table(ttf, TAG_GLYF);
	if(table) {
	    INIT_READ(m, table->data, table->len, 0);
	    glyf_parse(&m, ttf, loca);
	    ttf_table_delete(ttf, table);
	}
	free(loca);
    }
    return 1;
}
static void ttf_collapse_tables(ttf_t*ttf)
{
    ttf_table_t*table;
    
    table = ttf_addtable(ttf, TAG_MAXP);
    maxp_write(ttf, table);
    maxp_delete(ttf);

    table = ttf_addtable(ttf, TAG_OS2);
    os2_write(ttf, table);
    os2_delete(ttf);

    if(!ttf->is_vertical) {
	table = ttf_addtable(ttf, TAG_HMTX);
	int num_advances = mtx_write(ttf, table);
	table = ttf_addtable(ttf, TAG_HHEA);
	hea_write(ttf, table, num_advances);
    } else {
	table = ttf_addtable(ttf, TAG_VMTX);
	int num_advances = mtx_write(ttf, table);
	table = ttf_addtable(ttf, TAG_VHEA);
	hea_write(ttf, table, num_advances);
    }

    int loca_size=0;
    if(ttf->num_glyphs) {
	table = ttf_addtable(ttf, TAG_GLYF);
	U32*locations = glyf_write(ttf, table);
	glyf_delete(ttf);
	
	table = ttf_addtable(ttf, TAG_LOCA);
	loca_size = loca_write(ttf, table, locations);
	free(locations);
    }
    
    table = ttf_addtable(ttf, TAG_HEAD);
    head_write(ttf, table, loca_size);
    head_delete(ttf);

}

ttf_t* load_ttf(void*data, int length)
{
    INIT_READ(r,data,length, 0);

    if(length<12) {
	msg("<error> Truncated Truetype file (%d bytes)", length);
	return 0;
    }

    ttf_t*ttf = rfx_calloc(sizeof(ttf_t));
    ttf->version = readU32(&r);
    if(ttf->version == TTCFTAG) {
	/* a ttc collection is a number of truetype fonts
	   packaged together */
	if(length<16) {
	    msg("<error> Truncated TTC file (%d bytes)", length);
	    return 0;
	}
	U32 ttcf_version = readU32(&r); // 0x00000100: v1.0, 0x00000200: v2.0, includes DSIG table
	U32 num_fonts = readU32(&r); // number of fonts
	U32 font1_position = readU32(&r);
	if(font1_position+12 > length) {\
	    msg("<error> Truncated TTC file (%d bytes, first font at %d)", length, font1_position);
	    return 0;
	}
	r.pos = font1_position;
	ttf->version = readU32(&r);
    }
    
    int num_tables = readU16(&r);
    
    readU16(&r); //search range
    readU16(&r); //entry selector
    readU16(&r); //range shift

    if(num_tables*16 > length) {
	msg("<error> Truncated TTC file (table entries: %d)", num_tables);
	if(ttf->version != OPENTYPE && 
	   ttf->version != TRUETYPE_MACOS && 
	   ttf->version != VERSION_1_0) {
	    // bad table length, weird version. This is probably not a ttf file.
	    return 0;
	}
    }

    U32*table_data = malloc(16*num_tables);
    int t;
    for(t=0;t<num_tables*4;t++) {
	table_data[t] = readU32(&r);
    }
    for(t=0;t<num_tables;t++) {
	U32 tag = table_data[t*4];
	U32 checksum = table_data[t*4+1];
	U32 pos = table_data[t*4+2];
	U32 len = table_data[t*4+3];

	if(pos+len > length) {
	    msg("<error> TTF Table %02x%02x%02x%02x outside of stream (pos %d)", (tag>>24)&0xff, (tag>>16)&0xff, (tag>>8)&0xff, (tag)&0xff, pos);
	} else {
	    U8*mem = malloc(len);
	    r.pos = pos;
	    readBlock(&r, mem, len);
	    
	    ttf_table_t*table = ttf_addtable(ttf, tag);
	    table->data = mem;
	    table->len = table->memsize = len;
	    
	    U32 checksum2 = ttf_table_checksum(table);
	    if(checksum2!=checksum) {
		msg("<warning> Checksum mismatch in tag %02x%02x%02x%02x %c%c%c%c (%d bytes) %08x!=%08x", 
			(tag>>24)&0xff, (tag>>16)&0xff, (tag>>8)&0xff, (tag)&0xff,
			(tag>>24)&0xff, (tag>>16)&0xff, (tag>>8)&0xff, (tag)&0xff,
			len, checksum2, checksum);
	    }
	    
	}
    }
    free(table_data);

    if(!ttf_parse_tables(ttf)) 
	return 0;

    return ttf;
}

ttf_table_t* ttf_write(ttf_t*ttf)
{
    ttf_collapse_tables(ttf);
   
    ttf_table_t*file = ttf_table_new(0);
    writeU32(file, VERSION_1_0);

    /* write number of tables */
    int num_tables=0;
    ttf_table_t*t = ttf->tables;
    while(t) {
	num_tables++;
	t = t->next;
    }
    writeU16(file, num_tables);
    
    /* write search range */
    int tmp = num_tables;
    int search_range = 0;
    while(tmp) {
	search_range = tmp;
	tmp = tmp&(tmp-1);
    }
    tmp = search_range;
    search_range*=16;
    writeU16(file, search_range);

    /* write entry selector */
    int entry_selector = 0;
    while(tmp>1) {
	tmp>>=1;
	entry_selector++;
    }
    writeU16(file, entry_selector);

    /* write range shift */
    int range_shift = num_tables*16 - search_range;
    writeU16(file, range_shift);

    /* write table dictionary */
    int table_dictionary_pos = file->len;
    int data_pos = file->len + num_tables*16;
    for(t=ttf->tables;t;t=t->next) {
	writeU32(file, t->id);
	writeU32(file, ttf_table_checksum(t));
	writeU32(file, data_pos);
	writeU32(file, t->len);
	data_pos += t->len;
	data_pos += (-t->len)&3; //pad
    }

    /* write tables */
    int head_pos = 0;
    U8 zero[4]={0,0,0,0};
    for(t=ttf->tables;t;t=t->next) {
	if(t->id == TAG_HEAD)
	    head_pos = file->len;
	writeBlock(file, t->data, t->len);
	writeBlock(file, zero, (-t->len)&3); //pad
    }
    U32 checksum = 0xb1b0afba - ttf_table_checksum(file);
    U8*checksum2 = file->data + head_pos + 8;
    checksum2[0] = checksum>>24;
    checksum2[1] = checksum>>16;
    checksum2[2] = checksum>>8;
    checksum2[3] = checksum>>0;
    return file;
}

void ttf_save(ttf_t*ttf, const char*filename)
{
    ttf_table_t* t = ttf_write(ttf);
    FILE*fi = fopen(filename, "wb");
    if(!fi) {
	perror(filename);
	return;
    }
    fwrite(t->data, t->len, 1, fi);
    fclose(fi);
    ttf_table_delete(0, t);
}

void ttf_dump(ttf_t*ttf)
{
    msg("<notice> Truetype file version %08x%s", ttf->version, ttf->version == OPENTYPE?" (opentype)":"");
    ttf_table_t*table = ttf->tables;
    while(table) {
	U32 tag = table->id;
	msg("<notice> Tag %02x%02x%02x%02x [%c%c%c%c] (length: %d)", 
		(tag>>24)&0xff, (tag>>16)&0xff, (tag>>8)&0xff, (tag)&0xff, 
		(tag>>24)&0xff, (tag>>16)&0xff, (tag>>8)&0xff, (tag)&0xff, table->len);
	table = table->next;
    }
    //ttf_table_dump(ttf_find_table(ttf, TAG_MAXP));

    head_dump(ttf);
    hea_dump(ttf);
    os2_dump(ttf);
    maxp_dump(ttf);
    glyf_dump(ttf);
}
void ttf_destroy(ttf_t*ttf)
{
    ttf_table_t*table = ttf->tables;
    while(table) {
	ttf_table_t*next = table->next;
	free(table->data);
	free(table);
	table = next;
    }
    maxp_delete(ttf);
    os2_delete(ttf);
    head_delete(ttf);
    hea_delete(ttf);
    glyf_delete(ttf);
    free(ttf);
}

#ifdef MAIN
int main(int argn, const char*argv[])
{
    setConsoleLogging(7);
    const char*filename = "comic.ttf";
    if(argn>1) 
	filename = argv[1];
    //msg("<notice> Loading %s", filename);
    memfile_t*m = memfile_open(filename);
    ttf_t*ttf = load_ttf(m->data, m->len);
    if(!ttf) return 1;
    memfile_close(m);
    ttf_dump(ttf);
    //printf("os2 version: %04x (%d), maxp size: %d\n", 
//	    ttf->os2->version, ttf->os2->size, ttf->maxp->size);
    ttf_save(ttf, "comic2.ttf");
    ttf_destroy(ttf);
    return 0;

}
#endif
