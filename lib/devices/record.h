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

#ifndef __record_h__
#define __record_h__

#include "../gfxdevice.h"
#include "../gfxtools.h"


#ifdef __cplusplus
extern "C" {
#endif

void gfxdevice_record_init(gfxdevice_t*, char use_tempfile);

gfxdevice_t* gfxdevice_record_new(char*filename);

void gfxdevice_record_flush(gfxdevice_t*, gfxdevice_t*, gfxfontlist_t**);

void gfxresult_record_replay(gfxresult_t*, gfxdevice_t*, gfxfontlist_t**);

#ifdef __cplusplus
}
#endif

#endif //__record_h__
