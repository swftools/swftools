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
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include "../lib/args.h"
#include "SWFOutputDev.h"
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
	sprintf(buf, "%s/swfs/default_loader.swf", DATADIR);
	preloader = strdup(buf);
	return 0;
    }
    else if (!strcmp(name, "b"))
    {
	char buf[256];
	sprintf(buf, "%s/swfs/default_viewer.swf", DATADIR);
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
	    systemf("ls %s/swfs/*_loader.swf", DATADIR);
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
	    systemf("ls %s/swfs/*_viewer.swf", DATADIR);
	    if(!system_quiet)
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
 {"s","shapes"},
 {"j","jpegquality"},
 {"p","pages"},
 {"w","samewindow"},
 {"f","fonts"},
 {"F","fontpath"},
 {"B","viewer"},
 {"L","preloader"},
 {"b","defaultviewer"},
 {"l","defaultpreloader"},
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
    printf("Usage: %s [-si] [-j quality] [-p range] [-P password] input.pdf -o output.swf\n", name);
    printf("\n");
    printf("-p  --pages=range          Convert only pages in range\n");
    printf("-P  --password=password    Use password for deciphering the pdf\n");
    printf("-s  --shapes               Don't use SWF Fonts, but store everything as shape\n");
    printf("-i  --ignore               Ignore draw order (makes the SWF file smaller, but may produce\n");
    printf("                           graphic errors)\n");
    printf("-z  --zlib                 Use Flash 6 (MX) zlib compression (Needs at least Flash 6 Plugin to play)\n");
    printf("-j  --jpegquality=quality  Set quality of embedded jpeg pictures (default:85)\n");
    printf("-v  --verbose              Be verbose. Use more than one -v for greater effect\n");
    printf("-q  --quiet                Suppress normal messages. Use -qq to suppress warnings, also.\n");
    printf("-w  --samewindow           Don't open a new Browser Window for Links in the SWF\n");
#ifdef HAVE_DIRENT
    printf("-F  --fontdir directory    Add directory to font search path\n");
#endif
    printf("-f  --fonts                Store full fonts in SWF. (Don't reduce to used characters)\n");
    printf("-V  --version              Print program version\n");
#ifndef SYSTEM_BACKTICKS
    printf("The following might not work because your system call doesn't support command substitution:\n");
#endif
    printf("-b  --defaultviewer        Link default viewer to the pdf (%s/swfs/default_viewer.swf)\n", DATADIR);
    printf("-l  --defaultpreloader     Link default preloader the pdf (%s/swfs/default_loader.swf)\n", DATADIR);
    printf("-B  --viewer=filename      Link viewer \"name\" to the pdf (\"%s -B\" for list)\n", name);
    printf("-L  --preloader=filename   Link preloader \"name\" to the pdf (\"%s -L\" for list)\n",name);
}

#ifdef HAVE_DIRENT_H
void addfontdir(FILE*database, char* dirname, int*numfonts, char*searchpath) 
{
    if(searchpath) {
	if(searchpath[0])
	    strcat(searchpath, ":");
	strcat(searchpath, dirname);
    }
    logf("<verbose> Adding %s to search path\n", dirname);

    DIR*dir = opendir(dirname);
    if(!dir) {
	logf("<warning> Couldn't open directory %s\n", dirname);
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
	if(!strncasecmp(&name[l-4], ".afm", 4)) 
	    type=1;
	if(!strncasecmp(&name[l-4], ".ttf", 4)) 
	    type=2;
	if(type)
	{
	    if(database && type==1) {
		char buf[256],a;
		FILE*fi;
		sprintf(buf, "%s/%s", dirname,name);
		fi = fopen(buf, "rb");
		if(!fi || !fread(&a,1,1,fi)) {
		    logf("<warning> Couldn't read from %s", buf);
		}
		fprintf(database, "%s\n", buf);
		logf("<verbose> Found font %s\n", buf);
	    } 
	    if(numfonts)
		(*numfonts)++;
	}
    }
    closedir(dir);
}
#endif

int main(int argn, char *argv[])
{
    int ret;
    char buf[256];
    int numfonts = 0;
    int t;
    char t1searchpath[1024];
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

    logf("<verbose> reading font files from %s/fonts\n", DATADIR);
    //TODO: use tempnam here. Check if environment already contains a
    //T1LIB_CONFIG.
    putenv( "T1LIB_CONFIG=/tmp/t1lib.config.tmp");
    FILE*db = fopen("/tmp/FontDataBase", "wb");
    FILE*fi = fopen("/tmp/t1lib.config.tmp", "wb");
    if(!db || !fi) {
	fprintf(stderr, "Couldn't create temporary file in /tmp/\n");
	exit(1);
    }
    t1searchpath[0] = 0;
#ifdef HAVE_DIRENT_H
    sprintf(buf, "%s/fonts",DATADIR);
    // pass 1
    addfontdir(0, buf, &numfonts, 0);
    for(t=0;t<fontpathpos;t++) {
	addfontdir(0, fontpaths[t], &numfonts,0);
    }
    fprintf(db, "%d\n", numfonts);
    // pass 2
    addfontdir(db, buf, 0, t1searchpath);
    for(t=0;t<fontpathpos;t++) {
	addfontdir(db, fontpaths[t], 0, t1searchpath);
    }
#else
/* This is a workaround. The correct way would be to
   get directory listings working on all systems.
*/
    strcpy(t1searchpath, DATADIR);
    strcat(t1searchpath, "/fonts");
    fprintf(db, "14\n");
    fprintf(db, "n021003l.afm\n");
    fprintf(db, "n021023l.afm\n");
    fprintf(db, "n021004l.afm\n");
    fprintf(db, "n021024l.afm\n");
    fprintf(db, "n019003l.afm\n");
    fprintf(db, "n019023l.afm\n");
    fprintf(db, "n019004l.afm\n");
    fprintf(db, "n019024l.afm\n");
    fprintf(db, "n022003l.afm\n");
    fprintf(db, "n022023l.afm\n");
    fprintf(db, "n022004l.afm\n");
    fprintf(db, "n022024l.afm\n");
    fprintf(db, "s050000l.afm\n");
    fprintf(db, "d050000l.afm\n");
#endif

    fprintf(fi, "FONTDATABASE=/tmp/FontDataBase\n");
    fprintf(fi, "ENCODING=%s:.\n", t1searchpath);
    fprintf(fi, "AFM=%s:.\n", t1searchpath);
    fprintf(fi, "TYPE1=%s:.\n", t1searchpath);
    fclose(fi);
    fclose(db);
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
    for(t = 1; t <= pages; t++) 
    {
	if(is_in_range(t, pagerange))
	pdfswf_convertpage(t);
    }
    pdfswf_performconversion();

    pdfswf_close();

    if(viewer || preloader) {
#ifndef SYSTEM_BACKTICKS
	logf("<warning> Not sure whether system() can handle command substitution");
	logf("<warning> (According to config.h, it can't)");
#endif
	if(!system_quiet)
	    printf("\n");
    }

    if(viewer && !preloader) {
	systemf("swfcombine `swfdump -XY %s` %s viewport=%s -o %s",
		outputname, viewer, outputname, outputname);
	if(!system_quiet)
	    printf("\n");
    }
    if(preloader && !viewer) {
	logf("<warning> --preloader option without --viewer option doesn't make very much sense.");
	ret = systemf("swfcombine `swfdump -r %s` %s/swfs/PreLoaderTemplate.swf loader=%s movie=%s -o %s",
		preloader, DATADIR, preloader, outputname, outputname);
	if(!system_quiet)
	    printf("\n");
    }
    if(preloader && viewer) {
	systemf("swfcombine %s viewport=%s -o __tmp__.swf",
		viewer, outputname, outputname);
	systemf("swfcombine `swfdump -XY %s` `swfdump -r %s` %s/swfs/PreLoaderTemplate.swf loader=%s movie=__tmp__.swf -o %s",
		outputname, preloader, DATADIR, preloader, outputname);
	systemf("rm __tmp__.swf");
    }

    unlink("/tmp/FontDataBase");
    return 0;
}


