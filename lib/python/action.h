/* action.h

   Python wrapper for librfxswf- actionscript stuff (header)

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

#ifndef __action_h__
#define __action_h__

#include <Python.h>
#undef HAVE_STAT
#include "../rfxswf.h"

extern PyTypeObject ActionClass;
PyObject* f_Action(PyObject* self, PyObject* args, PyObject* kwargs);
ActionTAG* action_getAction(PyObject* self);

PyMethodDef* action_getMethods();
#endif
