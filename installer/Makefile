CC=/opt/xmingw/bin/i386-mingw32msvc-gcc -DWIN32
STRIP=/opt/xmingw/bin/i386-mingw32msvc-strip 
WINDRES=/opt/xmingw/bin/i386-mingw32msvc-windres

LICENSE=gpl.c

all: files1.exe files2.exe

LIBS=-lgdi32 -lshlwapi -lcomctl32 -lshell32 -lole32 -luuid

files1.c: ../win32/*.exe
	CWD=`pwd`
	cd ../win32;../installer/mkarchive `find`
	mv ../win32/crnfiles.c ./files1.c

files2.c: ../win32_gui/*.exe ../win32_gui/*.swf
	CWD=`pwd`
	cd ../win32_gui;../installer/mkarchive `find`
	mv ../win32_gui/crnfiles.c ./files2.c

%.o: %.c
	$(CC) -c $<

crnfiles1.o: crnfiles1.c
crnfiles2.o: crnfiles2.c

depack.o: depack.c depack.h Makefile
	$(CC) -c depack.c -o depack.o

archive.o: archive.c archive.c depack.h
	$(CC) -c archive.c -o archive.o

license.o: $(LICENSE)
	$(CC) -c $< -o license.o

installer.o: installer.c installer.h depack.h archive.h ../lib/os.h
	$(CC) -c installer.c -o installer.o

os.o: ../lib/os.c ../lib/os.h
	$(CC) -c ../lib/os.c -o os.o

installer.coff: installer.rc swftools.ico installer.h
	$(WINDRES) installer.rc -O coff -F pe-i386 -o installer.coff

%.exe: %.c installer.o license.o depack.o archive.o depack.h os.o installer.coff Makefile
	$(CC) installer.o depack.o license.o archive.o os.o installer.coff $< -o $@ $(LIBS)
	$(STRIP) $@
	./makegui $@
	#upx -9 --best $@ 2>/dev/null || true

#installer.exe: installer.c depack.o archive.o depack.h crnfiles.o os.o installer.coff Makefile
#	$(CC) installer.c depack.o archive.o crnfiles.o os.o installer.coff -o installer.exe $(LIBS)
#	$(STRIP) installer.exe
#	#./makegui installer.exe
#	#upx -9 --best installer.exe 2>/dev/null || true

clean:
	rm -f installer.exe files*exe crnfiles.c *.o *.coff
