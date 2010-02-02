from sys import * 
from pdflib_py import * 
import random

p = PDF_new() 
PDF_open_file(p, "layers2.pdf")

PDF_set_parameter(p, "usercoordinates", "true")
PDF_set_info(p, "Creator", "layer2.py")

width=400
height=400

PDF_begin_page(p, width, height)

PDF_setrgbcolor_fill(p, 0, 0, 1.0)
PDF_moveto(p, 10, 10)
PDF_lineto(p, 100, 10)
PDF_lineto(p, 10, 100)

PDF_moveto(p, 20, 110)
PDF_lineto(p, 110, 110)
PDF_lineto(p, 110, 20)
PDF_fill(p)

for i in range(100):
    x = random.randint(10,110)
    y = random.randint(200,300)
    PDF_moveto(p, x, y)
    PDF_lineto(p, x+5, y)
    PDF_stroke(p)

PDF_setrgbcolor_fill(p, 0.5, 0.5, 1.0)
x,y = 100,100
def rect(x,y):
    PDF_moveto(p, x,y)
    PDF_lineto(p, x+50,y)
    PDF_lineto(p, x+50,y+50)
    PDF_lineto(p, x,y+50)
    PDF_lineto(p, x,y)
    PDF_fill(p)

rect(200,100)
rect(250,170)
rect(300,240)

PDF_setrgbcolor_fill(p, 0, 1.0, 0)
rect(200,300)

x,y = 230,330
PDF_moveto(p, x+25,y)
PDF_lineto(p, x+50,y)
PDF_lineto(p, x+50,y+50)
PDF_lineto(p, x,y+50)
PDF_lineto(p, x,y+25)
PDF_lineto(p, x+25,y+25)
PDF_lineto(p, x+25,y)
PDF_fill(p)


PDF_end_page(p)

PDF_close(p)
PDF_delete(p);
