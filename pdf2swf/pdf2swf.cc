/* pdf2swf.cc
   main routine for pdf2swf(1)

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../config.h"
#include "args.h"
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

struct options_t
{
    char shortoption;
    char*longoption;
} options[] =
{{'o',"output"},
 {'V',"version"},
 {'i',"ignore"},
 {'s',"shapes"},
 {'j',"jpegquality"},
 {'p',"pages"}
};

int args_callback_longoption(char*name,char*val) {
    int t;
    char*equal = strchr(name,'=');
    if (equal) {
	*equal = 0;
	equal++;
    }
    for(t=0;t<sizeof(options)/sizeof(struct options_t);t++) {
        if(!strcmp(options[t].longoption, name)) {
		char*tmp = (char*)malloc(strlen(name)+(equal?strlen(equal)+2:2));
		tmp[0] = options[t].shortoption;
		tmp[1] = 0;
		if(equal) {
		    strcpy(&tmp[1], equal);
		}
		return args_callback_option(tmp,val);
	}
    }
    fprintf(stderr, "Unknown option: --%s\n", name);
    exit(1);
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
    printf("-V  --version              Print program version\n");
}

/* check whether the value t is in a given range.
  examples: 3 is in range 1-10: true
            7 is in range 2-4,6,8-10: false
	    9 is in range 1,2,3-12: true
*/
char is_in_range(int t, char*irange)
{
    char*pos = irange;
    char*digits;
    int num;
    char range = 0;
    int last=0;
    char tmp;

    if(!irange)  // no range resembles (-OO,OO)
	return 1;

    while(*pos)
    {
	while(*pos == ' ' || *pos == '\r' || *pos == '\n' || *pos == '\t')
	    pos++;

	digits = pos;
	while(*digits>='0' && *digits<='9')
	    digits++;
	if(digits == pos) {
	    fprintf(stderr, "Error: \"%s\" is not a valid format (digit expected)\n",irange);
	    exit(1);
        }
	
	tmp=*digits;*digits=0;
	num = atoi(pos);
	*digits=tmp;
	pos = digits;

	while(*pos == ' ' || *pos == '\r' || *pos == '\n' || *pos == '\t')
	    pos++;

	if(range && last<=t && num>=t)
	    return 1;
	if(range) {
	    range = 0;
	    if(*pos)
	     pos ++;
	    continue;
	}

	if(*pos=='-')
	{
	    if(range) {
		fprintf(stderr, "Error: \"%s\" is not a valid format (too many '-'s)\n",irange);
		exit(1);
	    }
	    last = num;
	    range = 1;
	    if(*pos)
	     pos ++;
	    continue;
	} 
	else 
	{
	    /* if it isn't a '-', we assume it is a seperator like
	       ',', ';', ':', whatever. */
	    if(t == num)
		return 1;
	    if(*pos)
	     pos ++;
	    continue;
	}
    }
    if(range && last<=t)
	return 1;
    return 0;
}

int main(int argn, char *argv[])
{
    srand48(time(0));
    processargs(argn, argv);
    initLog(0,-1,0,0,-1,loglevel);

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
    fprintf(fi, "FONTDATABASE=%s/FontDataBase\n", DATADIR);
    fprintf(fi, "ENCODING=%s:.\n", DATADIR);
    fprintf(fi, "AFM=%s:.\n", DATADIR);
    fprintf(fi, "TYPE1=%s:.\n", DATADIR);
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

    pdfswf_close();
    return 0;
}


