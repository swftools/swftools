/* compile.h
 * 
 * $Id: compile.h,v 1.1 2004/05/08 17:14:20 kramm Exp $
 * 
 * Notice: This header file contains declarations of functions and types that
 * are just used internally. All library functions and types that are supposed
 * to be publicly accessable are defined in ./src/ming.h.
 */

#ifndef SWF_COMPILE_H_INCLUDED
#define SWF_COMPILE_H_INCLUDED

#include "ming.h"

typedef struct _buffer *Buffer;

/* shut up bison.simple */
void yyerror(char *msg);
int yylex();

#ifndef max
  #define max(x,y)	(((x)>(y))?(x):(y))
#endif

enum
{
  PUSH_STRING = 0,
  PUSH_PROPERTY = 1,
  PUSH_NULL = 2,
  PUSH_UNDEF = 3,
  PUSH_REGISTER = 4,
  PUSH_BOOLEAN = 5,
  PUSH_DOUBLE = 6,
  PUSH_INT = 7,
  PUSH_CONSTANT = 8,
  PUSH_CONSTANT16 = 9
};

typedef enum
{
  FUNCTION_RANDOM,
  FUNCTION_LENGTH,
  FUNCTION_TIME,
  FUNCTION_INT,
  FUNCTION_CONCAT,
  FUNCTION_DUPLICATECLIP
} SWFActionFunction;

typedef enum
{
  GETURL_METHOD_NOSEND = 0,
  GETURL_METHOD_GET    = 1,
  GETURL_METHOD_POST   = 2
} SWFGetUrl2Method;

#define GETURL_LOADMOVIE 0x40
#define GETURL_LOADVARIABLES 0x80

#define MAGIC_CONTINUE_NUMBER 0x7FFE
#define MAGIC_BREAK_NUMBER    0x7FFF

#define MAGIC_CONTINUE_NUMBER_LO 0xFE
#define MAGIC_CONTINUE_NUMBER_HI 0x7F
#define MAGIC_BREAK_NUMBER_LO    0xFF
#define MAGIC_BREAK_NUMBER_HI    0x7F

#define BUFFER_INCREMENT 128

struct _buffer
{
  byte *buffer;
  byte *pos;
  int buffersize;
  int free;
  byte *pushloc;
};

#define BUFFER_SIZE sizeof(struct _buffer)

struct switchcase
{	Buffer cond, action;
	int condlen, actlen, isbreak;
};

struct switchcases
{
	struct switchcase *list;
	int count;
};

enum ctx
{
	CTX_FUNCTION = 1,
	CTX_LOOP,
	CTX_FOR_IN,
	CTX_SWITCH,

	CTX_BREAK,
	CTX_CONTINUE
};

void addctx(enum ctx val);
void delctx(enum ctx val);
int chkctx(enum ctx val);

void checkByteOrder();

/* create/destroy buffer object */
Buffer newBuffer();
void destroyBuffer(Buffer out);
int bufferConcat(Buffer a, Buffer b);        /* destroys b. */
int bufferWriteBuffer(Buffer a, Buffer b);   /* doesn't. */

/* utilities for writing */
void bufferGrow(Buffer out);
void bufferCheckSize(Buffer out, int bytes);

int bufferLength(Buffer out);

/* constant pool stuff */
int addConstant(const char *s);
int bufferWriteConstants(Buffer out);
#define MAXCONSTANTPOOLSIZE 65533

/* write data to buffer */
int bufferWriteOp(Buffer out, int data);
int bufferWritePushOp(Buffer out);
int bufferWriteU8(Buffer out, int data);
int bufferWriteS16(Buffer out, int data);
int bufferWriteData(Buffer out, const byte *buffer, int bytes);
int bufferWriteHardString(Buffer out, byte *string, int length);
int bufferWriteConstantString(Buffer out, byte *string, int length);
int bufferWriteString(Buffer out, byte *string, int length);
#ifdef __cplusplus
/* helper function to avoid many casts */
inline int bufferWriteString(Buffer out, char *string, int length) {
	return bufferWriteString(out,(byte*) string, length); }
#endif
int bufferWriteInt(Buffer out, int i);
int bufferWriteDouble(Buffer out, double d);
int bufferWriteNull(Buffer out);
int bufferWriteBoolean(Buffer out, int val);
int bufferWriteRegister(Buffer out, int num);
int bufferWriteSetRegister(Buffer out, int num);
int bufferWriteGetProperty(Buffer out, char *string);
int bufferWriteSetProperty(Buffer out, char *string);
int bufferWriteWTHITProperty(Buffer out);

/* concat b to a, destroy b */
char *stringConcat(char *a, char *b);

/* resolve magic number standins to relative offsets */
void bufferResolveJumps(Buffer out);
void bufferResolveSwitch(Buffer buffer, struct switchcases *slp);

/* rather than setting globals... */
void swf4ParseInit(const char *string, int debug);
void swf5ParseInit(const char *string, int debug);

int swf4parse(void *b);
int swf5parse(void *b);

#endif /* SWF_COMPILE_H_INCLUDED */
