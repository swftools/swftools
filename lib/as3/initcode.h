/* initcode.h

   Routines for handling/compiling Flash2 AVM2 ABC Actionscript

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2008,2009 Matthias Kramm <kramm@quiss.org>
 
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

#ifndef __initcode_h__
#define __initcode_h__

#include "abc.h"
#include "registry.h"

DECLARE(parsedclass);
DECLARE_LIST(parsedclass);
struct _parsedclass {
    classinfo_t*cls; 
    dict_t parents;
    dict_t usedclasses_deep;
    dict_t usedclasses;
    abc_class_t*abc;
};

void initcode_add_classlist(abc_script_t*init, parsedclass_list_t*classes);
parsedclass_t* parsedclass_new(classinfo_t*c, abc_class_t*abc);
void parsedclass_add_dependency(parsedclass_t*p, classinfo_t*c);

#endif
