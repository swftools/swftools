/* swfbytes.c
   A tool for modifying swfs on the tag level

   Part of the swftools package.
   
   Copyright (c) 2008/2009 Matthias Kramm <kramm@quiss.org>
 
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

#include "../config.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"

static char * filename = 0;
static int verbose;
static char* output_filename = "output.swf";

static struct options_t options[] = {
{"h", "help"},
{"v", "verbose"},
{"V", "version"},
{0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfedit - part of %s %s\n", PACKAGE, VERSION);
	return 0;
    } else if(!strcmp(name, "v")) {
	verbose++;
	return 0;
    } else if(!strcmp(name, "o")) {
	output_filename = val;
	return 1;
    } else {
        printf("Unknown option: -%s\n", name);
	return 0;
    }
    return 0;
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char *name)
{
    printf("\n");
    printf("Usage: %s [-v] file.swf > file.hexdump\n", name);
    printf("OR:    %s file.hexdump\n", name);
    printf("\n");
    printf("-h , --help                    Print help and exit\n");
    printf("-v , --verbose                 Be more verbose\n");
    printf("-V , --version                 Print program version and exit\n");
    printf("\n");
}
int args_callback_command(char*name,char*val)
{
    if(filename) {
        fprintf(stderr, "Only one file allowed. You supplied at least two. (%s and %s)\n", filename, name);
    }
    filename = name;
    return 0;
}

void dumpTag(FILE*fo, char*prefix, TAG*tag)
{
    int t;
    for(t=0;t<tag->len;t++) {
	if(!(t&15))
	    fprintf(fo, "%s| ", prefix);
	fprintf(fo, "%02x ", tag->data[t]);
	if((t && ((t&15)==15)) || (t==tag->len-1)) {
	    fprintf(fo, "\n");
	}
    }
}

void dumpFile(SWF*swf, FILE*fo)
{
    TAG* tag = swf->firstTag;
    int indent = 0;
    int t;
    char whitespace[33];
    char*prefix = "";

    memset(whitespace, 32, 32); whitespace[32] = 0;
    
    fprintf(fo, "Version: %d\n", swf->fileVersion);
    fprintf(fo, "FrameRate: %f\n",swf->frameRate/256.0);
    fprintf(fo, "FrameCount: %d\n",swf->frameCount);
    fprintf(fo, "Width: %.2f\n",(swf->movieSize.xmax-swf->movieSize.xmin)/20.0);
    fprintf(fo, "X-Offset: %.2f\n", swf->movieSize.xmin/20.0);
    fprintf(fo, "Height: %.2f\n",(swf->movieSize.ymax-swf->movieSize.ymin)/20.0);
    fprintf(fo, "Y-Offset: %.2f\n", swf->movieSize.ymin/20.0);
    fprintf(fo, "\n");

    while(tag)
    {
	if(swf_isDefiningTag(tag)) {
	    fprintf(fo, "%s%s <%d>\n", prefix, swf_TagGetName(tag), swf_GetDefineID(tag));
	    swf_SetTagPos(tag, 2);
	    dumpTag(fo, prefix, tag);
	} else if(swf_isPseudoDefiningTag(tag)) {
	    fprintf(fo, "%s%s <%d>\n", prefix, swf_TagGetName(tag), swf_GetDefineID(tag));
	    swf_SetTagPos(tag, 2);
	    dumpTag(fo, prefix, tag);
	} else if(tag->id == ST_PLACEOBJECT || tag->id == ST_PLACEOBJECT2) { 
	    SWFPLACEOBJECT po;
	    swf_GetPlaceObject(tag, &po);
	    fprintf(fo, "%s%s <%d>\n",  prefix,swf_TagGetName(tag), po.id);
	    /* for now */
	    swf_SetTagPos(tag, 0);
	    dumpTag(fo, prefix, tag);
	    swf_PlaceObjectFree(&po);
	} else {
	    fprintf(fo, "%s%s\n", prefix, swf_TagGetName(tag));
	    dumpTag(fo, prefix, tag);
	}

	if(tag->id == ST_DEFINESPRITE) {
	    indent+=4;
	    if(indent>32)
		indent = 32;
	    prefix = &whitespace[32-indent];
	} else if(tag->id == ST_END) {
	    indent-=4;
	    if(indent<0)
		indent = 0;
	    prefix = &whitespace[32-indent];
	}

	tag = tag->next;
    }
}

static void readline(FILE*fi, char*line, int maxlen) {
    int pos = 0;
    while(!feof(fi)) {
        if(!fread(&line[pos],1,1,fi))
            break;

	/* cut of preceding whitespace */
	if(pos == 0 && (line[0] == 32 || line[0] == '\t'))
	    continue;

        if(line[pos] == 13 || line[pos]==10)
            break;
	if(pos<maxlen-1)
	    pos++;
    }
    line[pos]=0;

    /* cut off comments */
    char*x=strchr(line,'#');
    if(x) {
        *x=0;
	pos = x-line;
    }

    /* cut off trailing whitespace */
    while(pos>=1 && (line[pos-1]==32 || line[pos-1]==9)) {
	pos--;
	line[pos]=0;
    }
}

int getFloat(char*s)
{
    float x;
    int n;
    while(*s==32 || *s=='\t') s++;
    sscanf(s, "%f%n", &x, &n);
    if(n==0) 
	fprintf(stderr, "Not a float: %s\n", s);
    return x;
}
int getInt(char*s)
{
    int i;
    int n;
    while(*s==32 || *s=='\t') s++;
    sscanf(s, "%d%n", &i, &n);
    if(n==0) 
	fprintf(stderr, "Not an integer: %s\n", s);
    return i;
}
int getTwip(char*x)
{
    return (int)(getFloat(x)*20);
}


static char**lookup;
int swf_TagNameToID(char*name)
{
    int t;
    TAG tag;
    memset(&tag, 0, sizeof(tag));
    if(!lookup) {
	lookup = (char**)malloc(sizeof(char*)*65536);
	for(t=0;t<65536;t++) {
	    tag.id = t;
	    lookup[t] = swf_TagGetName(&tag);
	}
    }
    for(t=0;t<65536;t++) {
	if(lookup[t] && !strcasecmp(name, lookup[t]))
	    return t;
    }
    fprintf(stderr, "Not a tag name: \"%s\"\n", name);
    return -1;
}

void parseFile(FILE*fi, SWF*swf)
{
    TAG*tag = 0;
    char line[1024];
    memset(swf, 0, sizeof(SWF));
    while(1) {
	char*colon = 0;
	readline(fi, line, 1024);
	if(!line[0])
	    break;
	colon = strchr(line, ':');
	if(colon) {
	    int num = colon - line;
	    int n;
	    if(num == 9 && !strncmp(line, "FrameRate", num)) {
		swf->frameRate = getFloat(colon+1)*256;
	    } else if(num == 10 && !strncmp(line, "FrameCount", num)) {
		swf->frameCount = getInt(colon+1);
	    } else if(num == 7 && !strncmp(line, "Version", num)) {
		swf->fileVersion = getInt(colon+1);
	    } else if(num == 5 && !strncmp(line, "Width", num)) {
		int width = getTwip(colon+1);
		swf->movieSize.xmax += width;
	    } else if(num == 6 && !strncmp(line, "Height", num)) {
		int height = getTwip(colon+1);
		swf->movieSize.ymax += height;
	    } else if(num == 8 && !strncmp(line, "X-Offset", num)) {
		int xoffset = getTwip(colon+1);
		swf->movieSize.xmin += xoffset;
		swf->movieSize.xmax += xoffset;
	    } else if(num == 8 && !strncmp(line, "Y-Offset", num)) {
		int yoffset = getTwip(colon+1);
		swf->movieSize.ymin += yoffset;
		swf->movieSize.ymax += yoffset;
	    } else {
		fprintf(stderr, "Ignored line \"%s\"\n", line);
	    }
	}
    }
    while(!feof(fi)) {
	char*s, *tagname;
	char*br;
	readline(fi, line, 1024);
	if(!line[0]) 
	    continue;
	s = strchr(line, ' ');
	br = strchr(line, '|');
	if(!br) {
	    int id = 0;
	    /* DEFINESHAPE <id> ... type line */
	    if(!s) {
		tagname = strdup(line);
	    } else {
		tagname = strdup(line);
		tagname[s-line] = 0;
	    }
	    id = swf_TagNameToID(tagname);
	    free(tagname);
	    if(id<0) {
		fprintf(stderr, "Ignored line \"%s\"\n", line);
		continue;
	    }
	    tag = swf_InsertTag(tag, id);
	    if(!swf->firstTag)
		swf->firstTag = tag;
	} else {
	    /* | 00 34 fe c7 ... type line */
	    char*p = line;
	    int num = 0;
	    int digits = 0;

	    if(!tag) {
		fprintf(stderr, "Discarded unassignable data %s\n", line);
		continue;
	    }

	    while(*p) {
		int n = 0;
		if((*p>='a' && *p<='f') ||
		   (*p>='A' && *p<='F')) {
		    n = 9 + (*p & 15);
		    num = (num<<4) | n;
		    digits++;
		}
		else if(*p>='0' && *p<='9') {
		    n = *p & 15;
		    num = (num<<4) | n;
		    digits++;
		} else if(digits) {
		    swf_SetU8(tag, num);
		    num = 0;
		    digits = 0;
		}
		p++;
	    }
	    if(digits)
		swf_SetU8(tag, num);
	}
    }
}
    
char swf_IsSWF(char*filename)
{
    int fi = open(filename,O_RDONLY|O_BINARY);
    U8 buf[3] = {0,0,0};
    if(fi<0)
	return 0;
    read(fi, buf, 3);
    close(fi);
    if((buf[0] == 'F' || buf[0] == 'C') && buf[1] == 'W' && buf[2] == 'S')
	return 1;
    return 0;
}

int main (int argc,char ** argv)
{ 
    int fi;
    TAG*tag = 0;
    SWF swf;

    processargs(argc, argv);
    
    if(!filename) {
        fprintf(stderr, "You must supply a filename.\n");
        return -1;
    }
    if(swf_IsSWF(filename)) {
	fi = open(filename,O_RDONLY|O_BINARY);
	if (fi<0) { 
	    perror("Couldn't open file: ");
	    exit(1);
	}
	if FAILED(swf_ReadSWF(fi,&swf)) { 
	    fprintf(stderr, "%s is not a valid SWF file or contains errors.\n",filename);
	    close(fi);
	    exit(1);
	}
	dumpFile(&swf, stdout);
	swf_FreeTags(&swf);
    } else {
	SWF newswf;
	FILE*fi = fopen(filename, "rb");
	parseFile(fi, &newswf);
	fclose(fi);
	int f;
	char*sname = output_filename;
	f = open(sname,O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
	if FAILED(swf_WriteSWF(f,&newswf)) { 
	    fprintf(stderr, "Unable to write output file: %s\n", sname);
	}
	close(f);
    }

    return 0;
}
