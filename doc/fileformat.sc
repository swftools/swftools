# ==================================================================================
#
# swfc(1) File format specification (well, sort of)
# Version: 0.1
#
# ==================================================================================

# Notice that the following is actually a valid .sc file, generating the four swfs
# spec.flash, scroll1.flash, scroll2.flash and simple_viewer.flash. (Provided all included 
# files are present)

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

# load a simple object:
# ---------------------

.swf myshape1 "choo1.swf"
.swf myshape2 filename="choo1.swf" # long form
.swf myshape3 filename=choo1.swf # long form, without quotes
.swf myshape4 choo1.swf # short form, without quotes

#TODO: Do those files come from the current directory, the script directory or both?

# load an image:
# --------------

.jpeg background "panorama.jpg"
.jpeg background2 "panorama.jpg" quality=30% #reduces jpeg quality

.png background3 "choochoo.png"

# (you can put this directly (generating a shape), or insert it into a shape via the fill= argument

# define a texture from an image:
# -------------------------------

.texture text1=background scale=50% rotate=45

# define a gradient
# -----------------

#.gradient mygradient:
#    0.000 0 0 0
#    0.500 255 255 255
#    1.000 0 0 0
#.end

# load several pages:
# -------------------

.movie train1 "choochoopuff.swf"

# Notice: An embedded animation can't have a different framerate than
# the frame rate from the surrounding animation.

# Create simple objects:
# ----------------------

# We should support some primitives to not have to load everything
# from a file
.box mybox1 width=100 height=50 color=black fill=none #unfilled rectangle (just the lines)
.box mybox2 width=100 height=50 color=black	      #same as above
.box mybox3 width=100 height=50 color=black fill      #filled black rectangle
.box mybox4 width=100 height=50 color=red line=2 fill=blue #blue rectangle with red edges
.box mybox5 width=100 height=50 color=red line=2 fill=background #part the background image, with red edges
.circle mycircle1 r=50 color=#aa33ff fill=background
.egon poly1 vertices=5

# Continuous soundtrack synched up to several frames in a row:
# ------------------------------------------------------------

# load a sound effect:
.sound sound1 "LocoStart.wav"

.frame 16
    .play sound1 loop=1 #other arguments supported: nomultiple (don't start if already playing)

.frame 20
# this starts a synchronized soundtrack at this frame position. The command
# expects a filename, not a sound object, as it would be duplicating the
# sound object otherwise, and I'd rather have that done explicitly.
    .soundtrack "LocoStart.wav"
	 #...  TODO: should .soundtrack sounds have a name? we could support
	     # .stop for them, which would simply stop generating mp3 data

.frame 55
# unlike characters, sounds can't have multiple instances, and can't
# be referenced once playing. So we stop the sound, not the instance.
    .stop sound1

# Sound effects on user pressing or releasing a button 

.button playbutton shape=obj1
#    ...
.end
.buttonsounds playbutton press=sound1 release=sound2 enter=sound3 leave=sound4

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
    .qchange obj1 x=300 y=200 # make obj1 move towards 300,200...
.frame 210
    .qchange obj1 x=300 y=300 # ... and then smoothly over to 300,300 using bspline interpolation

#.frame 220   #TODO
#    .jump obj1 x=reset y=reset

.frame 230
    .del obj1	#Remove object again

# morphing (easy for simple polygons, hard for more complex shapes) 
# -----------------------------------------------------------------

.morphshape morphthing start="start.fig" end="end.fig"

#.morphshape blop [V77CJQAvn3KKYD8GDb4gFgbAZ4oM2RBdhOtvEVRdpNdIJF4tQ5gh7GzxnFYNvxe7lpXVranU
# 	          +v/OjhGkCoUrOq9gUj4bJj0JjioYaA+VDsZ4Wq0+XLm0MiIgJZICQbADnUpdro5N11eVS699O]

# the following would be more work, but may be nice:
.box mybox6 width=100 height=50 color=black
.box mybox7 width=100 height=50 color=red
.morphshape morphthing2 start=mybox1 end=mybox2

# interpolation between crude manual animation frames
# ---------------------------------------------------

.swf ball "ball.fig" #The ball should be white or violet to make the cxforms below work

# now draw two balls, which fly around each other.

.frame 300
.put myball1=ball x=0 y=100 blue=0
.put myball2=ball x=100 y=100 red=0

.frame 310
# the .qchange means, "interpolate to this position and frame, using quadratic splines".
         .qchange myball1 x=50 y=0
         .qchange myball2 x=50 y=100

.frame 320
# We could also use coordinates relative to start position: .change myball1 x+=100 y+=0
         .qchange myball1 x=100 y=100
         .qchange myball2 x=0 y=100

.frame 330
         .qchange myball1 x=50 y=100
         .qchange myball2 x=50 y=0
 
.frame 340
# Hmmm. This frame is the same as frame 0. How to prevent duplicating that one?
 	 .qchange myball1 x=0 y=100
 	 .qchange myball2 x=100 y=100

# fading into existence/out of existence (colour xform):
# ------------------------------------------------------

# .put, .change, .qchange, .jump all support params which result in color transform.
# Everything up to the + is multiplicated, the rest summed.
# TODO: negative values should also be allowed.

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

# TODO: we still don't have full control over the 2x3 object-placement matrix.
#       (As we only have one parameter for the two values on the primary diagonal) ActionScript
#       has the same problem, however.

# sprites
# -------

# synonym: submovie

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
.movie anim1 "penguins.swf"

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

# create at minimum a Play button and a Stop button, maybe a Rewind also 
.png playlogo "Play.png"
.png stoplogo "Stop.png"

.button myplaybutton shape=playlogo
#everything till the .end is actionscript.
    press:
	play "spr1"
.end
.button mystopbutton shape=stoplogo
    press:
	stop "spr1"
.end

.put myplaybutton x=100 y=100
.put mystopbutton x=200 y=100

# The following examples drop the .end command. This is only possible
# if we keep the leading dot (.) in the command names. (Due to naming
# conflicts with Actionscript)

# button which is blue, get's green when the mouse hovers over it, and
# white when it is pressed:

.button b1 shape=blueshape over=greenshape press=whiteshape

# button where the button position is not identical to the active area:

.button b2 area=activearea shape=blueshape over=greenshape press=whiteshape

# (if area is not set, it's the same as shape)

# the above put into use: pop up a window if the mouse cursor enters a certain
# area:

.button b3 area=activearea shape=emptyshape over=window1 press=window1

# button characters also support positioning and cxforming characters.
# to use this, we put the characters "virtually", and then use the
# instances in the button

.buttonput area1=playlogo x=300 y=400 rotate=0
.buttonput shape1=playlogo x=400 y=400 rotate=0 red=50%
.buttonput over1=playlogo x=400 y=400 rotate=30 red=50%
.buttonput press1=playlogo x=500 y=500 rotate=180
.button mybutton1 area=area1 shape=shape1 over=over1 press=press1
     #[...actionscript...]
.end

# fonts
# -----

#only supported via other tools (lib/examples/makefonts) at the moment.

.font font1 "myfont.swf"

# text fields
# -----------

#(these are scalable vector objects just like shapes)
.text mytext1 "I am a happy little scroll message" color=blue size=100% font=font1
#TODO: clipping

# edittext
# --------

#(field where the flash animation can get input from the user)

.font helvetica "helvetica-bold.swf"
.edittext edittext1 text="Type something here" color=blue maxlength=200 font=helvetica size=24 width=320 height=200
# width/height: dimensions of the box
# text: The text which appears initally in the box, and can be overwritten by the user
# maxlength: max. length of the text the user can type
# color/font/size: Font appeareance
# variable: Name of the actionscript variable the text is stored in
# The following boolean options are also supported:
# password (show input only as stars), wordwrap, multiline, html, noselect, readonly

# actionscript
# ------------

#simplified:

.action
    stop spr1
    setproperty spr1._visible 1  #TODO: spr1._visible=1?
.end

#full:

.action opcodes
    settarget "spr1"
    stop
    settarget ""
    push "spr1"
    push _visible
    push 1
    setproperty
    end
.end

#simplified:

.button playbutton2 shape=obj1 opcodes
    press:   play spr1
    release: stop spr1
.end

#full:

.button playbutton3 shape=obj1 opcodes
    press:
	settarget "spr1"
	play
	settarget ""
	end
    # [...]
.end

# todo: mx initaction, placeobject2 initclip

# depth control
# -------------------

# .put, .change, .qchange, .jump, .startclip take before and after parameters,
# which can be used to specify the depth of an object

.put o1=obj1
.put o2=obj1 below=o1 x=50
.frame
.change o1
.change o2 above=o1 x=50

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

# ==================================================================================
#
# Ok, now for some actual examples
#
# ==================================================================================

# scroll1.sc
#
# This example scrolls a single line of text from the bottom to the top, with
# a background image
#

.flash bbox=200x200 fps=25 version=6 name="scroll1.swf"

.jpeg background "panorama.jpg"

.font helvetica-bold "helvetica-bold.swf"
.text title1 "I am a happy little scroll message" color=blue font=helvetica-bold size=100%

.put background x=0 y=0

.frame 1
.put obj1=title1 x=0 y=200     # As text box positions are specified at the upper left corner, this
                          # places the text just below the bottom.
# (.put title1 is actually .put title1=title1 (i.e. the instance has the same name as the character)

.frame 200
    #
    # "change" moves the object to the new position, _interpolating_ in the frames between.
    # Do we also need a command for abruptly changing position (omitting interpolation)?
    #
.change obj1 x=0 y-=12     # Now move the text up until it disappears at the top, in 40 frames

# to scroll horizontally, we would have modified the x instead of the y coordinate, and scrolled to -200
# (assuming the text is < 200 pixels)

.end

# ==================================================================================

# scroll2.sc
#
# This example now also fades the text in and out again
#

.flash bbox=200x200 fps=25 version=6 name="scroll2.swf"
.jpeg background "panorama.jpg"

#create some text for a title and credits 

.font helvetica "helvetica-bold.swf"
.text title1 "ChooChoo Train" color=blue font=helvetica size=100%

# move from bottom to top. Fade it in on the way,
# then fade it out again.
# TODO: the following looks like fading only if the background is black. Add alpha channel support.
.put obj1=title1 x=0 y=200 luminance=0%
.frame 100
    .change obj1 luminance=100%
.frame 200
    .change obj1 y-=12 luminance=0%

.end

# simpleviewer.sc
#
# Create the simple_viewer.flash file used in SWFTools.

.flash bbox=1024x768 version=4 fps=25 name="simple_viewer.swf"
.frame 1
.swf leftshape "left.fig"
.swf rightshape "right.fig"

.sprite viewport
.end

.action
    stop viewport
    setvariable "/subtitle" ""
.end

.buttonput left=leftshape luminance=50%
.buttonput lefthover=leftshape luminance=75%
.buttonput leftpress=leftshape luminance=100%
.button leftbutton shape=left over=lefthover press=leftpress
    release: 
	previousframe viewport
.end

.buttonput right=rightshape x=20 luminance=50%
.buttonput righthover=rightshape x=20 luminance=75%
.buttonput rightpress=rightshape x=20 luminance=100%
.button rightbutton shape=right over=righthover press=rightpress
    release: 
	nextframe viewport
.end

.font arial "arial.swf"
.edittext subtitlebox width=826 height=160 color=black variable="/:subtitle" font=arial size=24

.put viewport
.put leftbutton
.put rightbutton
.put subtitlebox x=40

.end

# boxlayers.sc
# test "above"

.flash bbox=400x300 version=4 fps=50 name="boxLayers.swf"

.box box1 width=100 height=100 color=white fill
.box box2 width=100 height=100 color=white fill

.frame 0
     .put box1 x=30 y=10 blue=0 red=0 luminance=50
     .put box2 x=60 y=40 green=0 red=0 luminance=50
.frame 100
     .jump box1 above=box2
.frame 200
     .jump box2 above=box1
.end

# ---------------------- everything below this line should actually be working ----------------------


# boxes.sc
#
# Do things with rectangles.

.flash bbox=autocrop version=6 fps=50 name="boxes.swf" compress

    .box box1 width=100 height=100 color=red fill #filled red box
    .box box2 width=100 height=100 color=white    #white hollow frame
    .box box3 width=100 height=100 color=green line=10 fill=blue #filled blue box, with thick green border
    
.frame 0  
        .put box1 x=30 scalex=30
        .put box2 x=30 scalex=30
        .put box3 x=30 scalex=30
.frame 100
        .change box1 x+=300 scalex=100
        .change box2 y+=300 scalex=100
        .change box3 x+=300 y-=300 scalex=100
.frame 150
        .del box1
        .del box2
        .del box3
.end

# monkey.sc
#

.flash bbox=autocrop version=4 fps=50 name="monkey.swf"

    .swf island "island.swf"
    .swf monkey "monkey4.swf"
    .put island
    .point leftfoot 45 177
    .point rightfoot 138 177
    .frame 0 .put monkey 160 160
    .frame 20 .change monkey 160 160
    .frame 30 .change monkey 160 122
    .frame 35 .change monkey 160 112
    .frame 40 .change monkey 160 122
    .frame 50 .change monkey 160 160
    
    .frame 70 .change monkey 205 337 pin=leftfoot rotate=0 
    .frame 90 .change monkey rotate=30
    .frame 100 .change monkey rotate=30
    .frame 120 .change monkey rotate=0
    .frame 122 .change monkey 298 337 pin=rightfoot rotate=0
    .frame 140 .change monkey rotate=-30
    .frame 150 .change monkey rotate=-30
    .frame 170 .change monkey rotate=0
.end

# cxform.sc
#
# Try color transforms

.flash name="cxform.swf" version=5
    
    .swf s1 "photo.swf"

    .put s1 x=50 y=50 scalex=110 scaley=110
    .frame 100
    .change s1 x=0 y=0 scalex=210 scaley=210 red=-1+255 green=-1+255 blue=-1+255 #invert
    .frame 200
    .change s1 x=100 y=50 scalex=110 scaley=110 red=0 green=+0 blue=+0 #remove red
    .frame 300
    .change s1 x=0 y=0 scalex=210 scaley=210 red=+0 green=2 blue=-1+255 #amplify green, invert blue
    .frame 400
    .change s1 x=50 y=100 scalex=110 scaley=110 red=2-128 green=-2+255 blue=+0.7+40 #alien glow
    .frame 500
    .change s1 x=0 y=0 scalex=210 scaley=210 red=8-1024 green=8-1024 blue=8-1024 #palette reduce
    .frame 600
    .change s1 x=0 y=0 scalex=210 scaley=210 red=+0 green=+0 blue=+0 #back to normal
    .frame 700
    .change s1 x=105 y=105 scalex=0 scaley=0 luminance=0 #fadeout
.end

# transparency.sc
#
# Test transparency

.flash bbox=200x200 version=5 fps=25 name="transparency.swf"

.box box1 90 90 color=yellow fill=blue line=5
.box box2 90 90 color=white fill=red line=5

.put b1=box1 75 75 rotate=0
.put b2=box1 75 75 rotate=90
.put b3=box1 75 75 rotate=180
.put b4=box1 75 75 rotate=270
.put box2 30 30 alpha=100%
.frame 300
.change b1 rotate+=360
.change b2 rotate+=360
.change b3 rotate+=360
.change b4 rotate+=360
.change box2 alpha=0%
.end


# points.sc
#
# Test points, pivots, pins

.flash version=5 fps=20 name="points.swf"

.box box 90 90 color=white fill=red line=5
.point center 45 45
.point corner1 0 0 
.point corner2 90 0 
.point corner3 90 90 
.point corner4 0 90 

.put b1a=box x=90 y=90 pin=center
.put b1b=box x=90 y=90 pin=center alpha=50%
# OR: .put b1=box x=90 y=90 pin=(45,45) rotate=0

.put b2=box x=90 y=90 pin=corner1 alpha=75% blue=+128 red=0.2
.put b3=box x=90 y=90 pin=corner2 alpha=75% blue=+128 red=0.2
.put b4=box x=90 y=90 pin=corner3 alpha=75% blue=+128 red=0.2
.put b5=box x=90 y=90 pin=corner4 alpha=75% blue=+128 red=0.2

.frame 100
.change b1a rotate-=720
.change b1b rotate+=720
.change b2 rotate+=360
.change b3 rotate-=360
.change b4 rotate+=360
.change b5 rotate-=360

.end

# clipping.sc
#
# Test clipping

.flash name="clip.swf" fps=25
    .sprite s1 # this just constructs a colorful object "s1"
        .box b1 40 40 color=red fill=violet line=10
        .box b2 40 40 color=blue fill=green line=10
        .box b3 40 40 color=yellow fill=cyan line=10
        .box b4 40 40 color=white fill=black line=10
        .put b1 0 0
        .put b2 60 0
        .put b3 60 60
        .put b4 0 60
    .end

    # the line and fill colors of the circle don't actually do anything-
    # as it's used for clipping only it's shape matters
    .circle c1 80 color=blue fill=green

    .startclip c1 x=-160 y=-160 #c1 clips...
	.put s1  # ...the sprite s1
    .end

.frame 200
    .change c1 x=160 y=160 # movement doesn't change the clipping property
    .change s1 rotate=5

.end

# fonts.sc
#
# fonts&text

.flash name="fonts.swf"
    .font courier "Courier.swf"
    .font helvetica "Helvetica.swf"
    .text abc text="abcdefghijklmnopqrstuvwxyz" font=courier size=100% color=blue
    .text abc2 text="abcdefghijklmnopqrstuvwxyz" font=helvetica size=100% color=red
    .put abc
    .put abc2 y=200
.end


# shear.sc
#
# Shearing

.flash bbox=300x200:-30 name="shear.swf" fps=25
    .font helvetica "Helvetica.swf"
    .text text text="Shear" font=helvetica size=100% color=yellow
    .put t0=text y=150 alpha=0% shear=0
    .put t1=text y=150 alpha=0% shear=1
    .put t2=text y=150 alpha=0% shear=2
    .put t3=text y=150 alpha=0% shear=3
    .put t4=text y=150 alpha=0% shear=4
    .put t5=text y=150 alpha=0% shear=5
    .put t6=text y=150 alpha=0% shear=6
    .put t7=text y=150 alpha=0% shear=7
    .put t8=text y=150 alpha=0% shear=8
    .put t9=text y=150 alpha=0% shear=9
    .frame 50
    .change t0 alpha=40% shear=0
    .change t1 alpha=40% shear=0
    .change t2 alpha=40% shear=0
    .change t3 alpha=40% shear=0
    .change t4 alpha=40% shear=0
    .change t5 alpha=40% shear=0
    .change t6 alpha=40% shear=0
    .change t7 alpha=40% shear=0
    .change t8 alpha=40% shear=0
    .change t9 alpha=40% shear=0
    .frame 100
    .change t0 .change t1 .change t2 .change t3 .change t4
    .change t5 .change t6 .change t7 .change t8 .change t9
    .frame 150
    .change t0 alpha=0% shear=-9
    .change t1 alpha=0% shear=-8
    .change t2 alpha=0% shear=-7
    .change t3 alpha=0% shear=-6
    .change t4 alpha=0% shear=-5
    .change t5 alpha=0% shear=-4
    .change t6 alpha=0% shear=-3
    .change t7 alpha=0% shear=-2
    .change t8 alpha=0% shear=-1
    .change t9 alpha=0% shear=-0
.end

# sound.sc
#
# Sound effects

.flash name="sound.swf" fps=25
.sound loco "LocoStart.wav"
.sound whistle "Whistle.wav"

.frame 0
    .play loco
.frame 25
    .stop loco
    .play whistle loop=20
.frame 200
    .stop whistle
.end

