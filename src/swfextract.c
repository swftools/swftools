/* swfextract.c
   Allows to extract parts of the swf into a new file.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>
 
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

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "../lib/log.h"
#include "../lib/jpeg.h"
#include "../lib/png.h"
#ifdef HAVE_ZLIB_H
#ifdef HAVE_LIBZ
#include "zlib.h"
#define _ZLIB_INCLUDED_
#endif
#endif

char * filename = 0;
char * destfilename = "output.swf";
int verbose = 3;

char* extractids = 0;
char* extractframes = 0;
char* extractjpegids = 0;
char* extractfontids = 0;
char* extractpngids = 0;
char* extractsoundids = 0;
char* extractmp3ids = 0;
char* extractbinaryids = 0;
char* extractanyids = 0;
char extractmp3 = 0;

char* extractname = 0;

char hollow = 0;
char originalplaceobjects = 0;
char movetozero = 0;

int numextracts = 0;
char *outputformat = NULL;

struct options_t options[] =
{
 {"o","output"},
 {"w","hollow"},
 {"v","verbose"},
 {"i","id"},
 {"j","jpegs"},
 {"p","pngs"},
 {"a","any"},
 {"P","placeobject"},
 {"0","movetozero"},
 {"m","mp3"},
 {"M","embeddedmp3"},
 {"s","sound"},
 {"n","name"},
 {"f","frame"},
 {"F","font"},
 {"V","version"},
 {"b","binary"},
 {"O","outputformat"},
 {0,0}
};


int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfextract - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } 
    else if(!strcmp(name, "o")) {
	destfilename = val;
	return 1;
    } 
    else if(!strcmp(name, "i")) {
	extractids = val;
	numextracts++;
	if(extractname) {
	    fprintf(stderr, "You can only supply either name or id\n");
	    exit(1);
	}
	return 1;
    } 
    else if(!strcmp(name, "n")) {
	extractname = val;
	numextracts++;
	if(extractids) {
	    fprintf(stderr, "You can only supply either name or id\n");
	    exit(1);
	}
	return 1;
    } 
    else if(!strcmp(name, "v")) {
	verbose ++;
	return 0;
    } 
    else if(!strcmp(name, "m")) {
	extractmp3 = 1;
	numextracts++;
	return 0;
    }
    else if(!strcmp(name, "M")) {
	if(extractsoundids) {
	    fprintf(stderr, "Only one --embeddedmp3 argument is allowed. (Try to use a range, e.g. -M 1,2,3)\n");
	    exit(1);
	}
	numextracts++;
	extractmp3ids = val;
	return 1;
    } 
    else if(!strcmp(name, "j")) {
	if(extractjpegids) {
	    fprintf(stderr, "Only one --jpegs argument is allowed. (Try to use a range, e.g. -j 1,2,3)\n");
	    exit(1);
	}
	/* TODO: count number of IDs in val range */
	numextracts++;
	extractjpegids = val;
	return 1;
    } 
    else if(!strcmp(name, "F")) {
	if(extractfontids) {
	    fprintf(stderr, "Only one --font argument is allowed. (Try to use a range, e.g. -s 1,2,3)\n");
	    exit(1);
	}
	numextracts++;
	extractfontids = val;
	return 1;
    } 
    else if(!strcmp(name, "s")) {
	if(extractsoundids) {
	    fprintf(stderr, "Only one --sound argument is allowed. (Try to use a range, e.g. -s 1,2,3)\n");
	    exit(1);
	}
	numextracts++;
	extractsoundids = val;
	return 1;
    } 
    else if(!strcmp(name, "b")) {
	if(extractbinaryids) {
	    fprintf(stderr, "Only one --binary argument is allowed. (Try to use a range, e.g. -s 1,2,3)\n");
	    exit(1);
	}
	numextracts++;
	extractbinaryids = val;
	return 1;
    } 
#ifdef _ZLIB_INCLUDED_
    else if(!strcmp(name, "p")) {
	if(extractpngids) {
	    fprintf(stderr, "Only one --png argument is allowed. (Try to use a range, e.g. -p 1,2,3)\n");
	    exit(1);
	}
	numextracts++;
	extractpngids = val;
	return 1;
    } 
#endif
    else if(!strcmp(name, "a")) {
	numextracts++;
	extractanyids = val;
	return 1;
    }
    else if(!strcmp(name, "f")) {
	numextracts++;
	extractframes = val;
	return 1;
    }
    else if(!strcmp(name, "P")) {
	originalplaceobjects = 1;
	return 0;
    }
    else if(!strcmp(name, "0")) {
	movetozero = 1;
	return 0;
    }
    else if(!strcmp(name, "w")) {
	hollow = 1;
	return 0;
    }
    else if (!strcmp(name, "O")) {
      outputformat = val;
	return 1;
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
void args_callback_usage(char*name)
{    
    printf("Usage: %s [-v] [-n name] [-ijf ids] file.swf\n", name);
    printf("\t-v , --verbose\t\t\t Be more verbose\n");
    printf("\t-o , --output filename\t\t set output filename\n");
    printf("\t-V , --version\t\t\t Print program version and exit\n\n");
    printf("SWF Subelement extraction:\n");
    printf("\t-n , --name name\t\t instance name of the object (SWF Define) to extract\n");
    printf("\t-i , --id ID\t\t\t ID of the object, shape or movieclip to extract\n");
    printf("\t-f , --frame frames\t\t frame numbers to extract\n");
    printf("\t-w , --hollow\t\t\t hollow mode: don't remove empty frames\n"); 
    printf("\t             \t\t\t (use with -f)\n");
    printf("\t-P , --placeobject\t\t\t Insert original placeobject into output file\n"); 
    printf("\t             \t\t\t (use with -i)\n");
    printf("SWF Font/Text extraction:\n");
    printf("\t-F , --font ID\t\t\t Extract font(s)\n");
    printf("Picture extraction:\n");
    printf("\t-j , --jpeg ID\t\t\t Extract JPEG picture(s)\n");
#ifdef _ZLIB_INCLUDED_
    printf("\t-p , --pngs ID\t\t\t Extract PNG picture(s)\n");
#endif
    printf("\n");
    printf("Sound extraction:\n");
    printf("\t-m , --mp3\t\t\t Extract main mp3 stream\n");
    printf("\t-M , --embeddedmp3\t\t\t Extract embedded mp3 stream(s)\n");
    printf("\t-s , --sound ID\t\t\t Extract Sound(s)\n");
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

void prepare_name(char *buf, size_t len, const char *prefix, 
		  const char *suffix, int idx) {
  if (outputformat!=NULL) {
    // override default file name formatting
    // make sure single-file behavior is not used
    numextracts = -1;
    // Other parts of codebase use vsnprintf, so I assume snprintf
    // is available on all platforms that swftools currently works on.
    // We need to check for buffer overflows now that the user is 
    // supplying the format string.
    snprintf(buf,len,outputformat,idx,suffix);
  } else {
    // use default file name formatting, unchanged
    sprintf(buf,"%s%d.%s",prefix,idx,suffix);
  }
}

U8 mainr,maing,mainb;
/* 1 = used, not expanded,
   3 = used, expanded
   5 = wanted, not expanded
   7 = wanted, expanded
 */
char used[65536];
TAG*tags[65536];
int changed;
char * tagused;
int extractname_id = -1;

void idcallback(void*data)
{
    if(!(used[GET16(data)]&1)) {
	changed = 1;
	used[GET16(data)] |= 1;
    }
}

void enumerateIDs(TAG*tag, void(*callback)(void*))
{
/*    U8*data;
    int len = tag->len;
    if(tag->len>=64) {
	len += 6;
	data = (U8*)malloc(len);
	PUT16(data, (tag->id<<6)+63);
	*(U8*)&data[2] = tag->len;
	*(U8*)&data[3] = tag->len>>8;
	*(U8*)&data[4] = tag->len>>16;
	*(U8*)&data[5] = tag->len>>24;
	memcpy(&data[6], tag->data, tag->len);
    } else {
	len += 2;
	data = (U8*)malloc(len);
	PUT16(data, (tag->id<<6)+tag->len);
	memcpy(&data[2], tag->data, tag->len);
    }
    map_ids_mem(data, len, callback);
 */
    int num = swf_GetNumUsedIDs(tag);
    int *ptr = malloc(sizeof(int)*num);
    int t;
    swf_GetUsedIDs(tag, ptr);
    for(t=0;t<num;t++)
	callback(&tag->data[ptr[t]]);
}

void moveToZero(TAG*tag)
{
    if(!swf_isPlaceTag(tag))
	return;
    SWFPLACEOBJECT obj;
    swf_GetPlaceObject(tag, &obj);
    obj.matrix.tx = 0;
    obj.matrix.ty = 0;
    swf_ResetTag(tag, tag->id);
    swf_SetPlaceObject(tag, &obj);
}

void extractTag(SWF*swf, char*filename)
{
    SWF newswf;
    TAG*desttag;
    TAG*srctag;
    RGBA rgb;
    SRECT objectbbox;
    char sprite;
    int f;
    int t;
    int tagnum;
    int copy = 0;
    memset(&newswf,0x00,sizeof(SWF));        // set global movie parameters

    newswf.fileVersion    = swf->fileVersion;
    newswf.frameRate      = swf->frameRate;
    newswf.movieSize	  = swf->movieSize;
    if(movetozero && originalplaceobjects) {
	newswf.movieSize.xmax = swf->movieSize.xmax - swf->movieSize.xmin;
	newswf.movieSize.ymax = swf->movieSize.ymax - swf->movieSize.ymin;
	newswf.movieSize.xmin = 0;
	newswf.movieSize.ymin = 0;
    }
		
    newswf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    desttag = newswf.firstTag;
    rgb.r = mainr;
    rgb.g = maing;
    rgb.b = mainb;
    swf_SetRGB(desttag,&rgb);

    swf_GetRect(0, &objectbbox);

    do {
       changed = 0;
       for(t=0;t<65536;t++) {
	   if(used[t] && !(used[t]&2)) {
	     if(tags[t]==0) {
		 msg("<warning> ID %d is referenced, but never defined.", t);
	     } else if(tags[t]->id==ST_DEFINESPRITE) {
		 TAG*tag = tags[t];
		 while(tag->id != ST_END)
		 {
		     enumerateIDs(tag, idcallback);
		     tag = tag->next;
		 }
	     }
	     else 
	       enumerateIDs(tags[t], idcallback);
	     used[t] |= 2;
	   }
       }
    }
    while(changed);

    srctag = swf->firstTag;
    tagnum = 0;
    sprite = 0;
    while(srctag && (srctag->id || sprite)) {
	int reset = 0;
	if(!sprite) {
	    copy = 0;
	}
	if(srctag->id == ST_END) {
	    sprite = 0;
	}
	if(srctag->id == ST_DEFINESPRITE)
	    sprite = 1;
        if(srctag->id == ST_JPEGTABLES)
            copy = 1;
	if(swf_isDefiningTag(srctag)) {
	    int id = swf_GetDefineID(srctag);
	    if(used[id])  {
		SRECT b;
		copy = 1;
		b = swf_GetDefineBBox(srctag);
		swf_ExpandRect2(&objectbbox, &b);
	    }
	} else 
	if ((((swf_isPlaceTag(srctag) && originalplaceobjects)
	      || srctag->id == ST_STARTSOUND) && (used[swf_GetPlaceID(srctag)]&4) ) ||
	      (swf_isPseudoDefiningTag(srctag) && used[swf_GetDefineID(srctag)]) ||
	      (tagused[tagnum])) 
	{
		if(copy == 0)
		    reset = 1;
		copy = 1;
	} 
	if(srctag->id == ST_REMOVEOBJECT) {
	    if(!used[swf_GetPlaceID(srctag)])
		copy = 0;
	}

	if(copy) {
	    TAG*ttag = (TAG*)malloc(sizeof(TAG));
	    desttag = swf_InsertTag(desttag, srctag->id);
	    desttag->len = desttag->memsize = srctag->len;
	    desttag->data = malloc(srctag->len);
	    memcpy(desttag->data, srctag->data, srctag->len);
	    if(movetozero && swf_isPlaceTag(desttag)) {
		moveToZero(desttag);
	    }
	    if(reset)
	        copy = 0;
	}
	
	srctag = srctag->next;
	tagnum ++;
    }
    if(!extractframes && !hollow) {
	if(!originalplaceobjects && (extractids||extractname_id>=0)) {
            int number = 0;
            int id = 0;
	    int t;
            TAG* objtag = 0;
            SRECT bbox;
            memset(&bbox, 0, sizeof(SRECT));
	    if(extractids) {
		for(t=0;t<65536;t++) {
		    if(is_in_range(t, extractids)) {
			id = t;
			number++;
		    }
		}
	    }
            if(number>=2) {
		printf("warning! You should use the -P when extracting multiple objects\n");
	    }

            if(number == 1) {
                /* if there is only one object, we will scale it.
                   So let's figure out its bounding box */
                TAG*tag = swf->firstTag;
                while(tag) {
                    if(swf_isDefiningTag(tag) && tag->id != ST_DEFINESPRITE) {
                        if(swf_GetDefineID(tag) == id)
                            bbox = swf_GetDefineBBox(tag);
                        objtag = tag;
                    }
                    tag = tag->next;
                }
		newswf.movieSize.xmin = 0;
		newswf.movieSize.ymin = 0;
		newswf.movieSize.xmax = 512*20;
		newswf.movieSize.ymax = 512*20;
            } else {
		if((objectbbox.xmin|objectbbox.ymin|objectbbox.xmax|objectbbox.ymax)!=0)
		    newswf.movieSize = objectbbox;
	    }

	    if(extractname_id>=0) {
		desttag = swf_InsertTag(desttag, ST_PLACEOBJECT2);
		swf_ObjectPlace(desttag, extractname_id, extractname_id, 0,0,extractname);
	    } else {
		for(t=0;t<65536;t++) {
		    if(is_in_range(t, extractids)) {
                        MATRIX m;
			desttag = swf_InsertTag(desttag, ST_PLACEOBJECT2);
                        swf_GetMatrix(0, &m);
                        if(objtag) {
                            int width = bbox.xmax - bbox.xmin;
                            int height = bbox.ymax - bbox.ymin;
                            int max = width>height?width:height;
                            m.tx = -bbox.xmin;
                            m.ty = -bbox.ymin;
                            if(max) {
                                m.sx = (512*20*65536)/max;
                                m.sy = (512*20*65536)/max;
                            }
                            //newswf.movieSize = swf_TurnRect(newswf.movieSize, &m);
                        }
			swf_ObjectPlace(desttag, t, t, &m,0,0);
		    }
		}
	    }
	}
	desttag = swf_InsertTag(desttag,ST_SHOWFRAME);
    }
    desttag = swf_InsertTag(desttag,ST_END);
    
    f = open(filename, O_TRUNC|O_WRONLY|O_CREAT|O_BINARY, 0644);
    if FAILED(swf_WriteSWF(f,&newswf)) fprintf(stderr,"WriteSWF() failed.\n");
    close(f);

    swf_FreeTags(&newswf);                       // cleanup
}

int isOfType(int t, TAG*tag)
{
    int show = 0;
    if(t == 0 && (tag->id == ST_DEFINESHAPE ||
	tag->id == ST_DEFINESHAPE2 ||
	tag->id == ST_DEFINESHAPE3)) {
	show = 1;
    }
    if(t==1 && tag->id == ST_DEFINESPRITE) {
	show = 1;
    }
    if(t == 2 && (tag->id == ST_DEFINEBITS ||
	tag->id == ST_DEFINEBITSJPEG2 ||
	tag->id == ST_DEFINEBITSJPEG3)) {
	show = 1;
    }
    if(t == 3 && (tag->id == ST_DEFINEBITSLOSSLESS ||
	tag->id == ST_DEFINEBITSLOSSLESS2)) {
	show = 1;
    }
    if(t == 4 && (tag->id == ST_DEFINESOUND)) {
	show = 1;
    }
    if(t == 5 && (tag->id == ST_DEFINEFONT || tag->id == ST_DEFINEFONT2 || tag->id == ST_DEFINEFONT3)) {
	show = 1;
    }
    if (t== 6 && (tag->id == ST_DEFINEBINARY)) {
        show = 1;
    }
    if (t== 7 && (tag->id == ST_DEFINESPRITE)) {
	int wasFolded = swf_IsFolded(tag);
	TAG *toFold = tag;

	if(wasFolded) 
	    swf_UnFoldSprite(tag);

        while(tag->id != ST_END) { 
	    tag = tag->next;
            if(tag->id == ST_SOUNDSTREAMHEAD || tag->id == ST_SOUNDSTREAMHEAD2) {
                show = 1;
                break;
	    }
	}

	if(wasFolded) 
	    swf_FoldSprite(toFold);
    }

    return show;
}

void listObjects(SWF*swf)
{
    TAG*tag;
    char first;
    int t;
    int frame = 0;
    char*names[] = {"Shape", "MovieClip", "JPEG", "PNG", "Sound", "Font", "Binary", "Embedded MP3"};
    char*options[] = {"-i", "-i", "-j", "-p", "-s", "-F","-b","-M"};
    int mp3=0;
    printf("Objects in file %s:\n",filename);
    swf_FoldAll(swf);
    for(t=0;t<sizeof(names)/sizeof(names[0]);t++) {
	int nr=0;
	int lastid = -2, lastprint=-1;
	int follow=0;
	tag = swf->firstTag;
	first = 1;
	while(tag) {
	    if(tag->id == ST_SOUNDSTREAMHEAD || tag->id == ST_SOUNDSTREAMHEAD2)
		mp3 = 1;
	    if(isOfType(t,tag))
		nr++;
	    tag = tag->next;
	}
	if(!nr)
	    continue;
	
	printf(" [%s] %d %s%s: ID(s) ", options[t], nr, names[t], nr>1?"s":"");

	tag = swf->firstTag;
	while(tag) {
	    char text[80];
	    char show = isOfType(t,tag);
	    int id;
	    if(!show) {
		tag = tag->next;
		continue;
	    }
	    id = swf_GetDefineID(tag);

	    if(id == lastid+1) {
		follow=1;
	    } else {
		if(first || !follow) {
		    if(!first)
			printf(", ");
		    printf("%d", id);
		} else {
		    if(lastprint + 1 == lastid) 
			printf(", %d, %d", lastid, id);
		    else
			printf("-%d, %d", lastid, id);
		}
		lastprint = id;
		first = 0;
		follow = 0;
	    }
	    lastid = id;
	    tag=tag->next;
	}
	if(follow) {
	    if(lastprint + 1 == lastid)
		printf(", %d", lastid);
	    else
		printf("-%d", lastid);
	}
	printf("\n");
    }

    if(frame)
	printf(" [-f] %d Frames: ID(s) 0-%d\n", frame, frame);
    else
	printf(" [-f] 1 Frame: ID(s) 0\n");

    if(mp3)
	printf(" [-m] 1 MP3 Soundstream\n");
}

int handlefont(SWF*swf, TAG*tag)
{
    SWFFONT* f=0;
    U16 id;
    char name[80];
    char*filename = name;

    id = swf_GetDefineID(tag);
    prepare_name(name, sizeof(name), "font", "swf", id);
    if(numextracts==1) {
	filename = destfilename;
    }

    swf_FontExtract(swf, id, &f);
    if(!f) {
	if (!extractanyids) {
	   printf("Couldn't extract font %d\n", id);
 	}
	return 0;
    }

    swf_WriteFont(f, filename);
    swf_FontFree(f);
    return 1;
}

static char has_jpegtables=0;
static U8*jpegtables = 0;
static int jpegtablessize = 0;

void handlejpegtables(TAG*tag)
{
    if(tag->id == ST_JPEGTABLES) {
	jpegtables = tag->data;
	jpegtablessize = tag->len;
	has_jpegtables = 1;
    }
}

FILE* save_fopen(char* name, char* mode)
{
    FILE*fi = fopen(name, mode);
    if(!fi) {
	fprintf(stderr, "Error: Couldn't open %s\n", name);
	exit(1);
    }
    return fi;
}

int findjpegboundary(U8*data, int len)
{
    int t;
    int pos=-1;
    for(t=0;t<len-4;t++) {
	if(data[t  ]==0xff &&
	   data[t+1]==0xd9 &&
	   data[t+2]==0xff &&
	   data[t+3]==0xd8) {
	    pos = t;
	}
    }
    return pos;
}

/* extract jpeg data out of a tag */
int handlejpeg(TAG*tag)
{
    char name[80];
    char*filename = name;
    FILE*fi;
   
    if(tag->id != ST_DEFINEBITSJPEG3) {
	prepare_name(name, sizeof(name), "pic", "jpg", GET16(tag->data));
	if(numextracts==1) {
	    filename = destfilename;
	    if(!strcmp(filename,"output.swf"))
		filename = "output.jpg";
	}
    } else {
	prepare_name(name, sizeof(name), "pic", "png", GET16(tag->data));
	if(numextracts==1) {
	    filename = destfilename;
	    if(!strcmp(filename,"output.swf"))
		filename = "output.png";
	}
    }

    /* swf jpeg images have two streams, which both start with ff d8 and
       end with ff d9. The following code handles sorting the middle
       <ff d9 ff d8> bytes out, so that one stream remains */
    if(tag->id == ST_DEFINEBITSJPEG && tag->len>2 && has_jpegtables) {
	fi = save_fopen(filename, "wb");
	if(jpegtablessize>=2) {
	    fwrite(jpegtables, 1, jpegtablessize-2, fi); //don't write end tag (ff,d8)
	    fwrite(&tag->data[2+2], tag->len-2-2, 1, fi); //don't write start tag (ff,d9)
	} else {
	    fwrite(tag->data+2, tag->len-2, 1, fi);
	}
	fclose(fi);
    }
    else if(tag->id == ST_DEFINEBITSJPEG2 && tag->len>2) {
	int end = tag->len;
	int pos = findjpegboundary(&tag->data[2], tag->len-2);
	if(pos>=0) {
            int skip1=0;
            int skip2=0;
            pos+=2;
            fi = save_fopen(filename, "wb");
            if (end-pos > 8 && tag->data[pos+4] == 0xff && tag->data[pos+5] == 0xe0) { // app0 in second? move it
                int len = 2+(tag->data[pos+6]<<8)+tag->data[pos+7];
                if (pos+4+len <= end && tag->data[0] == 0xff && tag->data[1] == 0xd8) {
                    skip1 = 2;
                    skip2 = len;
                    fwrite(&tag->data[2], skip1, 1, fi); // write soi
                    fwrite(&tag->data[pos+4], skip2, 1, fi); // write app0
                }
            }
            fwrite(&tag->data[2+skip1], pos-2-skip1, 1, fi);
            fwrite(&tag->data[pos+4+skip2], end-(pos+4+skip2), 1, fi);
            fclose(fi);
        } else {
            fi = save_fopen(filename, "wb");
            fwrite(&tag->data[2], end-2, 1, fi);
            fclose(fi);
        }
    }
    else if(tag->id == ST_DEFINEBITSJPEG3 && tag->len>6) {
	U32 end = GET32(&tag->data[2])+6;
	int pos = findjpegboundary(&tag->data[6], end);
	if(end >= tag->len) {
	    msg("<error> zlib data out of bounds in definebitsjpeg3");
	    return 0;
	}
        if(pos) {
	    /* TODO: do we actually need this? */
	    memmove(&tag->data[pos], &tag->data[pos+4], end-(pos+4));
	}
	unsigned char*image;
	unsigned width=0, height=0;
	jpeg_load_from_mem(&tag->data[6], end-6, &image, &width, &height);

	uLongf datalen = width*height;
	Bytef *data = malloc(datalen);
        if(!data) {
            fprintf(stderr, "malloc error datalen:%ld tag->id:%d\n",
                    datalen, tag->id);
            return 0;
        }
	int error = uncompress(data, &datalen, &tag->data[end], (uLong)(tag->len - end));
	if(error != Z_OK) {
	  fprintf(stderr, "Zlib error %d\n", error);
	  return 0;
	}
	int t, size = width*height;
	for(t=0;t<size;t++) {
	    image[t*4+0] = data[t];
	}
	free(data);
	png_write(filename, image, width, height);
	free(image);
    }
    else {
	int id = GET16(tag->data);
	if (!extractanyids) {
	  fprintf(stderr, "Object %d is not a JPEG picture!\n", id);
	  exit(1);
        }
	return 0;
    }
    return 1;
}

#ifdef _ZLIB_INCLUDED_
static U32 mycrc32;

static U32*crc32_table = 0;
static void make_crc32_table(void)
{
  int t;
  if(crc32_table) 
      return;
  crc32_table = (U32*)malloc(1024);

  for (t = 0; t < 256; t++) {
    U32 c = t;
    int s;
    for (s = 0; s < 8; s++) {
      c = (0xedb88320L*(c&1)) ^ (c >> 1);
    }
    crc32_table[t] = c;
  }
}
static inline void png_write_byte(FILE*fi, U8 byte)
{
    fwrite(&byte,1,1,fi);
    mycrc32 = crc32_table[(mycrc32 ^ byte) & 0xff] ^ (mycrc32 >> 8);
}
static void png_start_chunk(FILE*fi, char*type, int len)
{
    U8 mytype[4]={0,0,0,0};
    U32 mylen = BE_32_TO_NATIVE(len);
    memcpy(mytype,type,strlen(type));
    fwrite(&mylen, 4, 1, fi);
    mycrc32=0xffffffff;
    png_write_byte(fi,mytype[0]);
    png_write_byte(fi,mytype[1]);
    png_write_byte(fi,mytype[2]);
    png_write_byte(fi,mytype[3]);
}
static void png_write_bytes(FILE*fi, U8*bytes, int len)
{
    int t;
    for(t=0;t<len;t++)
	png_write_byte(fi,bytes[t]);
}
static void png_write_dword(FILE*fi, U32 dword)
{
    png_write_byte(fi,dword>>24);
    png_write_byte(fi,dword>>16);
    png_write_byte(fi,dword>>8);
    png_write_byte(fi,dword);
}
static void png_end_chunk(FILE*fi)
{
    U32 tmp = BE_32_TO_NATIVE((mycrc32^0xffffffff));
    fwrite(&tmp,4,1,fi);
}


/* extract a lossless image (png) out of a tag 
   This routine was originally meant to be a one-pager. I just
   didn't know png is _that_ much fun. :) -mk
 */
int handlelossless(TAG*tag)
{
    char name[80];
    char*filename = name;
    FILE*fi;
    int width, height;
    int id;
    int t;
    U8 bpp = 1;
    U8 format;
    Bytef* data=0;
    U8* data2=0;
    U8* data3=0;
    uLongf datalen;
    uLongf datalen2;
    U32 datalen3;
    U8 head[] = {137,80,78,71,13,10,26,10};
    int cols;
    char alpha = tag->id == ST_DEFINEBITSLOSSLESS2;
    RGBA* palette;
    int pos;
    int error;

    make_crc32_table();

    if(tag->id != ST_DEFINEBITSLOSSLESS &&
       tag->id != ST_DEFINEBITSLOSSLESS2) {
	int id = GET16(tag->data);
	if (!extractanyids) {
	  fprintf(stderr, "Object %d is not a PNG picture!\n",id);
	  exit(1);
	}
	return 0;
    }

    id =swf_GetU16(tag);
    format = swf_GetU8(tag);
    if(format == 3) bpp = 8;
    if(format == 4) bpp = 16;
    if(format == 5) bpp = 32;
    if(format!=3 && format!=5) {
	if(format==4)
	fprintf(stderr, "Can't handle 16-bit palette images yet (image %d)\n",id);
	else 
	fprintf(stderr, "Unknown image type %d in image %d\n", format, id);
	return 0;
    }
    width = swf_GetU16(tag);
    height = swf_GetU16(tag);
    if(format == 3) cols = swf_GetU8(tag) + 1;
    else cols = 0;

    msg("<verbose> Width %d", width);
    msg("<verbose> Height %d", height);
    msg("<verbose> Format %d", format);
    msg("<verbose> Cols %d", cols);
    msg("<verbose> Bpp %d", bpp);
    msg("<verbose> Alpha %d", alpha);

#define align_width(w, a) (((w) + ((a)-1)) & -(a))
    datalen = (3+alpha)*cols + align_width((uLongf)width*(bpp/8), 4)*height;
    data = malloc(datalen);
    if(!data) {
	fprintf(stderr, "malloc error datalen:%ld tag->id:%d\n",
		datalen, tag->id);
        return 0;
    }
    error = uncompress (data, &datalen, &tag->data[tag->pos], tag->len-tag->pos);
    if(error != Z_OK) {
	fprintf(stderr, "Zlib error %d (image %d)\n", error, id);
	free(data);
	return 0;
    }
    msg("<verbose> Uncompressed image is %d bytes (%d colormap)", datalen, (3+alpha)*cols);
    pos = 0;
    datalen2 = datalen+16;
    data2 = malloc(datalen2);
    palette = (RGBA*)malloc(cols*sizeof(RGBA));

    for(t=0;t<cols;t++) {
	palette[t].r = data[pos++];
	palette[t].g = data[pos++];
	palette[t].b = data[pos++];
	if(alpha) {
	    palette[t].a = data[pos++];
	}
    }

    prepare_name(name, sizeof(name), "pic", "png", id);
    if(numextracts==1) {
	filename = destfilename;
	if(!strcmp(filename,"output.swf"))
	    filename = "output.png";
    }
    fi = save_fopen(filename, "wb");
    fwrite(head,sizeof(head),1,fi);     

    png_start_chunk(fi, "IHDR", 13);
     png_write_dword(fi,width);
     png_write_dword(fi,height);
     png_write_byte(fi,8);
     if(format == 3)
     png_write_byte(fi,3); //indexed
     else if(format == 5 && alpha==0)
     png_write_byte(fi,2); //rgb
     else if(format == 5 && alpha==1)
     png_write_byte(fi,6); //rgba
     else return 0;

     png_write_byte(fi,0); //compression mode
     png_write_byte(fi,0); //filter mode
     png_write_byte(fi,0); //interlace mode
    png_end_chunk(fi);
   
    if(format == 3) {
	png_start_chunk(fi, "PLTE", 768);
	 
	 for(t=0;t<256;t++) {
	     png_write_byte(fi,palette[t].r);
	     png_write_byte(fi,palette[t].g);
	     png_write_byte(fi,palette[t].b);
	 }
	png_end_chunk(fi);

	if(alpha) {
	    /* write alpha palette */
	    png_start_chunk(fi, "tRNS", 256);
	    for(t=0;t<256;t++) {
		png_write_byte(fi,palette[t].a);
	    }
	    png_end_chunk(fi);
	}
    }
    {
	int pos2 = 0;
	int x,y;
	int srcwidth = width * (bpp/8);
	datalen3 = (width*4+5)*height;
	data3 = (U8*)malloc(datalen3);
	for(y=0;y<height;y++)
	{
	   data3[pos2++]=0; //filter type
	   if(bpp==32) {
	    if(!alpha) {
		// 32 bit to 24 bit "conversion"
		for(x=0;x<width;x++) {
		    data3[pos2++]=data[pos+1];
		    data3[pos2++]=data[pos+2];
		    data3[pos2++]=data[pos+3];
		    pos+=4; //ignore padding byte
		}
	    } else {
		for(x=0;x<width;x++) {
		    data3[pos2++]=data[pos+1];
		    data3[pos2++]=data[pos+2];
		    data3[pos2++]=data[pos+3];
		    data3[pos2++]=data[pos+0]; //alpha
		    pos+=4;
		}
	    }
	   }
	   else {
		for(x=0;x<srcwidth;x++)
		    data3[pos2++]=data[pos++];
	   }
	   
	   pos+=((srcwidth+3)&~3)-srcwidth; //align
	}
	datalen3=pos2;
    }

    if(compress (data2, &datalen2, data3, datalen3) != Z_OK) {
	fprintf(stderr, "zlib error in pic %d\n", id);
	return 0;
    }
    msg("<verbose> Compressed data is %d bytes", datalen2);
    png_start_chunk(fi, "IDAT", datalen2);
    png_write_bytes(fi,data2,datalen2);
    png_end_chunk(fi);
    png_start_chunk(fi, "IEND", 0);
    png_end_chunk(fi);

    free(data);
    free(data2);
    free(data3);
    return 1;
}
#endif

static FILE*mp3file=0;
void handlesoundstream(TAG*tag)
{
    char*filename = "output.mp3";
    if(numextracts==1) {
	filename = destfilename;
	if(!strcmp(filename,"output.swf"))
	    filename = "output.mp3";
    }
    switch(tag->id) {
	case ST_SOUNDSTREAMHEAD:
	    if((tag->data[1]&0x30) == 0x20) { //mp3 compression
		mp3file = fopen(filename, "wb");
		msg("<notice> Writing mp3 data to %s",filename);
	    }
	    else
		msg("<error> Soundstream is not mp3");
	break;
	case ST_SOUNDSTREAMHEAD2:
	    if((tag->data[1]&0x30) == 0x20) {//mp3 compression
		mp3file = fopen(filename, "wb");
		msg("<notice> Writing mp3 data to %s",filename);
	    }
	    else
		msg("<error> Soundstream is not mp3 (2)");
	break;
	case ST_SOUNDSTREAMBLOCK:
	    if(mp3file)
		fwrite(&tag->data[4],tag->len-4,1,mp3file);
	break;
    }
}

int handledefinesound(TAG*tag)
{
    U8 flags;
    U32 samples;
    char buf[128];
    char*filename = buf;
    FILE*fi;
    char*extension = 0;
    int format;
    U16 id;
    int rate,bits,stereo;
    char*rates[] = {"5500","11025","22050","44100"};
    id = swf_GetU16(tag); //id
    
    flags = swf_GetU8(tag);
    format = flags>>4;
    rate = (flags>>2)&3;
    bits = flags&2?16:8;
    stereo = flags&1;
    
    samples = swf_GetU32(tag);

    extension = "raw";

    if(format == 2) { // mp3
	swf_GetU16(tag); //numsamples_seek
	extension = "mp3";
    } else if(format == 0) { // raw
	printf("Sound is RAW, format: %s samples/sec, %d bit, %s\n", rates[rate], bits, stereo?"stereo":"mono");
	// TODO: convert to WAV
	extension = "raw";
    } else if(format == 1) { // adpcm
	printf("Sound is ADPCM, format: %s samples/sec, %d bit, %s\n", rates[rate], bits, stereo?"stereo":"mono");
	extension = "adpcm";
    } else {
	printf("Unknown sound format %d: %s samples/sec, %d bit, %s\n", format, rates[rate], bits, stereo?"stereo":"mono");
	extension = "unknown";
    }
    prepare_name(buf, sizeof(buf), "sound", extension, id);
    if(numextracts==1) {
	filename = destfilename;
	if(!strcmp(filename,"output.swf")) {
	    sprintf(buf, "output.%s", extension);
	    filename = buf;
	}
    }
    fi = save_fopen(filename, "wb");
    fwrite(&tag->data[tag->pos], tag->len - tag->pos, 1, fi);
    fclose(fi);
    return 1;
}

int handlebinary(TAG*tag) {
    FILE *fout = NULL;
    char buf[100];
    char *filename = buf;
    int len = tag->memsize;
    int dx = 6; // offset to binary data
    if (tag->id!=ST_DEFINEBINARY) {
        if (!extractanyids) {
          fprintf(stderr, "Object %d is not a binary entity!\n",
                          GET16(tag->data));
        }
        return 0;
    }
    prepare_name(buf, sizeof(buf), "binary", "bin", GET16(tag->data));
    if(numextracts==1) {
	filename = destfilename;
	if(!strcmp(filename,"output.swf")) {
	    sprintf(buf, "output.bin");
	    filename = buf;
	}
    }
    fout = fopen(filename, "wb");
    fwrite(tag->data+dx,len-dx,1,fout);
    fclose(fout);
    return 1;
}

int handleembeddedmp3(TAG*tag) {
    int wasFolded;
    TAG *toFold;

    if (tag->id!=ST_DEFINESPRITE) {
        if (!extractanyids) {
          fprintf(stderr, "Object %d is not a sprite entity!\n",
                          GET16(tag->data));
        }
        return 0;
    }

    wasFolded = swf_IsFolded(tag);
    toFold = tag;

    if(wasFolded) 
	swf_UnFoldSprite(tag);

    while(tag->id != ST_END) { 
	tag = tag->next;
	if(tag->id == ST_SOUNDSTREAMHEAD ||
	   tag->id == ST_SOUNDSTREAMHEAD2 ||
	   tag->id == ST_SOUNDSTREAMBLOCK) {
 	   handlesoundstream(tag);
	}
    }

    if(wasFolded) 
	swf_FoldSprite(toFold);

    return 1;
}

int main (int argc,char ** argv)
{ 
    TAG*tag;
    SWF swf;
    int f;
    int found = 0;
    int frame = 0;
    int tagnum = 0;
    char depths[65536];
    char listavailable = 0;
    processargs(argc, argv);

    if(!extractframes && !extractids && ! extractname && !extractjpegids && !extractpngids
	&& !extractmp3 && !extractsoundids && !extractfontids && !extractbinaryids 
        && !extractanyids && !extractmp3ids)
	listavailable = 1;

    if(!originalplaceobjects && movetozero) {
	fprintf(stderr, "Error: -0 (--movetozero) can only be used in conjunction with -P (--placeobject)\n");
	return 0;
    }

    if(!filename)
    {
        fprintf(stderr, "You must supply a filename.\n");
        return 1;
    }
    initLog(0,-1,0,0,-1, verbose);

    f = open(filename,O_RDONLY|O_BINARY);

    if (f<0)
    { 
        perror("Couldn't open file: ");
        exit(1);
    }
    if (swf_ReadSWF(f,&swf) < 0)
    { 
        fprintf(stderr, "%s is not a valid SWF file or contains errors.\n",filename);
        close(f);
        exit(1);
    }
    close(f);

    if(listavailable) {
	listObjects(&swf);
	swf_FreeTags(&swf);
	return 0;
    }

    tag = swf.firstTag;
    tagnum = 0;
    while(tag) {
	tagnum ++;
	tag = tag->next;
    }

    tagused = (char*)malloc(tagnum);
    memset(tagused, 0, tagnum);
    memset(used, 0, 65536);
    memset(depths, 0, 65536);

    tag = swf.firstTag;
    tagnum = 0;
    while(tag) {
	if(swf_isAllowedSpriteTag(tag)) {
	    int write = 0;
	    if(extractframes && is_in_range(frame, extractframes)) {
		write = 1;
		if(tag->id == ST_PLACEOBJECT || tag->id == ST_PLACEOBJECT2) {
		    depths[swf_GetDepth(tag)] = 1;
		}
		if(tag->id == ST_REMOVEOBJECT || tag->id == ST_REMOVEOBJECT2) {
		    int depth = swf_GetDepth(tag);
		    if(!depths[depth]) 
			write = 0;
		    depths[swf_GetDepth(tag)] = 0;
		}
	    } else {
		if((tag->id == ST_REMOVEOBJECT || tag->id == ST_REMOVEOBJECT2) && 
		    (depths[swf_GetDepth(tag)]) && hollow) {
		    write = 1;
		    depths[swf_GetDepth(tag)] = 0;
		}
	    }
	    if(write) {
	        enumerateIDs(tag, idcallback);
	        found = 1;
	        tagused[tagnum] = 1;
	    }
	}

	if(tag->id == ST_SOUNDSTREAMHEAD ||
	   tag->id == ST_SOUNDSTREAMHEAD2 ||
	   tag->id == ST_SOUNDSTREAMBLOCK) {
	    if(extractmp3)
		handlesoundstream(tag);
	}

	if(tag->id == ST_JPEGTABLES) {
	    handlejpegtables(tag);
	}

	if(swf_isDefiningTag(tag)) {
	    int id = swf_GetDefineID(tag);
	    tags[id] = tag;
	    if(extractids && is_in_range(id, extractids)) {
		used[id] = 5;
		found = 1;
	    }
	    if(extractfontids && is_in_range(id, extractfontids)) {
		handlefont(&swf, tag);
	    }
	    if(extractjpegids && is_in_range(id, extractjpegids)) {
		handlejpeg(tag);
	    }
	    if(extractsoundids && is_in_range(id, extractsoundids)) {
		handledefinesound(tag);
	    }
	    if(extractmp3ids && is_in_range(id, extractmp3ids)) {
		handleembeddedmp3(tag);
	    }
	    if(extractbinaryids && is_in_range(id, extractbinaryids)) {
		handlebinary(tag);
	    }
#ifdef _ZLIB_INCLUDED_
	    if(extractpngids && is_in_range(id, extractpngids)) {
		handlelossless(tag);
	    }
#endif
	    if(extractanyids && is_in_range(id, extractanyids)) {
	        if (handlefont(&swf,tag)) {
		    // pass
		} else if (handlejpeg(tag)) {
		    // pass
		} else if (handlebinary(tag)) {
		    // pass
#ifdef _ZLIB_INCLUDED_
		} else if (handlelossless(tag)) {
		    // pass
#endif
		} else if (handledefinesound(tag)) {
		    // Not sure if sound code checks carefully for type.
		    // pass
		} else if (handleembeddedmp3(tag)) {
		    // pass
		} else {
		    printf("#%d not processed\n", id);
		}
	    }
	}
	else if (tag->id == ST_SETBACKGROUNDCOLOR) {
	    mainr = tag->data[0];
	    maing = tag->data[1];
	    mainb = tag->data[2];
	}
	else if(swf_isPlaceTag(tag) && tag->id != ST_PLACEOBJECT ) {
	    char*name = swf_GetName(tag);
	    if(name && extractname && !strcmp(name, extractname)) {
		int id = swf_GetPlaceID(tag); 
		used[id] = 5;
		found = 1;
		if(originalplaceobjects) {
		    tagused[tagnum] = 1;
		}
		depths[swf_GetDepth(tag)] = 1;
		extractname_id = id;
	    }
	}
	else if(tag->id == ST_SHOWFRAME) {
	    frame ++;
	    if(hollow) {
		tagused[tagnum] = 1;
		found = 1;
	    }
	}
	
	if(tag->id == ST_DEFINESPRITE) {
	    while(tag->id != ST_END) { 
		tag = tag->next;
		tagnum ++;
	    }
	}
	tag = tag->next;
	tagnum ++;
    }
    if (found)
	extractTag(&swf, destfilename);

    if(mp3file) {
	fclose(mp3file);
    } else {
        if(extractmp3) {
            msg("<error> Didn't find a soundstream in file");
        }
    }

    swf_FreeTags(&swf);
    return 0;
}

