/*	$FreeBSD: src/sys/crypto/md5.h,v 1.4 2002/03/20 05:13:50 alfred Exp $	*/
/*	$KAME: md5.h,v 1.4 2000/03/27 04:36:22 sumikawa Exp $	*/

/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "../config.h"

#include <stdlib.h>
#include <string.h>
#include <memory.h>

#ifndef _NETINET6_MD5_H_
#define _NETINET6_MD5_H_

#define MD5_BUFLEN	64

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1
#endif
#ifndef BIG_ENDIAN
#define BIG_ENDIAN 2
#endif
#ifndef BYTE_ORDER
#ifdef WORDS_BIGENDIAN
#define BYTE_ORDER BIG_ENDIAN
#else
#define BYTE_ORDER LITTLE_ENDIAN
#endif
#endif

#include "types.h"

typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned char u_char;

typedef struct {
	union {
		U32	md5_state32[4];
		U8	md5_state8[16];
	} md5_st;

#define md5_sta		md5_st.md5_state32[0]
#define md5_stb		md5_st.md5_state32[1]
#define md5_stc		md5_st.md5_state32[2]
#define md5_std		md5_st.md5_state32[3]
#define md5_st8		md5_st.md5_state8

	union {
		U64	md5_count64;
		U8	md5_count8[8];
	} md5_count;
#define md5_n	md5_count.md5_count64
#define md5_n8	md5_count.md5_count8

	u_int	md5_i;
	U8	md5_buf[MD5_BUFLEN];
} md5_ctxt;

static void md5_init(md5_ctxt *);
static void md5_loop(md5_ctxt *, const U8 *, u_int);
static void md5_pad(md5_ctxt *);
static void md5_result(U8 *, md5_ctxt *);

/* compatibility */
#define MD5_CTX		md5_ctxt
#define MD5Init(x)	md5_init((x))
#define MD5Update(x, y, z)	md5_loop((x), (y), (z))
#define MD5Final(x, y) \
do {				\
	md5_pad((y));		\
	md5_result((x), (y));	\
} while (0)

#endif /* ! _NETINET6_MD5_H_*/

/*-
 * Copyright (c) 2003 Poul-Henning Kamp
 * Copyright (c) 1999
 *      University of California.  All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


/*
 * UNIX password
 */

#ifndef HAVE_BCOPY
void bcopy(const void*src, void*dest, int len) 
{
    memcpy(dest, src, len);
}
#endif

#ifndef HAVE_BZERO
void bzero(void*mem, int len) 
{
    memset(mem, 0, len);
}
#endif

#define MD4_SIZE 16
#define MD5_SIZE 16

static char itoa64[] =          /* 0 ... 63 => ascii - 64 */
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static void
_crypt_to64(char *s, u_long v, int n)
{
        while (--n >= 0) {
                *s++ = itoa64[v&0x3f];
                v >>= 6;
        }
}

void hash_md5(const unsigned char*buf, int len, unsigned char*dest)
{
    u_char final[MD5_SIZE];

    MD5_CTX ctx;
    MD5Init(&ctx);
    MD5Update(&ctx, buf, len);
    MD5Final(dest, &ctx);
}

void* initialize_md5()
{
    MD5_CTX* ctx = malloc(sizeof(MD5_CTX));
    memset(ctx, 0, sizeof(MD5_CTX));
    MD5Init(ctx);
    return ctx;
}
void update_md5(void*ctx, unsigned char*data, int len)
{
    MD5Update(ctx, data, len);
}
void finish_md5(void*ctx, unsigned char*dest)
{
    MD5Final(dest, ctx);
    free(ctx);
}


char * crypt_md5(const char *pw, const char *salt)
{
	MD5_CTX	ctx,ctx1;
	unsigned long l;
	int sl, pl;
	u_int i;
	u_char final[MD5_SIZE];
	static const char *sp, *ep;
	static char passwd[120], *p;
	static const char *magic = "$1$";

	/* Refine the Salt first */
	sp = salt;

	/* If it starts with the magic string, then skip that */
	if(!strncmp(sp, magic, strlen(magic)))
		sp += strlen(magic);

	/* It stops at the first '$', max 8 chars */
	for(ep = sp; *ep && *ep != '$' && ep < (sp + 8); ep++)
		continue;

	/* get the length of the true salt */
	sl = ep - sp;

	MD5Init(&ctx);

	/* The password first, since that is what is most unknown */
	MD5Update(&ctx, (const u_char *)pw, strlen(pw));

	/* Then our magic string */
	MD5Update(&ctx, (const u_char *)magic, strlen(magic));

	/* Then the raw salt */
	MD5Update(&ctx, (const u_char *)sp, (u_int)sl);

	/* Then just as many characters of the MD5(pw,salt,pw) */
	MD5Init(&ctx1);
	MD5Update(&ctx1, (const u_char *)pw, strlen(pw));
	MD5Update(&ctx1, (const u_char *)sp, (u_int)sl);
	MD5Update(&ctx1, (const u_char *)pw, strlen(pw));
	MD5Final(final, &ctx1);
	for(pl = (int)strlen(pw); pl > 0; pl -= MD5_SIZE)
		MD5Update(&ctx, (const u_char *)final,
		    (u_int)(pl > MD5_SIZE ? MD5_SIZE : pl));

	/* Don't leave anything around in vm they could use. */
	memset(final, 0, sizeof(final));

	/* Then something really weird... */
	for (i = strlen(pw); i; i >>= 1)
		if(i & 1)
		    MD5Update(&ctx, (const u_char *)final, 1);
		else
		    MD5Update(&ctx, (const u_char *)pw, 1);

	/* Now make the output string */
	strcpy(passwd, magic);
	strncat(passwd, sp, (u_int)sl);
	strcat(passwd, "$");

	MD5Final(final, &ctx);

	/*
	 * and now, just to make sure things don't run too fast
	 * On a 60 Mhz Pentium this takes 34 msec, so you would
	 * need 30 seconds to build a 1000 entry dictionary...
	 */
	for(i = 0; i < 1000; i++) {
		MD5Init(&ctx1);
		if(i & 1)
			MD5Update(&ctx1, (const u_char *)pw, strlen(pw));
		else
			MD5Update(&ctx1, (const u_char *)final, MD5_SIZE);

		if(i % 3)
			MD5Update(&ctx1, (const u_char *)sp, (u_int)sl);

		if(i % 7)
			MD5Update(&ctx1, (const u_char *)pw, strlen(pw));

		if(i & 1)
			MD5Update(&ctx1, (const u_char *)final, MD5_SIZE);
		else
			MD5Update(&ctx1, (const u_char *)pw, strlen(pw));
		MD5Final(final, &ctx1);
	}

	p = passwd + strlen(passwd);

	l = (final[ 0]<<16) | (final[ 6]<<8) | final[12];
	_crypt_to64(p, l, 4); p += 4;
	l = (final[ 1]<<16) | (final[ 7]<<8) | final[13];
	_crypt_to64(p, l, 4); p += 4;
	l = (final[ 2]<<16) | (final[ 8]<<8) | final[14];
	_crypt_to64(p, l, 4); p += 4;
	l = (final[ 3]<<16) | (final[ 9]<<8) | final[15];
	_crypt_to64(p, l, 4); p += 4;
	l = (final[ 4]<<16) | (final[10]<<8) | final[ 5];
	_crypt_to64(p, l, 4); p += 4;
	l = final[11];
	_crypt_to64(p, l, 2); p += 2;
	*p = '\0';

	/* Don't leave anything around in vm they could use. */
	memset(final, 0, sizeof(final));

	return (passwd);
}

/*	$KAME: md5.c,v 1.5 2000/11/08 06:13:08 itojun Exp $	*/
/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define SHIFT(X, s) (((X) << (s)) | ((X) >> (32 - (s))))

#define F(X, Y, Z) (((X) & (Y)) | ((~X) & (Z)))
#define G(X, Y, Z) (((X) & (Z)) | ((Y) & (~Z)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I(X, Y, Z) ((Y) ^ ((X) | (~Z)))

#define ROUND1(a, b, c, d, k, s, i) { \
	(a) = (a) + F((b), (c), (d)) + X[(k)] + T[(i)]; \
	(a) = SHIFT((a), (s)); \
	(a) = (b) + (a); \
}

#define ROUND2(a, b, c, d, k, s, i) { \
	(a) = (a) + G((b), (c), (d)) + X[(k)] + T[(i)]; \
	(a) = SHIFT((a), (s)); \
	(a) = (b) + (a); \
}

#define ROUND3(a, b, c, d, k, s, i) { \
	(a) = (a) + H((b), (c), (d)) + X[(k)] + T[(i)]; \
	(a) = SHIFT((a), (s)); \
	(a) = (b) + (a); \
}

#define ROUND4(a, b, c, d, k, s, i) { \
	(a) = (a) + I((b), (c), (d)) + X[(k)] + T[(i)]; \
	(a) = SHIFT((a), (s)); \
	(a) = (b) + (a); \
}

#define Sa	 7
#define Sb	12
#define Sc	17
#define Sd	22

#define Se	 5
#define Sf	 9
#define Sg	14
#define Sh	20

#define Si	 4
#define Sj	11
#define Sk	16
#define Sl	23

#define Sm	 6
#define Sn	10
#define So	15
#define Sp	21

#define MD5_A0	0x67452301
#define MD5_B0	0xefcdab89
#define MD5_C0	0x98badcfe
#define MD5_D0	0x10325476

/* Integer part of 4294967296 times abs(sin(i)), where i is in radians. */
static const U32 T[65] = {
	0,
	0xd76aa478, 	0xe8c7b756,	0x242070db,	0xc1bdceee,
	0xf57c0faf,	0x4787c62a, 	0xa8304613,	0xfd469501,
	0x698098d8,	0x8b44f7af,	0xffff5bb1,	0x895cd7be,
	0x6b901122, 	0xfd987193, 	0xa679438e,	0x49b40821,

	0xf61e2562,	0xc040b340, 	0x265e5a51, 	0xe9b6c7aa,
	0xd62f105d,	0x2441453,	0xd8a1e681,	0xe7d3fbc8,
	0x21e1cde6,	0xc33707d6, 	0xf4d50d87, 	0x455a14ed,
	0xa9e3e905,	0xfcefa3f8, 	0x676f02d9, 	0x8d2a4c8a,

	0xfffa3942,	0x8771f681, 	0x6d9d6122, 	0xfde5380c,
	0xa4beea44, 	0x4bdecfa9, 	0xf6bb4b60, 	0xbebfbc70,
	0x289b7ec6, 	0xeaa127fa, 	0xd4ef3085,	0x4881d05,
	0xd9d4d039, 	0xe6db99e5, 	0x1fa27cf8, 	0xc4ac5665,

	0xf4292244, 	0x432aff97, 	0xab9423a7, 	0xfc93a039,
	0x655b59c3, 	0x8f0ccc92, 	0xffeff47d, 	0x85845dd1,
	0x6fa87e4f, 	0xfe2ce6e0, 	0xa3014314, 	0x4e0811a1,
	0xf7537e82, 	0xbd3af235, 	0x2ad7d2bb, 	0xeb86d391,
};

static const U8 md5_paddat[MD5_BUFLEN] = {
	0x80,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	
};

static void md5_calc(U8 *, md5_ctxt *);

static void md5_init(md5_ctxt *ctxt)
{
	ctxt->md5_n = 0;
	ctxt->md5_i = 0;
	ctxt->md5_sta = MD5_A0;
	ctxt->md5_stb = MD5_B0;
	ctxt->md5_stc = MD5_C0;
	ctxt->md5_std = MD5_D0;
	bzero(ctxt->md5_buf, sizeof(ctxt->md5_buf));
}

static void md5_loop(md5_ctxt *ctxt, const U8 *input, u_int len)
{
	u_int gap, i;

	ctxt->md5_n += len * 8; /* byte to bit */
	gap = MD5_BUFLEN - ctxt->md5_i;

	if (len >= gap) {
		bcopy((void *)input, (void *)(ctxt->md5_buf + ctxt->md5_i),
			gap);
		md5_calc(ctxt->md5_buf, ctxt);

		for (i = gap; i + MD5_BUFLEN <= len; i += MD5_BUFLEN) {
			md5_calc((U8 *)(input + i), ctxt);
		}
		
		ctxt->md5_i = len - i;
		bcopy((void *)(input + i), (void *)ctxt->md5_buf, ctxt->md5_i);
	} else {
		bcopy((void *)input, (void *)(ctxt->md5_buf + ctxt->md5_i),
			len);
		ctxt->md5_i += len;
	}
}

static void md5_pad(md5_ctxt *ctxt)
{
	u_int gap;

	/* Don't count up padding. Keep md5_n. */	
	gap = MD5_BUFLEN - ctxt->md5_i;
	if (gap > 8) {
		bcopy(md5_paddat,
		      (void *)(ctxt->md5_buf + ctxt->md5_i),
		      gap - sizeof(ctxt->md5_n));
	} else {
		/* including gap == 8 */
		bcopy(md5_paddat, (void *)(ctxt->md5_buf + ctxt->md5_i),
			gap);
		md5_calc(ctxt->md5_buf, ctxt);
		bcopy((md5_paddat + gap),
		      (void *)ctxt->md5_buf,
		      MD5_BUFLEN - sizeof(ctxt->md5_n));
	}

	/* 8 byte word */	
#if BYTE_ORDER == LITTLE_ENDIAN
	bcopy(&ctxt->md5_n8[0], &ctxt->md5_buf[56], 8);
#endif
#if BYTE_ORDER == BIG_ENDIAN
	ctxt->md5_buf[56] = ctxt->md5_n8[7];
	ctxt->md5_buf[57] = ctxt->md5_n8[6];
	ctxt->md5_buf[58] = ctxt->md5_n8[5];
	ctxt->md5_buf[59] = ctxt->md5_n8[4];
	ctxt->md5_buf[60] = ctxt->md5_n8[3];
	ctxt->md5_buf[61] = ctxt->md5_n8[2];
	ctxt->md5_buf[62] = ctxt->md5_n8[1];
	ctxt->md5_buf[63] = ctxt->md5_n8[0];
#endif

	md5_calc(ctxt->md5_buf, ctxt);
}

static void md5_result(U8 *digest, md5_ctxt *ctxt)
{
	/* 4 byte words */
#if BYTE_ORDER == LITTLE_ENDIAN
	bcopy(&ctxt->md5_st8[0], digest, 16);
#endif
#if BYTE_ORDER == BIG_ENDIAN
	digest[ 0] = ctxt->md5_st8[ 3]; digest[ 1] = ctxt->md5_st8[ 2];
	digest[ 2] = ctxt->md5_st8[ 1]; digest[ 3] = ctxt->md5_st8[ 0];
	digest[ 4] = ctxt->md5_st8[ 7]; digest[ 5] = ctxt->md5_st8[ 6];
	digest[ 6] = ctxt->md5_st8[ 5]; digest[ 7] = ctxt->md5_st8[ 4];
	digest[ 8] = ctxt->md5_st8[11]; digest[ 9] = ctxt->md5_st8[10];
	digest[10] = ctxt->md5_st8[ 9]; digest[11] = ctxt->md5_st8[ 8];
	digest[12] = ctxt->md5_st8[15]; digest[13] = ctxt->md5_st8[14];
	digest[14] = ctxt->md5_st8[13]; digest[15] = ctxt->md5_st8[12];
#endif
}

static void md5_calc(U8 *b64, md5_ctxt *ctxt)
{
	U32 A = ctxt->md5_sta;
	U32 B = ctxt->md5_stb;
	U32 C = ctxt->md5_stc;
	U32 D = ctxt->md5_std;
#if BYTE_ORDER == LITTLE_ENDIAN
	U32 *X = (U32 *)b64;
#endif	
#if BYTE_ORDER == BIG_ENDIAN
	U32 X[16];
	/* 4 byte words */
	/* what a brute force but fast! */
	U8 *y = (U8 *)X;
	y[ 0] = b64[ 3]; y[ 1] = b64[ 2]; y[ 2] = b64[ 1]; y[ 3] = b64[ 0];
	y[ 4] = b64[ 7]; y[ 5] = b64[ 6]; y[ 6] = b64[ 5]; y[ 7] = b64[ 4];
	y[ 8] = b64[11]; y[ 9] = b64[10]; y[10] = b64[ 9]; y[11] = b64[ 8];
	y[12] = b64[15]; y[13] = b64[14]; y[14] = b64[13]; y[15] = b64[12];
	y[16] = b64[19]; y[17] = b64[18]; y[18] = b64[17]; y[19] = b64[16];
	y[20] = b64[23]; y[21] = b64[22]; y[22] = b64[21]; y[23] = b64[20];
	y[24] = b64[27]; y[25] = b64[26]; y[26] = b64[25]; y[27] = b64[24];
	y[28] = b64[31]; y[29] = b64[30]; y[30] = b64[29]; y[31] = b64[28];
	y[32] = b64[35]; y[33] = b64[34]; y[34] = b64[33]; y[35] = b64[32];
	y[36] = b64[39]; y[37] = b64[38]; y[38] = b64[37]; y[39] = b64[36];
	y[40] = b64[43]; y[41] = b64[42]; y[42] = b64[41]; y[43] = b64[40];
	y[44] = b64[47]; y[45] = b64[46]; y[46] = b64[45]; y[47] = b64[44];
	y[48] = b64[51]; y[49] = b64[50]; y[50] = b64[49]; y[51] = b64[48];
	y[52] = b64[55]; y[53] = b64[54]; y[54] = b64[53]; y[55] = b64[52];
	y[56] = b64[59]; y[57] = b64[58]; y[58] = b64[57]; y[59] = b64[56];
	y[60] = b64[63]; y[61] = b64[62]; y[62] = b64[61]; y[63] = b64[60];
#endif

	ROUND1(A, B, C, D,  0, Sa,  1); ROUND1(D, A, B, C,  1, Sb,  2);
	ROUND1(C, D, A, B,  2, Sc,  3); ROUND1(B, C, D, A,  3, Sd,  4);
	ROUND1(A, B, C, D,  4, Sa,  5); ROUND1(D, A, B, C,  5, Sb,  6);
	ROUND1(C, D, A, B,  6, Sc,  7); ROUND1(B, C, D, A,  7, Sd,  8);
	ROUND1(A, B, C, D,  8, Sa,  9); ROUND1(D, A, B, C,  9, Sb, 10);
	ROUND1(C, D, A, B, 10, Sc, 11); ROUND1(B, C, D, A, 11, Sd, 12);
	ROUND1(A, B, C, D, 12, Sa, 13); ROUND1(D, A, B, C, 13, Sb, 14);
	ROUND1(C, D, A, B, 14, Sc, 15); ROUND1(B, C, D, A, 15, Sd, 16);
	
	ROUND2(A, B, C, D,  1, Se, 17); ROUND2(D, A, B, C,  6, Sf, 18);
	ROUND2(C, D, A, B, 11, Sg, 19); ROUND2(B, C, D, A,  0, Sh, 20);
	ROUND2(A, B, C, D,  5, Se, 21); ROUND2(D, A, B, C, 10, Sf, 22);
	ROUND2(C, D, A, B, 15, Sg, 23); ROUND2(B, C, D, A,  4, Sh, 24);
	ROUND2(A, B, C, D,  9, Se, 25); ROUND2(D, A, B, C, 14, Sf, 26);
	ROUND2(C, D, A, B,  3, Sg, 27); ROUND2(B, C, D, A,  8, Sh, 28);
	ROUND2(A, B, C, D, 13, Se, 29); ROUND2(D, A, B, C,  2, Sf, 30);
	ROUND2(C, D, A, B,  7, Sg, 31); ROUND2(B, C, D, A, 12, Sh, 32);

	ROUND3(A, B, C, D,  5, Si, 33); ROUND3(D, A, B, C,  8, Sj, 34);
	ROUND3(C, D, A, B, 11, Sk, 35); ROUND3(B, C, D, A, 14, Sl, 36);
	ROUND3(A, B, C, D,  1, Si, 37); ROUND3(D, A, B, C,  4, Sj, 38);
	ROUND3(C, D, A, B,  7, Sk, 39); ROUND3(B, C, D, A, 10, Sl, 40);
	ROUND3(A, B, C, D, 13, Si, 41); ROUND3(D, A, B, C,  0, Sj, 42);
	ROUND3(C, D, A, B,  3, Sk, 43); ROUND3(B, C, D, A,  6, Sl, 44);
	ROUND3(A, B, C, D,  9, Si, 45); ROUND3(D, A, B, C, 12, Sj, 46);
	ROUND3(C, D, A, B, 15, Sk, 47); ROUND3(B, C, D, A,  2, Sl, 48);
	
	ROUND4(A, B, C, D,  0, Sm, 49); ROUND4(D, A, B, C,  7, Sn, 50);	
	ROUND4(C, D, A, B, 14, So, 51); ROUND4(B, C, D, A,  5, Sp, 52);	
	ROUND4(A, B, C, D, 12, Sm, 53); ROUND4(D, A, B, C,  3, Sn, 54);	
	ROUND4(C, D, A, B, 10, So, 55); ROUND4(B, C, D, A,  1, Sp, 56);	
	ROUND4(A, B, C, D,  8, Sm, 57); ROUND4(D, A, B, C, 15, Sn, 58);	
	ROUND4(C, D, A, B,  6, So, 59); ROUND4(B, C, D, A, 13, Sp, 60);	
	ROUND4(A, B, C, D,  4, Sm, 61); ROUND4(D, A, B, C, 11, Sn, 62);	
	ROUND4(C, D, A, B,  2, So, 63); ROUND4(B, C, D, A,  9, Sp, 64);

	ctxt->md5_sta += A;
	ctxt->md5_stb += B;
	ctxt->md5_stc += C;
	ctxt->md5_std += D;
}

