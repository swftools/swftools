#ifndef __opcodes_h__
#define __opcodes_h__
#include "abc.h"
#include "pool.h"
#include "code.h"
code_t* abc_add(code_t*prev);
#define add(method) (method->code = abc_add(method->code))
code_t* abc_add_i(code_t*prev);
#define add_i(method) (method->code = abc_add_i(method->code))
code_t* abc_astype(code_t*prev, char* name);
#define astype(method,name) (method->code = abc_astype(method->code,name))
code_t* abc_astype2(code_t*prev, multiname_t* name);
#define astype2(method,name) (method->code = abc_astype2(method->code,name))
code_t* abc_astypelate(code_t*prev);
#define astypelate(method) (method->code = abc_astypelate(method->code))
code_t* abc_bitand(code_t*prev);
#define bitand(method) (method->code = abc_bitand(method->code))
code_t* abc_bitnot(code_t*prev);
#define bitnot(method) (method->code = abc_bitnot(method->code))
code_t* abc_bitor(code_t*prev);
#define bitor(method) (method->code = abc_bitor(method->code))
code_t* abc_bitxor(code_t*prev);
#define bitxor(method) (method->code = abc_bitxor(method->code))
code_t* abc_call(code_t*prev, int v);
#define call(method,v) (method->code = abc_call(method->code,v))
code_t* abc_callmethod(code_t*prev, abc_method_body_t* m, int v);
#define callmethod(method,m,v) (method->code = abc_callmethod(method->code,m,v))
code_t* abc_callproplex(code_t*prev, char* name, int v);
#define callproplex(method,name,v) (method->code = abc_callproplex(method->code,name,v))
code_t* abc_callproplex2(code_t*prev, multiname_t* name, int v);
#define callproplex2(method,name,v) (method->code = abc_callproplex2(method->code,name,v))
code_t* abc_callproperty(code_t*prev, char* name, int v);
#define callproperty(method,name,v) (method->code = abc_callproperty(method->code,name,v))
code_t* abc_callproperty2(code_t*prev, multiname_t* name, int v);
#define callproperty2(method,name,v) (method->code = abc_callproperty2(method->code,name,v))
code_t* abc_callpropvoid(code_t*prev, char* name, int v);
#define callpropvoid(method,name,v) (method->code = abc_callpropvoid(method->code,name,v))
code_t* abc_callpropvoid2(code_t*prev, multiname_t* name, int v);
#define callpropvoid2(method,name,v) (method->code = abc_callpropvoid2(method->code,name,v))
code_t* abc_callstatic(code_t*prev, abc_method_t* m, int v);
#define callstatic(method,m,v) (method->code = abc_callstatic(method->code,m,v))
code_t* abc_callsuper(code_t*prev, char* name, int v);
#define callsuper(method,name,v) (method->code = abc_callsuper(method->code,name,v))
code_t* abc_callsuper2(code_t*prev, multiname_t* name, int v);
#define callsuper2(method,name,v) (method->code = abc_callsuper2(method->code,name,v))
code_t* abc_callsupervoid(code_t*prev, char* name, int v);
#define callsupervoid(method,name,v) (method->code = abc_callsupervoid(method->code,name,v))
code_t* abc_callsupervoid2(code_t*prev, multiname_t* name, int v);
#define callsupervoid2(method,name,v) (method->code = abc_callsupervoid2(method->code,name,v))
code_t* abc_checkfilter(code_t*prev);
#define checkfilter(method) (method->code = abc_checkfilter(method->code))
code_t* abc_coerce(code_t*prev, char* name);
#define coerce(method,name) (method->code = abc_coerce(method->code,name))
code_t* abc_coerce2(code_t*prev, multiname_t* name);
#define coerce2(method,name) (method->code = abc_coerce2(method->code,name))
code_t* abc_coerce_a(code_t*prev);
#define coerce_a(method) (method->code = abc_coerce_a(method->code))
code_t* abc_coerce_s(code_t*prev);
#define coerce_s(method) (method->code = abc_coerce_s(method->code))
code_t* abc_construct(code_t*prev, int v);
#define construct(method,v) (method->code = abc_construct(method->code,v))
code_t* abc_constructprop(code_t*prev, char* name, int v);
#define constructprop(method,name,v) (method->code = abc_constructprop(method->code,name,v))
code_t* abc_constructprop2(code_t*prev, multiname_t* name, int v);
#define constructprop2(method,name,v) (method->code = abc_constructprop2(method->code,name,v))
code_t* abc_constructsuper(code_t*prev, int v);
#define constructsuper(method,v) (method->code = abc_constructsuper(method->code,v))
code_t* abc_convert_b(code_t*prev);
#define convert_b(method) (method->code = abc_convert_b(method->code))
code_t* abc_convert_i(code_t*prev);
#define convert_i(method) (method->code = abc_convert_i(method->code))
code_t* abc_convert_d(code_t*prev);
#define convert_d(method) (method->code = abc_convert_d(method->code))
code_t* abc_convert_o(code_t*prev);
#define convert_o(method) (method->code = abc_convert_o(method->code))
code_t* abc_convert_u(code_t*prev);
#define convert_u(method) (method->code = abc_convert_u(method->code))
code_t* abc_convert_s(code_t*prev);
#define convert_s(method) (method->code = abc_convert_s(method->code))
code_t* abc_debug(code_t*prev, void* debuginfo);
#define debug(method,debuginfo) (method->code = abc_debug(method->code,debuginfo))
code_t* abc_debugfile(code_t*prev, char* s);
#define debugfile(method,s) (method->code = abc_debugfile(method->code,s))
code_t* abc_debugline(code_t*prev, int v);
#define debugline(method,v) (method->code = abc_debugline(method->code,v))
code_t* abc_declocal(code_t*prev, int reg);
#define declocal(method,reg) (method->code = abc_declocal(method->code,reg))
code_t* abc_declocal_i(code_t*prev, int reg);
#define declocal_i(method,reg) (method->code = abc_declocal_i(method->code,reg))
code_t* abc_decrement(code_t*prev);
#define decrement(method) (method->code = abc_decrement(method->code))
code_t* abc_decrement_i(code_t*prev);
#define decrement_i(method) (method->code = abc_decrement_i(method->code))
code_t* abc_deleteproperty(code_t*prev, char* name);
#define deleteproperty(method,name) (method->code = abc_deleteproperty(method->code,name))
code_t* abc_deleteproperty2(code_t*prev, multiname_t* name);
#define deleteproperty2(method,name) (method->code = abc_deleteproperty2(method->code,name))
code_t* abc_divide(code_t*prev);
#define divide(method) (method->code = abc_divide(method->code))
code_t* abc_dup(code_t*prev);
#define dup(method) (method->code = abc_dup(method->code))
code_t* abc_dxns(code_t*prev, char* s);
#define dxns(method,s) (method->code = abc_dxns(method->code,s))
code_t* abc_dxnslate(code_t*prev);
#define dxnslate(method) (method->code = abc_dxnslate(method->code))
code_t* abc_equals(code_t*prev);
#define equals(method) (method->code = abc_equals(method->code))
code_t* abc_esc_xattr(code_t*prev);
#define esc_xattr(method) (method->code = abc_esc_xattr(method->code))
code_t* abc_esc_xelem(code_t*prev);
#define esc_xelem(method) (method->code = abc_esc_xelem(method->code))
code_t* abc_findproperty(code_t*prev, char* name);
#define findproperty(method,name) (method->code = abc_findproperty(method->code,name))
code_t* abc_findproperty2(code_t*prev, multiname_t* name);
#define findproperty2(method,name) (method->code = abc_findproperty2(method->code,name))
code_t* abc_findpropstrict(code_t*prev, char* name);
#define findpropstrict(method,name) (method->code = abc_findpropstrict(method->code,name))
code_t* abc_findpropstrict2(code_t*prev, multiname_t* name);
#define findpropstrict2(method,name) (method->code = abc_findpropstrict2(method->code,name))
code_t* abc_getdescendants(code_t*prev, char* name);
#define getdescendants(method,name) (method->code = abc_getdescendants(method->code,name))
code_t* abc_getdescendants2(code_t*prev, multiname_t* name);
#define getdescendants2(method,name) (method->code = abc_getdescendants2(method->code,name))
code_t* abc_getglobalscope(code_t*prev);
#define getglobalscope(method) (method->code = abc_getglobalscope(method->code))
code_t* abc_getglobalslot(code_t*prev, int v);
#define getglobalslot(method,v) (method->code = abc_getglobalslot(method->code,v))
code_t* abc_getlex(code_t*prev, char* name);
#define getlex(method,name) (method->code = abc_getlex(method->code,name))
code_t* abc_getlex2(code_t*prev, multiname_t* name);
#define getlex2(method,name) (method->code = abc_getlex2(method->code,name))
code_t* abc_getlocal(code_t*prev, int reg);
#define getlocal(method,reg) (method->code = abc_getlocal(method->code,reg))
code_t* abc_getlocal_0(code_t*prev);
#define getlocal_0(method) (method->code = abc_getlocal_0(method->code))
code_t* abc_getlocal_1(code_t*prev);
#define getlocal_1(method) (method->code = abc_getlocal_1(method->code))
code_t* abc_getlocal_2(code_t*prev);
#define getlocal_2(method) (method->code = abc_getlocal_2(method->code))
code_t* abc_getlocal_3(code_t*prev);
#define getlocal_3(method) (method->code = abc_getlocal_3(method->code))
code_t* abc_getproperty(code_t*prev, char* name);
#define getproperty(method,name) (method->code = abc_getproperty(method->code,name))
code_t* abc_getproperty2(code_t*prev, multiname_t* name);
#define getproperty2(method,name) (method->code = abc_getproperty2(method->code,name))
code_t* abc_getscopeobject(code_t*prev, int v);
#define getscopeobject(method,v) (method->code = abc_getscopeobject(method->code,v))
code_t* abc_getslot(code_t*prev, int v);
#define getslot(method,v) (method->code = abc_getslot(method->code,v))
code_t* abc_getsuper(code_t*prev, char* name);
#define getsuper(method,name) (method->code = abc_getsuper(method->code,name))
code_t* abc_getsuper2(code_t*prev, multiname_t* name);
#define getsuper2(method,name) (method->code = abc_getsuper2(method->code,name))
code_t* abc_greaterthan(code_t*prev);
#define greaterthan(method) (method->code = abc_greaterthan(method->code))
code_t* abc_greaterequals(code_t*prev);
#define greaterequals(method) (method->code = abc_greaterequals(method->code))
code_t* abc_hasnext(code_t*prev);
#define hasnext(method) (method->code = abc_hasnext(method->code))
code_t* abc_hasnext2(code_t*prev, int reg, int reg2);
#define hasnext2(method,reg,reg2) (method->code = abc_hasnext2(method->code,reg,reg2))
code_t* abc_ifeq(code_t*prev, code_t* label);
#define ifeq(method,label) (method->code = abc_ifeq(method->code,label))
code_t* abc_iffalse(code_t*prev, code_t* label);
#define iffalse(method,label) (method->code = abc_iffalse(method->code,label))
code_t* abc_ifge(code_t*prev, code_t* label);
#define ifge(method,label) (method->code = abc_ifge(method->code,label))
code_t* abc_ifgt(code_t*prev, code_t* label);
#define ifgt(method,label) (method->code = abc_ifgt(method->code,label))
code_t* abc_ifle(code_t*prev, code_t* label);
#define ifle(method,label) (method->code = abc_ifle(method->code,label))
code_t* abc_iflt(code_t*prev, code_t* label);
#define iflt(method,label) (method->code = abc_iflt(method->code,label))
code_t* abc_ifnge(code_t*prev, code_t* label);
#define ifnge(method,label) (method->code = abc_ifnge(method->code,label))
code_t* abc_ifngt(code_t*prev, code_t* label);
#define ifngt(method,label) (method->code = abc_ifngt(method->code,label))
code_t* abc_ifnle(code_t*prev, code_t* label);
#define ifnle(method,label) (method->code = abc_ifnle(method->code,label))
code_t* abc_ifnlt(code_t*prev, code_t* label);
#define ifnlt(method,label) (method->code = abc_ifnlt(method->code,label))
code_t* abc_ifne(code_t*prev, code_t* label);
#define ifne(method,label) (method->code = abc_ifne(method->code,label))
code_t* abc_ifstricteq(code_t*prev, code_t* label);
#define ifstricteq(method,label) (method->code = abc_ifstricteq(method->code,label))
code_t* abc_ifstrictne(code_t*prev, code_t* label);
#define ifstrictne(method,label) (method->code = abc_ifstrictne(method->code,label))
code_t* abc_iftrue(code_t*prev, code_t* label);
#define iftrue(method,label) (method->code = abc_iftrue(method->code,label))
code_t* abc_in(code_t*prev);
#define in(method) (method->code = abc_in(method->code))
code_t* abc_inclocal(code_t*prev, int reg);
#define inclocal(method,reg) (method->code = abc_inclocal(method->code,reg))
code_t* abc_inclocal_i(code_t*prev, int reg);
#define inclocal_i(method,reg) (method->code = abc_inclocal_i(method->code,reg))
code_t* abc_increment(code_t*prev);
#define increment(method) (method->code = abc_increment(method->code))
code_t* abc_increment_i(code_t*prev);
#define increment_i(method) (method->code = abc_increment_i(method->code))
code_t* abc_initproperty(code_t*prev, char* name);
#define initproperty(method,name) (method->code = abc_initproperty(method->code,name))
code_t* abc_initproperty2(code_t*prev, multiname_t* name);
#define initproperty2(method,name) (method->code = abc_initproperty2(method->code,name))
code_t* abc_instanceof(code_t*prev);
#define instanceof(method) (method->code = abc_instanceof(method->code))
code_t* abc_istype(code_t*prev, char* name);
#define istype(method,name) (method->code = abc_istype(method->code,name))
code_t* abc_istype2(code_t*prev, multiname_t* name);
#define istype2(method,name) (method->code = abc_istype2(method->code,name))
code_t* abc_istypelate(code_t*prev);
#define istypelate(method) (method->code = abc_istypelate(method->code))
code_t* abc_jump(code_t*prev, code_t* label);
#define jump(method,label) (method->code = abc_jump(method->code,label))
code_t* abc_kill(code_t*prev, int reg);
#define kill(method,reg) (method->code = abc_kill(method->code,reg))
code_t* abc_label(code_t*prev);
#define label(method) (method->code = abc_label(method->code))
code_t* abc_lessequals(code_t*prev);
#define lessequals(method) (method->code = abc_lessequals(method->code))
code_t* abc_lessthan(code_t*prev);
#define lessthan(method) (method->code = abc_lessthan(method->code))
code_t* abc_lookupswitch(code_t*prev, lookupswitch_t* l);
#define lookupswitch(method,l) (method->code = abc_lookupswitch(method->code,l))
code_t* abc_lshift(code_t*prev);
#define lshift(method) (method->code = abc_lshift(method->code))
code_t* abc_modulo(code_t*prev);
#define modulo(method) (method->code = abc_modulo(method->code))
code_t* abc_multiply(code_t*prev);
#define multiply(method) (method->code = abc_multiply(method->code))
code_t* abc_multiply_i(code_t*prev);
#define multiply_i(method) (method->code = abc_multiply_i(method->code))
code_t* abc_negate(code_t*prev);
#define negate(method) (method->code = abc_negate(method->code))
code_t* abc_negate_i(code_t*prev);
#define negate_i(method) (method->code = abc_negate_i(method->code))
code_t* abc_newactivation(code_t*prev);
#define newactivation(method) (method->code = abc_newactivation(method->code))
code_t* abc_newarray(code_t*prev, int v);
#define newarray(method,v) (method->code = abc_newarray(method->code,v))
code_t* abc_newcatch(code_t*prev, int v);
#define newcatch(method,v) (method->code = abc_newcatch(method->code,v))
code_t* abc_newclass(code_t*prev, abc_class_t* m);
#define newclass(method,m) (method->code = abc_newclass(method->code,m))
code_t* abc_newfunction(code_t*prev, abc_method_body_t* m);
#define newfunction(method,m) (method->code = abc_newfunction(method->code,m))
code_t* abc_newobject(code_t*prev, int v);
#define newobject(method,v) (method->code = abc_newobject(method->code,v))
code_t* abc_nextname(code_t*prev);
#define nextname(method) (method->code = abc_nextname(method->code))
code_t* abc_nextvalue(code_t*prev);
#define nextvalue(method) (method->code = abc_nextvalue(method->code))
code_t* abc_nop(code_t*prev);
#define nop(method) (method->code = abc_nop(method->code))
code_t* abc_not(code_t*prev);
#define not(method) (method->code = abc_not(method->code))
code_t* abc_pop(code_t*prev);
#define pop(method) (method->code = abc_pop(method->code))
code_t* abc_popscope(code_t*prev);
#define popscope(method) (method->code = abc_popscope(method->code))
code_t* abc_pushbyte(code_t*prev, int v);
#define pushbyte(method,v) (method->code = abc_pushbyte(method->code,v))
code_t* abc_pushdouble(code_t*prev, double f);
#define pushdouble(method,f) (method->code = abc_pushdouble(method->code,f))
code_t* abc_pushfalse(code_t*prev);
#define pushfalse(method) (method->code = abc_pushfalse(method->code))
code_t* abc_pushint(code_t*prev, int i);
#define pushint(method,i) (method->code = abc_pushint(method->code,i))
code_t* abc_pushnamespace(code_t*prev, int v);
#define pushnamespace(method,v) (method->code = abc_pushnamespace(method->code,v))
code_t* abc_pushnan(code_t*prev);
#define pushnan(method) (method->code = abc_pushnan(method->code))
code_t* abc_pushnull(code_t*prev);
#define pushnull(method) (method->code = abc_pushnull(method->code))
code_t* abc_pushscope(code_t*prev);
#define pushscope(method) (method->code = abc_pushscope(method->code))
code_t* abc_pushshort(code_t*prev, int v);
#define pushshort(method,v) (method->code = abc_pushshort(method->code,v))
code_t* abc_pushstring(code_t*prev, char* s);
#define pushstring(method,s) (method->code = abc_pushstring(method->code,s))
code_t* abc_pushtrue(code_t*prev);
#define pushtrue(method) (method->code = abc_pushtrue(method->code))
code_t* abc_pushuint(code_t*prev, unsigned int u);
#define pushuint(method,u) (method->code = abc_pushuint(method->code,u))
code_t* abc_pushundefined(code_t*prev);
#define pushundefined(method) (method->code = abc_pushundefined(method->code))
code_t* abc_pushwith(code_t*prev);
#define pushwith(method) (method->code = abc_pushwith(method->code))
code_t* abc_returnvalue(code_t*prev);
#define returnvalue(method) (method->code = abc_returnvalue(method->code))
code_t* abc_returnvoid(code_t*prev);
#define returnvoid(method) (method->code = abc_returnvoid(method->code))
code_t* abc_rshift(code_t*prev);
#define rshift(method) (method->code = abc_rshift(method->code))
code_t* abc_setlocal(code_t*prev, int reg);
#define setlocal(method,reg) (method->code = abc_setlocal(method->code,reg))
code_t* abc_setlocal_0(code_t*prev);
#define setlocal_0(method) (method->code = abc_setlocal_0(method->code))
code_t* abc_setlocal_1(code_t*prev);
#define setlocal_1(method) (method->code = abc_setlocal_1(method->code))
code_t* abc_setlocal_2(code_t*prev);
#define setlocal_2(method) (method->code = abc_setlocal_2(method->code))
code_t* abc_setlocal_3(code_t*prev);
#define setlocal_3(method) (method->code = abc_setlocal_3(method->code))
code_t* abc_setglobalslot(code_t*prev, int v);
#define setglobalslot(method,v) (method->code = abc_setglobalslot(method->code,v))
code_t* abc_setproperty(code_t*prev, char* name);
#define setproperty(method,name) (method->code = abc_setproperty(method->code,name))
code_t* abc_setproperty2(code_t*prev, multiname_t* name);
#define setproperty2(method,name) (method->code = abc_setproperty2(method->code,name))
code_t* abc_setslot(code_t*prev, int v);
#define setslot(method,v) (method->code = abc_setslot(method->code,v))
code_t* abc_setsuper(code_t*prev, char* name);
#define setsuper(method,name) (method->code = abc_setsuper(method->code,name))
code_t* abc_setsuper2(code_t*prev, multiname_t* name);
#define setsuper2(method,name) (method->code = abc_setsuper2(method->code,name))
code_t* abc_strictequals(code_t*prev);
#define strictequals(method) (method->code = abc_strictequals(method->code))
code_t* abc_subtract(code_t*prev);
#define subtract(method) (method->code = abc_subtract(method->code))
code_t* abc_subtract_i(code_t*prev);
#define subtract_i(method) (method->code = abc_subtract_i(method->code))
code_t* abc_swap(code_t*prev);
#define swap(method) (method->code = abc_swap(method->code))
code_t* abc_throw(code_t*prev);
#define throw(method) (method->code = abc_throw(method->code))
code_t* abc_typeof(code_t*prev);
#define typeof(method) (method->code = abc_typeof(method->code))
code_t* abc_urshift(code_t*prev);
#define urshift(method) (method->code = abc_urshift(method->code))
#endif
