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
#include "tokenizer.h"
#include "assets.h"
#include "../os.h"
#include "../xml.h"
#ifdef HAVE_ZZIP
#include "zzip/lib.h"
#endif

static void import_code(void*_abc, char*filename, int pass, asset_bundle_t*a);

void as3_import_abc(char*filename)
{
    TAG*tag = swf_InsertTag(0, ST_RAWABC);
    memfile_t*file = memfile_open(filename);
    tag->data = file->data;
    tag->len = file->len;
    abc_file_t*abc = swf_ReadABC(tag);
    import_code(abc, filename, 0, 0);
    import_code(abc, filename, 1, 0);
    swf_FreeABC(abc);
    memfile_close(file);
    free(tag);
}

typedef struct _deps {
    const char*name;
    struct _deps*next;
} deps_t;

void as3_import_swf2(reader_t*r, char*filename, dict_t*deps)
{
    SWF _swf,*swf=&_swf;
    if(swf_ReadSWF2(r, &_swf)<0)
        return;
    swf_FoldAll(swf);

    TAG*tag = swf->firstTag;

    asset_resolver_t* assets =  swf_ParseAssets(swf);

    asset_bundle_list_t* asset_bundles = list_new();

    dict_t*name2bundle = dict_new();
    /* pass 1 */
    while(tag) {
        if(tag->id == ST_DOABC || tag->id == ST_RAWABC) {
            abc_file_t*abc = swf_ReadABC(tag);
            import_code(abc, filename, 0, 0);
	    NEW(asset_bundle_t, a);
	    a->file = abc;
	    if(abc->name) {
		dict_put(name2bundle, abc->name, a);
	    }
	    list_append(asset_bundles, a);
        }
        tag = tag->next;
    }

    tag = swf->firstTag;
    asset_bundle_list_t*b = asset_bundles;
    /* pass 2 */
    while(tag) {
        if(tag->id == ST_DOABC || tag->id == ST_RAWABC) {
	    asset_bundle_t*a = b->asset_bundle;
	    abc_file_t*abc = a->file;
	    swf_ResolveAssets(assets, abc);
	    if(deps && abc->name) {
		deps_t*d = dict_lookup(deps, abc->name);
		while(d) {
		    if(d->name) {
			asset_bundle_t*other = dict_lookup(name2bundle, d->name);
			list_append(a->dependencies, other);
		    }
		    d = d->next;
		}
	    }
	    registry_add_asset(a);
            import_code(abc, filename, 1, a);
	    b=b->next;
        }
        tag = tag->next;
    }

    dict_destroy(name2bundle);
    list_free(asset_bundles);

    //swf_FreeTags(swf); // FIXME: mem leak
}

void as3_import_swf(char*filename)
{
    reader_t reader;
    reader_init_filereader2(&reader, filename);
    as3_import_swf2(&reader, filename, 0);
    reader.dealloc(&reader);
}

#ifdef HAVE_ZZIP
typedef struct _catalog_state {
    char*xml_filename;
    char in_libraries;
    char*library;
    char*script;
    dict_t*deps;
    deps_t*current_deps;
    dict_t*name2deps;
    dict_t*id2script;
    ZZIP_DIR*dir;
} catalog_state_t;

const char* fa(catalog_state_t*state, xmlattribute_t*attr, const char*name)
{
    while(attr) {
	if(!strcmp(attr->name, name)) return attr->value;
	attr = attr->next;
    }
    syntaxerror("error parsing %s: attribute %s missing", state->xml_filename, name);
}

void catalog_start_tag(xmlconsumer_t*c, char*name, xmlattribute_t*attr)
{
    catalog_state_t*state = (catalog_state_t*)c->internal;
    if(!strcmp(name, "libraries")) {
	state->in_libraries = 1;
    } else if(!strcmp(name, "library")) {
	state->library = strdup(fa(state, attr, "path"));
    } else if(!strcmp(name, "script")) {
	state->script = strdup(fa(state, attr, "name"));
    } else if(!strcmp(name, "def")) {
	dict_put(state->id2script, strdup(fa(state, attr, "id")), strdup(state->script));
    } else if(!strcmp(name, "dep")) {
	NEW(deps_t,d);
	d->name = strdup(fa(state, attr, "id"));
	d->next = state->current_deps;
	state->current_deps = d;
    }
}
void catalog_data(xmlconsumer_t*c, char*data, int len)
{
    catalog_state_t*state = (catalog_state_t*)c->internal;
}
void catalog_end_tag(xmlconsumer_t*c, char*name)
{
    catalog_state_t*state = (catalog_state_t*)c->internal;
    if(!strcmp(name, "libraries")) {
	state->in_libraries = 0;
    } else if(!strcmp(name, "library")) {
	ZZIP_FILE*file = zzip_file_open(state->dir, state->library, 0);

	DICT_ITERATE_DATA(state->deps,deps_t*,deps) {
	    while(deps) {
		char*script = dict_lookup(state->id2script, deps->name);
		if(!script) {
		    //as3_warning("when importing %s: depencency %s referenced in catalog.xml, but not found.", state->xml_filename, deps->name);
		}
		deps->name = script;
		deps = deps->next;
	    }
	}

	if(!file) {
	    as3_warning("when importing %s: %s referenced in catalog.xml, but not found.", state->xml_filename, state->library);
	} else {
	    reader_t r;
	    reader_init_zzipreader(&r, file);
	    as3_import_swf2(&r, state->library, state->deps);
	    r.dealloc(&r);
	    zzip_file_close(file);
	}
	dict_destroy(state->deps);
	state->deps = 0;
	free(state->library);
	state->library = 0;
    } else if(!strcmp(name, "script")) {
	dict_put(state->deps, state->script, state->current_deps);
	free(state->script);
	state->current_deps = 0;
	state->script = 0;
    }
}
void as3_import_zipfile(char*filename)
{
    ZZIP_DIR*dir = zzip_opendir(filename);
    if(!dir) as3_error("Error reading %s\n", filename);
    ZZIP_FILE*file = zzip_file_open(dir, "catalog.xml", 0);
    reader_t r;
    reader_init_zzipreader(&r, file);

    xmlconsumer_t c;
    catalog_state_t state;
    memset(&state, 0, sizeof(state));
    state.dir = dir;
    state.xml_filename = filename;
    state.name2deps = dict_new();
    state.id2script = dict_new();
    state.deps = dict_new();
    c.start_tag = catalog_start_tag;
    c.data= catalog_data;
    c.end_tag = catalog_end_tag;
    c.internal = &state;

    xml_parse(&r, &c);

    r.dealloc(&r);
}
#else
void as3_import_zipfile(char*filename)
{
    as3_warning("No zipfile support compiled in- can't import %s\n", filename);
}
#endif

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
    } else if(!strncmp(head, "PK", 2)) {
	as3_import_zipfile(filename);
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
    if(!n->name[0]) return 0;
    if(!strcmp(n->name, "void")) 
        return &voidclass;

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

static void import_code(void*_abc, char*filename, int pass, asset_bundle_t*asset_bundle)
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

	c->assets = asset_bundle;

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
        dict_t*names = dict_new();
        while(l) {
            trait_t*trait = l->trait;
            U8 access = trait->name->ns->access;

            if(access==ACCESS_PRIVATE)
                goto cont;
            const char*name = trait->name->name;
            char* ns = access==ACCESS_NAMESPACE?strdup(trait->name->ns->name):"";
        
	    if(registry_findmember(c, ns, name, 0, is_static))
                goto cont;

            name = strdup(name);

            memberinfo_t*s = 0;
            if(trait->kind == TRAIT_METHOD) {
                s = (memberinfo_t*)methodinfo_register_onclass(c, access, ns, name, is_static);
                s->return_type = resolve_class(filename, "return type", trait->method->return_type);
                dict_put(names, name, 0);
            } else if(trait->kind == TRAIT_SLOT) {
                s = (memberinfo_t*)varinfo_register_onclass(c, access, ns, name, is_static);
                s->type = resolve_class(filename, "type", trait->type_name);
                dict_put(names, name, 0);
            } else if(trait->kind == TRAIT_GETTER) {
                s = (memberinfo_t*)varinfo_register_onclass(c, access, ns, name, is_static);
                s->type = resolve_class(filename, "type", trait->method->return_type);
                dict_put(names, name, 0);
            } else if(trait->kind == TRAIT_CONST) {
                /* some variables (e.g. XML.length) are apparently both a method and a slot.
                   needs split of static/non-static first */
                if(!dict_contains(names, name)) {
                    varinfo_t*v = (varinfo_t*)varinfo_register_onclass(c, access, ns, name, is_static);
                    v->type = resolve_class(filename, "type", trait->type_name);
                    v->flags |= FLAG_CONST;
                    /* leave this alone for now- it blows up the file too much 
                    v->value = constant_clone(trait->value);*/
                    s = (memberinfo_t*)v;
                    dict_put(names, name, 0);
                } else 
                    goto cont;

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
        dict_destroy(names);
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
    import_code(_abc, "", 0, 0);
    import_code(_abc, "", 1, 0);
}
