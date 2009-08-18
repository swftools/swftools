from sys import * 
from pdflib_py import * 
from math import sin,cos
p = PDF_new() 
PDF_open_file(p, "textposition.pdf")

PDF_set_parameter(p, "usercoordinates", "true")

width = 612
height = 500
PDF_begin_page(p, width, height)

font = PDF_load_font(p, "Helvetica-Bold", "host", "")

PDF_setfont(p, font, 18.0)

a=0.7
b=-0.7
matrices = [[1,0,0,1,100,200],
            [cos(a),sin(a),-sin(a),cos(a),400,75],
            [1,0,0,-1,100,350],
            [-1,0,0,1,450,270],
            [1.9,0.5,0.6,1.4,50,-140],
            [cos(b),sin(b),sin(b),-cos(b),100,300],
            [1.0,0,0,5,-90,-200],
           ]

for m in matrices:
    PDF_save(p)
    PDF_setmatrix(p, m[0],m[1],m[2],m[3],m[4],m[5])
    x,y = 100,100
    PDF_set_text_pos(p, x,y)
    w = PDF_stringwidth(p, "HELLO WORLD", font, 18.0)
    h = 18.0 - 4
    PDF_setrgbcolor_fill(p, 0.0, 0.0, 0.0)
    PDF_show(p, "HELLO WORLD")

    PDF_setrgbcolor_fill(p, 0.0, 0.0, 1.0)
    PDF_moveto(p, x,y)
    PDF_lineto(p, x+w,y)
    PDF_lineto(p, x+w,y+h)
    PDF_lineto(p, x,y+h)
    PDF_lineto(p, x,y)
    PDF_moveto(p, x-20,y-20)
    PDF_lineto(p, x-20,y+20+h)
    PDF_lineto(p, x+20+w,y+20+h)
    PDF_lineto(p, x+20+w,y-20)
    PDF_lineto(p, x-20,y-20)
    PDF_fill(p);
    PDF_restore(p)

PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
