from sys import * 
from pdflib_py import * 
p = PDF_new() 
PDF_open_file(p, "clip.pdf")

PDF_set_parameter(p, "usercoordinates", "true")
PDF_set_value(p, "compress", 0)

width = 800
height = 800
PDF_begin_page(p, width, height)

PDF_setcolor(p, "fill", "rgb", 0.0, 0.0, 0.0, 0.0)
x1,y1,x2,y2 = 100,100,300,300
PDF_moveto(p, x1,y1)
PDF_lineto(p, x2,y1)
PDF_lineto(p, x2,y2)
PDF_lineto(p, x1,y2)
PDF_lineto(p, x1,y1)
x1,y1,x2,y2 = 200,200,400,400
PDF_moveto(p, x1,y1)
PDF_lineto(p, x2,y1)
PDF_lineto(p, x2,y2)
PDF_lineto(p, x1,y2)
PDF_lineto(p, x1,y1)
PDF_stroke(p)

PDF_setcolor(p, "fill", "rgb", 0.0, 1.0, 0.0, 1.0)
x1,y1,x2,y2 = 100,100,300,300
PDF_moveto(p, x1,y1)
PDF_lineto(p, x2,y1)
PDF_lineto(p, x2,y2)
PDF_lineto(p, x1,y2)
PDF_lineto(p, x1,y1)
PDF_clip(p)
x1,y1,x2,y2 = 200,200,400,400
PDF_moveto(p, x1,y1)
PDF_lineto(p, x2,y1)
PDF_lineto(p, x2,y2)
PDF_lineto(p, x1,y2)
PDF_lineto(p, x1,y1)
PDF_fill(p)

PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
