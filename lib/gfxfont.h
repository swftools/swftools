/* swffont.h

   Functions for loading external fonts. Header file
   of swffont.c

   Part of the swftools package.

   Copyright (c) 2005 Matthias Kramm
 
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

#ifndef __gfxfont_h__
#define __gfxfont_h__

#include "gfxdevice.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The quality parameter is given by 1 divided by the zoom factor at which the font
   still looks good. 1=normal, 0.5=double resolution etc. */
gfxfont_t* gfxfont_load(const char*id, const char*filename, unsigned int flags, double quality);
void gfxfont_save(gfxfont_t*font, const char*filename);
void gfxfont_save_eot(gfxfont_t*font, const char*filename);
void gfxfont_fix_unicode(gfxfont_t*font, char remove_duplicates);
void gfxfont_free(gfxfont_t*font);
void gfxfont_add_unicode2glyph(gfxfont_t*font);

#ifdef __cplusplus
}
#endif

#endif //__gfxfont_h__
