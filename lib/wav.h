/* wav.h
   Header file for wav.c

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

struct WAV {
    unsigned short      tag;
    unsigned short      channels;
    unsigned long       sampsPerSec;
    unsigned long       bytesPerSec;
    unsigned short      align;
    unsigned short      bps;
    
    unsigned char*data;
    unsigned int size;
};

int readWAV(char* filename, struct WAV*wav);
int writeWAV(char*filename, struct WAV*wav);
void printWAVInfo(struct WAV*wav);
int convertWAV2mono(struct WAV*src, struct WAV*dest, int rate);

