/* swfoutput.h
   Header file for swfoutput.cc (and swfoutput_x11.cc).

   Part of the swftools package.

   Copyright (c) 2001,2002,2003 Matthias Kramm <kramm@quiss.org> 
 
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

#ifndef __gfxdevice_rescale_h__
#define __gfxdevice_rescale_h__

#include "../gfxdevice.h"

#ifdef __cplusplus
extern "C" {
#endif

/* either:
    width,height = desired output size
   or:
    scale = desired zoom factor
*/
void gfxdevice_rescale_init(gfxdevice_t*dev, gfxdevice_t*out, int width, int height, double scale);
gfxdevice_t* gfxdevice_rescale_new(gfxdevice_t*out, int width, int height, double scale);

void gfxdevice_rescale_setzoom(gfxdevice_t*dev, double scale);
void gfxdevice_rescale_setdevice(gfxdevice_t*dev, gfxdevice_t*out);


#ifdef __cplusplus
}
#endif

#endif //__gfxdevice_rescale_h__
