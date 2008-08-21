/*  png.c
   
   Copyright (c) 2003/2004/2005 Matthias Kramm <kramm@quiss.org>

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <zlib.h>

#ifdef EXPORT
#undef EXPORT
#endif

#ifdef PNG_INLINE_EXPORTS
#define EXPORT static
#else
#define EXPORT
#include "png.h"
#endif

typedef unsigned u32;

typedef struct _COL {
    unsigned char a,r,g,b;
} COL;

static int png_read_chunk(char (*head)[4], int*destlen, unsigned char**destdata, FILE*fi)
{
    unsigned int len;
    unsigned char blen[4];
    if(destlen) *destlen=0;
    if(destdata) *destdata=0;
    if(!fread(&blen, 4, 1, fi)) {
	return 0;
    }
    if(!fread(head, 4, 1, fi)) {
	return 0;
    }
    len = blen[0]<<24|blen[1]<<16|blen[2]<<8|blen[3];
    if(destlen) *destlen = len;
    if(destdata) {
	if(!len) {
	    *destdata = 0;
	} else {
	    *destdata = (unsigned char*)malloc(len);
	    if(!fread(*destdata, len, 1, fi)) {
		*destdata = 0;
		if(destlen) *destlen=0;
		return 0;
	    }
	}
	fseek(fi, 4, SEEK_CUR);

    } else {
	fseek(fi, len+4, SEEK_CUR);
    }
    return 1;
}

static unsigned int png_get_dword(FILE*fi)
{
    unsigned int a;
    unsigned char b[4];
    fread(&b,4,1,fi);
    return b[0]<<24|b[1]<<16|b[2]<<8|b[3];
}

struct png_header
{
    int width;
    int height;
    int bpp;
    int mode;
};

static int png_read_header(FILE*fi, struct png_header*header)
{
    char id[4];
    int len;
    int ok=0;
    unsigned char head[8] = {137,80,78,71,13,10,26,10};
    unsigned char head2[8];
    unsigned char*data;
    fread(head2,8,1,fi);
    if(strncmp((const char*)head,(const char*)head2,4))
	return 0; // not a png file
    
    while(png_read_chunk(&id, &len, &data, fi))
    {
	//printf("Chunk: %c%c%c%c (len:%d)\n", id[0],id[1],id[2],id[3], len);
	if(!strncmp(id, "IHDR", 4)) {
	    char a,b,c,f,i;
	    if(len < 8) exit(1);
	    header->width = data[0]<<24|data[1]<<16|data[2]<<8|data[3];
	    header->height = data[4]<<24|data[5]<<16|data[6]<<8|data[7];
	    a = data[8];      // should be 8
	    b = data[9];      // should be 3(indexed) or 2(rgb)

	    c = data[10];     // compression mode (0)
	    f = data[11];     // filter mode (0)
	    i = data[12];     // interlace mode (0)

	    if(b!=0 && b!=4 && b!=2 && b!=3 && b!=6) {
		fprintf(stderr, "Image mode %d not supported!\n", b);
		return 0;
	    }
	    if(a!=8 && (b==2 || b==6)) {
		printf("Bpp %d in mode %d not supported!\n", a);
		return 0;
	    }
	    if(c!=0) {
		printf("Compression mode %d not supported!\n", c);
		return 0;
	    }
	    if(f!=0) {
		printf("Filter mode %d not supported!\n", f);
		return 0;
	    }
	    if(i!=0) {
		printf("Interlace mode %d not supported!\n", i);
		return 0;
	    }
	    //printf("%dx%d bpp:%d mode:%d comp:%d filter:%d interlace:%d\n",header->width, header->height, a,b,c,f,i);
	    header->bpp = a;
	    header->mode = b;
	    ok = 1;
	} 
	
	free(data);
    }
    return ok;
}

typedef unsigned char byte;
#define ABS(a) ((a)>0?(a):(-(a)))
static inline byte PaethPredictor (byte a,byte b,byte c)
{
        // a = left, b = above, c = upper left
        int p = a + b - c;        // initial estimate
        int pa = ABS(p - a);      // distances to a, b, c
        int pb = ABS(p - b);
        int pc = ABS(p - c);
        // return nearest of a,b,c,
        // breaking ties in order a,b,c.
        if (pa <= pb && pa <= pc) 
		return a;
        else if (pb <= pc) 
		return b;
        else return c;
}

static void applyfilter1(int mode, unsigned char*src, unsigned char*old, unsigned char*dest, int width)
{
    int x;
    unsigned char last=0;
    unsigned char upperlast=0;

    if(mode==0) {
	for(x=0;x<width;x++) {
	    *dest = *src;
	    dest++;
	    src++;
	}
    }
    else if(mode==1) {
	for(x=0;x<width;x++) {
	    *dest = *src+last;
	    last = *dest;
	    dest++;
	    src++;
	}
    }
    else if(mode==2) {
	for(x=0;x<width;x++) {
	    *dest = *src+*old;
	    dest++;
	    old++;
	    src++;
	}
    }
    else if(mode==3) {
	for(x=0;x<width;x++) {
	    *dest = *src+(*old+last)/2;
	    last = *dest;
	    dest++;
	    old++;
	    src++;
	}
    }
    else if(mode==4) {
	for(x=0;x<width;x++) {
	    *dest = *src+PaethPredictor(last,*old,upperlast);
	    last = *dest;
	    upperlast = *old;
	    dest++;
	    old++;
	    src++;
	}
    }    

}

static void applyfilter2(int mode, unsigned char*src, unsigned char*old, unsigned char*dest, int width)
{
    int x;
    unsigned char lasta=0;
    unsigned char lastb=0;
    unsigned char upperlasta=0;
    unsigned char upperlastb=0;

    if(mode==0) {
	for(x=0;x<width;x++) {
	    dest[0] = src[0];
	    dest[1] = src[1];
	    dest+=2;
	    src+=2;
	}
    }
    else if(mode==1) {
	for(x=0;x<width;x++) {
	    dest[0] = src[0]+lasta;
	    dest[1] = src[1]+lastb;
	    lasta = dest[0];
	    lastb = dest[1];
	    dest+=2;
	    src+=2;
	}
    }
    else if(mode==2) {
	for(x=0;x<width;x++) {
	    dest[0] = src[0]+old[0];
	    dest[1] = src[1]+old[1];
	    dest+=2;
	    old+=2;
	    src+=2;
	}
    }
    else if(mode==3) {
	for(x=0;x<width;x++) {
	    dest[0] = src[0]+(old[0]+lasta)/2;
	    dest[1] = src[1]+(old[1]+lastb)/2;
	    lasta = dest[0];
	    lastb = dest[1];
	    dest+=2;
	    old+=2;
	    src+=2;
	}
    }
    else if(mode==4) {
	for(x=0;x<width;x++) {
	    dest[0] = src[0]+PaethPredictor(lasta,old[0],upperlasta);
	    dest[1] = src[1]+PaethPredictor(lastb,old[1],upperlastb);
	    lasta = dest[0];
	    lastb = dest[1];
	    upperlasta = old[0];
	    upperlastb = old[1];
	    dest+=2;
	    old+=2;
	    src+=2;
	}
    }    
}


/* also performs 24 bit conversion! */
static void applyfilter3(int mode, unsigned char*src, unsigned char*old, unsigned char*dest, int width)
{
    int x;
    unsigned char lastr=0;
    unsigned char lastg=0;
    unsigned char lastb=0;
    unsigned char upperlastr=0;
    unsigned char upperlastg=0;
    unsigned char upperlastb=0;

    if(mode==0) {
	for(x=0;x<width;x++) {
	    dest[0] = 255;
	    dest[1] = src[0];
	    dest[2] = src[1];
	    dest[3] = src[2];
	    dest+=4;
	    src+=3;
	}
    }
    else if(mode==1) {
	for(x=0;x<width;x++) {
	    dest[0] = 255;
	    dest[1] = src[0]+lastr;
	    dest[2] = src[1]+lastg;
	    dest[3] = src[2]+lastb;
	    lastr = dest[1];
	    lastg = dest[2];
	    lastb = dest[3];
	    dest+=4;
	    src+=3;
	}
    }
    else if(mode==2) {
	for(x=0;x<width;x++) {
	    dest[0] = 255;
	    dest[1] = src[0]+old[1];
	    dest[2] = src[1]+old[2];
	    dest[3] = src[2]+old[3];
	    dest+=4;
	    old+=4;
	    src+=3;
	}
    }
    else if(mode==3) {
	for(x=0;x<width;x++) {
	    dest[0] = 255;
	    dest[1] = src[0]+(old[1]+lastr)/2;
	    dest[2] = src[1]+(old[2]+lastg)/2;
	    dest[3] = src[2]+(old[3]+lastb)/2;
	    lastr = dest[1];
	    lastg = dest[2];
	    lastb = dest[3];
	    dest+=4;
	    old+=4;
	    src+=3;
	}
    }
    else if(mode==4) {
	for(x=0;x<width;x++) {
	    dest[0] = 255;
	    dest[1] = src[0]+PaethPredictor(lastr,old[1],upperlastr);
	    dest[2] = src[1]+PaethPredictor(lastg,old[2],upperlastg);
	    dest[3] = src[2]+PaethPredictor(lastb,old[3],upperlastb);
	    lastr = dest[1];
	    lastg = dest[2];
	    lastb = dest[3];
	    upperlastr = old[1];
	    upperlastg = old[2];
	    upperlastb = old[3];
	    dest+=4;
	    old+=4;
	    src+=3;
	}
    }    
}

static void inline applyfilter4(int mode, unsigned char*src, unsigned char*old, unsigned char*dest, int width)
{
    int x;
    unsigned char lastr=0;
    unsigned char lastg=0;
    unsigned char lastb=0;
    unsigned char lasta=0;
    unsigned char upperlastr=0;
    unsigned char upperlastg=0;
    unsigned char upperlastb=0;
    unsigned char upperlasta=0;

    if(mode==0) {
	for(x=0;x<width;x++) {
	    dest[0] = src[3];
	    dest[1] = src[0];
	    dest[2] = src[1];
	    dest[3] = src[2];
	    dest+=4;
	    src+=4;
	}
    }
    else if(mode==1) {
	for(x=0;x<width;x++) {
	    dest[0] = src[3]+lasta;
	    dest[1] = src[0]+lastr;
	    dest[2] = src[1]+lastg;
	    dest[3] = src[2]+lastb;
	    lasta = dest[0];
	    lastr = dest[1];
	    lastg = dest[2];
	    lastb = dest[3];
	    dest+=4;
	    src+=4;
	}
    }
    else if(mode==2) {
	for(x=0;x<width;x++) {
	    dest[0] = src[3]+old[0];
	    dest[1] = src[0]+old[1];
	    dest[2] = src[1]+old[2];
	    dest[3] = src[2]+old[3];
	    dest+=4;
	    old+=4;
	    src+=4;
	}
    }
    else if(mode==3) {
	for(x=0;x<width;x++) {
	    dest[0] = src[3]+(old[0]+lasta)/2;
	    dest[1] = src[0]+(old[1]+lastr)/2;
	    dest[2] = src[1]+(old[2]+lastg)/2;
	    dest[3] = src[2]+(old[3]+lastb)/2;
	    lasta = dest[0];
	    lastr = dest[1];
	    lastg = dest[2];
	    lastb = dest[3];
	    dest+=4;
	    old+=4;
	    src+=4;
	}
    }
    else if(mode==4) {
	for(x=0;x<width;x++) {
	    dest[0] = src[3]+PaethPredictor(lasta,old[0],upperlasta);
	    dest[1] = src[0]+PaethPredictor(lastr,old[1],upperlastr);
	    dest[2] = src[1]+PaethPredictor(lastg,old[2],upperlastg);
	    dest[3] = src[2]+PaethPredictor(lastb,old[3],upperlastb);
	    lasta = dest[0];
	    lastr = dest[1];
	    lastg = dest[2];
	    lastb = dest[3];
	    upperlasta = old[0];
	    upperlastr = old[1];
	    upperlastg = old[2];
	    upperlastb = old[3];
	    dest+=4;
	    old+=4;
	    src+=4;
	}
    }    
}


EXPORT int getPNGdimensions(const char*sname, int*destwidth, int*destheight)
{
    FILE*fi;
    struct png_header header;
    if ((fi = fopen(sname, "rb")) == NULL) {
	fprintf(stderr, "Couldn't open %s\n", sname);
	return 0;
    }
    if(!png_read_header(fi, &header)) {
	return 0;
    }

    *destwidth = header.width;
    *destheight = header.height;
    return 1;
}

EXPORT int getPNG(const char*sname, int*destwidth, int*destheight, unsigned char**destdata)
{
    char tagid[4];
    int len;
    unsigned char*data;
    unsigned char*imagedata;
    unsigned char*zimagedata=0;
    unsigned long int imagedatalen;
    unsigned long int zimagedatalen=0;
    unsigned char*palette = 0;
    int palettelen = 0;
    unsigned char*alphapalette = 0;
    int alphapalettelen = 0;
    struct png_header header;
    int bypp;
    unsigned char*data2 = 0;
    unsigned char alphacolor[3];
    int hasalphacolor=0;

    FILE *fi;
    unsigned char *scanline;

    if ((fi = fopen(sname, "rb")) == NULL) {
	printf("Couldn't open %s\n", sname);
	return 0;
    }

    if(!png_read_header(fi, &header)) {
	return 0;
    }

    if(header.mode == 3 || header.mode == 0) bypp = 1;
    else if(header.mode == 4) bypp = 2;
    else if(header.mode == 2) bypp = 3;
    else if(header.mode == 6) bypp = 4;
    else {
	printf("ERROR: mode:%d\n", header.mode);
	return 0;
    }

    imagedatalen = bypp * header.width * header.height + 65536;
    imagedata = (unsigned char*)malloc(imagedatalen);

    fseek(fi,8,SEEK_SET);
    while(!feof(fi))
    {
	if(!png_read_chunk(&tagid, &len, &data, fi))
	    break;
	if(!strncmp(tagid, "IEND", 4)) {
	    break;
	}
	if(!strncmp(tagid, "PLTE", 4)) {
	    palette = data;
	    palettelen = len/3;
	    data = 0; //don't free data
	    //printf("%d colors in palette\n", palettelen);
	}
	if(!strncmp(tagid, "tRNS", 4)) {
	    if(header.mode == 3) {
		alphapalette = data;
		alphapalettelen = len;
		data = 0; //don't free data
		//printf("found %d alpha colors\n", alphapalettelen);
	    } else if(header.mode == 0 || header.mode == 2) {
		int t;
		if(header.mode == 2) {
		    alphacolor[0] = data[1];
		    alphacolor[1] = data[3];
		    alphacolor[2] = data[5];
		} else {
		    alphacolor[0] = alphacolor[1] = alphacolor[2] = data[1];
		}
		hasalphacolor = 1;
	    }
	}
	if(!strncmp(tagid, "IDAT", 4)) {
	    if(!zimagedata) {
		zimagedatalen = len;
		zimagedata = (unsigned char*)malloc(len);
		memcpy(zimagedata,data,len);
	    } else {
		zimagedata = (unsigned char*)realloc(zimagedata, zimagedatalen+len);
		memcpy(&zimagedata[zimagedatalen], data, len);
		zimagedatalen += len;
	    }
	}
	if(!strncmp(tagid, "tEXt", 4)) {
	    /*int t;
	    printf("Image Text: ");
	    for(t=0;t<len;t++) {
		if(data[t]>=32 && data[t]<128)
		    printf("%c", data[t]);
		else
		    printf("?");
	    }
	    printf("\n");*/
	}
	if(data) {
	    free(data); data=0;
        }
    }
    
    if(!zimagedata || uncompress(imagedata, &imagedatalen, zimagedata, zimagedatalen) != Z_OK) {
	printf("Couldn't uncompress %s!\n", sname);
	if(zimagedata)
	    free(zimagedata);
	return 0;
    }
    free(zimagedata);
    fclose(fi);

    *destwidth = header.width;
    *destheight = header.height;
	
    data2 = (unsigned char*)malloc(header.width*header.height*4);

    if(header.mode == 4)
    {
	int i,s=0;
	int x,y;
	int pos=0;
	unsigned char* old= (unsigned char*)malloc(header.width*2);
	memset(old, 0, header.width*2);
	*destdata = data2;
	for(y=0;y<header.height;y++) {
	    int mode = imagedata[pos++]; //filter mode
	    unsigned char*src;
	    unsigned char*dest;
	    int x;
	    dest = &data2[(y*header.width)*4];

	    if(header.bpp == 8) {
		/* one byte per pixel */
		src = &imagedata[pos];
		pos+=header.width*2;
	    } else {
		/* not implemented yet */
		fprintf(stderr, "ERROR: mode=4 bpp:%d\n", header.bpp);
		free(data2);
		return 0;
	    }

	    applyfilter2(mode, src, old, dest, header.width);
	    memcpy(old, dest, header.width*2);

	    for(x=header.width-1;x>=0;x--) {
		unsigned char gray = dest[x*2+0];
		unsigned char alpha = dest[x*2+1];
		dest[x*4+0] = alpha;
		dest[x*4+1] = gray;
		dest[x*4+2] = gray;
		dest[x*4+3] = gray;
	    }
	}
	free(old);
        free(imagedata);
    } else if(header.mode == 6 || header.mode == 2) {
	int i,s=0;
	int x,y;
	int pos=0;
	*destdata = data2;
	
	unsigned char* firstline = malloc(header.width*4);
	memset(firstline,0,header.width*4);
	for(y=0;y<header.height;y++) {
	    int mode = imagedata[pos++]; //filter mode
	    unsigned char*src;
	    unsigned char*dest;
	    unsigned char*old;
	    dest = &data2[(y*header.width)*4];

	    if(header.bpp == 8)
	    {
		/* one byte per pixel */
		src = &imagedata[pos];
		pos+=header.width*(header.mode==6?4:3);
	    } else {
		/* not implemented yet */
		fprintf(stderr, "ERROR: bpp:%d\n", header.bpp);
		free(data2);
		return 0;
	    }

	    if(!y) {
		old = firstline;
	    } else {
		old = &data2[(y-1)*header.width*4];
	    }
	    if(header.mode == 6) { 
		applyfilter4(mode, src, old, dest, header.width);
	    } else { // header.mode = 2
		applyfilter3(mode, src, old, dest, header.width);
		/* replace alpha color */
		if(hasalphacolor) {
		    int x;
		    for(x=0;x<header.width;x++) {
			if(dest[x*4+1] == alphacolor[0] &&
			   dest[x*4+2] == alphacolor[1] &&
			   dest[x*4+3] == alphacolor[2]) {
			    *(u32*)&dest[x*4] = 0;
			}
		    }
		}
	    }
	}
	free(firstline);
        free(imagedata);
    } else if(header.mode == 0 || header.mode == 3) {
	COL*rgba = 0;
	unsigned char*tmpline = (unsigned char*)malloc(header.width+1);
	unsigned char*destline = (unsigned char*)malloc(header.width+1);
	int i,x,y;
	int pos=0;
	
	*destdata = data2;
	
	if(header.mode == 0) { // grayscale palette
            int mult = (0x1ff>>header.bpp);
	    palettelen = 1<<header.bpp;
	    rgba = (COL*)malloc(palettelen*sizeof(COL));
	    for(i=0;i<palettelen;i++) {
		rgba[i].a = 255;
		rgba[i].r = i*mult;
		rgba[i].g = i*mult;
		rgba[i].b = i*mult;
		if(hasalphacolor) {
		    if(rgba[i].r == alphacolor[0])
			rgba[i].a = 0;
		}
	    }
	} else {
	    if(!palette) {
		fprintf(stderr, "Error: No palette found!\n");
		exit(1);
	    }
	    rgba = (COL*)malloc(palettelen*4);
	    /* 24->32 bit conversion */
	    for(i=0;i<palettelen;i++) {
		rgba[i].r = palette[i*3+0];
		rgba[i].g = palette[i*3+1];
		rgba[i].b = palette[i*3+2];
		if(alphapalette && i<alphapalettelen) {
		    rgba[i].a = alphapalette[i];
		    /*rgba[i].r = ((int)rgba[i].r*rgba[i].a)/255;
		    rgba[i].g = ((int)rgba[i].g*rgba[i].a)/255;
		    rgba[i].b = ((int)rgba[i].b*rgba[i].a)/255;*/
		} else {
		    rgba[i].a = 255;
		}
		if(hasalphacolor) {
		    if(rgba[i].r == alphacolor[0] &&
		       rgba[i].g == alphacolor[1] &&
		       rgba[i].b == alphacolor[2])
			rgba[i].a = 0;
		}
	    }
	}

	for(y=0;y<header.height;y++) {
	    int mode = imagedata[pos++]; //filter mode
	    int x;
	    unsigned char*old;
	    unsigned char*src;
	    src = &imagedata[pos];
	    if(header.bpp == 8) {
		pos+=header.width;
	    } else {
		int x,s=0;
		int bitpos = 0;
		u32 v = (1<<header.bpp)-1;
		for(x=0;x<header.width;x++) {
		    u32 r = src[s/8]<<8 | 
			    src[s/8+1];
		    int t;
		    tmpline[x] = (r>>(16-header.bpp-(s&7)))&v;
		    s+=header.bpp;
		}
		src = tmpline;
		pos+=(header.width*header.bpp+7)/8;
	    }

	    if(!y) {
		memset(destline,0,header.width);
		old = &destline[y*header.width];
	    } else {
		old = tmpline;
	    }
	    applyfilter1(mode, src, old, destline, header.width);
	    memcpy(tmpline,destline,header.width);
	    for(x=0;x<header.width;x++) {
		*(COL*)&data2[y*header.width*4+x*4+0] = rgba[destline[x]];
	    }
	}
	free(tmpline);
	free(destline);
	free(rgba);
        free(imagedata);
    } else {
	printf("expected PNG mode to be 2, 3 or 6 (is:%d)\n", header.mode);
	return 0;
    }

    return 1;
}

static u32 mycrc32;

static u32*crc32_table = 0;
static void make_crc32_table(void)
{
  int t;
  if(crc32_table) 
      return;
  crc32_table = (u32*)malloc(1024);

  for (t = 0; t < 256; t++) {
    u32 c = t;
    int s;
    for (s = 0; s < 8; s++) {
      c = (0xedb88320L*(c&1)) ^ (c >> 1);
    }
    crc32_table[t] = c;
  }
}
static inline void png_write_byte(FILE*fi, unsigned char byte)
{
    fwrite(&byte,1,1,fi);
    mycrc32 = crc32_table[(mycrc32 ^ byte) & 0xff] ^ (mycrc32 >> 8);
}
static long png_start_chunk(FILE*fi, char*type, int len)
{
    unsigned char mytype[4]={0,0,0,0};
    unsigned char mylen[4];
    long filepos;
    mylen[0] = len>>24;
    mylen[1] = len>>16;
    mylen[2] = len>>8;
    mylen[3] = len;
    memcpy(mytype,type,strlen(type));
    filepos = ftell(fi);
    fwrite(&mylen, 4, 1, fi);
    mycrc32=0xffffffff;
    png_write_byte(fi,mytype[0]);
    png_write_byte(fi,mytype[1]);
    png_write_byte(fi,mytype[2]);
    png_write_byte(fi,mytype[3]);
    return filepos;
}
static void png_patch_len(FILE*fi, int pos, int len)
{
    unsigned char mylen[4];
    long filepos;
    mylen[0] = len>>24;
    mylen[1] = len>>16;
    mylen[2] = len>>8;
    mylen[3] = len;
    fseek(fi, pos, SEEK_SET);
    fwrite(&mylen, 4, 1, fi);
    fseek(fi, 0, SEEK_END);
}
static void png_write_bytes(FILE*fi, unsigned char*bytes, int len)
{
    int t;
    for(t=0;t<len;t++)
	png_write_byte(fi,bytes[t]);
}
static void png_write_dword(FILE*fi, u32 dword)
{
    png_write_byte(fi,dword>>24);
    png_write_byte(fi,dword>>16);
    png_write_byte(fi,dword>>8);
    png_write_byte(fi,dword);
}
static void png_end_chunk(FILE*fi)
{
    u32 tmp = mycrc32^0xffffffff;
    unsigned char tmp2[4];
    tmp2[0] = tmp>>24;
    tmp2[1] = tmp>>16;
    tmp2[2] = tmp>>8;
    tmp2[3] = tmp;
    fwrite(&tmp2,4,1,fi);
}

#define ZLIB_BUFFER_SIZE 16384

static long compress_line(z_stream*zs, Bytef*line, int len, FILE*fi)
{
    long size = 0;
    zs->next_in = line;
    zs->avail_in = len;

    while(1) {
	int ret = deflate(zs, Z_NO_FLUSH);
	if (ret != Z_OK) {
	    fprintf(stderr, "error in deflate(): %s", zs->msg?zs->msg:"unknown");
	    return 0;
	}
	if(zs->avail_out != ZLIB_BUFFER_SIZE) {
	    int consumed = ZLIB_BUFFER_SIZE - zs->avail_out;
	    size += consumed;
	    png_write_bytes(fi, zs->next_out - consumed , consumed);
	    zs->next_out = zs->next_out - consumed;
	    zs->avail_out = ZLIB_BUFFER_SIZE;
	}
	if(!zs->avail_in) {
	    break;
	}
    }
    return size;
}

static int test_line(z_stream*zs_orig, Bytef*line, int linelen)
{
    z_stream zs;
    int ret = deflateCopy(&zs, zs_orig);
    if(ret != Z_OK) {
	fprintf(stderr, "Couldn't copy stream\n");
	return 0;
    }

    zs.next_in = line;
    zs.avail_in = linelen;

    long size = 0;

    int mode = Z_SYNC_FLUSH;
    while(1) {
	int ret = deflate(&zs, mode);
	if (ret != Z_OK && ret != Z_STREAM_END) {
	    fprintf(stderr, "error in deflate(): %s (mode %s, %d bytes remaining)\n", zs.msg?zs.msg:"unknown", 
		    mode==Z_SYNC_FLUSH?"Z_SYNC_FLUSH":"Z_FINISH", zs.avail_in);
	    return 0;
	}
	if(zs.avail_out != ZLIB_BUFFER_SIZE) {
	    int consumed = ZLIB_BUFFER_SIZE - zs.avail_out;
	    size += consumed;
	    zs.next_out = zs.next_out - consumed;
	    zs.avail_out = ZLIB_BUFFER_SIZE;
	}
        if (ret == Z_STREAM_END) {
            break;
        }
	if(!zs.avail_in) {
	    mode = Z_FINISH;
	}
    }
    ret = deflateEnd(&zs);
    if (ret != Z_OK) {
	fprintf(stderr, "error in deflateEnd(): %s\n", zs.msg?zs.msg:"unknown");
	return 0;
    }
    return size;
}

static int finishzlib(z_stream*zs, FILE*fi)
{
    int size = 0;
    int ret;
    while(1) {
        ret = deflate(zs, Z_FINISH);
        if (ret != Z_OK &&
            ret != Z_STREAM_END) {
	    fprintf(stderr, "error in deflate(finish): %s\n", zs->msg?zs->msg:"unknown");
	    return 0;
	}

	if(zs->avail_out != ZLIB_BUFFER_SIZE) {
	    int consumed = ZLIB_BUFFER_SIZE - zs->avail_out;
	    size += consumed;
	    png_write_bytes(fi, zs->next_out - consumed , consumed);
	    zs->next_out = zs->next_out - consumed;
	    zs->avail_out = ZLIB_BUFFER_SIZE;
	}
        if (ret == Z_STREAM_END) {
            break;
        }
    }
    ret = deflateEnd(zs);
    if (ret != Z_OK) {
	fprintf(stderr, "error in deflateEnd(): %s\n", zs->msg?zs->msg:"unknown");
	return 0;
    }
    return size;
}

static void filter_line(int filtermode, unsigned char*dest, unsigned char*src, int width)
{
    int pos2 = 0;
    int pos = 0;
    int srcwidth = width*4;
    int x;
    if(filtermode == 0) {
	for(x=0;x<width;x++) {
	    dest[pos2++]=src[pos+1];
	    dest[pos2++]=src[pos+2];
	    dest[pos2++]=src[pos+3];
	    dest[pos2++]=src[pos+0]; //alpha
	    pos+=4;
	}
    } else if(filtermode == 1) {
	/* x difference filter */
	dest[pos2++]=src[pos+1];
	dest[pos2++]=src[pos+2];
	dest[pos2++]=src[pos+3];
	dest[pos2++]=src[pos+0];
	pos+=4;
	for(x=1;x<width;x++) {
	    dest[pos2++]=src[pos+1] - src[pos-4+1];
	    dest[pos2++]=src[pos+2] - src[pos-4+2];
	    dest[pos2++]=src[pos+3] - src[pos-4+3];
	    dest[pos2++]=src[pos+0] - src[pos-4+0]; //alpha
	    pos+=4;
	}
    } else if(filtermode == 2) {
	/* y difference filter */
	for(x=0;x<width;x++) {
	    dest[pos2++]=src[pos+1] - src[pos-srcwidth+1];
	    dest[pos2++]=src[pos+2] - src[pos-srcwidth+2];
	    dest[pos2++]=src[pos+3] - src[pos-srcwidth+3];
	    dest[pos2++]=src[pos+0] - src[pos-srcwidth+0]; //alpha
	    pos+=4;
	}
    } else if(filtermode == 3) {
	dest[pos2++]=src[pos+1] - src[pos-srcwidth+1]/2;
	dest[pos2++]=src[pos+2] - src[pos-srcwidth+2]/2;
	dest[pos2++]=src[pos+3] - src[pos-srcwidth+3]/2;
	dest[pos2++]=src[pos+0] - src[pos-srcwidth+0]/2;
	pos+=4;
	/* x+y difference filter */
	for(x=1;x<width;x++) {
	    dest[pos2++]=src[pos+1] - (src[pos-4+1] + src[pos-srcwidth+1])/2;
	    dest[pos2++]=src[pos+2] - (src[pos-4+2] + src[pos-srcwidth+2])/2;
	    dest[pos2++]=src[pos+3] - (src[pos-4+3] + src[pos-srcwidth+3])/2;
	    dest[pos2++]=src[pos+0] - (src[pos-4+0] + src[pos-srcwidth+0])/2; //alpha
	    pos+=4;
	}
    } else if(filtermode == 4) {
	dest[pos2++]=src[pos+1] - PaethPredictor(0, src[pos-srcwidth+1], 0);
	dest[pos2++]=src[pos+2] - PaethPredictor(0, src[pos-srcwidth+2], 0);
	dest[pos2++]=src[pos+3] - PaethPredictor(0, src[pos-srcwidth+3], 0);
	dest[pos2++]=src[pos+0] - PaethPredictor(0, src[pos-srcwidth+0], 0);
	pos+=4;
	/* paeth difference filter */
	for(x=1;x<width;x++) {
	    dest[pos2++]=src[pos+1] - PaethPredictor(src[pos-4+1], src[pos-srcwidth+1], src[pos-4-srcwidth+1]);
	    dest[pos2++]=src[pos+2] - PaethPredictor(src[pos-4+2], src[pos-srcwidth+2], src[pos-4-srcwidth+2]);
	    dest[pos2++]=src[pos+3] - PaethPredictor(src[pos-4+3], src[pos-srcwidth+3], src[pos-4-srcwidth+3]);
	    dest[pos2++]=src[pos+0] - PaethPredictor(src[pos-4+0], src[pos-srcwidth+0], src[pos-4-srcwidth+0]);
	    pos+=4;
	}
    }
}

EXPORT void writePNG(const char*filename, unsigned char*data, int width, int height)
{
    FILE*fi;
    int crc;
    int t;
    unsigned char format;
    unsigned char tmp;
    unsigned char* data2=0;
    u32 datalen;
    u32 datalen2;
    unsigned char head[] = {137,80,78,71,13,10,26,10}; // PNG header
    int cols;
    char alpha = 1;
    int pos = 0;
    int error;
    u32 tmp32;
    int bpp;
    int ret;
    z_stream zs;

    make_crc32_table();

    bpp = 32;
    cols = 0;
    format = 5;

    datalen = (width*height*bpp/8+cols*8);
    
    fi = fopen(filename, "wb");
    if(!fi) {
	perror("open");
	return;
    }
    fwrite(head,sizeof(head),1,fi);     

    png_start_chunk(fi, "IHDR", 13);
     png_write_dword(fi,width);
     png_write_dword(fi,height);
     png_write_byte(fi,8);
     if(format == 3)
     png_write_byte(fi,3); //indexed
     else if(format == 5 && alpha==0)
     png_write_byte(fi,2); //rgb
     else if(format == 5 && alpha==1)
     png_write_byte(fi,6); //rgba
     else return;

     png_write_byte(fi,0); //compression mode
     png_write_byte(fi,0); //filter mode
     png_write_byte(fi,0); //interlace mode
    png_end_chunk(fi);

/*    if(format == 3) {
	png_start_chunk(fi, "PLTE", 768);
	 
	 for(t=0;t<256;t++) {
	     png_write_byte(fi,palette[t].r);
	     png_write_byte(fi,palette[t].g);
	     png_write_byte(fi,palette[t].b);
	 }
	png_end_chunk(fi);
    }*/
    long idatpos = png_start_chunk(fi, "IDAT", 0);
    
    memset(&zs,0,sizeof(z_stream));
    Bytef*writebuf = (Bytef*)malloc(ZLIB_BUFFER_SIZE);
    zs.zalloc = Z_NULL;
    zs.zfree  = Z_NULL;
    zs.opaque = Z_NULL;
    zs.next_out = writebuf;
    zs.avail_out = ZLIB_BUFFER_SIZE;
    ret = deflateInit(&zs, 1);
    if (ret != Z_OK) {
	fprintf(stderr, "error in deflateInit(): %s", zs.msg?zs.msg:"unknown");
	return;
    }

    long idatsize = 0;
    {
	int x,y;
	int srcwidth = width * (bpp/8);
	int linelen = 1 + ((srcwidth+3)&~3);
	unsigned char* line = (unsigned char*)malloc(linelen);
	unsigned char* bestline = (unsigned char*)malloc(linelen);
	memset(line, 0, linelen);
	for(y=0;y<height;y++)
	{
	    int filtermode;
	    int bestsize = 0x7fffffff;
	    for(filtermode=0;filtermode<5;filtermode++) {

		if(!y && filtermode>=2)
		    continue; // don't do y direction filters in the first row
		
		line[0]=filtermode; //filter type
		filter_line(filtermode, line+1, &data[y*srcwidth], width);

		int size = test_line(&zs, line, linelen);
		if(size < bestsize) {
		    memcpy(bestline, line, linelen);
		    bestsize = size;
		}
	    }
	    idatsize += compress_line(&zs, bestline, linelen, fi);
	}
	free(line);free(bestline);
    }
    idatsize += finishzlib(&zs, fi);
    png_patch_len(fi, idatpos, idatsize);
    png_end_chunk(fi);

    png_start_chunk(fi, "IEND", 0);
    png_end_chunk(fi);

    free(writebuf);
    free(data2);
    fclose(fi);
}

