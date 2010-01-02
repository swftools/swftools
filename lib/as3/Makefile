include ../../Makefile.common

all: parser swfdump mklib
tests: testwrite testrewrite testpaths testreadwrite 

D=-g -pg

#BISONDEBUG=yes

MODULES = abc.o opcodes.o code.o pool.o scripts.o expr.o common.o initcode.o
SOURCES = abc.c abc.h pool.c pool.h files.c files.h code.c code.h registry.c registry.h opcodes.c opcodes.h builtin.c builtin.h compiler.c compiler.h parser.tab.h parser.tab.c tokenizer.yy.c scripts.c import.c import.h expr.c expr.h common.c common.h initcode.c initcode.h assets.c assets.h

tokenizer.yy.c: tokenizer.lex tokenizer.h
	flex -Pas3_ -8 -B -otokenizer.yy.c tokenizer.lex

ifeq "$(BISONDEBUG)" "yes"
BISONDEBUGFLAGS=-t
BISONDEBUGDEFINE=-DBISONDEBUG
endif
parser.tab.h parser.tab.c: parser.y parser.h skeleton.m4 Makefile
	bison $(BISONDEBUGFLAGS) -S ./skeleton.m4 -v --defines -pa3_ parser.y -o parser.tab.c

main.o: main.c parser.tab.h parser.h
	$(C) $(BISONDEBUGDEFINE) main.c -o main.o

mklib.o: mklib.c parser.tab.h parser.h
	$(C) mklib.c -o mklib.o

parser: main.o tokenizer.yy.c ../librfxswf.a ../libbase.a parser.tab.h 
	$(L) main.o ../librfxswf.a ../libbase.a -o parser -lz

rfxswf_modules=../drawer.o ../rfxswf.o ../modules/*.o ../lame/*.o ../action/*.o ../MD5.o $(MODULES)
mklib: mklib.o $(rfxswf_modules) import.o registry.o tokenizer.yy.o parser.tab.o files.o compiler.o assets.o ../libbase.a
	$(L) mklib.o $(rfxswf_modules) import.o registry.o tokenizer.yy.o parser.tab.o files.o compiler.o assets.o ../libbase.a -o mklib $(LIBS)

../q.o: ../q.c ../q.h ../mem.h
	cd ..; make q.o; cd -

../librfxswf.a: ../rfxswf.h ../rfxswf.c $(SOURCES)
	cd ..; make librfxswf.a

../libbase.a: ../q.c ../q.h ../mem.c ../mem.h
	cd ..; make libbase.a

swfdump: ../librfxswf.a ../../src/swfdump.c ../libbase.a $(SOURCES) ../q.c ../q.h
	$(L) -g ../../src/swfdump.c ../librfxswf.a ../libbase.a -o swfdump $(LIBS)

pdf2swf: ../librfxswf.a ../../src/pdf2swf.c ../libpdf.a ../libbase.a
	$(LL) -g ../../src/pdf2swf.c ../libgfxswf.a ../libpdf.a ../libgfx.a ../librfxswf.a ../libbase.a -o pdf2swf $(LIBS)

testreadwrite: testreadwrite.c ../librfxswf.a ../libbase.a
	$(L) -DMAIN -o testreadwrite testreadwrite.c ../librfxswf.a ../libbase.a $(LIBS)

testrewrite: testrewrite.c ../librfxswf.a ../libbase.a
	$(L) -DMAIN -o testrewrite testrewrite.c ../librfxswf.a ../libbase.a $(LIBS)

testwrite: testwrite.c ../librfxswf.a ../libbase.a
	$(L) -DMAIN -o testwrite testwrite.c ../librfxswf.a ../libbase.a $(LIBS)

testpaths: testpaths.c ../librfxswf.a ../libbase.a
	$(L) -DMAIN -o testpaths testpaths.c ../librfxswf.a ../libbase.a $(LIBS)

clean:
	rm -f *.o *.yy.c *.tab.c *.tab.h testreadwrite swfdump testpaths testwrite ../librfxswf.a

.PHONY: tests
