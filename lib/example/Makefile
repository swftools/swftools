# Makefile

RFXSWF	= ../rfxswf.o
LDLIBS	= -ljpeg -lm -lz
CFLAGS	= -funsigned-char
CC	= gcc
DBFLAGS	= -g2

.c.o:
		$(CC) -c $(CFLAGS) $(DBFLAGS) -o $@ $<

all: jpegtest box shape1 transtest zlibtest

box: $(RFXSWF) box.o
		$(CC) -o box box.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

jpegtest: $(RFXSWF) jpegtest.o
		$(CC) -o jpegtest jpegtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

shape1: $(RFXSWF) shape1.o
		$(CC) -o shape1 shape1.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

transtest: $(RFXSWF) transtest.o
		$(CC) -o transtest transtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

zlibtest: $(RFXSWF) zlibtest.o
		$(CC) -o zlibtest zlibtest.o $(RFXSWF) $(LDLIBS) $(DBFLAGS)

clean:
		rm -f jpegtest.o box.o shape1.o trastest.o zlibtest.o \
		jpegtest.swf box.swf shape1.swf trastest.swf zlibtest.swf


