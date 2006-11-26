#include "../config.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"
#include "../lib/png.h"
#include "../lib/args.h"

static struct options_t options[] = {
{"h", "help"},
{"o", "output"},
{"V", "version"},
{0,0}
};

static char*filename = 0;
static char*outputname = "output.png";

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfrender - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } else if(!strcmp(name, "o")) {
	outputname = strdup(val);
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

    fi = open(filename, O_RDONLY|O_BINARY);
    if (fi<=0) { 
	fprintf(stderr,"Couldn't open %s\n", filename);
        perror(argv[1]);
        exit(1);
    }

    if(swf_ReadSWF(fi,&swf)<0) { 
	fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",argv[1]);
	close(fi);
    }

    RENDERBUF buf;
    swf_Render_Init(&buf, 0,0, (swf.movieSize.xmax - swf.movieSize.xmin) / 20,
	                       (swf.movieSize.ymax - swf.movieSize.ymin) / 20, 2, 1);

    swf_RenderSWF(&buf, &swf);

    RGBA* img = swf_Render(&buf);

    writePNG(outputname, (unsigned char*)img, buf.width, buf.height);

    swf_Render_Delete(&buf);
    return 0;
}

