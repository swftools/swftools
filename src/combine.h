/* combine.h
   Header file for combine.c

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __combine_h__
#define __combine_h__
unsigned char * combine(unsigned char*masterdata, int masterlength, 
			char*slavename, unsigned char*slavedata, int slavelength, int*newlength);
#endif //__combine_h__
