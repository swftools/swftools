/* compiler.h

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

#ifndef __as3_compiler_h__
#define __as3_compiler_h__

void registry_init();

void as3_setverbosity(int level);
void as3_add_include_dir(char*dir);

void as3_set_define(const char*c);
void as3_set_option(const char*key, const char*value);

void as3_parse_file(const char*filename);
void as3_parse_bytearray(const char*name, const void*mem, int length);
void as3_parse_directory(const char*dir);

char as3_schedule_directory(const char*dir);
void as3_schedule_package(const char*package);
void as3_schedule_class(const char*package, const char*cls);
void as3_schedule_class_noerror(const char*package, const char*cls);

void as3_warning(const char*format, ...);
char* as3_getglobalclass();
void* as3_getcode();
void* as3_getassets(void*);
void as3_destroy();

#endif //__as3_compiler_h__
