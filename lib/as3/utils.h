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

#ifndef __as3_utils_h__
#define __as3_utils_h__

#define NEW(t,y) t*y = (t*)malloc(sizeof(t));memset(y, 0, sizeof(t));

#define DECLARE(x) struct _##x;typedef struct _##x x##_t;
#define DECLARE_LIST(x) \
struct _##x##_list { \
    struct _##x* x; \
    struct _##x##_list*next; \
}; \
typedef struct _##x##_list x##_list_t; \

int list_length(void*_list);
void list_append(void*_list, void*entry);

typedef struct _dict_entry {
    const char*name;
    void*data;
} dict_entry_t;

typedef struct _dict {
    int num;
    int size;
    dict_entry_t*d;
} dict_t;

dict_t* dict_new();
void dict_free(dict_t*dict);
const char*dict_getstr(dict_t*dict, int nr);
char*dict_getdata(dict_t*dict, int nr);
int dict_append(dict_t*dict, const char*name, void*data);
int dict_find(dict_t*dict, const char*name);
int dict_find2(dict_t*dict, const char*name, void*data);
int dict_update(dict_t*dict, const char*name, void*data);
int dict_append_if_new(dict_t*dict, const char*name, void*data);

#endif
