# Makefile

RFXSWF	= ../lib/librfxswf.a
LDLIBS	= -L/usr/X11R6/lib -ljpeg -lm -lz -lX11
#CFLAGS	= -funsigned-char
CC	= gcc
CXX	= g++
DBFLAGS	= -g2

%.o: %.c
		$(CC) -c $(CFLAGS) $(DBFLAGS) -o $@ $<
%.o: %.cc
		$(CXX) -c $(CFLAGS) $(DBFLAGS) -o $@ $<

all: avi2swf

avi2swf.o: avi2swf.cc
		$(CXX) -O2 -fomit-frame-pointer -o avi2swf.o -c avi2swf.cc `avifile-config --cflags`

avi2swf: $(RFXSWF) avi2swf.o
		$(CXX) -O2 -fomit-frame-pointer -o avi2swf avi2swf.o  $(RFXSWF) $(LDLIBS) `avifile-config --libs`

