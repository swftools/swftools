/* swfoutput.h
   Header file for swfoutput.cc (and swfoutput_x11.cc).

   Part of the swftools package.

   Copyright (c) 2001,2002,2003 Matthias Kramm <kramm@quiss.org> 
 
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

#ifndef __swfoutput_h__
#define __swfoutput_h__

extern "C" {
#include "../lib/rfxswf.h"
#include "../lib/gfxdevice.h"
}
typedef long int twip;

struct swfmatrix {
    double m11,m12,m21,m22,m13,m23;
};

struct swfcoord {
    twip x;
    twip y;
};

struct swfoutput 
{
    MATRIX fontmatrix;
    double fontm11,fontm12,fontm21,fontm22;
    SWFFONT *swffont;
    RGBA strokergb;
    RGBA fillrgb;
    int drawmode;
    int x1,y1,x2,y2;
    void*internal;
};

#define DRAWMODE_STROKE 1
#define DRAWMODE_FILL 2
#define DRAWMODE_EOFILL 3
#define DRAWMODE_CLIP 4
#define DRAWMODE_EOCLIP 5

void swfoutput_setparameter(char*name, char*value);

void swfoutput_init(struct swfoutput*);
int swfoutput_save(struct swfoutput*, char*filename);
SWF* swfoutput_get(struct swfoutput*);
void swfoutput_getdimensions(struct swfoutput*, int*x1, int*y1, int*x2, int*y2);
    
void swfoutput_pagefeed(struct swfoutput*obj);
void swfoutput_newpage(struct swfoutput*, int pageNum, int movex, int movey, int x1, int y1, int x2, int y2);

void swfoutput_setfont(struct swfoutput*, char*fontid, char*filename);
int swfoutput_queryfont(struct swfoutput*, char*fontid);
int getCharID(SWFFONT *font, int charnr, char *charname, int u);
void swfoutput_setfontmatrix(struct swfoutput*,double,double,double,double);
int swfoutput_drawchar(struct swfoutput*,double x,double y,char*a, int charnr, int u, gfxcolor_t* col);

void swfoutput_drawgfxline(struct swfoutput*, gfxline_t*line, gfxcoord_t width, gfxcolor_t*col, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit);
void swfoutput_fillgfxline(struct swfoutput*, gfxline_t*line, gfxcolor_t*col);
void swfoutput_startclip(struct swfoutput*, gfxline_t*line);
void swfoutput_endclip(struct swfoutput*);

void swfoutput_drawimagejpeg(struct swfoutput*, RGBA*pic, int sizex,int sizey, 
	double x1,double y1,
	double x2,double y2,
	double x3,double y3,
	double x4,double y4);
void swfoutput_drawimagelossless(struct swfoutput*, RGBA*pic, int sizex, int sizey,
	double x1,double y1,
	double x2,double y2,
	double x3,double y3,
	double x4,double y4);

void swfoutput_linktopage(struct swfoutput*, int page, swfcoord*points);
void swfoutput_linktourl(struct swfoutput*, char*url, swfcoord*points);
void swfoutput_namedlink(struct swfoutput*obj, char*name, swfcoord*points);

void swfoutput_destroy(struct swfoutput*);

#endif //__swfoutput_h__
