#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mem.h"

// memory allocation

void rfx_free(void*ptr)
{
  if(!ptr)
    return;
  free(ptr);
}

void start_debugger()
{
    //*(int*)0=0;
}

void* rfx_alloc(int size)
{
  void*ptr;
  if(size == 0) {
    //*(int*)0 = 0xdead;
    //fprintf(stderr, "Warning: Zero alloc\n");
    return 0;
  }

  ptr = malloc(size);
  if(!ptr) {
    fprintf(stderr, "FATAL: Out of memory (while trying to claim %d bytes)\n", size);
    start_debugger();
    exit(1);
  }
  return ptr;
}
void* rfx_realloc(void*data, int size)
{
  void*ptr;
  if(size == 0) {
    //*(int*)0 = 0xdead;
    //fprintf(stderr, "Warning: Zero realloc\n");
    rfx_free(data);
    return 0;
  }
  if(!data) {
    ptr = malloc(size);
  } else {
    ptr = realloc(data, size);
  }

  if(!ptr) {
    fprintf(stderr, "FATAL: Out of memory (while trying to claim %d bytes)\n", size);
    start_debugger();
    exit(1);
  }
  return ptr;
}
void* rfx_calloc(int size)
{
  void*ptr;
  if(size == 0) {
    //*(int*)0 = 0xdead;
    //fprintf(stderr, "Warning: Zero alloc\n");
    return 0;
  }
#ifdef HAVE_CALLOC
  ptr = calloc(1, size);
#else
  ptr = malloc(size);
#endif
  if(!ptr) {
    fprintf(stderr, "FATAL: Out of memory (while trying to claim %d bytes)\n", size);
    start_debugger();
    exit(1);
  }
#ifndef HAVE_CALLOC
  memset(ptr, 0, size);
#endif
  return ptr;
}
#ifndef HAVE_CALLOC
void* rfx_calloc_replacement(int nmemb, int size)
{
    rfx_calloc(nmemb*size);
}
#endif

#ifdef MEMORY_INFO
long rfx_memory_used()
{
}

char* rfx_memory_used_str()
{
}
#endif

