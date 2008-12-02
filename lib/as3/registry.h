#ifndef __abc_registry_h__
#define __abc_registry_h__
multiname_t* registry_findclass(const char*s);
multiname_t* registry_getobjectclass();
multiname_t* registry_getsuperclass(multiname_t*m);
#endif
