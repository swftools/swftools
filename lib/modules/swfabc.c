/* swfbits.c

   Bitmap functions (needs libjpeg) 

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2007 Alessandro Molina
   Copyright (c) 2007 Matthias Kramm <kramm@quiss.org>
 
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

#include "../rfxswf.h"

static unsigned AVM2_uint32toU30(unsigned val, char * out)
{
    unsigned len = 0;

    while(val) {
	  out[len++] = val & 0xFF;
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

    char pool2[26] = {0x01,0x00,0x00,0x00,0x73,0x74,0x6f,0x70,0x5f,0x66,0x6c,0x61,0x2e,0x4d,0x61,0x69
    ,0x6e,0x54,0x69,0x6d,0x65,0x6c,0x69,0x6e,0x65,0x00};
    TAG * classCall = swf_InsertTagBefore(swf, swf->firstTag, 0x04c);
    swf_SetBlock(classCall, (U8*)pool2, sizeof(pool2));

    //0x52 is the Flash9 equivalent to DoAction
    char init_pool[322] = { 0x01,0x00,0x00,0x00, /* Flag Parameter to 0x52 */
		       0x00,                /* 0 terminated class name */
		       0x10,0x00,0x2e,0x00, /* ABC file magic number */
		       0x00,0x00,0x00,0x11, /* Constant Pool dec:
    0 Integers, 0 Unsigned Integers, 0 Doubles, 17 Strings */

      /* HERE FOLLOW THE 16 STRINGS OF THE CPOOL */
      0x08,                            /* Len of 'stop_fla' (8) */
      's','t','o','p','_','f','l','a', 
      0x0c,                            /* Len of 'MainTimeline' */
      'M','a','i','n','T','i','m','e','l','i','n','e',
      0x0d,                            /* Len of 'flash.display' (13) */
      'f','l','a','s','h','.','d','i','s','p','l','a','y', 
      0x09,                                                    /* Len of 'MovieClip' (9) */
      'M','o','v','i','e','C','l','i','p',
      0x15,                                                    /* Len of 'stop_fla:MainTimeLine' (21) */
      's','t','o','p','_','f','l','a',':','M','a','i','n','T','i','m','e','l','i','n','e',
      0x06,                                                    /* Len of 'dostop' (6) */
      'd','o','s','t','o','p',
      0x00,   /* Empty string: ref. to the global namespace */
      0x04,                                                    /* Len of 'stop' (4) */
      's','t','o','p',
      0x0e,                                                    /* Len of 'addFrameScript' (14) */
      'a','d','d','F','r','a','m','e','S','c','r','i','p','t',
      0x06,                                                    /* Len of 'Object' (6) */
      'O','b','j','e','c','t',
      0x0c,                                                    /* Len of 'flash.events' (12) */
      'f','l','a','s','h','.','e','v','e','n','t','s',
      0x0f,                                                    /* Len of 'EventDispatcher' (15) */
      'E','v','e','n','t','D','i','s','p','a','t','c','h','e','r',
      0x0d,                                                    /* Len of 'DisplayObject' (13) */
      'D','i','s','p','l','a','y','O','b','j','e','c','t',
      0x11,                                                    /* Len of 'InteractiveObject' (17) */
      'I','n','t','e','r','a','c','t','i','v','e','O','b','j','e','c','t',
      0x16,                                                    /* Len of 'DisplayObjectContainer' (22) */
      'D','i','s','p','l','a','y','O','b','j','e','c','t','C','o','n','t','a','i','n','e','r',
      0x06,                                                    /* Len of 'Sprite' (6) */
      'S','p','r','i','t','e',

      0x07, /* No. of NameSpaces (+1) */
      /* HERE FOLLOW THE 6 NAMESPACES OF THE CPOOL*/
      0x16,0x01, /* kind: CONSTANT_PackageNamespace, ref. to: stop_fla */
      0x16,0x03, /* kind: CONSTANT_PackageNamespace, ref. to: flash.display */
      0x18,0x05, /* kind: CONSTANT_ProtectedNamespace, ref. to: stop_fla:MainTimeline */
      0x17,0x01, /* kind: CONSTANT_PackageInternalNs, ref. to: MainTimeline */
      0x16,0x07, /* kind: CONSTANT_PackageNamespace, ref. to the global namespace */
      0x16,0x0b, /* kind: CONSTANT_PackageNamespace, ref. to: flash.events */
			      
      0x00, /* NO NAMESPACE_SET */
			      
      0x0c, /* No. of MultiNames (+1) */
      /* HERE FOLLOW THE 11 MULTINAMES OF THE CPOOL */
      /*      1st value: Multiname kind (0x07 = Qualified Name, a multiname with 1 namespace, refers to a method)
		      2nd value: refers to namespace name index in the namespace table
		      3rd value: refers to method name index in the string table */
      0x07,0x01,0x02, /* Ref. to method: stop_fla:MainTimeLine */
      0x07,0x02,0x04, /* Ref. to method: flash.display:MovieClip */
      0x07,0x04,0x06, /* Ref. to method: <4th namespace>:dostop */
      0x07,0x05,0x08, /* Ref. to method: <global>:stop */
      0x07,0x05,0x09, /* Ref. to method: <global>:addFrameScript */
      0x07,0x05,0x0a, /* Ref. to method: <global>:Object */
      0x07,0x06,0x0c, /* Ref. to method: flash.events:EventDispatcher */
      0x07,0x02,0x0d, /* Ref. to method: flash.display:DisplayObject */
      0x07,0x02,0x0e, /* Ref. to method: flash.display:InteractiveObject */
      0x07,0x02,0x0f, /* Ref. to method: flash.display:DisplayObjectContainer */
      0x07,0x02,0x10, /* Ref. to method: flash.display:Sprite */
      /* END OF THE CPOOL */  
			      
      0x04,   /* Method count: there are 4 methods */
      /* HERE FOLLOW THE 4 METHODS SIGNATURES */              
      /* METHOD 1 */
      0x00, /* No params (nor params types)*/
      0x00, /* Any return type */
      0x00, /* No method name */
      0x00, /* Flags */
      /* SAME FOR THE OTHER METHODS */
      0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,

      0x00,   /* Metadata count: there is no metadata (nor metadata_info) */

      0x01,   /* Class count: there is 1 class */
      /* HERE FOLLOW THE INSTANCE DATA */
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

      0x01, /* Script count: there is 1 script */
      /* HERE FOLLOW THE SCRIPT DATA */
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

    TAG *classInit = swf_InsertTagBefore(swf, swf->firstTag, 0x052);

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

