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
    void*internal;
};

/* outline definition, adapted from t1lib.h */

/* A fractional point */
typedef struct {
  long x;
  long y;
} SWF_PATHPOINT;

/* A straight outline segment, stroked or not stroked */
typedef struct pathsegment {  
  char type;                /* type of segment (line or move) */
  unsigned char flag;       /* type1 rasterizer internal stuff */
  short references;         /* type1 rasterizer internal stuff */
  unsigned char size;       /* size of the structure */
  unsigned char context;    /* index to device context */
  struct pathsegment *link; /* pointer to next structure in linked list */
  struct pathsegment *last; /* pointer to last structure in list */
  SWF_PATHPOINT    dest;     /* relative ending location of path segment */
} SWF_PATHSEGMENT;

/* A third order bezier segment */
typedef struct bezierpathsegment {
  char type;                /* type of segment (bezier) */
  unsigned char flag;     /* type1 rasterizer internal stuff */
  short references;       /* type1 rasterizer internal stuff */
  unsigned char size;     /* as with any 'segment' type */
  unsigned char context;  /* as with any 'segment' type */
  SWF_PATHSEGMENT *link;   /* as with any 'segment' type */
  SWF_PATHSEGMENT *last;   /* as with any 'segment' type */
  SWF_PATHPOINT    dest;   /* ending point (D) */
  SWF_PATHPOINT    B;      /* control point B */
  SWF_PATHPOINT    C;      /* control point C */
} SWF_BEZIERSEGMENT;

typedef SWF_PATHSEGMENT  SWF_OUTLINE;

#define   SWF_PATHTYPE_LINE           0x10
#define   SWF_PATHTYPE_BEZIER         0x12
#define   SWF_PATHTYPE_MOVE           0x15


#define DRAWMODE_STROKE 1
#define DRAWMODE_FILL 2
#define DRAWMODE_EOFILL 3
#define DRAWMODE_CLIP 4
#define DRAWMODE_EOCLIP 5

void swfoutput_setparameter(char*name, char*value);

void swfoutput_init(struct swfoutput*);
void swfoutput_save(struct swfoutput*, char*filename);
void swfoutput_newpage(struct swfoutput*, int pageNum, int movex, int movey, int x1, int y1, int x2, int y2);
void swfoutput_setfont(struct swfoutput*, char*fontid, char*filename);
int swfoutput_queryfont(struct swfoutput*, char*fontid);
int getCharID(SWFFONT *font, int charnr, char *charname, int u);

void swfoutput_setdrawmode(struct swfoutput*, int drawmode);
void swfoutput_setfillcolor(struct swfoutput*, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void swfoutput_setstrokecolor(struct swfoutput*, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void swfoutput_setfontmatrix(struct swfoutput*,double,double,double,double);
void swfoutput_setlinewidth(struct swfoutput*, double linewidth);

int swfoutput_drawchar(struct swfoutput*,double x,double y,char*a, int charnr, int u);
void swfoutput_drawpath(struct swfoutput*, SWF_OUTLINE*outline, struct swfmatrix*m);
#define LINE_CAP_BUTT 0
#define LINE_CAP_ROUND 1 
#define LINE_CAP_SQUARE 2
#define LINE_JOIN_MITER 0 
#define LINE_JOIN_ROUND 1
#define LINE_JOIN_BEVEL 2
void swfoutput_drawpath2poly(struct swfoutput*, SWF_OUTLINE*outline, struct swfmatrix*m, int line_join, int line_cap, double line_width, double miter_limit);
void swfoutput_startclip(struct swfoutput*, SWF_OUTLINE*outline, struct swfmatrix*m);
void swfoutput_endclip(struct swfoutput*);
int swfoutput_drawimagejpeg(struct swfoutput*, RGBA*pic, int sizex,int sizey, 
	double x1,double y1,
	double x2,double y2,
	double x3,double y3,
	double x4,double y4);
int swfoutput_drawimagelossless(struct swfoutput*, RGBA*pic, int sizex, int sizey,
	double x1,double y1,
	double x2,double y2,
	double x3,double y3,
	double x4,double y4);
int swfoutput_drawimagelosslessN(struct swfoutput*, U8*pic, RGBA*pal, int sizex, int sizey,
	double x1,double y1,
	double x2,double y2,
	double x3,double y3,
	double x4,double y4, int n);
void swfoutput_drawimageagain(struct swfoutput*, int id, int sizex, int sizey,
	double x1,double y1,
	double x2,double y2,
	double x3,double y3,
	double x4,double y4);

void swfoutput_linktopage(struct swfoutput*, int page, swfcoord*points);
void swfoutput_linktourl(struct swfoutput*, char*url, swfcoord*points);
void swfoutput_namedlink(struct swfoutput*obj, char*name, swfcoord*points);

void swfoutput_destroy(struct swfoutput*);

#endif //__swfoutput_h__
