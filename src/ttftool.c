/* ttftool.c

   Truetype utility.
   
   Part of the swftools package.

   Copyright (c) 2010 Matthias Kramm <kramm@quiss.org>
 
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "../lib/args.h"
#include "../lib/ttf.h"

static char * filename = 0;
static int showname = 0;
static int verbose = 0;

static struct options_t options[] = {
{"n", "name"},
{0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("ttftool - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } else if(!strcmp(name, "n")) {
	showname = 1;
	return 0;
    } else if(!strcmp(name, "v")) {
	verbose ++;
	return 0;
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
    printf("Usage: %s <fontfile>\n", name);
    printf("\n");
    printf("-h , --help                    Print short help message and exit\n");
    printf("-v , --verbose                 Be verbose. Use more than one -v for greater effect.\n");
    printf("-o , --output <filename>       Write output to file <filename>.\n");
    printf("-V , --version                 Print version info and exit\n");
    printf("\n");
}
int args_callback_command(char*name,char*val)
{
    if(filename) {
        fprintf(stderr, "Please specify only one font\n");
        exit(1);
    }
    filename = name;
    return 0;
}


int main(int argc, char ** argv)
{
    processargs(argc, argv);
    ttf_t* font = ttf_open(filename);
    if(showname && font->full_name) {
	printf("%s\n", font->full_name);
    }
    return 0;
}

