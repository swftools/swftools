/*  v2swf.h 
    header file for v2swf.h - part of SWFTools

    Copyright (C) 2003 Matthias Kramm <kramm@quiss.org>

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

#ifndef __video_h__
#define __video_h__

#ifdef __cplusplus
extern "C" {
#else
typedef unsigned char bool;
#endif

typedef struct _videoreader_t
{
    void*internal;

    /* video */
    int width;
    int height;
    double fps;

    /* audio */
    int channels;
    int rate;

    int (*getsamples) (struct _videoreader_t*, void*buffer, int num);
    /* buffer must be big enough to hold width*height*4 bytes: */
    int (*getimage) (struct _videoreader_t*, void*buffer);
    bool (*eof) (struct _videoreader_t*);
    /* multi purpose functions */
    void (*setparameter) (struct _videoreader_t*, char*name, char*value);
    void* (*getinfo) (struct _videoreader_t*, char*name);
    void (*close) (struct _videoreader_t*);
} videoreader_t;

#define videoreader_getsamples(v, buffer, num) ((v)->getsamples((v),(buffer),(num)))
#define videoreader_getimage(v, buffer) ((v)->getimage((v),(buffer)))
#define videoreader_eof(v) ((v)->eof(v))
#define videoreader_setparameter(v,name,value) ((v)->setparameter((v),(name),(value)))
#define videoreader_getinfo(v,name) ((v)->getinfo((v),(name)))
#define videoreader_close(v) ((v)->close(v))

typedef struct _v2swf_t
{
    void*internal;
} v2swf_t;

int v2swf_init(v2swf_t*v2swf, videoreader_t * video);
int v2swf_read(v2swf_t*v2swf, void*buffer, int len);
void v2swf_setparameter(v2swf_t*v2swf, char*name, char*value);
void v2swf_close(v2swf_t*v2swf);
void v2swf_backpatch(v2swf_t*v2swf, char*filename);

#ifdef __cplusplus
}
#endif

#endif
