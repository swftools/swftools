/* swfstrings.c
   Scans a swf file for strings

   Part of the swftools package.
   
   Copyright (c) 2000,2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
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
    return 0;
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
  
void fontcallback(void*self,U16 id,U8 * name)
{ SWFFONT* font;
  TAG* t;
  
  swf_FontExtract(&swf,id,&font);
  printf("#< %s %s %s>\n",name,swf_FontIsBold(font)?"bold":"",swf_FontIsItalic(font)?"italic":"");

  t = swf.firstTag;

  while (t)
  { swf_TextPrintDefineText(t,font);
    t = swf_NextTag(t);
  }
  
  swf_FontFree(font);
}

int main (int argc,char ** argv)
{ int f;

  processargs(argc, argv);
  if(!filename)
      exit(0);

  f = open(filename,O_RDONLY|O_BINARY);
  if (f>=0)
  { if FAILED(swf_ReadSWF(f,&swf))
    { fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",filename);
      close(f);
    }
    else
    { close(f);
      swf_FontEnumerate(&swf,&fontcallback,0);
      swf_FreeTags(&swf);
    }
  } else {
      fprintf(stderr,"File not found: %s\n",argv[1]);
  }
  
  return 0;
}

