#ifndef __initcode_h__
#define __initcode_h__

#include "abc.h"
#include "registry.h"

DECLARE(parsedclass);
DECLARE_LIST(parsedclass);
struct _parsedclass {
    classinfo_t*cls; 
    abc_class_t*abc;
};

void initcode_add_classlist(abc_script_t*init, parsedclass_list_t*classes);

#endif
