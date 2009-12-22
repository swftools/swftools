/* pdfswf.h
   Header file for pdfswf.cc.

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

#ifndef __gfxsource_h__
#define __gfxsource_h__

#include "gfxdevice.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _gfxdocument;
struct _gfxpage;

typedef struct _gfxsource
{
    void                 (*setparameter)(struct _gfxsource*src, const char*name, const char*value);
    struct _gfxdocument* (*open)(struct _gfxsource*src, const char*filename);
    void  (*destroy)(struct _gfxsource*src);
    void*internal;
} gfxsource_t;

typedef struct _gfxdocument
{
    gfxsource_t*driver;
    int num_pages;
    void* (*get)(struct _gfxdocument*gfx, const char*name);
    void  (*destroy)(struct _gfxdocument*gfx);
    void  (*setparameter)(struct _gfxdocument*gfx, const char*name, const char*value);
    char* (*getinfo)(struct _gfxdocument*gfx, const char*key);
    struct _gfxpage* (*getpage)(struct _gfxdocument*gfx, int page);
    void  (*prepare)(struct _gfxdocument*gfx, gfxdevice_t*device);
    void*internal;
} gfxdocument_t;

typedef struct _gfxpage
{
    gfxdocument_t*parent;
    gfxcoord_t width;
    gfxcoord_t height;
    int nr;
    void (*render)(struct _gfxpage*gfx, gfxdevice_t*output);
    void (*rendersection)(struct _gfxpage*gfx, gfxdevice_t*output, gfxcoord_t x, gfxcoord_t y, gfxcoord_t x1, gfxcoord_t y1, gfxcoord_t x2, gfxcoord_t y2);
    void (*destroy)(struct _gfxpage*page);
    void*internal;
} gfxpage_t;

#ifdef __cplusplus
}
#endif

#endif //__gfxsource_h__
