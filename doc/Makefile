all:
	rm -f swfc.*.html
	rm -f data/* || true
	xsltproc guide.xsl swfc.xml > tmp.html
	perl codebeautifier.pl < tmp.html > swfc.html
	perl seperate.pl swfc.html
	cp swfc.1.html swfc.html
	rm tmp.html
	cp beach.jpg Arial.swf Courier.swf Times.swf scenery.jpg data
	cp swfc.html swfc.*.html caiviar.css data

upload:
	./syncswfc
