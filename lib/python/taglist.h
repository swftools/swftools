/* taglist.h

   Python wrapper for librfxswf- taglist handlers, header file

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

#ifndef __taglist_h__
#define __taglist_h__

#include <Python.h>
#undef HAVE_STAT
#include "../rfxswf.h"

#include "./tagmap.h"

extern PyTypeObject TagListClass;

PyObject * taglist_new();

/* warning: will modify tag order*/
PyObject * taglist_new2(TAG*tag);

TAG* taglist_getTAGs(PyObject*);
/* for sprites: */
TAG* taglist_getTAGs2(PyObject*self, PyObject*tagmap, int addDependencies);

PyMethodDef* taglist_getMethods();
#endif
