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

struct options_t options[] =
{
 {"a","action"},
 {"X","width"},
 {"Y","height"},
 {"r","rate"},
 {"e","html"},
 {"v","verbose"},
 {"V","version"},
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
    printf("Usage: %s [-a] file.swf\n", name);
    printf("-h , --help\t\t\t Print help and exit\n");
    printf("-e , --html\t\t\t Create a html embedding the file (simple, but useful)\n");
    printf("-X , --width\t\t\t Prints out a string of the form \"-X width\"\n");
    printf("-Y , --height\t\t\t Prints out a string of the form \"-Y height\"\n");
    printf("-r , --rate\t\t\t Prints out a string of the form \"-r rate\"\n");
    printf("-a , --action\t\t\t Disassemble action tags\n");
    printf("-V , --version\t\t\t Print program version and exit\n");
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
      swf_GetCXForm(tag,NULL,0);  // matrix
    }

    while(offsetpos)
    { U8 a;
      ActionTAG*actions;
        
      offsetpos = swf_GetU16(tag);
      condition = swf_GetU16(tag);                // condition
      
      actions = swf_GetActions(tag);
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
    actions = swf_GetActions(tag);
    swf_DumpActions(actions, prefix);
}

int main (int argc,char ** argv)
{ 
    SWF swf;
    TAG*tag;
#ifdef HAVE_STAT
    struct stat statbuf;
#endif
    int f;
    int xsize,ysize;
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
        
        printf("\n");
        sprintf(myprefix, "                %s", prefix);

        if(tag->id == ST_DEFINESPRITE) {
            sprintf(prefix, "         ");
        }
        else if(tag->id == ST_END) {
            *prefix = 0;
        }
        else if(tag->id == ST_DOACTION && action) {
            ActionTAG*actions;
            actions = swf_GetActions(tag);
            swf_DumpActions(actions, myprefix);
        }
	else if(tag->id == ST_DEFINEBUTTON && action) {
	    dumpButtonActions(tag, myprefix);
	}
	else if(tag->id == ST_DEFINEBUTTON2 && action) {
	    dumpButton2Actions(tag, myprefix);
	}
        tag = tag->next;
    }

    swf_FreeTags(&swf);
    return 0;
}

