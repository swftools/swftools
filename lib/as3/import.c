/* import.c

   Extension module for the rfxswf library.
   Part of the swftools package.

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

#include "import.h"
#include "abc.h"
#include "registry.h"
#include "../os.h"

void as3_import_abc(char*filename)
{
    TAG*tag = swf_InsertTag(0, ST_RAWABC);
    memfile_t*file = memfile_open(filename);
    tag->data = file->data;
    tag->len = file->len;
    abc_file_t*abc = swf_ReadABC(tag);
    as3_import_code(abc);
    swf_FreeABC(abc);
    memfile_close(file);
    free(tag);
}

void as3_import_swf(char*filename)
{
    SWF* swf = swf_OpenSWF(filename);
    if(!swf)
        return;
    TAG*tag = swf->firstTag;
    while(tag) {
        if(tag->id == ST_DOABC || tag->id == ST_RAWABC) {
            abc_file_t*abc = swf_ReadABC(tag);
            as3_import_code(abc);
            swf_FreeABC(abc);
        }
        tag = tag->next;
    }
    swf_FreeTags(swf);
    free(swf);
}

void as3_import_file(char*filename)
{
    FILE*fi = fopen(filename, "rb");
    if(!fi) return;
    char head[3];
    fread(head, 3, 1, fi);
    fclose(fi);
    if(!strncmp(head, "FWS", 3) ||
       !strncmp(head, "CWS", 3)) {
        as3_import_swf(filename);
    } else {
        as3_import_abc(filename);
    }
}

static int compare_traits(const void*v1, const void*v2)
{
    trait_t* x1 = *(trait_t**)v1;
    trait_t* x2 = *(trait_t**)v2;
    int i = strcmp(x1->name->ns->name, x2->name->ns->name);
    if(i)
        return i;
    return strcmp(x1->name->name, x2->name->name);
}


void as3_import_code(abc_file_t*abc)
{
    int t;
    for(t=0;t<abc->classes->num;t++) {
        abc_class_t*cls = array_getvalue(abc->classes, t);
        U8 access = cls->classname->ns->access;
        if(access==ACCESS_PRIVATE ||
           access==ACCESS_PACKAGEINTERNAL)
            continue;
        if(!strncmp(cls->classname->ns->name, "__AS3", 5))
            continue;

        const char*package = strdup(cls->classname->ns->name);
        const char*name = strdup(cls->classname->name);
                
        multiname_list_t*i=cls->interfaces;
        classinfo_t*c = classinfo_register(access, package, name, list_length(i));

        if(cls->flags & CLASS_FINAL)
            c->flags |= FLAG_FINAL;
        if(cls->flags & CLASS_INTERFACE)
            c->flags |= FLAG_INTERFACE;
        if(!(cls->flags & CLASS_SEALED))
            c->flags |= FLAG_DYNAMIC;
        /*
          FIXME 
           int nr = 0;
        while(i) {
            c->interfaces[nr++] = i->multiname;
            i = i->next;
        }
        c->superclass = ...
         
         */
      
        trait_list_t*l=0;
        char is_static = 0;
        l = cls->traits;
        while(l) {
            trait_t*trait = l->trait;
            U8 access = trait->name->ns->access;
            if(access==ACCESS_PRIVATE)
                goto cont;
            const char*name = trait->name->name;
            if(registry_findmember(c, name, 0))
                goto cont;
            name = strdup(name);

            memberinfo_t*s = 0;
            if(trait->kind == TRAIT_METHOD) {
                s = (memberinfo_t*)methodinfo_register_onclass(c, access, name);
            } else if(trait->kind == TRAIT_SLOT ||
                      trait->kind == TRAIT_GETTER) {
                s = (memberinfo_t*)varinfo_register_onclass(c, access, name);
            } else {
                goto cont;
            }

            s->flags = is_static?FLAG_STATIC:0;
            s->parent = c;

            cont:
            l = l->next;
            if(!l && !is_static) {
                l = cls->static_traits;
                is_static = 1;
            }
        }
    }

#   define IS_PUBLIC_MEMBER(trait) ((trait)->kind != TRAIT_CLASS)

    /* count public functions */
    int num_methods=0;
    for(t=0;t<abc->scripts->num;t++) {
        trait_list_t*l = ((abc_script_t*)array_getvalue(abc->scripts, t))->traits;
        for(;l;l=l->next) {
            num_methods += IS_PUBLIC_MEMBER(l->trait);
        }
    }
    trait_t**traits = (trait_t**)malloc(num_methods*sizeof(trait_t*));
    num_methods=0;
    for(t=0;t<abc->scripts->num;t++) {
        trait_list_t*l = ((abc_script_t*)array_getvalue(abc->scripts, t))->traits;
        for(;l;l=l->next) {
            if(IS_PUBLIC_MEMBER(l->trait)) {
                traits[num_methods++] = l->trait;
            }
        }
    }
    qsort(traits, num_methods, sizeof(trait_t*), compare_traits);
    for(t=0;t<num_methods;t++) {
        trait_t*trait = traits[t];
        if(IS_PUBLIC_MEMBER(trait)) {
            U8 access = trait->name->ns->access;
            const char*package = strdup(trait->name->ns->name);
            const char*name = strdup(trait->name->name);
            char np = 0;
            memberinfo_t*m = 0;
            if(trait->kind == TRAIT_METHOD) {
                m = (memberinfo_t*)methodinfo_register_global(access, package, name);
            } else {
                m = (memberinfo_t*)varinfo_register_global(access, package, name);
            }
            m->return_type = 0;
            m->parent = 0;
        }
    }
}
