/* png.h

   Copyright (c) 2003/2004/2005 Matthias Kramm <kramm@quiss.org>

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

#ifndef __png_h__
#define __png_h__

#ifdef __cplusplus
extern "C" {
#endif

int png_apply_filter_32(unsigned char*dest, unsigned char*src, int width, int y);
void png_inverse_filter_32(int mode, unsigned char*src, unsigned char*old, unsigned char*dest, int width);

int getPNG(const char*sname, int*destwidth, int*destheight, unsigned char**destdata);
int getPNGdimensions(const char*sname, int*destwidth, int*destheight);

void savePNG(const char*filename, unsigned char*data, int width, int height, int numcolors);

void writePNG(const char*filename, unsigned char*data, int width, int height);
void writePalettePNG(const char*filename, unsigned char*data, int width, int height);

#ifdef __cplusplus
}
#endif

#endif

