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
#include "../lib/args.h"
#include "../lib/os.h"
#include "../lib/gfxsource.h"
#include "../lib/gfxdevice.h"
#include "../lib/gfxpoly.h"
#include "../lib/devices/pdf.h"
#include "../lib/devices/swf.h"
#include "../lib/devices/text.h"
#include "../lib/devices/render.h"
#include "../lib/devices/file.h"
#include "../lib/devices/bbox.h"
#ifdef HAVE_LRF
#include "../lib/devices/lrf.h"
#endif
#include "../lib/devices/rescale.h"
#include "../lib/devices/record.h"
#include "../lib/readers/image.h"
#include "../lib/readers/swf.h"
#include "../lib/pdf/pdf.h"
#include "../lib/log.h"

static gfxsource_t*driver = 0;

static char * outputname = 0;
static int loglevel = 3;
static char * maxdpi = "72";
static char * pagerange = 0;
static char * filename = 0;
static const char * format = 0;

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
    else if (!strcmp(name, "f"))
    {
	format = val;
	return 1;
    }
    else if (!strcmp(name, "q"))
    {
	loglevel --;
        setConsoleLogging(loglevel);
	return 0;
    }
    else if (!strcmp(name, "r"))
    {
	maxdpi = val;
	return 1;
    }
    else if (name[0]=='p')
    {
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
    else if (!strcmp(name, "s"))
    {
        if(!driver) {
            fprintf(stderr, "Specify input file before -s\n");
            exit(1);
        }
	char*s = strdup(val);
	char*c = strchr(s, '=');
	if(c && *c && c[1])  {
	    *c = 0;
	    c++;
	    driver->setparameter(driver, s,c);
	} else {
	    driver->setparameter(driver, s,"1");
        }
        free(s);
	return 1;
    }
    else if (!strcmp(name, "V"))
    {	
	printf("gfx2gfx - part of %s %s\n", PACKAGE, VERSION);
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
 {"q","quiet"},
 {"V","version"},
 {"s","set"},
 {"r","resolution"},
 {"p","pages"},
 {0,0}
};

int args_callback_longoption(char*name,char*val) {
    return args_long2shortoption(options, name, val);
}

int args_callback_command(char*name, char*val) {
    if (!filename) {

        filename = name;

        if(strstr(filename, ".pdf") || strstr(filename, ".PDF")) {
            msg("<notice> Treating file as PDF");
            driver = gfxsource_pdf_create();
        } else if(strstr(filename, ".swf") || strstr(filename, ".SWF")) {
            msg("<notice> Treating file as SWF");
            driver = gfxsource_swf_create();
        } else if(strstr(filename, ".jpg") || strstr(filename, ".JPG") ||
                  strstr(filename, ".png") || strstr(filename, ".PNG")) {
            msg("<notice> Treating file as Image");
            driver = gfxsource_image_create();
        }
    } else {
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
}

int main(int argn, char *argv[])
{
    processargs(argn, argv);
    initLog(0,-1,0,0,-1,loglevel);
    
    if(!filename) {
	fprintf(stderr, "Please specify an input file\n");
	exit(1);
    }
    
    if(!outputname)
    {
	if(filename) {
	    outputname = stripFilename(filename, ".out");
	    msg("<notice> Output filename not given. Writing to %s", outputname);
	} 
    }
    if(!outputname)
    {
	fprintf(stderr, "Please use -o to specify an output file\n");
	exit(1);
    }
    is_in_range(0x7fffffff, pagerange);
    if(pagerange)
	driver->setparameter(driver, "pages", pagerange);

    if(!filename) {
	args_callback_usage(argv[0]);
	exit(0);
    }

    gfxdocument_t* doc = driver->open(driver, filename);
    if(!doc) {
        msg("<error> Couldn't open %s", filename);
        exit(1);
    }

    if(!format) {
	char*x = strrchr(outputname, '.');
	if(x) 
	    format = x+1;
    }


    gfxresult_t*result = 0;
#ifdef HAVE_LRF
    if(!strcasecmp(format, "lrf")) {
        gfxdevice_t lrf;
        gfxdevice_lrf_init(&lrf);

        gfxdevice_t rescale;
        gfxdevice_rescale_init(&rescale, &lrf, 592, 732, 0);

        gfxdevice_t*out = &rescale;
        out->setparameter(out, "keepratio", "1");
        out->setparameter(out, "pagepattern", outputname);

        gfxdevice_t bbox2,*bbox=&bbox2;
        gfxdevice_bbox_init(bbox);
        bbox->setparameter(bbox, "graphics", "0");

        int pagenr;

        for(pagenr = 1; pagenr <= doc->num_pages; pagenr++) 
        {
            if(is_in_range(pagenr, pagerange)) {
                gfxpage_t* page = doc->getpage(doc, pagenr);
                bbox->startpage(bbox,-1,-1);
                page->render(page, bbox);
                gfxbbox_t b = gfxdevice_bbox_getbbox(bbox);

                out->startpage(out, b.xmax-b.xmin, b.ymax-b.ymin);
                page->rendersection(page, out, -b.xmin, -b.ymin, 0,0,b.xmax-b.xmin,b.ymax-b.ymin);
                out->endpage(out);

                page->destroy(page);
            }
        }
        result = out->finish(out);
    } else 
#endif
    {
        gfxdevice_t _out,*out=&_out;
        if(!strcasecmp(format, "swf")) {
            gfxdevice_swf_init(out);
	    out->setparameter(out, "maxdpi", "320");
        } else if(!strcasecmp(format, "img") || !strcasecmp(format, "png")) {
            gfxdevice_render_init(out);
	    out->setparameter(out, "antialize", "4");
        } else if(!strcasecmp(format, "txt")) {
            gfxdevice_text_init(out);
        } else if(!strcasecmp(format, "log")) {
            gfxdevice_file_init(out, "/tmp/device.log");
        } else if(!strcasecmp(format, "pdf")) {
            gfxdevice_pdf_init(out);
        } else {
	    msg("<error> Invalid output format: %s", format);
	    exit(1);
	}
	    
	out->setparameter(out, "maxdpi", maxdpi);

        int pagenr;
        for(pagenr = 1; pagenr <= doc->num_pages; pagenr++) 
        {
            if(is_in_range(pagenr, pagerange)) {
                gfxpage_t* page = doc->getpage(doc, pagenr);
                out->startpage(out, page->width, page->height);
                page->render(page, out);
                out->endpage(out);
                page->destroy(page);
            }
        }
        result = out->finish(out);
    }

    if(result) {
	if(result->save(result, outputname) < 0) {
	    exit(1);
	}
	result->destroy(result);
    }

    doc->destroy(doc);

    driver->destroy(driver);
    return 0;
}

