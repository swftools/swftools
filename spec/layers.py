from sys import * 
from pdflib_py import * 

p = PDF_new() 
PDF_open_file(p, "layers.pdf")

PDF_set_parameter(p, "usercoordinates", "true")
PDF_begin_page(p, 500, 200)

font = PDF_load_font(p, "Helvetica-Bold", "host", "")
PDF_setfont(p, font, 80.0)
PDF_set_text_pos(p, 0, 10)
PDF_show(p, "Bottom Text")

PDF_setrgbcolor_stroke(p, 0, 1.0, 1.0)
PDF_setlinewidth(p, 10)
PDF_moveto(p, 0, 0)
PDF_lineto(p, 500, 100)
PDF_stroke(p)
PDF_moveto(p, 0, 100)
PDF_lineto(p, 500, 0)
PDF_stroke(p)

PDF_setrgbcolor_fill(p, 0, 0, 1.0)
PDF_moveto(p, 350, 25)
PDF_lineto(p, 490, 25)
PDF_lineto(p, 490, 75)
PDF_lineto(p, 350, 75)
PDF_lineto(p, 350, 25)
PDF_fill(p)

PDF_setrgbcolor_fill(p, 1, 1, 0)
PDF_set_text_pos(p, 400, 20);PDF_show(p, "A")
PDF_set_text_pos(p, 400, 80);PDF_show(p, "B")
PDF_set_text_pos(p, 400, 140);PDF_show(p, "C")


PDF_end_page(p)

PDF_close(p)
PDF_delete(p);
