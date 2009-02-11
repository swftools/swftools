#include "builtin.h"
#include "../q.h"

dict_t* builtin_getclasses()
{
    return dict_new2(&slotinfo_type);
}

