/* video.c
   Shows the structure of a swf file containing video

   Part of the swftools package.
   
   Copyright (c) 2003 Matthias Kramm <kramm@quiss.org> */

#include "../config.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"
#include "h263tables.c"

static char * filename = 0;
static char * indent = "                ";
int hex = 0;
int debug = 0;

struct options_t options[] =
{
 {"v","verbose"},
 {"V","version"},
 {"d","hex"},
 {"M","video"},
 {0,0}
};

/* TODO:
   * check rle tables
*/
int args_callback_option(char*name,char*val)
{
    if(!strcmp(name, "V")) {
        printf("swfdump - part of %s %s\n", PACKAGE, VERSION);
        exit(0);
    }
    else if(name[0]=='d') {
	hex = 1;
	return 0;
    }
    else if(name[0]=='v') {
	debug = 1;
	return 0;
    }
    else {
        printf("Unknown option: -%s\n", name);
	exit(1);
    }

    return 0;
}
int args_callback_longoption(char*name,char*val)
{
    return args_long2shortoption(options, name, val);
}
void args_callback_usage(char*name)
{
    printf("Usage: %s [-at] file.swf\n", name);
    printf("\t-h , --help\t\t Print help and exit\n");
    printf("\t-d , --hex\t\t Print hex output of tag data, too\n");
    printf("\t-v , --verbose\t\t Print debugging information\n");
    printf("\t-V , --version\t\t Print program version and exit\n");
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

#define DEBUG if(debug)

void handleVideoStream(TAG*tag, char*prefix)
{
    U16 id = swf_GetU16(tag);
    U16 frames = swf_GetU16(tag);
    U16 width = swf_GetU16(tag);
    U16 height = swf_GetU16(tag);
    U8 flags = swf_GetU8(tag); //5-2(videopacket 01=off 10=on)-1(smoothing 1=on)
    U8 codec = swf_GetU8(tag);
    printf(" (%d frames, %dx%d", frames, width, height);
    if(flags&1)
	printf(" smoothed");
    if(codec == 2)
	printf(" sorenson h.263)");
    else
	printf(" codec 0x%02x)", codec);
}

int checkhufftable(struct huffcode*code, char*name)
{
    int t=0;
    while(code[t].code) {
	int s=0;
	if(strlen(code[t].code)!=code[t].len) {
	    printf("len mismatch in %s, index %d\n", name, t);
	    exit(1);
	}
	if(t != code[t].index) {
	    printf("index mismatch in %s, index %d\n", name, t);
	    exit(1);
	}
	while(code[s].code) {
	    char*a = code[s].code;
	    char*b = code[t].code;
	    int ai = s;
	    int bi = t;
	    if(s==t) {s++;continue;}
	    if(code[t].len < code[s].len) {
	         a = code[t].code;
	         b = code[s].code;
		 ai = t; bi = s;
	    }
	    if(!strncmp(a,b,strlen(a))) {
		printf("index %d (%s) is prefix of %d (%s)\n", ai, a, bi, b);
		exit(1);
	    }

	    s++;
	}

	t++;
    }
}

int gethuffvalue(TAG*tag, struct huffcode*code)
{
    int len = 0;
    char bits[80];
    while(1) {
	int t=0,pot=0;
	bits[len] = swf_GetBits(tag, 1)+0x30;
	len++;
	bits[len] = 0;
	while(code[t].code) {
	    if(!strcmp(bits, code[t].code))
		return t;
	    t++;
	    if(code[t].len >= len)
		pot++;
	}
	if(!pot) {
	    int nr=0;
	    printf("error: %s\n", bits);
	    while(tag->pos < tag->len && nr<80) {
		int b = swf_GetBits(tag, 1);
		printf("%d", b);
		nr++;
	    }
	    if(nr==80)
		printf("...");
	    printf("\n");
	    exit(1);
	    return -1;
	}
    }

	/*type = 0; // mb-type =3, cbpc = 00
	if(!bit) {
	    printf("can't handle i-frame mcbpc bits %d yet\n", bit);
	}
	bit = swf_GetBits(tag, 1);
	type = 0; // mb-type =0, cbpc = 00
	if(!bit) {
	    bit = swf_GetBits(tag, 2);
	    type = 8; // mb-type =2, cbpc = 00
	    if(bit!=3) {
		printf("can't handle p-frame mcbpc bits 0-%d yet\n", bit);
		exit(1);
	    }
	}*/
}
		
void get_DC_TCOEF(TAG*tag, int t, int has_dc, int has_tcoef)
{
    int dc;
    int ac;// = swf_GetBits();
    int index;
    int pos = 0;
    //printf("DC:%d\n", dc);
    if(has_dc) {
	dc = swf_GetBits(tag, 8);
	if(dc == 0 || dc == 128) {
	    printf("error: dc=%d\n", dc);
	    exit(1);
	}
	DEBUG printf(" %d ", dc);
	pos++;
    }

    if(has_tcoef) {
	DEBUG printf("[");
	while(1) {
	    int last;
	    int run;
	    int level;
	    index = gethuffvalue(tag, rle);
	    last = rle_params[index].last;
	    run = rle_params[index].run;
	    level = rle_params[index].level;
	    //DEBUG printf("index:%d\n", index);
	    if(index == RLE_ESCAPE) {
		last = swf_GetBits(tag, 1);
		run = swf_GetBits(tag, 6);
		level = swf_GetBits(tag, 8);
		if(run)
		    DEBUG printf("(%d) E%d", run, level);
		else
		    DEBUG printf("E");
		if(level == 0 || level == 128) {
		    printf("error: level=%d\n", level);
		    exit(1);
		}
		level = (int)((signed char)level);
	    } else {
		int sign = swf_GetBits(tag, 1);
		if(sign)
		    level = -level;
		if(run)
		    DEBUG printf("(%d) %s%d", run, level>0?"+":"",level);
		else
		    DEBUG printf("%s%d", level>0?"+":"",level);
	    }
	    pos += run+1;
	    //DEBUG printf("run:%d level:%d\n", run, level);
	    if(last) {
		DEBUG printf("] pos: %d", pos);
		if(pos>64) {
		    printf("\nerror:bad pos (%d)\n", pos);
		    exit(1);
		}
		return;
	    }
	}
    }
}
	    
void readMVD(TAG*tag)
{
    int index = gethuffvalue(tag, mvd);
    DEBUG printf("mvd index:%d\n", index);
}

char has_quant[] = {0,1,0,0,1};
char has_mvd[] = {1,1,3,0,0};

#define TYPE_INTRA 0
#define TYPE_INTER 1

int tagnr = 0;

void decode_block(TAG*tag, int pictype)
{
    int t;
    int mb_type = -1, cbpc = -1;
    int dquant;
    int cbpy_index, cbpy_value;
    int intrablock = 0;
    int type;
    if(pictype == TYPE_INTER) /* non-intra pictures have a cod flag */
    {
	int cod = swf_GetBits(tag, 1);
	DEBUG printf("cod=%d\n",cod);
	if(cod) {
	    printf(".");
	    return;
	}
    }
    type = -1;
    
    /* read mcbpc */

    if(pictype == TYPE_INTRA) { /* I-frame */
	type = gethuffvalue(tag, mcbpc_intra);
	DEBUG printf("mcbpc=%d\n",type);
	mb_type = mcbpc_intra_params[type].mb_type;
	cbpc = mcbpc_intra_params[type].cbpc;
	if(type == MCBPC_INTRA_STUFFING) {
	    printf("stuffing not supported yet!\n");
	    exit(1); //TODO: goto COD
	}
    } 
    else if(pictype == 1) { /* P-frame */
	type = gethuffvalue(tag, mcbpc_inter);
	DEBUG printf("mcbpc=%d\n",type);
	mb_type = mcbpc_inter_params[type].mb_type;
	cbpc = mcbpc_inter_params[type].cbpc;
	if(type == MCBPC_INTER_STUFFING) {
	    printf("stuffing not supported yet!(2)\n");
	    exit(1); //TODO: goto COD
	}
    }

    if(mb_type == 3 || mb_type == 4)
    {
	intrablock = 1;
    }

    printf("%c", "vqVii"[mb_type]);

    DEBUG printf("mcbpc type: %d mb_type:%d cbpc:%d\n", type, mb_type, cbpc);

    /* read cbpy */

    cbpy_index = gethuffvalue(tag, cbpy);
    cbpy_value = cbpy_index;
    if(!intrablock)
	cbpy_value ^= 15;
    DEBUG printf("cbpy value:%d (%d)\n", cbpy_value, cbpy_index);


    /* I 0: 00 mcbpc/cbpy 
       P 0: 00 cod/mcbpc/cbpy/mvd
       P 6: 10 cod/mcbpc/cbpy/dquant/mvd
       P 8: 00 cod/mcbpc/cbpy/mvd/mvd24
    */

    /* quantizer */
    if(has_quant[mb_type]) {
	dquant = swf_GetBits(tag, 2);
	if(dquant == 0) dquant = -1;
	else if(dquant == 1) dquant = -2;
	else if(dquant == 2) dquant = +1;
	else if(dquant == 3) dquant = +2;
	DEBUG printf("dquant: %d\n", dquant);
    }

    if(has_mvd[mb_type]&1) {
	readMVD(tag); //horizontal
	readMVD(tag); //vertical
    }
    if(has_mvd[mb_type]&2) {
	/* only in advanced prediction mode */
	readMVD(tag); //horizontal
	readMVD(tag); //vertical
	readMVD(tag); //horizontal
	readMVD(tag); //vertical
	readMVD(tag); //horizontal
	readMVD(tag); //vertical
    }

    for(t=0;t<4;t++) {
	int has_intradc = intrablock;
	int has_tcoef = cbpy_value & (8>>t);
	DEBUG printf("luminance%d ", t);
	get_DC_TCOEF(tag, t, has_intradc, has_tcoef); /*luminance - affected by cbpy*/
	DEBUG printf("\n");
    }
    for(t=0;t<2;t++) {
	int has_intradc = intrablock;
	int has_tcoef = cbpc & (2>>t);
	DEBUG printf("chrominance%d ", t); 
	get_DC_TCOEF(tag, t, has_intradc, has_tcoef); /*chrominance - affected by mcbc*/
	DEBUG printf("\n");
    }
}

void handleVideoFrame(TAG*tag, char*prefix)
{
    U32 code, version, reference, sizeflags;
    U32 width, height;
    U8 blocktype, pictype;
    U16 id = swf_GetU16(tag);
    U16 frame = swf_GetU16(tag);
    U8 deblock,flags, tmp, bit;
    U32 quantizer, extrainfo;
    int skipped = 0;
    int pos=0;
    int num;
    int disposable = 0;
    int blocknum;
    int bbx,bby,bx,by;
    char*types[] = {"intra- (I-)frame", "inter- (P-)frame", "disposable interframe", "<reserved>"};
    printf("============================= frame %d ===================================", frame);

    /* video packet follows */
    printf("\n");
    code = swf_GetBits(tag, 17);
    if(code!=1) {
	printf("code: %x (?)\n", code);
	return;
    }
    version = swf_GetBits(tag, 5); /*actually, part of the picture start code */
    //printf("version: %x\n", version); /*usually 0*/
    if(version >= 1) {
	/* version 1 has some different transform coefficient coding which we
	   can't handle yet */
	printf("spark version %d not supported yet\n", version);
	exit(1);
    }
    reference = swf_GetBits(tag, 8);
    DEBUG printf("reference: %d\n", reference); /*usually same as frame number (unless frames were skipped while encoding)*/

    sizeflags = swf_GetBits(tag, 3);
    switch(sizeflags)
    {
	case 0: width = swf_GetU8(tag); height = swf_GetU8(tag); break;
	case 1: width = swf_GetBits(tag, 16); height = swf_GetBits(tag, 16); break;
	case 2: width = 352; height = 288; break;
	case 3: width = 176; height = 144; break;
	case 4: width = 128; height = 96; break;
	case 5: width = 320; height = 240; break;
	case 6: width = 160; height = 120; break;
	case 7: width = -1; height = -1;/*reserved*/ break;
    }
    
    pictype = swf_GetBits(tag, 2);
    if(pictype==3) {
	printf("error: unknown pictype: %d\n", pictype);
	exit(1);
    }
    if(pictype==2)  {
	pictype = TYPE_INTER;
	disposable = 1;
    }
    if(pictype==TYPE_INTER)
	printf("INTER P%s", disposable?" (disposeable)":"");
    else
	printf("INTRA I");

    deblock = swf_GetBits(tag, 1); /*usually 0*/
    DEBUG printf("deblock: %d\n", deblock);
    quantizer = swf_GetBits(tag, 5); /*usually 9*/
    DEBUG printf("quantizer: %d\n", quantizer);

    extrainfo = swf_GetBits(tag, 1); /*usually none */
    while(extrainfo) {
	extrainfo = swf_GetBits(tag, 8);
	printf("extrainfo: %02x\n", extrainfo);
	extrainfo = swf_GetBits(tag, 1);
    }

    /* macro block */
    bbx = (width+15)/16;
    bby = (height+15)/16;
    blocknum = bbx*bby;
    printf("%dx%d [blocks: %dx%d=%d]\n", width, height, bbx,bby, blocknum);

    /*if(pictype == TYPE_INTER)
	return;*/
    /*if(pictype == TYPE_INTRA)
	return;*/

    /*tagnr++;
    if(tagnr!=2)
	return;*/

    DEBUG printf("\n");
    for(by=0;by<bby;by++)
    {
	for(bx=0;bx<bbx;bx++)
	{
	    decode_block(tag, pictype);
	}
	printf("\n");
    }
}

void hexdumpTag(TAG*tag, char* prefix)
{
    int t;
    printf("                %s-=> ",prefix);
    for(t=0;t<tag->len;t++) {
	printf("%02x ", tag->data[t]);
	if((t && ((t&15)==15)) || (t==tag->len-1))
	{
	    if(t==tag->len-1)
		printf("\n");
	    else
		printf("\n                %s-=> ",prefix);
	}
    }
}

int main (int argc,char ** argv)
{
    TAG*tag;
    SWF swf;
    int f;
    char prefix[128];
    int filesize = 0;
    prefix[0] = 0;

    checkhufftable(rle, "rle");
    checkhufftable(mcbpc_intra, "intra");
    checkhufftable(mcbpc_inter, "inter");
    checkhufftable(cbpy, "cbpy");
    checkhufftable(mvd, "mvd");

    processargs(argc, argv);
    if(!filename) {
        fprintf(stderr, "You must supply a filename.\n");
        return 1;
    }

    f = open(filename,O_RDONLY);

    if (f<0) {
        perror("Couldn't open file: ");
        exit(1);
    }
    if FAILED(swf_ReadSWF(f,&swf)) {
        fprintf(stderr, "%s is not a valid SWF file or contains errors.\n",filename);
        close(f);
        exit(1);
    }

    close(f);

    printf("[HEADER]        File version: %d\n", swf.fileVersion);
    if(swf.compressed) {
	printf("[HEADER]        File is zlib compressed.");
	if(filesize && swf.fileSize)
	    printf(" Ratio: %02d%%\n", filesize*100/(swf.fileSize));
	else
	    printf("\n");
    }
    printf("[HEADER]        File size: %ld%s\n", swf.fileSize, swf.compressed?" (Depacked)":"");
    printf("[HEADER]        Frame rate: %f\n",swf.frameRate/256.0);
    printf("[HEADER]        Frame count: %d\n",swf.frameCount);
    printf("[HEADER]        Movie width: %.2f",(swf.movieSize.xmax-swf.movieSize.xmin)/20.0);
    if(swf.movieSize.xmin)
	printf(" (left offset: %.2f)\n", swf.movieSize.xmin/20.0);
    else
	printf("\n");
    printf("[HEADER]        Movie height: %.2f",(swf.movieSize.ymax-swf.movieSize.ymin)/20.0);
    if(swf.movieSize.ymin)
	printf(" (top offset: %.2f)\n", swf.movieSize.ymin/20.0);
    else
	printf("\n");

    tag = swf.firstTag;

    while(tag) {
        char*name = swf_TagGetName(tag);
        char myprefix[128];
        //printf("[%03x] %9ld %s%s", tag->id, tag->len, prefix, swf_TagGetName(tag));

        if(swf_isDefiningTag(tag)) {
            U16 id = swf_GetDefineID(tag);
            //printf(" defines id %04d", id);
        }
	else if(swf_isPseudoDefiningTag(tag)) {
            U16 id = swf_GetDefineID(tag);
            //printf(" adds information to id %04d", id);
	}

	if(tag->id == ST_VIDEOFRAME) {
	    handleVideoFrame(tag, myprefix);
	    //printf("\n");
	}
	else if(tag->id == ST_DEFINEVIDEOSTREAM) {
	    handleVideoStream(tag, myprefix);
	    printf("\n");
	}
	else {
	    //printf("\n");
	}

        sprintf(myprefix, "                %s", prefix);

	if(tag->len && hex) {
	    hexdumpTag(tag, prefix);
	}
        tag = tag->next;
	fflush(stdout);
    }

    swf_FreeTags(&swf);
    return 0;
}


