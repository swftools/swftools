#ifndef __dct_h__
#define __dct_h__
    
void dct(int*src);
void idct(int*src);

void preparequant(int quant);
void dct2(int*src, int*dest);

int zigzagtable[64];
void zigzag(int*src);

#endif //__dct_h__
