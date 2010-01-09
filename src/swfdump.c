/* swfdump.c
   Shows the structure of a swf file

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
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

#include "../config.h"

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#else
#undef HAVE_STAT
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#else
#undef HAVE_STAT
#endif

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"

static char * filename = 0;

/* idtab stores the ids which are defined in the file. This allows us
   to detect errors in the file. (i.e. ids which are defined more than 
   once */
static char idtab[65536];
static char * indent = "                ";

static int placements = 0;
static int action = 0;
static int html = 0;
static int xhtml = 0;
static int xy = 0;
static int showtext = 0;
static int showshapes = 0;
static int hex = 0;
static int used = 0;
static int bbox = 0;
static int cumulative = 0;
static int showfonts = 0;
static int showbuttons = 0;

static struct options_t options[] = {
{"h", "help"},
{"D", "full"},
{"V", "version"},
{"e", "html"},
{"E", "xhtml"},
{"a", "action"},
{"t", "text"},
{"s", "shapes"},
{"F", "fonts"},
{"p", "placements"},
{"B", "buttons"},
{"b", "bbox"},
{"X", "width"},
{"Y", "height"},
{"r", "rate"},
{"f", "frames"},
{"d", "hex"},
{"u", "used"},
{0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfdump - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } 
    else if(name[0]=='a') {
        action = 1;
        return 0;
    }
    else if(name[0]=='p') {
        placements = 1;
        return 0;
    }
    else if(name[0]=='t') {
        showtext = 1;
        return 0;
    }
    else if(name[0]=='s') {
        showshapes = 1;
        return 0;
    }
    else if(name[0]=='e') {
        html = 1;
        return 0;
    }
    else if(name[0]=='c') {
        cumulative = 1;
        return 0;
    }
    else if(name[0]=='E') {
        html = 1;
        xhtml = 1;
        return 0;
    }
    else if(name[0]=='X') {
	xy |= 1;
	return 0;
    }
    else if(name[0]=='Y') {
	xy |= 2;
	return 0;
    }
    else if(name[0]=='r') {
	xy |= 4;
	return 0;
    }
    else if(name[0]=='f') {
	xy |= 8;
	return 0;
    }
    else if(name[0]=='F') {
	showfonts = 1;
	return 0;
    }
    else if(name[0]=='d') {
	hex = 1;
	return 0;
    }
    else if(name[0]=='u') {
	used = 1;
	return 0;
    }
    else if(name[0]=='b') {
	bbox = 1;
	return 0;
    }
    else if(name[0]=='B') {
	showbuttons = 1;
	return 0;
    }
    else if(name[0]=='D') {
	showbuttons = action = placements = showtext = showshapes = 1;
	return 0;
    }
    else {
        printf("Unknown option: -%s\n", name);
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
    printf("Usage: %s [-atpdu] file.swf\n", name);
    printf("\n");
    printf("-h , --help                    Print short help message and exit\n");
    printf("-D , --full                    Show everything. Same as -atp\n");
    printf("-V , --version                 Print version info and exit\n");
    printf("-e , --html                    Print out html code for embedding the file\n");
    printf("-E , --xhtml                   Print out xhtml code for embedding the file\n");
    printf("-a , --action                  Disassemble action tags\n");
    printf("-t , --text                    Show text fields (like swfstrings).\n");
    printf("-s , --shapes                  Show shape coordinates/styles\n");
    printf("-F , --fonts                   Show font information\n");
    printf("-p , --placements              Show placement information\n");
    printf("-B , --buttons                 Show button information\n");
    printf("-b , --bbox                    Print tag's bounding boxes\n");
    printf("-X , --width                   Prints out a string of the form \"-X width\".\n");
    printf("-Y , --height                  Prints out a string of the form \"-Y height\".\n");
    printf("-r , --rate                    Prints out a string of the form \"-r rate\".\n");
    printf("-f , --frames                  Prints out a string of the form \"-f framenum\".\n");
    printf("-d , --hex                     Print hex output of tag data, too.\n");
    printf("-u , --used                    Show referred IDs for each Tag.\n");
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

char* what;
char* testfunc(char*str)
{
    printf("%s: %s\n", what, str);
    return 0;
}

void dumpButton2Actions(TAG*tag, char*prefix)
{
    U32 oldTagPos;
    U32 offsetpos;
    U32 condition;

    oldTagPos = swf_GetTagPos(tag);

    // scan DefineButton2 Record
    
    swf_GetU16(tag);          // Character ID
    swf_GetU8(tag);           // Flags;

    offsetpos = swf_GetTagPos(tag);  // first offset
    swf_GetU16(tag);

    while (swf_GetU8(tag))      // state  -> parse ButtonRecord
    { swf_GetU16(tag);          // id
      swf_GetU16(tag);          // layer
      swf_GetMatrix(tag,NULL);  // matrix
      swf_GetCXForm(tag,NULL,1);  // cxform
    }

    while(offsetpos)
    { U8 a;
      ActionTAG*actions;

      if(tag->pos >= tag->len)
	  break;
        
      offsetpos = swf_GetU16(tag);
      condition = swf_GetU16(tag);                // condition
      
      actions = swf_ActionGet(tag);
      printf("%s condition %04x\n", prefix, condition);
      swf_DumpActions(actions, prefix);
    }
    
    swf_SetTagPos(tag,oldTagPos);
    return;
}

void dumpButtonActions(TAG*tag, char*prefix)
{
    ActionTAG*actions;
    swf_SetTagPos(tag, 0);
    swf_GetU16(tag); // id
    while (swf_GetU8(tag))      // state  -> parse ButtonRecord
    { swf_GetU16(tag);          // id
      swf_GetU16(tag);          // layer
      swf_GetMatrix(tag,NULL);  // matrix
    }
    actions = swf_ActionGet(tag);
    swf_DumpActions(actions, prefix);
    swf_ActionFree(actions);
}

void dumpButton(TAG*tag, char*prefix)
{
    swf_SetTagPos(tag, 0);
    swf_GetU16(tag); // id
    while (1) {
        U8 flags = swf_GetU8(tag);
        if(!flags)
            break;
        U16 id = swf_GetU16(tag);
        U16 depth = swf_GetU16(tag);
        char event[80];
        sprintf(event, "%s%s%s%s", 
                (flags&BS_HIT)?"[hit]":"",
                (flags&BS_DOWN)?"[down]":"",
                (flags&BS_OVER)?"[over]":"",
                (flags&BS_UP)?"[up]":"");
        if(flags&0xf0) {
            printf("%s | Show %d at depth %d for %s flags=%02x\n", prefix, id, depth, event, flags);
        } else {
            printf("%s | Show %d at depth %d for %s\n", prefix, id, depth, event);
        }
        swf_GetMatrix(tag,NULL);  // matrix
    }
}

void dumpFont(TAG*tag, char*prefix)
{
    SWFFONT* font = malloc(sizeof(SWFFONT));
    memset(font, 0, sizeof(SWFFONT));
    if(tag->id == ST_DEFINEFONT2 || tag->id == ST_DEFINEFONT3) {
	swf_FontExtract_DefineFont2(0, font, tag);
    } else if(tag->id == ST_DEFINEFONT) {
	swf_FontExtract_DefineFont(0, font, tag);
    } else {
	printf("%sCan't parse %s yet\n", prefix,swf_TagGetName(tag));
    }
    printf("%sID: %d\n", prefix,font->id);
    printf("%sVersion: %d\n", prefix,font->version);
    printf("%sname: %s\n", prefix,font->name);
    printf("%scharacters: %d\n", prefix,font->numchars);
    printf("%shightest mapped unicode value: %d\n", prefix,font->maxascii);
    if(font->layout)
    {
	printf("%sascent:%.2f\n", prefix,font->layout->ascent / 20.0);
	printf("%sdescent:%.2f\n", prefix,font->layout->descent / 20.0);
	printf("%sleading:%.2f\n", prefix,font->layout->leading / 20.0);
	printf("%skerning records:%d\n", prefix,font->layout->kerningcount);
    }
    printf("%sstyle: %d\n", prefix,font->style);
    printf("%sencoding: %02x\n", prefix,font->encoding);
    printf("%slanguage: %02x\n", prefix,font->language);
    int t;
    for(t=0;t<font->numchars;t++) {
	int u = font->glyph2ascii?font->glyph2ascii[t]:-1;
	char ustr[16];
	if(u>=32) sprintf(ustr, " '%c'", u);
	else      sprintf(ustr, " 0x%02x", u);
	printf("%s== Glyph %d: advance=%d encoding=%d%s ==\n", prefix, t, font->glyph[t].advance, u, ustr);
	SHAPE2* shape = swf_ShapeToShape2(font->glyph[t].shape);
	SHAPELINE*line = shape->lines;

	while(line) {
	    if(line->type == moveTo) {
		printf("%smoveTo %.2f %.2f\n", prefix, line->x/20.0, line->y/20.0);
	    } else if(line->type == lineTo) {
		printf("%slineTo %.2f %.2f\n", prefix, line->x/20.0, line->y/20.0);
	    } else if(line->type == splineTo) {
		printf("%ssplineTo (%.2f %.2f) %.2f %.2f\n", prefix,
			line->sx/20.0, line->sy/20.0,
			line->x/20.0, line->y/20.0
			);
	    }
	    line = line->next;
	}
	swf_Shape2Free(shape);
	free(shape);
    }
    
    /*
      not part of the file

    printf("%sencoding table:", prefix, prefix);
    char filled0=0, lastfilled=0;
    for(t=0;t<font->maxascii;t++) {
        if((t&15)==0) {
            printf("\n%s%08x ", prefix, t);
            int s;
            if(!filled0 && t) {
                for(s=t;s<font->maxascii;s++) {
                    if(font->ascii2glyph[s]>=0) break;
                }
                if(s>t+32) {
                    printf("*");
                    t = ((s-16)&~15)-1;
                    continue;
                }
            }
            filled0 = 0;
            for(s=t;s<t+16 && s<font->maxascii;s++) {
                if(font->ascii2glyph[s]>=0) filled0=1;
            }
        }
        printf("%4d ", font->ascii2glyph[t]);
    }
    printf("\n");*/

    swf_FontFree(font);
}

static SWF swf;
static int fontnum = 0;
static SWFFONT**fonts;

void textcallback(void*self, int*glyphs, int*xpos, int nr, int fontid, int fontsize, int startx, int starty, RGBA*color) 
{
    int font=-1,t;
    if(nr<1) 
	return;
    printf("                <%2d glyphs in font %04d size %d, color #%02x%02x%02x%02x at %.2f,%.2f> ",nr, fontid, fontsize, color->r, color->g, color->b, color->a, (startx+xpos[0])/20.0, starty/20.0);
    for(t=0;t<fontnum;t++)
    {
	if(fonts[t]->id == fontid) {
	    font = t;
	    break;
	}
    }

    for(t=0;t<nr;t++)
    {
	unsigned char a; 
	if(font>=0) {
	    if(glyphs[t] >= fonts[font]->numchars  /*glyph is in range*/
		    || !fonts[font]->glyph2ascii /* font has ascii<->glyph mapping */
	      ) a = glyphs[t];
	    else {
		if(fonts[font]->glyph2ascii[glyphs[t]])
		    a = fonts[font]->glyph2ascii[glyphs[t]];
		else
		    a = glyphs[t];
	    }
	} else {
	    a = glyphs[t];
	}
	if(a>=32)
	    printf("%c", a);
	else
	    printf("\\x%x", (int)a);
    }
    printf("\n");
}

void handleText(TAG*tag, char*prefix) 
{
  printf("\n");
  if(placements) {
      swf_SetTagPos(tag, 0);
      swf_GetU16(tag);
      swf_GetRect(tag, 0);
      swf_ResetReadBits(tag);
      MATRIX m;
      swf_GetMatrix(tag, &m);
      printf("%s| Matrix\n",prefix);
      printf("%s| %5.3f %5.3f %6.2f\n", prefix, m.sx/65536.0, m.r1/65536.0, m.tx/20.0);
      printf("%s| %5.3f %5.3f %6.2f\n", prefix, m.r0/65536.0, m.sy/65536.0, m.ty/20.0);
      swf_SetTagPos(tag, 0);
  }
  if(showtext) {
      swf_ParseDefineText(tag,textcallback, 0);
  }
}
	    
void handleDefineSound(TAG*tag)
{
    U16 id = swf_GetU16(tag);
    U8 flags = swf_GetU8(tag);
    int compression = (flags>>4)&7;
    int rate = (flags>>2)&3;
    int bits = flags&2?16:8;
    int stereo = flags&1;
    printf(" (");
    if(compression == 0) printf("Raw ");
    else if(compression == 1) printf("ADPCM ");
    else if(compression == 2) printf("MP3 ");
    else if(compression == 3) printf("Raw little-endian ");
    else if(compression == 6) printf("ASAO ");
    else printf("? ");
    if(rate == 0) printf("5.5Khz ");
    if(rate == 1) printf("11Khz ");
    if(rate == 2) printf("22Khz ");
    if(rate == 3) printf("44Khz ");
    printf("%dBit ", bits);
    if(stereo) printf("stereo");
    else printf("mono");
    printf(")");
}

void handleDefineBits(TAG*tag)
{
    U16 id;
    U8 mode;
    U16 width,height;
    int bpp;
    id = swf_GetU16(tag);
    mode = swf_GetU8(tag);
    width = swf_GetU16(tag);
    height = swf_GetU16(tag);
    printf(" image %dx%d",width,height);
    if(mode == 3) printf(" (8 bpp)");
    else if(mode == 4) printf(" (16 bpp)");
    else if(mode == 5) printf(" (32 bpp)");
    else printf(" (? bpp)");
}

void handleEditText(TAG*tag)
{
    U16 id ;
    U16 flags;
    int t;
    id = swf_GetU16(tag);
    swf_GetRect(tag,0);
    
    //swf_ResetReadBits(tag);

    if (tag->readBit)  
    { tag->pos++; 
      tag->readBit = 0; 
    }
    flags = swf_GetBits(tag,16);
    if(flags & ET_HASFONT) {
	swf_GetU16(tag); //font
	swf_GetU16(tag); //fontheight
    }
    if(flags & ET_HASTEXTCOLOR) {
	swf_GetU8(tag); //rgba
	swf_GetU8(tag);
	swf_GetU8(tag);
	swf_GetU8(tag);
    }
    if(flags & ET_HASMAXLENGTH) {
	swf_GetU16(tag); //maxlength
    }
    if(flags & ET_HASLAYOUT) {
	swf_GetU8(tag); //align
	swf_GetU16(tag); //left margin
	swf_GetU16(tag); //right margin
	swf_GetU16(tag); //indent
	swf_GetU16(tag); //leading
    }
    printf(" variable \"%s\" ", &tag->data[tag->pos]);
    if(flags & ET_HTML) printf("(html)");
    if(flags & ET_NOSELECT) printf("(noselect)");
    if(flags & ET_PASSWORD) printf("(password)");
    if(flags & ET_READONLY) printf("(readonly)");

    if(flags & (ET_X1 | ET_X3 ))
    {
	printf(" undefined flags: %08x (%08x)", (flags&(ET_X1|ET_X3)), flags);
    }
    
    while(tag->data[tag->pos++]);
    if(flags & ET_HASTEXT)
   //  printf(" text \"%s\"\n", &tag->data[tag->pos]) //TODO
	;
}
void printhandlerflags(U32 handlerflags) 
{
    if(handlerflags&1) printf("[on load]");
    if(handlerflags&2) printf("[enter frame]");
    if(handlerflags&4) printf("[unload]");
    if(handlerflags&8) printf("[mouse move]");
    if(handlerflags&16) printf("[mouse down]");
    if(handlerflags&32) printf("[mouse up]");
    if(handlerflags&64) printf("[key down]");
    if(handlerflags&128) printf("[key up]");

    if(handlerflags&256) printf("[data]");
    if(handlerflags&512) printf("[initialize]");
    if(handlerflags&1024) printf("[mouse press]");
    if(handlerflags&2048) printf("[mouse release]");
    if(handlerflags&4096) printf("[mouse release outside]");
    if(handlerflags&8192) printf("[mouse rollover]");
    if(handlerflags&16384) printf("[mouse rollout]");
    if(handlerflags&32768) printf("[mouse drag over]");

    if(handlerflags&0x10000) printf("[mouse drag out]");
    if(handlerflags&0x20000) printf("[key press]");
    if(handlerflags&0x40000) printf("[construct even]");
    if(handlerflags&0xfff80000) printf("[???]");
}
void handleVideoStream(TAG*tag, char*prefix)
{
    U16 id = swf_GetU16(tag);
    U16 frames = swf_GetU16(tag);
    U16 width = swf_GetU16(tag);
    U16 height = swf_GetU16(tag);
    U8 flags = swf_GetU8(tag); //5-2(videopacket 01=off 10=on)-1(smoothing 1=on)
    U8 codec = swf_GetU8(tag);
    printf(" (%d frames, %dx%d", frames, width, height);
    if(flags&1)
	printf(" smoothed");
    if(codec == 2)
	printf(" sorenson h.263)");
    else
	printf(" codec 0x%02x)", codec);
}
void handleVideoFrame(TAG*tag, char*prefix)
{
    U32 code, version, reference, sizeflags;
    U32 width=0, height=0;
    U8 type;
    U16 id = swf_GetU16(tag);
    U16 frame = swf_GetU16(tag);
    U8 deblock,flags, tmp, bit;
    U32 quantizer;
    char*types[] = {"I-frame", "P-frame", "disposable P-frame", "<reserved>"};
    printf(" (frame %d) ", frame);

    /* video packet follows */
    code = swf_GetBits(tag, 17);
    version = swf_GetBits(tag, 5);
    reference = swf_GetBits(tag, 8);

    sizeflags = swf_GetBits(tag, 3);
    switch(sizeflags)
    {
	case 0: width = swf_GetBits(tag, 8); height = swf_GetBits(tag, 8); break;
	case 1: width = swf_GetBits(tag, 16); height = swf_GetBits(tag, 16); break;
	case 2: width = 352; height = 288; break;
	case 3: width = 176; height = 144; break;
	case 4: width = 128; height = 96; break;
	case 5: width = 320; height = 240; break;
	case 6: width = 160; height = 120; break;
	case 7: width = -1; height = -1;/*reserved*/ break;
    }
    printf("%dx%d ", width, height);
    type = swf_GetBits(tag, 2);
    printf("%s", types[type]);

    deblock = swf_GetBits(tag, 1);
    if(deblock)
	printf(" deblock %d ", deblock);
    quantizer = swf_GetBits(tag, 5);
    printf(" quant: %d ", quantizer);
}

void dumpFilter(FILTER*filter)
{
    if(filter->type == FILTERTYPE_BLUR) {
	FILTER_BLUR*f = (FILTER_BLUR*)filter;
	printf("blurx: %f blury: %f\n", f->blurx, f->blury);
	printf("passes: %d\n", f->passes);
    } if(filter->type == FILTERTYPE_GLOW) {
	FILTER_GLOW*f = (FILTER_GLOW*)filter;
	printf("color %02x%02x%02x%02x\n", f->rgba.r,f->rgba.g,f->rgba.b,f->rgba.a);
	printf("blurx: %f blury: %f strength: %f\n", f->blurx, f->blury, f->strength);
	printf("passes: %d\n", f->passes);
	printf("flags: %s%s%s\n", 
		f->knockout?"knockout ":"",
		f->composite?"composite ":"",
		f->innerglow?"innerglow":"");
    } if(filter->type == FILTERTYPE_DROPSHADOW) {
	FILTER_DROPSHADOW*f = (FILTER_DROPSHADOW*)filter;
	printf("blurx: %f blury: %f\n", f->blurx, f->blury);
	printf("passes: %d\n", f->passes);
	printf("angle: %f distance: %f\n", f->angle, f->distance);
	printf("strength: %f passes: %d\n", f->strength, f->passes);
	printf("flags: %s%s%s\n", 
		f->knockout?"knockout ":"",
		f->composite?"composite ":"",
		f->innershadow?"innershadow ":"");
    } if(filter->type == FILTERTYPE_BEVEL) {
	FILTER_BEVEL*f = (FILTER_BEVEL*)filter;
	printf("blurx: %f blury: %f\n", f->blurx, f->blury);
	printf("passes: %d\n", f->passes);
	printf("angle: %f distance: %f\n", f->angle, f->distance);
	printf("strength: %f passes: %d\n", f->strength, f->passes);
	printf("flags: %s%s%s%s\n", 
		f->ontop?"ontop":"",
		f->knockout?"knockout ":"",
		f->composite?"composite ":"",
		f->innershadow?"innershadow ":"");
    } if(filter->type == FILTERTYPE_GRADIENTGLOW) {
	FILTER_GRADIENTGLOW*f = (FILTER_GRADIENTGLOW*)filter;
	swf_DumpGradient(stdout, f->gradient);
	printf("blurx: %f blury: %f\n", f->blurx, f->blury);
	printf("angle: %f distance: %f\n", f->angle, f->distance);
	printf("strength: %f passes: %d\n", f->strength, f->passes);
	printf("flags: %s%s%s%s\n", 
		f->knockout?"knockout ":"",
		f->ontop?"ontop ":"",
		f->composite?"composite ":"",
		f->innershadow?"innershadow ":"");
    }
    rfx_free(filter);
}

void handlePlaceObject23(TAG*tag, char*prefix)
{
    U8 flags,flags2=0;
    MATRIX m;
    CXFORM cx;
    char pstr[3][256];
    int ppos[3] = {0,0,0};
    swf_SetTagPos(tag, 0);
    flags = swf_GetU8(tag);
    if(tag->id == ST_PLACEOBJECT3)
	flags2 = swf_GetU8(tag);
    swf_GetU16(tag); //depth

    //flags&1: move
    if(flags&2) swf_GetU16(tag); //id
    if(flags&4) {
	swf_GetMatrix(tag,&m);
	if(placements) {
	    ppos[0] += sprintf(pstr[0], "| Matrix             ");
	    ppos[1] += sprintf(pstr[1], "| %5.3f %5.3f %6.2f ", m.sx/65536.0, m.r1/65536.0, m.tx/20.0);
	    ppos[2] += sprintf(pstr[2], "| %5.3f %5.3f %6.2f ", m.r0/65536.0, m.sy/65536.0, m.ty/20.0);
	}
    }
    if(flags&8) {
	swf_GetCXForm(tag, &cx, 1);
	if(placements) {
	    ppos[0] += sprintf(pstr[0]+ppos[0], "| CXForm    r    g    b    a ");
	    ppos[1] += sprintf(pstr[1]+ppos[1], "| mul    %4.1f %4.1f %4.1f %4.1f ", cx.r0/256.0, cx.g0/256.0, cx.b0/256.0, cx.a0/256.0);
	    ppos[2] += sprintf(pstr[2]+ppos[2], "| add    %4d %4d %4d %4d ", cx.r1, cx.g1, cx.b1, cx.a1);
	}
    }
    if(flags&16) {
	U16 ratio = swf_GetU16(tag); //ratio
	if(placements) {
	    ppos[0] += sprintf(pstr[0]+ppos[0], "| Ratio ");
	    ppos[1] += sprintf(pstr[1]+ppos[1], "| %-5d ", ratio);
	    ppos[2] += sprintf(pstr[2]+ppos[2], "|       ");
	}
    }
    if(flags&64) {
	U16 clip = swf_GetU16(tag); //clip
	if(placements) {
	    ppos[0] += sprintf(pstr[0]+ppos[0], "| Clip  ");
	    ppos[1] += sprintf(pstr[1]+ppos[1], "| %-4d ", clip);
	    ppos[2] += sprintf(pstr[2]+ppos[2], "|       ");
	}
    }
    if(flags&32) { while(swf_GetU8(tag)); }

    if(flags2&1) { // filter list
	U8 num = swf_GetU8(tag);
	if(placements)
	    printf("\n%d filters\n", num);
	char*filtername[] = {"dropshadow","blur","glow","bevel","gradientglow","convolution","colormatrix","gradientbevel"};
	int t;
	for(t=0;t<num;t++) {
	    FILTER*filter = swf_GetFilter(tag);
	    if(!filter) {
		printf("\n"); 
		return;
	    }
	    if(placements) {
		printf("filter %d: %02x (%s)\n", t, filter->type, (filter->type<sizeof(filtername)/sizeof(filtername[0]))?filtername[filter->type]:"?");
		dumpFilter(filter);
	    }
	}
    }
    if(flags2&2) { // blend mode
	U8 blendmode = swf_GetU8(tag);
	if(placements) {
	    int t;
	    char name[80];
	    sprintf(name, "%-5d", blendmode);
	    for(t=0;blendModeNames[t];t++) {
		if(blendmode==t) {
		    sprintf(name, "%-5s", blendModeNames[t]);
		    break;
		}
	    }
	    ppos[0] += sprintf(pstr[0]+ppos[0], "| Blend ");
	    ppos[1] += sprintf(pstr[1]+ppos[1], "| %s ", name);
	    ppos[2] += sprintf(pstr[2]+ppos[2], "|       ");
	}
    }

    if(placements && ppos[0]) {
	printf("\n");
	printf("%s %s\n", prefix, pstr[0]);
	printf("%s %s\n", prefix, pstr[1]);
	printf("%s %s", prefix, pstr[2]);
    }
    if(flags&128) {
      if (action) {
	U16 reserved;
	U32 globalflags;
	U32 handlerflags;
	char is32 = 0;
	printf("\n");
	reserved = swf_GetU16(tag); // must be 0
	globalflags = swf_GetU16(tag); //TODO: 32 if version>=6
	if(reserved) {
	    printf("Unknown parameter field not zero: %04x\n", reserved);
	    return;
	}
	printf("global flags: %04x\n", globalflags);

	handlerflags = swf_GetU16(tag); //TODO: 32 if version>=6
	if(!handlerflags) {
	    handlerflags = swf_GetU32(tag);
	    is32 = 1;
	}
	while(handlerflags)  {
	    int length;
	    int t;
	    ActionTAG*a;

	    globalflags &= ~handlerflags;
	    printf("%s flags %08x ",prefix, handlerflags);
	    printhandlerflags(handlerflags);
	    length = swf_GetU32(tag);
	    printf(", %d bytes actioncode\n",length);
	    a = swf_ActionGet(tag);
	    swf_DumpActions(a,prefix);
	    swf_ActionFree(a);

	    handlerflags = is32?swf_GetU32(tag):swf_GetU16(tag); //TODO: 32 if version>=6
	}
	if(globalflags) // should go to sterr.
	    printf("ERROR: unsatisfied handlerflags: %02x\n", globalflags);
    } else {
      printf(" has action code\n");
    }
    } else printf("\n");
}

void handlePlaceObject(TAG*tag, char*prefix)
{
    TAG*tag2 = swf_InsertTag(0, ST_PLACEOBJECT2);
    U16 id, depth;
    MATRIX matrix; 
    CXFORM cxform;

    swf_SetTagPos(tag, 0);
    id = swf_GetU16(tag);
    depth = swf_GetU16(tag);
    swf_GetMatrix(tag, &matrix);
    swf_GetCXForm(tag, &cxform, 0);

    swf_SetU8(tag2, 14 /* char, matrix, cxform */);
    swf_SetU16(tag2, depth);
    swf_SetU16(tag2, id);
    swf_SetMatrix(tag2, &matrix);
    swf_SetCXForm(tag2, &cxform, 1);

    handlePlaceObject23(tag2, prefix);
}
char stylebuf[256];
char* fillstyle2str(FILLSTYLE*style)
{
    switch(style->type) {
	case 0x00:
	    sprintf(stylebuf, "SOLID %02x%02x%02x%02x", style->color.r, style->color.g, style->color.b, style->color.a);
	    break;
	case 0x10: case 0x11: case 0x12: case 0x13:
	    sprintf(stylebuf, "GRADIENT (%d steps)", style->gradient.num);
	    break;
	case 0x40: case 0x42:
	    /* TODO: display information about that bitmap */
	    sprintf(stylebuf, "BITMAPt%s %d", (style->type&2)?"n":"", style->id_bitmap);
	    /* TODO: show matrix */
	    //swf_DumpMatrix(stdout, &style->m);
	    break;
	case 0x41: case 0x43:
	    /* TODO: display information about that bitmap */
	    sprintf(stylebuf, "BITMAPc%s %d", (style->type&2)?"n":"", style->id_bitmap);
	    /* TODO: show matrix */
	    //swf_DumpMatrix(stdout, &style->m);
	    break;
	default:
	    sprintf(stylebuf, "UNKNOWN[%02x]",style->type);
    }
    return stylebuf;
}
char* linestyle2str(LINESTYLE*style)
{
    sprintf(stylebuf, "%.2f %02x%02x%02x%02x", style->width/20.0, style->color.r, style->color.g, style->color.b, style->color.a);
    return stylebuf;
}

void handleShape(TAG*tag, char*prefix)
{
    SHAPE2 shape;
    SHAPELINE*line;
    int t,max;

    tag->pos = 0;
    tag->readBit = 0;
    swf_ParseDefineShape(tag, &shape);

    max = shape.numlinestyles > shape.numfillstyles?shape.numlinestyles:shape.numfillstyles;

    if(max) printf("%s | fillstyles(%02d)        linestyles(%02d)\n", 
	    prefix,
	    shape.numfillstyles,
	    shape.numlinestyles
	    );
    else    printf("%s | (Neither line nor fill styles)\n", prefix);

    for(t=0;t<max;t++) {
	printf("%s", prefix);
	if(t < shape.numfillstyles) {
	    printf(" | %-2d) %-18.18s", t+1, fillstyle2str(&shape.fillstyles[t]));
	} else {
	    printf("                         ");
	}
	if(t < shape.numlinestyles) {
	    printf("%-2d) %s", t+1, linestyle2str(&shape.linestyles[t]));
	}
	printf("\n");
        //if(shape.fillstyles[t].type&0x40) {
        //    MATRIX m = shape.fillstyles[t].m;
        //    swf_DumpMatrix(stdout, &m);
        //}
    }

    printf("%s |\n", prefix);

    line = shape.lines;
    while(line) {
	printf("%s | fill: %02d/%02d line:%02d - ",
		prefix, 
		line->fillstyle0,
		line->fillstyle1,
		line->linestyle);
	if(line->type == moveTo) {
	    printf("moveTo %.2f %.2f\n", line->x/20.0, line->y/20.0);
	} else if(line->type == lineTo) {
	    printf("lineTo %.2f %.2f\n", line->x/20.0, line->y/20.0);
	} else if(line->type == splineTo) {
	    printf("splineTo (%.2f %.2f) %.2f %.2f\n", 
		    line->sx/20.0, line->sy/20.0,
		    line->x/20.0, line->y/20.0
		    );
	}
	line = line->next;
    }
    printf("%s |\n", prefix);
}
    
void fontcallback1(void*self, U16 id,U8 * name)
{ fontnum++;
}

void fontcallback2(void*self, U16 id,U8 * name)
{ 
  swf_FontExtract(&swf,id,&fonts[fontnum]);
  fontnum++;
}

static U8 printable(U8 a)
{
    if(a<32 || a==127) return '.';
    else return a;
}
void hexdumpTag(TAG*tag, char* prefix)
{
    int t;
    char ascii[32];
    printf("                %s-=> ",prefix);
    for(t=0;t<tag->len;t++) {
	printf("%02x ", tag->data[t]);
	ascii[t&15] = printable(tag->data[t]);
	if((t && ((t&15)==15)) || (t==tag->len-1))
	{
	    int s,p=((t)&15)+1;
	    ascii[p] = 0;
	    for(s=p-1;s<16;s++) {
		printf("   ");
	    }
	    if(t==tag->len-1)
		printf(" %s\n", ascii);
	    else
		printf(" %s\n                %s-=> ",ascii,prefix);
	}
    }
}

void handleExportAssets(TAG*tag, char* prefix)
{
    int num;
    U16 id;
    char* name;
    int t;
    num = swf_GetU16(tag);
    for(t=0;t<num;t++)
    {
	id = swf_GetU16(tag);
	name = swf_GetString(tag);
	printf("%sexports %04d as \"%s\"\n", prefix, id, name);
    }
}

static void handleFontAlign1(TAG*tag)
{
    swf_SetTagPos(tag, 0);
    U16 id = swf_GetU16(tag);
    U8 flags = swf_GetU8(tag);
    printf(" for font %04d, ", id);
    if((flags&3)==0) printf("thin, ");
    else if((flags&3)==1) printf("medium, ");
    else if((flags&3)==2) printf("thick, ");
    else printf("?, ");
    int num=0;
    while(tag->pos < tag->len) {
	int nr = swf_GetU8(tag); // should be 2
	int t;
	if(nr>2) {
	    printf("*** unsupported multiboxes ***, ");
	    break;
	}
	for(t=0;t<nr;t++) {
	    float v1 = swf_GetF16(tag);
	    float v2 = swf_GetF16(tag);
	}
	U8 xyflags = swf_GetU8(tag);
	num++;
    }
    printf(" %d glyphs", num);
}

#define ALIGN_WITH_GLYPHS
static void handleFontAlign2(TAG*tag, char*prefix)
{
    if(!showfonts)
	return;
    swf_SetTagPos(tag, 0);
    U16 id = swf_GetU16(tag);
    swf_GetU8(tag);
    int num = 0;
#ifdef ALIGN_WITH_GLYPHS
    SWF swf;
    swf.firstTag = tag;
    while(swf.firstTag->prev) swf.firstTag = swf.firstTag->prev;
    SWFFONT* font = 0;
    swf_FontExtract(&swf, id, &font);
#endif
    swf_SetTagPos(tag, 3);
    while(tag->pos < tag->len) {
	printf("%sglyph %d) ", prefix, num);
	int nr = swf_GetU8(tag); // should be 2
	int t;
	for(t=0;t<2;t++) {
	    // pos
	    float v = swf_GetF16(tag);
	    printf("%f ", v*1024.0);
	}
	int s;
	for(s=0;s<nr-1;s++) {
	    for(t=0;t<2;t++) {
		// width
		float v = swf_GetF16(tag);
		printf("+%f ", v*1024.0);
	    }
	}
	U8 xyflags = swf_GetU8(tag);
	printf("xy:%02x\n", xyflags);

#ifdef ALIGN_WITH_GLYPHS
	if(font && num<font->numchars) {
	    SHAPE2* shape = swf_ShapeToShape2(font->glyph[num].shape);
	    SHAPELINE*line = shape->lines;
	    while(line) {
		if(line->type == moveTo) {
		    printf("%smoveTo %.2f %.2f\n", prefix, line->x/20.0, line->y/20.0);
		} else if(line->type == lineTo) {
		    printf("%slineTo %.2f %.2f\n", prefix, line->x/20.0, line->y/20.0);
		} else if(line->type == splineTo) {
		    printf("%ssplineTo (%.2f %.2f) %.2f %.2f\n", prefix,
			    line->sx/20.0, line->sy/20.0,
			    line->x/20.0, line->y/20.0
			    );
		}
		line = line->next;
	    }
	    swf_Shape2Free(shape);
	    free(shape);
	}
	if(num==font->numchars-1) break;
#endif
	num++;
    }
}


void dumperror(const char* format, ...)
{
    char buf[1024];
    va_list arglist;

    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);

    if(!html && !xy)
	printf("==== Error: %s ====\n", buf);
}

static char strbuf[800];
static int bufpos=0;

char* timestring(double f)
{
    int hours   = (int)(f/3600);
    int minutes = (int)((f-hours*3600)/60);
    int seconds = (int)((f-hours*3600-minutes*60));
    int useconds = (int)((f-(int)f)*1000+0.5);
    bufpos+=100;
    bufpos%=800;
    sprintf(&strbuf[bufpos], "%02d:%02d:%02d,%03d",hours,minutes,seconds,useconds);
    return &strbuf[bufpos];
}

int main (int argc,char ** argv)
{ 
    TAG*tag;
#ifdef HAVE_STAT
    struct stat statbuf;
#endif
    int f;
    int xsize,ysize;
    char issprite = 0; // are we inside a sprite definition?
    int spriteframe = 0;
    int mainframe=0;
    char* spriteframelabel = 0;
    char* framelabel = 0;
    char prefix[128];
    int filesize = 0;
    int filepos = 0;
    prefix[0] = 0;
    memset(idtab,0,65536);

    processargs(argc, argv);

    if(!filename)
    {
        fprintf(stderr, "You must supply a filename.\n");
        return 1;
    }

    f = open(filename,O_RDONLY|O_BINARY);
    if (f<0)
    { 
	char buffer[256];
	sprintf(buffer, "Couldn't open %.200s", filename);
        perror(buffer);
        exit(1);
    }
    char header[3];
    read(f, header, 3);
    char compressed = (header[0]=='C');
    char isflash = (header[0]=='F' && header[1] == 'W' && header[2] == 'S') ||
                   (header[0]=='C' && header[1] == 'W' && header[2] == 'S');
    close(f);

    int fl=strlen(filename);
    if(!isflash && fl>3 && !strcmp(&filename[fl-4], ".abc")) {
        swf_ReadABCfile(filename, &swf);
    } else {
        f = open(filename,O_RDONLY|O_BINARY);
        if FAILED(swf_ReadSWF(f,&swf))
        { 
            fprintf(stderr, "%s is not a valid SWF file or contains errors.\n",filename);
            close(f);
            exit(1);
        }

#ifdef HAVE_STAT
        fstat(f, &statbuf);
        if(statbuf.st_size != swf.fileSize && !compressed)
            dumperror("Real Filesize (%d) doesn't match header Filesize (%d)",
                    statbuf.st_size, swf.fileSize);
        filesize = statbuf.st_size;
#endif
        close(f);
    }

    //if(action && swf.fileVersion>=9) {
    //    fprintf(stderr, "Actionscript parsing (-a) not yet supported for SWF versions>=9\n");
    //    action = 0;
    //}

    xsize = (swf.movieSize.xmax-swf.movieSize.xmin)/20;
    ysize = (swf.movieSize.ymax-swf.movieSize.ymin)/20;
    if(xy)
    {
	if(xy&1)
	printf("-X %d", xsize);

	if((xy&1) && (xy&6))
	printf(" ");

	if(xy&2)
	printf("-Y %d", ysize);
	
	if((xy&3) && (xy&4))
	printf(" ");

	if(xy&4)
	printf("-r %.2f", swf.frameRate/256.0);
	
	if((xy&7) && (xy&8))
	printf(" ");
	
	if(xy&8)
	printf("-f %d", swf.frameCount);
	
	printf("\n");
	return 0;
    }
    if(html)
    {
	char*fileversions[] = {"","1,0,0,0", "2,0,0,0","3,0,0,0","4,0,0,0",
			       "5,0,0,0","6,0,23,0","7,0,0,0","8,0,0,0","9,0,0,0","10,0,0,0", "11,0,0,0", "12,0,0,0"};
	if(swf.fileVersion>10) {
	    fprintf(stderr, "Fileversion>10\n");
	    exit(1);
	}

	if(xhtml) {
	    printf("<object type=\"application/x-shockwave-flash\" data=\"%s\" width=\"%d\" height=\"%d\">\n"
			    "<param name=\"movie\" value=\"%s\"/>\n"
			    "<param name=\"play\" value=\"true\"/>\n"
			    "<param name=\"loop\" value=\"false\"/>\n"
			    "<param name=\"quality\" value=\"high\"/>\n"
			    "<param name=\"loop\" value=\"false\"/>\n"
			    "</object>\n\n", filename, xsize, ysize, filename);
	} else {
	    printf("<OBJECT CLASSID=\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\"\n"
		   " WIDTH=\"%d\"\n"
		   //" BGCOLOR=#ffffffff\n"?
		   " HEIGHT=\"%d\"\n"
		   //http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,23,0?
		   " CODEBASE=\"http://active.macromedia.com/flash5/cabs/swflash.cab#version=%s\">\n"
		   "  <PARAM NAME=\"MOVIE\" VALUE=\"%s\">\n"
		   "  <PARAM NAME=\"PLAY\" VALUE=\"true\">\n" 
		   "  <PARAM NAME=\"LOOP\" VALUE=\"true\">\n"
		   "  <PARAM NAME=\"QUALITY\" VALUE=\"high\">\n"
		   "  <PARAM NAME=\"ALLOWSCRIPTACCESS\" VALUE=\"always\">\n"
		   "  <EMBED SRC=\"%s\" WIDTH=\"%d\" HEIGHT=\"%d\"\n" //bgcolor=#ffffff?
		   "   PLAY=\"true\" ALIGN=\"\" LOOP=\"true\" QUALITY=\"high\"\n"
		   "   TYPE=\"application/x-shockwave-flash\"\n"
                   "   ALLOWSCRIPTACCESS=\"always\"\n"
		   "   PLUGINSPAGE=\"http://www.macromedia.com/go/getflashplayer\">\n"
		   "  </EMBED>\n" 
		   "</OBJECT>\n", xsize, ysize, fileversions[swf.fileVersion], 
				  filename, filename, xsize, ysize);
	}
	return 0;
    } 
    printf("[HEADER]        File version: %d\n", swf.fileVersion);
    if(compressed) {
	printf("[HEADER]        File is zlib compressed.");
	if(filesize && swf.fileSize)
	    printf(" Ratio: %02d%%\n", filesize*100/(swf.fileSize));
	else
	    printf("\n");
    }
    printf("[HEADER]        File size: %d%s\n", swf.fileSize, swf.compressed?" (Depacked)":"");
    printf("[HEADER]        Frame rate: %f\n",swf.frameRate/256.0);
    printf("[HEADER]        Frame count: %d\n",swf.frameCount);
    printf("[HEADER]        Movie width: %.2f",(swf.movieSize.xmax-swf.movieSize.xmin)/20.0);
    if(swf.movieSize.xmin)
	printf(" (left offset: %.2f)\n", swf.movieSize.xmin/20.0);
    else 
	printf("\n");
    printf("[HEADER]        Movie height: %.2f",(swf.movieSize.ymax-swf.movieSize.ymin)/20.0);
    if(swf.movieSize.ymin)
	printf(" (top offset: %.2f)\n", swf.movieSize.ymin/20.0);
    else 
	printf("\n");

    tag = swf.firstTag;
   
    if(showtext) {
	fontnum = 0;
	swf_FontEnumerate(&swf,&fontcallback1, 0);
	fonts = (SWFFONT**)malloc(fontnum*sizeof(SWFFONT*));
	fontnum = 0;
	swf_FontEnumerate(&swf,&fontcallback2, 0);
    }

    while(tag) {
        char*name = swf_TagGetName(tag);
        char myprefix[128];
        if(!name) {
            dumperror("Unknown tag:0x%03x", tag->id);
            //tag = tag->next;
            //continue;
        }
	if(!name) {
	    name = "UNKNOWN TAG";
	}
	if(cumulative) {
	    filepos += tag->len;
	    printf("[%03x] %9d %9d %s%s", tag->id, tag->len, filepos, prefix, swf_TagGetName(tag));
	} else {
	    printf("[%03x] %9d %s%s", tag->id, tag->len, prefix, swf_TagGetName(tag));
	}
	
        if(tag->id == ST_PLACEOBJECT) {
            printf(" places id %04d at depth %04x", swf_GetPlaceID(tag), swf_GetDepth(tag));
            if(swf_GetName(tag))
                printf(" name \"%s\"",swf_GetName(tag));
        }
	else if(tag->id == ST_PLACEOBJECT2 || tag->id == ST_PLACEOBJECT3) {
	    if(tag->data[0]&1)
		printf(" moves");
	    else
		printf(" places");
	    
	    if(tag->data[0]&2)
		printf(" id %04d",swf_GetPlaceID(tag));
	    else
		printf(" object");

            printf(" at depth %04d", swf_GetDepth(tag));

	    if(tag->id == ST_PLACEOBJECT3 && tag->data[1]&4)
		printf(" as bitmap");
	   
	    swf_SetTagPos(tag, 0);
	    if(tag->data[0]&64) {
		SWFPLACEOBJECT po;
		swf_GetPlaceObject(tag, &po);
		printf(" (clip to %04d)", po.clipdepth);
		swf_PlaceObjectFree(&po);
	    }
            if(swf_GetName(tag))
                printf(" name \"%s\"",swf_GetName(tag));

	}
        else if(tag->id == ST_REMOVEOBJECT) {
            printf(" removes id %04d from depth %04d", swf_GetPlaceID(tag), swf_GetDepth(tag));
        }
        else if(tag->id == ST_REMOVEOBJECT2) {
            printf(" removes object from depth %04d", swf_GetDepth(tag));
        }
        else if(tag->id == ST_FREECHARACTER) {
            printf(" frees object %04d", swf_GetPlaceID(tag));
        }
        else if(tag->id == ST_FILEATTRIBUTES) {
            swf_SetTagPos(tag, 0);
            U32 flags = swf_GetU32(tag);
            if(flags&FILEATTRIBUTE_USENETWORK) printf(" usenetwork");
            if(flags&FILEATTRIBUTE_AS3) printf(" as3");
            if(flags&FILEATTRIBUTE_SYMBOLCLASS) printf(" symbolclass");
            if(flags&FILEATTRIBUTE_USEHARDWAREGPU) printf(" hardware-gpu");
            if(flags&FILEATTRIBUTE_USEACCELERATEDBLIT) printf(" accelerated-blit");
            if(flags&~(1|8|16|32|64))
                printf(" flags=%02x", flags);
        }
        else if(tag->id == ST_DOABC) {
            swf_SetTagPos(tag, 0);
            U32 flags = swf_GetU32(tag);
            char*s = swf_GetString(tag);
            if(flags&~1) {
                printf(" flags=%08x", flags);
            }
            if(*s) {
                printf(" \"%s\"", s);
            }
            if(flags&1) {
                if(name)
                    printf(",");
                printf(" lazy load");
            }
            swf_SetTagPos(tag, 0);
        }
	else if(tag->id == ST_STARTSOUND) {
	    U8 flags;
	    U16 id;
	    id = swf_GetU16(tag);
	    flags = swf_GetU8(tag);
	    if(flags & 32)
		printf(" stops sound with id %04d", id);
	    else
		printf(" starts sound with id %04d", id);
	    if(flags & 16)
		printf(" (if not already playing)");
	    if(flags & 1)
		swf_GetU32(tag);
	    if(flags & 2)
		swf_GetU32(tag);
	    if(flags & 4) {
		printf(" looping %d times", swf_GetU16(tag));
	    }
	}
	else if(tag->id == ST_FRAMELABEL) {
	    int l = strlen((char*)tag->data);
	    printf(" \"%s\"", tag->data);
	    if((l+1) < tag->len) {
		printf(" has %d extra bytes", tag->len-1-l);
		if(tag ->len - (l+1) == 1 && tag->data[tag->len-1] == 1)
		    printf(" (ANCHOR)");
	    }
	    if((framelabel && !issprite) ||
	       (spriteframelabel && issprite)) {
		dumperror("Frame %d has more than one label", 
			issprite?spriteframe:mainframe);
	    }
	    if(issprite) spriteframelabel = (char*)tag->data;
	    else framelabel = (char*)tag->data;
	}
	else if(tag->id == ST_SHOWFRAME) {
	    char*label = issprite?spriteframelabel:framelabel;
	    int frame = issprite?spriteframe:mainframe;
	    int nframe = frame;
	    if(!label) {
		while(tag->next && tag->next->id == ST_SHOWFRAME && tag->next->len == 0) {
		    tag = tag->next;
		    if(issprite) spriteframe++;
		    else mainframe++;
		    nframe++;
		}
	    }
	    if(nframe == frame)
		printf(" %d (%s)", frame+1, timestring(frame*(256.0/(swf.frameRate+0.1))));
	    else
		printf(" %d-%d (%s-%s)", frame+1, nframe+1,
			timestring(frame*(256.0/(swf.frameRate+0.1))),
			timestring(nframe*(256.0/(swf.frameRate+0.1)))
			);
	    if(label)
		printf(" (label \"%s\")", label);
	    if(issprite) {spriteframe++; spriteframelabel = 0;}
	    if(!issprite) {mainframe++; framelabel = 0;}
	}
        else if(tag->id == ST_SETBACKGROUNDCOLOR) {
	    U8 r = swf_GetU8(tag);
	    U8 g = swf_GetU8(tag);
	    U8 b = swf_GetU8(tag);
	    printf(" (%02x/%02x/%02x)",r,g,b);
	}
	else if(tag->id == ST_PROTECT) {
	    if(tag->len>0) {
		printf(" %s", swf_GetString(tag));
	    }
	}
	else if(tag->id == ST_DEFINEFONTALIGNZONES) {
	    handleFontAlign1(tag);
	}
	else if(tag->id == ST_CSMTEXTSETTINGS) {
	    U16 id = swf_GetU16(tag);
	    U8 flags = swf_GetU8(tag);
	    printf(" (");
	    if(flags&0x40) {
		printf("flashtype,");
	    }
	    switch(((flags>>3)&7)) {
		case 0:printf("no grid,");break;
		case 1:printf("pixel grid,");break;
		case 2:printf("subpixel grid,");break;
		case 3:printf("unknown grid,");break;
	    }
	    if(flags&0x87) 
		printf("unknown[%08x],", flags);
	    float thickness = swf_GetFixed(tag);
	    float sharpness = swf_GetFixed(tag);
	    printf("s=%.2f,t=%.2f)", thickness, sharpness);
	    swf_GetU8(tag);
	}
	else if(swf_isDefiningTag(tag)) {
            U16 id = swf_GetDefineID(tag);
            printf(" defines id %04d", id);
            if(idtab[id])
                dumperror("Id %04d is defined more than once.", id);
            idtab[id] = 1;
        }
	else if(swf_isPseudoDefiningTag(tag)) {
            U16 id = swf_GetDefineID(tag);
            printf(" adds information to id %04d", id);
            if(!idtab[id])
                dumperror("Id %04d is not yet defined.\n", id);
	}

	if(tag->id == ST_DEFINEBITSLOSSLESS ||
	   tag->id == ST_DEFINEBITSLOSSLESS2) {
	    handleDefineBits(tag);
	    printf("\n");
	}
	else if(tag->id == ST_DEFINESOUND) {
	    handleDefineSound(tag);
	    printf("\n");
	}
	else if(tag->id == ST_VIDEOFRAME) {
	    handleVideoFrame(tag, myprefix);
	    printf("\n");
	}
	else if(tag->id == ST_DEFINEVIDEOSTREAM) {
	    handleVideoStream(tag, myprefix);
	    printf("\n");
	}
	else if(tag->id == ST_DEFINEEDITTEXT) {
	    handleEditText(tag);
	    printf("\n");
	}
	else if(tag->id == ST_DEFINEMOVIE) {
	    U16 id = swf_GetU16(tag);
	    char*s = swf_GetString(tag);
	    printf(" URL: %s\n", s);
	}
	else if(tag->id == ST_DEFINETEXT || tag->id == ST_DEFINETEXT2) {
	    handleText(tag, myprefix);
	}
	else if(tag->id == ST_DEFINESCALINGGRID) {
	    U16 id = swf_GetU16(tag);
	    SRECT r;
	    swf_GetRect(tag, &r);
	    printf(" (%.2f,%.2f)-(%.2f,%.2f)\n", r.xmin/20.0, r.ymin/20.0, r.xmax/20.0, r.ymax/20.0);
	}
	else if(tag->id == ST_PLACEOBJECT2 || tag->id == ST_PLACEOBJECT3) {
	}
	else if(tag->id == ST_NAMECHARACTER || tag->id==ST_DEFINEFONTNAME) {
	    swf_GetU16(tag);
	    printf(" \"%s\"\n", swf_GetString(tag));
	}
	else {
	    printf("\n");
	}

	if(bbox && swf_isDefiningTag(tag) && tag->id != ST_DEFINESPRITE) {
	    SRECT r = swf_GetDefineBBox(tag);
	    printf("                %s bbox [%.2f, %.2f, %.2f, %.2f]\n", prefix,
		    r.xmin/20.0,
		    r.ymin/20.0,
		    r.xmax/20.0,
		    r.ymax/20.0);
	}
        
        sprintf(myprefix, "                %s", prefix);

        if(tag->id == ST_DEFINESPRITE) {
            sprintf(prefix, "         ");
	    if(issprite) {
		dumperror("Sprite definition inside a sprite definition");
	    }
	    issprite = 1;
	    spriteframe = 0;
	    spriteframelabel = 0;
        }
        else if(tag->id == ST_END) {
            *prefix = 0;
	    issprite = 0;
	    spriteframelabel = 0;
	    if(tag->len)
		dumperror("End Tag not empty");
        }
	else if(tag->id == ST_EXPORTASSETS || tag->id == ST_SYMBOLCLASS) {
	    handleExportAssets(tag, myprefix);
	}
        else if(tag->id == ST_DOACTION && action) {
            ActionTAG*actions;
            actions = swf_ActionGet(tag);
            swf_DumpActions(actions, myprefix);
        }
        else if((tag->id == ST_DOABC || tag->id == ST_RAWABC) && action) {
            void*abccode = swf_ReadABC(tag);
            swf_DumpABC(stdout, abccode, "");
            swf_FreeABC(abccode);
        }
        else if(tag->id == ST_DOINITACTION && action) {
            ActionTAG*actions;
            swf_GetU16(tag); // id
            actions = swf_ActionGet(tag);
            swf_DumpActions(actions, myprefix);
        }
	else if(tag->id == ST_DEFINEBUTTON) {
            if(showbuttons) {
                dumpButton(tag, myprefix);
            }
            if(action) {
	        dumpButtonActions(tag, myprefix);
            }
	}
	else if(swf_isFontTag(tag) && showfonts) {
	    dumpFont(tag, myprefix);
	}
	else if(tag->id == ST_DEFINEBUTTON2) {
            if(action) {
	        dumpButton2Actions(tag, myprefix);
            }
	}
	else if(tag->id == ST_PLACEOBJECT) {
	    handlePlaceObject(tag, myprefix);
	}
	else if(tag->id == ST_PLACEOBJECT2 || tag->id == ST_PLACEOBJECT3) {
	    handlePlaceObject23(tag, myprefix);
	}
	else if(tag->id == ST_DEFINEFONTALIGNZONES) {
	    handleFontAlign2(tag, myprefix);
	}
	else if(tag->id == ST_DEFINEFONTNAME) {
	    swf_SetTagPos(tag, 0);
	    swf_GetU16(tag); //id
	    swf_GetString(tag); //name
	    char* copyright = swf_GetString(tag);
	    printf("%s%s\n", myprefix, copyright);
	}
	else if(tag->id == ST_DEFINESHAPE ||
		tag->id == ST_DEFINESHAPE2 ||
		tag->id == ST_DEFINESHAPE3 ||
		tag->id == ST_DEFINESHAPE4) {
	    if(showshapes)
		handleShape(tag, myprefix);
	}

	if(tag->len && used) {
	    int num = swf_GetNumUsedIDs(tag);
	    int* used;
	    int t;
	    if(num) {
		used = (int*)malloc(sizeof(int)*num);
		swf_GetUsedIDs(tag, used);
		printf("%s%suses IDs: ", indent, prefix);
		for(t=0;t<num;t++) {
		    int id;
		    swf_SetTagPos(tag, used[t]);
		    id = swf_GetU16(tag);
		    printf("%d%s", id, t<num-1?", ":"");
		    if(!idtab[id]) {
			dumperror("Id %04d is not yet defined.\n", id);
		    }
		}
		printf("\n");
	    }
	}
	
	if(tag->id == ST_FREECHARACTER) {
	    U16 id;
	    swf_SetTagPos(tag, 0);
	    id = swf_GetU16(tag);
	    idtab[id] = 0;
	}

	if(tag->len && hex) {
	    hexdumpTag(tag, prefix);
	}
        tag = tag->next;
	fflush(stdout);
    }

    swf_FreeTags(&swf);
    return 0;
}


