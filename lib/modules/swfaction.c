/* swfaction.c

   Actionscript generation and parsing routines
   
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#include "../rfxswf.h"

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
C: constant pool header (byte)
c: constant pool entry (string)
s: skip (byte) (number of actions)
m: method (byte) swf_GetUrl2:(0=none, 1=get, 2=post)/GotoFrame2:(1=play)
b: branch (word) (number of bytes)
p (push): type(byte), type=0:string, type=1:double
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
{4,"MBStringLength", 0x31, ""}, // -1, +1
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
{4,"Call", 0x9e, ""}, //-1 (frame label/number) (high bit is wrong.)
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
{5,"Constantpool", 0x88, "Cc"},
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

ActionTAG* swf_GetActions(TAG*tag) 
{
    U8 op = 1;
    int length;
    ActionTAG tmp;
    ActionTAG*action = &tmp;
    U8*data;
    while(op)
    {
	action->next = (ActionTAG*)malloc(sizeof(ActionTAG));
	action->next->prev = action;
	action->next->next = 0;
	action = action->next;

	op = swf_GetU8(tag);
	if(op<0x80)
	    length = 0;
	else
	    length = swf_GetU16(tag);

	if(length) {
	    int t;
	    data = malloc(length);
	    for(t=0;t<length;t++)
		data[t] = swf_GetU8(tag);
	} else {
	  data = 0;
	}
	action->op = op;
	action->len = length;
	action->data = data;
	action->parent = tag;
    }
    return tmp.next;
}

void swf_SetActions(TAG*tag, ActionTAG*action)
{
    while(action)
    {
	swf_SetU8(tag, action->op);
	if(action->op & 128)
	  swf_SetU16(tag, action->len);

	swf_SetBlock(tag, action->data, action->len);

	action = action->next;
    }
}

int OpAdvance(char c, char*data)
{
    switch (c)
    {
	case 'f':
	    return 2;
	case 'u':
	    return strlen(data)+1;
	case 't':
	    return strlen(data)+1;
	case 'l': 
	    return strlen(data)+1;
	case 'c': 
	    return strlen(data)+1;
	case 'C': 
	    return 2;
	case 's':
	    return 1;
	case 'm':
	    return 1;
	case 'b':
	    return 2;
	case 'p': {
	    U8 type = *data++;
	    if(type == 0) {
		return 1+strlen(data)+1; //string
	    } else if (type == 1) {
		return 1+4; //float
	    } else if (type == 2) {
		return 1+0; //NULL
	    } else if (type == 4) {
		return 1+1; //register
	    } else if (type == 5) {
		return 1+1; //bool
	    } else if (type == 6) {
		return 1+8; //double
	    } else if (type == 7) {
		return 1+4; //int
	    } else if (type == 8) {
		return 1+1; //lookup
	    }
	    break;
	}
    }
    return 0;
}

/* TODO: this should be in swfdump.c */
void swf_DumpActions(ActionTAG*atag, char*prefix) 
{
    int t;
    U8*data;
    char* cp;
    if(!prefix) 
	prefix="";
    while(atag)
    {
	U8 poollen = 0;
	for(t=0;t<definedactions;t++)
	    if(actions[t].op == atag->op)
		break;

	if(t==definedactions) {
	    printf("%s (%5d bytes) action: %02x\n", prefix, atag->len, atag->op);
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
		    printf(" %d", *(U16*)data); //FIXME: le/be
		} break;
		case 'u': {
		    printf(" URL:\"%s\"", data);
		} break;
		case 't': {
		    printf(" Target:\"%s\"", data);
		} break;
		case 'l': {
		    printf(" Label:\"%s\"", data);
		} break;
		case 'c': {
		    printf(" String:\"%s\"", data);
	        } break;
		case 'C': {
		    poollen = *data;
		    printf("(%d entries)", poollen);
	    	} break;
		case 's': {
		    printf(" +%d", *data);
		} break;
		case 'm': {
		    //m: method (byte) url:(0=none, 1=get, 2=datat)/gf2:(1=play)
		    printf(" %d", *data);
		} break;
		case 'b': {
		    printf(" %d", *(U16*)data);
		} break;
		case 'p': {
		    U8 type = *data;
		    char*value = data+1;
		    if(type == 0) {
			printf(" String:\"%s\"", value);
		    } else if (type == 1) {
			printf(" Float:\"%f\"", *(float*)value);
		    } else if (type == 2) {
			printf(" NULL");
		    } else if (type == 4) {
			printf(" register:%d", *value);
		    } else if (type == 5) {
			printf(" %s", *value?"true":"false");
		    } else if (type == 6) {
			printf(" %f", *(double*)value);
		    } else if (type == 7) {
			printf(" %d", *(int*)value);
		    } else if (type == 8) {
			printf(" Lookup:%d", *value);
		    }
		} break;
	    }
	    data += OpAdvance(*cp, data);
	    if(*cp!='c' || !poollen)
		cp++;
	    if(poollen)
		poollen--;
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

static const char TYPE_URL = 1;
static const char TYPE_TARGET = 2;
static const char TYPE_STRING = 4;

int swf_ActionEnumerate(ActionTAG*atag, char*(*callback)(char*), int type)
{
    int t;
    U8*data;
    char* cp;
    int count = 0;
    while(atag)
    {
	U8 poollen = 0;
	for(t=0;t<definedactions;t++)
	    if(actions[t].op == atag->op)
		break;

	if(t==definedactions) {
	    // unknown actiontag
	    atag = atag->next;
	    count++;
	    continue;
	}
	cp = actions[t].flags;
	data = atag->data;
	if(atag->len) {
	    while(*cp) {
		U8 * replacepos = 0;
		int replacelen = 0;
		U8 * replacement = 0;
		switch(*cp)
		{
		    case 'u': {
			if(type&TYPE_URL)
			{
			    replacelen = strlen(data);
			    replacepos = data;
			    replacement = callback(data); // may be null
			}
		    } break;
	    	    case 't': {
			if(type&TYPE_TARGET)
			{
			    replacelen = strlen(data);
			    replacepos = data;
			    replacement = callback(data); // may be null
			}
		    } break;
		    case 'c': {
		    	if(type&TYPE_STRING)
			{
			    replacelen = strlen(data);
			    replacepos = data;
			    replacement = callback(data); // may be null
			}
		    } break;
		    case 'C': {
			poollen = (*data);
		    } break;
		    case 'o': {
		    } break;
		    case 'p': {
			U8 datatype = *data;
			char*value = &data[1];
			if(datatype == 0) { //string
			    if(type&TYPE_STRING)
			    {
				replacelen = strlen(value);
				replacepos = value;
				replacement = callback(value); // may be null
			    }
			} else if (datatype == 8) { //lookup
			}
		    } break;
		}
		data += OpAdvance(*cp, data);
		if(*cp!='c' || !poollen)
		    cp++;
		if(poollen)
		    poollen--;

		if(replacement)
		{
		    int newlen = strlen(replacement);
		    char * newdata = malloc(atag->len - replacelen + newlen);
		    int rpos = replacepos - atag->data;
		    memcpy(newdata, atag->data, rpos);
		    memcpy(&newdata[rpos], replacement, newlen);
		    memcpy(&newdata[rpos+newlen], &replacepos[replacelen],
			    &data[atag->len] - &replacepos[replacelen]);
		    free(atag->data);
		    atag->data = newdata;
		    data = &atag->data[rpos+newlen+1];
		}
	    }
	}
	atag = atag->next;
	count ++;
    }
    return count;
}

void swf_ActionEnumerateTargets(ActionTAG*atag, char*(*callback)(char*))
{
    swf_ActionEnumerate(atag, callback, TYPE_TARGET);
}
void swf_ActionEnumerateStrings(ActionTAG*atag, char*(*callback)(char*))
{
    swf_ActionEnumerate(atag, callback, TYPE_STRING);
}
void swf_ActionEnumerateURLs(ActionTAG*atag, char*(*callback)(char*))
{
    swf_ActionEnumerate(atag, callback, TYPE_URL);
}

static ActionTAG * currentatag;

ActionTAG* swf_ActionStart()
{
    currentatag = (ActionTAG*)malloc(sizeof(ActionTAG));
    currentatag->prev = 0;
    currentatag->parent = 0;
    currentatag->data = 0;
    currentatag->len = 0;
    return currentatag;
}

void swf_ActionEnd()
{
    currentatag->prev->next = 0;
    free(currentatag);
}

void swf_AddActionTAG(U8 op, U8*data, U16 len)
{
    currentatag->next = (ActionTAG*)malloc(sizeof(ActionTAG));
    currentatag->next->prev = currentatag;
    currentatag->parent = 0;
    currentatag->data = data;
    currentatag->len = len;
    currentatag->op = op;
    currentatag = currentatag->next;
}

#define ACTION_END            0x00
#define ACTION_NEXTFRAME      0x04
#define ACTION_PREVIOUSFRAME  0x05
#define ACTION_PLAY           0x06
#define ACTION_STOP           0x07
#define ACTION_TOGGLEQUALITY  0x08
#define ACTION_STOPSOUNDS     0x09
#define ACTION_ADD            0x0a
#define ACTION_SUBTRACT       0x0b
#define ACTION_MULTIPLY       0x0c
#define ACTION_DIVIDE         0x0d
#define ACTION_EQUALS         0x0e
#define ACTION_LESS           0x0f
#define ACTION_AND            0x10
#define ACTION_OR             0x11
#define ACTION_NOT            0x12
#define ACTION_STRINGEQUALS   0x13
#define ACTION_STRINGLENGTH   0x14
#define ACTION_STRINGEXTRACT  0x15
#define ACTION_POP            0x17
#define ACTION_TOINTEGER      0x18
#define ACTION_GETVARIABLE    0x1c
#define ACTION_SETVARIABLE    0x1d
#define ACTION_SETTARGET2     0x20
#define ACTION_STRINGADD      0x21
#define ACTION_GETPROPERTY    0x22
#define ACTION_SETPROPERTY    0x23
#define ACTION_CLONESPRITE    0x24
#define ACTION_REMOVESPRITE   0x25
#define ACTION_TRACE          0x26
#define ACTION_STARTDRAG      0x27
#define ACTION_ENDDRAG        0x28
#define ACTION_STRINGLESS     0x29
#define ACTION_RANDOMNUMBER   0x30
#define ACTION_MBSTRINGLENGTH 0x31
#define ACTION_CHARTOASCII    0x32
#define ACTION_ASCIITOCHAR    0x33
#define ACTION_GETTIME        0x34
#define ACTION_MBSTRINGEXTRACT 0x35
#define ACTION_MBCHARTOASCII  0x36
#define ACTION_MBASCIITOCHAR  0x37
#define ACTION_DELETE         0x3a
#define ACTION_DELETE2        0x3b
#define ACTION_DEFINELOCAL    0x3c
#define ACTION_CALLFUNCTION   0x3d
#define ACTION_RETURN         0x3e
#define ACTION_MODULO         0x3f
#define ACTION_NEWOBJECT      0x40
#define ACTION_DEFINELOCAL2   0x41
#define ACTION_INITARRAY      0x42
#define ACTION_MAKEHASH       0x43
#define ACTION_TYPEOF         0x44
#define ACTION_TARGETPATH     0x45
#define ACTION_ENUMERATE      0x46
#define ACTION_ADD2           0x47
#define ACTION_LESS2          0x48
#define ACTION_EQUALS2        0x49
#define ACTION_TONUMBER       0x4a
#define ACTION_TOSTRING       0x4b
#define ACTION_PUSHDUPLICATE  0x4c
#define ACTION_STACKSWAP      0x4d
#define ACTION_GETMEMBER      0x4e
#define ACTION_SETMEMBER      0x4f
#define ACTION_INCREMENT      0x50
#define ACTION_DECREMENT      0x51
#define ACTION_CALLMETHOD     0x52
#define ACTION_NEWMETHOD      0x53
#define ACTION_BITAND         0x60
#define ACTION_BITOR          0x61
#define ACTION_BITXOR         0x62
#define ACTION_BITLSHIFT      0x63
#define ACTION_BITRSHIFT      0x64
#define ACTION_BITURSHIFT     0x65
#define ACTION_GOTOFRAME      0x81
#define ACTION_GETURL         0x83
#define ACTION_STOREREGISTER  0x87
#define ACTION_CONSTANTPOOL   0x88
#define ACTION_WAITFORFRAME   0x8a
#define ACTION_SETTARGET      0x8b
#define ACTION_GOTOLABEL      0x8c
#define ACTION_WAITFORFRAME2  0x8d
#define ACTION_WITH           0x94
#define ACTION_PUSH           0x96
#define ACTION_JUMP           0x99
#define ACTION_GETURL2        0x9a
#define ACTION_DEFINEFUNCTION 0x9b
#define ACTION_IF             0x9d
#define ACTION_CALL           0x9e
#define ACTION_GOTOFRAME2     0x9f

void action_NextFrame() {swf_AddActionTAG(ACTION_NEXTFRAME, 0, 0);}
void action_PreviousFrame() {swf_AddActionTAG(ACTION_PREVIOUSFRAME, 0, 0);}
void action_Play() {swf_AddActionTAG(ACTION_PLAY, 0, 0);}
void action_Stop() {swf_AddActionTAG(ACTION_STOP, 0, 0);}
void action_ToggleQuality() {swf_AddActionTAG(ACTION_TOGGLEQUALITY, 0, 0);}
void action_StopSounds() {swf_AddActionTAG(ACTION_STOPSOUNDS, 0, 0);}
void action_Add() {swf_AddActionTAG(ACTION_ADD, 0, 0);}
void action_Subtract() {swf_AddActionTAG(ACTION_SUBTRACT, 0, 0);}
void action_Multiply() {swf_AddActionTAG(ACTION_MULTIPLY, 0, 0);}
void action_Divide() {swf_AddActionTAG(ACTION_DIVIDE, 0, 0);}
void action_Equals() {swf_AddActionTAG(ACTION_EQUALS, 0, 0);}
void action_Less() {swf_AddActionTAG(ACTION_LESS, 0, 0);}
void action_And() {swf_AddActionTAG(ACTION_AND, 0, 0);}
void action_Or() {swf_AddActionTAG(ACTION_OR, 0, 0);}
void action_Not() {swf_AddActionTAG(ACTION_NOT, 0, 0);}
void action_StringEquals() {swf_AddActionTAG(ACTION_STRINGEQUALS, 0, 0);}
void action_StringLength() {swf_AddActionTAG(ACTION_STRINGLENGTH, 0, 0);}
void action_StringExtract() {swf_AddActionTAG(ACTION_STRINGEXTRACT, 0, 0);}
void action_Pop() {swf_AddActionTAG(ACTION_POP, 0, 0);}
void action_ToInteger() {swf_AddActionTAG(ACTION_TOINTEGER, 0, 0);}
void action_GetVariable() {swf_AddActionTAG(ACTION_GETVARIABLE, 0, 0);}
void action_SetVariable() {swf_AddActionTAG(ACTION_SETVARIABLE, 0, 0);}
void action_SetTarget2() {swf_AddActionTAG(ACTION_SETTARGET2, 0, 0);}
void action_StringAdd() {swf_AddActionTAG(ACTION_STRINGADD, 0, 0);}
void action_GetProperty() {swf_AddActionTAG(ACTION_GETPROPERTY, 0, 0);}
void action_SetProperty() {swf_AddActionTAG(ACTION_SETPROPERTY, 0, 0);}
void action_CloneSprite() {swf_AddActionTAG(ACTION_CLONESPRITE, 0, 0);}
void action_RemoveSprite() {swf_AddActionTAG(ACTION_REMOVESPRITE, 0, 0);}
void action_Trace() {swf_AddActionTAG(ACTION_TRACE, 0, 0);}
void action_StartDrag() {swf_AddActionTAG(ACTION_STARTDRAG, 0, 0);}
void action_EndDrag() {swf_AddActionTAG(ACTION_ENDDRAG, 0, 0);}
void action_StringLess() {swf_AddActionTAG(ACTION_STRINGLESS, 0, 0);}
void action_RandomNumber() {swf_AddActionTAG(ACTION_RANDOMNUMBER, 0, 0);}
void action_MBStringLength() {swf_AddActionTAG(ACTION_MBSTRINGLENGTH, 0, 0);}
void action_CharToAscii() {swf_AddActionTAG(ACTION_CHARTOASCII, 0, 0);}
void action_AsciiToChar() {swf_AddActionTAG(ACTION_ASCIITOCHAR, 0, 0);}
void action_GetTime() {swf_AddActionTAG(ACTION_GETTIME, 0, 0);}
void action_MBStringExtract() {swf_AddActionTAG(ACTION_MBSTRINGEXTRACT, 0, 0);}
void action_MBCharToAscii() {swf_AddActionTAG(ACTION_MBCHARTOASCII, 0, 0);}
void action_MBAsciiToChar() {swf_AddActionTAG(ACTION_MBASCIITOCHAR, 0, 0);}
void action_Delete() {swf_AddActionTAG(ACTION_DELETE, 0, 0);}
void action_Delete2() {swf_AddActionTAG(ACTION_DELETE2, 0, 0);}
void action_DefineLocal() {swf_AddActionTAG(ACTION_DEFINELOCAL, 0, 0);}
void action_CallFunction() {swf_AddActionTAG(ACTION_CALLFUNCTION, 0, 0);}
void action_Return() {swf_AddActionTAG(ACTION_RETURN, 0, 0);}
void action_Modulo() {swf_AddActionTAG(ACTION_MODULO, 0, 0);}
void action_NewObject() {swf_AddActionTAG(ACTION_NEWOBJECT, 0, 0);}
void action_DefineLocal2() {swf_AddActionTAG(ACTION_DEFINELOCAL2, 0, 0);}
void action_InitArray() {swf_AddActionTAG(ACTION_INITARRAY, 0, 0);}
void action_Makehash() {swf_AddActionTAG(ACTION_MAKEHASH, 0, 0);}
void action_TypeOf() {swf_AddActionTAG(ACTION_TYPEOF, 0, 0);}
void action_TargetPath() {swf_AddActionTAG(ACTION_TARGETPATH, 0, 0);}
void action_Enumerate() {swf_AddActionTAG(ACTION_ENUMERATE, 0, 0);}
void action_Add2() {swf_AddActionTAG(ACTION_ADD2, 0, 0);}
void action_Less2() {swf_AddActionTAG(ACTION_LESS2, 0, 0);}
void action_Equals2() {swf_AddActionTAG(ACTION_EQUALS2, 0, 0);}
void action_ToNumber() {swf_AddActionTAG(ACTION_TONUMBER, 0, 0);}
void action_ToString() {swf_AddActionTAG(ACTION_TOSTRING, 0, 0);}
void action_PushDuplicate() {swf_AddActionTAG(ACTION_PUSHDUPLICATE, 0, 0);}
void action_StackSwap() {swf_AddActionTAG(ACTION_STACKSWAP, 0, 0);}
void action_GetMember() {swf_AddActionTAG(ACTION_GETMEMBER, 0, 0);}
void action_SetMember() {swf_AddActionTAG(ACTION_SETMEMBER, 0, 0);}
void action_Increment() {swf_AddActionTAG(ACTION_INCREMENT, 0, 0);}
void action_Decrement() {swf_AddActionTAG(ACTION_DECREMENT, 0, 0);}
void action_CallMethod() {swf_AddActionTAG(ACTION_CALLMETHOD, 0, 0);}
void action_NewMethod() {swf_AddActionTAG(ACTION_NEWMETHOD, 0, 0);}
void action_BitAnd() {swf_AddActionTAG(ACTION_BITAND, 0, 0);}
void action_BitOr() {swf_AddActionTAG(ACTION_BITOR, 0, 0);}
void action_BitXor() {swf_AddActionTAG(ACTION_BITXOR, 0, 0);}
void action_BitLShift() {swf_AddActionTAG(ACTION_BITLSHIFT, 0, 0);}
void action_BitRShift() {swf_AddActionTAG(ACTION_BITRSHIFT, 0, 0);}
void action_BitURShift() {swf_AddActionTAG(ACTION_BITURSHIFT, 0, 0);}
void action_Call() {swf_AddActionTAG(ACTION_CALL, 0, 0);}
void action_End() {swf_AddActionTAG(ACTION_END, 0, 0);}
void action_GotoFrame(U16 frame) 
{
    *(U16*)currentatag->tmp = frame;
    swf_AddActionTAG(ACTION_GOTOFRAME, (U8*)currentatag->tmp, 2);
}
void action_Jump(U16 branch) 
{
    *(U16*)currentatag->tmp = branch;
    swf_AddActionTAG(ACTION_JUMP, (U8*)currentatag->tmp, 2);
}
void action_If(U16 branch) 
{
    *(U16*)currentatag->tmp = branch;
    swf_AddActionTAG(ACTION_IF, (U8*)currentatag->tmp, 2);
}
void action_StoreRegister(U8 reg) 
{
    *(U8*)currentatag->tmp = reg;
    swf_AddActionTAG(ACTION_STOREREGISTER, (U8*)currentatag->tmp, 1);
}
void action_GotoFrame2(U8 method) 
{
    *(U8*)currentatag->tmp = method;
    swf_AddActionTAG(ACTION_GOTOFRAME2, (U8*)currentatag->tmp, 1);
}
void action_GetUrl2(U8 method) 
{
    *(U8*)currentatag->tmp = method;
    swf_AddActionTAG(ACTION_GETURL2, (U8*)currentatag->tmp, 1);
}
void action_WaitForFrame2(U8 skip) 
{
    *(U8*)currentatag->tmp = skip;
    swf_AddActionTAG(ACTION_WAITFORFRAME2, (U8*)currentatag->tmp, 1);
}
void action_WaitForFrame(U16 frame, U8 skip) 
{
    *(U16*)currentatag->tmp = frame;
    *(U8*)&currentatag->tmp[2] = skip;
    swf_AddActionTAG(ACTION_WAITFORFRAME, (U8*)currentatag->tmp, 3);
}
void action_SetTarget(char* target)
{
    char*ptr = strdup(target);
    swf_AddActionTAG(ACTION_SETTARGET, (U8*)ptr, strlen(ptr)+1);
}
void action_PushNULL() 
{
    *(U8*)currentatag->tmp = 2; //NULL
    swf_AddActionTAG(ACTION_PUSH, (U8*)currentatag->tmp, 1);
}
void action_PushBoolean(char c) 
{
    *(U8*)currentatag->tmp = 5; //bool
    *(U8*)&currentatag->tmp[1] = c;
    swf_AddActionTAG(ACTION_PUSH, (U8*)currentatag->tmp, 2);
}
void action_PushRegister(U8 reg) 
{
    *(U8*)currentatag->tmp = 4; //register
    *(U8*)&currentatag->tmp[1] = reg;
    swf_AddActionTAG(ACTION_PUSH, (U8*)currentatag->tmp, 2);
}
void action_PushLookup(U8 index) 
{
    *(U8*)currentatag->tmp = 8; //lookup
    *(U8*)&currentatag->tmp[1] = index;
    swf_AddActionTAG(ACTION_PUSH, (U8*)currentatag->tmp, 2);
}
void action_PushString(char*str) 
{
    int l = strlen(str);
    char*ptr = (char*)malloc(l+2);
    ptr[0] = 0; // string
    strcpy(&ptr[1], str);
    swf_AddActionTAG(ACTION_PUSH, (U8*)ptr, l+2);
}
void action_PushFloat(float f)
{
    char*ptr = (char*)malloc(5);
    ptr[0] = 1; //float
    *(float*)&ptr[1]  = f;
    swf_AddActionTAG(ACTION_PUSH, (U8*)ptr, 5);
}
void action_PushDouble(double d) 
{
    char*ptr = (char*)malloc(9);
    ptr[0] = 6; //double
    *(double*)&ptr[1]  = d;
    swf_AddActionTAG(ACTION_PUSH, (U8*)ptr, 9);
}
void action_PushInt(int i)
{
    *(U8*)currentatag->tmp = 7; //int
    *(U8*)&currentatag->tmp[1] = i;
    swf_AddActionTAG(ACTION_PUSH, (U8*)currentatag->tmp, 5);
}
void action_GotoLabel(char* label)
{
    char*ptr = strdup(label);
    swf_AddActionTAG(ACTION_GOTOLABEL, (U8*)ptr, strlen(ptr));
}
void action_GetUrl(char* url, char* label) 
{
    int l1= strlen(url);
    int l2= strlen(label);
    char*ptr = malloc(l1+l2+2);
    strcpy(ptr, url);
    strcpy(&ptr[l1+1], label);
    swf_AddActionTAG(ACTION_GETURL, ptr, l1+l2+2);
}
//TODO:
void action_DefineFunction(U8*data, int len) {}
void action_Constantpool(char* constantpool) {}
void action_With(char*object) {}
