/* expr.c

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2009 Matthias Kramm <kramm@quiss.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "expr.h"
#include "common.h"
#include "tokenizer.h"

#define IS_INT(a) (TYPE_IS_INT((a)) || TYPE_IS_UINT((a)))
#define BOTH_INT(a,b) (IS_INT(a) && IS_INT(b))

#define READ_HEADER_LEFTRIGHT \
    typedcode_t left = n->child[0]->type->read(n->child[0]);\
    typedcode_t right = n->child[1]->type->read(n->child[1]);\
    code_t*c=0;\
    classinfo_t*t=0;

#define READ_HEADER_ONE \
    typedcode_t x = n->child[0]->type->read(n->child[0]);\
    code_t*c=0;\
    classinfo_t*t=0;

#define EXEC_HEADER_ONE \
    code_t* x = n->child[0]->type->exec(n->child[0]);\
    code_t*c=0;\
    classinfo_t*t=0;

#define EXEC_HEADER_LEFTRIGHT \
    code_t* left = n->child[0]->type->exec(n->child[0]);\
    code_t* right = n->child[1]->type->exec(n->child[1]);\
    code_t*c=0;\
    classinfo_t*t=0;

#define EVAL_HEADER_LEFTRIGHT \
    constant_t left = n->child[0]->type->eval(n->child[0]);\
    constant_t right = n->child[1]->type->eval(n->child[1]);\
    constant_t r; \
    if(left.type==CONSTANT_UNKNOWN || right.type==CONSTANT_UNKNOWN) {\
        r.type = CONSTANT_UNKNOWN;return r; \
    }


#define RET \
    typedcode_t r; \
    r.c = c; \
    r.t = t; \
    return r;

static classinfo_t*join_types(classinfo_t*type1, classinfo_t*type2, nodetype_t*t)
{
    if(t == &node_plus) {
        if((TYPE_IS_XMLLIST(type1) || TYPE_IS_XML(type1)) &&
           (TYPE_IS_XMLLIST(type2) || TYPE_IS_XML(type2)))
            return TYPE_OBJECT;
        if(BOTH_INT(type1, type2))
            return TYPE_INT;
        if(IS_NUMBER_OR_INT(type1) && IS_NUMBER_OR_INT(type2))
            return TYPE_NUMBER;
        if(TYPE_IS_DATE(type1) || TYPE_IS_DATE(type2))
            return TYPE_OBJECT;
        if(TYPE_IS_STRING(type1) || TYPE_IS_STRING(type2)) {
            /* depending on where the strings come from, the result type
               of an "add" might be an object or a string, depending on the
               verifier's mood. So basically we just don't know the type. */
            return TYPE_VOID;
        }
        if(TYPE_IS_ANY(type1) || TYPE_IS_ANY(type2))
            return TYPE_ANY;
        return TYPE_OBJECT; // e.g. array+array = object
    }
    
    if(type1 == type2)
        return type1;
    return TYPE_ANY;
}
static int getlocalnr(code_t*c)
{
    if(c->opcode == OPCODE_GETLOCAL) {return (ptroff_t)c->data[0];}
    else if(c->opcode == OPCODE_GETLOCAL_0) {return 0;}
    else if(c->opcode == OPCODE_GETLOCAL_1) {return 1;}
    else if(c->opcode == OPCODE_GETLOCAL_2) {return 2;}
    else if(c->opcode == OPCODE_GETLOCAL_3) {return 3;}
    else syntaxerror("Internal error: opcode %02x is not a getlocal call", c->opcode);
    return 0;
}


int gettempvar(); /* FIXME: we should use a function pointer here */


static code_t* toreadwrite(code_t*in, code_t*middlepart, char justassign, char readbefore, char pushvalue)
{
    /* converts this:

       [prefix code] [read instruction]

       to this:

       [prefix code] ([dup]) [read instruction] [middlepart] [setvar] [write instruction] [getvar]
    */
    if(in && in->opcode == OPCODE_COERCE_A) {
        in = code_cutlast(in);
    }
    if(in->next)
        syntaxerror("internal error");

    /* chop off read instruction */
    code_t*prefix = in;
    code_t*r = in;
    if(r->prev) {
        prefix = r->prev;r->prev = 0;
        prefix->next=0;
    } else {
        prefix = 0;
    }
        
    char use_temp_var = readbefore;

    /* generate the write instruction, and maybe append a dup to the prefix code */
    code_t* write = abc_nop(0);
    if(r->opcode == OPCODE_GETPROPERTY) {
        write->opcode = OPCODE_SETPROPERTY;
        multiname_t*m = (multiname_t*)r->data[0];
        write->data[0] = multiname_clone(m);
        if(m->type == QNAME || m->type == MULTINAME ||
           m->type == QNAMEA || m->type == MULTINAMEA) {
            if(!justassign) {
                prefix = abc_dup(prefix); // we need the object, too
            }
            use_temp_var = 1;
        } else if(m->type == MULTINAMEL || m->type == MULTINAMELA ||
		  m->type == RTQNAME || m->type == RTQNAMEA) {
            if(!justassign) {
                /* dupping two values on the stack requires 5 operations and one register- 
                   couldn't adobe just have given us a dup2? */
                int temp = gettempvar();
                prefix = abc_setlocal(prefix, temp);
                prefix = abc_dup(prefix);
                prefix = abc_getlocal(prefix, temp);
                prefix = abc_swap(prefix);
                prefix = abc_getlocal(prefix, temp);
                if(!use_temp_var);
                    prefix = abc_kill(prefix, temp);
            }
            use_temp_var = 1;
        } else {
            syntaxerror("illegal lvalue: can't assign a value to this expression (not a qname/multiname)");
        }
    } else if(r->opcode == OPCODE_GETSLOT) {
        write->opcode = OPCODE_SETSLOT;
        write->data[0] = r->data[0];
        if(!justassign) {
            prefix = abc_dup(prefix); // we need the object, too
        }
        use_temp_var = 1;
    } else if(r->opcode == OPCODE_GETLOCAL) { 
        write->opcode = OPCODE_SETLOCAL;
        write->data[0] = r->data[0];
    } else if(r->opcode == OPCODE_GETLOCAL_0) { 
        write->opcode = OPCODE_SETLOCAL_0;
    } else if(r->opcode == OPCODE_GETLOCAL_1) { 
        write->opcode = OPCODE_SETLOCAL_1;
    } else if(r->opcode == OPCODE_GETLOCAL_2) { 
        write->opcode = OPCODE_SETLOCAL_2;
    } else if(r->opcode == OPCODE_GETLOCAL_3) { 
        write->opcode = OPCODE_SETLOCAL_3;
    } else if(r->opcode == OPCODE_GETSUPER) { 
        write->opcode = OPCODE_SETSUPER;
        multiname_t*m = (multiname_t*)r->data[0];
        write->data[0] = multiname_clone(m);
    } else {
        code_dump(r);
        syntaxerror("illegal lvalue: can't assign a value to this expression");
    }
    code_t* c = 0;
    
    int temp = -1;
    if(!justassign) {
        if(use_temp_var) {
            /* with getproperty/getslot, we have to be extra careful not
               to execute the read code twice, as it might have side-effects
               (e.g. if the property is in fact a setter/getter combination)

               So read the value, modify it, and write it again,
               using prefix only once and making sure (by using a temporary
               register) that the return value is what we just wrote */
            temp = gettempvar();
            c = code_append(c, prefix);
            c = code_append(c, r);
            if(pushvalue && readbefore) {
                c = abc_dup(c);
                c = abc_setlocal(c, temp);
            }
            c = code_append(c, middlepart);
            if(pushvalue && !readbefore) {
                c = abc_dup(c);
                c = abc_setlocal(c, temp);
            }
            c = code_append(c, write);
            if(pushvalue) {
                c = abc_getlocal(c, temp);
                c = abc_kill(c, temp);
            }
        } else {
            /* if we're allowed to execute the read code twice *and*
               the middlepart doesn't modify the code, things are easier.
            */
            //c = code_append(c, prefix);
            if(prefix) syntaxerror("internal error (prefix)");
            code_t* r2 = 0;
            if(pushvalue) {
                r2 = code_dup(r);
            }
            c = code_append(c, r);
            c = code_append(c, middlepart);
            c = code_append(c, write);
            if(pushvalue) {
                c = code_append(c, r2);
            }
        }
    } else {
        /* even smaller version: overwrite the value without reading
           it out first */
        if(!use_temp_var) {
            if(prefix) {
                c = code_append(c, prefix);
                c = abc_dup(c);
            }
            c = code_append(c, middlepart);
            c = code_append(c, write);
            if(pushvalue) {
                c = code_append(c, r);
            }
        } else {
            code_free(r);r=0;
            temp = gettempvar();
            if(prefix) {
                c = code_append(c, prefix);
            }
            c = code_append(c, middlepart);
            if(pushvalue) {
                c = abc_dup(c);
                c = abc_setlocal(c, temp);
            }
            c = code_append(c, write);
            if(pushvalue) {
                c = abc_getlocal(c, temp);
                c = abc_kill(c, temp);
            }
        }
    }
    return c;
}

typedcode_t push_constant(constant_t*v)
{
    typedcode_t t;
    switch(v->type) {
        case CONSTANT_INT: t.c = abc_pushint(0, v->i);t.t = TYPE_INT;break;
        case CONSTANT_UINT: t.c = abc_pushuint(0, v->u);t.t = TYPE_UINT;break;
        case CONSTANT_FLOAT: t.c = abc_pushdouble(0, v->f);t.t = TYPE_FLOAT;break;
        case CONSTANT_TRUE: t.c = abc_pushtrue(0);t.t = TYPE_BOOLEAN;break;
        case CONSTANT_FALSE: t.c = abc_pushfalse(0);t.t = TYPE_BOOLEAN;break;
        case CONSTANT_STRING: t.c = abc_pushstring2(0, v->s);t.t = TYPE_STRING;break;
        case CONSTANT_NULL: t.c = abc_pushnull(0);t.t = TYPE_NULL;break;
        case CONSTANT_UNDEFINED: t.c = abc_pushundefined(0);t.t = TYPE_ANY;break;
        case CONSTANT_NAMESPACE:
        case CONSTANT_NAMESPACE_PACKAGE:
        case CONSTANT_NAMESPACE_PACKAGEINTERNAL:
        case CONSTANT_NAMESPACE_PROTECTED:
        case CONSTANT_NAMESPACE_EXPLICIT:
        case CONSTANT_NAMESPACE_STATICPROTECTED:
        case CONSTANT_NAMESPACE_PRIVATE:
            t.c = abc_pushnamespace(0, v->ns);t.t = TYPE_NAMESPACE;break;
        default:
            syntaxerror("internal error: bad constant");
    }
    return t;
}

code_t*converttype(code_t*c, classinfo_t*from, classinfo_t*to);

int constant_to_int(constant_t*c) 
{
    switch(c->type) {
        case CONSTANT_INT:    return c->i;
        case CONSTANT_UINT:   return (int)c->u;
        case CONSTANT_FLOAT:  return c->f;
        case CONSTANT_TRUE:   return 1;
        case CONSTANT_FALSE:  return 0;
        case CONSTANT_STRING: return atoi(c->s->str);
        default: syntaxerror("can't convert this constant to an integer");
    }
}

double constant_to_float(constant_t*c) 
{
    switch(c->type) {
        case CONSTANT_INT:    return (double)c->i;
        case CONSTANT_UINT:   return (double)c->u;
        case CONSTANT_FLOAT:  return c->f;
        case CONSTANT_TRUE:   return (double)1;
        case CONSTANT_FALSE:  return (double)0;
        case CONSTANT_STRING: return atof(c->s->str);
        default: syntaxerror("can't convert this constant to a float");
    }
}

// -------------------------- x + y -----------------------------------

typedcode_t node_plus_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_plus_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c, right.c);
    if(BOTH_INT(left.t, right.t)) {
       c = abc_add_i(c);
       t = TYPE_INT;
    } else {
       c = abc_add(c);
       t = join_types(left.t,right.t,n->type);
    }
    RET;
}
code_t* node_plus_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_plus_eval(node_t*n)
{
    EVAL_HEADER_LEFTRIGHT;
    char left_int = left.type == CONSTANT_INT || left.type == CONSTANT_UINT;
    if(left_int && (right.type == CONSTANT_INT || right.type == CONSTANT_UINT)) { 
        int i = constant_to_int(&left) + constant_to_int(&right);
        r.type = CONSTANT_INT;
        r.i = i;
    } else if(left_int && right.type == CONSTANT_FLOAT) {
        double f = constant_to_int(&left) + constant_to_float(&right);
        r.type = CONSTANT_FLOAT;
        r.f = f;
    } else if(left.type == CONSTANT_STRING) {
        char*add = constant_tostring(&right);
        int l1 = left.s->len;
        int l2 = strlen(add);
        char*s = malloc(l1+l2+1);
        memcpy(s, left.s->str, l1);
        memcpy(s+l1, add, l2);
        s[l1+l2]=0;
        r.type = CONSTANT_STRING;
        r.s = malloc(sizeof(string_t));
        r.s->str = s;
        r.s->len = l1+l2;
        free(add);
    } else {
        r.type = CONSTANT_UNKNOWN;
    }
    return r;
}
nodetype_t node_plus =
{
name:"plus",
flags:NODE_HAS_CHILDREN,
eval:node_plus_eval,
write:node_plus_write,
read:node_plus_read,
exec:node_plus_exec,
};

// -------------------------- x - y -----------------------------------

typedcode_t node_minus_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_minus_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c, right.c);
    if(BOTH_INT(left.t,right.t)) {
       c = abc_subtract_i(c);
       t = TYPE_INT;
    } else {
       c = abc_subtract(c);
       t = TYPE_NUMBER;
    }
    RET;
}
code_t* node_minus_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_minus_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_minus =
{
name:"minus",
flags:NODE_HAS_CHILDREN,
eval: node_minus_eval,
write: node_minus_write,
read: node_minus_read,
exec: node_minus_exec
};

// ---------------------------- ++x -----------------------------------

typedcode_t node_lplusplus_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_lplusplus_read(node_t*n)
{
    READ_HEADER_ONE;
    t = x.t;
    if(is_getlocal(x.c) && (TYPE_IS_INT(x.t) || TYPE_IS_NUMBER(x.t))) {
        int nr = getlocalnr(x.c);
        code_free(x.c);x.c=0;
        if(TYPE_IS_INT(x.t)) {
           c = abc_inclocal_i(c, nr);
           c = abc_getlocal(c, nr);
        } else if(TYPE_IS_NUMBER(x.t)) {
           c = abc_inclocal(c, nr);
           c = abc_getlocal(c, nr);
        } else syntaxerror("internal error");
    } else {
        if(TYPE_IS_INT(x.t) || TYPE_IS_UINT(x.t)) {
            c = abc_increment_i(c);
            t = TYPE_INT;
        } else {
            c = abc_increment(c);
            t = TYPE_NUMBER;
        }
        c = converttype(c, t, x.t);
        c = toreadwrite(x.c, c, 0, 0, 1);
        t = x.t;
    }
    RET
}
code_t* node_lplusplus_exec(node_t*n)
{
    typedcode_t x = n->child[0]->type->read(n->child[0]);
    code_t*c=0;
    if(is_getlocal(x.c) && (TYPE_IS_INT(x.t) || TYPE_IS_NUMBER(x.t))) {
        int nr = getlocalnr(x.c);
        code_free(x.c);x.c=0;
        if(TYPE_IS_INT(x.t)) {
           c = abc_inclocal_i(c, nr);
        } else if(TYPE_IS_NUMBER(x.t)) {
           c = abc_inclocal(c, nr);
        } else syntaxerror("internal error");
    } else {
        classinfo_t*t;
        if(TYPE_IS_INT(x.t) || TYPE_IS_UINT(x.t)) {
            c = abc_increment_i(c);
            t = TYPE_INT;
        } else {
            c = abc_increment(c);
            t = TYPE_NUMBER;
        }
        c = converttype(c, t, x.t); //convert back to original type
        c = toreadwrite(x.c, c, 0, 0, 0);
        t = x.t;
    }
    return c;
}
constant_t node_lplusplus_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_lplusplus =
{
name: "lplusplus",
flags:NODE_HAS_CHILDREN,
eval: node_lplusplus_eval,
write: node_lplusplus_write,
read: node_lplusplus_read,
exec: node_lplusplus_exec
};


// ---------------------------- --x -----------------------------------

typedcode_t node_lminusminus_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_lminusminus_read(node_t*n)
{
    READ_HEADER_ONE;
    t = x.t;
    if(is_getlocal(x.c) && (TYPE_IS_INT(x.t) || TYPE_IS_NUMBER(x.t))) {
        int nr = getlocalnr(x.c);
        code_free(x.c);x.c=0;
        if(TYPE_IS_INT(x.t)) {
           c = abc_declocal_i(c, nr);
           c = abc_getlocal(c, nr);
        } else if(TYPE_IS_NUMBER(x.t)) {
           c = abc_declocal(c, nr);
           c = abc_getlocal(c, nr);
        } else syntaxerror("internal error");
    } else {
        if(TYPE_IS_INT(x.t) || TYPE_IS_UINT(x.t)) {
            c = abc_decrement_i(c);
            t = TYPE_INT;
        } else {
            c = abc_decrement(c);
            t = TYPE_NUMBER;
        }
        c = converttype(c, t, x.t);
        c = toreadwrite(x.c, c, 0, 0, 1);
        t = x.t;
    }
    RET
}
code_t* node_lminusminus_exec(node_t*n)
{
    typedcode_t x = n->child[0]->type->read(n->child[0]);
    code_t*c=0;
    if(is_getlocal(x.c) && (TYPE_IS_INT(x.t) || TYPE_IS_NUMBER(x.t))) {
        int nr = getlocalnr(x.c);
        code_free(x.c);x.c=0;
        if(TYPE_IS_INT(x.t)) {
           c = abc_declocal_i(c, nr);
        } else if(TYPE_IS_NUMBER(x.t)) {
           c = abc_declocal(c, nr);
        } else syntaxerror("internal error");
    } else {
        classinfo_t*t;
        if(TYPE_IS_INT(x.t) || TYPE_IS_UINT(x.t)) {
            c = abc_decrement_i(c);
            t = TYPE_INT;
        } else {
            c = abc_decrement(c);
            t = TYPE_NUMBER;
        }
        c = converttype(c, t, x.t); //convert back to original type
        c = toreadwrite(x.c, c, 0, 0, 0);
        t = x.t;
    }
    return c;
}
constant_t node_lminusminus_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_lminusminus =
{
name: "lminusminus",
flags:NODE_HAS_CHILDREN,
eval: node_lminusminus_eval,
write: node_lminusminus_write,
read: node_lminusminus_read,
exec: node_lminusminus_exec
};



// ---------------------------- x++ -----------------------------------

typedcode_t node_rplusplus_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_rplusplus_read(node_t*n)
{
    READ_HEADER_ONE;
    t = x.t;
    if(is_getlocal(x.c) && (TYPE_IS_INT(x.t) || TYPE_IS_NUMBER(x.t))) {
        int nr = getlocalnr(x.c);
        code_free(x.c);x.c=0;
        if(TYPE_IS_INT(x.t)) {
           c = abc_getlocal(0, nr);
           c = abc_inclocal_i(c, nr);
        } else if(TYPE_IS_NUMBER(x.t)) {
           c = abc_getlocal(0, nr);
           c = abc_inclocal(c, nr);
        } else syntaxerror("internal error");
    } else {
        if(TYPE_IS_INT(x.t) || TYPE_IS_UINT(x.t)) {
            c = abc_increment_i(c);
            t = TYPE_INT;
        } else {
            c = abc_increment(c);
            t = TYPE_NUMBER;
        }
        c = converttype(c, t, x.t);
        c = toreadwrite(x.c, c, 0, 1, 1);
        t = x.t;
    }
    RET
}
code_t* node_rplusplus_exec(node_t*n)
{
    typedcode_t x = n->child[0]->type->read(n->child[0]);
    code_t*c=0;
    if(is_getlocal(x.c) && (TYPE_IS_INT(x.t) || TYPE_IS_NUMBER(x.t))) {
        int nr = getlocalnr(x.c);
        code_free(x.c);x.c=0;
        if(TYPE_IS_INT(x.t)) {
           c = abc_inclocal_i(c, nr);
        } else if(TYPE_IS_NUMBER(x.t)) {
           c = abc_inclocal(c, nr);
        } else syntaxerror("internal error");
    } else {
        classinfo_t*t;
        if(TYPE_IS_INT(x.t) || TYPE_IS_UINT(x.t)) {
            c = abc_increment_i(c);
            t = TYPE_INT;
        } else {
            c = abc_increment(c);
            t = TYPE_NUMBER;
        }
        c = converttype(c, t, x.t); //convert back to original type
        c = toreadwrite(x.c, c, 0, 1, 0);
        t = x.t;
    }
    return c;
}
constant_t node_rplusplus_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_rplusplus =
{
name: "rplusplus",
flags:NODE_HAS_CHILDREN,
eval: node_rplusplus_eval,
write: node_rplusplus_write,
read: node_rplusplus_read,
exec: node_rplusplus_exec
};

// ---------------------------- x-- -----------------------------------

typedcode_t node_rminusminus_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_rminusminus_read(node_t*n)
{
    READ_HEADER_ONE;
    t = x.t;
    if(is_getlocal(x.c) && (TYPE_IS_INT(x.t) || TYPE_IS_NUMBER(x.t))) {
        int nr = getlocalnr(x.c);
        code_free(x.c);x.c=0;
        if(TYPE_IS_INT(x.t)) {
           c = abc_getlocal(0, nr);
           c = abc_declocal_i(c, nr);
        } else if(TYPE_IS_NUMBER(x.t)) {
           c = abc_getlocal(0, nr);
           c = abc_declocal(c, nr);
        } else syntaxerror("internal error");
    } else {
        if(TYPE_IS_INT(x.t) || TYPE_IS_UINT(x.t)) {
            c = abc_decrement_i(c);
            t = TYPE_INT;
        } else {
            c = abc_decrement(c);
            t = TYPE_NUMBER;
        }
        c = converttype(c, t, x.t);
        c = toreadwrite(x.c, c, 0, 1, 1);
        t = x.t;
    }
    RET
}
code_t* node_rminusminus_exec(node_t*n)
{
    typedcode_t x = n->child[0]->type->read(n->child[0]);
    code_t*c=0;
    if(is_getlocal(x.c) && (TYPE_IS_INT(x.t) || TYPE_IS_NUMBER(x.t))) {
        int nr = getlocalnr(x.c);
        code_free(x.c);x.c=0;
        if(TYPE_IS_INT(x.t)) {
           c = abc_declocal_i(c, nr);
        } else if(TYPE_IS_NUMBER(x.t)) {
           c = abc_declocal(c, nr);
        } else syntaxerror("internal error");
    } else {
        classinfo_t*t;
        if(TYPE_IS_INT(x.t) || TYPE_IS_UINT(x.t)) {
            c = abc_decrement_i(c);
            t = TYPE_INT;
        } else {
            c = abc_decrement(c);
            t = TYPE_NUMBER;
        }
        c = converttype(c, t, x.t); //convert back to original type
        c = toreadwrite(x.c, c, 0, 1, 0);
        t = x.t;
    }
    return c;
}
constant_t node_rminusminus_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_rminusminus =
{
name: "rminusminus",
flags:NODE_HAS_CHILDREN,
eval: node_rminusminus_eval,
write: node_rminusminus_write,
read: node_rminusminus_read,
exec: node_rminusminus_exec
};

// ---------------------------- x*y -----------------------------------

typedcode_t node_multiply_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_multiply_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    if(BOTH_INT(left.t,right.t)) {
       c = abc_multiply_i(c);
       t = TYPE_INT;
    } else {
       c = abc_multiply(c);
       t = TYPE_NUMBER;
    }
    RET
}
code_t* node_multiply_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_multiply_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_multiply =
{
name: "multiply",
flags:NODE_HAS_CHILDREN,
eval: node_multiply_eval,
write: node_multiply_write,
read: node_multiply_read,
exec: node_multiply_exec
};

// ---------------------------- x/y -----------------------------------

typedcode_t node_div_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_div_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c, right.c);
    c = abc_divide(c);
    t = TYPE_NUMBER;
    RET
}
code_t* node_div_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_div_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_div =
{
name: "div",
flags:NODE_HAS_CHILDREN,
eval: node_div_eval,
write: node_div_write,
read: node_div_read,
exec: node_div_exec
};

// ---------------------------- x%y -----------------------------------

typedcode_t node_mod_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_mod_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c, right.c);
    c = abc_modulo(c);
    t = TYPE_NUMBER;
    RET
}
code_t* node_mod_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_mod_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_mod =
{
name: "mod",
flags:NODE_HAS_CHILDREN,
eval: node_mod_eval,
write: node_mod_write,
read: node_mod_read,
exec: node_mod_exec
};

// ---------------------------- x<y -----------------------------------

typedcode_t node_lt_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_lt_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_lessthan(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_lt_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_lt_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_lt =
{
name: "lt",
flags:NODE_HAS_CHILDREN,
eval: node_lt_eval,
write: node_lt_write,
read: node_lt_read,
exec: node_lt_exec
};

// ---------------------------- x>y -----------------------------------

typedcode_t node_gt_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_gt_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_greaterthan(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_gt_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_gt_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_gt =
{
name: "gt",
flags:NODE_HAS_CHILDREN,
eval: node_gt_eval,
write: node_gt_write,
read: node_gt_read,
exec: node_gt_exec
};

// ---------------------------- x<=y ----------------------------------

typedcode_t node_le_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_le_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_lessequals(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_le_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_le_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_le = //<=
{
name: "le",
flags:NODE_HAS_CHILDREN,
eval: node_le_eval,
write: node_le_write,
read: node_le_read,
exec: node_le_exec
};

// ---------------------------- x>=y ----------------------------------

typedcode_t node_ge_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_ge_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_greaterequals(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_ge_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_ge_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_ge = //>=
{
name: "ge",
flags:NODE_HAS_CHILDREN,
eval: node_ge_eval,
write: node_ge_write,
read: node_ge_read,
exec: node_ge_exec
};

// ---------------------------- x==y ----------------------------------

typedcode_t node_eqeq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_eqeq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_equals(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_eqeq_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_eqeq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_eqeq = //==
{
name: "eqeq",
flags:NODE_HAS_CHILDREN,
eval: node_eqeq_eval,
write: node_eqeq_write,
read: node_eqeq_read,
exec: node_eqeq_exec
};

// --------------------------- x===y ----------------------------------

typedcode_t node_eqeqeq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_eqeqeq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_strictequals(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_eqeqeq_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_eqeqeq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_eqeqeq = //===
{
name: "eqeqeq",
flags:NODE_HAS_CHILDREN,
eval: node_eqeqeq_eval,
write: node_eqeqeq_write,
read: node_eqeqeq_read,
exec: node_eqeqeq_exec
};

// --------------------------- x!==y ----------------------------------

typedcode_t node_noteqeq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_noteqeq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_strictequals(c);
    c = abc_not(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_noteqeq_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_noteqeq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_noteqeq = //!==
{
name: "noteqeq",
flags:NODE_HAS_CHILDREN,
eval: node_noteqeq_eval,
write: node_noteqeq_write,
read: node_noteqeq_read,
exec: node_noteqeq_exec
};

// --------------------------- x!=y ----------------------------------

typedcode_t node_noteq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_noteq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_equals(c);
    c = abc_not(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_noteq_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_noteq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_noteq = //!=
{
name: "noteq",
flags:NODE_HAS_CHILDREN,
eval: node_noteq_eval,
write: node_noteq_write,
read: node_noteq_read,
exec: node_noteq_exec
};

// --------------------------- x||y ----------------------------------

typedcode_t node_oror_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_oror_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    t = join_types(left.t, right.t, n->type);
    c = left.c;
    c = converttype(c, left.t, t);
    c = abc_dup(c);
    code_t*jmp = c = abc_iftrue(c, 0);
    c = cut_last_push(c);
    c = code_append(c, right.c);
    c = converttype(c, right.t, t);
    code_t*label = c = abc_label(c);
    jmp->branch = label;
    RET;
}
code_t* node_oror_exec(node_t*n)
{
    typedcode_t left = n->child[0]->type->read(n->child[0]);
    code_t* right = n->child[1]->type->exec(n->child[1]);
    code_t*c = left.c;
    code_t*jmp = c = abc_iftrue(c, 0);
    c = code_append(c, right);
    code_t*label = c = abc_label(c);
    jmp->branch = label;
    return c;
}
constant_t node_oror_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_oror = //||
{
name: "oror",
flags:NODE_HAS_CHILDREN,
eval: node_oror_eval,
write: node_oror_write,
read: node_oror_read,
exec: node_oror_exec
};

// --------------------------- x&&y ----------------------------------

typedcode_t node_andand_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_andand_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    t = join_types(left.t, right.t, &node_andand);
    c = left.c;
    c = converttype(c, left.t, t);
    c = abc_dup(c);
    code_t*jmp = c = abc_iffalse(c, 0);
    c = cut_last_push(c);
    c = code_append(c,right.c);
    c = converttype(c, right.t, t);
    code_t*label = c = abc_label(c);
    jmp->branch = label;
    RET;
}
code_t* node_andand_exec(node_t*n)
{
    typedcode_t left = n->child[0]->type->read(n->child[0]);\
    code_t* right = n->child[1]->type->exec(n->child[1]);\
    code_t*c = left.c;
    code_t*jmp = c = abc_iffalse(c, 0);
    c = code_append(c, right);
    code_t*label = c = abc_label(c);
    jmp->branch = label;
    return c;
}
constant_t node_andand_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_andand = //&&
{
name: "andand",
flags:NODE_HAS_CHILDREN,
eval: node_andand_eval,
write: node_andand_write,
read: node_andand_read,
exec: node_andand_exec
};

// ----------------------------- !x -----------------------------------

typedcode_t node_not_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_not_read(node_t*n)
{
    READ_HEADER_ONE;
    c = x.c;
    c = abc_not(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_not_exec(node_t*n)
{
    EXEC_HEADER_ONE;
    return x;
}
constant_t node_not_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_not =
{
name: "not",
flags:NODE_HAS_CHILDREN,
eval: node_not_eval,
write: node_not_write,
read: node_not_read,
exec: node_not_exec
};

// ----------------------------- ~x -----------------------------------

typedcode_t node_bitnot_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_bitnot_read(node_t*n)
{
    READ_HEADER_ONE;
    c = x.c;
    c = abc_bitnot(c);
    t = TYPE_INT;
    RET;
}
code_t* node_bitnot_exec(node_t*n)
{
    EXEC_HEADER_ONE;
    return x;
}
constant_t node_bitnot_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_bitnot =
{
name: "bitnot",
flags:NODE_HAS_CHILDREN,
eval: node_bitnot_eval,
write: node_bitnot_write,
read: node_bitnot_read,
exec: node_bitnot_exec
};

// ----------------------------- x&y -----------------------------------

typedcode_t node_bitand_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_bitand_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_bitand(c);
    t = TYPE_INT;
    RET;
}
code_t* node_bitand_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_bitand_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_bitand =
{
name: "bitand",
flags:NODE_HAS_CHILDREN,
eval: node_bitand_eval,
write: node_bitand_write,
read: node_bitand_read,
exec: node_bitand_exec
};

// ----------------------------- x^y -----------------------------------

typedcode_t node_bitxor_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_bitxor_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_bitxor(c);
    t = TYPE_INT;
    RET;
}
code_t* node_bitxor_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_bitxor_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_bitxor =
{
name: "bitxor",
flags:NODE_HAS_CHILDREN,
eval: node_bitxor_eval,
write: node_bitxor_write,
read: node_bitxor_read,
exec: node_bitxor_exec
};

// ----------------------------- x|y -----------------------------------

typedcode_t node_bitor_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_bitor_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_bitor(c);
    t = TYPE_INT;
    RET;
}
code_t* node_bitor_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_bitor_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_bitor =
{
name: "bitor",
flags:NODE_HAS_CHILDREN,
eval: node_bitor_eval,
write: node_bitor_write,
read: node_bitor_read,
exec: node_bitor_exec
};

// ---------------------------- x>>y -----------------------------------

typedcode_t node_shr_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_shr_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_rshift(c);
    t = TYPE_INT;
    RET;
}
code_t* node_shr_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_shr_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_shr = //>>
{
name: "shr",
flags:NODE_HAS_CHILDREN,
eval: node_shr_eval,
write: node_shr_write,
read: node_shr_read,
exec: node_shr_exec
};

// ---------------------------- x<<y -----------------------------------

typedcode_t node_shl_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_shl_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_lshift(c);
    t = TYPE_INT;
    RET;
}
code_t* node_shl_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_shl_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_shl = //<<
{
name: "shl",
flags:NODE_HAS_CHILDREN,
eval: node_shl_eval,
write: node_shl_write,
read: node_shl_read,
exec: node_shl_exec
};

// ---------------------------- x>>>y -----------------------------------

typedcode_t node_ushr_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_ushr_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_urshift(c);
    t = TYPE_INT;
    RET;
}
code_t* node_ushr_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_ushr_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_ushr = //>>>
{
name: "ushr",
flags:NODE_HAS_CHILDREN,
eval: node_ushr_eval,
write: node_ushr_write,
read: node_ushr_read,
exec: node_ushr_exec
};

// ---------------------------- x in y ----------------------------------

typedcode_t node_in_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_in_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c,right.c);
    c = abc_in(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_in_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_in_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_in = //in
{
name: "in",
flags:NODE_HAS_CHILDREN,
eval: node_in_eval,
write: node_in_write,
read: node_in_read,
exec: node_in_exec
};

// ---------------------------- x as y ----------------------------------

typedcode_t node_as_write(node_t*n)
{
    /* ? */
    syntaxerror("can't assign to this expression");
}
typedcode_t node_as_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c, right.c);
    c = abc_astypelate(c);
    if(TYPE_IS_CLASS(right.t) && right.t->data) {
        t = (classinfo_t*)right.t->data;
    } else {
        t = TYPE_ANY;
    }
    RET;
}
code_t* node_as_exec(node_t*n)
{
    /* we assume here that "as" doesn't have side-effects (like
       early run time type checking) */
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_as_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_as = //as
{
name: "as",
flags:NODE_HAS_CHILDREN,
eval: node_as_eval,
write: node_as_write,
read: node_as_read,
exec: node_as_exec
};

// ------------------------- x instanceof y -----------------------------

typedcode_t node_instanceof_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_instanceof_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c, right.c);
    c = abc_instanceof(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_instanceof_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_instanceof_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_instanceof = //instanceof
{
name: "instanceof",
flags:NODE_HAS_CHILDREN,
eval: node_instanceof_eval,
write: node_instanceof_write,
read: node_instanceof_read,
exec: node_instanceof_exec
};

// ------------------------- x is y --------------------------------------

typedcode_t node_is_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_is_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c, right.c);
    c = abc_istypelate(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_is_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_is_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_is = //is
{
name: "is",
flags:NODE_HAS_CHILDREN,
eval: node_is_eval,
write: node_is_write,
read: node_is_read,
exec: node_is_exec
};

// ------------------------- x[y] --------------------------------------

typedcode_t node_arraylookup_write(node_t*n)
{
    syntaxerror("not implemented yet");
}
typedcode_t node_arraylookup_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = code_append(left.c, right.c);
   
    /* XXX not sure whether this access logic is correct */
    namespace_t ns = {left.t?left.t->access:ACCESS_PACKAGE, ""};
    namespace_set_t nsset;
    namespace_list_t l;l.next = 0;
    nsset.namespaces = &l;
    l.namespace = &ns;
    multiname_t m = {MULTINAMEL, 0, &nsset, 0};

    c = abc_getproperty2(c, &m);
    t = 0; // array elements have unknown type
    RET;
}
code_t* node_arraylookup_exec(node_t*n)
{
    EXEC_HEADER_LEFTRIGHT;
    return code_append(left, right);
}
constant_t node_arraylookup_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_arraylookup =
{
name: "arraylookup",
flags:NODE_HAS_CHILDREN,
eval: node_arraylookup_eval,
write: node_arraylookup_write,
read: node_arraylookup_read,
exec: node_arraylookup_exec
};

// ------------------------- typeof(x) ------------------------------------

typedcode_t node_typeof_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_typeof_read(node_t*n)
{
    READ_HEADER_ONE;
    c = x.c;
    c = abc_typeof(c);
    t = TYPE_BOOLEAN;
    RET;
}
code_t* node_typeof_exec(node_t*n)
{
    EXEC_HEADER_ONE;
    return x;
}
constant_t node_typeof_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_typeof = //typeof
{
name: "typeof",
flags:NODE_HAS_CHILDREN,
eval: node_typeof_eval,
write: node_typeof_write,
read: node_typeof_read,
exec: node_typeof_exec
};

// ------------------------- (void)(x) ------------------------------------

typedcode_t node_void_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_void_read(node_t*n)
{
    code_t*c = n->type->exec(n);
    c = abc_pushundefined(c);
    classinfo_t*t = TYPE_ANY;
    RET;
}
code_t* node_void_exec(node_t*n)
{
    EXEC_HEADER_ONE;
    return x;
}
constant_t node_void_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_void = //void
{
name: "void",
flags:NODE_HAS_CHILDREN,
eval: node_void_eval,
write: node_void_write,
read: node_void_read,
exec: node_void_exec
};

// ---------------------------- -x ----------------------------------------

typedcode_t node_neg_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_neg_read(node_t*n)
{
    READ_HEADER_ONE;
    c = x.c;
    if(IS_INT(x.t)) {
       c = abc_negate_i(c);
       t = TYPE_INT;
    } else {
       c = abc_negate(c);
       t = TYPE_NUMBER;
    }
    RET;
}
code_t* node_neg_exec(node_t*n)
{
    EXEC_HEADER_ONE;
    return x;
}
constant_t node_neg_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_neg = //-
{
name: "neg",
flags:NODE_HAS_CHILDREN,
eval: node_neg_eval,
write: node_neg_write,
read: node_neg_read,
exec: node_neg_exec
};

// ---------------------------- x*=y ----------------------------------------

typedcode_t node_muleq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_muleq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = right.c;
    classinfo_t*f = 0;
    if(BOTH_INT(left.t,right.t)) {
       c=abc_multiply_i(c);
       f = TYPE_INT;
    } else {
       c=abc_multiply(c);
       f = TYPE_NUMBER;
    }
    c=converttype(c, f, left.t);
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_muleq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = right.c;
    classinfo_t*f = 0;
    if(BOTH_INT(left.t,right.t)) {
        c=abc_multiply_i(c);
        f = TYPE_INT;
    } else {
        c=abc_multiply(c);
        f = TYPE_NUMBER;
    }
    c = converttype(c, f, left.t);
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_muleq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_muleq =
{
name: "muleq",
flags:NODE_HAS_CHILDREN,
eval: node_muleq_eval,
write: node_muleq_write,
read: node_muleq_read,
exec: node_muleq_exec
};

// ---------------------------- x%=y ----------------------------------------

typedcode_t node_modeq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_modeq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_modulo(right.c);
    c = converttype(c, TYPE_NUMBER, left.t);
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_modeq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_modulo(right.c);
    c = converttype(c, TYPE_NUMBER, left.t);
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_modeq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_modeq = //%=
{
name: "modeq",
flags:NODE_HAS_CHILDREN,
eval: node_modeq_eval,
write: node_modeq_write,
read: node_modeq_read,
exec: node_modeq_exec
};

// ---------------------------- x<<=y ----------------------------------------

typedcode_t node_shleq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_shleq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_lshift(right.c);
    c = converttype(c, TYPE_INT, left.t);
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_shleq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_lshift(right.c);
    c = converttype(c, TYPE_INT, left.t);
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_shleq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_shleq = //<<=
{
name: "shleq",
flags:NODE_HAS_CHILDREN,
eval: node_shleq_eval,
write: node_shleq_write,
read: node_shleq_read,
exec: node_shleq_exec
};

// ---------------------------- x>>=y ----------------------------------------

typedcode_t node_shreq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_shreq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_rshift(right.c);
    c = converttype(c, TYPE_INT, left.t);
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_shreq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_rshift(right.c);
    c = converttype(c, TYPE_INT, left.t);
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_shreq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_shreq = //>>=
{
name: "shreq",
flags:NODE_HAS_CHILDREN,
eval: node_shreq_eval,
write: node_shreq_write,
read: node_shreq_read,
exec: node_shreq_exec
};

// --------------------------- x>>>=y ----------------------------------------

typedcode_t node_ushreq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_ushreq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_urshift(right.c);
    c = converttype(c, TYPE_UINT, left.t);
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_ushreq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_urshift(right.c);
    c = converttype(c, TYPE_UINT, left.t);
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_ushreq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_ushreq = //>>>=
{
name: "ushreq",
flags:NODE_HAS_CHILDREN,
eval: node_ushreq_eval,
write: node_ushreq_write,
read: node_ushreq_read,
exec: node_ushreq_exec
};

// --------------------------- x/=y ----------------------------------------

typedcode_t node_diveq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_diveq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_divide(right.c);
    c = converttype(c, TYPE_NUMBER, left.t);
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_diveq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_divide(right.c);
    c = converttype(c, TYPE_NUMBER, left.t);
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_diveq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_diveq =
{
name: "diveq",
flags:NODE_HAS_CHILDREN,
eval: node_diveq_eval,
write: node_diveq_write,
read: node_diveq_read,
exec: node_diveq_exec
};

// --------------------------- x|=y ----------------------------------------

typedcode_t node_bitoreq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_bitoreq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_bitor(right.c);
    c = converttype(c, TYPE_INT, left.t);
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_bitoreq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_bitor(right.c);
    c = converttype(c, TYPE_INT, left.t);
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_bitoreq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_bitoreq = //|=
{
name: "bitoreq",
flags:NODE_HAS_CHILDREN,
eval: node_bitoreq_eval,
write: node_bitoreq_write,
read: node_bitoreq_read,
exec: node_bitoreq_exec
};

// --------------------------- x^=y ----------------------------------------

typedcode_t node_bitxoreq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_bitxoreq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_bitxor(right.c);
    c = converttype(c, TYPE_INT, left.t);
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_bitxoreq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_bitxor(right.c);
    c = converttype(c, TYPE_INT, left.t);
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_bitxoreq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_bitxoreq = //^=
{
name: "bitxoreq",
flags:NODE_HAS_CHILDREN,
eval: node_bitxoreq_eval,
write: node_bitxoreq_write,
read: node_bitxoreq_read,
exec: node_bitxoreq_exec
};

// --------------------------- x&=y ----------------------------------------

typedcode_t node_bitandeq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_bitandeq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_bitand(right.c);
    c = converttype(c, TYPE_INT, left.t);
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_bitandeq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = abc_bitand(right.c);
    c = converttype(c, TYPE_INT, left.t);
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_bitandeq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_bitandeq = //^=
{
name: "bitandeq",
flags:NODE_HAS_CHILDREN,
eval: node_bitandeq_eval,
write: node_bitandeq_write,
read: node_bitandeq_read,
exec: node_bitandeq_exec
};

// --------------------------- x+=y ----------------------------------------

typedcode_t node_pluseq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_pluseq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = right.c;
    if(TYPE_IS_INT(left.t)) {
       c = abc_add_i(c);
    } else {
       c = abc_add(c);
       c = converttype(c, join_types(left.t,right.t,&node_plus), left.t);
    }
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_pluseq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = right.c;
    if(TYPE_IS_INT(left.t)) {
       c = abc_add_i(c);
    } else {
       c = abc_add(c);
       c = converttype(c, join_types(left.t,right.t,&node_plus), left.t);
    }
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_pluseq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_pluseq = //+=
{
name: "pluseq",
flags:NODE_HAS_CHILDREN,
eval: node_pluseq_eval,
write: node_pluseq_write,
read: node_pluseq_read,
exec: node_pluseq_exec
};

// --------------------------- x-=y ----------------------------------------

typedcode_t node_minuseq_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_minuseq_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = right.c;
    if(TYPE_IS_INT(left.t)) {
       c = abc_subtract_i(c);
    } else {
       c = abc_subtract(c);
       c = converttype(c, TYPE_NUMBER, left.t);
    }
    c = toreadwrite(left.c, c, 0, 0, 1);
    t = left.t;
    RET;
}
code_t* node_minuseq_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = right.c;
    if(TYPE_IS_INT(left.t)) {
       c = abc_subtract_i(c);
    } else {
       c = abc_subtract(c);
       c = converttype(c, TYPE_NUMBER, left.t);
    }
    return toreadwrite(left.c, c, 0, 0, 0);
}
constant_t node_minuseq_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_minuseq = //-=
{
name: "minuseq",
flags:NODE_HAS_CHILDREN,
eval: node_minuseq_eval,
write: node_minuseq_write,
read: node_minuseq_read,
exec: node_minuseq_exec
};

// --------------------------- x=y -----------------------------------------

typedcode_t node_assign_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_assign_read(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = right.c;
    c = converttype(c, right.t, left.t);
    c = toreadwrite(left.c, c, 1, 0, 1);
    t = left.t;
    RET;
}
code_t* node_assign_exec(node_t*n)
{
    READ_HEADER_LEFTRIGHT;
    c = right.c;
    c = converttype(c, right.t, left.t);
    return toreadwrite(left.c, c, 1, 0, 0);
}
constant_t node_assign_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_assign =
{
name: "assign",
flags:NODE_HAS_CHILDREN,
eval: node_assign_eval,
write: node_assign_write,
read: node_assign_read,
exec: node_assign_exec
};

// --------------------------- x?y1:y2 --------------------------------------

typedcode_t node_tenary_write(node_t*n)
{
    /* TODO: this might actually be kinda useful.
       (global?global.x:this.x) = 3;
    */
    syntaxerror("can't assign to this expression");
}
typedcode_t node_tenary_read(node_t*n)
{
    typedcode_t cond = n->child[0]->type->read(n->child[0]);
    typedcode_t left = n->child[1]->type->read(n->child[1]);
    typedcode_t right = n->child[2]->type->read(n->child[2]);
    classinfo_t*t = join_types(left.t,right.t,&node_tenary);
    code_t* c = cond.c;
    code_t*j1 = c = abc_iffalse(c, 0);
    c = code_append(c, left.c);
    c = converttype(c, left.t, t);
    code_t*j2 = c = abc_jump(c, 0);
    c = j1->branch = abc_label(c);
    c = code_append(c, right.c);
    c = converttype(c, right.t, t);
    c = j2->branch = abc_label(c);
    RET;
}
code_t* node_tenary_exec(node_t*n)
{
    typedcode_t cond = n->child[0]->type->read(n->child[0]);
    code_t* left = n->child[1]->type->exec(n->child[1]);
    code_t* right = n->child[2]->type->exec(n->child[2]);
    code_t* c = cond.c;
    code_t*j1 = c = abc_iffalse(c, 0);
    c = code_append(c, left);
    code_t*j2 = c = abc_jump(c, 0);
    c = j1->branch = abc_label(c);
    c = code_append(c, right);
    c = j2->branch = abc_label(c);
    return c;
}
constant_t node_tenary_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_tenary =
{
name: "tenary",
flags:NODE_HAS_CHILDREN,
eval: node_tenary_eval,
write: node_tenary_write,
read: node_tenary_read,
exec: node_tenary_exec
};

// ---------------------------- comma ----------------------------------------

typedcode_t node_comma_write(node_t*n)
{
    syntaxerror("can't assign to this expression");
}
typedcode_t node_comma_read(node_t*n)
{
    code_t*c = 0;
    classinfo_t*t = 0;
    int i;
    for(i=0;i<n->num_children-1;i++) {
        c = code_append(c, n->child[i]->type->exec(n->child[i]));
    }
    typedcode_t o = n->child[i]->type->read(n->child[i]);
    c = code_append(c, o.c);
    t = o.t;
    RET;
}
code_t* node_comma_exec(node_t*n)
{
    int t;
    code_t*c = 0;
    for(t=0;t<n->num_children;t++) {
        c = code_append(c, n->child[t]->type->exec(n->child[t]));
    }
    return c;
}
constant_t node_comma_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_comma =
{
name: "expr",
flags: NODE_HAS_CHILDREN,
eval: node_comma_eval,
write: node_comma_write,
read: node_comma_read,
exec: node_comma_exec
};


#if 0
// -------------------------- new x -----------------------------------

typedcode_t node_new_write(node_t*n)
{
}
typedcode_t node_new_read(node_t*n)
{
}
code_t* node_new_exec(node_t*n)
{
}
constant_t node_new_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_new = //new
{
write: node_new_write,
read: node_new_read,
eval: node_new_eval,
exec: node_new_exec
};

// ------------------------ delete x ----------------------------------

typedcode_t node_delete_write(node_t*n)
{
}
typedcode_t node_delete_read(node_t*n)
{
}
code_t* node_delete_exec(node_t*n)
{
}
constant_t node_delete_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_delete = //delete
{
write: node_delete_write,
read: node_delete_read,
eval: node_delete_eval,
exec: node_delete_exec
};

// ---------------------------- x.y -----------------------------------

typedcode_t node_dot_write(node_t*n)
{
}
typedcode_t node_dot_read(node_t*n)
{
}
code_t* node_dot_exec(node_t*n)
{
}
constant_t node_dot_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_dot =
{
write: node_dot_write,
read: node_dot_read,
eval: node_dot_eval,
exec: node_dot_exec
};

// --------------------------- x..y -----------------------------------

typedcode_t node_dotdot_write(node_t*n)
{
}
typedcode_t node_dotdot_read(node_t*n)
{
}
code_t* node_dotdot_exec(node_t*n)
{

}
constant_t node_dotdot_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_dotdot = //..
{
write: node_dotdot_write,
read: node_dotdot_read,
eval: node_dotdot_eval,
exec: node_dotdot_exec
};

// --------------------------- x.@y -----------------------------------

typedcode_t node_dotat_write(node_t*n)
{
}
typedcode_t node_dotat_read(node_t*n)
{
}
code_t* node_dotat_exec(node_t*n)
{
}
constant_t node_dotat_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_dotat = //.@
{
write: node_dotat_write,
read: node_dotat_read,
eval: node_dotat_eval,
exec: node_dotat_exec
};

// --------------------------- x.*y -----------------------------------

typedcode_t node_dotstar_write(node_t*n)
{
}
typedcode_t node_dotstar_read(node_t*n)
{
}
code_t* node_dotstar_exec(node_t*n)
{
}
constant_t node_dotstar_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_dotstar = //.*
{
write: node_dotstar_write,
read: node_dotstar_read,
eval: node_dotstar_eval,
exec: node_dotstar_exec
};

// -------------------------- x.(y) -----------------------------------

typedcode_t node_filter_write(node_t*n)
{
}
typedcode_t node_filter_read(node_t*n)
{
}
code_t* node_filter_exec(node_t*n)
{
}
constant_t node_filter_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_filter = //.(
{
write: node_filter_write,
read: node_filter_read,
eval: node_filter_eval,
exec: node_filter_exec
};

// ------------------------ x(y1,...,yn) ------------------------------

typedcode_t node_call_write(node_t*n)
{
}
typedcode_t node_call_read(node_t*n)
{
}
code_t* node_call_exec(node_t*n)
{
}
constant_t node_call_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_call = //functioncall
{
write: node_call_write,
read: node_call_read,
eval: node_call_eval,
exec: node_call_exec
};

// ------------------------------ @x ----------------------------------------

typedcode_t node_at_write(node_t*n)
{
}
typedcode_t node_at_read(node_t*n)
{
}
code_t* node_at_exec(node_t*n)
{
}
constant_t node_at_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_at = //@
{
write: node_at_write,
read: node_at_read,
eval: node_at_eval,
exec: node_at_exec
};

// ---------------------------- x.ns::y ----------------------------------------

typedcode_t node_dotns_write(node_t*n)
{
}
typedcode_t node_dotns_read(node_t*n)
{
}
code_t* node_dotns_exec(node_t*n)
{
}
constant_t node_dotns_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_dotns = //.::
{
write: node_dotns_write,
read: node_dotns_read,
eval: node_dotns_eval,
exec: node_dotns_exec
};
#endif

// ------------------------ constant ------------------------------

typedcode_t node_const_write(node_t*n)
{
    syntaxerror("can't assign a value to a constant");
}
typedcode_t node_const_read(node_t*n)
{
    constant_t*v = n->value;
    code_t*c=0;
    classinfo_t*t=0;
    switch(v->type) {
        case CONSTANT_INT:
            if(v->i>-128 && v->i<128) {
                c = abc_pushbyte(0,v->i);
            } else if(v->i>=-32768 && v->i<32768) {
                c = abc_pushshort(0,v->i);
            } else {
                c = abc_pushint(0,v->i);
            }
            t = TYPE_INT;
        break;
        case CONSTANT_UINT:
            c = abc_pushuint(0,v->u);
            if(v->u<128) {
                c = abc_pushbyte(0,v->u);
            } else if(v->u<32768) {
                c = abc_pushshort(0,v->u);
            } else {
                c = abc_pushint(0,v->u);
            }
            t = TYPE_UINT;
        break;
        case CONSTANT_FLOAT:
            c = abc_pushdouble(0,v->f);
            t = TYPE_FLOAT;
        break;
        case CONSTANT_TRUE:
            c = abc_pushtrue(0);
            t = TYPE_BOOLEAN;
        break;
        case CONSTANT_FALSE:
            c = abc_pushfalse(0);
            t = TYPE_BOOLEAN;
        break;
        case CONSTANT_NULL:
            c = abc_pushnull(0);
            t = TYPE_NULL;
        break;
        case CONSTANT_STRING:
            c = abc_pushstring2(0,v->s);
            t = TYPE_STRING;
        break;
        case CONSTANT_UNDEFINED:
            c = abc_pushundefined(0);
            t = 0;
        break;
        case CONSTANT_NAMESPACE:
        case CONSTANT_NAMESPACE_PACKAGE:
        case CONSTANT_NAMESPACE_PACKAGEINTERNAL:
        case CONSTANT_NAMESPACE_PROTECTED:
        case CONSTANT_NAMESPACE_EXPLICIT:
        case CONSTANT_NAMESPACE_STATICPROTECTED:
        case CONSTANT_NAMESPACE_PRIVATE:
            c = abc_pushnamespace(0, v->ns);
        break;
        case CONSTANT_UNKNOWN:
            syntaxerror("internal error: invalid constant");
        default: 
            *(int*)0=0;
            syntaxerror("invalid constant (%d)", v->type);

    }
    RET;
}

code_t* node_const_exec(node_t*n)
{
    return 0;
}
constant_t node_const_eval(node_t*n)
{
    constant_t*c = constant_clone(n->value);
    constant_t r = *c;
    free(c); //shallow free
    return r;
}
nodetype_t node_const =
{
name: "const",
flags:0,
eval: node_const_eval,
write: node_const_write,
read: node_const_read,
exec: node_const_exec
};

// ------------------------ code node ------------------------------

typedcode_t node_code_write(node_t*n)
{
    typedcode_t t;
    t.c = 0;
    int tmp = gettempvar();
    t.c = abc_setlocal(t.c, tmp);
    code_t*w = toreadwrite(n->code.c, abc_getlocal(0,tmp), 1, 0, 0);
    t.c = code_append(t.c, w);
    t.c = abc_kill(t.c, tmp);
    n->code.c=0;
    t.t = n->code.t;
    return t;
}
typedcode_t node_code_read(node_t*n)
{
    typedcode_t t;
    t.c = n->code.c;
    n->code.c=0;
    t.t = n->code.t;
    return t;
}
code_t* node_code_exec(node_t*n)
{
    code_t*c = code_dup(n->code.c);
    c = cut_last_push(c);
    return c;
}
constant_t node_code_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_code =
{
name: "code",
flags:0,
eval: node_code_eval,
write: node_code_write,
read: node_code_read,
exec: node_code_exec
};

// ------------------------ dummy node ------------------------------
              
typedcode_t node_dummy_write(node_t*n)
{
    syntaxerror("not implemented yet");
}
typedcode_t node_dummy_read(node_t*n)
{
    typedcode_t t;
    t.c = abc_pushundefined(0);
    t.t = TYPE_ANY;
    return t;
}
code_t* node_dummy_exec(node_t*n)
{
    return 0;
}
constant_t node_dummy_eval(node_t*n)
{
    constant_t r;r.type=CONSTANT_UNKNOWN;return r;
}
nodetype_t node_dummy =
{
name: "dummy",
flags:0,
eval: node_dummy_eval,
write: node_dummy_write,
read: node_dummy_read,
exec: node_dummy_exec
};

// ======================== node handling ==============================

node_t* mkdummynode()
{
    node_t*n = (node_t*)rfx_calloc(sizeof(node_t));
    n->type = &node_dummy;
    return n;
}

node_t* mkconstnode(constant_t*c)
{
    node_t*n = (node_t*)malloc(sizeof(node_t));
    n->type = &node_const;
    n->parent = 0;
    n->value = c;
    return n;
}

node_t* mkcodenode(typedcode_t c)
{
    node_t*n = (node_t*)malloc(sizeof(node_t));
    n->type = &node_code;
    n->parent = 0;
    n->code = c;
    return n;
}

node_t* mkmultinode(nodetype_t*t, node_t*one)
{
    node_t*n = (node_t*)malloc(sizeof(node_t));
    n->type = t;
    n->parent = 0;
    n->child = (node_t**)malloc(sizeof(node_t*)*1);
    n->child[0] = one;
    n->num_children = 1;
    return n;
}

node_t* mkstringnode(const char*s)
{
    return mkconstnode(constant_new_string(s));
}

node_t* mkaddnode(node_t*n1, node_t*n2)
{
    return mknode2(&node_plus, n1, n2);
}

node_t* multinode_extend(node_t*n, node_t*add)
{
    n->child = realloc(n->child, (n->num_children+1)*sizeof(node_t*));
    n->child[n->num_children] = add;
    n->num_children++;
    return n;
}

node_t* mknode1(nodetype_t*t, node_t*node)
{
    node_t*n = (node_t*)malloc(sizeof(node_t)+sizeof(node_t*)*2);
    node_t**x = (node_t**)&n[1];
    n->type = t;
    n->parent = 0;
    n->child = x;
    n->num_children = 1;
    x[0] = node;
    x[1] = 0;
    return n;
};

node_t* mknode2(nodetype_t*t, node_t*left, node_t*right)
{
    node_t*n = (node_t*)malloc(sizeof(node_t)+sizeof(node_t*)*3);
    node_t**x = (node_t**)&n[1];
    n->type = t;
    n->parent = 0;
    n->child = x;
    n->num_children = 2;
    x[0] = left;
    x[1] = right;
    x[2] = 0;
    return n;
}
node_t* mknode3(nodetype_t*t, node_t*one, node_t*two, node_t*three)
{
    node_t*n = (node_t*)malloc(sizeof(node_t)+sizeof(node_t*)*4);
    node_t**x = (node_t**)&n[1];
    n->type = t;
    n->parent = 0;
    n->child = x;
    n->num_children = 3;
    x[0] = one;
    x[1] = two;
    x[2] = three;
    x[3] = 0;
    return n;
}

void node_free(node_t*n)
{
    int t;
    if(n->type == &node_code) {
        if(n->code.c) {
            code_free(n->code.c);n->code.c = 0;
        }
    } else if(n->type == &node_const) {
        /* keep, this is not our reference */
    } else for(t=0;t<n->num_children;t++) {
        node_free(n->child[t]);n->child[t] = 0;
    }
    free(n);
}

typedcode_t node_read(node_t*n)
{
    constant_t c = n->type->eval(n);
    if(c.type == CONSTANT_UNKNOWN) {
        typedcode_t t = n->type->read(n);
        node_free(n);
        return t;
    } else {
        typedcode_t t = push_constant(&c);
        node_free(n);
        return t;
    }
}
typedcode_t node_write(node_t*n)
{
    typedcode_t t = n->type->write(n);
    node_free(n);
    return t;
}
code_t* node_exec(node_t*n)
{
    code_t*c = n->type->exec(n);
    node_free(n);
    return c;
}
constant_t node_eval(node_t*n)
{
    constant_t c = n->type->eval(n);
    node_free(n);
    return c;
}

/*
 |
 +-add
 | |
 | +-code
 | |
 | +-code
 |   |
 |   +
 | 
 +-div

*/
void node_dump2(node_t*n, const char*p1, const char*p2, FILE*fi)
{
    if(n->type->flags&NODE_HAS_CHILDREN) {
        fprintf(fi, "%s%s\n", p1, n->type->name);
        int t;
        char*o2 = malloc(strlen(p2)+3);
        strcpy(o2, p2);strcat(o2, "| ");
        char*o3 = malloc(strlen(p2)+3);
        strcpy(o3, p2);strcat(o3, "+-");
        char*o4 = malloc(strlen(p2)+3);
        strcpy(o4, p2);strcat(o4, "  ");

        for(t=0;t<n->num_children;t++) {
            fprintf(fi, "%s\n", o2);
            node_dump2(n->child[t], o3, t<n->num_children-1?o2:o4, fi);
        }
        free(o2);
        free(o3);
        free(o4);
    } else if(n->type == &node_const) {
        char*s = constant_tostring(n->value);
        fprintf(fi, "%s%s (%s)\n", p1, n->type->name, s);
        free(s);
    } else if(n->type == &node_code) {
        fprintf(fi, "%s%s (%s)\n", p1, n->type->name, n->code.t?n->code.t->name:"*");
        code_dump2(n->code.c, 0, 0, (char*)p2, fi);
    } else {
        fprintf(fi, "%s%s\n", p1, n->type->name);
    }
}

void node_dump(node_t*n)
{
    printf("------------VVVV---------------\n");
    node_dump2(n,"","",stdout);
    printf("-------------------------------\n");
}

