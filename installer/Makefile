CC=/opt/xmingw/bin/i386-mingw32msvc-gcc -DWIN32 -Wno-trigraphs
STRIP=/opt/xmingw/bin/i386-mingw32msvc-strip 
WINDRES=/opt/xmingw/bin/i386-mingw32msvc-windres

LICENSE=gpl.c

all: files1.exe files2.exe uninstall.exe mkarchive

LIBS=-lgdi32 -lshlwapi -lcomctl32 -lshell32 -lole32 -luuid

files1.c: ../win32/*.exe mkarchive uninstall.exe
	cp uninstall.exe ../win32/
	CWD=`pwd`
	cd ../win32;../installer/mkarchive `find`
	mv ../win32/crnfiles.c ./files1.c

files2.c: ../win32_gui/*.exe ../win32_gui/*.swf mkarchive uninstall.exe
	cp uninstall.exe ../win32_gui/
	CWD=`pwd`
	cd ../win32_gui;../installer/mkarchive `find`
	mv ../win32_gui/crnfiles.c ./files2.c

%.o: %.c
	$(CC) -c $<

mkarchive: mkarchive.c
	gcc mkarchive.c -o mkarchive -lz

crnfiles1.o: crnfiles1.c
crnfiles2.o: crnfiles2.c

#depack.o: depack.c depack.h Makefile
#	$(CC) -c depack.c -o depack.o

archive.o: archive.c archive.c
	$(CC) -c archive.c -o archive.o

utils.o: utils.c utils.c
	$(CC) -c utils.c -o utils.o

license.o: $(LICENSE)
	$(CC) -c $< -o license.o

installer.o: installer.c installer.h archive.h
	$(CC) -c installer.c -o installer.o

uninstaller.o: installer.c installer.h
	$(CC) -DDEINSTALL -c installer.c -o uninstaller.o

installer.coff: installer.rc swftools.ico installer.h
	$(WINDRES) installer.rc -O coff -F pe-i386 -o installer.coff

lzma/LzmaDecode.o: lzma/LzmaDecode.c lzma/LzmaDecode.h lzma/LzmaTypes.h
	$(CC) -c lzma/LzmaDecode.c -o lzma/LzmaDecode.o

uninstall.exe: uninstaller.o utils.o installer.coff Makefile
	$(CC) uninstaller.o utils.o installer.coff -o $@ $(LIBS)
	$(STRIP) $@
	./makegui $@
	#upx -9 --best $@ 2>/dev/null || true

%.exe: %.o installer.o license.o archive.o utils.o lzma/LzmaDecode.o installer.coff Makefile
	$(CC) installer.o license.o utils.o archive.o lzma/LzmaDecode.o installer.coff $< -o $@ $(LIBS)
	$(STRIP) $@
	./makegui $@

#installer.exe: installer.c depack.o archive.o depack.h crnfiles.o installer.coff Makefile
#	$(CC) installer.c depack.o archive.o crnfiles.o installer.coff -o installer.exe $(LIBS)
#	$(STRIP) installer.exe
#	#./makegui installer.exe
#	#upx -9 --best installer.exe 2>/dev/null || true

clean:
	rm -f installer.exe files*exe crnfiles.c *.o *.coff
