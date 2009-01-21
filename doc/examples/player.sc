.flash version=6 fps=50 name="player.swf" bbox=800x600

    .swf filetoplay "cxform.swf"
    .put filetoplay

    .action:
	filetoplay.Stop();
	Stop();
    .end

    .gradient gblue:
        0% #4040ff
        25% #8080ff
        50% #c0c0ff
        75% #8080ff
        100% #4040ff
    .end

    .gradient ggreen:
        0% #40ff40
        25% #80ff80
        50% #c0ffc0
        75% #80ff80
        100% #40ff40
    .end

    .gradient gcyan:
        0% #40ffff
        25% #80ffff
        50% #c0ffff
        75% #80ffff
        100% #40ffff
    .end

    .outline arrowoutline:
	moveTo -30,-10
	lineTo -30,10
	lineTo    0,10
	lineTo    0,20
	lineTo  30,0
	lineTo    0,-20
	lineTo    0,-10
	lineTo -30,-10
    .end
    .outline stopoutline:
	moveTo -20,-20
	lineTo -20, 20
	lineTo  20, 20
	lineTo  20,-20
	lineTo -20,-20
    .end
    .outline pauseoutline:
	moveTo -20,-20
	lineTo -20, 20
	lineTo -4, 20
	lineTo -4,-20
	lineTo -20,-20

	moveTo  20,-20
	lineTo  20, 20
	lineTo  4, 20
	lineTo  4,-20
	lineTo  20,-20
    .end

    .filled arrow_blue outline=arrowoutline fill=gblue line=3 color=blue
    .filled arrow_green outline=arrowoutline fill=ggreen line=3 color=green
    .filled arrow_cyan outline=arrowoutline fill=gcyan line=3 color=cyan
    
    .filled stop_blue outline=stopoutline fill=gblue line=3 color=blue
    .filled stop_green outline=stopoutline fill=ggreen line=3 color=green
    .filled stop_cyan outline=stopoutline fill=gcyan line=3 color=cyan
    
    .filled pause_blue outline=pauseoutline fill=gblue line=3 color=blue
    .filled pause_green outline=pauseoutline fill=ggreen line=3 color=green
    .filled pause_cyan outline=pauseoutline fill=gcyan line=3 color=cyan

    .button leftbutton
	.show arrow_blue as=area,idle scalex=-100% alpha=50%
	.show arrow_green as=hover scalex=-100%
	.show arrow_cyan as=pressed scalex=-100%
	.on_release inside:
	    filetoplay.gotoAndStop(1);
	.end
    .end
    
    .button stopbutton
	.show stop_blue as=area,idle alpha=50%
	.show stop_green as=hover alpha=50%
	.show stop_cyan as=pressed alpha=50%
	.on_release inside:
	    filetoplay.Stop();
	.end
    .end

    .button pausebutton
	.show pause_blue as=area,idle alpha=50%
	.show pause_green as=hover alpha=50%
	.show pause_cyan as=pressed alpha=50%
	.on_release inside:
	    filetoplay.Stop();
	.end
    .end

    .button rightbutton
	.show arrow_blue as=area,idle alpha=50%
	.show arrow_green as=hover alpha=50%
	.show arrow_cyan as=pressed alpha=50%
	.on_release inside:
	    filetoplay.Play();
	.end
    .end
  
    .sprite overlay:
        .put leftbutton x=35 y=25
        .put stopbutton x=105 y=25
        .put pausebutton x=175 y=25
        .put rightbutton x=245 y=25
    .end


.end
