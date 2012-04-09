/* png2swf.c

   PNG to SWF converter tool

   Part of the swftools package.

   Copyright (c) 2002,2003 Matthias Kramm <kramm@quiss.org>
 
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
#include <math.h>
#include <fcntl.h>
#include <zlib.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "../lib/log.h"
#include "../lib/png.h"

#define MAX_INPUT_FILES 1024
#define VERBOSE(x) (global.verbose>=x)

static struct {
    float framerate;
    int max_image_width;
    int max_image_height;
    int force_width;
    int force_height;
    int nfiles;
    int verbose;
    int do_cgi;
    int version;
    char *outfile;
    int mkjpeg;
    float scale;
} global;

static struct {
    char *filename;
} image[MAX_INPUT_FILES];

static int custom_move=0;
static int move_x=0;
static int move_y=0;
static int clip_x1=0,clip_y1=0,clip_x2=0,clip_y2=0;
static int custom_clip = 0;

TAG *MovieStart(SWF * swf, float framerate, int dx, int dy)
{
    TAG *t;
    RGBA rgb;

    memset(swf, 0x00, sizeof(SWF));

    swf->fileVersion = global.version;
    swf->frameRate = (int)(256.0 * framerate);
    if(custom_clip) {
	swf->movieSize.xmin = clip_x1 * 20;
	swf->movieSize.ymin = clip_y1 * 20;
	swf->movieSize.xmax = clip_x2 * 20;
	swf->movieSize.ymax = clip_y2 * 20;
    } else {
	swf->movieSize.xmax = dx * 20;
	swf->movieSize.ymax = dy * 20;
    }

    t = swf->firstTag = swf_InsertTag(NULL, ST_SETBACKGROUNDCOLOR);

    rgb.r = rgb.g = rgb.b = rgb.a = 0x00;
    //rgb.g = 0xff; //<--- handy for testing alpha conversion
    swf_SetRGB(t, &rgb);

    return t;
}

int MovieFinish(SWF * swf, TAG * t, char *sname)
{
    int f, so = fileno(stdout);
    t = swf_InsertTag(t, ST_END);

    if ((!isatty(so)) && (!sname))
	f = so;
    else {
	if (!sname)
	    sname = "output.swf";
	f = open(sname,O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
    }

    if(global.do_cgi) {
	if FAILED(swf_WriteCGI(swf)) fprintf(stderr,"WriteCGI() failed.\n");
    } else {
        if (swf_WriteSWF(f, swf)<0) 
            fprintf(stderr, "Unable to write output file: %s\n", sname);
	if (f != so)
	    close(f);
    }

    swf_FreeTags(swf);
    return 0;
}

int png_read_chunk(char (*head)[4], int*destlen, U8**destdata, FILE*fi)
{
    unsigned int len;
    if(destlen) *destlen=0;
    if(destdata) *destdata=0;
    if(!fread(&len, 4, 1, fi))
	return 0;
    if(!fread(head, 4, 1, fi))
	return 0;
    len = BE_32_TO_NATIVE(len);
    if(destlen) *destlen = len;
    if(destdata) {
	if(len)
	    *destdata = malloc(len);
	else 
	    *destdata = 0;
	if(!fread(*destdata, len, 1, fi)) {
	    *destdata = 0;
	    if(destlen) *destlen=0;
	    return 0;
	}
	fseek(fi, 4, SEEK_CUR);

    } else {
	fseek(fi, len+4, SEEK_CUR);
    }
    return 1;
}

unsigned int png_get_dword(FILE*fi)
{
    unsigned int a;
    fread(&a,4,1,fi);
    return BE_32_TO_NATIVE(a);
}

struct png_header
{
    int width;
    int height;
    int bpp;
    int mode;
};

int png_read_header(FILE*fi, struct png_header*header)
{
    char id[4];
    int len;
    int ok=0;
    U8 head[8] = {137,80,78,71,13,10,26,10};
    U8 head2[8];
    U8*data;
    fread(head2,8,1,fi);
    if(strncmp((char*)head,(char*)head2,4))
	return 0;
   
    while(png_read_chunk(&id, &len, &data, fi))
    {
	if(VERBOSE(2))
	    printf("%c%c%c%c %d\n", id[0],id[1],id[2],id[3],len);
	if(!strncasecmp(id, "IHDR", 4)) {
	    char a,b,c,f,i;
	    if(len < 8) exit(1);
	    header->width = BE_32_TO_NATIVE(*(U32*)&data[0]);
	    header->height = BE_32_TO_NATIVE(*(U32*)&data[4]);
	    a = data[8];      // should be 8
	    b = data[9];      // should be 3(indexed), 2(rgb), 0(grayscale) or 6(truecolor+alpha)

	    c = data[10];     // compression mode (0)
	    f = data[11];     // filter mode (0)
	    i = data[12];     // interlace mode (0)
	
	    if(VERBOSE(2)) printf("image mode:%d\n", b);
	    if(VERBOSE(2)) printf("bpp: %d\n", a);
	    if(VERBOSE(2)) printf("compression: %d\n", c);
	    if(VERBOSE(2)) printf("filter: %d\n", f);
	    if(VERBOSE(2)) printf("interlace: %d\n", i);

	    if(b!=0 && b!=2 && b!=3 && b!=6) {
		fprintf(stderr, "Image mode %d not supported!\n", b);
		if(b == 4) {
		    fprintf(stderr, "(This is a grayscale image with alpha channel-\n");
		    fprintf(stderr, " try converting it into an RGB image with alpha channel)\n");
		}
		exit(1);
	    }
	    if(a!=8 && (b==2 || b==6)) {
		fprintf(stderr, "Bpp %d in mode %d not supported!\n", b, a);
		exit(1);
	    }
	    if(c!=0) {
		fprintf(stderr, "Compression mode %d not supported!\n", c);
		exit(1);
	    }
	    if(f!=0) {
		fprintf(stderr, "Filter mode %d not supported!\n", f);
		exit(1);
	    }
	    if(i!=0) {
		fprintf(stderr, "Interlace mode %d not supported!\n", i);
		exit(1);
	    }
	    if(VERBOSE(2))
		printf("%dx%d %d %d %d %d %d\n",header->width, header->height, a,b,c,f,i);
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
byte inline PaethPredictor (byte a,byte b,byte c)
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

void applyfilter3(int mode, U8*src, U8*old, U8*dest, int width)
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

void applyfilter4(int mode, U8*src, U8*old, U8*dest, int width)
{
    int x;
    unsigned char lastr=0;
    unsigned char lastg=0;
    unsigned char lastb=0;
    unsigned char lasta=0; //TODO: 255?
    unsigned char upperlastr=0;
    unsigned char upperlastg=0;
    unsigned char upperlastb=0;
    unsigned char upperlasta=0; //TODO: 255?

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

void applyfilter1(int mode, U8*src, U8*old, U8*dest, int width)
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

TAG *MovieAddFrame(SWF * swf, TAG * t, char *sname, int id)
{
    SHAPE *s;
    SRECT r;
    MATRIX m;
    int fs;

    unsigned width=0, height=0;

#ifndef HAVE_JPEGLIB
    if(global.mkjpeg) {
        global.mkjpeg = 0;
        msg("<warning> No jpeg support compiled in");
    }
#endif
    if(global.mkjpeg) {
#ifdef HAVE_JPEGLIB
	RGBA*data = 0;
	png_load(sname, &width, &height, (unsigned char**)&data);
	if(!data) 
	    exit(1);
	if(swf_ImageHasAlpha(data, width, height)) {
	    t = swf_InsertTag(t, ST_DEFINEBITSJPEG3);
	    swf_SetU16(t, id);
	    swf_SetJPEGBits3(t, width,height,data,global.mkjpeg);
	} else {
	    t = swf_InsertTag(t, ST_DEFINEBITSJPEG2);
	    swf_SetU16(t, id);
	    swf_SetJPEGBits2(t, width,height,data,global.mkjpeg);
	}
#endif
    } else {
	RGBA*data = 0;
	png_load(sname, &width, &height, (unsigned char**)&data);
	if(!data) 
	    exit(1);
	t = swf_InsertTag(t, ST_DEFINEBITSLOSSLESS);
	swf_SetU16(t, id);
	swf_SetLosslessImage(t, data,width,height);
    }

    t = swf_InsertTag(t, ST_DEFINESHAPE3);

    swf_ShapeNew(&s);
    swf_GetMatrix(NULL, &m);
    m.sx = (int)(20 * 0x10000);
    m.sy = (int)(20 * 0x10000);
    m.tx = 0;
    m.ty = 0;
    fs = swf_ShapeAddBitmapFillStyle(s, &m, id, 1);

    swf_SetU16(t, id + 1);	// id

    r.xmin = r.ymin = 0;
    r.xmax = width * 20;
    r.ymax = height * 20;
    swf_SetRect(t, &r);

    swf_SetShapeHeader(t, s);

    swf_ShapeSetAll(t, s, 0, 0, 0, fs, 0);
    swf_ShapeSetLine(t, s, r.xmax, 0);
    swf_ShapeSetLine(t, s, 0, r.ymax);
    swf_ShapeSetLine(t, s, -r.xmax, 0);
    swf_ShapeSetLine(t, s, 0, -r.ymax);

    swf_ShapeSetEnd(t);

    t = swf_InsertTag(t, ST_REMOVEOBJECT2);
    swf_SetU16(t, 50);		// depth

    t = swf_InsertTag(t, ST_PLACEOBJECT2);

    swf_GetMatrix(NULL, &m);
    m.sx = (int)(0x10000 * global.scale);
    m.sy = (int)(0x10000 * global.scale);

    if(custom_move) {
	m.tx = move_x*20;
	m.ty = move_y*20;
    } else {
	m.tx = (swf->movieSize.xmax - (int) (width * global.scale * 20)) / 2;
	m.ty = (swf->movieSize.ymax - (int) (height * global.scale * 20)) / 2;
    }
    swf_ObjectPlace(t, id + 1, 50, &m, NULL, NULL);

    t = swf_InsertTag(t, ST_SHOWFRAME);

    return t;
}


int CheckInputFile(char *fname, char **realname)
{
    FILE *fi;
    char *s = malloc(strlen(fname) + 5);
    struct png_header head;

    if (!s)
	exit(2);
    (*realname) = s;
    strcpy(s, fname);

    // Check whether file exists (with typical extensions)

    if ((fi = fopen(s, "rb")) == NULL) {
	sprintf(s, "%s.png", fname);
	if ((fi = fopen(s, "rb")) == NULL) {
	    sprintf(s, "%s.PNG", fname);
	    if ((fi = fopen(s, "rb")) == NULL) {
		sprintf(s, "%s.Png", fname);
		if ((fi = fopen(s, "rb")) == NULL) {
		    fprintf(stderr, "Couldn't open %s!\n", fname);
		    return -1;
		}
	    }
	}
    }

    if(!png_read_header(fi, &head)) {
	fprintf(stderr, "%s is not a PNG file!\n", fname);
	return -1;
    }

    if (global.max_image_width < head.width)
	global.max_image_width = head.width;
    if (global.max_image_height < head.height)
	global.max_image_height = head.height;

    fclose(fi);

    return 0;
}

int args_callback_option(char *arg, char *val)
{
    int res = 0;
    if (arg[1])
	res = -1;
    else
	switch (arg[0]) {
	case 'r':
	    if (val)
		global.framerate = atof(val);
	    /* removed framerate>0 restriction in order to make
	       Flash Communication Server compatible SWFs */
	    if ((global.framerate < 0) ||(global.framerate >= 256.0)) {
		if (VERBOSE(1))
		    fprintf(stderr,
			    "Error: You must specify a valid framerate between 1/256 and 255.\n");
		exit(1);
	    }
	    res = 1;
	    break;

	case 'o':
	    if (val)
		global.outfile = val;
	    res = 1;
	    break;

	case 's':
	    global.scale = atof(val)/100;
	    res = 1;
	    break;

	case 'z':
	    if(global.version<6)
		global.version = 6;
	    res = 0;
	    break;

	case 'j':
	    global.mkjpeg = atoi(val);
	    res = 1;
	    break;

	case 'T':
	    global.version = atoi(val);
	    res = 1;
	    break;

	case 'C':
	    global.do_cgi = 1;
	    break;

	case 'v':
	    global.verbose++;
	    res = 0;
	    break;

	case 'q':
	    global.verbose--;
	    if(global.verbose<0)
		global.verbose = 0;
	    res = 0;
	    break;

	case 'X':
	    if (val)
		global.force_width = atoi(val);
	    res = 1;
	    break;

	case 'Y':
	    if (val)
		global.force_height = atoi(val);
	    res = 1;
	    break;
	
	case 'V':
	    printf("png2swf - part of %s %s\n", PACKAGE, VERSION);
	    exit(0);
   
	case 'c': {
	    char*s = strdup(val);
	    char*x1 = strtok(s, ":");
	    char*y1 = strtok(0, ":");
	    char*x2 = strtok(0, ":");
	    char*y2 = strtok(0, ":");
	    if(!(x1 && y1 && x2 && y2)) {
		fprintf(stderr, "-m option requires four arguments, <x1>:<y1>:<x2>:<y2>\n");
		exit(1);
	    }
	    custom_clip = 1;
	    clip_x1 = atoi(x1);
	    clip_y1 = atoi(y1);
	    clip_x2 = atoi(x2);
	    clip_y2 = atoi(y2);
	    free(s);

	    res = 1;
	    break;
	}

	case 'm': {
	    char*s = strdup(val);
	    char*c = strchr(s, ':');
	    if(!c) {
		fprintf(stderr, "-m option requires two arguments, <x>:<y>\n");
		exit(1);
	    }
	    *c = 0;
	    custom_move = 1;
	    move_x = atoi(val);
	    move_y = atoi(c+1);
	    free(s);

	    res = 1;
	    break;
	}

	default:
	    res = -1;
	    break;
	}

    if (res < 0) {
	if (VERBOSE(1))
	    fprintf(stderr, "Unknown option: -%s\n", arg);
	exit(1);
	return 0;
    }
    return res;
}

static struct options_t options[] = {
{"r", "rate"},
{"o", "output"},
{"j", "jpeg"},
{"z", "zlib"},
{"T", "flashversion"},
{"X", "pixel"},
{"Y", "pixel"},
{"v", "verbose"},
{"q", "quiet"},
{"C", "cgi"},
{"V", "version"},
{"s", "scale"},
{0,0}
};

int args_callback_longoption(char *name, char *val)
{
    return args_long2shortoption(options, name, val);
}

int args_callback_command(char *arg, char *next)	// actually used as filename
{
    char *s;
    if (CheckInputFile(arg, &s) < 0) {
	if (VERBOSE(1))
	    fprintf(stderr, "Error opening input file: %s\n", arg);
	free(s);
    } else {
	image[global.nfiles].filename = s;
	global.nfiles++;
	if (global.nfiles >= MAX_INPUT_FILES) {
	    if (VERBOSE(1))
		fprintf(stderr, "Error: Too many input files.\n");
	    exit(1);
	}
    }
    return 0;
}

void args_callback_usage(char *name)
{
    printf("\n");
    printf("Usage: %s [-X width] [-Y height] [-o file.swf] [-r rate] file1.png [file2.png...]\n", name);
    printf("\n");
    printf("-r , --rate <framerate>        Set movie framerate (frames per second)\n");
    printf("-o , --output <filename>       Set name for SWF output file.\n");
    printf("-j , --jpeg <quality>          Generate a lossy jpeg bitmap inside the SWF, with a given quality (1-100)\n");
    printf("-z , --zlib <zlib>             Enable Flash 6 (MX) Zlib Compression\n");
    printf("-T , --flashversion            Set the flash version to generate\n");
    printf("-X , --pixel <width>           Force movie width to <width> (default: autodetect)\n");
    printf("-Y , --pixel <height>          Force movie height to <height> (default: autodetect)\n");
    printf("-v , --verbose <level>         Set verbose level (0=quiet, 1=default, 2=debug)\n");
    printf("-q , --quiet                   Omit normal log messages, only log errors\n");
    printf("-C , --cgi                     For use as CGI- prepend http header, write to stdout\n");
    printf("-V , --version                 Print version information and exit\n");
    printf("-s , --scale <percent>         Scale image to <percent>%% size.\n");
    printf("\n");
}

int main(int argc, char **argv)
{
    SWF swf;
    TAG *t;

    memset(&global, 0x00, sizeof(global));

    global.framerate = 1.0;
    global.verbose = 1;
    global.version = 8;
    global.scale = 1.0;

    processargs(argc, argv);
    
    if(global.nfiles<=0) {
	fprintf(stderr, "No png files found in arguments\n");
	return 1;
    }

    if (VERBOSE(2))
	fprintf(stderr, "Processing %i file(s)...\n", global.nfiles);

    t = MovieStart(&swf, global.framerate,
		   global.force_width ? global.force_width : (int)(global.max_image_width*global.scale),
		   global.force_height ? global.force_height : (int)(global.max_image_height*global.scale));

    {
	int i;
	for (i = 0; i < global.nfiles; i++) {
	    if (VERBOSE(3))
		fprintf(stderr, "[%03i] %s\n", i,
			image[i].filename);
	    t = MovieAddFrame(&swf, t, image[i].filename, (i * 2) + 1);
	    free(image[i].filename);
	}
    }

    MovieFinish(&swf, t, global.outfile);

    return 0;
}
