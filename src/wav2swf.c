/* wav2swf.c
   Converts WAV/WAVE files to SWF.

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

#define DEFINESOUND_MP3 1 //define sound uses mp3?- undefine for raw sound.

struct options_t options[] =
{
 {"o","output"},
 {"v","verbose"},
 {"d","definesound"},
 {"l","loop"},
 {"r","framerate"},
 {"b","bitrate"},
 {"V","version"},
 {0,0}
};

static int loop = 0;
static int definesound = 0;
static int framerate = 0;
static int bitrate = 32;

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
	definesound = 1;
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
    else if(!strcmp(name, "b")) {
	bitrate = atoi(val);
	if(bitrate<=0) {
	    fprintf(stderr, "Not a valid bitrate: %s\n", val);
	    exit(1);
	}
	if(bitrate>144) {
	    fprintf(stderr, "Bitrate must be <144. (%s)\n", val);
	    exit(1);
	}
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
    printf("\t-d , --definesound\t\t Generate a DefineSound tag instead of streaming sound\n");
    printf("\t-l , --loop n\t\t\t Loop sound n times (implies -d)\n");
    printf("\t-r , --framerate fps\t\t Set framerate to fps frames per seond\n");
    printf("\t-b , --bitrate bps\t\t Set mp3 bitrate (default: 32)\n");
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

extern int swf_mp3_bitrate;

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
    float samplesperframe;
    float framesperblock;
    float samplesperblock;
    U16* samples;
    int numsamples;

    processargs(argc, argv);

    blocksize = 576;
    blockspersecond = 11025.0/blocksize;

    framespersecond = blockspersecond;
    if(framerate)
	framespersecond = framerate/256.0;

    framesperblock = framespersecond / blockspersecond;
    samplesperframe = (blocksize * blockspersecond) / framespersecond;
    samplesperblock = samplesperframe * framesperblock;
    
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
	
    swf_mp3_bitrate = bitrate;

    if(!definesound)
    {
	int oldframepos=-1, newframepos=0;
	float framesamplepos = 0;
	float framepos = 0;
	float samplepos = 0;
	U16 v1=0,v2=0;
	tag = swf_InsertTag(tag, ST_SOUNDSTREAMHEAD);
	swf_SetSoundStreamHead(tag, samplesperframe);

	logf("<notice> %d blocks", numsamples/(blocksize*4));
	for(t=0;t<numsamples/(blocksize*4);t++) {
	    int s;
	    U16*block1;
	    int seek = blocksize - ((int)samplepos - (int)framesamplepos);

	    if(newframepos!=oldframepos) {
		tag = swf_InsertTag(tag, ST_SOUNDSTREAMBLOCK);
		logf("<notice> Starting block %d %d+%d", t, (int)samplepos, (int)blocksize);
		block1 = &samples[t*blocksize*4];
		swf_SetSoundStreamBlock(tag, block1, seek, 1);
		v1 = v2 = GET16(tag->data);
	    } else {
		logf("<notice> Adding data...", t);
		block1 = &samples[t*blocksize*4];
		swf_SetSoundStreamBlock(tag, block1, seek, 0);
		v1+=v2;
		PUT16(tag->data, v1);
	    }
	    samplepos += blocksize;

	    oldframepos = (int)framepos;
	    framepos += framesperblock;
	    newframepos = (int)framepos;

	    for(s=oldframepos;s<newframepos;s++) {
		tag = swf_InsertTag(tag, ST_SHOWFRAME);
		framesamplepos += samplesperframe;
	    }
	}
	tag = swf_InsertTag(tag, ST_END);
    } else {
	SOUNDINFO info;
	tag = swf_InsertTag(tag, ST_DEFINESOUND);
	swf_SetU16(tag, 24); //id
#ifdef DEFINESOUND_MP3
	swf_SetSoundDefine(tag, samples, numsamples);
#else
	swf_SetU8(tag,(/*compression*/0<<4)|(/*rate*/3<<2)|(/*size*/1<<1)|/*mono*/0);
	swf_SetU32(tag, numsamples); // 44100 -> 11025
	swf_SetBlock(tag, wav2.data, numsamples*2);
#endif

	tag = swf_InsertTag(tag, ST_STARTSOUND);
	swf_SetU16(tag, 24); //id
	memset(&info, 0, sizeof(info));
	info.loops = loop;
	swf_SetSoundInfo(tag, &info);
	tag = swf_InsertTag(tag, ST_SHOWFRAME);
	tag = swf_InsertTag(tag, ST_END);
    }

    f = open(outputname,O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
    if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
    close(f);

    swf_FreeTags(&swf);
    return 0;
}


