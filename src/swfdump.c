/* swfdump.c
   Shows the structure of a swf file

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#define HAVE_STAT

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
#include "../lib/rfxswf.h"
#include "../lib/args.h"

char * filename = 0;

/* idtab stores the ids which are defined in the file. This allows us
   to detect errors in the file. (i.e. ids which are defined more than 
   once */
char idtab[65536];

int action = 0;
int html = 0;
int xy = 0;
int showtext = 0;
int hex = 0;

struct options_t options[] =
{
 {"a","action"},
 {"t","text"},
 {"X","width"},
 {"Y","height"},
 {"r","rate"},
 {"e","html"},
 {"v","verbose"},
 {"V","version"},
 {"d","hex"},
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
    else if(name[0]=='t') {
        showtext = 1;
        return 0;
    }
    else if(name[0]=='e') {
        html = 1;
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
    else if(name[0]=='d') {
	hex = 1;
	return 0;
    }
    else {
        printf("Unknown option: -%s\n", name);
    }

    return 0;
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char*name)
{    
    printf("Usage: %s [-at] file.swf\n", name);
    printf("\t-h , --help\t\t Print help and exit\n");
    printf("\t-e , --html\t\t Create a html embedding the file (simple, but useful)\n");
    printf("\t-X , --width\t\t Prints out a string of the form \"-X width\"\n");
    printf("\t-Y , --height\t\t Prints out a string of the form \"-Y height\"\n");
    printf("\t-r , --rate\t\t Prints out a string of the form \"-r rate\"\n");
    printf("\t-a , --action\t\t Disassemble action tags\n");
    printf("\t-t , --text\t\t Show text data\n");
    printf("\t-d , --hex\t\t Print hex output of tag data, too\n");
    printf("\t-V , --version\t\t Print program version and exit\n");
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
    swf_GetU16(tag); // id
    while (swf_GetU8(tag))      // state  -> parse ButtonRecord
    { swf_GetU16(tag);          // id
      swf_GetU16(tag);          // layer
      swf_GetMatrix(tag,NULL);  // matrix
    }
    actions = swf_ActionGet(tag);
    swf_DumpActions(actions, prefix);
}

#define ET_HASTEXT 32768
#define ET_WORDWRAP 16384
#define ET_MULTILINE 8192
#define ET_PASSWORD 4096
#define ET_READONLY 2048
#define ET_HASTEXTCOLOR 1024
#define ET_HASMAXLENGTH 512
#define ET_HASFONT 256
#define ET_X3 128
#define ET_X2 64
#define ET_HASLAYOUT 32
#define ET_NOSELECT 16
#define ET_BORDER 8
#define ET_X1 4
#define ET_X0 2
#define ET_USEOUTLINES 1

SWF swf;
int fontnum = 0;
SWFFONT**fonts;

void textcallback(int*glyphs, int nr, int fontid) 
{
    int font=-1,t;
    printf("                <%2d glyphs in font %2d> ",nr, fontid);
    for(t=0;t<fontnum;t++)
    {
	if(fonts[t]->id == fontid) {
	    font = t;
	    break;
	}
    }
    if(font<0) {
	printf("\n");
	return; // todo: should we report this? (may only be that it's a definefont without fontinfo)
    }

    for(t=0;t<nr;t++)
    {
	unsigned char a; 
	if(glyphs[t] >= fonts[font]->numchars)
	    continue;
	a = fonts[font]->glyph2ascii[glyphs[t]];
	if(a>=32)
	    printf("%c", a);
	else
	    printf("\\x%x", (int)a);
    }
    printf("\n");
}

void handleText(TAG*tag) 
{
  printf("\n");
  swf_FontExtract_DefineTextCallback(-1,0,tag,4, textcallback);
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
    printf(" variable \"%s\"", &tag->data[tag->pos]);

    if(flags & (ET_X1 | ET_X2 | ET_X3 | ET_X0))
    {
	printf(" undefined flags: %d%d%d%d", 
		(flags&ET_X0?1:0),
		(flags&ET_X1?1:0),
		(flags&ET_X2?1:0),
		(flags&ET_X3?1:0));
    }
    
    while(tag->data[tag->pos++]);
    if(flags & ET_HASTEXT)
   //  printf(" text \"%s\"\n", &tag->data[tag->pos])
	;
}
void printhandlerflags(U16 handlerflags) 
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
    if(handlerflags&0xfe00) printf("[???]");
}
void handlePlaceObject2(TAG*tag, char*prefix)
{
    U8 flags = swf_GetU8(tag);
    swf_GetU16(tag); //depth
    //flags&1: move
    if(flags&2) swf_GetU16(tag); //id
    if(flags&4) swf_GetMatrix(tag,0);
    if(flags&8) swf_GetCXForm(tag,0,0);
    if(flags&16) swf_GetU16(tag); //ratio
    if(flags&32) { 
	while(swf_GetU8(tag));
    }
    if(flags&64) swf_GetU16(tag); //clip
    if(flags&128) {
      if (action) {
	U16 globalflags;
	U16 unknown;
	printf("\n");
	unknown = swf_GetU16(tag);
	globalflags = swf_GetU16(tag);
	if(unknown)
	    printf("Unknown parameter field not zero: %04x\n", unknown);
	while(1)  {
	    int length;
	    int t;
	    U16 handlerflags;
	    ActionTAG*a;
	    handlerflags = swf_GetU16(tag);
	    if(!handlerflags)
		break;
	    globalflags &= ~handlerflags;
	    printf("%s flags %04x ",prefix, handlerflags);
	    printhandlerflags(handlerflags);

	    length = swf_GetU32(tag);
	    printf(", %d bytes actioncode\n",length);
	    a = swf_ActionGet(tag);
	    swf_DumpActions(a,prefix);
	    swf_ActionFree(a);
	}
	if(globalflags) // should go to sterr.
	    printf("ERROR: unsatisfied handlerflags: %02x\n", globalflags);
    } else {
      printf(" has action code\n");
    }
    } else printf("\n");
}
    
void fontcallback1(U16 id,U8 * name)
{ fontnum++;
}

void fontcallback2(U16 id,U8 * name)
{ swf_FontExtract(&swf,id,&fonts[fontnum]);
  fontnum++;
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
    int spriteframe;
    int mainframe=0;
    char* spriteframelabel;
    char* framelabel = 0;
    char prefix[128];
    prefix[0] = 0;
    memset(idtab,0,65536);

    processargs(argc, argv);

    if(!filename)
    {
        fprintf(stderr, "You must supply a filename.\n");
        return 1;
    }

    f = open(filename,O_RDONLY);

    if (f<0)
    { 
        perror("Couldn't open file: ");
        exit(1);
    }
    if FAILED(swf_ReadSWF(f,&swf))
    { 
        fprintf(stderr, "%s is not a valid SWF file or contains errors.\n",filename);
        close(f);
        exit(1);
    }

#ifdef HAVE_STAT
    fstat(f, &statbuf);
    if(statbuf.st_size != swf.FileSize)
        fprintf(stderr, "Error: Real Filesize (%d) doesn't match header Filesize (%d)",
                statbuf.st_size, swf.FileSize);
#endif

    close(f);

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
	printf("-r %d", swf.frameRate*100/256);
	
	printf("\n");
	return 0;
    }
    if(html)
    {
 	printf("<OBJECT CLASSID=\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\"\n"
	       " WIDTH=\"%d\"\n"
	       " HEIGHT=\"%d\"\n"
	       " CODEBASE=\"http://active.macromedia.com/flash5/cabs/swflash.cab#version=%d,0,0,0\">\n"
               "  <PARAM NAME=\"MOVIE\" VALUE=\"%s\">\n"
	       "  <PARAM NAME=\"PLAY\" VALUE=\"true\">\n" 
	       "  <PARAM NAME=\"LOOP\" VALUE=\"true\">\n"
	       "  <PARAM NAME=\"QUALITY\" VALUE=\"high\">\n"
	       "  <EMBED SRC=\"%s\" WIDTH=\"%d\" HEIGHT=\"%d\"\n"
	       "   PLAY=\"true\" LOOP=\"true\" QUALITY=\"high\"\n"
	       "   TYPE=\"application/x-shockwave-flash\"\n"
	       "   PLUGINSPAGE=\"http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash\">\n"
               "  </EMBED>\n" 
	       "</OBJECT>\n", xsize, ysize, swf.fileVersion, filename, filename, xsize, ysize);
	return 0;
    } 
    printf("[HEADER]        File version: %d\n", swf.fileVersion);
    printf("[HEADER]        File size: %ld\n", swf.fileSize);
    printf("[HEADER]        Frame rate: %f\n",swf.frameRate/256.0);
    printf("[HEADER]        Frame count: %d\n",swf.frameCount);
    printf("[HEADER]        Movie width: %.3f\n",(swf.movieSize.xmax-swf.movieSize.xmin)/20.0);
    printf("[HEADER]        Movie height: %.3f\n",(swf.movieSize.ymax-swf.movieSize.ymin)/20.0);

    tag = swf.firstTag;
   
    if(showtext) {
	fontnum = 0;
	swf_FontEnumerate(&swf,&fontcallback1);
	fonts = (SWFFONT**)malloc(fontnum*sizeof(SWFFONT*));
	fontnum = 0;
	swf_FontEnumerate(&swf,&fontcallback2);
    }

    while(tag) {
        char*name = swf_TagGetName(tag);
        char myprefix[128];
        if(!name) {
            fprintf(stderr, "Error: Unknown tag:0x%03x\n", tag->id);
            tag = tag->next;
            continue;
        }
        printf("[%03x] %9ld %s%s", tag->id, tag->len, prefix, swf_TagGetName(tag));

        if(swf_isDefiningTag(tag)) {
            U16 id = swf_GetDefineID(tag);
            printf(" defines id %04x", id);
            if(idtab[id])
                fprintf(stderr, "Error: Id %04x is defined more than once.\n", id);
            idtab[id] = 1;
        }
	else if(swf_isPseudoDefiningTag(tag)) {
            U16 id = swf_GetDefineID(tag);
            printf(" adds information to id %04x", id);
            if(!idtab[id])
                fprintf(stderr, "Error: Id %04x is not yet defined.\n", id);
	}
        else if(tag->id == ST_PLACEOBJECT || 
                tag->id == ST_PLACEOBJECT2) {
            printf(" places id %04x at depth %04x", swf_GetPlaceID(tag), swf_GetDepth(tag));
            if(swf_GetName(tag))
                printf(" name \"%s\"",swf_GetName(tag));
        }
        else if(tag->id == ST_REMOVEOBJECT) {
            printf(" removes id %04x from depth %04x", swf_GetPlaceID(tag), swf_GetDepth(tag));
        }
        else if(tag->id == ST_REMOVEOBJECT2) {
            printf(" removes object from depth %04x", swf_GetDepth(tag));
        }
	else if(tag->id == ST_FRAMELABEL) {
	    printf(" \"%s\"", tag->data);
	    if(framelabel) {
		fprintf(stderr, "Error: Frame %d has more than one label\n", 
			issprite?spriteframe:mainframe);
	    }
	    if(issprite) spriteframelabel = tag->data;
	    else framelabel = tag->data;
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
		printf(" %d", frame);
	    else
		printf(" %d-%d", frame, nframe);
	    if(label)
		printf(" (label \"%s\")", label);
	    if(issprite) {spriteframe++; spriteframelabel = 0;}
	    if(!issprite) {mainframe++; framelabel = 0;}
	}

	if(tag->id == ST_DEFINEEDITTEXT) {
	    handleEditText(tag);
	    printf("\n");
	}
	else if(tag->id == ST_DEFINETEXT || tag->id == ST_DEFINETEXT2) {
	    if(showtext)
		handleText(tag);
	    else
		printf("\n");
	}
	else if(tag->id == ST_PLACEOBJECT2) {
	}
	else {
	    printf("\n");
	}
        
        sprintf(myprefix, "                %s", prefix);

        if(tag->id == ST_DEFINESPRITE) {
            sprintf(prefix, "         ");
	    if(issprite) {
		fprintf(stderr, "Error: Sprite definition inside a sprite definition");
	    }
	    issprite = 1;
	    spriteframe = 0;
	    spriteframelabel = 0;
        }
        else if(tag->id == ST_END) {
            *prefix = 0;
	    issprite = 0;
	    if(tag->len)
		fprintf(stderr, "Error: End Tag not empty");
        }
        else if(tag->id == ST_DOACTION && action) {
            ActionTAG*actions;
            actions = swf_ActionGet(tag);
            swf_DumpActions(actions, myprefix);
        }
	else if(tag->id == ST_DEFINEBUTTON && action) {
	    dumpButtonActions(tag, myprefix);
	}
	else if(tag->id == ST_DEFINEBUTTON2 && action) {
	    dumpButton2Actions(tag, myprefix);
	}
	else if(tag->id == ST_PLACEOBJECT2) {
	    if((*(U8*)tag->data)&0x80)
		handlePlaceObject2(tag, myprefix);
	    else
		printf("\n");
	}
	if(tag->len && hex) {
	    int t;
	    printf("                %s-=> ",prefix);
	    for(t=0;t<tag->len;t++) {
		printf("%02x ", tag->data[t]);
		if((t && !(t&15)) || (t==tag->len-1))
		{
		    if(t==tag->len-1)
			printf("\n");
		    else
			printf("\n                %s-=> ",prefix);
		}
	    }
	}
        tag = tag->next;
    }

    swf_FreeTags(&swf);
    return 0;
}


