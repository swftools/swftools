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
#include <stdint.h>
#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NEW(t,y) t*y = (t*)rfx_calloc(sizeof(t));
#define PTR_AS_INT(p) (((char*)(p))-((char*)NULL))
#define INT_AS_PTR(i) (((char*)NULL)+(int)(i))

/* dynamically growing mem section */
typedef struct _mem_t {
    char*buffer;
    int len;
    int pos;
    int read_pos;
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

/* type information */
typedef char (*equals_func)(const void*o1, const void*o2);
typedef unsigned int (*hash_func)(const void*o);
typedef void* (*dup_func)(const void*o);
typedef void (*free_func)(void*o);

typedef struct _type_t {
    equals_func equals;
    hash_func hash;
    dup_func dup;
    free_func free;
} type_t;

extern type_t charptr_type;
extern type_t stringstruct_type;
extern type_t ptr_type;
extern type_t int_type;

typedef struct _dictentry {
    void*key;
    unsigned int hash;
    void*data;
    struct _dictentry*next;
} dictentry_t;

/* (void*) pointers referenced by strings */
typedef struct _dict {
    dictentry_t**slots;
    type_t*key_type;
    int hashsize;
    int num;
} dict_t;

/* array of key/value pairs, with fast lookup */
typedef struct _array_entry {
    void*name;
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
typedef struct _heap {
    void**elements;
    char*data;
    int elem_size;
    int size;
    int max_size;
    int(*compare)(const void *, const void *);
} heap_t;

/* trie (with rollback) */
typedef struct _trielayer {
    struct _trielayer*row[256];
    unsigned char*rest;
    void*data;
} trielayer_t;

typedef struct _trie {
    trielayer_t* start;
    void*rollback;
} trie_t;

/* move to front list structure */
typedef struct _mtf_item {
    const void*key;
    int num;
    struct _mtf_item*next;
} mtf_item_t;

typedef struct _mtf {
    mtf_item_t*first;
    type_t*type;
} mtf_t;

char* strdup_n(const char*str, int size);
char* allocprintf(const char*str, ...);

float medianf(float*values, int n);

unsigned int crc32_add_byte(unsigned int crc32, unsigned char b);
unsigned int crc32_add_string(unsigned int crc32, const char*s);
unsigned int crc32_add_bytes(unsigned int checksum, const void*s, int len);

void mem_init(mem_t*mem);
int mem_put(mem_t*m, void*data, int length);
int mem_putstring(mem_t*m, string_t str);
int mem_get(mem_t*m, void*data, int length);
void mem_clear(mem_t*mem);
void mem_destroy(mem_t*mem);

void ringbuffer_init(ringbuffer_t*r);
void ringbuffer_put(ringbuffer_t*r, void*buf, int size);
int ringbuffer_read(ringbuffer_t*r, void*buf, int size);
void ringbuffer_clear(ringbuffer_t*r);

/* old style functions- should be renamed */
string_t string_new(const char*text, int len);
string_t string_new2(const char*text);
void string_dup(string_t*str, const char*text);
void string_dup2(string_t*str, const char*text, int len);

char* string_cstr(string_t*str);
char* string_escape(string_t*str);
string_t* string_new3(const char*text, int len);
string_t* string_new4(const char*text);
void string_free(string_t*s);
unsigned int string_hash(const string_t*str);
unsigned int string_hash2(const char*str);
unsigned int string_hash3(const char*str, int len);
uint64_t string_hash64(const char*str);
void string_set(string_t*str, const char*text);
void string_set2(string_t*str, const char*text, int len);
string_t*string_dup3(string_t*s);
int string_equals(string_t*str, const char*text);

char* concat2(const char* t1, const char* t2);
char* concat3(const char* t1, const char* t2, const char* t3);

void stringarray_init(stringarray_t*sa, int hashsize);
void stringarray_put(stringarray_t*sa, string_t str);

char* stringarray_at(stringarray_t*sa, int pos);
string_t stringarray_at2(stringarray_t*sa, int pos);
int stringarray_find(stringarray_t*sa, string_t*str);
void stringarray_clear(stringarray_t*sa);
void stringarray_destroy(stringarray_t*sa);

dict_t*dict_new();
dict_t*dict_new2(type_t*type);
void dict_init(dict_t*dict, int size);
void dict_init2(dict_t*dict, type_t*type, int size);
dictentry_t*dict_put(dict_t*h, const void*key, void* data);
void dict_put2(dict_t*h, const char*s, void*data);
int dict_count(dict_t*h);
void dict_dump(dict_t*h, FILE*fi, const char*prefix);
dictentry_t* dict_get_slot(dict_t*h, const void*key);
char dict_contains(dict_t*h, const void*s);
void* dict_lookup(dict_t*h, const void*s);
char dict_del(dict_t*h, const void*s);
char dict_del2(dict_t*h, const void*key, void*data);
dict_t*dict_clone(dict_t*);

void dict_foreach_keyvalue(dict_t*h, void (*runFunction)(void*data, const void*key, void*val), void*data);
void dict_foreach_value(dict_t*h, void (*runFunction)(void*));
void dict_free_all(dict_t*h, char free_keys, void (*free_data_function)(void*));
void dict_clear(dict_t*h);
void dict_destroy_shallow(dict_t*dict);
void dict_destroy(dict_t*dict);
#define DICT_ITERATE_DATA(d,t,v) \
    int v##_i;dictentry_t*v##_e;t v;\
    for(v##_i=0;v##_i<(d)->hashsize;v##_i++) \
        for(v##_e=(d)->slots[v##_i]; v##_e && ((v=(t)v##_e->data)||1); v##_e=v##_e->next)
#define DICT_ITERATE_KEY(d,t,v)  \
    int v##_i;dictentry_t*v##_e;t v;\
    for(v##_i=0;v##_i<(d)->hashsize;v##_i++) \
        for(v##_e=(d)->slots[v##_i];v##_e && ((v=(t)v##_e->key)||1);v##_e=v##_e->next)
#define DICT_ITERATE_ITEMS(d,t1,v1,t2,v2) \
    int v1##_i;dictentry_t*v1##_e;t1 v1;t2 v2; \
    for(v1##_i=0;v1##_i<(d)->hashsize;v1##_i++) \
        for(v1##_e=(d)->slots[v1##_i]; v1##_e && (((v1=(t1)v1##_e->key)||1)&&((v2=(t2)v1##_e->data)||1)); v1##_e=v1##_e->next)

void map_init(map_t*map);
void map_put(map_t*map, string_t t1, string_t t2);
const char* map_lookup(map_t*map, const char*name);
void map_dump(map_t*map, FILE*fi, const char*prefix);
void map_clear(map_t*map);
void map_destroy(map_t*map);

void heap_init(heap_t*h,int elem_size, int(*compare)(const void *, const void *));
heap_t* heap_new(int elem_size, int(*compare)(const void *, const void *));
heap_t* heap_clone(heap_t*o);
void heap_clear(heap_t*h);
void heap_destroy(heap_t*h);
void heap_put(heap_t*h, void*e);
int heap_size(heap_t*h);
void* heap_peek(heap_t*h);
void* heap_chopmax(heap_t*h);
void heap_dump(heap_t*h, FILE*fi);
void** heap_flatten(heap_t*h);

trie_t*trie_new();
void trie_put(trie_t*t, unsigned const char*id, void*data);
char trie_remove(trie_t*t, unsigned const char*id);
void*trie_lookup(trie_t*t, unsigned const char*id);
int trie_contains(trie_t*t, unsigned const char*id);
void trie_remember(trie_t*t);
void trie_rollback(trie_t*t);
void trie_dump(trie_t*t);

mtf_t* mtf_new(type_t*type);
void mtf_increase(mtf_t*m, const void*key);
void mtf_destroy(mtf_t*m);

array_t* array_new();
array_t* array_new2(type_t*type);
void array_free(array_t*array);
void*array_getkey(array_t*array, int nr);
void*array_getvalue(array_t*array, int nr);
int array_append(array_t*array, const void*name, void*data);
#define array_contains(a,b) (array_find((a),(b))>=0)
int array_find(array_t*array, const void*name);
int array_find2(array_t*array, const void*name, void*data);
int array_update(array_t*array, const void*name, void*data);
int array_append_if_new(array_t*array, const void*name, void*data);
#define array_length(a) ((a)->num)

#define DECLARE(x) struct _##x;typedef struct _##x x##_t;
#define DECLARE_LIST(x) \
struct _##x##_list { \
    struct _##x* x; \
    struct _##x##_list*next; \
}; \
typedef struct _##x##_list x##_list_t;
int list_length_(void*_list);
void*list_clone_(void*_list);
void list_append_(void*_list, void*entry);
void list_prepend_(void*_list, void*entry);
void list_free_(void*_list);
void list_deep_free_(void*_list);
void list_concat_(void*l1, void*l2);
#define list_new() ((void*)0)
#define list_append(list, e) {sizeof((list)->next);list_append_(&(list),(e));}
#define list_concat(l1, l2) {sizeof((l1)->next);sizeof((l2)->next);list_concat_(&(l1),&(l2));}
#define list_prepend(list, e) {sizeof((list)->next);list_prepend_(&(list),(e));}
#define list_free(list) {sizeof((list)->next);list_free_(&(list));}
#define list_deep_free(list) {sizeof((list)->next);list_deep_free_(&(list));}
#define list_clone(list) (sizeof((list)->next),(list?list_clone_(&(list)):0))
#define list_length(list) (sizeof((list)->next),list_length_(list))

#ifdef __cplusplus
}
#endif

#endif //__q_h__
