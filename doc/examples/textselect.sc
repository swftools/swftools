.flash version=6

    .font Arial "../doc/Arial.swf"
    .edittext et width=600 height=50 color=black font=Arial size=40pt border text=test variable=text_selection 

    .frame 1

	.swf TextBox document.swf
	.put TextBox
	.put et x=10 y=10

    .frame 2

    .action:
	str = TextBox.getTextSnapShot();
	str.setSelectColor(0xffff00);
	firstChar = -1;

        TextBox.onMouseDown = function() {
	    firstChar = str.hitTestTextNearPos(_xmouse, _ymouse, 0);
	    str.setSelected(0, str.getCount(), false);
	    _root.text_selection = str.getSelectedText(true);
        };

        TextBox.onMouseUp = function() {
	    if (firstChar >= 0) {
		lastChar = str.hitTestTextNearPos(_xmouse, _ymouse, 0);
		if (firstChar != lastChar) {
		     if (firstChar < lastChar) {
			 str.setSelected(firstChar, lastChar+1, true);
			 _root.text_selection = str.getSelectedText(true);
		     } else {
			 str.setSelected(lastChar, firstChar+1, true);
		     }
		 }
		 _root.text_selection = str.getSelectedText(true);
		 firstChar = -1;
		 lastChar = -1;
	    }
        };

        TextBox.onMouseMove = function() {
	    if(firstChar != -1) {
		str.setSelected(0, str.getCount(), false); //unselect everything

		lastChar = str.hitTestTextNearPos(_xmouse, _ymouse, 0);
		if (firstChar < lastChar) {
		    str.setSelected(firstChar, lastChar+1, true);
		} else if(lastChar >= 0) {
		    str.setSelected(lastChar, firstChar+1, true);
		}
		_root.text_selection = str.getSelectedText(true);
	    }
        };
    .end

    .frame 3
	.action:
	    Stop();
	.end

.end

