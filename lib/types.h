#ifndef __rfxtypes_h__
#define __rfxtypes_h__

#include "../config.h"

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

/* little/big endian stuff */

#define PUT8(ptr,x) {((U8*)(ptr))[0]=x;}
#define PUT16(ptr,x) {((U8*)(ptr))[0]=(U8)(x);((U8*)(ptr))[1]=(U8)((x)>>8);}
#define PUT32(ptr,x) {((U8*)(ptr))[0]=(U8)(x);((U8*)(ptr))[1]=(U8)((x)>>8);((U8*)(ptr))[2]=(U8)((x)>>16);((U8*)(ptr))[3]=(U8)((x)>>24);}
#define GET16(ptr) (((U16)(((U8*)(ptr))[0]))+(((U16)(((U8*)(ptr))[1]))<<8))
#define GET32(ptr) (((U16)(((U8*)(ptr))[0]))+(((U16)(((U8*)(ptr))[1]))<<8)+(((U16)(((U8*)(ptr))[2]))<<16)+(((U16)(((U8*)(ptr))[3]))<<24))

#define SWAP16(s) ((((s)>>8)&0x00ff)|(((s)<<8)&0xff00))
#define SWAP32(s) (SWAP16(((s)>>16)&0x0000ffff)|((SWAP16(s)<<16)&0xffff0000))

#ifdef WORDS_BIGENDIAN
#define LE_16_TO_NATIVE(s) SWAP16(s)
#define LE_32_TO_NATIVE(s) SWAP32(s)
#define BE_16_TO_NATIVE(x) (x)
#define BE_32_TO_NATIVE(x) (x)
#else
#define LE_16_TO_NATIVE(x) (x)
#define LE_32_TO_NATIVE(x) (x)
#define BE_16_TO_NATIVE(s) SWAP16(s)
#define BE_32_TO_NATIVE(s) SWAP32(s)
#endif

// SWF Types

#if SIZEOF_SIGNED_LONG_LONG != 8
#error "no way to define 64 bit integer"
#endif
#if SIZEOF_SIGNED != 4
#error "don't know how to define 32 bit integer"
#endif
#if SIZEOF_SIGNED_SHORT != 2
#error "don't know how to define 16 bit integer"
#endif
#if SIZEOF_SIGNED_CHAR != 1
#error "don't know how to define 8 bit integer"
#endif

typedef         unsigned long long  U64;
typedef         signed long long    S64;
typedef         unsigned	    U32;
typedef         signed		    S32;
typedef         unsigned short      U16;
typedef         signed short        S16;
typedef         unsigned char       U8;
typedef         signed char         S8;

#if SIZEOF_VOIDP == SIZEOF_SIGNED_LONG_LONG
typedef unsigned long long ptroff_t;
#elif SIZEOF_VOIDP == SIZEOF_SIGNED
typedef unsigned ptroff_t;
#else
#error "Unknown pointer size"
#endif

#endif
