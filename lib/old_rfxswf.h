/* old_rfxswf.h

   File which defines macros and typedefs for old versions of
   the rfxswf lib functions and datatypes. (For routines which
   still use them)

   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#include "rfxswf.h"

/*
#define DataPos dataReadPos
#define DataSize dataWritePos
#define bitcount bitWritePos
#define bitmask bitWritePos

*/
#define GetDataSize GetTagLen
#define GetDataSizePtr GetTagLenPtr
