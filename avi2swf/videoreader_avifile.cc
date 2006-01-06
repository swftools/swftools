/* videoreader_avifile.cc
   Read avi files using the avifile library.

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

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "../config.h"
#include "videoreader.h"

#ifdef HAVE_AVIFILE

#undef HAVE_CONFIG_H

#ifdef HAVE_VERSION_H
  #include <version.h>
#endif
#ifdef HAVE_AVIFILE_VERSION_H
  #include <avifile/version.h>
#endif

#if (AVIFILE_MAJOR_VERSION == 0) && (AVIFILE_MINOR_VERSION>=6) 
   #include <avifile.h>
   #include <aviplay.h>
   #include <avm_fourcc.h>
   #include <avm_creators.h>
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

#ifdef HAVE_SIGNAL_H
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#include <signal.h>
#define DO_SIGNALS
#endif
#endif

#include "../lib/q.h"

static int verbose = 0;

typedef struct _videoreader_avifile_internal
{
    IAviReadFile* player;
    IAviReadStream* astream;
    IAviReadStream* vstream;
    int do_audio;
    int do_video;
    int video_eof;
    int audio_eof;
    int flip;
    int frame;
    int soundbits;
    ringbuffer_t audio_buffer;
} videoreader_avifile_internal;

static int readSamples(videoreader_avifile_internal*i, void*buffer, int buffer_size, int numsamples)
{
    int ret;
    while(i->audio_buffer.available < buffer_size) {
	unsigned int samples_read = 0, bytes_read = 0;
	ret = i->astream->ReadFrames(buffer, buffer_size, numsamples, samples_read, bytes_read);
	if(ret!=0) {
	    if(verbose) {
		printf("ReadFrames() returns %d\n", ret);fflush(stdout);
	    }
	}
	if(samples_read<=0) {
	    int l = i->audio_buffer.available; 
	    ringbuffer_read(&i->audio_buffer, buffer, l);
	    return l;
	}
	ringbuffer_put(&i->audio_buffer, buffer, bytes_read);
    }
    ringbuffer_read(&i->audio_buffer, buffer, buffer_size);
    return buffer_size;
}
static int videoreader_avifile_getsamples(videoreader_t* v, void*buffer, int num)
{
    videoreader_avifile_internal*i = (videoreader_avifile_internal*)v->internal;
    if(verbose) {
	printf("videoreader_getsamples(%d)\n", num);fflush(stdout);
    }
    if(i->audio_eof)
	return 0;
    if(i->soundbits == 8) {
	int num_read = readSamples(i, buffer, num/2, num/(v->channels*2))*2;
	unsigned char*b = (unsigned char*)buffer;
	int t;
	for(t=num_read-2;t>=0;t-=2) {
	    unsigned char x = b[t/2];
	    b[t] = 0;
	    b[t+1] = x-128;
	}
	if(!num_read) {
	    if(verbose) {
		printf("end of audio\n");fflush(stdout);
	    }
	    i->audio_eof=1;
	}
	return num_read;
    }
    if(i->soundbits == 16) {
	int num_read = readSamples(i, buffer, num, num/(v->channels*2));
	if(!num_read)  {
	    if(verbose) {
		printf("end of audio\n");fflush(stdout);
	    }
	    i->audio_eof=1;
	}
	return num_read;
    }
    return 0;
}
static int videoreader_avifile_getimage(videoreader_t* v, void*buffer)
{
    videoreader_avifile_internal*i = (videoreader_avifile_internal*)v->internal;
    if(verbose) {
	printf("videoreader_getimage()\n");fflush(stdout);
    }

    if(i->video_eof)
	return 0;

    if(i->vstream->ReadFrame() < 0) {
	if(verbose) printf("vstream->ReadFrame() returned value < 0, shutting down...\n");
	i->video_eof = 1;
	return 0;
    }
    CImage*img2 = 0;
    CImage*img = i->vstream->GetFrame();
    if(!img) {
	if(verbose) printf("vstream->GetFrame() returned NULL, shutting down...\n");
	i->video_eof = 1;
	return 0;
    }
    /* we convert the image to YUV first, because we can convert to RGB from YUV only */
    img->ToYUV();
    img->ToRGB();
    if(img->Bpp() != 3) {
	if(verbose) printf("Warning: converthing from bpp %d to bpp 3, this fails on older avifile versions...\n", img->Bpp());
	BitmapInfo tmp(v->width, v->height, 24);
	img2 = new CImage(img, &tmp);
	img = img2;
    }

    v->frame++;
    i->frame++;
    unsigned char*data = img->Data();
    int bpp = img->Bpp();
    if(bpp == 3) {
	int x,y;
	for(y=0;y<v->height;y++) {
	    unsigned char*from,*to;
	    to = &((unsigned char*)buffer)[y*v->width*4];
	    if(i->flip)
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
	i->video_eof = 1;
	return 0;
    }
}
static void videoreader_avifile_close(videoreader_t* v)
{
    videoreader_avifile_internal*i = (videoreader_avifile_internal*)v->internal;
    if(verbose) {
	printf("videoreader_close()\n");fflush(stdout);
    }
    if(i->do_audio) {
	ringbuffer_clear(&i->audio_buffer);
    }
}
static void videoreader_avifile_setparameter(videoreader_t*v, char*name, char*value)
{
    videoreader_avifile_internal*i = (videoreader_avifile_internal*)v->internal;
    if(!strcmp(name, "verbose")) {
	verbose = atoi(value);
    }
    if(!strcmp(name, "flip")) {
	i->flip = atoi(value);
    }
    if(verbose) {
	printf("videoreader_setparameter(%s, %s)\n", name, value);fflush(stdout);
    }
}

int videoreader_avifile_open(videoreader_t* v, char* filename)
{
    if(!filename) {
	/* codec query */
	return 0;
    }
    videoreader_avifile_internal* i;
    i = (videoreader_avifile_internal*)malloc(sizeof(videoreader_avifile_internal));
    memset(i, 0, sizeof(videoreader_avifile_internal));
    memset(v, 0, sizeof(videoreader_t));
    v->getsamples = videoreader_avifile_getsamples;
    v->close = videoreader_avifile_close;
    v->getimage = videoreader_avifile_getimage;
    v->getsamples = videoreader_avifile_getsamples;
    v->setparameter = videoreader_avifile_setparameter;
    v->internal = i;
    v->frame = 0;
    
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
#ifdef NO_MP3
    if(i->do_audio) {
	printf(stderr, "MP3 support has been disabled at compile time, not converting soundtrack");
	i->do_audio = 0;
    }
#endif

    if(!i->do_video && !i->do_audio) {
	printf("File has neither audio nor video streams.(?)\n");
	return -1;
    }

#ifndef VERSION6
    MainAVIHeader head;
    int dwMicroSecPerFrame = 0;
    player->GetFileHeader(&head);
    printf("fps: %d\n", 1000000/head.dwMicroSecPerFrame);
    printf("frames: %d\n", head.dwTotalFrames);
    printf("streams: %d\n", head.dwStreams);
    printf("width: %d\n", abs(head.dwWidth));
    printf("height: %d\n", abs(head.dwHeight));
    printf("sound: %u samples (%f seconds)\n", i->astream->GetEndPos(), i->astream->GetEndTime());
    v->width = abs(head.dwWidth);
    v->height = abs(head.dwHeight);
    dwMicroSecPerFrame = head.dwMicroSecPerFrame;
    samplesperframe = astream->GetEndPos()/astream->GetEndTime()*head.dwMicroSecPerFrame/1000000;
    v->samplerate = (int)(astream->GetEndPos()/astream->GetEndTime());
    v->fps = 1000000.0/dwMicroSecPerFrame;
    i->soundbits = 16;
#else
    if(i->do_video)
    {
	StreamInfo*videoinfo;
	videoinfo = i->vstream->GetStreamInfo();
	v->width = abs(videoinfo->GetVideoWidth());
	v->height = abs(videoinfo->GetVideoHeight());
	v->fps = (double)(videoinfo->GetFps());
    }
    if(i->do_audio)
    {
	WAVEFORMATEX wave;
	StreamInfo*audioinfo;

	i->astream->GetAudioFormatInfo(&wave,0);
	audioinfo = i->astream->GetStreamInfo();

	v->channels = wave.nChannels;
	v->samplerate = wave.nSamplesPerSec;
	i->soundbits = wave.wBitsPerSample;

	if(v->channels==0 || v->samplerate==0 || i->soundbits==0 || wave.wFormatTag!=1) {
	    v->samplerate = audioinfo->GetAudioSamplesPerSec();
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
	    v->samplerate = 0;
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

    return 0;
}

#else  //HAVE_AVIFILE

int videoreader_avifile_open(videoreader_t* v, char* filename)
{
    return -1;
}

#endif //HAVE_AVIFILE
