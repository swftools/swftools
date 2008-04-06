void abc_add(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xa0);
}
void abc_add_i(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xc5);
}
void abc_atype(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x86);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_astypelate(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x87);
}
void abc_bitand(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xA8);
}
void abc_bitnot(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x97);
}
void abc_bitor(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xa9);
}
void abc_bitxor(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xaa);
}
void abc_call(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x41);
    swf_SetU30(c->tag, v);
}
void abc_callmethod(abc_code_t*c, abc_code_t*m, int v)
{
    swf_SetU8(c->tag, 0x43);
    swf_SetU30(c->tag, m->index);
    swf_SetU30(c->tag, v);
}
void abc_callproplex(abc_code_t*c, char*name, int v)
{
    swf_SetU8(c->tag, 0x4c);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
    swf_SetU30(c->tag, v);
}
void abc_callproperty(abc_code_t*c, char*name, int v)
{
    swf_SetU8(c->tag, 0x46);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
    swf_SetU30(c->tag, v);
}
void abc_callpropvoid(abc_code_t*c, char*name, int v)
{
    swf_SetU8(c->tag, 0x4f);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
    swf_SetU30(c->tag, v);
}
void abc_callstatic(abc_code_t*c, abc_method_t*m, int v)
{
    swf_SetU8(c->tag, 0x44);
    swf_SetU30(c->tag, m->index);
    swf_SetU30(c->tag, v);
}
void abc_callsuper(abc_code_t*c, char*name, int v)
{
    swf_SetU8(c->tag, 0x45);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
    swf_SetU30(c->tag, v);
}
void abc_callsupervoid(abc_code_t*c, char*name, int v)
{
    swf_SetU8(c->tag, 0x4e);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
    swf_SetU30(c->tag, v);
}
void abc_checkfilter(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x78);
}
void abc_coerce(abc_code_t*c, abc_code_t*m)
{
    swf_SetU8(c->tag, 0x80);
    swf_SetU30(c->tag, m->index);
}
void abc_coerce_a(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x82);
}
void abc_coerce_s(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x85);
}
void abc_construct(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x42);
    swf_SetU30(c->tag, v);
}
void abc_constructprop(abc_code_t*c, char*name, int v)
{
    swf_SetU8(c->tag, 0x4a);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
    swf_SetU30(c->tag, v);
}
void abc_constructsuper(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x49);
    swf_SetU30(c->tag, v);
}
void abc_convert_b(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x76);
}
void abc_convert_i(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x73);
}
void abc_convert_d(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x75);
}
void abc_convert_o(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x77);
}
void abc_convert_u(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x74);
}
void abc_convert_s(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x70);
}
void abc_debug(abc_code_t*c, int v, char*s, int v3, int v2)
{
    swf_SetU8(c->tag, 0xef);
    swf_SetU8(c->tag, v);
    swf_SetU30(c->tag, dict_update(c->abc->strings, s, 0));
    swf_SetU8(c->tag, v3);
    swf_SetU30(c->tag, v2);
}
void abc_debugfile(abc_code_t*c, char*s)
{
    swf_SetU8(c->tag, 0xf1);
    swf_SetU30(c->tag, dict_update(c->abc->strings, s, 0));
}
void abc_debugline(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0xf0);
    swf_SetU30(c->tag, v);
}
void abc_declocal(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x94);
    swf_SetU30(c->tag, v);
}
void abc_declocal_i(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0xc3);
    swf_SetU30(c->tag, v);
}
void abc_decrement(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x93);
}
void abc_decrement_i(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xc1);
}
void abc_deleteproperty(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x6a);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_divide(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xa3);
}
void abc_dup(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x2a);
}
void abc_dxns(abc_code_t*c, char*s)
{
    swf_SetU8(c->tag, 0x06);
    swf_SetU30(c->tag, dict_update(c->abc->strings, s, 0));
}
void abc_dxnslate(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x07);
}
void abc_equals(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xab);
}
void abc_esc_xattr(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x72);
}
void abc_esc_xelem(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x71);
}
void abc_findproperty(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x5e);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_findpropstrict(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x5d);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_getdescendants(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x59);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_getglobalscope(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x64);
}
void abc_getglobalslot(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x6e);
    swf_SetU30(c->tag, v);
}
void abc_getlex(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x60);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_getlocal(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x62);
    swf_SetU30(c->tag, v);
}
void abc_getlocal_0(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xd0);
}
void abc_getlocal_1(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xd1);
}
void abc_getlocal_2(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xd2);
}
void abc_getlocal_3(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xd3);
}
void abc_getproperty(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x66);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_getscopeobject(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x65);
    swf_SetU30(c->tag, v);
}
void abc_getslot(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x6c);
    swf_SetU30(c->tag, v);
}
void abc_getsuper(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x04);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_greaterequals(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xaf);
}
void abc_hasnext(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x1f);
}
void abc_hasnext2(abc_code_t*c, int v, int v2)
{
    swf_SetU8(c->tag, 0x32);
    swf_SetU30(c->tag, v);
    swf_SetU30(c->tag, v2);
}
void abc_ifeq(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x13);
}
void abc_iffalse(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x12);
}
void abc_ifge(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x18);
}
void abc_ifgt(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x17);
}
void abc_ifle(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x16);
}
void abc_iflt(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x15);
}
void abc_ifnge(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x0f);
}
void abc_ifngt(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x0e);
}
void abc_ifnle(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x0d);
}
void abc_ifnlt(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x0c);
}
void abc_ifne(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x14);
}
void abc_ifstricteq(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x19);
}
void abc_ifstrictne(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x1a);
}
void abc_iftrue(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x11);
}
void abc_in(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xb4);
}
void abc_inclocal(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x92);
    swf_SetU30(c->tag, v);
}
void abc_inclocal_i(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0xc2);
    swf_SetU30(c->tag, v);
}
void abc_increment(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x91);
}
void abc_increment_i(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xc0);
}
void abc_initproperty(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x68);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_instanceof(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xb1);
}
void abc_istype(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0xb2);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_istypelate(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xb3);
}
void abc_jump(abc_code_t*c, int target)
{
    swf_SetU8(c->tag, 0x10);
}
void abc_kill(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x08);
    swf_SetU30(c->tag, v);
}
void abc_label(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x09);
}
void abc_lessequals(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xae);
}
void abc_lessthan(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xad);
}
void abc_lshift(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xa5);
}
void abc_modulo(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xa4);
}
void abc_multiply(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xa2);
}
void abc_multiply_i(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xc7);
}
void abc_negate(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x90);
}
void abc_negate_i(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xc4);
}
void abc_newactivation(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x57);
}
void abc_newarray(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x56);
    swf_SetU30(c->tag, v);
}
void abc_newcatch(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x5a);
    swf_SetU30(c->tag, v);
}
void abc_newclass(abc_code_t*c, abc_class_t*m)
{
    swf_SetU8(c->tag, 0x58);
    swf_SetU30(c->tag, m->index);
}
void abc_newfunction(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x40);
    swf_SetU30(c->tag, v);
}
void abc_newobject(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x55);
    swf_SetU30(c->tag, v);
}
void abc_nextname(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x1e);
}
void abc_nextvalue(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x23);
}
void abc_nop(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x02);
}
void abc_not(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x96);
}
void abc_pop(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x29);
}
void abc_popscope(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x1d);
}
void abc_pushbyte(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x24);
    swf_SetU8(c->tag, v);
}
void abc_pushdouble(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x2f);
    swf_SetU30(c->tag, v);
}
void abc_pushfalse(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x27);
}
void abc_pushint(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x2d);
    swf_SetU30(c->tag, v);
}
void abc_pushnamespace(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x31);
    swf_SetU30(c->tag, v);
}
void abc_pushnan(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x28);
}
void abc_pushnull(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x20);
}
void abc_pushscope(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x30);
}
void abc_pushshort(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x25);
    swf_SetU30(c->tag, v);
}
void abc_pushstring(abc_code_t*c, char*s)
{
    swf_SetU8(c->tag, 0x2c);
    swf_SetU30(c->tag, dict_update(c->abc->strings, s, 0));
}
void abc_pushtrue(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x26);
}
void abc_pushuint(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x2e);
    swf_SetU30(c->tag, v);
}
void abc_pushundefined(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x21);
}
void abc_pushwith(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x1c);
}
void abc_returnvalue(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x48);
}
void abc_returnvoid(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x47);
}
void abc_rshift(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xa6);
}
void abc_setlocal(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x63);
    swf_SetU30(c->tag, v);
}
void abc_setlocal_0(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xd4);
}
void abc_setlocal_1(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xd5);
}
void abc_setlocal_2(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xd6);
}
void abc_setlocal_3(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xd7);
}
void abc_setglobalshot(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x6f);
    swf_SetU30(c->tag, v);
}
void abc_setproperty(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x61);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_setslot(abc_code_t*c, int v)
{
    swf_SetU8(c->tag, 0x6d);
    swf_SetU30(c->tag, v);
}
void abc_setsuper(abc_code_t*c, char*name)
{
    swf_SetU8(c->tag, 0x05);
    swf_SetU30(c->tag, multiname_index(c->abc, name));
}
void abc_strictequals(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xac);
}
void abc_subtract(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xa1);
}
void abc_subtract_i(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xc6);
}
void abc_swap(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x2b);
}
void abc_throw(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x03);
}
void abc_typeof(abc_code_t*c)
{
    swf_SetU8(c->tag, 0x95);
}
void abc_urshift(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xa7);
}
void abc_xxx(abc_code_t*c)
{
    swf_SetU8(c->tag, 0xb0);
}
