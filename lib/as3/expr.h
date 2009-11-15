/* expr.h

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

#ifndef __expr_h__
#define __expr_h__

#include "../q.h"
#include "code.h"
#include "registry.h"
#include "pool.h"

DECLARE(node);
DECLARE(typedcode);
DECLARE(nodetype);
DECLARE_LIST(node);

struct _typedcode {
    code_t*c;
    classinfo_t*t;
};

#define NODE_HAS_CHILDREN 1

struct _nodetype {
    char*name;
    int flags;
    typedcode_t (*write)(node_t*n);
    typedcode_t (*read)(node_t*n);
    code_t* (*exec)(node_t*n);
    constant_t (*eval)(node_t*n);
};

extern nodetype_t node_plus;
extern nodetype_t node_minus;
extern nodetype_t node_lplusplus; //++x
extern nodetype_t node_lminusminus; //--x
extern nodetype_t node_rplusplus; //x++
extern nodetype_t node_rminusminus; //x--
extern nodetype_t node_multiply;
extern nodetype_t node_div;
extern nodetype_t node_mod;
extern nodetype_t node_dot;
extern nodetype_t node_lt;
extern nodetype_t node_gt;
extern nodetype_t node_le; //<=
extern nodetype_t node_ge; //>=
extern nodetype_t node_eqeq; //==
extern nodetype_t node_eqeqeq; //===
extern nodetype_t node_noteqeq; //!==
extern nodetype_t node_noteq; //!=
extern nodetype_t node_oror; //||
extern nodetype_t node_andand; //&&
extern nodetype_t node_not;
extern nodetype_t node_bitnot;
extern nodetype_t node_bitand;
extern nodetype_t node_bitxor;
extern nodetype_t node_bitor;
extern nodetype_t node_shr; //>>
extern nodetype_t node_shl; //<<
extern nodetype_t node_ushr; //>>>
extern nodetype_t node_in; //in
extern nodetype_t node_as; //as
extern nodetype_t node_instanceof; //instanceof
extern nodetype_t node_is; //is
extern nodetype_t node_typeof; //typeof
extern nodetype_t node_void; //void
extern nodetype_t node_neg; //-
extern nodetype_t node_muleq; //*=
extern nodetype_t node_modeq; //%=
extern nodetype_t node_shleq; //<<=
extern nodetype_t node_shreq; //>>=
extern nodetype_t node_ushreq; //>>>=
extern nodetype_t node_diveq; ///=
extern nodetype_t node_bitoreq; //|=
extern nodetype_t node_bitxoreq; //^=
extern nodetype_t node_bitandeq; //&=
extern nodetype_t node_pluseq; //+=
extern nodetype_t node_minuseq; //-=
extern nodetype_t node_assign; //-=
extern nodetype_t node_tenary; //?:
extern nodetype_t node_arraylookup; //x[y]
extern nodetype_t node_comma; //(y1,y2,...,yn)

extern nodetype_t node_const;
extern nodetype_t node_code;

#if 0
extern nodetype_t node_dotdot; //..
extern nodetype_t node_dotat; //.@
extern nodetype_t node_dotstar; //.*
extern nodetype_t node_filter; //.(
extern nodetype_t node_new; //new
extern nodetype_t node_delete; //delete
extern nodetype_t node_call; //functioncall
extern nodetype_t node_at; //@
extern nodetype_t node_dotns; //.::
#endif

node_t* multinode_extend(node_t*n, node_t*add);

struct _node {
    nodetype_t*type;
    node_t*parent;
    union {
        struct {
            node_t**child;
            int num_children;
        };
        constant_t*value;
        char*id;
        typedcode_t code;
    };
};

node_t* mkdummynode();
node_t* mkconstnode(constant_t*c);
node_t* mkcodenode(typedcode_t c);
node_t* mkstringnode(const char*s);
node_t* mkaddnode(node_t*n1, node_t*n2);
node_t* mkmultinode(nodetype_t*t, node_t*one);
node_t* mknode1(nodetype_t*t, node_t*node);
node_t* mknode2(nodetype_t*t, node_t*left, node_t*right);
node_t* mknode3(nodetype_t*t, node_t*one, node_t*two, node_t*three);
void node_free(node_t*n);
typedcode_t node_read(node_t*n); //read and free
typedcode_t node_write(node_t*n); //write and free
code_t* node_exec(node_t*n); //exec and free
constant_t node_eval(node_t*n); //eval and free
void node_dump(node_t*n);

#endif
