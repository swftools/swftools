/* dct.c

   DCT implementations and test routines.
   
   Copyright (c) 2003 Matthias Kramm <kramm@quiss.org>

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

#include <memory.h>

int zigzagtable[64] = {
    0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63};

static double table[8][8] =
{
{0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548,0.707106781186548},
{0.980785280403230,0.831469612302545,0.555570233019602,0.195090322016128,-0.195090322016128,-0.555570233019602,-0.831469612302545,-0.980785280403230},
{0.923879532511287,0.382683432365090,-0.382683432365090,-0.923879532511287,-0.923879532511287,-0.382683432365090,0.382683432365090,0.923879532511287},
{0.831469612302545,-0.195090322016128,-0.980785280403230,-0.555570233019602,0.555570233019602,0.980785280403230,0.195090322016129,-0.831469612302545},
{0.707106781186548,-0.707106781186547,-0.707106781186548,0.707106781186547,0.707106781186548,-0.707106781186547,-0.707106781186547,0.707106781186547},
{0.555570233019602,-0.980785280403230,0.195090322016128,0.831469612302545,-0.831469612302545,-0.195090322016128,0.980785280403231,-0.555570233019602},
{0.382683432365090,-0.923879532511287,0.923879532511287,-0.382683432365090,-0.382683432365091,0.923879532511287,-0.923879532511286,0.382683432365090},
{0.195090322016128,-0.555570233019602,0.831469612302545,-0.980785280403231,0.980785280403230,-0.831469612302545,0.555570233019602,-0.195090322016129}
};

void dct(int*src)
{
    double tmp[64];
    int x,y,u,v,t;

    for(v=0;v<8;v++)
    for(u=0;u<8;u++)
    {
	double c = 0;
	for(x=0;x<8;x++)
	{
	    c+=table[u][x]*src[v*8+x];
	}
	tmp[v*8+u] = c;
    }
    for(u=0;u<8;u++)
    for(v=0;v<8;v++)
    {
	double c = 0;
	for(y=0;y<8;y++)
	{
	    c+=table[v][y]*tmp[y*8+u];
	}
	src[v*8+u] = (int)(c*0.25+0.5);
    }
}

void idct(int*src)
{
    double tmp[64];
    int x,y,u,v;
    for(y=0;y<8;y++)
    for(x=0;x<8;x++)
    {
	double c = 0;
	for(u=0;u<8;u++)
	{
	    c+=table[u][x]*src[y*8+u];
	}
	tmp[y*8+x] = c;
    }
    for(y=0;y<8;y++)
    for(x=0;x<8;x++)
    {
	double c = 0;
	for(v=0;v<8;v++)
	{
	    c+=table[v][y]*tmp[v*8+x];
	}
	src[y*8+x] = (int)(c*0.25+0.5);
    }
}

static double c[8] = {1.0,
0.980785280403230, // cos(Pi*1/16), sin(Pi*7/16)
0.923879532511287, // cos(Pi*2/16), sin(Pi*6/16)
0.831469612302545, // cos(Pi*3/16), sin(Pi*5/16)
0.707106781186548, // cos(Pi*4/16), sin(Pi*4/16), 1/sqrt(2)
0.555570233019602, // cos(Pi*5/16), sin(Pi*3/16)
0.382683432365090, // cos(Pi*6/16), sin(Pi*2/16)
0.195090322016128 // cos(Pi*7/16), sin(Pi*1/16)
};

static double cc[8];
static int ccquant = -1;

void preparequant(int quant)
{
    if(ccquant == quant)
	return;
    cc[0] = c[0]/(quant*2*4);
    cc[1] = c[1]/(quant*2*4);
    cc[2] = c[2]/(quant*2*4);
    cc[3] = c[3]/(quant*2*4);
    cc[4] = c[4]/(quant*2*4);
    cc[5] = c[5]/(quant*2*4);
    cc[6] = c[6]/(quant*2*4);
    cc[7] = c[7]/(quant*2*4);
    ccquant = quant;
}

inline static void innerdct(const double*a,double*b, const double*c)
{
    // c1*c7*2 = c6
    // c2*c6*2 = c4
    // c3*c5*2 = c2
    // c4*c4*2 = 1

     //{  1,  3,  5,  7, -7, -5, -3, -1},
     //{  3, -7, -1, -5,  5,  1,  7, -3},
     //{  5, -1,  7,  3, -3, -7,  1, -5},
     //{  7, -5,  3, -1,  1, -3,  5, -7}
    double b0,b1,b2,b3,b4,b5;
    b2 = (a[0]+a[7]);
    b3 = (a[1]+a[6]);
    b4 = (a[2]+a[5]);
    b5 = (a[3]+a[4]);

    b0 = (b2+b5)*c[4];
    b1 = (b3+b4)*c[4];
    b[0*8] = b0 + b1;
    b[4*8] = b0 - b1;
    b[2*8] = (b2-b5)*c[2] + (b3-b4)*c[6];
    b[6*8] = (b2-b5)*c[6] + (b4-b3)*c[2];

    b0 = (a[0]-a[7]);
    b1 = (a[1]-a[6]);
    b2 = (a[2]-a[5]);
    b3 = (a[3]-a[4]);

    b[1*8] = b0*c[1] + b1*c[3] + b2*c[5] + b3*c[7];
    b[3*8] = b0*c[3] - b1*c[7] - b2*c[1] - b3*c[5];
    b[5*8] = b0*c[5] - b1*c[1] + b2*c[7] + b3*c[3];
    b[7*8] = b0*c[7] - b1*c[5] + b2*c[3] - b3*c[1];
}

void dct2(int*src, int*dest)
{
    double tmp[64], tmp2[64];
    double*p;
    int u,x,v,t;

    for(t=0;t<64;t++)
	tmp2[t] = src[t];

    for(v=0;v<8;v++)
    {
	double* a=&tmp2[v*8];
	double* b=&tmp[v];
	innerdct(a,b,c);
    }
    for(v=0;v<8;v++)
    {
	double* a=&tmp[v*8];
	double* b=&tmp2[v];
	innerdct(a,b,cc);
    }
    for(t=0;t<64;t++) {
	int v = (int)(tmp2[t]);
	dest[zigzagtable[t]] = v;
    }
}


void zigzag(int*src)
{
    int tmp[64];
    int t;
    for(t=0;t<64;t++) {
	tmp[zigzagtable[t]] = src[t];
    }
    memcpy(src, tmp, sizeof(int)*64);
}


