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

static char * filename = 0;
static char * indent = "                ";
int hex = 0;

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

#define DEBUG if(0)

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

struct huffcode {
    char*code;
    int len;
    int index;
};

#define MCBPC_INTRA_STUFFING 8

struct huffcode mcbpc_intra[] = {
{"1",1, 0},		 /*cbpc-00 mb type 3*/
{"001",3, 1},         /*cbpc-01 mb type 3*/
{"010",3, 2},         /*cbpc-10 mb type 3*/
{"011",3, 3},         /*cbpc-11 mb type 3*/
{"0001",4, 4},        /*cbpc-00 mb type 4*/
{"000001",6, 5},      /*cbpc-01 mb type 4*/
{"000010",6, 6},      /*cbpc-10 mb type 4*/
{"000011",6, 7},      /*cbpc-11 mb type 4*/
{"000000001",9, 8},   /* stuffing */
{0,0,0},
};

struct mcbpc_intra_params
{
    int index;
    int mb_type;
    int cbpc;
} mcbpc_intra_params[] =
{{0, 3, 0}, //00 
 {1, 3, 1}, //01 
 {2, 3, 2}, //10 
 {3, 3, 3}, //11 
 {4, 4, 0}, //00 
 {5, 4, 1}, //01 
 {6, 4, 2}, //10 
 {7, 4, 3}, //11 
 {8 -1, -1}
};

#define MCBPC_INTER_STUFFING 20

struct huffcode mcbpc_inter[] = {
{"1", 1, 0},
{"0011", 4, 1},
{"0010", 4 ,2},
{"000101", 6, 3},
{"011", 3, 4},
{"0000111", 7, 5},
{"0000110", 7, 6},
{"000000101", 9, 7},
{"010", 3, 8},
{"0000101", 7, 9},
{"0000100", 7, 10},
{"00000101", 8, 11},
{"00011", 5, 12},
{"00000100", 8, 13},
{"00000011", 8, 14},
{"0000011", 7, 15},
{"000100", 6, 16},
{"000000100", 9, 17},
{"000000011", 9, 18},
{"000000010", 9, 19},
{"000000001", 9, 20}, /*stuffing*/

/* h.263+ only
{"00000000010", 11, 21},
{"0000000001100", 13, 22},
{"0000000001110", 13, 23},
{"0000000001111", 13, 24},
*/
{0,0,0}
};

struct mcbpc_inter_params
{
    int index;
    int mb_type;
    int cbpc;
} mcbpc_inter_params[] =
{{0, 0, 0}, {1, 0, 1}, {2, 0, 2}, {3, 0, 3}, {4, 1, 0},
 {5, 1, 1}, {6, 1, 2}, {7, 1, 3}, {8, 2, 0}, {9, 2, 1},
 {10, 2, 2}, {11, 2, 3}, {12, 3, 0}, {13, 3, 1}, {14, 3, 2},
 {15, 3, 3}, {16, 4, 0}, {17, 4, 1}, {18, 4, 2}, {19, 4, 3},
 {20, -1, -1}, //stuffing
 //{21, 5, 0}, {22, 5, 1}, {23, 5, 2}, {24, 5, 3}, //h.263+ only
};

struct huffcode cbpy[] = {
{"0011", 4, 0},
{"00101", 5, 1},
{"00100", 5, 2},
{"1001", 4, 3},
{"00011", 5, 4},
{"0111", 4, 5},
{"000010", 6, 6},
{"1011", 4, 7},
{"00010", 5, 8},
{"000011", 6, 9},
{"0101", 4, 10},
{"1010", 4, 11},
{"0100", 4, 12},
{"1000", 4, 13},
{"0110", 4, 14},
{"11", 2, 15},
{0,0,0}
};

struct huffcode mvd[] = 
{
{"0000000000101", 13, 0},
{"0000000000111", 13, 1},
{"000000000101", 12, 2},
{"000000000111", 12, 3},
{"000000001001", 12, 4},
{"000000001011", 12, 5},
{"000000001101", 12, 6},
{"000000001111", 12, 7},
{"00000001001", 11, 8},
{"00000001011", 11, 9},
{"00000001101", 11, 10},
{"00000001111", 11, 11},
{"00000010001", 11, 12},
{"00000010011", 11, 13},
{"00000010101", 11, 14},
{"00000010111", 11, 15},
{"00000011001", 11, 16},
{"00000011011", 11, 17},
{"00000011101", 11, 18},
{"00000011111", 11, 19},
{"00000100001", 11, 20},
{"00000100011", 11, 21},
{"0000010011", 10, 22},
{"0000010101", 10, 23},
{"0000010111", 10, 24},
{"00000111", 8, 25},
{"00001001", 8, 26},
{"00001011", 8, 27},
{"0000111", 7, 28},
{"00011", 5, 29},
{"0011", 4, 30},
{"011", 3, 31},
{"1", 1, 32},
{"010", 3, 33},
{"0010", 4, 34},
{"00010", 5, 35},
{"0000110", 7, 36},
{"00001010", 8, 37},
{"00001000", 8, 38},
{"00000110", 8, 39},
{"0000010110", 10, 40},
{"0000010100", 10, 41},
{"0000010010", 10, 42},
{"00000100010", 11, 43},
{"00000100000", 11, 44},
{"00000011110", 11, 45},
{"00000011100", 11, 46},
{"00000011010", 11, 47},
{"00000011000", 11, 48},
{"00000010110", 11, 49},
{"00000010100", 11, 50},
{"00000010010", 11, 51},
{"00000010000", 11, 52},
{"00000001110", 11, 53},
{"00000001100", 11, 54},
{"00000001010", 11, 55},
{"00000001000", 11, 56},
{"000000001110", 12, 57},
{"000000001100", 12, 58},
{"000000001010", 12, 59},
{"000000001000", 12, 60},
{"000000000110", 12, 61},
{"000000000100", 12, 62},
{"0000000000110", 13, 63},
{0,0,0}};

struct huffcode rle[] = 
{
{"10", 2, 0},
{"1111", 4, 1},
{"010101", 6, 2},
{"0010111", 7, 3},
{"00011111", 8, 4},
{"000100101", 9, 5},
{"000100100", 9, 6},
{"0000100001", 10, 7},
{"0000100000", 10, 8},
{"00000000111", 11, 9},
{"00000000110", 11, 10},
{"00000100000", 11, 11},
{"110", 3, 12},
{"010100", 6, 13},
{"00011110", 8, 14},
{"0000001111", 10, 15},
{"00000100001", 11, 16},
{"000001010000", 12, 17},
{"1110", 4, 18},
{"00011101", 8, 19},
{"0000001110", 10, 20},
{"000001010001", 12, 21},
{"01101", 5, 22},
{"000100011", 9, 23},
{"0000001101", 10, 24},
{"01100", 5, 25},
{"000100010", 9, 26},
{"000001010010", 12, 27},
{"01011", 5, 28},
{"0000001100", 10, 29},
{"000001010011", 12, 30},
{"010011", 6, 31},
{"0000001011", 10, 32},
{"000001010100", 12, 33},
{"010010", 6, 34},
{"0000001010", 10, 35},
{"010001", 6, 36},
{"0000001001", 10, 37},
{"010000", 6, 38},
{"0000001000", 10, 39},
{"0010110", 7, 40},
{"000001010101", 12, 41},
{"0010101", 7, 42},
{"0010100", 7, 43},
{"00011100", 8, 44},
{"00011011", 8, 45},
{"000100001", 9, 46},
{"000100000", 9, 47},
{"000011111", 9, 48},
{"000011110", 9, 49},
{"000011101", 9, 50},
{"000011100", 9, 51},
{"000011011", 9, 52},
{"000011010", 9, 53},
{"00000100010", 11, 54},
{"00000100011", 11, 55},
{"000001010110", 12, 56},
{"000001010111", 12, 57},
{"0111", 4, 58},
{"000011001", 9, 59},
{"00000000101", 11, 60},
{"001111", 6, 61},
{"00000000100", 11, 62},
{"001110", 6, 63},
{"001101", 6, 64},
{"001100", 6, 65},
{"0010011", 7, 66},
{"0010010", 7, 67},
{"0010001", 7, 68},
{"0010000", 7, 69},
{"00011010", 8, 70},
{"00011001", 8, 71},
{"00011000", 8, 72},
{"00010111", 8, 73},
{"00010110", 8, 74},
{"00010101", 8, 75},
{"00010100", 8, 76},
{"00010011", 8, 77},
{"000011000", 9, 78},
{"000010111", 9, 79},
{"000010110", 9, 80},
{"000010101", 9, 81},
{"000010100", 9, 82},
{"000010011", 9, 83},
{"000010010", 9, 84},
{"000010001", 9, 85},
{"0000000111", 10, 86},
{"0000000110", 10, 87},
{"0000000101", 10, 88},
{"0000000100", 10, 89},
{"00000100100", 11, 90},
{"00000100101", 11, 91},
{"00000100110", 11, 92},
{"00000100111", 11, 93},
{"000001011000", 12, 94},
{"000001011001", 12, 95},
{"000001011010", 12, 96},
{"000001011011", 12, 97},
{"000001011100", 12, 98},
{"000001011101", 12, 99},
{"000001011110", 12, 100},
{"000001011111", 12, 101},
{"0000011", 7, 102}, /*ESCAPE*/
{0,0,0}
};

#define RLE_ESCAPE 102

struct rle_params
{
    int index;
    int last;
    int run;
    int level;
} rle_params[] =
{{0,0,0,1}, {1,0,0,2}, {2,0,0,3}, {3,0,0,4}, {4,0,0,5}, {5,0,0,6}, {6,0,0,7},
{7,0,0,8}, {8,0,0,9}, {9,0,0,10}, {10,0,0,11}, {11,0,0,12}, {12,0,1,1}, {13,0,1,2},
{14,0,1,3}, {15,0,1,4}, {16,0,1,5}, {17,0,1,6}, {18,0,2,1}, {19,0,2,2}, {20,0,2,3}, {21,0,2,4},
{22,0,3,1}, {23,0,3,2}, {24,0,3,3}, {25,0,4,1}, {26,0,4,2}, {27,0,4,3}, {28,0,5,1},
{29,0,5,2}, {30,0,5,3}, {31,0,6,1}, {32,0,6,2}, {33,0,6,3},
{34,0,7,1}, {35,0,7,2}, {36,0,8,1}, {37,0,8,2}, {38,0,9,1}, {39,0,9,2}, {40,0,10,1}, 
{41,0,10,2}, {42,0,11,1}, {43,0,12,1}, {44,0,13,1}, {45,0,14,1}, {46,0,15,1}, 
{47,0,16,1}, {48,0,17,1}, {49,0,18,1}, {50,0,19,1}, {51,0,20,1}, {52,0,21,1}, 
{53,0,22,1}, {54,0,23,1}, {55,0,24,1}, {56,0,25,1}, {57,0,26,1}, {58,1,0,1}, 
{59,1,0,2}, {60,1,0,3}, {61,1,1,1}, {62,1,1,2}, {63,1,2,1}, {64,1,3,1}, {65, 1,4,1}, 
{66,1,5,1}, {67,1,6,1}, {68,1,7,1}, {69,1,8,1}, {70,1,9,1}, {71,1,10,1}, 
{72,1,11,1}, {73,1,12,1}, {74,1,13,1}, {75,1,14,1}, {76,1,15,1}, {77,1,16,1}, 
{78,1,17,1}, {79,1,18,1}, {80,1,19,1}, {81,1,20,1}, {82,1,21,1}, {83,1,22,1}, 
{84,1,23,1}, {85,1,24,1}, {86,1,25,1}, {87,1,26,1}, {88,1,27,1}, {89,1,28,1}, 
{90,1,29,1}, {91,1,30,1}, {92,1,31,1}, {93,1,32,1}, {94,1,33,1}, {95,1,34,1}, 
{96,1,35,1}, {97,1,36,1}, {98,1,37,1}, {99,1,38,1}, {100,1,39,1}, {101,1,40,1}, 
{102,-1, -1, -1}
};

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
		
void getCoefficient(TAG*tag, int t, int has_dc, int has_tcoef)
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
		    printf("\n");
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
    int dbquant;
    int cbpy_index, cbpy_value;
    int intrablock = 0;
    int type;
    if(pictype == TYPE_INTER) /* non-intra pictures have a cod flag */
    /* TODO: according to the flash spec, this field is always present */
    {
	int cod = swf_GetBits(tag, 1);
	if(cod) {
	    printf(".");
	    return;
	}
    }
    type = -1;
    
    /* read mcbpc */

    if(pictype == TYPE_INTRA) { /* I-frame */
	type = gethuffvalue(tag, mcbpc_intra);
	mb_type = mcbpc_intra_params[type].mb_type;
	cbpc = mcbpc_intra_params[type].cbpc;
	if(type == MCBPC_INTRA_STUFFING)
	    exit(1); //TODO: goto COD
    } 
    else if(pictype == 1) { /* P-frame */
	type = gethuffvalue(tag, mcbpc_inter);
	mb_type = mcbpc_inter_params[type].mb_type;
	cbpc = mcbpc_inter_params[type].cbpc;
	if(type == MCBPC_INTER_STUFFING)
	    exit(1); //TODO: goto COD
    }

    if(mb_type == 3 || mb_type == 4)
    {
	intrablock = 1;
    }
	
    printf("%d", intrablock);

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
	dbquant = swf_GetBits(tag, 2);
	DEBUG printf("quantizer: %d\n", dbquant);
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
	getCoefficient(tag, t, has_intradc, has_tcoef); /*luminance - affected by cbpy*/
	DEBUG printf("\n");
    }
    for(t=0;t<2;t++) {
	int has_intradc = intrablock;
	int has_tcoef = cbpc & (2>>t);
	DEBUG printf("chrominance%d ", t); 
	getCoefficient(tag, t, has_intradc, has_tcoef); /*chrominance - affected by mcbc*/
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
	printf("pictype: %d\n", pictype);
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


