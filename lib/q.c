/* q.c

   Part of the swftools package.
   
   Copyright (c) 2001,2002,2003,2004 Matthias Kramm <kramm@quiss.org>
 
   This program is rfx_free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the rfx_free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the rfx_free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <memory.h>
#include "mem.h"
#include "types.h"
#include "q.h"

// ------------------------------- malloc, alloc routines ---------------------

#ifndef STRNDUP
char* strdup_n(const char*str, int size)
{
    char*m = (char*)rfx_alloc(size+1);
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
char* allocprintf(const char*format, ...)
{
    va_list arglist1;
    va_start(arglist1, format);
    char dummy;
    int l = vsnprintf(&dummy, 1, format, arglist1);
    va_end(arglist1);

    va_list arglist2;
    va_start(arglist2, format);
    char*buf = malloc(l+1);
    vsnprintf(buf, l+1, format, arglist2);
    va_end(arglist2);
    return buf;
}

// ------------------------------- mem_t --------------------------------------

void mem_init(mem_t*mem)
{
    memset(mem, 0, sizeof(mem_t));
}
void mem_clear(mem_t*mem)
{
    rfx_free(mem->buffer);mem->buffer = 0;
}
void mem_destroy(mem_t*mem)
{
    mem_clear(mem);
    rfx_free(mem);
}
static int mem_put_(mem_t*m,const void*data, int length, int null)
{
    int n = m->pos;
    m->pos += length + (null?1:0);
    if(m->pos > m->len) { 
        int v1 = (m->pos+63)&~63;
        int v2 = m->len + m->len / 2;
        m->len = v1>v2?v1:v2;
	m->buffer = m->buffer?(char*)rfx_realloc(m->buffer,m->len):(char*)rfx_alloc(m->len);
    }
    assert(n+length <= m->len);
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
int mem_get(mem_t*m, void*data, int length)
{
    if(m->read_pos + length > m->pos) {
        length = m->pos - m->read_pos;
    }
    memcpy(data, m->buffer+m->read_pos, length);
    m->read_pos += length;
    return length;
}

// ------------------------------- median -------------------------------------

float medianf(float*a, int n)
{
    int i,j,l,m;
    float x;
    int k=n&1?n/2:n/2-1;
    l=0; 
    m=n-1;
    while(l<m) {
        x=a[k];
        i=l;j=m;
        do {
            while(a[i]<x) i++;
            while(x<a[j]) j--;
            if(i<=j) {
		//swap
		float t = a[i];
		a[i] = a[j];
		a[j] = t;
                i++;
		j--;
            }
        } while(i<=j);
        if(j<k) l=i;
        if(k<i) m=j;
    }
    return a[k];
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
    ringbuffer_internal_t*i = (ringbuffer_internal_t*)rfx_calloc(sizeof(ringbuffer_internal_t)); 
    memset(r, 0, sizeof(ringbuffer_t));
    r->internal = i;
    i->buffer = (unsigned char*)rfx_alloc(1024);
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

	buf2 = (unsigned char*)rfx_alloc(newbuffersize);
	ringbuffer_read(r, buf2, r->available);
	rfx_free(i->buffer);
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
    rfx_free(i->buffer);i->buffer = 0;
    rfx_free(i);
}

// ------------------------------- heap_t -------------------------------

void heap_init(heap_t*h,int elem_size, int(*compare)(const void *, const void *))
{
    memset(h, 0, sizeof(heap_t));
    h->size = 0;
    h->elem_size = elem_size;
    h->compare = compare;
    h->elements = 0;
    h->max_size = 0;
}
heap_t* heap_new(int elem_size, int(*compare)(const void *, const void *))
{
    heap_t*h = malloc(sizeof(heap_t));
    heap_init(h, elem_size, compare);
    return h;
}
heap_t* heap_clone(heap_t*o)
{
    heap_t*h = malloc(sizeof(heap_t));
    memcpy(h, o, sizeof(heap_t));
    h->elements = rfx_alloc(sizeof(void*)*h->size);
    int t;
    for(t=0;t<h->size;t++) {
        h->elements[t] = rfx_alloc(h->elem_size);
        memcpy(h->elements[t], o->elements[t], h->elem_size);
    }
    return h;
}
void heap_clear(heap_t*h)
{
    int t;
    for(t=0;t<h->size;t++) {
        rfx_free(h->elements[t]);
        h->elements[t]=0;
    }
    rfx_free(h->elements);
}
void heap_destroy(heap_t*h)
{
    heap_clear(h);
    free(h);
}

#define HEAP_NODE_LARGER(h,node1,node2) ((h)->compare((node1),(node2))>0)
#define HEAP_NODE_SMALLER(h,node1,node2) ((h)->compare((node1),(node2))<0)

static void up(heap_t*h, int node)
{
    void*node_p = h->elements[node];
    int parent = node;
    int tmp = node;
    do {
	node = parent;
	if(!node) break;
	parent = (node-1)/2;
	h->elements[node] = h->elements[parent];
    } while(HEAP_NODE_SMALLER(h, h->elements[parent], node_p));
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
    void*data = rfx_alloc(h->elem_size);
    memcpy(data,e,h->elem_size);

    if(pos>=h->max_size) {
        h->max_size = h->max_size<15?15:(h->max_size+1)*2-1;
        h->elements = (void**)rfx_realloc(h->elements, h->max_size*sizeof(void*));
        assert(pos<h->max_size);
    }

    h->elements[pos] = data;
    up(h, pos);
}
int heap_size(heap_t*h)
{
    return h->size;
}
void* heap_peek(heap_t*h)
{
    if(!h || !h->size) 
        return 0;
    return h->elements[0];
}
void* heap_chopmax(heap_t*h)
{
    if(!h->size)
        return 0;
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
    void**nodes = (void**)rfx_alloc((h->size+1)*sizeof(void*));
    void**p = nodes;
   
    while(h->size) {
	/*printf("Heap Size: %d\n", h->size);
	heap_print(stdout, h);
	printf("\n");*/
	*p++ = heap_chopmax(h);
    }
    *p++ = 0;
    return nodes;
}

// ------------------------------- trie --------------------------------------

trie_t*trie_new()
{
    return (trie_t*)rfx_calloc(sizeof(trie_t));
}
static char _trie_put(trielayer_t**t, unsigned const char*id, void*data)
{
    if(!*t) {
        (*t) = rfx_calloc(sizeof(trielayer_t));
        (*t)->rest = (unsigned char*)strdup((char*)id);
        (*t)->data = data;
        return 0;
    } 
    if((*t)->rest && (*t)->rest[0]) {
        // make room: shift whatever's currently in here one node down
        _trie_put(&(*t)->row[(*t)->rest[0]], (*t)->rest+1, (*t)->data);
        (*t)->rest = 0;
    }
    if(id[0]) {
        return _trie_put(&(*t)->row[id[0]], id+1, data);
    } else {
        char overwrite = 0;
        if((*t)->rest) 
            overwrite = 1;
        (*t)->rest = (unsigned char*)strdup("");
        (*t)->data = data;
        return overwrite;
    }
}
static char _trie_remove(trielayer_t*t, unsigned const char*id)
{
    while(t) {
        if(t->rest && !strcmp((char*)t->rest, (char*)id)) {
            free(t->rest);
            t->rest = 0;
            return 1;
        }
        if(!*id) 
            return 0;
        t = t->row[*id++];
    }
    return 0;
}

static void trie_rollback_removes(trie_t*t, unsigned const char*id, void*data);
static void trie_rollback_adds(trie_t*t, unsigned const char*id, void*data);

void trie_put(trie_t*t, unsigned const char*id, void*data)
{
    if(!t->rollback) {
        _trie_put(&t->start, id, data);
    } else {
        char contains = trie_contains(t, id);
        void*olddata = contains?trie_lookup(t, id):0;
        _trie_put(&t->start, id, data);
        if(contains) {
            trie_rollback_adds(t, id, olddata);
        }
        trie_rollback_removes(t, id, data);
    }
}
char trie_remove(trie_t*t, unsigned const char*id)
{
    if(!t->rollback) {
        return _trie_remove(t->start, id);
    } else {
        void*olddata = trie_lookup(t, id);
        char exists = _trie_remove(t->start, id);
        if(exists) {
            trie_rollback_adds(t, id, olddata);
        }
        return exists;
    }
}
int trie_contains(trie_t*trie, unsigned const char*id)
{
    trielayer_t*t = trie->start;
    while(t) {
        if(t->rest && !strcmp((char*)t->rest, (char*)id))
            return 1;
        if(!*id) 
            return 0;
        t = t->row[*id++];
    }
    return 0;
}
void* trie_lookup(trie_t*trie, unsigned const char*id)
{
    trielayer_t*t = trie->start;
    while(t) {
        if(t->rest && !strcmp((char*)t->rest, (char*)id))
            return t->data;
        if(!*id) 
            return 0;
        t = t->row[*id++];
    }
    return 0;
}

typedef struct _triememory {
    const unsigned char*key;
    void*data;
    char del; // 0/1
    struct _triememory*next;
} triememory_t;

typedef struct _trierollback {
    triememory_t*ops;
    struct _trierollback*prev;
} trierollback_t;

static void trie_rollback_adds(trie_t*t, unsigned const char*id, void*data)
{
    trierollback_t*rollback = (trierollback_t*)t->rollback;
    triememory_t*m = (triememory_t*)rfx_calloc(sizeof(triememory_t));
    m->key = id;
    m->data = data;
    m->del = 0;
    m->next = rollback->ops;
    rollback->ops = m;
}
static void trie_rollback_removes(trie_t*t, unsigned const char*id, void*data)
{
    trierollback_t*rollback = (trierollback_t*)t->rollback;
    triememory_t*m = (triememory_t*)rfx_calloc(sizeof(triememory_t));
    m->key = id;
    m->data = data;
    m->del = 1;
    m->next = rollback->ops;
    rollback->ops = m;
}

void _trie_dump(trielayer_t*t, char*buffer, int pos)
{
    int i;
    for(i=0;i<256;i++) {
        if(t->row[i]) {
            buffer[pos]=i;
            _trie_dump(t->row[i], buffer, pos+1);
        }
    }
    if(t->rest) {
        buffer[pos]=0;
        printf("%s%s %08x\n", buffer, t->rest, (int)t->data);
    }
}

void trie_dump(trie_t*t) 
{
    char buffer[256];
    _trie_dump(t->start, buffer, 0);
}


void trie_remember(trie_t*t)
{
    trierollback_t*old = (trierollback_t*)t->rollback;
    t->rollback = (trierollback_t*)rfx_calloc(sizeof(trierollback_t));
    ((trierollback_t*)t->rollback)->prev = old;
}

void trie_rollback(trie_t*t)
{
    trierollback_t*rollback = (trierollback_t*)t->rollback;
    if(!rollback) {
        fprintf(stderr, "Internal error: can't roll back this trie any further\n");
        return;
    }
    t->rollback = ((trierollback_t*)t->rollback)->prev;

    triememory_t*op = rollback->ops;
    while(op) {
        triememory_t*next = op->next;
        if(op->del) {
            if(!_trie_remove(t->start, op->key)) {
                fprintf(stderr, "Internal error: can't delete key %s in trie during rollback\n", op->key);
            }
        } else {
            if(_trie_put(&t->start, op->key, op->data)) {
                fprintf(stderr, "Internal error: overwrote key %s in trie during rollback\n", op->key);
            }
        }
        free(op);
        op = next;
    }
}


// ------------------------------- crc32 --------------------------------------
static unsigned int crc32[256];
static char crc32_initialized=0;
static void crc32_init(void)
{
    int t;
    if(crc32_initialized)
        return;
    crc32_initialized = 1;
    for(t=0; t<256; t++) {
        unsigned int c = t;
        int s;
        for (s = 0; s < 8; s++) {
          c = (0xedb88320L*(c&1)) ^ (c >> 1);
        }
        crc32[t] = c;
    }
}
static uint64_t crc64[256];
static char crc64_initialized=0;
static void crc64_init(void)
{
    int t;
    if(crc64_initialized)
        return;
    crc64_initialized = 1;
    for(t=0; t<256; t++) {
        unsigned int c = t;
        int s;
        for (s = 0; s < 8; s++) {
          c = ((c&1)?0xC96C5795D7870F42ll:0) ^ (c >> 1);
        }
        crc64[t] = c;
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
    if(text) {
        str->len = strlen(text);
    } else {
        str->len = 0;
    }
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
    if(text) {
        s.len = strlen(text);
    } else {
        s.len = 0;
    }
    s.str = text;
    return s;
}
string_t* string_new3(const char*text, int len)
{
    if(!text) {
        string_t*s = malloc(sizeof(string_t));
        s->len = 0;
        s->str = 0;
        return s;
    } else {
        string_t*s = malloc(sizeof(string_t)+len+1);
        s->len = len;
        s->str = (const char*)(s+1);
        memcpy((char*)s->str, text, len);
        ((char*)s->str)[len]=0;
        return s;
    }
}
string_t* string_new4(const char*text)
{
    int l = strlen(text);
    return string_new3(text, l);
}

void string_free(string_t*s)
{
    if(!s) 
        return;
    s->len = 0;
    if((string_t*)(s->str) == s+1) {
        s->str = 0;
        rfx_free(s);
    } else {
        rfx_free((char*)(s->str));
        s->str = 0;
        rfx_free(s);
    }
}
char* string_cstr(string_t*str)
{
    return strdup_n(str->str, str->len);
}
char* string_escape(string_t*str)
{
    int t;
    int len = 0;
    for(t=0;t<str->len;t++) {
        if(str->str[t]<0x20)
            len+=3;
        else
            len++;
    }
    char*s = malloc(len+1);
    char*p=s;
    for(t=0;t<str->len;t++) {
        if(str->str[t]<0x20) {
            *p++ ='\\';
            unsigned char c = str->str[t];
            *p++ = "0123456789abcdef"[c>>4];
            *p++ = "0123456789abcdef"[c&0x0f];
        } else {
            *p++ = str->str[t];
        }
    }
    *p++ = 0;
    assert(p == &s[len+1]);
    return s;
}

unsigned int crc32_add_byte(unsigned int checksum, unsigned char b) 
{
    crc32_init();
    return checksum>>8 ^ crc32[(b^checksum)&0xff];
}
unsigned int crc32_add_string(unsigned int checksum, const char*s)
{
    crc32_init();
    if(!s)
        return checksum;
    while(*s) {
        checksum = checksum>>8 ^ crc32[(*s^checksum)&0xff];
        s++;
    }
    return checksum;
}
unsigned int crc32_add_bytes(unsigned int checksum, const void*_s, int len)
{
    unsigned char*s = (unsigned char*)_s;
    crc32_init();
    if(!s || !len)
        return checksum;
    do {
        checksum = checksum>>8 ^ crc32[(*s^checksum)&0xff];
        s++;
    } while(--len);
    return checksum;
}

unsigned int string_hash(const string_t*str)
{
    int t;
    unsigned int checksum = 0;
    crc32_init();
    for(t=0;t<str->len;t++) {
        checksum = checksum>>8 ^ crc32[(str->str[t]^checksum)&0xff];
    }
    return checksum;
}
unsigned int string_hash2(const char*str)
{
    unsigned int checksum = 0;
    const char*p = str;
    crc32_init();
    while(*p) {
        checksum = checksum>>8 ^ crc32[(*p^checksum)&0xff];
        p++;
    }
    return checksum;
}
uint64_t string_hash64(const char*str)
{
    uint64_t checksum = 0;
    const char*p = str;
    crc64_init();
    while(*p) {
        checksum = checksum>>8 ^ crc64[(*p^checksum)&0xff];
        p++;
    }
    return checksum;
}
unsigned int string_hash3(const char*str, int len)
{
    string_t s;
    s.str = str;
    s.len = len;
    return string_hash(&s);
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

char* concat2(const char* t1, const char* t2)
{
    int l1 = strlen(t1);
    int l2 = strlen(t2);
    char*text = malloc(l1+l2+1);
    memcpy(text   , t1, l1);
    memcpy(text+l1, t2, l2);
    text[l1+l2] = 0;
    return text;
}

char* concat3(const char* t1, const char* t2, const char* t3)
{
    int l1 = strlen(t1);
    int l2 = strlen(t2);
    int l3 = strlen(t3);
    char*text = malloc(l1+l2+l3+1);
    memcpy(text   , t1, l1);
    memcpy(text+l1, t2, l2);
    memcpy(text+l1+l2, t3, l3);
    text[l1+l2+l3] = 0;
    return text;
}

// ------------------------------- stringarray_t ------------------------------

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
    sa->internal = (stringarray_internal_t*)rfx_calloc(sizeof(stringarray_internal_t)); 
    s = (stringarray_internal_t*)sa->internal;
    mem_init(&s->pos);
    s->hash = rfx_calloc(sizeof(stringlist_t*)*hashsize);
    s->hashsize = hashsize;
}
void stringarray_put(stringarray_t*sa, string_t str)
{
    stringarray_internal_t*s = (stringarray_internal_t*)sa->internal;
    int pos;
    int hash = string_hash(&str) % s->hashsize;

    char*ss = string_cstr(&str);
    mem_put(&s->pos, &ss, sizeof(char*));

    stringlist_t*l = rfx_alloc(sizeof(stringlist_t));
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
            rfx_free(l);
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
            rfx_free(l);
            l = next;
        }
    }
    rfx_free(s->hash);s->hash=0;
    rfx_free(s);
}
void stringarray_destroy(stringarray_t*sa)
{
    stringarray_clear(sa);
    rfx_free(sa);
}

// ------------------------------- type_t -------------------------------

char ptr_equals(const void*o1, const void*o2) 
{
    return o1==o2;
}
unsigned int ptr_hash(const void*o) 
{
    return string_hash3((const char*)&o, sizeof(o));
}
void* ptr_dup(const void*o) 
{
    return (void*)o;
}
void ptr_free(void*o) 
{
    return;
}

char int_equals(const void*o1, const void*o2) 
{
    return o1==o2;
}
unsigned int int_hash(const void*o) 
{
    return string_hash3((const char*)&o, sizeof(o));
}
void* int_dup(const void*o) 
{
    return (void*)o;
}
void int_free(void*o) 
{
    return;
}

char charptr_equals(const void*o1, const void*o2) 
{
    if(!o1 || !o2)
        return o1==o2;
    return !strcmp(o1,o2);
}
unsigned int charptr_hash(const void*o) 
{
    if(!o)
        return 0;
    return string_hash2(o);
}
void* charptr_dup(const void*o) 
{
    if(!o)
        return 0;
    return strdup(o);
}
void charptr_free(void*o) 
{
    if(o) {
        rfx_free(o);
    }
}

char stringstruct_equals(const void*o1, const void*o2) 
{
    if(!o1 || !o2) 
        return o1==o2;
    string_t*s1 = (string_t*)o1;
    string_t*s2 = (string_t*)o2;
    int l = s1->len<s2->len?s1->len:s2->len;
    int r = memcmp(s1->str, s2->str, l);
    if(r)
        return 0;
    else
        return s1->len==s2->len;
}
unsigned int stringstruct_hash(const void*o) 
{
    if(!o) return 0;
    return string_hash(o);
}
string_t*string_dup3(string_t*o)
{
    if(!o) return 0;
    if(!o->str) {
        string_t*s = malloc(sizeof(string_t));
        s->str=0;
        s->len=0;
        return s;
    }
    string_t*s = rfx_alloc(sizeof(string_t)+o->len+1);
    s->len = o->len;
    s->str = (const char*)(s+1);
    memcpy((char*)s->str, o->str, s->len);
    ((char*)s->str)[s->len]=0;
    return s;
}
void stringstruct_free(void*o) 
{
    if(o)
        string_free(o);
}

type_t int_type = {
    equals: int_equals,
    hash: int_hash,
    dup: int_dup,
    free: int_free,
};

type_t ptr_type = {
    equals: ptr_equals,
    hash: ptr_hash,
    dup: ptr_dup,
    free: ptr_free,
};

type_t charptr_type = {
    equals: charptr_equals,
    hash: charptr_hash,
    dup: charptr_dup,
    free: charptr_free,
};

type_t stringstruct_type = {
    equals: stringstruct_equals,
    hash: stringstruct_hash,
    dup: (dup_func)string_dup3,
    free: stringstruct_free,
};

// ------------------------------- dictionary_t -------------------------------

#define INITIAL_SIZE 1

static int max(int x, int y) {
    return x>y?x:y;
}

dict_t*dict_new()
{
    dict_t*d = rfx_alloc(sizeof(dict_t));
    dict_init(d, INITIAL_SIZE);
    return d;
}
dict_t*dict_new2(type_t*t)
{
    dict_t*d = rfx_alloc(sizeof(dict_t));
    dict_init(d, INITIAL_SIZE);
    d->key_type = t;
    return d;
}
void dict_init(dict_t*h, int size) 
{
    memset(h, 0, sizeof(dict_t));
    h->hashsize = size;
    h->slots = h->hashsize?(dictentry_t**)rfx_calloc(sizeof(dictentry_t*)*h->hashsize):0;
    h->num = 0;
    h->key_type = &charptr_type;
}
void dict_init2(dict_t*h, type_t*t, int size) 
{
    memset(h, 0, sizeof(dict_t));
    h->hashsize = size;
    h->slots = h->hashsize?(dictentry_t**)rfx_calloc(sizeof(dictentry_t*)*h->hashsize):0;
    h->num = 0;
    h->key_type = t;
}

dict_t*dict_clone(dict_t*o)
{
    dict_t*h = rfx_alloc(sizeof(dict_t));
    memcpy(h, o, sizeof(dict_t));
    h->slots = h->hashsize?(dictentry_t**)rfx_calloc(sizeof(dictentry_t*)*h->hashsize):0;
    int t;
    for(t=0;t<o->hashsize;t++) {
        dictentry_t*e = o->slots[t];
        while(e) {
            dictentry_t*n = (dictentry_t*)rfx_alloc(sizeof(dictentry_t));
            memcpy(n, e, sizeof(dictentry_t));
            n->key = h->key_type->dup(e->key);
            n->data = e->data;
            n->next = h->slots[t];
            h->slots[t] = n;
            e = e->next;
        }
    }
    return h;
}

static void dict_expand(dict_t*h, int newlen)
{
    assert(h->hashsize < newlen);
    dictentry_t**newslots = (dictentry_t**)rfx_calloc(sizeof(dictentry_t*)*newlen);
    int t; 
    for(t=0;t<h->hashsize;t++) {
        dictentry_t*e = h->slots[t];
        while(e) {
            dictentry_t*next = e->next;
            unsigned int newhash = e->hash%newlen;
            e->next = newslots[newhash];
            newslots[newhash] = e;
            e = next;
        }
    }
    if(h->slots)
        rfx_free(h->slots);
    h->slots = newslots;
    h->hashsize = newlen;
}

dictentry_t* dict_put(dict_t*h, const void*key, void* data)
{
    unsigned int hash = h->key_type->hash(key);
    dictentry_t*e = (dictentry_t*)rfx_alloc(sizeof(dictentry_t));
    
    if(!h->hashsize)
        dict_expand(h, 1);

    unsigned int hash2 = hash % h->hashsize;
    
    e->key = h->key_type->dup(key);
    e->hash = hash; //for resizing
    e->next = h->slots[hash2];
    e->data = data;
    h->slots[hash2] = e;
    h->num++;
    return e;
}
void dict_put2(dict_t*h, const char*s, void*data) 
{
    assert(h->key_type == &charptr_type);
    dict_put(h, s, data);
}
void dict_dump(dict_t*h, FILE*fi, const char*prefix)
{
    int t;
    for(t=0;t<h->hashsize;t++) {
        dictentry_t*e = h->slots[t];
        while(e) {
            if(h->key_type!=&charptr_type) {
                fprintf(fi, "%s%08x=%08x\n", prefix, (int)e->key, (int)e->data);
            } else {
                fprintf(fi, "%s%s=%08x\n", prefix, (char*)e->key, (int)e->data);
            }
            e = e->next;
        }
    }
}

int dict_count(dict_t*h)
{
    return h->num;
}

static inline dictentry_t* dict_do_lookup(dict_t*h, const void*key)
{
    if(!h->num) {
        return 0;
    }
    
    unsigned int ohash = h->key_type->hash(key);
    unsigned int hash = ohash % h->hashsize;

    /* check first entry for match */
    dictentry_t*e = h->slots[hash];
    if(e && h->key_type->equals(e->key, key)) {
        return e;
    } else if(e) {
        e = e->next;
    }

    /* if dict is 2/3 filled, double the size. Do
       this the first time we have to actually iterate
       through a slot to find our data */
    if(e && h->num*3 >= h->hashsize*2) {
        int newsize = h->hashsize;
        while(h->num*3 >= newsize*2) {
            newsize = newsize<15?15:(newsize+1)*2-1;
        }
        dict_expand(h, newsize);
        hash = ohash % h->hashsize;
        e = h->slots[hash];
        if(e && h->key_type->equals(e->key, key)) {
            // omit move to front
            return e;
        } else if(e) {
            e = e->next;
        }
    }

    /* check subsequent entries for a match */
    dictentry_t*last = h->slots[hash];
    while(e) {
        if(h->key_type->equals(e->key, key)) {
            /* move to front- makes a difference of about 10% in most applications */
            last->next = e->next;
            e->next = h->slots[hash];
            h->slots[hash] = e;
            return e;
        }
        last=e;
        e = e->next;
    }
    return 0;
}
void* dict_lookup(dict_t*h, const void*key)
{
    dictentry_t*e = dict_do_lookup(h, key);
    if(e)
        return e->data;
    return 0;
}
char dict_contains(dict_t*h, const void*key)
{
    dictentry_t*e = dict_do_lookup(h, key);
    return !!e;
}

char dict_del(dict_t*h, const void*key)
{
    if(!h->num)
        return 0;
    unsigned int hash = h->key_type->hash(key) % h->hashsize;
    dictentry_t*head = h->slots[hash];
    dictentry_t*e = head, *prev=0;
    while(e) {
        if(h->key_type->equals(e->key, key)) {
            dictentry_t*next = e->next;
            h->key_type->free(e->key);
            memset(e, 0, sizeof(dictentry_t));
            rfx_free(e);
            if(e == head) {
                h->slots[hash] = next;
            } else {
                assert(prev);
                prev->next = next;
            }
            h->num--;
            return 1;
        }
        prev = e;
        e = e->next;
    }
    return 0;
}

char dict_del2(dict_t*h, const void*key, void*data)
{
    if(!h->num)
        return 0;
    unsigned int hash = h->key_type->hash(key) % h->hashsize;
    dictentry_t*head = h->slots[hash];
    dictentry_t*e = head, *prev=0;
    while(e) {
        if(h->key_type->equals(e->key, key) && e->data == data) {
            dictentry_t*next = e->next;
            h->key_type->free(e->key);
            memset(e, 0, sizeof(dictentry_t));
            rfx_free(e);
            if(e == head) {
                h->slots[hash] = next;
            } else {
                assert(prev);
                prev->next = next;
            }
            h->num--;
            return 1;
        }
        prev = e;
        e = e->next;
    }
    return 0;
}

dictentry_t* dict_get_slot(dict_t*h, const void*key)
{
    if(!h->num)
        return 0;
    unsigned int ohash = h->key_type->hash(key);
    unsigned int hash = ohash % h->hashsize;
    return h->slots[hash];
}

void dict_foreach_keyvalue(dict_t*h, void (*runFunction)(void*data, const void*key, void*val), void*data)
{
    int t;
    for(t=0;t<h->hashsize;t++) {
        dictentry_t*e = h->slots[t];
        while(e) {
            dictentry_t*next = e->next;
            if(runFunction) {
                runFunction(data, e->key, e->data);
            }
            e = e->next;
        }
    }
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

void dict_free_all(dict_t*h, char free_keys, void (*free_data_function)(void*))
{
    int t;
    for(t=0;t<h->hashsize;t++) {
        dictentry_t*e = h->slots[t];
        while(e) {
            dictentry_t*next = e->next;
            if(free_keys) {
                h->key_type->free(e->key);
            }
            if(free_data_function) {
                free_data_function(e->data);
            }
            memset(e, 0, sizeof(dictentry_t));
            rfx_free(e);
            e = next;
        }
        h->slots[t]=0;
    }
    rfx_free(h->slots);
    memset(h, 0, sizeof(dict_t));
}

void dict_clear_shallow(dict_t*h) 
{
    dict_free_all(h, 0, 0);
}

void dict_clear(dict_t*h) 
{
    dict_free_all(h, 1, 0);
}

void dict_destroy_shallow(dict_t*dict)
{
    dict_clear_shallow(dict);
    rfx_free(dict);
}

void dict_destroy(dict_t*dict)
{
    if(!dict)
        return;
    dict_clear(dict);
    rfx_free(dict);
}

// ------------------------------- mtf_t --------------------------------------
mtf_t* mtf_new(type_t*type)
{
    NEW(mtf_t, mtf);
    mtf->type = type;
    return mtf;
}
void mtf_increase(mtf_t*m, const void*key)
{
    mtf_item_t*item = m->first;
    mtf_item_t*last = 0;
    while(item) {
	if(m->type->equals(item->key, key)) {
	    item->num++;
	    if(item->num>m->first->num) {
		if(last) last->next = item->next;
		else m->first = item->next;
		item->next = m->first;
		m->first = item;
	    }
            return;
	}
	last = item;
	item = item->next;
    }
    NEW(mtf_item_t,n);
    if(last) last->next = n;
    else m->first = n;
    n->key = key;
    n->num = 1;
}
void mtf_destroy(mtf_t*m)
{
    if(!m) return;
    mtf_item_t*item = m->first;
    m->first = 0;
    while(item) {
	mtf_item_t*next = item->next;
	item->next = 0;
	free(item);
	item = next;
    }
    free(m);
}

// ------------------------------- map_t --------------------------------------

typedef struct _map_internal_t
{
    dict_t d;
} map_internal_t;

void map_init(map_t*map)
{
    map_internal_t*m;
    map->internal = (map_internal_t*)rfx_calloc(sizeof(map_internal_t));
    m = (map_internal_t*)map->internal;
    dict_init(&m->d, INITIAL_SIZE);
}
void map_put(map_t*map, string_t t1, string_t t2)
{
    map_internal_t*m = (map_internal_t*)map->internal;
    string_t s;
    char* s1 = string_cstr(&t1);
    dict_put2(&m->d, s1, (void*)string_cstr(&t2));
    rfx_free(s1);
}
const char* map_lookup(map_t*map, const char*name)
{
    map_internal_t*m = (map_internal_t*)map->internal;
    const char*value = dict_lookup(&m->d, name);
    return value;
}
static void freestring(void*data)
{
    rfx_free(data);
}
static void dumpmapentry(void*data, const void*key, void*value)
{
    FILE*fi = (FILE*)data;
    fprintf(fi, "%s=%s\n", (char*)key, (char*)value);
}
void map_dump(map_t*map, FILE*fi, const char*prefix)
{
    int t;
    map_internal_t*m = (map_internal_t*)map->internal;
    dict_foreach_keyvalue(&m->d, dumpmapentry, fi);
}
void map_clear(map_t*map)
{
    map_internal_t*m = (map_internal_t*)map->internal;
    dict_free_all(&m->d, 1, freestring);
    rfx_free(m);
}
void map_destroy(map_t*map)
{
    map_clear(map);
    rfx_free(map);
}

// ------------------------------- array_t --------------------------------------

array_t* array_new() {
    array_t*d = malloc(sizeof(array_t));
    memset(d, 0, sizeof(array_t));
    d->entry2pos = dict_new();
    return d;
}
array_t* array_new2(type_t*type) {
    array_t*d = malloc(sizeof(array_t));
    memset(d, 0, sizeof(array_t));
    d->entry2pos = dict_new2(type);
    return d;
}
void*array_getkey(array_t*array, int nr) {
    if(nr >= array->num || nr<0) {
	fprintf(stderr, "error: reference to element %d in array[%d]\n", nr, array->num);
	return 0;
    }
    return array->d[nr].name;
}
void*array_getvalue(array_t*array, int nr) {
    if(nr >= array->num || nr<0) {
	fprintf(stderr, "error: reference to element %d in array[%d]\n", nr, array->num);
	return 0;
    }
    return array->d[nr].data;
}
int array_append(array_t*array, const void*name, void*data) {
    while(array->size <= array->num) {
	array->size += 64;
	if(!array->d) {
	    array->d = malloc(sizeof(array_entry_t)*array->size);
	} else {
	    array->d = realloc(array->d, sizeof(array_entry_t)*array->size);
	}
    }

    dictentry_t*e = dict_put(array->entry2pos, name, (void*)(ptroff_t)(array->num+1));

    if(name) {
	array->d[array->num].name = e->key;
    } else {
	array->d[array->num].name = 0;
    }
    array->d[array->num].data = (void*)data;
    return array->num++;
}
int array_find(array_t*array, const void*name)
{
    int pos = (int)(ptroff_t)dict_lookup(array->entry2pos, name);
    return pos-1;
}
int array_find2(array_t*array, const void*name, void*data)
{
    dict_t*h= array->entry2pos;
    dictentry_t*e = dict_get_slot(array->entry2pos, name);

    while(e) {
        int index = ((int)(ptroff_t)e->data) - 1;
        if(h->key_type->equals(e->key, name) && array->d[index].data == data) {
            return index;
        }
        e = e->next;
    }
    return -1;
}
int array_update(array_t*array, const void*name, void*data) {
    int pos = array_find(array, name);
    if(pos>=0) {
	array->d[pos].data = data;
	return pos;
    }
    return array_append(array, name, data);
}
int array_append_if_new(array_t*array, const void*name, void*data) {
    int pos = array_find(array, name);
    if(pos>=0)
	return pos;
    return array_append(array, name, data);
}
void array_free(array_t*array) {
    dict_destroy(array->entry2pos);
    if(array->d) {
        free(array->d);array->d = 0;
    }
    free(array);
}

// ------------------------------- list_t --------------------------------------

struct _commonlist;
typedef struct _listinfo {
    int size;
    struct _commonlist*last;
} listinfo_t;

typedef struct _commonlist {
    void*entry;
    struct _commonlist*next;
    listinfo_t info[0];
} commonlist_t;

int list_length_(void*_list)
{
    commonlist_t*l = (commonlist_t*)_list;
    if(!l)
        return 0;
    return l->info[0].size;
}
void list_concat_(void*_l1, void*_l2)
{
    commonlist_t**l1 = (commonlist_t**)_l1;
    commonlist_t**l2 = (commonlist_t**)_l2;

    if(!*l1) {
        *l1 = *l2;
    } else if(*l2) {
        (*l1)->info[0].last->next = *l2;
        (*l1)->info[0].last = (*l2)->info[0].last;
        (*l1)->info[0].size += (*l2)->info[0].size;
    }
    *l2 = 0;
}
void list_append_(void*_list, void*entry)
{
    commonlist_t**list = (commonlist_t**)_list;
    commonlist_t* n = 0;
    if(!*list) {
        n = (commonlist_t*)malloc(sizeof(commonlist_t)+sizeof(listinfo_t));
        *list = n;
        (*list)->info[0].size = 0;
    } else {
        n = malloc(sizeof(commonlist_t));
        (*list)->info[0].last->next = n;
    }
    n->next = 0;
    n->entry = entry;
    (*list)->info[0].last = n;
    (*list)->info[0].size++;
}
/* notice: prepending uses slighly more space than appending */
void list_prepend_(void*_list, void*entry)
{
    commonlist_t**list = (commonlist_t**)_list;
    commonlist_t* n = (commonlist_t*)malloc(sizeof(commonlist_t)+sizeof(listinfo_t));
    int size = 0;
    commonlist_t* last = 0;
    if(*list) {
        last = (*list)->info[0].last;
        size = (*list)->info[0].size;
    }
    n->next = *list;
    n->entry = entry;
    *list = n;
    (*list)->info[0].last = last;
    (*list)->info[0].size = size+1;
}
void list_free_(void*_list) 
{
    commonlist_t**list = (commonlist_t**)_list;
    commonlist_t*l = *list;
    while(l) {
        commonlist_t*next = l->next;
        free(l);
        l = next;
    }
    *list = 0;
}
void list_deep_free_(void*_list)
{
    commonlist_t**list = (commonlist_t**)_list;
    commonlist_t*l = *list;
    while(l) {
        commonlist_t*next = l->next;
        if(l->entry) {
            free(l->entry);l->entry=0;
        }
        free(l);
        l = next;
    }
    *list = 0;
}
void*list_clone_(void*_list) 
{
    commonlist_t*l = *(commonlist_t**)_list;

    void*dest = 0;
    while(l) {
        commonlist_t*next = l->next;
        list_append_(&dest, l->entry);
        l = next;
    }
    return dest;

}

