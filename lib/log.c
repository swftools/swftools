/* log.c 
   Logging facilities for displaying information on screen, as well as
   (optional) storing it to a file and transmitting it over the network.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifdef __NT__
#include "stdafx.h"
#include <string.h>
#include <winsock2.h>
#include <stdlib.h>
#include <malloc.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#endif

#include "log.h"

static int screenloglevel = 1;
static int fileloglevel = -1;
static int socketloglevel = -1;
static int maxloglevel = 1;
static FILE *logFile = 0;
#ifdef __NT__
static SOCKET logSocket;
#else
static int logSocket = 0;
#endif

static char bLogToSock = 0;

static void initlogSocket(char* servAddr, char* logPort);

void initLog(char* pLogName, int fileloglevel, char* servAddr, char* logPort, int serverlevel, int screenlevel)
{
   screenloglevel = screenlevel;
   fileloglevel = fileloglevel;
   socketloglevel = screenlevel;

   maxloglevel=screenloglevel;
   if(fileloglevel>maxloglevel && pLogName)
       maxloglevel=fileloglevel;
   if(serverlevel>maxloglevel && servAddr)
       maxloglevel=serverlevel;

   if(!servAddr)
       serverlevel = -1;
   if(!pLogName)
       fileloglevel = -1;

   logFile = NULL;
   bLogToSock = 0;

   if (pLogName && fileloglevel>=0)
   logFile = fopen(pLogName, "a+");
   bLogToSock = (servAddr && logPort && (serverlevel>=0));
   if(bLogToSock)
       initlogSocket(servAddr, logPort);
}

static void initlogSocket(char* servAddr, char* logPort)
{
#ifndef __NT__
   bLogToSock = 0;
#else
   // init winsock
   // check and prepare WinSock DLL
   WORD wVersionRequested = MAKEWORD( 2, 2 );
   WSADATA wsaData;
   if ( WSAStartup(wVersionRequested, &wsaData) != 0 )
   {
      bLogToSock = false;
      return;
   }
   // Confirm that the WinSock DLL supports 2.2.
   // Note that if the DLL supports versions greater
   // than 2.2 in addition to 2.2, it will still return
   // 2.2 in wVersion since that is the version we
   // requested.

   if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )
   {
      bLogToSock = false;
      return;
   }

   struct hostent *hp;
   hp = gethostbyname(servAddr);
   if (hp == NULL) // we don't know who this host is
   {
      bLogToSock = false;
      return;
   }

   // connect socket
   sockaddr_in SocketAddress;

   memset(&SocketAddress, 0, sizeof(SocketAddress));
   memcpy((char*)&SocketAddress.sin_addr, hp->h_addr, hp->h_length); // set address
   SocketAddress.sin_family = hp->h_addrtype;
   SocketAddress.sin_port = htons((u_short)atoi(logPort));

   logSocket = socket(hp->h_addrtype, SOCK_STREAM, 0);
   if (logSocket == INVALID_SOCKET)
   {
      bLogToSock = false;
      return;
   }

   // try to connect to the specified socket
   if ( connect(logSocket, (struct sockaddr*)&SocketAddress, sizeof (SocketAddress)) == SOCKET_ERROR) {
      bLogToSock = false;
      return;
   }
   bLogToSock = true;
#endif
}

void exitLog()
{
   // close socket communication
   if(bLogToSock)
#ifndef __NT__
     close(logSocket);
#else
     closesocket(logSocket);
#endif
   // close file
   if(logFile != NULL)
     fclose(logFile);
}


static char * logimportance[]= {"Fatal","Error","Warning","Notice","Verbose","Debug"};
static int loglevels=6;
static char * logimportance2[]= {"       ","FATAL  ","ERROR  ","WARNING","NOTICE ","VERBOSE","DEBUG  "};
static inline void log(char* logString)
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
	  fprintf(logFile, "%s\n", logBuffer); 
	  fflush(logFile);
       }
   }

   if (level <= socketloglevel)
   {
       if (bLogToSock)
       {
	  // send data
#ifndef __NT__
	  write(logSocket, logBuffer, strlen(logBuffer));
#else
	  send(logSocket, logBuffer, strlen(logBuffer), 0);
#endif
       }
   }
   free (logBuffer);
}

void logf(const char* format, ...)
{
    char buf[1024];
	va_list arglist;
	va_start(arglist, format);
    
    /* speed up hack */
    if(format[0]=='<') {
	char*z = "fewnvd";
	char*x = strchr(z,format[0]);
	if(x && (x-z)>maxloglevel)
		return;
    }

    vsprintf(buf, format, arglist);
	va_end(arglist);
    strcat(buf, "\n");
    log(buf);
}

