all: parser swfdump mklib tests
tests: testwrite testrewrite testpaths testreadwrite 

D=-g -pg

#compiled by ../Makefile (into librfxswf.a)
#
#abc.o: abc.c abc.h ../q.h pool.h
#	gcc $(D) -c abc.c -o abc.o
#
#pool.o: pool.c pool.h
#	gcc $(D) -c pool.c -o pool.o
#
#files.o: files.h files.c
#	gcc $(D) -c files.c -o files.o
#
#registry.o: registry.h registry.c
#	gcc $(D) -c registry.c -o registry.o
#
#tokenizer.yy.o: tokenizer.yy.c tokenizer.h parser.tab.h
#	gcc $(D) -c tokenizer.yy.c -o tokenizer.yy.o
#
#parser.tab.o: parser.tab.c parser.tab.h parser.h
#	gcc $(D) -c parser.tab.c -o parser.tab.o


tokenizer.yy.c: tokenizer.lex tokenizer.h
	flex -Pavm2_ -8 -B -otokenizer.yy.c tokenizer.lex

parser.tab.h parser.tab.c: parser.y parser.h
	bison -v --defines -pavm2_ parser.y -o parser.tab.c

main.o: main.c parser.tab.h parser.h
	gcc $(D) -c main.c -o main.o

mklib.o: mklib.c parser.tab.h parser.h
	gcc $(D) -c mklib.c -o mklib.o

parser: main.o tokenizer.yy.o ../librfxswf.a ../libbase.a parser.tab.h 
	gcc $(D) main.o ../librfxswf.a ../libbase.a -o parser -lz

mklib: mklib.o ../librfxswf.a ../libbase.a
	gcc $(D) mklib.o ../librfxswf.a ../libbase.a -o mklib -lz

../q.o: ../q.c ../q.h ../mem.h
	cd ..; make q.o; cd -

../librfxswf.a: ../rfxswf.h ../rfxswf.c abc.c abc.h pool.c pool.h files.c files.h code.c code.h registry.c registry.h opcodes.c opcodes.h builtin.c builtin.h compiler.c compiler.h parser.tab.h parser.tab.c tokenizer.yy.c
	cd ..; make librfxswf.a

../libbase.a: ../q.c ../q.h ../mem.c ../mem.h
	cd ..; make libbase.a

swfdump: ../librfxswf.a ../../src/swfdump.c ../libbase.a abc.c abc.h pool.c pool.h code.c opcodes.c ../q.c ../q.h
	gcc -g ../../src/swfdump.c ../librfxswf.a ../libbase.a -ljpeg -lz -lfreetype -o swfdump

testreadwrite: testreadwrite.c ../librfxswf.a ../libbase.a
	gcc $(D) -DMAIN -o testreadwrite testreadwrite.c ../librfxswf.a ../libbase.a -lz -ljpeg

testrewrite: testrewrite.c ../librfxswf.a ../libbase.a
	gcc $(D) -DMAIN -o testrewrite testrewrite.c ../librfxswf.a ../libbase.a -lz -ljpeg

testwrite: testwrite.c ../librfxswf.a ../libbase.a
	gcc $(D) -DMAIN -o testwrite testwrite.c ../librfxswf.a ../libbase.a -lz -ljpeg

testpaths: testpaths.c ../librfxswf.a ../libbase.a
	gcc $(D) -DMAIN -o testpaths testpaths.c ../librfxswf.a ../libbase.a -lz -ljpeg

clean:
	rm -f *.o *.yy.c *.tab.c *.tab.h testreadwrite swfdump testpaths testwrite ../librfxswf.a

.PHONY: tests
