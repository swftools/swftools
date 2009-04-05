#include "opcodes.h"
code_t* abc_add(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xa0);
    return self;
}
code_t* abc_add_i(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xc5);
    return self;
}
code_t* abc_astype(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x86);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_astype2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x86);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_astypelate(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x87);
    return self;
}
code_t* abc_bitand(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xA8);
    return self;
}
code_t* abc_bitnot(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x97);
    return self;
}
code_t* abc_bitor(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xa9);
    return self;
}
code_t* abc_bitxor(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xaa);
    return self;
}
code_t* abc_call(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x41);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callmethod(code_t*prev, abc_method_t* m, int v)
{
    code_t*self = add_opcode(prev, 0x43);
    self->data[0] = m;
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callproplex(code_t*prev, char* name, int v)
{
    code_t*self = add_opcode(prev, 0x4c);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callproplex2(code_t*prev, multiname_t* name, int v)
{
    code_t*self = add_opcode(prev, 0x4c);
    self->data[0] = multiname_clone(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callproperty(code_t*prev, char* name, int v)
{
    code_t*self = add_opcode(prev, 0x46);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callproperty2(code_t*prev, multiname_t* name, int v)
{
    code_t*self = add_opcode(prev, 0x46);
    self->data[0] = multiname_clone(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callpropvoid(code_t*prev, char* name, int v)
{
    code_t*self = add_opcode(prev, 0x4f);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callpropvoid2(code_t*prev, multiname_t* name, int v)
{
    code_t*self = add_opcode(prev, 0x4f);
    self->data[0] = multiname_clone(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callstatic(code_t*prev, abc_method_body_t* m, int v)
{
    code_t*self = add_opcode(prev, 0x44);
    self->data[0] = m;
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callsuper(code_t*prev, char* name, int v)
{
    code_t*self = add_opcode(prev, 0x45);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callsuper2(code_t*prev, multiname_t* name, int v)
{
    code_t*self = add_opcode(prev, 0x45);
    self->data[0] = multiname_clone(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callsupervoid(code_t*prev, char* name, int v)
{
    code_t*self = add_opcode(prev, 0x4e);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_callsupervoid2(code_t*prev, multiname_t* name, int v)
{
    code_t*self = add_opcode(prev, 0x4e);
    self->data[0] = multiname_clone(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_checkfilter(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x78);
    return self;
}
code_t* abc_coerce(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x80);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_coerce2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x80);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_coerce_a(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x82);
    return self;
}
code_t* abc_coerce_s(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x85);
    return self;
}
code_t* abc_construct(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x42);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_constructprop(code_t*prev, char* name, int v)
{
    code_t*self = add_opcode(prev, 0x4a);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_constructprop2(code_t*prev, multiname_t* name, int v)
{
    code_t*self = add_opcode(prev, 0x4a);
    self->data[0] = multiname_clone(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_constructsuper(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x49);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_convert_b(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x76);
    return self;
}
code_t* abc_convert_i(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x73);
    return self;
}
code_t* abc_convert_d(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x75);
    return self;
}
code_t* abc_convert_o(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x77);
    return self;
}
code_t* abc_convert_u(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x74);
    return self;
}
code_t* abc_convert_s(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x70);
    return self;
}
code_t* abc_debug(code_t*prev, void* debuginfo)
{
    code_t*self = add_opcode(prev, 0xef);
    /* FIXME: write debuginfo debuginfo */
    return self;
}
code_t* abc_debugfile(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0xf1);
    self->data[0] = string_new4(name);
    return self;
}
code_t* abc_debugfile2(code_t*prev, string_t* s)
{
    code_t*self = add_opcode(prev, 0xf1);
    self->data[0] = string_dup3(s);
    return self;
}
code_t* abc_debugline(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0xf0);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_declocal(code_t*prev, int reg)
{
    code_t*self = add_opcode(prev, 0x94);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
code_t* abc_declocal_i(code_t*prev, int reg)
{
    code_t*self = add_opcode(prev, 0xc3);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
code_t* abc_decrement(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x93);
    return self;
}
code_t* abc_decrement_i(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xc1);
    return self;
}
code_t* abc_deleteproperty(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x6a);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_deleteproperty2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x6a);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_divide(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xa3);
    return self;
}
code_t* abc_dup(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x2a);
    return self;
}
code_t* abc_dxns(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x06);
    self->data[0] = string_new4(name);
    return self;
}
code_t* abc_dxns2(code_t*prev, string_t* s)
{
    code_t*self = add_opcode(prev, 0x06);
    self->data[0] = string_dup3(s);
    return self;
}
code_t* abc_dxnslate(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x07);
    return self;
}
code_t* abc_equals(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xab);
    return self;
}
code_t* abc_esc_xattr(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x72);
    return self;
}
code_t* abc_esc_xelem(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x71);
    return self;
}
code_t* abc_findproperty(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x5e);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_findproperty2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x5e);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_findpropstrict(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x5d);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_findpropstrict2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x5d);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_getdescendants(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x59);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_getdescendants2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x59);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_getglobalscope(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x64);
    return self;
}
code_t* abc_getglobalslot(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x6e);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_getlex(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x60);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_getlex2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x60);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_getlocal(code_t*prev, int reg)
{
    code_t*self = add_opcode(prev, 0x62);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
code_t* abc_getlocal_0(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xd0);
    return self;
}
code_t* abc_getlocal_1(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xd1);
    return self;
}
code_t* abc_getlocal_2(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xd2);
    return self;
}
code_t* abc_getlocal_3(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xd3);
    return self;
}
code_t* abc_getproperty(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x66);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_getproperty2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x66);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_getscopeobject(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x65);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_getslot(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x6c);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_getsuper(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x04);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_getsuper2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x04);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_greaterthan(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xaf);
    return self;
}
code_t* abc_greaterequals(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xb0);
    return self;
}
code_t* abc_hasnext(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x1f);
    return self;
}
code_t* abc_hasnext2(code_t*prev, int reg, int reg2)
{
    code_t*self = add_opcode(prev, 0x32);
    self->data[0] = (void*)(ptroff_t)reg;
    self->data[1] = (void*)(ptroff_t)reg2;
    return self;
}
code_t* abc_ifeq(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x13);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_iffalse(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x12);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifge(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x18);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifgt(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x17);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifle(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x16);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_iflt(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x15);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifnge(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x0f);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifngt(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x0e);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifnle(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x0d);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifnlt(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x0c);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifne(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x14);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifstricteq(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x19);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_ifstrictne(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x1a);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_iftrue(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x11);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_in(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xb4);
    return self;
}
code_t* abc_inclocal(code_t*prev, int reg)
{
    code_t*self = add_opcode(prev, 0x92);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
code_t* abc_inclocal_i(code_t*prev, int reg)
{
    code_t*self = add_opcode(prev, 0xc2);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
code_t* abc_increment(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x91);
    return self;
}
code_t* abc_increment_i(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xc0);
    return self;
}
code_t* abc_initproperty(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x68);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_initproperty2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x68);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_instanceof(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xb1);
    return self;
}
code_t* abc_istype(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0xb2);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_istype2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0xb2);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_istypelate(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xb3);
    return self;
}
code_t* abc_jump(code_t*prev, code_t* label)
{
    code_t*self = add_opcode(prev, 0x10);
    self->data[0] = 0; //placeholder
    self->branch = label;
    return self;
}
code_t* abc_kill(code_t*prev, int reg)
{
    code_t*self = add_opcode(prev, 0x08);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
code_t* abc_label(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x09);
    return self;
}
code_t* abc_lessequals(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xae);
    return self;
}
code_t* abc_lessthan(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xad);
    return self;
}
code_t* abc_lookupswitch(code_t*prev, lookupswitch_t* l)
{
    code_t*self = add_opcode(prev, 0x1b);
    self->data[0] = l;
    return self;
}
code_t* abc_lshift(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xa5);
    return self;
}
code_t* abc_modulo(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xa4);
    return self;
}
code_t* abc_multiply(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xa2);
    return self;
}
code_t* abc_multiply_i(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xc7);
    return self;
}
code_t* abc_negate(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x90);
    return self;
}
code_t* abc_negate_i(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xc4);
    return self;
}
code_t* abc_newactivation(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x57);
    return self;
}
code_t* abc_newarray(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x56);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_newcatch(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x5a);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_newclass(code_t*prev, abc_class_t* m)
{
    code_t*self = add_opcode(prev, 0x58);
    self->data[0] = m;
    return self;
}
code_t* abc_newfunction(code_t*prev, abc_method_t* m)
{
    code_t*self = add_opcode(prev, 0x40);
    self->data[0] = m;
    return self;
}
code_t* abc_newobject(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x55);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_nextname(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x1e);
    return self;
}
code_t* abc_nextvalue(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x23);
    return self;
}
code_t* abc_nop(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x02);
    return self;
}
code_t* abc_not(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x96);
    return self;
}
code_t* abc_pop(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x29);
    return self;
}
code_t* abc_popscope(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x1d);
    return self;
}
code_t* abc_pushbyte(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x24);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_pushdouble(code_t*prev, double f)
{
    code_t*self = add_opcode(prev, 0x2f);
    double*fp = malloc(sizeof(double));
    *fp = f;
    self->data[0] = fp;
    return self;
}
code_t* abc_pushfalse(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x27);
    return self;
}
code_t* abc_pushint(code_t*prev, int i)
{
    code_t*self = add_opcode(prev, 0x2d);
    self->data[0] = (void*)(ptroff_t)i;
    return self;
}
code_t* abc_pushnamespace(code_t*prev, namespace_t* ns)
{
    code_t*self = add_opcode(prev, 0x31);
    self->data[0] = namespace_clone(ns);
    return self;
}
code_t* abc_pushnan(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x28);
    return self;
}
code_t* abc_pushnull(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x20);
    return self;
}
code_t* abc_pushscope(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x30);
    return self;
}
code_t* abc_pushshort(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x25);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_pushstring(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x2c);
    self->data[0] = string_new4(name);
    return self;
}
code_t* abc_pushstring2(code_t*prev, string_t* s)
{
    code_t*self = add_opcode(prev, 0x2c);
    self->data[0] = string_dup3(s);
    return self;
}
code_t* abc_pushtrue(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x26);
    return self;
}
code_t* abc_pushuint(code_t*prev, unsigned int u)
{
    code_t*self = add_opcode(prev, 0x2e);
    self->data[0] = (void*)(ptroff_t)u;
    return self;
}
code_t* abc_pushundefined(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x21);
    return self;
}
code_t* abc_pushwith(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x1c);
    return self;
}
code_t* abc_returnvalue(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x48);
    return self;
}
code_t* abc_returnvoid(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x47);
    return self;
}
code_t* abc_rshift(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xa6);
    return self;
}
code_t* abc_setlocal(code_t*prev, int reg)
{
    code_t*self = add_opcode(prev, 0x63);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
code_t* abc_setlocal_0(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xd4);
    return self;
}
code_t* abc_setlocal_1(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xd5);
    return self;
}
code_t* abc_setlocal_2(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xd6);
    return self;
}
code_t* abc_setlocal_3(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xd7);
    return self;
}
code_t* abc_setglobalslot(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x6f);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_setproperty(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x61);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_setproperty2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x61);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_setslot(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x6d);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc_setsuper(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0x05);
    self->data[0] = multiname_fromstring(name);
    return self;
}
code_t* abc_setsuper2(code_t*prev, multiname_t* name)
{
    code_t*self = add_opcode(prev, 0x05);
    self->data[0] = multiname_clone(name);
    return self;
}
code_t* abc_strictequals(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xac);
    return self;
}
code_t* abc_subtract(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xa1);
    return self;
}
code_t* abc_subtract_i(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xc6);
    return self;
}
code_t* abc_swap(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x2b);
    return self;
}
code_t* abc_throw(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x03);
    return self;
}
code_t* abc_typeof(code_t*prev)
{
    code_t*self = add_opcode(prev, 0x95);
    return self;
}
code_t* abc_urshift(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xa7);
    return self;
}
code_t* abc_applytype(code_t*prev, int v)
{
    code_t*self = add_opcode(prev, 0x53);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
code_t* abc___pushpackage__(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0xfb);
    self->data[0] = string_new4(name);
    return self;
}
code_t* abc___pushpackage__2(code_t*prev, string_t* s)
{
    code_t*self = add_opcode(prev, 0xfb);
    self->data[0] = string_dup3(s);
    return self;
}
code_t* abc___rethrow__(code_t*prev)
{
    code_t*self = add_opcode(prev, 0xfc);
    return self;
}
code_t* abc___fallthrough__(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0xfd);
    self->data[0] = string_new4(name);
    return self;
}
code_t* abc___fallthrough__2(code_t*prev, string_t* s)
{
    code_t*self = add_opcode(prev, 0xfd);
    self->data[0] = string_dup3(s);
    return self;
}
code_t* abc___continue__(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0xfe);
    self->data[0] = string_new4(name);
    return self;
}
code_t* abc___continue__2(code_t*prev, string_t* s)
{
    code_t*self = add_opcode(prev, 0xfe);
    self->data[0] = string_dup3(s);
    return self;
}
code_t* abc___break__(code_t*prev, char* name)
{
    code_t*self = add_opcode(prev, 0xff);
    self->data[0] = string_new4(name);
    return self;
}
code_t* abc___break__2(code_t*prev, string_t* s)
{
    code_t*self = add_opcode(prev, 0xff);
    self->data[0] = string_dup3(s);
    return self;
}
