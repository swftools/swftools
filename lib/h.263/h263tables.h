struct huffcode {
    char*code;
    int len;
    int index;
};

struct mcbpc_intra_params
{
    int index;
    int mb_type;
    int cbpc;
};

struct mcbpc_inter_params
{
    int index;
    int mb_type;
    int cbpc;
};

struct rle_params
{
    int index;
    int last;
    int run;
    int level;
};

#define MCBPC_INTRA_STUFFING 8
#define MCBPC_INTER_STUFFING 20
#define RLE_ESCAPE 102

extern struct huffcode mcbpc_intra[];
extern struct mcbpc_intra_params mcbpc_intra_params[];
extern struct mcbpc_inter_params mcbpc_inter_params[];
extern struct mvd_params mvd_params[];
extern struct rle_params rle_params[];

extern struct huffcode mcbpc_inter[];
extern struct huffcode cbpy[];
extern struct huffcode mvd[];
extern struct huffcode rle[];
