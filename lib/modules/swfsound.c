/* swfaction.c

   SWF Sound handling routines
   
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001, 2002 Matthias Kramm <kramm@quiss.org>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#ifndef RFXSWF_DISABLESOUND

#include "../rfxswf.h"

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

    if(!buf)
	return;
    
    buf = malloc(init->bufferSize);
    
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

#endif // RFXSWF_DISABLESOUND
