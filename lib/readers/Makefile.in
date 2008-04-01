# Generated automatically from Makefile.in by configure.
top_builddir = ../..
srcdir = .
top_srcdir = ../..
include ../../Makefile.common

all: swf.$(O) image.$(O)

libswf: ../libswf$(A)

swf.$(O): swf.c swf.h
	$(C) swf.c -o swf.$(O)

image.$(O): image.c image.h
	$(C) image.c -o image.$(O)

swfrender: $(libswf_objects) ../../src/swfrender.c ../devices/render.$(O)
	$(L) ../../src/swfrender.c $(libswf_objects) ../librfxswf$(A) ../gfxtools.$(O) ../png.$(O) ../devices/render.$(O) -o swfrender $(LIBS)

install:
uninstall:

clean: 
	rm -f *.o *.obj *.lo *.a *.lib *.la gmon.out

