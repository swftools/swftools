/* XMLOutputDev.cc

   This file is part of swftools.

   Swftools is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Swftools is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with swftools; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "../../config.h"
#include <stdio.h>
#include <stdlib.h>
#include "XMLOutputDev.h"
#include "GfxState.h"
#include "popplercompat.h"
#ifndef HAVE_POPPLER
  #include "gfile.h"
#endif

XMLOutputDev::XMLOutputDev(char*filename)
:TextOutputDev(mktmpname(0), false, false, false)
{
  out = fopen(filename, "wb");
  if(!out) {
      perror(filename);
      exit(-1);
  }
  fprintf(out, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
  fprintf(out, "<document>\n");
}

XMLOutputDev::~XMLOutputDev()
{
  fprintf(out, "</document>\n");
  fclose(out);
}

void XMLOutputDev::startPage(int pageNum, GfxState *state)
{
    TextOutputDev::startPage(pageNum, state);
    fprintf(out, "<page nr=\"%d\" width=\"%.0f\" height=\"%.0f\">\n", pageNum,
	state->getPageWidth(), state->getPageHeight());
}

void XMLOutputDev::endPage()
{
    TextOutputDev::endPage();
    TextWordList* list = makeWordList();
    int len = list->getLength();
    int i;

    char textTag = 0;
    GString*fontname = new GString();
    double fontsize = -99999;
    double base = -9999;
    double color_r = -1;
    double color_g = -1;
    double color_b = -1;
    for(i=0;i<len;i++) {
	TextWord*word = list->get(i);
	GString*newfont = word->getFontName();
	double newsize = word->getFontSize();
#ifdef HAVE_POPPLER
	double newbase = word->getBaseline();
#else
	double newbase = word->base;
#endif
	double newcolor_r;
	double newcolor_g;
	double newcolor_b;
  word->getColor(&newcolor_r, &newcolor_g, &newcolor_b);

	if((newfont && newfont->cmp(fontname)) || 
	   newsize != fontsize ||
	   newbase != base ||
	   newcolor_r != color_r ||
	   newcolor_g != color_g ||
	   newcolor_b != color_b
	   ) 
	{
	    TextFontInfo*info = word->getFontInfo();
	    if(textTag)
		fprintf(out, "</t>\n");
	    textTag = 1;
	    GBool italic = gFalse;
	    GBool bold = gFalse;
	    GBool serif = gFalse;

	    if(info->isItalic()) italic = gTrue;
	    if(info->isBold()) bold = gTrue;
	    if(info->isSerif()) serif = gTrue;
	    char*name = (char*)"";
	    if(newfont) {
		name = newfont->lowerCase()->getCString();
		if(strlen(name)>7 && name[6]=='+')
		    name += 7;
		if(strstr(name, "ital")) italic = gTrue;
		if(strstr(name, "slan")) italic = gTrue;
		if(strstr(name, "obli")) italic = gTrue;
		if(strstr(name, "bold")) bold = gTrue;
		if(strstr(name, "heav")) bold = gTrue;
		if(strstr(name, "medi")) bold = gTrue;
		if(strstr(name, "serif")) serif = gTrue;
	    }

    double xMin,yMin,xMax,yMax;
    word->getBBox(&xMin, &yMin, &xMax, &yMax);

    int rot = word->getRotation();

	    fprintf(out, "<t font=\"%s\" y=\"%f\" x=\"%f\" bbox=\"%f:%f:%f:%f\" style=\"%s%s%s%s\" fontsize=\"%.0fpt\" color=\"%02x%02x%02x\">", 
		    name,
		    newbase,
		    (rot&1)?yMin:xMin,
		    (rot&1)?yMin:xMin,
		    (rot&1)?xMin:yMin,
		    (rot&1)?yMax:xMax,
		    (rot&1)?xMax:yMax,
		    info->isFixedWidth()?"fixed;":"",
		    serif?"serif;":"",
		    italic?"italic;":"",
		    bold?"bold;":"",
		    newsize,
		    ((int)(newcolor_r*255))&0xff,
		    ((int)(newcolor_g*255))&0xff,
		    ((int)(newcolor_b*255))&0xff
		    );
	    fontname = newfont->copy();
	    fontsize = newsize;
	    base = newbase;
	    color_r = newcolor_r;
	    color_g = newcolor_g;
	    color_b = newcolor_b;
	}
	char*s = word->getText()->getCString();
	while(*s) {
	    switch(*s) {
		case '<': fprintf(out, "&lt;");break;
		case '>': fprintf(out, "&gt;");break;
		case '&': fprintf(out, "&amp;");break;
		default: fwrite(s, 1, 1, out);
	    }
	    s++;
	}
	if(word->getSpaceAfter())
	    fprintf(out, " ");
    }
    if(textTag) fprintf(out, "</t>\n");
    fprintf(out, "</page>\n");
}

