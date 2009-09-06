/* swfstrings.c
   Scans a swf file for strings

   Part of the swftools package.
   
   Copyright (c) 2000,2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
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
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "../lib/utf8.h"

static char * filename = 0;
static char showfonts = 0;
static int x=0,y=0,w=0,h=0;

static struct options_t options[] = {
{"f", "fonts"},
{"x", "xpos"},
{"y", "ypos"},
{"W", "width"},
{"H", "height"},
{"V", "version"},
{0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfstrings - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } else if(!strcmp(name, "x")) {
	x = atoi(val);
	return 1;
    } else if(!strcmp(name, "y")) {
	y = atoi(val);
	return 1;
    } else if(!strcmp(name, "W")) {
	w = atoi(val);
	return 1;
    } else if(!strcmp(name, "H")) {
	h = atoi(val);
	return 1;
    } else if(!strcmp(name, "f")) {
	showfonts = 1;
	return 0;
    } else if(!strcmp(name, "V")) {
        printf("swfstrings - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } else {
	fprintf(stderr, "Unknown option: -%s\n", name);
	exit(1);
    }
    return 0;
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char *name)
{
    printf("\n");
    printf("Usage: %s [options] file.swf\n", name);
    printf("\n");
    printf("-f , --fonts                   Print out font information for each text block\n");
    printf("-x , --xpos <x>                Set bounding box x coordinate\n");
    printf("-y , --ypos <y>                Set bounding box y coordinate\n");
    printf("-W , --width <width>           Set bounding box width\n");
    printf("-H , --height <height>         Set bounding box height\n");
    printf("-V , --version                 Print version information and exit\n");
    printf("\n");
}
int args_callback_command(char*name,char*val)
{
    if(filename) {
        fprintf(stderr, "Only one file allowed. You supplied at least two. (%s and %s)\n",
                 filename, name);
    }
    filename = name;
    return 0;
}

static SWF swf;
static int fontnum = 0;
static SWFFONT**fonts = 0;

void fontcallback1(void*self, U16 id,U8 * name)
{ fontnum++;
}

void fontcallback2(void*self, U16 id,U8 * name)
{ 
  swf_FontExtract(&swf,id,&fonts[fontnum]);
  fontnum++;
}


void textcallback(void*self, int*glyphs, int*advance, int nr, int fontid, int fontsize, int startx, int starty, RGBA*color) 
{
    SWFFONT*font = 0;
    int t;
    for(t=0;t<fontnum;t++)
    {
	if(fonts[t]->id == fontid) {
	    font = fonts[t];
	    break;
	}
    }

    if(showfonts) {
	if(font)
	    printf("#<font %d \"%s\"%s%s>\n", fontid, font->name, swf_FontIsBold(font)?" bold":"",swf_FontIsItalic(font)?" italic":"");
	printf("#<color #%02x%02x%02x%02x>\n", color->r, color->g, color->b, color->a);
	printf("#<size %d>\n", fontsize);
    }

    for(t=0;t<nr;t++)
    {
	int xx = startx + advance[t];
	int yy = starty;
	MATRIX*m = (MATRIX*)self;
	
	SPOINT p = {xx,yy};
	p = swf_TurnPoint(p, m);
	xx = p.x / 20;
	yy = p.y / 20;

	if(x|y|w|h) {
	    if(xx < x || yy < y || xx > x+w || yy > y+h) {
		/* outside of bounding box */
		///printf("(%d+%d,%d) -> (%d,%d)\n", startx, advance[t]/20, starty, xx, yy);
		if(t==nr-1) return;
		else continue;
	    }
	}

	unsigned char a; 
	int advance = 0;
	if(font) {
	    if(glyphs[t]<0 || glyphs[t] >= font->numchars  /*glyph is not in range*/
		    || !font->glyph2ascii /* font has ascii<->glyph mapping */
	      ) a = glyphs[t];
	    else {
		if(font->glyph2ascii[glyphs[t]])
		    a = font->glyph2ascii[glyphs[t]];
		else
		    a = glyphs[t];
	    }
	} else {
	    a = glyphs[t];
	}

	if(a>=32) {
	    char* utf8 = getUTF8(a);
	    printf("%s", utf8);
	} else {
	    printf("\\x%x", (int)a);
	}
    }
    printf("\n");
}

void fontcallback(void*self,U16 id,U8 * name)
{ SWFFONT* font;
  TAG* t;
  
  swf_FontExtract(&swf,id,&font);

  t = swf.firstTag;

  swf_FontFree(font);
}

TAG**id2tag = 0;

int main (int argc,char ** argv)
{ 
    int f;
    processargs(argc, argv);
    if(!filename)
	exit(0);

    f = open(filename,O_RDONLY|O_BINARY);
    if (f<0 || swf_ReadSWF(f,&swf)<0) {
	fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",filename);
	if(f>=0) close(f);
	exit(-1);
    }
    close(f);
    
    if(x|y|w|h) {
	if(!w) w = (swf.movieSize.xmax - swf.movieSize.xmin) / 20;
	if(!h) h = (swf.movieSize.ymax - swf.movieSize.ymin) / 20;
    }

    id2tag = malloc(sizeof(TAG)*65536);

    fontnum = 0;
    swf_FontEnumerate(&swf,&fontcallback1, 0);
    fonts = (SWFFONT**)malloc(fontnum*sizeof(SWFFONT*));
    fontnum = 0;
    swf_FontEnumerate(&swf,&fontcallback2, 0);
 
    TAG*tag = swf.firstTag;
    while (tag)
    { 
	if(swf_isTextTag(tag)) {
	    id2tag[swf_GetDefineID(tag)] = tag;
	} else if(swf_isPlaceTag(tag)) {
	    SWFPLACEOBJECT po;
	    swf_SetTagPos(tag, 0);
	    swf_GetPlaceObject(tag, &po);
	    if(!po.move && id2tag[po.id]) {
		TAG*text = id2tag[po.id];
		swf_SetTagPos(text, 0);
		swf_GetU16(text);
		swf_GetRect(text, NULL);
		swf_ResetReadBits(text);
		MATRIX m,tm;
		swf_GetMatrix(text, &tm);
		swf_MatrixJoin(&m, &po.matrix, &tm);
		swf_ParseDefineText(text, textcallback, &m);
	    }
	}
	tag = tag->next;
    }
  
    swf_FreeTags(&swf);
    return 0;
}

