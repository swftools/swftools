/* makegui.c

   Small program to set the gui flag in Win32 executables.
   
   Copyright (c) 2004 Matthias Kramm <kramm@quiss.org> 
 
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
#include <stdlib.h>

int main(int argn, char*argv[])
{
    if(argn<2) {
	printf("Usage:\n");
	printf("\t%s program.exe\n", argv[0]);
	return 0;
    }
    FILE*fi = fopen(argv[1], "rb+");
    if(!fi) {
	perror(argv[1]);
	exit(1);
    }
    fseek(fi,220,SEEK_SET); 
    char two=2;
    printf("Setting Win32 GUI flag in %s\n", argv[1]);
    fwrite(&two, 1, 1, fi);
    fclose(fi);
    return 0;
}

