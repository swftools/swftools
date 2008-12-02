#ifndef __abc_registry_h__
#define __abc_registry_h__
multiname_t* registry_findclass(const char*package, const char*name);
multiname_t* registry_getanytype();
multiname_t* registry_getobjectclass();
multiname_t* registry_getnumberclass();
multiname_t* registry_getstringclass();
multiname_t* registry_getintclass();
multiname_t* registry_getuintclass();
multiname_t* registry_getbooleanclass();
multiname_t* registry_getsuperclass(multiname_t*m);
#endif
