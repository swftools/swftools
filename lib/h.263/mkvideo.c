/* mkvideo.c
   Create a video file.

   Part of the swftools package.
   
   Copyright (c) 2003 Matthias Kramm <kramm@quiss.org> */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../lib/rfxswf.h"
#include "png.h"
#include "h263tables.c"


void swf_SetVideoStreamDefine(TAG*tag, U16 frames, U16 width, U16 height)
{
    width=width&~15; height=height&~15;
    swf_SetU16(tag, frames);
    swf_SetU16(tag, width);
    swf_SetU16(tag, height);
    swf_SetU8(tag, 1); /* smoothing on */
    swf_SetU8(tag, 2); /* codec = h.263 sorenson spark */
}

struct block_t
{
    int y1[64];
    int y2[64];
    int y3[64];
    int y4[64];
    int u[64];
    int v[64];
};

struct fblock_t
{
    double y1[64];
    double y2[64];
    double y3[64];
    double y4[64];
    double u[64];
    double v[64];
};

void zigzag(int*src) 
{
    int table[64] = {
	0, 1, 5, 6, 14, 15, 27, 28, 
	2, 4, 7, 13, 16, 26, 29, 42, 
	3, 8, 12, 17, 25, 30, 41, 43, 
	9, 11, 18, 24, 31, 40, 44, 53, 
	10, 19, 23, 32, 39, 45, 52, 54, 
	20, 22, 33, 38, 46, 51, 55, 60, 
	21, 34, 37, 47, 50, 56, 59, 61, 
	35, 36, 48, 49, 57, 58, 62, 63};
    int tmp[64];
    int t;
    for(t=0;t<64;t++) {
	tmp[table[t]] = src[t];
    }
    memcpy(src, tmp, sizeof(int)*64);
}

#define PI 3.14159265358979
#define SQRT2 1.414214
#define RSQRT2 (1.0/1.414214)

void dct(double*src)
{
    double tmp[64];
    int x,y,u,v;
    for(y=0;y<8;y++)
    for(x=0;x<8;x++)
    {
	double c = 0;
	for(v=0;v<8;v++)
	for(u=0;u<8;u++)
	{
	    double f = 0.25*cos(PI*(2.0*x+1.0)*u/16.0)*cos(PI*(2.0*y+1.0)*v/16.0);
	    if(!u) f *= RSQRT2;
	    if(!v) f *= RSQRT2;
	    c+=f*src[v*8+u];
	}
	tmp[y*8+x] = c;
    }
    memcpy(src, tmp, sizeof(double)*64);
}

void idct(double*src)
{
    double tmp[64];
    int x,y,u,v;
    for(v=0;v<8;v++)
    for(u=0;u<8;u++)
    {
	double c = 0;
	for(y=0;y<8;y++)
	for(x=0;x<8;x++)
	{
	    double f = 0.25*cos(PI*(2.0*x+1.0)*u/16.0)*cos(PI*(2.0*y+1.0)*v/16.0);
	    if(!u) f *= RSQRT2;
	    if(!v) f *= RSQRT2;
	    c+=f*src[y*8+x];
	}
	tmp[v*8+u] = c;
    }
    memcpy(src, tmp, sizeof(double)*64);
}

void getregion(struct fblock_t* bb, RGBA*pic, int bx, int by, int width, int height)
{
    RGBA*p1 = &pic[by*width*16+bx*16];
    RGBA*p2 = p1;
    int linex = width;
    int y1=0, y2=0, y3=0, y4=0;
    int u=0,v=0;
    int x,y;
    for(y=0;y<8;y++) {
	for(x=0;x<8;x++) {
	    double r,g,b;
	    r = (p2[x*2].r + p2[x*2+1].r + p2[linex+x*2].r + p2[linex+x*2+1].r)/4.0;
	    g = (p2[x*2].g + p2[x*2+1].g + p2[linex+x*2].g + p2[linex+x*2+1].g)/4.0;
	    b = (p2[x*2].b + p2[x*2+1].b + p2[linex+x*2].b + p2[linex+x*2+1].b)/4.0;
	    bb->u[u++] = (r*-0.169 + g*-0.332 + b*0.500 + 128.0);
	    bb->v[v++] = (r*0.500 + g*-0.419 + b*-0.0813 + 128.0);

	    r = p1[x].r; g = p1[x].g; b = p1[x].b;
	    bb->y1[y1++] = (r*0.299 + g*0.587 + b*0.114);
	    r = p1[x+8].r; g = p1[x+8].g; b = p1[x+8].b;
	    bb->y2[y2++] = (r*0.299 + g*0.587 + b*0.114);
	    r = p1[linex*8+x].r; g = p1[linex*8+x].g; b = p1[linex*8+x].b;
	    bb->y3[y3++] = (r*0.299 + g*0.587 + b*0.114);
	    r = p1[linex*8+x+8].r; g = p1[linex*8+x+8].g; b = p1[linex*8+x+8].b;
	    bb->y4[y4++] = (r*0.299 + g*0.587 + b*0.114);
	}
	p1+=linex;
	p2+=linex*2;
    }
}

int valtodc(int val)
{
    assert(val>=0);

    /* table 12/h.263 */
    val/=8;
    /* TODO: what to do for zero values? skip the block? */
    if(val==0)
	return 1;
    if(val==128)
	return 255;
    if(val>254)
	return 254;
    return val;
}

void codehuffman(TAG*tag, struct huffcode*table, int index)
{
    /* TODO: !optimize! */
    int i=0;
    while(table[index].code[i]) {
	if(table[index].code[i]=='0')
	    swf_SetBits(tag, 0, 1);
	else
	    swf_SetBits(tag, 1, 1);
	i++;
    }
}

int see=0;

void quantize(double*src, int*dest, int quant, int has_dc)
{
    int t,pos=0;
    if(has_dc) {
	dest[0] = (int)src[0]; /*DC*/
	dest[0] = valtodc(dest[0]);
	pos++;
    }
    for(t=pos;t<64;t++)
    {
	dest[t] = (int)src[t];
	//val = (quant*(2*level+1)-1)+quant&1
	if(quant&1) {
	    dest[t] = (dest[t]/quant - 1)/2;
	} else {
	    dest[t] = ((dest[t]+1)/quant - 1)/2;
	}
    }
}

int hascoef(int*b, int has_dc)
{
    int t;
    int pos=0;
    int range=2;
    if(has_dc)
	pos++;
    for(t=pos;t<64;t++) {
	if(b[t]<=-range || b[t]>=range)
	    return 1;
    }
    return 0;
}

void encode8x8(TAG*tag, int*bb, int has_dc, int has_tcoef)
{
    int t;
    int pos=0;

    if(has_dc) {
	swf_SetBits(tag, bb[0], 8);
	pos++;
    }

    if(has_tcoef) {
	int last;
	/* determine last non-null coefficient */
	for(last=63;last>=pos;last--) {
	    if(bb[last])
		break;
	}
	assert(bb[last]);
	/* blocks without coefficients should not be included
	   in the cbpy/cbpc patterns */
	while(1) {
	    int run=0;
	    int level=0;
	    int islast=0;
	    int sign=0;
	    int t;
	    while(!bb[pos] && pos<last) {
		pos++;
		run++;
	    }
	    if(pos==last)
		islast=1;
	    level=bb[pos];
	    if(level<0) {
		level = -level;
		sign = 1;
	    }
	    for(t=0;t<RLE_ESCAPE;t++) {
		if(rle_params[t].run == run &&
		   rle_params[t].level == level &&
		   rle_params[t].last == islast) {
		    codehuffman(tag, rle, t);
		    swf_SetBits(tag, sign, 1);
		    break;
		}
	    }
	    if(t==RLE_ESCAPE) {
		codehuffman(tag, rle, RLE_ESCAPE);
		/* table 14/h.263 */
		assert(level);
		if(level<-127) level = -127;
		if(level>127) level = 127;

		swf_SetBits(tag, islast, 1);
		swf_SetBits(tag, run, 6);
		swf_SetBits(tag, level, 8); //fixme
	    }

	    if(islast)
		break;
	    pos++;
	}

	//codehuffman(tag, rle, 58);
	//swf_SetBits(tag, 1, 1); //sign
    }
    see++;
}

void encode_blockI(TAG*tag, RGBA*pic, int bx, int by, int width, int height, int*quant)
{
    struct fblock_t b;
    int dquant=0; /* TODO: should we take advantage of the dquant feature?*/
    //int cbpcbits = 3;
    //int cbpybits = 15;
    int cbpcbits = 0;
    int cbpybits = 0;
    int y1[64],y2[64],y3[64],y4[64],u[64],v[64];

    getregion(&b, pic, bx, by, width, height);
    dct(b.y1); quantize(b.y1,y1,1,*quant); zigzag(y1); cbpybits|=hascoef(y1, 1)*8;
    dct(b.y2); quantize(b.y2,y2,1,*quant); zigzag(y2); cbpybits|=hascoef(y2, 1)*4;
    dct(b.y3); quantize(b.y3,y3,1,*quant); zigzag(y3); cbpybits|=hascoef(y3, 1)*2;
    dct(b.y4); quantize(b.y4,y4,1,*quant); zigzag(y4); cbpybits|=hascoef(y4, 1)*1;
    dct(b.u); quantize(b.u,u,1,*quant); zigzag(u); cbpcbits|=hascoef(u, 1)*2;
    dct(b.v); quantize(b.v,v,1,*quant); zigzag(v); cbpcbits|=hascoef(v, 1)*1;

    if(dquant) {
	codehuffman(tag, mcbpc_intra, 4+cbpcbits);
    } else {
	codehuffman(tag, mcbpc_intra, 0+cbpcbits);
    }
    //swf_SetBits(tag, 1, 1); /*cbpc-00 mb_type=3 (no bquant), cbc=0*/

    /* if this was an intra frame, we'd need to code (cbpybits^15) */
    codehuffman(tag, cbpy, cbpybits);

    if(dquant) {
	/* 00 01 10 11
	   -1 -2 +1 +2
	*/
	swf_SetBits(tag, 0x3, 2);
    }

    /* luminance */
    encode8x8(tag, y1, 1, cbpybits&8);
    encode8x8(tag, y2, 1, cbpybits&4);
    encode8x8(tag, y3, 1, cbpybits&2);
    encode8x8(tag, y4, 1, cbpybits&1);
    /*swf_SetBits(tag, 0x1, 8);
    swf_SetBits(tag, 0xfe, 8);
    swf_SetBits(tag, 0x1, 8);
    swf_SetBits(tag, 0x1, 8);*/

    /* chrominance */
    encode8x8(tag, u, 1, cbpcbits&2);
    encode8x8(tag, v, 1, cbpcbits&1);
    /*swf_SetBits(tag, 0xfe, 8);
    swf_SetBits(tag, 0xfe, 8);*/
}

void swf_SetVideoStreamIFrame(TAG*tag, RGBA*pic, U16 width, U16 height, int frame)
{
    U32 i32;
    int bx, by, bbx, bby;
    int quant = 9;

    width=width&~15; height=height&~15;

    swf_SetU16(tag, frame);
    swf_SetBits(tag, 1, 17); /* picture start code*/
    swf_SetBits(tag, 0, 5); /* version=0, version 1 would optimize rle behaviour*/
    swf_SetBits(tag, frame, 8); /* time reference */

    /* write dimensions, taking advantage of some predefined sizes
       if the opportunity presents itself */
    i32 = width<<16|height;
    switch(i32)
    {
	case 352<<16|288: swf_SetBits(tag, 2, 3);break;
	case 176<<16|144: swf_SetBits(tag, 3, 3);break;
	case 128<<16|96: swf_SetBits(tag, 4, 3);break;
	case 320<<16|240: swf_SetBits(tag, 5, 3);break;
	case 160<<16|120: swf_SetBits(tag, 6, 3);break;
	default:
	    if(width>255 || height>255) {
		swf_SetBits(tag, 1, 3);
		swf_SetBits(tag, width, 16);
		swf_SetBits(tag, height, 16);
	    } else {
		swf_SetBits(tag, 0, 3);
		swf_SetBits(tag, width, 8);
		swf_SetBits(tag, height, 8);
	    }
    }

    swf_SetBits(tag, 0, 2); /* I-Frame */
    swf_SetBits(tag, 0, 1); /* No deblock filter */
    swf_SetBits(tag, quant, 5); /* quantizer (1-31), may be updated later on*/
    swf_SetBits(tag, 0, 1); /* No extra info */

    bbx = (width+15)/16;
    bby = (height+15)/16;

    for(by=0;by<bby;by++)
    {
	for(bx=0;bx<bbx;bx++)
	{
	    encode_blockI(tag, pic, bx, by, width, height, &quant);
	}
    }
}

int main()
{
    int fi;
    int t;
    SWF swf;
    TAG * tag;
    RGBA* pic, rgb;
    SWFPLACEOBJECT obj;
    int width = 0;
    int height = 0;
    int frames = 1;
    unsigned char*data;
    char* fname = "/home/kramm/pics/lena.png";

    getimage(fname, &width, &height, &data);
    pic = (RGBA*)malloc(width*height*sizeof(RGBA));
    memcpy(pic, data, width*height*sizeof(RGBA));
    free(data);
    printf("Compressing %s, size %dx%d\n", fname, width, height);

    memset(&swf,0,sizeof(SWF));
    memset(&obj,0,sizeof(obj));

    swf.fileVersion    = 6;
    swf.frameRate      = 29*256;
    swf.movieSize.xmax = 20*width;
    swf.movieSize.ymax = 20*height;

    swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    tag = swf.firstTag;
    rgb.r = 0x00;rgb.g = 0x00;rgb.b = 0x00;
    swf_SetRGB(tag,&rgb);

    tag = swf_InsertTag(tag, ST_DEFINEVIDEOSTREAM);
    swf_SetU16(tag, 33);
    swf_SetVideoStreamDefine(tag, frames, width, height);
    
    for(t=0;t<frames;t++)
    {
	tag = swf_InsertTag(tag, ST_VIDEOFRAME);
	swf_SetU16(tag, 33);
	swf_SetVideoStreamIFrame(tag, pic, width, height, t);

	tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
	swf_GetPlaceObject(0, &obj);
	if(t==0) {
	    obj.depth = 1;
	    obj.id = 33;
	} else {
	    obj.move = 1;
	    obj.depth = 1;
	    obj.ratio = t;
	}
	swf_SetPlaceObject(tag,&obj);

	tag = swf_InsertTag(tag, ST_SHOWFRAME);
    }
   
    tag = swf_InsertTag(tag, ST_END);

    fi = open("video3.swf", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if(swf_WriteSWF(fi,&swf)<0) {
	fprintf(stderr,"WriteSWF() failed.\n");
    }
    close(fi);
    swf_FreeTags(&swf);
}
