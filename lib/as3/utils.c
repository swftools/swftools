/* utils.c

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2008 Matthias Kramm <kramm@quiss.org>
 
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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "utils.h"

dict_t* dict_new() {
    dict_t*d = malloc(sizeof(dict_t));
    memset(d, 0, sizeof(dict_t));
    return d;
}

void dict_free(dict_t*dict) {
    if(dict->d)
    free(dict->d);dict->d = 0;
    free(dict);
}

const char*dict_getstr(dict_t*dict, int nr) {
    if(nr > dict->num || nr<0) {
	printf("error: reference to string %d in dict\n");
	return 0;
    }
    return dict->d[nr].name;
}
char*dict_getdata(dict_t*dict, int nr) {
    if(nr > dict->num || nr<0) {
	printf("error: reference to string %d in dict\n");
	return 0;
    }
    return dict->d[nr].data;
}
int dict_append(dict_t*dict, const char*name, void*data) {
    while(dict->size <= dict->num) {
	dict->size += 64;
	if(!dict->d) {
	    dict->d = malloc(sizeof(dict_entry_t)*dict->size);
	} else {
	    dict->d = realloc(dict->d, sizeof(dict_entry_t)*dict->size);
	}
    }
    if(name) {
	dict->d[dict->num].name = strdup(name);
    } else {
	dict->d[dict->num].name = 0;
    }
    dict->d[dict->num].data = data;
    return dict->num++;
}
int dict_find(dict_t*dict, const char*name)
{
    if(!name)
	name = "";
    int t;
    for(t=0;t<dict->num;t++) {
	if(dict->d[t].name && !strcmp(dict->d[t].name,name))
	    return t;
    }
    return -1;
}
int dict_find2(dict_t*dict, const char*name, void*data)
{
    if(!name)
	name = "";
    int t;
    for(t=0;t<dict->num;t++) {
	if(dict->d[t].name && !strcmp(dict->d[t].name,name) && dict->d[t].data == data)
	    return t;
    }
    return -1;
}
int dict_update(dict_t*dict, const char*name, void*data) {
    int pos = dict_find(dict, name);
    if(pos>=0) {
	dict->d[pos].data = data;
	return pos;
    }
    return dict_append(dict, name, data);
}
int dict_append_if_new(dict_t*dict, const char*name, void*data) {
    int pos = dict_find(dict, name);
    if(pos>=0)
	return pos;
    return dict_append(dict, name, data);
}
int dict_append_if_new2(dict_t*dict, const char*name, void*data) {
    int pos = dict_find2(dict, name, data);
    if(pos>=0)
	return pos;
    return dict_append(dict, name, data);
}

typedef struct _commonlist {
    void*entry;
    struct _commonlist*next;
    struct _commonlist*last[0];
} commonlist_t;

int list_length(void*_list)
{
    commonlist_t*l = (commonlist_t*)_list;
    int n=0;
    while(l) {
        l = l->next;
        n++;
    }
    return n;
}
void list_append(void*_list, void*entry)
{
    commonlist_t**list = (commonlist_t**)_list;
    commonlist_t* n = 0;
    if(!*list) {
        n = malloc(sizeof(commonlist_t)+sizeof(commonlist_t*));
        *list = n;
    } else {
        n = malloc(sizeof(commonlist_t));
        (*list)->last[0]->next = n;
    }
    n->next = 0;
    n->entry = entry;
    (*list)->last[0] = n;
}

