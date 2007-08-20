/* q.h
   Header file for q.c.

   Part of the swftools package.
   
   Copyright (c) 2001,2002,2003,2004 Matthias Kramm <kramm@quiss.org>
 
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

#ifndef __q_h__
#define __q_h__

#include <stdio.h>
#include "../config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* dynamically growing mem section */
typedef struct _mem_t {
    char*buffer;
    int len;
    int pos;
} mem_t;

/* fifo buffered growing mem region */
typedef struct _ringbuffer_t
{
    void*internal;
    int available;
} ringbuffer_t;

/* non-nul terminated string */
typedef struct _string_t {
    char*str;
    int len;
} string_t;

/* key/value pairs of strings */
typedef struct _map_t {
    void*internal;
} map_t;

/* (void*)s referenced by strings */
typedef struct _dictionary_t {
    void*internal;
} dictionary_t;

/* array of strings */
typedef struct _stringarray_t
{
    void*internal;
} stringarray_t;

/* heap */
typedef struct _heap
{
    void**elements;
    char*data;
    int elem_size;
    int size;
    int max_size;
    int(*compare)(const void *, const void *);
} heap_t;

void mem_init(mem_t*mem);
int mem_put(mem_t*m, void*data, int length);
int mem_putstring(mem_t*m, string_t str);
void mem_clear(mem_t*mem);
void mem_destroy(mem_t*mem);

void ringbuffer_init(ringbuffer_t*r);
void ringbuffer_put(ringbuffer_t*r, void*buf, int size);
int ringbuffer_read(ringbuffer_t*r, void*buf, int size);
void ringbuffer_clear(ringbuffer_t*r);

void string_set(string_t*str, char*text);
void string_set2(string_t*str, char*text, int len);
void string_dup(string_t*str, const char*text);
void string_dup2(string_t*str, const char*text, int len);
int string_equals(string_t*str, const char*text);

void stringarray_init(stringarray_t*sa);
void stringarray_put(stringarray_t*sa, string_t str);
char* stringarray_at(stringarray_t*sa, int pos);
string_t stringarray_at2(stringarray_t*sa, int pos);
int stringarray_find(stringarray_t*sa, string_t*str);
void stringarray_clear(stringarray_t*sa);
void stringarray_destroy(stringarray_t*sa);

void map_init(map_t*map);
void map_put(map_t*map, string_t t1, string_t t2);
char* map_lookup(map_t*map, const char*name);
void map_dump(map_t*map, FILE*fi, const char*prefix);
void map_clear(map_t*map);
void map_destroy(map_t*map);

void dictionary_init(dictionary_t*dict);
void dictionary_put(dictionary_t*dict, string_t t1, void* t2);
void dictionary_put2(dictionary_t*dict, const char* t1, void* t2);
stringarray_t* dictionary_index(dictionary_t*dict);
int dictionary_count(dictionary_t* dict);
void* dictionary_lookup(dictionary_t*dict, const char*name);
void dictionary_dump(dictionary_t*dict, FILE*fi, const char*prefix);
void dictionary_del(dictionary_t*dict, const char* name);
void dictionary_clear(dictionary_t*dict);
void dictionary_destroy(dictionary_t*dict);
void dictionary_free_all(dictionary_t* dict, void (*freeFunction)(void*));

void heap_init(heap_t*h,int n,int elem_size, int(*compare)(const void *, const void *));
void heap_clear(heap_t*h);
void heap_put(heap_t*h, void*e);
int heap_size(heap_t*h);
void* heap_max(heap_t*h);
void* heap_chopmax(heap_t*h);
void heap_dump(heap_t*h, FILE*fi);
void** heap_flatten(heap_t*h);

char* strdup_n(const char*str, int size);

#ifdef __cplusplus
}
#endif

#endif //__q_h__
