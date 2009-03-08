/* import.h

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2009 Matthias Kramm <kramm@quiss.org>
 
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

#ifndef __as3_import_h__
#define __as3_import_h__

void as3_import_file(char*filename);
void as3_import_swf(char*filename);
void as3_import_abc(char*filename);
void as3_import_code(void*f);

#endif
