/* reloc.h
   Implements swf_relocate(), which changes the id range of a swf file in
   memory.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include "flash.h"
#include "../lib/rfxswf.h"

static struct swffile file;

int slaveids[65536];

void maponeid(void*idpos)
{
    u16*idptr = (u16*)idpos;
    if(slaveids[SWAP16(*idptr)]<0) {
	logf("<error> Trying to map id never encountered before: id=%d", *idptr);
	return ;
    }
    logf("<debug> mapping %d to %d", SWAP16(*idptr), slaveids[*idptr]);
    *idptr =  SWAP16(slaveids[*idptr]);
}


void mapstyles(struct reader_t*reader, int num, void(*callback)(void*))
{
    u16 count;
    int t;
    reader_resetbits(reader);
    count = reader_readu8(reader);
    if(count == 0xff && num>1) // defineshape2,3 only
	count = reader_readu16(reader);

//	    printf("%d fillstyles\n", count);
    for(t=0;t<count;t++)
    {
	int type;
	u8*pos;
	pos=reader_getinputpos(reader);
//		printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", 
//			pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7]);
	reader_resetbits(reader);
	type = reader_readu8(reader); //type
//		printf("fillstyle %d is type 0x%02x\n", t, type);
	if(type == 0) {
//	    printf("solid fill\n");
	    if(num == 3)
		readRGBA(reader);
	    else 
		readRGB(reader);
	}
	else if(type == 0x10 || type == 0x12)
	{
//	    printf("gradient fill\n");
	    reader_resetbits(reader);
	    readMATRIX(reader);
	    reader_resetbits(reader);
	    readGRADIENT(reader, num);
	}
	else if(type == 0x40 || type == 0x41)
	{
	    reader_resetbits(reader);
	    // we made it.
//	    printf("bitmap fill:%04x\n", *(u16*)getinputpos());
	    if(*(u16*)reader_getinputpos(reader) != 65535)
		(callback)(reader_getinputpos(reader));

	    reader_readu16(reader);
	    reader_resetbits(reader);
	    readMATRIX(reader);
	}
	else {
	    logf("<error> Unknown fillstyle:0x%02x\n",type);
	}
    }
    reader_resetbits(reader);
    count = reader_readu8(reader); // line style array
//	    printf("%d linestyles\n", count);
    if(count == 0xff)
	count = reader_readu16(reader);
    for(t=0;t<count;t++) 
    {
	reader_readu16(reader);
	if(num == 3)
	    readRGBA(reader);
	else
	    readRGB(reader);
    }
}

// take a memory region which contains a tag, and
// map the ids inside this tag to new values
void map_ids_mem(u8*mem, int length, void(*callback)(void*))
{
    int num=1;
    struct swf_tag newtag_instance;
    struct swf_tag*newtag = &newtag_instance;
    struct reader_t reader;
    reader_init (&reader, mem, length);
    swf_read_tag(&reader, newtag);

    switch(newtag->id)
    {
	case TAGID_DEFINEBUTTONCXFORM: {
	    int t;
	    struct reader_t reader;
	    callback(&newtag->data[0]); //button id
	    reader_init (&reader, newtag->data, newtag->length);
	    for(t=0;t<4;t++) {
		int flags;
		callback(&newtag->data[0]);
		reader_readu16(&reader); //sound id
		flags = reader_readu8(&reader);
		if(flags&1)
		    reader_readu32(&reader); // in point
		if(flags&2)
		    reader_readu32(&reader); // out points
		if(flags&4)
		    reader_readu16(&reader); // loop count
		if(flags&8)
		{
		    int npoints = reader_readu8(&reader);
		    int s;
		    for(s=0;s<npoints;s++)
		    {
			reader_readu32(&reader);
			reader_readu16(&reader);
			reader_readu16(&reader);
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
		struct reader_t reader;

		reader_init (&reader, mem, len);
		swf_read_tag (&reader, &sprtag);

		mem = reader_getinputpos(&reader);
		len = reader_getinputlength(&reader);

		if(sprtag.id == TAGID_END)
		    break;

		map_ids_mem (fmem,flen,callback);
	    }
	} 
	break;
	case TAGID_DEFINEBUTTON2: // has some font ids in the button records
	    num++; 
	//fallthrough
	case TAGID_DEFINEBUTTON: {
	    struct reader_t reader;
	    reader_init (&reader, newtag->data, newtag->length);
	    reader_readu16(&reader); //button id
	    if(num>1)
	    { 
		int offset;
		reader_readu8(&reader); //flag
		offset = reader_readu16(&reader); //offset
	    }
	    while(1)
	    {
		u16 charid;
		if(!reader_readu8(&reader)) //flags
		    break; 
		charid = *(u16*)reader_getinputpos(&reader);
		callback(reader_getinputpos(&reader));
		reader_readu16(&reader); //char
		reader_readu16(&reader); //layer
		reader_resetbits(&reader);
		readMATRIX(&reader);
		if(num>1) {
		  reader_resetbits(&reader);
		  readCXFORM(&reader, 1);
		}
	    }
	    // ...
	}
	break;
	case TAGID_DEFINEEDITTEXT:  {
	    u8 flags1,flags2;
	    struct reader_t reader;
	    reader_init (&reader, newtag->data, newtag->length);
	    reader_readu16(&reader); //id
	    readRECT(&reader); //bounding box
	    reader_resetbits(&reader);
	    flags1 = reader_readu8(&reader);
	    flags2 = reader_readu8(&reader);
	    if(flags1 & 128)
		callback(reader_getinputpos(&reader));
	}
	break;
	case TAGID_DEFINETEXT2:
	    num ++;
	case TAGID_DEFINETEXT: { 
	    int glyphbits, advancebits;
	    int id;
	    struct reader_t reader;
	    reader_init (&reader, newtag->data, newtag->length);
	    id = reader_readu16(&reader); //id
	    readRECT(&reader); //bounding box
	    reader_resetbits(&reader);
	    readMATRIX(&reader); //matrix
	    reader_resetbits(&reader);
	    glyphbits = reader_readu8(&reader); //glyphbits
	    advancebits = reader_readu8(&reader); //advancebits
	    while(1) {
		u16 flags;
		reader_resetbits(&reader);
		flags = reader_getbits(&reader, 8);
		if(!flags) break;
		if(flags & 128) // text style record
		{
		    reader_resetbits(&reader);
		    if(flags & 8) { // hasfont
			callback(reader_getinputpos(&reader));
			id = reader_readu16(&reader);
		    }
		    if(flags & 4) { // hascolor
			if(num==1) readRGB(&reader);
			else       readRGBA(&reader);
		    }
		    if(flags & 2) { //has x offset
			reader_resetbits(&reader);
			reader_readu16(&reader);
		    }
		    if(flags & 1) { //has y offset
			reader_resetbits(&reader);
			reader_readu16(&reader);
		    }
		    if(flags & 8) { //has height
			reader_resetbits(&reader);
			reader_readu16(&reader);
		    }
		} else { // glyph record
		    int t;
		    reader_resetbits(&reader);
		    for(t=0;t<flags;t++) {
			reader_getbits(&reader, glyphbits);
			reader_getbits(&reader, advancebits);
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
	    struct reader_t reader;
	    //printf("defineshape%d\n", num);
	    reader_init (&reader, newtag->data, newtag->length);
	    reader_readu16(&reader); // id;
	    r = readRECT(&reader); // bounds
//	    printf("%d shape bounds: %d %d %d %d\n",newtag->id,r.x1,r.y1,r.x2,r.y2);

	    mapstyles(&reader, num, callback);
	    fillbits = reader_getbits(&reader, 4);
	    linebits = reader_getbits(&reader, 4);
	    reader_resetbits(&reader);
	    //printf("%d %d\n", fillbits, linebits);
	    while(1) {
		int flags;
		/*printf("data: %02x %02x >%02x< %02x %02x\n",
		    ((u8*)getinputpos())[-2],
		    ((u8*)getinputpos())[-1],
		    ((u8*)getinputpos())[0],
		    ((u8*)getinputpos())[1],
		    ((u8*)getinputpos())[2]);*/
		flags = reader_getbits(&reader, 1);
		if(!flags) { //style change
		    flags = reader_getbits(&reader, 5);
		    //printf("style flags:%02x\n",flags);
		    if(!flags)
			break;
		    if(flags&1) { //move
			int n = reader_getbits(&reader, 5); 
			//printf("n:%d\n",n);
			reader_getbits(&reader, n); //x
			reader_getbits(&reader, n); //y
		    }
		    if(flags&2) { //fill0
			reader_getbits(&reader, fillbits); 
		    }
		    if(flags&4) { //fill1
			reader_getbits(&reader, fillbits); 
		    }
		    if(flags&8) { //linestyle
			reader_getbits(&reader, linebits); 
		    }
		    if(flags&16) {
			mapstyles(&reader, num, callback);
			fillbits = reader_getbits(&reader, 4);
			linebits = reader_getbits(&reader, 4);
		    }
		} else {
		    flags = reader_getbits(&reader, 1);
		    //printf("edge:%d\n", flags);
		    if(flags) { //straight edge
			int n = reader_getbits(&reader, 4) + 2;
			if(reader_getbits(&reader, 1)) { //line flag
			    reader_getbits(&reader, n); //delta x
			    reader_getbits(&reader, n); //delta y
			} else {
			    int v=reader_getbits(&reader, 1);
			    reader_getbits(&reader, n); //vert/horz
			}
		    } else { //curved edge
			int n = reader_getbits(&reader, 4) + 2;
			reader_getbits(&reader, n);
			reader_getbits(&reader, n);
			reader_getbits(&reader, n);
			reader_getbits(&reader, n);
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

