# simple pmake detecting Makefile

all: clear  test.o
clean: clear
install:
uninstall:

CC=@echo "*******************************************************";\
    echo "Your make is broken- try to get yourself a working one.";\
    echo "gmake 3.79.1 or newer should be fine.                  ";\
    echo "*******************************************************";\
    exit 1;exec >/dev/null;echo

clear:
	@rm -f test.c

%.o: %.c

test.c:
	@touch test.c

test.o: test.c

.SUFFIXES: .o .cpp
