/* swfextract.c
   Allows to extract parts of the swf into a new file.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "reloc.h"

char * filename = 0;
char * destfilename = "output.swf";
int verbose = 2;
int extractid = -1;
char* extractname = 0;

struct options_t options[] =
{
 {"o","output"},
 {"v","verbose"},
 {"i","id"},
 {"n","name"},
 {"V","version"},
 {0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfextract - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } 
    if(!strcmp(name, "o")) {
	destfilename = val;
	return 1;
    } 
    if(!strcmp(name, "i")) {
	extractid = atoi(val);
	if(extractname) {
	    fprintf(stderr, "You can only supply either name or id\n");
	    exit(1);
	}
	return 1;
    } 
    if(!strcmp(name, "n")) {
	extractname = val;
	if(extractid>=0) {
	    fprintf(stderr, "You can only supply either name or id\n");
	    exit(1);
	}
	return 1;
    } 
    if(!strcmp(name, "v")) {
	verbose ++;
	return 0;
    } 
    else {
        printf("Unknown option: -%s\n", name);
	return 0;
    }

    return 0;
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char*name)
{    
    printf("Usage: %s [-v] [-i id] file.swf\n", name);
    printf("\t-v , --verbose\t\t\t Be more verbose\n");
    printf("\t-i , --id ID\t\t\t ID of the object to extract\n");
    printf("\t-n , --name name\t\t\t instance name of the object to extract\n");
    printf("\t-V , --version\t\t\t Print program version and exit\n");
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

U8 mainr,maing,mainb;
char used[65536];
TAG*tags[65536];
int changed;

void idcallback(void*data)
{
    if(!used[*(U16*)data]) {
	changed = 1;
	used[*(U16*)data] = 1;
    }
}

void enumerateIDs(TAG*tag, void(*callback)(void*))
{
    U8*data;
    int len = tag->len;
    if(tag->len>=64) {
	len += 6;
	data = (U8*)malloc(len);
	*(U16*)data = (tag->id<<6)+63;
	*(U32*)&data[2] = tag->len;
	memcpy(&data[6], tag->data, tag->len);
    } else {
	len += 2;
	data = (U8*)malloc(len);
	*(U16*)data = (tag->id<<6)+tag->len;
	memcpy(&data[2], tag->data, tag->len);
    }
    map_ids_mem(data, len, callback);
}

void extractTag(SWF*swf, TAG*maintag, char*filename)
{
    SWF newswf;
    TAG*desttag;
    TAG*srctag;
    RGBA rgb;
    char sprite;
    int f;
    int t;
    int copy = 0;
    int defineid = swf_GetDefineID(maintag);
    memset(&newswf,0x00,sizeof(SWF));        // set global movie parameters
    memset(used, 0,65536);

    newswf.fileVersion    = swf->fileVersion;
    newswf.frameRate      = swf->frameRate;
    newswf.movieSize	  = swf->movieSize;
		
    newswf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    desttag = newswf.firstTag;
    rgb.r = mainr;
    rgb.g = maing;
    rgb.b = mainb;
    swf_SetRGB(desttag,&rgb);

    used[defineid] = 1;
    do {
       changed = 0;
       for(t=0;t<65536;t++) {
	   if(used[t]==1) {
	     if(tags[t]->id==ST_DEFINESPRITE) {
		 TAG*tag = tags[t];
		 while(tag->id != ST_END)
		 {
		     enumerateIDs(tag, idcallback);
		     tag = tag->next;
		 }
	     }
	     else
		enumerateIDs(tags[t], idcallback);
	     used[t] = 2;
	   }
       }
    }
    while(changed);

    srctag = swf->firstTag;
    while(srctag && (srctag->id || sprite)) {
	if(!sprite) {
	    copy = 0;
	}
	if(srctag->id == ST_END) {
	    sprite = 0;
	}
	if(srctag->id == ST_DEFINESPRITE)
	    sprite = 1;
	if(swf_isDefiningTag(srctag)) {
	    int id = swf_GetDefineID(srctag);
	    if(used[id]) 
		copy = 1;
	} else 
	if(srctag->id == ST_PLACEOBJECT ||
	    srctag->id == ST_PLACEOBJECT2) {
	    if(swf_GetPlaceID(srctag) == defineid)
		copy = 1;
	}
	if(copy) {
	    TAG*ttag = (TAG*)malloc(sizeof(TAG));
	    desttag = swf_InsertTag(desttag, srctag->id);
	    desttag->len = desttag->memsize = srctag->len;
	    desttag->data = malloc(srctag->len);
	    memcpy(desttag->data, srctag->data, srctag->len);
	}
	
	srctag = srctag->next;
    }
    desttag = swf_InsertTag(desttag,ST_SHOWFRAME);
    
    f = open(filename, O_TRUNC|O_WRONLY|O_CREAT, 0644);
    if FAILED(swf_WriteSWF(f,&newswf)) fprintf(stderr,"WriteSWF() failed.\n");
    close(f);

    swf_FreeTags(&newswf);                       // cleanup
}

int main (int argc,char ** argv)
{ 
    TAG*tag;
    SWF swf;
    int f;
    processargs(argc, argv);

    if(!filename)
    {
        fprintf(stderr, "You must supply a filename.\n");
        return 1;
    }
    initLog(0,-1,0,0,-1, verbose);

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

    tag = swf.firstTag;

    while(tag) {
	if(swf_isDefiningTag(tag)) {
	    int id = swf_GetDefineID(tag);
	    tags[id] = tag;
	}
	else if (tag->id == ST_SETBACKGROUNDCOLOR) {
	    mainr = tag->data[0];
	    maing = tag->data[1];
	    mainb = tag->data[2];
	}
	else if(tag->id == ST_PLACEOBJECT2) {
	    char*name = swf_GetName(tag);
	    if(name && !strcmp(name, extractname)) {
		int id = swf_GetPlaceID(tag); 
		if(extractid>=0 && id != extractid) {
		    fprintf(stderr, "Error: More than one instance with name \"%s\"", name);
		    exit(1);
		}
		extractid = swf_GetPlaceID(tag); 
	    }
	}
	tag = tag->next;
    }
    if(tags[extractid])
	extractTag(&swf, tags[extractid], destfilename);

    swf_FreeTags(&swf);
    return 0;
}

