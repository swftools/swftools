/* reloc.h
   Implements swf_relocate(), which changes the id range of a swf file in
   memory.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include "flash.h"

static struct swffile file;

int slaveids[65536];

void maponeid(void*idpos)
{
    u16*idptr = (u16*)idpos;
    if(slaveids[*idptr]<0) {
	logf("<error> Trying to map id never encountered before: id=%d", *idptr);
	return ;
    }
    logf("<debug> mapping %d to %d", *idptr, slaveids[*idptr]);
    *idptr =  slaveids[*idptr];
}


void mapstyles(int num, void(*callback)(void*))
{
    u16 count;
    int t;
    resetbits();
    count = readu8();
    if(count == 0xff && num>1) // defineshape2,3 only
	count = readu16();

//	    printf("%d fillstyles\n", count);
    for(t=0;t<count;t++)
    {
	int type;
	u8*pos;
	pos=getinputpos();
//		printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", 
//			pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7]);
	resetbits();
	type = readu8(); //type
//		printf("fillstyle %d is type 0x%02x\n", t, type);
	if(type == 0) {
//	    printf("solid fill\n");
	    if(num == 3)
		readRGBA();
	    else 
		readRGB();
	}
	else if(type == 0x10 || type == 0x12)
	{
//	    printf("gradient fill\n");
	    resetbits();
	    readMATRIX();
	    resetbits();
	    readGRADIENT(num);
	}
	else if(type == 0x40 || type == 0x41)
	{
	    resetbits();
	    // we made it.
//	    printf("bitmap fill:%04x\n", *(u16*)getinputpos());
	    if(*(u16*)getinputpos() != 65535)
		(callback)(getinputpos());

	    readu16();
	    resetbits();
	    readMATRIX();
	}
	else {
	    logf("<error> Unknown fillstyle:0x%02x\n",type);
	}
    }
    resetbits();
    count = readu8(); // line style array
//	    printf("%d linestyles\n", count);
    if(count == 0xff)
	count = readu16();
    for(t=0;t<count;t++) 
    {
	readu16();
	if(num == 3)
	    readRGBA();
	else
	    readRGB();
    }
}

// take a memory region which contains a tag, and
// map the ids inside this tag to new values
void map_ids_mem(u8*mem, int length, void(*callback)(void*))
{
    int num=1;
    struct swf_tag newtag_instance;
    struct swf_tag*newtag = &newtag_instance;
    reader_init (mem, length);
    swf_read_tag(newtag);

    switch(newtag->id)
    {
	case TAGID_DEFINEBUTTONCXFORM: {
	    int t;
	    callback(&newtag->data[0]); //button id
	    reader_init (newtag->data, newtag->length);
	    for(t=0;t<4;t++) {
		int flags;
		callback(&newtag->data[0]);
		readu16(); //sound id
		flags = readu8();
		if(flags&1)
		    readu32(); // in point
		if(flags&2)
		    readu32(); // out points
		if(flags&4)
		    readu16(); // loop count
		if(flags&8)
		{
		    int npoints = readu8();
		    int s;
		    for(s=0;s<npoints;s++)
		    {
			readu32();
			readu16();
			readu16();
		    }
		}
	    }
        } break;
	case TAGID_DEFINEBUTTONSOUND:
	    callback(&newtag->data[0]); //button id
	break;
	case TAGID_PLACEOBJECT:
	    callback(&newtag->data[0]);
        break;
	case TAGID_PLACEOBJECT2:
	    // only if placeflaghascharacter
	    if(!(newtag->data[0]&2))
		break;
	    callback(&newtag->data[3]);
        break;
	case TAGID_REMOVEOBJECT:
	    callback(&newtag->data[0]);
	break;
	case TAGID_STARTSOUND:
	    callback(&newtag->data[0]);
	break;
	case TAGID_DEFINESPRITE: {
	    u8*mem = &newtag->data[4];
	    int len = newtag->length-4;

	    while(1) {
		u8*fmem = mem;
		int flen = len;
		struct swf_tag sprtag;

		reader_init (mem, len);
		swf_read_tag (&sprtag);

		mem = getinputpos();
		len = getinputlength();

		if(sprtag.id == TAGID_END)
		    break;

		map_ids_mem (fmem,flen,callback);
	    }
	} 
	break;
	case TAGID_DEFINEBUTTON2: // has some font ids in the button records
	    num++; 
	//fallthrough
	case TAGID_DEFINEBUTTON:
	    reader_init (newtag->data, newtag->length);
	    readu16(); //button id
	    if(num>1)
	    { 
		int offset;
		readu8(); //flag
		offset = readu16(); //offset
	    }
	    while(1)
	    {
		u16 charid;
		if(!readu8()) //flags
		    break; 
		charid = *(u16*)getinputpos();
		callback(getinputpos());
		readu16(); //char
		readu16(); //layer
		resetbits();
		readMATRIX();
		if(num>1) {
		  resetbits();
		  readCXFORM(1);
		}
	    }
	    // ...
	break;
	case TAGID_DEFINEEDITTEXT:  {
	    u8 flags1,flags2;
	    reader_init (newtag->data, newtag->length);
	    readu16(); //id
	    readRECT(); //bounding box
	    resetbits();
	    flags1 = readu8();
	    flags2 = readu8();
	    if(flags1 & 128)
		callback(getinputpos());
	}
	break;
	case TAGID_DEFINETEXT2:
	    num ++;
	case TAGID_DEFINETEXT: { 
	    int glyphbits, advancebits;
	    int id;
	    reader_init (newtag->data, newtag->length);
	    id = readu16(); //id
	    readRECT(); //bounding box
	    resetbits();
	    readMATRIX(); //matrix
	    resetbits();
	    glyphbits = readu8(); //glyphbits
	    advancebits = readu8(); //advancebits
	    while(1) {
		u16 flags;
		resetbits();
		flags = getbits(8);
		if(!flags) break;
		if(flags & 128) // text style record
		{
		    resetbits();
		    if(flags & 8) { // hasfont
			callback(getinputpos());
			id = readu16();
		    }
		    if(flags & 4) { // hascolor
			if(num==1) readRGB();
			else       readRGBA();
		    }
		    if(flags & 2) { //has x offset
			resetbits();
			readu16();
		    }
		    if(flags & 1) { //has y offset
			resetbits();
			readu16();
		    }
		    if(flags & 8) { //has height
			resetbits();
			readu16();
		    }
		} else { // glyph record
		    int t;
		    resetbits();
		    for(t=0;t<flags;t++) {
			getbits(glyphbits);
			getbits(advancebits);
		    }
		}
	    }
	    break;
	}
	case TAGID_DEFINEFONTINFO:
	    callback(&newtag->data[0]);
	break;

	case TAGID_DEFINESHAPE3: // these thingies might have bitmap ids in their fillstyles
	num++; //fallthrough
	case TAGID_DEFINESHAPE2:
	num++; //fallthrough
	case TAGID_DEFINESHAPE: {
	    int fillbits;
	    int linebits;
	    struct RECT r;
	    //printf("defineshape%d\n", num);
	    reader_init (newtag->data, newtag->length);
	    readu16(); // id;
	    r = readRECT(); // bounds
//	    printf("%d shape bounds: %d %d %d %d\n",newtag->id,r.x1,r.y1,r.x2,r.y2);

	    mapstyles(num, callback);
	    fillbits = getbits(4);
	    linebits = getbits(4);
	    resetbits();
	    //printf("%d %d\n", fillbits, linebits);
	    while(1) {
		int flags;
		/*printf("data: %02x %02x >%02x< %02x %02x\n",
		    ((u8*)getinputpos())[-2],
		    ((u8*)getinputpos())[-1],
		    ((u8*)getinputpos())[0],
		    ((u8*)getinputpos())[1],
		    ((u8*)getinputpos())[2]);*/
		flags = getbits(1);
		if(!flags) { //style change
		    flags = getbits(5);
		    //printf("style flags:%02x\n",flags);
		    if(!flags)
			break;
		    if(flags&1) { //move
			int n = getbits(5); 
			//printf("n:%d\n",n);
			getbits(n); //x
			getbits(n); //y
		    }
		    if(flags&2) { //fill0
			getbits(fillbits); 
		    }
		    if(flags&4) { //fill1
			getbits(fillbits); 
		    }
		    if(flags&8) { //linestyle
			getbits(linebits); 
		    }
		    if(flags&16) {
			mapstyles(num, callback);
			fillbits = getbits(4);
			linebits = getbits(4);
		    }
		} else {
		    flags = getbits(1);
		    //printf("edge:%d\n", flags);
		    if(flags) { //straight edge
			int n = getbits(4) + 2;
			if(getbits(1)) { //line flag
			    getbits(n); //delta x
			    getbits(n); //delta y
			} else {
			    int v=getbits(1);
			    getbits(n); //vert/horz
			}
		    } else { //curved edge
			int n = getbits(4) + 2;
			getbits(n);
			getbits(n);
			getbits(n);
			getbits(n);
		    }
		}
	    }
	}
	break;
	default:
	break;
    }
}

static int*bitmap;

static int get_free_id()
{
    int t;
    for (t=1;t<65536;t++)
    {
	if(bitmap[t] == -1)
	{
	    bitmap[t] = 1;
	    return t;
	}
    }
    return -1;
}

static struct swf_tag* map_ids(struct swf_tag*tag)
{
    map_ids_mem(tag->fulldata, tag->fulllength, maponeid);
    return tag;
}

void swf_relocate (u8*data, int length, int*_bitmap)
{
    int pos;
    bitmap = _bitmap;
    read_swf(&file, data, length);
    memset(slaveids, -1, sizeof(slaveids));

    pos = 0;
    while(file.tags[pos].id != 0) {
	struct swf_tag*tag = &file.tags[pos];
        
	logf("<debug> relocator: processing tag %02x", tag->id);
	map_ids(&file.tags[pos]);

	if(is_defining_tag(tag->id))
	{
	    int newid;
	    int id;
	    
	    id = getidfromtag(tag); //own id

	    if(bitmap[id] < 0) { //free
		newid = id;
	    }
	    else {
		newid = get_free_id(id);
	    }
	    bitmap[newid] = 1;
	    slaveids[id] = newid;

	    logf("<debug> relocator: id %d mapped to %d",id, newid);
	    
	    setidintag(tag, newid);

	    logf("<debug> [reloc] write tag %02x (%d bytes in body)", 
		    tag->id, tag->length);
	} 
        pos++;
    }
}

