all: mypython SWF.so

LIB=-L/usr/lib/python2.2/config/ -lpython2.2
I=-I/usr/include/python2.2

primitives.o: primitives.c primitives.h
	gcc -g -O2 -Wall -Wno-unused -fPIC $(I) -c primitives.c -o primitives.o
tagmap.o: tagmap.c tagmap.h
	gcc -g -O2 -Wall -Wno-unused -fPIC $(I) -c tagmap.c -o tagmap.o
taglist.o: taglist.c taglist.h tagmap.h tag.h
	gcc -g -O2 -Wall -Wno-unused -fPIC $(I) -c taglist.c -o taglist.o
tag.o: tag.c tag.h tagmap.h
	gcc -g -O2 -Wall -Wno-unused -fPIC $(I) -c tag.c -o tag.o
action.o: action.c action.h
	gcc -g -O2 -Wall -Wno-unused -fPIC $(I) -c action.c -o action.o
pyutils.o: pyutils.c pyutils.h
	gcc -g -O2 -Wall -Wno-unused -fPIC $(I) -c pyutils.c -o pyutils.o

SWF.o: SWF.c pyutils.h primitives.h taglist.h
	gcc -g -O2 -Wall -Wno-unused -fPIC $(I) -c SWF.c -o SWF.o

SWF.so: SWF.o taglist.o tag.o tagmap.o action.o primitives.o pyutils.o ../librfxswf.a
	gcc -shared SWF.o tagmap.o taglist.o tag.o action.o primitives.o pyutils.o  -o SWF.so ../librfxswf.a -ljpeg -lz -lt1 -lfreetype

mypython: mypython.c
	gcc -g -I/usr/include/python2.2 mypython.c -o mypython $(LIB) -ldl -lm -lpthread -lutil

test: SWF.so
	#./test_create.py
	#swfdump -p test/test.swf
	./test_combine.py
	swfdump -p tmp/combined.swf
clean:
	rm *.o *.so mypython

.PHONY: test
