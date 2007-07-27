CC=/opt/xmingw/bin/i386-mingw32msvc-gcc -DWIN32
STRIP=/opt/xmingw/bin/i386-mingw32msvc-strip 
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

installer.coff: installer.rc swftools.ico
	$(WINDRES) installer.rc -O coff -F pe-i386 -o installer.coff

installer.exe: installer.c depack.o archive.o depack.h crnfiles.o os.o installer.coff Makefile
	$(CC) installer.c depack.o archive.o crnfiles.o os.o installer.coff -o installer.exe $(LIBS)
	$(STRIP) installer.exe
	./makegui installer.exe
	upx -9 --best installer.exe 2>/dev/null || true

crnfiles.c: ../win32/*.exe
	CWD=`pwd`
	cd ../win32;../installer/mkarchive `find`
	mv ../win32/crnfiles.c ./

clean:
	rm -f installer.exe crnfiles.c *.o *.coff
