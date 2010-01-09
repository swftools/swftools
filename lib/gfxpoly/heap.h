/* heap.h

   An inline heap implementation

   Copyright (c) 2009 Matthias Kramm <kramm@quiss.org>

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

#define HEAP_DEFINE(name,t,lt)                                         \
typedef struct {                                                       \
    t**elements;                                                       \
    int size;                                                          \
    int max_size;                                                      \
} name##_t;                                                            \
static void name##_put(name##_t*h, t*e)                                \
{                                                                      \
    int parent = h->size++;                                            \
    if(h->size>=h->max_size) {                                         \
        h->max_size = h->max_size<15?15:(h->max_size+1)*2-1;           \
        h->elements = (t**)realloc(h->elements,                        \
		                      h->max_size*sizeof(t*));         \
    }                                                                  \
    int node;                                                          \
    do {                                                               \
	node = parent;                                                 \
	if(!node) break;                                               \
	parent = (node-1)/2;                                           \
	h->elements[node] = h->elements[parent];                       \
    } while(lt(e, h->elements[parent]));                               \
    h->elements[node] = e;                                             \
}                                                                      \
static t* name##_get(name##_t*h)                                       \
{                                                                      \
    if(!h->size) return 0;                                             \
    t*r = h->elements[0];                                              \
    int node,child = 0;                                                \
    t*node_p = h->elements[--h->size];                                 \
    h->elements[0] = node_p; /* for the size==1 case */                \
    do {                                                               \
	node = child;                                                  \
	child = node<<1|1;                                             \
	if(child >= h->size) {                                         \
            break;                                                     \
	}                                                              \
        if(child+1 < h->size && lt(h->elements[child+1],               \
		                   h->elements[child]))                \
	    child++;                                                   \
	h->elements[node] = h->elements[child];                        \
    } while(lt(h->elements[child],node_p));                            \
    h->elements[node] = node_p;                                        \
    return r;                                                          \
}                                                                      \
static void name##_init(name##_t*h)                                    \
{                                                                      \
    memset(h, 0, sizeof(*h));                                          \
    h->max_size = 15;                                                  \
    h->elements = malloc(h->max_size*sizeof(t*));                      \
}                                                                      \
static void name##_destroy(name##_t*h)                                 \
{                                                                      \
    free((h)->elements);                                               \
}
