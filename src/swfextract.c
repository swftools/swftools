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

char* extractids = 0;
char* extractframes = 0;

char* extractname = 0;

char hollow = 0;

struct options_t options[] =
{
 {"o","output"},
 {"w","hollow"},
 {"v","verbose"},
 {"i","id"},
 {"n","name"},
 {"f","frame"},
 {"V","version"},
 {0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfextract - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } 
    else if(!strcmp(name, "o")) {
	destfilename = val;
	return 1;
    } 
    else if(!strcmp(name, "i")) {
	extractids = val;
	if(extractname) {
	    fprintf(stderr, "You can only supply either name or id\n");
	    exit(1);
	}
	return 1;
    } 
    else if(!strcmp(name, "n")) {
	extractname = val;
	if(extractids) {
	    fprintf(stderr, "You can only supply either name or id\n");
	    exit(1);
	}
	return 1;
    } 
    else if(!strcmp(name, "v")) {
	verbose ++;
	return 0;
    } 
    else if(!strcmp(name, "f")) {
	extractframes = val;
	return 1;
    }
    else if(!strcmp(name, "w")) {
	hollow = 1;
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
    printf("\t-o , --output filename\t\t set output filename\n");
    printf("\t-i , --id ID\t\t\t ID of the object to extract\n");
    printf("\t-n , --name name\t\t instance name of the object to extract\n");
    printf("\t-f , --frame frame\t\t frame number to extract\n");
    printf("\t-w , --hollow\t\t\t hollow mode: don't remove empty frames\n");
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
/* 1 = used, not expanded,
   3 = used, expanded
   5 = wanted, not expanded
   7 = wanted, expanded
 */
char used[65536];
TAG*tags[65536];
int changed;
char * tagused;

void idcallback(void*data)
{
    if(!(used[*(U16*)data]&1)) {
	changed = 1;
	used[*(U16*)data] |= 1;
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

void extractTag(SWF*swf, char*filename)
{
    SWF newswf;
    TAG*desttag;
    TAG*srctag;
    RGBA rgb;
    char sprite;
    int f;
    int t;
    int tagnum;
    int copy = 0;
    memset(&newswf,0x00,sizeof(SWF));        // set global movie parameters

    newswf.fileVersion    = swf->fileVersion;
    newswf.frameRate      = swf->frameRate;
    newswf.movieSize	  = swf->movieSize;
		
    newswf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    desttag = newswf.firstTag;
    rgb.r = mainr;
    rgb.g = maing;
    rgb.b = mainb;
    swf_SetRGB(desttag,&rgb);

    do {
       changed = 0;
       for(t=0;t<65536;t++) {
	   if(used[t] && !(used[t]&2)) {
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
	     used[t] |= 2;
	   }
       }
    }
    while(changed);

    srctag = swf->firstTag;
    tagnum = 0;
    sprite = 0;
    while(srctag && (srctag->id || sprite)) {
	int reset = 0;
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
	if (((srctag->id == ST_PLACEOBJECT ||
	      srctag->id == ST_PLACEOBJECT2 ||
	      srctag->id == ST_STARTSOUND) && (used[swf_GetPlaceID(srctag)]&4) ) ||
	      (swf_isPseudoDefiningTag(srctag) && used[swf_GetDefineID(srctag)]) ||
	      (tagused[tagnum])) 
	{
		if(copy == 0)
		    reset = 1;
		copy = 1;
	} 
	if(srctag->id == ST_REMOVEOBJECT) {
	    if(!used[swf_GetPlaceID(srctag)])
		copy = 0;
	}

	if(copy) {
	    TAG*ttag = (TAG*)malloc(sizeof(TAG));
	    desttag = swf_InsertTag(desttag, srctag->id);
	    desttag->len = desttag->memsize = srctag->len;
	    desttag->data = malloc(srctag->len);
	    memcpy(desttag->data, srctag->data, srctag->len);
	    if(reset)
	        copy = 0;
	}
	
	srctag = srctag->next;
	tagnum ++;
    }
    if(!extractframes && !hollow)
	desttag = swf_InsertTag(desttag,ST_SHOWFRAME);

    desttag = swf_InsertTag(desttag,ST_END);
    
    f = open(filename, O_TRUNC|O_WRONLY|O_CREAT, 0644);
    if FAILED(swf_WriteSWF(f,&newswf)) fprintf(stderr,"WriteSWF() failed.\n");
    close(f);

    swf_FreeTags(&newswf);                       // cleanup
}

void listObjects(SWF*swf)
{
    TAG*tag;
    char first;
    int t;
    int frame = 0;
    char*names[] = {"Shapes","MovieClips","Bitmaps","Sounds","Frames"};
    printf("Objects in file %s:\n",filename);
    for(t=0;t<5;t++) {
	tag = swf->firstTag;
	first = 1;
	while(tag) {
	    char show = 0;
	    char text[80];
	    if(t == 0 &&
	       (tag->id == ST_DEFINESHAPE ||
		tag->id == ST_DEFINESHAPE2 ||
		tag->id == ST_DEFINESHAPE3)) {
		show = 1;
	        sprintf(text,"%d", swf_GetDefineID(tag));
	    }

	    if(tag->id == ST_DEFINESPRITE) {
		if (t == 1)  {
		    show = 1;
		    sprintf(text,"%d", swf_GetDefineID(tag));
		}

		while(tag->id != ST_END)
		    tag = tag->next;
	    }

	    if(t == 2 && (tag->id == ST_DEFINEBITSLOSSLESS ||
		tag->id == ST_DEFINEBITSJPEG2 ||
		tag->id == ST_DEFINEBITSLOSSLESS2 ||
		tag->id == ST_DEFINEBITSJPEG3)) {
		show = 1;
		sprintf(text,"%d", swf_GetDefineID(tag));
	    }

	    if(t == 3 && (tag->id == ST_DEFINESOUND)) {
		show = 1;
		sprintf(text,"%d", swf_GetDefineID(tag));
	    }
	    
	    if(t == 4 && (tag->id == ST_SHOWFRAME)) {
		show = 1;
		sprintf(text,"%d", frame);
		frame ++;
	    }

	    if(show) {
		if(!first)
		    printf(", ");
		else
		    printf("%s: ", names[t]);
		printf("%s", text);
		first = 0;
	    }
	    tag=tag->next;
	}
	if(!first)
	    printf("\n");
    }
}

int main (int argc,char ** argv)
{ 
    TAG*tag;
    SWF swf;
    int f;
    int found = 0;
    int frame = 0;
    int tagnum = 0;
    char depths[65536];
    char listavailable = 0;
    processargs(argc, argv);

    if(!extractframes && !extractids && ! extractname)
	listavailable = 1;

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
    if (swf_ReadSWF(f,&swf) < 0)
    { 
        fprintf(stderr, "%s is not a valid SWF file or contains errors.\n",filename);
        close(f);
        exit(1);
    }
    close(f);

    if(listavailable) {
	listObjects(&swf);
	swf_FreeTags(&swf);
	return 0;
    }

    tag = swf.firstTag;
    tagnum = 0;
    while(tag) {
	tagnum ++;
	tag = tag->next;
    }

    tagused = (char*)malloc(tagnum);
    memset(tagused, 0, tagnum);
    memset(used, 0, 65536);
    memset(depths, 0, 65536);

    tag = swf.firstTag;
    tagnum = 0;
    while(tag) {
	if(swf_isAllowedSpriteTag(tag)) {
	    int write = 0;
	    if(extractframes && is_in_range(frame, extractframes)) {
		write = 1;
		if(tag->id == ST_PLACEOBJECT || tag->id == ST_PLACEOBJECT2) {
		    depths[swf_GetDepth(tag)] = 1;
		}
		if(tag->id == ST_REMOVEOBJECT || tag->id == ST_REMOVEOBJECT2) {
		    int depth = swf_GetDepth(tag);
		    if(!depths[depth]) 
			write = 0;
		    depths[swf_GetDepth(tag)] = 0;
		}
	    } else {
		if((tag->id == ST_REMOVEOBJECT || tag->id == ST_REMOVEOBJECT2) && 
		    (depths[swf_GetDepth(tag)]) && hollow) {
		    write = 1;
		    depths[swf_GetDepth(tag)] = 0;
		}
	    }
	    if(write) {
	        enumerateIDs(tag, idcallback);
	        found = 1;
	        tagused[tagnum] = 1;
	    }
	}

	if(swf_isDefiningTag(tag)) {
	    int id = swf_GetDefineID(tag);
	    tags[id] = tag;
	    if(extractids && is_in_range(id, extractids)) {
		used[id] = 5;
		found = 1;
	    }
	}
	else if (tag->id == ST_SETBACKGROUNDCOLOR) {
	    mainr = tag->data[0];
	    maing = tag->data[1];
	    mainb = tag->data[2];
	}
	else if(tag->id == ST_PLACEOBJECT2) {
	    char*name = swf_GetName(tag);
	    if(name && extractname && !strcmp(name, extractname)) {
		int id = swf_GetPlaceID(tag); 
		used[id] = 5;
		found = 1;
		tagused[tagnum] = 1;
		depths[swf_GetDepth(tag)] = 1;
	    }
	}
	else if(tag->id == ST_SHOWFRAME) {
	    frame ++;
	    if(hollow) {
		tagused[tagnum] = 1;
		found = 1;
	    }
	}
	
	if(tag->id == ST_DEFINESPRITE) {
	    while(tag->id != ST_END) { 
		tag = tag->next;
		tagnum ++;
	    }
	}
	tag = tag->next;
	tagnum ++;
    }
    if (found)
	extractTag(&swf, destfilename);

    swf_FreeTags(&swf);
    return 0;
}

