/* avi2swf.cc
   Convert avi movie files into swf.

   Part of the swftools package.
   
   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org>

   This file is distributed under the GPL, see file COPYING for details */

#include <stdio.h>
#include <fcntl.h>
extern "C" {
#include "../lib/rfxswf.h"
#include "../lib/args.h"
}
#include <avifile/version.h>
#if (AVIFILE_MAJOR_VERSION == 0) && (AVIFILE_MINOR_VERSION>=6) 
   #include <avifile.h>
   #include <aviplay.h>
   #include <fourcc.h>
   #include <creators.h>
   #include <StreamInfo.h>
   #define VERSION6
#else
   #include <avifile.h>
   #include <aviplay.h>
   #include <aviutil.h>
   #define Width width
   #define Height height
   #define Data data
   #define Bpp bpp
#endif

/*
statistics: (for now)
   37   bytes per shape (rectangle)
   8-12 bytes per placeobject
   4    bytes per removeobject2
   696+ bytes per definejpeg2 (576 jpegtables)
   576  bytes per jpegtables
   122+ bytes per definejpeg

   blocks*140 = minimal bytes per frames
   5000/140   = maximal blocks with framerate 5000

   2    bytes per showframe
*/

static int cache_size=38; //in frames

static char * filename = 0;
static char * outputfilename = "output.swf";
static unsigned int firstframe = 0;
static unsigned int lastframe = 0x7fffffff;

static int jpeg_quality = 20;

static char zlib = 0;
static double scale = 1.0;

#ifndef ST_DEFINEBITSJPEG
#define ST_DEFINEBITSJPEG       6 
#endif
  
int filesize = 0;

struct options_t options[] =
{
 {"v","verbose"},
 {"o","output"},
 {"n","num"},
 {"s","start"},
 {"z","zlib"},
 {"V","version"},
 {0,0}
};

int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("avi2swf - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    } 
    else if(!strcmp(name, "o")) {
	outputfilename = val;
	return 1;
    }
    else if(!strcmp(name, "n")) {
	lastframe = atoi(val);
	return 1;
    }
    else if(!strcmp(name, "s")) {
	firstframe = atoi(val);
	return 1;
    }
    else if(!strcmp(name, "d")) {
	scale = atoi(val)/100.0;
	if(scale>1.0 || scale<=0) {
	    fprintf(stderr, "Scale must be in the range 1-100!\n");
	    exit(1);
	}
	return 1;
    }
    else if(!strcmp(name, "z")) {
	zlib = 1;
	return 0;
    }
    fprintf(stderr, "Unknown option: -%s\n", name);
    exit(1);
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char*name)
{    
    printf("\nUsage: %s file.avi\n", name);
    printf("\t-h , --help\t\t Print help and exit\n");
    printf("\t-o , --output filename\t Specify output filename\n"); 
    printf("\t-n , --num frames\t Number of frames to encode\n");
    printf("\t-s , --start frame\t First frame to encode\n");
    printf("\t-d , --scale factor\t Scale to factor percent\n");
    printf("\t-V , --version\t\t Print program version and exit\n");
    exit(0);
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

/* id allocation/deallocation */
char idtab[65536];
unsigned int idtab_pos=1;
int get_free_id()
{
    while(idtab[idtab_pos] || !idtab_pos)
	idtab_pos++;
    idtab[idtab_pos]=1;
    return idtab_pos;
}
void free_id(int id)
{
    idtab[id] = 0;
}

void makeshape(int file, int id, int gfxid, int width, int height)
{
    TAG*tag;
    RGBA rgb;
    MATRIX m;
    SHAPE*s;
    SRECT r;
    int lines = 0;
    int ls,fs;
    tag = swf_InsertTag(NULL, ST_DEFINESHAPE);
    swf_ShapeNew(&s);
    rgb.b = rgb.g = rgb.r = 0xff;
    if(lines)
	ls = swf_ShapeAddLineStyle(s,20,&rgb);  
    swf_GetMatrix(NULL,&m);
    m.sx = 20*65536;
    m.sy = 20*65536;

    fs = swf_ShapeAddBitmapFillStyle(s,&m,gfxid,0);
    swf_SetU16(tag,id);   // ID   
    r.xmin = 0;
    r.ymin = 0;
    r.xmax = width*20;
    r.ymax = height*20;
    swf_SetRect(tag,&r);

    swf_SetShapeStyles(tag,s);
    swf_ShapeCountBits(s,NULL,NULL);
    swf_SetShapeBits(tag,s);

    swf_ShapeSetAll(tag,s,0,0,lines?ls:0,fs,0);

    swf_ShapeSetLine(tag,s,width*20,0);
    swf_ShapeSetLine(tag,s,0,height*20);
    swf_ShapeSetLine(tag,s,-width*20,0);
    swf_ShapeSetLine(tag,s,0,-height*20);
    swf_ShapeSetEnd(tag);
    filesize += swf_WriteTag(file, tag);
    swf_DeleteTag(tag);
    swf_ShapeFree(s);
}

void setshape(int file,int id,int depth,int x,int y,CXFORM*cx)
{
    TAG*tag;
    MATRIX m;
    m.sx = 0x10000; m.sy = 0x10000;
    m.r0 = 0; m.r1 = 0;
    m.tx = x*20; 
    m.ty = y*20;
    if(cx && !((cx->a0!=256)||(cx->r0!=256)||(cx->g0!=256)||(cx->b0!=256)
		||(cx->a1|cx->r1|cx->g1|cx->b1))) cx = 0;
    tag = swf_InsertTag(NULL,ST_PLACEOBJECT2);
      swf_ObjectPlace(tag,id,depth,&m,cx,0);
    filesize += swf_WriteTag(file, tag);
    swf_DeleteTag(tag);
}


int xblocksize;
int yblocksize;
struct GfxBlock {
//    static int xblocksize;
//    static int yblocksize;
    U8*data;
    int len;
};

int width=0;
int height=0;

int xblocks;
int yblocks;

U8* blockbuffer = 0;
   
class GfxBlockCache {

    GfxBlock*list;
    char*expire; //0=block's free
    int*ids;
    int size;
    int pos;
    int hits;
    int misses;

    public:

    GfxBlockCache(int file) 
    {
	list=0;
	size = xblocks*yblocks*cache_size;
	printf("initializing cache (%d entries)\n", size);
	list = new GfxBlock[size];
	expire = new char[size];
	ids = new int[size];
	memset(expire,0,size);
	memset(list,0,sizeof(GfxBlock)*size);
	memset(ids,0,sizeof(int)*size);
	pos = 0;
	hits =0;
	misses =0;
    }
    void insert(GfxBlock*block, int gfxid)
    {
	int oldpos = pos;
	while(++pos!=oldpos)
	{
	    if(pos==size) pos=0;
	    if(!expire[pos])
		break;
	}
	if(pos==oldpos) {
	    // cache full- don't insert item
	    return;
	}
	if(list[pos].data) {
	    free(list[pos].data);
	    list[pos].data = 0;
	    //TODO: free this in the SWF, also
	}
	list[pos].data=(U8*)malloc(block->len);
	memcpy(list[pos].data,block->data,block->len);
	list[pos].len = block->len;
	expire[pos] = cache_size;
	ids[pos] = gfxid;
    }
    int find(GfxBlock*block, CXFORM*cxform)
    {
	//TODO: do least square regression here to derive cxform
	int s;
	int bestsum=-1;
	int bestid;
	float best;
	for(s=0;s<size;s++)
	if(expire[s])
	{
	    int t = (block->len);
	    U8*ptr1 = block->data;
	    U8*ptr2 = list[s].data;
	    int sum2 = 0;
	    // notice: we treat r,g,b as equal here.
	    do {
		int a = (*ptr1++)-(*ptr2++);
		sum2 += a*a;
	    } while(--t);
	    if(bestsum < 0 || bestsum > sum2) {
		bestid = s;
		bestsum = sum2;
	    }
	}
	if(bestsum<0) {
	    misses++;
	    return -1;
	}
	best = bestsum/block->len;

	if(best > 96.0) {
	    misses++;
	    return -1;
	} 
	expire[bestid]= cache_size;
	hits++;
	cxform->a0 = 256;
	cxform->r0 = 256;
	cxform->g0 = 256;
	cxform->b0 = 256;
	cxform->a1 = 0;
	cxform->r1 = 0;
	cxform->g1 = 0;
	cxform->b1 = 0;
	return ids[bestid];
    }
    void newframe()
    {
	int t;
	for(t=0;t<size;t++)
	    if(expire[t])
		expire[t]--;

    }
    ~GfxBlockCache()
    {
	int t;
	printf("destroying cache...\n");
	if(hits+misses) {
	    printf("hits:%d (%02d%%)\n", hits, hits*100/(hits+misses));
	    printf("misses:%d (%02d%%)\n", misses, misses*100/(hits+misses));
	}
	for(t=0;t<size;t++)
	    if(expire[t] && list[t].data)
		free(list[t].data);
	free(list);
	free(expire);
	free(ids);
    }
} * cache = 0;

class GfxBlockEncoder {
    int sizex;
    int sizey;
    int posx;
    int posy;
    int basedepth;
    int depth[3];
    public:
    void init(int depth, int posx,int posy, int sizex, int sizey) 
    {
	this->basedepth = depth;
	this->posx = posx;
	this->posy = posy;
	this->sizex = sizex;
	this->sizey = sizey;
	this->depth[0] = this->depth[1] = this->depth[2] = -1;
    }
    void clear(int file)
    {
	/* clear everything in the block */
	int t;
	for(t=0;t<3;t++)
	if(depth[t]>=0)
	{
	    TAG*tag;
	    tag = swf_InsertTag(NULL, ST_REMOVEOBJECT2);
	    swf_SetU16(tag, basedepth+t); //depth
	    filesize += swf_WriteTag(file, tag);
	    swf_DeleteTag(tag);
	    depth[t] = -1;
	}
    }
    void writeiframe(int file, GfxBlock*block)
    {
	clear(file);

	int gfxid = get_free_id();
	int shapeid = get_free_id();

	//memset(data,0,sizex*sizey*3);
	TAG*tag = swf_InsertTag(NULL, ST_DEFINEBITS);
	JPEGBITS * jb = swf_SetJPEGBitsStart(tag,sizex,sizey,jpeg_quality);
	tag->len = 0; //bad hack
	swf_SetU16(tag, gfxid);
	int y;
	for(y=0;y<sizey;y++)
	    swf_SetJPEGBitsLine(jb,&block->data[y*sizex*3]);
	swf_SetJPEGBitsFinish(jb);
	filesize += swf_WriteTag(file, tag);
	swf_DeleteTag(tag);

	cache->insert(block, shapeid);

	makeshape(file, shapeid, gfxid, sizex, sizey);
	setshape(file, shapeid, basedepth+1, posx, posy, 0);
	depth[1] = shapeid;
    }
    void writereference(int file, int shapeid, CXFORM*form)
    {
	if(depth[1]!=shapeid)
	{
	    clear(file);
	    setshape(file, shapeid, basedepth+1, posx, posy, form);
	    depth[1] = shapeid;
	}
    }
    void compress(int file, GfxBlock*block)
    {	
	CXFORM form;
	int id = cache->find(block, &form);
	if(id<0)
	    writeiframe(file, block);
	else {
	    writereference(file, id, &form);
	}
    }
} *blocks = 0;

void initdisplay(int file)
{
    if(blockbuffer)
	free(blockbuffer);
    if(blocks) {
	int t;
	for(t=0;t<xblocks;t++)
	    blocks[t].clear(file);
	free(blocks);
    }
    if(cache)
	delete cache;
    xblocksize = (width/3)&~7;
    yblocksize = (height/2)&~7;
    xblocks = width/xblocksize;
    yblocks = height/yblocksize;
    printf("%dx%d blocks of size %dx%d\n", xblocks,yblocks, xblocksize, yblocksize);
    printf("cutting lower %d lines, right %d columns\n", 
	    height-yblocks*yblocksize, width-xblocks*xblocksize);
    blocks = new GfxBlockEncoder[xblocks*yblocks];
    blockbuffer = new U8[xblocksize*yblocksize*4]; //should be 3
    cache = new GfxBlockCache(file);
    int t;
    for(t=0;t<xblocks*yblocks;t++) {
	blocks[t].init(t*64,
		       (t%xblocks)*xblocksize,
		       (t/xblocks)*yblocksize,
		       xblocksize, yblocksize);
    }

    TAG*tag = swf_InsertTag(NULL, ST_JPEGTABLES);
    JPEGBITS * jpeg = swf_SetJPEGBitsStart(tag, xblocksize, yblocksize, jpeg_quality);
    filesize += swf_WriteTag(file, tag);
    swf_DeleteTag(tag);
    free(jpeg);
}

void destroydisplay(int file)
{
    delete cache;
    free(blocks);
    free(blockbuffer);
}

SWF swf;
TAG*tag;

class SoundReader
{

    short int* sound_buffer;
    int mp3_block_size;
    int write_pos;
    int read_pos;
    IAviReadStream* astream;
    void readBlock()
    {
	unsigned samples_read, bytes_read;
	int ret;
	short int tmpbuf[4096];
	ret = astream->ReadFrames(tmpbuf, 4096*sizeof(short int),
		4096, samples_read, bytes_read);
	if(ret<0) {
	    printf("couldn't read %d samples\n", mp3_block_size);
	    exit(1);
	}
	int t;
	samples_read = bytes_read/sizeof(short int);
	for(t=0;t<samples_read/2;t++) {
	    sound_buffer[write_pos+t] = tmpbuf[t*2];
	}
	write_pos += samples_read/2;

	if(write_pos >= mp3_block_size*8)
	{
	    if(write_pos > mp3_block_size*8)
		memcpy(&sound_buffer[0],&sound_buffer[mp3_block_size*8],write_pos - mp3_block_size*8);
	    write_pos %= (mp3_block_size*8);
	}
    }
    public:

    SoundReader(IAviReadStream*astream)
    {
	this->astream = astream;
	this->write_pos = 0;
	this->read_pos = 0;
	this->mp3_block_size = 2304;
	this->sound_buffer = new short int[mp3_block_size*16];
    }
    ~SoundReader()
    {
	delete sound_buffer;
    }
    int available()
    {
	if(read_pos<=write_pos)
	    return write_pos-read_pos;
	else
	    return (write_pos+mp3_block_size*8)-read_pos;
    }
    short int* readFrame()
    {
	int tmp;
	while(available()<mp3_block_size) {
	    readBlock();
	}
	tmp = read_pos;
	read_pos += mp3_block_size;
	read_pos %= mp3_block_size*8;
	return &sound_buffer[tmp];
    }
};


static int mp3_block_size = 2304;

static int do_video = 1;
static int do_audio = 1;

int main (int argc,char ** argv)
{ 
  int file;
  IAviReadFile* player;
  IAviReadStream* astream;
  IAviReadStream* vstream;
  SRECT r;
  double samplesperframe;
  int samplerate;
  int samplefix;
  double fps;
  int oldwidth;
  int oldheight;
  double reziscale;

  processargs(argc, argv);
  lastframe += firstframe;
  if(!filename)
      exit(0);

  memset(idtab, 0, sizeof(idtab));

  player = CreateIAviReadFile(filename);    
  astream = player->GetStream(0, AviStream::Audio);
  vstream = player->GetStream(0, AviStream::Video);
  if(!vstream)
      do_video = 0;
  if(!astream)
      do_audio = 0;
#ifndef VERSION6
  MainAVIHeader head;
  int dwMicroSecPerFrame = 0;
  player->GetFileHeader(&head);
  printf("fps: %d\n", 1000000/head.dwMicroSecPerFrame);
  printf("frames: %d\n", head.dwTotalFrames);
  printf("streams: %d\n", head.dwStreams);
  printf("width: %d\n", head.dwWidth);
  printf("height: %d\n", head.dwHeight);
  printf("sound: %u samples (%f seconds)\n", astream->GetEndPos(),
	  astream->GetEndTime());
  oldwidth = head.dwWidth;
  oldheight = head.dwHeight;
  dwMicroSecPerFrame = head.dwMicroSecPerFrame;
  samplesperframe = astream->GetEndPos()/astream->GetEndTime()*head.dwMicroSecPerFrame/1000000;
  samplerate = (int)(astream->GetEndPos()/astream->GetEndTime());
  fps = 1000000.0/dwMicroSecPerFrame;
#else
  StreamInfo*audioinfo;
  StreamInfo*videoinfo;
  if(do_video)
  {
    videoinfo = vstream->GetStreamInfo();
    oldwidth = videoinfo->GetVideoWidth();
    oldheight = videoinfo->GetVideoHeight();
    fps = (double)(videoinfo->GetFps());
    delete(videoinfo);
  }
  if(do_audio)
  {
    audioinfo = astream->GetStreamInfo();
    samplerate = audioinfo->GetAudioSamplesPerSec();
    samplesperframe = audioinfo->GetAudioSamplesPerSec()/videoinfo->GetFps();
    delete(audioinfo);
  }
#endif
  width = (int)(oldwidth*scale);
  height = (int)(oldheight*scale);
  reziscale = 1/scale;

  vstream -> StartStreaming();
  if(do_audio)
  {
    astream -> StartStreaming();
    printf("%f framerate\n", fps);
    printf("%f samples/frame\n", samplesperframe);
    printf("%d samplerate\n", samplerate);
  }

  if(zlib)
    file = open("__tmp__.swf", O_WRONLY|O_CREAT|O_TRUNC, 0644);
  else
    file = open(outputfilename, O_WRONLY|O_CREAT|O_TRUNC, 0644);
  
  memset(&swf, 0, sizeof(swf));
  swf.frameRate = (int)(fps*256);
  swf.fileVersion = 4;
  swf.fileSize = 0x0fffffff;
  swf.frameCount = lastframe - firstframe;
  r.xmin = 0;
  r.ymin = 0;
  r.xmax = width*20;
  r.ymax = height*20;
  swf.movieSize = r;

  filesize += swf_WriteHeader(file, &swf);

  tag = swf_InsertTag(NULL, ST_SETBACKGROUNDCOLOR);
  swf_SetU8(tag,0); //black
  swf_SetU8(tag,0);
  swf_SetU8(tag,0);
  filesize += swf_WriteTag(file, tag);
  swf_DeleteTag(tag);

  tag = swf_InsertTag(NULL, ST_SOUNDSTREAMHEAD2);
  swf_SetSoundStreamHead(tag, (int)samplesperframe/4);
  filesize += swf_WriteTag(file, tag);
  swf_DeleteTag(tag);

  int frame = 0;
  initdisplay(file);

  double movie_sound_pos = 0;
  int mp3_sound_pos = 0;

  if(do_audio)
  {
      WAVEFORMATEX wave;
      astream->GetAudioFormatInfo(&wave,0);

      printf("nChannels:%d\n", wave.nChannels);
      printf("nSamplesPerSec:%d\n", wave.nSamplesPerSec);
      printf("nAvgBytesPerSec:%d\n", wave.nAvgBytesPerSec);
      printf("nBlockAlign:%d\n", wave.nBlockAlign);
      printf("wBitsPerSample:%d\n", wave.wBitsPerSample);
      printf("cbSize:%d\n", wave.cbSize);
  }

  SoundReader* sound = new SoundReader(astream);

  while(1) {
    if(vstream->ReadFrame()<0) {
	printf("\n");
	break;
    }

    if(frame < firstframe)
    {
	movie_sound_pos += samplesperframe;
	if(do_audio)
	while(mp3_sound_pos<movie_sound_pos) {
	    short int* samples = sound->readFrame();
	    mp3_sound_pos += mp3_block_size;
	}
	printf("\rskipping frame %d",frame);
	fflush(stdout);
	frame++;
	if(frame == firstframe)
	    printf("\n");
	continue;
    }
    
    printf("\rconvert frame %d",frame);
    fflush(stdout);

    // audio
    movie_sound_pos += samplesperframe;

    int first=1;
    if(do_audio)
    while(mp3_sound_pos<movie_sound_pos) {
	// rawplay -s 44100 -f s16_le -c 2 samples.test 
	short int* samples = sound->readFrame();
	int s;
	int c=0;
	if(first) { //first run
	      tag = swf_InsertTag(NULL, ST_SOUNDSTREAMBLOCK);
	      swf_SetSoundStreamBlock(tag, samples, 1);
	} else {
	      swf_SetSoundStreamBlock(tag, samples, 0);
	}
	
	mp3_sound_pos += mp3_block_size;

	if(mp3_sound_pos>=movie_sound_pos) { // last run
	    filesize += swf_WriteTag(file, tag);
	    swf_DeleteTag(tag);
	}
	first = 0;
    }
 
    // video

    CImage*img = vstream->GetFrame();
    img->ToRGB();
    U8*data = img->Data();
    int bpp = img->Bpp();
    int x,y;
    int xx,yy;
    int fs,ls;
    SHAPE*s;
    MATRIX m;
    SRECT r;
    RGBA rgb;

    /* some movies have changing dimensions */
    if(img->Width() != oldwidth ||
       img->Height() != oldheight) {
	printf("\n");
	oldwidth = img->Width();
	oldheight = img->Height();
	width = (int)(oldwidth*scale);
	height = (int)(oldheight*scale);
	initdisplay(file);
    }

    for(yy=0;yy<yblocks;yy++)
    for(xx=0;xx<xblocks;xx++) 
    {
	int x,y;
	for(y=0;y<yblocksize;y++) {
	    /* avifile 0.6 returns images upside down. versoins 5 and >=7 don't */
#if (AVIFILE_MAJOR_VERSION == 0) && (AVIFILE_MINOR_VERSION==6) 
	    U8*mydata = img->At(oldheight-(int)((yy*yblocksize+y)*reziscale));
#else
	    U8*mydata = img->At((int)((yy*yblocksize+y)*reziscale));
#endif
	    for(x=0;x<xblocksize;x++) {
		blockbuffer[(y*xblocksize+x)*3+2] = mydata[((int)(((xx*xblocksize+x)*reziscale)))*3+0];
		blockbuffer[(y*xblocksize+x)*3+1] = mydata[((int)(((xx*xblocksize+x)*reziscale)))*3+1];
		blockbuffer[(y*xblocksize+x)*3+0] = mydata[((int)(((xx*xblocksize+x)*reziscale)))*3+2];
	    }
	}
	GfxBlock b;
	b.data = blockbuffer;
	b.len = xblocksize*yblocksize*3;
	blocks[yy*xblocks+xx].compress(file, &b);
    }

    tag = swf_InsertTag(NULL, ST_SHOWFRAME);
    filesize += swf_WriteTag(file, tag);
    swf_DeleteTag(tag);

    cache->newframe();

    frame++;
    if(frame == lastframe)
	break;
  }
  delete sound;
  printf("\n");
  destroydisplay(file);

  tag = swf_InsertTag(NULL, ST_END);
  filesize += swf_WriteTag(file, tag);
  swf_DeleteTag(tag);

  close(file);

  FILE*fi;
  if(zlib)
    fi=fopen("tmp.swf", "r+");
  else
    fi=fopen(outputfilename, "r+");

  if(fi)
  {
       fseek(fi,4,SEEK_SET);
       unsigned char f;
       f = filesize      ;fwrite(&f,1,1,fi);
       f = filesize >> 8 ;fwrite(&f,1,1,fi);
       f = filesize >> 16;fwrite(&f,1,1,fi);
       f = filesize >> 24;fwrite(&f,1,1,fi);
       fclose(fi);
  }

  if(zlib) {
      char buffer[1024];
      snprintf(buffer, 1024, "swfcombine -dz __tmp__.swf -o %s", outputfilename);
      printf("%s\n", buffer);
      system(buffer);
      sprintf(buffer, "rm __tmp__.swf");
      printf("%s\n", buffer);
      system(buffer);
  }
  
  return 0;
}

