/* swfoutput.h
   Header file for swfoutput.cc (and swfoutput_x11.cc).

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __swfoutput_h__
#define __swfoutput_h__

#include <t1lib.h>
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

class SWFFont
{
    T1_OUTLINE**outline;
    char**charname;
    char*used;

    char*name;
    int charnum;

    public:
    
    int t1id;
    char*fontid;
    unsigned int swfid;

    SWFFont(char*name, int t1id, char*filename);
    SWFFont::~SWFFont();
    T1_OUTLINE*getOutline(char*charname);
    char*getName();
    char*getCharName(int t);
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

void swfoutput_drawchar(struct swfoutput*,double x,double y,char*a);
void swfoutput_drawpath(struct swfoutput*, T1_OUTLINE*outline, struct swfmatrix*m);
void swfoutput_startclip(struct swfoutput*, T1_OUTLINE*outline, struct swfmatrix*m);
void swfoutput_endclip(struct swfoutput*);
void swfoutput_drawimagefile(struct swfoutput*, char*filename, int sizex,int sizey, 
	double x1,double y1,
	double x2,double y2,
	double x3,double y3,
	double x4,double y4);

void swfoutput_destroy(struct swfoutput*);

#endif //__swfoutput_h__
