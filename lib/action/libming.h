#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __libming_h__
#define __libming_h__

typedef unsigned char byte;

extern void (*SWF_error)(const char *msg, ...);
extern void (*SWF_warn)(const char *msg, ...);

extern int SWF_versionNum;

#endif
