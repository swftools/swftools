/* log.c 
   Logging facilities for displaying information on screen, as well as
   (optional) storing it to a file and transmitting it over the network.

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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifdef WIN32
//#include "stdafx.h"
#include <malloc.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#else
#include <stdio.h>
#include <unistd.h>
#endif

#include "log.h"

int maxloglevel = 1;
static int screenloglevel = 1;
static int fileloglevel = -1;
static FILE *logFile = 0;

int getScreenLogLevel()
{
    return screenloglevel;
}
int getLogLevel()
{
    return maxloglevel;
}

void setConsoleLogging(int level)
{
    if(level>maxloglevel)
        maxloglevel=level;
    screenloglevel = level;
}
void setFileLogging(char*filename, int level, char append)
{
    if(level>maxloglevel)
        maxloglevel=level;
    if(logFile) {
        fclose(logFile);logFile=0;
    }
    if(filename && level>=0) {
        logFile = fopen(filename, append?"ab+":"wb");
        fileloglevel = level;
    } else {
        logFile = 0;
        fileloglevel = 0;
    }
}
/* deprecated */
void initLog(char* filename, int filelevel, char* s00, char* s01, int s02, int screenlevel)
{
    setFileLogging(filename, filelevel, 0);
    setConsoleLogging(screenlevel);
}

void exitLog()
{
   // close file
   if(logFile != NULL) {
     fclose(logFile);
     logFile = 0;
     fileloglevel = -1;
     screenloglevel = 1;
     maxloglevel = 1;
   }
}

static char * logimportance[]= {"Fatal","Error","Warning","Notice","Verbose","Debug","Trace"};
static int loglevels=7;
static char * logimportance2[]= {"       ","FATAL  ","ERROR  ","WARNING","NOTICE ","VERBOSE","DEBUG  ", "TRACE  "};

static inline void log_str(const char* logString)
{
   char timebuffer[32];
   char* logBuffer;
   char dbuffer[9];
   char tbuffer[9];
   int level;
   char*lt;
   char*gt;
   int l;

   logBuffer = (char*)malloc (strlen(logString) + 24 + 15);
#ifndef __NT__
   {
     /*time_t t = time(0);
     tm*t2 = localtime(t);
     strftime(dbuffer, 8, "%m %d", t2);
     strftime(tbuffer, 8, "%m %d", t2);
     dbuffer[0]=0; //FIXME
     tbuffer[0]=0;*/
     time_t t = time(0);
     char* a = ctime(&t);
     int l = strlen(a);
     while(a[l-1] == 13 || a[l-1] == 10)
       l--;
     a[l]=0;
     sprintf(timebuffer, "%s", a);
   }
#else
   _strdate( dbuffer );
   _strtime( tbuffer );
   sprintf(timebuffer, "%s - %s",dbuffer,tbuffer);
#endif

   // search for <level> field
   level = -1;
   lt=strchr(logString, '<');
   gt=strchr(logString, '>');
   if(lt && gt && lt<gt)
   {
       int t;
       for(t=0;t<loglevels;t++)
       {
#ifndef __NT__
	   if(!strncasecmp(lt+1,logimportance[t],strlen(logimportance[t])))
#else
	   if(!strnicmp(lt+1,logimportance[t],strlen(logimportance[t])))
#endif
	   {
	       logString = gt+1;
	       while(logString[0]==' ') logString ++;
	       level = t;
	       break;
	   }
       }
   }
   
//   sprintf(logBuffer, "%s: %s %s", timebuffer, logimportance2[level + 1],logString);
   sprintf(logBuffer, "%s %s", logimportance2[level + 1],logString);

   // we always do exactly one newline.
   
   l=strlen(logBuffer)-1;
   while((logBuffer[l]==13 || logBuffer[l]==10) && l>=0)
   {
       logBuffer[l]=0;
       l--;
   }

   if (level <= screenloglevel)
   {
       printf("%s\n", logBuffer); 
       fflush(stdout);
   }

   if (level <= fileloglevel)
   {
       if (logFile != NULL)
       {
	  fprintf(logFile, "%s\r\n", logBuffer); 
	  fflush(logFile);
       }
   }

   free (logBuffer);
}

void msg_str(const char* buf)
{
    if(buf[0]=='<') {
	char*z = "fewnvdt";
	char*x = strchr(z,buf[1]);
	if(x && (x-z)>maxloglevel)
		return;
    }
    log_str(buf);
}

char char2loglevel[32] =
/*   a  b  c           d           e            f            g            h   i  j  k  l  m           n  o */
{-1,-1,-1,-1, /*debug*/5, /*error*/1,  /*fatal*/0,          -1,          -1, -1,-1,-1,-1,-1,/*notice*/3,-1,
/*   p  q  r           s           t            u            v            w   x  y  z                       */
    -1,-1,-1,         -1, /*trace*/6,          -1,/*verbose*/4,/*warning*/2, -1,-1,-1, -1,-1,-1,-1,-1};

int msg_internal(const char* format, ...)
{
    char buf[1024];
	va_list arglist;
	va_start(arglist, format);
    
    /* speed up hack */
    if(format[0]=='<') {
	char*z = "fewnvdt";
	char*x = strchr(z,format[1]);
	if(x && (x-z)>maxloglevel)
		return 0;
    }

    vsnprintf(buf, sizeof(buf)-1, format, arglist);
	va_end(arglist);
    strcat(buf, "\n");
    log_str(buf);
    return 0;
}

