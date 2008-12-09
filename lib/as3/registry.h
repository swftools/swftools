/* registry.h

   Routines for compiling Flash2 AVM2 ABC Actionscript

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2008 Matthias Kramm <kramm@quiss.org>
 
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

#ifndef __abc_registry_h__
#define __abc_registry_h__
multiname_t* registry_findclass(const char*package, const char*name);
multiname_t* registry_getanytype();
multiname_t* registry_getobjectclass();
multiname_t* registry_getnumberclass();
multiname_t* registry_getstringclass();
multiname_t* registry_getintclass();
multiname_t* registry_getuintclass();
multiname_t* registry_getnullclass();
multiname_t* registry_getbooleanclass();

multiname_t* registry_getsuperclass(multiname_t*m);

multiname_t* registry_getMovieClip();
#endif
