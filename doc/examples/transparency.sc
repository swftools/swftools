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

