## Process this file with automake to produce Makefile.in
SUBDIRS = xpdf fonts
bin_PROGRAMS = pdf2swf
LDADD = 
pdf2swf_SOURCES = pdf2swf.cc pdfswf.cc pdfswf.h swfoutput.cc swfoutput.h spline.cc spline.h
pdf2swf_LDADD = ../lib/log.o ../lib/rfxswf.o xpdf/libpdf.a
INCLUDES = -I../lib -I./xpdf
man_MANS = pdf2swf.1
CXXCOMPILE = $(CXX) -DDATADIR=\"$(pkgdatadir)\" $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)

