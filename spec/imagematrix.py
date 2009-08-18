from sys import * 
from pdflib_py import * 
from math import sin,cos
p = PDF_new() 
PDF_open_file(p, "imagematrix.pdf")

PDF_set_parameter(p, "usercoordinates", "true")

width = 612
height = 600
PDF_begin_page(p, width, height)

a=0.7
b=-0.7
matrices = [[1,0,0,1,100,200],
            [cos(a),sin(a),-sin(a),cos(a),400,75],
            [1,0,0,-1,100,350],
            [-1,0,0,1,450,370],
            [1.9,0.5,0.6,1.4,50,-200],
            [cos(b),sin(b),sin(b),-cos(b),500,630],
            [1.0,0,0,3,-90,-200],
           ]

i = PDF_load_image(p, "png", "karo.png", "")

for m in matrices:
    PDF_save(p)
    PDF_setmatrix(p, m[0],m[1],m[2],m[3],m[4],m[5])

    x,y = 100,100
    #PDF_fit_image(p, i, x, y, "")
    PDF_place_image(p, i, x, y, 100/4)
    w,h = 100,100

    PDF_setrgbcolor_fill(p, 0.0, 1.0, 1.0)
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
