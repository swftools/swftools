/* hexfont.c

   Example for how to construct an SWF font from another SWF font.
   
   Part of the swftools package.

   Copyright (c) 2004 Matthias Kramm <kramm@quiss.org>
 
   This file is distributed under the GPL, see file COPYING for details 

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

#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "../rfxswf.h"

static void write_font(SWFFONT * font, char *filename)
{
    SWF swf;
    TAG *t;
    SRECT r;
    RGBA rgb;
    int f;
    int useDefineFont2 = 1;
    int storeGlyphNames = 1;
   
#define WRITEFONTID 8888
    font->id = WRITEFONTID;

    memset(&swf, 0x00, sizeof(SWF));

    swf.fileVersion = 9;
    swf.frameRate = 0x4000;

    t = swf_InsertTag(NULL, ST_SETBACKGROUNDCOLOR);
    swf.firstTag = t;
    rgb.r = 0xef;
    rgb.g = 0xef;
    rgb.b = 0xff;
    swf_SetRGB(t, &rgb);
    
    t = swf_InsertTag(t, ST_DEFINEFONT3);
    swf_FontSetDefine2(t, font);

    t = swf_InsertTag(t, ST_DEFINEFONTALIGNZONES);
    swf_SetU16(t, font->id);
    swf_SetU8(t, 0); //thin
    int i;
    for(i=0;i<256;i++) {
	swf_SetU8(t, 2);
	swf_SetF16(t, 82.0 / 1024.0);
	swf_SetF16(t, 82.0 / 1024.0);
	swf_SetF16(t, ((i%16/2)*82.0) / 1024.0);
	swf_SetF16(t, ((i/16/2)*82.0) / 1024.0);
	/*
	if(i<128 && (i&15)<8) {
	    swf_SetF16(t, 0.0);
	    swf_SetF16(t, 0.0);
	    swf_SetF16(t, 640.0 / 1024.0);
	    swf_SetF16(t, 640.0 / 1024.0);
	} else if(i<128 && (i&15)>=8) {
	    swf_SetF16(t, 0.0 / 1024.0);
	    swf_SetF16(t, 0.0   / 1024.0);
	    swf_SetF16(t, 330.0 / 1024.0);
	    swf_SetF16(t, 640.0 / 1024.0);
	} else if(i>=128 && (i&15)<8) {
	    swf_SetF16(t, 0.0   / 1024.0);
	    swf_SetF16(t, 0.0 / 1024.0);
	    swf_SetF16(t, 640.0 / 1024.0);
	    swf_SetF16(t, 330.0 / 1024.0);
	} else {
	    swf_SetF16(t, 0.0 / 1024.0);
	    swf_SetF16(t, 0.0 / 1024.0);
	    swf_SetF16(t, 330.0 / 1024.0);
	    swf_SetF16(t, 330.0 / 1024.0);
	}*/
	swf_SetU8(t, 3); // x and y
    }

    int s;
    int xmax = 0;
    int ymax = 0;
    int ypos = 1;
    U8 gbits, abits;
    int x, y, c;
    int range = font->maxascii;

    c = 0;
    range = 256;

    xmax = 1280;
    ymax = 1280*20;

    swf.movieSize.xmax = xmax * 20;
    swf.movieSize.ymax = ymax;

    t = swf_InsertTag(t, ST_DEFINETEXT);
    swf_SetU16(t, font->id + 1);	// ID
    r.xmin = 0;
    r.ymin = 0;
    r.xmax = swf.movieSize.xmax;
    r.ymax = swf.movieSize.ymax;
    swf_SetRect(t, &r);
    swf_SetMatrix(t, NULL);
    abits = swf_CountBits(xmax * 16, 0);
    gbits = 8;
    swf_SetU8(t, gbits);
    swf_SetU8(t, abits);

    rgb.r = 0x00;
    rgb.g = 0x00;
    rgb.b = 0x00;
    ypos = 2;
    
    int textscale = 1024;
    for (y = 0; y < ((range + 15) / 16); y++) {
	for (x = 0; x < 16; x++) {
	    //swf_TextSetInfoRecord(t, font, textscale, &rgb, x*64*20+64*20+10+(x+y)*20, y*64*20+128*20+10+(x^y)*20);
	    swf_TextSetInfoRecord(t, font, textscale, &rgb, x*64*20+64*20+10, y*64*20+128*20+10);
	    int g = y * 16 + x;
	    swf_SetU8(t, 1);
	    swf_SetBits(t, g, gbits);
	    swf_SetBits(t, 0, abits);
	    swf_ResetWriteBits(t);
	}
    }
    swf_SetU8(t, 0);
		 
    t = swf_InsertTag(t, ST_CSMTEXTSETTINGS);
    swf_SetU16(t, font->id + 1);
    swf_SetU8(t, (1<<3)//grid
	         |0x40//flashtype
		 );
    swf_SetU32(t, 0x20000);//thickness
    swf_SetU32(t, 0x800000);//sharpness
    swf_SetU8(t, 0);//reserved

    t = swf_InsertTag(t, ST_PLACEOBJECT2);
    swf_ObjectPlace(t, font->id + 1, 1, NULL, NULL, NULL);

    t = swf_InsertTag(t, ST_SHOWFRAME);
    t = swf_InsertTag(t, ST_END);

    f = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0644);
    if FAILED
	(swf_WriteSWF(f, &swf)) fprintf(stderr, "WriteSWF() failed in writeFont().\n");
    close(f);

    swf_FreeTags(&swf);
}


int main()
{
    SWFFONT hexfont;

    memset(&hexfont, 0, sizeof(hexfont));
    hexfont.name = (U8*)"HexFont";
    hexfont.layout = malloc(sizeof(SWFLAYOUT));
    hexfont.numchars = 256;
    hexfont.maxascii = 256;
    hexfont.encoding = 32;
    hexfont.glyph2ascii = malloc(sizeof(U16)*256);
    hexfont.ascii2glyph = malloc(sizeof(int)*256);
    hexfont.glyph = malloc(sizeof(SWFGLYPH)*256);
    hexfont.glyphnames = malloc(sizeof(char*)*256);
    hexfont.layout->bounds = malloc(sizeof(SRECT)*256);
    hexfont.layout->kerningcount = 0;
    hexfont.layout->kerning = 0;
    int t;
    int ymax =-0x7fffffff;
    int ymin = 0x7fffffff;
    for(t=0;t<256;t++) 
    {
	drawer_t _draw,*draw=&_draw;
	swf_Shape01DrawerInit(draw, 0);
	int x,y;
	FPOINT d;
	int rx = 0;//t&15;
	int ry = 0;//(t&15)^0x05;
#define S {d.x*=20*4+2;d.y*=20*4+2;}
	if(1) {
	    for(x=0;x<8;x++) 
	    for(y=0;y<8;y++) {
		if((x^y)&1) {
		    d.x=x;d.y=-y;     S;draw->moveTo(draw, &d);
		    d.x=x+1;d.y=-y;   S;draw->lineTo(draw, &d);
		    d.x=x+1;d.y=-y-1; S;draw->lineTo(draw, &d);
		    d.x=x;d.y=-y-1;   S;draw->lineTo(draw, &d);
		    d.x=x;d.y=-y;     S;draw->lineTo(draw, &d);
		}
	    }
	} else {
	    d.x=0;d.y=-0;   S;draw->moveTo(draw, &d);
	    d.x=0;d.y=-8;   S;draw->lineTo(draw, &d);
	    d.x=8;d.y=-8;   S;draw->lineTo(draw, &d);
	    d.x=8;d.y=-0;   S;draw->lineTo(draw, &d);
	    d.x=0;d.y=-0;   S;draw->lineTo(draw, &d);

	    d.x=1;d.y=-1;   S;draw->moveTo(draw, &d);
	    d.x=7;d.y=-1;   S;draw->lineTo(draw, &d);
	    d.x=7;d.y=-7;   S;draw->lineTo(draw, &d);
	    d.x=1;d.y=-7;   S;draw->lineTo(draw, &d);
	    d.x=1;d.y=-1;   S;draw->lineTo(draw, &d);
	}
	draw->finish(draw);
	hexfont.glyph[t].shape = swf_ShapeDrawerToShape(draw);
	hexfont.layout->bounds[t] = swf_ShapeDrawerGetBBox(draw);
	hexfont.glyph[t].advance = hexfont.layout->bounds[t].xmax + hexfont.layout->bounds[t].xmin;
	draw->dealloc(draw);

	hexfont.glyph2ascii[t] = t;
	hexfont.ascii2glyph[t] = t;
	hexfont.glyphnames[t] = 0;
	if(hexfont.layout->bounds[t].ymax > ymax)
	    ymax = hexfont.layout->bounds[t].ymax;
	if(hexfont.layout->bounds[t].ymin < ymin)
	    ymin = hexfont.layout->bounds[t].ymin;

	if(t>=0xe4) {
	    /* breaks flashtype, but not (non-flashtype) definefont2 */
	    //hexfont.glyph2ascii[t] = 0;
	}
    }
    hexfont.layout->ascent = ymin<0?-ymin:0;
    hexfont.layout->descent = ymax>0?ymax:0;
    hexfont.layout->leading = hexfont.layout->ascent + hexfont.layout->descent;

    write_font(&hexfont, "alignzones.swf");
    return 0;
}

