# Pins.sc
# tutorial on Pins for the user.

.flash bbox=autocrop version=4 fps=25 name="Pins.swf"

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
.text mesg1 text="(1) This tutorial explains \"pins\" . . ." font=helv color=yellow size=100%
.text mesg2 text="(2) Every object such as this simple box ..." font=helv color=red size=100%
.text mesg3 text="(3) ... is \"pinned\" to a screen x,y." font=helv color=yellow size=100%
#
.text mesg4 text="(4) The yellow reticle is at 100,100..." font=helv color=yellow size=100%
.text mesg5 text="(5) The box's \"pin\" is at (relative to the box) 0,0." font=helv color=green size=100%
.text mesg6 text="(6) When we .put the box at 100,100, its pin lands..." font=helv color=blue size=100%
.text mesg7 text="(7) ... on 100,100." font=helv color=yellow size=100%
#
.text mesg8 text="(8) Relative 0,0 is the default pin." font=helv color=yellow size=100%
.text mesg9 text="(9) If we rotate the box..." font=helv color=red size=100%
.text mesg10 text="(10)... it rotates around its default pin" font=helv color=red size=100%
.text mesg11 text="(11) If we specify a special pin called \"center\"" font=helv color=red size=100%
.text mesg12 text="(12) the box center will be at 100,100..." font=helv color=red size=100%
.text mesg13 text="(13) and it will rotate in place." font=helv color=red size=100%
#
.text mesg14 text="(14) Interesting things will happen..." font=helv color=red size=100%
.text mesg15 text="(15) ... if a .change command moves the pin!" font=helv color=red size=100%
# .text mesg13 text="(16) and it will rotate in place." font=helv color=red size=100%
# .text mesg13 text="(17) and it will rotate in place." font=helv color=red size=100%
# .text mesg13 text="(18) and it will rotate in place." font=helv color=red size=100%
# .text mesg13 text="(19) and it will rotate in place." font=helv color=red size=100%

#
# oh no, we have to make zillions of these sprites... sigh...
.sprite m1
	.frame 0
 	.put mesg1 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg1 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg1			# mesg 1 hold
	.frame 60
 	.change mesg1 alpha=0		# mesg 1 fade
	.frame 65
 	.del mesg1			# mesg 1 gone
.end
#
.sprite m2
	.frame 0
 	.put mesg2 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg2 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg2			# mesg 1 hold
	.frame 60
 	.change mesg2 alpha=0		# mesg 1 fade
	.frame 65
 	.del mesg2			# mesg 1 gone
.end
#
.sprite m3
	.frame 0
 	.put mesg3 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg3 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg3			# mesg 1 hold
	.frame 60
 	.change mesg3 alpha=0		# mesg 1 fade
	.frame 65
 	.del mesg3			# mesg 1 gone
.end
#
.sprite m4
	.frame 0
 	.put mesg4 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg4 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg4			# mesg 1 hold
	.frame 60
 	.change mesg4 alpha=0		# mesg 1 fade
	.frame 65
 	.del mesg4			# mesg 1 gone
.end
#
.sprite m5
	.frame 0
 	.put mesg5 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg5 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg5			# mesg 1 hold
	.frame 60
 	.change mesg5 alpha=0		# mesg 1 fade
	.frame 65
 	.del mesg5			# mesg 1 gone
.end
#
.sprite m6
	.frame 0
 	.put mesg6 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg6 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg6			# mesg 1 hold
	.frame 60
 	.change mesg6 alpha=0		# mesg 1 fade
	.frame 65
 	.del mesg6			# mesg 1 gone
.end
#
.sprite m7
	.frame 0
 	.put mesg7 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg7 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg7			# mesg 1 hold
	.frame 60
 	.change mesg7 alpha=0		# mesg 1 fade
	.frame 65
 	.del mesg7			# mesg 1 gone
.end
#
.sprite m8
	.frame 0
 	.put mesg8 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg8 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg8			# mesg 1 hold
	.frame 60
 	.change mesg8 alpha=0		# mesg 1 fade
	.frame 65
 	.del mesg8			# mesg 1 gone
.end
#
.sprite m9
	.frame 0
 	.put mesg9 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg9 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg9			# mesg 1 hold
	.frame 60
 	.change mesg9 alpha=0		# mesg 1 fade
.end
#
.sprite m10
	.frame 0
 	.put mesg10 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg10 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg10			# mesg 1 hold
	.frame 60
 	.change mesg10 alpha=0		# mesg 1 fade
.end
#
.sprite m11
	.frame 0
 	.put mesg11 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg11 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg11			# mesg 1 hold
	.frame 60
 	.change mesg11 alpha=0		# mesg 1 fade
.end
#
.sprite m12
	.frame 0
 	.put mesg12 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg12 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg12			# mesg 1 hold
	.frame 60
 	.change mesg12 alpha=0		# mesg 1 fade
.end
#
.sprite m13
	.frame 0
 	.put mesg13 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg13 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg13			# mesg 1 hold
	.frame 60
 	.change mesg13 alpha=0		# mesg 1 fade
.end
#
.sprite m14
	.frame 0
 	.put mesg14 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg14 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg14			# mesg 1 hold
	.frame 60
 	.change mesg14 alpha=0		# mesg 1 fade
.end
#
.sprite m15
	.frame 0
 	.put mesg15 x=0 y=10 alpha=0	# mesg 1 placed
	.frame 20
 	.change mesg15 alpha=100%	# mesg 1 bright
	.frame 40
 	.change mesg15			# mesg 1 hold
	.frame 60
 	.change mesg15 alpha=0		# mesg 1 fade
.end
#
.box box1 width=100 height=100 color=red line=5     # a simple red box
    
.frame 0  
        .put m1 0 0			# at rel frame 20 is the bright spot, or "next" frame
        .put box1 x=100 y=100 alpha=0
.frame 20				# *** slide 1
.frame 60
	.change box1 alpha=100%		# box bright
	.put m2 0 0 
.frame 61
	.del m1
.frame 80
	.change box1 			# box hold
.frame 120
	.put m3 0 0
	.jump box1 alpha=100%		# make the box appear suddenly and leap around a bit
.frame 121 .del m2
.frame 125
	.jump box1 x+=20 y+=20		# why doesn't this work?
.frame 130
	.jump box1 x-=20
.frame 135
	.jump box1 x-=20 y-=20
.frame 140
	.jump box1 x=100 y=100		# put box bck where it belongs
.frame 145
	.jump box1 x-=20 y-=15
.frame 150
	.jump box1 x+=20 y+=40
.frame 155
	.jump box1 x=100 y=100
.frame 160
	.put pin 100 100 alpha=0 pin=center	# pin placed (dark, bottom layer)
	.put reticle 100 100 alpha=0 pin=center	# place reticle (dark, top layer)
.frame 180
	.put m4 0 0
	.change box1 alpha=0		# box fade
	.change reticle alpha=100%	# reticle bright
.frame 181
	.del m3
.frame 200
	.change reticle			# hold reticle
.frame 220
	.change reticle alpha=0		# reticle dark
	.change pin			# hold that pin
	.change box1			# hold that box
.frame 240
	.put m5 0 0
.frame 241
	.del m4
.frame 260
	.change box1 alpha=100%		# box bright
.frame 280
	.change pin alpha=100%		# pin bright
.frame 300
	.put m6 0 0
	.jump pin blue=0 red=+100%	# blink the pin!
.frame 301
	.del m5
.frame 305
	.jump pin blue=100% red=0
.frame 310
	.jump pin blue=0 red=+100%
.frame 315
	.jump pin blue=100% red=0
.frame 320
	.jump pin blue=0 red=+100%
.frame 325
	.jump pin blue=100% red=0
.frame 330
	.jump pin blue=0 red=+100%
.frame 335
	.jump pin blue=100% red=0
.frame 340
	.change reticle			# hold that reticle!
	.jump pin blue=0 red=+100%
.frame 345
	.jump pin blue=100% red=0
.frame 360
	.put m7 0 0
	.change reticle alpha=100%
.frame 361
	.del m6
.frame 420
	.put m8 0 0
.frame 421
	.del m7
.frame 460
	.change box1
.frame 480
	.put m9
.frame 481 
	.del m8
.frame 500
	.change box1 rotate+=90
.frame 540
	.put m10
.frame 541 
	.change box1 rotate-=90
	.del m9
	.change pin
.frame 600
	.put m11
	.change box1
.frame 601
	.del m10
.frame 620
	.change box1 x=100 y=100 pin=center
.frame 625
        .jump pin blue=100% red=0
.frame 640
        .jump pin blue=0 red=+100%
.frame 655
        .jump pin blue=100% red=0
.frame 660
	.put m12
        .jump pin blue=0 red=+100%
.frame 661 
	.del m11
.frame 665
        .jump pin blue=100% red=0
.frame 670
        .jump pin blue=0 red=+100%
.frame 675
	.jump pin blue=100% red=0	
.frame 700
	.change box1
.frame 720
	.put m13
	.change box1 rotate+=90 pin=center
.frame 721
	.del m12
.frame 760
	.change box1 rotate-=90
.frame 780
	.put m14
	.change box1 pin=(0,0) x=100 y=100
.frame 781
	.del m13
.frame 840
	.put m15
	.change box1 pin=center rotate+=90
.frame 841
	.del m14
.frame 901
	.del m15
	.change box1 pin=(0,0) rotate-=90
.frame 960
	.change box1 alpha=0

.end


