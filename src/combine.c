/* combine.c 
   Implements combine(), which merges two swfs in memory.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "../lib/log.h"
#include "./flash.h"
#include "./reloc.h"
#include "./settings.h"

// TODO:
// * readers should be object-oriented

static char* slavename;
static int slaveid;

static char* tag_placeobject2_name (struct swf_tag* tag)
{
    struct PlaceObject2 plo2;
    placeobject2_init (&plo2, tag);
    return plo2.name;
}

static u16 tag_placeobject2_character (struct swf_tag* tag)
{
    struct PlaceObject2 plo2;
    placeobject2_init (&plo2, tag);
    return plo2.id;
}

static struct swffile master;
static struct swffile slave;

static int masterids[65536];

static int get_free_id()
{
    int t;
    for (t=1;t<65536;t++)
    {
	if(masterids[t] == -1)
	{
	    masterids[t] = 1;
	    return t;
	}
    }
    return -1;
}

void changedepth(struct swf_tag*tag, int add)
{
    if(tag->id == TAGID_PLACEOBJECT)
	(*(u16*)&tag->data[2]) += add;
    if(tag->id == TAGID_PLACEOBJECT2)
	(*(u16*)&tag->data[1]) += add;
    if(tag->id == TAGID_REMOVEOBJECT)
	(*(u16*)&tag->data[2]) += add;
    if(tag->id == TAGID_REMOVEOBJECT2)
	(*(u16*)&tag->data[0]) += add;
}

/* applies the config move and scale parameters to
 * a matrix. (If settings would provide a rotation,
 * this would be a matrix concatenation/multiplication
 * routine. In this case, it's just element-wise multiplication.
 */
void matrix_adjust(struct MATRIX*m)
{
    if(config.scalex != 1 || config.scaley != 1)
    {
	if(!m->hasscale) {
	    m->hasscale = 1;
	    m->a[0][0] = config.scalex;
	    m->a[1][1] = config.scaley;
	} else {
	    m->a[0][0] *= config.scalex;
	    m->a[1][1] *= config.scaley;
	}
	if(m->hasrotate) {
	    m->a[0][1] *= config.scalex;
	    m->a[1][0] *= config.scaley;
	}
	m->b[0] *= config.scalex;
	m->b[1] *= config.scaley;
    }
/*    printf("hasscale: %d\n",m->hasscale);
    printf("hasrotate: %d\n", m->hasrotate);
    printf("move: %d %d\n", m->b[0],m->b[1]);
    printf("rot: %f %f\n",m->a[0][0],m->a[0][1]);
    printf("     %f %f\n",m->a[1][0],m->a[1][1]);*/
    m->b[0] += config.movex;
    m->b[1] += config.movey;
}

void write_changepos(struct swf_tag*tag, struct writer_t*w)
{
    if(config.movex || config.movey || config.scalex != 1 || config.scaley != 1)
    {
	switch(tag->id)
	{
	    case TAGID_PLACEOBJECT: {
		struct PlaceObject p;
		placeobject_init(&p, tag);
		matrix_adjust(&p.matrix);
		placeobject_write(&p, w);
		break;
	    }
	    case TAGID_PLACEOBJECT2: {
		struct PlaceObject2 p;
		placeobject2_init(&p, tag);
		if(!p.hasmatrix) {
		    p.hasmatrix = 1;
		    MATRIX_init(&p.matrix);
		}
	        matrix_adjust(&p.matrix);
		placeobject2_write(&p, w);
		break;
	    }
	    default:
	    writer_write(w, tag->fulldata, tag->fulllength);
	}
    } 
    else 
    {
	    writer_write(w, tag->fulldata, tag->fulllength);
    }
}

void write_sprite_defines(struct writer_t*w)
{
    int pos = 0;
    while(slave.tags[pos].id != 0) {
	struct swf_tag * tag = &slave.tags[pos];
	if(!is_sprite_tag(tag->id)) {
	    logf("<debug> processing sprite tag %02x", slave.tags[pos].id);
	    if(is_defining_tag(tag->id))
	    {
		logf("<debug> [sprite defs] write tag %02x (%d bytes in body)", 
			tag->id, tag->length);
		writer_write(w, tag->fulldata, tag->fulllength);
	    }
	    else
	    {
		switch(tag->id)
		{case TAGID_DEFINEFONTINFO:
		    {
			/* define font info is not a defining tag, in that
			 * it doesn't define a new id, but rather extends
			 * an existing one. It also isn't a sprite tag. 
			 * Anyway we can't throw it out, so we just pass it
			 * through.
			 */
			writer_write(w, tag->fulldata, tag->fulllength);
			break;
		    }
		 case TAGID_JPEGTABLES:
			/* according to the flash specs, there may only 
			   be one JPEGTABLES tag per swf. This is maybe
			   a big FIXME */
			writer_write(w, tag->fulldata, tag->fulllength);
		    break;
		 case TAGID_EXPORTASSETS:
		    logf("<debug> deliberately ignoring EXPORTASSETS tag");
		    break;
		 case TAGID_ENABLEDEBUGGER:
		    logf("<debug> deliberately ignoring ENABLEDEBUGGER tag");
		    break;
		 case TAGID_BACKGROUNDCOLOR:
		    logf("<debug> deliberately ignoring BACKGROUNDCOLOR tag");
		    break;
		 case 40:
		 case 49:
		 case 51:
		    logf("<notice> found tag %d. This is a Generator template, isn't it?", slave.tags[pos].id);
		    break;
		 default:
		    logf("<notice> funny tag: %d is neither defining nor sprite", slave.tags[pos].id);
		}
	    }
	}
	pos++;
    }
}


void write_sprite(struct writer_t*w, int spriteid, int replaceddefine)
{
    u16 tmp;
    u32 tmp32;
    u32*tagidpos;
    u8*startpos;
    int pos = 0;
    // write slave(2) (header)
    tmp = 0x3f + (TAGID_DEFINESPRITE << 6);
    writer_write(w, &tmp, 2);
    tagidpos = (u32*)writer_getpos(w);
    writer_write(w, &tmp32, 4);
    
    startpos = (u8*)writer_getpos(w);

    logf ("<notice> sprite id is %d", spriteid);
    tmp = spriteid;
    writer_write(w, &tmp, 2);
    tmp = slave.header.count;
    writer_write(w, &tmp, 2);


    // write slave(2) (body)
    tmp = slave.header.count;
    logf("<debug> %d frames to go",tmp);

    if(config.clip) {
	tmp = 7 + (TAGID_PLACEOBJECT2 << 6);
	writer_write(w, &tmp, 2);
	tmp = 2+64; //flags: character + clipaction
	writer_write(w, &tmp, 1);
	tmp = 0; //depth
	writer_write(w, &tmp,2);
	tmp = replaceddefine; //id
	writer_write(w, &tmp,2);
	tmp = 65535; //clipdepth
	writer_write(w, &tmp,2);
    }

    if(config.overlay) {
	tmp = 5 + (TAGID_PLACEOBJECT2 << 6);
	writer_write(w, &tmp, 2);
	tmp = 2; //flags: character
	writer_write(w, &tmp, 1);
	tmp = 0; //depth
	writer_write(w, &tmp,2);
	tmp = replaceddefine; //id
	writer_write(w, &tmp,2);
    }

    do {
	struct swf_tag * tag = &slave.tags[pos];
	if (is_sprite_tag(tag->id)) {

	    changedepth(tag, +1);
	    logf("<debug> [sprite main] write tag %02x (%d bytes in body)", 
		    slave.tags[pos].id, slave.tags[pos].length);
	    write_changepos(tag, w);

	    if(tag->id == TAGID_SHOWFRAME)
	    {
		tmp--;
		logf("<debug> %d frames to go",tmp);
	    }
	}
    }
    while(slave.tags[pos++].id != TAGID_END);

    *tagidpos = (u8*)writer_getpos(w) - startpos; // set length of sprite (in header)
    logf("<verbose> sprite length is %d",*tagidpos);
}

#define FLAGS_WRITEDEFINES 1
#define FLAGS_WRITENONDEFINES 2
#define FLAGS_WRITESPRITE 4
void write_master(struct writer_t*w, int spriteid, int replaceddefine, int flags)
{
    int pos = 0;
    do {
	if(is_defining_tag(master.tags[pos].id) && (flags&1))
	{
	    logf("<debug> [master] write tag %02x (%d bytes in body)", 
		    master.tags[pos].id, master.tags[pos].length);
	    if( getidfromtag(&master.tags[pos]) == spriteid) 
	    {
		if(config.overlay)
		{
		    *(u16*)master.tags[pos].data = replaceddefine;
		    writer_write(w, master.tags[pos].fulldata, master.tags[pos].fulllength);
		} else {
		    /* don't write this tag */
		    logf("<verbose> replacing tag %d id %d with sprite", master.tags[pos].id
			    ,spriteid);
		}

		if(flags&4)
		{
		    write_sprite_defines(w);
		    write_sprite(w, spriteid, replaceddefine);
		}
	    } else { 
		writer_write(w, master.tags[pos].fulldata, master.tags[pos].fulllength);
	    }
	}
	if(!is_defining_tag(master.tags[pos].id) && (flags&2))
	{
	    logf("<debug> [master] write tag %02x (%d bytes in body)", 
		    master.tags[pos].id, master.tags[pos].length);
	    writer_write(w, master.tags[pos].fulldata, master.tags[pos].fulllength);
	}
    }
    while(master.tags[pos++].id != 0);
}

void writeheader(struct writer_t*w, u8*data, int length)
{
    if(config.hassizex || config.hassizey || config.framerate)
    {
	struct flash_header head;
	swf_init(data-3, length+3);
	head = swf_read_header();
	if(config.hassizex)
	{
	    head.boundingBox.x2 = head.boundingBox.x1 + config.sizex;
	}
	if(config.hassizey)
	{
	    head.boundingBox.y2 = head.boundingBox.y1 + config.sizey;
	}
	if(config.framerate)
	{
	    head.rate = config.framerate;
	}
	swf_write_header(w, &head);
    }
    else
    writer_write(w, data, length);
}

uchar * combine(uchar*masterdata, int masterlength, char*_slavename, uchar*slavedata, int slavelength, int*newlength)
{
    char master_flash = 0;
    char slave_flash = 0;
    slavename = _slavename;
    if(slavename[0] == '#')
    {
	slaveid = atoi(&slavename[1]);
	slavename = 0;
    }

    logf("<debug> move x (%d)", config.movex);
    logf("<debug> move y (%d)", config.movey);
    logf("<debug> scale x (%d)", config.scalex);
    logf("<debug> scale y (%d)", config.scaley);
    
    memset(masterids, -1, sizeof(masterids));

    if(masterlength < 3)
    {
	logf("<fatal> the master file is too small (%d bytes)", masterlength);
	return 0;
    }
    if(slavelength < 3)
    {
	logf("<fatal> the slave file is too small (%d bytes)", slavelength);
	return 0;
    }
    if(masterdata[2] == 'S' &&
       masterdata[1] == 'W' &&
       masterdata[0] == 'F')
    {
	logf("<notice> the master file is flash (swf) format\n");
	master_flash = 1;
    }
    else
	logf("<notice> the master file is not flash (swf) format!\n");

    if(slavedata[2] == 'S' &&
       slavedata[1] == 'W' &&
       slavedata[0] == 'F')
    {
	logf("<notice> the slave file is flash (swf) format\n");
	slave_flash = 1;
    }
    else
	logf("<notice> the slave file is not flash (swf) format!\n");

    if(master_flash && slave_flash)
    {
	int length;
	int pos=0;
	u32 tmp32;
	u32*headlength;
	uchar*newdata;
	int spriteid = -1;
	int replaceddefine = -1;
	struct writer_t w;
	
	read_swf(&master, masterdata, masterlength);

	length = masterlength + slavelength*2 + 128; // this is a guess, but a good guess.
	newdata = malloc(length);
	writer_init(&w, newdata, length);

	if(!newdata) {
	    logf("<fatal> Couldn't allocate %d bytes of memory", length);
	    return 0;
	}

	// set the idtab
	pos = 0;
        do {
	    int tag = master.tags[pos].id;
	    if(is_defining_tag(tag)) {
		int defineid = getidfromtag(&master.tags[pos]);
		logf("<debug> tagid %02x defines object %d", tag, defineid);
		masterids[defineid] = 1;
	    } else if(tag == TAGID_PLACEOBJECT2) {
		char * name = tag_placeobject2_name(&master.tags[pos]);
		int id = tag_placeobject2_character(&master.tags[pos]);

		if(name)
		  logf("<verbose> tagid %02x places object %d named \"%s\"", tag, id, name);
		else
		  logf("<verbose> tagid %02x places object %d (no name)", tag, id);

		if ((name && slavename && !strcmp(name,slavename)) || 
		    (!slavename && id==slaveid)) {
		    if(id>=0) {
		      spriteid = id;
		      logf("<notice> Slave file attached to object %d.", id);
		    }
		}
	    }
	}
	while(master.tags[pos++].id != 0);

	if (spriteid<0)
	{
	    if(slavename) {
		if(strcmp(slavename,"!!dummy!!"))
		    logf("<warning> Didn't find anything named %s in file. No substitutions will occur.", slavename);
	    }
	    else
		logf("<warning> Didn't find id %d in file. No substitutions will occur.", slaveid);
	    spriteid = get_free_id();
	}

	swf_relocate (slavedata, slavelength, masterids);

	read_swf(&slave, slavedata, slavelength);
	
	if(config.overlay)
	    replaceddefine = get_free_id();
	
	// write file 

	writer_write(&w, "FWS",3);
	headlength = (u32*)(writer_getpos(&w) + 1);
	writeheader(&w, master.header.headerdata, master.header.headerlength);

	if(config.antistream) {
	    write_sprite_defines(&w);
	    write_sprite(&w, spriteid, replaceddefine);
	    write_master(&w, spriteid, replaceddefine, FLAGS_WRITEDEFINES);
	    write_master(&w, spriteid, replaceddefine, FLAGS_WRITENONDEFINES);
	} else {
	    write_master(&w, spriteid, replaceddefine, 
		    FLAGS_WRITEDEFINES|FLAGS_WRITENONDEFINES|FLAGS_WRITESPRITE);
	}

	tmp32 = (u8*)writer_getpos(&w) - (u8*)newdata; //length
	*newlength = tmp32;
	*headlength = tmp32; // set the header to the correct length

	return newdata; //length
    }
    
    *newlength = 0;
    return 0;
}
