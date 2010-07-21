/* gfxfilter.h

   Part of the swftools package.

   Copyright (c) 2010 Matthias Kramm <kramm@quiss.org> 
 
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

#ifndef __gfxfilter_h__
#define __gfxfilter_h__

#include "gfxdevice.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {gfxfilter_none, gfxfilter_onepass, gfxfilter_twopass} gfxfiltertype_t;

typedef struct _gfxfilterbase
{
    gfxfiltertype_t type;
} gfxfilterbase_t;

typedef struct _gfxfilter
{
    gfxfiltertype_t type;
    int num_passes;
    const char*name;

    int pass;

    int (*setparameter)(struct _gfxfilter*in, const char*key, const char*value, struct _gfxdevice*out);
    void (*startpage)(struct _gfxfilter*in, int width, int height, struct _gfxdevice*out);
    void (*startclip)(struct _gfxfilter*in, gfxline_t*line, struct _gfxdevice*out);
    void (*endclip)(struct _gfxfilter*in, struct _gfxdevice*out);
    void (*stroke)(struct _gfxfilter*in, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit, struct _gfxdevice*out);
    void (*fill)(struct _gfxfilter*in, gfxline_t*line, gfxcolor_t*color, struct _gfxdevice*out);
    void (*fillbitmap)(struct _gfxfilter*in, gfxline_t*line, gfximage_t*img, gfxmatrix_t*imgcoord2devcoord, gfxcxform_t*cxform, struct _gfxdevice*out); //cxform? tiling?
    void (*fillgradient)(struct _gfxfilter*in, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*gradcoord2devcoord, struct _gfxdevice*out); //?
    void (*addfont)(struct _gfxfilter*in, gfxfont_t*font, struct _gfxdevice*out);
    void (*drawchar)(struct _gfxfilter*in, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix, struct _gfxdevice*out);
    void (*drawlink)(struct _gfxfilter*in, gfxline_t*line, const char*action, const char*text, struct _gfxdevice*out);
    void (*endpage)(struct _gfxfilter*in, struct _gfxdevice*out);
    gfxresult_t* (*finish)(struct _gfxfilter*in, struct _gfxdevice*out);

    void (*dealloc)(struct _gfxfilter*f);

    void* internal;
} gfxfilter_t;

typedef struct _gfxtwopassfilter
{
    gfxfiltertype_t type;
    gfxfilter_t pass1;
    gfxfilter_t pass2;
} gfxtwopassfilter_t;

gfxdevice_t*gfxfilter_apply(gfxfilter_t*filter, gfxdevice_t*dev);
gfxdevice_t*gfxtwopassfilter_apply(gfxtwopassfilter_t*filter, gfxdevice_t*dev);

typedef struct _gfxfilterchain {
    gfxfilterbase_t*filter;
    struct _gfxfilterchain*next;
} gfxfilterchain_t;

gfxfilterchain_t* gfxfilterchain_parse(const char*filterexpr);
gfxdevice_t* gfxfilterchain_apply(gfxfilterchain_t*chain, gfxdevice_t*dev);
void gfxfilterchain_destroy(gfxfilterchain_t*chain);

#define wrap_filter(dev, name, args...) \
    {gfxfilter_t f_##name; \
     gfxfilter_##name##_init(&f_##name, ## args); \
     dev = gfxfilter_apply(&f_##name, dev); \
    }

#define wrap_filter2(dev, name, args...) \
    {gfxtwopassfilter_t f_##name; \
     gfxtwopassfilter_##name##_init(&f_##name, ## args); \
     dev = gfxtwopassfilter_apply(&f_##name, dev); \
    }

/* known filters */
void gfxfilter_maketransparent_init(gfxfilter_t*f, U8 alpha);
void gfxtwopassfilter_remove_font_transforms_init(gfxtwopassfilter_t*f);
void gfxtwopassfilter_one_big_font_init(gfxtwopassfilter_t*f);
void gfxtwopassfilter_vectors_to_glyphs_init(gfxtwopassfilter_t*f);


#ifdef __cplusplus
}
#endif

#endif //__gfxfilter_h__
