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

struct options_t options[] =
{
 {"a","action"},
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

int main (int argc,char ** argv)
{ 
    SWF swf;
    TAG*tag;
#ifdef HAVE_STAT
    struct stat statbuf;
#endif
    int f;
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
    if(statbuf.st_size != swf.fileSize)
        fprintf(stderr, "Error: Real Filesize (%d) doesn't match header Filesize (%d)",
                statbuf.st_size, swf.fileSize);
#endif

    close(f);

    printf("[HEADER]        File version: %d\n", swf.fileVersion);
    printf("[HEADER]        File size: %ld\n", swf.fileSize);
    printf("[HEADER]        Frame rate: %f\n",swf.frameRate/256.0);
    printf("[HEADER]        Frame count: %d\n",swf.frameCount);
    printf("[HEADER]        Movie width: %.3f\n",(swf.movieSize.xmax-swf.movieSize.xmin)/20.0);
    printf("[HEADER]        Movie height: %.3f\n",(swf.movieSize.ymax-swf.movieSize.ymin)/20.0);

    tag = swf.firstTag;

    while(tag) {
        char*name = swf_TagGetName(tag);
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
            if(swf_TagGetName(tag))
                printf(" name \"%s\"",swf_TagGetName(tag));
        }
        else if(tag->id == ST_REMOVEOBJECT) {
            printf(" removes id %04x from depth %04x", swf_GetPlaceID(tag), swf_GetDepth(tag));
        }
        else if(tag->id == ST_REMOVEOBJECT2) {
            printf(" removes object from depth %04x", swf_GetDepth(tag));
        }
        
        printf("\n");

        if(tag->id == ST_DEFINESPRITE) {
            sprintf(prefix, "         ");
        }
        else if(tag->id == ST_END) {
            *prefix = 0;
        }
        else if(tag->id == ST_DOACTION && action) {
            char myprefix[128];
            ActionTAG*actions;
            sprintf(myprefix, "                %s", prefix);
            
            actions = swf_GetActions(tag);

            swf_DumpActions(actions, myprefix);
        }
        tag = tag->next;
    }

    swf_FreeTags(&swf);
    return 0;
}

