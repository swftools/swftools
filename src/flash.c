/* flash.c
   Various routines for reading and writing swf files and tags.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#include "flash.h"
#include "bitio.h"

void swf_init(uchar*newdata, int newlength)
{
    reader_init (&newdata[3], newlength - 3);
}

struct flash_header swf_read_header()
{
    struct flash_header head;
    u16 rate;
    u16 count;
    char version;
    int length;
    u8* oldpos = getinputpos();

    input1(&version);
    head.version = version;
    input4(&length);
    head.length = length;
    
    resetbits();
    head.boundingBox = readRECT();
    input2(&rate);
    head.rate = rate;
    input2(&count);
    head.count = count;

    head.headerlength = getinputpos() - oldpos;
    head.headerdata = oldpos;

    return head;
}

void swf_write_header(struct writer_t*w, struct flash_header*head)
{
    writer_writeu8(w, head->version);
    writer_writeu32(w, head->length);
    
    writer_writebits(w, 31, 5); 
    writer_writebits(w, head->boundingBox.x1, 31);
    writer_writebits(w, head->boundingBox.x2, 31);
    writer_writebits(w, head->boundingBox.y1, 31);
    writer_writebits(w, head->boundingBox.y2, 31);
    writer_resetbits(w);
    writer_writeu16(w, head->rate);
    writer_writeu16(w, head->count);
}

struct RGB readRGB()
{
    struct RGB rgb;
    input1(&rgb.r);
    input1(&rgb.g);
    input1(&rgb.b);
    return rgb;
}

struct RGBA readRGBA()
{
    struct RGBA rgba;
    input1(&rgba.r);
    input1(&rgba.g);
    input1(&rgba.b);
    input1(&rgba.a);
    return rgba;
}

struct GRADIENT readGRADIENT(int shape)
{
    struct GRADIENT gradient;
    int t;
    gradient.num = readu8();
    for(t=0;t<gradient.num;t++)
    {
	gradient.ratios[t] = readu8();
	if(shape<3)
	    gradient.rgb[t] = readRGB();
	else
	    gradient.rgba[t] = readRGBA();
    }
}

struct RECT readRECT()
{
    u32 a;
    struct RECT r;
    s32 b;
    readbits(&a,5);
    readsbits(&b,a);
    r.x1=b;
    readsbits(&b,a);
    r.x2=b;
    readsbits(&b,a);
    r.y1=b;
    readsbits(&b,a);
    r.y2=b;
    return r;
}


void writeRECT(u8**pos, struct RECT*r)
{
    struct writer_t w;
    int t=0;
    writer_init(&w, *pos, 256);
    writer_writebits(&w, 31, 5); //FIXME:count
    writer_writebits(&w, r->x1, 31);
    writer_writebits(&w, r->x2, 31);
    writer_writebits(&w, r->y1, 31);
    writer_writebits(&w, r->y2, 31);
    writer_resetbits(&w);
    *pos = writer_getpos(&w);
}

struct CXFORM readCXFORM(char alpha)
{
    struct CXFORM c;
    int bits;
    c.hasadd=readbit();
    c.hasmult=readbit();
    bits=getbits(4);
    c.alpha = alpha;

    if (c.hasmult)
    {
	c.rmult=getsbits(bits)/65536.0;
	c.gmult=getsbits(bits)/65536.0;
	c.bmult=getsbits(bits)/65536.0;
	if(c.alpha)
	    c.amult=getsbits(bits)/65536.0;
    }
    if (c.hasadd)
    {
	c.radd=getsbits(bits)/65536.0;
	c.gadd=getsbits(bits)/65536.0;
	c.badd=getsbits(bits)/65536.0;
	if(c.alpha)
	    c.aadd=getsbits(bits)/65536.0;
    }
    return c;
}

void CXFORM_write(struct CXFORM *obj, struct writer_t*w)
{
    int bits = 15;
    writer_writebit(w,obj->hasadd);
    writer_writebit(w,obj->hasmult);
    writer_writebits(w, bits, 4);
    if (obj->hasmult)
    {
	writer_writebits(w, obj->rmult, bits);
	writer_writebits(w, obj->gmult, bits);
	writer_writebits(w, obj->bmult, bits);
	if(obj->alpha)
	    writer_writebits(w, obj->amult, 4);
    }
    if (obj->hasadd)
    {
	writer_writebits(w, obj->radd, bits);
	writer_writebits(w, obj->gadd, bits);
	writer_writebits(w, obj->badd, bits);
	if(obj->alpha)
	    writer_writebits(w, obj->aadd, 4);
    }
}

unsigned char* readSTRING()
{
    unsigned char*now = getinputpos();
    char a;
    do
    {
	input1(&a);
    }
    while(a);
    return now;
}
void MATRIX_init(struct MATRIX*m)
{
    m->hasrotate = 0;
    m->hasscale = 0;
    m->b[0] = 0; 
    m->b[1] = 0; 
    m->a[0][0] = 1;
    m->a[1][1] = 1;
    m->a[1][0] = 0;
    m->a[0][1] = 0;
}
struct MATRIX readMATRIX()
{
    struct MATRIX m;
    u8 hasrotate;
    u8 translatebits;
    u32 translatex;
    u32 translatey;

    m.a[0][0] = m.a[1][1] = 1;
    m.a[0][1] = m.a[1][0] = 0;
    m.hasscale=readbit();
    if(m.hasscale)
    {
	u8 scalebits=getbits(5);
	s32 scalex=getsbits(scalebits);
	s32 scaley=getsbits(scalebits);
	m.a[0][0]=scalex/65536.0;
	m.a[1][1]=scaley/65536.0;
    }
    m.hasrotate=readbit();
    if(m.hasrotate)
    {
	u8 rotbits=getbits(5);
	s32 rotateskew0=getsbits(rotbits);
	s32 rotateskew1=getsbits(rotbits);
	m.a[0][1]=rotateskew0/65536.0;
	m.a[1][0]=rotateskew1/65536.0;
    }

    translatebits=getbits(5);
    translatex=getsbits(translatebits);
    translatey=getsbits(translatebits);
    m.b[0]=translatex;
    m.b[1]=translatey;

    return m;
}

void MATRIX_write(struct MATRIX * m , struct writer_t*w)
{
    writer_writebit (w, m->hasscale);
    if(m->hasscale)
    {
	int bits = 31;
	writer_writebits(w, bits, 5);
	writer_writebits(w, (u32)(m->a[0][0]*65536.0), bits);
	writer_writebits(w, (u32)(m->a[1][1]*65536.0), bits);
    }
    writer_writebit (w, m->hasrotate);
    if(m->hasrotate)
    {
	int bits = 31;
	writer_writebits(w, bits, 5);
	writer_writebits(w, (u32)(m->a[0][1]*65536.0), bits);
	writer_writebits(w, (u32)(m->a[1][0]*65536.0), bits);
    }

    //translate
    {
	int bits = 31;
	writer_writebits(w, bits, 5);
	writer_writebits(w, (u32)(m->b[0]), bits);
	writer_writebits(w, (u32)(m->b[1]), bits);
    }
}


int swf_read_tag(struct swf_tag* swftag)
{
    u8*pos2,*pos = getinputpos();
    u16 tag;
    u32 taglength;
    u32 tagid;
    int t;

    input2(&tag);

    taglength = tag & 0x3f;
    if (taglength == 0x3f)
    {
	input4(&taglength);
    }

    swftag->id=tag>>6;
    swftag->length = taglength;
    swftag->data = getinputpos();
    skip(taglength);
    pos2 = getinputpos();
    swftag->fulllength = pos2 - pos;
    swftag->fulldata = pos;
    return 1;
}

int swf_count_tags()
{
    u8*pos = getinputpos();
    int t=0;
    struct swf_tag tag;

    while(1)
    {
	swf_read_tag(&tag);
	t++;
	if (tag.id == 0)
	    break;
    }
    
    setinputpos(pos);
    return t;
}

void placeobject_init (struct PlaceObject*obj, struct swf_tag*tag)
{
    reader_init (tag->data, tag->length);
    obj -> id = readu16();
    obj -> depth = readu16();
    obj -> matrix = readMATRIX();
    obj -> hascxform = (getinputpos() < &tag->data[tag->length]);
    if(obj -> hascxform)
	obj -> cxform = readCXFORM(0);
}

void placeobject_write (struct PlaceObject*obj, struct writer_t*w)
{
    u16 taghead = 0x3f | TAGID_PLACEOBJECT<<6;
    u8*pos;
    u8*lenpos;
    writer_resetbits(w);
    writer_write(w, &taghead, 2);
    lenpos = writer_getpos(w);

    writer_write(w, &taghead, 2);
    writer_write(w, &taghead, 2);

    pos = writer_getpos(w);

    writer_write(w, &obj->id, 2);
    writer_write(w, &obj->depth, 2);
    MATRIX_write(&obj->matrix, w);

    if(obj->hascxform)
    {
	CXFORM_write(&obj->cxform, w);
    }
    writer_resetbits(w);
    
    *(u32*)lenpos = (u8*)writer_getpos(w) - pos;
}

void placeobject2_init (struct PlaceObject2*obj,struct swf_tag*tag)
{    
    u8 b;
    reader_init (tag->data, tag->length);
    b=readu8();
    obj->reserved=         (b>>7)&1;
    obj->hasclipactions=   (b>>6)&1;
    obj->hasname=          (b>>5)&1;
    obj->hasratio=         (b>>4)&1;
    obj->hascolortransform=(b>>3)&1;
    obj->hasmatrix=        (b>>2)&1;
    obj->hascharacter=     (b>>1)&1;
    obj->hasmove=          (b>>0)&1;

    obj->depth = readu16();
    obj->id = -1;
    if(obj->hascharacter) {
	obj->id = readu16();
    }
    if(obj->hasmatrix) { 
	obj->matrix = readMATRIX();
    } 
    if(obj->hascolortransform) {
	obj->cxform = readCXFORM(0);
    }
    if(obj->hasratio) {
	obj->ratio=readu16();
    }
    obj->name = 0;
    if(obj->hasname) {
	obj->name=readSTRING();
    }
    if(obj->hasclipactions) {
	obj->clipactions=readu16();
    }
}

void placeobject2_write (struct PlaceObject2*obj, struct writer_t*w)
{
    u8 flags = obj->reserved<<7 | obj->hasclipactions<<6 | obj->hasname<<5 | obj->hasratio<<4 |
	       obj->hascolortransform<<3 | obj->hasmatrix<<2 | obj->hascharacter<<1 | obj->hasmove;
    u16 taghead = 0x3f | TAGID_PLACEOBJECT2<<6;
    u8*pos;
    u8*lenpos;
    writer_resetbits(w);
    writer_write(w, &taghead, 2);
    lenpos = writer_getpos(w);
    writer_write(w, &taghead, 2);
    writer_write(w, &taghead, 2);

    pos = writer_getpos(w);

    writer_writeu8(w, flags);
    writer_writeu16(w, obj->depth);
    if(obj->hascharacter)
	writer_writeu16(w, obj->id);
    if(obj->hasmatrix)
	MATRIX_write(&obj->matrix, w);
    if(obj->hascolortransform)
	CXFORM_write(&obj->cxform, w);
    if(obj->hasratio)
	writer_writeu16(w, obj->ratio);
    writer_resetbits(w);
    if(obj->hasname)
	writer_write(w, obj->name, strlen(obj->name) + 1);
    if(obj->hasclipactions)
	writer_writeu16(w, obj->clipactions);
    writer_resetbits(w);
    *(u32*)lenpos = (u8*)writer_getpos(w) - pos;
}

void read_swf(struct swffile*swf, uchar*data, int length)
{
    int pos;
    struct flash_header head;
    int tagnum;
    swf_init(data, length);
    head = swf_read_header(data);
    logf("<debug> [HEADER] the version is %d", head.version);
    logf("<debug> [HEADER] the length is %d", head.length);
    logf("<debug> [HEADER] the boundingBox is %d:%d:%d:%d", 
	    head.boundingBox.x1,head.boundingBox.y1,
	    head.boundingBox.x2,head.boundingBox.y2);
    logf("<debug> [HEADER] the rate (frames/second) is %d", head.rate);
    logf("<debug> [HEADER] the count (frame number) is %d", head.count);

    tagnum = swf_count_tags();
    swf->tags = (struct swf_tag*)malloc(sizeof(struct swf_tag)*tagnum);
    
    logf("<debug> [HEADER] the file consists of %d tags", tagnum);

    pos = 0;
    while(1)
    {
	struct swf_tag tag;
	swf_read_tag(&tag);
	logf("<debug> read tag %02x (%d bytes)", tag.id, tag.length);
	swf->tags[pos] = tag;
	pos++;
	if(tag.id == TAGID_END)
	    break;
    }
    swf->tagnum = tagnum;
    swf->header = head;
}

int definingtagids[] =
{TAGID_DEFINESHAPE,
 TAGID_DEFINESHAPE2,
 TAGID_DEFINESHAPE3,
 TAGID_DEFINEMORPHSHAPE,
 TAGID_DEFINEFONT,
 TAGID_DEFINEFONT2,
 TAGID_DEFINETEXT,
 TAGID_DEFINETEXT2,
 TAGID_DEFINEEDITTEXT,
 TAGID_DEFINEBITS,
 TAGID_DEFINEBITSJPEG2,
 TAGID_DEFINEBITSJPEG3,
 TAGID_DEFINEBITSLOSSLESS,
 TAGID_DEFINEBITSLOSSLESS2,
 TAGID_DEFINEMOVIE,
 TAGID_DEFINESPRITE,
 TAGID_DEFINEBUTTON,
 TAGID_DEFINEBUTTON2,
 TAGID_DEFINESOUND,
 -1
};

// tags which may be used inside a sprite definition
int spritetagids[] =
{TAGID_SHOWFRAME,
 TAGID_PLACEOBJECT,
 TAGID_PLACEOBJECT2,
 TAGID_REMOVEOBJECT,
 TAGID_REMOVEOBJECT2, //?
 TAGID_DOACTION,
 TAGID_STARTSOUND,
 TAGID_FRAMELABEL,
 TAGID_SOUNDSTREAMHEAD,
 TAGID_SOUNDSTREAMHEAD2,
 TAGID_SOUNDSTREAMBLOCK,
 TAGID_END,
 -1
};

int getidfromtag(struct swf_tag* tag)
{
    // todo: check for more than one id
    return *(u16*)tag->data;
}

void setidintag(struct swf_tag* tag, int id)
{
    *(u16*)tag->data = id;
}

char is_sprite_tag (int id)
{

    int t=0;
    while(spritetagids[t]>=0)
    {
	if(spritetagids[t] == id) 
	    return 1;
	t++;
    }
    return 0; 
}

char is_defining_tag (int id)
{

    int t=0;
    while(definingtagids[t]>=0)
    {
	if(definingtagids[t] == id) 
	    return 1;
	t++;
    }
    return 0; 
}

struct swf_tag* duptag(struct swf_tag*tag)
{
    struct swf_tag* newtag = (struct swf_tag*)malloc(sizeof(struct swf_tag));
    newtag->id = tag->id;
    newtag->fulldata = (u8*)malloc(tag->fulllength);
    newtag->fulllength = tag->fulllength;
    newtag->data = newtag->fulldata + (tag->data - tag->fulldata);
    newtag->length = tag->length;
    memcpy(newtag->fulldata, tag->fulldata, tag->fulllength);
    return newtag;
}

