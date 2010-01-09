/* wav.c
   Routines for handling .wav files

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
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
#include <string.h>
#include "wav.h"

struct WAVBlock {
    char id[5];
    unsigned int size;
};

int getWAVBlock(FILE*fi, struct WAVBlock*block)
{
    unsigned int size;
    unsigned char b[4];
    if(fread(block->id,1,4,fi)<4)
	return 0;
    block->id[4] = 0;
    if(fread(b,1,4,fi)<4)
	return 0;
    block->size = b[0]|b[1]<<8|b[2]<<16|b[3]<<24;
    /*printf("Chunk: [%c%c%c%c] (%d bytes)\n", 
	    block->id[0],block->id[1],
	    block->id[2],block->id[3],
	    block->size);*/
    return 1;
}

int wav_read(struct WAV*wav, const char* filename)
{
    FILE*fi = fopen(filename, "rb");
    unsigned char b[16];
    long int filesize;
    struct WAVBlock block;
    long int pos;
    
    if(!fi)
        return 0;
    fseek(fi, 0, SEEK_END);
    filesize = ftell(fi);
    fseek(fi, 0, SEEK_SET);

    //printf("Filesize: %d\n", filesize);

    if(!getWAVBlock (fi, &block))
    {
        fclose(fi);
        return 0;
    }
    if(strncmp(block.id,"RIFF",4))
    {
        fprintf(stderr, "wav_read: not a WAV file\n");
        fclose(fi);
        return 0;
    }
    if(block.size + 8 < filesize)
        fprintf(stderr, "wav_read: warning - more tags (%lu extra bytes)\n", filesize - block.size - 8);

    if(block.size == filesize)
	/* some buggy software doesn't generate the right tag length */
        block.size = filesize - 8;

    if(block.size + 8 > filesize)
        fprintf(stderr, "wav_read: warning - short file (%lu bytes missing)\n", block.size + 8 -  filesize);
    if(fread(b, 1, 4, fi) < 4)
    {
        fclose(fi);
		return 0;
    }
    if(strncmp((const char*)b, "WAVE", 4))
    {
        fprintf(stderr, "wav_read: not a WAV file (2)\n");
        fclose(fi);
		return 0;
	}
 
    do
    {
        getWAVBlock(fi, &block);
        pos = ftell(fi);
        if(!strncmp(block.id, "fmt ", 4))
        {
            if(fread(&b, 1, 16, fi)<16)
            {
                fclose(fi);
                return 0;
            }
            wav->tag = b[0]|b[1]<<8;
            wav->channels = b[2]|b[3]<<8;
            wav->sampsPerSec = b[4]|b[5]<<8|b[6]<<16|b[7]<<24;
            wav->bytesPerSec = b[8]|b[9]<<8|b[10]<<16|b[11]<<24;
            wav->align = b[12]|b[13]<<8;
            wav->bps = b[14]|b[15]<<8;
        }
        else
            if (!strncmp(block.id, "LIST", 4))
            {
        // subchunk ICMT (comment) may exist
            }
            else
                if (!strncmp(block.id, "data", 4))
                {
                    int l;
                    wav->data = (unsigned char*)malloc(block.size);
                    if(!wav->data)
                    {
                        fprintf(stderr, "Out of memory (%d bytes needed)", block.size);
                        fclose(fi);
                        return 0;
                    }
                    l = fread(wav->data, 1, block.size, fi);
                    if(l<=0) {
                        fprintf(stderr, "Error: Couldn't read WAV data block\n");
                        fclose(fi);
                        return 0;
                    } else if(l < block.size)
                    {
                        fprintf(stderr, "Warning: data block of size %d is only %d bytes (%d bytes missing)\n", block.size, l, block.size-l);
                        wav->size = l;
                    } else {
                        wav->size = block.size;
                    }
                }
                pos+=block.size;
                fseek(fi, pos, SEEK_SET);
    }
    while (pos < filesize);
    fclose(fi);
    return 1;
}

int wav_write(struct WAV*wav, const char*filename)
{
    FILE*fi = fopen(filename, "wb");
    char*b="RIFFWAVEfmt \x10\0\0\0data";
    char c[16];
    unsigned long int w32;
    if(!fi)
	return 0;
    fwrite(b, 4, 1, fi);
    w32=(/*fmt*/8+0x10+/*data*/8+wav->size);
    c[0] = w32;
    c[1] = w32>>8;
    c[2] = w32>>16;
    c[3] = w32>>24;
    fwrite(c, 4, 1, fi);
    fwrite(&b[4], 12, 1, fi);
    c[0] = wav->tag;
    c[1] = wav->tag>>8;
    c[2] = wav->channels;
    c[3] = wav->channels>>8;
    c[4] = wav->sampsPerSec;
    c[5] = wav->sampsPerSec>>8;
    c[6] = wav->sampsPerSec>>16;
    c[7] = wav->sampsPerSec>>24;
    c[8] = wav->bytesPerSec;
    c[9] = wav->bytesPerSec>>8;
    c[10] = wav->bytesPerSec>>16;
    c[11] = wav->bytesPerSec>>24;
    c[12] = wav->align;
    c[13] = wav->align>>8;
    c[14] = wav->bps;
    c[15] = wav->bps>>8;
    fwrite(c, 16, 1, fi);
    fwrite(&b[16], 4, 1, fi);
    c[0] = wav->size;
    c[1] = wav->size>>8;
    c[2] = wav->size>>16;
    c[3] = wav->size>>24;
    fwrite(c,4,1,fi);
    printf("writing %d converted bytes\n", wav->size);
    fwrite(wav->data,wav->size,1,fi);
    fclose(fi);
    return 1;
}

void wav_print(struct WAV*wav)
{
    printf("tag:%04x channels:%d samples/sec:%lu bytes/sec:%lu align:%d bits/sample:%d size:%d\n",
	    wav->tag, wav->channels, wav->sampsPerSec, wav->bytesPerSec, 
	    wav->align, wav->bps, wav->size);
}

int wav_convert2mono(struct WAV*src, struct WAV*dest, int rate)
{
    int samplelen=src->size/src->align;
    int bps=src->bps;
    double ratio;
    double pos = 0;
    int pos2 = 0;
    int channels=src->channels;
    int i;
    int fill;

    dest->sampsPerSec = rate;
    dest->bps = 16;
    dest->channels = 1;
    dest->align = 2;
    dest->tag = src->tag;
    dest->bytesPerSec = dest->sampsPerSec*dest->align;

    ratio = (double)dest->sampsPerSec/(double)src->sampsPerSec;
    fill = (int)(ratio+1)*2;
    
    dest->data = (unsigned char*)malloc((int)(samplelen*ratio*2)+128);
    if(!dest->data) 
	return 0;
    dest->size = (int)(samplelen*ratio)*2;

    if(bps == 8) {
	if(ratio <= 1) {
	    for(i=0; i<src->size; i+=channels) {
		int pos2 = ((int)pos)*2;
		dest->data[pos2] = 0;
		dest->data[pos2+1] = src->data[i]+128;
		pos += ratio;
	    }
	} else {
	    for(i=0; i<src->size; i+=channels) {
		int j;
		int pos2 = ((int)pos)*2;
		for(j=0;j<fill;j+=2) {
		    dest->data[pos2+j+0] = 0;
		    dest->data[pos2+j+1] = src->data[i]+128;
		}
		pos += ratio;
	    }
	}
    } else if(bps == 16) {
	if(ratio <= 1) {
	    for(i=0; i<src->size/2; i+=channels) {
		int pos2 = ((int)pos)*2;
		dest->data[pos2+0]=src->data[i*2];
		dest->data[pos2+1]=src->data[i*2+1];
		pos += ratio;
	    }
	} else {
	    for(i=0; i<src->size/2; i+=channels) {
		int j;
		int pos2 = ((int)pos)*2;
		for(j=0;j<fill;j+=2) {
		    dest->data[pos2+j+0] = src->data[i*2];
		    dest->data[pos2+j+1] = src->data[i*2+1];
		}
		pos += ratio;
	    }
	}
    } else if(bps == 32) {
	if(ratio <= 1) {
	    for(i=0; i<src->size/4; i+=channels) {
		int pos2 = ((int)pos)*2;
		dest->data[pos2+0]=src->data[i*4+2];
		dest->data[pos2+1]=src->data[i*4+3];
		pos += ratio;
	    }
	} else {
	    for(i=0; i<src->size/4; i+=channels) {
		int j;
		int pos2 = ((int)pos)*2;
		for(j=0;j<fill;j+=2) {
		    dest->data[pos2+j+0] = src->data[i*4+2];
		    dest->data[pos2+j+1] = src->data[i*4+3];
		}
		pos += ratio;
	    }
	}
    } else {
	fprintf(stderr, "Unsupported bitspersample value: %d\n", bps);
    }
    return 1;
}


