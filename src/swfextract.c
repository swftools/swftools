/* swfextract.c
   Allows to extract parts of the swf into a new file.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "../lib/log.h"
#include "reloc.h"
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
char* extractpngids = 0;
char extractmp3 = 0;

char* extractname = 0;

char hollow = 0;

int numextracts = 0;

struct options_t options[] =
{
 {"o","output"},
 {"w","hollow"},
 {"v","verbose"},
 {"i","id"},
 {"j","jpegs"},
 {"p","pngs"},
 {"m","mp3"},
 {"n","name"},
 {"f","frame"},
 {"V","version"},
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
    else if(!strcmp(name, "j")) {
	if(extractjpegids) {
	    fprintf(stderr, "Only one --jpegs argument is allowed. (Try to use a range, e.g. -j 1,2,3)\n");
	    exit(1);
	}
	numextracts++;
	extractjpegids = val;
	return 1;
    } 
#ifdef _ZLIB_INCLUDED_
    else if(!strcmp(name, "p")) {
	if(extractpngids) {
	    fprintf(stderr, "Only one --pngs argument is allowed. (Try to use a range, e.g. -p 1,2,3)\n");
	    exit(1);
	}
	numextracts++;
	extractpngids = val;
	return 1;
    } 
#endif
    else if(!strcmp(name, "f")) {
	numextracts++;
	extractframes = val;
	return 1;
    }
    else if(!strcmp(name, "w")) {
	hollow = 1;
	return 0;
    }
    else {
        printf("Unknown option: -%s\n", name);
	return 0;
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
    printf("\t-n , --name name\t\t instance name of the object to extract\n");
    printf("\t-i , --id IDs\t\t\t ID of the object to extract\n");
    printf("\t-j , --jpeg IDs\t\t\t IDs of the JPEG pictures to extract\n");
#ifdef _ZLIB_INCLUDED_
    printf("\t-p , --pngs IDs\t\t\t IDs of the PNG pictures to extract\n");
#endif
    printf("\t-m , --mp3\t\t\t Extract main mp3 stream\n");
    printf("\t-f , --frame frames\t\t frame numbers to extract\n");
    printf("\t-w , --hollow\t\t\t hollow mode: don't remove empty frames (use with -f)\n");
    printf("\t-V , --version\t\t\t Print program version and exit\n");
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

void idcallback(void*data)
{
    if(!(used[GET16(data)]&1)) {
	changed = 1;
	used[GET16(data)] |= 1;
    }
}

void enumerateIDs(TAG*tag, void(*callback)(void*))
{
    U8*data;
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
}

void extractTag(SWF*swf, char*filename)
{
    SWF newswf;
    TAG*desttag;
    TAG*srctag;
    RGBA rgb;
    char sprite;
    int f;
    int t;
    int tagnum;
    int copy = 0;
    memset(&newswf,0x00,sizeof(SWF));        // set global movie parameters

    newswf.fileVersion    = swf->fileVersion;
    newswf.frameRate      = swf->frameRate;
    newswf.movieSize	  = swf->movieSize;
		
    newswf.firstTag = swf_InsertTag(NULL,ST_SETBACKGROUNDCOLOR);
    desttag = newswf.firstTag;
    rgb.r = mainr;
    rgb.g = maing;
    rgb.b = mainb;
    swf_SetRGB(desttag,&rgb);

    do {
       changed = 0;
       for(t=0;t<65536;t++) {
	   if(used[t] && !(used[t]&2)) {
	     if(tags[t]->id==ST_DEFINESPRITE) {
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
	if(swf_isDefiningTag(srctag)) {
	    int id = swf_GetDefineID(srctag);
	    if(used[id]) 
		copy = 1;
	} else 
	if (((srctag->id == ST_PLACEOBJECT ||
	      srctag->id == ST_PLACEOBJECT2 ||
	      srctag->id == ST_STARTSOUND) && (used[swf_GetPlaceID(srctag)]&4) ) ||
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
	    if(reset)
	        copy = 0;
	}
	
	srctag = srctag->next;
	tagnum ++;
    }
    if(!extractframes && !hollow)
	desttag = swf_InsertTag(desttag,ST_SHOWFRAME);

    desttag = swf_InsertTag(desttag,ST_END);
    
    f = open(filename, O_TRUNC|O_WRONLY|O_CREAT, 0644);
    if FAILED(swf_WriteSWF(f,&newswf)) fprintf(stderr,"WriteSWF() failed.\n");
    close(f);

    swf_FreeTags(&newswf);                       // cleanup
}

void listObjects(SWF*swf)
{
    TAG*tag;
    char first;
    int t;
    int frame = 0;
    char*names[] = {"Shapes","MovieClips","JPEGs","PNGs","Sounds","Frames"};
    printf("Objects in file %s:\n",filename);
    for(t=0;t<6;t++) {
	tag = swf->firstTag;
	first = 1;
	while(tag) {
	    char show = 0;
	    char text[80];
	    if(t == 0 &&
	       (tag->id == ST_DEFINESHAPE ||
		tag->id == ST_DEFINESHAPE2 ||
		tag->id == ST_DEFINESHAPE3)) {
		show = 1;
	        sprintf(text,"%d", swf_GetDefineID(tag));
	    }

	    if(tag->id == ST_DEFINESPRITE) {
		if (t == 1)  {
		    show = 1;
		    sprintf(text,"%d", swf_GetDefineID(tag));
		}

		while(tag->id != ST_END)
		    tag = tag->next;
	    }

	    if(t == 2 && (tag->id == ST_DEFINEBITS ||
		tag->id == ST_DEFINEBITSJPEG2 ||
		tag->id == ST_DEFINEBITSJPEG3)) {
		show = 1;
		sprintf(text,"%d", swf_GetDefineID(tag));
	    }

	    if(t == 3 && (tag->id == ST_DEFINEBITSLOSSLESS ||
		tag->id == ST_DEFINEBITSLOSSLESS2)) {
		show = 1;
		sprintf(text,"%d", swf_GetDefineID(tag));
	    }


	    if(t == 4 && (tag->id == ST_DEFINESOUND)) {
		show = 1;
		sprintf(text,"%d", swf_GetDefineID(tag));
	    }
	    
	    if(t == 5 && (tag->id == ST_SHOWFRAME)) {
		show = 1;
		sprintf(text,"%d", frame);
		frame ++;
	    }

	    if(show) {
		if(!first)
		    printf(", ");
		else
		    printf("%s: ", names[t]);
		printf("%s", text);
		first = 0;
	    }
	    tag=tag->next;
	}
	if(!first)
	    printf("\n");
    }
}

U8*jpegtables = 0;
int jpegtablessize;

void handlejpegtables(TAG*tag)
{
    if(tag->id == ST_JPEGTABLES) {
	jpegtables = tag->data;
	jpegtablessize = tag->len;
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
    for(t=0;t<len;t++) {
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
void handlejpeg(TAG*tag)
{
    char name[80];
    FILE*fi;
    sprintf(name, "pic%d.jpeg", GET16(tag->data));
    /* swf jpeg images have two streams, which both start with ff d8 and
       end with ff d9. The following code handles sorting the middle
       <ff d9 ff d8> bytes out, so that one stream remains */
    if(tag->id == ST_DEFINEBITS && tag->len>2 && jpegtables) {
	fi = save_fopen(name, "wb");
	fwrite(jpegtables, 1, jpegtablessize-2, fi); //don't write end tag (ff,d8)
	fwrite(&tag->data[2+2], tag->len-2-2, 1, fi); //don't write start tag (ff,d9)
	fclose(fi);
    }
    if(tag->id == ST_DEFINEBITSJPEG2 && tag->len>2) {
	int end = tag->len;
	int pos = findjpegboundary(&tag->data[2], tag->len-2);
	if(pos<0)
	    return;
	pos+=2;
	fi = save_fopen(name, "wb");
	fwrite(&tag->data[2], pos-2, 1, fi);
	fwrite(&tag->data[pos+4], end-(pos+4), 1, fi);
	fclose(fi);
    }
    if(tag->id == ST_DEFINEBITSJPEG3 && tag->len>6) {
	U32 end = GET32(&tag->data[2])+6;
	int pos = findjpegboundary(&tag->data[6], tag->len-6);
	if(pos<0)
	    return;
	pos+=6;
	fi = save_fopen(name, "wb");
	fwrite(&tag->data[6], pos-6, 1, fi);
	fwrite(&tag->data[pos+4], end-(pos+4), 1, fi);
	fclose(fi);
    }
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
static void png_write_byte(FILE*fi, U8 byte)
{
    fwrite(&byte,1,1,fi);
    mycrc32 = crc32_table[(mycrc32 ^ byte) & 0xff] ^ (mycrc32 >> 8);
}
static void png_start_chunk(FILE*fi, char*type, int len)
{
    U8 mytype[4]={0,0,0,0};
    U32 mylen = REVERSESWAP32(len);
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
    U32 tmp = REVERSESWAP32((mycrc32^0xffffffff));
    fwrite(&tmp,4,1,fi);
}


/* extract a lossless image (png) out of a tag 
   This routine was originally meant to be a one-pager. I just
   didn't know png is _that_ much fun. :) -mk
 */
void handlelossless(TAG*tag)
{
    char name[80];
    FILE*fi;
    int width, height;
    int crc;
    int id;
    int t;
    U8 bpp = 1;
    U8 format;
    U8 tmp;
    U8* data=0;
    U8* data2=0;
    U8* data3=0;
    U32 datalen;
    U32 datalen2;
    U32 datalen3;
    U8 head[] = {137,80,78,71,13,10,26,10};
    int cols;
    char alpha = tag->id == ST_DEFINEBITSLOSSLESS2;
    RGBA* palette;
    int pos;
    int error;
    U32 tmp32;

    make_crc32_table();

    if(tag->id != ST_DEFINEBITSLOSSLESS &&
       tag->id != ST_DEFINEBITSLOSSLESS2)
	return;

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
	return;
    }
    width = swf_GetU16(tag);
    height = swf_GetU16(tag);
    if(format == 3) cols = swf_GetU8(tag) + 1;
// this is what format means according to the flash specification. (which is
// clearly wrong)
//    if(format == 4) cols = swf_GetU16(tag) + 1;
//    if(format == 5) cols = swf_GetU32(tag) + 1;
    else cols = 0;

    logf("<verbose> Width %d", width);
    logf("<verbose> Height %d", height);
    logf("<verbose> Format %d", format);
    logf("<verbose> Cols %d", cols);
    logf("<verbose> Bpp %d", bpp);

    datalen = (width*height*bpp/8+cols*8);
    do {
	if(data)
	    free(data);
	datalen+=4096;
	data = malloc(datalen);
	error = uncompress (data, &datalen, &tag->data[tag->pos], tag->len-tag->pos);
    } while(error == Z_BUF_ERROR);
    if(error != Z_OK) {
	fprintf(stderr, "Zlib error %d (image %d)\n", error, id);
	return;
    }
    logf("<verbose> Uncompressed image is %d bytes (%d colormap)", datalen, (3+alpha)*cols);
    pos = 0;
    datalen2 = datalen;
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

    sprintf(name, "pic%d.png", id);
    fi = save_fopen(name, "wb");
    fwrite(head,sizeof(head),1,fi);     

    png_start_chunk(fi, "IHDR", 13);
     png_write_dword(fi,width);
     png_write_dword(fi,height);
     png_write_byte(fi,8);
     if(format == 3)
     png_write_byte(fi,3); //indexed
     else if(format == 5)
     png_write_byte(fi,2); //rgb
     else return;

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
    }
    {
	int pos2 = 0;
	int x,y;
	int srcwidth = width * (bpp/8);
	datalen3 = width*height*4;
	data3 = (U8*)malloc(datalen3);
	for(y=0;y<height;y++)
	{
	   data3[pos2++]=0; //filter type
	   if(bpp==32) {
	    // 32 bit to 24 bit "conversion"
	    for(x=0;x<width;x++) {
	        data3[pos2++]=data[pos+1];
	        data3[pos2++]=data[pos+2];
		data3[pos2++]=data[pos+3];
		pos+=4; //ignore padding byte
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
	return;
    }
    logf("<verbose> Compressed data is %d bytes", datalen2);
    png_start_chunk(fi, "IDAT", datalen2);
    png_write_bytes(fi,data2,datalen2);
    png_end_chunk(fi);
    png_start_chunk(fi, "IEND", 0);
    png_end_chunk(fi);

    free(data);
    free(data2);
    free(data3);
}
#endif

FILE*mp3file;
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
		logf("<notice> Writing mp3 data to %s",filename);
	    }
	    else
		logf("<error> Soundstream is not mp3");
	break;
	case ST_SOUNDSTREAMHEAD2:
	    if((tag->data[1]&0x30) == 0x20) {//mp3 compression
		mp3file = fopen("mainstream.mp3", "wb");
		logf("<notice> Writing mp3 data to %s",filename);
	    }
	    else
		logf("<error> Soundstream is not mp3 (2)");
	break;
	case ST_SOUNDSTREAMBLOCK:
	    if(mp3file)
		fwrite(&tag->data[4],tag->len-4,1,mp3file);
	break;
    }
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
	&& !extractmp3)
	listavailable = 1;

    if(!filename)
    {
        fprintf(stderr, "You must supply a filename.\n");
        return 1;
    }
    initLog(0,-1,0,0,-1, verbose);

    f = open(filename,O_RDONLY);

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
	    handlesoundstream(tag);
	}

	if(tag->id == ST_JPEGTABLES)
	    handlejpegtables(tag);

	if(swf_isDefiningTag(tag)) {
	    int id = swf_GetDefineID(tag);
	    tags[id] = tag;
	    if(extractids && is_in_range(id, extractids)) {
		used[id] = 5;
		found = 1;
	    }
	    if(extractjpegids && is_in_range(id, extractjpegids)) {
		handlejpeg(tag);
	    }
#ifdef _ZLIB_INCLUDED_
	    if(extractpngids && is_in_range(id, extractpngids)) {
		handlelossless(tag);
	    }
#endif
	}
	else if (tag->id == ST_SETBACKGROUNDCOLOR) {
	    mainr = tag->data[0];
	    maing = tag->data[1];
	    mainb = tag->data[2];
	}
	else if(tag->id == ST_PLACEOBJECT2) {
	    char*name = swf_GetName(tag);
	    if(name && extractname && !strcmp(name, extractname)) {
		int id = swf_GetPlaceID(tag); 
		used[id] = 5;
		found = 1;
		tagused[tagnum] = 1;
		depths[swf_GetDepth(tag)] = 1;
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

    if(mp3file)
	fclose(mp3file);

    swf_FreeTags(&swf);
    return 0;
}

