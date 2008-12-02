#include "pool.h"
#include "registry.h"

static namespace_t static_empty_ns = {
    ACCESS_PACKAGE, ""
};
static multiname_t static_object_class = {
    QNAME, &static_empty_ns, 0, "Object"
};
static multiname_t static_anytype_class = {
    QNAME, &static_empty_ns, 0, 0
};
static multiname_t static_string_class = {
    QNAME, &static_empty_ns, 0, "String"
};
static multiname_t static_boolean_class = {
    QNAME, &static_empty_ns, 0, "Boolean"
};
static multiname_t static_number_class = {
    QNAME, &static_empty_ns, 0, "Number"
};
static multiname_t static_int_class = {
    QNAME, &static_empty_ns, 0, "int"
};
static multiname_t static_uint_class = {
    QNAME, &static_empty_ns, 0, "uint"
};

multiname_t* registry_getobjectclass() {return &static_object_class;}
multiname_t* registry_getanytype() {return &static_anytype_class;}
multiname_t* registry_getstringclass() {return &static_string_class;}
multiname_t* registry_getintclass() {return &static_int_class;}
multiname_t* registry_getuintclass() {return &static_uint_class;}
multiname_t* registry_getbooleanclass() {return &static_boolean_class;}
multiname_t* registry_getnumberclass() {return &static_number_class;}

multiname_t* registry_findclass(const char*package, const char*name)
{
    multiname_t*m=0;
    if(!package) {
        m = multiname_new(0, name);
    } else {
        namespace_t*ns = namespace_new_packageinternal(package);
        m = multiname_new(ns,name);
        namespace_destroy(ns);
    }
    return m;
}
multiname_t* registry_getsuperclass(multiname_t*m)
{
    if(m->name && !strcmp(m->name, "Object"))
            return 0;
    return &static_object_class;
}

