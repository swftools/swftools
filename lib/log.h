/* log.h 
   Header file for log.c.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __log_h__
#define __log_h__

#ifdef __NT__
#include "stdafx.h"
#include <time.h>
#include <windef.h>
#else
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#endif

#define LOGLEVEL_FATAL 0
#define LOGLEVEL_ERROR 1
#define LOGLEVEL_WARNING 2
#define LOGLEVEL_NOTICE 3
#define LOGLEVEL_VERBOSE 4
#define LOGLEVEL_DEBUG 5

extern int screenloglevel;

extern void initLog(char* pLogDir, int fileloglevel, char* servAddr, char* logPort, int serverloglevel, int screenloglevel);
extern void logf(const char* logFormat, ...);
extern void exitLog(void);

#define FIXNULL(a) ((int)(a)?(a):"NULL")

#endif  // __log_h__
