/* swfdump.c

   Dump / debug functions
   
   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

void DumpHeader(FILE * f,SWF * swf)
{ if (!f) f = stderr;
  fprintf(f,"File size\t%u\n",swf->FileSize);
  fprintf(f,"Movie width\t%u\n",(swf->MovieSize.xmax - swf->MovieSize.xmin)/20);
  fprintf(f,"Movie height\t%u\n",(swf->MovieSize.ymax - swf->MovieSize.ymin)/20);
  fprintf(f,"Frame rate\t%u.%u\n",swf->FrameRate>>8,swf->FrameRate&0xff);
  fprintf(f,"Frame count\t%u\n",swf->FrameCount);
}

void DumpMatrix(FILE * f,MATRIX * m)
{ if (!f) f = stderr;
  fprintf(f,"[%08x][%08x]\n",m->sx,m->r1);
  fprintf(f,"[%08x][%08x]\n",m->r0,m->sy);
  fprintf(f," %08x, %08x\n",m->tx,m->ty);
}

void DumpTag(FILE * f,TAG * t)
{ int i;
  if (!f) f = stderr;
  for (i=0;i<t->len;i++)
  { if (!(i&15)) fprintf(f,"\n");
    fprintf(f,"%02x ",t->data[i]);
  }
  fprintf(f,"\n");
}

char* getTagName(TAG*tag)
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
	case ST_DEFINEBITS:
	    return "DEFINEBITS";
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
	case ST_DOACTION:
	    return "DOACTION";
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
	case ST_DEFINEFONT2:
	    return "DEFINEFONT2";
	case ST_TEMPLATECOMMAND:
	    return "TEMPLATECOMMAND";
	case ST_GENERATOR3:
	    return "GENERATOR3";
	case ST_EXTERNALFONT:
	    return "EXTERNALFONT";
	case ST_REFLEX:
	    return "REFLEX";
    }
    return 0;
}
