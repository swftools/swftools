# Pins.sc
# tutorial on Pins for the user.

.swf bbox=autocrop version=4 fps=25 name="Pins.swf"

.sprite reticle
	.box box width=5 height=5 color=yellow
	.put b1=box 0 0
	.put b2=box 5 0
	.put b3=box 0 5
	.put b4=box 5 5
.end
#
.circle pin 8 color=blue fill=blue
#
.font helv "HelveticaBold.swf"
#
.text mesg1 text="(1) This tutorial explains \"pins\" . . ." font=helv color=yellow size=25%
.text mesg2 text="(2) Every object such as this simple box ..." font=helv color=red size=25%
.text mesg3 text="(3) ... is \"pinned\" to a screen x,y." font=helv color=yellow size=25%
.text mesg4 text="(4) The yellow reticle is at 100,100..." font=helv color=yellow size=25%
.text mesg5 text="(5) The box's \"pin\" is at (relative to the box) 0,0." font=helv color=green size=25%
.text mesg6 text="(6) When we .put the box at 100,100, its pin lands..." font=helv color=blue size=25%
.text mesg7 text="(7) ... on 100,100." font=helv color=yellow size=25%
#
.text mesg8 text="(8) Relative 0,0 is the default pin for placement." font=helv color=yellow size=25%
.text mesg9 text="(9) If we rotate..." font=helv color=red size=25%

.box box1 width=100 height=100 color=red line=5     # a simple red box
    
.frame 0  
        .put mesg1 x=0 y=10		# mesg1 bright 
        .put box1 x=100 y=100 alpha=0
.frame 20
	.change mesg1			# mesg 1 hold
.frame 40
	.change mesg1 alpha=0		# mesg 1 fade
	.put mesg2 x=0 y=10 alpha=0	# mesg 2 placed	 (new message placed during old fade)
.frame 60
	.del mesg1			# mesg 1 gone
	.change mesg2 alpha=100% 	# MESG 2 bright	 *** a "slide" lasts 40 frames
	.change box1 alpha=100%		# box bright
.frame 80
	.change mesg2 			# mesg 2 hold 
	.change box1 			# box hold
	.change box1 alpha=0		# box fade
.frame 100
	.change mesg2 alpha=0		# mesg 2 fade
	.put mesg3 x=0 y=10 alpha=0	# mesg 3 placed
.frame 120
	.del mesg2			# mesg 2 gone
	.change mesg3 alpha=100%	# mesg 3 bright	***
	.jump box1 alpha=100%		# make the box appear suddenly and leap around a bit
.frame 125
	.jump box1 x+=20 y+=20		# BUG?  why doesn't this work?  am I confused?
.frame 130
	.jump box1 x-=20		# shouldn't the box be dancing around the screen?
.frame 135
	.jump box1 x-=20 y-=20
.frame 140
	.change mesg3 			# mesg 3 hold
	.jump box1 x=100 y=100		# put box bck where it belongs
.frame 160
	.change mesg3 alpha=0		# mesg 3 fade
	.put mesg4 x=0 y=10 alpha=0	# mesg 4 placed
	.put pin 100 100 alpha=0 pin=center	# pin placed (dark, bottom layer)
	.put reticle 100 100 alpha=0 pin=center	# place reticle (dark, top layer)
	.change box1 alpha=0		# box fade
.frame 180
	.del mesg3			# mesg 3 gone
	.change mesg4 alpha=100		# mesg 4 bright	***
	.change reticle alpha=100%	# reticle bright
.frame 200
	.change mesg4			# mesg 4 hold
	.change reticle			# hold reticle
.frame 220
	.change mesg4 alpha=0		# mesg 4 fade
	.put mesg5 x=0 y=10 alpha=0	# mesg 5 placed
	.change reticle alpha=0		# reticle dark
.frame 240
	.del mesg4			# mesg 4 gone
	.change mesg5 alpha=100%	# mesg 5 bright	***
	.change box1 alpha=100%		# box bright
	.change pin alpha=100%		# pin bright
.frame 260
	.change mesg5			# mesg 5 hold
.frame 280
	.change mesg5 alpha=0		# mesg 5 fade
	.put mesg6 x=0 y=10 alpha=0	# mesg 6 placed
.frame 300
	.del mesg5			# mesg 5 gone
	.change mesg6 alpha=100%	# mesg 6 bright	 ***
	.jump pin blue=0 red=+100%	# blink the pin
.frame 305
	.jump pin blue=100% red=0
.frame 310
	.jump pin blue=0 red=+100%
.frame 315
	.jump pin blue=100% red=0
.frame 320
	.change mesg6			# mesg 6 hold
.frame 340
	.change mesg6 alpha=0		# mesg 6 fade
	.put mesg7  x=0 y=10 alpha=0 	# mesg7 placed
	.change reticle			# hold that reticle!
.frame 360
	.change mesg7 alpha=100%	# mesg7 bright	***
	.change reticle alpha=100%
.frame 380
	.change mesg7			# mesg 7 hold
.frame 400
	.change mesg7 alpha=0		# mesg 7 fade
	.put mesg8 x=0 y=10 alpha=0	# mesg 8 placed
.frame 420
	.del mesg7			# mesg 7 gone
	.change mesg8 alpha=100%	# mesg 8 bright ***
.frame 440
	.change mesg8			# mesg 8 hold
.frame 460
	.change mesg8 alpha=0		# mesg 8 fade
.frame 480
	.del mesg8			# mesg 8 gone
.frame 500
.frame 520
.end


# life cycle of text
# 	.put mesg6 x=0 y=10 alpha=0	# mesg 6 placed
# 	.change mesg6 alpha=100%	# mesg 6 bright
# 	.change mesg6			# mesg 6 hold
# 	.change mesg6 alpha=0		# mesg 6 fade
# 	.del mesg6			# mesg 6 gone
