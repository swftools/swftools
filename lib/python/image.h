/* image.h

   Python wrapper for librfxswf- image handling

   Part of the swftools package.

   Copyright (c) 2003 Matthias Kramm <kramm@quiss.org>
 
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

#ifndef __image_h__
#define __image_h__

#include <Python.h>
#undef HAVE_STAT
#include "../rfxswf.h"

int image_getWidth(PyObject*image);
int image_getHeight(PyObject*image);
int image_getBPP(PyObject*image);
RGBA* image_toRGBA(PyObject*image);
PyObject* rgba_to_image(RGBA*rgba, int width, int height);

#endif
