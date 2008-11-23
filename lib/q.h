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

#ifdef __cplusplus
extern "C" {
#endif

#define NEW(t,y) t*y = (t*)malloc(sizeof(t));memset(y, 0, sizeof(t));

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
    const char*str;
    int len;
} string_t;

/* key/value pairs of strings */
typedef struct _map_t {
    void*internal;
} map_t;

typedef struct _dictentry {
    const char*s;
    int len;
    unsigned int hash;
    void*data;
    struct _dictentry*next;
} dictentry_t;

/* (void*) pointers referenced by strings */
typedef struct _dict {
    dictentry_t**slots;
    int hashsize;
    int num;
} dict_t;

/* array of key/value pairs, with fast lookup */
typedef struct _array_entry {
    const char*name;
    void*data;
} array_entry_t;

typedef struct _array {
    int num;
    int size;
    array_entry_t*d;
    dict_t*entry2pos;
} array_t;

/* array of strings, string<->int mapping,
   with O(1) for int->string lookup and
        ~O(n/hashsize) for string->int lookup */
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

char* strdup_n(const char*str, int size);

void mem_init(mem_t*mem);
int mem_put(mem_t*m, void*data, int length);
int mem_putstring(mem_t*m, string_t str);
void mem_clear(mem_t*mem);
void mem_destroy(mem_t*mem);

void ringbuffer_init(ringbuffer_t*r);
void ringbuffer_put(ringbuffer_t*r, void*buf, int size);
int ringbuffer_read(ringbuffer_t*r, void*buf, int size);
void ringbuffer_clear(ringbuffer_t*r);

string_t string_new(const char*text, int len);
string_t string_new2(const char*text);
unsigned int string_hash(string_t*str);
unsigned int string_hash2(const char*str);
unsigned int string_hash3(const char*str, int len);
void string_set(string_t*str, const char*text);
void string_set2(string_t*str, const char*text, int len);
void string_dup(string_t*str, const char*text);
void string_dup2(string_t*str, const char*text, int len);
int string_equals(string_t*str, const char*text);

void stringarray_init(stringarray_t*sa, int hashsize);
void stringarray_put(stringarray_t*sa, string_t str);

char* stringarray_at(stringarray_t*sa, int pos);
string_t stringarray_at2(stringarray_t*sa, int pos);
int stringarray_find(stringarray_t*sa, string_t*str);
void stringarray_clear(stringarray_t*sa);
void stringarray_destroy(stringarray_t*sa);

dict_t*dict_new();
void dict_init(dict_t*dict);
void dict_put(dict_t*dict, string_t t1, void* t2);
void dict_put2(dict_t*dict, const char* t1, void* t2);
void dict_put3(dict_t*dict, const char* t1, int len, void* t2);
int dict_count(dict_t*h);
void dict_dump(dict_t*h, FILE*fi, const char*prefix);
void* dict_lookup3(dict_t*h, const char*s, const void*data);
void* dict_lookup2(dict_t*h, const char*s, int len);
void* dict_lookup(dict_t*h, const char*s);
char dict_del(dict_t*h, const char*s);
void dict_foreach_keyvalue(dict_t*h, void (*runFunction)(void*data, const char*key, void*val), void*data);
void dict_foreach_value(dict_t*h, void (*runFunction)(void*));
void dict_free_all(dict_t*h, void (*freeFunction)(void*));
void dict_clear(dict_t*h);
void dict_destroy(dict_t*dict);

void map_init(map_t*map);
void map_put(map_t*map, string_t t1, string_t t2);
const char* map_lookup(map_t*map, const char*name);
void map_dump(map_t*map, FILE*fi, const char*prefix);
void map_clear(map_t*map);
void map_destroy(map_t*map);

void heap_init(heap_t*h,int n,int elem_size, int(*compare)(const void *, const void *));
void heap_clear(heap_t*h);
void heap_put(heap_t*h, void*e);
int heap_size(heap_t*h);
void* heap_max(heap_t*h);
void* heap_chopmax(heap_t*h);
void heap_dump(heap_t*h, FILE*fi);
void** heap_flatten(heap_t*h);

array_t* array_new();
void array_free(array_t*array);
const char*array_getkey(array_t*array, int nr);
char*array_getvalue(array_t*array, int nr);
int array_append(array_t*array, const char*name, const void*data);
int array_find(array_t*array, const char*name);
int array_find2(array_t*array, const char*name, void*data);
int array_update(array_t*array, const char*name, void*data);
int array_append_if_new(array_t*array, const char*name, void*data);

#define DECLARE(x) struct _##x;typedef struct _##x x##_t;
#define DECLARE_LIST(x) \
struct _##x##_list { \
    struct _##x* x; \
    struct _##x##_list*next; \
}; \
typedef struct _##x##_list x##_list_t;
int list_length(void*_list);
void list_append_(void*_list, void*entry);
void list_free_(void*_list);
#define list_new() ((void*)0)
#define list_append(list, e) {sizeof((list)->next);list_append_(&(list),(e));}
#define list_free(list) {sizeof((list)->next);list_free_(&(list));}

#ifdef __cplusplus
}
#endif

#endif //__q_h__
