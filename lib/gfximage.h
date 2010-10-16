#ifndef __gfximage_h__
#define __gfximage_h__

#include <stdbool.h>
#include "gfxdevice.h"

gfximage_t*gfximage_new(int width, int height);
void gfximage_save_jpeg(gfximage_t*image, const char*filename, int quality);
void gfximage_save_png(gfximage_t*image, const char*filename);
void gfximage_save_png_quick(gfximage_t*image, const char*filename);
gfximage_t* gfximage_rescale(gfximage_t*image, int newwidth, int newheight);
bool gfximage_has_alpha(gfximage_t*image);
void gfximage_free(gfximage_t*b);

#endif
