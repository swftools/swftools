/* avi2swf.cc
   Convert avi movie files into swf.

   Part of the swftools package.
   
   Copyright (c) 2001,2002,2003 Matthias Kramm <kramm@quiss.org>
 
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "../config.h"

#include "../lib/args.h"
#include "v2swf.h"
#ifdef WIN32
#include "videoreader_vfw.hh"
#else
#include "videoreader_avifile.hh"
#endif

static char * filename = 0;
static char * outputfilename = "output.swf";
int verbose = 0;

static int quality = 80;
static double scale = 1.0;
static int flip = 0;
static int expensive = 0;
static int flashversion = 6;
static int keyframe_interval = -1;
static int skip = 0;
static float audio_adjust = 0;
static int mp3_bitrate = 32;
static int samplerate = 11025;
static int numframes = 0;
static char* skipframes = 0;

static struct options_t options[] = {
{"h", "help"},
{"o", "output"},
{"A", "adjust"},
{"n", "num"},
{"m", "mp3-bitrate"},
{"r", "mp3-samplerate"},
{"s", "scale"},
{"S", "skipframes"},
{"p", "flip"},
{"q", "quality"},
{"k", "keyframe"},
{"x", "extragood"},
{"T", "flashversion"},
{"V", "version"},
{0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("avi2swf-ng - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } 
    else if(!strcmp(name, "o")) {
	outputfilename = val;
	return 1;
    }
    else if(!strcmp(name, "n")) {
	numframes = atoi(val);
	return 1;
    }
    else if(!strcmp(name, "d")) {
	scale = atof(val);
	return 1;
    }
    else if(!strcmp(name, "q")) {
	quality = atoi(val);
	if(quality<0)
	    quality = 0;
	if(quality>100)
	    quality = 100;
	return 1;
    }
    else if(!strcmp(name, "p")) {
	flip = 1;
	return 0;
    }
    else if(!strcmp(name, "k")) {
	keyframe_interval = atoi(val);
	return 1;
    }
    else if(!strcmp(name, "A")) {
	audio_adjust = atof(val);
	return 1;
    }
    else if(!strcmp(name, "v")) {
	verbose = 1;
	return 0;
    }
    else if(!strcmp(name, "T")) {
	flashversion = atoi(val);
	return 1;
    }
    else if(!strcmp(name, "x")) {
	expensive = 1;
	return 0;
    }
    else if(!strcmp(name, "m")) {
	mp3_bitrate = atoi(val);
	return 1;
    }
    else if(!strcmp(name, "r")) {
        samplerate = atoi(val);
        if(samplerate >= 11000 && samplerate <= 12000)
            samplerate = 11025;
        else if(samplerate >= 22000 && samplerate <= 23000)
            samplerate = 22050;
        else if(samplerate >= 44000 && samplerate <= 45000)
            samplerate = 44100;
        else {
            fprintf(stderr, "Invalid samplerate: %d\n", samplerate);
            fprintf(stderr, "Allowed values: 11025, 22050, 44100\n", samplerate);
            exit(1);
        }
        return 1;
    }
    else if(!strcmp(name, "S")) {
	skip = atoi(val);
	return 1;
    }
    else if(!strcmp(name, "C")) {
	skipframes = strdup(val);
	return 1;
    }
    else if(!strcmp(name, "s")) {
	scale = atoi(val)/100.0;
	if(scale>1.0 || scale<=0) {
	    fprintf(stderr, "Scale must be in the range 1-100!\n");
	    exit(1);
	}
	return 1;
    }
    fprintf(stderr, "Unknown option: -%s\n", name);
    exit(1);
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char *name)
{
    printf("\n");
    printf("Usage: %s file.avi [-o output.swf]\n", name);
    printf("\n");
    printf("-h , --help                    Print help and exit\n");
    printf("-o , --output filename         Specify output filename\n");
    printf("-A , --adjust seconds          Audio adjust: Shift sound -seconds to the future or +seconds into the past.\n");
    printf("-n , --num frames              Number of frames to encode\n");
    printf("-m , --mp3-bitrate <kbps>      Set the mp3 bitrate to encode audio with\n");
    printf("-r , --mp3-samplerate <hz>     Set the mp3 samplerate to encode audio with (default: 11025)\n");
    printf("-s , --scale <val>             Scale down to factor <val>. (in %, e.g. 100 = original size)\n");
    printf("-S , --skipframes <num>        Skip <num> frames before starting the conversion.\n");
    printf("-p , --flip                    Turn movie upside down\n");
    printf("-q , --quality <val>           Set the quality to <val>. (0-100, 0=worst, 100=best, default:80)\n");
    printf("-k , --keyframe                Set the number of intermediate frames between keyframes.\n");
    printf("-x , --extragood               Enable some *very* expensive compression strategies.\n");
    printf("-T , --flashversion <n>        Set output flash version to <n>.\n");
    printf("-V , --version                 Print program version and exit\n");
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

static char toabuf[128];
static char*ftoa(double a)
{
    sprintf(toabuf, "%f", a);
    return toabuf;
}
static char*itoa(int a)
{
    sprintf(toabuf, "%d", a);
    return toabuf;
}

#ifdef DO_SIGNALS
pthread_t main_thread;
static void sigterm(int sig)
{
    if(pthread_equal (pthread_self(), main_thread))
    {
	if(frameno>0 && !shutdown_avi2swf) {
	    if(verbose)
		printf("Thread [%08x] got sigterm %d\n", pthread_self(), sig);
	    shutdown_avi2swf++;
	} else {
	    exit(1);
	}
    }
}
#endif

int main (int argc,char ** argv)
{ 
    videoreader_t video;
    v2swf_t v2swf;
    int ret;
    FILE*fi;

#ifdef DO_SIGNALS
    signal(SIGTERM, sigterm);
    signal(SIGINT , sigterm);
    signal(SIGQUIT, sigterm);
    main_thread = pthread_self();
#endif

    processargs(argc, argv);
    if(!filename) {
	fprintf(stderr, "You must supply a filename");
	exit(0);
    }
    if(keyframe_interval<0) {
	if(flashversion>=6)
	    keyframe_interval=20;
	else
	    keyframe_interval=5;
    }
    
    fi = fopen(outputfilename, "wb");
    if(!fi) {
	fflush(stdout); fflush(stderr);
	fprintf(stderr, "Couldn't open %s\n", outputfilename);
	exit(1);
    }
    
#ifdef WIN32
    ret = videoreader_vfw_open(&video, filename);
#else
    ret = videoreader_avifile_open(&video, filename);
#endif

    if(ret<0) {
	fprintf(stderr, "Error opening %s\n", filename);
	exit(1);
    }

    if(verbose) {
	printf("| video framerate: %f\n", video.fps);
	printf("| video size: %dx%d\n", video.width, video.height);
	printf("| audio rate: %d\n", video.samplerate);
	printf("| audio channels: %d\n", video.channels);
    }

    ret = v2swf_init(&v2swf, &video);
    if(verbose)
	v2swf_setparameter(&v2swf, "verbose", "1");
    if(numframes)
	v2swf_setparameter(&v2swf, "numframes", itoa(numframes));
    v2swf_setparameter(&v2swf, "quality", itoa(quality));
    v2swf_setparameter(&v2swf, "blockdiff", "0");
    v2swf_setparameter(&v2swf, "blockdiff_mode", "exact");
    v2swf_setparameter(&v2swf, "mp3_bitrate", itoa(mp3_bitrate));
    v2swf_setparameter(&v2swf, "samplerate", itoa(samplerate));
    //v2swf_setparameter(&v2swf, "fixheader", "1");
    //v2swf_setparameter(&v2swf, "framerate", "15");
    v2swf_setparameter(&v2swf, "scale", ftoa(scale));
    v2swf_setparameter(&v2swf, "prescale", "1");
    v2swf_setparameter(&v2swf, "flash_version", itoa(flashversion));
    v2swf_setparameter(&v2swf, "keyframe_interval", itoa(keyframe_interval));
    if(skipframes)
	v2swf_setparameter(&v2swf, "skipframes", skipframes);
    if(expensive)
	v2swf_setparameter(&v2swf, "motioncompensation", "1");
    if(flip)
	video.setparameter(&video, "flip", "1");
    if(verbose)
	video.setparameter(&video, "verbose", "1");

    if(!verbose)
	printf("\n");

    if(audio_adjust>0) {
	int num = ((int)(audio_adjust*video.samplerate))*video.channels*2;
	void*buf = malloc(num);
	video.getsamples(&video, buf, num);
	free(buf);
    } else if(audio_adjust<0) {
	int num = (int)(-audio_adjust*video.fps);
	void*buf = malloc(video.width*video.height*4);
	int t;
	for(t=0;t<num;t++) {
	    video.getimage(&video, buf);
	}
	free(buf);
    }

    if(skip) {
	int t;
	void*buf = malloc(video.width*video.height*4);
	for(t=0;t<skip;t++) {
	    video.getimage(&video, buf);
	    video.getsamples(&video, buf, (int)((video.samplerate/video.fps)*video.channels*2));
	    if(!verbose) {
		printf("\rSkipping frame %d", video.frame);fflush(stdout);
	    }
	}
	free(buf);
    }

    char buffer[4096];
    while(1) {
	int l=v2swf_read(&v2swf, buffer, 4096);
	fwrite(buffer, l, 1, fi);
	if(!l)
	    break;
	if(!verbose) {
	    printf("\rConverting frame %d", video.frame);fflush(stdout);
	}
    }
    if(!verbose)
	printf("\n");
    fclose(fi);
    v2swf_backpatch(&v2swf, outputfilename);
    v2swf_close(&v2swf);
}

