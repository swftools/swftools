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

int main()
{
    char* hex = "0123456789ABCDEF";
    SWFFONT* font = swf_LoadFont("Courier.swf");
    SWFFONT hexfont;

    memset(&hexfont, 0, sizeof(hexfont));
    hexfont.name = "HexFont";
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
	char nr[5];
	nr[0] = '[';
	nr[1] = hex[t/16];
	nr[2] = hex[t&15];
	nr[3] = ']';
	nr[4] = 0;
	drawer_t draw;
	swf_Shape01DrawerInit(&draw, 0);
	swf_DrawText(&draw, font, (int)(10*20), nr);
	draw.finish(&draw);
	hexfont.glyph[t].shape = swf_ShapeDrawerToShape(&draw);
	hexfont.layout->bounds[t] = swf_ShapeDrawerGetBBox(&draw);
	hexfont.glyph[t].advance = hexfont.layout->bounds[t].xmax + hexfont.layout->bounds[t].xmin;
	draw.dealloc(&draw);
	hexfont.glyph2ascii[t] = t;
	hexfont.ascii2glyph[t] = t;
	hexfont.glyphnames[t] = strdup(nr);
	if(hexfont.layout->bounds[t].ymax > ymax)
	    ymax = hexfont.layout->bounds[t].ymax;
	if(hexfont.layout->bounds[t].ymin < ymin)
	    ymin = hexfont.layout->bounds[t].ymin;
	if(t>=0xe4) {
	    hexfont.glyph2ascii[t] = 0;
	}
    }

    hexfont.layout->ascent = ymin<0?-ymin:0;
    hexfont.layout->descent = ymax>0?ymax:0;
    hexfont.layout->leading = hexfont.layout->ascent + hexfont.layout->descent;

    swf_WriteFont(&hexfont, "hexfont.swf");
    return 0;
}

