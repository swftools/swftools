/* swfstrings.c
   Scans a swf file for strings

   Part of the swftools package.
   
   Copyright (c) 2000,2001 Rainer Böhme <rfxswf@reflex-studio.de>

   This file is distributed under the GPL, see file COPYING for details */

#include <stdio.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"

char * filename = 0;

struct options_t options[] =
{
 {"v","verbose"},
 {"V","version"},
 {0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
	printf("swfstrings - part of %s %s\n", PACKAGE, VERSION);
	exit(0);
    }
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char*name)
{    
    printf("\nreflex SWF Text Scan Utility\n(w) 2000 by Rainer Boehme <rb@reflex-studio.de>\n\nUsage: %s filename.swf\n", name);
    exit(0);
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

SWF swf;
  
void fontcallback(U16 id,U8 * name)
{ LPSWFFONT font;
  LPTAG t;
  
  FontExtract(&swf,id,&font);
  printf("#< %s %s %s>\n",name,FontIsBold(font)?"bold":"",FontIsItalic(font)?"italic":"");

  t = swf.FirstTag;

  while (t)
  { TextPrintDefineText(t,font);
    t = NextTag(t);
  }
  
  FontFree(font);
}

int main (int argc,char ** argv)
{ int f;

  processargs(argc, argv);
  if(!filename)
      exit(0);

  f = open(filename,O_RDONLY);
  if (f>=0)
  { if FAILED(ReadSWF(f,&swf))
    { fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",filename);
      close(f);
    }
    else
    { close(f);
      FontEnumerate(&swf,&fontcallback);
      FreeTags(&swf);
    }
  } else {
      fprintf(stderr,"File not found: %s\n",argv[1]);
  }
  
  return 0;
}

