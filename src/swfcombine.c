/* swfcombine.c
   main routine for swfcombine(1), a tool for merging .swf-files.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "../lib/log.h"
#include "../config.h"

struct config_t
{
   char overlay;
   char alloctest;
   char clip;
   char stack;
   char stack1;
   char antistream;
   char dummy;
   char zlib;
   char cat;
   char merge;
   char isframe;
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
struct config_t config;

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
    else if (!strcmp(name, "z"))
    {
	config.zlib = 1;
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
 {"z","zlib"},
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
    printf("Usage: %s [-rXYomlcv] [-f] masterfile [-xysf] [(name1|#id1)=]slavefile1 .. [-xysf] [(nameN|#idN)=]slavefileN\n", name);
    printf("OR:    %s [-rXYomv] --stack[1] [-xysf] [(name1|#id1)=]slavefile1 .. [-xysf] [(nameN|#idN)=]slavefileN\n", name);
    printf("OR:    %s [-rXYov] --cat [-xysf] [(name1|#id1)=]slavefile1 .. [-xysf] [(nameN|#idN)=]slavefileN\n", name);
    printf("OR:    %s [-rXYomlcv] --dummy [-xys] [file]\n", name);
    printf("\n");
    printf("-o outputfile       --output    explicitly specify output file. (otherwise, output.swf will be used\n");
    printf("-t                  --stack     place each slave in a seperate frame (no master movie)\n");
    printf("-T                  --stack1    place each slave in the first frame (no master movie)\n");
    printf("-m                  --merge     Don't store the slaves in Sprites/MovieClips\n");
    printf("-a                  --cat       concatenate all slave files (no master movie)\n");
    printf("-l                  --overlay   Don't remove any master objects, only overlay new objects\n");
    printf("-c                  --clip      Clip the slave objects by the corresponding master objects\n");
    printf("-v                  --verbose   Use more than one -v for greater effect \n");
    printf("-d                  --dummy     Don't require slave objects \n");
    printf("-f                  --frame     The following identifier is a frame or framelabel, not an id or objectname\n");
    printf("-x xpos             --movex     x Adjust position of slave by xpos twips (1/20 pixel)\n");
    printf("-y ypos             --movey     y Adjust position of slave by ypos twips (1/20 pixel)\n");
    printf("-s scale            --scale     Adjust size of slave by scale%\n");
    printf("-r framerate        --rate      Set movie framerate (100 frames/sec)\n");
    printf("-X width            --width     Force movie width to scale (default: use master width (not with -t))\n");
    printf("-Y height           --height    Force movie height to scale (default: use master height (not with -t))\n");
    printf("-z zlib             --zlib      Enable Flash 6 (MX) Zlib Compression\n");
}

static void makestackmaster(SWF*swf)
{
    TAG*tag;
    int t;
    SRECT box;
    int fileversion = 1;
    int frameRate = 256;
    RGBA rgb;
    rgb.r=rgb.b=rgb.g=0;
    memset(&box, 0, sizeof(box));

    /* scan all slaves for bounding box */
    for(t=numslaves-1;t>=0;t--)
    {
	SWF head;
	int ret;
	int fi=open(slave_filename[t],O_RDONLY);
	TAG*tag;
	if(fi<0 || swf_ReadSWF(fi, &head)<0) {
	    logf("<fatal> Couldn't open/read %s.", slave_filename[t]);
	    exit(1);
	}
	close(fi);
	logf("<verbose> File %s has bounding box %d:%d:%d:%d\n",
		slave_filename[t], 
		head.movieSize.xmin, head.movieSize.ymin,
		head.movieSize.xmax, head.movieSize.ymax);

	tag = head.firstTag;
	while(tag) {
	    if(tag->id == ST_SETBACKGROUNDCOLOR && tag->len>=3) {
		rgb.r = tag->data[0];
		rgb.g = tag->data[1];
		rgb.b = tag->data[2];
	    }
	    tag=tag->next;
	}
	frameRate = head.frameRate;
	if(head.fileVersion > fileversion)
	    fileversion = head.fileVersion;
	if(!t)
	    box = head.movieSize;
	else {
	    if(head.movieSize.xmin < box.xmin)
		box.xmin = head.movieSize.xmin;
	    if(head.movieSize.ymin < box.ymin)
		box.ymin = head.movieSize.ymin;
	    if(head.movieSize.xmax > box.xmax)
		box.xmax = head.movieSize.xmax;
	    if(head.movieSize.ymax > box.ymax)
		box.ymax = head.movieSize.ymax;
	}
	logf("<verbose> New master bounding box is %d:%d:%d:%d\n",
		box.xmin, box.ymin,
		box.xmax, box.ymax);
	swf_FreeTags(&head);
    }

    memset(swf, 0, sizeof(SWF));
    swf->fileVersion = fileversion;
    swf->movieSize = box;
    swf->frameRate = frameRate;

    swf->firstTag = swf_InsertTag(0, ST_SETBACKGROUNDCOLOR);
    tag = swf->firstTag;
    swf_SetRGB(tag, &rgb);
    
    for(t=0;t<numslaves;t++)
    {
	char buf[128];
	sprintf(buf, "Frame%02d", t);
	slave_name[t] = strdup(buf);

	tag = swf_InsertTag(tag, ST_DEFINESPRITE);
	swf_SetU16(tag, t+1);
	swf_SetU16(tag, 0);
	tag = swf_InsertTag(tag, ST_END);
	tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
	swf_ObjectPlace(tag, t+1, 1+t,0,0, slave_name[t]);

	if(!config.stack1 || t == numslaves-1) {
	    tag = swf_InsertTag(tag, ST_SHOWFRAME);
	}
	if(!config.stack)
	if(t!=numslaves-1)
	{
	    tag = swf_InsertTag(tag, ST_REMOVEOBJECT2);
	    swf_SetU16(tag, 1+t);
	}
    }
    tag = swf_InsertTag(tag, ST_END);
    logf("<verbose> temporary SWF created");
}

static char* slavename = 0;
static int slaveid = -1;
static int slaveframe = -1;
static char masterbitmap[65536];

#define FLAGS_WRITEDEFINES 1
#define FLAGS_WRITENONDEFINES 2
#define FLAGS_WRITESPRITE 4
#define FLAGS_WRITESLAVE 8

int get_free_id(char*bitmap)
{
    int t;
    for(t=1;t<65536;t++)
	if(!bitmap[t]) {
	    bitmap[t] = 1;
	    return t;
	}
    return -1;
}

void jpeg_assert(SWF*master, SWF*slave)
{
    /* TODO: if there's a jpegtable found, store it
       and handle it together with the flash file
       headers */

    /* check that master and slave don't have both
       jpegtables (which would be fatal) */
    int pos;
    TAG *mpos=0, *spos=0;
    TAG *mtag,*stag;
    pos = 0;
    mtag = master->firstTag;
    stag = slave->firstTag;
    while(mtag)
    {
	if(mtag->id  == ST_JPEGTABLES)
	    mpos = mtag;
	mtag = mtag->next;
    }
    while(stag)
    {
	if(stag->id == ST_JPEGTABLES)
	    spos = stag;
	stag = stag->next;
    }
    if(mpos && spos)
    {
	if(spos->len == mpos->len &&
	!memcmp(spos->data, mpos->data, mpos->len))
	{
	    // ok, both have jpegtables, but they're identical.
	    // delete one and don't throw an error
	    swf_DeleteTag(spos);
	    spos = 0;
	}
    }
    if(spos && mpos) {
	logf("<error> Master and slave have incompatible JPEGTABLES.");
    }
}

TAG* write_sprite_defines(TAG*tag, SWF*sprite)
{
    TAG*rtag = sprite->firstTag;
    while(rtag && rtag->id!=ST_END) {
	if(!swf_isAllowedSpriteTag(rtag)) {
	    logf("<debug> processing sprite tag %02x", tag->id);
	    if(swf_isDefiningTag(rtag))
	    {
		logf("<debug> [sprite defs] write tag %02x (%d bytes in body)", 
			tag->id, tag->len);
		tag = swf_InsertTag(tag, rtag->id);
		swf_SetBlock(tag, rtag->data, rtag->len);
	    }
	    else if(swf_isPseudoDefiningTag(rtag))
	    {
		logf("<debug> [sprite defs] write tag %02x (%d bytes in body)", 
			tag->id, tag->len);
		tag = swf_InsertTag(tag, rtag->id);
		swf_SetBlock(tag, rtag->data, rtag->len);
	    }
	    else {
		switch(rtag->id)
		{
		    case ST_JPEGTABLES:
			   /* if we get here, jpeg_assert has already run,
			      ensuring this is the only one of it's kind,
			      so we may safely write it out */
			   tag = swf_InsertTag(tag, rtag->id);
			   swf_SetBlock(tag, rtag->data, rtag->len);
		       break;
		    case ST_EXPORTASSETS:
		       logf("<debug> deliberately ignoring EXPORTASSETS tag");
		       break;
		    case ST_ENABLEDEBUGGER:
		       logf("<debug> deliberately ignoring ENABLEDEBUGGER tag");
		       break;
		    case ST_SETBACKGROUNDCOLOR:
		       logf("<debug> deliberately ignoring BACKGROUNDCOLOR tag");
		       break;
		    case 40:
		    case 49:
		    case 51:
		       logf("<notice> found tag %d. This is a Generator template, isn't it?", tag->id);
		       break;
		    default:
		       logf("<notice> funny tag: %d is neither defining nor sprite", tag->id);
		}
	    }
	}
	rtag = rtag->next;
    }
    return tag;
}

void changedepth(TAG*tag, int add)
{
    /* fucking byteorders */
    if(tag->id == ST_PLACEOBJECT)
	PUT16(&tag->data[2],GET16(&tag->data[2])+add);
    if(tag->id == ST_PLACEOBJECT2)
	PUT16(&tag->data[1],GET16(&tag->data[1])+add);
    if(tag->id == ST_REMOVEOBJECT)
	PUT16(&tag->data[2],GET16(&tag->data[2])+add);
    if(tag->id == ST_REMOVEOBJECT2)
	PUT16(&tag->data[0],GET16(&tag->data[0])+add);
}

void matrix_adjust(MATRIX*m)
{
    m->sx = (int)(m->sx*config.scalex);
    m->sy = (int)(m->sy*config.scaley);
    m->r0 = (int)(m->r0*config.scalex);
    m->r1 = (int)(m->r1*config.scaley);
    m->tx += config.movex;
    m->ty += config.movey;
}

void write_changepos(TAG*output, TAG*tag)
{
    if(config.movex || config.movey || config.scalex != 1 || config.scaley != 1)
    {
	switch(tag->id)
	{
	    case ST_PLACEOBJECT2: {
		MATRIX m;
		U8 flags;
		swf_GetMatrix(0, &m);

		flags = swf_GetU8(tag);
		swf_SetU8(output, flags|4);
		swf_SetU16(output, swf_GetU16(tag)); //depth
		//flags&1: move
		if(flags&2) {
		    swf_SetU16(output, swf_GetU16(tag)); //id
		}
		// flags & 4
		swf_GetMatrix(tag, &m);
		matrix_adjust(&m);
		swf_SetMatrix(output, &m);

		//swf_ResetReadBits(tag);
		swf_SetBlock(output, &tag->data[tag->pos], tag->len - tag->pos);
		break;
	    }
	    case ST_PLACEOBJECT: {
		MATRIX m;
		swf_SetU16(output, swf_GetU16(tag)); //id
		swf_SetU16(output, swf_GetU16(tag)); //depth
		
		swf_GetMatrix(tag, &m);
		matrix_adjust(&m);
		swf_SetMatrix(output, &m);
		
		//swf_ResetReadBits(tag);
		swf_SetBlock(output, &tag->data[tag->pos], tag->len - tag->pos);
		break;
	    }
	    default:
	    swf_SetBlock(output, tag->data, tag->len);
	}
    } 
    else 
    {
	    swf_SetBlock(output, tag->data, tag->len);
    }
}

TAG* write_sprite(TAG*tag, SWF*sprite, int spriteid, int replaceddefine)
{
    TAG* definespritetag;
    TAG* rtag;
    int tmp;

    definespritetag = tag = swf_InsertTag(tag, ST_DEFINESPRITE);
    swf_SetU16(tag, spriteid);
    swf_SetU16(tag, sprite->frameCount);
    logf ("<notice> sprite id is %d", spriteid);

    tmp = sprite->frameCount;
    logf("<debug> %d frames to go",tmp);

    if(config.clip) {
	tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
	swf_SetU8(tag, 2+64); //flags: character+clipdepth
	swf_SetU16(tag, 0); //depth
	swf_SetU16(tag, replaceddefine); //id
	swf_SetU16(tag, 65535); //clipdepth
    }

    if(config.overlay && !config.isframe) {
	tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
	swf_SetU8(tag, 2); //flags: character
	swf_SetU16(tag, 0); //depth
	swf_SetU16(tag, replaceddefine); //id
    }

    rtag = sprite->firstTag;
    while(rtag && rtag->id!=ST_END)
    {
	if (swf_isAllowedSpriteTag(rtag)) {

	    logf("<debug> [sprite main] write tag %02x (%d bytes in body)", 
		    rtag->id, rtag->len);
	    tag = swf_InsertTag(tag, rtag->id);
	    write_changepos(tag, rtag);
	  
	    changedepth(tag, +1);

	    if(tag->id == ST_SHOWFRAME)
	    {
		tmp--;
		logf("<debug> %d frames to go",tmp);
	    }
	}
	rtag = rtag->next;
    }
    tag = swf_InsertTag(tag, ST_END);
    return tag;
}

static char tag_ok_for_slave(int id)
{
    if(id == ST_SETBACKGROUNDCOLOR)
	return 0;
    return 1;
}

TAG* write_master(TAG*tag, SWF*master, SWF*slave, int spriteid, int replaceddefine, int flags)
{
    int outputslave = 0;
    int frame = 0;
    int sframe = 0;
    int slavewritten = 0;

    TAG* rtag = master->firstTag;
    TAG* stag = slave->firstTag;

    while(rtag && rtag->id!=ST_END)
    {
	if(rtag->id == ST_SHOWFRAME && outputslave)
	{
	    while(stag && stag->id!=ST_END) {
		if(stag->id == ST_SHOWFRAME) {
		    stag = stag->next;
		    sframe++;
		    break;
		}
		if(tag_ok_for_slave(stag->id)) {
		    tag = swf_InsertTag(tag, stag->id);
		    swf_SetBlock(tag, stag->data, stag->len);
		}
		stag = stag->next;
	    }
	}
	if(rtag->id == ST_SHOWFRAME)
	{
	    frame ++;
	}

	if(swf_isDefiningTag(rtag) && (flags&FLAGS_WRITEDEFINES))
	{
	    logf("<debug> [master] write tag %02x (%d bytes in body)", 
		    rtag->id, rtag->len);
	    if(swf_GetDefineID(rtag) == spriteid && !config.isframe)
	    {
		if(config.overlay)
		{
		    tag = swf_InsertTag(tag, rtag->id);
		    swf_SetBlock(tag, rtag->data, rtag->len);
		    swf_SetDefineID(tag, replaceddefine);
		} else {
		    /* don't write this tag */
		    logf("<verbose> replacing tag %d id %d with sprite", rtag->id
			    ,spriteid);
		}

		if(flags&FLAGS_WRITESPRITE)
		{
		    tag = write_sprite_defines(tag, slave);
		    tag = write_sprite(tag, slave, spriteid, replaceddefine);
		}
		if(flags&FLAGS_WRITESLAVE)
		{
		    outputslave = 1;
		}
	    } else { 
		tag = swf_InsertTag(tag, rtag->id);
		swf_SetBlock(tag, rtag->data, rtag->len);
	    }
	}
	if(frame == slaveframe)
	{
	    if(flags&FLAGS_WRITESLAVE) {
		outputslave = 1;
		slavewritten = 1;
	    }
	    if((flags&FLAGS_WRITESPRITE) && !slavewritten)
	    {
		int id = get_free_id(masterbitmap);
		int depth = 0;
		if(config.clip) {
		    logf("<fatal> Can't combine --clip and --frame");
		}
		
		tag = write_sprite_defines(tag, slave);
		tag = write_sprite(tag, slave, id, -1);

		tag = swf_InsertTag(tag, ST_PLACEOBJECT2);
		    swf_SetU8(tag, 2); //flags: id
		    swf_SetU16(tag, depth);
		    swf_SetU16(tag, id);

		slavewritten = 1;
	    }
	}
	if(!swf_isDefiningTag(rtag) && (flags&FLAGS_WRITENONDEFINES))
	{
	    int dontwrite = 0;
	    switch(rtag->id) {
		case ST_PLACEOBJECT:
		case ST_PLACEOBJECT2:
		    if(frame == slaveframe && !config.overlay)
			dontwrite = 1;
		case ST_REMOVEOBJECT:
//		case ST_REMOVEOBJECT2:
		    /* place/removetags for the object we replaced
		       should be discarded, too, as the object to insert 
		       isn't a sprite 
		     */
		    if(spriteid>=0 && swf_GetPlaceID(rtag) == spriteid && 
			    !config.isframe && config.merge)
			dontwrite = 1;
		break;
	    }
	    if(!dontwrite) {
		logf("<debug> [master] write tag %02x (%d bytes in body)", 
			rtag->id, rtag->len);
		tag = swf_InsertTag(tag, rtag->id);
		swf_SetBlock(tag, rtag->data, rtag->len);
	    }
	}
	rtag = rtag->next;
    }
   
    if(outputslave) 
    while(stag && stag->id!=ST_END)
    {
	    if(tag_ok_for_slave(stag->id)) {
		tag = swf_InsertTag(tag, stag->id);
		swf_SetBlock(tag, stag->data, stag->len);
	    }
	    stag = stag->next;
    }
    if(!slavewritten && config.isframe && (flags&(FLAGS_WRITESLAVE|FLAGS_WRITESPRITE)))
    {
	if(slaveframe>=0)
	    logf("<warning> Frame %d doesn't exist in file. No substitution will occur",
		    slaveframe);
	else
	    logf("<warning> Frame \"%s\" doesn't exist in file. No substitution will occur",
		    slavename);
    }
    tag = swf_InsertTag(tag, ST_END);
    return tag;
}

void adjustheader(SWF*swf)
{
    if(config.framerate)
	swf->frameRate = config.framerate;
    if(config.hassizex) {
	swf->movieSize.xmax = 
	swf->movieSize.xmin + config.sizex;
    }
    if(config.hassizey) {
	swf->movieSize.ymax = 
	swf->movieSize.ymin + config.sizey;
    }
}

void catcombine(SWF*master, char*slave_name, SWF*slave, SWF*newswf)
{
    char* depths;
    int t;
    TAG*tag;
    TAG*mtag,*stag;
    if(config.isframe) {
	logf("<fatal> Can't combine --cat and --frame");
	exit(1);
    }
   
    tag = master->firstTag;
    while(tag)
    {
	if(swf_isDefiningTag(tag)) {
	    int defineid = swf_GetDefineID(tag);
	    logf("<debug> tagid %02x defines object %d", tag->id, defineid);
	    masterbitmap[defineid] = 1;
	}
	tag = tag->next;
    }
    
    swf_Relocate(slave, masterbitmap);
    jpeg_assert(master, slave);
    
    memcpy(newswf, master, sizeof(SWF));
    adjustheader(newswf);

    tag = newswf->firstTag = swf_InsertTag(0, ST_REFLEX); // to be removed later

    depths = malloc(65536);
    if(!depths) {
	logf("<fatal> Couldn't allocate %d bytes of memory", 65536);
	return;
    }
    memset(depths, 0, 65536);
    mtag = master->firstTag;
    while(mtag && mtag->id!=ST_END)
    {
	int num=1;
	U16 depth;
	logf("<debug> [master] write tag %02x (%d bytes in body)", 
		mtag->id, mtag->len);
	switch(mtag->id) {
	    case ST_PLACEOBJECT2:
		num++;
	    case ST_PLACEOBJECT: {
	       depth = swf_GetDepth(mtag);
	       depths[depth] = 1;
	    }
	    break;
	    case ST_REMOVEOBJECT: {
	       depth = swf_GetDepth(mtag);
	       depths[depth] = 0;
	    }
	    break;
	    case ST_REMOVEOBJECT2: {
	       depth = swf_GetDepth(mtag);
	       depths[depth] = 0;
	    }
	    break;
	}
	tag = swf_InsertTag(tag, mtag->id);
	swf_SetBlock(tag, mtag->data, mtag->len);

	mtag = mtag->next;
    }

    for(t=0;t<65536;t++) 
    if(depths[t])
    {
	char data[16];
	int len;
	tag = swf_InsertTag(tag, ST_REMOVEOBJECT2);
	swf_SetU16(tag, t);
    }
    free(depths);

    stag = slave->firstTag;
    while(stag && stag->id!=ST_END)
    {
	logf("<debug> [slave] write tag %02x (%d bytes in body)", 
		stag->id, stag->len);
	tag = swf_InsertTag(tag, stag->id);
	swf_SetBlock(tag, stag->data, stag->len);
	stag = stag->next;
    }
    tag = swf_InsertTag(tag, ST_END);

    tag = newswf->firstTag;
    newswf->firstTag = newswf->firstTag->next; //remove temporary tag
    swf_DeleteTag(tag);
}

void normalcombine(SWF*master, char*slave_name, SWF*slave, SWF*newswf)
{
    int spriteid = -1;
    int replaceddefine = -1;
    int frame = 0;
    char*framelabel;
    TAG * tag = master->firstTag;
    
    // set the idtab
    while(tag)
    {
	if(swf_isDefiningTag(tag)) {
	    int defineid = swf_GetDefineID(tag);
	    logf("<debug> tagid %02x defines object %d", tag->id, defineid);
	    masterbitmap[defineid] = 1;
	} else if(tag->id == ST_PLACEOBJECT2) {
	    char * name = swf_GetName(tag);
	    int id = swf_GetPlaceID(tag);

	    if(name)
	      logf("<verbose> tagid %02x places object %d named \"%s\"", tag->id, id, name);
	    else
	      logf("<verbose> tagid %02x places object %d (no name)", tag->id, id);

	    if ((name && slavename && !strcmp(name,slavename)) || 
		(!slavename && id==slaveid)) {
		if(id>=0) {
		  spriteid = id;
		  logf("<notice> Slave file attached to object %d.", id);
		}
	    }
	} else if(tag->id == ST_SHOWFRAME) {
	    if(slaveframe>=0 && frame==slaveframe) {
		logf("<notice> Slave file attached to frame %d.", frame);
	    }
	    frame++;
	} else if(tag->id == ST_FRAMELABEL) {
	    char * name = tag->data;
	    if(name && slavename && config.isframe && !strcmp(name, slavename)) {
		slaveframe = frame;
		logf("<notice> Slave file attached to frame %d (%s).", frame, name);
	    }
	}
	tag = tag->next;
    };

    if (spriteid<0 && !config.isframe) {
	if(slavename) {
	    if(strcmp(slavename,"!!dummy!!"))
		logf("<warning> Didn't find anything named %s in file. No substitutions will occur.", slavename);
	}
	else
	    logf("<warning> Didn't find id %d in file. No substitutions will occur.", slaveid);
	spriteid = get_free_id(masterbitmap);
    }

    swf_Relocate (slave, masterbitmap);
    jpeg_assert(slave, master);
    
    if (config.overlay)
	replaceddefine = get_free_id(masterbitmap);
    
    // write file 

    memcpy(newswf, master, sizeof(SWF));
    adjustheader(newswf);

    newswf->firstTag = tag = swf_InsertTag(0, ST_REFLEX); // to be removed later

    if (config.antistream) {
	if (config.merge) {
	    logf("<fatal> Can't combine --antistream and --merge");
	}
	tag = write_sprite_defines(tag, slave);
	tag = write_sprite(tag, slave, spriteid, replaceddefine);
	tag = write_master(tag, master, slave, spriteid, replaceddefine, FLAGS_WRITEDEFINES);
	tag = write_master(tag, master, slave, spriteid, replaceddefine, FLAGS_WRITENONDEFINES);
    } else {
	if (config.merge)
	    tag = write_master(tag, master, slave, spriteid, replaceddefine, 
		FLAGS_WRITEDEFINES|FLAGS_WRITENONDEFINES|FLAGS_WRITESLAVE);
	else
	    tag = write_master(tag, master, slave, spriteid, replaceddefine, 
		FLAGS_WRITEDEFINES|FLAGS_WRITENONDEFINES|FLAGS_WRITESPRITE);
    }

    tag = newswf->firstTag;
    newswf->firstTag = newswf->firstTag->next; //remove temporary tag
    swf_DeleteTag(tag);
}

void combine(SWF*master, char*slave_name, SWF*slave, SWF*newswf)
{
    slavename = slave_name;
    slaveid = -1;
    slaveframe = -1;

    swf_FoldAll(master);
    swf_FoldAll(slave);

    if(slavename[0] == '#')
    {
	slaveid = atoi(&slavename[1]);
	slavename = 0;
    }

    if(config.isframe)
    {
	int tmp;
	if(slavename && slavename[0]!='#' && (sscanf(slavename, "%d", &tmp) ==
		strlen(slavename))) {
	/* if the name the slave should replace 
	   consists only of digits and the -f
	   option is given, it probably is not
	   a frame name but a frame number.
	 */
	    slaveid = tmp;
	    slavename = 0;
	}

	if(slaveid>=0) {
	    slaveframe = slaveid;
	    slaveid = -1;
	} else {
	/* if id wasn't given as either #number or number,
	   the name is a frame label. BTW: The user wouldn't have
	   needed to supply the -f option in this case */
	}
    }

    logf("<debug> move x (%d)", config.movex);
    logf("<debug> move y (%d)", config.movey);
    logf("<debug> scale x (%f)", config.scalex);
    logf("<debug> scale y (%f)", config.scaley);
    logf("<debug> is frame (%d)", config.isframe);
    
    memset(masterbitmap, 0, sizeof(masterbitmap));

    if(config.cat) 
	return catcombine(master, slave_name, slave, newswf);
    else
	return normalcombine(master, slave_name, slave, newswf);
}

int main(int argn, char *argv[])
{
    int fi;
    SWF master;
    SWF slave;
    SWF newswf;
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
    config.zlib = 0;

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

	makestackmaster(&master);
    }
    else {
	int ret;
	logf("<verbose> master entity %s (named \"%s\")\n", master_filename, master_name);
	fi = open(master_filename, O_RDONLY);
	if(fi<0) {
	    logf("<fatal> Failed to open %s\n", master_filename);
	    exit(1);
	}
	ret = swf_ReadSWF(fi, &master);
	if(ret<0) {
	    logf("<fatal> Failed to read from %s\n", master_filename);
	    exit(1);
	}
	logf("<debug> Read %d bytes from masterfile\n", ret);
	close(fi);
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
	char*bitmap = malloc(sizeof(char)*65536);
	memset(bitmap, 0, 65536*sizeof(char));
	memset(bitmap, 1, 101*sizeof(char));
	swf_Relocate(&master, bitmap);
	newswf = master;
	free(bitmap);
//	makestackmaster(&newswf);
    }
    else
    {
	if (!numslaves)
	{
	    if(config.cat)
		logf("<error> You must have at least two objects.");
	    else
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
		int ret;
		fi = open(slave_filename[t], O_RDONLY);
		if(!fi) {
		    logf("<fatal> Failed to open %s\n", slave_filename[t]);
		    exit(1);
		}
		ret = swf_ReadSWF(fi, &slave);
		if(ret<0) {
		    logf("<fatal> Failed to read from %s\n", slave_filename[t]);
		    exit(1);
		}
		logf("<debug> Read %d bytes from slavefile\n", ret);
		close(fi);
	    }
	    else
	    {
		memset(&slave, 0, sizeof(slave));
		slave.firstTag = swf_InsertTag(0, ST_END);
		slave.frameRate = 0;
		slave.fileVersion = 4;
		slave.frameCount = 0;
	    }

	    combine(&master, slave_name[t], &slave, &newswf);
	    master = newswf;
	}
    }

    fi = open(outputname, O_RDWR|O_TRUNC|O_CREAT, 0777);

    if(config.zlib)
	swf_WriteSWC(fi, &newswf);
    else {
	newswf.compressed = 0;
	swf_WriteSWF(fi, &newswf);
    }
    close(fi);
    return 0;
}

