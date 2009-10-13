all: test stroke
include ../../Makefile.common

CC = gcc -O2 -g
#CC = gcc -O3

../libbase.a: ../q.c ../q.h ../mem.c ../mem.h
	cd ..; make libbase.a

../libgfx.a: ../gfxtools.h ../gfxtools.c ../gfxpoly.h ../gfxpoly.c
	cd ..; make libgfx.a

testheap: ../libbase.a testheap.c
	$(CC) testheap.c ../libbase.a -o testheap -lm -lz -ljpeg

OBJS = active.o convert.o poly.o wind.o renderpoly.o xrow.o stroke.o

active.o: active.c active.h poly.h
	$(CC) -c active.c -o active.o

convert.o: convert.c convert.h poly.h
	$(CC) -c convert.c -o convert.o

poly.o: poly.c poly.h active.h heap.h ../q.h
	$(CC) -c poly.c -o poly.o

wind.o: wind.c wind.h poly.h
	$(CC) -c wind.c -o wind.o

renderpoly.o: renderpoly.c wind.h poly.h renderpoly.h
	$(CC) -c renderpoly.c -o renderpoly.o

xrow.o: xrow.c xrow.h ../q.h ../mem.h
	$(CC) -c xrow.c -o xrow.o

stroke.o: stroke.c poly.h convert.h wind.h
	$(CC) -c stroke.c -o stroke.o

GFX=../gfxfont.o ../gfxtools.o  ../devices/ops.o ../devices/polyops.o ../devices/text.o ../devices/bbox.o ../devices/render.o ../devices/rescale.o ../devices/record.o
stroke: test_stroke.c $(OBJS) ../libgfxswf.a ../librfxswf.a ../libbase.a 
	$(CC) test_stroke.c $(OBJS) ../libgfxswf.a ../librfxswf.a $(GFX) ../libbase.a -o stroke $(LIBS)

SWF = ../librfxswf.a ../libpdf.a -lstdc++
test: ../libbase.a test.c $(OBJS) poly.h convert.h $(GFX) 
	$(CC) test.c $(OBJS) $(SWF) $(GFX) ../libbase.a -o test $(LIBS)

clean: 
	rm -f *.o test stroke
