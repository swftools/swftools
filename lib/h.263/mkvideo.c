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

typedef struct _VIDEOSTREAM
{
    int width;
    int height;
    RGBA*oldpic;
} VIDEOSTREAM;

void swf_SetVideoStreamDefine(TAG*tag, VIDEOSTREAM*stream, U16 frames, U16 width, U16 height)
{
    width=width&~15; height=height&~15;
    swf_SetU16(tag, frames);
    swf_SetU16(tag, width);
    swf_SetU16(tag, height);
    swf_SetU8(tag, 1); /* smoothing on */
    swf_SetU8(tag, 2); /* codec = h.263 sorenson spark */

    stream->width = width;
    stream->height = height;
    stream->oldpic = 0;
}

typedef struct _block_t
{
    int y1[64];
    int y2[64];
    int y3[64];
    int y4[64];
    int u[64];
    int v[64];
} block_t;

typedef struct _fblock_t
{
    double y1[64];
    double y2[64];
    double y3[64];
    double y4[64];
    double u[64];
    double v[64];
} fblock_t;

void fzigzag(double*src) 
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
    double tmp[64];
    int t;
    for(t=0;t<64;t++) {
	((int*)&tmp[table[t]])[0] = ((int*)&src[t])[0];
	((int*)&tmp[table[t]])[1] = ((int*)&src[t])[1];
    }
    memcpy(src, tmp, sizeof(double)*64);
}

#define PI 3.14159265358979
#define SQRT2 1.414214
#define RSQRT2 (1.0/1.414214)

double table[8][8] =
{
{0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548},
{0.980785280403230,0.831469612302545,0.555570233019602,0.195090322016128,-0.195090322016128,-0.555570233019602,-0.831469612302545,-0.980785280403230},
{0.923879532511287,0.382683432365090,-0.382683432365090,-0.923879532511287,-0.923879532511287,-0.382683432365090,0.382683432365090,0.923879532511287},
{0.831469612302545,-0.195090322016128,-0.980785280403230,-0.555570233019602,0.555570233019602,0.980785280403230,0.195090322016129,-0.831469612302545},
{0.707106781186548,-0.707106781186547,-0.707106781186548,0.707106781186547,0.707106781186548,-0.707106781186547,-0.707106781186547,0.707106781186547},
{0.555570233019602,-0.980785280403230,0.195090322016128,0.831469612302545,-0.831469612302545,-0.195090322016128,0.980785280403231,-0.555570233019602},
{0.382683432365090,-0.923879532511287,0.923879532511287,-0.382683432365090,-0.382683432365091,0.923879532511287,-0.923879532511286,0.382683432365090},
{0.195090322016128,-0.555570233019602,0.831469612302545,-0.980785280403231,0.980785280403230,-0.831469612302545,0.555570233019602,-0.195090322016129}
};

void dct(double*src)
{
    double tmp[64];
    int x,y,u,v,t;

    for(v=0;v<8;v++)
    for(u=0;u<8;u++)
    {
	double c = 0;
	for(x=0;x<8;x++)
	{
	    c+=table[u][x]*src[v*8+x];
	}
	tmp[v*8+u] = c;
    }
    for(u=0;u<8;u++)
    for(v=0;v<8;v++)
    {
	double c = 0;
	for(y=0;y<8;y++)
	{
	    c+=table[v][y]*tmp[y*8+u];
	}
	src[v*8+u] = c*0.25;
    }
}

void idct(double*src)
{
    double tmp[64];
    int x,y,u,v;
    for(y=0;y<8;y++)
    for(x=0;x<8;x++)
    {
	double c = 0;
	for(u=0;u<8;u++)
	{
	    c+=table[u][x]*src[y*8+u];
	}
	tmp[y*8+x] = c;
    }
    for(y=0;y<8;y++)
    for(x=0;x<8;x++)
    {
	double c = 0;
	for(v=0;v<8;v++)
	{
	    c+=table[v][y]*tmp[v*8+x];
	}
	src[y*8+x] = c*0.25;
    }
}

void getregion(fblock_t* bb, RGBA*pic, int bx, int by, int width, int height)
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

    val+=4; //round
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

void quantize8x8(double*src, int*dest, int has_dc, int quant)
{
    int t,pos=0;
    if(has_dc) {
	dest[0] = valtodc((int)src[0]); /*DC*/
	pos++;
    }
    for(t=pos;t<64;t++)
    {
	dest[t] = (int)src[t];
	//val = (quant*(2*level+1)-1)+quant&1
/*	if(quant&1) {
	    dest[t] = (dest[t]/quant - 1)/2;
	} else {
	    dest[t] = ((dest[t]+1)/quant - 1)/2;
	}*/
	//dest[t] = (dest[t]/quant-1)/2;
	dest[t] = dest[t]/(quant*2);
    }
}

int hascoef(int*b, int has_dc)
{
    int t;
    int pos=0;
    int range=2; /*TODO: should be a global parameter */
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
	    /* TODO: we could leave out small coefficients
	             after a certain point (32?) */
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
	    assert(level);
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
		level=bb[pos];
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
}

void dodct(fblock_t*fb)
{
    dct(fb->y1); dct(fb->y2); dct(fb->y3); dct(fb->y4); 
    dct(fb->u);  dct(fb->v);  
    fzigzag(fb->y1);
    fzigzag(fb->y2);
    fzigzag(fb->y3);
    fzigzag(fb->y4);
    fzigzag(fb->u);
    fzigzag(fb->v); 
}

void quantize(fblock_t*fb, block_t*b, int has_dc, int quant)
{
    quantize8x8(fb->y1,b->y1,has_dc,quant); 
    quantize8x8(fb->y2,b->y2,has_dc,quant); 
    quantize8x8(fb->y3,b->y3,has_dc,quant); 
    quantize8x8(fb->y4,b->y4,has_dc,quant); 
    quantize8x8(fb->u,b->u,has_dc,quant);   
    quantize8x8(fb->v,b->v,has_dc,quant);   
}

void getblockpatterns(block_t*b, int*cbpybits,int*cbpcbits, int has_dc)
{
    *cbpybits = 0;
    *cbpcbits = 0;

    *cbpybits|=hascoef(b->y1, has_dc)*8;
    *cbpybits|=hascoef(b->y2, has_dc)*4;
    *cbpybits|=hascoef(b->y3, has_dc)*2;
    *cbpybits|=hascoef(b->y4, has_dc)*1;

    *cbpcbits|=hascoef(b->u, has_dc)*2;
    *cbpcbits|=hascoef(b->v, has_dc)*1;
}

void setQuant(TAG*tag, int dquant)
{
    int code = 0;
    /* 00 01 10 11
       -1 -2 +1 +2
    */
    if(dquant == -1) {
	swf_SetBits(tag, 0x0, 2);
    } else if(dquant == -2) {
	swf_SetBits(tag, 0x1, 2);
    } else if(dquant == +1) {
	swf_SetBits(tag, 0x2, 2);
    } else if(dquant == +2) {
	swf_SetBits(tag, 0x3, 2);
    } else {
	assert(0*strlen("invalid dquant"));
    }
}

void change_quant(int quant, int*dquant)
{
    /* TODO */
    *dquant = 0;
}

void encode_blockI(TAG*tag, RGBA*pic, int bx, int by, int width, int height, int*quant)
{
    fblock_t fb;
    block_t b;
    int dquant=0;
    int cbpcbits = 0, cbpybits=0;

    getregion(&fb, pic, bx, by, width, height);
    dodct(&fb);
    
    change_quant(*quant, &dquant);
    *quant+=dquant;

    quantize(&fb, &b, 1, *quant);
    getblockpatterns(&b, &cbpybits, &cbpcbits, 1);

    if(dquant) {
	codehuffman(tag, mcbpc_intra, 4+cbpcbits);
    } else {
	codehuffman(tag, mcbpc_intra, 0+cbpcbits);
    }

    codehuffman(tag, cbpy, cbpybits);

    if(dquant) {
	setQuant(tag, dquant);
    }

    /* luminance */
    encode8x8(tag, b.y1, 1, cbpybits&8);
    encode8x8(tag, b.y2, 1, cbpybits&4);
    encode8x8(tag, b.y3, 1, cbpybits&2);
    encode8x8(tag, b.y4, 1, cbpybits&1);

    /* chrominance */
    encode8x8(tag, b.u, 1, cbpcbits&2);
    encode8x8(tag, b.v, 1, cbpcbits&1);
}

void encode_blockP(TAG*tag, RGBA*pic, int bx, int by, int width, int height, int*quant)
{
    fblock_t fb;
    block_t b;
    int dquant=0;
    int has_mvd=0;
    int has_mvd24=0;
    int has_dc=1;
    int mode = 0;
    int cbpcbits = 0, cbpybits=0;

    getregion(&fb, pic, bx, by, width, height);
    dodct(&fb);

    change_quant(*quant, &dquant);
    *quant += dquant;

    quantize(&fb, &b, has_dc, *quant);

    getblockpatterns(&b, &cbpybits, &cbpcbits, has_dc);

    if(!dquant && has_mvd && !has_mvd24 && !has_dc) mode = 0;
    else if(dquant && has_mvd && !has_mvd24 && !has_dc) mode = 1;
    else if(!dquant && has_mvd && has_mvd24 && !has_dc) mode = 2;
    else if(!dquant && !has_mvd && !has_mvd24 && has_dc) mode = 3;
    else if(dquant && !has_mvd && !has_mvd24 && has_dc) mode = 4;
    else exit(1);

    swf_SetBits(tag,0,1); /* cod - 1 if we're not going to code this block*/
	
    codehuffman(tag, mcbpc_inter, mode*4+cbpcbits);
    codehuffman(tag, cbpy, (mode==3 || mode==4)?cbpybits:cbpybits^15);

    if(!bx&&!by) {
	printf("cbpcbits: %d\n", cbpcbits);
	printf("cbpybits: %d\n", cbpybits);
    }

    if(dquant) {
	setQuant(tag, dquant);
    }

    if(has_mvd) {
    }
    if(has_mvd24) {
    }

    /* luminance */
    encode8x8(tag, b.y1, has_dc, cbpybits&8);
    encode8x8(tag, b.y2, has_dc, cbpybits&4);
    encode8x8(tag, b.y3, has_dc, cbpybits&2);
    encode8x8(tag, b.y4, has_dc, cbpybits&1);

    /* chrominance */
    encode8x8(tag, b.u, has_dc, cbpcbits&2);
    encode8x8(tag, b.v, has_dc, cbpcbits&1);
}

#define TYPE_IFRAME 0
#define TYPE_PFRAME 1

static void writeHeader(TAG*tag, int width, int height, int frame, int quant, int type)
{
    U32 i32;
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

    swf_SetBits(tag, type, 2); /* I-Frame or P-Frame */
    swf_SetBits(tag, 0, 1); /* No deblock filter */
    swf_SetBits(tag, quant, 5); /* quantizer (1-31), may be updated later on*/
    swf_SetBits(tag, 0, 1); /* No extra info */
}

void swf_SetVideoStreamIFrame(TAG*tag, VIDEOSTREAM*s, RGBA*pic, U16 width, U16 height, int frame)
{
    int bx, by, bbx, bby;
    int quant = 7;

    /* TODO: width not divisible by 16 will get us in trouble */
    width=width&~15; height=height&~15;

    writeHeader(tag, width, height, frame, quant, TYPE_IFRAME);

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

void swf_SetVideoStreamPFrame(TAG*tag, VIDEOSTREAM*s, RGBA*pic, U16 width, U16 height, int frame)
{
    int bx, by, bbx, bby;
    int quant = 7;

    /* TODO: width not divisible by 16 will get us in trouble */
    width=width&~15; height=height&~15;

    writeHeader(tag, width, height, frame, quant, TYPE_PFRAME);

    bbx = (width+15)/16;
    bby = (height+15)/16;

    for(by=0;by<bby;by++)
    {
	for(bx=0;bx<bbx;bx++)
	{
	    encode_blockP(tag, pic, bx, by, width, height, &quant);
	}
    }
}

int main(int argn, char*argv[])
{
    int fi;
    int t;
    SWF swf;
    TAG * tag;
    RGBA* pic, rgb;
    SWFPLACEOBJECT obj;
    int width = 0;
    int height = 0;
    int frames = 2;
    unsigned char*data;
    char* fname = "/home/kramm/pics/peppers.png";
    VIDEOSTREAM stream;

    memset(&stream, 0, sizeof(stream));

    getPNG(fname, &width, &height, &data);
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
    swf_SetVideoStreamDefine(tag, &stream, frames, width, height);
    
    for(t=0;t<frames;t++)
    {
	tag = swf_InsertTag(tag, ST_VIDEOFRAME);
	swf_SetU16(tag, 33);
	if(t==0)
	    swf_SetVideoStreamIFrame(tag, &stream, pic, width, height, t);
	else
	    swf_SetVideoStreamPFrame(tag, &stream, pic, width, height, t);

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
