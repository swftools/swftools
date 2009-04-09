#include "abc.h"
#include "code.h"
#include "registry.h"
#include "initcode.h"

int compare_lists(const void *_v1, const void *_v2)
{
    classinfo_t**v1 = *(classinfo_t***)_v1;
    classinfo_t**v2 = *(classinfo_t***)_v2;
    parsedclass_t*p1 = **(parsedclass_t***)_v1;
    parsedclass_t*p2 = **(parsedclass_t***)_v2;
    if((p1->cls->flags^p2->cls->flags)&FLAG_INTERFACE) {
        return (int)(p2->cls->flags&FLAG_INTERFACE) - (int)(p1->cls->flags&FLAG_INTERFACE);
    }
    do {
        v1++;v2++;
        if(*v1<*v2) return -1;
        else if(*v1>*v2) return 1;
    } while(*v1 && *v2);
    return 0;
}

/* sort classes so that 
   (a) interfaces appear before classes
   (b) base classes always appear before their subclasses
   (c) neighboring elements have similar scope stacks 

   Notice: we don't yet handle multiple inheritance (in interfaces) correctly.
*/
parsedclass_t** initcode_sort_classlist(parsedclass_list_t*classes)
{
    /* count classes */
    int count = 0;
    parsedclass_list_t*l;
    for(l=classes;l;l=l->next) {
        count++;
    }
    void***list = malloc(sizeof(void**)*count);

    /* build an array for each class */
    int i = 0;
    for(l=classes;l;l=l->next) {
        classinfo_t*cls = l->parsedclass->cls;
        int len=0;
        classinfo_t*c=cls;
        while(c) {
            len ++;
            c = c->flags&FLAG_INTERFACE?c->interfaces[0]:c->superclass;
        }
        void**a = (void**)malloc(sizeof(void*)*(len+2));
        a[0] = l->parsedclass;
        a[len+1] = 0;
        int t=len;
        c = cls;
        while(c) {
            len ++;
            a[t--] = c;
            c = c->flags&FLAG_INTERFACE?c->interfaces[0]:c->superclass;
        }
        list[i++] = a;
    }
    
    /* sort and flatten */
    qsort(list, count, sizeof(void**), compare_lists);

    parsedclass_t**list2 = malloc(sizeof(parsedclass_t*)*(count+1));
    for(i=0;i<count;i++) {
        list2[i] = (parsedclass_t*)list[i][0];
        free(list[i]);
    }
    list2[count]=0;
    free(list);
    return list2;
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
        int slotindex = abc_initscript_addClassTrait(init, &classname2, abc);

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
        c = abc_setslot(c, slotindex);
        multiname_destroy(extends2);
    }
    c = abc_returnvoid(c);

    free(classes);

    init->method->body->code = c;
}

