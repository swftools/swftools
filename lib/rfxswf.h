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
  LPTAG         FirstTag;
} SWF, * LPSWF;

// Basic Functions

int  ReadSWF(int handle,LPSWF swf);     // Reads SWF to memory (malloc'ed), returns length or <0 if fails
int  WriteSWF(int handle,LPSWF swf);    // Writes SWF to file, returns length or <0 if fails
int  WriteCGI(LPSWF swf);               // Outputs SWF with valid CGI header to stdout
void FreeTags(LPSWF swf);               // Frees all malloc'ed memory for swf
    
LPTAG InsertTag(LPTAG after,U16 id);    // updates frames, if necessary
int   DeleteTag(LPTAG t);

void  SetTagPos(LPTAG t,U32 pos);       // resets Bitcount
U32   GetTagPos(LPTAG t);

LPTAG NextTag(LPTAG t);
LPTAG PrevTag(LPTAG t);

int   GetFrameNo(LPTAG t);
U16   GetTagID(LPTAG t);
U32   GetDataSize(LPTAG t);
U8*   GetDataSizePtr(LPTAG t);

U32   GetBits(LPTAG t,int nbits);
S32   GetSBits(LPTAG t,int nbits);
int   SetBits(LPTAG t,U32 v,int nbits);

int   GetBlock(LPTAG t,U8 * b,int l);   // resets Bitcount
int   SetBlock(LPTAG t,U8 * b,int l);

U8    GetU8(LPTAG t);                   // resets Bitcount
U16   GetU16(LPTAG t);
U32   GetU32(LPTAG t);

int   SetU8(LPTAG t,U8 v);              // resets Bitcount
int   SetU16(LPTAG t,U16 v);
int   SetU32(LPTAG t,U32 v);

int   GetPoint(LPTAG t,LPSPOINT p);     // resets Bitcount
int   GetRect(LPTAG t,LPSRECT r);
int   GetMatrix(LPTAG t,LPMATRIX m);
int   GetCXForm(LPTAG t,LPCXFORM cx,U8 alpha);

int   SetPoint(LPTAG t,LPSPOINT p);     // resets Bitcount
int   SetRect(LPTAG t,LPSRECT r);
int   SetMatrix(LPTAG t,LPMATRIX m);
int   SetCXForm(LPTAG t,LPCXFORM cx,U8 alpha);
int   SetRGB(LPTAG t,LPRGBA col);
int   SetRGBA(LPTAG t,LPRGBA col);

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
#define ST_DEFINEMOVIE		38
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

void DumpHeader(FILE * f,LPSWF swf);
void DumpMatrix(FILE * f,LPMATRIX m);
void DumpTag(FILE * f,LPTAG t); 
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
  { LPLINESTYLE data;
    U16         n;
  } linestyle;
                  // note: changes of shape structure
  struct                  // lead to incompatible .efont formats
  { LPFILLSTYLE data;
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

int   NewShape(LPSHAPE * s);
void  ShapeFree(LPSHAPE s);

int   GetSimpleShape(LPTAG t,LPSHAPE * s); // without Linestyle/Fillstyle Record
int   SetSimpleShape(LPTAG t,LPSHAPE s);   // without Linestyle/Fillstyle Record

int   ShapeAddLineStyle(LPSHAPE s,U16 width,LPRGBA color);
int   ShapeAddSolidFillStyle(LPSHAPE s,LPRGBA color);
int   ShapeAddBitmapFillStyle(LPSHAPE s,LPMATRIX m,U16 id_bitmap,int clip);

int   SetShapeStyles(LPTAG t,LPSHAPE s);
int   ShapeCountBits(LPSHAPE s,U8 * fbits,U8 * lbits);
int   SetShapeBits(LPTAG t,LPSHAPE s);
int   SetShapeHeader(LPTAG t,LPSHAPE s); // one call for upper three functions

int   ShapeSetMove(LPTAG t,LPSHAPE s,S32 x,S32 y);
int   ShapeSetStyle(LPTAG t,LPSHAPE s,U16 line,U16 fill0,U16 fill1);
int   ShapeSetAll(LPTAG t,LPSHAPE s,S32 x,S32 y,U16 line,U16 fill0,U16 fill1);

int   ShapeSetLine(LPTAG t,LPSHAPE s,S32 x,S32 y);
int   ShapeSetCurve(LPTAG t,LPSHAPE s,S32 x,S32 y,S32 ax,S32 ay);
int   ShapeSetCircle(LPTAG t,LPSHAPE s,S32 x,S32 y,S32 rx,S32 ry);
int   ShapeSetEnd(LPTAG t);


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
  LPSWFLAYOUT   layout;

  U8            flags; // bold/italic/unicode/ansi ...

  U16           codes[MAX_CHAR_PER_FONT];
  
  struct
  { U16         advance;
    U16         gid;            // Glyph-ID after DefineFont
    LPSHAPE     shape;
  }             glyph[MAX_CHAR_PER_FONT];
} SWFFONT, * LPSWFFONT;

typedef struct _FONTUSAGE
{ U8 code[MAX_CHAR_PER_FONT];
} FONTUSAGE, * LPFONTUSAGE;

int FontEnumerate(LPSWF swf,void (*FontCallback) (U16,U8*));
// -> void fontcallback(U16 id,U8 * name); returns number of defined fonts

int FontExtract(LPSWF swf,int id,LPSWFFONT * f);
// Fetches all available information from DefineFont, DefineFontInfo, DefineText, ...
// id = FontID, id=0 -> Extract first Font

int FontIsItalic(LPSWFFONT f);
int FontIsBold(LPSWFFONT f);

int FontSetID(LPSWFFONT f,U16 id);
int FontReduce(LPSWFFONT f,LPFONTUSAGE use);

int FontInitUsage(LPFONTUSAGE use);
int FontUse(LPFONTUSAGE use,U8 * s);

int FontSetDefine(LPTAG t,LPSWFFONT f);
int FontSetInfo(LPTAG t,LPSWFFONT f);

int FontExport(int handle,LPSWFFONT f);
int FontImport(int handle,LPSWFFONT * f);

void FontFree(LPSWFFONT f);

U32 TextGetWidth(LPSWFFONT font,U8 * s,int scale);
int TextCountBits(LPSWFFONT font,U8 * s,int scale,U8 * gbits,U8 * abits);

int TextSetInfoRecord(LPTAG t,LPSWFFONT font,U16 size,LPRGBA color,S16 dx,S16 dy);
int TextSetCharRecord(LPTAG t,LPSWFFONT font,U8 * s,int scale,U8 gbits,U8 abits);

int TextPrintDefineText(LPTAG t,LPSWFFONT f);
// Prints text defined in tag t with font f to stdout


// swfobject.c

// Always use ST_PLACEOBJECT2 !!!

int ObjectPlace(LPTAG t,U16 id,U16 depth,LPMATRIX m,LPCXFORM cx,U8 * name);
int PlaceObject(LPTAG t,U16 id,U16 depth,LPMATRIX m,LPCXFORM cx,U8 * name, U16 clipaction);
int ObjectMove(LPTAG t,U16 depth,LPMATRIX m,LPCXFORM cx);

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

int ButtonSetRecord(LPTAG t,U8 state,U16 id,U16 layer,LPMATRIX m,LPCXFORM cx);
int ButtonSetCondition(LPTAG t,U16 condition); // for DefineButton2
int ButtonSetFlags(LPTAG t,U8 flags);  // necessary for DefineButton2
int ButtonPostProcess(LPTAG t,int anz_action); // Set all offsets in DefineButton2-Tags (how many conditions to process)

// swfbits.c

typedef int * LPJPEGBITS; // cover libjpeg structures

LPJPEGBITS SetJPEGBitsStart(LPTAG t,int width,int height,int quality);
int SetJPEGBitsLines(LPJPEGBITS jpegbits,U8 ** data,int n);
int SetJPEGBitsLine(LPJPEGBITS jpegbits,U8 * data);
int SetJPEGBitsFinish(LPJPEGBITS jpegbits);

int SetJPEGBits(LPTAG t,char * fname,int quality); // paste jpg file into swf stream

// swftools.c

char isDefiningTag(LPTAG t);
char isAllowedSpriteTag(LPTAG t);
U16 GetDefineID(LPTAG t);
U16 GetPlaceID(LPTAG t); //PLACEOBJECT, PLACEOBJECT2 (sometimes), REMOVEOBJECT
U16 GetDepth(LPTAG t); //PLACEOBJECT,PLACEOBJECT2,REMOVEOBJECT,REMOVEOBJECT2
char* GetName(LPTAG t); //PLACEOBJECT2, FRAMELABEL
LPMATRIX MatrixJoin(LPMATRIX d,LPMATRIX s1,LPMATRIX s2);
LPMATRIX MatrixMapTriangle(LPMATRIX m,int dx,int dy,
                    int x0,int y0,int x1,int y1,int x2,int y2);


// swfcgi.c

void uncgi();  // same behaviour as Steven Grimm's uncgi-library

#endif
