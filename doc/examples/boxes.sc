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

