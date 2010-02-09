from sys import * 
from pdflib_py import * 
import md5
import Image
import math

img = Image.open("baboon.png")
width, height = img.size
img.load()

p = PDF_new() 
PDF_open_file(p, "textarea3.pdf")

PDF_set_parameter(p, "usercoordinates", "true")

PDF_set_info(p, "Creator", "smalltext.py")
PDF_begin_page(p, width, height)
font = PDF_load_font(p, "Courier", "host", "")

PDF_setrgbcolor_fill(p, 0.0, 0.0, 0.0)
PDF_moveto(p, 0, 0)
PDF_lineto(p, width, 0)
PDF_lineto(p, width, height)
PDF_lineto(p, 0, height)
PDF_lineto(p, 0, 0)
PDF_fill(p)

PDF_setfont(p, font, 4.0)
i = 0
for y in range(height / 3):
    text = "".join([md5.md5(str(i+j*732849)).hexdigest() for j in range(9)])
    for x in range(width / 3):
        r,g,b = img.getpixel((x*3, height-1-y*3))
        l = math.sqrt(r*r+g*g+b*b)
        if not r and not g and not b:
            continue
        white = l / 444.0
        PDF_setfont(p, font, 0.5+white*7)
        PDF_setrgbcolor_fill(p, r/l, g/l, b/l)
        PDF_set_text_pos(p, x*3, y*3);
        PDF_show(p, text[x])
    i = i + 1

PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
