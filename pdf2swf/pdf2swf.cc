/* pdf2swf.cc
   main routine for pdf2swf(1)

   Part of the swftools package.
   
   Copyright (c) 2001,2002,2003 Matthias Kramm <kramm@quiss.org> 
 
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
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include "../config.h"
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include "../lib/args.h"
#include "../lib/os.h"
#include "SWFOutputDev.h"
#include "log.h"

#define FONTDIR concatPaths(getInstallationPath(), "fonts")
#define SWFDIR concatPaths(getInstallationPath(), "swfs")

static char * outputname = 0;
static int loglevel = 3;
static char * pagerange = 0;
static char * filename = 0;
static char * password = 0;
static int zlib = 0;

static char * preloader = 0;
static char * viewer = 0;
static int nup = 0;

char* fontpaths[256];
int fontpathpos = 0;

static int system_quiet=0;

int systemf(const char* format, ...)
{
    char buf[1024];
    int ret;
    va_list arglist;
    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);

    if(!system_quiet) {
	printf("%s\n", buf);
	fflush(stdout);
    }
    ret = system(buf);
    if(ret) {
	fprintf(stderr, "system() returned %d\n", ret);
	exit(ret);
    }
    return ret;
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
        setConsoleLogging(loglevel);
	return 0;
    }
    else if (!strcmp(name, "2"))
    {
        nup = 2;
	return 0;
    }
    else if (!strcmp(name, "4"))
    {
        nup = 4;
	return 0;
    }
    else if (!strcmp(name, "q"))
    {
	loglevel --;
        setConsoleLogging(loglevel);
	system_quiet = 1;
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
	char*s = strdup(val);
	char*c = strchr(s, '=');
	if(c && *c && c[1])  {
	    *c = 0;
	    c++;
	    pdfswf_setparameter(s,c);
	}
	else
	    pdfswf_setparameter(s,"1");
	return 1;
    }
    else if (!strcmp(name, "S"))
    {
	pdfswf_setparameter("drawonlyshapes", "1");
	return 0;
    }
    else if (!strcmp(name, "i"))
    {
	pdfswf_setparameter("ignoredraworder", "1");
	return 0;
    }
    else if (!strcmp(name, "z"))
    {
	pdfswf_setparameter("enablezlib", "1");
	zlib = 1;
	return 0;
    }
    else if (!strcmp(name, "n"))
    {
	pdfswf_setparameter("opennewwindow", "1");
	return 0;
    }
    else if (!strcmp(name, "t"))
    {
	pdfswf_setparameter("insertstop", "1");
	return 0;
    }
    else if (!strcmp(name, "T"))
    {
	if(!strcasecmp(val, "mx"))
	    pdfswf_setparameter("flashversion", "6");
	else
	    pdfswf_setparameter("flashversion", val);

	return 1;
    }
    else if (!strcmp(name, "f"))
    {
	pdfswf_setparameter("storeallcharacters", "1");
	return 0;
    }
    else if (!strcmp(name, "F"))
    {
	char *s = strdup(val);
	int l = strlen(s);
	while(l && s[l-1]=='/') {
	    s[l-1] = 0;
	    l--;
	}
	fontpaths[fontpathpos++] = s;
	return 1;
    }
    else if (!strcmp(name, "l"))
    {
	char buf[256];
	sprintf(buf, "%s/default_loader.swf", SWFDIR);
	preloader = strdup(buf);
	return 0;
    }
    else if (!strcmp(name, "b"))
    {
	char buf[256];
	sprintf(buf, "%s/default_viewer.swf", SWFDIR);
	viewer = strdup(buf);
	return 0;
    }
    else if (!strcmp(name, "L"))
    {
	if(val)
	{
	    preloader = val;
	}
	else
	{
	    systemf("ls %s/*_loader.swf", SWFDIR);
	    if(!system_quiet)
		printf("\n");
	    exit(1);
	}
	return 1;
    }
    else if (!strcmp(name, "B"))
    {
	if(val)
	{
	    viewer = val;
	}
	else
	{
	    systemf("ls %s/*_viewer.swf", SWFDIR);
	    if(!system_quiet)
		printf("\n");
	    exit(1);
	}
	return 1;
    }
    else if (!strcmp(name, "j"))
    {
	if(name[1]) {
	    pdfswf_setparameter("jpegquality", &name[1]);
	    return 0;
	} else {
	    pdfswf_setparameter("jpegquality", val);
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

/*struct docoptions_t options[] =
{{"o","output","filename::Specify output file"},
 {"V","version","Print program version"},
 {"i","ignore","Ignore draw order (makes the SWF file smaller, but may produce graphic errors)"},
 {"z","zlib","Use Flash 6 (MX) zlib compression (Needs at least Flash 6 Plugin to play)"},
 {"s","shapes","Don't use SWF Fonts, but store everything as shape"},
 {"j","jpegquality","Set quality of embedded jpeg pictures (default: 85)"},
 {"p","pages","Convert only pages in range. (E.g. 3-85)"},
 {"w","samewindow","Don't open a new browser window for links in the SWF"},
 {"f","fonts","Stroe full fonts in SWF. (Don't reduce to used characters)"},
 {"F","fontpath","path::Add directory to font search path"},
 {"B","viewer","name::Link viewer \"name\" to the pdf"},
 {"L","preloader","file.swf::Link preloader \"file.swf\" to the pdf"},
 {"b","defaultviewer","Link default viewer to the pdf"},
 {"l","defaultpreloader","Link default preloader to the pdf"}
 {0,0}
};*/
struct options_t options[] =
{{"o","output"},
 {"q","quiet"},
 {"V","version"},
 {"i","ignore"},
 {"z","zlib"},
 {"s","set"},
 {"S","shapes"},
 {"j","jpegquality"},
 {"p","pages"},
 {"w","samewindow"},
 {"f","fonts"},
 {"F","fontdir"},
 {"B","viewer"},
 {"L","preloader"},
 {"b","defaultviewer"},
 {"l","defaultpreloader"},
 {"t","stop"},
 {"T","flashversion"},
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
    printf("Usage: %s [Options] input.pdf [-o output.swf]\n", name);
    printf("\nBasic options:\n");
    printf("-p  --pages=range          Convert only pages in range\n");
    printf("-P  --password=password    Use password for deciphering the pdf\n");
    printf("-v  --verbose              Be verbose. Use more than one -v for greater effect\n");
    printf("-q  --quiet                Suppress normal messages. Use -qq to suppress warnings, also.\n");
#ifdef HAVE_DIRENT_H
    printf("-F  --fontdir directory    Add directory to font search path\n");
#endif
    printf("-V  --version              Print program version\n");
    printf("\nEnhanced conversion options:\n");
    printf("-S  --shapes               Don't use SWF Fonts, but store everything as shape\n");
    printf("-z  --zlib                 Use Flash 6 (MX) zlib compression (Needs at least Flash 6 Plugin to play)\n");
    printf("-w  --samewindow           Don't open a new Browser Window for Links in the SWF\n");
    printf("-f  --fonts                Store full fonts in SWF. (Don't reduce to used characters)\n");
    printf("-T  --flashversion=num     Set the flash version in the header to num (default: 4)\n");
    printf("-s insertstop              Insert a \"Stop\" Tag in every frame (don't turn pages automatically)\n");
    printf("-s zoom=factor             Scale result, default: 72\n");
    printf("-s jpegquality=quality     Set quality of embedded jpeg pictures (default:85)\n");
    printf("-s caplinewidth=value      Set the minimum line width to trigger cap style handling to value. (3)\n");
    printf("-s splinequality=value     Set the quality of spline convertion to value (0-100, default: 100).\n");
    printf("-s fontquality=value       Set the quality of font convertion to value (0-100, default: 100).\n");
    printf("-s ignoredraworder         Ignore draw order (makes the SWF file smaller and faster, but may produce\n"
	   "                           graphic errors)\n");
    printf("-s filloverlap             Make intersecting shapes overlap, instead of canceling each\n"
	   "                           other out. (Needed for some Powerpoint PDFs)\n");
    //deliberately undocumented (for now)
    //printf("-2                         Put 2 pages into each frame.\n");
    //printf("-4                         Put 4 pages into each frame.\n");
    printf("Postprocessing options:\n");
#ifndef SYSTEM_BACKTICKS
    printf("(They might not work because your system call doesn't support command substitution)\n");
#endif
    printf("-b  --defaultviewer        Link default viewer to the pdf (%s)\n", concatPaths(SWFDIR, "default_viewer.swf"));
    printf("-l  --defaultpreloader     Link default preloader the pdf (%s)\n", concatPaths(SWFDIR, "default_loader.swf"));
    printf("-B  --viewer=filename      Link viewer \"name\" to the pdf (\"%s -B\" for list)\n", name);
    printf("-L  --preloader=filename   Link preloader \"name\" to the pdf (\"%s -L\" for list)\n",name);
}

int main(int argn, char *argv[])
{
    int ret;
    char buf[256];
    int numfonts = 0;
    int t;
    char t1searchpath[1024];
    int nup_pos = 0;
    int x,y;
    
    initLog(0,-1,0,0,-1,loglevel);

#if defined(WIN32) && defined(HAVE_STAT) && defined(HAVE_SYS_STAT_H)
    FILE*test = fopen(concatPaths(FONTDIR,"\\d050000l.afm"), "rb");
    if(!test) {
	fprintf(stderr, "Couldn't find file %s - pdf2swf not installed properly? OS says:\n", concatPaths(FONTDIR, "\\d050000l.afm"));
	perror("open");
	exit(1);
    }
    fclose(test);
#endif

#ifdef HAVE_SRAND48
    srand48(time(0));
#else
#ifdef HAVE_SRAND
    srand(time(0));
#endif
#endif
    processargs(argn, argv);

    if(!filename)
    {
	fprintf(stderr, "Please specify an input file\n");
	exit(1);
    }

    if(!outputname)
    {
	if(filename) {
	    outputname = stripFilename(filename, ".swf");
	    msg("<notice> Output filename not given. Writing to %s", outputname);
	} 
    }
	
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

    /* add fonts */
    pdfswf_addfontdir(FONTDIR);
    for(t=0;t<fontpathpos;t++) {
	pdfswf_addfontdir(fontpaths[t]);
    }

    pdf_doc_t* pdf = pdf_init(filename, password);
    if(!pdf) {
        msg("<error> Couldn't open %s", filename);
        exit(1);
    }
    swf_output_t* swf = swf_output_init();

    for(t = 1; t <= pdf->num_pages; t++) 
    {
	if(is_in_range(t, pagerange)) {
            /* for links: FIXME */
            pdfswf_preparepage(t);
        }
	if(is_in_range(t, pagerange)) {
            pdf_page_t*page = pdf_getpage(pdf, t);
            if(nup) {
                pdf_page_info_t* info = pdf_page_getinfo(page);
                if(nup_pos%nup == 0) {
                    pdf_page_render(page, swf);
                    x = info->xMax;
                    y = info->yMax;
                } else if(nup_pos%nup == 1) {
                    pdf_page_rendersection(page, swf, x, 0, info->xMin+x, info->yMin, info->xMax+x, info->yMax);
                } else if(nup_pos%nup == 2) {
                    pdf_page_rendersection(page, swf, 0, y, info->xMin, info->yMin+y, info->xMax, info->yMax+y);
                } else if(nup_pos%nup == 3) {
                    pdf_page_rendersection(page, swf, x, y, info->xMin+x, info->yMin+y, info->xMax+x, info->yMax+y);
                }
                if(nup_pos % nup == nup-1)
                    swf_output_pagefeed(swf);

                pdf_page_info_destroy(info);
            } else {
                pdf_page_render(page, swf);
                swf_output_pagefeed(swf);
            }

            pdf_page_destroy(page);
        }
        nup_pos++;
    }
    if(!swf_output_save(swf, outputname))
        exit(1);
    msg("<notice> SWF written");
    swf_output_destroy(swf);

    pdf_destroy(pdf);

    if(viewer || preloader) {
#ifndef SYSTEM_BACKTICKS
	msg("<warning> Not sure whether system() can handle command substitution");
	msg("<warning> (According to config.h, it can't)");
#endif
	if(!system_quiet)
	    printf("\n");
    }

    char*zip = "";
    if(zlib)
	zip = "-z";

    if(viewer && !preloader) {
	systemf("swfcombine %s `swfdump -XY \"%s\"` \"%s\" viewport=\"%s\" -o \"%s\"",zip,
		outputname, viewer, outputname, outputname);
	if(!system_quiet)
	    printf("\n");
    }
    if(preloader && !viewer) {
	msg("<warning> --preloader option without --viewer option doesn't make very much sense.");
	ret = systemf("swfcombine %s `swfdump -r \"%s\"` %s/PreLoaderTemplate.swf loader=\"%s\" movie=\"%s\" -o \"%s\"",zip,
		preloader, SWFDIR, preloader, outputname, outputname);
	if(!system_quiet)
	    printf("\n");
    }
    if(preloader && viewer) {
	systemf("swfcombine \"%s\" viewport=%s -o __tmp__.swf",
		viewer, outputname, outputname);
	systemf("swfcombine %s `swfdump -XY \"%s\"` `swfdump -r \"%s\"` %s/PreLoaderTemplate.swf loader=%s movie=__tmp__.swf -o \"%s\"",zip,
		outputname, preloader, SWFDIR, preloader, outputname);
	systemf("rm __tmp__.swf");
    }

    return 0;
}

