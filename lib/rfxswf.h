/* rfxswf.h

   Headers for rfxswf.c and modules

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
 

#ifndef __RFX_SWF_INCLUDED__
#define __RFX_SWF_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include "../config.h"
#include "./bitio.h"
#include "./drawer.h"

#define DEBUG_RFXSWF
#ifdef RFXSWF_DISABLESOUND
#define NO_MP3
#endif

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif


/* little/big endian stuff */

#define PUT8(ptr,x) {((U8*)(ptr))[0]=x;}
#define PUT16(ptr,x) {((U8*)(ptr))[0]=(U8)(x);((U8*)(ptr))[1]=(U8)((x)>>8);}
#define PUT32(ptr,x) {((U8*)(ptr))[0]=(U8)(x);((U8*)(ptr))[1]=(U8)((x)>>8);((U8*)(ptr))[2]=(U8)((x)>>16);((U8*)(ptr))[3]=(U8)((x)>>24);}
#define GET16(ptr) (((U16)(((U8*)(ptr))[0]))+(((U16)(((U8*)(ptr))[1]))<<8))
#define GET32(ptr) (((U16)(((U8*)(ptr))[0]))+(((U16)(((U8*)(ptr))[1]))<<8)+(((U16)(((U8*)(ptr))[2]))<<16)+(((U16)(((U8*)(ptr))[3]))<<24))

#ifdef WORDS_BIGENDIAN
#define SWAP16(s) ((((s)>>8)&0x00ff)|(((s)<<8)&0xff00))
#define SWAP32(s) (SWAP16(((s)>>16)&0x0000ffff)|((SWAP16(s)<<16)&0xffff0000))
#define REVERSESWAP16(x) (x)
#define REVERSESWAP32(x) (x)
#else
#define SWAP16(x) (x)
#define SWAP32(x) (x)
#define REVERSESWAP16(s) ((((s)>>8)&0x00ff)|(((s)<<8)&0xff00))
#define REVERSESWAP32(s) (REVERSESWAP16(((s)>>16)&0x0000ffff)|((REVERSESWAP16(s)<<16)&0xffff0000))
#endif
// SWF Types

typedef         unsigned long   U32;
typedef         signed long     S32;
typedef         unsigned short  U16;
typedef         signed short    S16;
typedef         unsigned char   U8;
typedef         signed char     S8;
typedef         signed long     SFIXED;
typedef         signed long     SCOORD;

#define SCOORD_MAX 0x7fffffff
#define SCOORD_MIN -0x80000000

// Basic Structures

typedef struct _SPOINT
{ SCOORD        x;
  SCOORD        y;
} SPOINT, * LPSPOINT;

typedef struct _RGBA
{ U8    a;
  U8    r;
  U8    g;
  U8    b;
} RGBA, * LPRGBA;

typedef struct _YUV
{
  U8	y,u,v;
} YUV;

typedef struct _SRECT
{ SCOORD        xmin;
  SCOORD        ymin;
  SCOORD        xmax;
  SCOORD        ymax;
} SRECT, * LPSRECT;

typedef struct _MATRIX
{ SFIXED        sx;     // factor x
  SFIXED        sy;
  SFIXED        r0;     // rotation
  SFIXED        r1;
  SCOORD        tx;     // delta x
  SCOORD        ty;
} MATRIX, * LPMATRIX;

typedef struct _CXFORM
{ S16           a0, a1; /* mult, add */
  S16           r0, r1;
  S16           g0, g1;
  S16           b0, b1;
} CXFORM, * LPCXFORM;

#define GRADIENT_LINEAR 0x10
#define GRADIENT_RADIAL 0x12
typedef struct _GRADIENT
{
    int num;
    U8 ratios[8];
    RGBA rgba[8];
} GRADIENT;

typedef struct _TAG             // NEVER access a Tag-Struct directly !
{ U16           id;
  U8 *          data;
  U32           memsize;        // to minimize realloc() calls

  U32         len;            // for Set-Access
  U32         pos;            // for Get-Access

  struct _TAG * next;
  struct _TAG * prev;

  U8            readBit;        // for Bit-Manipulating Functions [read]
  U8            writeBit;       // [write]
  
} TAG, * LPTAG;

#define swf_ResetReadBits(tag)   if (tag->readBit)  { tag->pos++; tag->readBit = 0; }
#define swf_ResetWriteBits(tag)  if (tag->writeBit) { tag->writeBit = 0; }

typedef struct _SOUNDINFO 
{
    U8 stop;
    U8 nomultiple; //continue playing if already started

    U32 inpoint;
    U32 outpoint;

    U16 loops;
    U8 envelopes;

    //envelope:
    U32* pos;
    U32* left;
    U32* right;
} SOUNDINFO;

typedef struct _SWF
{ U8            fileVersion;
  U8		compressed;     // SWF or SWC?
  U32           fileSize;       // valid after load and save
  SRECT         movieSize;
  U16           frameRate;
  U16           frameCount;     // valid after load and save
  TAG *         firstTag;
} SWF, * LPSWF;

// Basic Functions

int  swf_ReadSWF2(struct reader_t*reader, SWF * swf);   // Reads SWF via callback
int  swf_ReadSWF(int handle,SWF * swf);     // Reads SWF to memory (malloc'ed), returns length or <0 if fails
int  swf_WriteSWF2(struct writer_t*writer, SWF * swf);     // Writes SWF via callback, returns length or <0 if fails
int  swf_WriteSWF(int handle,SWF * swf);    // Writes SWF to file, returns length or <0 if fails
int  swf_WriteSWC(int handle, SWF * swf);   // for convenience, equal to swf->compressed=1;swf_WriteSWF(..)
int  swf_WriteCGI(SWF * swf);               // Outputs SWF with valid CGI header to stdout
void swf_FreeTags(SWF * swf);               // Frees all malloc'ed memory for swf

// for streaming:
int  swf_WriteHeader(int handle,SWF * swf);    // Writes Header of swf to file
int  swf_WriteHeader2(struct writer_t*writer,SWF * swf);    // Writes Header of swf to file
int  swf_WriteTag(int handle,TAG * tag);    // Writes TAG to file
int  swf_WriteTag2(struct writer_t*writer, TAG * t); //Write TAG via callback

int  swf_ReadHeader(struct reader_t*reader, SWF * swf);   // Reads SWF Header via callback

// folding/unfolding:

void swf_FoldAll(SWF*swf);
void swf_UnFoldAll(SWF*swf);
void swf_FoldSprite(TAG*tag);
void swf_UnFoldSprite(TAG*tag);
int swf_IsFolded(TAG*tag);

// tag reordering:

void swf_OptimizeTagOrder(SWF*swf);

// basic routines:
    
TAG * swf_InsertTag(TAG * after,U16 id);    // updates frames, if necessary
TAG * swf_InsertTagBefore(SWF*swf, TAG * before,U16 id);     // like InsertTag, but insert tag before argument
int   swf_DeleteTag(TAG * t);

void  swf_ClearTag(TAG * t);                //frees tag data
void  swf_ResetTag(TAG*tag, U16 id);        //set's tag position and length to 0, without freeing it
    
void  swf_SetTagPos(TAG * t,U32 pos);       // resets Bitcount
U32   swf_GetTagPos(TAG * t);

TAG * swf_NextTag(TAG * t);
TAG * swf_PrevTag(TAG * t);

U16   swf_GetTagID(TAG * t);                // ... TagGetID
U32   swf_GetTagLen(TAG * t);             // ... TagGetTagLen
U8*   swf_GetTagLenPtr(TAG * t);

U32   swf_GetBits(TAG * t,int nbits);
S32   swf_GetSBits(TAG * t,int nbits);
int   swf_SetBits(TAG * t,U32 v,int nbits);

int   swf_GetBlock(TAG * t,U8 * b,int l);   // resets Bitcount
int   swf_SetBlock(TAG * t,U8 * b,int l);

U8    swf_GetU8(TAG * t);                   // resets Bitcount
U16   swf_GetU16(TAG * t);
U32   swf_GetU32(TAG * t);
void  swf_GetRGB(TAG * t, RGBA * col);
void  swf_GetRGBA(TAG * t, RGBA * col);
void  swf_GetGradient(TAG * t, GRADIENT * gradient, char alpha);
char* swf_GetString(TAG*t);
int   swf_SetU8(TAG * t,U8 v);              // resets Bitcount
int   swf_SetU16(TAG * t,U16 v);
int   swf_SetU32(TAG * t,U32 v);

//int   swf_GetPoint(TAG * t,SPOINT * p);     // resets Bitcount
int   swf_GetRect(TAG * t,SRECT * r);
int   swf_GetMatrix(TAG * t,MATRIX * m);
int   swf_GetCXForm(TAG * t,CXFORM * cx,U8 alpha);

//int   swf_SetPoint(TAG * t,SPOINT * p);     // resets Bitcount
int   swf_SetRect(TAG * t,SRECT * r);
int   swf_SetMatrix(TAG * t,MATRIX * m);
int   swf_SetCXForm(TAG * t,CXFORM * cx,U8 alpha);
int   swf_SetRGB(TAG * t,RGBA * col);
int   swf_SetRGBA(TAG * t,RGBA * col);
void  swf_SetPassword(TAG * t, const char * password);

int   swf_VerifyPassword(TAG * t, const char * password);

// helper functions:

void swf_ExpandRect(SRECT*src, SPOINT add);
void swf_ExpandRect2(SRECT*src, SRECT*add);
SPOINT swf_TurnPoint(SPOINT p, MATRIX* m);
SRECT swf_TurnRect(SRECT r, MATRIX* m);

// Function Macros

#define swf_GetS8(tag)      ((S8)swf_GetU8(tag))
#define swf_GetS16(tag)     ((S16)swf_GetU16(tag))
#define swf_GetS32(tag)     ((S32)swf_GetU32(tag))
#define swf_GetCoord(tag)   ((SCOORD)swf_GetU32(tag))
#define swf_GetFixed(tag)   ((SFIXED)swf_GetU32(tag))

#define swf_SetS8(tag,v)    swf_SetU8(tag,(U8)v)
#define swf_SetS16(tag,v)   swf_SetU16(tag,(U16)v)
#define swf_SetS32(tag,v)   swf_SetU32(tag,(U32)v)
#define swf_SetCoord(tag,v) swf_SetU32(tag,(U32)v)
#define swf_SetFixed(tag,v) swf_SetU32(tag,(U32)v)
#define swf_SetString(t,s)  swf_SetBlock(t,s,strlen(s)+1)

#ifndef FAILED
#define FAILED(b)       ((b)<0)
#endif
#ifndef SUCCEEDED
#define SUCCEEDED(b)     ((b)>=0)
#endif

// Tag IDs (adopted from J. C. Kessels' Form2Flash)

#define ST_END                  0
#define ST_SHOWFRAME            1
#define ST_DEFINESHAPE          2
#define ST_FREECHARACTER        3
#define ST_PLACEOBJECT          4
#define ST_REMOVEOBJECT         5
#define ST_DEFINEBITS           6
#define ST_DEFINEBITSJPEG       6 
#define ST_DEFINEBUTTON         7
#define ST_JPEGTABLES           8
#define ST_SETBACKGROUNDCOLOR   9
#define ST_DEFINEFONT           10
#define ST_DEFINETEXT           11
#define ST_DOACTION             12
#define ST_DEFINEFONTINFO       13
#define ST_DEFINESOUND          14 /* Event sound tags. */
#define ST_STARTSOUND           15
#define ST_DEFINEBUTTONSOUND    17
#define ST_SOUNDSTREAMHEAD      18
#define ST_SOUNDSTREAMBLOCK     19
#define ST_DEFINEBITSLOSSLESS   20 /* A bitmap using lossless zlib compression. */
#define ST_DEFINEBITSJPEG2      21 /* A bitmap using an internal JPEG compression table. */
#define ST_DEFINESHAPE2         22
#define ST_DEFINEBUTTONCXFORM   23
#define ST_PROTECT              24 /* This file should not be importable for editing. */
#define ST_PLACEOBJECT2         26 /* The new style place w/ alpha color transform and name. */
#define ST_REMOVEOBJECT2        28 /* A more compact remove object that omits the character tag (just depth). */
#define ST_FREEALL              31 /* ? */
#define ST_DEFINESHAPE3         32 /* A shape V3 includes alpha values. */
#define ST_DEFINETEXT2          33 /* A text V2 includes alpha values. */
#define ST_DEFINEBUTTON2        34 /* A button V2 includes color transform, alpha and multiple actions */
#define ST_DEFINEBITSJPEG3      35 /* A JPEG bitmap with alpha info. */
#define ST_DEFINEBITSLOSSLESS2  36 /* A lossless bitmap with alpha info. */
#define ST_DEFINEEDITTEXT       37
#define ST_DEFINEMOVIE          38
#define ST_DEFINESPRITE         39 /* Define a sequence of tags that describe the behavior of a sprite. */
#define ST_NAMECHARACTER        40 /* Name a character definition, character id and a string, (used for buttons, bitmaps, sprites and sounds). */
#define ST_SERIALNUMBER         41
#define ST_GENERATORTEXT        42 /* contains an id */
#define ST_FRAMELABEL           43 /* A string label for the current frame. */
#define ST_SOUNDSTREAMHEAD2     45 /* For lossless streaming sound, should not have needed this... */
#define ST_DEFINEMORPHSHAPE     46 /* A morph shape definition */
#define ST_DEFINEFONT2          48
#define ST_TEMPLATECOMMAND      49
#define ST_GENERATOR3           51
#define ST_EXTERNALFONT         52
#define ST_EXPORTASSETS		56
#define ST_IMPORTASSETS		57
#define ST_ENABLEDEBUGGER	58
#define ST_DOINITACTION         59
#define ST_DEFINEVIDEOSTREAM    60
#define ST_VIDEOFRAME           61
#define ST_DEFINEFONTINFO2	62
#define ST_MX4			63 /*(?) */
#define ST_SCRIPTLIMITS		65 /* version 7- u16 maxrecursedepth, u16 scripttimeoutseconds */
#define ST_SETTABINDEX		66 /* version 7- u16 depth(!), u16 tab order value */

/* custom tags- only valid for swftools */
#define ST_REFLEX              777 /* to identify generator software */
#define ST_GLYPHNAMES          778

// Advanced Funtions

// swfshape.c

typedef struct _LINESTYLE
{ U16           width;
  RGBA          color;
} LINESTYLE, * LPLINESTYLE;

typedef struct _FILLSTYLE
{ U8	    type;
  RGBA	    color;
  MATRIX    m; 
  U16	    id_bitmap;
  GRADIENT  gradient;
} FILLSTYLE, * LPFILLSTYLE;
     
typedef struct _SHAPE           // NEVER access a Shape-Struct directly !
{                 
  struct
  { LINESTYLE * data;
    U16         n;
  } linestyle;
                  
  struct                    
  { FILLSTYLE * data;
    U16         n;
  } fillstyle;
 
  struct
  { U16         fill;
    U16         line;
  } bits;
  				// used by Get/SetSimpleShape and glyph handling
  U8 *          data;
  U32           bitlen;         // length of data in bits
} SHAPE, * LPSHAPE;

/* SHAPE can be converted into SHAPE2: */

struct _SHAPELINE;
typedef struct _SHAPE2
{
    LINESTYLE * linestyles;
    int numlinestyles;
    FILLSTYLE* fillstyles;
    int numfillstyles;
    struct _SHAPELINE * lines;
    SRECT* bbox; // may be NULL
} SHAPE2;

typedef struct _SHAPELINE
{
    enum {moveTo, lineTo, splineTo} type;
    SCOORD x,y;
    SCOORD sx,sy; //only if type==splineTo
    int fillstyle0;
    int fillstyle1;
    int linestyle;
    struct _SHAPELINE * next;
} SHAPELINE;

// Shapes

int   swf_ShapeNew(SHAPE ** s);
void  swf_ShapeFree(SHAPE * s);

int   swf_GetSimpleShape(TAG * t,SHAPE ** s); // without Linestyle/Fillstyle Record
int   swf_SetSimpleShape(TAG * t,SHAPE * s);   // without Linestyle/Fillstyle Record

int   swf_ShapeAddLineStyle(SHAPE * s,U16 width,RGBA * color);
int   swf_ShapeAddSolidFillStyle(SHAPE * s,RGBA * color);
int   swf_ShapeAddBitmapFillStyle(SHAPE * s,MATRIX * m,U16 id_bitmap,int clip);
int   swf_ShapeAddGradientFillStyle(SHAPE * s,MATRIX * m,GRADIENT* gradient,int radial);

int   swf_SetShapeStyles(TAG * t,SHAPE * s);
int   swf_ShapeCountBits(SHAPE * s,U8 * fbits,U8 * lbits);
int   swf_SetShapeBits(TAG * t,SHAPE * s);
int   swf_SetShapeHeader(TAG * t,SHAPE * s); // one call for upper three functions

int   swf_ShapeSetMove(TAG * t,SHAPE * s,S32 x,S32 y);
int   swf_ShapeSetStyle(TAG * t,SHAPE * s,int line,int fill0,int fill1);
int   swf_ShapeSetAll(TAG * t,SHAPE * s,S32 x,S32 y,int line,int fill0,int fill1);

int   swf_ShapeSetLine(TAG * t,SHAPE * s,S32 x,S32 y);
int   swf_ShapeSetCurve(TAG * t,SHAPE * s,S32 x,S32 y,S32 ax,S32 ay);
int   swf_ShapeSetCircle(TAG * t,SHAPE * s,S32 x,S32 y,S32 rx,S32 ry);
int   swf_ShapeSetEnd(TAG * t);

void  swf_ShapeSetBitmapRect(TAG * t, U16 gfxid, int width, int height);

SHAPELINE* swf_ParseShapeData(U8*data, int bits, int fillbits, int linebits);
SHAPE2*	   swf_ShapeToShape2(SHAPE*shape);
void	   swf_Shape2ToShape(SHAPE2*shape2, SHAPE*shape);
SRECT	   swf_GetShapeBoundingBox(SHAPE2*shape);
void	    swf_SetShape2(TAG*tag, SHAPE2*shape);
void	   swf_Shape2Free(SHAPE2 * s);
void	swf_DumpShape(SHAPE2*shape2);

void swf_ParseDefineShape(TAG*tag, SHAPE2*shape);
void swf_Shape2ToShape(SHAPE2*shape2, SHAPE*shape);
void swf_SetShape2(TAG*tag, SHAPE2*shape2);

// swfdraw.c

void swf_Shape10DrawerInit(drawer_t*draw, TAG*tag);
void swf_Shape01DrawerInit(drawer_t*draw, TAG*tag);
void swf_Shape11DrawerInit(drawer_t*draw, TAG*tag);
SHAPE* swf_ShapeDrawerToShape(drawer_t*draw);
SRECT swf_ShapeDrawerGetBBox(drawer_t*draw);

void swf_DrawString(drawer_t*draw, const char*source);

// swftext.c

typedef struct _KERNING
{
  U16	      char1;
  U16	      char2;
  U16	      adjustment;
} SWFKERNING;

typedef struct _SWFLAYOUT
{ S16          ascent;
  S16          descent;
  S16          leading;
  SRECT      * bounds;
  U16	       kerningcount;
  SWFKERNING * kerning;
} SWFLAYOUT, * LPSWFLAYOUT;

typedef struct
{ S16         advance;
  SHAPE *     shape;
} SWFGLYPH;

#define FONT_STYLE_BOLD 1
#define FONT_STYLE_ITALIC 2
#define FONT_ENCODING_UNICODE 1
#define FONT_ENCODING_ANSI 2 
#define FONT_ENCODING_SHIFTJIS 4

typedef struct _SWFFONT
{ int		id; // -1 = not set
  U8		version; // 0 = not set, 1 = definefont, 2 = definefont2
  U8 *          name;
  SWFLAYOUT *   layout;
  U16		numchars;
  U16		maxascii; // highest mapped ascii value
  
  U8		style;
  U8		encoding;

  U16	*	glyph2ascii;
  int	*	ascii2glyph;
  SWFGLYPH *	glyph;
  U8		language;
  char **	glyphnames;
} SWFFONT, * LPSWFFONT;

// does not support wide characters !
typedef struct _FONTUSAGE
{ U8* code;
} FONTUSAGE, * LPFONTUSAGE;

#define ET_HASTEXT 32768
#define ET_WORDWRAP 16384
#define ET_MULTILINE 8192
#define ET_PASSWORD 4096
#define ET_READONLY 2048
#define ET_HASTEXTCOLOR 1024
#define ET_HASMAXLENGTH 512
#define ET_HASFONT 256
#define ET_X3 128
#define ET_AUTOSIZE 64 /* MX */
#define ET_HASLAYOUT 32
#define ET_NOSELECT 16
#define ET_BORDER 8
#define ET_X1 4
#define ET_HTML 2 /* MX? */
#define ET_USEOUTLINES 1

typedef struct _EditTextLayout
{
    U8 align; // 0=left, 1=right, 2=center, 3=justify
    U16 leftmargin;
    U16 rightmargin;
    U16 indent;
    U16 leading;
} EditTextLayout;

int swf_FontEnumerate(SWF * swf,void (*FontCallback) (U16,U8*));
// -> void fontcallback(U16 id,U8 * name); returns number of defined fonts

int swf_FontExtract(SWF * swf,int id,SWFFONT ** f);
// Fetches all available information from DefineFont, DefineFontInfo, DefineText, ...
// id = FontID, id=0 -> Extract first Font

int swf_FontIsItalic(SWFFONT * f);
int swf_FontIsBold(SWFFONT * f);

int swf_FontSetID(SWFFONT * f,U16 id);
int swf_FontReduce(SWFFONT * f,FONTUSAGE * use);

int swf_FontInitUsage(SWFFONT * f,FONTUSAGE * use);
int swf_FontUse(SWFFONT* f,FONTUSAGE * use,U8 * s);

int swf_FontSetDefine(TAG * t,SWFFONT * f);
int swf_FontSetDefine2(TAG * t,SWFFONT * f);
int swf_FontSetInfo(TAG * t,SWFFONT * f);

void swf_FontCreateLayout(SWFFONT*f);
void swf_FontAddLayout(SWFFONT * f, int ascent, int descent, int leading);

int swf_ParseDefineText(TAG * t, void(*callback)(void*self, int*chars, int*xpos, int nr, int fontid, int fontsize, int xstart, int ystart, RGBA* color), void*self);

void swf_WriteFont(SWFFONT* font, char* filename);
SWFFONT* swf_ReadFont(char* filename);

void swf_FontFree(SWFFONT * f);

U32 swf_TextGetWidth(SWFFONT * font,U8 * s,int scale);
int swf_TextCountBits(SWFFONT * font,U8 * s,int scale,U8 * gbits,U8 * abits);

#define SET_TO_ZERO 0x80000000
int swf_TextSetInfoRecord(TAG * t,SWFFONT * font,U16 size,RGBA * color,int dx,int dy);
int swf_TextSetCharRecord(TAG * t,SWFFONT * font,U8 * s,int scale,U8 gbits,U8 abits);

int swf_TextPrintDefineText(TAG * t,SWFFONT * f);
// Prints text defined in tag t with font f to stdout

/* notice: if you set the fontid, make sure the corresponding font has layout information */
void swf_SetEditText(TAG*tag, U16 flags, SRECT r, char*text, RGBA*color, 
	int maxlength, U16 font, U16 height, EditTextLayout*layout, char*variable);

SRECT swf_SetDefineText(TAG*tag, SWFFONT*font, RGBA*rgb, char*text, int scale, MATRIX* m);

void swf_DrawText(drawer_t*draw, SWFFONT*font, int size, char*text);

// swffont.c

SWFFONT* swf_LoadTrueTypeFont(char*filename);
SWFFONT* swf_LoadT1Font(char*filename);
SWFFONT* swf_LoadFont(char*filename);

void swf_SetLoadFontParameters(int scale, int skip_unused);

// swfdump.c

void swf_DumpHeader(FILE * f,SWF * swf);
void swf_DumpMatrix(FILE * f,MATRIX * m);
void swf_DumpTag(FILE * f,TAG * t); 
void swf_DumpSWF(FILE * f,SWF*swf);
char* swf_TagGetName(TAG*tag);
void swf_DumpFont(SWFFONT * font);

// swfbutton.c

// Button States

#define BS_HIT          0x08
#define BS_DOWN         0x04
#define BS_OVER         0x02
#define BS_UP           0x01

// Button Conditions

/* missing: IDLE_OUTDOWN 
            OUTDOWN_OVERUP
	    OVERUP_OUTDOWN
*/
#define BC_OVERDOWN_IDLE        0x0100
#define BC_IDLE_OVERDOWN        0x0080
#define BC_OUTDOWN_IDLE         0x0040
#define BC_OUTDOWN_OVERDOWN     0x0020
#define BC_OVERDOWN_OUTDOWN     0x0010
#define BC_OVERDOWN_OVERUP      0x0008
#define BC_OVERUP_OVERDOWN      0x0004
#define BC_OVERUP_IDLE          0x0002
#define BC_IDLE_OVERUP          0x0001

#define BC_KEY(c) (c<<9)

#define BC_CURSORLEFT           0x0200
#define BC_CURSORRIGHT          0x0400
#define BC_POS1                 0x0600
#define BC_END                  0x0800
#define BC_INSERT               0x0a00
#define BC_DELETE               0x0c00
#define BC_CLEAR		0x0e00
#define BC_BACKSPACE            0x1000
#define BC_ENTER                0x1a00
#define BC_CURSORUP             0x1c00
#define BC_CURSORDOWN           0x1e00
#define BC_PAGEUP               0x2000
#define BC_PAGEDOWN             0x2200
#define BC_TAB                  0x2400
#define BC_ESCAPE		0x3600
#define BC_SPACE                0x4000

/* these are probably only valid with linux:
   Ctrl-A	 0x0200
   Ctrl-X	 0x3000
   Ctrl-Y	 0x3200
   Ctrl-Z	 0x3400
   Escape/Ctrl-[ 0x3600
   Ctrl-\	 0x3800
   Ctrl-]	 0x3a00
   Ctrl-^	 0x3c00
   Ctrl-/	 0x3e00
   */

/* everything above 0x4000 is standard ascii:
   0x4000 ' ' 0x4200 '!' 0x4600 '#' 0x4800 '$' 0x4a00 '%' 0x4c00 '&' ...
   0x6000 '0' ... 0x7200 '9' 
   0x8000 '@' 
   0x8200 'A' ...  0xb400 'Z' 
   ...
   0xfc00 '~'
 */

// Button Flag

#define BF_TRACKMENU            0x01

int swf_ButtonSetRecord(TAG * t,U8 state,U16 id,U16 layer,MATRIX * m,CXFORM * cx);
int swf_ButtonSetCondition(TAG * t,U16 condition); // for DefineButton2
int swf_ButtonSetFlags(TAG * t,U8 flags);  // necessary for DefineButton2
int swf_ButtonPostProcess(TAG * t,int anz_action); // Set all offsets in DefineButton2-Tags (how many conditions to process)

// swfbits.c

typedef int JPEGBITS,* LPJPEGBITS; // cover libjpeg structures

JPEGBITS * swf_SetJPEGBitsStart(TAG * t,int width,int height,int quality);
int swf_SetJPEGBitsLines(JPEGBITS * jpegbits,U8 ** data,int n);
int swf_SetJPEGBitsLine(JPEGBITS * jpegbits,U8 * data);
int swf_SetJPEGBitsFinish(JPEGBITS * jpegbits);

void swf_GetJPEGSize(char * fname, int*width, int*height);

int swf_SetJPEGBits(TAG * t,char * fname,int quality);
void swf_SetJPEGBits2(TAG * t,U16 width,U16 height,RGBA * bitmap,int quality);
int swf_SetJPEGBits3(TAG * tag,U16 width,U16 height,RGBA* bitmap, int quality);

#define BYTES_PER_SCANLINE(width) ((width+3)&0xfffffffc)

#define BMF_8BIT        3               // Bitmap formats
#define BMF_16BIT       4
#define BMF_32BIT       5

int swf_SetLosslessBits(TAG * t,U16 width,U16 height,void * bitmap,U8 bitmap_flags);
int swf_SetLosslessBitsIndexed(TAG * t,U16 width,U16 height,U8 * bitmap,RGBA * palette,U16 ncolors);
int swf_SetLosslessBitsGrayscale(TAG * t,U16 width,U16 height,U8 * bitmap);

// swfsound.c
void swf_SetSoundStreamHead(TAG*tag, int avgnumsamples);
void swf_SetSoundStreamBlock(TAG*tag, S16*samples, int seek, char first); /* expects 2304 samples */
void swf_SetSoundDefine(TAG*tag, S16*samples, int num);
void swf_SetSoundInfo(TAG*tag, SOUNDINFO*info);

// swftools.c

U8 swf_isDefiningTag(TAG * t);
U8 swf_isPseudoDefiningTag(TAG * t);
U8 swf_isAllowedSpriteTag(TAG * t);
U16 swf_GetDefineID(TAG * t);
SRECT swf_GetDefineBBox(TAG * t);
void swf_SetDefineID(TAG * t, U16 newid);
U16 swf_GetPlaceID(TAG * t); //PLACEOBJECT, PLACEOBJECT2 (sometimes), REMOVEOBJECT
int swf_GetDepth(TAG * t); //PLACEOBJECT,PLACEOBJECT2,REMOVEOBJECT,REMOVEOBJECT2,SETTABINDEX
char* swf_GetName(TAG * t); //PLACEOBJECT2, FRAMELABEL
MATRIX * swf_MatrixJoin(MATRIX * d,MATRIX * s1,MATRIX * s2);
MATRIX * swf_MatrixMapTriangle(MATRIX * m,int dx,int dy,
                    int x0,int y0,int x1,int y1,int x2,int y2);
int swf_GetNumUsedIDs(TAG * t);
void swf_GetUsedIDs(TAG * t, int * positions);
void swf_Relocate(SWF*swf, char*bitmap); // bitmap is 65536 bytes, bitmap[a]==0 means id a is free
void swf_RelocateDepth(SWF*swf, char*bitmap); // bitmap is 65536 bytes, bitmap[d]==0 means depth d is free

TAG* swf_Concatenate (TAG*list1,TAG*list2); // warning: both list1 and list2 are invalid after this call.

// swfcgi.c

void swf_uncgi();  // same behaviour as Steven Grimm's uncgi-library

// swfaction.c

typedef struct _ActionTAG 
{ U8            op;
  U16           len;
  U8 *          data;

  struct _ActionTAG * next;
  struct _ActionTAG * prev;

  struct _ActionTAG * parent;
  U8 tmp[4]; // store small operands here.
} ActionTAG;

typedef struct _ActionMarker
{
  ActionTAG* atag;
} ActionMarker;

ActionTAG* swf_ActionGet(TAG*tag);
void swf_ActionFree(ActionTAG*tag);
void swf_ActionSet(TAG*tag, ActionTAG*actions);
void swf_DumpActions(ActionTAG*atag, char*prefix);
void swf_ActionEnumerateURLs(ActionTAG*atag, char*(*callback)(char*));
void swf_ActionEnumerateTargets(ActionTAG*atag, char*(*callback)(char*));
void swf_ActionEnumerateStrings(ActionTAG*atag, char*(*callback)(char*));

// using action/actioncompiler.h:
ActionTAG* swf_ActionCompile(const char* source, int version);

ActionTAG* action_End(ActionTAG*atag);
ActionTAG* action_NextFrame(ActionTAG*atag);
ActionTAG* action_PreviousFrame(ActionTAG*atag);
ActionTAG* action_Play(ActionTAG*atag);
ActionTAG* action_Stop(ActionTAG*atag);
ActionTAG* action_ToggleQuality(ActionTAG*atag);
ActionTAG* action_StopSounds(ActionTAG*atag);
ActionTAG* action_Add(ActionTAG*atag);
ActionTAG* action_Subtract(ActionTAG*atag);
ActionTAG* action_Multiply(ActionTAG*atag);
ActionTAG* action_Divide(ActionTAG*atag);
ActionTAG* action_Equals(ActionTAG*atag);
ActionTAG* action_Less(ActionTAG*atag);
ActionTAG* action_And(ActionTAG*atag);
ActionTAG* action_Or(ActionTAG*atag);
ActionTAG* action_Not(ActionTAG*atag);
ActionTAG* action_StringEquals(ActionTAG*atag);
ActionTAG* action_StringLength(ActionTAG*atag);
ActionTAG* action_StringExtract(ActionTAG*atag);
ActionTAG* action_Pop(ActionTAG*atag);
ActionTAG* action_ToInteger(ActionTAG*atag);
ActionTAG* action_GetVariable(ActionTAG*atag);
ActionTAG* action_SetVariable(ActionTAG*atag);
ActionTAG* action_SetTarget2(ActionTAG*atag);
ActionTAG* action_StringAdd(ActionTAG*atag);
ActionTAG* action_GetProperty(ActionTAG*atag);
ActionTAG* action_SetProperty(ActionTAG*atag);
ActionTAG* action_CloneSprite(ActionTAG*atag);
ActionTAG* action_RemoveSprite(ActionTAG*atag);
ActionTAG* action_Trace(ActionTAG*atag);
ActionTAG* action_StartDrag(ActionTAG*atag);
ActionTAG* action_EndDrag(ActionTAG*atag);
ActionTAG* action_StringLess(ActionTAG*atag);
ActionTAG* action_RandomNumber(ActionTAG*atag);
ActionTAG* action_MBStringLength(ActionTAG*atag);
ActionTAG* action_CharToAscii(ActionTAG*atag);
ActionTAG* action_AsciiToChar(ActionTAG*atag);
ActionTAG* action_GetTime(ActionTAG*atag);
ActionTAG* action_MBStringExtract(ActionTAG*atag);
ActionTAG* action_MBCharToAscii(ActionTAG*atag);
ActionTAG* action_MBAsciiToChar(ActionTAG*atag);
ActionTAG* action_Delete(ActionTAG*atag);
ActionTAG* action_Delete2(ActionTAG*atag);
ActionTAG* action_DefineLocal(ActionTAG*atag);
ActionTAG* action_CallFunction(ActionTAG*atag);
ActionTAG* action_Return(ActionTAG*atag);
ActionTAG* action_Modulo(ActionTAG*atag);
ActionTAG* action_NewObject(ActionTAG*atag);
ActionTAG* action_DefineLocal2(ActionTAG*atag);
ActionTAG* action_InitArray(ActionTAG*atag);
ActionTAG* action_Makehash(ActionTAG*atag);
ActionTAG* action_TypeOf(ActionTAG*atag);
ActionTAG* action_TargetPath(ActionTAG*atag);
ActionTAG* action_Enumerate(ActionTAG*atag);
ActionTAG* action_Add2(ActionTAG*atag);
ActionTAG* action_Less2(ActionTAG*atag);
ActionTAG* action_Equals2(ActionTAG*atag);
ActionTAG* action_ToNumber(ActionTAG*atag);
ActionTAG* action_ToString(ActionTAG*atag);
ActionTAG* action_PushDuplicate(ActionTAG*atag);
ActionTAG* action_StackSwap(ActionTAG*atag);
ActionTAG* action_GetMember(ActionTAG*atag);
ActionTAG* action_SetMember(ActionTAG*atag);
ActionTAG* action_Increment(ActionTAG*atag);
ActionTAG* action_Decrement(ActionTAG*atag);
ActionTAG* action_CallMethod(ActionTAG*atag);
ActionTAG* action_NewMethod(ActionTAG*atag);
ActionTAG* action_BitAnd(ActionTAG*atag);
ActionTAG* action_BitOr(ActionTAG*atag);
ActionTAG* action_BitXor(ActionTAG*atag);
ActionTAG* action_BitLShift(ActionTAG*atag);
ActionTAG* action_BitRShift(ActionTAG*atag);
ActionTAG* action_BitURShift(ActionTAG*atag);
ActionTAG* action_GotoFrame(ActionTAG*atag, U16 frame);
ActionTAG* action_GetUrl(ActionTAG*atag, char* url, char* label);
ActionTAG* action_StoreRegister(ActionTAG*atag, U8 reg);
ActionTAG* action_Constantpool(ActionTAG*atag, char* constantpool);
ActionTAG* action_WaitForFrame(ActionTAG*atag, U16 frame, U8 skip);
ActionTAG* action_SetTarget(ActionTAG*atag, char* target);
ActionTAG* action_GotoLabel(ActionTAG*atag, char* label);
ActionTAG* action_WaitForFrame2(ActionTAG*atag, U8 skip);
ActionTAG* action_With(ActionTAG*atag, char*object);
ActionTAG* action_PushString(ActionTAG*atag, char*str);
ActionTAG* action_PushFloat(ActionTAG*atag, float f);
ActionTAG* action_PushNULL(ActionTAG*atag);
ActionTAG* action_PushRegister(ActionTAG*atag, U8 reg);
ActionTAG* action_PushBoolean(ActionTAG*atag, char c);
ActionTAG* action_PushDouble(ActionTAG*atag, double d);
ActionTAG* action_PushInt(ActionTAG*atag, int i);
ActionTAG* action_PushLookup(ActionTAG*atag, U8 index);
ActionTAG* action_Jump(ActionTAG*atag, U16 branch);
ActionTAG* action_GetUrl2(ActionTAG*atag, U8 method);
ActionTAG* action_DefineFunction(ActionTAG*atag, U8*data, int len);
ActionTAG* action_If(ActionTAG*atag, U16 branch);
ActionTAG* action_Call(ActionTAG*atag);
ActionTAG* action_GotoFrame2(ActionTAG*atag, U8 method);
ActionMarker action_setMarker(ActionTAG*atag);
void action_fixjump(ActionMarker m1, ActionMarker m2);

// swfobject.c

// The following 3 routines only use placeobject2:

int swf_ObjectPlace(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,U8 * name);
int swf_ObjectPlaceClip(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,U8 * name, U16 clipaction);
int swf_ObjectMove(TAG * t,U16 depth,MATRIX * m,CXFORM * cx);

typedef struct _SWFPLACEOBJECT {
    U16 depth;
    U16 id; // may be 0
    bool move; //true: move/replace character, false: set character
    MATRIX matrix;
    CXFORM cxform;
    U16 ratio;
    U8*name;
    U16 clipdepth;
    ActionTAG* actions;
} SWFPLACEOBJECT;

void swf_SetPlaceObject(TAG * t,SWFPLACEOBJECT* obj);
void swf_GetPlaceObject(TAG * t,SWFPLACEOBJECT* obj);
void swf_PlaceObjectFree(SWFPLACEOBJECT* obj);

// swfvideo.c

typedef struct _VIDEOSTREAM
{
    int width;
    int height;
    int linex;

    int owidth;
    int oheight;
    int olinex;

    int frame;
    YUV*oldpic;
    YUV*current;
    int bbx;
    int bby;
    int*mvdx;
    int*mvdy;
    int quant;

    /* modifyable: */
    int do_motion; //enable motion compensation (slow!)

} VIDEOSTREAM;

void swf_SetVideoStreamDefine(TAG*tag, VIDEOSTREAM*stream, U16 frames, U16 width, U16 height);
void swf_SetVideoStreamIFrame(TAG*tag, VIDEOSTREAM*s, RGBA*pic, int quant/* 1-31, 1=best quality, 31=best compression*/);
void swf_SetVideoStreamPFrame(TAG*tag, VIDEOSTREAM*s, RGBA*pic, int quant/* 1-31, 1=best quality, 31=best compression*/);
void swf_SetVideoStreamMover(TAG*tag, VIDEOSTREAM*s, signed char* movex, signed char* movey, int quant);
void swf_VideoStreamClear(VIDEOSTREAM*stream);

#ifdef __cplusplus
}
#endif

#endif

