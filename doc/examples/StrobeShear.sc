# shear.sc
#
# Shearing

.swf bbox=300x200:-30 name="StrobeShear.swf" fps=25
    .font helvetica "Helvetica.swf"
    .text text text="Shear" font=helvetica size=100% color=yellow
.frame 0
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

