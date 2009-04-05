#ifndef __opcodes_h__
#define __opcodes_h__
#include "abc.h"
#include "pool.h"
#include "code.h"
code_t* abc_add(code_t*prev);
#define add(method) (method->code = abc_add(method->code))
#define OPCODE_ADD 0xa0
code_t* abc_add_i(code_t*prev);
#define add_i(method) (method->code = abc_add_i(method->code))
#define OPCODE_ADD_I 0xc5
code_t* abc_astype(code_t*prev, char* name);
#define astype(method,name) (method->code = abc_astype(method->code,name))
#define OPCODE_ASTYPE 0x86
code_t* abc_astype2(code_t*prev, multiname_t* name);
#define astype2(method,name) (method->code = abc_astype2(method->code,name))
#define OPCODE_ASTYPE2 0x86
code_t* abc_astypelate(code_t*prev);
#define astypelate(method) (method->code = abc_astypelate(method->code))
#define OPCODE_ASTYPELATE 0x87
code_t* abc_bitand(code_t*prev);
#define bitand(method) (method->code = abc_bitand(method->code))
#define OPCODE_BITAND 0xA8
code_t* abc_bitnot(code_t*prev);
#define bitnot(method) (method->code = abc_bitnot(method->code))
#define OPCODE_BITNOT 0x97
code_t* abc_bitor(code_t*prev);
#define bitor(method) (method->code = abc_bitor(method->code))
#define OPCODE_BITOR 0xa9
code_t* abc_bitxor(code_t*prev);
#define bitxor(method) (method->code = abc_bitxor(method->code))
#define OPCODE_BITXOR 0xaa
code_t* abc_call(code_t*prev, int v);
#define call(method,v) (method->code = abc_call(method->code,v))
#define OPCODE_CALL 0x41
code_t* abc_callmethod(code_t*prev, abc_method_t* m, int v);
#define callmethod(method,m,v) (method->code = abc_callmethod(method->code,m,v))
#define OPCODE_CALLMETHOD 0x43
code_t* abc_callproplex(code_t*prev, char* name, int v);
#define callproplex(method,name,v) (method->code = abc_callproplex(method->code,name,v))
#define OPCODE_CALLPROPLEX 0x4c
code_t* abc_callproplex2(code_t*prev, multiname_t* name, int v);
#define callproplex2(method,name,v) (method->code = abc_callproplex2(method->code,name,v))
#define OPCODE_CALLPROPLEX2 0x4c
code_t* abc_callproperty(code_t*prev, char* name, int v);
#define callproperty(method,name,v) (method->code = abc_callproperty(method->code,name,v))
#define OPCODE_CALLPROPERTY 0x46
code_t* abc_callproperty2(code_t*prev, multiname_t* name, int v);
#define callproperty2(method,name,v) (method->code = abc_callproperty2(method->code,name,v))
#define OPCODE_CALLPROPERTY2 0x46
code_t* abc_callpropvoid(code_t*prev, char* name, int v);
#define callpropvoid(method,name,v) (method->code = abc_callpropvoid(method->code,name,v))
#define OPCODE_CALLPROPVOID 0x4f
code_t* abc_callpropvoid2(code_t*prev, multiname_t* name, int v);
#define callpropvoid2(method,name,v) (method->code = abc_callpropvoid2(method->code,name,v))
#define OPCODE_CALLPROPVOID2 0x4f
code_t* abc_callstatic(code_t*prev, abc_method_body_t* m, int v);
#define callstatic(method,m,v) (method->code = abc_callstatic(method->code,m,v))
#define OPCODE_CALLSTATIC 0x44
code_t* abc_callsuper(code_t*prev, char* name, int v);
#define callsuper(method,name,v) (method->code = abc_callsuper(method->code,name,v))
#define OPCODE_CALLSUPER 0x45
code_t* abc_callsuper2(code_t*prev, multiname_t* name, int v);
#define callsuper2(method,name,v) (method->code = abc_callsuper2(method->code,name,v))
#define OPCODE_CALLSUPER2 0x45
code_t* abc_callsupervoid(code_t*prev, char* name, int v);
#define callsupervoid(method,name,v) (method->code = abc_callsupervoid(method->code,name,v))
#define OPCODE_CALLSUPERVOID 0x4e
code_t* abc_callsupervoid2(code_t*prev, multiname_t* name, int v);
#define callsupervoid2(method,name,v) (method->code = abc_callsupervoid2(method->code,name,v))
#define OPCODE_CALLSUPERVOID2 0x4e
code_t* abc_checkfilter(code_t*prev);
#define checkfilter(method) (method->code = abc_checkfilter(method->code))
#define OPCODE_CHECKFILTER 0x78
code_t* abc_coerce(code_t*prev, char* name);
#define coerce(method,name) (method->code = abc_coerce(method->code,name))
#define OPCODE_COERCE 0x80
code_t* abc_coerce2(code_t*prev, multiname_t* name);
#define coerce2(method,name) (method->code = abc_coerce2(method->code,name))
#define OPCODE_COERCE2 0x80
code_t* abc_coerce_a(code_t*prev);
#define coerce_a(method) (method->code = abc_coerce_a(method->code))
#define OPCODE_COERCE_A 0x82
code_t* abc_coerce_s(code_t*prev);
#define coerce_s(method) (method->code = abc_coerce_s(method->code))
#define OPCODE_COERCE_S 0x85
code_t* abc_construct(code_t*prev, int v);
#define construct(method,v) (method->code = abc_construct(method->code,v))
#define OPCODE_CONSTRUCT 0x42
code_t* abc_constructprop(code_t*prev, char* name, int v);
#define constructprop(method,name,v) (method->code = abc_constructprop(method->code,name,v))
#define OPCODE_CONSTRUCTPROP 0x4a
code_t* abc_constructprop2(code_t*prev, multiname_t* name, int v);
#define constructprop2(method,name,v) (method->code = abc_constructprop2(method->code,name,v))
#define OPCODE_CONSTRUCTPROP2 0x4a
code_t* abc_constructsuper(code_t*prev, int v);
#define constructsuper(method,v) (method->code = abc_constructsuper(method->code,v))
#define OPCODE_CONSTRUCTSUPER 0x49
code_t* abc_convert_b(code_t*prev);
#define convert_b(method) (method->code = abc_convert_b(method->code))
#define OPCODE_CONVERT_B 0x76
code_t* abc_convert_i(code_t*prev);
#define convert_i(method) (method->code = abc_convert_i(method->code))
#define OPCODE_CONVERT_I 0x73
code_t* abc_convert_d(code_t*prev);
#define convert_d(method) (method->code = abc_convert_d(method->code))
#define OPCODE_CONVERT_D 0x75
code_t* abc_convert_o(code_t*prev);
#define convert_o(method) (method->code = abc_convert_o(method->code))
#define OPCODE_CONVERT_O 0x77
code_t* abc_convert_u(code_t*prev);
#define convert_u(method) (method->code = abc_convert_u(method->code))
#define OPCODE_CONVERT_U 0x74
code_t* abc_convert_s(code_t*prev);
#define convert_s(method) (method->code = abc_convert_s(method->code))
#define OPCODE_CONVERT_S 0x70
code_t* abc_debug(code_t*prev, void* debuginfo);
#define debug(method,debuginfo) (method->code = abc_debug(method->code,debuginfo))
#define OPCODE_DEBUG 0xef
code_t* abc_debugfile(code_t*prev, char* name);
#define debugfile(method,name) (method->code = abc_debugfile(method->code,name))
#define OPCODE_DEBUGFILE 0xf1
code_t* abc_debugfile2(code_t*prev, string_t* s);
#define debugfile2(method,s) (method->code = abc_debugfile2(method->code,s))
#define OPCODE_DEBUGFILE2 0xf1
code_t* abc_debugline(code_t*prev, int v);
#define debugline(method,v) (method->code = abc_debugline(method->code,v))
#define OPCODE_DEBUGLINE 0xf0
code_t* abc_declocal(code_t*prev, int reg);
#define declocal(method,reg) (method->code = abc_declocal(method->code,reg))
#define OPCODE_DECLOCAL 0x94
code_t* abc_declocal_i(code_t*prev, int reg);
#define declocal_i(method,reg) (method->code = abc_declocal_i(method->code,reg))
#define OPCODE_DECLOCAL_I 0xc3
code_t* abc_decrement(code_t*prev);
#define decrement(method) (method->code = abc_decrement(method->code))
#define OPCODE_DECREMENT 0x93
code_t* abc_decrement_i(code_t*prev);
#define decrement_i(method) (method->code = abc_decrement_i(method->code))
#define OPCODE_DECREMENT_I 0xc1
code_t* abc_deleteproperty(code_t*prev, char* name);
#define deleteproperty(method,name) (method->code = abc_deleteproperty(method->code,name))
#define OPCODE_DELETEPROPERTY 0x6a
code_t* abc_deleteproperty2(code_t*prev, multiname_t* name);
#define deleteproperty2(method,name) (method->code = abc_deleteproperty2(method->code,name))
#define OPCODE_DELETEPROPERTY2 0x6a
code_t* abc_divide(code_t*prev);
#define divide(method) (method->code = abc_divide(method->code))
#define OPCODE_DIVIDE 0xa3
code_t* abc_dup(code_t*prev);
#define dup(method) (method->code = abc_dup(method->code))
#define OPCODE_DUP 0x2a
code_t* abc_dxns(code_t*prev, char* name);
#define dxns(method,name) (method->code = abc_dxns(method->code,name))
#define OPCODE_DXNS 0x06
code_t* abc_dxns2(code_t*prev, string_t* s);
#define dxns2(method,s) (method->code = abc_dxns2(method->code,s))
#define OPCODE_DXNS2 0x06
code_t* abc_dxnslate(code_t*prev);
#define dxnslate(method) (method->code = abc_dxnslate(method->code))
#define OPCODE_DXNSLATE 0x07
code_t* abc_equals(code_t*prev);
#define equals(method) (method->code = abc_equals(method->code))
#define OPCODE_EQUALS 0xab
code_t* abc_esc_xattr(code_t*prev);
#define esc_xattr(method) (method->code = abc_esc_xattr(method->code))
#define OPCODE_ESC_XATTR 0x72
code_t* abc_esc_xelem(code_t*prev);
#define esc_xelem(method) (method->code = abc_esc_xelem(method->code))
#define OPCODE_ESC_XELEM 0x71
code_t* abc_findproperty(code_t*prev, char* name);
#define findproperty(method,name) (method->code = abc_findproperty(method->code,name))
#define OPCODE_FINDPROPERTY 0x5e
code_t* abc_findproperty2(code_t*prev, multiname_t* name);
#define findproperty2(method,name) (method->code = abc_findproperty2(method->code,name))
#define OPCODE_FINDPROPERTY2 0x5e
code_t* abc_findpropstrict(code_t*prev, char* name);
#define findpropstrict(method,name) (method->code = abc_findpropstrict(method->code,name))
#define OPCODE_FINDPROPSTRICT 0x5d
code_t* abc_findpropstrict2(code_t*prev, multiname_t* name);
#define findpropstrict2(method,name) (method->code = abc_findpropstrict2(method->code,name))
#define OPCODE_FINDPROPSTRICT2 0x5d
code_t* abc_getdescendants(code_t*prev, char* name);
#define getdescendants(method,name) (method->code = abc_getdescendants(method->code,name))
#define OPCODE_GETDESCENDANTS 0x59
code_t* abc_getdescendants2(code_t*prev, multiname_t* name);
#define getdescendants2(method,name) (method->code = abc_getdescendants2(method->code,name))
#define OPCODE_GETDESCENDANTS2 0x59
code_t* abc_getglobalscope(code_t*prev);
#define getglobalscope(method) (method->code = abc_getglobalscope(method->code))
#define OPCODE_GETGLOBALSCOPE 0x64
code_t* abc_getglobalslot(code_t*prev, int v);
#define getglobalslot(method,v) (method->code = abc_getglobalslot(method->code,v))
#define OPCODE_GETGLOBALSLOT 0x6e
code_t* abc_getlex(code_t*prev, char* name);
#define getlex(method,name) (method->code = abc_getlex(method->code,name))
#define OPCODE_GETLEX 0x60
code_t* abc_getlex2(code_t*prev, multiname_t* name);
#define getlex2(method,name) (method->code = abc_getlex2(method->code,name))
#define OPCODE_GETLEX2 0x60
code_t* abc_getlocal(code_t*prev, int reg);
#define getlocal(method,reg) (method->code = abc_getlocal(method->code,reg))
#define OPCODE_GETLOCAL 0x62
code_t* abc_getlocal_0(code_t*prev);
#define getlocal_0(method) (method->code = abc_getlocal_0(method->code))
#define OPCODE_GETLOCAL_0 0xd0
code_t* abc_getlocal_1(code_t*prev);
#define getlocal_1(method) (method->code = abc_getlocal_1(method->code))
#define OPCODE_GETLOCAL_1 0xd1
code_t* abc_getlocal_2(code_t*prev);
#define getlocal_2(method) (method->code = abc_getlocal_2(method->code))
#define OPCODE_GETLOCAL_2 0xd2
code_t* abc_getlocal_3(code_t*prev);
#define getlocal_3(method) (method->code = abc_getlocal_3(method->code))
#define OPCODE_GETLOCAL_3 0xd3
code_t* abc_getproperty(code_t*prev, char* name);
#define getproperty(method,name) (method->code = abc_getproperty(method->code,name))
#define OPCODE_GETPROPERTY 0x66
code_t* abc_getproperty2(code_t*prev, multiname_t* name);
#define getproperty2(method,name) (method->code = abc_getproperty2(method->code,name))
#define OPCODE_GETPROPERTY2 0x66
code_t* abc_getscopeobject(code_t*prev, int v);
#define getscopeobject(method,v) (method->code = abc_getscopeobject(method->code,v))
#define OPCODE_GETSCOPEOBJECT 0x65
code_t* abc_getslot(code_t*prev, int v);
#define getslot(method,v) (method->code = abc_getslot(method->code,v))
#define OPCODE_GETSLOT 0x6c
code_t* abc_getsuper(code_t*prev, char* name);
#define getsuper(method,name) (method->code = abc_getsuper(method->code,name))
#define OPCODE_GETSUPER 0x04
code_t* abc_getsuper2(code_t*prev, multiname_t* name);
#define getsuper2(method,name) (method->code = abc_getsuper2(method->code,name))
#define OPCODE_GETSUPER2 0x04
code_t* abc_greaterthan(code_t*prev);
#define greaterthan(method) (method->code = abc_greaterthan(method->code))
#define OPCODE_GREATERTHAN 0xaf
code_t* abc_greaterequals(code_t*prev);
#define greaterequals(method) (method->code = abc_greaterequals(method->code))
#define OPCODE_GREATEREQUALS 0xb0
code_t* abc_hasnext(code_t*prev);
#define hasnext(method) (method->code = abc_hasnext(method->code))
#define OPCODE_HASNEXT 0x1f
code_t* abc_hasnext2(code_t*prev, int reg, int reg2);
#define hasnext2(method,reg,reg2) (method->code = abc_hasnext2(method->code,reg,reg2))
#define OPCODE_HASNEXT2 0x32
code_t* abc_ifeq(code_t*prev, code_t* label);
#define ifeq(method,label) (method->code = abc_ifeq(method->code,label))
#define OPCODE_IFEQ 0x13
code_t* abc_iffalse(code_t*prev, code_t* label);
#define iffalse(method,label) (method->code = abc_iffalse(method->code,label))
#define OPCODE_IFFALSE 0x12
code_t* abc_ifge(code_t*prev, code_t* label);
#define ifge(method,label) (method->code = abc_ifge(method->code,label))
#define OPCODE_IFGE 0x18
code_t* abc_ifgt(code_t*prev, code_t* label);
#define ifgt(method,label) (method->code = abc_ifgt(method->code,label))
#define OPCODE_IFGT 0x17
code_t* abc_ifle(code_t*prev, code_t* label);
#define ifle(method,label) (method->code = abc_ifle(method->code,label))
#define OPCODE_IFLE 0x16
code_t* abc_iflt(code_t*prev, code_t* label);
#define iflt(method,label) (method->code = abc_iflt(method->code,label))
#define OPCODE_IFLT 0x15
code_t* abc_ifnge(code_t*prev, code_t* label);
#define ifnge(method,label) (method->code = abc_ifnge(method->code,label))
#define OPCODE_IFNGE 0x0f
code_t* abc_ifngt(code_t*prev, code_t* label);
#define ifngt(method,label) (method->code = abc_ifngt(method->code,label))
#define OPCODE_IFNGT 0x0e
code_t* abc_ifnle(code_t*prev, code_t* label);
#define ifnle(method,label) (method->code = abc_ifnle(method->code,label))
#define OPCODE_IFNLE 0x0d
code_t* abc_ifnlt(code_t*prev, code_t* label);
#define ifnlt(method,label) (method->code = abc_ifnlt(method->code,label))
#define OPCODE_IFNLT 0x0c
code_t* abc_ifne(code_t*prev, code_t* label);
#define ifne(method,label) (method->code = abc_ifne(method->code,label))
#define OPCODE_IFNE 0x14
code_t* abc_ifstricteq(code_t*prev, code_t* label);
#define ifstricteq(method,label) (method->code = abc_ifstricteq(method->code,label))
#define OPCODE_IFSTRICTEQ 0x19
code_t* abc_ifstrictne(code_t*prev, code_t* label);
#define ifstrictne(method,label) (method->code = abc_ifstrictne(method->code,label))
#define OPCODE_IFSTRICTNE 0x1a
code_t* abc_iftrue(code_t*prev, code_t* label);
#define iftrue(method,label) (method->code = abc_iftrue(method->code,label))
#define OPCODE_IFTRUE 0x11
code_t* abc_in(code_t*prev);
#define in(method) (method->code = abc_in(method->code))
#define OPCODE_IN 0xb4
code_t* abc_inclocal(code_t*prev, int reg);
#define inclocal(method,reg) (method->code = abc_inclocal(method->code,reg))
#define OPCODE_INCLOCAL 0x92
code_t* abc_inclocal_i(code_t*prev, int reg);
#define inclocal_i(method,reg) (method->code = abc_inclocal_i(method->code,reg))
#define OPCODE_INCLOCAL_I 0xc2
code_t* abc_increment(code_t*prev);
#define increment(method) (method->code = abc_increment(method->code))
#define OPCODE_INCREMENT 0x91
code_t* abc_increment_i(code_t*prev);
#define increment_i(method) (method->code = abc_increment_i(method->code))
#define OPCODE_INCREMENT_I 0xc0
code_t* abc_initproperty(code_t*prev, char* name);
#define initproperty(method,name) (method->code = abc_initproperty(method->code,name))
#define OPCODE_INITPROPERTY 0x68
code_t* abc_initproperty2(code_t*prev, multiname_t* name);
#define initproperty2(method,name) (method->code = abc_initproperty2(method->code,name))
#define OPCODE_INITPROPERTY2 0x68
code_t* abc_instanceof(code_t*prev);
#define instanceof(method) (method->code = abc_instanceof(method->code))
#define OPCODE_INSTANCEOF 0xb1
code_t* abc_istype(code_t*prev, char* name);
#define istype(method,name) (method->code = abc_istype(method->code,name))
#define OPCODE_ISTYPE 0xb2
code_t* abc_istype2(code_t*prev, multiname_t* name);
#define istype2(method,name) (method->code = abc_istype2(method->code,name))
#define OPCODE_ISTYPE2 0xb2
code_t* abc_istypelate(code_t*prev);
#define istypelate(method) (method->code = abc_istypelate(method->code))
#define OPCODE_ISTYPELATE 0xb3
code_t* abc_jump(code_t*prev, code_t* label);
#define jump(method,label) (method->code = abc_jump(method->code,label))
#define OPCODE_JUMP 0x10
code_t* abc_kill(code_t*prev, int reg);
#define kill(method,reg) (method->code = abc_kill(method->code,reg))
#define OPCODE_KILL 0x08
code_t* abc_label(code_t*prev);
#define label(method) (method->code = abc_label(method->code))
#define OPCODE_LABEL 0x09
code_t* abc_lessequals(code_t*prev);
#define lessequals(method) (method->code = abc_lessequals(method->code))
#define OPCODE_LESSEQUALS 0xae
code_t* abc_lessthan(code_t*prev);
#define lessthan(method) (method->code = abc_lessthan(method->code))
#define OPCODE_LESSTHAN 0xad
code_t* abc_lookupswitch(code_t*prev, lookupswitch_t* l);
#define lookupswitch(method,l) (method->code = abc_lookupswitch(method->code,l))
#define OPCODE_LOOKUPSWITCH 0x1b
code_t* abc_lshift(code_t*prev);
#define lshift(method) (method->code = abc_lshift(method->code))
#define OPCODE_LSHIFT 0xa5
code_t* abc_modulo(code_t*prev);
#define modulo(method) (method->code = abc_modulo(method->code))
#define OPCODE_MODULO 0xa4
code_t* abc_multiply(code_t*prev);
#define multiply(method) (method->code = abc_multiply(method->code))
#define OPCODE_MULTIPLY 0xa2
code_t* abc_multiply_i(code_t*prev);
#define multiply_i(method) (method->code = abc_multiply_i(method->code))
#define OPCODE_MULTIPLY_I 0xc7
code_t* abc_negate(code_t*prev);
#define negate(method) (method->code = abc_negate(method->code))
#define OPCODE_NEGATE 0x90
code_t* abc_negate_i(code_t*prev);
#define negate_i(method) (method->code = abc_negate_i(method->code))
#define OPCODE_NEGATE_I 0xc4
code_t* abc_newactivation(code_t*prev);
#define newactivation(method) (method->code = abc_newactivation(method->code))
#define OPCODE_NEWACTIVATION 0x57
code_t* abc_newarray(code_t*prev, int v);
#define newarray(method,v) (method->code = abc_newarray(method->code,v))
#define OPCODE_NEWARRAY 0x56
code_t* abc_newcatch(code_t*prev, int v);
#define newcatch(method,v) (method->code = abc_newcatch(method->code,v))
#define OPCODE_NEWCATCH 0x5a
code_t* abc_newclass(code_t*prev, abc_class_t* m);
#define newclass(method,m) (method->code = abc_newclass(method->code,m))
#define OPCODE_NEWCLASS 0x58
code_t* abc_newfunction(code_t*prev, abc_method_t* m);
#define newfunction(method,m) (method->code = abc_newfunction(method->code,m))
#define OPCODE_NEWFUNCTION 0x40
code_t* abc_newobject(code_t*prev, int v);
#define newobject(method,v) (method->code = abc_newobject(method->code,v))
#define OPCODE_NEWOBJECT 0x55
code_t* abc_nextname(code_t*prev);
#define nextname(method) (method->code = abc_nextname(method->code))
#define OPCODE_NEXTNAME 0x1e
code_t* abc_nextvalue(code_t*prev);
#define nextvalue(method) (method->code = abc_nextvalue(method->code))
#define OPCODE_NEXTVALUE 0x23
code_t* abc_nop(code_t*prev);
#define nop(method) (method->code = abc_nop(method->code))
#define OPCODE_NOP 0x02
code_t* abc_not(code_t*prev);
#define not(method) (method->code = abc_not(method->code))
#define OPCODE_NOT 0x96
code_t* abc_pop(code_t*prev);
#define pop(method) (method->code = abc_pop(method->code))
#define OPCODE_POP 0x29
code_t* abc_popscope(code_t*prev);
#define popscope(method) (method->code = abc_popscope(method->code))
#define OPCODE_POPSCOPE 0x1d
code_t* abc_pushbyte(code_t*prev, int v);
#define pushbyte(method,v) (method->code = abc_pushbyte(method->code,v))
#define OPCODE_PUSHBYTE 0x24
code_t* abc_pushdouble(code_t*prev, double f);
#define pushdouble(method,f) (method->code = abc_pushdouble(method->code,f))
#define OPCODE_PUSHDOUBLE 0x2f
code_t* abc_pushfalse(code_t*prev);
#define pushfalse(method) (method->code = abc_pushfalse(method->code))
#define OPCODE_PUSHFALSE 0x27
code_t* abc_pushint(code_t*prev, int i);
#define pushint(method,i) (method->code = abc_pushint(method->code,i))
#define OPCODE_PUSHINT 0x2d
code_t* abc_pushnamespace(code_t*prev, namespace_t* ns);
#define pushnamespace(method,ns) (method->code = abc_pushnamespace(method->code,ns))
#define OPCODE_PUSHNAMESPACE 0x31
code_t* abc_pushnan(code_t*prev);
#define pushnan(method) (method->code = abc_pushnan(method->code))
#define OPCODE_PUSHNAN 0x28
code_t* abc_pushnull(code_t*prev);
#define pushnull(method) (method->code = abc_pushnull(method->code))
#define OPCODE_PUSHNULL 0x20
code_t* abc_pushscope(code_t*prev);
#define pushscope(method) (method->code = abc_pushscope(method->code))
#define OPCODE_PUSHSCOPE 0x30
code_t* abc_pushshort(code_t*prev, int v);
#define pushshort(method,v) (method->code = abc_pushshort(method->code,v))
#define OPCODE_PUSHSHORT 0x25
code_t* abc_pushstring(code_t*prev, char* name);
#define pushstring(method,name) (method->code = abc_pushstring(method->code,name))
#define OPCODE_PUSHSTRING 0x2c
code_t* abc_pushstring2(code_t*prev, string_t* s);
#define pushstring2(method,s) (method->code = abc_pushstring2(method->code,s))
#define OPCODE_PUSHSTRING2 0x2c
code_t* abc_pushtrue(code_t*prev);
#define pushtrue(method) (method->code = abc_pushtrue(method->code))
#define OPCODE_PUSHTRUE 0x26
code_t* abc_pushuint(code_t*prev, unsigned int u);
#define pushuint(method,u) (method->code = abc_pushuint(method->code,u))
#define OPCODE_PUSHUINT 0x2e
code_t* abc_pushundefined(code_t*prev);
#define pushundefined(method) (method->code = abc_pushundefined(method->code))
#define OPCODE_PUSHUNDEFINED 0x21
code_t* abc_pushwith(code_t*prev);
#define pushwith(method) (method->code = abc_pushwith(method->code))
#define OPCODE_PUSHWITH 0x1c
code_t* abc_returnvalue(code_t*prev);
#define returnvalue(method) (method->code = abc_returnvalue(method->code))
#define OPCODE_RETURNVALUE 0x48
code_t* abc_returnvoid(code_t*prev);
#define returnvoid(method) (method->code = abc_returnvoid(method->code))
#define OPCODE_RETURNVOID 0x47
code_t* abc_rshift(code_t*prev);
#define rshift(method) (method->code = abc_rshift(method->code))
#define OPCODE_RSHIFT 0xa6
code_t* abc_setlocal(code_t*prev, int reg);
#define setlocal(method,reg) (method->code = abc_setlocal(method->code,reg))
#define OPCODE_SETLOCAL 0x63
code_t* abc_setlocal_0(code_t*prev);
#define setlocal_0(method) (method->code = abc_setlocal_0(method->code))
#define OPCODE_SETLOCAL_0 0xd4
code_t* abc_setlocal_1(code_t*prev);
#define setlocal_1(method) (method->code = abc_setlocal_1(method->code))
#define OPCODE_SETLOCAL_1 0xd5
code_t* abc_setlocal_2(code_t*prev);
#define setlocal_2(method) (method->code = abc_setlocal_2(method->code))
#define OPCODE_SETLOCAL_2 0xd6
code_t* abc_setlocal_3(code_t*prev);
#define setlocal_3(method) (method->code = abc_setlocal_3(method->code))
#define OPCODE_SETLOCAL_3 0xd7
code_t* abc_setglobalslot(code_t*prev, int v);
#define setglobalslot(method,v) (method->code = abc_setglobalslot(method->code,v))
#define OPCODE_SETGLOBALSLOT 0x6f
code_t* abc_setproperty(code_t*prev, char* name);
#define setproperty(method,name) (method->code = abc_setproperty(method->code,name))
#define OPCODE_SETPROPERTY 0x61
code_t* abc_setproperty2(code_t*prev, multiname_t* name);
#define setproperty2(method,name) (method->code = abc_setproperty2(method->code,name))
#define OPCODE_SETPROPERTY2 0x61
code_t* abc_setslot(code_t*prev, int v);
#define setslot(method,v) (method->code = abc_setslot(method->code,v))
#define OPCODE_SETSLOT 0x6d
code_t* abc_setsuper(code_t*prev, char* name);
#define setsuper(method,name) (method->code = abc_setsuper(method->code,name))
#define OPCODE_SETSUPER 0x05
code_t* abc_setsuper2(code_t*prev, multiname_t* name);
#define setsuper2(method,name) (method->code = abc_setsuper2(method->code,name))
#define OPCODE_SETSUPER2 0x05
code_t* abc_strictequals(code_t*prev);
#define strictequals(method) (method->code = abc_strictequals(method->code))
#define OPCODE_STRICTEQUALS 0xac
code_t* abc_subtract(code_t*prev);
#define subtract(method) (method->code = abc_subtract(method->code))
#define OPCODE_SUBTRACT 0xa1
code_t* abc_subtract_i(code_t*prev);
#define subtract_i(method) (method->code = abc_subtract_i(method->code))
#define OPCODE_SUBTRACT_I 0xc6
code_t* abc_swap(code_t*prev);
#define swap(method) (method->code = abc_swap(method->code))
#define OPCODE_SWAP 0x2b
code_t* abc_throw(code_t*prev);
#define throw(method) (method->code = abc_throw(method->code))
#define OPCODE_THROW 0x03
code_t* abc_typeof(code_t*prev);
#define typeof(method) (method->code = abc_typeof(method->code))
#define OPCODE_TYPEOF 0x95
code_t* abc_urshift(code_t*prev);
#define urshift(method) (method->code = abc_urshift(method->code))
#define OPCODE_URSHIFT 0xa7
code_t* abc_applytype(code_t*prev, int v);
#define applytype(method,v) (method->code = abc_applytype(method->code,v))
#define OPCODE_APPLYTYPE 0x53
code_t* abc___pushpackage__(code_t*prev, char* name);
#define __pushpackage__(method,name) (method->code = abc___pushpackage__(method->code,name))
#define OPCODE___PUSHPACKAGE__ 0xfb
code_t* abc___pushpackage__2(code_t*prev, string_t* s);
#define __pushpackage__2(method,s) (method->code = abc___pushpackage__2(method->code,s))
#define OPCODE___PUSHPACKAGE__2 0xfb
code_t* abc___rethrow__(code_t*prev);
#define __rethrow__(method) (method->code = abc___rethrow__(method->code))
#define OPCODE___RETHROW__ 0xfc
code_t* abc___fallthrough__(code_t*prev, char* name);
#define __fallthrough__(method,name) (method->code = abc___fallthrough__(method->code,name))
#define OPCODE___FALLTHROUGH__ 0xfd
code_t* abc___fallthrough__2(code_t*prev, string_t* s);
#define __fallthrough__2(method,s) (method->code = abc___fallthrough__2(method->code,s))
#define OPCODE___FALLTHROUGH__2 0xfd
code_t* abc___continue__(code_t*prev, char* name);
#define __continue__(method,name) (method->code = abc___continue__(method->code,name))
#define OPCODE___CONTINUE__ 0xfe
code_t* abc___continue__2(code_t*prev, string_t* s);
#define __continue__2(method,s) (method->code = abc___continue__2(method->code,s))
#define OPCODE___CONTINUE__2 0xfe
code_t* abc___break__(code_t*prev, char* name);
#define __break__(method,name) (method->code = abc___break__(method->code,name))
#define OPCODE___BREAK__ 0xff
code_t* abc___break__2(code_t*prev, string_t* s);
#define __break__2(method,s) (method->code = abc___break__2(method->code,s))
#define OPCODE___BREAK__2 0xff
#endif
