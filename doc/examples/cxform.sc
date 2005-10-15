# cxform.sc
#
# Try color transforms

.flash name="cxform.swf" version=5 fps=50
    
    .png s1 "lena.png"

    .put s1 x=50 y=50 scalex=110 scaley=110
    .frame 50
    .change s1 x=0 y=0 scalex=210 scaley=210 red=-1+255 green=-1+255 blue=-1+255 #invert
    .frame 100
    .change s1 x=100 y=50 scalex=110 scaley=110 red=0 green=+0 blue=+0 #remove red
    .frame 150
    .change s1 x=0 y=0 scalex=210 scaley=210 red=+0 green=2 blue=-1+255 #amplify green, invert blue
    .frame 200
    .change s1 x=50 y=100 scalex=110 scaley=110 red=2-128 green=-2+255 blue=+0.7+40 #alien glow
    .frame 250
    .change s1 x=0 y=0 scalex=210 scaley=210 red=8-1024 green=8-1024 blue=8-1024 #palette reduce
    .frame 300
    .change s1 x=0 y=0 scalex=210 scaley=210 red=+0 green=+0 blue=+0 #back to normal
    .frame 350
    .change s1 x=105 y=105 scalex=0 scaley=0 luminance=0 #fadeout
.end
