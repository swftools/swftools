#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory.h>
#include <zlib.h>
#define ZLIB_BUFFER_SIZE 16384

typedef struct _writer
{
    int (*write)(struct _writer*, void*data, int len);
    void (*finish)(struct _writer*);
    void *internal;
} writer_t;

/* ---------------------------- include file filter ------------------------------- */

typedef struct _ifwrite
{
    FILE*fi;
    int bp;
    int pos;
    int last;
    char buf[1024];
    char*filename;
} ifwrite_t;

static int writer_ifwrite_write(writer_t*out, void*data, int len)
{
    ifwrite_t*i = (ifwrite_t*)out->internal;
    FILE*fo = i->fi;
    int t;
    for(t=0;t<len;t++) {
        unsigned char c = ((unsigned char*)data)[t];
        if(i->bp>=80) {
            fprintf(fo, "\"%s\"\n", i->buf);
            i->bp = 0;
        }
        if(c<=32 || c>0x7e || c=='"' || c == '\\' || c == '%'
        || (i->last && c>='0' && c<='9') 
        || (i->last && c>='a' && c<='f')
        || (i->last && c>='A' && c<='F')
        ) {
            i->bp += sprintf(i->buf+i->bp, "\\x%x",c);
            i->last = 1;
        } else {
            i->buf[i->bp] = c;
            i->buf[i->bp+1] = 0;
            i->last = 0;
            i->bp++;
        }
        i->pos++;
    }
}
static void writer_ifwrite_finish(writer_t*w)
{
    ifwrite_t *i= (ifwrite_t*)w->internal;
    fprintf(i->fi, "\"%s\"\n", i->buf);
    fprintf(i->fi, ";\n");
    fclose(i->fi);
    printf("wrote file %s\n", i->filename);
    free(w->internal);w->internal = 0;
}

writer_t*writer_init_includewriter(char*filename)
{
    writer_t*w = malloc(sizeof(writer_t));
    FILE*fi = fopen(filename, "wb");
    memset(w, 0, sizeof(writer_t));
    w->write = writer_ifwrite_write;
    w->finish = writer_ifwrite_finish;
    ifwrite_t*i = (ifwrite_t *)malloc(sizeof(ifwrite_t));
    i->fi = fi;
    i->pos = 0;
    i->bp=0;
    i->last = 0;
    i->filename = filename;
    w->internal = i;
    
    fprintf(i->fi, "char* crndata =\n");
    return w;

}

/* ---------------------------- zlibdeflate writer -------------------------- */

struct zlibdeflate_t
{
    z_stream zs;
    writer_t*output;
    int written;
    int zwritten;
    unsigned char writebuffer[ZLIB_BUFFER_SIZE];
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

static int writer_zlibdeflate_write(writer_t*writer, void* data, int len) 
{
    struct zlibdeflate_t*z = (struct zlibdeflate_t*)writer->internal;
    int ret;
    if(!z) {
	fprintf(stderr, "zlib not initialized!\n");
	return 0;
    }
    if(!len)
	return 0;
    
    z->zs.next_in = (Bytef *)data;
    z->zs.avail_in = len;

    while(1) {
	ret = deflate(&z->zs, Z_NO_FLUSH);
	
	if (ret != Z_OK) zlib_error(ret, "bitio:deflate_deflate", &z->zs);

	if(z->zs.next_out != z->writebuffer) {
            z->zwritten += z->zs.next_out - (Bytef*)z->writebuffer;
	    z->output->write(z->output, z->writebuffer, z->zs.next_out - (Bytef*)z->writebuffer);
	    z->zs.next_out = z->writebuffer;
	    z->zs.avail_out = ZLIB_BUFFER_SIZE;
	}

	if(!z->zs.avail_in) {
	    break;
	}
    }
    z->written += len;
    return len;
}
static void writer_zlibdeflate_finish(writer_t*writer)
{
    struct zlibdeflate_t*z = (struct zlibdeflate_t*)writer->internal;
    writer_t*output;
    int ret;
    if(!z)
	return;
    output= z->output;
    while(1) {
	ret = deflate(&z->zs, Z_FINISH);
	if (ret != Z_OK &&
	    ret != Z_STREAM_END) zlib_error(ret, "bitio:deflate_finish", &z->zs);

	if(z->zs.next_out != z->writebuffer) {
            z->zwritten += z->zs.next_out - (Bytef*)z->writebuffer;
	    z->output->write(z->output, z->writebuffer, z->zs.next_out - (Bytef*)z->writebuffer);
	    z->zs.next_out = z->writebuffer;
	    z->zs.avail_out = ZLIB_BUFFER_SIZE;
	}

	if (ret == Z_STREAM_END) {
	    break;

	}
    }
    ret = deflateEnd(&z->zs);
    if (ret != Z_OK) zlib_error(ret, "bitio:deflate_end", &z->zs);

    if(z->written) {
        printf("Compression ratio: %.2f%% (%d bytes of input data, %d bytes of output data)\n", 
                z->zwritten*100.0 / z->written,
                z->written, z->zwritten);
    } else {
        printf("Compression ratio: 0%% (0 bytes of input data, %d bytes of output data)\n", z->zwritten);
    }

    free(writer->internal);
    memset(writer, 0, sizeof(writer_t));
    output->finish(output); 
}
writer_t*writer_init_zwriter(writer_t*output)
{
    writer_t*w = malloc(sizeof(writer_t));
    struct zlibdeflate_t*z;
    int ret;
    memset(w, 0, sizeof(writer_t));
    z = (struct zlibdeflate_t*)malloc(sizeof(struct zlibdeflate_t));
    memset(z, 0, sizeof(struct zlibdeflate_t));
    w->internal = z;
    w->write = writer_zlibdeflate_write;
    w->finish = writer_zlibdeflate_finish;
    z->output = output;
    memset(&z->zs,0,sizeof(z_stream));
    z->zs.zalloc = Z_NULL;
    z->zs.zfree  = Z_NULL;
    z->zs.opaque = Z_NULL;
    ret = deflateInit(&z->zs, 9);
    if (ret != Z_OK) zlib_error(ret, "bitio:deflate_init", &z->zs);
    z->zs.next_out = z->writebuffer;
    z->zs.avail_out = ZLIB_BUFFER_SIZE;
    z->written = 0;
    z->zwritten = 0;
    return w;
}

/* ------------------------------- main ----------- ------------------------------- */

static int compare_filenames(const void*_n1, const void*_n2)
{
    const char*s1 = *(const char**)_n1;
    const char*s2 = *(const char**)_n2;

    char*e1 = strrchr(s1, '.');
    char*e2 = strrchr(s2, '.');
    if(!e1 && !e2)
        return 0;
    if(e1 && !e2)
        return 1;
    if(!e1 && e2)
        return -1;
    int c = strcasecmp(e1+1,e2+1);
    if(c)
        return c;
    return strcasecmp(s1,s2);
}

int main (int argn, char*argv[])
{
    int t;
    char buf[320];

    writer_t*include_writer = writer_init_includewriter("crnfiles.c");
    writer_t*zwriter = writer_init_zwriter(include_writer);

    qsort(argv+1, argn-1, sizeof(argv[0]), compare_filenames);

    for(t=1;t<argn;t++)
    {
	    if(argv[t][0]!='-')
	    {
		    FILE*fi=fopen(argv[t],"rb");
		    if(fi)
		    {
			    fseek(fi,0,SEEK_END);
			    long l=ftell(fi);
			    fseek(fi,0,SEEK_SET);

			    char*buf=(char*)malloc(l);
			    char*id="NUL";
			    struct stat st;
			    fstat(fileno(fi),&st);
			    if(S_ISDIR(st.st_mode)) {id ="DIR";l=0;}
			    else if(strstr(argv[t],".swf")) id="SWF";
			    else if(strstr(argv[t],".gif")) id="PIC";
			    else if(strstr(argv[t],".png")) id="PIC";
			    else if(strstr(argv[t],".jpeg")) id="PIC";
			    else if(strstr(argv[t],".jpg")) id="PIC";
			    else if(strstr(argv[t],".tif")) id="PIC";
			    else if(strstr(argv[t],".c")) id="SRC";
			    else if(strstr(argv[t],".cc")) id="SRC";
			    else if(strstr(argv[t],".cpp")) id="SRC";
			    else if(strstr(argv[t],".h")) id="SRC";
			    else if(strstr(argv[t],".class")) id="CLA";
			    else if(strstr(argv[t],".java")) id="SRC";
			    else if(strstr(argv[t],".exe")) id="EXE";
			    else if(strstr(argv[t],".ttf")) id="FON";
			    else if(strstr(argv[t],".pfa")) id="FON";
			    else if(strstr(argv[t],".pfb")) id="FON";
			    else if(strstr(argv[t],".afm")) id="FON";
			    else if(strstr(argv[t],".1")) id="DOC";
			    else if(strstr(argv[t],".doc")) id="DOC";
			    else if(strstr(argv[t],".hlp")) id="DOC";
			    else if(strstr(argv[t],".dll")) id="DLL";

			    fread(buf,l,1,fi);
			    zwriter->write(zwriter,id,3);
			    zwriter->write(zwriter,&l,4);
			    int sl=strlen(argv[t]);
			    zwriter->write(zwriter,argv[t],sl+1); //write filename
			    zwriter->write(zwriter,buf,l); //write data
			    fprintf(stderr,"[%s] %s: %d bytes written.\n", id, argv[t], l);
			    fclose(fi);
                            free(buf);
		    }
		    else
		    {
			    fprintf(stderr,"%s not found.\n", argv[t]);
		    }
	    }
    }
    zwriter->finish(zwriter);
}
