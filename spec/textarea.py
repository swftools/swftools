from sys import * 
from pdflib_py import * 
import md5
p = PDF_new() 
PDF_open_file(p, "textarea.pdf")

PDF_set_parameter(p, "usercoordinates", "true")

PDF_set_info(p, "Creator", "smalltext.py")
PDF_begin_page(p, 612, 3000)
font = PDF_load_font(p, "Courier", "host", "")

PDF_setfont(p, font, 12.0)
i = 0
# the idea is to overflow the placetext matrix once, so that
# we have at least two different ty values
for y in range(3000 / 9):
    PDF_set_text_pos(p, 0, y*9);
    text = "".join([md5.md5(str(i+j*732849)).hexdigest() for j in range(3)])
    print text
    PDF_show(p, text)
    i = i + 1

PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
