/* videoreader_dummy.cc
   Simple example for a videoreader.

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

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "videoreader.h"

typedef struct _my_internal
{
    int pos;
    int len; //frames
} my_internal;

typedef struct _RGBA
{ unsigned char a;
  unsigned char r;
  unsigned char g;
  unsigned char b;
} RGBA;

int my_getsamples(videoreader_t* v, void*buffer, int num)
{
    /* generate audio data */
    my_internal*i = (my_internal*)v->internal;
    // printf("request for %d samples\n", num);
    
    return 0; // no audio
}

int my_getimage(videoreader_t* v, void*buffer)
{
    /* generate video frame */
    my_internal*i = (my_internal*)v->internal;
    RGBA*buf = (RGBA*)buffer;

    if(i->pos>=i->len)
	return 0; /* end of video */

    i->pos++; // next frame
    
    int x,y;
    /* generate a simple color gradient */
    for(y=0;y<v->height;y++)
    for(x=0;x<v->width;x++) {
	int r,g,b;
	r = x;
	g = y;
	b = x+y;
	buf[y*v->width + x].r = r; 
	buf[y*v->width + x].g = g;
	buf[y*v->width + x].b = b;
	buf[y*v->width + x].a = 255;
    }
    return v->width*v->height*4;
}
void my_close(videoreader_t* v)
{
    my_internal*i = (my_internal*)v->internal;
    free(v->internal);v->internal = 0;
}
void my_setparameter(videoreader_t*v, char*name, char*value)
{
}
int videoreader_dummy_open(videoreader_t* v, char* filename)
{
    my_internal* i;
    i = (my_internal*)malloc(sizeof(my_internal));
    memset(i, 0, sizeof(my_internal));
    v->internal = i;
    v->getsamples = my_getsamples;
    v->close = my_close;
    v->getimage = my_getimage;
    v->getsamples = my_getsamples;
    v->setparameter = my_setparameter;

    i->len = 2000; //number of frames

    v->width = 320; // video
    v->height = 200;
    v->fps = 15;
    
    v->channels = 0; // no audio
    v->samplerate = 0;
    
    return 0;
}
