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
#include "../lib/rfxswf.h"
#include "./flash.h"
#include "./reloc.h"
#include "./settings.h"

static char* slavename = 0;
static int slaveid = -1;
static int slaveframe = -1;

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
    /* fucking big endian byte order */
    if(tag->id == TAGID_PLACEOBJECT)
	(*(u16*)&tag->data[2]) =
	SWAP16(SWAP16(*(u16*)&tag->data[2]) + add);
    if(tag->id == TAGID_PLACEOBJECT2)
	(*(u16*)&tag->data[1]) =
	SWAP16(SWAP16(*(u16*)&tag->data[1]) + add);
    if(tag->id == TAGID_REMOVEOBJECT)
	(*(u16*)&tag->data[2]) =
	SWAP16(SWAP16(*(u16*)&tag->data[2]) + add);
    if(tag->id == TAGID_REMOVEOBJECT2)
	(*(u16*)&tag->data[0]) =
	SWAP16(SWAP16(*(u16*)&tag->data[0]) + add);
}

void jpeg_assert()
{
    /* TODO: if there's a jpegtable found, store it
       and handle it together with the flash file
       headers */
    /* check that master and slave don't have both
       jpegtables (which would be fatal) */
    int pos;
    int mpos=-1, spos=-1;
    pos = 0;
    while(master.tags[pos].id != 0)
    {
	if(master.tags[pos].id == TAGID_JPEGTABLES)
	    mpos = pos;
	pos++;
    }
    pos = 0;
    while(master.tags[pos].id != 0)
    {
	if(slave.tags[pos].id == TAGID_JPEGTABLES)
	    spos = pos;
	pos++;
    }
    if(spos>=0 && mpos>=0)
    {
	if(slave.tags[pos].length ==
	   master.tags[pos].length &&
	!memcmp(slave.tags[pos].data, master.tags[pos].data,
	    master.tags[pos].length))
	{
	    // ok, both have jpegtables, but they're identical.
	    // delete one and don't make an error
	    for(;spos<slave.tagnum-1;spos++)
		slave.tags[spos] =
		    slave.tags[spos+1];
	    spos = -1;
	}
    }
    if(spos>=0 && mpos>=0) {
	logf("<error> Master and slave have incompatible JPEGTABLES.");
    }
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
			/* if we get here, jpeg_assert has already run,
			   ensuring this is the only one of it's kind,
			   so we may safely write it out */
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
    tmp = SWAP16(0x3f + (TAGID_DEFINESPRITE << 6));
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
	tmp = SWAP16(7 + (TAGID_PLACEOBJECT2 << 6));
	writer_write(w, &tmp, 2);
	tmp = SWAP16(2+64); //flags: character + clipaction
	writer_write(w, &tmp, 1);
	tmp = SWAP16(0); //depth
	writer_write(w, &tmp,2);
	tmp = SWAP16(replaceddefine); //id
	writer_write(w, &tmp,2);
	tmp = SWAP16(65535); //clipdepth
	writer_write(w, &tmp,2);
    }

    if(config.overlay && !config.isframe) {
	tmp = SWAP16(5 + (TAGID_PLACEOBJECT2 << 6));
	writer_write(w, &tmp, 2);
	tmp = SWAP16(2); //flags: character
	writer_write(w, &tmp, 1);
	tmp = SWAP16(0); //depth
	writer_write(w, &tmp,2);
	tmp = SWAP16(replaceddefine); //id
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

    *tagidpos = SWAP32((u8*)writer_getpos(w) - startpos); // set length of sprite (in header)
    logf("<verbose> sprite length is %d",SWAP32(*tagidpos));
}

static char tag_ok_for_slave(int id)
{
    if(id == TAGID_BACKGROUNDCOLOR)
	return 0;
    return 1;
}

#define FLAGS_WRITEDEFINES 1
#define FLAGS_WRITENONDEFINES 2
#define FLAGS_WRITESPRITE 4
#define FLAGS_WRITESLAVE 8
void write_master(struct writer_t*w, int spriteid, int replaceddefine, int flags)
{
    int pos = 0;
    int spos = 0;
    int outputslave = 0;
    int frame = 0;
    int sframe = 0;
    int slavewritten = 0;
    while(master.tags[pos].id != 0)
    {
	if(master.tags[pos].id == TAGID_SHOWFRAME && outputslave)
	{
	    while(slave.tags[spos].id) {
		if(slave.tags[spos].id == TAGID_SHOWFRAME) {
		    spos++;
		    sframe++;
		    break;
		}
		if(tag_ok_for_slave(slave.tags[spos].id))
		    writer_write(w, slave.tags[spos].fulldata, slave.tags[spos].fulllength);
		spos++;
	    }
	    frame ++;
	}

	if(is_defining_tag(master.tags[pos].id) && (flags&FLAGS_WRITEDEFINES))
	{
	    logf("<debug> [master] write tag %02x (%d bytes in body)", 
		    master.tags[pos].id, master.tags[pos].length);
	    if(getidfromtag(&master.tags[pos]) == spriteid && !config.isframe)
	    {
		if(config.overlay)
		{
		    *(u16*)master.tags[pos].data = SWAP16(replaceddefine);
		    writer_write(w, master.tags[pos].fulldata, master.tags[pos].fulllength);
		} else {
		    /* don't write this tag */
		    logf("<verbose> replacing tag %d id %d with sprite", master.tags[pos].id
			    ,spriteid);
		}

		if(flags&FLAGS_WRITESPRITE)
		{
		    write_sprite_defines(w);
		    write_sprite(w, spriteid, replaceddefine);
		}
		if(flags&FLAGS_WRITESLAVE)
		{
		    outputslave = 1;
		}
	    } else { 
		writer_write(w, master.tags[pos].fulldata, master.tags[pos].fulllength);
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
		int id = get_free_id();
		int depth = 0;
		char data[7];
		if(config.clip) {
		    logf("<fatal> Can't combine --clip and --frame");
		}
		*(u16*)&data[0] = SWAP16((u16)(TAGID_PLACEOBJECT2<<6) + 5);
		*(u8*)&data[2]= SWAP16(2); //flags: id
		*(u16*)&data[3]= SWAP16(depth); // depth
		*(u16*)&data[5]= SWAP16(id);
		write_sprite_defines(w);
		write_sprite(w, id, -1);
		writer_write(w,data,7);
		slavewritten = 1;
	    }
	}
	if(!is_defining_tag(master.tags[pos].id) && (flags&FLAGS_WRITENONDEFINES))
	{
	    int dontwrite = 0;
	    switch(master.tags[pos].id) {
		case TAGID_PLACEOBJECT:
		case TAGID_PLACEOBJECT2:
		    if(frame == slaveframe && !config.overlay)
			dontwrite = 1;
		case TAGID_REMOVEOBJECT:
//		case TAGID_REMOVEOBJECT2:
		    /* place/removetags for the object we replaced
		       should be discarded, too, as the object to insert 
		       isn't a sprite 
		     */
		    if(spriteid>=0 && getidfromtag(&master.tags[pos]) == spriteid && 
			    !config.isframe && config.merge)
			dontwrite = 1;
		break;
	    }
	    if(!dontwrite) {
		logf("<debug> [master] write tag %02x (%d bytes in body)", 
			master.tags[pos].id, master.tags[pos].length);
		writer_write(w, master.tags[pos].fulldata, master.tags[pos].fulllength);
	    }
	}
	pos++;
    }
   
    if(outputslave) 
    while(slave.tags[spos].id)
    {
	    if(tag_ok_for_slave(slave.tags[spos].id))
		writer_write(w, slave.tags[spos].fulldata, slave.tags[spos].fulllength);
	    spos++;
    }
    if(!slavewritten && config.isframe && (flags&(FLAGS_WRITESLAVE|FLAGS_WRITESPRITE)))
    {
	logf("<warning> Frame %d doesn't exist in file. No substitution will occur",
		slaveframe);
    }
    //write END tag: 
    writer_write(w, master.tags[pos].fulldata, master.tags[pos].fulllength);
}

void writeheader(struct writer_t*w, u8*data, int length)
{
    if(config.hassizex || config.hassizey || config.framerate)
    {
	struct flash_header head;
	struct reader_t reader;
	swf_init(&reader, data-3, length+3);
	head = swf_read_header(&reader);
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

uchar * catcombine(uchar*masterdata, int masterlength, char*_slavename, uchar*slavedata, int slavelength, int*newlength)
{
	struct writer_t w;
	u32*headlength;
	u32 tmp32;
	int length = masterlength*2 + slavelength;
	int pos = 0;
	int t;
	char* depths;
	uchar*newdata = malloc(length);
	if(!newdata) {
	    logf("<fatal> Couldn't allocate %d bytes of memory", length);
	    return 0;
	}
	if(config.isframe) {
	    logf("<fatal> Can't combine --cat and --frame");
	    exit(1);
	}
	writer_init(&w, newdata, length);
	
	do {
	    int tag = master.tags[pos].id;
	    if(is_defining_tag(tag)) {
		int defineid = getidfromtag(&master.tags[pos]);
		logf("<debug> tagid %02x defines object %d", tag, defineid);
		masterids[defineid] = 1;
	    }
	}
	while(master.tags[pos++].id != 0);
	
	swf_relocate (slavedata, slavelength, masterids);
	read_swf(&slave, slavedata, slavelength);
	jpeg_assert();
	
	writer_write(&w, "FWS",3);
	headlength = (u32*)(writer_getpos(&w) + 1);
	writeheader(&w, master.header.headerdata, master.header.headerlength);

	depths = malloc(65536);
	if(!depths) {
	    logf("<fatal> Couldn't allocate %d bytes of memory", 65536);
	    return 0;
	}
	memset(depths, 0, 65536);
	pos = 0;
	do {
	    int num=1;
	    u16 depth;
	    logf("<debug> [master] write tag %02x (%d bytes in body)", 
		    master.tags[pos].id, master.tags[pos].length);
	    switch(master.tags[pos].id) {
		case TAGID_PLACEOBJECT2:
		    num++;
		case TAGID_PLACEOBJECT: {
		   struct reader_t r;
		   reader_init (&r, master.tags[pos].data, master.tags[pos].length);
		   if(num>=2)
			reader_readu8(&r);
		   depth = reader_readu16(&r);
		   depths[depth] = 1;
		}
	        break;
		case TAGID_REMOVEOBJECT: {
		   struct reader_t r;
		   reader_init (&r, master.tags[pos].data, master.tags[pos].length);
		   reader_readu16(&r);
		   depths[reader_readu16(&r)] = 0;
		}
		break;
		case TAGID_REMOVEOBJECT2: {
		   struct reader_t r;
		   reader_init (&r, master.tags[pos].data, master.tags[pos].length);
		   depths[reader_readu16(&r)] = 0;
		}
		break;
	    }
	    if(master.tags[pos].id != 0)
		writer_write(&w, master.tags[pos].fulldata, master.tags[pos].fulllength);
	}
	while(master.tags[pos++].id != 0);

	for(t=0;t<65536;t++) 
	if(depths[t])
	{
	    char data[16];
	    int len;
	    *(u16*)(&data[0]) = SWAP16((TAGID_REMOVEOBJECT2<<6) + 2);
	    *(u16*)(&data[2]) = SWAP16(t);
	    writer_write(&w, data, 4);
	}
	free(depths);

	pos = 0;
	do {
	    logf("<debug> [slave] write tag %02x (%d bytes in body)", 
		    slave.tags[pos].id, slave.tags[pos].length);
	    writer_write(&w, slave.tags[pos].fulldata, slave.tags[pos].fulllength);
	}
	while(slave.tags[pos++].id != 0);

	tmp32 = (u8*)writer_getpos(&w) - (u8*)newdata; //length
	*newlength = tmp32;
	*headlength = SWAP32(tmp32); // set the header to the correct length

	return newdata; //length
}

uchar * normalcombine(uchar*masterdata, int masterlength, char*_slavename, uchar*slavedata, int slavelength, int*newlength)
{
	int length;
	int pos=0;
	u32 tmp32;
	u32*headlength;
	uchar*newdata;
	int spriteid = -1;
	int replaceddefine = -1;
	struct writer_t w;
	int frame;
	char*framelabel;
	
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
	    } else if(tag == TAGID_SHOWFRAME) {
		if(slaveframe>=0 && frame==slaveframe) {
		    logf("<notice> Slave file attached to frame %d.", frame);
		}
		frame++;
	    } else if(tag == TAGID_FRAMELABEL) {
		char * name = master.tags[pos].data;
		if(name && slavename && config.isframe && !strcmp(name, slavename)) {
		    slaveframe = frame;
		    logf("<notice> Slave file attached to frame %d (%s).", frame, name);
		}
	    }
	}
	while(master.tags[pos++].id != 0);

	if (spriteid<0 && !config.isframe) {
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
	jpeg_assert();
	
	if (config.overlay)
	    replaceddefine = get_free_id();
	
	// write file 

	writer_write(&w, "FWS",3);
	headlength = (u32*)(writer_getpos(&w) + 1);
	writeheader(&w, master.header.headerdata, master.header.headerlength);

	if (config.antistream) {
	    if (config.merge) {
		logf("<fatal> Can't combine --antistream and --merge");
	    }
	    write_sprite_defines(&w);
	    write_sprite(&w, spriteid, replaceddefine);
	    write_master(&w, spriteid, replaceddefine, FLAGS_WRITEDEFINES);
	    write_master(&w, spriteid, replaceddefine, FLAGS_WRITENONDEFINES);
	} else {
	    if (config.merge)
		write_master(&w, spriteid, replaceddefine, 
		    FLAGS_WRITEDEFINES|FLAGS_WRITENONDEFINES|FLAGS_WRITESLAVE);
	    else
		write_master(&w, spriteid, replaceddefine, 
		    FLAGS_WRITEDEFINES|FLAGS_WRITENONDEFINES|FLAGS_WRITESPRITE);
	}

	tmp32 = (u8*)writer_getpos(&w) - (u8*)newdata; //length
	*newlength = tmp32;
	*headlength = SWAP32(tmp32); // set the header to the correct length

	return newdata; //length
}

uchar * combine(uchar*masterdata, int masterlength, char*_slavename, uchar*slavedata, int slavelength, int*newlength)
{
    char master_flash = 0;
    char slave_flash = 0;
    slavename = _slavename;

    slaveid = -1;
    slaveframe = -1;

    if(slavename[0] == '#')
    {
	slaveid = atoi(&slavename[1]);
	slavename = 0;
    }
    if(config.isframe)
    {
	slaveframe = slaveid;
	slaveid = -1;
    }

    logf("<debug> move x (%d)", config.movex);
    logf("<debug> move y (%d)", config.movey);
    logf("<debug> scale x (%d)", config.scalex);
    logf("<debug> scale y (%d)", config.scaley);
    logf("<debug> is frame (%d)", config.isframe);
    
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

    if(master_flash && slave_flash) {
	read_swf(&master, masterdata, masterlength);
	if(config.cat) 
	    return catcombine(masterdata, masterlength, _slavename, slavedata, slavelength, newlength);
	else
	    return normalcombine(masterdata, masterlength, _slavename, slavedata, slavelength, newlength);
    }
    
    *newlength = 0;
    return 0;
}
