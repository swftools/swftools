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
#include <memory.h>
#include <unistd.h>
#include "../config.h"
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

#include "../lib/args.h"
#include "../lib/os.h"
#include "../lib/rfxswf.h"
#include "../lib/devices/swf.h"
#include "../lib/devices/polyops.h"
#include "../lib/devices/record.h"
#include "../lib/devices/rescale.h"
#include "../lib/gfxfilter.h"
#include "../lib/pdf/pdf.h"
#include "../lib/log.h"

#define SWFDIR concatPaths(getInstallationPath(), "swfs")

static gfxsource_t*driver = 0;
static gfxdevice_t*out = 0;

static int maxwidth=0, maxheight=0;

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

static int max_time = 0;

static int flatten = 0;

static char* filters = 0;

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
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
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

#ifdef HAVE_SIGNAL_H
void sigalarm(int signal)
{
    msg("<fatal> Aborting rendering after %d seconds", max_time);
#if 0 && defined(HAVE_SYS_TIME_H) && defined(HAVE_SYS_RESOURCE_H) && defined(HAVE_GETRUSAGE)
    struct rusage usage;
    getrusage(RUSAGE_CHILDREN, &usage);
    msg("<fatal> Memory used: %d,%d,%d", usage.ru_maxrss, usage.ru_idrss, usage.ru_isrss);
#endif 
#if defined(HAVE_MALLINFO) && defined(HAVE_MALLOC_H)
    struct mallinfo info = mallinfo();
    msg("<fatal> Memory used: %d Mb (%d bytes)", info.uordblks/1048576, info.uordblks);
#endif
    exit(1);
}
#endif

typedef struct _parameter {
    struct _parameter*next;
    const char*name;
    const char*value;
} parameter_t;

static parameter_t* device_config = 0;
static parameter_t* device_config_next = 0;
static void store_parameter(const char*name, const char*value)
{
    parameter_t*o = device_config;
    while(o) {
        if(!strcmp(name, o->name)) {
            /* overwrite old value */
            free((void*)o->value);
            o->value = strdup(value);
            return;
        }
        o = o->next;
    }
    parameter_t*p = (parameter_t*)malloc(sizeof(parameter_t));
    p->name = strdup(name);
    p->value = strdup(value);
    p->next = 0;

    if(device_config_next) {
	device_config_next->next = p;
	device_config_next = p;
    } else {
	device_config = p;
	device_config_next = p;
    }
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
    else if (!strcmp(name, "X"))
    {
        maxwidth = atoi(val);
	return 1;
    }
    else if (!strcmp(name, "Y"))
    {
        maxheight = atoi(val);
	return 1;
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
	char*s = val;
	char*c = strchr(s, '=');
	if(c && *c && c[1])  {
	    *c = 0;
	    c++;
	    store_parameter(s,c);
	} else if(!strcmp(s,"help")) {
	    printf("PDF Parameters:\n");
	    gfxsource_t*pdf = gfxsource_pdf_create();
	    pdf->setparameter(pdf, "help", "");
	    gfxdevice_t swf;
	    gfxdevice_swf_init(&swf);
	    printf("SWF Parameters:\n");
	    swf.setparameter(&swf, "help", "");
	    exit(0);
	} else {
	    store_parameter(s,"1");
	}
	return 1;
    }
    else if (!strcmp(name, "S"))
    {
	store_parameter("drawonlyshapes", "1");
	return 0;
    }
    else if (!strcmp(name, "i"))
    {
	store_parameter("ignoredraworder", "1");
	return 0;
    }
#ifndef WIN32
    else if (!strcmp(name, "Q"))
    {
	max_time = atoi(val);
	alarm(max_time);
# ifdef HAVE_SIGNAL_H
        signal(SIGALRM, sigalarm);
# endif
	return 1;
    }
#endif
    else if (!strcmp(name, "z"))
    {
	store_parameter("enablezlib", "1");
	zlib = 1;
	return 0;
    }
    else if (!strcmp(name, "n"))
    {
	store_parameter("opennewwindow", "1");
	return 0;
    }
    else if (!strcmp(name, "I"))
    {
	info_only = 1;
	return 0;
    }
    else if (!strcmp(name, "t"))
    {
	store_parameter("insertstop", "1");
	return 0;
    }
    else if (!strcmp(name, "T"))
    {
	if(!strcasecmp(val, "mx"))
	    store_parameter("flashversion", "6");
	else
	    store_parameter("flashversion", val);

	return 1;
    }
    else if (!strcmp(name, "f"))
    {
	store_parameter("storeallcharacters", "1");
	store_parameter("extrafontdata", "1");
	return 0;
    }
    else if (!strcmp(name, "ff"))
    {
	if(filters) {
	    // append this to the current filter expression (we allow more than one --filter)
	    int l = strlen(filters);
	    int new_len = l + strlen(val) + 2;
	    filters = (char*)realloc(filters, new_len);
	    filters[l] = ':';
	    strcpy(filters+l+1, val);
	} else {
	    filters = strdup(val);
	}
	return 1;
    }
    else if (!strcmp(name, "w"))
    {
	store_parameter("linksopennewwindow", "0");
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
	    store_parameter("poly2bitmap", "1");
	if(level>=2)
	    store_parameter("bitmapfonts", "1");
	if(level>=3)
	    store_parameter("ignoredraworder", "1");
	return ret;
    }
    else if (!strcmp(name, "G"))
    {
	//store_parameter("optimize_polygons", "1");
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
	    store_parameter("jpegquality", &name[1]);
	    return 0;
	} else {
	    store_parameter("jpegquality", val);
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

static struct options_t options[] = {
{"h", "help"},
{"V", "version"},
{"o", "output"},
{"p", "pages"},
{"P", "password"},
{"v", "verbose"},
{"z", "zlib"},
{"i", "ignore"},
{"j", "jpegquality"},
{"s", "set"},
{"w", "samewindow"},
{"t", "stop"},
{"T", "flashversion"},
{"F", "fontdir"},
{"ff", "filter"},
{"b", "defaultviewer"},
{"l", "defaultloader"},
{"B", "viewer"},
{"L", "preloader"},
{"q", "quiet"},
{"S", "shapes"},
{"f", "fonts"},
{"G", "flatten"},
{"I", "info"},
{"Q", "maxtime"},
{"X", "width"},
{"Y", "height"},
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

void args_callback_usage(char *name)
{
    printf("\n");
    printf("Usage: %s [-options] file.pdf -o file.swf\n", name);
    printf("\n");
    printf("-h , --help                    Print short help message and exit\n");
    printf("-V , --version                 Print version info and exit\n");
    printf("-o , --output file.swf         Direct output to file.swf. If file.swf contains '%%' (file%%.swf), then each page goes to a separate file.\n");
    printf("-p , --pages range             Convert only pages in range with range e.g. 1-20 or 1,4,6,9-11 or\n");
    printf("-P , --password password       Use password for deciphering the pdf.\n");
    printf("-v , --verbose                 Be verbose. Use more than one -v for greater effect.\n");
    printf("-z , --zlib                    Use Flash 6 (MX) zlib compression.\n");
    printf("-i , --ignore                  Allows pdf2swf to change the draw order of the pdf. This may make the generated\n");
    printf("-j , --jpegquality quality     Set quality of embedded jpeg pictures to quality. 0 is worst (small), 100 is best (big). (default:85)\n");
    printf("-s , --set param=value         Set a SWF encoder specific parameter.  See pdf2swf -s help for more information.\n");
    printf("-w , --samewindow              When converting pdf hyperlinks, don't make the links open a new window. \n");
    printf("-t , --stop                    Insert a stop() command in each page. \n");
    printf("-T , --flashversion num        Set Flash Version in the SWF header to num.\n");
    printf("-F , --fontdir directory       Add directory to the font search path.\n");
    printf("-b , --defaultviewer           Link a standard viewer to the swf file. \n");
    printf("-l , --defaultloader           Link a standard preloader to the swf file which will be displayed while the main swf is loading.\n");
    printf("-B , --viewer filename         Link viewer filename to the swf file. \n");
    printf("-L , --preloader filename      Link preloader filename to the swf file. \n");
    printf("-q , --quiet                   Suppress normal messages.  Use -qq to suppress warnings, also.\n");
    printf("-S , --shapes                  Don't use SWF Fonts, but store everything as shape.\n");
    printf("-f , --fonts                   Store full fonts in SWF. (Don't reduce to used characters).\n");
    printf("-G , --flatten                 Remove as many clip layers from file as possible. \n");
    printf("-I , --info                    Don't do actual conversion, just display a list of all pages in the PDF.\n");
    printf("-Q , --maxtime n               Abort conversion after n seconds. Only available on Unix.\n");
    printf("\n");
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


static gfxdevice_t swf,wrap,rescale;
gfxdevice_t*create_output_device()
{
    gfxdevice_swf_init(&swf);

    /* set up filter chain */
	
    out = &swf;
    if(flatten) {
        gfxdevice_removeclippings_init(&wrap, &swf);
        out = &wrap;
    }

    if(maxwidth || maxheight) {
        gfxdevice_rescale_init(&rescale, out, maxwidth, maxheight, 0);
        out = &rescale;
    }

    if(filters) {
	gfxfilterchain_t*chain = gfxfilterchain_parse(filters);
	if(!chain) {
	    fprintf(stderr, "Unable to parse filters: %s\n", filters);
	    exit(1);
	}
	out = gfxfilterchain_apply(chain, out);
	gfxfilterchain_destroy(chain);
    }

    /* pass global parameters to output device */
    parameter_t*p = device_config;
    while(p) {
	out->setparameter(out, p->name, p->value);
	p = p->next;
    }
    return out;
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
    int one_file_per_page = 0;
    
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

    processargs(argn, argv);
    
    driver = gfxsource_pdf_create();
    
    /* pass global parameters to PDF driver*/
    parameter_t*p = device_config;
    while(p) {
	driver->setparameter(driver, p->name, p->value);
	p = p->next;
    }

    if(!filename)
    {
	fprintf(stderr, "Please specify an input file\n");
	exit(1);
    }

    if (!info_only) {
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
    }

    // test if the page range is o.k.
    is_in_range(0x7fffffff, pagerange);

    if (!filename) {
	args_callback_usage(argv[0]);
	exit(0);
    }
    
    char fullname[256];
    if(password && *password) {
	sprintf(fullname, "%s|%s", filename, password);
	filename = fullname;
    }
    
    if(pagerange)
	driver->setparameter(driver, "pages", pagerange);

    /* add fonts */
    for(t=0;t<fontpathpos;t++) {
	driver->setparameter(driver, "fontdir", fontpaths[t]);
    }

    if(info_only) {
	show_info(driver, filename);
	return 0;
    }

    char*u = 0;
    if((u = strchr(outputname, '%'))) {
	if(strchr(u+1, '%') || 
	   strchr(outputname, '%')!=u)  {
	    msg("<error> only one %% allowed in filename\n");
	    return 1;
	}
	if(preloader || viewer) {
	    msg("<error> -b/-l/-B/-L not supported together with %% in filename\n");
	    return 1;
	}
	msg("<notice> outputting one file per page");
	one_file_per_page = 1;
	char*pattern = (char*)malloc(strlen(outputname)+2);
	/* convert % to %d */
	int l = u-outputname+1;
	memcpy(pattern, outputname, l);
	pattern[l]='d';
	strcpy(pattern+l+1, outputname+l);
	outputname = pattern;
    }

    gfxdocument_t* pdf = driver->open(driver, filename);
    if(!pdf) {
        msg("<error> Couldn't open %s", filename);
        exit(1);
    }
    /* pass global parameters document */
    p = device_config;
    while(p) {
	pdf->setparameter(pdf, p->name, p->value);
	p = p->next;
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
	    pdf->setparameter(pdf, "pagemap", mapping);
	    pagenum++;
	}
	if(pagenum == xnup*ynup || (pagenr == pdf->num_pages && pagenum>1)) {
	    pagenum = 0;
	    frame++;
	}
    }
    if(pagerange && !pagenum && frame==1) {
	fprintf(stderr, "No pages in range %s", pagerange);
	exit(1);
    }

    pagenum = 0;

    gfxdevice_t*out = create_output_device();;
    pdf->prepare(pdf, out);

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

	    if(one_file_per_page) {
		gfxresult_t*result = out->finish(out);out=0;
		char buf[1024];
		sprintf(buf, outputname, pagenr);
		if(result->save(result, buf) < 0) {
		    return 1;
		}
		result->destroy(result);result=0;
		out = create_output_device();;
                pdf->prepare(pdf, out);
		msg("<notice> Writing SWF file %s", buf);
	    }
	}
    }
   
    if(one_file_per_page) {
	// remove empty device
	gfxresult_t*result = out->finish(out);out=0;
	result->destroy(result);result=0;
    } else {
	gfxresult_t*result = out->finish(out);
	msg("<notice> Writing SWF file %s", outputname);
	if(result->save(result, outputname) < 0) {
	    exit(1);
	}
	int width = (int)(ptroff_t)result->get(result, "width");
	int height = (int)(ptroff_t)result->get(result, "height");
	result->destroy(result);result=0;

	if(preloader || viewer) {
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
	}
    }

    pdf->destroy(pdf);
    driver->destroy(driver);

   
    /* free global parameters */
    p = device_config;
    while(p) {
	parameter_t*next = p->next;
	if(p->name) free((void*)p->name);p->name = 0;
	if(p->value) free((void*)p->value);p->value =0;
	p->next = 0;free(p);
	p = next;
    }
    if(filters) {
	free(filters);
    }

    return 0;
}

