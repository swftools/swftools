#include <stdio.h>
#include <stdarg.h>
#include "libming.h"

int SWF_versionNum = 7;

void (*SWF_error)(const char *msg, ...) = 0;
void (*SWF_warn)(const char *msg, ...) = 0;

