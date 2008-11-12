/* mp3.h
   Header file for mp3.c

   Part of the swftools package.
   
   Copyright (c) 2005 Joel Yliluoma <bisqwit@iki.fi>

   This file is distributed under the GPL, see file COPYING for details */

struct MP3 {
    unsigned short  SampRate;
    unsigned char   Channels;
    unsigned int    NumFrames;
    unsigned char*  data;
    unsigned long   size;
};

int mp3_read(struct MP3*mp3, const char* filename);
void mp3_clear(struct MP3*mp3);
