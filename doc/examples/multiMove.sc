# shear.sc
#
# test shear

.flash bbox=autocrop version=6 fps=50 name="multiMove.swf"  
#
.font helv "HelveticaBold.swf"
#
.text mesg1 text="I am just translating now" font=helv color=red size=50%
.text mesg2 text="I am just shearing now" font=helv color=red size=50%
.text mesg3 text="I am translating AND shearing now" font=helv color=yellow size=50%
#
.text mesg4 text="I am just rotating now" font=helv color=blue size=50%
.text mesg5 text="I am translating and rotating now" font=helv color=green size=50%
#
.text mesg6 text="I am just rotating now" font=helv color=blue size=50%
.text mesg7 text="I am shearing and rotating now" font=helv color=yellow size=50%
#
.text mesg8 text="I am now translating AND rotating AND shearing!" font=helv color=yellow size=50%
.text mesg9 text="And now we are done." font=helv color=red size=50%

.box box1 width=100 height=100 color=red line=5     # a simple red box
    
.frame 0  
        .put box1 x=100 y=100 pin=center
	.put mesg1 x=0 y=10
.frame 50
        .change box1 x+=100  pin=center
.frame 100
	.change box1 x-=100 pin=center
	.del mesg1
	.put mesg2 x=0 y=10
.frame 150
	.change box1 shear=1.0 pin=center
.frame 200
	.change box1 shear=0.0 pin=center
	.del mesg2
	.put mesg3  x=0 y=10
.frame 250
	.change box1 x+=100 shear=1.0 pin=center
.frame 300
	.change box1 x-=100 shear=0.0 pin=center
	.del mesg3
	.put mesg4  x=0 y=10
.frame 350
	.change box1 rotate+=180 pin=center
.frame 400
	.change box1 rotate-=180 pin=center
	.del mesg4
	.put mesg5  x=0 y=10
.frame 450
	.change box1 rotate+=180 x+=100 pin=center
.frame 500
	.change box1 rotate-=180 x-=100 pin=center
	.del mesg5
	.put mesg7 x=0 y=10
.frame 550
	.change box1 rotate+=180 shear=1.0 pin=center
.frame 600
	.change box1 rotate-=180 shear=0.0 pin=center
	.del mesg7
	.put mesg8 x=0 y=10
.frame 650
	.change box1 rotate+=180 x+=100 shear=1.0 pin=center
.frame 700
	.change box1 rotate-=180 x-=100 shear=0.0 pin=center
	.del mesg8
	.put mesg9 x=0 y=10
.frame 750
	.change mesg9 alpha=0
	.change box1 alpha=0 scale=0


.end

