/* args.h
   Settings for swfcombine. This file is deliberately not named "config.h" to
   avoid conflicts with GNU autoconf.

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __config_h__
#define __config_h__
struct config_t
{
   char overlay;
   char alloctest;
   char clip;
   char stack;
   char antistream;
   char dummy;
   int loglevel;
   int movex;
   int movey;
   int sizex;
   char hassizex;
   int sizey;
   char hassizey;
   int framerate;
   float scalex;
   float scaley;
};
extern struct config_t config;
#endif
