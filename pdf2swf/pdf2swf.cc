/* pdf2swf.cc
   main routine for pdf2swf(1)

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include "../config.h"
#include "../lib/args.h"
#include "pdfswf.h"
#include "t1lib.h"
extern "C" {
#include "log.h"
}

static char * outputname = 0;
static int loglevel = 3;
static char * pagerange = 0;
static char * filename = 0;
static char * password = 0;

static char * preloader = 0;
static char * viewer = 0;

int systemf(const char* format, ...)
{
    char buf[1024];
    va_list arglist;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);

    printf("%s:\n\n", buf);
    fflush(stdout);
    return system(buf);
}

int args_callback_option(char*name,char*val) {
    if (!strcmp(name, "o"))
    {
	outputname = val;
	return 1;
    }
    else if (!strcmp(name, "v"))
    {
	loglevel ++;
	return 0;
    }
    else if (name[0]=='p')
    {
	/* check whether the page range follows the p directly, like 
	   in -p1,2 */
	do {
	    name++;
	} while(*name == 32 || *name == 13 || *name == 10 || *name == '\t');

	if(*name) {
	    pagerange = name;
	    return 0;
	} 
	pagerange = val;	
	return 1;
    }
    else if (!strcmp(name, "P"))
    {
	password = val;
	return 1;
    }
    else if (!strcmp(name, "s"))
    {
	pdfswf_drawonlyshapes();
	return 0;
    }
    else if (!strcmp(name, "i"))
    {
	pdfswf_ignoredraworder();
	return 0;
    }
    else if (!strcmp(name, "n"))
    {
	pdfswf_linksopennewwindow();
	return 0;
    }
    else if (!strcmp(name, "f"))
    {
	pdfswf_storeallcharacters();
	return 0;
    }
    else if (!strcmp(name, "l"))
    {
	if(val)
	{
	    preloader = val;
	}
	else
	{
	    systemf("ls %s/swfs/*_loader.swf", DATADIR);
	    printf("\n");
	    exit(1);
	}
	return 1;
    }
    else if (!strcmp(name, "b"))
    {
	if(val)
	{
	    viewer = val;
	}
	else
	{
	    systemf("ls %s/swfs/*_viewer.swf", DATADIR);
	    printf("\n");
	    exit(1);
	}
	return 1;
    }
    else if (name[0]=='j')
    {
	if(name[1]) {
	    pdfswf_jpegquality(atoi(&name[1]));
	    return 0;
	} else {
	    pdfswf_jpegquality(atoi(val));
	    return 1;
	}
    }
    else if (!strcmp(name, "V"))
    {	
	printf("pdf2swf - part of %s %s\n", PACKAGE, VERSION);
	exit(0);
    }
    else 
    {
	fprintf(stderr, "Unknown option: -%s\n", name);
	exit(1);
    }
    return 0;
}

struct options_t options[] =
{{"o","output"},
 {"V","version"},
 {"i","ignore"},
 {"s","shapes"},
 {"j","jpegquality"},
 {"p","pages"},
 {"w","samewindow"},
 {"f","fonts"},
 {"b","viewer"},
 {"l","preloader"},
 {0,0}
};

int args_callback_longoption(char*name,char*val) {
    return args_long2shortoption(options, name, val);
}

int args_callback_command(char*name, char*val) {
    if (!filename) 
        filename = name;
    else {
	if(outputname)
	{
	     fprintf(stderr, "Error: Do you want the output to go to %s or to %s?", 
		     outputname, name);
	     exit(1);
	}
	outputname = name;
    }
    return 0;
}

void args_callback_usage(char*name)
{
    printf("Usage: %s [-si] [-j quality] [-p range] [-P password] input.pdf [output.swf]\n", name);
    printf("\n");
    printf("-p  --pages=range          Convert only pages in range\n");
    printf("-P  --password=password    Use password for deciphering the pdf\n");
    printf("-s  --shapes               Don't use SWF Fonts, but store everything as shape\n");
    printf("-i  --ignore               Ignore draw order (makes the SWF file smaller)\n");
    printf("-j  --jpegquality=quality  Set quality of embedded jpeg pictures (default:85)\n");
    printf("-v  --verbose              Be verbose. Use more than one -v for greater effect\n");
    printf("-w  --samewindow           Don't open a new Browser Window for Links in the SWF\n");
    printf("-f  --fonts                Store full fonts in SWF. (Don't reduce to used characters)\n");
    printf("-b  --viewer name	       Link viewer \"name\" to the pdf (\"%s -b\" for list)\n");
    printf("-l  --preloader name       Link preloader \"name\" to the pdf (\"%s -l\" for list)\n");
    printf("-V  --version              Print program version\n");
}

int main(int argn, char *argv[])
{
#ifdef HAVE_SRAND48
    srand48(time(0));
#else
#ifdef HAVE_SRAND
    srand(time(0));
#endif
#endif
    processargs(argn, argv);
    initLog(0,-1,0,0,-1,loglevel);
    if(!outputname)
    {
	fprintf(stderr, "Please use -o to specify an output file\n");
	exit(1);
    }

    // test if the page range is o.k.
    is_in_range(0x7fffffff, pagerange);

    if (!filename) {
	args_callback_usage(argv[0]);
	exit(0);
    }

    logf("<verbose> reading data files from %s\n", DATADIR);
    //TODO: use tempnam here. Check if environment already contains a
    //T1LIB_CONFIG.
    putenv( "T1LIB_CONFIG=/tmp/t1lib.config.tmp");
    FILE*fi = fopen("/tmp/t1lib.config.tmp", "wb");
    fprintf(fi, "FONTDATABASE=%s/fonts/FontDataBase\n", DATADIR);
    fprintf(fi, "ENCODING=%s/fonts:.\n", DATADIR);
    fprintf(fi, "AFM=%s/fonts:.\n", DATADIR);
    fprintf(fi, "TYPE1=%s/fonts:.\n", DATADIR);
    fclose(fi);
    /* initialize t1lib */
    T1_SetBitmapPad( 16);
    if ((T1_InitLib(NO_LOGFILE)==NULL)){
	fprintf(stderr, "Initialization of t1lib failed\n");
	exit(1);
    }
    unlink("/tmp/t1lib.config.tmp");

    pdfswf_init(filename, password);
    pdfswf_setoutputfilename(outputname);

    int pages = pdfswf_numpages();
    int t = 1;
    for(t = 1; t <= pages; t++) 
    {
	if(is_in_range(t, pagerange))
	pdfswf_convertpage(t);
    }
    pdfswf_performconversion();

    pdfswf_close();

    if(viewer) {
	systemf("swfcombine %s viewport=%s -o %s",
		viewer, outputname, outputname);
	printf("\n");
    }
    if(preloader) {
	systemf("swfcombine %s/swfs/PreLoaderTemplate.swf loader=%s movie=%s -o %s",
		DATADIR, preloader, outputname, outputname);
	printf("\n");
    }

    return 0;
}


