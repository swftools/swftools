/* swfaction.c

   Actionscript generation and parsing routines
   
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#include "../rfxswf.h"
#include "../args.h"

struct Action
{
    int version;
    char*name;
    U8 op;
    char*flags;
} static actions[] =
{
/*
f: frame (word)
u: url (string)
t: target (string)
l: label (string)
c: constant pool (string)
s: skip (byte) (number of actions)
m: method (byte) url:(0=none, 1=get, 2=post)/gf2:(1=play)
b: branch (word) (number of bytes)
p: type(byte), type=0:string, type=1:double
{: define function (name (string), num (word), params (num strings), codesize (word)
o: object (string)
r: register (byte)
 */
{3,"End", 0x00, ""},
{3,"GotoFrame", 0x81, "f"},
{4,"GotoFrame2", 0x9f, "m"}, // -1 (/Movieclip:3)
{3,"GetUrl", 0x83, "ul"},
{4,"GetUrl2", 0x9a, "m"}, //-2
{3,"NextFrame", 0x04, ""},
{3,"PreviousFrame", 0x05, ""},
{3,"Play", 0x06, ""},
{3,"Stop", 0x07, ""},
{3,"ToggleQuality", 0x08, ""},
{3,"StopSounds", 0x09, ""},
{3,"WaitForFrame", 0x8a, "fs"},
{4,"WaitForFrame2", 0x8d, "s"}, // -1
{3,"SetTarget", 0x8b, "t"},
{4,"SetTarget2", 0x20, ""}, //-1
{3,"GotoLabel", 0x8c, "l"},
{4,"Add", 0x0a, ""}, //  -2, +1
{4,"Multiply", 0x0c, ""}, //  -2, +1
{4,"Divide", 0x0d, ""}, //  -2, +1
{4,"Subtract", 0x0b, ""}, //  -2, +1
{4,"Less", 0x0f, ""}, //  -2, +1
{4,"Equals", 0x0e, ""}, //  -2, +1
{4,"And", 0x10, ""}, //  -2, +1
{4,"Or", 0x11, ""}, //  -2, +1
{4,"Not", 0x12, ""}, //  -1, +1
{4,"StringAdd", 0x21, ""}, // -2,+1
{4,"StringLength", 0x14, ""}, // -1, +1
{4,"MBStringLength", 0x31}, // -1, +1
{4,"StringEquals", 0x13, ""}, // -2, +1
{4,"StringLess", 0x29, ""}, //-2, +1
{4,"StringExtract", 0x15, ""}, // -3, +1
{4,"MBStringExtract", 0x35, ""}, //-3 +1
{4,"Push", 0x96, "p"}, //  +1
{4,"Pop", 0x17, ""}, //  -1
{4,"ToInteger", 0x18, ""}, // -1, +1
{4,"CharToAscii", 0x32, ""}, // -1, +1
{4,"AsciiToChar", 0x33, ""}, // -1, +1
{4,"MBCharToAscii", 0x36, ""}, // -1, +1
{4,"MBAsciiToChar", 0x37, ""}, // -1, +1
{4,"Jump", 0x99, "b"},
{4,"If", 0x9d, "b"}, // -1
{4,"Call", 0x9e, ""}, //-1 (frame label/number)
{4,"GetVariable", 0x1c,""}, // -1, +1
{4,"SetVariable", 0x1d,""}, // -2
{4,"GetProperty", 0x22,""}, //-2, +1
{4,"SetProperty", 0x23, ""}, // -3
{4,"RemoveSprite", 0x25, ""}, //-1
{4,"StartDrag", 0x27, ""}, // -2, -1, (-4)
{4,"EndDrag", 0x28, ""}, 
{4,"CloneSprite", 0x24, ""}, // -3
{4,"Trace", 0x26, ""}, //-1
{4,"GetTime", 0x34, ""}, //+1
{4,"RandomNumber", 0x30, ""}, //-1,+1

{5,"Modulo", 0x3f,""},
{5,"BitAnd", 0x60,""},
{5,"BitLShift", 0x63,""},
{5,"BitOr", 0x61,""},
{5,"BitRShift", 0x64,""},
{5,"BitURShift", 0x65,""},
{5,"BitXor", 0x62,""},//66?
{5,"Decrement", 0x51,""},
{5,"Increment", 0x50,""},
{5,"PushDuplicate", 0x4c,""},
{5,"StackSwap", 0x4d,""}, //?
{5,"StoreRegister", 0x87,"r"},
{5,"CallFunction", 0x3d,""},
{5,"DefineFunction", 0x9b, "{"},
{5,"Return", 0x3e,""},
{5,"GetMember", 0x4e,""},
{5,"SetMember", 0x4f,""},
{5,"CallMethod", 0x52,""},
{5,"Constantpool", 0x88, "c"},
{5,"DefineLocal", 0x3c,""},
{5,"DefineLocal2", 0x41,""},
{5,"Makehash", 0x43, ""}, //??
{5,"Delete", 0x3a,""}, //?
{5,"Delete2", 0x3b,""},
{5,"Enumerate", 0x46,""},
{5,"Equals2", 0x49,""},
{5,"InitArray", 0x42,""}, // InitObject?
{5,"NewMethod", 0x53,""}, //?
{5,"NewObject", 0x40,""},
{5,"TargetPath", 0x45,""}, //?
{5,"With", 0x94, "o"},
{5,"ToNumber", 0x4a,""}, //?
{5,"ToString", 0x4b,""}, //?
{5,"TypeOf", 0x44,""},
{5,"Add2", 0x47,""},
{5,"Less2", 0x48,""}
};
static int definedactions = sizeof(actions)/sizeof(struct Action);

ActionTAG* GetActions(TAG*tag) 
{
    U8 op = 1;
    int length;
    ActionTAG tmp;
    ActionTAG*atag = &tmp;
    U8*data;
    while(op)
    {
	int pos;
	atag->next = (ActionTAG*)malloc(sizeof(ActionTAG));
	atag->next->prev = atag;
	atag->next->next = 0;
	atag = atag->next;

	op = GetU8(tag);
	if(op<0x80)
	    length = 0;
	else
	    length = GetU16(tag);

	if(length) {
	    int t;
	    data = malloc(length);
	    for(t=0;t<length;t++)
		data[t] = GetU8(tag);
	} else {
	  data = 0;
	}
	atag->op = op;
	atag->len = length;
	atag->data = data;
    }
    return tmp.next;
}

/* TODO: this should be in swfdump.c */
void DumpActions(ActionTAG*atag, char*prefix) 
{
    U8 op;
    int t;
    U8*data;
    char* cp;
    if(!prefix) 
	prefix="";
    while(atag)
    {
	for(t=0;t<definedactions;t++)
	    if(actions[t].op == atag->op)
		break;

	if(t==definedactions) {
	    printf("%s (%5d bytes) action: %02x\n", prefix, atag->len, op);
	    atag = atag->next;
	    continue;
	}
	printf("%s (%5d bytes) action: %s", prefix, atag->len, actions[t].name);
	cp = actions[t].flags;
	data = atag->data;
	if(atag->len) //TODO: check for consistency: should we have a length?
	while(*cp)
	{
	    switch(*cp)
	    {
		case 'f': {
		    printf(" %d", *(U16*)data);data+=2; //FIXME: le/be
		} break;
		case 'u': {
		    printf(" URL:\"%s\"", data);data+=strlen(data)+1;
		} break;
		case 't': {
		    printf(" Target:\"%s\"", data);data+=strlen(data)+1;
		} break;
		case 'l': {
		    printf(" Label:\"%s\"", data);data+=strlen(data)+1;
		} break;
		case 'c': {
		    printf(" Constant Pool:\"%s\"", data);data+=strlen(data)+1;
	    	} break;
		case 's': {
		    printf(" +%d", data);data++;
		} break;
		case 'm': {
//m: method (byte) url:(0=none, 1=get, 2=datat)/gf2:(1=play)
		    printf(" %d", data);data++;
		} break;
		case 'b': {
		    printf(" %d", *(U16*)data);data+=2;
		} break;
		case 'p': {
		    U8 type = *data++;
		    if(type == 0) {
			printf(" String:\"%s\"", data);data+=strlen(data)+1;
		    } else if (type == 1) {
			printf(" Double:\"%f\"", *(float*)data);data+=4;
		    } else if (type == 2) {
			printf(" NULL");
		    } else if (type == 4) {
			printf(" register:%d", data++);
		    } else if (type == 5) {
			printf(" %s", data++?"true":"false");
		    } else if (type == 6) {
			printf(" %f", *(double*)data);  data+=8;
		    } else if (type == 7) {
			printf(" %d", *(int*)data); data+=4;
		    } else if (type == 8) {
			printf(" Lookup:%d", data++);
		    }
		} break;
	    
	    }
	    cp++;
	}

	if(data < atag->data + atag->len)
	{
	    int nl = ((atag->data+atag->len)-data);
	    int t;
	    printf(" remainder of %d bytes:\"", nl);
	    for(t=0;t<nl;t++) {
		if(data[t]<32)
		    printf("\\%d",data[t]);
		else
		    printf("%c", data[t]);
	    }
	    printf("\"");
	}
	printf("\n");
	atag = atag->next;
    }
}

