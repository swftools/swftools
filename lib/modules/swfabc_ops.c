void abc_add(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xa0);
}
#define add() abc_add(abc_code)
void abc_add_i(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xc5);
}
#define add_i() abc_add_i(abc_code)
void abc_atype(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x86);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define atype(name) abc_atype(abc_code,name)
void abc_astypelate(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x87);
}
#define astypelate() abc_astypelate(abc_code)
void abc_bitand(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xA8);
}
#define bitand() abc_bitand(abc_code)
void abc_bitnot(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x97);
}
#define bitnot() abc_bitnot(abc_code)
void abc_bitor(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xa9);
}
#define bitor() abc_bitor(abc_code)
void abc_bitxor(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xaa);
}
#define bitxor() abc_bitxor(abc_code)
void abc_call(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x41);
    swf_SetU30(c->tag, v);
}
#define call(v) abc_call(abc_code,v)
void abc_callmethod(abc_method_body_t*c, abc_method_body_t* m, int v)
{
    swf_SetU8(c->tag, 0x43);
    swf_SetU30(c->tag, m->index);
    swf_SetU30(c->tag, v);
}
#define callmethod(m,v) abc_callmethod(abc_code,m,v)
void abc_callproplex(abc_method_body_t*c, char* name, int v)
{
    swf_SetU8(c->tag, 0x4c);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
    swf_SetU30(c->tag, v);
}
#define callproplex(name,v) abc_callproplex(abc_code,name,v)
void abc_callproperty(abc_method_body_t*c, char* name, int v)
{
    swf_SetU8(c->tag, 0x46);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
    swf_SetU30(c->tag, v);
}
#define callproperty(name,v) abc_callproperty(abc_code,name,v)
void abc_callpropvoid(abc_method_body_t*c, char* name, int v)
{
    swf_SetU8(c->tag, 0x4f);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
    swf_SetU30(c->tag, v);
}
#define callpropvoid(name,v) abc_callpropvoid(abc_code,name,v)
void abc_callstatic(abc_method_body_t*c, abc_method_t* m, int v)
{
    swf_SetU8(c->tag, 0x44);
    swf_SetU30(c->tag, m->index);
    swf_SetU30(c->tag, v);
}
#define callstatic(m,v) abc_callstatic(abc_code,m,v)
void abc_callsuper(abc_method_body_t*c, char* name, int v)
{
    swf_SetU8(c->tag, 0x45);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
    swf_SetU30(c->tag, v);
}
#define callsuper(name,v) abc_callsuper(abc_code,name,v)
void abc_callsupervoid(abc_method_body_t*c, char* name, int v)
{
    swf_SetU8(c->tag, 0x4e);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
    swf_SetU30(c->tag, v);
}
#define callsupervoid(name,v) abc_callsupervoid(abc_code,name,v)
void abc_checkfilter(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x78);
}
#define checkfilter() abc_checkfilter(abc_code)
void abc_coerce(abc_method_body_t*c, abc_method_body_t* m)
{
    swf_SetU8(c->tag, 0x80);
    swf_SetU30(c->tag, m->index);
}
#define coerce(m) abc_coerce(abc_code,m)
void abc_coerce_a(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x82);
}
#define coerce_a() abc_coerce_a(abc_code)
void abc_coerce_s(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x85);
}
#define coerce_s() abc_coerce_s(abc_code)
void abc_construct(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x42);
    swf_SetU30(c->tag, v);
}
#define construct(v) abc_construct(abc_code,v)
void abc_constructprop(abc_method_body_t*c, char* name, int v)
{
    swf_SetU8(c->tag, 0x4a);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
    swf_SetU30(c->tag, v);
}
#define constructprop(name,v) abc_constructprop(abc_code,name,v)
void abc_constructsuper(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x49);
    swf_SetU30(c->tag, v);
}
#define constructsuper(v) abc_constructsuper(abc_code,v)
void abc_convert_b(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x76);
}
#define convert_b() abc_convert_b(abc_code)
void abc_convert_i(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x73);
}
#define convert_i() abc_convert_i(abc_code)
void abc_convert_d(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x75);
}
#define convert_d() abc_convert_d(abc_code)
void abc_convert_o(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x77);
}
#define convert_o() abc_convert_o(abc_code)
void abc_convert_u(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x74);
}
#define convert_u() abc_convert_u(abc_code)
void abc_convert_s(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x70);
}
#define convert_s() abc_convert_s(abc_code)
void abc_debug(abc_method_body_t*c, int v, char* s, int v2, int v3)
{
    swf_SetU8(c->tag, 0xef);
    swf_SetU8(c->tag, v);
    swf_SetU30(c->tag, dict_update(c->pool->strings, s, 0));
    swf_SetU8(c->tag, v2);
    swf_SetU30(c->tag, v3);
}
#define debug(v,s,v2,v3) abc_debug(abc_code,v,s,v2,v3)
void abc_debugfile(abc_method_body_t*c, char* s)
{
    swf_SetU8(c->tag, 0xf1);
    swf_SetU30(c->tag, dict_update(c->pool->strings, s, 0));
}
#define debugfile(s) abc_debugfile(abc_code,s)
void abc_debugline(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0xf0);
    swf_SetU30(c->tag, v);
}
#define debugline(v) abc_debugline(abc_code,v)
void abc_declocal(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x94);
    swf_SetU30(c->tag, v);
}
#define declocal(v) abc_declocal(abc_code,v)
void abc_declocal_i(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0xc3);
    swf_SetU30(c->tag, v);
}
#define declocal_i(v) abc_declocal_i(abc_code,v)
void abc_decrement(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x93);
}
#define decrement() abc_decrement(abc_code)
void abc_decrement_i(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xc1);
}
#define decrement_i() abc_decrement_i(abc_code)
void abc_deleteproperty(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x6a);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define deleteproperty(name) abc_deleteproperty(abc_code,name)
void abc_divide(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xa3);
}
#define divide() abc_divide(abc_code)
void abc_dup(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x2a);
}
#define dup() abc_dup(abc_code)
void abc_dxns(abc_method_body_t*c, char* s)
{
    swf_SetU8(c->tag, 0x06);
    swf_SetU30(c->tag, dict_update(c->pool->strings, s, 0));
}
#define dxns(s) abc_dxns(abc_code,s)
void abc_dxnslate(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x07);
}
#define dxnslate() abc_dxnslate(abc_code)
void abc_equals(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xab);
}
#define equals() abc_equals(abc_code)
void abc_esc_xattr(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x72);
}
#define esc_xattr() abc_esc_xattr(abc_code)
void abc_esc_xelem(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x71);
}
#define esc_xelem() abc_esc_xelem(abc_code)
void abc_findproperty(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x5e);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define findproperty(name) abc_findproperty(abc_code,name)
void abc_findpropstrict(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x5d);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define findpropstrict(name) abc_findpropstrict(abc_code,name)
void abc_getdescendants(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x59);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define getdescendants(name) abc_getdescendants(abc_code,name)
void abc_getglobalscope(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x64);
}
#define getglobalscope() abc_getglobalscope(abc_code)
void abc_getglobalslot(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x6e);
    swf_SetU30(c->tag, v);
}
#define getglobalslot(v) abc_getglobalslot(abc_code,v)
void abc_getlex(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x60);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define getlex(name) abc_getlex(abc_code,name)
void abc_getlocal(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x62);
    swf_SetU30(c->tag, v);
}
#define getlocal(v) abc_getlocal(abc_code,v)
void abc_getlocal_0(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xd0);
}
#define getlocal_0() abc_getlocal_0(abc_code)
void abc_getlocal_1(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xd1);
}
#define getlocal_1() abc_getlocal_1(abc_code)
void abc_getlocal_2(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xd2);
}
#define getlocal_2() abc_getlocal_2(abc_code)
void abc_getlocal_3(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xd3);
}
#define getlocal_3() abc_getlocal_3(abc_code)
void abc_getproperty(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x66);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define getproperty(name) abc_getproperty(abc_code,name)
void abc_getscopeobject(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x65);
    swf_SetU30(c->tag, v);
}
#define getscopeobject(v) abc_getscopeobject(abc_code,v)
void abc_getslot(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x6c);
    swf_SetU30(c->tag, v);
}
#define getslot(v) abc_getslot(abc_code,v)
void abc_getsuper(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x04);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define getsuper(name) abc_getsuper(abc_code,name)
void abc_greaterequals(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xaf);
}
#define greaterequals() abc_greaterequals(abc_code)
void abc_hasnext(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x1f);
}
#define hasnext() abc_hasnext(abc_code)
void abc_hasnext2(abc_method_body_t*c, int v, int v2)
{
    swf_SetU8(c->tag, 0x32);
    swf_SetU30(c->tag, v);
    swf_SetU30(c->tag, v2);
}
#define hasnext2(v,v2) abc_hasnext2(abc_code,v,v2)
void abc_ifeq(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x13);
    /* FIXME: write label j */
}
#define ifeq(j) abc_ifeq(abc_code,j)
void abc_iffalse(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x12);
    /* FIXME: write label j */
}
#define iffalse(j) abc_iffalse(abc_code,j)
void abc_ifge(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x18);
    /* FIXME: write label j */
}
#define ifge(j) abc_ifge(abc_code,j)
void abc_ifgt(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x17);
    /* FIXME: write label j */
}
#define ifgt(j) abc_ifgt(abc_code,j)
void abc_ifle(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x16);
    /* FIXME: write label j */
}
#define ifle(j) abc_ifle(abc_code,j)
void abc_iflt(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x15);
    /* FIXME: write label j */
}
#define iflt(j) abc_iflt(abc_code,j)
void abc_ifnge(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x0f);
    /* FIXME: write label j */
}
#define ifnge(j) abc_ifnge(abc_code,j)
void abc_ifngt(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x0e);
    /* FIXME: write label j */
}
#define ifngt(j) abc_ifngt(abc_code,j)
void abc_ifnle(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x0d);
    /* FIXME: write label j */
}
#define ifnle(j) abc_ifnle(abc_code,j)
void abc_ifnlt(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x0c);
    /* FIXME: write label j */
}
#define ifnlt(j) abc_ifnlt(abc_code,j)
void abc_ifne(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x14);
    /* FIXME: write label j */
}
#define ifne(j) abc_ifne(abc_code,j)
void abc_ifstricteq(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x19);
    /* FIXME: write label j */
}
#define ifstricteq(j) abc_ifstricteq(abc_code,j)
void abc_ifstrictne(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x1a);
    /* FIXME: write label j */
}
#define ifstrictne(j) abc_ifstrictne(abc_code,j)
void abc_iftrue(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x11);
    /* FIXME: write label j */
}
#define iftrue(j) abc_iftrue(abc_code,j)
void abc_in(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xb4);
}
#define in() abc_in(abc_code)
void abc_inclocal(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x92);
    swf_SetU30(c->tag, v);
}
#define inclocal(v) abc_inclocal(abc_code,v)
void abc_inclocal_i(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0xc2);
    swf_SetU30(c->tag, v);
}
#define inclocal_i(v) abc_inclocal_i(abc_code,v)
void abc_increment(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x91);
}
#define increment() abc_increment(abc_code)
void abc_increment_i(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xc0);
}
#define increment_i() abc_increment_i(abc_code)
void abc_initproperty(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x68);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define initproperty(name) abc_initproperty(abc_code,name)
void abc_instanceof(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xb1);
}
#define instanceof() abc_instanceof(abc_code)
void abc_istype(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0xb2);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define istype(name) abc_istype(abc_code,name)
void abc_istypelate(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xb3);
}
#define istypelate() abc_istypelate(abc_code)
void abc_jump(abc_method_body_t*c, abc_label_t* j)
{
    swf_SetU8(c->tag, 0x10);
    /* FIXME: write label j */
}
#define jump(j) abc_jump(abc_code,j)
void abc_kill(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x08);
    swf_SetU30(c->tag, v);
}
#define kill(v) abc_kill(abc_code,v)
void abc_label(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x09);
}
#define label() abc_label(abc_code)
void abc_lessequals(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xae);
}
#define lessequals() abc_lessequals(abc_code)
void abc_lessthan(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xad);
}
#define lessthan() abc_lessthan(abc_code)
void abc_lookupswitch(abc_method_body_t*c, void* labels)
{
    swf_SetU8(c->tag, 0x1b);
    /* FIXME: write labels labels */
}
#define lookupswitch(labels) abc_lookupswitch(abc_code,labels)
void abc_lshift(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xa5);
}
#define lshift() abc_lshift(abc_code)
void abc_modulo(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xa4);
}
#define modulo() abc_modulo(abc_code)
void abc_multiply(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xa2);
}
#define multiply() abc_multiply(abc_code)
void abc_multiply_i(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xc7);
}
#define multiply_i() abc_multiply_i(abc_code)
void abc_negate(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x90);
}
#define negate() abc_negate(abc_code)
void abc_negate_i(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xc4);
}
#define negate_i() abc_negate_i(abc_code)
void abc_newactivation(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x57);
}
#define newactivation() abc_newactivation(abc_code)
void abc_newarray(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x56);
    swf_SetU30(c->tag, v);
}
#define newarray(v) abc_newarray(abc_code,v)
void abc_newcatch(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x5a);
    swf_SetU30(c->tag, v);
}
#define newcatch(v) abc_newcatch(abc_code,v)
void abc_newclass(abc_method_body_t*c, abc_class_t* m)
{
    swf_SetU8(c->tag, 0x58);
    swf_SetU30(c->tag, m->index);
}
#define newclass(m) abc_newclass(abc_code,m)
void abc_newfunction(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x40);
    swf_SetU30(c->tag, v);
}
#define newfunction(v) abc_newfunction(abc_code,v)
void abc_newobject(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x55);
    swf_SetU30(c->tag, v);
}
#define newobject(v) abc_newobject(abc_code,v)
void abc_nextname(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x1e);
}
#define nextname() abc_nextname(abc_code)
void abc_nextvalue(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x23);
}
#define nextvalue() abc_nextvalue(abc_code)
void abc_nop(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x02);
}
#define nop() abc_nop(abc_code)
void abc_not(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x96);
}
#define not() abc_not(abc_code)
void abc_pop(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x29);
}
#define pop() abc_pop(abc_code)
void abc_popscope(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x1d);
}
#define popscope() abc_popscope(abc_code)
void abc_pushbyte(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x24);
    swf_SetU8(c->tag, v);
}
#define pushbyte(v) abc_pushbyte(abc_code,v)
void abc_pushdouble(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x2f);
    swf_SetU30(c->tag, v);
}
#define pushdouble(v) abc_pushdouble(abc_code,v)
void abc_pushfalse(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x27);
}
#define pushfalse() abc_pushfalse(abc_code)
void abc_pushint(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x2d);
    swf_SetU30(c->tag, v);
}
#define pushint(v) abc_pushint(abc_code,v)
void abc_pushnamespace(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x31);
    swf_SetU30(c->tag, v);
}
#define pushnamespace(v) abc_pushnamespace(abc_code,v)
void abc_pushnan(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x28);
}
#define pushnan() abc_pushnan(abc_code)
void abc_pushnull(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x20);
}
#define pushnull() abc_pushnull(abc_code)
void abc_pushscope(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x30);
}
#define pushscope() abc_pushscope(abc_code)
void abc_pushshort(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x25);
    swf_SetU30(c->tag, v);
}
#define pushshort(v) abc_pushshort(abc_code,v)
void abc_pushstring(abc_method_body_t*c, char* s)
{
    swf_SetU8(c->tag, 0x2c);
    swf_SetU30(c->tag, dict_update(c->pool->strings, s, 0));
}
#define pushstring(s) abc_pushstring(abc_code,s)
void abc_pushtrue(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x26);
}
#define pushtrue() abc_pushtrue(abc_code)
void abc_pushuint(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x2e);
    swf_SetU30(c->tag, v);
}
#define pushuint(v) abc_pushuint(abc_code,v)
void abc_pushundefined(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x21);
}
#define pushundefined() abc_pushundefined(abc_code)
void abc_pushwith(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x1c);
}
#define pushwith() abc_pushwith(abc_code)
void abc_returnvalue(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x48);
}
#define returnvalue() abc_returnvalue(abc_code)
void abc_returnvoid(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x47);
}
#define returnvoid() abc_returnvoid(abc_code)
void abc_rshift(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xa6);
}
#define rshift() abc_rshift(abc_code)
void abc_setlocal(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x63);
    swf_SetU30(c->tag, v);
}
#define setlocal(v) abc_setlocal(abc_code,v)
void abc_setlocal_0(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xd4);
}
#define setlocal_0() abc_setlocal_0(abc_code)
void abc_setlocal_1(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xd5);
}
#define setlocal_1() abc_setlocal_1(abc_code)
void abc_setlocal_2(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xd6);
}
#define setlocal_2() abc_setlocal_2(abc_code)
void abc_setlocal_3(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xd7);
}
#define setlocal_3() abc_setlocal_3(abc_code)
void abc_setglobalshot(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x6f);
    swf_SetU30(c->tag, v);
}
#define setglobalshot(v) abc_setglobalshot(abc_code,v)
void abc_setproperty(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x61);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define setproperty(name) abc_setproperty(abc_code,name)
void abc_setslot(abc_method_body_t*c, int v)
{
    swf_SetU8(c->tag, 0x6d);
    swf_SetU30(c->tag, v);
}
#define setslot(v) abc_setslot(abc_code,v)
void abc_setsuper(abc_method_body_t*c, char* name)
{
    swf_SetU8(c->tag, 0x05);
    swf_SetU30(c->tag, multiname_index(c->pool, name));
}
#define setsuper(name) abc_setsuper(abc_code,name)
void abc_strictequals(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xac);
}
#define strictequals() abc_strictequals(abc_code)
void abc_subtract(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xa1);
}
#define subtract() abc_subtract(abc_code)
void abc_subtract_i(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xc6);
}
#define subtract_i() abc_subtract_i(abc_code)
void abc_swap(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x2b);
}
#define swap() abc_swap(abc_code)
void abc_throw(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x03);
}
#define throw() abc_throw(abc_code)
void abc_typeof(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0x95);
}
#define typeof() abc_typeof(abc_code)
void abc_urshift(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xa7);
}
#define urshift() abc_urshift(abc_code)
void abc_xxx(abc_method_body_t*c)
{
    swf_SetU8(c->tag, 0xb0);
}
#define xxx() abc_xxx(abc_code)
