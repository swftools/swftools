/* flash.h
   Header file for flash.c

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __flash_h__
#define __flash_h__

#include "types.h"

#define TAGID_END 0
#define TAGID_SHOWFRAME 1
#define TAGID_DEFINESHAPE 2
#define TAGID_PLACEOBJECT 4
#define TAGID_REMOVEOBJECT 5
#define TAGID_DEFINEBITS 6
#define TAGID_DEFINEBUTTON 7
#define TAGID_JPEGTABLES 8
#define TAGID_BACKGROUNDCOLOR 9
#define TAGID_DEFINEFONT 10
#define TAGID_DEFINETEXT 11
#define TAGID_DOACTION 12
#define TAGID_DEFINEFONTINFO 13
#define TAGID_DEFINESOUND 14
#define TAGID_STARTSOUND 15
#define TAGID_DEFINEBUTTONSOUND 17
#define TAGID_SOUNDSTREAMHEAD 18
#define TAGID_SOUNDSTREAMHEAD2 18
#define TAGID_SOUNDSTREAMBLOCK 19
#define TAGID_DEFINEBITSLOSSLESS 20
#define TAGID_DEFINEBITSJPEG2 21
#define TAGID_DEFINESHAPE2 22
#define TAGID_DEFINEBUTTONCXFORM 23
#define TAGID_PROTECT 24
#define TAGID_PLACEOBJECT2 26
#define TAGID_REMOVEOBJECT2 28
#define TAGID_DEFINESHAPE3 32
#define TAGID_DEFINETEXT2 33
#define TAGID_DEFINEBUTTON2 34
#define TAGID_DEFINEBITSJPEG3 35
#define TAGID_DEFINEBITSLOSSLESS2 36
#define TAGID_DEFINEEDITTEXT 37
#define TAGID_DEFINEMOVIE 38
#define TAGID_DEFINESPRITE 39
#define TAGID_FRAMELABEL 43
#define TAGID_DEFINEMORPHSHAPE 46
#define TAGID_DEFINEFONT2 48
#define TAGID_EXPORTASSETS   56
#define TAGID_IMPORTASSETS   57
#define TAGID_ENABLEDEBUGGER 58

#include "bitio.h"

struct swf_tag;
struct RECT;
struct RGB;
struct MATRIX;
struct CXFORM;
struct CLIPACTIONS;

struct swf_tag
{
    u8 id;
    u32 length;
    u8*data;
    u32 fulllength; // includes id
    u8*fulldata; // likewise
};

struct RGB
{
    byte r,g,b;
};

struct RGBA
{
    byte r,g,b,a;
};

struct RECT
{
    int x1, y1, x2, y2;
};

struct MATRIX 
{
    u8 hasscale;
    u8 hasrotate;
    float a[2][2];
    int b[2];
};

struct CXFORM 
{
    u8 hasadd;
    u8 hasmult;
    float rmult, gmult, bmult, amult;
    float radd, gadd, badd, aadd;
    u8 alpha;
};

struct GRADIENT
{
    int num;
    u8 ratios[8];
    struct RGB rgb[8];
    struct RGBA rgba[8];
};

struct RECT readRECT();
struct RGB readRGB();
struct MATRIX readMATRIX();
unsigned char* readSTRING();
struct CLIPACTIONS readCLIPACTIONS();

void writeRECT(u8**pos, struct RECT*r);

void swf_init(uchar*newdata, int newlength);
struct flash_header
{
    int version;
    u32 length;
    struct RECT boundingBox;
    u16 rate;
    u16 count;

    u8*headerdata;
    int headerlength;
};

struct swffile
{
    struct flash_header header;
    int tagnum;
    struct swf_tag* tags;
};

struct flash_header swf_read_header();
struct RGB readRGB();
struct RGBA readRGBA();
struct GRADIENT readGRADIENT(int shape);
struct RECT readRECT();
struct CXFORM readCXFORM(char alpha);
struct MATRIX readMATRIX();
unsigned char* readSTRING();
int swf_read_tag(struct swf_tag* swftag);
int swf_count_tags();


struct PlaceObject
{
    u16 id;
    u16 depth;
    struct MATRIX matrix;
    struct CXFORM cxform;
    u8 hascxform;
};

struct PlaceObject2
{
    u8 reserved;
    u8 hasclipactions;
    u8 hasname;
    u8 hasratio;
    u8 hascolortransform;
    u8 hasmatrix;
    u8 hascharacter;
    u8 hasmove;

    u16 depth;
    int id;
    struct MATRIX matrix;
    struct CXFORM cxform;
    u16 ratio;
    char * name;
    u16 clipactions;
};


void placeobject_init (struct PlaceObject*obj, struct swf_tag*tag);
void placeobject_write (struct PlaceObject*obj, struct writer_t*w);

void placeobject2_init (struct PlaceObject2*obj, struct swf_tag*tag);

void read_swf(struct swffile*swf, uchar*data, int length);

int getidfromtag(struct swf_tag* tag);
void setidintag(struct swf_tag* tag, int id);
char is_sprite_tag (int id);
char is_defining_tag (int id);
struct swf_tag* duptag(struct swf_tag*tag);

#endif //__flash_h__
