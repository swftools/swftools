/* mkvideo.c
   Create a video file.

   Part of the swftools package.
   
   Copyright (c) 2003 Matthias Kramm <kramm@quiss.org> */

#ifndef __swfvideo_h__
#define __swfvideo_h__

#include "../rfxswf.h"

typedef struct _YUV
{
    unsigned char y,u,v;
} YUV;

typedef struct _VIDEOSTREAM
{
    int width;
    int height;
    int frame;
    int linex;
    int olinex;
    YUV*oldpic;
    YUV*current;
    int bbx,bby;
    int*mvdx;
    int*mvdy;

    /* modifyable: */
    int do_motion; //enable motion compensation (slow!)

} VIDEOSTREAM;

void swf_SetVideoStreamDefine(TAG*tag, VIDEOSTREAM*stream, U16 frames, U16 width, U16 height);
void swf_SetVideoStreamIFrame(TAG*tag, VIDEOSTREAM*s, RGBA*pic, int quant/* 1-31, 1=best quality, 31=best compression*/);
void swf_SetVideoStreamPFrame(TAG*tag, VIDEOSTREAM*s, RGBA*pic, int quant/* 1-31, 1=best quality, 31=best compression*/);
void swf_VideoStreamClear(VIDEOSTREAM*stream);

#endif
