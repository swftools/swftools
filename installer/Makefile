CC=/opt/xmingw/bin/i386-mingw32msvc-gcc -DWIN32

all: installer.exe

LIBS=-lgdi32 -lshlwapi 

crnfiles.o: crnfiles.c
	$(CC) -c crnfiles.c

depack.o: depack.c depack.h Makefile
	$(CC) -c depack.c -o depack.o

archive.o: archive.c archive.c depack.h
	$(CC) -c archive.c -o archive.o

installer.exe: installer.c depack.o archive.o depack.h crnfiles.o Makefile
	$(CC) installer.c depack.o archive.o crnfiles.o -o installer.exe $(LIBS)

