#include "abc.h"
#include "code.h"
#include "registry.h"
#include "initcode.h"

void initcode_add_classlist(abc_script_t*init, parsedclass_list_t*classes)
{
    code_t*c = 0;

    c = abc_getlocal_0(c);
    c = abc_pushscope(c);

    for(;classes;classes=classes->next) {
        abc_class_t*abc = classes->parsedclass->abc;
        classinfo_t*cls = classes->parsedclass->cls;

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

    init->method->body->code = c;
}

