/* avi2swf.cc
   Convert avi movie files into swf.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// TODO: those two should be checked for by configure
#include <pthread.h>
#include <signal.h>

#include "../config.h"
extern "C" {
#include "../lib/args.h"
}
#include "v2swf.h"

#undef HAVE_CONFIG_H
#include <avifile/version.h>
#if (AVIFILE_MAJOR_VERSION == 0) && (AVIFILE_MINOR_VERSION>=6) 
   #include <avifile.h>
   #include <aviplay.h>
   #include <fourcc.h>
   #include <creators.h>
   #include <StreamInfo.h>
   #define VERSION6
#else
   #include <avifile.h>
   #include <aviplay.h>
   #include <aviutil.h>
   #define Width width
   #define Height height
   #define Data data
   #define Bpp bpp
#endif

static char * filename = 0;
static char * outputfilename = "output.swf";
int verbose = 0;

static int quality = 80;
static double scale = 1.0;
static int flip = 0;
static int expensive = 0;

struct options_t options[] =
{
 {"v","verbose"},
 {"o","output"},
 {"p","flip"},
 {"q","quality"},
 {"d","scale"},
 {"z","zlib"},
 {"x","extragood"},
 {"V","version"},
 {0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("avi2swf - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } 
    else if(!strcmp(name, "o")) {
	outputfilename = val;
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
    else if(!strcmp(name, "v")) {
	verbose = 1;
	return 0;
    }
    else if(!strcmp(name, "x")) {
	expensive = 1;
	return 0;
    }
    else if(!strcmp(name, "d")) {
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
void args_callback_usage(char*name)
{    
    printf("\nUsage: %s file.avi\n", name);
    printf("\t-h , --help\t\t Print help and exit\n");
    printf("\t-o , --output filename\t Specify output filename\n"); 
    printf("\t-n , --num frames\t Number of frames to encode\n");
    printf("\t-s , --start frame\t First frame to encode\n");
    printf("\t-d , --scale factor\t Scale to factor percent\n");
    printf("\t-p , --flip\t\t Turn movie upside down\n");
    printf("\t-V , --version\t\t Print program version and exit\n");
    exit(0);
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

typedef struct _videoreader_avifile_internal
{
    IAviReadFile* player;
    IAviReadStream* astream;
    IAviReadStream* vstream;
    int do_audio;
    int do_video;
    int eof;
    int frame;
    int soundbits;
} videoreader_avifile_internal;

static int shutdown_avi2swf = 0;
static int frameno = 0;
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

int videoreader_avifile_getsamples(videoreader_t* v, void*buffer, int num)
{
    int ret;
    unsigned int samples_read, bytes_read;
    if(verbose) {
	printf("videoreader_getsamples(%d)\n", num);fflush(stdout);
    }
    videoreader_avifile_internal*i = (videoreader_avifile_internal*)v->internal;
    if(i->soundbits == 8) {
	ret = i->astream->ReadFrames(buffer, num/2, num/(v->channels*2), samples_read, bytes_read);
	unsigned char*b = (unsigned char*)buffer;
	int t;
	for(t=num-2;t>=0;t-=2) {
	    unsigned char x = b[t/2];
	    b[t] = 0;
	    b[t+1] = x-128;
	}
	return num;
    }
    if(i->soundbits == 16) {
	ret = i->astream->ReadFrames(buffer, num, num/(v->channels*2), samples_read, bytes_read);
	return num;
    }
    return 0;
}
int videoreader_avifile_getimage(videoreader_t* v, void*buffer)
{
    videoreader_avifile_internal*i = (videoreader_avifile_internal*)v->internal;
    if(verbose) {
	printf("videoreader_getimage()\n");fflush(stdout);
    }

    if(shutdown_avi2swf)
	i->eof = 1;
    
    if(i->eof)
	return 0;

    if(i->vstream->ReadFrame() < 0) {
	i->eof = 1;
	return 0;
    }
    CImage*img = i->vstream->GetFrame(true);
    if(!img) {
	i->eof = 1;
	return 0;
    }
    img->ToRGB();
    frameno++;
    i->frame++;
    unsigned char*data = img->Data();
    int bpp = img->Bpp();
    if(bpp == 3) {
	int x,y;
	for(y=0;y<v->height;y++) {
	    unsigned char*from,*to;
	    to = &((unsigned char*)buffer)[y*v->width*4];
	    if(flip)
		from = img->At(v->height-y-1);
	    else
		from = img->At(y);
	    for(x=0;x<v->width;x++) {
		to[x*4+0] = 0;
		to[x*4+1] = from[x*3+2];
		to[x*4+2] = from[x*3+1];
		to[x*4+3] = from[x*3+0];
	    }
	}
	return v->width*v->height*4;
    } else {
	return 0;
    }
}
bool videoreader_avifile_eof(videoreader_t* v)
{
    videoreader_avifile_internal*i = (videoreader_avifile_internal*)v->internal;
    if(verbose) {
	printf("videoreader_eof()\n");fflush(stdout);
    }
    return i->eof;
}
void videoreader_avifile_close(videoreader_t* v)
{
    videoreader_avifile_internal*i = (videoreader_avifile_internal*)v->internal;
    if(verbose) {
	printf("videoreader_close()\n");fflush(stdout);
    }
}
void* videoreader_avifile_getinfo(videoreader_t* v, char* name)
{
    return 0;
}
void videoreader_avifile_setparameter(videoreader_t*v, char*name, char*value)
{
    if(verbose) {
	printf("videoreader_setparameter(%s, %s)\n", name, value);fflush(stdout);
    }
}

int videoreader_avifile_open(videoreader_t* v, char* filename)
{
    videoreader_avifile_internal* i;
    i = (videoreader_avifile_internal*)malloc(sizeof(videoreader_avifile_internal));
    memset(i, 0, sizeof(videoreader_avifile_internal));
    memset(v, 0, sizeof(videoreader_t));
    v->getsamples = videoreader_avifile_getsamples;
    v->getinfo = videoreader_avifile_getinfo;
    v->close = videoreader_avifile_close;
    v->eof = videoreader_avifile_eof;
    v->getimage = videoreader_avifile_getimage;
    v->getsamples = videoreader_avifile_getsamples;
    v->setparameter = videoreader_avifile_setparameter;
    v->internal = i;

    i->do_video = 1;
    i->do_audio = 1;

    i->player = CreateIAviReadFile(filename);    
    if(verbose) {
	printf("%d streams (%d video, %d audio)\n", 
		i->player->StreamCount(),
		i->player->VideoStreamCount(),
		i->player->AudioStreamCount());
    }
    i->astream = i->player->GetStream(0, AviStream::Audio);
    i->vstream = i->player->GetStream(0, AviStream::Video);
    if(!i->vstream) {
	printf("Couldn't open video stream\n");
	i->do_video = 0;
    }
    if(!i->astream) {
	printf("Couldn't open video stream\n");
	i->do_audio = 0;
    }

    if(!i->do_video && !i->do_audio) {
	printf("File has neither audio nor video streams.(?)\n");
	return 0;
    }

#ifndef VERSION6
    MainAVIHeader head;
    int dwMicroSecPerFrame = 0;
    player->GetFileHeader(&head);
    printf("fps: %d\n", 1000000/head.dwMicroSecPerFrame);
    printf("frames: %d\n", head.dwTotalFrames);
    printf("streams: %d\n", head.dwStreams);
    printf("width: %d\n", head.dwWidth);
    printf("height: %d\n", head.dwHeight);
    printf("sound: %u samples (%f seconds)\n", i->astream->GetEndPos(), i->astream->GetEndTime());
    v->width = head.dwWidth;
    v->height = head.dwHeight;
    dwMicroSecPerFrame = head.dwMicroSecPerFrame;
    samplesperframe = astream->GetEndPos()/astream->GetEndTime()*head.dwMicroSecPerFrame/1000000;
    v->rate = (int)(astream->GetEndPos()/astream->GetEndTime());
    v->fps = 1000000.0/dwMicroSecPerFrame;
    i->soundbits = 16;
#else
    StreamInfo*audioinfo;
    StreamInfo*videoinfo;
    if(i->do_video)
    {
	videoinfo = i->vstream->GetStreamInfo();
	v->width = videoinfo->GetVideoWidth();
	v->height = videoinfo->GetVideoHeight();
	v->fps = (double)(videoinfo->GetFps());
    }
    if(i->do_audio)
    {
	WAVEFORMATEX wave;
	i->astream->GetAudioFormatInfo(&wave,0);
	v->channels = wave.nChannels;
	i->soundbits = wave.wBitsPerSample;
	if(wave.wBitsPerSample != 8 && wave.wBitsPerSample != 16) {
	    printf("Can't handle %d bit audio, disabling sound\n", wave.wBitsPerSample);
	    i->do_audio = 0;
	    v->channels = 0;
	    v->rate = 0;
	}
    }
    if(i->do_audio)
    {
	audioinfo = i->astream->GetStreamInfo();
	v->rate = audioinfo->GetAudioSamplesPerSec();
    }
#endif
    i->vstream -> StartStreaming();
    if(i->do_audio)
	i->astream -> StartStreaming();

    return 1;
}

int main (int argc,char ** argv)
{ 
    videoreader_t video;
    v2swf_t v2swf;
    int ret;
    FILE*fi;

    signal(SIGTERM, sigterm);
    signal(SIGINT , sigterm);
    signal(SIGQUIT, sigterm);
    main_thread = pthread_self();

    processargs(argc, argv);
    if(!filename)
	exit(0);
    
    fi = fopen(outputfilename, "wb");
    if(!fi) {
	fflush(stdout); fflush(stderr);
	fprintf(stderr, "Couldn't open %s\n", outputfilename);
	exit(1);
    }

    ret = videoreader_avifile_open(&video, filename);

    if(!ret) {
	printf("Error opening %s\n", filename);
	exit(1);
    }

    if(verbose) {
	printf("| video framerate: %f\n", video.fps);
	printf("| video size: %dx%d\n", video.width, video.height);
	printf("| audio rate: %d\n", video.rate);
	printf("| audio channels: %d\n", video.channels);
    }

    ret = v2swf_init(&v2swf, &video);
    if(verbose)
	v2swf_setparameter(&v2swf, "verbose", "1");
    v2swf_setparameter(&v2swf, "quality", itoa(quality));
    v2swf_setparameter(&v2swf, "blockdiff", "0");
    v2swf_setparameter(&v2swf, "blockdiff_mode", "exact");
    v2swf_setparameter(&v2swf, "mp3_bitrate", "128");
    //v2swf_setparameter(&v2swf, "fixheader", "1");
    //v2swf_setparameter(&v2swf, "framerate", "15");
    v2swf_setparameter(&v2swf, "scale", ftoa(scale));
    v2swf_setparameter(&v2swf, "prescale", "1");
    v2swf_setparameter(&v2swf, "keyframe_interval", "200");
    v2swf_setparameter(&v2swf, "flash_version", "6");
    if(expensive)
	v2swf_setparameter(&v2swf, "motioncompensation", "1");


    char buffer[4096];
    if(!verbose)
	printf("\n");
    while(1) {
	int l=v2swf_read(&v2swf, buffer, 4096);
	fwrite(buffer, l, 1, fi);
	if(!l)
	    break;
	if(!verbose) {
	    printf("\rConverting frame %d", frameno);fflush(stdout);
	}
    }
    if(!verbose)
	printf("\n");
    fclose(fi);
    v2swf_backpatch(&v2swf, outputfilename);
    v2swf_close(&v2swf);
}

