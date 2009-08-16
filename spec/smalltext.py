from sys import * 
from pdflib_py import * 
p = PDF_new() 
PDF_open_file(p, "smalltext.pdf")

PDF_set_parameter(p, "usercoordinates", "true")

PDF_set_info(p, "Creator", "smalltext.py")
PDF_begin_page(p, 612, 200)
font = PDF_load_font(p, "Helvetica", "host", "")

PDF_setfont(p, font, 6.0)
PDF_set_text_pos(p, 40, 100);PDF_show(p, "'")
PDF_set_text_pos(p, 80, 100);PDF_show(p, "i")
PDF_set_text_pos(p, 120, 100);PDF_show(p, "l")
PDF_set_text_pos(p, 160, 100);PDF_show(p, "-")

PDF_setfont(p, font, 9.0)
PDF_set_text_pos(p, 40, 50);PDF_show(p, "|")
PDF_set_text_pos(p, 80, 50);PDF_show(p, "i")
PDF_set_text_pos(p, 120, 50);PDF_show(p, "l")
PDF_set_text_pos(p, 160, 50);PDF_show(p, "-")

PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
