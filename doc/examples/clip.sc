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
