/* as3compile.c
   Compiles ActionScript 3.0 (.as) files into .swf files.

   Part of the swftools package.

   Copyright (c) 2008/2009 Matthias Kramm <kramm@quiss.org>

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
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "../lib/q.h"
#include "../lib/os.h"

static char * filename = 0;
static char * outputname = 0;
static int override_outputname = 0;
static int do_cgi = 0;
static double framerate = 25.0;
static double width = 400;
static double height = 300;
static int flashversion = 9;
static int verbose = 1;
static char local_with_networking = 0;
static char local_with_filesystem = 0;
static char*mainclass = 0;

static struct options_t options[] = {
{"h", "help"},
{"V", "version"},
{"v", "verbose"},
{"q", "quiet"},
{"C", "cgi"},
{"R", "resolve"},
{"D", "define"},
{"X", "width"},
{"Y", "height"},
{"r", "rate"},
{"M", "mainclass"},
{"l", "library"},
{"I", "include"},
{"N", "local-with-network"},
{"L", "local-with-filesystem"},
{"T", "flashversion"},
{"o", "output"},
{0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfc - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    }
    else if(!strcmp(name, "o")) {
	outputname = val;
	override_outputname = 1;
	return 1;
    }
    else if(!strcmp(name, "r")) {
	framerate = atof(val);
        return 1;
    }
    else if(!strcmp(name, "M")) {
        mainclass = val;
        return 1;
    }
    else if(!strcmp(name, "v")) {
	verbose++;
        return 0;
    }
    else if(!strcmp(name, "q")) {
	verbose--;
        return 0;
    }
    else if(!strcmp(name, "X")) {
	width = atof(val);
        return 1;
    }
    else if(!strcmp(name, "Y")) {
	height = atof(val);
        return 1;
    }
    else if(!strcmp(name, "T")) {
        flashversion = atoi(val);
        return 1;
    }
    else if(!strcmp(name, "C")) {
	do_cgi = 1;
	return 0;
    }
    else if(!strcmp(name, "l")) {
        as3_import_file(val);
	return 1;
    }
    else if(!strcmp(name, "I")) {
        as3_add_include_dir(val);
	return 1;
    }
    else if(!strcmp(name, "R")) {
        as3_set_option("recurse","1");
	return 0;
    }
    else if(!strcmp(name, "D")) {
        if(!strstr(val, "::")) {
            fprintf(stderr, "Error: compile definition must contain \"::\"\n");
            exit(1);
        }
        as3_set_define(val);
	return 1;
    }
    else if (!strcmp(name, "N"))
    {
	local_with_networking = 1;
	return 0;
    }
    else if (!strcmp(name, "L"))
    {
	local_with_filesystem = 1;
	return 0;
    }
    else {
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
    printf("Usage: %s file.as [-o file.swf] \n", name);
    printf("\n");
    printf("-h , --help                    Print short help message and exit\n");
    printf("-V , --version                 Print version info and exit\n");
    printf("-v , --verbose                 Increase verbosity\n");
    printf("-q , --quiet                   Decrease verbosity\n");
    printf("-C , --cgi                     Output to stdout (for use in CGI environments)\n");
    printf("-R , --resolve                 Try to resolve undefined classes automatically.\n");
    printf("-D , --define <namespace::variable>    Set a compile time variable (for doing conditional compilation)\n");
    printf("-X , --width                   Set target SWF width\n");
    printf("-Y , --height                  Set target SWF width\n");
    printf("-r , --rate                    Set target SWF framerate\n");
    printf("-M , --mainclass               Set the name of the main class (extending flash.display.MovieClip)\n");
    printf("-l , --library <file>          Include library file <file>. <file> can be an .abc or .swf file.\n");
    printf("-I , --include <dir>           Add additional include dir <dir>.\n");
    printf("-N , --local-with-network      Make output file \"local with networking\"\n");
    printf("-L , --local-with-filesystem     Make output file \"local with filesystem\"\n");
    printf("-T , --flashversion <num>      Set target SWF flash version to <num>.\n");
    printf("-o , --output <filename>       Set output file to <filename>.\n");
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

void writeSWF(SWF*swf)
{
    int fi = -1;
    if(do_cgi || !strcmp(filename, "-"))
	fi = fileno(stdout);
    else
	fi = open(outputname, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
    if(fi<0) {
	fprintf(stderr, "couldn't create output file %s\n", filename);
        exit(1);
    }
    if(do_cgi) {
        if(swf_WriteCGI(swf)<0) {
            fprintf(stderr, "WriteCGI failed.\n");
            exit(1);
        }
    } else {
	if(swf_WriteSWF(fi, swf)<0) {
            fprintf(stderr, "WriteSWF() failed.\n");
            exit(1);
        }
    }
}

int main (int argc,char ** argv)
{
    char buf[512];
    char*currentdir = getcwd(buf, 512);
    if(!currentdir) {
        as3_warning("Could not determine the current directory");
    } else {
        as3_add_include_dir(currentdir);
    }

    int t;
    processargs(argc, argv);
    as3_setverbosity(verbose);

    if(!filename) {
	args_callback_usage(argv[0]);
	exit(1);
    }
    if(!outputname) {
	outputname = stripFilename(filename, ".swf");
        //as3_warning("output name not given, writing to %s", outputname);
    }

    if(!strcmp(filename, ".")) {
        as3_parse_directory(".");
    } else {
        as3_parse_file(filename);
    }

    void*code = as3_getcode();

    SWF swf;
    memset(&swf, 0, sizeof(swf));
    swf.fileVersion = flashversion;
    swf.frameRate = framerate*0x100;
    swf.movieSize.xmin = 0;
    swf.movieSize.ymin = 0;
    swf.movieSize.xmax = width*20;
    swf.movieSize.ymax = height*20;
    TAG*tag = swf.firstTag = swf_InsertTag(0, ST_DOABC);
    swf_WriteABC(tag, code);

    if(!mainclass)
        mainclass = as3_getglobalclass();
    if(mainclass) {
        tag = swf_InsertTag(tag, ST_SYMBOLCLASS);
        swf_SetU16(tag, 1);
        swf_SetU16(tag, 0);
        swf_SetString(tag, mainclass);
    } else {
        as3_warning("no global public MovieClip subclass");
    }
    
    as3_destroy();

    tag = swf_InsertTag(tag, ST_SHOWFRAME);
    tag = swf_InsertTag(tag, ST_END);

    swf_FreeABC(code);
    
    if(local_with_filesystem)
        swf.fileAttributes &= ~FILEATTRIBUTE_USENETWORK;
    if(local_with_networking)
        swf.fileAttributes |= FILEATTRIBUTE_USENETWORK;

    writeSWF(&swf);
    swf_FreeTags(&swf);

    return 0;
}

