AC_DEFUN([RFX_CHECK_BYTEORDER],
[
AC_MSG_CHECKING([for byte order])
AC_TRY_RUN([
int main (int argc, char *argv[])
{
    int i = 1;
    return *(char*)&i;
}
], [BIGENDIAN=1], [LITTLEENDIAN=1], [CROSSCOMPILE=1])
export CROSSCOMPILE 

if test "x${CROSSCOMPILE}" = "x1"; then
 AC_MSG_RESULT([we are cross compiling- trying to guess from system type])
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
AC_MSG_RESULT([little endian])
BYTEORDERCHECKOK=1
fi
export BIGENDIAN
if test "x${BIGENDIAN}" = "x1"; then
AC_MSG_RESULT([big endian])
BYTEORDERCHECKOK=1
WORDS_BIGENDIAN=1
export WORDS_BIGENDIAN
AC_DEFINE([WORDS_BIGENDIAN], [1], [Define to 1 if this machine has network byte order])
fi
if test "x${BYTEORDERCHECKOK}" != "x1"; then
AC_MSG_RESULT(unknown)
echo Byte order could not determined.
exit 1
fi
])


