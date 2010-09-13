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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <fcntl.h>
#include <ctype.h>
#include "../config.h"
#include "./bitio.h"
#include "./drawer.h"
#include "./mem.h"
#include "./types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_RFXSWF
#ifdef RFXSWF_DISABLESOUND
#define NO_MP3
#endif

typedef signed SFIXED;
typedef signed SCOORD;

#define SCOORD_MAX 0x7fffffffl
#define SCOORD_MIN -0x80000000l

// Basic Structures

typedef struct _SPOINT
{ SCOORD        x;
  SCOORD        y;
} SPOINT;

typedef struct _RGBA
{ U8    a;
  U8    r;
  U8    g;
  U8    b;
} RGBA;

typedef struct _YUV
{
  U8	y,u,v;
} YUV;

typedef struct _SRECT
{ SCOORD        xmin;
  SCOORD        ymin;
  SCOORD        xmax;
  SCOORD        ymax;
} SRECT;

typedef struct _MATRIX
{ SFIXED        sx,r1, tx;
  SFIXED        r0,sy, ty;
} MATRIX;

typedef struct _CXFORM
{ S16           a0, a1; /* mult, add */
  S16           r0, r1;
  S16           g0, g1;
  S16           b0, b1;
} CXFORM;

#define GRADIENT_LINEAR 0x10
#define GRADIENT_RADIAL 0x12
typedef struct _GRADIENT
{
    int num;
    U8* ratios;
    RGBA* rgba;
} GRADIENT;

typedef struct _FILTER
{
    U8 type;
} FILTER;

typedef struct _FILTERLIST
{
    int num;
    FILTER*filter[8];
} FILTERLIST;

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

} TAG;

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

#define FILEATTRIBUTE_USENETWORK 1
#define FILEATTRIBUTE_AS3 8
#define FILEATTRIBUTE_SYMBOLCLASS 16
#define FILEATTRIBUTE_USEACCELERATEDBLIT 32
#define FILEATTRIBUTE_USEHARDWAREGPU 64

typedef struct _SWF
{ U8            fileVersion;
  U8		compressed;     // SWF or SWC?
  U32           fileSize;       // valid after load and save
  SRECT         movieSize;
  U16           frameRate;
  U16           frameCount;     // valid after load and save
  TAG *         firstTag;
  U32           fileAttributes; // for SWFs >= Flash9
} SWF;

// Basic Functions

SWF* swf_OpenSWF(char*filename);
int  swf_ReadSWF2(reader_t*reader, SWF * swf);   // Reads SWF via callback
int  swf_ReadSWF(int handle,SWF * swf);     // Reads SWF to memory (malloc'ed), returns length or <0 if fails
int  swf_WriteSWF2(writer_t*writer, SWF * swf);     // Writes SWF via callback, returns length or <0 if fails
int  swf_WriteSWF(int handle,SWF * swf);    // Writes SWF to file, returns length or <0 if fails
int  swf_SaveSWF(SWF * swf, char*filename);
int  swf_WriteCGI(SWF * swf);               // Outputs SWF with valid CGI header to stdout
void swf_FreeTags(SWF * swf);               // Frees all malloc'ed memory for swf
SWF* swf_CopySWF(SWF*swf);
void swf_ReadABCfile(char*filename, SWF*swf);

// for streaming:
int  swf_WriteHeader(int handle,SWF * swf);    // Writes Header of swf to file
int  swf_WriteHeader2(writer_t*writer,SWF * swf);    // Writes Header of swf to file
int  swf_WriteTag(int handle,TAG * tag);    // Writes TAG to file
int  swf_WriteTag2(writer_t*writer, TAG * t); //Write TAG via callback

int  swf_ReadHeader(reader_t*reader, SWF * swf);   // Reads SWF Header via callback

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
TAG * swf_DeleteTag(SWF*swf, TAG * t);

void  swf_ClearTag(TAG * t);                //frees tag data
void  swf_ResetTag(TAG*tag, U16 id);        //set's tag position and length to 0, without freeing it
TAG*  swf_CopyTag(TAG*tag, TAG*to_copy);     //stores a copy of another tag into this taglist

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
int   swf_CountUBits(U32 v,int nbits);
int   swf_CountBits(U32 v,int nbits);

int   swf_GetBlock(TAG * t,U8 * b,int l);   // resets Bitcount
int   swf_SetBlock(TAG * t,const U8 * b,int l);

U8    swf_GetU8(TAG * t);                   // resets Bitcount
U16   swf_GetU16(TAG * t);
#define swf_GetS16(tag)     ((S16)swf_GetU16(tag))
U32   swf_GetU32(TAG * t);
float swf_GetF16(TAG * t);
void  swf_GetRGB(TAG * t, RGBA * col);
void  swf_GetRGBA(TAG * t, RGBA * col);
void  swf_GetGradient(TAG * t, GRADIENT * gradient, char alpha);
void  swf_SetGradient(TAG * tag, GRADIENT * gradient, char alpha);
void  swf_FreeGradient(GRADIENT* gradient);
char* swf_GetString(TAG*t);
int   swf_SetU8(TAG * t,U8 v);              // resets Bitcount
int   swf_SetU16(TAG * t,U16 v);
void  swf_SetS16(TAG * t,int v);
int   swf_SetU32(TAG * t,U32 v);
void  swf_SetF16(TAG * t,float f);
void  swf_SetString(TAG*t,const char*s);

float floatToF16(float f);
float F16toFloat(U16 x);

/* abc datatypes */
U32 swf_GetU30(TAG*tag);
int swf_SetU30(TAG*tag, U32 u);
void swf_SetABCU32(TAG*tag, U32 u);
U32 swf_GetABCU32(TAG*tag);
void swf_SetABCS32(TAG*tag, S32 v);
S32 swf_GetABCS32(TAG*tag);
int    swf_SetU30String(TAG*tag, const char*str, int len);
double swf_GetD64(TAG*tag);
int    swf_SetD64(TAG*tag, double v);
int    swf_GetU24(TAG*tag);
int    swf_GetS24(TAG*tag);
int    swf_SetU24(TAG*tag, U32 v);
int    swf_SetS24(TAG*tag, U32 v);

//int   swf_GetPoint(TAG * t,SPOINT * p);     // resets Bitcount
int   swf_GetRect(TAG * t,SRECT * r);
int   swf_GetMatrix(TAG * t,MATRIX * m);
int   swf_GetCXForm(TAG * t,CXFORM * cx,U8 alpha);

double swf_GetFixed(TAG * t);
void swf_SetFixed(TAG * t, double f);
float swf_GetFixed8(TAG * t);
void swf_SetFixed8(TAG * t, float f);

//int   swf_SetPoint(TAG * t,SPOINT * p);     // resets Bitcount
int   swf_SetRect(TAG * t,SRECT * r);
int   swf_SetMatrix(TAG * t,MATRIX * m);
int   swf_SetCXForm(TAG * t,CXFORM * cx,U8 alpha);
int   swf_SetRGB(TAG * t,RGBA * col);
int   swf_SetRGBA(TAG * t,RGBA * col);
void  swf_SetPassword(TAG * t, const char * password);

int   swf_VerifyPassword(TAG * t, const char * password);

// helper functions:

SRECT swf_ClipRect(SRECT border, SRECT r);
void swf_ExpandRect(SRECT*src, SPOINT add);
void swf_ExpandRect2(SRECT*src, SRECT*add);
void swf_ExpandRect3(SRECT*src, SPOINT center, int radius);
SPOINT swf_TurnPoint(SPOINT p, MATRIX* m);
SRECT swf_TurnRect(SRECT r, MATRIX* m);

#ifndef FAILED
#define FAILED(b)       ((b)<0)
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
#define ST_ENABLEDEBUGGER2      64 /* version 8 */
#define ST_SCRIPTLIMITS		65 /* version 7- u16 maxrecursedepth, u16 scripttimeoutseconds */
#define ST_SETTABINDEX		66 /* version 7- u16 depth(!), u16 tab order value */
#define ST_FILEATTRIBUTES	69 /* version 8 (required)- */
#define ST_PLACEOBJECT3		70 /* version 8 */
#define ST_IMPORTASSETS2	71 /* version 8 */
#define ST_RAWABC               72 /* version 9, used by flex */
#define ST_DEFINEFONTALIGNZONES 73 /* version 8 */
#define ST_CSMTEXTSETTINGS	74 /* version 8 */
#define ST_DEFINEFONT3		75 /* version 8 */
#define ST_SYMBOLCLASS		76 /* version 9 */
#define ST_METADATA		77 /* version 8 */
#define ST_DEFINESCALINGGRID    78 /* version 8 */
#define ST_DOABC		82 /* version 9 */
#define ST_DEFINESHAPE4		83 /* version 8 */
#define ST_DEFINEMORPHSHAPE2    84 /* version 8 */
#define ST_SCENEDESCRIPTION	86 /* version 9 */
#define ST_DEFINEBINARY		87 /* version 9 */
#define ST_DEFINEFONTNAME	88 /* version 9 */

/* custom tags- only valid for swftools */
#define ST_REFLEX              777 /* to identify generator software */
#define ST_GLYPHNAMES          778

// Advanced Funtions

// swfshape.c

typedef struct _LINESTYLE
{ U16           width;
  RGBA          color;
} LINESTYLE;

#define FILL_SOLID      0x00
#define FILL_LINEAR     0x10  // Gradient
#define FILL_RADIAL     0x12
#define FILL_TILED      0x40  // Bitmap
#define FILL_CLIPPED    0x41

typedef struct _FILLSTYLE
{ U8	    type;
  RGBA	    color;
  MATRIX    m;
  U16	    id_bitmap;
  GRADIENT  gradient;
} FILLSTYLE;

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
} SHAPE;

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

enum SHAPELINETYPE {moveTo, lineTo, splineTo};
typedef struct _SHAPELINE
{
    enum SHAPELINETYPE type;
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
char  swf_ShapeIsEmpty(SHAPE*s);

int   swf_GetSimpleShape(TAG * t,SHAPE ** s); // without Linestyle/Fillstyle Record
int   swf_SetSimpleShape(TAG * t,SHAPE * s);   // without Linestyle/Fillstyle Record

int   swf_ShapeAddLineStyle(SHAPE * s,U16 width,RGBA * color);
int   swf_ShapeAddSolidFillStyle(SHAPE * s,RGBA * color);
int   swf_ShapeAddBitmapFillStyle(SHAPE * s,MATRIX * m,U16 id_bitmap,int clip);
int   swf_ShapeAddGradientFillStyle(SHAPE * s,MATRIX * m,GRADIENT* gradient,int radial);
int   swf_ShapeAddFillStyle2(SHAPE * s,FILLSTYLE*fs);

int   swf_SetShapeStyles(TAG * t,SHAPE * s);
int   swf_ShapeCountBits(SHAPE * s,U8 * fbits,U8 * lbits);
int   swf_SetShapeBits(TAG * t,SHAPE * s);
int   swf_SetShapeHeader(TAG * t,SHAPE * s); // one call for upper three functions

int   swf_ShapeSetMove(TAG * t,SHAPE * s,S32 x,S32 y);
int   swf_ShapeSetStyle(TAG * t,SHAPE * s,int line,int fill0,int fill1);
#define UNDEFINED_COORD 0x7fffffff
int   swf_ShapeSetAll(TAG * t,SHAPE * s,S32 x,S32 y,int line,int fill0,int fill1);

int   swf_ShapeSetLine(TAG * t,SHAPE * s,S32 x,S32 y);
int   swf_ShapeSetCurve(TAG * t,SHAPE * s,S32 x,S32 y,S32 ax,S32 ay);
int   swf_ShapeSetCircle(TAG * t,SHAPE * s,S32 x,S32 y,S32 rx,S32 ry);
int   swf_ShapeSetEnd(TAG * t);
int   swf_SetShapeStyleCount(TAG * t,U16 n);
int   swf_SetFillStyle(TAG * t,FILLSTYLE * f);
int   swf_SetLineStyle(TAG * t,LINESTYLE * l);


void swf_ShapeSetRectangle(TAG*tag, U16 shapeid, int width, int height, RGBA*rgba);
void swf_ShapeSetRectangleWithBorder(TAG*tag, U16 shapeid, int width, int height, RGBA*rgba, int linewidth, RGBA*linecolor);
void  swf_ShapeSetBitmapRect(TAG * t, U16 gfxid, int width, int height);

//SHAPELINE* swf_ParseShapeData(U8*data, int bits, int fillbits, int linebits);
SHAPE2*	   swf_ShapeToShape2(SHAPE*shape);
void	   swf_Shape2ToShape(SHAPE2*shape2, SHAPE*shape);
SRECT	   swf_GetShapeBoundingBox(SHAPE2*shape);
void	    swf_SetShape2(TAG*tag, SHAPE2*shape);
SHAPE2*    swf_Shape2Clone(SHAPE2 * s);
void	   swf_Shape2Free(SHAPE2 * s);
void	swf_DumpShape(SHAPE2*shape2);

void swf_ParseDefineShape(TAG*tag, SHAPE2*shape);
void swf_SetShape2(TAG*tag, SHAPE2*shape2);

void swf_RecodeShapeData(U8*data, int bitlen, int in_bits_fill, int in_bits_line,
                         U8**destdata, U32*destbitlen, int out_bits_fill, int out_bits_line);

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
{ U16          ascent;
  U16          descent;
  U16          leading;
  SRECT      * bounds;
  U16	       kerningcount;
  SWFKERNING * kerning;
} SWFLAYOUT;

typedef struct
{ S16         advance;
  SHAPE *     shape;
} SWFGLYPH;

typedef struct _SWFGLYPHPAIR
{
    U16 char1;
    U16 char2;
    int num;
} SWFGLYPHPAIR;

typedef struct _FONTUSAGE
{ int* chars;
  char is_reduced;
  int used_glyphs;
  int glyphs_specified;
  U16 smallest_size;

  SWFGLYPHPAIR* neighbors;
  int num_neighbors;
  int neighbors_size;
  int* neighbors_hash;
  int neighbors_hash_size;
} FONTUSAGE;

#define FONT_STYLE_BOLD 1
#define FONT_STYLE_ITALIC 2
#define FONT_ENCODING_UNICODE 1
#define FONT_ENCODING_ANSI 2
#define FONT_ENCODING_SHIFTJIS 4

#define FONTALIGN_THIN 0 
#define FONTALIGN_MEDIUM 1
#define FONTALIGN_THICK 2

typedef struct _ALIGNZONE
{
    U16 x,y;
    U16 dx,dy;
} ALIGNZONE;

typedef struct _SWFFONT
{ int		id; // -1 = not set
  U8		version; // 0 = not set, 1 = definefont, 2 = definefont2, 3 = definefont3
  U8 *          name;
  SWFLAYOUT *   layout;
  int           numchars;
  int           maxascii; // highest mapped ascii/unicode value

  U8		style;
  U8		encoding;

  U16	*	glyph2ascii;
  int	*	ascii2glyph;
  int   *       glyph2glyph; // only if the font is resorted
  SWFGLYPH *	glyph;
  ALIGNZONE *	alignzones;
  U8            alignzone_flags;
  U8		language;
  char **	glyphnames;

  FONTUSAGE *   use;

} SWFFONT;


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

#define ET_ALIGN_LEFT 0
#define ET_ALIGN_RIGHT  1
#define ET_ALIGN_CENTER 2
#define ET_ALIGN_JUSTIFY 3

typedef struct _EditTextLayout
{
    U8 align; // 0=left, 1=right, 2=center, 3=justify
    U16 leftmargin;
    U16 rightmargin;
    U16 indent;
    U16 leading;
} EditTextLayout;

int swf_FontEnumerate(SWF * swf,void (*FontCallback) (void*,U16,U8*), void*self);
// -> void fontcallback(U16 id,U8 * name); returns number of defined fonts

int swf_FontExtract(SWF * swf,int id,SWFFONT ** f);
// Fetches all available information from DefineFont, DefineFontInfo, DefineText, ...
// id = FontID, id=0 -> Extract first Font
int swf_FontExtract_DefineFont2(int id, SWFFONT * font, TAG * tag);
int swf_FontExtract_DefineFontInfo(int id, SWFFONT * f, TAG * t);
int swf_FontExtract_DefineFont(int id, SWFFONT * f, TAG * t);
int swf_FontExtract_GlyphNames(int id, SWFFONT * f, TAG * tag);
int swf_FontExtract_DefineFontAlignZones(int id, SWFFONT * font, TAG * tag);


int swf_FontIsItalic(SWFFONT * f);
int swf_FontIsBold(SWFFONT * f);

int swf_FontSetID(SWFFONT * f,U16 id);
int swf_FontReduce(SWFFONT * f);
int swf_FontReduce_swfc(SWFFONT * f);

int swf_FontInitUsage(SWFFONT * f);
int swf_FontUseGlyph(SWFFONT * f, int glyph, U16 size);
void swf_FontUsePair(SWFFONT * f, int char1, int char2);
int swf_FontUseGetPair(SWFFONT * f, int char1, int char2);
int swf_FontUseAll(SWFFONT* f);
int swf_FontUseUTF8(SWFFONT * f, const U8 * s, U16 size);
int swf_FontUse(SWFFONT* f,U8 * s);
void swf_FontSort(SWFFONT * font);

int swf_FontSetDefine(TAG * t,SWFFONT * f);
int swf_FontSetDefine2(TAG * t,SWFFONT * f);
int swf_FontSetInfo(TAG * t,SWFFONT * f);
void swf_FontSetAlignZones(TAG*t, SWFFONT *f);

void swf_FontCreateLayout(SWFFONT*f);
void swf_FontCreateAlignZones(SWFFONT * f);
void swf_FontAddLayout(SWFFONT * f, int ascent, int descent, int leading);
void swf_FontPostprocess(SWF*swf);

int swf_ParseDefineText(TAG * t, void(*callback)(void*self, int*chars, int*xpos, int nr, int fontid, int fontsize, int xstart, int ystart, RGBA* color), void*self);

void swf_WriteFont(SWFFONT* font, char* filename);
SWFFONT* swf_ReadFont(const char* filename);

void swf_FontFree(SWFFONT * f);

U32 swf_TextGetWidth(SWFFONT * font,U8 * s,int scale);
int swf_TextCountBits(SWFFONT * font,U8 * s,int scale,U8 * gbits,U8 * abits);

#define SET_TO_ZERO 0x80000000
int swf_TextSetInfoRecord(TAG * t,SWFFONT * font,U16 size,RGBA * color,int dx,int dy);
int swf_TextSetCharRecord(TAG * t,SWFFONT * font,U8 * s,int scale,U8 gbits,U8 abits);

int swf_TextPrintDefineText(TAG * t,SWFFONT * f);
// Prints text defined in tag t with font f to stdout

void swf_FontPrepareForEditText(SWFFONT * f);

/* notice: if you set the fontid, make sure you call swf_FontPrepareForEditText() for the font first */
void swf_SetEditText(TAG*tag, U16 flags, SRECT r, const char*text, RGBA*color,
	int maxlength, U16 font, U16 height, EditTextLayout*layout, const char*variable);

SRECT swf_SetDefineText(TAG*tag, SWFFONT*font, RGBA*rgb, const char*text, int scale);

void swf_DrawText(drawer_t*draw, SWFFONT*font, int size, const char*text);

// swffont.c

SWFFONT* swf_LoadTrueTypeFont(const char*filename, char flashtype);
SWFFONT* swf_LoadT1Font(const char*filename);
SWFFONT* swf_LoadFont(const char*filename, char flashtype);

void swf_SetLoadFontParameters(int scale, int skip_unused, int full_unicode);

// swfdump.c

void swf_DumpHeader(FILE * f,SWF * swf);
void swf_DumpMatrix(FILE * f,MATRIX * m);
void swf_DumpTag(FILE * f,TAG * t);
void swf_DumpSWF(FILE * f,SWF*swf);
void swf_DumpGradient(FILE* f, GRADIENT*gradient);
char* swf_TagGetName(TAG*tag);
void swf_DumpFont(SWFFONT * font);

// swfbits.c

int swf_ImageHasAlpha(RGBA*img, int width, int height);
int swf_ImageGetNumberOfPaletteEntries(RGBA*img, int width, int height, RGBA*palette);

typedef int JPEGBITS;
JPEGBITS * swf_SetJPEGBitsStart(TAG * t,int width,int height,int quality); // deprecated
int swf_SetJPEGBitsLines(JPEGBITS * jpegbits,U8 ** data,int n); // deprecated
int swf_SetJPEGBitsLine(JPEGBITS * jpegbits,U8 * data); // deprecated
int swf_SetJPEGBitsFinish(JPEGBITS * jpegbits); // deprecated

void swf_GetJPEGSize(const char * fname, int*width, int*height);

int swf_SetJPEGBits(TAG * t,const char * fname,int quality);
void swf_SetJPEGBits2(TAG * t,U16 width,U16 height,RGBA * bitmap,int quality);
int swf_SetJPEGBits3(TAG * tag,U16 width,U16 height,RGBA* bitmap, int quality);
RGBA* swf_JPEG2TagToImage(TAG*tag, int*width, int*height);
void swf_RemoveJPEGTables(SWF*swf);

void swf_SaveJPEG(char*filename, RGBA*pixels, int width, int height, int quality);

#define BYTES_PER_SCANLINE(width) ((width+3)&0xfffffffc)

#define BMF_8BIT        3               // Bitmap formats
#define BMF_16BIT       4
#define BMF_32BIT       5

int swf_SetLosslessBits(TAG * t,U16 width,U16 height,void * bitmap,U8 bitmap_flags);
int swf_SetLosslessBitsIndexed(TAG * t,U16 width,U16 height,U8 * bitmap,RGBA * palette,U16 ncolors);
int swf_SetLosslessBitsGrayscale(TAG * t,U16 width,U16 height,U8 * bitmap);
void swf_SetLosslessImage(TAG*tag, RGBA*data, int width, int height); //WARNING: will change tag->id

RGBA* swf_DefineLosslessBitsTagToImage(TAG*tag, int*width, int*height);

RGBA* swf_ExtractImage(TAG*tag, int*dwidth, int*dheight);
TAG* swf_AddImage(TAG*tag, int bitid, RGBA*mem, int width, int height, int quality);

// swfsound.c
void swf_SetSoundStreamHead(TAG*tag, int avgnumsamples);
void swf_SetSoundStreamBlock(TAG*tag, S16*samples, int seek, char first); /* expects 2304 samples */
void swf_SetSoundDefine(TAG*tag, S16*samples, int num);
void swf_SetSoundDefineMP3(TAG*tag, U8* data, unsigned length,
                           unsigned SampRate,
                           unsigned Channels,
                           unsigned NumFrames);
void swf_SetSoundInfo(TAG*tag, SOUNDINFO*info);

// swftools.c

void swf_Optimize(SWF*swf);
U8 swf_isDefiningTag(TAG * t);
U8 swf_isPseudoDefiningTag(TAG * t);
U8 swf_isAllowedSpriteTag(TAG * t);
U8 swf_isImageTag(TAG*tag);
U8 swf_isShapeTag(TAG*tag);
U8 swf_isTextTag(TAG*tag);
U8 swf_isFontTag(TAG*tag);
U8 swf_isPlaceTag(TAG*tag);

U16 swf_GetDefineID(TAG * t);
SRECT swf_GetDefineBBox(TAG * t);
void swf_SetDefineBBox(TAG * t, SRECT r);

void swf_SetDefineID(TAG * t, U16 newid);
U16 swf_GetPlaceID(TAG * t); //PLACEOBJECT, PLACEOBJECT2 (sometimes), REMOVEOBJECT
int swf_GetDepth(TAG * t); //PLACEOBJECT,PLACEOBJECT2,REMOVEOBJECT,REMOVEOBJECT2,SETTABINDEX
char* swf_GetName(TAG * t); //PLACEOBJECT2, FRAMELABEL
MATRIX * swf_MatrixJoin(MATRIX * d,MATRIX * s1,MATRIX * s2);
MATRIX * swf_MatrixMapTriangle(MATRIX * m,int dx,int dy,
                    int x0,int y0,int x1,int y1,int x2,int y2);
int swf_GetNumUsedIDs(TAG * t);
void swf_GetUsedIDs(TAG * t, int * positions);
char swf_Relocate(SWF*swf, char*bitmap); // bitmap is 65536 bytes, bitmap[a]==0 means id a is free
void swf_RelocateDepth(SWF*swf, char*bitmap); // bitmap is 65536 bytes, bitmap[d]==0 means depth d is free

TAG* swf_Concatenate (TAG*list1,TAG*list2); // warning: both list1 and list2 are invalid after this call.

RGBA swf_GetSWFBackgroundColor(SWF*swf);

// swfcgi.c

void swf_uncgi();  // same behaviour as Steven Grimm's uncgi-library

// as3/abc.c
void*swf_ReadABC(TAG*tag);
void*swf_DumpABC(FILE*fo, void*code, char*prefix);
void swf_WriteABC(TAG*tag, void*code);
void swf_FreeABC(void*code);
void swf_AddButtonLinks(SWF*swf, char stop_each_frame, char events);
TAG*swf_AddAS3FontDefine(TAG*tag, U16 id, char*fontname);
#include "as3/compiler.h"
#include "as3/import.h"

// swfaction.c

typedef struct _ActionTAG
{ U8            op;
  U16           len;
  U8 *          data;

  struct _ActionTAG * next;
  struct _ActionTAG * prev;

  struct _ActionTAG * parent;
  U8 tmp[8]; // store small operands here.
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

#define ACTION__GOTOFRAME      0x81
#define ACTION__GETURL         0x83
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
ActionTAG* action_GetUrl(ActionTAG*atag, const char* url, char* label);
ActionTAG* action_StoreRegister(ActionTAG*atag, U8 reg);
ActionTAG* action_Constantpool(ActionTAG*atag, char* constantpool);
ActionTAG* action_WaitForFrame(ActionTAG*atag, U16 frame, U8 skip);
ActionTAG* action_SetTarget(ActionTAG*atag, const char* target);
ActionTAG* action_GotoLabel(ActionTAG*atag, char* label);
ActionTAG* action_WaitForFrame2(ActionTAG*atag, U8 skip);
ActionTAG* action_With(ActionTAG*atag, char*object);
ActionTAG* action_PushString(ActionTAG*atag, const char*str);
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

extern char*blendModeNames[];

int swf_ObjectPlace(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,const char* name);
int swf_ObjectPlaceClip(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,const char* name, U16 clipaction);
int swf_ObjectPlaceBlend(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,const char* name, U8 blendmode);
int swf_ObjectMove(TAG * t,U16 depth,MATRIX * m,CXFORM * cx);

#define PF_MOVE         0x01
#define PF_CHAR         0x02
#define PF_MATRIX       0x04
#define PF_CXFORM       0x08
#define PF_RATIO        0x10
#define PF_NAME         0x20
#define PF_CLIPDEPTH    0x40
#define PF_ACTIONEVENT  0x80

#define PF2_FILTERS      0x01
#define PF2_BLENDMODE    0x02
#define PF2_ASBITMAP     0x04
//...

#define BLENDMODE_NORMAL 0
#define BLENDMODE_NORMAL2 1
#define BLENDMODE_LAYER 2
#define BLENDMODE_MULTIPLY 3
#define BLENDMODE_SCREEN 4
#define BLENDMODE_LIGHTEN 5
#define BLENDMODE_DARKEN 6
#define BLENDMODE_ADD 7
#define BLENDMODE_SUBSTRACT 8
#define BLENDMODE_DIFFERENCE 9
#define BLENDMODE_INVERT 10
#define BLENDMODE_ALPHA 11
#define BLENDMODE_ERASE 12
#define BLENDMODE_OVERLAY 13
#define BLENDMODE_HARDLIGHT 14

typedef struct _SWFPLACEOBJECT {
    U8 flags;
    U16 depth;
    U16 id; // may be 0
    char move; //true: move/replace character, false: set character
    MATRIX matrix;
    CXFORM cxform;
    U16 ratio;
    char*name;
    U16 clipdepth;
    ActionTAG* actions;
    U8 blendmode;
    FILTERLIST*filters;
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
void swf_SetVideoStreamBlackFrame(TAG*tag, VIDEOSTREAM*s);
void swf_SetVideoStreamPFrame(TAG*tag, VIDEOSTREAM*s, RGBA*pic, int quant/* 1-31, 1=best quality, 31=best compression*/);
void swf_SetVideoStreamMover(TAG*tag, VIDEOSTREAM*s, signed char* movex, signed char* movey, void** image, int quant);
void swf_VideoStreamClear(VIDEOSTREAM*stream);

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
ActionTAG* swf_ButtonGetAction(TAG*t);

// swfrender.c

typedef struct RENDERBUF
{
    int width;
    int height;
    int posx,posy;
    void*internal;
} RENDERBUF;

void swf_Render_Init(RENDERBUF*buf, int posx, int posy, int width, int height, int antialize, int multiply);
void swf_Render_SetBackground(RENDERBUF*buf, RGBA*img, int width, int height);
void swf_Render_SetBackgroundColor(RENDERBUF*buf, RGBA color);
RGBA* swf_Render(RENDERBUF*dest);
void swf_RenderShape(RENDERBUF*dest, SHAPE2*shape, MATRIX*m, CXFORM*c, U16 depth,U16 clipdepth);
void swf_RenderSWF(RENDERBUF*buf, SWF*swf);
void swf_Render_AddImage(RENDERBUF*buf, U16 id, RGBA*img, int width, int height); /* img is non-premultiplied */
void swf_Render_ClearCanvas(RENDERBUF*dest);
void swf_Render_Delete(RENDERBUF*dest);

// swffilter.c

#define FILTERTYPE_DROPSHADOW 0
#define FILTERTYPE_BLUR 1
#define FILTERTYPE_GLOW 2
#define FILTERTYPE_BEVEL 3
#define FILTERTYPE_GRADIENTGLOW 4
#define FILTERTYPE_CONVOLUTION 5
#define FILTERTYPE_COLORMATRIX 6
#define FILTERTYPE_GRADIENTBEVEL 7

extern char*filtername[];

typedef struct _FILTER_GRADIENTGLOW {
    U8 type;
    GRADIENT*gradient;
    double blurx;
    double blury;
    double angle;
    double distance;
    float strength;
    char innershadow;
    char knockout;
    char composite;
    char ontop;
    int passes;
} FILTER_GRADIENTGLOW;

typedef struct _FILTER_DROPSHADOW {
    U8 type;
    RGBA color;
    double blurx;
    double blury;
    double angle;
    double distance;
    float strength;
    char innershadow;
    char knockout;
    char composite;
    int passes;
} FILTER_DROPSHADOW;

typedef struct _FILTER_BEVEL {
    U8 type;
    RGBA shadow;
    RGBA highlight;
    double blurx;
    double blury;
    double angle;
    double distance;
    float strength;
    char innershadow;
    char knockout;
    char composite;
    char ontop;
    int passes;
} FILTER_BEVEL;

typedef struct _FILTER_BLUR {
    U8 type;
    double blurx;
    double blury;
    int passes;
} FILTER_BLUR;

typedef struct _FILTER_GLOW {
    U8 type;
    RGBA rgba;
    double blurx;
    double blury;
    double strength;
    int passes;
    char innerglow;
    char knockout;
    char composite;
} FILTER_GLOW;

void swf_SetFilter(TAG*tag, FILTER*f);
FILTER*swf_GetFilter(TAG*tag);
FILTER*swf_NewFilter(U8 type);
void swf_DeleteFilter(FILTER*f);

#ifdef __cplusplus
}
#endif

#endif
