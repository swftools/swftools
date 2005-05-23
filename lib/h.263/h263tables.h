/* h263tables.h

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
extern struct rle_params rle_params[];

extern struct huffcode mcbpc_inter[];
extern struct huffcode cbpy[];
extern struct huffcode mvd[];
extern struct huffcode rle[];
