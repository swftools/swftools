from sys import * 
from pdflib_py import * 
p = PDF_new() 
PDF_open_file(p, "links.pdf")

PDF_set_parameter(p, "usercoordinates", "true")
PDF_set_info(p, "Creator", "links.py")

width=200
height=200

PDF_begin_page(p, width, height)
def draw_link(x1,y1,x2,y2,url):
    action = PDF_create_action(p, "URI", "url="+url);
    optlist = "action={activate "+str(action)+"} linewidth=5"
    PDF_create_annotation(p, x1,y1,x2,y2, "Link", optlist);
draw_link(0, 0, 100, 100, "http://www.swftools.org")
draw_link(0, 100, 100, 200, "http://www.quiss.org")
PDF_end_page(p)

PDF_begin_page(p, width, height)
draw_link(0, 0, 100, 100, "http://www.pdf2swf.org")
PDF_end_page(p)

PDF_close(p)
PDF_delete(p);
