# ===========================================================================
#
# swfc(1) File format specification
#
# Warning: not all of this is actually implemented yet- if there's a
# "TODO" behind a command, it has yet to be integrated into swfc.
#
# ===========================================================================

# File header
# -----------

.flash bbox=200x200 version=6 fps=50 name="spec.swf" compress

# bbox=200x200         : set bounding box to 200x200
# bbox=200x200:100:100 : set bounding box to (100,100)-(300,300)
# bbox=autocrop        : (default) - make bbox just big enough so that nothing ever get's clipped
#
# all coordinates are in pixels, not in twips. fixed point values like 1.25, 333.40 are allowed.
#
# version=6    : generate flash mx movie
# fps=50       : set framerate to 50 frames/second
# name="..."   : save result to spec.flash. (Default: input file with extension 
#					   changed to .flash, or the value of the -o command line 
#					   parameter if given)

# inherit swf movies:
# ---------------------

.swf mymovie1 "choo1.swf"
.swf mymovie2 filename="choo1.swf" # long form
.swf mymovie3 filename=choo1.swf # long form, without quotes
.swf mymovie4 choo1.swf # short form, without quotes

# Notice: An embedded animation can't have a different framerate than
# the frame rate from the surrounding animation.

#TODO: Those files currently come from the current directory- they should come from the script directory.

# load an image:
# --------------

.jpeg background "panorama.jpg"
.jpeg background2 "panorama.jpg" quality=30% #reduces jpeg quality

.png background3 "choochoo.png" #TODO

# (you can put this directly (generating a shape), or insert it into a shape via the fill= argument

# define a texture from an image:
# -------------------------------

.texture text1=background scale=50% rotate=45

# define a gradient
# -----------------

.gradient mygradient:
    0% red
    50% green
    75% aquamarine
    100% #ffffff
.end

# with a hard edge:

.gradient mygradient:
    0% red
    50% green
    50% red
    75% aquamarine
    100% #ffffff
.end

# with alpha:

.gradient mygradient:
    0%   #00000000
    50%  #0000ff40
    75%  #0000ff80
    100% #0000ffc0
.end

# define a texture from a gradient:
# ---------------------------------

.texture grad2=mygradient x=0 y=0 width=125 height=200 rotate=45

# Create simple objects:
# ----------------------

.box mybox1 width=100 height=50 color=black fill=none #unfilled rectangle (just the lines)
.box mybox2 width=100 height=50 color=black	      #same as above
.box mybox3 width=100 height=50 color=black fill      #filled black rectangle
.box mybox4 width=100 height=50 color=red line=2 fill=blue #blue rectangle with red edges
.box mybox5 width=100 height=50 color=red line=2 fill=background #filled with image "background", with red edges

.circle mycircle1 r=50 color=#aa33ff fill=mygradient # filled with gradient (defined above)
.circle mycircle1 r=50 color=#aa33ff fill=grad2      # filled with gradient texture

# Sounds:
# -------

.sound sound1 "LocoStart.wav"

.frame 16
    .play sound1 loop=1 #other arguments supported: nomultiple (don't start if already playing)
.frame 55
    .stop sound1

.frame 66
    # this starts a synchronized soundtrack at this frame position. The command
    # expects a filename, not a sound object, as it would be duplicating the
    # sound object otherwise, and I'd rather have that done explicitly.
    .soundtrack "LocoStart.wav" # TODO

# Elements of the movie should be animated by various means 
# moving (x,y and zoom) 
# ---------------------

.box obj1 width=200.0 height=100.0

.frame 100
    .put obj1 x=0 y=0 #object starts at (0,0)

.frame 140
    .change obj1 x+=100 y=0 scale=120% # smoothly move obj1 100 pixels to the right over 40 frames,
	                               # scaling it to 120% of it's original size
.frame 170
    .jump obj1 x=200 y=200 # make object to suddenly change to 200,200 at frame 70.

.frame 190
    .qchange obj1 x=300 y=200 # TODO # make obj1 move towards 300,200...
.frame 210
    .qchange obj1 x=300 y=300 # TODO # ... and then smoothly over to 300,300 using bspline interpolation

#.frame 220   #TODO
#    .jump obj1 x=reset y=reset

.frame 230
    .del obj1	#Remove object again

# morphing
# --------

.outline shape1:
    #define shape 1
.end
.outline shape2:
    #define shape 2
.end
.morphshape morphthing start=shape1 end=shape2
.frame 250
    .put morphthing ratio=0%
.frame 270
    .change morphthing ratio=100%

# interpolation between crude manual animation frames
# ---------------------------------------------------

.circle ball r=50 color=blue fill=violet

# now draw two balls, which fly around each other.

.frame 300
.put myball1=ball x=0 y=100 blue=0
.put myball2=ball x=100 y=100 red=0

.frame 310
# the .qchange means, "interpolate to this position and frame, using quadratic splines".
         .qchange myball1 x=50 y=0 #TODO
         .qchange myball2 x=50 y=100 #TODO

.frame 320
# We could also use coordinates relative to start position: .change myball1 x+=100 y+=0
         .qchange myball1 x=100 y=100 #TODO
         .qchange myball2 x=0 y=100 #TODO

.frame 330
         .qchange myball1 x=50 y=100 #TODO
         .qchange myball2 x=50 y=0 #TODO
 
.frame 340
# Hmmm. This frame is the same as frame 0. How to prevent duplicating that one?
 	 .qchange myball1 x=0 y=100 #TODO
 	 .qchange myball2 x=100 y=100 #TODO

# color transforms (fading)
# -------------------------

# .put, .change, .qchange, .jump all support params which result in color transform.
# Everything up to the + is multiplicated, the rest summed.

.put obj1 blue=50% #(scale blue color component to 50%)
.change obj1 blue=128 #(same as above)
.change obj1 red=+25% #(shift blue color component by 25%)
.change obj1 red=+64 #(same as above)
.change obj1 green=50%+50% #(scale green color component to 50% and then shift it by 50%)
.change obj1 green=128+128 #(same as above)
.change obj1 green=-1+255 #(invert green color component)
.change obj1 green=2-128  #(reduce spectrum for green to the upper 50%)
.change obj1 luminance=50% #(fade all color components 50% to black, same as red=50% green=50% blue=50%)
.change obj1 alpha=50% #(make object 50% transparent)

# scaling
# -------

.change obj1 scale=50%   # change an object to 50% of it's size
.change obj1 scaley=50%  # flatten object to 50% of it's height
.change obj1 scaley=110% scalex=75% #(funhouse mirror!)

# rotating
# --------

.change obj1 rotate=45  # tilt object, counterclockwise

# shearing
# --------

.change obj1 shear=100%  # shear object

# sprites (a.k.a. movieClips)
# -------

.sprite spr1
    .frame 1	.put ball1=ball 0 0
    .frame 2	.change ball1 100 100
.end

.sprite spr2
    .frame 1	.put ball2=ball 100 0
    .frame 7	.change ball2 0 100
.end

.frame 400
    .put spr1
    .put spr2

# so spr1 has three frames, spr2 has seven. The whole animation loops after 21 frames.

# multi-level nesting: (sprites can be nested to any depth)

.sprite mspr1
    .sprite mspr2
	.sprite spr3
	.put ball 100 0
	.end
    .end
.end

# clipping an animation inside a star-shaped sprite:

.swf mystar1 "star.swf"
.swf anim1 "penguins.swf"

.sprite cspr1
    .startclip mystar1
	.put anim1
    .end
.end
.frame 401
    .put cspr1 rotate=0
.frame 402
    .change cspr1 rotate=360

# buttons
# -------

.png play "Play.png" #TODO
.png play_pressed "Play_pressed.png" #TODO
.png stoplogo "Stop.png" #TODO

.button playbutton
    .show play as=idle
    .show play as=area
    .show play_pressed as=pressed
    .show play_pressed as=hover
.end

.button playbutton2
    .show play as=idle
    .on_press inside
    .on_release inside
    .on_release outside
    .on_move_in pressed
    .on_move_out pressed
    .on_move_in
    .on_move_out
    .on_key "a":
	# actionscript...
    .end
.end

.button mybutton
    .show play as=idle
    .show obj1 x=300 y=400 scale=110% pivot=center 
         as=pressed
.end

.box blueshape 10 10
.box greenshape 10 10
.box whiteshape 10 10
.box activearea 10 10
.box emtpyshape 10 10
.box window1 10 10
# button which is blue, get's green when the mouse hovers over it, and
# white when it is pressed:
.button b1
    .show blueshape as=idle
    .show greenshape as=hower
    .show whiteshape as=press
.end

# button where the button position is not identical to the active area:
.button b2
    .show activearea as=area
    .show blueshape as=idle
    .show greenshape as=hower
    .show whiteshape as=press
.end

# (if area is not set, it's the same as shape)

# the above put into use: pop up a window if the mouse cursor enters a certain
# area:
.button b3
    .show activearea as=area
    .show emtpyshape as=idle
    .show window1 as=hower
.end

# fonts
# -----
.font font1 "myfont.swf"
.font font1 "myfont.ttf"
.font font1 "myfont.pfa"

# text fields
# -----------
.text mytext1 font=font1 text="I am a happy little scroll message" color=blue size=100%

# edittext
# --------
#(field where the flash animation can get input from the user)

.font helvetica "helvetica-bold.swf"
.edittext edittext1 text="Type something here" color=blue maxlength=200 font=helvetica size=24 width=320 height=200 #TODO
# width/height: dimensions of the box
# text: The text which appears initally in the box, and can be overwritten by the user
# maxlength: max. length of the text the user can type
# color/font/size: Font appeareance
# variable: Name of the actionscript variable the text is stored in
# The following boolean options are also supported:
# password (show input only as stars), wordwrap, multiline, html, noselect, readonly

# actionscript
# ------------

.action:
    _root.play();
.end

# depth control
# -------------------
# .put, .change, .qchange, .jump, .startclip take before and after parameters,
# which can be used to specify the depth of an object

.put o1=obj1 #TODO
.put o2=obj1 below=o1 x=50 #TODO
.frame
.change o1 #TODO
.change o2 above=o1 x=50 #TODO

# newline conventions
# -------------------

# All Newlines are treated as normal whitespace. Therefore, splitting
# commands over several lines, like
.put obj2=obj1 x=300
	     y=300
	     red=100%
	     green=50%
	     blue=50%
# is allowed.

# Furthermore, having more than one command in one line is also possible:

.frame 600  .change obj1 x+=700  .change obj2 x+=300
.frame 601  .change obj1 x+=500  .change obj2 x+=500
.frame 602  .change obj1 x+=300  .change obj2 x+=700

.end # end of first file

