#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "popplercompat.h"

#ifdef HAVE_POPPLER
static char* getTempDir()
{
#ifdef WIN32
    char*dir = getenv("TMP");
    if(!dir) dir = getenv("TEMP");
    if(!dir) dir = getenv("tmp");
    if(!dir) dir = getenv("temp");
    if(!dir) dir = "C:\\";
#else
    char* dir = "/tmp/";
#endif
    return dir;
}

char* mktmpname(char*ptr) {
    static char tmpbuf[128];
    char*dir = getTempDir();
    int l = strlen(dir);
    char*sep = "";
    if(!ptr)
	ptr = tmpbuf;
    if(l && dir[l-1]!='/' && dir[l-1]!='\\') {
#ifdef WIN32
	sep = "\\";
#else
	sep = "/";
#endif
    }

 //   used to be mktemp. This does remove the warnings, but
 //   It's not exactly an improvement.
#ifdef HAVE_LRAND48
    sprintf(ptr, "%s%s%08x%08x",dir,sep,(unsigned int)lrand48(),(unsigned int)lrand48());
#else
#   ifdef HAVE_RAND
	sprintf(ptr, "%s%s%08x%08x",dir,sep,rand(),rand());
#   else
	static int count = 1;
	sprintf(ptr, "%s%s%08x%04x%04x",dir,sep,time(0),(unsigned int)tmpbuf^((unsigned int)tmpbuf)>>16,count);
	count ++;
#   endif
#endif
     return ptr;
}

#endif
