from sys import * 
from pdflib_py import * 
import md5
import random
p = PDF_new() 
PDF_open_file(p, "fonts.pdf")

PDF_set_parameter(p, "usercoordinates", "true")

PDF_set_info(p, "Creator", "smalltext.py")
PDF_begin_page(p, 612, 600)

fonts = []
fonts += [PDF_load_font(p, "Times-Roman", "host", "")]
fonts += [PDF_load_font(p, "Times-Italic", "host", "")]
fonts += [PDF_load_font(p, "Times-Bold", "host", "")]
fonts += [PDF_load_font(p, "Times-BoldItalic", "host", "")]
fonts += [PDF_load_font(p, "Helvetica", "host", "")]
fonts += [PDF_load_font(p, "Helvetica-Oblique", "host", "")]
fonts += [PDF_load_font(p, "Helvetica-Bold", "host", "")]
fonts += [PDF_load_font(p, "Helvetica-BoldOblique", "host", "")]
fonts += [PDF_load_font(p, "Courier", "host", "")]
fonts += [PDF_load_font(p, "Courier-Oblique", "host", "")]
fonts += [PDF_load_font(p, "Courier-Bold", "host", "")]
fonts += [PDF_load_font(p, "Courier-BoldOblique", "host", "")]
#fonts += [PDF_load_font(p, "Symbol", "host", "")]
#fonts += [PDF_load_font(p, "ZapfDingbats", "host", "")]

words = list(open("/usr/share/dict/words", "rb").readlines())

i = 0
# the idea is to overflow the placetext matrix once, so that
# we have at least two different ty values
for y in range(3000 / 24):
    x = 0
    while x < 612:
        font = fonts[random.randint(0,len(fonts)-1)]
        fontsize = 10+14*random.random()
        PDF_setfont(p, font, fontsize)
        PDF_set_text_pos(p, x, y*24+(24-fontsize)/2);
        text = words[random.randint(0,len(words)-1)]
        x += PDF_stringwidth(p, text, font, fontsize)
        PDF_show(p, text)

PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
