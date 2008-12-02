#include "pool.h"
#include "registry.h"

static namespace_t static_empty_ns = {
    ACCESS_PACKAGE, ""
};
static multiname_t static_object_class = {
    QNAME, &static_empty_ns, 0, "Object"
};

multiname_t* registry_getobjectclass()
{
    return &static_object_class;
}
multiname_t* registry_findclass(const char*s)
{
    multiname_t*m = multiname_fromstring(s);
    return m;
}
multiname_t* registry_getsuperclass(multiname_t*m)
{
    if(m->name && !strcmp(m->name, "Object"))
            return 0;
    return &static_object_class;
}

