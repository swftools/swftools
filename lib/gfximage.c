#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <memory.h>
#include <assert.h>
#include "../config.h"
#include "jpeg.h"
#include "png.h"
#include "mem.h"
#include "gfximage.h"
#include "types.h"
#ifdef HAVE_FFTW3
#include <fftw3.h>
#endif

#define MOD(x,d) (((x)+(d))%(d))

gfximage_t*gfximage_new(int width, int height)
{
    gfximage_t*i = rfx_calloc(sizeof(gfximage_t));
    i->data = rfx_calloc(width*height*4);
    i->width = width;
    i->height = height;
    return i;
}

void gfximage_save_jpeg(gfximage_t*img, const char*filename, int quality)
{
    int x,y;
    int l = img->width*img->height;
    unsigned char*data = (unsigned char*)rfx_alloc(img->width*img->height*3);
    int s,t;
    for(t=0,s=0;t<l;s+=3,t++) {
	data[s+0] = img->data[t].r;
	data[s+1] = img->data[t].g;
	data[s+2] = img->data[t].b;
    }
    jpeg_save(data, img->width, img->height, quality, filename);
    free(data);
}

void gfximage_save_png(gfximage_t*image, const char*filename)
{
    png_write(filename, (void*)image->data, image->width, image->height);
}

void gfximage_save_png_quick(gfximage_t*image, const char*filename)
{
    png_write_quick(filename, (void*)image->data, image->width, image->height);
}

typedef struct scale_lookup {
    int pos;
    unsigned int weight;
} scale_lookup_t;

typedef struct rgba_int {
    unsigned int r,g,b,a;
} rgba_int_t;

static int bicubic = 0;

static scale_lookup_t**make_scale_lookup(int width, int newwidth)
{
    scale_lookup_t*lookupx = (scale_lookup_t*)rfx_alloc((width>newwidth?width:newwidth)*2*sizeof(scale_lookup_t));
    scale_lookup_t**lblockx = (scale_lookup_t**)rfx_alloc((newwidth+1)*sizeof(scale_lookup_t**));
    double fx = ((double)width)/((double)newwidth);
    double px = 0;
    int x;
    scale_lookup_t*p_x = lookupx;

    if(newwidth<=width) {
	for(x=0;x<newwidth;x++) {
	    double ex = px + fx;
	    int fromx = (int)px;
	    int tox = (int)ex;
	    double rem = fromx+1-px;
	    int i = (int)(256/fx);
	    int xweight = (int)(rem*256/fx);
	    int xx;
	    int w = 0;
	    lblockx[x] = p_x;
	    if(tox>=width) tox = width-1;
	    for(xx=fromx;xx<=tox;xx++) {
		if(xx==fromx && xx==tox) p_x->weight = 256;
		else if(xx==fromx) p_x->weight = xweight;
		else if(xx==tox) p_x->weight = 256-w;
		else p_x->weight = i;
		w+=p_x->weight;
		p_x->pos = xx;
		p_x++;
	    }
	    px = ex;
	}
    } else {
	for(x=0;x<newwidth;x++) {
	    int ix1 = (int)px;
	    int ix2 = ((int)px)+1;
	    double r = px-ix1;
	    if(ix2>=width) ix2=width-1;
	    lblockx[x] = p_x;
	    if(bicubic)
		r = -2*r*r*r+3*r*r;
	    p_x[0].weight = (int)(256*(1-r));
	    p_x[0].pos = ix1;
	    p_x[1].weight = 256-p_x[0].weight;
	    p_x[1].pos = ix2;
	    p_x+=2;
	    px += fx;
	}
    }
    lblockx[newwidth] = p_x;
    return lblockx;
}

static void encodeMonochromeImage(gfxcolor_t*data, int width, int height, gfxcolor_t*colors)
{
    int t;
    int len = width*height;

    U32* img = (U32*)data;
    U32 color1 = img[0];
    U32 color2 = 0;
    for(t=1;t<len;t++) {
	if(img[t] != color1) {
	    color2 = img[t];
	    break;
	}
    }
    *(U32*)&colors[0] = color1;
    *(U32*)&colors[1] = color2;
    for(t=0;t<len;t++) {
	if(img[t] == color1) {
	    img[t] = 0;
	} else {
	    img[t] = 0xffffffff;
	}
    }
}

static void decodeMonochromeImage(gfxcolor_t*data, int width, int height, gfxcolor_t*colors)
{
    int t;
    int len = width*height;

    for(t=0;t<len;t++) {
	U32 m = data[t].a;
	data[t].r = (colors[0].r * (255-m) + colors[1].r * m) >> 8;
	data[t].g = (colors[0].g * (255-m) + colors[1].g * m) >> 8;
	data[t].b = (colors[0].b * (255-m) + colors[1].b * m) >> 8;
	data[t].a = (colors[0].a * (255-m) + colors[1].a * m) >> 8;
    }
}

void blurImage(gfxcolor_t*src, int width, int height, int r)  __attribute__ ((noinline));

void blurImage(gfxcolor_t*src, int width, int height, int r)
{
    int e = 2; // r times e is the sampling interval
    double*gauss = (double*)rfx_alloc(r*e*sizeof(double));
    double sum=0;
    int x;
    for(x=0;x<r*e;x++) {
        double t = (x - r*e/2.0)/r;
        gauss[x] = exp(-0.5*t*t);
        sum += gauss[x];
    }
    int*weights = (int*)rfx_alloc(r*e*sizeof(int));
    for(x=0;x<r*e;x++) {
        weights[x] = (int)(gauss[x]*65536.0001/sum);
    }
    int range = r*e/2;

    gfxcolor_t*tmp = rfx_alloc(sizeof(gfxcolor_t)*width*height);

    int y;
    for(y=0;y<height;y++) {
        gfxcolor_t*s = &src[y*width];
        gfxcolor_t*d = &tmp[y*width];
        for(x=0;x<range && x<width;x++) {
            d[x] = s[x];
        }
        for(;x<width-range;x++) {
            int r=0;
            int g=0;
            int b=0;
            int a=0;
            int*f = weights;
            int xx;
            for(xx=x-range;xx<x+range;xx++) {
                r += s[xx].r * f[0];
                g += s[xx].g * f[0];
                b += s[xx].b * f[0];
                a += s[xx].a * f[0];
                f++;
            }
            d[x].r = r >> 16;
            d[x].g = g >> 16;
            d[x].b = b >> 16;
            d[x].a = a >> 16;
        }
        for(;x<width;x++) {
            d[x] = s[x];
        }
    }

    for(x=0;x<width;x++) {
        gfxcolor_t*s = &tmp[x];
        gfxcolor_t*d = &src[x];
        int yy=0;
        for(y=0;y<range&&y<height;y++) {
            d[yy] = s[yy];
            yy+=width;
        }
        for(;y<height-range;y++) {
            int r=0;
            int g=0;
            int b=0;
            int a=0;
            int*f = weights;
            int cy,cyy=yy-range*width;
            for(cy=y-range;cy<y+range;cy++) {
                r += s[cyy].r * f[0];
                g += s[cyy].g * f[0];
                b += s[cyy].b * f[0];
                a += s[cyy].a * f[0];
                cyy += width;
                f++;
            }
            d[yy].r = r >> 16;
            d[yy].g = g >> 16;
            d[yy].b = b >> 16;
            d[yy].a = a >> 16;
            yy += width;
        }
        for(;y<height;y++) {
            d[yy] = s[yy];
            yy += width;
        }
    }

    rfx_free(tmp);
    rfx_free(weights);
    rfx_free(gauss);
}

int gfximage_getNumberOfPaletteEntries(gfximage_t*img)
{
    int width = img->width;
    int height = img->height;

    int size = width*height;
    int t;
    U32* data = (U32*)img->data;
    U32 color1 = data[0];
    U32 color2 = 0;
    for(t=1;t<size;t++) {
	if(data[t] != color1) {
	    color2 = data[t];
	    break;
	}
    }
    if(t==size)
	return 1;

    for(;t<size;t++) {
	if(data[t] != color1 && data[t] != color2) {
	    return width*height;
	}
    }
    return 2;
}

gfximage_t* gfximage_rescale_old(gfximage_t*image, int newwidth, int newheight)
{
    int x,y;
    gfxcolor_t* newdata; 
    scale_lookup_t *p, **lblockx,**lblocky;
    rgba_int_t*tmpline;
    int monochrome = 0;
    gfxcolor_t monochrome_colors[2];
   
    if(newwidth<1)
	newwidth=1;
    if(newheight<1)
	newheight=1;

    int width = image->width;
    int height = image->height;
    gfxcolor_t*data = image->data;

    if(gfximage_getNumberOfPaletteEntries(image) == 2) {
	monochrome=1;
	encodeMonochromeImage(data, width, height, monochrome_colors);
        int r1 = width / newwidth;
        int r2 = height / newheight;
        int r = r1<r2?r1:r2;
        if(r>4) {
            /* high-resolution monochrome images are usually dithered, so 
               low-pass filter them first to get rid of any moire patterns */
            blurImage(data, width, height, r+1);
        }
    }

    tmpline = (rgba_int_t*)rfx_alloc(width*sizeof(rgba_int_t));
    newdata = (gfxcolor_t*)rfx_alloc(newwidth*newheight*sizeof(gfxcolor_t));
  
    lblockx = make_scale_lookup(width, newwidth);
    lblocky = make_scale_lookup(height, newheight);

    for(p=lblocky[0];p<lblocky[newheight];p++)
	p->pos*=width;

    for(y=0;y<newheight;y++) {
	gfxcolor_t*destline = &newdata[y*newwidth];
	
	/* create lookup table for y */
	rgba_int_t*l = tmpline;
	scale_lookup_t*p_y,*p_x;
	memset(tmpline, 0, width*sizeof(rgba_int_t));
	for(p_y=lblocky[y];p_y<lblocky[y+1];p_y++) {
	    gfxcolor_t*line = &data[p_y->pos];
	    scale_lookup_t*p_x;
	    int weight = p_y->weight;
	    for(x=0;x<width;x++) {
		tmpline[x].r += line[x].r*weight;
		tmpline[x].g += line[x].g*weight;
		tmpline[x].b += line[x].b*weight;
		tmpline[x].a += line[x].a*weight;
	    }
	}

	/* process x direction */
	p_x = lblockx[0];
	for(x=0;x<newwidth;x++) {
	    unsigned int r=0,g=0,b=0,a=0;
	    scale_lookup_t*p_x_to = lblockx[x+1];
	    do { 
		rgba_int_t* col = &tmpline[p_x->pos];
		unsigned int weight = p_x->weight;
		r += col->r*weight;
		g += col->g*weight;
		b += col->b*weight;
		a += col->a*weight;
		p_x++;
	    } while (p_x<p_x_to);

	    destline->r = r >> 16;
	    destline->g = g >> 16;
	    destline->b = b >> 16;
	    destline->a = a >> 16;
	   
	    destline++;
	}
    }

    if(monochrome)
	decodeMonochromeImage(newdata, newwidth, newheight, monochrome_colors);

    rfx_free(tmpline);
    rfx_free(*lblockx);
    rfx_free(lblockx);
    rfx_free(*lblocky);
    rfx_free(lblocky);

    gfximage_t*image2 = (gfximage_t*)malloc(sizeof(gfximage_t));
    image2->data = newdata;
    image2->width = newwidth;
    image2->height = newheight;
    return image2;
}

#ifdef HAVE_FFTW3
gfximage_t* gfximage_rescale_fft(gfximage_t*image, int newwidth, int newheight)
{
    int channel;

    int oldwidth = image->width;
    int oldheight = image->height;

    unsigned char*rgba = (unsigned char*)image->data;

    bool has_alpha = gfximage_has_alpha(image);
    bool monochrome = 0;
    gfxcolor_t monochrome_colors[2];
    if(gfximage_getNumberOfPaletteEntries(image) == 2) {
	monochrome=1;
	encodeMonochromeImage(image->data, image->width, image->height, monochrome_colors);
    }
    
    float*data = fftwf_malloc(sizeof(float)*oldwidth*oldheight);
    int osize = oldwidth*oldheight;
    int nsize = newwidth*newheight;

    assert(newwidth <= oldwidth && newheight <= oldheight);

    gfxcolor_t*rgba_new = malloc(newwidth*newheight*sizeof(gfxcolor_t));
    unsigned char*rgba_new_asbytes = (unsigned char*)rgba_new;

    int oxwidth = oldwidth/2+1;
    int oxsize = oxwidth*oldheight;
    fftwf_complex* fft = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*oxwidth*oldheight);
    fftwf_complex* fft2 = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*newwidth*newheight);
    fftwf_complex* data2 = fftwf_malloc(sizeof(fftwf_complex)*newwidth*newheight);
    fftwf_plan plan1 = fftwf_plan_dft_r2c_2d(oldheight, oldwidth, data, fft, FFTW_ESTIMATE);
    fftwf_plan plan2 = fftwf_plan_dft_2d(newheight, newwidth, fft2, data2, FFTW_BACKWARD, FFTW_ESTIMATE);

    double ff = 1.0/osize;

    for(channel=0;channel<4;channel++) {
	if(channel==0 && !has_alpha)
	    continue;
	if(channel>=1 && monochrome)
	    continue;
	int t;
	for(t=0;t<osize;t++) {
	    data[t] = rgba[t*4+channel];
	}
	fftwf_execute(plan1);

	int x,y;
	int width2 = newwidth >> 2;
	for(y=0;y<newheight;y++) {
	    fftwf_complex*from,*to;
	    
	    int fromy = (oldheight-MOD(y-newheight/2,oldheight))%oldheight;
	    from = &fft[fromy*oxwidth + 1];
	    to = &fft2[MOD(y-newheight/2,newheight)*newwidth+newwidth-width2];
	    int x;
	    for(x=0;x<width2;x++) {
		to[x][0] =  from[width2-x-1][0]*ff;
		to[x][1] = -from[width2-x-1][1]*ff;
	    }

	    from = &fft[MOD(y-newheight/2,oldheight)*oxwidth];
	    to = &fft2[MOD(y-newheight/2,newheight)*newwidth];
	    for(x=0;x<width2;x++) {
		to[x][0] = from[x][0]*ff;
		to[x][1] = from[x][1]*ff;
	    }
	}
	fftwf_execute(plan2);

	for(t=0;t<nsize;t++) {
	    int f = data2[t][0];
	    rgba_new_asbytes[t*4+channel] = f<0?0:(f>255?255:f);
	}
    }
    fftwf_destroy_plan(plan1);
    fftwf_destroy_plan(plan2);
    free(fft);
    free(fft2);
    if(!has_alpha) {
	int t;
	for(t=0;t<nsize;t++) {
	    rgba_new[t].a = 255;
	}
    }
    
    if(monochrome)
	decodeMonochromeImage(rgba_new, newwidth, newheight, monochrome_colors);

    gfximage_t*image2 = (gfximage_t*)malloc(sizeof(gfximage_t));
    image2->data = rgba_new;
    image2->width = newwidth;
    image2->height = newheight;
    return image2;
}
#endif

#ifdef HAVE_FFTW3
gfximage_t* gfximage_rescale(gfximage_t*image, int newwidth, int newheight)
{
    //return gfximage_rescale_fft(image, newwidth, newheight);
    return gfximage_rescale_old(image, newwidth, newheight);
}
#else
gfximage_t* gfximage_rescale(gfximage_t*image, int newwidth, int newheight)
{
    return gfximage_rescale_old(image, newwidth, newheight);
}
#endif

bool gfximage_has_alpha(gfximage_t*img)
{
    int size = img->width*img->height;
    gfxcolor_t*data = img->data;
    int t;
    for(t=0;t<size;t++) {
	if(data[t].a!=255) 
	    return 1;
    }
    return 0;
}

void gfximage_free(gfximage_t*b)
{
    free(b->data);
    b->data = 0;
    free(b);
}

