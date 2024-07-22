/* swfaction.c

   Actionscript generation and parsing routines
   
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
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

#define MAX_LOOKUP 1024   // make cross references in dumps

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
C: constant pool header (word)
c: constant pool entry (string)
s: skip (byte) (number of actions)
m: method (byte) swf_GetUrl2:(0=none, 1=get, 2=post)/GotoFrame2:(1=play)
b: branch (word) (number of bytes)
p (push): type(byte), type=0:string, type=1:double
{: define function (name (string), num (word), params (num strings), codesize (word)
o: codesize (word) object (string)
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
{5,"Less2", 0x48,""},
{6,"Greater", 0x67,""},
{6,"StringGreater", 0x68,""},
{6,"Enumerate2", 0x55,""},
{6,"InstanceOf", 0x54,""},
{6,"StrictEquals", 0x66,""},
{7,"DefineFunction2", 0x8e, "{"}
};
static int definedactions = sizeof(actions)/sizeof(struct Action);

ActionTAG* swf_ActionGet(TAG*tag) 
{
    U8 op = 1;
    int length;
    ActionTAG tmp;
    ActionTAG*action = &tmp;
    U8*data;
    while(op)
    {
	action->next = (ActionTAG*)rfx_calloc(sizeof(ActionTAG));
	action->next->prev = action;
	action->next->next = 0;
	action->next->parent = tmp.next;
	action = action->next;

	op = swf_GetU8(tag);
	if(op<0x80)
	    length = 0;
	else
	    length = swf_GetU16(tag);

	if(length) {
	    data = (U8*)rfx_alloc(length);
	    swf_GetBlock(tag, data, length);
	} else {
	  data = 0;
	}
	action->op = op;
	action->len = length;
	action->data = data;
    }
    return tmp.next;
}

void swf_ActionFree(ActionTAG*action)
{
    if(!action) {
	return;
    }
    action = action->parent;
    if(!action) {
	fprintf(stderr, "Warning: freeing zero action (no parent)");
	return;
    }

    while(action)
    {
	ActionTAG*tmp;
	if(action->data && action->data != action->tmp) {
	    rfx_free(action->data);
	    action->data = 0;
	}
	action->len = 0;
	
	tmp = action;
	action=action->next;
	rfx_free(tmp);
    }
}

void swf_ActionSet(TAG*tag, ActionTAG*action)
{
    if(!action) {
	return;
    }
    action=action->parent;
    while(action)
    {
	swf_SetU8(tag, action->op);
	if(action->op & 128)
	  swf_SetU16(tag, action->len);

	swf_SetBlock(tag, action->data, action->len);

	action = action->next;
    }
}

int OpAdvance(ActionTAG *atag, char c, U8*data)
{
    switch (c)
    {
	case 'f':
	    return 2;
	case 'u':
	    return strlen((const char*)data)+1;
	case 't':
	    return strlen((const char*)data)+1;
	case 'l': 
	    return strlen((const char*)data)+1;
	case 'c': 
	    return strlen((const char*)data)+1;
	case 'C': 
	    return 2;
	case 's':
	    return 1;
	case 'm':
	    return 1;
	case 'b':
	    return 2;
	case 'r':
	    return 1;
	case 'p': {
	    U8 type = *data++;
	    if(type == 0) {
		return 1+strlen((const char*)data)+1; //string
	    } else if (type == 1) {
		return 1+4; //float
	    } else if (type == 2) {
		return 1+0; //NULL
	    } else if (type == 3) {
		return 1+0; //Undefined
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
	    } else if (type == 9) {
		return 1+2; //lookup 16
	    } else return 1;
	    break;
	}
	case 'o': {
	    return 2;
	}
	case '{': {
	    U16 num;
	    U16 codesize;
	    U8* odata = data;
	    int t;
	    while(*data++); //name
	    num = (*data++); //num
	    num += (*data++)*256;
	    if(atag->op == 0x8e/*DefineFunction2*/) {
		data += 3; // RegisterCount and flags
	    }
	    for(t=0;t<num;t++)
	    {
		if(atag->op == 0x8e/*DefineFunction2*/)
			data++; // Register
		while(*data++); //param
	    }
	    codesize = (*data++); //num
	    codesize += (*data++)*256;
	    return data-odata;
	}
    }
    return 0;
}
#define ATAG_FULLLENGTH(atag) ((atag)->len + 1 + ((atag)->op&0x80?2:0))
#define MAX_LEVELS 16
/* TODO: * this should be in swfdump.c */
void swf_DumpActions(ActionTAG*atag, char*prefix) 
{
    int t;
    U8*data;
    char* cp;
    int entry = 0;
    char spaces[MAX_LEVELS*4+1];
    struct {
	char*text;
	int count;
    } counter[MAX_LEVELS];
    int countpos = 0;
#ifdef MAX_LOOKUP
    char * lookup[MAX_LOOKUP];
    memset(lookup,0x00,sizeof(lookup));
#endif
    memset(spaces, 32, sizeof(spaces));
    spaces[sizeof(spaces)-1] = 0;

   if (!prefix)
        prefix="";

    while(atag)
    {
	char*indent = &spaces[sizeof(spaces)-1-countpos*4];
	U16 poollen = 0;
	for(t=0;t<definedactions;t++)
	    if(actions[t].op == atag->op)
		break;

	if(t==definedactions) {
	    printf("%s (%5d bytes) action:%s unknown[%02x]", prefix, atag->len, indent, atag->op);
	} else {
	    printf("%s (%5d bytes) action:%s %s", prefix, atag->len, indent, actions[t].name);
	}
	data = atag->data;
	if(atag->len && t!=definedactions) //TODO: check for consistency: should we have a length?
	{
	  cp = actions[t].flags;
	  while(*cp)
	  {
	      switch(*cp)
	      {
		  case 'f': { //frame
		      printf(" %d", data[0]+256*data[1]);
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
#ifdef MAX_LOOKUP
		      if (entry<MAX_LOOKUP)
			lookup[entry++] = strdup((const char*)data);
#endif
		  } break;
		  case 'C': {
		      poollen = data[0]+256*data[1];
		      entry = 0;
		      printf("(%d entries)", poollen);
		  } break;
		  case 's': {
		      printf(" +%d", *data);
		  } break;
		  case 'm': {
		      //m: method (byte) url:(0=none, 1=get, 2=datat)/gf2:(1=play)
		      printf(" %d", *data);
		  } break;
		  case '{': {
		      U16 num;
		      U16 codesize;
		      int s = 0;
		      int t;
		      printf(" %s(", data);
		      while(data[s++]); //name
		      num = (data[s++]); //num
		      num += (data[s++])*256;
		      if(atag->op == 0x8e/*DefineFunction2*/) {
			  s += 3; // RegisterCount and flags
		      }
		      for(t=0;t<num;t++) {
			  if(atag->op == 0x8e/*DefineFunction2*/)
			      s++; // Register
			  printf("%s",data+s);  // 10/22/04 MD: added +s to
			  if(t<num-1)
			      printf(", ");
			  while(data[s++]); //param
		      }
		      printf(")");
		      codesize = (data[s++]); //num
		      codesize += (data[s++])*256;
		      printf(" codesize:%d ",codesize);
		      printf("\n%s                       %s{", prefix, indent);
		      if(countpos>=15) {
			  printf("Error: nested too deep\n");
			  continue;
		      }
		      counter[countpos].text = "}";
		      counter[countpos].count = codesize + ATAG_FULLLENGTH(atag);
		      countpos++;
		  } break;
		  case 'o': {
		      int t;
		      U16 codesize = data[0]+256*data[1];
		      printf(" codesize:%d ", codesize);

		      /* the following tries to find the "string"
			 the flash documentation speaks of- I've
			 never actually seen one yet. -mk */
		      for(t=2;t<atag->len;t++)
			  printf("[%02x]", atag->data[t]);

		      printf("\n%s                       %s{", prefix, indent);
		      if(countpos>=15) {
			  printf("Error: nested too deep\n");
			  continue;
		      }
		      counter[countpos].text = "}";
		      counter[countpos].count = codesize + ATAG_FULLLENGTH(atag);
		      countpos++;
		  } break;
		  case 'b': {
		      printf(" %d", data[0]+256*(signed char)data[1]);
		  } break;
		  case 'r': {
		      printf(" %d", data[0]);
		  } break;
		  case 'p': {
		      U8 type = *data;
		      unsigned char*value = data+1;
		      if(type == 0) {
			  printf(" String:\"%s\"", value);
		      } else if (type == 1) {
			  U32 f = value[0]+(value[1]<<8)+
				  (value[2]<<16)+(value[3]<<24);
			  float f2;
			  memcpy(&f2, f, sizeof(f2));
			  printf(" Float:%f", f2);
		      } else if (type == 2) {
			  printf(" NULL");
		      } else if (type == 3) {
			  printf(" Undefined");
		      } else if (type == 4) {
			  printf(" register:%d", *value);
		      } else if (type == 5) {
			  printf(" bool:%s", *value?"true":"false");
		      } else if (type == 6) {
			  U8 a[8];
			  double b;
			  memcpy(&a[4],value,4);
			  memcpy(a,&value[4],4);
#ifdef WORDS_BIGENDIAN
			  int t;
			  for(t=0;t<4;t++) {
			      U8 tmp = a[t];
			      a[t]=a[7-t];
			      a[7-t] = tmp;
			  }
#endif
			  memcpy(&b, a, sizeof(b));
			  printf(" double:%f", b);
		      } else if (type == 7) {
			  printf(" int:%d", value[0]+(value[1]<<8)+
					    (value[2]<<16)+(value[3]<<24));
		      } else if (type == 8) {
			  printf(" Lookup:%d", *value);
#ifdef MAX_LOOKUP
			  if (lookup[*value])
			    printf(" (\"%s\")",lookup[*value]);
#endif
		      } else if (type == 9) {
			  U32 offset = value[0]+(value[1]<<8);
			  printf(" Lookup16:%d", offset);
#ifdef MAX_LOOKUP
			  if (lookup[offset])
			    printf(" (\"%s\")",lookup[offset]);
#endif
		      } else {
			  printf(" UNKNOWN[%02x]",type);
		      }
		  } break;
	      }
	      data += OpAdvance(atag, *cp, data);
	      if((*cp!='c' || !poollen) &&
		 (*cp!='p' || !(data<&atag->data[atag->len])))
		  cp++;
	      if(poollen)
		  poollen--;
	  }
	}

	if(data < atag->data + atag->len)
	{
	    int nl = ((atag->data+atag->len)-data);
	    int t;
	    printf(" (remainder of %d bytes:\"", nl);
	    for(t=0;t<nl;t++) {
		if(data[t]<32)
		    printf("\\%d",data[t]);
		else
		    printf("%c", data[t]);
	    }
	    printf("\")");
	}
	printf("\n");

	for(t=0;t<countpos;t++) {
	    counter[t].count -= ATAG_FULLLENGTH(atag);
	    if(counter[t].count < 0) {
		printf("===== Error: Oplength errors =====\n");
		countpos = 0;
		break;
	    }
	}

	while(countpos && !counter[countpos-1].count)
	{
	    printf("%s                   %s%s\n", 
		prefix, indent, counter[countpos-1].text);
	    indent += 4;
	    countpos--;
	}

	atag = atag->next;
    }

#ifdef MAX_LOOKUP
  for (t=0;t<MAX_LOOKUP;t++) if (lookup[t]) rfx_free(lookup[t]);
#endif
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
	U16 poollen = 0;
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
			    replacelen = strlen((const char*)data);
			    replacepos = data;
			    replacement = (U8*)callback((char*)data); // may be null
			}
		    } break;
	    	    case 't': {
			if(type&TYPE_TARGET)
			{
			    replacelen = strlen((const char*)data);
			    replacepos = data;
			    replacement = (U8*)callback((char*)data); // may be null
			}
		    } break;
		    case 'c': {
		    	if(type&TYPE_STRING)
			{
			    replacelen = strlen((const char*)data);
			    replacepos = data;
			    replacement = (U8*)callback((char*)data); // may be null
			}
		    } break;
		    case 'C': {
			poollen = (data[0]+256*data[1]);
		    } break;
		    case 'o': {
		    } break;
		    case 'p': {
			U8 datatype = *data;
			char*value = (char*)&data[1];
			if(datatype == 0) { //string
			    if(type&TYPE_STRING)
			    {
				replacelen = strlen(value);
				replacepos = (U8*)value;
				replacement = (U8*)callback(value); // may be null
			    }
			} else if (datatype == 8) { //lookup
			}
		    } break;
		}
		data += OpAdvance(atag, *cp, data);
		if(*cp!='c' || !poollen)
		    cp++;
		if(poollen)
		    poollen--;

		if(replacement)
		{
		    int newlen = strlen((const char *)replacement);
		    char * newdata = (char*)rfx_alloc(atag->len - replacelen + newlen);
		    int rpos = replacepos - atag->data;
		    memcpy(newdata, atag->data, rpos);
		    memcpy(&newdata[rpos], replacement, newlen);
		    memcpy(&newdata[rpos+newlen], &replacepos[replacelen],
			    &data[atag->len] - &replacepos[replacelen]);
		    rfx_free(atag->data);
		    atag->data = (U8*)newdata;
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

/*static ActionTAG* swf_ActionStart()
{
    ActionTAG*atag;
    atag = (ActionTAG*)rfx_alloc(sizeof(ActionTAG));
    atag->prev = 0;
    atag->next = 0;
    atag->parent = 0;
    atag->data = 0;
    atag->len = 0;
    return atag;
}

void swf_ActionEnd(ActionTAG* atag)
{
    ActionTAG*last;
    while(atag) {
	last = atag;
	atag=atag->next;
    } 

    last->prev->next = 0;
    rfx_free(last);
}*/

static ActionTAG*lastATAG(ActionTAG*atag)
{
    ActionTAG*last = 0;
    while(atag) {
	last = atag;
	atag=atag->next;
    } 
    return last;
}

ActionTAG* swf_AddActionTAG(ActionTAG*atag, U8 op, U8*data, U16 len)
{
    ActionTAG*tmp;
    tmp = (ActionTAG*)rfx_alloc(sizeof(ActionTAG));
    tmp->next = 0;
    if(atag) {
	tmp->prev = atag;
	atag->next = tmp;
	tmp->parent = atag->parent;
    } else {
	tmp->prev = 0;
	tmp->parent = tmp;
    }
    if(data || !len) {
	tmp->data = data;
    } else {
	tmp->data = tmp->tmp;
    }

    tmp->len = len;
    tmp->op = op;
    return tmp;
}

ActionMarker action_setMarker(ActionTAG*atag)
{
    ActionMarker m;
    m.atag = atag;
    return m;
}

int inline ActionTagSize(ActionTAG*atag)
{
    return (atag->op&0x80)?3+(atag->len):1+0;
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
#define ACTION_DEFINEFUNCTION2 0x8e
#define ACTION_WITH           0x94
#define ACTION_PUSH           0x96
#define ACTION_JUMP           0x99
#define ACTION_GETURL2        0x9a
#define ACTION_DEFINEFUNCTION 0x9b
#define ACTION_IF             0x9d
#define ACTION_CALL           0x9e
#define ACTION_GOTOFRAME2     0x9f

void action_fixjump(ActionMarker m1, ActionMarker m2)
{
    ActionTAG* a1 = m1.atag;
    ActionTAG* a2 = m2.atag;
    ActionTAG* a;
    int len = 0;
    int oplen = 0;
    a = a1;
    
    a = a->next; //first one is free
    while(a && a!=a2)
    {
	len += ActionTagSize(a);
	oplen ++;
	a = a->next;
    }
    if(!a)
    { len = 0;
      oplen = 0;
      a = a2;
      while(a && a!=a1) {
	  len -= ActionTagSize(a);
	  oplen --;
	  a = a->next;
      }
      if(!a) {
	  fprintf(stderr, "action_fixjump: couldn't find second tag\n");
	  return;
      }
      len -= ActionTagSize(a);
      oplen --;
    }

    if (a1->op == ACTION_IF || a1->op == ACTION_JUMP) 
    {
	*(U16*)(a1->data) = LE_16_TO_NATIVE(len);
    }
    else if(a1->op == ACTION_WAITFORFRAME)
    {
	((U8*)(a1->data))[2] = oplen;
    }
    else if(a1->op == ACTION_WAITFORFRAME2)
    {
	((U8*)(a1->data))[0] = oplen;
    }
    
}

ActionTAG* action_NextFrame(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_NEXTFRAME, 0, 0);}
ActionTAG* action_PreviousFrame(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_PREVIOUSFRAME, 0, 0);}
ActionTAG* action_Play(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_PLAY, 0, 0);}
ActionTAG* action_Stop(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_STOP, 0, 0);}
ActionTAG* action_ToggleQuality(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_TOGGLEQUALITY, 0, 0);}
ActionTAG* action_StopSounds(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_STOPSOUNDS, 0, 0);}
ActionTAG* action_Add(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_ADD, 0, 0);}
ActionTAG* action_Subtract(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_SUBTRACT, 0, 0);}
ActionTAG* action_Multiply(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_MULTIPLY, 0, 0);}
ActionTAG* action_Divide(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_DIVIDE, 0, 0);}
ActionTAG* action_Equals(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_EQUALS, 0, 0);}
ActionTAG* action_Less(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_LESS, 0, 0);}
ActionTAG* action_And(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_AND, 0, 0);}
ActionTAG* action_Or(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_OR, 0, 0);}
ActionTAG* action_Not(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_NOT, 0, 0);}
ActionTAG* action_StringEquals(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_STRINGEQUALS, 0, 0);}
ActionTAG* action_StringLength(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_STRINGLENGTH, 0, 0);}
ActionTAG* action_StringExtract(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_STRINGEXTRACT, 0, 0);}
ActionTAG* action_Pop(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_POP, 0, 0);}
ActionTAG* action_ToInteger(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_TOINTEGER, 0, 0);}
ActionTAG* action_GetVariable(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_GETVARIABLE, 0, 0);}
ActionTAG* action_SetVariable(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_SETVARIABLE, 0, 0);}
ActionTAG* action_SetTarget2(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_SETTARGET2, 0, 0);}
ActionTAG* action_StringAdd(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_STRINGADD, 0, 0);}
ActionTAG* action_GetProperty(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_GETPROPERTY, 0, 0);}
ActionTAG* action_SetProperty(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_SETPROPERTY, 0, 0);}
ActionTAG* action_CloneSprite(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_CLONESPRITE, 0, 0);}
ActionTAG* action_RemoveSprite(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_REMOVESPRITE, 0, 0);}
ActionTAG* action_Trace(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_TRACE, 0, 0);}
ActionTAG* action_StartDrag(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_STARTDRAG, 0, 0);}
ActionTAG* action_EndDrag(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_ENDDRAG, 0, 0);}
ActionTAG* action_StringLess(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_STRINGLESS, 0, 0);}
ActionTAG* action_RandomNumber(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_RANDOMNUMBER, 0, 0);}
ActionTAG* action_MBStringLength(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_MBSTRINGLENGTH, 0, 0);}
ActionTAG* action_CharToAscii(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_CHARTOASCII, 0, 0);}
ActionTAG* action_AsciiToChar(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_ASCIITOCHAR, 0, 0);}
ActionTAG* action_GetTime(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_GETTIME, 0, 0);}
ActionTAG* action_MBStringExtract(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_MBSTRINGEXTRACT, 0, 0);}
ActionTAG* action_MBCharToAscii(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_MBCHARTOASCII, 0, 0);}
ActionTAG* action_MBAsciiToChar(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_MBASCIITOCHAR, 0, 0);}
ActionTAG* action_Delete(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_DELETE, 0, 0);}
ActionTAG* action_Delete2(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_DELETE2, 0, 0);}
ActionTAG* action_DefineLocal(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_DEFINELOCAL, 0, 0);}
ActionTAG* action_CallFunction(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_CALLFUNCTION, 0, 0);}
ActionTAG* action_Return(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_RETURN, 0, 0);}
ActionTAG* action_Modulo(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_MODULO, 0, 0);}
ActionTAG* action_NewObject(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_NEWOBJECT, 0, 0);}
ActionTAG* action_DefineLocal2(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_DEFINELOCAL2, 0, 0);}
ActionTAG* action_InitArray(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_INITARRAY, 0, 0);}
ActionTAG* action_Makehash(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_MAKEHASH, 0, 0);}
ActionTAG* action_TypeOf(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_TYPEOF, 0, 0);}
ActionTAG* action_TargetPath(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_TARGETPATH, 0, 0);}
ActionTAG* action_Enumerate(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_ENUMERATE, 0, 0);}
ActionTAG* action_Add2(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_ADD2, 0, 0);}
ActionTAG* action_Less2(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_LESS2, 0, 0);}
ActionTAG* action_Equals2(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_EQUALS2, 0, 0);}
ActionTAG* action_ToNumber(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_TONUMBER, 0, 0);}
ActionTAG* action_ToString(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_TOSTRING, 0, 0);}
ActionTAG* action_PushDuplicate(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_PUSHDUPLICATE, 0, 0);}
ActionTAG* action_StackSwap(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_STACKSWAP, 0, 0);}
ActionTAG* action_GetMember(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_GETMEMBER, 0, 0);}
ActionTAG* action_SetMember(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_SETMEMBER, 0, 0);}
ActionTAG* action_Increment(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_INCREMENT, 0, 0);}
ActionTAG* action_Decrement(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_DECREMENT, 0, 0);}
ActionTAG* action_CallMethod(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_CALLMETHOD, 0, 0);}
ActionTAG* action_NewMethod(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_NEWMETHOD, 0, 0);}
ActionTAG* action_BitAnd(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_BITAND, 0, 0);}
ActionTAG* action_BitOr(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_BITOR, 0, 0);}
ActionTAG* action_BitXor(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_BITXOR, 0, 0);}
ActionTAG* action_BitLShift(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_BITLSHIFT, 0, 0);}
ActionTAG* action_BitRShift(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_BITRSHIFT, 0, 0);}
ActionTAG* action_BitURShift(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_BITURSHIFT, 0, 0);}
ActionTAG* action_Call(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_CALL, 0, 0);}
ActionTAG* action_End(ActionTAG*atag) {return swf_AddActionTAG(atag, ACTION_END, 0, 0);}
ActionTAG* action_GotoFrame(ActionTAG*atag, U16 frame) 
{
    U16 tmp;
    atag = swf_AddActionTAG(atag, ACTION_GOTOFRAME, 0, 2);
    tmp = LE_16_TO_NATIVE(frame);
    memcpy(&atag->tmp, tmp, sizeof(tmp));
    return atag;
}

ActionTAG* action_Jump(ActionTAG*atag, U16 branch) 
{
    U16 tmp;
    atag = swf_AddActionTAG(atag, ACTION_JUMP, 0, 2);
    tmp = LE_16_TO_NATIVE(branch);
    memcpy(&atag->tmp, tmp, sizeof(tmp));
    return atag;
}
ActionTAG* action_If(ActionTAG*atag, U16 branch) 
{
    U16 tmp;
    atag = swf_AddActionTAG(atag, ACTION_IF, 0, 2);
    tmp = LE_16_TO_NATIVE(branch);
    memcpy(&atag->tmp, tmp, sizeof(tmp));
    return atag;
}
ActionTAG* action_StoreRegister(ActionTAG*atag, U8 reg) 
{
    atag = swf_AddActionTAG(atag, ACTION_STOREREGISTER, 0, 1);
    *(U8*)atag->tmp = reg;
    return atag;
}
ActionTAG* action_GotoFrame2(ActionTAG*atag, U8 method) 
{
    atag = swf_AddActionTAG(atag, ACTION_GOTOFRAME2, 0, 1);
    *(U8*)atag->tmp = method;
    return atag;
}
ActionTAG* action_GetUrl2(ActionTAG*atag, U8 method) 
{
    atag = swf_AddActionTAG(atag, ACTION_GETURL2, 0, 1);
    *(U8*)atag->tmp = method;
    return atag;
}
ActionTAG* action_WaitForFrame2(ActionTAG*atag, U8 skip) 
{
    atag = swf_AddActionTAG(atag, ACTION_WAITFORFRAME2, 0, 1);
    *(U8*)atag->tmp = skip;
    return atag;
}
ActionTAG* action_WaitForFrame(ActionTAG*atag, U16 frame, U8 skip) 
{
    U16 tmp;
    atag = swf_AddActionTAG(atag, ACTION_WAITFORFRAME, 0, 3);
    tmp = LE_16_TO_NATIVE(frame);
    memcpy(&atag->tmp, tmp, sizeof(tmp));
    *(U8*)&atag->tmp[2] = skip;
    return atag;
}
ActionTAG* action_SetTarget(ActionTAG*atag, const char* target)
{
    char*ptr = strdup(target);
    return swf_AddActionTAG(atag, ACTION_SETTARGET, (U8*)ptr, strlen(ptr)+1);
}
ActionTAG* action_PushNULL(ActionTAG*atag) 
{
    atag = swf_AddActionTAG(atag, ACTION_PUSH, 0, 1);
    *(U8*)atag->tmp = 2; //NULL
    return atag;
}
ActionTAG* action_PushUndefined(ActionTAG*atag) 
{
    atag = swf_AddActionTAG(atag, ACTION_PUSH, 0, 1);
    *(U8*)atag->tmp = 3; //Undefined
    return atag;
}
ActionTAG* action_PushBoolean(ActionTAG*atag, char c) 
{
    atag = swf_AddActionTAG(atag, ACTION_PUSH, 0, 2);
    *(U8*)atag->tmp = 5; //bool
    *(U8*)&atag->tmp[1] = c;
    return atag;
}
ActionTAG* action_PushRegister(ActionTAG*atag, U8 reg) 
{
    atag = swf_AddActionTAG(atag, ACTION_PUSH, 0, 2);
    *(U8*)atag->tmp = 4; //register
    *(U8*)&atag->tmp[1] = reg;
    return atag;
}
ActionTAG* action_PushLookup(ActionTAG*atag, U8 index) 
{
    atag = swf_AddActionTAG(atag, ACTION_PUSH, 0, 2);
    *(U8*)atag->tmp = 8; //lookup
    *(U8*)&atag->tmp[1] = index;
    return atag;
}
ActionTAG* action_PushLookup16(ActionTAG*atag, U16 index) 
{
    atag = swf_AddActionTAG(atag, ACTION_PUSH, 0, 3);
    *(U8*)atag->tmp = 9; //lookup
    *(U8*)&atag->tmp[1] = (U8)index;
    *(U8*)&atag->tmp[2] = index>>8;
    return atag;
}
ActionTAG* action_PushString(ActionTAG*atag, const char*str) 
{
    int l = strlen(str);
    char*ptr = (char*)rfx_alloc(l+2);
    ptr[0] = 0; // string
    strcpy(&ptr[1], str);
    return swf_AddActionTAG(atag, ACTION_PUSH, (U8*)ptr, l+2);
}
ActionTAG* action_PushFloat(ActionTAG*atag, float f)
{
    char*ptr = (char*)rfx_alloc(5);
    U32 fd;
    memcpy(&fd, &f, sizeof(f));
    ptr[0] = 1; //float
    ptr[1]  = fd;
    ptr[2]  = fd>>8;
    ptr[3]  = fd>>16;
    ptr[4]  = fd>>24;
    return swf_AddActionTAG(atag, ACTION_PUSH, (U8*)ptr, 5);
}
ActionTAG* action_PushDouble(ActionTAG*atag, double d) 
{
    char*ptr = (char*)rfx_alloc(9);
    U8*dd = (U8*)&d;
    ptr[0] = 6; //double
#ifdef WORDS_BIGENDIAN
    ptr[1] = dd[7];ptr[2] = dd[6];
    ptr[3] = dd[5];ptr[4] = dd[4];
    ptr[5] = dd[3];ptr[6] = dd[2];
    ptr[7] = dd[1];ptr[8] = dd[0];
#else
    ptr[1] = dd[0];ptr[2] = dd[1];
    ptr[3] = dd[2];ptr[4] = dd[3];
    ptr[5] = dd[4];ptr[6] = dd[5];
    ptr[7] = dd[6];ptr[8] = dd[7];
#endif
    return swf_AddActionTAG(atag, ACTION_PUSH, (U8*)ptr, 9);
}
ActionTAG* action_PushInt(ActionTAG*atag, int i)
{
    atag = swf_AddActionTAG(atag, ACTION_PUSH, 0, 5);
    atag->tmp[0] = 7; //int
    atag->tmp[1] = i;
    atag->tmp[2] = i>>8;
    atag->tmp[3] = i>>16;
    atag->tmp[4] = i>>24;
    return atag;
}
ActionTAG* action_GotoLabel(ActionTAG*atag, char* label)
{
    char*ptr = strdup(label);
    return swf_AddActionTAG(atag, ACTION_GOTOLABEL, (U8*)ptr, strlen(ptr));
}
ActionTAG* action_GetUrl(ActionTAG*atag, const char* url, char* label) 
{
    int l1= strlen(url);
    int l2= strlen(label);
    char*ptr = (char*)rfx_alloc(l1+l2+2);
    strcpy(ptr, url);
    strcpy(&ptr[l1+1], label);
    return swf_AddActionTAG(atag, ACTION_GETURL, (U8*)ptr, l1+l2+2);
}
//TODO:
ActionTAG* action_DefineFunction(ActionTAG*atag, U8*data, int len) {return atag;}
ActionTAG* action_Constantpool(ActionTAG*atag, char* constantpool) {return atag;}
ActionTAG*  action_With(ActionTAG*atag, char*object) {return atag;}

#include "../action/actioncompiler.h"

ActionTAG* swf_ActionCompile(const char* source, int version)
{
    TAG* tag;
    ActionTAG* a = 0;
    void*buffer = 0;
    int len = 0;
    int ret;
    
    tag = swf_InsertTag(NULL, ST_DOACTION);
    ret = compileSWFActionCode(source, version, &buffer, &len);
    if(!ret || buffer==0 || len == 0)
	return 0;

    swf_SetBlock(tag, (U8*)buffer, len);
    swf_SetU8(tag, 0);

    rfx_free(buffer);

    a = swf_ActionGet(tag);
    swf_DeleteTag(0, tag);
    return a;
}


/*
  Properties:

  _X 0
  _Y 1
  _xscale 2
  _yscale 3
  _currentframe 4
  _totalframes 5
  _alpha 6
  _visible 7
  _width 8
  _height 9
  _rotation 10
  _target 11
  _framesloaded 12
  _name 13
  _droptarget 14
  _url 15
  _highquality 16
  _focusrect 17
  _soundbuftime 18
  _quality* 19
  _xmouse* 20
  _ymouse* 21
*/
