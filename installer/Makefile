CC=/opt/xmingw/bin/i386-mingw32msvc-gcc -DWIN32
WINDRES=/opt/xmingw/bin/i386-mingw32msvc-windres

all: installer.exe

LIBS=-lgdi32 -lshlwapi -lcomctl32 -lshell32 -lole32

crnfiles.o: crnfiles.c
	$(CC) -c crnfiles.c

depack.o: depack.c depack.h Makefile
	$(CC) -c depack.c -o depack.o

archive.o: archive.c archive.c depack.h
	$(CC) -c archive.c -o archive.o

os.o: ../lib/os.c ../lib/os.h
	$(CC) -c ../lib/os.c -o os.o

installer.coff: installer.rc
	$(WINDRES) installer.rc -O coff -F pe-i386 -o installer.coff

installer.exe: installer.c depack.o archive.o depack.h crnfiles.o os.o installer.coff Makefile
	$(CC) installer.c depack.o archive.o crnfiles.o os.o installer.coff -o installer.exe $(LIBS)
