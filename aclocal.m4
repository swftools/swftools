AC_DEFUN(SWF_CHECK_BYTEORDER,
[
AC_MSG_CHECKING([for byte order])
AC_TRY_RUN([
int main (int argc, char *argv[])
{
    int i = 1;
    return *(char*)&i;
}
], BIGENDIAN=1, LITTLEENDIAN=1, CROSSCOMPILE=1)
export CROSSCOMPILE 

if test "x${CROSSCOMPILE}" = "x1"; then
 AC_MSG_RESULT(we are cross compiling- trying to guess from system type)
 AC_CANONICAL_SYSTEM
 AC_MSG_CHECKING([for byte order, try 2])
 case "${target}" in
  *86* | *-pc-* )
     LITTLEENDIAN=1
     ;;
  *sparc* | *68* | *88k* | *mac* | *Mac* | *sun* | *Sun* | *Amiga* | *amiga* )
     BIGENDIAN=1
     ;;
 esac
fi

export LITTLEENDIAN
if test "x${LITTLEENDIAN}" = "x1"; then
AC_MSG_RESULT(little endian)
BYTEORDERCHECKOK=1
fi
export BIGENDIAN
if test "x${BIGENDIAN}" = "x1"; then
AC_MSG_RESULT(big endian)
BYTEORDERCHECKOK=1
WORDS_BIGENDIAN=1
export WORDS_BIGENDIAN
AC_DEFINE_UNQUOTED(WORDS_BIGENDIAN, 1)
fi
if test "x${BYTEORDERCHECKOK}" != "x1"; then
AC_MSG_RESULT(unknown)
echo Byte order could not determined. Try to insert your system type into aclocal.m4 about line 21
exit 1
fi
])

AC_DEFUN(SWF_CHECK_SYSTEM_BACKTICKS,
[
AC_MSG_CHECKING([whether system() can handle command substitution])
AC_TRY_RUN([
#include "stdlib.h"
int main (int argc, char*argv[])
{
    return system("echo `touch config.tmp2`");
}
], OK=OK,,CROSSCOMPILE=1)

if test "x${OK}" = "xOK";then
    test -e config.tmp2 || OK=
fi

if test "x${CROSSCOMPILE}" = "x1";then
    OK=OK;
fi
if test "x${OK}" = "xOK";then
rm -f config.tmp2
AC_MSG_RESULT(yes)
export SYSTEM_BACKTICKS=1
AC_DEFINE_UNQUOTED(SYSTEM_BACKTICKS, 1)
else
AC_MSG_RESULT(no)
fi
])

