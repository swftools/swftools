/* swfcombine.c
   main routine for swfcombine(1), a tool for merging .swf-files.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "../lib/log.h"
#include "combine.h"
#include "settings.h"
#include "types.h"
#include "flash.h"
#include "reloc.h"
#include "../config.h"

char * master_filename = 0;
char * master_name = 0;
char * slave_filename[128];
char * slave_name[128];
int slave_movex[128];
int slave_movey[128];
float slave_scalex[128];
float slave_scaley[128];
char slave_isframe[128];
int numslaves = 0;

char * outputname = "output.swf";

int args_callback_option(char*name,char*val) {
    if(!strcmp(name,"c"))
    {
	config.clip = 1;
	return 0;
    }
    else if(!strcmp(name,"l"))
    {
	config.overlay = 1;
	return 0;
    }
    else if (!strcmp(name, "o"))
    {
	outputname = val;
	return 1;
    }
    else if (!strcmp(name, "v"))
    {
	config.loglevel ++;
	return 0;
    }
    else if (!strcmp(name, "a"))
    {
	config.cat = 1;
	return 0;
    }
    else if (!strcmp(name, "A"))
    {
	config.alloctest = 1;
	return 0;
    }
    else if (!strcmp(name, "x"))
    {
	config.movex = atoi(val);
	return 1;
    }
    else if (!strcmp(name, "y"))
    {
	config.movey = atoi(val);
	return 1;
    }
    else if (!strcmp(name, "m"))
    {
	config.merge = 1;
	return 0;
    }
    else if (!strcmp(name, "f"))
    {
	config.isframe = 1;
	return 0;
    }
    else if (!strcmp(name, "d"))
    {
	config.dummy = 1;
	return 0;
    }
    else if (!strcmp(name, "r"))
    {
	config.framerate = atoi(val)*256/100;
	return 1;
    }
    else if (!strcmp(name, "X"))
    {
	config.sizex = atoi(val)*20;
	config.hassizex = 1;
	return 1;
    }
    else if (!strcmp(name, "Y"))
    {
	config.sizey = atoi(val)*20;
	config.hassizey = 1;
	return 1;
    }
    else if (!strcmp(name, "s"))
    {
	config.scalex = config.scaley = atoi(val)/100.0;
	return 1;
    }
    else if (!strcmp(name, "t") || !strcmp(name, "T"))
    {
	if(master_filename) {
	    fprintf(stderr, "error with arguments. Try --help.\n");
	    exit(1);
	}
	config.stack = 1;
	if(!strcmp(name,"T"))
	    config.stack1 = 1;
	master_filename = "__none__";
	return 0;
    }
    else if (!strcmp(name, "V"))
    {	
	printf("swfcombine - part of %s %s\n", PACKAGE, VERSION);
	exit(0);
    }
    else 
    {
	fprintf(stderr, "Unknown option: -%s\n", name);
	exit(1);
    }
}

struct options_t options[] =
{{"o","output"},
 {"s","scale"},
 {"d","dummy"},
 {"x","xpos"},
 {"y","ypos"},
 {"X","width"},
 {"Y","height"},
 {"r","rate"},
 {"f","frame"},
 {"l","overlay"},
 {"m","merge"},
 {"t","stack"},
 {"T","stack1"},
 {"v","verbose"},
 {"V","version"},
 {"c","clip"},
 {"a","cat"},
 {0,0}
};

int args_callback_longoption(char*name,char*val) {
    return args_long2shortoption(options, name, val);
}

int args_callback_command(char*name, char*val) {
    char*myname = strdup(name);
    char*filename;
    filename = strchr(myname, '=');
    if(filename) {
	*filename = 0;
	filename++;
    } else {
	// argument has no explicit name field. guess one from the file name
	char*path = strrchr(myname, '/');
	char*ext = strrchr(myname, '.');
	if(!path) path = myname;
	else path ++;
	if(ext) *ext = 0;
	myname = path;
	filename = name;
    }

    if(!master_filename) {
	master_filename = filename;
	master_name = myname;
    } else {		 
	logf("<verbose> slave entity %s (named \"%s\")\n", filename, myname);

	slave_filename[numslaves] = filename;
	slave_name[numslaves] = myname;
	slave_movex[numslaves] = config.movex;
	slave_movey[numslaves] = config.movey;
	slave_scalex[numslaves] = config.scalex;
	slave_scaley[numslaves] = config.scaley;
	slave_isframe[numslaves] = config.isframe; 
	config.isframe = 0;
	config.movex = config.movey = 0;
	config.scalex = config.scaley = 1.0;
	numslaves ++;
    }
    return 0;
}

void args_callback_usage(char*name)
{
    printf("Usage: %s [-rXYomlcv] [-f] masterfile] [-xysf] [(name1|#id1)=]slavefile1 .. [-xysf] [(nameN|#idN)=]slavefileN\n", name);
    printf("OR:    %s [-rXYomv] --stack[1] [-xysf] [(name1|#id1)=]slavefile1 .. [-xysf] [(nameN|#idN)=]slavefileN\n", name);
    printf("OR:    %s [-rXYov] --cat [-xysf] [(name1|#id1)=]slavefile1 .. [-xysf] [(nameN|#idN)=]slavefileN\n", name);
    printf("OR:    %s [-rXYomlcv] --dummy [-xys] [file]\n", name);
    printf("\n");
    printf("-o outputfile       --output    explicitly specify output file. (otherwise, output.swf will be used\n");
    printf("-t                  --stack     place each slave in a seperate frame (no master movie\n");
    printf("-T                  --stack1    place each slave in the first frame (no master movie\n");
    printf("-m                  --merge     Don't store the slaves in Sprites/MovieClips\n");
    printf("-a                  --cat       concatenate all slave files (no master movie\n");
    printf("-l                  --overlay   Don't remove any master objects, only overlay new objects\n");
    printf("-c                  --clip      Clip the slave objects by the corresponding master objects\n");
    printf("-v                  --verbose   Use more than one -v for greater effect \n");
    printf("-d                  --dummy     Don't require slave objects \n");
    printf("-f                  --frame     The following identifier is a frame or framelabel, not an id or objectname\n");
    printf("-x xpos             --movex     x Adjust position of slave by xpos twips (1/20 pixel\n");
    printf("-y ypos             --movey     y Adjust position of slave by ypos twips (1/20 pixel\n");
    printf("-s scale            --scale     Adjust size of slave by scale%\n");
    printf("-r framerate        --rate      Set movie framerate (100 frames/sec\n");
    printf("-X width            --width     Force movie width to scale (default: use master width (not with -t\n");
    printf("-Y height           --height    Force movie height to scale (default: use master height (not with -t\n");
}

/* read a whole file in memory */
char* fi_slurp(FILE*fi, unsigned int * setlength)
{
    char * mem;
    long long int length; //;)  
    long long int pos = 0;
    fseek(fi,0,SEEK_END);
    length = ftell(fi);
    fseek(fi,0,SEEK_SET);
    if(!length)
	return 0;
    mem = malloc(length);
    if(!mem)
	return 0;
    while(!feof(fi))
    {
	pos += fread(&mem[pos], 1, 65536, fi);
    }
    if (setlength) 
	*setlength = length;
    return mem;
}

void fi_dump(FILE*fi, void*_mem, int length)
{
    char*mem = (char*)_mem;
    int pos = 0;
    while(pos < length)
    {
	int size = 65536;
	if (size > (length - pos))
		size = (length - pos);
	pos += fwrite(&mem[pos], 1, size, fi);
    }
}

/* todo: use rfxswf */
void makestackmaster(u8**masterdata, int*masterlength)
{
    u8 head[] = {'F','W','S'};
    u8 *pos;
    u32 * fixpos;
    int t;
    struct RECT box;
    int strlength = 0;
    int fileversion = 1;

    /* scan all slaves for bounding box */
    for(t=0;t<numslaves;t++)
    {
	FILE*fi=fopen(slave_filename[t],"rb");
	u8 data[256];
	int ret;
	struct flash_header head;
	struct reader_t r;
	strlength += strlen(slave_name[t]) + 9;
	if(!fi) {
	    logf("<fatal> Couldn't open %s.", slave_filename[t]);
	    exit(1);
	}
	ret = fread(data,1,256,fi);
	if(ret < 13) {
	    logf("<fatal> File %s is to small (%d bytes)", slave_filename[t], ret);
	    exit(1);
	}
	swf_init(&r, data,256);
	head = swf_read_header(&r);
	logf("<verbose> File %s has bounding box %d:%d:%d:%d\n",
		slave_filename[t], 
		head.boundingBox.x1, head.boundingBox.y1,
		head.boundingBox.x2, head.boundingBox.y2);
	if(head.version > fileversion)
	    fileversion = head.version;
	if(!t)
	    box = head.boundingBox;
	else {
	    if(head.boundingBox.x1 < box.x1)
		box.x1 = head.boundingBox.x1;
	    if(head.boundingBox.y1 < box.y1)
		box.y1 = head.boundingBox.y1;
	    if(head.boundingBox.x2 > box.x2)
		box.x2 = head.boundingBox.x2;
	    if(head.boundingBox.y2 > box.y2)
		box.y2 = head.boundingBox.y2;
	}
	logf("<verbose> New master bounding box is %d:%d:%d:%d\n",
		box.x1, box.y1,
		box.x2, box.y2);
	fclose(fi);
    }

    /* we don't have a master, so we create one ourselves. */
    *masterlength = (numslaves + 1) * 32 + strlength;
    *masterdata = (u8*)malloc(*masterlength);
    pos = *masterdata;
    memcpy(pos, head, sizeof(head));
    pos += sizeof(head);
    *pos++ = fileversion;
    fixpos = (u32*)pos;
    PUT32(pos, 0x12345678); // to be overwritten
    pos += 4;
    writeRECT(&pos, &box);
    PUT16(pos, 0x2000) // framerate
    pos += 2;
    PUT16(pos, numslaves) // framerate
    pos += 2;
    for(t=0;t<numslaves;t++)
    {
	char buf[128];
	int namelen;

	if(1) {
	    sprintf(buf, "Frame%02d", t);
	    slave_name[t] = strdup(buf);
	} 
	namelen = strlen(slave_name[t]);

	PUT16(&pos[0] , ((u16)(TAGID_DEFINESPRITE<<6) + 6));
	PUT16(&pos[2] , (t+1)); //ID
	PUT16(&pos[4] , 0); // Frames
	PUT16(&pos[6] , 0); // TAG1
	PUT16(&pos[8] , ((u16)(TAGID_PLACEOBJECT2<<6) + 6 + namelen));
	PUT16(&pos[10], (34)); //flags: id+name
	PUT16(&pos[11], (1+t)); // depth
	PUT16(&pos[13], (t+1)); // id
	sprintf(&pos[15],slave_name[t]);
	pos += 15 + namelen + 1;
	if(!config.stack1 || t == numslaves-1) {
	    PUT16(&pos[0],((u16)(TAGID_SHOWFRAME<<6) + 0));
	    pos += 2;
	}
	if(!config.stack)
	if(t!=numslaves-1)
	{
	    PUT16(&pos[0], ((u16)(TAGID_REMOVEOBJECT2<<6) + 2));
	    PUT16(&pos[2], (1+t)); // depth;
	    pos += 4;
	}
    }
    PUT16(pos, (TAGID_END<<6 + 0));
    *masterlength = pos - *masterdata;
    PUT32(fixpos, *masterlength);
}

struct config_t config;
int main(int argn, char *argv[])
{
    FILE*fi;
    u8*masterdata;
    unsigned int masterlength;
    u8*slavedata;
    unsigned int slavelength;
    u8*newdata;
    unsigned int newlength;
    int t;

    config.overlay = 0; 
    config.antistream = 0; 
    config.alloctest = 0;
    config.cat = 0;
    config.merge = 0;
    config.clip = 0;
    config.loglevel = 2; 
    config.movex = 0;
    config.movey = 0;
    config.scalex = 1.0;
    config.scaley = 1.0;
    config.sizex = 0;
    config.sizey = 0;
    config.hassizex = 0;
    config.hassizey = 0;
    config.framerate = 0;
    config.stack = 0;
    config.stack1 = 0;
    config.dummy = 0;

    processargs(argn, argv);
    initLog(0,-1,0,0,-1,config.loglevel);

    if(config.merge && config.cat) {
	logf("<error> Can't combine --cat and --merge");
	exit(1);
    }

    if(config.stack) {

	if(config.overlay) {
	    logf("<error> Can't combine -l and -t");
	    exit(1);
	}
	if(config.clip) {
	    logf("<error> Can't combine -c and -t");
	    exit(1);
	}
	logf("<verbose> (stacking) %d files found\n", numslaves);

	makestackmaster(&masterdata,&masterlength);

	logf("<verbose> Generated %d bytes of master data", masterlength);
    }
    else {
	logf("<verbose> master entity %s (named \"%s\")\n", master_filename, master_name);
	fi = fopen(master_filename, "rb");
	if(!fi) {
	    fprintf(stderr, "Failed to open %s\n", master_filename);
	    return 1;
	}
	masterdata = fi_slurp(fi, &masterlength);
	if(!masterdata) {
	    fprintf(stderr, "Failed to read from %s\n", master_filename);
	    return 1;
	}
	logf("<debug> Read %d bytes from masterfile\n", masterlength);
	fclose(fi);
    }

    for(t=0;t<numslaves;t++) {
	    logf("<verbose> slave entity(%d) %s (%s \"%s\")\n", t+1, slave_filename[t], 
		    slave_isframe[t]?"frame":"object", slave_name[t]);
    }

    if(config.dummy)
    {
	if(numslaves)
	{
	    logf("<error> --dummy (-d) implies there are zero slave objects. You supplied %d.", numslaves);
	    exit(1);
	}
	numslaves = 1;
	slave_filename[0] = "!!dummy!!";
	slave_name[0] = "!!dummy!!";
	slave_isframe[0] = 0;
    }

    if (config.alloctest)
    {
	int*bitmap = malloc(sizeof(int)*65536);
	memset(bitmap, -1, 65536*sizeof(int));
	memset(bitmap, 1, 101*sizeof(int));
	swf_relocate(masterdata, masterlength, bitmap);
	newdata = masterdata;
	newlength = masterlength;
	free(bitmap);
    }
    else
    {
	if (!numslaves)
	{
	    logf("<error> You must have at least one slave entity.");
	    return 0;
	}
	for(t = 0; t < numslaves; t++)
	{
	    config.movex = slave_movex[t];
	    config.movey = slave_movey[t];
	    config.scalex = slave_scalex[t];
	    config.scaley = slave_scaley[t];
	    config.isframe = slave_isframe[t];

	    logf("<notice> Combine [%s]%s and [%s]%s", master_name, master_filename,
		    slave_name[t], slave_filename[t]);
	    if(!config.dummy)
	    {
		fi = fopen(slave_filename[t], "rb");
		if(!fi) {
		    fprintf(stderr, "Failed to open %s\n", slave_filename[t]);
		    return 1;
		}
		slavedata = fi_slurp(fi, &slavelength);
		if(!slavedata) {
		    fprintf(stderr, "Failed to read from %s\n", slave_filename[t]);
		    return 1;
		}
		logf("<debug> Read %d bytes from slavefile\n", slavelength);
		fclose(fi);
	    }
	    else
	    {
		slavedata = (u8*)malloc(16);
		slavedata[0] = 'F';
		slavedata[1] = 'W';
		slavedata[2] = 'S';
		slavedata[3] = 4; //version
		PUT32(&slavedata[4], 14); ; // length
		slavedata[8] = 0; // boundingbox
		PUT16(&slavedata[9] , (0)); // rate
		PUT16(&slavedata[11] , (0)); // count
		PUT16(&slavedata[13] , (0)); // end tag
		slavelength = 17;
	    }

	    newdata = combine(masterdata, masterlength, slave_name[t], slavedata, slavelength, &newlength);
	    if(!newdata) { 
		logf("<fatal> Aborting.");
		return 1;
	    }

	    free(masterdata);
	    masterdata = newdata;
	    masterlength = newlength;
	}
    }

    logf("<debug> New File is %d bytes \n", newlength);
    if(newdata && newlength) {
	FILE*fi = fopen(outputname, "wb");
	fi_dump(fi, newdata, newlength);
	fclose(fi);
    }
    return 0;
}

