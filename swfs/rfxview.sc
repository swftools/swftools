# rfxview.sc - a document viewer for pdf2swf converted files
#
# Copyright (c) 2008 Matthias Kramm 
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following condition:
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


.flash filename=rfxview.swf fps=30 bbox=600x800 version=8 compress background=#CCCCCC export=0

.define width 600
.define height 800

#======================================== buttons ===========================================================

.gradient grad1 radial x=4 y=4 r=28:
 0% white
 50% #666666
 100% #202020
.end
.gradient grad2 linear width=8 height=6 rotate=120:
 0% white
 70% #666666
 100% #202020  
.end
.gradient grad4 linear width=8 height=6 rotate=120:
 0% white
 70% #999999
 100% #404040
.end

.gradient grad2l linear width=8 height=6 rotate=120:
 0% #e0e0e0
 100% #000000  
.end
.gradient grad4l linear width=8 height=6 rotate=120:
 0% #ffffff
 100% #404040
.end

.gradient grad3 radial x=4 y=4 r=28:
 0% white
 70% #999999
 100% #404040
.end

.gradient grad7 linear rotate=180:
 0% #777777
 100% #666666
.end

.gradient grad72 linear rotate=45:
 0% #999999
 100% #666666
.end

.outline arrowoutline:
    moveTo -8,-8
    lineTo -8, 8
    lineTo  8, 0
    lineTo -8,-8
.end

.outline larrowoutline:
    moveTo 8,-8
    lineTo 8, 8
    lineTo -8, 0
    lineTo 8,-8
.end

.font arialbold filename="../wx/ArialBold.ttf" glyphs="1:"
.font arial filename="../wx/Arial.ttf" glyphs="0123456789 /:"

.textshape oneoneoutline text="1:1" font=arialbold size=14pt

.outline plusoutline:
    moveTo  2,-8
    lineTo -2,-8
    lineTo -2,-2
    lineTo -8,-2
    lineTo -8,2
    lineTo -2,2
    lineTo -2,8
    lineTo  2,8
    lineTo  2,2
    lineTo  8,2
    lineTo  8,-2
    lineTo  2,-2
    lineTo  2,-8
.end

.outline zoomoutline:
    moveTo 8,0 lineTo 4,3 lineTo 4,-3 lineTo 8,0
    moveTo -8,0 lineTo -4,3 lineTo -4,-3 lineTo -8,0
    moveTo 0,8 lineTo 3,4 lineTo -3,4 lineTo 0,8
    moveTo 0,-8 lineTo 3,-4 lineTo -3,-4 lineTo 0,-8
.end

.outline minusoutline:
    moveTo -7,2
    lineTo -7,-2
    lineTo  7,-2
    lineTo  7,2
    lineTo -7,2
.end

.circle shadcircle line=0 fill=#00000033 r=11.5

.circle outcircle line=2 color=#00000060 fill=grad1 r=11
.circle outcircle_over line=2 color=#00000060 fill=grad3 r=11

.filled incircle outline=arrowoutline fill=grad2 line=1 color=#00000060
.filled incircle_over outline=arrowoutline fill=grad4 line=1 color=#00000060
.filled lincircle outline=larrowoutline fill=grad2l line=1 color=#00000060
.filled lincircle_over outline=larrowoutline fill=grad4l line=1 color=#00000060
.filled pincircle outline=plusoutline fill=grad2l line=1 color=#00000060
.filled pincircle_over outline=plusoutline fill=grad4l line=1 color=#00000060
.filled mincircle outline=minusoutline fill=grad2l line=1 color=#00000060
.filled mincircle_over outline=minusoutline fill=grad4l line=1 color=#00000060
.filled oincircle outline=oneoneoutline fill=grad2l line=1.2 color=#00000060
.filled oincircle_over outline=oneoneoutline fill=grad4l line=1.2 color=#00000060
.filled zincircle outline=zoomoutline fill=grad2l line=1 color=#00000060
.filled zincircle_over outline=zoomoutline fill=grad4l line=1 color=#00000060

.sprite plusface
 .put shadcircle 11 13 pin=center .put outcircle 11 11 pin=center .put pincircle 11 11 pin=center scale=90%
.end
.sprite plusfaceover 
 .put shadcircle 11 13 pin=center .put outcircle_over 11 11 pin=center .put pincircle_over 11 11 pin=center scale=90%
.end
.sprite plusfacedown 
 .put shadcircle 11 13 pin=center scale=90% .put outcircle_over 11 11 pin=center scale=90% .put pincircle_over 11 11 pin=center scale=79%
.end

.sprite minusface
 .put shadcircle 11 13 pin=center .put outcircle 11 11 pin=center .put mincircle 11 11 pin=center scale=90% luminance=60%
.end
.sprite minusfaceover 
 .put shadcircle 11 13 pin=center .put outcircle_over 11 11 pin=center .put mincircle_over 11 11 pin=center scale=90%
.end
.sprite minusfacedown 
 .put shadcircle 11 13 pin=center scale=90% .put outcircle_over 11 11 pin=center scale=90% .put mincircle_over 11 11 pin=center scale=79%
.end

.sprite rbface
 .put shadcircle 11 13 pin=center .put outcircle 11 11 pin=center .put incircle 13 11 pin=center scale=90%
.end
.sprite rbfaceover 
 .put shadcircle 11 13 pin=center .put outcircle_over 11 11 pin=center .put incircle_over 13 11 pin=center scale=90%
.end
.sprite rbfacedown 
 .put shadcircle 11 13 pin=center scale=90% .put outcircle_over 11 11 pin=center scale=90% .put incircle_over 13 11 pin=center scale=79%
.end

.sprite lbface
 .put shadcircle 11 13 pin=center .put outcircle 11 11 pin=center .put lincircle 9 11 pin=center scale=90%
.end
.sprite lbfaceover 
 .put shadcircle 11 13 pin=center .put outcircle_over 11 11 pin=center .put lincircle_over 9 11 pin=center scale=90%
.end
.sprite lbfacedown 
 .put shadcircle 11 13 pin=center scale=90% .put outcircle_over 11 11 pin=center scale=90% .put lincircle_over 9 11 pin=center scale=79%
.end

.sprite obface
 .put shadcircle 11 13 pin=center .put outcircle 11 11 pin=center .put oincircle 9 11 pin=center scale=90% luminance=50%
.end
.sprite obfaceover 
 .put shadcircle 11 13 pin=center .put outcircle_over 11 11 pin=center .put oincircle_over 9 11 pin=center scale=90%
.end
.sprite obfacedown 
 .put shadcircle 11 13 pin=center scale=90% .put outcircle_over 11 11 pin=center scale=90% .put oincircle_over 9 11 pin=center scale=79%
.end

.sprite zbface
 .put shadcircle 11 13 pin=center .put outcircle 11 11 pin=center .put zincircle 10 11 pin=center scale=90% luminance=50%
.end
.sprite zbfaceover 
 .put shadcircle 11 13 pin=center .put outcircle_over 11 11 pin=center .put zincircle_over 10 11 pin=center scale=90%
.end
.sprite zbfacedown 
 .put shadcircle 11 13 pin=center scale=90% .put outcircle_over 11 11 pin=center scale=90% .put zincircle_over 10 11 pin=center scale=79%
.end

.button rightbutton
  .show rbface as=idle
  .show outcircle as=area
  .show rbfacedown as=pressed
  .show rbfaceover as=hover
.end

.button leftbutton
  .show lbface as=idle
  .show outcircle as=area
  .show lbfacedown as=pressed
  .show lbfaceover as=hover
.end

.button plusbutton
  .show plusface as=idle
  .show outcircle as=area
  .show plusfacedown as=pressed
  .show plusfaceover as=hover
.end

.button minusbutton
  .show minusface as=idle
  .show outcircle as=area
  .show minusfacedown as=pressed
  .show minusfaceover as=hover
.end

.button oneonebutton
  .show obface as=idle
  .show outcircle as=area
  .show obfacedown as=pressed
  .show obfaceover as=hover
.end

.button fullsizebutton
  .show zbface as=idle
  .show outcircle as=area
  .show zbfacedown as=pressed
  .show zbfaceover as=hover
.end

#======================================== frame ===========================================================
  
.frame 1

.box background_box line=1 color=#00000030 fill=#00000030 width=800 height=600
.sprite background
    .put background_box x=0 y=0
.end
.put background

# horizontal scroll outline
.box hscroll_box line=1 color=#000000 width=800-20 height=10
.sprite hscroll_outline
    .put hscroll_box x=0 y=0
.end
.put hscroll_outline

# vertical scroll outline
.box vscroll_box line=1 color=#000000 width=10 height=600-20
.sprite vscroll_outline
    .put vscroll_box x=0 y=0
.end
.put vscroll_outline

# content outline
.box content_box width=100 height=100 line=1 color=#000000
.sprite content_outline
    .put content_box x=0 y=0
.end
.put content_outline

.put p1=plusbutton x=width-25-20 y=20 pin=center
.put m1=minusbutton x=width-50-20 y=20 pin=center
.put o1=oneonebutton x=width-75-20 y=20 pin=center
.put z1=fullsizebutton x=width-100-20 y=20 pin=center

.put l1=leftbutton x=width/2-100 y=20 pin=center
.put r1=rightbutton x=width/2+100 y=20 pin=center

.sprite vscrollbar
    .box vscroll2_shadow line=0 fill=#00000033 width=14 height=100
    .sprite vshadow
    .put vscroll2_shadow x=0 y=0 luminance=1.8
    .end
    .put vshadow

    .box vscroll2 width=14 height=100 line=1 color=#00000060 fill=grad7
    .sprite vbox
    .put vscroll2 x=0 y=0 luminance=1.5
    .end
    .put vbox

    .box tick line=1 color=black width=8 height=0

    .sprite t1 .put tt1=tick y=0 x=3 .end .put t1
    .sprite t2 .put tt2=tick y=0 x=3 .end .put t2 y=50
    .sprite t3 .put tt3=tick y=0 x=3 .end .put t3
    
    .button vscrollbutton
	.show vscroll2 as=area
	.on_press inside:
	    startDrag(false, _parent.fullwidth-22, 40, 
	                     _parent.fullwidth-22, 40+_parent.scrollbaryrange);
	    _parent.Dragging = "v";
	.end
	.on_release:
	    stopDrag();
	    _parent.refreshDrag();
	    _parent.Dragging = "";
	.end
    .end
    .put vscrollbutton
.end

.sprite hscrollbar
    .box hscroll2_shadow width=100 line=0 fill=#00000033 height=14
    .sprite hshadow
	.put hscroll2_shadow x=0 y=0 luminance=1.5
    .end
    .put hshadow

    .box hscroll2 width=100 height=14 line=1 color=#00000060 fill=grad72
    .sprite hbox
    .put hscroll2 x=0 y=0 luminance=1.5
    .end 
    .put hbox

    .box htick line=1 color=black width=0 height=8

    .sprite th1 .put tht1=htick x=0 y=3 .end .put th1
    .sprite th2 .put tht2=htick x=0 y=3 .end .put th2 x=50
    .sprite th3 .put tht3=htick x=0 y=3 .end .put th3

    .button hscrollbutton
	.show hscroll2 as=area
	.on_press inside:
	    startDrag(false, 10                        , _parent.fullheight-22, 
	                     10+_parent.scrollbarxrange, _parent.fullheight-22);
	    _parent.Dragging = "h";
	.end
	.on_release:
	    stopDrag();
	    _parent.refreshDrag();
	    _parent.Dragging = "";
	.end
    .end
    .put hscrollbutton
.end

.put hscrollbar y=height-22 x=10
.put vscrollbar x=width-22 y=40

.edittext et width=50 height=20 font=arial size=18pt color=black variable=current_pagenumber maxlength=3 text="1" align=left
.edittext etmiddle text="/" width=10 height=25 font=arial color=black noselect size=18pt align=center
.edittext et_total_pages width=50 height=25 font=arial color=black noselect size=18pt align=right

#.font dbgarial filename="Courier.ttf"
#.edittext debugtxt width=width height=20 font=dbgarial size=18pt color=#004000 noselect
#.put debugtxt y=20

.put et x=width/2-55 y=8
.put etmiddle x=width/2-5 y=8
.put et_total_pages x=width/2+5 y=8

#.swf viewport filename=paper5.viewport
.sprite viewport
.end

.box f width=100 height=100 line=0 fill=black

.button areabutton
    .show f as=area
    .on_press inside:
	viewport.startDrag(false, left-scrollxrange, top-scrollyrange, left,top);
	Dragging = "xy";
    .end
    .on_release:
	viewport.stopDrag();
	Dragging = "";
    .end
.end
.put areabutton x=10 y=40

.sprite cf
    .put f
.end
.put cliparea=cf x=10 y=40
.put viewport x=10 y=40


.action:
    et_total_pages.text=viewport._totalframes;
    swfwidth = viewport._width;
    swfheight = viewport._height;

    Dragging = "";
    
    Stage.scaleMode="noScale";
    Stage.align ="LT";
    
    //Stage.showMenu = false;

    fullwidth = Stage.width;
    fullheight = Stage.height;
    contentwidth = fullwidth - 40;
    contentheight = fullheight - 70;

    background._width = fullwidth-1;
    background._height = fullheight-1;

    // resize/position hscroll outline
    hscroll_outline._x = 10;
    hscroll_outline._y = fullheight-20;
    hscroll_outline._width = fullwidth-40;
    hscroll_outline._height = 10;

    // resize/position vscroll outline
    vscroll_outline._x = fullwidth-20;
    vscroll_outline._y = 40;
    vscroll_outline._width = 10;
    vscroll_outline._height = fullheight-70;

    // resize/position content outline
    content_outline._x = 9;
    content_outline._y = 39;
    content_outline._width = contentwidth+1;
    content_outline._height = contentheight+1;

    // move all objects to their proper positions
    vscrollbar._x = fullwidth-22;
    hscrollbar._y = fullheight-22;
    p1._x = fullwidth-25-20 - p1._width/2;
    o1._x = fullwidth-50-20 - o1._width/2;
    z1._x = fullwidth-75-20 - z1._width/2;
    m1._x = fullwidth-100-20 - m1._width/2;

    l1._x = fullwidth/2-100 - l1._width/2;
    r1._x = fullwidth/2+100 - r1._width/2;

    et._x = fullwidth/2 - (et._width+etmiddle._width+et_total_pages._width)/2;
    etmiddle._x = fullwidth/2 - (et._width+etmiddle._width+et_total_pages._width)/2 + et._width;
    et_total_pages._x = fullwidth/2 - (et._width+etmiddle._width+et_total_pages._width)/2 + et._width+etmiddle._width;
    
    areabutton._xscale = contentwidth;
    areabutton._yscale = contentheight;
    cliparea._xscale = contentwidth;
    cliparea._yscale = contentheight;
    viewport.setMask(cliparea);
    

    lastzoom = 1;

    zoom = 1;
 	
    //debugtxt.text = Stage.width+ " x " + Stage.height;
    //debugtxt.text = zoomtype;

    setPageNr = function(setscroll) {
        current_pagenumber = pagenr;
        viewport.gotoAndStop(pagenr);
        if(!setscroll) {
            viewport._y = top;
            swfpos2scrollbars();
        }
        else {
            viewport._y = setscroll;                
            swfpos2scrollbars();
        }
    };
   
    setNoScrollZoomLevel = function() {
	// determine initial zoom level
	xscale = contentwidth / swfwidth;
	yscale = contentheight / swfheight;
	if(xscale < yscale) {
	    zoom = xscale;
	    setZoomLevel();
	} else {
	    zoom = yscale;
	    setZoomLevel();
	}
    };
    
    setOneDirScrollZoomLevel = function() {
	xscale = contentwidth / swfwidth;
	yscale = contentheight / swfheight;
	if(xscale > yscale) {
	    zoom = xscale;
	    setZoomLevel();
	} else {
	    zoom = yscale;
	    setZoomLevel();
	}
    };
   
    set11ZoomLevel = function() {
	zoom = 1.0;
	setZoomLevel();
    };

    swfpos2scrollbars = function() {
	if(scrollxrange) {
	    hscrollbar._x = 10 + (left-viewport._x)*scrollbarxrange/scrollxrange;
	} else {
	    hscrollbar._x = 10;
	}
	if(scrollyrange) {
	    vscrollbar._y = 40 + (top-viewport._y)*scrollbaryrange/scrollyrange;
	} else {
	    vscrollbar._y = 40;
	}
    };
    setZoomLevel = function() {

	width = contentwidth;
	height = contentheight;
	left = 10;
	top = 40;

	if(swfwidth * zoom < contentwidth) {
	    width = swfwidth*zoom;
	    left = 10+(contentwidth-width)/2;
	}
	if(swfheight * zoom < contentheight) {
	    height = swfheight*zoom;
	    top = 40+(contentheight-height)/2;
	}
	    
	scrollxrange = swfwidth*zoom-width;
	scrollyrange = swfheight*zoom-height;
   
	hscrollbar._xscale = (contentwidth*width) / (swfwidth*zoom);
	vscrollbar._yscale = (contentheight*height) / (swfheight*zoom);
	scrollbarxrange = contentwidth - hscrollbar._xscale;
	scrollbaryrange = contentheight - vscrollbar._yscale;
	if(scrollbarxrange<0) {
	    scrollbarxrange = 0;
	}
	if(scrollbaryrange<0) {
	    scrollbaryrange = 0;
	}

	viewport._xscale = zoom*100;
	viewport._yscale = zoom*100;
   
	focusx = contentwidth/2 - (viewport._x-10);
	focusy = contentheight/2 - (viewport._y-40);

	viewport._x = left - focusx * zoom / lastzoom + width/2;
	viewport._y = top - focusy * zoom / lastzoom + height/2;

	if(viewport._x > left) {
	    viewport._x = left;
	} else if(viewport._x < left-scrollxrange) {
	    viewport._x = left-scrollxrange;
	} 
	if(viewport._y > top) {
	    viewport._y = top;
	} else if(viewport._y < top-scrollyrange) {
	    viewport._y = top-scrollyrange;
	}

	swfpos2scrollbars();

	if(scrollxrange) {
	    hscrollbar._visible = 1;
	} else {
	    //hscrollbar._visible = 0;
	}
	if(scrollyrange) {
	    vscrollbar._visible = 1;
	} else {
	    //vscrollbar._visible = 0;
	}

	lastzoom = zoom;

	// update the ruler decorations

	vscrollbar.t1._y = vscrollbar.t2._y - 600.0/vscrollbar._yscale;
	vscrollbar.t3._y = vscrollbar.t2._y + 600.0/vscrollbar._yscale;
	hscrollbar.th1._x = hscrollbar.th2._x - 600.0/hscrollbar._xscale;
	hscrollbar.th3._x = hscrollbar.th2._x + 700.0/hscrollbar._xscale;

	vscrollbar.vshadow._x = vscrollbar.vbox._x + 500/vscrollbar._xscale;
	vscrollbar.vshadow._y = vscrollbar.vbox._y + 500/vscrollbar._yscale;
	
	hscrollbar.hshadow._x = hscrollbar.hbox._x + 500/hscrollbar._xscale;
	hscrollbar.hshadow._y = hscrollbar.hbox._y + 500/hscrollbar._yscale;
    };

    pagenr = 1;
    setPageNr();

    if(zoomtype=="1") {
	set11ZoomLevel();
    } else if(zoomtype=="2") {
	setNoScrollZoomLevel();
    } else {
	setOneDirScrollZoomLevel();
    }
	
    et.onChanged = function(){
	if(current_pagenumber <= viewport._totalframes) {
	    if (current_pagenumber > 0) {
		pagenr = int(current_pagenumber);
		setPageNr();
	    }
	}
    };
    l1.onRelease = function(){ 
	if(pagenr > 1) {
	    pagenr = pagenr - 1;
	    setPageNr();
	}
    };
    r1.onRelease = function(){ 
	if(pagenr < viewport._totalframes) {
	    pagenr = pagenr + 1;
	    setPageNr();
	}
    };
    p1.onRelease = function(){ 
	if(zoom < 4) {
	    zoom = zoom + 1;
	    setZoomLevel();
	}
    };
    m1.onRelease = function(){ 
	if(zoom > 1) {
	    zoom = zoom - 1;
	    if(zoom < 1)
		zoom = 1;
	    setZoomLevel();
	}
    };
    o1.onRelease = function(){ 
	setNoScrollZoomLevel();
    };
    z1.onRelease = function(){ 
	setOneDirScrollZoomLevel();
    };
    refreshDrag = function(){
	if(Dragging == "h") {
	    viewport._x = left + (10-hscrollbar._x)*scrollxrange/scrollbarxrange;
	} else if(Dragging == "v") {
	    viewport._y = top + (40-vscrollbar._y)*scrollyrange/scrollbaryrange;
	} else if(Dragging == "xy") {
	    swfpos2scrollbars();
	}
    };
    dragrefresh = setInterval(refreshDrag, 20);

    var mouseListener = new Object();

    mouseListener.onMouseWheel = function(delta) {
        divideDelta = delta/Math.abs(delta);
        viewport._y = viewport._y + 30*zoom*divideDelta;
        swfpos2scrollbars();
        if(viewport._y < top-scrollyrange) {
            if(pagenr < viewport._totalframes) {
                pageNr = pageNr + 1;
                setPageNr();
            }
            else {
                viewport._y = top-scrollyrange;
                swfpos2scrollbars();
            }
        }
        else if(viewport._y > top) {
            if(pagenr > 1) {
                pageNr = pageNr - 1;
                setPageNr(top-scrollyrange);
            }
            else {
                viewport._y = top;
                swfpos2scrollbars();
            }
        }
    };

    Mouse.addListener(mouseListener);
.end

.end
