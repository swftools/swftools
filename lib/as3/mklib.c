/* code.c

   File to generate builtin.c

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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include "../rfxswf.h"
#include "../os.h"
#include "pool.h"
#include "files.h"
#include "tokenizer.h"
#include "parser.tab.h"
#include "parser.h"

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

char* mkpid(const char*package)
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
char* mkid(const char*package, const char*name)
{
    char*id = malloc(strlen(package)+strlen(name)+10);
    strcpy(id, package);
    strcat(id, "_");
    strcat(id, name);
    fixstring(id);
    return id;
}

char*mkid2(multiname_t*m)
{
    if(m->type == QNAME)
        return mkid(m->ns->name, m->name);
    else if(m->type == MULTINAME) {
        namespace_list_t*l = m->namespace_set->namespaces;
        while(l) {
            if(l->namespace->name &&
               l->namespace->name[0])
                break;
            l = l->next;
        }
        return mkid(l->namespace->name, m->name);
    }
    else {
        fprintf(stderr, "can't convert multiname type %d\n", m->type);
    }
}

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

char* kind2string(int kind)
{
}

void write_member_info(FILE*fi, char*parent, char*id2, const char*name, int flags, trait_t*trait)
{
    char*retvalue = 0;
    char*type="0";
    switch(trait->kind) {
        case TRAIT_METHOD: {
            multiname_t*ret = trait->method->return_type;

            if(!ret)
                retvalue = 0;
            else
                retvalue = mkid(ret->ns->name, ret->name);
            if(ret && !strcmp(ret->name, "void"))
                retvalue = 0;
        } //fallthrough
        case TRAIT_FUNCTION:
            type = "MEMBER_METHOD";
            break;
        case TRAIT_CONST:
        case TRAIT_GETTER:
        case TRAIT_SETTER:
        case TRAIT_SLOT:
            type = "MEMBER_SLOT";
            break;
        default:
            fprintf(stderr, "Unknown trait type %d\n", trait->kind);
    }
    fprintf(fi, "static memberinfo_t %s = {%s, 0x%02x, \"%s\"", id2, type, flags, name);
    if(!retvalue)
        fprintf(fi, ", 0");
    else
        fprintf(fi, ", &%s", retvalue);
  
    if(!parent) 
        fprintf(fi, ", 0");
    else
        fprintf(fi, ", &%s", parent); // parent

    fprintf(fi, ", 0,0,0};\n");
}

int access2flags(multiname_t*m)
{
    int access = m->ns->access;
    int flags=0;
    if(access == ACCESS_PACKAGE) flags|=FLAG_PUBLIC;
    if(access == ACCESS_PRIVATE) flags|=FLAG_PRIVATE;
    if(access == ACCESS_PROTECTED) flags|=FLAG_PROTECTED;
    if(access == ACCESS_PACKAGEINTERNAL) flags|=FLAG_PACKAGEINTERNAL;
    if(access == ACCESS_NAMESPACE) {
        if(!strcmp(m->ns->name, "http://adobe.com/AS3/2006/builtin")) {
            flags|=FLAG_NAMESPACE_ADOBE;
        }
    }
    return flags;
}

void load_libraries(char*filename, int pass, FILE*fi)
{
    SWF swf;
    memset(&swf, 0, sizeof(SWF));
    TAG*tag = swf_InsertTag(0, ST_RAWABC);
    memfile_t*file = memfile_open(filename);
    tag->data = file->data;
    tag->len = file->len;
    abc_file_t*abc = swf_ReadABC(tag);
    //swf_DumpABC(stdout, abc, "");

    if(pass<=2) {
        int*index = malloc(abc->classes->num*sizeof(int));
        int t;
        tosort=abc->classes;
        for(t=0;t<abc->classes->num;t++) {index[t]=t;}
        qsort(index, abc->classes->num, sizeof(int), compare_classes);

        if(pass==-1) {
            const char*last_package = "-*-";
            for(t=0;t<abc->classes->num;t++) {
                abc_class_t*cls = array_getvalue(abc->classes, index[t]);
                const char*package = cls->classname->ns->name;
                char*pid = mkpid(package);
                if(strcmp(last_package, package)) {
                    last_package = package;
                    fprintf(fi, "static packageinfo_t %s = {\"%s\"};\n", pid, package);
                }
            }
        }

        if(pass>=0)
        for(t=0;t<abc->classes->num;t++) {
            abc_class_t*cls = array_getvalue(abc->classes, index[t]);
            int access = cls->classname->ns->access;
            if(access==ACCESS_PRIVATE ||
               access==ACCESS_PACKAGEINTERNAL)
                continue;
            if(!strncmp(cls->classname->ns->name, "__AS3", 5))
                continue;

            const char*package = cls->classname->ns->name;
            const char*name = cls->classname->name;
            const char*superpackage = 0;
            const char*supername = 0;
            char*superid = 0;
            if(cls->superclass) {
                superpackage = cls->superclass->ns->name;
                supername = cls->superclass->name;
                superid = mkid(superpackage, supername);
            }
            char*id = mkid(package, name);
            U8 flags = cls->flags;
            
            if(pass==0)  {
                fprintf(fi, "static classinfo_t %s;\n", id);
            } else if(pass==1) {
                fprintf(fi, "static classinfo_t %s = {0x%02x, 0x%02x, \"%s\", \"%s\"", id, access, flags, package, name);
                fprintf(fi, ", (void*)0"); //slot
                if(superid)
                    fprintf(fi, ", &%s, interfaces:{", superid);
                else
                    fprintf(fi, ", (void*)0, interfaces:{");
                if(cls->interfaces) {
                    multiname_list_t*i=cls->interfaces;
                    while(i) {
                        char*iid = mkid2(i->multiname);
                        fprintf(fi, "&%s, ", iid);
                        i = i->next;
                    }
                }
                fprintf(fi, "(void*)0}};\n");
            } else if(pass==2) {
                trait_list_t*l=cls->traits;
                fprintf(fi, "    dict_put(d, &%s, &%s);\n", id, id);
                fprintf(fi, "    dict_init(&%s.members, %d);\n", id, list_length(l)+1);
            }

          
            trait_list_t*l=0;
            char is_static = 0;
            dict_t*d = dict_new();
            l = cls->traits;
            while(l) {
                trait_t*trait = l->trait;
                if(trait->name->ns->access==ACCESS_PRIVATE)
                    goto cont;
                const char*name = trait->name->name;
                char id2[1024];
                sprintf(id2, "%s_%s", id, name);
                
                if(dict_lookup(d, name)) {
                    goto cont;
                } else {
                    dict_put(d, (char*)name, (char*)name);
                }
                int flags = is_static?FLAG_STATIC:0;
                //flags |= access2flags(access);
                flags |= access2flags(trait->name);

                if(pass==0) {
                    fprintf(fi, "static memberinfo_t %s;\n", id2);
                } if(pass==1) {
                    write_member_info(fi, id, id2, name, flags, trait);
                } else if(pass==2) {
                    fprintf(fi, "    dict_put(&%s.members, \"%s\", &%s);\n", id, name, id2);
                }
    cont:
                l = l->next;
                if(!l && !is_static) {
                    l = cls->static_traits;
                    is_static = 1;
                }
            }
            
            dict_destroy(d);

            if(id) free(id);
            if(superid) free(superid);
        }
        free(index);
    }

    if(pass==0 || pass==1 || pass==3) {
        int t;

#define IS_PUBLIC_MEMBER(trait) ((trait)->kind != TRAIT_CLASS)
  
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
        const char*last_package = "-xxx-";
        for(t=0;t<num_methods;t++) {
            trait_t*trait = traits[t];
            if(IS_PUBLIC_MEMBER(trait)) {
                const char*package = trait->name->ns->name;
                const char*name = trait->name->name;
                char*pid = mkpid(package);
                char*id2 = mkid2(trait->name);
                int flags = FLAG_STATIC|access2flags(trait->name);
                NEW(memberinfo_t,m);
                char np = 0;
                int clsflags = FLAG_STATIC | FLAG_METHOD;
                if(pass==0) {
                    fprintf(fi, "static memberinfo_t %s;\n", id2);
                    fprintf(fi, "static classinfo_t %s_class;\n", id2);
                } else if(pass==1) {
                    write_member_info(fi, 0, id2, name, flags, trait);
                    fprintf(fi, "static classinfo_t %s_class = {0x%02x, 0x%02x, \"%s\", \"%s\", &%s, (void*)0, members:{(void*)0}};\n", 
                            id2,
                            trait->name->ns->access, clsflags,
                            package, name, 
                            id2);
                } else if(pass==3) {
                    /*if(np) {
                        fprintf(fi, "    dict_init(%s.classes, 1); //not used yet\n", pid);
                        fprintf(fi, "    dict_init(%s.functions, 1);\n", pid);
                    }
                    fprintf(fi, "    dict_put(&%s.functions, \"%s\", &%s);\n", pid, name, id2);*/
                    fprintf(fi, "    dict_put(d, &%s_class, &%s_class);\n", id2, id2);
                }
            } else if(trait->kind == TRAIT_CLASS) {
                // ignore classes, these are treated above
            } else {
                printf("%02x %s\n", trait->kind, multiname_tostring(trait->name));
            }
        }
    }

    swf_FreeABC(abc);
    memfile_close(file);tag->data=0;
    swf_DeleteTag(0, tag);
}

int main()
{
    FILE*fi = fopen("builtin.c", "wb");
    fprintf(fi, "#include \"builtin.h\"\n");

    //load_libraries("builtin.abc", -1, fi);
    //load_libraries("playerglobal.abc", -1, fi);
    //fprintf(fi, "static packageinfo_t package_flash_debugger = {\"flash.debugger\"};\n");
    //fprintf(fi, "static packageinfo_t package_flash_profiler = {\"flash.profiler\"};\n");

    load_libraries("builtin.abc", 0, fi);
    load_libraries("playerglobal.abc", 0, fi);
    
    load_libraries("builtin.abc", 1, fi);
    load_libraries("playerglobal.abc", 1, fi);
   
    fprintf(fi, "dict_t* builtin_getclasses()\n");
    fprintf(fi, "{\n");
    fprintf(fi, "    dict_t*d = dict_new2(&classinfo_type);\n");
    load_libraries("builtin.abc", 2, fi);
    load_libraries("playerglobal.abc", 2, fi);
    load_libraries("builtin.abc", 3, fi);
    load_libraries("playerglobal.abc", 3, fi);
    fprintf(fi, "    return d;\n");
    fprintf(fi, "}\n");
    fprintf(fi, "\n");
    
    //fprintf(fi, "dict_t* builtin_getglobalfunctions()\n");
    //fprintf(fi, "{\n");
    //fprintf(fi, "    dict_t*d = dict_new();\n");
    //load_libraries("builtin.abc", 3, fi);
    //load_libraries("playerglobal.abc", 3, fi);
    //fprintf(fi, "    return d;\n");
    //fprintf(fi, "}\n");

    fclose(fi);
}
