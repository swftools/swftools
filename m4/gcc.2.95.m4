AC_DEFUN([RFX_CHECK_OLDGCC],
[
AC_MSG_CHECKING([whether gcc supports lazy variable declaration])

cat > conftest.c << EOF
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char*argv[])
{
    int a,b;
    b=3;
    int c;
    c=4;
    return 0;
}
EOF

testprog_link='$CC $CPPFLAGS $CFLAGS conftest.c -o conftest${ac_exeext}'
if { (eval echo gcc.2.95.m4:19: \"$testprog_link\") 1>&5; (eval $testprog_link) 2>&5; } && test -s conftest${ac_exeext}; then
  AC_MSG_RESULT(yes)
  GCC_IS_OK=true
  export GCC_IS_OK
else
  echo "configure: failed program was:" >&5
  cat conftest.c >&5
  AC_MSG_RESULT(no)
fi
rm -f conftest*
])
