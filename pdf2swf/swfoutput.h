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

#include <t1lib.h>
#ifdef T1ERR_SCAN_ENCODING //t1lib 5.0.0
#define T1_Get_no_fonts T1_GetNoFonts
#endif

extern "C" {
#include "../lib/rfxswf.h"
}

extern int enablezlib; //default:0
extern int opennewwindow; //default:0
extern int ignoredraworder; //default:0
extern int drawonlyshapes; //default:0
extern int jpegquality; //default:100;
extern int storeallcharacters; // default:0
extern int insertstoptag; //default:0
extern int flashversion; //default:4
extern int splinemaxerror; //default:1
extern int fontsplinemaxerror; //default:1

typedef long int twip;

struct swfmatrix {
    double m11,m12,m21,m22,m13,m23;
};

struct swfcoord {
    twip x;
    twip y;
};

class SWFFont
{
    T1_OUTLINE**outline;
    char**charname;
    int*width;
    char*used;

    char*name;
    int charnum;

    U16*char2swfcharid;
    U16*swfcharid2char;
    int swfcharpos;

    char**standardtable;
    int standardtablesize;

    public:
    
    int t1id;
    char*fontid;
    unsigned int swfid;

    SWFFont(char*name, int t1id, char*filename);
    SWFFont::~SWFFont();
    T1_OUTLINE*getOutline(char*charname, int charnr);
    int getSWFCharID(char*name, int charnr);
    int getWidth(char*name);
    char*getName();
    char*getCharName(int t);
    int getCharWidth(int t) {return width[t];}
};

struct swfoutput 
{
    //int t1font;
    double fontm11,fontm12,fontm21,fontm22;
    unsigned short int linewidth;
    SWFFont*font;
    RGBA strokergb;
    RGBA fillrgb;
};

#define DRAWMODE_STROKE 1
#define DRAWMODE_FILL 2
#define DRAWMODE_EOFILL 3
#define DRAWMODE_CLIP 4
#define DRAWMODE_EOCLIP 5

void swfoutput_init(struct swfoutput*, char*filename, int sizex, int sizey);
void swfoutput_setprotected(); //write PROTECT tag

void swfoutput_newpage(struct swfoutput*);

void swfoutput_setfont(struct swfoutput*, char*fontid, int t1font, char*filename);
int swfoutput_queryfont(struct swfoutput*, char*fontid);
void swfoutput_setdrawmode(struct swfoutput*, int drawmode);
void swfoutput_setfillcolor(struct swfoutput*, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void swfoutput_setstrokecolor(struct swfoutput*, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void swfoutput_setfontmatrix(struct swfoutput*,double,double,double,double);
void swfoutput_setlinewidth(struct swfoutput*, double linewidth);

void swfoutput_drawchar(struct swfoutput*,double x,double y,char*a, int charnr);
void swfoutput_drawpath(struct swfoutput*, T1_OUTLINE*outline, struct swfmatrix*m);
#define LINE_CAP_BUTT 0
#define LINE_CAP_ROUND 1 
#define LINE_CAP_SQUARE 2
#define LINE_JOIN_MITER 0 
#define LINE_JOIN_ROUND 1
#define LINE_JOIN_BEVEL 2
void swfoutput_drawpath2poly(struct swfoutput*, T1_OUTLINE*outline, struct swfmatrix*m, int line_join, int line_cap, double line_width, double miter_limit);
void swfoutput_startclip(struct swfoutput*, T1_OUTLINE*outline, struct swfmatrix*m);
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
