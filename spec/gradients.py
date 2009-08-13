from sys import * 
from pdflib_py import * 
p = PDF_new() 
PDF_open_file(p, "gradients.pdf")

PDF_set_parameter(p, "usercoordinates", "true")
PDF_set_value(p, "compress", 0)

PDF_set_info(p, "Author", "pdflib")
PDF_set_info(p, "Creator", "pdflib_py")
PDF_set_info(p, "Title", "gradients")

width = 1024
height = 800
PDF_begin_page(p, width, height)

type,x,params = "radial",0,"r0=0 r1=320"
y = 0
PDF_setcolor(p, "fill", "rgb", 0.0, 0.0, 0.0, 1.0)
shading = PDF_shading(p, type, 160+x,160+y, 160+x, 160+y, 1.0, 1.0, 1.0, 1.0, params) #axial|radial
pattern = PDF_shading_pattern(p,shading,"")
PDF_setcolor(p, "fill", "pattern", pattern,0,0,0)
PDF_moveto(p, x,y)
PDF_curveto(p, x+80, y+80, x+80, y+240, x, y+320)
PDF_curveto(p, x+80, y+240, x+240, y+240, x+320, y+320)
PDF_curveto(p, x+240, y+240, x+240, y+80, x+320, y)
PDF_curveto(p, x+240, y+80, x+80, y+80, x, y)
PDF_fill(p)
PDF_moveto(p, x,y)
PDF_curveto(p, x+80, y+80, x+80, y+240, x, y+320)
PDF_curveto(p, x+80, y+240, x+240, y+240, x+320, y+320)
PDF_curveto(p, x+240, y+240, x+240, y+80, x+320, y)
PDF_curveto(p, x+240, y+80, x+80, y+80, x, y)
PDF_stroke(p)

type,x,params = "axial",200,""
y = 0
PDF_setcolor(p, "fill", "rgb", 0.0, 0.0, 0.4, 1.0)
shading = PDF_shading(p, type, 0+x,0+y, 320+x,320+y, 1.0, 1.0, 1.0, 1.0, params) #axial|radial
pattern = PDF_shading_pattern(p,shading,"")
PDF_setcolor(p, "fill", "pattern", pattern,0,0,0)
PDF_moveto(p, x,y)
PDF_curveto(p, x+80, y+80, x+80, y+240, x, y+320)
PDF_curveto(p, x+80, y+240, x+240, y+240, x+320, y+320)
PDF_curveto(p, x+240, y+240, x+240, y+80, x+320, y)
PDF_curveto(p, x+240, y+80, x+80, y+80, x, y)
PDF_fill(p)
PDF_moveto(p, x,y)
PDF_curveto(p, x+80, y+80, x+80, y+240, x, y+320)
PDF_curveto(p, x+80, y+240, x+240, y+240, x+320, y+320)
PDF_curveto(p, x+240, y+240, x+240, y+80, x+320, y)
PDF_curveto(p, x+240, y+80, x+80, y+80, x, y)
PDF_stroke(p)

type,x,params = "radial",500,"r0=0 r1=220"
y = 0
PDF_setcolor(p, "fill", "rgb", 0.0, 0.0, 0.4, 1.0)
shading = PDF_shading(p, type, 120+x, 340+y, 120+x, 340+y, 1.0, 1.0, 1.0, 1.0, params) #axial|radial
pattern = PDF_shading_pattern(p,shading,"")
PDF_setcolor(p, "fill", "pattern", pattern,0,0,0)
PDF_moveto(p, x+80, y+80)
PDF_lineto(p, x+80, y+640)
PDF_lineto(p, x+160, y+640)
PDF_lineto(p, x+160, y+80)
PDF_lineto(p, x+80, y+80)
PDF_fill(p)
PDF_moveto(p, x+80, y+80)
PDF_lineto(p, x+80, y+640)
PDF_lineto(p, x+160, y+640)
PDF_lineto(p, x+160, y+80)
PDF_lineto(p, x+80, y+80)
PDF_stroke(p)

type,x,params = "axial",600,""
y = 0
PDF_setcolor(p, "fill", "rgb", 0.0, 0.0, 0.4, 1.0)
shading = PDF_shading(p, type, 80+x, 80+y, 80+x, 640+y, 1.0, 1.0, 1.0, 1.0, params) #axial|radial
pattern = PDF_shading_pattern(p,shading,"")
PDF_setcolor(p, "fill", "pattern", pattern,0,0,0)
PDF_moveto(p, x+80, y+80)
PDF_lineto(p, x+80, y+640)
PDF_lineto(p, x+160, y+640)
PDF_lineto(p, x+160, y+80)
PDF_lineto(p, x+80, y+80)
PDF_fill(p)
PDF_moveto(p, x+80, y+80)
PDF_lineto(p, x+80, y+640)
PDF_lineto(p, x+160, y+640)
PDF_lineto(p, x+160, y+80)
PDF_lineto(p, x+80, y+80)
PDF_stroke(p)

type,x,params = "axial",50,""
y = 300
PDF_setcolor(p, "fill", "rgb", 0.0, 0.0, 0.4, 1.0)
shading = PDF_shading(p, type, 80+x, 80+y, 400+x, 80+y, 1.0, 1.0, 1.0, 1.0, params) #axial|radial
pattern = PDF_shading_pattern(p,shading,"")
PDF_setcolor(p, "fill", "pattern", pattern,0,0,0)
PDF_moveto(p, x+80, y+80)
PDF_lineto(p, x+80, y+160)
PDF_lineto(p, x+400, y+160)
PDF_lineto(p, x+400, y+80)
PDF_lineto(p, x+80, y+80)
PDF_fill(p)
PDF_moveto(p, x+80, y+80)
PDF_lineto(p, x+80, y+160)
PDF_lineto(p, x+400, y+160)
PDF_lineto(p, x+400, y+80)
PDF_lineto(p, x+80, y+80)
PDF_stroke(p)

PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
