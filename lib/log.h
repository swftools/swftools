/* log.h 
   Header file for log.c.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 
 
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

#ifndef __log_h__
#define __log_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define LOGLEVEL_FATAL 0
#define LOGLEVEL_ERROR 1
#define LOGLEVEL_WARNING 2
#define LOGLEVEL_NOTICE 3
#define LOGLEVEL_VERBOSE 4
#define LOGLEVEL_DEBUG 5
#define LOGLEVEL_TRACE 6

extern int getLogLevel();
extern int getScreenLogLevel();

extern void initLog(char* pLogDir, int fileloglevel, char* servAddr, char* logPort, int serverloglevel, int screenloglevel);
extern void setConsoleLogging(int level);
extern void setFileLogging(char*filename, int level, char append);

extern int maxloglevel;
extern char char2loglevel[32];

#define msg(fmt,args...) \
    (((fmt)[0]=='<' && char2loglevel[(fmt)[1]&31]<=maxloglevel)?msg_internal((fmt),## args):0)

extern int msg_internal(const char* logFormat, ...);
extern void msg_str(const char* log);
extern void exitLog(void);

#define FIXNULL(a) ((a)?(a):"NULL")

#ifdef __cplusplus
}
#endif
#endif  // __log_h__
