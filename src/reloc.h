/* reloc.h
   Header file for reloc.c

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __swf_reallocate__
#define __swf_reallocate__
void map_ids_mem(unsigned char*mem, int length, void(*callback)(void*));
void swf_relocate (unsigned char*data, int length, int*bitmap);
#endif // __swf_reallocate__

