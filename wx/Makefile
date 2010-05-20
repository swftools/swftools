top_builddir = ../..
srcdir = .
top_srcdir = ..
include ../Makefile.common

all: pdf2swf_gui.exe

pdf2swf_gui.exe: gpdf2swf.py app.py document.py gui/*.py viewers/*.py
	cp gpdf2swf.py /xpython/main.py
	cp -R app.py document.py viewers gui /xpython/
	cd /xpython/swftools;git pull
	cd /xpython;make xpython.exe
	ln -f /xpython/xpython.exe ./gpdf2swf.exe
	ln -f gpdf2swf.exe ../win32_gui/
	ln -f gpdf2swf.exe ../win32/

.PHONY: exe
