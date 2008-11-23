#ifndef __opcodes_h__
#define __opcodes_h__
#include "abc.h"
#include "pool.h"
abc_code_t* abc_add(abc_code_t*prev);
#define add(method) {method->code = abc_add(method->code);}
abc_code_t* abc_add_i(abc_code_t*prev);
#define add_i(method) {method->code = abc_add_i(method->code);}
abc_code_t* abc_astype(abc_code_t*prev, char* name);
#define astype(method,name) {method->code = abc_astype(method->code,name);}
abc_code_t* abc_astypelate(abc_code_t*prev);
#define astypelate(method) {method->code = abc_astypelate(method->code);}
abc_code_t* abc_bitand(abc_code_t*prev);
#define bitand(method) {method->code = abc_bitand(method->code);}
abc_code_t* abc_bitnot(abc_code_t*prev);
#define bitnot(method) {method->code = abc_bitnot(method->code);}
abc_code_t* abc_bitor(abc_code_t*prev);
#define bitor(method) {method->code = abc_bitor(method->code);}
abc_code_t* abc_bitxor(abc_code_t*prev);
#define bitxor(method) {method->code = abc_bitxor(method->code);}
abc_code_t* abc_call(abc_code_t*prev, int v);
#define call(method,v) {method->code = abc_call(method->code,v);}
abc_code_t* abc_callmethod(abc_code_t*prev, abc_method_body_t* m, int v);
#define callmethod(method,m,v) {method->code = abc_callmethod(method->code,m,v);}
abc_code_t* abc_callproplex(abc_code_t*prev, char* name, int v);
#define callproplex(method,name,v) {method->code = abc_callproplex(method->code,name,v);}
abc_code_t* abc_callproperty(abc_code_t*prev, char* name, int v);
#define callproperty(method,name,v) {method->code = abc_callproperty(method->code,name,v);}
abc_code_t* abc_callpropvoid(abc_code_t*prev, char* name, int v);
#define callpropvoid(method,name,v) {method->code = abc_callpropvoid(method->code,name,v);}
abc_code_t* abc_callstatic(abc_code_t*prev, abc_method_t* m, int v);
#define callstatic(method,m,v) {method->code = abc_callstatic(method->code,m,v);}
abc_code_t* abc_callsuper(abc_code_t*prev, char* name, int v);
#define callsuper(method,name,v) {method->code = abc_callsuper(method->code,name,v);}
abc_code_t* abc_callsupervoid(abc_code_t*prev, char* name, int v);
#define callsupervoid(method,name,v) {method->code = abc_callsupervoid(method->code,name,v);}
abc_code_t* abc_checkfilter(abc_code_t*prev);
#define checkfilter(method) {method->code = abc_checkfilter(method->code);}
abc_code_t* abc_coerce(abc_code_t*prev, char* name);
#define coerce(method,name) {method->code = abc_coerce(method->code,name);}
abc_code_t* abc_coerce_a(abc_code_t*prev);
#define coerce_a(method) {method->code = abc_coerce_a(method->code);}
abc_code_t* abc_coerce_s(abc_code_t*prev);
#define coerce_s(method) {method->code = abc_coerce_s(method->code);}
abc_code_t* abc_construct(abc_code_t*prev, int v);
#define construct(method,v) {method->code = abc_construct(method->code,v);}
abc_code_t* abc_constructprop(abc_code_t*prev, char* name, int v);
#define constructprop(method,name,v) {method->code = abc_constructprop(method->code,name,v);}
abc_code_t* abc_constructsuper(abc_code_t*prev, int v);
#define constructsuper(method,v) {method->code = abc_constructsuper(method->code,v);}
abc_code_t* abc_convert_b(abc_code_t*prev);
#define convert_b(method) {method->code = abc_convert_b(method->code);}
abc_code_t* abc_convert_i(abc_code_t*prev);
#define convert_i(method) {method->code = abc_convert_i(method->code);}
abc_code_t* abc_convert_d(abc_code_t*prev);
#define convert_d(method) {method->code = abc_convert_d(method->code);}
abc_code_t* abc_convert_o(abc_code_t*prev);
#define convert_o(method) {method->code = abc_convert_o(method->code);}
abc_code_t* abc_convert_u(abc_code_t*prev);
#define convert_u(method) {method->code = abc_convert_u(method->code);}
abc_code_t* abc_convert_s(abc_code_t*prev);
#define convert_s(method) {method->code = abc_convert_s(method->code);}
abc_code_t* abc_debug(abc_code_t*prev, void* debuginfo);
#define debug(method,debuginfo) {method->code = abc_debug(method->code,debuginfo);}
abc_code_t* abc_debugfile(abc_code_t*prev, char* s);
#define debugfile(method,s) {method->code = abc_debugfile(method->code,s);}
abc_code_t* abc_debugline(abc_code_t*prev, int v);
#define debugline(method,v) {method->code = abc_debugline(method->code,v);}
abc_code_t* abc_declocal(abc_code_t*prev, int reg);
#define declocal(method,reg) {method->code = abc_declocal(method->code,reg);}
abc_code_t* abc_declocal_i(abc_code_t*prev, int reg);
#define declocal_i(method,reg) {method->code = abc_declocal_i(method->code,reg);}
abc_code_t* abc_decrement(abc_code_t*prev);
#define decrement(method) {method->code = abc_decrement(method->code);}
abc_code_t* abc_decrement_i(abc_code_t*prev);
#define decrement_i(method) {method->code = abc_decrement_i(method->code);}
abc_code_t* abc_deleteproperty(abc_code_t*prev, char* name);
#define deleteproperty(method,name) {method->code = abc_deleteproperty(method->code,name);}
abc_code_t* abc_divide(abc_code_t*prev);
#define divide(method) {method->code = abc_divide(method->code);}
abc_code_t* abc_dup(abc_code_t*prev);
#define dup(method) {method->code = abc_dup(method->code);}
abc_code_t* abc_dxns(abc_code_t*prev, char* s);
#define dxns(method,s) {method->code = abc_dxns(method->code,s);}
abc_code_t* abc_dxnslate(abc_code_t*prev);
#define dxnslate(method) {method->code = abc_dxnslate(method->code);}
abc_code_t* abc_equals(abc_code_t*prev);
#define equals(method) {method->code = abc_equals(method->code);}
abc_code_t* abc_esc_xattr(abc_code_t*prev);
#define esc_xattr(method) {method->code = abc_esc_xattr(method->code);}
abc_code_t* abc_esc_xelem(abc_code_t*prev);
#define esc_xelem(method) {method->code = abc_esc_xelem(method->code);}
abc_code_t* abc_findproperty(abc_code_t*prev, char* name);
#define findproperty(method,name) {method->code = abc_findproperty(method->code,name);}
abc_code_t* abc_findpropstrict(abc_code_t*prev, char* name);
#define findpropstrict(method,name) {method->code = abc_findpropstrict(method->code,name);}
abc_code_t* abc_getdescendants(abc_code_t*prev, char* name);
#define getdescendants(method,name) {method->code = abc_getdescendants(method->code,name);}
abc_code_t* abc_getglobalscope(abc_code_t*prev);
#define getglobalscope(method) {method->code = abc_getglobalscope(method->code);}
abc_code_t* abc_getglobalslot(abc_code_t*prev, int v);
#define getglobalslot(method,v) {method->code = abc_getglobalslot(method->code,v);}
abc_code_t* abc_getlex(abc_code_t*prev, char* name);
#define getlex(method,name) {method->code = abc_getlex(method->code,name);}
abc_code_t* abc_getlocal(abc_code_t*prev, int reg);
#define getlocal(method,reg) {method->code = abc_getlocal(method->code,reg);}
abc_code_t* abc_getlocal_0(abc_code_t*prev);
#define getlocal_0(method) {method->code = abc_getlocal_0(method->code);}
abc_code_t* abc_getlocal_1(abc_code_t*prev);
#define getlocal_1(method) {method->code = abc_getlocal_1(method->code);}
abc_code_t* abc_getlocal_2(abc_code_t*prev);
#define getlocal_2(method) {method->code = abc_getlocal_2(method->code);}
abc_code_t* abc_getlocal_3(abc_code_t*prev);
#define getlocal_3(method) {method->code = abc_getlocal_3(method->code);}
abc_code_t* abc_getproperty(abc_code_t*prev, char* name);
#define getproperty(method,name) {method->code = abc_getproperty(method->code,name);}
abc_code_t* abc_getscopeobject(abc_code_t*prev, int v);
#define getscopeobject(method,v) {method->code = abc_getscopeobject(method->code,v);}
abc_code_t* abc_getslot(abc_code_t*prev, int v);
#define getslot(method,v) {method->code = abc_getslot(method->code,v);}
abc_code_t* abc_getsuper(abc_code_t*prev, char* name);
#define getsuper(method,name) {method->code = abc_getsuper(method->code,name);}
abc_code_t* abc_greaterequals(abc_code_t*prev);
#define greaterequals(method) {method->code = abc_greaterequals(method->code);}
abc_code_t* abc_hasnext(abc_code_t*prev);
#define hasnext(method) {method->code = abc_hasnext(method->code);}
abc_code_t* abc_hasnext2(abc_code_t*prev, int reg, int reg2);
#define hasnext2(method,reg,reg2) {method->code = abc_hasnext2(method->code,reg,reg2);}
abc_code_t* abc_ifeq(abc_code_t*prev, abc_code_t* label);
#define ifeq(method,label) {method->code = abc_ifeq(method->code,label);}
abc_code_t* abc_iffalse(abc_code_t*prev, abc_code_t* label);
#define iffalse(method,label) {method->code = abc_iffalse(method->code,label);}
abc_code_t* abc_ifge(abc_code_t*prev, abc_code_t* label);
#define ifge(method,label) {method->code = abc_ifge(method->code,label);}
abc_code_t* abc_ifgt(abc_code_t*prev, abc_code_t* label);
#define ifgt(method,label) {method->code = abc_ifgt(method->code,label);}
abc_code_t* abc_ifle(abc_code_t*prev, abc_code_t* label);
#define ifle(method,label) {method->code = abc_ifle(method->code,label);}
abc_code_t* abc_iflt(abc_code_t*prev, abc_code_t* label);
#define iflt(method,label) {method->code = abc_iflt(method->code,label);}
abc_code_t* abc_ifnge(abc_code_t*prev, abc_code_t* label);
#define ifnge(method,label) {method->code = abc_ifnge(method->code,label);}
abc_code_t* abc_ifngt(abc_code_t*prev, abc_code_t* label);
#define ifngt(method,label) {method->code = abc_ifngt(method->code,label);}
abc_code_t* abc_ifnle(abc_code_t*prev, abc_code_t* label);
#define ifnle(method,label) {method->code = abc_ifnle(method->code,label);}
abc_code_t* abc_ifnlt(abc_code_t*prev, abc_code_t* label);
#define ifnlt(method,label) {method->code = abc_ifnlt(method->code,label);}
abc_code_t* abc_ifne(abc_code_t*prev, abc_code_t* label);
#define ifne(method,label) {method->code = abc_ifne(method->code,label);}
abc_code_t* abc_ifstricteq(abc_code_t*prev, abc_code_t* label);
#define ifstricteq(method,label) {method->code = abc_ifstricteq(method->code,label);}
abc_code_t* abc_ifstrictne(abc_code_t*prev, abc_code_t* label);
#define ifstrictne(method,label) {method->code = abc_ifstrictne(method->code,label);}
abc_code_t* abc_iftrue(abc_code_t*prev, abc_code_t* label);
#define iftrue(method,label) {method->code = abc_iftrue(method->code,label);}
abc_code_t* abc_in(abc_code_t*prev);
#define in(method) {method->code = abc_in(method->code);}
abc_code_t* abc_inclocal(abc_code_t*prev, int reg);
#define inclocal(method,reg) {method->code = abc_inclocal(method->code,reg);}
abc_code_t* abc_inclocal_i(abc_code_t*prev, int reg);
#define inclocal_i(method,reg) {method->code = abc_inclocal_i(method->code,reg);}
abc_code_t* abc_increment(abc_code_t*prev);
#define increment(method) {method->code = abc_increment(method->code);}
abc_code_t* abc_increment_i(abc_code_t*prev);
#define increment_i(method) {method->code = abc_increment_i(method->code);}
abc_code_t* abc_initproperty(abc_code_t*prev, char* name);
#define initproperty(method,name) {method->code = abc_initproperty(method->code,name);}
abc_code_t* abc_instanceof(abc_code_t*prev);
#define instanceof(method) {method->code = abc_instanceof(method->code);}
abc_code_t* abc_istype(abc_code_t*prev, char* name);
#define istype(method,name) {method->code = abc_istype(method->code,name);}
abc_code_t* abc_istypelate(abc_code_t*prev);
#define istypelate(method) {method->code = abc_istypelate(method->code);}
abc_code_t* abc_jump(abc_code_t*prev, abc_code_t* label);
#define jump(method,label) {method->code = abc_jump(method->code,label);}
abc_code_t* abc_kill(abc_code_t*prev, int reg);
#define kill(method,reg) {method->code = abc_kill(method->code,reg);}
abc_code_t* abc_label(abc_code_t*prev);
#define label(method) {method->code = abc_label(method->code);}
abc_code_t* abc_lessequals(abc_code_t*prev);
#define lessequals(method) {method->code = abc_lessequals(method->code);}
abc_code_t* abc_lessthan(abc_code_t*prev);
#define lessthan(method) {method->code = abc_lessthan(method->code);}
abc_code_t* abc_lookupswitch(abc_code_t*prev, void* labels);
#define lookupswitch(method,labels) {method->code = abc_lookupswitch(method->code,labels);}
abc_code_t* abc_lshift(abc_code_t*prev);
#define lshift(method) {method->code = abc_lshift(method->code);}
abc_code_t* abc_modulo(abc_code_t*prev);
#define modulo(method) {method->code = abc_modulo(method->code);}
abc_code_t* abc_multiply(abc_code_t*prev);
#define multiply(method) {method->code = abc_multiply(method->code);}
abc_code_t* abc_multiply_i(abc_code_t*prev);
#define multiply_i(method) {method->code = abc_multiply_i(method->code);}
abc_code_t* abc_negate(abc_code_t*prev);
#define negate(method) {method->code = abc_negate(method->code);}
abc_code_t* abc_negate_i(abc_code_t*prev);
#define negate_i(method) {method->code = abc_negate_i(method->code);}
abc_code_t* abc_newactivation(abc_code_t*prev);
#define newactivation(method) {method->code = abc_newactivation(method->code);}
abc_code_t* abc_newarray(abc_code_t*prev, int v);
#define newarray(method,v) {method->code = abc_newarray(method->code,v);}
abc_code_t* abc_newcatch(abc_code_t*prev, int v);
#define newcatch(method,v) {method->code = abc_newcatch(method->code,v);}
abc_code_t* abc_newclass(abc_code_t*prev, abc_class_t* m);
#define newclass(method,m) {method->code = abc_newclass(method->code,m);}
abc_code_t* abc_newfunction(abc_code_t*prev, abc_method_body_t* m);
#define newfunction(method,m) {method->code = abc_newfunction(method->code,m);}
abc_code_t* abc_newobject(abc_code_t*prev, int v);
#define newobject(method,v) {method->code = abc_newobject(method->code,v);}
abc_code_t* abc_nextname(abc_code_t*prev);
#define nextname(method) {method->code = abc_nextname(method->code);}
abc_code_t* abc_nextvalue(abc_code_t*prev);
#define nextvalue(method) {method->code = abc_nextvalue(method->code);}
abc_code_t* abc_nop(abc_code_t*prev);
#define nop(method) {method->code = abc_nop(method->code);}
abc_code_t* abc_not(abc_code_t*prev);
#define not(method) {method->code = abc_not(method->code);}
abc_code_t* abc_pop(abc_code_t*prev);
#define pop(method) {method->code = abc_pop(method->code);}
abc_code_t* abc_popscope(abc_code_t*prev);
#define popscope(method) {method->code = abc_popscope(method->code);}
abc_code_t* abc_pushbyte(abc_code_t*prev, int v);
#define pushbyte(method,v) {method->code = abc_pushbyte(method->code,v);}
abc_code_t* abc_pushdouble(abc_code_t*prev, int v);
#define pushdouble(method,v) {method->code = abc_pushdouble(method->code,v);}
abc_code_t* abc_pushfalse(abc_code_t*prev);
#define pushfalse(method) {method->code = abc_pushfalse(method->code);}
abc_code_t* abc_pushint(abc_code_t*prev, int v);
#define pushint(method,v) {method->code = abc_pushint(method->code,v);}
abc_code_t* abc_pushnamespace(abc_code_t*prev, int v);
#define pushnamespace(method,v) {method->code = abc_pushnamespace(method->code,v);}
abc_code_t* abc_pushnan(abc_code_t*prev);
#define pushnan(method) {method->code = abc_pushnan(method->code);}
abc_code_t* abc_pushnull(abc_code_t*prev);
#define pushnull(method) {method->code = abc_pushnull(method->code);}
abc_code_t* abc_pushscope(abc_code_t*prev);
#define pushscope(method) {method->code = abc_pushscope(method->code);}
abc_code_t* abc_pushshort(abc_code_t*prev, int v);
#define pushshort(method,v) {method->code = abc_pushshort(method->code,v);}
abc_code_t* abc_pushstring(abc_code_t*prev, char* s);
#define pushstring(method,s) {method->code = abc_pushstring(method->code,s);}
abc_code_t* abc_pushtrue(abc_code_t*prev);
#define pushtrue(method) {method->code = abc_pushtrue(method->code);}
abc_code_t* abc_pushuint(abc_code_t*prev, int v);
#define pushuint(method,v) {method->code = abc_pushuint(method->code,v);}
abc_code_t* abc_pushundefined(abc_code_t*prev);
#define pushundefined(method) {method->code = abc_pushundefined(method->code);}
abc_code_t* abc_pushwith(abc_code_t*prev);
#define pushwith(method) {method->code = abc_pushwith(method->code);}
abc_code_t* abc_returnvalue(abc_code_t*prev);
#define returnvalue(method) {method->code = abc_returnvalue(method->code);}
abc_code_t* abc_returnvoid(abc_code_t*prev);
#define returnvoid(method) {method->code = abc_returnvoid(method->code);}
abc_code_t* abc_rshift(abc_code_t*prev);
#define rshift(method) {method->code = abc_rshift(method->code);}
abc_code_t* abc_setlocal(abc_code_t*prev, int reg);
#define setlocal(method,reg) {method->code = abc_setlocal(method->code,reg);}
abc_code_t* abc_setlocal_0(abc_code_t*prev);
#define setlocal_0(method) {method->code = abc_setlocal_0(method->code);}
abc_code_t* abc_setlocal_1(abc_code_t*prev);
#define setlocal_1(method) {method->code = abc_setlocal_1(method->code);}
abc_code_t* abc_setlocal_2(abc_code_t*prev);
#define setlocal_2(method) {method->code = abc_setlocal_2(method->code);}
abc_code_t* abc_setlocal_3(abc_code_t*prev);
#define setlocal_3(method) {method->code = abc_setlocal_3(method->code);}
abc_code_t* abc_setglobalslot(abc_code_t*prev, int v);
#define setglobalslot(method,v) {method->code = abc_setglobalslot(method->code,v);}
abc_code_t* abc_setproperty(abc_code_t*prev, char* name);
#define setproperty(method,name) {method->code = abc_setproperty(method->code,name);}
abc_code_t* abc_setslot(abc_code_t*prev, char* name);
#define setslot(method,name) {method->code = abc_setslot(method->code,name);}
abc_code_t* abc_setsuper(abc_code_t*prev, char* name);
#define setsuper(method,name) {method->code = abc_setsuper(method->code,name);}
abc_code_t* abc_strictequals(abc_code_t*prev);
#define strictequals(method) {method->code = abc_strictequals(method->code);}
abc_code_t* abc_subtract(abc_code_t*prev);
#define subtract(method) {method->code = abc_subtract(method->code);}
abc_code_t* abc_subtract_i(abc_code_t*prev);
#define subtract_i(method) {method->code = abc_subtract_i(method->code);}
abc_code_t* abc_swap(abc_code_t*prev);
#define swap(method) {method->code = abc_swap(method->code);}
abc_code_t* abc_throw(abc_code_t*prev);
#define throw(method) {method->code = abc_throw(method->code);}
abc_code_t* abc_typeof(abc_code_t*prev);
#define typeof(method) {method->code = abc_typeof(method->code);}
abc_code_t* abc_urshift(abc_code_t*prev);
#define urshift(method) {method->code = abc_urshift(method->code);}
abc_code_t* abc_xxx(abc_code_t*prev);
#define xxx(method) {method->code = abc_xxx(method->code);}
#endif
