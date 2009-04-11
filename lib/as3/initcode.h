#ifndef __initcode_h__
#define __initcode_h__

#include "abc.h"
#include "registry.h"

DECLARE(parsedclass);
DECLARE_LIST(parsedclass);
struct _parsedclass {
    classinfo_t*cls; 
    dict_t parents;
    dict_t usedclasses_deep;
    dict_t usedclasses;
    abc_class_t*abc;
};

void initcode_add_classlist(abc_script_t*init, parsedclass_list_t*classes);
parsedclass_t* parsedclass_new(classinfo_t*c, abc_class_t*abc);
void parsedclass_add_dependency(parsedclass_t*p, classinfo_t*c);

#endif
