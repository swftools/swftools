#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "action.h"
#include "compile.h"


#define print(x)	{fputs(x,stdout);}

int gIndent;
#define INDENT_LEVEL 2

void println(const char *s, ...)
{
  va_list ap;
  int n = gIndent*INDENT_LEVEL;

  while(n-- > 0)
    putchar(' ');

  va_start(ap, s);
  vprintf(s, ap);
  va_end(ap);

  putchar('\n');
}

int fileOffset = 0;

int readUInt8(Buffer f)
{
  return f->buffer[fileOffset++];
}

int readSInt8(Buffer f)
{
  return (signed char)readUInt8(f);
}

int readSInt16(Buffer f)
{
  return readUInt8(f) + readSInt8(f)*256;
}

int readUInt16(Buffer f)
{
  return readUInt8(f) + (readUInt8(f)<<8);
}

long readSInt32(Buffer f)
{
  return (long)readUInt8(f) + (readUInt8(f)<<8) + (readUInt8(f)<<16) + (readUInt8(f)<<24);
}

unsigned long readUInt32(Buffer f)
{
  return (unsigned long)(readUInt8(f) + (readUInt8(f)<<8) + (readUInt8(f)<<16) + (readUInt8(f)<<24));
}

double readDouble(Buffer f)
{
  double d;
  unsigned char *p = (unsigned char *)&d;

  p[4] = readUInt8(f);
  p[5] = readUInt8(f);
  p[6] = readUInt8(f);
  p[7] = readUInt8(f);
  p[0] = readUInt8(f);
  p[1] = readUInt8(f);
  p[2] = readUInt8(f);
  p[3] = readUInt8(f);

  return d;
}

char *readString(Buffer f)
{
  int len = 0, buflen = 256;
  char c, *buf, *p;

  buf = (char *)malloc(sizeof(char)*256);
  p = buf;

  while((c=(char)readUInt8(f)) != '\0')
  {
    if(len==buflen)
    {
      buf = (char *)realloc(buf, sizeof(char)*(buflen+256));
      buflen += 256;
      p = buf+len;
    }

    *(p++) = c;
    ++len;
  }

  *p = 0;

  return buf;
}

void dumpBytes(Buffer f, int length)
{
  int j=0, i, k;
  unsigned char buf[16];

  if(length==0)
    return;

  for(;;)
  {
    for(i=0; i<16; ++i)
    {
      printf("%02x ", buf[i] = readUInt8(f));
      ++j;

      if(j==length)
		break;
    }

    if(j==length)
    {
      for(k=i+1; k<16; ++k)
	print("   ");

      ++i;
    }

    print("   ");

    for(k=0; k<i; ++k)
      if((buf[k] > 31) && (buf[k] < 128))
	putchar(buf[k]);
      else
	putchar('.');

    putchar('\n');

    if(j==length)
      break;
  }
  putchar('\n');
  putchar('\n');
}

void printDoAction(Buffer f, int length);

char *dictionary[256];

int printActionRecord(Buffer f)
{
  int length = 0, type;

  printf("(%i)\t", fileOffset);

  type = readUInt8(f);

  if((type&0x80) == 0x80)
    length = readUInt16(f);

  switch(type)
  {
    case SWFACTION_ADD:
      println("Add");
      break;
    case SWFACTION_SUBTRACT:
      println("Subtract");
      break;
    case SWFACTION_MULTIPLY:
      println("Multiply");
      break;
    case SWFACTION_DIVIDE:
      println("Divide");
      break;
    case SWFACTION_EQUAL:
      println("Equals");
      break;
    case SWFACTION_LESSTHAN:
      println("Less Than");
      break;
    case SWFACTION_LOGICALAND:
      println("And");
      break;
    case SWFACTION_LOGICALOR:
      println("Or");
      break;
    case SWFACTION_LOGICALNOT:
      println("Not");
      break;
    case SWFACTION_STRINGEQ:
      println("String eq");
      break;
    case SWFACTION_STRINGLENGTH:
      println("String Length");
      break;
    case SWFACTION_SUBSTRING:
      println("Substring");
      break;
    case SWFACTION_POP:
      println("Pop");
      break;
    case SWFACTION_INT:
      println("Int");
      break;
    case SWFACTION_GETVARIABLE:
      println("Get Variable");
      break;
    case SWFACTION_SETVARIABLE:
      println("Set Variable");
      break;
    case SWFACTION_SETTARGETEXPRESSION:
      println("Set Target Expression");
      break;
    case SWFACTION_STRINGCONCAT:
      println("String Concat");
      break;
    case SWFACTION_GETPROPERTY:
      println("Get Property");
      break;
    case SWFACTION_SETPROPERTY:
      println("Set Property");
      break;
    case SWFACTION_DUPLICATECLIP:
      println("Duplicate Clip");
      break;
    case SWFACTION_REMOVECLIP:
      println("Remove Clip");
      break;
    case SWFACTION_TRACE:
      println("Trace");
      break;
    case SWFACTION_STARTDRAGMOVIE:
      println("Start Drag Movie");
      break;
    case SWFACTION_STOPDRAGMOVIE:
      println("Stop Drag Movie");
      break;
    case SWFACTION_STRINGCOMPARE:
      println("String Compare");
      break;
    case SWFACTION_RANDOM:
      println("Random");
      break;
    case SWFACTION_MBLENGTH:
      println("String MB Length");
      break;
    case SWFACTION_ORD:
      println("Ord");
      break;
    case SWFACTION_CHR:
      println("Chr");
      break;
    case SWFACTION_GETTIMER:
      println("Get Timer");
      break;
    case SWFACTION_MBSUBSTRING:
      println("MB Substring");
      break;
    case SWFACTION_MBORD:
      println("MB Ord");
      break;
    case SWFACTION_MBCHR:
      println("MB Chr");
      break;
    case SWFACTION_NEXTFRAME:
      println("Next Frame");
      break;
    case SWFACTION_PREVFRAME:
      println("Previous Frame");
      break;
    case SWFACTION_PLAY:
      println("Play");
      break;
    case SWFACTION_STOP:
      println("Stop");
      break;
    case SWFACTION_TOGGLEQUALITY:
      println("Toggle Quality");
      break;
    case SWFACTION_STOPSOUNDS:
      println("Stop Sounds");
      break;

    /* ops with args */
    case SWFACTION_PUSHDATA:
    {
      int type;
      int start = fileOffset;

      while(fileOffset < start+length)
      {
	switch(type = readUInt8(f))
	{
	  case 0: /* string */
	    println("Push String: %s", readString(f));
	    break;
	  case 1: /* property */
	    readUInt16(f); /* always 0? */
	    println("Push Property: %04x", readUInt16(f));
	    break;
	  case 2: /* null */
	    println("Push NULL");
	    break;
	  case 3: /* ??? */
	    println("Push type 3- ??");
	    break;
	  case 4: 
	    println("Push register %i", readUInt8(f));
	    break;
	  case 5:
	    if(readUInt8(f))
	      println("Push true");
	    else
	      println("Push false");
	    break;
	  case 6: /* double */
	    println("Push %f", readDouble(f));
	    break;
	  case 7: /* int */
	    println("Push %i", readSInt32(f));
	    break;
	  case 8: /* dictionary */
	    println("Push \"%s\"", dictionary[readUInt8(f)]);
	    break;
	  case 9: /* dictionary */
	    println("Push \"%s\"", dictionary[readSInt16(f)]);
	  default:
	    println("unknown push type: %i", type);
	}
      }
      break;
    }
    case SWFACTION_GOTOFRAME:
      println("Goto Frame %i", readUInt16(f));
      break;
    case SWFACTION_GETURL:
    {
      char *url = readString(f);
      println("Get URL \"%s\" target \"%s\"", url, readString(f));
      break;
    }
    case SWFACTION_WAITFORFRAMEEXPRESSION:
      println("Wait For Frame Expression, skip %i\n", readUInt8(f));
      break;
    case SWFACTION_BRANCHALWAYS:
      println("Branch Always %i", readSInt16(f));
      break;
    case SWFACTION_GETURL2:
      {
	int flags = readUInt8(f);

	const char *op = (flags & 0x80) ? "Get URL2 (loadvariables)" : "Get URL2";
	const char *tgt = (flags & 0x40) ? " into target" : "";

	switch(flags & 0x03)
	{
          case 0: println("%s%s (Don't send)", op, tgt); break;
          case 1: println("%s%s (GET)", op, tgt); break;
          case 2: println("%s%s (POST)", op, tgt); break;
	}
      }
      break;
    case SWFACTION_BRANCHIFTRUE:
      println("Branch If True %i", readSInt16(f));
      break;
    case SWFACTION_CALLFRAME:
      println("Call Frame");
      dumpBytes(f, length);
      break;
    case SWFACTION_GOTOEXPRESSION:
      print("Goto Expression");
      if(readUInt8(f) == 1)
	printf(" and Play\n");
      else
	printf(" and Stop\n");
      break;
    case SWFACTION_WAITFORFRAME:
    {
      int frame = readUInt16(f);
      println("Wait for frame %i else skip %i", frame, readUInt8(f));
      break;
    }
    case SWFACTION_SETTARGET:
      println("Set Target %s", readString(f));
      break;
    case SWFACTION_GOTOLABEL:
      println("Goto Label %s", readString(f));
      break;
    case SWFACTION_END:
      return 0;
      break;

    /* f5 ops */
    case SWFACTION_DELETE:
      println("Delete");
      break;
    case SWFACTION_VAR:
      println("Var");
      break;
    case SWFACTION_VAREQUALS:
      println("Var assign");
      break;
    case SWFACTION_INITARRAY:
      println("Init array");
      break;
    case SWFACTION_INITOBJECT:
      println("Init object");
      break;
    case SWFACTION_CALLFUNCTION:
      println("call function");
      break;
    case SWFACTION_RETURN:
      println("return");
      break;
    case SWFACTION_MODULO:
      println("modulo");
      break;
    case SWFACTION_NEW:
      println("new");
      break;
    case SWFACTION_TYPEOF:
      println("typeof");
      break;
    case SWFACTION_NEWADD:
      println("new add");
      break;
    case SWFACTION_NEWLESSTHAN:
      println("new less than");
      break;
    case SWFACTION_NEWEQUALS:
      println("new equals");
      break;
    case SWFACTION_DUP:
      println("dup");
      break;
    case SWFACTION_SWAP:
      println("swap");
      break;
    case SWFACTION_GETMEMBER:
      println("get member");
      break;
    case SWFACTION_SETMEMBER:
      println("set member");
      break;
    case SWFACTION_INCREMENT:
      println("increment");
      break;
    case SWFACTION_CALLMETHOD:
      println("call method");
      break;
    case SWFACTION_BITWISEAND:
      println("bitwise and");
      break;
    case SWFACTION_BITWISEOR:
      println("bitwise or");
      break;
    case SWFACTION_BITWISEXOR:
      println("bitwise xor");
      break;
    case SWFACTION_SHIFTLEFT:
      println("shift left");
      break;
    case SWFACTION_SHIFTRIGHT:
      println("shift right");
      break;
    case SWFACTION_SHIFTRIGHT2:
      println("shift right 2");
      break;

    case SWFACTION_CONSTANTPOOL:
    {
      int i, n = readUInt16(f);
      print("declare dictionary:");

      for(i=0; i<n; ++i)
	printf(" %s%c", dictionary[i]=readString(f), (i<n-1)?',':'\n');

      break;
    }
    case SWFACTION_WITH:
    {
      println("with");

      ++gIndent;
      printDoAction(f, readUInt16(f));
      --gIndent;

      break;
    }
    case SWFACTION_DEFINEFUNCTION:
    {
      char *name = readString(f);
      int n = readUInt16(f);

      print("function ");
      print(name);
      putchar('(');

      if(n > 0)
      {
	printf("%s", readString(f));
	--n;
      }

      for(; n>0; --n)
	printf(", %s", readString(f));

      putchar(')');
      putchar('\n');

      ++gIndent;
      printDoAction(f, readUInt16(f));
      --gIndent;

      break;
    }

    case SWFACTION_ENUMERATE:
      println("enumerate");
      break;

    case SWFACTION_SETREGISTER:
      println("set register %i", readUInt8(f));
      break;

/* f6 actions */
    case SWFACTION_INSTANCEOF:
      println("instanceof");
      break;
    case SWFACTION_STRICTEQ:
      println("strict_equals");
      break;
    case SWFACTION_ENUM2:
      println("enum2");
      break;

    default:
      println("Unknown Action: %02X", type);
      dumpBytes(f, length);
  }

  return 1;
}

void printDoAction(Buffer f, int length)
{
  int end;

  if(!f)
    return;

  end = fileOffset + length;

  while(fileOffset < end && printActionRecord(f))
    ;
}
