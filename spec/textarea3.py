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
for y in range(height / 6):
    text = "".join([md5.md5(str(i+j*732849)).hexdigest() for j in range(9)])
    for x in range(width / 6):
        r,g,b = img.getpixel((x*6, height-1-y*6))
        l = math.sqrt(r*r+g*g+b*b)
        if not r and not g and not b:
            continue
        white = (l / 444.0)*5
        PDF_setfont(p, font, 0.5+int(white)*4)
        r = 0.3 + 0.3 * int((r/l)*3)
        g = 0.3 + 0.3 * int((g/l)*3)
        b = 0.3 + 0.3 * int((b/l)*3)
        PDF_setrgbcolor_fill(p, r, g, b)
        PDF_set_text_pos(p, x*6, y*6);
        PDF_show(p, text[x])
    i = i + 1

PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
