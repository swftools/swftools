# Shearing

.flash bbox=autocrop name="ghosts.swf" fps=25

.font helvetica "Helvetica.swf"
.text text text="swfc" font=helvetica size=100% color=yellow

.frame 0
    .put t0=text x=150 y=150 alpha=100%
    .put t0a=text x=150 y=150 alpha=95% 
    .put t0b=text x=150 y=150 alpha=90% 
    .put t1=text x=150 y=150 alpha=85% 
    .put t1a=text x=150 y=150 alpha=80% 
    .put t1b=text x=150 y=150 alpha=75% 
    .put t2=text x=150 y=150 alpha=70% 
    .put t2a=text x=150 y=150 alpha=65%
    .put t2b=text x=150 y=150 alpha=60% 
    .put t3=text x=150 y=150 alpha=55% 
    .put t3a=text x=150 y=150 alpha=50% 
    .put t3b=text x=150 y=150 alpha=45% 
.frame 2
	.change t0a
.frame 4
	.change t0b
.frame 5
	.change t1
.frame 7
	.change t1a
.frame 9
	.change t1b
.frame 10
	.change t2
.frame 12
	.change t2a
.frame 14
	.change t2b
.frame 15 
	.change t3
.frame 20
	.change t3a
.frame 25
	.change t3b
.frame 100
    .change t0 rotate+=360 
.frame 102
    .change t0a rotate+=360 alpha=0
.frame 104
    .change t0b rotate+=360 alpha=0
.frame 105
    .change t1 rotate+=360 alpha=0
.frame 107
    .change t1a rotate+=360 alpha=0
.frame 109
    .change t1b rotate+=360 alpha=0
.frame 110
    .change t2 rotate+=360 alpha=0
.frame 112
    .change t2a rotate+=360 alpha=0
.frame 114
    .change t2b rotate+=360 alpha=0
.frame 115
    .change t3 rotate+=360 alpha=0
.frame 117
    .change t3a  rotate+=360 alpha=0
.frame 119
     .change t3b rotate+=360 alpha=0
.frame 122
    .change t0 .change t1 .change t2 .change t3
    .change t0a .change t0b .change t1a .change t1b 
    .change t2a .change t2b .change t3a .change t3b
.frame 150
    .change t0 alpha=0
.end

