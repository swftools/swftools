/*
    Ming, an SWF output library
    Copyright (C) 2002  Opaque Industries - http://www.opaque.net/

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef WIN32
	#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "libming.h"
#include "compile.h"
#include "action.h"
#include "blocks/error.h"


static int nConstants = {0}, maxConstants = {0}, sizeConstants = {0};
static char **constants;

/* XXX - temp hack until we check at compile time */

enum
{
	SWF_BIG_ENDIAN,
	SWF_LITTLE_ENDIAN
};

static int byteorder;

void checkByteOrder()
{
	unsigned int x;
	unsigned char *p;

	x = 0x01020304;
	p = (unsigned char *)&x;

	if(*p == 1)
		byteorder = SWF_BIG_ENDIAN;
	else
		byteorder = SWF_LITTLE_ENDIAN;
}


char *stringConcat(char *a, char *b)
{
	if ( a != NULL )
	{
		if ( b != NULL )
		{
			a = (char*)realloc(a, strlen(a)+strlen(b)+1);
			strcat(a, b);
			free(b);
		}

		return a;
	}
	else
		return b;
}

void bufferPatchLength(Buffer buffer, int back)
{
	unsigned char *output = buffer->buffer;
	int len = bufferLength(buffer);

	output[len-back-1] = (back>>8) & 0xff;
	output[len-back-2] = back & 0xff;
}


/* add len more bytes to length of the pushdata opcode pointed to by
	 buffer->pushloc */

void bufferPatchPushLength(Buffer buffer, int len)
{
	int oldsize;

	if(buffer->pushloc != NULL)
	{
		oldsize = (buffer->pushloc[0] & 0xff) | ((buffer->pushloc[1] & 0xff) << 8);
		oldsize += len;
		buffer->pushloc[0] = oldsize & 0xff;
		buffer->pushloc[1] = (oldsize >> 8) & 0xff;
	}
	else
		SWF_error("problem with bufferPatchPushLength\n");
}


static int useConstants = 1;
void Ming_useConstants(int flag)
{	useConstants = flag;
}


int addConstant(const char *s)
{
	int i;

	for(i=0; i<nConstants; ++i)
	{
		if(strcmp(s, constants[i]) == 0)
			return i;
	}

	/* Don't let constant pool biggern then allowed */
	if ( sizeConstants+strlen(s)+1 > MAXCONSTANTPOOLSIZE ) return -1;

	if(nConstants == maxConstants)
		constants = (char **) realloc(constants, (maxConstants += 64) * sizeof(char *));
	constants[nConstants] = strdup(s);
	sizeConstants += (strlen(s)+1);
	return nConstants++;
}

int bufferWriteConstants(Buffer out)
{
	int i, len=2;

	if(nConstants == 0)
		return 0;

	bufferWriteU8(out, SWFACTION_CONSTANTPOOL);
	bufferWriteS16(out, 0); /* length */
	bufferWriteS16(out, nConstants);

	for(i=0; i<nConstants; ++i)
	{
		len += bufferWriteHardString(out,(byte*) constants[i], strlen(constants[i])+1);
		free(constants[i]);
	}

	nConstants = 0;
	sizeConstants = 0;
	bufferPatchLength(out, len);

	return len+3;
}

Buffer newBuffer()
{
	Buffer out = (Buffer)malloc(BUFFER_SIZE);
	memset(out, 0, BUFFER_SIZE);

	out->buffer = (byte*)malloc(BUFFER_INCREMENT);
	out->pos = out->buffer;
	*(out->pos) = 0;
	out->buffersize = out->free = BUFFER_INCREMENT;
	out->pushloc = NULL;

	return out;
}

void destroyBuffer(Buffer out)
{
	free(out->buffer);
	free(out);
}

int bufferLength(Buffer out)
{
	if(out)
		return (out->pos)-(out->buffer);
	else
		return 0;
}

/* make sure there's enough space for bytes bytes */
void bufferCheckSize(Buffer out, int bytes)
{
	if(bytes > out->free)
	{
		int New = BUFFER_INCREMENT * ((bytes-out->free-1)/BUFFER_INCREMENT + 1);

		int num = bufferLength(out); /* in case buffer gets displaced.. */
		unsigned char *newbuf = (unsigned char*)realloc(out->buffer, out->buffersize+New);

		if(newbuf != out->buffer)
		{
			int pushd;

			if(out->pushloc)
	pushd = out->pos - out->pushloc;

			out->pos = newbuf+num;

			if(out->pushloc)
	out->pushloc = out->pos - pushd;
		}

		out->buffer = newbuf;
		out->buffersize += New;
		out->free += New;
	}
}

int bufferWriteData(Buffer b, const byte *data, int length)
{
	int i;

	bufferCheckSize(b, length);

	for(i=0; i<length; ++i)
		bufferWriteU8(b, data[i]);

	return length;
}

int bufferWriteBuffer(Buffer a, Buffer b)
{
	if(!a)
		return 0;

	if(b)
		return bufferWriteData(a, b->buffer, bufferLength(b));

	return 0;
}

/* if a's last op and b's first op are both PUSHDATA, concat into one op */

int bufferWriteDataAndPush(Buffer a, Buffer b)
{
	int i, pushd;

	byte *data = b->buffer;
	int length = b->pos - b->buffer;

	if(a->pushloc && (b->buffer[0] == SWFACTION_PUSHDATA) && SWF_versionNum > 4)
	{
		pushd = (b->buffer[1] & 0xff) | ((b->buffer[2] & 0xff) << 8);
		bufferPatchPushLength(a, pushd);
		data += 3;
		length -= 3;
	}

	if(b->pushloc)
		pushd = b->pos - b->pushloc;

	bufferCheckSize(a, length);

	for(i=0; i<length; ++i)
		bufferWriteU8(a, data[i]);

	if(a->pushloc &&
		 (b->buffer[0] == SWFACTION_PUSHDATA) && (b->pushloc == b->buffer+1))
		; /* b is just one pushdata, so do nothing.. */
	else if(b->pushloc)
		a->pushloc = a->pos - pushd;
	else
		a->pushloc = 0;

	return length;
}

int bufferConcat(Buffer a, Buffer b)
{
	int len;

	if(!a)
		return 0;

	if(b)
	{	len = bufferWriteDataAndPush(a, b);
		destroyBuffer(b);
	}

	return len;
}

int bufferWriteOp(Buffer out, int data)
{
	bufferWriteU8(out, data);
	out->pushloc = NULL;

	return 1;
}

int bufferWritePushOp(Buffer out)
{
	bufferWriteU8(out, SWFACTION_PUSHDATA);
	out->pushloc = out->pos;

	return 1;
}

int bufferWriteU8(Buffer out, int data)
{
	bufferCheckSize(out, 1);
	*(out->pos) = data;
	out->pos++;
	out->free--;

	return 1;
}

int bufferWriteS16(Buffer out, int data)
{
	if(data < 0)
		data = (1<<16)+data;

	bufferWriteU8(out, data%256);
	data >>= 8;
	bufferWriteU8(out, data%256);

	return 2;
}

int bufferWriteHardString(Buffer out, byte *string, int length)
{
	int i;

	for(i=0; i<length; ++i)
		bufferWriteU8(out, string[i]);

	return length;
}

int bufferWriteConstantString(Buffer out, byte *string, int length)
{
	int n;

	if(SWF_versionNum < 5)
		return -1;

	if(useConstants)
		n = addConstant((char*) string);
	else
		n = -1;

	if(n == -1)
	{
		bufferWriteU8(out, PUSH_STRING);
		return bufferWriteHardString(out, string, length) + 1;
	}
	else if(n < 256)
	{
		bufferWriteU8(out, PUSH_CONSTANT);
		return bufferWriteU8(out, n) + 1;
	}
	else
	{
		bufferWriteU8(out, PUSH_CONSTANT16);
		return bufferWriteS16(out, n) + 1;
	}
}

int bufferWriteString(Buffer out, byte *string, int length)
{
	if(SWF_versionNum < 5)
	{
		bufferWritePushOp(out);
		bufferWriteS16(out, length+1);
		bufferWriteU8(out, PUSH_STRING);
		bufferWriteHardString(out, string, length);

		return 4 + length;
	}
	else
	{
		int l;

		if(out->pushloc == NULL)
		{
			bufferWritePushOp(out);
			bufferWriteS16(out, 0);
		}

		l = bufferWriteConstantString(out, string, length);

		bufferPatchPushLength(out, l);
		return l;
	}
}

int bufferWriteInt(Buffer out, int i)
{
	int len = 0;
	unsigned char *p = (unsigned char *)&i;

	if(out->pushloc == NULL || SWF_versionNum < 5)
	{
		len = 3;
		bufferWritePushOp(out);
		bufferWriteS16(out, 5);
	}
	else
		bufferPatchPushLength(out, 5);

	bufferWriteU8(out, PUSH_INT);

	if(byteorder == SWF_LITTLE_ENDIAN)
	{
		bufferWriteU8(out, p[0]);
		bufferWriteU8(out, p[1]);
		bufferWriteU8(out, p[2]);
		bufferWriteU8(out, p[3]);
	}
	else
	{
		bufferWriteU8(out, p[3]);
		bufferWriteU8(out, p[2]);
		bufferWriteU8(out, p[1]);
		bufferWriteU8(out, p[0]);
	}

	return len + 5;
}

int bufferWriteDouble(Buffer out, double d)
{
	int len = 0;
	unsigned char *p = (unsigned char *)&d;

	if(out->pushloc == NULL || SWF_versionNum < 5)
	{
		len = 3;
		bufferWritePushOp(out);
		bufferWriteS16(out, 9);
	}
	else
		bufferPatchPushLength(out, 5);

	bufferWriteU8(out, PUSH_DOUBLE);

	if(byteorder == SWF_LITTLE_ENDIAN)
	{
		bufferWriteU8(out, p[4]);
		bufferWriteU8(out, p[5]);
		bufferWriteU8(out, p[6]);
		bufferWriteU8(out, p[7]);
		bufferWriteU8(out, p[0]);
		bufferWriteU8(out, p[1]);
		bufferWriteU8(out, p[2]);
		bufferWriteU8(out, p[3]);
	}
	else
	{
		bufferWriteU8(out, p[3]);
		bufferWriteU8(out, p[2]);
		bufferWriteU8(out, p[1]);
		bufferWriteU8(out, p[0]);
		bufferWriteU8(out, p[7]);
		bufferWriteU8(out, p[6]);
		bufferWriteU8(out, p[5]);
		bufferWriteU8(out, p[4]);
	}

	return len + 9;
}

int bufferWriteNull(Buffer out)
{
	int len = 0;

	if(out->pushloc == NULL || SWF_versionNum < 5)
	{
		len = 3;
		bufferWritePushOp(out);
		bufferWriteS16(out, 1);
	}
	else
		bufferPatchPushLength(out, 1);

	bufferWriteU8(out, PUSH_NULL);

	return len + 1;
}

int bufferWriteBoolean(Buffer out, int val)
{
	int len = 0;

	if(out->pushloc == NULL || SWF_versionNum < 5)
	{
		len = 3;
		bufferWritePushOp(out);
		bufferWriteS16(out, 2);
	}
	else
		bufferPatchPushLength(out, 2);

	bufferWriteU8(out, PUSH_BOOLEAN);
	bufferWriteU8(out, val ? 1 : 0);

	return len + 2;
}

int bufferWriteRegister(Buffer out, int num)
{
	int len = 0;

	if(out->pushloc == NULL || SWF_versionNum < 5)
	{
		len = 3;
		bufferWritePushOp(out);
		bufferWriteS16(out, 2);
	}
	else
		bufferPatchPushLength(out, 2);

	bufferWriteU8(out, PUSH_REGISTER);
	bufferWriteU8(out, num);

	return len + 2;
}

int bufferWriteSetRegister(Buffer out, int num)
{
	bufferWriteU8(out, SWFACTION_SETREGISTER);
	bufferWriteS16(out, 1);
	bufferWriteU8(out, num);
	return 4;
}

void lower(char *s)
{
	while(*s)
	{
		*s = tolower(*s);
		++s;
	}
}

/* this code will eventually help to pop extra values off the
 stack and make sure that continue and break address the proper
 context
 */
static enum ctx *ctx_stack = {0};
static int ctx_count = {0}, ctx_len = {0};
void addctx(enum ctx val)
{	if(ctx_count >= ctx_len)
		ctx_stack = (enum ctx*) realloc(ctx_stack, (ctx_len += 10) * sizeof(enum ctx));
	ctx_stack[ctx_count++] = val;
}
void delctx(enum ctx val)
{	if(ctx_count <= 0 || ctx_stack[--ctx_count] != val)
		SWF_error("consistency check in delctx");
}

int chkctx(enum ctx val)
{	int n, ret = 0;
	switch(val)
	{	case CTX_FUNCTION:
			for(n = ctx_count ; --n >= 0 ; )
				switch(ctx_stack[n])
				{	case CTX_SWITCH:
					case CTX_FOR_IN:
						ret++;
						break;
					case CTX_FUNCTION:
						return ret;
					default: ; /* computers are stupid */
				}
			return -1;
		case CTX_BREAK:
			for(n = ctx_count ; --n >= 0 ; )
				switch(ctx_stack[n])
				{	case CTX_SWITCH:
					case CTX_LOOP:
						return 0;
					case CTX_FOR_IN:
						return 1;
					case CTX_FUNCTION:
						return -1;
					default: ; /* computers are stupid */
				}
		case CTX_CONTINUE:
			for(n = ctx_count ; --n >= 0 ; )
				switch(ctx_stack[n])
				{	case CTX_LOOP:
					case CTX_FOR_IN:
						return 0;
					case CTX_FUNCTION:
						return -1;
					default: ; /* computers are stupid */
				}
		default: ; /* computers are stupid */
	}
	return 0;
}

/* replace MAGIC_CONTINUE_NUMBER and MAGIC_BREAK_NUMBER with jumps to
	 head or tail, respectively */
/* jump offset is relative to end of jump instruction */
/* I can't believe this actually worked */

void bufferResolveJumps(Buffer out)
{
	byte *p = out->buffer;
	int l, target;

	while(p < out->pos)
	{
		if(*p & 0x80) /* then it's a multibyte instruction */
		{
			if(*p == SWFACTION_BRANCHALWAYS)
			{
	p += 3; /* plus instruction plus two-byte length */

	if(*p == MAGIC_CONTINUE_NUMBER_LO &&
		 *(p+1) == MAGIC_CONTINUE_NUMBER_HI)
	{
		target = out->buffer - (p+2);
		*p = target & 0xff;
		*(p+1) = (target>>8) & 0xff;
	}
	else if(*p == MAGIC_BREAK_NUMBER_LO &&
		*(p+1) == MAGIC_BREAK_NUMBER_HI)
	{
		target = out->pos - (p+2);
		*p = target & 0xff;
		*(p+1) = (target>>8) & 0xff;
	}

	p += 2;
			}
			else
			{
	++p;
	l = *p;
	++p;
	l += *p<<8;
	++p;

	p += l;
			}
		}
		else
			++p;
	}
}

// handle SWITCH statement

void bufferResolveSwitch(Buffer buffer, struct switchcases *slp)
{	struct switchcase *scp;
	int n, len;
	unsigned char *output;
			
	len = bufferLength(buffer);
	for(n = 0, scp = slp->list ; n < slp->count ; n++, scp++)
	{	scp->actlen = bufferLength(scp->action);
		if((n < slp->count-1))
			scp->actlen += 5;
		if(scp->cond)
		{	scp->condlen = bufferLength(scp->cond) + 8;
			bufferWriteOp(buffer, SWFACTION_DUP);
			bufferConcat(buffer, scp->cond);
			bufferWriteOp(buffer, SWFACTION_NEWEQUALS);
			bufferWriteOp(buffer, SWFACTION_LOGICALNOT);
			bufferWriteOp(buffer, SWFACTION_BRANCHIFTRUE);
			bufferWriteS16(buffer, 2);
			bufferWriteS16(buffer, scp->actlen);
		}
		else
			scp->condlen = 0;
		bufferConcat(buffer, scp->action);
		bufferWriteOp(buffer, SWFACTION_BRANCHALWAYS);
		bufferWriteS16(buffer, 2);
		bufferWriteS16(buffer, scp->isbreak ? MAGIC_BREAK_NUMBER : 0);
		if(!scp->cond)
		{	slp->count = n+1;
			break;
		}
	}
	for(n = 0, scp = slp->list ; n < slp->count ; n++, scp++)
	{	len += scp->condlen;
		output = buffer->buffer + len;
		if((n < slp->count-1) && !scp->isbreak)
		{	output[scp->actlen-2] = (scp+1)->condlen & 0xff;
			output[scp->actlen-1] = (scp+1)->condlen >> 8;
		}
		len += scp->actlen;
	}
}
	
int lookupSetProperty(char *string)
{
	lower(string);

	if(strcmp(string,"x")==0)		return 0x0000;
	if(strcmp(string,"y")==0)		return 0x3f80;
	if(strcmp(string,"xscale")==0)	return 0x4000;
	if(strcmp(string,"yscale")==0)	return 0x4040;
	if(strcmp(string,"alpha")==0)		return 0x40c0;
	if(strcmp(string,"visible")==0)	return 0x40e0;
	if(strcmp(string,"rotation")==0)	return 0x4120;
	if(strcmp(string,"name")==0)		return 0x4140;
	if(strcmp(string,"quality")==0)	return 0x4180;
	if(strcmp(string,"focusrect")==0)	return 0x4188;
	if(strcmp(string,"soundbuftime")==0)	return 0x4190;

	SWF_error("No such property: %s\n", string);
	return -1;
}

int bufferWriteSetProperty(Buffer out, char *string)
{
	int property = lookupSetProperty(string);

	bufferWriteU8(out, SWFACTION_PUSHDATA);
	bufferWriteS16(out, 5);
	bufferWriteU8(out, PUSH_PROPERTY);
	bufferWriteS16(out, 0);
	bufferWriteS16(out, property);

	return 8;
}

int bufferWriteWTHITProperty(Buffer out)
{
	bufferWriteU8(out, SWFACTION_PUSHDATA);
	bufferWriteS16(out, 5);
	bufferWriteU8(out, PUSH_PROPERTY);
	bufferWriteS16(out, 0);
	bufferWriteS16(out, 0x4680);

	return 8;
}

const char *lookupGetProperty(char *string)
{
	lower(string);

	if(strcmp(string,"x")==0)		return "0";
	if(strcmp(string,"y")==0)		return "1";
	if(strcmp(string,"xscale")==0)	return "2";
	if(strcmp(string,"yscale")==0)	return "3";
	if(strcmp(string,"currentframe")==0)	return "4";
	if(strcmp(string,"totalframes")==0)	return "5";
	if(strcmp(string,"alpha")==0)		return "6";
	if(strcmp(string,"visible")==0)	return "7";
	if(strcmp(string,"width")==0)		return "8";
	if(strcmp(string,"height")==0)	return "9";
	if(strcmp(string,"rotation")==0)	return "10";
	if(strcmp(string,"target")==0)	return "11";
	if(strcmp(string,"framesloaded")==0)	return "12";
	if(strcmp(string,"name")==0)		return "13";
	if(strcmp(string,"droptarget")==0)	return "14";
	if(strcmp(string,"url")==0)		return "15";
	if(strcmp(string,"quality")==0)	return "16";
	if(strcmp(string,"focusrect")==0)	return "17";
	if(strcmp(string,"soundbuftime")==0)	return "18";

	SWF_error("No such property: %s\n", string);
	return "";
}

int bufferWriteGetProperty(Buffer out, char *string)
{
	const char *property = lookupGetProperty(string);

	bufferWriteU8(out, SWFACTION_PUSHDATA);
	bufferWriteS16(out, strlen(property)+2);
	bufferWriteU8(out, PUSH_STRING);

	return 4 + bufferWriteData(out, (byte*) property, strlen(property)+1);
}


/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 */
