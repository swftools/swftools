AC_DEFUN([RFX_CHECK_SYSTEM_BACKTICKS],
[
AC_MSG_CHECKING([whether system() can handle command substitution])
AC_TRY_RUN([
#include "stdlib.h"
int main (int argc, char*argv[])
{
    return system("test `touch config.tmp2`");
}
], [OK=OK],,[CROSSCOMPILE=1])

if test "x${OK}" = "xOK";then
    test -f config.tmp2 || OK=
fi

if test "x${CROSSCOMPILE}" = "x1";then
    OK=no;
fi
if test "x${OK}" = "xOK";then
rm -f config.tmp2
AC_MSG_RESULT(yes)
SYSTEM_BACKTICKS=1
export SYSTEM_BACKTICKS
AC_DEFINE([SYSTEM_BACKTICKS], [1], [Define if system handles command substitution])
else
AC_MSG_RESULT(no)
fi
])


