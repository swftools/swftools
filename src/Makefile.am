## Process this file with automake to produce Makefile.in
bin_PROGRAMS = swfcombine swfstrings swfdump jpeg2swf
jpeg2swf_SOURCES = jpeg2swf.c
swfdump_SOURCES = swfdump.c
swfcombine_SOURCES = bitio.c bitio.h reloc.c reloc.h combine.c combine.h flash.c flash.h swfcombine.c types.h
swfstrings_SOURCES = swfstrings.c
jpeg2swf_LINK = $(CCLD) ../lib/rfxswf.o -o $@
swfdump_LINK = $(CCLD) ../lib/rfxswf.o -o $@
swfcombine_LINK = $(CCLD) ../lib/log.o -o $@
swfstrings_LINK = $(CCLD) ../lib/log.o ../lib/rfxswf.o -o $@
man_MANS = swfcombine.1 swfstrings.1 swfdump.1

