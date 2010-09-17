/* font_filter.h

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

#ifndef __fontfilter_h__
#define __fontfilter_h__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _fontfilter {
    void*internal;

    /* pass 1 */
    void (*countchar)(struct _fontfilter*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix);
    /* pass 1 -> pass 2*/
    void (*freeze)(struct _fontfilter*f);
    /* pass 2 */
    void (*drawchar)(struct _fontfilter*f, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix, gfxdevice_t*out);
   
    void (*destroy)(struct _fontfilter*f);
} fontfilter_t;

fontfilter_t* fontfilter_remove_font_transforms();

#ifdef __cplusplus
}
#endif

#endif
