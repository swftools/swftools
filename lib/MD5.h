/* MD5.h

   Header for the FreeBSD MD5 password hashing algorithm

   Part of the swftools package.

   Copyright (c) 2003, 2004 Matthias Kramm <kramm@quiss.org>
 
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


#ifndef __MD5_h__
#define __MD5_h__
/* WARNING: crypt_md5 is not reentrant */
char * crypt_md5(const char *pw, const char *salt);
#endif
