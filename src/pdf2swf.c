/* pdf2swf.c
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
#include "../lib/args.h"
#include "../lib/os.h"
#include "../lib/rfxswf.h"
#include "../lib/devices/swf.h"
#include "../lib/devices/polyops.h"
#include "../lib/devices/record.h"
#include "../lib/pdf/pdf.h"
#include "../lib/log.h"

#define SWFDIR concatPaths(getInstallationPath(), "swfs")

static gfxsource_t*driver = 0;
static gfxdevice_t*out = 0;

static char * outputname = 0;
static int loglevel = 3;
static char * pagerange = 0;
static char * filename = 0;
static char * password = 0;
static int zlib = 0;

static char * preloader = 0;
static char * viewer = 0;
static int xnup = 1;
static int ynup = 1;

static int info_only = 0;

static int flatten = 0;

char* fontpaths[256];
int fontpathpos = 0;

int move_x=0;
int move_y=0;
int custom_move = 0;
int clip_x1=0,clip_y1=0,clip_x2=0,clip_y2=0;
int custom_clip = 0;

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
        xnup = 2;
        ynup = 1;
	return 0;
    }
    else if (!strcmp(name, "4"))
    {
        xnup = 2;
        ynup = 2;
	return 0;
    }
    else if (!strcmp(name, "9"))
    {
        xnup = 3;
        ynup = 3;
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
    else if (!strcmp(name, "c"))
    {
	char*s = strdup(val);
	char*x1 = strtok(s, ":");
	char*y1 = strtok(0, ":");
	char*x2 = strtok(0, ":");
	char*y2 = strtok(0, ":");
	if(!(x1 && y1 && x2 && y2)) {
	    fprintf(stderr, "-c option requires four arguments, <x1>:<y1>:<x2>:<y2>\n");
	    exit(1);
	}
	custom_clip = 1;
	clip_x1 = atoi(x1);
	clip_y1 = atoi(y1);
	clip_x2 = atoi(x2);
	clip_y2 = atoi(y2);
	free(s);
	return 1;
    }
    else if (!strcmp(name, "m"))
    {
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
	return 1;
    }
    else if (!strcmp(name, "s"))
    {
	char*s = strdup(val);
	char*c = strchr(s, '=');
	if(c && *c && c[1])  {
	    *c = 0;
	    c++;
	    driver->set_parameter(driver, s,c);
	    out->setparameter(out, s,c);
	} else {
	    driver->set_parameter(driver, s,"1");
	    out->setparameter(out, s,"1");
	}
	return 1;
    }
    else if (!strcmp(name, "S"))
    {
	driver->set_parameter(driver, "drawonlyshapes", "1");
	return 0;
    }
    else if (!strcmp(name, "i"))
    {
	driver->set_parameter(driver, "ignoredraworder", "1");
	return 0;
    }
#ifndef WIN32
    else if (!strcmp(name, "Q"))
    {
	int seconds = atoi(val);
	alarm(seconds);
	return 1;
    }
#endif
    else if (!strcmp(name, "z"))
    {
	driver->set_parameter(driver, "enablezlib", "1");
	zlib = 1;
	return 0;
    }
    else if (!strcmp(name, "n"))
    {
	driver->set_parameter(driver, "opennewwindow", "1");
	return 0;
    }
    else if (!strcmp(name, "I"))
    {
	info_only = 1;
	return 0;
    }
    else if (!strcmp(name, "t"))
    {
	driver->set_parameter(driver, "insertstop", "1");
	return 0;
    }
    else if (!strcmp(name, "T"))
    {
	if(!strcasecmp(val, "mx"))
	    driver->set_parameter(driver, "flashversion", "6");
	else
	    driver->set_parameter(driver, "flashversion", val);

	return 1;
    }
    else if (!strcmp(name, "f"))
    {
	driver->set_parameter(driver, "storeallcharacters", "1");
	driver->set_parameter(driver, "extrafontdata", "1");
	return 0;
    }
    else if (!strcmp(name, "w"))
    {
	driver->set_parameter(driver, "linksopennewwindow", "0");
	return 0;
    }
    else if (!strcmp(name, "O"))
    {
	int level = 1;
	int ret=0;
	if(val&& val[0] && val[1]==0 && isdigit(val[0])) {
	    level = atoi(val);
	    ret=1;
	}
	if(level>=1)
	    driver->set_parameter(driver, "poly2bitmap", "1");
	if(level>=2)
	    driver->set_parameter(driver, "bitmapfonts", "1");
	if(level>=3)
	    driver->set_parameter(driver, "ignoredraworder", "1");
	return ret;
    }
    else if (!strcmp(name, "G"))
    {
	driver->set_parameter(driver, "optimize_polygons", "1");
	flatten = 1;
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
	    driver->set_parameter(driver, "jpegquality", &name[1]);
	    return 0;
	} else {
	    driver->set_parameter(driver, "jpegquality", val);
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
 {"I","info"},
 {"z","zlib"},
 {"s","set"},
 {"S","shapes"},
 {"Q","maxtime"},
 {"j","jpegquality"},
 {"p","pages"},
 {"w","samewindow"},
 {"f","fonts"},
 {"F","fontdir"},
 {"B","viewer"},
 {"G","flatten"},
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
    printf("-s transparent             Make the SWF transparent\n");
    //deliberately undocumented (for now)
    //printf("-2                         Put 2 pages into each frame.\n");
    //printf("-4                         Put 4 pages into each frame.\n");
    printf("Postprocessing options:\n");
    printf("-b  --defaultviewer        Link default viewer to the pdf (%s)\n", concatPaths(SWFDIR, "default_viewer.swf"));
    printf("-l  --defaultpreloader     Link default preloader the pdf (%s)\n", concatPaths(SWFDIR, "default_loader.swf"));
    printf("-B  --viewer=filename      Link viewer \"name\" to the pdf (\"%s -B\" for list)\n", name);
    printf("-L  --preloader=filename   Link preloader \"name\" to the pdf (\"%s -L\" for list)\n",name);
}

float getRate(char*filename)
{
    int fi;
    SWF swf;
    fi = open(filename,O_RDONLY|O_BINARY);
    if(fi<0) { 
	char buffer[256];
	sprintf(buffer, "Couldn't open %s", filename);
        perror(buffer);
        exit(1);
    }
    if(swf_ReadSWF(fi,&swf) < 0)
    { 
        fprintf(stderr, "%s is not a valid SWF file or contains errors.\n",filename);
        close(fi);
        exit(1);
    }
    swf_FreeTags(&swf);
    return swf.frameRate / 256.0;
}

void show_info(gfxsource_t*driver, char*filename)
{
    gfxdocument_t* pdf = driver->open(driver, filename);
    int pagenr;
    FILE*fo=0;
    if(!pdf) {
	msg("<error> Couldn't open %s", filename);
	exit(1);
    }
    if(outputname) {
	fo = fopen(outputname, "wb");
	if(!fo) {
	    perror(outputname);exit(1);;
	}
    } else {
	fo = stdout;
    }

    for(pagenr = 1; pagenr <= pdf->num_pages; pagenr++) 
    {
	gfxpage_t*page = pdf->getpage(pdf,pagenr);
	if(is_in_range(pagenr, pagerange)) {
	    fprintf(fo, "page=%d width=%.2f height=%.2f\n", pagenr, page->width, page->height);
	}
    }
    pdf->destroy(pdf);
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
    char* installPath = getInstallationPath();
    
    initLog(0,-1,0,0,-1,loglevel);

    /* not needed anymore since fonts are embedded
       if(installPath) {
	fontpaths[fontpathpos++] = concatPaths(installPath, "fonts");
    }*/

#ifdef HAVE_SRAND48
    srand48(time(0));
#else
#ifdef HAVE_SRAND
    srand(time(0));
#endif
#endif
    driver = gfxsource_pdf_create();

    gfxdevice_t swf,wrap;
    gfxdevice_swf_init(&swf);
    
    gfxdevice_removeclippings_init(&wrap, &swf);

    out = &wrap;
    processargs(argn, argv);
    
    if(!flatten) {
	out = &swf;
    }
    
    if(!filename)
    {
	fprintf(stderr, "Please specify an input file\n");
	exit(1);
    }

    if(info_only) {
	show_info(driver, filename);
	return 0;
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

    if(pagerange)
	driver->set_parameter(driver, "pages", pagerange);

    if (!filename) {
	args_callback_usage(argv[0]);
	exit(0);
    }

    /* add fonts */
    for(t=0;t<fontpathpos;t++) {
	driver->set_parameter(driver, "fontdir", fontpaths[t]);
    }

    char fullname[256];
    if(password && *password) {
	sprintf(fullname, "%s|%s", filename, password);
	filename = fullname;
    }

    gfxdocument_t* pdf = driver->open(driver, filename);
    if(!pdf) {
        msg("<error> Couldn't open %s", filename);
        exit(1);
    }

    struct mypage_t {
	int x;
	int y;
	gfxpage_t*page;
    } pages[4];

    int pagenum = 0;
    int frame = 1;
    int pagenr;
    
    for(pagenr = 1; pagenr <= pdf->num_pages; pagenr++) 
    {
	if(is_in_range(pagenr, pagerange)) {
	    char mapping[80];
	    sprintf(mapping, "%d:%d", pagenr, frame);
	    pdf->set_parameter(pdf, "pagemap", mapping);
	    pagenum++;
	}
	if(pagenum == xnup*ynup || (pagenr == pdf->num_pages && pagenum>1)) {
	    pagenum = 0;
	    frame++;
	}
    }

    pagenum = 0;

    for(pagenr = 1; pagenr <= pdf->num_pages; pagenr++) 
    {
	if(is_in_range(pagenr, pagerange)) {
	    gfxpage_t* page = pages[pagenum].page = pdf->getpage(pdf, pagenr);
	    pages[pagenum].x = 0;
	    pages[pagenum].y = 0;
	    pages[pagenum].page = page;
	    pagenum++;
	}
	if(pagenum == xnup*ynup || (pagenr == pdf->num_pages && pagenum>1)) {

	    int t;
	    int xmax[xnup], ymax[xnup];
	    int x,y;
	    int width=0, height=0;

	    memset(xmax, 0, xnup*sizeof(int));
	    memset(ymax, 0, ynup*sizeof(int));

	    for(y=0;y<ynup;y++)
	    for(x=0;x<xnup;x++) {
		int t = y*xnup + x;

		if(pages[t].page->width > xmax[x])
		    xmax[x] = (int)pages[t].page->width;
		if(pages[t].page->height > ymax[y])
		    ymax[y] = (int)pages[t].page->height;
	    }
	    for(x=0;x<xnup;x++) {
		width += xmax[x];
		xmax[x] = width;
	    }
	    for(y=0;y<ynup;y++) {
		height += ymax[y];
		ymax[y] = height;
	    }
	    if(custom_clip) {
		out->startpage(out,clip_x2 - clip_x1, clip_y2 - clip_y1);
	    } else {
		out->startpage(out,width,height);
	    }
	    for(t=0;t<pagenum;t++) {
		int x = t%xnup;
		int y = t/xnup;
		int xpos = x>0?xmax[x-1]:0;
		int ypos = y>0?ymax[y-1]:0;
		msg("<verbose> Render (%d,%d) move:%d/%d\n",
			(int)(pages[t].page->width + xpos),
			(int)(pages[t].page->height + ypos), xpos, ypos);
		pages[t].page->rendersection(pages[t].page, out, custom_move? move_x : xpos, 
			                                   custom_move? move_y : ypos,
							   custom_clip? clip_x1 : 0 + xpos, 
							   custom_clip? clip_y1 : 0 + ypos, 
							   custom_clip? clip_x2 : pages[t].page->width + xpos, 
							   custom_clip? clip_y2 : pages[t].page->height + ypos);
	    }
	    out->endpage(out);
	    for(t=0;t<pagenum;t++)  {
		pages[t].page->destroy(pages[t].page);
	    }
	    pagenum = 0;
	}
    }
    
    gfxresult_t*result = out->finish(out);

    if(result->save(result, outputname) < 0) {
        exit(1);
    }

    int width = (int)result->get(result, "width");
    int height = (int)result->get(result, "height");
    msg("<notice> SWF written");
    
    result->destroy(result);

    pdf->destroy(pdf);
    driver->destroy(driver);

    const char*zip = "";
    if(zlib) {
	zip = "-z";
    }
    if(!preloader && viewer) {
	systemf("swfcombine %s -X %d -Y %d \"%s\" viewport=\"%s\" -o \"%s\"",zip,width,height,
		viewer, outputname, outputname);
	if(!system_quiet)
	    printf("\n");
    }
    if(preloader && !viewer) {
	msg("<warning> --preloader option without --viewer option doesn't make very much sense.");
	ret = systemf("swfcombine %s -Y %d -X %d %s/PreLoaderTemplate.swf loader=\"%s\" movie=\"%s\" -o \"%s\"",zip,width,height,
		SWFDIR, preloader, outputname, outputname);
	if(!system_quiet)
	    printf("\n");
    }
    if(preloader && viewer) {
#ifdef HAVE_MKSTEMP
        char tmpname[] = "__swf__XXXXXX";
        mkstemp(tmpname);
#else 
	char*tmpname = "__tmp__.swf";
#endif
	systemf("swfcombine \"%s\" viewport=%s -o %s",
		viewer, outputname, tmpname);
        systemf("swfcombine %s -X %d -Y %d -r %f %s/PreLoaderTemplate.swf loader=%s movie=%s -o \"%s\"",zip,width,height,
                getRate(preloader), SWFDIR, preloader, tmpname, outputname);
        systemf("rm %s", tmpname);
    }


    return 0;
}

