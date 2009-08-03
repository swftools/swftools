/* initcode.c

   Routines for handling/compiling Flash2 AVM2 ABC Actionscript

   Extension module for the rfxswf library.
   Part of the swftools package.

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

#include <assert.h>
#include "../q.h"
#include "abc.h"
#include "code.h"
#include "common.h"
#include "registry.h"
#include "initcode.h"

int compare_parsedclass(const void *_v1, const void *_v2)
{
    parsedclass_t*p1 = *(parsedclass_t**)_v1;
    parsedclass_t*p2 = *(parsedclass_t**)_v2;
    if((p1->cls->flags^p2->cls->flags)&FLAG_INTERFACE) {
        return (int)(p2->cls->flags&FLAG_INTERFACE) - (int)(p1->cls->flags&FLAG_INTERFACE);
    }
    classinfo_t*c2 = dict_lookup(&p1->parents, p2);
    classinfo_t*c1 = dict_lookup(&p2->parents, p1);
    assert(!c1 || !c2); // otherwise we would have a loop
    assert(!c1 || c1==p1->cls);
    assert(!c2 || c2==p2->cls);

    if(c1) {
        return -1;
    }
    if(c2) {
        return 1;
    }
    
    c2 = dict_lookup(&p1->usedclasses_deep, p2);
    c1 = dict_lookup(&p2->usedclasses_deep, p1);
    assert(!c1 || !c2);
    assert(!c1 || c1==p1->cls);
    assert(!c2 || c2==p2->cls);
    if(c1) {
        return -1;
    }
    if(c2) {
        return 1;
    }

    return 0;
}

static void add_parent(parsedclass_t*p, classinfo_t*c, dict_t*s2p, char soft)
{
    dict_t*parents = soft?(&p->usedclasses_deep):(&p->parents);
    int t;
    if(dict_contains(parents, p)) {
        if(soft) {
            as3_warning("circular reference: class %s references self (through static code)", p->cls->name);
            return;
        } else {
            syntaxerror("circular reference: class %s references self", p->cls->name);
        }
    }

    if(c) {
        parsedclass_t*n = dict_lookup(s2p, c);
        if(n && !dict_contains(parents, n)) {
            assert(n->cls == c);
            dict_put(parents, n, c);
        }
    } else {
        c = p->cls;
    }

    if(soft && dict_contains(s2p, c)) {
        parsedclass_t*pp = dict_lookup(s2p, c);
        DICT_ITERATE_KEY(&pp->usedclasses, classinfo_t*, cc) {
            add_parent(p, cc, s2p, soft);
        }
    }
    if(c->superclass) {
        add_parent(p, c->superclass, s2p, soft);
    }
    for(t=0;c->interfaces[t];t++) {
        add_parent(p, c->interfaces[t], s2p, soft);
    }
}

parsedclass_t* parsedclass_new(classinfo_t*cls, abc_class_t*abc)
{
    NEW(parsedclass_t,p);
    p->cls = cls;
    p->abc = abc;
    dict_init2(&p->parents, &ptr_type, 1);
    dict_init2(&p->usedclasses, &ptr_type, 1);
    dict_init2(&p->usedclasses_deep, &ptr_type, 1);
    return p;
}

/* sort classes so that 
   (a) interfaces appear before classes
   (b) base classes always appear before their subclasses
   (c) classes appear after the classes they use in static code
*/
parsedclass_t** initcode_sort_classlist(parsedclass_list_t*classes)
{
    dict_t* s2p = dict_new2(&ptr_type);

    /* create hash tables */
    int count = 0;
    parsedclass_list_t*l;
    for(l=classes;l;l=l->next) {
        dict_put(s2p, l->parsedclass->cls, l->parsedclass);
        count++;
    }
    for(l=classes;l;l=l->next) {
        add_parent(l->parsedclass, 0, s2p, 0);
        DICT_ITERATE_KEY(&l->parsedclass->usedclasses, classinfo_t*, c) {
            add_parent(l->parsedclass, c, s2p, 1);
        }
    }

    parsedclass_t**list = malloc(sizeof(parsedclass_t*)*count);

    /* build an array for each class */
    int i = 0;
    for(l=classes;l;l=l->next) {
        list[i++] = l->parsedclass;
    }
    
    /* sort and flatten.
       We unfortunately need to do insertion sort O(n^2) as
       our dependencies are only partially ordered */
    int j;
    for(i=0;i<count;i++) {
        for(j=i+1;j<count;j++) {
            int r = compare_parsedclass(list+i,list+j);
            if(r>0) {
                parsedclass_t*p1 = list[i];
                parsedclass_t*p2 = list[j];
                list[i] = p2;
                list[j] = p1;
            }
        }
    }

    parsedclass_t**list2 = malloc(sizeof(parsedclass_t*)*(count+1));
    for(i=0;i<count;i++) {
        list2[i] = (parsedclass_t*)list[i];
#ifdef DEBUG
        parsedclass_t*p = list[i];
        printf("%s\n", p->cls->name);
        if(p->cls->superclass)
            printf("  extends %s\n", p->cls->superclass->name);
        int t;
        for(t=0;p->cls->interfaces[t];t++)
            printf("  interface %s\n", p->cls->interfaces[t]->name);
        DICT_ITERATE_KEY(&p->usedclasses, classinfo_t*, c) {
            printf("  uses %s\n", c->name);
        }
        DICT_ITERATE_KEY(&p->parents, parsedclass_t*, pp) {
            printf("  depends on (deep) %s\n", pp->cls->name);
        }
        DICT_ITERATE_KEY(&p->usedclasses_deep, parsedclass_t*, px) {
            printf("  uses (deep) %s\n", px->cls->name);
        }
        printf("\n");
#endif
    }
    list2[count]=0;
    free(list);

    dict_destroy(s2p);
    return list2;
}

void parsedclass_add_dependency(parsedclass_t*p, classinfo_t*c)
{
    if(!dict_contains(&p->usedclasses, c)) {
        dict_put(&p->usedclasses, c, c);
    }
}

void initcode_add_classlist(abc_script_t*init, parsedclass_list_t*_classes)
{
    code_t*c = 0;

    c = abc_getlocal_0(c);
    c = abc_pushscope(c);
  
    parsedclass_t**classes = initcode_sort_classlist(_classes);

    int t;
    for(t=0;classes[t];t++) {
        abc_class_t*abc = classes[t]->abc;
        classinfo_t*cls = classes[t]->cls;
        
        array_append(init->file->classes, "", abc);

        /* write the construction code for this class to the global init
           function */
        MULTINAME(classname2,cls);
        trait_t*trait = abc_initscript_addClassTrait(init, &classname2, abc);

        c = abc_getglobalscope(c);
        classinfo_t*s = cls->superclass;

        int count=0;

        while(s) {
            //TODO: take a look at the current scope stack, maybe 
            //      we can re-use something
            s = s->superclass;
            if(!s) 
            break;
           
            multiname_t*s2 = sig2mname(s);
            c = abc_getlex2(c, s2);
            multiname_destroy(s2);

            c = abc_pushscope(c); count++;
            c = c->prev->prev; // invert
        }
        /* continue appending after last op end */
        while(c && c->next) c = c->next; 

        multiname_t*extends2 = sig2mname(cls->superclass);
        /* TODO: if this is one of *our* classes, we can also 
                 do a getglobalscope/getslot <nr> (which references
                 the init function's slots) */
        if(extends2) {
            c = abc_getlex2(c, extends2);
            c = abc_dup(c);
            /* notice: we get a Verify Error #1107 if the top elemnt on the scope
               stack is not the superclass */
            c = abc_pushscope(c);count++;
        } else {
            c = abc_pushnull(c);
            /* notice: we get a verify error #1107 if the top element on the scope 
               stack is not the global object */
            c = abc_getlocal_0(c);
            c = abc_pushscope(c);count++;
        }
        c = abc_newclass(c,abc);
        while(count--) {
            c = abc_popscope(c);
        }
        c = abc_setslot(c, trait->slot_id);
        multiname_destroy(extends2);
    }
    c = abc_returnvoid(c);

    free(classes);

    init->method->body->code = c;
}
