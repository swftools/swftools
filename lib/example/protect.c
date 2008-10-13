/* protect.c 

   Example for protecting/unprotecting SWFs.
   
   Part of the swftools package.

   Copyright (C) 2004 Matthias Kramm

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

#include <stdio.h>
#include "../rfxswf.h"

int main(int argn,char ** argv)
{ 
    int fi;
    SWF swf;
    TAG* tag;
    char* outfilename = "test.swf";
    char*oldpassword = "fireworks";
    char*newpassword = "cyberdine";

    if (argn<1)
    { 
	return 0;
    }
	
    fi = open(argv[1],O_RDONLY|O_BINARY);
	
    if (fi<=0)
    { 
	fprintf(stderr,"Couldn't open %s\n", argv[1]);
    }

    if(swf_ReadSWF(fi,&swf)<0)
    { 
	fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",argv[1]);
	close(fi);
    }

    tag = swf.firstTag;

    while(tag) {
	TAG*next = tag->next;
	//void  swf_SetPassword(TAG * t, const char * password);
	//int   swf_VerifyPassword(TAG * t, const char * password);
	if(tag->id == ST_PROTECT) {
	    int ret;
	    ret = swf_VerifyPassword(tag, oldpassword);
	    if(!ret) printf("Password validation failed\n");
	    else printf("Password ok\n");

	    swf_DeleteTag(&swf, tag);
	}
	tag = next;
    }

    tag = swf_InsertTag(swf.firstTag, ST_PROTECT);
    swf_SetPassword(tag, newpassword);

    fi = open(outfilename, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
    if(fi<0) {
	fprintf(stderr, "couldn't create output file %s", outfilename);
    }
    if(swf_WriteSWF(fi, &swf)<0) 
        fprintf(stderr, "WriteSWF() failed.\n");

    close(fi);
    
    return 0;
}

