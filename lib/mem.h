#ifndef __mem_h__
#define __mem_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "../config.h"

#define ALLOC_ARRAY(type, num) (((type)*)rfxalloc(sizeof(type)*(num)))
void* rfx_alloc(int size);
void* rfx_calloc(int size);
void* rfx_realloc(void*data, int size);
void rfx_free(void*data);
#ifndef HAVE_CALLOC
void* rfx_calloc_replacement(int nmemb, int size);
#endif
#ifdef MEMORY_INFO
long rfx_memory_used();
char* rfx_memory_used_str();
#endif

#ifdef __cplusplus
}
#endif

#endif //__mem_h__
