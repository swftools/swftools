/* types.h
   Some typedefs to make life simpler. If this file causes you problems, you might
   try to remove all #include "types.h" and live with what configure does define.

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __types_h__
#define __types_h__
typedef long unsigned w32;
typedef short unsigned w16;
typedef unsigned char u8;
typedef signed char s8;
typedef short unsigned u16;
typedef short signed s16;
typedef long unsigned u32;
typedef long signed s32;
typedef long long unsigned u64;
typedef long long signed s64;
typedef unsigned char        byte;
typedef unsigned char        uchar;
typedef   signed char       sbyte;
typedef unsigned short int   word;
typedef   signed short int  sword;
typedef unsigned long int   dword;
typedef   signed long int  sdword;
typedef   signed short int integer;
typedef   signed long int longint;
typedef long double extended;
#endif // __types_h__
