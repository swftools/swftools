all: testheap test

CC = gcc -g -pg

../libbase.a: ../q.c ../q.h ../mem.c ../mem.h
	cd ..; make libbase.a

../libgfx.a: ../gfxtools.h ../gfxtools.c ../gfxpoly.h ../gfxpoly.c
	cd ..; make libgfx.a

testheap: ../libbase.a testheap.c
	$(CC) testheap.c ../libbase.a -o testheap -lm -lz -ljpeg

active.o: active.c active.h poly.h
	$(CC) -c active.c -o active.o

convert.o: convert.c convert.h poly.h
	$(CC) -c convert.c -o convert.o

poly.o: poly.c poly.h active.h ../q.h
	$(CC) -c poly.c -o poly.o

xrow.o: xrow.c xrow.h ../q.h ../mem.h
	$(CC) -c xrow.c -o xrow.o

SWF = ../librfxswf.a
test: ../libbase.a ../libgfx.a test.c poly.o convert.o active.o xrow.o poly.h convert.h
	$(CC) test.c poly.o convert.o active.o xrow.o $(SWF) ../libbase.a ../libgfx.a -o test -lm -lz -ljpeg -lfreetype

clean: 
	rm -f *.o test
