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
static int mem_put_(mem_t*m,void*data, int length, int null)
{
    int n = m->pos;
    m->pos += length + (null?1:0);
    if(m->pos > m->len)
    { 
	//m->len += 1024>length?1024:(null?length*2:length);

	m->len *= 2;
	while(m->len < m->pos)
	    m->len += 64;

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

// ------------------------------- string_t -----------------------------------

void string_set2(string_t*str, char*text, int len)
{
    str->len = len;
    str->str = text;
}
void string_set(string_t*str, char*text)
{
    str->len = strlen(text);
    str->str = text;
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
    if(str->len == l && !strncmp(str->str, text, l))
	return 1;
    return 0;
}
int string_equals2(string_t*str, string_t*str2)
{
    if(str->len == str2->len && !strncmp(str->str, str2->str, str->len))
	return 1;
    return 0;
}
char* string_cstr(string_t*str)
{
    return strdup_n(str->str, str->len);
}

// ------------------------------- stringarray_t ------------------------------

typedef struct _stringarray_internal_t
{
    mem_t data;
    mem_t pos;
    int num;
} stringarray_internal_t;
void stringarray_init(stringarray_t*sa)
{
    stringarray_internal_t*s;
    sa->internal = (stringarray_internal_t*)malloc(sizeof(stringarray_internal_t)); 
    memset(sa->internal, 0, sizeof(stringarray_internal_t));
    s = (stringarray_internal_t*)sa->internal;
    mem_init(&s->data);
    mem_init(&s->pos);
}
void stringarray_put(stringarray_t*sa, string_t str)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    int pos;
    pos = mem_putstring(&s->data, str);
    mem_put(&s->pos, &pos, sizeof(int));
    s->num++;
}
char* stringarray_at(stringarray_t*sa, int pos)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    int p;
    if(pos<0 || pos>=s->num)
	return 0;
    p = *(int*)&s->pos.buffer[pos*sizeof(int)];
    if(p<0)
	return 0;
    return &s->data.buffer[p];
}
string_t stringarray_at2(stringarray_t*sa, int pos)
{
    string_t s;
    s.str = stringarray_at(sa, pos);
    s.len = s.str?strlen(s.str):0;
    return s;
}
void stringarray_del(stringarray_t*sa, int pos)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    *(int*)&s->pos.buffer[pos*sizeof(int)] = -1;
}
int stringarray_find(stringarray_t*sa, string_t* str)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    int t;
    for(t=0;t<s->num;t++) {
	string_t s = stringarray_at2(sa, t);
	if(s.str && string_equals2(&s, str)) {
	    return t;
	}
    }
    return -1;
}
void stringarray_clear(stringarray_t*sa)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    mem_clear(&s->data);
    mem_clear(&s->pos);
    free(s);
}
void stringarray_destroy(stringarray_t*sa)
{
    stringarray_clear(sa);
    free(sa);
}


// ------------------------------- map_t --------------------------------------

typedef struct _map_internal_t
{
    stringarray_t keys;
    stringarray_t values;
    int num;
} map_internal_t;

void map_init(map_t*map)
{
    map_internal_t*m;
    map->internal = (map_internal_t*)malloc(sizeof(map_internal_t));
    memset(map->internal, 0, sizeof(map_internal_t));
    m = (map_internal_t*)map->internal;
    stringarray_init(&m->keys);
    stringarray_init(&m->values);
}
void map_put(map_t*map, string_t t1, string_t t2)
{
    map_internal_t*m = (map_internal_t*)map->internal;
    stringarray_put(&m->keys, t1);
    stringarray_put(&m->values, t2);
    m->num++;
}
char* map_lookup(map_t*map, const char*name)
{
    int s;
    map_internal_t*m = (map_internal_t*)map->internal;
    string_t str;
    string_set(&str, (char*)name);
    s = stringarray_find(&m->keys, &str);
    if(s>=0) {
	string_t s2 = stringarray_at2(&m->values, s);
	return s2.str;
    }
    return 0;
}
void map_dump(map_t*map, FILE*fi, const char*prefix)
{
    int t;
    map_internal_t*m = (map_internal_t*)map->internal;
    for(t=0;t<m->num;t++) {
	string_t s1 = stringarray_at2(&m->keys, t);
	string_t s2 = stringarray_at2(&m->values, t);
	fprintf(fi, "%s%s=%s\n", prefix, s1.str, s2.str);
    }
}
void map_clear(map_t*map)
{
    map_internal_t*m = (map_internal_t*)map->internal;
    stringarray_clear(&m->keys);
    stringarray_clear(&m->values);
    free(m);
}
void map_destroy(map_t*map)
{
    map_clear(map);
    free(map);
}

// ------------------------------- dictionary_t -------------------------------

typedef struct _dictionary_internal_t
{
    stringarray_t keys;
    mem_t values;
    int num;
} dictionary_internal_t;

void dictionary_init(dictionary_t*dict)
{
    dictionary_internal_t*d;
    dict->internal = (dictionary_internal_t*)malloc(sizeof(dictionary_internal_t));
    memset(dict->internal, 0, sizeof(dictionary_internal_t));
    d = (dictionary_internal_t*)dict->internal;
    stringarray_init(&d->keys);
    mem_init(&d->values);
}
void dictionary_put(dictionary_t*dict, string_t t1, void* t2)
{
    dictionary_internal_t*d = (dictionary_internal_t*)dict->internal;
    int s=0;
    s = stringarray_find(&d->keys, &t1);
    if(s>=0) {
	/* replace */
	*(void**)(&d->values.buffer[s*sizeof(void*)]) = t2;
    } else {
	stringarray_put(&d->keys, t1);
	mem_put(&d->values, &t2, sizeof(void*));
	d->num++;
    }
}
void dictionary_put2(dictionary_t*dict, const char*t1, void* t2)
{
    string_t s;
    string_set(&s, (char*)t1);
    dictionary_put(dict, s, t2);
}
stringarray_t* dictionary_index(dictionary_t*dict)
{
    dictionary_internal_t*d = (dictionary_internal_t*)dict->internal;
    return &d->keys;
}
int dictionary_count(dictionary_t* dict) // this count includes entries that have been deleted
{
    dictionary_internal_t*d = (dictionary_internal_t*)dict->internal;
    return d->num;
}
void* dictionary_lookup(dictionary_t*dict, const char*name)
{
    int s;
    dictionary_internal_t*d = (dictionary_internal_t*)dict->internal;
    string_t str;
    string_set(&str, (char*)name);
    s = stringarray_find(&d->keys, &str);
    if(s>=0) {
	return *(void**)&d->values.buffer[sizeof(void*)*s];
    }
    return 0;
}
void dictionary_dump(dictionary_t*dict, FILE*fi, const char*prefix)
{
    dictionary_internal_t*d = (dictionary_internal_t*)dict->internal;
    int t;
    for(t=0;t<d->num;t++) {
	string_t s1 = stringarray_at2(&d->keys, t);
	fprintf(fi, "%s%s=%08x\n", prefix, s1.str, *(void**)&d->values.buffer[sizeof(void*)*t]);
    }
}
void dictionary_del(dictionary_t*dict, const char* name)
{
    dictionary_internal_t*d = (dictionary_internal_t*)dict->internal;
    int s;
    string_t str;
    string_set(&str, (char*)name);
    s = stringarray_find(&d->keys, &str);
    if(s>=0) {
	*(void**)(&d->values.buffer[s*sizeof(void*)]) = 0;
	stringarray_del(&d->keys, s);
    }
}
void dictionary_clear(dictionary_t*dict)
{
    dictionary_internal_t*d = (dictionary_internal_t*)dict->internal;
    stringarray_clear(&d->keys);
    mem_clear(&d->values);
    free(d);
}
void dictionary_destroy(dictionary_t*dict)
{
    dictionary_clear(dict);
    free(dict);
}

void dictionary_free_all(dictionary_t* dict, void (*freeFunction)(void*))
{
    dictionary_internal_t*d = (dictionary_internal_t*)dict->internal;
    int num = 0;
    char* name = stringarray_at(&d->keys, num)    ;
    while (name)
    {
        freeFunction(dictionary_lookup(dict, name));
        num++;
        name = stringarray_at(&d->keys, num);
    }
    dictionary_clear(dict);
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

