from sys import * 
from pdflib_py import * 
p = PDF_new() 
PDF_open_file(p, "textselectspaces.pdf")

PDF_set_parameter(p, "usercoordinates", "true")

PDF_set_info(p, "Creator", "smalltext.py")
PDF_begin_page(p, 612, 200)
font = PDF_load_font(p, "Helvetica", "host", "")

PDF_setfont(p, font, 18.0)

x = 40
y = 100
def write_word(s, d):
    global x,y
    w = PDF_stringwidth(p, s, font, 18.0)
    PDF_show(p, s)
    x+=w+d
    PDF_set_text_pos(p, x, y)

x,y = 40,160
PDF_set_text_pos(p, x, y)
write_word("The", 20)
write_word("quick", 20)
write_word("brown", 20)
write_word("fox", 20)

x,y = 40,130
PDF_set_text_pos(p, x, y)
write_word("The", 10)
write_word("quick", 10)
write_word("brown", 10)
write_word("fox", 10)

x,y = 40,100
PDF_set_text_pos(p, x, y)
write_word("The", 1)
write_word("quick", 1)
write_word("brown", 1)
write_word("fox", 1)

PDF_setmatrix(p, 2.5, 0, 0, 1.0, 0, 0)
x,y = 16,70
PDF_set_text_pos(p, x, y)
write_word("The", 0.5)
write_word("quick", 0.5)
write_word("brown", 0.5)
write_word("fox", 0.5)

PDF_setmatrix(p, 2.5, 0, 0, 1.0, 0, 0)
x,y = 16,40
PDF_set_text_pos(p, x, y)
write_word("The", 5)
write_word("quick", 5)
write_word("brown", 5)
write_word("fox", 5)


PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
