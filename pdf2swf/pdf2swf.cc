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
#include "SWFOutputDev.h"
extern "C" {
#include "log.h"
}

#ifndef WIN32
#define FONTDIR SWFTOOLS_DATADIR "/fonts"
#define SWFDIR SWFTOOLS_DATADIR "/swfs"
#else
#define FONTDIR "C:\\swftools\\fonts"
#define SWFDIR "C:\\swftools\\swfs"
#endif

static char * outputname = 0;
static int loglevel = 3;
static char * pagerange = 0;
static char * filename = 0;
static char * password = 0;

static char * preloader = 0;
static char * viewer = 0;

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
	return 0;
    }
    else if (!strcmp(name, "q"))
    {
	loglevel --;
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
	pdfswf_drawonlyshapes();
	return 0;
    }
    else if (!strcmp(name, "i"))
    {
	pdfswf_ignoredraworder();
	return 0;
    }
    else if (!strcmp(name, "z"))
    {
	pdfswf_enablezlib();
	return 0;
    }
    else if (!strcmp(name, "n"))
    {
	pdfswf_linksopennewwindow();
	return 0;
    }
    else if (!strcmp(name, "t"))
    {
	pdfswf_insertstop();
	return 0;
    }
    else if (!strcmp(name, "T"))
    {
	int i = atoi(val);
	if(!strcasecmp(val, "mx"))
	    i = 6;
	pdfswf_setversion(i);
	return 1;
    }
    else if (!strcmp(name, "f"))
    {
	pdfswf_storeallcharacters();
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
    printf("-s zoom=factor             Scale result, default: 100 (before 0.6.2: 72)\n");
    printf("-s jpegquality=quality     Set quality of embedded jpeg pictures (default:85)\n");
    printf("-s caplinewidth=value      Set the minimum line width to trigger cap style handling to value. (3)\n");
    printf("-s splinequality=value     Set the quality of spline convertion to value (0-100, default: 100).\n");
    printf("-s fontquality=value       Set the quality of font convertion to value (0-100, default: 100).\n");
    printf("-s ignoredraworder         Ignore draw order (makes the SWF file smaller and faster, but may produce\n"
	   "                           graphic errors)\n");
    printf("-s filloverlap             Make intersecting shapes overlap, instead of canceling each\n"
	   "                           other out. (Needed for some Powerpoint PDFs)\n");
    printf("Postprocessing options:\n");
#ifndef SYSTEM_BACKTICKS
    printf("(They might not work because your system call doesn't support command substitution)\n");
#endif
    printf("-b  --defaultviewer        Link default viewer to the pdf (%s/swfs/default_viewer.swf)\n", SWFTOOLS_DATADIR);
    printf("-l  --defaultpreloader     Link default preloader the pdf (%s/swfs/default_loader.swf)\n", SWFTOOLS_DATADIR);
    printf("-B  --viewer=filename      Link viewer \"name\" to the pdf (\"%s -B\" for list)\n", name);
    printf("-L  --preloader=filename   Link preloader \"name\" to the pdf (\"%s -L\" for list)\n",name);
}

#ifdef HAVE_DIRENT_H
void addfontdir(char* dirname, int*numfonts)
{
    if(!numfonts)
	msg("<verbose> Adding %s to search path\n", dirname);

    DIR*dir = opendir(dirname);
    if(!dir) {
	msg("<warning> Couldn't open directory %s\n", dirname);
	return;
    }
    dirent*ent;
    while(1) {
	ent = readdir (dir);
	if (!ent) 
	    break;
	int l;
	char*name = ent->d_name;
	char type = 0;
	if(!name) continue;
	l=strlen(name);
	if(l<4)
	    continue;
	if(!strncasecmp(&name[l-4], ".pfa", 4)) 
	    type=1;
	if(!strncasecmp(&name[l-4], ".pfb", 4)) 
	    type=3;
	if(!strncasecmp(&name[l-4], ".ttf", 4)) 
	    type=2;
	if(type)
	{
	    char*fontname = (char*)malloc(strlen(dirname)+strlen(name)+2);
	    strcpy(fontname, dirname);
#ifdef WIN32
		strcat(fontname, "\\");
#else
		strcat(fontname, "/");
#endif
	    strcat(fontname, name);
	    if(!numfonts)
		msg("<debug> Adding %s to fonts", fontname);
	    pdfswf_addfont(fontname);
	    if(numfonts)
		(*numfonts)++;
	}
    }
    closedir(dir);
}
#endif

char* stripfilename(char*filename, char*newext)
{
    char*last1 = strrchr(filename, '/');
    char*last2 = strrchr(filename, '\\');
    char*pos = filename;
    char*name;
    char*dot;
    if(last1>pos) pos = last1 + 1;
    if(last2>pos) pos = last2 + 1;
    name = (char*)malloc(strlen(pos)+5);
    strcpy(name, pos);
    dot = strrchr(name, '.');
    if(dot) {
	*dot = 0;
    }
    strcat(name, newext);
    return name;
}

int main(int argn, char *argv[])
{
    int ret;
    char buf[256];
    int numfonts = 0;
    int t;
    char t1searchpath[1024];

#if defined(WIN32) && defined(HAVE_STAT) && defined(HAVE_SYS_STAT_H)
    FILE*test = fopen(FONTDIR "\\d050000l.afm", "rb");
    if(!test) {
	fprintf(stderr, "Couldn't find file " FONTDIR "\\d050000l.afm- pdf2swf not installed properly? OS says:\n");
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
    initLog(0,-1,0,0,-1,loglevel);

    if(!filename)
    {
	fprintf(stderr, "Please specify an input file\n");
	exit(1);
    }

    if(!outputname)
    {
	if(filename) {
	    outputname = stripfilename(filename, ".swf");
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

#ifdef HAVE_DIRENT_H
    // pass 1
    addfontdir(FONTDIR, &numfonts);
    for(t=0;t<fontpathpos;t++) {
	addfontdir(fontpaths[t], &numfonts);
    }
    // pass 2
    addfontdir(FONTDIR, 0);
    for(t=0;t<fontpathpos;t++) {
	addfontdir(fontpaths[t], 0);
    }
#else
    msg("<error> Couldn't find any fonts!");
#endif

    pdfswf_init(filename, password);
    pdfswf_setoutputfilename(outputname);

    int pages = pdfswf_numpages();
    for(t = 1; t <= pages; t++) 
    {
	if(is_in_range(t, pagerange))
	pdfswf_convertpage(t);
    }
    pdfswf_performconversion();

    pdfswf_close();

    if(viewer || preloader) {
#ifndef SYSTEM_BACKTICKS
	msg("<warning> Not sure whether system() can handle command substitution");
	msg("<warning> (According to config.h, it can't)");
#endif
	if(!system_quiet)
	    printf("\n");
    }

    if(viewer && !preloader) {
	systemf("swfcombine `swfdump -XY \"%s\"` \"%s\" viewport=\"%s\" -o \"%s\"",
		outputname, viewer, outputname, outputname);
	if(!system_quiet)
	    printf("\n");
    }
    if(preloader && !viewer) {
	msg("<warning> --preloader option without --viewer option doesn't make very much sense.");
	ret = systemf("swfcombine `swfdump -r \"%s\"` %s/PreLoaderTemplate.swf loader=\"%s\" movie=\"%s\" -o \"%s\"",
		preloader, SWFDIR, preloader, outputname, outputname);
	if(!system_quiet)
	    printf("\n");
    }
    if(preloader && viewer) {
	systemf("swfcombine \"%s\" viewport=%s -o __tmp__.swf",
		viewer, outputname, outputname);
	systemf("swfcombine `swfdump -XY \"%s\"` `swfdump -r \"%s\"` %s/PreLoaderTemplate.swf loader=%s movie=__tmp__.swf -o \"%s\"",
		outputname, preloader, SWFDIR, preloader, outputname);
	systemf("rm __tmp__.swf");
    }

    return 0;
}


