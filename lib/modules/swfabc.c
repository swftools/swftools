/* swfabc.c

   Routines for handling Flash2 AVM2 ABC Actionscript

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2007 Alessandro Molina
   Copyright (c) 2007,2008 Matthias Kramm <kramm@quiss.org>
 
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

#include <stdarg.h>
#include "../rfxswf.h"

static unsigned AVM2_uint32toU30(unsigned val, char * out)
{
    unsigned len = 0;

    while(val) {
	  out[len++] = (val&~0x7f?0x80:0) | (val & 0x7F);
	  val >>= 7;
    }

    return len;
}

void AVM2_InsertStops(SWF*swf)
{
    TAG * t;

    //MAIN AS3 CLASS INIT
    unsigned frame_nums = 0;

    /* Count number of pages to set up stop callback for every frame */
    t = swf->firstTag;
    while(t) {
	if (t->id == ST_SHOWFRAME) ++frame_nums;
	t = t->next;
    }

    TAG * classCall = swf_InsertTagBefore(swf, swf->firstTag, ST_SYMBOLCLASS);
    swf_SetU16(classCall, 1); // number of symbols
    swf_SetU16(classCall, 0); // object id
    swf_SetString(classCall, "stop_fla:MainTimeline"); // class to associate with the symbol
	              
    //0x52 is the Flash9 equivalent to DoAction
    char init_pool[322] = { 
      0x01,0x00,0x00,0x00, /* Flag Parameter to ST_DOABC (1 = lazy load)*/
      0x00,                /* 0 terminated class name */
      0x10,0x00,0x2e,0x00, /* ABC file magic number (minor version, major version) */
      0x00, /* zero integers */
      0x00, /* zero unsigned integers */
      0x00, /* zero doubles */
		
      0x11, /* 16 strings (+1) */
	0x08, 's','t','o','p','_','f','l','a', 
	0x0c, 'M','a','i','n','T','i','m','e','l','i','n','e',
	0x0d, 'f','l','a','s','h','.','d','i','s','p','l','a','y', 
	0x09, 'M','o','v','i','e','C','l','i','p',
	0x15, 's','t','o','p','_','f','l','a',':','M','a','i','n','T','i','m','e','l','i','n','e',
	0x06, 'd','o','s','t','o','p',
	0x00, // Empty string: ref. to the global namespace
	0x04, 's','t','o','p',
	0x0e, 'a','d','d','F','r','a','m','e','S','c','r','i','p','t',
	0x06, 'O','b','j','e','c','t',
	0x0c, 'f','l','a','s','h','.','e','v','e','n','t','s',
	0x0f, 'E','v','e','n','t','D','i','s','p','a','t','c','h','e','r',
	0x0d, 'D','i','s','p','l','a','y','O','b','j','e','c','t',
	0x11, 'I','n','t','e','r','a','c','t','i','v','e','O','b','j','e','c','t',
	0x16, 'D','i','s','p','l','a','y','O','b','j','e','c','t','C','o','n','t','a','i','n','e','r',
	0x06, 'S','p','r','i','t','e',

      0x07, /* 6 namespaces (+1) */
	0x16,0x01, /* kind: CONSTANT_PackageNamespace, ref. to: stop_fla */
	0x16,0x03, /* kind: CONSTANT_PackageNamespace, ref. to: flash.display */
	0x18,0x05, /* kind: CONSTANT_ProtectedNamespace, ref. to: stop_fla:MainTimeline */
	0x17,0x01, /* kind: CONSTANT_PackageInternalNs, ref. to: MainTimeline */
	0x16,0x07, /* kind: CONSTANT_PackageNamespace, ref. to the global namespace */
	0x16,0x0b, /* kind: CONSTANT_PackageNamespace, ref. to: flash.events */
			      
      0x00, /* zero namespace sets */
			      
      0x0c, /* 11 MultiNames (+1) */
	/*  1st value: Multiname kind (0x07 = Qualified Name, a multiname with 1 namespace, refers to a method)
	    2nd value: refers to namespace name index in the namespace table
	    3rd value: refers to method name index in the string table */
	0x07,0x01,0x02, /* stop_fla:MainTimeLine */
	0x07,0x02,0x04, /* flash.display:MovieClip */
	0x07,0x04,0x06, /* <4th namespace>:dostop */
	0x07,0x05,0x08, /* <global>:stop */
	0x07,0x05,0x09, /* <global>:addFrameScript */
	0x07,0x05,0x0a, /* <global>:Object */
	0x07,0x06,0x0c, /* flash.events:EventDispatcher */
	0x07,0x02,0x0d, /* flash.display:DisplayObject */
	0x07,0x02,0x0e, /* flash.display:InteractiveObject */
	0x07,0x02,0x0f, /* flash.display:DisplayObjectContainer */
	0x07,0x02,0x10, /* flash.display:Sprite */
			      
      0x04, /* 4 Methods */
	/* METHOD 1 */
	0x00,0x00,0x00,0x00, /* No params (nor params types), no return type, no method name, flags */
	0x00,0x00,0x00,0x00, /* No params (nor params types), no return type, no method name, flags */
	0x00,0x00,0x00,0x00, /* No params (nor params types), no return type, no method name, flags */
	0x00,0x00,0x00,0x00, /* No params (nor params types), no return type, no method name, flags */

      0x00, /* Zero Metadata */

      0x01, /* 1 Class */
	0x01, /* Name: ref. to multiname no. 1 (MainTimeline) */
	0x02, /* Super_name (base class): ref. to multiname no. 2 (flash.display) */
	0x08, /* Flags: 0x08 value indicates that it uses its protected namespace (and make next field exists) */
	0x03, /* Ref. to namespace no. 3 (MainTimeline, protected namespace for this class) */
	0x00, /* No. of interfaces: there are no interfaces (nor interface definition) */
	0x02, /* Initializer index: ref. to method no. 2 */
	0x01, /* No. of traits */
	/* Trait section */     
		0x03, /* Name: ref. to multiname no. 3 (stop) */
		0x01, /* 0000: no attributes (nor metadata in the next subsection);
				 then 1: type is Trait_Method */
		/* Trait_method section */
			0x00, /* NO optimization for virtual calls */
			0x01, /* Ref. to method no. 1 */
	/* HERE FOLLOW THE CLASS DATA */
	0x00, /* Ref. to method no. 0 as static initializer for this class */
	0x00, /* No. of traits for this class (no ones) */

      0x01, /* 1 Script */
	0x03, /* Ref. to method no. 3, invoked prior to any other code in the script  */
	0x01, /* No. of traits */
	/* Trait section */
	0x01, /* Name: ref. to multiname no. 1 (flash.display:MovieClip) */
	0x04,  /* 0000: no attributes (nor metadata in the next subsection);
				  then 4: type is Trait_Class*/
			/* Trait_class section */
			0x01, /* Slot ID where the trait resides */
			0x00, /* Class index (there is only one class)*/

      0x04, /* Method body count: there are 4 method bodies */
	/* HERE FOLLOW THE METHOD BODY DATA */
	0x00, /* Method index, ref. to method no. 0 */
	0x01, /* Max stack slots the method can use */
	0x01, /* No. of registers +1 the method can use: this one cannot use registers */
	0x09,0x0a, /* Min and max scope depth the method can access*/
	0x03, /* Code length (in bytes) */
	/* The actual method code:
			this is the function stop_fla::MainTimeline$cinit()*/
			0xd0,0x30,0x47,
	0x00, /* No. of exceptions (no exceptions) */
	0x00, /* No. of traits (no traits) */

	0x01, /* Method index, ref. to method no. 1 */
	0x01, /* Max stack slots the method can use */
	0x01, /* No. of registers +1 the method can use: this one cannot use registers */
	0x0a,0x0b, /* Min and max scope depth the method can access*/
	0x08, /* Code length (in bytes) */
	/* The actual method code:
	   this is the function stop_fla::dostop(), 
	   the one that actually executes the stop() */
	0xd0,0x30,0x5d,0x04,0x4f,0x04,0x00,0x47,
	0x00, /* No. of exceptions (no exceptions) */
	0x00, /* No. of traits (no traits) */
    };

    /* Header of stop_fla::MainTimeline() method */
    char constructor_header[5] = {
      0x02, /* Method index, ref. to method no. 2 */
      0x03, /* Max stack slots the method can use */
      0x01, /* No. of registers +1 the method can use: this one cannot use registers */
      0x0a,0x0b /* Min and max scope depth the method can access*/
    };

    char constructor_first[5] = {
      /* The actual method code:
    This is the function stop_fla::MainTimeline()
    Does: calls the super constructor for class #0 (MainTimeline),
	    then puts on the stack the object, namespace and name of the arguments
	    needed by the addFrameScripts, then calls the addFrameScripts
	     */
      0xd0,   /* getlocal0 */
      0x30,   /* pushscope */
      0xd0,   /* getlocal0 */
      0x49,   /* constructsuper */
      0x00   /*              0 */
    };

    char constructor_frame_register[10] = {
      0x5d,   /* findpropstrict */
      0x05,   /*              'addFrameScript' */
      0x25,   /* pushshort */
      0x00,   /*              frame number for pushshort */
  0x02,   /* NOP (hack to write u30 value of frame number when > 127) */
      0x60,   /* getlex */
      0x03,   /*              'stop_fla::dostop' */
      0x4f,   /* callpropvoid */
      0x05,   /*              'addFrameScript' */
      0x02   /*              2*/
    };

    char constructor_return[3] = { 
      0x47, /* returnvoid */
      0x00, /* No. of exceptions (no exceptions) */
      0x00, /* No. of traits (no traits) */
    };

    char script_init_pool[47] = {
      0x03, /* Method index, ref. to method no. 3 */
      0x02, /* Max stack slots the method can use */
      0x01, /* No. of registers +1 the method can use: this one cannot use registers */
      0x01,0x09, /* Min and max scope depth the method can access*/
      0x27, /* Code length (in bytes) */
      /* The actual method code:
		      this is the function script0$init() */
      0xd0,0x30,0x65,0x00,0x60,0x06,0x30,0x60,0x07,0x30,0x60,0x08,0x30,0x60,0x09,0x30,0x60,0x0a,0x30,0x60,
      0x0b,0x30,0x60,0x02,0x30,0x60,0x02,0x58,0x00,0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,0x68,0x01,0x47,
      0x00, /* No. of exceptions (no exceptions) */
      0x00  /* No. of traits (no traits) */
    };
    /* END OF THE ABCFILE (Actionscript Byte Code) */

    char code_len[5] = {0, };
    unsigned clen_len = AVM2_uint32toU30((6 + sizeof(constructor_frame_register) * frame_nums), code_len);
    unsigned i, offset = 0;

    TAG *classInit = swf_InsertTagBefore(swf, swf->firstTag, ST_DOABC);

    /* Copy the environment Initialization code */
    swf_SetBlock(classInit, (U8*)init_pool,sizeof(init_pool));
    /* Copy Constructor Method header */
    swf_SetBlock(classInit, (U8*)constructor_header, sizeof(constructor_header));
    /* Add Code block size (u30) to the method header */
    swf_SetBlock(classInit, (U8*)code_len, clen_len);

    /* Copy Constructor Method body first part */
    swf_SetBlock(classInit, (U8*)constructor_first, sizeof(constructor_first));

    /* Register the callback for every frame */
    for (i = 0; i < frame_nums; ++i) {
	AVM2_uint32toU30(i,constructor_frame_register + 3); // Write current frame number
	swf_SetBlock(classInit, (U8*)constructor_frame_register, sizeof(constructor_frame_register));
    }

    /* Copy Constructor method body ending, just a return */
    swf_SetBlock(classInit, (U8*)constructor_return, sizeof(constructor_return));

    /* Copy the script init code */
    swf_SetBlock(classInit, (U8*)script_init_pool, sizeof(script_init_pool));
}

void AVM2_InsertButtonLink(SWF*swf)
{		
    unsigned char displayEventCode[] = {
    // button.dispatchEvent(new Event("pdflinkdown"),true,true)

    0x01, 0x00, 0x00, 0x00, //flags
    0x00, 
    0x10, 0x00, 0x2e, 0x00, //version
    0x00, //no ints
    0x00, //no uints
    0x00, //no floats
    0x14, //19 strings
    0x17, 'e','v','e','n','t','_','o','h','n','e','_','l','i','s','t','e','n','e','r','_','f','l','a',
    0x0c, 'M','a','i','n','t','i','m','e','l','i','n','e',
    0x0d, 'f','l','a','s','h','.','d','i','s','p','l','a','y',
    0x09, 'M','o','v','i','e','c','l','i','p',
    0x24, 'e','v','e','n','t','_','o','h','n','e','_','l','i','s','t','e','n','e','r','_','f','l','a',':','M','a','i','n','t','i','m','e','l','i','n','e',
    0x00,
    0x06, 'b','u','t','t','o','n', 
    0x06, 'f','r','a','m','e','1', 
    0x0c, 'f','l','a','s','h','.','e','v','e','n','t','s', 
    0x05, 'E','v','e','n','t', 
    0x0b, 'p','d','f','l','i','n','k','d','o','w','n', 
    0x0d, 'd','i','s','p','a','t','c','h','E','v','e','n','t', 
    0x0e, 'a','d','d','F','r','a','m','e','S','c','r','i','p','t',
    0x06, 'O','b','j','e','c','t', 
    0x0f, 'E','v','e','n','t','d','i','s','p','a','t','c','h','e','r',
    0x0d, 'D','i','s','p','l','a','y','O','b','j','e','c','t', 
    0x11, 'I','n','t','e','r','a','c','t','i','v','e','O','b','j','e','c','t',
    0x16, 'D','i','s','p','l','a','y','O','b','j','e','c','t','C','o','n','t','a','i','n','e','r',
    0x06, 'S','p','r','i','t','e',

    0x07,  // 6 namespaces
    0x16, 0x01, 
    0x16, 0x03, 
    0x18, 0x05, 
    0x16, 0x06, 
    0x17, 0x01, 
    0x16, 0x09,

    0x00, //zero namespace sets

    0x0e, //13 multinames
    0x07, 0x01, 0x02, 
    0x07, 0x02, 0x04, 
    0x07, 0x04, 0x07, 
    0x07, 0x05, 0x08, 
    0x07, 0x06, 0x0a, 
    0x07, 0x04, 0x0c, 
    0x07, 0x04, 0x0d, 
    0x07, 0x04, 0x0e, 
    0x07, 0x06, 0x0f, 
    0x07, 0x02, 0x10, 
    0x07, 0x02, 0x11, 
    0x07, 0x02, 0x12, 
    0x07, 0x02, 0x13,

    0x04, // 4 methods
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    
    0x00, // zero metadata

    0x01, // 1 class
      0x01, 0x02, 0x08, 0x03, 0x00, 0x02, 
      0x02, // 2 traits
        0x03, 0x00, // name, slot
	  0x00, 0x02, 0x00, 
	0x04, 0x01, // name, method,
	  0x00, 0x01,
      0x00, // ref to method 0 (initializer)
      0x00, // no traits

    0x01, // 1 script
      0x03, 0x01, 0x01, 0x04, 0x01, 0x00, 

    0x04, // 4 method bodies
       // method 1
       0x00, 0x01, 0x01, 0x09, 0x0a, 
       0x03, 0xd0, 0x30, 0x47, // code
       0x00, 0x00, 
       // method 2
       0x01, 0x05, 0x01, 0x0a, 0x0b, 
       0x11, 0xd0, 0x30, 0x60, 0x03, 0x5d, 0x05, 0x2c, 0x0b, 0x26, 0x26, 0x4a, 0x05, 0x03, 0x4f, 0x06, 0x01, 0x47, // code
       0x00, 0x00, 
       // method 3
       0x02, 0x03, 0x01, 0x0a, 0x0b, 
       0x0f, 0xd0, 0x30, 0xd0, 0x49, 0x00, 0x5d, 0x07, 0x24, 0x00, 0x60, 0x04, 0x4f, 0x07, 0x02, 0x47, // code
       0x00, 0x00, 
       // method 4
       0x03, 0x02, 0x01, 0x01, 0x09, 
       0x27, 0xd0, 0x30, 0x65, 0x00, 0x60, 0x08, 0x30, 0x60, 0x09, 0x30, 0x60, 0x0a, 0x30, 0x60, 0x0b, //code
       0x30, 0x60, 0x0c, 0x30, 0x60, 0x0d, 0x30, 0x60, 0x02, 0x30, 0x60, 0x02, 0x58, 0x00, 0x1d, 0x1d, 
       0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x68, 0x01, 0x47, 
       0x00, 0x00,
    };
}

int swf_GetU30(TAG*tag)
{
    U32 shift = 0;
    U32 s = 0;
    while(1) {
	U8 b = swf_GetU8(tag);
	s|=(b&127)<<shift;
	shift+=7;
	if(!(b&128))
	    break;
    }
    return s;
}

int swf_GetS30(TAG*tag)
{
    U32 shift = 0;
    U32 s = 0;
    while(1) {
	U8 b = swf_GetU8(tag);
	s|=(b&127)<<shift;
	shift+=7;
	if(!(b&128)) {
            if(b&64) {
                s|=0xffffffff<<shift;
            }
	    break;
        }
    }
    return s;
}

double swf_GetD64(TAG*tag)
{
    U32 b[2];

    b[0] = swf_GetU32(tag);
    b[1] = swf_GetU32(tag);

    return 0.0;
}

typedef struct _abc_method {
    /* from method signature: */
    const char*name;
    char*paramstr;
    int return_type_index;//index into multiname
    int param_count;
    int params[16]; // indexes into multiname
    U8 flags;

    int index;
    int method_body_index;
} abc_method_t;

typedef struct _abc_multiname {
    int namespace_index;
    int name_index;
} abc_multiname_t;

typedef struct _dict_entry {
    const char*name;
    void*data;
} dict_entry_t;

typedef struct _dict {
    int num;
    int size;
    dict_entry_t*d;
} dict_t;


dict_t* dict_new() {
    dict_t*d = malloc(sizeof(dict_t));
    memset(d, 0, sizeof(dict_t));
    return d;
}
const char*dict_getstr(dict_t*dict, int nr) {
    if(nr > dict->num || nr<0) {
	printf("error: reference to string %d in dict\n");
	return 0;
    }
    return dict->d[nr].name;
}
char*dict_getdata(dict_t*dict, int nr) {
    if(nr > dict->num || nr<0) {
	printf("error: reference to string %d in dict\n");
	return 0;
    }
    return dict->d[nr].data;
}
int dict_append(dict_t*dict, const char*name, void*data) {
    while(dict->size <= dict->num) {
	dict->size += 64;
	if(!dict->d) {
	    dict->d = malloc(sizeof(dict_entry_t)*dict->size);
	} else {
	    dict->d = realloc(dict->d, sizeof(dict_entry_t)*dict->size);
	}
    }
    if(name) {
	dict->d[dict->num].name = strdup(name);
    } else {
	dict->d[dict->num].name = 0;
    }
    dict->d[dict->num].data = data;
    return dict->num++;
}
int dict_find(dict_t*dict, const char*name)
{
    if(!name)
	name = "";
    int t;
    for(t=0;t<dict->num;t++) {
	if(dict->d[t].name && !strcmp(dict->d[t].name,name))
	    return t;
    }
    return -1;
}
int dict_update(dict_t*dict, char*name, void*data) {
    int pos = dict_find(dict, name);
    if(pos>=0) {
	dict->d[pos].data = data;
	return pos;
    }
    return dict_append(dict, name, data);
}
int dict_append_if_new(dict_t*dict, char*name, void*data) {
    int pos = dict_find(dict, name);
    if(pos>=0)
	return pos;
    return dict_append(dict, name, data);
}

typedef struct _abc_file_t {

    // contant_pool

    dict_t*ints;
    dict_t*uints;
    dict_t*floats;
    dict_t*strings;
    dict_t*namespaces;
    dict_t*namespace_sets;
    dict_t*sets;
    dict_t*multinames;

    // abc_file

    dict_t*methods;
    dict_t*classes;
    dict_t*scripts;
    dict_t*method_bodies;
} abc_file_t;

typedef struct _abc_trait {
    unsigned char type;
    int name_index;
    U32 data1;
    U32 data2;
    int vindex;
    int vkind;
} abc_trait_t;

typedef struct _abc_class {
    int index;
    abc_file_t*abc;
    const char*name;
    int classname_index;
    int superclass_index;
    int ns_index;
    int iinit;
    int cinit;
    dict_t*traits;
} abc_class_t;

typedef struct _abc_code {
    int index;
    abc_file_t*abc;
    //abc_class_t*cls;
    abc_method_t*method;
    TAG*tag;
    int max_stack;
    int local_count;
    int init_scope_depth;
    int max_scope_depth;
    int exception_count;
    dict_t*traits;
} abc_code_t;

typedef struct _opcode
{
    unsigned char opcode;
    char*name;
    char*params;
} opcode_t;

/* 2 = multiname
   m = method
   n = number of params
   i = method info
   b = byte
   s = short
   c = class
   s = string
   S = switch
*/

int abc_RegisterNameSpace(abc_file_t*file, char*name);
int abc_RegisterPackageNameSpace(abc_file_t*file, char*name);
int abc_RegisterPackageInternalNameSpace(abc_file_t*file, char*name);
int abc_RegisterProtectedNameSpace(abc_file_t*file, char*name);
int abc_RegisterExplicitNameSpace(abc_file_t*file, char*name);
int abc_RegisterStaticProtectedNameSpace(abc_file_t*file, char*name);
int abc_RegisterPrivateNameSpace(abc_file_t*file, char*name);


opcode_t opcodes[]={
{0xa0, "add", ""},
{0xc5, "add_i", ""},
{0x86, "atype", "2"},
{0x87, "astypelate", ""},
{0xA8, "bitand", ""},
{0x97, "bitnot", ""},
{0xa9, "bitor", ""},
{0xaa, "bitxor", ""},
{0x41, "call", "n"},
{0x43, "callmethod", "mn"},
{0x4c, "callproplex", "2n"},
{0x46, "callproperty", "2n"},
{0x4f, "callpropvoid", "2n"},
{0x44, "callstatic", "in"},
{0x45, "callsuper", "2n"},
{0x4e, "callsupervoid", "2n"},
{0x78, "checkfilter", ""},
{0x80, "coerce", "m"},
{0x82, "coerce_a", ""},
{0x85, "coerce_s", ""},
{0x42, "construct", "n"},
{0x4a, "constructprop", "2n"},
{0x49, "constructsuper", "n"},
{0x76, "convert_b", ""},
{0x73, "convert_i", ""},
{0x75, "convert_d", ""},
{0x77, "convert_o", ""},
{0x74, "convert_u", ""},
{0x70, "convert_s", ""},
{0xef, "debug", "bsbu"},
{0xf1, "debugfile", "s"},
{0xf0, "debugline", "u"},
{0x94, "declocal", "u"},
{0xc3, "declocal_i", "u"},
{0x93, "decrement", ""},
{0xc1, "decrement_i", ""},
{0x6a, "deleteproperty", "2"},
{0xa3, "divide", ""},
{0x2a, "dup", ""},
{0x06, "dxns", "s"},
{0x07, "dxnslate", ""},
{0xab, "equals", ""},
{0x72, "esc_xattr", ""},
{0x71, "esc_xelem", ""},
{0x5e, "findproperty", "2"},
{0x5d, "findpropstrict", "2"},
{0x59, "getdescendants", "2"},
{0x64, "getglobalscope", ""},
{0x6e, "getglobalslot", "u"},
{0x60, "getlex", "2"},
{0x62, "getlocal", "u"},
{0xd0, "getlocal_0", ""},
{0xd1, "getlocal_1", ""},
{0xd2, "getlocal_2", ""},
{0xd3, "getlocal_3", ""},
{0x66, "getproperty", "2"},
{0x65, "getscopeobject", "u"},
{0x6c, "getslot", "u"},
{0x04, "getsuper", "2"},
{0xaf, "greaterequals", ""},
{0x1f, "hasnext", ""},
{0x32, "hasnext2", "uu"},
{0x13, "ifeq", "j"},
{0x12, "iffalse", "j"},
{0x18, "ifge", "j"},
{0x17, "ifgt", "j"},
{0x16, "ifle", "j"},
{0x15, "iflt", "j"},
{0x0f, "ifnge", "j"},
{0x0e, "ifngt", "j"},
{0x0d, "ifnle", "j"},
{0x0c, "ifnlt", "j"},
{0x14, "ifne", "j"},
{0x19, "ifstricteq", "j"},
{0x1a, "ifstrictne", "j"},
{0x11, "iftrue", "j"},
{0xb4, "in", ""},
{0x92, "inclocal", "u"},
{0xc2, "inclocal_i", "u"},
{0x91, "increment", ""},
{0xc0, "increment_i", ""},
{0x68, "initproperty", "2"},
{0xb1, "instanceof", ""},
{0xb2, "istype", "2"},
{0xb3, "istypelate", ""},
{0x10, "jump", "j"},
{0x08, "kill", "u"},
{0x09, "label", ""},
{0xae, "lessequals", ""},
{0xad, "lessthan", ""},
{0x1b, "lookupswitch", "S"},
{0xa5, "lshift", ""},
{0xa4, "modulo", ""},
{0xa2, "multiply", ""},
{0xc7, "multiply_i", ""},
{0x90, "negate", ""},
{0xc4, "negate_i", ""},
{0x57, "newactivation", ""},
{0x56, "newarray", "u"},
{0x5a, "newcatch", "u"}, //index into exception_info
{0x58, "newclass", "c"}, //index into class_info
{0x40, "newfunction", "u"}, //index into method_info
{0x55, "newobject", "u"},
{0x1e, "nextname", ""},
{0x23, "nextvalue", ""},
{0x02, "nop", ""},
{0x96, "not", ""},
{0x29, "pop", ""},
{0x1d, "popscope", ""},
{0x24, "pushbyte", "b"},
{0x2f, "pushdouble", "u"}, //index into floats
{0x27, "pushfalse", ""},
{0x2d, "pushint", "u"}, //index into ints
{0x31, "pushnamespace", "u"}, //index into namespace
{0x28, "pushnan", ""},
{0x20, "pushnull", ""},
{0x30, "pushscope", ""},
{0x25, "pushshort", "u"},
{0x2c, "pushstring", "s"},
{0x26, "pushtrue", ""},
{0x2e, "pushuint", "u"}, //index into uints
{0x21, "pushundefined", ""},
{0x1c, "pushwith", ""},
{0x48, "returnvalue", ""},
{0x47, "returnvoid", ""},
{0xa6, "rshift", ""},
{0x63, "setlocal", "u"},
{0xd4, "setlocal_0", ""},
{0xd5, "setlocal_1", ""},
{0xd6, "setlocal_2", ""},
{0xd7, "setlocal_3", ""},
{0x6f, "setglobalshot", "u"},
{0x61, "setproperty", "2"},
{0x6d, "setslot", "u"},
{0x05, "setsuper", "2"},
{0xac, "strictequals", ""},
{0xa1, "subtract", ""},
{0xc6, "subtract_i", ""},
{0x2b, "swap", ""},
{0x03, "throw", ""},
{0x95, "typeof", ""},
{0xa7, "urshift", ""},
{0xb0, "xxx", ""},
};

int swf_GetU24(TAG*tag)
{
    int b1 = swf_GetU8(tag);
    int b2 = swf_GetU8(tag);
    int b3 = swf_GetU8(tag);
    return b3<<16|b2<<8|b1;
}
static int parse_code(TAG*tag, int len, abc_file_t*pool, char*prefix)
{
    int end=tag->pos+len;
    while(tag->pos<end) {
	U8 opcode = swf_GetU8(tag);
	int t;
	char found = 0;
	for(t=0;t<sizeof(opcodes)/sizeof(opcodes[0]);t++) {
	    if(opcodes[t].opcode == opcode) {
		printf("%s%s ", prefix, opcodes[t].name);
		char*p = opcodes[t].params;
		char first = 1;
		while(*p) {
		    if(!first)
			printf(", ");
		    if(*p == 'n') {
			int n = swf_GetU30(tag);
			printf("%d params", n);
		    } else if(*p == '2') {
			const char* m = dict_getstr(pool->multinames, swf_GetU30(tag));
			printf("%s", m);
		    } else if(*p == 'm') {
			int n = swf_GetU30(tag);
			printf("[method%d]", n);
		    } else if(*p == 'c') {
			int n = swf_GetU30(tag);
			printf("[classinfo%d]", n);
		    } else if(*p == 'i') {
			int n = swf_GetU30(tag);
			printf("[methodbody%d]", n);
		    } else if(*p == 'u') {
			int n = swf_GetU30(tag);
			printf("%d", n);
		    } else if(*p == 'b') {
			int b = swf_GetU8(tag);
			printf("%02x", b);
		    } else if(*p == 'j') {
			printf("%d", swf_GetU24(tag));
		    } else if(*p == 's') {
			const char*s = dict_getstr(pool->strings, swf_GetU30(tag));
			printf("\"%s\"", s);
		    } else if(*p == 'S') {
                        swf_GetU24(tag); //default
			int num = swf_GetU30(tag)+1;
                        int t;
                        for(t=0;t<num;t++) 
                            swf_GetU24(tag);
		    } else {
                        printf("Can't parse opcode param type \"%c\"\n", *p);
                        return 0;
                    }
		    p++;
		    first = 0;
		}
		found = 1;
		break;
	    }
	}
	if(!found) {
	    printf("Can't parse opcode %02x\n", opcode);
	    return 0;
	}
	printf("\n");
    }
    if(tag->pos!=end) {
	printf("Read beyond end of ABC Bytecode\n");
	return 0;
    }
    return 1;
}

static void dump_method(const char*prefix, const char*type, const char*name, int nr, abc_file_t*pool)
{
    if(nr >= pool->methods->num) {
	printf("Invalid method number: %d\n", nr);
	return;
    }
    abc_method_t*m = (abc_method_t*)dict_getdata(pool->methods, nr);
    printf("%s%s %s %s%s\n", prefix, type, dict_getstr(pool->multinames,m->return_type_index), name, m->paramstr);

    abc_code_t*c = (abc_code_t*)dict_getdata(pool->method_bodies, m->method_body_index);
    
    printf("%s[%d %d %d %d %d]\n", prefix, c->max_stack, c->local_count, c->init_scope_depth, c->max_scope_depth, c->exception_count);

    swf_SetTagPos(c->tag, 0);
    char prefix2[80];
    sprintf(prefix2, "%s    ", prefix);
    printf("%s{\n", prefix);
    parse_code(c->tag, c->tag->len, pool,prefix2);
    printf("%s}\n", prefix);
}

//#define DEBUG
#define DEBUG if(0)

static void parse_metadata(TAG*tag, abc_file_t*pool)
{
    int t;
    int num_metadata = swf_GetU30(tag);
    DEBUG printf("%d metadata\n");
    for(t=0;t<num_metadata;t++) {
        const char*name = dict_getstr(pool->strings, swf_GetU30(tag));
        int num = swf_GetU30(tag);
        int s;
        DEBUG printf("  %s\n", name);
        for(s=0;s<num;s++) {
            const char*key = dict_getstr(pool->strings, swf_GetU30(tag));
            const char*value = dict_getstr(pool->strings, swf_GetU30(tag));
            DEBUG printf("    %s=%s\n", key, value);
        }
    }
}

#define TRAIT_SLOT 0
#define TRAIT_METHOD 1
#define TRAIT_GETTER 2
#define TRAIT_SETTER 3
#define TRAIT_CLASS 4
#define TRAIT_FUNCTION 5
static dict_t* parse_traits(char*prefix, TAG*tag, abc_file_t*pool, char print)
{
    int num_traits = swf_GetU30(tag);
    dict_t*traits = dict_new();
    int t;
    if(num_traits) {
        DEBUG printf("%d traits\n", num_traits);
    }
    for(t=0;t<num_traits;t++) {
	abc_trait_t*trait = malloc(sizeof(abc_trait_t));
	memset(trait, 0, sizeof(abc_trait_t));
	dict_append(traits, 0, trait);
	int name_index = swf_GetU30(tag);
	const char*name = dict_getstr(pool->multinames, name_index);
	U8 kind = swf_GetU8(tag);
        U8 attributes = kind&0xf0;
        kind&=0x0f;
	DEBUG printf("  trait %d) %s type=%02x\n", t, name, kind);
	if(kind == 1 || kind == 2 || kind == 3) { // method / getter / setter
	    int disp_id = swf_GetU30(tag);
	    int nr = swf_GetU30(tag);
	    DEBUG printf("  %smethod %d %d %s\n", prefix, nr, disp_id, ((abc_method_t*)dict_getdata(pool->methods, nr))->paramstr);
	    if(print) dump_method(prefix, kind==1?"method":(kind==2?"getter":"setter"), name, nr, pool);
	} else if(kind == 5) { // function
	    int slot_id = swf_GetU30(tag);
	    int nr = swf_GetU30(tag);
	    if(print) dump_method(prefix, "function", name, nr, pool);
	} else if(kind == 4) { // class
	    int slot_id = swf_GetU30(tag);
	    int cls = swf_GetU30(tag);
	    if(print) printf("  %sclass %s %d %d\n", prefix, name, slot_id, cls);
	} else if(kind == 0 || kind == 6) { // slot, const
	    int slot_id = swf_GetU30(tag);
	    const char*type_name = dict_getstr(pool->multinames, swf_GetU30(tag));
	    int vindex = swf_GetU30(tag);
	    if(vindex) {
		U8 vkind = swf_GetU8(tag);
	    }
	    if(print) printf("  %sslot %s %d %s (vindex=%d)\n", prefix, name, slot_id, type_name, vindex);
	} else {
	    printf("    can't parse trait type %d\n", kind);
	    return 0;
	}
        if(attributes&0x40) {
            int num = swf_GetU30(tag);
            int s;
            for(s=0;s<num;s++) {
                swf_GetU30(tag); //index into metadata array
            }
        }
    }
    return traits;
}

void swf_CopyData(TAG*to, TAG*from, int len)
{
    unsigned char*data = malloc(len);
    swf_GetBlock(from, data, len);
    swf_SetBlock(to, data, len);
    free(data);
}

abc_file_t*abc_file_new()
{
    abc_file_t*f = malloc(sizeof(abc_file_t));
    memset(f, 0, sizeof(abc_file_t));

    f->ints = dict_new();
    dict_append(f->ints, 0, (void*)0);
    f->uints = dict_new();
    dict_append(f->uints, 0, (void*)0);
    f->floats = dict_new();
    dict_append(f->floats, 0, 0);
    f->strings = dict_new();
    dict_append(f->strings, "--<UNDEFINED>--", 0);
    f->namespaces = dict_new();
    dict_append(f->namespaces, "--<UNDEFINED>--", 0);
    f->namespace_sets = dict_new();
    dict_append(f->namespace_sets, "--<UNDEFINED>--", 0);
    f->sets = dict_new();
    dict_append(f->sets, "--<UNDEFINED>--", 0);
    f->multinames = dict_new();
    dict_append(f->multinames, "--<UNDEFINED>--", 0);

    // abc_file

    f->methods = dict_new();
    f->classes = dict_new();
    f->scripts = dict_new();
    f->method_bodies = dict_new();

    return f;
}

static char* access2str(int type)
{
    if(type==0x08) return "";
    else if(type==0x16) return "package";
    else if(type==0x17) return "packageinternal";
    else if(type==0x18) return "protected";
    else if(type==0x19) return "explicit";
    else if(type==0x1A) return "staticprotected";
    else if(type==0x05) return "private";
    else return "undefined";
}

void swf_DissassembleABC(TAG*tag)
{
    abc_file_t* pool = abc_file_new();

    swf_SetTagPos(tag, 0);
    U32 flags = swf_GetU32(tag);
    int t;
    DEBUG printf("flags=%08x\n", flags);
    char*classname = swf_GetString(tag);
    U32 version = swf_GetU32(tag);
    if(version!=0x002e0010) {
        fprintf(stderr, "Warning: unknown AVM2 version %08x\n", version);
    }

    int num_ints = swf_GetU30(tag);
    DEBUG printf("%d ints\n", num_ints);
    for(t=1;t<num_ints;t++) {
        S32 v = swf_GetU30(tag);
        DEBUG printf("int %d) %d\n", t, v);
        dict_append(pool->ints, 0, (void*)v);
    }

    int num_uints = swf_GetU30(tag);
    DEBUG printf("%d uints\n", num_uints);
    for(t=1;t<num_uints;t++) {
        U32 v = swf_GetS30(tag);
        DEBUG printf("uint %d) %d\n", t, v);
        dict_append(pool->uints, 0, (void*)v);
    }
    
    int num_floats = swf_GetU30(tag);
    DEBUG printf("%d floats\n", num_floats);
    for(t=1;t<num_floats;t++) {
        double d = swf_GetD64(tag);
        DEBUG printf("float %d) %f\n", t, d);
        dict_append(pool->floats, 0, 0);
    }
    
    int num_strings = swf_GetU30(tag);
    DEBUG printf("%d strings\n", num_strings);
    for(t=1;t<num_strings;t++) {
	int len = swf_GetU30(tag);
	char*s = malloc(len+1);
	swf_GetBlock(tag, s, len);
	s[len] = 0;
	dict_append(pool->strings, s, 0);
	DEBUG printf("%d) \"%s\"\n", t, pool->strings->d[t].name);
    }
    int num_namespaces = swf_GetU30(tag);
    DEBUG printf("%d namespaces\n", num_namespaces);
    for(t=1;t<num_namespaces;t++) {
	U8 type = swf_GetU8(tag);
	int namenr = swf_GetU30(tag);
	const char*name = dict_getstr(pool->strings, namenr);
	dict_append(pool->namespaces, name, (void*)(int)type);
	int w = 0;
	DEBUG w=1;
	if(w) {
	    if(type==0x08) printf("Namespace %s\n", name);
	    else if(type==0x16) printf("PackageNamespace %s\n", name);
	    else if(type==0x17) printf("PackageInternalNs %s\n", name);
	    else if(type==0x18) printf("ProtectedNamespace %s\n", name);
	    else if(type==0x19) printf("ExplicitNamespace %s\n", name);
	    else if(type==0x1A) printf("StaticProtectedNs %s\n", name);
	    else if(type==0x05) printf("PrivateNs %s\n", name);
	    else {
		printf("Undefined namespace type\n");
		return;
	    }
	}
    }
    int num_sets = swf_GetU30(tag);
    DEBUG printf("%d namespace sets\n", num_namespaces);
    for(t=1;t<num_sets;t++) {
        int count = swf_GetU30(tag);
        int s;
        const char**name = malloc(sizeof(const char*)*count);
        int l = 0;
        for(s=0;s<count;s++) {
            int nsnr = swf_GetU30(tag);
            name[s] = dict_getstr(pool->namespaces, nsnr);
            l += strlen(name[s])+1;
        }
        char*desc = malloc(l+16);
        strcpy(desc, "{");
        for(s=0;s<count;s++) {
            strcat(desc, name[s]);
            strcat(desc, ",");
        }
        strcat(desc, "}");
        dict_append(pool->namespace_sets, desc, 0);
        DEBUG printf("set %d) %s\n", t, desc);
    }

    int num_multinames = swf_GetU30(tag);
    DEBUG printf("%d multinames\n", num_multinames);
    for(t=1;t<num_multinames;t++) {
	U8 type = swf_GetU8(tag);
	char*mname = 0;
	if(type==0x07 || type==0x0d) {
	    int nr1 = swf_GetU30(tag);
	    const char*namespace = dict_getstr(pool->namespaces, nr1);
	    U8 access = (U8)(int)dict_getdata(pool->namespaces, nr1);
	    const char*methodname = dict_getstr(pool->strings, swf_GetU30(tag));
	    DEBUG printf("multiname %d) <%s> %s:%s\n", t, access2str(access), namespace, methodname);
	    mname = malloc(strlen(namespace)+strlen(methodname)+300);
	    sprintf(mname, "[%s]\0", access2str(access));
	    strcat(mname, namespace);
	    strcat(mname, ":");
	    strcat(mname, methodname);
	} else if(type==0x0f || type==0x10) {
	    const char*methodname = dict_getstr(pool->strings, swf_GetU30(tag));
	    mname = strdup(methodname);
	} else if(type==0x11 || type==0x12) {
	    mname = strdup("");
	} else if(type==0x09 || type==0x0e) {
	    const char*methodname = dict_getstr(pool->strings, swf_GetU30(tag));
	    const char*namespace = dict_getstr(pool->namespace_sets, swf_GetU30(tag));
            DEBUG printf("multiname %d) %s:%s\n", t, namespace, methodname);
	    mname = malloc(strlen(namespace)+strlen(methodname)+16);
	    strcpy(mname, namespace);
	    strcat(mname, ":");
	    strcat(mname, methodname);
        } else if(type==0x1b || type==0x1c) {
	    const char*nsset = dict_getstr(pool->namespace_sets, swf_GetU30(tag));
	    mname = strdup(nsset);
	} else {
	    printf("can't parse type %d multinames yet\n", type);
	    return;
	}
	dict_append(pool->multinames, mname, 0);
	free(mname);
    }
    
    int num_methods = swf_GetU30(tag);
    DEBUG printf("%d methods\n", num_methods);
    for(t=0;t<num_methods;t++) {
	abc_method_t*m = malloc(sizeof(abc_method_t));
	memset(m, 0, sizeof(*m));
	m->param_count = swf_GetU30(tag);
	m->return_type_index = swf_GetU30(tag);
	m->index = t;
	int s;
        int params_len = 256;
	char* params = malloc(params_len);
	params[0]='(';
	params[1]=0;
	for(s=0;s<m->param_count;s++) {
	    int typenr = swf_GetU30(tag);
	    if(s < sizeof(m->params)/sizeof(m->params[0]))
		m->params[s] = typenr;
	    const char*type = dict_getstr(pool->multinames, typenr);
            while(strlen(type)+strlen(params)>params_len-4) {
                params_len+=256;
                params = realloc(params, params_len);
            }
	    if(s)
		strcat(params, ", ");
	    strcat(params, type);
	}
	strcat(params, ")");
	int namenr = swf_GetU30(tag);
	m->name = "";
	if(namenr)
	    m->name = dict_getstr(pool->strings, namenr);
	m->paramstr=strdup(params);
        free(params);params = 0;

	m->flags = swf_GetU8(tag);
	
        DEBUG printf("method %d) %s flags=%02x\n", t, m->paramstr, m->flags);

        if(m->flags&0x08) {
            /* optional parameters */
            int num = swf_GetU30(tag);
            int s;
            for(s=0;s<num;s++) {
                int val = swf_GetU30(tag);
                U8 kind = swf_GetU8(tag); // specifies index type for "val"
            }
        }
	if(m->flags&0x80) {
	    printf("can't parse param names yet\n");
	    return;
	}
	dict_append(pool->methods, m->name, m);
    }
            
    parse_metadata(tag, pool);
	
    /* skip classes, and scripts for now, and do the real parsing later */
    int num_classes = swf_GetU30(tag);
    int classes_pos = tag->pos;
    DEBUG printf("%d classes\n", num_classes);
    for(t=0;t<num_classes;t++) {
        DEBUG printf("class %d\n", t);
	swf_GetU30(tag); //classname
	swf_GetU30(tag); //supername
	U8 flags = swf_GetU8(tag);
	if(flags&8) 
	    swf_GetU30(tag); //protectedNS
	int inum = swf_GetU30(tag); //interface count
        int s;
        for(s=0;s<inum;s++) {
            const char*interface = dict_getstr(pool->multinames, swf_GetU30(tag));
            DEBUG printf("  class %d interface: %s\n", t, interface);
        }
	swf_GetU30(tag); //iinit
	if(!parse_traits("", tag, pool, 0))
	    return;
    }
    for(t=0;t<num_classes;t++) {
	swf_GetU30(tag); //cinit
	if(!parse_traits("", tag, pool, 0))
	    return;
    }
    int num_scripts = swf_GetU30(tag);
    DEBUG printf("%d scripts\n", num_scripts);
    for(t=0;t<num_scripts;t++) {
	int init = swf_GetU30(tag);
	if(!parse_traits("", tag, pool, 0))
	    return;
    }

    int num_method_bodies = swf_GetU30(tag);
    DEBUG printf("%d method bodies\n", num_method_bodies);
    for(t=0;t<num_method_bodies;t++) {
	int methodnr = swf_GetU30(tag);
	if(methodnr >= pool->methods->num) {
	    printf("Invalid method number: %d\n", methodnr);
	    return;
	}
	abc_method_t*m = (abc_method_t*)dict_getdata(pool->methods, methodnr);
	abc_code_t*c = malloc(sizeof(abc_code_t));
	memset(c, 0, sizeof(abc_code_t));
	c->max_stack = swf_GetU30(tag);
	c->local_count = swf_GetU30(tag);
	c->init_scope_depth = swf_GetU30(tag);
	c->max_scope_depth = swf_GetU30(tag);
	int code_length = swf_GetU30(tag);
	c->method = m;
	m->method_body_index = t;

	c->tag = swf_InsertTag(0,0);

	swf_CopyData(c->tag, tag, code_length);

	int exception_count = swf_GetU30(tag);
        int s;
        for(s=0;s<exception_count;s++) {
            swf_GetU30(tag); //from
            swf_GetU30(tag); //to
            swf_GetU30(tag); //target
            swf_GetU30(tag); //exc_type
            swf_GetU30(tag); //var_name
        }
	c->traits = parse_traits("<method body trait>", tag, pool, 1);
	if(!c->traits) {
	    return;
	}
	DEBUG printf("method_body %d) (method %d), %d bytes of code", t, methodnr, code_length);
	int r,l = code_length>32?32:code_length;
	for(r=0;r<l;r++) {
	    DEBUG printf("%02x ", c->tag->data[r]);
	}
	DEBUG printf("\n");

	dict_append(pool->method_bodies, 0, c);
    }
    if(tag->len - tag->pos) {
	printf("%d unparsed bytes remaining in ABC block\n", tag->len - tag->pos);
	return;
    }

    swf_SetTagPos(tag, classes_pos);
    for(t=0;t<num_classes;t++) {
	abc_class_t*cls = malloc(sizeof(abc_class_t));
	memset(cls, 0, sizeof(abc_class_t));
	const char* classname = dict_getstr(pool->multinames, swf_GetU30(tag));
	const char* supername = dict_getstr(pool->multinames, swf_GetU30(tag));
	cls->name = classname;
	U8 flags = swf_GetU8(tag);
	const char*ns = "";
	if(flags&8) {
	    cls->ns_index = swf_GetU30(tag);
	    ns = dict_getstr(pool->namespaces, cls->ns_index);
	}
	printf("class %s extends %s, %s, flags=%02x\n", classname, supername, ns, flags);
	printf("{\n");
	
        int num_interfaces = swf_GetU30(tag); //interface count
        int s;
        for(s=0;s<num_interfaces;s++) {
            swf_GetU30(tag); // multiname index TODO
        }
	cls->iinit = swf_GetU30(tag);
	dump_method("    ","constructor", classname, cls->iinit, pool);
	cls->traits = parse_traits("    ",tag, pool, 1);
	if(!cls->traits) {
	    return;
	}
	printf("}\n");
	dict_append(pool->classes, 0, cls);
    }
    for(t=0;t<num_classes;t++) {
	int cinit = swf_GetU30(tag);
	dump_method("    ","staticconstructor", "", cinit, pool);
	if(!parse_traits("    ",tag, pool, 1))
	    return;
    }
    int num_scripts2 = swf_GetU30(tag);
    printf("\n");
    for(t=0;t<num_scripts2;t++) {
	int init = swf_GetU30(tag);
	dump_method("","initmethod", "init", init, pool);
	if(!parse_traits("", tag, pool, 1))
	    return;
    }
}

static int registerNameSpace(abc_file_t*file, U8 access, char*name) {
    if(access==0) { // autodetect access
	char*n = strdup(name);
	if(n[0] == '[') {
	    char*bracket = strchr(n, ']');
	    if(bracket) {
		*bracket = 0;
		char*a = n+1;
		name += (bracket-n)+1;
		if(!strcmp(a, "")) access=0x16;
		else if(!strcmp(a, "package")) access=0x16;
		else if(!strcmp(a, "packageinternal")) access=0x17;
		else if(!strcmp(a, "protected")) access=0x18;
		else if(!strcmp(a, "explicit")) access=0x19;
		else if(!strcmp(a, "staticprotected")) access=0x1a;
		else if(!strcmp(a, "private")) access=0x05;
		else {
		    fprintf(stderr, "Undefined access level: [%s]\n", a);
		    return -1;
		}
	    }
	} else {
	    access = 0x16;
	}
	free(n);
    }
    int t;
    for(t=0;t<file->namespaces->num;t++) {
	const char*name2 = dict_getstr(file->namespaces, t);
	U8 access2 = (U8)(int)dict_getdata(file->namespaces, t);
	if(access == access2 && !strcmp(name, name2)) {
	    return t;
	}
    }
    dict_update(file->strings, name, 0);
    return dict_append(file->namespaces, name, (void*)(int)access);
}
int abc_RegisterNameSpace(abc_file_t*file, char*name) {
    return registerNameSpace(file, 0x08, name);
}
int abc_RegisterPackageNameSpace(abc_file_t*file, char*name) {
    return registerNameSpace(file, 0x16 , name);
}
int abc_RegisterPackageInternalNameSpace(abc_file_t*file, char*name) {
    return registerNameSpace(file, 0x17, name);
}
int abc_RegisterProtectedNameSpace(abc_file_t*file, char*name) {
    return registerNameSpace(file, 0x18, name);
}
int abc_RegisterExplicitNameSpace(abc_file_t*file, char*name) {
    return registerNameSpace(file, 0x19, name);
}
int abc_RegisterStaticProtectedNameSpace(abc_file_t*file, char*name) {
    return registerNameSpace(file, 0x1a, name);
}
int abc_RegisterPrivateNameSpace(abc_file_t*file, char*name) {
    return registerNameSpace(file, 0x05, name);
}
static int multiname_index(abc_file_t*abc, const char*name2) 
{
    if(!name2)
	name2 = ":";
    int pos = dict_find(abc->multinames, name2);
    if(pos>=0)
	return pos;

    int access = 0x16;
    char*n = strdup(name2);
    char*p = strchr(n, ':');
    char*namespace=0,*name=0;
    if(!p) {
	namespace = "";
	name = n;
    } else {
	*p = 0;
	namespace = n;
	name = p+1;
    }
    abc_multiname_t*m = malloc(sizeof(abc_multiname_t));
    m->namespace_index = registerNameSpace(abc, 0, namespace);
    m->name_index = dict_append_if_new(abc->strings, name, 0);
    return dict_append(abc->multinames, name2, m);
}

abc_class_t* abc_NewClass(abc_file_t*abc, char*classname, char*superclass) {
    abc_class_t* c = malloc(sizeof(abc_class_t));
    memset(c, 0, sizeof(abc_class_t));
    c->index = dict_append(abc->classes, 0, c);
    c->abc = abc;
    c->name = strdup(classname);
    c->classname_index = multiname_index(abc, classname);
    c->superclass_index = multiname_index(abc, superclass);
    c->ns_index = abc_RegisterProtectedNameSpace(abc, classname);
    c->iinit = -1;
    c->cinit = -1;

    c->traits = dict_new();
    return c;
}

abc_code_t* add_method(abc_file_t*abc, abc_class_t*cls, char*returntype, char*name, int num_params, va_list va)
{
    /* construct code (method body) object */
    abc_code_t* c = malloc(sizeof(abc_code_t));
    memset(c, 0, sizeof(abc_code_t));
    c->index = dict_append(abc->method_bodies, 0, c);
    c->tag = swf_InsertTag(0,0);
    c->abc = abc;
    c->traits = dict_new();

    /* construct method object */
    abc_method_t* m = malloc(sizeof(abc_method_t));
    memset(m, 0, sizeof(abc_method_t));
    m->param_count = num_params;
    m->index = dict_append(abc->methods, 0, m);
    if(returntype) 
	m->return_type_index = multiname_index(abc, returntype);
    else
	m->return_type_index = 0;
    if(num_params>sizeof(m->params)/sizeof(m->params[0])) {
	fprintf(stderr, "abc: Too many parameters\n");
	return 0;
    }
    int t;
    for(t=0;t<num_params;t++) {
	const char*param = va_arg(va, const char*);
	m->params[t] = multiname_index(abc, param);
    }

    /* crosslink the two objects */
    m->method_body_index = c->index;
    c->method = m;

    return c;
}

abc_code_t* abc_AddConstructor(abc_class_t*cls, char*returntype, int num_params, ...) 
{
    va_list va;
    va_start(va, num_params);
    abc_code_t* c = add_method(cls->abc, cls, returntype, 0, num_params, va);
    va_end(va);
    cls->iinit = c->index;
    return c;
}

abc_code_t* abc_AddStaticConstructor(abc_class_t*cls, char*returntype, int num_params, ...) 
{
    va_list va;
    va_start(va, num_params);
    abc_code_t* c = add_method(cls->abc, cls, returntype, 0, num_params, va);
    va_end(va);
    cls->cinit = c->index;
    return c;
}

abc_trait_t*trait_new(int type, int name_index, int data1, int data2, int vindex, int vkind)
{
    abc_trait_t*trait = malloc(sizeof(abc_trait_t));
    memset(trait, 0, sizeof(abc_trait_t));
    trait->type = type;
    trait->name_index = name_index;
    trait->data1 = data1;
    trait->data2 = data2;
    trait->vindex = vindex;
    trait->vkind = vkind;
    return trait;
}

abc_code_t* abc_AddMethod(abc_class_t*cls, char*returntype, char*name, int num_params, ...) 
{
    abc_file_t*abc = cls->abc;
    va_list va;
    va_start(va, num_params);
    abc_code_t* c = add_method(cls->abc, cls, returntype, name, num_params, va);
    va_end(va);
    dict_append(cls->traits, 0, trait_new(TRAIT_METHOD, multiname_index(abc, name), 0, c->method->index, 0, 0));
    return c;
}

void abc_AddSlot(abc_class_t*cls, char*name, int slot, char*multiname)
{
    abc_file_t*abc = cls->abc;
    int i = multiname_index(abc, name);
    dict_append(cls->traits, 0, trait_new(TRAIT_SLOT, i, slot, multiname_index(abc, multiname), 0, 0));
}

void abc_code_addClassTrait(abc_code_t*code, char*multiname, int slotid, abc_class_t*cls)
{
    abc_file_t*abc = code->abc;
    int i = multiname_index(abc, multiname);
    abc_trait_t*trait = trait_new(TRAIT_CLASS, i, slotid, cls->index, 0, 0);
    dict_append(code->traits, 0, trait);
}

abc_code_t* abc_AddInitScript(abc_file_t*abc, char*returntype, int num_params, ...) 
{
    va_list va;
    va_start(va, num_params);
    abc_code_t* c = add_method(abc, 0, returntype, 0, num_params, va);
    dict_append(abc->scripts, 0, c);
    va_end(va);
    return c;
}

void swf_SetU30(TAG*tag, U32 u)
{
    do {
	swf_SetU8(tag, (u&~0x7f?0x80:0) | (u&0x7F));
	u>>=7;
    } while(u);
}
void swf_SetU30String(TAG*tag, const char*str)
{
    int l = strlen(str);
    swf_SetU30(tag, l);
    swf_SetBlock(tag, (void*)str, l);
}

static void write_traits(abc_file_t*abc, TAG*tag, dict_t*traits)
{
    swf_SetU30(tag, traits->num);
    int s;

    for(s=0;s<traits->num;s++) {
	abc_trait_t*trait = (abc_trait_t*)dict_getdata(traits, s);
	swf_SetU30(tag, trait->name_index);
	swf_SetU8(tag, trait->type);
	swf_SetU30(tag, trait->data1);
	swf_SetU30(tag, trait->data2);
	if(trait->type == 0) { //slot
	    swf_SetU30(tag, trait->vindex);
	    if(trait->vindex) {
		swf_SetU8(tag, trait->vkind);
	    }
	}
    }
}

void swf_WriteABC(TAG*tag, abc_file_t*abc)
{
    swf_SetU32(tag, 1);
    swf_SetU8(tag, 0);
    swf_SetU16(tag, 0x10);
    swf_SetU16(tag, 0x2e);
    swf_SetU30(tag, abc->ints->num>1?abc->ints->num:0);
    // ...
    swf_SetU30(tag, abc->uints->num>1?abc->uints->num:0);
    // ...
    swf_SetU30(tag, abc->floats->num>1?abc->floats->num:0);
    // ...
    swf_SetU30(tag, abc->strings->num>1?abc->strings->num:0);
    int t;
    for(t=1;t<abc->strings->num;t++) {
	swf_SetU30String(tag, dict_getstr(abc->strings, t));
    }
    swf_SetU30(tag, abc->namespaces->num>1?abc->namespaces->num:0);
    for(t=1;t<abc->namespaces->num;t++) {
	U8 type = (U8)(int)dict_getdata(abc->namespaces, t);
	const char*name = dict_getstr(abc->namespaces, t);
	int i = dict_find(abc->strings, name);
	if(i<0) {
	    fprintf(stderr, "Couldn't find namespace \"%s\" in constant pool\n", name);
	    return;
	}
	swf_SetU8(tag, type);
	swf_SetU30(tag, i);
    }
    swf_SetU30(tag, abc->sets->num>1?abc->sets->num:0);
    // ...

    swf_SetU30(tag, abc->multinames->num>1?abc->multinames->num:0);
    // ...
    for(t=1;t<abc->multinames->num;t++) {
	abc_multiname_t*m = (abc_multiname_t*)dict_getdata(abc->multinames, t);
	swf_SetU8(tag, 0x07);
	swf_SetU30(tag, m->namespace_index);
	swf_SetU30(tag, m->name_index);
    }
    
    swf_SetU30(tag, abc->methods->num);
    for(t=0;t<abc->methods->num;t++) {
	abc_method_t*m = (abc_method_t*)dict_getdata(abc->methods, t);
	swf_SetU30(tag, m->param_count);
	swf_SetU30(tag, m->return_type_index);
	int s;
	for(s=0;s<m->param_count;s++) {
	    swf_SetU30(tag, m->params[s]);
	}
	swf_SetU30(tag, 0); // name
	swf_SetU8(tag, 0); //flags
    }

    swf_SetU30(tag, 0);//metadata

    swf_SetU30(tag, abc->classes->num);

    for(t=0;t<abc->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)dict_getdata(abc->classes, t);
	swf_SetU30(tag, c->classname_index);
	swf_SetU30(tag, c->superclass_index);

	swf_SetU8(tag, 8); // flags
	swf_SetU30(tag, c->ns_index);

	swf_SetU30(tag, 0); // no interfaces
	if(c->iinit<0) {
	    fprintf(stderr, "Error: Class %s has no constructor\n", c->name);
	    return;
	}
	swf_SetU30(tag, c->iinit);
	write_traits(abc, tag, c->traits);
    }
    for(t=0;t<abc->classes->num;t++) {
	abc_class_t*c = (abc_class_t*)dict_getdata(abc->classes, t);
	if(c->cinit<0) {
	    fprintf(stderr, "Error: Class %s has no constructor\n", c->name);
	    return;
	}
	swf_SetU30(tag, c->cinit);
	swf_SetU30(tag, 0); // no traits
    }

    swf_SetU30(tag, abc->scripts->num);
    for(t=0;t<abc->scripts->num;t++) {
	abc_code_t*c = (abc_code_t*)dict_getdata(abc->scripts, t);
	swf_SetU30(tag, c->index); //!=t!
	write_traits(abc, tag, c->traits);
    }

    swf_SetU30(tag, abc->method_bodies->num);
    for(t=0;t<abc->method_bodies->num;t++) {
	abc_code_t*c = (abc_code_t*)dict_getdata(abc->method_bodies, t);
	abc_method_t*m = c->method;
	swf_SetU30(tag, m->index);
	swf_SetU30(tag, c->max_stack);
	swf_SetU30(tag, c->local_count);
	swf_SetU30(tag, c->init_scope_depth);
	swf_SetU30(tag, c->max_scope_depth);
	swf_SetU30(tag, c->tag->len);
	swf_SetBlock(tag, c->tag->data, c->tag->len);
	swf_SetU30(tag, c->exception_count);
	swf_SetU8(tag, 0); // no traits
    }
}

#include "swfabc_ops.c"

void swf_AddButtonLinks(TAG*tag)
{
    abc_file_t*abc = abc_file_new();
    abc_code_t*c = 0;
    
    abc_class_t*maintimeline = abc_NewClass(abc, "buttonmitlink_fla:MainTimeline", "flash.display:MovieClip");
    
    c = abc_AddStaticConstructor(maintimeline, 0, 0);
    c->max_stack = 1;
    c->local_count = 1;
    c->init_scope_depth = 9;
    c->max_scope_depth = 10;
    abc_getlocal_0(c);
    abc_pushscope(c);
    abc_returnvoid(c);
    
    c = abc_AddMethod(maintimeline, 0, "[packageinternal]buttonmitlink_fla:frame1", 0);
    c->max_stack = 3;
    c->local_count = 1;
    c->init_scope_depth = 10;
    c->max_scope_depth = 11;
    abc_getlocal_0(c);
    abc_pushscope(c);
    abc_returnvoid(c);
    
    abc_AddSlot(maintimeline, ":MyButton1", 0, "flash.display:SimpleButton");
    abc_AddSlot(maintimeline, ":MyButton2", 0, "flash.display:SimpleButton");

    c = abc_AddMethod(maintimeline, ":void", ":gotoPage2", 1, "flash.events:MouseEvent");
    c->max_stack = 3;
    c->local_count = 2;
    c->init_scope_depth = 10;
    c->max_scope_depth = 11;
    abc_getlocal_0(c);
    abc_pushscope(c);
    abc_findpropstrict(c, "flash.net:navigateToURL");
    abc_findpropstrict(c, "flash.net:URLRequest");
    abc_pushstring(c, "http://www.quiss.org");
    abc_constructprop(c, "flash.net:URLRequest", 1);
    abc_callpropvoid(c, "flash.net:navigateToURL", 1);
    abc_returnvoid(c);
    
    c = abc_AddMethod(maintimeline, ":void", ":gotoPage1", 1, "flash.events:MouseEvent");
    c->max_stack = 3;
    c->local_count = 2;
    c->init_scope_depth = 10;
    c->max_scope_depth = 11;
    abc_getlocal_0(c);
    abc_pushscope(c);
    abc_findpropstrict(c,"flash.net:navigateToURL");
    abc_findpropstrict(c,"flash.net:URLRequest");
    abc_pushstring(c,"http://www.google.com/");
    abc_constructprop(c,"flash.net:URLRequest", 1);
    abc_callpropvoid(c,"flash.net:navigateToURL", 1);
    abc_returnvoid(c);
    
    c = abc_AddConstructor(maintimeline, 0, 0);
    c->max_stack = 3;
    c->local_count = 1;
    c->init_scope_depth = 10;
    c->max_scope_depth = 11;

    abc_getlocal_0(c);
    abc_pushscope(c);

    abc_getlocal_0(c);
    abc_constructsuper(c,0);
    abc_findpropstrict(c,":addFrameScript");
    abc_pushbyte(c,0x00);
    abc_getlex(c,"[packageinternal]buttonmitlink_fla:frame1");
    abc_callpropvoid(c,":addFrameScript",2);

    abc_getlex(c,":MyButton1");
    abc_getlex(c,"flash.events:MouseEvent");
    abc_getproperty(c, ":CLICK");
    abc_getlex(c, ":gotoPage1");
    abc_callpropvoid(c, ":addEventListener" ,2);

    abc_getlex(c,":MyButton2");
    abc_getlex(c,"flash.events:MouseEvent");
    abc_getproperty(c, ":CLICK");
    abc_getlex(c,":gotoPage2");
    abc_callpropvoid(c,":addEventListener",2);

    abc_returnvoid(c);
    
    c = abc_AddInitScript(abc, 0, 0);
    c->max_stack = 2;
    c->local_count = 1;
    c->init_scope_depth = 1;
    c->max_scope_depth = 9;
    abc_getlocal_0(c);
    abc_pushscope(c);
    abc_getscopeobject(c, 0);
    abc_getlex(c,":Object");
    abc_pushscope(c);
    abc_getlex(c,"flash.events:EventDispatcher");
    abc_pushscope(c);
    abc_getlex(c,"flash.display:DisplayObject");
    abc_pushscope(c);
    abc_getlex(c,"flash.display:InteractiveObject");
    abc_pushscope(c);
    abc_getlex(c,"flash.display:DisplayObjectContainer");
    abc_pushscope(c);
    abc_getlex(c,"flash.display:Sprite");
    abc_pushscope(c);
    abc_getlex(c,"flash.display:MovieClip");
    abc_pushscope(c);
    abc_getlex(c,"flash.display:MovieClip");
    abc_newclass(c,maintimeline);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_popscope(c);
    abc_initproperty(c,"buttonmitlink_fla:MainTimeline");
    abc_returnvoid(c);

    abc_code_addClassTrait(c, "buttonmitlink_fla:MainTimeline", 1, maintimeline);

    swf_WriteABC(tag, abc);
}
