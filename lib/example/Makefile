# Makefile

RFXSWF	= ../librfxswf.a
LDLIBS	= -L/usr/X11R6/lib -ljpeg -lm -lz -lX11 -lfreetype -lt1
#CFLAGS	= -funsigned-char
CC	= gcc
CXX	= g++
DBFLAGS	= -g2

%.o: %.c
		$(CC) -c $(CFLAGS) $(DBFLAGS) -o $@ $<
%.o: %.cc
		$(CXX) -c $(CFLAGS) $(DBFLAGS) -o $@ $<
%.swf: %
		./$<

all: sound jpegtest box shape1 transtest zlibtest sprites buttontest dumpfont text glyphshape edittext protect

movies: sound.swf jpegtest.swf box.swf shape1.swf transtest.swf zlibtest.swf sprites.swf buttontest.swf dumpfont.swf text.swf glyphshape.swf edittext.swf
	
sound: $(RFXSWF) sound.o
		$(CC) -o sound sound.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

box: $(RFXSWF) box.o
		$(CC) -o box box.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

protect: $(RFXSWF) protect.o
		$(CC) -o protect protect.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

buttontest: $(RFXSWF) buttontest.o
buttontest: $(RFXSWF) buttontest.o
		$(CC) -o buttontest buttontest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

dumpfont: $(RFXSWF) dumpfont.o
		$(CC) -o dumpfont dumpfont.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

glyphshape: $(RFXSWF) glyphshape.o
		$(CC) -o glyphshape glyphshape.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

jpegtest: $(RFXSWF) jpegtest.o
		$(CC) -o jpegtest jpegtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

shape1: $(RFXSWF) shape1.o
		$(CC) -o shape1 shape1.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

sprites: $(RFXSWF) sprites.o
		$(CC) -o sprites sprites.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

transtest: $(RFXSWF) transtest.o
		$(CC) -o transtest transtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

text: $(RFXSWF) text.o
		$(CC) -o text text.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

edittext: $(RFXSWF) edittext.o
		$(CC) -o edittext edittext.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

zlibtest: $(RFXSWF) zlibtest.o
		$(CC) -o zlibtest zlibtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

clean:
		rm -f jpegtest.o box.o shape1.o transtest.o zlibtest.o \
                sprites.o glyphshape.o edittext.o \
		buttontest.o dumpfont.o text.o \
		jpegtest.swf box.swf shape1.swf transtest.swf zlibtest.swf \
                sprites.swf buttontest.swf text.swf glyphshape.swf sound.swf \
		transtest.swf

.SUFFIXES:
