/* q.c

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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "q.h"

// ------------------------------- malloc, alloc routines ---------------------

#ifndef STRNDUP
char* strdup_n(const char*str, int size)
{
    char*m = (char*)malloc(size+1);
    memcpy(m, str, size);
    m[size] = 0;
    return m;
}
#endif
char*qstrdup(const char*string)
{
    return strdup(string);
}
char*qstrndup(const char*string, int len)
{
    return strdup_n(string, len);
}

// ------------------------------- mem_t --------------------------------------

void mem_init(mem_t*mem)
{
    memset(mem, 0, sizeof(mem_t));
}
void mem_clear(mem_t*mem)
{
    free(mem->buffer);mem->buffer = 0;
}
void mem_destroy(mem_t*mem)
{
    mem_clear(mem);
    free(mem);
}
static int mem_put_(mem_t*m,const void*data, int length, int null)
{
    int n = m->pos;
    m->pos += length + (null?1:0);
    if(m->pos > m->len) { 
        m->len = (m->pos+63)&~63;
	m->buffer = m->buffer?(char*)realloc(m->buffer,m->len):(char*)malloc(m->len);
    }
    memcpy(&m->buffer[n], data, length);
    if(null)
	m->buffer[n + length] = 0;
    return n;
}
int mem_put(mem_t*m,void*data, int length)
{
    return mem_put_(m, data, length, 0);
}
int mem_putstring(mem_t*m,string_t str)
{
    return mem_put_(m, str.str, str.len, 1);
}

// ------------------------------- ringbuffer_t -------------------------------

typedef struct _ringbuffer_internal_t
{
    unsigned char*buffer;
    int readpos;
    int writepos;
    int buffersize;
} ringbuffer_internal_t;

void ringbuffer_init(ringbuffer_t*r)
{
    ringbuffer_internal_t*i = (ringbuffer_internal_t*)malloc(sizeof(ringbuffer_internal_t)); 
    memset(r, 0, sizeof(ringbuffer_t));
    memset(i, 0, sizeof(ringbuffer_internal_t));
    r->internal = i;
    i->buffer = (unsigned char*)malloc(1024);
    i->buffersize = 1024;
}
int ringbuffer_read(ringbuffer_t*r, void*buf, int len)
{
    unsigned char* data = (unsigned char*)buf;
    ringbuffer_internal_t*i = (ringbuffer_internal_t*)r->internal;
    if(r->available < len)
	len = r->available;
    if(!len)
	return 0;
    if(i->readpos + len > i->buffersize) {
	int read1 = i->buffersize-i->readpos;
	memcpy(data, &i->buffer[i->readpos], read1);
	memcpy(&data[read1], &i->buffer[0], len - read1);
	i->readpos = len - read1;
    } else {
	memcpy(data, &i->buffer[i->readpos], len);
	i->readpos += len;
	i->readpos %= i->buffersize;
    }
    r->available -= len;
    return len;
}
void ringbuffer_put(ringbuffer_t*r, void*buf, int len)
{
    unsigned char* data = (unsigned char*)buf;
    ringbuffer_internal_t*i = (ringbuffer_internal_t*)r->internal;
    
    if(i->buffersize - r->available < len)
    {
	unsigned char* buf2;
	int newbuffersize = i->buffersize;
	int oldavailable = r->available;
	newbuffersize*=3;newbuffersize/=2; /*grow at least by 50% each time */

	if(newbuffersize < r->available + len)
	    newbuffersize = r->available + len + 1024;

	buf2 = (unsigned char*)malloc(newbuffersize);
	ringbuffer_read(r, buf2, r->available);
	free(i->buffer);
	i->buffer = buf2;
	i->buffersize = newbuffersize;
	i->readpos = 0;
	i->writepos = oldavailable;
	r->available = oldavailable;
    }
    if(i->writepos + len > i->buffersize) {
	int read1 = i->buffersize-i->writepos;
	memcpy(&i->buffer[i->writepos], data, read1);
	memcpy(&i->buffer[0], &data[read1], len - read1);
	i->writepos = len - read1;
    } else {
	memcpy(&i->buffer[i->writepos], data, len);
	i->writepos += len;
	i->writepos %= i->buffersize;
    }
    r->available += len;
}
void ringbuffer_clear(ringbuffer_t*r)
{
    ringbuffer_internal_t*i = (ringbuffer_internal_t*)r->internal;
    free(i->buffer);i->buffer = 0;
    free(i);
}

// ------------------------------- heap_t -------------------------------

void heap_init(heap_t*h,int n,int elem_size, int(*compare)(const void *, const void *))
{
    memset(h, 0, sizeof(heap_t));
    h->max_size = n;
    h->size = 0;
    h->elem_size = elem_size;
    h->compare = compare;
    h->elements = (void**)malloc(n*sizeof(void*));memset(h->elements, 0, n*sizeof(void*));
    h->data = (char*)malloc(h->max_size*h->elem_size);memset(h->data, 0, h->max_size*h->elem_size);
}
void heap_clear(heap_t*h)
{
    free(h->elements);
    free(h->data);
}

#define HEAP_NODE_SMALLER(h,node1,node2) ((h)->compare((node1),(node2))>0)

static void up(heap_t*h, int node)
{
    void*node_p = h->elements[node];
    int parent = node;
    do {
	node = parent;
	if(!node) break;
	parent = (node-1)/2;
	h->elements[node] = h->elements[parent];
    } while(HEAP_NODE_SMALLER(h,h->elements[parent], node_p));

    h->elements[node] = node_p;
}
static void down(heap_t*h, int node)
{
    void*node_p = h->elements[node];
    int child = node;
    do {
	node = child;

	/* determine new child's position */
	child = node<<1|1;
	if(child >= h->size) 
            break;
        if(child+1 < h->size && HEAP_NODE_SMALLER(h,h->elements[child],h->elements[child+1])) // search for bigger child
	    child++;

	h->elements[node] = h->elements[child];
    } while(HEAP_NODE_SMALLER(h,node_p, h->elements[child]));
    
    h->elements[node] = node_p;
}
void heap_put(heap_t*h, void*e) 
{
    int pos = h->size++;
    memcpy(&h->data[pos*h->elem_size],e,h->elem_size);
    h->elements[pos] = &h->data[pos];
    up(h, pos);
}
int heap_size(heap_t*h)
{
    return h->size;
}
void* heap_max(heap_t*h)
{
    return h->elements[0];
}
void* heap_chopmax(heap_t*h)
{
    void*p = h->elements[0];
    h->elements[0] = h->elements[--h->size];
    down(h,0);
    return p;
}
void heap_dump(heap_t*h, FILE*fi)
{
    int t;
    for(t=0;t<h->size;t++) {
	int s;
	for(s=0;s<=t;s=(s+1)*2-1) {
	    if(s==t) fprintf(fi,"\n");
	}
	//fprintf(fi,"%d ", h->elements[t]->x); //?
    }
}
void** heap_flatten(heap_t*h)
{
    void**nodes = (void**)malloc(h->size*sizeof(void*));
    void**p = nodes;
   
    while(h->size) {
	/*printf("Heap Size: %d\n", h->size);
	heap_print(stdout, h);
	printf("\n");*/
	*p++ = heap_chopmax(h);
    }
    return nodes;
}

// ------------------------------- crc32 --------------------------------------
static unsigned int*crc32 = 0;
static void crc32_init(void)
{
    int t;
    if(crc32) 
        return;
    crc32= (unsigned int*)malloc(sizeof(unsigned int)*256);
    for(t=0; t<256; t++) {
        unsigned int c = t;
        int s;
        for (s = 0; s < 8; s++) {
          c = (0xedb88320L*(c&1)) ^ (c >> 1);
        }
        crc32[t] = c;
    }
}
// ------------------------------- string_t -----------------------------------

void string_set2(string_t*str, const char*text, int len)
{
    str->len = len;
    str->str = text;
}
void string_set(string_t*str, const char*text)
{
    str->len = strlen(text);
    str->str = text;
}
string_t string_new(const char*text, int len)
{
    string_t s;
    s.len = len;
    s.str = text;
    return s;
}
string_t string_new2(const char*text)
{
    string_t s;
    s.len = strlen(text);
    s.str = text;
    return s;
}
unsigned int string_hash(string_t*str)
{
    int t;
    unsigned int checksum = 0;
    if(!crc32)
        crc32_init();
    for(t=0;t<str->len;t++) {
        checksum = checksum>>8 ^ crc32[(str->str[t]^checksum)&0xff];
    }
    return checksum;
}
unsigned int string_hash2(const char*str)
{
    unsigned int checksum = 0;
    if(!crc32)
        crc32_init();
    while(*str) {
        checksum = checksum>>8 ^ crc32[(*str^checksum)&0xff];
        str++;
    }
    return checksum;
}
void string_dup2(string_t*str, const char*text, int len)
{
    str->len = len;
    str->str = strdup_n(text, len);
}
void string_dup(string_t*str, const char*text)
{
    str->len = strlen(text);
    str->str = strdup(text);
}
int string_equals(string_t*str, const char*text)
{
    int l = strlen(text);
    if(str->len == l && !memcmp(str->str, text, l))
	return 1;
    return 0;
}
int string_equals2(string_t*str, string_t*str2)
{
    if(str->len == str2->len && !memcmp(str->str, str2->str, str->len))
	return 1;
    return 0;
}
char* string_cstr(string_t*str)
{
    return strdup_n(str->str, str->len);
}

// ------------------------------- stringarray_t ------------------------------

#define DEFAULT_HASH_SIZE 257

typedef struct _stringlist {
    int index;
    struct _stringlist*next;
} stringlist_t;

typedef struct _stringarray_internal_t
{
    mem_t pos;
    stringlist_t**hash;
    int num;
    int hashsize;
} stringarray_internal_t;

void stringarray_init(stringarray_t*sa, int hashsize)
{
    stringarray_internal_t*s;
    int t;
    sa->internal = (stringarray_internal_t*)malloc(sizeof(stringarray_internal_t)); 
    memset(sa->internal, 0, sizeof(stringarray_internal_t));
    s = (stringarray_internal_t*)sa->internal;
    mem_init(&s->pos);
    s->hash = malloc(sizeof(stringlist_t*)*hashsize);
    memset(s->hash, 0, sizeof(stringlist_t*)*hashsize);
    s->hashsize = hashsize;
}
void stringarray_put(stringarray_t*sa, string_t str)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    int pos;
    int hash = string_hash(&str) % s->hashsize;

    char*ss = string_cstr(&str);
    mem_put(&s->pos, &ss, sizeof(char*));

    stringlist_t*l = malloc(sizeof(stringlist_t));
    l->index = s->num;
    l->next = s->hash[hash];
    s->hash[hash] = l;

    s->num++;
}
char* stringarray_at(stringarray_t*sa, int pos)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    char*p;
    if(pos<0 || pos>=s->num)
	return 0;
    p = *(char**)&s->pos.buffer[pos*sizeof(char*)];
    if(p<0)
	return 0;
    return p;
}
string_t stringarray_at2(stringarray_t*sa, int pos)
{
    string_t s;
    s.str = stringarray_at(sa, pos);
    s.len = s.str?strlen(s.str):0;
    return s;
}
static stringlist_t* stringlist_del(stringarray_t*sa, stringlist_t*l, int index)
{
    stringlist_t*ll = l;
    stringlist_t*old = l;
    while(l) {
        if(index==l->index) {
            old->next = l->next;
            memset(l, 0, sizeof(stringlist_t));
            free(l);
            if(old==l)
                return 0;
            else
                return ll;
        }
        old = l;
        l = l->next;
    }
    fprintf(stderr, "Internal error: did not find string %d in hash\n", index);
    return ll;
}

void stringarray_del(stringarray_t*sa, int pos)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    string_t str = stringarray_at2(sa, pos);
    int hash = string_hash(&str) % s->hashsize;
    s->hash[hash] = stringlist_del(sa, s->hash[hash], pos);
    *(char**)&s->pos.buffer[pos*sizeof(char*)] = 0;
}
int stringarray_find(stringarray_t*sa, string_t* str)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    int hash = string_hash(str) % s->hashsize;
    int t;
    stringlist_t*l = s->hash[hash];
    //TODO: statistics
    while(l) {
        string_t s = stringarray_at2(sa, l->index);
        if(string_equals2(str, &s)) {
            return l->index;
        }
        l = l->next;
    }
    return -1;
}
void stringarray_clear(stringarray_t*sa)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    mem_clear(&s->pos);
    int t;
    for(t=0;t<s->hashsize;t++) {
        stringlist_t*l = s->hash[t];
        while(l) {
            stringlist_t*next = l->next;
            memset(l, 0, sizeof(stringlist_t));
            free(l);
            l = next;
        }
    }
    free(s->hash);s->hash=0;
    free(s);
}
void stringarray_destroy(stringarray_t*sa)
{
    stringarray_clear(sa);
    free(sa);
}


// ------------------------------- dictionary_t -------------------------------

dict_t*dict_new()
{
    dict_t*d = malloc(sizeof(dict_t));
    dict_init(d);
    return d;
}
void dict_init(dict_t*h) 
{
    memset(h, 0, sizeof(dict_t));
    h->hashsize = DEFAULT_HASH_SIZE;
    h->slots = malloc(sizeof(dictentry_t*)*h->hashsize);
    h->num = 0;
    memset(h->slots, 0, sizeof(dictentry_t*)*h->hashsize);
}
void dict_put(dict_t*h, string_t s, void* data)
{
    unsigned int checksum = string_hash(&s) % h->hashsize;
    dictentry_t*e = (dictentry_t*)malloc(sizeof(dictentry_t));
    e->s = strdup(s.str);
    e->next = h->slots[checksum];
    e->data = data;
    h->slots[checksum] = e;
    h->num++;
}
void dict_put2(dict_t*h, const char*s, void*data) 
{
    string_t s2;
    string_set(&s2, s);
    dict_put(h, s2, data);
}
void dict_dump(dict_t*h, FILE*fi, const char*prefix)
{
    int t;
    for(t=0;t<h->hashsize;t++) {
        dictentry_t*e = h->slots[t];
        while(e) {
            fprintf(fi, "%s%s=%08x\n", prefix, e->s, e->data);
            e = e->next;
        }
    }
}
int dict_count(dict_t*h)
{
    return h->num;
}
void* dict_lookup(dict_t*h, const char*s)
{
    unsigned int checksum = string_hash2(s) % h->hashsize;
    dictentry_t*e = h->slots[checksum];
    while(e) {
        if(!strcmp(e->s, s)) {
            return e->data;
        }
        e = e->next;
    }
    return 0;
}
char dict_del(dict_t*h, const char*s)
{
    unsigned int checksum = string_hash2(s) % h->hashsize;
    dictentry_t*head = h->slots[checksum];
    dictentry_t*e = head, *prev=0;
    while(e) {
        if(!strcmp(e->s, s)) {
            dictentry_t*next = e->next;
            free((void*)e->s);
            memset(e, 0, sizeof(dictentry_t));
            free(e);
            if(e == head) {
                h->slots[checksum] = 0;
            } else {
                prev->next = next;
            }
            return 1;
        }
        prev = e;
        e = e->next;
    }
    return 0;
}
void dict_foreach_value(dict_t*h, void (*runFunction)(void*))
{
    int t;
    for(t=0;t<h->hashsize;t++) {
        dictentry_t*e = h->slots[t];
        while(e) {
            dictentry_t*next = e->next;
            if(runFunction) {
                runFunction(e->data);
            }
            e = e->next;
        }
    }
}
void dict_free_all(dict_t*h, void (*freeFunction)(void*))
{
    int t;
    for(t=0;t<h->hashsize;t++) {
        dictentry_t*e = h->slots[t];
        while(e) {
            dictentry_t*next = e->next;
            free((void*)e->s);
            if(freeFunction) {
                freeFunction(e->data);
            }
            memset(e, 0, sizeof(dictentry_t));
            free(e);
            e = next;
        }
    }
    free(h->slots);h->slots = 0;
}
void dict_clear(dict_t*h) 
{
    dict_free_all(h, 0);
}
void dict_destroy(dict_t*dict)
{
    dict_clear(dict);
    free(dict);
}

// ------------------------------- map_t --------------------------------------

typedef struct _map_internal_t
{
    dict_t d;
} map_internal_t;

void map_init(map_t*map)
{
    map_internal_t*m;
    map->internal = (map_internal_t*)malloc(sizeof(map_internal_t));
    memset(map->internal, 0, sizeof(map_internal_t));
    m = (map_internal_t*)map->internal;
    dict_init(&m->d);
}
void map_put(map_t*map, string_t t1, string_t t2)
{
    map_internal_t*m = (map_internal_t*)map->internal;
    dict_put2(&m->d, string_cstr(&t1), (void*)string_cstr(&t2));
}
const char* map_lookup(map_t*map, const char*name)
{
    map_internal_t*m = (map_internal_t*)map->internal;
    const char*value = dict_lookup(&m->d, name);
    return value;
}
void map_dump(map_t*map, FILE*fi, const char*prefix)
{
    int t;
    map_internal_t*m = (map_internal_t*)map->internal;
    fprintf(fi, "ERROR: map dumping not implemented yet\n");
    /*for(t=0;t<m->num;t++) {
	string_t s1 = stringarray_at2(&m->keys, t);
	string_t s2 = stringarray_at2(&m->values, t);
	fprintf(fi, "%s%s=%s\n", prefix, s1.str, s2.str);
    }*/
}
void map_clear(map_t*map)
{
    map_internal_t*m = (map_internal_t*)map->internal;
    dict_clear(&m->d);
    free(m);
}
void map_destroy(map_t*map)
{
    map_clear(map);
    free(map);
}

