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
#ifdef __cplusplus
extern "C" {
#endif

void pdfswf_addfont(char*filename);
void pdfswf_addfontdir(char*dir);
void pdfswf_addlanguagedir(char*dir);

void pdfswf_setparameter(char*name, char*value);

void pdfswf_preparepage(int page);

typedef struct _pdf_doc
{
    char*info;
    char*title;
    int num_pages;
    void*internal;
} pdf_doc_t;

pdf_doc_t*  pdf_init(char*filename, char*userPassword);
void pdf_destroy(pdf_doc_t*doc);

typedef struct _swf_output
{
    int num_pages;
    int x1,y1,x2,y2;
    void*internal;
} swf_output_t;

swf_output_t* swf_output_init();
void swf_output_setparameter(swf_output_t*, char*name, char*value);
int swf_output_save(swf_output_t*, char*filename);

typedef struct _pdf_page
{
    pdf_doc_t*parent;
    int nr;
    void*internal;
} pdf_page_t;

pdf_page_t* pdf_getpage(pdf_doc_t*doc, int page);
void pdf_page_render(pdf_page_t*page, swf_output_t*output);
void pdf_page_rendersection(pdf_page_t*page, swf_output_t*output, int x, int y, int x1, int y1, int x2, int y2);
void pdf_page_destroy(pdf_page_t*page);
void swf_output_destroy(swf_output_t*page);

typedef struct _pdf_page_info
{
    int xMin, yMin, xMax, yMax;
    int number_of_images;
    int number_of_links;
    int number_of_fonts;
} pdf_page_info_t;

pdf_page_info_t* pdf_page_getinfo(pdf_page_t*page);
void pdf_page_info_destroy(pdf_page_info_t*info);

#ifdef __cplusplus
}
#endif

#endif //__pdf_h__
