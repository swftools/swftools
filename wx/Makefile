top_builddir = ../..
srcdir = .
top_srcdir = ..
include ../Makefile.common

all: pdf2swf_gui.exe

pdf2swf_gui.exe: gpdf2swf.py gui/*.py viewers/*.py
	cp -a ../installer/swftools.ico /xpython/xpython.ico
	cp -a gpdf2swf.py /xpython/main.py
	cp -a -R viewers gui lib /xpython/
	cd /xpython/swftools;git pull
	cd /xpython;make xpython.exe
	ln -f /xpython/xpython.exe ./gpdf2swf.exe
	#ln -f gpdf2swf.exe ../win32_gui/
	ln -f gpdf2swf.exe ../win32/
	cp -R viewers/* ../win32/viewers/
	rm -rf ../win32/viewers/flexpaper* # we're not distributing flexpaper

.PHONY: exe
