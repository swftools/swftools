/* base64.c

   Copyright (c) 2010 Matthias Kramm <kramm@quiss.org>

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"

static const char base64[65]=
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void write_base64(FILE*fi, const char* s)
{
    const unsigned char*b = (const unsigned char*)s;
    const unsigned char*end = b + strlen(b);
    const unsigned char*end_minus_3 = end - 3;

    while(b <= end_minus_3) {
	char data[4];
	data[0] = base64[(b[0]>>2)&63        ];
	data[1] = base64[(b[0]<<4|b[1]>>4)&63];
	data[2] = base64[(b[1]<<2|b[2]>>6)&63];
	data[3] = base64[(b[2])&63           ];
	fwrite(data, 4, 1, fi);
	b += 3;
    }

    unsigned char data[4];
    switch(end - b) {
	case 0: 
	    return; //done
	case 1:
	    data[0] = base64[(b[0]>>2)&63];
	    data[1] = base64[(b[0]<<4)&63];
	    data[2] = '=';
	    data[3] = '=';
	break;
	case 2:
	    data[0] = base64[(b[0]>>2)&63];
	    data[1] = base64[(b[0]<<4|b[1]>>4)&63];
	    data[2] = base64[(b[1]<<2)&63];
	    data[3] = '=';
	break;
	default:
	    fprintf(stderr, "internal error in base64 implementation (%d)\n", end-b);
	break;
    }
    fwrite(data, 4, 1, fi);
}
