#ifndef __gfxfilter_h__
#define __gfxfilter_h__

#include "gfxdevice.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _gfxfilter
{
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
    void (*drawlink)(struct _gfxfilter*in, gfxline_t*line, const char*action, struct _gfxdevice*out);
    void (*endpage)(struct _gfxfilter*in, struct _gfxdevice*out);
    gfxresult_t* (*finish)(struct _gfxfilter*in, struct _gfxdevice*out);
    void* internal;
} gfxfilter_t;

typedef struct _gfxtwopassfilter
{
    gfxfilter_t pass1;
    gfxfilter_t pass2;
} gfxtwopassfilter_t;

gfxdevice_t*gfxfilter_apply(gfxfilter_t*filter, gfxdevice_t*dev);
gfxdevice_t*gfxtwopassfilter_apply(gfxtwopassfilter_t*filter, gfxdevice_t*dev);

#ifdef __cplusplus
}
#endif

#endif //__gfxfilter_h__
