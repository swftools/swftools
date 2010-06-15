#ifndef __jpeg_h__
#define __jpeg_h__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int jpeg_save(unsigned char*data, unsigned int width, unsigned int height, int quality, const char*filename);
int jpeg_save_gray(unsigned char*data, unsigned int width, unsigned int height, int quality, const char*filename);
int jpeg_save_to_file(unsigned char*data, unsigned int width, unsigned int height, int quality, FILE*fi);
int jpeg_save_to_mem(unsigned char*data, unsigned int width, unsigned int height, int quality, unsigned char*dest, int destsize);
int jpeg_load(const char*filename, unsigned char**dest, unsigned int*width, unsigned int*height);
int jpeg_load_from_mem(unsigned char*_data, int _size, unsigned char**dest, unsigned int*width, unsigned int*height);
void jpeg_get_size(const char *fname, unsigned int *width, unsigned int *height);

#ifdef __cplusplus
}
#endif

#endif
