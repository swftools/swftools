/* depack.h

   Part of the swftools installer.
   
   Copyright (c) 2004 Matthias Kramm <kramm@quiss.org> 
 
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

#ifndef Depack_h
#define Depack_h

typedef unsigned long int U32;
typedef unsigned char U8;

typedef struct {
    void*internal;
    int pos;
    int size;
} depack_t;

typedef void (*writefunc_t)(void*writer, void*mem, int len);
typedef void (*readfunc_t)(void*reader, void*mem, int len);

void depack_init(depack_t*d, void*reader, readfunc_t);
void depack_process(depack_t*d, void*writer, writefunc_t);
void depack_destroy(depack_t*d);

#endif //Depack_h

