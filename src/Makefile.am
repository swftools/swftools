## Process this file with automake to produce Makefile.in
bin_PROGRAMS = swfcombine swfstrings swfdump
swfdump_SOURCES = swfdump.c
swfcombine_SOURCES = bitio.c bitio.h reloc.c reloc.h combine.c combine.h flash.c flash.h swfcombine.c types.h
swfstrings_SOURCES = swfstrings.c
swfdump_LINK = $(CCLD) ../lib/rfxswf.o -o $@
swfcombine_LINK = $(CCLD) ../lib/log.o -o $@
swfstrings_LINK = $(CCLD) ../lib/log.o ../lib/rfxswf.o -o $@
##LIBS = 
##swfcombine_LIBS =
##swfstrings_LIBS = -ljpeg
man_MANS = swfcombine.1 swfstrings.1 swfdump.1

