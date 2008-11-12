/* mp3.c
   Routines for handling .mp3 files

   Part of the swftools package.
   
   Copyright (c) 2005 Joel Yliluoma <bisqwit@iki.fi>
 
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

#include "mp3.h"

struct MP3Frame
{
    unsigned bitrate;
    unsigned samprate;
    unsigned chanmode;
    unsigned framesize;
    unsigned char* data;
    struct MP3Frame* next;
};

//                                    0          4           8               C
static const unsigned BR_mpeg1[16] = {0,32,40,48,56,64,80,96,112,128,160,192,224,256,320,0};
static const unsigned BR_mpeg2[16] = {0,8, 16,24,32,40,48,56, 64, 80, 96,112,128,144,160,0};
static const unsigned BR_reserved[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
static const unsigned*const BR[4] = {BR_mpeg2, BR_reserved, BR_mpeg2, BR_mpeg1};

static const unsigned SR_mpeg1[4] = {44100,48000,32000,0};
static const unsigned SR_mpeg2[4] = {22050,24000,16000,0};
static const unsigned SR_mpeg25[4] = {11025,12000,8000,0};
static const unsigned SR_reserved[4] = {0,0,0,0};
static const unsigned*const SR[4] = {SR_mpeg25, SR_reserved, SR_mpeg2, SR_mpeg1};

int mp3_read(struct MP3*mp3, const char* filename)
{
    struct MP3Frame* root = 0;
    struct MP3Frame** cur = &root;
    
    unsigned totalsize      = 0;
    unsigned first_samprate = 0;
    unsigned nframes = 0;
    int first_chanmode = -1;

    FILE*fi = fopen(filename, "rb");
    if(!fi) return 0;
    
    for(;;)
    {
        unsigned char FrameBuf[2048];
        unsigned char* hdr = FrameBuf;
        unsigned char* data = FrameBuf+4;
        unsigned char* frdata;

        unsigned char mpegver;
        unsigned padding;
        unsigned bitrate;
        unsigned samprate;
        unsigned framesize;
        int chanmode;

        if(fread(hdr,1,4,fi) < 4) break;

        if(hdr[0] == 'I' && hdr[1] == 'D' && hdr[2] == '3')
        {
            /* Skip ID3 header */
            unsigned id3_size = 0;
            if(fread(FrameBuf, 1, 6, fi) < 6) break;
 
            id3_size = (FrameBuf[5])
                     + (FrameBuf[4] << 7)
                     + (FrameBuf[3] << 14)
                     + (FrameBuf[2] << 21);
            fprintf(stderr, "readMP3: skipping ID3 tag (10+%u bytes)\n", id3_size);
            if(fseek(fi, id3_size, SEEK_CUR) < 0)
            {
                /* Cannot seek? Try reading. */
                char* tmpbuf = (char*)malloc(id3_size);
                int nread=0;
                if(!tmpbuf)
                {
                    fprintf(stderr, "readMP3: fseek and malloc both failed?\n");
                    break;
                }
                nread = fread(tmpbuf, 1, id3_size, fi);
                free(tmpbuf);
                if(nread < id3_size) break;
            }
            continue;
        }


        if(hdr[0] != 0xFF
        || (hdr[1] & 0xE0) != 0xE0)
        {
            fprintf(stderr, "readMP3: invalid header %02X %02X %02X %02X\n",
                hdr[0],hdr[1],hdr[2],hdr[3]);
            break;
        }
        
        mpegver = (hdr[1] >> 3) & 0x03;
        
        bitrate  = BR[mpegver][ (hdr[2] >> 4) & 0x0F ] * 1000;
        samprate = SR[mpegver][ (hdr[2] >> 2) & 0x03 ];
        chanmode = (hdr[3] >> 6) & 0x03;
        
        padding = (hdr[2] & 2) ? 1 : 0;
        
        if(!bitrate || !samprate)
        {
            /* Invalid frame */
            /*break;*/
        }
        if(!first_samprate) first_samprate = samprate;
        else if(first_samprate != samprate)
        {
            /* Sampling rate changed?!? */
            fprintf(stderr, "readMP3: sampling rate changed?\n");
            /*break;*/
        }
        if(first_chanmode<0) first_chanmode = chanmode;
        else if(first_chanmode != chanmode)
        {
            /* Channel mode changed?!? */
            fprintf(stderr, "readMP3: chanmode changed?\n");
            /*break;*/
        }
        
        framesize = ((mpegver == 3 ? 144 : 72) * bitrate) / samprate + padding;
        
/*
        fprintf(stderr, "%02X %02X %02X %02X - bitrate=%u,samprate=%u,chanmode=%u,padding=%u,framesize=%u\n",
            hdr[0],hdr[1],hdr[2],hdr[3],bitrate,samprate,chanmode,padding,framesize);
*/
        if(framesize > sizeof(FrameBuf)) break;
        if(fread(data, 1, framesize - 4, fi) < framesize-4)
        {
            fprintf(stderr, "readMP3: short read at frame %u\n", nframes);
            break;
        }
        
        if(!bitrate || !samprate) continue; 
        
        frdata = (unsigned char*)malloc(framesize);
        if(!frdata)
        {
            fprintf(stderr, "readMP3: malloc failed\n");
            break;
        }
        
        *cur = (struct MP3Frame*)malloc(sizeof(*root));
        if(!*cur)
        {
            fprintf(stderr, "readMP3: malloc failed\n");
            free(frdata);
            break;
        }
        
        (*cur)->next = 0;
        (*cur)->bitrate = bitrate;
        (*cur)->samprate = samprate;
        (*cur)->chanmode = chanmode;
        (*cur)->framesize = framesize;
        (*cur)->data = frdata;
        
        memcpy(frdata, FrameBuf, framesize);
        cur = &(*cur)->next;
        
        totalsize += framesize;
        ++nframes;
    }
    if(!root)
    {
        fprintf(stderr, "readMP3: not a MP3 file\n");
        fclose(fi);
        return 0;
    }
    
    /*
    fprintf(stderr, "readMP3: read %u frames (%u bytes)\n", nframes, totalsize);
    */

    mp3->SampRate = first_samprate;
    mp3->Channels = first_chanmode == 3 ? 1 : 2;
    mp3->NumFrames = nframes;
    mp3->size = totalsize;
    mp3->data = (unsigned char*)malloc(mp3->size);
    if(mp3->data)
    {
        unsigned pos=0;
        struct MP3Frame* it;
        for(it=root; it; it=it->next)
        {
            memcpy(mp3->data + pos, it->data, it->framesize);
            pos += it->framesize;
        }
    }
    else
    {
        fprintf(stderr, "readMP3: malloc failed\n");
    }
    
    while(root)
    {
        struct MP3Frame* next = root->next;
        free(root->data);
        free(root);
        root = next;
    }
    
    fclose(fi);
    return mp3->data != NULL;
}

void mp3_clear(struct MP3*mp3)
{
    free(mp3->data);
    mp3->data = 0;
}


