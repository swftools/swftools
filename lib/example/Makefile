# Makefile

RFXSWF	= ../librfxswf.a
LDLIBS	= -L/usr/X11R6/lib -ljpeg -lm -lz -lX11
#CFLAGS	= -funsigned-char
CC	= gcc
CXX	= g++
DBFLAGS	= -g2

%.o: %.c
		$(CC) -c $(CFLAGS) $(DBFLAGS) -o $@ $<
%.o: %.cc
		$(CXX) -c $(CFLAGS) $(DBFLAGS) -o $@ $<

all: sound jpegtest box shape1 transtest zlibtest sprites buttontest dumpfont text makefonts glyphshape

movies: sound jpegtest box shape1 transtest zlibtest sprites buttontest dumpfont text glyphshape
		./sound
		./jpegtest
		./box
		./glyphshape
		./shape1
		./transtest
		./zlibtest
		./sprites
		./buttontest
		./text
		./makefonts

sound: $(RFXSWF) sound.o
		$(CC) -o sound sound.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

box: $(RFXSWF) box.o
		$(CC) -o box box.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

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

zlibtest: $(RFXSWF) zlibtest.o
		$(CC) -o zlibtest zlibtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

makefonts: $(RFXSWF) makefonts.o ../../pdf2swf/swfoutput.o
		$(CXX) -o makefonts makefonts.o ../../pdf2swf/swfoutput.o \
                ../../pdf2swf/spline.o ../librfxswf.a -lt1 $(LDLIBS) \
                ../../pdf2swf/xpdf/libpdf.a $(DBFLAGS)

clean:
		rm -f jpegtest.o box.o shape1.o transtest.o zlibtest.o \
                sprites.o glyphshape.o \
		buttontest.o dumpfont.o text.o makefonts.o\
		jpegtest.swf box.swf shape1.swf trastest.swf zlibtest.swf \
                sprites.swf buttontest.swf text.swf glyphshape.swf


