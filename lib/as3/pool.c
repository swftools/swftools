/* pool.c

   Routines for handling Flash2 AVM2 ABC contantpool entries.

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

#include "pool.h"

char* access2str(int type)
{
    if(type==0x08) return "";
    else if(type==0x16) return "package";
    else if(type==0x17) return "packageinternal";
    else if(type==0x18) return "protected";
    else if(type==0x19) return "explicit";
    else if(type==0x1A) return "staticprotected";
    else if(type==0x05) return "private";
    else return "undefined";
}

char* multiname_to_string(multiname_t*m)
{
    char*mname = 0;
    if(!m)
        return strdup("--<UNDEFINED_MULTINAME>--");
    if(m->type==0x07 || m->type==0x0d) {
        mname = malloc(strlen(m->ns->name)+strlen(m->name)+32);
        sprintf(mname, "<%s>\0", access2str(m->ns->access));
        strcat(mname, m->ns->name);
        strcat(mname, "::");
        strcat(mname, m->name);
    } else if(m->type==0x0f || m->type==0x10) {
        mname = strdup(m->name);
    } else if(m->type==0x11 || m->type==0x12) {
        mname = strdup("");
    } else if(m->type==0x09 || m->type==0x0e) {
        char*s = namespace_set_to_string(m->namespace_set);
        mname = malloc(strlen(s)+strlen(m->name)+16);
        strcpy(mname, s);
        free(s);
        strcat(mname, "::");
        strcat(mname, m->name);
    } else if(m->type==0x1b || m->type==0x1c) {
        mname = namespace_set_to_string(m->namespace_set);
    }
    return mname;
}

char* namespace_to_string(namespace_t*ns)
{
    char*access = 0;
    U8 type = ns->access;
    if(type==0x08) access = "";
    else if(type==0x16) access = "Package";
    else if(type==0x17) access = "PackageInternal";
    else if(type==0x18) access = "Protected";
    else if(type==0x19) access = "Explicit";
    else if(type==0x1A) access = "StaticProtected";
    else if(type==0x05) access = "Private";
    else {
        fprintf(stderr, "Undefined namespace type %02x\n", type);
        access = "UnknownNS";
    }
    char*string = malloc(strlen(access)+strlen(ns->name)+3);
    sprintf(string, "<%s>%s", access, ns->name);
    return string;
}

char* namespace_set_to_string(namespace_set_t*set)
{
    /* TODO: is the order of the namespaces important (does it
       change the lookup order?). E.g. flex freely shuffles namespaces
       around.
       If the order is not important, we can optimize constant pools by sorting
       the namespaces.
    */
    int l = 0;
    namespace_list_t*lns = set->namespaces;
    while(lns) {
        char*s = namespace_to_string(lns->namespace);
        l += strlen(s)+1;
        free(s);
        lns = lns->next;
    }
    char*desc = malloc(l+16);
    strcpy(desc, "{");
    lns = set->namespaces;
    while(lns) {
        char*s = namespace_to_string(lns->namespace);
        strcat(desc, s);
        free(s);
        lns = lns->next;
        if(lns)
            strcat(desc, ",");
    }
    strcat(desc, "}");
    return desc;
}


int pool_register_string(pool_t*pool, const char*s)
{
    return array_append_if_new(pool->strings, s, 0);
}

int pool_register_namespace(pool_t*pool, namespace_t*ns)
{
    if(!ns)
        return 0;
    char*name = namespace_to_string(ns);
    int i = array_append_if_new(pool->namespaces, name, ns);
    free(name);
    return i;
}
int pool_register_namespace_set(pool_t*pool, namespace_set_t*set)
{
    char*name = namespace_set_to_string(set);
    int i = array_append_if_new(pool->namespace_sets, name, set);
    free(name);
    return i;
}
int pool_register_multiname(pool_t*pool, multiname_t*n)
{
    if(!n)
        return 0;
    char*name = multiname_to_string(n);
    int i =array_append_if_new(pool->multinames, name, n);
    free(name);
    return i;
}
int pool_register_multiname2(pool_t*pool, char*name)
{
    multiname_t*n = multiname_fromstring(name);
    int i =array_append_if_new(pool->multinames, name, n);
    return i;
}
int pool_find_namespace(pool_t*pool, namespace_t*ns)
{
    if(!ns)
        return 0;
    char*name = namespace_to_string(ns);
    int i = array_find(pool->namespaces, name);
    if(i<0) {
        fprintf(stderr, "Couldn't find namespace \"%s\" %08x in constant pool\n", name, ns);
        return 0;
    }
    free(name);
    return i;
}
int pool_find_namespace_set(pool_t*pool, namespace_set_t*set)
{
    char*name = namespace_set_to_string(set);
    int i = array_find(pool->namespace_sets, name);
    free(name);
    if(i<0) {
        fprintf(stderr, "Couldn't find namespace_set \"%s\" in constant pool\n", name);
        return 0;
    }
    return i;
}
int pool_find_string(pool_t*pool, const char*s)
{
    int i = array_find(pool->strings, s);
    if(i<0) {
        fprintf(stderr, "Couldn't find string \"%s\" in constant pool\n", s);
        *(int*)0=0;
        return 0;
    }
    return i;
}
int pool_find_multiname(pool_t*pool, multiname_t*name)
{
    char*n = multiname_to_string(name);
    int i = array_find(pool->multinames, n);
    if(i<0) {
        fprintf(stderr, "Couldn't find multiname \"%s\" in constant pool\n", n);
        return 0;
    }
    return i;
}

multiname_t*pool_lookup_multiname(pool_t*pool, int i)
{
    multiname_t*m = (multiname_t*)array_getvalue(pool->multinames, i);
    return m;
}

multiname_t* multiname_fromstring(const char*name2)
{
    if(!name2)
	name2 = "::";
    char*n = strdup(name2);
    char*p = strstr(n, "::");
    char*namespace=0,*name=0;
    if(!p) {
        if(strchr(n, ':')) {
            fprintf(stderr, "Error: single ':' in name\n");
        }
	namespace = "";
	name = n;
    } else {
	*p = 0;
	namespace = n;
	name = p+2;
        if(strchr(namespace, ':')) {
            fprintf(stderr, "Error: single ':' in namespace\n");
        }
        if(strchr(name, ':')) {
            fprintf(stderr, "Error: single ':' in qualified name\n");
        }
    }

    multiname_t*m = malloc(sizeof(multiname_t));
    memset(m, 0, sizeof(multiname_t));
    m->type = QNAME;
    m->namespace_set = 0;
    NEW(namespace_t,ns);
    ns->name= namespace;
    m->ns = ns;
    m->name = name;
    return m;
}

pool_t*pool_new()
{
    NEW(pool_t, p);

    p->ints = array_new();
    array_append(p->ints, 0, (void*)(ptroff_t)0);
    p->uints = array_new();
    array_append(p->uints, 0, (void*)(ptroff_t)0);
    p->floats = array_new();
    array_append(p->floats, 0, 0);
    p->strings = array_new();
    array_append(p->strings, "--<UNDEFINED_STRING>--", 0);
    p->namespaces = array_new();
    array_append(p->namespaces, "--<UNDEFINED_NAMESPACE>--", 0);
    p->namespace_sets = array_new();
    array_append(p->namespace_sets, "--<UNDEFINED_NSSET>--", 0);
    p->multinames = array_new();
    array_append(p->multinames, "--<UNDEFINED_MULTINAME>--", 0);

    return p;
}

#define DEBUG if(0)

void pool_read(pool_t*pool, TAG*tag)
{
    int num_ints = swf_GetU30(tag);
    DEBUG printf("%d ints\n", num_ints);
    int t;
    for(t=1;t<num_ints;t++) {
        S32 v = swf_GetS30(tag);
        DEBUG printf("int %d) %d\n", t, v);
        array_append(pool->ints, 0, (void*)(ptroff_t)v);
    }

    int num_uints = swf_GetU30(tag);
    DEBUG printf("%d uints\n", num_uints);
    for(t=1;t<num_uints;t++) {
        U32 v = swf_GetU30(tag);
        DEBUG printf("uint %d) %d\n", t, v);
        array_append(pool->uints, 0, (void*)(ptroff_t)v);
    }
    
    int num_floats = swf_GetU30(tag);
    DEBUG printf("%d floats\n", num_floats);
    for(t=1;t<num_floats;t++) {
        double d = swf_GetD64(tag);
        DEBUG printf("float %d) %f\n", t, d);
        array_append(pool->floats, 0, 0);
    }
    
    int num_strings = swf_GetU30(tag);
    DEBUG printf("%d strings\n", num_strings);
    for(t=1;t<num_strings;t++) {
	int len = swf_GetU30(tag);
	char*s = malloc(len+1);
	swf_GetBlock(tag, s, len);
	s[len] = 0;
	array_append(pool->strings, s, 0);
	DEBUG printf("%d) \"%s\"\n", t, pool->strings->d[t].name);
    }
    int num_namespaces = swf_GetU30(tag);
    DEBUG printf("%d namespaces\n", num_namespaces);
    for(t=1;t<num_namespaces;t++) {
	U8 type = swf_GetU8(tag);
	int namenr = swf_GetU30(tag);
	const char*name = array_getkey(pool->strings, namenr);
        NEW(namespace_t, ns);
        memset(ns, 0, sizeof(namespace_t));
        ns->access = type;
        ns->name = strdup(name);
	array_append(pool->namespaces, name, ns);
    }
    int num_sets = swf_GetU30(tag);
    DEBUG printf("%d namespace sets\n", num_sets);
    for(t=1;t<num_sets;t++) {
        int count = swf_GetU30(tag);
        int s;
        
        NEW(namespace_set_t, nsset);
        for(s=0;s<count;s++) {
            int nsnr = swf_GetU30(tag);
            namespace_t*ns = (namespace_t*)array_getvalue(pool->namespaces, nsnr);
            list_append(nsset->namespaces, ns);
        }
        char*desc = namespace_set_to_string(nsset);

        array_append(pool->namespace_sets, desc, nsset);
        DEBUG printf("set %d) %s\n", t, desc);
    }

    int num_multinames = swf_GetU30(tag);
    DEBUG printf("%d multinames\n", num_multinames);
    for(t=1;t<num_multinames;t++) {
        
        NEW(multiname_t,m);
	m->type = swf_GetU8(tag);
	if(m->type==0x07 || m->type==0x0d) {
	    int namespace_index = swf_GetU30(tag);
            m->ns = (namespace_t*)array_getvalue(pool->namespaces, namespace_index);
            int name_index = swf_GetU30(tag);
	    m->name = array_getkey(pool->strings, name_index);
	} else if(m->type==0x0f || m->type==0x10) {
            int name_index = swf_GetU30(tag);
	    m->name = array_getkey(pool->strings, name_index);
	} else if(m->type==0x11 || m->type==0x12) {
	} else if(m->type==0x09 || m->type==0x0e) {
            int name_index = swf_GetU30(tag);
            int namespace_set_index = swf_GetU30(tag);
	    m->name = array_getkey(pool->strings, name_index);
	    m->namespace_set = (namespace_set_t*)array_getvalue(pool->namespace_sets, namespace_set_index);
        } else if(m->type==0x1b || m->type==0x1c) {
            int namespace_set_index = swf_GetU30(tag);
	    m->namespace_set = (namespace_set_t*)array_getvalue(pool->namespace_sets, namespace_set_index);
	} else {
	    printf("can't parse type %d multinames yet\n", m->type);
	}
	char*mname = multiname_to_string(m);
        DEBUG printf("multiname %d) %s\n", t, mname);
	array_append(pool->multinames, mname, m);
	free(mname);
    }
} 

void pool_destroy(pool_t*pool)
{
    int t;
    array_free(pool->ints);
    array_free(pool->uints);
    array_free(pool->floats);

    for(t=1;t<pool->strings->num;t++) {
        free((void*)array_getkey(pool->strings, t));
    }
    array_free(pool->strings);

    for(t=1;t<pool->namespaces->num;t++) {
        free(array_getvalue(pool->namespaces, t));
    }
    array_free(pool->namespaces);
   
    for(t=1;t<pool->namespace_sets->num;t++) {
        namespace_set_t*set = (namespace_set_t*)array_getvalue(pool->namespace_sets, t);
        list_free(set->namespaces);
        free(set);
    }
    array_free(pool->namespace_sets);

    for(t=1;t<pool->multinames->num;t++) {
        free(array_getvalue(pool->multinames, t));
    }
    array_free(pool->multinames);
}
