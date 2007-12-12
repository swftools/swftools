#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "mem.h"

// memory allocation

void rfx_free(void*ptr)
{
  if(!ptr)
    return;
  free(ptr);
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
    /* TODO: we should send a signal, so that the debugger kicks in? */
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
    /* TODO: we should send a signal, so that the debugger kicks in? */
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
  ptr = calloc(size);
#else
  ptr = malloc(size);
#endif
  if(!ptr) {
    fprintf(stderr, "FATAL: Out of memory (while trying to claim %d bytes)\n", size);
    /* TODO: we should send a signal, so that the debugger kicks in? */
    exit(1);
  }
#ifndef HAVE_CALLOC
  memset(ptr, 0, size);
#endif
  return ptr;
}

#ifdef MEMORY_INFO
long rfx_memory_used()
{
}

char* rfx_memory_used_str()
{
}
#endif

