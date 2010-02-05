import sys
from pdflib_py import * 
from math import sin,cos
import Image
import ImageDraw

img = Image.new("L", (3, 3))
draw = ImageDraw.Draw(img)
draw.point((0,1), fill=255)
draw.point((0,2), fill=255)
draw.point((1,0), fill=255)
draw.point((1,2), fill=255)
draw.point((2,0), fill=255)
draw.point((2,1), fill=255)
img.save("/tmp/mask1.png")

img = Image.new("L", (16, 1))
draw = ImageDraw.Draw(img)
for i in range(16):
    draw.point((i,0), fill=i*16)
img.save("/tmp/mask2.png")

img = Image.new("RGB", (3, 3))
draw = ImageDraw.Draw(img)
draw.point((0,1), fill=(0,0,0))
draw.point((0,2), fill=(255,0,0))
draw.point((1,0), fill=(0,255,0))
draw.point((1,2), fill=(0,0,255))
draw.point((2,0), fill=(255,255,0))
draw.point((2,1), fill=(0,255,255))
img.save("/tmp/img1.png")

img = Image.new("RGB", (16, 1))
draw = ImageDraw.Draw(img)
for i in range(16):
    draw.point((i,0), fill=(0,255,0))
img.save("/tmp/img2.png")

p = PDF_new() 
PDF_open_file(p, "transparency.pdf")
font = PDF_load_font(p, "Helvetica", "host", "")

PDF_set_parameter(p, "usercoordinates", "true")

width = 400
height = 400
PDF_begin_page(p, width, height)

PDF_setcolor(p, "fill", "rgb", 0.0,0.0,0.0,1.0)
PDF_moveto(p, 0,0)
PDF_lineto(p, width, 0)
PDF_lineto(p, width, height)
PDF_lineto(p, 0, height)
PDF_lineto(p, 0, 0)
PDF_fill(p)

PDF_setfont(p, font, 10.0)
PDF_setcolor(p, "fill", "rgb", 1.0,1.0,1.0,1.0)
PDF_set_text_pos(p, 50, 205);PDF_show(p, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz")
PDF_set_text_pos(p, 50, 105);PDF_show(p, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz")

mask = PDF_load_image(p, "png", "/tmp/mask1.png", "mask")
i = PDF_load_image(p, "png", "/tmp/img1.png", "masked "+str(mask))
PDF_place_image(p, i, 100, 300, 20)

mask2 = PDF_load_image(p, "png", "/tmp/mask2.png", "mask")
i2 = PDF_load_image(p, "png", "/tmp/img2.png", "masked "+str(mask2))
PDF_place_image(p, i2, 0, 200, 25)

PDF_setcolor(p, "fill", "rgb", 1.0,1.0,1.0,1.0)
gstate = PDF_create_gstate(p, "opacityfill 0.25") # blendmode multiply opacityfill 0.5")
PDF_set_gstate(p, gstate)
PDF_moveto(p, 50, 75)
PDF_lineto(p, 50+300, 75)
PDF_lineto(p, 50+300, 150)
PDF_lineto(p, 50, 150)
PDF_lineto(p, 50, 75)
PDF_fill(p)

PDF_end_page(p)
PDF_close(p)
PDF_delete(p);
