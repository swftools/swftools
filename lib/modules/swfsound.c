/* swfaction.c

   SWF Sound handling routines
   
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001, 2002 Matthias Kramm <kramm@quiss.org>
 
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

#ifndef NO_MP3

#include "../rfxswf.h"

#ifdef BLADEENC
#define HAVE_SOUND

CodecInitOut * init = 0;
void swf_SetSoundStreamHead(TAG*tag, U16 avgnumsamples)
{
    U8 playbackrate = 3; // 0 = 5.5 Khz, 1 = 11 Khz, 2 = 22 Khz, 3 = 44 Khz
    U8 playbacksize = 1; // 0 = 8 bit, 1 = 16 bit
    U8 playbacktype = 0; // 0 = mono, 1 = stereo
    U8 compression = 2; // 0 = raw, 1 = ADPCM, 2 = mp3, 3 = raw le, 6 = nellymoser
    U8 rate = 3; // 0 = 5.5 Khz, 1 = 11 Khz, 2 = 22 Khz, 3 = 44 Khz
    U8 size = 1; // 0 = 8 bit, 1 = 16 bit
    U8 type = 0; // 0 = mono, 1 = stereo

    CodecInitIn params;
    memset(&params, 0, sizeof(params));
    params.frequency = 44100;  //48000, 44100 or 32000
    params.mode = 3;	  //0 = Stereo, 2 = Dual Channel, 3 = Mono
    params.emphasis = 0;  //0 = None, 1 = 50/15 microsec, 3 = CCITT J.17
    params.bitrate = 128;	  //default is 128 (64 for mono)
    init = codecInit(&params);

    swf_SetU8(tag,(playbackrate<<2)|(playbacksize<<1)|playbacktype);
    swf_SetU8(tag,(compression<<4)|(rate<<2)|(size<<1)|type);
    swf_SetU16(tag,avgnumsamples);

    printf("numSamples:%d\n",init->nSamples);
    printf("bufferSize:%d\n",init->bufferSize);
}

void swf_SetSoundStreamBlock(TAG*tag, S16*samples, int numsamples, char first)
{
    char*buf;
    int len = 0;

    buf = rfx_alloc(init->bufferSize);
    if(!buf)
	return;
    
    len = codecEncodeChunk (numsamples, samples, buf);
    len += codecFlush (&buf[len]);
    len += codecExit (&buf[len]);

    if(first) {
	swf_SetU16(tag, numsamples); // number of samples
	swf_SetU16(tag, 0); // seek
    }
    swf_SetBlock(tag, buf, len);
    rfx_free(buf);
}
#endif

void swf_SetSoundDefineRaw(TAG*tag, S16*samples, int numsamples)
{
    swf_SetU8(tag,(/*compression*/0<<4)|(/*rate*/3<<2)|(/*size*/1<<1)|/*mono*/0);
    swf_SetU32(tag, numsamples); // 44100 -> 11025
    swf_SetBlock(tag, (U8*)samples, numsamples*2);
}

/* TODO: find a better way to set these from the outside */

int swf_mp3_in_samplerate = 44100;
int swf_mp3_out_samplerate = 11025;
int swf_mp3_channels = 1;
int swf_mp3_bitrate = 32;

#ifdef HAVE_LAME
#define HAVE_SOUND

#include <stdarg.h>
#include <lame.h>

static lame_global_flags*lame_flags;

void null_errorf(const char *format, va_list ap)
{
}

static void initlame()
{
    unsigned char buf[4096];
    int bufsize = 1152*2;

    lame_flags = lame_init();

    lame_set_in_samplerate(lame_flags, swf_mp3_in_samplerate);
    lame_set_num_channels(lame_flags, swf_mp3_channels);
    lame_set_scale(lame_flags, 0);

    // MPEG1    32, 44.1,   48khz
    // MPEG2    16, 22.05,  24
    // MPEG2.5   8, 11.025, 12
    lame_set_out_samplerate(lame_flags, swf_mp3_out_samplerate);

    lame_set_quality(lame_flags, 0);
    lame_set_mode(lame_flags, MONO/*3*/);
    lame_set_brate(lame_flags, swf_mp3_bitrate);
    //lame_set_compression_ratio(lame_flags, 11.025);
    lame_set_bWriteVbrTag(lame_flags, 0);

    lame_init_params(lame_flags);
    lame_init_bitstream(lame_flags);

    lame_set_errorf(lame_flags, null_errorf);
    /* The first two flush calls to lame always fail, for
       some reason. Do them here where they cause no damage. */
    lame_encode_flush_nogap(lame_flags, buf, bufsize);
    //printf("init:flush_nogap():%d\n", len);
    lame_encode_flush(lame_flags, buf, bufsize);
    //printf("init:flush():%d\n", len);
    lame_set_errorf(lame_flags, 0);
}

void swf_SetSoundStreamHead(TAG*tag, int avgnumsamples)
{
    int len;

    U8 playbackrate = 1; // 0 = 5.5 Khz, 1 = 11 Khz, 2 = 22 Khz, 3 = 44 Khz
    U8 playbacksize = 1; // 0 = 8 bit, 1 = 16 bit
    U8 playbacktype = 0; // 0 = mono, 1 = stereo
    U8 compression = 2; // 0 = raw, 1 = ADPCM, 2 = mp3, 3 = raw le, 6 = nellymoser
    U8 rate = 1; // 0 = 5.5 Khz, 1 = 11 Khz, 2 = 22 Khz, 3 = 44 Khz
    U8 size = 1; // 0 = 8 bit, 1 = 16 bit
    U8 type = 0; // 0 = mono, 1 = stereo

    if(swf_mp3_out_samplerate == 5512) playbackrate = rate = 0; // lame doesn't support this
    else if(swf_mp3_out_samplerate == 11025) playbackrate = rate = 1;
    else if(swf_mp3_out_samplerate == 22050) playbackrate = rate = 2;
    else if(swf_mp3_out_samplerate == 44100) playbackrate = rate = 3;
    else fprintf(stderr, "Invalid samplerate: %d\n", swf_mp3_out_samplerate);
    
    initlame();

    swf_SetU8(tag,(playbackrate<<2)|(playbacksize<<1)|playbacktype);
    swf_SetU8(tag,(compression<<4)|(rate<<2)|(size<<1)|type);
    swf_SetU16(tag,avgnumsamples);
}

void swf_SetSoundStreamBlock(TAG*tag, S16*samples, int seek, char first)
{
    char*buf;
    int len = 0;
    int bufsize = 16384;
    int numsamples = (int)(((swf_mp3_out_samplerate > 22050) ? 1152 : 576) * ((double)swf_mp3_in_samplerate/swf_mp3_out_samplerate));
    int fs = 0;

    buf = rfx_alloc(bufsize);
    if(!buf)
	return;

    if(first) {
	fs = lame_get_framesize(lame_flags);
	swf_SetU16(tag, fs * first); // samples per mp3 frame
	swf_SetU16(tag, seek); // seek
    }

    len += lame_encode_buffer(lame_flags, samples, samples, numsamples, &buf[len], bufsize-len);
    len += lame_encode_flush_nogap(lame_flags, &buf[len], bufsize-len);
    swf_SetBlock(tag, buf, len);
    if(len == 0) {
	fprintf(stderr, "error: mp3 empty block, %d samples, first:%d, framesize:%d\n",
		numsamples, first, fs);
    }/* else {
	fprintf(stderr, "ok: mp3 nonempty block, %d samples, first:%d, framesize:%d\n",
		numsamples, first, fs);
    }*/
    rfx_free(buf);
}

void swf_SetSoundStreamEnd(TAG*tag)
{
    lame_close (lame_flags);
}

void swf_SetSoundDefine(TAG*tag, S16*samples, int num)
{
    char*buf;
    int oldlen=0,len = 0;
    int bufsize = 16384;
    int blocksize = (int)(((swf_mp3_out_samplerate > 22050) ? 1152 : 576) * ((double)swf_mp3_in_samplerate/swf_mp3_out_samplerate));
    int t;
    int blocks;

    U8 compression = 2; // 0 = raw, 1 = ADPCM, 2 = mp3, 3 = raw le, 6 = nellymoser
    U8 rate = 1; // 0 = 5.5 Khz, 1 = 11 Khz, 2 = 22 Khz, 3 = 44 Khz
    U8 size = 1; // 0 = 8 bit, 1 = 16 bit
    U8 type = 0; // 0 = mono, 1 = stereo
    
    if(swf_mp3_out_samplerate == 5512) rate = 0;
    else if(swf_mp3_out_samplerate == 11025) rate = 1;
    else if(swf_mp3_out_samplerate == 22050) rate = 2;
    else if(swf_mp3_out_samplerate == 44100) rate = 3;
    else fprintf(stderr, "Invalid samplerate: %d\n", swf_mp3_out_samplerate);

    blocks = num / (blocksize);

    swf_SetU8(tag,(compression<<4)|(rate<<2)|(size<<1)|type);

    swf_SetU32(tag, (int)(tag,blocks*blocksize / 
	    ((double)swf_mp3_in_samplerate/swf_mp3_out_samplerate)) // account for resampling
	    );

    buf = rfx_alloc(bufsize);
    if(!buf)
	return;

    initlame();

    swf_SetU16(tag, 0); //delayseek
    for(t=0;t<blocks;t++) {
	int s;
	U16*pos;
	pos= &samples[t*blocksize];
	len += lame_encode_buffer(lame_flags, pos, pos, blocksize, &buf[len], bufsize-len);
	len += lame_encode_flush_nogap(lame_flags, &buf[len], bufsize-len);
	swf_SetBlock(tag, buf, len);
	len = 0;
    }

    rfx_free(buf);
}

#endif

#endif

#ifndef HAVE_SOUND

// supply stubs

void swf_SetSoundStreamHead(TAG*tag, int avgnumsamples)
{
    fprintf(stderr, "Error: no mp3 soundstream support compiled in.\n");exit(1);
}
void swf_SetSoundStreamBlock(TAG*tag, S16*samples, int seek, char first)
{
    fprintf(stderr, "Error: no mp3 soundstream support compiled in.\n");exit(1);
}
void swf_SetSoundStreamEnd(TAG*tag)
{
    fprintf(stderr, "Error: no mp3 soundstream support compiled in.\n");exit(1);
}
void swf_SetSoundDefine(TAG*tag, S16*samples, int num)
{
    swf_SetSoundDefineRaw(tag, samples,num);
}

#endif

#define SOUNDINFO_STOP 32
#define SOUNDINFO_NOMULTIPLE 16
#define SOUNDINFO_HASENVELOPE 8
#define SOUNDINFO_HASLOOPS 4
#define SOUNDINFO_HASOUTPOINT 2
#define SOUNDINFO_HASINPOINT 1


void swf_SetSoundInfo(TAG*tag, SOUNDINFO*info)
{
    U8 flags = (info->stop?SOUNDINFO_STOP:0)
	      |(info->nomultiple?SOUNDINFO_NOMULTIPLE:0)
	      |(info->envelopes?SOUNDINFO_HASENVELOPE:0)
	      |(info->loops?SOUNDINFO_HASLOOPS:0)
	      |(info->outpoint?SOUNDINFO_HASOUTPOINT:0)
	      |(info->inpoint?SOUNDINFO_HASINPOINT:0);
    swf_SetU8(tag, flags);
    if(flags&SOUNDINFO_HASINPOINT)
	swf_SetU32(tag, info->inpoint);
    if(flags&SOUNDINFO_HASOUTPOINT)
	swf_SetU32(tag, info->outpoint);
    if(flags&SOUNDINFO_HASLOOPS)
	swf_SetU16(tag, info->loops);
    if(flags&SOUNDINFO_HASENVELOPE) {
	int t;
	swf_SetU8(tag, info->envelopes);
	for(t=0;t<info->envelopes;t++) {
	    swf_SetU32(tag, info->pos[t]);
	    swf_SetU16(tag, info->left[t]);
	    swf_SetU16(tag, info->right[t]);
	}
    }
}


void swf_SetSoundDefineMP3(TAG*tag, U8* data, unsigned length,
                           unsigned SampRate,
                           unsigned Channels,
                           unsigned NumFrames)
{
    U8 compression = 2; // 0 = raw, 1 = ADPCM, 2 = mp3, 3 = raw le, 6 = nellymoser
    U8 rate;     // 0 = 5.5 Khz, 1 = 11 Khz, 2 = 22 Khz, 3 = 44 Khz
    U8 size = 1; // 0 = 8 bit, 1 = 16 bit
    U8 type = Channels==2; // 0=mono, 1=stereo
    
    rate = (SampRate >= 40000) ? 3
         : (SampRate >= 19000) ? 2
         : (SampRate >= 8000) ? 1
         : 0;

    swf_SetU8(tag,(compression<<4)|(rate<<2)|(size<<1)|type);

    swf_SetU32(tag, NumFrames * 576);

    swf_SetU16(tag, 0); //delayseek
    swf_SetBlock(tag, data, length);
}
