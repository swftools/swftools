#!/usr/bin/python
import sys
sys.path += ["../scripts/"]
import pdf

# a test for transparency groups:
# form xobjects used for doing transparency groups can do savestate (q)
# without ever needing to do a corresponding restorestate (Q) because
# their content stream is self-contained.
#
# Test that this doesn't confuse the pdf reader.
    
file = pdf.PDF()
    
page = file.add_page(612,100)

group1 = file.create_object("/XObject", "/Form")
group1.stream = """
0.0 1.0 0.0 rg
0.0 0.0 0.0 RG
10 10 m 70 10 l 70 70 l 10 70 l 10 10 l f
10 10 m 70 10 l 70 70 l 10 70 l 10 10 l s
0.0 0.0 1.0 rg
0.0 0.0 0.0 RG
30 30 m 90 30 l 90 90 l 30 90 l 30 30 l f
30 30 m 90 30 l 90 90 l 30 90 l 30 30 l s
1.0 0 0 1.0 1000 1000 cm q
1.0 0 0 1.0 1000 1000 cm q
1.0 0 0 1.0 1000 1000 cm q
1.0 0 0 1.0 1000 1000 cm q
""" 
isolated = "true"
knockout = "true"
group1["/Group"] = pdf.PDFDict({"/S": "/Transparency", "/CS": "/DeviceRGB", "/I": isolated, "/K": knockout})
group1["/BBox"] = pdf.PDFArray([0, 0, 100, 100])

gs = file.create_object("/ExtGState")
gs["/BM"] = "/Normal"
gs["/CA"] = "1.0" # stroke alpha
gs["/ca"] = "1.0" # fill alpha

resources = file.create_object("/Resources")
resources["/XObject"] = pdf.PDFDict({"/mygroup": group1})
resources["/ExtGState"] = pdf.PDFDict({"/gs0": gs})

page.header["/Resources"] = resources

page.stream = """q

1.0 0.0 0.0 rg

0 40 m 612 40 l 612 60 l 0 60 l 0 40 l f
q /gs0 gs 1.0 0 0 1.0 0   0 cm /mygroup Do Q

Q"""

file.write("transpstack.pdf")

