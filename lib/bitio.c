#define READER_TYPE_FILE 0
#define READER_TYPE_MEM  1
#define READER_TYPE_ZLIB 2
#define WRITER_TYPE_FILE 0
#define WRITER_TYPE_MEM  1
#define WRITER_TYPE_ZLIB 2

struct reader_t
{
    int (*read)(struct reader_t*, void*data, int len);
    void *internal;
    int type;
    unsigned char mybyte;
    unsigned char bitpos;
};
struct writer_t
{
    int (*write)(struct writer_t*, void*data, int len);
    void *internal;
    int type;
    unsigned char mybyte;
    unsigned char bitpos;
};
static int reader_fileread(struct reader_t*reader, void* data, int len) 
{
    return read((int)reader->internal, data, len);
}
static void reader_resetbits(struct reader_t*r)
{
    r->mybyte = 0;
    r->bitpos = 8;
}
static void reader_init_filereader(struct reader_t*r, int handle)
{
    r->read = reader_fileread;
    r->internal = (void*)handle;
    r->type = READER_TYPE_FILE;
    reader_resetbits(r);
}
#define ZLIB_BUFFER_SIZE 16384
struct zlibinflate_t
{
    z_stream zs;
    struct reader_t*input;
    U8 readbuffer[ZLIB_BUFFER_SIZE];
};
static void zlib_error(int ret, char* msg, z_stream*zs)
{
    fprintf(stderr, "%s: zlib error (%d): last zlib error: %s\n",
	  msg,
	  ret,
	  zs->msg?zs->msg:"unknown");
    perror("errno:");
    exit(1);
}
static int reader_zlibinflate(struct reader_t*reader, void* data, int len);
static void reader_init_zlibinflate(struct reader_t*r, struct reader_t*input)
{
    struct zlibinflate_t*z;
    int ret;
    z = (struct zlibinflate_t*)malloc(sizeof(struct zlibinflate_t));
    r->internal = z;
    r->read = reader_zlibinflate;
    r->type = READER_TYPE_ZLIB;
    z->input = input;
    memset(&z->zs,0,sizeof(z_stream));
    z->zs.zalloc = Z_NULL;
    z->zs.zfree  = Z_NULL;
    z->zs.opaque = Z_NULL;
    ret = inflateInit(&z->zs);
    if (ret != Z_OK) zlib_error(ret, "bitio:inflate_init", &z->zs);
    reader_resetbits(r);
}
static int reader_zlibinflate(struct reader_t*reader, void* data, int len) 
{
    struct zlibinflate_t*z = (struct zlibinflate_t*)reader->internal;
    int ret;
    if(!z)
	return 0;
    
    z->zs.next_out = data;
    z->zs.avail_out = len;

    while(1) {
	if(!z->zs.avail_in) {
	    z->zs.avail_in = z->input->read(z->input, z->readbuffer, ZLIB_BUFFER_SIZE);
	    z->zs.next_in = z->readbuffer;
	}
	if(z->zs.avail_in)
	    ret = inflate(&z->zs, Z_NO_FLUSH);
	else
	    ret = inflate(&z->zs, Z_FINISH);
    
	if (ret != Z_OK &&
	    ret != Z_STREAM_END) zlib_error(ret, "bitio:inflate_inflate", &z->zs);

	if (ret == Z_STREAM_END) {
		int pos = z->zs.next_out - (Bytef*)data;
		ret = inflateEnd(&z->zs);
		if (ret != Z_OK) zlib_error(ret, "bitio:inflate_end", &z->zs);
		free(reader->internal);
		reader->internal = 0;
		return pos;
	}
	if(!z->zs.avail_out) {
	    break;
	}
    }
    return len;
}
static unsigned int reader_readbit(struct reader_t*r)
{
    if(r->bitpos==8) 
    {
	r->bitpos=0;
        r->read(r, &r->mybyte, 1);
    }
    return (r->mybyte>>(7-r->bitpos++))&1;
}
static unsigned int reader_readbits(struct reader_t*r, int num)
{
    int t;
    int val = 0;
    for(t=0;t<num;t++)
    {
	val<<=1;
	val|=reader_readbit(r);
    }
    return val;
}
static int writer_filewrite(struct writer_t*w, void* data, int len) 
{
    return write((int)w->internal, data, len);
}
static void writer_resetbits(struct writer_t*w)
{
    if(w->bitpos)
	w->write(w, &w->mybyte, 1);
    w->bitpos = 0;
    w->mybyte = 0;
}
static void writer_init_filewriter(struct writer_t*w, int handle)
{
    w->write = writer_filewrite;
    w->internal = (void*)handle;
    w->type = WRITER_TYPE_FILE;
    w->bitpos = 0;
    w->mybyte = 0;
}
static void writer_writebit(struct writer_t*w, int bit)
{    
    if(w->bitpos==8) 
    {
        w->write(w, &w->mybyte, 1);
	w->bitpos = 0;
	w->mybyte = 0;
    }
    if(bit&1)
	w->mybyte |= 1 << (7 - w->bitpos);
    w->bitpos ++;
}
static void writer_writebits(struct writer_t*w, U32 data, int bits)
{
    int t;
    for(t=0;t<bits;t++)
    {
	writer_writebit(w, (data >> (bits-t-1))&1);
    }
}

