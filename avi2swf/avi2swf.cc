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

#ifdef HAVE_SIGNAL_H
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#include <signal.h>
#define DO_SIGNALS
#endif
#endif

extern "C" {
#include "../lib/args.h"
}
#include "v2swf.h"
#include "../lib/q.h"

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
static int flashversion = 6;
static int keyframe_interval = -1;
static int skip = 0;
static float audio_adjust = 0;
static int mp3_bitrate = 32;
static int samplerate = 11025;

struct options_t options[] =
{
 {"v","verbose"},
 {"A","adjust"},
 {"o","output"},
 {"p","flip"},
 {"m","mp3-bitrate"},
 {"r","mp3-samplerate"},
 {"q","quality"},
 {"s","scale"},
 {"S","skip"},
 {"x","extragood"},
 {"T","flashversion"},
 {"V","version"},
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
	return 0;
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
void args_callback_usage(char*name)
{    
    printf("\nUsage: %s file.avi\n", name);
    printf("\t-h , --help\t\t Print help and exit\n");
    printf("\t-o , --output filename\t Specify output filename\n"); 
    printf("\t-A , --adjust seconds\t Audio adjust: Shift sound -seconds to the future or +seconds into the past.\n"); 
    printf("\t-n , --num frames\t Number of frames to encode\n");
    printf("\t-m , --mp3-bitrate <rate> (kbps)\t Set the mp3 bitrate to encode audio with\n");
    printf("\t-r , --mp3-samplerate <rate> (Hz)\t Set the mp3 samplerate to encode audio with (default: 11025)\n");
    printf("\t-d , --scale <val>\t Scale down to factor <val>. (in %, e.g. 100 = original size)\n");
    printf("\t-p , --flip\t\t Turn movie upside down\n");
    printf("\t-q , --quality <val>\t Set the quality to <val>. (0-100, 0=worst, 100=best, default:80)\n");
    printf("\t-x , --extragood\t Enable some *very* expensive compression strategies. You may\n");
    printf("\t                \t want to let this run overnight.\n");
    printf("\t-T , --flashversion <n>\t Set output flash version to <n>. Notice: H.263 compression will only be\n");
    printf("\t                       \t used for n >= 6.\n");
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
    ringbuffer_t audio_buffer;
} videoreader_avifile_internal;

static int shutdown_avi2swf = 0;
static int frameno = 0;

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

static void readSamples(videoreader_avifile_internal*i, void*buffer, int buffer_size, int numsamples)
{
    int ret;
    while(i->audio_buffer.available < buffer_size) {
	unsigned int samples_read = 0, bytes_read = 0;
	ret = i->astream->ReadFrames(buffer, buffer_size, numsamples, samples_read, bytes_read);
	if(samples_read<=0)
	    return;
	ringbuffer_put(&i->audio_buffer, buffer, bytes_read);
    }
    ringbuffer_read(&i->audio_buffer, buffer, buffer_size);
}
int videoreader_avifile_getsamples(videoreader_t* v, void*buffer, int num)
{
    if(verbose) {
	printf("videoreader_getsamples(%d)\n", num);fflush(stdout);
    }
    videoreader_avifile_internal*i = (videoreader_avifile_internal*)v->internal;
    if(i->soundbits == 8) {
	readSamples(i, buffer, num/2, num/(v->channels*2));
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
	readSamples(i, buffer, num, num/(v->channels*2));
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
	if(verbose) printf("vstream->ReadFrame() returned value < 0, shutting down...\n");
	i->eof = 1;
	return 0;
    }
    CImage*img2 = 0;
    CImage*img = i->vstream->GetFrame();
    if(!img) {
	if(verbose) printf("vstream->GetFrame() returned NULL, shutting down...\n");
	i->eof = 1;
	return 0;
    }
    /* we convert the image to YUV first, because we can convert to RGB from YUV only */
    img->ToYUV();
    img->ToRGB();
    if(img->Bpp() != 3) {
	/* TODO: this doesn't work yet */
	if(verbose) printf("Can't handle Bpp %d, shutting down...\n", img->Bpp());
	return 0;
	BitmapInfo tmp(v->width, v->height, 24);
	img2 = new CImage(img, &tmp);
	img = img2;
    }


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
	if(img2) delete img2;
	return v->width*v->height*4;
    } else {
	if(img2) delete img2;
	if(verbose) printf("Can't handle bpp %d, shutting down...\n", bpp);
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
    if(i->do_audio) {
	ringbuffer_clear(&i->audio_buffer);
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
    if(i->do_video)
    {
	StreamInfo*videoinfo;
	videoinfo = i->vstream->GetStreamInfo();
	v->width = videoinfo->GetVideoWidth();
	v->height = videoinfo->GetVideoHeight();
	v->fps = (double)(videoinfo->GetFps());
    }
    if(i->do_audio)
    {
	WAVEFORMATEX wave;
	StreamInfo*audioinfo;

	i->astream->GetAudioFormatInfo(&wave,0);
	audioinfo = i->astream->GetStreamInfo();

	v->channels = wave.nChannels;
	v->rate = wave.nSamplesPerSec;
	i->soundbits = wave.wBitsPerSample;

	if(v->channels==0 || v->rate==0 || i->soundbits==0 || wave.wFormatTag!=1) {
	    v->rate = audioinfo->GetAudioSamplesPerSec();
	    v->channels = audioinfo->GetAudioChannels();
	    i->soundbits = audioinfo->GetAudioBitsPerSample();
	}

	if(verbose) {
	    printf("formatinfo: format %d, %d channels, %d bits/sample, rate %d, blockalign %d\n", wave.wFormatTag, wave.nChannels, wave.wBitsPerSample, wave.nSamplesPerSec, wave.nBlockAlign);
	    printf("audioinfo: %d channels, %d bits/sample, rate %d\n", audioinfo->GetAudioChannels(), audioinfo->GetAudioBitsPerSample(), audioinfo->GetAudioSamplesPerSec());
	}
	if(i->soundbits != 8 && i->soundbits != 16) {
	    printf("Can't handle %d bit audio, disabling sound\n", wave.wBitsPerSample);
	    i->do_audio = 0;
	    i->soundbits = 0;
	    v->channels = 0;
	    v->rate = 0;
	}
    }
#endif
    i->vstream -> StartStreaming();
    if(i->do_audio) {
	i->astream -> StartStreaming();
	ringbuffer_init(&i->audio_buffer);
#ifdef VERSION6
	WAVEFORMATEX wave;
	i->astream -> GetOutputFormat(&wave, sizeof(wave));
	printf("formatinfo: format %d, %d channels, %d bits/sample, rate %d, blockalign %d\n", wave.wFormatTag, wave.nChannels, wave.wBitsPerSample, wave.nSamplesPerSec, wave.nBlockAlign);
#endif
    }

    return 1;
}

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
    if(!filename)
	exit(0);
    if(keyframe_interval<0) {
	if(flashversion>=6)
	    keyframe_interval=200;
	else
	    keyframe_interval=5;
    }
    
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
    v2swf_setparameter(&v2swf, "mp3_bitrate", itoa(mp3_bitrate));
    v2swf_setparameter(&v2swf, "samplerate", itoa(samplerate));
    //v2swf_setparameter(&v2swf, "fixheader", "1");
    //v2swf_setparameter(&v2swf, "framerate", "15");
    v2swf_setparameter(&v2swf, "scale", ftoa(scale));
    v2swf_setparameter(&v2swf, "prescale", "1");
    v2swf_setparameter(&v2swf, "flash_version", itoa(flashversion));
    v2swf_setparameter(&v2swf, "keyframe_interval", itoa(keyframe_interval));
    if(expensive)
	v2swf_setparameter(&v2swf, "motioncompensation", "1");

    if(!verbose)
	printf("\n");

    if(audio_adjust>0) {
	int num = ((int)(audio_adjust*video.rate))*video.channels*2;
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
	    video.getsamples(&video, buf, (int)((video.rate/video.fps)*video.channels*2));
	    if(!verbose) {
		printf("\rSkipping frame %d", frameno);fflush(stdout);
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
	    printf("\rConverting frame %d", frameno);fflush(stdout);
	}
    }
    if(!verbose)
	printf("\n");
    fclose(fi);
    v2swf_backpatch(&v2swf, outputfilename);
    v2swf_close(&v2swf);
}

