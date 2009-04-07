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
#include "common.h"
#include "common.h"
#include "tokenizer.h"
#include "../os.h"

static void import_code(void*_abc, char*filename, int pass);

void as3_import_abc(char*filename)
{
    TAG*tag = swf_InsertTag(0, ST_RAWABC);
    memfile_t*file = memfile_open(filename);
    tag->data = file->data;
    tag->len = file->len;
    abc_file_t*abc = swf_ReadABC(tag);
    import_code(abc, filename, 0);
    import_code(abc, filename, 1);
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

    /* pass 1 */
    while(tag) {
        if(tag->id == ST_DOABC || tag->id == ST_RAWABC) {
            abc_file_t*abc = swf_ReadABC(tag);
            import_code(abc, filename, 0);
            swf_FreeABC(abc);
        }
        tag = tag->next;
    }

    tag = swf->firstTag;
    /* pass 2 */
    while(tag) {
        if(tag->id == ST_DOABC || tag->id == ST_RAWABC) {
            abc_file_t*abc = swf_ReadABC(tag);
            import_code(abc, filename, 1);
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

static classinfo_t*resolve_class(char*filename, char*what, multiname_t*n)
{
    if(!n) return 0;
    if(!n->name[0] || !strcmp(n->name, "void")) return 0;

    classinfo_t*c = 0;
    if(n->ns && n->ns->name) {
        c = (classinfo_t*)registry_find(n->ns->name, n->name);
    } else if(n->namespace_set) {
        namespace_list_t*s = n->namespace_set->namespaces;
        while(s) {
            c = (classinfo_t*)registry_find(s->namespace->name, n->name);
            if(c)
                break;
            s = s->next;
        }
    }

    if(!c) {
        as3_warning("import %s: couldn't resolve %s %s.%s", filename, what, n->ns->name, n->name);
        return 0;
    }
    if(c->kind != INFOTYPE_CLASS)
        as3_warning("import %s: %s %s resolves to something that's not a class", filename, what, n->name);
    return c;
}

static void import_code(void*_abc, char*filename, int pass)
{
    abc_file_t*abc = _abc;
    int t;
    if(pass==0) {
        for(t=0;t<abc->classes->num;t++) {
            abc_class_t*cls = array_getvalue(abc->classes, t);
            U8 access = cls->classname->ns->access;
            if(access==ACCESS_PRIVATE ||
               access==ACCESS_PACKAGEINTERNAL)
                continue;
            //if(!strncmp(cls->classname->ns->name, "__AS3", 5))
            //    continue;

            const char*package = strdup(cls->classname->ns->name);
            const char*name = strdup(cls->classname->name);
                    
            multiname_list_t*i=cls->interfaces;
            classinfo_t*c = classinfo_register(access, package, name, list_length(i));
            c->flags|=FLAG_BUILTIN;

            if(cls->flags & CLASS_FINAL)
                c->flags |= FLAG_FINAL;
            if(cls->flags & CLASS_INTERFACE)
                c->flags |= FLAG_INTERFACE;
            if(!(cls->flags & CLASS_SEALED))
                c->flags |= FLAG_DYNAMIC;
        }
        return;
    }
    
    for(t=0;t<abc->classes->num;t++) {
        abc_class_t*cls = array_getvalue(abc->classes, t);
        const char*package = strdup(cls->classname->ns->name);
        const char*name = strdup(cls->classname->name);
        classinfo_t*c = (classinfo_t*)registry_find(package, name);
        if(!c) continue;

        int nr = 0;
        multiname_list_t*i = cls->interfaces;
        while(i) {
            c->interfaces[nr++] = resolve_class(filename, "interface", i->multiname);
            i = i->next;
        }
        c->superclass = resolve_class(filename, "superclass", cls->superclass);
      
        trait_list_t*l=0;
        char is_static = 0;
        l = cls->traits;
        if(!l) {
            l = cls->static_traits;
            is_static = 1;
        }
        while(l) {
            trait_t*trait = l->trait;
            U8 access = trait->name->ns->access;
            if(access==ACCESS_PRIVATE)
                goto cont;
            const char*name = trait->name->name;
            char* ns= ACCESS_NAMESPACE?strdup(trait->name->ns->name):"";
            if(registry_findmember(c, ns, name, 0))
                goto cont;
            name = strdup(name);

            memberinfo_t*s = 0;
            if(trait->kind == TRAIT_METHOD) {
                s = (memberinfo_t*)methodinfo_register_onclass(c, access, ns, name);
                s->return_type = resolve_class(filename, "return type", trait->method->return_type);
            } else if(trait->kind == TRAIT_SLOT ||
                      trait->kind == TRAIT_GETTER) {
                s = (memberinfo_t*)varinfo_register_onclass(c, access, ns, name);
                s->type = resolve_class(filename, "type", trait->type_name);
#if 0 // some variables are apparently both a static const and a slot
      // needs split of static/non-static first
            } else if(trait->kind == TRAIT_CONST) {
                varinfo_t*v = (varinfo_t*)varinfo_register_onclass(c, access, ns, name);
                v->type = resolve_class(filename, "type", trait->type_name);
                v->flags |= FLAG_CONST;
                /* leave this alone for now- it blows up the file too much 
                v->value = constant_clone(trait->value);*/
                s = (memberinfo_t*)v;
#endif
            } else {
                goto cont;
            }

            s->flags = is_static?FLAG_STATIC:0;
            s->flags |= FLAG_BUILTIN;
            s->parent = c;

            cont:
            l = l->next;
            if(!l && !is_static) {
                l = cls->static_traits;
                is_static = 1;
            }
        }
    }

#   define IS_PUBLIC_MEMBER(trait) ((trait)->kind != TRAIT_CLASS && (trait)->name->ns->access != ACCESS_PRIVATE)

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
                m->return_type = resolve_class(filename, "return type", trait->method->return_type);
            } else {
                varinfo_t*v = varinfo_register_global(access, package, name);
                v->type = resolve_class(filename, "type", trait->type_name);
                v->value = constant_clone(trait->value);
                v->flags |= trait->kind==TRAIT_CONST?FLAG_CONST:0;
                m = (memberinfo_t*)v;
            }
            m->flags |= FLAG_BUILTIN;
            m->parent = 0;
        }
    }
}

void as3_import_code(void*_abc)
{
    import_code(_abc, "", 0);
    import_code(_abc, "", 1);
}
