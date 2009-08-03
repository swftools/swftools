/* mklib.c

   File to generate builtin.c

   Copyright (c) 2008,2009 Matthias Kramm <kramm@quiss.org>
 
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
#include <unistd.h>
#include <memory.h>
#include "../rfxswf.h"
#include "../os.h"
#include "../q.h"
#include "pool.h"
#include "files.h"
#include "tokenizer.h"
#include "parser.tab.h"
#include "parser.h"
#include "import.h"

void fixstring(char*s)
{
    char first=1;
    for(;*s;s++) {
        if(!((*s>='a' && *s<='z') || (*s>='A' && *s<='Z') || 
                    (*s>='0' && *s<='9' && !first))) {
            *s = '_';
        }
        first = 0;
    }
}

static char* mkpid(const char*package)
{
    char*id = malloc(strlen(package)+20);
    strcpy(id, "package_");
    if(!*package) 
        strcat(id, "global");
    else
        strcat(id, package);
    fixstring(id);
    return id;
}
static char* mkcid(const char*package, const char*name)
{
    char*id = malloc(strlen(package)+strlen(name)+10);
    strcpy(id, package);
    strcat(id, "_");
    strcat(id, name);
    fixstring(id);
    return id;
}
static char* mkptr2(const char*package, const char*name)
{
    char*id = malloc(strlen(package)+strlen(name)+10);
    strcpy(id, "&");
    strcat(id, package);
    strcat(id, "_");
    strcat(id, name);
    fixstring(id+1);
    return id;
}
static char* mkid2(const char*cls, const char*member)
{
    char*id = malloc(strlen(cls)+strlen(member)+10);
    strcpy(id, cls);
    strcat(id, "_");
    strcat(id, member);
    fixstring(id);
    return id;
}
#define mkid(s) ((s)?mkcid((s)->package, (s)->name):"0")
#define mkptr(s) ((s)?mkptr2((s)->package, (s)->name):"0")

static array_t*tosort=0;
static int compare_classes(const void*v1, const void*v2)
{
    int x1 = *(int*)v1;
    int x2 = *(int*)v2;
    abc_class_t*c1 = array_getvalue(tosort, x1);
    abc_class_t*c2 = array_getvalue(tosort, x2);
    int i = strcmp(c1->classname->ns->name, c2->classname->ns->name);
    if(i)
        return i;
    return strcmp(c1->classname->name, c2->classname->name);
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

dict_t* builtin_getclasses()
{
    return dict_new2(&slotinfo_type);
}

extern dict_t*registry_classes;

char*mktype(slotinfo_t*s) 
{
    if(s->kind == INFOTYPE_CLASS) {
        return "classinfo_t";
    } else if(s->kind == INFOTYPE_METHOD) {
        return "methodinfo_t";
    } else if(s->kind == INFOTYPE_VAR) {
        return "varinfo_t";
    }
    return "**ERROR**";
}

void write_slotinfo(FILE*fi, slotinfo_t*s, char*id, char*prefix);

void write_slotinfo_decl(FILE*fi, slotinfo_t*s, char*prefix)
{
    fprintf(fi, "%s", prefix);
    char*id = mkid(s);
    fprintf(fi, "static %s %s;\n", mktype(s), id);

    if(s->kind == INFOTYPE_CLASS) {
        classinfo_t*c = (classinfo_t*)s;
        dict_t*d = &c->members;
        int t;
        for(t=0;t<d->hashsize;t++) {
            dictentry_t*l = d->slots[t];
            while(l) {
                slotinfo_t*s2 = (slotinfo_t*)l->data;
                fprintf(fi, "static %s %s;\n", mktype(s2), mkid2(id, s2->name));
                l = l->next;
            }
        }
    }
}
void write_initinfo(FILE*fi, slotinfo_t*s, char*prefix)
{
    if(s->kind == INFOTYPE_CLASS) {
        classinfo_t*c = (classinfo_t*)s;
        fprintf(fi, "%s", prefix);
        char*id = mkid(c);
        dict_t*d = &c->members;
        fprintf(fi, "dict_init2(&%s.members, &memberinfo_type, %d);\n", id, d->hashsize);
        int t;
        for(t=0;t<d->hashsize;t++) {
            dictentry_t*l = d->slots[t];
            while(l) {
                slotinfo_t*s2 = (slotinfo_t*)l->data;
                fprintf(fi, "%s", prefix);
                char*id2 = mkid2(id, s2->name);
                fprintf(fi, "dict_put(&%s.members, &%s, &%s);\n", id, id2,id2);
                l = l->next;
            }
        }
    }
}

void write_constant(FILE*fi, constant_t*value, char*id, char*prefix)
{
    if(NS_TYPE(value->type)) {
        fprintf(fi, "%s", prefix);
        fprintf(fi, "static namespace_t %s_constant_ns = {0x%02x, \"%s\"};\n", id, value->ns->access, value->ns->name);
    } else if(value->type == CONSTANT_STRING) {
        fprintf(fi, "%s", prefix);
        fprintf(fi, "static string_t %s_constant_s = {\"%s\", %d};\n", id, value->s->str, value->s->len);
    }
    fprintf(fi, "%s", prefix);
    fprintf(fi, "static constant_t %s_constant = ", id);
    fprintf(fi, "{type: %d", value->type);
    if(NS_TYPE(value->type)) {
        fprintf(fi, ", &%s_constant_ns", id);
    } else if(value->type == CONSTANT_INT) {
        fprintf(fi, ",i: %d,", value->type);
    } else if(value->type == CONSTANT_UINT) {
        fprintf(fi, ",u: %u", value->u);
    } else if(value->type == CONSTANT_FLOAT) {
        if(!isnan(value->f) && !isinf(value->f))
            fprintf(fi, ", %f", value->f);
    } else if(value->type == CONSTANT_STRING) {
        fprintf(fi, ", &%s_constant_s", id);
    }
    fprintf(fi, "};\n");
}

void write_slotinfo(FILE*fi, slotinfo_t*s, char*id, char*prefix)
{
    if(s->kind == INFOTYPE_VAR) {
        varinfo_t*v = (varinfo_t*)s;
        if(v->value) {
            write_constant(fi, v->value, id, prefix);
        }
    }
    fprintf(fi, "%s", prefix);
    fprintf(fi, "static %s %s = {", mktype(s), id);
    fprintf(fi, "0x%02x, 0x%02x, 0x%02x, 0x%02x, ", s->kind, s->subtype, s->flags, s->access);
    if(s->package)
        fprintf(fi, "\"%s\", ", s->package);
    else
        fprintf(fi, "0, ");
    
    if(s->name)
        fprintf(fi, "\"%s\", ", s->name);
    else
        fprintf(fi, "0, ");

    fprintf(fi, "%d, ", s->slot);

    if(s->kind == INFOTYPE_CLASS) {
        classinfo_t*c = (classinfo_t*)s;
        fprintf(fi, "%s, ", mkptr(c->superclass));
        fprintf(fi, "interfaces: {");
        int t;
        for(t=0;c->interfaces[t];t++) {
            fprintf(fi, "%s", mkptr(c->interfaces[t]));
            fprintf(fi, ", ");
        }
        fprintf(fi, "0}};\n");
    }
    if(s->kind == INFOTYPE_METHOD) {
        methodinfo_t*m = (methodinfo_t*)s;
        fprintf(fi, "%s, ", mkptr(m->return_type));
        fprintf(fi, "%s, ", mkptr(m->parent));
        fprintf(fi, "0"); // params TODO
        fprintf(fi, "};\n");
    }
    if(s->kind == INFOTYPE_VAR) {
        varinfo_t*m = (varinfo_t*)s;
        fprintf(fi, "%s, ", mkptr(m->type));
        fprintf(fi, "%s, ", mkptr(m->parent));
        if(!m->value) fprintf(fi, "0");
        else          fprintf(fi, "&%s_constant", id);
        fprintf(fi, "};\n");
    }
    
    if(s->kind == INFOTYPE_CLASS) {
        classinfo_t*c = (classinfo_t*)s;
        dict_t*d = &c->members;
        int t;
        for(t=0;t<d->hashsize;t++) {
            dictentry_t*l = d->slots[t];
            while(l) {
                slotinfo_t*s2 = (slotinfo_t*)l->data;
                write_slotinfo(fi, s2, mkid2(id,s2->name), prefix);
                l = l->next;
            }
        }
    }
}

int main()
{
    registry_classes = builtin_getclasses();

    as3_import_abc("/home/kramm/c/swftools/lib/as3/builtin.abc");
    as3_import_abc("/home/kramm/c/swftools/lib/as3/playerglobal.abc");

    FILE*fi = fopen("builtin.c", "wb");

    int t;
    int pass;


    for(pass=1;pass<=3;pass++) {
        if(pass==1) {
            fprintf(fi, "#include \"builtin.h\"\n");
            fprintf(fi, "\n");
        }
        if(pass==3) {
            fprintf(fi, "dict_t* builtin_getclasses()\n");
            fprintf(fi, "{\n");
            fprintf(fi, "    dict_t*d = dict_new2(&slotinfo_type);\n");
        }
        for(t=0;t<registry_classes->hashsize;t++) {
            dictentry_t*l = registry_classes->slots[t];
            while(l) {
                slotinfo_t*s = (slotinfo_t*)l->key;
                //printf("%08x %s %s\n", s, s->package, s->name);
                char*id = mkid(s);
                if(pass==1) {
                    write_slotinfo_decl(fi, s, "");
                }
                if(pass==2) {
                    write_slotinfo(fi, s, mkid(s), "");
                }
                if(pass==3) {
                    fprintf(fi, "    dict_put(d, &%s, &%s);\n", id, id);
                    write_initinfo(fi, s, "    ");
                }
                l = l->next;
            }
        }
    }
    fprintf(fi, "    _NaN_constant.f =  __builtin_nan(\"\");\n");
    fprintf(fi, "    _Infinity_constant.f = __builtin_inf();\n");
    fprintf(fi, "    return d;\n");
    fprintf(fi, "}\n");
    return 0;
}
