/* pdfswf.h
   Header file for pdfswf.cc.

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 
 
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

#ifndef __pdf_h__
#define __pdf_h__
void pdfswf_init(char*filename, char*userPassword) ;

void pdfswf_setparameter(char*name, char*value);

void pdfswf_setoutputfilename(char*filename);
void pdfswf_drawonlyshapes();
void pdfswf_enablezlib();
void pdfswf_ignoredraworder();
void pdfswf_linksopennewwindow();
void pdfswf_jpegquality(int);
void pdfswf_performconversion();
void pdfswf_storeallcharacters();
void pdfswf_insertstop();
void pdfswf_setversion(int);


int pdfswf_numpages();
void pdfswf_convertpage(int page) ;
void pdfswf_close();
#endif //__pdf_h__
