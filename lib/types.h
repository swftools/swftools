#ifndef __rfxtypes_h__
#define __rfxtypes_h__

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

#ifdef WORDS_BIGENDIAN
#define SWAP16(s) ((((s)>>8)&0x00ff)|(((s)<<8)&0xff00))
#define SWAP32(s) (SWAP16(((s)>>16)&0x0000ffff)|((SWAP16(s)<<16)&0xffff0000))
#define REVERSESWAP16(x) (x)
#define REVERSESWAP32(x) (x)
#else
#define SWAP16(x) (x)
#define SWAP32(x) (x)
#define REVERSESWAP16(s) ((((s)>>8)&0x00ff)|(((s)<<8)&0xff00))
#define REVERSESWAP32(s) (REVERSESWAP16(((s)>>16)&0x0000ffff)|((REVERSESWAP16(s)<<16)&0xffff0000))
#endif

// SWF Types

typedef         unsigned long long  U64;
typedef         signed long long    S64;
typedef         unsigned	    U32;
typedef         signed		    S32;
typedef         unsigned short      U16;
typedef         signed short        S16;
typedef         unsigned char       U8;
typedef         signed char         S8;

#endif
