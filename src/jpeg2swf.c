/* jpeg2swf.c

   JPEG to SWF converter tool

   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
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
#include <jpeglib.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"	// not really a header ;-)

#define MAX_INPUT_FILES 1024
#define VERBOSE(x) (global.verbose>=x)

static struct {
    int quality;
    float framerate;
    int max_image_width;
    int max_image_height;
    int force_width;
    int force_height;
    int nfiles;
    int verbose;
    char *outfile;
    int mx;
    int next_id;
    char *asset_name;
    int version;
    int fit_to_movie;
    float scale;
} global;

static int custom_move=0;
static int move_x=0;
static int move_y=0;
static int clip_x1=0,clip_y1=0,clip_x2=0,clip_y2=0;
static int custom_clip = 0;

typedef struct _image {
    char *filename;
    int quality;
    int width;
    int height;
} image_t;
image_t image[MAX_INPUT_FILES];

VIDEOSTREAM stream;

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
	swf->movieSize.xmin = 0;
	swf->movieSize.ymin = 0;
	swf->movieSize.xmax = swf->movieSize.xmin + dx * 20;
	swf->movieSize.ymax = swf->movieSize.ymin + dy * 20;
    }

    t = swf->firstTag = swf_InsertTag(NULL, ST_SETBACKGROUNDCOLOR);

    rgb.r = rgb.g = rgb.b = rgb.a = 0x00;
    swf_SetRGB(t, &rgb);

    if (global.mx) {
	t = swf_InsertTag(t, ST_DEFINEVIDEOSTREAM);
	swf_SetU16(t, 0xf00d);
	swf_SetVideoStreamDefine(t, &stream, 65535, dx, dy);
    } else if (global.asset_name) {
	t = swf_InsertTag(t, ST_DEFINESPRITE);
	swf_SetU16(t, 1);
	swf_SetU16(t, global.next_id++);
    }

    return t;
}

int MovieFinish(SWF * swf, TAG * t, char *sname)
{
    int handle, so = fileno(stdout);

    if (global.asset_name) {
	SWFPLACEOBJECT obj;

	t = swf_InsertTag(t, ST_END);
	t = swf_InsertTag(t, ST_EXPORTASSETS);
	swf_SetU16(t, 1);
	swf_SetU16(t, 1);
	swf_SetString(t, global.asset_name);

	t = swf_InsertTag(t, ST_PLACEOBJECT2);
	swf_GetPlaceObject(0, &obj);
	obj.depth = 1;
	obj.id = 1;
	swf_SetPlaceObject(t, &obj);

	t = swf_InsertTag(t, ST_SHOWFRAME);
    }

    t = swf_InsertTag(t, ST_END);

    if ((!isatty(so)) && (!sname))
	handle = so;
    else {
	if (!sname)
	    sname = "output.swf";
	handle = open(sname, O_BINARY | O_RDWR | O_CREAT | O_TRUNC, 0666);
    }
    if(handle<0 && sname) {
        perror(sname);
    }
    if (swf_WriteSWF(handle, swf)<0) 
        fprintf(stderr, "Unable to write output file: %s\n", sname);

    if (handle != so)
	close(handle);

    swf_FreeTags(swf);
    return 0;
}

int getJPEG(char*filename, int* width, int* height, RGBA**pic2)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    struct jpeg_source_mgr mgr;
    int x,y;
    FILE*f;
    RGBA*pic,*js;
    U8*buf;

    if ((f=fopen(filename,"rb"))==NULL) {
	fprintf(stderr, "rfxswf: file open error\n");
	return 0;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo); 
    jpeg_stdio_src(&cinfo, f);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    pic = malloc(cinfo.output_width*cinfo.output_height*sizeof(RGBA));
    buf = malloc(cinfo.output_width*4);
    memset(pic, 255, cinfo.output_width*cinfo.output_height*sizeof(RGBA));
    js = pic;

    *width = cinfo.output_width;
    *height = cinfo.output_height;
    
    for (y=0;y<cinfo.output_height;y++) {
	int x;
	jpeg_read_scanlines(&cinfo,&buf,1);

	if(cinfo.out_color_space == JCS_GRAYSCALE) {
	    for(x=0;x<cinfo.output_width;x++) {
		js[x].r = js[x].g = js[x].b = buf[x];
	    }
	} else if(cinfo.out_color_space == JCS_RGB) {
	    for (x=0;x<cinfo.output_width;x++)
	    { 
		js[x].r = buf[x*3+0];
		js[x].g = buf[x*3+1];
		js[x].b = buf[x*3+2];
	    }
	} else if(cinfo.out_color_space == JCS_YCCK) {
	    //FIXME
	    fprintf(stderr, "Error: Can't convert YCCK to RGB.\n");
	    return -1;
	} else if(cinfo.out_color_space == JCS_YCbCr) {
	    for(x=0;x<cinfo.output_width;x++) {
		int y = buf[x*3+0];
		int u = buf[x*3+1];
		int v = buf[x*3+1];
		js[x].r = y + ((360*(v-128))>>8);
		js[x].g = y - ((88*(u-128)+183*(v-128))>>8);
		js[x].b = y + ((455 * (u-128))>>8);
	    }
	}
	else if(cinfo.out_color_space == JCS_CMYK) 
	{ 
	    for(x=0;x<cinfo.output_width;x++) {
		  int white = 255 - buf[x*4+3];
		  js[x].r = white - ((buf[x*4]*white)>>8);
		  js[x].g = white - ((buf[x*4+1]*white)>>8);
		  js[x].b = white - ((buf[x*4+2]*white)>>8);
	    }
	}
	js += cinfo.output_width;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    
    free(buf);
    *pic2 = pic;
    return 1;
}


int frame = 0;
TAG *MovieAddFrame(SWF * swf, TAG * t, char *sname, int quality, 
		   int width, int height)
{
    SHAPE *s;
    SRECT r;
    MATRIX m;
    int fs;
    int movie_width = swf->movieSize.xmax - swf->movieSize.xmin;
    int movie_height = swf->movieSize.ymax - swf->movieSize.ymin;

    if(global.mx) {
	int sizex, sizey;
	RGBA * pic2;
	SWFPLACEOBJECT obj;
	int quant=0;
	getJPEG(sname, &sizex, &sizey, &pic2);
	if(sizex != stream.owidth || sizey != stream.oheight) {
	    fprintf(stderr, "All images must have the same dimensions if using -m!");
	    exit(1);
	}

	t = swf_InsertTag(t, ST_VIDEOFRAME);
	swf_SetU16(t, 0xf00d);
	quant = 1+(30-(30*quality)/100);
	if(!(frame%20)) {
	    swf_SetVideoStreamIFrame(t, &stream, pic2, quant);
	} else {
	    swf_SetVideoStreamPFrame(t, &stream, pic2, quant);
	}

	t = swf_InsertTag(t, ST_PLACEOBJECT2);
	swf_GetPlaceObject(0, &obj);
	if(frame==0) {
	    obj.depth = 1;
	    obj.id = 0xf00d;
	} else {
	    obj.depth = 1;
	    obj.move = 1;
	    obj.ratio = frame;
	}
	swf_SetPlaceObject(t,&obj);

	t = swf_InsertTag(t, ST_SHOWFRAME);
    } else {
	t = swf_InsertTag(t, ST_DEFINEBITSJPEG2);
	swf_SetU16(t, global.next_id);		// id
	swf_SetJPEGBits(t,sname,quality);

	t = swf_InsertTag(t, ST_DEFINESHAPE);
	swf_ShapeNew(&s);
	swf_GetMatrix(NULL, &m);
	if (global.fit_to_movie) {
	    m.sx = 0x10000 * movie_width / width;
	    m.sy = 0x10000 * movie_height / height;
	    width = movie_width / 20;
	    height = movie_height / 20;
	} else {
	    m.sx = 20 * 0x10000;
	    m.sy = 20 * 0x10000;
	}
	m.tx = 0;
	m.ty = 0;
	fs = swf_ShapeAddBitmapFillStyle(s, &m, global.next_id, 1);
	global.next_id++;
	swf_SetU16(t, global.next_id);	// id
	r.xmin = 0;
	r.ymin = 0;
	r.xmax = r.xmin + width * 20;
	r.ymax = r.ymin + height * 20;
	swf_SetRect(t, &r);
	swf_SetShapeHeader(t, s);
	swf_ShapeSetAll(t, s, r.xmin, r.ymin, 0, fs, 0);
	swf_ShapeSetLine(t, s, r.xmax - r.xmin, 0);
	swf_ShapeSetLine(t, s, 0, r.ymax - r.ymin);
	swf_ShapeSetLine(t, s, -r.xmax + r.xmin, 0);
	swf_ShapeSetLine(t, s, 0, -r.ymax + r.ymin);
	swf_ShapeSetEnd(t);

	if(frame) {
	    t = swf_InsertTag(t, ST_REMOVEOBJECT2);
	    swf_SetU16(t, 1);		// depth
	}

	t = swf_InsertTag(t, ST_PLACEOBJECT2);
	swf_GetMatrix(NULL, &m);
	m.sx = (int)(0x10000 * global.scale);
	m.sy = (int)(0x10000 * global.scale);

	if(custom_move) {
	    m.tx = move_x*20;
	    m.ty = move_y*20;
	} else {
	    m.tx = (movie_width - (width * global.scale * 20)) / 2;
	    m.ty = (movie_height - (height * global.scale * 20)) / 2;
	}
	swf_ObjectPlace(t, global.next_id, 1, &m, NULL, NULL);
	global.next_id++;
	t = swf_InsertTag(t, ST_SHOWFRAME);
    }
    frame++;

    return t;
}

int CheckInputFile(image_t* i, char *fname, char **realname)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *f;
    char *s = malloc(strlen(fname) + 5);
    int width, height;

    if (!s)
	exit(2);
    (*realname) = s;
    strcpy(s, fname);

    // Check whether file exists (with typical extensions)

    if ((f = fopen(s, "rb")) == NULL) {
	sprintf(s, "%s.jpg", fname);
	if ((f = fopen(s, "rb")) == NULL) {
	    sprintf(s, "%s.jpeg", fname);
	    if ((f = fopen(s, "rb")) == NULL) {
		sprintf(s, "%s.JPG", fname);
		if ((f = fopen(s, "rb")) == NULL) {
		    sprintf(s, "%s.JPEG", fname);
		    if ((f = fopen(s, "rb")) == NULL)
			return -1;
		}
	    }
	}
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, f);
    jpeg_read_header(&cinfo, TRUE);

    width = cinfo.image_width;
    height = cinfo.image_height;

    i->width = width;
    i->height = height;

    // Get image dimensions

    if (global.max_image_width < width)
	global.max_image_width = width;
    if (global.max_image_height < height)
	global.max_image_height = height;

    jpeg_destroy_decompress(&cinfo);
    fclose(f);

    return 0;
}

int args_callback_option(char *arg, char *val)
{
    int res = 0;
    if (arg[1])
	res = -1;
    else
	switch (arg[0]) {
	case 'q':
	    if (val)
		global.quality = atoi(val);
	    if ((global.quality < 1) ||(global.quality > 100)) {
		if (VERBOSE(1))
		    fprintf(stderr,
			    "Error: You must specify a valid quality between 1 and 100.\n");
		exit(1);
	    }
	    res = 1;
	    break;

	case 'r':
	    if (val)
		global.framerate = atof(val);
	    if ((global.framerate < 1.0/256) || (global.framerate >= 256.0)) {
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
	    global.verbose++;
	    res = 0;
	    break;

/*	case 'q':
	    global.verbose--;
	    if(global.verbose<0)
		global.verbose = 0;
	    res = 0;
	    break;*/

	case 'X':
	    if (val)
		global.force_width = atoi(val);
	    res = 1;
	    break;

	case 'z':
	    global.version = 6;
	    return 0;

	case 'Y':
	    if (val)
		global.force_height = atoi(val);
	    res = 1;
	    break;

	case 'V':
	    printf("jpeg2swf - part of %s %s\n", PACKAGE, VERSION);
	    exit(0);

	case 'e':
	    if (val)
		global.asset_name = val;
	    res = 1;
	    break;
	
	case 'T':
	    global.version = atoi(val);
	    res = 1;
	    break;

	case 'f':
	    global.fit_to_movie = 1;
	    res = 0;
	    break;
	
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

	case 'M': {
	    global.mx = 1;
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
	
	case 's': {
	    global.scale = atof(val)/100;
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
{"o", "output"},
{"q", "quality"},
{"r", "rate"},
{"z", "zlib"},
{"M", "mx"},
{"x", "xoffset"},
{"y", "yoffset"},
{"X", "width"},
{"Y", "height"},
{"T", "flashversion"},
{"v", "verbose"},
{"V", "version"},
{"f", "fit-to-movie"},
{"e", "export"},
{0,0}
};

int args_callback_longoption(char *name, char *val)
{
    return args_long2shortoption(options, name, val);
}

int args_callback_command(char *arg, char *next)	// actually used as filename
{
    char *s;
    image_t* i = &image[global.nfiles];
    if (CheckInputFile(i, arg, &s) < 0) {
	if (VERBOSE(1))
	    fprintf(stderr, "Unable to open input file: %s\n", arg);
	free(s);
    } else {
	i->filename = s;
	i->quality = global.quality;
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
    printf("Usage: %s [-options [value]] imagefiles[.jpg]|[.jpeg] [...]\n", name);
    printf("\n");
    printf("-o , --output <outputfile>     Explicitly specify output file. (otherwise, output.swf will be used)\n");
    printf("-q , --quality <quality>       Set compression quality (1-100, 1=worst, 100=best)\n");
    printf("-r , --rate <framerate>        Set movie framerate (frames per second)\n");
    printf("-z , --zlib <zlib>             Enable Flash 6 (MX) Zlib Compression\n");
    printf("-M , --mx                      Use Flash MX H.263 compression (use for correlated images)\n");
    printf("-x , --xoffset <offset>        horizontally offset images by <offset>\n");
    printf("-y , --yoffset <offset>        vertically offset images by <offset>\n");
    printf("-X , --width <width>           Force movie width to <width> (default: autodetect)\n");
    printf("-Y , --height <height>         Force movie height to <height> (default: autodetect)\n");
    printf("-T , --flashversion <version>      Set flash file version to <version>\n");
    printf("-v , --verbose <level>         Set verbose level to <level> (0=quiet, 1=default, 2=debug)\n");
    printf("-V , --version                 Print version information and exit\n");
    printf("-f , --fit-to-movie            Fit images to movie size\n");
    printf("-e , --export <assetname>          Make importable as asset with <assetname>\n");
    printf("\n");
}


int main(int argc, char **argv)
{
    SWF swf;
    TAG *t;

    memset(&global, 0x00, sizeof(global));

    global.quality = 60;
    global.framerate = 1.0;
    global.verbose = 1;
    global.version = 4;
    global.asset_name = NULL;
    global.next_id = 1;
    global.fit_to_movie = 0;
    global.scale = 1.0;
	
    processargs(argc, argv);

    if (VERBOSE(2))
	fprintf(stderr, "Processing %i file(s)...\n", global.nfiles);

    t = MovieStart(&swf, global.framerate,
		   global.force_width ? global.force_width : (int)(global.max_image_width*global.scale),
		   global.force_height ? global.force_height : (int)(global.max_image_height*global.scale));

    {
	int i;
	for (i = 0; i < global.nfiles; i++) {
	    if (VERBOSE(3))
		fprintf(stderr, "[%03i] %s (%i%%)\n", i,
			image[i].filename, image[i].quality);
	    t = MovieAddFrame(&swf, t, image[i].filename, image[i].quality,
			      image[i].width, image[i].height);
	    free(image[i].filename);
	}
    }

    MovieFinish(&swf, t, global.outfile);

    return 0;
}
