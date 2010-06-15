#include "../config.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"
#include "../lib/png.h"
#include "../lib/args.h"
#include "../lib/gfxsource.h"
#include "../lib/readers/swf.h"
#include "../lib/devices/render.h"
#include "../lib/devices/rescale.h"

static struct options_t options[] = {
{"h", "help"},
{"o", "output"},
{"p", "pages"},
{"l", "legacy"},
{"V", "version"},
{"X", "width"},
{"Y", "height"},
{0,0}
};

static int ng = 1;
static char*filename = 0;
static char*outputname = "output.png";
static int quantize = 0;
static char*pagerange = 0;

static int width = 0;
static int height = 0;

typedef struct _parameter {
    const char*name;
    const char*value;
    struct _parameter*next;
} parameter_t;

parameter_t*params = 0;

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfrender - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } else if(!strcmp(name, "o")) {
	outputname = strdup(val);
	return 1;
    } else if(!strcmp(name, "l")) {
	ng = 0;
	return 0;
    } else if(!strcmp(name, "q")) {
	quantize = 1;
	return 0;
    } else if(!strcmp(name, "p")) {
	pagerange = val;
	return 1;
    } else if(!strcmp(name, "s")) {
	char*s = strdup(val);
	char*c = strchr(s, '=');
	parameter_t*p = malloc(sizeof(parameter_t));
	p->next = params;
	params = p;
	if(c && *c && c[1])  {
	    *c = 0;
	    c++;
	    p->name = s;
	    p->value = s;
	} else {
	    p->name = s;
	    p->value = "1";
	}
	return 1;
    } else if(!strcmp(name, "X")) {
	width = atoi(val);
	return 1;
    } else if(!strcmp(name, "Y")) {
	height = atoi(val);
	return 1;
    } else {
        printf("Unknown option: -%s\n", name);
	exit(1);
    }

    return 0;
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char *name)
{
    printf("\n");
    printf("Usage: %s file.swf [-o output.png]\n", name);
    printf("\n");
    printf("-h , --help                    Print short help message and exit\n");
    printf("-l , --legacy                  Use old rendering framework\n");
    printf("-o , --output		   Output file (default: output.png)\n");
    printf("\n");
}
int args_callback_command(char*name,char*val)
{
    if(filename) {
        fprintf(stderr, "Only one file allowed. You supplied at least two. (%s and %s)\n",
                 filename, name);
    }
    filename = name;
    return 0;
}



int main(int argn, char*argv[])
{
    SWF swf;
    int fi;

    processargs(argn, argv);

    if(!filename) {
	fprintf(stderr, "You must supply a filename.\n");
	return 1;
    }

    if(!ng) {
	fi = open(filename, O_RDONLY|O_BINARY);
	if (fi<=0) { 
	    fprintf(stderr,"Couldn't open %s\n", filename);
	    perror(filename);
	    exit(1);
	}
	if(swf_ReadSWF(fi,&swf)<0) { 
	    fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",argv[1]);
	    close(fi);
	}
	assert(swf.movieSize.xmax > swf.movieSize.xmin && swf.movieSize.ymax > swf.movieSize.ymin);
	RENDERBUF buf;
	swf_Render_Init(&buf, 0,0, (swf.movieSize.xmax - swf.movieSize.xmin) / 20,
				   (swf.movieSize.ymax - swf.movieSize.ymin) / 20, 2, 1);
	swf_RenderSWF(&buf, &swf);
	RGBA* img = swf_Render(&buf);
        if(quantize)
	    png_write_palette_based_2(outputname, (unsigned char*)img, buf.width, buf.height);
        else
	    png_write(outputname, (unsigned char*)img, buf.width, buf.height);
	swf_Render_Delete(&buf);
    } else {
	parameter_t*p;

	gfxsource_t*src = gfxsource_swf_create();
	for(p=params;p;p=p->next) {
	    src->setparameter(src, p->name, p->value);
	}
	gfxdocument_t*doc = src->open(src, filename);
	for(p=params;p;p=p->next) {
	    doc->setparameter(doc, p->name, p->value);
	}
	if(!doc) {
	    fprintf(stderr,"Couldn't open %s\n", filename);
	    exit(1);
	}
	gfxdevice_t dev2,*dev=&dev2;
	gfxdevice_render_init(dev);
        dev->setparameter(dev, "antialise", "4");
        if(quantize) {
            dev->setparameter(dev, "palette", "1");
        }
	if(width || height) {
	    dev = gfxdevice_rescale_new(dev, width, height, 0);
	}
	for(p=params;p;p=p->next) {
	    dev->setparameter(dev, p->name, p->value);
	}

	int t;
	for(t=1;t<=doc->num_pages;t++) {
            if(!is_in_range(t, pagerange))
                continue;
	    gfxpage_t* page = doc->getpage(doc, t);
	    dev->startpage(dev, page->width, page->height);
	    page->render(page, dev);
	    dev->endpage(dev);
	    page->destroy(page);
	    break;
	}
	gfxresult_t*result = dev->finish(dev);
	if(result) {
	    if(result->save(result, outputname) < 0) {
		exit(1);
	    }
	    result->destroy(result);
	}
	doc->destroy(doc);
    }
    return 0;
}

