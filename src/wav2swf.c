/* wav2swf.c
   Converts WAV/WAVE files to SWF.

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

#include <stdlib.h>
#include <stdio.h>
#include "../lib/rfxswf.h"
#include "../lib/log.h"
#include "../lib/args.h"
#include "../lib/wav.h"

char * filename = 0;
char * outputname = "output.swf";
int verbose = 2;
int stopframe0 = 0;
int stopframe1 = 0;

#define DEFINESOUND_MP3 1 //define sound uses mp3?- undefine for raw sound.

static struct options_t options[] = {
{"h", "help"},
{"V", "version"},
{"o", "output"},
{"r", "framerate"},
{"s", "samplerate"},
{"d", "definesound"},
{"l", "loop"},
{"C", "cgi"},
{"S", "stop"},
{"E", "end"},
{"b", "bitrate"},
{"v", "verbose"},
{0,0}
};

static int loop = 0;
static int definesound = 0;
static int framerate = 0;
static int samplerate = 11025;
static int bitrate = 32;
static int do_cgi = 0;

static int mp3_bitrates[] =
{ 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0};

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
    else if(!strcmp(name, "S")) {
	stopframe0 = 1;
	return 0;
    }
    else if(!strcmp(name, "E")) {
	stopframe1 = 1;
	return 0;
    }
    else if(!strcmp(name, "C")) {
	do_cgi = 1;
	return 0;
    }
    else if(!strcmp(name, "r")) {
	float f;
	sscanf(val, "%f", &f);
	framerate = f*256;
	return 1;
    }
    else if(!strcmp(name, "s")) {
	samplerate = atoi(val);
	if(samplerate > 5000 && samplerate < 6000)
	    samplerate = 5512;
	else if(samplerate > 11000 && samplerate < 12000)
	    samplerate = 11025;
	else if(samplerate > 22000 && samplerate < 23000)
	    samplerate = 22050;
	else if(samplerate > 44000 && samplerate < 45000)
	    samplerate = 44100;
	else {
	    fprintf(stderr, "Invalid samplerate: %d\n", samplerate);
	    fprintf(stderr, "Allowed values: 11025, 22050, 44100\n");
	    exit(1);
	}
	return 1;
    }
    else if(!strcmp(name, "b")) {
	int t;
	int b = atoi(val);
	if(b<=0) {
	    fprintf(stderr, "Not a valid bitrate: %s\n", val);
	    exit(1);
	}
	if(b>160) {
	    fprintf(stderr, "Bitrate must be <144. (%s)\n", val);
	    exit(1);
	}
	for(t=0;mp3_bitrates[t];t++) {
	    if(b== mp3_bitrates[t]) {
		bitrate = b;
		return 1;
	    }
	}
	fprintf(stderr, "Invalid bitrate. Allowed bitrates are:\n");
	for(t=0;mp3_bitrates[t];t++) {
	    printf("%d ", mp3_bitrates[t]);
	}
	printf("\n");
	exit(1);
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
    printf("Usage: %s [-o filename] file.wav\n", name);
    printf("\n");
    printf("-h , --help                    Print short help message and exit\n");
    printf("-V , --version                 Print version info and exit\n");
    printf("-o , --output <filename>       Explicitly specify output file. (Otherwise, output will go to output.swf)\n");
    printf("-r , --framerate <fps>         Set file framerate to <fps> frames per second.\n");
    printf("-s , --samplerate <sps>        Set samplerate to <sps> frames per second (default: 11025).\n");
    printf("-d , --definesound             Generate a DefineSound tag instead of streaming sound.\n");
    printf("-l , --loop n                  (Only used with -d)\n");
    printf("-C , --cgi                     For use as CGI- prepend http header, write to stdout.\n");
    printf("-S , --stop                    Stop the movie at frame 0\n");
    printf("-E , --end                     Stop the movie at the end frame\n");
    printf("-b , --bitrate <bps>           Set mp3 bitrate to <bps> (default: 32)\n");
    printf("-v , --verbose                 Be more verbose\n");
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

extern int swf_mp3_bitrate;
extern int swf_mp3_out_samplerate;
extern int swf_mp3_in_samplerate;

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

    blocksize = (samplerate > 22050) ? 1152 : 576;

    blockspersecond = (float)samplerate/blocksize;

    framespersecond = blockspersecond;
    if(framerate)
	framespersecond = framerate/256.0;

    framesperblock = framespersecond / blockspersecond;
    samplesperframe = (blocksize * blockspersecond) / framespersecond;
    samplesperblock = samplesperframe * framesperblock;
    
    initLog(0,-1,0,0,-1,verbose);

    if(!filename) {
	msg("<fatal> You must supply a filename");
	exit(1);
    }

    if(!wav_read(&wav, filename))
    {
	msg("<fatal> Error reading %s", filename);
	exit(1);
    }
    wav_convert2mono(&wav,&wav2, samplerate);
    //wav_print(&wav);
    //wav_print(&wav2);
    samples = (U16*)wav2.data;
    numsamples = wav2.size/2;

#ifdef WORDS_BIGENDIAN
    /* swap bytes */
    for(t=0;t<numsamples;t++) {
        samples[t] = (samples[t]>>8)&0xff | (samples[t]<<8)&0xff00;
    }
#endif

    if(numsamples%blocksize != 0)
    {
	// apply padding, so that block is a multiple of blocksize
	int numblocks = (numsamples+blocksize-1)/blocksize;
	int numsamples2;
	U16* samples2;
	numsamples2 = numblocks * blocksize;
	samples2 = malloc(sizeof(U16)*numsamples2);
	memcpy(samples2, samples, numsamples*sizeof(U16));
	memset(&samples2[numsamples], 0, sizeof(U16)*(numsamples2 - numsamples));
	numsamples = numsamples2;
	samples = samples2;
    }

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

    if(stopframe0) {
	ActionTAG*action = 0;
	tag = swf_InsertTag(tag, ST_DOACTION);
	action = action_Stop(action);
	action = action_End(action);
	swf_ActionSet(tag, action);
	swf_ActionFree(action);

	tag = swf_InsertTag(tag, ST_SHOWFRAME);
    }
	
    swf_mp3_bitrate = bitrate;
    swf_mp3_out_samplerate = samplerate;
    swf_mp3_in_samplerate = samplerate;

    if(!definesound)
    {
	int oldframepos=-1, newframepos=0;
	float framesamplepos = 0;
	float framepos = 0;
	float samplepos = 0;
	ActionTAG* a = 0;
	U16 v1=0,v2=0;
	tag = swf_InsertTag(tag, ST_SOUNDSTREAMHEAD);
	swf_SetSoundStreamHead(tag, samplesperframe);
	msg("<notice> %d blocks", numsamples/blocksize);
	for(t=0;t<numsamples/blocksize;t++) {
	    int s;
	    U16*block1;
	    int seek = blocksize - ((int)samplepos - (int)framesamplepos);

	    if(newframepos!=oldframepos) {
		tag = swf_InsertTag(tag, ST_SOUNDSTREAMBLOCK);
		msg("<notice> Starting block %d %d+%d", t, (int)samplepos, (int)blocksize);
		block1 = &samples[t*blocksize];
		swf_SetSoundStreamBlock(tag, block1, seek, 1);
		v1 = v2 = GET16(tag->data);
	    } else {
		msg("<notice> Adding data...", t);
		block1 = &samples[t*blocksize];
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
        swf_SetBlock(tag, samples, numsamples*2);
#endif


	tag = swf_InsertTag(tag, ST_STARTSOUND);
	swf_SetU16(tag, 24); //id
	memset(&info, 0, sizeof(info));
	info.loops = loop;
	swf_SetSoundInfo(tag, &info);
	tag = swf_InsertTag(tag, ST_SHOWFRAME);
        if(stopframe1) {
	    ActionTAG*action = 0;
	    tag = swf_InsertTag(tag, ST_DOACTION);
	    action = action_Stop(action);
	    action = action_End(action);
	    swf_ActionSet(tag, action);
	    swf_ActionFree(action);
	    tag = swf_InsertTag(tag, ST_SHOWFRAME);
        }
	tag = swf_InsertTag(tag, ST_END);
    }

    if(do_cgi) {
	if FAILED(swf_WriteCGI(&swf)) fprintf(stderr,"WriteCGI() failed.\n");
    } else {
	f = open(outputname,O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
	if FAILED(swf_WriteSWF(f,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
	close(f);
    }

    swf_FreeTags(&swf);
    return 0;
}


