/* mkarchive.c

   Part of the rfx installer (Main program).
   
   Copyright (c) 2004-2008 Matthias Kramm <kramm@quiss.org> 
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

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

/* ---------------------------- file writer ------------------------------- */

typedef struct
{
    FILE*fi;
} filewrite_t;
static int writer_filewrite_write(writer_t*w, void* data, int len) 
{
    filewrite_t * fw= (filewrite_t*)w->internal;
    return fwrite(data, len, 1, fw->fi);
}
static void writer_filewrite_finish(writer_t*w)
{
    filewrite_t *mr = (filewrite_t*)w->internal;
    fclose(mr->fi);
    free(w->internal);
    memset(w, 0, sizeof(writer_t));
}
writer_t*writer_init_filewriter(char*filename)
{
    writer_t*w = malloc(sizeof(writer_t));
    FILE*fi = fopen(filename, "wb");
    if(!fi) {
	perror(filename);
	return 0;
    }
    filewrite_t *mr = (filewrite_t *)malloc(sizeof(filewrite_t));
    mr->fi = fi;
    memset(w, 0, sizeof(writer_t));
    w->write = writer_filewrite_write;
    w->finish = writer_filewrite_finish;
    w->internal = mr;
    return w;
}

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
    fprintf(i->fi, "int crndata_len = %d;\n", i->pos);
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
    int num = 0;

    int fullsize = 0;
    for(t=1;t<argn;t++)
    {
	if(argv[t][0]!='-')
	{
	    FILE*fi=fopen(argv[t],"rb");
	    if(fi) {
		fseek(fi,0,SEEK_END);
		long l=ftell(fi);
		fclose(fi);
		fullsize += l;
		num++;
	    }
	}
    }


#ifdef ZLIB
    writer_t*include_writer = writer_init_includewriter("crnfiles.c");
    writer_t*zwriter = writer_init_zwriter(include_writer);
#else //LZMA
    unlink("crnfiles.dat");
    unlink("crnfiles.7z");
    unlink("crnfiles.c");
    writer_t*zwriter = writer_init_filewriter("crnfiles.dat");
#endif
    unsigned char n1=num;
    unsigned char n2=num>>8;
    unsigned char n3=num>>16;
    unsigned char n4=num>>24;
    zwriter->write(zwriter,&n1,1);
    zwriter->write(zwriter,&n2,1);
    zwriter->write(zwriter,&n3,1);
    zwriter->write(zwriter,&n4,1);

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
			    else if(strstr(argv[t],".ico")) id="PIC";
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
			    else if(strstr(argv[t],".txt")) id="TXT";
			    else if(strstr(argv[t],".hlp")) id="DOC";
			    else if(strstr(argv[t],".dll")) id="DLL";

			    fread(buf,l,1,fi);
			    zwriter->write(zwriter,id,3);
			    unsigned char b1=l;
			    unsigned char b2=l>>8;
			    unsigned char b3=l>>16;
			    unsigned char b4=l>>24;
			    zwriter->write(zwriter,&b1,1);
			    zwriter->write(zwriter,&b2,1);
			    zwriter->write(zwriter,&b3,1);
			    zwriter->write(zwriter,&b4,1);
			    int sl=strlen(argv[t]);
			    if(sl>255) {
				fprintf(stderr, "Error: filename %s too long\n", argv[t]);
			    }
			    unsigned char b = sl;
			    zwriter->write(zwriter,&b,1); //write filename len
			    zwriter->write(zwriter,argv[t],sl); //write filename
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
    char*id_end = "END";
    zwriter->write(zwriter,id_end,3);
    zwriter->finish(zwriter);

#ifndef ZLIB
    if(system("do_lzma e crnfiles.dat crnfiles.7z")&0xff00) {
	perror("do_lzma");
	exit(1);
    }
    FILE*fi = fopen("crnfiles.7z", "rb");
    if(!fi) {
	perror("crnfiles.7z");
	exit(1);
    }
    writer_t*w = writer_init_includewriter("crnfiles.c");
    while(!feof(fi)) {
	char buf[4096];
	int len = fread(buf,1,4096,fi);
	if(!len)
	    break;
	w->write(w, buf, len);
    }
    w->finish(w);
    fclose(fi);
    if(unlink("crnfiles.dat")) {
        perror("crnfiles.dat");
        exit(1);
    }
    if(unlink("crnfiles.7z")) {
        perror("crnfiles.7z");
        exit(1);
    }
#endif

    fi = fopen("crnfiles.c", "ab+");
    fprintf(fi, "\nint crn_decompressed_size=%d;\n", fullsize);
    fclose(fi);

    return 0;
}

