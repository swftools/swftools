all: mypython SWF.so

primitives.o: primitives.c primitives.h
	gcc -g -O2 -Wall -Wno-unused -fPIC -I/usr/include/python2.2 -c primitives.c -o primitives.o
pyutils.o: pyutils.c pyutils.h
	gcc -g -O2 -Wall -Wno-unused -fPIC -I/usr/include/python2.2 -c pyutils.c -o pyutils.o
SWF.o: SWF.c pyutils.h primitives.h
	gcc -g -O2 -Wall -Wno-unused -fPIC -I/usr/include/python2.2 -c SWF.c -o SWF.o

SWF.so: primitives.o pyutils.o SWF.o
	gcc -shared primitives.o SWF.o pyutils.o -o SWF.so -lpython2.2 ../librfxswf.a -ljpeg -lz

mypython: mypython.c
	gcc -I/usr/include/python2.2 mypython.c -o mypython -lpython2.2

test: SWF.so
	python fonts.py
