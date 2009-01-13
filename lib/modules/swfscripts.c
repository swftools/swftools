/* swfscripts.c

   AVM2 Utility Actionscripts

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




