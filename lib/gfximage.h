#ifndef __gfximage_h__
#define __gfximage_h__

#include "gfxdevice.h"

void gfximage_save_jpeg(gfximage_t*image, int quality, const char*filename);
void gfximage_save_png(gfximage_t*image, const char*filename);
gfximage_t* gfximage_rescale(gfximage_t*image, int newwidth, int newheight);

#endif
