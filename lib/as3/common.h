/* common.h

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

#ifndef __as3_common_h__
#define __as3_common_h__

extern int as3_lex();
extern int as3_verbosity;
extern int as3_pass;
#define syntaxerror as3_error
int a3_error(char*s);
void as3_error(const char*format, ...) __attribute__ ((__noreturn__));
void as3_warning(const char*format, ...);
void as3_softwarning(const char*format, ...);

void internal_error(const char*file, int line, const char*f);
#define as3_assert(b) {if(!(b)) internal_error(__FILE__, __LINE__,__func__);}

#endif
