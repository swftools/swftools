#ifndef __gfximage_h__
#define __gfximage_h__

#include "gfxdevice.h"

gfximage_t*gfximage_new(int width, int height);
void gfximage_save_jpeg(gfximage_t*image, const char*filename, int quality);
void gfximage_save_png(gfximage_t*image, const char*filename);
gfximage_t* gfximage_rescale(gfximage_t*image, int newwidth, int newheight);
void gfximage_free(gfximage_t*b);

#endif
