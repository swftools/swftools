/* swfaction.c

   SWF Sound handling routines
   
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001, 2002 Matthias Kramm <kramm@quiss.org>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#ifndef RFXSWF_DISABLESOUND

#include "../rfxswf.h"

#ifdef BLADEENC
fjokjklj
CodecInitOut * init = 0;
void swf_SetSoundStreamHead(TAG*tag, U16 avgnumsamples)
{
    U8 playbackrate = 3; // 0 = 5.5 Khz, 1 = 11 Khz, 2 = 22 Khz, 3 = 44 Khz
    U8 playbacksize = 1; // 0 = 8 bit, 1 = 16 bit
    U8 playbacktype = 0; // 0 = mono, 1 = stereo
    U8 compression = 2; // 0 = raw, 1 = ADPCM, 2 = mp3
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

    buf = malloc(init->bufferSize);
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
    free(buf);
}
#endif


#ifdef LAME

#include "../lame/lame.h"
    
static lame_global_flags*lame_flags;

void swf_SetSoundStreamHead(TAG*tag, int avgnumsamples)
{
    unsigned char buf[4096];
    int bufsize = 1152*2;
    int len;
    short int samples[1152*2];

    U8 playbackrate = 1; // 0 = 5.5 Khz, 1 = 11 Khz, 2 = 22 Khz, 3 = 44 Khz
    U8 playbacksize = 1; // 0 = 8 bit, 1 = 16 bit
    U8 playbacktype = 0; // 0 = mono, 1 = stereo
    U8 compression = 2; // 0 = raw, 1 = ADPCM, 2 = mp3
    U8 rate = 1; // 0 = 5.5 Khz, 1 = 11 Khz, 2 = 22 Khz, 3 = 44 Khz
    U8 size = 1; // 0 = 8 bit, 1 = 16 bit
    U8 type = 0; // 0 = mono, 1 = stereo
    
    memset(samples,0,sizeof(samples));

    lame_flags = lame_init();

    lame_set_in_samplerate(lame_flags, 44100);
    lame_set_num_channels(lame_flags, 1);
    lame_set_scale(lame_flags, 0);

    // MPEG1    32, 44.1,   48khz
    // MPEG2    16, 22.05,  24
    // MPEG2.5   8, 11.025, 12
    // (not used by decoding routines)
    lame_set_out_samplerate(lame_flags, 11025);

    lame_set_quality(lame_flags, 0);
    lame_set_mode(lame_flags, MONO/*3*/);
    //lame_set_compression_ratio(lame_flags, 11.025);
    lame_set_bWriteVbrTag(lame_flags, 0);

    lame_init_params(lame_flags);
    lame_init_bitstream(lame_flags);

    swf_SetU8(tag,(playbackrate<<2)|(playbacksize<<1)|playbacktype);
    swf_SetU8(tag,(compression<<4)|(rate<<2)|(size<<1)|type);
    swf_SetU16(tag,avgnumsamples);
   
    /* The first two flush calls to lame always fail, for
       some reason. Do them here where they cause no damage. */
    len = lame_encode_flush_nogap(lame_flags, buf, bufsize);
    //printf("init:flush_nogap():%d\n", len);
    len = lame_encode_flush(lame_flags, buf, bufsize);
    //printf("init:flush():%d\n", len);
}

void swf_SetSoundStreamBlock(TAG*tag, S16*samples, char first)
{
    char*buf;
    int oldlen=0,len = 0;
    int bufsize = 16384;
    int numsamples = 1152*2;

    buf = malloc(bufsize);
    if(!buf)
	return;

    if(first) {
	int fs = lame_get_framesize(lame_flags);
	//printf("framesize:%d\n", fs);
	swf_SetU16(tag, fs); // samples per mp3 frame
	swf_SetU16(tag, 0); // seek
    }

    len += lame_encode_buffer(lame_flags, samples, samples, numsamples, &buf[len], bufsize-len);
    //printf("block: %d (+%d)\n", len, len-oldlen);
    oldlen = len;

    len += lame_encode_flush_nogap(lame_flags, &buf[len], bufsize-len);
    //printf("flush: %d (+%d)\n", len, len-oldlen);
    oldlen = len;
    
    swf_SetBlock(tag, buf, len);

   /* len += lame_encode_flush(lame_flags, &buf[len], bufsize-len);
    printf("flush! %d (+%d)\n", len, len-oldlen);*/

    free(buf);
}

void swf_SetSoundStreamEnd(TAG*tag)
{
    lame_close (lame_flags);
}
#endif

#endif // RFXSWF_DISABLESOUND
