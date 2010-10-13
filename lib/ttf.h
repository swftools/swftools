/* ttf.h
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

#ifndef __ttf_h__
#define __ttf_h__

#include "types.h"

typedef struct _ttf_table {
    U32 id;
    struct _ttf_table*prev;
    struct _ttf_table*next;

    U8*data;
    int len;
    int memsize;
} ttf_table_t;

typedef struct _table_maxp {
    U16 maxPoints;
    U16 maxContours;
    U16 maxComponentPoints;
    U16 maxComponentContours;
    U16 maxZones;
    U16 maxTwilightPoints;
    U16 maxStorage;
    U16 maxFunctionDefs;
    U16 maxInstructionDefs;
    U16 maxStackElements;
    U16 maxSizeOfInstructions;
    U16 maxComponentElements;
    U16 maxComponentDepth;
} table_maxp_t;

typedef struct _table_os2 {
    S16 xAvgCharWidth;
    U16 usWeightClass;
    U16 usWidthClass;
    U16 ySubscriptXSize;
    U16 ySubscriptYSize;
    U16 ySubscriptXOffset;
    U16 ySubscriptYOffset;
    U16 ySuperscriptXSize;
    U16 ySuperscriptYSize;
    U16 ySuperscriptXOffset;
    U16 ySuperscriptYOffset;
    U16 yStrikeoutSize;
    U16 yStrikeoutPosition;
    U16 sFamilyClass;
    U8 panose_FamilyType;
    U8 panose_SerifStyle;
    U8 panose_Weight;
    U8 panose_Proportion;
    U8 panose_Contrast;
    U8 panose_StrokeVariation;
    U8 panose_ArmStyle;
    U8 panose_Letterform;
    U8 panose_Midline;
    U8 panose_XHeight;
    U32 ulCharRange[4];

    U16 fsSelection;
    U16 fsFirstCharIndex;
    U16 fsLastCharIndex;

    S16 sTypoAscender;
    S16 sTypoDescender;
    S16 sTypoLineGap;
    U16 usWinAscent;
    U16 usWinDescent;

    /* for version >= 0x0001 */
    U32 ulCodePageRange1;
    U32 ulCodePageRange2;
    
    /* for version >= 0x0002 */
    S16 sxHeight;
    S16 sCapHeight;
    U16 usDefaultChar;
    U16 usBreakChar;
    U16 usMaxContext;
} table_os2_t;

typedef struct _table_hea
{
    U16 advanceWidthMax;
    S16 minLeftSideBearing;
    S16 minRightSideBearing;
    S16 xMaxExtent;
    S16 caretSlopeRise;
    S16 caretSlopeRun;
    S16 caretOffset;
} table_hea_t;

#define GLYPH_ON_CURVE 0x01
#define GLYPH_CONTOUR_START 0x40
#define GLYPH_CONTOUR_END 0x80

typedef U32 unicode_t;

typedef struct _ttfpoint {
    int x,y;
    U8 flags;
} ttfpoint_t;
typedef struct _ttfglyph {
    U16 advance;
    S16 bearing;
    S16 xmin,ymin,xmax,ymax;
    int code_size;
    U8*code;
    int num_points;
    ttfpoint_t*points;
} ttfglyph_t;

typedef struct _table_head {
    U16 flags;
    U16 units_per_em;
    S16 xmin,ymin,xmax,ymax;
    U16 macStyle;
    U16 lowest_readable_size;
    S16 dir_hint;
} table_head_t;

typedef struct _table_post {
    U32 italic_angle;
    U16 underline_position;
    U16 underline_thickness;
} table_post_t;

typedef struct _table_cvt {
    S16*values;
    int num;
} table_cvt_t;

typedef struct _table_gasp {
    int num;
    struct {
	U16 size;
	U16 behaviour;
    } *records;
} table_gasp_t;

typedef struct _table_code {
    U8*code;
    int size;
} table_code_t;

typedef struct _ttf {
    char*family_name;     /* nameId 1 */
    char*subfamily_name;  /* nameId 2 */
    char*font_uid;        /* nameId 3 */
    char*full_name;       /* nameId 4 */
    char*version_string;  /* nameId 5 */
    char*postscript_name; /* nameId 6 */

    ttf_table_t*tables;

    table_head_t*head;
    table_maxp_t*maxp;
    table_os2_t*os2;
    table_hea_t*hea;
    table_post_t*post;
    table_cvt_t*cvt;
    table_gasp_t*gasp;
    table_code_t*prep;
    table_code_t*fpgm;

    U16 flags;
    char is_vertical;

    S16 ascent;
    S16 descent; // ymin, *not* negative ymin
    S16 lineGap;

    int num_glyphs;
    ttfglyph_t*glyphs;
    
    int unicode_size;
    unicode_t*unicode;

    U32 version;
} ttf_t;


ttf_t*ttf_new();
ttf_t* ttf_open(const char*filename);
void ttf_reduce(ttf_t*ttf);
ttf_t*ttf_load(void*data, int length);
ttf_table_t*ttf_addtable(ttf_t*ttf, U32 tag);
void ttf_create_truetype_tables(ttf_t*ttf);
void ttf_dump(ttf_t*ttf);
void ttf_destroy(ttf_t*ttf);
void ttf_save(ttf_t*ttf, const char*filename);
void ttf_save_eot(ttf_t*ttf, const char*filename);

#endif
