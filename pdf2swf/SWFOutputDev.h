/* pdfswf.h
   Header file for pdfswf.cc.

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __pdf_h__
#define __pdf_h__
void pdfswf_init(char*filename, char*userPassword) ;
void pdfswf_setoutputfilename(char*filename);
void pdfswf_drawonlyshapes();
void pdfswf_ignoredraworder();
void pdfswf_linksopennewwindow();
void pdfswf_jpegquality(int);
void pdfswf_performconversion();

int pdfswf_numpages();
void pdfswf_convertpage(int page) ;
void pdfswf_close();
#endif //__pdf_h__
