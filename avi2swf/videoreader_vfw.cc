/* videoreader_vfw.cc
   Read avi files using Video For Windows (vfw).

   Part of the swftools package.
   
   Copyright (c) 2004 Matthias Kramm <kramm@quiss.org>
 
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

#ifdef WIN32
#include <windows.h>
#include <vfw.h>
#include <stdlib.h>
#include <stdio.h>
#include "videoreader.h"

typedef struct _videoreader_vfw_internal { 
    //video:
    PAVISTREAM vs;
    //audio:
    PAVISTREAM as;

    PGETFRAME getframe;
    IAVIFile* avifile;
    BITMAPINFOHEADER bitmap;
    WAVEFORMATEX waveformat;

    int video_pos;
    int video_end;

    int audio_pos;
    int audio_end;

    float fps;
    int width,height;

    int samplerate;
    int channels;

} videoreader_vfw_internal_t;

bool videoreader_vfw_eof(videoreader_t* vr)
{
    videoreader_vfw_internal_t* i = (videoreader_vfw_internal_t*)vr->internal;
    return (i->video_pos >= i->video_end);
}

static int bitmap_to_rgba(BITMAPINFOHEADER*bi, void*buffer, const int dest_width, const int dest_height)
{
    UCHAR*data = (UCHAR*)(bi+1); // actual bitmap data starts after the header

    if(bi->biPlanes!=1 || bi->biCompression!=0 || bi->biBitCount%4!=0) {
	/* unsupported format */
	fprintf(stderr, "bitmap_to_rgba: unsupported format: biPlanes=%d, biCompression=%d biBitCount=%d\n",
		bi->biPlanes, bi->biCompression, bi->biBitCount);
	return 0;
    }
    
    ULONG*dest = (ULONG*)buffer;

    int width = bi->biWidth;
    int height = bi->biHeight;
    if(dest_width != width || dest_height != height) {
	/* TODO: size conversion */
	fprintf(stderr, "size mismatch: %dx%d != %dx%d\n", width, height, dest_width, dest_height);
	return 0;
    }

    /* convert the various image types to RGBA-
       TODO: is there some way to let the Windows API do this? */
    int bytesperpixel = ((bi->biWidth*bi->biBitCount)+7)&~7;
    int linex = ((bytesperpixel/8)+3)&~3;
    memset(dest, 255, dest_width*dest_height*4);//pre-fill alpha channel
    if(bi->biBitCount==1) {
	int y;
	UCHAR*img = data;
	for(y=0;y<dest_height;y++) {
	    UCHAR*line = &img[linex*y];
	    int x;
	    for(x=0;x<dest_width;x++) {
		*dest++ = 255*((line[x/8]>>(x&7))&1);
	    }
	}
    } else if(bi->biBitCount==4) {
	int y;
	UCHAR*img = &data[bi->biClrUsed*4];
	UCHAR*pal = data;
	for(y=0;y<dest_height;y++) {
	    UCHAR*line = &img[linex*y];
	    int x;
	    for(x=0;x<dest_width/2;x++) {
		*dest++ = 255|pal[(line[0]>>4)<<2|0]<<8|pal[(line[0]>>4)<<2|1]<<16|pal[(line[0]>>4)<<2|2]<<24;
		*dest++ = 255|pal[(line[0]&0x0f)<<2|0]<<8|pal[(line[0]&0x0f)<<2|1]<<16|pal[(line[0]&0x0f)<<2|2]<<24;
		line++;
	    }
	}
    } else if(bi->biBitCount==8) {
	int y;
	UCHAR*img = &data[bi->biClrUsed*4];
	UCHAR*pal = data;
	for(y=0;y<dest_height;y++) {
	    UCHAR*line = &img[linex*y];
	    int x;
	    for(x=0;x<dest_width;x++) {
		*dest++ = 255|pal[line[0]*4+2]<<8|pal[line[0]*4+1]<<16|pal[line[0]*4+0]<<24;
		line++;
	    }
	}
    } else if(bi->biBitCount==24) {
	UCHAR*img = data;
	int y;
	for(y=0;y<dest_height;y++) {
	    UCHAR*line = &img[linex*y];
	    int x;
	    for(x=0;x<dest_width;x++) {
		*dest++ = 255|line[2]<<8|line[1]<<16|line[0]<<24;
		line+=3;
	    }
	}
    } else if(bi->biBitCount==32) {
	UCHAR*img = data;
	int y;
	for(y=0;y<dest_height;y++) {
	    UCHAR*line = &img[linex*y];
	    int x;
	    for(x=0;x<dest_width;x++) {
		*dest++ = 255|line[0]<<8|line[1]<<16|line[2]<<24;
		line+=4;
	    }
	}
    } else {
	fprintf(stderr, "Unsupported format: bitcount=%d\n", bi->biBitCount);
	return 0;
    }
    return 1;
}

int videoreader_vfw_getimage(videoreader_t* vr, void*buffer)
{
    videoreader_vfw_internal_t* i = (videoreader_vfw_internal_t*)vr->internal;

    if(videoreader_vfw_eof(vr))
	return 0;

    LPBITMAPINFOHEADER bi;
    bi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(i->getframe, i->video_pos);
	
    i->video_pos++;

    if(!bi) {
	fprintf(stderr, "AVIStreamGetFrame failed\n");
	return 0;
    }
    printf("%dx%d:%d\n", bi->biWidth, bi->biHeight, bi->biBitCount);
    
    if(!bitmap_to_rgba(bi, buffer, i->width, i->height)) {
	fprintf(stderr, "couldn't convert bitmap to RGBA.\n");
	return 0;
    }
    return i->width*i->height*4;
}

static int readAudioBlock(videoreader_vfw_internal_t* i, void*buf, int len)
{
    LONG bytes;
    LONG samples;
    AVIStreamRead(i->as, i->audio_pos, len/(2*i->waveformat.nChannels), buf, len, &bytes, &samples);
    i->audio_pos += samples;
    return bytes;
}

int videoreader_vfw_getsamples(videoreader_t* vr, void*buf, int num)
{
    videoreader_vfw_internal_t* i = (videoreader_vfw_internal_t*)vr->internal;
   
    switch(i->waveformat.wBitsPerSample) {
	case 1: {
	    int len = readAudioBlock(i, buf, num);
	    int t = len-1;
	    do {
		((SHORT*)buf)[t] = ((((BYTE*)buf)[t>>3])>>(t&7))<<15;
	    } while(--t>=0);
	    return len*8;
	}
	case 8: {
	    int len = readAudioBlock(i, buf, num);
	    int t = len-1;
	    do {
		((SHORT*)buf)[t] = (((BYTE*)buf)[t]<<8)^0x8000;
	    } while(--t>=0);
	    return len*2;
	}
	case 16: {
	    return readAudioBlock(i, buf, num);
	}
	default: {
	    return 0;
	}
    }
}

void videoreader_vfw_close(videoreader_t* vr)
{
    videoreader_vfw_internal_t* i = (videoreader_vfw_internal_t*)vr->internal;

    AVIStreamGetFrameClose(i->getframe);
    if(i->vs) {
	AVIStreamRelease(i->vs); i->vs = 0;
    }
    if(i->as) {
	AVIStreamRelease(i->as); i->vs = 0;
    }
    AVIFileRelease(i->avifile); i->avifile = 0;
    
    AVIFileExit(); avifile_initialized=0;

    free(vr->internal); vr->internal = 0;
}

void videoreader_vfw_setparameter(videoreader_t* vr, char*name, char*value) {}

static int avifile_initialized = 0;

int videoreader_vfw_open(videoreader_t* vr, char* filename)
{
    memset(vr, 0, sizeof(videoreader_t));
    if(!filename) {
	/* codec query */
	return 1;
    }

    videoreader_vfw_internal_t* i = (videoreader_vfw_internal_t*)malloc(sizeof(videoreader_vfw_internal_t));
    memset(i, 0, sizeof(videoreader_vfw_internal_t));

    vr->internal = i;
    vr->eof = videoreader_vfw_eof;
    vr->getimage = videoreader_vfw_getimage;
    vr->getsamples = videoreader_vfw_getsamples;
    vr->close = videoreader_vfw_close;
    vr->setparameter = videoreader_vfw_setparameter;

    if(!avifile_initialized) {
	AVIFileInit();
    }
    if(AVIFileOpen(&i->avifile, filename, OF_SHARE_DENY_WRITE, 0)) {
        fprintf(stderr, "Couldn't open %s\n", filename);
        return -1;
    }
    AVIFILEINFO info;
    AVIFileInfo(i->avifile, &info, sizeof(info));
   
    /* calculate framerate */
    i->fps = (double)info.dwRate/(double)info.dwScale;

    unsigned int t;
    while(t<info.dwStreams) {
	PAVISTREAM stream;
        if(AVIFileGetStream(i->avifile, &stream, streamtypeANY, t) != AVIERR_OK || !stream)
	    break; //video_end of (working) streams

	AVISTREAMINFO streaminfo;
        AVIStreamInfo(stream, &streaminfo, sizeof(streaminfo));

        if (streaminfo.fccType == streamtypeVIDEO) {
	    /* video stream */

	    BITMAPINFOHEADER bitmap;
	    LONG size = sizeof(i->bitmap);
	    AVIStreamReadFormat(i->vs, 0, &bitmap, &size);

	    if(i->bitmap.biCompression == 0/*RGB*/) {
		i->bitmap = bitmap;
		i->vs = stream;
		i->width = bitmap.biWidth;
		i->height = bitmap.biHeight;
	    }
        }
        else if (streaminfo.fccType == streamtypeAUDIO) {
	    /* audio stream */

	    WAVEFORMATEX waveformat;
	    LONG size = sizeof(i->waveformat);
	    AVIStreamReadFormat(i->as, 0, &waveformat, &size);

	    if(i->waveformat.wBitsPerSample == 16 || i->waveformat.wBitsPerSample == 8) {
		i->waveformat = waveformat;
		i->as = stream;
		i->channels = i->waveformat.nChannels;
		i->samplerate = i->waveformat.nSamplesPerSec;
	    }
        }
    }

    if(i->vs) {
	vr->width = i->width;
	vr->height = i->height;
	vr->fps = i->fps;
    } else {
	fprintf(stderr, "AVIReader: Warning: No video stream\n");
    }
    if(i->as) {
	vr->channels = i->channels;
	vr->samplerate = i->samplerate;
    } else {
	fprintf(stderr, "AVIReader: Warning: No audio stream\n");
    }
    
    i->getframe = AVIStreamGetFrameOpen(i->vs, 0);
    if(!i->getframe) {
	return -1;
    }
    
    i->video_pos = AVIStreamStart(i->vs);
    i->video_end = AVIStreamEnd(i->vs);
    i->audio_pos = 0;
    i->audio_end = 0x7fffffff;

    return 0;
}

#else //WIN32

int videoreader_vfw_open(videoreader_t* vr, char* filename)
{
    return -1;
}

#endif //WIN32

