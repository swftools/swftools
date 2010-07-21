#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "utf8.h"

static char utf8buf[16];

int writeUTF8(unsigned int charnum, char*dest)
{
    dest[0] = 0;
    if(charnum < 0x80) {
	dest[0] = charnum;
	dest[1] = 0;
	return 1;
    } else if(charnum <0x800) {
	/* 0000 0080-0000 07FF   110xxxxx 10xxxxxx */
	dest[0] = 0xc0 | (charnum >> 6);
	dest[1] = 0x80 | (charnum & 0x3f);
	dest[2] = 0;
	return 2;
    } else if(charnum < 0x10000) {
	/* 0000 0800-0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx */
	dest[0] = 0xe0 | (charnum >> 12);
	dest[1] = 0x80 |((charnum >> 6)&0x3f);
	dest[2] = 0x80 |((charnum     )&0x3f);
	dest[3] = 0;
	return 3;
    } else if(charnum < 0x200000) {
	/* 0001 0000-001F FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
	dest[0] = 0xf0 | (charnum >> 18);
	dest[1] = 0x80 |((charnum >> 12)&0x3f);
	dest[2] = 0x80 |((charnum >> 6 )&0x3f);
	dest[3] = 0x80 |((charnum      )&0x3f);
	dest[4] = 0;
	return 4;
    } else if(charnum < 0x4000000) {
	/* 0020 0000-03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
	dest[0] = 0xf8 | (charnum >> 24);
	dest[1] = 0x80 |((charnum >> 18)&0x3f);
	dest[2] = 0x80 |((charnum >> 12)&0x3f);
	dest[3] = 0x80 |((charnum >> 6 )&0x3f);
	dest[4] = 0x80 |((charnum      )&0x3f);
	dest[5] = 0;
	return 5;
    } else if(charnum < 0x80000000) {
	/* 0400 0000-7FFF FFFF   1111110x 10xxxxxx ... 10xxxxxx */
	dest[0] = 0xfc | (charnum >> 30);
	dest[1] = 0x80 |((charnum >> 24)&0x3f);
	dest[2] = 0x80 |((charnum >> 18)&0x3f);
	dest[3] = 0x80 |((charnum >> 12)&0x3f);
	dest[4] = 0x80 |((charnum >> 6 )&0x3f);
	dest[5] = 0x80 |((charnum      )&0x3f);
	dest[6] = 0;
	return 6;
    } else {
	fprintf(stderr, "Illegal character: 0x%08x\n", charnum);
	dest[0] = 0;
	return 0;
    }
}

char* getUTF8(unsigned int charnum)
{
    memset(utf8buf, 0, sizeof(utf8buf));
    writeUTF8(charnum, utf8buf);
    return utf8buf;
}

