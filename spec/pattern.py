from sys import * 
from pdflib_py import * 
p = PDF_new() 
PDF_open_file(p, "pattern.pdf")

PDF_set_parameter(p, "usercoordinates", "true")
PDF_set_info(p, "Creator", "pattern.py")

width=595
height=842

PDF_begin_page(p, width, height)

PDF_setrgbcolor_stroke(p, 0, 0, 0.9)
PDF_setrgbcolor_fill(p, 0.7, 0.7, 0.0)
pattern = PDF_begin_pattern(p, 4, 4, 4, 4, 2)
PDF_moveto(p, 0, 0)
PDF_lineto(p, 4, 4)
PDF_moveto(p, 4, 0)
PDF_lineto(p, 0, 4)
PDF_stroke(p)
PDF_moveto(p, 0, 2)
PDF_lineto(p, 0, 4)
PDF_lineto(p, 4, 4)
PDF_lineto(p, 4, 2)
PDF_lineto(p, 0, 2)
PDF_fill(p)
PDF_end_pattern(p)

PDF_setcolor(p, "fill", "pattern", pattern, 0.0, 0.9, 0.0)
PDF_moveto(p, 0, 0)
PDF_lineto(p, width-100, 0)
PDF_lineto(p, width-100, height)
PDF_lineto(p, 0, height)
PDF_lineto(p, 0, 0)
PDF_fill(p)

PDF_end_page(p)

PDF_close(p)
PDF_delete(p);
