# Moves.sc
# tutorial on types of Moves for the user.

.flash bbox=autocrop version=4 fps=25 name="MoveTutorial.swf"

# SPRITES

.sprite reticle
	.box box width=5 height=5 color=yellow
	.put b1=box 0 0
	.put b2=box 5 0
	.put b3=box 0 5
	.put b4=box 5 5
.end
#
# CHARACTERS
#
.swf axes axesClip.swf
.circle pin 8 color=blue fill=blue
#
.font helv "HelveticaBold.swf"
#
.text mesg1 text="(1) This tutorial demonstrates object motions." font=helv color=yellow size=25%
.text mesg2 text="(2) Every object can be animated in coordinate space." font=helv color=red size=25%
.text mesg3 text="(3) Let's examine each type of motion." font=helv color=magenta size=25%
#
.text mesg4 text="(4) It can translate in x..." font=helv color=green size=25%
.text mesg5 text="(5) ... it can translate in y ..." font=helv color=cyan size=25%
.text mesg6 text="(6) or both." font=helv color=yellow size=25%
.text mesg7 text="(7) It can rotate." font=helv color=magenta size=25%
#
.text mesg8 text="(8) It can scale in x ..." font=helv color=blue size=25%
.text mesg9 text="(9) ... it can scale in y ..." font=helv color=red size=25%
.text mesg10 text="(10)... or both." font=helv color=cyan size=25%
.text mesg11 text="(11) It can shear positively ..." font=helv color=green size=25%
.text mesg12 text="(12) ... or negatively." font=helv color=yellow size=25%
.text mesg13 text="(13) By combining these motions ..." font=helv color=blue size=25%
#
.text mesg14 text="(14) ... you can produce various illusions ..." font=helv color=red size=25%
.text mesg15 text="(15) ... of depth and velocity." font=helv color=magenta size=25%
# .text mesg13 text="(16) and it will rotate in place." font=helv color=red size=25%
# .text mesg13 text="(17) and it will rotate in place." font=helv color=red size=25%
# .text mesg13 text="(18) and it will rotate in place." font=helv color=red size=25%
# .text mesg13 text="(19) and it will rotate in place." font=helv color=red size=25%

.text annot1 text="Increasing X" font=helv color=yellow size=12%
.text annot2 text="Increasing Y" font=helv color=yellow size=12%

# MACROS

# MAIN

.box box1 width=100 height=100 color=red line=5     # a simple red box
    
.frame 0  
        .macro tfader 0,10 mesg1
        .put box1 x=100 y=100 pin=center
.frame n+=30
	.change box1			# release that box
.frame n+=30				# *** slide 1
	.change box1 pin=center x=120 y=120 scale=75%
.frame n+=30
	.change box1 pin=center x=150 y=150 rotate+=90 scale=100%
        .macro tfader 0,10 mesg2
.frame n+=30
	.change box1 shear=+1.5 pin=center 
.frame n+=30
        .macro tfader 0,10 mesg3
	.change box1 x=100 y=100 shear=0 pin=(0,0) 
.frame n+=60
	.macro tfader 0,10 mesg4
	.put axes x=20 y=20 scale=120%
	.put annot1 x=30 y=50
	.put annot2 x=30 y=70
	.change box1 x=100 y=100 shear=0 pin=(0,0) rotate=0
.frame n+=20
	.change box1 x+=50
.frame n+=20
	.change box1 x-=50
.frame n+=20
	.macro tfader 0,10 mesg5
.frame n+=20
	.change box1 y+=50
.frame n+=20
	.change box1 y-=50
.frame n+=20
	.macro tfader 0,10 mesg6
.frame n+=30
	.change box1 x+=100 y+=100
	.change annot1 x=80
	.change annot2 y=120
#	.change box1 x=100
.frame n+=30
	.change box1 x-=50 y-=50
	.macro tfader 0,10 mesg7		# it can rotate
	.change annot1 alpha=0
	.change annot2 alpha=0
	.change axes alpha=0
.frame n+=2
	.del annot1
	.del annot2
	.del axes
.frame n+=30
	.change box1 rotate=180
.frame n+=30
	.change box1 rotate=0
	.macro tfader 0,10 mesg8		# it can scale in x
.frame n+=30
	.change box1 scalex=50%
.frame n+=30
	.change box1 scalex=100%
	.macro tfader 0,10 mesg9		# or in y
.frame n+=30
	.change box1 scaley=50%
.frame n+=30
	.change box1 scaley=100%
	.macro tfader 0,10 mesg10		# or both
.frame n+=30
	.change box1 scale=50%
.frame n+=30
	.change box1 scale=100%
	.macro tfader 0,10 mesg11		# positive shear
.frame n+=30
	.change box1 shear=+.5
.frame n+=30
	.change box1 shear=0
	.macro tfader 0,10 mesg12		# negative shear
.frame n+=30
	.change box1 shear=-.5
.frame n+=30
	.change box1 shear=0
	.macro tfader 0,10 mesg13		# by combining
.frame n+=90
	.change box1 pin=center x=200 y+=50 rotate=360 scale=25%
.frame n+=30
	.macro tfader 0,10 mesg14		# we produce the illusion
	.change box1 pin=center x=100 y=100 rotate=0 scale=100
.frame n+=2
	.jump box1 shear=1.0 x+=4
.frame n+=18
	.change box1 x+=80 shear=0
.frame n+=20
	.change box1 x+=80 
.frame n+=5
	.change box1 shear=-.5
.frame n+=5
	.change box1 shear=0
.frame n+=2
	.change box1 y+=5
.frame n+=2
	.change box1 y-=5
.frame n+=2
	.change box1 y+=5
.frame n+=2
	.change box1 y-=5
.frame n+=60
	.change box1 x-=150 y-=10 rotate=180 scalex=-100%
.frame n+=60
	.change box1 pin=center rotate=0 scalex=100%
.frame n+=60
	.macro tfader 0,10 mesg15		# of depth and velocity
.frame n+=60
	.change box1 alpha=0

.end


