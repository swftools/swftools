# (The only difference between those two files is the change alpha to 0 at
#  frame 100.)

.swf bbox=autocrop version=4 fps=25 name="GoodPins.swf"
.box box1 width=100 height=100 color=red line=5     # a simple red box
.frame 0  
        .put box1 x=100 y=100 alpha=0
.frame 60
	.change box1 alpha=100%		# box bright
.frame 80
	.change box1 			# box hold
.frame 120
	.jump box1 alpha=100%		# make the box appear suddenly and leap around a bit
.frame 125
	.jump box1 x+=20 y+=20
.frame 130
	.jump box1 x-=20
.frame 135
	.jump box1 x-=20 y-=20
.frame 140
	.jump box1 x=100 y=100		# put box bck where it belongs
.frame 160
	.change box1 alpha=0		# box fade
.end


.swf bbox=autocrop version=4 fps=25 name="BadPins.swf"
.box box1 width=100 height=100 color=red line=5     # a simple red box
.frame 0  
        .put box1 x=100 y=100 alpha=0
.frame 60
	.change box1 alpha=100%		# box bright
.frame 80
	.change box1 			# box hold
.frame 100
	.change box1 alpha=0		# box fade
.frame 120
	.jump box1 alpha=100%		# make the box appear suddenly and leap around a bit
.frame 125
	.jump box1 x+=20 y+=20		# why doesn't this work?
.frame 130
	.jump box1 x-=20
.frame 135
	.jump box1 x-=20 y-=20
.frame 140
	.jump box1 x=100 y=100		# put box bck where it belongs
.frame 160
	.change box1 alpha=0		# box fade
.end
