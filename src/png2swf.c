/* png2swf.c

   PNG to SWF converter tool

   Part of the swftools package.

   Copyright (c) 2002 Matthias Kramm <kramm@quiss.org>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <zlib.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"

#define MAX_INPUT_FILES 1024
#define VERBOSE(x) (global.verbose>=x)

struct {
    int framerate;
    int max_image_width;
    int max_image_height;
    int force_width;
    int force_height;
    int nfiles;
    int verbose;
    char *outfile;
} global;

struct {
    char *filename;
} image[MAX_INPUT_FILES];

TAG *MovieStart(SWF * swf, int framerate, int dx, int dy)
{
    TAG *t;
    RGBA rgb;

    memset(swf, 0x00, sizeof(SWF));

    swf->fileVersion = 4;
    swf->frameRate = (25600 / framerate);
    swf->movieSize.xmax = dx * 20;
    swf->movieSize.ymax = dy * 20;

    t = swf->firstTag = swf_InsertTag(NULL, ST_SETBACKGROUNDCOLOR);

    rgb.r = rgb.g = rgb.b = rgb.a = 0x00;
    swf_SetRGB(t, &rgb);

    return t;
}

int MovieFinish(SWF * swf, TAG * t, char *sname)
{
    int handle, so = fileno(stdout);
    t = swf_InsertTag(t, ST_END);

    if ((!isatty(so)) && (!sname))
	handle = so;
    else {
	if (!sname)
	    sname = "output.swf";
	handle = open(sname, O_RDWR | O_CREAT | O_TRUNC, 0666);
    }
    if FAILED
	(swf_WriteSWF(handle, swf)) if (VERBOSE(1))
	    fprintf(stderr, "Unable to write output file: %s\n", sname);
    if (handle != so)
	close(handle);

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
    len = REVERSESWAP32(len);
    printf("id: %.4s len: %d\n", head, len);
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
    return REVERSESWAP32(a);
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
    U8 head[8] = {137,80,78,71,13,10,26,10};
    U8 head2[8];
    U8*data;
    fread(head2,8,1,fi);
    if(strncmp(head,head2,4))
	return 0;
   
    while(png_read_chunk(&id, &len, &data, fi))
    {
	printf("%c%c%c%c %d\n", id[0],id[1],id[2],id[3],len);
	if(!strncasecmp(id, "IHDR", 4)) {
	    char a,b,c,f,i;
	    if(len < 8) exit(1);
	    header->width = REVERSESWAP32(*(U32*)&data[0]);
	    header->height = REVERSESWAP32(*(U32*)&data[4]);
	    a = data[8];      // should be 8
	    b = data[9];      // should be 3(indexed) or 2(rgb)

	    c = data[10];     // compression mode (0)
	    f = data[11];     // filter mode (0)
	    i = data[12];     // interlace mode (0)
	    printf("%08xx%08x %d %d %d %d %d\n",header->width, header->height, a,b,c,f,i);
	    header->bpp = a;
	    header->mode = b;
	    return 1;
	} else {
	    fseek(fi, len, SEEK_CUR);
	}
	free(data);
    }
    return 0;
}

TAG *MovieAddFrame(SWF * swf, TAG * t, char *sname, int id)
{
    SHAPE *s;
    SRECT r;
    MATRIX m;
    int fs;

    char tagid[4];
    int len;
    U8*data;
    U8*imagedata;
    unsigned long int imagedatalen;
    U8*palette = 0;
    int palettelen = 0;
    struct png_header header;
    int bypp;

    FILE *fi;
    U8 *scanline;

    if ((fi = fopen(sname, "rb")) == NULL) {
	if (VERBOSE(1))
	    fprintf(stderr, "Read access failed: %s\n", sname);
	return t;
    }

    if(!png_read_header(fi, &header))
	return 0;

    if(header.mode == 3) bypp = 1;
    else
    if(header.mode == 2) bypp = 3;
    else
	return 0;
    imagedatalen = bypp * header.width * header.height + 65536;
    imagedata = malloc(imagedatalen);

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
	    data = 0;
	    printf("%d palette\n", len);
	}
	if(!strncmp(tagid, "IDAT", 4)) {
	    if(uncompress(imagedata, &imagedatalen, data, len) != Z_OK) {
		fprintf(stderr, "Couldn't uncompress %s!\n", sname);
		return 0;
	    }
	    printf("IDAT %d -> %d\n", len, imagedatalen);
	}
	if(data)
	    free(data);
    }

    t = swf_InsertTag(t, ST_DEFINEBITSLOSSLESS);
    swf_SetU16(t, id);		// id
    if(header.mode == 2) {
	U8*data2 = malloc(header.width*header.height*4);
	int i,s=0;
	int x,y;
	int pos=0;
	/* 24->32 bit conversion */
	for(y=0;y<header.height;y++) {
	    pos++; // filter mode
	    for(x=0;x<header.width;x++) {
		U8*src  = &imagedata[pos];
		U8*dest = &data2[(y*header.width+x)*4];
		dest[0] = 255;
		dest[1] = src[0];
		dest[2] = src[1];
		dest[3] = src[2];
		pos+=3;
	    }
	}
	swf_SetLosslessBits(t, header.width, header.height, data2, BMF_32BIT);
	free(data2);
    }
    else {
	RGBA*rgba = (RGBA*)malloc(palettelen*sizeof(RGBA));
	U8*data2 = malloc((header.width+4)*header.height);
	int i,x,y;
	int pos=0;
	if(!palette) {
	    fprintf(stderr, "Error: No palette found!\n");
	    exit(1);
	}
	/* 24->32 bit conversion */
	for(i=0;i<palettelen;i++) {
	    rgba[i].r = palette[i*3+0];
	    rgba[i].g = palette[i*3+1];
	    rgba[i].b = palette[i*3+2];
	    rgba[i].a = 255;
	}
	for(y=0;y<header.height;y++) {
	    pos++; //filter mode
	    for(x=0;x<header.width;x++) {
		data2[y*header.width+x] = 
		    imagedata[pos++];
	    }
	}
	swf_SetLosslessBitsIndexed(t, header.width, header.height, data2, rgba, palettelen);
	free(rgba);
	free(data2);
    }

    t = swf_InsertTag(t, ST_DEFINESHAPE);

    swf_ShapeNew(&s);
    swf_GetMatrix(NULL, &m);
    m.sx = 20 * 0x10000;
    m.sy = 20 * 0x10000;
    fs = swf_ShapeAddBitmapFillStyle(s, &m, id, 0);

    swf_SetU16(t, id + 1);	// id

    r.xmin = r.ymin = 0;
    r.xmax = header.width * 20;
    r.ymax = header.height * 20;
    swf_SetRect(t, &r);

    swf_SetShapeHeader(t, s);

    swf_ShapeSetAll(t, s, 0, 0, 0, fs, 0);
    swf_ShapeSetLine(t, s, r.xmax, 0);
    swf_ShapeSetLine(t, s, 0, r.ymax);
    swf_ShapeSetLine(t, s, -r.xmax, 0);
    swf_ShapeSetLine(t, s, 0, -r.ymax);

    swf_ShapeSetEnd(t);

    t = swf_InsertTag(t, ST_REMOVEOBJECT2);
    swf_SetU16(t, 1);		// depth

    t = swf_InsertTag(t, ST_PLACEOBJECT2);

    swf_GetMatrix(NULL, &m);
    m.tx = (swf->movieSize.xmax - (int) header.width * 20) / 2;
    m.ty = (swf->movieSize.ymax - (int) header.height * 20) / 2;
    swf_ObjectPlace(t, id + 1, 1, &m, NULL, NULL);

    t = swf_InsertTag(t, ST_SHOWFRAME);

    fclose(fi);

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
		if ((fi = fopen(s, "rb")) == NULL)
		    fprintf(stderr, "Couldn't open %s!\n", fname);
		    return -1;
	    }
	}
    }

    if(!png_read_header(fi, &head)) {
	fprintf(stderr, "%s is not a PNG file!\n", fname);
	return -1;
    }
    if(head.mode!=2 && head.mode!=3) {
	fprintf(stderr, "%s has unsupported mode %d\n", head.mode);
	return -1;
    }
    if(head.bpp!=8) {
	fprintf(stderr, "%s has unsupported bpp %d\n", head.bpp);
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
		global.framerate = atoi(val);
	    if ((global.framerate < 1) ||(global.framerate > 5000)) {
		if (VERBOSE(1))
		    fprintf(stderr,
			    "Error: You must specify a valid framerate between 1 and 10000.\n");
		exit(1);
	    }
	    res = 1;
	    break;

	case 'o':
	    if (val)
		global.outfile = val;
	    res = 1;
	    break;

	case 'v':
	    if (val)
		global.verbose = atoi(val);
	    res = 1;
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

struct options_t options[] = 
{ 
{"o", "output"},
{"r", "rate"},
{"v", "verbose"},
{"X", "width"},
{"Y", "height"},
{"V", "version"},
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
    printf("Usage: %s  [-options [value]] imagefiles[.png] [...]\n", name);
    printf("-r framerate          (rate) Set movie framerate (100/sec)\n");
    printf("-o outputfile         (output) Set name for SWF output file\n");
    printf("-X pixel              (width) Force movie width to pixel (default: autodetect)\n");
    printf("-Y pixel              (height) Force movie height to pixel (default: autodetect)\n");
    printf("-v level              (verbose) Set verbose level (0=quiet, 1=default, 2=debug)\n");
    printf("-V                    (version) Print version information and exit\n");
}


int main(int argc, char **argv)
{
    SWF swf;
    TAG *t;

    memset(&global, 0x00, sizeof(global));

    global.framerate = 100;
    global.verbose = 1;

    processargs(argc, argv);

    if (VERBOSE(2))
	fprintf(stderr, "Processing %i file(s)...\n", global.nfiles);

    t = MovieStart(&swf, global.framerate,
		   global.force_width ? global.force_width : global.
		   max_image_width,
		   global.force_height ? global.force_height : global.
		   max_image_height);

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
