# Pins.sc
# tutorial on Pins for the user.

.flash bbox=autocrop version=4 fps=25 name="PinsTutorial.swf"

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
.circle pin 8 color=blue fill=blue
#
.font helv "HelveticaBold.swf"
#
.text mesg1 text="(1) This tutorial explains \"pins\" . . ." font=helv color=yellow size=25%
.text mesg2 text="(2) Every object such as this simple box ..." font=helv color=magenta size=25%
.text mesg3 text="(3) ... is \"pinned\" to a screen x,y." font=helv color=cyan size=25%
#
.text mesg4 text="(4) The yellow reticle is at 100,100..." font=helv color=red size=25%
.text mesg5 text="(5) The box's \"pin\" is at (relative to the box) 0,0." font=helv color=green size=25%
.text mesg6 text="(6) When we .put the box at 100,100, its pin lands..." font=helv color=blue size=25%
.text mesg7 text="(7) ... on 100,100." font=helv color=magenta size=25%
#
.text mesg8 text="(8) Relative 0,0 is the default pin." font=helv color=yellow size=25%
.text mesg9 text="(9) If we rotate the box..." font=helv color=red size=25%
.text mesg10 text="(10)... it rotates around its default pin" font=helv color=blue size=25%
.text mesg11 text="(11) If we specify a special pin called \"center\"" font=helv color=green size=25%
.text mesg12 text="(12) the box center will be at 100,100..." font=helv color=yellow size=25%
.text mesg13 text="(13) and it will rotate in place." font=helv color=cyan size=25%
#
.text mesg14 text="(14) Interesting things will happen if we rotate..." font=helv color=magenta size=25%
.text mesg15 text="(15) ... while a .change command moves the pin!" font=helv color=green size=25%
.text mesg16 text="(16) The pin is also relevant when scaling." font=helv color=blue size=25%
.text mesg17 text="(17) Scaling takes place relative to the fixed pin." font=helv color=red size=25%
.text mesg18 text="(18) It's important to know where your pin is!" font=helv color=white size=25%
# .text mesg19 text="(19) and it will rotate in place." font=helv color=red size=25%

# MACROS

# MAIN

.box box1 width=100 height=100 color=red line=5     # a simple red box
    
.frame 0  
        .macro tfader 0,10 mesg1
        .put box1 x=100 y=100 alpha=0
.frame n+=20				# *** slide 1
	.change box1			# hold that box
.frame n+=40
	.change box1 alpha=100%		# box bright
        .macro tfader 0,10 mesg2
.frame n+=40
	.change box1 			# box hold
.frame n+=5
	.jump box1 x=120 y=120		# why doesn't this work?
.frame n+=5
	.jump box1 x=60 y=120		# why doesn't this work?
.frame n+=20
        .macro tfader 0,10 mesg3
	.jump box1 			# make the box appear suddenly and leap around a bit
.frame n+=5
	.jump box1 x=100 y=100		# why doesn't this work?
.frame n+=5
	.jump box1 x-=20
.frame n+=5
	.jump box1 x-=20 y-=20
.frame n+=5
	.jump box1 x=100 y=100		# put box bck where it belongs
.frame n+=5
	.jump box1 x-=20 y-=15
.frame n+=5
	.jump box1 x+=20 y+=40
.frame n+=5
	.jump box1 x=100 y=100
.frame n+=5
	.put pin 100 100 alpha=0 pin=center	# pin placed (dark, bottom layer)
	.put reticle 100 100 alpha=0 pin=center	# place reticle (dark, top layer)
.frame n+=20
	.macro tfader 0,10 mesg4
	.change box1 alpha=0		# box fade
	.change reticle alpha=100%	# reticle bright
.frame n+=20
	.change reticle			# hold reticle
.frame n+=20
	.change reticle alpha=0		# reticle dark
	.change pin			# hold that pin
	.change box1			# hold that box
.frame n+=20
	.macro tfader 0,10 mesg5
.frame n+=20
	.change box1 alpha=100%		# box bright
.frame n+=20
	.change pin alpha=100%		# pin bright
.frame n+=20
	.macro tfader 0,10 mesg6
	.jump pin blue=0 red=+100%	# blink the pin!
.frame n+=5
	.jump pin blue=100% red=0
.frame n+=5
	.jump pin blue=0 red=+100%
.frame n+=5
	.jump pin blue=100% red=0
.frame n+=5
	.jump pin blue=0 red=+100%
.frame n+=5
	.jump pin blue=100% red=0
.frame n+=5
	.jump pin blue=0 red=+100%
.frame n+=5
	.jump pin blue=100% red=0
.frame n+=5
	.change reticle			# hold that reticle!
	.jump pin blue=0 red=+100%
.frame n+=5
	.jump pin blue=100% red=0
.frame n+=15
	.macro tfader 0,10 mesg7
	.change reticle alpha=100%
.frame n+=60
	.macro tfader 0,10 mesg8
.frame n+=20
	.change box1
.frame n+=20
	.macro tfader 0,10 mesg9
.frame n+=20
	.change box1 rotate+=90
.frame n+=40
	.macro tfader 0,10 mesg10
.frame n+=1
	.change box1 rotate-=90
	.change pin
.frame n+=59
	.macro tfader 0,10 mesg11
	.change box1
.frame n+=20
	.change box1 x=100 y=100 pin=center
.frame n+=5
        .jump pin blue=100% red=0
.frame n+=15
        .jump pin blue=0 red=+100%
.frame n+=15
        .jump pin blue=100% red=0
.frame n+=5
	.macro tfader 0,10 mesg12
        .jump pin blue=0 red=+100%
.frame n+=5
        .jump pin blue=100% red=0
.frame n+=5
        .jump pin blue=0 red=+100%
.frame n+=5
	.jump pin blue=100% red=0	
.frame n+=5
	.change box1
.frame n+=20
	.macro tfader 0,10 mesg13
	.change box1 rotate+=90 pin=center
.frame n+=40
	.change box1 rotate-=90
.frame n+=20
	.macro tfader 0,10 mesg14
	.change box1 pin=(0,0) x=100 y=100
.frame n+=60
	.macro tfader 0,10 mesg15
	.change box1 pin=center rotate+=90
.frame n+=60
	.change box1 pin=(0,0) rotate-=90
	.macro tfader 0,10 mesg16
.frame n+=40
	.change box1 scale=50%
.frame n+=40
	.change box1 scale=100%
.frame n+=15 
	.change box1 pin=center
.frame n+=5
	.macro tfader 0,10 mesg17
.frame n+=40
	.change box1 scale=50%
.frame n+=40
	.change box1 scale=100% alpha=0
.frame n+=5
	.macro tfader 0,10 mesg18
        .jump pin blue=0 red=+100%
.frame n+=5
        .jump pin blue=100% red=0
.frame n+=5
        .jump pin blue=0 red=+100%
.frame n+=5
        .jump pin blue=100% red=0
.frame n+=20
	.change pin alpha=0

.end


