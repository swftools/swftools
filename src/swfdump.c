/* swfdump.c
   Shows the structure of a swf file

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#define HAVE_STAT

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#else
#undef HAVE_STAT
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#else
#undef HAVE_STAT
#endif

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"

char * filename = 0;

/* idtab stores the ids which are defined in the file. This allows us
   to detect errors in the file. (i.e. ids which are defined more than 
   once */
char idtab[65536];
int action = 0;

struct options_t options[] =
{
 {"a","action"},
 {"v","verbose"},
 {"V","version"},
 {0,0}
};


int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
	printf("swfdump - part of %s %s\n", PACKAGE, VERSION);
	exit(0);
    } 
    else if(name[0]=='a') {
	action = 1;
	return 0;
    }
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char*name)
{    
    printf("Usage: %s [-a] file.swf\n", name);
    printf("-h , --help\t\t\t Print help and exit\n");
    printf("-a , --action\t\t\t Disassemble action tags\n");
    printf("-V , --version\t\t\t Print program version and exit\n");
}
int args_callback_command(char*name,char*val)
{
    if(filename) {
	fprintf(stderr, "Only one file allowed. You supplied at least two. (%s and %s)\n",
		 filename, name);
    }
    filename = name;
    return 0;
}

struct Action
{
    int version;
    char*name;
    U8 op;
    char*flags;
} actions[] =
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
{4,"CloneSprite", 0x24}, // -3
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

int definedactions = sizeof(actions)/sizeof(struct Action);


struct _ActionTAG {
  U8            op;
  U16           len;
  U8 *          data;

  struct _ActionTAG * next;
  struct _ActionTAG * prev;

} ;
typedef struct _ActionTAG ActionTAG;

ActionTAG* showdoaction(TAG*tag, char*prefix) 
{
    U8 op;
    int length;
    int t;
    ActionTAG tmp;
    ActionTAG*atag = &tmp;
    U8*data;
    char* cp;
    while(1)
    {
	int pos;
	atag->next = (ActionTAG*)malloc(sizeof(ActionTAG));
	atag->next->prev = atag;
	atag->next->next = 0;

	op = GetU8(tag);
	if(op<0x80)
	    length = 0;
	else
	    length = GetU16(tag);

	if(length) {
	    data = malloc(length);
	    for(t=0;t<length;t++)
		data[t] = GetU8(tag);
	} else {
	  data = 0;
	}
	atag->op = op;
	atag->len = length;
	atag->data = data;

	if(!op) 
	    break;

	for(t=0;t<definedactions;t++)
	    if(actions[t].op == op)
		break;

	if(t==definedactions) {
	    printf("%s (%5d bytes) action: %02x\n", prefix, length, op);
	    continue;
	}
	printf("%s (%5d bytes) action: %s", prefix, length, actions[t].name);
	cp = actions[t].flags;
	pos = 0;
	if(length) //TODO: check for consistency: should we have a length?
	while(*cp)
	{
	    switch(*cp)
	    {
		case 'f': {
		    printf(" %d", *(U16*)&data[pos]);pos+=2; //FIXME: le/be
		} break;
		case 'u': {
		    printf(" URL:\"%s\"", &data[pos]);pos+=strlen(&data[pos])+1;
		} break;
		case 't': {
		    printf(" Target:\"%s\"", &data[pos]);pos+=strlen(&data[pos])+1;
		} break;
		case 'l': {
		    printf(" Label:\"%s\"", &data[pos]);pos+=strlen(&data[pos])+1;
		} break;
		case 'c': {
		    printf(" Constant Pool:\"%s\"", &data[pos]);pos+=strlen(&data[pos])+1;
	    	} break;
		case 's': {
		    printf(" +%d", data[pos]);pos+=1;
		} break;
		case 'm': {
//m: method (byte) url:(0=none, 1=get, 2=post)/gf2:(1=play)
		    printf(" %d", data[pos]);pos+=1;
		} break;
		case 'b': {
		    printf(" %d", *(U16*)&data[pos]);pos+=2;
		} break;
		case 'p': {
		    U8 type = data[pos++];
		    if(type == 0) {
			printf(" String:\"%s\"", &data[pos]);pos+=strlen(&data[pos])+1;
		    } else if (type == 1) {
			printf(" Double:\"%f\"", *(float*)&data[pos]);pos+=4;
		    } else if (type == 2) {
			printf(" NULL");
		    } else if (type == 4) {
			printf(" register:%d", data[pos++]);
		    } else if (type == 5) {
			printf(" %s", data[pos++]?"true":"false");
		    } else if (type == 6) {
			printf(" %f", *(double*)&data[pos]);  pos+=8;
		    } else if (type == 7) {
			printf(" %d", *(int*)&data[pos]); pos+=4;
		    } else if (type == 8) {
			printf(" Lookup:%d", data[pos++]);
		    }
		} break;
	    
	    }
	    cp++;
	}

	printf("\n");
    }
    printf("%s (%5d bytes) action: End\n", prefix, 0);
    return tmp.next;
}
  
int main (int argc,char ** argv)
{ 
    SWF swf;
    TAG*tag;
#ifdef HAVE_STAT
    struct stat statbuf;
#endif
    int f;
    char prefix[128];
    prefix[0] = 0;
    memset(idtab,0,65536);

    processargs(argc, argv);

    f = open(filename,O_RDONLY);

    if (f<0)
    { 
	perror("Couldn't open file: ");
	exit(1);
    }
    if FAILED(ReadSWF(f,&swf))
    { 
	fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",filename);
        close(f);
	exit(1);
    }

#ifdef HAVE_STAT
    fstat(f, &statbuf);
    if(statbuf.st_size != swf.FileSize)
	fprintf(stderr, "Error: Real Filesize (%d) doesn't match header Filesize (%d)",
		statbuf.st_size, swf.FileSize);
#endif

    close(f);

    printf("[HEADER]        File version: %d\n", swf.FileVersion);
    printf("[HEADER]        File size: %d\n", swf.FileSize);
    printf("[HEADER]        Frame rate: %f\n",swf.FrameRate/256.0);
    printf("[HEADER]        Frame count: %d\n",swf.FrameCount);
    printf("[HEADER]        Movie width: %.3f\n",(swf.MovieSize.xmax-swf.MovieSize.xmin)/20.0);
    printf("[HEADER]        Movie height: %.3f\n",(swf.MovieSize.ymax-swf.MovieSize.ymin)/20.0);

    tag = swf.FirstTag;

    while(tag) {
	char*name = getTagName(tag);
	if(!name) {
	    fprintf(stderr, "Error: Unknown tag:0x%03x\n", tag->id);
	    tag = tag->next;
	    continue;
	}
	printf("[%03x] %9d %s%s", tag->id, tag->len, prefix, getTagName(tag));

	if(isDefiningTag(tag)) {
	    U16 id = GetDefineID(tag);
	    printf(" defines id %04x", id);
	    if(idtab[id])
		fprintf(stderr, "Error: Id %04x is defined more than once.\n", id);
	    idtab[id] = 1;
	}
	else if(tag->id == ST_PLACEOBJECT || 
		tag->id == ST_PLACEOBJECT2) {
	    printf(" places id %04x at depth %04x", GetPlaceID(tag), GetDepth(tag));
	    if(GetName(tag))
		printf(" name \"%s\"",GetName(tag));
        }
	else if(tag->id == ST_REMOVEOBJECT) {
	    printf(" removes id %04x from depth %04x", GetPlaceID(tag), GetDepth(tag));
	}
	else if(tag->id == ST_REMOVEOBJECT2) {
	    printf(" removes object from depth %04x", GetDepth(tag));
	}
	
	printf("\n");

	if(tag->id == ST_DEFINESPRITE) {
	    sprintf(prefix, "         ");
	}
	else if(tag->id == ST_END) {
	    sprintf(prefix, "");
	}
	else if(tag->id == ST_DOACTION && action) {
	    char myprefix[128];
	    sprintf(myprefix, "                %s", prefix);
	    showdoaction(tag, myprefix);
	}
	tag = tag->next;
    }

    FreeTags(&swf);
    return 0;
}

