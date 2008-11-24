#include "opcodes.h"
abc_code_t* abc_add(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xa0);
    return self;
}
abc_code_t* abc_add_i(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xc5);
    return self;
}
abc_code_t* abc_astype(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x86);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_astypelate(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x87);
    return self;
}
abc_code_t* abc_bitand(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xA8);
    return self;
}
abc_code_t* abc_bitnot(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x97);
    return self;
}
abc_code_t* abc_bitor(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xa9);
    return self;
}
abc_code_t* abc_bitxor(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xaa);
    return self;
}
abc_code_t* abc_call(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x41);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_callmethod(abc_code_t*prev, abc_method_body_t* m, int v)
{
    abc_code_t*self = add_opcode(prev, 0x43);
    self->data[0] = m;
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_callproplex(abc_code_t*prev, char* name, int v)
{
    abc_code_t*self = add_opcode(prev, 0x4c);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_callproperty(abc_code_t*prev, char* name, int v)
{
    abc_code_t*self = add_opcode(prev, 0x46);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_callpropvoid(abc_code_t*prev, char* name, int v)
{
    abc_code_t*self = add_opcode(prev, 0x4f);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_callstatic(abc_code_t*prev, abc_method_t* m, int v)
{
    abc_code_t*self = add_opcode(prev, 0x44);
    self->data[0] = m;
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_callsuper(abc_code_t*prev, char* name, int v)
{
    abc_code_t*self = add_opcode(prev, 0x45);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_callsupervoid(abc_code_t*prev, char* name, int v)
{
    abc_code_t*self = add_opcode(prev, 0x4e);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_checkfilter(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x78);
    return self;
}
abc_code_t* abc_coerce(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x80);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_coerce_a(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x82);
    return self;
}
abc_code_t* abc_coerce_s(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x85);
    return self;
}
abc_code_t* abc_construct(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x42);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_constructprop(abc_code_t*prev, char* name, int v)
{
    abc_code_t*self = add_opcode(prev, 0x4a);
    self->data[0] = multiname_fromstring(name);
    self->data[1] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_constructsuper(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x49);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_convert_b(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x76);
    return self;
}
abc_code_t* abc_convert_i(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x73);
    return self;
}
abc_code_t* abc_convert_d(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x75);
    return self;
}
abc_code_t* abc_convert_o(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x77);
    return self;
}
abc_code_t* abc_convert_u(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x74);
    return self;
}
abc_code_t* abc_convert_s(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x70);
    return self;
}
abc_code_t* abc_debug(abc_code_t*prev, void* debuginfo)
{
    abc_code_t*self = add_opcode(prev, 0xef);
    /* FIXME: write debuginfo debuginfo */
    return self;
}
abc_code_t* abc_debugfile(abc_code_t*prev, char* s)
{
    abc_code_t*self = add_opcode(prev, 0xf1);
    self->data[0] = strdup(s);
    return self;
}
abc_code_t* abc_debugline(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0xf0);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_declocal(abc_code_t*prev, int reg)
{
    abc_code_t*self = add_opcode(prev, 0x94);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
abc_code_t* abc_declocal_i(abc_code_t*prev, int reg)
{
    abc_code_t*self = add_opcode(prev, 0xc3);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
abc_code_t* abc_decrement(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x93);
    return self;
}
abc_code_t* abc_decrement_i(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xc1);
    return self;
}
abc_code_t* abc_deleteproperty(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x6a);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_divide(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xa3);
    return self;
}
abc_code_t* abc_dup(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x2a);
    return self;
}
abc_code_t* abc_dxns(abc_code_t*prev, char* s)
{
    abc_code_t*self = add_opcode(prev, 0x06);
    self->data[0] = strdup(s);
    return self;
}
abc_code_t* abc_dxnslate(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x07);
    return self;
}
abc_code_t* abc_equals(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xab);
    return self;
}
abc_code_t* abc_esc_xattr(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x72);
    return self;
}
abc_code_t* abc_esc_xelem(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x71);
    return self;
}
abc_code_t* abc_findproperty(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x5e);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_findpropstrict(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x5d);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_getdescendants(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x59);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_getglobalscope(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x64);
    return self;
}
abc_code_t* abc_getglobalslot(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x6e);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_getlex(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x60);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_getlocal(abc_code_t*prev, int reg)
{
    abc_code_t*self = add_opcode(prev, 0x62);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
abc_code_t* abc_getlocal_0(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xd0);
    return self;
}
abc_code_t* abc_getlocal_1(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xd1);
    return self;
}
abc_code_t* abc_getlocal_2(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xd2);
    return self;
}
abc_code_t* abc_getlocal_3(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xd3);
    return self;
}
abc_code_t* abc_getproperty(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x66);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_getscopeobject(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x65);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_getslot(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x6c);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_getsuper(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x04);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_greaterequals(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xaf);
    return self;
}
abc_code_t* abc_hasnext(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x1f);
    return self;
}
abc_code_t* abc_hasnext2(abc_code_t*prev, int reg, int reg2)
{
    abc_code_t*self = add_opcode(prev, 0x32);
    self->data[0] = (void*)(ptroff_t)reg;
    self->data[1] = (void*)(ptroff_t)reg2;
    return self;
}
abc_code_t* abc_ifeq(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x13);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_iffalse(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x12);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifge(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x18);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifgt(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x17);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifle(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x16);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_iflt(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x15);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifnge(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x0f);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifngt(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x0e);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifnle(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x0d);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifnlt(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x0c);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifne(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x14);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifstricteq(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x19);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_ifstrictne(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x1a);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_iftrue(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x11);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_in(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xb4);
    return self;
}
abc_code_t* abc_inclocal(abc_code_t*prev, int reg)
{
    abc_code_t*self = add_opcode(prev, 0x92);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
abc_code_t* abc_inclocal_i(abc_code_t*prev, int reg)
{
    abc_code_t*self = add_opcode(prev, 0xc2);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
abc_code_t* abc_increment(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x91);
    return self;
}
abc_code_t* abc_increment_i(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xc0);
    return self;
}
abc_code_t* abc_initproperty(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x68);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_instanceof(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xb1);
    return self;
}
abc_code_t* abc_istype(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0xb2);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_istypelate(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xb3);
    return self;
}
abc_code_t* abc_jump(abc_code_t*prev, abc_code_t* label)
{
    abc_code_t*self = add_opcode(prev, 0x10);
    self->data[0] = label;
    return self;
}
abc_code_t* abc_kill(abc_code_t*prev, int reg)
{
    abc_code_t*self = add_opcode(prev, 0x08);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
abc_code_t* abc_label(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x09);
    return self;
}
abc_code_t* abc_lessequals(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xae);
    return self;
}
abc_code_t* abc_lessthan(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xad);
    return self;
}
abc_code_t* abc_lookupswitch(abc_code_t*prev, void* labels)
{
    abc_code_t*self = add_opcode(prev, 0x1b);
    /* FIXME: write labels labels */
    return self;
}
abc_code_t* abc_lshift(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xa5);
    return self;
}
abc_code_t* abc_modulo(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xa4);
    return self;
}
abc_code_t* abc_multiply(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xa2);
    return self;
}
abc_code_t* abc_multiply_i(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xc7);
    return self;
}
abc_code_t* abc_negate(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x90);
    return self;
}
abc_code_t* abc_negate_i(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xc4);
    return self;
}
abc_code_t* abc_newactivation(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x57);
    return self;
}
abc_code_t* abc_newarray(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x56);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_newcatch(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x5a);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_newclass(abc_code_t*prev, abc_class_t* m)
{
    abc_code_t*self = add_opcode(prev, 0x58);
    self->data[0] = m;
    return self;
}
abc_code_t* abc_newfunction(abc_code_t*prev, abc_method_body_t* m)
{
    abc_code_t*self = add_opcode(prev, 0x40);
    self->data[0] = m;
    return self;
}
abc_code_t* abc_newobject(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x55);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_nextname(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x1e);
    return self;
}
abc_code_t* abc_nextvalue(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x23);
    return self;
}
abc_code_t* abc_nop(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x02);
    return self;
}
abc_code_t* abc_not(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x96);
    return self;
}
abc_code_t* abc_pop(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x29);
    return self;
}
abc_code_t* abc_popscope(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x1d);
    return self;
}
abc_code_t* abc_pushbyte(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x24);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_pushdouble(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x2f);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_pushfalse(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x27);
    return self;
}
abc_code_t* abc_pushint(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x2d);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_pushnamespace(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x31);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_pushnan(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x28);
    return self;
}
abc_code_t* abc_pushnull(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x20);
    return self;
}
abc_code_t* abc_pushscope(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x30);
    return self;
}
abc_code_t* abc_pushshort(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x25);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_pushstring(abc_code_t*prev, char* s)
{
    abc_code_t*self = add_opcode(prev, 0x2c);
    self->data[0] = strdup(s);
    return self;
}
abc_code_t* abc_pushtrue(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x26);
    return self;
}
abc_code_t* abc_pushuint(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x2e);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_pushundefined(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x21);
    return self;
}
abc_code_t* abc_pushwith(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x1c);
    return self;
}
abc_code_t* abc_returnvalue(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x48);
    return self;
}
abc_code_t* abc_returnvoid(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x47);
    return self;
}
abc_code_t* abc_rshift(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xa6);
    return self;
}
abc_code_t* abc_setlocal(abc_code_t*prev, int reg)
{
    abc_code_t*self = add_opcode(prev, 0x63);
    self->data[0] = (void*)(ptroff_t)reg;
    return self;
}
abc_code_t* abc_setlocal_0(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xd4);
    return self;
}
abc_code_t* abc_setlocal_1(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xd5);
    return self;
}
abc_code_t* abc_setlocal_2(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xd6);
    return self;
}
abc_code_t* abc_setlocal_3(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xd7);
    return self;
}
abc_code_t* abc_setglobalslot(abc_code_t*prev, int v)
{
    abc_code_t*self = add_opcode(prev, 0x6f);
    self->data[0] = (void*)(ptroff_t)v;
    return self;
}
abc_code_t* abc_setproperty(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x61);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_setslot(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x6d);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_setsuper(abc_code_t*prev, char* name)
{
    abc_code_t*self = add_opcode(prev, 0x05);
    self->data[0] = multiname_fromstring(name);
    return self;
}
abc_code_t* abc_strictequals(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xac);
    return self;
}
abc_code_t* abc_subtract(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xa1);
    return self;
}
abc_code_t* abc_subtract_i(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xc6);
    return self;
}
abc_code_t* abc_swap(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x2b);
    return self;
}
abc_code_t* abc_throw(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x03);
    return self;
}
abc_code_t* abc_typeof(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0x95);
    return self;
}
abc_code_t* abc_urshift(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xa7);
    return self;
}
abc_code_t* abc_xxx(abc_code_t*prev)
{
    abc_code_t*self = add_opcode(prev, 0xb0);
    return self;
}
