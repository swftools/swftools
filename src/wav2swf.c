/* swfextract.c
   Allows to extract parts of the swf into a new file.

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#include <stdlib.h>
#include <stdio.h>
#include "../lib/rfxswf.h"
#include "../lib/log.h"
#include "../lib/args.h"
#include "wav.h"

char * filename = 0;
char * outputname = "output.swf";
int verbose = 2;

struct options_t options[] =
{
 {"o","output"},
 {"v","verbose"},
 {"d","definesound"},
 {"l","loop"},
 {"r","framerate"},
 {"V","version"},
 {0,0}
};

static int loop = 0;
static int definesound = 0;
static int framerate = 0;

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("wav2swf - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    }
    else if(!strcmp(name, "o")) {
	outputname = val;
	return 1;
    }
    else if(!strcmp(name, "d")) {
	definesound = 1;
	return 0;
    }
    else if(!strcmp(name, "l")) {
	loop = atoi(val);
	return 1;
    }
    else if(!strcmp(name, "v")) {
	verbose ++;
	return 0;
    }
    else if(!strcmp(name, "r")) {
	float f;
	sscanf(val, "%f", &f);
	framerate = f*256;
	return 1;
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
void args_callback_usage(char*name)
{
    printf("Usage: %s [-o filename] file.wav\n", name);
    printf("\t-v , --verbose\t\t\t Be more verbose\n");
    printf("\t-d , --definesound\t\t\t Generate a DefineSound tag instead of streaming sound\n");
    printf("\t-l , --loop n\t\t\t Loop sound n times (implies -d)\n");
    printf("\t-r , --framerate fps\t\t\t Set framerate to fps frames per seond\n");
    printf("\t-o , --output filename\t\t set output filename (default: output.swf)\n");
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

int main (int argc,char ** argv)
{ 
    SWF swf;
    RGBA rgb;
    SRECT r;
    S32 width=300,height = 300;
    TAG * tag;

    int f,i,ls1,fs1;
    int count;
    int t;
    struct WAV wav,wav2;
    int blocksize;
    float blockspersecond;
    float framespersecond;
    float framesperblock;
    float framepos = 0;
    U16* samples;
    int numsamples;

    processargs(argc, argv);

    blocksize = 1152;
    blockspersecond = 11025.0/blocksize;
    framespersecond = blockspersecond;
    if(framerate)
	framespersecond = framerate/256.0;
    framesperblock = framespersecond/blockspersecond;

    initLog(0,-1,0,0,-1,verbose);

    if(!readWAV(filename, &wav))
    {
	logf("<fatal> Error reading %s", filename);
	exit(1);
    }
    convertWAV2mono(&wav,&wav2, 44100);
    //printWAVInfo(&wav);
    //printWAVInfo(&wav2);
    samples = (U16*)wav2.data;
    numsamples = wav2.size/2;

    memset(&swf,0x00,sizeof(SWF));

    swf.fileVersion    = 5;
    swf.frameRate      = (int)(framespersecond*256);

    swf.movieSize.xmax = 20*width;
    swf.movieSize.ymax = 20*height;

    swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    tag = swf.firstTag;
    rgb.r = 0xff;
    rgb.g = 0xff;
    rgb.b = 0xff;
    swf_SetRGB(tag,&rgb);

    if(!definesound)
    {
	tag = swf_InsertTag(tag, ST_SOUNDSTREAMHEAD);
	swf_SetSoundStreamHead(tag, blocksize);

	logf("<notice> %d blocks", numsamples/(blocksize*2));
	for(t=0;t<numsamples/(blocksize*2);t++) {
	    int s;
	    int oldframe, newframe;
	    U16*block1;
	    tag = swf_InsertTag(tag, ST_SOUNDSTREAMBLOCK);
	    logf("<notice> Writing block %d", t);
	    block1 = &samples[t*2*blocksize];
	    swf_SetSoundStreamBlock(tag, block1, 1);

	    oldframe = (int)framepos;
	    framepos += framesperblock;
	    newframe = (int)framepos;
	    for(s=oldframe;s<newframe;s++)
		tag = swf_InsertTag(tag, ST_SHOWFRAME);
	}
	tag = swf_InsertTag(tag, ST_END);
    } else {
	SOUNDINFO info;
	tag = swf_InsertTag(tag, ST_DEFINESOUND);
	swf_SetU16(tag, 24); //id
	swf_SetSoundDefine(tag, samples, numsamples);
	tag = swf_InsertTag(tag, ST_STARTSOUND);
	swf_SetU16(tag, 24); //id
	memset(&info, 0, sizeof(info));
	info.loops = loop;
	swf_SetSoundInfo(tag, &info);
	tag = swf_InsertTag(tag, ST_SHOWFRAME);
	tag = swf_InsertTag(tag, ST_END);
    }

    f = open(outputname,O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
    close(f);

    swf_FreeTags(&swf);
    return 0;
}


