/* swfstrings.c
   Scans a swf file for strings

   Part of the swftools package.
   
   Copyright (c) 2000,2001 Rainer Böhme <rfxswf@reflex-studio.de>

   This file is distributed under the GPL, see file COPYING for details */

#include <stdio.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"

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

  if (argc>1)
  { f = open(argv[1],O_RDONLY);
    if (f>=0)
    { if FAILED(ReadSWF(f,&swf))
      { fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",argv[1]);
        close(f);
      }
      else
      { close(f);
        FontEnumerate(&swf,&fontcallback);
        FreeTags(&swf);
      }
    } else fprintf(stderr,"File not found: %s\n",argv[1]);
  }
  else fprintf(stderr,"\nreflex SWF Text Scan Utility\n(w) 2000 by Rainer Boehme <rb@reflex-studio.de>\n\nUsage: %s filename.swf\n", argv[0]);
  
  return 0;
}

