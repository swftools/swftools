/* swfdump.c
   Shows the structure of a swf file

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
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

struct options_t options[] =
{
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
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char*name)
{    
    printf("Usage: %s file.swf\n", name);
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
  
int main (int argc,char ** argv)
{ 
    SWF swf;
    TAG*tag;
#ifdef HAVE_SYS_STAT_H
    struct stat statbuf;
#endif
    int f;
    char prefix[128];
    prefix[0] = 0;
    memset(idtab,0,65536);

    processargs(argc, argv);

    f = open(filename,O_RDONLY);

    if (f<0)
    { 
	perror("Couldn't open file: ");
	exit(1);
    }
    if FAILED(ReadSWF(f,&swf))
    { 
	fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",filename);
        close(f);
	exit(1);
    }

#ifdef HAVE_SYS_STAT_H
    fstat(f, &statbuf);
    if(statbuf.st_size != swf.FileSize)
	fprintf(stderr, "Error: Real Filesize (%d) doesn't match header Filesize (%d)",
		statbuf.st_size, swf.FileSize);
#endif

    close(f);

    printf("[HEADER]        File version: %d\n", swf.FileVersion);
    printf("[HEADER]        File size: %d\n", swf.FileSize);
    printf("[HEADER]        Frame rate: %f\n",swf.FrameRate/256.0);
    printf("[HEADER]        Frame count: %d\n",swf.FrameCount);
    printf("[HEADER]        Movie width: %.3f\n",(swf.MovieSize.xmax-swf.MovieSize.xmin)/20.0);
    printf("[HEADER]        Movie height: %.3f\n",(swf.MovieSize.ymax-swf.MovieSize.ymin)/20.0);

    tag = swf.FirstTag;

    while(tag) {
	printf("[%03x] %9d %s%s", tag->id, tag->len, prefix, getTagName(tag));
	if(isDefiningTag(tag)) {
	    U16 id = GetDefineID(tag);
	    printf(" defines id %04x", id);
	    if(idtab[id])
		fprintf(stderr, "Error: Id %04x is defined more than once.\n", id);
	    idtab[id] = 1;
	}
	else if(tag->id == ST_PLACEOBJECT || 
		tag->id == ST_PLACEOBJECT2) {
	    printf(" places id %04x at depth %04x", GetPlaceID(tag), GetDepth(tag));
	    if(GetName(tag))
		printf(" name \"%s\"",GetName(tag));
        }
	else if(tag->id == ST_REMOVEOBJECT) {
	    printf(" removes id %04x from depth %04x", GetPlaceID(tag), GetDepth(tag));
	}
	else if(tag->id == ST_REMOVEOBJECT2) {
	    printf(" removes object from depth %04x", GetDepth(tag));
	}
	
	printf("\n");

	if(tag->id == ST_DEFINESPRITE)
	{
	    sprintf(prefix, "         ");
	}
	if(tag->id == ST_END) 
	{
	    sprintf(prefix, "");
	}
	tag = tag->next;
    }

    FreeTags(&swf);
    return 0;
}

