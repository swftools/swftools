## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libpdf.a
libpdf_a_SOURCES = Array.cc Catalog.cc \
Decrypt.cc Dict.cc Error.cc FontEncoding.cc FontFile.cc \
FormWidget.cc GString.cc Gfx.cc GfxFont.cc GfxState.cc Lexer.cc \
Link.cc Object.cc OutputDev.cc PDFDoc.cc Page.cc Params.cc \
Parser.cc Stream.cc XRef.cc gfile.cc gmem.cc ##parseargs.cc 
## this is a workaround. automake always tries to use $(COMPILE)
## for .c sources, which is not defined
COMPILE = $(CC)
