# points.sc
#
# Test points, pivots, pins

.swf version=5 fps=20 name="points.swf"

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

