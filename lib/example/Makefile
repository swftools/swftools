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

all: sound jpegtest box shape1 transtest zlibtest sprites buttontest dumpfont text glyphshape edittext protect hexfont

movies: sound.swf jpegtest.swf box.swf shape1.swf transtest.swf zlibtest.swf sprites.swf buttontest.swf dumpfont.swf text.swf glyphshape.swf edittext.swf
	
sound: $(RFXSWF) sound.o $(RFXSWF)
		$(CC) -o sound sound.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

box: $(RFXSWF) box.o $(RFXSWF)
		$(CC) -o box box.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

protect: $(RFXSWF) protect.o $(RFXSWF)
		$(CC) -o protect protect.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

buttontest: $(RFXSWF) buttontest.o $(RFXSWF)
		$(CC) -o buttontest buttontest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

dumpfont: $(RFXSWF) dumpfont.o $(RFXSWF)
		$(CC) -o dumpfont dumpfont.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

glyphshape: $(RFXSWF) glyphshape.o $(RFXSWF)
		$(CC) -o glyphshape glyphshape.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

jpegtest: $(RFXSWF) jpegtest.o $(RFXSWF)
		$(CC) -o jpegtest jpegtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

shape1: $(RFXSWF) shape1.o $(RFXSWF)
		$(CC) -o shape1 shape1.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

sprites: $(RFXSWF) sprites.o $(RFXSWF)
		$(CC) -o sprites sprites.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

transtest: $(RFXSWF) transtest.o $(RFXSWF)
		$(CC) -o transtest transtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

hexfont: $(RFXSWF) hexfont.o $(RFXSWF)
		$(CC) -o hexfont hexfont.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

text.o: demofont.c
text: $(RFXSWF) text.o $(RFXSWF)
		$(CC) -o text text.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

edittext: $(RFXSWF) edittext.o $(RFXSWF)
		$(CC) -o edittext edittext.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

zlibtest: $(RFXSWF) zlibtest.o $(RFXSWF)
		$(CC) -o zlibtest zlibtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

clean:
		rm -f jpegtest.o box.o shape1.o transtest.o zlibtest.o \
                sprites.o glyphshape.o edittext.o \
		buttontest.o dumpfont.o text.o edittext.swf \
		jpegtest.swf box.swf shape1.swf transtest.swf zlibtest.swf \
                sprites.swf buttontest.swf text.swf glyphshape.swf sound.swf \
		transtest.swf

.SUFFIXES:
