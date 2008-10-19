/* swfdump.c

   Dump / debug functions
   
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
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
#include "../rfxswf.h"

void swf_DumpHeader(FILE * f,SWF * swf)
{ if (!f) f = stderr;
  fprintf(f,"File size\t%u\n",swf->fileSize);
  fprintf(f,"Movie width\t%u\n",(swf->movieSize.xmax - swf->movieSize.xmin)/20);
  fprintf(f,"Movie height\t%u\n",(swf->movieSize.ymax - swf->movieSize.ymin)/20);
  fprintf(f,"Frame rate\t%u.%u\n",swf->frameRate>>8,swf->frameRate&0xff);
  fprintf(f,"Frame count\t%u\n",swf->frameCount);
}

void swf_DumpMatrix(FILE * f,MATRIX * m)
{ if (!f) f = stderr;
  /*fprintf(f,"[%08x][%08x]\n",m->sx,m->r1);
  fprintf(f,"[%08x][%08x]\n",m->r0,m->sy);
  fprintf(f," %08x, %08x\n",m->tx,m->ty);*/
  fprintf(f,"[%08x][%08x] %5.2f %5.2f %5.2f\n",m->sx,m->r1, m->sx/65536.0,m->r1/65536.0, m->tx/20.0);
  fprintf(f,"[%08x][%08x] %5.2f %5.2f %5.2f\n",m->r0,m->sy, m->r0/65536.0,m->sy/65536.0, m->ty/20.0 );
  fprintf(f," %08x, %08x \n",m->tx,m->ty);
}

void swf_DumpGradient(FILE * f,GRADIENT * g)
{ if (!f) f = stderr;
    fprintf(f, "%d gradient steps\n", g->num);
    int t;
    for(t=0;t<g->num;t++) {
	RGBA c = g->rgba[t];
	fprintf(f, "%d) %02x%02x%02x%02x at %d\n", t, c.r,c.g,c.b,c.a, g->ratios[t]);
    }
}

void swf_DumpTag(FILE * f,TAG * t)
{ int i;
  if (!f) f = stderr;
  for (i=0;i<t->len;i++)
  { if (!(i&15)) fprintf(f,"\n");
    fprintf(f,"%02x ",t->data[i]);
  }
  fprintf(f,"\n");
}

void swf_DumpSWF(FILE * f, SWF*swf)
{
    TAG* tag = swf->firstTag;
    fprintf(f, "vvvvvvvvvvvvvvvvvvvvv\n");
    while(tag) {
	printf("%8d %s\n", tag->len, swf_TagGetName(tag));
	tag = tag->next;
    }
    fprintf(f, "^^^^^^^^^^^^^^^^^^^^^\n");
}

void swf_DumpFont(SWFFONT * font)
{
    printf("ID: %d\n", font->id);
    printf("Version: %d\n", font->version);
    printf("name: %s\n", font->name);
    printf("characters: %d\n", font->numchars);
    printf("biggest mapped ascii value: %d\n", font->maxascii);
    printf("layout: %s\n", font->layout?"yes":"no");
    if(font->layout)
    {
	printf(" ascent:%d\n", font->layout->ascent);
	printf(" descent:%d\n", font->layout->descent);
	printf(" leading:%d\n", font->layout->leading);
	printf(" bounds: (not shown)\n");
	printf(" kerning records:%d\n", font->layout->kerningcount);
	printf(" kerning records: (not shown)\n");
    }
    printf("style: %d\n", font->style);
    printf("encoding: %d\n", font->encoding);
    printf("language: %d\n", font->language);
}

void swf_DumpShape(SHAPE2*shape2)
{
    SHAPELINE*l = shape2->lines;
    while(l) {
	if(l->type == moveTo) {
	    //printf("fill %d/%d line %d\n", l->fillstyle0, l->fillstyle1, l->linestyle);
	    printf("moveTo %.2f,%.2f (fill0:%d fill1:%d line:%d)\n", l->x/20.0, l->y/20.0, l->fillstyle0, l->fillstyle1, l->linestyle);
	}
	if(l->type == lineTo) {
	    //printf("fill %d/%d line %d\n", l->fillstyle0,  l->fillstyle1, l->linestyle);
	    printf("lineTo %.2f,%.2f (fill0:%d fill1:%d line:%d)\n", l->x/20.0, l->y/20.0, l->fillstyle0, l->fillstyle1, l->linestyle);
	}
	if(l->type == splineTo) {
	    //printf("fill %d/%d line %d\n", l->fillstyle0, l->fillstyle1, l->linestyle);
	    printf("splineTo %.2f,%.2f %.2f,%.2f (fill0:%d fill1:%d line:%d)\n", l->sx/20.0, l->sy/20.0, l->x/20.0, l->y/20.0, l->fillstyle0, l->fillstyle1, l->linestyle);
	}
	l = l->next;
    }
}

char* swf_TagGetName(TAG*tag)
{
    switch(tag->id)
    {
        case ST_END:
            return "END";
        case ST_SHOWFRAME:
            return "SHOWFRAME";
        case ST_DEFINESHAPE:
            return "DEFINESHAPE";
        case ST_FREECHARACTER:
            return "FREECHARACTER";
        case ST_PLACEOBJECT:
            return "PLACEOBJECT";
        case ST_REMOVEOBJECT:
            return "REMOVEOBJECT";
        case ST_DEFINEBITSJPEG:
            return "DEFINEBITSJPEG";
        case ST_DEFINEBUTTON:
            return "DEFINEBUTTON";
        case ST_JPEGTABLES:
            return "JPEGTABLES";
        case ST_SETBACKGROUNDCOLOR:
            return "SETBACKGROUNDCOLOR";
        case ST_DEFINEFONT:
            return "DEFINEFONT";
        case ST_DEFINETEXT:
            return "DEFINETEXT";
        case ST_DEFINEEDITTEXT:
            return "DEFINEEDITTEXT";
        case ST_DOACTION:
            return "DOACTION";
        case ST_DOABC:
            return "DOABC";
        case ST_RAWABC:
            return "RAWABC";
        case ST_DEFINEFONTINFO:
            return "DEFINEFONTINFO";
        case ST_DEFINESOUND:
            return "DEFINESOUND";
        case ST_STARTSOUND:
            return "STARTSOUND";
        case ST_DEFINEBUTTONSOUND:
            return "DEFINEBUTTONSOUND";
        case ST_SOUNDSTREAMHEAD:
            return "SOUNDSTREAMHEAD";
        case ST_SOUNDSTREAMBLOCK:
            return "SOUNDSTREAMBLOCK";
        case ST_DEFINEBITSLOSSLESS:
            return "DEFINEBITSLOSSLESS";
        case ST_DEFINEBITSJPEG2:
            return "DEFINEBITSJPEG2";
        case ST_DEFINESHAPE2:
            return "DEFINESHAPE2";
        case ST_DEFINEBUTTONCXFORM:
            return "DEFINEBUTTONCXFORM";
        case ST_PROTECT:
            return "PROTECT";
        case ST_PLACEOBJECT2:
            return "PLACEOBJECT2";
        case ST_REMOVEOBJECT2:
            return "REMOVEOBJECT2";
        case ST_DEFINESHAPE3:
            return "DEFINESHAPE3";
        case ST_DEFINETEXT2:
            return "DEFINETEXT2";
        case ST_DEFINEBUTTON2:
            return "DEFINEBUTTON2";
        case ST_DEFINEBITSJPEG3:
            return "DEFINEBITSJPEG3";
        case ST_DEFINEBITSLOSSLESS2:
            return "DEFINEBITSLOSSLESS2";
        case ST_DEFINESPRITE:
            return "DEFINESPRITE";
        case ST_NAMECHARACTER:
            return "NAMECHARACTER";
        case ST_SERIALNUMBER:
            return "SERIALNUMBER";
        case ST_GENERATORTEXT:
            return "GENERATORTEXT";
        case ST_FRAMELABEL:
            return "FRAMELABEL";
        case ST_SOUNDSTREAMHEAD2:
            return "SOUNDSTREAMHEAD2";
        case ST_DEFINEMORPHSHAPE:
            return "DEFINEMORPHSHAPE";
        case ST_DEFINEMORPHSHAPE2:
            return "DEFINEMORPHSHAPE2";
        case ST_DEFINEFONT2:
            return "DEFINEFONT2";
        case ST_TEMPLATECOMMAND:
            return "TEMPLATECOMMAND";
        case ST_GENERATOR3:
            return "GENERATOR3";
        case ST_EXTERNALFONT:
            return "EXTERNALFONT";
	case ST_EXPORTASSETS:
	    return "EXPORTASSETS";
	case ST_SYMBOLCLASS:
	    return "SYMBOLCLASS";
	case ST_DEFINEBINARY:
	    return "DEFINEBINARY";
	case ST_IMPORTASSETS:
	    return "IMPORTASSETS";
	case ST_ENABLEDEBUGGER:
	    return "ENABLEDEBUGGER";
        case ST_DOINITACTION:
            return "DOINITACTION";
        case ST_DEFINEMOVIE:
            return "DEFINEMOVIE";
        case ST_DEFINEVIDEOSTREAM:
            return "DEFINEVIDEOSTREAM";
        case ST_VIDEOFRAME:
            return "VIDEOFRAME";
        case ST_DEFINEFONTINFO2:
            return "DEFINEFONTINFO2";
        case ST_FILEATTRIBUTES:
            return "FILEATTRIBUTES";
        case ST_IMPORTASSETS2:
            return "IMPORTASSETS2";
        case ST_DEFINESCALINGGRID:
            return "DEFINESCALINGGRID";
        case ST_DEFINESHAPE4:
            return "DEFINESHAPE4";
        case ST_DEFINEFONT3:
            return "DEFINEFONT3";
        case ST_DEFINEFONTALIGNZONES:
            return "DEFINEFONTALIGNZONES";
        case ST_DEFINEFONTNAME:
            return "DEFINEFONTNAME";
        case ST_CSMTEXTSETTINGS:
            return "CSMTEXTSETTINGS";
        case ST_FREEALL:
            return "FREEALL";
        case ST_MX4:
            return "MX4";
        case ST_SCRIPTLIMITS:
            return "SCRIPTLIMITS";
        case ST_SETTABINDEX:
            return "SETTABINDEX";
	case ST_ENABLEDEBUGGER2:
	    return "ENABLEDEBUGGER2";
	case ST_PLACEOBJECT3:
	    return "PLACEOBJECT3";
	case ST_METADATA:
	    return "METADATA";
	case ST_SCENEDESCRIPTION:
	    return "SCENEDESCRIPTION";
        
	case ST_REFLEX:
            return "REFLEX";
	case ST_GLYPHNAMES:
            return "GLYPHNAMES";
    }
    return 0;
}
