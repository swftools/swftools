/* args.h
   Settings for swfcombine. This is deliberately not named "config.h" to
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
   int loglevel;
   int movex;
   int movey;
   float scalex;
   float scaley;
};
extern struct config_t config;
#endif
