all: test speedtest stroke
include ../../Makefile.common

CC = gcc -DCHECKS -O2 -g -pg
CCO = gcc -O2 -fno-inline -g -pg

../libbase.a: ../q.c ../q.h ../mem.c ../mem.h
	cd ..; make libbase.a

../libgfx.a: ../gfxtools.h ../gfxtools.c ../gfxpoly.h ../gfxpoly.c
	cd ..; make libgfx.a

testheap: ../libbase.a testheap.c
	$(CC) testheap.c ../libbase.a -o testheap -lm -lz -ljpeg

SRC = active.c convert.c poly.c wind.c renderpoly.c xrow.c stroke.c moments.c
OBJS = active.o convert.o poly.o wind.o renderpoly.o xrow.o stroke.o moments.o

active.o: active.c active.h poly.h Makefile
	$(CC) -c active.c -o active.o

convert.o: convert.c convert.h poly.h Makefile
	$(CC) -c convert.c -o convert.o

poly.o: poly.c poly.h active.h heap.h ../q.h Makefile
	$(CC) -c poly.c -o poly.o

wind.o: wind.c wind.h poly.h Makefile
	$(CC) -c wind.c -o wind.o

renderpoly.o: renderpoly.c wind.h poly.h renderpoly.h Makefile
	$(CC) -c renderpoly.c -o renderpoly.o

xrow.o: xrow.c xrow.h ../q.h ../mem.h Makefile
	$(CC) -c xrow.c -o xrow.o

stroke.o: stroke.c poly.h convert.h wind.h Makefile
	$(CC) -c stroke.c -o stroke.o

moments.o: moments.c moments.h ../q.h ../mem.h Makefile
	$(CC) -c moments.c -o moments.o

GFX=../gfxfont.o ../gfxtools.o ../gfximage.o ../devices/ops.o ../devices/polyops.o ../devices/text.o ../devices/bbox.o ../devices/render.o ../devices/rescale.o ../devices/record.o
stroke: test_stroke.c $(OBJS) ../libgfxswf.a ../librfxswf.a ../libbase.a 
	$(CC) test_stroke.c $(OBJS) ../libgfxswf.a ../librfxswf.a $(GFX) ../libbase.a -o stroke $(LIBS)

SWF = ../librfxswf.a ../libgfxpdf.a -lstdc++
test: ../libbase.a test.c $(OBJS) poly.h convert.h $(GFX)  Makefile
	$(CC) test.c $(OBJS) $(SWF) $(GFX) ../libbase.a -o test $(LIBS)

speedtest: ../libbase.a speedtest.c $(SRC) poly.h convert.h $(GFX) 
	$(CCO) speedtest.c $(SRC) $(GFX) ../libbase.a -o speedtest $(LIBS)

clean: 
	rm -f *.o test stroke
