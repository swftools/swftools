/* rfxswf.h

   Headers for rfxswf.c and modules

   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#ifndef __RFX_SWF_INCLUDED__
#define __RFX_SWF_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../config.h"
#include "old_rfxswf.h"

#define DEBUG_RFXSWF

// SWF Types

typedef         unsigned long   U32;
typedef         signed long     S32;
typedef         unsigned short  U16;
typedef         signed short    S16;
typedef         unsigned char   U8;
typedef         signed char     S8;
typedef         signed long     SFIXED;
typedef         signed long     SCOORD;

// Basic Structures

typedef struct _SPOINT
{ SCOORD        x;
  SCOORD        y;
} SPOINT, * LPSPOINT;

typedef struct _RGBA
{ U8    r;
  U8    g;
  U8    b;
  U8    a;
} RGBA, * LPRGBA;

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
{ S16           a0, a1;
  S16           r0, r1;
  S16           g0, g1;
  S16           b0, b1;
} CXFORM, * LPCXFORM;

typedef struct _TAG             // NEVER access a Tag-Struct directly !
{ U16           id;
  U32           len;
  U8 *          data;

  int           frame;

  struct _TAG * next;
  struct _TAG * prev;

  U32           memsize;        // to minimize realloc() calls
  U32           pos;            // for Get/Set-Access
  U8            bitmask;        // for Bit-Manipulating Functions [read]
  U8            bitcount;       // [write]
} TAG, * LPTAG;

typedef struct _SWF
{ U8            FileVersion;
  U32           FileSize;       // valid after load and save
  SRECT         MovieSize;
  U16           FrameRate;
  U16           FrameCount;     // valid after load and save
  TAG *         FirstTag;
} SWF, * LPSWF;

// Basic Functions

int  ReadSWF(int handle,SWF * swf);     // Reads SWF to memory (malloc'ed), returns length or <0 if fails
int  WriteSWF(int handle,SWF * swf);    // Writes SWF to file, returns length or <0 if fails
int  WriteCGI(SWF * swf);               // Outputs SWF with valid CGI header to stdout
void FreeTags(SWF * swf);               // Frees all malloc'ed memory for swf
    
TAG * InsertTag(TAG * after,U16 id);    // updates frames, if necessary
int   DeleteTag(TAG * t);

void  SetTagPos(TAG * t,U32 pos);       // resets Bitcount
U32   GetTagPos(TAG * t);

TAG * NextTag(TAG * t);
TAG * PrevTag(TAG * t);

int   GetFrameNo(TAG * t);              // should be renamed to TagGetFrame
U16   GetTagID(TAG * t);                // ... TagGetID
U32   GetDataSize(TAG * t);             // ... TagGetDataSize
U8*   GetDataSizePtr(TAG * t);

U32   GetBits(TAG * t,int nbits);
S32   GetSBits(TAG * t,int nbits);
int   SetBits(TAG * t,U32 v,int nbits);

int   GetBlock(TAG * t,U8 * b,int l);   // resets Bitcount
int   SetBlock(TAG * t,U8 * b,int l);

U8    GetU8(TAG * t);                   // resets Bitcount
U16   GetU16(TAG * t);
U32   GetU32(TAG * t);

int   SetU8(TAG * t,U8 v);              // resets Bitcount
int   SetU16(TAG * t,U16 v);
int   SetU32(TAG * t,U32 v);

int   GetPoint(TAG * t,SPOINT * p);     // resets Bitcount
int   GetRect(TAG * t,SRECT * r);
int   GetMatrix(TAG * t,MATRIX * m);
int   GetCXForm(TAG * t,CXFORM * cx,U8 alpha);

int   SetPoint(TAG * t,SPOINT * p);     // resets Bitcount
int   SetRect(TAG * t,SRECT * r);
int   SetMatrix(TAG * t,MATRIX * m);
int   SetCXForm(TAG * t,CXFORM * cx,U8 alpha);
int   SetRGB(TAG * t,RGBA * col);
int   SetRGBA(TAG * t,RGBA * col);

// Function Macros

#define GetS8(tag)      ((S8)GetU8(tag))
#define GetS16(tag)     ((S16)GetU16(tag))
#define GetS32(tag)     ((S32)GetU32(tag))
#define GetCoord(tag)   ((SCOORD)GetU32(tag))
#define GetFixed(tag)   ((SFIXED)GetU32(tag))

#define SetS8(tag,v)    SetU8(tag,(U8)v)
#define SetS16(tag,v)   SetU16(tag,(U16)v)
#define SetS32(tag,v)   SetU32(tag,(U32)v)
#define SetCoord(tag,v) SetU32(tag,(U32)v)
#define SetFixed(tag,v) SetU32(tag,(U32)v)
#define SetString(t,s)  SetBlock(t,s,strlen(s)+1)

#define FAILED(b)       ((b)<0)
#define SUCCEDED(b)     ((b)>=0)

// Tag IDs (adopted from J. C. Kessels' Form2Flash)

#define ST_END                  0
#define ST_SHOWFRAME            1
#define ST_DEFINESHAPE          2
#define ST_FREECHARACTER        3
#define ST_PLACEOBJECT          4
#define ST_REMOVEOBJECT         5
#define ST_DEFINEBITS           6
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

#define ST_REFLEX              777 /* to identify generator software */

// Advanced Funtions

// swfdump.c

void DumpHeader(FILE * f,SWF * swf);
void DumpMatrix(FILE * f,MATRIX * m);
void DumpTag(FILE * f,TAG * t); 
char* getTagName(TAG*tag);

// swfshape.c

typedef struct _LINESTYLE
{ U16           width;
  RGBA          color;
} LINESTYLE, * LPLINESTYLE;

typedef struct _FILLSTYLE
{ U8     type;
  RGBA   color;
  MATRIX         m; 
  U16    id_bitmap;
} FILLSTYLE, * LPFILLSTYLE;
     
typedef struct _SHAPE           // NEVER access a Shape-Struct directly !
{                 
  struct
  { LINESTYLE * data;
    U16         n;
  } linestyle;
                  // note: changes of shape structure
  struct                  // lead to incompatible .efont formats
  { FILLSTYLE * data;
    U16         n;
  } fillstyle;

  S32           px;
  S32           py;
  
  struct
  { U16         fill;
    U16         line;
  } bits;
  
  U8 *          data;
  U32           bitlen;         // length of data in bits
} SHAPE, * LPSHAPE;

// Shapes

int   NewShape(SHAPE ** s);
void  ShapeFree(SHAPE * s);

int   GetSimpleShape(TAG * t,SHAPE ** s); // without Linestyle/Fillstyle Record
int   SetSimpleShape(TAG * t,SHAPE * s);   // without Linestyle/Fillstyle Record

int   ShapeAddLineStyle(SHAPE * s,U16 width,RGBA * color);
int   ShapeAddSolidFillStyle(SHAPE * s,RGBA * color);
int   ShapeAddBitmapFillStyle(SHAPE * s,MATRIX * m,U16 id_bitmap,int clip);

int   SetShapeStyles(TAG * t,SHAPE * s);
int   ShapeCountBits(SHAPE * s,U8 * fbits,U8 * lbits);
int   SetShapeBits(TAG * t,SHAPE * s);
int   SetShapeHeader(TAG * t,SHAPE * s); // one call for upper three functions

int   ShapeSetMove(TAG * t,SHAPE * s,S32 x,S32 y);
int   ShapeSetStyle(TAG * t,SHAPE * s,U16 line,U16 fill0,U16 fill1);
int   ShapeSetAll(TAG * t,SHAPE * s,S32 x,S32 y,U16 line,U16 fill0,U16 fill1);

int   ShapeSetLine(TAG * t,SHAPE * s,S32 x,S32 y);
int   ShapeSetCurve(TAG * t,SHAPE * s,S32 x,S32 y,S32 ax,S32 ay);
int   ShapeSetCircle(TAG * t,SHAPE * s,S32 x,S32 y,S32 rx,S32 ry);
int   ShapeSetEnd(TAG * t);


// swffont.c

// does not support wide characters !

#define MAX_CHAR_PER_FONT 256

typedef struct _SWFLAYOUT
{ S16         ascent;
  S16         descent;
  S16         leading;
  SRECT       bounds[MAX_CHAR_PER_FONT];
  struct
  { U16       count;
    U8 *      data;  // size = count*4 bytes
  } kerning;
} SWFLAYOUT, * LPSWFLAYOUT;

typedef struct _SWFFONT
{ U16           id;
  U8 *          name;
  SWFLAYOUT *   layout;

  U8            flags; // bold/italic/unicode/ansi ...

  U16           codes[MAX_CHAR_PER_FONT];
  
  struct
  { U16         advance;
    U16         gid;            // Glyph-ID after DefineFont
    SHAPE *     shape;
  }             glyph[MAX_CHAR_PER_FONT];
} SWFFONT, * LPSWFFONT;

typedef struct _FONTUSAGE
{ U8 code[MAX_CHAR_PER_FONT];
} FONTUSAGE, * LPFONTUSAGE;

int FontEnumerate(SWF * swf,void (*FontCallback) (U16,U8*));
// -> void fontcallback(U16 id,U8 * name); returns number of defined fonts

int FontExtract(SWF * swf,int id,SWFFONT ** f);
// Fetches all available information from DefineFont, DefineFontInfo, DefineText, ...
// id = FontID, id=0 -> Extract first Font

int FontIsItalic(SWFFONT * f);
int FontIsBold(SWFFONT * f);

int FontSetID(SWFFONT * f,U16 id);
int FontReduce(SWFFONT * f,FONTUSAGE * use);

int FontInitUsage(FONTUSAGE * use);
int FontUse(FONTUSAGE * use,U8 * s);

int FontSetDefine(TAG * t,SWFFONT * f);
int FontSetInfo(TAG * t,SWFFONT * f);

int FontExport(int handle,SWFFONT * f);
int FontImport(int handle,SWFFONT * * f);

void FontFree(SWFFONT * f);

U32 TextGetWidth(SWFFONT * font,U8 * s,int scale);
int TextCountBits(SWFFONT * font,U8 * s,int scale,U8 * gbits,U8 * abits);

int TextSetInfoRecord(TAG * t,SWFFONT * font,U16 size,RGBA * color,S16 dx,S16 dy);
int TextSetCharRecord(TAG * t,SWFFONT * font,U8 * s,int scale,U8 gbits,U8 abits);

int TextPrintDefineText(TAG * t,SWFFONT * f);
// Prints text defined in tag t with font f to stdout


// swfobject.c

// Always use ST_PLACEOBJECT2 !!!

int ObjectPlace(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,U8 * name);
int PlaceObject(TAG * t,U16 id,U16 depth,MATRIX * m,CXFORM * cx,U8 * name, U16 clipaction);
int ObjectMove(TAG * t,U16 depth,MATRIX * m,CXFORM * cx);

// swfbutton.c

// Button States

#define BS_HIT          0x08
#define BS_DOWN         0x04
#define BS_OVER         0x02
#define BS_UP           0x01

// Button Conditions

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
#define BC_BACKSPACE            0x1000
#define BC_ENTER                0x1a00
#define BC_CURSORUP             0x1c00
#define BC_CURSORDOWN           0x1e00
#define BC_PAGEUP               0x2000
#define BC_PAGEDOWN             0x2200
#define BC_TAB                  0x2400
#define BC_SPACE                0x4000

// Button Flag

#define BF_TRACKMENU            0x01

int ButtonSetRecord(TAG * t,U8 state,U16 id,U16 layer,MATRIX * m,CXFORM * cx);
int ButtonSetCondition(TAG * t,U16 condition); // for DefineButton2
int ButtonSetFlags(TAG * t,U8 flags);  // necessary for DefineButton2
int ButtonPostProcess(TAG * t,int anz_action); // Set all offsets in DefineButton2-Tags (how many conditions to process)

// swfbits.c

typedef int JPEGBITS,* LPJPEGBITS; // cover libjpeg structures

JPEGBITS * SetJPEGBitsStart(TAG * t,int width,int height,int quality);
int SetJPEGBitsLines(JPEGBITS * jpegbits,U8 ** data,int n);
int SetJPEGBitsLine(JPEGBITS * jpegbits,U8 * data);
int SetJPEGBitsFinish(JPEGBITS * jpegbits);

int SetJPEGBits(TAG * t,char * fname,int quality); // paste jpg file into swf stream

// swftools.c

char isDefiningTag(TAG * t);
char isAllowedSpriteTag(TAG * t);
U16 GetDefineID(TAG * t);
U16 GetPlaceID(TAG * t); //PLACEOBJECT, PLACEOBJECT2 (sometimes), REMOVEOBJECT
U16 GetDepth(TAG * t); //PLACEOBJECT,PLACEOBJECT2,REMOVEOBJECT,REMOVEOBJECT2
char* GetName(TAG * t); //PLACEOBJECT2, FRAMELABEL
MATRIX * MatrixJoin(MATRIX * d,MATRIX * s1,MATRIX * s2);
MATRIX * MatrixMapTriangle(MATRIX * m,int dx,int dy,
                    int x0,int y0,int x1,int y1,int x2,int y2);


// swfcgi.c

void uncgi();  // same behaviour as Steven Grimm's uncgi-library

#endif
