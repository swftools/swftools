/* swfvideo.c
   Routines for handling h.263 video tags

   Part of the swftools package.

   Copyright (c) 2003 Matthias Kramm <kramm@quiss.org>

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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../rfxswf.h"
#include "h263tables.h"
#include "dct.h"

/* TODO:
   - use prepare* / write* in encode_IFrame_block
   - check whether mvd steps of 2 lead to (much) smaller results
*/ 

#ifdef MAIN
U16 totalframes = 0;
#endif
void swf_SetVideoStreamDefine(TAG*tag, VIDEOSTREAM*stream, U16 frames, U16 width, U16 height)
{
#ifdef MAIN
    totalframes = frames;
#endif
    memset(stream, 0, sizeof(VIDEOSTREAM));
    stream->olinex = width;
    stream->owidth = width;
    stream->oheight = height;
    width+=15;width&=~15;
    height+=15;height&=~15;
    stream->linex = width;
    stream->width = width;
    stream->height = height;
    stream->bbx = width/16;
    stream->bby = height/16;
    stream->current = (YUV*)rfx_calloc(width*height*sizeof(YUV));
    stream->oldpic = (YUV*)rfx_calloc(width*height*sizeof(YUV));
    stream->mvdx = (int*)rfx_alloc(stream->bbx*stream->bby*sizeof(int));
    stream->mvdy = (int*)rfx_alloc(stream->bbx*stream->bby*sizeof(int));
    stream->do_motion = 0;

    assert((stream->width&15) == 0);
    assert((stream->height&15) == 0);
    assert((stream->bbx*16) == stream->width);
    assert((stream->bby*16) == stream->height);
    
    swf_SetU16(tag, frames);
    swf_SetU16(tag, width);
    swf_SetU16(tag, height);
    //swf_SetU8(tag, 1); /* smoothing on */
    swf_SetU8(tag, 0); /* smoothing off */
    swf_SetU8(tag, 2); /* codec = h.263 sorenson spark */

}
void swf_VideoStreamClear(VIDEOSTREAM*stream)
{
    rfx_free(stream->oldpic);stream->oldpic = 0;
    rfx_free(stream->current);stream->current = 0;
    rfx_free(stream->mvdx);stream->mvdx=0;
    rfx_free(stream->mvdy);stream->mvdy=0;
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

static inline int truncate256(int a)
{
    if(a>255) return 255;
    if(a<0) return 0;
    return a;
}

static void getregion(block_t* bb, YUV*pic, int posx, int posy, int linex)
{
    YUV*p1;
    YUV*p2;
    int i=0;
    int x,y;
    posx*=16;
    posy*=16;
    p1 = &pic[posy*linex+posx];
    p2 = p1;
    for(y=0;y<8;y++) {
	for(x=0;x<8;x++) {
	    bb->u[i] = (p2[x*2].u + p2[x*2+1].u + p2[linex+x*2].u + p2[linex+x*2+1].u)/4;
	    bb->v[i] = (p2[x*2].v + p2[x*2+1].v + p2[linex+x*2].v + p2[linex+x*2+1].v)/4;
	    bb->y1[i] = p1[x].y;
	    bb->y2[i] = p1[x+8].y;
	    bb->y3[i] = p1[linex*8+x].y;
	    bb->y4[i] = p1[linex*8+x+8].y;
	    i++;
	}
	p1+=linex;
	p2+=linex*2;
    }
}

/* This function is pretty complex. Let's hope it works correctly */
static void getmvdregion(block_t* bb, YUV*pic, int posx, int posy, int mvdx, int mvdy, int linex)
{
    YUV*p1;
    YUV*p2;
    int yy=0,uv=0;
    int x,y;
    int yhp = 0, uvhp=0;
    int uvposx, uvposy;
    posx = posx*16 + ((mvdx&~1)/2); //works also for negative mvdx (unlike mvdx/2)
    posy = posy*16 + ((mvdy&~1)/2);
    p1 = &pic[posy*linex+posx];
    p2 = &pic[(posy&~1)*linex+(posx&~1)];
    uvhp = ((mvdx&1)|((mvdx>>1)&1))|((mvdy&2)|((mvdy&1)<<1));
    yhp = ((mvdy&1)<<1|(mvdx&1));

    /* y */
    if(yhp==0 || yhp==2) {
	for(y=0;y<8;y++) {
	    for(x=0;x<8;x++) {
		bb->y1[yy] = p1[x].y;
		bb->y2[yy] = p1[x+8].y;
		bb->y3[yy] = p1[linex*8+x].y;
		bb->y4[yy] = p1[linex*8+x+8].y;
		yy++;
	    }
	    p1+=linex;

	    if(yhp==2) {
		yy-=8;
		for(x=0;x<8;x++) {
		    bb->y1[yy] += p1[x].y; bb->y1[yy] /= 2;
		    bb->y2[yy] += p1[x+8].y; bb->y2[yy] /= 2;
		    bb->y3[yy] += p1[linex*8+x].y; bb->y3[yy] /= 2;
		    bb->y4[yy] += p1[linex*8+x+8].y; bb->y4[yy] /= 2;
		    yy++;
		}
	    }
	}
    } else if(yhp==1 || yhp==3) {
	for(y=0;y<8;y++) {
	    for(x=0;x<8;x++) {
		bb->y1[yy] = (p1[x].y + p1[x+1].y);
		bb->y2[yy] = (p1[x+8].y + p1[x+8+1].y);
		bb->y3[yy] = (p1[linex*8+x].y + p1[linex*8+x+1].y);
		bb->y4[yy] = (p1[linex*8+x+8].y + p1[linex*8+x+8+1].y);
		yy++;
	    }
	    yy-=8;
	    p1+=linex;
	    if(yhp==3) {
		for(x=0;x<8;x++) {
		    bb->y1[yy] += (p1[x].y + p1[x+1].y); bb->y1[yy]/=4;
		    bb->y2[yy] += (p1[x+8].y + p1[x+8+1].y); bb->y2[yy]/=4;
		    bb->y3[yy] += (p1[linex*8+x].y + p1[linex*8+x+1].y); bb->y3[yy]/=4;
		    bb->y4[yy] += (p1[linex*8+x+8].y + p1[linex*8+x+8+1].y); bb->y4[yy]/=4;
		    yy++;
		}
	    } else {
		for(x=0;x<8;x++) {
		    bb->y1[yy]/=2; bb->y2[yy]/=2; bb->y3[yy]/=2; bb->y4[yy]/=2;
		    yy++;
		}
	    }
	}
    }

    /* u,v */
    if(uvhp==0 || uvhp==2) {
	for(y=0;y<8;y++) {
	    for(x=0;x<8;x++) {
		bb->u[uv] = (p2[x*2].u + p2[x*2+1].u + p2[linex+x*2].u + p2[linex+x*2+1].u)/4;
		bb->v[uv] = (p2[x*2].v + p2[x*2+1].v + p2[linex+x*2].v + p2[linex+x*2+1].v)/4;
		uv++;
	    }
	    p2+=linex*2;
	    if(uvhp==2) {
		uv-=8;
		for(x=0;x<8;x++) {
		    bb->u[uv] += (p2[x*2].u + p2[x*2+1].u + p2[linex+x*2].u + p2[linex+x*2+1].u)/4;
		    bb->v[uv] += (p2[x*2].v + p2[x*2+1].v + p2[linex+x*2].v + p2[linex+x*2+1].v)/4;
		    bb->u[uv] /= 2;
		    bb->v[uv] /= 2;
		    uv++;
		}
	    }
	}
    } else /* uvhp==1 || uvhp==3 */ {
	for(y=0;y<8;y++) {
	    for(x=0;x<8;x++) {
		bb->u[uv] = ((p2[x*2].u + p2[x*2+1].u + p2[linex+x*2].u + p2[linex+x*2+1].u)/4+
		             (p2[x*2+2].u + p2[x*2+1+2].u + p2[linex+x*2+2].u + p2[linex+x*2+1+2].u)/4);
		bb->v[uv] = ((p2[x*2].v + p2[x*2+1].v + p2[linex+x*2].v + p2[linex+x*2+1].v)/4+
			     (p2[x*2+2].v + p2[x*2+1+2].v + p2[linex+x*2+2].v + p2[linex+x*2+1+2].v)/4);
		uv++;
	    }
	    uv-=8;
	    p2+=linex*2;
	    if(uvhp==3) {
		for(x=0;x<8;x++) {
		    bb->u[uv] += ((p2[x*2].u + p2[x*2+1].u + p2[linex+x*2].u + p2[linex+x*2+1].u)/4+
			 	  (p2[x*2+2].u + p2[x*2+1+2].u + p2[linex+x*2+2].u + p2[linex+x*2+1+2].u)/4);
		    bb->v[uv] += ((p2[x*2].v + p2[x*2+1].v + p2[linex+x*2].v + p2[linex+x*2+1].v)/4+
				  (p2[x*2+2].v + p2[x*2+1+2].v + p2[linex+x*2+2].v + p2[linex+x*2+1+2].v)/4);
		    bb->u[uv] /= 4;
		    bb->v[uv] /= 4;
		    uv++;
		}
	    } else {
		for(x=0;x<8;x++) {
		    bb->u[uv] /= 2;
		    bb->v[uv] /= 2;
		    uv++;
		}
	    }
	}
    }
}

static void rgb2yuv(YUV*dest, RGBA*src, int dlinex, int slinex, int width, int height)
{
    int x,y;
    for(y=0;y<height;y++) {
	for(x=0;x<width;x++) {
	    int r,g,b;
	    r = src[y*slinex+x].r;
	    g = src[y*slinex+x].g;
	    b = src[y*slinex+x].b;
	    /*dest[y*dlinex+x].y = (r*0.299 + g*0.587 + b*0.114);
	    dest[y*dlinex+x].u = (r*-0.169 + g*-0.332 + b*0.500 + 128.0);
	    dest[y*dlinex+x].v = (r*0.500 + g*-0.419 + b*-0.0813 + 128.0);*/

	    //dest[y*dlinex+x].y = 128;//(r*((int)( 0.299*256)) + g*((int)( 0.587*256)) + b*((int)( 0.114 *256)))>>8;

	    dest[y*dlinex+x].y = (r*((int)( 0.299*256)) + g*((int)( 0.587*256)) + b*((int)( 0.114 *256)))>>8;
	    dest[y*dlinex+x].u = (r*((int)(-0.169*256)) + g*((int)(-0.332*256)) + b*((int)( 0.500 *256))+ 128*256)>>8;
	    dest[y*dlinex+x].v = (r*((int)( 0.500*256)) + g*((int)(-0.419*256)) + b*((int)(-0.0813*256))+ 128*256)>>8;
	}
    }
}

static void copyregion(VIDEOSTREAM*s, YUV*dest, YUV*src, int bx, int by)
{
    YUV*p1 = &dest[by*s->linex*16+bx*16];
    YUV*p2 = &src[by*s->linex*16+bx*16];
    int y;
    for(y=0;y<16;y++) {
	memcpy(p1, p2, 16*sizeof(YUV));
	p1+=s->linex;p2+=s->linex;
    }
}

static void yuv2rgb(RGBA*dest, YUV*src, int linex, int width, int height)
{
    int x,y;
    for(y=0;y<height;y++) {
	for(x=0;x<width;x++) {
	    int u,v,yy;
	    u = src[y*linex+x].u;
	    v = src[y*linex+x].v;
	    yy = src[y*linex+x].y;
	    dest[y*linex+x].r = truncate256(yy + ((360*(v-128))>>8));
	    dest[y*linex+x].g = truncate256(yy - ((88*(u-128)+183*(v-128))>>8));
	    dest[y*linex+x].b = truncate256(yy + ((455 * (u-128))>>8));
	}
    }
}
static void copy_block_pic(VIDEOSTREAM*s, YUV*dest, block_t*b, int bx, int by)
{
    YUV*p1 = &dest[(by*16)*s->linex+bx*16];
    YUV*p2 = &dest[(by*16+8)*s->linex+bx*16];
    int x,y;
    for(y=0;y<8;y++) {
	for(x=0;x<8;x++) {
	    int u,v,yy;
	    p1[x+0].u = b->u[(y/2)*8+(x/2)];
	    p1[x+0].v = b->v[(y/2)*8+(x/2)];
	    p1[x+0].y = b->y1[y*8+x];
	    p1[x+8].u = b->u[(y/2)*8+(x/2)+4];
	    p1[x+8].v = b->v[(y/2)*8+(x/2)+4];
	    p1[x+8].y = b->y2[y*8+x];
	    p2[x+0].u = b->u[(y/2+4)*8+(x/2)];
	    p2[x+0].v = b->v[(y/2+4)*8+(x/2)];
	    p2[x+0].y = b->y3[y*8+x];
	    p2[x+8].u = b->u[(y/2+4)*8+(x/2)+4];
	    p2[x+8].v = b->v[(y/2+4)*8+(x/2)+4];
	    p2[x+8].y = b->y4[y*8+x];
	}
	p1+=s->linex;
	p2+=s->linex;
    }
}

static int compare_pic_pic(VIDEOSTREAM*s, YUV*pp1, YUV*pp2, int bx, int by)
{
    int linex = s->width;
    YUV*p1 = &pp1[by*linex*16+bx*16];
    YUV*p2 = &pp2[by*linex*16+bx*16];
    int diffy=0, diffuv = 0;
    int x,y;
    for(y=0;y<16;y++) {
	for(x=0;x<16;x++) {
	    YUV*m = &p1[x];
	    YUV*n = &p2[x];
	    int y = m->y - n->y;
	    int u = m->u - n->u;
	    int v = m->v - n->v;
	    diffy += abs(y);
	    diffuv += abs(u)+abs(v);
	}
	p1+=linex;
	p2+=linex;
    }
    return diffy + diffuv/4;
}

static int compare_pic_block(VIDEOSTREAM*s, block_t* b, YUV*pic, int bx, int by)
{
    int linex = s->width;
    YUV*y1 = &pic[(by*2)*linex*8+bx*16];
    YUV*y2 = &pic[(by*2)*linex*8+bx*16+8];
    YUV*y3 = &pic[(by*2+1)*linex*8+bx*16];
    YUV*y4 = &pic[(by*2+1)*linex*8+bx*16+8];
    YUV*uv1 = y1;
    YUV*uv2 = &y1[linex];
    int diffy=0, diffuv = 0;
    int x,y;
    for(y=0;y<8;y++) {
	for(x=0;x<8;x++) {
	    int yy,u1,v1,u2,v2,u3,v3,u4,v4;
	    int y8x = y*8+x;
	    yy = y1[x].y - b->y1[y8x];
	    diffy += abs(yy);
	    yy = y2[x].y - b->y2[y8x];
	    diffy += abs(yy);
	    yy = y3[x].y - b->y3[y8x];
	    diffy += abs(yy);
	    yy = y4[x].y - b->y4[y8x];
	    diffy += abs(yy);
	    u1 = uv1[x*2].u - b->u[y8x];
	    v1 = uv1[x*2].v - b->v[y8x];
	    u2 = uv1[x*2+1].u - b->u[y8x];
	    v2 = uv1[x*2+1].v - b->v[y8x];
	    u3 = uv2[x*2].u - b->u[y8x];
	    v3 = uv2[x*2].v - b->v[y8x];
	    u4 = uv2[x*2+1].u - b->u[y8x];
	    v4 = uv2[x*2+1].v - b->v[y8x];
	    diffuv += (abs(u1)+abs(v1));
	    diffuv += (abs(u2)+abs(v2));
	    diffuv += (abs(u3)+abs(v3));
	    diffuv += (abs(u4)+abs(v4));
	}
	y1+=linex;
	y2+=linex;
	y3+=linex;
	y4+=linex;
	uv1+=linex*2;
	uv2+=linex*2;
    }
    return diffy + diffuv/4;
}

static inline int valtodc(int val)
{
    assert(val>=0);

    /* table 12/h.263 */

    //val+=4; //round
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
static int dctoval(int dc)
{
    int val;
    assert(dc>0);
    assert(dc!=128);
    assert(dc<256);
    /* table 12/h.263 */
    val = dc*8;
    if(val == 255*8)
	val = 128*8;
    return val;
}

/* TODO: we could also just let the caller pass only the string table[index] here */
static int codehuffman(TAG*tag, struct huffcode*table, int index)
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
    return i;
}

static void quantize8x8(int*src, int*dest, int has_dc, int quant)
{
    int t,pos=0;
    double q = 1.0/(quant*2);
    if(has_dc) {
	dest[0] = valtodc((int)src[0]); /*DC*/
	pos++;
    }
    for(t=pos;t<64;t++)
    {
	//dest[t] = (int)src[t];
    /* exact: if(quant&1){dest[t] = (dest[t]/quant - 1)/2;}else{dest[t] = ((dest[t]+1)/quant - 1)/2;} */
        //if(quant&1){dest[t] = (dest[t]/quant - 1)/2;}else{dest[t] = ((dest[t]+1)/quant - 1)/2;}
	//dest[t] = dest[t]/(quant*2);
	dest[t] = (int)(src[t]*q);
	/* TODO: warn if this happens- the video will be buggy */
	if(dest[t]>127) dest[t]=127;
	if(dest[t]<-127) dest[t]=-127;
    }
}

static void dequantize8x8(int*b, int has_dc, int quant)
{
    int t,pos=0;
    if(has_dc) {
	b[0] = dctoval(b[0]); //DC
	pos++;
    }
    for(t=pos;t<64;t++) {
	if(b[t]) {
	    int sign = 0;
	    if(b[t]<0) {
		b[t] = -b[t];
		sign = 1;
	    }

	    if(quant&1) {
		b[t] = quant*(2*b[t]+1); //-7,8,24,40
	    } else {
		b[t] = quant*(2*b[t]+1)-1; //-8,7,23,39
	    }

	    if(sign)
		b[t] = -b[t];
	}

	/* paragraph 6.2.2, "clipping of reconstruction levels": */
	if(b[t]>2047) b[t]=2047;
	if(b[t]<-2048) b[t]=-2048;
    }
}

static int hascoef(int*b, int has_dc)
{
    int t;
    int pos=0;
    if(has_dc)
	pos++;
    for(t=pos;t<64;t++) {
	if(b[t])
	    return 1;
    }
    return 0;
}

static int coefbits8x8(int*bb, int has_dc)
{
    int t;
    int pos=0;
    int bits=0;
    int last;

    if(has_dc) {
	bits+=8;
	pos++;
    }
    for(last=63;last>=pos;last--) {
	if(bb[last])
	    break;
    }
    if(last < pos)
	return bits;
    while(1) {
	int run=0, level=0, islast=0,t;
	while(!bb[pos] && pos<last) {
	    pos++;
	    run++;
	}
	if(pos==last)
	    islast=1;
	level=bb[pos];
	if(level<0) level=-level;
	assert(level);
	for(t=0;t<RLE_ESCAPE;t++) {
	    if(rle_params[t].run == run &&
	       rle_params[t].level == level &&
	       rle_params[t].last == islast) {
		bits += rle[t].len + 1;
		break;
	    }
	}
	if(t==RLE_ESCAPE) {
	    bits += rle[RLE_ESCAPE].len + 1 + 6 + 8;
	}
	if(islast)
	    break;
	pos++;
    }
    return bits;
}

static int encode8x8(TAG*tag, int*bb, int has_dc, int has_tcoef)
{
    int t;
    int pos=0;
    int bits=0;

    if(has_dc) {
	swf_SetBits(tag, bb[0], 8);
	bits += 8;
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
	/* blocks without coefficients should not be included
	   in the cbpy/cbpc patterns: */
	assert(bb[last]);

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
		/* TODO: lookup table */
		if(rle_params[t].run == run &&
		   rle_params[t].level == level &&
		   rle_params[t].last == islast) {
		    bits += codehuffman(tag, rle, t);
		    swf_SetBits(tag, sign, 1);
		    bits += 1;
		    break;
		}
	    }
	    if(t==RLE_ESCAPE) {
		bits += codehuffman(tag, rle, RLE_ESCAPE);
		level=bb[pos];
		/* table 14/h.263 */
		if(!level || level<-127 || level>127) {
		    fprintf(stderr, "Warning: Overflow- Level %d at pos %d\n", level, pos);
		    if(level<-127) level=-127;
		    if(level>127) level=127;
		}

		assert(level);
		assert(level>=-127);
		assert(level<=127); //TODO: known to fail for pos=0 (with custom frames?)

		swf_SetBits(tag, islast, 1);
		swf_SetBits(tag, run, 6);
		swf_SetBits(tag, level, 8); //FIXME: fixme??
		bits += 1 + 6 + 8;
	    }

	    if(islast)
		break;
	    pos++;
	}
    }
    return bits;
}

static void quantize(block_t*fb, block_t*b, int has_dc, int quant)
{
    quantize8x8(fb->y1, b->y1, has_dc, quant);
    quantize8x8(fb->y2, b->y2, has_dc, quant);
    quantize8x8(fb->y3, b->y3, has_dc, quant);
    quantize8x8(fb->y4, b->y4, has_dc, quant);
    quantize8x8(fb->u, b->u, has_dc, quant);
    quantize8x8(fb->v, b->v, has_dc, quant);
}

static void dodct(block_t*fb)
{
    dct(fb->y1); dct(fb->y2); dct(fb->y3); dct(fb->y4);
    dct(fb->u);  dct(fb->v);
    zigzag(fb->y1);
    zigzag(fb->y2);
    zigzag(fb->y3);
    zigzag(fb->y4);
    zigzag(fb->u);
    zigzag(fb->v);
}

static void dodctandquant(block_t*fb, block_t*b, int has_dc, int quant)
{
    int t;
    if(has_dc) {
	dodct(fb);
	quantize(fb,b,has_dc,quant);
	return;
    }
    preparequant(quant);
    dct2(fb->y1,b->y1); dct2(fb->y2,b->y2); dct2(fb->y3,b->y3); dct2(fb->y4,b->y4);
    dct2(fb->u,b->u);  dct2(fb->v,b->v);

    for(t=0;t<64;t++) {
	/* prepare for encoding (only values in (-127..-1,1..127) are
	   allowed as non-zero, non-dc values */
	if(b->y1[t]<-127) b->y1[t]=-127;
	if(b->y2[t]<-127) b->y2[t]=-127;
	if(b->y3[t]<-127) b->y3[t]=-127;
	if(b->y4[t]<-127) b->y4[t]=-127;
	if(b->u[t]<-127) b->u[t]=-127;
	if(b->v[t]<-127) b->v[t]=-127;

	if(b->y1[t]>127) b->y1[t]=127;
	if(b->y2[t]>127) b->y2[t]=127;
	if(b->y3[t]>127) b->y3[t]=127;
	if(b->y4[t]>127) b->y4[t]=127;
	if(b->u[t]>127) b->u[t]=127;
	if(b->v[t]>127) b->v[t]=127;
    }
}

static void doidct(block_t*b)
{
    block_t fb;
    int t;
    for(t=0;t<64;t++) {
	fb.y1[t] = b->y1[zigzagtable[t]];
	fb.y2[t] = b->y2[zigzagtable[t]];
	fb.y3[t] = b->y3[zigzagtable[t]];
	fb.y4[t] = b->y4[zigzagtable[t]];
	fb.u[t] = b->u[zigzagtable[t]];
	fb.v[t] = b->v[zigzagtable[t]];
    }
    idct(fb.y1); idct(fb.y2); idct(fb.y3); idct(fb.y4);
    idct(fb.u);  idct(fb.v);

    memcpy(b, &fb, sizeof(block_t));
}

static void truncateblock(block_t*b)
{
    int t;
    for(t=0;t<64;t++) {
	b->y1[t] = truncate256(b->y1[t]);
	b->y2[t] = truncate256(b->y2[t]);
	b->y3[t] = truncate256(b->y3[t]);
	b->y4[t] = truncate256(b->y4[t]);
	b->u[t] = truncate256(b->u[t]);
	b->v[t] = truncate256(b->v[t]);
    }
}

static void dequantize(block_t*b, int has_dc, int quant)
{
    dequantize8x8(b->y1, has_dc, quant);
    dequantize8x8(b->y2, has_dc, quant);
    dequantize8x8(b->y3, has_dc, quant);
    dequantize8x8(b->y4, has_dc, quant);
    dequantize8x8(b->u, has_dc, quant);
    dequantize8x8(b->v, has_dc, quant);
}

static void getblockpatterns(block_t*b, int*cbpybits,int*cbpcbits, int has_dc)
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

static void setQuant(TAG*tag, int dquant)
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

static void change_quant(int quant, int*dquant)
{
    /* TODO */
    *dquant = 0;
}

static void yuvdiff(block_t*a, block_t*b)
{
    int t;
    for(t=0;t<64;t++) {
	a->y1[t] = (a->y1[t] - b->y1[t]);
	a->y2[t] = (a->y2[t] - b->y2[t]);
	a->y3[t] = (a->y3[t] - b->y3[t]);
	a->y4[t] = (a->y4[t] - b->y4[t]);
	a->u[t]  = (a->u[t] - b->u[t]);
	a->v[t]  = (a->v[t] - b->v[t]);
    }
}

static void predictmvd(VIDEOSTREAM*s, int bx, int by, int*px, int*py)
{
    int i1,i2;
    int x1,y1,x2,y2,x3,y3;
    int x4,y4,p;
    if(bx) {x1=s->mvdx[by*s->bbx+bx-1];
	    y1=s->mvdy[by*s->bbx+bx-1];
    } else {x1=y1=0;}

    if(by) {x2=s->mvdx[(by-1)*s->bbx+bx];
	    y2=s->mvdy[(by-1)*s->bbx+bx];
	    if(bx<s->bbx-1) {
		x3=s->mvdx[(by-1)*s->bbx+bx+1];
		y3=s->mvdy[(by-1)*s->bbx+bx+1];
	    } else {
		x3=y3=0;
	    }
           }
    else   {x2=x3=x1;y2=y3=y1;}

	   if((x1 <= x2 && x2 <= x3) ||
	      (x3 <= x2 && x2 <= x1)) {
	x4=x2;
    } else if((x2 <= x1 && x1 <= x3) ||
	      (x3 <= x1 && x1 <= x2)) {
	x4=x1;
    } else if((x1 <= x3 && x3 <= x2) ||
	      (x2 <= x3 && x3 <= x1)) {
	x4=x3;
    } else {
	x4=0;
	assert(x4);
    }

	   if((y1 <= y2 && y2 <= y3) ||
	      (y3 <= y2 && y2 <= y1)) {
	y4=y2;
    } else if((y2 <= y1 && y1 <= y3) ||
	      (y3 <= y1 && y1 <= y2)) {
	y4=y1;
    } else if((y1 <= y3 && y3 <= y2) ||
	      (y2 <= y3 && y3 <= y1)) {
	y4=y3;
    } else {
	y4=0;
	assert(y4);
    }

    *px = x4;
    *py = y4;
    assert((x4>=-32 && x4<=31) && (y4>=-32 && y4<=31));
}

static inline int mvd2index(int px, int py, int x, int y, int xy)
{

    if((x<-32 && x>31) || (y<-32 && y>31)) 
	fprintf(stderr, "(%d,%d)\n", x,y);
    assert((x>=-32 && x<=31) && (y>=-32 && y<=31));
    //assert((x&1)==0 && (y&1)==0);//for now
    //assert((x&2)==0 && (y&2)==0);//for now(2)

    x-=px;
    y-=py;

    if(xy)
	x=y;
    x+=32;

    /* (x&63) */
    if(x>63)
	x-=64;
    if(x<0)
	x+=64;

    assert(x>=0 && x<64);
    return x;
}

typedef struct _iblockdata_t
{
    block_t b; //transformed quantized coefficients
    block_t reconstruction;
    int bits;
    int bx,by;
    struct huffcode*ctable; //table to use for chrominance encoding (different for i-frames)
    int iframe; // 1 if this is part of an iframe
} iblockdata_t;

typedef struct _mvdblockdata_t
{
    block_t b;
    block_t fbold;
    block_t reconstruction;
    int xindex;
    int yindex;
    int movex;
    int movey;
    int bits;
    int bx,by;
} mvdblockdata_t;

void prepareIBlock(VIDEOSTREAM*s, iblockdata_t*data, int bx, int by, block_t* fb, int*bits, int iframe)
{
    /* consider I-block */
    block_t fb_i;
    block_t b;
    int y,c;
    struct huffcode*ctable;

    data->bx = bx;
    data->by = by;

    data->iframe = iframe;
    if(!iframe) {
	data->ctable = &mcbpc_inter[3*4];
    } else {
	data->ctable = &mcbpc_intra[0];
    }

    memcpy(&fb_i, fb, sizeof(block_t));
    dodctandquant(&fb_i, &data->b, 1, s->quant);
    getblockpatterns(&data->b, &y, &c, 1);
    *bits = 0;
    if(!data->iframe) {
	*bits += 1; //cod
    }
    *bits += data->ctable[c].len;
    *bits += cbpy[y].len;
    *bits += coefbits8x8(data->b.y1, 1);
    *bits += coefbits8x8(data->b.y2, 1);
    *bits += coefbits8x8(data->b.y3, 1);
    *bits += coefbits8x8(data->b.y4, 1);
    *bits += coefbits8x8(data->b.u, 1);
    *bits += coefbits8x8(data->b.v, 1);
    data->bits = *bits;
    
    /* -- reconstruction -- */
    memcpy(&data->reconstruction,&data->b,sizeof(block_t));
    dequantize(&data->reconstruction, 1, s->quant);
    doidct(&data->reconstruction);
    truncateblock(&data->reconstruction);
}

int writeIBlock(VIDEOSTREAM*s, TAG*tag, iblockdata_t*data)
{
    int c = 0, y = 0;
    int has_dc=1;
    int bits = 0;
    block_t b;

    getblockpatterns(&data->b, &y, &c, has_dc);
    if(!data->iframe) {
	swf_SetBits(tag,0,1); bits += 1; // COD
    }
    bits += codehuffman(tag, data->ctable, c);
    bits += codehuffman(tag, cbpy, y);

    /* luminance */
    bits += encode8x8(tag, data->b.y1, has_dc, y&8);
    bits += encode8x8(tag, data->b.y2, has_dc, y&4);
    bits += encode8x8(tag, data->b.y3, has_dc, y&2);
    bits += encode8x8(tag, data->b.y4, has_dc, y&1);

    /* chrominance */
    bits += encode8x8(tag, data->b.u, has_dc, c&2);
    bits += encode8x8(tag, data->b.v, has_dc, c&1);

    copy_block_pic(s, s->current, &data->reconstruction, data->bx, data->by);
    assert(data->bits == bits);
    return bits;
}

int getmvdbits(VIDEOSTREAM*s,block_t*fb, int bx,int by,int hx,int hy)
{
    block_t b;
    block_t fbold;
    block_t fbdiff;
    int bits = 0;
    memcpy(&fbdiff, fb, sizeof(block_t));
    getmvdregion(&fbold, s->oldpic, bx, by, hx, hy, s->linex);
    yuvdiff(&fbdiff, &fbold);
    dodctandquant(&fbdiff, &b, 0, s->quant);
    bits += coefbits8x8(b.y1, 0);
    bits += coefbits8x8(b.y2, 0);
    bits += coefbits8x8(b.y3, 0);
    bits += coefbits8x8(b.y4, 0);
    bits += coefbits8x8(b.u, 0);
    bits += coefbits8x8(b.v, 0);
    return bits;
}

void prepareMVDBlock(VIDEOSTREAM*s, mvdblockdata_t*data, int bx, int by, block_t* fb, int*bits)
{ /* consider mvd(x,y)-block */

    int t;
    int y,c;
    block_t fbdiff;
    int predictmvdx;
    int predictmvdy;

    data->bx = bx;
    data->by = by;
    predictmvd(s,bx,by,&predictmvdx,&predictmvdy);

    data->bits = 65535;
    data->movex=0;
    data->movey=0;

    if(s->do_motion) {
	int hx,hy;
	int bestx=0,besty=0,bestbits=65536;
	int startx=-32,endx=31;
	int starty=-32,endy=31;

	if(!bx) startx=0;
	if(!by) starty=0;
	if(bx==s->bbx-1) endx=0;
	if(by==s->bby-1) endy=0;

	for(hx=startx;hx<=endx;hx+=4)
	for(hy=starty;hy<=endy;hy+=4)
	{
	    int bits = 0;
	    bits = getmvdbits(s,fb,bx,by,hx,hy);
	    if(bits<bestbits) {
		bestbits = bits;
		bestx = hx;
		besty = hy;
	    }
	}
	
	if(bestx-3 > startx) startx = bestx-3;
	if(besty-3 > starty) starty = besty-3;
	if(bestx+3 < endx) endx = bestx+3;
	if(besty+3 < endy) endy = besty+3;

	for(hx=startx;hx<=endx;hx++)
	for(hy=starty;hy<=endy;hy++)
	{
	    int bits = 0;
	    bits = getmvdbits(s,fb,bx,by,hx,hy);
	    if(bits<bestbits) {
		bestbits = bits;
		bestx = hx;
		besty = hy;
	    }
	}
	data->movex = bestx;
	data->movey = besty;
    }

    memcpy(&fbdiff, fb, sizeof(block_t));
    getmvdregion(&data->fbold, s->oldpic, bx, by, data->movex, data->movey, s->linex);
    yuvdiff(&fbdiff, &data->fbold);
    dodctandquant(&fbdiff, &data->b, 0, s->quant);
    getblockpatterns(&data->b, &y, &c, 0);

    data->xindex = mvd2index(predictmvdx, predictmvdy, data->movex, data->movey, 0);
    data->yindex = mvd2index(predictmvdx, predictmvdy, data->movex, data->movey, 1);

    *bits = 1; //cod
    *bits += mcbpc_inter[0*4+c].len;
    *bits += cbpy[y^15].len;
    *bits += mvd[data->xindex].len; // (0,0)
    *bits += mvd[data->yindex].len;
    *bits += coefbits8x8(data->b.y1, 0);
    *bits += coefbits8x8(data->b.y2, 0);
    *bits += coefbits8x8(data->b.y3, 0);
    *bits += coefbits8x8(data->b.y4, 0);
    *bits += coefbits8x8(data->b.u, 0);
    *bits += coefbits8x8(data->b.v, 0);
    data->bits = *bits;

    /* -- reconstruction -- */
    memcpy(&data->reconstruction, &data->b, sizeof(block_t));
    dequantize(&data->reconstruction, 0, s->quant);
    doidct(&data->reconstruction);
    for(t=0;t<64;t++) {
	data->reconstruction.y1[t] = 
	    truncate256(data->reconstruction.y1[t] + (int)data->fbold.y1[t]);
	data->reconstruction.y2[t] = 
	    truncate256(data->reconstruction.y2[t] + (int)data->fbold.y2[t]);
	data->reconstruction.y3[t] = 
	    truncate256(data->reconstruction.y3[t] + (int)data->fbold.y3[t]);
	data->reconstruction.y4[t] = 
	    truncate256(data->reconstruction.y4[t] + (int)data->fbold.y4[t]);
	data->reconstruction.u[t] = 
	    truncate256(data->reconstruction.u[t] + (int)data->fbold.u[t]);
	data->reconstruction.v[t] = 
	    truncate256(data->reconstruction.v[t] + (int)data->fbold.v[t]);
    }
}

int writeMVDBlock(VIDEOSTREAM*s, TAG*tag, mvdblockdata_t*data)
{
    int c = 0, y = 0;
    int t;
    int has_dc=0; // mvd w/o mvd24
    /* mvd (0,0) block (mode=0) */
    int mode = 0;
    int bx = data->bx;
    int by = data->by;
    int bits = 0;

    getblockpatterns(&data->b, &y, &c, has_dc);
    swf_SetBits(tag,0,1); bits += 1; // COD
    bits += codehuffman(tag, mcbpc_inter, mode*4+c);
    bits += codehuffman(tag, cbpy, y^15);

    /* vector */
    bits += codehuffman(tag, mvd, data->xindex);
    bits += codehuffman(tag, mvd, data->yindex);

    /* luminance */
    bits += encode8x8(tag, data->b.y1, has_dc, y&8);
    bits += encode8x8(tag, data->b.y2, has_dc, y&4);
    bits += encode8x8(tag, data->b.y3, has_dc, y&2);
    bits += encode8x8(tag, data->b.y4, has_dc, y&1);

    /* chrominance */
    bits += encode8x8(tag, data->b.u, has_dc, c&2);
    bits += encode8x8(tag, data->b.v, has_dc, c&1);

    s->mvdx[by*s->bbx+bx] = data->movex;
    s->mvdy[by*s->bbx+bx] = data->movey;

    copy_block_pic(s, s->current, &data->reconstruction, data->bx, data->by);
    assert(data->bits == bits);
    return bits;
}

static int encode_PFrame_block(TAG*tag, VIDEOSTREAM*s, int bx, int by)
{
    block_t fb;
    int diff1,diff2;
    int bits_i;
    int bits_vxy;

    iblockdata_t iblock;
    mvdblockdata_t mvdblock;
    
    getregion(&fb, s->current, bx, by, s->linex);
    prepareIBlock(s, &iblock, bx, by, &fb, &bits_i, 0);

    /* encoded last frame <=> original current block: */
    diff1 = compare_pic_pic(s, s->current, s->oldpic, bx, by);
    /* encoded current frame <=> original current block: */
    diff2 = compare_pic_block(s, &iblock.reconstruction, s->current, bx, by);

    if(diff1 <= diff2) {
	swf_SetBits(tag, 1,1); /* cod=1, block skipped */
	/* copy the region from the last frame so that we have a complete reconstruction */
	copyregion(s, s->current, s->oldpic, bx, by);
	return 1;
    }
    prepareMVDBlock(s, &mvdblock, bx, by, &fb, &bits_vxy);

    if(bits_i > bits_vxy) {
	return writeMVDBlock(s, tag, &mvdblock);
    } else {
	return writeIBlock(s, tag, &iblock);
    }
}

/* should be called encode_IFrameBlock */
static void encode_IFrame_block(TAG*tag, VIDEOSTREAM*s, int bx, int by)
{
    block_t fb;
    iblockdata_t data;
    int bits;

    getregion(&fb, s->current, bx, by, s->width);
    prepareIBlock(s, &data, bx, by, &fb, &bits, 1);
    writeIBlock(s, tag, &data);
}

#ifdef MAIN
static int bmid = 0;

void setdbgpic(TAG*tag, RGBA*pic, int width, int height)
{
    MATRIX m;
    tag = tag->prev;

    tag = swf_InsertTag(tag,ST_REMOVEOBJECT2);
    swf_SetU16(tag, 133);

    tag = swf_InsertTag(tag, ST_DEFINEBITSLOSSLESS);
    swf_SetU16(tag, 1000+bmid);
    swf_SetLosslessBits(tag, width, height, (void*)pic, BMF_32BIT);

    tag = swf_InsertTag(tag, ST_DEFINESHAPE);
    swf_SetU16(tag, 2000+bmid);
    swf_ShapeSetBitmapRect(tag, 1000+bmid, width, height);

    tag = swf_InsertTag(tag,ST_PLACEOBJECT2);
    swf_GetMatrix(0,&m);
    m.tx = width*20;
    swf_ObjectPlace(tag, 2000+bmid, 133, &m, 0, 0);

    bmid++;
}
#endif

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
    assert(quant>0);
    assert(quant<32);
    swf_SetBits(tag, quant, 5); /* quantizer (1-31), may be updated later on*/
    swf_SetBits(tag, 0, 1); /* No extra info */
}

void swf_SetVideoStreamIFrame(TAG*tag, VIDEOSTREAM*s, RGBA*pic, int quant)
{
    int bx, by;

    if(quant<1) quant=1;
    if(quant>31) quant=31;
    s->quant = quant;

    writeHeader(tag, s->width, s->height, s->frame, quant, TYPE_IFRAME);

    /* fixme: should fill with 0,128,128, not 0,0,0 */
    memset(s->current, 0, s->linex*s->height*sizeof(YUV));

    rgb2yuv(s->current, pic, s->linex, s->olinex, s->owidth, s->oheight);

    for(by=0;by<s->bby;by++)
    {
	for(bx=0;bx<s->bbx;bx++)
	{
	    encode_IFrame_block(tag, s, bx, by);
	}
    }
    s->frame++;
    memcpy(s->oldpic, s->current, s->width*s->height*sizeof(YUV));
}
void swf_SetVideoStreamBlackFrame(TAG*tag, VIDEOSTREAM*s)
{
    int bx, by;
    int quant = 31;
    int x,y;
    s->quant = quant;

    writeHeader(tag, s->width, s->height, s->frame, quant, TYPE_IFRAME);

    for(y=0;y<s->height;y++)
    for(x=0;x<s->width;x++) {
	s->current[y*s->width+x].y = 0;
	s->current[y*s->width+x].u = 128;
	s->current[y*s->width+x].v = 128;
    }
    for(x=0;x<16;x++)
    for(y=0;y<16;y++) {
	s->current[y*s->width+x].y = 64; 
	s->current[y*s->width+x].u = 128; 
	s->current[y*s->width+x].v = 128;
    }

    for(by=0;by<s->bby;by++)
    {
	for(bx=0;bx<s->bbx;bx++)
	{
	    encode_IFrame_block(tag, s, bx, by);
	}
    }
    s->frame++;
    memcpy(s->oldpic, s->current, s->width*s->height*sizeof(YUV));
}

void swf_SetVideoStreamPFrame(TAG*tag, VIDEOSTREAM*s, RGBA*pic, int quant)
{
    int bx, by;

    if(quant<1) quant=1;
    if(quant>31) quant=31;
    s->quant = quant;

    writeHeader(tag, s->width, s->height, s->frame, quant, TYPE_PFRAME);

    /* fixme: should fill with 0,128,128, not 0,0,0 */
    memset(s->current, 0, s->linex*s->height*sizeof(YUV));

    rgb2yuv(s->current, pic, s->linex, s->olinex, s->owidth, s->oheight);
    memset(s->mvdx, 0, s->bbx*s->bby*sizeof(int));
    memset(s->mvdy, 0, s->bbx*s->bby*sizeof(int));

    for(by=0;by<s->bby;by++)
    {
	for(bx=0;bx<s->bbx;bx++)
	{
	    encode_PFrame_block(tag, s, bx, by);
	}
    }
    s->frame++;
    memcpy(s->oldpic, s->current, s->width*s->height*sizeof(YUV));

#ifdef MAIN
#ifdef PNG
    yuv2rgb(pic, s->current, s->linex, s->width, s->height);
    setdbgpic(tag, pic, s->width, s->height);
#endif
#endif
}

void swf_SetVideoStreamMover(TAG*tag, VIDEOSTREAM*s, signed char* movex, signed char* movey, void**pictures, int quant)
{
    int bx, by;
    YUV pic[16*16];

    if(quant<1) quant=1;
    if(quant>31) quant=31;
    s->quant = quant;

    writeHeader(tag, s->width, s->height, s->frame, quant, TYPE_PFRAME);

    memset(s->mvdx, 0, s->bbx*s->bby*sizeof(int));
    memset(s->mvdy, 0, s->bbx*s->bby*sizeof(int));

    for(by=0;by<s->bby;by++)
    {
	for(bx=0;bx<s->bbx;bx++)
	{
	    int predictmvdx=0, predictmvdy=0;
	    int mvx=movex[by*s->bbx+bx];
	    int mvy=movey[by*s->bbx+bx];
	    void*picture = pictures?pictures[by*s->bbx+bx]:0;
    
	    if(mvx<-32) mvx=-32;
	    if(mvx>31) mvx=31;
	    if(mvy<-32) mvy=-32;
	    if(mvy>31) mvy=31;

	    if(mvx == 0 && mvy == 0 && picture == 0) {
		swf_SetBits(tag,1,1); // COD skip
	    } else {
		int mode = 0;
		int has_dc=0;
		int y=0,c=0;
		block_t b;
		block_t b2;
		
		swf_SetBits(tag,0,1); // COD

		if(mvx==0 && mvy==0 && picture) { // only picture
		    mode = 3;
		    has_dc = 1;
		}

		if(picture) {
		    RGBA* picblock = (RGBA*)picture;
		    rgb2yuv(pic, picblock,16,16,16,16);
		    /* TODO: if has_dc!=1, subtract 128 from rgb values */
		    getregion(&b, pic, 0,0,16);
		    dodctandquant(&b, &b2, 1, s->quant);
		    getblockpatterns(&b2, &y, &c, 1);
		} else {
		    y=0;c=0;
		}

		codehuffman(tag, mcbpc_inter, mode*4+c);
		codehuffman(tag, cbpy, mode==3?y:y^15);
		
		if(mode < 3) {
		    /* has motion vector */
		    predictmvd(s,bx,by,&predictmvdx,&predictmvdy);
		    codehuffman(tag, mvd, mvd2index(predictmvdx, predictmvdy, mvx, mvy, 0));
		    codehuffman(tag, mvd, mvd2index(predictmvdx, predictmvdy, mvx, mvy, 1));
		    s->mvdx[by*s->bbx+bx] = mvx;
		    s->mvdy[by*s->bbx+bx] = mvy;
		}

		if(has_dc||y||c) {
		    encode8x8(tag, b2.y1, has_dc, y&8);
		    encode8x8(tag, b2.y2, has_dc, y&4);
		    encode8x8(tag, b2.y3, has_dc, y&2);
		    encode8x8(tag, b2.y4, has_dc, y&1);
		    encode8x8(tag, b2.u, has_dc, c&2);
		    encode8x8(tag, b2.v, has_dc, c&1);
		}
	    }
	}
    }
    s->frame++;
}

#define TESTS
#ifdef TESTS
void test_copy_diff()
{
    VIDEOSTREAM stream;
    VIDEOSTREAM* s = &stream;
    TAG*tag;
    RGBA*pic = (RGBA*)rfx_alloc(256*256*sizeof(RGBA));
    block_t fb;
    int x,y;
    int bx,by;
    for(x=0;x<256;x++)
    for(y=0;y<256;y++) {
	pic[y*256+x].r = x*y;
	pic[y*256+x].g = x+y;
	pic[y*256+x].b = (x+1)%(y+1);
    }
    tag = swf_InsertTag(0, ST_DEFINEVIDEOSTREAM);
    swf_SetU16(tag, 33);
    swf_SetVideoStreamDefine(tag, s, 10, 256, 256);
    
    rgb2yuv(s->current, pic, s->linex, s->olinex, s->owidth, s->oheight);
    for(by=0;by<16;by++)
    for(bx=0;bx<16;bx++) {
	int diff1,diff2;
	/* test1: does compare pic pic return zero for identical blocks? */
	diff1 = compare_pic_pic(s, s->current, s->current, bx, by);
	assert(!diff1);
	/* test2: do blocks which are copied back return zero diff? */
	getregion(&fb, s->current, bx, by, s->linex);
	copy_block_pic(s, s->oldpic, &fb, bx, by);
	diff1 = compare_pic_block(s, &fb, s->oldpic, bx, by);
	assert(!diff1);
	/* test3: does compare_pic_block return the same result as compare_pic_pic? */
	getregion(&fb, s->current, 15-bx, 15-by, s->linex);
	copy_block_pic(s, s->oldpic, &fb, bx, by);
	diff1 = compare_pic_block(s, &fb, s->current, bx, by);
	diff2 = compare_pic_pic(s, s->current, s->oldpic, bx, by);
	assert(diff1 == diff2);
    }
}

#endif

#ifdef MAIN
#include "png.h"

int compileSWFActionCode(const char *script, int version, void**data, int*len) {return 0;}

void mkblack()
{
    SWF swf;
    SWFPLACEOBJECT obj;
    int frames = 88;
    int width = 160;
    int height = 112;
    int x,y;
    TAG*tag = 0;
    RGBA rgb;
    RGBA* pic = 0;
    VIDEOSTREAM stream;
   
    pic = rfx_calloc(width*height*4);

    memset(&swf,0,sizeof(SWF));
    memset(&obj,0,sizeof(obj));

    swf.fileVersion    = 6;
    swf.frameRate      = 15*256;
    swf.movieSize.xmax = 20*width;
    swf.movieSize.ymax = 20*height;

    swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    tag = swf.firstTag;
    rgb.r = 0x00;rgb.g = 0x30;rgb.b = 0xff;
    swf_SetRGB(tag,&rgb);

    tag = swf_InsertTag(tag, ST_DEFINEVIDEOSTREAM);
    swf_SetU16(tag, 1);
    swf_SetVideoStreamDefine(tag, &stream, frames, width, height);
    stream.do_motion = 0;

    for(y=0;y<height;y++)  {
	for(x=0;x<width;x++) {
	    int dx = x/16;
	    int dy = y/16;

	    pic[y*width+x].r = 0;
	    pic[y*width+x].g = 0;
	    pic[y*width+x].b = 0;
	    pic[y*width+x].a = 0;
	}
    }
    tag = swf_InsertTag(tag, ST_VIDEOFRAME);
    swf_SetU16(tag, 1);
    
    swf_SetVideoStreamIFrame(tag, &stream, pic, 7);

    tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
    swf_GetPlaceObject(0, &obj);
    
    obj.depth = 4;
    obj.id = 1;
    
    swf_SetPlaceObject(tag,&obj);

    tag = swf_InsertTag(tag, ST_SHOWFRAME);
    
    swf_VideoStreamClear(&stream);

    tag = swf_InsertTag(tag, ST_END);

    int fi = open("black.swf", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if(swf_WriteSWF(fi,&swf)<0) {
	fprintf(stderr,"WriteSWF() failed.\n");
    }
    close(fi);
    swf_FreeTags(&swf);
}

int main(int argn, char*argv[])
{
    int fi;
    int t;
    SWF swf;
    TAG * tag;
    RGBA* pic, *pic2, rgb;
    SWFPLACEOBJECT obj;
    unsigned width = 0;
    unsigned height = 0;
    int frames = 10;
    int framerate = 29;
    unsigned char*data;
    char* fname = "/home/kramm/pics/peppers_fromjpg.png";
    //char* fname = "/home/kramm/pics/baboon.png";
    VIDEOSTREAM stream;
    double d = 1.0;

#ifdef TESTS
    test_copy_diff();
#endif

    mkblack();

    memset(&stream, 0, sizeof(stream));

    png_load(fname, &width, &height, &data);
    pic = (RGBA*)rfx_alloc(width*height*sizeof(RGBA));
    pic2 = (RGBA*)rfx_alloc(width*height*sizeof(RGBA));
    memcpy(pic, data, width*height*sizeof(RGBA));
    rfx_free(data);

    printf("Compressing %s, size %dx%d\n", fname, width, height);

    memset(&swf,0,sizeof(SWF));
    memset(&obj,0,sizeof(obj));

    swf.fileVersion    = 6;
    swf.frameRate      = framerate*256;
    swf.movieSize.xmax = 20*width*2;
    swf.movieSize.ymax = 20*height;

    swf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    tag = swf.firstTag;
    rgb.r = 0x00;rgb.g = 0x30;rgb.b = 0xff;
    swf_SetRGB(tag,&rgb);

    tag = swf_InsertTag(tag, ST_DEFINEVIDEOSTREAM);
    swf_SetU16(tag, 33);
    swf_SetVideoStreamDefine(tag, &stream, frames, width, height);
    stream.do_motion = 0;

    //srand48(time(0));

    for(t=0;t<frames;t++)
    {
	int x,y;
	double xx,yy;
	for(y=0,yy=0;y<height;y++,yy+=d)  {
	    RGBA*line = &pic[((int)yy)*width];
	    for(x=0,xx=0;x<width;x++,xx+=d) {
		int dx = x/16;
		int dy = y/16;
		if(dx==0 && dy==0) {
		    pic2[y*width+x] = line[((int)xx)];
		    pic2[y*width+x].r+=2;
		    pic2[y*width+x].g+=2;
		    pic2[y*width+x].b+=2;
		} else {
		    //pic2[y*width+x] = line[((int)xx)];
		    //pic2[y*width+x].r = lrand48();//line[((int)xx)];
		    //pic2[y*width+x].g = lrand48();//line[((int)xx)];
		    //pic2[y*width+x].b = lrand48();//line[((int)xx)];
		    pic2[y*width+x].r = 0;
		    pic2[y*width+x].g = 0;
		    pic2[y*width+x].b = 0;
		}
		/*if(dx==16 && dy==16) 
		    pic2[y*width+x] = pic[(y-16*16)*width+(x-16*16)];*/
		/*if(dx<=0 && dy<=0) {
		    pic2[y*width+x] = line[((int)xx)];*/
		/*if(x==0 && y==0) {
		    RGBA color;
		    memset(&color, 0, sizeof(RGBA));
		    pic2[y*width+x] = color;*/
		/*} else  {
		    RGBA color;
		    color.r = lrand48();
		    color.g = lrand48();
		    color.b = lrand48();
		    color.a = 0;
		    pic2[y*width+x] = color;
		}*/
	    }
	}
	printf("frame:%d\n", t);fflush(stdout);

	if(t==1)
	    break;

	tag = swf_InsertTag(tag, ST_VIDEOFRAME);
	swf_SetU16(tag, 33);
	if(t==0)
	    swf_SetVideoStreamIFrame(tag, &stream, pic2, 7);
	else {
	    swf_SetVideoStreamPFrame(tag, &stream, pic2, 7);
	}

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
	d-=0.015;
    }
    swf_VideoStreamClear(&stream);

    tag = swf_InsertTag(tag, ST_END);

    fi = open("video3.swf", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if(swf_WriteSWF(fi,&swf)<0) {
	fprintf(stderr,"WriteSWF() failed.\n");
    }
    close(fi);
    swf_FreeTags(&swf);
    return 0;
}
#undef MAIN
#endif
